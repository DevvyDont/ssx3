#include "common.h"
#include "sound/bankmonitor.h"
#include "sound/streaminstance.h"
#include "sound/streamsys.h"

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_cStreamInstance);
#ifdef SKIP_ASM
struct sSsEnt_99D8 { char vol; char pad[0x17]; };
struct sSsPlayer_99D8 {
    char pad0[0x1C];
    volatile int cur;           // 0x1C
    sSsEnt_99D8* ents;          // 0x20
    char pad24[0x38 - 0x24];
    int* vols;                  // 0x38
    float** scales;             // 0x3C
};
struct sStreamInst_99D8 {
    char* mem;          // 0x0
    int f4;             // 0x4
    int handle;         // 0x8
    char vol;           // 0xC
    float* scale;       // 0x10
    char pad14[0x74 - 0x14];
    int f74;            // 0x74
    int f78;            // 0x78
    int heap;           // 0x7C
    int f80;            // 0x80
};
extern "C" int SNDSTRM_create(void* desc, int n, int m, void* mem, int size);
extern "C" int SNDSTRM_overhead(int n, int m);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_Stream[];

static inline int curVol_99D8(sSsPlayer_99D8* p) { return p->vols[p->cur]; }

#define MIN_99D8(a, b) ((a) < (b) ? (a) : (b))
#define MAX_99D8(a, b) ((a) >= (b) ? (a) : (b))

/**
 * Constructs a sound stream instance from play options: applies the volume scale and opens the stream.
 *
 * Scales the options' current volume by their volume-scale pointer (clamped to 0-127), allocates the stream
 * buffer ("Stream": the library's size for 8 requests of 300 plus @p extra) and opens the stream into it.
 *
 * @param p  play options (On Tour cSndPlayOpts* PlayOpts)
 * @param heap  memory type for the buffer (On Tour MemType)
 * @param extra  extra buffer space in bytes (On Tour BuffSpace)
 * @return @p self
 */
extern "C" sStreamInst_99D8* cStreamInstance_cStreamInstance(sStreamInst_99D8* self, sSsPlayer_99D8* p, int heap, int extra)
{
    self->f74 = 0;
    self->handle = -1;
    self->f78 = 0;
    self->heap = heap;
    self->f80 = 0;
    self->scale = p->scales[p->cur];
    self->vol = curVol_99D8(p);
    if (self->scale) {
        p->vols[p->cur] = (int)MAX_99D8(MIN_99D8((float)p->vols[p->cur] * *self->scale, 127.0f), 0.0f);
        p->ents[p->cur].vol = MAX_99D8(MIN_99D8(p->vols[p->cur], 127), 0);
    }
    int n = SNDSTRM_overhead(8, 300) + extra;
    self->mem = new (str_Stream, heap, 0) char[n];
    self->f4 = 0;
    self->handle = SNDSTRM_create(&p->ents[p->cur], 8, 300, self->mem, n);
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_cStreamInstance1);
#ifdef SKIP_ASM
/**
 * Constructs a sound stream instance in caller-supplied memory, from the current play-option level.
 *
 * Takes the level's volume control and volume (scaled by it and clamped to 0..127, as cStreamInstance_cStreamInstance
 * does), keeps @p mem as the buffer without owning it (+4 = 1) and opens the stream with SNDSTRM_create(.., @p n,
 * @p maxChunks, mem, size). cStreamSys_STREAM_CreateInstance1 runs it.
 *
 * @param p  the play options (cBankManager + 0x1D8)
 * @param mem  the stream buffer
 * @param size  the buffer size
 * @param n  the maximum number of requests
 * @param maxChunks  the maximum number of chunks
 * @return this
 */
extern "C" sStreamInst_99D8* cStreamInstance_cStreamInstance1(sStreamInst_99D8* self, sSsPlayer_99D8* p, char* mem, int size, int n, int maxChunks)
{
    self->scale = p->scales[p->cur];
    self->vol = curVol_99D8(p);
    if (self->scale) {
        p->vols[p->cur] = (int)MAX_99D8(MIN_99D8((float)p->vols[p->cur] * *self->scale, 127.0f), 0.0f);
        p->ents[p->cur].vol = MAX_99D8(MIN_99D8(p->vols[p->cur], 127), 0);
    }
    self->mem = mem;
    self->f4 = 1;
    self->handle = SNDSTRM_create(&p->ents[p->cur], n, maxChunks, mem, size);
    return self;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_cStreamInstance2);
#ifdef SKIP_ASM
/**
 * Constructs a stream instance around an existing stream handle: no buffer, no big-file data.
 *
 * @param stream  this
 * @param handle  the sound-driver stream handle
 * @return this
 */
extern "C" void *cStreamInstance_cStreamInstance2(void *stream, int handle) {
    (*(int *)((char*)(stream) + (8))) = handle;
    (*(int *)((char*)(stream) + (0))) = 0;
    (*(int *)((char*)(stream) + (0x74))) = 0;
    (*(int *)((char*)(stream) + (0x78))) = 0;
    return stream;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance__cStreamInstance);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" int SNDSTRM_destroy(int h);
extern "C" void* MEM_free(void* self);

/**
 * Destroys a stream instance: closes the stream, frees an owned buffer and the big-file data, and with bit 0 of
 * @p flags deletes it.
 *
 * SNDSTRM_destroy runs when there is a buffer; it is freed (cMemMan_free) unless it was supplied (+4); the big-file
 * directory (+0x74) is freed with MEM_free.
 *
 * @param flags  bit 0 set to also delete the object
 */
extern "C" void cStreamInstance__cStreamInstance(cStreamInstance* self, int flags)
{
    if (*(void**)self != 0) {
        SNDSTRM_destroy(self->mHandle);
        if (self->mExternalBuffer == 0 && *(void**)self != 0) {
            cMemMan_free(*(void**)self);
        }
    }
    if (self->mBigHeader != 0) {
        MEM_free(self->mBigHeader);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_SetBigFile);
#ifdef SKIP_ASM
extern "C" int BXFILE_exists(const char* name);
extern "C" int FILE_loadbigheader(const char* name, int a1);

/**
 * Uses a big file for the stream's requests: when it exists, keeps its name (+0x14) and loads its directory (+0x74,
 * FILE_loadbigheader with the instance's memory type).
 *
 * @param name  the big-file name
 */
// PORT: second parameter is a const char* passed as int (unit declares cStreamInstance_SetBigFile(void*, int)).
extern "C" void cStreamInstance_SetBigFile(cStreamInstance* self, int name)
{
    if (BXFILE_exists((const char*)name) != 0) {
        strcpy((char*)&self->mBigFileName, (const char*)name);
        self->mBigHeader = (char*)FILE_loadbigheader((char*)&self->mBigFileName, self->mMemAllocType);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_Set3DPosition);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" void func_003B7F60(int h, int a, int b);

/**
 * Sets a stream's 3D position: azimuth and elevation (func_003B7F60, in a sound-driver critical section).
 *
 * @param azimuth  the azimuth
 * @param elevation  the elevation
 */
extern "C" void cStreamInstance_Set3DPosition(cStreamInstance* self, int azimuth, int elevation)
{
    SNDSYS_entercritical();
    func_003B7F60(self->mHandle, azimuth, elevation);
    SNDSYS_leavecritical();
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_SetBaseVolume__FPvi);
#ifdef SKIP_ASM
/**
 * Sets a stream instance's base volume.
 *
 * Stores the low byte of @p val at +0xC.
 *
 * @param val  the volume, 0..127 (On Tour Vol)
 */
void cStreamInstance_SetBaseVolume(void* self, int val)
{
    *(char*)((char*)self + 0xC) = (char)val;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_SetPitch);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" void SNDSTRM_pitchmult(int h, int v);

/**
 * Sets a stream's pitch multiplier (SNDSTRM_pitchmult, in a sound-driver critical section).
 *
 * @param pitch  the pitch (0x1000 = normal)
 */
extern "C" void cStreamInstance_SetPitch(cStreamInstance* self, int pitch)
{
    SNDSYS_entercritical();
    SNDSTRM_pitchmult(self->mHandle, pitch);
    SNDSYS_leavecritical();
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_Update);
#ifdef SKIP_ASM
int cAudioInterface_getTimerHz(void* self);
void* cStreamInstance_Purge(void* self);
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" void func_003B7E70(int h, int vol);

/**
 * Updates a stream instance: purges it once its counter (+0x80) passes twice the timer rate, otherwise re-applies the
 * volume scaled by its volume control (clamped 0..127, func_003B7E70).
 */
extern "C" void cStreamInstance_Update(cStreamInstance* self)
{
    if (cAudioInterface_getTimerHz(self) * 2 < self->mQueuedTimeout) {
        cStreamInstance_Purge(self);
        self->mQueuedTimeout = 0;
        return;
    }
    float* volumeScale = self->mVolControl;
    if (volumeScale != 0) {
        int volume = (int)((float)self->mBaseVol * *volumeScale);
        if (volume > 0x7F) {
            volume = 0x7F;
        }
        if (volume < 0) {
            volume = 0;
        }
        SNDSYS_entercritical();
        func_003B7E70(self->mHandle, volume);
        SNDSYS_leavecritical();
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_GetStatus);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" int SNDSTRM_status(int h, void* out);

// PORT: returns int; the unit declares it void(void*, void*) further down
int cStreamInstance_GetStatus_impl(void* self, void* out) __asm__("cStreamInstance_GetStatus");

/**
 * Reads a stream's status from the sound driver (SNDSTRM_status, in a critical section).
 *
 * @param out  receives the status (SNDSTREAMSTATUS)
 * @return the driver's result
 */
int cStreamInstance_GetStatus_impl(void* self, void* out)
{
    int result;
    SNDSYS_entercritical();
    result = SNDSTRM_status(*(int*)((char*)self + 0x8), out);
    SNDSYS_leavecritical();
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamInstance_GetRequestStatus);
#ifdef SKIP_ASM
extern "C" void cStreamInstance_GetStatus(void*, void*);
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" int SNDSTRM_requeststatus(int h, int a1);

// PORT: returns int and takes (self, int, out); later callers in this unit declare it void(void*, sStatus*, int)
int cStreamInstance_GetRequestStatus_impl(void* self, int a1, int* out) __asm__("cStreamInstance_GetRequestStatus");

/**
 * Reads the status of a stream's current request: the stream status first (into @p out, or a local buffer), then
 * SNDSTRM_requeststatus of the request it names.
 *
 * @param requestStatus  receives the request status (SNDREQUESTSTATUS)
 * @param out  receives the stream status, or 0
 * @return the driver's result
 */
int cStreamInstance_GetRequestStatus_impl(void* self, int requestStatus, int* out)
{
    int buf[4];
    int* status = out ? out : buf;
    int result;
    cStreamInstance_GetStatus(self, status);
    SNDSYS_entercritical();
    result = SNDSTRM_requeststatus(status[1], requestStatus);
    SNDSYS_leavecritical();
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_GetTime);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cStreamInstance_GetTime_sStatus {
    int state;
    int a;
    int b;
    int c;
};

// the unit declares cStreamInstance_GetRequestStatus later with a different pointer type; alias it
void cStreamInstance_GetRequestStatus_status(void* self, cStreamInstance_GetTime_sStatus* out, int flags) __asm__("cStreamInstance_GetRequestStatus");

/**
 * Returns the played time of the current request while it plays (request state 2), else 0.
 *
 * @return the time
 */
extern "C" float cStreamInstance_GetTime(cStreamInstance* self)
{
    cStreamInstance_GetTime_sStatus st;
    cStreamInstance_GetRequestStatus_status(self, &st, 0);
    if (st.state == 2) {
        return (float)st.a;
    }
    return 0.0f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamInstance_GetTimeRemaining);
#ifdef SKIP_ASM
struct cStreamInstance_Playing_sStatus;
extern "C" void cStreamInstance_GetRequestStatus(void* self, cStreamInstance_Playing_sStatus* out, int flags);

struct cStreamInstance_GetTimeRemaining_sStatus {
    int state;
    int a;
    int b;
    int c;
};

/**
 * Returns the remaining time of the current request while it plays (request state 2), else 0.
 *
 * @return the time
 */
extern "C" float cStreamInstance_GetTimeRemaining(cStreamInstance* self)
{
    cStreamInstance_GetTimeRemaining_sStatus st;
    cStreamInstance_GetRequestStatus(self, (cStreamInstance_Playing_sStatus*)&st, 0);
    if (st.state == 2) {
        return (float)st.b;
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_IsBufferFilled);
#ifdef SKIP_ASM
struct cStreamInstance_IsBufferFilled_sStatus {
    int state;
    int a;
    int b;
    int c;
};

/**
 * Tells whether a stream has at least the given amount buffered (status word 1 >= @p minVal).
 *
 * @param minVal  the amount (milliseconds)
 * @return 1 or 0
 */
extern "C" int cStreamInstance_IsBufferFilled(cStreamInstance* self, int minVal)
{
    cStreamInstance_IsBufferFilled_sStatus st;
    cStreamInstance_GetStatus(self, &st);
    return st.b >= minVal;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_IsCurrRequestedRead);
#ifdef SKIP_ASM
struct cStreamInstance_IsCurrRequestedRead_sStatus {
    int state;
    int a;
    int b;
    int c;
};

/**
 * Tells whether the current request has been read (request state 1).
 *
 * @return 1 or 0
 */
extern "C" int cStreamInstance_IsCurrRequestedRead(cStreamInstance* self)
{
    cStreamInstance_IsCurrRequestedRead_sStatus st;
    cStreamInstance_GetRequestStatus(self, (cStreamInstance_Playing_sStatus*)&st, 0);
    return st.state == 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamInstance_Playing);
#ifdef SKIP_ASM
struct cStreamInstance_Playing_sStatus {
    int state;
    int a;
    int b;
    int c;
};

extern "C" void cStreamInstance_GetRequestStatus(void* self, cStreamInstance_Playing_sStatus* out, int flags);

/**
 * Tells whether the current request is playing (request state 2).
 *
 * @return 1 or 0
 */
extern "C" int cStreamInstance_Playing(cStreamInstance* self)
{
    cStreamInstance_Playing_sStatus st;
    cStreamInstance_GetRequestStatus(self, &st, 0);
    return st.state == 2;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_ReleaseHold);
#ifdef SKIP_ASM
extern "C" void SNDSTRM_modifyhold(int, int);

/**
 * Releases the hold on the stream's current request (SNDSTRM_modifyhold(request, 0)).
 */
extern "C" void cStreamInstance_ReleaseHold(cStreamInstance* self)
{
    int buf[2];
    cStreamInstance_GetStatus(self, buf);
    SNDSTRM_modifyhold(buf[1], 0);
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_GetCurrActiveSampleHandle);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// cStreamInstance_GetStatus returns the status (the unit declares it void); the int return keeps $2 free as the target has it.
int cStreamInstance_GetStatus_st(void* self, int* out) __asm__("cStreamInstance_GetStatus");

/**
 * Returns the stream's current request handle, or -1 when the stream has no request (status word 0 is 0).
 *
 * @return the request handle, or -1
 */
extern "C" int cStreamInstance_GetCurrActiveSampleHandle(cStreamInstance* self)
{
    int st[2];
    cStreamInstance_GetStatus_st(self, st);
    if (st[0] == 0) {
        return -1;
    }
    return st[1];
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_ReleaseHold1);
#ifdef SKIP_ASM
extern "C" void SNDSTRM_modifyhold(int, int);

/**
 * Releases the hold on a given request of the stream (SNDSTRM_modifyhold(requestHandle, 0)).
 *
 * @param requestHandle  the request
 */
extern "C" void cStreamInstance_ReleaseHold1(cStreamInstance* self, int requestHandle)
{
    SNDSTRM_modifyhold(requestHandle, 0);
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_GetRequestLength);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cStreamInstance_GetRequestLength_sStatus {
    int state;
    int a;
    int b;
    int c;
};

void cStreamInstance_GetRequestStatus_status(void* self, cStreamInstance_GetRequestLength_sStatus* out, int flags) __asm__("cStreamInstance_GetRequestStatus");

/**
 * Returns the length of the current request (played plus remaining time) while it plays, else 0.
 *
 * @return the length
 */
extern "C" float cStreamInstance_GetRequestLength(cStreamInstance* self)
{
    cStreamInstance_GetRequestLength_sStatus st;
    cStreamInstance_GetRequestStatus_status(self, &st, 0);
    if (st.state == 2) {
        return (float)(st.a + st.b);
    }
    return 0.0f;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_Pause);
#ifdef SKIP_ASM
/**
 * Pauses the stream: pitch multiplier 0 (SNDSTRM_pitchmult, in a critical section).
 *
 * @param stream  this
 */
extern "C" void cStreamInstance_Pause(cStreamInstance* stream) {
    SNDSYS_entercritical();
    SNDSTRM_pitchmult((stream->mHandle), 0);
    SNDSYS_leavecritical();
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_Resume);
#ifdef SKIP_ASM
/**
 * Resumes the stream: pitch multiplier 0x1000 (SNDSTRM_pitchmult, in a critical section).
 *
 * @param stream  this
 */
extern "C" void cStreamInstance_Resume(cStreamInstance* stream) {
    SNDSYS_entercritical();
    SNDSTRM_pitchmult((stream->mHandle), 0x1000);
    SNDSYS_leavecritical();
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_Queue);
#ifdef SKIP_ASM
extern "C" int SNDSTRM_queuefile(int h, int a, int buf, int off);
extern "C" void BIG_locateentry(void* dec, int src, int a, int* out, int b);

/**
 * Queues a file on the stream: through the big file when one is set (its entry for @p buf adds its offset), else the
 * file itself (SNDSTRM_queuefile).
 *
 * @param holdTime  the hold time of the request
 * @param buf  the file name
 * @param off  the offset in the file
 * @return the request handle
 */
// PORT: buf is a pointer carried in an int (the unit's caller declares this function with int params).
extern "C" int cStreamInstance_Queue(cStreamInstance* self, int holdTime, int buf, int off)
{
    if (self->mBigHeader != 0) {
        int n;
        BIG_locateentry(self->mBigHeader, buf, 0, &n, 0);
        return SNDSTRM_queuefile(self->mHandle, holdTime, (int)((char*)self + 0x14), off + n);
    }
    return SNDSTRM_queuefile(self->mHandle, holdTime, buf, off);
}
#endif

extern "C" void* SNDSTRM_purge(int);

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_Queue1);
#ifdef SKIP_ASM
/**
 * Queues a big-file element on the stream (by index, BIG_locateentry); 0 when no big file is set.
 *
 * @param holdTime  the hold time of the request
 * @param element  the element index in the big file
 * @param off  the offset in the element
 * @return the request handle, or 0
 */
extern "C" int cStreamInstance_Queue1(cStreamInstance* self, int holdTime, int element, int off)
{
    void* dec = self->mBigHeader;
    if (dec != 0) {
        int n;
        BIG_locateentry(dec, 0, element, &n, 0);
        return SNDSTRM_queuefile(self->mHandle, holdTime, (int)((char*)self + 0x14), off + n);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_Purge__FPv);
#ifdef SKIP_ASM
/**
 * Purges (stops and releases) a stream instance's stream.
 *
 * Calls SNDSTRM_purge (On Tour SNDSTRM_purge) on the stream handle at +0x8.
 *
 * @return SNDSTRM_purge's result
 */
void* cStreamInstance_Purge(void* self)
{
    return SNDSTRM_purge(*(int*)((char*)self + 0x8));
}
#endif

extern "C" void cStreamInstance_GetStatus(void*, void*);

//99.38% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("sound/streamsys", cStreamInstance_Requests);
#ifdef SKIP_ASM
/**
 * Returns the number of requests queued on the stream (status word 0).
 *
 * @return the count
 */
extern "C" int cStreamInstance_Requests(cStreamInstance* self)
{
    int buf[4];
    cStreamInstance_GetStatus(self, buf);
    return buf[0];
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_TimeMult);
#ifdef SKIP_ASM
extern "C" int SNDSTRM_timemult(int, int);

/**
 * Multiplies the stream's timing (SNDSTRM_timemult, in a critical section) and returns the driver's result.
 *
 * @param stream  this
 * @param multAmount  the multiplier
 * @return the driver's result
 */
extern "C" int cStreamInstance_TimeMult(cStreamInstance* stream, int multAmount) {
    int result;

    SNDSYS_entercritical();
    result = SNDSTRM_timemult((stream->mHandle), multAmount);
    SNDSYS_leavecritical();
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_cStreamSys);
#ifdef SKIP_ASM
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_StreamArray[];
extern void* cBankMonitor_vtbl[];

struct sSsVEntryA490 {
    short delta;
    short index;
    void* fn;
};
struct sSsVtblAA490 {
    sSsVEntryA490 e[4];
} __attribute__((aligned(8)));
struct sSsVtblBA490 {
    sSsVEntryA490 e[7];
} __attribute__((aligned(8)));
extern const sSsVtblAA490 D_00483998;
extern const sSsVtblBA490 D_004839B8;

struct sStreamSysA490 {
    char* vbase;        // 0x0
    int count;          // 0x4
    void** items;       // 0x8
    char vb2[0x8];      // 0xC
    char vb[0x4];       // 0x14
};

/**
 * Constructs the stream system part of the audio system and allocates its stream table.
 *
 * With @p inchrg set it first builds the shared virtual bases (cSoundSys, cBankManager); otherwise it patches
 * stack copies of the base vtables with this object's offset. The table ("StreamArray") starts empty.
 *
 * @param inchrg  g++ in-charge flag: nonzero when the virtual bases are built here
 * @param banks  maximum number of banks (On Tour MaxBanks)
 * @param count  maximum number of streams (On Tour MaxStreams)
 * @param heap  audio mode (eSOUNDDEF_MODE), passed to the base constructors
 * @return @p self
 */
extern "C" sStreamSysA490* cStreamSys_cStreamSys(sStreamSysA490* self, int inchrg, int banks, int count, int heap)
{
    if (inchrg) {
        self->vbase = self->vb;
        *(char**)(self->vb + 0x1D8) = self->vb2;
        cSoundSys_cSoundSys(self->vb2, heap);
        cBankManager_cBankManager(self->vbase, 0, banks, heap);
    }
    *(const sSsVtblAA490**)(*(char**)(self->vbase + 0x1D8) + 4) = &D_00483998;
    *(void***)(self->vbase + 0xAB0) = cBankMonitor_vtbl;
    *(const sSsVtblBA490**)(self->vbase + 0x1D4) = &D_004839B8;
    if (!inchrg) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-ups (copied vtables on the stack), written out by hand.
        sSsVtblAA490 vtCopyA = D_00483998;
        *(sSsVtblAA490**)(*(char**)(self->vbase + 0x1D8) + 4) = &vtCopyA;
        char* base1 = *(char**)(self->vbase + 0x1D8) - 0xC;
        int adjustA = (char*)self - base1;
        vtCopyA.e[1].delta = D_00483998.e[1].delta + adjustA;
        sSsVtblBA490 vtCopyB = D_004839B8;
        *(sSsVtblBA490**)(self->vbase + 0x1D4) = &vtCopyB;
        char* base2 = self->vbase - 0x14;
        int adjustB = (char*)self - base2;
        vtCopyB.e[1].delta = D_004839B8.e[1].delta + adjustB;
    }
    self->count = count;
    self->items = new (str_StreamArray, 0, 0) void*[count];
    for (int i = 0; i < self->count; i++)
        self->items[i] = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys__cStreamSys);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void cBankManager__cBankManager(void* self, int flags);
extern "C" void cStreamInstance__cStreamInstance(cStreamInstance* e, int a1);
extern "C" void cSoundSys__cSoundSys(void* self, int flags);
extern void* cBankMonitor_vtbl[];

struct sSsVEntryA648 {
    short delta;
    short index;
    void* fn;
};
struct sSsVtblAA648 {
    sSsVEntryA648 e[4];
} __attribute__((aligned(8)));
struct sSsVtblBA648 {
    sSsVEntryA648 e[7];
} __attribute__((aligned(8)));
// Views of the vtables cStreamSys_cStreamSys (earlier in this unit) declares with its own types.
extern const sSsVtblAA648 D_00483998_A648 __asm__("D_00483998");
extern const sSsVtblBA648 D_004839B8_A648 __asm__("D_004839B8");

/**
 * Destroys the stream system: re-installs its vtables, destroys every stream instance and frees the array; with bit 1
 * of @p flags also the virtual bases cBankManager and cSoundSys, with bit 0 the object.
 *
 * Without bit 1 (a base of a bigger object) the vtables are temporary copies with adjusted deltas.
 * cController__cController, cSSXAudio__cSSXAudio, cSpeechSys__cSpeechSys and func_002B3398 call it.
 *
 * @param streamSysObj  the stream system
 * @param flags  bit 1: destroy the virtual bases; bit 0: delete the object
 */
// PORT: g++ 2.95 virtual-base destruction with stack vtable this-adjust fix-ups, written out by hand.
extern "C" void cStreamSys__cStreamSys(cStreamSys* streamSysObj, int flags)
{
    *(const sSsVtblAA648**)(*(char**)(*(char**)streamSysObj + 0x1D8) + 4) = &D_00483998_A648;
    *(void***)(*(char**)streamSysObj + 0xAB0) = cBankMonitor_vtbl;
    *(const sSsVtblBA648**)(*(char**)streamSysObj + 0x1D4) = &D_004839B8_A648;
    if (flags == 0) {
        sSsVtblAA648 soundSysVtbl = D_00483998_A648;
        *(sSsVtblAA648**)(*(char**)(*(char**)streamSysObj + 0x1D8) + 4) = &soundSysVtbl;
        char* base1 = *(char**)(*(char**)streamSysObj + 0x1D8) - 0xC;
        int soundSysDelta = ((char*)streamSysObj) - base1;
        soundSysVtbl.e[1].delta = D_00483998_A648.e[1].delta + soundSysDelta;
        sSsVtblBA648 bankMgrVtbl = D_004839B8_A648;
        *(sSsVtblBA648**)(*(char**)streamSysObj + 0x1D4) = &bankMgrVtbl;
        char* base2 = *(char**)streamSysObj - 0x14;
        int bankMgrDelta = ((char*)streamSysObj) - base2;
        bankMgrVtbl.e[1].delta = D_004839B8_A648.e[1].delta + bankMgrDelta;
    }
    for (int i = 0; i < streamSysObj->mNumStreams; i++) {
        void* streamInst = ((void**)streamSysObj->mStreamInstance)[i];
        if (streamInst != 0) {
            cStreamInstance__cStreamInstance((cStreamInstance*)streamInst, 3);
        }
    }
    if (streamSysObj->mStreamInstance != 0) {
        cMemMan_free(streamSysObj->mStreamInstance);
    }
    if (flags & 2) {
        cBankManager__cBankManager(*(void**)streamSysObj, 0);
        cSoundSys__cSoundSys(*(void**)(*(char**)streamSysObj + 0x1D8), 0);
    }
    if (flags & 1) {
        operator_delete((int*)streamSysObj);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_CreateInstance);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char str_StreamBuffer[];

/**
 * Creates stream instance @p idx: allocates 132 bytes ("StreamBuffer") and constructs it from the current play-option
 * level (cStreamInstance_cStreamInstance with @p heap and @p extra).
 *
 * @param idx  the stream index
 * @param heap  the memory type
 * @param extra  the extra buffer space
 */
extern "C" void cStreamSys_STREAM_CreateInstance(cStreamSys* self, int idx, int heap, int extra)
{
    sStreamInst_99D8** slot = (sStreamInst_99D8**)((idx << 2) + (int)self->mStreamInstance);
    void* mem = cMemMan_alloc(132, str_StreamBuffer, heap, 0);
    *slot = cStreamInstance_cStreamInstance((sStreamInst_99D8*)mem, (sSsPlayer_99D8*)(*(char**)self + 0x1D8), heap, extra);
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_CreateInstance1);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char str_StreamBuffer[];
extern "C" sStreamInst_99D8* cStreamInstance_cStreamInstance1(sStreamInst_99D8* self, sSsPlayer_99D8* p, char* mem, int size, int n, int m);

/**
 * Creates stream instance @p idx in caller-supplied memory: allocates the 132-byte object ("StreamBuffer") and runs
 * cStreamInstance_cStreamInstance1 with the buffer and counts.
 *
 * @param idx  the stream index
 * @param heap  the memory type of the object
 * @param mem  the stream buffer
 * @param size  the buffer size
 * @param n  the maximum number of requests
 * @param maxChunks  the maximum number of chunks
 */
extern "C" void cStreamSys_STREAM_CreateInstance1(cStreamSys* self, int idx, int heap, char* mem, int size, int n, int maxChunks)
{
    sStreamInst_99D8** slot = (sStreamInst_99D8**)((idx << 2) + (int)self->mStreamInstance);
    void* obj = cMemMan_alloc(132, str_StreamBuffer, heap, 0);
    *slot = cStreamInstance_cStreamInstance1((sStreamInst_99D8*)obj, (sSsPlayer_99D8*)(*(char**)self + 0x1D8), mem, size, n, maxChunks);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_Create);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void cStreamSys_STREAM_CreateInstance(cStreamSys* self, int a1, int a2, int a3);

struct sEntry_A910 {
    signed char kind;
    char pad[0x17];
};
struct sQuad_A910 {
    int a;
    int b;
    int c;
    int d;
};
struct sStack_A910 {
    int field_0x0;                  // 0x1D8
    sEntry_A910 cur;                // 0x1DC
    volatile int depth;             // 0x1F4
    sEntry_A910* entries;           // 0x1F8
    int flags[4];                   // 0x1FC
    int* a34;
    int* a38;
    int* a3C;
    char a40[4][8];
    int* a60;
    int* a64;
    int* a68;
    int* a6C;
    sQuad_A910* a70;
    int* a74;
    int* a78;
    int* a7C;
    int* a80;
    int* a84;
};
struct sMgr_A910 {
    char pad_0x0[0x1D8];
    sStack_A910 st;                 // 0x1D8
};

/**
 * Creates a stream with a volume control: pushes a play-option level with the defaults, sets its volume control,
 * creates the instance (cStreamSys_STREAM_CreateInstance) and pops the level.
 *
 * @param streamIndex  the stream index
 * @param volControl  the volume control (float*)
 * @param memType  the memory type
 * @param buffSpace  the extra buffer space
 */
extern "C" void cStreamSys_STREAM_Create(sMgr_A910** self, int streamIndex, int volControl, int memType, int buffSpace)
{
    sMgr_A910* bankMgr = *self;
    sStack_A910* st = &bankMgr->st;
    bankMgr->st.depth++;
    st->entries[bankMgr->st.depth] = bankMgr->st.cur;
    bankMgr->st.flags[bankMgr->st.depth] = 0;
    st->a34[bankMgr->st.depth] = 0;
    st->a38[bankMgr->st.depth] = st->entries[bankMgr->st.depth].kind;
    st->a3C[bankMgr->st.depth] = 0;
    func_00416210((char*)st + bankMgr->st.depth * 8 + 0x40, 0, 8);
    st->a60[bankMgr->st.depth] = 0;
    st->a64[bankMgr->st.depth] = 0;
    st->a68[bankMgr->st.depth] = 0;
    st->a6C[bankMgr->st.depth] = 0;
    st->a70[bankMgr->st.depth].b = 0;
    st->a70[bankMgr->st.depth].a = 100;
    st->a70[bankMgr->st.depth].c = 90;
    st->a70[bankMgr->st.depth].d = 50;
    st->a74[bankMgr->st.depth] = 0;
    st->a80[bankMgr->st.depth] = 0;
    st->a84[bankMgr->st.depth] = 0;
    st->a78[bankMgr->st.depth] = 127;
    st->a7C[bankMgr->st.depth] = 1;
    (*self)->st.a3C[(*self)->st.depth] = volControl;
    cStreamSys_STREAM_CreateInstance((cStreamSys*)self, streamIndex, memType, buffSpace);
    (*self)->st.depth--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_SetBigFile);
#ifdef SKIP_ASM
extern "C" void cStreamInstance_SetBigFile(cStreamInstance*, int);

/**
 * Sets the big file of stream @p i (cStreamInstance_SetBigFile), when that stream exists.
 *
 * @param i  the stream index
 * @param val  the big-file name
 */
extern "C" void cStreamSys_STREAM_SetBigFile(cStreamSys* self, int i, int val)
{
    void* stream = ((void**)self->mStreamInstance)[i];
    if (stream != 0) {
        cStreamInstance_SetBigFile((cStreamInstance*)stream, val);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_Destroy);
#ifdef SKIP_ASM
extern "C" void cStreamInstance__cStreamInstance(cStreamInstance* e, int a1);

/**
 * Destroys stream @p i (cStreamInstance__cStreamInstance with flags 3) and clears its slot.
 *
 * @param i  the stream index
 */
extern "C" void cStreamSys_STREAM_Destroy(cStreamSys* self, int i)
{
    void* stream = ((void**)self->mStreamInstance)[i];
    if (stream != 0) {
        cStreamInstance__cStreamInstance((cStreamInstance*)stream, 3);
    }
    ((void**)self->mStreamInstance)[i] = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_Update);
#ifdef SKIP_ASM
extern "C" void cStreamInstance_Update(cStreamInstance* e);

/**
 * Updates every stream instance (cStreamInstance_Update); cController_Update calls it each frame.
 */
extern "C" void cStreamSys_STREAM_Update(cStreamSys* self)
{
    for (int i = 0; i < self->mNumStreams; i++) {
        void* stream = ((void**)self->mStreamInstance)[i];
        if (stream != 0) {
            cStreamInstance_Update((cStreamInstance*)stream);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_Pause);
#ifdef SKIP_ASM
extern "C" void cStreamInstance_Pause(cStreamInstance* p);

/**
 * Pauses every stream instance (cStreamInstance_Pause).
 */
extern "C" void cStreamSys_STREAM_Pause(cStreamSys* self)
{
    int i;
    for (i = 0; i < self->mNumStreams; i++) {
        void* stream = *(void**)((char*)self->mStreamInstance + i * 4);
        if (stream != 0) {
            cStreamInstance_Pause((cStreamInstance*)stream);
        }
    }
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_Resume);
#ifdef SKIP_ASM
extern "C" void cStreamInstance_Resume(cStreamInstance* p);

/**
 * Resumes every stream instance (cStreamInstance_Resume).
 */
extern "C" void cStreamSys_STREAM_Resume(cStreamSys* self)
{
    int i;
    for (i = 0; i < self->mNumStreams; i++) {
        void* stream = *(void**)((char*)self->mStreamInstance + i * 4);
        if (stream != 0) {
            cStreamInstance_Resume((cStreamInstance*)stream);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_Queue);
#ifdef SKIP_ASM
extern "C" int cStreamInstance_Queue(cStreamInstance* e, int a, int b, int c);

/**
 * Queues a file on stream @p i (cStreamInstance_Queue); -1 when the stream does not exist.
 *
 * @param i  the stream index
 * @param holdTime  the hold time
 * @param buf  the file name
 * @param offset  the offset in the file
 * @return the request handle, or -1
 */
extern "C" int cStreamSys_STREAM_Queue(cStreamSys* self, int i, int holdTime, int buf, int offset)
{
    void* stream = ((void**)self->mStreamInstance)[i];
    if (stream != 0) {
        return cStreamInstance_Queue((cStreamInstance*)stream, holdTime, buf, offset);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_Queue1);
#ifdef SKIP_ASM
extern "C" int cStreamInstance_Queue1(cStreamInstance*, int, int, int);

/**
 * Queues a big-file element on stream @p idx (cStreamInstance_Queue1); -1 when the stream does not exist.
 *
 * @param idx  the stream index
 * @param holdTime  the hold time
 * @param element  the element index
 * @param offset  the offset in the element
 * @return the request handle, or -1
 */
extern "C" int cStreamSys_STREAM_Queue1(cStreamSys* self, int idx, int holdTime, int element, int offset)
{
    void* stream = *(void**)((char*)self->mStreamInstance + idx * 4);
    if (stream != 0) {
        return cStreamInstance_Queue1((cStreamInstance*)stream, holdTime, element, offset);
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_Requests);
#ifdef SKIP_ASM
extern "C" int cStreamInstance_Requests(cStreamInstance*);

/**
 * Returns the number of requests queued on stream @p i (cStreamInstance_Requests), or 0 without the stream.
 *
 * @param i  the stream index
 * @return the count
 */
extern "C" int cStreamSys_STREAM_Requests(cStreamSys* self, int i)
{
    void* stream = ((void**)self->mStreamInstance)[i];
    if (stream != 0) {
        return cStreamInstance_Requests((cStreamInstance*)stream);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_Purge);
#ifdef SKIP_ASM
/**
 * Purges stream @p i (cStreamInstance_Purge), when it exists.
 *
 * @param i  the stream index
 */
extern "C" void cStreamSys_STREAM_Purge(cStreamSys* self, int i)
{
    void* stream = ((void**)self->mStreamInstance)[i];
    if (stream != 0) {
        cStreamInstance_Purge(stream);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_TimeRemaining);
#ifdef SKIP_ASM
extern "C" float cStreamInstance_GetTimeRemaining(cStreamInstance* self);

/**
 * Returns the remaining time of stream @p i's current request (cStreamInstance_GetTimeRemaining), or 0.
 *
 * @param i  the stream index (negative gives 0)
 * @return the time
 */
extern "C" float cStreamSys_STREAM_TimeRemaining(cStreamSys* self, int i)
{
    void* stream;
    if (i < 0 || (stream = ((void**)self->mStreamInstance)[i]) == 0) {
        return 0.0f;
    }
    return cStreamInstance_GetTimeRemaining((cStreamInstance*)stream);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_RequestLength);
#ifdef SKIP_ASM
extern "C" float cStreamInstance_GetRequestLength(cStreamInstance*);

/**
 * Returns the length of stream @p idx's current request (cStreamInstance_GetRequestLength), or 0.
 *
 * @param idx  the stream index (negative gives 0)
 * @return the length
 */
extern "C" float cStreamSys_STREAM_RequestLength(cStreamSys* self, int idx)
{
    void* stream;
    if (idx < 0 || (stream = *(void**)((char*)self->mStreamInstance + idx * 4)) == 0) {
        return 0.0f;
    }
    return cStreamInstance_GetRequestLength((cStreamInstance*)stream);
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_IsBufferFilled);
#ifdef SKIP_ASM
extern "C" int cStreamInstance_IsBufferFilled(cStreamInstance*, int);

/**
 * Tells whether stream @p idx has at least @p minVal buffered (cStreamInstance_IsBufferFilled); 0 without the stream.
 *
 * @param idx  the stream index
 * @param minVal  the amount (milliseconds)
 * @return 1 or 0
 */
extern "C" int cStreamSys_STREAM_IsBufferFilled(cStreamSys* self, int idx, int minVal)
{
    void* stream = *(void**)((char*)self->mStreamInstance + idx * 4);
    return stream != 0 ? cStreamInstance_IsBufferFilled((cStreamInstance*)stream, minVal) : 0;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_IsCurrRequestedRead);
#ifdef SKIP_ASM
extern "C" int cStreamInstance_IsCurrRequestedRead(cStreamInstance*);

/**
 * Tells whether stream @p idx's current request has been read (cStreamInstance_IsCurrRequestedRead); 0 without the stream.
 *
 * @param idx  the stream index
 * @return 1 or 0
 */
extern "C" int cStreamSys_STREAM_IsCurrRequestedRead(cStreamSys* self, int idx)
{
    void* stream = *(void**)((char*)self->mStreamInstance + idx * 4);
    return stream != 0 ? cStreamInstance_IsCurrRequestedRead((cStreamInstance*)stream) : 0;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_IsRequestedDequeued);
#ifdef SKIP_ASM
// PORT: the unit declares SNDSTRM_requeststatus(int, int); this call passes an out buffer
int SNDSTRM_requeststatus_pbuf(int h, int* out) __asm__("SNDSTRM_requeststatus");

/**
 * Tells whether a stream request is finished: its request status is 3 (dequeued) or the query fails.
 *
 * @param handle  the request handle
 * @return 1 or 0
 */
extern "C" int cStreamSys_STREAM_IsRequestedDequeued(cStreamSys* self, int handle)
{
    int st[4];
    int result;
    SNDSYS_entercritical();
    result = SNDSTRM_requeststatus_pbuf(handle, st);
    SNDSYS_leavecritical();
    return st[0] == 3 || result < 0;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_IsRequestedPlaying);
#ifdef SKIP_ASM
// PORT: the unit declares SNDSTRM_requeststatus(int, int); this call passes an out buffer
int SNDSTRM_requeststatus_pbuf(int h, int* out) __asm__("SNDSTRM_requeststatus");

/**
 * Tells whether a stream request is playing: its request status is 2 and the query succeeds.
 *
 * @param handle  the request handle
 * @return 1 or 0
 */
extern "C" int cStreamSys_STREAM_IsRequestedPlaying(cStreamSys* self, int handle)
{
    int st[4];
    int result;
    SNDSYS_entercritical();
    result = SNDSTRM_requeststatus_pbuf(handle, st);
    SNDSYS_leavecritical();
    return st[0] == 2 && !(result < 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_Playing);
#ifdef SKIP_ASM
extern "C" int cStreamInstance_Playing(cStreamInstance*);

/**
 * Tells whether stream @p i is playing (cStreamInstance_Playing); 0 without the stream.
 *
 * @param i  the stream index
 * @return 1 or 0
 */
extern "C" int cStreamSys_STREAM_Playing(cStreamSys* self, int i)
{
    void* stream = ((void**)self->mStreamInstance)[i];
    if (stream != 0) {
        return cStreamInstance_Playing((cStreamInstance*)stream);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_ReleaseHold);
#ifdef SKIP_ASM
extern "C" void cStreamInstance_ReleaseHold(cStreamInstance*);

/**
 * Releases the hold on stream @p idx's current request (cStreamInstance_ReleaseHold), when the stream exists.
 *
 * @param idx  the stream index
 */
extern "C" void cStreamSys_STREAM_ReleaseHold(cStreamSys* self, int idx)
{
    void* stream = *(void**)((char*)self->mStreamInstance + idx * 4);
    if (stream != 0) {
        cStreamInstance_ReleaseHold((cStreamInstance*)stream);
    }
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_ReleaseHold1);
#ifdef SKIP_ASM
extern "C" void cStreamInstance_ReleaseHold1(cStreamInstance*, int);

/**
 * Releases the hold on a given request of stream @p idx (cStreamInstance_ReleaseHold1), when the stream exists.
 *
 * @param idx  the stream index
 * @param requestHandle  the request
 */
extern "C" void cStreamSys_STREAM_ReleaseHold1(cStreamSys* self, int idx, int requestHandle)
{
    void* stream = *(void**)((char*)self->mStreamInstance + idx * 4);
    if (stream != 0) {
        cStreamInstance_ReleaseHold1((cStreamInstance*)stream, requestHandle);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_SetVolControl);
#ifdef SKIP_ASM
/**
 * Sets the volume control (float*) of stream @p i (its +0x10), when the stream exists.
 *
 * @param i  the stream index
 * @param val  the volume control
 */
extern "C" void cStreamSys_STREAM_SetVolControl(cStreamSys* self, int i, int val)
{
    void* stream = ((void**)self->mStreamInstance)[i];
    if (stream != 0) {
        *(int*)((char*)stream + 0x10) = val;
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_SetBaseVolume);
#ifdef SKIP_ASM
/**
 * Sets the base volume of stream @p idx (cStreamInstance_SetBaseVolume), when the stream exists.
 *
 * @param idx  the stream index
 * @param val  the volume
 */
extern "C" void cStreamSys_STREAM_SetBaseVolume(cStreamSys* self, int idx, int val)
{
    void* stream = *(void**)((char*)self->mStreamInstance + idx * 4);
    if (stream != 0) {
        cStreamInstance_SetBaseVolume(stream, val);
    }
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_GetHandle);
#ifdef SKIP_ASM
/**
 * Returns the sound-driver handle of stream @p idx (its +8), or -1 without the stream.
 *
 * @param idx  the stream index
 * @return the handle, or -1
 */
extern "C" int cStreamSys_STREAM_GetHandle(cStreamSys* self, int idx)
{
    void* stream = *(void**)((char*)self->mStreamInstance + idx * 4);
    if (stream != 0) {
        return *(int*)((char*)stream + 0x8);
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_GetCurrActiveSampleHandle);
#ifdef SKIP_ASM
extern "C" int cStreamInstance_GetCurrActiveSampleHandle(cStreamInstance*);

/**
 * Returns stream @p i's current request handle (cStreamInstance_GetCurrActiveSampleHandle), or -1.
 *
 * @param i  the stream index
 * @return the request handle, or -1
 */
extern "C" int cStreamSys_STREAM_GetCurrActiveSampleHandle(cStreamSys* self, int i)
{
    void* stream = ((void**)self->mStreamInstance)[i];
    if (stream != 0) {
        return cStreamInstance_GetCurrActiveSampleHandle((cStreamInstance*)stream);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_GetInstance__FPvi);
#ifdef SKIP_ASM
/**
 * Returns stream instance @p index from the table (it may be 0).
 *
 * @param index  the stream index
 * @return the stream instance
 */
int cStreamSys_STREAM_GetInstance(void* self, int index)
{
    return *(int*)((char*)*(void**)((char*)self + 0x8) + index * 4);
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_ReleaseHoldCallback);
#ifdef SKIP_ASM
extern "C" void SNDSTRM_modifyhold(int, int);

/**
 * Monitor callback: releases the hold on the request whose handle the monitor carries (+0x1C); returns 1.
 *
 * @param monitorInst  the monitor instance
 * @return 1
 */
extern "C" int cStreamSys_STREAM_ReleaseHoldCallback(cStreamSys* self, cMonitorInstance* monitorInst)
{
    SNDSTRM_modifyhold(monitorInst->mData1, 0);
    return 1;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_STREAM_TimeMult);
#ifdef SKIP_ASM
extern "C" int cStreamInstance_TimeMult(cStreamInstance*, int);

/**
 * Multiplies the timing of stream @p idx (cStreamInstance_TimeMult); -1 without the stream.
 *
 * @param idx  the stream index
 * @param multAmount  the multiplier
 * @return the driver's result, or -1
 */
extern "C" int cStreamSys_STREAM_TimeMult(cStreamSys* self, int idx, int multAmount)
{
    void* stream = *(void**)((char*)self->mStreamInstance + idx * 4);
    return stream != 0 ? cStreamInstance_TimeMult((cStreamInstance*)stream, multAmount) : -1;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cController_cController);
#ifdef SKIP_ASM
extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" void* cCrowdSys_cCrowdSys(void* self, int embedded, int a2, int a3, int heap);
extern "C" void* func_002B28C0(void* self, int embedded, int a2, int a3, int heap);
extern "C" void* cSpeechSys_cSpeechSys(void* self, int embedded, int a2, int a3, int heap);
extern void* D_004838F8[];
extern void* D_004837F8[];
extern void* cSpeechSys_vtbl[];

struct sSsVEntryB200 {
    short delta;
    short index;
    void* fn;
};
struct sSsVtblAB200 {
    sSsVEntryB200 e[4];
} __attribute__((aligned(8)));
struct sSsVtblBB200 {
    sSsVEntryB200 e[8];
} __attribute__((aligned(8)));
struct sSsVtblCB200 {
    sSsVEntryB200 e[7];
} __attribute__((aligned(8)));
extern const sSsVtblAB200 D_004837D8_B200 __asm__("D_004837D8");
extern const sSsVtblBB200 D_00483760_B200 __asm__("D_00483760");
extern const sSsVtblCB200 D_004837A0_B200 __asm__("D_004837A0");

/**
 * Constructs the sound controller: the virtual bases when complete, then the crowd, music and speech systems, and the
 * vtables.
 *
 * With @p embedded set it wires the virtual-base pointers inside the object and constructs cSoundSys (at +0x5728),
 * cBankManager (+0x5730) and cStreamSys (+0x6200). Then cCrowdSys_cCrowdSys, the music system (func_002B28C0 at +0x118)
 * and cSpeechSys_cSpeechSys (+0x5560) run, and the cController vtables D_004837D8, D_00483760 and D_004837A0 are
 * installed on the bases (temporary copies with adjusted deltas when not complete). Finally the primary vtables
 * D_004838F8, cSpeechSys_vtbl and D_004837F8 are stored and +0x5720 is cleared.
 *
 * @param embedded  nonzero to also construct the virtual bases (the in-charge flag)
 * @param maxBanks  the maximum number of banks
 * @param maxStreams  the maximum number of streams
 * @param heap  the audio mode passed to the bases
 * @return this
 */
// PORT: g++ 2.95 virtual-base construction with stack vtable this-adjust fix-ups, written out by hand.
extern "C" char* cController_cController(char* self, int embedded, int maxBanks, int maxStreams, int heap)
{
    if (embedded != 0) {
        *(char**)(self + 0x5560) = self + 0x6200;
        *(char**)(self + 0x6200) = self + 0x5730;
        *(char**)(self + 0x0) = self + 0x5730;
        *(char**)(self + 0x5908) = self + 0x5728;
        *(char**)(self + 0x118) = self + 0x6200;
        cSoundSys_cSoundSys(self + 0x5728, heap);
        cBankManager_cBankManager(*(void**)(self + 0x6200), 0, maxBanks, heap);
        cStreamSys_cStreamSys(*(sStreamSysA490**)(self + 0x5560), 0, maxBanks, maxStreams, heap);
    }
    cCrowdSys_cCrowdSys(self, 0, maxBanks, maxStreams, heap);
    func_002B28C0(self + 0x118, 0, maxBanks, maxStreams, heap);
    cSpeechSys_cSpeechSys(self + 0x5560, 0, maxBanks, maxStreams, heap);
    *(const sSsVtblAB200**)(*(char**)(**(char***)(self + 0x118) + 0x1D8) + 4) = &D_004837D8_B200;
    *(const sSsVtblBB200**)(**(char***)(self + 0x118) + 0xAB0) = &D_00483760_B200;
    *(const sSsVtblCB200**)(**(char***)(self + 0x118) + 0x1D4) = &D_004837A0_B200;
    if (embedded == 0) {
        sSsVtblAB200 soundSysVtbl = D_004837D8_B200;
        *(sSsVtblAB200**)(*(char**)(**(char***)(self + 0x118) + 0x1D8) + 4) = &soundSysVtbl;
        char* base1 = *(char**)(**(char***)(self + 0x118) + 0x1D8) - 0x5728;
        int soundSysDelta = self - base1;
        soundSysVtbl.e[1].delta = D_004837D8_B200.e[1].delta + soundSysDelta;
        sSsVtblBB200 monitorVtbl = D_00483760_B200;
        *(sSsVtblBB200**)(**(char***)(self + 0x118) + 0xAB0) = &monitorVtbl;
        char* base2 = **(char***)(self + 0x118) - 0x5730;
        int bankMgrDelta = self - base2;
        monitorVtbl.e[1].delta = D_00483760_B200.e[1].delta + bankMgrDelta;
        monitorVtbl.e[5].delta = D_00483760_B200.e[5].delta + bankMgrDelta;
        sSsVtblCB200 bankMgrVtbl = D_004837A0_B200;
        *(sSsVtblCB200**)(**(char***)(self + 0x118) + 0x1D4) = &bankMgrVtbl;
        bankMgrVtbl.e[1].delta = D_004837A0_B200.e[1].delta + bankMgrDelta;
    }
    *(void***)(self + 0x5558) = D_004838F8;
    *(void***)(self + 0x571C) = cSpeechSys_vtbl;
    *(void***)(self + 0xC) = D_004837F8;
    *(int*)(self + 0x5720) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cController__cController);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cBankManager__cBankManager(void* self, int flags);
extern "C" void cCrowdSys__cCrowdSys(void* self, int flags);
extern "C" void cStreamSys__cStreamSys(cStreamSys* self, int flags);
extern "C" void cSoundSys__cSoundSys(void* self, int flags);
extern "C" void cSpeechSys__cSpeechSys(char* self, int flags);
extern "C" void func_002B3398(char* self, int flags);
extern void* D_004838F8[];
extern void* D_004837F8[];
extern void* cSpeechSys_vtbl[];

struct sSsVEntryB478 {
    short delta;
    short index;
    void* fn;
};
struct sSsVtblAB478 {
    sSsVEntryB478 e[4];
} __attribute__((aligned(8)));
struct sSsVtblBB478 {
    sSsVEntryB478 e[8];
} __attribute__((aligned(8)));
struct sSsVtblCB478 {
    sSsVEntryB478 e[7];
} __attribute__((aligned(8)));
extern const sSsVtblAB478 D_004837D8;
extern const sSsVtblBB478 D_00483760;
extern const sSsVtblCB478 D_004837A0;

/**
 * Destroys the sound controller and its bank, stream and monitor sub-systems.
 *
 * Hand-written g++ 2.95 virtual-base destruction: installs the cController vtables at +0x5558, +0x571C and +0xC and in
 * the shared sound object (+0x118), then runs the sub-object destructors (cSpeechSys__cSpeechSys, func_002B3398, cCrowdSys__cCrowdSys,
 * cStreamSys__cStreamSys, cBankManager__cBankManager, cSoundSys__cSoundSys) with this-adjusted temporary vtables, and deletes the object when
 * bit 0 of @p flags is set.
 *
 * @param flags  g++ 2.95 in-charge flags; bit 0 deletes the object
 */
// PORT: g++ 2.95 virtual-base destruction with stack vtable this-adjust fix-ups, written out by hand.
extern "C" void cController__cController(char* self, int flags)
{
    char* mon = self + 0x118;
    char* bank = self + 0x5560;
    *(void***)(self + 0x5558) = D_004838F8;
    *(void***)(self + 0x571C) = cSpeechSys_vtbl;
    *(void***)(self + 0xC) = D_004837F8;
    *(const sSsVtblAB478**)(*(char**)(**(char***)(self + 0x118) + 0x1D8) + 4) = &D_004837D8;
    *(const sSsVtblBB478**)(**(char***)(self + 0x118) + 0xAB0) = &D_00483760;
    *(const sSsVtblCB478**)(**(char***)(self + 0x118) + 0x1D4) = &D_004837A0;
    if (flags == 0) {
        sSsVtblAB478 soundSysVtbl = D_004837D8;
        *(sSsVtblAB478**)(*(char**)(**(char***)(self + 0x118) + 0x1D8) + 4) = &soundSysVtbl;
        char* base1 = *(char**)(**(char***)(self + 0x118) + 0x1D8) - 0x5728;
        int soundSysAdjust = self - base1;
        soundSysVtbl.e[1].delta = D_004837D8.e[1].delta + soundSysAdjust;
        sSsVtblBB478 bankMonitorVtbl = D_00483760;
        *(sSsVtblBB478**)(**(char***)(self + 0x118) + 0xAB0) = &bankMonitorVtbl;
        char* base2 = **(char***)(self + 0x118) - 0x5730;
        int bankMgrAdjust = self - base2;
        bankMonitorVtbl.e[1].delta = D_00483760.e[1].delta + bankMgrAdjust;
        bankMonitorVtbl.e[5].delta = D_00483760.e[5].delta + bankMgrAdjust;
        sSsVtblCB478 bankMgrVtbl = D_004837A0;
        *(sSsVtblCB478**)(**(char***)(self + 0x118) + 0x1D4) = &bankMgrVtbl;
        bankMgrVtbl.e[1].delta = D_004837A0.e[1].delta + bankMgrAdjust;
    }
    cSpeechSys__cSpeechSys(bank, 0);
    func_002B3398(mon, 0);
    cCrowdSys__cCrowdSys(self, 0);
    if (flags & 2) {
        cStreamSys__cStreamSys((cStreamSys*)(*(void**)(self + 0x118)), 0);
        cBankManager__cBankManager(**(void***)(self + 0x118), 0);
        cSoundSys__cSoundSys(*(void**)(**(char***)(self + 0x118) + 0x1D8), 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Update);
#ifdef SKIP_ASM
extern "C" int cBankMonitorInstance_IsFree(void* self);
extern "C" void cBankMonitorInstance_Update3D(void* voice, cBankMonitor* bm, float v);

class cStrmCbOwner {
public:
    virtual void dummy();
};

struct sStrmVoiceCb {
    char pad[0xA8];
    void (cStrmCbOwner::*cb)(void* voice, float v);
};

/**
 * Updates one bank-monitor instance for a frame.
 *
 * Only when the instance is in use (cBankMonitorInstance_IsFree returns 0) and @p time is positive: calls the
 * member-function callback at +0xA8 on the controller (if set), then cBankMonitorInstance_Update3D with the
 * controller's bank monitor (its sound object's +0x1D8, or 0 without a controller). The parameter named self is the
 * controller (On Tour cController* Monitor), not this.
 *
 * @param voice  the bank-monitor instance (this)
 * @param time   elapsed time (On Tour Time)
 */
extern "C" void cBankMonitorInstance_Update(void* voice, void* self, float time)
{
    if (cBankMonitorInstance_IsFree(voice) == 0 && time > 0.0f) {
        sStrmVoiceCb* sv = (sStrmVoiceCb*)voice;
        if (sv->cb) {
            (((cStrmCbOwner*)self)->*(sv->cb))(voice, time);
        }
        char* bm = 0;
        if (self) {
            char* snd = **(char***)((char*)self + 0x118);
            if (snd) {
                bm = snd;
                bm += 0x1D8;
            }
        }
        cBankMonitorInstance_Update3D(voice, (cBankMonitor*)bm, time);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cController_BANKMONITOR_Update);
#ifdef SKIP_ASM
extern "C" void cBankMonitorInstance_Update(void* voice, void* self, float v);

/**
 * Updates all 64 bank-monitor instances.
 *
 * Uses a time of 0 when the bank monitor's pause flag (+0xAC0 of the sound object) is set, then calls cBankMonitorInstance_Update for
 * each instance in the array at +0xAC4.
 *
 * @param time  elapsed time (On Tour Time)
 */
extern "C" void cController_BANKMONITOR_Update(void* self, float time)
{
    if (*(int*)(**(char***)((char*)self + 0x118) + 0xAC0) != 0) {
        time = 0.0f;
    }
    for (int i = 0; i < 64; i++) {
        cBankMonitorInstance_Update(*(char**)(**(char***)((char*)self + 0x118) + 0xAC4) + i * 0xC0, self, time);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cMonitorInstance_Update);
#ifdef SKIP_ASM
extern "C" void cMonitorInstance_Reset(void* e);

class cStrmTickOwner {
public:
    virtual void dummy();
};

struct sStrmTick {
    int active;
    int period;
    int count;
    int (cStrmTickOwner::*cb)(sStrmTick* e);
};

/**
 * Updates one monitor instance by 10 ms: when active and its count drops below 5, calls its callback on the controller
 * (pointer to member); a nonzero result resets the instance (cMonitorInstance_Reset), else the count is re-armed by the
 * period.
 *
 * @param entry  the monitor instance
 */
extern "C" void cMonitorInstance_Update(void* entry, void* self)
{
    sStrmTick* tick = (sStrmTick*)entry;
    if (tick->active) {
        if ((tick->count -= 10) < 5) {
            if ((((cStrmTickOwner*)self)->*(tick->cb))(tick)) {
                cMonitorInstance_Reset(tick);
            } else {
                tick->count += tick->period;
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cController_MONITOR_Update);
#ifdef SKIP_ASM
extern "C" void cMonitor_MONITOR_Update(void* bm);
extern "C" void cMonitorInstance_Update(void* e, void* self);

/**
 * Updates the monitors unless paused: cMonitor_MONITOR_Update and then each of the 48 monitor instances
 * (cMonitorInstance_Update).
 *
 * It fills slot 1 of cSSXAudio's cBankMonitor vtable (D_00483640), where cBankMonitor has cMonitor_MONITOR_Update.
 */
extern "C" void cController_MONITOR_Update(void* self)
{
    char* snd = **(char***)((char*)self + 0x118);
    char* bm = snd + 0x1D8;
    if (*(int*)(snd + 0x26C) == 0) {
        cMonitor_MONITOR_Update(bm);
        for (int i = 0; i < 48; i++) {
            cMonitorInstance_Update(**(char***)((char*)self + 0x118) + 0x270 + i * 44, self);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cController_Update);
#ifdef SKIP_ASM
extern "C" void cAsyncSys_ASYNCSYS_Update(void* snd);
extern "C" void func_002A6D18(void* self);
extern "C" void cStreamSys_STREAM_Update(cStreamSys* self);
extern "C" void func_002B0C78(void* self);
extern "C" void func_002B4388(void* self);

struct sStrmVtEntF { short delta; short index; void (*fn)(void*, float); };

/**
 * Updates the sound controller each frame: async loads, crowd, bank monitor, music, speech and streams.
 *
 * Runs cAsyncSys_ASYNCSYS_Update, func_002A6D18 (crowd), the bank monitor's vtable slot 5 (BANKMONITOR_Update) with
 * @p frameTime, func_002B4388 (music), func_002B0C78 (speech) and cStreamSys_STREAM_Update (streams). cSSXAudio_Update calls it.
 *
 * @param frameTime  the frame time
 */
extern "C" void cController_Update(void* self, float frameTime)
{
    char** sys = (char**)((char*)self + 0x118);
    cAsyncSys_ASYNCSYS_Update(*(void**)*sys);
    func_002A6D18(self);
    char* bankMan = *(char**)*sys;
    char* bankMon = bankMan + 0x1D8;
    sStrmVtEntF* entry = &(*(sStrmVtEntF**)(bankMan + 0xAB0))[5];
    entry->fn(bankMon + entry->delta, frameTime);
    func_002B4388(sys);
    func_002B0C78((char*)self + 0x5560);
    cStreamSys_STREAM_Update((cStreamSys*)(*sys));
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_cBankMonitorInstance);
#ifdef SKIP_ASM
/**
 * Constructs a bank monitor instance: free (type 0), 0x8000 pitch, distance -1.0, the handles -1 / -30000.
 *
 * @return this
 */
extern "C" void* cBankMonitorInstance_cBankMonitorInstance(void* self)
{
    *(int*)((char*)self + 0xB0) = 0x8000;
    *(float*)((char*)self + 0x90) = -1.0f;
    *(int*)((char*)self + 0xB4) = -1;
    *(int*)((char*)self + 0xB8) = -30000;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(int*)((char*)self + 0x58) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance__cBankMonitorInstance);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys a bank monitor instance: nothing to release; deletes it when bit 0 of @p flags is set.
 *
 * @param flags  bit 0 set to also delete the object
 */
extern "C" void cBankMonitorInstance__cBankMonitorInstance(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_DonePlaying);
#ifdef SKIP_ASM
extern "C" int SNDover(int h);
extern "C" int SNDseqstatus(int h);
// PORT: the unit declares SNDSTRM_requeststatus(int, int); its second argument is a status buffer here.
int SNDSTRM_requeststatus_st(int h, int* st) __asm__("SNDSTRM_requeststatus");

/**
 * Returns whether the instance's sound has finished playing.
 *
 * By the type at +0: 1 (sample) asks SNDover under the sound critical section; 2 (sequence) checks SNDseqstatus
 * for status 2 or an error; 3 (stream) reads the request status (SNDSTRM_requeststatus) and reports done when it is 3 or cannot
 * be read. Any other type counts as done.
 *
 * @return nonzero when done
 */
extern "C" int cBankMonitorInstance_DonePlaying(void* self)
{
    int type = *(int*)self;
    switch (type) {
    case 1: {
        SNDSYS_entercritical();
        int result = SNDover(*(int*)((char*)self + 4)) != 0;
        SNDSYS_leavecritical();
        return result;
    }
    case 2: {
        SNDSYS_entercritical();
        int result = SNDseqstatus(*(int*)((char*)self + 4));
        SNDSYS_leavecritical();
        return result == 2 || result < 0;
    }
    case 3: {
        int st[4];
        SNDSYS_entercritical();
        if (SNDSTRM_requeststatus_st(*(int*)((char*)self + 4), st) >= 0) {
            SNDSYS_leavecritical();
            return st[0] == 3;
        }
        SNDSYS_leavecritical();
        return 1;
    }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Reset);
#ifdef SKIP_ASM
extern "C" void cBankMonitorInstance_StopSound(void*);

/**
 * Stops an instance's sound and returns it to the free state.
 *
 * Calls cBankMonitorInstance_StopSound, then sets the cached pitch (+0xB0) to 0x8000, the cached volume (+0xB4) to -1, the cached
 * position (+0xB8) to 0xFFFF and clears the type (+0), the stopping flag (+0x54) and +0x58.
 */
extern "C" void cBankMonitorInstance_Reset(void* self)
{
    cBankMonitorInstance_StopSound(self);
    *(int*)((char*)self + 0xB0) = 0x8000;
    *(int*)((char*)self + 0xB4) = -1;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0xB8) = 0xFFFF;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_IsFree);
#ifdef SKIP_ASM
extern "C" int cBankMonitorInstance_DonePlaying(void* self);
extern "C" void cBankMonitorInstance_Reset(void* self);

/**
 * Returns whether an instance is free, releasing it if its sound has finished.
 *
 * An instance with no type (+0) is free. One without the auto-release flag (+0x9C) is never free. Otherwise it is free
 * once cBankMonitorInstance_DonePlaying says the sound is done, after cBankMonitorInstance_Reset resets it.
 *
 * @return nonzero when free
 */
extern "C" int cBankMonitorInstance_IsFree(void* self)
{
    if (*(int*)self == 0) {
        return 1;
    }
    if (*(int*)((char*)self + 0x9C) == 0) {
        return 0;
    }
    if (cBankMonitorInstance_DonePlaying(self) == 0) {
        return 0;
    }
    cBankMonitorInstance_Reset(self);
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_DistSq);
#ifdef SKIP_ASM
/**
 * Returns the squared distance between two positions.
 *
 * @param posA  the first position
 * @param posB  the second position
 * @return the squared distance
 */
extern "C" float cBankMonitorInstance_DistSq(void* self, float* posA, float* posB)
{
    float dx = posA[0] - posB[0];
    float dy = posA[1] - posB[1];
    float dz = posA[2] - posB[2];
    return dx * dx + dy * dy + dz * dz;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", ExpScale);
#ifdef SKIP_ASM
// PORT: the unit declares ExpScale as void*(int); the body takes (int, float, float).
int ExpScale_impl(int vol, float dist, float range) __asm__("ExpScale");

/**
 * Attenuates a volume by distance with a squared fall-off.
 *
 * The distance is reduced by 0.5 (not below 0); within @p range the result is vol * ((range - dist) / range)^2, beyond
 * it (or for a non-positive range) 0.
 *
 * @param vol  the full volume (On Tour Vol)
 * @param dist  the distance (On Tour Dist)
 * @param range  the distance at which the sound vanishes (On Tour Vanish)
 * @return the attenuated volume
 */
int ExpScale_impl(int vol, float dist, float range)
{
    dist = dist - 0.5f;
    if (dist < 0.0f) {
        dist = 0.0f;
    }
    int result = 0;
    if (dist < range && 0.0f < range) {
        float falloff = (range - dist) / range;
        result = (int)(falloff * falloff * (float)vol);
    }
    return result;
}
#endif

extern "C" void* ExpScale(int);

//99.29%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Dist2Vol__FPvi);
#ifdef SKIP_ASM
/**
 * Converts a distance to a volume for this instance (forwards to ExpScale).
 *
 * The C declaration shows only the volume; the distance and vanish range pass through in $f12/$f13.
 *
 * @param vol  the full volume (On Tour Vol)
 * @return the attenuated volume
 */
void* cBankMonitorInstance_Dist2Vol(void* self, int vol)
{
    return ExpScale(vol);
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Doppler);
#ifdef SKIP_ASM
/**
 * Returns the Doppler pitch factor for a change in distance over a time step.
 *
 * The speed @p delta / @p time is clamped to +-894.08 and mapped to (speed + 8000) / 8000. The object pointer is not used, so
 * the C definition omits it.
 *
 * @param delta  change in distance (On Tour Delta)
 * @param time  time step (On Tour Time)
 * @return the pitch factor (1.0 for no motion)
 */
extern "C" float cBankMonitorInstance_Doppler(float delta, float time)
{
    float speed = delta / time;
    if (speed < -894.0799560546875f) {
        speed = -894.0799560546875f;
    } else if (894.0799560546875f < speed) {
        speed = 894.0799560546875f;
    }
    return (speed + 8000.0f) * 0.0001250000059371814f;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_SetBank__FPvii);
#ifdef SKIP_ASM
/**
 * Records a monitor instance's bank and patch (+0xC, +0x10).
 *
 * @param bank  the bank
 * @param patch  the patch
 */
void cBankMonitorInstance_SetBank(void* self, int bank, int patch)
{
    *(int*)((char*)self + 0xc) = bank;
    *(int*)((char*)self + 0x10) = patch;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_SetStream);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);

/**
 * Records a monitor instance's stream: the stream instance (+0xC), the file name (+0x10) and the offset (+0x50).
 *
 * @param streamInst  the stream instance
 * @param name  the file name
 * @param fileOffset  the offset in the file
 */
extern "C" void cBankMonitorInstance_SetStream(void* self, int streamInst, const char* name, int fileOffset)
{
    *(int*)((char*)self + 0xC) = streamInst;
    strcpy((char*)self + 0x10, name);
    *(int*)((char*)self + 0x50) = fileOffset;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_SetMIDI__FPvii);
#ifdef SKIP_ASM
/**
 * Records a monitor instance's MIDI sequence and bank (+0xC, +0x10).
 *
 * @param midiData  the sequence data
 * @param bank  the bank
 */
void cBankMonitorInstance_SetMIDI(void* self, int midiData, int bank)
{
    *(int*)((char*)self + 0xc) = midiData;
    *(int*)((char*)self + 0x10) = bank;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_SetLastDistance);
#ifdef SKIP_ASM
class cBankMonitorInstance_SetLastDistance_cObj {
public:
    char pad[0x8D8];
    // vptr at 0x8D8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual float v06(void* a, void* out);
};

/**
 * Records a monitor instance's current distance: the square root of the monitor's vtable slot 6 (Calc3DPos) for it,
 * stored at +0x5C.
 *
 * @param obj  the bank monitor
 */
extern "C" void cBankMonitorInstance_SetLastDistance(void* self, cBankMonitorInstance_SetLastDistance_cObj* obj)
{
    char buf[0x20];
    float distance;
    float distSq = obj->v06(self, buf);
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(distance) : "f"(distSq));
    *(float*)((char*)self + 0x5C) = distance;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_PosUpdate);
#ifdef SKIP_ASM
extern "C" void cStreamInstance_Set3DPosition(cStreamInstance* s, int pos, int a2);
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" void SND3dpos(int h, int pos, int a2);
extern "C" void SNDMIDI_azimuth(int h, int pos);

/**
 * Updates the 3D position (azimuth, elevation) of an instance's sound.
 *
 * Does nothing when the azimuth differs from the cached one (+0xB8) by less than 0x200; otherwise caches it and, by
 * type, calls SND3dpos (sample), SNDMIDI_azimuth (sequence) or cStreamInstance_Set3DPosition (stream at +0xC).
 *
 * @param pos  the azimuth (On Tour Azimuth)
 * @param elevation  the elevation (On Tour Elevation)
 */
extern "C" void cBankMonitorInstance_PosUpdate(void* self, int pos, int elevation)
{
    int azimuthDiff = pos - *(int*)((char*)self + 0xB8);
    if (azimuthDiff >= -0x1FF && azimuthDiff <= 0x1FF) {
        return;
    }
    *(int*)((char*)self + 0xB8) = pos;
    switch (*(int*)self) {
    case 1:
        SNDSYS_entercritical();
        SND3dpos(*(int*)((char*)self + 0x4), pos, elevation);
        SNDSYS_leavecritical();
        break;
    case 2:
        SNDSYS_entercritical();
        SNDMIDI_azimuth(*(int*)((char*)self + 0x4), pos);
        SNDSYS_leavecritical();
        break;
    case 3:
        cStreamInstance_Set3DPosition((cStreamInstance*)(*(void**)((char*)self + 0xC)), pos, elevation);
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_PitchUpdate);
#ifdef SKIP_ASM
extern "C" void cStreamInstance_SetPitch(cStreamInstance* s, int vol);
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" void SNDpitchmult(int h, int vol);
extern "C" void SNDMIDI_pitchmult(int h, int vol);

/**
 * Sets the pitch multiplier of an instance's sound.
 *
 * The value is clamped to 0..0x3FFF (0x1000 = normal pitch) and ignored when within 0xFF of the cached pitch at +0xB0;
 * otherwise it is cached and applied by type: SNDpitchmult (sample), SNDMIDI_pitchmult (sequence) or cStreamInstance_SetPitch (stream
 * at +0xC). Note the parameter is named vol but is a pitch.
 *
 * @param vol  the pitch multiplier, 4.12 fixed point (On Tour Pitch)
 */
extern "C" void cBankMonitorInstance_PitchUpdate(void* self, int vol)
{
    if (vol > 0x3FFF) {
        vol = 0x3FFF;
    }
    if (vol < 0) {
        vol = 0;
    }
    int pitchDiff = vol - *(int*)((char*)self + 0xB0);
    if (pitchDiff >= -0xFF && pitchDiff <= 0xFF) {
        return;
    }
    *(int*)((char*)self + 0xB0) = vol;
    switch (*(int*)self) {
    case 1:
        SNDSYS_entercritical();
        SNDpitchmult(*(int*)((char*)self + 0x4), vol);
        SNDSYS_leavecritical();
        break;
    case 2:
        SNDSYS_entercritical();
        SNDMIDI_pitchmult(*(int*)((char*)self + 0x4), vol);
        SNDSYS_leavecritical();
        break;
    case 3:
        cStreamInstance_SetPitch((cStreamInstance*)(*(void**)((char*)self + 0xC)), vol);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_VolUpdate);
#ifdef SKIP_ASM
void cStreamInstance_SetBaseVolume(void* self, int val);
void* cStreamInstance_Purge(void* self);
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" void SNDvol(int h, int a);
extern "C" void SNDstop(int h);
extern "C" void SNDseqstop(int h);
extern "C" void SNDseqvol(int h, int a);

/**
 * Sets the volume of an instance's sound, or stops it for -1.
 *
 * The volume is scaled by the factor at +0x90 when that is non-negative, clamped to 0..127 and ignored when within 4 of
 * the cached volume at +0xB4. A requested -1 stops the sound instead: SNDstop (sample), SNDseqstop (sequence)
 * or cStreamInstance_Purge__FPv (stream); other values go to SNDvol, SNDseqvol or cStreamInstance_SetBaseVolume__FPvi.
 *
 * @param vol  the volume 0..127, or -1 to stop (On Tour Vol)
 */
extern "C" void cBankMonitorInstance_VolUpdate(void* self, int vol)
{
    int orig = vol;
    switch (*(int*)self) {
    case 1: {
        if (*(float*)((char*)self + 0x90) >= 0.0f)
            vol = (int)(*(float*)((char*)self + 0x90) * (float)vol);
        if (vol > 127)
            vol = 127;
        int none = -1;
        if (vol <= none)
            vol = 0;
        if (*(int*)((char*)self + 0xB4) >= 0) {
            int volDiff = vol - *(int*)((char*)self + 0xB4);
            if (volDiff >= -4 && volDiff <= 4)
                return;
        }
        *(int*)((char*)self + 0xB4) = vol;
        SNDSYS_entercritical();
        if (orig == none)
            SNDstop(*(int*)((char*)self + 0x4));
        else
            SNDvol(*(int*)((char*)self + 0x4), vol);
        SNDSYS_leavecritical();
        break;
    }
    case 2: {
        if (*(float*)((char*)self + 0x90) >= 0.0f)
            vol = (int)(*(float*)((char*)self + 0x90) * (float)vol);
        if (vol > 127)
            vol = 127;
        int none = -1;
        if (vol <= none)
            vol = 0;
        if (*(int*)((char*)self + 0xB4) >= 0) {
            int volDiff = vol - *(int*)((char*)self + 0xB4);
            if (volDiff >= -4 && volDiff <= 4)
                return;
        }
        *(int*)((char*)self + 0xB4) = vol;
        SNDSYS_entercritical();
        if (orig == none)
            SNDseqstop(*(int*)((char*)self + 0x4));
        else
            SNDseqvol(*(int*)((char*)self + 0x4), vol);
        SNDSYS_leavecritical();
        break;
    }
    case 3: {
        if (*(float*)((char*)self + 0x90) >= 0.0f)
            vol = (int)(*(float*)((char*)self + 0x90) * (float)vol);
        if (vol > 127)
            vol = 127;
        int none = -1;
        if (vol <= none)
            vol = 0;
        if (*(int*)((char*)self + 0xB4) >= 0) {
            int volDiff = vol - *(int*)((char*)self + 0xB4);
            if (volDiff >= -4 && volDiff <= 4)
                return;
        }
        *(int*)((char*)self + 0xB4) = vol;
        if (orig == none)
            cStreamInstance_Purge(*(void**)((char*)self + 0xC));
        else
            cStreamInstance_SetBaseVolume(*(void**)((char*)self + 0xC), vol);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_StopSound);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" void SNDautovol(int h, int a, int b);
extern "C" void SNDseqautovol(int h, int a, int b);

/**
 * Starts fading an instance's sound out.
 *
 * Does nothing when already stopping (+0x54). Otherwise clears +0x8, sets +0x54 and starts an automatic volume ramp
 * (0x19, -1) on the sample (SNDautovol) or sequence (SNDseqautovol).
 */
extern "C" void cBankMonitorInstance_StopSound(void* self)
{
    if (*(int*)((char*)self + 0x54) != 0) {
        return;
    }
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x54) = 1;
    switch (*(int*)self) {
    case 1:
        SNDSYS_entercritical();
        SNDautovol(*(int*)((char*)self + 0x4), 25, -1);
        SNDSYS_leavecritical();
        break;
    case 2:
        SNDSYS_entercritical();
        SNDseqautovol(*(int*)((char*)self + 0x4), 25, -1);
        SNDSYS_leavecritical();
        break;
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_RestartSound);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" int SNDBANK_play(int a, int b, void* desc);
extern "C" int SNDMIDI_play(void* desc, int a, int b, int c);

struct sSndDesc_C220 {
    char c[0x18];
};

struct sSndEnv_C220 {
    int f0;
    int f4;
    int f8;
    int fC;
};

struct sSndPlayOpts_C220 {
    char* vbase;                // 0x00
    sSndDesc_C220 cur;          // 0x04
    volatile int depth;         // 0x1C
    sSndDesc_C220* descs;       // 0x20
    int f24[4];                 // 0x24
    int* f34;                   // 0x34
    int* f38;                   // 0x38
    int* f3C;                   // 0x3C
    char f40[4][8];             // 0x40
    int* f60;                   // 0x60
    int* f64;                   // 0x64
    int* f68;                   // 0x68
    int* f6C;                   // 0x6C
    sSndEnv_C220* env;          // 0x70
    int* f74;                   // 0x74
    int* f78;                   // 0x78
    int* f7C;                   // 0x7C
    int* f80;                   // 0x80
    int* f84;                   // 0x84
};

#define C220_MIN(a, b) ((a) < (b) ? (a) : (b))
#define C220_MAX(a, b) ((a) < (b) ? (b) : (a))

// PORT: the unit declares cBankMonitorInstance_RestartSound(void*) later; the real function also takes the play options
extern "C" void cBankMonitorInstance_RestartSound_impl(void* self, sSndPlayOpts_C220* opts) __asm__("cBankMonitorInstance_RestartSound");

/**
 * Restarts an instance's bank or MIDI sound with fresh play options.
 *
 * Instance fields by On Tour's names (no header here): sets mActive (+8) to 1 and mFading (+0x54) to 0. For a bank
 * sound (type 1) or MIDI sound (type 2) it pushes a play-option level on @p opts (mInst + 1): copies mDefault into
 * mCurrent[mInst] and resets that level's options (no position, not static, no callback, pointer, viewport, vanish and
 * radius 0, mDelay {100, 0, 90, 50}, external volume and two-player options off, mMapToMaxVol 127, mCloseOnEnd 1), sets
 * mVolControl to the instance's mVolControl (+0x60), mBaseVol to 0 and the level's volume to 0. Then, inside
 * SNDSYS_entercritical / SNDSYS_leavecritical, it starts the sound from the restart info and stores the handle
 * (mHandle, +4): SNDBANK_play(+0xC, +0x10, options) or SNDMIDI_play(options, +0xC, +0x10, -1). Finally pops the level.
 * Streams (type 3) are left alone. cBankMonitorInstance_CheckActive calls it when a silent sound comes back in range.
 *
 * @param opts  the cBankMonitor whose play-option stack (mDefault, mInst, mCurrent, ...) is used
 */
extern "C" void cBankMonitorInstance_RestartSound_impl(void* self, sSndPlayOpts_C220* opts)
{
    *(int*)((char*)self + 0x8) = 1;
    *(int*)((char*)self + 0x54) = 0;
    switch (*(int*)self) {
    case 1:
        opts->depth++;
        opts->descs[opts->depth] = opts->cur;
        opts->f24[opts->depth] = 0;
        opts->f34[opts->depth] = 0;
        opts->f38[opts->depth] = opts->descs[opts->depth].c[0];
        opts->f3C[opts->depth] = 0;
        func_00416210((char*)opts + opts->depth * 8 + 0x40, 0, 8);
        opts->f60[opts->depth] = 0;
        opts->f64[opts->depth] = 0;
        opts->f68[opts->depth] = 0;
        opts->f6C[opts->depth] = 0;
        opts->env[opts->depth].f4 = 0;
        opts->env[opts->depth].f0 = 100;
        opts->env[opts->depth].f8 = 90;
        opts->env[opts->depth].fC = 50;
        opts->f74[opts->depth] = 0;
        opts->f80[opts->depth] = 0;
        opts->f84[opts->depth] = 0;
        opts->f78[opts->depth] = 127;
        opts->f7C[opts->depth] = 1;
        opts->f3C[opts->depth] = *(int*)((char*)self + 0x60);
        opts->f38[opts->depth] = 0;
        opts->descs[opts->depth].c[0] = C220_MAX(C220_MIN(opts->f38[opts->depth], 127), 0);
        SNDSYS_entercritical();
        *(int*)((char*)self + 0x4) = SNDBANK_play(*(int*)((char*)self + 0xC), *(int*)((char*)self + 0x10), &opts->descs[opts->depth]);
        SNDSYS_leavecritical();
        opts->depth--;
        break;
    case 2:
        opts->depth++;
        opts->descs[opts->depth] = opts->cur;
        opts->f24[opts->depth] = 0;
        opts->f34[opts->depth] = 0;
        opts->f38[opts->depth] = opts->descs[opts->depth].c[0];
        opts->f3C[opts->depth] = 0;
        func_00416210((char*)opts + opts->depth * 8 + 0x40, 0, 8);
        opts->f60[opts->depth] = 0;
        opts->f64[opts->depth] = 0;
        opts->f68[opts->depth] = 0;
        opts->f6C[opts->depth] = 0;
        opts->env[opts->depth].f4 = 0;
        opts->env[opts->depth].f0 = 100;
        opts->env[opts->depth].f8 = 90;
        opts->env[opts->depth].fC = 50;
        opts->f74[opts->depth] = 0;
        opts->f80[opts->depth] = 0;
        opts->f84[opts->depth] = 0;
        opts->f78[opts->depth] = 127;
        opts->f7C[opts->depth] = 1;
        opts->f3C[opts->depth] = *(int*)((char*)self + 0x60);
        opts->f38[opts->depth] = 0;
        opts->descs[opts->depth].c[0] = C220_MAX(C220_MIN(opts->f38[opts->depth], 127), 0);
        SNDSYS_entercritical();
        *(int*)((char*)self + 0x4) = SNDMIDI_play(&opts->descs[opts->depth], *(int*)((char*)self + 0xC), *(int*)((char*)self + 0x10), -1);
        SNDSYS_leavecritical();
        opts->depth--;
        break;
    case 3:
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_CheckActive);
#ifdef SKIP_ASM
extern "C" void cBankMonitorInstance_StopSound(void*);
extern "C" void cBankMonitorInstance_RestartSound(void*);

/**
 * Starts or stops an instance's sound as it crosses its audible distance.
 *
 * With no limit (+0x8C == 0) the sound is always active. A playing sound (+0x8 set) farther than the limit is stopped
 * (cBankMonitorInstance_StopSound, returns 0); a silent one within it is restarted (cBankMonitorInstance_RestartSound, returns 1).
 *
 * @param radius  the current distance (On Tour Radius)
 * @return nonzero while the sound should play
 */
extern "C" int cBankMonitorInstance_CheckActive(void* self, float radius)
{
    float th = *(float*)((char*)self + 0x8C);
    if (th == 0.0f) {
        return 1;
    }
    if (*(int*)((char*)self + 0x8) != 0) {
        if (th < radius) {
            cBankMonitorInstance_StopSound(self);
            return 0;
        }
        return 1;
    }
    if (radius < th) {
        cBankMonitorInstance_RestartSound(self);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Update3D);
#ifdef SKIP_ASM
class cStreamBM_C868 {
public:
    char pad[0x8D8];
    // vptr at 0x8D8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual float v06(void* a, void* out);
};
struct sPos_C868 {
    int a;
    int b;
    unsigned short pos;     // 0x8
    short pos2;             // 0xA
    int c[5];
};
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
// PORT: the unit declares SNDSTRM_requeststatus(int, int); its second argument is a status buffer here.
int SNDSTRM_requeststatus_st(int h, int* st) __asm__("SNDSTRM_requeststatus");
extern "C" int cBankMonitorInstance_DonePlaying(void* self);
extern "C" void cBankMonitorInstance_PosUpdate(void* self, int pos, int a2);
extern "C" void cBankMonitorInstance_PitchUpdate(void* self, int a1);
extern "C" void cBankMonitorInstance_VolUpdate(void* self, int a1);
// PORT: these callees are declared with fewer parameters elsewhere in the unit; the caller passes the extra registers.
int cBankMonitorInstance_Dist2Vol_4(void* self, int vol, float dist, float range) __asm__("cBankMonitorInstance_Dist2Vol__FPvi");
float cBankMonitorInstance_Doppler_3(void* self, float a, float b) __asm__("cBankMonitorInstance_Doppler");
int cBankMonitorInstance_CheckActive_3(void* self, void* bm, float t) __asm__("cBankMonitorInstance_CheckActive");

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_C868(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Updates the 3D audio of an instance: Doppler pitch, position and distance volume.
 *
 * A free instance (type +0x0 is 0) is skipped, and so is a stream (type 3) when SNDSTRM_requeststatus returns 0 or less and the
 * status word is not 2. For a positioned instance (+0x68) while the monitor has 3D on (+0x8E4), the distance is the
 * square root of the monitor's virtual slot 6. When the instance is held active (+0x94) or
 * cBankMonitorInstance_CheckActive says so, the Doppler factor (cBankMonitorInstance_Doppler, from the previous
 * distance at +0x5C, when +0x8E0 enables it), azimuth and elevation (cBankMonitorInstance_PosUpdate), pitch
 * (cBankMonitorInstance_PitchUpdate, factor * 4096) and volume (the fixed +0x64 when held, else
 * cBankMonitorInstance_Dist2Vol__FPvi over the range at +0x88, default 30, rescaled by 127 / the maximum at +0x98) are
 * applied; an inactive one gets volume 0. The distance is kept at +0x5C. Otherwise the fixed volume at +0x64 is used.
 *
 * Unless the instance is stopping (+0x54), the volume is multiplied by the optional factor at +0x60 and set with
 * cBankMonitorInstance_VolUpdate; a stopping instance whose sound is done (cBankMonitorInstance_DonePlaying) is cleared
 * instead (+0x8 and +0x54 zeroed, handle +0x4 set to -1).
 *
 * @param voice  the bank-monitor instance (this)
 * @param bm     the bank monitor (On Tour Monitor)
 * @param time   elapsed time (On Tour Time); reused for the Doppler factor
 */
extern "C" void cBankMonitorInstance_Update3D(void* voice, cBankMonitor* bm, float time)
{
    char* inst = (char*)voice;
    if (*(int*)inst == 0) {
        return;
    }
    int st[4];
    sPos_C868 pos;
    int vol = 0;
    if (*(int*)inst == 3) {
        SNDSYS_entercritical();
        int result = SNDSTRM_requeststatus_st(*(int*)(inst + 0x4), st);
        SNDSYS_leavecritical();
        if (result <= 0 && st[0] != 2) {
            return;
        }
    }
    if (bm->mPositionalEnable != 0 && *(int*)(inst + 0x68) != 0) {
        float dist = Sqrt_C868(((cStreamBM_C868*)bm)->v06(voice, &pos));
        if (*(int*)(inst + 0x94) != 0 || cBankMonitorInstance_CheckActive_3(voice, bm, dist) != 0) {
            float prevDist = *(float*)(inst + 0x5C);
            if (prevDist != -1.0f && bm->mDopplerEnable != 0) {
                time = cBankMonitorInstance_Doppler_3(voice, prevDist - dist, time);
            } else {
                time = 1.0f;
            }
            cBankMonitorInstance_PosUpdate(voice, pos.pos, pos.pos2);
            cBankMonitorInstance_PitchUpdate(voice, (int)(time * 4096.0f));
            if (*(int*)(inst + 0x94) != 0) {
                vol = *(int*)(inst + 0x64);
            } else {
                float range = *(float*)(inst + 0x88);
                if (range == 0.0f) {
                    vol = cBankMonitorInstance_Dist2Vol_4(voice, *(int*)(inst + 0x64), dist, 30.0f);
                } else {
                    vol = cBankMonitorInstance_Dist2Vol_4(voice, *(int*)(inst + 0x64), dist, range);
                }
                int maxVol = *(int*)(inst + 0x98);
                if (maxVol != 0x7F) {
                    vol = vol * 0x7F / maxVol;
                    if (vol > 0x7F) {
                        vol = 0x7F;
                    }
                    if (vol < 0) {
                        vol = 0;
                    }
                }
            }
        }
        if (vol > 0x7F) {
            vol = 0x7F;
        }
        if (vol < 0) {
            vol = 0;
        }
        *(float*)(inst + 0x5C) = dist;
    } else {
        vol = *(int*)(inst + 0x64);
    }
    if (*(int*)(inst + 0x54) == 0) {
        float* volControl = *(float**)(inst + 0x60);
        if (volControl != 0) {
            vol = (int)((float)vol * *volControl);
        }
        cBankMonitorInstance_VolUpdate(voice, vol);
        return;
    }
    if (cBankMonitorInstance_DonePlaying(voice) != 0) {
        *(int*)(inst + 0x8) = 0;
        *(int*)(inst + 0x54) = 0;
        *(int*)(inst + 0x4) = -1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Pause);
#ifdef SKIP_ASM
extern "C" int cBankMonitorInstance_IsFree(void* self);
extern "C" void cBankMonitorInstance_PitchUpdate(void* self, int a1);
extern "C" void cBankMonitorInstance_VolUpdate(void* self, int a1);
extern "C" void SNDseqpause(int h);

/**
 * Pauses an instance's sound.
 *
 * For a non-free instance that is not stopping: sets pitch and volume to 0 and, for a sequence (type 2), pauses it with
 * SNDseqpause.
 */
extern "C" void cBankMonitorInstance_Pause(void* self)
{
    if (cBankMonitorInstance_IsFree(self) == 0 && *(int*)((char*)self + 0x54) == 0) {
        cBankMonitorInstance_PitchUpdate(self, 0);
        cBankMonitorInstance_VolUpdate(self, 0);
        if (*(int*)((char*)self + 0x0) == 2) {
            SNDseqpause(*(int*)((char*)self + 0x4));
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Resume);
#ifdef SKIP_ASM
extern "C" int cBankMonitorInstance_IsFree(void* self);
extern "C" void cBankMonitorInstance_PitchUpdate(void* self, int a1);
extern "C" void SNDseqresume(int h);

/**
 * Resumes an instance's sound after a pause.
 *
 * For a non-free instance (cBankMonitorInstance_IsFree) it restores normal pitch (cBankMonitorInstance_PitchUpdate
 * with 0x1000) and, for a MIDI sequence (type 2), resumes it with SNDseqresume on its handle (+4). The volume that
 * cBankMonitorInstance_Pause set to 0 is not restored here. cBankMonitor_BANKMONITOR_Resume calls it for all 64
 * instances.
 */
extern "C" void cBankMonitorInstance_Resume(void* self)
{
    if (cBankMonitorInstance_IsFree(self) == 0) {
        cBankMonitorInstance_PitchUpdate(self, 0x1000);
        if (*(int*)self == 2) {
            SNDseqresume(*(int*)((char*)self + 0x4));
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Fade);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" void SNDautovol(int h, int a, int b);
extern "C" void SNDvol(int h, int a);
extern "C" void SNDseqvol(int h, int a);
extern "C" void SNDseqautovol(int h, int a, int b);
void* cStreamInstance_Purge(void* self);
void cStreamInstance_SetBaseVolume(void* self, int val);

/**
 * Fades an instance's sound to a volume level over a duration, or out and stopped for level -1.
 *
 * When @p resetMonitor is set the instance is flagged for release (+0x58, +0x9C). A stopping instance is left alone. Level -1
 * marks it stopping; other levels are clamped to 0..127. Samples and sequences ramp over duration * 100 ticks
 * (SNDautovol, SNDseqautovol) or jump at once when the duration is 0 (SNDvol, SNDseqvol); streams are
 * purged when stopping or get the level as base volume.
 *
 * @param n  the target volume 0..127, or -1 to stop (On Tour Level)
 * @param resetMonitor  nonzero to release the instance afterwards (On Tour ResetMonitor)
 * @param duration  the fade time in seconds (On Tour Duration)
 */
extern "C" void cBankMonitorInstance_Fade(void* self, int n, int resetMonitor, float duration)
{
    if (resetMonitor) {
        *(int*)((char*)self + 0x58) = 1;
        *(int*)((char*)self + 0x9C) = 1;
    }
    if (*(int*)((char*)self + 0x54) != 0) {
        return;
    }
    if (n == -1) {
        *(int*)((char*)self + 0x54) = 1;
    } else {
        if (n > 127) {
            n = 127;
        }
        if (n < 0) {
            n = 0;
        }
    }
    switch (*(int*)self) {
    case 1:
        SNDSYS_entercritical();
        if (duration != 0.0f) {
            SNDautovol(*(int*)((char*)self + 0x4), (int)(duration * 100.0f), n);
        } else {
            SNDvol(*(int*)((char*)self + 0x4), n);
        }
        SNDSYS_leavecritical();
        break;
    case 2:
        SNDSYS_entercritical();
        if (duration != 0.0f) {
            SNDseqautovol(*(int*)((char*)self + 0x4), (int)(duration * 100.0f), n);
        } else {
            SNDseqvol(*(int*)((char*)self + 0x4), n);
        }
        SNDSYS_leavecritical();
        break;
    case 3:
        if (*(int*)((char*)self + 0x54) != 0) {
            cStreamInstance_Purge(*(void**)((char*)self + 0xC));
        } else {
            cStreamInstance_SetBaseVolume(*(void**)((char*)self + 0xC), n);
        }
        break;
    }
}
#endif

