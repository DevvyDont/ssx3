#include "vtable.h"
#include "common.h"
#include "movie/videodriver.h"
#include "movie/movieplayer.h"
#include "render/ps2graphicsman.h"
#include "online/commchannel.h"

//100%
INCLUDE_ASM("mem/memstd", MEMCLASS_create);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
// PORT: debugmenu defines MEM_initblock with a short flags parameter; this caller passes the int unconverted.
extern "C" void* MEM_initblock(void* self, const char* name, int size, int hdr, int flags, void* prev, void* next);
extern "C" void* MEM_fill(void* dst, int value, int size);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int MUTEX_create(void* sema);
extern char str_pct_s_LOW[];
extern char str_pct_s_HIGH[];
extern void* D_004D2438[];

struct sMemNode_251718 {
    unsigned short tag;
    unsigned short flags;
    int size;
    char* end;
    void* fC;
    sMemNode_251718* next;
    sMemNode_251718* prev;
};

struct sMemClass_251718 {
    char name[8];
    sMemNode_251718* first;     // 0x8
    sMemNode_251718* last;      // 0xC
    sMemNode_251718 head;       // 0x10
    int f28;                    // 0x28
    int align;                  // 0x2C
    int hdr;                    // 0x30
    int flags;                  // 0x34
    int locked;                 // 0x38
    int sema[5];                // 0x3C
};

/**
 * Creates a memory class (heap) over a block of memory and registers it under an id.
 *
 * Builds the "<name> LOW" sentinel block at the start, one free block covering the aligned middle, and the
 * "<name> HIGH" sentinel at the end, sets up the class record (+0x10) with its free list, stores it in the
 * class table (D_004D2438, id & 63) and optionally creates its mutex.
 *
 * @param id     class id; its low 6 bits index the class table, and it also seeds the block flags
 * @param name   class name (copied into the record and used in the sentinel names)
 * @param mem    start of the memory block
 * @param size   size of the block in bytes
 * @param sizeAlign    stored at class +0x28, which _MEM_free uses to round allocation sizes (MEM_initadr: 0x40)
 * @param align  alignment of the first free block's data (MEM_initadr: 0x80)
 * @param hdr    extra header bytes per block
 * @param endMarker     nonzero sets block flag 0x2000, which the block check uses to expect a "BEND" end marker
 * @param nameBlocks     nonzero sets block flag 0x1000 (meaning unknown)
 * @param lock   nonzero creates a mutex and marks the class locked
 * @return the size of the initial free block
 */
extern "C" int MEMCLASS_create(int id, char* name, char* mem, int size, int sizeAlign, int align, int hdr, int endMarker, int nameBlocks, int lock)
{
    char buf[0x100];
    int flags = id;
    if (endMarker) {
        flags |= 0x2000;
    }
    if (nameBlocks) {
        flags |= 0x1000;
    }
    char* data = mem;
    data += hdr + 0x60;
    data += align + 47;
    data = (char*)((int)data & -align);
    char* end = mem + size;
    end -= hdr + 0x30;
    sprintf(buf, str_pct_s_LOW, name);
    sMemNode_251718* blk = (sMemNode_251718*)(data - 0x10);
    sMemClass_251718* cls = (sMemClass_251718*)(mem + 0x10);
    MEM_initblock(mem, buf, 80, hdr, flags | 0x8000, 0, blk);
    MEM_initblock(blk, 0, end - (char*)blk - 0x10, hdr, flags, mem, end);
    sprintf(buf, str_pct_s_HIGH, name);
    MEM_initblock(end, buf, 0, hdr, flags | 0x8100, blk, 0);
    D_004D2438[id & 0x3F] = cls;
    MEM_fill(cls, 0, 80);
    strcpy(cls->name, name);
    *(char**)(mem + 0x1C) = end;
    *(char**)(mem + 0x18) = mem;
    cls->first->tag = 16979;
    cls->last->tag = 16979;
    *(unsigned short*)(mem + 0x20) = 16979;
    *(int*)(mem + 0x24) = 0x7FFFFFFF;
    *(int*)(mem + 0x38) = sizeAlign;
    *(int*)(mem + 0x3C) = align;
    *(int*)(mem + 0x40) = hdr;
    *(int*)(mem + 0x44) = flags;
    sMemNode_251718* head = (sMemNode_251718*)(mem + 0x20);
    *(sMemNode_251718**)(mem + 0x30) = head;
    *(sMemNode_251718**)(mem + 0x34) = head;
    *(int*)(mem + 0x48) = 0;
    sMemNode_251718* nextNode = head;
    sMemNode_251718* prevNode = nextNode;
    int sz = *(char**)(data - 8) - (char*)blk;
    if (*(char**)(mem + 0x30) + (*(char**)(mem + 0x34) - *(char**)(mem + 0x30)) / 2 < (char*)blk) {
        do
            prevNode = prevNode->prev;
        while (blk < prevNode);
        nextNode = prevNode->next;
    } else {
        do
            nextNode = nextNode->next;
        while (nextNode < blk);
        prevNode = nextNode->prev;
    }
    blk->size = sz;
    blk->next = nextNode;
    blk->prev = prevNode;
    prevNode->next = blk;
    nextNode->prev = blk;
    blk->tag = 16966;
    blk->flags |= 0x4000;
    if (lock) {
        MUTEX_create(cls->sema);
        cls->locked = 1;
    }
    return blk->size;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", MEMCLASS_remove);
#ifdef SKIP_ASM
extern "C" void MUTEX_destroy(void*);

/**
 * Removes memory class @p id from the class table: deletes its mutex (MUTEX_destroy) when the class is locked, zeroes
 * the 80-byte class record and clears its slot in D_004D2438 (id & 63).
 *
 * The blocks inside the class are not freed.
 *
 * @param id  memory class id
 * @return 1 when the class existed, else 0
 */
extern "C" int MEMCLASS_remove(int id)
{
    void** slot = &D_004D2438[id & 0x3F];
    sMemClass_251718* cls = (sMemClass_251718*)*slot;
    int removed = 0;
    if (cls != 0) {
        if (cls->locked != 0) {
            MUTEX_destroy(cls->sema);
        }
        MEM_fill(cls, 0, 80);
        *slot = 0;
        removed = 1;
    }
    return removed;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEMCLASS_purge);
#ifdef SKIP_ASM
extern "C" void* MEM_free(void* self);
struct sMemNode_251A08 {
    unsigned short tag;
    unsigned short flags;
    int size;
    sMemNode_251A08* next;
};

struct sMemClass_251A08 {
    char pad0[0x8];
    sMemNode_251A08* first;     // 0x8
};

/**
 * Frees every block of memory class @p id: walks its block list from the first block and calls MEM_free on the data
 * (block + 0x10) of each block not flagged 0xC000 (free or sentinel), stopping when a free fails.
 *
 * The walk reads each block's next pointer after freeing it.
 *
 * @param id  memory class id
 * @return 1, or the failing MEM_free's result
 */
extern "C" void* MEMCLASS_purge(int id)
{
    sMemClass_251A08* cls = (sMemClass_251A08*)D_004D2438[id & 0x3F];
    void* freeResult = (void*)1;
    for (sMemNode_251A08* n = cls->first; n != 0 && freeResult != 0; n = n->next) {
        if (!(n->flags & 0xC000)) {
            freeResult = MEM_free((char*)n + 0x10);
        }
    }
    return freeResult;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEMCLASS_getfree);
#ifdef SKIP_ASM
/**
 * Returns the first unused memory class id: the first empty slot of D_004D2438 from 2 on (0x40 when all are used).
 *
 * @return a free class id, or 64
 */
extern "C" int MEMCLASS_getfree(void)
{
    int i = 2;
    while (i < 0x40 && D_004D2438[i] != 0) {
        i++;
    }
    return i;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_largestunused);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: an empty ASSERT macro (`ASSERT_1AC0(best != 0)`, `do { } while (0)`) before the return, perhaps EA's compiled-out assert but unconfirmed; its loop notes give retail's extra no-op before `jr ra`; plain form 97.22%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern int D_004A2E78_1AC0 __asm__("D_004A2E78");
// Release-build assert (compiled out); EA code asserts its pointers
#define ASSERT_1AC0(cond) do { } while (0)
/**
 * Returns the size of the largest free block of memory class @p id (0 means the default class D_004A2E78).
 *
 * Walks the free list from the class head, keeping the block with the largest size - 16, until it meets the sentinel
 * (tag 0x4253). Returns that block's size field, header included, or 0 when there is none.
 *
 * @param id  memory class id, or 0
 * @return the largest free block's size
 */
extern "C" int MEM_largestunused(int id)
{
    if (id == 0) {
        id = D_004A2E78_1AC0;
    }
    sMemClass_251718* cls = (sMemClass_251718*)D_004D2438[id & 0x3F];
    sMemNode_251718* n = cls->head.next;
    sMemNode_251718* best = 0;
    int largestData = 0;
    for (;;) {
        if (largestData < n->size - 0x10) {
            if (n->tag == 16979) {
                break;
            }
            best = n;
            largestData = best->size - 0x10;
        }
        n = n->next;
    }
    ASSERT_1AC0(best != 0);
    return best != 0 ? best->size : 0;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_totalunused);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: an empty ASSERT macro (`ASSERT_1B50(n != 0)`, `do { } while (0)`) in the loop body, perhaps EA's compiled-out assert but unconfirmed; its loop notes give retail's extra no-op before `jr ra`; plain form 95.83%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern void* D_004D2438[];

extern int D_004A2E78_1B50 __asm__("D_004A2E78");

// Release-build assert (compiled out); EA code asserts its pointers
#define ASSERT_1B50(cond) do { } while (0)
/**
 * Returns the total free memory of memory class @p id (0 means the default class D_004A2E78): the sum of size - 16 over
 * its free list up to the sentinel (tag 0x4253).
 *
 * @param id  memory class id, or 0
 * @return the free bytes
 */
extern "C" int MEM_totalunused(int id)
{
    if (id == 0) {
        id = D_004A2E78_1B50;
    }
    sMemClass_251718* cls = (sMemClass_251718*)D_004D2438[id & 0x3F];
    int total = 0;
    sMemNode_251718* n = cls->head.next;
    while (n->tag != 16979) {
        ASSERT_1B50(n != 0);
        total += n->size - 0x10;
        n = n->next;
    }
    return total;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_findcontaining);
#ifdef SKIP_ASM
struct sMemNode_251BB0 {
    unsigned short tag;
    unsigned short flags;
    int size;
    sMemNode_251BB0* next;
};

struct sMemClass_251BB0 {
    char pad0[0x8];
    sMemNode_251BB0* first;     // 0x8
};

/**
 * Finds the block that contains address @p addr: searches the block list of every memory class for a block whose data
 * (block + 0x10, size bytes) holds it.
 *
 * @param addr  any address
 * @return the block's data pointer, or 0 when none holds it or the block is free or a sentinel (flags 0xC000)
 */
extern "C" void* MEM_findcontaining(unsigned int addr)
{
    int i;
    for (i = 0; i < 0x40; i++) {
        sMemClass_251BB0* cls = (sMemClass_251BB0*)D_004D2438[i & 0x3F];
        if (cls != 0) {
            sMemNode_251BB0* n = cls->first;
            unsigned int data;
            while (1) {
                n = n->next;
                if (n == 0) break;
                data = (unsigned int)n + 0x10;
                if (addr >= data && addr < data + n->size) break;
            }
            if (n != 0 && !(n->flags & 0xC000)) {
                return (void*)data;
            }
        }
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_find);
#ifdef SKIP_ASM
struct sMemNode_251C38 {
    unsigned short tag;
    unsigned short flags;
    int size;
    sMemNode_251C38* next;
};

struct sMemClass_251C38 {
    char pad0[0x8];
    sMemNode_251C38* first;     // 0x8
};

int MEM_name(void* self);
extern "C" int strcasecmp(int, int);

/**
 * Finds a block by name: searches the block lists of every memory class for a block whose name (MEM_name) matches
 * @p name case-insensitively (strcasecmp).
 *
 * MEM_name returns 0 in this build (no block names), so the search never matches.
 *
 * @param name  the block name
 * @return the block's data pointer, or 0
 */
extern "C" void* MEM_find(int name)
{
    int i;
    for (i = 0; i < 0x40; i++) {
        sMemClass_251C38* cls = (sMemClass_251C38*)D_004D2438[i & 0x3F];
        if (cls != 0) {
            sMemNode_251C38* n = cls->first;
            void* data = (char*)n + 0x10;
            do {
                int blockName = MEM_name(data);
                if (blockName != 0 && strcasecmp(blockName, name) == 0) {
                    return data;
                }
                n = n->next;
                data = (char*)n + 0x10;
            } while (n != 0);
        }
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_initsize);
#ifdef SKIP_ASM
/**
 * Returns 0; a stub in this build (the first of the three stubs before MEM_initadr).
 *
 * @return 0
 */
extern "C" int MEM_initsize(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_init);
#ifdef SKIP_ASM
/**
 * Returns 0; a stub in this build (the second of the three stubs before MEM_initadr).
 *
 * @return 0
 */
extern "C" int MEM_init(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_restore);
#ifdef SKIP_ASM
/**
 * Does nothing (the last of the three stubs before MEM_initadr).
 */
extern "C" void MEM_restore(void) {

}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_initadr);
#ifdef SKIP_ASM
extern char str_RAM[];

/**
 * Creates the main "RAM" memory class over a block of memory.
 *
 * Calls MEMCLASS_create() for class 0 with that name and fixed settings (0x40, alignment 0x80, no header, lock
 * on).
 *
 * @param mem   start of the memory block
 * @param size  size of the block in bytes
 * @return MEMCLASS_create()'s result
 */
extern "C" int MEM_initadr(char* mem, int size)
{
    return MEMCLASS_create(0, str_RAM, mem, size, 0x40, 0x80, 0, 0, 0, 1);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", MEMCLASS_link);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
// PORT: debugmenu defines MEM_initblock with a short flags parameter; this caller passes the int unconverted.
extern "C" void* MEM_initblock(void* self, const char* name, int size, int hdr, int flags, void* prev, void* next);
extern char str_pct_s_LINK[];
extern char str_pct_s_HIGH[];

struct sMemNode_251D50 {
    unsigned short tag;
    unsigned short flags;
    int size;
    char* end;
    void* fC;
    sMemNode_251D50* next;
    sMemNode_251D50* prev;
};

struct sMemClass_251D50 {
    char pad0[0xC];
    char* last;                 // 0xC
    sMemNode_251D50 head;       // 0x10
    int f28;
    int align;                  // 0x2C
    int hdr;                    // 0x30
    int flags;                  // 0x34
};

extern void* D_004D2438[];

/**
 * Adds another block of memory to an existing memory class.
 *
 * Turns the class's old HIGH sentinel into a "<name> LINK" block pointing at the new memory, which gets a
 * LINK block of its own, one free block and a new HIGH sentinel. The free block is inserted into the class's
 * sorted free list (searched from whichever end is nearer).
 *
 * @param id    class id (low 6 bits index the class table)
 * @param mem   start of the memory to add
 * @param size  its size in bytes
 * @return always 1
 */
extern "C" int MEMCLASS_link(int id, char* mem, int size)
{
    sMemClass_251D50* cls = (sMemClass_251D50*)D_004D2438[id & 0x3F];
    char buf[0x100];
    int hdr = cls->hdr;
    int align = cls->align;
    char* end = mem + size;
    char* data = mem;
    data += hdr + 0x10;
    data += align + 47;
    data = (char*)((int)data & -align);
    end -= hdr + 0x30;
    int flags = cls->flags;
    sMemNode_251D50* blk = (sMemNode_251D50*)(data - 0x10);
    char* last = cls->last;
    sprintf(buf, str_pct_s_LINK, cls);
    MEM_initblock(last, buf, 0, hdr, flags | 0x8000, *(void**)(last + 0xC), mem);
    MEM_initblock(mem, buf, 0, hdr, flags | 0x8000, last, blk);
    MEM_initblock(blk, 0, end - (char*)blk - 0x10, hdr, flags, mem, end);
    sprintf(buf, str_pct_s_HIGH, cls);
    MEM_initblock(end, buf, 0, hdr, flags | 0x8100, blk, 0);
    cls->last = end;
    sMemNode_251D50* nextNode = &cls->head;
    sMemNode_251D50* prevNode = nextNode;
    int sz = *(char**)(data - 8) - (char*)blk;
    if ((char*)cls->head.next + ((char*)cls->head.prev - (char*)cls->head.next) / 2 < (char*)blk) {
        do
            prevNode = prevNode->prev;
        while (blk < prevNode);
        nextNode = prevNode->next;
    } else {
        do
            nextNode = nextNode->next;
        while (nextNode < blk);
        prevNode = nextNode->prev;
    }
    blk->size = sz;
    blk->next = nextNode;
    blk->prev = prevNode;
    prevNode->next = blk;
    nextNode->prev = blk;
    blk->tag = 16966;
    blk->flags |= 0x4000;
    return 1;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", DEBUG_trace);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
typedef char* DEBUG_trace_va_list;
#define DEBUG_trace_va_start(ap)                                       \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

extern "C" int vsprintf(char* dst, const char* fmt, char* ap);
extern "C" void PRINT_string(int, char*);

/**
 * Prints a printf-style debug message on print channel 2.
 *
 * Formats @p fmt into a 512-byte stack buffer with vsprintf (vsprintf) and passes it to PRINT_string.
 * MEM_printclass uses it as the line printer for MEM_printclassf.
 *
 * @param fmt  printf-style format, followed by its arguments
 */
extern "C" void DEBUG_trace(const char* fmt, ...)
{
    char buf[0x200];
    DEBUG_trace_va_list ap;
    DEBUG_trace_va_start(ap);
    vsprintf(buf, fmt, ap);
    PRINT_string(2, buf);
}
#endif

extern const char str_pct_12s_pct_24_24s_pct_08lx_pct_11lu_pct[];
extern "C" void MEM_printclassf(void* thing, const char* fmt, void (*cb)(const char*, ...));

//100%
INCLUDE_ASM("mem/memstd", MEM_printclass__FPv);
#ifdef SKIP_ASM
/**
 * Prints a report of a memory class's blocks to the debug output.
 *
 * Calls MEM_printclassf() with the row format "%-12s %-24.24s %08lx %11lu %11lu %04x %04x %3s %s" and
 * DEBUG_trace(), a printf that writes through PRINT_string(2, ...).
 *
 * @param thing  the memory class to report
 */
void MEM_printclass(void* thing)
{
    MEM_printclassf(thing, str_pct_12s_pct_24_24s_pct_08lx_pct_11lu_pct, DEBUG_trace);
}
#endif

extern void* D_004A2E78;

//99.25%
INCLUDE_ASM("mem/memstd", MEM_print__Fv);
#ifdef SKIP_ASM
/**
 * Prints a report of the default memory class (D_004A2E78) through MEM_printclass().
 */
void MEM_print()
{
    MEM_printclass(D_004A2E78);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", MEM_printclassf);
#ifdef SKIP_ASM
int MEM_name(void* self);
extern "C" int MEM_comment(void* p);
struct sMemBlock002522B0;
extern "C" int checksentinel(sMemBlock002522B0* blk);
extern "C" int MEM_crc16(void* p, int size, int seed);
extern char D_004800E0[];
extern char D_00480140[];
extern char D_00480198[];
extern char str_SPACE[];
extern char D_004801C0[];
extern char* D_004801E8[];
extern char D_004A2E58[];
extern char str_FREE[];
extern char str_unknown[];
extern char D_004A2E70[];

struct sMemNode_252010 {
    unsigned short tag;
    unsigned short flags;
    int size;
    char* next;
    void* fC;
};

struct sMemClass_252010 {
    char pad0[0x8];
    sMemNode_252010* first;     // 0x8
};

extern void* D_004D2438[];

/**
 * Prints a table of every block in a memory class through a printf-like callback.
 *
 * Prints a header, then one row per block: free blocks as "FREE" with "   SPACE " as the name, others as
 * "unknown" with the names from MEM_name/MEM_comment; each row has the address, size, span, flags, a
 * CRC (MEM_crc16, seed 0xFBEA; 0 for sentinels) and the block check's status text. Ends with the total
 * free memory, or prints "Not initialized" if the class does not exist.
 *
 * @param thing  memory class id (masked with 63), passed as a pointer
 * @param fmt    row format string handed to the callback
 * @param cb     printf-like output function
 */
// PORT: thing is a memory class id (masked with 0x3F) passed through a void* parameter.
extern "C" void MEM_printclassf(void* thing, const char* fmt, void (*cb)(const char*, ...))
{
    int total = 0;
    int crc = 0;
    cb(D_004800E0);
    cb(D_00480140);
    sMemClass_252010* cls = (sMemClass_252010*)D_004D2438[(int)thing & 0x3F];
    if (cls == 0) {
        cb(D_00480198);
        return;
    }
    for (sMemNode_252010* blk = cls->first; blk != 0; blk = (sMemNode_252010*)blk->next) {
        char* data = (char*)blk + 0x10;
        char* blockName = (char*)MEM_name(data);
        if (blockName == 0)
            blockName = D_004A2E58;
        char* comment = (char*)MEM_comment(data);
        if (comment == 0)
            comment = D_004A2E58;
        if (blk->flags & 0x4000) {
            int sz = blk->size;
            if (sz > 0x10 && !(blk->flags & 0x8000))
                crc = MEM_crc16((char*)blk + 0x18, sz - 0x18, 64490);
            total += sz;
            int st = checksentinel((sMemBlock002522B0*)blk);
            cb(fmt, str_FREE, str_SPACE, data, sz, blk->next - (char*)blk, blk->flags, crc, D_004801E8[st], D_004A2E58);
        } else {
            int sz = blk->size;
            crc = 0;
            if (sz != 0 && !(blk->flags & 0x8000))
                crc = MEM_crc16(data, sz, 64490);
            int st = checksentinel((sMemBlock002522B0*)blk);
            cb(fmt, str_unknown, blockName, data, blk->size, blk->next - (char*)blk, blk->flags, crc, D_004801E8[st], comment);
        }
        cb(D_004A2E70);
    }
    cb(D_004801C0, total, total);
    cb(D_00480140);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", checkfree);
#ifdef SKIP_ASM
extern int D_004A2E74;
extern "C" int MEMSYS_validaddress(void* p, int size, int flags);

/**
 * Checks a free-list link: it must point at a readable 0x18-byte block (MEMSYS_validaddress) tagged as free (0x4246) or as a
 * sentinel (0x4253).
 *
 * On failure sets the memory error code D_004A2E74 to 1.
 *
 * @return 1 when the link is valid, else 0
 */
extern "C" int checkfree(void* self)
{
    int ok = 0;
    if (MEMSYS_validaddress(self, 0x18, 0) != 0) {
        unsigned short blockTag = *(unsigned short*)self;
        if (blockTag == 16966 || blockTag == 16979) {
            ok = 1;
        } else {
            D_004A2E74 = 1;
        }
    } else {
        D_004A2E74 = 1;
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", checksentinel);
#ifdef SKIP_ASM
struct sMemBlock002522B0 {
    unsigned short tag;
    unsigned short flags;
    int size;
    int f8;
    int fC;
    void* a;
    void* b;
};

static inline unsigned int readBE32_002522B0(unsigned char* p)
{
    return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}

/**
 * Checks one heap block's integrity by its tag and records the failure kind in D_004A2E74.
 *
 * A sentinel (0x4253) needs flag 0x8000 (else error 2); a free block (0x4246) with flag 0x4000 needs valid next and
 * previous links (checkfree); an allocated block (0x424D) with flag 0x2000 needs the big-endian end marker "BEND"
 * just past its data (else error 3); any other tag is error 4. MEM_printclassf prints its result.
 *
 * @param blk  the block header
 * @return 1 when the block is consistent, else 0
 */
// PORT: the block-end address is computed as size + (int)blk (pointer held in int) to get the target's addu operand order.
extern "C" int checksentinel(sMemBlock002522B0* blk)
{
    int ok = 0;
    unsigned short tag = blk->tag;
    if (tag == 16979) {
        if (blk->flags & 0x8000) {
            ok = 1;
        } else {
            D_004A2E74 = 2;
        }
    } else if (tag == 16966) {
        if (blk->flags & 0x4000) {
            ok = checkfree(blk->a);
            if (ok) {
                ok = checkfree(blk->b);
            }
        }
    } else if (tag == 16973) {
        if (!(blk->flags & 0x2000) || readBE32_002522B0((unsigned char*)(blk->size + (int)blk) + 0x10) == 0x42454E44) {
            ok = 1;
        } else {
            D_004A2E74 = 3;
        }
    } else {
        D_004A2E74 = 4;
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", MEM_validate);
#ifdef SKIP_ASM
/**
 * Returns 1: the heap validation is a stub in this build.
 *
 * @return 1
 */
extern "C" int MEM_validate(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", MEM_free);
#ifdef SKIP_ASM
void cMemMan_free(void*);

/**
 * Frees a block through the memory manager, ignoring a null pointer.
 *
 * @param self  block to free, or 0
 * @return always 1
 */
extern "C" void* MEM_free(void* self)
{
    if (self != 0) {
        cMemMan_free(self);
    }
    return (void*)1;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", _MEM_free);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern "C" int MEM_tailsize(int unused, int id);

struct sMemNode_2523D0 {
    unsigned short tag;         // 0x0
    unsigned short flags;       // 0x2
    int size;                   // 0x4
    sMemNode_2523D0* end;       // 0x8 (next block in memory)
    sMemNode_2523D0* back;      // 0xC (previous block in memory)
    sMemNode_2523D0* next;      // 0x10 (free list)
    sMemNode_2523D0* prev;      // 0x14 (free list)
};

struct sMemClass_2523D0 {
    char pad0[0x10];
    sMemNode_2523D0 head;       // 0x10
    int align;                  // 0x28
    char pad2C[0xC];
    int locked;                 // 0x38
    char mutex[4];              // 0x3C
};

extern void* D_004D2438[];

static inline void Unlink_2523D0(sMemNode_2523D0* b)
{
    sMemNode_2523D0* p = b->prev;
    sMemNode_2523D0* n = b->next;
    p->next = n;
    n->prev = p;
}

/**
 * Frees a block of the EA heap, merging it with free neighbours, and puts it on the class's sorted free list.
 *
 * The class comes from the block's type word (data - 0xE, & 63). Under the class mutex when locked, it absorbs a free
 * previous block and a free next block (unlinking them and retagging them 0x424F), then re-places the start right after
 * the previous block's used size plus its tail (MEM_tailsize), rounded up to the class's size alignment (+0x28), and
 * inserts the block into the address-sorted free list, searching from whichever end is nearer. The block becomes tag
 * 0x4246 with flag 0x4000. A null @p mem does nothing.
 *
 * @param mem  the data pointer returned by the allocator
 * @return 1
 */
// PORT: pointer arithmetic done in int (see MEM_comment).
extern "C" int _MEM_free(void* mem)
{
    sMemClass_2523D0* cls;
    sMemNode_2523D0* blk;
    sMemNode_2523D0* back;
    sMemNode_2523D0* fwd;
    sMemNode_2523D0* split;
    sMemNode_2523D0* prevFree;
    sMemNode_2523D0* nextFree;

    int size;
    int sz;

    if (mem != 0) {
        cls = (sMemClass_2523D0*)D_004D2438[*(unsigned short*)((int)mem - 0xE) & 0x3F];
        blk = (sMemNode_2523D0*)((int)mem - 0x10);
        if (cls->locked) {
            MUTEX_lock(cls->mutex);
        }
        back = *(sMemNode_2523D0**)((int)mem - 4);
        fwd = *(sMemNode_2523D0**)((int)mem - 8);
        if (back->flags & 0x4000) {
            Unlink_2523D0(back);
            back->tag = 16975;
            back->flags &= ~0x4000;
            blk = back;
            back = blk->back;
            blk->end = fwd;
            back->end = blk;
            fwd->back = blk;
        }
        if (fwd->flags & 0x4000) {
            Unlink_2523D0(fwd);
            fwd->tag = 16975;
            fwd->flags &= ~0x4000;
            fwd = fwd->end;
            blk->back = back;
            blk->end = fwd;
            fwd->back = blk;
        }
        size = back->size + MEM_tailsize(MEM_name(&back->next), back->flags);
        if (size <= 7) {
            size = 8;
        }
        split = (sMemNode_2523D0*)((char*)back + ((size + cls->align + 0xF) & -cls->align));
        if (blk != split) {
            split->flags = blk->flags;
            blk = split;
            blk->end = fwd;
            blk->back = back;
            back->end = blk;
            fwd->back = blk;
        }
        nextFree = &cls->head;
        prevFree = nextFree;
        sz = (char*)blk->end - (char*)blk;
        if ((char*)cls->head.next + ((char*)cls->head.prev - (char*)cls->head.next) / 2 < (char*)blk) {
            do
                prevFree = prevFree->prev;
            while (blk < prevFree);
            nextFree = prevFree->next;
        } else {
            do
                nextFree = nextFree->next;
            while (nextFree < blk);
            prevFree = nextFree->prev;
        }
        blk->size = sz;
        blk->next = nextFree;
        blk->prev = prevFree;
        prevFree->next = blk;
        nextFree->prev = blk;
        blk->tag = 16966;
        blk->flags |= 0x4000;
        if (cls->locked) {
            MUTEX_unlock(cls->mutex);
        }
    }
    return 1;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", blockSize);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern "C" int cMemMan_internalBlockSize(int);
extern void* cMemMan_m_mutex[16];

/**
 * Returns the size of a memory-manager block: cMemMan_internalBlockSize under the cMemMan_m_mutex lock.
 *
 * @param memBlock  the block's data pointer
 * @return the block size
 */
extern "C" int blockSize(int memBlock) {
    int result;

    MUTEX_lock(&cMemMan_m_mutex);
    result = cMemMan_internalBlockSize(memBlock);
    MUTEX_unlock(&cMemMan_m_mutex);
    return result;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", _MEM_size);
#ifdef SKIP_ASM
/**
 * Returns an EA heap block's data size, the int stored at @p allocation - 0xC.
 *
 * @param allocation  the block's data pointer
 * @return the size in bytes
 */
extern "C" int _MEM_size(void *allocation) {
    return (*(int *)((char*)(allocation) + (-0xC)));
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_type);
#ifdef SKIP_ASM
/**
 * Returns an EA heap block's type word (class id and flags), the halfword at @p allocation - 0xE.
 *
 * @param allocation  the block's data pointer
 * @return the type word
 */
extern "C" unsigned short MEM_type(void *allocation) {
    return (*(unsigned short *)((char*)(allocation) + (-0xE)));
}
#endif

//100%
INCLUDE_ASM("mem/memstd", MEM_name__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: block names are not stored in this build.
 *
 * MEM_find, MEM_printclassf and _MEM_free ask it for a block's name.
 *
 * @return 0
 */
int MEM_name(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", MEM_comment);
#ifdef SKIP_ASM
struct sMemUnalignedInt {
    int value;
} __attribute__((packed));

/**
 * Returns an EA heap block's comment word: when its type word (@p allocation - 0xE) has flag 0x800, the unaligned int
 * stored 4 bytes past the end of its data (@p allocation + size + 4), else 0.
 *
 * MEM_printclassf prints it as the block comment.
 *
 * @param allocation  the block's data pointer
 * @return the stored value, or 0
 */
// PORT: pointer arithmetic done in int; a plain `(char*)p - 0xE` makes gcc 2.95
// emit the offset as unsigned 0xFFFFFFF2 (addiu -0x8000 / 0x7ff2 split).
extern "C" int MEM_comment(void* allocation)
{
    int trailerValue = 0;
    if (*(unsigned short*)((int)allocation - 0xE) & 0x800) {
        trailerValue = ((sMemUnalignedInt*)((char*)allocation + *(int*)((int)allocation - 0xC) + 4))->value;
    }
    return trailerValue;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", MEM_setcomment);
#ifdef SKIP_ASM
/**
 * Stores @p trailerValue in an EA heap block's comment slot (4 bytes past the end of its data), when its type word has
 * flag 0x800.
 *
 * @param allocation  the block's data pointer
 * @param trailerValue  the value to store
 * @return 1 when stored, else 0
 */
// PORT: pointer arithmetic done in int (see MEM_comment).
extern "C" int MEM_setcomment(void* allocation, int trailerValue)
{
    int written = 0;
    if (*(unsigned short*)((int)allocation - 0xE) & 0x800) {
        written = 1;
        ((sMemUnalignedInt*)((char*)allocation + *(int*)((int)allocation - 0xC) + 4))->value = trailerValue;
    }
    return written;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", resizeBlock);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern "C" void* cMemMan_internalResizeBlock(void* block, int size);
extern void* cMemMan_m_mutex[16];

/**
 * Resizes a memory-manager block: cMemMan_internalResizeBlock under the cMemMan_m_mutex lock.
 *
 * The sound bank monitor installs it as its resize hook (D_004A3790).
 *
 * @param block  the block's data pointer
 * @param size  the new size
 * @return the (possibly moved) block
 */
extern "C" void* resizeBlock(void* block, int size)
{
    void* newBlock;
    MUTEX_lock(cMemMan_m_mutex);
    newBlock = cMemMan_internalResizeBlock(block, size);
    MUTEX_unlock(cMemMan_m_mutex);
    return newBlock;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", _MEM_resize);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern "C" void* MEM_initblock(void* self, const char* name, int size, int hdr, int flags, void* prev, void* next);
extern "C" int MEM_tailsize(int unused, int id);
extern "C" void MEM_move(void* a, void* b);

struct sMemNode_252718 {
    unsigned short tag;         // 0x0
    unsigned short flags;       // 0x2
    int size;                   // 0x4
    sMemNode_252718* end;       // 0x8 (next block in memory)
    sMemNode_252718* back;      // 0xC (previous block in memory)
    sMemNode_252718* next;      // 0x10 (free list)
    sMemNode_252718* prev;      // 0x14 (free list)
};

struct sMemClass_252718 {
    char pad0[0x10];
    sMemNode_252718 head;       // 0x10
    int align;                  // 0x28
    char pad2C[0xC];
    int locked;                 // 0x38
    char mutex[4];              // 0x3C
};

extern void* D_004D2438[];

static inline void Unlink_252718(sMemNode_252718* b)
{
    sMemNode_252718* p = b->prev;
    sMemNode_252718* n = b->next;
    p->next = n;
    n->prev = p;
}

/**
 * Resizes an EA heap block in place, absorbing a free block after it and splitting off any large remainder.
 *
 * Under the class mutex when locked: a free next block is unlinked (tag 0x424F) and merged. The wanted size (8 at
 * least; -1 means 0x40000000, i.e. as much as possible) plus the class tail (MEM_tailsize) is rounded to the class
 * alignment and capped at the space up to the next block. MEM_move moves the tail data to the new end, the block size
 * is updated, and a remainder of more than 0x40 bytes becomes a new free block (MEM_initblock) inserted into the
 * address-sorted free list. The data never moves.
 *
 * @param mem  the block's data pointer
 * @param size  the new size
 * @return @p mem
 */
// PORT: pointer arithmetic done in int (see MEM_comment).
extern "C" void* _MEM_resize(void* mem, int size)
{
    int id = *(unsigned short*)((int)mem - 0xE);
    sMemClass_252718* cls = (sMemClass_252718*)D_004D2438[id & 0x3F];
    sMemNode_252718* blk = (sMemNode_252718*)((int)mem - 0x10);
    sMemNode_252718* fwd;
    sMemNode_252718* nb;
    sMemNode_252718* prevFree;
    sMemNode_252718* nextFree;
    int want;
    int avail;
    int hdr;
    int sz;

    if (cls->locked) {
        MUTEX_lock(cls->mutex);
    }
    fwd = *(sMemNode_252718**)((int)mem - 8);
    if (fwd->flags & 0x4000) {
        Unlink_252718(fwd);
        fwd->tag = 16975;
        fwd->flags &= ~0x4000;
        fwd = fwd->end;
        *(sMemNode_252718**)((int)mem - 8) = fwd;
        fwd->back = blk;
    }
    want = size;
    if (want < 8) {
        if (want == -1) {
            want = 0x40000000;
        } else if (want >= 0) {
            want = 8;
        }
    }
    hdr = MEM_tailsize(0, id);
    avail = ((char*)fwd - (char*)blk) - 0x10;
    want = (((want + hdr + 0x10) + (cls->align - 1)) & -cls->align) - 0x10;
    if (avail < want) {
        want = avail;
        size = avail - hdr;
    }
    MEM_move((char*)mem + size, (char*)mem + blk->size);
    blk->size = size;
    if (avail - want > 0x40) {
        nb = (sMemNode_252718*)((char*)mem + (want + 0x10));
        MEM_initblock(nb, 0, 0, 0, 0, blk, fwd);
        nextFree = &cls->head;
        prevFree = nextFree;
        sz = (char*)nb->end - (char*)nb;
        if ((char*)cls->head.next + ((char*)cls->head.prev - (char*)cls->head.next) / 2 < (char*)nb) {
            do
                prevFree = prevFree->prev;
            while (nb < prevFree);
            nextFree = prevFree->next;
        } else {
            do
                nextFree = nextFree->next;
            while (nextFree < nb);
            prevFree = nextFree->prev;
        }
        nb->size = sz;
        nb->next = nextFree;
        nb->prev = prevFree;
        prevFree->next = nb;
        nextFree->prev = nb;
        nb->tag = 16966;
        nb->flags |= 0x4000;
        fwd->back = nb;
        blk->end = nb;
    }
    if (cls->locked) {
        MUTEX_unlock(cls->mutex);
    }
    return mem;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_tailsize);
#ifdef SKIP_ASM
/**
 * Returns the per-block tail size of memory class @p id: the class record's int at +0x30 (MEMCLASS_create's hdr).
 *
 * @param ignoredName  ignored (callers pass 0 or a block name)
 * @param id  memory class id (masked with 63)
 * @return the tail bytes each block of the class reserves
 */
extern "C" int MEM_tailsize(int ignoredName, int id)
{
    return *(int*)((char*)D_004D2438[id & 0x3F] + 0x30);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", MEM_allocaligna);
#ifdef SKIP_ASM
extern "C" int memlog2(int);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

// PORT: the unit declares MEM_allocaligna as `void* (void*)`; existing callers bind the
// 6-argument form below by asm label (the 6th argument is unused).
void* MEM_allocaligna_6(void*, int, int, int, int, int) __asm__("MEM_allocaligna");

/**
 * Allocates a named block with a given alignment from the EA memory manager.
 *
 * Converts @p align to a power-of-two level with memlog2 (memlog2), minus 2 and at least 0, and calls
 * operator_new_tag with @p size, the name, @p flags | level << 24 | 0x10000000 and the header size @p hdrsize. The abort
 * flag is not used.
 *
 * @param tag     the block's name (On Tour name)
 * @param size    bytes to allocate
 * @param align   required alignment in bytes
 * @param hdrsize       header size (On Tour hdrsize)
 * @param flags   memory type flags (On Tour memtype)
 * @param abortflag  On Tour abortflag (not read)
 * @return the new block
 */
void* MEM_allocaligna_6(void* tag, int size, int align, int hdrsize, int flags, int abortflag)
{
    int lvl = memlog2(align) - 2 < 0 ? 0 : memlog2(align) - 2;
    return operator_new_tag(size, (const char*)tag, flags | (lvl << 24) | 0x10000000, hdrsize);
}
#endif

extern "C" void* MEM_allocaligna(void*);

//100%
INCLUDE_ASM("mem/memstd", _MEM_allocaligna);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sMemNode_2A10 {
    unsigned short tag;
    unsigned short flags;
    int size;
    sMemNode_2A10* pnext;       // 0x8
    sMemNode_2A10* pprev;       // 0xC
    sMemNode_2A10* fnext;       // 0x10
    sMemNode_2A10* fprev;       // 0x14
};

struct sMemHdr_2A10 {
    unsigned short tag;
    unsigned short flags;
    int size;
    sMemNode_2A10* pnext;       // 0x8
    sMemNode_2A10* pprev;       // 0xC
};

struct sMemClass_2A10 {
    char pad0[0x10];
    sMemNode_2A10 head;         // 0x10
    int align;                  // 0x28
    int bigAlign;               // 0x2C
    int hdr;                    // 0x30
    int flags;                  // 0x34
    int locked;                 // 0x38
    int sema[5];                // 0x3C
};

extern int D_004A2E78_2A10 __asm__("D_004A2E78");
extern int (*D_004A2E7C_2A10)(int, int, int) __asm__("D_004A2E7C");

// First free block of at least size bytes (same body as func_00253100).
static inline sMemNode_2A10* FirstFit_2A10(sMemClass_2A10* cls, int size, int back)
{
    sMemNode_2A10* n = &cls->head;
    if (back == 0) {
        do {
            n = n->fnext;
        } while (n->size < size);
    } else {
        do {
            n = n->fprev;
        } while (n->size < size);
    }
    return n->tag != 0x4253 ? n : 0;
}

// Largest free block holding min bytes (same body as func_00253170).
static inline sMemNode_2A10* BestFit_2A10(sMemClass_2A10* cls, int min, int back)
{
    int v = min - 1;
    if (v < 0) {
        v = 0;
    }
    sMemNode_2A10* best = 0;
    sMemNode_2A10* n = &cls->head;
    if (back == 0) {
        n = cls->head.fnext;
        for (;;) {
            if (v < n->size - 0x10) {
                if (n->tag == 0x4253) {
                    break;
                }
                best = n;
                v = best->size - 0x10;
            }
            n = n->fnext;
        }
    } else {
        n = cls->head.fprev;
        for (;;) {
            if (v < n->size - 0x10) {
                if (n->tag == 0x4253) {
                    break;
                }
                best = n;
                v = best->size - 0x10;
            }
            n = n->fprev;
        }
    }
    return best;
}

// Inserts a block into the address-sorted free list (same body as func_002532B0).
static inline void InsertFree_2A10(sMemClass_2A10* cls, sMemNode_2A10* blk)
{
    sMemNode_2A10* q = &cls->head;
    sMemNode_2A10* p = q;
    int sz = (char*)blk->pnext - (char*)blk;
    if ((char*)cls->head.fnext + ((char*)cls->head.fprev - (char*)cls->head.fnext) / 2 < (char*)blk) {
        do
            p = p->fprev;
        while (blk < p);
        q = p->fnext;
    } else {
        do
            q = q->fnext;
        while (q < blk);
        p = q->fprev;
    }
    blk->size = sz;
    blk->fnext = q;
    blk->fprev = p;
    p->fnext = blk;
    q->fprev = blk;
    blk->tag = 0x4246;
    blk->flags |= 0x4000;
}

// Takes a block off the free list (same body as func_00253368).
static inline void RemoveFree_2A10(sMemNode_2A10* blk)
{
    sMemNode_2A10* prev = blk->fprev;
    sMemNode_2A10* next = blk->fnext;
    prev->fnext = next;
    next->fprev = prev;
    blk->tag = 0x424F;
    blk->flags &= 0xBFFF;
}

// Allocates size bytes from the class picked by flags, aligned so that (p + offset) is aligned;
// retries while the out-of-memory callback asks for it.
extern "C" char* _MEM_allocaligna_impl(void* name, int size, int align, int offset, int flags, int unused) __asm__("_MEM_allocaligna");
/**
 * Allocates @p size bytes from the EA heap class @p flags selects, aligned so that data + @p offset is aligned.
 *
 * Flags 0 means the default class D_004A2E78; bit 0x200 picks the largest fitting free block (func_00253170 style)
 * instead of the first (func_00253100 style), bit 0x100 searches and places from the top, bit 0x400 uses the class's
 * big alignment (+0x2C) when @p align is 0. The size grows by the class tail (MEM_tailsize), 8 at least. The chosen
 * free block is unlinked, the data placed at its aligned start (or end, from the top), a remainder of more than
 * alignment + 0x30 becomes a new free block, and MEM_initblock writes the header with the class's header size (+0x30)
 * and @p flags. When nothing fits, the out-of-memory callback D_004A2E7C (0, size, flags) is asked and a result of 1
 * retries. Runs under the class mutex when the class is locked. A negative size returns 0.
 *
 * @param name  the block's name (not stored)
 * @param size  bytes wanted
 * @param align  alignment, or 0 for the class default
 * @param offset  offset within the block that must be aligned
 * @param flags  memory class id and placement flags
 * @param abortFlag  abort-on-failure flag, ignored here
 * @return the data pointer, or 0
 */
extern "C" char* _MEM_allocaligna_impl(void* name, int size, int align, int offset, int flags, int abortFlag)
{
    sMemHdr_2A10 save;
    char* allocation = 0;
    int retry;
    if (flags == 0)
        flags = D_004A2E78_2A10;
    sMemClass_2A10* cls = (sMemClass_2A10*)D_004D2438[flags & 0x3F];
    if (size < 0)
        return 0;
    int top = flags & 0x200;
    int back = flags & 0x100;
    do {
        retry = 0;
        if (cls->locked != 0)
            MUTEX_lock(cls->sema);
        int amask = cls->align - 1;
        int mask;
        if (align == 0) {
            mask = amask;
            if (flags & 0x400)
                mask = cls->bigAlign - 1;
        } else {
            mask = align - 1;
        }
        int n = size + MEM_tailsize(0, flags);
        if (n < 8)
            n = 8;
        int alignSlack = mask + 0x10;
        int need = n + alignSlack;
        sMemNode_2A10* blk;
        if (top)
            blk = BestFit_2A10(cls, need, back);
        else
            blk = FirstFit_2A10(cls, need, back);
        if (blk != 0) {
            sMemNode_2A10* rest = 0;
            int bsize = blk->size;
            RemoveFree_2A10(blk);
            save.pnext = blk->pnext;
            save.pprev = blk->pprev;
            if (back) {
                allocation = (char*)(((int)blk + bsize - n + offset) & ~mask) - offset;
                rest = blk;
            } else {
                allocation = (char*)(((int)((char*)blk + (mask + 0x10)) + offset) & ~mask) - offset;
            }
            blk = (sMemNode_2A10*)(allocation - 0x10);
            ((sMemHdr_2A10*)((int)allocation - 0x10))->pnext = save.pnext;
            ((sMemHdr_2A10*)((int)allocation - 0x10))->pprev = save.pprev;
            if (amask + 0x30 < bsize - need) {
                if (back) {
                    rest->pnext->pprev = blk;
                    ((sMemHdr_2A10*)((int)allocation - 0x10))->pnext = rest->pnext;
                    ((sMemHdr_2A10*)((int)allocation - 0x10))->pprev = rest;
                    MEM_initblock(rest, 0, 0, 0, 0, rest->pprev, blk);
                    InsertFree_2A10(cls, rest);
                } else {
                    rest = (sMemNode_2A10*)(((int)allocation + n + amask) & ~amask);
                    blk->pnext->pprev = rest;
                    blk->pprev->pnext = blk;
                    MEM_initblock(rest, 0, 0, 0, 0, blk, ((sMemHdr_2A10*)((int)allocation - 0x10))->pnext);
                    InsertFree_2A10(cls, rest);
                    blk->pnext = rest;
                }
            } else {
                blk->pnext->pprev = blk;
                blk->pprev->pnext = blk;
            }
            MEM_initblock((sMemNode_2A10*)(allocation - 0x10), 0, size, cls->hdr, flags, blk->pprev, blk->pnext);
        } else if (D_004A2E7C_2A10 != 0) {
            retry = D_004A2E7C_2A10(0, size, flags);
        }
        if (cls->locked != 0)
            MUTEX_unlock(cls->sema);
    } while (retry == 1);
    return allocation;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", MEM_allocalign__FPv);
#ifdef SKIP_ASM
void* MEM_allocaligna_6(void*, int, int, int, int, int) __asm__("MEM_allocaligna");

// PORT: the project's symbol name says (void*), but callers pass and this
// forwards 5 args (see cMoviePlayer_SSXAllocMemFunc); the real body is bound by asm label.
void* MEM_allocalign_impl(void* a0, int a1, int a2, int a3, int a4) __asm__("MEM_allocalign__FPv");

/**
 * Allocates an aligned block that aborts on failure: MEM_allocaligna(@p name, @p size, @p alignment, @p hdrSize,
 * @p memType, 1).
 *
 * @param name  the block's name
 * @param size  bytes wanted
 * @param alignment  alignment
 * @param hdrSize  aligned offset (header bytes)
 * @param memType  memory class and flags
 * @return MEM_allocaligna's result
 */
void* MEM_allocalign_impl(void* name, int size, int alignment, int hdrSize, int memType)
{
    return MEM_allocaligna_6(name, size, alignment, hdrSize, memType, 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", _MEM_allocalign);
#ifdef SKIP_ASM
extern "C" void _MEM_allocaligna(int, int, int, int, int, int);

/**
 * Allocates an aligned block from the EA heap, aborting on failure: _MEM_allocaligna(@p name, @p size, @p alignment,
 * @p hdrSize, @p memType, 1).
 *
 * @param name  the block's name
 * @param size  bytes wanted
 * @param alignment  alignment
 * @param hdrSize  aligned offset (header bytes)
 * @param memType  memory class and flags
 */
extern "C" void _MEM_allocalign(int name, int size, int alignment, int hdrSize, int memType) {
    _MEM_allocaligna(name, size, alignment, hdrSize, memType, 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", MEM_alloc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* MEM_allocaligna_6(void*, int, int, int, int, int) __asm__("MEM_allocaligna");

/**
 * Allocates a named block with the default 128-byte alignment.
 *
 * Calls MEM_allocaligna (MEM_allocaligna) with alignment 0x80, no header and the abort flag set.
 *
 * @param name  the block's name
 * @param size  bytes to allocate
 * @param memtype  memory type flags
 * @return the new block
 */
extern "C" void* MEM_alloc(void* name, int size, int memtype)
{
    return MEM_allocaligna_6(name, size, 0x80, 0, memtype, 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", _MEM_alloc);
#ifdef SKIP_ASM
/**
 * Allocates a block from the EA heap with 128-byte alignment, aborting on failure: _MEM_allocaligna(@p name, @p size,
 * 0x80, 0, @p memType, 1).
 *
 * @param name  the block's name
 * @param size  bytes wanted
 * @param memType  memory class and flags
 */
extern "C" void _MEM_alloc(int name, int size, int memType)
{
    _MEM_allocaligna(name, size, 0x80, 0, memType, 1);
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_allocz);
#ifdef SKIP_ASM
/**
 * Allocates a block that returns 0 instead of aborting on failure: MEM_allocaligna(@p name, @p size, 0, 0,
 * @p memType, 0), with the class's default alignment.
 *
 * @param name  the block's name
 * @param size  bytes wanted
 * @param memType  memory class and flags
 */
extern "C" void MEM_allocz(void* name, int size, int memType)
{
    MEM_allocaligna_6(name, size, 0, 0, memType, 0);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", _MEM_allocz);
#ifdef SKIP_ASM
extern "C" void _MEM_allocaligna(int, int, int, int, int, int);

/**
 * Allocates a block from the EA heap that returns 0 instead of aborting: _MEM_allocaligna(@p name, @p size, 0, 0,
 * @p memType, 0).
 *
 * @param name  the block's name
 * @param size  bytes wanted
 * @param memType  memory class and flags
 */
extern "C" void _MEM_allocz(int name, int size, int memType) {
    _MEM_allocaligna(name, size, 0, 0, memType, 0);
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_allocsize);
#ifdef SKIP_ASM
/**
 * Allocates @p size bytes, unnamed, from the default class, aborting on failure: MEM_allocaligna(0, @p size, 0, 0, 0,
 * 1).
 *
 * @param size  bytes wanted
 */
extern "C" void MEM_allocsize(int size)
{
    MEM_allocaligna_6(0, size, 0, 0, 0, 1);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", _MEM_allocsize);
#ifdef SKIP_ASM
extern "C" void _MEM_allocaligna(int, int, int, int, int, int);

/**
 * Allocates @p size bytes, unnamed, from the EA heap's default class, aborting on failure: _MEM_allocaligna(0, @p size, 0,
 * 0, 0, 1).
 *
 * @param size  bytes wanted
 */
extern "C" void _MEM_allocsize(int size) {
    _MEM_allocaligna(0, size, 0, 0, 0, 1);
}
#endif
//100%
INCLUDE_ASM("mem/memstd", MEM_allocsizez);
#ifdef SKIP_ASM
/**
 * Allocates @p size bytes, unnamed, from the default class, returning 0 instead of aborting: MEM_allocaligna(0,
 * @p size, 0, 0, 0, 0).
 *
 * @param size  bytes wanted
 */
extern "C" void MEM_allocsizez(int size)
{
    MEM_allocaligna_6(0, size, 0, 0, 0, 0);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", _MEM_allocsizez);
#ifdef SKIP_ASM
extern "C" void _MEM_allocaligna(int, int, int, int, int, int);

/**
 * Allocates @p size bytes, unnamed, from the EA heap's default class, returning 0 instead of aborting: _MEM_allocaligna(0,
 * @p size, 0, 0, 0, 0).
 *
 * @param size  bytes wanted
 */
extern "C" void _MEM_allocsizez(int size) {
    _MEM_allocaligna(0, size, 0, 0, 0, 0);
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_00253100);
#ifdef SKIP_ASM
/**
 * Returns the first free block of the EA heap class @p cls that holds at least @p size bytes (size field, header
 * included), walking the free list from the head forwards, or backwards when @p back is nonzero.
 *
 * The sentinel (tag 0x4253) stops the walk; reaching it returns 0. _MEM_allocaligna inlines the same search.
 *
 * @param cls  the memory class
 * @param size  minimum block size
 * @param back  nonzero to search from the top of memory down
 * @return the block, or 0
 */
extern "C" sMemNode_251718* func_00253100(sMemClass_251718* cls, int size, int back)
{
    sMemNode_251718* n = &cls->head;
    if (back == 0) {
        do {
            n = n->next;
        } while (n->size < size);
    } else {
        do {
            n = n->prev;
        } while (n->size < size);
    }
    return n->tag != 16979 ? n : 0;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_00253170);
#ifdef SKIP_ASM
/**
 * Returns the largest free block of the EA heap class @p cls whose data (size - 16) exceeds @p size - 1, walking the
 * free list forwards, or backwards when @p dir is nonzero, up to the sentinel (tag 0x4253).
 *
 * _MEM_allocaligna inlines the same search for its best-fit mode (flag 0x200).
 *
 * @param cls  the memory class
 * @param size  minimum data size
 * @param dir  nonzero to walk the list backwards
 * @return the block, or 0 when none is big enough
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Largest block in the class whose payload exceeds size - 1, walking forward
// (dir 0) or backward; 0 if none.
extern "C" sMemNode_251718* func_00253170(sMemClass_251718* cls, int size, int dir)
{
    int best = size - 1;
    if (best < 0) {
        best = 0;
    }
    sMemNode_251718* bestNode = 0;
    sMemNode_251718* node;
    if (dir == 0) {
        node = cls->head.next;
        for (;;) {
            if (best < node->size - 0x10) {
                if (node->tag == 16979) {
                    break;
                }
                bestNode = node;
                best = bestNode->size - 0x10;
            }
            node = node->next;
        }
    } else {
        node = cls->head.prev;
        for (;;) {
            if (best < node->size - 0x10) {
                if (node->tag == 16979) {
                    break;
                }
                bestNode = node;
                best = bestNode->size - 0x10;
            }
            node = node->prev;
        }
    }
    return bestNode;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_00253230);
#ifdef SKIP_ASM
/**
 * Returns the total free data bytes of the EA heap class @p cls: the sum of size - 16 over its free list up to the
 * sentinel (tag 0x4253).
 *
 * Both directions give the same total; @p back only chooses which end the walk starts from.
 *
 * @param cls  the memory class
 * @param back  nonzero to walk the list backwards
 * @return the free bytes
 */
extern "C" int func_00253230(sMemClass_251718* cls, int back)
{
    int total = 0;
    if (back == 0) {
        sMemNode_251718* n = cls->head.next;
        while (n->tag != 16979) {
            int size = n->size;
            int rest = total - 0x10;
            n = n->next;
            total = rest + size;
        }
    } else {
        sMemNode_251718* n = cls->head.prev;
        while (n->tag != 16979) {
            int size = n->size;
            int rest = total - 0x10;
            n = n->prev;
            total = rest + size;
        }
    }
    return total;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_002532B0);
#ifdef SKIP_ASM
/**
 * Inserts block @p blk into the address-sorted free list of the EA heap class @p cls.
 *
 * Sets the block's size to the distance to the next block in memory (end), finds its place by walking from the list end
 * nearer to its address, links it in, and tags it free (0x4246, flag 0x4000). _MEM_allocaligna inlines the same code.
 *
 * @param cls  the memory class
 * @param blk  the block to insert
 */
extern "C" void func_002532B0(sMemClass_251D50* cls, sMemNode_251D50* blk)
{
    sMemNode_251D50* nextNode = &cls->head;
    sMemNode_251D50* prevNode = nextNode;
    int sz = blk->end - (char*)blk;
    if ((char*)cls->head.next + ((char*)cls->head.prev - (char*)cls->head.next) / 2 < (char*)blk) {
        do
            prevNode = prevNode->prev;
        while (blk < prevNode);
        nextNode = prevNode->next;
    } else {
        do
            nextNode = nextNode->next;
        while (nextNode < blk);
        prevNode = nextNode->prev;
    }
    blk->size = sz;
    blk->next = nextNode;
    blk->prev = prevNode;
    prevNode->next = blk;
    nextNode->prev = blk;
    blk->tag = 16966;
    blk->flags |= 0x4000;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_00253368);
#ifdef SKIP_ASM
struct sMemBlock {
    unsigned short magic;
    unsigned short flags;
    int size;
    char pad[8];
    sMemBlock* next;
    sMemBlock* prev;
};

/**
 * Takes block @p blk off the EA heap's free list: unlinks it (next/prev), tags it 0x424F and clears its free flag
 * (0x4000).
 *
 * @param blk  the free block
 */
extern "C" void func_00253368(sMemBlock* blk) {
    sMemBlock* prev = blk->prev;
    sMemBlock* next = blk->next;
    prev->next = next;
    next->prev = prev;
    blk->magic = 16975;
    blk->flags &= 0xBFFF;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", tMoviePlayOptions_SetDefaults);
#ifdef SKIP_ASM
/**
 * Fills a movie play-options block with its defaults: play speed (+0x20) 1.0, draw layer (+0x1C) 31, and every other
 * word (video file +0x0, audio file +0x4, flags +0x8..+0x14, memory type +0x18) 0.
 *
 * Callers (cVidEngine_ReadyVideo, cRealFE's movie start, the rewards video player) fill it in before cMoviePlayer_init.
 */
extern "C" void tMoviePlayOptions_SetDefaults(cMoviePlayer* self)
{
    *(float*)&self->m_PlayOp.bFullScreen = 1.0f;
    self->m_PlayOp.bSoundOn = 31;
    self->mbMovieFinished = 0;
    self->mbReDraw = 0;
    self->mpCurrFrame = 0;
    self->mbPaused = 0;
    *(int*)&self->m_fSpeed = 0;
    *(int*)&self->m_PlayOp = 0;
    self->m_PlayOp.sAudioFileName = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", cMoviePlayer_cMoviePlayer);
#ifdef SKIP_ASM
extern "C" void cMoviePlayer_SetSSXREALDefaults();

/**
 * Constructs a movie player (0x44 bytes): installs the movie memory callbacks (cMoviePlayer_SetSSXREALDefaults), sets the play options
 * (+0x14) to their defaults (tMoviePlayOptions_SetDefaults), copies the default play speed (+0x34) to the speed (+0x10) and clears the
 * finished (+0x0), paused (+0x8) and frame (+0xC) words, the decoder (+0x38), the video driver (+0x3C) and the
 * new-frame flag (+0x40).
 *
 * @return @p self
 */
extern "C" void* cMoviePlayer_cMoviePlayer(cMoviePlayer* self)
{
    cMoviePlayer_SetSSXREALDefaults();
    tMoviePlayOptions_SetDefaults((cMoviePlayer*)&self->m_PlayOp);
    self->m_fSpeed = self->m_PlayOp.fPlaySpeed;
    self->m_pPlayer = 0;
    self->m_pVd = 0;
    self->mpCurrFrame = 0;
    self->mbNewFrame = 0;
    self->mbPaused = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cMoviePlayer__cMoviePlayer);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void cMoviePlayer_purge(cMoviePlayer* self);

/**
 * Destroys a movie player: releases everything (cMoviePlayer_purge) and frees the object when @p flags bit 0 is set.
 *
 * @param flags  bit 0 frees the memory (callers pass 3)
 */
extern "C" void cMoviePlayer__cMoviePlayer(cMoviePlayer* self, int flags)
{
    cMoviePlayer_purge(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cMoviePlayer_init1);
#ifdef SKIP_ASM
struct cMoviePlayer_init1_sInfo {
    int f0;
    char pad[0x20];
};

extern "C" void cMoviePlayer_init(void* self, void* info);

/**
 * Opens the movie @p videoFile with default play options: purges the player (cMoviePlayer_purge), fills a play-options block with
 * defaults (tMoviePlayOptions_SetDefaults), sets its video file to @p videoFile and opens it (cMoviePlayer_init).
 *
 * @param videoFile  the video file name
 */
extern "C" void cMoviePlayer_init1(cMoviePlayer* self, int videoFile)
{
    cMoviePlayer_init1_sInfo info;
    cMoviePlayer_purge(self);
    tMoviePlayOptions_SetDefaults((cMoviePlayer*)(&info));
    info.f0 = videoFile;
    cMoviePlayer_init(self, &info);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cMoviePlayer_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sMemAllocFuncs_2534A8 {
    void* (*alloc)(void*, int, int, int, int);
    void* (*free)(void*);
    int field_0x8;
};
// Typed view of the allocator table (the unit defines sMemAllocFuncs later).
extern sMemAllocFuncs_2534A8 D_00509430_2534A8 __asm__("RCMP_rcmp_sys");
extern char D_004A2E98[];
extern "C" void* func_003ADE08(void* mem, int a1, int a2, char* name, int a4, int a5, int a6);
extern "C" void* func_003ADDC0(void* mem, int a1, int a2, int a3, int a4);

struct sMemCfg_2534A8 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
};

extern "C" int cMoviePlayer_init_impl(void* self, sMemCfg_2534A8* cfg) __asm__("cMoviePlayer_init");
/**
 * Opens a movie: purges the player (cMoviePlayer_purge), copies the 0x24-byte play options @p cfg to +0x14 and creates the
 * REAL decoder (+0x38).
 *
 * Copies the options' play speed (+0x34) to the speed (+0x10). The decoder memory (0x78 bytes) comes from the movie
 * allocator table RCMP_rcmp_sys. With an audio file name (+0x18, non-empty) it builds the decoder with func_003ADE08
 * (video +0x14, 0x60000, audio file, 0x20000), else with func_003ADDC0 (video, 0x80000); both also get whether the
 * options word +0x24 is set and whether the word +0x1C is clear. Callers then start it with cMoviePlayer_asyncPlay.
 *
 * @param cfg  the play options (filled from tMoviePlayOptions_SetDefaults's defaults)
 * @return 1 (always)
 */
extern "C" int cMoviePlayer_init_impl(void* vself, sMemCfg_2534A8* cfg)
{
    cMoviePlayer* self = (cMoviePlayer*)vself;
    cMoviePlayer_purge((cMoviePlayer*)self);
    int soundOff = 1;
    *(sMemCfg_2534A8*)&self->m_PlayOp = *cfg;
    int optionSet = 0;
    if (self->m_PlayOp.bPreLoad != 0) {
        optionSet = 1;
    }
    if (self->m_PlayOp.bSoundOn != 0) {
        soundOff = 0;
    }
    self->m_fSpeed = self->m_PlayOp.fPlaySpeed;
    char* name = (char*)self->m_PlayOp.sAudioFileName;
    if (name != 0 && *name != 0) {
        void* mem = D_00509430_2534A8.alloc(D_004A2E98, 0x78, 0, 0, D_00509430_2534A8.field_0x8);
        self->m_pPlayer = func_003ADE08(mem, *(int*)&self->m_PlayOp, 0x60000, (char*)self->m_PlayOp.sAudioFileName, 0x20000, optionSet, soundOff);
    } else {
        void* mem = D_00509430_2534A8.alloc(D_004A2E98, 0x78, 0, 0, D_00509430_2534A8.field_0x8);
        self->m_pPlayer = func_003ADDC0(mem, *(int*)&self->m_PlayOp, 0x80000, optionSet, soundOff);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cMoviePlayer_purge);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void RCMP_DECODER_ReleaseFrame(void*, void*);
extern "C" void func_003AE300(void* p, int flags);
extern int cMoviePlayer_m_isPlaying;

class cWorld002535F8 {
public:
    char pad[0x10D8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void FlushRenderer();
};



/**
 * Releases the player's movie resources and resets its state.
 *
 * With a decoder (+0x38) and a video driver (+0x3C) it first flushes the renderer (graphics manager slot 19,
 * cGraphicsMan_FlushRenderer); a pending frame (+0x40, +0xC) goes back to the decoder (RCMP_DECODER_ReleaseFrame). The
 * driver is deleted (vtable slot 1, flags 3) and the active-player count cMoviePlayer_m_isPlaying decremented, the decoder is deleted
 * (func_003AE300, flags 3), and the finished, redraw, paused, frame and new-frame words are cleared.
 */
extern "C" void cMoviePlayer_purge(cMoviePlayer* self)
{
    cMoviePlayer* moviePlayer = self;
    if (((char*)moviePlayer->m_pPlayer) != 0 && moviePlayer->m_pVd != 0) {
        ((cWorld002535F8*)cGraphicsMan_TheGraphicsMan)->FlushRenderer();
    }
    if (moviePlayer->mbNewFrame != 0 && moviePlayer->mpCurrFrame != 0) {
        RCMP_DECODER_ReleaseFrame(*(void**)(((char*)moviePlayer->m_pPlayer) + 0x64), moviePlayer->mpCurrFrame);
    }
    RCMP_VIDEO_DRIVER* videoDriver = moviePlayer->m_pVd;
    if (((void*)videoDriver) != 0) {
        sVEntry<void(void*, int)>* dtorEntry = &((sVEntry<void(void*, int)>*)videoDriver->mVtbl)[1];
        dtorEntry->fn(vthis(videoDriver, *dtorEntry), 3);
        moviePlayer->m_pVd = 0;
        cMoviePlayer_m_isPlaying--;
    }
    char* decoderObj = ((char*)moviePlayer->m_pPlayer);
    if (decoderObj != 0) {
        func_003AE300(decoderObj, 3);
        *(char**)&moviePlayer->m_pPlayer = 0;
    }
    moviePlayer->mbMovieFinished = 0;
    moviePlayer->mbReDraw = 0;
    moviePlayer->mbPaused = 0;
    moviePlayer->mpCurrFrame = 0;
    moviePlayer->mbNewFrame = 0;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cMoviePlayer_startPlay);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int cMoviePlayer_m_isPlaying;
extern "C" int func_003AE450(void* p, int a, int b);
extern "C" void* SSX_VD_create(void* a0, int a1, int a2, int a3);
int func_003AE938(void*);
int func_003AE958_i(void*) __asm__("func_003AE958__FPv");

/**
 * Starts playback: fetches the first frame (func_003AE450(decoder, 1, 33)) into +0xC and sets the new-frame flag
 * (+0x40), pauses the decoder (func_003AE938), creates the video driver for that frame (SSX_VD_create with the
 * decoder's +0x64, the frame, cGraphicsMan_TheGraphicsMan and the draw layer +0x30) into +0x3C, resumes the decoder
 * (func_003AE958) and increments the active-player count cMoviePlayer_m_isPlaying.
 *
 * Clears the finished (+0x0) and redraw (+0x4) words first.
 */
extern "C" void cMoviePlayer_startPlay(cMoviePlayer* self)
{
    self->mbMovieFinished = 0;
    self->mbReDraw = 0;
    self->mpCurrFrame = 0;
    self->mpCurrFrame = (void*)func_003AE450(self->m_pPlayer, 1, 33);
    self->mbNewFrame = 1;
    func_003AE938(self->m_pPlayer);
    self->m_pVd = (RCMP_VIDEO_DRIVER*)SSX_VD_create(*(void**)((char*)self->m_pPlayer + 0x64), (int)self->mpCurrFrame, (int)cGraphicsMan_TheGraphicsMan, self->m_PlayOp.iDrawLayer);
    func_003AE958_i(self->m_pPlayer);
    cMoviePlayer_m_isPlaying++;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cMoviePlayer_syncPlay);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_003AE6C8(void* p, float f);
extern "C" int func_003AE780(void* p);
extern "C" int func_003AE860(void* p);
extern "C" void SYNCTASK_run(int);
extern "C" void THREAD_yield(int);



struct sWorld_253748 {
    char pad[0x10D8];
    sVEntry<void(void*)>* vt;            // 0x10D8
};

/**
 * Plays the whole movie before returning, when a decoder (+0x38) exists.
 *
 * Starts playback (cMoviePlayer_startPlay), then until finished (+0x0): draws the frame (driver slot 5 when redraw +0x4 is set,
 * else slot 4 with the frame +0xC), shows it (graphics manager slot 20, cPS2GraphicsMan_DrawFrame), runs
 * SYNCTASK_run(0) and THREAD_yield(0), and when the decoder is due (func_003AE780) completes the draw (driver slot 6)
 * and fetches the next frame for the decoder's time (+0x48, func_003AE6C8). Without a frame it asks func_003AE860
 * whether the movie has finished. Clears the new-frame flag (+0x40) at the end.
 */
extern "C" void cMoviePlayer_syncPlay(cMoviePlayer* self)
{
    cMoviePlayer* moviePlayer = self;
    if (((char*)moviePlayer->m_pPlayer) != 0) {
        cMoviePlayer_startPlay((cMoviePlayer*)moviePlayer);
        do {
            if (moviePlayer->mbReDraw != 0) {
                RCMP_VIDEO_DRIVER* videoDriver = moviePlayer->m_pVd;
                sVEntry<void(void*)>* slotEntry = &((sVEntry<void(void*)>*)videoDriver->mVtbl)[5];
                slotEntry->fn(vthis(videoDriver, *slotEntry));
            } else {
                RCMP_VIDEO_DRIVER* videoDriver = moviePlayer->m_pVd;
                sVEntry<void(void*, void*)>* slotEntry = &((sVEntry<void(void*, void*)>*)videoDriver->mVtbl)[4];
                slotEntry->fn(vthis(videoDriver, *slotEntry), moviePlayer->mpCurrFrame);
            }
            sVEntry<void(void*)>* w = &((sVEntry<void(void*)>*)cGraphicsMan_TheGraphicsMan->mVtbl)[20];
            w->fn(vthis(cGraphicsMan_TheGraphicsMan, *w));
            SYNCTASK_run(0);
            THREAD_yield(0);
            if (func_003AE780(((char*)moviePlayer->m_pPlayer)) != 0) {
                RCMP_VIDEO_DRIVER* videoDriver = moviePlayer->m_pVd;
                sVEntry<void(void*)>* slotEntry = &((sVEntry<void(void*)>*)videoDriver->mVtbl)[6];
                slotEntry->fn(vthis(videoDriver, *slotEntry));
                moviePlayer->mpCurrFrame = (void*)func_003AE6C8(((char*)moviePlayer->m_pPlayer), *(float*)(((char*)moviePlayer->m_pPlayer) + 0x48));
                moviePlayer->mbReDraw = 0;
            } else {
                moviePlayer->mbReDraw = 1;
            }
            if (moviePlayer->mpCurrFrame == 0) {
                moviePlayer->mbReDraw = 1;
                moviePlayer->mbMovieFinished = func_003AE860(((char*)moviePlayer->m_pPlayer));
            }
        } while (moviePlayer->mbMovieFinished == 0);
        moviePlayer->mbNewFrame = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", cMoviePlayer_asyncPlay);
#ifdef SKIP_ASM
extern "C" void cMoviePlayer_startPlay(cMoviePlayer* self);

/**
 * Starts asynchronous playback: when a decoder (+0x38) exists, starts it (cMoviePlayer_startPlay).
 *
 * @return 1 when started, 0 without a decoder
 */
extern "C" int cMoviePlayer_asyncPlay(cMoviePlayer* self)
{
    if (self->m_pPlayer == 0) {
        return 0;
    }
    cMoviePlayer_startPlay(self);
    return 1;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cMoviePlayer_asyncRender);
#ifdef SKIP_ASM
class cMemObj_253890 {
public:
    int field_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04(int a);
    virtual void v05();
};

/**
 * Draws the movie for this frame, when a decoder (+0x38) and a video driver (+0x3C) exist.
 *
 * Redraws the last image (driver slot 5) when @p force is set, when paused (+0x8), when no new frame is waiting (+0x40)
 * and redraw (+0x4) is set, or when the movie has finished (+0x0); otherwise draws the current frame (driver slot 4
 * with +0xC) and clears the new-frame flag.
 *
 * @param force  nonzero to redraw the previous image
 */
extern "C" void cMoviePlayer_asyncRender(cMoviePlayer* self, int force)
{
    if (self->m_pPlayer == 0) {
        return;
    }
    cMemObj_253890* obj = (cMemObj_253890*)self->m_pVd;
    if (obj == 0) {
        return;
    }
    if (force != 0 || self->mbPaused != 0
        || (self->mbNewFrame == 0 && self->mbReDraw != 0)
        || self->mbMovieFinished != 0) {
        ((cMemObj_253890*)self->m_pVd)->v05();
    } else {
        obj->v04((int)self->mpCurrFrame);
        self->mbNewFrame = 0;
    }
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cMoviePlayer_asyncUpdate);
#ifdef SKIP_ASM
extern "C" int func_003AE780(void* p);
extern "C" int func_003AE6C8(void* p, float f);
extern "C" int func_003AE860(void* p);

class cMemObj_253938 {
public:
    int field_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04(int a);
    virtual void v05();
    virtual void v06();
};

/**
 * Movie player step: decodes the next frame when it is due and notices when the movie has finished.
 *
 * If no frame is waiting (+0x40) and func_003AE780 says the decoder (+0x38) is due, calls virtual slot 6 of +0x3C,
 * gets a frame for the decoder's time (+0x48) with func_003AE6C8 into +0xC and marks it waiting; otherwise sets
 * +0x4. When there is no frame and the movie is not finished (+0x0), clears the waiting flag and stores
 * func_003AE860's answer (audio finished) in +0x0.
 */
extern "C" void cMoviePlayer_asyncUpdate(cMoviePlayer* self)
{
    if (self->mbNewFrame == 0 && func_003AE780(self->m_pPlayer) != 0) {
        ((cMemObj_253938*)self->m_pVd)->v06();
        void* decoder = self->m_pPlayer;
        self->mpCurrFrame = (void*)func_003AE6C8(decoder, *(float*)((char*)decoder + 0x48));
        self->mbNewFrame = 1;
        self->mbReDraw = 0;
    } else {
        self->mbReDraw = 1;
    }
    if ((int)self->mpCurrFrame == 0 && self->mbMovieFinished == 0) {
        self->mbNewFrame = 0;
        self->mbReDraw = 1;
        self->mbMovieFinished = func_003AE860(self->m_pPlayer);
    }
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cMoviePlayer_asyncPause);
#ifdef SKIP_ASM
int func_003AE938(void*);

/**
 * Pauses the movie: when a decoder (+0x38) and a driver (+0x3C) exist and it is neither finished (+0x0) nor already
 * paused (+0x8), pauses the decoder (func_003AE938) and sets the paused word.
 */
extern "C" void cMoviePlayer_asyncPause(cMoviePlayer* self)
{
    void* handle = self->m_pPlayer;
    if (handle != 0 && (int)self->m_pVd != 0 && self->mbMovieFinished == 0
        && self->mbPaused == 0) {
        func_003AE938(handle);
        self->mbPaused = 1;
    }
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cMoviePlayer_asyncResume);
#ifdef SKIP_ASM
void func_003AE958(void*);

/**
 * Resumes a paused movie: when a decoder (+0x38) and a driver (+0x3C) exist, it has not finished (+0x0) and it is
 * paused (+0x8), resumes the decoder (func_003AE958) and clears the paused word.
 */
extern "C" void cMoviePlayer_asyncResume(cMoviePlayer* self)
{
    void* handle = self->m_pPlayer;
    if (handle != 0 && (int)self->m_pVd != 0 && self->mbMovieFinished == 0
        && self->mbPaused != 0) {
        func_003AE958(handle);
        self->mbPaused = 0;
    }
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cMoviePlayer_SetSSXREALDefaults);
#ifdef SKIP_ASM
extern "C" void* cMoviePlayer_SSXAllocMemFunc(int size);
void* cMoviePlayer_SSXFreeMemFunc(void* self);

struct sMemAllocFuncs {
    void* (*alloc)(int);    // 0x0
    void* (*free)(void*);   // 0x4
    int field_0x8;          // 0x8
};
extern sMemAllocFuncs RCMP_rcmp_sys;

/**
 * Installs the movie memory callbacks in RCMP_rcmp_sys: allocate cMoviePlayer_SSXAllocMemFunc, free cMoviePlayer_SSXFreeMemFunc, memory type 0x400.
 *
 * The decoder and video driver code allocate through this table.
 */
extern "C" void cMoviePlayer_SetSSXREALDefaults()
{
    RCMP_rcmp_sys.alloc = cMoviePlayer_SSXAllocMemFunc;
    RCMP_rcmp_sys.free = cMoviePlayer_SSXFreeMemFunc;
    RCMP_rcmp_sys.field_0x8 = 0x400;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", cMoviePlayer_SSXAllocMemFunc);
#ifdef SKIP_ASM
void* MEM_allocalign_5(void* a0, int a1, int a2, int a3, int a4) __asm__("MEM_allocalign__FPv");

// PORT: the unit declares this as (int size); the body uses 5 args (prototype mismatch).
void* cMoviePlayer_SSXAllocMemFunc_impl(void* a0, int a1, int align, int a3, int a4) __asm__("cMoviePlayer_SSXAllocMemFunc");

/**
 * Movie allocation callback: allocates through MEM_allocalign (MEM_allocaligna with abort) with at least 64-byte
 * alignment.
 *
 * @param blockName  the block's name
 * @param byteCount  bytes wanted
 * @param align  alignment, raised to 64
 * @param hdrSize  header bytes
 * @param memType  memory class and flags
 * @return the new block
 */
void* cMoviePlayer_SSXAllocMemFunc_impl(void* blockName, int byteCount, int align, int hdrSize, int memType)
{
    if (align < 64) {
        align = 64;
    }
    return MEM_allocalign_5(blockName, byteCount, align, hdrSize, memType);
}
#endif

extern "C" void* MEM_free(void* self);

//99.29%
INCLUDE_ASM("mem/memstd", cMoviePlayer_SSXFreeMemFunc__FPv);
#ifdef SKIP_ASM
/**
 * Movie free callback: releases the block @p self through MEM_free.
 *
 * @return MEM_free's result
 */
void* cMoviePlayer_SSXFreeMemFunc(void* self)
{
    return MEM_free(self);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00253B18);
#ifdef SKIP_ASM
/**
 * Returns @p value unchanged, ignoring any further arguments (a variadic no-op).
 *
 * @param value  the value returned
 * @return @p value
 */
extern "C" int func_00253B18(int value, ...)
{
    return value;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00253B58);
#ifdef SKIP_ASM
inline void* operator new(unsigned int, void* p) { return p; }

struct sV4_3B58 {
    float x, y, z, w;
    sV4_3B58(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

extern char D_004D2538[], D_004D2548[], D_004D2558[], D_004D2568[], D_004D2578[], D_004D2588[], D_004D2598[], D_004D25A8[], D_004D25B8[], D_004D25C8[], D_004D25D8[], D_004D25E8[], D_004D25F8[], D_004D2608[], D_004D2618[], D_004D2628[], D_004D2638[], D_004D2648[], D_004D2658[], D_004D2668[], D_004D2678[], D_004D2688[], D_004D2698[], D_004D26A8[], D_004D26B8[], D_004D26C8[], D_004D26D8[], D_004D26E8[], D_004D26F8[], D_004D2708[], D_004D2718[], D_004D2728[], D_004D2738[], D_004D2748[], D_004D2758[], D_004D2768[], D_004D2778[], D_004D2788[], D_004D2798[], D_004D27A8[], D_004D27B8[], D_004D27C8[], D_004D27D8[], D_004D27E8[], D_004D27F8[], D_004D2808[], D_004D2818[], D_004D2828[], D_004D2838[], D_004D2848[], D_004D2858[], D_004D2868[], D_004D2878[], D_004D2888[], D_004D2898[], D_004D28A8[];

// PORT: the unit declares func_00253B58 as void*(int, int); it returns nothing.
extern "C" void func_00253B58_impl(int initialize, int priority) __asm__("func_00253B58");
/**
 * Static initialiser of the movie player's unit: when @p initialize is set and @p priority is 0xFFFF, constructs 56
 * four-float constants (D_004D2538..D_004D28A8, colour-like values between 0 and 1).
 *
 * func_00254330 calls it as the unit's global constructor; func_00254E60 builds an identical table for the next unit.
 *
 * @param initialize  nonzero to construct (0 would be destruction, which has nothing to do)
 * @param priority  init priority; only 0xFFFF does anything
 */
extern "C" void func_00253B58_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (D_004D2538) sV4_3B58(1.0f, 1.0f, 1.0f, 0.5f);
        new (D_004D2548) sV4_3B58(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (D_004D2558) sV4_3B58(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (D_004D2568) sV4_3B58(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (D_004D2578) sV4_3B58(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (D_004D2588) sV4_3B58(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (D_004D2598) sV4_3B58(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (D_004D25A8) sV4_3B58(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (D_004D25B8) sV4_3B58(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (D_004D25C8) sV4_3B58(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (D_004D25D8) sV4_3B58(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D25E8) sV4_3B58(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D25F8) sV4_3B58(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (D_004D2608) sV4_3B58(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2618) sV4_3B58(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2628) sV4_3B58(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D2638) sV4_3B58(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D2648) sV4_3B58(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2658) sV4_3B58(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D2668) sV4_3B58(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D2678) sV4_3B58(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D2688) sV4_3B58(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (D_004D2698) sV4_3B58(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (D_004D26A8) sV4_3B58(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (D_004D26B8) sV4_3B58(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D26C8) sV4_3B58(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (D_004D26D8) sV4_3B58(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (D_004D26E8) sV4_3B58(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (D_004D26F8) sV4_3B58(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (D_004D2708) sV4_3B58(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (D_004D2718) sV4_3B58(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2728) sV4_3B58(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2738) sV4_3B58(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (D_004D2748) sV4_3B58(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2758) sV4_3B58(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (D_004D2768) sV4_3B58(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2778) sV4_3B58(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D2788) sV4_3B58(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (D_004D2798) sV4_3B58(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (D_004D27A8) sV4_3B58(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (D_004D27B8) sV4_3B58(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (D_004D27C8) sV4_3B58(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (D_004D27D8) sV4_3B58(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D27E8) sV4_3B58(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (D_004D27F8) sV4_3B58(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D2808) sV4_3B58(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D2818) sV4_3B58(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D2828) sV4_3B58(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (D_004D2838) sV4_3B58(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (D_004D2848) sV4_3B58(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (D_004D2858) sV4_3B58(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (D_004D2868) sV4_3B58(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (D_004D2878) sV4_3B58(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (D_004D2888) sV4_3B58(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (D_004D2898) sV4_3B58(1.0f, 0.0f, 1.0f, 0.0f);
        new (D_004D28A8) sV4_3B58(1.0f, 1.0f, 0.0f, 0.0f);
    }
}
#endif

extern "C" void* func_00253B58(int, int);

//99.38%
INCLUDE_ASM("mem/memstd", func_00254330__FPv);
#ifdef SKIP_ASM
/**
 * Runs the movie player unit's static initialisation: func_00253B58(1, 0xFFFF).
 *
 * @return whatever func_00253B58 left in the return register
 */
void* func_00254330(void* self)
{
    return func_00253B58(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00254350);
#ifdef SKIP_ASM
/**
 * Returns floor(log2(@p x)) for a positive @p x, read from the exponent of its float conversion.
 *
 * For values above 2^24 the float rounding can round up to the next power of two.
 *
 * @param x  a positive integer
 * @return the index of its highest set bit
 */
extern "C" int func_00254350(int x)
{
    float fv = (float)x;
    int bits = *(int*)&fv;
    return (bits >> 23) - 0x7f;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", SSX_VD_create);
#ifdef SKIP_ASM
extern char D_004A2EA0[];
extern "C" void* func_00254410(void* self, void* a0, int a1, int a2, int a3);

// PORT: the unit's sMemAllocFuncs declares alloc as `void* (*)(int)`; it really
// takes (tag, size, align, d, flags) (see MEM_allocalign / cMoviePlayer_SSXAllocMemFunc).
typedef void* (*tMemAlloc5_00254368)(void*, int, int, int, int);

/**
 * Creates the movie's video driver: allocates 84 bytes from the movie allocator table RCMP_rcmp_sys and constructs the
 * tiled-texture driver in it (func_00254410).
 *
 * @param framesDecoder  the decoder (handed to RCMP_DECODER_ReleaseFrame later)
 * @param firstFrame  the first frame
 * @param graphicsMan  the graphics manager (cGraphicsMan_TheGraphicsMan)
 * @param drawLayer  the draw layer
 * @return the new driver
 */
extern "C" void* SSX_VD_create(void* framesDecoder, int firstFrame, int graphicsMan, int drawLayer)
{
    void* driverMem = ((tMemAlloc5_00254368)RCMP_rcmp_sys.alloc)(D_004A2EA0, 84, 0, 0, RCMP_rcmp_sys.field_0x8);
    return func_00254410(driverMem, framesDecoder, firstFrame, graphicsMan, drawLayer);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", SSX_VD_PS2_SINGLE_BUFFERD_SetPosition);
#ifdef SKIP_ASM
/**
 * Sets the video driver's screen position: +0x40, +0x44, +0x48 = @p posX, @p posY, @p posZ.
 *
 * @param posX  left edge
 * @param posY  top edge
 * @param posZ  depth
 */
extern "C" void SSX_VD_PS2_SINGLE_BUFFERD_SetPosition(SSX_VD_PS2_SINGLE_BUFFERD* self, float posX, float posY, float posZ)
{
    self->mPosX = posX;
    self->mPosY = posY;
    self->mPosZ = posZ;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", SSX_VD_PS2_SINGLE_BUFFERD_SetScale__FPvff);
#ifdef SKIP_ASM
/**
 * Sets the video driver's scale: +0x4C (width) and +0x50 (height) = @p scaleX, @p scaleY.
 *
 * @param scaleX  horizontal scale
 * @param scaleY  vertical scale
 */
void SSX_VD_PS2_SINGLE_BUFFERD_SetScale(void* vself, float scaleX, float scaleY)
{
    SSX_VD_PS2_SINGLE_BUFFERD* self = (SSX_VD_PS2_SINGLE_BUFFERD*)vself;
    self->mScaleX = scaleX;
    self->mScaleY = scaleY;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00254410);
#ifdef SKIP_ASM
// PORT: the unit's sMemAllocFuncs declares alloc as `void* (*)(int)`; it really
// takes (tag, size, align, d, flags); view bound by asm label.
struct sMemAlloc_4410 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* (*free)(void* p);
    int flags;
};
extern sMemAlloc_4410 D_00509430_4410 __asm__("RCMP_rcmp_sys");
extern void* D_00480270[];
extern char str_VD_VD_TEXTURE_POINTER[];
extern char D_004A2EA0[];
extern char D_004A2EA8[];
extern float D_004A2EB0[];
extern "C" int func_00254350(int x);
extern "C" void SSX_VD_PS2_SINGLE_BUFFERD_SetPosition(SSX_VD_PS2_SINGLE_BUFFERD* self, float a, float b, float c);
void SSX_VD_PS2_SINGLE_BUFFERD_SetScale(void* self, float f0, float f1);
// PORT: the project names this func_0037D090__FPv (one arg); this caller passes six.
void func_0037D090_6(void* ctx, int id, int a, int b, int c, int d) __asm__("func_0037D090__FPv");

struct sVE_4410a {
    short delta;
    short index;
    int (*fn)(void*, char*, char*, int, int, int, int, int, int, int, int, int);
};
// PORT: the unit declares cGraphicsMan_TheGraphicsMan with other types; view bound by asm label

struct sVE_4410b {
    short delta;
    short index;
    unsigned int (*fn)(void*);
};

class cRend_4410 {
public:
    char pad[0x10D8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual unsigned int width();
    virtual unsigned int height();
};

struct sTile_4410 {
    int tex;        // 0x0
    int w;          // 0x4
    int h;          // 0x8
    static void* operator new(unsigned int size) { return D_00509430_4410.alloc(D_004A2EA0, size, 0, 0, D_00509430_4410.flags); }
    sTile_4410(char* pixels, int height)
    {
        w = 16;
        h = height;
        cPS2GraphicsMan* g = cGraphicsMan_TheGraphicsMan;
        tex = ((sVE_4410a*)g->mVtbl)[47].fn((char*)g + ((sVE_4410a*)g->mVtbl)[47].delta, pixels, D_004A2EA8, w, h, 0, 0, 0, 0, 0, 0, -1);
        func_0037D090_6(g, tex, 0, 0, 0, 0);
    }
};

struct sTexSheet_4410 {
    void* owner;            // 0x00
    void** vt;              // 0x04
    cRend_4410* rend;       // 0x08
    int fC;                 // 0x0C
    char* hdr;              // 0x10
    sTile_4410** tiles;     // 0x14
    int count;              // 0x18
    int cols;               // 0x1C
    int rows;               // 0x20
    int heights[6];         // 0x24
    int f3C;                // 0x3C
};

// PORT: the unit declares this as (void*, void*, int, int, int); typed body bound by asm label.
extern "C" void* func_00254410_body(sTexSheet_4410* self, void* owner, char* res, cRend_4410* rend, int a3) __asm__("func_00254410");

/**
 * Constructs the movie's tiled-texture video driver over the first frame and uploads the frame as textures.
 *
 * Stores the decoder (+0x0), the vtable D_00480270 and the graphics manager (+0x8), reads the frame image header
 * (@p res + 4; data at header + 0x10, or at its offset +0x10 when flag 0x1000 is set). The image is cut into 16-pixel
 * columns (+0x1C) and power-of-two row heights (+0x24, count +0x20, func_00254350); each tile (12 bytes from the movie
 * allocator) gets a texture from graphics manager slot 47 (cPS2GraphicsMan_AddTexture1) and func_0037D090. The tile table (+0x14,
 * name "VD: VD_TEXTURE_POINTER") holds +0x18 entries. Centres the image on the screen (SSX_VD_PS2_SINGLE_BUFFERD_SetPosition with the screen
 * width and height from slots 10/11, depth D_004A2EB0), sets the scale to 1 (SSX_VD_PS2_SINGLE_BUFFERD_SetScale) and stores the draw layer
 * (+0x3C).
 *
 * @param owner  the decoder
 * @param res  the first frame
 * @param rend  the graphics manager
 * @param drawLayer  the draw layer
 * @return @p self
 */
extern "C" void* func_00254410_body(sTexSheet_4410* self, void* owner, char* res, cRend_4410* rend, int drawLayer)
{
    self->owner = owner;
    self->vt = D_00480270;
    self->rend = rend;
    char* hdr = *(char**)(res + 0x4);
    self->hdr = hdr;
    char* data;
    if (*(int*)(hdr + 0xC) & 0x1000) {
        data = hdr + *(int*)(hdr + 0x10);
    } else {
        data = hdr + 0x10;
    }
    int w = *(short*)(self->hdr + 4);
    int frameHeight = *(short*)(self->hdr + 6);
    self->rows = 0;
    self->cols = w / 16;
    int rem = frameHeight;
    for (;;) {
        int stripHeight = 1 << func_00254350(rem);
        if (rem == stripHeight) {
            self->heights[self->rows] = rem;
            self->rows++;
            break;
        }
        self->heights[self->rows] = stripHeight;
        rem -= self->heights[self->rows++];
    }
    self->count = self->cols * self->rows;
    self->tiles = (sTile_4410**)D_00509430_4410.alloc(str_VD_VD_TEXTURE_POINTER, self->count * 12, 0, 0, D_00509430_4410.flags);
    float fh = (float)frameHeight;
    float fw = (float)w;
    int k = 0;
    for (int i = 0; i < self->cols; i++) {
        int y = 0;
        for (int j = 0; j < self->rows; j++) {
            self->tiles[k++] = new sTile_4410(data + ((y << 6) + frameHeight * (i << 6)), self->heights[j]);
            y += self->heights[j];
        }
    }
    self->fC = 0;
    float sw = (float)self->rend->width();
    float sh = (float)self->rend->height() - fh;
    SSX_VD_PS2_SINGLE_BUFFERD_SetPosition((SSX_VD_PS2_SINGLE_BUFFERD*)self, (sw - fw) * 0.5f, sh * 0.5f, D_004A2EB0[0]);
    SSX_VD_PS2_SINGLE_BUFFERD_SetScale(self, 1.0f, 1.0f);
    self->f3C = drawLayer;
    return self;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_002547B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_00480270[];
extern void* D_004802B0[];
extern void* (*D_00509434[])(void*);

struct sRVEntry_2547B8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sRCtx_2547B8 {
    char pad[0x10D8];
    sRVEntry_2547B8* vt;
};

struct sTexCache_2547B8 {
    void* owner;
    void** vt;
    int f8;
    void* fC;
    int f10;
    int** texs;
    int count;
};

/**
 * Destroys the tiled-texture video driver: removes each tile's texture (graphics manager slot 50,
 * cPS2GraphicsMan_RemoveTexture), marks it -1 and frees the tile through RCMP_rcmp_sys, frees the tile table
 * (cMemMan_free), returns a held frame (+0xC) to the decoder (RCMP_DECODER_ReleaseFrame), installs the base vtable
 * D_004802B0 and, when @p flags bit 0 is set, frees the object through the movie free hook.
 *
 * Vtable slot 1 of D_00480270.
 *
 * @param flags  bit 0 frees the memory
 */
extern "C" void func_002547B8(sTexCache_2547B8* self, int flags)
{
    self->vt = D_00480270;
    for (int i = 0; i < self->count; i++) {
        int* tileEntry = self->texs[i];
        if (tileEntry != 0) {
            ((sRVEntry_2547B8*)cGraphicsMan_TheGraphicsMan->mVtbl)[50].fn(vthis(cGraphicsMan_TheGraphicsMan, ((sRVEntry_2547B8*)cGraphicsMan_TheGraphicsMan->mVtbl)[50]), *tileEntry);
            *tileEntry = -1;
            RCMP_rcmp_sys.free(tileEntry);
        }
    }
    if (self->texs != 0) {
        cMemMan_free(self->texs);
    }
    self->texs = 0;
    if (self->fC != 0) {
        RCMP_DECODER_ReleaseFrame(self->owner, self->fC);
        self->fC = 0;
    }
    self->vt = D_004802B0;
    if (flags & 1) {
        D_00509434[0](self);
    }
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_002548D0);
#ifdef SKIP_ASM
struct sVec4_2548D0 {
    float v[4];
    sVec4_2548D0() {}
} __attribute__((aligned(16)));

struct sVtx_2548D0 {
    float u, v, q;
    int pad;
    int r, g, b, a;
    sVec4_2548D0 pos;
    sVtx_2548D0() {}
};

struct sRS_2548D0 {
    int f0;
    int f4;
    int f8;
    int fC;
    short f10;
};

class cWorld_2548D0 {
public:
    char pad0[0xE84];
    sRS_2548D0* top;
    char pad1[0x10D8 - 0xE88];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual unsigned int GetScreenWidth();
    virtual unsigned int GetScreenHeight();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual int IsRenderReady();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void SetViewportOrthographic(int a, int b, float x, float y, float w, float h, float zn, float zf);
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void SetView1(void* p);
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void RenderTriStrip(int n, void* verts, int flags);
};

extern char t4mat_Identity[];

struct sMesh_2548D0 {
    int f0;
    int f4;
    cWorld_2548D0* world;   // 0x8
    void* res;              // 0xC
    char* hdr;              // 0x10
    int** items;            // 0x14
    int f18;
    int rows;               // 0x1C
    int cols;               // 0x20
    int widths[6];          // 0x24
    int f3C;                // 0x3C
    float x;                // 0x40
    float y;                // 0x44
    float z;                // 0x48
    float cw;               // 0x4C
    float ch;               // 0x50
};

// PORT: the unit declares func_002548D0 as returning void*, but it returns nothing (v0 is left as is); bind the real signature.
extern "C" void func_002548D0_impl(void* p) __asm__("func_002548D0");
/**
 * Draws the driver's tiles as screen quads: waits until the renderer is ready (graphics manager slot 17), sets a
 * full-screen viewport (slot 26) and the identity view (slot 34), puts the draw layer (+0x3C) into the render state,
 * then emits one 4-vertex strip (slot 71, cPS2GraphicsMan_RenderTriStrip) per tile.
 *
 * Tiles are laid out column by column from the position (+0x40, +0x44, +0x48, offset by half a pixel): each column 16 *
 * scale X (+0x4C) wide, each row its height (+0x24 table) times scale Y (+0x50), with grey (0x80) vertex colour and
 * full UVs, and the tile's texture set as the current one.
 *
 * @param videoDriver  the video driver
 */
extern "C" void func_002548D0_impl(void* videoDriver)
{
    sMesh_2548D0* self = (sMesh_2548D0*)videoDriver;
    while (self->world->IsRenderReady() == 0) {
    }
    float w = (float)self->world->GetScreenWidth();
    float screenHeight = (float)self->world->GetScreenHeight();
    self->world->SetViewportOrthographic(0, 0, 0.0f, 0.0f, w, screenHeight, 0.0f, 1.0f);
    self->world->SetView1(t4mat_Identity);
    sRS_2548D0* rs = self->world->top;
    rs->f8 = (rs->f8 & ~0x3E0) | ((self->f3C << 5) & 0x3E0);
    sVtx_2548D0 quadVerts[4];
    int i;
    for (i = 0; i < 4; i++) {
        quadVerts[i].a = 0x80;
        quadVerts[i].r = 0x80;
        quadVerts[i].g = 0x80;
        quadVerts[i].b = 0x80;
        quadVerts[i].q = 1.0f;
    }
    float cw = self->cw * 16.0f;
    float x0 = self->x + 0.5f;
    quadVerts[0].u = 0.0f;
    quadVerts[0].v = 0.0f;
    float x1 = x0 + cw;
    quadVerts[1].u = 1.0f;
    quadVerts[1].v = 0.0f;
    quadVerts[2].u = 0.0f;
    quadVerts[2].v = 1.0f;
    quadVerts[3].u = 1.0f;
    quadVerts[3].v = 1.0f;
    int cell = 0;
    int row;
    for (row = 0; row < self->rows; row++) {
        float yoff = 0.0f;
        int col = 0;
        if (col < self->cols) {
            int* wp = self->widths;
            do {
                float y = self->y + 0.5f + yoff;
                float ch = (float)*wp * self->ch;
                int id = *self->items[cell];
                ((cWorld_2548D0*)cGraphicsMan_TheGraphicsMan)->top->f10 = id;
                { sVec4_2548D0 cornerPos; cornerPos.v[0] = x0; cornerPos.v[1] = y; cornerPos.v[2] = self->z; cornerPos.v[3] = 1.0f; quadVerts[0].pos = cornerPos; }
                { sVec4_2548D0 cornerPos; cornerPos.v[0] = x1; cornerPos.v[1] = y; cornerPos.v[2] = self->z; cornerPos.v[3] = 1.0f; quadVerts[1].pos = cornerPos; }
                { sVec4_2548D0 cornerPos; cornerPos.v[0] = x0; cornerPos.v[1] = y + ch; cornerPos.v[2] = self->z; cornerPos.v[3] = 1.0f; quadVerts[2].pos = cornerPos; }
                { sVec4_2548D0 cornerPos; cornerPos.v[0] = x1; cornerPos.v[1] = y + ch; cornerPos.v[2] = self->z; cornerPos.v[3] = 1.0f; quadVerts[3].pos = cornerPos; }
                yoff += ch;
                cell++;
                wp++;
                col++;
                self->world->RenderTriStrip(4, quadVerts, 0);
            } while (col < self->cols);
        }
        x0 += cw;
        x1 += cw;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", func_00254C48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cWorld_254C48 {
public:
    char pad[0x10D8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void SetNewTextureData1(int a, char* p, int b, int c, int d, int e, int f, int g, int h);
};
extern "C" void* func_002548D0(void* self);

struct sMesh_254C48 {
    int f0;
    int f4;
    int f8;
    void* res;
    char* hdr;
    char** items;
    int f18;
    int rows;
    int cols;
    int widths[1];
};

/**
 * Draws a new movie frame: uploads the frame's pixels (@p res + 4 header, data as in func_00254410) into each tile's
 * texture (graphics manager slot 49, cPS2GraphicsMan_SetNewTextureData1) and draws the tiles (func_002548D0).
 *
 * Remembers @p res in +0xC when it is non-null, though it then reads it unconditionally. Vtable slot 4 of D_00480270.
 *
 * @param res  the frame
 */
extern "C" void func_00254C48(sMesh_254C48* self, void* res)
{
    if (res != 0) {
        self->res = res;
    }
    char* hdr = *(char**)((char*)res + 4);
    self->hdr = hdr;
    char* data;
    if (*(int*)(hdr + 0xC) & 0x1000) {
        data = hdr + *(int*)(hdr + 0x10);
    } else {
        data = hdr + 0x10;
    }
    int k = 0;
    short stride = *(short*)(self->hdr + 6);
    for (int i = 0; i < self->rows; i++) {
        int off = 0;
        for (int j = 0; j < self->cols; j++) {
            int* tileEntry = ((int**)self->items)[k];
            ((cWorld_254C48*)cGraphicsMan_TheGraphicsMan)->SetNewTextureData1(tileEntry[0], data + ((off << 6) + stride * (i << 6)), tileEntry[1], tileEntry[2], 0, 0, 0, 0, 0);
            k++;
            off += self->widths[j];
        }
    }
    func_002548D0(self);
}
#endif

extern "C" void* func_002548D0(void* self);

//99.29%
INCLUDE_ASM("mem/memstd", func_00254DA0__FPv);
#ifdef SKIP_ASM
/**
 * Redraws the last frame's tiles (func_002548D0). Vtable slot 5 of D_00480270.
 *
 * @return whatever func_002548D0 left in the return register
 */
void* func_00254DA0(void* self)
{
    return func_002548D0(self);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", SSX_VD_PS2_SINGLE_BUFFERD_CompleteDraw);
#ifdef SKIP_ASM
struct sMemStdVEntry {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void RCMP_DECODER_ReleaseFrame(void*, void*);

/**
 * Finishes the frame: waits until the renderer is idle (graphics manager slot 18, cPS2GraphicsMan_IsRenderIdle), then
 * hands the held frame (+0xC) back to the decoder (+0x0, RCMP_DECODER_ReleaseFrame) and clears it.
 *
 * Vtable slot 6 of D_00480270.
 */
extern "C" void SSX_VD_PS2_SINGLE_BUFFERD_CompleteDraw(SSX_VD_PS2_SINGLE_BUFFERD* self)
{
    void* rend;
    sMemStdVEntry* vt;
    do {
        rend = (void*)self->m_pGrMan;
        vt = *(sMemStdVEntry**)((char*)rend + 0x10D8);
    } while (vt[18].fn(vthis(rend, vt[18])) == 0);
    void* resource = self->mpCurrentFrame;
    if (resource != 0) {
        RCMP_DECODER_ReleaseFrame(self->m_pdecoder, resource);
        self->mpCurrentFrame = 0;
    }
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00254E20);
#ifdef SKIP_ASM
/**
 * Returns @p value unchanged, ignoring any further arguments (a variadic no-op; the next unit's copy of func_00253B18).
 *
 * @param value  the value returned
 * @return @p value
 */
extern "C" int func_00254E20(int value, ...)
{
    return value;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00254E60);
#ifdef SKIP_ASM
struct sV4_4E60 {
    float x, y, z, w;
    sV4_4E60(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

extern char D_004D28B8[], D_004D28C8[], D_004D28D8[], D_004D28E8[], D_004D28F8[], D_004D2908[], D_004D2918[], D_004D2928[], D_004D2938[], D_004D2948[], D_004D2958[], D_004D2968[], D_004D2978[], D_004D2988[], D_004D2998[], D_004D29A8[], D_004D29B8[], D_004D29C8[], D_004D29D8[], D_004D29E8[], D_004D29F8[], D_004D2A08[], D_004D2A18[], D_004D2A28[], D_004D2A38[], D_004D2A48[], D_004D2A58[], D_004D2A68[], D_004D2A78[], D_004D2A88[], D_004D2A98[], D_004D2AA8[], D_004D2AB8[], D_004D2AC8[], D_004D2AD8[], D_004D2AE8[], D_004D2AF8[], D_004D2B08[], D_004D2B18[], D_004D2B28[], D_004D2B38[], D_004D2B48[], D_004D2B58[], D_004D2B68[], D_004D2B78[], D_004D2B88[], D_004D2B98[], D_004D2BA8[], D_004D2BB8[], D_004D2BC8[], D_004D2BD8[], D_004D2BE8[], D_004D2BF8[], D_004D2C08[], D_004D2C18[], D_004D2C28[];

// PORT: the unit declares func_00254E60 as void*(int, int); it returns nothing.
extern "C" void func_00254E60_impl(int initialize, int priority) __asm__("func_00254E60");
/**
 * Static initialiser of the video driver's unit: when @p initialize is set and @p priority is 0xFFFF, constructs the
 * same 56 four-float constants as func_00253B58, into D_004D28B8..D_004D2C28.
 *
 * func_002557C0 calls it as the unit's global constructor.
 *
 * @param initialize  nonzero to construct
 * @param priority  init priority; only 0xFFFF does anything
 */
extern "C" void func_00254E60_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (D_004D28B8) sV4_4E60(1.0f, 1.0f, 1.0f, 0.5f);
        new (D_004D28C8) sV4_4E60(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (D_004D28D8) sV4_4E60(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (D_004D28E8) sV4_4E60(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (D_004D28F8) sV4_4E60(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (D_004D2908) sV4_4E60(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (D_004D2918) sV4_4E60(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (D_004D2928) sV4_4E60(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (D_004D2938) sV4_4E60(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (D_004D2948) sV4_4E60(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (D_004D2958) sV4_4E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2968) sV4_4E60(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D2978) sV4_4E60(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (D_004D2988) sV4_4E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2998) sV4_4E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D29A8) sV4_4E60(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D29B8) sV4_4E60(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D29C8) sV4_4E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D29D8) sV4_4E60(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D29E8) sV4_4E60(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D29F8) sV4_4E60(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D2A08) sV4_4E60(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (D_004D2A18) sV4_4E60(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (D_004D2A28) sV4_4E60(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (D_004D2A38) sV4_4E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2A48) sV4_4E60(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (D_004D2A58) sV4_4E60(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (D_004D2A68) sV4_4E60(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (D_004D2A78) sV4_4E60(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (D_004D2A88) sV4_4E60(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (D_004D2A98) sV4_4E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2AA8) sV4_4E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2AB8) sV4_4E60(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (D_004D2AC8) sV4_4E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2AD8) sV4_4E60(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (D_004D2AE8) sV4_4E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2AF8) sV4_4E60(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D2B08) sV4_4E60(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (D_004D2B18) sV4_4E60(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (D_004D2B28) sV4_4E60(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (D_004D2B38) sV4_4E60(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (D_004D2B48) sV4_4E60(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (D_004D2B58) sV4_4E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D2B68) sV4_4E60(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (D_004D2B78) sV4_4E60(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D2B88) sV4_4E60(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D2B98) sV4_4E60(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D2BA8) sV4_4E60(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (D_004D2BB8) sV4_4E60(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (D_004D2BC8) sV4_4E60(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (D_004D2BD8) sV4_4E60(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (D_004D2BE8) sV4_4E60(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (D_004D2BF8) sV4_4E60(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (D_004D2C08) sV4_4E60(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (D_004D2C18) sV4_4E60(1.0f, 0.0f, 1.0f, 0.0f);
        new (D_004D2C28) sV4_4E60(1.0f, 1.0f, 0.0f, 0.0f);
    }
}
#endif

//100%
INCLUDE_ASM("mem/memstd", VIDEO_DRIVER__VIDEO_DRIVER);
#ifdef SKIP_ASM
extern void* D_004802B0[];
void operator_delete(int*);

/**
 * Destroys a base video driver: installs the base vtable D_004802B0 and frees the object (operator_delete) when
 * @p flags bit 0 is set.
 *
 * @param flags  bit 0 frees the memory
 */
extern "C" void VIDEO_DRIVER__VIDEO_DRIVER(RCMP_VIDEO_DRIVER* self, int flags)
{
    self->mVtbl = D_004802B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("mem/memstd", VIDEO_DRIVER_CompleteDraw__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base video driver's default for vtable slot 6 of D_004802B0.
 */
void VIDEO_DRIVER_CompleteDraw(void* self)
{
}
#endif

extern "C" void* func_00254E60(int, int);

//100%
INCLUDE_ASM("mem/memstd", func_00255670);
#ifdef SKIP_ASM
/**
 * Allocates @p allocSize bytes through the movie allocator table (RCMP_rcmp_sys's allocate with name D_004A2EA0 and memory type
 * +0x8); the video driver's class operator new.
 *
 * The result is left in the return register.
 *
 * @param allocSize  bytes wanted
 */
extern "C" void func_00255670(int allocSize) {
    (*(int (**)(int *, int, int, int, int))((char*)((int*)&RCMP_rcmp_sys) + (0)))((int*)&D_004A2EA0, allocSize, 0, 0, (*(int *)((char*)((int*)&RCMP_rcmp_sys) + (8))));
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_002556B0);
#ifdef SKIP_ASM
/**
 * Allocates @p allocSize bytes through the movie allocator table, like func_00255670; the video driver's array operator new.
 *
 * @param allocSize  bytes wanted
 */
extern "C" void func_002556B0(int allocSize) {
    (*(int (**)(int *, int, int, int, int))((char*)((int*)&RCMP_rcmp_sys) + (0)))((int*)&D_004A2EA0, allocSize, 0, 0, (*(int *)((char*)((int*)&RCMP_rcmp_sys) + (8))));
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_002556F0);
#ifdef SKIP_ASM
/**
 * Allocates @p size bytes through the movie allocator table with the caller's name, alignment, header size and memory
 * type; the video driver's operator new with allocation arguments.
 *
 * @param size  bytes wanted
 * @param tag  the block's name
 * @param align  alignment
 * @param hdrSize  header bytes
 * @param flags  memory class and flags
 */
extern "C" void func_002556F0(int size, void* tag, int align, int hdrSize, int flags)
{
    ((tMemAlloc5_00254368)RCMP_rcmp_sys.alloc)(tag, size, align, hdrSize, flags);
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_00255720);
#ifdef SKIP_ASM
/**
 * Allocates @p size bytes through the movie allocator table with the caller's name, alignment, header size and memory
 * type; the array form of func_002556F0.
 *
 * @param size  bytes wanted
 * @param tag  the block's name
 * @param align  alignment
 * @param hdrSize  header bytes
 * @param flags  memory class and flags
 */
extern "C" void func_00255720(int size, void* tag, int align, int hdrSize, int flags)
{
    ((tMemAlloc5_00254368)RCMP_rcmp_sys.alloc)(tag, size, align, hdrSize, flags);
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_00255750);
#ifdef SKIP_ASM
/**
 * Frees @p self through the movie free hook (D_00509434); the video driver's operator delete.
 */
extern "C" void func_00255750(void* self)
{
    D_00509434[0](self);
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_00255778);
#ifdef SKIP_ASM
/**
 * Frees @p self through the movie free hook (D_00509434); the video driver's array operator delete.
 */
extern "C" void func_00255778(void* self)
{
    D_00509434[0](self);
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_002557A0);
#ifdef SKIP_ASM
/**
 * Returns @p value: the placement form of operator new (the size in @p self is ignored).
 *
 * @param value  the place to construct at
 * @return @p value
 */
extern "C" int func_002557A0(void* self, int value) {
    return value;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_002557A8);
#ifdef SKIP_ASM
/**
 * Does nothing: the placement form of operator delete.
 */
extern "C" void func_002557A8(void) {

}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_002557B0);
#ifdef SKIP_ASM
/**
 * Returns @p value: the array placement form of operator new.
 *
 * @param value  the place to construct at
 * @return @p value
 */
extern "C" int func_002557B0(void* self, int value) {
    return value;
}
#endif
//100%
INCLUDE_ASM("mem/memstd", func_002557B8);
#ifdef SKIP_ASM
/**
 * Does nothing: the array placement form of operator delete.
 */
extern "C" void func_002557B8(void) {

}
#endif

//99.38%
INCLUDE_ASM("mem/memstd", func_002557C0__FPv);
#ifdef SKIP_ASM
/**
 * Runs the video driver unit's static initialisation: func_00254E60(1, 0xFFFF).
 *
 * @return whatever func_00254E60 left in the return register
 */
void* func_002557C0(void* self)
{
    return func_00254E60(1, 0xffff);
}
#endif

extern void* cCommChannel_vtbl[];

//100%
INCLUDE_ASM("mem/memstd", cCommChannel_cCommChannel__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a comm channel: clears mChannelOpen and mError and installs cCommChannel_vtbl in mVtbl.
 *
 * @return @p self
 */
void* cCommChannel_cCommChannel(void* vself)
{
    cCommChannel* self = (cCommChannel*)vself;
    int zero = 0;
    *(int*)self = zero;
    self->mVtbl = (void**)((int)(void*)cCommChannel_vtbl);
    self->mError = zero;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cCommChannel__cCommChannel);
#ifdef SKIP_ASM
void operator_delete(int*);

/**
 * Destroys a comm channel: restores cCommChannel's vtable, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cCommChannel__cCommChannel(cCommChannel* self, int flags)
{
    self->mVtbl = cCommChannel_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cCommChannel_channelOpen__FPv);
#ifdef SKIP_ASM
/**
 * Opens the channel: clears mError and sets mChannelOpen to 1.
 *
 * @return 1
 */
int cCommChannel_channelOpen(void* vself)
{
    cCommChannel* self = (cCommChannel*)vself;
    int one = 1;
    self->mError = 0;
    *(int*)self = one;
    return one;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", cCommChannel_channelClose);
#ifdef SKIP_ASM
/**
 * Closes a comm channel: if it is open, clears mChannelOpen and mError.
 */
extern "C" void cCommChannel_channelClose(cCommChannel* self)
{
    if (*(int*)self != 0) {
        *(int*)self = 0;
        self->mError = 0;
    }
}
#endif

