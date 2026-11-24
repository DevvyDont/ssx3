#ifndef MEM_MEMSTD_H
#define MEM_MEMSTD_H

// EA's standard memory manager (MEM_* / MEMCLASS_*): a memory class is one heap region whose blocks carry a MEMBLOCK
// header; free blocks are FREEBLOCKs on the class's free list (the list head freehead lives in the MEMCLASS itself).
// MEMCLASS_create(id, name, mem, size, defaultalign, align, infosize, sentinel, nameblocks, mutex) lays a region out as
// a bottom block at mem (whose 0x50-byte data area is the MEMCLASS, at mem + 0x10), one free block and a top block,
// and records the class in memclass[id & 0x3F]. EA source: SSX On Tour's mem/all_in_one_mem.cpp (MEMCLASS_create,
// MEM_initblock, ...). Retail code: unit mem/memstd (MEMCLASS_create .. MEM_*; its views sMemNode_251718 /
// sMemClass_251718 are these structs).
//
// Names, member order and offsets: [O] On Tour's struct MEMBLOCK (0x10), FREEBLOCK (0x18), MEMCLASS (0x50); retail
// matches them (On Tour's bool hasmutex is a word in retail). FLAT C structs. Tags: [O] SSX On Tour's DWARF; [R]
// retail MEMCLASS_create; [U] retail use.

#include "layout.h"
#include "ealib/stream.h"   // MUTEX (REAL's opaque mutex, 0x10)

#ifndef __cplusplus
typedef struct MEMBLOCK MEMBLOCK;
typedef struct FREEBLOCK FREEBLOCK;
typedef struct MEMCLASS MEMCLASS;
#endif

// MEMBLOCK (0x10 [O]): the header in front of every block (MEM_initblock(MEMBLOCK* memblk, name, datasize, infosize,
// memtype, prev, next) [O]).
struct MEMBLOCK {
    unsigned short signature;       // 0x0 [O]; [R] MEMCLASS_create stores 16979 in the bottom / top / free blocks
    unsigned short type;            // 0x2 [O] (memtype flags: 0x8000 bottom, 0x8100 top)
    int size;                       // 0x4 [O]
    struct MEMBLOCK* next;          // 0x8 [O]
    struct MEMBLOCK* prev;          // 0xC [O]
};

CHECK_OFFSET(MEMBLOCK, signature, 0x0);
CHECK_OFFSET(MEMBLOCK, type, 0x2);
CHECK_OFFSET(MEMBLOCK, size, 0x4);
CHECK_OFFSET(MEMBLOCK, next, 0x8);
CHECK_OFFSET(MEMBLOCK, prev, 0xC);
CHECK_SIZE(MEMBLOCK, 0x10);

// FREEBLOCK (0x18 [O]): a free block - a MEMBLOCK plus the free-list links.
struct FREEBLOCK {
    unsigned short signature;       // 0x00 [O]; [R] MEMCLASS_create: freehead's 16979
    unsigned short type;            // 0x02 [O]
    int size;                       // 0x04 [O]; [R] MEMCLASS_create: freehead's 0x7FFFFFFF
    struct MEMBLOCK* next;          // 0x08 [O]
    struct MEMBLOCK* prev;          // 0x0C [O]
    struct FREEBLOCK* nextfree;     // 0x10 [O]; [R] MEMCLASS_create links freehead to itself
    struct FREEBLOCK* prevfree;     // 0x14 [O]; [R] as nextfree
};

CHECK_OFFSET(FREEBLOCK, signature, 0x00);
CHECK_OFFSET(FREEBLOCK, type, 0x02);
CHECK_OFFSET(FREEBLOCK, size, 0x04);
CHECK_OFFSET(FREEBLOCK, next, 0x08);
CHECK_OFFSET(FREEBLOCK, prev, 0x0C);
CHECK_OFFSET(FREEBLOCK, nextfree, 0x10);
CHECK_OFFSET(FREEBLOCK, prevfree, 0x14);
CHECK_SIZE(FREEBLOCK, 0x18);

// MEMCLASS (0x50 [O]; [R] MEMCLASS_create clears 80 bytes at mem + 0x10 and fills every member below).
struct MEMCLASS {
    char name[8];                   // 0x00 [O]; [R] strcpy(name)
    struct MEMBLOCK* bottomblock;   // 0x08 [O]; [R] = mem (the bottom block); [U] its signature is set
    struct MEMBLOCK* topblock;      // 0x0C [O]; [R] = mem + size - (infosize + 0x30) (the top block); [U] signature
    struct FREEBLOCK freehead;      // 0x10 [O]; [R] signature 16979, size 0x7FFFFFFF, nextfree / prevfree = itself
    int defaultalign;               // 0x28 [O]; [R] MEMCLASS_create's defaultalign argument
    int align;                      // 0x2C [O]; [R] the align argument
    int infosize;                   // 0x30 [O]; [R] the infosize argument (extra header bytes per block)
    int flags;                      // 0x34 [O]; [R] the class number | 0x2000 (sentinel) | 0x1000 (name blocks)
    int hasmutex;                   // 0x38 [O] (bool; a word in retail); [R] 0, then 1 when the mutex argument is set;
                                    //      [U] MEMCLASS_remove destroys the mutex only when set
    MUTEX mutex;                    // 0x3C [O]; [R] MUTEX_create(&mutex); [U] MEMCLASS_remove MUTEX_destroy()s it
    void* freeadr;                  // 0x4C [O]; no retail access seen
};

CHECK_OFFSET(MEMCLASS, name, 0x00);
CHECK_OFFSET(MEMCLASS, bottomblock, 0x08);
CHECK_OFFSET(MEMCLASS, topblock, 0x0C);
CHECK_OFFSET(MEMCLASS, freehead, 0x10);
CHECK_OFFSET(MEMCLASS, defaultalign, 0x28);
CHECK_OFFSET(MEMCLASS, align, 0x2C);
CHECK_OFFSET(MEMCLASS, infosize, 0x30);
CHECK_OFFSET(MEMCLASS, flags, 0x34);
CHECK_OFFSET(MEMCLASS, hasmutex, 0x38);
CHECK_OFFSET(MEMCLASS, mutex, 0x3C);
CHECK_OFFSET(MEMCLASS, freeadr, 0x4C);
CHECK_SIZE(MEMCLASS, 0x50);

#endif /* MEM_MEMSTD_H */
