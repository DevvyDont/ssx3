#ifndef SOUND_PATHFINDERPLAT_H
#define SOUND_PATHFINDERPLAT_H

// The PS2 glue of EA's "Pathfinder" interactive-music library (retail unit sound/bankmonitor, pathxSND_getPlayer ..
// pathxSND_stop): the library (in ealib/seg_2B4578, func_003D25F0 ..) opens up to 24 players and drives them through
// these functions by player index. A player is either a stream player (an EA SNDSTRM stream that queues sections of a
// file) or a bank player (plays patches from loaded sound banks with SNDBANK_play).
//   D_004D3E98[24]  the players (tPathPlayer*; 0 = unused). pathxSND_openPlayer claims a free entry (pathxSND_findFreePlayer) and
//                   points it at the caller's memory, pathxSND_closePlayer / pathxSND_forgetPlayer clear it; pathxSND_init clears all.
//   D_004D3EF8[24]  the bank records (tPathBank) of the bank players: which player and bank index a loaded bank
//                   belongs to. pathxSND_findBank searches them, pathxSND_addBank fills one, pathxSND_removeBank / pathxSND_clearBank
//                   clear one.
// The type and member names are ours, derived from what the code does (no EA symbols or assert strings survive for
// these records); the ssx3-rust recomp calls the player a "track" (cf. the ssx3-rust recomp's notes). Tags: [R] the
// initialisers (pathxSND_initPlayer for a player, pathxSND_openPlayer when it opens one, pathxSND_clearBank / pathxSND_init for a bank
// record); [U] how retail code uses the field.

#include "layout.h"
#include "ealib/snd.h"

// One queued request of a player (12 bytes): a section queued on the stream, or the sound a bank player started.
// [R] pathxSND_openPlayer places count of them right after the player record (tPathPlayer::mRequests) and sets each
// mHandle to -1. pathxSND_play shifts the table down one and fills entry 0 with the newest request.
struct tPathRequest {
    int mLength;    // 0x0 [U] pathxSND_play stores its requestLength argument; pathxSND_refreshAll adds it to the player's
                    //     totals (and copies it to mPosition) when the request has finished
    int mHandle;    // 0x4 [R] pathxSND_openPlayer: -1 (free); [U] pathxSND_play stores SNDBANK_play's / the queue call's
                    //     handle; pathxSND_refreshAll queries SNDSTRM_requeststatus / SNDtimeremaining on it and sets it to
                    //     -1 when the request is gone; pathxSND_releaseHold / pathxSND_fadeVolume / pathxSND_setVolume / pathxSND_setPaused /
                    //     pathxSND_stop pass entry 0's to the driver; pathxSND_getHandlePosition searches for it
    int mPosition;  // 0x8 [U] pathxSND_refreshAll stores the request's time to end (or its length once finished, or a bank
                    //     sound's remaining ms); pathxSND_getRequestPosition / pathxSND_getHandlePosition return it
};
CHECK_OFFSET(tPathRequest, mLength, 0x0);
CHECK_OFFSET(tPathRequest, mHandle, 0x4);
CHECK_OFFSET(tPathRequest, mPosition, 0x8);
CHECK_SIZE(tPathRequest, 0xC);

// One Pathfinder player (0x70), at the start of the memory the library gives pathxSND_openPlayer (pathxSND_getPlayerSize bytes: this
// record, the request table, then a stream's SNDSTRM memory, 16-aligned). [R] pathxSND_initPlayer zeroes 0x70 bytes and sets
// the -1 / 1 defaults below; pathxSND_openPlayer fills the rest.
struct tPathPlayer {
    int mHandle;                // 0x00 [R] pathxSND_initPlayer: -1; [U] pathxSND_openPlayer stores SNDSTRM_create's stream
                                //      handle; pathxSND_play stores the bank handle it played from; the SNDSTRM_* /
                                //      status calls take it; pathxSND_getBankHandle / pathxSND_getStreamHandle return it
    int mPatch;                 // 0x04 [R] pathxSND_initPlayer: -1; [U] pathxSND_play stores the patch it passes to
                                //      SNDBANK_play; pathxSND_refreshAll passes it to SNDBANK_readPatchHeader when >= 0
    int mFadeTarget;            // 0x08 [R] pathxSND_initPlayer: -1; [U] pathxSND_fadeVolume stores the fade's target volume;
                                //      pathxSND_setVolume compares it with mVolume and resets it to -1
    int mVolume;                // 0x0C [U] pathxSND_refreshAll stores the driver's volume (SND_getVolume / SNDSTRM_getVolume);
                                //      pathxSND_setVolume compares and stores the volume it sets; pathxSND_getVolume returns it
    int mFxLevel;               // 0x10 [R] pathxSND_openPlayer: 0; [U] pathxSND_setFxLevel stores the effect send level;
                                //      pathxSND_getFxLevel returns it
    int mDryLevel;              // 0x14 [R] pathxSND_openPlayer: 0x7F; [U] pathxSND_setDryLevel stores the dry level;
                                //      pathxSND_getDryLevel returns it
    int mSavedPitchMult;        // 0x18 [U] pathxSND_setPaused saves mPlayOpts.pitchmult here when it pauses (pitch 0) and
                                //      applies it again when it resumes
    char mName[12];             // 0x1C [R] pathxSND_openPlayer: sprintf("%.*s", 11, name)
    int mPreloadData;           // 0x28 [U] pathxSND_setPreload stores it; pathxSND_play queues file offsets below
                                //      mPreloadSize from it (SNDSTRM_queueMemory) when set
    unsigned int mPreloadSize;  // 0x2C [U] pathxSND_setPreload stores it; pathxSND_play compares the file offset with it
                                //      (unsigned)
    SNDPLAYOPTS mPlayOpts;      // 0x30 [R] pathxSND_openPlayer copies the caller's options (or SNDplaysetdef's defaults),
                                //      clears fxlevel0 and passes them to SNDSTRM_create; [U] pathxSND_play stores vol
                                //      and passes them to SNDBANK_play; pathxSND_setPaused reads pitchmult
    int mElapsedMs;             // 0x48 [U] pathxSND_refreshAll stores the elapsed ms (bank: sample position / rate; stream:
                                //      the playing request's currenttime); pathxSND_getStatus returns it
    int mRemainingMs;           // 0x4C [U] pathxSND_refreshAll stores the remaining ms (bank: SNDtimeremaining x 10;
                                //      stream: timetoend); pathxSND_getStatus returns it
    int mBuffered;              // 0x50 [U] pathxSND_refreshAll: a bank player's request-0 mLength, a stream's sum of its
                                //      requests' timebuffered; pathxSND_getPlayed returns it
    int mPositionTotal;         // 0x54 [U] pathxSND_refreshAll (stream) adds the finished requests' mLength and the
                                //      playing ones' time to end; pathxSND_getRequestPosition returns it for sub < 0
    int mBusy;                  // 0x58 [U] pathxSND_refreshAll sets it to 1, then clears it when SNDSTRM_status's count
                                //      reaches mRequestCount, a request has finished, or a playing request has less
                                //      buffered than left to play; pathxSND_isBusy returns it
    int mCurrentRequest;        // 0x5C [R] pathxSND_initPlayer: -1; [U] pathxSND_refreshAll stores the handle of the request it
                                //      last saw playing; pathxSND_getStatus returns it when mStatus is 0
    int mStatus;                // 0x60 [U] pathxSND_refreshAll stores SNDBANK_readPatchHeader's / SNDSTRM_status's result (-8 when it
                                //      made no call; 0 on a reset); pathxSND_getStatus returns it when nonzero
    signed char mIsBankPlayer;  // 0x64 [R] pathxSND_openPlayer: 1 when maxChunks is 0; [U] every driver call picks the SND*
                                //      (bank) or SNDSTRM_* (stream) variant on it
    signed char mDirty;         // 0x65 [R] pathxSND_initPlayer: 1; [U] pathxSND_play / pathxSND_markDirty / pathxSND_setPaused /
                                //      pathxSND_stop set it; pathxSND_refreshAll clears it; the queries refresh while set
    signed char mRequestCount;  // 0x66 [R] pathxSND_initPlayer: -1; pathxSND_openPlayer: the request count
    char unk_67[0x5];           // 0x67
    tPathRequest* mRequests;    // 0x6C [R] pathxSND_openPlayer points it right after this record (mRequestCount entries)
};
CHECK_OFFSET(tPathPlayer, mHandle, 0x00);
CHECK_OFFSET(tPathPlayer, mPatch, 0x04);
CHECK_OFFSET(tPathPlayer, mFadeTarget, 0x08);
CHECK_OFFSET(tPathPlayer, mVolume, 0x0C);
CHECK_OFFSET(tPathPlayer, mFxLevel, 0x10);
CHECK_OFFSET(tPathPlayer, mDryLevel, 0x14);
CHECK_OFFSET(tPathPlayer, mSavedPitchMult, 0x18);
CHECK_OFFSET(tPathPlayer, mName, 0x1C);
CHECK_OFFSET(tPathPlayer, mPreloadData, 0x28);
CHECK_OFFSET(tPathPlayer, mPreloadSize, 0x2C);
CHECK_OFFSET(tPathPlayer, mPlayOpts, 0x30);
CHECK_OFFSET(tPathPlayer, mElapsedMs, 0x48);
CHECK_OFFSET(tPathPlayer, mRemainingMs, 0x4C);
CHECK_OFFSET(tPathPlayer, mBuffered, 0x50);
CHECK_OFFSET(tPathPlayer, mPositionTotal, 0x54);
CHECK_OFFSET(tPathPlayer, mBusy, 0x58);
CHECK_OFFSET(tPathPlayer, mCurrentRequest, 0x5C);
CHECK_OFFSET(tPathPlayer, mStatus, 0x60);
CHECK_OFFSET(tPathPlayer, mIsBankPlayer, 0x64);
CHECK_OFFSET(tPathPlayer, mDirty, 0x65);
CHECK_OFFSET(tPathPlayer, mRequestCount, 0x66);
CHECK_OFFSET(tPathPlayer, unk_67, 0x67);
CHECK_OFFSET(tPathPlayer, mRequests, 0x6C);
CHECK_SIZE(tPathPlayer, 0x70);

// One bank record (0x28): a sound bank loaded for a bank player. [R] pathxSND_clearBank zeroes 40 bytes and sets the first
// three words to -1; pathxSND_init sets them to -1 and clears +0x18 (stride 0x28); [U] pathxSND_findBank searches the 24
// records by mPlayer and mBankIndex.
struct tPathBank {
    int mBankIndex;     // 0x00 [R] -1; [U] pathxSND_addBank stores the bank index within the player; pathxSND_findBank
                        //      matches it; pathxSND_removeBank resets it to -1
    int mHandle;        // 0x04 [R] -1; [U] pathxSND_addBank stores the loaded bank handle; pathxSND_play plays from it;
                        //      pathxSND_removeBank tests it (>= 0) and resets it to -1
    int mPlayer;        // 0x08 [R] -1 (free); [U] pathxSND_addBank stores the player index; pathxSND_findBank matches it
                        //      (< 0: free); pathxSND_removeBank resets it to -1
    int unk_0C;         // 0x0C
    int unk_10;         // 0x10
    void* mMemory;      // 0x14 [U] pathxSND_closePlayer passes it to the free hook D_004A3794 when set; pathxSND_removeBank clears
                        //      it (no store of a nonzero value found)
    int unk_18;         // 0x18 [R] pathxSND_init: 0
    int mUsed;          // 0x1C [U] pathxSND_addBank stores 1; pathxSND_removeBank stores 0
    int unk_20;         // 0x20
    int mLastPlayTime;  // 0x24 [U] pathxSND_play stores the time (ms, pathxSND_getTimeMs) it played from the bank;
                        //      pathxSND_evictOldestBank evicts the bank with the smallest; pathxSND_removeBank resets it to -1
};
CHECK_OFFSET(tPathBank, mBankIndex, 0x00);
CHECK_OFFSET(tPathBank, mHandle, 0x04);
CHECK_OFFSET(tPathBank, mPlayer, 0x08);
CHECK_OFFSET(tPathBank, unk_0C, 0x0C);
CHECK_OFFSET(tPathBank, unk_10, 0x10);
CHECK_OFFSET(tPathBank, mMemory, 0x14);
CHECK_OFFSET(tPathBank, unk_18, 0x18);
CHECK_OFFSET(tPathBank, mUsed, 0x1C);
CHECK_OFFSET(tPathBank, unk_20, 0x20);
CHECK_OFFSET(tPathBank, mLastPlayTime, 0x24);
CHECK_SIZE(tPathBank, 0x28);

extern tPathPlayer* D_004D3E98[24];
extern tPathBank D_004D3EF8[24];

#endif /* SOUND_PATHFINDERPLAT_H */
