#ifndef EALIB_THREADS_H
#define EALIB_THREADS_H

// REAL core's thread and mutex records, PS2 versions: THREAD (threads.cpp: THREAD_createparam .. THREAD_waitexit) and
// the record behind the opaque MUTEX handle (mutex2.cpp: MUTEX_create / _lock / _unlock / _destroy; MUTEX itself is in
// ealib/stream.h). EA source: realcore/source/system/<platform>/threads.cpp and system/cmn/mutex2.cpp (SSX On Tour:
// realcore 1.06.00, psp). Retail code: unit ealib/seg_2DCAE8.
//
// On Tour keeps both handles opaque (THREAD {int reserved[2]}, MUTEX {int reserved[4]}) and works through platform
// records: THREAD_createparam keeps `_PSPTHREAD* pspthread` (0x8: signature, thId), the MUTEX functions keep
// `PSPMUTEX* m` (0x10: signature, lockthreadid, lockcount, mutex). Retail's PS2 records have the same shapes.
// C structs. Tags: [O] SSX On Tour's DWARF; [U] retail use.

#include "layout.h"

#ifndef __cplusplus
typedef struct THREAD THREAD;
typedef struct PS2MUTEX PS2MUTEX;
#endif

// THREAD (0x8): REAL's thread record ([O] On Tour's THREAD_testexit(THREAD* thread); its DWARF keeps the struct opaque,
// `int reserved[2]`). [U] THREAD_createparam stores the kernel thread id (CreateThread) at 0x4 and, once the thread
// exists, the tag 0x50533254 ('PS2T') at 0x0; THREAD_destroy, THREAD_iscurrent and THREAD_getpriority read the id at
// 0x4. Member names ours (On Tour's _PSPTHREAD: signature, thId).
struct THREAD {
    int tag;                        // 0x0 [U] 0x50533254 once created
    int threadId;                   // 0x4 [U] the kernel thread id
};

CHECK_OFFSET(THREAD, tag, 0x0);
CHECK_OFFSET(THREAD, threadId, 0x4);
CHECK_SIZE(THREAD, 0x8);

// PS2MUTEX (0x10; name ours, after On Tour's PSPMUTEX, whose members sit at the same offsets with the same uses): the
// recursive mutex behind a MUTEX handle (the MUTEX functions take the handle as void*).
struct PS2MUTEX {
    unsigned int signature;         // 0x0 [O]; retail never reads or writes it
    int lockthreadid;               // 0x4 [O]; [U] MUTEX_lock: the owning thread (GetThreadId); MUTEX_create /
                                    //      MUTEX_unlock clear it
    int lockcount;                  // 0x8 [O]; [U] the recursion depth (MUTEX_lock adds 1, MUTEX_unlock takes 1)
    int mutex;                      // 0xC [O]; [U] the kernel semaphore (CreateSema / WaitSema / SignalSema /
                                    //      DeleteSema)
};

CHECK_OFFSET(PS2MUTEX, signature, 0x0);
CHECK_OFFSET(PS2MUTEX, lockthreadid, 0x4);
CHECK_OFFSET(PS2MUTEX, lockcount, 0x8);
CHECK_OFFSET(PS2MUTEX, mutex, 0xC);
CHECK_SIZE(PS2MUTEX, 0x10);

#endif /* EALIB_THREADS_H */
