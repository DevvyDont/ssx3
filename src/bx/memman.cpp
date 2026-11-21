#include "common.h"

extern unsigned int newmbdefault;
extern const char str_default[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

//100%
INCLUDE_ASM("bx/memman", operator_new1__Fi);
#ifdef SKIP_ASM
/**
 * Allocates memory with the default allocation name and type.
 *
 * Calls cMemMan_alloc() with the name "default" (str_default), the type in newmbdefault (0 in the image) and no
 * header size. operator_new2() has the same body.
 *
 * @param size  bytes to allocate
 * @return the new block, or 0 if the allocation failed
 */
void* operator_new1(int size)
{
    return cMemMan_alloc(size, str_default, newmbdefault, 0);
}
#endif

//100%
INCLUDE_ASM("bx/memman", operator_new2__Fi);
#ifdef SKIP_ASM
/**
 * Allocates memory with the default allocation name and type; same body as operator_new1().
 *
 * Calls cMemMan_alloc() with the name "default" (str_default), the type in newmbdefault (0 in the image) and no
 * header size.
 *
 * @param size  bytes to allocate
 * @return the new block, or 0 if the allocation failed
 */
void* operator_new2(int size)
{
    return cMemMan_alloc(size, str_default, newmbdefault, 0);
}
#endif

//100%
INCLUDE_ASM("bx/memman", cMemMan_alloc);
#ifdef SKIP_ASM
extern void* cMemMan_m_mutex[16];
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern "C" void* cMemMan_internalAlloc(unsigned int size, unsigned int align, int flags, int top);

/**
 * Allocates a heap block, choosing its alignment from the allocation type, under the memory manager's mutex.
 *
 * Alignment: 4 << n when bits 24-27 of @p type hold a nonzero n; otherwise 128 when bit 0x400 is set, else 16
 * (8 for blocks under 16 bytes). cMemMan_internalAlloc() then serves small blocks (up to 0x200 bytes, @p headersize 0)
 * from the block manager and the rest from TheDynamicMemMan, and zero-fills the block when bit 31 of @p type is
 * set.
 *
 * @param size        bytes to allocate
 * @param name        allocation name (On Tour's operator new calls it name); not passed on to the allocator
 * @param type        allocation flags: alignment in bits 24-27, bit 0x400 for 128-byte alignment, bit 31 to
 *                    zero-fill the block
 * @param headersize  passed on as cMemMan_internalAlloc()'s fourth argument; nonzero bypasses the small-block pool and
 *                    uses the other dynamic-heap allocator (cDynamicMemMan_allocAlignHdr instead of cDynamicMemMan_allocAlign)
 * @return the new block, or 0 if the allocation failed
 */
// On Tour: operator new(unsigned int size, const char* name, int type, int headersize); its debug build keeps this
// branchless mask arithmetic statement by statement. The unit declares (int, const char*, unsigned int, int).
extern "C" void* cMemMan_alloc(int size, const char* name, unsigned int type, int headersize)
{
    int alignbits = (type & 0x0F000000) >> 24;
    int alignmask = (int)((type & 0x400) << 21) >> 31;
    int align = alignmask & 0x80;
    align |= (alignmask + 1) << (((size - 16) >> 31) + 4);
    alignmask = (alignbits - 1) >> 31;
    align &= alignmask;
    align |= (4 << alignbits) & ~alignmask;
    MUTEX_lock(cMemMan_m_mutex);
    void* block = cMemMan_internalAlloc(size, align, type, headersize);
    MUTEX_unlock(cMemMan_m_mutex);
    return block;
}
#endif

//100%
INCLUDE_ASM("bx/memman", operator_new__FUi);
#ifdef SKIP_ASM
// PORT: the target calls the 4-arg cMemMan_alloc with only $a0 set (a1..a3 left as-is).
void* cMemMan_alloc1(unsigned int size) __asm__("cMemMan_alloc");

/**
 * Allocates memory for operator new through the memory manager.
 *
 * @param size  number of bytes to allocate
 * @return the block from cMemMan_alloc1()
 */
void* operator_new(unsigned int size)
{
    return cMemMan_alloc1(size);
}
#endif

extern void* cMemMan_m_mutex[16];
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern "C" void cMemMan_internalFree(void* ptr);

//100%
INCLUDE_ASM("bx/memman", operator_delete__FPi);
#ifdef SKIP_ASM
/**
 * Frees memory for operator delete while holding the memory-manager mutex.
 *
 * cMemMan_internalFree() ignores a null pointer and returns the block to the block manager or the dynamic heap,
 * whichever owns it. Same body as cMemMan_free().
 *
 * @param ptr  block to free, or 0
 */
void operator_delete(int* ptr)
{
    MUTEX_lock(cMemMan_m_mutex);
    cMemMan_internalFree(ptr);
    MUTEX_unlock(cMemMan_m_mutex);
}
#endif

//100%
INCLUDE_ASM("bx/memman", cMemMan_free__FPv);
#ifdef SKIP_ASM
/**
 * Frees a block while holding the memory-manager mutex.
 *
 * cMemMan_internalFree() ignores a null pointer and returns the block to the block manager or the dynamic heap,
 * whichever owns it. Same body as operator_delete().
 *
 * @param ptr  block to free, or 0
 */
void cMemMan_free(void* ptr)
{
    MUTEX_lock(cMemMan_m_mutex);
    cMemMan_internalFree(ptr);
    MUTEX_unlock(cMemMan_m_mutex);
}
#endif

//100%
INCLUDE_ASM("bx/memman", func_00317EE0);
#ifdef SKIP_ASM
/**
 * Locks and immediately unlocks the memory-manager mutex (cMemMan_m_mutex); nothing runs in between.
 */
extern "C" void func_00317EE0(void) {
    MUTEX_lock(cMemMan_m_mutex);
    MUTEX_unlock(cMemMan_m_mutex);
}
#endif
//100%
INCLUDE_ASM("bx/memman", func_00317F18);
#ifdef SKIP_ASM
extern "C" void cMemMan_validate();

/**
 * Checks the memory manager's pools by calling cMemMan_validate.
 */
extern "C" void func_00317F18(void) {
    cMemMan_validate();
}
#endif
