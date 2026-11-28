#ifndef WORLD_STREAMMAN_H
#define WORLD_STREAMMAN_H

// cStreamMan: the world's disc streamer. It reads one hull page of the track's "|<track>.ssb" stream at a time
// through the bx async-file API, decompresses it and hands each chunk to its cWorldCache
// (addBxStreamDataTest/addBxStreamDataSubmit). There is one, embedded by value at cWorldCache+0x300
// (cWorldCache_cWorldCache constructs it in place, cWorldCache__cWorldCache destroys it); no vtable, no base class.
// EA's source is d:/ssx/code/source/world/streamman.cpp (preview assert strings); methods 0x003A6D50-0x003A7598
// (world/streamman):
//   cStreamMan::cStreamMan, cStreamMan::~cStreamMan, cStreamMan_init, cStreamMan_reset,
//   cStreamMan_purge, cStreamMan_addStreamRequest, cStreamMan_cancelStreamRequest,
//   cStreamMan_setBandWidthLimit, cStreamMan_update, cStreamMan_decompressChunk,
//   cStreamMan_processChunk. The static _NULLCHUNK is cStreamMan_NULLCHUNK.
//
// Size 0xE8: cWorldCache's next member, mp_ChunkResolve, is at 0x3E8 = 0x300 + 0xE8 (cWorldCache_init stores
// cChunkResolve_construct() there), and the last field written is 0xE4 (cStreamMan_addStreamRequest, cStreamMan_update).
// On Tour's cStreamMan is 0xE4: its bools m_bBxNullChunk/m_bBxChunkWriteEnable share one word at 0xC4/0xC5; retail
// stores each as a 4-byte word (cStreamMan_processChunk sw 0xC4 and 0xC8), so retail = On Tour + 4 from m_bBxChunkWriteEnable on.
//
// A FLAT layout, like main/game.h: every field at its retail offset. Member names are EA's, from SSX On Tour's PSP
// DWARF (class cStreamMan), placed by:
//   [P] the July 2003 preview's assert strings in streamman.cpp ("uiHullID==(tUInt32)m_ActiveHull",
//       "m_uiBxChunkSizeLeft==0");
//   [R] retail's constructor (cStreamMan::cStreamMan: tags "cStreamMan CDBUFFER", "cStreamMan DECOMPRESS"), init and
//       reset (cStreamMan_reset);
//   [U] how retail code uses the field;
//   [O] On Tour's member order, where the fields on both sides are placed by [P], [R] or [U].
// PS2 bools are 4-byte ints here. Unnamed fields would be unk_XXX; there are none.

#include "layout.h"

struct cWorldCache;

void operator_delete(int* p);

struct cStreamMan {
    cWorldCache* mp_WorldCache;     // 0x000 [O]; [R] init(baseFileName, pWorldCache) stores its third argument
                                    //       (the view stores it as int); [U] processChunk/update pass it to the cache
    char m_filename[128];           // 0x004 [O]; [R] init: sprintf(m_filename, "|%s%s", name, ".ssb"); reset clears [0];
                                    //       [U] addStreamRequest passes it to the async open (STREAM_queuefile)
    char* m_streamBuffer;           // 0x084 [O]; [R] new(min(0x80000, ...), "cStreamMan CDBUFFER"); freed by purge
    int m_streamHandle;             // 0x088 [O]; [R] = STREAM_create(2, 1, 1, buffer, size) (the ctor's view stores a
                                    //       void*); [U] the async read/poll/cancel calls take it
    char* m_pDecompressBuffer;      // 0x08C [O]; [R] new(0x14000, "cStreamMan DECOMPRESS"); [U] update decompresses into it
    int m_bActive;                  // 0x090 [O] (a bool); [R] reset = 0; [U] addStreamRequest sets 1 (one read in flight),
                                    //       cWorldCache_updatePages tests it (lw cache+0x390)
    int m_ActiveHull;               // 0x094 [P] assert m_ActiveHull; [R] reset = -1; [U] addStreamRequest stores the hull
                                    //       page; update passes it to signalPageLoadComplete (cWorldCache_signalPageLoadComplete)
    unsigned int m_uiNumHullChunk;  // 0x098 [O]; [R] reset = 0; [U] addStreamRequest stores the chunk count,
                                    //       processChunk counts it down
    int m_streamRequest;            // 0x09C [O]; [R] ctor = 0; [U] addStreamRequest stores the request id,
                                    //       cancelStreamRequest passes it to STREAM_cancelrequest
    int m_iCarrySize;               // 0x0A0 [O]; [U] processChunk: bytes of a split 8-byte chunk header carried over
    char m_sCarryData[8];           // 0x0A4 [O]; [U] processChunk copies the carried header bytes here (lbu/sb)
    int m_bAllocFailed;             // 0x0AC [O] (a bool); [R] reset = 0; [U] update sets it when processChunk
                                    //       can't finish a chunk (addBxStreamDataTest returned -1) and retries that chunk
                                    //       first while it is set
    int m_iDecompressSizeLeft;      // 0x0B0 [O]; [R] reset = 0; [U] update stores decompressChunk's result and passes
                                    //       &it to processChunk (update's view types it void*)
    char* m_pDecompressDataLeft;    // 0x0B4 [O]; [R] reset = 0; [U] update stores m_pDecompressBuffer and passes &it to
                                    //       processChunk (update's view types it int)
    int m_BxChunkDataType;          // 0x0B8 [O] eBxStreamDataType (0..27); [U] processChunk: header byte 0
    unsigned int m_BxChunkResourceID; // 0x0BC [O] tResourceID (section:8 | index<<8; section 0xFF = shared);
                                    //       [R] ctor = 0xFFFFFFFF; [U] processChunk: header word 1; the dtor reads its
                                    //       low byte (lbu 0xBC == 0xFF)
    unsigned int m_uiBxChunkSizeLeft; // 0x0C0 [P] assert m_uiBxChunkSizeLeft==0; [R] reset = 0; [U] processChunk:
                                    //       header bytes 1-3, counted down while copying
    int m_bBxNullChunk;             // 0x0C4 [O] (a bool); [R] reset = 0; [U] processChunk sets it for an empty chunk
    int m_bBxChunkWriteEnable;      // 0x0C8 [O] (a bool); [R] reset = 0; [U] processChunk: 1 while copying into the cache
    char* m_pBxChunkMemHandle;      // 0x0CC [O]; [R] reset = 0; [U] processChunk stores addBxStreamDataTest's result
                                    //       (its view types it int); the dtor frees it when a shared chunk is unfinished
    char* m_pBxChunkMemPtr;         // 0x0D0 [O]; [R] reset = 0; [U] processChunk's copy cursor
    int m_bLimitBandWidth;          // 0x0D4 [O] (a bool); [R] reset = 1; [U] update throttles while set; cGame_loadTrack,
                                    //       cGame_update, cGame_updateStreamLoading store 0/1 through the world (sw cache+0x3D4)
    int m_iDataRatePerSecond;       // 0x0D8 [O]; [R] setBandWidthLimit stores max(0x8000, rate); reset passes 0x19000
    float m_fTicksPerChunk;         // 0x0DC [O]; [R] setBandWidthLimit: ticks per second / (rate / 32768)
    float m_fTicksLeft;             // 0x0E0 [O]; [U] update counts it down and reloads it from m_fTicksPerChunk;
                                    //       addStreamRequest stores 0 (as an int in its view)
    int m_iLastTimerTick;           // 0x0E4 [O]; [U] update: the app timer tick of the last throttle step;
                                    //       addStreamRequest = 0

    // Methods bound to their retail symbols by asm labels.
    void reset() __asm__("cStreamMan_reset");
    void purge() __asm__("cStreamMan_purge");
    bool addStreamRequest(int uiHullID, int uiNumHullChunk, int uiFileOffset) __asm__("cStreamMan_addStreamRequest");
    void cancelStreamRequest() __asm__("cStreamMan_cancelStreamRequest");
    void setBandWidthLimit(int rate) __asm__("cStreamMan_setBandWidthLimit");
    void update() __asm__("cStreamMan_update");
    bool processChunk(void* pcount_, void* pdata_) __asm__("cStreamMan_processChunk");
    void* decompressChunk(int dst, int srcSize, int src) __asm__("cStreamMan_decompressChunk__FPviii");
    int init(const char* name, int id) __asm__("cStreamMan_init");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    cStreamMan();
    ~cStreamMan();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

CHECK_OFFSET(cStreamMan, mp_WorldCache, 0x000);
CHECK_OFFSET(cStreamMan, m_filename, 0x004);
CHECK_OFFSET(cStreamMan, m_streamBuffer, 0x084);
CHECK_OFFSET(cStreamMan, m_streamHandle, 0x088);
CHECK_OFFSET(cStreamMan, m_pDecompressBuffer, 0x08C);
CHECK_OFFSET(cStreamMan, m_bActive, 0x090);
CHECK_OFFSET(cStreamMan, m_ActiveHull, 0x094);
CHECK_OFFSET(cStreamMan, m_uiNumHullChunk, 0x098);
CHECK_OFFSET(cStreamMan, m_streamRequest, 0x09C);
CHECK_OFFSET(cStreamMan, m_iCarrySize, 0x0A0);
CHECK_OFFSET(cStreamMan, m_sCarryData, 0x0A4);
CHECK_OFFSET(cStreamMan, m_bAllocFailed, 0x0AC);
CHECK_OFFSET(cStreamMan, m_iDecompressSizeLeft, 0x0B0);
CHECK_OFFSET(cStreamMan, m_pDecompressDataLeft, 0x0B4);
CHECK_OFFSET(cStreamMan, m_BxChunkDataType, 0x0B8);
CHECK_OFFSET(cStreamMan, m_BxChunkResourceID, 0x0BC);
CHECK_OFFSET(cStreamMan, m_uiBxChunkSizeLeft, 0x0C0);
CHECK_OFFSET(cStreamMan, m_bBxNullChunk, 0x0C4);
CHECK_OFFSET(cStreamMan, m_bBxChunkWriteEnable, 0x0C8);
CHECK_OFFSET(cStreamMan, m_pBxChunkMemHandle, 0x0CC);
CHECK_OFFSET(cStreamMan, m_pBxChunkMemPtr, 0x0D0);
CHECK_OFFSET(cStreamMan, m_bLimitBandWidth, 0x0D4);
CHECK_OFFSET(cStreamMan, m_iDataRatePerSecond, 0x0D8);
CHECK_OFFSET(cStreamMan, m_fTicksPerChunk, 0x0DC);
CHECK_OFFSET(cStreamMan, m_fTicksLeft, 0x0E0);
CHECK_OFFSET(cStreamMan, m_iLastTimerTick, 0x0E4);
CHECK_SIZE(cStreamMan, 0xE8);

#endif /* WORLD_STREAMMAN_H */
