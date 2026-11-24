#ifndef EALIB_AVPLAYER_H
#define EALIB_AVPLAYER_H

// RCMP_AV_PLAYER (name ours; the PS2 counterpart of On Tour's PspAvPlayer): EA's AV movie player that cMoviePlayer
// drives (cMoviePlayer::m_pPlayer). It owns the video / audio STREAMs, the audio object, the RCMP DECODER and the
// millisecond timer. Retail code: unit ealib/seg_2AEDC0 (constructors func_003ADDC0 / func_003ADE08 / func_003ADE58 /
// func_003ADE90 -> func_003ADEC8, destructor func_003AE300, start func_003AE450, decode func_003AE6C8, due check
// func_003AE780). The library's strings say AV ("AV_VideoStreamBuffer"; RCMP_AV_MS_TIMER, RCMP_AV_SUBTITLE_ARRAY),
// hence the name.
//
// Size 0x78 [R]: cMoviePlayer_init allocates 0x78 bytes for it from RCMP_rcmp_sys. A FLAT layout; only members with a
// clear retail use are named. Evidence tags: [R] the constructor func_003ADEC8 / destructor func_003AE300; [U] how
// retail code uses the field.

#include "layout.h"
#include "ealib/rcmpbase.h"     // RCMP_DECODER, RCMP_STREAMER

struct RCMP_AV_PLAYER {
    void* mpAudio;                  // 0x00 [R] ctor 0; func_003AE450 = the 20-byte audio object (func_003AED40) unless
                                    //      no-audio; dtor deletes it (func_003AEE30)
    void* mpVideoStreamBuffer;      // 0x04 [R] ctor allocates "AV_VideoStreamBuffer"; dtor frees it
    void* mpAudioStreamBuffer;      // 0x08 [R] ctor allocates "AV_AudioStreamBuffer" with separate audio, else 0
    int mVideoSource;               // 0x0C [R] ctor: the video file name or data address
    int mAudioSource;               // 0x10 [R] ctor: the audio file name or data address
    int mbSeparateAudio;            // 0x14 [R] ctor 1 with an audio source, else 0; [U] the dtor destroys
                                    //      mAudioStream only when set
    int mSourceType;                // 0x18 [R] ctor: 0 streams the files, 1 loads them whole, 2 plays from memory
    int mbNoAudio;                  // 0x1C [R] ctor; [U] func_003AE450 skips the audio object when set
    void* mpVideoFileData;          // 0x20 [R] ctor 0; [U] func_003AE450 ASYNCFILE_release()s the load into it
    int mVideoLoad;                 // 0x24 [R] ctor 0, ASYNCFILE_load() for source type 1
    void* mpAudioFileData;          // 0x28 [R] as 0x20, for the audio
    int mAudioLoad;                 // 0x2C [R] as 0x24, for the audio
    int mVideoStream;               // 0x30 [R] ctor STREAM_create(); dtor STREAM_destroy()s it
    int mAudioStream;               // 0x34 [R] ctor STREAM_create() / STREAM_taphandle()
    int mVideoQueue;                // 0x38 [R] ctor / func_003AE450 STREAM_queuefile() / STREAM_queuemem()
    int mAudioQueue;                // 0x3C [R] as 0x38; [U] func_003AE450 copies 0x38 when there is no separate audio
    unsigned int mTimeOffsetMs;     // 0x40 [U] func_003AE450 stores its offset argument; func_003AE780 adds it to the
                                    //      synced time
    int mCurrentFrameNumber;        // 0x44 [R] ctor 0; [U] func_003AE6C8 = RCMP_DECODER_GetCurrentFrameNumber()
    float mGoalFrame;               // 0x48 [U] func_003AE780 = the frame due now; func_003AE6C8 stores its goal;
                                    //      cMoviePlayer_syncPlay / asyncUpdate pass it back to func_003AE6C8
    unsigned int mStartTimeMs;      // 0x4C [U] func_003AE450: the timer's start time in ms
    char pad50[0x5C - 0x50];
    void* mpTimer;                  // 0x5C [R] ctor 0; [U] func_003AE450 = the 0x18-byte millisecond timer; the dtor
                                    //      frees it
    RCMP_STREAMER mStreamer;        // 0x60 [R] ctors 0; [U] func_003AE450 = this, handed to the CODEC_IDATA, which
                                    //      passes it back to the get / release functions
    RCMP_DECODER* mpDecoder;        // 0x64 [R] ctor 0; [U] func_003AE450 = RCMP_DECODER_DECODER(0x1C bytes); dtor
                                    //      deletes it (slot 1); cMoviePlayer_startPlay hands it to SSX_VD_create,
                                    //      cMoviePlayer_purge to RCMP_DECODER_ReleaseFrame
    char pad68[0x70 - 0x68];
    int mSubtitles;                 // 0x70 [R] ctor 0; [U] func_003AE718 searches it
                                    //      (RCMP_AV_SUBTITLE_ARRAY_FindSubtitle)
    int mCurrFrame;                 // 0x74 [R] ctor 0; [U] func_003AE450 / func_003AE6C8 = RCMP_DECODER_GetFrame()
};

CHECK_OFFSET(RCMP_AV_PLAYER, mpAudio, 0x00);
CHECK_OFFSET(RCMP_AV_PLAYER, mpVideoStreamBuffer, 0x04);
CHECK_OFFSET(RCMP_AV_PLAYER, mpAudioStreamBuffer, 0x08);
CHECK_OFFSET(RCMP_AV_PLAYER, mVideoSource, 0x0C);
CHECK_OFFSET(RCMP_AV_PLAYER, mAudioSource, 0x10);
CHECK_OFFSET(RCMP_AV_PLAYER, mbSeparateAudio, 0x14);
CHECK_OFFSET(RCMP_AV_PLAYER, mSourceType, 0x18);
CHECK_OFFSET(RCMP_AV_PLAYER, mbNoAudio, 0x1C);
CHECK_OFFSET(RCMP_AV_PLAYER, mpVideoFileData, 0x20);
CHECK_OFFSET(RCMP_AV_PLAYER, mVideoLoad, 0x24);
CHECK_OFFSET(RCMP_AV_PLAYER, mpAudioFileData, 0x28);
CHECK_OFFSET(RCMP_AV_PLAYER, mAudioLoad, 0x2C);
CHECK_OFFSET(RCMP_AV_PLAYER, mVideoStream, 0x30);
CHECK_OFFSET(RCMP_AV_PLAYER, mAudioStream, 0x34);
CHECK_OFFSET(RCMP_AV_PLAYER, mVideoQueue, 0x38);
CHECK_OFFSET(RCMP_AV_PLAYER, mAudioQueue, 0x3C);
CHECK_OFFSET(RCMP_AV_PLAYER, mTimeOffsetMs, 0x40);
CHECK_OFFSET(RCMP_AV_PLAYER, mCurrentFrameNumber, 0x44);
CHECK_OFFSET(RCMP_AV_PLAYER, mGoalFrame, 0x48);
CHECK_OFFSET(RCMP_AV_PLAYER, mStartTimeMs, 0x4C);
CHECK_OFFSET(RCMP_AV_PLAYER, mpTimer, 0x5C);
CHECK_OFFSET(RCMP_AV_PLAYER, mStreamer, 0x60);
CHECK_OFFSET(RCMP_AV_PLAYER, mpDecoder, 0x64);
CHECK_OFFSET(RCMP_AV_PLAYER, mSubtitles, 0x70);
CHECK_OFFSET(RCMP_AV_PLAYER, mCurrFrame, 0x74);
CHECK_SIZE(RCMP_AV_PLAYER, 0x78);

#endif /* EALIB_AVPLAYER_H */
