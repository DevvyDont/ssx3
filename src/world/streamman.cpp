#include "common.h"
#include "world/streamman.h"

//100%
INCLUDE_ASM("world/streamman", cStreamMan_cStreamMan);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int func_003E06B0(int a, int b, int c);
extern "C" void* func_003E06D8(int a, int b, int c, void* buf, int size);
extern "C" void cStreamMan_reset(cStreamMan* self);
extern char D_00494D90[];
extern char D_00494DA8[];

static inline int cStreamMan_min(int a, int b)
{
    return a < b ? a : b;
}

/**
 * Constructs the world stream manager and allocates its disc and decompression buffers.
 *
 * Sizes the stream buffer as the EA stream bookkeeping for 2 slots, 1 queue and 1 request plus 0x68000 bytes,
 * capped at 0x80000; allocates it as "cStreamMan CDBUFFER" and creates the stream in it (func_003E06D8). Then
 * allocates the 0x14000-byte "cStreamMan DECOMPRESS" buffer and calls cStreamMan_reset().
 *
 * @return @p self
 */
extern "C" void* cStreamMan_cStreamMan(cStreamMan* self)
{
    self->m_BxChunkResourceID = 0xFFFFFFFF;
    int size = func_003E06B0(2, 1, 1);
    size += 0x18000;
    size += 0x50000;
    size = cStreamMan_min(0x80000, size);
    void* buf = operator_new_tag(size, D_00494D90, 0, 0);
    self->m_streamBuffer = (char*)buf;
    self->m_streamHandle = (int)func_003E06D8(2, 1, 1, buf, size);
    self->m_streamRequest = 0;
    self->m_pDecompressBuffer = (char*)operator_new_tag(0x14000, D_00494DA8, 0, 0);
    cStreamMan_reset(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan__cStreamMan);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void cStreamMan_reset(cStreamMan* self);
extern "C" void cStreamMan_purge(cStreamMan* self);

/**
 * Destroys the world streamer: frees an unfinished shared chunk, resets and purges the streamer, then frees it
 * when asked to.
 *
 * m_pBxChunkMemHandle is freed only when it is set, m_uiBxChunkSizeLeft is nonzero and the low byte of
 * m_BxChunkResourceID (the section) is 0xFF, the shared section. cStreamMan_reset() (reset) and cStreamMan_purge()
 * (purge: releases the stream handle and frees the stream and decompression buffers) follow.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cStreamMan__cStreamMan(cStreamMan* self, int flags)
{
    void* buf = self->m_pBxChunkMemHandle;
    if (buf != 0 && (int)self->m_uiBxChunkSizeLeft != 0 && *(unsigned char*)&self->m_BxChunkResourceID == 0xFF) {
        cMemMan_free(buf);
    }
    cStreamMan_reset(self);
    cStreamMan_purge(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_init);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern const char D_004A47D8[];
extern const char D_004A47E0[];

extern "C" int cStreamMan_init(cStreamMan* self, const char* name, int id)
{
    *(int*)self = id;
    sprintf((char*)self + 4, D_004A47D8, name, D_004A47E0);
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_reset);
#ifdef SKIP_ASM
extern "C" void cStreamMan_setBandWidthLimit(cStreamMan* self, int size);

extern "C" void cStreamMan_reset(cStreamMan* self)
{
    self->m_ActiveHull = -1;
    self->m_bLimitBandWidth = 1;
    self->m_uiNumHullChunk = (unsigned int)0;
    *(char*)((char*)self + 0x4) = 0;
    self->m_bActive = 0;
    self->m_uiBxChunkSizeLeft = (unsigned int)0;
    self->m_pBxChunkMemHandle = 0;
    self->m_pBxChunkMemPtr = 0;
    self->m_bBxChunkWriteEnable = 0;
    self->m_bBxNullChunk = 0;
    self->m_bAllocFailed = 0;
    self->m_iDecompressSizeLeft = 0;
    self->m_pDecompressDataLeft = 0;
    cStreamMan_setBandWidthLimit(self, 0x19000);
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_purge);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void func_003E0A28(int h);

extern "C" void cStreamMan_purge(cStreamMan* self)
{
    func_003E0A28(self->m_streamHandle);
    if (self->m_streamBuffer != 0) {
        cMemMan_free(self->m_streamBuffer);
    }
    if (self->m_pDecompressBuffer != 0) {
        cMemMan_free(self->m_pDecompressBuffer);
    }
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_addStreamRequest);
#ifdef SKIP_ASM
extern "C" int func_003E0C88(int h, void* p, int a, int tag);

extern "C" int cStreamMan_addStreamRequest(cStreamMan* self, int uiHullID, int uiNumHullChunk, int uiFileOffset)
{
    if (self->m_bActive == 0) {
        int r = func_003E0C88(self->m_streamHandle, (char*)self + 0x4, uiFileOffset, 0x444E4543);
        self->m_streamRequest = r;
        if (r != 0) {
            self->m_ActiveHull = uiHullID;
            self->m_bActive = 1;
            self->m_uiNumHullChunk = (unsigned int)uiNumHullChunk;
            self->m_iCarrySize = 0;
            self->m_bBxNullChunk = 0;
            self->m_iLastTimerTick = 0;
            *(int*)&self->m_fTicksLeft = 0;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_cancelStreamRequest);
#ifdef SKIP_ASM
extern "C" void func_003E0E90(int, int);

extern "C" void cStreamMan_cancelStreamRequest(cStreamMan* self)
{
    if (self->m_bActive != 0) {
        func_003E0E90(self->m_streamHandle, self->m_streamRequest);
        self->m_bActive = 0;
        self->m_ActiveHull = -1;
    }
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_setBandWidthLimit);
#ifdef SKIP_ASM
extern char* D_004A5B64;

static inline int imax(int a, int b)
{
    return a > b ? a : b;
}

extern "C" void cStreamMan_setBandWidthLimit(cStreamMan* self, int rate)
{
    rate = imax(0x8000, rate);
    char* app = D_004A5B64;
    self->m_iDataRatePerSecond = rate;
    self->m_fTicksPerChunk = (float)*(int*)(app + 0x10) / ((float)rate * 0.000030517578125f);
}
#endif

//100%
INCLUDE_ASM("world/streamman", func_003A7098);
#ifdef SKIP_ASM
struct sStreamMan7098 {
    void* f0;                   // 0x00
    char pad4[0x84];
    void* cache;                // 0x88
    int f8C;                    // 0x8C
    int active;                 // 0x90
    int f94;                    // 0x94
    char pad98[0x14];
    int pending;                // 0xAC
    void* fB0;                  // 0xB0
    int fB4;                    // 0xB4
    char padB8[0x1C];
    int fD4;                    // 0xD4
    int fD8;                    // 0xD8
    float fDC;                  // 0xDC
    float fE0;                  // 0xE0
    int fE4;                    // 0xE4
};

extern char* D_004A5B64;
void* func_003A7218(void* self, int a1, int a2, int a3);
extern "C" int func_003A7238(void* self, void* a, void* b);
extern "C" void cWorldCache_signalPageLoadComplete(void* self, int a1);
extern "C" unsigned char* func_003E12E0(void* cache);
extern "C" void func_003E13E8(void* cache, void* p);
extern "C" int func_003E1530(void* cache);

extern "C" void func_003A7098(sStreamMan7098* self)
{
    unsigned char* p;
    if (self->active == 0) {
        return;
    }
    if (self->pending != 0) {
        if (func_003A7238(self, &self->fB0, &self->fB4) == 0) {
            return;
        }
    }
    if (self->fD4 != 0) {
        int t = *(int*)(D_004A5B64 + 0x18);
        if (self->fE4 != t) {
            self->fE0 -= (float)(t - self->fE4);
            self->fE4 = t;
        }
        if (0.0f < self->fE0) {
            return;
        }
    }
    p = func_003E12E0(self->cache);
    while (p != 0) {
        int size;
        self->fE0 = self->fDC;
        size = (p[7] << 24) | (p[6] << 16) | (p[5] << 8) | p[4];
        // PORT: the payload pointer is passed as an int.
        self->fB0 = func_003A7218(self, self->f8C, size - 8, (int)(p + 8));
        func_003E13E8(self->cache, p);
        self->fB4 = self->f8C;
        if (func_003A7238(self, &self->fB0, &self->fB4) == 0) {
            self->pending = 1;
            return;
        }
        if (self->fD4 != 0) {
            break;
        }
        p = func_003E12E0(self->cache);
    }
    if (func_003E1530(self->cache) == 0) {
        return;
    }
    cWorldCache_signalPageLoadComplete(self->f0, self->f94);
    self->active = 0;
    self->f94 = -1;
}
#endif

extern "C" void* func_003B47F8(int);

//100%
INCLUDE_ASM("world/streamman", func_003A7218__FPviii);
#ifdef SKIP_ASM
void* func_003A7218(void* self, int a1, int a2, int a3)
{
    return func_003B47F8(a3);
}
#endif

//100%
INCLUDE_ASM("world/streamman", func_003A7238);
#ifdef SKIP_ASM
extern "C" unsigned int cWorldCache_addBxStreamDataTest(void* cache, int group, int id, int key, int size);
extern "C" void func_003A8CD0_s(void* self, int group, int id, unsigned int key, int a4) __asm__("func_003A8CD0");
extern "C" void MEM_copy(void* dst, void* src, int size);
extern int D_004A47D0;

struct sStreamMan7238 {
    void* cache;                // 0x00
    char pad4[0x90];
    int group;                  // 0x94
    int remaining;              // 0x98
    char pad9C[4];
    int partial;                // 0xA0
    unsigned char buf[8];       // 0xA4
    int retry;                  // 0xAC
    char padB0[8];
    int id;                     // 0xB8
    int key;                    // 0xBC
    int size;                   // 0xC0
    int flagC4;                 // 0xC4
    int copying;                // 0xC8
    int result;                 // 0xCC
    char* dst;                  // 0xD0
};


// PORT: header fields are packed in a 64-bit long; data pointers pass through int parameters.
extern "C" int func_003A7238(void* self_, void* pcount_, void* pdata_)
{
    sStreamMan7238* self = (sStreamMan7238*)self_;
    int* pcount = (int*)pcount_;
    unsigned char** pdata = (unsigned char**)pdata_;
    while (*pcount >= 8 || (*pcount > 0 && self->size != 0) || self->retry != 0)
    {
        unsigned int r;
        if (self->retry != 0)
        {
            r = cWorldCache_addBxStreamDataTest(self->cache, self->group, self->id, self->key, self->size);
            self->result = r;
            if (r == 0xFFFFFFFF)
                return 0;
            self->retry = 0;
            if (r != 0)
            {
                self->dst = (char*)r;
                self->copying = 1;
            }
            else
            {
                self->copying = 0;
                if (self->size == 0)
                    self->flagC4 = 1;
            }
        }
        else
        if (self->size == 0)
        {
            unsigned char hdr[8] __attribute__((aligned(8)));
            if (self->partial != 0)
            {
                int i;
                unsigned char* h = hdr;
                *(unsigned int*)(hdr + 4) = 0xFFFFFFFF;
                for (i = 0; i < self->partial; i++)
                    h[i] = self->buf[i];
                for (i = self->partial; i < 8; i++)
                {
                    h[i] = *(*pdata)++;
                    (*pcount)--;
                }
                self->id = hdr[0];
                self->key = *(int*)(hdr + 4);
                self->size = (int)(*(long*)hdr >> 8) & 0xFFFFFF;
                self->partial = 0;
            }
            else
            {
                *(unsigned int*)(hdr + 4) = 0xFFFFFFFF;
                unsigned char* h = hdr;
                for (int i = 0; i < 8; i++)
                {
                    *h++ = *(*pdata)++;
                    (*pcount)--;
                }
                self->id = hdr[0];
                self->key = *(int*)(hdr + 4);
                self->size = (int)(*(long*)hdr >> 8) & 0xFFFFFF;
            }
            r = cWorldCache_addBxStreamDataTest(self->cache, self->group, self->id, self->key, self->size);
            self->result = r;
            if (r == 0xFFFFFFFF)
                return 0;
            if (r != 0)
            {
                self->dst = (char*)r;
                self->copying = 1;
            }
            else
            {
                self->copying = 0;
                if (self->size == 0)
                    self->flagC4 = 1;
            }
        }
        if (*pcount != 0)
        {
            int n = *pcount;
            if (self->size < n)
                n = self->size;
            self->size -= n;
            if (self->copying != 0)
            {
                MEM_copy(self->dst, *pdata, n);
                self->dst += n;
                if (self->size == 0)
                {
                    func_003A8CD0_s(self->cache, self->group, self->id, self->key, self->result);
                    self->result = 0;
                    self->remaining--;
                }
            }
            else if (self->size == 0)
            {
                self->remaining--;
                if (self->flagC4 != 0)
                {
                    func_003A8CD0_s(self->cache, self->group, self->id, self->key, (int)&D_004A47D0);
                    self->flagC4 = 0;
                }
            }
            *pdata += n;
            *pcount -= n;
        }
        if (self->remaining == 0)
        {
            *pcount = 0;
            break;
        }
    }
    if (*pcount != 0)
    {
        self->partial = *pcount;
        for (int i = 0; i < *pcount; i++)
            self->buf[i] = *(*pdata)++;
    }
    return 1;
}
#endif

