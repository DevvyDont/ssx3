#ifndef EALIB_AVPLAYER_H
#define EALIB_AVPLAYER_H

// RCMP_AV_PLAYER (name ours; the PS2 counterpart of On Tour's PspAvPlayer): EA's AV movie player that cMoviePlayer
// drives (cMoviePlayer::m_pPlayer). It owns the video / audio STREAMs, the audio object, the RCMP DECODER and the
// millisecond timer. Retail code: unit ealib/seg_2AEDC0 (constructors RCMP_AV_PLAYER_AV_PLAYER / RCMP_AV_PLAYER_AV_PLAYER1 / RCMP_AV_PLAYER_AV_PLAYER2 /
// RCMP_AV_PLAYER_AV_PLAYER3 -> RCMP_AV_PLAYER_Init, destructor RCMP_AV_PLAYER__AV_PLAYER, start RCMP_AV_PLAYER_GetFirstFrame, decode RCMP_AV_PLAYER_GetFrame, due check
// RCMP_AV_PLAYER_IsTimeForDecode). The library's strings say AV ("AV_VideoStreamBuffer"; RCMP_AV_MS_TIMER, RCMP_AV_SUBTITLE_ARRAY),
// hence the name.
//
// Size 0x78 [R]: cMoviePlayer_init allocates 0x78 bytes for it from RCMP_rcmp_sys. A FLAT layout; only members with a
// clear retail use are named. Evidence tags: [R] the constructor RCMP_AV_PLAYER_Init / destructor RCMP_AV_PLAYER__AV_PLAYER; [U] how
// retail code uses the field.

#include "layout.h"
#include "ealib/rcmpbase.h"     // RCMP_DECODER, RCMP_STREAMER

struct RCMP_AV_SUBTITLE_ARRAY;  // ealib/avsubtitle.h

// RCMP::AV_MS_TIMER (0x18 [R] RCMP_AV_PLAYER_GetFirstFrame allocates 0x18 bytes; [O] On Tour's PspMsTimer has the same members at the
// same offsets): a millisecond clock that runs at a settable speed. Retail code: RCMP_AV_MS_TIMER_Update (the symbol
// names the class), RCMP_AV_PLAYER_SetSpeed (set speed), RCMP_AV_PLAYER_SyncedAudioTime (read).
struct RCMP_AV_MS_TIMER {
    unsigned int m_OldTime;         // 0x00 [O]; [R] RCMP_AV_PLAYER_GetFirstFrame = TIMER_gettick(); [U] RCMP_AV_PLAYER_SetSpeed stores the tick
                                    //      of its update
    int pad04;
    unsigned long m_AccTime;        // 0x08 [O] (unsigned long long; EE long is 8 bytes); [R] RCMP_AV_PLAYER_GetFirstFrame 0; [U] the
                                    //      accumulated ticks, read as ms by * 1000 / TIMER_getfrequency()
    unsigned int m_Speed;           // 0x10 [O]; [R] RCMP_AV_PLAYER_GetFirstFrame 0x1000 (normal speed); [U] RCMP_AV_PLAYER_SetSpeed
    int pad14;
};

CHECK_OFFSET(RCMP_AV_MS_TIMER, m_OldTime, 0x00);
CHECK_OFFSET(RCMP_AV_MS_TIMER, m_AccTime, 0x08);
CHECK_OFFSET(RCMP_AV_MS_TIMER, m_Speed, 0x10);
CHECK_SIZE(RCMP_AV_MS_TIMER, 0x18);

// The movie player's audio object (0x14 [R] RCMP_AV_PLAYER_GetFirstFrame allocates 20 bytes; name ours, as On Tour's PSP player
// uses its own PspAudioPlayer): an SNDSTRM stream fed from the player's audio STREAM. Retail code: constructor
// RCMP_AV_AUDIO_AV_AUDIO, destructor RCMP_AV_AUDIO__AV_AUDIO, speed RCMP_AV_AUDIO_SetSpeed, start RCMP_AV_AUDIO_StartSound, volume RCMP_AV_AUDIO_SetVol, finished
// RCMP_AV_AUDIO_IsAudioFinished.
struct RCMP_AV_AUDIO {
    float mVolume;                  // 0x00 [R] ctor 1.0f (never read in this unit; the name is a guess)
    void* mpBuffer;                 // 0x04 [R] ctor allocates "AV_audiobuff"; dtor frees it
    int pad08;
    int mStream;                    // 0x0C [R] ctor the SNDSTRM handle (SNDSTRM_createFromFileStream), -1 until opened; [U] every
                                    //      SNDSTRM call uses it
    int mRequest;                   // 0x10 [R] ctor the queued request (SNDSTRM_queueFileRequest), -1 until queued; [U]
                                    //      RCMP_AV_AUDIO_StartSound releases its hold
};

CHECK_OFFSET(RCMP_AV_AUDIO, mVolume, 0x00);
CHECK_OFFSET(RCMP_AV_AUDIO, mpBuffer, 0x04);
CHECK_OFFSET(RCMP_AV_AUDIO, mStream, 0x0C);
CHECK_OFFSET(RCMP_AV_AUDIO, mRequest, 0x10);
CHECK_SIZE(RCMP_AV_AUDIO, 0x14);

struct RCMP_AV_PLAYER {
    RCMP_AV_AUDIO* mpAudio;         // 0x00 [R] ctor 0; RCMP_AV_PLAYER_GetFirstFrame = the 20-byte audio object (RCMP_AV_AUDIO_AV_AUDIO) unless
                                    //      no-audio; dtor deletes it (RCMP_AV_AUDIO__AV_AUDIO)
    void* mpVideoStreamBuffer;      // 0x04 [R] ctor allocates "AV_VideoStreamBuffer"; dtor frees it
    void* mpAudioStreamBuffer;      // 0x08 [R] ctor allocates "AV_AudioStreamBuffer" with separate audio, else 0
    char* mVideoSource;             // 0x0C [R] ctor: the video file name or data address ([U] STREAM_queuefile's
                                    //      char* name / STREAM_queuemem's char* data)
    char* mAudioSource;             // 0x10 [R] ctor: the audio file name or data address
    int mbSeparateAudio;            // 0x14 [R] ctor 1 with an audio source, else 0; [U] the dtor destroys
                                    //      mAudioStream only when set
    int mSourceType;                // 0x18 [R] ctor: 0 streams the files, 1 loads them whole, 2 plays from memory
    int mbNoAudio;                  // 0x1C [R] ctor; [U] RCMP_AV_PLAYER_GetFirstFrame skips the audio object when set
    char* mpVideoFileData;          // 0x20 [R] ctor 0; [U] RCMP_AV_PLAYER_GetFirstFrame ASYNCFILE_release()s the load into it and
                                    //      queues it (STREAM_queuemem's char* data)
    int mVideoLoad;                 // 0x24 [R] ctor 0, ASYNCFILE_load() for source type 1
    char* mpAudioFileData;          // 0x28 [R] as 0x20, for the audio
    int mAudioLoad;                 // 0x2C [R] as 0x24, for the audio
    int mVideoStream;               // 0x30 [R] ctor STREAM_create(); dtor STREAM_destroy()s it
    int mAudioStream;               // 0x34 [R] ctor STREAM_create() / STREAM_taphandle()
    int mVideoQueue;                // 0x38 [R] ctor / RCMP_AV_PLAYER_GetFirstFrame STREAM_queuefile() / STREAM_queuemem()
    int mAudioQueue;                // 0x3C [R] as 0x38; [U] RCMP_AV_PLAYER_GetFirstFrame copies 0x38 when there is no separate audio
    unsigned int mTimeOffsetMs;     // 0x40 [U] RCMP_AV_PLAYER_GetFirstFrame stores its offset argument; RCMP_AV_PLAYER_IsTimeForDecode adds it to the
                                    //      synced time
    unsigned int mCurrentFrameNumber; // 0x44 [O] (mCurFrame, unsigned int); [R] ctor 0; [U] RCMP_AV_PLAYER_GetFrame =
                                    //      RCMP_DECODER_GetCurrentFrameNumber()
    float mGoalFrame;               // 0x48 [U] RCMP_AV_PLAYER_IsTimeForDecode = the frame due now; RCMP_AV_PLAYER_GetFrame stores its goal;
                                    //      cMoviePlayer_syncPlay / asyncUpdate pass it back to RCMP_AV_PLAYER_GetFrame
    unsigned int mStartTimeMs;      // 0x4C [U] RCMP_AV_PLAYER_GetFirstFrame: the timer's start time in ms; RCMP_AV_PLAYER_SyncedAudioTime moves it
                                    //      to follow the audio. [O] On Tour's PspAvPlayer has this (mRefMS) and the
                                    //      next three members, in another order
    int mOldAudioTime;              // 0x50 [O]; [U] RCMP_AV_PLAYER_GetFirstFrame 0; RCMP_AV_PLAYER_SyncedAudioTime: the last audio position read
    unsigned int mbTrackingAudio;   // 0x54 [O] (bool there); [U] RCMP_AV_PLAYER_GetFirstFrame: 1 when the audio stream opened;
                                    //      RCMP_AV_PLAYER_SyncedAudioTime syncs to the audio only when set
    int mFilteredError;             // 0x58 [O]; [U] RCMP_AV_PLAYER_GetFirstFrame 0; RCMP_AV_PLAYER_SyncedAudioTime: the filtered audio drift
    RCMP_AV_MS_TIMER* mpTimer;      // 0x5C [R] ctor 0; [U] RCMP_AV_PLAYER_GetFirstFrame = the 0x18-byte millisecond timer; the dtor
                                    //      frees it
    RCMP_STREAMER mStreamer;        // 0x60 [R] ctors 0; [U] RCMP_AV_PLAYER_GetFirstFrame = this, handed to the CODEC_IDATA, which
                                    //      passes it back to the get / release functions
    RCMP_DECODER* mpDecoder;        // 0x64 [R] ctor 0; [U] RCMP_AV_PLAYER_GetFirstFrame = RCMP_DECODER_DECODER(0x1C bytes); dtor
                                    //      deletes it (slot 1); cMoviePlayer_startPlay hands it to SSX_VD_create,
                                    //      cMoviePlayer_purge to RCMP_DECODER_ReleaseFrame
    char pad68[0x70 - 0x68];
    RCMP_AV_SUBTITLE_ARRAY* mSubtitles; // 0x70 [R] ctor 0; [U] RCMP_AV_PLAYER_GetSubtitle searches it
                                    //      (RCMP_AV_SUBTITLE_ARRAY_FindSubtitle)
    int mCurrFrame;                 // 0x74 [R] ctor 0; [U] RCMP_AV_PLAYER_GetFirstFrame / RCMP_AV_PLAYER_GetFrame = RCMP_DECODER_GetFrame()
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
CHECK_OFFSET(RCMP_AV_PLAYER, mOldAudioTime, 0x50);
CHECK_OFFSET(RCMP_AV_PLAYER, mbTrackingAudio, 0x54);
CHECK_OFFSET(RCMP_AV_PLAYER, mFilteredError, 0x58);
CHECK_OFFSET(RCMP_AV_PLAYER, mpTimer, 0x5C);
CHECK_OFFSET(RCMP_AV_PLAYER, mStreamer, 0x60);
CHECK_OFFSET(RCMP_AV_PLAYER, mpDecoder, 0x64);
CHECK_OFFSET(RCMP_AV_PLAYER, mSubtitles, 0x70);
CHECK_OFFSET(RCMP_AV_PLAYER, mCurrFrame, 0x74);
CHECK_SIZE(RCMP_AV_PLAYER, 0x78);

#endif /* EALIB_AVPLAYER_H */
