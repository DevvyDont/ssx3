#ifndef FE_FESTATENETMESSENGER_H
#define FE_FESTATENETMESSENGER_H

// cFEStateNetMessenger: the online messenger (buddy list) screen, a cFEStateNet. It finds users by name, keeps the
// buddy names it shows in a vector and opens the read-message / challenge popups from it. EA source
// d:/ssx/code/source/fe/festatenetmessenger.cpp (an assert string of the July 2003 preview).
//
// Size 0x6FC: the cFEStateNet method cFEStateNet_openMessenger allocates it with cMemMan_alloc(0x6FC, "cFEStateNetMessenger")
// and constructs it with cFEStateNetMessenger_cFEStateNetMessenger(mem, mpEngine, self); the constructor stores up to 0x6F8.
// Constructor cFEStateNetMessenger_cFEStateNetMessenger (cFEStateNet ctor cFEStateNet_cFEStateNet, then vtable cFEStateNetMessenger_vtbl, mHashName 0x46 and its members),
// destructor cFEStateNetMessenger__cFEStateNetMessenger. Methods: fe/feasyncfile 0x1A3DC8-0x1A7390 (43 functions) and the slots of cFEStateNetMessenger_vtbl.
//
// Derives from cFEStateNet (fe/festatenet.h); its own members at their retail offsets. No EA member names exist for
// this class (no On Tour DWARF, no preview accessors): the own fields are named from their retail use.
// Evidence tags:
//   [R] retail's constructor cFEStateNetMessenger_cFEStateNetMessenger / destructor cFEStateNetMessenger__cFEStateNetMessenger (and cFEStateNet's, see fe/festatenet.h);
//   [P] the July 2003 preview (see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festatenet.h"

struct cBXString;

// An EA STL vector of cBXString* (0x10 bytes; the begin/end/capacity triple follows one word).
struct tFEStringPtrVector {
    int unk_00;                     // 0x0 never read or written directly; the vector's address (0x6E4) is what the
                                    //     vector helpers (tFEStringPtrVector_insertAux insert) take
    cBXString** mpStart;            // 0x4 [R] 0; [U] element i = mpStart[i]; size = mpFinish - mpStart
    cBXString** mpFinish;           // 0x8 [R] 0; [U] push_back stores here and advances it, cFEStateNetMessenger_clearNames pops from it
    cBXString** mpEndOfStorage;     // 0xC [R] 0; [U] push_back compares mpFinish with it; the dtor frees
                                    //     [mpStart, mpEndOfStorage)

    // Methods bound to their retail symbols by asm labels.
    void insertAux(cBXString** pos, cBXString* const& x) __asm__("tFEStringPtrVector_insertAux");
};

CHECK_OFFSET(tFEStringPtrVector, unk_00, 0x0);
CHECK_OFFSET(tFEStringPtrVector, mpStart, 0x4);
CHECK_OFFSET(tFEStringPtrVector, mpFinish, 0x8);
CHECK_OFFSET(tFEStringPtrVector, mpEndOfStorage, 0xC);
CHECK_SIZE(tFEStringPtrVector, 0x10);

struct cFEStateNetHelpPopup;
struct online_ErrorEvent;
struct online_FindPlayerEvent;

struct cFEStateNetMessenger : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateNetMessenger_vtbl (vtables.tsv; 35 slots), stored by cFEStateNetMessenger_cFEStateNetMessenger
    //       after the cFEStateNet ctor and by the destructor cFEStateNetMessenger__cFEStateNetMessenger
    // cFEStateNetMessenger's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateNetMessenger_cFEStateNetMessenger stores 0x46
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateNetMessenger
    char* mszFoundName;             // 0x6D0 [R] cBXString: cFEStateNetMessenger_cFEStateNetMessenger stores the empty-string rep cBXString_EmptyString, the dtor
                                    //       destroys it; [U] cFEStateNetMessenger_showUserFoundPopup copies a find result's name (result + 0x8) into
                                    //       it and shows "kT_POPUP279UserFound" with it as option 0; cFEStateNetMessenger_onFoundUserPopupEvent (that
                                    //       XX
    char* mszSearchName;            // 0x6D4 [R] cBXString (as above); [U] cFEStateNetMessenger_startFindUser stores the name typed in the
                                    //       keyboard and submits the find ("kT_POPUP231SubmittingFind",
                                    //       LobbyMan_findPlayer); cFEStateNetMessenger_showFindUserError prints it into "kT_POPUP279InvalidName"
    int mbFoundOnline;              // 0x6D8 (name ours) [U] cFEStateNetMessenger_showUserFoundPopup copies the find result's online flag
                                    //       (online_FindPlayerEvent::mOnline); no read seen
    int mLastRefreshFrame;          // 0x6DC [R] 0; [U] onUpdate override cFEStateNetMessenger_onUpdate: when 1..59 frames have not
                                    //       passed since it (frame counter cAppMan_TheAppMan + 0x18) it only updates, else it
                                    //       stores the frame and rebuilds the list (cFEStateNetMessenger_redrawList)
    char* mszShownName;             // 0x6E0 (name ours) [R] cBXString (as above); [U] cFEStateNetMessenger_showBuddyDetails copies the shown
                                    //       buddy's roster name into it (cBXString_operatorE1)
    tFEStringPtrVector mNames;      // 0x6E4 [R] zeroed by cFEStateNetMessenger_cFEStateNetMessenger, freed by cFEStateNetMessenger__cFEStateNetMessenger after cFEStateNetMessenger_clearNames
                                    //       destroys every element; [U] cFEStateNetMessenger_appendFlaggedNames / cFEStateNetMessenger_rebuildNameList push_back
                                    //       cMemMan_alloc(4, "cBXString") copies of the session's buddy names,
                                    //       cFEStateNetMessenger_hasName/cFEStateNetMessenger_findNameIndex/cFEStateNetMessenger_findNameIndexByString search it by name
    int miScrollOffset;             // 0x6F4 [R] 0; [U] cFEStateNetMessenger_onWidgetMenu (onWidgetMenu override) stores the list's new top
                                    //       row (event 4), cFEStateNetMessenger_getNameIndexForRow returns it + row, cFEStateNetMessenger_onWidgetEvent adds it to
                                    //       the selected row
    int miSelectedIndex;            // 0x6F8 [R] -1; [U] cFEStateNetMessenger_selectPendingBuddy stores the index of the session's first pending
                                    //       name (cFEStateNetMessenger_findNameIndexByString) or -1; cFEStateNetMessenger_onInputBegin passes it to cFEStateNetMessenger_selectMenuItem

    // The ids the messenger's keyboard popups carry (the keyboard's miMiscData; names ours). [U]
    // cFEStateNetMessenger_onWidgetEvent opens the find-user keyboard with kKeyboardFindUser; cFEStateNet's
    // read-or-reply cFEStateNet_openPlayerMessages opens the reply keyboard with kKeyboardMessage; cFEStateNetMessenger_handleKeyboardText dispatches on them.
    enum {
        kKeyboardFindUser = 1,
        kKeyboardMessage = 2
    };

    // A buddy row's status icon (cFEStateNetMessenger_setRowStatusIcon's state; names ours). [U] cFEStateNetMessenger_setRowStatusIcon shows "IconBlocked%02d" for
    // kRowIconBlocked, "IconMessage%02d" for kRowIconMessage and "IconChallenge%02d" for kRowIconChallenge, and none
    // for any other value.
    enum {
        kRowIconBlocked = 0,
        kRowIconMessage = 1,
        kRowIconChallenge = 2
    };

    // A buddy's row class (cFEStateNetMessenger_getBuddyStatus's result; names ours). [U] cFEStateNetMessenger_getBuddyStatus returns kBuddyFlagged for a roster
    // entry with flag 0x100000 (BuddyMan_isTempEntry), kBuddyOnline when func_00263338 reports it online, else kBuddyOffline;
    // cFEStateNetMessenger_colourRow colours the row grey, light blue or dark blue by it.
    enum {
        kBuddyFlagged = 0,
        kBuddyOffline = 1,
        kBuddyOnline = 2
    };

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateNetMessenger1() __asm__("cFEStateNetMessenger_cFEStateNetMessenger1");
    // retail's destructor cFEStateNetMessenger__cFEStateNetMessenger stays a C function (g++ 2.95 takes no asm label on
    // a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateNetMessenger();
    virtual void onCreateScreen() __asm__("cFEStateNetMessenger_onCreateScreen");
    virtual void onUpdate() __asm__("cFEStateNetMessenger_onUpdate");
    virtual bool onInputBegin(cUIInput* obj) __asm__("cFEStateNetMessenger_onInputBegin");
    virtual bool onLoseTransition() __asm__("cFEStateNetMessenger_onLoseTransition__FPv");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int event, unsigned int value)
        __asm__("cFEStateNetMessenger_onWidgetMenu");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int msg) __asm__("cFEStateNetMessenger_onWidgetEvent");
    virtual void onPopupEvent(cUIState* pPopup, unsigned int popupEvent) __asm__("cFEStateNetMessenger_onPopupEvent");
    virtual void onWidgetCreate(cUIThing* pWidget) __asm__("cFEStateNetMessenger_onWidgetCreate");
    void fillHelpPopup(cFEStateNetHelpPopup* popup) __asm__("cFEStateNetMessenger_fillHelpPopup");
    int getBuddyStatus(int idx) __asm__("cFEStateNetMessenger_getBuddyStatus");
    int getNameIndexForRow(int row) __asm__("cFEStateNetMessenger_getNameIndexForRow");
    void handleKeyboardText() __asm__("cFEStateNetMessenger_handleKeyboardText");
    void startFindUser(void* text) __asm__("cFEStateNetMessenger_startFindUser");
    void onFoundUserPopupEvent(void* popup, int event) __asm__("cFEStateNetMessenger_onFoundUserPopupEvent");
    void onUserActionsPopupEvent(void* popup, unsigned int kind)
        __asm__("cFEStateNetMessenger_onUserActionsPopupEvent");
    void onOfflineUserActionsPopupEvent(void* popup, unsigned int kind)
        __asm__("cFEStateNetMessenger_onOfflineUserActionsPopupEvent");
    void showUserFoundPopup(online_FindPlayerEvent* src) __asm__("cFEStateNetMessenger_showUserFoundPopup");
    void showLookingForSelfPopup() __asm__("cFEStateNetMessenger_showLookingForSelfPopup");
    void showFindUserError(online_ErrorEvent* msg) __asm__("cFEStateNetMessenger_showFindUserError");
    void handleErrorEvent(int msgId, void* data) __asm__("cFEStateNetMessenger_handleErrorEvent");
    void* onNetEvent(int eventType, void* eventData) __asm__("cFEStateNetMessenger_onNetEvent");
    void placeScrollThumb(int pos, int count) __asm__("cFEStateNetMessenger_placeScrollThumb");
    void selectPendingBuddy() __asm__("cFEStateNetMessenger_selectPendingBuddy");
    void clearNames() __asm__("cFEStateNetMessenger_clearNames");
    void appendFlaggedNames() __asm__("cFEStateNetMessenger_appendFlaggedNames");
    bool hasName(const char* name) __asm__("cFEStateNetMessenger_hasName");
    int findNameIndex(const char* name) __asm__("cFEStateNetMessenger_findNameIndex");
    int findNameIndexByString(char** name) __asm__("cFEStateNetMessenger_findNameIndexByString");
    void rebuildNameList() __asm__("cFEStateNetMessenger_rebuildNameList");
    void showRowSelectIcon(int sel) __asm__("cFEStateNetMessenger_showRowSelectIcon");
    bool showBuddyDetails(int idx) __asm__("cFEStateNetMessenger_showBuddyDetails");
    void setRowStatusIcon(unsigned int index, int state) __asm__("cFEStateNetMessenger_setRowStatusIcon");
    void setRowName(unsigned int index, const char* str, int rosterIndex) __asm__("cFEStateNetMessenger_setRowName");
    void colourRow(unsigned int index, int flag) __asm__("cFEStateNetMessenger_colourRow");
    void redrawList() __asm__("cFEStateNetMessenger_redrawList");
    void selectMenuItem(int idx) __asm__("cFEStateNetMessenger_selectMenuItem");
    void updatePendingHints() __asm__("cFEStateNetMessenger_updatePendingHints");
    void removeStaleNames() __asm__("cFEStateNetMessenger_removeStaleNames");
    void fillRow(unsigned int index, const char* name, int rosterIndex) __asm__("cFEStateNetMessenger_fillRow");
};

CHECK_OFFSET(cFEStateNetMessenger, mszFoundName, 0x6D0);
CHECK_OFFSET(cFEStateNetMessenger, mszSearchName, 0x6D4);
CHECK_OFFSET(cFEStateNetMessenger, mbFoundOnline, 0x6D8);
CHECK_OFFSET(cFEStateNetMessenger, mLastRefreshFrame, 0x6DC);
CHECK_OFFSET(cFEStateNetMessenger, mszShownName, 0x6E0);
CHECK_OFFSET(cFEStateNetMessenger, mNames, 0x6E4);
CHECK_OFFSET(cFEStateNetMessenger, miScrollOffset, 0x6F4);
CHECK_OFFSET(cFEStateNetMessenger, miSelectedIndex, 0x6F8);
CHECK_SIZE(cFEStateNetMessenger, 0x6FC);

#endif /* FE_FESTATENETMESSENGER_H */
