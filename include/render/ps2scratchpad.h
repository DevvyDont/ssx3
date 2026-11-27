#ifndef RENDER_PS2SCRATCHPAD_H
#define RENDER_PS2SCRATCHPAD_H

// cPS2ScratchPad: the scratchpad (SPR, 0x70000000) manager: it splits the 16 KB scratchpad into up to three DMA blocks
// and moves them to and from main memory in turn. EA's source: d:/ssx/code/source/render/ps2scratchpad.cpp (preview
// assert strings). The game's instance is the global D_004A4474 (SPRMan in EA's asserts), which points at the 0x38-byte
// object D_00504F80 (the next symbol, cSinCosTable_gSinCosTable, is at 0x00504FB8).
//
// Retail code (render/lightman unit): func_0038F300 (set the block layout for a mode 1..7), func_0038F460 (open the next
// block), func_0038F4F8 (open and fetch), func_0038F560 / func_0038F598 (flush the current block to main memory),
// func_0038F668 (close); callers in render/graphicsman, render/particle, render/bezierman, object/railmodifier.
// The preview's ps2scratchpad.cpp functions at 0x36FF00 (= func_0038F460: asserts "!((int)Addr & 0xf)",
// "Size == -1 || !(Size & 0xf)", "Size <= mBlockSize" against the word at +0x10) and 0x370108 (= func_0038F598:
// asserts "!(Size & 0xf)", "Size > 0", "Size <= mBlockSize", "mBlockAddr[mDMABlock] != NULL" on the word at
// +0x14 + 4 * (word +0xC)) place the named members. The class name: [P] "cPS2ScratchPad::GetTempSize() >= MaxSize"
// and the same file's "Size <= GetTempSize()", SPRMan->GetTempSize() / GetMaxBlockSize().
//
// Evidence tags: [P] the July 2003 preview (asserts above, same code shape as retail); [R] retail code and the size of
// the global object; [U] how retail code uses the member. Members without an EA name are marked "name ours" or keep
// unk_ names.

#include "layout.h"

struct cPS2ScratchPad {
    // The block layouts func_0038F300 sets up (names ours, after what it does: block count x block size in quadwords).
    // [U] func_0038F300; func_00363490 (object/railmodifier) switches to kLayout_2x256 and back.
    enum eLayout {
        kLayout_1x1024 = 1, kLayout_2x512 = 2, kLayout_2x448 = 3, kLayout_2x384 = 4, kLayout_2x256 = 5,
        kLayout_3x320 = 6, kLayout_3x256 = 7
    };
    int unk_00;                     // 0x00 [U] func_0038F300: the current layout mode, eLayout (returns when unchanged)
    int mNumBlocks;                 // 0x04 (name ours) [U] func_0038F300: the number of blocks (1..3);
                                    //      func_0038F460 / func_0038F598 wrap the block indices with it
    int unk_08;                     // 0x08 [U] func_0038F460: the next block to open (post-incremented, wrapped)
    int mDMABlock;                  // 0x0C [P] "mBlockAddr[mDMABlock] != NULL"; [U] func_0038F598: the block being
                                    //      flushed (advanced, wrapped)
    int mBlockSize;                 // 0x10 [P] "Size <= mBlockSize"; [U] func_0038F300: the block size in quadwords
                                    //      (0x400 .. 0x100); func_0038F460's default size
    char* mBlockAddr[3];            // 0x14 [P] "mBlockAddr[mDMABlock] != NULL"; [U] func_0038F300: 0x70000000 +
                                    //      i * mBlockSize * 16; func_0038F460 returns one
    unsigned int mBlockMainAddr[3]; // 0x20 (name ours) [U] func_0038F460: the main-memory address of each open
                                    //      block; func_0038F598 DMAs the block there
    int unk_2C[3];                  // 0x2C [U] func_0038F460: each open block's size in quadwords; func_0038F560 flushes
                                    //      with it
};

CHECK_OFFSET(cPS2ScratchPad, unk_00, 0x00);
CHECK_OFFSET(cPS2ScratchPad, mNumBlocks, 0x04);
CHECK_OFFSET(cPS2ScratchPad, unk_08, 0x08);
CHECK_OFFSET(cPS2ScratchPad, mDMABlock, 0x0C);
CHECK_OFFSET(cPS2ScratchPad, mBlockSize, 0x10);
CHECK_OFFSET(cPS2ScratchPad, mBlockAddr, 0x14);
CHECK_OFFSET(cPS2ScratchPad, mBlockMainAddr, 0x20);
CHECK_OFFSET(cPS2ScratchPad, unk_2C, 0x2C);
CHECK_SIZE(cPS2ScratchPad, 0x38);

#endif /* RENDER_PS2SCRATCHPAD_H */
