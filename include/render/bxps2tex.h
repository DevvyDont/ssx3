#ifndef RENDER_BXPS2TEX_H
#define RENDER_BXPS2TEX_H

// cTextureMan: the PS2 renderer's texture manager (cPS2GraphicsMan::mTexMan, one instance). It owns the texture
// id table (ids 0..1499 are "bound" ids the caller picks, 1500..1999 "non-bound" ids taken from a free list), one
// tTexInfo record per id, the GS register state of the two drawing contexts, and two GS-memory block heaps the
// textures are uploaded into on demand. No vtable, no base class.
// EA source: d:/ssx/code/source/render/bxps2tex.cpp (July 2003 preview: its assert strings "m_pNonBindFreeTexInfo",
// "sizeof(cLinkTexInfo)==4", "(texid>=kMaxVirtualBindTextures)&& (texid<kMaxTextures)", "m_iNumBindTextures==0",
// "!m_pTexInfo[texid].pTexInfo", "Tex.mTexAddr != -1", "Tex.mTexBlock >= 0", ... sit with that file name, after the
// VRAM-heap asserts "mFree.mNext >= 0", "0 <= BlockIdx && BlockIdx < kMaxVRAMBlocks", "Block->mBlockAddr & VRB_USED").
// The preview's render/ps2graphicsman.cpp names the owner's member mTexMan ("mTexMan->GetTextureWidth(iTexId) ==
// pShp->width"); its accessor is cPS2GraphicsMan::GetTextureMan (preview mangled GetTextureMan__15cPS2GraphicsMan).
// CLASS NAME: EA's identifier is not attested. "cTextureMan" follows the accessor (GetTextureMan) and the allocation tag
// ("TEX MAN"), and is the name render/ps2graphicsman.h already forward-declares for mTexMan. (SSX Tricky's
// symbol list, refs/ssxdvd, has a cPS2TexMan of a different layout; Tricky also allocates "TEX MAN".)
//
// Size 0x6740 [R]: cPS2GraphicsMan_Init does cMemMan_alloc(0x6740, "TEX MAN"), stores this + 0x1F60 at +0x3F50 and
// this + 0x4350 at +0x6340 (each heap's block-table pointer, i.e. the heaps' inlined constructors), calls cTextureMan_init
// (the init: TexMemType 0, cTextureMan_InitTexInfo, bound ids -1, both heaps set up by vramHeap_init) and stores the object in
// cPS2GraphicsMan::mTexMan (+0x18F4). cPS2GraphicsMan_Shutdown purges it (cTextureMan_purgeAll) and deletes it.
// Retail methods: render/ps2graphicsman 0x367150..0x369230 (the six cPSPGraphicsMan_* functions there are this class's
// NewNonBindTexID .. PurgeBindTexIDs, named after On Tour, where the PSP port folded them into cPSPGraphicsMan), the
// tail of object/railmodifier 0x366E90..0x367138 (init / purge / reset), and cTextureMan_resetBoundTextures (render/font).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] cPS2GraphicsMan_Init (allocation, heap pointers), the init cTextureMan_init / cTextureMan_InitTexInfo, the record allocations
//       cMemMan_alloc(0x58, "NBTexInfo" / "BTexInfo");
//   [P] the July 2003 preview's assert strings in bxps2tex.cpp (member names only; no out-of-line accessor of this class
//       exists in the preview, so no offsets);
//   [O] SSX On Tour's DWARF: cPSPGraphicsMan carries the same id members in the same order (TexMemType 0x16B8,
//       m_iNumBindTextures 0x16BC, m_iNumNonBindTextures 0x16C0, tLinkTexInfo m_pTexInfo[2048] 0x16C4,
//       m_pNonBindFreeTexInfo 0x36C4); offsets differ;
//   [U] how retail code uses the field;
//   [X] refs/ssx3-rust docs/investigation/structs/render.md "TexMan" (names are guesses).
// The two block heaps (EA's VRAM heap class, name unknown; preview members mBlocks / mFree / mTexStats, kMaxVRAMBlocks
// 292 blocks of 0x1C bytes: address + flags VRB_USED 2 / VRB_LOCKED 0x20, size, owner id, address and size-list links)
// are left as raw bytes here; their methods are vramHeap_init..vramHeap_freeTextureBlocks (object/railmodifier).

#include "layout.h"

// One texture's record (0x58 [R]: cMemMan_alloc(0x58, "NBTexInfo") in cTextureMan_NewNonBindTexID, "BTexInfo" in
// cTextureMan_NewBindTexID). Type name: On Tour's tTexInfo (the record tLinkTexInfo points to; a different 0x14 PSP
// layout there) - the retail type name is not attested (the preview's asserts call a record variable "Tex").
// Filled by cTextureMan_AddTexture (create), cTextureMan_ensureContextTextures (VRAM allocation on bind); 64-bit fields are GS register values.
struct tTexInfo {
    int mMemAlloc;          // 0x00 [P] "Tex.mMemAlloc==bCopyMem"; [U] cTextureMan_AddTexture stores its ownsData argument;
                            //      cTextureMan_RemoveTexture frees mpTexels / mCLUTData only when set
    char mName[8];          // 0x04 (our name) [U] cTextureMan_AddTexture strncpy(mName, name, 8)
    int mFormat;            // 0x0C (our name) texel format, eGMTexelFormat (render/graphicsman.h);
                            //      [U] cTextureMan_AddTexture stores it; cTextureMan_unlockTexture / cTextureMan_dropVramCopy / cTextureMan_RemoveTexture /
                            //      cTextureMan_ensureContextTextures pick the second heap for 9; cTextureMan_rearrangeMipLevels stores 10
    int mSize;              // 0x10 [P] "Size <= Tex.mSize"; [U] cTextureMan_AddTexture sums the data size of all levels;
                            //      cTextureMan_SetNewTextureData copies that many bytes; cTextureMan_rearrangeMipLevels sizes its temp buffer by it
    int mVRAMSize;          // 0x14 (our name) [U] cTextureMan_AddTexture (page-rounded VRAM bytes); cTextureMan_ensureContextTextures allocates it;
                            //      cTextureMan_AddTexture compares it with the second heap's free size (+0x6344)
    int mBPP;               // 0x18 (our name; the preview's Tex.BPP()) [U] cTextureMan_AddTexture stores the GS format's bits
                            //      per pixel; cTextureMan_buildPixelUpload / cTextureMan_rearrangeMipLevels / cTextureMan_SetNewTextureData read it
    void* mpTexels;         // 0x1C (our name) [U] pixel data (all levels): cTextureMan_AddTexture copies or keeps it,
                            //      cTextureMan_SetNewTextureData overwrites it, cTextureMan_buildPixelUpload DMA-refs it, cTextureMan_RemoveTexture frees it
    void* mCLUTData;        // 0x20 [P] "Tex.mCLUTData"; [U] cTextureMan_AddTexture, cTextureMan_SetNewTextureData, cTextureMan_buildClutUpload (DMA ref),
                            //      cTextureMan_RemoveTexture frees it
    int mNumMips;           // 0x24 (our name) [U] extra mip levels: cTextureMan_AddTexture stores it; cTextureMan_buildPixelUpload /
                            //      cTextureMan_selectContextTexture / cTextureMan_rearrangeMipLevels loop 0..mNumMips
    enum { kNoVRAM = -1 };  // mTexAddr / mCLUTAddr / mTexBlock / mCLUTBlock while not in VRAM (name ours; [P] below;
                            //      [R] cTextureMan_AddTexture stores it in all four)
    int mTexAddr;           // 0x28 [P] "Tex.mTexAddr != -1", "!(Tex.mTexAddr & 0x1f)"; [U] -1 = not in VRAM
                            //      (cTextureMan_AddTexture sets -1; cTextureMan_unlockTexture / cTextureMan_dropVramCopy / cTextureMan_ensureContextTextures test it;
                            //      cTextureMan_ensureContextTextures stores the block address; cTextureMan_LockFrameToTexture builds FRAME_1 from it)
    int mCLUTAddr;          // 0x2C (our name) [U] CLUT VRAM address: cTextureMan_AddTexture -1; cTextureMan_ensureContextTextures stores;
                            //      cTextureMan_buildClutUpload uploads to it; cTextureMan_selectContextTexture writes it into TEX0.CBP
    int mTexBlock;          // 0x30 [P] "Tex.mTexBlock >= 0"; [U] the texture's block index in its heap's block table
                            //      (cTextureMan_unlockTexture / cTextureMan_RemoveTexture / cTextureMan_ensureContextTextures)
    int mCLUTBlock;         // 0x34 [P] "Tex.mCLUTBlock >= 0"; [U] the CLUT's block index (cTextureMan_RemoveTexture / cTextureMan_ensureContextTextures)
    unsigned long mTEX0;    // 0x38 (our name; GS TEX0, 64-bit) [P] Tex.Width() / Tex.Height() decode its TW / TH;
                            //      [U] cTextureMan_AddTexture builds it, cTextureMan_selectContextTexture writes it (reg 0x06 / 0x07),
                            //      cTextureMan_setTextureFunction sets TFX, cTextureMan_rearrangeMipLevels / cTextureMan_LockFrameToTexture / cTextureMan_ShadowClearLockTexture decode it
    unsigned long mTEX1;    // 0x40 (our name; GS TEX1) [U] cTextureMan_AddTexture, cTextureMan_selectContextTexture (reg 0x14 / 0x15),
                            //      cTextureMan_setTextureFilter sets MMIN / MMAG / L / K
    unsigned long mMIPTBP1; // 0x48 (our name; GS MIPTBP1 level widths) [U] cTextureMan_AddTexture, cTextureMan_selectContextTexture (0x34 / 0x35)
    unsigned long mMIPTBP2; // 0x50 (our name; GS MIPTBP2) [U] cTextureMan_AddTexture, cTextureMan_selectContextTexture (0x36 / 0x37, 4+ levels)
};

// An id slot (4 bytes [P] "sizeof(cLinkTexInfo)==4"; On Tour tLinkTexInfo, an anonymous union): a used id holds its
// record, a free non-bound id holds the next free slot.
union cLinkTexInfo {
    tTexInfo* pTexInfo;         // [P] "m_pTexInfo[texid].pTexInfo"; [U] cTextureMan_NewNonBindTexID / NewBindTexID
                                //      store the new record, Delete* free it
    cLinkTexInfo* pNextFree;    // (our name) [U] cTextureMan_InitTexInfo chains slots 1500..1999; cTextureMan_NewNonBindTexID
                                //      pops, cTextureMan_DeleteNonBindTexID pushes
};

struct cTextureMan {
    int m_iNumBindTextures;              // 0x0000 [O]; [P] "m_iNumBindTextures==0"; [R] cTextureMan_InitTexInfo 0;
                                         //        [U] cTextureMan_NewBindTexID ++, cTextureMan_DeleteBindTexID --
    int m_iNumNonBindTextures;           // 0x0004 [O]; [R] cTextureMan_InitTexInfo 0; [U] cTextureMan_NewNonBindTexID ++,
                                         //        cTextureMan_DeleteNonBindTexID --
    cLinkTexInfo m_pTexInfo[2000];       // 0x0008 [O][P] (kMaxTextures 2000; ids < 1500 = kMaxVirtualBindTextures are
                                         //        bound); [R] cTextureMan_InitTexInfo MEM_fill()s 8000 bytes; [U] every method
                                         //        indexes it by texture id (self + 8 + id * 4)
    cLinkTexInfo* m_pNonBindFreeTexInfo; // 0x1F48 [O][P]; [R] cTextureMan_InitTexInfo: &m_pTexInfo[1500]; [U]
                                         //        cTextureMan_NewNonBindTexID pops, DeleteNonBindTexID pushes
    // No texture (names ours): [U] mBoundTex / mCLUTTex are reset to kNoTexID, cTextureMan_setTextureFunction ignores it as an id and
    // cTextureMan_AddTexture returns it for a format it cannot store.
    enum { kNoTexID = -1 };
    // [U] tex_rearrange4BitTexture / tex_rearrange8BitTexture accept a texture whose sides are one of the kNumTexSides powers of two
    // 1024 >> 0 .. 1024 >> 10 (name ours).
    enum { kNumTexSides = 11 };
    int mBoundTex[2];                    // 0x1F4C (our name; [X] boundTex) texture id selected in GS context 1 / 2;
                                         //        [R] cTextureMan_init -1; [U] cTextureMan_selectContextTexture compares / stores;
                                         //        cTextureMan_ensureContextTextures, cTextureMan_purgeAll, cTextureMan_freeAllHeapBlocks, cTextureMan_RemoveTexture,
                                         //        cTextureMan_dropVramCopy, cTextureMan_resetBoundTextures reset both to -1
    int mCLUTTex;                        // 0x1F54 (our name) texture whose CLUT is loaded in the GS; [R] cTextureMan_init -1;
                                         //        [U] cTextureMan_selectContextTexture stores / compares; reset -1 by the same functions
    int TexMemType;                      // 0x1F58 [O] (On Tour: the member before m_iNumBindTextures; placed by [U]);
                                         //        [R] cTextureMan_init 0; [U] cTextureMan_SetTextureMemType stores the memory type
                                         //        cPS2GraphicsMan_InitTextures passes; cTextureMan_AddTexture / cTextureMan_rearrangeMipLevels
                                         //        allocate texture copies with it
    char unk_1F5C[0x4];                  // 0x1F5C no access found (padding before the heaps?)
    char mMainHeap[0x23F0];              // 0x1F60 (our name) GS-memory block heap over the 4 MB of VRAM: [R]
                                         //        cTextureMan_init vramHeap_init(this + 0x1F60, 0, 0x400000, m_pTexInfo);
                                         //        cPS2GraphicsMan_Init stores this + 0x1F60 at its block-table pointer
                                         //        (+0x1FF0 = 0x3F50); [U] cTextureMan_allocLockedPages / cTextureMan_ensureContextTextures / cTextureMan_AddTexture
                                         //        read 0x3F50; cTextureMan_purgeAll / cTextureMan_freeAllHeapBlocks / cTextureMan_unlockTexture /
                                         //        cTextureMan_RemoveTexture / cTextureMan_dropVramCopy pass it to the heap functions
    char mTempHeap[0x23F0];              // 0x4350 (our name; [X] tempVram) the second heap (8-bit format-9 textures, over
                                         //        the display buffers): [R] cTextureMan_init vramHeap_init(this + 0x4350, 0,
                                         //        0x1000, m_pTexInfo); cPS2GraphicsMan_Init stores this + 0x4350 at +0x6340;
                                         //        [U] cTextureMan_initTempHeap re-initialises it; cTextureMan_AddTexture reads its size
                                         //        (+0x1FF4 = 0x6344)

    // Methods bound to their retail symbols by asm labels.
    void SetTextureMemType(int val) __asm__("cTextureMan_SetTextureMemType__FPvi");
    void InitTexInfo() __asm__("cTextureMan_InitTexInfo");
    int NewNonBindTexID() __asm__("cTextureMan_NewNonBindTexID");
    void DeleteNonBindTexID(int id) __asm__("cTextureMan_DeleteNonBindTexID");
    void NewBindTexID(int id) __asm__("cTextureMan_NewBindTexID");
    void DeleteBindTexID(int idx) __asm__("cTextureMan_DeleteBindTexID");
    int AddTexture(void* pixelData, void* texName, int texWidth, int texHeight, int mipCount, int texelFormat,
                   int clutFormat, void* clutData, int vramMode, int ownsData, int bindId)
        __asm__("cTextureMan_AddTexture");
    void RemoveTexture(int id) __asm__("cTextureMan_RemoveTexture");
    int AddLockedFrameTexture(int width, int height, int format) __asm__("cTextureMan_AddLockedFrameTexture");
    void SetNewTextureData(int idx, void* data, int width, int height, int mipCount, int texelFormat, int clutFormat,
                           void* clut, int upload)
        __asm__("cTextureMan_SetNewTextureData");
    void rearrangeMipLevels(tTexInfo* tex) __asm__("cTextureMan_rearrangeMipLevels");
    void init() __asm__("cTextureMan_init");
    void freeAllHeapBlocks() __asm__("cTextureMan_freeAllHeapBlocks");
    void* initTempHeap(int start, int size) __asm__("cTextureMan_initTempHeap__FPvii");
    void purgeAll() __asm__("cTextureMan_purgeAll");
    int allocLockedPages(int size) __asm__("cTextureMan_allocLockedPages");
    void unlockTexture(int i) __asm__("cTextureMan_unlockTexture");
    void appendFixedPacketRef(ulong** pkt) __asm__("cTextureMan_appendFixedPacketRef");
    void buildPixelUpload(int idx, ulong** dma, ulong** gif) __asm__("cTextureMan_buildPixelUpload");
    void ensureContextTextures(int context1Tex, int context2Tex, int evictLevel, ulong** pp, void** gifOut)
        __asm__("cTextureMan_ensureContextTextures");
    void selectContextTexture(int idx, int slot, ulong** dma) __asm__("cTextureMan_selectContextTexture");
    void dropVramCopy(int idx) __asm__("cTextureMan_dropVramCopy");
    void resetBoundTextures() __asm__("cTextureMan_resetBoundTextures");
};

CHECK_OFFSET(tTexInfo, mMemAlloc, 0x00);
CHECK_OFFSET(tTexInfo, mName, 0x04);
CHECK_OFFSET(tTexInfo, mFormat, 0x0C);
CHECK_OFFSET(tTexInfo, mSize, 0x10);
CHECK_OFFSET(tTexInfo, mVRAMSize, 0x14);
CHECK_OFFSET(tTexInfo, mBPP, 0x18);
CHECK_OFFSET(tTexInfo, mpTexels, 0x1C);
CHECK_OFFSET(tTexInfo, mCLUTData, 0x20);
CHECK_OFFSET(tTexInfo, mNumMips, 0x24);
CHECK_OFFSET(tTexInfo, mTexAddr, 0x28);
CHECK_OFFSET(tTexInfo, mCLUTAddr, 0x2C);
CHECK_OFFSET(tTexInfo, mTexBlock, 0x30);
CHECK_OFFSET(tTexInfo, mCLUTBlock, 0x34);
CHECK_OFFSET(tTexInfo, mTEX0, 0x38);
CHECK_OFFSET(tTexInfo, mTEX1, 0x40);
CHECK_OFFSET(tTexInfo, mMIPTBP1, 0x48);
CHECK_OFFSET(tTexInfo, mMIPTBP2, 0x50);
CHECK_SIZE(tTexInfo, 0x58);

CHECK_SIZE(cLinkTexInfo, 0x4);

CHECK_OFFSET(cTextureMan, m_iNumBindTextures, 0x0000);
CHECK_OFFSET(cTextureMan, m_iNumNonBindTextures, 0x0004);
CHECK_OFFSET(cTextureMan, m_pTexInfo, 0x0008);
CHECK_OFFSET(cTextureMan, m_pNonBindFreeTexInfo, 0x1F48);
CHECK_OFFSET(cTextureMan, mBoundTex, 0x1F4C);
CHECK_OFFSET(cTextureMan, mCLUTTex, 0x1F54);
CHECK_OFFSET(cTextureMan, TexMemType, 0x1F58);
CHECK_OFFSET(cTextureMan, unk_1F5C, 0x1F5C);
CHECK_OFFSET(cTextureMan, mMainHeap, 0x1F60);
CHECK_OFFSET(cTextureMan, mTempHeap, 0x4350);
CHECK_SIZE(cTextureMan, 0x6740);

#endif /* RENDER_BXPS2TEX_H */
