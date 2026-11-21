#ifndef VISUALFX_PS2LENSFX_H
#define VISUALFX_PS2LENSFX_H

// cPS2LensFxMan: the PS2 lens-flare manager. It adds the halo visibility test cLensFxMan leaves pure virtual
// (sampleZBuffer, slot 4): read a halo's depth-buffer rectangle back from the GS and count the samples in front of it.
// EA's source: d:/ssx/code/source/visualfx/ps2lensfx.cpp (preview assert strings); retail code: cLensFxMan_construct,
// cPS2LensFxMan_cPS2LensFxMan, cPS2LensFxMan__cPS2LensFxMan__FPv and cPS2LensFxMan_sampleZBuffer in the visualfx/lensfx
// splat unit (0x2EC3E0 ... 0x2EC9D0). On Tour (a PSP build) has cPSPLensFxMan instead, with no data of its own.
//
// Size 0x6820: [R] cLensFxMan_construct: cMemMan_alloc(0x6820, "LensFX") + cPS2LensFxMan_cPS2LensFxMan; the last
// region (the read-back buffer at 0x6510) runs to the end.
// Base: cLensFxMan (visualfx/lensfx.h, 0x64A0, vptr at 0x649C). The ctor runs cLensFxMan_cLensFxMan and stores
// cPS2LensFxMan_vtbl (0x00487F00, 4 slots: 1 dtor cPS2LensFxMan__cPS2LensFxMan__FPv, 2 cLensFxMan_beginDrawFrame,
// 3 cLensFxMan_endDrawFrame, 4 cPS2LensFxMan_sampleZBuffer); the dtor stores it again and tail-calls
// cLensFxMan__cLensFxMan.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Evidence tags: [R] retail constructor / destructor / allocation; [U] how retail code uses the field
// (cPS2LensFxMan_sampleZBuffer's asm; no C body yet); [O] SSX On Tour's DWARF (for the cLensFxMan base, see
// visualfx/lensfx.h); [P] the preview's cLensFxMan accessors (base fields).

#include "layout.h"
#include "visualfx/lensfx.h"

struct cPS2LensFxMan {
    // --- cLensFxMan (0x64A0; visualfx/lensfx.h)
    tViewInfo mViewInfo;            // 0x000 [O]; [U] cLensFxMan_ProcessLens fills it
    int mNumHalo[2];                // 0x060 [O] (per view); [R] cLensFxMan_cLensFxMan zeroes both
    cLightHalo mLightHalo[2][256];  // 0x068 [O] (per view); [R] cHalo_cHalo on each
    char unk_6068[0x8];             // 0x6068 padding (cSunHalo is 16-aligned)
    cSunHalo mSunHalo[2];           // 0x6070 [O] (per view); [R] cSunHalo_cSunHalo on both
    int mNumViews;                  // 0x6490 [O]; [P] SetNumViews
    int mbLocked;                   // 0x6494 [O] (bool, a word); [P] Lock; [R] cLensFxMan_cLensFxMan sets 1
    int unk_6498;                   // 0x6498 no retail access (On Tour: int mTexID)
    void** mVtbl;                   // 0x649C [R] cPS2LensFxMan_vtbl (cPS2LensFxMan_cPS2LensFxMan, the dtor)
    // --- cPS2LensFxMan
    unsigned long mFinishPacket[4]; // 0x64A0 [U] sampleZBuffer builds a 2-qword GIF packet here (GIFtag NLOOP 1, EOP,
                                    //       A+D; then FINISH = 0) and sends it with func_00371940(this+0x64A0, 2, 6)
                                    //       before the read-back (EE long = 8 bytes)
    unsigned long mTransferPacket[10]; // 0x64C0 [U] sampleZBuffer's 5-qword GIF packet (GIFtag NLOOP 4, EOP, A+D;
                                    //       BITBLTBUF 0x50 built from the graphics manager's current buffer, TRXPOS
                                    //       0x51 / TRXREG 0x52 from the halo's mZSRect, TRXDIR 0x53 = 1, local ->
                                    //       host); sent with func_00371940(this+0x64C0, 5, 6)
    unsigned char mZBuffer[0x310];  // 0x6510 [U] sampleZBuffer points VIF1 DMA (D1_MADR) here and reads the
                                    //       rectangle's 24-bit z samples back (3 bytes each; at most 16 x 16 = 0x300
                                    //       bytes are used), then counts those at or below the halo's mZDist
};

CHECK_OFFSET(cPS2LensFxMan, mViewInfo, 0x000);
CHECK_OFFSET(cPS2LensFxMan, mNumHalo, 0x060);
CHECK_OFFSET(cPS2LensFxMan, mLightHalo, 0x068);
CHECK_OFFSET(cPS2LensFxMan, unk_6068, 0x6068);
CHECK_OFFSET(cPS2LensFxMan, mSunHalo, 0x6070);
CHECK_OFFSET(cPS2LensFxMan, mNumViews, 0x6490);
CHECK_OFFSET(cPS2LensFxMan, mbLocked, 0x6494);
CHECK_OFFSET(cPS2LensFxMan, unk_6498, 0x6498);
CHECK_OFFSET(cPS2LensFxMan, mVtbl, 0x649C);
CHECK_OFFSET(cPS2LensFxMan, mFinishPacket, 0x64A0);
CHECK_OFFSET(cPS2LensFxMan, mTransferPacket, 0x64C0);
CHECK_OFFSET(cPS2LensFxMan, mZBuffer, 0x6510);
CHECK_SIZE(cPS2LensFxMan, 0x6820);

#endif /* VISUALFX_PS2LENSFX_H */
