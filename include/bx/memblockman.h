#ifndef BX_MEMBLOCKMAN_H
#define BX_MEMBLOCKMAN_H

// TMemContainer / TMemBlock: the fixed-size block pools behind cMemBlockMan (EA's d:/ssx/code/source/bx/memblockman.cpp,
// preview asserts). cMemBlockMan_initialize allocates seven containers ("m_container8" .. "m_container512") and stores
// them in the statics cMemBlockMan_m_container8 .. _m_container512; cMemBlockMan_alloc / _free route a request to the
// smallest pool that fits (falling back to the heap) by the block's address.
//
// EA's types are templates: TMemContainer<TMemBlock<Size>, Count> ([P] the preview's out-of-line instances
// __t13TMemContainer2Zt9TMemBlock1Ui512Ui256 (ctor), alloc__..Ui, free__..Pv, validate__.. for <512,256>, <256,512>,
// <128,1200>, <64,3200>, <32,768>, <16,512>, <8,256>; [O] On Tour's struct TMemContainer {TMemBlock m_blockList[N];
// TMemBlock* m_freeList;} with methods TMemContainer(), alloc(unsigned align), free(void*), isIn(const void*),
// validate(), reserveBlock(TMemBlock**), and struct TMemBlock {anonymous union} (size = the block size)).
// They are written out here as one FLAT struct per instance; the names TMemBlockN / TMemContainerN are ours (N = the
// block size; the block count is fixed per size), spelling TMemBlock<N> / TMemContainer<TMemBlock<N>, Count>.
//
// Sizes: [R] cMemBlockMan_initialize's cMemMan_alloc(Count * N + 4, "m_containerN", 0xK000000) per pool: 2052,
// 8196, 24580, 0x32004, 0x25804, 0x20004, 0x20004; [O] On Tour's 16-block instance is 0x84 (16 * 8 + 4).
// No vtable. Retail methods (bx/cubicspline, C++ linkage, self = void*): ctor TMemContainer8_TMemContainer8 .. TMemContainer512_TMemContainer512 (builds
// the free list through every block, last link 0, m_freeList = block 0), alloc TMemContainer8_alloc .. TMemContainer512_alloc (pops
// m_freeList, else cMemBlockMan_alloc(N + 4)), free TMemContainer8_free .. TMemContainer512_free (pushes the block),
// validate TMemContainer8_validate .. TMemContainer512_validate (empty). isIn is inlined into cMemBlockMan_free (inPool_319F68<bytes>).
//
// Evidence tags: [R] retail allocation and constructor; [P] preview template instances; [O] On Tour DWARF; [U] retail
// use (the C bodies' raw +0x800 .. +0x32000 accesses and the TMemContainer_0408 / _0448 views in bx/cubicspline.cpp).
// The union members' names are not in On Tour's DWARF (anonymous union, member names dropped): the link is m_pNext
// (name ours, after its [U] use), the storage unk_00Data.

#include "layout.h"

// ---- TMemBlock<8> / TMemContainer<TMemBlock<8>, 256> ("m_container8", 0x804)
struct TMemBlock8 {
    union {
        TMemBlock8* m_pNext;        // 0x0 (name ours) [U] a free block's link: the ctor stores block i + 1, alloc
                                    //     pops through it, free pushes (sw 0x0 of the freed block)
        char unk_00Data[0x8];       // 0x0 [O] the block's storage (TMemBlock size = the block size)
    };
};

struct TMemContainer8 {
    TMemBlock8 m_blockList[256];    // 0x000 [O]; [R] ctor links block i to i + 1 (stride 8), the last link (+0x7F8) 0
    TMemBlock8* m_freeList;         // 0x800 [O]; [R] ctor: block 0; [U] alloc pops, free pushes (lw/sw 0x800)

    // Methods bound to their retail symbols by asm labels.
    void* alloc() __asm__("TMemContainer8_alloc__FPv");
    void free(void** node) __asm__("TMemContainer8_free__FPvPPv");
};

CHECK_OFFSET(TMemBlock8, m_pNext, 0x0);
CHECK_OFFSET(TMemBlock8, unk_00Data, 0x0);
CHECK_SIZE(TMemBlock8, 0x8);
CHECK_OFFSET(TMemContainer8, m_blockList, 0x000);
CHECK_OFFSET(TMemContainer8, m_freeList, 0x800);
CHECK_SIZE(TMemContainer8, 0x804);

// ---- TMemBlock<16> / TMemContainer<TMemBlock<16>, 512> ("m_container16", 0x2004)
struct TMemBlock16 {
    union {
        TMemBlock16* m_pNext;       // 0x0 (name ours) [U] free-list link (see TMemBlock8)
        char unk_00Data[0x10];      // 0x0 [O]
    };
};

struct TMemContainer16 {
    TMemBlock16 m_blockList[512];   // 0x0000 [O]; [R] ctor links them, last link (+0x1FF0) 0
    TMemBlock16* m_freeList;        // 0x2000 [O]; [R] ctor: block 0; [U] alloc / free (lw/sw 0x2000)

    // Methods bound to their retail symbols by asm labels.
    void* alloc() __asm__("TMemContainer16_alloc__FPv");
    void free(void** node) __asm__("TMemContainer16_free__FPvPPv");
};

CHECK_OFFSET(TMemBlock16, m_pNext, 0x0);
CHECK_OFFSET(TMemBlock16, unk_00Data, 0x0);
CHECK_SIZE(TMemBlock16, 0x10);
CHECK_OFFSET(TMemContainer16, m_blockList, 0x0000);
CHECK_OFFSET(TMemContainer16, m_freeList, 0x2000);
CHECK_SIZE(TMemContainer16, 0x2004);

// ---- TMemBlock<32> / TMemContainer<TMemBlock<32>, 768> ("m_container32", 0x6004)
struct TMemBlock32 {
    union {
        TMemBlock32* m_pNext;       // 0x0 (name ours) [U] free-list link (see TMemBlock8)
        char unk_00Data[0x20];      // 0x0 [O]
    };
};

struct TMemContainer32 {
    TMemBlock32 m_blockList[768];   // 0x0000 [O]; [R] ctor links them, last link (+0x5FE0) 0
    TMemBlock32* m_freeList;        // 0x6000 [O]; [R] ctor: block 0; [U] alloc / free (lw/sw 0x6000)

    // Methods bound to their retail symbols by asm labels.
    void* alloc() __asm__("TMemContainer32_alloc__FPv");
    void free(void** node) __asm__("TMemContainer32_free__FPvPPv");
};

CHECK_OFFSET(TMemBlock32, m_pNext, 0x0);
CHECK_OFFSET(TMemBlock32, unk_00Data, 0x0);
CHECK_SIZE(TMemBlock32, 0x20);
CHECK_OFFSET(TMemContainer32, m_blockList, 0x0000);
CHECK_OFFSET(TMemContainer32, m_freeList, 0x6000);
CHECK_SIZE(TMemContainer32, 0x6004);

// ---- TMemBlock<64> / TMemContainer<TMemBlock<64>, 3200> ("m_container64", 0x32004)
struct TMemBlock64 {
    union {
        TMemBlock64* m_pNext;       // 0x0 (name ours) [U] free-list link (see TMemBlock8)
        char unk_00Data[0x40];      // 0x0 [O]
    };
};

struct TMemContainer64 {
    TMemBlock64 m_blockList[3200];  // 0x00000 [O]; [R] ctor links them, last link (+0x31FC0) 0
    TMemBlock64* m_freeList;        // 0x32000 [O]; [R] ctor: block 0; [U] alloc / free (lui 0x3 + 0x2000)

    // Methods bound to their retail symbols by asm labels.
    void* alloc() __asm__("TMemContainer64_alloc__FPv");
    void free(void** node) __asm__("TMemContainer64_free__FPvPPv");
};

CHECK_OFFSET(TMemBlock64, m_pNext, 0x0);
CHECK_OFFSET(TMemBlock64, unk_00Data, 0x0);
CHECK_SIZE(TMemBlock64, 0x40);
CHECK_OFFSET(TMemContainer64, m_blockList, 0x00000);
CHECK_OFFSET(TMemContainer64, m_freeList, 0x32000);
CHECK_SIZE(TMemContainer64, 0x32004);

// ---- TMemBlock<128> / TMemContainer<TMemBlock<128>, 1200> ("m_container128", 0x25804)
struct TMemBlock128 {
    union {
        TMemBlock128* m_pNext;      // 0x0 (name ours) [U] free-list link (see TMemBlock8)
        char unk_00Data[0x80];      // 0x0 [O]
    };
};

struct TMemContainer128 {
    TMemBlock128 m_blockList[1200]; // 0x00000 [O]; [R] ctor links them, last link (+0x25780) 0
    TMemBlock128* m_freeList;       // 0x25800 [O]; [R] ctor: block 0; [U] alloc / free

    // Methods bound to their retail symbols by asm labels.
    void* alloc() __asm__("TMemContainer128_alloc__FPv");
    void free(void** node) __asm__("TMemContainer128_free__FPvPPv");
};

CHECK_OFFSET(TMemBlock128, m_pNext, 0x0);
CHECK_OFFSET(TMemBlock128, unk_00Data, 0x0);
CHECK_SIZE(TMemBlock128, 0x80);
CHECK_OFFSET(TMemContainer128, m_blockList, 0x00000);
CHECK_OFFSET(TMemContainer128, m_freeList, 0x25800);
CHECK_SIZE(TMemContainer128, 0x25804);

// ---- TMemBlock<256> / TMemContainer<TMemBlock<256>, 512> ("m_container256", 0x20004)
struct TMemBlock256 {
    union {
        TMemBlock256* m_pNext;      // 0x0 (name ours) [U] free-list link (see TMemBlock8); [U] view
                                    //     TMemBlock_0408::next
        char unk_00Data[0x100];     // 0x0 [O]
    };
};

struct TMemContainer256 {
    TMemBlock256 m_blockList[512];  // 0x00000 [O]; [R] ctor links them, last link (+0x1FF00) 0
    TMemBlock256* m_freeList;       // 0x20000 [O]; [R] ctor: block 0; [U] alloc / free (view TMemContainer_0408)

    // Methods bound to their retail symbols by asm labels.
    void* alloc(unsigned int align) __asm__("TMemContainer256_alloc__FPv");
    void free(void** node) __asm__("TMemContainer256_free__FPvPPv");
};

CHECK_OFFSET(TMemBlock256, m_pNext, 0x0);
CHECK_OFFSET(TMemBlock256, unk_00Data, 0x0);
CHECK_SIZE(TMemBlock256, 0x100);
CHECK_OFFSET(TMemContainer256, m_blockList, 0x00000);
CHECK_OFFSET(TMemContainer256, m_freeList, 0x20000);
CHECK_SIZE(TMemContainer256, 0x20004);

// ---- TMemBlock<512> / TMemContainer<TMemBlock<512>, 256> ("m_container512", 0x20004)
struct TMemBlock512 {
    union {
        TMemBlock512* m_pNext;      // 0x0 (name ours) [U] free-list link (see TMemBlock8); [U] view
                                    //     TMemBlock_0448::next
        char unk_00Data[0x200];     // 0x0 [O]
    };
};

struct TMemContainer512 {
    TMemBlock512 m_blockList[256];  // 0x00000 [O]; [R] ctor links them, last link (+0x1FE00) 0
    TMemBlock512* m_freeList;       // 0x20000 [O]; [R] ctor: block 0; [U] alloc / free (view TMemContainer_0448)

    // Methods bound to their retail symbols by asm labels.
    void* alloc(unsigned int align) __asm__("TMemContainer512_alloc__FPv");
    void free(void** node) __asm__("TMemContainer512_free__FPvPPv");
};

CHECK_OFFSET(TMemBlock512, m_pNext, 0x0);
CHECK_OFFSET(TMemBlock512, unk_00Data, 0x0);
CHECK_SIZE(TMemBlock512, 0x200);
CHECK_OFFSET(TMemContainer512, m_blockList, 0x00000);
CHECK_OFFSET(TMemContainer512, m_freeList, 0x20000);
CHECK_SIZE(TMemContainer512, 0x20004);

#endif /* BX_MEMBLOCKMAN_H */
