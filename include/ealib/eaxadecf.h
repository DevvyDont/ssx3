#ifndef EALIB_EAXADECF_H
#define EALIB_EAXADECF_H

// EA SND's EA-XA block decoder (float output): the records its methods pass around (views group D).
// EA source: snd/source/library/extern/coda/cmn/eaxadecf.cpp (On Tour). Retail code: unit ealib/seg_2B4578
// (CEAXABLKDecf_* and the static process_raw_block).
// The decoder class CEAXABLKDecf (0xA8) is laid out below with its EAXAVARSF block state; its predictor history is
// xap.sample1 / sample2 (+0x98 / +0x9C, [R] CEAXABLKDecf_GetState).
// Tags: [O] SSX On Tour's DWARF (struct names, members, the paired functions); [R] retail code; [U] retail use.

#include "layout.h"

// C units (ealib/seg_2B4578.c, ealib/seg_2DCAE8.c) name these types without the struct keyword (views group D).
#ifndef __cplusplus
typedef struct XAFSTATE XAFSTATE;
typedef struct MXAPACKETF MXAPACKETF;
typedef struct EAXAVARSF EAXAVARSF;
typedef struct CEAXABLKDecf CEAXABLKDecf;
#endif

// XAFSTATE: the decoder's saved predictor history. [O] On Tour struct XAFSTATE {float sample1; float sample2;} (0x8),
// returned by CEAXABLKDecf::GetState() and taken by SetState; [R] CEAXABLKDecf_GetState returns the two history samples
// (+0x98, +0x9C) as this pair (view V2_D138).
struct XAFSTATE {
    float sample1;                  // 0x0 [O]
    float sample2;                  // 0x4 [O]
};

CHECK_OFFSET(XAFSTATE, sample1, 0x0);
CHECK_OFFSET(XAFSTATE, sample2, 0x4);
CHECK_SIZE(XAFSTATE, 0x8);

// MXAPACKETF: one EA-XA decode job. [O] On Tour struct MXAPACKETF {int numframes; float sample1; float sample2;
// unsigned char* psrc; float* pdst;} (0x14) and process_raw_block(MXAPACKETF* pxap); [U] the retail
// process_raw_block (view S_C938) reads the two history samples at +0x4 / +0x8 and the source at +0xC and writes
// floats through +0x10.
struct MXAPACKETF {
    int numframes;                  // 0x00 [O]
    float sample1;                  // 0x04 [O]; [U] the predictor history
    float sample2;                  // 0x08 [O]; [U]
    unsigned char* psrc;            // 0x0C [O]; [U] the compressed input
    float* pdst;                    // 0x10 [O]; [U] the decoded output
};

CHECK_OFFSET(MXAPACKETF, numframes, 0x00);
CHECK_OFFSET(MXAPACKETF, sample1, 0x04);
CHECK_OFFSET(MXAPACKETF, sample2, 0x08);
CHECK_OFFSET(MXAPACKETF, psrc, 0x0C);
CHECK_OFFSET(MXAPACKETF, pdst, 0x10);
CHECK_SIZE(MXAPACKETF, 0x14);

// EAXAVARSF: the decoder's block state - the samples still owed from the last decoded block and the block buffer.
// [O] On Tour struct EAXAVARSF (0x90: residual, numsamples, sampledatasize, presidue, xablockprev[2], xablock[30]),
// CEAXABLKDecf::xav; [R] the CEAXABLKDecf constructor clears residual / numsamples / sampledatasize (+0x4..+0xC of the
// decoder); [U] CEAXABLKDecf_Feed and CEAXABLKDecf_Decode (view S_CF90) use the members below.
struct EAXAVARSF {
    int residual;                   // 0x00 [O]; [U] Decode: decoded samples still to hand out (from presidue); set to
                                    //      -n after a partial block, 0 at the end
    int numsamples;                 // 0x04 [O]; [U] Feed stores the sample count (Feed refuses new data while it is
                                    //      non-zero); Decode counts it down
    int sampledatasize;             // 0x08 [O]; [U] Feed stores the byte size
    float* presidue;                // 0x0C [O]; [U] Decode reads the residual samples through it and advances it
    float xablockprev[2];           // 0x10 [O] (the two samples before xablock; layout pinned by presidue and xablock)
    float xablock[30];              // 0x18 [O]; [U] Decode decodes a partial block into xablock + 2 and copies the
                                    //      samples out
};

CHECK_OFFSET(EAXAVARSF, residual, 0x00);
CHECK_OFFSET(EAXAVARSF, numsamples, 0x04);
CHECK_OFFSET(EAXAVARSF, sampledatasize, 0x08);
CHECK_OFFSET(EAXAVARSF, presidue, 0x0C);
CHECK_OFFSET(EAXAVARSF, xablockprev, 0x10);
CHECK_OFFSET(EAXAVARSF, xablock, 0x18);
CHECK_SIZE(EAXAVARSF, 0x90);

// CEAXABLKDecf: EA SND's EA-XA block decoder (float output). [O] On Tour class CEAXABLKDecf (0xA8: decodexa, EAXAVARSF
// xav, MXAPACKETF xap; no vtable) and CEAXABLKDecf::CEAXABLKDecf(CEAXABLKDecf* const this); [R] every EA-XA unpack init
// (SFILTER_unpackxafinit / unpackxalfinit / unpackxapfinit) allocates 168 = 0xA8 bytes for it and runs the
// constructor, which clears xav.residual / numsamples / sampledatasize and xap.sample1 / sample2.
// Methods (unit ealib/seg_2B4578): CEAXABLKDecf_CEAXABLKDecf, _Feed, _Decode, _GetState, _SetState.
struct CEAXABLKDecf {
    void* decodexa;                 // 0x00 [O] (a decode-function pointer: `void (*decodexa)()`); no retail access seen
    struct EAXAVARSF xav;           // 0x04 [O]; [R] ctor; [U] Feed / Decode
    struct MXAPACKETF xap;          // 0x94 [O]; [U] Decode sets numframes (0x94) and pdst (0xA4) and runs
                                    //      process_raw_block on it; Feed stores the input in psrc (0xA0); GetState /
                                    //      SetState read / write sample1 / sample2 (0x98 / 0x9C)
};

CHECK_OFFSET(CEAXABLKDecf, decodexa, 0x00);
CHECK_OFFSET(CEAXABLKDecf, xav, 0x04);
CHECK_OFFSET(CEAXABLKDecf, xap, 0x94);
CHECK_SIZE(CEAXABLKDecf, 0xA8);

#endif /* EALIB_EAXADECF_H */
