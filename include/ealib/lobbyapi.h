#ifndef EALIB_LOBBYAPI_H
#define EALIB_LOBBYAPI_H

// LobbyApiRefT: the state of DirtySock's LobbyApi (C), the client for EA's online lobby server: the login / account /
// persona commands, the room / user / rank lists, the request queue, server pings and the event routines LobbyMan
// installs. LobbyMan (online/lobbyman.h) owns one as mpLobbyApi: func_0025A948 creates it with func_003E7FF0 (0x550
// bytes from func_002591E8, zero-filled) and registers its handlers with func_003E8248. Retail code: unit
// ealib/seg_2DCAE8, func_003E7038 .. func_003EAEB8 (the LobbyApi part of the unit). EA source: DirtySock lobbyapi.c
// (retail's "lobbyapi" label); no On Tour / preview layout.
//
// Name: DirtySock's LobbyApiRefT, from the July 2003 preview's mangled LobbyMan callbacks (on*__8LobbyManP12LobbyApiMsgT
// P12LobbyApiRefTP12LobbyApiMsgTPv: the callbacks receive the LobbyApiRefT*) together with retail's data flow:
// func_0025A948 stores func_003E7FF0's result at LobbyMan +0x54, the member include/online/lobbyman.h types as
// LobbyApiRefT* mpLobbyApi, and the LobbyApi functions all take it first. Member names are ours, from retail use
// (the DirtySock tag / FourCC each field carries is quoted); the 68-byte request record, the list slot and the event
// routine pair are left as plain arrays.
//
// C struct, flat. Evidence tags: [R] the create function func_003E7FF0; [U] retail use.

#include "layout.h"

// C units (ealib/seg_2DCAE8.c) name the type without the struct keyword (the CHECK lines below need it too).
#ifndef __cplusplus
typedef struct LobbyApiRefT LobbyApiRefT;
#endif

struct LobbyApiRefT {
    void* mpSession;                    // 0x000 [R] create: func_003F5478(0x8000) (the packet session, as in
                                        //       BuddyApiRefT); [U] func_003E80C0 and func_003E8260 disconnect it
                                        //       (func_003F5738), func_003E8260 connects it (func_003F5638),
                                        //       func_003E8B10 / func_003E8DC0 send on it (func_003F5910); func_003E7C40
                                        //       reads it
    void* unk_004;                      // 0x004 [R] create: func_003E7340(); [U] func_003E7C40 / func_003E8DC0 read it
    int mEventFlags;                    // 0x008 [U] func_003E7F70 ORs its flags in; func_003E8FB8 reads and updates
                                        //       them; func_003E95C8
    int mState;                         // 0x00C [U] FourCC state: 'offl' (func_003E80C0), 'rdir' (func_003E8260),
                                        //       'term' (func_003E7F70); func_003E8B10 / func_003E8DC0 test it; 'stat'
                                        //       query (func_003E8F08)
    int mTimeout;                       // 0x010 [U] func_003E8260: now + 120000; func_003E95C8 tests and updates it
    int mStateTime;                     // 0x014 [U] func_003E8260: now; func_003E95C8
    int mNextPingTime;                  // 0x018 [U] func_003E7F58 0, func_003E94F0 schedules it; func_003E95C8
    int mLastPing;                      // 0x01C [U] func_003E7F58 -1 (none), func_003E94F0 stores a ping result (sent
                                        //       in '~png' replies)
    int mPingFailures;                  // 0x020 [U] func_003E7F58 0, func_003E94F0 counts server ping timeouts
    char mSessionKey[0x10];             // 0x024 [U] func_003E8B10: empty = no session key (lb of the first byte)
    char mTags[0x100];                  // 0x034 [R] create: the default tags (TagFieldDupl / TagFieldSetString of its
                                        //       tag string); [U] func_003E8260 sends them with '@dir', func_003E8B10
                                        //       merges them into commands
    char mAccount[0x40];                // 0x134 [U] the account name from the login reply ('user' / 'acct' query,
                                        //       func_003E8A70)
    char mPersona[0x40];                // 0x174 [U] the persona ('pers' query, func_003E8A70)
    int unk_1B4;                        // 0x1B4 [U] func_003E95C8 (sw)
    int unk_1B8;                        // 0x1B8 [U] func_003E95C8 (lw / sw)
    int mRoomFlags;                     // 0x1BC [U] the room flags ('rflg' query, func_003E8F08); func_003E95C8 stores
                                        //       them
    char mRoomName[0x40];               // 0x1C0 [U] the room name ('room' query, func_003E8A70)
    char mGameSession[0xC0];            // 0x200 [U] the game-session record ('play' query, func_003E8A70) - continues
                                        //       into 0x2C0..0x2CC
    int unk_2C0;                        // 0x2C0 [U] func_003E95C8 (sw)
    int unk_2C4;                        // 0x2C4 [U] func_003E95C8 (sw)
    int unk_2C8;                        // 0x2C8 [U] func_003E95C8 (sw)
    int mGameWhen;                      // 0x2CC [U] the game session's WHEN time: func_003E8A70 returns the record only
                                        //       when set; func_003E8AF8 ('play') clears it; func_003E95C8 stores it
    char unk_2D0[0x40];                 // 0x2D0 no access seen
    int mLists[8][5];                   // 0x310 [U] 8 list slots of 0x14 bytes {index, list, routine, context, ?} (0
                                        //       rooms, 1 users, 2 ranks): func_003E8688 / func_003E8650 / func_003E80C0
                                        //       / func_003E7B00; func_003E7C40 uses slots 0-1 (+0x310 / +0x314 / +0x324
                                        //       / +0x328), func_003E9160 slot 2 (+0x338 / +0x33C)
    int mPort;                          // 0x3B0 [U] func_003E8260 stores the port; func_003E95C8 / func_003E9590
    int mAddress;                       // 0x3B4 [U] func_003E8260 stores the server address; func_003E9590 pings it
    char mDownText[0x100];              // 0x3B8 [U] the server-down message: func_003E8260 clears it, func_003E8A70
                                        //       ('down') returns it
    int mSess;                          // 0x4B8 [U] the SESS number of the '@dir' reply (func_003E95C8); func_003E8B10
                                        //       sends it as MASK
    char mMask[0x40];                   // 0x4BC [U] the MASK text of the '@dir' reply (func_003E8B10 encrypts PASS with
                                        //       it)
    int mExternalAddr;                  // 0x4FC [U] 'extn' query (func_003E8F08); func_003E95C8
    int mLocalAddr;                     // 0x500 [U] 'locl' query (func_003E8F08); func_003E95C8
    int mMore;                          // 0x504 [U] 'more' query (func_003E8F08): the 'sele' reply's MORE
    int unk_508;                        // 0x508 [U] func_003E8B10 clears it; func_003E95C8
    int mPendingSnapshots;              // 0x50C [U] func_003E8260 0, func_003E8B10 counts; func_003E95C8
    int mSlots;                         // 0x510 [U] 'slot' query (func_003E8F08): the 'sele' reply's SLOTS
    int mEventProcs[5][2];              // 0x514 [U] five {routine, context} pairs set by func_003E8248 (LobbyMan's
                                        //       handlers); func_003E7F70 calls [3] (+0x52C / +0x530) with a 'disc'
                                        //       event; func_003E95C8
    char* mpRequests;                   // 0x53C [U] the pending request list (68-byte records, next at +0x40):
                                        //       func_003E7718 links, func_003E77F0 pops, func_003E80C0 frees
    int mPingsOutstanding;              // 0x540 [U] func_003E80C0 0; func_003E8DC0 counts; func_003E7918 decrements
    void* mpDebugData;                  // 0x544 [R] create's 2nd argument; [U] func_003E76A8 passes it to mpDebugProc
    void (*mpDebugProc)(void*, const char*);// 0x548 [R] create's 3rd argument; [U] func_003E76A8 calls it with the
                                        //       formatted text
    int mRequestId;                     // 0x54C [U] func_003E7718 numbers request records from it; func_003E80C0 0
};

CHECK_OFFSET(LobbyApiRefT, mpSession, 0x000);
CHECK_OFFSET(LobbyApiRefT, unk_004, 0x004);
CHECK_OFFSET(LobbyApiRefT, mEventFlags, 0x008);
CHECK_OFFSET(LobbyApiRefT, mState, 0x00C);
CHECK_OFFSET(LobbyApiRefT, mTimeout, 0x010);
CHECK_OFFSET(LobbyApiRefT, mStateTime, 0x014);
CHECK_OFFSET(LobbyApiRefT, mNextPingTime, 0x018);
CHECK_OFFSET(LobbyApiRefT, mLastPing, 0x01C);
CHECK_OFFSET(LobbyApiRefT, mPingFailures, 0x020);
CHECK_OFFSET(LobbyApiRefT, mSessionKey, 0x024);
CHECK_OFFSET(LobbyApiRefT, mTags, 0x034);
CHECK_OFFSET(LobbyApiRefT, mAccount, 0x134);
CHECK_OFFSET(LobbyApiRefT, mPersona, 0x174);
CHECK_OFFSET(LobbyApiRefT, unk_1B4, 0x1B4);
CHECK_OFFSET(LobbyApiRefT, unk_1B8, 0x1B8);
CHECK_OFFSET(LobbyApiRefT, mRoomFlags, 0x1BC);
CHECK_OFFSET(LobbyApiRefT, mRoomName, 0x1C0);
CHECK_OFFSET(LobbyApiRefT, mGameSession, 0x200);
CHECK_OFFSET(LobbyApiRefT, unk_2C0, 0x2C0);
CHECK_OFFSET(LobbyApiRefT, unk_2C4, 0x2C4);
CHECK_OFFSET(LobbyApiRefT, unk_2C8, 0x2C8);
CHECK_OFFSET(LobbyApiRefT, mGameWhen, 0x2CC);
CHECK_OFFSET(LobbyApiRefT, unk_2D0, 0x2D0);
CHECK_OFFSET(LobbyApiRefT, mLists, 0x310);
CHECK_OFFSET(LobbyApiRefT, mPort, 0x3B0);
CHECK_OFFSET(LobbyApiRefT, mAddress, 0x3B4);
CHECK_OFFSET(LobbyApiRefT, mDownText, 0x3B8);
CHECK_OFFSET(LobbyApiRefT, mSess, 0x4B8);
CHECK_OFFSET(LobbyApiRefT, mMask, 0x4BC);
CHECK_OFFSET(LobbyApiRefT, mExternalAddr, 0x4FC);
CHECK_OFFSET(LobbyApiRefT, mLocalAddr, 0x500);
CHECK_OFFSET(LobbyApiRefT, mMore, 0x504);
CHECK_OFFSET(LobbyApiRefT, unk_508, 0x508);
CHECK_OFFSET(LobbyApiRefT, mPendingSnapshots, 0x50C);
CHECK_OFFSET(LobbyApiRefT, mSlots, 0x510);
CHECK_OFFSET(LobbyApiRefT, mEventProcs, 0x514);
CHECK_OFFSET(LobbyApiRefT, mpRequests, 0x53C);
CHECK_OFFSET(LobbyApiRefT, mPingsOutstanding, 0x540);
CHECK_OFFSET(LobbyApiRefT, mpDebugData, 0x544);
CHECK_OFFSET(LobbyApiRefT, mpDebugProc, 0x548);
CHECK_OFFSET(LobbyApiRefT, mRequestId, 0x54C);
CHECK_SIZE(LobbyApiRefT, 0x550);

// LobbyApiMsgT (0x10): the event LobbyApi hands to the routines LobbyMan installs (func_003E8248): the routine is
// called as (LobbyApiRefT*, LobbyApiMsgT*, context). Name: DirtySock's, from the July 2003 preview's mangled LobbyMan
// callbacks (on*__8LobbyManP12LobbyApiMsgT, e.g. onauthentication / onchatmsg / onplaymsg / onPingReceived, and
// LobbyFunctor's invoke(LobbyApiRefT*, LobbyApiMsgT*, void*)); [R] LobbyApi builds it as four words on the stack
// (func_003E7F70: {3, 'disc', reason, 0}); [U] every LobbyMan handler reads the reply's tag text at 0xC (TagFieldFind),
// onchatmsg switches on 0x4, most handlers test the status at 0x8. Member names are not known (no DirtySock source or
// On Tour LobbyApi here).
#ifndef __cplusplus
typedef struct LobbyApiMsgT LobbyApiMsgT;
#endif

struct LobbyApiMsgT {
    int unk_00;                         // 0x0 [R] func_003E7F70: 3 (the event type)
    int mKind;                          // 0x4 [R] the event kind, a FourCC ('disc'); [U] onchatmsg switches on it
                                        //     (name ours)
    int mStatus;                        // 0x8 [R] func_003E7F70: the reason code; [U] the handlers' status test (0 =
                                        //     success) (name ours)
    char* mpTagText;                    // 0xC [U] the reply's tag text (cDirtysock_tag_TagFieldFind's source); 0 in
                                        //     func_003E7F70's event (name ours)
};

CHECK_OFFSET(LobbyApiMsgT, unk_00, 0x0);
CHECK_OFFSET(LobbyApiMsgT, mKind, 0x4);
CHECK_OFFSET(LobbyApiMsgT, mStatus, 0x8);
CHECK_OFFSET(LobbyApiMsgT, mpTagText, 0xC);
CHECK_SIZE(LobbyApiMsgT, 0x10);

#endif /* EALIB_LOBBYAPI_H */
