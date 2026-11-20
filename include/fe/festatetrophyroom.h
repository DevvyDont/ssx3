#ifndef FE_FESTATETROPHYROOM_H
#define FE_FESTATETROPHYROOM_H

// cFEStateTrophyRoom: the lodge's trophy room ("127trophyroom"): one peak trophy and its four challenge rows, with a
// big picture of the trophy or the earned medal (loaded through the FE file cache at RealFE + 0x1A70) and the
// challenge stats text.
// EA's source file is not known (no assert of its own); retail code: the ctor in fe/ovstatelodge (0x001D4268), the
// methods in fe/festatetrophyroom (0x001D4368 ... 0x001D4B20), the default-ctor copy and the destructor in
// fe/fepopupmisc (0x001E01D8, 0x001E0210).
//
// Size 0xA0:
//   [R] cMemMan_alloc(0xA0, "cFEStateTrophyRoom") + cFEStateTrophyRoom_cFEStateTrophyRoom(mem, engine, userNum,
//       peakNum, trophyType) in cFECustom_getNextState; the ctor writes up to mCurSelection (0x9C).
//   [O] On Tour's class is 0x98: its mTextureIDs and mTextureFileIndex have 4 entries; retail's have 5 (the ctor sets
//       5 + 5 to -1 from 0x60 and 0x74; updateTexturesLoad loops over 5 slots; loadTextures fills [0] and [1..4]),
//       which moves mpMedalStats and mCurSelection by 8: mCurSelection is 0x9C (preview 0x98, layout_xlate).
// Base: cFEState (no data; vtable cFEState_vtbl) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The ctor
// runs cUIState_cUIState(this, pEngine), stores mHashName 45, cFEStateTrophyRoom_vtbl (0x004696B8, 24 slots: 1 dtor,
// 4 onCreateScreen, 6 onGainFocus__FPv, 12 onUpdate, 18 onWidgetCreate, 19 onWidgetEvent, 20 onWidgetMenu), mUserNum
// and mCtrlBits; the destructor purges the textures, stores cFEState_vtbl and runs cUIState__cUIState.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Evidence tags:
//   [R] retail's constructors / destructor and allocation size;
//   [P] the July 2003 preview (layout_xlate: 0x50 -> 0x54, 0x98 -> 0x9C, from onWidgetEvent);
//   [U] how retail code uses the field (the C bodies in src/fe/festatetrophyroom.cpp, fe/ovstatelodge.cpp, asm);
//   [O] SSX On Tour's DWARF (class cFEStateTrophyRoom, cUIState, cListNode): names, types and order.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIIcon;
struct cUIText;

struct cFEStateTrophyRoom {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState_cUIState links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateTrophyRoom_vtbl (ctors, dtor first), cFEState_vtbl (dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [R] the ctor stores 45
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the engine argument; [U] onCreateScreen adds "127trophyroom";
                                    //       onWidgetEvent asks the engine's cFECustom for the previous state
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]; [R] cFECustom_getPlayerCtrlMask(.., userNum)
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (word, see ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O][P] (preview 0x38); [U] onCreateScreen stores the screen
    signed char mUserNum;           // 0x044 [O]; [R] the ctor's userNum; [U] the reward / challenge queries
    char unk_045[0x3];              // 0x045 padding
    // --- cFEState (On Tour: size 0x48, no data)
    // --- cFEStateTrophyRoom
    cUIIcon* mpBigIcon;             // 0x048 [O]; [R] ctor 0; [U] onWidgetCreate binds "bitmap"; updateBigArt sets its
                                    //       texture or hides it
    cUIText* mpStats;               // 0x04C [O]; [U] onWidgetCreate binds "stats"; updateStats fills it
    int mCharID;                    // 0x050 [O]; [R] cBENewPlayerInterface_getPlayerCharID(.., mUserNum); [U] the file
                                    //       cache bank and the reward queries
    int mPeakNum;                   // 0x054 [O][P] (preview 0x50); [R] the ctor's peakNum; [U] onWidgetEvent passes it
                                    //       to cFECustom getPrevState
    int mTrophyType;                // 0x058 [O]; [R] the ctor's trophyType; [U] onWidgetCreate picks the title by it
    int mNumLoadingFiles;           // 0x05C [O]; [R] ctor 0; [U] loadTextures counts the started loads,
                                    //       updateTexturesLoad counts them down, onUpdate polls while > 0
    int mTextureIDs[5];             // 0x060 [O] (On Tour [4]); [R] ctor -1 x5; [U] updateTexturesLoad stores [i];
                                    //       updateBigArt uses [0] (trophy) or [medal + 1]
    int mTextureFileIndex[5];       // 0x074 [O] (On Tour [4]); [R] ctor -1 x5; [U] loadTextures stores the cache slots
                                    //       ([0] trophy, [1..4] medals); updateTexturesLoad / purgeTextures walk 5
    const char* mpMedalStats[5];    // 0x088 [O]; no retail access
    int mCurSelection;              // 0x09C [O][P] (preview 0x98); [R] ctor -1; [U] onWidgetEvent stores the focused
                                    //       row's misc data; updateTexturesLoad redraws it
};

CHECK_OFFSET(cFEStateTrophyRoom, mpPrev, 0x000);
CHECK_OFFSET(cFEStateTrophyRoom, mpNext, 0x004);
CHECK_OFFSET(cFEStateTrophyRoom, mVtbl, 0x008);
CHECK_OFFSET(cFEStateTrophyRoom, mHashName, 0x00C);
CHECK_OFFSET(cFEStateTrophyRoom, mpEngine, 0x010);
CHECK_OFFSET(cFEStateTrophyRoom, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateTrophyRoom, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateTrophyRoom, unk_016, 0x016);
CHECK_OFFSET(cFEStateTrophyRoom, miMiscData, 0x018);
CHECK_OFFSET(cFEStateTrophyRoom, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateTrophyRoom, mpParentState, 0x020);
CHECK_OFFSET(cFEStateTrophyRoom, mScreens, 0x024);
CHECK_OFFSET(cFEStateTrophyRoom, mpScreen, 0x040);
CHECK_OFFSET(cFEStateTrophyRoom, mUserNum, 0x044);
CHECK_OFFSET(cFEStateTrophyRoom, unk_045, 0x045);
CHECK_OFFSET(cFEStateTrophyRoom, mpBigIcon, 0x048);
CHECK_OFFSET(cFEStateTrophyRoom, mpStats, 0x04C);
CHECK_OFFSET(cFEStateTrophyRoom, mCharID, 0x050);
CHECK_OFFSET(cFEStateTrophyRoom, mPeakNum, 0x054);
CHECK_OFFSET(cFEStateTrophyRoom, mTrophyType, 0x058);
CHECK_OFFSET(cFEStateTrophyRoom, mNumLoadingFiles, 0x05C);
CHECK_OFFSET(cFEStateTrophyRoom, mTextureIDs, 0x060);
CHECK_OFFSET(cFEStateTrophyRoom, mTextureFileIndex, 0x074);
CHECK_OFFSET(cFEStateTrophyRoom, mpMedalStats, 0x088);
CHECK_OFFSET(cFEStateTrophyRoom, mCurSelection, 0x09C);
CHECK_SIZE(cFEStateTrophyRoom, 0xA0);

#endif /* FE_FESTATETROPHYROOM_H */
