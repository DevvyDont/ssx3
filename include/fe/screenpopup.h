#ifndef FE_SCREENPOPUP_H
#define FE_SCREENPOPUP_H

// cScreenPopup: the front end's generic message/confirmation popup (title, message, up to 5 options, an optional
// text-entry box that opens a cKeyboardPopup). A cUIState; its methods are in fe/fepopup.cpp. Retail vtable
// cScreenPopup_vtbl (stored by cScreenPopup_cScreenPopup and the default ctor cScreenPopup_cScreenPopup1). Created with
// cMemMan_alloc(0x360, "Confirmation Popup") + cScreenPopup_cScreenPopup (cScreenPopup_createConfirmationPopup,
// feasyncfile.cpp); cFEPopupConfirm (fememcardcallbacks.cpp) derives from it.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. Evidence tags:
//   [R] retail's constructors: cUIState(cUIEngine*, cUIState*) = cUIState_cUIState1 (ui/uiscreen.cpp),
//       cScreenPopup_cScreenPopup, the destructor cScreenPopup__cScreenPopup, and the allocation size above;
//   [P] the July 2003 preview's inline accessors, carried to retail by tools/layout_xlate.py ("seen" pairs);
//   [U] how matched retail code uses the field; widget fields name the UI object cScreenPopup_FillObjectPointers
//       binds to them (strings read from the retail ELF);
//   [O] SSX On Tour's DWARF (class cListNode / cUIState / cScreenPopup): same member order; On Tour's bools are
//       1 byte (retail's are words) and its tARGBs are 16-aligned, so its offsets drift by 0..0x14. Used only where
//       the fields on both sides are placed by other evidence.
// Vectors are EA's t3Vector (3 floats, 4-aligned) and tARGB (4 floats {A, R, G, B}); they are written as float
// arrays here because fe/fepopup.cpp defines its own t3Vector.

#include "layout.h"
#include "fe/popupinfo.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIVector;
struct cUIMenu;
struct cUIGroup;
struct cUIText;
struct cUIPair;
struct cKeyboardPopup;

// One option's confirmation request (On Tour: tConfirmElement, 0xC bytes, a tPopupElement + iDefaultOption).
// tConfirmElement: fe/popupinfo.h

struct cScreenPopup {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cScreenPopup vtable cScreenPopup_vtbl
    // --- cUIState (On Tour: size 0x48, same offsets)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] cUIEngine_addScreenByHashName
    signed char mCurrentCtrl;       // 0x014 [O]; [R] ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData lw/sw (ui/uistate.h); [U] cScreenPopup_initScreenPopup copies the popup info's miMiscData
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [P] cUIState's accessors lw/sw the word (ui/uistate.h); [R] ctor sh 0x218
    cUIState* mpParentState;        // 0x020 [O]; [R] the ctor's owner argument; [U] receives onPopupEvent (slot 17)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens (two cListNodes + the list's vptr at 0x3C);
                                    //       [R] constructed by cList_cList(self + 0x24)
    cUIScreen* mpScreen;            // 0x040 [O]; [R] ctor zeroes it; [U] cScreenPopup_onCreateScreen stores the "popup" screen
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cScreenPopup
    cUIVector* mpBackGroundWhite;   // 0x048 [O]; [R] ctor zeroes; [U] widget "whitefg"
    cUIVector* mpBackGroundRed;     // 0x04C [O]; [U] widget "redbg"
    cUIVector* mpBackGroundLarge;   // 0x050 [O]; [U] widget "Big shape"
    cUIVector* mpBackGroundLargeOutline; // 0x054 [O]; [U] widget "Big outline"
    cUIVector* mpBackGroundSmall;   // 0x058 [O]; [U] widget "Small shape"
    cUIVector* mpBackGroundSmallOutline; // 0x05C [O]; [U] widget "Small outline"
    cUIVector* mpBackGroundShadow;  // 0x060 [O]; [U] widget "Shadow"
    cUIVector* mpBackGroundGreyFade; // 0x064 [O]; [U] widget "grey background"
    cUIVector* mpBackGroundBlack;   // 0x068 [O]; [U] widget "black bg"
    cUIMenu* mpMenu;                // 0x06C [O]; [U] widget "Menu0000"; cUIMenu_setSelectedByIndex in setPrivateDefaultOption
    cUIGroup* mpButtonGroup;        // 0x070 [O]; [U] widget "buttons"
    cUIText* mpMessageText;         // 0x074 [O]; [U] widget "popupText"; cScreenPopup_setPopupMessage
    cUIText* mpPostText;            // 0x078 [O]; [U] widget "popupPost"; cScreenPopup_handlePostPending (handlePostPending)
    cUIText* mpTitleText;           // 0x07C [O]; [U] widget "popupTitle"; cScreenPopup_createTitleBar
    cUIText* mpEnterBox;            // 0x080 [O]; [U] widget "EnterTextBox"; cScreenPopup_handleEntryBox
    cUIText* mpStaticBox;           // 0x084 [O]; [U] widget "StaticTextBoxLabel"
    cUIPair* mpTextBoxPair;         // 0x088 [O]; [U] widget "TextBoxPair"; selecting it opens the keyboard
    cUIVector* mpEnterBoxBackground; // 0x08C [O]; [U] widget "EnterBackground"
    cUIText* mpOptions[5];          // 0x090 [O]; [U] widgets "Option%d"; [R] ctor zeroes 5
    int miGainTransistionCount;     // 0x0A4 [O]; [U] onGainTransition counts it from 1 up to miStartFrame
    int miStartFrame;               // 0x0A8 [O]; [U] onGainTransition: frame of a label, else 25
    float mvAnimateIncrement[3];    // 0x0AC [O] t3Vector; [U] onGainTransition: per-frame scale step (x at 0xAC, y at 0xB0)
    int mbNoEntryOnNext;            // 0x0B8 [O] (On Tour: bool); [U] cScreenPopup_onWidgetEvent sets/clears it,
                                    //       cScreenPopup_onWidgetMenu (case 6) tests it
    cPopupInfo info;                // 0x0BC [O] cPopupInfo info; [R] constructed by cPopupInfo_cPopupInfo(self + 0xBC), destroyed by
                                    //       cPopupInfo__cPopupInfo; getPopupInfo (cScreenPopup_getPopupInfo) returns self + 0xBC; retail cPopupInfo
                                    //       is 0xCC bytes (data to 0xC4, vptr cPopupInfo_vtbl at 0xC8)
    float mvPosBackWhite[3];        // 0x188 [O]; [U] FillObjectPointers: "whitefg" position
    float mvPosBackRed[3];          // 0x194 [O]; [U] "redbg" position
    float mvPosMenu[3];             // 0x1A0 [O]; [U] "Menu0000" position
    float mvPosButtons[3];          // 0x1AC [O]; [U] "buttons" position
    float mvPosTitle[3];            // 0x1B8 [O]; [U] "popupTitle" position
    float mvPosText[3];             // 0x1C4 [O]; [U] "popupText" position
    float mvPosPost[3];             // 0x1D0 [O]; [U] "popupPost" position
    float mvPosEntryText[3];        // 0x1DC [O]; [U] "EnterTextBox" position
    float mvPosTextBoxPair[3];      // 0x1E8 [O]; [U] "TextBoxPair" position
    float mvSizeBackWhite[3];       // 0x1F4 [O]; [U] "whitefg" size (vt slot 20 GetRect)
    float mvPosBackEnterBox[3];     // 0x200 [O]; [U] "EnterBackground" position
    float mvSizeBackEnterBox[3];    // 0x20C [O]; [U] "EnterBackground" size
    float mvSizeBackSmall[3];       // 0x218 [O]; [U] "Small shape" size
    float mvPosBackLarge[3];        // 0x224 [O]; [U] "Big shape" position
    float mvPosBackLargeOutline[3]; // 0x230 [O]; [U] "Big outline" position
    float mvPosBackSmall[3];        // 0x23C [O]; [U] "Small shape" position
    float mvPosBackSmallOutline[3]; // 0x248 [O]; [U] "Small outline" position
    float mvPosBackShadow[3];       // 0x254 [O]; [U] "Shadow" position
    float mvCurSize[3];             // 0x260 [O]; [U] grown by every text line (cScreenPopup_increaseBoxSize calls)
    float mvOffset[3];              // 0x26C [O]; [U] copied from the popup info's mvOffset (info + 0x9C)
    float mvCurrentScale[3];        // 0x278 [O]; [R] ctor sets (1, 1, 1); [U] passed to scaleBackground
    float mvMenuSize[3];            // 0x284 [O]; [U] setPopupMessage copies mvCurSize into it
    float mvButtonOffset[3];        // 0x290 [O]; [U] cScreenPopup_adjustMenuPosition writes it, cScreenPopup_adjustButtonPosition adds it to mvPosButtons
    float mvLargestOption[3];       // 0x29C [O]; [U] FillOptions keeps the widest option width in x
    cKeyboardPopup* mpKeyboard;     // 0x2A8 [O]; [R] ctor zeroes; [U] onWidgetEvent allocates the cKeyboardPopup into it
    cScreenPopup* mpConfirmPopup;   // 0x2AC [O]; [U] createConfirmationPopup stores the "Confirmation Popup" there
    char* mszInputText;             // 0x2B0 [P] getEntryBoxText (preview 0x16C, seen); [O] cBXString; [R] ctor stores
                                    //       the empty-string rep cBXString_EmptyString; [U] filled from the keyboard's text
    char* mszStaticText;            // 0x2B4 [O] cBXString; [R] ctor/dtor like mszInputText
    int mbShowing;                  // 0x2B8 [P] isShowing (preview 0x174, seen); [U] onCreateScreen 1, onDestroyScreen 0
    int mbFirstGainTran;            // 0x2BC [O]; [U] set on the first onGainTransition; redrawPopup checks it
    int mbPassUpdate;               // 0x2C0 [P] getPassUpdate/setPassUpdate (preview 0x178, seen); [U] onUpdate calls
                                    //       the parent's onUpdate when set
    int miDefaultOption;            // 0x2C4 [O]; [R] ctor -1; [U] from the popup info (info + 0xAC)
    int mbCenterOptions;            // 0x2C8 [O]; [U] from info + 0xB8; cScreenPopup_adjustMenuPosition centres the options when set
    int mbCenterText;               // 0x2CC [O]; [U] from info + 0xBC
    tConfirmElement mtConfirmation[5]; // 0x2D0 [O]; [R] ctor clears 5 x 12 bytes; [U] onWidgetEvent, createConfirmationPopup
    int meType;                     // 0x30C [O] ePopupType; [U] from info + 0x0; -1 = text entry
    int mbReceivePrev;              // 0x310 [O]; [R] ctor 1; [U] from info + 0xA8; onWidgetMenu case 7
    int miLayerOffset;              // 0x314 [O]; [U] from info + 0x90; cScreenPopup_adjustLayer shifts the widgets' layers by it
    float mfMinSize;                // 0x318 [O]; [U] from info + 0xB0 (minimum width)
    int meBackgroundType;           // 0x31C [O] ePopupBackground; [U] from info + 0xC0; switch 0/1/2 in scaleBackground
    int miConfimationReturnCode;    // 0x320 [O] (EA's spelling); [U] onPopupEvent sets it and reports it to the parent
    int miActiveConfirmation;       // 0x324 [O]; [U] createConfirmationPopup stores the option index
    int miMaxChars;                 // 0x328 [O]; [R] ctor 0x3F; [U] from info + 0xC4; passed to the keyboard
    float mtHighlightedColor[4];    // 0x32C [O] tARGB; [U] onWidgetMenu colours the selected entry with it
    float mtUnhighlightColor[4];    // 0x33C [O] tARGB; [U] the other entries
    float mtIndicatorColor[4];      // 0x34C [O] tARGB; no retail access seen (placed by the 0x360 size)
    float mfIndicatorAlpha;         // 0x35C [O]; no retail access seen (placed by the 0x360 size)
};

CHECK_OFFSET(cScreenPopup, mpPrev, 0x0);
CHECK_OFFSET(cScreenPopup, mpNext, 0x4);
CHECK_OFFSET(cScreenPopup, mVtbl, 0x8);
CHECK_OFFSET(cScreenPopup, mHashName, 0xC);
CHECK_OFFSET(cScreenPopup, mpEngine, 0x10);
CHECK_OFFSET(cScreenPopup, mCurrentCtrl, 0x14);
CHECK_OFFSET(cScreenPopup, mCtrlBits, 0x15);
CHECK_OFFSET(cScreenPopup, unk_016, 0x16);
CHECK_OFFSET(cScreenPopup, miMiscData, 0x18);
CHECK_OFFSET(cScreenPopup, mAttrState, 0x1C);
CHECK_OFFSET(cScreenPopup, mpParentState, 0x20);
CHECK_OFFSET(cScreenPopup, mScreens, 0x24);
CHECK_OFFSET(cScreenPopup, mpScreen, 0x40);
CHECK_OFFSET(cScreenPopup, mUserNum, 0x44);
CHECK_OFFSET(cScreenPopup, unk_045, 0x45);
CHECK_OFFSET(cScreenPopup, mpBackGroundWhite, 0x48);
CHECK_OFFSET(cScreenPopup, mpBackGroundRed, 0x4C);
CHECK_OFFSET(cScreenPopup, mpBackGroundLarge, 0x50);
CHECK_OFFSET(cScreenPopup, mpBackGroundLargeOutline, 0x54);
CHECK_OFFSET(cScreenPopup, mpBackGroundSmall, 0x58);
CHECK_OFFSET(cScreenPopup, mpBackGroundSmallOutline, 0x5C);
CHECK_OFFSET(cScreenPopup, mpBackGroundShadow, 0x60);
CHECK_OFFSET(cScreenPopup, mpBackGroundGreyFade, 0x64);
CHECK_OFFSET(cScreenPopup, mpBackGroundBlack, 0x68);
CHECK_OFFSET(cScreenPopup, mpMenu, 0x6C);
CHECK_OFFSET(cScreenPopup, mpButtonGroup, 0x70);
CHECK_OFFSET(cScreenPopup, mpMessageText, 0x74);
CHECK_OFFSET(cScreenPopup, mpPostText, 0x78);
CHECK_OFFSET(cScreenPopup, mpTitleText, 0x7C);
CHECK_OFFSET(cScreenPopup, mpEnterBox, 0x80);
CHECK_OFFSET(cScreenPopup, mpStaticBox, 0x84);
CHECK_OFFSET(cScreenPopup, mpTextBoxPair, 0x88);
CHECK_OFFSET(cScreenPopup, mpEnterBoxBackground, 0x8C);
CHECK_OFFSET(cScreenPopup, mpOptions, 0x90);
CHECK_OFFSET(cScreenPopup, miGainTransistionCount, 0xA4);
CHECK_OFFSET(cScreenPopup, miStartFrame, 0xA8);
CHECK_OFFSET(cScreenPopup, mvAnimateIncrement, 0xAC);
CHECK_OFFSET(cScreenPopup, mbNoEntryOnNext, 0xB8);
CHECK_OFFSET(cScreenPopup, info, 0xBC);
CHECK_OFFSET(cScreenPopup, mvPosBackWhite, 0x188);
CHECK_OFFSET(cScreenPopup, mvPosBackRed, 0x194);
CHECK_OFFSET(cScreenPopup, mvPosMenu, 0x1A0);
CHECK_OFFSET(cScreenPopup, mvPosButtons, 0x1AC);
CHECK_OFFSET(cScreenPopup, mvPosTitle, 0x1B8);
CHECK_OFFSET(cScreenPopup, mvPosText, 0x1C4);
CHECK_OFFSET(cScreenPopup, mvPosPost, 0x1D0);
CHECK_OFFSET(cScreenPopup, mvPosEntryText, 0x1DC);
CHECK_OFFSET(cScreenPopup, mvPosTextBoxPair, 0x1E8);
CHECK_OFFSET(cScreenPopup, mvSizeBackWhite, 0x1F4);
CHECK_OFFSET(cScreenPopup, mvPosBackEnterBox, 0x200);
CHECK_OFFSET(cScreenPopup, mvSizeBackEnterBox, 0x20C);
CHECK_OFFSET(cScreenPopup, mvSizeBackSmall, 0x218);
CHECK_OFFSET(cScreenPopup, mvPosBackLarge, 0x224);
CHECK_OFFSET(cScreenPopup, mvPosBackLargeOutline, 0x230);
CHECK_OFFSET(cScreenPopup, mvPosBackSmall, 0x23C);
CHECK_OFFSET(cScreenPopup, mvPosBackSmallOutline, 0x248);
CHECK_OFFSET(cScreenPopup, mvPosBackShadow, 0x254);
CHECK_OFFSET(cScreenPopup, mvCurSize, 0x260);
CHECK_OFFSET(cScreenPopup, mvOffset, 0x26C);
CHECK_OFFSET(cScreenPopup, mvCurrentScale, 0x278);
CHECK_OFFSET(cScreenPopup, mvMenuSize, 0x284);
CHECK_OFFSET(cScreenPopup, mvButtonOffset, 0x290);
CHECK_OFFSET(cScreenPopup, mvLargestOption, 0x29C);
CHECK_OFFSET(cScreenPopup, mpKeyboard, 0x2A8);
CHECK_OFFSET(cScreenPopup, mpConfirmPopup, 0x2AC);
CHECK_OFFSET(cScreenPopup, mszInputText, 0x2B0);
CHECK_OFFSET(cScreenPopup, mszStaticText, 0x2B4);
CHECK_OFFSET(cScreenPopup, mbShowing, 0x2B8);
CHECK_OFFSET(cScreenPopup, mbFirstGainTran, 0x2BC);
CHECK_OFFSET(cScreenPopup, mbPassUpdate, 0x2C0);
CHECK_OFFSET(cScreenPopup, miDefaultOption, 0x2C4);
CHECK_OFFSET(cScreenPopup, mbCenterOptions, 0x2C8);
CHECK_OFFSET(cScreenPopup, mbCenterText, 0x2CC);
CHECK_OFFSET(cScreenPopup, mtConfirmation, 0x2D0);
CHECK_OFFSET(cScreenPopup, meType, 0x30C);
CHECK_OFFSET(cScreenPopup, mbReceivePrev, 0x310);
CHECK_OFFSET(cScreenPopup, miLayerOffset, 0x314);
CHECK_OFFSET(cScreenPopup, mfMinSize, 0x318);
CHECK_OFFSET(cScreenPopup, meBackgroundType, 0x31C);
CHECK_OFFSET(cScreenPopup, miConfimationReturnCode, 0x320);
CHECK_OFFSET(cScreenPopup, miActiveConfirmation, 0x324);
CHECK_OFFSET(cScreenPopup, miMaxChars, 0x328);
CHECK_OFFSET(cScreenPopup, mtHighlightedColor, 0x32C);
CHECK_OFFSET(cScreenPopup, mtUnhighlightColor, 0x33C);
CHECK_OFFSET(cScreenPopup, mtIndicatorColor, 0x34C);
CHECK_OFFSET(cScreenPopup, mfIndicatorAlpha, 0x35C);
CHECK_SIZE(cScreenPopup, 0x360);
CHECK_SIZE(tConfirmElement, 0xC);

#endif /* FE_SCREENPOPUP_H */
