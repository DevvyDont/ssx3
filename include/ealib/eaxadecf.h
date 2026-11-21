#ifndef EALIB_EAXADECF_H
#define EALIB_EAXADECF_H

// EA SND's EA-XA block decoder (float output): the records its methods pass around (views group D).
// EA source: snd/source/library/extern/coda/cmn/eaxadecf.cpp (On Tour). Retail code: unit ealib/seg_2B4578
// (CEAXABLKDecf_* and the static process_raw_block).
// The decoder class CEAXABLKDecf itself is not laid out here; its predictor history is at +0x98 / +0x9C
// ([R] CEAXABLKDecf_GetState).
// Tags: [O] SSX On Tour's DWARF (struct names, members, the paired functions); [R] retail code; [U] retail use.

#include "layout.h"

// C units (ealib/seg_2B4578.c, ealib/seg_2DCAE8.c) name these types without the struct keyword (views group D).
#ifndef __cplusplus
typedef struct XAFSTATE XAFSTATE;
typedef struct MXAPACKETF MXAPACKETF;
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

#endif /* EALIB_EAXADECF_H */
