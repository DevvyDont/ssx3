#ifndef UI_UISTATE_H
#define UI_UISTATE_H

// cUIState: one state (page/popup) of the UI state machine. The UI engine owns a cUIStateStack of them; each state
// owns a cList of cUIScreens (cUIState_addScreen, cUIEngine_addScreenByHashName). The FE and in-game states
// (cFEState*, cOVState*, cGameLoadState, popups ...) derive from it: they call the cUIState constructor
// cUIState_cUIState(this, pEngine) or cUIState_cUIState1(this, pEngine, pParentState), store their own vtable at 0x8, and
// put their own members from 0x48 on (cFEStateCharSelect_cFEStateCharSelect; ovstates allocates a 0x4C-byte popup), so cUIState is 0x48 bytes.
//
// A real class: cUIState : cListNode (util/list.h; the vptr at 0x8), its own members at their retail offsets.
// Base class: cListNode (On Tour: mpPrev 0x0, mpNext 0x4; its vptr follows at 0x8). _vt$8cUIState (cUIState_vtbl)
// keeps cListNode's traverseFlat/traverseRecurse (cListNode_traverseFlat/cListNode_traverseRecurse) in slots 2-3.
//
// Member names are EA's, from SSX On Tour's PSP DWARF (class cUIState, size 0x48: the same offsets as retail),
// placed at retail offsets by:
//   [P] the July 2003 preview's inline accessors; retail keeps out-of-line copies of them in On Tour's declaration
//       order in ui/uivector (cUIState_setHashName setHashName ... cUIState_setVisible setVisible, cUIState_setParentState ...
//       cUIState_getUserNum getUserNum), which places each one. The preview itself packed the bytes differently
//       (tAttrState at 0x16, miMiscData last), so preview offsets are not used directly;
//   [R] retail's constructors cUIState_cUIState/cUIState_cUIState1 and the destructor cUIState__cUIState;
//   [U] how retail code uses the field;
//   [O] On Tour's member order alone, where the fields on both sides of it are placed by [P] or [R].
//
// mAttrState is EA's tAttrState (On Tour, LSB first): mIsPopup:1 mHasPopup:2 mVisible:1 mFirstTime:1 mEnabled:1
// mExplicit:1 mPendingDelete:1 mMode:6 mPushCheck:1 mPopFloor:1 (struct tAttrState below). EA's type is 2 bytes (the constructors store it
// with sh 0x218 = visible, first time, mode 2), but every accessor reads and writes the aligned word (lw/sw 0x1C),
// so it is an int here and covers the 2 padding bytes.

#include "layout.h"
#include "util/list.h"

struct cUIInput;
struct cUIGroup;
struct cUIThing;
struct cUIMenu;
struct cUIPair;
struct cUIListBox;
struct cUIEngine;
struct cUIScreen;

// EA's tAttrState, the type of cUIState::mAttrState (On Tour struct tAttrState, size 0x2, LSB first). cUIState keeps
// mAttrState an int (its accessors use lw/sw 0x1C, and it covers the 2 padding bytes); this type names the bits for
// code that reads one field, e.g. ((tAttrState*)&state->mAttrState)->mMode. [R] the constructors store 0x218 with sh
// (mVisible, mFirstTime, mMode = 2); [P] the inline accessors getMode (lbu 0x1D), isFirstTime ... setVisible.
struct tAttrState {
    unsigned short mIsPopup : 1;       // bit 0 [O]
    unsigned short mHasPopup : 2;      // bits 1-2 [O]
    unsigned short mVisible : 1;       // bit 3 [O]; [R] set by the ctors (0x218); [P] setVisible; [U]
                                       //       cUIStateStack_setVisibleToChain writes bit 3 along the parent chain
    unsigned short mFirstTime : 1;     // bit 4 [O]; [R] set by the ctors; [P] isFirstTime
    unsigned short mEnabled : 1;       // bit 5 [O]
    unsigned short mExplicit : 1;      // bit 6 [O]
    unsigned short mPendingDelete : 1; // bit 7 [O]
    unsigned short mMode : 6;          // bits 8-13 [O]; [R] 2 in the ctors; [P] getMode (lbu 0x1D); [U] "FEOptions"
                                       //       states set it (func_001887A0 callers mask 0x3F00)
    unsigned short mPushCheck : 1;     // bit 14 [O]
    unsigned short mPopFloor : 1;      // bit 15 [O]
};

struct cListNode;

struct cUIState : public cListNode {
    // cListNode (util/list.h): mpPrev 0x000 / mpNext 0x004 ([R] the constructors self-link them), the vptr 0x008:
    //   [R] _vt$8cUIState (cUIState_vtbl), stored by cUIState_cUIState/cUIState_cUIState1/cUIState_cUIState2
    int mHashName;                   // 0x00C [P] getHashName/setHashName (retail cUIState_getHashName/cUIState_setHashName); [R] = 0
                                     //       (derived states store their state id here, e.g. 9 in cFEStateCharSelect_cFEStateCharSelect)
    cUIEngine* mpEngine;             // 0x010 [P] getEngine (retail cUIState_getEngine); [R] = pEngine; [U] cUIState_onUpdate reads the
                                     //       engine's input object through it
    signed char mCurrentCtrl;        // 0x014 [P] getCurrentCtrl/setCurrentCtrl (lb/sb; retail cUIState_getCurrentCtrl/cUIState_setCurrentCtrl);
                                     //       [R] = -1; [U] controller loop counter in cUIState_onUpdate (On Tour: char)
    unsigned char mCtrlBits;         // 0x015 [P] getCtrlBitMask/setAllCtrlBits (lbu/sb; retail cUIState_getCtrlBitMask/cUIState_setAllCtrlBits);
                                     //       [R] = 0xF; [U] setCtrlBitsByCtrlIndex/clearCtrlBits (cUIState_setCtrlBitsByCtrlIndex/cUIState_clearCtrlBits)
    char unk_016[0x2];               // 0x016 padding
    int miMiscData;                  // 0x018 [P] getMiscData/setMiscData (lw/sw 0x18; retail cUIState_getMiscData/cUIState_setMiscData,
                                     //       between getCtrlBitMask and getMode in On Tour's order); not set by the constructors
    int mAttrState;                  // 0x01C [P] tAttrState: getMode (lbu 0x1D), isFirstTime ... setVisible (lw/sw 0x1C; retail
                                     //       cUIState_getMode..cUIState_setVisible); [R] sh 0x218 in the constructors
    cUIState* mpParentState;         // 0x020 [P] getParentState (retail cUIState_getParentState; setter cUIState_setParentState); [R] = pParentState
                                     //       (cUIState_cUIState1), 0 (cUIState_cUIState)
    cList mScreens;                  // 0x024 [O] cList mScreens, the state's cUIScreens (util/list.h: mStart 0x24, mEnd 0x30, vptr 0x3C);
                                     //       [R] the ctors cList_cList(&mScreens); the destructor cUIState__cUIState restores
                                     //       mScreens.mVtbl (0x3C, cList_vtbl), mEnd.mVtbl (0x38) and mStart.mVtbl (0x2C, cListNode_vtbl);
                                     //       [U] cUIState_addScreen appends (cList_addToEnd), cUIState_updateScreens / cUIState_onUpdate
                                     //       walk it (cList_first), cUIState_onDestroyScreen destroys it
    cUIScreen* mpScreen;             // 0x040 [P] getScreen (retail cUIState_getScreen); [R] = 0
    signed char mUserNum;            // 0x044 [P] getUserNum/setUserNum (lb/sb; retail cUIState_getUserNum/cUIState_setUserNum); [R] = -1 (On Tour: char)
    char unk_045[0x3];               // 0x045 padding

    // EA's tUIStateEvent: the Event a state's onWidgetEvent / onPopupEvent receive. [O] On Tour enum
    // cUIState::tUIStateEvent, names and values. [U] retail fits: cUIMenu_setFocus sends kONGAINFOCUS (1) with
    // setFocus(1) and kONLOSEFOCUS (2) with setFocus(0); cUIMenu_onInput reports inputs 4..8 as these events and
    // passes kONNEXT (5) / kONPREV (6) / kONOPTION (7) / kONMISC (8) on as cUIMenu's kONNEXT / kONPREVIOUS / kONOPTION
    // / kONMISC (6..9); cUIPair_onInput reports 5 / 6 as cUIPair's ONNEXT 0 / ONPREVIOUS 1; cUIListBox_onInput
    // reports a changed entry as kONCHANGE (9); the keyboard popup reports kONKEYBOARDEND (13) / kONKEYBOARDCANCEL
    // (14) to its parent, and cScreenPopup reports its options as kONSCREENPOPUPOPTION0..4 (15..19).
    enum tUIStateEvent {
        kONGAINFOCUS = 1,
        kONLOSEFOCUS = 2,
        kONSTART = 3,
        kONSELECT = 4,
        kONNEXT = 5,
        kONPREV = 6,
        kONOPTION = 7,
        kONMISC = 8,
        kONCHANGE = 9,
        kONMENUPREVITEM = 10,
        kONMENUNEXTITEM = 11,
        kONKEYBOARDKEY = 12,
        kONKEYBOARDEND = 13,
        kONKEYBOARDCANCEL = 14,
        kONSCREENPOPUPOPTION0 = 15,
        kONSCREENPOPUPOPTION1 = 16,
        kONSCREENPOPUPOPTION2 = 17,
        kONSCREENPOPUPOPTION3 = 18,
        kONSCREENPOPUPOPTION4 = 19,
        kONSCREENPOPUPPREV = 20,
        kONPOPUPCREATE = 21,
        kONPOPUPDESTROY = 22
    };

    // EA's tUIStateMode: a state's life-cycle step, tAttrState::mMode. [O] On Tour enum cUIState::tUIStateMode, names
    // and values. [U] retail fits: the ctors start at kPENDINGCREATE; cUIStateStack_updateLoop creates the screens
    // (slot 4) and moves to kGAINTRANSITION, waits for it (slot 7), gains focus (slot 6) in kGAINFOCUS, runs kUPDATE
    // (slot 12) every pass, waits out kLOSETRANSITION (slot 10), and in kLOSEFOCUS destroys the screens (slot 5) and
    // ends in kNOFOCUS; it brings up a kPENDINGNEXT (else a kNOFOCUS) state when nothing is current.
    enum tUIStateMode {
        kNONE = 0,
        kPENDINGNEXT = 1,
        kPENDINGCREATE = 2,
        kGAINFOCUS = 3,
        kGAINTRANSITION = 4,
        kUPDATE = 5,
        kLOSETRANSITION = 6,
        kLOSEFOCUS = 7,
        kNOFOCUS = 8
    };

    // Virtuals. Slots [R] cUIState_vtbl (2-3 are cListNode's traverseFlat / traverseRecurse), names, order and types
    // [O] (On Tour class cUIState: the same 21 in the same order); bool parameters are words.
    virtual ~cUIState();                                                              // 1
    virtual void onCreateScreen()
        __asm__("cUIState_onCreateScreen__FPv");                                                    // 4
    virtual void onDestroyScreen();                                                   // 5
    virtual void onGainFocus(cUIState* pPrevState);                                   // 6
    virtual bool onGainTransition(int bFirstTime);                                    // 7
    virtual bool onInputBegin(cUIInput* pInput);                                      // 8
    virtual void onInputAlways(cUIInput* pInput);                                     // 9
    virtual bool onLoseTransition()
        __asm__("cUIState_onLoseTransition__FPv");                                                  // 10
    virtual void onLoseFocus(cUIState* pNextState);                                   // 11
    virtual void onUpdate();                                                          // 12
    virtual void onMenuGainFocus(cUIGroup* pRootGroup)
        __asm__("cUIState_onMenuGainFocus");                               // 13
    virtual void onPreRender2D()
        __asm__("cUIState_onPreRender2D__FPv");                                                     // 14
    virtual void onRender2D()
        __asm__("cUIState_onRender2D");                                                        // 15
    virtual bool onScreenEvent(cUIScreen* pScreen, unsigned int Event);               // 16
    virtual void onPopupEvent(cUIState* pPopup, unsigned int Event);                  // 17
    virtual void onWidgetCreate(cUIThing* pWidget);                                   // 18
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int Event);                // 19
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int Event, unsigned int index); // 20
    virtual int onWidgetPair(cUIPair* pPair, unsigned int Event);                     // 21
    virtual int onWidgetListBox(cUIListBox* pListBox, unsigned int Event);            // 22
    virtual void* getData(int msg);                                                   // 23
    virtual int handleEvent(int msg, void* pParam);                                   // 24

    // Methods bound to their retail symbols by asm labels.
    void* cUIState1(int engine, int parentState) __asm__("cUIState_cUIState1");
    void setCtrlBitsByCtrlIndex(signed char bit) __asm__("cUIState_setCtrlBitsByCtrlIndex");
    void clearCtrlBits(signed char bit) __asm__("cUIState_clearCtrlBits");
    void addScreen(cListNode* node) __asm__("cUIState_addScreen");
    void hideScreens() __asm__("cUIState_hideScreens");
    void showScreens() __asm__("cUIState_showScreens");
    void updateScreens() __asm__("cUIState_updateScreens");
    void hideObjSafe(char* name) __asm__("cUIState_hideObjSafe");
    void showObjSafe(char* name) __asm__("cUIState_showObjSafe");
    void* cUIState2() __asm__("cUIState_cUIState2");
    void setHashName(int hashName) __asm__("cUIState_setHashName");
    int getHashName() __asm__("cUIState_getHashName");
    int getEngine() __asm__("cUIState_getEngine");
    void setCurrentCtrl(signed char currentCtrl) __asm__("cUIState_setCurrentCtrl");
    signed char getCurrentCtrl() __asm__("cUIState_getCurrentCtrl");
    void setAllCtrlBits(signed char ctrlBits) __asm__("cUIState_setAllCtrlBits");
    unsigned char getCtrlBitMask() __asm__("cUIState_getCtrlBitMask");
    int getMiscData() __asm__("cUIState_getMiscData");
    void setMiscData(int miscData) __asm__("cUIState_setMiscData");
    int getMode() __asm__("cUIState_getMode");
    void setMode(int mode) __asm__("cUIState_setMode");
    int isFirstTime() __asm__("cUIState_isFirstTime");
    void setFirstTime(int firstTime) __asm__("cUIState_setFirstTime");
    int isEnabled() __asm__("cUIState_isEnabled");
    void setEnabled(int enabled) __asm__("cUIState_setEnabled");
    int isPushCheck() __asm__("cUIState_isPushCheck");
    void setPushCheck(int pushCheck) __asm__("cUIState_setPushCheck");
    int isPopFloor() __asm__("cUIState_isPopFloor");
    void setPopFloor(int popFloor) __asm__("cUIState_setPopFloor");
    int isExplicit() __asm__("cUIState_isExplicit");
    void setExplicit(int isExplicit) __asm__("cUIState_setExplicit");
    int isPendingDelete() __asm__("cUIState_isPendingDelete");
    void setPendingDelete(int pendingDelete) __asm__("cUIState_setPendingDelete");
    bool hasPopup() __asm__("cUIState_hasPopup");
    int numPopup() __asm__("cUIState_numPopup");
    void setHasPopup(int popupCount) __asm__("cUIState_setHasPopup");
    void incrementPopup() __asm__("cUIState_incrementPopup");
    void decrementPopup() __asm__("cUIState_decrementPopup");
    int isPopup() __asm__("cUIState_isPopup");
    void setPopup(int isPopup) __asm__("cUIState_setPopup");
    int isVisible() __asm__("cUIState_isVisible");
    void setVisible(int visible) __asm__("cUIState_setVisible");
    void setParentState(int parentState) __asm__("cUIState_setParentState");
    int getParentState() __asm__("cUIState_getParentState");
    int getScreen() __asm__("cUIState_getScreen");
    void setUserNum(signed char userNum) __asm__("cUIState_setUserNum");
    signed char getUserNum() __asm__("cUIState_getUserNum");
};

CHECK_SIZE(tAttrState, 0x2);

CHECK_OFFSET(cUIState, mHashName, 0x00C);
CHECK_OFFSET(cUIState, mpEngine, 0x010);
CHECK_OFFSET(cUIState, mCurrentCtrl, 0x014);
CHECK_OFFSET(cUIState, mCtrlBits, 0x015);
CHECK_OFFSET(cUIState, miMiscData, 0x018);
CHECK_OFFSET(cUIState, mAttrState, 0x01C);
CHECK_OFFSET(cUIState, mpParentState, 0x020);
CHECK_OFFSET(cUIState, mScreens, 0x024);
CHECK_OFFSET(cUIState, mpScreen, 0x040);
CHECK_OFFSET(cUIState, mUserNum, 0x044);
CHECK_SIZE(cUIState, 0x48);

#endif /* UI_UISTATE_H */
