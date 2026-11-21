#ifndef EALIB_MTDECF_H
#define EALIB_MTDECF_H

// CMTBLKDecf: EA SND's MicroTalk block decoder (float output), used by the MicroTalk unpack filters
// (ealib/sfilter.h: UNPACKMTPFSTATE.pDecoder; SFILTER_unpackmtfinit / mtlfinit / mtpfinit create one with
// func_003CD1A8(3432) + CMTBLKDecf_CMTBLKDecf). Retail code: unit ealib/seg_2B4578, CMTBLKDecf_initmut ..
// CMTBLKDecf_SetCodecVersion. EA source: snd/source/library/extern/coda/cmn/mtdecf.cpp (On Tour).
//
// Name: EA's class CMTBLKDecf (On Tour's DWARF: class CMTBLKDecf, 0xD68, the same members; methods GetState returning
// MTFSTATE, SetState, Feed, Decode, SetCodecVersion), matching the project's function names and retail's 3432-byte
// allocation. No vtable (size = end of the last member).
//
// FLAT layout. Evidence tags: [R] the constructor CMTBLKDecf_CMTBLKDecf and the 3432-byte allocation; [O] On Tour's
// DWARF; [U] retail use.

#include "layout.h"

struct CMTBLKDecf {
    char mutstate[0xD44];               // 0x000 [O] UTALKSTATE_CODA (the MicroTalk codec state; not mapped); [U] +0x4 is the
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
