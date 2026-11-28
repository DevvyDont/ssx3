#ifndef ONLINE_GAMECOMM_H
#define ONLINE_GAMECOMM_H

// cGameComm: the in-race link of an online game (two humans, peer to peer over one cCommSystem channel). It runs the
// pre-race handshake (settings, BE sync data, latency: the tGameComm states below), carries the input samples that
// cInputSync exchanges every frame (lockstep with a shared AI random seed), pause/unplug/exit commands, chat, and a
// once-a-second checksum used to detect desyncs; it also tracks the comm errors that the results report (REPT) sends.
// EA's source: d:/ssx/code/source/online/gamecomm.cpp and online/gamecomm.h (the preview build's assert strings).
//
// One instance: cGameComm_construct(channel, server) does `cMemMan_alloc(0x5F0, "cGameComm", ...)` + the constructor
// cGameComm::cGameComm into cGameComm::mGameComm (retail cGameComm_mGameComm); cGameComm_destroy (destroy) runs the destructor
// cGameComm__cGameComm. Size 0x5F0 [R]. No base class and no virtuals (the constructor stores no vtable; On Tour has none).
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] retail's constructor cGameComm::cGameComm (initial values below) and the allocation size;
//   [U] how retail code uses the field: the class's own methods (cGameComm::cGameComm..cGameComm_updateSyncPacket in seg_156860) and the
//       ~50 users of cGameComm_mGameComm (ovstatepause, ovtemplatepausemenu, gamestate's cInputSync, ai, gameload, ...). The
//       results reporter LobbyMan_reportRaceResult (tagsunk) sends many fields under DirtySock tags, quoted as "TAG";
//   [O] SSX On Tour's DWARF (class cGameComm, size 0x101C): same member order where both games have the member. On
//       Tour is a 4-player rewrite (per-player [4] arrays, 1-byte bools, mPlayerID/mNumPlayers, disable/rejoin), so a
//       name is used only where the retail use matches; On Tour's [4] arrays are [2] here (index 0 = this machine,
//       1 = the peer).
// Retail bools are 4-byte ints. Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"

// tGameComm: the handshake/race state machine, cGameComm::mState (On Tour enum tGameComm, same values; retail's
// update cGameComm_update walks them on the server and, with each Send/Recv pair swapped, on the client).
enum tGameComm {
    kGameComm_Init = 0,
    kGameComm_SendSettings = 1,
    kGameComm_SendSettingsWait = 2,
    kGameComm_RecvSettings = 3,
    kGameComm_RecvSettingsWait = 4,
    kGameComm_SendBE = 5,
    kGameComm_SendBEWait = 6,
    kGameComm_RecvBE = 7,
    kGameComm_RecvBEWait = 8,
    kGameComm_SendLatency = 9,
    kGameComm_SendLatencyWait = 10,
    kGameComm_RecvLatency = 11,
    kGameComm_RecvLatencyWait = 12,
    kGameComm_InitGameActive = 13,
    kGameComm_GameActive = 14,     // cGameComm_syncGame loops until this state
    kGameComm_GameOver = 15        // gameEnded (cGameComm_gameEnded)
};

// tGameCommPacket: one queued packet, 0xF0 bytes ([R] cGameComm_allocPacket: cMemMan_alloc(0xF0, "tGameCommPacket")).
// On Tour: tGameCommPacket (0xF0) = its 4-byte base tGameCommHeader {mType, mPlayerID, mSize} + mData[236]; written
// flat here.
struct tGameCommPacket {
    unsigned char mType;         // 0x00 [O] tGameCommHeader::mType; [U] the queue it goes to (mReadQueue[mType]):
                                 //      0 input samples (cInputSync), 1 chat (sendChat cGameComm_sendChat, getQueuedChat
                                 //      cGameComm_getQueuedChat), 2 BE sync data (asyncRead/asyncWrite), 3 commands (tGameCmdPacket)
    unsigned char mPlayerID;     // 0x01 [O]; [U] writePacket (cGameComm_writePacket) stores 0
    unsigned short mSize;        // 0x02 [O]; [U] the packet size including this header (writePacket: len + 4)
    unsigned char mData[236];    // 0x04 [O]; [U] payload (writePacket copies len bytes)
};

// tGameCmdPacket: the 0xC-byte payload of a type-3 (command) packet (On Tour tGameCmdPacket, same layout). Retail
// commands: 0 pause and 1 unpause (sendPauseCmd cGameComm_sendPauseCmd), 2 controller unplugged (sendUnplugCmd cGameComm_sendUnplugCmd),
// 3 sync-wait done (syncWait cGameComm_syncWait), 4 checksum (updateSyncPacket cGameComm_updateSyncPacket), 5 exit game (sendExitGame
// cGameComm_sendExitGame); handled by updateCmdQueue cGameComm_updateCmdQueue.
struct tGameCmdPacket {
    unsigned char Cmd;           // 0x0 [O]; [U] the command number above
    unsigned char FromPlayerID;  // 0x1 [O]; never set in retail
    char unk_02[0x2];            // 0x2 padding
    int AITick;                  // 0x4 [O]; [U] command 4: the frame the checksum belongs to
    unsigned int CheckSum;       // 0x8 [O]; [U] command 4: the checksum
};

// tGameCommQueue: a 32-slot ring of packet pointers (On Tour tGameCommQueue, same layout).
struct tGameCommQueue {
    int mHead;                   // 0x00 [O]; [U] push count: mQueue[mHead++ % 32] = packet (updateReadQueue, writePacket)
    int mTail;                   // 0x04 [O]; [U] pop count: mQueue[mTail++ % 32] (readPacket, updateWriteQueue)
    tGameCommPacket* mQueue[32]; // 0x08 [O]; [R] the constructor clears them
};

// tServerSettings: what the server sends in kGameComm_SendSettings (0x2C bytes, cGameComm_update). On Tour's version
// (0x1C) is AIRandSeed[6] + NumPlayers; retail has the server's link info instead of NumPlayers.
struct tServerSettings {
    unsigned int AIRandSeed[6];  // 0x00 [O]; [U] cGameComm_syncGame saves the AI random state into it
                                 //      (AIgetseed, bx/bxrandom) on the server and restores it (AIsetseed) once synced
    int mBroadBand;              // 0x18 (name ours) [U] the server's cCommSystem::mBroadBand (syncGame)
    char mSignature[0x10];       // 0x1C (name ours) [U] the server's build signature "NA_R_F004" (strncpy 0xF in
                                 //      syncGame); compared with the client's (9 chars) after the handshake
};

struct cGameComm {
    int mConnected;              // 0x000 [O]; [R] 1; [U] cleared by localDisconnected (cGameComm_localDisconnected), displayError
                                 //       (cGameComm_displayError) and the remote exit command; senders require it
    int mError;                  // 0x004 [O]; [R] 0; [U] displayError sets 1; nearly every method returns early on it
    int mDisplayError;           // 0x008 [O] (int); [U] displayError sets 1; updateDisplayError (cGameComm_updateDisplayError) counts it
                                 //       down and raises overlay 0x27
    int mDesync;                 // 0x00C [O]; [U] updateSyncPacket sets 1 on a checksum mismatch; reported as "DESYNC"
    int mLocalDisconnected;      // 0x010 [O]; [U] localDisconnected sets 1; "DIDQUIT0/1"; bit 2 of mFlagsOnCommError
    int mRemoteDisconnected;     // 0x014 [O]; [U] the exit-game command (5) sets 1; "DIDQUIT0/1"; bit 3 of the flags
    int mCommErrorWasSignificant; // 0x018 [O]; [U] setCommError (cGameComm_setCommError); reported as "SCOMERR"
    int mStateOnCommError;       // 0x01C [O] tGameComm; [U] setCommError copies mState; "CERRSTAT"
    unsigned int mFlagsOnCommError; // 0x020 [O]; [U] setCommError packs 11 flags (mLocalFinishedRace = bit 0 ...
                                 //       mLoading = bit 10); "CERRFLGS"
    int mCheating[2];            // 0x024 [O] (On Tour [4]); [U] checkErrors (cGameComm_checkErrors) sets 2 (6+ pauses) or 3
                                 //       (3+ unplugs) for this machine / the peer; "CHEAT0/1"
    // mCheating's reasons (names ours). [U] cGameComm_checkActive sets kCheatDiscTrayOpened when the disc tray was
    // opened (D_004A3ED0); checkErrors sets kCheatTooManyPauses (6+ pauses) and kCheatTooManyUnplugs (3+ controller
    // unplugs); displayError sets kCheatControllerUnplugged for an unplugged controller mid-race. The online error
    // overlay (cOVState_PAUSE_ONLINE_ERROR_onCreateScreen) reads them.
    enum {
        kCheatNone = 0,
        kCheatDiscTrayOpened = 1,
        kCheatTooManyPauses = 2,
        kCheatTooManyUnplugs = 3,
        kCheatControllerUnplugged = 4
    };
    int mLinkTimedOut;           // 0x02C (name ours) [R] 0; [U] set to 1 when the link times out (mLinkTimeout runs
                                 //       out in checkErrors, mSyncInputDelay reaches 1800 in cGameComm_checkActive);
                                 //       never read in retail
    int mCommError;              // 0x030 [O]; [U] setCommError sets 1 (once: it guards the other fields); "COMMERR"
    int mRaceStartTime;          // 0x034 (name ours) [R] the time at construction; [U] gameStartRace
                                 //       (cGameComm_gameStartRace) stores the race start time (SSXApp +0x18 / +0x10);
                                 //       the reporter subtracts it from mLocalEndTime / mRemoteEndTime
    int mLocalEndTime;           // 0x038 (name ours) [U] this machine's end time (localDisconnected, gameEnded
                                 //       cGameComm_gameEnded)
    int mRemoteEndTime;          // 0x03C (name ours) [U] the peer's end time (exit-game command, gameEnded)
    int mSyncInput;              // 0x040 [O] (just before mServer in both games); [R] 1; [U] cInputSync (gamestate
                                 //       cInputSync_preupdate / cInputSync_sendClientInput) exchanges pads only while it is set;
                                 //       updateSyncPacket requires it; displayError, gameEnded and the out-of-line
                                 //       stopSyncInput cGameComm_stopSyncInput (main/ssxappdtor: sw $0, 0x40) clear it
    int mServer;                 // 0x044 [O]; [R] the constructor's second argument; [U] update/syncGame take the
                                 //       server side of the handshake when set
    int mAlone;                  // 0x048 [O] (On Tour's next member after mServer that retail has); [U] setFinishRaceAlone
                                 //       (cGameComm_setFinishRaceAlone, from ai cAI_finishRemotePlayer) sets 1; ai cAI_onlineAICheck and the timeouts test it
    int mPauseCount[2];          // 0x04C [O] (On Tour [4]); [U] [0] counted by sendPauseCmd(true), [1] by the pause
                                 //       command; 6+ means cheating 2
    int mLocalUnplugCount;       // 0x054 (name ours) [U] this machine's controller-unplug count (sendUnplugCmd); 3+
                                 //       means cheating 3
    int mRemoteUnplugCount;      // 0x058 (name ours) [U] the peer's unplug count (command 2); 3+ means cheating 3
    int mSyncInputDelay;                 // 0x05C [U] frames without a packet while the peer isn't paused (cGameComm_checkActive); 1800 ends
                                 //       the link; cleared when an input packet arrives (readPacket channel 0)
    int mWaitingTimer;                 // 0x060 [R] 30; [U] frames without a packet (cGameComm_checkActive); at 60 mWaitingMessage is set
                                 //       (On Tour has mWaitingTimer[4] in this area)
    int mOverlayTimeout;         // 0x064 [O]; [R] -1; [U] the pause overlays start it (0x4B0, cOVState_MULTI_PREEVENT_onGainFocus; 0x8CA0 in
                                 //       ovtemplatedialog), stop it (-1) and test == 0; counted down by cGameComm_updateTimeout
    int mPauseTimeout;           // 0x068 [O]; [R] -1; [U] counted down by cGameComm_updateTimeout, tested and reset to -1 by the pause
                                 //       menu; never armed in retail
    int mChannel;                // 0x06C (name ours) [R] the constructor's first argument: the cCommSystem channel
                                 //       index passed to its readPacket/writePacket/getLatency/getError (On Tour has no
                                 //       channel member)
    int mState;                  // 0x070 [O] tGameComm; [U] setState (cGameComm_setState) stores it; cInputSync waits for >= 14
    int mPauseMenu[2];           // 0x074 [O] (On Tour [4]); [U] [0] this machine paused (sendPauseCmd), [1] the peer
                                 //       (commands 0/1)
    int mGameStartGate;          // 0x07C [O]; [U] gameStartGate (cGameComm_gameStartGate, from cGFGateState_gainFocus) sets 1
    int mRaceInProgress;         // 0x080 [O]; [U] gameStartRace sets 1, gameEnded clears it
    int mWaitingMessage;         // 0x084 [O]; [U] set when the peer goes quiet (mWaitingTimer == 60); updateMessageDisplay
                                 //       (cGameComm_updateMessageDisplay) then shows "kT_OVRCMNWaitPlayer"
    int mRemoteExitGame;         // 0x088 [O]; [U] the exit-game command sets 1
    int mRemoteEndGame;          // 0x08C [O]; [U] the exit-game command sets 1 while the race still runs; checkErrors
                                 //       then calls displayError
    int mAtFinishLine;           // 0x090 (name ours) [R] 0; [U] festateloadscreen cGFFinishlineState_gainFocus (the
                                 //       finish-line results state starting) sets 1 right after stopSyncInput; read
                                 //       only into bit 9 of mFlagsOnCommError (On Tour's setCommError skips bit 9)
    int mLinkTimeout;            // 0x094 (name ours) [U] link timeout in frames: cGameComm_armLinkTimeout arms it
                                 //       (1800/3600/3660), checkErrors counts it down (0 = timed out, mLinkTimedOut)
    int mMessageDisplay;         // 0x098 [O]; [U] updateMessageDisplay's popup state (0 none, 1 peer paused, 2 waiting)
    int mPingResult;             // 0x09C (name ours) [R] 1; [U] ping result: cleared by the online-error overlay
                                 //       (cOVState_PAUSE_ONLINE_ERROR_onUpdate) when the ping times out; "PINGRSLT"
    int mLocalFinishedRace;      // 0x0A0 [O]; [U] "FINISH0/1"; bit 0 of the flags
    int mRemoteFinishedRace;     // 0x0A4 [O]; [U] "FINISH0/1"; bit 1
    int mStallThreshold;         // 0x0A8 [O] (On Tour [4], same place: before mLoading); [U] frames since the last input
                                 //       packet; at 10 a stall is counted (mStallCount) and mStallTimer grows
    int mLoading;                // 0x0AC [O]; [R] 1; [U] syncWait clears it; updateDisplayError waits while it is set
    float mStallTimer;           // 0x0B0 [O] (On Tour [4]); [U] +1.0 per stalled frame, -0.15 per update; 3600 = error
    int mStallCount;             // 0x0B4 [O] (On Tour [4]); [U] "STALLCNT"
    int mPingCount;              // 0x0B8 (name ours) [U] ping server (cGameComm_update / cGameComm_startServerPing): number of
                                 //       pings sent (max 4)
    int mPingDelay;              // 0x0BC (name ours) [U] ping: frames until the next ping
    int mPingAddr;               // 0x0C0 (name ours) [U] ping: the address being pinged (cGameComm_startServerPing's argument)
    int mPingID[4];              // 0x0C4 (name ours) [U] ping: the id of each ping sent (matched against replies);
                                 //       [R] cleared
    tServerSettings mServerSettings; // 0x0D4 [O] (On Tour 0xFC, 0x1C bytes); [U] sent by the server (0x2C bytes)
    int mClientBroadBand;        // 0x100 (name ours) [U] the client's cCommSystem::mBroadBand (sent by the client
                                 //       with mClientSignature, 0x14 bytes)
    char mClientSignature[0x10]; // 0x104 (name ours) [U] the client's build signature "NA_R_F004"
    float mNetLatency[2];        // 0x114 [O] (On Tour [4]); [R] the default frame time (SSXApp +0x14); [U] the latency
                                 //       measured on the server [0] and on the client [1] (states 9/11)
    float mLatency;              // 0x11C [O]; [U] calcLatency (cGameComm_calcLatency): max of mNetLatency, at least one frame;
                                 //       syncGame copies it into tBENetworkDB::fLatency
    int mNumSamples;             // 0x120 [O]; [R] 3; [U] syncGame: 2 when both sides are broadband; cGame_load passes it
                                 //       to cInputSync as its samples per packet; getLatency (cGameComm_getLatency) adds frames
    char* mAsyncReadData;        // 0x124 [O] (On Tour [4]); [U] asyncRead (cGameComm_asyncRead); asyncReadComplete tests 0
    int mAsyncReadSize;          // 0x128 [O]; [U] asyncReadUpdate (cGameComm_asyncReadUpdate) reads in 0xEC-byte pieces
    char* mAsyncWriteData;       // 0x12C [O]; [U] asyncWrite (cGameComm_asyncWrite); asyncWriteComplete tests 0
    int mAsyncWriteSize;         // 0x130 [O]; [U] asyncWriteUpdate (cGameComm_asyncWriteUpdate)
    tGameCommPacket* mPacketCache; // 0x134 [O]; [U] the packet being received (updateReadQueue cGameComm_updateReadQueue); the
                                 //       destructor frees it
    int mPingTimer;              // 0x138 (name ours) [U] ping: frames left (cGameComm_startServerPing sets seconds * 60); the
                                 //       online-error overlay tests it
    int mPingReceived;           // 0x13C (name ours) [U] ping: a reply arrived (also set without broadband); the
                                 //       online-error overlay then shows its ping-received message
    void* mPingSocket;           // 0x140 (name ours) [R] PingRpc_create() (a DirtySock handle; the destructor frees it
                                 //       with PingRpc_destroy); [U] the ping server's socket ("SSXPingServer")
    tGameCommQueue mReadQueue[4]; // 0x144 [O] (On Tour [4][4]); [U] one per packet type (tGameCommPacket::mType)
    tGameCommQueue mWriteQueue;  // 0x364 [O] (On Tour [4]); [U] packets waiting for the channel (updateWriteQueue)
    char* mBEBuffer;             // 0x3EC [O] (On Tour [4]); [U] syncGame allocates it ("bebuffer", mBEBufferSize) for
                                 //       cBENetworkInterface getSyncData/setSyncData
    int mBEBufferSize;           // 0x3F0 [O]; [R] cBENetworkInterface::getSyncDataSize() (0xC44)
    int mSyncWaitComplete;       // 0x3F4 [O]; [U] command 3 sets it; syncWait loops until it is set
    int mUnPauseFlag;            // 0x3F8 [O]; [U] set by a pause during the race, cleared by an unpause; cInputSync's
                                 //       cInputSync_aiinputready holds the race while it is set
    int mUnPauseTimer;           // 0x3FC [O]; [U] setUnPauseTimer (cGameComm_setUnPauseTimer) sets 180; counted down by update; the
                                 //       HUD (cOVStateHUD1P_onRender2D) shows it
    int mCheckSumSent;           // 0x400 [O]; [U] updateSyncPacket
    int mCheckSumReceived;       // 0x404 [O] (On Tour [4]); [U] command 4 sets it
    int mAITick[2];              // 0x408 [O] (On Tour [4]); [R] -1; [U] the last checksummed frame here [0] / from the
                                 //       peer [1]
    unsigned int mCheckSumBuffer[2][60]; // 0x410 [O] (On Tour [60][4]: frame-major); [U] [0] this machine's checksums,
                                 //       [1] the peer's, at [(frame / 60) % 60]; [R] both cleared (MEM_fill 0xF0)

    // Methods bound to their retail symbols by asm labels.
    void stopSyncInput() __asm__("cGameComm_stopSyncInput__FPv");
    void setState(int mode) __asm__("cGameComm_setState");
    void update() __asm__("cGameComm_update");
    void localDisconnected(int force) __asm__("cGameComm_localDisconnected");
    void setCommError() __asm__("cGameComm_setCommError");
    void updateTimeout() __asm__("cGameComm_updateTimeout");
    void displayError() __asm__("cGameComm_displayError");
    void updateDisplayError() __asm__("cGameComm_updateDisplayError");
    int readPacket(int ch, void* dst, int maxsize) __asm__("cGameComm_readPacket");
    void updateReadQueue() __asm__("cGameComm_updateReadQueue");
    void updateWriteQueue() __asm__("cGameComm_updateWriteQueue");
    void updateCmdQueue() __asm__("cGameComm_updateCmdQueue");
    void freePacket(int *packet) __asm__("cGameComm_freePacket");
    int asyncReadComplete() __asm__("cGameComm_asyncReadComplete");
    void asyncRead(void* data, int size) __asm__("cGameComm_asyncRead");
    void asyncReadUpdate() __asm__("cGameComm_asyncReadUpdate");
    int asyncWriteComplete() __asm__("cGameComm_asyncWriteComplete");
    void asyncWrite(void* data, int size) __asm__("cGameComm_asyncWrite");
    void asyncWriteUpdate() __asm__("cGameComm_asyncWriteUpdate");
    void calcLatency() __asm__("cGameComm_calcLatency");
    float getLatency() __asm__("cGameComm_getLatency");
    void syncGame() __asm__("cGameComm_syncGame");
    void syncWait() __asm__("cGameComm_syncWait");
    void sendPauseCmd(int pause) __asm__("cGameComm_sendPauseCmd");
    void setUnPauseTimer() __asm__("cGameComm_setUnPauseTimer");
    void setFinishRaceAlone() __asm__("cGameComm_setFinishRaceAlone");
    void gameStartGate() __asm__("cGameComm_gameStartGate");
    void gameStartRace() __asm__("cGameComm_gameStartRace");
    void gameEnded() __asm__("cGameComm_gameEnded");
    void gameExit() __asm__("cGameComm_gameExit");
    void sendExitGame() __asm__("cGameComm_sendExitGame");
    void sendUnplugCmd() __asm__("cGameComm_sendUnplugCmd");
    void sendChat(const char* message) __asm__("cGameComm_sendChat");
    void updateMessageDisplay() __asm__("cGameComm_updateMessageDisplay");
    void updateSyncPacket(int frame, int value) __asm__("cGameComm_updateSyncPacket");
    int checkErrors() __asm__("cGameComm_checkErrors");
    void checkActive() __asm__("cGameComm_checkActive");
    int writePacket(int type, const void* data, int len) __asm__("cGameComm_writePacket");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    cGameComm(int port, int isServer);
    void startServerPing(int serverAddr, int durationSecs) __asm__("cGameComm_startServerPing");
    void armLinkTimeout(int timeoutFrames, int forceArm) __asm__("cGameComm_armLinkTimeout");
};

CHECK_SIZE(tGameCommPacket, 0xF0);
CHECK_OFFSET(tGameCommPacket, mType, 0x0);
CHECK_OFFSET(tGameCommPacket, mPlayerID, 0x1);
CHECK_OFFSET(tGameCommPacket, mSize, 0x2);
CHECK_OFFSET(tGameCommPacket, mData, 0x4);
CHECK_SIZE(tGameCmdPacket, 0xC);
CHECK_OFFSET(tGameCmdPacket, Cmd, 0x0);
CHECK_OFFSET(tGameCmdPacket, FromPlayerID, 0x1);
CHECK_OFFSET(tGameCmdPacket, unk_02, 0x2);
CHECK_OFFSET(tGameCmdPacket, AITick, 0x4);
CHECK_OFFSET(tGameCmdPacket, CheckSum, 0x8);
CHECK_SIZE(tGameCommQueue, 0x88);
CHECK_OFFSET(tGameCommQueue, mHead, 0x0);
CHECK_OFFSET(tGameCommQueue, mTail, 0x4);
CHECK_OFFSET(tGameCommQueue, mQueue, 0x8);
CHECK_SIZE(tServerSettings, 0x2C);
CHECK_OFFSET(tServerSettings, AIRandSeed, 0x0);
CHECK_OFFSET(tServerSettings, mBroadBand, 0x18);
CHECK_OFFSET(tServerSettings, mSignature, 0x1C);

CHECK_OFFSET(cGameComm, mConnected, 0x000);
CHECK_OFFSET(cGameComm, mError, 0x004);
CHECK_OFFSET(cGameComm, mDisplayError, 0x008);
CHECK_OFFSET(cGameComm, mDesync, 0x00C);
CHECK_OFFSET(cGameComm, mLocalDisconnected, 0x010);
CHECK_OFFSET(cGameComm, mRemoteDisconnected, 0x014);
CHECK_OFFSET(cGameComm, mCommErrorWasSignificant, 0x018);
CHECK_OFFSET(cGameComm, mStateOnCommError, 0x01C);
CHECK_OFFSET(cGameComm, mFlagsOnCommError, 0x020);
CHECK_OFFSET(cGameComm, mCheating, 0x024);
CHECK_OFFSET(cGameComm, mLinkTimedOut, 0x02C);
CHECK_OFFSET(cGameComm, mCommError, 0x030);
CHECK_OFFSET(cGameComm, mRaceStartTime, 0x034);
CHECK_OFFSET(cGameComm, mLocalEndTime, 0x038);
CHECK_OFFSET(cGameComm, mRemoteEndTime, 0x03C);
CHECK_OFFSET(cGameComm, mSyncInput, 0x040);
CHECK_OFFSET(cGameComm, mServer, 0x044);
CHECK_OFFSET(cGameComm, mAlone, 0x048);
CHECK_OFFSET(cGameComm, mPauseCount, 0x04C);
CHECK_OFFSET(cGameComm, mLocalUnplugCount, 0x054);
CHECK_OFFSET(cGameComm, mRemoteUnplugCount, 0x058);
CHECK_OFFSET(cGameComm, mSyncInputDelay, 0x05C);
CHECK_OFFSET(cGameComm, mWaitingTimer, 0x060);
CHECK_OFFSET(cGameComm, mOverlayTimeout, 0x064);
CHECK_OFFSET(cGameComm, mPauseTimeout, 0x068);
CHECK_OFFSET(cGameComm, mChannel, 0x06C);
CHECK_OFFSET(cGameComm, mState, 0x070);
CHECK_OFFSET(cGameComm, mPauseMenu, 0x074);
CHECK_OFFSET(cGameComm, mGameStartGate, 0x07C);
CHECK_OFFSET(cGameComm, mRaceInProgress, 0x080);
CHECK_OFFSET(cGameComm, mWaitingMessage, 0x084);
CHECK_OFFSET(cGameComm, mRemoteExitGame, 0x088);
CHECK_OFFSET(cGameComm, mRemoteEndGame, 0x08C);
CHECK_OFFSET(cGameComm, mAtFinishLine, 0x090);
CHECK_OFFSET(cGameComm, mLinkTimeout, 0x094);
CHECK_OFFSET(cGameComm, mMessageDisplay, 0x098);
CHECK_OFFSET(cGameComm, mPingResult, 0x09C);
CHECK_OFFSET(cGameComm, mLocalFinishedRace, 0x0A0);
CHECK_OFFSET(cGameComm, mRemoteFinishedRace, 0x0A4);
CHECK_OFFSET(cGameComm, mStallThreshold, 0x0A8);
CHECK_OFFSET(cGameComm, mLoading, 0x0AC);
CHECK_OFFSET(cGameComm, mStallTimer, 0x0B0);
CHECK_OFFSET(cGameComm, mStallCount, 0x0B4);
CHECK_OFFSET(cGameComm, mPingCount, 0x0B8);
CHECK_OFFSET(cGameComm, mPingDelay, 0x0BC);
CHECK_OFFSET(cGameComm, mPingAddr, 0x0C0);
CHECK_OFFSET(cGameComm, mPingID, 0x0C4);
CHECK_OFFSET(cGameComm, mServerSettings, 0x0D4);
CHECK_OFFSET(cGameComm, mClientBroadBand, 0x100);
CHECK_OFFSET(cGameComm, mClientSignature, 0x104);
CHECK_OFFSET(cGameComm, mNetLatency, 0x114);
CHECK_OFFSET(cGameComm, mLatency, 0x11C);
CHECK_OFFSET(cGameComm, mNumSamples, 0x120);
CHECK_OFFSET(cGameComm, mAsyncReadData, 0x124);
CHECK_OFFSET(cGameComm, mAsyncReadSize, 0x128);
CHECK_OFFSET(cGameComm, mAsyncWriteData, 0x12C);
CHECK_OFFSET(cGameComm, mAsyncWriteSize, 0x130);
CHECK_OFFSET(cGameComm, mPacketCache, 0x134);
CHECK_OFFSET(cGameComm, mPingTimer, 0x138);
CHECK_OFFSET(cGameComm, mPingReceived, 0x13C);
CHECK_OFFSET(cGameComm, mPingSocket, 0x140);
CHECK_OFFSET(cGameComm, mReadQueue, 0x144);
CHECK_OFFSET(cGameComm, mWriteQueue, 0x364);
CHECK_OFFSET(cGameComm, mBEBuffer, 0x3EC);
CHECK_OFFSET(cGameComm, mBEBufferSize, 0x3F0);
CHECK_OFFSET(cGameComm, mSyncWaitComplete, 0x3F4);
CHECK_OFFSET(cGameComm, mUnPauseFlag, 0x3F8);
CHECK_OFFSET(cGameComm, mUnPauseTimer, 0x3FC);
CHECK_OFFSET(cGameComm, mCheckSumSent, 0x400);
CHECK_OFFSET(cGameComm, mCheckSumReceived, 0x404);
CHECK_OFFSET(cGameComm, mAITick, 0x408);
CHECK_OFFSET(cGameComm, mCheckSumBuffer, 0x410);
CHECK_SIZE(cGameComm, 0x5F0);

// The one game comm (On Tour: cGameComm::mGameComm), created by cGameComm_construct.
extern cGameComm* cGameComm_mGameComm;

#endif /* ONLINE_GAMECOMM_H */
