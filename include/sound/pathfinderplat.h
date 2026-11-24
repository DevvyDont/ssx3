#ifndef SOUND_PATHFINDERPLAT_H
#define SOUND_PATHFINDERPLAT_H

// The PS2 glue of EA's "Pathfinder" interactive-music library (retail unit sound/bankmonitor, func_002ADFB0 ..
// func_002AF838): the library (in ealib/seg_2B4578, func_003D25F0 ..) opens up to 24 players and drives them through
// these functions by player index. A player is either a stream player (an EA SNDSTRM stream that queues sections of a
// file) or a bank player (plays patches from loaded sound banks with SNDBANK_play).
//   D_004D3E98[24]  the players (tPathPlayer*; 0 = unused). func_002AE4C0 claims a free entry (func_002AE1B8) and
//                   points it at the caller's memory, func_002AE690 / func_002AE188 clear it; func_002AE048 clears all.
//   D_004D3EF8[24]  the bank records (tPathBank) of the bank players: which player and bank index a loaded bank
//                   belongs to. func_002AE230 searches them, func_002AE298 fills one, func_002AE328 / func_002AE1F0
//                   clear one.
// The type and member names are ours, derived from what the code does (no EA symbols or assert strings survive for
// these records); the ssx3-rust recomp calls the player a "track" (cf. the ssx3-rust recomp's notes). Tags: [R] the
// initialisers (func_002AE138 for a player, func_002AE4C0 when it opens one, func_002AE1F0 / func_002AE048 for a bank
// record); [U] how retail code uses the field.

#include "layout.h"
#include "ealib/snd.h"

// One queued request of a player (12 bytes): a section queued on the stream, or the sound a bank player started.
// [R] func_002AE4C0 places count of them right after the player record (tPathPlayer::mRequests) and sets each
// mHandle to -1. func_002AE7A8 shifts the table down one and fills entry 0 with the newest request.
struct tPathRequest {
    int mLength;    // 0x0 [U] func_002AE7A8 stores its requestLength argument; func_002AEFA8 adds it to the player's
                    //     totals (and copies it to mPosition) when the request has finished
    int mHandle;    // 0x4 [R] func_002AE4C0: -1 (free); [U] func_002AE7A8 stores SNDBANK_play's / the queue call's
                    //     handle; func_002AEFA8 queries SNDSTRM_requeststatus / SNDtimeremaining on it and sets it to
                    //     -1 when the request is gone; func_002AE9F8 / func_002AF608 / func_002AF6C0 / func_002AF7A0 /
                    //     func_002AF838 pass entry 0's to the driver; func_002AEB40 searches for it
    int mPosition;  // 0x8 [U] func_002AEFA8 stores the request's time to end (or its length once finished, or a bank
                    //     sound's remaining ms); func_002AEAA8 / func_002AEB40 return it
};
CHECK_OFFSET(tPathRequest, mLength, 0x0);
CHECK_OFFSET(tPathRequest, mHandle, 0x4);
CHECK_OFFSET(tPathRequest, mPosition, 0x8);
CHECK_SIZE(tPathRequest, 0xC);

// One Pathfinder player (0x70), at the start of the memory the library gives func_002AE4C0 (func_002AE478 bytes: this
// record, the request table, then a stream's SNDSTRM memory, 16-aligned). [R] func_002AE138 zeroes 0x70 bytes and sets
// the -1 / 1 defaults below; func_002AE4C0 fills the rest.
struct tPathPlayer {
    int mHandle;                // 0x00 [R] func_002AE138: -1; [U] func_002AE4C0 stores SNDSTRM_create's stream
                                //      handle; func_002AE7A8 stores the bank handle it played from; the SNDSTRM_* /
                                //      status calls take it; func_002AF3B8 / func_002AF3F0 return it
    int mPatch;                 // 0x04 [R] func_002AE138: -1; [U] func_002AE7A8 stores the patch it passes to
                                //      SNDBANK_play; func_002AEFA8 passes it to func_003B6670 when >= 0
    int mFadeTarget;            // 0x08 [R] func_002AE138: -1; [U] func_002AF608 stores the fade's target volume;
                                //      func_002AF6C0 compares it with mVolume and resets it to -1
    int mVolume;                // 0x0C [U] func_002AEFA8 stores the driver's volume (func_003B86C0 / func_003B7F18);
                                //      func_002AF6C0 compares and stores the volume it sets; func_002AF428 returns it
    int mFxLevel;               // 0x10 [R] func_002AE4C0: 0; [U] func_002AF4E0 stores the effect send level;
                                //      func_002AF4B0 returns it
    int mDryLevel;              // 0x14 [R] func_002AE4C0: 0x7F; [U] func_002AF580 stores the dry level;
                                //      func_002AF480 returns it
    int mSavedPitchMult;        // 0x18 [U] func_002AF7A0 saves mPlayOpts.pitchmult here when it pauses (pitch 0) and
                                //      applies it again when it resumes
    char mName[12];             // 0x1C [R] func_002AE4C0: sprintf("%.*s", 11, name)
    int mPreloadData;           // 0x28 [U] func_002AE768 stores it; func_002AE7A8 queues file offsets below
                                //      mPreloadSize from it (func_003B7E10) when set
    unsigned int mPreloadSize;  // 0x2C [U] func_002AE768 stores it; func_002AE7A8 compares the file offset with it
                                //      (unsigned)
    SNDPLAYOPTS mPlayOpts;      // 0x30 [R] func_002AE4C0 copies the caller's options (or SNDplaysetdef's defaults),
                                //      clears fxlevel0 and passes them to SNDSTRM_create; [U] func_002AE7A8 stores vol
                                //      and passes them to SNDBANK_play; func_002AF7A0 reads pitchmult
    int mElapsedMs;             // 0x48 [U] func_002AEFA8 stores the elapsed ms (bank: sample position / rate; stream:
                                //      the playing request's currenttime); func_002AF2F0 returns it
    int mRemainingMs;           // 0x4C [U] func_002AEFA8 stores the remaining ms (bank: SNDtimeremaining x 10;
                                //      stream: timetoend); func_002AF2F0 returns it
    int mBuffered;              // 0x50 [U] func_002AEFA8: a bank player's request-0 mLength, a stream's sum of its
                                //      requests' timebuffered; func_002AEA38 returns it
    int mPositionTotal;         // 0x54 [U] func_002AEFA8 (stream) adds the finished requests' mLength and the
                                //      playing ones' time to end; func_002AEAA8 returns it for sub < 0
    int mBusy;                  // 0x58 [U] func_002AEFA8 sets it to 1, then clears it when SNDSTRM_status's count
                                //      reaches mRequestCount, a request has finished, or a playing request has less
                                //      buffered than left to play; func_002AEBD0 returns it
    int mCurrentRequest;        // 0x5C [R] func_002AE138: -1; [U] func_002AEFA8 stores the handle of the request it
                                //      last saw playing; func_002AF2F0 returns it when mStatus is 0
    int mStatus;                // 0x60 [U] func_002AEFA8 stores func_003B6670's / SNDSTRM_status's result (-8 when it
                                //      made no call; 0 on a reset); func_002AF2F0 returns it when nonzero
    signed char mIsBankPlayer;  // 0x64 [R] func_002AE4C0: 1 when maxChunks is 0; [U] every driver call picks the SND*
                                //      (bank) or SNDSTRM_* (stream) variant on it
    signed char mDirty;         // 0x65 [R] func_002AE138: 1; [U] func_002AE7A8 / func_002AEC28 / func_002AF7A0 /
                                //      func_002AF838 set it; func_002AEFA8 clears it; the queries refresh while set
    signed char mRequestCount;  // 0x66 [R] func_002AE138: -1; func_002AE4C0: the request count
    char unk_67[0x5];           // 0x67
    tPathRequest* mRequests;    // 0x6C [R] func_002AE4C0 points it right after this record (mRequestCount entries)
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

// One bank record (0x28): a sound bank loaded for a bank player. [R] func_002AE1F0 zeroes 40 bytes and sets the first
// three words to -1; func_002AE048 sets them to -1 and clears +0x18 (stride 0x28); [U] func_002AE230 searches the 24
// records by mPlayer and mBankIndex.
struct tPathBank {
    int mBankIndex;     // 0x00 [R] -1; [U] func_002AE298 stores the bank index within the player; func_002AE230
                        //      matches it; func_002AE328 resets it to -1
    int mHandle;        // 0x04 [R] -1; [U] func_002AE298 stores the loaded bank handle; func_002AE7A8 plays from it;
                        //      func_002AE328 tests it (>= 0) and resets it to -1
    int mPlayer;        // 0x08 [R] -1 (free); [U] func_002AE298 stores the player index; func_002AE230 matches it
                        //      (< 0: free); func_002AE328 resets it to -1
    int unk_0C;         // 0x0C
    int unk_10;         // 0x10
    void* mMemory;      // 0x14 [U] func_002AE690 passes it to the free hook D_004A3794 when set; func_002AE328 clears
                        //      it (no store of a nonzero value found)
    int unk_18;         // 0x18 [R] func_002AE048: 0
    int mUsed;          // 0x1C [U] func_002AE298 stores 1; func_002AE328 stores 0
    int unk_20;         // 0x20
    int mLastPlayTime;  // 0x24 [U] func_002AE7A8 stores the time (ms, func_002AF370) it played from the bank;
                        //      func_002AE3A8 evicts the bank with the smallest; func_002AE328 resets it to -1
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
