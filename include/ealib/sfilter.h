#ifndef EALIB_SFILTER_H
#define EALIB_SFILTER_H

// EA SND's software mixer filter graph (C): every filter is a node whose state struct starts with an SFILTERNODE
// (process function, restore function, graph links, priority); the mixer pulls frames through the graph by calling
// filterfn(state, frames, src, dst, requester). Each filter kind has its own state struct after that common header.
// Retail code: unit ealib/seg_2B4578 (SFILTER_add .. SFILTER_modifyECHO, stretch.c's helpers, the unpackers). EA
// source: snd/source/library/mix/*.c (SSX On Tour's DWARF: sfhpffir8.c, sflpffir8.c, sfbpffir8.c, sfsplit.c, sfsrc.c,
// supmutpf.c, stretch.c). Only the state structs of this group's functions are here (see the report for the others
// On Tour defines: RSFSTATE, LPFRCSTATE, RESONSTATE, AMPLFSTATE, MIXERSTATE, ECHOSTATE, FT16STATE, UNPACK*STATE).
//
// Names: SSX On Tour's DWARF (the functions' prototypes, e.g. SFILTER_modifyHPFFIR8(HPFSTATE*, int*),
// SFILTER_modifyLPFFIR8(LPFSTATE*, int*), SFILTER_modifyBPFFIR8(BPFSTATE*, int*), and the struct definitions) with
// every retail access on the same member; node sizes from retail's create-size table D_0044E848 (per filter type:
// SOURCE 0x20, LPFRC 0x28, LPF / HPF / BPF FIR8 0x120 each, RESON 0x40, AMPLF 0x20, ECHO 0x40, MIXER 0x24, SPLIT 0x28)
// and the mixer's own allocations (MIX_sethighpass: 288 bytes; the time stretcher: 6184).
// Retail's FIR state is the PS2 SIMD version: 0x100 bytes at node + 0x20 (16-byte aligned) instead of On Tour's 0x3C at
// + 0x1C; only its cut-off members are placed.
//
// C structs, flat (the SFILTERNODE is written out at the start of each state). Evidence tags: [R] the create / init
// functions and node sizes; [O] On Tour's DWARF; [U] retail use.

#include "layout.h"

// SFILTERNODE (0x1C [O]).
struct SFILTERNODE {
    int (*filterfn)(void* pstate, int frames, void* psrc, void* pdst, int req);
                                        // 0x00 [O]; [R] every create / init stores its process function
    void (*restorefn)(void* pstate);    // 0x04 [O]; [R] create / init (0 or the kind's restore function)
    SFILTERNODE* pfnnext;               // 0x08 [O]; [U] SFILTER_add / remove / connect; process functions pull their input
                                        //      from it (filterfn of pfnnext)
    SFILTERNODE* pfnnext2;              // 0x0C [O]; [R] create 0; [U] SFILTER_connect (second input)
    SFILTERNODE* pfnprev1;              // 0x10 [O]; [R] create 0; [U] SFILTER_add / remove
    SFILTERNODE* pfnprev2;              // 0x14 [O]; [R] create 0
    unsigned short priority;            // 0x18 [O]; [U] SFILTER_add keeps the list sorted by it (MIX_sethighpass: 80)
    unsigned char requester;            // 0x1A [O]; [U] passed on as the requester argument
    unsigned char clean;                // 0x1B [O]; [R] create 0
};

// UNPACKMTPFSTATE (0x3C [O]): MicroTalk decoder fed by the packet player (supmutpf.c). SFILTER_unpackmtpfinit /
// SFILTER_unpackmtpf / SFILTER_unpackmtpfrestore.
struct CMTBLKDecf;
struct UNPACKMTPFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]; [R] init stores filterfn / restorefn
    unsigned char* psample;             // 0x1C [O]; [R] init 0; [U] unpackmtpf: the packet SNDPKTPLAYI_get returned
    int totalframes;                    // 0x20 [O]; [R] init: the frame count (init block + 0xC); [U] unpackmtpf: the
                                        //      packet's frames (SNDPKTPLAYI_get output), 0 at the end
    int packetinstancehandle;           // 0x24 [O]; [R] init SNDPKTPLAYI_voicetopackethandle(master voice); [U]
                                        //      SNDPKTPLAYI_get / freeframes
    unsigned int prevframesunpacked;    // 0x28 [O]; [R] init 0; [U] frames decoded since the last freeframes report
    unsigned short curframe;            // 0x2C [O]; [R] init 0 (sh); [U] unpackmtpf adds the frames decoded (lhu / sh)
    unsigned short unk_2E;              // 0x2E padding
    int platformver;                    // 0x30 [O]; [R] init: the codec version (init block + 0x24); [U] < 3 picks the old
                                        //      codec (CMTBLKDecf_SetCodecVersion 0 / 1)
    CMTBLKDecf* pDecoder;               // 0x34 [O]; [R] init: a new 3432-byte CMTBLKDecf; [U] Decode / GetState /
                                        //      SetState / Feed; restore destroys it
    unsigned char samplechan;           // 0x38 [O]; [R] init SNDDRV_getsamplechan(voice) (sb); [U] lbu
    char pad[3];                        // 0x39 [O]
};

// SNDFIRSTATE, retail PS2 version (0x100 [R]: node 0x120 - 0x20). SNDI_fir8init sets it up, calcFIRCoeffs computes the
// coefficients from the cut-offs, SNDI_fir8 runs it.
struct SNDFIRSTATE {
    char unk_00[0xF4];                  // 0x00 history / coefficients in the SIMD layout (SNDI_fir8init writes 0x4..0xDC;
                                        //      On Tour: history float[8], coef float[5]); not mapped
    float cutLow;                       // 0xF4 [O]; [U] SFILTER_modifyHPFFIR8 / modifyBPFFIR8 store the low cut-off ratio;
                                        //      calcFIRCoeffs reads / writes it
    float cutHigh;                      // 0xF8 [O]; [U] SFILTER_modifyLPFFIR8 / modifyBPFFIR8 store the high cut-off ratio;
                                        //      calcFIRCoeffs reads it
    unsigned char unk_FC;               // 0xFC [U] calcFIRCoeffs writes it (sb)
    char unk_FD[3];                     // 0xFD padding
};

// LPFSTATE / HPFSTATE / BPFSTATE (0x120 [R] D_0044E848 types 2 / 3 / 4; MIX_sethighpass allocates 288 for HPF).
// SFILTER_create?PFFIR8 store the node fields and call SNDI_fir8init(&fir); SFILTER_modify?PFFIR8 set the cut-offs.
struct LPFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]
    int unk_1C;                         // 0x1C padding (fir is 16-byte aligned on the PS2)
    SNDFIRSTATE fir;                    // 0x20 [O] (at 0x1C there); [U] modifyLPFFIR8 writes + 0xF8, calcFIRCoeffs(fir, 2)
};

struct HPFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]
    int unk_1C;                         // 0x1C padding
    SNDFIRSTATE fir;                    // 0x20 [O]; [U] modifyHPFFIR8 writes + 0xF4, calcFIRCoeffs(fir, 3)
};

struct BPFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]
    int unk_1C;                         // 0x1C padding
    SNDFIRSTATE fir;                    // 0x20 [O]; [U] modifyBPFFIR8 writes + 0xF4 and + 0xF8, calcFIRCoeffs(fir, 4)
};

// TIMESTRETCHSTATE (retail 0x1828 [R] the 6184-byte node SFILTER_timestretchinit sets up; On Tour 0x1834 adds three
// members after output[]). stretch.c: SFILTER_timestretchinit / _timestretch / _timestretchsetratio, stretch,
// stretchframesneeded, determineaction, applyaction, transferframes.
struct TIMESTRETCHSTATE {
    SFILTERNODE sfn;                    // 0x000 [O]; [R] init filterfn / restorefn 0
    unsigned char* psideband;           // 0x01C [O]; [R] init: header + 6; [U] determineaction steps through it
    int packetinstancehandle;           // 0x020 [O]; [R] init: the voice's packet player or -1; [U] SFILTER_timestretch
                                        //      waits for SNDPKTPLAY_framesoutstanding when >= 0
    float stretchratio;                 // 0x024 [O]; [R] init 1.0f; [U] setratio (time mult / 4096 clamped to 0.5..2.0)
    float sampleerror;                  // 0x028 [O]; [R] init 0; [U] determineaction
    int windowsize;                     // 0x02C [O]; [R] init: header byte 1 * 2; [U] stretch / stretchframesneeded
    int framesleft;                     // 0x030 [O]; [R] init SNDI_getb(header + 2, 4); [U] stretch counts it down
    int inputsamples;                   // 0x034 [O]; [R] init 0; [U] stretch: the window kept in input[]
    int outputsamples;                  // 0x038 [O]; [R] init 0; [U] stretchframesneeded
    int outputstart;                    // 0x03C [O]; [R] init 0
    float input[510];                   // 0x040 [O]; [U] stretch copies the last window here
    float output[1020];                 // 0x838 [O]
};

// SPLITSTATE (0x28 [O] [R] D_0044E848 type 9). sfsplit.c: SFILTER_createSPLIT / SFILTER_splitter /
// SFILTER_splitrestore.
struct SPLITSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]; [R] create
    void* pdatabuf;                     // 0x1C [O]; [R] create; [U] splitter buffers the input for the second output;
                                        //      restore frees it
    int sizebuf;                        // 0x20 [O]; [R] create; [U] splitter
    short mustempty;                    // 0x24 [O]; [R] create (sh); [U] splitter (lh / sh)
    short isempty;                      // 0x26 [O]; [R] create (sh); [U] splitter (lh / sh)
};

// SRCSTATE (0x20 [O] [R] D_0044E848 type 0). sfsrc.c: SFILTER_createSOURCE / SFILTER_initSOURCE / SFILTER_src.
struct SRCSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]; [R] create / init
    float* pdata;                       // 0x1C [O]; [R] create / init; [U] SFILTER_src copies from it and advances it
};

CHECK_OFFSET(SFILTERNODE, filterfn, 0x00);
CHECK_OFFSET(SFILTERNODE, restorefn, 0x04);
CHECK_OFFSET(SFILTERNODE, pfnnext, 0x08);
CHECK_OFFSET(SFILTERNODE, pfnnext2, 0x0C);
CHECK_OFFSET(SFILTERNODE, pfnprev1, 0x10);
CHECK_OFFSET(SFILTERNODE, pfnprev2, 0x14);
CHECK_OFFSET(SFILTERNODE, priority, 0x18);
CHECK_OFFSET(SFILTERNODE, requester, 0x1A);
CHECK_OFFSET(SFILTERNODE, clean, 0x1B);
CHECK_SIZE(SFILTERNODE, 0x1C);

CHECK_OFFSET(UNPACKMTPFSTATE, sfn, 0x00);
CHECK_OFFSET(UNPACKMTPFSTATE, psample, 0x1C);
CHECK_OFFSET(UNPACKMTPFSTATE, totalframes, 0x20);
CHECK_OFFSET(UNPACKMTPFSTATE, packetinstancehandle, 0x24);
CHECK_OFFSET(UNPACKMTPFSTATE, prevframesunpacked, 0x28);
CHECK_OFFSET(UNPACKMTPFSTATE, curframe, 0x2C);
CHECK_OFFSET(UNPACKMTPFSTATE, unk_2E, 0x2E);
CHECK_OFFSET(UNPACKMTPFSTATE, platformver, 0x30);
CHECK_OFFSET(UNPACKMTPFSTATE, pDecoder, 0x34);
CHECK_OFFSET(UNPACKMTPFSTATE, samplechan, 0x38);
CHECK_OFFSET(UNPACKMTPFSTATE, pad, 0x39);
CHECK_SIZE(UNPACKMTPFSTATE, 0x3C);

CHECK_OFFSET(SNDFIRSTATE, unk_00, 0x00);
CHECK_OFFSET(SNDFIRSTATE, cutLow, 0xF4);
CHECK_OFFSET(SNDFIRSTATE, cutHigh, 0xF8);
CHECK_OFFSET(SNDFIRSTATE, unk_FC, 0xFC);
CHECK_OFFSET(SNDFIRSTATE, unk_FD, 0xFD);
CHECK_SIZE(SNDFIRSTATE, 0x100);

CHECK_OFFSET(LPFSTATE, sfn, 0x00);
CHECK_OFFSET(LPFSTATE, unk_1C, 0x1C);
CHECK_OFFSET(LPFSTATE, fir, 0x20);
CHECK_SIZE(LPFSTATE, 0x120);
CHECK_OFFSET(HPFSTATE, sfn, 0x00);
CHECK_OFFSET(HPFSTATE, unk_1C, 0x1C);
CHECK_OFFSET(HPFSTATE, fir, 0x20);
CHECK_SIZE(HPFSTATE, 0x120);
CHECK_OFFSET(BPFSTATE, sfn, 0x00);
CHECK_OFFSET(BPFSTATE, unk_1C, 0x1C);
CHECK_OFFSET(BPFSTATE, fir, 0x20);
CHECK_SIZE(BPFSTATE, 0x120);

CHECK_OFFSET(TIMESTRETCHSTATE, sfn, 0x000);
CHECK_OFFSET(TIMESTRETCHSTATE, psideband, 0x01C);
CHECK_OFFSET(TIMESTRETCHSTATE, packetinstancehandle, 0x020);
CHECK_OFFSET(TIMESTRETCHSTATE, stretchratio, 0x024);
CHECK_OFFSET(TIMESTRETCHSTATE, sampleerror, 0x028);
CHECK_OFFSET(TIMESTRETCHSTATE, windowsize, 0x02C);
CHECK_OFFSET(TIMESTRETCHSTATE, framesleft, 0x030);
CHECK_OFFSET(TIMESTRETCHSTATE, inputsamples, 0x034);
CHECK_OFFSET(TIMESTRETCHSTATE, outputsamples, 0x038);
CHECK_OFFSET(TIMESTRETCHSTATE, outputstart, 0x03C);
CHECK_OFFSET(TIMESTRETCHSTATE, input, 0x040);
CHECK_OFFSET(TIMESTRETCHSTATE, output, 0x838);
CHECK_SIZE(TIMESTRETCHSTATE, 0x1828);

CHECK_OFFSET(SPLITSTATE, sfn, 0x00);
CHECK_OFFSET(SPLITSTATE, pdatabuf, 0x1C);
CHECK_OFFSET(SPLITSTATE, sizebuf, 0x20);
CHECK_OFFSET(SPLITSTATE, mustempty, 0x24);
CHECK_OFFSET(SPLITSTATE, isempty, 0x26);
CHECK_SIZE(SPLITSTATE, 0x28);

CHECK_OFFSET(SRCSTATE, sfn, 0x00);
CHECK_OFFSET(SRCSTATE, pdata, 0x1C);
CHECK_SIZE(SRCSTATE, 0x20);

#endif /* EALIB_SFILTER_H */
