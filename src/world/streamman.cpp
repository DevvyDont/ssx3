#include "common.h"
#include "bx/appman.h"
#include "world/streamman.h"
#include "world/worldcache.h"

//100%
INCLUDE_ASM("world/streamman", cStreamMan_cStreamMan);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int STREAM_overhead(int a, int b, int c);
extern "C" void* STREAM_create(int a, int b, int c, void* buf, int size);
extern "C" void cStreamMan_reset(cStreamMan* self);
extern char str_cStreamMan_CDBUFFER[];
extern char str_cStreamMan_DECOMPRESS[];

static inline int cStreamMan_min(int a, int b)
{
    return a < b ? a : b;
}

/**
 * Constructs the world stream manager and allocates its disc and decompression buffers.
 *
 * Sizes the stream buffer as the EA stream bookkeeping for 2 slots, 1 queue and 1 request plus 0x68000 bytes,
 * capped at 0x80000; allocates it as "cStreamMan CDBUFFER" and creates the stream in it (STREAM_create). Then
 * allocates the 0x14000-byte "cStreamMan DECOMPRESS" buffer and calls cStreamMan_reset().
 *
 * @return @p self
 */
extern "C" void* cStreamMan_cStreamMan(cStreamMan* self)
{
    self->m_BxChunkResourceID = 0xFFFFFFFF;
    int size = STREAM_overhead(2, 1, 1);
    size += 0x18000;
    size += 0x50000;
    size = cStreamMan_min(0x80000, size);
    void* buf = operator_new_tag(size, str_cStreamMan_CDBUFFER, 0, 0);
    self->m_streamBuffer = (char*)buf;
    self->m_streamHandle = (int)STREAM_create(2, 1, 1, buf, size);
    self->m_streamRequest = 0;
    self->m_pDecompressBuffer = (char*)operator_new_tag(0x14000, str_cStreamMan_DECOMPRESS, 0, 0);
    self->reset();
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
 * m_BxChunkResourceID (the bank) is cWorldCache::kSharedBank (0xFF), the shared bank. cStreamMan_reset() (reset) and
 * cStreamMan_purge() (purge: releases the stream handle and frees the stream and decompression buffers) follow.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cStreamMan__cStreamMan(cStreamMan* self, int flags)
{
    void* buf = self->m_pBxChunkMemHandle;
    if (buf != 0 && (int)self->m_uiBxChunkSizeLeft != 0
        && *(unsigned char*)&self->m_BxChunkResourceID == cWorldCache::kSharedBank) {
        cMemMan_free(buf);
    }
    self->reset();
    self->purge();
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_init);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern const char str_pct_spct_s_e[];
extern const char str_ssb[];

/**
 * Sets the streamer up for a track: stores the world cache to feed and builds the stream file name
 * "|<name>.ssb" in m_filename.
 *
 * Despite its name, @p id is the cWorldCache pointer (stored in mp_WorldCache; On Tour pWorldCache).
 *
 * @param name  the track's base file name (On Tour baseFileName)
 * @param id  the world cache that receives the streamed chunks
 * @return always 1
 */
extern "C" int cStreamMan_init(cStreamMan* self, const char* name, int id)
{
    *(int*)self = id;
    sprintf((char*)&self->m_filename, str_pct_spct_s_e, name, str_ssb);
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_reset);
#ifdef SKIP_ASM
extern "C" void cStreamMan_setBandWidthLimit(cStreamMan* self, int size);

/**
 * Returns the streamer to idle: no active hull request, no chunk in progress and the default bandwidth limit.
 *
 * Sets m_ActiveHull to -1, clears m_filename[0], m_bActive, m_uiNumHullChunk and the chunk, decompression and
 * allocation-failure state, turns m_bLimitBandWidth on and calls cStreamMan_setBandWidthLimit() with 0x19000
 * bytes (100 KB) per second.
 */
void cStreamMan::reset()
{
    m_ActiveHull = -1;
    m_bLimitBandWidth = 1;
    m_uiNumHullChunk = (unsigned int)0;
    m_filename[0] = 0;
    m_bActive = 0;
    m_uiBxChunkSizeLeft = (unsigned int)0;
    m_pBxChunkMemHandle = 0;
    m_pBxChunkMemPtr = 0;
    m_bBxChunkWriteEnable = 0;
    m_bBxNullChunk = 0;
    m_bAllocFailed = 0;
    m_iDecompressSizeLeft = 0;
    m_pDecompressDataLeft = 0;
    setBandWidthLimit(0x19000);
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_purge);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void STREAM_destroy(int h);

/**
 * Releases the streamer's EA stream and frees its disc and decompression buffers.
 *
 * STREAM_destroy() waits while the stream is busy and then shuts it down; m_streamBuffer and
 * m_pDecompressBuffer are freed when set. The pointers are not cleared afterwards.
 */
void cStreamMan::purge()
{
    STREAM_destroy(m_streamHandle);
    if (m_streamBuffer != 0) {
        cMemMan_free(m_streamBuffer);
    }
    if (m_pDecompressBuffer != 0) {
        cMemMan_free(m_pDecompressBuffer);
    }
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_addStreamRequest);
#ifdef SKIP_ASM
extern "C" int STREAM_queuefile(int h, void* p, int a, int tag);

/**
 * Starts loading a hull page by queuing a read of the track's stream file, unless a request is already
 * active.
 *
 * Queues m_filename at @p uiFileOffset on the EA stream (STREAM_queuefile, chunk tag 0x444E4543, "CEND"). On
 * success it records the hull and its chunk count, sets m_bActive and clears m_iCarrySize, m_bBxNullChunk,
 * m_iLastTimerTick and m_fTicksLeft.
 *
 * @param uiHullID  the hull page to load (stored in m_ActiveHull)
 * @param uiNumHullChunk  number of chunks in the page (m_uiNumHullChunk)
 * @param uiFileOffset  byte offset of the page in the stream file
 * @return true when the read was queued (stored in m_streamRequest), false when a request was already active or
 *         queuing failed
 */
bool cStreamMan::addStreamRequest(int uiHullID, int uiNumHullChunk, int uiFileOffset)
{
    if (m_bActive == 0) {
        int streamReq = STREAM_queuefile(m_streamHandle, (char*)&m_filename, uiFileOffset, 0x444E4543);
        m_streamRequest = streamReq;
        if (streamReq != 0) {
            m_ActiveHull = uiHullID;
            m_bActive = 1;
            m_uiNumHullChunk = (unsigned int)uiNumHullChunk;
            m_iCarrySize = 0;
            m_bBxNullChunk = 0;
            m_iLastTimerTick = 0;
            *(int*)&m_fTicksLeft = 0;
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_cancelStreamRequest);
#ifdef SKIP_ASM
extern "C" void STREAM_cancelrequest(int, int);

/**
 * Cancels the active hull stream request, if there is one, and marks the streamer idle.
 *
 * Passes m_streamRequest to the EA stream's cancel (STREAM_cancelrequest), clears m_bActive and sets m_ActiveHull to
 * -1. On Tour's version takes the hull ID (and only asserts on it); this one takes none.
 */
void cStreamMan::cancelStreamRequest()
{
    if (m_bActive != 0) {
        STREAM_cancelrequest(m_streamHandle, m_streamRequest);
        m_bActive = 0;
        m_ActiveHull = -1;
    }
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_setBandWidthLimit);
#ifdef SKIP_ASM

static inline int imax(int a, int b)
{
    return a > b ? a : b;
}

/**
 * Sets the streaming bandwidth limit and the matching number of timer ticks per 32 KB chunk.
 *
 * The rate is raised to at least 0x8000 bytes per second and kept in m_iDataRatePerSecond; m_fTicksPerChunk
 * becomes the app timer's ticks per second (word +0x10 of cAppMan_TheAppMan) divided by rate / 32768.
 *
 * @param rate  bytes per second (On Tour iBytesPerSecond); values below 0x8000 are raised to it
 */
void cStreamMan::setBandWidthLimit(int rate)
{
    rate = imax(0x8000, rate);
    cAppMan* app = cAppMan_TheAppMan;
    m_iDataRatePerSecond = rate;
    m_fTicksPerChunk = (float)app->TimerHz / ((float)rate * 0.000030517578125f);
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_update);
#ifdef SKIP_ASM

void* cStreamMan_decompressChunk(void* self, int a1, int a2, int a3);
extern "C" bool cStreamMan_processChunk(cStreamMan* self, void* a, void* b);
extern "C" void cWorldCache_signalPageLoadComplete(void* self, int a1);
extern "C" unsigned char* STREAM_get(void* cache);
extern "C" void STREAM_release(void* cache, void* p);
extern "C" int STREAM_isendofstream(void* cache);

/**
 * Feeds newly streamed world data from the disc stream into the world cache.
 *
 * Does nothing while inactive (+0x90). A chunk left half processed (+0xAC) is retried first. When throttled (+0xD4),
 * the countdown at +0xE0 drops by the app ticks passed and nothing more happens while it is positive. Then each chunk
 * STREAM_get returns resets the countdown, is decompressed (cStreamMan_decompressChunk) into the buffer at +0x8C, released
 * (STREAM_release) and handed to cStreamMan_processChunk; if that cannot finish, the chunk is marked pending. Only one chunk is
 * taken per call while throttled. Once STREAM_isendofstream reports the end of the stream, the world cache is told page +0x94
 * is loaded, the manager goes inactive and the page becomes -1.
 */
void cStreamMan::update()
{
    unsigned char* chunk;
    if (m_bActive == 0) {
        return;
    }
    if (m_bAllocFailed != 0) {
        if (processChunk((void**)&m_iDecompressSizeLeft, (int*)&m_pDecompressDataLeft) == 0) {
            return;
        }
    }
    if (m_bLimitBandWidth != 0) {
        int tick = cAppMan_TheAppMan->TimerTick;
        if (m_iLastTimerTick != tick) {
            m_fTicksLeft -= (float)(tick - m_iLastTimerTick);
            m_iLastTimerTick = tick;
        }
        if (0.0f < m_fTicksLeft) {
            return;
        }
    }
    chunk = STREAM_get(((void*)m_streamHandle));
    while (chunk != 0) {
        int size;
        m_fTicksLeft = m_fTicksPerChunk;
        size = (chunk[7] << 24) | (chunk[6] << 16) | (chunk[5] << 8) | chunk[4];
        // PORT: the payload pointer is passed as an int.
        *(void**)&m_iDecompressSizeLeft = this->decompressChunk(((int)m_pDecompressBuffer), size - 8,
                (int)(chunk + 8));
        STREAM_release(((void*)m_streamHandle), chunk);
        *(int*)&m_pDecompressDataLeft = ((int)m_pDecompressBuffer);
        if (processChunk((void**)&m_iDecompressSizeLeft, (int*)&m_pDecompressDataLeft) == 0) {
            m_bAllocFailed = 1;
            return;
        }
        if (m_bLimitBandWidth != 0) {
            break;
        }
        chunk = STREAM_get(((void*)m_streamHandle));
    }
    if (STREAM_isendofstream(((void*)m_streamHandle)) == 0) {
        return;
    }
    cWorldCache_signalPageLoadComplete(((void*)mp_WorldCache), m_ActiveHull);
    m_bActive = 0;
    m_ActiveHull = -1;
}
#endif

extern "C" void* unpack(int);

//100%
INCLUDE_ASM("world/streamman", cStreamMan_decompressChunk__FPviii);
#ifdef SKIP_ASM
/**
 * Decompresses a stream chunk: unpack(@p src, @p dst), returning the unpacked size.
 *
 * @p srcSize is not used. cStreamMan_update calls it with the decompression buffer (+0x8C), the chunk size minus 8 and the
 * chunk data after its 8-byte header.
 *
 * @param dst  the destination buffer
 * @param srcSize  the source size (unused)
 * @param src  the compressed data
 * @return unpack's result
 */
void* cStreamMan::decompressChunk(int dst, int srcSize, int src)
{
    return unpack(src);
}
#endif

//100%
INCLUDE_ASM("world/streamman", cStreamMan_processChunk);
#ifdef SKIP_ASM
extern "C" unsigned int cWorldCache_addBxStreamDataTest(void* cache, int group, int id, int key, int size);
extern "C" void cWorldCache_addBxStreamDataSubmit_s(void* self, int group, int id, unsigned int key, int a4) __asm__("cWorldCache_addBxStreamDataSubmit");
extern "C" void MEM_copy(void* dst, void* src, int size);
extern int cStreamMan_NULLCHUNK;

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


/**
 * Hands a buffer of streamed world data, block by block, to the world cache.
 *
 * While data remains, a new block's 8-byte header is read (type byte m_BxChunkDataType, 24-bit m_uiBxChunkSizeLeft,
 * resource id word; a header split across buffers is completed from m_sCarryData, m_iCarrySize bytes) and
 * cWorldCache_addBxStreamDataTest asked for a destination for page m_ActiveHull: kStreamNoRoom means no room yet and
 * false is returned (the caller, cStreamMan_update, then sets m_bAllocFailed so the next call retests first); 0 means
 * the block is skipped (an empty one is submitted as cStreamMan_NULLCHUNK). Block data is copied with MEM_copy and each
 * completed block is submitted with cWorldCache_addBxStreamDataSubmit, counting down the blocks left in the page
 * (m_uiNumHullChunk). Leftover bytes shorter than a header are kept for the next call; when the page's last block is
 * done the rest of the buffer is dropped.
 *
 * @param self_    the stream manager (cStreamMan, this)
 * @param pcount_  bytes left in the buffer; updated as data is consumed (On Tour iSize)
 * @param pdata_   read position in the buffer; advanced as data is consumed (On Tour pData)
 * @return true when the buffer was used up, false when the cache had no room and the call must be repeated
 */
// PORT: header fields are packed in a 64-bit long; data pointers pass through int parameters.
bool cStreamMan::processChunk(void* pcount_, void* pdata_)
{
    sStreamMan7238* self = (sStreamMan7238*)this;
    int* pcount = (int*)pcount_;
    unsigned char** pdata = (unsigned char**)pdata_;
    while (*pcount >= 8 || (*pcount > 0 && self->size != 0) || self->retry != 0)
    {
        unsigned int addResult;
        if (self->retry != 0)
        {
            addResult = cWorldCache_addBxStreamDataTest(self->cache, self->group, self->id, self->key, self->size);
            self->result = addResult;
            if (addResult == cWorldCache::kStreamNoRoom)
                return false;
            self->retry = 0;
            if (addResult != 0)
            {
                self->dst = (char*)addResult;
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
                unsigned char* hdrCursor = hdr;
                *(unsigned int*)(hdr + 4) = 0xFFFFFFFF;
                for (i = 0; i < self->partial; i++)
                    hdrCursor[i] = self->buf[i];
                for (i = self->partial; i < 8; i++)
                {
                    hdrCursor[i] = *(*pdata)++;
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
                unsigned char* hdrCursor = hdr;
                for (int i = 0; i < 8; i++)
                {
                    *hdrCursor++ = *(*pdata)++;
                    (*pcount)--;
                }
                self->id = hdr[0];
                self->key = *(int*)(hdr + 4);
                self->size = (int)(*(long*)hdr >> 8) & 0xFFFFFF;
            }
            addResult = cWorldCache_addBxStreamDataTest(self->cache, self->group, self->id, self->key, self->size);
            self->result = addResult;
            if (addResult == cWorldCache::kStreamNoRoom)
                return false;
            if (addResult != 0)
            {
                self->dst = (char*)addResult;
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
                    cWorldCache_addBxStreamDataSubmit_s(self->cache, self->group, self->id, self->key, self->result);
                    self->result = 0;
                    self->remaining--;
                }
            }
            else if (self->size == 0)
            {
                self->remaining--;
                if (self->flagC4 != 0)
                {
                    cWorldCache_addBxStreamDataSubmit_s(self->cache, self->group, self->id, self->key, (int)&cStreamMan_NULLCHUNK);
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
    return true;
}
#endif

