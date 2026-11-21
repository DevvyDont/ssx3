#ifndef EALIB_PS2SONYCODEC_H
#define EALIB_PS2SONYCODEC_H

// PS2_SONY_CODEC_INTERNAL: RCMP's PS2 MPEG codec (an RCMP::CODEC over Sony's libmpeg: sceMpegInit / sceMpegCreate /
// sceMpegGetPicture, with an mpeg2decode header parser), and its decoded frames. Created by func_003B11A0 (0x80 bytes
// from the RCMP allocator, empty tag) and offered to the DECODER by the movie player's get-data function (func_003AEAD0
// -> RCMP_DECODER_ChooseCodec). Retail code: unit ealib/seg_2AEDC0, func_003B07F8 .. func_003B11A0 (0x3B07F8..0x3B11E8).
// EA source: not named for PS2 (On Tour's PSP twin is rcmp/dev/source/decoder/psp/rcmp_psp_sony_codec.cpp:
// PspSonyCodec / PspSonyFrame).
//
// Names: PS2_SONY_CODEC_INTERNAL is EA's (retail type_info string "23PS2_SONY_CODEC_INTERNAL", linkonce
// CreateFrame__23PS2_SONY_CODEC_INTERNALUiUi, _vt$23PS2_SONY_CODEC_INTERNAL; global namespace). Its member names follow
// On Tour's PspSonyCodec where the role matches ([O]); the PS2-only ones are named from use ([U]).
// RCON_DLNode / RCON_DLList: EA's RCON intrusive list (On Tour: RCON::DLNode {Next, Prev}, RCON::DLList {HeadNode},
// typeinfo "RCON::TDLNode<RCMP::PspSonyFrame>"); retail code inlines the same operations (DLNode ctor marks the links
// with 3, ~DLNode with 7, Remove with 0xB; an empty list's head points at itself).
// The decoded frames (0x14 bytes each, allocated by PS2_SONY_CODEC_INTERNAL_Init) are the PS2 counterpart of On Tour's
// PspSonyFrame (an RCMP::FRAME, an RCON::DLNode at +0x8, mReferenceCount at +0x10); retail gives the class no name, so
// it has no struct here (func_003B07F8 / func_003B08E0 are its ctor / dtor).
//
// Vtable PS2_SONY_CODEC_INTERNAL_vtbl (_vt$23PS2_SONY_CODEC_INTERNAL): slot 1 PS2_SONY_CODEC_INTERNAL__PS2_SONY_CODEC_INTERNAL, 2 _Init,
// 3 _GetFrame, 4 _GetCurrentFrameNumber__FPv, 5 _GetFrameRate, 6 _ReleaseFrame (RCMP::CODEC's virtual order).
//
// FLAT layouts. Evidence tags: [R] constructors PS2_SONY_CODEC_INTERNAL_PS2_SONY_CODEC_INTERNAL / func_003B07F8 and the
// allocation sizes (0x80 in func_003B11A0, 20 in PS2_SONY_CODEC_INTERNAL_Init); [O] On Tour's DWARF; [U] retail use.

#include "layout.h"
#include "ealib/rcmpbase.h"

// RCON::DLNode (0x8 [O]).
struct RCON_DLNode {
    RCON_DLNode* Next;                  // 0x0 [O]
    RCON_DLNode* Prev;                  // 0x4 [O]
};

// RCON::DLList (0x8 [O]): the head node of a circular list.
struct RCON_DLList {
    RCON_DLNode HeadNode;               // 0x0 [O]; empty when HeadNode.Next == &HeadNode
};

// PS2_SONY_CODEC_INTERNAL. 0x80 [R] func_003B11A0 allocates 0x80.
struct PS2_SONY_CODEC_INTERNAL {
    // --- RCMP::CODEC
    void** mVtbl;                       // 0x00 [R] ctor PS2_SONY_CODEC_INTERNAL_vtbl; the dtor ends with RCMP::CODEC's RCMP_CODEC_vtbl
    // --- PS2_SONY_CODEC_INTERNAL
    int unk_04;                         // 0x04 [U] Init sets 1; no other access seen
    int mWidth;                         // 0x08 [O] (PspSonyCodec); [U] Init: macroblock columns * 16; DecodeFrame /
                                        //      frame sizes use it
    int mHeight;                        // 0x0C [O] (PspSonyCodec); [U] Init: coded height
    unsigned int mCurrentFrameNumber;   // 0x10 [O] (PspSonyCodec); [U] Init 0, DecodeFrame ++, GetFrame compares,
                                        //      GetCurrentFrameNumber__FPv returns it
    int unk_14;                         // 0x14 [U] Init 0; func_003B0B40 counts the chunks fed to the decoder
    RCON_DLList mUsedFrames;            // 0x18 [O]; [R] ctor: empty list; [U] func_003B10D0 adds, ReleaseFrame removes;
                                        //      the dtor destroys what is left (func_003B08E0)
    RCON_DLList mFreeFrames;            // 0x20 [O]; [R] ctor: empty list; [U] Init adds the new frames, func_003B10D0
                                        //      takes from it, ReleaseFrame returns frames to it
    RCMP_DECODER* mpDecoder;            // 0x28 [U] Init stores its DECODER argument; func_003B0B40 gets / releases chunks
                                        //      through it (RCMP_DECODER_GetChunk / ReleaseChunk)
    void* mpWorkBuffer;                 // 0x2C [R] ctor 0; [U] Init frees the old one and allocates "RCMP::mpegbuff"
                                        //      (w * h * 9 / 2 + 5996, mem dir | 0x400) for sceMpegCreate; dtor frees it
    char mMpeg[0x48];                   // 0x30 [U] Sony's sceMpeg (passed to sceMpegCreate / sceMpegAddCallback /
                                        //      sceMpegGetPicture / sceMpegIsEnd; 0x48 = its PS2 size up to mpChunkBuffer)
    void* mpChunkBuffer;                // 0x78 [R] ctor 0; [U] Init allocates "RCMP::chunkbuff" (0x32000, | 0x400), dtor
                                        //      frees it
    char* mpChunkBufferUncached;        // 0x7C [U] Init: mpChunkBuffer | 0x30000000 (uncached alias); func_003B0B40 copies
                                        //      each chunk there and hands it to sceMpegAddBs
};

CHECK_OFFSET(RCON_DLNode, Next, 0x0);
CHECK_OFFSET(RCON_DLNode, Prev, 0x4);
CHECK_SIZE(RCON_DLNode, 0x8);
CHECK_OFFSET(RCON_DLList, HeadNode, 0x0);
CHECK_SIZE(RCON_DLList, 0x8);


CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mVtbl, 0x00);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, unk_04, 0x04);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mWidth, 0x08);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mHeight, 0x0C);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mCurrentFrameNumber, 0x10);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, unk_14, 0x14);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mUsedFrames, 0x18);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mFreeFrames, 0x20);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mpDecoder, 0x28);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mpWorkBuffer, 0x2C);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mMpeg, 0x30);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mpChunkBuffer, 0x78);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mpChunkBufferUncached, 0x7C);
CHECK_SIZE(PS2_SONY_CODEC_INTERNAL, 0x80);

#endif /* EALIB_PS2SONYCODEC_H */
