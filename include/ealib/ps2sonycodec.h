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
// PS2_SONY_FRAME below is ours (func_003B07F8 / func_003B08E0 are its ctor / dtor).
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

#ifdef __cplusplus
    // [O] DLNode::Remove(): unlinks the node and marks its links 0xB (retail inlines it).
    void Remove()
    {
        RCON_DLNode* n = Next;
        RCON_DLNode* p = Prev;
        p->Next = n;
        n->Prev = p;
        Prev = Next = (RCON_DLNode*)0xB;
    }
#endif
};

// RCON::DLList (0x8 [O]): the head node of a circular list.
struct RCON_DLList {
    RCON_DLNode HeadNode;               // 0x0 [O]; empty when HeadNode.Next == &HeadNode

#ifdef __cplusplus
    // [O] DLList::AddHead(DLNode*) / AddTail(DLNode*) (retail inlines them).
    void AddHead(RCON_DLNode* node)
    {
        RCON_DLNode* old = HeadNode.Next;
        HeadNode.Next = node;
        old->Prev = node;
        node->Next = old;
        node->Prev = &HeadNode;
    }
    void AddTail(RCON_DLNode* node)
    {
        RCON_DLNode* tail = HeadNode.Prev;
        tail->Next = node;
        HeadNode.Prev = node;
        node->Prev = tail;
        node->Next = &HeadNode;
    }
#endif
};

// The Sony codec's decoded frame (0x14 [R] PS2_SONY_CODEC_INTERNAL_Init allocates 20 bytes; name ours). [O] On Tour's
// PspSonyFrame : FRAME, TDLNode has the same bases and mReferenceCount after them (its FRAME has a vptr, so its
// members sit 4 bytes higher). [R] ctor func_003B07F8 (type 2, links 3, count 0), dtor func_003B08E0 (links 7).
#ifdef __cplusplus
struct PS2_SONY_FRAME : RCMP_FRAME, RCON_DLNode {
    int mReferenceCount;                // 0x10 [O]; [R] ctor 0; [U] func_003B10D0 sets 1, ReleaseFrame counts down
};
#endif

// PS2_SONY_CODEC_INTERNAL. 0x80 [R] func_003B11A0 allocates 0x80.
struct RCMP_CHUNK;
struct RCMP_DECODER;

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
    int mChunksFed;                     // 0x14 [U] Init 0; func_003B0B40 counts the chunks fed to the decoder
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

    // Methods bound to their retail symbols by asm labels.
    void* DecodeFrame() __asm__("PS2_SONY_CODEC_INTERNAL_DecodeFrame");
    void* GetFrame(unsigned int limit) __asm__("PS2_SONY_CODEC_INTERNAL_GetFrame");
    int Init(RCMP_DECODER* script, RCMP_CHUNK* firstChunk) __asm__("PS2_SONY_CODEC_INTERNAL_Init");
    void ReleaseFrame(PS2_SONY_FRAME* item) __asm__("PS2_SONY_CODEC_INTERNAL_ReleaseFrame");
};

// The MPEG-2 video constants the unit's mpeg2decode header parser tests: start codes, extension ids, scalable modes,
// picture coding types and picture structures. Names as in the MSSG reference decoder's mpeg2dec.h (not in refs/);
// values per ISO/IEC 13818-2 (start codes 00 00 01 xx, extension_start_code_identifier, picture_coding_type,
// picture_structure; scalable_mode is the spec's value + 1, "to make SC_DP != SC_NONE"). [U] Get_Hdr dispatches on the
// start codes, extension_and_user_data on the extension ids, picture_header reads the vectors of P and B pictures.
#define START_CODE_PREFIX                      0x000001 /* name ours: the 24 bits before a start code's last byte */
#define PICTURE_START_CODE                     0x100
#define USER_DATA_START_CODE                   0x1B2
#define SEQUENCE_HEADER_CODE                   0x1B3
#define EXTENSION_START_CODE                   0x1B5
#define SEQUENCE_END_CODE                      0x1B7
#define GROUP_START_CODE                       0x1B8

#define SEQUENCE_EXTENSION_ID                  1
#define SEQUENCE_DISPLAY_EXTENSION_ID          2
#define QUANT_MATRIX_EXTENSION_ID              3
#define COPYRIGHT_EXTENSION_ID                 4
#define SEQUENCE_SCALABLE_EXTENSION_ID         5
#define PICTURE_DISPLAY_EXTENSION_ID           7
#define PICTURE_CODING_EXTENSION_ID            8
#define PICTURE_SPATIAL_SCALABLE_EXTENSION_ID  9
#define PICTURE_TEMPORAL_SCALABLE_EXTENSION_ID 10

#define SC_NONE 0
#define SC_DP   1
#define SC_SPAT 2
#define SC_SNR  3
#define SC_TEMP 4

#define I_TYPE 1
#define P_TYPE 2
#define B_TYPE 3
#define D_TYPE 4

#define TOP_FIELD     1
#define BOTTOM_FIELD  2
#define FRAME_PICTURE 3

// layer_data: mpeg2decode's per-layer state (the MSSG reference decoder's struct layer_data, EA's memory-buffer
// variant: no file, read buffer or end pointer before the bit reader). The unit's copy of the mpeg2decode header parser
// reads through the global ld (D_0050A088), which PS2_SONY_CODEC_INTERNAL_Init points at the base layer D_00509648.
// Member names are mpeg2decode's (global.h), in its order; tags: [U] retail use. Size not known (block[] etc. unused).
struct layer_data {
    unsigned char* Rdptr;                       // 0x000 [U] Initialize_Buffer sets it to the data; Flush_Buffer /
                                                //       Flush_Buffer32 read a byte at a time from it
    unsigned int Bfr;                           // 0x004 [U] the 32-bit bit buffer (next bit in the top bit): Show_Bits
                                                //       reads it, Flush_Buffer shifts and refills it
    int Incnt;                                  // 0x008 [U] the valid bits in Bfr: Flush_Buffer counts it down / up,
                                                //       next_start_code aligns with Incnt & 7
    int Bitcnt;                                 // 0x00C [U] sequence_header reads it
    int intra_quantizer_matrix[64];             // 0x010 [U] sequence_header
    int non_intra_quantizer_matrix[64];         // 0x110 [U] sequence_header
    int chroma_intra_quantizer_matrix[64];      // 0x210 [U] sequence_header copies the intra matrix
    int chroma_non_intra_quantizer_matrix[64];  // 0x310 [U] sequence_header copies the non-intra matrix
    int load_intra_quantizer_matrix;            // 0x410 [U] sequence_header, quant_matrix_extension (mpeg2decode:
                                                //       int)
    int load_non_intra_quantizer_matrix;        // 0x414 [U] sequence_header, quant_matrix_extension
    int load_chroma_intra_quantizer_matrix;     // 0x418 [U] sequence_header clears it; quant_matrix_extension
    int load_chroma_non_intra_quantizer_matrix; // 0x41C [U] sequence_header clears it; quant_matrix_extension
    int MPEG2_Flag;                             // 0x420 [U] slice_header tests it; PS2_SONY_CODEC_INTERNAL_Init
                                                //       reads it
    int scalable_mode;                          // 0x424 [U] slice_header: == 1 (data partitioning) reads 7 bits
    int q_scale_type;                           // 0x428 [U] the picture coding extension sets it; slice_header maps
                                                //       the quantiser scale code with it
    int alternate_scan;                         // 0x42C [U] the picture coding extension stores 1 bit
    int pict_scal;                              // 0x430 [U] picture_header clears it
    int priority_breakpoint;                    // 0x434 [U] slice_header (data partitioning)
    int quantizer_scale;                        // 0x438 [U] slice_header
    int intra_slice;                            // 0x43C [U] slice_header
};

CHECK_OFFSET(layer_data, Rdptr, 0x000);
CHECK_OFFSET(layer_data, Bfr, 0x004);
CHECK_OFFSET(layer_data, Incnt, 0x008);
CHECK_OFFSET(layer_data, Bitcnt, 0x00C);
CHECK_OFFSET(layer_data, intra_quantizer_matrix, 0x010);
CHECK_OFFSET(layer_data, non_intra_quantizer_matrix, 0x110);
CHECK_OFFSET(layer_data, chroma_intra_quantizer_matrix, 0x210);
CHECK_OFFSET(layer_data, chroma_non_intra_quantizer_matrix, 0x310);
CHECK_OFFSET(layer_data, load_intra_quantizer_matrix, 0x410);
CHECK_OFFSET(layer_data, MPEG2_Flag, 0x420);
CHECK_OFFSET(layer_data, scalable_mode, 0x424);
CHECK_OFFSET(layer_data, q_scale_type, 0x428);
CHECK_OFFSET(layer_data, alternate_scan, 0x42C);
CHECK_OFFSET(layer_data, pict_scal, 0x430);
CHECK_OFFSET(layer_data, priority_breakpoint, 0x434);
CHECK_OFFSET(layer_data, quantizer_scale, 0x438);
CHECK_OFFSET(layer_data, intra_slice, 0x43C);

#ifdef __cplusplus
CHECK_OFFSET(PS2_SONY_FRAME, mReferenceCount, 0x10);
CHECK_SIZE(PS2_SONY_FRAME, 0x14);
#endif

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
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mChunksFed, 0x14);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mUsedFrames, 0x18);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mFreeFrames, 0x20);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mpDecoder, 0x28);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mpWorkBuffer, 0x2C);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mMpeg, 0x30);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mpChunkBuffer, 0x78);
CHECK_OFFSET(PS2_SONY_CODEC_INTERNAL, mpChunkBufferUncached, 0x7C);
CHECK_SIZE(PS2_SONY_CODEC_INTERNAL, 0x80);

#endif /* EALIB_PS2SONYCODEC_H */
