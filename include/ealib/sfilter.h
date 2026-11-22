#ifndef EALIB_SFILTER_H
#define EALIB_SFILTER_H

// EA SND's software mixer filter graph (C): every filter is a node whose state struct starts with an SFILTERNODE
// (process function, restore function, graph links, priority); the mixer pulls frames through the graph by calling
// filterfn(state, frames, src, dst, requester). Each filter kind has its own state struct after that common header.
// Retail code: unit ealib/seg_2B4578 (SFILTER_add .. SFILTER_modifyECHO, stretch.c's helpers, the unpackers). EA
// source: snd/source/library/mix/*.c (SSX On Tour's DWARF: sfhpffir8.c, sflpffir8.c, sfbpffir8.c, sfsplit.c, sfsrc.c,
// supmutpf.c, stretch.c). Not laid out yet: On Tour's RSFSTATE and FT16STATE (no retail evidence gathered).
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

// C units (ealib/seg_2B4578.c, ealib/seg_2DCAE8.c) name these types without the struct keyword (views group D).
// CMTBLKDecf's typedef is in ealib/mtdecf.h (pDecoder spells `struct CMTBLKDecf`), so both headers can be included.
#ifndef __cplusplus
typedef struct SFILTERNODE SFILTERNODE;
typedef struct UNPACKFSTATE UNPACKFSTATE;
typedef struct UNPACKLFSTATE UNPACKLFSTATE;
typedef struct UNPACKMTPFSTATE UNPACKMTPFSTATE;
typedef struct SNDFIRSTATE SNDFIRSTATE;
typedef struct LPFSTATE LPFSTATE;
typedef struct HPFSTATE HPFSTATE;
typedef struct BPFSTATE BPFSTATE;
typedef struct TIMESTRETCHSTATE TIMESTRETCHSTATE;
typedef struct SPLITSTATE SPLITSTATE;
typedef struct SRCSTATE SRCSTATE;
typedef struct UNPACKINITPARAMS UNPACKINITPARAMS;
typedef struct UNPACKPFSTATE UNPACKPFSTATE;
typedef struct UNPACKXAFSTATE UNPACKXAFSTATE;
typedef struct UNPACKXALFSTATE UNPACKXALFSTATE;
typedef struct UNPACKXAPFSTATE UNPACKXAPFSTATE;
typedef struct UNPACKMTFSTATE UNPACKMTFSTATE;
typedef struct UNPACKMTLFSTATE UNPACKMTLFSTATE;
typedef struct LPFRC LPFRC;
typedef struct LPFRCSTATE LPFRCSTATE;
typedef struct RESONSTATE RESONSTATE;
typedef struct AMPLFSTATE AMPLFSTATE;
typedef struct ECHOSTATE ECHOSTATE;
typedef struct MIXERSTATE MIXERSTATE;
typedef struct SNDFILTERDEF SNDFILTERDEF;
typedef struct SNDMIXVOICE SNDMIXVOICE;
#endif

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

// UNPACKFSTATE (0x2C [O], views group D: R_9A00): the plain PCM unpack filter (supf.c). [O] SFILTER_unpackf is its
// process function; [U] retail SFILTER_unpackf reads the samples from psample, steps curframe up to totalframes and picks
// 8- or 16-bit samples by bits16.
struct UNPACKFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]
    char* psample;                      // 0x1C [O]; [U] the sample data
    unsigned int curframe;              // 0x20 [O]; [U] the next frame
    unsigned int totalframes;           // 0x24 [O]; [U] curframe < totalframes
    int bits16;                         // 0x28 [O]; [U] 16-bit samples when set
};

CHECK_OFFSET(UNPACKFSTATE, sfn, 0x00);
CHECK_OFFSET(UNPACKFSTATE, psample, 0x1C);
CHECK_OFFSET(UNPACKFSTATE, curframe, 0x20);
CHECK_OFFSET(UNPACKFSTATE, totalframes, 0x24);
CHECK_OFFSET(UNPACKFSTATE, bits16, 0x28);
CHECK_SIZE(UNPACKFSTATE, 0x2C);

// UNPACKLFSTATE (0x30 [O], views group D: R_9B10): the looping PCM unpack filter (suplf.c). [U] retail SFILTER_unpacklf
// loops curframe back to sustainstart when it reaches sustainend.
struct UNPACKLFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]
    char* psample;                      // 0x1C [O]; [U] the sample data
    unsigned int curframe;              // 0x20 [O]; [U] the next frame
    unsigned int sustainstart;          // 0x24 [O]; [U] the loop start
    unsigned int sustainend;            // 0x28 [O]; [U] the loop end
    int bits16;                         // 0x2C [O]; [U] 16-bit samples when set
};

CHECK_OFFSET(UNPACKLFSTATE, sfn, 0x00);
CHECK_OFFSET(UNPACKLFSTATE, psample, 0x1C);
CHECK_OFFSET(UNPACKLFSTATE, curframe, 0x20);
CHECK_OFFSET(UNPACKLFSTATE, sustainstart, 0x24);
CHECK_OFFSET(UNPACKLFSTATE, sustainend, 0x28);
CHECK_OFFSET(UNPACKLFSTATE, bits16, 0x2C);
CHECK_SIZE(UNPACKLFSTATE, 0x30);

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
    struct CMTBLKDecf* pDecoder;               // 0x34 [O]; [R] init: a new 3432-byte CMTBLKDecf; [U] Decode / GetState /
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

// ---- Unpackers, the other filter kinds, the filter definition and the mixer voice (types round, group D) ----
// Same sources and tags as above. The EA-XA / MicroTalk unpackers own a decoder object (ealib/eaxadecf.h CEAXABLKDecf,
// ealib/mtdecf.h CMTBLKDecf), so their pDecoder members spell `struct`.
struct CEAXABLKDecf;

// UNPACKINITPARAMS (0x2C [O]): the block MIX_playinit fills and passes to the chosen unpacker's init
// (SFILTER_unpack*init(void* pstate, UNPACKINITPARAMS* pupip) [O]). [R] MIX_playinit (view Par_8158) stores every member
// in this order; [U] the inits read totalframes 0xC, voice 0x1C, platformver 0x24.
struct UNPACKINITPARAMS {
    void* psample;                      // 0x00 [O]; [R] MIX_playinit; [U] the inits copy it
    void* ploopstart;                   // 0x04 [O]; [R]; [U] SFILTER_unpackxalfinit's ploop
    int numchan;                        // 0x08 [O]; [R]
    int totalframes;                    // 0x0C [O]; [R]; [U] the inits' totalframes
    int sustainstart;                   // 0x10 [O]; [R]; [U] the looping inits
    int sustainend;                     // 0x14 [O]; [R]; [U] the looping inits
    int bits16;                         // 0x18 [O]; [R] D_0050AA62; [U] SFILTER_unpackpfinit (sb into bits16)
    int voice;                          // 0x1C [O]; [R]; [U] the packet inits' SNDPKTPLAYI_voicetopackethandle /
                                        //      SNDDRV_getsamplechan argument
    int cpu;                            // 0x20 [O]; [R] MIX_playinit stores 1
    int platformver;                    // 0x24 [O]; [R]; [U] the inits copy it (codec version)
    void* unpackgetframe;               // 0x28 [O] (`int (*unpackgetframe)()`): [R] the init returns its getframe here
};

CHECK_OFFSET(UNPACKINITPARAMS, psample, 0x00);
CHECK_OFFSET(UNPACKINITPARAMS, ploopstart, 0x04);
CHECK_OFFSET(UNPACKINITPARAMS, numchan, 0x08);
CHECK_OFFSET(UNPACKINITPARAMS, totalframes, 0x0C);
CHECK_OFFSET(UNPACKINITPARAMS, sustainstart, 0x10);
CHECK_OFFSET(UNPACKINITPARAMS, sustainend, 0x14);
CHECK_OFFSET(UNPACKINITPARAMS, bits16, 0x18);
CHECK_OFFSET(UNPACKINITPARAMS, voice, 0x1C);
CHECK_OFFSET(UNPACKINITPARAMS, cpu, 0x20);
CHECK_OFFSET(UNPACKINITPARAMS, platformver, 0x24);
CHECK_OFFSET(UNPACKINITPARAMS, unpackgetframe, 0x28);
CHECK_SIZE(UNPACKINITPARAMS, 0x2C);

// UNPACKPFSTATE (0x34 [O]): PCM fed by the packet player (suppf.c). [O] SFILTER_unpackpf's local `UNPACKPFSTATE* pups`;
// [R] SFILTER_unpackpfinit stores the members below; [U] SFILTER_unpackpf reads them with the same widths.
struct UNPACKPFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]; [R] init stores filterfn / restorefn
    signed char* psample;               // 0x1C [O]; [R] init 0
    int totalframes;                    // 0x20 [O]; [R] init -1
    int curframe;                       // 0x24 [O]; [R] init 0
    int packetinstancehandle;           // 0x28 [O]; [R] init SNDPKTPLAYI_voicetopackethandle(master voice)
    unsigned int prevframesunpacked;    // 0x2C [O]; [R] init 0
    unsigned char bits16;               // 0x30 [O]; [R] init: the init block's bits16 (sb)
    unsigned char samplechan;           // 0x31 [O]; [R] init SNDDRV_getsamplechan(voice) (sb)
    char pad[2];                        // 0x32 [O]
};

CHECK_OFFSET(UNPACKPFSTATE, sfn, 0x00);
CHECK_OFFSET(UNPACKPFSTATE, psample, 0x1C);
CHECK_OFFSET(UNPACKPFSTATE, totalframes, 0x20);
CHECK_OFFSET(UNPACKPFSTATE, curframe, 0x24);
CHECK_OFFSET(UNPACKPFSTATE, packetinstancehandle, 0x28);
CHECK_OFFSET(UNPACKPFSTATE, prevframesunpacked, 0x2C);
CHECK_OFFSET(UNPACKPFSTATE, bits16, 0x30);
CHECK_OFFSET(UNPACKPFSTATE, samplechan, 0x31);
CHECK_OFFSET(UNPACKPFSTATE, pad, 0x32);
CHECK_SIZE(UNPACKPFSTATE, 0x34);

// UNPACKXAFSTATE (0x2C [O]): EA-XA (float) from memory (supxaf.c). [O] SFILTER_unpackxafinit's local
// `UNPACKXAFSTATE* pups`; [R] the init: pDecoder = a new 168-byte CEAXABLKDecf, psample, totalframes, curframe = 0;
// [U] SFILTER_unpackxaf / unpackgetframexaf compare curframe with totalframes.
struct UNPACKXAFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]; [R] init
    struct CEAXABLKDecf* pDecoder;      // 0x1C [O]; [R] init: new CEAXABLKDecf (168 bytes)
    unsigned char* psample;             // 0x20 [O]; [R] init
    int totalframes;                    // 0x24 [O]; [R] init; [U] the end test
    int curframe;                       // 0x28 [O]; [R] init 0; [U] advanced per decode
};

CHECK_OFFSET(UNPACKXAFSTATE, sfn, 0x00);
CHECK_OFFSET(UNPACKXAFSTATE, pDecoder, 0x1C);
CHECK_OFFSET(UNPACKXAFSTATE, psample, 0x20);
CHECK_OFFSET(UNPACKXAFSTATE, totalframes, 0x24);
CHECK_OFFSET(UNPACKXAFSTATE, curframe, 0x28);
CHECK_SIZE(UNPACKXAFSTATE, 0x2C);

// UNPACKXALFSTATE (0x54 [O]): looping EA-XA (float) from memory (supxalf.c). [O] SFILTER_unpackxalf's local
// `UNPACKXALFSTATE* pups`; [R] SFILTER_unpackxalfinit stores every member from the init block (ploop = ploopstart,
// loopstart / loopend = sustainstart / sustainend, startblockmet / feddatabeforess = 0) and a 168-byte CEAXABLKDecf;
// [U] the process function keeps the decoder's history (XAFSTATE) in sustainstarts1 / sustainstarts2.
struct UNPACKXALFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]; [R] init
    struct CEAXABLKDecf* pDecoder;      // 0x1C [O]; [R] init: new CEAXABLKDecf
    int curframe;                       // 0x20 [O]; [U] advanced, reset to loopstart
    int loopstart;                      // 0x24 [O]; [R] init = sustainstart
    int loopend;                        // 0x28 [O]; [R] init = sustainend
    void* ploop;                        // 0x2C [O]; [R] init = ploopstart
    int platformver;                    // 0x30 [O]; [R] init
    unsigned char* psample;             // 0x34 [O]; [R] init
    int totalframes;                    // 0x38 [O]; [R] init
    int startblockmet;                  // 0x3C [O]; [R] init 0
    int feddatabeforess;                // 0x40 [O]; [R] init 0
    int sustainstart;                   // 0x44 [O]; [R] init
    int sustainend;                     // 0x48 [O]; [R] init
    float sustainstarts1;               // 0x4C [O]; [U] the saved history sample1 at the sustain start
    float sustainstarts2;               // 0x50 [O]; [U] sample2
};

CHECK_OFFSET(UNPACKXALFSTATE, sfn, 0x00);
CHECK_OFFSET(UNPACKXALFSTATE, pDecoder, 0x1C);
CHECK_OFFSET(UNPACKXALFSTATE, curframe, 0x20);
CHECK_OFFSET(UNPACKXALFSTATE, loopstart, 0x24);
CHECK_OFFSET(UNPACKXALFSTATE, loopend, 0x28);
CHECK_OFFSET(UNPACKXALFSTATE, ploop, 0x2C);
CHECK_OFFSET(UNPACKXALFSTATE, platformver, 0x30);
CHECK_OFFSET(UNPACKXALFSTATE, psample, 0x34);
CHECK_OFFSET(UNPACKXALFSTATE, totalframes, 0x38);
CHECK_OFFSET(UNPACKXALFSTATE, startblockmet, 0x3C);
CHECK_OFFSET(UNPACKXALFSTATE, feddatabeforess, 0x40);
CHECK_OFFSET(UNPACKXALFSTATE, sustainstart, 0x44);
CHECK_OFFSET(UNPACKXALFSTATE, sustainend, 0x48);
CHECK_OFFSET(UNPACKXALFSTATE, sustainstarts1, 0x4C);
CHECK_OFFSET(UNPACKXALFSTATE, sustainstarts2, 0x50);
CHECK_SIZE(UNPACKXALFSTATE, 0x54);

// UNPACKXAPFSTATE (0x3C [O]): EA-XA (float) fed by the packet player (supxapf.c). [O] SFILTER_unpackxapfinit's local
// `UNPACKXAPFSTATE* pups`; [R] the init stores every member below.
struct UNPACKXAPFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]; [R] init
    struct CEAXABLKDecf* pDecoder;      // 0x1C [O]; [R] init: new CEAXABLKDecf (168 bytes)
    unsigned char* psample;             // 0x20 [O]; [R] init 0
    int totalframes;                    // 0x24 [O]; [R] init: the init block's totalframes
    int curframe;                       // 0x28 [O]; [R] init 0
    int platformver;                    // 0x2C [O]; [R] init
    int packetinstancehandle;           // 0x30 [O]; [R] init SNDPKTPLAYI_voicetopackethandle
    unsigned int prevframesunpacked;    // 0x34 [O]; [R] init 0
    unsigned char samplechan;           // 0x38 [O]; [R] init SNDDRV_getsamplechan (sb)
    char pad[2];                        // 0x39 [O] (0x3B is tail padding)
};

CHECK_OFFSET(UNPACKXAPFSTATE, sfn, 0x00);
CHECK_OFFSET(UNPACKXAPFSTATE, pDecoder, 0x1C);
CHECK_OFFSET(UNPACKXAPFSTATE, psample, 0x20);
CHECK_OFFSET(UNPACKXAPFSTATE, totalframes, 0x24);
CHECK_OFFSET(UNPACKXAPFSTATE, curframe, 0x28);
CHECK_OFFSET(UNPACKXAPFSTATE, platformver, 0x2C);
CHECK_OFFSET(UNPACKXAPFSTATE, packetinstancehandle, 0x30);
CHECK_OFFSET(UNPACKXAPFSTATE, prevframesunpacked, 0x34);
CHECK_OFFSET(UNPACKXAPFSTATE, samplechan, 0x38);
CHECK_OFFSET(UNPACKXAPFSTATE, pad, 0x39);
CHECK_SIZE(UNPACKXAPFSTATE, 0x3C);

// UNPACKMTFSTATE (0x2C [O]): MicroTalk from memory (supmutf.c). [O] SFILTER_unpackmtf's local `UNPACKMTFSTATE* pups`;
// [U] retail compares curframe with totalframes (unsigned) and decodes with pDecoder; SFILTER_unpackmtfrestore destroys
// pDecoder.
struct UNPACKMTFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]
    unsigned int totalframes;           // 0x1C [O]; [U] the end test (unsigned)
    unsigned int curframe;              // 0x20 [O]; [U] advanced per decode
    int platformver;                    // 0x24 [O]
    struct CMTBLKDecf* pDecoder;        // 0x28 [O]; [U] CMTBLKDecf_Decode; restore destroys it
};

CHECK_OFFSET(UNPACKMTFSTATE, sfn, 0x00);
CHECK_OFFSET(UNPACKMTFSTATE, totalframes, 0x1C);
CHECK_OFFSET(UNPACKMTFSTATE, curframe, 0x20);
CHECK_OFFSET(UNPACKMTFSTATE, platformver, 0x24);
CHECK_OFFSET(UNPACKMTFSTATE, pDecoder, 0x28);
CHECK_SIZE(UNPACKMTFSTATE, 0x2C);

// UNPACKMTLFSTATE (0x30 [O]): looping MicroTalk from memory (supmutlf.c). [O] SFILTER_unpackmtlf's local
// `UNPACKMTLFSTATE* pups`; [U] pDecoder goes to CMTBLKDecf_Decode / SetState / Feed, curframe is advanced and reset to
// loopstart, the loop length is loopend - loopstart + 1 and ploop is fed back.
struct UNPACKMTLFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]
    struct CMTBLKDecf* pDecoder;        // 0x1C [O]; [U]
    int curframe;                       // 0x20 [O]; [U]
    int loopstart;                      // 0x24 [O]; [U]
    int loopend;                        // 0x28 [O]; [U]
    void* ploop;                        // 0x2C [O]; [U] fed back to the decoder at the loop
};

CHECK_OFFSET(UNPACKMTLFSTATE, sfn, 0x00);
CHECK_OFFSET(UNPACKMTLFSTATE, pDecoder, 0x1C);
CHECK_OFFSET(UNPACKMTLFSTATE, curframe, 0x20);
CHECK_OFFSET(UNPACKMTLFSTATE, loopstart, 0x24);
CHECK_OFFSET(UNPACKMTLFSTATE, loopend, 0x28);
CHECK_OFFSET(UNPACKMTLFSTATE, ploop, 0x2C);
CHECK_SIZE(UNPACKMTLFSTATE, 0x30);

// LPFRC (0xC [O]) / LPFRCSTATE (0x28 [O] [R] D_0044E848 LPFRC 0x28): the RC low-pass filter. [O]
// SFILTER_createLPFRC(LPFRCSTATE* pls); [R] create clears lpf.accumulator; [U] SFILTER_modifyLPFRC writes leakage /
// inverse (floats).
struct LPFRC {
    float accumulator;                  // 0x0 [O]; [R] create 0
    float leakage;                      // 0x4 [O]; [U] modifyLPFRC
    float inverse;                      // 0x8 [O]; [U] modifyLPFRC
};

CHECK_OFFSET(LPFRC, accumulator, 0x0);
CHECK_OFFSET(LPFRC, leakage, 0x4);
CHECK_OFFSET(LPFRC, inverse, 0x8);
CHECK_SIZE(LPFRC, 0xC);

struct LPFRCSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]; [R] create
    struct LPFRC lpf;                   // 0x1C [O]
};

CHECK_OFFSET(LPFRCSTATE, sfn, 0x00);
CHECK_OFFSET(LPFRCSTATE, lpf, 0x1C);
CHECK_SIZE(LPFRCSTATE, 0x28);

// RESONSTATE (0x40 [O] [R] D_0044E848 RESON 0x40): the resonator. [O] SFILTER_createRESON(RESONSTATE* prs); [R] create
// clears the node and xn / ynminus1 / ynminus2; [U] SFILTER_modifyRESON writes samplerate (int) and the coefficients.
struct RESONSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]; [R] create
    int samplerate;                     // 0x1C [O]; [U] modifyRESON
    float resonfreq;                    // 0x20 [O]; [U] modifyRESON
    float bw;                           // 0x24 [O]; [U] modifyRESON
    float r;                            // 0x28 [O]; [U] modifyRESON
    float costheta;                     // 0x2C [O]; [U] modifyRESON
    float a0;                           // 0x30 [O]; [U] modifyRESON
    float xn;                           // 0x34 [O]; [R] create 0
    float ynminus1;                     // 0x38 [O]; [R] create 0
    float ynminus2;                     // 0x3C [O]; [R] create 0
};

CHECK_OFFSET(RESONSTATE, sfn, 0x00);
CHECK_OFFSET(RESONSTATE, samplerate, 0x1C);
CHECK_OFFSET(RESONSTATE, resonfreq, 0x20);
CHECK_OFFSET(RESONSTATE, bw, 0x24);
CHECK_OFFSET(RESONSTATE, r, 0x28);
CHECK_OFFSET(RESONSTATE, costheta, 0x2C);
CHECK_OFFSET(RESONSTATE, a0, 0x30);
CHECK_OFFSET(RESONSTATE, xn, 0x34);
CHECK_OFFSET(RESONSTATE, ynminus1, 0x38);
CHECK_OFFSET(RESONSTATE, ynminus2, 0x3C);
CHECK_SIZE(RESONSTATE, 0x40);

// AMPLFSTATE (0x20 [O] [R] D_0044E848 AMPLF 0x20): the amplifier. [O] SFILTER_createAMPLF(AMPLFSTATE* pas); [R] create
// clears the node and gain.
struct AMPLFSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]; [R] create
    float gain;                         // 0x1C [O]; [R] create 0
};

CHECK_OFFSET(AMPLFSTATE, sfn, 0x00);
CHECK_OFFSET(AMPLFSTATE, gain, 0x1C);
CHECK_SIZE(AMPLFSTATE, 0x20);

// ECHOSTATE, retail PS2 version (0x40 [R] D_0044E848 ECHO 0x40; On Tour 0x34): the echo. [O] SFILTER_createECHO(
// ECHOSTATE* pes) and the member names; retail keeps fblevels / fbneglevels in a 16-byte quad at 0x30 (SFILTER_echo
// passes node + 0x30 to a lqc2 VU0 loop), so 0x2C and 0x38..0x3F are padding. [U] SFILTER_modifyECHO stores the
// members below.
struct ECHOSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]; [R] create
    float* pdelaybuf;                   // 0x1C [O]; [U] modifyECHO: the allocation rounded up to 16 bytes
    float* punaligneddelaybuf;          // 0x20 [O]; [U] modifyECHO: the allocation (freed by the restore)
    int sampleindex;                    // 0x24 [O]; [U] modifyECHO 0
    int bufsamples;                     // 0x28 [O]; [U] modifyECHO: the delay in samples
    int unk_2C;                         // 0x2C padding (the quad below is 16-byte aligned)
    float fblevels;                     // 0x30 [O]; [U] modifyECHO: the feedback gain
    float fbneglevels;                  // 0x34 [O]; [U] modifyECHO: -gain
    char unk_38[0x8];                   // 0x38 the rest of the VU0 quad
};

CHECK_OFFSET(ECHOSTATE, sfn, 0x00);
CHECK_OFFSET(ECHOSTATE, pdelaybuf, 0x1C);
CHECK_OFFSET(ECHOSTATE, punaligneddelaybuf, 0x20);
CHECK_OFFSET(ECHOSTATE, sampleindex, 0x24);
CHECK_OFFSET(ECHOSTATE, bufsamples, 0x28);
CHECK_OFFSET(ECHOSTATE, unk_2C, 0x2C);
CHECK_OFFSET(ECHOSTATE, fblevels, 0x30);
CHECK_OFFSET(ECHOSTATE, fbneglevels, 0x34);
CHECK_OFFSET(ECHOSTATE, unk_38, 0x38);
CHECK_SIZE(ECHOSTATE, 0x40);

// MIXERSTATE (0x24 [O] [R] D_0044E848 MIXER 0x24): the two-input mixer. [O] SFILTER_createMIX(MIXERSTATE* pms); [R]
// create clears the node, ptempbuf and tempbufframes.
struct MIXERSTATE {
    SFILTERNODE sfn;                    // 0x00 [O]; [R] create
    float* ptempbuf;                    // 0x1C [O]; [R] create 0
    int tempbufframes;                  // 0x20 [O]; [R] create 0
};

CHECK_OFFSET(MIXERSTATE, sfn, 0x00);
CHECK_OFFSET(MIXERSTATE, ptempbuf, 0x1C);
CHECK_OFFSET(MIXERSTATE, tempbufframes, 0x20);
CHECK_SIZE(MIXERSTATE, 0x24);

// SNDFILTERDEF (0x18 [O]): a registered filter kind. [U] MIX_filteradd allocates statesize bytes, calls
// filterinit(state, id, channel), stores priority as the node's priority and filter / filterrestore as its filterfn /
// restorefn.
struct SNDFILTERDEF {
    int id;                             // 0x00 [O]; [U] passed to filterinit
    int statesize;                      // 0x04 [O]; [U] the allocation size
    int priority;                       // 0x08 [O]; [U] stored as the node's u16 priority
    void (*filterinit)(void* pstate, int id, int chan);
                                        // 0x0C [O]; [U] called on the new state
    int (*filter)(void* pstate, int frames, void* psrc, void* pdst, int req);
                                        // 0x10 [O]; [U] becomes the node's filterfn
    void (*filterrestore)(void* pstate); // 0x14 [O]; [U] becomes the node's restorefn
};

CHECK_OFFSET(SNDFILTERDEF, id, 0x00);
CHECK_OFFSET(SNDFILTERDEF, statesize, 0x04);
CHECK_OFFSET(SNDFILTERDEF, priority, 0x08);
CHECK_OFFSET(SNDFILTERDEF, filterinit, 0x0C);
CHECK_OFFSET(SNDFILTERDEF, filter, 0x10);
CHECK_OFFSET(SNDFILTERDEF, filterrestore, 0x14);
CHECK_SIZE(SNDFILTERDEF, 0x18);

// SNDMIXVOICE, retail PS2 version (0x60 [R]: MIX_create allocates n * 0x60, the voice table at D_00515B40 + 0x1DC is
// indexed with a 0x60 stride; On Tour 0x5C): one software-mixer voice. [O] MIX_sethighpass's local `SNDMIXVOICE*
// psmv` and the member names; retail's views of it in ealib/seg_2B4578: Ent_8468, Ent_85D0, E_8560, E_8598, Ent_8158,
// E_8B80. Retail has one wet bus, kept inline as floats (On Tour: float* to per-bus arrays), and its 0x50..0x5B differ
// from On Tour's prss / plps (a clipping-filter node at 0x54, func_003C8B80).
struct SNDMIXVOICE {
    char status;                        // 0x00 [O]; [U] the views' state / active byte
    signed char gainchanged;            // 0x01 [O]; [U] SNDMIX_setdrygain / setwetgain set it, MIX_play clears it
    short pad;                          // 0x02 [O]
    float drybusgainprev[6];            // 0x04 [O]; [U] the gains the mixer last applied (ramped towards drybusgain)
    float drybusgain[6];                // 0x1C [O]; [U] SNDMIX_setdrygain stores the target gains
    float wetbusgainprev[1];            // 0x34 [O] (On Tour float*); [U] as drybusgainprev for the wet bus
    float wetbusgain[1];                // 0x38 [O] (On Tour float*); [U] SNDMIX_setwetgain stores wetbusgain[k]
    float lastmixed;                    // 0x3C [O]; [U] MIX_play clears it, SNDMIXI_volramp stores the last mixed
                                        //      level, MIX_stop scales the remaining gains by it
    SFILTERNODE* filterhead;            // 0x40 [O]; [U] the voice's filter list (SFILTER_add / remove take
                                        //      &filterhead); MIX_sethighpass links phps into it
    void* unpackgetframe;               // 0x44 [O] (`int (*unpackgetframe)()`); [U] MIX_playinit stores what the init
                                        //      returned
    void* pups;                         // 0x48 [O]; [U] MIX_playinit: the unpacker state
    TIMESTRETCHSTATE* ptss;             // 0x4C [O]; [U] MIX_playinit: the 6184-byte time stretcher
    int unk_50;                         // 0x50 [R] MIX_playinit 0 (On Tour has RSFSTATE* prss here)
    void* unk_54;                       // 0x54 [U] func_003C8B80: the voice's 80-byte clipping filter node (priority 0x78)
    int unk_58;                         // 0x58 [R] MIX_playinit 0
    HPFSTATE* phps;                     // 0x5C [O]; [R] MIX_playinit 0; [U] MIX_sethighpass: the 288-byte high-pass node
};

CHECK_OFFSET(SNDMIXVOICE, status, 0x00);
CHECK_OFFSET(SNDMIXVOICE, gainchanged, 0x01);
CHECK_OFFSET(SNDMIXVOICE, pad, 0x02);
CHECK_OFFSET(SNDMIXVOICE, drybusgainprev, 0x04);
CHECK_OFFSET(SNDMIXVOICE, drybusgain, 0x1C);
CHECK_OFFSET(SNDMIXVOICE, wetbusgainprev, 0x34);
CHECK_OFFSET(SNDMIXVOICE, wetbusgain, 0x38);
CHECK_OFFSET(SNDMIXVOICE, lastmixed, 0x3C);
CHECK_OFFSET(SNDMIXVOICE, filterhead, 0x40);
CHECK_OFFSET(SNDMIXVOICE, unpackgetframe, 0x44);
CHECK_OFFSET(SNDMIXVOICE, pups, 0x48);
CHECK_OFFSET(SNDMIXVOICE, ptss, 0x4C);
CHECK_OFFSET(SNDMIXVOICE, unk_50, 0x50);
CHECK_OFFSET(SNDMIXVOICE, unk_54, 0x54);
CHECK_OFFSET(SNDMIXVOICE, unk_58, 0x58);
CHECK_OFFSET(SNDMIXVOICE, phps, 0x5C);
CHECK_SIZE(SNDMIXVOICE, 0x60);

#endif /* EALIB_SFILTER_H */
