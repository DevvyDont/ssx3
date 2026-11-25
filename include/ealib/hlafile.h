#ifndef EALIB_HLAFILE_H
#define EALIB_HLAFILE_H

// REQUESTSTRUCTtag (hlafile.cpp's): one request of REAL core's asynchronous file loader (ASYNCFILE_load / loadat /
// setcallback / cancel / release and the size / load callbacks). EA source: realcore/source/file/cmn/hlafile.cpp (SSX
// On Tour: realcore 1.06.00; loadfilesizecallback(void* userdata) keeps a local `REQUESTSTRUCTtag* req`, and its DWARF
// has this 0x30 struct). Retail code: unit ealib/seg_2DCAE8 (ASYNCFILE_*, finishrequest, releaserequest,
// locaterequest, the FILESYS callbacks).
//
// NAME CLASH: stream.cpp has its own 0x124 REQUESTSTRUCTtag, and both modules sit in the same C unit
// (ealib/seg_2DCAE8.c), so ealib/stream.h gives that one the C name STREAMREQUESTtag; the name here is EA's.
//
// Names, member order and offsets: [O] On Tour struct REQUESTSTRUCTtag (0x30) and REQUESTQUEUE (0x8, queueadd(
// REQUESTQUEUE* queue, REQUESTSTRUCTtag* req)); [U] retail use as noted.
// C structs. Tags: [O] SSX On Tour's DWARF; [U] retail use.

#include "layout.h"

#ifndef __cplusplus
typedef struct REQUESTSTRUCTtag REQUESTSTRUCTtag;
typedef struct REQUESTQUEUE REQUESTQUEUE;
#endif

struct REQUESTSTRUCTtag {
    int id;                             // 0x00 [O]; [U] finishrequest passes it to the callback; locaterequest
                                        //      matches it
    struct REQUESTSTRUCTtag* nextreq;   // 0x04 [O] (the request list link)
    int bytesread;                      // 0x08 [O]
    int releaseflag;                    // 0x0C [O]; [U] finishrequest: a cancelled request is freed now unless set
    int cancelflag;                     // 0x10 [O]; [U] finishrequest skips the callback when set
    char* loadaddr;                     // 0x14 [O]; [U] the load address
    void (*callback)(int id);           // 0x18 [O]; [U] ASYNCFILE_setcallback stores it, finishrequest calls it with the
                                        //      request id
    volatile int fop;                   // 0x1C [O] (volatile); [U] the pending file operation, 0 when done
                                        //      (finishrequest, ASYNCFILE_setcallback)
    int fhandle;                        // 0x20 [O]; [U] the open file
    int foffset;                        // 0x24 [O]; [U] the read offset
    int readsize;                       // 0x28 [O]; [U] the read size
    char* destaddr;                     // 0x2C [O]; [U] the destination
};

CHECK_OFFSET(REQUESTSTRUCTtag, id, 0x00);
CHECK_OFFSET(REQUESTSTRUCTtag, nextreq, 0x04);
CHECK_OFFSET(REQUESTSTRUCTtag, bytesread, 0x08);
CHECK_OFFSET(REQUESTSTRUCTtag, releaseflag, 0x0C);
CHECK_OFFSET(REQUESTSTRUCTtag, cancelflag, 0x10);
CHECK_OFFSET(REQUESTSTRUCTtag, loadaddr, 0x14);
CHECK_OFFSET(REQUESTSTRUCTtag, callback, 0x18);
CHECK_OFFSET(REQUESTSTRUCTtag, fop, 0x1C);
CHECK_OFFSET(REQUESTSTRUCTtag, fhandle, 0x20);
CHECK_OFFSET(REQUESTSTRUCTtag, foffset, 0x24);
CHECK_OFFSET(REQUESTSTRUCTtag, readsize, 0x28);
CHECK_OFFSET(REQUESTSTRUCTtag, destaddr, 0x2C);
CHECK_SIZE(REQUESTSTRUCTtag, 0x30);

// A request queue (0x8 [O]): the loader's free / pending request lists. [U] queueadd appends at tail (linking through
// nextreq), queuefetch pops head.
struct REQUESTQUEUE {
    struct REQUESTSTRUCTtag* head;      // 0x0 [O]; [U] queuefetch pops it; queueadd sets it on an empty queue
    struct REQUESTSTRUCTtag* tail;      // 0x4 [O]; [U] queueadd links after it and moves it
};

CHECK_OFFSET(REQUESTQUEUE, head, 0x0);
CHECK_OFFSET(REQUESTQUEUE, tail, 0x4);
CHECK_SIZE(REQUESTQUEUE, 0x8);

#endif /* EALIB_HLAFILE_H */
