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
// cFEStateReadMessagePopup_cFEStateReadMessagePopup (cFEStateNet(pEngine, pParent), vtable cFEStateReadMessagePopup_vtbl, mHashName 0x49) and the default cFEStateReadMessagePopup_cFEStateReadMessagePopup1
// (fe/fepopupmisc); destructor cFEStateReadMessagePopup__cFEStateReadMessagePopup (slot 1: deletes every message, frees the vector, destroys mszSender).
// Retail vtable cFEStateReadMessagePopup_vtbl (32 slots; vtables.tsv lists 24: 4 onCreateScreen cFEStateReadMessagePopup_onCreateScreen, 7 cFEStateReadMessagePopup_onGainTransition, 10
// cFEStateReadMessagePopup_onLoseTransition, 12 onUpdate cFEStateReadMessagePopup_onUpdate, 18 onWidgetCreate cFEStateReadMessagePopup_onWidgetCreate, 19 onWidgetEvent
// cFEStateReadMessagePopup_onWidgetEvent, 20 cFEStateReadMessagePopup_onWidgetMenu, 24 handleEvent cFEStateNet_handleEvent, 31 func_001A7D40 (net events), 32 func_001A7B40).
// Methods: fe/feasyncfile 0x1A77A0-0x1A81F0.
//
// Derives from cFEStateNet (fe/festatenet.h); its own members at their retail offsets. There is no On Tour DWARF for
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
#include "ui/uistate.h"
#include "fe/festatenet.h"
#include "ui/uithing.h"

struct cPrivateMessage;

// The conversation at 0x6D8: an EA STL vector of cPrivateMessage* (the preview instantiates
// vector<const cPrivateMessage*, BXAllocator<..>>; online/buddyman.h keeps the received ones in the same kind of
// vector). A cPrivateMessage is 0x10 bytes: three cBXStrings (sender, text, a third one shown in the "TextFromX"
// line) and its vptr cPrivateMessage_vtbl at 0xC (cMemMan_alloc(0x10, "cPrivateMessage") in func_001A7ED8 / func_001A7F78).
struct tFEPrivateMessageVector {
    int unk_00;                     // 0x0 never read or written directly; the vector's address (0x6D8) is what
                                    //     the vector helpers (func_001DB918 insert) take
    cPrivateMessage** mpStart;      // 0x4 [R] 0 (cFEStateReadMessagePopup_cFEStateReadMessagePopup1); [U] message i = mpStart[i] (func_001A81F0)
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

struct cFEStateReadMessagePopup : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateReadMessagePopup_vtbl (vtables.tsv), stored by cFEStateReadMessagePopup_cFEStateReadMessagePopup /
    //       cFEStateReadMessagePopup_cFEStateReadMessagePopup1 after the cFEStateNet ctor and by the destructor
    //       cFEStateReadMessagePopup__cFEStateReadMessagePopup
    // cFEStateReadMessagePopup's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateReadMessagePopup_cFEStateReadMessagePopup
    //       stores 0x49
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] func_001A92D8 calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateReadMessagePopup
    char* mszSender;                // 0x6D0 [R] cBXString built from the ctor's 4th argument (cBXString_cBXString2),
                                    //       the empty rep cBXString_EmptyString in the default ctor, destroyed by cFEStateReadMessagePopup__cFEStateReadMessagePopup;
                                    //       [U] cFEStateReadMessagePopup_onWidgetEvent sends the typed reply to it (func_00262690) and titles
                                    //       the keyboard with it; func_001A7F78 moves BuddyMan's messages from it
                                    //       into mMessages; func_001A81F0 shows it ("kT_POPUP242PlayerID")
    int mScrollIndex;               // 0x6D4 [R] 0; [U] cFEStateReadMessagePopup_onWidgetMenu (slot 20, event 4: the text scroller moved)
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

CHECK_OFFSET(cFEStateReadMessagePopup, mszSender, 0x6D0);
CHECK_OFFSET(cFEStateReadMessagePopup, mScrollIndex, 0x6D4);
CHECK_OFFSET(cFEStateReadMessagePopup, mMessages, 0x6D8);
CHECK_OFFSET(cFEStateReadMessagePopup, mbResetScroll, 0x6E8);
CHECK_OFFSET(cFEStateReadMessagePopup, mTextColour, 0x6EC);
CHECK_SIZE(cFEStateReadMessagePopup, 0x6FC);

#endif /* FE_FESTATEREADMESSAGEPOPUP_H */
