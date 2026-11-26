#ifndef FE_SCREENPOPUP_H
#define FE_SCREENPOPUP_H

// cScreenPopup: the front end's generic message/confirmation popup (title, message, up to 5 options, an optional
// text-entry box that opens a cKeyboardPopup). A cUIState; its methods are in fe/fepopup.cpp. Retail vtable
// cScreenPopup_vtbl (stored by cScreenPopup_cScreenPopup and the default ctor cScreenPopup_cScreenPopup1). Created with
// cMemMan_alloc(0x360, "Confirmation Popup") + cScreenPopup_cScreenPopup (cScreenPopup_createConfirmationPopup,
// feasyncfile.cpp); cFEPopupConfirm (fememcardcallbacks.cpp) derives from it.
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. Evidence tags:
//   [R] retail's constructors: cUIState(cUIEngine*, cUIState*) = cUIState_cUIState1 (ui/uiscreen.cpp),
//       cScreenPopup_cScreenPopup, the destructor cScreenPopup__cScreenPopup, and the allocation size above;
//   [P] the July 2003 preview's inline accessors, carried to retail by tools/layout_xlate.py ("seen" pairs);
//   [U] how matched retail code uses the field; widget fields name the UI object cScreenPopup_FillObjectPointers
//       binds to them (strings read from the retail ELF);
//   [O] SSX On Tour's DWARF (class cListNode / cUIState / cScreenPopup): same member order; On Tour's bools are
//       1 byte (retail's are words) and its tARGBs are 16-aligned, so its offsets drift by 0..0x14. Used only where
//       the fields on both sides are placed by other evidence.
// Vectors are EA's t3Vector (bx/math3d.h, 3 floats, 4-aligned) and tARGB (ui/uithing.h, 4 floats {A, R, G, B}),
// the types On Tour's DWARF gives them.

#include "layout.h"
#include "bx/bxstring.h"
#include "bx/math3d.h"
#include "ui/uistate.h"
#include "ui/uithing.h"
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

struct cPopupInfo;
struct cUIThing;
struct t3Vector;
struct tRect;

struct cScreenPopup : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cScreenPopup vtable cScreenPopup_vtbl
    // cScreenPopup's notes on cUIState's fields:
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
    t3Vector mvAnimateIncrement;    // 0x0AC [O]; [U] onGainTransition: per-frame scale step (x at 0xAC, y at 0xB0)
    int mbNoEntryOnNext;            // 0x0B8 [O] (On Tour: bool); [U] cScreenPopup_onWidgetEvent sets/clears it,
                                    //       cScreenPopup_onWidgetMenu (case 6) tests it
    cPopupInfo info;                // 0x0BC [O] cPopupInfo info; [R] constructed by cPopupInfo_cPopupInfo(self + 0xBC), destroyed by
                                    //       cPopupInfo__cPopupInfo; getPopupInfo (cScreenPopup_getPopupInfo) returns self + 0xBC; retail cPopupInfo
                                    //       is 0xCC bytes (data to 0xC4, vptr cPopupInfo_vtbl at 0xC8)
    t3Vector mvPosBackWhite;        // 0x188 [O]; [U] FillObjectPointers: "whitefg" position
    t3Vector mvPosBackRed;          // 0x194 [O]; [U] "redbg" position
    t3Vector mvPosMenu;             // 0x1A0 [O]; [U] "Menu0000" position
    t3Vector mvPosButtons;          // 0x1AC [O]; [U] "buttons" position
    t3Vector mvPosTitle;            // 0x1B8 [O]; [U] "popupTitle" position
    t3Vector mvPosText;             // 0x1C4 [O]; [U] "popupText" position
    t3Vector mvPosPost;             // 0x1D0 [O]; [U] "popupPost" position
    t3Vector mvPosEntryText;        // 0x1DC [O]; [U] "EnterTextBox" position
    t3Vector mvPosTextBoxPair;      // 0x1E8 [O]; [U] "TextBoxPair" position
    t3Vector mvSizeBackWhite;       // 0x1F4 [O]; [U] "whitefg" size (vt slot 20 GetRect)
    t3Vector mvPosBackEnterBox;     // 0x200 [O]; [U] "EnterBackground" position
    t3Vector mvSizeBackEnterBox;    // 0x20C [O]; [U] "EnterBackground" size
    t3Vector mvSizeBackSmall;       // 0x218 [O]; [U] "Small shape" size
    t3Vector mvPosBackLarge;        // 0x224 [O]; [U] "Big shape" position
    t3Vector mvPosBackLargeOutline; // 0x230 [O]; [U] "Big outline" position
    t3Vector mvPosBackSmall;        // 0x23C [O]; [U] "Small shape" position
    t3Vector mvPosBackSmallOutline; // 0x248 [O]; [U] "Small outline" position
    t3Vector mvPosBackShadow;       // 0x254 [O]; [U] "Shadow" position
    t3Vector mvCurSize;             // 0x260 [O]; [U] grown by every text line (cScreenPopup_increaseBoxSize calls)
    t3Vector mvOffset;              // 0x26C [O]; [U] copied from the popup info's mvOffset (info + 0x9C)
    t3Vector mvCurrentScale;        // 0x278 [O]; [R] ctor sets (1, 1, 1); [U] passed to scaleBackground
    t3Vector mvMenuSize;            // 0x284 [O]; [U] setPopupMessage copies mvCurSize into it
    t3Vector mvButtonOffset;        // 0x290 [O]; [U] cScreenPopup_adjustMenuPosition writes it,
                                    //       cScreenPopup_adjustButtonPosition adds it to mvPosButtons
    t3Vector mvLargestOption;       // 0x29C [O]; [U] FillOptions keeps the widest option width in x
    cKeyboardPopup* mpKeyboard;     // 0x2A8 [O]; [R] ctor zeroes; [U] onWidgetEvent allocates the cKeyboardPopup into it
    cScreenPopup* mpConfirmPopup;   // 0x2AC [O]; [U] createConfirmationPopup stores the "Confirmation Popup" there
    cBXString mszInputText;         // 0x2B0 [P] getEntryBoxText (preview 0x16C, seen); [O]; [R] ctor stores the
                                    //       empty-string rep cBXString_EmptyString; [U] filled from the keyboard's text
    cBXString mszStaticText;        // 0x2B4 [O]; [R] ctor/dtor like mszInputText
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
    tARGB mtHighlightedColor;       // 0x32C [O]; [U] onWidgetMenu colours the selected entry with it
    tARGB mtUnhighlightColor;       // 0x33C [O]; [U] the other entries
    tARGB mtIndicatorColor;         // 0x34C [O]; no retail access seen (placed by the 0x360 size)
    float mfIndicatorAlpha;         // 0x35C [O]; no retail access seen (placed by the 0x360 size)

    // miConfimationReturnCode when the confirmation popup was refused (names ours). [U] cScreenPopup_onPopupEvent
    // stores it for the confirmation's second option or back, and does not report it to the parent.
    enum { kNoConfirmationResult = -1 };

    // Methods bound to their retail symbols by asm labels.
    void redrawPopup() __asm__("cScreenPopup_redrawPopup");
    void SetTextObjectsVisibility(int visible) __asm__("cScreenPopup_SetTextObjectsVisibility");
    void SetTextObjectsAlpha(float fAlpha) __asm__("cScreenPopup_SetTextObjectsAlpha");
    void FillObjectPointers() __asm__("cScreenPopup_FillObjectPointers");
    void initScreenPopup(cPopupInfo* info) __asm__("cScreenPopup_initScreenPopup");
    bool handleEntryBox(cPopupInfo* infop) __asm__("cScreenPopup_handleEntryBox");
    void handlePostPending(cPopupInfo* pInfo) __asm__("cScreenPopup_handlePostPending");
    void setTextObjectSize(t3Vector* vp) __asm__("cScreenPopup_setTextObjectSize");
    void setPopupMessage(cPopupInfo* pInfo) __asm__("cScreenPopup_setPopupMessage");
    void FillOptions(cPopupInfo* infop) __asm__("cScreenPopup_FillOptions");
    void setPrivateDefaultOption(cUIMenu* menu, int idx, int max) __asm__("cScreenPopup_setPrivateDefaultOption");
    void scaleBackground(t3Vector* color) __asm__("cScreenPopup_scaleBackground");
    float centerOffset() __asm__("cScreenPopup_centerOffset");
    void centerPopup() __asm__("cScreenPopup_centerPopup");
    void FillTextEntryBox() __asm__("cScreenPopup_FillTextEntryBox");
    void setTextString2(cUIText* text, int* info) __asm__("cScreenPopup_setTextString2");
    void setTextString1(void* dst, int* info) __asm__("cScreenPopup_setTextString1");
    void setTextString(void* dst, cUIText* text) __asm__("cScreenPopup_setTextString");
    void getStringSize(cUIText* text, int* info, float* out) __asm__("cScreenPopup_getStringSize");
    void getStringSize1(cUIText* text, const char* str, tRect* out) __asm__("cScreenPopup_getStringSize1");
    void getStringSize2(cUIText* text, const char* str, tRect* out) __asm__("cScreenPopup_getStringSize2");
    void getStringSize3(void* obj, tRect* out) __asm__("cScreenPopup_getStringSize3");
    void createTitleBar(cPopupInfo* pInfo) __asm__("cScreenPopup_createTitleBar");
    void adjustMenuPosition() __asm__("cScreenPopup_adjustMenuPosition");
    void adjustButtonPosition() __asm__("cScreenPopup_adjustButtonPosition");
    void changePivot(cUIThing* obj, unsigned char state, float fOffset) __asm__("cScreenPopup_changePivot");
    void restoreElements() __asm__("cScreenPopup_restoreElements");
    void getTextBoxSize(int* item, float* size, int flag, float extra) __asm__("cScreenPopup_getTextBoxSize");
    void getTextBoxSize1(void* text, unsigned short* str, float* size) __asm__("cScreenPopup_getTextBoxSize1");
    void getTextBoxSize2(cUIText* text, const char* str, int sizePtr) __asm__("cScreenPopup_getTextBoxSize2");
    void getMessageLineWrapSize1(int* msg, t3Vector* out) __asm__("cScreenPopup_getMessageLineWrapSize1");
    void getMessageLineWrapSize2(cUIText* text, void* strp, t3Vector* outp)
        __asm__("cScreenPopup_getMessageLineWrapSize2");
    void getMessageLineWrapSize(cUIText* pText, const char* pString, t3Vector* pvSize)
        __asm__("cScreenPopup_getMessageLineWrapSize");
    void getTextBoxWrapSize(void* ptElement, void* vSize, int which, float bMessage)
        __asm__("cScreenPopup_getTextBoxWrapSize");
    void getTextBoxWrapSize1(cUIText* text, void* strp, float* out) __asm__("cScreenPopup_getTextBoxWrapSize1");
    void getTextBoxWrapSize2(cUIText* text, const char* str, int sizePtr) __asm__("cScreenPopup_getTextBoxWrapSize2");
    void increaseBoxSize(float* size, float* item) __asm__("cScreenPopup_increaseBoxSize");
    void createConfirmationPopup(int idx) __asm__("cScreenPopup_createConfirmationPopup");
    void adjustLayer(int layerOffset) __asm__("cScreenPopup_adjustLayer");
    void setStatic(int idx, int bStatic) __asm__("cScreenPopup_setStatic");
    int getCurrentlySelected() __asm__("cScreenPopup_getCurrentlySelected");
    void hideWidgetSafe(cUIThing* obj) __asm__("cScreenPopup_hideWidgetSafe");
    void showWidgetSafe(cUIThing* obj) __asm__("cScreenPopup_showWidgetSafe");
    void checkAndFixTextEntry(cUIText* text, const char* str) __asm__("cScreenPopup_checkAndFixTextEntry");
    cScreenPopup * cScreenPopup1() __asm__("cScreenPopup_cScreenPopup1");
    void setPassUpdate(int passUpdate) __asm__("cScreenPopup_setPassUpdate");
    int getPassUpdate() __asm__("cScreenPopup_getPassUpdate");
    int getEntryBoxText() __asm__("cScreenPopup_getEntryBoxText");
    int isShowing() __asm__("cScreenPopup_isShowing");
    // retail's destructor cScreenPopup__cScreenPopup stays a C function (g++ 2.95 takes no asm label on a destructor);
    // declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cScreenPopup();
    virtual void onCreateScreen() __asm__("cScreenPopup_onCreateScreen");
    virtual bool onGainTransition(int first) __asm__("cScreenPopup_onGainTransition");
    virtual void onUpdate() __asm__("cScreenPopup_onUpdate");
};

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
