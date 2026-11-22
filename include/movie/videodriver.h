#ifndef MOVIE_VIDEODRIVER_H
#define MOVIE_VIDEODRIVER_H

// RCMP::VIDEO_DRIVER (abstract: draws decoded RCMP frames) and SSX_VD_PS2_SINGLE_BUFFERD, SSX 3's PS2 driver: it cuts
// the movie frame into 16-pixel-wide columns of power-of-two-high strips, keeps one graphics-manager texture per tile
// and draws them as screen quads. cMoviePlayer_startPlay creates it with SSX_VD_create (84 bytes from the RCMP
// allocator, constructor SSX_VD_PS2_SINGLE_BUFFERD_SSX_VD_PS2_SINGLE_BUFFERD) and calls its slots 4-6 (Draw / ReDraw / CompleteDraw); cMoviePlayer_purge
// deletes it (slot 1, flags 3). Retail code: unit mem/memstd, SSX_VD_create .. VIDEO_DRIVER__VIDEO_DRIVER
// (0x254368..0x2552A0), next to cMoviePlayer's methods. EA source: the July 2003 preview keeps this code with
// d:/ssx/code/source/movie/movieplayer.cpp's; On Tour's twin is in movie/all_in_one_movie.cpp (VIDEO_DRIVER,
// SSX_VD_PSP_ONE_TEXTURE, SSX_VD_create(DECODER*, FRAME*, cGraphicsMan*, eGMDrawLayer)).
//
// Names: RCMP::VIDEO_DRIVER from the preview's linkonce sections CompleteDraw__Q24RCMP12VIDEO_DRIVER,
// _$_Q24RCMP12VIDEO_DRIVER, _vt$Q24RCMP12VIDEO_DRIVER and On Tour's class VIDEO_DRIVER {DECODER* m_pdecoder} (0x8, vptr
// at +0x4); flattened as RCMP_VIDEO_DRIVER (the project's functions say VIDEO_DRIVER__VIDEO_DRIVER /
// VIDEO_DRIVER_CompleteDraw__FPv). SSX_VD_PS2_SINGLE_BUFFERD from the preview's _vt$25SSX_VD_PS2_SINGLE_BUFFERD and its
// operator new / delete linkonces: that vtable has retail D_00480270's six slots in the same order, and the preview's
// slot bodies match retail's (SetPosition 3 floats, SetScale 2, the dtor walks the tile table at +0x14 / +0x18 and
// releases +0xC to the decoder at +0x0, CompleteDraw polls graphics-manager slot 18 through +0x8); the preview's
// position / scale sit 4 bytes earlier (0x3C..0x4C: one word less somewhere in 0x24..0x3C).
//
// Vtables: D_004802B0 RCMP::VIDEO_DRIVER (slot 1 VIDEO_DRIVER__VIDEO_DRIVER, 2-5 pure virtual, 6
// VIDEO_DRIVER_CompleteDraw__FPv); D_00480270 SSX_VD_PS2_SINGLE_BUFFERD (1 SSX_VD_PS2_SINGLE_BUFFERD__SSX_VD_PS2_SINGLE_BUFFERD dtor, 2 SSX_VD_PS2_SINGLE_BUFFERD_SetPosition
// SetPosition, 3 SSX_VD_PS2_SINGLE_BUFFERD_SetScale__FPvff SetScale, 4 SSX_VD_PS2_SINGLE_BUFFERD_Draw Draw, 5 SSX_VD_PS2_SINGLE_BUFFERD_ReDraw__FPv ReDraw, 6 SSX_VD_PS2_SINGLE_BUFFERD_CompleteDraw
// CompleteDraw) - On Tour's VIDEO_DRIVER method order (SetPosition, SetScale, Draw, ReDraw, CompleteDraw).
//
// FLAT layouts. Evidence tags: [R] the constructor SSX_VD_PS2_SINGLE_BUFFERD_SSX_VD_PS2_SINGLE_BUFFERD, the destructors and SSX_VD_create's 84-byte
// allocation; [P] the preview's SSX_VD_PS2_SINGLE_BUFFERD methods; [O] On Tour's DWARF; [U] retail use.

#include "layout.h"

struct RCMP_DECODER;
struct RCMP_FRAME;
struct cPS2GraphicsMan;

// RCMP::VIDEO_DRIVER (0x8 [O]; [R] VIDEO_DRIVER__VIDEO_DRIVER stores D_004802B0 at +0x4).
struct RCMP_VIDEO_DRIVER {
    RCMP_DECODER* m_pdecoder;           // 0x0 [O]; [R] SSX_VD_PS2_SINGLE_BUFFERD_SSX_VD_PS2_SINGLE_BUFFERD stores the decoder
    void** mVtbl;                       // 0x4 [R] D_004802B0 (base) / D_00480270
};

// SSX_VD_PS2_SINGLE_BUFFERD (0x54 [R] SSX_VD_create allocates 84 bytes).
struct SSX_VD_PS2_SINGLE_BUFFERD {
    // --- RCMP::VIDEO_DRIVER
    RCMP_DECODER* m_pdecoder;           // 0x00 [O]; [R] ctor: its FramesDecoder argument; [U] the dtor and CompleteDraw
                                        //      return the held frame to it (RCMP_DECODER_ReleaseFrame); [P] same
    void** mVtbl;                       // 0x04 [R] ctor D_00480270; the dtor ends with D_004802B0
    // --- SSX_VD_PS2_SINGLE_BUFFERD
    cPS2GraphicsMan* m_pGrMan;          // 0x08 [O] (SSX_VD_PSP_ONE_TEXTURE); [R] ctor: its graphics-manager argument; [U]
                                        //      CompleteDraw polls its slot 18 (IsRenderIdle), the quad drawer (func_002548D0)
                                        //      uses it; [P] same
    RCMP_FRAME* mpCurrentFrame;         // 0x0C [R] ctor 0; [U] Draw stores the frame it is given; CompleteDraw and the dtor
                                        //      release it and clear it; [P] same
    char* mpImageHeader;                // 0x10 [R] ctor: the first frame's SHAPE (frame + 4); [U] Draw: each new frame's
                                        //      SHAPE (width / height at +4 / +6, pixel data after it)
    int** mppTiles;                     // 0x14 [R] ctor: "VD::VD_TEXTURE_POINTER" table of mNumTiles pointers to 12-byte
                                        //      tiles {texture handle, width 16, height} (graphics-manager slot 47); [U] Draw
                                        //      updates each tile's texture (SetNewTextureData1), the dtor frees them; [P]
    int mNumTiles;                      // 0x18 [R] ctor mNumColumns * mNumRows; [U] dtor loop bound; [P]
    int mNumColumns;                    // 0x1C [R] ctor width / 16; [U] Draw / func_002548D0 outer loop
    int mNumRows;                       // 0x20 [R] ctor: number of strips; [U] Draw / func_002548D0 inner loop
    int mRowHeights[6];                 // 0x24 [R] ctor: the power-of-two strip heights (func_00254350); [U] Draw /
                                        //      func_002548D0 step by them
    int mDrawLayer;                     // 0x3C [R] ctor: its DrawLayer argument; [U] func_002548D0
    float mPosX;                        // 0x40 [R] ctor centres the frame (SSX_VD_PS2_SINGLE_BUFFERD_SetPosition); [U] SetPosition; func_002548D0
    float mPosY;                        // 0x44 [R]; [U] SetPosition; func_002548D0
    float mPosZ;                        // 0x48 [R] ctor D_004A2EB0; [U] SetPosition; func_002548D0
    float mScaleX;                      // 0x4C [R] ctor 1.0f (SSX_VD_PS2_SINGLE_BUFFERD_SetScale); [U] SetScale; func_002548D0
    float mScaleY;                      // 0x50 [R] ctor 1.0f; [U] SetScale; func_002548D0
};

CHECK_OFFSET(RCMP_VIDEO_DRIVER, m_pdecoder, 0x0);
CHECK_OFFSET(RCMP_VIDEO_DRIVER, mVtbl, 0x4);
CHECK_SIZE(RCMP_VIDEO_DRIVER, 0x8);

CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, m_pdecoder, 0x00);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mVtbl, 0x04);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, m_pGrMan, 0x08);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mpCurrentFrame, 0x0C);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mpImageHeader, 0x10);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mppTiles, 0x14);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mNumTiles, 0x18);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mNumColumns, 0x1C);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mNumRows, 0x20);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mRowHeights, 0x24);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mDrawLayer, 0x3C);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mPosX, 0x40);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mPosY, 0x44);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mPosZ, 0x48);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mScaleX, 0x4C);
CHECK_OFFSET(SSX_VD_PS2_SINGLE_BUFFERD, mScaleY, 0x50);
CHECK_SIZE(SSX_VD_PS2_SINGLE_BUFFERD, 0x54);

#endif /* MOVIE_VIDEODRIVER_H */
