#ifndef ONLINE_ONLINEEVENT_H
#define ONLINE_ONLINEEVENT_H

// online::Event and its subclasses: the messages the online code (LobbyMan, BuddyMan's update, cConnMan, the setup
// manager cSetupMan) posts to the front end on LobbyMan::mEventList (a std::list<online::Event*>, online/lobbyman.h);
// the front-end states switch on mType and free them. Each class has a 2-slot vtable: 1 the deleting destructor, 2
// clone() (a heap copy). Class names are EA's, from the July 2003 preview's linkonce sections (_vt$Q26online5Event,
// _$_Q26online10ErrorEvent, clone__CQ26online15AcctLockedEvent, ...: the classes live in namespace online and their
// methods are inline, so they are declared in a header). EA's header path is not known; online/ is our choice (the
// online code's directory). There is no On Tour counterpart (On Tour has no online code).
// Struct names follow the fork's Outer_Inner convention for nested names (functions are already online_Event_*).
//
// Retail adds five event classes the preview does not have (vtables 0x00481230, 0x004811B0, 0x00481150, 0x00481130,
// 0x00481110); with only one name signal each they are not declared here (see the group I report), except ChatEvent
// (0x00481230), named after its "ChatEvent" allocation tag like its destructor and clone (online_ChatEvent_*).
//
// Layout: online::Event has one data member and declares the virtuals, so g++ 2.95 puts its vptr after it, at 0x4;
// the subclasses append from 0x8. Every event is allocated with cMemMan_alloc(size, "NetEvent" | "NetErrorEvent" |
// "ChatEvent", 0x20000000). cBXString members are one char* (bx/bxstring.h).
// A FLAT layout per class, like main/game.h: bases written out inline. Evidence tags:
//   [R] the retail destructors (slot 1) / clone (slot 2) and the allocations at the posting sites
//       (src/dirtysock/tagsunk.cpp: LobbyMan_on*, func_0025AC50, func_002636F0);
//   [U] how retail code uses the field.
// The destructors store online_Event_vtbl (g++ drops the dead stores of the derived vtables), so they do not show
// the class tree; AcctTooYoungEvent / AcctLockedEvent carry ErrorEvent's type (258) and code word (0x8), so they
// probably derive from ErrorEvent (laid out inline either way).

#include "layout.h"
#include "online/lobbyman.h"            // tLobbyStats

// online::Event: size 8 ([R] online_Event_clone allocates 8; func_0025F628 / func_0025FC70 / func_002642B8 post
// plain 8-byte events of types 218, 238, 246, 256, 272 ...). Vtable online_Event_vtbl (0x004812B0).
struct online_Event {
    int mType;                      // 0x0 [R] every poster stores it first (222 chat, 226 news, 227 find player,
                                    //     229 ping, 251/252 setup updates, 258 error, 259 buddy error, 269 user record,
                                    //     278 net error, ...); [U] the front end switches on it
    void** mVtbl;                   // 0x4 [R] online_Event_vtbl (clone, destructors)
};

CHECK_OFFSET(online_Event, mType, 0x0);
CHECK_OFFSET(online_Event, mVtbl, 0x4);
CHECK_SIZE(online_Event, 0x8);

// online::ErrorEvent: type 258, size 0xC ([R] online_ErrorEvent_clone allocates 0xC). Vtable online_ErrorEvent_vtbl
// (0x00481290).
struct online_ErrorEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 258
    void** mVtbl;                   // 0x4 [R] online_ErrorEvent_vtbl
    // --- online::ErrorEvent
    int mError;                     // 0x8 [R] clone copies it; [U] a four-character code ('time', 'dnsf' from
                                    //     func_0025AC50) or the LobbyApi reply's error (LobbyMan_onPlayerFound ...)
};

CHECK_OFFSET(online_ErrorEvent, mType, 0x0);
CHECK_OFFSET(online_ErrorEvent, mVtbl, 0x4);
CHECK_OFFSET(online_ErrorEvent, mError, 0x8);
CHECK_SIZE(online_ErrorEvent, 0xC);

// online::BuddyErrorEvent: type 259, size 0xC ([R] func_0025AC50 allocates 0xC). Vtable online_BuddyErrorEvent_vtbl
// (0x00481270). Its clone builds an ErrorEvent (type 258, online_ErrorEvent_vtbl), as in the preview.
struct online_BuddyErrorEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 259
    void** mVtbl;                   // 0x4 [R] online_BuddyErrorEvent_vtbl
    // --- online::BuddyErrorEvent
    int mError;                     // 0x8 [R] func_0025AC50 stores BuddyMan's miError; clone copies it
};

CHECK_OFFSET(online_BuddyErrorEvent, mType, 0x0);
CHECK_OFFSET(online_BuddyErrorEvent, mVtbl, 0x4);
CHECK_OFFSET(online_BuddyErrorEvent, mError, 0x8);
CHECK_SIZE(online_BuddyErrorEvent, 0xC);

// online::NetErrorEvent: type 278, size 0xC ([R] func_002636F0 allocates 0xC with the tag "NetErrorEvent"). Vtable
// online_NetErrorEvent_vtbl (0x00481250).
struct online_NetErrorEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 278
    void** mVtbl;                   // 0x4 [R] online_NetErrorEvent_vtbl
    // --- online::NetErrorEvent
    int mError;                     // 0x8 [R] func_002636F0 stores its argument (the disconnect reason); clone copies it
};

CHECK_OFFSET(online_NetErrorEvent, mType, 0x0);
CHECK_OFFSET(online_NetErrorEvent, mVtbl, 0x4);
CHECK_OFFSET(online_NetErrorEvent, mError, 0x8);
CHECK_SIZE(online_NetErrorEvent, 0xC);

// online::NewsEvent: type 226, size 0xC ([R] online_NewsEvent_clone allocates 0xC). Vtable online_NewsEvent_vtbl
// (0x00481210).
struct online_NewsEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 226
    void** mVtbl;                   // 0x4 [R] online_NewsEvent_vtbl
    // --- online::NewsEvent
    char* mText;                    // 0x8 [R] cBXString: clone builds it from the source's text, the destructor
                                    //     destroys it; [U] LobbyMan_onNewsRetrieved posts the news text
};

CHECK_OFFSET(online_NewsEvent, mType, 0x0);
CHECK_OFFSET(online_NewsEvent, mVtbl, 0x4);
CHECK_OFFSET(online_NewsEvent, mText, 0x8);
CHECK_SIZE(online_NewsEvent, 0xC);

// online::AcctTooYoungEvent: type 258 with code 'tooy', size 0x10 ([R] clone allocates 0x10). Vtable
// online_AcctTooYoungEvent_vtbl (0x004811F0).
struct online_AcctTooYoungEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 258
    void** mVtbl;                   // 0x4 [R] online_AcctTooYoungEvent_vtbl
    // --- online::ErrorEvent (probable base, see above)
    int mError;                     // 0x8 [R] clone stores the constant 'tooy' (0x746F6F79)
    // --- online::AcctTooYoungEvent
    int mAge;                       // 0xC [R] clone copies it; [U] LobbyMan_onaccountcreated stores the reply's "AGE"
};

CHECK_OFFSET(online_AcctTooYoungEvent, mType, 0x0);
CHECK_OFFSET(online_AcctTooYoungEvent, mVtbl, 0x4);
CHECK_OFFSET(online_AcctTooYoungEvent, mError, 0x8);
CHECK_OFFSET(online_AcctTooYoungEvent, mAge, 0xC);
CHECK_SIZE(online_AcctTooYoungEvent, 0x10);

// online::AcctLockedEvent: type 258 with code 'lock', size 0x14 ([R] clone allocates 20). Vtable
// online_AcctLockedEvent_vtbl (0x004811D0).
struct online_AcctLockedEvent {
    // --- online::Event
    int mType;                      // 0x00 [R] 258
    void** mVtbl;                   // 0x04 [R] online_AcctLockedEvent_vtbl
    // --- online::ErrorEvent (probable base, see above)
    int mError;                     // 0x08 [R] clone stores the constant 'lock' (0x6C6F636B)
    // --- online::AcctLockedEvent
    char* mDate;                    // 0x0C [R] cBXString (clone builds it, the destructor destroys it second);
                                    //      [U] LobbyMan_onauthentication: the reply's "DATE"
    char* mIntro;                   // 0x10 [R] cBXString (destroyed first); [U] the reply's "INTRO" text
};

CHECK_OFFSET(online_AcctLockedEvent, mType, 0x00);
CHECK_OFFSET(online_AcctLockedEvent, mVtbl, 0x04);
CHECK_OFFSET(online_AcctLockedEvent, mError, 0x08);
CHECK_OFFSET(online_AcctLockedEvent, mDate, 0x0C);
CHECK_OFFSET(online_AcctLockedEvent, mIntro, 0x10);
CHECK_SIZE(online_AcctLockedEvent, 0x14);

// online::FindPlayerEvent: type 227, size 0x2C ([R] LobbyMan_onPlayerFound and clone allocate 44). Vtable
// online_FindPlayerEvent_vtbl (0x00481190).
struct online_FindPlayerEvent {
    // --- online::Event
    int mType;                      // 0x00 [R] 227
    void** mVtbl;                   // 0x04 [R] online_FindPlayerEvent_vtbl
    // --- online::FindPlayerEvent
    char* mName;                    // 0x08 [R] cBXString (destroyed second); [U] the reply's "PERS" (persona name)
    char* mRoom;                    // 0x0C [R] cBXString (destroyed first); [U] the reply's "ROOM"
    int mOnline;                    // 0x10 [U] onPlayerFound: "ADDR" > 0
    int mAddr;                      // 0x14 [U] onPlayerFound: the "ADDR" number
    tLobbyStats mStats;             // 0x18 [R] clone block-copies 20 bytes; [U] "RANK" and "STAT" fields, -1 last
};

CHECK_OFFSET(online_FindPlayerEvent, mType, 0x00);
CHECK_OFFSET(online_FindPlayerEvent, mVtbl, 0x04);
CHECK_OFFSET(online_FindPlayerEvent, mName, 0x08);
CHECK_OFFSET(online_FindPlayerEvent, mRoom, 0x0C);
CHECK_OFFSET(online_FindPlayerEvent, mOnline, 0x10);
CHECK_OFFSET(online_FindPlayerEvent, mAddr, 0x14);
CHECK_OFFSET(online_FindPlayerEvent, mStats, 0x18);
CHECK_SIZE(online_FindPlayerEvent, 0x2C);

// online::PingEvent: type 229, size 0x10 ([R] clone allocates 0x10). Vtable online_PingEvent_vtbl (0x00481170).
struct online_PingEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 229
    void** mVtbl;                   // 0x4 [R] online_PingEvent_vtbl
    // --- online::PingEvent
    char* mName;                    // 0x8 [R] cBXString (clone, destructor); [U] LobbyMan_onPingReceived: "NAME"
    int mPing;                      // 0xC [R] clone copies it; [U] "PING", 32656 when the reply carried an error
};

CHECK_OFFSET(online_PingEvent, mType, 0x0);
CHECK_OFFSET(online_PingEvent, mVtbl, 0x4);
CHECK_OFFSET(online_PingEvent, mName, 0x8);
CHECK_OFFSET(online_PingEvent, mPing, 0xC);
CHECK_SIZE(online_PingEvent, 0x10);

// online::ChatEvent: type 222, size 0x10 ([R] func_0025AB40 and online_ChatEvent_clone allocate 0x10 bytes tagged
// "ChatEvent"). Vtable 0x00481230 (D_00481230: 1 online_ChatEvent__ChatEvent, 2 online_ChatEvent_clone). Retail only
// (not in the preview), so the class and member names are ours.
struct online_ChatEvent {
    // --- online::Event
    int mType;                      // 0x0 [R] 222 (func_0025AB40, clone)
    void** mVtbl;                   // 0x4 [R] D_00481230; the destructor stores online_Event_vtbl back
    // --- online::ChatEvent
    char* mSender;                  // 0x8 [R] cBXString: func_0025AB40 copies its sender argument (the chat line's
                                    //     sender, from LobbyMan_onchatmsg); clone copies it; destroyed second
    char* mText;                    // 0xC [R] cBXString: the message text (func_0025AB40's text argument); destroyed
                                    //     first
};

CHECK_OFFSET(online_ChatEvent, mType, 0x0);
CHECK_OFFSET(online_ChatEvent, mVtbl, 0x4);
CHECK_OFFSET(online_ChatEvent, mSender, 0x8);
CHECK_OFFSET(online_ChatEvent, mText, 0xC);
CHECK_SIZE(online_ChatEvent, 0x10);

#endif /* ONLINE_ONLINEEVENT_H */
