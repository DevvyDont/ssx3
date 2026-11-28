#include "vtable.h"
#include "common.h"
#include "bx/math3d.h"
#include "render/bxps2tex.h"
#include "render/graphicsman.h"
#include "render/ps2graphicsman.h"

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_NewNonBindTexID);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_NBTexInfo[];


/**
 * Takes a free texture id from the texture manager and allocates its texture info.
 *
 * Pops the head of the free list (+0x1F48, a pointer into the 2000-slot array at +0x8; each free slot holds
 * the next free pointer), counts it at +0x4 and stores a new 0x58-byte "NBTexInfo" in the slot. The free list
 * is not checked for empty. Despite the On Tour class name, self is the graphics manager's mTexMan
 * (cTextureMan_AddTexture).
 *
 * @return the new texture id (its slot index)
 */
int cTextureMan::NewNonBindTexID()
{
    cLinkTexInfo* head = m_pNonBindFreeTexInfo;
    int id = head - m_pTexInfo;
    m_pNonBindFreeTexInfo = head->pNextFree;
    m_iNumNonBindTextures++;
    cLinkTexInfo* texInfo = m_pTexInfo;
    cLinkTexInfo* slot = texInfo + id;
    slot->pTexInfo = (tTexInfo*)cMemMan_alloc(88, str_NBTexInfo, 0x21000000, 0);
    return id;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_DeleteNonBindTexID);
#ifdef SKIP_ASM
void operator_delete(int* ptr);


/**
 * Frees a non-bound texture id: deletes its texture info and returns the slot to the free list.
 *
 * Deletes slots[@p id] (+0x8), stores the free-list head (+0x1F48) in the slot, makes the slot the new head and
 * decrements the allocated count at +0x4. Undoes cTextureMan_NewNonBindTexID; cTextureMan_RemoveTexture calls it for ids 1500
 * and up.
 *
 * @param id  the texture id (slot index) to free
 */
void cTextureMan::DeleteNonBindTexID(int id)
{
    operator_delete((int*)m_pTexInfo[id].pTexInfo);
    m_pTexInfo[id].pNextFree = m_pNonBindFreeTexInfo;
    m_pNonBindFreeTexInfo = &m_pTexInfo[id];
    m_iNumNonBindTextures--;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_PurgeNonBindTexIDs);
#ifdef SKIP_ASM
/**
 * Does nothing useful: counts through the 500 non-bound texture ids with an empty loop body.
 *
 * The per-id work of this purge is compiled out in retail; only the loop remains. cTextureMan_purgeAll calls it.
 */
extern "C" void cTextureMan_PurgeNonBindTexIDs(void)
{
    for (int i = 0; i < 500; i++) {
    }
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_NewBindTexID);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BTexInfo[];

/**
 * Allocates the texture info for a texture id the caller chose.
 *
 * Stores a new 0x58-byte "BTexInfo" in slot @p id of the array at +0x8 and increments the bound count at
 * +0x0; the free list is not touched. self is the texture manager, as for cTextureMan_NewNonBindTexID().
 *
 * @param id  texture id (slot index) to fill
 */
void cTextureMan::NewBindTexID(int id)
{
    cLinkTexInfo* texInfo = m_pTexInfo;
    cLinkTexInfo* slot = texInfo + id;
    slot->pTexInfo = (tTexInfo*)cMemMan_alloc(88, str_BTexInfo, 0x21000000, 0);
    m_iNumBindTextures += 1;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_DeleteBindTexID);
#ifdef SKIP_ASM
void operator_delete(int* ptr);


/**
 * Frees a bound texture id: deletes its texture info, clears the slot and decrements the bound count (+0x0).
 *
 * Undoes cTextureMan_NewBindTexID; cTextureMan_RemoveTexture calls it for ids below 1500.
 *
 * @param idx  the texture id (slot index) to free
 */
void cTextureMan::DeleteBindTexID(int idx)
{
    int** base = ((int**)&m_pTexInfo[0]); int** slot = base + idx;
    operator_delete(*slot);
    *slot = 0;
    m_iNumBindTextures--;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_PurgeBindTexIDs);
#ifdef SKIP_ASM
/**
 * Does nothing useful: counts through the 1500 bound texture ids with an empty loop body.
 *
 * The per-id work of this purge is compiled out in retail; only the loop remains. cTextureMan_purgeAll calls it (passing the
 * texture manager, which the definition ignores).
 */
extern "C" void cTextureMan_PurgeBindTexIDs(void)
{
    for (int i = 0; i < 1500; i++) {
    }
}
#endif

extern "C" void* vramHeap_init(void*, int, int, void*);

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_initTempHeap__FPvii);
#ifdef SKIP_ASM
/**
 * Sets up the texture manager's second GS memory heap (+0x4350) to cover @p size units from @p start.
 *
 * Forwards to vramHeap_init (the heap initialiser, which starts with one free block) with the texture record table
 * (+0x8) as the owner table. Texel format 9 textures, sent as PSMT8H, are allocated from this heap. cPS2GraphicsMan_applyDisplayMode calls
 * it after allocating the frame and z buffers, with the start of the first 24-bit buffer (page << 11, i.e. in 32-bit
 * words) and the pixel count of the 24-bit buffers from there.
 *
 * @param start  the first address of the heap, in 32-bit words
 * @param size  the heap size, in 32-bit words
 * @return nothing meaningful: vramHeap_init returns nothing, though the definition is declared void*
 */
void* cTextureMan::initTempHeap(int start, int size)
{
    return vramHeap_init(mTempHeap, start, size, &m_pTexInfo[0]);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_purgeAll);
#ifdef SKIP_ASM
extern "C" void vramHeap_freeUnlockedBlocks(void* p);
// PORT: the unit defines cTextureMan_PurgeBindTexIDs(void), but this caller passes self.
void cPSPGraphicsMan_PurgeBindTexIDs_1(void* self) __asm__("cTextureMan_PurgeBindTexIDs");

/**
 * Purges the texture manager: frees every unlocked VRAM block and forgets the textures bound to the GS contexts.
 *
 * Runs the two id purges (cTextureMan_PurgeNonBindTexIDs, cTextureMan_PurgeBindTexIDs), frees the unlocked blocks of both GS memory heaps (+0x1F60 and
 * +0x4350, vramHeap_freeUnlockedBlocks), and sets the per-context bound texture ids (+0x1F4C, two words) and the loaded-CLUT texture
 * (+0x1F54) to -1. cPS2GraphicsMan_Purge calls it on mTexMan.
 */
void cTextureMan::purgeAll()
{
    int i;
    cTextureMan_PurgeNonBindTexIDs();
    cPSPGraphicsMan_PurgeBindTexIDs_1(this);
    vramHeap_freeUnlockedBlocks(mMainHeap);
    vramHeap_freeUnlockedBlocks(mTempHeap);
    for (i = 1; i >= 0; i--) {
        ((int*)(&mBoundTex[0]))[i] = -1;
    }
    mCLUTTex = -1;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003673D0__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: an empty texture manager hook that the frame-end code calls on mTexMan.
 */
void func_003673D0(void* vself)
{
    cTextureMan* self = (cTextureMan*)vself;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_allocLockedPages);
#ifdef SKIP_ASM
extern "C" int vramHeap_allocBlock(void* heap, int size, int idx, int other, int a4, int a5);

/**
 * Allocates a locked block of @p size bytes in the texture manager's main GS memory heap and returns its page number.
 *
 * Takes a page-aligned block from the heap at +0x1F60 (vramHeap_allocBlock, no owner), sets flags 0x22 on its node (used and
 * locked) and owner 0x7FFFFFFF, and returns the block's address in 8 KB pages (node flags >> 13). cPS2GraphicsMan_applyDisplayMode
 * allocates the frame and z buffers with it.
 *
 * @param size  the number of bytes wanted
 * @return the block's first GS page
 */
int cTextureMan::allocLockedPages(int size)
{
    int idx = vramHeap_allocBlock(mMainHeap, size, -1, -1, -1, 1);
    char* node = *(char**)(((char*)this) + 0x3F50) + idx * 0x1C;
    *(int*)node |= 0x22;
    *(int*)(node + 8) = 0x7FFFFFFF;
    return *(int*)node >> 13;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_AddTexture);
#ifdef SKIP_ASM
extern "C" void cTextureMan_NewBindTexID(cTextureMan* self, int id);
extern "C" int vramHeap_allocBlock(void* heap, int size, int idx, int other, int a4, int a5);
extern "C" void cTextureMan_rearrangeMipLevels(void* self, void* tex);
extern "C" void* MEM_copy(void*, void*, int);
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
// PORT: operator_new__FUi is the game's tagged allocator (size, tag, flags, d); bound by asm label
void* operator_new_7440(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

// PORT: PS2-only MMI count-leading-sign-bits (plzcw) = floor(log2(x)); off-PS2 use a clz builtin.
// Volatile: the target keeps each log2 in order (zero-extend, plzcw, truncate).
// Volatile: the target keeps each log2 in order (zero-extend, plzcw, truncate) before the next.
static inline int texLog2_7440(unsigned int x)
{
    long r;
    __asm__ __volatile__("plzcw %0, %1\n\txori %0, %0, 0x1f\n\taddiu %0, %0, -1" : "=r"(r) : "r"((unsigned long)x));
    return r;
}

// GS pixel storage mode -> bits per pixel
static inline int psmBpp_7440(int psm)
{
    switch (psm) {
    case 0x0:
    case 0x1:
    case 0x30:
    case 0x31:
        return 32;
    case 0x2:
    case 0xA:
    case 0x32:
    case 0x3A:
        return 16;
    case 0x13:
    case 0x1B:
        return 8;
    case 0x14:
    case 0x24:
    case 0x2C:
        return 4;
    }
    return 0;
}

static inline int imax_7440(int a, int b)
{
    return a < b ? b : a;
}

// VRAM bytes for one level: the image rounded up to whole GS pages, then to 256 bytes
static inline int vramBytes_7440(int w, int h, int bpp)
{
    int pw, ph;
    switch (bpp) {
    case 4:
        pw = 0x80;
        ph = 0x80;
        break;
    case 8:
        pw = 0x80;
        ph = 0x40;
        break;
    case 15:
    case 16:
        pw = 0x40;
        ph = 0x40;
        break;
    case 32:
    default:
        pw = 0x40;
        ph = 0x20;
        break;
    }
    int n = imax_7440(w, pw) * imax_7440(h, ph) * bpp / 8;
    return n + (-n & 0xFF);
}

// CLUT format -> bytes per CLUT entry
static inline int clutEntryBytes_7440(int f)
{
    switch (f) {
    case 0:
        return 0;
    case 1:
        return 4;
    case 2:
        return 2;
    }
    return 0;
}

// CLUT format -> GS CPSM
static inline int clutPsm_7440(int f)
{
    switch (f) {
    case 0:
        return 0;
    case 1:
        return 0;
    case 2:
        return 2;
    }
    return 0;
}

/**
 * Creates a texture in the texture manager: takes an id, fills its texture info (sizes, GS TEX0/TEX1/MIPTBP registers)
 * and optionally copies the pixels and CLUT and reserves VRAM.
 *
 * The id is @p bindId when it is 0 or more (cTextureMan_NewBindTexID) or a free non-bound one
 * (cTextureMan_NewNonBindTexID). The 0x58-byte info gets: the copy flag (+0x0), 8 characters of @p texName (+0x4), the
 * texel format (+0xC), mip count (+0x24), tTexInfo::kNoVRAM in the four VRAM fields (+0x28..+0x34), and the bits per
 * pixel (+0x18) of the GS format chosen from @p texelFormat (GM_TEXEL_RGB888 -> PSMCT24, GM_TEXEL_ARGB1555 -> PSMCT16,
 * the INDEX8 formats -> PSMT8, the INDEX4 formats -> PSMT4, else PSMCT32). With pixels it sums the data size (+0x10,
 * each level at least 0x100 bytes) and the page-rounded VRAM size (+0x14) over levels 0 to @p mipCount, and either
 * copies the data into a new buffer tagged with the name (when @p ownsData) or keeps the pointer (+0x1C); without
 * pixels the data size is 0 and the VRAM size covers one level. A CLUT (16 entries for the INDEX4 formats, else 256) is
 * copied or referenced likewise (+0x20).
 *
 * TEX0 (+0x38) gets the buffer width, format, log2 size, TCC and the CLUT format; TEX1 (+0x40) the mip settings; with
 * mips, MIPTBP1/2 (+0x48/+0x50) get the level widths. @p vramMode GM_TEXTYPE_LOCKED allocates the VRAM block now (heap
 * +0x1F60, node flag 2) and GM_TEXTYPE_WINDOW allocates 0x2000 bytes more, locked (flags 0x22), with the address
 * rounded up to 32 blocks. A GM_TEXEL_INDEX8 texture becomes PSMT8H, or is converted by cTextureMan_rearrangeMipLevels when it owns its
 * data, the free space at +0x6344 is smaller than its VRAM size and @p vramMode is GM_TEXTYPE_DYNAMIC.
 * cPS2GraphicsMan_AddTexture1 and cTextureMan_AddLockedFrameTexture call it.
 *
 * @param pixelData  the pixel data (all mip levels), or null
 * @param texName  the texture name (8 characters are kept)
 * @param texWidth  width in texels
 * @param texHeight  height in texels
 * @param mipCount  number of extra mip levels
 * @param texelFormat  texel format, eGMTexelFormat (GM_TEXEL_RGB888 24-bit, GM_TEXEL_ARGB1555 16-bit, the INDEX8
 *                     formats 8-bit, the INDEX4 formats 4-bit, other 32-bit)
 * @param clutFormat  CLUT format (1 32-bit, 2 16-bit, 0 none)
 * @param clutData  the CLUT, or null
 * @param vramMode  VRAM handling, eGMTexLockType (On Tour lockType): GM_TEXTYPE_DYNAMIC on demand, GM_TEXTYPE_LOCKED
 *                  allocate now, GM_TEXTYPE_WINDOW allocate now and lock
 * @param ownsData  nonzero to copy the pixels and CLUT into the manager's own buffers
 * @param bindId  the id to use, or a negative value to take a free non-bound id
 * @return the texture id, or kNoTexID for a format with no bit depth
 */
int cTextureMan::AddTexture(void* pixelData, void* texName, int texWidth, int texHeight, int mipCount, int texelFormat,
                            int clutFormat, void* clutData, int vramMode, int ownsData, int bindId)
{
    void* data = pixelData;
    const char* name = (const char*)texName;
    unsigned int w = texWidth;
    unsigned int height = texHeight;
    int mip = mipCount;
    int fmt = texelFormat;
    int clutFmt = clutFormat;
    void* clut = clutData;
    int id;
    if (bindId < 0) {
        id = NewNonBindTexID();
    } else {
        NewBindTexID(bindId);
        id = bindId;
    }
    tTexInfo* tex = m_pTexInfo[id].pTexInfo;
    strncpy(tex->mName, name, 8);
    tex->mMemAlloc = ownsData;
    tex->mFormat = fmt;
    tex->mTexAddr = tTexInfo::kNoVRAM;
    tex->mTexBlock = tTexInfo::kNoVRAM;
    tex->mCLUTAddr = tTexInfo::kNoVRAM;
    tex->mNumMips = mip;
    tex->mCLUTBlock = tTexInfo::kNoVRAM;
    int psm;
    switch (fmt) {
    case GM_TEXEL_RGB888:
        psm = kGS_PSMCT24;
        break;
    case GM_TEXEL_ARGB1555:
        psm = kGS_PSMCT16;
        break;
    case GM_TEXEL_INDEX8:
    case GM_TEXEL_INDEX8S:
        psm = kGS_PSMT8;
        break;
    case GM_TEXEL_INDEX4:
    case GM_TEXEL_INDEX4S:
        psm = kGS_PSMT4;
        break;
    case GM_TEXEL_ARGB8888:
    default:
        psm = kGS_PSMCT32;
        break;
    }
    int bpp = psmBpp_7440(psm);
    tex->mBPP = bpp;
    if (bpp <= 0)
        return kNoTexID;
    if (data != 0) {
        int size = 0;
        int vram = 0;
        for (int lv = 0; lv <= mip; lv++) {
            int lw = w >> lv;
            int lh = height >> lv;
            int n = lw * lh * bpp / 8;
            if (n < 0x100)
                n = 0x100;
            size += n;
            vram += vramBytes_7440(lw, lh, bpp);
        }
        tex->mVRAMSize = vram;
        tex->mSize = size;
        if (tex->mMemAlloc != 0) {
            void* copy = operator_new_7440(size, name, TexMemType | 0x05000000, 0);
            tex->mpTexels = copy;
            MEM_copy(copy, data, size);
        } else {
            tex->mpTexels = data;
        }
    } else {
        tex->mpTexels = 0;
        tex->mSize = 0;
        tex->mVRAMSize = vramBytes_7440(w, height, bpp);
    }
    int esz;
    if (clut != 0 && (esz = clutEntryBytes_7440(clutFmt)) != 0) {
        int entries = (unsigned int)(tex->mFormat - 11) <= 1 ? 0x10 : 0x100;
        if (tex->mMemAlloc != 0) {
            void* copy = operator_new_7440(0x400, name, TexMemType | 0x05000000, 0);
            tex->mCLUTData = copy;
            MEM_copy(copy, clut, entries * esz);
        } else {
            tex->mCLUTData = clut;
        }
    } else {
        tex->mCLUTData = 0;
    }
    int lcm = 0;
    int lodL = 0;
    int k = -185;
    int mmin = 5;
    if (mip == 0) {
        lcm = 1;
        lodL = mip;
        k = 0;
        mmin = 1;
    }
    ulong* tex0p = (ulong*)(&tex->mTEX0);
    int lw2 = texLog2_7440(w - 1);
    int lh2 = texLog2_7440(height - 1);
    int cpsm = clutPsm_7440(clutFmt);
    *tex0p = (ulong)0 | ((ulong)((w + 63) >> 6) << 14) | ((ulong)psm << 20) | ((ulong)(lw2 + 1) << 26)
        | ((ulong)(lh2 + 1) << 30) | ((ulong)1 << 34) | ((ulong)0 << 35) | ((ulong)0 << 37)
        | ((ulong)cpsm << 51) | ((ulong)0 << 55) | ((ulong)0 << 56) | ((ulong)(bpp <= 8) << 61);
    tex->mTEX1 = (ulong)lcm | ((ulong)(unsigned int)mip << 2) | ((ulong)1 << 5)
        | ((ulong)mmin << 6) | ((ulong)lodL << 19) | ((ulong)k << 32);
    if (mip != 0) {
        tex->mMIPTBP1 = ((ulong)((w + 0x7F) >> 7) << 14) | ((ulong)((w + 0xFF) >> 8) << 34)
            | ((ulong)((w + 0x1FF) >> 9) << 54);
        tex->mMIPTBP2 = ((ulong)((w + 0x3FF) >> 10) << 14) | ((ulong)((w + 0x7FF) >> 11) << 34)
            | ((ulong)((w + 0xFFF) >> 12) << 54);
    }
    if (vramMode == GM_TEXTYPE_LOCKED) {
        int node = vramHeap_allocBlock(mMainHeap, tex->mVRAMSize, id, -1, -1, 1);
        int* nodeFlags = (int*)(*(char**)(((char*)this) + 0x3F50) + node * 0x1C);
        int fl = *nodeFlags | 2;
        *nodeFlags = fl;
        tex->mTexBlock = node;
        tex->mTexAddr = fl >> 8;
    } else if (vramMode == GM_TEXTYPE_WINDOW) {
        int node = vramHeap_allocBlock(mMainHeap, tex->mVRAMSize + 0x2000, id, -1, -1, 1);
        int* nodeFlags = (int*)(*(char**)(((char*)this) + 0x3F50) + node * 0x1C);
        int fl = *nodeFlags | 0x22;
        *nodeFlags = fl;
        tex->mTexBlock = node;
        int vramAddr = fl >> 8;
        tex->mTexAddr = vramAddr + (-vramAddr & 0x1F);
    }
    if (tex->mMemAlloc != 0) {
        if (tex->mFormat == GM_TEXEL_INDEX8) {
            if (*(int*)(((char*)this) + 0x6344) < tex->mVRAMSize && vramMode == GM_TEXTYPE_DYNAMIC)
                cTextureMan_rearrangeMipLevels(this, tex);
            else
                tex->mTEX0 = (tex->mTEX0 & ~((ulong)0x3F << 20)) | ((ulong)27 << 20);
        }
    } else if (tex->mFormat == GM_TEXEL_INDEX8) {
        tex->mTEX0 = (tex->mTEX0 & ~((ulong)0x3F << 20)) | ((ulong)27 << 20);
    }
    return id;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_unlockTexture);
#ifdef SKIP_ASM
/**
 * Unlocks texture @p i's VRAM block so the heap may evict it again.
 *
 * When the texture has a block (+0x28 not tTexInfo::kNoVRAM) it clears flag bit 1 of that block's node (+0x30) in the
 * main heap (+0x1F60), or the second heap (+0x4350) for GM_TEXEL_INDEX8. cPS2GraphicsMan_RemoveScreenGrabTexture and
 * cPlayerShadow::~cPlayerShadow call it on mTexMan.
 *
 * @param i  the texture id
 */
void cTextureMan::unlockTexture(int i)
{
    tTexInfo* obj = m_pTexInfo[i].pTexInfo;
    if (obj->mTexAddr != tTexInfo::kNoVRAM) {
        char* sub = obj->mFormat != GM_TEXEL_INDEX8 ? mMainHeap : mTempHeap;
        char* node = *(char**)(sub + 0x1FF0) + obj->mTexBlock * 0x1C;
        *(int*)node &= ~2;
    }
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_RemoveTexture);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
extern "C" void vramHeap_freeBlock(void* self, int id);

static inline void fill7BC0(int* p, int v)
{
    for (int i = 1; i >= 0; i--) {
        p[i] = v;
    }
}

/**
 * Removes texture @p id from the texture manager: frees its VRAM blocks, owned buffers and id.  Frees the texture's
 * block (+0x30) in the main heap (+0x1F60; the second heap +0x4350 for GM_TEXEL_INDEX8) and its CLUT block (+0x34, main
 * heap) with vramHeap_freeBlock when they exist; when the texture owns its data (+0x0) frees the pixel and CLUT buffers
 * (+0x1C, +0x20). Then deletes the id (cTextureMan_DeleteBindTexID for ids below 1500, else
 * cTextureMan_DeleteNonBindTexID) and sets the per-context bound texture ids (+0x1F4C) and the loaded-CLUT texture
 * (+0x1F54) to -1. cPS2GraphicsMan_RemoveTexture calls it.
 *
 * @param id  the texture id
 */
void cTextureMan::RemoveTexture(int id)
{
    tTexInfo* tex = m_pTexInfo[id].pTexInfo;
    if (tex->mTexAddr != tTexInfo::kNoVRAM) {
        vramHeap_freeBlock(tex->mFormat != GM_TEXEL_INDEX8 ? mMainHeap : mTempHeap, tex->mTexBlock);
    }
    if (tex->mCLUTAddr != tTexInfo::kNoVRAM) {
        vramHeap_freeBlock(mMainHeap, tex->mCLUTBlock);
    }
    if (tex->mMemAlloc != 0) {
        if (tex->mpTexels != 0) {
            cMemMan_free(tex->mpTexels);
        }
        if (tex->mCLUTData != 0) {
            cMemMan_free(tex->mCLUTData);
        }
    }
    if (id < 1500) {
        DeleteBindTexID(id);
    } else {
        DeleteNonBindTexID(id);
    }
    fill7BC0(mBoundTex, -1);
    mCLUTTex = -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_AddLockedFrameTexture);
#ifdef SKIP_ASM
extern "C" int cTextureMan_AddTexture(cTextureMan* self, int a1, void* a2, int a3, int a4, int a5, int a6, int a7,
                             int a8, int a9, int a10, int a11);
extern char str_LockTex[];

/**
 * Creates a render-target texture: a texture named "LockTex" with no pixels, locked in VRAM.
 *
 * Calls cTextureMan_AddTexture with no data, @p width x @p height, no mips, texel format @p format, no CLUT, VRAM mode 3
 * (allocate now and lock), no copy and a free non-bound id. cPS2GraphicsMan_AddLockedFrameTexture__FPv calls it.
 *
 * @param width  width in texels
 * @param height  height in texels
 * @param format  the texel format
 * @return the new texture id
 */
int cTextureMan::AddLockedFrameTexture(int width, int height, int format)
{
    return AddTexture(0, str_LockTex, width, height, 0, format, 0, 0, 3, 0, -1);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_SetNewTextureData);
#ifdef SKIP_ASM
extern "C" void* MEM_copy(void*, void*, int);
extern "C" void cTextureMan_dropVramCopy(cTextureMan* self, int idx);

/**
 * Replaces the pixels and CLUT of texture @p idx and marks it for re-upload.
 *
 * When @p upload is set, copies @p data over the texture's pixel buffer (+0x1C, size +0x10) and @p clut over its CLUT
 * buffer (+0x20, 4 << bits-per-pixel bytes), each only when given. Then calls cTextureMan_dropVramCopy on the texture. @p width,
 * @p height and the format arguments are not used. cPS2GraphicsMan_SetNewTextureData1 calls it on mTexMan.
 *
 * @param idx  the texture id
 * @param data  the new pixels, or null
 * @param width  not used
 * @param height  not used
 * @param mipCount  not used (the mip count of the caller's overload)
 * @param texelFormat  not used (the texel format)
 * @param clutFormat  not used (the CLUT format)
 * @param clut  the new CLUT, or null
 * @param upload  nonzero to copy the data into the texture's buffers
 */
void cTextureMan::SetNewTextureData(int idx, void* data, int width, int height, int mipCount, int texelFormat,
                                    int clutFormat, void* clut, int upload)
{
    tTexInfo* tex = m_pTexInfo[idx].pTexInfo;
    if (upload != 0) {
        if (data != 0) {
            MEM_copy(tex->mpTexels, data, tex->mSize);
        }
        if (clut != 0) {
            MEM_copy(tex->mCLUTData, clut, 4 << tex->mBPP);
        }
    }
    this->dropVramCopy(idx);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_LockFrameToTexture);
#ifdef SKIP_ASM
// PORT: 64-bit GS register words (ulong is 64-bit here).
struct sGsTex0Bits {
    ulong TBP0 : 14;
    ulong TBW : 6;
    ulong PSM : 6;
    ulong TW : 4;
    ulong TH : 4;
    ulong TCC : 1;
    ulong TFX : 2;
    ulong CBP : 14;
    ulong CPSM : 4;
    ulong CSM : 1;
    ulong CSA : 5;
    ulong CLD : 3;
};

struct sPs2RenderTex {
    char pad_0x00[0x28];
    int addr;               // 0x28
    char pad_0x2C[0xC];
    sGsTex0Bits tex0;       // 0x38
};

struct sPs2TexSet {
    char pad_0x00[0x8];
    sPs2RenderTex* tex[1];  // 0x08
};

static inline ulong gsSetFrame367DB8(int fbp, int fbw, int psm, int fbmsk)
{
    return (ulong)fbp | ((ulong)fbw << 16) | ((ulong)psm << 24) | ((ulong)fbmsk << 32);
}

/**
 * Appends a GS packet that makes texture @p idx the render target: FRAME_1, SCISSOR_1 and XYOFFSET_1.
 *
 * Writes at *@p pp a DMA cnt tag, a VIF DIRECT code and an A+D GIF tag of three registers: FRAME_1 (0x4C) with the
 * texture's VRAM address (+0x28) in pages, width / 64 and pixel format from its TEX0 (+0x38); SCISSOR_1 (0x40) covering
 * the texture; XYOFFSET_1 (0x18) placing it centred on 2048. Advances *@p pp. cPS2GraphicsMan_LockFrameToTexture calls
 * it.
 *
 * @param idx  the texture id
 * @param pp  the packet cursor, advanced past the 12 words written
 */
extern "C" void cTextureMan_LockFrameToTexture(sPs2TexSet* self, int idx, ulong** pp)
{
    sPs2RenderTex* renderTex = self->tex[idx];
    ulong* packet = *pp;
    packet[0] = 0x10000005;
    packet[1] = 0;
    packet[2] = (ulong)34816 << 45;
    packet[3] = (ulong)0x50000004 << 32;
    packet[4] = ((ulong)0x10000000 << 32) | 0x8003;
    packet[5] = 0xE;
    packet[6] = gsSetFrame367DB8(renderTex->addr >> 5, (1 << renderTex->tex0.TW) >> 6, renderTex->tex0.PSM, 0);
    packet[7] = 76;
    packet[8] = ((ulong)((1 << renderTex->tex0.TW) - 1) << 16) | ((ulong)((1 << renderTex->tex0.TH) - 1) << 48);
    packet[9] = 0x40;
    packet[10] = (ulong)((0x800 - ((1 << renderTex->tex0.TW) >> 1)) << 4) |
            ((ulong)((0x800 - ((1 << renderTex->tex0.TH) >> 1)) << 4) << 32);
    packet[11] = 0x18;
    packet += 12;
    *pp = packet;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_ShadowClearLockTexture);
#ifdef SKIP_ASM
// PORT: 64-bit GS register words (ulong is 64-bit here).
struct sGsTex0Bits7F18 {
    ulong TBP0 : 14;
    ulong TBW : 6;
    ulong PSM : 6;
    ulong TW : 4;
    ulong TH : 4;
    ulong TCC : 1;
    ulong TFX : 2;
    ulong CBP : 14;
    ulong CPSM : 4;
    ulong CSM : 1;
    ulong CSA : 5;
    ulong CLD : 3;
};


struct sPs2TexSet7F18 {
    char pad_0x00[0x8];
    tTexInfo* tex[1];  // 0x08
};

union sFloatBits7F18 {
    float f;
    int i;
};

/**
 * Appends a GS packet that fills texture @p idx (the current render target) with a flat colour and then shrinks the
 * scissor by a one-pixel border.
 *
 * Writes at *@p pp a DMA cnt tag, a VIF DIRECT code, a GIF tag with PRIM 0x10E (flat sprite), RGBAQ (R, G, B =
 * color[1..3] * 128, A = color[0] * 128, Q = 1) and the two XYZ2 corners of the texture centred on 2048, then an A+D
 * SCISSOR_1 (0x40) from 1 to size - 2. Advances *@p pp. cPS2GraphicsMan_ShadowClearLockTexture calls it.
 *
 * @param idx  the texture id
 * @param color  the fill colour {a, r, g, b} in 0..1
 * @param pp  the packet cursor, advanced past the 14 words written
 */
extern "C" void cTextureMan_ShadowClearLockTexture(sPs2TexSet7F18* self, int idx, float* color, ulong** pp)
{
    tTexInfo* renderTex = self->tex[idx];
    ulong* packet = *pp;
    sFloatBits7F18 qValue;
    qValue.f = 1.0f;
    packet[0] = 0x10000006;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = (ulong)0x50000005 << 32;
    packet[4] = ((ulong)0x44000000 << 32) | 1;
    packet[5] = 21776;
    packet[6] = 270;
    packet[7] = (ulong)(int)(color[1] * 128.0f) | ((ulong)(int)(color[2] * 128.0f) << 8) |
           ((ulong)(int)(color[3] * 128.0f) << 16) | ((ulong)(int)(color[0] * 128.0f) << 24) |
           ((ulong)qValue.i << 32);
    packet[8] = (ulong)((0x800 - ((1 << (*(sGsTex0Bits7F18*)&renderTex->mTEX0).TW) >> 1)) << 4) |
           ((ulong)((0x800 - ((1 << (*(sGsTex0Bits7F18*)&renderTex->mTEX0).TH) >> 1)) << 4) << 16) | ((ulong)1 << 32);
    packet[9] = (ulong)((((1 << (*(sGsTex0Bits7F18*)&renderTex->mTEX0).TW) >> 1) + 2049) << 4) |
           ((ulong)((((1 << (*(sGsTex0Bits7F18*)&renderTex->mTEX0).TH) >> 1) + 2049) << 4) << 16) | ((ulong)1 << 32);
    packet[10] = ((ulong)0x10000000 << 32) | 0x8001;
    packet[11] = 0xE;
    {
        ulong hi = ((ulong)((1 << (*(sGsTex0Bits7F18*)&renderTex->mTEX0).TH) - 2) << 48) | (((ulong)1 << 32) | 1);
        packet[12] = ((ulong)((1 << (*(sGsTex0Bits7F18*)&renderTex->mTEX0).TW) - 2) << 16) | hi;
    }
    packet[13] = 0x40;
    packet += 14;
    *pp = packet;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_appendFixedPacketRef);
#ifdef SKIP_ASM
extern char D_0044B200[];

/**
 * Appends a DMA ref tag that sends the fixed 0x22-quadword packet D_0044B200.
 *
 * Writes the tag at *@p pkt and advances it by one quadword. The texture upload builders (cTextureMan_buildPixelUpload, cTextureMan_buildClutUpload)
 * and several draw paths in render/graphicsman and render/lightman end their uploads with it.
 *
 * @param pkt  the DMA packet cursor
 */
// PORT: 64-bit `ulong` DMA tag, pointer packed into the upper word
void cTextureMan::appendFixedPacketRef(ulong** pkt)
{
    (*pkt)[0] = ((ulong)(int)D_0044B200 << 32) | 0x30000022;
    (*pkt)[1] = 0;
    *pkt += 2;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_buildPixelUpload);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef int cQuad128_8170 __attribute__((mode(TI)));

struct sBXTex_8170 {
    char pad_0x00[0xC];
    int psm;                // 0xC
    char pad_0x10[0x8];
    int bpp;                // 0x18
    int src;                // 0x1C
    char pad_0x20[0x4];
    int levels;             // 0x24
    int dbp;                // 0x28
    char pad_0x2C[0xC];
    ulong tex0;             // 0x38

    int Width() { return 1 << (int)((tex0 >> 26) & 0xF); }
    int Height() { return 1 << (int)((tex0 >> 30) & 0xF); }
    int BPP() { return bpp; }
};

struct sTexMgr_8170 {
    char pad_0x0[0x8];
    sBXTex_8170* texs[1];   // 0x8
};

template <class T> static inline T Max_8170(T a, T b)
{
    return a >= b ? a : b;
}

/**
 * Builds the GS upload of every mip level of texture @p idx: per level an image transfer of its pixels to VRAM.
 *
 * For levels 0 to the mip count (+0x24) it appends to the @p gif chain a DMA cnt with an A+D GIF tag for BITBLTBUF
 * (0x50: VRAM block, buffer width, format), TRXPOS (0x51), TRXREG (0x52: width x height) and TRXDIR (0x53), an IMAGE
 * GIF tag and a DMA ref to the level's pixels (at least 0x100 bytes), and an A+D TEXFLUSH (0x3F; end of packet on the
 * last level). Each level halves the size and moves the source and VRAM block on (VRAM in page-rounded steps). Texel
 * format GM_TEXEL_INDEX8S is sent as 32-bit at half size, GM_TEXEL_INDEX4S as 32-bit at a quarter, and GM_TEXEL_INDEX8
 * as PSMT8H with a smaller block shift. Ends the @p dma chain with cTextureMan_appendFixedPacketRef. cTextureMan_ensureContextTextures calls it for textures
 * not yet in VRAM.
 *
 * @param self_  the texture manager (this)
 * @param idx  the texture id
 * @param dma  the DMA chain cursor
 * @param gif  the upload chain cursor
 */
// PORT: 64-bit `ulong` GIF/DMA packet words.
void cTextureMan::buildPixelUpload(int idx, ulong** dma, ulong** gif)
{
    sTexMgr_8170* self = (sTexMgr_8170*)this;
    sBXTex_8170* texture = self->texs[idx];
    int psm = texture->psm;
    ulong* dmaCursor = *dma;
    int w = texture->Width();
    int height = texture->Height();
    int fmt = (int)((texture->tex0 >> 20) & 0x3F);
    ulong* gifCursor = *gif;
    int src = texture->src;
    int dbp = texture->dbp;
    int shift = 8;
    if (psm == GM_TEXEL_INDEX8S) {
        fmt = 0;
        w >>= 1;
        height >>= 1;
    } else if (psm == GM_TEXEL_INDEX8) {
        fmt = 27;
        shift = 6;
    } else if (psm == GM_TEXEL_INDEX4S) {
        fmt = 0;
        if (w == height) {
            w >>= 1;
            height >>= 2;
        } else {
            w >>= 2;
            height >>= 1;
        }
    }
    for (int i = 0; i <= texture->levels; i++) {
        int lw = texture->Width() >> i;
        int lh = texture->Height() >> i;
        int size = Max_8170(lw * lh * texture->BPP() / 8, 256);
        int bpp = texture->BPP();
        int pw, ph;
        switch (bpp) {
        case 4:
            pw = 0x80;
            ph = 0x80;
            break;
        case 8:
            pw = 0x80;
            ph = 0x40;
            break;
        case 15:
        case 16:
            pw = 0x40;
            ph = 0x40;
            break;
        case 32:
        default:
            pw = 0x40;
            ph = 0x20;
            break;
        }
        int vram = Max_8170(lw, pw) * Max_8170(lh, ph) * bpp / 8;
        vram += -vram & 0xFF;
        *(cQuad128_8170*)gifCursor = 0x10000006;
        gifCursor[2] = ((ulong)0x10000000 << 32) | 4;
        gifCursor[3] = 0xE;
        gifCursor[4] = ((ulong)dbp << 32) | ((ulong)((w + 63) >> 6) << 48) | ((ulong)fmt << 56);
        gifCursor[5] = 80;
        gifCursor[6] = 0;
        gifCursor[7] = 81;
        gifCursor[8] = w | ((ulong)height << 32);
        gifCursor[9] = 82;
        gifCursor[10] = 0;
        gifCursor[11] = 83;
        int nloop = size >> 4;
        if (nloop < 1) nloop = 1;
        gifCursor[12] = nloop | ((ulong)2 << 58);
        gifCursor[13] = 0;
        int qwc = size >> 4;
        if (qwc < 1) qwc = 1;
        gifCursor[14] = qwc | 0x30000000 | ((ulong)src << 32);
        gifCursor[15] = 0;
        gifCursor[16] = 0x10000002;
        gifCursor[17] = 0;
        gifCursor[18] = i == texture->levels ? ((ulong)0x10000000 << 32) | 0x8001 : ((ulong)0x10000000 << 32) | 1;
        gifCursor[19] = 0xE;
        gifCursor[20] = 0;
        gifCursor[21] = 0x3F;
        src += size;
        dbp += vram >> shift;
        w >>= 1;
        height >>= 1;
        gifCursor += 22;
    }
    ((cTextureMan*)self)->appendFixedPacketRef(&dmaCursor);
    *dma = dmaCursor;
    *gif = gifCursor;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_buildClutUpload);
#ifdef SKIP_ASM
struct sPs2UpTex {
    char pad_0x00[0xC];
    unsigned int psm;       // 0xC
    char pad_0x10[0x10];
    int addr;               // 0x20
    char pad_0x24[0x8];
    int dbp;                // 0x2C
};

struct sPs2UpMgr {
    char pad_0x0[0x8];
    sPs2UpTex* texs[1];     // 0x8
};

/**
 * Builds the GS upload of texture @p idx's CLUT: one image transfer into its CLUT block.
 *
 * Appends to the @p gif chain an A+D BITBLTBUF/TRXPOS/TRXREG/TRXDIR set for the CLUT block (+0x2C) as 32-bit, 8 x 2 (16
 * entries, texel formats 11/12) or 16 x 16 (256 entries), an IMAGE GIF tag with a DMA ref to the CLUT (+0x20) and an
 * A+D TEXFLUSH; then ends the @p dma chain with cTextureMan_appendFixedPacketRef. cTextureMan_ensureContextTextures calls it when the CLUT is not in VRAM.
 *
 * @param idx  the texture id
 * @param dma  the DMA chain cursor
 * @param gif  the upload chain cursor
 */
// PORT: 64-bit `ulong` GIF/DMA packet words.
extern "C" void cTextureMan_buildClutUpload(sPs2UpMgr* self, int idx, ulong** dma, ulong** gif)
{
    sPs2UpTex* tex = self->texs[idx];
    ulong* gifCursor = *gif;
    ulong* dmaCursor = *dma;
    int w;
    int height;
    int qwc;
    if (tex->psm - 11 < 2) {
        w = 8;
        height = 2;
        qwc = 0x10;
    } else {
        w = 0x10;
        height = 0x10;
        qwc = 0x40;
    }
    gifCursor[0] = 0x10000006;
    gifCursor[1] = 0;
    gifCursor[2] = ((ulong)0x10000000 << 32) | 4;
    gifCursor[3] = 0xE;
    gifCursor[4] = ((ulong)tex->dbp << 32) | ((ulong)1 << 48);
    gifCursor[5] = 80;
    gifCursor[6] = 0;
    gifCursor[7] = 81;
    gifCursor[8] = w | ((ulong)height << 32);
    gifCursor[9] = 82;
    gifCursor[10] = 0;
    gifCursor[11] = 83;
    gifCursor[12] = qwc | ((ulong)2 << 58);
    gifCursor[13] = 0;
    gifCursor[14] = 0x30000000 | qwc | ((ulong)tex->addr << 32);
    gifCursor[15] = 0;
    gifCursor[16] = 0x10000002;
    gifCursor[17] = 0;
    gifCursor[18] = ((ulong)0x10000000 << 32) | 0x8001;
    gifCursor[19] = 0xE;
    gifCursor[20] = 0;
    gifCursor[21] = 0x3F;
    ((cTextureMan*)self)->appendFixedPacketRef(&dmaCursor);
    gifCursor += 22;
    *dma = dmaCursor;
    *gif = gifCursor;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_ensureContextTextures);
#ifdef SKIP_ASM
extern "C" int vramHeap_allocBlock(void* heap, int size, int idx, int other, int a4, int a5);
extern "C" void cTextureMan_buildPixelUpload(cTextureMan* self, int idx, ulong** dma, ulong** gif);
extern "C" void cTextureMan_selectContextTexture(cTextureMan* self, int idx, int slot, ulong** dma);
extern "C" ulong* cPS2ScratchPad_claimBuffer(void* ring, unsigned int addr, int size, int flags);
extern "C" void* cPS2ScratchPad_flushBufferTo(void* ring, ulong* p, int a2);
extern void* D_004A4474;

struct sUpNode_8660 {
    int flags;
    int f4[6];
};

struct sUpHeap_8660 {
    char pad_0x0[0x1FF0];
    sUpNode_8660* nodes;    // 0x1FF0
};


struct sUpMgr_8660 {
    char pad_0x0[0x8];
    tTexInfo* texs[0x7D1];   // 0x8
    int state[2];               // 0x1F4C
};

/**
 * Makes sure the textures for GS contexts 1 and 2 are in VRAM (uploading them if needed) and binds them.
 *
 * Does nothing when both ids are negative. For each given texture without a VRAM block (+0x28 < 0) it forgets the
 * context's bound id, allocates a block in the main heap (+0x1F60; +0x4350 with a shift of 2 for GM_TEXEL_INDEX8)
 * protecting the other context's texture, stores the node (+0x30) and address and queues the pixel upload
 * (cTextureMan_buildPixelUpload) in an upload chain opened at *@p gifOut; a CLUT not in VRAM gets a 0x100- or 0x400-byte block (flag
 * 4) and cTextureMan_buildClutUpload. The upload chain is closed into *@p gifOut; when nothing was sent a 1-quadword no-op goes to
 * *@p pp instead. Finally cTextureMan_selectContextTexture emits the register settings for each given texture. The rail/particle batch
 * renderer cPS2DeferManager_buildDmaChains calls it per draw node.
 *
 * @param context1Tex  texture id for context 1 (negative for none)
 * @param context2Tex  texture id for context 2 (negative for none)
 * @param evictLevel  passed to the VRAM allocator as its eviction level (the caller's running draw index)
 * @param pp  the DMA chain cursor
 * @param gifOut  the upload buffer cursor, updated when an upload chain was built
 */
// PORT: 64-bit `ulong` GIF/DMA packet words.
void cTextureMan::ensureContextTextures(int context1Tex, int context2Tex, int evictLevel, ulong** pp, void** gifOut)
{
    ulong* cur = *pp;
    tTexInfo* objs[2];
    if (context1Tex < 0 && context2Tex < 0)
        return;
    objs[0] = context1Tex >= 0 ? ((sUpMgr_8660*)this)->texs[context1Tex] : 0;
    objs[1] = context2Tex >= 0 ? ((sUpMgr_8660*)this)->texs[context2Tex] : 0;
    int sent = 0;
    ulong* gif = 0;
    for (int k = 0; k < 2; k++) {
        int idx = context2Tex;
        if (k == 0)
            idx = context1Tex;
        if (idx < 0)
            continue;
        int other = context1Tex;
        if (k == 0)
            other = context2Tex;
        tTexInfo* texRecord = ((sUpMgr_8660*)this)->texs[idx];
        sUpHeap_8660* heap;
        int shift;
        if (((unsigned int)texRecord->mFormat) == GM_TEXEL_INDEX8) {
            heap = (sUpHeap_8660*)(((char*)this) + 0x4350);
            shift = 2;
        } else {
            heap = (sUpHeap_8660*)(((char*)this) + 0x1F60);
            shift = 0;
        }
        if (texRecord->mTexAddr < 0) {
            ((sUpMgr_8660*)this)->state[k] = -1;
            int n = vramHeap_allocBlock(heap, texRecord->mVRAMSize, idx, other, evictLevel, 1);
            sUpNode_8660* nd = &heap->nodes[n];
            int fl = nd->flags;
            texRecord->mTexBlock = n;
            texRecord->mTexAddr = (fl >> 8) << shift;
            if (gif == 0)
                gif = cPS2ScratchPad_claimBuffer(D_004A4474, *(unsigned int*)gifOut, -1, 0);
            buildPixelUpload(idx, &cur, &gif);
            sent = 1;
        }
        if (((int)texRecord->mCLUTData) != 0 && texRecord->mCLUTAddr < 0) {
            mCLUTTex = -1;
            int n = vramHeap_allocBlock(mMainHeap, ((unsigned int)texRecord->mFormat) - 11 < 2 ? 0x100 : 0x400, idx, other, evictLevel, 0);
            int* nodeFlags = &((sUpHeap_8660*)(((char*)this) + 0x1F60))->nodes[n].flags;
            int fl = *nodeFlags | 4;
            *nodeFlags = fl;
            texRecord->mCLUTBlock = n;
            texRecord->mCLUTAddr = fl >> 8;
            if (gif == 0)
                gif = cPS2ScratchPad_claimBuffer(D_004A4474, *(unsigned int*)gifOut, -1, 0);
            cTextureMan_buildClutUpload((sPs2UpMgr*)this, idx, &cur, &gif);
            sent = 1;
        }
    }
    if (gif != 0)
        *gifOut = cPS2ScratchPad_flushBufferTo(D_004A4474, gif, 4);
    if (!sent) {
        cur[0] = 0x10000001;
        cur[1] = 0;
        cur[2] = 0;
        cur[3] = (ulong)34816 << 45;
        cur += 4;
    }
    if (context1Tex >= 0)
        selectContextTexture(context1Tex, 0, &cur);
    if (context2Tex >= 0)
        selectContextTexture(context2Tex, 1, &cur);
    *pp = cur;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_selectContextTexture);
#ifdef SKIP_ASM
static inline int imax_8970(int a, int b)
{
    return a < b ? b : a;
}

// VRAM bytes of a w x h image: rounded up to whole GS pages, then to 256 bytes
static inline int vramBytes_8970(int w, int h, int bpp)
{
    int pw, ph;
    switch (bpp) {
    case 4:
        pw = 0x80;
        ph = 0x80;
        break;
    case 8:
        pw = 0x80;
        ph = 0x40;
        break;
    case 15:
    case 16:
        pw = 0x40;
        ph = 0x40;
        break;
    case 32:
    default:
        pw = 0x40;
        ph = 0x20;
        break;
    }
    int n = imax_8970(w, pw) * imax_8970(h, ph) * bpp / 8;
    return n + (-n & 0xFF);
}

// TEX0.TW / TEX0.TH of a texture record
static inline int texTW_8970(char* vt)
{
    tTexInfo* t = (tTexInfo*)vt;
    return (t->mTEX0 >> 26) & 0xF;
}

static inline int texTH_8970(char* vt)
{
    tTexInfo* t = (tTexInfo*)vt;
    return (t->mTEX0 >> 30) & 0xF;
}

// VRAM blocks of mip level lv
static inline int levelBlocks_8970(char* vt, int lv, int shift)
{
    tTexInfo* t = (tTexInfo*)vt;
    return vramBytes_8970((1 << texTW_8970(((char*)t))) >> lv, (1 << texTH_8970(((char*)t))) >> lv, t->mBPP) >> shift;
}

/**
 * Appends the GS register settings that select texture @p idx in context @p slot (TEX0, TEX1, MIPTBP1/2), or only the
 * CLUT load when it is already selected.
 *
 * When the context's bound id (+0x1F4C[@p slot]) differs, stores @p idx there and writes a DMA cnt, VIF DIRECT and A+D
 * GIF tag with: for mipmapped textures (+0x24 > 0) MIPTBP1 (0x34/0x35) built from the stored widths (+0x48) and the
 * levels' VRAM blocks, and MIPTBP2 (0x36/0x37, +0x50) when there are 4 or more levels; TEX1 (0x14/0x15, +0x40); and
 * TEX0 (0x06/0x07, +0x38) with the texture's VRAM block and its CLUT block, remembering the texture as the loaded CLUT
 * (+0x1F54) when it has one. When the context already has it but another texture's CLUT is loaded, writes only TEX2
 * (0x16/0x17) with this texture's CLUT. Advances *@p dma. cTextureMan_ensureContextTextures calls it.
 *
 * @param idx  the texture id
 * @param slot  the GS context: 0 for context 1, 1 for context 2
 * @param dma  the DMA chain cursor
 */
// PORT: 64-bit `ulong` GIF/DMA packet words.
void cTextureMan::selectContextTexture(int idx, int slot, ulong** dma)
{
    ulong* packet = *dma;
    int off = slot << 2;
    cTextureMan* bound = (cTextureMan*)((char*)&mBoundTex[0]);
    int* cur = (int*)(((char*)bound) + off);
    if (*cur != idx) {
        *cur = idx;
        tTexInfo* tex = m_pTexInfo[idx].pTexInfo;
        int rTex0, rTex1, rMip1, rMip2;
        if (slot == 0) {
            rTex0 = 6;
            rTex1 = 20;
            rMip1 = 52;
            rMip2 = 54;
        } else {
            rTex0 = 7;
            rTex1 = 21;
            rMip1 = 53;
            rMip2 = 55;
        }
        int shift = tex->mFormat == GM_TEXEL_INDEX8 ? 6 : 8;
        int n;
        if (tex->mNumMips >= 4) {
            n = 4;
        } else {
            n = 2;
            if (tex->mNumMips >= 1)
                n = 3;
        }
        packet[0] = (n + 2) | 0x10000000;
        packet[1] = 0;
        packet[2] = 0;
        unsigned int direct = (n + 1) | 0x50000000;
        packet[3] = (ulong)direct << 32;
        packet[4] = (ulong)(n | 0x8000) | ((ulong)1 << 60);
        packet[5] = 0xE;
        packet += 6;
        if (tex->mNumMips > 0) {
            int tbp1 = tex->mTexAddr + levelBlocks_8970(((char*)tex), 0, shift);
            int tbp2 = tbp1 + levelBlocks_8970(((char*)tex), 1, shift);
            int tbp3 = tbp2 + levelBlocks_8970(((char*)tex), 2, shift);
            packet[0] = tex->mMIPTBP1 | ((ulong)tbp1 | ((ulong)tbp2 << 20) | ((ulong)tbp3 << 40));
            packet[1] = rMip1;
            packet += 2;
            if (tex->mNumMips >= 4) {
                int tbp4 = tbp3 + levelBlocks_8970(((char*)tex), 3, shift);
                int tbp5 = tbp4 + levelBlocks_8970(((char*)tex), 4, shift);
                int tbp6 = tbp5 + levelBlocks_8970(((char*)tex), 5, shift);
                packet[0] = tex->mMIPTBP2 | ((ulong)tbp4 | ((ulong)tbp5 << 20) | ((ulong)tbp6 << 40));
                packet[1] = rMip2;
                packet += 2;
            }
        }
        packet[0] = tex->mTEX1;
        packet[1] = rTex1;
        int caddr = tex->mCLUTAddr;
        int cbp = 0;
        if (caddr >= 0) {
            mCLUTTex = idx;
            cbp = caddr;
        }
        packet[2] = tex->mTEX0 | ((ulong)tex->mTexAddr | ((ulong)cbp << 37));
        packet[3] = rTex0;
        packet += 4;
    } else if (mCLUTTex != idx) {
        tTexInfo* tex = m_pTexInfo[idx].pTexInfo;
        if (tex->mCLUTAddr >= 0) {
            mCLUTTex = idx;
            packet[0] = 0x10000003;
            packet[1] = 0;
            packet[2] = 0;
            packet[3] = (ulong)0x50000002 << 32;
            packet[4] = (ulong)(1 | 0x8000) | ((ulong)1 << 60);
            packet[5] = 0xE;
            packet[6] = tex->mTEX0 | ((ulong)tex->mCLUTAddr << 37);
            packet[7] = slot == 0 ? 22 : 23;
            packet += 8;
        }
    }
    *dma = packet;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_dropVramCopy);
#ifdef SKIP_ASM
extern "C" void vramHeap_freeTextureBlocks(void* list, int i);

/**
 * Drops texture @p idx's VRAM copy so it is uploaded again, and forgets the textures bound to the GS contexts.
 *
 * When the texture has a VRAM block (+0x28 not tTexInfo::kNoVRAM), vramHeap_freeTextureBlocks frees its data and CLUT blocks in the
 * main heap (+0x1F60), or the second heap (+0x4350) for GM_TEXEL_INDEX8. Then sets both per-context bound ids (+0x1F4C)
 * and the loaded-CLUT texture (+0x1F54) to -1. cTextureMan_SetNewTextureData calls it after replacing a texture's data.
 *
 * @param idx  the texture id
 */
void cTextureMan::dropVramCopy(int idx)
{
    tTexInfo* tex = m_pTexInfo[idx].pTexInfo;
    if (tex->mTexAddr != tTexInfo::kNoVRAM) {
        if (tex->mFormat != GM_TEXEL_INDEX8) {
            vramHeap_freeTextureBlocks(mMainHeap, idx);
        } else {
            vramHeap_freeTextureBlocks(mTempHeap, idx);
        }
    }
    int unbound = -1;
    int* bound = (int*)(&mBoundTex[0]);
    for (int i = 1; i >= 0; i--) {
        bound[i] = unbound;
    }
    mCLUTTex = -1;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_setTextureFilter);
#ifdef SKIP_ASM
// PORT: 64-bit `ulong` bitfield (GS TEX1 register layout)
struct sGsTex1 {
    ulong LCM : 1;
    ulong pad0 : 1;
    ulong MXL : 3;
    ulong MMAG : 1;
    ulong MMIN : 3;
    ulong MTBA : 1;
    ulong pad1 : 9;
    ulong L : 2;
    ulong pad2 : 11;
    ulong K : 12;
    ulong pad3 : 20;
};

struct sGfxTex1 {
    char pad[0x40];
    sGsTex1 tex1;
};

struct sGfxTexTable1 {
    int pad[2];
    sGfxTex1* entries[1];
};

/**
 * Sets the filtering fields of texture @p idx's GS TEX1 register value (+0x40): MMIN, MMAG, L and K.
 *
 * Only those four bit fields change. cPS2GraphicsMan_setTextureFilter__FPv calls it on mTexMan.
 *
 * @param idx  the texture id
 * @param mmin  the minification filter (TEX1.MMIN)
 * @param mmag  the magnification filter (TEX1.MMAG)
 * @param lodL  the LOD shift (TEX1.L)
 * @param k  the LOD bias (TEX1.K)
 */
extern "C" void cTextureMan_setTextureFilter(sGfxTexTable1* self, int idx, int mmin, int mmag, int lodL, int k)
{
    sGfxTex1* entry = self->entries[idx];
    sGsTex1 tex1Reg = entry->tex1;
    tex1Reg.L = lodL;
    tex1Reg.K = k;
    tex1Reg.MMIN = mmin;
    tex1Reg.MMAG = mmag;
    entry->tex1 = tex1Reg;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cTextureMan_setTextureFunction);
#ifdef SKIP_ASM
// PORT: 64-bit `ulong` bitfield (GS TEX0 register layout)
struct sGsTex0 {
    ulong TBP0 : 14;
    ulong TBW : 6;
    ulong PSM : 6;
    ulong TW : 4;
    ulong TH : 4;
    ulong TCC : 1;
    ulong TFX : 2;
    ulong CBP : 14;
    ulong CPSM : 4;
    ulong CSM : 1;
    ulong CSA : 5;
    ulong CLD : 3;
};

struct sGfxTex {
    char pad[0x38];
    sGsTex0 tex0;
};

struct sGfxTexTable {
    int pad[2];
    sGfxTex* entries[1];
};

/**
 * Sets the texture function (TEX0.TFX: modulate, decal, highlight, highlight 2) of texture @p idx.
 *
 * Changes only that field of the stored TEX0 value (+0x38); an @p idx of cTextureMan::kNoTexID is ignored.
 * cPS2GraphicsMan_setTextureFunction__FPv calls it on mTexMan.
 *
 * @param idx  the texture id, or cTextureMan::kNoTexID
 * @param tfx  the TFX value (0-3)
 */
extern "C" void cTextureMan_setTextureFunction(sGfxTexTable* self, int idx, int tfx)
{
    if (idx != cTextureMan::kNoTexID) {
        self->entries[idx]->tex0.TFX = tfx;
    }
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cGraphicsMan_cGraphicsMan);
#ifdef SKIP_ASM
extern "C" void memset(void* dst, int c, int n);
extern "C" void MEM_fill(void* dst, unsigned int value, int size);
extern void* cGraphicsMan_vtbl[];

inline void* operator new(unsigned int, void* p) { return p; }

struct sTexPair_691F8 {
    short s[2];
    sTexPair_691F8() { for (unsigned int i = 0; i < 2; i++) s[i] = -1; }
};

struct sRS_691F8 {
    int f0;                     // 0x00
    int flagsA;                 // 0x04
    int flagsB;                 // 0x08
    int fC;                     // 0x0C
    sTexPair_691F8 tex;         // 0x10
    sRS_691F8() { memset(this, 0, 0x10); }
};

struct sGfxPair_691F8 {
    int a, b;
    sGfxPair_691F8() {}
};

struct sGfxState_691F8 {
    sRS_691F8* top;                 // 0x00 (0xE84)
    sRS_691F8 stack[4];             // 0x04 (0xE88)
    sRS_691F8 cur;                  // 0x54 (0xED8)
    int pad68;                      // 0x68 (0xEEC)
    sTexPair_691F8 texs[4];         // 0x6C (0xEF0)
    sTexPair_691F8 tex;             // 0x7C (0xF00)
    sGfxPair_691F8 p0[2];           // 0x80 (0xF04)
    sGfxPair_691F8 p1[2];           // 0x90 (0xF14)
    sGfxState_691F8() {}
};


extern sRS_691F8 sDrawState_s_defaultDrawState;
extern sTexPair_691F8 D_004A5A10[];
extern t2Vector t2vect_Zero[];

static inline void gfxSet0_691F8(sRS_691F8* r, int v, int mask, int shift)
{
    r->f0 = (r->f0 & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

static inline void gfxSetA_691F8(sRS_691F8* r, int v, int mask, int shift)
{
    r->flagsA = (r->flagsA & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

static inline void gfxSetB_691F8(sRS_691F8* r, int v, int mask, int shift)
{
    r->flagsB = (r->flagsB & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

static inline void gfxSetC_691F8(sRS_691F8* r, int v, int mask, int shift)
{
    r->fC = (r->fC & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

static inline void gfxSetTex_691F8(sRS_691F8* r, const sTexPair_691F8& t)
{
    r->tex = t;
}

/**
 * Constructs the base graphics manager and publishes it in cGraphicsMan_TheGraphicsMan.
 *
 * Stores cGraphicsMan_vtbl at +0x10D8, builds the default draw state sDrawState_s_defaultDrawState (depth, alpha, blend and texture
 * fields; texture ids -1), copies it into all 4 draw-state stack entries (+0xE88) and makes the first current, fills
 * the 86-entry effect texture map (+0xF50) with -1 and sets mNumViews (+0x270) to 1.
 *
 * @return @p self
 */
extern "C" void* cGraphicsMan_cGraphicsMan(cGraphicsMan* self)
{
    VPTR_OF(cGraphicsMan, self) = cGraphicsMan_vtbl;
    new ((char*)&self->m_pCurrentDrawState) sGfxState_691F8;
    self->mSkipNextPageFlip = 0;
    cGraphicsMan_TheGraphicsMan = (cPS2GraphicsMan*)(self);
    for (unsigned int i = 0; i < 2; i++) D_004A5A10[0].s[i] = -1;
    memset(&sDrawState_s_defaultDrawState, 0, 0x10);
    gfxSetA_691F8(&sDrawState_s_defaultDrawState, 1, 3, 23);
    gfxSetA_691F8(&sDrawState_s_defaultDrawState, 0, 1, 22);
    gfxSetA_691F8(&sDrawState_s_defaultDrawState, 3, 3, 20);
    gfxSetA_691F8(&sDrawState_s_defaultDrawState, 0, 0xFF, 12);
    gfxSetA_691F8(&sDrawState_s_defaultDrawState, 0, 3, 0);
    gfxSetB_691F8(&sDrawState_s_defaultDrawState, 4, 0x1F, 5);
    gfxSetB_691F8(&sDrawState_s_defaultDrawState, 0, 0x1F, 0);
    gfxSet0_691F8(&sDrawState_s_defaultDrawState, 0, 3, 0);
    gfxSet0_691F8(&sDrawState_s_defaultDrawState, 0, 3, 10);
    gfxSet0_691F8(&sDrawState_s_defaultDrawState, 0, 0xF, 6);
    gfxSetC_691F8(&sDrawState_s_defaultDrawState, 0, 0x3FFFFFFF, 0);
    for (unsigned int k = 0; k < 2; k++) {
        gfxSetA_691F8(&sDrawState_s_defaultDrawState, 5, 0x1F, 2 + k * 5);
        gfxSet0_691F8(&sDrawState_s_defaultDrawState, 0, 0xFF, 12 + k * 8);
    }
    for (unsigned int k = 0; k < 2; k++) {
        gfxSet0_691F8(&sDrawState_s_defaultDrawState, 0, 3, 2 + k * 2);
    }
    gfxSetTex_691F8(&sDrawState_s_defaultDrawState, D_004A5A10[0]);
    for (int k = 0; k < 4; k++) {
        ((sRS_691F8*)(&self->m_drawStateStack[0]))[k] = sDrawState_s_defaultDrawState;
    }
    self->m_pCurrentDrawState = (sDrawState*)((sRS_691F8*)(&self->m_drawStateStack[0]));
    *(t2Vector*)self = t2vect_Zero[0];
    self->m_AIUpdatesPerRender = 0;
    MEM_fill(&self->_EffectTexMap[0], 0xFFFFFFFF, 344);
    self->mNumViews = 1;
    self->mFarPlane = 0.0f;
    self->mViewIndex = 0;
    self->m_blendedMatrixHistory = 0;
    self->m_blendedMatrixHistorySize = 0;
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cGraphicsMan__cGraphicsMan);
#ifdef SKIP_ASM
extern void* cGraphicsMan_vtbl[];
void operator_delete(int* ptr);

/**
 * Destroys the base graphics manager: stores cGraphicsMan_vtbl, clears cGraphicsMan_TheGraphicsMan and frees the
 * object when @p flags bit 0 is set.
 *
 * @param flags  destructor flags; bit 0 frees the object
 */
extern "C" void cGraphicsMan__cGraphicsMan(cGraphicsMan* self, int flags)
{
    VPTR_OF(cGraphicsMan, self) = cGraphicsMan_vtbl;
    cGraphicsMan_TheGraphicsMan = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cGraphicsMan_ForceActiveDrawState);
#ifdef SKIP_ASM

/**
 * Forces the whole draw state to be resent: the cached active state (+0xED8, 4 words) becomes ~mask so every field
 * differs, the cached texture ids become 0x7FFF, then virtual slot 62 applies @p mask.
 *
 * @param mask  the draw state to apply (4 words)
 */
extern "C" void cGraphicsMan_ForceActiveDrawState(cGraphicsMan* self, int* mask)
{
    int i;
    unsigned int j;
    for (i = 0; i < 4; i++) {
        self->m_activeDrawState.m_bitGroups[i] = ~mask[i];
    }
    for (j = 0; j < 2; j++) {
        self->m_activeDrawState.m_textureState.m_texid[j] = 0x7FFF;
    }
    self->SetActiveDrawState(*(const sDrawState*)mask);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cGraphicsMan_SetTextureRefs);
#ifdef SKIP_ASM
extern "C" void* MEM_copy(void*, void*, int);

/**
 * Replaces the named-texture reference table: stores @p count in mNumTexRefs and copies @p count tTextureRefs (0xC
 * bytes each) from @p src into mTexRefs.
 *
 * cFERider_compileModel calls it on the graphics manager after clearing the texture file list (cGraphicsMan_InitTextureFiles).
 *
 * @param count  number of references
 * @param src  the tTextureRef array to copy
 */
void cGraphicsMan::SetTextureRefs(int count, tTextureRef* src)
{
    mNumTexRefs = count;
    MEM_copy(&mTexRefs[0], src, count * 0xC);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cGraphicsMan_GetTextureID);
#ifdef SKIP_ASM
extern "C" int strcmp(const void* a, const void* b);
extern "C" void* SHAPE_locatez(void* src, const char* name);
extern "C" char* strcpy(char* dst, const char* src);
extern char str_charshp_b[];

/**
 * Returns the texture id for a short texture name, loading and caching the texture on first use.
 *
 * Uses up to the first 4 characters of @p name. Searches the texture-ref cache (On Tour mTexRefs: 256 entries of {id,
 * name[8]} at +0x280, count +0xE80) with strcmp (strcmp). On a miss it copies the name into the next entry and
 * asks each loaded texture file (+0x27C, count +0x278) for the shape (SHAPE_locatez); the first hit is added through
 * vtable slot 46 (cPS2GraphicsMan_AddTexture) as a "charshp" texture. A name no file has is cached with id -1. There is no check
 * against the 256-entry limit (On Tour asserts mNumTexRefs < arraysize(mTexRefs)): a 257th distinct name would
 * overwrite the count.
 *
 * @param graphicsMan  the graphics manager (this)
 * @param name   the texture name
 * @return the texture id, or -1 when no texture file has the shape
 */
// Look up (or load and cache) a texture by its 4-character name.
int cGraphicsMan::GetTextureID(const char* name)
{
    cGraphicsMan* self = this;
    char buf[0x10];
    const char* namePtr = name;
    if (*namePtr) {
        char* bufPtr = buf;
        do
            *bufPtr++ = *namePtr++;
        while (*namePtr && namePtr - name < 4);
    }
    buf[namePtr - name] = 0;
    for (int i = 0; i < self->mNumTexRefs; i++) {
        if (strcmp(buf, self->mTexRefs[i].Name) == 0)
            return self->mTexRefs[i].TextureID;
    }
    strcpy(self->mTexRefs[self->mNumTexRefs].Name, buf);
    for (int j = 0; j < self->mNumTextureFiles; j++) {
        void* shape = SHAPE_locatez(self->mTextureFiles[j], self->mTexRefs[self->mNumTexRefs].Name);
        if (shape != 0) {
            sVEntry<int(void*, void*, const char*, int, int, int)>* vt =
                (sVEntry<int(void*, void*, const char*, int, int, int)>*)VPTR_OF(cGraphicsMan, self);
            self->mTexRefs[self->mNumTexRefs].TextureID =
                vt[46].fn(vthis(self, vt[46]), shape, str_charshp_b, 0, 1, -1);
            return self->mTexRefs[self->mNumTexRefs++].TextureID;
        }
    }
    self->mTexRefs[self->mNumTexRefs].TextureID = -1;
    return self->mTexRefs[self->mNumTexRefs++].TextureID;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cGraphicsMan_InitTextureFiles__FPvii);
#ifdef SKIP_ASM
/**
 * Sets the list of texture files that cGraphicsMan_GetTextureID searches and clears the cached references.
 *
 * Stores @p fileNames in mTextureFiles and @p numFiles in mNumTextureFiles, and sets mNumTexRefs to 0.
 * cFERider_compileModel calls it with an empty list.
 *
 * @param numFiles  number of texture file names
 * @param fileNames  the array of file names (const char**, held as an int)
 */
void cGraphicsMan::InitTextureFiles(int numFiles, int fileNames)
{
    // ORIGINAL-PTR: the retail symbol cGraphicsMan_InitTextureFiles__FPvii mangles the file-name list as int, so the
    // original converted it back to the pointer too
    mTextureFiles = (void**)fileNames;
    mNumTextureFiles = numFiles;
    mNumTexRefs = 0;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cGraphicsMan_ResetBlendedMatrixHistory);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

/**
 * Empties the blended-matrix history: frees m_blendedMatrixHistory (if any) and zeroes it and
 * m_blendedMatrixHistorySize.
 *
 * cAnimModel_compile calls it; cGraphicsMan_AddBlendedMatrix fills the history again.
 */
void cGraphicsMan::ResetBlendedMatrixHistory()
{
    void* history = m_blendedMatrixHistory;
    if (history != 0) {
        cMemMan_free(history);
    }
    m_blendedMatrixHistory = 0;
    m_blendedMatrixHistorySize = 0;
}
#endif

