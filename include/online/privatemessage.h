#ifndef ONLINE_PRIVATEMESSAGE_H
#define ONLINE_PRIVATEMESSAGE_H

// cPrivateMessage: one online private (buddy) message: sender, text and a timestamp string. BuddyMan keeps the received
// ones in an EA STL vector<cPrivateMessage*> (online/buddyman.h mMessagesStart..End); cFEStateReadMessagePopup keeps
// a conversation in another (fe/festatereadmessagepopup.h mMessages). EA's source path is not known (no assert names
// it); the header sits with BuddyMan, its owner.
//
// Size 0x10: [R] every allocation is cMemMan_alloc(0x10, "cPrivateMessage", 0x20000000, 0) / operator new(0x10,
// "cPrivateMessage") (func_00261CD0 in dirtysock/tagsunk, func_001A7ED8 / func_001A7F78 in fe/feasyncfile), each
// followed by the inline ctor (vptr, then three cBXString ctors). [P] the preview's linkonce _vt$15cPrivateMessage,
// _$_15cPrivateMessage (its out-of-line dtor) and vector<const cPrivateMessage*, BXAllocator<..>>. Not in On Tour's
// DWARF. No base class: the class's only virtual is its destructor, so g++ 2.95 puts the vptr after the data (0xC).
// Vtable cPrivateMessage_vtbl (0x0046D8C0, 1 slot: the deleting dtor cPrivateMessage__cPrivateMessage).
// Methods: cPrivateMessage__cPrivateMessage (main/all_in_one_main, 0x1DCAF0) and its out-of-line twin func_002688E0
// (bx/bxstringctor): vptr, then cBXString dtors on +8, +4, +0, delete when flags & 1.
//
// A FLAT layout. Evidence tags: [R] retail allocation, inline constructor and destructor; [P] preview symbols; [U] how
// retail code uses the field. The member names are ours, from what the constructors are given.

#include "layout.h"
#include "bx/bxstring.h"

struct cPrivateMessage {
    cBXString mSender;              // 0x0 [R] ctor cBXString(name) first; dtor destroys it last; [U] BuddyMan_onReceiveMessage
                                    //     passes the TagField "USER" (func_00261CD0 name); func_001A7ED8 the own persona;
                                    //     func_001A7F78 compares it with cFEStateReadMessagePopup::mszSender (strcasecmp)
    cBXString mText;                // 0x4 [R] ctor second; [U] BuddyMan_onReceiveMessage: "SUBJ" + "BODY"; func_001A7ED8:
                                    //     the reply text
    cBXString mDate;                // 0x8 [R] ctor third; [U] BuddyMan_onReceiveMessage: the "TIME" field printed as
                                    //     "%02d/%02d/%02d %02d:%02d"; func_001A7ED8: D_004A1408 (""); the popup shows it in
                                    //     its "TextFromX" line
    void** mVtbl;                   // 0xC [R] the ctors and both dtors store cPrivateMessage_vtbl here; [P] _vt$15cPrivateMessage
};

CHECK_OFFSET(cPrivateMessage, mSender, 0x0);
CHECK_OFFSET(cPrivateMessage, mText, 0x4);
CHECK_OFFSET(cPrivateMessage, mDate, 0x8);
CHECK_OFFSET(cPrivateMessage, mVtbl, 0xC);
CHECK_SIZE(cPrivateMessage, 0x10);

#endif /* ONLINE_PRIVATEMESSAGE_H */
