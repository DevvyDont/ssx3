#ifndef ONLINE_SETUPMAN_H
#define ONLINE_SETUPMAN_H

// cSetupMan: the online race setup manager. After the lobby has matched two players (cGameConnMan's channel is open),
// it runs the pre-race handshake with the peer over that comm channel: character select (pick / confirm / withdraw,
// with the 7 rider attributes), the bragging-rights rules screen (row, rule values, lock / unlock) and the start or
// cancel of the race; each step is echoed to the front end as an online::Event on LobbyMan's mEventList. The host
// (mIsHost) owns the state machine; the client mirrors it.
// EA's source: d:/ssx/code/source/online/setupman.cpp (the July 2003 preview's assert strings, with "!mSetupMan" and
// the allocation tag "cSetupMan" in the same function). Retail code: dirtysock/tagsunk 0x00264030..0x00265768, plus
// the per-message handlers cSetupMan_handleCharPickEcho .. cSetupMan_handleBragUnlockMessage in bx/bxstringctor (see the methods file).
// One instance, cSetupMan_mSetupMan (EA: the static cSetupMan::mSetupMan, preview assert "!mSetupMan"): cSetupMan_createInstance creates
// it, cSetupMan_destroyInstance destroys it. No On Tour counterpart (On Tour has no online code).
//
// Size 0x12C: [R] cSetupMan_createInstance: cMemMan_alloc(300, "cSetupMan", 0x20000000) + the constructor cSetupMan::cSetupMan, which
// stores every word except the queue slots (cleared with MEM_fill) up to 0x128. No vtable, no base class (the
// destructor cSetupMan::~cSetupMan is called directly with flags 3).
// The two packet queues are the same 32-slot ring of tGameCommPacket* that cGameComm uses (online/gamecomm.h
// tGameCommQueue: mHead push count, mTail pop count, mQueue[32]); packets are 0xF0-byte "tGameCommPacket" buffers
// (cSetupMan_allocPacket). Field names for them follow cGameComm's (On Tour names); the rest are named from retail use.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] the constructor cSetupMan::cSetupMan, the destructor cSetupMan::~cSetupMan, the allocation in cSetupMan_createInstance;
//   [U] how retail code uses the field (src/dirtysock/tagsunk.cpp views sRingOwner_264138, sNetSession42B8,
//       sDsPacketQueueK2, sDsRecvQueue_00264A58, sDsSendQueue_00264B20 / _00264BE8, and the asm).
// mState values (retail use): 1 start (ctor, dtor); 2 the host has a pick; 3 the peer confirmed; 4 / 5 this host
// confirmed (5: both confirmed); 6..10 bragging-rights phase (7 rules being edited, 9 / 10 panels locked); 11 cancelled
// (cSetupMan_cancelSetup, peer codes 0xF8 / 250); 12 the host sent "start" (0xF); 13 the race starts.

#include "layout.h"
#include "online/gamecomm.h"            // tGameCommQueue, tGameCommPacket

void operator_delete(int* p);

struct cSetupMan {
    int mState;                     // 0x000 [R] ctor 1, dtor 1; [U] values above; cGameConnMan's cGameConnMan_update reads
                                    //       it through cSetupMan_mSetupMan (12 / 13: start the race)
    tGameCommPacket* mPacketCache;  // 0x004 [R] ctor 0; [U] the buffer being read into (cSetupMan_receivePackets allocates it,
                                    //       cCommSystem_readPacket fills it, queued or freed); the dtor frees it
                                    //       (cGameComm::mPacketCache's role)
    tGameCommQueue mReadQueue;      // 0x008 [R] ctor: mHead/mTail 0, slots cleared (MEM_fill 0x80 at 0x10);
                                    //       [U] cSetupMan_receivePackets pushes (mHead++), cSetupMan_readMessage pops (mTail++), the dtor
                                    //       frees what is left
    tGameCommQueue mWriteQueue;     // 0x090 [R] ctor: counts 0, slots cleared (0x98); [U] cSetupMan_sendMessage pushes a copy
                                    //       when the channel refuses a packet, cSetupMan_flushSendQueue sends and pops
    int unk_118;                    // 0x118 [R] ctor 0; no other access seen
    int unk_11C;                    // 0x11C [R] ctor 0; no other access seen
    int mChannel;                   // 0x120 [R] ctor argument 1 (cSetupMan_createInstance's channel, from cGameConnMan); [U] the
                                    //       cCommSystem_readPacket / writePacket channel
    int mIsHost;                    // 0x124 [R] ctor argument 2; [U] every step branches on it (host vs client
                                    //       message codes)
    int mRanked;                    // 0x128 [R] ctor argument 3 (cBENetworkInterface_networkOpt.bIsRankedGame);
                                    //       [U] cSetupMan_sendCharacterMessage sends flat attributes (40) when set; cSetupMan_update skips
                                    //       the bragging-rights phase

    // Methods bound to their retail symbols by asm labels.
    void handleCharPickEcho(int riderId, int hostState) __asm__("cSetupMan_handleCharPickEcho");
    void handleCharConfirmEcho(int riderId, int hostState) __asm__("cSetupMan_handleCharConfirmEcho");
    void handleWithdrawEcho(int hostState) __asm__("cSetupMan_handleWithdrawEcho");
    void handleHostCharPick(int riderId, int hostState, void* riderStats) __asm__("cSetupMan_handleHostCharPick");
    void handleHostCharConfirm(int riderId, int hostState) __asm__("cSetupMan_handleHostCharConfirm");
    void handleHostWithdraw(int hostState) __asm__("cSetupMan_handleHostWithdraw");
    void handleClientBragRow(int rowIndex) __asm__("cSetupMan_handleClientBragRow");
    void handleClientBragRule(int ruleSlot, int ruleValue) __asm__("cSetupMan_handleClientBragRule");
    void handleClientBragLock() __asm__("cSetupMan_handleClientBragLock");
    void handleClientBragUnlock() __asm__("cSetupMan_handleClientBragUnlock");
    void handleHostBragRow(int rowIndex) __asm__("cSetupMan_handleHostBragRow");
    void handleBragRowMessage(int rowIndex) __asm__("cSetupMan_handleBragRowMessage");
    void handleHostBragRule(int ruleSlot, int ruleValue) __asm__("cSetupMan_handleHostBragRule");
    void handleBragRuleMessage(int ruleSlot, int ruleValue) __asm__("cSetupMan_handleBragRuleMessage");
    void handleHostBragLock(int hostState) __asm__("cSetupMan_handleHostBragLock");
    void handleBragLockAnswer(int hostState) __asm__("cSetupMan_handleBragLockAnswer");
    void handleHostBragUnlock(int hostState) __asm__("cSetupMan_handleHostBragUnlock");
    void handleBragUnlockMessage(int hostState) __asm__("cSetupMan_handleBragUnlockMessage");
    void update() __asm__("cSetupMan_update");
    void freePacket(int* packet) __asm__("cSetupMan_freePacket");
    void receivePackets() __asm__("cSetupMan_receivePackets");
    void flushSendQueue() __asm__("cSetupMan_flushSendQueue");
    void sendRank() __asm__("cSetupMan_sendRank");
    void applyMyCharacter(int charID) __asm__("cSetupMan_applyMyCharacter");
    void sendCharacterMessage(int type, int rider, int state) __asm__("cSetupMan_sendCharacterMessage");
    void highlightCharacter(int rider) __asm__("cSetupMan_highlightCharacter");
    void confirmCharacter(int rider) __asm__("cSetupMan_confirmCharacter");
    void unconfirmCharacter() __asm__("cSetupMan_unconfirmCharacter");
    void handleOpponentConfirm(int rider) __asm__("cSetupMan_handleOpponentConfirm");
    void sendBraggingSelection(int id) __asm__("cSetupMan_sendBraggingSelection");
    void sendBraggingRule(int ruleSlot, int newValue) __asm__("cSetupMan_sendBraggingRule");
    void unlockBragging() __asm__("cSetupMan_unlockBragging");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    cSetupMan(int channel, int isHost, int ranked);
    ~cSetupMan();
    static void operator delete(void* p) { operator_delete((int*)p); }
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
