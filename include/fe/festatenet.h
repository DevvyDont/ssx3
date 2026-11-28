#ifndef FE_FESTATENET_H
#define FE_FESTATENET_H
#pragma interface

// cFEStateNet: the base of every online front-end state (the 29 classes in fe/feasyncfile.cpp: cFEStateNetMessenger,
// cFEStateLogin, cFEStateLobbyMenu, cFEStateNetConfig, cFEStateCreateStep1/2, cFEStateStats, cFEBlankPopup, ...,
// plus cFEStateBraggingRights in festateruleselect.cpp). A cUIState that adds the net popups (mpPopup, mpKeyboard),
// three unicode text buffers, an ascii mask buffer and a queue of pending net messages.
//
// Size 0x6D0: cFEStateOnlineMainMenu, cFEStateNetNews and cFEStateNetSSXWorld are allocated with
// cMemMan_alloc(0x6D0, "<class name>") and their constructors (cFEStateOnlineMainMenu_cFEStateOnlineMainMenu, cFEStateNetNews_cFEStateNetNews, cFEStateNetSSXWorld_cFEStateNetSSXWorld) only
// call this class's constructor and store mHashName and the vtable; every other subclass's own fields start at 0x6D0.
// Constructors: cFEStateNet(cUIEngine*, cUIState*) = cFEStateNet_cFEStateNet (unit fe/feasyncfile), default copy cFEStateNet_cFEStateNet1
// (main/all_in_one_main); destructor cFEStateNet__cFEStateNet. Retail vtable _vt$11cFEStateNet = cFEStateNet_vtbl (32 slots; slots 24-31
// are its own virtuals: func_001DE868, cFEStateNet_validatePushState, cFEStateNet_pollPendingMessages, cFEStateNet_blinkPendingHints, func_001DE870, cFEStateNet_isProcessingEvents,
// cFEStateNet_onNetEvent, cFEStateNet_publishPresence). Methods: fe/feasyncfile 0x1A8500-0x1ABED8, plus the out-of-line inlines
// cFEStateNet_onInputBegin/func_001DE868/func_001DE870/cFEStateNet_setLeavingOnline/cFEStateNet_setUserPopupOpen/cFEStateNet_isProcessingEvents in main/all_in_one_main.
// EA source: probably d:/ssx/code/source/fe/festatenet.cpp (the preview's assert strings name its subclasses' files,
// festatenetmessenger.cpp, festatenetlobby.cpp ..., not this one).
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. Subclasses: retype a subclass method
// with this header only when it touches nothing past 0x6D0; otherwise use the subclass's own header (e.g.
// fe/festatenetmessenger.h).
// Evidence tags:
//   [R] retail's constructors cFEStateNet_cFEStateNet / cFEStateNet_cFEStateNet1 and destructor cFEStateNet__cFEStateNet (and the cUIState
//       constructor cUIState_cUIState1 for the base);
//   [P] the July 2003 preview: _vt$11cFEStateNet, its inline isProcessingEvents (lw 0x38 = preview mpScreen),
//       getFEPath/fePathIsOnline (a global, cFEStateNet_fePath in retail, not a member) and its default constructor
//       __11cFEStateNet (a 0xAC-byte class; its list at 0xA4/0xA8 is retail 0x6C8/0x6CC, +0x624: the preview had no
//       text buffers). Preview offsets are not used directly;
//   [U] how retail code uses the field (the methods named above and the subclasses' methods);
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only (On Tour has no cFEStateNet).
// Names: base fields are EA's (ui/uistate.h); cFEStateNet's own fields carry no EA names (no DWARF, no preview
// accessors), so only fields whose use is unambiguous are named, the rest are unk_.

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIText;
struct cScreenPopup;
struct cKeyboardPopup;

// The pending-message queue at 0x6C8: an EA STL list (8 bytes). The constructor allocates its sentinel node with
// cMemMan_alloc(0xC, "StlBlock") and links it to itself; nodes are {next, prev, data} (cFEStateNet_queueEvent inserts one per
// message, handleEvent cFEStateNet_handleEvent walks them, the destructor deletes each message through its vtable).
struct tFEStateNetMsgList {
    int unk_00;                     // 0x0 never read or written directly (list allocator / first word); the list's
                                    //     address (0x6C8) is what the list helpers take
    void* mpNode;                   // 0x4 [R] the sentinel node (cMemMan_alloc(0xC, "StlBlock")); [U] node->next ==
                                    //       mpNode means empty (cFEStateNet_handleEvent, the destructor)
};

CHECK_OFFSET(tFEStateNetMsgList, unk_00, 0x0);
CHECK_OFFSET(tFEStateNetMsgList, mpNode, 0x4);
CHECK_SIZE(tFEStateNetMsgList, 0x8);

struct cUIListBox;
struct online_ErrorEvent;
struct online_NetErrorEvent;

struct cFEStateNet : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] _vt$11cFEStateNet cFEStateNet_vtbl, stored by cFEStateNet_cFEStateNet/cFEStateNet_cFEStateNet1; subclasses
    //       store theirs after calling it
    // cFEStateNet's notes on cUIState's fields:
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // --- cFEStateNet
    int mHasFocus;                  // 0x048 [R] 0; [U] the onGainFocus override cFEStateNet_onGainFocus sets 1;
                                    //       cFEStateNet_isProcessingEvents (own virtual, slot 29) returns mpScreen != 0
                                    //       && mHasFocus != 0 (the preview's isProcessingEvents tested only the screen)
    unsigned int mLastPollFrame;    // 0x04C [R] 0; [U] cFEStateNet_pollPendingMessages (slot 26) polls the session for new messages /
                                    //       challenges at most once per 60 frames and stores the frame here
    unsigned int mMessageHintFrame; // 0x050 [R] 0; [U] cFEStateNet_pollPendingMessages stores the frame when the session's count
                                    //       (BuddyMan_getNumMessages) becomes > 0 and 0 when it drops; cFEStateNet_blinkPendingHints (slot 27)
                                    //       blinks widgets "IconMessageHint"/"TextMessageCount" while
                                    //       (frame - it) % 60 > 30 (frame counter cAppMan_TheAppMan + 0x18)
    unsigned int mChallengeHintFrame; // 0x054 [R] 0; [U] cFEStateNet_pollPendingMessages, the same for the session's other list (the
                                    //       vector at session + 0xBC); cFEStateNet_blinkPendingHints blinks "IconChallengeHint" with it
    int mBlockInput;                // 0x058 (name ours) [R] 0; [U] cFEStateNet_onDisconnectedPopupEvent/cFEStateNet_onNetInterruptedPopupEvent set 1 before pushing
                                    //       cFEStateLogin; cFEStateNet_onInputBegin (slot 7, the onInputBegin
                                    //       override) returns it (input swallowed while set)
    char mszMaskedText[0x40];       // 0x05C [U] cFEStateNet_setMaskedText writes one '*' per character of its string (at most 0x3F;
                                    //       " " when empty) and a NUL; cUIText_setAsciiString(.., mszMaskedText) in
                                    //       cFEUseExistingAccount_handleKeyboardText, cFEStateCreateStep1_showPassword, cFEStateCreateStep1_showConfirmPassword, cFEStateCreateStep1_handleKeyboardText (password boxes)
    unsigned short mustrMessage[0x200]; // 0x09C [U] sprintf1 (EA's unicode sprintf) target that becomes the popup's
                                    //       message (cFEStateNetMessenger_showFindUserError); extent: up to the next used offset
    unsigned short mustrStatusLine1[0x80]; // 0x49C [U] cFEStateNet_fillUserPopup: sprintf1 of a status line, then
                                    //       setPopupConfirmation (cPopupInfo_setPopupConfirmation1, unicode) with it
    unsigned short mustrStatusLine2[0x80]; // 0x59C [U] cFEStateNet_fillUserPopup: the second status line, used the same way
    cKeyboardPopup* mpKeyboard;     // 0x69C [R] 0; [U] cFEStateNet_createKeyboard stores cMemMan_alloc(0x444, "cKeyboardPopup") +
                                    //       cKeyboardPopup_cKeyboardPopup and pushes it; cFEStateNet_openKeyboardWithAsciiLabel/90F0/9150 set its texts;
                                    //       cFEStateNet_onPopupEvent compares the closing popup with it
    cScreenPopup* mpPopup;          // 0x6A0 [R] 0; [U] createPopup cFEStateNet_createPopup: cMemMan_alloc(0x360, "createPopup") +
                                    //       cScreenPopup_cScreenPopup, reused (cPopupInfo::clear) when set; cFEStateNet_showPopup
                                    //       shows it; cFEStateNet_handleDefaultPopupEvent clears it when that popup closes (event 22)
    cUIText* mpKeyboardText;        // 0x6A4 [R] 0; [U] cFEStateNet_createKeyboard stores its a1 (the text box the
                                    //       keyboard edits); cUIText_setAsciiString(mpKeyboardText, mszMaskedText)
    int mNeedsLobby;                // 0x6A8 [R] 1; 0 in the ctors of cFEUseExistingAccount, cFEStateNetConfig,
                                    //       cFEStateAuthDNAS, cFEStateCreateStep1/2, cFEStateCreateDuplicate,
                                    //       cFEStateLogin; [U] while set and the lobby is disconnected, cFEStateNet_validatePushState
                                    //       (slot 25) refuses to push any state but 0x31/0x33/0x4D/0x4E (net config,
                                    //       login, account creation steps 1/2: mHashName) and opens the disconnect
                                    //       popup (cFEStateNet_showDisconnectPopup)
    int mDisconnectPopupOpen;       // 0x6AC [R] 0; [U] cFEStateNet_showDisconnectPopup sets it when it opens the
                                    //       "kT_POPUPCMNNetInterrupted" / "kT_POPUPCMNDisconnected" popup (only when
                                    //       clear); cFEStateNet_onDisconnectedPopupEvent/cFEStateNet_onNetInterruptedPopupEvent clear it, cFEStateNetMessenger_onNetEvent tests it
    int mLeavingOnline;             // 0x6B0 (name ours) [R] 0; [U] set to 1 by cFEStateNet_setLeavingOnline (inline setter) and by
                                    //       cFEStateLogin_onNetConfigPopupEvent / cFEStateOnlineMainMenu_onLogOffPopupEvent as they leave for the network config or
                                    //       main menu and log off; no read seen
    int mEventPending;              // 0x6B4 [R] 0; [U] handleEvent cFEStateNet_handleEvent: 1 while a net event
                                    //       (0xC8..0x116) waits for slot 30 to accept it, 0 once delivered
    void* mpPopupTextBuf;           // 0x6B8 [R] 0; [U] a heap text buffer: cFEStateNet_buildErrorPopup frees and reallocates it and
                                    //       points the popup's message at it; the destructor frees it
    int mPopupResult;               // 0x6BC (name ours) [R] 2; [U] cFEStateNet_onNestedChallengePopupEvent (popup events): 2 on event 21, 0 on
                                    //       event 15; on event 22 it tells the parent state (slot 17, event 0xF) when 0
    char* mszTargetUser;            // 0x6C0 [R] cBXString built from str_sp (cBXString_cBXString2), destroyed by the
                                    //       dtor; [U] cFEStateNet_setTargetUser assigns it (cBXString_operatorE1), cFEStateNet_getTargetUser
                                    //       returns the text: the user name passed to the session calls
                                    //       (BuddyMan_sendMessage); the messenger sets it to the selected buddy
                                    //       (cFEStateNetMessenger_onFoundUserPopupEvent)
    int mUserPopupOpen;             // 0x6C4 (name ours) [R] 0; [U] cFEStateNet_setUserPopupOpen (inline setter) and cFEStateNet_openUserPopup
                                    //       (opens the user-actions popup) set it, the popup's actions clear it;
                                    //       cFEStateNet_openPlayerMessages / cFEStateNet_refreshUserPopup rebuild that popup only when it and mpPopup
                                    //       are set
    tFEStateNetMsgList mMsgList;    // 0x6C8 [R] see tFEStateNetMsgList; [P] the preview's list at 0xA4

    // The front-end path, the global cFEStateNet_fePath (names ours): [P] the preview reads it through getFEPath and
    // fePathIsOnline (path == 1); [U] its setter cFEStateNet_setFEPath is only passed 0 or 1 (fereal enters the online menus
    // with 1; leaving them and ovstates pass 0, which also logs out of the buddy server).
    enum { kFEPathOffline = 0, kFEPathOnline = 1 };

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateNet1() __asm__("cFEStateNet_cFEStateNet1");
    int isProcessingEvents() __asm__("cFEStateNet_isProcessingEvents");
    virtual ~cFEStateNet();   // g++'s own symbol _$_11cFEStateNet (the retail label renamed; still a C function)
    virtual void onCreateScreen() __asm__("cFEStateNet_onCreateScreen__FPv");
    virtual void onUpdate() __asm__("cFEStateNet_onUpdate");
    virtual bool onGainTransition(int) __asm__("cFEStateNet_onGainTransition__FPv");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int msg) __asm__("cFEStateNet_onWidgetEvent");
    virtual void onPopupEvent(cUIState* popup, unsigned int event) __asm__("cFEStateNet_onPopupEvent");
    void showBuddyPresence(const char* name, int mode) __asm__("cFEStateNet_showBuddyPresence");
    void publishPresence() __asm__("cFEStateNet_publishPresence");
    void* validatePushState(cUIState* msg) __asm__("cFEStateNet_validatePushState");
    int onNetEvent(int msg, void* data) __asm__("cFEStateNet_onNetEvent");
    void showNetInterruptedAndLogOut() __asm__("cFEStateNet_showNetInterruptedAndLogOut");
    void openKeyboardWithAsciiLabel(void* textObj, const char* staticLabel, char* inputText, int password, int popupId,
                                    int maxChars)
        __asm__("cFEStateNet_openKeyboardWithAsciiLabel");
    void openKeyboardWithUnicodeLabel(void* textObj, int staticText, char* inputText, int password, int popupId,
                                      int maxChars)
        __asm__("cFEStateNet_openKeyboardWithUnicodeLabel");
    void openKeyboardWithLabelID(void* textObj, int staticText, char* inputText, int password, int popupId,
                                 int maxChars)
        __asm__("cFEStateNet_openKeyboardWithLabelID");
    void createKeyboard(void* textObj, int password, int popupId, int maxChars) __asm__("cFEStateNet_createKeyboard");
    void onMessengerPopupEvent(cUIState* popup, int event) __asm__("cFEStateNet_onMessengerPopupEvent");
    void onNestedChallengePopupEvent(cUIState* popup, unsigned int kind)
        __asm__("cFEStateNet_onNestedChallengePopupEvent");
    void handleDefaultPopupEvent(int id, int event) __asm__("cFEStateNet_handleDefaultPopupEvent");
    void onAccountMemPopupEvent(cUIState* popup, int event) __asm__("cFEStateNet_onAccountMemPopupEvent");
    void onKeyboardPopupEvent(cUIState* popup, int event) __asm__("cFEStateNet_onKeyboardPopupEvent");
    void onChallengeLostPopupEvent(cUIState* popup, int event) __asm__("cFEStateNet_onChallengeLostPopupEvent");
    void onWaitPlayerPopupEvent(int popup, int event, int extra) __asm__("cFEStateNet_onWaitPlayerPopupEvent");
    void onDisconnectedPopupEvent(cUIState* popup, int event) __asm__("cFEStateNet_onDisconnectedPopupEvent");
    void onNetInterruptedPopupEvent(cUIState* popup, int event) __asm__("cFEStateNet_onNetInterruptedPopupEvent");
    void setTargetUser(const char* str) __asm__("cFEStateNet_setTargetUser");
    char* getTargetUser() __asm__("cFEStateNet_getTargetUser__FPv");
    void setMaskedText(const char* str) __asm__("cFEStateNet_setMaskedText");
    void showDisconnectPopup() __asm__("cFEStateNet_showDisconnectPopup");
    void fillYesNoListBox(cUIListBox* box, int value) __asm__("cFEStateNet_fillYesNoListBox");
    void fillOnOffListBox(cUIListBox* box, int value) __asm__("cFEStateNet_fillOnOffListBox");
    void openMessenger() __asm__("cFEStateNet_openMessenger");
    void showPopup(cScreenPopup* popup) __asm__("cFEStateNet_showPopup");
    void* createPopup() __asm__("cFEStateNet_createPopup");
    void buildErrorPopup(void** popup, void* owner, online_ErrorEvent* data) __asm__("cFEStateNet_buildErrorPopup");
    void blinkPendingHints() __asm__("cFEStateNet_blinkPendingHints");
    void pollPendingMessages() __asm__("cFEStateNet_pollPendingMessages");
    int getNetErrorText(online_NetErrorEvent* msg) __asm__("cFEStateNet_getNetErrorText");
    void showWaitPlayerPopup() __asm__("cFEStateNet_showWaitPlayerPopup");
    void showChallengeErrorPopup(int err) __asm__("cFEStateNet_showChallengeErrorPopup");
    void showChallengeLostPopup(int err) __asm__("cFEStateNet_showChallengeLostPopup");
    void openUserPopup(const char* playerName, int fiveOptions) __asm__("cFEStateNet_openUserPopup");
    void refreshUserPopup(const char* playerName, int fiveOptions) __asm__("cFEStateNet_refreshUserPopup");
    void fillUserPopup(cScreenPopup* popup, const char* namePtr, int fiveOptions, int selectedOption)
        __asm__("cFEStateNet_fillUserPopup");
    void openPlayerMessages(const char* playerName, void* popup, int event) __asm__("cFEStateNet_openPlayerMessages");
    void toggleBlockPlayer(const char* playerName, int* outError) __asm__("cFEStateNet_toggleBlockPlayer");
    void challengePlayer(const char* playerName, void* popup, int event, int fromPopup)
        __asm__("cFEStateNet_challengePlayer");
    void toggleBuddy(const char* playerName, int* outError) __asm__("cFEStateNet_toggleBuddy");
    void showMaxBuddiesPopup() __asm__("cFEStateNet_showMaxBuddiesPopup");
    void swallowFirstOptionPopupEvent(void* popup, int kind) __asm__("cFEStateNet_swallowFirstOptionPopupEvent");
    void setLeavingOnline() __asm__("cFEStateNet_setLeavingOnline");
    void setUserPopupOpen(int value) __asm__("cFEStateNet_setUserPopupOpen");
    virtual void onGainFocus(cUIState* pPrevState) __asm__("cFEStateNet_onGainFocus");
};

CHECK_OFFSET(cFEStateNet, mHasFocus, 0x048);
CHECK_OFFSET(cFEStateNet, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateNet, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateNet, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateNet, mBlockInput, 0x058);
CHECK_OFFSET(cFEStateNet, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateNet, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateNet, mustrStatusLine1, 0x49C);
CHECK_OFFSET(cFEStateNet, mustrStatusLine2, 0x59C);
CHECK_OFFSET(cFEStateNet, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateNet, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateNet, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateNet, mNeedsLobby, 0x6A8);
CHECK_OFFSET(cFEStateNet, mDisconnectPopupOpen, 0x6AC);
CHECK_OFFSET(cFEStateNet, mLeavingOnline, 0x6B0);
CHECK_OFFSET(cFEStateNet, mEventPending, 0x6B4);
CHECK_OFFSET(cFEStateNet, mpPopupTextBuf, 0x6B8);
CHECK_OFFSET(cFEStateNet, mPopupResult, 0x6BC);
CHECK_OFFSET(cFEStateNet, mszTargetUser, 0x6C0);
CHECK_OFFSET(cFEStateNet, mUserPopupOpen, 0x6C4);
CHECK_OFFSET(cFEStateNet, mMsgList, 0x6C8);
CHECK_SIZE(cFEStateNet, 0x6D0);
__asm__("_vt$11cFEStateNet = cFEStateNet_vtbl");   // g++'s name for the retail vtable

#endif /* FE_FESTATENET_H */
