#ifndef EALIB_LOBBYAPI_H
#define EALIB_LOBBYAPI_H

// LobbyApiRefT: the state of DirtySock's LobbyApi (C), the client for EA's online lobby server: the login / account /
// persona commands, the room / user / rank lists, the request queue, server pings and the event routines LobbyMan
// installs. LobbyMan (online/lobbyman.h) owns one as mpLobbyApi: LobbyMan_createLobbyApi creates it with LobbyApi_create (0x550
// bytes from lobbyman_alloc, zero-filled) and registers its handlers with LobbyApi_setEventRoutine. Retail code: unit
// ealib/seg_2DCAE8, NetGameUtil_queryStatus .. LobbyApi_secondsToCalendar (the LobbyApi part of the unit). EA source: DirtySock lobbyapi.c
// (retail's "lobbyapi" label); no On Tour / preview layout.
//
// Name: DirtySock's LobbyApiRefT, from the July 2003 preview's mangled LobbyMan callbacks (on*__8LobbyManP12LobbyApiMsgT
// P12LobbyApiRefTP12LobbyApiMsgTPv: the callbacks receive the LobbyApiRefT*) together with retail's data flow:
// LobbyMan_createLobbyApi stores LobbyApi_create's result at LobbyMan +0x54, the member include/online/lobbyman.h types as
// LobbyApiRefT* mpLobbyApi, and the LobbyApi functions all take it first. Member names are ours, from retail use
// (the DirtySock tag / FourCC each field carries is quoted); the 68-byte request record, the list slot and the event
// routine pair are left as plain arrays.
//
// C struct, flat. Evidence tags: [R] the create function LobbyApi_create; [U] retail use.

#include "layout.h"

// C units (ealib/seg_2DCAE8.c) name the type without the struct keyword (the CHECK lines below need it too).
#ifndef __cplusplus
typedef struct LobbyApiRefT LobbyApiRefT;
#endif

struct LobbyApiRefT {
    void* mpSession;                    // 0x000 [R] create: NetSession_create(0x8000) (the packet session, as in
                                        //       BuddyApiRefT); [U] LobbyApi_disconnect and LobbyApi_connect disconnect it
                                        //       (NetSession_disconnect), LobbyApi_connect connects it (NetSession_connect),
                                        //       LobbyApi_sendCommand / LobbyApi_ping send on it (NetSession_send); LobbyApi_handlePings
                                        //       reads it
    void* mpPing;                       // 0x004 (name ours) [R] create: PingRpc_create() (the ping block,
                                        //       PingRpc_pollResponse's view); [U] LobbyApi_handlePings / LobbyApi_ping read it
    int mEventFlags;                    // 0x008 [U] LobbyApi_dropConnection ORs its flags in; LobbyApi_takeEvent reads and updates
                                        //       them; LobbyApi_update
    int mState;                         // 0x00C [U] FourCC state: 'offl' (LobbyApi_disconnect), 'rdir' (LobbyApi_connect),
                                        //       'term' (LobbyApi_dropConnection); LobbyApi_sendCommand / LobbyApi_ping test it; 'stat'
                                        //       query (LobbyApi_getStatus)
    int mTimeout;                       // 0x010 [U] LobbyApi_connect: now + 120000; LobbyApi_update tests and updates it
    int mStateTime;                     // 0x014 [U] LobbyApi_connect: now; LobbyApi_update
    int mNextPingTime;                  // 0x018 [U] LobbyApi_resetServerPing 0, LobbyApi_onServerPing schedules it; LobbyApi_update
    int mLastPing;                      // 0x01C [U] LobbyApi_resetServerPing -1 (none), LobbyApi_onServerPing stores a ping result (sent
                                        //       in '~png' replies)
    int mPingFailures;                  // 0x020 [U] LobbyApi_resetServerPing 0, LobbyApi_onServerPing counts server ping timeouts
    char mSessionKey[0x10];             // 0x024 [U] LobbyApi_sendCommand: empty = no session key (lb of the first byte)
    char mTags[0x100];                  // 0x034 [R] create: the default tags (TagFieldDupl / TagFieldSetString of its
                                        //       tag string); [U] LobbyApi_connect sends them with '@dir', LobbyApi_sendCommand
                                        //       merges them into commands
    char mAccount[0x40];                // 0x134 [U] the account name from the login reply ('user' / 'acct' query,
                                        //       LobbyApi_getText)
    char mPersona[0x40];                // 0x174 [U] the persona ('pers' query, LobbyApi_getText)
    int mUserFlags;                     // 0x1B4 (name ours) [U] LobbyApi_update: the '+who' reply's F flags, or the flags
                                        //       of the own '+usr' record (bit 0x200000)
    int mRoomId;                        // 0x1B8 (name ours) [U] LobbyApi_update: the room id ('+who' RI, 'move' IDENT);
                                        //       a change calls LobbyApi_setRoomPopulation
    int mRoomFlags;                     // 0x1BC [U] the room flags ('rflg' query, LobbyApi_getStatus); LobbyApi_update stores
                                        //       them
    char mRoomName[0x40];               // 0x1C0 [U] the room name ('room' query, LobbyApi_getText)
    char mGameSession[0xC0];            // 0x200 [U] the game-session record ('play' query, LobbyApi_getText) - continues
                                        //       into 0x2C0..0x310 (LobbyApi_update's Sess_95C8)
    int mGameAddr;                      // 0x2C0 (name ours) [U] LobbyApi_update: the '+ses' reply's ADDR
    int mGameFrom;                      // 0x2C4 (name ours) [U] LobbyApi_update: its FROM address
    int mGameSeed;                      // 0x2C8 (name ours) [U] LobbyApi_update: its SEED
    int mGameWhen;                      // 0x2CC [U] the game session's WHEN time: LobbyApi_getText returns the record only
                                        //       when set; LobbyApi_clearField ('play') clears it; LobbyApi_update stores it
    char mGameAuth[0x40];               // 0x2D0 (name ours) [U] LobbyApi_update: its AUTH text
    int mLists[8][5];                   // 0x310 [U] 8 list slots of 0x14 bytes {index, list, routine, context, ?} (0
                                        //       rooms, 1 users, 2 ranks): LobbyApi_closeList / LobbyApi_setListRoutine / LobbyApi_disconnect
                                        //       / LobbyApi_setRoomPopulation; LobbyApi_handlePings uses slots 0-1 (+0x310 / +0x314 / +0x324
                                        //       / +0x328), LobbyApi_rankList slot 2 (+0x338 / +0x33C)
    int mPort;                          // 0x3B0 [U] LobbyApi_connect stores the port; LobbyApi_update / LobbyApi_pingServer
    int mAddress;                       // 0x3B4 [U] LobbyApi_connect stores the server address; LobbyApi_pingServer pings it
    char mDownText[0x100];              // 0x3B8 [U] the server-down message: LobbyApi_connect clears it, LobbyApi_getText
                                        //       ('down') returns it
    int mSess;                          // 0x4B8 [U] the SESS number of the '@dir' reply (LobbyApi_update); LobbyApi_sendCommand
                                        //       sends it as MASK
    char mMask[0x40];                   // 0x4BC [U] the MASK text of the '@dir' reply (LobbyApi_sendCommand encrypts PASS with
                                        //       it)
    int mExternalAddr;                  // 0x4FC [U] 'extn' query (LobbyApi_getStatus); LobbyApi_update
    int mLocalAddr;                     // 0x500 [U] 'locl' query (LobbyApi_getStatus); LobbyApi_update
    int mMore;                          // 0x504 [U] 'more' query (LobbyApi_getStatus): the 'sele' reply's MORE
    int mChannel;                       // 0x508 (name ours) [U] LobbyApi_sendCommand clears it; LobbyApi_update stores the
                                        //       'snap' reply's CHAN, the list slot (4..7) its '+snp' ranks go to
    int mPendingSnapshots;              // 0x50C [U] LobbyApi_connect 0, LobbyApi_sendCommand counts; LobbyApi_update
    int mSlots;                         // 0x510 [U] 'slot' query (LobbyApi_getStatus): the 'sele' reply's SLOTS
    int mEventProcs[5][2];              // 0x514 [U] five {routine, context} pairs set by LobbyApi_setEventRoutine (LobbyMan's
                                        //       handlers); LobbyApi_dropConnection calls [3] (+0x52C / +0x530) with a 'disc'
                                        //       event; LobbyApi_update
    char* mpRequests;                   // 0x53C [U] the pending request list (68-byte records, next at +0x40):
                                        //       LobbyApi_queueRequest links, LobbyApi_takeRequest pops, LobbyApi_disconnect frees
    int mPingsOutstanding;              // 0x540 [U] LobbyApi_disconnect 0; LobbyApi_ping counts; LobbyApi_takePing decrements
    void* mpDebugData;                  // 0x544 [R] create's 2nd argument; [U] LobbyApi_debugPrintf passes it to mpDebugProc
    void (*mpDebugProc)(void*, const char*);// 0x548 [R] create's 3rd argument; [U] LobbyApi_debugPrintf calls it with the
                                        //       formatted text
    int mRequestId;                     // 0x54C [U] LobbyApi_queueRequest numbers request records from it; LobbyApi_disconnect 0
};

CHECK_OFFSET(LobbyApiRefT, mpSession, 0x000);
CHECK_OFFSET(LobbyApiRefT, mpPing, 0x004);
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
CHECK_OFFSET(LobbyApiRefT, mUserFlags, 0x1B4);
CHECK_OFFSET(LobbyApiRefT, mRoomId, 0x1B8);
CHECK_OFFSET(LobbyApiRefT, mRoomFlags, 0x1BC);
CHECK_OFFSET(LobbyApiRefT, mRoomName, 0x1C0);
CHECK_OFFSET(LobbyApiRefT, mGameSession, 0x200);
CHECK_OFFSET(LobbyApiRefT, mGameAddr, 0x2C0);
CHECK_OFFSET(LobbyApiRefT, mGameFrom, 0x2C4);
CHECK_OFFSET(LobbyApiRefT, mGameSeed, 0x2C8);
CHECK_OFFSET(LobbyApiRefT, mGameWhen, 0x2CC);
CHECK_OFFSET(LobbyApiRefT, mGameAuth, 0x2D0);
CHECK_OFFSET(LobbyApiRefT, mLists, 0x310);
CHECK_OFFSET(LobbyApiRefT, mPort, 0x3B0);
CHECK_OFFSET(LobbyApiRefT, mAddress, 0x3B4);
CHECK_OFFSET(LobbyApiRefT, mDownText, 0x3B8);
CHECK_OFFSET(LobbyApiRefT, mSess, 0x4B8);
CHECK_OFFSET(LobbyApiRefT, mMask, 0x4BC);
CHECK_OFFSET(LobbyApiRefT, mExternalAddr, 0x4FC);
CHECK_OFFSET(LobbyApiRefT, mLocalAddr, 0x500);
CHECK_OFFSET(LobbyApiRefT, mMore, 0x504);
CHECK_OFFSET(LobbyApiRefT, mChannel, 0x508);
CHECK_OFFSET(LobbyApiRefT, mPendingSnapshots, 0x50C);
CHECK_OFFSET(LobbyApiRefT, mSlots, 0x510);
CHECK_OFFSET(LobbyApiRefT, mEventProcs, 0x514);
CHECK_OFFSET(LobbyApiRefT, mpRequests, 0x53C);
CHECK_OFFSET(LobbyApiRefT, mPingsOutstanding, 0x540);
CHECK_OFFSET(LobbyApiRefT, mpDebugData, 0x544);
CHECK_OFFSET(LobbyApiRefT, mpDebugProc, 0x548);
CHECK_OFFSET(LobbyApiRefT, mRequestId, 0x54C);
CHECK_SIZE(LobbyApiRefT, 0x550);

// LobbyApiMsgT (0x10): the event LobbyApi hands to the routines LobbyMan installs (LobbyApi_setEventRoutine): the routine is
// called as (LobbyApiRefT*, LobbyApiMsgT*, context). Name: DirtySock's, from the July 2003 preview's mangled LobbyMan
// callbacks (on*__8LobbyManP12LobbyApiMsgT, e.g. onauthentication / onchatmsg / onplaymsg / onPingReceived, and
// LobbyFunctor's invoke(LobbyApiRefT*, LobbyApiMsgT*, void*)); [R] LobbyApi builds it as four words on the stack
// (LobbyApi_dropConnection: {3, 'disc', reason, 0}); [U] every LobbyMan handler reads the reply's tag text at 0xC (TagFieldFind),
// onchatmsg switches on 0x4, most handlers test the status at 0x8. Member names are not known (no DirtySock source or
// On Tour LobbyApi here).
#ifndef __cplusplus
typedef struct LobbyApiMsgT LobbyApiMsgT;
#endif

struct LobbyApiMsgT {
    int mType;                          // 0x0 [R] LobbyApi_dropConnection: 3 (the event type); LobbyApi_handlePings's ping events 0
                                        //     (name ours)
    int mKind;                          // 0x4 [R] the event kind, a FourCC ('disc'); [U] onchatmsg switches on it
                                        //     (name ours)
    int mStatus;                        // 0x8 [R] LobbyApi_dropConnection: the reason code; [U] the handlers' status test (0 =
                                        //     success) (name ours)
    char* mpTagText;                    // 0xC [U] the reply's tag text (cDirtysock_tag_TagFieldFind's source); 0 in
                                        //     LobbyApi_dropConnection's event (name ours)
};

CHECK_OFFSET(LobbyApiMsgT, mType, 0x0);
CHECK_OFFSET(LobbyApiMsgT, mKind, 0x4);
CHECK_OFFSET(LobbyApiMsgT, mStatus, 0x8);
CHECK_OFFSET(LobbyApiMsgT, mpTagText, 0xC);
CHECK_SIZE(LobbyApiMsgT, 0x10);

#endif /* EALIB_LOBBYAPI_H */
