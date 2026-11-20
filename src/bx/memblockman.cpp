#include "common.h"

//100%
INCLUDE_ASM("bx/memblockman", cMemMan_initialize);
#ifdef SKIP_ASM
extern void* cMemMan_m_mutex[16];
extern char TheDynamicMemMan[];
extern unsigned int cMemMan_m_heap;
extern unsigned int cMemMan_m_heapSize;
extern int cMemMan_m_initialized;
extern "C" void MUTEX_create(void* mutex);
extern "C" void cDynamicMemMan_init(void* heap, unsigned int base, unsigned int size);
extern "C" void cMemBlockMan_initialize(void);

/**
 * Sets up the memory manager on a heap region.
 *
 * Creates the manager mutex, records the heap base and size, initialises TheDynamicMemMan over the region
 * (cDynamicMemMan_init, which 16-byte aligns it and reserves a 0x380-byte header), marks the manager initialised and
 * then initialises the block manager.
 *
 * @param base  start address of the heap
 * @param size  heap size in bytes
 */
extern "C" void cMemMan_initialize(unsigned int base, unsigned int size)
{
    MUTEX_create(cMemMan_m_mutex);
    cMemMan_m_heap = base;
    cMemMan_m_heapSize = size;
    cDynamicMemMan_init(TheDynamicMemMan, base, size);
    cMemMan_m_initialized = 1;
    cMemBlockMan_initialize();
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", cMemMan_internalAlloc);
#ifdef SKIP_ASM
extern "C" void* cMemBlockMan_alloc(unsigned int size);
extern "C" void MEM_clear(void* p, unsigned int n);
extern char TheDynamicMemMan[];
// PORT: func_00320058__FPv / func_00320078__FPv take (heap, align, size); the defining unit declares 1 arg.
void* func_00320058_K2(void* heap, unsigned int align, unsigned int size) __asm__("func_00320058__FPv");
void* func_00320078_K2(void* heap, unsigned int align, unsigned int size) __asm__("func_00320078__FPv");

/**
 * Allocates memory from the fixed-size block pools or the dynamic heap.
 *
 * A request of 1-0x200 bytes with @p top clear uses cMemBlockMan_alloc (cMemBlockMan::alloc). Otherwise the alignment is
 * raised to at least 16 and TheDynamicMemMan allocates from the top (func_00320078) or bottom (func_00320058) of the
 * heap. A negative @p flags fills the new block with MEM_clear.
 *
 * @param size  bytes wanted
 * @param align  the alignment; at least 16 is used for heap blocks
 * @param flags  allocation flags; negative asks for the block to be filled
 * @param top  nonzero to allocate from the top of the heap
 * @return the block, or null
 */
extern "C" void* cMemMan_internalAlloc(unsigned int size, unsigned int align, int flags, int top)
{
    void* block;
    if (top == 0 && size != 0 && size <= 0x200) {
        block = cMemBlockMan_alloc(size);
    } else {
        if (align <= 15)
            align = 16;
        if (top != 0)
            block = func_00320078_K2(TheDynamicMemMan, align, size);
        else
            block = func_00320058_K2(TheDynamicMemMan, align, size);
    }
    if (block != 0 && flags < 0)
        MEM_clear(block, size);
    return block;
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", cMemMan_internalFree);
#ifdef SKIP_ASM
extern "C" int cMemBlockMan_isMemBlock(void* p);
extern "C" void cMemBlockMan_free(void* p);
extern char TheDynamicMemMan[];
// PORT: cDynamicMemMan_free__FPv is called with (list, item) here; bind the 2-arg form to that symbol.
int cDynamicMemMan_free_2(void* list, void* item) __asm__("cDynamicMemMan_free__FPv");

/**
 * Frees memory from either the block pools or the dynamic heap.
 *
 * Null is ignored. A block-pool address (cMemBlockMan_isMemBlock) goes to cMemBlockMan_free, anything else to cDynamicMemMan_free__FPv on
 * TheDynamicMemMan.
 *
 * @param block  the block to free, or null
 */
extern "C" void cMemMan_internalFree(void* block)
{
    if (block != 0)
    {
        if (cMemBlockMan_isMemBlock(block) != 0)
        {
            cMemBlockMan_free(block);
        }
        else
        {
            cDynamicMemMan_free_2(TheDynamicMemMan, block);
        }
    }
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_00319B98);
#ifdef SKIP_ASM
extern "C" int func_0031A130(void* self);
void func_0031FBB8(void* list, void* item);
extern char TheDynamicMemMan[];

extern "C" void func_00319B98(void* self)
{
    if (func_0031A130(self) == 0) {
        func_0031FBB8(TheDynamicMemMan, self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/memblockman", cMemMan_internalResizeBlock);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern "C" void* func_0041605C(void* dst, const void* src, int n);
extern void* cMemMan_m_mutex[16];
extern char str_resizeBlock[];
// PORT: operator_new__FUi takes (size, tag, flags, align) here.
void* operator_new_K2(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
// PORT: func_00319B98 returns the block size; the unit declares it void.
extern "C" unsigned int func_00319B98_K2(void* p) __asm__("func_00319B98");

static inline unsigned int cMemMan_blockSizeK2(void* p)
{
    MUTEX_lock(cMemMan_m_mutex);
    unsigned int n = func_00319B98_K2(p);
    MUTEX_unlock(cMemMan_m_mutex);
    return n;
}

static inline void cMemMan_freeBlockK2(void* p)
{
    MUTEX_lock(cMemMan_m_mutex);
    cMemMan_internalFree(p);
    MUTEX_unlock(cMemMan_m_mutex);
}

/**
 * Resizes a heap block by allocating a new one, copying the contents across and freeing the old block.
 *
 * The new block comes from operator new with the tag "resizeBlock" and flags 0x100. It copies the smaller of
 * @p size and the old block's size (func_0041605C, probably memcpy); the size query and the free each take the
 * memory manager's mutex.
 *
 * @param old   block to resize; freed on return
 * @param size  new size in bytes
 * @return the new block, which callers must use in place of @p old
 */
extern "C" void* cMemMan_internalResizeBlock(void* old, unsigned int size)
{
    void* newBlock = operator_new_K2(size, str_resizeBlock, 0x100, 0);
    func_0041605C(newBlock, old, size < cMemMan_blockSizeK2(old) ? size : cMemMan_blockSizeK2(old));
    cMemMan_freeBlockK2(old);
    return newBlock;
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", cMemMan_validate);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern "C" void cMemBlockMan_validate(void);
extern void* cMemMan_m_mutex[16];

/**
 * Checks the memory manager's pools for corruption.
 *
 * Locks cMemMan_m_mutex around cMemBlockMan_validate (cMemBlockMan::validate), whose per-pool checks are empty in this build.
 */
extern "C" void cMemMan_validate(void)
{
    MUTEX_lock(cMemMan_m_mutex);
    cMemBlockMan_validate();
    MUTEX_unlock(cMemMan_m_mutex);
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_00319D08);
#ifdef SKIP_ASM
extern "C" void func_00319D08(void) {

}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_00319D10__FPv);
#ifdef SKIP_ASM
void func_00319D10(void* self)
{
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_00319D18__FPv);
#ifdef SKIP_ASM
int func_00319D18(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", cMemBlockMan_initialize);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* func_003200E8(void* p);
void* func_00320120(void* p);
void* func_00320158(void* p);
void* func_00320190(void* p);
void* func_003201D8(void* p);
void* func_00320220(void* p);
void* func_00320270(void* p);
extern const char str_m_container8[];
extern const char str_m_container16[];
extern const char str_m_container32[];
extern const char str_m_container64[];
extern const char str_m_container128[];
extern const char str_m_container256[];
extern const char str_m_container512[];
extern void* cMemBlockMan_m_container8;
extern void* cMemBlockMan_m_container16;
extern void* cMemBlockMan_m_container32;
extern void* cMemBlockMan_m_container64;
extern void* cMemBlockMan_m_container128;
extern void* cMemBlockMan_m_container256;
extern void* cMemBlockMan_m_container512;
extern int cMemBlockMan_m_initialized;

/**
 * Creates the seven fixed-size block pools ("m_container8" to "m_container512") and marks the block manager
 * ready.
 */
extern "C" void cMemBlockMan_initialize(void)
{
    cMemBlockMan_m_container8 = func_003200E8(cMemMan_alloc(2052, str_m_container8, 0x1000000, 0));
    cMemBlockMan_m_container16 = func_00320120(cMemMan_alloc(8196, str_m_container16, 0x2000000, 0));
    cMemBlockMan_m_container32 = func_00320158(cMemMan_alloc(24580, str_m_container32, 0x3000000, 0));
    cMemBlockMan_m_container64 = func_00320190(cMemMan_alloc(0x32004, str_m_container64, 0x4000000, 0));
    cMemBlockMan_m_container128 = func_003201D8(cMemMan_alloc(0x25804, str_m_container128, 0x5000000, 0));
    cMemBlockMan_m_container256 = func_00320220(cMemMan_alloc(0x20004, str_m_container256, 0x6000000, 0));
    cMemBlockMan_m_container512 = func_00320270(cMemMan_alloc(0x20004, str_m_container512, 0x7000000, 0));
    cMemBlockMan_m_initialized = 1;
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", cMemBlockMan_alloc);
#ifdef SKIP_ASM
extern "C" void* cMemMan_internalAlloc(unsigned int size, unsigned int align, int flags, int top);
// PORT: the pool allocators are bound with their real (pool, align) arity; the
// symbols are named __FPv upstream.
void* func_003202C0_impl(void* pool, unsigned int align) __asm__("func_003202C0__FPv");
void* func_003202F8_impl(void* pool, unsigned int align) __asm__("func_003202F8__FPv");
void* func_00320330_impl(void* pool, unsigned int align) __asm__("func_00320330__FPv");
void* func_00320368_impl(void* pool, unsigned int align) __asm__("func_00320368__FPv");
void* func_003203B8_impl(void* pool, unsigned int align) __asm__("func_003203B8__FPv");
void* func_00320408_impl(void* pool, unsigned int align) __asm__("func_00320408__FPv");
void* func_00320448_impl(void* pool, unsigned int align) __asm__("func_00320448__FPv");
extern void* cMemBlockMan_m_container8;
extern void* cMemBlockMan_m_container16;
extern void* cMemBlockMan_m_container32;
extern void* cMemBlockMan_m_container64;
extern void* cMemBlockMan_m_container128;
extern void* cMemBlockMan_m_container256;
extern void* cMemBlockMan_m_container512;
extern int cMemBlockMan_m_initialized;

// PORT: binds the real 2-arg body; the unit declares a 1-arg prototype used by callers
extern "C" void* cMemBlockMan_alloc_impl(unsigned int size, unsigned int align) __asm__("cMemBlockMan_alloc");
/**
 * Allocates a small block from the fixed-size block pools, or from the heap when they cannot serve it.
 *
 * The size becomes max(@p size, @p align) rounded up to 4; once the pools exist (cMemBlockMan_m_initialized) the first
 * pool of 8, 16, 32, 64, 128, 256 or 512 bytes that fits serves it (cMemBlockMan_m_container8 ...). Larger requests, or
 * any before initialisation (as size 0x201), go to cMemMan_internalAlloc (cMemMan::internalAlloc) with at least 8-byte
 * alignment.
 *
 * @param size  bytes wanted
 * @param align  the alignment
 * @return the block
 */
extern "C" void* cMemBlockMan_alloc_impl(unsigned int size, unsigned int align)
{
    size = size > align ? size : align;
    if (cMemBlockMan_m_initialized) {
        size = size + (-(size & 3) & 3);
        if (size <= 8)
            return func_003202C0_impl(cMemBlockMan_m_container8, align);
        if (size <= 0x10)
            return func_003202F8_impl(cMemBlockMan_m_container16, align);
        if (size <= 0x20)
            return func_00320330_impl(cMemBlockMan_m_container32, align);
        if (size <= 0x40)
            return func_00320368_impl(cMemBlockMan_m_container64, align);
        if (size <= 0x80)
            return func_003203B8_impl(cMemBlockMan_m_container128, align);
        if (size <= 0x100)
            return func_00320408_impl(cMemBlockMan_m_container256, align);
        if (size <= 0x200)
            return func_00320448_impl(cMemBlockMan_m_container512, align);
    } else
        size = 513;
    return cMemMan_internalAlloc(size, align > 7 ? align : 8, 0, 0);
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", cMemBlockMan_free);
#ifdef SKIP_ASM
void func_00320488(void* pool, void** p);
void func_00320498(void* pool, void** p);
void func_003204A8(void* pool, void** p);
void func_003204B8(void* pool, void** p);
void func_003204D0(void* pool, void** p);
void func_003204E8(void* pool, void** p);
void func_00320500(void* pool, void** p);
extern void* cMemBlockMan_m_container8;
extern void* cMemBlockMan_m_container16;
extern void* cMemBlockMan_m_container32;
extern void* cMemBlockMan_m_container64;
extern void* cMemBlockMan_m_container128;
extern void* cMemBlockMan_m_container256;
extern void* cMemBlockMan_m_container512;

template <unsigned int N>
inline bool inPool_319F68(void* pool, void* p)
{
    return (unsigned int)((char*)p - (char*)pool) < N;
}

/**
 * Returns a block to the fixed-size pool it came from.
 *
 * Tests the address against each pool's range (8-byte pool 0x800 bytes, 16: 0x2000, 32: 0x6000, 64: 0x32000, 128:
 * 0x25800, 256 and 512: 0x20000) and calls that pool's free; an address in no pool is ignored.
 *
 * @param block  the block
 */
extern "C" void cMemBlockMan_free(void* block)
{
    if (inPool_319F68<0x800>(cMemBlockMan_m_container8, block))
        func_00320488(cMemBlockMan_m_container8, (void**)block);
    else if (inPool_319F68<0x2000>(cMemBlockMan_m_container16, block))
        func_00320498(cMemBlockMan_m_container16, (void**)block);
    else if (inPool_319F68<0x6000>(cMemBlockMan_m_container32, block))
        func_003204A8(cMemBlockMan_m_container32, (void**)block);
    else if (inPool_319F68<0x32000>(cMemBlockMan_m_container64, block))
        func_003204B8(cMemBlockMan_m_container64, (void**)block);
    else if (inPool_319F68<0x25800>(cMemBlockMan_m_container128, block))
        func_003204D0(cMemBlockMan_m_container128, (void**)block);
    else if (inPool_319F68<0x20000>(cMemBlockMan_m_container256, block))
        func_003204E8(cMemBlockMan_m_container256, (void**)block);
    else if (inPool_319F68<0x20000>(cMemBlockMan_m_container512, block))
        func_00320500(cMemBlockMan_m_container512, (void**)block);
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", cMemBlockMan_isMemBlock);
#ifdef SKIP_ASM
extern void* cMemBlockMan_m_container16;
extern void* cMemBlockMan_m_container512;
extern void* cMemBlockMan_m_container32;
extern void* cMemBlockMan_m_container8;
extern void* cMemBlockMan_m_container256;
extern void* cMemBlockMan_m_container64;
extern void* cMemBlockMan_m_container128;

static inline int cMemBlockMan_isMemBlock_in(void* p, void* base, unsigned int size)
{
    return (unsigned int)((char*)p - (char*)base) <= size - 1;
}

/**
 * Tells whether an address belongs to one of the fixed-size block pools.
 *
 * Same ranges as cMemBlockMan_free: 8-byte pool 0x800 bytes, 16: 0x2000, 32: 0x6000, 64: 0x32000, 128: 0x25800, 256 and
 * 512: 0x20000.
 *
 * @param block  the address
 * @return 1 when it is inside a pool
 */
extern "C" int cMemBlockMan_isMemBlock(void* block)
{
    if (cMemBlockMan_isMemBlock_in(block, cMemBlockMan_m_container8, 0x800))
        return 1;
    if (cMemBlockMan_isMemBlock_in(block, cMemBlockMan_m_container16, 0x2000))
        return 1;
    if (cMemBlockMan_isMemBlock_in(block, cMemBlockMan_m_container32, 0x6000))
        return 1;
    if (cMemBlockMan_isMemBlock_in(block, cMemBlockMan_m_container64, 0x32000))
        return 1;
    if (cMemBlockMan_isMemBlock_in(block, cMemBlockMan_m_container128, 0x25800))
        return 1;
    if (cMemBlockMan_isMemBlock_in(block, cMemBlockMan_m_container256, 0x20000))
        return 1;
    return cMemBlockMan_isMemBlock_in(block, cMemBlockMan_m_container512, 0x20000);
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031A130);
#ifdef SKIP_ASM
extern void* cMemBlockMan_m_container16;
extern void* cMemBlockMan_m_container512;
extern void* cMemBlockMan_m_container32;
extern void* cMemBlockMan_m_container8;
extern void* cMemBlockMan_m_container256;
extern void* cMemBlockMan_m_container64;
extern void* cMemBlockMan_m_container128;

static inline int func_0031A130_in(void* p, void* base, unsigned int size)
{
    return (unsigned int)((char*)p - (char*)base) <= size - 1;
}

/**
 * Returns the block size of the cMemBlockMan pool that holds an address, or 0 when no pool does.
 *
 * Checks the pools in turn: cMemBlockMan_m_container8 (0x800 bytes, 8-byte blocks), cMemBlockMan_m_container16
 * (0x2000, 16), cMemBlockMan_m_container32 (0x6000, 32), cMemBlockMan_m_container64 (0x32000, 64),
 * cMemBlockMan_m_container128 (0x25800, 128), cMemBlockMan_m_container256 (0x20000, 256) and
 * cMemBlockMan_m_container512 (0x20000, 512). The same range test as cMemBlockMan_isMemBlock, but it returns the size.
 * func_00319B98 uses it to tell pool blocks from heap blocks.
 *
 * @param addr  the address to look up
 * @return 8, 16, 32, 64, 128, 256 or 512 for a pool address, else 0
 */
extern "C" int func_0031A130(void* addr)
{
    if (func_0031A130_in(addr, cMemBlockMan_m_container8, 0x800))
        return 0x8;
    if (func_0031A130_in(addr, cMemBlockMan_m_container16, 0x2000))
        return 0x10;
    if (func_0031A130_in(addr, cMemBlockMan_m_container32, 0x6000))
        return 0x20;
    if (func_0031A130_in(addr, cMemBlockMan_m_container64, 0x32000))
        return 0x40;
    if (func_0031A130_in(addr, cMemBlockMan_m_container128, 0x25800))
        return 0x80;
    if (func_0031A130_in(addr, cMemBlockMan_m_container256, 0x20000))
        return 0x100;
    if (func_0031A130_in(addr, cMemBlockMan_m_container512, 0x20000))
        return 0x200;
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", cMemBlockMan_validate);
#ifdef SKIP_ASM
extern int cMemBlockMan_m_initialized;
extern void* cMemBlockMan_m_container16;
extern void* cMemBlockMan_m_container512;
extern void* cMemBlockMan_m_container32;
extern void* cMemBlockMan_m_container8;
extern void* cMemBlockMan_m_container256;
extern void* cMemBlockMan_m_container64;
extern void* cMemBlockMan_m_container128;
// PORT: these empty stubs are defined as f(void), but this caller passes one argument.
void func_00320518_1(void*) __asm__("func_00320518__Fv");
void func_00320520_1(void*) __asm__("func_00320520__Fv");
void func_00320528_1(void*) __asm__("func_00320528__Fv");
void func_00320530_1(void*) __asm__("func_00320530__Fv");
void func_00320538_1(void*) __asm__("func_00320538__Fv");
void func_00320540_1(void*) __asm__("func_00320540__Fv");
void func_00320548_1(void*) __asm__("func_00320548__Fv");

/**
 * Checks all fixed-size block pools for corruption.
 *
 * When cMemBlockMan_m_initialized is set, calls the per-pool validate functions func_00320518 ... func_00320548 on the
 * 8- to 512-byte pools; they are empty in this build.
 */
extern "C" void cMemBlockMan_validate(void)
{
    if (cMemBlockMan_m_initialized != 0) {
        func_00320518_1(cMemBlockMan_m_container8);
        func_00320520_1(cMemBlockMan_m_container16);
        func_00320528_1(cMemBlockMan_m_container32);
        func_00320530_1(cMemBlockMan_m_container64);
        func_00320538_1(cMemBlockMan_m_container128);
        func_00320540_1(cMemBlockMan_m_container256);
        func_00320548_1(cMemBlockMan_m_container512);
    }
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031A258);
#ifdef SKIP_ASM
/**
 * Returns 0.
 *
 * @return 0
 */
extern "C" int func_0031A258(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("bx/memblockman", func_0031A260);
#ifdef SKIP_ASM
/**
 * Does nothing.
 */
extern "C" void func_0031A260(void) {

}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031A268);
#ifdef SKIP_ASM
/**
 * Recreates a saved VU0 MAC flag value by running a VU0 multiply whose result raises the same flags.
 *
 * For each of the four components, from the low flag bits upwards (@p flags is shifted left once per component), picks
 * a value: when neither the overflow nor the underflow bit (mask 0x8800) is set, 0.0 if the zero bit (8) is set, else
 * 1.0; otherwise 1e-30 if the zero bit is set (underflow) or 1e30 if not (overflow). The second vector gets the same
 * value, negated when the sign bit (0x80) is set. Then multiplies the two vectors in VF1/VF2 (vmul), keeping the old
 * VF1 and VF2 in $8/$9 and putting them back afterwards. func_0031A3C0 calls it with the MAC flags saved at +0x200 of
 * a func_0031A308 buffer.
 *
 * @param flags  the MAC flag word to reproduce
 */
// PORT: PS2-only VU0 FPU-flag probe: builds two float4s from flag bits and multiplies them on VU0.
extern "C" void func_0031A268(int flags)
{
    float lhs[4] __attribute__((aligned(16)));
    float rhs[4] __attribute__((aligned(16)));
    int i;
    for (i = 0; i < 4; i++) {
        float component;
        if ((flags & 0x8800) == 0) {
            component = 0.0f;
            if (!(flags & 8))
                component = 1.0f;
        } else {
            component = 1.0000000031710769e-30f;
            if (!(flags & 8))
                component = 1.0000000150474662e+30f;
        }
        lhs[i] = component;
        if (flags & 0x80)
            component = -component;
        rhs[i] = component;
        flags <<= 1;
    }
    __asm__ __volatile__(
        "qmfc2.ni   $8, $vf1\n"
        "qmfc2.ni   $9, $vf2\n"
        "lqc2       $vf1, %0\n"
        "lqc2       $vf2, %1\n"
        "vmul.xyzw  $vf1, $vf1, $vf2\n"
        "qmtc2.ni   $8, $vf1\n"
        "qmtc2.ni   $9, $vf2\n"
        "vnop\n"
        :
        : "m"(lhs[0]), "m"(rhs[0])
        : "$8", "$9");
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031A308);
#ifdef SKIP_ASM
/**
 * Saves the VU0 register state to a 0x210-byte buffer.
 *
 * Stores VF1-VF31 at +0x0..+0x1E0, the accumulator at +0x1F0 (read out through VF1 with vmaddx after VF1 is saved),
 * the MAC flags ($vi17) at +0x200, the status flags ($vi16) at +0x204, Q ($vi22, after vwaitq) at +0x208 and the clip
 * flags ($vi18) at +0x20C. func_0031A3C0 restores it. func_0031A6D8 and func_0031AA18 save into the PS2 execution
 * manager's buffer at +0x14080.
 *
 * @param buf  the 0x210-byte, 16-byte aligned save buffer
 */
// PORT: PS2-only VU0 state save (VF1-VF31, MAC/status/Q/clip flags, ACC) to a 0x210-byte buffer.
extern "C" void func_0031A308(void* buf)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "daddu      $8, %0, $0\n"
        "cfc2.i     $9, $vi17\n"
        "sqc2       $vf1, 0x0($8)\n"
        "sqc2       $vf2, 0x10($8)\n"
        "sw         $9, 0x200($8)\n"
        "sqc2       $vf3, 0x20($8)\n"
        "sqc2       $vf4, 0x30($8)\n"
        "cfc2.ni    $9, $vi16\n"
        "sqc2       $vf5, 0x40($8)\n"
        "sqc2       $vf6, 0x50($8)\n"
        "sw         $9, 0x204($8)\n"
        "vwaitq\n"
        "cfc2.ni    $9, $vi22\n"
        "sw         $9, 0x208($8)\n"
        "cfc2.ni    $9, $vi18\n"
        "sw         $9, 0x20C($8)\n"
        "sqc2       $vf7, 0x60($8)\n"
        "sqc2       $vf8, 0x70($8)\n"
        "vmaddx.xyzw $vf1, $vf0, $vf0x\n"
        "sqc2       $vf9, 0x80($8)\n"
        "sqc2       $vf10, 0x90($8)\n"
        "sqc2       $vf11, 0xA0($8)\n"
        "sqc2       $vf12, 0xB0($8)\n"
        "sqc2       $vf13, 0xC0($8)\n"
        "sqc2       $vf14, 0xD0($8)\n"
        "sqc2       $vf15, 0xE0($8)\n"
        "sqc2       $vf16, 0xF0($8)\n"
        "sqc2       $vf17, 0x100($8)\n"
        "sqc2       $vf18, 0x110($8)\n"
        "sqc2       $vf19, 0x120($8)\n"
        "sqc2       $vf20, 0x130($8)\n"
        "sqc2       $vf21, 0x140($8)\n"
        "sqc2       $vf22, 0x150($8)\n"
        "sqc2       $vf23, 0x160($8)\n"
        "sqc2       $vf24, 0x170($8)\n"
        "sqc2       $vf25, 0x180($8)\n"
        "sqc2       $vf26, 0x190($8)\n"
        "sqc2       $vf27, 0x1A0($8)\n"
        "sqc2       $vf28, 0x1B0($8)\n"
        "sqc2       $vf29, 0x1C0($8)\n"
        "sqc2       $vf30, 0x1D0($8)\n"
        "sqc2       $vf31, 0x1E0($8)\n"
        "sqc2       $vf1, 0x1F0($8)\n"
        ".set pop\n"
        :
        : "r"(buf)
        : "$8", "$9", "memory");
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/memblockman", func_0031A3C0);
#ifdef SKIP_ASM
extern "C" void func_0031A268(int status);

/**
 * Restores the VU0 register state that func_0031A308 saved.
 *
 * Loads Q (+0x208) and the clip flags (+0x20C), the accumulator (+0x1F0, through VF1 and vaddax), VF1-VF6, then
 * recreates the MAC flags from +0x200 with func_0031A268, and finally loads VF7-VF31 and the status flags (+0x204).
 *
 * @param buf  the 0x210-byte save buffer
 */
// PORT: PS2-only VU0 state restore (inverse of func_0031A308): VF1-VF31, flags, ACC.
extern "C" void func_0031A3C0(void* buf)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "daddu      $8, %0, $0\n"
        "lw         $9, 0x208($8)\n"
        "ctc2.ni    $9, $vi22\n"
        "lw         $9, 0x20C($8)\n"
        "ctc2.ni    $9, $vi18\n"
        "lqc2       $vf1, 0x1F0($8)\n"
        "vaddax.xyzw ACC, $vf1, $vf0x\n"
        "lqc2       $vf1, 0x0($8)\n"
        "lqc2       $vf2, 0x10($8)\n"
        "lqc2       $vf3, 0x20($8)\n"
        "lqc2       $vf4, 0x30($8)\n"
        "lqc2       $vf5, 0x40($8)\n"
        "lqc2       $vf6, 0x50($8)\n"
        ".set pop\n"
        :
        : "r"(buf)
        : "$8", "$9", "memory");
    func_0031A268(*(int*)((char*)buf + 0x200));
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "daddu      $8, %0, $0\n"
        "lqc2       $vf7, 0x60($8)\n"
        "lqc2       $vf8, 0x70($8)\n"
        "lqc2       $vf9, 0x80($8)\n"
        "lqc2       $vf10, 0x90($8)\n"
        "lqc2       $vf11, 0xA0($8)\n"
        "lqc2       $vf12, 0xB0($8)\n"
        "lw         $9, 0x204($8)\n"
        "lqc2       $vf13, 0xC0($8)\n"
        "lqc2       $vf14, 0xD0($8)\n"
        "ctc2.ni    $9, $vi16\n"
        "lqc2       $vf15, 0xE0($8)\n"
        "lqc2       $vf16, 0xF0($8)\n"
        "lqc2       $vf17, 0x100($8)\n"
        "lqc2       $vf18, 0x110($8)\n"
        "lqc2       $vf19, 0x120($8)\n"
        "lqc2       $vf20, 0x130($8)\n"
        "lqc2       $vf21, 0x140($8)\n"
        "lqc2       $vf22, 0x150($8)\n"
        "lqc2       $vf23, 0x160($8)\n"
        "lqc2       $vf24, 0x170($8)\n"
        "lqc2       $vf25, 0x180($8)\n"
        "lqc2       $vf26, 0x190($8)\n"
        "lqc2       $vf27, 0x1A0($8)\n"
        "lqc2       $vf28, 0x1B0($8)\n"
        "lqc2       $vf29, 0x1C0($8)\n"
        "lqc2       $vf30, 0x1D0($8)\n"
        "lqc2       $vf31, 0x1E0($8)\n"
        ".set pop\n"
        :
        : "r"(buf)
        : "$8", "$9", "memory");
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031A490);
#ifdef SKIP_ASM
/**
 * Calls an interrupt callback with all 32 FPU registers preserved, then re-enables interrupts.
 *
 * func_0031A6D8 installs it as the INTC 3 handler (func_00423A90) with func_0031ABD0 as its argument. Saves $f0-$f31
 * on the stack, calls @p handler with @p handlerArg, restores the registers and runs `sync; ei`.
 *
 * @param handlerArg  the value passed to @p handler
 * @param handler  the callback to run
 * @return 1
 */
// PORT: PS2-only handler trampoline: saves/restores all 32 FPU registers around the
// handler call and re-enables interrupts (sync; ei). Needs a native replacement off-PS2.
extern "C" int func_0031A490(int handlerArg, void (*handler)(int))
{
    int save[32];
    __asm__ __volatile__(
        "mfc1 $2, $f0\n\tsw $2, %0\n\t"
        "mfc1 $2, $f1\n\tsw $2, %1\n\t"
        "mfc1 $2, $f2\n\tsw $2, %2\n\t"
        "mfc1 $2, $f3\n\tsw $2, %3\n\t"
        "mfc1 $2, $f4\n\tsw $2, %4\n\t"
        "mfc1 $2, $f5\n\tsw $2, %5\n\t"
        "mfc1 $2, $f6\n\tsw $2, %6\n\t"
        "mfc1 $2, $f7\n\tsw $2, %7\n\t"
        : "=m"(save[0]), "=m"(save[1]), "=m"(save[2]), "=m"(save[3]), "=m"(save[4]), "=m"(save[5]), "=m"(save[6]), "=m"(save[7])
        :
        : "$2");
    __asm__ __volatile__(
        "mfc1 $2, $f8\n\tsw $2, %0\n\t"
        "mfc1 $2, $f9\n\tsw $2, %1\n\t"
        "mfc1 $2, $f10\n\tsw $2, %2\n\t"
        "mfc1 $2, $f11\n\tsw $2, %3\n\t"
        "mfc1 $2, $f12\n\tsw $2, %4\n\t"
        "mfc1 $2, $f13\n\tsw $2, %5\n\t"
        "mfc1 $2, $f14\n\tsw $2, %6\n\t"
        "mfc1 $2, $f15\n\tsw $2, %7\n\t"
        : "=m"(save[8]), "=m"(save[9]), "=m"(save[10]), "=m"(save[11]), "=m"(save[12]), "=m"(save[13]), "=m"(save[14]), "=m"(save[15])
        :
        : "$2");
    __asm__ __volatile__(
        "mfc1 $2, $f16\n\tsw $2, %0\n\t"
        "mfc1 $2, $f17\n\tsw $2, %1\n\t"
        "mfc1 $2, $f18\n\tsw $2, %2\n\t"
        "mfc1 $2, $f19\n\tsw $2, %3\n\t"
        "mfc1 $2, $f20\n\tsw $2, %4\n\t"
        "mfc1 $2, $f21\n\tsw $2, %5\n\t"
        "mfc1 $2, $f22\n\tsw $2, %6\n\t"
        "mfc1 $2, $f23\n\tsw $2, %7\n\t"
        : "=m"(save[16]), "=m"(save[17]), "=m"(save[18]), "=m"(save[19]), "=m"(save[20]), "=m"(save[21]), "=m"(save[22]), "=m"(save[23])
        :
        : "$2");
    __asm__ __volatile__(
        "mfc1 $2, $f24\n\tsw $2, %0\n\t"
        "mfc1 $2, $f25\n\tsw $2, %1\n\t"
        "mfc1 $2, $f26\n\tsw $2, %2\n\t"
        "mfc1 $2, $f27\n\tsw $2, %3\n\t"
        "mfc1 $2, $f28\n\tsw $2, %4\n\t"
        "mfc1 $2, $f29\n\tsw $2, %5\n\t"
        "mfc1 $2, $f30\n\tsw $2, %6\n\t"
        "mfc1 $2, $f31\n\tsw $2, %7\n\t"
        : "=m"(save[24]), "=m"(save[25]), "=m"(save[26]), "=m"(save[27]), "=m"(save[28]), "=m"(save[29]), "=m"(save[30]), "=m"(save[31])
        :
        : "$2");
    handler(handlerArg);
    __asm__ __volatile__(
        "lw $2, %0\n\tmtc1 $2, $f0\n\t"
        "lw $2, %1\n\tmtc1 $2, $f1\n\t"
        "lw $2, %2\n\tmtc1 $2, $f2\n\t"
        "lw $2, %3\n\tmtc1 $2, $f3\n\t"
        "lw $2, %4\n\tmtc1 $2, $f4\n\t"
        "lw $2, %5\n\tmtc1 $2, $f5\n\t"
        "lw $2, %6\n\tmtc1 $2, $f6\n\t"
        "lw $2, %7\n\tmtc1 $2, $f7\n\t"
        :
        : "m"(save[0]), "m"(save[1]), "m"(save[2]), "m"(save[3]), "m"(save[4]), "m"(save[5]), "m"(save[6]), "m"(save[7])
        : "$2");
    __asm__ __volatile__(
        "lw $2, %0\n\tmtc1 $2, $f8\n\t"
        "lw $2, %1\n\tmtc1 $2, $f9\n\t"
        "lw $2, %2\n\tmtc1 $2, $f10\n\t"
        "lw $2, %3\n\tmtc1 $2, $f11\n\t"
        "lw $2, %4\n\tmtc1 $2, $f12\n\t"
        "lw $2, %5\n\tmtc1 $2, $f13\n\t"
        "lw $2, %6\n\tmtc1 $2, $f14\n\t"
        "lw $2, %7\n\tmtc1 $2, $f15\n\t"
        :
        : "m"(save[8]), "m"(save[9]), "m"(save[10]), "m"(save[11]), "m"(save[12]), "m"(save[13]), "m"(save[14]), "m"(save[15])
        : "$2");
    __asm__ __volatile__(
        "lw $2, %0\n\tmtc1 $2, $f16\n\t"
        "lw $2, %1\n\tmtc1 $2, $f17\n\t"
        "lw $2, %2\n\tmtc1 $2, $f18\n\t"
        "lw $2, %3\n\tmtc1 $2, $f19\n\t"
        "lw $2, %4\n\tmtc1 $2, $f20\n\t"
        "lw $2, %5\n\tmtc1 $2, $f21\n\t"
        "lw $2, %6\n\tmtc1 $2, $f22\n\t"
        "lw $2, %7\n\tmtc1 $2, $f23\n\t"
        :
        : "m"(save[16]), "m"(save[17]), "m"(save[18]), "m"(save[19]), "m"(save[20]), "m"(save[21]), "m"(save[22]), "m"(save[23])
        : "$2");
    __asm__ __volatile__(
        "lw $2, %0\n\tmtc1 $2, $f24\n\t"
        "lw $2, %1\n\tmtc1 $2, $f25\n\t"
        "lw $2, %2\n\tmtc1 $2, $f26\n\t"
        "lw $2, %3\n\tmtc1 $2, $f27\n\t"
        "lw $2, %4\n\tmtc1 $2, $f28\n\t"
        "lw $2, %5\n\tmtc1 $2, $f29\n\t"
        "lw $2, %6\n\tmtc1 $2, $f30\n\t"
        "lw $2, %7\n\tmtc1 $2, $f31\n\t"
        :
        : "m"(save[24]), "m"(save[25]), "m"(save[26]), "m"(save[27]), "m"(save[28]), "m"(save[29]), "m"(save[30]), "m"(save[31])
        : "$2");
    __asm__ __volatile__("sync\n\tei");
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031A6B8);
#ifdef SKIP_ASM
extern int D_004A3ECC;
extern "C" void ChangeThreadPriority(int, int);

/**
 * Sets the PS2 execution manager's loader thread (D_004A3ECC) to priority 101 with ChangeThreadPriority.
 *
 * func_0031A6D8 creates that thread at the main thread's priority + 1 and stores its id in D_004A3ECC;
 * cSSXApp_initload calls this afterwards.
 */
extern "C" void func_0031A6B8(void)
{
    ChangeThreadPriority(D_004A3ECC, 101);
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031A6D8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// 100.00 objdiff; blocked only by the symbol name: needs 0x4A30F0 named _gp (linker symbol in SLUS_207.72.ld) instead of D_004A30F0.
// gas never makes '_gp' gp-relative, gcc treats 'extern int _gp' as small data (one unsplit la insn -> cse'd, hoisted, no tie).
extern "C" void func_0031A308(void* p);
extern "C" void func_0031ABD0();
extern "C" void func_0031AC08();
extern "C" void func_0031AC60();
extern "C" int func_00416210(void* p, int c, int n);
extern "C" int GetThreadId();
extern "C" int ReferThreadStatus(int id, void* st);
extern "C" int CreateSema(void* sema);
extern "C" int CreateThread(void* tp);
extern "C" void StartThread(int id, void* arg);
extern "C" void func_00424880(int n);
extern "C" int func_00423A90(int n, void (*a)(), int b, void (*c)());
extern "C" int func_004248E8(int n);
extern int _gp;     // linker-defined $gp base (0x4A30F0; splat names it D_004A30F0)
extern int D_004A3ECC;

struct sMgr_31A6D8 {
    char pad0[0xC];
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    volatile int f20;
    char pad24[0xC];
    char stack2[0x4000];
    int f4030;
    int f4034;
    int f4038;
    int pad403C;
    char stack1[0x10000];
    int f14040;
    int f14044;
    int f14048;
    int pad1404C;
    char pad14050[0x30];
    char vu0[0x210];
    int f14290;
};
// Sony EE kernel ThreadParam / SemaParam.
struct sThreadParam_31A6D8 {
    int status;
    void (*entry)();
    void* stack;
    int stackSize;
    void* gpReg;
    int initPriority;
    int currentPriority;
    unsigned int attr;
    unsigned int option;
    int waitType;
    int waitId;
    int wakeupCount;
};
struct sSemaParam_31A6D8 {
    int count;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};

/**
 * Starts the PS2 execution manager: its semaphores, its loader and timer threads and its INTC 3 interrupt handler.
 *
 * Clears the quit flag (+0x20), saves the current VU0 state into the buffer at +0x14080 (func_0031A308), records the
 * calling (main) thread's id (+0x10) and its current priority (+0x14, from ReferThreadStatus). Creates the halt
 * semaphore (+0xC) and the sync semaphore (+0x18, its waiting flag +0x1C cleared), and a counting semaphore (max
 * 0x400) for loader jobs (+0x14044, pending job +0x14048 cleared). Creates and starts the loader thread
 * (func_0031AC60, 0x10000-byte stack at +0x4040, priority main + 1; id at +0x14040 and in D_004A3ECC), clears the
 * halt-request flag (+0x4038), creates the timer counting semaphore (+0x4034, max 0x400) and starts the timer thread
 * (func_0031AC08, 0x4000-byte stack at +0x30, priority main - 1; id at +0x4030). Both threads get @p self as their
 * argument and _gp as their $gp. Finally, between func_00424880(3) and func_004248E8(3), installs func_0031A490 as
 * the INTC 3 handler with func_00423A90, passing func_0031ABD0 as its argument; the handler id goes to +0x14290.
 */
extern "C" void func_0031A6D8(sMgr_31A6D8* self)
{
    sThreadParam_31A6D8 st;
    sSemaParam_31A6D8 s1;
    sSemaParam_31A6D8 s2;
    sSemaParam_31A6D8 s3;
    sThreadParam_31A6D8 tp;
    self->f20 = 0;
    func_0031A308(self->vu0);
    self->f10 = GetThreadId();
    func_00416210(&st, 0, 0x30);
    ReferThreadStatus(self->f10, &st);
    self->f14 = st.currentPriority;
    func_00416210(&s1, 0, 0x18);
    self->fC = CreateSema(&s1);
    func_00416210(&s2, 0, 0x18);
    self->f18 = CreateSema(&s2);
    self->f1C = 0;
    func_00416210(&s3, 0, 0x18);
    s3.maxCount = 0x400;
    self->f14044 = CreateSema(&s3);
    self->f14048 = 0;
    func_00416210(&tp, 0, 0x30);
    tp.status = 0x10;
    tp.entry = func_0031AC60;
    tp.stack = self->stack1;
    tp.stackSize = 0x10000;
    tp.initPriority = self->f14 + 1;
    tp.gpReg = &_gp;
    tp.currentPriority = self->f14 + 1;
    tp.attr = 0;
    tp.option = 0;
    self->f14040 = CreateThread(&tp);
    D_004A3ECC = self->f14040;
    StartThread(self->f14040, self);
    self->f4038 = 0;
    func_00416210(&s3, 0, 0x18);
    s3.maxCount = 0x400;
    self->f4034 = CreateSema(&s3);
    func_00416210(&tp, 0, 0x30);
    tp.status = 0x10;
    tp.entry = func_0031AC08;
    tp.stack = self->stack2;
    tp.stackSize = 0x4000;
    tp.gpReg = &_gp;
    tp.initPriority = self->f14 - 1;
    tp.currentPriority = self->f14 - 1;
    tp.attr = 0;
    tp.option = 0;
    self->f4030 = CreateThread(&tp);
    StartThread(self->f4030, self);
    func_00424880(3);
    self->f14290 = func_00423A90(3, (void (*)())func_0031A490, 0, func_0031ABD0);
    func_004248E8(3);
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031A920);
#ifdef SKIP_ASM
extern "C" void func_00424880(int);
extern "C" void RemoveIntcHandler(int, int);
extern "C" int func_004248E8(int);
extern "C" void SignalSema(int);
extern "C" void SleepThread();
extern "C" void TerminateThread(int);
extern "C" void DeleteThread(int);
extern "C" void DeleteSema(int);

/**
 * Shuts the PS2 execution manager down, undoing func_0031A6D8.
 *
 * Between func_00424880(3) and func_004248E8(3) removes the INTC 3 handler (id at +0x14290). Sets the quit flag
 * (+0x20) and signals the loader semaphore (+0x14044) and the timer semaphore (+0x4034) so both threads see it, then
 * sleeps twice (each thread wakes this one, the main thread, on its way out). Terminates and deletes the timer
 * thread (+0x4030) and the loader thread (+0x14040), then deletes the timer, loader, sync (+0x18) and halt (+0xC)
 * semaphores.
 */
extern "C" void func_0031A920(void* self)
{
    func_00424880(3);
    RemoveIntcHandler(3, *(int*)((char*)self + 0x14290));
    func_004248E8(3);
    *(volatile int*)((char*)self + 0x20) = 1;
    SignalSema(*(int*)((char*)self + 0x14044));
    SignalSema(*(int*)((char*)self + 0x4034));
    SleepThread();
    SleepThread();
    TerminateThread(*(int*)((char*)self + 0x4030));
    DeleteThread(*(int*)((char*)self + 0x4030));
    TerminateThread(*(int*)((char*)self + 0x14040));
    DeleteThread(*(int*)((char*)self + 0x14040));
    DeleteSema(*(int*)((char*)self + 0x4034));
    DeleteSema(*(int*)((char*)self + 0x14044));
    DeleteSema(*(int*)((char*)self + 0x18));
    DeleteSema(*(int*)((char*)self + 0xC));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/memblockman", func_0031A9D8);
#ifdef SKIP_ASM
extern "C" void func_0031A3C0(void* buf);

/**
 * Restores the VU0 state saved at +0x14080 (func_0031A3C0) when a loader job is pending (+0x14048).
 *
 * Slot 11 of the PS2 execution manager vtable D_0048DB68 (cExecutionMan_prepause in the base vtable); func_0031AA58
 * calls it before it blocks on the sync semaphore.
 */
extern "C" void func_0031A9D8(void* self)
{
    if (*(int*)((char*)self + 0x14048) != 0) {
        func_0031A3C0((char*)self + 0x14080);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/memblockman", func_0031AA18);
#ifdef SKIP_ASM
/**
 * Saves the VU0 state into the buffer at +0x14080 (func_0031A308) when a loader job is pending (+0x14048).
 *
 * Slot 12 of the PS2 execution manager vtable D_0048DB68 (cExecutionMan_postpause in the base vtable); func_0031AA58
 * calls it after its wait on the sync semaphore ends.
 */
extern "C" void func_0031AA18(void* self)
{
    if (*(int*)((char*)self + 0x14048) != 0) {
        func_0031A308((char*)self + 0x14080);
    }
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031AA58);
#ifdef SKIP_ASM
extern "C" void* WaitSema(int);
extern "C" int PollSema(int);

struct sMemBlkVEntryA {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sMemBlkVEntryB {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Blocks the calling thread until func_0031AAC8 signals the sync semaphore, wrapped in the pause hooks.
 *
 * Sets the waiting flag (+0x1C), calls vtable slot 11 (func_0031A9D8 in this class), waits on the sync semaphore
 * (+0x18), clears the waiting flag, drains any extra signals with PollSema (it loops while PollSema returns the
 * semaphore id), and calls vtable slot 12 (func_0031AA18).
 */
extern "C" void func_0031AA58(void* self)
{
    *(int*)((char*)self + 0x1C) = 1;
    sMemBlkVEntryA* vt = *(sMemBlkVEntryA**)((char*)self + 0x8);
    vt[11].fn((char*)self + vt[11].delta);
    WaitSema(*(int*)((char*)self + 0x18));
    *(int*)((char*)self + 0x1C) = 0;
    while (PollSema(*(int*)((char*)self + 0x18)) == *(int*)((char*)self + 0x18))
    {
    }
    sMemBlkVEntryB* vt2 = *(sMemBlkVEntryB**)((char*)self + 0x8);
    vt2[12].fn((char*)self + vt2[12].delta);
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031AAC8);
#ifdef SKIP_ASM
extern "C" void SignalSema(int);

/**
 * Wakes a thread blocked in func_0031AA58: signals the sync semaphore (+0x18) when the waiting flag (+0x1C) is set.
 */
extern "C" void func_0031AAC8(void* self)
{
    if (*(int*)((char*)self + 0x1C) != 0) {
        SignalSema(*(int*)((char*)self + 0x18));
    }
}
#endif

extern "C" void* WaitSema(int);

//100%
INCLUDE_ASM("bx/memblockman", func_0031AAF0__FPv);
#ifdef SKIP_ASM
/**
 * Waits on the PS2 execution manager's halt semaphore (+0xC), which func_0031AB10 signals.
 *
 * @return WaitSema's result
 */
void* func_0031AAF0(void* self)
{
    return WaitSema(*(int*)((char*)self + 0xc));
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031AB10);
#ifdef SKIP_ASM
/**
 * Signals the halt semaphore (+0xC), releasing a thread blocked in func_0031AAF0, then sleeps forever.
 *
 * Slot 14 of the PS2 execution manager vtable D_0048DB68; cExecutionMan_checkHalt calls it after running the halt
 * callback when its flag is set (cAppMan_checkHalt passes 1). Never returns.
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_0031AB10(void* self)
{
    SignalSema(*(int*)((char*)self + 0xC));
    for (;;) {
        SleepThread();
    }
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031AB30);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int GetThreadId();
extern "C" void SuspendThread(int thread);
int cExecutionMan_checkHalt(void* self, int flag);

struct sMbVE_AB30 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sMbOwner_AB30 {
    char pad_0x0[0x28];
    sMbVE_AB30 vt5;     // 0x28
};

/**
 * Halts the PS2 execution manager: stops the timer wake-ups and the loader thread, then runs or waits for the halt.
 *
 * Slot 13 of the PS2 execution manager vtable D_0048DB68, called by cExecutionMan_halt once it has stored the halt
 * callback. Sets the halt-request flag (+0x4038, which stops func_0031ABD0 waking the timer thread) and, unless it
 * runs on the loader thread (+0x14040), suspends that thread. On the main thread (+0x10) it runs
 * cExecutionMan_checkHalt(self, 0) directly; on any other thread it calls vtable slot 5 (func_0031AAC8, which wakes
 * the main thread if it is waiting in func_0031AA58) and then waits on the halt semaphore (func_0031AAF0).
 */
// Halt request: on the execution thread checks the halt directly; otherwise forwards it and wakes the worker.
extern "C" void func_0031AB30(void* self)
{
    char* s = (char*)self;
    *(volatile int*)(s + 0x4038) = 1;
    int cur = GetThreadId();
    if (cur != *(int*)(s + 0x14040)) {
        SuspendThread(*(int*)(s + 0x14040));
    }
    if (cur == *(int*)(s + 0x10)) {
        cExecutionMan_checkHalt(self, 0);
        return;
    }
    sMbOwner_AB30* o = *(sMbOwner_AB30**)(s + 0x8);
    o->vt5.fn(s + o->vt5.delta);
    func_0031AAF0(self);
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031ABC0__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (slot 7 of the PS2 execution manager vtable D_0048DB68).
 */
void func_0031ABC0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031ABC8__FPv);
#ifdef SKIP_ASM
/**
 * Returns 60 (slot 8 of the PS2 execution manager vtable D_0048DB68).
 *
 * @return 60
 */
int func_0031ABC8(void* self)
{
    return 0x3C;
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031ABD0);
#ifdef SKIP_ASM
void* func_00317520();
extern "C" void iSignalSema(int);

/**
 * Wakes the timer thread from the INTC 3 interrupt unless a halt has been requested.
 *
 * Runs inside func_0031A490 (the handler func_0031A6D8 installs). Gets the execution manager with func_00317520 and,
 * while its halt-request flag (+0x4038) is clear, signals the timer semaphore (+0x4034) with iSignalSema, releasing
 * one pass of func_0031AC08.
 */
extern "C" void func_0031ABD0(void)
{
    char* app = (char*)func_00317520();
    if (*(volatile int*)(app + 0x4038) == 0) {
        iSignalSema(*(int*)(app + 0x4034));
    }
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031AC08);
#ifdef SKIP_ASM
extern "C" void* WaitSema(int);
extern "C" void WakeupThread(int);
extern "C" void SleepThread();
// PORT: func_00317500__Fv is called with self here; bind the 1-arg form to that symbol.
int func_00317500_1(void* self) __asm__("func_00317500__Fv");

/**
 * Runs the PS2 execution manager's timer thread (its entry point).
 *
 * Each time the timer semaphore (+0x4034, signalled from the interrupt by func_0031ABD0) is released and the quit flag
 * (+0x20) is still clear, calls func_00317500 (which runs func_00317348 on cAppMan_TheAppMan, the per-tick update).
 * Once the quit flag is set, wakes the main thread (+0x10) and sleeps.
 */
extern "C" void func_0031AC08(void* self)
{
    while (WaitSema(*(int*)((char*)self + 0x4034)), *(volatile int*)((char*)self + 0x20) == 0)
    {
        func_00317500_1(self);
    }
    WakeupThread(*(int*)((char*)self + 0x10));
    SleepThread();
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031AC60);
#ifdef SKIP_ASM
struct sMemJobVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Runs the PS2 execution manager's loader thread (its entry point).
 *
 * Each time the loader semaphore (+0x14044, signalled by func_0031ACD8) is released and the quit flag (+0x20) is
 * still clear, calls virtual slot 2 of the pending job object (+0x14048) and then clears the job pointer, which
 * func_0031AD00 reports as done. Once the quit flag is set, wakes the main thread (+0x10) and sleeps.
 */
extern "C" void func_0031AC60(void* self)
{
    while (WaitSema(*(int*)((char*)self + 0x14044)), *(volatile int*)((char*)self + 0x20) == 0)
    {
        void* obj = *(void* volatile*)((char*)self + 0x14048);
        sMemJobVEntry* vt = *(sMemJobVEntry**)obj;
        vt[2].fn((char*)obj + vt[2].delta);
        *(void* volatile*)((char*)self + 0x14048) = 0;
    }
    WakeupThread(*(int*)((char*)self + 0x10));
    SleepThread();
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031ACD8);
#ifdef SKIP_ASM
extern "C" void SignalSema(int);

/**
 * Hands an object to the loader thread: stores it as the pending job (+0x14048) and signals the loader semaphore.
 *
 * func_0031AC60 then calls the object's virtual slot 2 and clears the job; func_0031AD00 reports when it has.
 *
 * @param a1  the job object
 */
extern "C" void func_0031ACD8(void* self, int a1)
{
    *(int*)((char*)self + 0x14048) = a1;
    SignalSema(*(int*)((char*)self + 0x14044));
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031AD00);
#ifdef SKIP_ASM
/**
 * Returns whether the loader thread has finished its job (the pending job at +0x14048 is null).
 *
 * @return 1 when no job is pending, else 0
 */
extern "C" int func_0031AD00(void* self)
{
    return *(int*)((char*)self + 0x14048) == 0;
}
#endif

extern "C" unsigned int D_004A3ED0;

//99.5% - single $gp-relative sw; SN's assembler doesn't support %gp_rel(),
// so our object always carries a real relocation here while the target's
// raw disassembly has the offset already baked in as a literal immediate.
INCLUDE_ASM("bx/memblockman", func_0031AD18);
#ifdef SKIP_ASM
/**
 * Clears the flag D_004A3ED0, which func_0031AD20 sets when it finds the disc tray has been opened.
 *
 * cGameComm_setState calls it on entering the GameActive state.
 */
extern "C" void func_0031AD18()
{
    D_004A3ED0 = 0;
}
#endif

//100%
INCLUDE_ASM("bx/memblockman", func_0031AD20);
#ifdef SKIP_ASM
extern int D_00519C40[];
extern int D_004A3ED4;
extern "C" int func_00402618(int cmd, int* out);
extern "C" void func_003E3D78(void);

/**
 * Recovers from a failed disc read and notes whether the disc tray was opened (a sync task).
 *
 * systemInit installs it with SYNCTASK_add in place of the CD file system's func_003E4000, which does the same
 * recovery. When D_004A3ED4 is set, clears it and calls func_00402618(2, ...) (command 2, the result is discarded).
 * When both bits 2 and 4 (mask 6) of the CD file system flags (D_00519C40) are set, calls func_00402618(2, ...) again
 * and sets D_004A3ED0 when it reports a nonzero count, clears the two bits and re-runs the read with func_003E3D78.
 *
 * @return 0
 */
extern "C" int func_0031AD20(void)
{
    int flags = D_00519C40[0];
    if (D_004A3ED4 != 0) {
        int r;
        D_004A3ED4 = 0;
        r = 0;
        func_00402618(2, &r);
    }
    if ((flags & 6) == 6) {
        int r2 = 0;
        func_00402618(2, &r2);
        if (r2 != 0)
            D_004A3ED0 = 1;
        D_00519C40[0] &= ~6;
        func_003E3D78();
    }
    return 0;
}
#endif

