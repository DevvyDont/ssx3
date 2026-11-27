#ifndef EALIB_STREAM_H
#define EALIB_STREAM_H

// REAL's STREAM module (C): a ring buffer that reads queued files (FILESYS) or memory blocks, splits the data into
// chunks ({type, size} headers) and hands each chunk to a "tap" chosen by type filters. STREAM_create lays out one
// caller-supplied block: the 0x180-byte STREAMHEADERtag ('STRM'), `requests` 0x124-byte requests, `filters` 0xC-byte
// filters, `taps` 0x10-byte taps, then the 64-byte-aligned data area (STREAM_overhead = 0x180 + 0x124 r + 0xC f +
// 0x10 t + 0x40). The handle callers hold is a TAPSTRUCTtag* (validatehandle checks tap->stream->id == 'STRM').
// Users: the movie player (ealib/avplayer.h: video and audio STREAMs), SNDSTRM. Retail code: unit ealib/seg_2DCAE8,
// validatehandle .. STREAM_requeststate (0x3DFBD0..0x3E1700). EA source: realcore source/file/cmn/stream.cpp (On Tour).
//
// Names: SSX On Tour's DWARF has this module's static functions with their types - getfreerequest(STREAMHEADERtag*)
// returning REQUESTSTRUCTtag*, filterchunk(STREAMHEADERtag*, STREAMCHUNKHDR*), parsechunks / startnextrequest /
// restartstream(STREAMHEADERtag*, ...) in stream.cpp - and the structs STREAMHEADERtag (0x180), FILTERSTRUCTtag (0xC),
// TAPSTRUCTtag (0x10), STREAMCHUNKHDR (0x8), MUTEX (0x10), whose members sit at every retail offset. The 0x124 request
// is stream.cpp's own REQUESTSTRUCTtag, not in the DWARF (only asyncfile's 0x30 REQUESTSTRUCTtag and streamplus.cpp's
// 0x12C REQUESTSTRUCT are): its members follow REQUESTSTRUCT, which has the same layout up to 0x11C, and the API's
// parameter names (STREAM_queuefile(handle, fname, offset, endchunkid)). Its C name here is STREAMREQUESTtag (ours):
// hlafile.cpp's own, different 0x30 REQUESTSTRUCTtag (ealib/hlafile.h) shares the C unit ealib/seg_2DCAE8.
//
// C structs, flat. Evidence tags: [R] STREAM_create (stores every header field) and STREAM_overhead's sizes; [O] On
// Tour's DWARF; [U] retail use.

#include "layout.h"

// C units (ealib/seg_2DCAE8.c) name these types without the struct keyword.
#ifndef __cplusplus
typedef struct MUTEX MUTEX;
typedef struct STREAMCHUNKHDR STREAMCHUNKHDR;
typedef struct STREAMREQUESTtag STREAMREQUESTtag;
typedef struct FILTERSTRUCTtag FILTERSTRUCTtag;
typedef struct TAPSTRUCTtag TAPSTRUCTtag;
typedef struct STREAMHEADERtag STREAMHEADERtag;
#endif

// MUTEX (0x10 [O]): REAL's opaque mutex (MUTEX_create / _lock / _unlock / _destroy).
struct MUTEX {
    int reserved[4];                    // 0x0 [O]
};

// STREAMCHUNKHDR (0x8 [O]): the header of every chunk in the data (type -1 = wrap marker, -2 = released / no tap).
struct STREAMCHUNKHDR {
    int type;                           // 0x0 [O]; [U] filterchunk matches it; parsechunks / restartstream mark it
    int size;                           // 0x4 [O]; [U] parsechunks: top byte = tap number once filtered
};

// One stream request (0x124 [R] STREAM_overhead / STREAM_create); EA's REQUESTSTRUCTtag (see the note above).
struct STREAMREQUESTtag {
    int id;                             // 0x000 [O]; [R] STREAM_create: the slot index; [U] getfreerequest ORs a serial
                                        //       (step 0x100) above the slot byte; locaterequest1 compares it
    int state;                          // 0x004 [O] STREAMREQUESTSTATE (FREE 0, PENDING 1, EXECUTING 2, COMPLETED 3,
                                        //       CANCELED 4); [R] 0; [U] queuerequest 1, startnextrequest 2, readcallback 3
    STREAMREQUESTtag* prev;             // 0x008 [O]; [U] queuerequest / freerequest
    STREAMREQUESTtag* next;             // 0x00C [O]; [R] STREAM_create chains the free list through it
    int type;                           // 0x010 [O] READTYPE; [U] STREAM_queuefile 0 (file), STREAM_queuemem 1 (memory)
    char fname[255];                    // 0x014 [O]; [U] STREAM_queuefile strncpy's the name; startnextrequest compares /
                                        //       copies it to the header's fname
    char unk_113;                       // 0x113 padding
    char* address;                      // 0x114 [O]; [U] STREAM_queuemem: the data; restartstream advances it
    int parm;                           // 0x118 [O]; [U] queuefile: start offset (-> foffset); queuemem: the length
    int endchunkid;                     // 0x11C [U] queuefile / queuemem store their endchunkid argument; parsechunks stops
                                        //       at a chunk of that type (pads it to 64 bytes)
    char* datastart;                    // 0x120 [O] (REQUESTSTRUCT); [U] startnextrequest: the header's datatail at
                                        //       start; restartstream frees requests whose data was consumed
};

// FILTERSTRUCTtag (0xC [O]): chunk type filter (STREAM_setfilter(handle, filternum, mask, value, tapnum)).
struct FILTERSTRUCTtag {
    int mask;                           // 0x0 [O]; [R] STREAM_create 0
    int value;                          // 0x4 [O]; [R] 0
    int tapnum;                         // 0x8 [O]; [R] 1; [U] filterchunk returns it (-2 when nothing matches)
};

// TAPSTRUCTtag (0x10 [O]): one consumer of the stream; the handle returned by STREAM_create / STREAM_taphandle.
struct STREAMHEADERtag;
struct TAPSTRUCTtag {
    STREAMHEADERtag* stream;            // 0x0 [O]; [R] STREAM_create; [U] validatehandle
    int tapnum;                         // 0x4 [O]; [R] index + 1
    int gettable;                       // 0x8 [O]; [R] 0; [U] parsechunks adds each chunk's size
    char* getptr;                       // 0xC [O]; [U] parsechunks: the first chunk when gettable was 0
};

// STREAMHEADERtag (0x180 [O] [R]).
// The id word of a live stream header (name ours): [R] the bytes "STRM" in memory, a little-endian word; [U]
// STREAM_create stores it, validatehandle checks it.
#define STREAM_HEADER_ID 0x4D525453
struct STREAMHEADERtag {
    int id;                             // 0x000 [O]; [R] 'STRM' (0x4D525453); [U] validatehandle
    MUTEX mutex;                        // 0x004 [O]; [R] MUTEX_create; [U] locked around queue / usage changes
    STREAMREQUESTtag* request;          // 0x014 [O]; [R] the request array (after the header); [U] locaterequest1
    int requests;                       // 0x018 [O]; [R]; [U] locaterequest1
    FILTERSTRUCTtag* filter;            // 0x01C [O]; [R]; [U] filterchunk
    int filters;                        // 0x020 [O]; [R]; [U] filterchunk
    TAPSTRUCTtag* tap;                  // 0x024 [O]; [R]; [U] parsechunks
    int taps;                           // 0x028 [O]; [R]
    char* actualbufferstart;            // 0x02C [O]; [R] the aligned data area; [U] restartstream
    char* bufferstart;                  // 0x030 [O]; [R]; [U] restartstream moves it when it compacts the data
    char* bufferend;                    // 0x034 [O]; [R] block + size; [U] restartstream
    volatile int state;                 // 0x038 [O] STREAMSTATE (IDLE 0, RUNNING 1, STOPPED 2); [R] 0; [U] queue*,
                                        //       startnextrequest 0, restartstream 2, decbufferusage
    int prioritylow;                    // 0x03C [O]; [R] 150; [U] STREAM_queuefile / setpriority
    int priorityhigh;                   // 0x040 [O]; [R] 50; [U] the callbacks, decbufferusage
    int greedylevel;                    // 0x044 [O]; [R] 0; [U] parsechunks / decbufferusage
    int greedystate;                    // 0x048 [O]; [R] 0; [U] parsechunks 0, decbufferusage 1
    int bufferusage;                    // 0x04C [O]; [R] 0; [U] parsechunks adds, decbufferusage subtracts
    char* datastart;                    // 0x050 [O]; [R]; [U] restartstream skips consumed chunks
    char* datatail;                     // 0x054 [O]; [R]; [U] parsechunks walks it; startnextrequest
    char* dataend;                      // 0x058 [O]; [R]; [U] readcallback adds the bytes read; parsechunks
    STREAMREQUESTtag* firstreq;         // 0x05C [O]; [R] 0; [U] queuerequest / freerequest / restartstream
    STREAMREQUESTtag* curreq;           // 0x060 [O]; [R] 0; [U] parsechunks, startnextrequest, readcallback
    STREAMREQUESTtag* lastreq;          // 0x064 [O]; [R] 0; [U] queuerequest / freerequest
    STREAMREQUESTtag* freereq;          // 0x068 [O]; [R] the first request; [U] getfreerequest pops, freerequest pushes
    char fname[255];                    // 0x06C [O]; [R] MEM_clear 0x100; [U] startnextrequest / closecallback (the open
                                        //       file's name)
    char unk_16B;                       // 0x16B padding (cleared with fname)
    int fhandle;                        // 0x16C [O]; [R] 0; [U] opencallback stores the FILESYS handle; reads use it
    int foffset;                        // 0x170 [O]; [U] startnextrequest / readcallback / restartstream
    int fop;                            // 0x174 [O]; [U] the pending FILESYS operation (open / close / read)
    int readsize;                       // 0x178 [O]; [U] restartstream / readcallback
    int readblocksize;                  // 0x17C [O]; [R] 0x800..0x8000 by the data size; [U] restartstream
};

CHECK_OFFSET(MUTEX, reserved, 0x0);
CHECK_SIZE(MUTEX, 0x10);

CHECK_OFFSET(STREAMCHUNKHDR, type, 0x0);
// (size at 0x4 follows from type and the 0x8 size; CHECK_OFFSET(.., size, ..) would clash with CHECK_SIZE in C)
CHECK_SIZE(STREAMCHUNKHDR, 0x8);

CHECK_OFFSET(STREAMREQUESTtag, id, 0x000);
CHECK_OFFSET(STREAMREQUESTtag, state, 0x004);
CHECK_OFFSET(STREAMREQUESTtag, prev, 0x008);
CHECK_OFFSET(STREAMREQUESTtag, next, 0x00C);
CHECK_OFFSET(STREAMREQUESTtag, type, 0x010);
CHECK_OFFSET(STREAMREQUESTtag, fname, 0x014);
CHECK_OFFSET(STREAMREQUESTtag, unk_113, 0x113);
CHECK_OFFSET(STREAMREQUESTtag, address, 0x114);
CHECK_OFFSET(STREAMREQUESTtag, parm, 0x118);
CHECK_OFFSET(STREAMREQUESTtag, endchunkid, 0x11C);
CHECK_OFFSET(STREAMREQUESTtag, datastart, 0x120);
CHECK_SIZE(STREAMREQUESTtag, 0x124);

CHECK_OFFSET(FILTERSTRUCTtag, mask, 0x0);
CHECK_OFFSET(FILTERSTRUCTtag, value, 0x4);
CHECK_OFFSET(FILTERSTRUCTtag, tapnum, 0x8);
CHECK_SIZE(FILTERSTRUCTtag, 0xC);

CHECK_OFFSET(TAPSTRUCTtag, stream, 0x0);
CHECK_OFFSET(TAPSTRUCTtag, tapnum, 0x4);
CHECK_OFFSET(TAPSTRUCTtag, gettable, 0x8);
CHECK_OFFSET(TAPSTRUCTtag, getptr, 0xC);
CHECK_SIZE(TAPSTRUCTtag, 0x10);

CHECK_OFFSET(STREAMHEADERtag, id, 0x000);
CHECK_OFFSET(STREAMHEADERtag, mutex, 0x004);
CHECK_OFFSET(STREAMHEADERtag, request, 0x014);
CHECK_OFFSET(STREAMHEADERtag, requests, 0x018);
CHECK_OFFSET(STREAMHEADERtag, filter, 0x01C);
CHECK_OFFSET(STREAMHEADERtag, filters, 0x020);
CHECK_OFFSET(STREAMHEADERtag, tap, 0x024);
CHECK_OFFSET(STREAMHEADERtag, taps, 0x028);
CHECK_OFFSET(STREAMHEADERtag, actualbufferstart, 0x02C);
CHECK_OFFSET(STREAMHEADERtag, bufferstart, 0x030);
CHECK_OFFSET(STREAMHEADERtag, bufferend, 0x034);
CHECK_OFFSET(STREAMHEADERtag, state, 0x038);
CHECK_OFFSET(STREAMHEADERtag, prioritylow, 0x03C);
CHECK_OFFSET(STREAMHEADERtag, priorityhigh, 0x040);
CHECK_OFFSET(STREAMHEADERtag, greedylevel, 0x044);
CHECK_OFFSET(STREAMHEADERtag, greedystate, 0x048);
CHECK_OFFSET(STREAMHEADERtag, bufferusage, 0x04C);
CHECK_OFFSET(STREAMHEADERtag, datastart, 0x050);
CHECK_OFFSET(STREAMHEADERtag, datatail, 0x054);
CHECK_OFFSET(STREAMHEADERtag, dataend, 0x058);
CHECK_OFFSET(STREAMHEADERtag, firstreq, 0x05C);
CHECK_OFFSET(STREAMHEADERtag, curreq, 0x060);
CHECK_OFFSET(STREAMHEADERtag, lastreq, 0x064);
CHECK_OFFSET(STREAMHEADERtag, freereq, 0x068);
CHECK_OFFSET(STREAMHEADERtag, fname, 0x06C);
CHECK_OFFSET(STREAMHEADERtag, unk_16B, 0x16B);
CHECK_OFFSET(STREAMHEADERtag, fhandle, 0x16C);
CHECK_OFFSET(STREAMHEADERtag, foffset, 0x170);
CHECK_OFFSET(STREAMHEADERtag, fop, 0x174);
CHECK_OFFSET(STREAMHEADERtag, readsize, 0x178);
CHECK_OFFSET(STREAMHEADERtag, readblocksize, 0x17C);
CHECK_SIZE(STREAMHEADERtag, 0x180);

#endif /* EALIB_STREAM_H */
