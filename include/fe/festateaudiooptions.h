#ifndef FE_FESTATEAUDIOOPTIONS_H
#define FE_FESTATEAUDIOOPTIONS_H

// cFEStateAudioOptions: the front end's audio / radio-mode screen ("140audio", or "142audio_pda" in game): four music
// modes with check boxes (the selected one shows the "checkbox" texture, the others "empty box"), plus the request-line
// / playlist editing options that open cFEStateRequestLine.
// EA's source file is not known (no assert of its own; the preview keeps its inline ctor, dtor and isInStore as linkonce
// copies, so the class is declared in a header); this one is named after the retail unit. Retail code: the ctor and
// onCreateScreen ... onGainFocus at the end of util/locale (0x00195FF0 ... 0x00196378), onWidgetCreate / onWidgetEvent /
// onWidgetMenu in fe/festateaudiooptions (0x00196540 ... 0x00196B08) and the out-of-line inlines in fe/fepopupmisc
// (0x001E0510 ... 0x001E0570).
//
// Size 0x80:
//   [R] cMemMan_alloc(0x80, "cFEStateAudioOptions") + cFEStateAudioOptions_cFEStateAudioOptions(mem, engine, userNum,
//       inStore) in fe/fereal (three sites), fe/messagecenter and fe/ovtemplatepausemenu; the ctor writes up to 0x7C.
//   [O] On Tour's class is 0x80 too, with the same members at the same offsets (its two bools are followed by a word,
//       so 4-byte bools move nothing).
// Base: cFEState (On Tour 0x48, no data; vtable cFEState_vtbl) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at
// 0x8). The ctor runs cUIState_cUIState(this, pEngine) and stores mHashName 0x10 and cFEStateAudioOptions_vtbl
// (0x00469448, 24 slots: 1 dtor, 4 onCreateScreen, 5 onDestroyScreen, 6 onGainFocus, 7 onGainTransition, 11
// cFEState_onLoseFocus, 18 onWidgetCreate, 19 onWidgetEvent, 20 onWidgetMenu; the rest cUIState_* / cListNode_*); the
// destructor stores cFEState_vtbl and runs cUIState__cUIState.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet. bools are
// ints (ee-gcc 2.95's bool is 4 bytes).
// Evidence tags:
//   [R] retail's constructors / destructor and allocation size;
//   [U] how retail code uses the field (the C bodies in src/util/locale.cpp, src/fe/festateaudiooptions.cpp,
//       src/fe/fepopupmisc.cpp, and the asm);
//   [O] SSX On Tour's DWARF (class cFEStateAudioOptions, cUIState, cListNode): names, types and order.
// (The preview's isInStore reads 0x60: the preview layout differs and layout_xlate has no paired method to shift it.)

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIText;
struct cUIIcon;
struct cTextureElement;

struct cFEStateAudioOptions {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState_cUIState links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateAudioOptions_vtbl (both ctors), cFEState_vtbl (dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [R] the ctor stores 0x10
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the engine argument; the ctor reads its cFECustom (*mpEngine) and
                                    //       texture bank (+0x58); [U] onCreateScreen / onDestroyScreen / onWidgetEvent
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]; [R] cFECustom_getPlayerCtrlMask(.., userNum)
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (word, see ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen; onWidgetEvent looks objects up on it
    signed char mUserNum;           // 0x044 [O]; [R] the ctor's userNum (sb); [U] the BE calls take it (lb)
    char unk_045[0x3];              // 0x045 padding
    // --- cFEState (On Tour: size 0x48, no data)
    // --- cFEStateAudioOptions
    cUIText* mpHelpText;            // 0x048 [O]; [R] 0
    cUIIcon* mpChecks[4];           // 0x04C [O]; [R] all 0 (loop from 3 down); [U] onWidgetCreate stores the four mode
                                    //       check icons, onWidgetEvent sets their textures
    cTextureElement* mpCheckBox;    // 0x05C [O]; [R] the "checkbox" texture-bank element; [U] onWidgetCreate /
                                    //       onWidgetEvent show it on the selected mode (the C view calls it selColor)
    cTextureElement* mpEmptyBox;    // 0x060 [O]; [R] the "empty box" element; [U] ditto for the other modes (normColor)
    cUIText* mpEditRequestOption;   // 0x064 [O]; [R] 0; [U] onWidgetCreate stores it; onGainFocus / onGainTransition /
                                    //       onWidgetEvent enable or hide it
    cUIText* mpRequestLineOption;   // 0x068 [O]; [R] 0; [U] ditto
    cUIText* mpPlaylistOption;      // 0x06C [O]; [R] 0; [U] ditto
    int mCharID;                    // 0x070 [O]; [R] cBENewPlayerInterface_getPlayerCharID(.., userNum); [U] onWidgetEvent
                                    //       passes it to the reward interface
    int mInStore;                   // 0x074 [O] (bool); [R] the ctor's inStore; [U] isInStore returns it; onGainFocus /
                                    //       onGainTransition / onWidgetEvent / onWidgetMenu test it
    int mRadioMode;                 // 0x078 [O] eRadioModeID; [R] cBERewardInterface_getRadioMode(.., userNum, mCharID);
                                    //       [U] onWidgetCreate / onWidgetEvent: the selected mode
    int mCreatedRequestList;        // 0x07C [O] (bool); [R] cBERewardInterface_getNumSongsInPlayList(..) > 0; [U]
                                    //       onGainFocus / onGainTransition / onWidgetEvent / onWidgetMenu test it
};

CHECK_OFFSET(cFEStateAudioOptions, mpPrev, 0x000);
CHECK_OFFSET(cFEStateAudioOptions, mpNext, 0x004);
CHECK_OFFSET(cFEStateAudioOptions, mVtbl, 0x008);
CHECK_OFFSET(cFEStateAudioOptions, mHashName, 0x00C);
CHECK_OFFSET(cFEStateAudioOptions, mpEngine, 0x010);
CHECK_OFFSET(cFEStateAudioOptions, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateAudioOptions, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateAudioOptions, unk_016, 0x016);
CHECK_OFFSET(cFEStateAudioOptions, miMiscData, 0x018);
CHECK_OFFSET(cFEStateAudioOptions, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateAudioOptions, mpParentState, 0x020);
CHECK_OFFSET(cFEStateAudioOptions, mScreens, 0x024);
CHECK_OFFSET(cFEStateAudioOptions, mpScreen, 0x040);
CHECK_OFFSET(cFEStateAudioOptions, mUserNum, 0x044);
CHECK_OFFSET(cFEStateAudioOptions, unk_045, 0x045);
CHECK_OFFSET(cFEStateAudioOptions, mpHelpText, 0x048);
CHECK_OFFSET(cFEStateAudioOptions, mpChecks, 0x04C);
CHECK_OFFSET(cFEStateAudioOptions, mpCheckBox, 0x05C);
CHECK_OFFSET(cFEStateAudioOptions, mpEmptyBox, 0x060);
CHECK_OFFSET(cFEStateAudioOptions, mpEditRequestOption, 0x064);
CHECK_OFFSET(cFEStateAudioOptions, mpRequestLineOption, 0x068);
CHECK_OFFSET(cFEStateAudioOptions, mpPlaylistOption, 0x06C);
CHECK_OFFSET(cFEStateAudioOptions, mCharID, 0x070);
CHECK_OFFSET(cFEStateAudioOptions, mInStore, 0x074);
CHECK_OFFSET(cFEStateAudioOptions, mRadioMode, 0x078);
CHECK_OFFSET(cFEStateAudioOptions, mCreatedRequestList, 0x07C);
CHECK_SIZE(cFEStateAudioOptions, 0x80);

#endif /* FE_FESTATEAUDIOOPTIONS_H */
