#include "common.h"

extern unsigned int D_004A3E84;
extern const char D_004A3E88[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

//100%
INCLUDE_ASM("bx/memman", operator_new1__Fi);
#ifdef SKIP_ASM
void* operator_new1(int size)
{
    return cMemMan_alloc(size, D_004A3E88, D_004A3E84, 0);
}
#endif

//100%
INCLUDE_ASM("bx/memman", operator_new2__Fi);
#ifdef SKIP_ASM
void* operator_new2(int size)
{
    return cMemMan_alloc(size, D_004A3E88, D_004A3E84, 0);
}
#endif

//100%
INCLUDE_ASM("bx/memman", cMemMan_alloc);
#ifdef SKIP_ASM
extern void* cMemMan_m_mutex[16];
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern "C" void* func_00319A90(unsigned int size, unsigned int align, int flags, int top);

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
    void* p = func_00319A90(size, align, type, headersize);
    MUTEX_unlock(cMemMan_m_mutex);
    return p;
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
extern "C" void func_00319B48(void* ptr);

//100%
INCLUDE_ASM("bx/memman", operator_delete__FPi);
#ifdef SKIP_ASM
/**
 * Frees memory for operator delete while holding the memory-manager mutex.
 *
 * func_00319B48() ignores a null pointer and returns the block to the block manager or the dynamic heap,
 * whichever owns it. Same body as cMemMan_free().
 *
 * @param ptr  block to free, or 0
 */
void operator_delete(int* ptr)
{
    MUTEX_lock(cMemMan_m_mutex);
    func_00319B48(ptr);
    MUTEX_unlock(cMemMan_m_mutex);
}
#endif

//100%
INCLUDE_ASM("bx/memman", cMemMan_free__FPv);
#ifdef SKIP_ASM
/**
 * Frees a block while holding the memory-manager mutex.
 *
 * func_00319B48() ignores a null pointer and returns the block to the block manager or the dynamic heap,
 * whichever owns it. Same body as operator_delete().
 *
 * @param ptr  block to free, or 0
 */
void cMemMan_free(void* ptr)
{
    MUTEX_lock(cMemMan_m_mutex);
    func_00319B48(ptr);
    MUTEX_unlock(cMemMan_m_mutex);
}
#endif

//100%
INCLUDE_ASM("bx/memman", func_00317EE0);
#ifdef SKIP_ASM
extern "C" void func_00317EE0(void) {
    MUTEX_lock(cMemMan_m_mutex);
    MUTEX_unlock(cMemMan_m_mutex);
}
#endif
//100%
INCLUDE_ASM("bx/memman", func_00317F18);
#ifdef SKIP_ASM
extern "C" void func_00319CC8();

extern "C" void func_00317F18(void) {
    func_00319CC8();
}
#endif
