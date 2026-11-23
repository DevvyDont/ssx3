#ifndef FE_FEPOPUPCONFIRM_H
#define FE_FEPOPUPCONFIRM_H

// cFEPopupConfirm: the front end's yes/no confirmation popup (a cScreenPopup with its own copies of the message and
// button texts): message plus 0, 1 (yes), 2 (yes/no) or 3 buttons. Created by the memory-card code (fe/fememcardcallbacks,
// fe/fememcard2), the options (fe/festateoptions) and the lodge (fe/festatelodge). No EA source path in the preview's
// asserts (On Tour: fe/all_in_one_fe.cpp); retail code: cFEPopupConfirm_cFEPopupConfirm ...
// cFEPopupConfirm_setDefaultButton in the fe/fememcardcallbacks unit (0x001D99C8 ... 0x001D9D18) and the default
// ctor, the dtor and setUsePreFE in main/all_in_one_main (0x001DDE78 ... 0x001DDEE0). The header path follows the
// unit's directory and the class name.
//
// Size 0xBFC: [R] cMemMan_alloc(0xBFC, "cFEPopupConfirm", 0x100) + cFEPopupConfirm_cFEPopupConfirm
// (src/fe/fememcardcallbacks.cpp, src/fe/festatelodge.cpp); the last member, mNumButtons, is the word at 0xBF8.
// [O] On Tour's class is 0xC00 with the same members at the same offsets (its tail padding differs).
// Base: cScreenPopup (fe/screenpopup.h, 0x360) : cUIState (0x48) : cListNode (vptr at 0x8). The ctor runs
// cScreenPopup_cScreenPopup(this, pEngine, pParentState), stores cFEPopupConfirm_vtbl (0x0046C890, 28 slots: 1 dtor,
// 25 setDefaultButton, 26 setButtonText, 27 setMessage, 28 initPopup, the rest cScreenPopup's / cUIState's), picks the
// background (info.meBackgroundType 1, or 2 with info.miLayerOffset 4 while the static cFEPopupConfirm_mUsePreFE is
// set) and runs initPopup; the dtor stores cFEPopupConfirm_vtbl and runs cScreenPopup__cScreenPopup.
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. The base's fields are copied from
// fe/screenpopup.h (first line of each comment). Evidence tags: [R] retail ctors / dtor and the
// allocation size; [P] the July 2003 preview's inline accessors (base fields); [U] how retail code uses the field
// (src/fe/fememcardcallbacks.cpp and the asm); [O] SSX On Tour's DWARF (class cFEPopupConfirm, cScreenPopup).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/screenpopup.h"

struct cFEPopupConfirm : public cUIState {
    // --- cScreenPopup (0x360; fe/screenpopup.h, which has the full evidence for these fields)
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEPopupConfirm_vtbl (both ctors, the dtor) over cScreenPopup_vtbl
    // cFEPopupConfirm's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName
    //   miMiscData: [P] cUIState::getMiscData lw/sw (ui/uistate.h); [U] cScreenPopup_initScreenPopup copies the popup
    //       info's miMiscData
    //   mpParentState: [U] receives onPopupEvent (slot 17)
    //   mpScreen: [R] ctor zeroes it; [U] cScreenPopup_onCreateScreen stores the "popup" screen
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
    cUIText* mpPostText;            // 0x078 [O]; [U] widget "popupPost"; cScreenPopup_handlePostPending
    cUIText* mpTitleText;           // 0x07C [O]; [U] widget "popupTitle"; cScreenPopup_createTitleBar
    cUIText* mpEnterBox;            // 0x080 [O]; [U] widget "EnterTextBox"; cScreenPopup_handleEntryBox
    cUIText* mpStaticBox;           // 0x084 [O]; [U] widget "StaticTextBoxLabel"
    cUIPair* mpTextBoxPair;         // 0x088 [O]; [U] widget "TextBoxPair"; selecting it opens the keyboard
    cUIVector* mpEnterBoxBackground; // 0x08C [O]; [U] widget "EnterBackground"
    cUIText* mpOptions[5];          // 0x090 [O]; [U] widgets "Option%d"; [R] ctor zeroes 5
    int miGainTransistionCount;     // 0x0A4 [O]; [U] onGainTransition counts it from 1 up to miStartFrame
    int miStartFrame;               // 0x0A8 [O]; [U] onGainTransition: frame of a label, else 25
    float mvAnimateIncrement[3];    // 0x0AC [O] t3Vector; [U] onGainTransition: per-frame scale step (x at 0xAC, y at 0xB0)
    int mbNoEntryOnNext;            // 0x0B8 [O] (On Tour: bool); [U] cScreenPopup_onWidgetEvent sets/clears it
    cPopupInfo info;                // 0x0BC [O]; [R] cPopupInfo_cPopupInfo(self + 0xBC); [U] initPopup / setMessage /
                                    //       setDefaultButton write its meType, mtMessage, miLayerOffset, mbPassUpdate,
                                    //       mbReceivePrev, miDefaultOption; the ctor its meBackgroundType
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
    char* mszInputText;             // 0x2B0 [P] getEntryBoxText (preview 0x16C, seen); [O] cBXString
    char* mszStaticText;            // 0x2B4 [O] cBXString; [R] ctor/dtor like mszInputText
    int mbShowing;                  // 0x2B8 [P] isShowing (preview 0x174, seen); [U] onCreateScreen 1, onDestroyScreen 0
    int mbFirstGainTran;            // 0x2BC [O]; [U] set on the first onGainTransition; redrawPopup checks it
    int mbPassUpdate;               // 0x2C0 [P] getPassUpdate/setPassUpdate (preview 0x178, seen); [U] onUpdate calls the
                                    //       parent's onUpdate when set
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
    // --- cFEPopupConfirm
    unsigned short mDisplayText[800]; // 0x360 [O]; [U] initPopup / setMessage copy the 16-bit message here and point
                                    //       info.mtMessage's value (+0xC4) at it
    unsigned short mYesText[100];   // 0x9A0 [O]; [U] setButtonText(text, 0) copies the first button's label here
    unsigned short mNoText[100];    // 0xA68 [O]; [U] setButtonText(text, 1), when mNumButtons >= 2
    unsigned short mAltText[100];   // 0xB30 [O]; [U] setButtonText(text, other): makes it a 3-button popup
    int mNumButtons;                // 0xBF8 [O]; [U] initPopup stores 0 (no buttons), 1 (yes only) or 2;
                                    //       setButtonText stores 3 for a third button and tests >= 2
};

CHECK_OFFSET(cFEPopupConfirm, mpBackGroundWhite, 0x048);
CHECK_OFFSET(cFEPopupConfirm, mpBackGroundRed, 0x04C);
CHECK_OFFSET(cFEPopupConfirm, mpBackGroundLarge, 0x050);
CHECK_OFFSET(cFEPopupConfirm, mpBackGroundLargeOutline, 0x054);
CHECK_OFFSET(cFEPopupConfirm, mpBackGroundSmall, 0x058);
CHECK_OFFSET(cFEPopupConfirm, mpBackGroundSmallOutline, 0x05C);
CHECK_OFFSET(cFEPopupConfirm, mpBackGroundShadow, 0x060);
CHECK_OFFSET(cFEPopupConfirm, mpBackGroundGreyFade, 0x064);
CHECK_OFFSET(cFEPopupConfirm, mpBackGroundBlack, 0x068);
CHECK_OFFSET(cFEPopupConfirm, mpMenu, 0x06C);
CHECK_OFFSET(cFEPopupConfirm, mpButtonGroup, 0x070);
CHECK_OFFSET(cFEPopupConfirm, mpMessageText, 0x074);
CHECK_OFFSET(cFEPopupConfirm, mpPostText, 0x078);
CHECK_OFFSET(cFEPopupConfirm, mpTitleText, 0x07C);
CHECK_OFFSET(cFEPopupConfirm, mpEnterBox, 0x080);
CHECK_OFFSET(cFEPopupConfirm, mpStaticBox, 0x084);
CHECK_OFFSET(cFEPopupConfirm, mpTextBoxPair, 0x088);
CHECK_OFFSET(cFEPopupConfirm, mpEnterBoxBackground, 0x08C);
CHECK_OFFSET(cFEPopupConfirm, mpOptions, 0x090);
CHECK_OFFSET(cFEPopupConfirm, miGainTransistionCount, 0x0A4);
CHECK_OFFSET(cFEPopupConfirm, miStartFrame, 0x0A8);
CHECK_OFFSET(cFEPopupConfirm, mvAnimateIncrement, 0x0AC);
CHECK_OFFSET(cFEPopupConfirm, mbNoEntryOnNext, 0x0B8);
CHECK_OFFSET(cFEPopupConfirm, info, 0x0BC);
CHECK_OFFSET(cFEPopupConfirm, mvPosBackWhite, 0x188);
CHECK_OFFSET(cFEPopupConfirm, mvPosBackRed, 0x194);
CHECK_OFFSET(cFEPopupConfirm, mvPosMenu, 0x1A0);
CHECK_OFFSET(cFEPopupConfirm, mvPosButtons, 0x1AC);
CHECK_OFFSET(cFEPopupConfirm, mvPosTitle, 0x1B8);
CHECK_OFFSET(cFEPopupConfirm, mvPosText, 0x1C4);
CHECK_OFFSET(cFEPopupConfirm, mvPosPost, 0x1D0);
CHECK_OFFSET(cFEPopupConfirm, mvPosEntryText, 0x1DC);
CHECK_OFFSET(cFEPopupConfirm, mvPosTextBoxPair, 0x1E8);
CHECK_OFFSET(cFEPopupConfirm, mvSizeBackWhite, 0x1F4);
CHECK_OFFSET(cFEPopupConfirm, mvPosBackEnterBox, 0x200);
CHECK_OFFSET(cFEPopupConfirm, mvSizeBackEnterBox, 0x20C);
CHECK_OFFSET(cFEPopupConfirm, mvSizeBackSmall, 0x218);
CHECK_OFFSET(cFEPopupConfirm, mvPosBackLarge, 0x224);
CHECK_OFFSET(cFEPopupConfirm, mvPosBackLargeOutline, 0x230);
CHECK_OFFSET(cFEPopupConfirm, mvPosBackSmall, 0x23C);
CHECK_OFFSET(cFEPopupConfirm, mvPosBackSmallOutline, 0x248);
CHECK_OFFSET(cFEPopupConfirm, mvPosBackShadow, 0x254);
CHECK_OFFSET(cFEPopupConfirm, mvCurSize, 0x260);
CHECK_OFFSET(cFEPopupConfirm, mvOffset, 0x26C);
CHECK_OFFSET(cFEPopupConfirm, mvCurrentScale, 0x278);
CHECK_OFFSET(cFEPopupConfirm, mvMenuSize, 0x284);
CHECK_OFFSET(cFEPopupConfirm, mvButtonOffset, 0x290);
CHECK_OFFSET(cFEPopupConfirm, mvLargestOption, 0x29C);
CHECK_OFFSET(cFEPopupConfirm, mpKeyboard, 0x2A8);
CHECK_OFFSET(cFEPopupConfirm, mpConfirmPopup, 0x2AC);
CHECK_OFFSET(cFEPopupConfirm, mszInputText, 0x2B0);
CHECK_OFFSET(cFEPopupConfirm, mszStaticText, 0x2B4);
CHECK_OFFSET(cFEPopupConfirm, mbShowing, 0x2B8);
CHECK_OFFSET(cFEPopupConfirm, mbFirstGainTran, 0x2BC);
CHECK_OFFSET(cFEPopupConfirm, mbPassUpdate, 0x2C0);
CHECK_OFFSET(cFEPopupConfirm, miDefaultOption, 0x2C4);
CHECK_OFFSET(cFEPopupConfirm, mbCenterOptions, 0x2C8);
CHECK_OFFSET(cFEPopupConfirm, mbCenterText, 0x2CC);
CHECK_OFFSET(cFEPopupConfirm, mtConfirmation, 0x2D0);
CHECK_OFFSET(cFEPopupConfirm, meType, 0x30C);
CHECK_OFFSET(cFEPopupConfirm, mbReceivePrev, 0x310);
CHECK_OFFSET(cFEPopupConfirm, miLayerOffset, 0x314);
CHECK_OFFSET(cFEPopupConfirm, mfMinSize, 0x318);
CHECK_OFFSET(cFEPopupConfirm, meBackgroundType, 0x31C);
CHECK_OFFSET(cFEPopupConfirm, miConfimationReturnCode, 0x320);
CHECK_OFFSET(cFEPopupConfirm, miActiveConfirmation, 0x324);
CHECK_OFFSET(cFEPopupConfirm, miMaxChars, 0x328);
CHECK_OFFSET(cFEPopupConfirm, mtHighlightedColor, 0x32C);
CHECK_OFFSET(cFEPopupConfirm, mtUnhighlightColor, 0x33C);
CHECK_OFFSET(cFEPopupConfirm, mtIndicatorColor, 0x34C);
CHECK_OFFSET(cFEPopupConfirm, mfIndicatorAlpha, 0x35C);
CHECK_OFFSET(cFEPopupConfirm, mDisplayText, 0x360);
CHECK_OFFSET(cFEPopupConfirm, mYesText, 0x9A0);
CHECK_OFFSET(cFEPopupConfirm, mNoText, 0xA68);
CHECK_OFFSET(cFEPopupConfirm, mAltText, 0xB30);
CHECK_OFFSET(cFEPopupConfirm, mNumButtons, 0xBF8);
CHECK_SIZE(cFEPopupConfirm, 0xBFC);

#endif /* FE_FEPOPUPCONFIRM_H */
