#ifndef SEG_BUDDYAPI_H
#define SEG_BUDDYAPI_H

// BuddyApiRefT: the state of DirtySock's BuddyApi (C), the client for EA's buddy / messenger server: login ('AUTH'),
// presence (PROD / STAT / RSRC / SHOW), the roster and ignore lists, add / delete / send requests and incoming
// messages. BuddyMan (online/buddyman.h) owns one as mpBuddyApi: BuddyMan_createBuddyApi creates it with BuddyApi_create (0x50C bytes
// from lobbyman_alloc, zero-filled). Retail code: unit seg/seg_2F0F40, BuddyApi_sendCommand .. BuddyApi_update (its MD5 helpers
// LobbyMd5_transform / LobbyMd5_init / LobbyMd5_update work on their own context). EA source: DirtySock buddyapi.c (retail's
// "buddyapi" label); no On Tour / preview layout.
//
// Name: DirtySock's BuddyApiRefT, from the July 2003 preview's mangled BuddyMan callbacks (on*__8BuddyManP12BuddyApiMsgT
// P12BuddyApiRefTP12BuddyApiMsgTPv) together with retail's data flow: BuddyMan_createBuddyApi stores BuddyApi_create's result in
// the member include/online/buddyman.h types as BuddyApiRefT* mpBuddyApi (+0xC8), and the BuddyApi functions all take it
// first. Member names are ours, from retail use (the DirtySock tag each text carries is quoted); the request slot is
// left as a plain array.
//
// C struct, flat. Evidence tags: [R] the create function BuddyApi_create; [U] retail use.

#include "layout.h"
#include "seg/dirtynet.h"            // HostentT (mpHostLookup)

// C units (seg/seg_2F0F40.c, the BuddyApi code) name the type without the struct keyword.
#ifndef __cplusplus
typedef struct BuddyApiRefT BuddyApiRefT;
#endif

struct BuddyApiRefT {
    HostentT* mpHostLookup;             // 0x000 [U] BuddyApi_connect: the pending host lookup (SocketLookup(server name,
                                        //       ms * 1000)); BuddyApi_update polls it and destroys it (its slot +0xC);
                                        //       BuddyApi_destroy / BuddyApi_disconnect destroy a pending one
    void* mpSession;                    // 0x004 [R] create: NetSession_create(0x8000) (the packet session); [U]
                                        //       BuddyApi_sendCommand sends on it, BuddyApi_armRequest reads the time from it
                                        //       (NetSession_getTick), BuddyApi_disconnect closes it (NetSession_disconnect), BuddyApi_destroy
                                        //       destroys it (NetSession_destroy)
    int mState;                         // 0x008 [U] connection state (0 idle, 1 connecting, 2 lookup failed, 3 timed
                                        //       out, 5 online, 6 dropped): BuddyApi_setState stores it, BuddyApi_getState
                                        //       returns it; BuddyApi_disconnect / BuddyApi_update
    int mConnectDeadline;               // 0x00C [U] BuddyApi_connect: now + ms * 1000; BuddyApi_update reports state 3 when
                                        //       it passes
    int mPort;                          // 0x010 [U] BuddyApi_connect stores the port; BuddyApi_update connects with it
    int mRequestSeq;                    // 0x014 [R] create 100; [U] BuddyApi_armRequest numbers request slots from it;
                                        //       BuddyApi_addUser
    int unk_018;                        // 0x018 [U] BuddyApi_configure stores its last argument; when 0, BuddyApi_addUser /
                                        //       BuddyApi_removeUser refuse names containing '.'; BuddyApi_update
    int mProductKey;                    // 0x01C [U] BuddyApi_configure stores the product key (BuddyMan passes 'ssx3');
                                        //       BuddyApi_sendPresence puts it in STAT as 'P'; BuddyApi_parsePresence matches received
                                        //       presence against it
    char unk_020[0x20];                 // 0x020 no access seen
    char mProdText[0x100];              // 0x040 [U] the PROD presence text: set by BuddyApi_setProductText, cleared by
                                        //       BuddyApi_clearPresenceTexts, sent by BuddyApi_sendPresence
    char mStatText[0x100];              // 0x140 [U] the STAT presence text: set by BuddyApi_setStatusText, cleared by
                                        //       BuddyApi_clearPresenceTexts, sent by BuddyApi_sendPresence
    char mPresence[0x80];               // 0x240 [U] the base presence (RSRC and SHOW) built by BuddyApi_setPresence;
                                        //       BuddyApi_sendPresence sends it
    int mPresenceSendTime;              // 0x2C0 [U] BuddyApi_setPresence schedules a presence send, BuddyApi_sendPresence clears it,
                                        //       BuddyApi_update sends when due
    int mLastShow;                      // 0x2C4 [U] BuddyApi_connect -1; BuddyApi_setPresence records the last show state
    char mPresText[0x40];               // 0x2C8 [U] PRES text: BuddyApi_configure copies it, BuddyApi_connect puts it in the
                                        //       'AUTH' login, BuddyApi_setProductText tests it
    int mNumLangs;                      // 0x308 [U] BuddyApi_configure: number of language codes; BuddyApi_parsePresence
    short mLangs[4];                    // 0x30C [U] BuddyApi_configure: up to four two-letter language codes (16-bit);
                                        //       BuddyApi_parsePresence (lh / lhu)
    char mProduct[0x40];                // 0x314 [R] create: its product argument (PROD in the 'AUTH' login)
    char mVersion[0x40];                // 0x354 [R] create: its version argument (VERS in the 'AUTH' login; BuddyMan
                                        //       passes "2003-10-20")
    char mAuthText[0x100];              // 0x394 [U] BuddyApi_connect builds the 'AUTH' login tags here; BuddyApi_update
                                        //       sends them
    char mResource[0x10];               // 0x494 [U] BuddyApi_connect: the login resource (sent as LRSC); BuddyApi_requestRoster /
                                        //       BuddyApi_addUser / BuddyApi_removeUser
    void* mpNameTable;                  // 0x4A4 [U] BuddyApi_requestRoster creates it (HashTable_create(1000, 2000)); BuddyApi_freeRoster
                                        //       frees it; BuddyApi_addUser / BuddyApi_removeUser / BuddyApi_findRosterEntry / BuddyApi_update
                                        //       look names up in it
    void* mpRoster;                     // 0x4A8 [U] BuddyApi_requestRoster creates the roster list (SortList_create(100, 100, 0));
                                        //       BuddyApi_getRoster returns it; BuddyApi_freeRoster frees it; BuddyApi_addUser
    void* mpRecvData;                   // 0x4AC [U] BuddyApi_setReceiveCallback stores it; BuddyApi_deliverChatMessage passes it to mpRecvProc
    void (*mpRecvProc)();               // 0x4B0 [U] BuddyApi_setReceiveCallback stores it; BuddyApi_deliverChatMessage calls it for received
                                        //       messages; BuddyApi_update
    void* mpRosterData;                 // 0x4B4 [U] BuddyApi_requestRoster stores it; BuddyApi_update
    void (*mpRosterProc)();             // 0x4B8 [U] BuddyApi_requestRoster stores it; BuddyApi_freeRoster / BuddyApi_update call it
    int unk_4BC;                        // 0x4BC [U] BuddyApi_update (lw / sw)
    void* mpConnectData;                // 0x4C0 [U] BuddyApi_connect stores it; BuddyApi_setState passes it to mpConnectProc
    void (*mpConnectProc)();            // 0x4C4 [U] BuddyApi_connect stores it; BuddyApi_setState calls it with a {'conn',
                                        //       state} message; BuddyApi_disconnect clears it
    int mRequests[3][5];                // 0x4C8 [U] three request slots of 0x14 bytes {kind, sequence, deadline, user
                                        //       data, callback} (0 'addu', 1 'delu', 2 'send') armed by BuddyApi_armRequest;
                                        //       BuddyApi_addUser reads slot 0's sequence (+0x4CC), BuddyApi_removeUser slot 1's
                                        //       (+0x4E0); BuddyApi_update
    void* mpDebugData;                  // 0x504 [U] BuddyApi_setDebugHook stores it; passed to mpDebugProc
    void (*mpDebugProc)(void*, const char*);// 0x508 [U] BuddyApi_setDebugHook stores it; BuddyApi_sendCommand / BuddyApi_update trace
                                        //       through it
};

CHECK_OFFSET(BuddyApiRefT, mpHostLookup, 0x000);
CHECK_OFFSET(BuddyApiRefT, mpSession, 0x004);
CHECK_OFFSET(BuddyApiRefT, mState, 0x008);
CHECK_OFFSET(BuddyApiRefT, mConnectDeadline, 0x00C);
CHECK_OFFSET(BuddyApiRefT, mPort, 0x010);
CHECK_OFFSET(BuddyApiRefT, mRequestSeq, 0x014);
CHECK_OFFSET(BuddyApiRefT, unk_018, 0x018);
CHECK_OFFSET(BuddyApiRefT, mProductKey, 0x01C);
CHECK_OFFSET(BuddyApiRefT, unk_020, 0x020);
CHECK_OFFSET(BuddyApiRefT, mProdText, 0x040);
CHECK_OFFSET(BuddyApiRefT, mStatText, 0x140);
CHECK_OFFSET(BuddyApiRefT, mPresence, 0x240);
CHECK_OFFSET(BuddyApiRefT, mPresenceSendTime, 0x2C0);
CHECK_OFFSET(BuddyApiRefT, mLastShow, 0x2C4);
CHECK_OFFSET(BuddyApiRefT, mPresText, 0x2C8);
CHECK_OFFSET(BuddyApiRefT, mNumLangs, 0x308);
CHECK_OFFSET(BuddyApiRefT, mLangs, 0x30C);
CHECK_OFFSET(BuddyApiRefT, mProduct, 0x314);
CHECK_OFFSET(BuddyApiRefT, mVersion, 0x354);
CHECK_OFFSET(BuddyApiRefT, mAuthText, 0x394);
CHECK_OFFSET(BuddyApiRefT, mResource, 0x494);
CHECK_OFFSET(BuddyApiRefT, mpNameTable, 0x4A4);
CHECK_OFFSET(BuddyApiRefT, mpRoster, 0x4A8);
CHECK_OFFSET(BuddyApiRefT, mpRecvData, 0x4AC);
CHECK_OFFSET(BuddyApiRefT, mpRecvProc, 0x4B0);
CHECK_OFFSET(BuddyApiRefT, mpRosterData, 0x4B4);
CHECK_OFFSET(BuddyApiRefT, mpRosterProc, 0x4B8);
CHECK_OFFSET(BuddyApiRefT, unk_4BC, 0x4BC);
CHECK_OFFSET(BuddyApiRefT, mpConnectData, 0x4C0);
CHECK_OFFSET(BuddyApiRefT, mpConnectProc, 0x4C4);
CHECK_OFFSET(BuddyApiRefT, mRequests, 0x4C8);
CHECK_OFFSET(BuddyApiRefT, mpDebugData, 0x504);
CHECK_OFFSET(BuddyApiRefT, mpDebugProc, 0x508);
CHECK_SIZE(BuddyApiRefT, 0x50C);

// BuddyApiMsgT (0x10): the message BuddyApi hands to the routines BuddyMan installs (mpRecvProc, mpConnectProc): the
// routine is called as (BuddyApiRefT*, BuddyApiMsgT*, data). Name: DirtySock's, from the July 2003 preview's mangled
// BuddyMan callbacks (on*__8BuddyManP12BuddyApiMsgT, e.g. onReceiveMessage, and the functor's invoke(BuddyApiRefT*,
// BuddyApiMsgT*, void*)); [R] BuddyApi builds it as four words (BuddyApi_setState: {'conn', state, 0, 0}; BuddyApi_deliverChatMessage
// sets the kind to 'chat' and the tag text at 0xC); [U] BuddyMan_onReceiveMessage reads the tag text at 0xC. EA's
// member names are not known: mKind and mpTagText are ours, the rest stay unk_.
#ifndef __cplusplus
typedef struct BuddyApiMsgT BuddyApiMsgT;
#endif

struct BuddyApiMsgT {
    int mKind;                          // 0x0 [R] the message kind, a FourCC ('conn', 'chat')
    int unk_04;                         // 0x4 [R] BuddyApi_setState: the connection state
    int unk_08;                         // 0x8 [R] BuddyApi_setState: 0
    char* mpTagText;                    // 0xC [R] BuddyApi_deliverChatMessage: the rewritten tag text; [U] BuddyMan_onReceiveMessage
};

CHECK_OFFSET(BuddyApiMsgT, mKind, 0x0);
CHECK_OFFSET(BuddyApiMsgT, unk_04, 0x4);
CHECK_OFFSET(BuddyApiMsgT, unk_08, 0x8);
CHECK_OFFSET(BuddyApiMsgT, mpTagText, 0xC);
CHECK_SIZE(BuddyApiMsgT, 0x10);

#endif /* SEG_BUDDYAPI_H */
