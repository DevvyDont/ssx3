#ifndef FE_FEMEMCARD_H
#define FE_FEMEMCARD_H
#pragma interface

// cFEMemCard: the memory-card screen base (load / save / delete / format flow on top of cMCOverlayManager). The FE and
// overlay save screens derive from it: cFEStateMemCard, cFEStateProfileLoad, cFEStateOptionsDeviceSelect,
// cOVState_AUTOSAVE, cOVState_SAVEOPTIONS, cOVState_SAVEREPLAY, cOVState_PROFILE (their own fields start at 0x22C).
// EA's source is fe/fememcard.cpp (the July 2003 preview's asserts in cFEMemCard::onDestroyScreen, builds/x/v2
// file_hints). Retail splits it over fe/festatetrophyroom (0x001D5330 ... 0x001D64C0), fe/fememcardcallbacks
// (0x001D66B0 ... 0x001D9948) and main/all_in_one_main (the is*Pressed accessors 0x001DDD28 ... 0x001DDE18).
//
// Size 0x22C:
//   [R] every subclass constructor runs cFEMemCard_cFEMemCard2 and then writes its own fields from 0x22C on
//       (cFEStateOptionsDeviceSelect 0x22C-0x238, cFEStateMemCard 0x22C-0x244 (alloc 584 = 0x248), cFEStateProfileLoad
//       0x22C/0x230/0x234/0x27C), and cFEMemCard_initFEMemCard clears the last base word, 0x228.
//   On Tour's DWARF says 0x164: its cOVState is 0x90 (retail 0x9C), its two file names are unsigned short[32] (retail
//   [64]) and its 22 bools are 1 byte (retail: words). With those three changes On Tour's 36 members after the file
//   names fill retail's 0x19C-0x22C exactly, in On Tour's order, and every one is confirmed by retail use below.
//
// Base: cOVState (fe/ovstate.h, 0x9C) : cUIState (0x48) : cListNode (vptr at 0x8). There are two constructors:
// cFEMemCard_cFEMemCard2(engine, iType) (cOVState inlined, mOVID = 0) and cFEMemCard_cFEMemCard(engine, iType, eOVState)
// (mOVID = eOVState; used by cOVState_SAVEREPLAY & co. via ovstatepause.cpp), plus the default one
// cFEMemCard_cFEMemCard1. All three store cOVState_vtbl, then cFEMemCard_vtbl (_vt$10cFEMemCard, 47 slots) at 0x8.
// The one live screen is the class static cFEMemCard::mInstance = cFEMemCard_mInstance (D_004A2028), set by
// cFEMemCard_initFEMemCard; the static callbacks (cFEMemCard_callback*, confirmLoad, exitDelete, readStart, readDone,
// writeStart, callbackRead/WriteUpdate, is*Pressed) work through it instead of `this`.
//
// Derives from cOVState (fe/ovstate.h); its own members at their retail offsets.
// Evidence tags:
//   [R] retail's constructors and cFEMemCard_initFEMemCard (which sets every own word: 0x19C = 25, 0x1BC = iType,
//       the rest 0; the short at 0x9C = 0; the file-name pattern into 0x11C);
//   [U] how retail code uses the field (cFEMemCard_* asm and C bodies, and the subclasses' methods);
//   [O] SSX On Tour's DWARF (class cFEMemCard): member names, types and order.
// Retail bools are words (int here); On Tour's types are noted where they differ.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cKeyboardPopup;
struct cFEPopupConfirm;

struct cMCOverlayManager;

struct cFEMemCard : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEMemCard_vtbl (_vt$10cFEMemCard), stored over cOVState_vtbl by the ctors; [U] the screen's virtual calls
    //       (getPopup, cancel, triggerDisplayState ...)
    // cFEMemCard's notes on cUIState's fields:
    //   mpEngine: [R] the ctor's engine argument; [U] cFEMemCard_getPopup passes it to cFEPopupConfirm and pushes the
    //       popup on its state stack (+0x18)
    //   mpScreen: [U] cUIScreen_getObjectByHashName(mpScreen, "onmenu" ...) (onGainFocus, setMenuHighlight,
    //       triggerDisplayState, triggerArrowPulse, onUpdate)
    //   mUserNum: [U] cFEStateProfileLoad's ctor sets it from mPlayerIdx
    // cOVState (fe/ovstate.h, 0x9C)
    // cFEMemCard's notes on cOVState's fields:
    //   mOVID: [O] tOVStateEnum; [R] 0, or cFEMemCard_cFEMemCard's eOVState argument
    // --- cFEMemCard
    // cFEMemCard's own slots ([R] cFEMemCard_vtbl; types [O] On Tour's class cFEMemCard):
    virtual void displayStatus();                                                   // 26
    virtual void updateChecking()
        __asm__("cFEMemCard_updateChecking__FPv");                                                  // 27
    virtual void updateSaving()
        __asm__("cFEMemCard_updateSaving__FPv");                                                    // 28
    virtual void updateLoading()
        __asm__("cFEMemCard_updateLoading__FPv");                                                   // 29
    virtual void updateDeleting()
        __asm__("cFEMemCard_updateDeleting__FPv");                                                  // 30
    virtual void updateStart()
        __asm__("cFEMemCard_updateStart");                                                     // 31
    virtual void updateSkip()
        __asm__("cFEMemCard_updateSkip__FPv");                                                      // 32
    virtual void updateDone();                                                      // 33
    virtual void triggerArrowPulse(const char* strArrowName, const char* strAnimName)
        __asm__("cFEMemCard_triggerArrowPulse"); // 34
    virtual void triggerDisplayState(int gotoScreenState)
        __asm__("cFEMemCard_triggerDisplayState");                          // 35
    virtual void getPopup(unsigned short* ustrDisplay, int bYesOnly, int bNoButtons, int layerOffset, int passUpdate,
                          int bSnowFlake)
        __asm__("cFEMemCard_getPopup");                                          // 36 (On Tour: the flags are bool)
    virtual void hideActionButtons(bool bHide);                                     // 37
    virtual void hideDeleteButton(bool bHide);                                      // 38
    virtual void hideMainButton(bool bLoadHide, bool bSaveHide);                    // 39
    virtual void exit()
        __asm__("cFEMemCard_exit__FPv");                                                            // 40
    virtual void cancel() __asm__("cFEMemCard_cancel");                                                          // 41
    virtual bool displayFileOK()
        __asm__("cFEMemCard_displayFileOK");                                                   // 42
    virtual void setMenuHighlight()
        __asm__("cFEMemCard_setMenuHighlight");                                                // 43
    virtual void confirmLoad();                                                     // 44
    virtual void startLoad()
        __asm__("cFEMemCard_startLoad");                                                       // 45
    virtual void exitDelete();                                                      // 46
    virtual void displayStatusPopup()
        __asm__("cFEMemCard_displayStatusPopup");                                              // 47
    unsigned short mLastLoadFileName[64]; // 0x09C [O] ([32] in On Tour); [R] initFEMemCard clears the first char (sh);
                                    //       [U] cFEMemCard_SetLoadPressed(1) has the card (manager +0x434, slot 14)
                                    //       write
                                    //       the selected file's title here, SetLoadPressed(0) clears it
    unsigned short mFileName[64];   // 0x11C [O] ([32] in On Tour); [R] initFEMemCard sprintf1s the "<product>-<type>*"
                                    //       file pattern into it (cMCOverlayManager_getFileName, "%s-%s*")
    int mPopupType;                 // 0x19C [O] (unsigned int in On Tour); [R] initFEMemCard sets 25; [U] the popup id:
                                    //       each callback sets it before getPopup, onPopupEvent switches on it
    int mProcessingKeyboard;        // 0x1A0 [O] (bool); [U] cFEStateProfileLoad_onWidgetEvent sets it when it pushes
                                    //       the name keyboard; onPopupEvent (event 22) clears it and mpKeyboard
    cKeyboardPopup* mpKeyboard;     // 0x1A4 [O]; [U] cFEStateProfileLoad_onWidgetEvent's keyboard popup; onUpdate skips
                                    //       the base update while it is set
    int mIsProcessing;              // 0x1A8 [O] (bool); [U] returned by cFEMemCard_isDeviceSelected; cleared by
                                    //       cancel, exitSave, exitDelete, callbackReadSuccess; tested by onInputBegin
    int mIsDone;                    // 0x1AC [O] (bool); [U] the subclasses' updateDone / exitScreen test and set it
    int mIsLoadWait;                // 0x1B0 [O] (bool); [U] readStart sets 1; onUpdate: busy while 1 and
                                    //       cMCOverlayManager_isManagerDone is false
    int mIsLoadDoneOK;              // 0x1B4 [O] (bool); [U] readStart clears it, callbackReadSuccess sets it;
                                    //       onDestroyScreen tests it
    int mIsInvalidConfirmed;        // 0x1B8 [O] (bool); [R] initFEMemCard clears it (no other retail use found)
    int mType;                      // 0x1BC [O]; [R] initFEMemCard's iType; [U] 0 profile, 1 replay, 2 settings
                                    //       (readStart, readDone, writeStart, callbackRead/WriteUpdate)
    int mState;                     // 0x1C0 [O]; [U] the screen state onUpdate dispatches on: 0 updateStart,
                                    //       1 updateChecking, 2 updateSkip, 3 updateSaving, 4 updateDeleting,
                                    //       5 updateLoading, 6 updateDone; exitSave / exitDelete set it back to 1
    int mSlot;                      // 0x1C4 [O]; [U] the selected menu row (setMenuHighlight stores it); readStart
                                    //       hands
                                    //       it to the manager (+0xF8) and to cMCOverlayManager_readFile
    int mIsReplayAlloc;             // 0x1C8 [O] (bool); [U] writeStart sets it for a replay save;
                                    //       cOVState_SAVEREPLAY_onDestroyScreen / onPopupEvent test and clear it
    int mPlayerIdx;                 // 0x1CC [O] (unsigned int in On Tour); [U] the profile slot for
                                    //       cBESaveInterface_generateSaveProfile (writeStart) / loadSaveProfile
                                    //       (readDone);
                                    //       set by the cFEStateProfileLoad / cOVState_PROFILE ctors
    char* mpReadBuffer;             // 0x1D0 [O]; [U] the read cursor: createReadBuffer sets it to the new block,
                                    //       callbackReadUpdate advances it, readDone loads from it
    char* mpStartReadBuffer;        // 0x1D4 [O]; [U] the block itself: createReadBuffer / readDone / onDestroyScreen
                                    //       free it when mBufferState is 1; writeStart writes the replay from it
    int mIsManagerStarted;          // 0x1D8 [O] (bool); [U] onUpdate starts the manager (ManagerStart) once and sets it
    int mIsSavePressed;             // 0x1DC [O] (bool); [U] returned by cFEMemCard_isSavePressed
    int mIsDeletePressed;           // 0x1E0 [O] (bool); [U] returned by cFEMemCard_isDeletePressed; exitDelete clears
                                    //       it
    int mIsCancelPressed;           // 0x1E4 [O] (bool); [U] returned and cleared by cFEMemCard_isCancelPressed
    int mIsFormatPressed;           // 0x1E8 [O] (bool); [U] returned by cFEMemCard_isFormatPressed
    int mIsRetryPressed;            // 0x1EC [O] (bool); [U] returned and cleared by cFEMemCard_callbackIsRetryPressed
    int mIsOverwriteOK;             // 0x1F0 [O] (bool); [U] returned by cFEMemCard_isOWPressed; onPopupEvent sets it
    int mIsLoadPressed;             // 0x1F4 [O] (bool); [U] returned by cFEMemCard_isLoadPressed; SetLoadPressed stores
                                    //       it
    int mDoNextMemCard;             // 0x1F8 [O] (bool); [U] returned and cleared by cFEMemCard_isNextMCPressed;
                                    //       onInputBegin sets it
    int mDoPrevMemCard;             // 0x1FC [O] (bool); [U] returned and cleared by cFEMemCard_isPrevMCPressed;
                                    //       onInputBegin sets it
    int mLoadBEData;                // 0x200 [O] (bool); [U] readStart sets it for a replay read; callbackReadUpdate
                                    //       CRC-checks and loads the back end once, then clears it
    int mAutoLoadOn;                // 0x204 [O] (bool); [U] callbackReadSuccess / callbackDeleteDone / onPopupEvent
                                    //       skip
                                    //       the popup and exit when set; onUpdate skips the status popup
    int mAutoSaveOn;                // 0x208 [O] (bool); [U] callbackSaveSuccess exits without a popup when set;
                                    //       cOVState_AUTOSAVE's ctor sets it
    int mFirstCheck;                // 0x20C [O] (bool); [R] initFEMemCard clears it (no other retail use found)
    int mIsStatusPopupOn;           // 0x210 [O] (bool); [U] displayStatusPopup and writeStart set it; getPopup clears
                                    //       it
                                    //       (unless popup 22); onUpdate refreshes the popup text while it is set
    int mScreenStatus;              // 0x214 [O]; [U] the display state: triggerDisplayState stores its argument,
                                    //       updateStart and the Display* callbacks test and set it
    cFEPopupConfirm* mStatusPopup;  // 0x218 [O]; [U] getPopup allocates the cFEPopupConfirm (3068 bytes) into it or
                                    //       re-inits it; callbacks label its buttons (popup slot 26)
    int mLastSelectedDevice;        // 0x21C [O]; [R] initFEMemCard clears it; [U] the subclasses' displayStatus /
                                    //       displayOn store the device they show
    int mDeleteButtonActive;        // 0x220 [O] (bool); [U] cFEStateProfileLoad_hideDeleteButton and the init*Screen
                                    //       functions store it
    int mMainButtonActive;          // 0x224 [O] (bool); [U] cFEStateProfileLoad_hideMainButton and the init*Screen
                                    //       functions store it
    int mBufferState;               // 0x228 [O] eBufferState; [U] 1 = mpStartReadBuffer was allocated by
                                    //       createReadBuffer
                                    //       (freed by readDone / onDestroyScreen), 2 = the save interface's replay
                                    //       buffer
                                    //       (readStart), 0 = none

    // EA's file types (mType). [O] On Tour's anonymous enum cFEMemCard::kTYPE_*, names and values; [U] retail's
    // readStart / readDone / writeStart treat 0 as a profile, 1 as a replay and 2 as the game options.
    enum { kTYPE_PROFILE = 0, kTYPE_REPLAY = 1, kTYPE_GAMEOPTIONS = 2 };

    // EA's screen states (mState). [O] On Tour's anonymous enum of cFEMemCard, names and values; [U] retail's
    // onUpdate dispatches 0 updateStart, 1 updateChecking, 2 updateSkip, 3 updateSaving, 4 updateDeleting,
    // 5 updateLoading, 6 updateDone.
    enum { kSTART = 0, kCHECKING = 1, kSKIP = 2, kSAVING = 3, kDELETING = 4, kLOADING = 5, kDONE = 6 };

    // EA's popup ids (mPopupType). [O] On Tour's anonymous enum of cFEMemCard, names and values; [U] retail's
    // callbacks fit: callbackConfirmOverwrite sets kPOP_OVERWRITE, confirmLoad kPOP_CONFIRMLOAD, callbackConfirmDelete
    // kPOP_DELETE, callbackSaveSuccess kPOP_SAVEDONE, callbackReadSuccess kPOP_LOADDONE, callbackDeleteDone /
    // callbackDeleteFailed kPOP_DELETEDONE / kPOP_DELETEFAIL, callbackConfirmNoCards kPOP_NOCARD, the load / save
    // failures kPOP_LOADFAILED / kPOP_SAVEFAILED, callbackFileCorrupt kPOP_CORRUPT, callbackSaveFileCorrupt
    // kPOP_CORRUPTDELETE, the format callbacks kPOP_FORMAT / kPOP_FORMATDONE / kPOP_FORMATFAIL,
    // callbackConfirmInvalidCard kPOP_INVALID, callbackConfirmSaveTooBig / NoFreeBlocks kPOP_FREESPACE,
    // callbackFileExists kPOP_FILEEXISTS,
    // callbackCardChanged kPOP_CARDCHANGED, displayStatusPopup kPOP_STATUS; initFEMemCard starts at kPOP_OTHER.
    enum {
        kPOP_KEYBOARD = 0, kPOP_OVERWRITE = 1, kPOP_CONFIRMLOAD = 2, kPOP_DELETE = 3, kPOP_MESSAGE = 4,
        kPOP_SAVEDONE = 5, kPOP_LOADDONE = 6, kPOP_DELETEDONE = 7, kPOP_DELETEFAIL = 8, kPOP_NOCARD = 9,
        kPOP_LOADFAILED = 10, kPOP_SAVEFAILED = 11, kPOP_CORRUPT = 12, kPOP_CORRUPTDELETE = 13, kPOP_FORMAT = 14,
        kPOP_FORMATNGC = 15, kPOP_FORMATDONE = 16, kPOP_FORMATFAIL = 17, kPOP_INVALID = 18, kPOP_FREESPACE = 19,
        kPOP_NOSPACE = 20, kPOP_WARNING = 21, kPOP_STATUS = 22, kPOP_FILEEXISTS = 23, kPOP_CARDCHANGED = 24,
        kPOP_OTHER = 25
    };

    // The display states (mScreenStatus, triggerDisplayState's gotoScreenState; names ours). [U] triggerDisplayState
    // jumps to the screen's "off" frame (1: menu off, checking the card), its "file display" frame (2: menu off,
    // checking the files) or its "on" frame (3: the slot menu is live).
    enum { kDISPLAY_OFF = 1, kDISPLAY_FILES = 2, kDISPLAY_ON = 3 };

    // EA's eBufferState (mBufferState). [O] On Tour enum cFEMemCard::eBufferState, names and values; [U] 1 marks a
    // block createReadBuffer allocated, 2 the save interface's replay buffer (readStart).
    enum eBufferState { keBufferState_Invalid = 0, keBufferState_Valid = 1, keBufferState_ReplayExternal = 2 };

    // checkNextAvailableDevice's results (names ours): [U] it returns 1 when the next (or previous) device index is
    // valid and -1 at either end of the device list; the save screens hide the left / right arrow on -1.
    enum { kNoNextDevice = -1, kNextDevice = 1 };

    // Methods bound to their retail symbols by asm labels.
    virtual ~cFEMemCard();   // g++'s own symbol _$_10cFEMemCard (the retail label renamed)
    void exitSave() __asm__("cFEMemCard_exitSave__FPv");
    void SetLoadPressed(int on) __asm__("cFEMemCard_SetLoadPressed");
    int checkNextAvailableDevice(cMCOverlayManager* list, int idx, int back)
        __asm__("cFEMemCard_checkNextAvailableDevice");
    void createReadBuffer(unsigned int size) __asm__("cFEMemCard_createReadBuffer");
    void* cFEMemCard2(int pEngine, int iType) __asm__("cFEMemCard_cFEMemCard2");
    void initFEMemCard(int mode) __asm__("cFEMemCard_initFEMemCard");
    void * cFEMemCard1() __asm__("cFEMemCard_cFEMemCard1");
    virtual bool onInputBegin(cUIInput* input) __asm__("cFEMemCard_onInputBegin");
    virtual void onUpdate() __asm__("cFEMemCard_onUpdate");
    virtual bool onLoseTransition() __asm__("cFEMemCard_onLoseTransition");
    virtual void onDestroyScreen() __asm__("cFEMemCard_onDestroyScreen");
    void writeDone() __asm__("cFEMemCard_writeDone__FPv");
    virtual void onPopupEvent(cUIState* popup, unsigned int button) __asm__("cFEMemCard_onPopupEvent");
    virtual void onGainFocus(cUIState*) __asm__("cFEMemCard_onGainFocus");
    virtual void onLoseFocus(cUIState*) __asm__("cFEMemCard_onLoseFocus__FPv");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int key, unsigned int) __asm__("cFEMemCard_onWidgetMenu");
};

CHECK_OFFSET(cFEMemCard, mLastLoadFileName, 0x9C);
CHECK_OFFSET(cFEMemCard, mFileName, 0x11C);
CHECK_OFFSET(cFEMemCard, mPopupType, 0x19C);
CHECK_OFFSET(cFEMemCard, mProcessingKeyboard, 0x1A0);
CHECK_OFFSET(cFEMemCard, mpKeyboard, 0x1A4);
CHECK_OFFSET(cFEMemCard, mIsProcessing, 0x1A8);
CHECK_OFFSET(cFEMemCard, mIsDone, 0x1AC);
CHECK_OFFSET(cFEMemCard, mIsLoadWait, 0x1B0);
CHECK_OFFSET(cFEMemCard, mIsLoadDoneOK, 0x1B4);
CHECK_OFFSET(cFEMemCard, mIsInvalidConfirmed, 0x1B8);
CHECK_OFFSET(cFEMemCard, mType, 0x1BC);
CHECK_OFFSET(cFEMemCard, mState, 0x1C0);
CHECK_OFFSET(cFEMemCard, mSlot, 0x1C4);
CHECK_OFFSET(cFEMemCard, mIsReplayAlloc, 0x1C8);
CHECK_OFFSET(cFEMemCard, mPlayerIdx, 0x1CC);
CHECK_OFFSET(cFEMemCard, mpReadBuffer, 0x1D0);
CHECK_OFFSET(cFEMemCard, mpStartReadBuffer, 0x1D4);
CHECK_OFFSET(cFEMemCard, mIsManagerStarted, 0x1D8);
CHECK_OFFSET(cFEMemCard, mIsSavePressed, 0x1DC);
CHECK_OFFSET(cFEMemCard, mIsDeletePressed, 0x1E0);
CHECK_OFFSET(cFEMemCard, mIsCancelPressed, 0x1E4);
CHECK_OFFSET(cFEMemCard, mIsFormatPressed, 0x1E8);
CHECK_OFFSET(cFEMemCard, mIsRetryPressed, 0x1EC);
CHECK_OFFSET(cFEMemCard, mIsOverwriteOK, 0x1F0);
CHECK_OFFSET(cFEMemCard, mIsLoadPressed, 0x1F4);
CHECK_OFFSET(cFEMemCard, mDoNextMemCard, 0x1F8);
CHECK_OFFSET(cFEMemCard, mDoPrevMemCard, 0x1FC);
CHECK_OFFSET(cFEMemCard, mLoadBEData, 0x200);
CHECK_OFFSET(cFEMemCard, mAutoLoadOn, 0x204);
CHECK_OFFSET(cFEMemCard, mAutoSaveOn, 0x208);
CHECK_OFFSET(cFEMemCard, mFirstCheck, 0x20C);
CHECK_OFFSET(cFEMemCard, mIsStatusPopupOn, 0x210);
CHECK_OFFSET(cFEMemCard, mScreenStatus, 0x214);
CHECK_OFFSET(cFEMemCard, mStatusPopup, 0x218);
CHECK_OFFSET(cFEMemCard, mLastSelectedDevice, 0x21C);
CHECK_OFFSET(cFEMemCard, mDeleteButtonActive, 0x220);
CHECK_OFFSET(cFEMemCard, mMainButtonActive, 0x224);
CHECK_OFFSET(cFEMemCard, mBufferState, 0x228);
CHECK_SIZE(cFEMemCard, 0x22C);
__asm__("_vt$10cFEMemCard = cFEMemCard_vtbl");   // g++'s name for the retail vtable

// cFEMemCard::mInstance (D_004A2028), the live memory-card screen ([O] static cFEMemCard* mInstance), set by
// cFEMemCard_initFEMemCard
extern cFEMemCard* cFEMemCard_mInstance;

#endif /* FE_FEMEMCARD_H */
