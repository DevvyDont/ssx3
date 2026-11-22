#include "vtable.h"
#include "common.h"
#include "ealib/shape.h"
#include "ai/player.h"
#include "ealib/rcmpbase.h"
#include "ealib/avsubtitle.h"
#include "ealib/ps2sonycodec.h"

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003ADDC0);
#ifdef SKIP_ASM
extern "C" void func_003ADEC8(void* self, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

/**
 * Constructs a movie player that plays video only, with the source type given.
 *
 * Clears +0x60 and calls func_003ADEC8 with the missing sizes as 0. Passes no audio source, so the player uses one
 * interleaved stream. Its caller cMoviePlayer_init uses it when the movie has no separate audio file.
 *
 * The player object is 0x78 bytes: +0x0 audio object, +0x4/+0x8 video/audio stream buffers, +0xC/+0x10 video/audio
 * sources, +0x14 separate-audio flag, +0x18 source type, +0x1C no-audio flag, +0x30/+0x34 video/audio STREAMs,
 * +0x38/+0x3C their queued handles, +0x5C millisecond timer, +0x64 the RCMP DECODER.
 *
 * @param videoSource  the video file name, or the data address for source type 2
 * @param videoBufferSize  size of the video stream buffer
 * @param sourceType  0 streams the file (STREAM_queuefile), 1 loads it whole (ASYNCFILE_load), 2 plays from memory
 * @param noAudio  nonzero to play without sound
 * @return self
 */
extern "C" void* func_003ADDC0(void* self, int videoSource, int videoBufferSize, int sourceType, int noAudio)
{
    *(int*)((char*)self + 0x60) = 0;
    func_003ADEC8(self, videoSource, 0, videoBufferSize, 0, 0, 0, sourceType, noAudio);
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003ADE08);
#ifdef SKIP_ASM
extern "C" void func_003ADEC8(void* self, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

/**
 * Constructs a movie player with separate video and audio sources.
 *
 * Clears +0x60 and calls func_003ADEC8 with the missing sizes as 0. The data sizes (used only for in-memory sources)
 * are passed as 0. Its caller cMoviePlayer_init uses it when the movie has a separate audio file.
 *
 * The player object is 0x78 bytes: +0x0 audio object, +0x4/+0x8 video/audio stream buffers, +0xC/+0x10 video/audio
 * sources, +0x14 separate-audio flag, +0x18 source type, +0x1C no-audio flag, +0x30/+0x34 video/audio STREAMs,
 * +0x38/+0x3C their queued handles, +0x5C millisecond timer, +0x64 the RCMP DECODER.
 *
 * @param videoSource  the video file name or data address
 * @param videoBufferSize  size of the video stream buffer
 * @param audioSource  the audio file name or data address
 * @param audioBufferSize  size of the audio stream buffer
 * @param sourceType  0 streams the files, 1 loads them whole, 2 plays from memory
 * @param noAudio  nonzero to play without sound
 * @return self
 */
extern "C" void* func_003ADE08(void* self, int videoSource, int videoBufferSize, int audioSource, int audioBufferSize, int sourceType, int noAudio)
{
    *(int*)((char*)self + 0x60) = 0;
    func_003ADEC8(self, videoSource, 0, videoBufferSize, audioSource, 0, audioBufferSize, sourceType, noAudio);
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003ADE58);
#ifdef SKIP_ASM
extern "C" void func_003ADEC8(void* self, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

/**
 * Constructs a movie player with every source parameter given.
 *
 * Clears +0x60 and forwards all arguments to func_003ADEC8. Nothing in the code or data refers to it.
 *
 * The player object is 0x78 bytes: +0x0 audio object, +0x4/+0x8 video/audio stream buffers, +0xC/+0x10 video/audio
 * sources, +0x14 separate-audio flag, +0x18 source type, +0x1C no-audio flag, +0x30/+0x34 video/audio STREAMs,
 * +0x38/+0x3C their queued handles, +0x5C millisecond timer, +0x64 the RCMP DECODER.
 *
 * @param videoSource  the video file name or data address
 * @param videoDataSize  size of the in-memory video data (type 2)
 * @param videoBufferSize  size of the video stream buffer
 * @param audioSource  the audio file name or data address (0 for none)
 * @param audioDataSize  size of the in-memory audio data (type 2)
 * @param audioBufferSize  size of the audio stream buffer
 * @param sourceType  0 streams the files, 1 loads them whole, 2 plays from memory
 * @param noAudio  nonzero to play without sound
 * @return self
 */
extern "C" void* func_003ADE58(void* self, int videoSource, int videoDataSize, int videoBufferSize, int audioSource, int audioDataSize, int audioBufferSize, int sourceType, int noAudio)
{
    *(int*)((char*)self + 0x60) = 0;
    func_003ADEC8(self, videoSource, videoDataSize, videoBufferSize, audioSource, audioDataSize, audioBufferSize, sourceType, noAudio);
    return self;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003ADE90);
#ifdef SKIP_ASM
extern "C" void func_003ADEC8(void* self, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

/**
 * Constructs a movie player that plays video (and audio) already in memory.
 *
 * Clears +0x60 and calls func_003ADEC8 with source type 2. Nothing in the code or data refers to it.
 *
 * @param videoSource  address of the video data
 * @param videoDataSize  size of the video data
 * @param videoBufferSize  size of the video stream buffer
 * @param audioSource  address of the audio data (0 for none)
 * @param audioDataSize  size of the audio data
 * @param audioBufferSize  size of the audio stream buffer
 * @param noAudio  nonzero to play without sound
 * @return self
 */
extern "C" void* func_003ADE90(void* self, int videoSource, int videoDataSize, int videoBufferSize, int audioSource, int audioDataSize, int audioBufferSize, int noAudio)
{
    *(int*)((char*)self + 0x60) = 0;
    func_003ADEC8(self, videoSource, videoDataSize, videoBufferSize, audioSource, audioDataSize, audioBufferSize, 2, noAudio);
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003ADEC8);
#ifdef SKIP_ASM
struct sAlloc_ADEC8 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* (*free)(void* p);
    int flags;
};
// PORT: the unit declares RCMP_rcmp_sys as char[] later; view bound by asm label
extern sAlloc_ADEC8 D_00509430_ADEC8 __asm__("RCMP_rcmp_sys");
extern char str_AV_VideoStreamBuffer[];
extern char str_AV_AudioStreamBuffer[];
extern "C" signed char func_003B5440();
extern "C" void func_003E62B0();
extern "C" void MEM_fill(void* dst, int value, int size);
extern "C" void STREAM_setpriority(int h, int a, int b);
extern "C" int STREAM_queuemem(int h, int a, int b, int c);
extern "C" int ASYNCFILE_load(int a, int flags);
extern "C" int STREAM_create(int a, int b, int c, void* buf, int size);
extern "C" void STREAM_setfilter(int h, int a, int b, int c, int d);
extern "C" int STREAM_taphandle(int h, int a);
extern "C" int STREAM_queuefile(int h, int a, int b, int c);

static inline bool wsCanAlloc_ADEC8(sAlloc_ADEC8* a)
{
    return a->alloc != 0 && a->free != 0;
}

/**
 * Initialises a movie player: clears it, allocates its stream buffers and opens its sources.
 *
 * Forces no-audio when func_003B5440 returns 0, clears the 0x78-byte object, stores the source type (+0x18), no-audio
 * flag (+0x1C) and the two sources (+0xC, +0x10). Without RCMP allocator functions in RCMP_rcmp_sys it calls func_003E62B0
 * (a debug break) and stops.
 *
 * With an audio source it sets the separate-audio flag (+0x14), allocates "AV_VideoStreamBuffer" and
 * "AV_AudioStreamBuffer", creates one STREAM on each (STREAM_create), sets their filters with STREAM_setfilter (video
 * chunks tagged 0x4D under mask 0xFF, audio 0x4353 under 0xFFFF; with no audio the audio filter gets -1), then opens
 * both sources by type: 1 ASYNCFILE_load (+0x24/+0x2C), 0 STREAM_queuefile (+0x38/+0x3C), 2 STREAM_queuemem on the
 * memory. Without one it makes a single interleaved stream and takes the audio STREAM from it with STREAM_taphandle.
 *
 * STREAM_setpriority(stream, 152, 52) is called on both streams first; func_003AE450 later calls it with 151, 51.
 *
 * @param selfp  the player object
 * @param videoSource  the video file name or data address
 * @param videoDataSize  size of the in-memory video data (type 2)
 * @param videoBufferSize  size of the video stream buffer
 * @param audioSource  the audio file name or data address; 0 for one interleaved stream
 * @param audioDataSize  size of the in-memory audio data
 * @param audioBufferSize  size of the audio stream buffer
 * @param sourceType  source type: 0 stream, 1 async load, 2 memory
 * @param noAudio  nonzero to play without sound
 */
extern "C" void func_003ADEC8(void* selfp, int videoSource, int videoDataSize, int videoBufferSize, int audioSource, int audioDataSize, int audioBufferSize, int sourceType, int noAudio)
{
    char* self = (char*)selfp;
    if (func_003B5440() == 0) {
        noAudio = 1;
    }
    MEM_fill(self, 0, 0x78);
    *(int*)(self + 0x18) = sourceType;
    *(int*)(self + 0x1C) = noAudio;
    *(int*)(self + 0x20) = 0;
    *(int*)(self + 0x28) = 0;
    *(int*)(self + 0x24) = 0;
    *(int*)(self + 0x2C) = 0;
    *(int*)(self + 0x44) = 0;
    *(int*)(self + 0x5C) = 0;
    *(int*)(self + 0x64) = 0;
    *(int*)(self + 0x0) = 0;
    *(int*)(self + 0x70) = 0;
    *(int*)(self + 0x74) = 0;
    *(int*)(self + 0xC) = videoSource;
    *(int*)(self + 0x10) = audioSource;
    if (!wsCanAlloc_ADEC8(&D_00509430_ADEC8)) {
        func_003E62B0();
        return;
    }
    if (audioSource != 0) {
        *(int*)(self + 0x14) = 1;
        *(void**)(self + 0x4) = D_00509430_ADEC8.alloc(str_AV_VideoStreamBuffer, videoBufferSize, 0, 0, D_00509430_ADEC8.flags);
        *(void**)(self + 0x8) = D_00509430_ADEC8.alloc(str_AV_AudioStreamBuffer, audioBufferSize, 0, 0, D_00509430_ADEC8.flags);
        *(int*)(self + 0x30) = STREAM_create(2, 2, 1, *(void**)(self + 0x4), videoBufferSize);
        *(int*)(self + 0x34) = STREAM_create(2, 2, 1, *(void**)(self + 0x8), audioBufferSize);
        STREAM_setpriority(*(int*)(self + 0x34), 152, 52);
        STREAM_setpriority(*(int*)(self + 0x30), 152, 52);
        STREAM_setfilter(*(int*)(self + 0x30), 1, 0xFF, 77, 1);
        STREAM_setfilter(*(int*)(self + 0x30), 2, 0, 0, -2);
        if (*(int*)(self + 0x1C) == 0) {
            STREAM_setfilter(*(int*)(self + 0x34), 1, 0xFFFF, 17235, 1);
        } else {
            STREAM_setfilter(*(int*)(self + 0x34), 1, 0xFFFF, 17235, -1);
        }
        STREAM_setfilter(*(int*)(self + 0x34), 2, 0, 0, -2);
        int type = *(int*)(self + 0x18);
        if (type == 1) {
            *(int*)(self + 0x2C) = ASYNCFILE_load(audioSource, D_00509430_ADEC8.flags);
            *(int*)(self + 0x24) = ASYNCFILE_load(videoSource, D_00509430_ADEC8.flags);
        } else if (type == 0) {
            *(int*)(self + 0x3C) = STREAM_queuefile(*(int*)(self + 0x34), audioSource, 0, 0);
            *(int*)(self + 0x38) = STREAM_queuefile(*(int*)(self + 0x30), videoSource, 0, 0);
        } else if (type == 2) {
            *(int*)(self + 0x38) = STREAM_queuemem(*(int*)(self + 0x30), *(int*)(self + 0xC), videoDataSize, 0);
            *(int*)(self + 0x3C) = STREAM_queuemem(*(int*)(self + 0x34), *(int*)(self + 0x10), audioDataSize, 0);
        }
    } else {
        *(int*)(self + 0x14) = 0;
        *(void**)(self + 0x4) = D_00509430_ADEC8.alloc(str_AV_VideoStreamBuffer, videoBufferSize, 0, 0, D_00509430_ADEC8.flags);
        *(void**)(self + 0x8) = 0;
        *(int*)(self + 0x30) = STREAM_create(2, 3, 2, *(void**)(self + 0x4), videoBufferSize);
        *(int*)(self + 0x34) = STREAM_taphandle(*(int*)(self + 0x30), 2);
        STREAM_setpriority(*(int*)(self + 0x34), 152, 52);
        STREAM_setpriority(*(int*)(self + 0x30), 152, 52);
        STREAM_setfilter(*(int*)(self + 0x30), 1, 0xFF, 77, 1);
        STREAM_setfilter(*(int*)(self + 0x30), 3, 0, 0, -2);
        if (*(int*)(self + 0x1C) == 0) {
            STREAM_setfilter(*(int*)(self + 0x30), 2, 0xFFFF, 17235, 2);
        } else {
            STREAM_setfilter(*(int*)(self + 0x30), 2, 0xFFFF, 17235, -1);
        }
        int type = *(int*)(self + 0x18);
        if (type == 1) {
            *(int*)(self + 0x24) = ASYNCFILE_load(videoSource, D_00509430_ADEC8.flags);
        } else if (type == 0) {
            *(int*)(self + 0x38) = STREAM_queuefile(*(int*)(self + 0x30), videoSource, 0, 0);
        } else if (type == 2) {
            *(int*)(self + 0x38) = STREAM_queuemem(*(int*)(self + 0x30), *(int*)(self + 0xC), videoDataSize, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE2F8);
#ifdef SKIP_ASM
/**
 * Sets the movie player's subtitle table (+0x70).
 *
 * func_003AE718 looks subtitles up in it with RCMP_AV_SUBTITLE_ARRAY_FindSubtitle. Nothing in the code or data calls it.
 *
 * @param player  the movie player
 * @param table  the subtitle array (two words: entries, count), or 0 for none
 */
extern "C" void func_003AE2F8(cPlayer* player, int table) {
    (*(int *)((char*)(player) + (0x70))) = table;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE300);
#ifdef SKIP_ASM
struct sWSVtE300 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

// Memory interface record filled by FILESYS_getopts (size field first).
struct sWSMemIfE300 {
    int size;                   // 0x0
    int f4;                     // 0x4
    void* (*release)(void*);    // 0x8
    int pad[5];
};

extern void* (*D_00509434[])(void*);
extern "C" void func_003AEE30(void* self, int flags);
extern "C" int FILESYS_getopts(sWSMemIfE300* mi);
extern "C" void STREAM_destroy(void* p);

/**
 * Destroys a movie player: its decoder, audio object, timer, streams and buffers.
 *
 * Deletes the decoder (+0x64) through its slot-1 destructor with flags 3, the audio object (+0x0) with func_003AEE30,
 * frees the timer (+0x5C), destroys the video STREAM (+0x30) and, with separate audio, the audio STREAM (+0x34). Loaded
 * file data (+0x20, +0x28) is released through the memory interface FILESYS_getopts fills; the stream buffers (+0x4,
 * +0x8) and, when flags bit 0 is set, the player itself go to the RCMP free function (D_00509434).
 *
 * @param flags  bit 0 frees the player itself
 */
extern "C" void func_003AE300(char* self, int flags)
{
    char* decoder = *(char**)(self + 0x64);
    if (decoder != 0) {
        sWSVtE300* vt = *(sWSVtE300**)(decoder + 0x18);
        vt[1].fn(vthis(decoder, vt[1]), 3);
        *(void**)(self + 0x64) = 0;
    }
    if (*(void**)(self + 0x0) != 0) {
        func_003AEE30(*(void**)(self + 0x0), 3);
        *(void**)(self + 0x0) = 0;
    }
    if (*(void**)(self + 0x5C) != 0) {
        D_00509434[0](*(void**)(self + 0x5C));
        *(void**)(self + 0x5C) = 0;
    }
    STREAM_destroy(*(void**)(self + 0x30));
    *(void**)(self + 0x30) = 0;
    if (*(int*)(self + 0x14) != 0) {
        STREAM_destroy(*(void**)(self + 0x34));
        *(void**)(self + 0x34) = 0;
    }
    {
        void* fileData = *(void**)(self + 0x20);
        if (fileData != 0) {
            sWSMemIfE300 mi;
            mi.size = 0x20;
            FILESYS_getopts(&mi);
            mi.release(fileData);
            *(void**)(self + 0x20) = 0;
        }
    }
    {
        void* fileData = *(void**)(self + 0x28);
        if (fileData != 0) {
            sWSMemIfE300 mi;
            mi.size = 0x20;
            FILESYS_getopts(&mi);
            mi.release(fileData);
            *(void**)(self + 0x28) = 0;
        }
    }
    if (*(void**)(self + 0x4) != 0) {
        D_00509434[0](*(void**)(self + 0x4));
        *(void**)(self + 0x4) = 0;
    }
    if (*(void**)(self + 0x8) != 0) {
        D_00509434[0](*(void**)(self + 0x8));
        *(void**)(self + 0x8) = 0;
    }
    if (flags & 1) {
        D_00509434[0](self);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE450);
#ifdef SKIP_ASM
struct sAlloc_AE450 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* free;
    int flags;
};
// PORT: the unit declares RCMP_rcmp_sys as char[] later; view bound by asm label
extern sAlloc_AE450 D_00509430_AE450 __asm__("RCMP_rcmp_sys");
extern char D_00495210[];

struct sTimer_AE450 {
    unsigned int start;     // 0x0
    int pad4;
    long ticks;             // 0x8
    int period;             // 0x10
    int pad14;
};

struct RCMP_DECODER_DECODER_sObj;
struct RCMP_DECODER_DECODER_sKey;
extern "C" RCMP_DECODER_DECODER_sObj* RCMP_DECODER_DECODER(RCMP_DECODER_DECODER_sObj* self, RCMP_DECODER_DECODER_sKey* key);
// PORT: the unit declares RCMP_CODEC_IDATA_CODEC_IDATA1 with int params; it stores pointers.
extern "C" void* RCMP_CODEC_IDATA_CODEC_IDATA1_AE450(void* self, void* a1, void* a2, void* a3, int a4) __asm__("RCMP_CODEC_IDATA_CODEC_IDATA1");
void* func_003AECB8(void* self, void* a1);
void* func_003AED20(void* self, void* a1, int a2);
extern "C" void* func_003AED40(void* self, int a1, int a2);
extern "C" int func_003AEEF8(void* self);
extern "C" void STREAM_setpriority(int h, int a, int b);
extern "C" int STREAM_queuemem(int h, int a, int b, int c);
extern "C" void ASYNCFILE_release(int h, void* data, int* size);
extern "C" int RCMP_DECODER_GetFrame(void* p, int a1);
extern "C" unsigned int TIMER_gettick();
extern "C" unsigned int TIMER_getfrequency();
extern "C" void RCMP_AV_MS_TIMER_Update(void* timer);
extern "C" void* func_003AE888(void*, int);

static inline void* wsAlloc_AE450(sAlloc_AE450* a, int size)
{
    return a->alloc(D_00495210, size, 0, 0, a->flags);
}

static inline bool wsCanAlloc_AE450(sAlloc_AE450* a)
{
    return a->alloc != 0 && a->free != 0;
}

/**
 * Starts a movie player: finishes loading, creates its decoder, audio and timer, and decodes the first frame.
 *
 * Calls STREAM_setpriority(stream, 151, 51) on both streams. Async loads (+0x24, +0x2C) are released with ASYNCFILE_release
 * and handed to their STREAMs with STREAM_queuemem. Without separate audio the audio handle (+0x3C) is the video one.
 * Unless no-audio is set it allocates the 20-byte audio object (func_003AED40) at +0x0, and +0x54 records whether its
 * stream opened.
 *
 * It builds the CODEC_IDATA (RCMP_CODEC_IDATA_CODEC_IDATA1: streamer = &+0x60, which holds the player, get-data func_003AECB8, release
 * func_003AED20) and constructs the 0x1C-byte DECODER (RCMP_DECODER_DECODER) at +0x64, then gets frame 0 with RCMP_DECODER_GetFrame
 * into +0x74. It allocates the 0x18-byte millisecond timer (+0x5C, rate 0x1000), stores the start time in ms (+0x4C)
 * and the offset (+0x40), clears +0x50 and +0x58, starts the audio (func_003AEEF8) and sets normal speed (func_003AE888
 * with 0x1000).
 *
 * The game calls it as func_003AE450(player, 1, 33) in cMoviePlayer_startPlay.
 *
 * @param framesToBuffer  number of frames the decoder keeps (the CODEC_IDATA frame count)
 * @param timeOffsetMs  milliseconds added to the playback time when choosing the frame (+0x40)
 * @return the first decoded frame (+0x74)
 */
// PORT: 64-bit long tick counter
extern "C" int func_003AE450(char* self, int framesToBuffer, int timeOffsetMs)
{
    STREAM_setpriority(*(int*)(self + 0x34), 151, 51);
    STREAM_setpriority(*(int*)(self + 0x30), 151, 51);
    if (*(int*)(self + 0x24) != 0) {
        int size;
        ASYNCFILE_release(*(int*)(self + 0x24), self + 0x20, &size);
        *(int*)(self + 0x38) = STREAM_queuemem(*(int*)(self + 0x30), *(int*)(self + 0x20), size, 0);
    }
    if (*(int*)(self + 0x2C) != 0) {
        int size;
        ASYNCFILE_release(*(int*)(self + 0x2C), self + 0x28, &size);
        *(int*)(self + 0x3C) = STREAM_queuemem(*(int*)(self + 0x34), *(int*)(self + 0x28), size, 0);
    }
    if (*(int*)(self + 0x14) == 0) {
        *(int*)(self + 0x3C) = *(int*)(self + 0x38);
    }
    if (*(int*)(self + 0x1C) == 0) {
        char* audioObj = (char*)func_003AED40(wsAlloc_AE450(&D_00509430_AE450, 20), *(int*)(self + 0x34), *(int*)(self + 0x3C));
        *(char**)(self + 0x0) = audioObj;
        *(unsigned int*)(self + 0x54) = *(int*)(audioObj + 0xC) >= 0;
    } else {
        *(unsigned int*)(self + 0x54) = 0;
    }
    *(void**)(self + 0x64) = 0;
    if (wsCanAlloc_AE450(&D_00509430_AE450)) {
        char key[0x10];
        *(char**)(self + 0x60) = self;
        RCMP_CODEC_IDATA_CODEC_IDATA1_AE450(key, self + 0x60, (void*)func_003AECB8, (void*)func_003AED20, framesToBuffer);
        *(RCMP_DECODER_DECODER_sObj**)(self + 0x64) = RCMP_DECODER_DECODER((RCMP_DECODER_DECODER_sObj*)wsAlloc_AE450(&D_00509430_AE450, 0x1C), (RCMP_DECODER_DECODER_sKey*)key);
    }
    *(int*)(self + 0x74) = RCMP_DECODER_GetFrame(*(void**)(self + 0x64), 0);
    sTimer_AE450* msTimer = (sTimer_AE450*)wsAlloc_AE450(&D_00509430_AE450, 0x18);
    msTimer->ticks = 0;
    msTimer->start = TIMER_gettick();
    msTimer->period = 0x1000;
    *(sTimer_AE450**)(self + 0x5C) = msTimer;
    RCMP_AV_MS_TIMER_Update(msTimer);
    unsigned int ms = (unsigned int)(msTimer->ticks * 1000) / TIMER_getfrequency();
    *(int*)(self + 0x40) = timeOffsetMs;
    *(int*)(self + 0x50) = 0;
    *(int*)(self + 0x58) = 0;
    *(unsigned int*)(self + 0x4C) = ms;
    if (*(void**)(self + 0x0) != 0) {
        func_003AEEF8(*(void**)(self + 0x0));
    }
    func_003AE888(self, 0x1000);
    return *(int*)(self + 0x74);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE6C8);
#ifdef SKIP_ASM
extern "C" int fptoui(float f);
extern "C" int RCMP_DECODER_GetFrame(void* p, int a1);
extern "C" int RCMP_DECODER_GetCurrentFrameNumber(RCMP_DECODER* self);

/**
 * Decodes up to a goal frame and returns the movie player's current frame.
 *
 * Stores the goal at +0x48, asks the decoder (+0x64) for that frame with RCMP_DECODER_GetFrame (goal converted by
 * fptoui) into +0x74, and stores the decoder's current frame number (RCMP_DECODER_GetCurrentFrameNumber) at +0x44.
 *
 * @param position  the goal frame number
 * @return the frame object (+0x74), or 0
 */
extern "C" int func_003AE6C8(void* self, float position)
{
    *(float*)((char*)self + 0x48) = position;
    *(int*)((char*)self + 0x74) = RCMP_DECODER_GetFrame(*(void**)((char*)self + 0x64), fptoui(position));
    *(int*)((char*)self + 0x44) = RCMP_DECODER_GetCurrentFrameNumber((RCMP_DECODER*)(*(void**)((char*)self + 0x64)));
    return *(int*)((char*)self + 0x74);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE718);
#ifdef SKIP_ASM
extern "C" int RCMP_AV_SUBTITLE_ARRAY_FindSubtitle(int, int);

/**
 * Returns the subtitle that applies to the movie player's current frame.
 *
 * With a subtitle table (+0x70) it searches it with RCMP_AV_SUBTITLE_ARRAY_FindSubtitle, by the decoder's current frame number (+0x44) when a
 * frame is decoded (+0x74), else by the goal frame (+0x48). Nothing in the code or data calls it.
 *
 * @return the subtitle entry, or 0 without a table
 */
extern "C" int func_003AE718(void *self) {
    if (*(int *)((char *)self + 0x70) != 0) {
        if (*(int *)((char *)self + 0x74) != 0) {
            return RCMP_AV_SUBTITLE_ARRAY_FindSubtitle(*(int *)((char *)self + 0x70), *(int *)((char *)self + 0x44));
        }
        return RCMP_AV_SUBTITLE_ARRAY_FindSubtitle(*(int *)((char *)self + 0x70), fptoui(*(float *)((char *)self + 0x48)));
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE780);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" unsigned int func_003AE9A0_E780(void* self) __asm__("func_003AE9A0");
extern "C" float RCMP_DECODER_GetFrameRate(RCMP_DECODER*);

/**
 * Tells whether the movie player is due to decode its next frame.
 *
 * Computes the goal frame as (func_003AE9A0's synced time + the offset at +0x40) * 0.001 * the decoder's frame rate
 * (RCMP_DECODER_GetFrameRate), stores it at +0x48, and compares it with the current frame number (+0x44) plus 1.
 *
 * @return 1 when the goal frame has reached the next frame, else 0
 */
extern "C" int func_003AE780(char* self)
{
    unsigned int elapsed = func_003AE9A0_E780(self);
    float rate = RCMP_DECODER_GetFrameRate((RCMP_DECODER*)(*(void**)(self + 0x64)));
    *(float*)(self + 0x48) = (float)(elapsed + *(unsigned int*)(self + 0x40)) * 0.001f * rate;
    if (*(float*)(self + 0x48) < (float)*(unsigned int*)(self + 0x44) + 1.0f)
        return 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE860);
#ifdef SKIP_ASM
extern "C" int func_003AEFA8(void* p);

/**
 * Tells whether the movie player's audio has finished.
 *
 * Asks the audio object (+0x0) with func_003AEFA8.
 *
 * @return func_003AEFA8's answer, or 1 when there is no audio object
 */
extern "C" int func_003AE860(void** self)
{
    void* audio = *self;
    int result = 1;
    if (audio != 0) {
        result = func_003AEFA8(audio);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE888);
#ifdef SKIP_ASM
struct func_003AE888_sTimer {
    unsigned int last;
    int pad4;
    ulong accum;
    unsigned int rate;
};

extern "C" int func_003AEE98(void* self, int v);
extern "C" unsigned int TIMER_gettick();

/**
 * Sets the movie player's playback speed (0x1000 is normal, 0 pauses).
 *
 * Clamps the speed to 0x4000, passes it to the audio object (func_003AEE98), then updates the timer (+0x5C): it adds
 * the ticks since the last update (TIMER_gettick) to the 64-bit accumulator, scaled by the old rate / 0x1000, and
 * stores the new rate and time.
 *
 * @param speed  the new speed in 1/0x1000 units
 * @return 0
 */
// PORT: accum is a 64-bit ulong
extern "C" void* func_003AE888(void* self, int speed)
{
    if ((unsigned int)speed > 0x4000) {
        speed = 0x4000;
    }
    int newRate = speed;
    if (*(void**)self != 0) {
        func_003AEE98(*(void**)self, newRate);
    }
    func_003AE888_sTimer* timer = *(func_003AE888_sTimer**)((char*)self + 0x5C);
    unsigned int now = TIMER_gettick();
    if (timer->rate == 0x1000) {
        timer->accum += (ulong)(now - timer->last);
    } else {
        timer->accum += ((ulong)timer->rate * (ulong)(now - timer->last)) >> 12;
    }
    timer->rate = newRate;
    timer->last = now;
    return 0;
}
#endif

extern "C" void* func_003AE888(void*, int);

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE938__FPv);
#ifdef SKIP_ASM
/**
 * Pauses the movie player (speed 0).
 *
 * Calls func_003AE888(self, 0). The game's movie code calls it from cMoviePlayer_startPlay and cMoviePlayer_asyncPause.
 *
 * @return func_003AE888's result (0)
 */
void* func_003AE938(void* self)
{
    return func_003AE888(self, 0);
}
#endif

extern "C" void* func_003AE888(void*, int);

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE958__FPv);
#ifdef SKIP_ASM
/**
 * Resumes the movie player at normal speed (0x1000).
 *
 * Calls func_003AE888(self, 0x1000). The game's movie code calls it from cMoviePlayer_startPlay and cMoviePlayer_asyncResume.
 *
 * @return func_003AE888's result (0)
 */
void* func_003AE958(void* self)
{
    return func_003AE888(self, 0x1000);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE978);
#ifdef SKIP_ASM
extern "C" int func_003AEF48(int);

/**
 * Sets the movie player's audio volume.
 *
 * Forwards the volume (the second argument, which the declaration omits) to the audio object (+0x0) with func_003AEF48.
 * Nothing in the code or data calls it.
 *
 * @param player  the movie player
 * @return func_003AEF48's result, or 0 without audio
 */
extern "C" int func_003AE978(int *player) {
    int temp_4;
    int var_2;

    temp_4 = *player;
    var_2 = 0;
    if (temp_4 != 0) {
        var_2 = func_003AEF48(temp_4);
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE9A0);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" int SNDPROFILE_outputlatency();
extern "C" int SNDSTRM_status(int id, void* out);
extern "C" int SNDSTRM_requeststatus(int id, void* out);
extern "C" unsigned int TIMER_getfrequency();
extern "C" void RCMP_AV_MS_TIMER_Update(void* timer);

// PORT: accum is a 64-bit ulong
struct func_003AE9A0_sTimer {
    unsigned int last;
    int pad4;
    ulong accum;
    unsigned int rate;
};

struct func_003AE9A0_sInfo {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xC;
};

struct func_003AE9A0_sOwner {
    char pad[0xC];
    int id;
};

struct func_003AE9A0_sSync {
    func_003AE9A0_sOwner* owner;
    char pad04[0x48];
    int offset;
    int last;
    int enabled;
    int drift;
    func_003AE9A0_sTimer* timer;
};

/**
 * Returns the movie player's playback time in ms, kept in step with the audio.
 *
 * Updates the timer (+0x5C, RCMP_AV_MS_TIMER_Update) and converts its tick count to ms (ticks * 1000 / TIMER_getfrequency), minus the
 * start time at +0x4C. With audio (+0x54) it reads the audio stream's current request (SNDSTRM_status on the audio
 * object's handle), its play position in ms (SNDSTRM_requeststatus) minus SNDPROFILE_outputlatency, all between SNDSYS_entercritical and
 * SNDSYS_leavecritical. When that position is newer than the last one (+0x50) it feeds the difference into a filtered drift
 * (+0x58, drift - drift/8 + error); once |drift| exceeds 264 it moves the start time by drift/8 and clears the drift.
 *
 * Its only caller is func_003AE780.
 *
 * @return the playback time in ms
 */
extern "C" int func_003AE9A0(func_003AE9A0_sSync* self)
{
    func_003AE9A0_sInfo requestStatus;
    func_003AE9A0_sInfo streamStatus;
    func_003AE9A0_sTimer* msTimer = self->timer;
    RCMP_AV_MS_TIMER_Update(msTimer);
    unsigned int ms = (unsigned int)(msTimer->accum * 1000) / TIMER_getfrequency();
    int res = ms - self->offset;
    if (self->enabled != 0) {
        SNDSYS_entercritical();
        SNDSTRM_status(self->owner->id, &streamStatus);
        SNDSTRM_requeststatus(streamStatus.field_0x4, &requestStatus);
        SNDSYS_leavecritical();
        int audioMs = requestStatus.field_0x4 - SNDPROFILE_outputlatency();
        if (self->last < audioMs) {
            self->last = audioMs;
            self->drift = self->drift - self->drift / 8 + (audioMs - res);
            int av = -self->drift;
            if (av < self->drift) av = self->drift;
            if (av > 264) {
                self->offset -= self->drift / 8;
                self->drift = 0;
                res = ms - self->offset;
            }
        }
    }
    return res;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AEAD0);
#ifdef SKIP_ASM
struct sAlloc_AEAD0 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* free;
    int flags;
};
// PORT: the unit declares RCMP_rcmp_sys as char[] later; view bound by asm label
extern sAlloc_AEAD0 D_00509430_AEAD0 __asm__("RCMP_rcmp_sys");
extern char D_00495210[];

extern "C" unsigned char* STREAM_get(void* q);
extern "C" void SYNCTASK_run(int a);
extern "C" int func_003B11F0(int v);
void* RCMP_CHUNK_CHUNK(void* self);
extern "C" void* func_003B11A0();
// PORT: RCMP_DECODER_ChooseCodec returns its vcall's int result; the unit defines it void.
extern "C" int RCMP_DECODER_ChooseCodec_i(void* self, void* obj, void* item) __asm__("RCMP_DECODER_ChooseCodec");
extern "C" void RCMP_DECODER_FreeChosenCodec(RCMP_DECODER* self);
extern "C" void* STREAM_release(void* p, int a1);
extern "C" int STREAM_isendofstream(void* q);

struct sChunk_AEAD0 {
    unsigned char* data;    // 0x0
    unsigned char* body;    // 0x4
    unsigned int size;      // 0x8
};

static inline unsigned int le32_AEAD0(unsigned char* p)
{
    return (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
}

// PORT: chunk pointers passed through int parameters of some callees. The unit declares
// func_003AEAD0(int, void*) for its caller; the real body takes (self, ctx, out), bound by asm label.
extern "C" void func_003AEAD0_impl(char* self, char* ctx, sChunk_AEAD0** out) __asm__("func_003AEAD0");
/**
 * Fetches the next video chunk from the movie player's stream for the decoder.
 *
 * The decoder's get-data function (reached through func_003AECB8). It pulls blocks from the video STREAM (+0x30) with
 * STREAM_get, running SYNCTASK_run(0) between tries, until one whose big-endian tag func_003B11F0 accepts; other
 * blocks go back with STREAM_release, and it gives up when STREAM_isendofstream says the stream is done. For a match it
 * allocates a 12-byte CHUNK (RCMP_CHUNK_CHUNK) holding the block, its data after the 8-byte header and the little-endian
 * size at +4.
 *
 * If the decoder has no codec yet it creates one (func_003B11A0) and chooses it with RCMP_DECODER_ChooseCodec, freeing it with
 * RCMP_DECODER_FreeChosenCodec unless that returns 2. Quirks: the kind-2 branch (data without header) is unreachable, and the tag
 * copied into a local name buffer is never used.
 *
 * @param ctx  the RCMP DECODER asking for data
 * @param out  receives the new CHUNK, or 0
 */
extern "C" void func_003AEAD0_impl(char* self, char* ctx, sChunk_AEAD0** out)
{
    int kind = 0;
    void* videoStream = *(void**)(self + 0x30);
    *out = 0;
    for (;;) {
        unsigned char* blockData = STREAM_get(videoStream);
        SYNCTASK_run(0);
        if (blockData != 0) {
            unsigned int tag = (blockData[0] << 24) | (blockData[1] << 16) | (blockData[2] << 8) | blockData[3];
            if (func_003B11F0(tag))
                kind = 3;
            if (kind != 0) {
                sChunk_AEAD0* newChunk = (sChunk_AEAD0*)RCMP_CHUNK_CHUNK(D_00509430_AEAD0.alloc(D_00495210, 0xC, 0, 0, D_00509430_AEAD0.flags));
                *out = newChunk;
                newChunk->data = blockData;
                if (kind == 2) {
                    unsigned int size = le32_AEAD0(blockData + 4);
                    newChunk->body = blockData;
                    newChunk->size = size;
                } else {
                    unsigned int size = le32_AEAD0(blockData + 4);
                    newChunk->body = blockData + 8;
                    newChunk->size = size;
                }
                if (*(int*)(ctx + 0x14) != 0)
                    return;
                if (kind != 3)
                    return;
                if (RCMP_DECODER_ChooseCodec_i(ctx, func_003B11A0(), newChunk) != 2)
                    RCMP_DECODER_FreeChosenCodec((RCMP_DECODER*)ctx);
                return;
            }
            char name[8];
            *(unsigned int*)name = tag;
            name[4] = 0;
            STREAM_release(videoStream, (int)blockData);
        }
        if (STREAM_isendofstream(videoStream))
            return;
    }
}
#endif

extern "C" void* func_003AEAD0(int, void*);

//99.44%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AECB8__FPvT0);
#ifdef SKIP_ASM
/**
 * Forwards the decoder's get-data call to func_003AEAD0.
 *
 * Called with the DECODER as self. Installed by func_003AE450 as the CODEC_IDATA get-data function: the streamer
 * argument points at the player's +0x60, which holds the player. The output pointer (third argument) passes through
 * untouched.
 *
 * @param context  the streamer: address of the player pointer
 * @return func_003AEAD0's result
 */
void* func_003AECB8(void* self, void* context)
{
    return func_003AEAD0(*(int*)context, self);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AECE0);
#ifdef SKIP_ASM
extern "C" void* STREAM_release(void* p, int a1);
extern void* (*D_00509434[])(void*);

/**
 * Releases a video chunk: returns its block to the stream and frees the CHUNK.
 *
 * Hands the CHUNK's block (m_UserChunkData) back to the video STREAM (+0x30) with STREAM_release, then frees the CHUNK
 * with the RCMP free function (D_00509434). Likely bug: it reads the CHUNK before checking it for null.
 *
 * @param chunkAddr  the CHUNK
 * @return the free function's result
 */
extern "C" void* func_003AECE0(int self, int chunkAddr)
{
    int* obj = (int*)chunkAddr;
    void* lastResult = STREAM_release(*(void**)(self + 0x30), *obj);
    if (obj != 0) {
        lastResult = D_00509434[0](obj);
    }
    return lastResult;
}
#endif

extern "C" void* func_003AECE0(int, int);

//99.38%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AED20__FPvT0i);
#ifdef SKIP_ASM
/**
 * Forwards the decoder's release-data call to func_003AECE0.
 *
 * Installed by func_003AE450 as the CODEC_IDATA release function; context points at the player pointer.
 *
 * @param context  the streamer: address of the player pointer
 * @param chunk  the CHUNK to release
 * @return func_003AECE0's result
 */
void* func_003AED20(void* self, void* context, int chunk)
{
    return func_003AECE0(*(int*)context, chunk);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AED40);
#ifdef SKIP_ASM
struct func_003AED40_sAlloc {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* free;
    int flags;
};
// PORT: the unit declares RCMP_rcmp_sys as char[] later; view bound by asm label
extern func_003AED40_sAlloc D_00509430_v __asm__("RCMP_rcmp_sys");
extern char str_AV_audiobuff[];

extern "C" signed char func_003B5440();
extern "C" void func_003E62B0();
extern "C" int SNDplaysetdef(void* desc);
extern "C" int SNDSTRM_overheadtap(int a, int b);
extern "C" int func_003B7E38(int a, void* desc, int b, int c, void* mem, int size);
extern "C" int func_003B7DE8(int h, int a, int b);

/**
 * Constructs the movie player's audio object and opens its audio stream.
 *
 * When func_003B5440 returns 0 it only calls func_003E62B0 (a debug break). Otherwise it fills a descriptor with
 * SNDplaysetdef, allocates the "AV_audiobuff" buffer sized by SNDSTRM_overheadtap(1, 0x1E) at +0x4, opens the stream with
 * func_003B7E38 (handle at +0xC), queues the source with func_003B7DE8 (request at +0x10) and sets +0x0 to 1.0f. The
 * object is 20 bytes; func_003AE450 allocates it.
 *
 * @param sourceStream  the audio STREAM
 * @param sourceHandle  the audio source handle to queue
 * @return self
 */
extern "C" void* func_003AED40(void* self, int sourceStream, int sourceHandle)
{
    char desc[0x18];
    if (func_003B5440() == 0) {
        func_003E62B0();
    } else {
        *(void**)((char*)self + 0x4) = 0;
        *(int*)((char*)self + 0x10) = -1;
        *(int*)((char*)self + 0xC) = -1;
        SNDplaysetdef(desc);
        int size = SNDSTRM_overheadtap(1, 0x1E);
        void* mem = D_00509430_v.alloc(str_AV_audiobuff, size, 0, 0, D_00509430_v.flags);
        *(void**)((char*)self + 0x4) = mem;
        int streamHandle = func_003B7E38(sourceStream, desc, 1, 0x1E, mem, size);
        *(int*)((char*)self + 0xC) = streamHandle;
        *(int*)((char*)self + 0x10) = func_003B7DE8(streamHandle, -1, sourceHandle);
        *(float*)self = 1.0f;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AEE30);
#ifdef SKIP_ASM
extern "C" void SNDSTRM_destroy(void* a);
extern void* (*D_00509434[])(void*);

/**
 * Destroys the movie player's audio object.
 *
 * When the buffer (+0x4) exists it closes the stream (+0xC) with SNDSTRM_destroy and frees the buffer; with flags bit 0
 * it frees the object too (D_00509434). Called by func_003AE300.
 *
 * @param flags  bit 0 frees the object itself
 */
extern "C" void func_003AEE30(void* self, int flags)
{
    if (*(void**)((char*)self + 0x4) != 0) {
        SNDSTRM_destroy(*(void**)((char*)self + 0xC));
        D_00509434[0](*(void**)((char*)self + 0x4));
        *(void**)((char*)self + 0x4) = 0;
    }
    if (flags & 1) {
        D_00509434[0](self);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AEE98);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" int SNDSTRM_pitchmult(int h, int v);

/**
 * Sets the audio object's playback speed.
 *
 * Calls SNDSTRM_pitchmult on the stream handle (+0xC) between SNDSYS_entercritical and SNDSYS_leavecritical. Called by func_003AE888.
 *
 * @param speed  the speed in 1/0x1000 units
 * @return SNDSTRM_pitchmult's result, or 0 without a stream
 */
extern "C" int func_003AEE98(void* self, int speed)
{
    int result;
    if (*(int*)((char*)self + 0xC) != -1) {
        SNDSYS_entercritical();
        result = SNDSTRM_pitchmult(*(int*)((char*)self + 0xC), speed);
        SNDSYS_leavecritical();
        return result;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AEEF8);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" int SNDSTRM_modifyhold(int h, int a1);

/**
 * Starts the audio object's queued request.
 *
 * Calls SNDSTRM_modifyhold(request (+0x10), 0) between SNDSYS_entercritical and SNDSYS_leavecritical. Called by func_003AE450.
 *
 * @return SNDSTRM_modifyhold's result, or 0 without a request
 */
extern "C" int func_003AEEF8(void* self)
{
    int result;
    if (*(int*)((char*)self + 0x10) != -1) {
        SNDSYS_entercritical();
        result = SNDSTRM_modifyhold(*(int*)((char*)self + 0x10), 0);
        SNDSYS_leavecritical();
        return result;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AEF48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_003B7E70(int, int);
int func_003AEF48_impl(void *self, int arg) __asm__("func_003AEF48");

/**
 * Sets the audio object's volume.
 *
 * Calls func_003B7E70 on the stream handle (+0xC) between SNDSYS_entercritical and SNDSYS_leavecritical. Called by func_003AE978.
 *
 * @param volume  the new volume
 * @return func_003B7E70's result, or 0 without a stream
 */
int func_003AEF48_impl(void *self, int volume) {
    int result;

    if (*(int *)((char *)self + 0xC) != -1) {
        SNDSYS_entercritical();
        result = func_003B7E70(*(int *)((char *)self + 0xC), volume);
        SNDSYS_leavecritical();
        return result;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AEFA8);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" int SNDSTRM_status(int id, void* out);
extern "C" int SNDSTRM_requeststatus(int id, void* out);

struct func_003AEFA8_sInfo {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xC;
};

/**
 * Tells whether the audio object's stream has finished playing.
 *
 * Reads the stream's status (SNDSTRM_status on +0xC) and its current request's status (SNDSTRM_requeststatus), between
 * SNDSYS_entercritical and SNDSYS_leavecritical. Called by func_003AE860.
 *
 * @return 1 when the request has no time left (third status word 0), 0 otherwise or when a query fails
 */
extern "C" int func_003AEFA8(void* self)
{
    func_003AEFA8_sInfo b;
    func_003AEFA8_sInfo a;
    SNDSYS_entercritical();
    if (SNDSTRM_status(*(int*)((char*)self + 0xC), &a) >= 0 && SNDSTRM_requeststatus(a.field_0x4, &b) >= 0) {
        SNDSYS_leavecritical();
        return b.field_0x8 == 0;
    }
    SNDSYS_leavecritical();
    return 0;
}
#endif

extern void* RCMP_RCMP_SYSTEM_vtbl[];

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_ARRAY_AV_SUBTITLE_ARRAY);
#ifdef SKIP_ASM
/**
 * Constructs an empty subtitle array (no entries, count 0).
 *
 * Nothing in the code or data calls it.
 *
 * @param table  the array
 * @return table
 */
extern "C" void *RCMP_AV_SUBTITLE_ARRAY_AV_SUBTITLE_ARRAY(void *table) {
    (*(int *)((char*)(table) + (0))) = 0;
    (*(int *)((char*)(table) + (4))) = 0;
    return table;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_ARRAY__AV_SUBTITLE_ARRAY);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern char D_00495260[];
extern void (*D_00509434_AF020[])(void*) __asm__("D_00509434");
struct sElem_AF020 {
    char pad[0x34];
    // PORT: the constructor is RCMP_AV_SUBTITLE_AV_SUBTITLE, defined later in this unit as a C function.
    sElem_AF020() __asm__("RCMP_AV_SUBTITLE_AV_SUBTITLE");
    ~sElem_AF020() {}
    void* operator new[](unsigned int size) { return D_00509430_ADEC8.alloc(D_00495260, size, 0, 0, D_00509430_ADEC8.flags); }
    void operator delete[](void* p) { D_00509430_ADEC8.free(p); }
};

/**
 * Destroys a subtitle array: deletes its entries and clears the count.
 *
 * Runs delete[] on the entries (0x34-byte elements, freed through RCMP_rcmp_sys's free function) and nulls the pointer;
 * with flags bit 0 frees the array itself. Nothing in the code or data calls it.
 *
 * @param flags  bit 0 frees the array itself
 */
extern "C" void RCMP_AV_SUBTITLE_ARRAY__AV_SUBTITLE_ARRAY(RCMP_AV_SUBTITLE_ARRAY* self, int flags)
{
    if (((sElem_AF020*)self->mSubTitle)) {
        delete[] ((sElem_AF020*)self->mSubTitle);
        (*(sElem_AF020**)&self->mSubTitle) = 0;
    }
    self->mNumberOfSubtitle = 0;
    if (flags & 1) D_00509434_AF020[0](self);
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_ARRAY_Init);
#ifdef SKIP_ASM
extern "C" void* RCMP_AV_SUBTITLE_AV_SUBTITLE(RCMP_AV_SUBTITLE*);
extern "C" void f03A8_F0C8(void*, int) __asm__("RCMP_AV_SUBTITLE_SetPosX");
extern "C" void f03B0_F0C8(void*, int) __asm__("RCMP_AV_SUBTITLE_SetPosY");
extern "C" void f03B8_F0C8(void*, int) __asm__("RCMP_AV_SUBTITLE_SetWidth");
extern "C" void f03C0_F0C8(void*, int) __asm__("RCMP_AV_SUBTITLE_SetHeight");
extern "C" void f03C8_F0C8(void*, unsigned int) __asm__("RCMP_AV_SUBTITLE_SetStrID");
extern "C" void f03D0_F0C8(void*, int) __asm__("RCMP_AV_SUBTITLE_SetFrameNumber");
extern char D_00495260[];
struct sAlloc_F0C8 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void (*free)(void* p);
    int flags;
};
// PORT: the unit declares RCMP_rcmp_sys as char[] later; view bound by asm label
extern sAlloc_F0C8 D_00509430_F0C8 __asm__("RCMP_rcmp_sys");

struct sElem_F0C8 { char pad[0x34]; };
struct sSelf_F0C8 { sElem_F0C8* arr; unsigned int count; };

/**
 * Fills a subtitle array with a number of default subtitles at one position and size.
 *
 * Allocates count AV_SUBTITLEs (array new with a 0x10-byte count header, tag D_00495260) and constructs each with
 * RCMP_AV_SUBTITLE_AV_SUBTITLE, sets the first one's frame number to 0 (RCMP_AV_SUBTITLE_SetFrameNumber), then gives each the position (RCMP_AV_SUBTITLE_SetPosX,
 * RCMP_AV_SUBTITLE_SetPosY), size (RCMP_AV_SUBTITLE_SetWidth, RCMP_AV_SUBTITLE_SetHeight) and its index as string id (RCMP_AV_SUBTITLE_SetStrID). It does not free
 * earlier entries. Nothing in the code or data calls it.
 *
 * @param vself  the subtitle array
 * @param count  number of subtitles
 * @param posX  x position
 * @param posY  y position
 * @param width  width
 * @param height  height
 */
extern "C" void RCMP_AV_SUBTITLE_ARRAY_Init(void* vself, unsigned int count, int posX, int posY, int width, int height)
{
    sSelf_F0C8* self = (sSelf_F0C8*)vself;
    unsigned int* raw;
    char* arr;
    char* subtitle;
    int i;
    unsigned int k;

    i = count - 1;
    raw = (unsigned int*)D_00509430_F0C8.alloc(D_00495260, count * 52 + 0x10, 0, 0, D_00509430_F0C8.flags);
    arr = (char*)raw + 0x10;
    *raw = count;
    subtitle = arr;
    if (count != 0) {
        do {
            i--;
            RCMP_AV_SUBTITLE_AV_SUBTITLE((RCMP_AV_SUBTITLE*)subtitle);
            subtitle += 0x34;
        } while (i != -1);
    }
    self->arr = (sElem_F0C8*)arr;
    self->count = count;
    if (count != 0) {
        f03D0_F0C8(arr, 0);
    }
    for (k = 0; k < self->count; k++) {
        f03A8_F0C8(&self->arr[k], posX);
        f03B0_F0C8(&self->arr[k], posY);
        f03B8_F0C8(&self->arr[k], width);
        f03C0_F0C8(&self->arr[k], height);
        f03C8_F0C8(&self->arr[k], k);
    }
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_ARRAY_GetSubtitle);
#ifdef SKIP_ASM

/**
 * Returns a subtitle array's entry by index.
 *
 * Likely bug: the bound test is count < index, so index == count returns the address one past the last entry. Nothing
 * in the code or data calls it.
 *
 * @param i  the entry index
 * @return the entry (0x34-byte stride), or 0 when index > count
 */
extern "C" RCMP_AV_SUBTITLE* RCMP_AV_SUBTITLE_ARRAY_GetSubtitle(RCMP_AV_SUBTITLE_ARRAY* self, unsigned int i)
{
    if (self->mNumberOfSubtitle < i) {
        return 0;
    }
    return &(self->mSubTitle)[i];
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_ARRAY_FindSubtitle);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void* RCMP_AV_SUBTITLE_ARRAY_FindSubtitle_impl(void* self, unsigned int key) __asm__("RCMP_AV_SUBTITLE_ARRAY_FindSubtitle");
/**
 * Finds the subtitle shown at a frame: the last entry whose frame number is below it.
 *
 * Binary-searches the entries by their frame number (+0x14), which must be ascending, and clamps the result to the
 * array. Called by func_003AE718 with the movie player's current frame.
 *
 * @param key  the frame number
 * @return the entry, or 0 for an empty array
 */
extern "C" void* RCMP_AV_SUBTITLE_ARRAY_FindSubtitle_impl(void* self, unsigned int key)
{
    struct S { char* arr; int n; };
    S* subtitleArray = (S*)self;
    if (subtitleArray->n == 0) {
        return 0;
    }
    int hi = subtitleArray->n;
    char* arr = subtitleArray->arr;
    int lo = -1;
    while (hi - lo >= 2) {
        int mid = (hi + lo) / 2;
        if (key <= *(unsigned int*)(arr + mid * 0x34 + 0x14)) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    int i = hi - 1;
    if (i < 0) i = 0;
    i = (i < subtitleArray->n - 1) ? i : subtitleArray->n - 1;
    return arr + i * 52;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_ARRAY_GetNumberOfSubtitle);
#ifdef SKIP_ASM
/**
 * Returns a subtitle array's entry count.
 *
 * Nothing in the code or data calls it.
 *
 * @param table  the array
 * @return the count (+0x4)
 */
extern "C" int RCMP_AV_SUBTITLE_ARRAY_GetNumberOfSubtitle(void *table) {
    return (*(int *)((char*)(table) + (4)));
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF300);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern unsigned char D_00499C81[];
/**
 * Skips leading whitespace in a text span.
 *
 * Advances the text pointer past characters whose D_00499C81 class byte has bit 3 (space) set and shortens the length
 * to match. Used by func_003AF670 in the subtitle-file parser.
 *
 * @param pp  in/out: the text pointer
 * @param plen  in/out: the remaining length
 */
extern "C" void func_003AF300(char** pp, unsigned int* plen)
{
    unsigned int i;
    char* p = *pp;
    for (i = 0; i < *plen && ((D_00499C81 + p[i])[0] & 8) != 0; i++) {
    }
    *pp = p + i;
    *plen -= i;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF378);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern unsigned char D_00499C81[];

static inline int to_lower_3AF378(int c) { if (D_00499C81[c] & 1) return c + 0x20; return c; }
/**
 * Compares two strings ignoring case.
 *
 * Lowers characters whose D_00499C81 class byte has bit 0 (upper case) by adding 0x20. Quirk: the difference is summed,
 * and once a string ends it returns the raw difference of the last characters. Used by func_003AF428.
 *
 * @param lhs  first string
 * @param rhs  second string
 * @return 0 when equal, else the first nonzero difference
 */
extern "C" int func_003AF378(const char* lhs, const char* rhs)
{
    int i;
    int diff;
    diff = 0;
    i = 0;
    while (lhs[i] != 0 && rhs[i] != 0) {
        diff += to_lower_3AF378(lhs[i]) - to_lower_3AF378(rhs[i]);
        if (diff != 0) {
            return diff;
        }
        i++;
    }
    return lhs[i] - rhs[i];
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF428);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sNames_F428 { const char* e[5]; };
struct sVals_F428 { unsigned int e[5]; };
extern const sNames_F428 D_00495290;
extern const sVals_F428 D_004952A8;
extern "C" int func_003AF378(const char*, const char*) __asm__("func_003AF378");

/**
 * Parses a justification keyword (RIGHT, TOP, CENTER, LEFT, BOTTOM) into its value.
 *
 * Temporarily ends the word at the first tab or carriage return, compares it case-insensitively (func_003AF378) with
 * the names in D_00495290 and returns the matching value from D_004952A8 (0..4, the order of On Tour's AV_SUBTITLE
 * JUSTIFY_ENUM), then restores the character. Used by the subtitle-file parser func_003AF7F0 for JUSTIFY_X and
 * JUSTIFY_Y.
 *
 * @param text  the text, modified temporarily
 * @param len  its length
 * @return the justification value, or 0xFFFFFFFF for an unknown word
 */
extern "C" unsigned int func_003AF428(char* text, unsigned int len)
{
    unsigned int i;
    char ch = 9;
    unsigned int justify;
    unsigned int j;

    for (i = 0; i < len; i++) {
        char curChar = text[i];
        if (curChar == 9 || curChar == 13) {
            text[i] = 0;
            ch = curChar;
            break;
        }
    }
    sNames_F428 names = D_00495290;
    sVals_F428 vals = D_004952A8;
    justify = 0xFFFFFFFF;
    for (j = 0; j < 5; j++) {
        if (func_003AF378(names.e[j], text) == 0) {
            justify = vals.e[j];
            break;
        }
    }
    if (i != len) {
        text[i] = ch;
    }
    return justify;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF560);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long strtol(unsigned char *s, unsigned char **end, int base);

/**
 * Parses a decimal integer from a text span and advances past it.
 *
 * Temporarily ends the text at the first tab or newline, converts with strtol (base 10), shortens the length by the
 * characters read, then restores the character. Quirk: the restore compares the found position with the already
 * shortened length. Used by the subtitle-file parser func_003AF7F0 for numeric keywords.
 *
 * @param pp  in: the text pointer (not advanced)
 * @param plen  in/out: the remaining length
 * @param out  receives the value
 * @return 1 when a number was read, else 0
 */
extern "C" int func_003AF560(unsigned char **pp, unsigned int *plen, int *out) {
    unsigned int i;
    int ch = 9;
    unsigned char *end;
    long value;

    for (i = 0; i < *plen; i++) {
        unsigned char curChar = (*pp)[i];
        if ((unsigned int)(curChar - 9) < 2U) {
            ch = (signed char)curChar;
            (*pp)[i] = 0;
            break;
        }
    }
    value = strtol(*pp, &end, 10);
    *plen = *plen - (end - *pp);
    if (i != *plen) {
        (*pp)[i] = (unsigned char)ch;
    }
    if (end == *pp) {
        return 0;
    }
    *out = (int)value;
    return 1;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF670);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void func_003AF300(char** pp, unsigned int* plen);
/**
 * Finds the first of several keywords in a text span and moves past it.
 *
 * Skips leading whitespace (func_003AF300), then for each position tries every keyword in order; on the first match it
 * sets the text pointer just after it, shortens the length and reports the keyword index. Without a match (or without
 * text) it clears pointer and length and reports 0xFFFFFFFF. Used by the subtitle-file parser func_003AF7F0.
 *
 * @param ps  in/out: the text pointer
 * @param plen  in/out: the remaining length
 * @param words  the keywords
 * @param lens  their lengths
 * @param nwords  number of keywords
 * @param outIdx  receives the matched index or 0xFFFFFFFF
 */
extern "C" void func_003AF670(char** ps, unsigned int* plen, char** words, unsigned int* lens, unsigned int nwords, unsigned int* outIdx)
{
    char* text = *ps;
    if (text == 0) {
        *ps = 0;
        *plen = 0;
        *outIdx = 0xFFFFFFFF;
        return;
    }
    func_003AF300(&text, plen);
    for (unsigned int i = 0; i < *plen; i++) {
        for (unsigned int k = 0; k < nwords; k++) {
            unsigned int j;
            for (j = 0; j < lens[k] && i + j < *plen && words[k][j] == text[i + j]; j++) {
            }
            if (j == lens[k]) {
                unsigned int consumed = i + j;
                *ps = text + consumed;
                *plen -= consumed;
                *outIdx = k;
                return;
            }
        }
    }
    *ps = 0;
    *plen = 0;
    *outIdx = 0xFFFFFFFF;
}
#endif
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF7F0);
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_AV_SUBTITLE);
#ifdef SKIP_ASM
/**
 * Constructs a subtitle entry with its defaults.
 *
 * Position (0x20, 350), size 600 x 50, string id and frame number 0xFFFFFFFF, both justifications 2 (CENTER), shape
 * index 0, colour bytes A, R, G, B 0xFF, and the texture, string and wrapped-string pointers (+0x28..+0x30) null. Used
 * by RCMP_AV_SUBTITLE_ARRAY_Init and the subtitle-file parser func_003AF7F0.
 *
 * @return self
 */
extern "C" void *RCMP_AV_SUBTITLE_AV_SUBTITLE(RCMP_AV_SUBTITLE* self) {
    self->mPosX = 0x20;
    self->mPosY = 350;
    self->mWidth = 600;
    self->mHeight = 50;
    self->mStrID = 0xFFFFFFFFu;
    self->mA = 0xFF;
    self->mR = 0xFF;
    self->mG = 0xFF;
    self->mJustifyX = 2;
    self->mFrameNumber = 0xFFFFFFFFu;
    self->mB = 0xFF;
    self->mJustifyY = 2;
    self->mShapeIndex = 0;
    self->mTexturePtr = 0;
    self->mStringPtr = 0;
    self->mWrapStringPtr = 0;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetPosX);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's x position.
 *
 * Stores the value at +0x0 (mPosX). Called by the subtitle-file parser func_003AF7F0 for the keyword X, and by
 * RCMP_AV_SUBTITLE_ARRAY_Init.
 *
 * @param entry  the AV_SUBTITLE entry
 * @param value  the x position
 */
extern "C" void RCMP_AV_SUBTITLE_SetPosX(int *entry, int value) {
    *entry = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetPosY);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's y position.
 *
 * Stores the value at +0x4 (mPosY). Called by the subtitle-file parser func_003AF7F0 for the keyword Y, and by
 * RCMP_AV_SUBTITLE_ARRAY_Init.
 *
 * @param entry  the AV_SUBTITLE entry
 * @param value  the y position
 */
extern "C" void RCMP_AV_SUBTITLE_SetPosY(void *entry, int value) {
    (*(int *)((char*)(entry) + (4))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetWidth);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's width.
 *
 * Stores the value at +0x8 (mWidth). Called by the subtitle-file parser func_003AF7F0 for the keyword WIDTH, and by
 * RCMP_AV_SUBTITLE_ARRAY_Init.
 *
 * @param entry  the AV_SUBTITLE entry
 * @param value  the width
 */
extern "C" void RCMP_AV_SUBTITLE_SetWidth(void *entry, int value) {
    (*(int *)((char*)(entry) + (8))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetHeight);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's height.
 *
 * Stores the value at +0xC (mHeight). Called by the subtitle-file parser func_003AF7F0 for the keyword HEIGHT, and by
 * RCMP_AV_SUBTITLE_ARRAY_Init.
 *
 * @param entry  the AV_SUBTITLE entry
 * @param value  the height
 */
extern "C" void RCMP_AV_SUBTITLE_SetHeight(void *entry, int value) {
    (*(int *)((char*)(entry) + (0xC))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetStrID);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's string id.
 *
 * Stores the value at +0x10 (mStrID). Called by the subtitle-file parser func_003AF7F0 for the keyword STRINGID, and by
 * RCMP_AV_SUBTITLE_ARRAY_Init.
 *
 * @param entry  the AV_SUBTITLE entry
 * @param index  the string id (RCMP_AV_SUBTITLE_ARRAY_Init passes the entry index)
 */
extern "C" void RCMP_AV_SUBTITLE_SetStrID(void *entry, int index) {
    (*(int *)((char*)(entry) + (0x10))) = index;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetFrameNumber);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's first frame.
 *
 * Stores the value at +0x14 (mFrameNumber). Called by the subtitle-file parser func_003AF7F0 for the keyword FRAME, and
 * by RCMP_AV_SUBTITLE_ARRAY_Init.
 *
 * @param entry  the AV_SUBTITLE entry
 * @param value  the frame number from which it shows
 */
extern "C" void RCMP_AV_SUBTITLE_SetFrameNumber(void *entry, int value) {
    (*(int *)((char*)(entry) + (0x14))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetJustifyX);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's horizontal justification.
 *
 * Stores the value at +0x18 (mJustifyX). Called by the subtitle-file parser func_003AF7F0 for the keyword JUSTIFY_X
 * (value parsed by func_003AF428).
 *
 * @param entry  the AV_SUBTITLE entry
 * @param value  the justification (0 RIGHT, 1 TOP, 2 CENTER, 3 LEFT, 4 BOTTOM)
 */
extern "C" void RCMP_AV_SUBTITLE_SetJustifyX(void *entry, int value) {
    (*(int *)((char*)(entry) + (0x18))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetJustifyY);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's vertical justification.
 *
 * Stores the value at +0x1C (mJustifyY). Called by the subtitle-file parser func_003AF7F0 for the keyword JUSTIFY_Y
 * (value parsed by func_003AF428).
 *
 * @param entry  the AV_SUBTITLE entry
 * @param value  the justification (0 RIGHT, 1 TOP, 2 CENTER, 3 LEFT, 4 BOTTOM)
 */
extern "C" void RCMP_AV_SUBTITLE_SetJustifyY(void *entry, int value) {
    (*(int *)((char*)(entry) + (0x1C))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetShapeIndex);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's shape index.
 *
 * Stores the value at +0x20 (mShapeIndex). Called by the subtitle-file parser func_003AF7F0 for the keyword SHAPE.
 *
 * @param entry  the AV_SUBTITLE entry
 * @param value  the shape index
 */
extern "C" void RCMP_AV_SUBTITLE_SetShapeIndex(void *entry, int value) {
    (*(int *)((char*)(entry) + (0x20))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetA);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's alpha byte.
 *
 * Stores the value at +0x24 (mA). Called by the subtitle-file parser func_003AF7F0 for the keyword A.
 *
 * @param entry  the AV_SUBTITLE entry
 * @param value  the alpha value (low byte stored)
 */
extern "C" void RCMP_AV_SUBTITLE_SetA(void *entry, signed char value) {
    (*(signed char *)((char*)(entry) + (0x24))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetR);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's red byte.
 *
 * Stores the value at +0x25 (mR). Called by the subtitle-file parser func_003AF7F0 for the keyword R.
 *
 * @param entry  the AV_SUBTITLE entry
 * @param value  the red value (low byte stored)
 */
extern "C" void RCMP_AV_SUBTITLE_SetR(void *entry, signed char value) {
    (*(signed char *)((char*)(entry) + (0x25))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetG);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's green byte.
 *
 * Stores the value at +0x26 (mG). Called by the subtitle-file parser func_003AF7F0 for the keyword G.
 *
 * @param entry  the AV_SUBTITLE entry
 * @param value  the green value (low byte stored)
 */
extern "C" void RCMP_AV_SUBTITLE_SetG(void *entry, signed char value) {
    (*(signed char *)((char*)(entry) + (0x26))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_AV_SUBTITLE_SetB);
#ifdef SKIP_ASM
/**
 * Sets a subtitle entry's blue byte.
 *
 * Stores the value at +0x27 (mB). Called by the subtitle-file parser func_003AF7F0 for the keyword B.
 *
 * @param entry  the AV_SUBTITLE entry
 * @param value  the blue value (low byte stored)
 */
extern "C" void RCMP_AV_SUBTITLE_SetB(void *entry, signed char value) {
    (*(signed char *)((char*)(entry) + (0x27))) = value;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_RCMP_SYSTEM_RCMP_SYSTEM__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the RCMP system object with no allocator functions.
 *
 * Clears the alloc and free function pointers (+0x0, +0x4) and installs the vtable RCMP_RCMP_SYSTEM_vtbl at +0xC. The static
 * initialiser func_003B0770 builds the global RCMP_rcmp_sys with it; that global's alloc (+0x0), free (+0x4, D_00509434)
 * and memory flags (+0x8) are what the movie player and decoder allocate with.
 *
 * @return self
 */
void* RCMP_RCMP_SYSTEM_RCMP_SYSTEM(void* vself)
{
    RCMP_RCMP_SYSTEM* self = (RCMP_RCMP_SYSTEM*)vself;
    int zero = 0;
    *(int*)self = zero;
    self->mVtbl = (void**)((int)(void*)RCMP_RCMP_SYSTEM_vtbl);
    self->FreeMemFunc = (void*(*)(void*))zero;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_DECODER_DECODER);
#ifdef SKIP_ASM
void* RCMP_CODEC_IDATA_CODEC_IDATA(void* self);
extern void* RCMP_DECODER_vtbl[];

struct RCMP_DECODER_DECODER_sKey {
    int v[4];
};

struct RCMP_DECODER_DECODER_sObj {
    int field_0x0;
    RCMP_DECODER_DECODER_sKey key;   // 0x4
    int field_0x14;
    void** vtable;            // 0x18
};

/**
 * Constructs an RCMP DECODER from its input description.
 *
 * Installs the vtable RCMP_DECODER_vtbl, constructs and then copies the 16-byte CODEC_IDATA at +0x4 (RCMP_CODEC_IDATA_CODEC_IDATA), and
 * clears the codec (+0x14) and first chunk (+0x0). func_003AE450 allocates it for the movie player.
 *
 * The DECODER is 0x1C bytes: +0x0 first CHUNK, +0x4 CODEC_IDATA (streamer, get-data function, release function, frame
 * count), +0x14 the chosen codec, +0x18 vptr (RCMP_DECODER_vtbl).
 *
 * @param key  the CODEC_IDATA to copy
 * @return self
 */
extern "C" RCMP_DECODER_DECODER_sObj* RCMP_DECODER_DECODER(RCMP_DECODER_DECODER_sObj* self, RCMP_DECODER_DECODER_sKey* key)
{
    self->vtable = RCMP_DECODER_vtbl;
    RCMP_CODEC_IDATA_CODEC_IDATA(&self->key);
    self->key = *key;
    self->field_0x14 = 0;
    self->field_0x0 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_DECODER__DECODER);
#ifdef SKIP_ASM
extern "C" void RCMP_DECODER_FreeChosenCodec(RCMP_DECODER* self);
extern void* (*D_00509434[])(void*);
extern void* RCMP_DECODER_vtbl[];

/**
 * Destroys an RCMP DECODER, freeing its chosen codec.
 *
 * Reinstalls RCMP_DECODER_vtbl, calls RCMP_DECODER_FreeChosenCodec, and frees the object (D_00509434) when flags bit 0 is set. Slot 1 of the
 * DECODER vtable; func_003AE300 deletes the player's decoder through it.
 *
 * @param flags  bit 0 frees the object itself
 */
extern "C" void RCMP_DECODER__DECODER(RCMP_DECODER* self, int flags)
{
    self->mVtbl = RCMP_DECODER_vtbl;
    RCMP_DECODER_FreeChosenCodec(self);
    if (flags & 1) {
        D_00509434[0](self);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_DECODER_ChooseCodec);
#ifdef SKIP_ASM
class RCMP_DECODER_ChooseCodec_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void* a, int b);
    virtual void v03();
    virtual int v04();
    virtual float v05();
};

/**
 * Makes a codec the DECODER's codec and initialises it with the first chunk.
 *
 * Stores the codec (+0x14) and chunk (+0x0) and calls the codec's slot 2 with (decoder, chunk); the result passes
 * through in $v0 although the declaration is void. func_003AEAD0 calls it and frees the codec unless the result is 2.
 *
 * @param obj  the codec
 * @param firstChunk  the first CHUNK
 */
extern "C" void RCMP_DECODER_ChooseCodec(RCMP_DECODER* self, RCMP_DECODER_ChooseCodec_cObj* obj, int firstChunk)
{
    self->m_codec = (RCMP_CODEC*)obj;
    *(int*)self = firstChunk;
    obj->v02(self, firstChunk);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_DECODER_FreeChosenCodec);
#ifdef SKIP_ASM

/**
 * Deletes the DECODER's codec, if any.
 *
 * Calls the codec's slot-1 destructor with flags 3 and clears +0x14. Called by RCMP_DECODER__DECODER and func_003AEAD0.
 */
extern "C" void RCMP_DECODER_FreeChosenCodec(RCMP_DECODER* self)
{
    void* obj = self->m_codec;
    if (obj != 0) {
        sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)obj;
        vt[1].fn(vthis(obj, vt[1]), 3);
        self->m_codec = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_DECODER_GetCurrentFrameNumber);
#ifdef SKIP_ASM
/**
 * Returns the DECODER's current frame number.
 *
 * Calls the codec's slot 4 (PS2_SONY_CODEC_INTERNAL_GetCurrentFrameNumber for the Sony codec). Called by func_003AE6C8.
 *
 * @return the frame number, or 0 without a codec
 */
extern "C" int RCMP_DECODER_GetCurrentFrameNumber(RCMP_DECODER* self)
{
    RCMP_DECODER_ChooseCodec_cObj* obj = (RCMP_DECODER_ChooseCodec_cObj*)self->m_codec;
    if (obj == 0) {
        return 0;
    }
    return obj->v04();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_DECODER_GetFrameRate);
#ifdef SKIP_ASM
/**
 * Returns the DECODER's frame rate.
 *
 * Calls the codec's slot 5 (PS2_SONY_CODEC_INTERNAL_GetFrameRate for the Sony codec). Called by func_003AE780.
 *
 * @return frames per second, or 0.0f without a codec
 */
extern "C" float RCMP_DECODER_GetFrameRate(RCMP_DECODER* self)
{
    RCMP_DECODER_ChooseCodec_cObj* obj = (RCMP_DECODER_ChooseCodec_cObj*)self->m_codec;
    if (obj == 0) {
        return 0.0f;
    }
    return obj->v05();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_DECODER_GetFrame);
#ifdef SKIP_ASM
class RCMP_DECODER_GetFrame_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int v03(int);
};

struct RCMP_DECODER_GetFrame_sObj {
    int loaded;                                    // 0x0
    void* arg;                                     // 0x4
    void (*load)(RCMP_DECODER_GetFrame_sObj*, void*, RCMP_DECODER_GetFrame_sObj*); // 0x8
    char pad_0xC[0x8];
    RCMP_DECODER_GetFrame_cVirt* obj;                      // 0x14
};

/**
 * Returns the decoder's frame for a goal frame number, fetching the first chunk if needed.
 *
 * Without a codec and a first chunk it calls the get-data function (+0x8) as (decoder, streamer, &first chunk), which
 * for the movie player also chooses the codec (func_003AEAD0). Then it asks the codec's slot 3 for the frame. Called by
 * func_003AE450 and func_003AE6C8.
 *
 * @param decoder  the DECODER
 * @param goalFrame  the goal frame number
 * @return the frame, or 0 without a codec
 */
extern "C" int RCMP_DECODER_GetFrame(void* decoder, int goalFrame)
{
    RCMP_DECODER_GetFrame_sObj* self = (RCMP_DECODER_GetFrame_sObj*)decoder;
    if (self->obj == 0) {
        if (self->loaded == 0) {
            self->load(self, self->arg, self);
        }
    }
    int frame;
    if (self->obj != 0) {
        frame = self->obj->v03(goalFrame);
    } else {
        frame = 0;
    }
    return frame;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_DECODER_ReleaseFrame);
#ifdef SKIP_ASM
class RCMP_DECODER_ReleaseFrame_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int a);
};

/**
 * Gives a frame back to the decoder's codec.
 *
 * Calls the codec's slot 6 without checking that a codec exists. The game's movie code (cMoviePlayer_purge, func_002547B8,
 * SSX_VD_PS2_SINGLE_BUFFERD_CompleteDraw) calls it on frames it got from func_003AE6C8.
 *
 * @param frame  the frame
 */
extern "C" void RCMP_DECODER_ReleaseFrame(RCMP_DECODER* self, int frame)
{
    ((RCMP_DECODER_ReleaseFrame_cObj*)self->m_codec)->v06(frame);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_DECODER_GetChunk);
#ifdef SKIP_ASM
struct RCMP_DECODER_GetChunk_sReq {
    int pending;
    int arg;
    void (*fn)(RCMP_DECODER_GetChunk_sReq* self, int arg, int* out);
};

/**
 * Returns the decoder's next CHUNK of input.
 *
 * Hands out the stored first chunk (+0x0) once, clearing it; afterwards it calls the get-data function (+0x8) as
 * (decoder, streamer, &chunk). Called by the Sony codec's feeder func_003B0B40.
 *
 * @return the CHUNK, or 0 when there is no more data
 */
extern "C" int RCMP_DECODER_GetChunk(RCMP_DECODER_GetChunk_sReq* self)
{
    int chunk;
    int pendingChunk = self->pending;
    if (pendingChunk != 0) {
        self->pending = 0;
        chunk = pendingChunk;
        return pendingChunk;
    }
    self->fn(self, self->arg, &chunk);
    return chunk;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_DECODER_ReleaseChunk);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sWSCb06F8 {
    int pad0;
    void* ctx;
    int pad8;
    void (*fn)(sWSCb06F8* self, void* ctx, int arg);
};

// The unit later declares RCMP_DECODER_ReleaseChunk(void*, void*) for its caller; bind this definition by asm label.
void RCMP_DECODER_ReleaseChunk_impl(sWSCb06F8* self, int arg) __asm__("RCMP_DECODER_ReleaseChunk");
/**
 * Gives a CHUNK back through the decoder's release function.
 *
 * Calls the release function (+0xC) as (decoder, streamer (+0x4), chunk). Called by func_003B0B40.
 *
 * @param chunk  the CHUNK
 */
void RCMP_DECODER_ReleaseChunk_impl(sWSCb06F8* self, int chunk)
{
    self->fn(self, self->ctx, chunk);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_CHUNK_CHUNK__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an empty CHUNK (user data, data and size all 0).
 *
 * func_003AEAD0 allocates 12 bytes, constructs them with it and fills the fields.
 *
 * @return self
 */
void* RCMP_CHUNK_CHUNK(void* vself)
{
    RCMP_CHUNK* self = (RCMP_CHUNK*)vself;
    int zero = 0;
    self->m_DataToDecode = (void*)zero;
    self->m_DataToDecodeSize = zero;
    *(int*)self = zero;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_CODEC_IDATA_CODEC_IDATA__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an empty CODEC_IDATA with a frame count of 2.
 *
 * Clears the streamer, get-data and release functions (+0x0..+0x8) and sets the frame count (+0xC) to 2. Called by the
 * DECODER constructor RCMP_DECODER_DECODER for its +0x4 member.
 *
 * @return self
 */
void* RCMP_CODEC_IDATA_CODEC_IDATA(void* vself)
{
    RCMP_CODEC_IDATA* self = (RCMP_CODEC_IDATA*)vself;
    int zero = 0;
    *(int*)self = zero;
    self->m_MaxFramesOutstanding = 2;
    self->m_GetDataFunc = (void (*)(RCMP_DECODER*, RCMP_STREAMER*, RCMP_CHUNK**))zero;
    self->m_ReleaseDataFunc = (void (*)(RCMP_DECODER*, RCMP_STREAMER*, RCMP_CHUNK*))zero;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", RCMP_CODEC_IDATA_CODEC_IDATA1);
#ifdef SKIP_ASM
/**
 * Constructs a CODEC_IDATA from a streamer, its two callbacks and a frame count.
 *
 * func_003AE450 builds the movie player's input description with it.
 *
 * @param context  the streamer pointer passed back to the callbacks
 * @param fetchFn  the get-data function
 * @param releaseFn  the release-data function
 * @param framesToBuffer  number of frames the codec keeps
 * @return self
 */
extern "C" void* RCMP_CODEC_IDATA_CODEC_IDATA1(void* self, int context, int fetchFn, int releaseFn, int framesToBuffer)
{
    *(int*)((char*)self + 0xc) = framesToBuffer;
    *(int*)self = context;
    *(int*)((char*)self + 0x4) = fetchFn;
    *(int*)((char*)self + 0x8) = releaseFn;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0770);
#ifdef SKIP_ASM
extern char RCMP_rcmp_sys[];
extern void* D_0050943C[];

/**
 * Constructs or tears down the global RCMP system object RCMP_rcmp_sys (static initialiser).
 *
 * For priority 0xFFFF: on init it runs RCMP_RCMP_SYSTEM_RCMP_SYSTEM__FPv on RCMP_rcmp_sys; on exit it only resets the vtable word
 * (D_0050943C) to RCMP_RCMP_SYSTEM_vtbl. Called by func_003B07B8__FPv and func_003B07D8__FPv.
 *
 * @param init  nonzero to construct, 0 to destroy
 * @param prio  the initialisation priority; only 0xFFFF acts
 * @return nothing meaningful (no return statement)
 */
extern "C" void* func_003B0770(int init, int prio)
{
    if (prio == 0xFFFF) {
        if (init != 0) {
            RCMP_RCMP_SYSTEM_RCMP_SYSTEM(RCMP_rcmp_sys);
        } else {
            D_0050943C[0] = (void*)RCMP_RCMP_SYSTEM_vtbl;
        }
    }
}
#endif

extern "C" void* func_003B0770(int, int);

//99.38%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B07B8__FPv);
#ifdef SKIP_ASM
/**
 * Runs the unit's static constructors (func_003B0770(1, 0xFFFF)).
 *
 * Referenced from the constructor table in data.
 *
 * @return func_003B0770's result
 */
void* func_003B07B8(void* self)
{
    return func_003B0770(1, 0xffff);
}
#endif

extern "C" void* func_003B0770(int, int);

//99.38%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B07D8__FPv);
#ifdef SKIP_ASM
/**
 * Runs the unit's static destructors (func_003B0770(0, 0xFFFF)).
 *
 * Referenced from the destructor table in data.
 *
 * @return func_003B0770's result
 */
void* func_003B07D8(void* self)
{
    return func_003B0770(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B07F8);
#ifdef SKIP_ASM
extern "C" void* SHAPE_create(unsigned int w, int h, int bpp, int a3, int a4, int flags, int a6, int a7);
extern "C" void* func_003B3D40(void* img);
extern "C" void MEM_fill(void* dst, int value, int size);
extern int D_00509438[];

static inline char* func_003B07F8_data(char* p)
{
    if (*(int*)(p + 0xC) & 0x1000) {
        return p + *(int*)(p + 0x10);
    }
    return p + 0x10;
}

/**
 * Constructs a 20-byte video frame with a cleared 32-bit SHAPE of the given size.
 *
 * Sets the frame type (+0x0) to 2, the list links (+0x8, +0xC) to 3 and the reference count (+0x10) to 0, creates the
 * image with SHAPE_create(w, h, 32 bpp, ..., memory type D_00509438, ..., 0xF0) at +0x4, writes "240" into bytes 4..6
 * of the header func_003B3D40 returns, zeroes the pixels ((w/16) * (h/16) macroblocks of 1 KB) and sets the type byte
 * to 5. Called by the Sony codec's Init (PS2_SONY_CODEC_INTERNAL_Init) and PS2_SONY_CODEC_INTERNAL_CreateFrame.
 *
 * @param w  width in pixels
 * @param height  height in pixels
 * @return self
 */
extern "C" void* func_003B07F8(RCMP_FRAME* self, unsigned int w, int height)
{
    self->m_FrameType = 2;
    *(int*)((char*)self + 0x8) = 3;
    *(int*)((char*)self + 0xC) = 3;
    *(int*)((char*)self + 0x10) = 0;
    char* img = (char*)SHAPE_create(w, height, 0x20, 0, 0, D_00509438[0], 0, 0xF0);
    self->m_Shp = (SHAPE*)img;
    char* hdr = (char*)func_003B3D40(img);
    hdr[4] = 50;
    hdr[5] = 52;
    hdr[6] = 0x30;
    unsigned int size = ((w >> 4) * height >> 4) << 10;
    MEM_fill(func_003B07F8_data((char*)self->m_Shp), 0, size);
    *(char*)self->m_Shp = 5;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B08E0);
#ifdef SKIP_ASM
extern "C" void SHAPE_destroy(char* buf);
extern void* (*D_00509434[])(void*);

/**
 * Destroys a video frame and its SHAPE.
 *
 * Frees the image (+0x4) with SHAPE_destroy, marks the list links (+0x8, +0xC) with 7 and frees the frame (D_00509434)
 * when flags bit 0 is set. Called by the Sony codec's destructor PS2_SONY_CODEC_INTERNAL__PS2_SONY_CODEC_INTERNAL.
 *
 * @param flags  bit 0 frees the frame itself
 */
extern "C" void func_003B08E0(RCMP_FRAME* self, int flags)
{
    if (self->m_Shp != 0) {
        SHAPE_destroy((char*)self->m_Shp);
        self->m_Shp = 0;
    }
    *(int*)((char*)self + 0x8) = 7;
    *(int*)((char*)self + 0xC) = 7;
    if (flags & 1) {
        D_00509434[0](self);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", PS2_SONY_CODEC_INTERNAL_PS2_SONY_CODEC_INTERNAL);
#ifdef SKIP_ASM
extern void* PS2_SONY_CODEC_INTERNAL_vtbl[];

struct sWSNode2 {
    sWSNode2* next;
    sWSNode2* prev;
};

static inline void wsListInit(sWSNode2* h)
{
    h->next = (sWSNode2*)3;
    h->prev = (sWSNode2*)3;
    h->next = h;
    h->prev = h;
    h->next = h;
    h->prev = h;
}

/**
 * Constructs the Sony MPEG codec object with empty frame lists.
 *
 * Installs the vtable PS2_SONY_CODEC_INTERNAL_vtbl, makes the used (+0x18) and free (+0x20) frame lists empty and clears the work area
 * (+0x2C) and chunk buffer (+0x78). func_003B11A0 allocates it.
 *
 * The Sony codec object is 0x80 bytes: +0x0 vptr, +0x4 state, +0x8/+0xC coded width/height, +0x10 current frame number,
 * +0x14 chunks fed, +0x18 used frames, +0x20 free frames, +0x28 the DECODER, +0x2C MPEG work area, +0x30 the 0x48-byte
 * MPEG decoder state, +0x78 chunk buffer, +0x7C its uncached alias.
 *
 * @return self
 */
extern "C" void* PS2_SONY_CODEC_INTERNAL_PS2_SONY_CODEC_INTERNAL(PS2_SONY_CODEC_INTERNAL* self)
{
    *(void***)self = PS2_SONY_CODEC_INTERNAL_vtbl;
    wsListInit((sWSNode2*)(&self->mUsedFrames));
    wsListInit((sWSNode2*)(&self->mFreeFrames));
    self->mpWorkBuffer = 0;
    self->mpChunkBuffer = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", PS2_SONY_CODEC_INTERNAL__PS2_SONY_CODEC_INTERNAL);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern void* RCMP_CODEC_vtbl[];

struct sWSList_09A0 {
    sWSNode2* next;
    sWSNode2* prev;
    int empty() { return next == (sWSNode2*)this; }
    sWSNode2* front() { return next; }
    void kill() { next = (sWSNode2*)7; prev = (sWSNode2*)7; }
};


static inline void wsUnlink_09A0(sWSNode2* n)
{
    void* obj = n ? (char*)n - 8 : 0;
    sWSNode2* next = n->next;
    sWSNode2* prev = n->prev;
    prev->next = next;
    next->prev = prev;
    n->prev = (sWSNode2*)0xB;
    n->next = (sWSNode2*)0xB;
    if (obj) func_003B08E0((RCMP_FRAME*)obj, 3);
}

/**
 * Destroys the Sony MPEG codec object: buffers, frames and lists.
 *
 * Frees the work area (+0x2C) and chunk buffer (+0x78), unlinks and destroys every frame on the used (+0x18) and free
 * (+0x20) lists with func_003B08E0, marks the list heads with 7, ends with the base CODEC vtable RCMP_CODEC_vtbl and frees
 * the object when flags bit 0 is set. Slot 1 of PS2_SONY_CODEC_INTERNAL_vtbl.
 *
 * @param flags  bit 0 frees the object itself
 */
extern "C" void PS2_SONY_CODEC_INTERNAL__PS2_SONY_CODEC_INTERNAL(PS2_SONY_CODEC_INTERNAL* self, int flags)
{
    self->mVtbl = PS2_SONY_CODEC_INTERNAL_vtbl;
    if (self->mpWorkBuffer != 0) {
        D_00509434[0](self->mpWorkBuffer);
        self->mpWorkBuffer = 0;
    }
    if (self->mpChunkBuffer != 0) {
        D_00509434[0](self->mpChunkBuffer);
        self->mpChunkBuffer = 0;
    }
    while (!(*(sWSList_09A0*)&self->mUsedFrames).empty()) wsUnlink_09A0((*(sWSList_09A0*)&self->mUsedFrames).front());
    while (!(*(sWSList_09A0*)&self->mFreeFrames).empty()) wsUnlink_09A0((*(sWSList_09A0*)&self->mFreeFrames).front());
    (*(sWSList_09A0*)&self->mFreeFrames).kill();
    (*(sWSList_09A0*)&self->mUsedFrames).kill();
    self->mVtbl = RCMP_CODEC_vtbl;
    if (flags & 1) D_00509434[0](self);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0B10);
#ifdef SKIP_ASM
extern "C" void func_003B0B40(void* a, void* b, void* c);

/**
 * Feeds the MPEG decoder more input when it runs out (decoder callback).
 *
 * Registered by PS2_SONY_CODEC_INTERNAL_Init through sceMpegAddCallback with type 1; calls func_003B0B40(codec, decoder state,
 * callbackArg).
 *
 * @param callbackArg  the callback's second argument (passed on unused)
 * @param source  the Sony codec object (callback data)
 * @return 1
 */
extern "C" int func_003B0B10(void* self, void* callbackArg, void* source)
{
    func_003B0B40(source, self, callbackArg);
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0B40);
#ifdef SKIP_ASM
extern "C" void RCMP_DECODER_ReleaseChunk(void* req, void* blk);
extern "C" void MEM_copy(void* dst, void* src, int size);
extern "C" void sceMpegAddBs(void* dst, void* src, int size);

struct func_003B0B40_sBlock {
    int f0;
    void* data;
    int size;
};

struct func_003B0B40_sScript {
    char pad00[0x14];
    int count;
    char pad18[0x10];
    RCMP_DECODER_GetChunk_sReq* req;
    char pad2C[0x4C];
    void* hdr;
    char* buf;
};

/**
 * Copies the next input CHUNK into the codec's chunk buffer and hands it to the MPEG decoder.
 *
 * Gets a CHUNK from the DECODER (+0x28) with RCMP_DECODER_GetChunk. With data it copies size - 8 bytes into the uncached buffer
 * (+0x7C) and zero-pads to the 16-byte-rounded size plus 16; without data it writes four 0xB7010000 words (MPEG
 * sequence end codes) so the decoder stops. It releases the CHUNK (RCMP_DECODER_ReleaseChunk), counts it (+0x14) and passes the
 * buffer (+0x78) and size to sceMpegAddBs.
 *
 * @param source  the Sony codec object
 * @param consumer  the MPEG decoder state passed to sceMpegAddBs
 * @param callbackArg  the callback argument (unused)
 */
// PORT: RCMP_DECODER_GetChunk returns the block pointer as int.
extern "C" void func_003B0B40(void* source, void* consumer, void* callbackArg)
{
    func_003B0B40_sScript* self = (func_003B0B40_sScript*)source;
    void* data = 0;
    func_003B0B40_sBlock* blk = (func_003B0B40_sBlock*)RCMP_DECODER_GetChunk(self->req);
    if (blk != 0) {
        data = blk->data;
    }
    int total;
    int i;
    if (data != 0) {
        i = blk->size;
        total = (i + 7) & ~0xF;
        i -= 8;
        MEM_copy(self->buf, data, i);
        for (; i < total + 0x10; i += 4) {
            *(int*)(self->buf + i) = 0;
        }
    } else {
        total = 0x10;
        for (i = 0; i < 0x10; i += 4) {
            *(int*)(self->buf + i) = 0xB7010000;
        }
    }
    if (blk != 0) {
        RCMP_DECODER_ReleaseChunk(self->req, blk);
    }
    self->count++;
    sceMpegAddBs(consumer, self->hdr, total);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", PS2_SONY_CODEC_INTERNAL_Init);
#ifdef SKIP_ASM
struct sAlloc_B0C58 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* (*free)(void* p);
    int flags;
};
// PORT: the unit declares RCMP_rcmp_sys as char[] later; view bound by asm label
extern sAlloc_B0C58 D_00509430_B0C58 __asm__("RCMP_rcmp_sys");
// mpeg2decode: struct layer_data *ld (current layer), outside small data like the other MPEG globals
struct layer_data;
extern layer_data* ld __asm__("D_0050A088") __attribute__((section(".bss")));
extern int D_00509648[];
// MPEG decoder globals: plain scalars outside small data (the original TU kept them out of $gp)
#define BSS_B0C58 __attribute__((section(".bss")))
extern int D_005094F4_s __asm__("D_005094F4") BSS_B0C58;
extern int D_005094F8_s __asm__("D_005094F8") BSS_B0C58;
extern int D_005094FC_s __asm__("D_005094FC") BSS_B0C58;
extern int D_00509500_s __asm__("D_00509500") BSS_B0C58;
extern int D_00509524_s __asm__("D_00509524") BSS_B0C58;
extern int D_00509548_s __asm__("D_00509548") BSS_B0C58;
extern int D_00509584_s __asm__("D_00509584") BSS_B0C58;
extern int D_0050958C_s __asm__("D_0050958C") BSS_B0C58;
extern int D_005095A0_s __asm__("D_005095A0") BSS_B0C58;
extern int D_005094D4_s __asm__("D_005094D4") BSS_B0C58;
extern int D_005094D8_s __asm__("D_005094D8") BSS_B0C58;
extern int D_005094DC_s __asm__("D_005094DC") BSS_B0C58;
extern int D_005094E0_s __asm__("D_005094E0") BSS_B0C58;
extern char D_004953E8[];
extern char str_RCMP_mpegbuff[];
extern char str_RCMP_chunkbuff[];
extern "C" void Initialize_Buffer(void* data);
extern "C" int Get_Hdr(void);
extern "C" void* func_003B07F8(RCMP_FRAME* self, unsigned int w, int h);
extern "C" void sceMpegInit(void);
extern "C" void sceMpegCreate(void* a, void* b, int size);
extern "C" void* sceMpegAddCallback(void* a, int n, void* fn, void* arg);
extern "C" int func_003B0B10(void* self, void* a1, void* a2);

struct sWSNode_B0C58 {
    sWSNode_B0C58* next;
    sWSNode_B0C58* prev;
};


struct sData_B0C58 {
    int f0;
    void* data;
};


/**
 * Initialises the Sony MPEG codec from the first chunk: reads the headers, sizes and allocates frames.
 *
 * Stores the DECODER (+0x28), clears the counters, frees old buffers, sets the state to 1 and reads the stream headers
 * from the chunk's data (Initialize_Buffer, Get_Hdr) with ld pointing at the base layer D_00509648. For MPEG-1
 * streams (base +0x420 clear) it presets the MPEG-2 picture globals to progressive frame values. It computes the
 * macroblock counts and coded sizes (D_005094FC/D_00509500, D_005094D4/D_005094D8, chroma halves) into +0x8 and +0xC.
 *
 * It allocates as many frames as the DECODER's frame count (+0x10) with func_003B07F8 onto the free list (+0x20), an
 * "RCMP::mpegbuff" work area of w * h * 9 / 2 + 5996 bytes (+0x2C) and a 0x32000-byte "RCMP::chunkbuff" (+0x78,
 * uncached alias at +0x7C), both with flag 0x400, then sets the MPEG decoder up with sceMpegInit, sceMpegCreate and
 * registers func_003B0B10 with sceMpegAddCallback. Slot 2 of PS2_SONY_CODEC_INTERNAL_vtbl.
 *
 * @param script  the DECODER
 * @param firstChunk  the first CHUNK
 * @return 2 (usable)
 */
extern "C" int PS2_SONY_CODEC_INTERNAL_Init(PS2_SONY_CODEC_INTERNAL* self, RCMP_DECODER* script, sData_B0C58* firstChunk)
{
    (*(RCMP_DECODER**)&self->mpDecoder) = script;
    self->mCurrentFrameNumber = 0;
    self->unk_14 = 0;
    if (self->mpWorkBuffer != 0) {
        D_00509430_B0C58.free(self->mpWorkBuffer);
        self->mpWorkBuffer = 0;
    }
    if (self->mpChunkBuffer != 0) {
        D_00509430_B0C58.free(self->mpChunkBuffer);
        self->mpChunkBuffer = 0;
    }
    self->unk_04 = 1;
    void* data = firstChunk->data;
    ld = (layer_data*)D_00509648;
    Initialize_Buffer(data);
    Get_Hdr();
    int mode = D_00509648[1056 / 4];
    if (mode == 0) {
        D_00509524_s = 1;
        D_005095A0_s = 1;
        D_00509584_s = 3;
        D_0050958C_s = 1;
        D_00509548_s = 5;
    }
    D_005094FC_s = (D_005094F4_s + 15) / 16;
    D_00509500_s = (mode != 0 && D_00509524_s == 0) ? (D_005094F8_s + 31) / 32 * 2 : (D_005094F8_s + 15) / 16;
    int w = D_005094FC_s * 16;
    D_005094D4_s = w;
    self->mWidth = w;
    int codedHeight = D_00509500_s * 16;
    D_005094D8_s = codedHeight;
    self->mHeight = codedHeight;
    D_005094DC_s = w >> 1;
    D_005094E0_s = codedHeight >> 1;
    int n = ((int)(self->mpDecoder)->m_IData.m_MaxFramesOutstanding);
    for (int i = 0; i < n; i++) {
        int cw = self->mWidth;
        int ch = self->mHeight;
        char* newFrame = (char*)func_003B07F8((RCMP_FRAME*)D_00509430_B0C58.alloc(D_004953E8, 20, 0, 0, D_00509430_B0C58.flags), cw, ch);
        sWSNode_B0C58* node = newFrame ? (sWSNode_B0C58*)(newFrame + 8) : 0;
        sWSNode_B0C58* head = &(*(sWSNode_B0C58*)&self->mFreeFrames);
        sWSNode_B0C58* old = head->next;
        head->next = node;
        old->prev = node;
        node->next = old;
        node->prev = head;
    }
    self->mpWorkBuffer = D_00509430_B0C58.alloc(str_RCMP_mpegbuff, self->mWidth * self->mHeight * 9 / 2 + 5996, 0, 0, D_00509430_B0C58.flags | 0x400);
    self->mpChunkBuffer = D_00509430_B0C58.alloc(str_RCMP_chunkbuff, 0x32000, 0, 0, D_00509430_B0C58.flags | 0x400);
    (*(void**)&self->mpChunkBufferUncached) = (void*)(((unsigned int)self->mpChunkBuffer & 0x0FFFFFFF) | 0x30000000);
    sceMpegInit();
    sceMpegCreate(self->mMpeg, self->mpWorkBuffer, self->mWidth * self->mHeight * 9 / 2 + 5996);
    sceMpegAddCallback(self->mMpeg, 1, (void*)func_003B0B10, self);
    return 2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", PS2_SONY_CODEC_INTERNAL_GetCurrentFrameNumber__FPv);
#ifdef SKIP_ASM
/**
 * Returns the Sony codec's current frame number (+0x10).
 *
 * Slot 4 of PS2_SONY_CODEC_INTERNAL_vtbl; PS2_SONY_CODEC_INTERNAL_DecodeFrame increments the counter for every decoded picture.
 *
 * @return the number of pictures decoded since Init
 */
int PS2_SONY_CODEC_INTERNAL_GetCurrentFrameNumber(void* vself)
{
    PS2_SONY_CODEC_INTERNAL* self = (PS2_SONY_CODEC_INTERNAL*)vself;
    return (int)self->mCurrentFrameNumber;
}
#endif

// padded past the 8-byte gp-relative threshold so the compiler emits
// absolute lui/lo addressing like the target, instead of assuming small data
struct sD_00509508 { float value; int pad[2]; };
extern sD_00509508 D_00509508;

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", PS2_SONY_CODEC_INTERNAL_GetFrameRate);
#ifdef SKIP_ASM
/**
 * Returns the frame rate the last MPEG sequence extension set (D_00509508).
 *
 * Slot 5 of PS2_SONY_CODEC_INTERNAL_vtbl. The value is global, not per codec object, and only the sequence-extension parser sequence_extension
 * writes it, so an MPEG-1 stream keeps whatever was there before.
 *
 * @return frames per second
 */
extern "C" float PS2_SONY_CODEC_INTERNAL_GetFrameRate()
{
    return D_00509508.value;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", PS2_SONY_CODEC_INTERNAL_DecodeFrame);
#ifdef SKIP_ASM
struct sWSPool;
struct sWSItem;
extern "C" sWSItem* func_003B10D0(sWSPool* self);
extern "C" void sceMpegGetPicture(void* dma, void* data, int qwc);

/**
 * Decodes the next picture into a free frame and returns it.
 *
 * Takes a frame from the free list (func_003B10D0), finds its SHAPE pixel data (after the 16-byte header, or at the
 * header's offset when flag 0x1000 is set), has the MPEG decoder (+0x30) write the picture there with sceMpegGetPicture for
 * (width/16) * (height/16) macroblocks, and increments the frame number (+0x10). Likely bug: it does not check for an
 * empty free list before reading the frame. Called by PS2_SONY_CODEC_INTERNAL_GetFrame.
 *
 * @return the decoded frame
 */
extern "C" void* PS2_SONY_CODEC_INTERNAL_DecodeFrame(PS2_SONY_CODEC_INTERNAL* self)
{
    sWSItem* item = func_003B10D0((sWSPool*)self);
    char* hdr = *(char**)((char*)item + 0x4);
    char* data;
    if (*(int*)(hdr + 0xC) & 0x1000) {
        data = hdr + *(int*)(hdr + 0x10);
    } else {
        data = hdr + 0x10;
    }
    sceMpegGetPicture(self->mMpeg, data, self->mWidth / 16 * self->mHeight / 16);
    *(int*)&self->mCurrentFrameNumber += 1;
    return item;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", PS2_SONY_CODEC_INTERNAL_GetFrame);
#ifdef SKIP_ASM
extern "C" void* PS2_SONY_CODEC_INTERNAL_DecodeFrame(PS2_SONY_CODEC_INTERNAL* self);
extern "C" int sceMpegIsEnd(void* sema);

class PS2_SONY_CODEC_INTERNAL_GetFrame_cCache {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(void*);
};

/**
 * Decodes pictures until the frame number reaches the goal and returns the last one.
 *
 * Each pass first releases the previously decoded frame through the codec's slot 6, stops with 0 when sceMpegIsEnd
 * reports the MPEG decoder (+0x30) finished, and decodes with PS2_SONY_CODEC_INTERNAL_DecodeFrame. Slot 3 of PS2_SONY_CODEC_INTERNAL_vtbl.
 *
 * @param limit  the goal frame number
 * @return the frame for the goal, or 0 at the end of the stream
 */
extern "C" void* PS2_SONY_CODEC_INTERNAL_GetFrame(PS2_SONY_CODEC_INTERNAL* self, unsigned int limit)
{
    void* item = 0;
    do {
        if (item != 0) {
            ((PS2_SONY_CODEC_INTERNAL_GetFrame_cCache*)self)->v06(item);
        }
        if (sceMpegIsEnd(self->mMpeg) != 0) {
            return 0;
        }
        item = PS2_SONY_CODEC_INTERNAL_DecodeFrame(self);
    } while (self->mCurrentFrameNumber < limit);
    return item;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B10D0);
#ifdef SKIP_ASM
struct sWSNode {
    sWSNode* next;
    sWSNode* prev;

    void unlink()
    {
        sWSNode* n = next;
        sWSNode* p = prev;
        p->next = n;
        n->prev = p;
        prev = next = (sWSNode*)0xB;
    }
};

struct sWSList {
    sWSNode head;

    void addTail(sWSNode* n)
    {
        sWSNode* tail = head.prev;
        tail->next = n;
        head.prev = n;
        n->prev = tail;
        n->next = &head;
    }
};

struct sWSItemBase {
    int a;
    int b;
};

struct sWSItem : sWSItemBase, sWSNode {
    int refs; // 0x10
};

struct sWSPool {
    char pad00[0x18];
    sWSList used; // 0x18
    sWSList free; // 0x20
};

/**
 * Moves the first frame of the codec's free list to its used list and returns it.
 *
 * Sets the frame's reference count (+0x10) to 1. Called by PS2_SONY_CODEC_INTERNAL_DecodeFrame.
 *
 * @return the frame, or 0 when the free list (+0x20) is empty
 */
extern "C" sWSItem* func_003B10D0(sWSPool* self)
{
    sWSNode* n = self->free.head.next;
    if (n != &self->free.head) {
        sWSItem* item;
        n->unlink();
        item = static_cast<sWSItem*>(n);
        self->used.addTail(item);
        item->refs = 1;
        return item;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", PS2_SONY_CODEC_INTERNAL_ReleaseFrame);
#ifdef SKIP_ASM
/**
 * Drops one reference to a frame; at zero moves it from the used list back to the free list.
 *
 * Slot 6 of PS2_SONY_CODEC_INTERNAL_vtbl, reached through DECODER forwarder RCMP_DECODER_ReleaseFrame and from PS2_SONY_CODEC_INTERNAL_GetFrame.
 *
 * @param item  the frame
 */
extern "C" void PS2_SONY_CODEC_INTERNAL_ReleaseFrame(sWSPool* self, sWSItem* item)
{
    if (--item->refs > 0) {
        return;
    }
    item->unlink();
    self->free.addTail(item);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B11A0);
#ifdef SKIP_ASM
struct sAlloc_11A0 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* (*free)(void* p);
    int flags;
};
// PORT: the unit declares RCMP_rcmp_sys as char[] later; view bound by asm label
extern sAlloc_11A0 D_00509430_11A0 __asm__("RCMP_rcmp_sys");
/**
 * Allocates and constructs a Sony MPEG codec object (0x80 bytes).
 *
 * Allocates through RCMP_rcmp_sys with an empty tag (D_004953E8) and runs PS2_SONY_CODEC_INTERNAL_PS2_SONY_CODEC_INTERNAL. func_003AEAD0 offers the result
 * to the DECODER with RCMP_DECODER_ChooseCodec.
 *
 * @return the new codec
 */
extern "C" void* func_003B11A0(void)
{
    return PS2_SONY_CODEC_INTERNAL_PS2_SONY_CODEC_INTERNAL((PS2_SONY_CODEC_INTERNAL*)D_00509430_11A0.alloc(D_004953E8, 0x80, 0, 0, D_00509430_11A0.flags));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", Error__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the parser's error report, with its output removed.
 *
 * Called with an error text by sequence_scalable_extension ("temporal scalability not implemented") and picture_temporal_scalable_extension
 * ("temporal scalability not supported").
 */
void Error(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B11F0);
#ifdef SKIP_ASM
extern int D_00495448[];

/**
 * Tells whether a chunk tag belongs to the MPEG codec ("MPCh" or "MPCd").
 *
 * Compares with the two tags in D_00495448. Called by func_003AEAD0 on each stream block's big-endian tag.
 *
 * @param chunkTag  the four-character tag as a big-endian word
 * @return 1 for an MPEG chunk, else 0
 */
extern "C" int func_003B11F0(int chunkTag)
{
    unsigned int i;
    for (i = 0; i < 2; i++) {
        if (D_00495448[i] == chunkTag) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", Initialize_Buffer);
#ifdef SKIP_ASM
struct layer_data;
extern layer_data* ld __asm__("D_0050A088") __attribute__((section(".bss")));

extern "C" void Flush_Buffer(int n);

/**
 * Points the MPEG bit reader at new data and fills its bit buffer.
 *
 * Sets the read pointer of the bit reader at ld: +0x0 read pointer, +0x4 32-bit bit buffer (next bit in the top bit),
 * +0x8 count of valid bits to the data, clears the buffer and count and fills them with Flush_Buffer(0). Called by the
 * Sony codec's Init PS2_SONY_CODEC_INTERNAL_Init.
 *
 * @param data  the MPEG data
 */
extern "C" void Initialize_Buffer(void* data)
{
    char* reader = (char*)ld;
    *(void**)(reader + 0x0) = data;
    *(int*)(reader + 0x8) = 0;
    *(int*)(reader + 0x4) = 0;
    Flush_Buffer(0);
}
#endif

struct layer_data;
extern layer_data* ld __asm__("D_0050A088") __attribute__((section(".bss")));

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", Show_Bits);
#ifdef SKIP_ASM
/**
 * Returns the next bits of the MPEG bit reader without consuming them.
 *
 * Shifts the bit buffer (ld +0x4) right by 32 - n.
 *
 * @param numBits  how many bits (1..32)
 * @return the bits, right-aligned
 */
extern "C" unsigned int Show_Bits(int numBits)
{
    return *(unsigned int*)((char*)ld + 0x4) >> (unsigned int)(-numBits);
}
#endif

extern "C" void* Get_Bits(int);

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", Get_Bits1__FPv);
#ifdef SKIP_ASM
/**
 * Reads one bit from the MPEG bit reader.
 *
 * Calls Get_Bits(1); the argument is unused. Called by extra_bit_information.
 *
 * @return the bit
 */
void* Get_Bits1(void* self)
{
    return Get_Bits(1);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", Flush_Buffer);
#ifdef SKIP_ASM
struct sBitReader_1290 {
    unsigned char* p;   // 0x0
    unsigned int bits;  // 0x4
    int count;          // 0x8
};

// Reads the next byte into the bit buffer; taking the reader by reference matches the target's register copy.
static inline void FillByte_1290(sBitReader_1290*& r, int c)
{
    r->bits |= *r->p++ << (24 - c);
}

/**
 * Consumes bits from the MPEG bit reader and refills it a byte at a time.
 *
 * Shifts the bit buffer left by n, then while fewer than 25 bits remain ORs in the next byte at bit 24 - count and adds
 * 8. Never checks for the end of the data.
 *
 * @param n  how many bits to drop
 */
// Drops n bits from the bit buffer and refills it a byte at a time.
extern "C" void Flush_Buffer(int n)
{
    sBitReader_1290* reader = (sBitReader_1290*)ld;
    reader->bits <<= n;
    int bitCount = reader->count - n;
    while (bitCount < 25) {
        FillByte_1290(reader, bitCount);
        bitCount += 8;
    }
    ((sBitReader_1290*)ld)->count = bitCount;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", Get_Bits);
#ifdef SKIP_ASM
extern "C" void Flush_Buffer(int n);

/**
 * Reads and consumes n bits from the MPEG bit reader.
 *
 * Show_Bits then Flush_Buffer. Declared as returning void* although the value is a bit field.
 *
 * @param n  how many bits
 * @return the bits, right-aligned
 */
// PORT: returns a bit-field value as void* to fit the unit's existing
// `extern "C" void* Get_Bits(int)` declaration (really unsigned int).
extern "C" void* Get_Bits(int n)
{
    unsigned int bits = Show_Bits(n);
    Flush_Buffer(n);
    return (void*)bits;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", Flush_Buffer32);
#ifdef SKIP_ASM
struct sBitReader_1340 {
    unsigned char* p;   // 0x0
    unsigned int bits;  // 0x4
    int count;          // 0x8
};

// Reads the next byte into the bit buffer; taking the reader by reference matches the target's register copy.
static inline void FillByte_1340(sBitReader_1340*& r, int c)
{
    r->bits |= *r->p++ << (24 - c);
}

/**
 * Consumes all 32 bits of the MPEG bit reader's buffer and refills it.
 *
 * Clears the buffer, subtracts 32 from the count and refills a byte at a time. Called by Get_Bits32 and
 * extension_and_user_data.
 */
// Drops the whole 32-bit buffer and refills it a byte at a time.
extern "C" void Flush_Buffer32(void)
{
    sBitReader_1340* reader = (sBitReader_1340*)ld;
    reader->bits = 0;
    int bitCount = reader->count - 32;
    while (bitCount < 25) {
        FillByte_1340(reader, bitCount);
        bitCount += 8;
    }
    ((sBitReader_1340*)ld)->count = bitCount;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", Get_Bits32);
#ifdef SKIP_ASM
extern "C" void Flush_Buffer32(void);

/**
 * Reads and consumes 32 bits from the MPEG bit reader.
 *
 * Show_Bits(32) then Flush_Buffer32. Called by Get_Hdr to read start codes.
 *
 * @return the 32 bits
 */
extern "C" unsigned int Get_Bits32(void)
{
    unsigned int bits = Show_Bits(0x20);
    Flush_Buffer32();
    return bits;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", Get_Hdr);
#ifdef SKIP_ASM
// PORT: next_start_code takes no arguments (the unit declares it void*(void*)); bound by asm label
extern "C" void next_start_code_noarg(void) __asm__("next_start_code");
extern "C" void sequence_header(void);
extern "C" void group_of_pictures_header(void);
extern "C" void picture_header(void);

/**
 * Reads MPEG headers up to the next picture header or the end of the sequence.
 *
 * Loops: aligns to the next start code (next_start_code) and reads it (Get_Bits32); 0x1B3 parses a sequence header
 * (sequence_header), 0x1B8 a group of pictures (group_of_pictures_header), 0x100 a picture header (picture_header, then
 * returns 1), 0x1B7 (sequence end) returns 0; other codes are skipped. Called by the Sony codec's Init.
 *
 * @return 1 after a picture header, 0 at the sequence end
 */
extern "C" int Get_Hdr(void)
{
    for (;;) {
        next_start_code_noarg();
        switch ((int)Get_Bits32()) {
        case 435:
            sequence_header();
            break;
        case 440:
            group_of_pictures_header();
            break;
        case 439:
            return 0;
        case 0x100:
            picture_header();
            return 1;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", next_start_code);
#ifdef SKIP_ASM
extern "C" void Flush_Buffer(int n);

// PORT: no return value here; the unit declares it void* and callers forward $v0.
void next_start_code_impl(void* self) __asm__("next_start_code");

/**
 * Advances the MPEG bit reader to the next start code (00 00 01).
 *
 * Drops bits up to a byte boundary (count & 7) then whole bytes until the next 24 bits are 0x000001. Has no end check.
 * The declaration takes an unused argument.
 */
void next_start_code_impl(void* self)
{
    Flush_Buffer(*(int*)((char*)ld + 0x8) & 7);
    while (Show_Bits(0x18) != 1) {
        Flush_Buffer(8);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", sequence_header);
#ifdef SKIP_ASM
extern int D_005094F4[];
extern int D_005094F8[];
extern void* D_0050950C[];
extern int D_00509510[];
extern int D_00509514[];
extern int D_00509518[];
extern void* D_0050951C[];
extern unsigned char D_0044C218[];
extern unsigned char D_0044C298[];
extern char str_sequence_header[];
void* marker_bit(void* self);
extern "C" void extension_and_user_data(void);

// mpeg2decode's struct layer_data (EA's memory-buffer variant); the unit declares `ld` (D_0050A088).
struct layer_data {
    char pad0[0xC];
    int Bitcnt;                                 // 0x00C
    int intra_quantizer_matrix[64];             // 0x010
    int non_intra_quantizer_matrix[64];         // 0x110
    int chroma_intra_quantizer_matrix[64];      // 0x210
    int chroma_non_intra_quantizer_matrix[64];  // 0x310
    void* load_intra_quantizer_matrix;          // 0x410 (the unit's Get_Bits returns void*)
    void* load_non_intra_quantizer_matrix;      // 0x414
    int load_chroma_intra_quantizer_matrix;     // 0x418
    int load_chroma_non_intra_quantizer_matrix; // 0x41C
    int MPEG2_Flag;                             // 0x420
    int scalable_mode;                          // 0x424 (sequence scalable extension)
};

/**
 * Parses an MPEG sequence header and loads its quantiser matrices.
 *
 * Part of the unit's copy of the MSSG mpeg2decode header parser, reading from the bit reader at ld (D_0050A088). Reads
 * horizontal and vertical size (12 bits each, D_005094F4, D_005094F8), aspect ratio and frame-rate codes (4 each),
 * bit-rate value (18), a marker bit (marker_bit with "sequence_header"), VBV buffer size (10) and the constrained
 * flag. The intra matrix is read in zigzag order (D_0044C218) when its load flag is set, else copied from the default
 * D_0044C298; the non-intra matrix is read or set to 16; chroma matrices copy the luma ones. Ends with extension_and_user_data
 * for extensions and user data.
 */
// MPEG2_sequence_header (mpeg2decode getheader.c sequence_header()).
extern "C" void sequence_header(void)
{
    int i;
    int pos;

    pos = ld->Bitcnt;  // mpeg2decode reads it unconditionally; only its VERBOSE trace uses pos
    D_005094F4[0] = (int)Get_Bits(0xC);
    D_005094F8[0] = (int)Get_Bits(0xC);
    D_0050950C[0] = Get_Bits(4);
    D_00509510[0] = (int)Get_Bits(4);
    D_00509514[0] = (int)Get_Bits(18);
    marker_bit(str_sequence_header);
    D_00509518[0] = (int)Get_Bits(10);
    D_0050951C[0] = Get_Bits(1);
    if ((ld->load_intra_quantizer_matrix = Get_Bits(1)) != 0) {
        for (i = 0; i < 64; i++)
            ld->intra_quantizer_matrix[D_0044C218[i]] = (int)Get_Bits(8);
    } else {
        for (i = 0; i < 64; i++)
            ld->intra_quantizer_matrix[i] = D_0044C298[i];
    }
    if ((ld->load_non_intra_quantizer_matrix = Get_Bits(1)) != 0) {
        for (i = 0; i < 64; i++)
            ld->non_intra_quantizer_matrix[D_0044C218[i]] = (int)Get_Bits(8);
    } else {
        for (i = 0; i < 64; i++)
            ld->non_intra_quantizer_matrix[i] = 16;
    }
    for (i = 0; i < 64; i++) {
        ld->chroma_intra_quantizer_matrix[i] = ld->intra_quantizer_matrix[i];
        ld->chroma_non_intra_quantizer_matrix[i] = ld->non_intra_quantizer_matrix[i];
    }
    extension_and_user_data();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", group_of_pictures_header);
#ifdef SKIP_ASM
extern int D_00509648[];
extern int D_0044C378[];
extern int D_0044C37C[];
extern int D_0044C380[];
extern void* D_00509628[];
extern void* D_0050962C[];
extern void* D_00509630[];
extern void* D_00509634[];
extern void* D_00509638[];
extern void* D_0050963C[];
extern void* D_00509640[];
extern char str_group_of_pictures_header[];
void* marker_bit(void* self);
extern "C" void extension_and_user_data(void);

/**
 * Parses an MPEG-2 group-of-pictures header from the bitstream (mpeg2decode's getheader.c).
 *
 * On the base layer it first resets the temporal reference base to the last frame number + 1 and flags a GOP
 * reset. Then reads drop_flag, hour, minute, a marker bit, second, frame, closed_gop and broken_link into the
 * decoder's globals (D_00509628-D_00509640), and finishes with the extension and user data (extension_and_user_data).
 */
extern "C" void group_of_pictures_header(void)
{
    if (ld == (layer_data*)D_00509648) {
        D_0044C378[0] = D_0044C37C[0] + 1;
        D_0044C380[0] = 1;
    }
    D_00509628[0] = Get_Bits(1);
    D_0050962C[0] = Get_Bits(5);
    D_00509630[0] = Get_Bits(6);
    marker_bit(str_group_of_pictures_header);
    D_00509634[0] = Get_Bits(6);
    D_00509638[0] = Get_Bits(6);
    D_0050963C[0] = Get_Bits(1);
    D_00509640[0] = Get_Bits(1);
    extension_and_user_data();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", picture_header);
#ifdef SKIP_ASM
extern int D_00509554[];
extern int D_00509558[];
extern void* D_0050955C[];
extern void* D_00509560[];
extern void* D_00509564[];
extern void* D_00509568[];
extern void* D_0050956C[];
extern "C" void extension_and_user_data(void);
extern "C" int extra_bit_information(void);
extern "C" void Update_Temporal_Reference_Tacking_Data(void);

/**
 * Parses an MPEG picture header.
 *
 * Part of the unit's copy of the MSSG mpeg2decode header parser, reading from the bit reader at ld (D_0050A088). Clears
 * ld +0x430, reads the temporal reference (10 bits, D_00509554), picture coding type (3, D_00509558) and VBV delay
 * (16); for P and B pictures the full-pel forward vector flag and forward code, for B pictures also the backward ones.
 * Then the extra-bit information (extra_bit_information), extensions (extension_and_user_data) and the temporal reference tracking
 * (Update_Temporal_Reference_Tacking_Data).
 */
// PORT: Get_Bits really returns unsigned int (unit declares void*)
extern "C" void picture_header(void)
{
    *(int*)((char*)ld + 0x430) = 0;
    D_00509554[0] = (int)Get_Bits(10);
    D_00509558[0] = (int)Get_Bits(3);
    D_0050955C[0] = Get_Bits(0x10);
    if (D_00509558[0] == 2 || D_00509558[0] == 3) {
        D_00509560[0] = Get_Bits(1);
        D_00509564[0] = Get_Bits(3);
    }
    if (D_00509558[0] == 3) {
        D_00509568[0] = Get_Bits(1);
        D_0050956C[0] = Get_Bits(3);
    }
    extra_bit_information();
    extension_and_user_data();
    Update_Temporal_Reference_Tacking_Data();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", slice_header);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char D_0044C2D8[];
extern int D_005094F8_B1878[] __asm__("D_005094F8");

/**
 * Parses an MPEG slice header.
 *
 * Part of the unit's copy of the MSSG mpeg2decode header parser, reading from the bit reader at ld (D_0050A088). Reads
 * the slice vertical position extension (3 bits, MPEG-2 pictures taller than 2800 lines only), the priority breakpoint
 * for data-partitioned scalable mode (ld +0x424 == 1), the 5-bit quantiser scale code, mapped through D_0044C2D8 or
 * doubled per the q-scale type (ld +0x428) into ld +0x438, and the optional intra-slice flag (ld +0x43C) with its
 * picture id and extra bits. Nothing in the code or data calls it: the IPU decodes slices.
 *
 * @return the slice vertical position extension
 */
extern "C" int slice_header(void)
{
    int verticalExtension;
    int v;
    int quantizerScaleCode;

    if (*(int*)((char*)ld + 0x420) != 0) {
        verticalExtension = 0;
        if (D_005094F8_B1878[0] >= 2801) {
            verticalExtension = (int)Get_Bits(3);
        }
    } else {
        verticalExtension = 0;
    }
    if (*(int*)((char*)ld + 0x424) == 1) {
        *(int*)((char*)ld + 0x434) = (int)Get_Bits(7);
    }
    quantizerScaleCode = (int)Get_Bits(5);
    {
        char *layer = (char*)ld;
        *(int*)(layer + 0x438) = *(int*)(layer + 0x420) != 0
            ? (*(int*)(layer + 0x428) != 0 ? (int)D_0044C2D8[quantizerScaleCode] : quantizerScaleCode * 2)
            : quantizerScaleCode;
    }
    if (Get_Bits(1) != 0) {
        *(int*)((char*)ld + 0x43C) = (int)Get_Bits(1);
        Get_Bits(1);
        Get_Bits(6);
        extra_bit_information();
    } else {
        *(int*)((char*)ld + 0x43C) = 0;
    }
    return verticalExtension;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", extension_and_user_data);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" unsigned int Show_Bits(int a0);
extern "C" unsigned int Get_Bits_u(int n) __asm__("Get_Bits");
extern "C" void Flush_Buffer32(void);
extern "C" void next_start_code_v(void) __asm__("next_start_code");
extern "C" void user_data_v(void) __asm__("user_data__FPv");
extern "C" void sequence_extension(void);
extern "C" void sequence_display_extension(void);
extern "C" void quant_matrix_extension(void);
extern "C" void sequence_scalable_extension(void);
extern "C" void picture_display_extension(void);
extern "C" void picture_coding_extension(void);
extern "C" void picture_spatial_scalable_extension(void);
extern "C" void picture_temporal_scalable_extension(void);
extern "C" void copyright_extension(void);

/**
 * Parses the MPEG extension and user-data blocks that follow a header.
 *
 * Part of the unit's copy of the MSSG mpeg2decode header parser, reading from the bit reader at ld (D_0050A088). Aligns
 * to the next start code; while it is 0x1B5 (extension) it consumes it and dispatches on the 4-bit id: 1 sequence, 2
 * sequence display, 3 quant matrix, 4 copyright, 5 sequence scalable, 7 picture display, 8 picture coding, 9 picture
 * spatial scalable, 10 picture temporal scalable; for 0x1B2 (user data) it skips to the next start code. The comment
 * above its body ("script block ... command per 0x1B5 token") is wrong.
 */
extern "C" void extension_and_user_data(void)
{
    unsigned int tok;
    next_start_code_v();
    while ((tok = Show_Bits(0x20)) == 437 || tok == 434) {
        if (tok == 437) {
            Flush_Buffer32();
            switch (Get_Bits_u(4)) {
            case 1: sequence_extension(); break;
            case 2: sequence_display_extension(); break;
            case 3: quant_matrix_extension(); break;
            case 5: sequence_scalable_extension(); break;
            case 7: picture_display_extension(); break;
            case 8: picture_coding_extension(); break;
            case 9: picture_spatial_scalable_extension(); break;
            case 10: picture_temporal_scalable_extension(); break;
            case 4: copyright_extension(); break;
            case 6: break;
            }
            next_start_code_v();
        } else {
            Flush_Buffer32();
            user_data_v();
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", sequence_extension);
#ifdef SKIP_ASM
extern int D_005095DC[];
extern int D_00509520[];
extern int D_00509524[];
extern int D_00509528[];
extern int D_0050952C[];
extern int D_00509530[];
extern int D_00509534[];
extern int D_00509510[];
extern int D_00509514[];
extern int D_00509518[];
extern int D_005094EC[];
extern int D_005094F0[];
extern int D_005094F4[];
extern int D_005094F8[];
extern float D_00509504[];
struct sD_00509508;
extern float D_0044C388[];
extern char str_sequence_extension[];
void* marker_bit(void* self);

struct sMpegCtx1AB0 {
    char pad[0x420];
    int f420;
    int f424;
};

/**
 * Parses an MPEG-2 sequence extension and computes the frame rate.
 *
 * Part of the unit's copy of the MSSG mpeg2decode header parser, reading from the bit reader at ld (D_0050A088). Sets
 * MPEG2_Flag (ld +0x420) and clears scalable mode, reads profile and level (8 bits), progressive sequence, chroma
 * format, size and bit-rate extensions, a marker bit ("sequence_extension"), VBV extension, low delay and the
 * frame-rate extension n/d. Stores the frame rate D_00509508 = D_0044C388[frame rate code] * (n + 1) / (d + 1) as
 * floats, splits profile and level (an escape value with low nibble 5 gives profile 133, level 8), and merges the
 * extensions into the size, bit-rate (also kept * 400.0f) and VBV globals.
 */
// Parse an MPEG-2 sequence extension.
extern "C" void sequence_extension(void)
{
    sMpegCtx1AB0* layer = (sMpegCtx1AB0*)ld;
    layer->f420 = 1;
    layer->f424 = 0;
    D_005095DC[0] = 0;
    D_00509520[0] = (int)Get_Bits(8);
    D_00509524[0] = (int)Get_Bits(1);
    D_00509528[0] = (int)Get_Bits(2);
    int hext = (int)Get_Bits(2);
    int vext = (int)Get_Bits(2);
    int brext = (int)Get_Bits(0xC);
    marker_bit(str_sequence_extension);
    int vbvext = (int)Get_Bits(8);
    D_0050952C[0] = (int)Get_Bits(1);
    D_00509530[0] = (int)Get_Bits(2);
    D_00509534[0] = (int)Get_Bits(5);
    D_00509508.value = D_0044C388[D_00509510[0]] * ((float)(D_00509530[0] + 1) / (float)(D_00509534[0] + 1));
    if ((D_00509520[0] >> 7) & 1) {
        if ((D_00509520[0] & 0xF) == 5) {
            D_005094EC[0] = 133;
            D_005094F0[0] = 8;
        }
    } else {
        D_005094EC[0] = D_00509520[0] >> 4;
        D_005094F0[0] = D_00509520[0] & 0xF;
    }
    D_005094F4[0] = (hext << 12) | (D_005094F4[0] & 0xFFF);
    D_005094F8[0] = (vext << 12) | (D_005094F8[0] & 0xFFF);
    D_00509514[0] = D_00509514[0] + (brext << 18);
    D_00509504[0] = D_00509514[0] * 400.0f;
    D_00509518[0] = D_00509518[0] + (vbvext << 10);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", sequence_display_extension);
#ifdef SKIP_ASM
extern void* D_00509538[];
extern void* D_0050953C[];
extern void* D_00509540[];
extern void* D_00509544[];
extern void* D_00509548[];
extern void* D_0050954C[];
extern void* D_00509550[];
extern char str_sequence_display_extension[];
void* marker_bit(void* self);

/**
 * Parses an MPEG-2 sequence display extension.
 *
 * Part of the unit's copy of the MSSG mpeg2decode header parser, reading from the bit reader at ld (D_0050A088). Reads
 * the video format (3 bits) and, when the colour description flag is set, colour primaries, transfer characteristics
 * and matrix coefficients (8 each), then the display horizontal size (14), a marker bit ("sequence_display_extension")
 * and the display vertical size (14).
 */
extern "C" void sequence_display_extension(void)
{
    D_00509538[0] = Get_Bits(3);
    D_0050953C[0] = Get_Bits(1);
    if (D_0050953C[0] != 0) {
        D_00509540[0] = Get_Bits(8);
        D_00509544[0] = Get_Bits(8);
        D_00509548[0] = Get_Bits(8);
    }
    D_0050954C[0] = Get_Bits(0xE);
    marker_bit(str_sequence_display_extension);
    D_00509550[0] = Get_Bits(0xE);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", quant_matrix_extension);
#ifdef SKIP_ASM
struct sWSTables1D58 {
    char pad0[0x10];
    void* a10[64];              // 0x010
    void* a110[64];             // 0x110
    void* a210[64];             // 0x210
    void* a310[64];             // 0x310
    void* p410;                 // 0x410
    void* p414;                 // 0x414
    void* p418;                 // 0x418
    void* p41C;                 // 0x41C
};

extern unsigned char D_0044C218[];


/**
 * Parses an MPEG-2 quant matrix extension.
 *
 * Part of the unit's copy of the MSSG mpeg2decode header parser, reading from the bit reader at ld (D_0050A088). For
 * each of the four load flags (intra, non-intra, chroma intra, chroma non-intra; ld +0x410..+0x41C) that is set it
 * reads 64 8-bit values in zigzag order (D_0044C218); a luma matrix also replaces its chroma matrix.
 */
extern "C" void quant_matrix_extension(void)
{
    layer_data** gd = &ld;
    {
        void* loadFlag = Get_Bits(1);
        ((sWSTables1D58*)*gd)->p410 = loadFlag;
        if (loadFlag != 0) {
            int i;
            for (i = 0; i < 64; i++) {
                void* quantValue = Get_Bits(8);
                int k = D_0044C218[i];
                sWSTables1D58* layer = ((sWSTables1D58*)ld);
                layer->a10[k] = quantValue;
                layer->a210[k] = quantValue;
            }
        }
    }
    {
        void* loadFlag = Get_Bits(1);
        ((sWSTables1D58*)ld)->p414 = loadFlag;
        if (loadFlag != 0) {
            int i;
            for (i = 0; i < 64; i++) {
                void* quantValue = Get_Bits(8);
                int k = D_0044C218[i];
                sWSTables1D58* layer = ((sWSTables1D58*)ld);
                layer->a110[k] = quantValue;
                layer->a310[k] = quantValue;
            }
        }
    }
    {
        void* loadFlag = Get_Bits(1);
        ((sWSTables1D58*)ld)->p418 = loadFlag;
        if (loadFlag != 0) {
            int i;
            for (i = 0; i < 64; i++) {
                void* quantValue = Get_Bits(8);
                int k = D_0044C218[i];
                sWSTables1D58* layer = ((sWSTables1D58*)ld);
                layer->a210[k] = quantValue;
            }
        }
    }
    {
        void* loadFlag = Get_Bits(1);
        ((sWSTables1D58*)ld)->p41C = loadFlag;
        if (loadFlag != 0) {
            int i;
            for (i = 0; i < 64; i++) {
                void* quantValue = Get_Bits(8);
                int k = D_0044C218[i];
                sWSTables1D58* layer = ((sWSTables1D58*)ld);
                layer->a310[k] = quantValue;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", sequence_scalable_extension);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// mpeg2decode global.h: the sequence scalable extension's plain int globals
extern int layer_id __asm__("D_005095DC");
extern int lower_layer_prediction_horizontal_size __asm__("D_005095E0");
extern int lower_layer_prediction_vertical_size __asm__("D_005095E4");
extern int horizontal_subsampling_factor_m __asm__("D_005095E8");
extern int horizontal_subsampling_factor_n __asm__("D_005095EC");
extern int vertical_subsampling_factor_m __asm__("D_005095F0");
extern int vertical_subsampling_factor_n __asm__("D_005095F4");
extern char str_sequence_scalable_extension[];
extern char D_00495510[];
void* marker_bit(void* self);
// Get_Bits returning int: with the unit's void* (or mpeg2decode's unsigned int) declaration GCC keeps the
// globals' %hi parts in s-registers across the calls.
extern "C" int Get_Bits_i(int n) __asm__("Get_Bits");
void Error(void* self);

/**
 * Parses an MPEG-2 sequence scalable extension from the bitstream (mpeg2decode's getheader.c).
 *
 * Reads the scalable mode (stored + 1) and layer ID; spatial scalability also reads the lower layer's
 * prediction size and the subsampling factors. Temporal scalability is reported as unsupported through the
 * error routine (Error).
 */
// mpeg2decode getheader.c sequence_scalable_extension(); Get_Bits = Get_Bits, marker_bit = marker_bit,
// Error = Error.
extern "C" void sequence_scalable_extension(void)
{
    int pos;

    pos = ld->Bitcnt;  // mpeg2decode reads it unconditionally; only its VERBOSE trace uses pos

    /* values (without the +1 offset) of scalable_mode are defined in Table 6-10 of ISO/IEC 13818-2 */
    ld->scalable_mode = Get_Bits_i(2) + 1; /* add 1 to make SC_DP != SC_NONE */

    layer_id = Get_Bits_i(4);

    if (ld->scalable_mode == 2) { /* SC_SPAT */
        lower_layer_prediction_horizontal_size = Get_Bits_i(14);
        marker_bit(str_sequence_scalable_extension);
        lower_layer_prediction_vertical_size = Get_Bits_i(14);
        horizontal_subsampling_factor_m = Get_Bits_i(5);
        horizontal_subsampling_factor_n = Get_Bits_i(5);
        vertical_subsampling_factor_m = Get_Bits_i(5);
        vertical_subsampling_factor_n = Get_Bits_i(5);
    }

    if (ld->scalable_mode == 4) /* SC_TEMP */
        Error(D_00495510);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", picture_display_extension);
#ifdef SKIP_ASM
extern int D_00509524[];
extern int D_00509584[];
extern int D_00509588[];
extern int D_00509598[];
extern void* D_005095C0[];
extern void* D_005095D0[];
extern char str_picture_display_extension_first_marker_b[];
extern char str_picture_display_extension_second_marker[];
void* marker_bit(void* self);

/**
 * Parses an MPEG-2 picture display extension (frame centre offsets).
 *
 * Part of the unit's copy of the MSSG mpeg2decode header parser, reading from the bit reader at ld (D_0050A088). The
 * number of offsets is 1 for progressive sequences unless repeat-first-field (3 with top-field-first, else 2); for
 * interlaced, 1 for field pictures or frames without repeat-first-field, else 3 or 2 the same way. Each offset reads 16
 * bits into D_005095C0[i] and D_005095D0[i], each followed by a marker bit.
 */
extern "C" void picture_display_extension(void)
{
    int n;
    int i;
    if (D_00509524[0] != 0) {
        if (D_00509598[0] == 0) {
            n = 1;
        } else {
            n = D_00509588[0] != 0 ? 3 : 2;
        }
    } else if (D_00509584[0] != 3) {
        n = 1;
    } else {
        n = D_00509598[0] != 0 ? 3 : 2;
    }
    for (i = 0; i < n; i++) {
        D_005095C0[i] = Get_Bits(0x10);
        marker_bit(str_picture_display_extension_first_marker_b);
        D_005095D0[i] = Get_Bits(0x10);
        marker_bit(str_picture_display_extension_second_marker);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", picture_coding_extension);
#ifdef SKIP_ASM
extern void* D_00509570[];
extern void* D_00509580[];
extern int D_00509584[];
extern int D_00509588[];
extern void* D_0050958C[];
extern void* D_00509590[];
extern void* D_00509594[];
extern int D_00509598[];
extern void* D_0050959C[];
extern void* D_005095A0[];
extern void* D_005095A4[];
extern void* D_005095A8[];
extern void* D_005095AC[];
extern void* D_005095B0[];
extern void* D_005095B4[];
extern void* D_005095B8[];

/**
 * Parses an MPEG-2 picture coding extension.
 *
 * Part of the unit's copy of the MSSG mpeg2decode header parser, reading from the bit reader at ld (D_0050A088). Reads
 * the four f_codes (4 bits each, D_00509570), intra DC precision, picture structure, top field first, frame pred frame
 * DCT, concealment vectors, q-scale type (ld +0x428), intra VLC format, alternate scan (ld +0x42C), repeat first field,
 * chroma 420 type, progressive frame and the composite display flag with its fields.
 */
extern "C" void picture_coding_extension(void)
{
    D_00509570[0] = Get_Bits(4);
    D_00509570[1] = Get_Bits(4);
    D_00509570[2] = Get_Bits(4);
    D_00509570[3] = Get_Bits(4);
    D_00509580[0] = Get_Bits(2);
    D_00509584[0] = (int)Get_Bits(2);
    D_00509588[0] = (int)Get_Bits(1);
    D_0050958C[0] = Get_Bits(1);
    D_00509590[0] = Get_Bits(1);
    *(void**)((char*)ld + 0x428) = Get_Bits(1);
    D_00509594[0] = Get_Bits(1);
    *(void**)((char*)ld + 0x42C) = Get_Bits(1);
    D_00509598[0] = (int)Get_Bits(1);
    D_0050959C[0] = Get_Bits(1);
    D_005095A0[0] = Get_Bits(1);
    D_005095A4[0] = Get_Bits(1);
    if (D_005095A4[0] != 0) {
        D_005095A8[0] = Get_Bits(1);
        D_005095AC[0] = Get_Bits(3);
        D_005095B0[0] = Get_Bits(1);
        D_005095B4[0] = Get_Bits(7);
        D_005095B8[0] = Get_Bits(8);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", picture_spatial_scalable_extension);
#ifdef SKIP_ASM
extern void* D_005095F8[];
extern int D_005095FC[];
extern int D_00509600[];
extern void* D_00509604[];
extern void* D_00509608[];
extern void* D_0050960C[];
extern char str_picture_spatial_scalable_extension_first[];
extern char str_picture_spatial_scalable_extension_secon[];
void* marker_bit(void* self);

/**
 * Reads an MPEG-2 picture spatial scalable extension header (as in the reference mpeg2decode's gethdr.c).
 *
 * Reads the lower layer's temporal reference, its signed 15-bit horizontal and vertical offsets (each after a
 * checked marker bit), the spatial-temporal weight table index and two lower-layer field flags, and marks the
 * stream as spatially scalable (ld+0x430).
 */
// PORT: Get_Bits really returns unsigned int (unit declares void*)
extern "C" void picture_spatial_scalable_extension(void)
{
    *(int*)((char*)ld + 0x430) = 1;
    D_005095F8[0] = Get_Bits(10);
    marker_bit(str_picture_spatial_scalable_extension_first);
    D_005095FC[0] = (int)Get_Bits(0xF);
    if (D_005095FC[0] >= 0x4000) {
        D_005095FC[0] -= 0x8000;
    }
    marker_bit(str_picture_spatial_scalable_extension_secon);
    D_00509600[0] = (int)Get_Bits(0xF);
    if (D_00509600[0] >= 0x4000) {
        D_00509600[0] -= 0x8000;
    }
    D_00509604[0] = Get_Bits(2);
    D_00509608[0] = Get_Bits(1);
    D_0050960C[0] = Get_Bits(1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", picture_temporal_scalable_extension);
#ifdef SKIP_ASM
extern int D_00495608[];

/**
 * Reports that temporal scalability is unsupported (does nothing else).
 *
 * Calls Error__FPv with "temporal scalability not supported\n" (D_00495608). Called for extension id 10 by
 * extension_and_user_data.
 */
extern "C" void picture_temporal_scalable_extension(void)
{
    Error(D_00495608);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", extra_bit_information);
#ifdef SKIP_ASM
// PORT: Get_Bits1 takes a void* it never reads; this caller passes nothing, so it binds
// a no-argument declaration to the mangled symbol.
void* Get_Bits1_noarg() __asm__("Get_Bits1__FPv");

/**
 * Skips the extra information bytes after a header and counts them.
 *
 * While the next bit (Get_Bits1) is 1 it drops 8 bits. Called by picture_header and slice_header.
 *
 * @return the number of extra bytes skipped
 */
extern "C" int extra_bit_information(void)
{
    int n = 0;
    while (Get_Bits1_noarg() != 0) {
        Flush_Buffer(8);
        n++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", marker_bit__FPv);
#ifdef SKIP_ASM
/**
 * Reads one marker bit; the text naming it (the argument) is not used.
 *
 * Callers pass texts such as "sequence_header" and "copyright_extension first marker bit".
 *
 * @return the bit (Get_Bits(1))
 */
void* marker_bit(void* self)
{
    return Get_Bits(1);
}
#endif

extern "C" void* next_start_code(void* self);

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", user_data__FPv);
#ifdef SKIP_ASM
/**
 * Skips MPEG user data by moving to the next start code.
 *
 * Forwards to next_start_code. Called by extension_and_user_data for start code 0x1B2.
 *
 * @return next_start_code's $v0 (nothing meaningful)
 */
void* user_data(void* self)
{
    return next_start_code(self);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", copyright_extension);
#ifdef SKIP_ASM
extern void* D_00509610[];
extern void* D_00509614[];
extern void* D_00509618[];
extern void* D_0050961C[];
extern void* D_00509620[];
extern void* D_00509624[];
extern char str_copyright_extension_first_marker_bit[];
extern char str_copyright_extension_second_marker_bit[];
extern char str_copyright_extension_third_marker_bit[];
void* marker_bit(void* self);

/**
 * Reads an MPEG-2 copyright extension header into globals (as in the reference mpeg2decode's gethdr.c).
 *
 * Copyright flag, identifier, original-or-copy flag, 7 reserved bits, then the 20-, 22- and 22-bit copyright
 * number parts, each after a checked marker bit.
 */
extern "C" void copyright_extension(void)
{
    D_00509610[0] = Get_Bits(1);
    D_00509614[0] = Get_Bits(8);
    D_00509618[0] = Get_Bits(1);
    Get_Bits(7);
    marker_bit(str_copyright_extension_first_marker_bit);
    D_0050961C[0] = Get_Bits(20);
    marker_bit(str_copyright_extension_second_marker_bit);
    D_00509620[0] = Get_Bits(22);
    marker_bit(str_copyright_extension_third_marker_bit);
    D_00509624[0] = Get_Bits(22);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", Update_Temporal_Reference_Tacking_Data);
#ifdef SKIP_ASM
extern int D_00509648[];
extern int D_00509554[];
extern int D_00509558[];
extern int D_0044C378[];
extern int D_0044C37C[];
extern int D_0044C380[];
extern int D_0044C3C8[];
extern int D_0044C3CC[];
extern int D_0050A098[];

/**
 * Updates the true frame number from the picture's temporal reference, handling wraps.
 *
 * Only for the base layer (ld == D_00509648). For a new non-B picture: a pending wrap (D_0044C3C8) adds 1024 to the
 * base (D_0044C378); a temporal reference below the last one (D_0044C3CC) without a GOP reset (D_0044C380) marks a
 * wrap. Then the true frame number D_0050A098 = base + temporal reference (+1024 while a wrap is pending and the
 * reference has not passed the old one), and D_0044C37C keeps the maximum. Called by picture_header.
 */
extern "C" void Update_Temporal_Reference_Tacking_Data(void)
{
    if (ld != (layer_data*)D_00509648) {
        return;
    }
    int last;
    if (D_00509558[0] != 3 && D_00509554[0] != (last = D_0044C3CC[0])) {
        if (D_0044C3C8[0] != 0) {
            D_0044C3C8[0] = 0;
            D_0044C378[0] += 0x400;
        }
        if (D_00509554[0] < last && D_0044C380[0] == 0) {
            D_0044C3C8[0] = 1;
        }
        D_0044C380[0] = 0;
        D_0044C3CC[0] = D_00509554[0];
    }
    D_0050A098[0] = D_0044C378[0] + D_00509554[0];
    if (D_0044C3C8[0] != 0 && D_0044C3CC[0] >= D_00509554[0]) {
        D_0050A098[0] += 0x400;
    }
    D_0044C37C[0] = (D_0044C37C[0] < D_0050A098[0]) ? D_0050A098[0] : D_0044C37C[0];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", SHAPE_bpctobpp);
#ifdef SKIP_ASM
struct SHAPE_bpctobpp_sKey {
    char c;
};

extern "C" char SHAPE_type(int key);
extern "C" void SHAPE_depth(SHAPE_bpctobpp_sKey* k);

/**
 * Converts a bits-per-component format code to its bits per pixel.
 *
 * Maps @p key to a shape type with SHAPE_type() (codes such as 4, 8, 15, 16, 555, 1555, 32 and 8888, decimal)
 * and looks the type up with SHAPE_depth(). A @p key of 0 is treated as 32. Declared void here, but
 * SHAPE_depth()'s result is still in the return register when it returns.
 *
 * @param key  format code, or 0 for 32
 */
extern "C" void SHAPE_bpctobpp(int key)
{
    SHAPE_bpctobpp_sKey k;
    if (key == 0) {
        key = 0x20;
    }
    k.c = SHAPE_type(key);
    SHAPE_depth(&k);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", SHAPE_imagesize);
#ifdef SKIP_ASM
// PORT: the unit declares SHAPE_bpctobpp `void`, but it returns SHAPE_depth's result
// (the format's bits per pixel); bound via asm label.
extern "C" int SHAPE_bpctobpp_bits(int fmt) __asm__("SHAPE_bpctobpp");

/**
 * Computes the bytes needed for a texture's pixels, including its mipmap levels.
 *
 * The bits per pixel come from SHAPE_bpctobpp_bits(fmt) (16 when it returns 15). Each level halves the size
 * (down to 1x1, where it stops), rows round up to whole bytes, and each level after the first starts 16-byte
 * aligned.
 *
 * @param w         width in pixels, 1 to 65536
 * @param height         height in pixels, 1 to 65536
 * @param fmt       pixel format code (On Tour: bpc)
 * @param maxLevel  last mipmap level to include (0 for none)
 * @return the size in bytes, or 0 for an invalid size or a format SHAPE_type() rejects
 */
extern "C" int SHAPE_imagesize(int w, int height, int fmt, int maxLevel)
{
    int total = 0;
    int bppBits = SHAPE_bpctobpp_bits(fmt);
    int bpp = 0x10;
    if (bppBits != 0xF) bpp = bppBits;
    if (w < 1 || w > 0x10000 || height < 1) return 0;
    if (height > 0x10000) return 0;
    if (w * height > 1000000000) return 0;
    if (SHAPE_type(bppBits) == 0) return 0;
    int i;
    for (i = 0; i <= maxLevel; i++) {
        int lw = w >> i;
        int lh = height >> i;
        if (lw <= 0) lw = 1;
        if (lh <= 0) lh = 1;
        if (total != 0) total = (total + 0xF) & ~0xF;
        total += ((lw * bpp + 7) / 8) * lh;
        if (lw == 1 && lh == 1) break;
    }
    return total;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", GetNumberOfColours);
#ifdef SKIP_ASM
/**
 * Returns the palette size for a palettised bit depth.
 *
 * @param bpp  bits per pixel
 * @return 256 for 8, 16 for 4, otherwise 0
 */
extern "C" int GetNumberOfColours(int bpp)
{
    switch (bpp) {
    case 8:
        return 0x100;
    case 4:
        return 0x10;
    default:
        return 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", SHAPE_createsize);
#ifdef SKIP_ASM
// PORT: the unit declares SHAPE_bpctobpp `void`, but it returns the format's bits per pixel.
extern "C" int SHAPE_bpctobpp_bits(int fmt) __asm__("SHAPE_bpctobpp");
extern "C" int SHAPE_imagesize(int w, int h, int fmt, int maxLevel);

/**
 * Computes the full size of a SHAPE texture: header, pixels, palette and optional comment and metal-bin blocks.
 *
 * Adds a 16-byte header to SHAPE_imagesize(); a palettised format (8 bits or fewer) with a palette depth gets a
 * 16-byte aligned palette with its own 16-byte header (depth 15 counts as 16). A comment adds 8 bytes plus its
 * size, a metal bin 16 bytes plus its size.
 *
 * @param w         width in pixels
 * @param height         height in pixels
 * @param fmt       pixel format code (On Tour: bpc)
 * @param clut      palette entry depth in bits (On Tour: clutbpp), 0 for none
 * @param maxLevel  last mipmap level to include
 * @param extra1    comment size in bytes (On Tour: commentsize), 0 for none
 * @param extra2    metal bin size in bytes (On Tour: metalbinsize), 0 for none
 * @return the total size in bytes
 */
extern "C" int SHAPE_createsize(int w, int height, int fmt, int clut, int maxLevel, int extra1, int extra2)
{
    int bppBits = SHAPE_bpctobpp_bits(fmt);
    int size = SHAPE_imagesize(w, height, fmt, maxLevel);
    size += 0x10;
    if (bppBits < 9 && clut != 0) {
        if (clut == 0xF) clut = 0x10;
        if (size != 0) size = (size + 0xF) & ~0xF;
        int n = GetNumberOfColours(bppBits) * clut / 8 + 0x10;
        size = ((size + 0xF) & ~0xF) + n;
    }
    if (extra1 != 0) {
        int withHeader = size + 8;
        size = withHeader + extra1;
    }
    if (extra2 != 0) {
        int withHeader = size + 0x10;
        size = withHeader + extra2;
    }
    return size;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", SHAPE_createat);
#ifdef SKIP_ASM
extern "C" char SHAPE_type(int key);
extern "C" void MEM_fill(void* dst, int value, int size);
// PORT: memset-like fill called with an unsigned all-ones value here
extern "C" void MEM_fill_u(void* dst, unsigned int value, int size) __asm__("MEM_fill");
extern "C" int SHAPE_imagesize(int w, int h, int fmt, int maxLevel);
extern "C" int GetNumberOfColours(int v);
extern "C" unsigned char SHAPE_cluttype(int clut);

struct sTexHdr_27C8 {
    unsigned type : 8;          // 0x0
    unsigned next : 24;
    short w;                    // 0x4
    short h;                    // 0x6
    int f8;                     // 0x8
    unsigned flags : 28;        // 0xC
    unsigned levels : 4;
    int dataOff;                // 0x10
};

struct sRGBA_27C8 {
    unsigned char r, g, b, a;
};

static inline unsigned char* texData_27C8(sTexHdr_27C8* t)
{
    if (*(int*)((char*)t + 0xC) & 0x1000) {
        return (unsigned char*)t + t->dataOff;
    }
    return (unsigned char*)t + 0x10;
}

/**
 * Lays out a new SHAPE in caller-provided memory: header, then the palette, comment and metal-bin blocks it
 * asks for, linked in a chain.
 *
 * A 32-bit or wider palette starts as a grey ramp; narrower ones are filled with 0xFF.
 *
 * @param tex  memory for the shape, sized by SHAPE_createsize (On Tour SHAPE* shp)
 * @param w  width in pixels
 * @param height  height in pixels
 * @param fmt  pixel format; 0 means 32 bpp (On Tour bpp)
 * @param clut  palette colour depth, 0 for none (On Tour clutbpp)
 * @param levels  number of mipmap levels (On Tour mipmaplevels)
 * @param extra1  comment size, 0 for none (On Tour commentsize)
 * @param extra2  metal-bin size, 0 for none (On Tour metalbinsize)
 */
extern "C" void SHAPE_createat(void* tex, int w, int height, int fmt, int clut, int levels, int extra1, int extra2)
{
    sTexHdr_27C8* texHeader = (sTexHdr_27C8*)tex;
    int bpp = 0x20;
    if (fmt != 0) bpp = fmt;
    char shapeType = SHAPE_type(bpp);
    MEM_fill(texHeader, 0, 0x10);
    texHeader->type = shapeType;
    texHeader->w = w;
    texHeader->h = height;
    texHeader->levels = levels;
    sTexHdr_27C8* hdr = texHeader;
    unsigned char* cursor = (unsigned char*)hdr + (SHAPE_imagesize(w, height, bpp, levels) + 0x10);
    if (bpp < 9 && clut != 0) {
        int n = GetNumberOfColours(bpp);
        unsigned char cfmt = SHAPE_cluttype(clut);
        int off = ((cursor - (unsigned char*)hdr) + 0xF) & ~0xF;
        cursor = (unsigned char*)hdr + off;
        int cbits = 0x10;
        if (clut != 0xF) cbits = clut;
        hdr->next = off;
        MEM_fill(cursor, 0, 0x10);
        ((sTexHdr_27C8*)cursor)->type = cfmt;
        ((sTexHdr_27C8*)cursor)->h = 1;
        ((sTexHdr_27C8*)cursor)->w = n;
        if (clut >= 0x20) {
            sRGBA_27C8* pal = (sRGBA_27C8*)texData_27C8((sTexHdr_27C8*)cursor);
            for (int i = 0; i < n; i++) {
                pal[i].b = i;
                pal[i].g = i;
                pal[i].r = i;
                pal[i].a = 0xFF;
            }
        } else {
            MEM_fill_u(texData_27C8((sTexHdr_27C8*)cursor), 0xFFFFFFFF, n * cbits / 8);
        }
        hdr = (sTexHdr_27C8*)cursor;
        cursor += n * cbits / 8 + 0x10;
    }
    if (extra1 != 0) {
        hdr->next = cursor - (unsigned char*)hdr;
        hdr = (sTexHdr_27C8*)cursor;
        MEM_fill(cursor, 0, extra1 + 8);
        *(int*)(cursor + 4) = extra1;
        cursor[0] = 111;
        cursor += extra1 + 8;
    }
    if (extra2 != 0) {
        hdr->next = cursor - (unsigned char*)hdr;
        MEM_fill(cursor, 0, extra2 + 0x10);
        cursor[0] = 105;
        *(short*)(cursor + 6) = 0x10;
        *(int*)(cursor + 0x10) = 0x455355;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", SHAPE_create);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int SHAPE_createsize(int w, int h, int fmt, int clut, int maxLevel, int extra1, int extra2);
extern "C" void SHAPE_createat(void* tex, int w, int h, int fmt, int clut, int maxLevel, int extra1, int extra2);
extern void* (*SHAPE_memallocalign[])(char* name, int size, int a2, int align, int heap);
extern char str_SHPpct_dxpct_dxpct_d[];

/**
 * Allocates and initialises a SHAPE texture.
 *
 * Sizes it with SHAPE_createsize(), allocates it 16-byte aligned through the SHAPE_memallocalign hook under the
 * name "SHP<w>x<h>x<fmt>", then lays it out with SHAPE_createat().
 *
 * @param w         width in pixels
 * @param height         height in pixels
 * @param fmt       pixel format code (On Tour: bpp)
 * @param clut      palette entry depth in bits, 0 for none
 * @param maxLevel  last mipmap level to include
 * @param heap      memory type passed to the allocator (On Tour: memtype)
 * @param extra1    comment size in bytes, 0 for none
 * @param extra2    metal bin size in bytes, 0 for none
 * @return the new texture, or 0 when the size is invalid or the allocation fails
 */
extern "C" void* SHAPE_create(unsigned int w, int height, int fmt, int clut, int maxLevel, int heap, int extra1, int extra2)
{
    char name[16];
    void* tex = 0;
    int size = SHAPE_createsize(w, height, fmt, clut, maxLevel, extra1, extra2);
    if (size != 0) {
        sprintf(name, str_SHPpct_dxpct_dxpct_d, w, height, fmt);
        tex = SHAPE_memallocalign[0](name, size, 0, 0x10, heap);
        if (tex != 0) {
            SHAPE_createat(tex, w, height, fmt, clut, maxLevel, extra1, extra2);
        }
    }
    return tex;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", SHAPE_destroy);
#ifdef SKIP_ASM
extern void (*SHAPE_memfree[])(char* buf);

/**
 * Destroys a SHAPE: clears its type byte and frees it.
 *
 * Frees through the SHAPE library's free callback SHAPE_memfree. Called by the video frame destructor func_003B08E0 on the
 * image its constructor made with SHAPE_create.
 *
 * @param buf  the SHAPE
 */
extern "C" void SHAPE_destroy(char* buf)
{
    buf[0] = 0;
    SHAPE_memfree[0](buf);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", decodeAlpha);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Decodes one 4x4 alpha block into the alpha bytes of dst: mode 3 is DXT3 (explicit 4-bit alpha),
// anything else is DXT5 (two endpoints + 3-bit indices).
// NOTE: the DXT3 path steps through dst as `unsigned long` (64-bit on the EE), not as 32-bit pixels.
// PORT: callers declare decodeAlpha with char* parameters; defined under an asm label.
extern "C" void decodeAlpha_impl(unsigned long* dst, int stride, unsigned char* src, int mode) __asm__("decodeAlpha");
/**
 * Decodes the alpha of one DXT block into the alpha bytes of its 4x4 texels.
 *
 * Mode 3 (DXT3) has explicit 4-bit alpha; otherwise (DXT5) two endpoints with 6 interpolated levels, or 4
 * plus 0
 * and 255.
 *
 * @param dst  the block's first output texel (On Tour d)
 * @param stride  bytes per output row (On Tour rowbytes)
 * @param src  the compressed alpha block (On Tour blockbuf)
 * @param mode  the DXT type (On Tour dxttype)
 */
extern "C" void decodeAlpha_impl(unsigned long* dst, int stride, unsigned char* src, int mode)
{
    if (mode == 3) {
        for (unsigned int y = 0; y < 4; y++) {
            for (unsigned int x = 0; x < 2; x++) {
                unsigned char alphas = *src++;
                unsigned int lo = alphas & 0xF;
                *dst = (*dst & 0xFFFFFF) | (int)((lo | (lo << 4)) << 24);
                dst++;
                unsigned int hi = alphas & 0xF0;
                *dst = (*dst & 0xFFFFFF) | (int)((hi | (hi >> 4)) << 24);
                dst++;
            }
            dst = (unsigned long*)((char*)dst + stride - 0x20);
        }
        return;
    }
    unsigned char* curPtr = src;
    unsigned char* dstAlpha = (unsigned char*)dst + 3;
    unsigned char alpha0 = *curPtr++;
    unsigned char alpha1 = *curPtr++;
    unsigned char alpha2, alpha3, alpha4, alpha5, alpha6, alpha7;
    if (alpha0 > alpha1) {
        alpha2 = (6 * alpha0 + alpha1 + 3) / 7;
        alpha3 = (5 * alpha0 + 2 * alpha1 + 3) / 7;
        alpha4 = (4 * alpha0 + 3 * alpha1 + 3) / 7;
        alpha5 = (3 * alpha0 + 4 * alpha1 + 3) / 7;
        alpha6 = (2 * alpha0 + 5 * alpha1 + 3) / 7;
        alpha7 = (alpha0 + 6 * alpha1 + 3) / 7;
    } else {
        alpha2 = (4 * alpha0 + alpha1 + 2) / 5;
        alpha3 = (3 * alpha0 + 2 * alpha1 + 2) / 5;
        alpha4 = (2 * alpha0 + 3 * alpha1 + 2) / 5;
        alpha5 = (alpha0 + 4 * alpha1 + 2) / 5;
        alpha6 = 0;
        alpha7 = 0xFF;
    }
    for (int i = 0; i < 2; i++) {
        unsigned long bits = *(unsigned long*)curPtr;
        curPtr += 3;
        for (int y = 0; y < 2; y++) {
            for (int x = 0; x < 4; x++) {
                unsigned char idx = bits & 7;
                bits >>= 3;
                switch (idx) {
                case 0: *dstAlpha = alpha0; break;
                case 1: *dstAlpha = alpha1; break;
                case 2: *dstAlpha = alpha2; break;
                case 3: *dstAlpha = alpha3; break;
                case 4: *dstAlpha = alpha4; break;
                case 5: *dstAlpha = alpha5; break;
                case 6: *dstAlpha = alpha6; break;
                case 7: *dstAlpha = alpha7; break;
                }
                dstAlpha += 4;
            }
            dstAlpha += stride - 0x10;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", decodeRGB);
#ifdef SKIP_ASM
// PORT: 64-bit `ulong` palette entries (`long` is 8 bytes on this compiler).
static inline unsigned short dxtTo1555_2E78(unsigned short c)
{
    if (c != 0) {
        // the alpha bit is a sign-extended short constant (forces the final 16-bit mask)
        int g = ((c >> 1) & 0x3E0) | (short)0x8000;
        c = ((c & 0x1F) << 10) | g | (c >> 11);
    }
    return c;
}

/**
 * Decodes the colour part of one DXT block into 4 rows of 4 texels.
 *
 * Builds the four-colour palette from the two 565 endpoints (5:3 and 3:5 blends, or their average and
 * transparent
 * black when the first is not larger) and converts it with dxtTo1555. Despite the names, @p src is the output
 * and
 * @p dst the compressed block.
 *
 * @param src  the block's first output texel (On Tour d)
 * @param pitch  bytes per output row (On Tour rowbytes)
 * @param dst  the compressed colour block (On Tour blockbuf)
 */
// Decode one DXT1 block into 4 rows of 4 64-bit texels.
extern "C" void decodeRGB(char* src, int pitch, char* dst)
{
    ulong pal[4];
    ulong endpoints = *(ulong*)dst;
    ulong color0 = ((endpoints & 0x1F) << 3) | ((endpoints & 0x7E0) << 5) | ((endpoints & 0xF800) << 8);
    ulong color1 = ((endpoints >> 13) & 0xF8) | ((endpoints >> 11) & 0xFC00) | ((endpoints & 0xF8000000) >> 8);
    pal[0] = color0 | 0x40204;
    pal[1] = color1 | 0x40204;
    if (color0 >= color1) {
        int blue0 = color0 & 0xFF0000;
        int blue1 = color1 & 0xFF0000;
        int green0 = color0 & 0xFF00;
        int green1 = color1 & 0xFF00;
        int red0 = color0 & 0xFF;
        int red1 = color1 & 0xFF;
        pal[2] = ((((blue0 * 5 + blue1 * 3) >> 3) & 0xFF0000) | (((green0 * 5 + green1 * 3) >> 3) & 0xFF00)) | (((red0 * 5 + red1 * 3) >> 3) & 0xFF);
        pal[3] = (((((blue0 * 3 + blue1 * 5) >> 3) & 0xFF0000) | (((green0 * 3 + green1 * 5) >> 3) & 0xFF00)) | (((red0 * 3 + red1 * 5) >> 3) & 0xFF)) | 0xFF000000UL;
    } else {
        int blue0 = color0 & 0xFF0000;
        int blue1 = color1 & 0xFF0000;
        int green0 = color0 & 0xFF00;
        int green1 = color1 & 0xFF00;
        int red0 = color0 & 0xFF;
        int red1 = color1 & 0xFF;
        pal[2] = ((ulong)((red0 + red1) | ((green0 + green1) | (blue0 + blue1))) >> 1) | 0xFF000000UL;
        pal[3] = 0;
    }
    pal[0] |= 0xFF000000;
    pal[1] |= 0xFF000000;
    pal[2] |= 0xFF000000;
    pal[0] = dxtTo1555_2E78(pal[0]);
    pal[1] = dxtTo1555_2E78(pal[1]);
    pal[2] = dxtTo1555_2E78(pal[2]);
    pal[3] = dxtTo1555_2E78(pal[3]);
    unsigned int rowIndices;
    rowIndices = ((unsigned char*)dst)[4];
    ((ulong*)src)[0] = pal[rowIndices & 3];
    ((ulong*)src)[1] = pal[(rowIndices >> 2) & 3];
    ((ulong*)src)[2] = pal[(rowIndices >> 4) & 3];
    ((ulong*)src)[3] = pal[rowIndices >> 6];
    src += pitch;
    rowIndices = ((unsigned char*)dst)[5];
    ((ulong*)src)[0] = pal[rowIndices & 3];
    ((ulong*)src)[1] = pal[(rowIndices >> 2) & 3];
    ((ulong*)src)[2] = pal[(rowIndices >> 4) & 3];
    ((ulong*)src)[3] = pal[rowIndices >> 6];
    src += pitch;
    rowIndices = ((unsigned char*)dst)[6];
    ((ulong*)src)[0] = pal[rowIndices & 3];
    ((ulong*)src)[1] = pal[(rowIndices >> 2) & 3];
    ((ulong*)src)[2] = pal[(rowIndices >> 4) & 3];
    ((ulong*)src)[3] = pal[rowIndices >> 6];
    src += pitch;
    rowIndices = ((unsigned char*)dst)[7];
    ((ulong*)src)[0] = pal[rowIndices & 3];
    ((ulong*)src)[1] = pal[(rowIndices >> 2) & 3];
    ((ulong*)src)[2] = pal[(rowIndices >> 4) & 3];
    ((ulong*)src)[3] = pal[rowIndices >> 6];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", hasAlpha);
#ifdef SKIP_ASM
/**
 * Tells whether DXT-compressed data has any alpha that is not fully opaque.
 *
 * Checks @p size / 16 blocks. Type 3 steps 16 bytes and needs both 64-bit words of each block to equal
 * 0xFFFFFFFF; any other type steps 32 bytes and needs the first 16-bit word to be 0xFFFF.
 *
 * @param buf   the compressed data
 * @param size  number of pixels (blocks are counted as size / 16)
 * @param type  DXT type (On Tour: dxttype)
 * @return 1 at the first block that fails the test, 0 when all pass
 */
// PORT: 64-bit `long` compares (ld); `long` is 8 bytes on this compiler.
extern "C" int hasAlpha(void* buf, int size, int type)
{
    if (type == 3) {
        unsigned long* pixel32 = (unsigned long*)buf;
        int n = size / 16;
        int i;
        for (i = 0; i < n; i++) {
            unsigned long first = *pixel32++;
            if (first != 0xFFFFFFFF) {
                return 1;
            }
            if (*pixel32 != first) {
                return 1;
            }
            pixel32 += 3;
        }
    } else {
        unsigned short* pixel16 = (unsigned short*)buf;
        int n = size / 16;
        int i;
        for (i = 0; i < n; i++) {
            if (*pixel16 != 0xFFFF) {
                return 1;
            }
            pixel16 += 16;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", undxtpack);
#ifdef SKIP_ASM
extern "C" int hasAlpha(void* buf, int size, int type);
extern "C" void decodeRGB(char* src, int pitch, char* dst);
extern "C" void decodeAlpha(char* src, int pitch, char* dst, int fmt);

/**
 * Decompresses a DXT-compressed image into pixels, one 4x4 block at a time.
 *
 * Types 3 and 5 use 16-byte blocks: decodeRGB() decodes the colour half (+8), and decodeAlpha() the alpha half
 * only when hasAlpha() finds non-opaque alpha. Other types use 8-byte colour-only blocks.
 *
 * @param out  the compressed data (On Tour: compressed); despite the name it is the input
 * @param src  the destination pixels (On Tour: uncompressed); despite the name it is the output
 * @param w    width in pixels
 * @param height    height in pixels
 * @param fmt  DXT type (On Tour: dxttype)
 */
// Compress a w x h image into 4x4 blocks.
extern "C" void undxtpack(void* out, char* src, int w, int height, int fmt)
{
    char* dst = (char*)out;
    int x, y;
    int n = w * height;
    if (fmt == 3 || fmt == 5) {
        if (hasAlpha(dst, n, fmt)) {
            for (y = 0; y < height; y += 4) {
                for (x = 0; x < w; x += 4) {
                    decodeRGB(src, w * 8, dst + 8);
                    decodeAlpha(src, w * 8, dst, fmt);
                    src += 0x20;
                    dst += 0x10;
                }
                src += w * 0x18;
            }
        } else {
            for (y = 0; y < height; y += 4) {
                for (x = 0; x < w; x += 4) {
                    decodeRGB(src, w * 8, dst + 8);
                    src += 0x20;
                    dst += 0x10;
                }
                src += w * 0x18;
            }
        }
    } else {
        for (y = 0; y < height; y += 4) {
            for (x = 0; x < w; x += 4) {
                decodeRGB(src, w * 4, dst);
                src += 0x20;
                dst += 8;
            }
            src += w * 0x18;
        }
    }
}
#endif

