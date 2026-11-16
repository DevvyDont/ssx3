#ifndef FE_FESTATEREADMESSAGEPOPUP_H
#define FE_FESTATEREADMESSAGEPOPUP_H

// cFEStateReadMessagePopup: screen "282ReadPrivateMessage": the private-message conversation with one player (the
// messages BuddyMan received from mszSender plus the ones sent from here) and the keyboard to reply. A cFEStateNet
// state (fe/festatenet.h, 0x6D0), opened from the messenger (fe/festatenetmessenger.h). EA's name: the preview's
// __24cFEStateReadMessagePopup and onLoseTransition__24cFEStateReadMessagePopup. EA's source file is probably
// d:/ssx/code/source/fe/festatenetmessenger.cpp (its code follows cFEStateNetMessenger's in both builds; no assert of
// its own), so this header could also be merged into fe/festatenetmessenger.h.
//
// Size 0x6FC: cMemMan_alloc(0x6FC, "cFEStateReadMessagePopup") + cFEStateReadMessagePopup_cFEStateReadMessagePopup(mem, pEngine, pParent, szSender)
// (func_001ABB30, the only site); the constructor's last store is the 16-byte mTextColour at 0x6EC. Constructors
// cFEStateReadMessagePopup_cFEStateReadMessagePopup (cFEStateNet(pEngine, pParent), vtable cFEStateReadMessagePopup_vtbl, mHashName 0x49) and the default func_001E0FF0
// (fe/fepopupmisc); destructor cFEStateReadMessagePopup__cFEStateReadMessagePopup (slot 1: deletes every message, frees the vector, destroys mszSender).
// Retail vtable cFEStateReadMessagePopup_vtbl (32 slots; vtables.tsv lists 24: 4 onCreateScreen cFEStateReadMessagePopup_onCreateScreen, 7 cFEStateReadMessagePopup_onGainTransition, 10
// cFEStateReadMessagePopup_onLoseTransition (onLoseTransition), 12 onUpdate cFEStateReadMessagePopup_onUpdate, 18 onWidgetCreate cFEStateReadMessagePopup_onWidgetCreate, 19 onWidgetEvent
// cFEStateReadMessagePopup_onWidgetEvent, 20 cFEStateReadMessagePopup_onWidgetMenu, 24 handleEvent func_001A8B88, 31 func_001A7D40 (net events), 32 func_001A7B40).
// Methods: fe/feasyncfile 0x1A77A0-0x1A81F0.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. cListNode, cUIState and
// cFEStateNet are written out inline (copied from fe/festatenet.h; keep them in step). There is no On Tour DWARF for
// these classes (On Tour has no online FE), so the own fields are named from their retail use unless noted.
// Evidence tags:
//   [R] retail's constructors / destructors named above (and cFEStateNet's, see fe/festatenet.h);
//   [P] the July 2003 preview (its default constructors, destructors and inline accessors, see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.
// Popup events (onPopupEvent / the cFEStateNet popup handlers): 0xF + i = option i chosen, 0x14 = back, 0x16 = the
// popup closed (fe/screenpopup.h). The popup id is the cScreenPopup's miMiscData (+ 0x18), set to 0x8xx by its
// creator.

#include "layout.h"
#include "fe/festatenet.h"
#include "ui/uithing.h"

struct cPrivateMessage;

// The conversation at 0x6D8: an EA STL vector of cPrivateMessage* (the preview instantiates
// vector<const cPrivateMessage*, BXAllocator<..>>; online/buddyman.h keeps the received ones in the same kind of
// vector). A cPrivateMessage is 0x10 bytes: three cBXStrings (sender, text, a third one shown in the "TextFromX"
// line) and its vptr D_0046D8C0 at 0xC (cMemMan_alloc(0x10, "cPrivateMessage") in func_001A7ED8 / func_001A7F78).
struct tFEPrivateMessageVector {
    int unk_00;                     // 0x0 never read or written directly; the vector's address (0x6D8) is what
                                    //     the vector helpers (func_001DB918 insert) take
    cPrivateMessage** mpStart;      // 0x4 [R] 0 (func_001E0FF0); [U] message i = mpStart[i] (func_001A81F0)
    cPrivateMessage** mpFinish;     // 0x8 [R] 0; [U] func_001A7DD8 appends here (and drops the oldest past 31),
                                    //     the destructor pops and deletes from it
    cPrivateMessage** mpEndOfStorage; // 0xC [R] 0; [U] func_001A7DD8 compares mpFinish with it; the dtor frees
                                    //     [mpStart, mpEndOfStorage)
};

CHECK_OFFSET(tFEPrivateMessageVector, unk_00, 0x0);
CHECK_OFFSET(tFEPrivateMessageVector, mpStart, 0x4);
CHECK_OFFSET(tFEPrivateMessageVector, mpFinish, 0x8);
CHECK_OFFSET(tFEPrivateMessageVector, mpEndOfStorage, 0xC);
CHECK_SIZE(tFEPrivateMessageVector, 0x10);

struct cFEStateReadMessagePopup {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateReadMessagePopup_vtbl (vtables.tsv), stored by cFEStateReadMessagePopup_cFEStateReadMessagePopup / func_001E0FF0
                                    //       after the cFEStateNet ctor and by the destructor cFEStateReadMessagePopup__cFEStateReadMessagePopup
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateReadMessagePopup_cFEStateReadMessagePopup stores
                                    //       0x49
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
    signed char mUserNum;           // 0x044 [O]; [R] cUIState ctor -1, then func_001A8500 stores 0 (sb)
    char unk_045[0x3];              // 0x045 padding
    // --- cFEStateNet (fe/festatenet.h)
    int unk_048;                    // 0x048 [R] 0; [U] the onGainFocus override func_001A87D0 sets 1; func_001DE8A8 (own
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
                                    //       func_001A97B8 (onPopupEvent) compares the closing popup with it
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
    int unk_6B4;                    // 0x6B4 [R] 0; [U] handleEvent func_001A8B88: 1 while a net event (0xC8..0x116)
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
    // --- cFEStateReadMessagePopup
    char* mszSender;                // 0x6D0 [R] cBXString built from the ctor's 4th argument (cBXString_cBXString2),
                                    //       the empty rep cBXString_EmptyString in the default ctor, destroyed by cFEStateReadMessagePopup__cFEStateReadMessagePopup;
                                    //       [U] cFEStateReadMessagePopup_onWidgetEvent sends the typed reply to it (func_00262690) and titles
                                    //       the keyboard with it; func_001A7F78 moves BuddyMan's messages from it
                                    //       into mMessages; func_001A81F0 shows it ("kT_POPUP242PlayerID")
    int unk_6D4;                    // 0x6D4 [R] 0; [U] cFEStateReadMessagePopup_onWidgetMenu (slot 20, event 4: the text scroller moved)
                                    //       stores the value, then refreshes; no read seen
    tFEPrivateMessageVector mMessages; // 0x6D8 [R] see tFEPrivateMessageVector (the default ctor zeroes
                                    //       0x6DC..0x6E4); [U] func_001A81F0 lists "sender: text" for each message in
                                    //       "textScrollChat"
    int mbResetScroll;              // 0x6E8 [R] 1 (cFEStateReadMessagePopup_cFEStateReadMessagePopup); [U] func_001A81F0 passes it to the text-scroll
                                    //       helper func_001A83D8 (as func_001B2CD0 passes "first fill") and clears it
    tARGB mTextColour;              // 0x6EC [R] 1.0, 1.0, 1.0, 1.0 (an unaligned 16-byte copy: sdl/sdr); [U]
                                    //       onCreateScreen cFEStateReadMessagePopup_onCreateScreen copies it to "textScrollChat" + 0xA8
                                    //       (ldl/ldr)
};

CHECK_OFFSET(cFEStateReadMessagePopup, mpPrev, 0x000);
CHECK_OFFSET(cFEStateReadMessagePopup, mpNext, 0x004);
CHECK_OFFSET(cFEStateReadMessagePopup, mVtbl, 0x008);
CHECK_OFFSET(cFEStateReadMessagePopup, mHashName, 0x00C);
CHECK_OFFSET(cFEStateReadMessagePopup, mpEngine, 0x010);
CHECK_OFFSET(cFEStateReadMessagePopup, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateReadMessagePopup, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_016, 0x016);
CHECK_OFFSET(cFEStateReadMessagePopup, miMiscData, 0x018);
CHECK_OFFSET(cFEStateReadMessagePopup, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateReadMessagePopup, mpParentState, 0x020);
CHECK_OFFSET(cFEStateReadMessagePopup, mScreens, 0x024);
CHECK_OFFSET(cFEStateReadMessagePopup, mpScreen, 0x040);
CHECK_OFFSET(cFEStateReadMessagePopup, mUserNum, 0x044);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_045, 0x045);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_048, 0x048);
CHECK_OFFSET(cFEStateReadMessagePopup, mLastPollFrame, 0x04C);
CHECK_OFFSET(cFEStateReadMessagePopup, mMessageHintFrame, 0x050);
CHECK_OFFSET(cFEStateReadMessagePopup, mChallengeHintFrame, 0x054);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_058, 0x058);
CHECK_OFFSET(cFEStateReadMessagePopup, mszMaskedText, 0x05C);
CHECK_OFFSET(cFEStateReadMessagePopup, mustrMessage, 0x09C);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_49C, 0x49C);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_59C, 0x59C);
CHECK_OFFSET(cFEStateReadMessagePopup, mpKeyboard, 0x69C);
CHECK_OFFSET(cFEStateReadMessagePopup, mpPopup, 0x6A0);
CHECK_OFFSET(cFEStateReadMessagePopup, mpKeyboardText, 0x6A4);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_6A8, 0x6A8);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_6AC, 0x6AC);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_6B0, 0x6B0);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_6B4, 0x6B4);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_6B8, 0x6B8);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_6BC, 0x6BC);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_6C0, 0x6C0);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_6C4, 0x6C4);
CHECK_OFFSET(cFEStateReadMessagePopup, mMsgList, 0x6C8);
CHECK_OFFSET(cFEStateReadMessagePopup, mszSender, 0x6D0);
CHECK_OFFSET(cFEStateReadMessagePopup, unk_6D4, 0x6D4);
CHECK_OFFSET(cFEStateReadMessagePopup, mMessages, 0x6D8);
CHECK_OFFSET(cFEStateReadMessagePopup, mbResetScroll, 0x6E8);
CHECK_OFFSET(cFEStateReadMessagePopup, mTextColour, 0x6EC);
CHECK_SIZE(cFEStateReadMessagePopup, 0x6FC);

#endif /* FE_FESTATEREADMESSAGEPOPUP_H */
