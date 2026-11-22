#ifndef EALIB_MTDECF_H
#define EALIB_MTDECF_H

// CMTBLKDecf: EA SND's MicroTalk block decoder (float output), used by the MicroTalk unpack filters
// (ealib/sfilter.h: UNPACKMTPFSTATE.pDecoder; SFILTER_unpackmtfinit / mtlfinit / mtpfinit create one with
// CMTBLKDecf_operator_new(3432) + CMTBLKDecf_CMTBLKDecf). Retail code: unit ealib/seg_2B4578, CMTBLKDecf_initmut ..
// CMTBLKDecf_SetCodecVersion. EA source: snd/source/library/extern/coda/cmn/mtdecf.cpp (On Tour).
//
// Name: EA's class CMTBLKDecf (On Tour's DWARF: class CMTBLKDecf, 0xD68, the same members; methods GetState returning
// MTFSTATE, SetState, Feed, Decode, SetCodecVersion), matching the project's function names and retail's 3432-byte
// allocation. No vtable (size = end of the last member).
//
// FLAT layout. Evidence tags: [R] the constructor CMTBLKDecf_CMTBLKDecf and the 3432-byte allocation; [O] On Tour's
// DWARF; [U] retail use.

#include "layout.h"

// C units (ealib/seg_2B4578.c, ealib/seg_2DCAE8.c) name these types without the struct keyword (views group D).
#ifndef __cplusplus
typedef struct UTALKSTATE_CODA UTALKSTATE_CODA;
typedef struct MTFSTATE MTFSTATE;
typedef struct CMTBLKDecf CMTBLKDecf;
#endif

// UTALKSTATE_CODA: the MicroTalk codec state (views group D: B_D260). [O] On Tour struct UTALKSTATE_CODA (0xD44,
// snd/source/library/extern/coda mtdecf.cpp), discardbits(UTALKSTATE_CODA* state, int count) and the decoder's other
// static helpers take it; [R] CMTBLKDecf::mutstate is exactly 0xD44 bytes and GetState / SetState read and write
// shiftreg (+0x4); [U] discardbits / the bit reader in ealib/seg_2B4578 use dataptr 0x0, shiftreg 0x4, bitcount 0x8.
struct UTALKSTATE_CODA {
    unsigned char* dataptr;         // 0x000 [O]; [U] the bit reader's source pointer
    unsigned int shiftreg;          // 0x004 [O]; [U] the bit reader's shift register (GetState / SetState)
    int bitcount;                   // 0x008 [O]; [U] bits left in shiftreg
    int mode;                       // 0x00C [O]
    int voicing_threshold;          // 0x010 [O]
    float stepval[64];              // 0x014 [O]
    float coeff[12];                // 0x114 [O]
    float output[12];               // 0x144 [O]
    float history[324];             // 0x174 [O]
    float data[432];                // 0x684 [O]
};

CHECK_OFFSET(UTALKSTATE_CODA, dataptr, 0x000);
CHECK_OFFSET(UTALKSTATE_CODA, shiftreg, 0x004);
CHECK_OFFSET(UTALKSTATE_CODA, bitcount, 0x008);
CHECK_OFFSET(UTALKSTATE_CODA, mode, 0x00C);
CHECK_OFFSET(UTALKSTATE_CODA, voicing_threshold, 0x010);
CHECK_OFFSET(UTALKSTATE_CODA, stepval, 0x014);
CHECK_OFFSET(UTALKSTATE_CODA, coeff, 0x114);
CHECK_OFFSET(UTALKSTATE_CODA, output, 0x144);
CHECK_OFFSET(UTALKSTATE_CODA, history, 0x174);
CHECK_OFFSET(UTALKSTATE_CODA, data, 0x684);
CHECK_SIZE(UTALKSTATE_CODA, 0xD44);

// MTFSTATE: the saved decoder state (views group D: V3_E078, Q_9520, Q_96F0). [O] On Tour struct MTFSTATE {unsigned int
// shiftreg; unsigned int bufframes; int doWholeInit;} (0xC), returned by CMTBLKDecf::GetState() and taken by
// SetState(MTFSTATE); [R] CMTBLKDecf_GetState returns mutstate.shiftreg, mbufframes, mdoWholeInit in that order; the
// MicroTalk unpack filters (SFILTER_unpackmtlf / mtpf) save and restore it around a loop.
struct MTFSTATE {
    unsigned int shiftreg;          // 0x0 [O] = CMTBLKDecf::mutstate.shiftreg
    unsigned int bufframes;         // 0x4 [O] = CMTBLKDecf::mbufframes
    int doWholeInit;                // 0x8 [O] = CMTBLKDecf::mdoWholeInit
};

CHECK_OFFSET(MTFSTATE, shiftreg, 0x0);
CHECK_OFFSET(MTFSTATE, bufframes, 0x4);
CHECK_OFFSET(MTFSTATE, doWholeInit, 0x8);
CHECK_SIZE(MTFSTATE, 0xC);

struct CMTBLKDecf {
    UTALKSTATE_CODA mutstate;           // 0x000 [O] UTALKSTATE_CODA (the MicroTalk codec state; views group D); [U] +0x4 is the
                                        //       bit-reader shift register (GetState / SetState); Decode writes +0x0..+0x8
    unsigned int mbufframes;            // 0xD44 [O]; [R] ctor; [U] GetState returns it (frames left in the decoded
                                        //       frame); Feed / Decode
    unsigned char* mpSrc;               // 0xD48 [O]; [U] Feed stores the input, Decode advances it
    float* mpDst;                       // 0xD4C [O]; [U] Decode
    short mRawframes;                   // 0xD50 [O]; [R] ctor (sh); [U] Decode (lh; also an sb there, see the report)
    short mOffset;                      // 0xD52 [O]; [R] ctor (sh); [U] Decode (lh; also an sb)
    int mRemainingSamples;              // 0xD54 [O]; [R] ctor; [U] Feed / Decode
    int mSampleDataSize;                // 0xD58 [O]; [R] ctor; [U] Feed
    int mdoWholeInit;                   // 0xD5C [O]; [R] ctor; [U] GetState / SetState; Feed re-reads the codec header
                                        //       when set
    int mHasPCMData;                    // 0xD60 [O]; [R] ctor; [U] Feed / Decode
    int mCodecVersion;                  // 0xD64 [O]; [R] ctor; [U] SetCodecVersion; Feed / Decode
};

CHECK_OFFSET(CMTBLKDecf, mutstate, 0x000);
CHECK_OFFSET(CMTBLKDecf, mbufframes, 0xD44);
CHECK_OFFSET(CMTBLKDecf, mpSrc, 0xD48);
CHECK_OFFSET(CMTBLKDecf, mpDst, 0xD4C);
CHECK_OFFSET(CMTBLKDecf, mRawframes, 0xD50);
CHECK_OFFSET(CMTBLKDecf, mOffset, 0xD52);
CHECK_OFFSET(CMTBLKDecf, mRemainingSamples, 0xD54);
CHECK_OFFSET(CMTBLKDecf, mSampleDataSize, 0xD58);
CHECK_OFFSET(CMTBLKDecf, mdoWholeInit, 0xD5C);
CHECK_OFFSET(CMTBLKDecf, mHasPCMData, 0xD60);
CHECK_OFFSET(CMTBLKDecf, mCodecVersion, 0xD64);
CHECK_SIZE(CMTBLKDecf, 0xD68);

#endif /* EALIB_MTDECF_H */
