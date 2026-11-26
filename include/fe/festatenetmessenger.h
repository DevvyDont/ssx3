#ifndef FE_FESTATENETMESSENGER_H
#define FE_FESTATENETMESSENGER_H

// cFEStateNetMessenger: the online messenger (buddy list) screen, a cFEStateNet. It finds users by name, keeps the
// buddy names it shows in a vector and opens the read-message / challenge popups from it. EA source
// d:/ssx/code/source/fe/festatenetmessenger.cpp (an assert string of the July 2003 preview).
//
// Size 0x6FC: the cFEStateNet method func_001A9B58 allocates it with cMemMan_alloc(0x6FC, "cFEStateNetMessenger")
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
                                    //     vector helpers (func_001DB778 insert) take
    cBXString** mpStart;            // 0x4 [R] 0; [U] element i = mpStart[i]; size = mpFinish - mpStart
    cBXString** mpFinish;           // 0x8 [R] 0; [U] push_back stores here and advances it, func_001A5450 pops from it
    cBXString** mpEndOfStorage;     // 0xC [R] 0; [U] push_back compares mpFinish with it; the dtor frees
                                    //     [mpStart, mpEndOfStorage)
};

CHECK_OFFSET(tFEStringPtrVector, unk_00, 0x0);
CHECK_OFFSET(tFEStringPtrVector, mpStart, 0x4);
CHECK_OFFSET(tFEStringPtrVector, mpFinish, 0x8);
CHECK_OFFSET(tFEStringPtrVector, mpEndOfStorage, 0xC);
CHECK_SIZE(tFEStringPtrVector, 0x10);

struct cFEStateNetMessenger : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateNetMessenger_vtbl (vtables.tsv; 35 slots), stored by cFEStateNetMessenger_cFEStateNetMessenger
    //       after the cFEStateNet ctor and by the destructor cFEStateNetMessenger__cFEStateNetMessenger
    // cFEStateNetMessenger's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateNetMessenger_cFEStateNetMessenger stores 0x46
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] func_001A92D8 calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateNetMessenger
    char* mszFoundName;             // 0x6D0 [R] cBXString: cFEStateNetMessenger_cFEStateNetMessenger stores the empty-string rep cBXString_EmptyString, the dtor
                                    //       destroys it; [U] func_001A4CE8 copies a find result's name (result + 0x8) into
                                    //       it and shows "kT_POPUP279UserFound" with it as option 0; func_001A4928 (that
                                    //       XX
    char* mszSearchName;            // 0x6D4 [R] cBXString (as above); [U] func_001A4810 stores the name typed in the
                                    //       keyboard and submits the find ("kT_POPUP231SubmittingFind",
                                    //       func_0025E6D0); func_001A4E28 prints it into "kT_POPUP279InvalidName"
    int unk_6D8;                    // 0x6D8 [U] func_001A4CE8 copies the find result's word at + 0x10; no read seen
    int mLastRefreshFrame;          // 0x6DC [R] 0; [U] onUpdate override cFEStateNetMessenger_onUpdate: when 1..59 frames have not
                                    //       passed since it (frame counter cAppMan_TheAppMan + 0x18) it only updates, else it
                                    //       stores the frame and rebuilds the list (func_001A6978)
    char* mszShownName;             // 0x6E0 (name ours) [R] cBXString (as above); [U] func_001A6070 copies the shown
                                    //       buddy's roster name into it (cBXString_operatorE1)
    tFEStringPtrVector mNames;      // 0x6E4 [R] zeroed by cFEStateNetMessenger_cFEStateNetMessenger, freed by cFEStateNetMessenger__cFEStateNetMessenger after func_001A5450
                                    //       destroys every element; [U] func_001A54D8 / func_001A5898 push_back
                                    //       cMemMan_alloc(4, "cBXString") copies of the session's buddy names,
                                    //       func_001A56D0/func_001A5768/func_001A5800 search it by name
    int miScrollOffset;             // 0x6F4 [R] 0; [U] cFEStateNetMessenger_onWidgetMenu (onWidgetMenu override) stores the list's new top
                                    //       row (event 4), func_001A4740 returns it + row, cFEStateNetMessenger_onWidgetEvent adds it to
                                    //       the selected row
    int miSelectedIndex;            // 0x6F8 [R] -1; [U] func_001A52B0 stores the index of the session's first pending
                                    //       name (func_001A5800) or -1; cFEStateNetMessenger_onInputBegin passes it to func_001A6C40

    // The ids the messenger's keyboard popups carry (the keyboard's miMiscData; names ours). [U]
    // cFEStateNetMessenger_onWidgetEvent opens the find-user keyboard with kKeyboardFindUser; cFEStateNet's
    // read-or-reply func_001ABB30 opens the reply keyboard with kKeyboardMessage; func_001A4750 dispatches on them.
    enum {
        kKeyboardFindUser = 1,
        kKeyboardMessage = 2
    };

    // A buddy row's status icon (func_001A63C8's state; names ours). [U] func_001A63C8 shows "IconBlocked%02d" for
    // kRowIconBlocked, "IconMessage%02d" for kRowIconMessage and "IconChallenge%02d" for kRowIconChallenge, and none
    // for any other value.
    enum {
        kRowIconBlocked = 0,
        kRowIconMessage = 1,
        kRowIconChallenge = 2
    };

    // A buddy's row class (func_001A4670's result; names ours). [U] func_001A4670 returns kBuddyFlagged for a roster
    // entry with flag 0x100000 (func_00262400), kBuddyOnline when func_00263338 reports it online, else kBuddyOffline;
    // func_001A66A8 colours the row grey, light blue or dark blue by it.
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
};

CHECK_OFFSET(cFEStateNetMessenger, mszFoundName, 0x6D0);
CHECK_OFFSET(cFEStateNetMessenger, mszSearchName, 0x6D4);
CHECK_OFFSET(cFEStateNetMessenger, unk_6D8, 0x6D8);
CHECK_OFFSET(cFEStateNetMessenger, mLastRefreshFrame, 0x6DC);
CHECK_OFFSET(cFEStateNetMessenger, mszShownName, 0x6E0);
CHECK_OFFSET(cFEStateNetMessenger, mNames, 0x6E4);
CHECK_OFFSET(cFEStateNetMessenger, miScrollOffset, 0x6F4);
CHECK_OFFSET(cFEStateNetMessenger, miSelectedIndex, 0x6F8);
CHECK_SIZE(cFEStateNetMessenger, 0x6FC);

#endif /* FE_FESTATENETMESSENGER_H */
