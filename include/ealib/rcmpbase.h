#ifndef EALIB_RCMPBASE_H
#define EALIB_RCMPBASE_H

// EA's RCMP video-decoder framework (namespace RCMP), the common classes: RCMP::RCMP_SYSTEM (the allocator the library
// calls), RCMP::CHUNK (one block of compressed input), RCMP::CODEC_IDATA (how a codec gets its input), RCMP::STREAMER
// (the opaque streamer handle), RCMP::CODEC (abstract codec base), RCMP::FRAME (a decoded picture) and RCMP::DECODER
// (owns the chosen codec and forwards to it). EA source: rcmp/dev/source/decoder/cmn/rcmpbase.cpp (SSX On Tour's DWARF
// names this file for RCMP_SYSTEM, CHUNK, CODEC_IDATA, DECODER and FRAME). Retail code: unit ealib/seg_2AEDC0
// (RCMP_RCMP_SYSTEM_RCMP_SYSTEM__FPv .. RCMP_CODEC_IDATA_CODEC_IDATA1, 0x3B0410..0x3B0758).
//
// Names: retail keeps the type_info names Q24RCMP11RCMP_SYSTEM, Q24RCMP7DECODER and Q24RCMP5CODEC and the linkonce
// sections GetCodecIData__Q24RCMP7DECODER, HasCodec__Q24RCMP7DECODER, _vt$Q24RCMP7DECODER etc.; On Tour's DWARF has
// every class below with the same member order (and its PSP strings name "RCMP::FRAME", "RCMP::RCMP_SYSTEM"). The
// namespace is flattened the way the project names these functions (RCMP_DECODER_GetFrame -> RCMP_DECODER).
//
// FLAT layouts, like main/game.h. g++ 2.95 puts a class's vptr after the data of the class that first declares
// virtuals: RCMP_SYSTEM +0xC, DECODER +0x18, CODEC +0x0 (no data). Evidence tags:
//   [R] retail constructors / destructors and allocation sizes;
//   [O] SSX On Tour's DWARF (member names and order);
//   [U] how retail code uses the field.

#include "layout.h"

struct RCMP_DECODER;
struct RCMP_CHUNK;
struct SHAPE;

// RCMP::RCMP_SYSTEM (0x10 [O]; [R] RCMP_RCMP_SYSTEM_RCMP_SYSTEM clears +0x0 / +0x4 and stores the vtable RCMP_RCMP_SYSTEM_vtbl at
// +0xC). The one instance is the global RCMP_rcmp_sys (constructed by the static initialiser __sti__rcmpbase_cpp at priority
// 0xFFFF; cMoviePlayer_SetSSXREALDefaults installs cMoviePlayer_SSXAllocMemFunc / SSXFreeMemFunc). Every RCMP and AV
// allocation goes through it: AllocMemFunc(tag, size, 0, 0, m_DefaultMemDir), FreeMemFunc(p).
struct RCMP_RCMP_SYSTEM {
    void* (*AllocMemFunc)(const char* tag, int size, int align, int d, int memDir);
                                        // 0x0 [O]; [R] ctor 0; [U] every RCMP allocation calls it (the movie player,
                                        //     DECODER, codec, frames, subtitle arrays); tested non-null before use
    void* (*FreeMemFunc)(void* p);      // 0x4 [O] (bool (*)(void*) there); [R] ctor 0; [U] RCMP deletes call it through
                                        //     D_00509434 (this field's address)
    int m_DefaultMemDir;                // 0x8 [O]; [U] passed as the last allocator argument (| 0x400 for the codec's
                                        //     uncached buffers)
    void** mVtbl;                       // 0xC [R] ctor RCMP_RCMP_SYSTEM_vtbl (_vt$Q24RCMP11RCMP_SYSTEM: slot 1 the destructor
                                        //     _$_Q24RCMP11RCMP_SYSTEM); __sti__rcmpbase_cpp restores it at exit (D_0050943C)
};

// RCMP::CHUNK (0xC [O]; [R] RCMP_CHUNK_CHUNK__FPv clears all three; func_003AEAD0 allocates 12 bytes and fills them).
struct RCMP_CHUNK {
    void* m_UserChunkData;              // 0x0 [O]; [R] ctor 0; [U] the movie player stores the STREAM block here and
                                        //     func_003AECE0 hands it back with STREAM_release
    void* m_DataToDecode;               // 0x4 [O]; [R] ctor 0; [U] func_003AEAD0: block + 8 (after the chunk header);
                                        //     the Sony codec copies from it (func_003B0B40, PS2_SONY_CODEC_INTERNAL_Init)
    unsigned int m_DataToDecodeSize;    // 0x8 [O]; [R] ctor 0; [U] func_003AEAD0: the little-endian size word of the
                                        //     chunk header; func_003B0B40 copies size - 8 bytes
};

// RCMP::STREAMER (0x4 [O]): an opaque handle the codec passes back to the get / release functions. The movie player
// (ealib/avplayer.h) embeds one at +0x60 pointing at itself.
struct RCMP_STREAMER {
    void* m_Streamer;                   // 0x0 [O]; [U] func_003AE450 stores the player; func_003AECB8 / func_003AED20
                                        //     read it back
};

// RCMP::CODEC_IDATA (0x10 [O]; [R] RCMP_CODEC_IDATA_CODEC_IDATA__FPv: 0, 0, 0, frame count 2;
// RCMP_CODEC_IDATA_CODEC_IDATA1: (Streamer, GetDataFunc, ReleaseDataFunc, NumberOfFramesToBuffer)).
struct RCMP_CODEC_IDATA {
    RCMP_STREAMER* m_Streamer;          // 0x0 [O]; [R] both ctors; [U] passed to both callbacks
    void (*m_GetDataFunc)(RCMP_DECODER* decoder, RCMP_STREAMER* streamer, RCMP_CHUNK** out);
                                        // 0x4 [O]; [R] both ctors; [U] RCMP_DECODER_GetFrame / GetChunk call it
                                        //     (func_003AECB8 for the movie player)
    void (*m_ReleaseDataFunc)(RCMP_DECODER* decoder, RCMP_STREAMER* streamer, RCMP_CHUNK* chunk);
                                        // 0x8 [O]; [R] both ctors; [U] RCMP_DECODER_ReleaseChunk calls it (func_003AED20)
    unsigned int m_MaxFramesOutstanding; // 0xC [O]; [R] default ctor 2, ctor1 its argument; [U] PS2_SONY_CODEC_INTERNAL_Init
                                        //     allocates that many frames (reads DECODER + 0x10)
};

// RCMP::CODEC (0x4 [O]: no data, only a vptr; abstract). Vtable _vt$Q24RCMP5CODEC = RCMP_CODEC_vtbl: slot 1 the destructor
// _$_Q24RCMP5CODEC (0x42E278), slots 2-6 pure virtual (func_00411530) = Init, GetFrame, GetCurrentFrameNumber,
// GetFrameRate, ReleaseFrame ([O] method order; the DECODER forwarders call slots 2 / 3 / 4 / 5 / 6 for them).
struct RCMP_CODEC {
    void** mVtbl;                       // 0x0 [R] derived destructors end with RCMP_CODEC_vtbl
};

// RCMP::FRAME, the base of a decoded picture (PS2: 0x8, no vptr; On Tour's 0xC adds one at +0x8). The PS2 Sony
// codec's frames (ealib/ps2sonycodec.h) add a list node at +0x8.
struct RCMP_FRAME {
    int m_FrameType;                    // 0x0 [O] FRAME_TYPE_ENUM (FRAME_MPC 0, FRAME_MAD 1, FRAME_PS2_SONY 2,
                                        //     FRAME_PSP_SONY 3); [R] func_003B07F8 stores 2
    SHAPE* m_Shp;                       // 0x4 [O]; [R] func_003B07F8 = SHAPE_create(w, h, 32 bpp, ...); [U] freed by
                                        //     func_003B08E0 (SHAPE_destroy); the codec decodes into its pixels;
                                        //     SSX_VD_PS2_SINGLE_BUFFERD_SSX_VD_PS2_SINGLE_BUFFERD (video driver) reads its header
};

// RCMP::DECODER (0x1C [R] func_003AE450 allocates 0x1C for it; [O] 0x1C). Vtable _vt$Q24RCMP7DECODER = RCMP_DECODER_vtbl
// (slot 1 RCMP_DECODER__DECODER).
struct RCMP_DECODER {
    RCMP_CHUNK* m_FirstChunk;           // 0x00 [O]; [R] ctor 0; [U] ChooseCodec stores it, GetFrame fills it through
                                        //     the get-data function, GetChunk hands it out once and clears it
    RCMP_CODEC_IDATA m_IData;           // 0x04 [O]; [R] ctor constructs then copies its argument (0x10 bytes); [U]
                                        //     GetFrame / GetChunk read +0x4 / +0x8, ReleaseChunk +0x4 / +0xC,
                                        //     PS2_SONY_CODEC_INTERNAL_Init reads + 0x10 (m_MaxFramesOutstanding)
    RCMP_CODEC* m_codec;                // 0x14 [O]; [R] ctor 0; [U] ChooseCodec stores it, FreeChosenCodec deletes it
                                        //     (slot 1, flags 3); GetFrame / GetCurrentFrameNumber / GetFrameRate /
                                        //     ReleaseFrame call its slots 3 / 4 / 5 / 6; func_003AEAD0 tests it
    void** mVtbl;                       // 0x18 [R] ctor and dtor RCMP_DECODER_vtbl; func_003AE300 deletes the decoder through
                                        //     slot 1

    // Methods bound to their retail symbols by asm labels.
    void FreeChosenCodec() __asm__("RCMP_DECODER_FreeChosenCodec");
    int GetCurrentFrameNumber() __asm__("RCMP_DECODER_GetCurrentFrameNumber");
    float GetFrameRate() __asm__("RCMP_DECODER_GetFrameRate");
    void ReleaseFrame(int frame) __asm__("RCMP_DECODER_ReleaseFrame");
    int GetChunk() __asm__("RCMP_DECODER_GetChunk");
};

CHECK_OFFSET(RCMP_RCMP_SYSTEM, AllocMemFunc, 0x0);
CHECK_OFFSET(RCMP_RCMP_SYSTEM, FreeMemFunc, 0x4);
CHECK_OFFSET(RCMP_RCMP_SYSTEM, m_DefaultMemDir, 0x8);
CHECK_OFFSET(RCMP_RCMP_SYSTEM, mVtbl, 0xC);
CHECK_SIZE(RCMP_RCMP_SYSTEM, 0x10);

CHECK_OFFSET(RCMP_CHUNK, m_UserChunkData, 0x0);
CHECK_OFFSET(RCMP_CHUNK, m_DataToDecode, 0x4);
CHECK_OFFSET(RCMP_CHUNK, m_DataToDecodeSize, 0x8);
CHECK_SIZE(RCMP_CHUNK, 0xC);

CHECK_OFFSET(RCMP_STREAMER, m_Streamer, 0x0);
CHECK_SIZE(RCMP_STREAMER, 0x4);

CHECK_OFFSET(RCMP_CODEC_IDATA, m_Streamer, 0x0);
CHECK_OFFSET(RCMP_CODEC_IDATA, m_GetDataFunc, 0x4);
CHECK_OFFSET(RCMP_CODEC_IDATA, m_ReleaseDataFunc, 0x8);
CHECK_OFFSET(RCMP_CODEC_IDATA, m_MaxFramesOutstanding, 0xC);
CHECK_SIZE(RCMP_CODEC_IDATA, 0x10);

CHECK_OFFSET(RCMP_CODEC, mVtbl, 0x0);
CHECK_SIZE(RCMP_CODEC, 0x4);

CHECK_OFFSET(RCMP_FRAME, m_FrameType, 0x0);
CHECK_OFFSET(RCMP_FRAME, m_Shp, 0x4);
CHECK_SIZE(RCMP_FRAME, 0x8);

CHECK_OFFSET(RCMP_DECODER, m_FirstChunk, 0x00);
CHECK_OFFSET(RCMP_DECODER, m_IData, 0x04);
CHECK_OFFSET(RCMP_DECODER, m_codec, 0x14);
CHECK_OFFSET(RCMP_DECODER, mVtbl, 0x18);
CHECK_SIZE(RCMP_DECODER, 0x1C);

#endif /* EALIB_RCMPBASE_H */
