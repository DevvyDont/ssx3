#ifndef SEG_BUDDYAPI_H
#define SEG_BUDDYAPI_H

// BuddyApiRefT: the state of DirtySock's BuddyApi (C), the client for EA's buddy / messenger server: login ('AUTH'),
// presence (PROD / STAT / RSRC / SHOW), the roster and ignore lists, add / delete / send requests and incoming
// messages. BuddyMan (online/buddyman.h) owns one as mpBuddyApi: BuddyMan_createBuddyApi creates it with func_003F0710 (0x50C bytes
// from func_002591E8, zero-filled). Retail code: unit seg/seg_2F0F40, func_003F00D8 .. func_003F17D8 (its MD5 helpers
// func_003F2220 / func_003F30F0 / func_003F3128 work on their own context). EA source: DirtySock buddyapi.c (retail's
// "buddyapi" label); no On Tour / preview layout.
//
// Name: DirtySock's BuddyApiRefT, from the July 2003 preview's mangled BuddyMan callbacks (on*__8BuddyManP12BuddyApiMsgT
// P12BuddyApiRefTP12BuddyApiMsgTPv) together with retail's data flow: BuddyMan_createBuddyApi stores func_003F0710's result in
// the member include/online/buddyman.h types as BuddyApiRefT* mpBuddyApi (+0xC8), and the BuddyApi functions all take it
// first. Member names are ours, from retail use (the DirtySock tag each text carries is quoted); the request slot is
// left as a plain array.
//
// C struct, flat. Evidence tags: [R] the create function func_003F0710; [U] retail use.

#include "layout.h"
#include "seg/dirtynet.h"            // HostentT (mpHostLookup)

// C units (seg/seg_2F0F40.c, the BuddyApi code) name the type without the struct keyword.
#ifndef __cplusplus
typedef struct BuddyApiRefT BuddyApiRefT;
#endif

struct BuddyApiRefT {
    HostentT* mpHostLookup;             // 0x000 [U] func_003F08E8: the pending host lookup (SocketLookup(server name,
                                        //       ms * 1000)); func_003F17D8 polls it and destroys it (its slot +0xC);
                                        //       func_003F07A0 / func_003F0B50 destroy a pending one
    void* mpSession;                    // 0x004 [R] create: func_003F5478(0x8000) (the packet session); [U]
                                        //       func_003F00D8 sends on it, func_003F0658 reads the time from it
                                        //       (func_003F5D88), func_003F0B50 closes it (func_003F5738), func_003F07A0
                                        //       destroys it (func_003F55B8)
    int mState;                         // 0x008 [U] connection state (0 idle, 1 connecting, 2 lookup failed, 3 timed
                                        //       out, 5 online, 6 dropped): func_003F0610 stores it, func_003F0800
                                        //       returns it; func_003F0B50 / func_003F17D8
    int mConnectDeadline;               // 0x00C [U] func_003F08E8: now + ms * 1000; func_003F17D8 reports state 3 when
                                        //       it passes
    int mPort;                          // 0x010 [U] func_003F08E8 stores the port; func_003F17D8 connects with it
    int mRequestSeq;                    // 0x014 [R] create 100; [U] func_003F0658 numbers request slots from it;
                                        //       func_003F0E38
    int unk_018;                        // 0x018 [U] func_003F0818 stores its last argument; when 0, func_003F0E38 /
                                        //       func_003F11F8 refuse names containing '.'; func_003F17D8
    int mProductKey;                    // 0x01C [U] func_003F0818 stores the product key (BuddyMan passes 'ssx3');
                                        //       func_003F0568 puts it in STAT as 'P'; func_003F02D0 matches received
                                        //       presence against it
    char unk_020[0x20];                 // 0x020 no access seen
    char mProdText[0x100];              // 0x040 [U] the PROD presence text: set by func_003F1548, cleared by
                                        //       func_003F1538, sent by func_003F0568
    char mStatText[0x100];              // 0x140 [U] the STAT presence text: set by func_003F1578, cleared by
                                        //       func_003F1538, sent by func_003F0568
    char mPresence[0x80];               // 0x240 [U] the base presence (RSRC and SHOW) built by func_003F15C8;
                                        //       func_003F0568 sends it
    int mPresenceSendTime;              // 0x2C0 [U] func_003F15C8 schedules a presence send, func_003F0568 clears it,
                                        //       func_003F17D8 sends when due
    int mLastShow;                      // 0x2C4 [U] func_003F08E8 -1; func_003F15C8 records the last show state
    char mPresText[0x40];               // 0x2C8 [U] PRES text: func_003F0818 copies it, func_003F08E8 puts it in the
                                        //       'AUTH' login, func_003F1548 tests it
    int mNumLangs;                      // 0x308 [U] func_003F0818: number of language codes; func_003F02D0
    short mLangs[4];                    // 0x30C [U] func_003F0818: up to four two-letter language codes (16-bit);
                                        //       func_003F02D0 (lh / lhu)
    char mProduct[0x40];                // 0x314 [R] create: its product argument (PROD in the 'AUTH' login)
    char mVersion[0x40];                // 0x354 [R] create: its version argument (VERS in the 'AUTH' login; BuddyMan
                                        //       passes "2003-10-20")
    char mAuthText[0x100];              // 0x394 [U] func_003F08E8 builds the 'AUTH' login tags here; func_003F17D8
                                        //       sends them
    char mResource[0x10];               // 0x494 [U] func_003F08E8: the login resource (sent as LRSC); func_003F0C90 /
                                        //       func_003F0E38 / func_003F11F8
    void* mpNameTable;                  // 0x4A4 [U] func_003F0C90 creates it (func_003EE778(1000, 2000)); func_003F0BE0
                                        //       frees it; func_003F0E38 / func_003F11F8 / func_003F14D8 / func_003F17D8
                                        //       look names up in it
    void* mpRoster;                     // 0x4A8 [U] func_003F0C90 creates the roster list (func_003EE1E8(100, 100, 0));
                                        //       func_003F0E30 returns it; func_003F0BE0 frees it; func_003F0E38
    void* mpRecvData;                   // 0x4AC [U] func_003F17C8 stores it; func_003F0198 passes it to mpRecvProc
    void (*mpRecvProc)();               // 0x4B0 [U] func_003F17C8 stores it; func_003F0198 calls it for received
                                        //       messages; func_003F17D8
    void* mpRosterData;                 // 0x4B4 [U] func_003F0C90 stores it; func_003F17D8
    void (*mpRosterProc)();             // 0x4B8 [U] func_003F0C90 stores it; func_003F0BE0 / func_003F17D8 call it
    int unk_4BC;                        // 0x4BC [U] func_003F17D8 (lw / sw)
    void* mpConnectData;                // 0x4C0 [U] func_003F08E8 stores it; func_003F0610 passes it to mpConnectProc
    void (*mpConnectProc)();            // 0x4C4 [U] func_003F08E8 stores it; func_003F0610 calls it with a {'conn',
                                        //       state} message; func_003F0B50 clears it
    int mRequests[3][5];                // 0x4C8 [U] three request slots of 0x14 bytes {kind, sequence, deadline, user
                                        //       data, callback} (0 'addu', 1 'delu', 2 'send') armed by func_003F0658;
                                        //       func_003F0E38 reads slot 0's sequence (+0x4CC), func_003F11F8 slot 1's
                                        //       (+0x4E0); func_003F17D8
    void* mpDebugData;                  // 0x504 [U] func_003F0808 stores it; passed to mpDebugProc
    void (*mpDebugProc)(void*, const char*);// 0x508 [U] func_003F0808 stores it; func_003F00D8 / func_003F17D8 trace
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
// BuddyApiMsgT*, void*)); [R] BuddyApi builds it as four words (func_003F0610: {'conn', state, 0, 0}; func_003F0198
// sets the kind to 'chat' and the tag text at 0xC); [U] BuddyMan_onReceiveMessage reads the tag text at 0xC. EA's
// member names are not known: mKind and mpTagText are ours, the rest stay unk_.
#ifndef __cplusplus
typedef struct BuddyApiMsgT BuddyApiMsgT;
#endif

struct BuddyApiMsgT {
    int mKind;                          // 0x0 [R] the message kind, a FourCC ('conn', 'chat')
    int unk_04;                         // 0x4 [R] func_003F0610: the connection state
    int unk_08;                         // 0x8 [R] func_003F0610: 0
    char* mpTagText;                    // 0xC [R] func_003F0198: the rewritten tag text; [U] BuddyMan_onReceiveMessage
};

CHECK_OFFSET(BuddyApiMsgT, mKind, 0x0);
CHECK_OFFSET(BuddyApiMsgT, unk_04, 0x4);
CHECK_OFFSET(BuddyApiMsgT, unk_08, 0x8);
CHECK_OFFSET(BuddyApiMsgT, mpTagText, 0xC);
CHECK_SIZE(BuddyApiMsgT, 0x10);

#endif /* SEG_BUDDYAPI_H */
