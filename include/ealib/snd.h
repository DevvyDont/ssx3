#ifndef EALIB_SND_H
#define EALIB_SND_H

// EA SND's shared structures (the sound library linked into unit ealib/seg_2B4578 and called from sound/*): play options,
// sample attributes / format / description, the tag reader, the bank patch header, the per-channel record CHANPUB, the
// system options (SNDSYS_getopts / setopts), the mixer config and the sound-heap records. The file name is ours (the
// types span many EA source files: snd/source/library/cmn/*.c, mix/smixer.c, psp|ps2 snddrv.c).
//
// Names, member order and kinds: SSX On Tour's DWARF ([O], snd 9.06.03-psp). Retail links an older SND: where its
// layout differs from On Tour's (CHANPUB lacks pFxVolume, SNDSYSSET has one more word before stealequalpriorityvoices,
// SNDSYSCAP keeps its render-mode count at 0x13, MIXCONFIG keeps chanovercallback last, SNDMEMREC has no debug id) the
// members are placed by retail use ([R] allocation / copy sizes, [U] accesses) and named only where the position and
// the use agree with On Tour's member. Views retired by this header: views2 group D (ealib/seg_2B4578).
//
// C structs (the EA library units are C): C units name the types without the struct keyword.

#include "layout.h"

#ifndef __cplusplus
typedef struct SNDPLAYOPTS SNDPLAYOPTS;
typedef struct SNDSAMPLEFORMAT SNDSAMPLEFORMAT;
typedef struct SNDSAMPLEDESC SNDSAMPLEDESC;
typedef struct SNDSAMPLEATTR SNDSAMPLEATTR;
typedef struct SNDTAGINFO SNDTAGINFO;
typedef struct TAGGEDPATCH TAGGEDPATCH;
typedef struct SNDIPATCHHEADER SNDIPATCHHEADER;
typedef struct ENVELOPE ENVELOPE;
typedef struct CHANPUB CHANPUB;
typedef struct SNDSYSCAP SNDSYSCAP;
typedef struct SNDSYSSET SNDSYSSET;
typedef struct SNDSYSVEC SNDSYSVEC;
typedef struct SNDSYSOPTS SNDSYSOPTS;
typedef struct MIXCONFIG MIXCONFIG;
typedef struct SNDMEMREC SNDMEMREC;
typedef struct SNDMEMSTATE SNDMEMSTATE;
typedef struct SNDIPACKETCALLBACK SNDIPACKETCALLBACK;
typedef struct SNDREQUESTSTATUS SNDREQUESTSTATUS;
#endif

// SNDPLAYOPTS (0x18 [O]): how to play a sound (SNDBANK_play(int bhandle, int patnum, SNDPLAYOPTS* pspo) [O]). Moved here
// from sound/bankmonitor.h so the C library unit can use it. [U] retail SNDplaysetdef stores On Tour's defaults in
// every member: vol / drylevel / fxlevel0 / velocity 0x7F, bend 0x40, keynum 0x3C, pitchmult / timemult / tempomult
// 0x1000, lowpasscutoff 0xFFFF, the rest 0.
struct SNDPLAYOPTS {
    signed char vol;               // 0x00 [O]; [U] SNDplaysetdef 0x7F; BANKMONITOR/MUSIC code seeds mBaseVol[mInst] from it
    signed char bend;              // 0x01 [O]; [U] SNDplaysetdef 0x40; SNDPKTPLAY_start -> CHANPUB::pitchbend
    signed char keynum;            // 0x02 [O]; [U] SNDplaysetdef 0x3C
    signed char velocity;          // 0x03 [O]; [U] SNDplaysetdef 0x7F
    signed char drylevel;          // 0x04 [O]; [U] SNDplaysetdef 0x7F; SNDPKTPLAY_start -> CHANPUB::drylevel
    signed char fxlevel0;          // 0x05 [O]; [U] SNDplaysetdef 0x7F; SNDPKTPLAY_start -> CHANPUB::unk_61[0]
    char pad[2];                   // 0x06 [O]
    unsigned short azimuth;        // 0x08 [O]; [U] -> CHANPUB::azimuth
    short elevation;               // 0x0A [O]
    unsigned short pitchmult;      // 0x0C [O]; [U] SNDplaysetdef 0x1000; -> CHANPUB::progpitch
    unsigned short timemult;       // 0x0E [O]; [U] SNDplaysetdef 0x1000; passed to SNDPLATFORM_playtimbre
    unsigned short tempomult;      // 0x10 [O]; [U] SNDplaysetdef 0x1000
    unsigned short pad2;           // 0x12 [O] (a pad there); [U] retail SNDplaysetdef clears it and SNDBANKI_playtimbre /
                                   //      SNDPKTPLAY_start pass it to SNDPLATFORM_playtimbre between timemult and
                                   //      lowpasscutoff (On Tour's playtimbre has no such argument)
    unsigned short lowpasscutoff;  // 0x14 [O]; [U] SNDplaysetdef 0xFFFF
    unsigned short highpasscutoff; // 0x16 [O]; [U] SNDplaysetdef 0
};

CHECK_OFFSET(SNDPLAYOPTS, vol, 0x00);
CHECK_OFFSET(SNDPLAYOPTS, bend, 0x01);
CHECK_OFFSET(SNDPLAYOPTS, keynum, 0x02);
CHECK_OFFSET(SNDPLAYOPTS, velocity, 0x03);
CHECK_OFFSET(SNDPLAYOPTS, drylevel, 0x04);
CHECK_OFFSET(SNDPLAYOPTS, fxlevel0, 0x05);
CHECK_OFFSET(SNDPLAYOPTS, pad, 0x06);
CHECK_OFFSET(SNDPLAYOPTS, azimuth, 0x08);
CHECK_OFFSET(SNDPLAYOPTS, elevation, 0x0A);
CHECK_OFFSET(SNDPLAYOPTS, pitchmult, 0x0C);
CHECK_OFFSET(SNDPLAYOPTS, timemult, 0x0E);
CHECK_OFFSET(SNDPLAYOPTS, tempomult, 0x10);
CHECK_OFFSET(SNDPLAYOPTS, pad2, 0x12);
CHECK_OFFSET(SNDPLAYOPTS, lowpasscutoff, 0x14);
CHECK_OFFSET(SNDPLAYOPTS, highpasscutoff, 0x16);
CHECK_SIZE(SNDPLAYOPTS, 0x18);

// SNDSAMPLEFORMAT (0x4 [O]): SNDI_patchtohdr(void* pbank, TAGGEDPATCH* ptp, SNDSAMPLEFORMAT* pssf, SNDSAMPLEATTR* pssa,
// SNDSAMPLEDESC* pssd, unsigned char* isgeneric) [O]; [U] retail SNDI_patchtohdr writes rate / channel count / codec
// (view Hdr_B820).
struct SNDSAMPLEFORMAT {
    unsigned short samplerate;     // 0x0 [O]; [U]
    unsigned char channels;        // 0x2 [O]; [U]
    unsigned char samplerep;       // 0x3 [O]; [U]
};

CHECK_OFFSET(SNDSAMPLEFORMAT, samplerate, 0x0);
CHECK_OFFSET(SNDSAMPLEFORMAT, channels, 0x2);
CHECK_OFFSET(SNDSAMPLEFORMAT, samplerep, 0x3);
CHECK_SIZE(SNDSAMPLEFORMAT, 0x4);

// SNDSAMPLEDESC (0x1C [O]): SNDI_patchtohdr's pssd; [U] retail fills the frame count and one data pointer per channel
// (view Extra_B820 {f0, offs[6]}).
struct SNDSAMPLEDESC {
    unsigned int totalframes;      // 0x00 [O]; [U]
    void* psamples[6];             // 0x04 [O]; [U] one per channel
};

CHECK_OFFSET(SNDSAMPLEDESC, totalframes, 0x00);
CHECK_OFFSET(SNDSAMPLEDESC, psamples, 0x04);
CHECK_SIZE(SNDSAMPLEDESC, 0x1C);

// SNDSAMPLEATTR (0x68 [O]): a sample's attributes. [O] SND_attrsetdef(SNDSAMPLEATTR* pssa) (sattrdef.c),
// SNDPKTPLAY_start(int, SNDSAMPLEFORMAT*, SNDSAMPLEATTR* pssa, SNDPLAYOPTS*), SNDI_patchtohdr's pssa; [U] retail
// SND_attrsetdef sets vol 0x7F, pan 0x40, rendermode 8 and clears every other member, in On Tour's order and widths;
// SNDPKTPLAY_start copies detune / vol / fxlevel0 / bendrange / azimuth[] into the CHANPUB.
struct SNDSAMPLEATTR {
    short detune;                  // 0x00 [O]; [U] attrsetdef 0; -> CHANPUB::initialdetune
    signed char priority;          // 0x02 [O]; [U] attrsetdef 0
    signed char vol;               // 0x03 [O]; [U] attrsetdef 0x7F; -> CHANPUB::builtinvol
    signed char pan;               // 0x04 [O]; [U] attrsetdef 0x40
    signed char fxlevel0;          // 0x05 [O]; [U] attrsetdef 0; -> CHANPUB::progfxlevel[0]
    signed char bendrange;         // 0x06 [O]; [U] attrsetdef 0; -> CHANPUB::bendrange (x 100)
    unsigned char platformver;     // 0x07 [O]; [U] attrsetdef 0
    unsigned short rendermode;     // 0x08 [O]; [U] attrsetdef 8; SNDPKTPLAY_start -> SNDIPATCHHEADER::rendermode
    char padchar[2];               // 0x0A [O]
    unsigned short azimuth[6];     // 0x0C [O]; [U] attrsetdef 0; -> CHANPUB::azimuthOffsets[]
    void* ptsdata[6];              // 0x18 [O]; [U] attrsetdef 0
    int tsdatasize[6];             // 0x30 [O]
    void* puserdata[4];            // 0x48 [O]; [U] attrsetdef 0
    int userdatasize[4];           // 0x58 [O]; [U] attrsetdef 0
};

CHECK_OFFSET(SNDSAMPLEATTR, detune, 0x00);
CHECK_OFFSET(SNDSAMPLEATTR, priority, 0x02);
CHECK_OFFSET(SNDSAMPLEATTR, vol, 0x03);
CHECK_OFFSET(SNDSAMPLEATTR, pan, 0x04);
CHECK_OFFSET(SNDSAMPLEATTR, fxlevel0, 0x05);
CHECK_OFFSET(SNDSAMPLEATTR, bendrange, 0x06);
CHECK_OFFSET(SNDSAMPLEATTR, platformver, 0x07);
CHECK_OFFSET(SNDSAMPLEATTR, rendermode, 0x08);
CHECK_OFFSET(SNDSAMPLEATTR, padchar, 0x0A);
CHECK_OFFSET(SNDSAMPLEATTR, azimuth, 0x0C);
CHECK_OFFSET(SNDSAMPLEATTR, ptsdata, 0x18);
CHECK_OFFSET(SNDSAMPLEATTR, tsdatasize, 0x30);
CHECK_OFFSET(SNDSAMPLEATTR, puserdata, 0x48);
CHECK_OFFSET(SNDSAMPLEATTR, userdatasize, 0x58);
CHECK_SIZE(SNDSAMPLEATTR, 0x68);

// SNDTAGINFO (0x14 [O]): the tag reader's cursor. [O] SNDI_gettag(SNDTAGINFO* psti) (sgettag.c); [U] retail SNDI_gettag
// reads the tag byte at ptag, returns its type / value / data pointer / size in the next members (view S_7388
// {p, cur, r8, c, len}); SNDI_patchtohdr walks a patch with one (view Rec_B820).
struct SNDTAGINFO {
    unsigned char* ptag;           // 0x00 [O]; [U] advanced past each tag
    int tagtype;                   // 0x04 [O]; [U]
    int tagval;                    // 0x08 [O]; [U]
    void* tagptr;                  // 0x0C [O]; [U]
    unsigned int tagsize;          // 0x10 [O]; [U]
};

CHECK_OFFSET(SNDTAGINFO, ptag, 0x00);
CHECK_OFFSET(SNDTAGINFO, tagtype, 0x04);
CHECK_OFFSET(SNDTAGINFO, tagval, 0x08);
CHECK_OFFSET(SNDTAGINFO, tagptr, 0x0C);
CHECK_OFFSET(SNDTAGINFO, tagsize, 0x10);
CHECK_SIZE(SNDTAGINFO, 0x14);

// TAGGEDPATCH (0x8 [O]): a patch in a bank (BANKVER5::patch[]); [O] SNDBANKI_playpatch(void*, TAGGEDPATCH* ptp, ...).
struct TAGGEDPATCH {
    short id;                      // 0x0 [O]
    unsigned char platform;        // 0x2 [O]
    unsigned char flags;           // 0x3 [O]
    int hdrsize;                   // 0x4 [O]
};

CHECK_OFFSET(TAGGEDPATCH, id, 0x0);
CHECK_OFFSET(TAGGEDPATCH, platform, 0x2);
CHECK_OFFSET(TAGGEDPATCH, flags, 0x3);
CHECK_OFFSET(TAGGEDPATCH, hdrsize, 0x4);
CHECK_SIZE(TAGGEDPATCH, 0x8);

// SNDIPATCHHEADER (0xCC [O]): a patch parsed into playable form. [O] SNDBANKI_playtimbre(..., SNDIPATCHHEADER* pph, ...),
// SNDI_validrendermode(int*, SNDIPATCHHEADER* pph), SNDPLATFORM_playtimbre(SNDIPATCHHEADER* pph, ...); [U] retail
// SNDBANKI_playtimbre reads numchan 0x3 .. sustainend 0x60 and azimuth[] 0xBC with On Tour's widths (view D_B140),
// SNDBANKI_playpatch tests velmin / velmax / keymin / keymax 0x8..0xB (view Rec_B588), SNDI_validrendermode numchan and
// rendermode 0x18 (view E_6158). Retail's on-stack copies are 0xD0 bytes (views Rec_B588, SNDIPATCHHEADER_6760): the
// last word may be stack padding or a retail-only member (no access seen).
struct SNDIPATCHHEADER {
    short masterranddetune;        // 0x00 [O]
    signed char platformver;       // 0x02 [O]
    signed char numchan;           // 0x03 [O]; [U] playtimbre / validrendermode
    short detune;                  // 0x04 [O]; [U]
    short randdetune;              // 0x06 [O]; [U]
    signed char velmin;            // 0x08 [O]; [U] SNDBANKI_playpatch: velocity range
    signed char velmax;            // 0x09 [O]; [U]
    signed char keymin;            // 0x0A [O]; [U] SNDBANKI_playpatch: key range
    signed char keymax;            // 0x0B [O]; [U]
    signed char priority;          // 0x0C [O]; [U]
    signed char basekey;           // 0x0D [O]; [U]
    signed char releaseenvelope;   // 0x0E [O]; [U]
    signed char numenvelopes;      // 0x0F [O]; [U]
    signed char bendrange;         // 0x10 [O]; [U]
    signed char pan;               // 0x11 [O]; [U]
    signed char randpan;           // 0x12 [O]; [U]
    unsigned char samplerep;       // 0x13 [O]; [U]
    signed char vol;               // 0x14 [O]; [U]
    signed char randvol;           // 0x15 [O]; [U]
    signed char fxlevel0;          // 0x16 [O]; [U]
    signed char numuserdata;       // 0x17 [O]
    unsigned short rendermode;     // 0x18 [O]; [U] SNDI_validrendermode, SNDPKTPLAY_start
    signed char initialenvvol;     // 0x1A [O]; [U]
    unsigned char lfovollength;    // 0x1B [O]; [U]
    signed char lfovolrand;        // 0x1C [O]; [U]
    unsigned char lfopitchlength;  // 0x1D [O]; [U]
    unsigned char lfopitchrand;    // 0x1E [O]; [U]
    signed char panmult;           // 0x1F [O]
    void* pvoltable;               // 0x20 [O]; [U]
    void* pbendtable;              // 0x24 [O]; [U]
    void* penvelope;               // 0x28 [O]; [U] -> CHANPUB::paenvelope
    void* plfovol;                 // 0x2C [O]; [U]
    void* plfopitch;               // 0x30 [O]; [U]
    void* puserdata[4];            // 0x34 [O]
    int userdatasize[4];           // 0x44 [O]
    short lfopitchdepth;           // 0x54 [O]; [U]
    unsigned short samplerate;     // 0x56 [O]; [U]
    int totalframes;               // 0x58 [O]; [U]
    int sustainstart;              // 0x5C [O]
    int sustainend;                // 0x60 [O]; [U]
    int* paudioramaddr;            // 0x64 [O]
    int sampleoffset[6];           // 0x68 [O]
    int loopoffset[6];             // 0x80 [O]
    void* ptimestretchdata[6];     // 0x98 [O]
    unsigned short timestretchsize[6]; // 0xB0 [O]
    unsigned short azimuth[6];     // 0xBC [O]; [U] playtimbre -> CHANPUB::azimuthOffsets[]
    void* ptimbre;                 // 0xC8 [O]
};

CHECK_OFFSET(SNDIPATCHHEADER, masterranddetune, 0x00);
CHECK_OFFSET(SNDIPATCHHEADER, platformver, 0x02);
CHECK_OFFSET(SNDIPATCHHEADER, numchan, 0x03);
CHECK_OFFSET(SNDIPATCHHEADER, detune, 0x04);
CHECK_OFFSET(SNDIPATCHHEADER, randdetune, 0x06);
CHECK_OFFSET(SNDIPATCHHEADER, velmin, 0x08);
CHECK_OFFSET(SNDIPATCHHEADER, velmax, 0x09);
CHECK_OFFSET(SNDIPATCHHEADER, keymin, 0x0A);
CHECK_OFFSET(SNDIPATCHHEADER, keymax, 0x0B);
CHECK_OFFSET(SNDIPATCHHEADER, priority, 0x0C);
CHECK_OFFSET(SNDIPATCHHEADER, basekey, 0x0D);
CHECK_OFFSET(SNDIPATCHHEADER, releaseenvelope, 0x0E);
CHECK_OFFSET(SNDIPATCHHEADER, numenvelopes, 0x0F);
CHECK_OFFSET(SNDIPATCHHEADER, bendrange, 0x10);
CHECK_OFFSET(SNDIPATCHHEADER, pan, 0x11);
CHECK_OFFSET(SNDIPATCHHEADER, randpan, 0x12);
CHECK_OFFSET(SNDIPATCHHEADER, samplerep, 0x13);
CHECK_OFFSET(SNDIPATCHHEADER, vol, 0x14);
CHECK_OFFSET(SNDIPATCHHEADER, randvol, 0x15);
CHECK_OFFSET(SNDIPATCHHEADER, fxlevel0, 0x16);
CHECK_OFFSET(SNDIPATCHHEADER, numuserdata, 0x17);
CHECK_OFFSET(SNDIPATCHHEADER, rendermode, 0x18);
CHECK_OFFSET(SNDIPATCHHEADER, initialenvvol, 0x1A);
CHECK_OFFSET(SNDIPATCHHEADER, lfovollength, 0x1B);
CHECK_OFFSET(SNDIPATCHHEADER, lfovolrand, 0x1C);
CHECK_OFFSET(SNDIPATCHHEADER, lfopitchlength, 0x1D);
CHECK_OFFSET(SNDIPATCHHEADER, lfopitchrand, 0x1E);
CHECK_OFFSET(SNDIPATCHHEADER, panmult, 0x1F);
CHECK_OFFSET(SNDIPATCHHEADER, pvoltable, 0x20);
CHECK_OFFSET(SNDIPATCHHEADER, pbendtable, 0x24);
CHECK_OFFSET(SNDIPATCHHEADER, penvelope, 0x28);
CHECK_OFFSET(SNDIPATCHHEADER, plfovol, 0x2C);
CHECK_OFFSET(SNDIPATCHHEADER, plfopitch, 0x30);
CHECK_OFFSET(SNDIPATCHHEADER, puserdata, 0x34);
CHECK_OFFSET(SNDIPATCHHEADER, userdatasize, 0x44);
CHECK_OFFSET(SNDIPATCHHEADER, lfopitchdepth, 0x54);
CHECK_OFFSET(SNDIPATCHHEADER, samplerate, 0x56);
CHECK_OFFSET(SNDIPATCHHEADER, totalframes, 0x58);
CHECK_OFFSET(SNDIPATCHHEADER, sustainstart, 0x5C);
CHECK_OFFSET(SNDIPATCHHEADER, sustainend, 0x60);
CHECK_OFFSET(SNDIPATCHHEADER, paudioramaddr, 0x64);
CHECK_OFFSET(SNDIPATCHHEADER, sampleoffset, 0x68);
CHECK_OFFSET(SNDIPATCHHEADER, loopoffset, 0x80);
CHECK_OFFSET(SNDIPATCHHEADER, ptimestretchdata, 0x98);
CHECK_OFFSET(SNDIPATCHHEADER, timestretchsize, 0xB0);
CHECK_OFFSET(SNDIPATCHHEADER, azimuth, 0xBC);
CHECK_OFFSET(SNDIPATCHHEADER, ptimbre, 0xC8);
CHECK_SIZE(SNDIPATCHHEADER, 0xCC);

// ENVELOPE (0x8 [O]): one step of a channel's volume envelope (CHANPUB::paenvelope[i], i < numenvelopes).
struct ENVELOPE {
    int duration;                  // 0x0 [O]
    int targetvol;                 // 0x4 [O]
};

CHECK_OFFSET(ENVELOPE, duration, 0x0);
CHECK_OFFSET(ENVELOPE, targetvol, 0x4);
CHECK_SIZE(ENVELOPE, 0x8);

// CHANPUB, retail version (0x8C [R]; On Tour 0x90): one playing channel (a sound's public state). [O] sndgs.chan is
// CHANPUB* and SNDpitchbend / SNDautovol / SNDPLATFORM_lowpass keep CHANPUB* locals; [R] func_003B5158 allocates
// voicestotal * 140 bytes for sndgs + 0x1F0 and every user indexes it with a 0x8C stride. Up to drylevel (0x5F) retail
// matches On Tour's offsets; retail has no pFxVolume (On Tour 0x64), so every member from pitchbend on sits 4 bytes
// earlier than On Tour's ([U] SNDPKTPLAY_start: pspo->bend -> 0x64, pssa->detune -> 0x84, pssa->bendrange * 100 -> 0x82,
// pspo->pitchmult -> 0x88; SNDPLATFORM_setpitch writes finalpitch 0x8A; status 0x69 and timemult 0x6A are tested / set
// by the voice code). The per-bus effect bytes at 0x60..0x63 are retail's own (On Tour keeps them behind pFxVolume).
// Floats in On Tour's fade members are 16.16 ints in retail (SNDvol stores vol << 16; func_003B86C0 reads the high half).
struct CHANPUB {
    int handle;                    // 0x00 [O]; [U] SNDbankremove / SNDmastervol / SNDVOICEI_free
    short voices[6];               // 0x04 [O]; [U] the platform voices (the SNDPLATFORM_* loops, numchan entries)
    unsigned short patnum;         // 0x10 [O]
    short bhandle;                 // 0x12 [O]; [U] SNDbankinuse / SNDbankremove compare it with the bank handle
    unsigned int frames;           // 0x14 [O]
    int sustainend;                // 0x18 [O]
    unsigned short azimuth;        // 0x1C [O]; [U] SND3dpos; SNDPKTPLAY_start (pspo->azimuth)
    short elevation;               // 0x1E [O]; [U] SND3dpos
    unsigned short samplerate;     // 0x20 [O]; [U] SNDPLATFORM_setpitch
    unsigned char samplerep;       // 0x22 [O]
    unsigned char numchan;         // 0x23 [O]; [U] the voices[] count in every SNDPLATFORM_* loop
    unsigned short rendermode;     // 0x24 [O]; [U] flag tests (8, 0x100) pick the voice type
    unsigned char patchkey;        // 0x26 [O]; [U] SNDVOICEI_free
    unsigned char ismaster;        // 0x27 [O]; [U] SNDVOICEI_free
    short masterchan;              // 0x28 [O]; [U] SNDDRV_getsamplechan: -1 = its own master
    unsigned char priority;        // 0x2A [O]
    signed char builtinvol;        // 0x2B [O]; [U] SNDPKTPLAY_start (pssa->vol)
    unsigned int timestamp;        // 0x2C [O]; [U] SNDVOICEI_free
    int fadePer100Hz;              // 0x30 [O] (float there); [U] SNDvol clears it
    int fadeTargetVol;             // 0x34 [O] (float there)
    int programmedVol;             // 0x38 [O] (float there); [U] 16.16: SNDvol stores vol << 16, func_003B86C0 reads the
                                   //      high half (lh 0x3A)
    int envpertick;                // 0x3C [O]
    int envvol;                    // 0x40 [O]
    int envduration;               // 0x44 [O]
    float finalvol;                // 0x48 [O]; [U] SNDPLATFORM_setfxlevel / func_003C2A50
    unsigned short builtinazimuth; // 0x4C [O]; [U] SND3dpos
    unsigned char isLfe;           // 0x4E [O]
    char pad[1];                   // 0x4F [O]
    unsigned short azimuthOffsets[6]; // 0x50 [O]; [U] SNDPKTPLAY_start (pssa->azimuth[i])
    signed char numenvelopes;      // 0x5C [O]; [U] the envelope step count
    signed char curenvelope;       // 0x5D [O]
    signed char releaseenvelope;   // 0x5E [O]
    signed char drylevel;          // 0x5F [O]; [U] SNDPKTPLAY_start (pspo->drylevel), func_003B8700, func_003C2A50
    signed char progfxlevel[1];    // 0x60 [O] (a byte there); [U] SNDPKTPLAY_start stores pssa->fxlevel0 in [0]; indexed by
                                   //      effect bus in func_003BA740
    signed char unk_61[1];         // 0x61 [U] the send level per effect bus: SNDPKTPLAY_start stores pspo->fxlevel0 in
                                   //      [0], func_003B88C8 stores its level argument (0..127) (On Tour: pad1)
    short unk_62[1];               // 0x62 [U] the computed send per bus: func_003BA740 stores progfxlevel[bus] *
                                   //      unk_61[bus] * the bus level >> 6; SNDvol / SNDPLATFORM_setfxlevel read it
    signed char pitchbend;         // 0x64 [O] (0x68 there); [U] SNDPKTPLAY_start (pspo->bend)
    unsigned char vollfolength;    // 0x65 [O]
    unsigned char pitchlfolength;  // 0x66 [O]
    unsigned char curvollfoentry;  // 0x67 [O]
    unsigned char curpitchlfoentry; // 0x68 [O]
    signed char status;            // 0x69 [O]; [U] 1 = playing (SNDSYS_service, func_003C0E28, SNDVOICEI_free)
    unsigned short timemult;       // 0x6A [O]; [U] SNDCTRL_timemult stores it
    ENVELOPE* paenvelope;          // 0x6C [O]; [U] SNDBANKI_playtimbre (pph->penvelope)
    signed char* pvoltable;        // 0x70 [O] (SCALINGTABLE* {signed char xlate[128]})
    signed char* pbendtable;       // 0x74 [O] (SCALINGTABLE*)
    signed char* pvollfo;          // 0x78 [O] (SCALINGTABLE*); [U] stepped by the 100 Hz server
    signed char* ppitchlfo;        // 0x7C [O] (SCALINGTABLE*); [U] stepped by the 100 Hz server
    short pitchlfodepth;           // 0x80 [O]
    short bendrange;               // 0x82 [O]; [U] SNDPKTPLAY_start (pssa->bendrange * 100)
    short initialdetune;           // 0x84 [O]; [U] SNDPKTPLAY_start (pssa->detune)
    unsigned short detunepitch;    // 0x86 [O]
    unsigned short progpitch;      // 0x88 [O]; [U] SNDPKTPLAY_start (pspo->pitchmult)
    unsigned short finalpitch;     // 0x8A [O]; [U] SNDPLATFORM_setpitch
};

CHECK_OFFSET(CHANPUB, handle, 0x00);
CHECK_OFFSET(CHANPUB, voices, 0x04);
CHECK_OFFSET(CHANPUB, patnum, 0x10);
CHECK_OFFSET(CHANPUB, bhandle, 0x12);
CHECK_OFFSET(CHANPUB, frames, 0x14);
CHECK_OFFSET(CHANPUB, sustainend, 0x18);
CHECK_OFFSET(CHANPUB, azimuth, 0x1C);
CHECK_OFFSET(CHANPUB, elevation, 0x1E);
CHECK_OFFSET(CHANPUB, samplerate, 0x20);
CHECK_OFFSET(CHANPUB, samplerep, 0x22);
CHECK_OFFSET(CHANPUB, numchan, 0x23);
CHECK_OFFSET(CHANPUB, rendermode, 0x24);
CHECK_OFFSET(CHANPUB, patchkey, 0x26);
CHECK_OFFSET(CHANPUB, ismaster, 0x27);
CHECK_OFFSET(CHANPUB, masterchan, 0x28);
CHECK_OFFSET(CHANPUB, priority, 0x2A);
CHECK_OFFSET(CHANPUB, builtinvol, 0x2B);
CHECK_OFFSET(CHANPUB, timestamp, 0x2C);
CHECK_OFFSET(CHANPUB, fadePer100Hz, 0x30);
CHECK_OFFSET(CHANPUB, fadeTargetVol, 0x34);
CHECK_OFFSET(CHANPUB, programmedVol, 0x38);
CHECK_OFFSET(CHANPUB, envpertick, 0x3C);
CHECK_OFFSET(CHANPUB, envvol, 0x40);
CHECK_OFFSET(CHANPUB, envduration, 0x44);
CHECK_OFFSET(CHANPUB, finalvol, 0x48);
CHECK_OFFSET(CHANPUB, builtinazimuth, 0x4C);
CHECK_OFFSET(CHANPUB, isLfe, 0x4E);
CHECK_OFFSET(CHANPUB, pad, 0x4F);
CHECK_OFFSET(CHANPUB, azimuthOffsets, 0x50);
CHECK_OFFSET(CHANPUB, numenvelopes, 0x5C);
CHECK_OFFSET(CHANPUB, curenvelope, 0x5D);
CHECK_OFFSET(CHANPUB, releaseenvelope, 0x5E);
CHECK_OFFSET(CHANPUB, drylevel, 0x5F);
CHECK_OFFSET(CHANPUB, progfxlevel, 0x60);
CHECK_OFFSET(CHANPUB, unk_61, 0x61);
CHECK_OFFSET(CHANPUB, unk_62, 0x62);
CHECK_OFFSET(CHANPUB, pitchbend, 0x64);
CHECK_OFFSET(CHANPUB, vollfolength, 0x65);
CHECK_OFFSET(CHANPUB, pitchlfolength, 0x66);
CHECK_OFFSET(CHANPUB, curvollfoentry, 0x67);
CHECK_OFFSET(CHANPUB, curpitchlfoentry, 0x68);
CHECK_OFFSET(CHANPUB, status, 0x69);
CHECK_OFFSET(CHANPUB, timemult, 0x6A);
CHECK_OFFSET(CHANPUB, paenvelope, 0x6C);
CHECK_OFFSET(CHANPUB, pvoltable, 0x70);
CHECK_OFFSET(CHANPUB, pbendtable, 0x74);
CHECK_OFFSET(CHANPUB, pvollfo, 0x78);
CHECK_OFFSET(CHANPUB, ppitchlfo, 0x7C);
CHECK_OFFSET(CHANPUB, pitchlfodepth, 0x80);
CHECK_OFFSET(CHANPUB, bendrange, 0x82);
CHECK_OFFSET(CHANPUB, initialdetune, 0x84);
CHECK_OFFSET(CHANPUB, detunepitch, 0x86);
CHECK_OFFSET(CHANPUB, progpitch, 0x88);
CHECK_OFFSET(CHANPUB, finalpitch, 0x8A);
CHECK_SIZE(CHANPUB, 0x8C);

// SNDSYSCAP, retail version (0x20 [R]: SNDSYSOPTS::set follows at 0x20; On Tour 0x1A): what the platform supports.
// [U] func_003C0C90 (SNDSYS_getopts' first-use defaults) fills it: rates 24000..48000, output channels 1..5, voice
// maxima 32 / 16 / 48, six render modes; func_003C0E28 clamps the requested SNDSYSSET by it. Retail keeps the
// render-mode count at 0x13 next to its table (On Tour: numrendermodes 0xC, rendermode 0xE).
struct SNDSYSCAP {
    unsigned short outputratemin;  // 0x00 [O]; [U] 24000
    unsigned short outputratemax;  // 0x02 [O]; [U] 48000
    unsigned char outputchannelsmin; // 0x04 [O]; [U] 1; func_003C0E28's lower bound for outputchannels
    unsigned char outputchannelsmax; // 0x05 [O]; [U] 5; the upper bound; func_003C54C0 (>= 2: stereo capable)
    unsigned char voicesmaincpumax; // 0x06 [O]; [U] 32; bounds voicesmaincpu
    unsigned char voicesiopcpumax; // 0x07 [O]; [U] 16; bounds voicesiopcpu
    unsigned char voicesspumax;    // 0x08 [O]; [U] 48; bounds voicesspu
    unsigned char voicesds2dhwmax; // 0x09 [O]
    unsigned char voicesdspmax;    // 0x0A [O]
    unsigned char eax;             // 0x0B [O]
    unsigned char unk_0C;          // 0x0C [U] func_003C0C90 stores 0 (On Tour has numrendermodes here)
    char unk_0D[6];                // 0x0D
    unsigned char numrendermodes;  // 0x13 [O]; [U] func_003C0C90: 6 (the entries it writes below)
    unsigned short rendermode[6];  // 0x14 [O]; [U] func_003C0C90: 40, 36, 288, 72, 68, 320
};

CHECK_OFFSET(SNDSYSCAP, outputratemin, 0x00);
CHECK_OFFSET(SNDSYSCAP, outputratemax, 0x02);
CHECK_OFFSET(SNDSYSCAP, outputchannelsmin, 0x04);
CHECK_OFFSET(SNDSYSCAP, outputchannelsmax, 0x05);
CHECK_OFFSET(SNDSYSCAP, voicesmaincpumax, 0x06);
CHECK_OFFSET(SNDSYSCAP, voicesiopcpumax, 0x07);
CHECK_OFFSET(SNDSYSCAP, voicesspumax, 0x08);
CHECK_OFFSET(SNDSYSCAP, voicesds2dhwmax, 0x09);
CHECK_OFFSET(SNDSYSCAP, voicesdspmax, 0x0A);
CHECK_OFFSET(SNDSYSCAP, eax, 0x0B);
CHECK_OFFSET(SNDSYSCAP, unk_0C, 0x0C);
CHECK_OFFSET(SNDSYSCAP, unk_0D, 0x0D);
CHECK_OFFSET(SNDSYSCAP, numrendermodes, 0x13);
CHECK_OFFSET(SNDSYSCAP, rendermode, 0x14);
CHECK_SIZE(SNDSYSCAP, 0x20);

// SNDSYSSET, retail version (0xA0 [R]: SNDSYS_setopts copies 0xA0 bytes into sndgs + 0x20 and keeps a 0xA0 copy at
// sndgs + 0xD4; On Tour 0x9C): the requested settings. Up to updateperiod retail matches On Tour's offsets; retail has
// one more word at 0x18, so stealequalpriorityvoices .. virtualspkrcfg3d sit 4 bytes later than On Tour's ([U]
// loadiopmodules 0x1F gates the IOP module loads in SNDPLATFORM_init, which builds the path from iopmodulepath 0x4C;
// outputchannels 0x27 is clamped by SNDSYSCAP and outputlfe 0x28 follows it (set for 5 channels); fastramaddr 0x44 is
// rounded up to 16 with fastramsize 0x48 reduced by 16; virtualspkrcfg3d 0x58 row n holds n + 1 speaker angles).
struct SNDSYSSET {
    int packetbufsize;             // 0x00 [O]
    unsigned int randomseed;       // 0x04 [O]; [U] func_003B5158 passes it to SNDI_randomseed; func_003C53D8 sets it
    unsigned short maxbanks;       // 0x08 [O]; [U] SNDSYS_getopts default 16; func_003B5158 allocates 16 bytes per bank
                                   //      slot; func_003C52F8 sets it
    unsigned short outputrate;     // 0x0A [O]; [U] func_003C0E28 rounds it to 24000 / 36000 / 48000; func_003C5788
    unsigned short outputrateiopcpu; // 0x0C [O]; [U] rounded the same way; func_003C5788
    unsigned char voicesmaincpu;   // 0x0E [O]; [U] bounded by voicesmaincpumax; SNDPLATFORM_init: MIXCONFIG::inputchannels
    unsigned char voicesiopcpu;    // 0x0F [O]; [U] bounded by voicesiopcpumax; func_003C5C30 (IOP render modes)
    unsigned char voicesspu;       // 0x10 [O]; [U] bounded by voicesspumax (the three sum to sndgs' voice total)
    unsigned char voicesds2dhw;    // 0x11 [O]
    unsigned char voicesdsp;       // 0x12 [O]
    char compatibility;            // 0x13 [O]
    float updateperiod;            // 0x14 [O]; [U] SNDSYS_getopts default -1.0f; func_003C5410 stores 1000 / rate
    int unk_18;                    // 0x18 [R] the retail-only word (every later member is On Tour's + 4)
    unsigned char stealequalpriorityvoices; // 0x1C [O]; [U] SNDSYS_getopts default 1; func_003C53A0 sets it
    unsigned char usesse;          // 0x1D [O]
    unsigned char useeax;          // 0x1E [O]; [U] func_003C0C90 default 0
    unsigned char loadiopmodules;  // 0x1F [O]; [U] SNDPLATFORM_init loads the IOP modules when set; default 1
    unsigned char resamplequality; // 0x20 [O]; [U] default 25; func_003C0E28 passes it to func_003C8148
    unsigned char iopcpuresamplequality; // 0x21 [O]; [U] default 25; copied into the IOP driver's parameters
    unsigned char routemaincpufxtoiopcpu; // 0x22 [O]; [U] default 0
    unsigned char routespufxtoiopcpu; // 0x23 [O]
    unsigned char emulationsubtype; // 0x24 [O]
    unsigned char initaram;        // 0x25 [O]
    unsigned char maxstreams;      // 0x26 [O]; [U] default 8, bounded by 32; func_003C5368 sets it
    unsigned char outputchannels;  // 0x27 [O]; [U] default 2; bounded by SNDSYSCAP; func_003C5C30 sets 1 / 2 / 4 / 5
    unsigned char outputlfe;       // 0x28 [O]; [U] 1 exactly when outputchannels == 5
    unsigned char outputspdifmode; // 0x29 [O]; [U] default 1; bits 1 / 2 pick the output path
    unsigned char dtsquality;      // 0x2A [O]; [U] bounded by 100; func_003C5B30 stores value * 100
    unsigned char sndheapthreshold; // 0x2B [O]; [U] SNDSYS_getopts default 90
    unsigned char numrendermodes;  // 0x2C [O]; [U] default 3; SNDPLATFORM_init / func_003C5C30
    unsigned char useASIOdriver;   // 0x2D [O]
    unsigned short rendermode[6];  // 0x2E [O]; [U] default 40, 36, 288; func_003C5C30 rebuilds the list
    char unk_3A[2];                // 0x3A alignment
    unsigned int arampooladdr;     // 0x3C [O]
    int arampoolsize;              // 0x40 [O]
    void* fastramaddr;             // 0x44 [O]; [U] SNDPLATFORM_init / func_003C0E28 (rounded up to 16)
    int fastramsize;               // 0x48 [O]; [U] SNDPLATFORM_init; func_003C0E28 (reduced by 16)
    char* iopmodulepath;           // 0x4C [O]; [U] func_003C0C90 default D_0044DD3C; SNDPLATFORM_init's module path
    char* ASIOconfig;              // 0x50 [O]
    void* snd2asio;                // 0x54 [O]
    unsigned short virtualspkrcfg3d[6][6]; // 0x58 [O]; [U] func_003C0C90's default speaker angles (row 1: 0x4000, 0xC000)
};

CHECK_OFFSET(SNDSYSSET, packetbufsize, 0x00);
CHECK_OFFSET(SNDSYSSET, randomseed, 0x04);
CHECK_OFFSET(SNDSYSSET, maxbanks, 0x08);
CHECK_OFFSET(SNDSYSSET, outputrate, 0x0A);
CHECK_OFFSET(SNDSYSSET, outputrateiopcpu, 0x0C);
CHECK_OFFSET(SNDSYSSET, voicesmaincpu, 0x0E);
CHECK_OFFSET(SNDSYSSET, voicesiopcpu, 0x0F);
CHECK_OFFSET(SNDSYSSET, voicesspu, 0x10);
CHECK_OFFSET(SNDSYSSET, voicesds2dhw, 0x11);
CHECK_OFFSET(SNDSYSSET, voicesdsp, 0x12);
CHECK_OFFSET(SNDSYSSET, compatibility, 0x13);
CHECK_OFFSET(SNDSYSSET, updateperiod, 0x14);
CHECK_OFFSET(SNDSYSSET, unk_18, 0x18);
CHECK_OFFSET(SNDSYSSET, stealequalpriorityvoices, 0x1C);
CHECK_OFFSET(SNDSYSSET, usesse, 0x1D);
CHECK_OFFSET(SNDSYSSET, useeax, 0x1E);
CHECK_OFFSET(SNDSYSSET, loadiopmodules, 0x1F);
CHECK_OFFSET(SNDSYSSET, resamplequality, 0x20);
CHECK_OFFSET(SNDSYSSET, iopcpuresamplequality, 0x21);
CHECK_OFFSET(SNDSYSSET, routemaincpufxtoiopcpu, 0x22);
CHECK_OFFSET(SNDSYSSET, routespufxtoiopcpu, 0x23);
CHECK_OFFSET(SNDSYSSET, emulationsubtype, 0x24);
CHECK_OFFSET(SNDSYSSET, initaram, 0x25);
CHECK_OFFSET(SNDSYSSET, maxstreams, 0x26);
CHECK_OFFSET(SNDSYSSET, outputchannels, 0x27);
CHECK_OFFSET(SNDSYSSET, outputlfe, 0x28);
CHECK_OFFSET(SNDSYSSET, outputspdifmode, 0x29);
CHECK_OFFSET(SNDSYSSET, dtsquality, 0x2A);
CHECK_OFFSET(SNDSYSSET, sndheapthreshold, 0x2B);
CHECK_OFFSET(SNDSYSSET, numrendermodes, 0x2C);
CHECK_OFFSET(SNDSYSSET, useASIOdriver, 0x2D);
CHECK_OFFSET(SNDSYSSET, rendermode, 0x2E);
CHECK_OFFSET(SNDSYSSET, unk_3A, 0x3A);
CHECK_OFFSET(SNDSYSSET, arampooladdr, 0x3C);
CHECK_OFFSET(SNDSYSSET, arampoolsize, 0x40);
CHECK_OFFSET(SNDSYSSET, fastramaddr, 0x44);
CHECK_OFFSET(SNDSYSSET, fastramsize, 0x48);
CHECK_OFFSET(SNDSYSSET, iopmodulepath, 0x4C);
CHECK_OFFSET(SNDSYSSET, ASIOconfig, 0x50);
CHECK_OFFSET(SNDSYSSET, snd2asio, 0x54);
CHECK_OFFSET(SNDSYSSET, virtualspkrcfg3d, 0x58);
CHECK_SIZE(SNDSYSSET, 0xA0);

// SNDSYSVEC (0x14 [O]): the system's callbacks. [U] func_003B5478 (the REAL glue) installs a do-nothing function as
// abortmsg (+0x8, D_0050A9B0); System_SetAbortHandler sets abortmsg through the options (opts + 0xC8); func_003C5FD0
// sets profileenter / profileleave (opts + 0xCC / + 0xD0).
struct SNDSYSVEC {
    void* aramalloc;               // 0x00 [O] (`unsigned int (*)()`)
    void* aramfree;                // 0x04 [O] (`void (*)()`)
    void (*abortmsg)(void);        // 0x08 [O]; [U] see above
    void* profileenter;            // 0x0C [O] (`void (*)()`); [U] func_003C5FD0
    void* profileleave;            // 0x10 [O] (`void (*)()`); [U] func_003C5FD0
};

CHECK_OFFSET(SNDSYSVEC, aramalloc, 0x00);
CHECK_OFFSET(SNDSYSVEC, aramfree, 0x04);
CHECK_OFFSET(SNDSYSVEC, abortmsg, 0x08);
CHECK_OFFSET(SNDSYSVEC, profileenter, 0x0C);
CHECK_OFFSET(SNDSYSVEC, profileleave, 0x10);
CHECK_SIZE(SNDSYSVEC, 0x14);

// SNDSYSOPTS, retail version (0xD4 [R]: SNDSYS_getopts copies 0xD4 bytes out of sndgs; On Tour 0xCC). [O]
// SNDSYS_getopts(SNDSYSOPTS* psso) / SNDSYS_setopts(SNDSYSOPTS* psso) (ssysinit.c); [U] SNDSYS_setopts copies set
// (0x20, 0xA0 bytes) and vec (0xC0, 0x14 bytes) into sndgs; the option setters in ealib/seg_2B4578 (func_003C52F8 ..
// func_003C5FD0) and sound/soundsys get the block, change one member and set it again (they size the buffer 0xE0).
struct SNDSYSOPTS {
    struct SNDSYSCAP cap;          // 0x00 [O]
    struct SNDSYSSET set;          // 0x20 [O]
    struct SNDSYSVEC vec;          // 0xC0 [O]
};

CHECK_OFFSET(SNDSYSOPTS, cap, 0x00);
CHECK_OFFSET(SNDSYSOPTS, set, 0x20);
CHECK_OFFSET(SNDSYSOPTS, vec, 0xC0);
CHECK_SIZE(SNDSYSOPTS, 0xD4);

// MIXCONFIG, retail version (0xC [O] size): the software mixer's setup. [O] MIX_create(MIXCONFIG* pmc) (smixer.c) and the
// member names; [U] retail SNDPLATFORM_init fills one on the stack (view L_1B80): outputrate = set.outputrate, a byte
// count of voices = set.voicesmaincpu, the output channel count = outputchannels + outputlfe, 0, and the channel-over
// callback last; MIX_create (view Cfg_7D00) copies the 0xC bytes into sndmix and allocates inputchannels * 0x60
// (SNDMIXVOICE) and outputchannels mix buffers. On Tour orders it outputrate, chanovercallback, inputchannels,
// outputchannels, outputinterleaved.
struct MIXCONFIG {
    int outputrate;                // 0x0 [O]; [U]
    unsigned char inputchannels;   // 0x4 [O]; [U] MIX_create: the SNDMIXVOICE count
    unsigned char outputchannels;  // 0x5 [O]; [U] MIX_create: the mix-buffer / FT16STATE count
    unsigned char outputinterleaved; // 0x6 [O]; [U] SNDPLATFORM_init 0
    char unk_7;                    // 0x7 padding
    void (*chanovercallback)(void); // 0x8 [O]; [U] SNDPLATFORM_init: func_003C0BE8
};

CHECK_OFFSET(MIXCONFIG, outputrate, 0x0);
CHECK_OFFSET(MIXCONFIG, inputchannels, 0x4);
CHECK_OFFSET(MIXCONFIG, outputchannels, 0x5);
CHECK_OFFSET(MIXCONFIG, outputinterleaved, 0x6);
CHECK_OFFSET(MIXCONFIG, unk_7, 0x7);
CHECK_OFFSET(MIXCONFIG, chanovercallback, 0x8);
CHECK_SIZE(MIXCONFIG, 0xC);

// SNDMEMREC (0x8 [R]; On Tour 0x28 in its debug build, which adds `char id[32]`; the DWARF lists 2 variants): one
// allocation of the sound heap. [O] smemman.c; [U] retail SNDMEMI_alloc (func_003B5C60, view SNDMEMREC_5C60) and
// SNDMEMI_free (view E_5E98) keep them sorted by addr, stored downward from the heap's end.
struct SNDMEMREC {
    unsigned int addr;             // 0x0 [O]; [U]
    int size;                      // 0x4 [O]; [U]
};

CHECK_OFFSET(SNDMEMREC, addr, 0x0);
// (size at 0x4: CHECK_OFFSET(SNDMEMREC, size, ...) would declare check_SNDMEMREC_size, the CHECK_SIZE typedef's name,
// which a C unit rejects as a redefinition.)
CHECK_SIZE(SNDMEMREC, 0x8);

// SNDMEMSTATE (0x18 [R]; On Tour 0x38 with the debug id): the sound heap. [O] SNDGLOBALSTATE::mm (sndgs + 0x234 in
// retail); [U] SNDMEMI_alloc / SNDMEMI_free (views SNDMEMSTATE_5C60, S_5E98): r grows down (indexed 0, -1, ...),
// nummallocs counts down, lowmark is the low-water mark.
struct SNDMEMSTATE {
    char* pheap;                   // 0x00 [O]; [U]
    struct SNDMEMREC* r;           // 0x04 [O]; [U]
    int heapsize;                  // 0x08 [O]; [U]
    unsigned int endaddr;          // 0x0C [O]; [U]
    int lowmark;                   // 0x10 [O]; [U]
    int nummallocs;                // 0x14 [O]; [U]
};

CHECK_OFFSET(SNDMEMSTATE, pheap, 0x00);
CHECK_OFFSET(SNDMEMSTATE, r, 0x04);
CHECK_OFFSET(SNDMEMSTATE, heapsize, 0x08);
CHECK_OFFSET(SNDMEMSTATE, endaddr, 0x0C);
CHECK_OFFSET(SNDMEMSTATE, lowmark, 0x10);
CHECK_OFFSET(SNDMEMSTATE, nummallocs, 0x14);
CHECK_SIZE(SNDMEMSTATE, 0x18);

// SNDIPACKETCALLBACK (0x8 [O]): a queued packet-player callback (SNDPACKETSTATE::spcb[96]). [U] retail
// SNDPKTPLAYI_flushcallbackdata (view Ev_7080 {u16 type; u16 id; int val}) reads type, the channel and the union word.
struct SNDIPACKETCALLBACK {
    unsigned short type;           // 0x0 [O]; [U]
    unsigned short chan;           // 0x2 [O]; [U]
    int u;                         // 0x4 [O] (an anonymous union there); [U] the callback's argument
};

CHECK_OFFSET(SNDIPACKETCALLBACK, type, 0x0);
CHECK_OFFSET(SNDIPACKETCALLBACK, chan, 0x2);
CHECK_OFFSET(SNDIPACKETCALLBACK, u, 0x4);
CHECK_SIZE(SNDIPACKETCALLBACK, 0x8);

// SNDREQUESTSTATUS (0x10 [O] sstrstat.c: int SNDSTRM_requeststatus(int sndrequesthandle, SNDREQUESTSTATUS* psrs)):
// a stream request's status. [R] retail SNDSTRM_requeststatus (ealib/seg_2B4578) clears and fills the four words in this
// order: the state (0 header not parsed, 1 queued, 2 playing, 3 finished), the position in ms, the time to the end in ms
// and the submitted time in ms; [U] the song instance's position getters (sound/bankmonitor func_002B2600 /
// func_002B2640) read state == 2, then currenttime / timetoend.
struct SNDREQUESTSTATUS {
    int state;                      // 0x0 [O]; [R]
    int currenttime;                // 0x4 [O]; [R] position * 1000 / sample rate
    int timetoend;                  // 0x8 [O]; [R] (end - position) * 1000 / rate
    int timebuffered;               // 0xC [O]; [R] submitted samples * 1000 / rate
};

CHECK_OFFSET(SNDREQUESTSTATUS, state, 0x0);
CHECK_OFFSET(SNDREQUESTSTATUS, currenttime, 0x4);
CHECK_OFFSET(SNDREQUESTSTATUS, timetoend, 0x8);
CHECK_OFFSET(SNDREQUESTSTATUS, timebuffered, 0xC);
CHECK_SIZE(SNDREQUESTSTATUS, 0x10);

#endif /* EALIB_SND_H */
