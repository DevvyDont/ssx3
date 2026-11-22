#ifndef ONLINE_SETUPMAN_H
#define ONLINE_SETUPMAN_H

// cSetupMan: the online race setup manager. After the lobby has matched two players (cGameConnMan's channel is open),
// it runs the pre-race handshake with the peer over that comm channel: character select (pick / confirm / withdraw,
// with the 7 rider attributes), the bragging-rights rules screen (row, rule values, lock / unlock) and the start or
// cancel of the race; each step is echoed to the front end as an online::Event on LobbyMan's mEventList. The host
// (mIsHost) owns the state machine; the client mirrors it.
// EA's source: d:/ssx/code/source/online/setupman.cpp (the July 2003 preview's assert strings, with "!mSetupMan" and
// the allocation tag "cSetupMan" in the same function). Retail code: dirtysock/tagsunk 0x00264030..0x00265768, plus
// the per-message handlers func_00269008 .. func_00269CF0 in bx/bxstringctor (see the methods file).
// One instance, cSetupMan_mSetupMan (EA: the static cSetupMan::mSetupMan, preview assert "!mSetupMan"): func_00264030 creates
// it, func_00264098 destroys it. No On Tour counterpart (On Tour has no online code).
//
// Size 0x12C: [R] func_00264030: cMemMan_alloc(300, "cSetupMan", 0x20000000) + the constructor cSetupMan_cSetupMan, which
// stores every word except the queue slots (cleared with MEM_fill) up to 0x128. No vtable, no base class (the
// destructor cSetupMan__cSetupMan is called directly with flags 3).
// The two packet queues are the same 32-slot ring of tGameCommPacket* that cGameComm uses (online/gamecomm.h
// tGameCommQueue: mHead push count, mTail pop count, mQueue[32]); packets are 0xF0-byte "tGameCommPacket" buffers
// (func_00264960). Field names for them follow cGameComm's (On Tour names); the rest are named from retail use.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] the constructor cSetupMan_cSetupMan, the destructor cSetupMan__cSetupMan, the allocation in func_00264030;
//   [U] how retail code uses the field (src/dirtysock/tagsunk.cpp views sRingOwner_264138, sNetSession42B8,
//       sDsPacketQueueK2, sDsRecvQueue_00264A58, sDsSendQueue_00264B20 / _00264BE8, and the asm).
// mState values (retail use): 1 start (ctor, dtor); 2 the host has a pick; 3 the peer confirmed; 4 / 5 this host
// confirmed (5: both confirmed); 6..10 bragging-rights phase (7 rules being edited, 9 / 10 panels locked); 11 cancelled
// (func_00265768, peer codes 0xF8 / 250); 12 the host sent "start" (0xF); 13 the race starts.

#include "layout.h"
#include "online/gamecomm.h"            // tGameCommQueue, tGameCommPacket

struct cSetupMan {
    int mState;                     // 0x000 [R] ctor 1, dtor 1; [U] values above; cGameConnMan's func_00263CF0 reads
                                    //       it through cSetupMan_mSetupMan (12 / 13: start the race)
    tGameCommPacket* mPacketCache;  // 0x004 [R] ctor 0; [U] the buffer being read into (func_00264A58 allocates it,
                                    //       cCommSystem_readPacket fills it, queued or freed); the dtor frees it
                                    //       (cGameComm::mPacketCache's role)
    tGameCommQueue mReadQueue;      // 0x008 [R] ctor: mHead/mTail 0, slots cleared (MEM_fill 0x80 at 0x10);
                                    //       [U] func_00264A58 pushes (mHead++), func_002649B0 pops (mTail++), the dtor
                                    //       frees what is left
    tGameCommQueue mWriteQueue;     // 0x090 [R] ctor: counts 0, slots cleared (0x98); [U] func_00264BE8 pushes a copy
                                    //       when the channel refuses a packet, func_00264B20 sends and pops
    int unk_118;                    // 0x118 [R] ctor 0; no other access seen
    int unk_11C;                    // 0x11C [R] ctor 0; no other access seen
    int mChannel;                   // 0x120 [R] ctor argument 1 (func_00264030's channel, from cGameConnMan); [U] the
                                    //       cCommSystem_readPacket / writePacket channel
    int mIsHost;                    // 0x124 [R] ctor argument 2; [U] every step branches on it (host vs client
                                    //       message codes)
    int mRanked;                    // 0x128 [R] ctor argument 3 (cBENetworkInterface_networkOpt.bIsRankedGame);
                                    //       [U] func_00264E50 sends flat attributes (40) when set; func_002642B8 skips
                                    //       the bragging-rights phase
};

CHECK_OFFSET(cSetupMan, mState, 0x000);
CHECK_OFFSET(cSetupMan, mPacketCache, 0x004);
CHECK_OFFSET(cSetupMan, mReadQueue, 0x008);
CHECK_OFFSET(cSetupMan, mWriteQueue, 0x090);
CHECK_OFFSET(cSetupMan, unk_118, 0x118);
CHECK_OFFSET(cSetupMan, unk_11C, 0x11C);
CHECK_OFFSET(cSetupMan, mChannel, 0x120);
CHECK_OFFSET(cSetupMan, mIsHost, 0x124);
CHECK_OFFSET(cSetupMan, mRanked, 0x128);
CHECK_SIZE(cSetupMan, 0x12C);

#endif /* ONLINE_SETUPMAN_H */
