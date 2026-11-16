#ifndef ONLINE_GAMECOMM_H
#define ONLINE_GAMECOMM_H

// cGameComm: the in-race link of an online game (two humans, peer to peer over one cCommSystem channel). It runs the
// pre-race handshake (settings, BE sync data, latency: the tGameComm states below), carries the input samples that
// cInputSync exchanges every frame (lockstep with a shared AI random seed), pause/unplug/exit commands, chat, and a
// once-a-second checksum used to detect desyncs; it also tracks the comm errors that the results report (REPT) sends.
// EA's source: d:/ssx/code/source/online/gamecomm.cpp and online/gamecomm.h (the preview build's assert strings).
//
// One instance: cGameComm_construct(channel, server) does `cMemMan_alloc(0x5F0, "cGameComm", ...)` + the constructor
// func_00256C78 into cGameComm::mGameComm (retail cGameComm_mGameComm); func_00256C50 (destroy) runs the destructor
// func_00256EE0. Size 0x5F0 [R]. No base class and no virtuals (the constructor stores no vtable; On Tour has none).
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] retail's constructor func_00256C78 (initial values below) and the allocation size;
//   [U] how retail code uses the field: the class's own methods (func_00256C78..func_00258F20 in seg_156860) and the
//       ~50 users of cGameComm_mGameComm (ovstatepause, ovtemplatepausemenu, gamestate's cInputSync, ai, gameload, ...). The
//       results reporter func_0025FD50 (tagsunk) sends many fields under DirtySock tags, quoted as "TAG";
//   [O] SSX On Tour's DWARF (class cGameComm, size 0x101C): same member order where both games have the member. On
//       Tour is a 4-player rewrite (per-player [4] arrays, 1-byte bools, mPlayerID/mNumPlayers, disable/rejoin), so a
//       name is used only where the retail use matches; On Tour's [4] arrays are [2] here (index 0 = this machine,
//       1 = the peer).
// Retail bools are 4-byte ints. Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"

// tGameComm: the handshake/race state machine, cGameComm::mState (On Tour enum tGameComm, same values; retail's
// update func_002570D8 walks them on the server and, with each Send/Recv pair swapped, on the client).
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
    kGameComm_GameOver = 15        // gameEnded (func_00258A48)
};

// tGameCommPacket: one queued packet, 0xF0 bytes ([R] func_00258188: cMemMan_alloc(0xF0, "tGameCommPacket")).
// On Tour: tGameCommPacket (0xF0) = its 4-byte base tGameCommHeader {mType, mPlayerID, mSize} + mData[236]; written
// flat here.
struct tGameCommPacket {
    unsigned char mType;         // 0x00 [O] tGameCommHeader::mType; [U] the queue it goes to (mReadQueue[mType]):
                                 //      0 input samples (cInputSync), 1 chat (sendChat func_00258C50, getQueuedChat
                                 //      func_00258160), 2 BE sync data (asyncRead/asyncWrite), 3 commands (tGameCmdPacket)
    unsigned char mPlayerID;     // 0x01 [O]; [U] writePacket (func_00257E98) stores 0
    unsigned short mSize;        // 0x02 [O]; [U] the packet size including this header (writePacket: len + 4)
    unsigned char mData[236];    // 0x04 [O]; [U] payload (writePacket copies len bytes)
};

// tGameCmdPacket: the 0xC-byte payload of a type-3 (command) packet (On Tour tGameCmdPacket, same layout). Retail
// commands: 0 pause and 1 unpause (sendPauseCmd func_00258790), 2 controller unplugged (sendUnplugCmd func_00258C00),
// 3 sync-wait done (syncWait func_002586B0), 4 checksum (updateSyncPacket func_00258F20), 5 exit game (sendExitGame
// func_00258BC8); handled by updateCmdQueue func_00257F98.
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

// tServerSettings: what the server sends in kGameComm_SendSettings (0x2C bytes, func_002570D8). On Tour's version
// (0x1C) is AIRandSeed[6] + NumPlayers; retail has the server's link info instead of NumPlayers.
struct tServerSettings {
    unsigned int AIRandSeed[6];  // 0x00 [O]; [U] cGameComm_syncGame saves the AI random state into it
                                 //      (func_003178E0, bx/bxrandom) on the server and restores it (func_00317908) once synced
    int unk_18;                  // 0x18 [U] the server's cCommSystem::mBroadBand (syncGame)
    char unk_1C[0x10];           // 0x1C [U] the server's build signature "NA_R_F004" (strncpy 0xF in syncGame);
                                 //      compared with the client's (9 chars) after the handshake
};

struct cGameComm {
    int mConnected;              // 0x000 [O]; [R] 1; [U] cleared by localDisconnected (func_00257610), displayError
                                 //       (func_002578F8) and the remote exit command; senders require it
    int mError;                  // 0x004 [O]; [R] 0; [U] displayError sets 1; nearly every method returns early on it
    int mDisplayError;           // 0x008 [O] (int); [U] displayError sets 1; updateDisplayError (func_002579C8) counts it
                                 //       down and raises overlay 0x27
    int mDesync;                 // 0x00C [O]; [U] updateSyncPacket sets 1 on a checksum mismatch; reported as "DESYNC"
    int mLocalDisconnected;      // 0x010 [O]; [U] localDisconnected sets 1; "DIDQUIT0/1"; bit 2 of mFlagsOnCommError
    int mRemoteDisconnected;     // 0x014 [O]; [U] the exit-game command (5) sets 1; "DIDQUIT0/1"; bit 3 of the flags
    int mCommErrorWasSignificant; // 0x018 [O]; [U] setCommError (func_00257678); reported as "SCOMERR"
    int mStateOnCommError;       // 0x01C [O] tGameComm; [U] setCommError copies mState; "CERRSTAT"
    unsigned int mFlagsOnCommError; // 0x020 [O]; [U] setCommError packs 11 flags (mLocalFinishedRace = bit 0 ...
                                 //       mLoading = bit 10); "CERRFLGS"
    int mCheating[2];            // 0x024 [O] (On Tour [4]); [U] checkErrors (func_002577C8) sets 2 (6+ pauses) or 3
                                 //       (3+ unplugs) for this machine / the peer; "CHEAT0/1"
    int unk_02C;                 // 0x02C [R] 0; [U] set to 1 when the link times out (unk_094 runs out in checkErrors,
                                 //       unk_05C reaches 1800 in func_00257A80); never read in retail
    int mCommError;              // 0x030 [O]; [U] setCommError sets 1 (once: it guards the other fields); "COMMERR"
    int unk_034;                 // 0x034 [R] the time at construction; [U] gameStartRace (func_00258988) stores the race
                                 //       start time (SSXApp +0x18 / +0x10); the reporter subtracts it from unk_038/unk_03C
    int unk_038;                 // 0x038 [U] this machine's end time (localDisconnected, gameEnded func_00258A48)
    int unk_03C;                 // 0x03C [U] the peer's end time (exit-game command, gameEnded)
    int mSyncInput;              // 0x040 [O] (just before mServer in both games); [R] 1; [U] cInputSync (gamestate
                                 //       func_002373B8 / func_00237A98) exchanges pads only while it is set;
                                 //       updateSyncPacket requires it; displayError, gameEnded and the out-of-line
                                 //       stopSyncInput func_00244880 (main/ssxappdtor: sw $0, 0x40) clear it
    int mServer;                 // 0x044 [O]; [R] the constructor's second argument; [U] update/syncGame take the
                                 //       server side of the handshake when set
    int mAlone;                  // 0x048 [O] (On Tour's next member after mServer that retail has); [U] setFinishRaceAlone
                                 //       (func_00258950, from ai func_0012B340) sets 1; ai func_0012BB20 and the timeouts test it
    int mPauseCount[2];          // 0x04C [O] (On Tour [4]); [U] [0] counted by sendPauseCmd(true), [1] by the pause
                                 //       command; 6+ means cheating 2
    int unk_054;                 // 0x054 [U] this machine's controller-unplug count (sendUnplugCmd); 3+ means cheating 3
    int unk_058;                 // 0x058 [U] the peer's unplug count (command 2)
    int unk_05C;                 // 0x05C [U] frames without a packet while the peer isn't paused (func_00257A80); 1800 ends
                                 //       the link; cleared when an input packet arrives (readPacket channel 0)
    int unk_060;                 // 0x060 [R] 30; [U] frames without a packet (func_00257A80); at 60 mWaitingMessage is set
                                 //       (On Tour has mWaitingTimer[4] in this area)
    int mOverlayTimeout;         // 0x064 [O]; [R] -1; [U] the pause overlays start it (0x4B0, func_001FD0D8; 0x8CA0 in
                                 //       ovtemplatedialog), stop it (-1) and test == 0; counted down by func_00257750
    int mPauseTimeout;           // 0x068 [O]; [R] -1; [U] counted down by func_00257750, tested and reset to -1 by the pause
                                 //       menu; never armed in retail
    int unk_06C;                 // 0x06C [R] the constructor's first argument: the cCommSystem channel index passed to
                                 //       its readPacket/writePacket/getLatency/getError (On Tour has no channel member)
    int mState;                  // 0x070 [O] tGameComm; [U] setState (func_00257038) stores it; cInputSync waits for >= 14
    int mPauseMenu[2];           // 0x074 [O] (On Tour [4]); [U] [0] this machine paused (sendPauseCmd), [1] the peer
                                 //       (commands 0/1)
    int mGameStartGate;          // 0x07C [O]; [U] gameStartGate (func_00258968, from cGFGateState_gainFocus) sets 1
    int mRaceInProgress;         // 0x080 [O]; [U] gameStartRace sets 1, gameEnded clears it
    int mWaitingMessage;         // 0x084 [O]; [U] set when the peer goes quiet (unk_060 == 60); updateMessageDisplay
                                 //       (func_00258CB0) then shows "kT_OVRCMNWaitPlayer"
    int mRemoteExitGame;         // 0x088 [O]; [U] the exit-game command sets 1
    int mRemoteEndGame;          // 0x08C [O]; [U] the exit-game command sets 1 while the race still runs; checkErrors
                                 //       then calls displayError
    int unk_090;                 // 0x090 [R] 0; [U] festateloadscreen func_00233C50 sets 1 right after stopSyncInput
                                 //       (leaving the race for a load state); read only into bit 9 of mFlagsOnCommError
    int unk_094;                 // 0x094 [U] link timeout in frames: func_00258870 arms it (1800/3600/3660), checkErrors
                                 //       counts it down (0 = timed out, unk_02C)
    int mMessageDisplay;         // 0x098 [O]; [U] updateMessageDisplay's popup state (0 none, 1 peer paused, 2 waiting)
    int unk_09C;                 // 0x09C [R] 1; [U] ping result: cleared by the online-error overlay (func_001FAF10) when
                                 //       the ping times out; "PINGRSLT"
    int mLocalFinishedRace;      // 0x0A0 [O]; [U] "FINISH0/1"; bit 0 of the flags
    int mRemoteFinishedRace;     // 0x0A4 [O]; [U] "FINISH0/1"; bit 1
    int mStallThreshold;         // 0x0A8 [O] (On Tour [4], same place: before mLoading); [U] frames since the last input
                                 //       packet; at 10 a stall is counted (mStallCount) and mStallTimer grows
    int mLoading;                // 0x0AC [O]; [R] 1; [U] syncWait clears it; updateDisplayError waits while it is set
    float mStallTimer;           // 0x0B0 [O] (On Tour [4]); [U] +1.0 per stalled frame, -0.15 per update; 3600 = error
    int mStallCount;             // 0x0B4 [O] (On Tour [4]); [U] "STALLCNT"
    int unk_0B8;                 // 0x0B8 [U] ping server (func_002570D8 / func_002583A8): number of pings sent (max 4)
    int unk_0BC;                 // 0x0BC [U] ping: frames until the next ping
    int unk_0C0;                 // 0x0C0 [U] ping: the address being pinged (func_002583A8's argument)
    int unk_0C4[4];              // 0x0C4 [U] ping: the id of each ping sent (matched against replies); [R] cleared
    tServerSettings mServerSettings; // 0x0D4 [O] (On Tour 0xFC, 0x1C bytes); [U] sent by the server (0x2C bytes)
    int unk_100;                 // 0x100 [U] the client's cCommSystem::mBroadBand (sent by the client with unk_104)
    char unk_104[0x10];          // 0x104 [U] the client's build signature "NA_R_F004"
    float mNetLatency[2];        // 0x114 [O] (On Tour [4]); [R] the default frame time (SSXApp +0x14); [U] the latency
                                 //       measured on the server [0] and on the client [1] (states 9/11)
    float mLatency;              // 0x11C [O]; [U] calcLatency (func_00258330): max of mNetLatency, at least one frame;
                                 //       syncGame copies it into tBENetworkDB::fLatency
    int mNumSamples;             // 0x120 [O]; [R] 3; [U] syncGame: 2 when both sides are broadband; cGame_load passes it
                                 //       to cInputSync as its samples per packet; getLatency (func_00258360) adds frames
    char* mAsyncReadData;        // 0x124 [O] (On Tour [4]); [U] asyncRead (func_002581E8); asyncReadComplete tests 0
    int mAsyncReadSize;          // 0x128 [O]; [U] asyncReadUpdate (func_002581F8) reads in 0xEC-byte pieces
    char* mAsyncWriteData;       // 0x12C [O]; [U] asyncWrite (func_00258290); asyncWriteComplete tests 0
    int mAsyncWriteSize;         // 0x130 [O]; [U] asyncWriteUpdate (func_002582A0)
    tGameCommPacket* mPacketCache; // 0x134 [O]; [U] the packet being received (updateReadQueue func_00257CD8); the
                                 //       destructor frees it
    int unk_138;                 // 0x138 [U] ping: frames left (func_002583A8 sets seconds * 60); the online-error overlay
                                 //       tests it
    int unk_13C;                 // 0x13C [U] ping: a reply arrived
    void* unk_140;               // 0x140 [R] func_003E7340() (a DirtySock handle; the destructor frees it with
                                 //       func_003E73E0); [U] the ping server's socket ("SSXPingServer")
    tGameCommQueue mReadQueue[4]; // 0x144 [O] (On Tour [4][4]); [U] one per packet type (tGameCommPacket::mType)
    tGameCommQueue mWriteQueue;  // 0x364 [O] (On Tour [4]); [U] packets waiting for the channel (updateWriteQueue)
    char* mBEBuffer;             // 0x3EC [O] (On Tour [4]); [U] syncGame allocates it ("bebuffer", mBEBufferSize) for
                                 //       cBENetworkInterface getSyncData/setSyncData
    int mBEBufferSize;           // 0x3F0 [O]; [R] cBENetworkInterface::getSyncDataSize() (0xC44)
    int mSyncWaitComplete;       // 0x3F4 [O]; [U] command 3 sets it; syncWait loops until it is set
    int mUnPauseFlag;            // 0x3F8 [O]; [U] set by a pause during the race, cleared by an unpause; cInputSync's
                                 //       func_00237950 holds the race while it is set
    int mUnPauseTimer;           // 0x3FC [O]; [U] setUnPauseTimer (func_002588A8) sets 180; counted down by update; the
                                 //       HUD (cOVStateHUD1P_onRender2D) shows it
    int mCheckSumSent;           // 0x400 [O]; [U] updateSyncPacket
    int mCheckSumReceived;       // 0x404 [O] (On Tour [4]); [U] command 4 sets it
    int mAITick[2];              // 0x408 [O] (On Tour [4]); [R] -1; [U] the last checksummed frame here [0] / from the
                                 //       peer [1]
    unsigned int mCheckSumBuffer[2][60]; // 0x410 [O] (On Tour [60][4]: frame-major); [U] [0] this machine's checksums,
                                 //       [1] the peer's, at [(frame / 60) % 60]; [R] both cleared (MEM_fill 0xF0)
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
CHECK_OFFSET(tServerSettings, unk_18, 0x18);
CHECK_OFFSET(tServerSettings, unk_1C, 0x1C);

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
CHECK_OFFSET(cGameComm, unk_02C, 0x02C);
CHECK_OFFSET(cGameComm, mCommError, 0x030);
CHECK_OFFSET(cGameComm, unk_034, 0x034);
CHECK_OFFSET(cGameComm, unk_038, 0x038);
CHECK_OFFSET(cGameComm, unk_03C, 0x03C);
CHECK_OFFSET(cGameComm, mSyncInput, 0x040);
CHECK_OFFSET(cGameComm, mServer, 0x044);
CHECK_OFFSET(cGameComm, mAlone, 0x048);
CHECK_OFFSET(cGameComm, mPauseCount, 0x04C);
CHECK_OFFSET(cGameComm, unk_054, 0x054);
CHECK_OFFSET(cGameComm, unk_058, 0x058);
CHECK_OFFSET(cGameComm, unk_05C, 0x05C);
CHECK_OFFSET(cGameComm, unk_060, 0x060);
CHECK_OFFSET(cGameComm, mOverlayTimeout, 0x064);
CHECK_OFFSET(cGameComm, mPauseTimeout, 0x068);
CHECK_OFFSET(cGameComm, unk_06C, 0x06C);
CHECK_OFFSET(cGameComm, mState, 0x070);
CHECK_OFFSET(cGameComm, mPauseMenu, 0x074);
CHECK_OFFSET(cGameComm, mGameStartGate, 0x07C);
CHECK_OFFSET(cGameComm, mRaceInProgress, 0x080);
CHECK_OFFSET(cGameComm, mWaitingMessage, 0x084);
CHECK_OFFSET(cGameComm, mRemoteExitGame, 0x088);
CHECK_OFFSET(cGameComm, mRemoteEndGame, 0x08C);
CHECK_OFFSET(cGameComm, unk_090, 0x090);
CHECK_OFFSET(cGameComm, unk_094, 0x094);
CHECK_OFFSET(cGameComm, mMessageDisplay, 0x098);
CHECK_OFFSET(cGameComm, unk_09C, 0x09C);
CHECK_OFFSET(cGameComm, mLocalFinishedRace, 0x0A0);
CHECK_OFFSET(cGameComm, mRemoteFinishedRace, 0x0A4);
CHECK_OFFSET(cGameComm, mStallThreshold, 0x0A8);
CHECK_OFFSET(cGameComm, mLoading, 0x0AC);
CHECK_OFFSET(cGameComm, mStallTimer, 0x0B0);
CHECK_OFFSET(cGameComm, mStallCount, 0x0B4);
CHECK_OFFSET(cGameComm, unk_0B8, 0x0B8);
CHECK_OFFSET(cGameComm, unk_0BC, 0x0BC);
CHECK_OFFSET(cGameComm, unk_0C0, 0x0C0);
CHECK_OFFSET(cGameComm, unk_0C4, 0x0C4);
CHECK_OFFSET(cGameComm, mServerSettings, 0x0D4);
CHECK_OFFSET(cGameComm, unk_100, 0x100);
CHECK_OFFSET(cGameComm, unk_104, 0x104);
CHECK_OFFSET(cGameComm, mNetLatency, 0x114);
CHECK_OFFSET(cGameComm, mLatency, 0x11C);
CHECK_OFFSET(cGameComm, mNumSamples, 0x120);
CHECK_OFFSET(cGameComm, mAsyncReadData, 0x124);
CHECK_OFFSET(cGameComm, mAsyncReadSize, 0x128);
CHECK_OFFSET(cGameComm, mAsyncWriteData, 0x12C);
CHECK_OFFSET(cGameComm, mAsyncWriteSize, 0x130);
CHECK_OFFSET(cGameComm, mPacketCache, 0x134);
CHECK_OFFSET(cGameComm, unk_138, 0x138);
CHECK_OFFSET(cGameComm, unk_13C, 0x13C);
CHECK_OFFSET(cGameComm, unk_140, 0x140);
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

#endif /* ONLINE_GAMECOMM_H */
