#ifndef FE_FESTATENETMESSENGER_H
#define FE_FESTATENETMESSENGER_H

// cFEStateNetMessenger: the online messenger (buddy list) screen, a cFEStateNet. It finds users by name, keeps the
// buddy names it shows in a vector and opens the read-message / challenge popups from it. EA source
// d:/ssx/code/source/fe/festatenetmessenger.cpp (an assert string of the July 2003 preview).
//
// Size 0x6FC: the cFEStateNet method func_001A9B58 allocates it with cMemMan_alloc(0x6FC, "cFEStateNetMessenger")
// and constructs it with cFEStateNetMessenger_cFEStateNetMessenger(mem, mpEngine, self); the constructor stores up to 0x6F8.
// Constructor cFEStateNetMessenger_cFEStateNetMessenger (cFEStateNet ctor cFEStateNet_cFEStateNet, then vtable D_00467F28, mHashName 0x46 and its members),
// destructor cFEStateNetMessenger__cFEStateNetMessenger. Methods: fe/feasyncfile 0x1A3DC8-0x1A7390 (43 functions) and the slots of D_00467F28.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. cListNode, cUIState and
// cFEStateNet are written out inline (copied from fe/festatenet.h; keep the two in step). No EA member names exist
// for this class (no On Tour DWARF, no preview accessors): the own fields are named from their retail use.
// Evidence tags:
//   [R] retail's constructor cFEStateNetMessenger_cFEStateNetMessenger / destructor cFEStateNetMessenger__cFEStateNetMessenger (and cFEStateNet's, see fe/festatenet.h);
//   [P] the July 2003 preview (see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.

#include "layout.h"
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

struct cFEStateNetMessenger {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] D_00467F28 (vtables.tsv; 35 slots), stored by cFEStateNetMessenger_cFEStateNetMessenger after the
                                    //       cFEStateNet ctor and by the destructor cFEStateNetMessenger__cFEStateNetMessenger
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateNetMessenger_cFEStateNetMessenger stores 0x46
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] passed to every child state's ctor
                                    //       (cMemMan_alloc(..) + ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    signed char mCurrentCtrl;       // 0x014 [O]; [R] cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData lw/sw (ui/uistate.h)
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [P] cUIState's accessors lw/sw the word
                                    //       (ui/uistate.h); [R] cUIState ctor sh 0x218
    cUIState* mpParentState;        // 0x020 [O]; [R] the ctor's owner argument; [U] func_001A92D8 calls its slot 17
                                    //       (onPopupEvent)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens (two cListNodes + the list's vptr at 0x3C)
    cUIScreen* mpScreen;            // 0x040 [O]; [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
                                    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    signed char mUserNum;           // 0x044 [O]; [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    char unk_045[0x3];              // 0x045 padding
    // --- cFEStateNet (fe/festatenet.h)
    int unk_048;                    // 0x048 [R] 0; [U] the onGainFocus override cFEStateNet_onGainFocus sets 1; func_001DE8A8 (own
                                    //       virtual, slot 29) returns mpScreen != 0 && unk_048 != 0 (the preview's
                                    //       isProcessingEvents tested only the screen)
    unsigned int mLastPollFrame;    // 0x04C [R] 0; [U] func_001AAB68 (slot 26) polls the session for new messages /
                                    //       challenges at most once per 60 frames and stores the frame here
    unsigned int mMessageHintFrame; // 0x050 [R] 0; [U] func_001AAB68 stores the frame when the session's count
                                    //       (func_00262938) becomes > 0 and 0 when it drops; func_001AA9B0 (slot 27)
                                    //       blinks widgets "IconMessageHint"/"TextMessageCount" while
                                    //       (frame - it) % 60 > 30 (frame counter cAppMan_TheAppMan + 0x18)
    unsigned int mChallengeHintFrame; // 0x054 [R] 0; [U] func_001AAB68, the same for the session's other list (the
                                    //       vector at session + 0xBC); func_001AA9B0 blinks "IconChallengeHint" with it
    int unk_058;                    // 0x058 [R] 0; [U] func_001A9668/func_001A9710 set 1 before pushing cFEStateLogin;
                                    //       func_001DE860 (slot 7, the onInputBegin override) returns it
    char mszMaskedText[0x40];       // 0x05C [U] func_001A98B8 writes one '*' per character of its string (at most 0x3F;
                                    //       " " when empty) and a NUL; cUIText_setAsciiString(.., mszMaskedText) in
                                    //       func_001ACDD0, func_001B9610, func_001B9678, func_001B9820 (password boxes)
    unsigned short mustrMessage[0x200]; // 0x09C [U] sprintf1 (EA's unicode sprintf) target that becomes the popup's
                                    //       message (func_001A4E28); extent: up to the next used offset
    unsigned short unk_49C[0x80];   // 0x49C [U] func_001AB478: sprintf1 of a status line, then setPopupConfirmation
                                    //       (func_001C5838, unicode) with it
    unsigned short unk_59C[0x80];   // 0x59C [U] func_001AB478: the second status line, used the same way
    cKeyboardPopup* mpKeyboard;     // 0x69C [R] 0; [U] func_001A91B0 stores cMemMan_alloc(0x444, "cKeyboardPopup") +
                                    //       cKeyboardPopup_cKeyboardPopup and pushes it; func_001A9090/90F0/9150 set its texts;
                                    //       cFEStateNet_onPopupEvent (onPopupEvent) compares the closing popup with it
    cScreenPopup* mpPopup;          // 0x6A0 [R] 0; [U] createPopup func_001A9C50: cMemMan_alloc(0x360, "createPopup") +
                                    //       cScreenPopup_cScreenPopup, reused (cPopupInfo::clear) when set; func_001A9BC0
                                    //       shows it; func_001A93D0 clears it when that popup closes (event 22)
    cUIText* mpKeyboardText;        // 0x6A4 [R] 0; [U] func_001A91B0 stores its a1 (the text box the
                                    //       keyboard edits); cUIText_setAsciiString(mpKeyboardText, mszMaskedText)
    int unk_6A8;                    // 0x6A8 [R] 1; 0 in the ctors of cFEUseExistingAccount, cFEStateNetConfig,
                                    //       cFEStateAuthDNAS, cFEStateCreateStep1/2, cFEStateCreateDuplicate, cFEStateLogin;
                                    //       [U] while set, func_001A89B0 (slot 25) drops net messages other than kinds
                                    //       0x31/0x33/0x4D/0x4E and opens the disconnect popup (func_001A9930)
    int unk_6AC;                    // 0x6AC [R] 0; [U] func_001A9930 sets it when it opens the "kT_POPUPCMNNetInterrupted"/
                                    //       "kT_POPUPCMNDisconnected" popup (only when clear); func_001A9668/func_001A9710
                                    //       clear it, func_001A5010 tests it
    int unk_6B0;                    // 0x6B0 [R] 0; [U] set to 1 by func_001DE890 (inline setter), func_001BF5A8,
                                    //       func_001C0138; no read seen
    int unk_6B4;                    // 0x6B4 [R] 0; [U] handleEvent cFEStateNet_handleEvent: 1 while a net event (0xC8..0x116)
                                    //       waits for slot 30 to accept it, 0 once delivered
    void* unk_6B8;                  // 0x6B8 [R] 0; [U] a heap text buffer: func_001A9CC0 frees and reallocates it and
                                    //       points the popup's message at it; the destructor frees it
    int unk_6BC;                    // 0x6BC [R] 2; [U] func_001A92D8 (popup events): 2 on event 21, 0 on event 15;
                                    //       on event 22 it tells the parent state (slot 17, event 0xF) when 0
    char* unk_6C0;                  // 0x6C0 [R] cBXString built from D_004A1390 (cBXString_cBXString2), destroyed by the
                                    //       dtor; [U] func_001A9890 assigns it (cBXString_cBXString4), func_001A98B0
                                    //       returns the text: the user name passed to the session calls (func_00262690);
                                    //       the messenger sets it to the selected buddy (func_001A4928)
    int unk_6C4;                    // 0x6C4 [R] 0; [U] func_001DE8A0 (inline setter) and func_001AB370 set it,
                                    //       func_001ABB30 opens the status popup only when it and mpPopup are set
    tFEStateNetMsgList mMsgList;    // 0x6C8 [R] see tFEStateNetMsgList; [P] the preview's list at 0xA4
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
    char* unk_6E0;                  // 0x6E0 [R] cBXString (as above); no other access seen
    tFEStringPtrVector mNames;      // 0x6E4 [R] zeroed by cFEStateNetMessenger_cFEStateNetMessenger, freed by cFEStateNetMessenger__cFEStateNetMessenger after func_001A5450
                                    //       destroys every element; [U] func_001A54D8 / func_001A5898 push_back
                                    //       cMemMan_alloc(4, "cBXString") copies of the session's buddy names,
                                    //       func_001A56D0/func_001A5768/func_001A5800 search it by name
    int miScrollOffset;             // 0x6F4 [R] 0; [U] cFEStateNetMessenger_onWidgetMenu (onWidgetMenu override) stores the list's new top
                                    //       row (event 4), func_001A4740 returns it + row, cFEStateNetMessenger_onWidgetEvent adds it to
                                    //       the selected row
    int miSelectedIndex;            // 0x6F8 [R] -1; [U] func_001A52B0 stores the index of the session's first pending
                                    //       name (func_001A5800) or -1; cFEStateNetMessenger_onInputBegin passes it to func_001A6C40
};

CHECK_OFFSET(cFEStateNetMessenger, mpPrev, 0x000);
CHECK_OFFSET(cFEStateNetMessenger, mpNext, 0x004);
CHECK_OFFSET(cFEStateNetMessenger, mVtbl, 0x008);
CHECK_OFFSET(cFEStateNetMessenger, mHashName, 0x00C);
CHECK_OFFSET(cFEStateNetMessenger, mpEngine, 0x010);
CHECK_OFFSET(cFEStateNetMessenger, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateNetMessenger, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateNetMessenger, unk_016, 0x016);
CHECK_OFFSET(cFEStateNetMessenger, miMiscData, 0x018);
CHECK_OFFSET(cFEStateNetMessenger, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateNetMessenger, mpParentState, 0x020);
CHECK_OFFSET(cFEStateNetMessenger, mScreens, 0x024);
CHECK_OFFSET(cFEStateNetMessenger, mpScreen, 0x040);
CHECK_OFFSET(cFEStateNetMessenger, mUserNum, 0x044);
CHECK_OFFSET(cFEStateNetMessenger, unk_045, 0x045);
CHECK_OFFSET(cFEStateNetMessenger, unk_048, 0x048);
CHECK_OFFSET(cFEStateNetMessenger, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateNetMessenger, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateNetMessenger, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateNetMessenger, unk_058, 0x058);
CHECK_OFFSET(cFEStateNetMessenger, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateNetMessenger, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateNetMessenger, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateNetMessenger, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateNetMessenger, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateNetMessenger, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateNetMessenger, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateNetMessenger, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateNetMessenger, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateNetMessenger, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateNetMessenger, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateNetMessenger, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateNetMessenger, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateNetMessenger, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateNetMessenger, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateNetMessenger, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateNetMessenger, mszFoundName, 0x6D0);
CHECK_OFFSET(cFEStateNetMessenger, mszSearchName, 0x6D4);
CHECK_OFFSET(cFEStateNetMessenger, unk_6D8, 0x6D8);
CHECK_OFFSET(cFEStateNetMessenger, mLastRefreshFrame, 0x6DC);
CHECK_OFFSET(cFEStateNetMessenger, unk_6E0, 0x6E0);
CHECK_OFFSET(cFEStateNetMessenger, mNames, 0x6E4);
CHECK_OFFSET(cFEStateNetMessenger, miScrollOffset, 0x6F4);
CHECK_OFFSET(cFEStateNetMessenger, miSelectedIndex, 0x6F8);
CHECK_SIZE(cFEStateNetMessenger, 0x6FC);

#endif /* FE_FESTATENETMESSENGER_H */
