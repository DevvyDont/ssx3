#ifndef EALIB_SMIDI_H
#define EALIB_SMIDI_H

// EA SND's MIDI sequence player (smidi.c / smidaz.c): 16 song instances, each with 16 MIDI channels, and the 24
// voices (partials) they play (views group D: B_93C8, R_93C8, E_93C8 in ealib/seg_2B4578).
// EA source: snd/source/library/cmn/smidi.c (On Tour: snd 9.06.03-psp). Retail code: unit ealib/seg_2B4578
// (SNDI_midi*, SNDMIDI_*), global sndms (retail 0x0050A120; also addressed as D_0050A820 = sndms.smp).
// Tags: [O] SSX On Tour's DWARF (struct names, members, offsets, the paired functions); [R] retail symbols / code;
// [U] retail use. Bit fields are allocated from bit 0 (little-endian g++/gcc); On Tour's DWARF bit offsets give the
// same order (azimuth10 at bits 0..9 ... patch at 24..31).

#include "layout.h"

// C units (ealib/seg_2B4578.c) name these types without the struct keyword.
#ifndef __cplusplus
typedef struct SNDMIDICHAN SNDMIDICHAN;
typedef struct SNDSEQINSTANCE SNDSEQINSTANCE;
typedef struct SNDMIDIPARTIAL SNDMIDIPARTIAL;
typedef struct SNDMIDISTATE SNDMIDISTATE;
#endif

// One MIDI channel of a song. [O] struct SNDMIDICHAN (0x4) and SNDI_midiparsepitchbend(int seqindex, SNDMIDICHAN* psmc,
// int chan, int data); [U] the retail pitch-bend parser (view B_93C8 {lo:17, f:7, hi:8}) rewrites bits 17..23 = bend.
struct SNDMIDICHAN {
    unsigned int azimuth10 : 10;    // bits 0..9 [O] the channel's pan
    unsigned int vol : 7;           // bits 10..16 [O]
    unsigned int bend : 7;          // bits 17..23 [O]; [U] SNDI_midiparsepitchbend
    int patch : 8;                  // bits 24..31 [O]
};

CHECK_SIZE(SNDMIDICHAN, 0x4);

// One playing song. [O] struct SNDSEQINSTANCE (0x70); [U] the retail sequencer (view R_93C8) steps psongpos (0x8) and
// indexes sndms.ssi[] with a 0x70 stride.
struct SNDSEQINSTANCE {
    unsigned int servertick;        // 0x00 [O]
    unsigned int miditick;          // 0x04 [O]
    unsigned char* psongpos;        // 0x08 [O]; [U] the current event pointer
    unsigned char* prepeatpos;      // 0x0C [O]
    int seqhandle;                  // 0x10 [O]
    int progvol;                    // 0x14 [O]
    int fadepertick;                // 0x18 [O]
    int fadetargetvol;              // 0x1C [O]
    unsigned short azimuth;         // 0x20 [O]
    unsigned short pitchmult;       // 0x22 [O]
    unsigned short tempo;           // 0x24 [O]
    unsigned char numrepeats;       // 0x26 [O]
    unsigned char nextevent;        // 0x27 [O]
    signed char status;             // 0x28 [O]
    char pad[3];                    // 0x29 [O]
    short bhandle;                  // 0x2C [O]
    short drumbhandle;              // 0x2E [O]
    struct SNDMIDICHAN smc[16];            // 0x30 [O]
};

CHECK_OFFSET(SNDSEQINSTANCE, servertick, 0x00);
CHECK_OFFSET(SNDSEQINSTANCE, miditick, 0x04);
CHECK_OFFSET(SNDSEQINSTANCE, psongpos, 0x08);
CHECK_OFFSET(SNDSEQINSTANCE, prepeatpos, 0x0C);
CHECK_OFFSET(SNDSEQINSTANCE, seqhandle, 0x10);
CHECK_OFFSET(SNDSEQINSTANCE, progvol, 0x14);
CHECK_OFFSET(SNDSEQINSTANCE, fadepertick, 0x18);
CHECK_OFFSET(SNDSEQINSTANCE, fadetargetvol, 0x1C);
CHECK_OFFSET(SNDSEQINSTANCE, azimuth, 0x20);
CHECK_OFFSET(SNDSEQINSTANCE, pitchmult, 0x22);
CHECK_OFFSET(SNDSEQINSTANCE, tempo, 0x24);
CHECK_OFFSET(SNDSEQINSTANCE, numrepeats, 0x26);
CHECK_OFFSET(SNDSEQINSTANCE, nextevent, 0x27);
CHECK_OFFSET(SNDSEQINSTANCE, status, 0x28);
CHECK_OFFSET(SNDSEQINSTANCE, pad, 0x29);
CHECK_OFFSET(SNDSEQINSTANCE, bhandle, 0x2C);
CHECK_OFFSET(SNDSEQINSTANCE, drumbhandle, 0x2E);
CHECK_OFFSET(SNDSEQINSTANCE, smc, 0x30);
CHECK_SIZE(SNDSEQINSTANCE, 0x70);

// One voice a song plays. [O] struct SNDMIDIPARTIAL (0x8); [U] view E_93C8 {id, owner, sub, pad} over sndms.smp[].
struct SNDMIDIPARTIAL {
    int shandle;                    // 0x0 [O] the sound handle
    signed char seqindex;           // 0x4 [O] the owning song (ssi index)
    signed char channel;            // 0x5 [O]
    signed char key;                // 0x6 [O]
    signed char velocity;           // 0x7 [O]
};

CHECK_OFFSET(SNDMIDIPARTIAL, shandle, 0x0);
CHECK_OFFSET(SNDMIDIPARTIAL, seqindex, 0x4);
CHECK_OFFSET(SNDMIDIPARTIAL, channel, 0x5);
CHECK_OFFSET(SNDMIDIPARTIAL, key, 0x6);
CHECK_OFFSET(SNDMIDIPARTIAL, velocity, 0x7);
CHECK_SIZE(SNDMIDIPARTIAL, 0x8);

// The player's state. [O] struct SNDMIDISTATE (0x7C4) and the global `SNDMIDISTATE sndms` (globals.tsv, smidaz.c);
// [R] retail's symbol sndms (0x0050A120), smp addressed as D_0050A820 (= sndms + 0x700).
struct SNDMIDISTATE {
    struct SNDSEQINSTANCE ssi[16];         // 0x000 [O]
    struct SNDMIDIPARTIAL smp[24];         // 0x700 [O]; [R] D_0050A820
    int lasthandle;                 // 0x7C0 [O]
};

CHECK_OFFSET(SNDMIDISTATE, ssi, 0x000);
CHECK_OFFSET(SNDMIDISTATE, smp, 0x700);
CHECK_OFFSET(SNDMIDISTATE, lasthandle, 0x7C0);
CHECK_SIZE(SNDMIDISTATE, 0x7C4);

extern struct SNDMIDISTATE sndms;                  // [O] smidaz.c; [R] 0x0050A120

#endif /* EALIB_SMIDI_H */
