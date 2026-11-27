#ifndef EALIB_SND_H
#define EALIB_SND_H

// EA SND's shared structures (the sound library linked into unit ealib/seg_2B4578 and called from sound/*): play options,
// sample attributes / format / description, the tag reader, the bank patch header, the per-channel record CHANPUB, the
// system options (SNDSYS_getopts / setopts), the mixer config, the sound-heap records, the bank table entry, the global
// state sndgs (SNDGLOBALSTATE) and the packet player's channels (sndpps). The file name is ours (the
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
typedef struct SNDSTREAMSTATUS SNDSTREAMSTATUS;
typedef struct BANKLIST BANKLIST;
typedef struct SNDTIMERCLIENT SNDTIMERCLIENT;
typedef struct SNDUSERDATAMSG SNDUSERDATAMSG;
typedef struct SNDGLOBALSTATE SNDGLOBALSTATE;
typedef struct SNDPACKETENTRY SNDPACKETENTRY;
typedef struct SNDPACKETCHAN SNDPACKETCHAN;
typedef struct SNDPACKETSTATE SNDPACKETSTATE;
typedef struct SNDPACKET SNDPACKET;
#endif

// SNDPLAYOPTS (0x18 [O]): how to play a sound (SNDBANK_play(int bhandle, int patnum, SNDPLAYOPTS* pspo) [O]). Moved here
// from sound/bankmonitor.h so the C library unit can use it. [U] retail SNDplaysetdef stores On Tour's defaults in
// every member: vol / drylevel / fxlevel0 / velocity 0x7F, bend 0x40, keynum 0x3C, pitchmult / timemult / tempomult
// 0x1000, lowpasscutoff 0xFFFF, the rest 0.
// kSNDMaxVol (name ours): the loudest volume, SNDPLAYOPTS::vol's default ([U] SNDplaysetdef 0x7F; the bank monitor
// clamps every volume to 0..127, cBankMonitorInstance_Fade / _Update3D).
enum { kSNDMaxVol = 127 };
struct SNDPLAYOPTS {
    signed char vol;               // 0x00 [O]; [U] SNDplaysetdef 0x7F; BANKMONITOR/MUSIC code seeds mBaseVol[mInst] from it
    signed char bend;              // 0x01 [O]; [U] SNDplaysetdef 0x40; SNDPKTPLAY_start -> CHANPUB::pitchbend
    signed char keynum;            // 0x02 [O]; [U] SNDplaysetdef 0x3C
    signed char velocity;          // 0x03 [O]; [U] SNDplaysetdef 0x7F
    signed char drylevel;          // 0x04 [O]; [U] SNDplaysetdef 0x7F; SNDPKTPLAY_start -> CHANPUB::drylevel
    signed char fxlevel0;          // 0x05 [O]; [U] SNDplaysetdef 0x7F; SNDPKTPLAY_start -> CHANPUB::fxsendlevel[0]
    char pad[2];                   // 0x06 [O]
    unsigned short azimuth;        // 0x08 [O]; [U] -> CHANPUB::azimuth
    short elevation;               // 0x0A [O]
    unsigned short pitchmult;      // 0x0C [O]; [U] SNDplaysetdef 0x1000; -> CHANPUB::progpitch
    unsigned short timemult;       // 0x0E [O]; [U] SNDplaysetdef 0x1000; passed to SNDPLATFORM_playtimbre
    unsigned short tempomult;      // 0x10 [O]; [U] SNDplaysetdef 0x1000
    unsigned short filteramount;   // 0x12 (name ours; On Tour: pad2, a pad there); [U] retail SNDplaysetdef clears it
                                   //      and SNDBANKI_playtimbre / SNDPKTPLAY_start pass it to SNDPLATFORM_playtimbre
                                   //      between timemult and lowpasscutoff (its filterAmount, the clip level for
                                   //      func_003C30C8; On Tour's playtimbre has no such argument)
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
CHECK_OFFSET(SNDPLAYOPTS, filteramount, 0x12);
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
// (tag offsets first, made bank- or sample-relative at the end).
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
// rendermode 0x18 (view E_6158). Retail's on-stack copies are 0xD0 bytes (view Rec_B588; SNDPKTPLAY_start's ph): the
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
    signed char fxsendlevel[1];    // 0x61 (name ours; On Tour: pad1) [U] the send level per effect bus:
                                   //      SNDPKTPLAY_start stores pspo->fxlevel0 in [0], func_003B88C8 stores its level
                                   //      argument (0..127)
    short fxsend[1];               // 0x62 (name ours) [U] the computed send per bus: func_003BA740 stores
                                   //      progfxlevel[bus] * fxsendlevel[bus] * the bus level >> 6; SNDvol /
                                   //      SNDPLATFORM_setfxlevel read it
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
CHECK_OFFSET(CHANPUB, fxsendlevel, 0x61);
CHECK_OFFSET(CHANPUB, fxsend, 0x62);
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
// and the submitted time in ms; [U] the song instance's position getters (sound/bankmonitor cSongInstance_GetPlayPosition /
// cSongInstance_GetTimeToEnd) read state == 2, then currenttime / timetoend.
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

// SNDSTREAMSTATUS (0xC [O] sststat.c: int SNDSTRM_status(int sndstreamhandle, SNDSTREAMSTATUS* psss)): a stream's
// status. [R] retail SNDSTRM_status (ealib/seg_2B4578) fills the three words: the queued requests, the first one's
// handle and the buffered time in ms; [U] the movie player's audio object (ealib/seg_2AEDC0) reads currentrequest.
struct SNDSTREAMSTATUS {
    int outstandingrequests;        // 0x0 [O]; [R]
    int currentrequest;             // 0x4 [O]; [R] the request handle at the head of the queue
    int timebuffered;               // 0x8 [O]; [R]
};

CHECK_OFFSET(SNDSTREAMSTATUS, outstandingrequests, 0x0);
CHECK_OFFSET(SNDSTREAMSTATUS, currentrequest, 0x4);
CHECK_OFFSET(SNDSTREAMSTATUS, timebuffered, 0x8);
CHECK_SIZE(SNDSTREAMSTATUS, 0xC);

// SNDREQUESTSTATUS::state (names ours). [R] SNDSTRM_requeststatus: 0 until the request's header has been parsed (and
// for an invalid handle), 1 while it is queued behind the request at the head, 2 while it is the head request (the one
// playing), 3 once it has left the queue (finished). [U] cStreamInstance_Playing tests 2, STREAM_IsRequestedDequeued 3.
enum eSndRequestState {
    kSNDREQUEST_NotParsed = 0, kSNDREQUEST_Queued = 1, kSNDREQUEST_Playing = 2, kSNDREQUEST_Done = 3
};

// A MIDI sequence's state, SNDseqstatus's result (names ours). [R] SNDseqstatus returns the sequence's state byte: 2
// stopped, 3 playing (6 when a fade step is set), 5 paused (SNDseqpause moves 3 to 5); negative for an invalid handle.
// [U] cBankMonitorInstance_DonePlaying counts 2 as done.
enum eSndSeqStatus { kSNDSEQ_Stopped = 2, kSNDSEQ_Playing = 3, kSNDSEQ_Paused = 5, kSNDSEQ_Fading = 6 };

// SNDbankadd's results (names ours). [R] SNDbankadd returns 7 when it uploaded sample data to sound memory, so only
// the bank's header has to stay in main memory, 8 for a bank with nothing to upload (it plays from where it was
// loaded), -6 when sound memory runs out. [U] cBankInstance_SyncLoad keeps just the header for 7 and the whole file
// for 8; cAsyncSys_ASYNCSYS_Update does the same.
enum eSndBankAddResult { kSNDBANKADD_Uploaded = 7, kSNDBANKADD_Resident = 8 };

// EA::Snd's result codes, output modes and devices: [O] On Tour's DWARF enums EA::Snd::Result, OutputMode and Device
// (the enumerators' names and values; the tags are ours, since EA's live in namespace EA::Snd) and the System
// functions that use them (Result CapOutputMode(OutputMode mode, bool* psupported), Result CapVoices(Device device,
// int* pvoices), Result GetOutputModeName(OutputMode mode, const char** pmodename), ...). [R] retail's
// System_GetOutputModeName names modes 0..8 in On Tour's order ("Mono" .. "Dolby ProLogic II"); [U] System_CapVoices
// reads the main-CPU, IOP-CPU and SPU voice maxima for devices 0, 1 and 2.
enum eSndResult {
    RESULT_OK = 0, RESULT_ERR_FILEERROR = -1, RESULT_ERR_MEMERROR = -2, RESULT_ERR_INVPLAYERTYPE = -3,
    RESULT_ERR_NOTINIT = -4, RESULT_ERR_INVOPT = -5, RESULT_ERR_GENERAL = -6, RESULT_ERR_ALREADYINUSE = -7,
    RESULT_ERR_UNSUPPORTEDOPT = -8, RESULT_ERR_DRIVER = -9, RESULT_ERR_INVPARAM = -10, RESULT_ERR_INVDATAFORMAT = -11
};
enum eSndOutputMode {
    OUTPUTMODE_MONO = 0, OUTPUTMODE_STEREO = 1, OUTPUTMODE_4POINT0 = 2, OUTPUTMODE_5POINT1 = 3,
    OUTPUTMODE_DTS4POINT0 = 4, OUTPUTMODE_DTS5POINT1 = 5, OUTPUTMODE_DD5POINT1 = 6, OUTPUTMODE_PROLOGIC = 7,
    OUTPUTMODE_PROLOGIC2 = 8, OUTPUTMODE_MAX = 9
};
enum eSndDevice { DEVICE_MAIN = 0, DEVICE_IOP = 1, DEVICE_HARDWARE = 2, DEVICE_HARDWARE3D = 3, DEVICE_MAX = 4 };

// SNDSYSSET::outputspdifmode's output paths (bits; names ours). [U] System_Init sets 2 for the DTS output modes and 1
// for the others; SNDPLATFORM_outputset and the voice code test the two bits, and the SPU voice paths check for 2.
enum eSndSpdifMode { kSNDSPDIF_Pcm = 1, kSNDSPDIF_Dts = 2 };

// The tags of an EA stream file's chunks, their first four bytes read as a little-endian word (names ours). [U]
// SNDSTRMI_parsechunk sends "SCDl" chunks to SNDSTRMI_parsedata and "SCHl" chunks to SNDSTRMI_parseheader.
enum eSndStreamChunk { kSNDCHUNK_SCDl = 0x6C444353, kSNDCHUNK_SCHl = 0x6C484353 };

// The headers SNDMIDI_play skips before a sequence (names ours): [U] an 8-byte header starting "MD" (its first two
// bytes read as a little-endian halfword) or a 4-byte "MIDx" tag (a little-endian word).
enum eSndMidiTag { kSNDMIDI_TagMD = 0x444D, kSNDMIDI_TagMIDx = 0x7844494D };

// BANKLIST, retail version (0x10 [R]: SNDSYSI_init allocates maxbanks * 16 bytes for sndgs.banklist; On Tour 0x8): one
// slot of the loaded-bank table, indexed by the bank handle. [O] SNDGLOBALSTATE::banklist is BANKLIST* and BANKLIST is
// {BANKVER5* phdr; signed char locked; char pad[3];}; retail keeps two more words between them ([U] SNDbankadd, view
// Ent_6098 {hdr, buf1, buf2, busy}: the header pointer the SNDBANKI_* functions read, two buffers, the busy byte).
struct BANKLIST {
    struct BANKVER5* phdr;         // 0x0 [O]; [U] SNDbankadd stores the bank header; SNDBANKI_getppatch's bank
    void* iopcpudata;              // 0x4 (name ours) [U] SNDbankadd allocates it with SNDPLATFORM_memalloc(0x100,
                                   //      iopcpusize + 0x40) and copies the bank's IOP / CPU sample section into it;
                                   //      SNDbankremove frees it (kind 0x100); retail-only
    void* spudata;                 // 0x8 (name ours) [U] the same for the SPU section (kind 8, spusize + 0x30);
                                   //      retail-only
    signed char locked;            // 0xC [O] (0x4 there); [U] the slot's busy byte (sA8E8Slot::f0C)
    char pad[3];                   // 0xD [O]
};

CHECK_OFFSET(BANKLIST, phdr, 0x0);
CHECK_OFFSET(BANKLIST, iopcpudata, 0x4);
CHECK_OFFSET(BANKLIST, spudata, 0x8);
CHECK_OFFSET(BANKLIST, locked, 0xC);
CHECK_OFFSET(BANKLIST, pad, 0xD);
CHECK_SIZE(BANKLIST, 0x10);

// SNDTIMERCLIENT (0x10, name ours; retail-only): a node of sndgs.variabletimerclients. [U] SNDSYSI_variabletimerservice
// walks the list by next and calls func(arg) for each node when sndgs.updatetick has moved; nothing in retail adds a
// node (the list stays empty). The word at 0x4 is never read.
struct SNDTIMERCLIENT {
    struct SNDTIMERCLIENT* next;   // 0x0 [U]
    int unk_4;                     // 0x4
    void (*func)(int);             // 0x8 [U] called with arg
    int arg;                       // 0xC [U]
};

CHECK_OFFSET(SNDTIMERCLIENT, next, 0x0);
CHECK_OFFSET(SNDTIMERCLIENT, unk_4, 0x4);
CHECK_OFFSET(SNDTIMERCLIENT, func, 0x8);
CHECK_OFFSET(SNDTIMERCLIENT, arg, 0xC);
CHECK_SIZE(SNDTIMERCLIENT, 0x10);

// SNDUSERDATAMSG (0x20, name ours): what SND hands its user-data clients (sndgs.userdataclient[i](&msg)). [U] both
// builders keep a 0x20-byte local: SNDBANKI_userdatacallback fills type (its argument: 2 from SNDbankremove), the
// block and its size from a parsed patch's puserdata[] / userdatasize[]; SNDSTRMI_parseheader fills type 3, a stream
// header's block and size and the request handle; the game's client cSSXAudio_HandleUserChunk (sound/ssxAudio) acts
// on type 3. The words at 0xC and 0x14..0x1F are never written.
// SNDUSERDATAMSG::type, what the block came with (names ours). [U] SNDBANKI_userdatacallback's callers pass 0
// (func_003B6788, which SNDbankadd calls once it resolved a patch's samples), 1 (SNDBANKI_playpatch, a timbre started)
// and 2 (SNDbankremove); SNDSTRMI_parseheader sends 3 for a stream header.
enum eSndUserDataType {
    kSNDUSERDATA_BankAdd = 0, kSNDUSERDATA_Play = 1, kSNDUSERDATA_BankRemove = 2, kSNDUSERDATA_StreamHeader = 3
};
struct SNDUSERDATAMSG {
    int type;                      // 0x00 [U] eSndUserDataType: 2 bank removed, 3 stream header
    void* pdata;                   // 0x04 [U] the user-data block
    int size;                      // 0x08 [U] its size
    int unk_0C;                    // 0x0C
    int handle;                    // 0x10 [U] SNDSTRMI_parseheader: the stream request's handle
    int unk_14[3];                 // 0x14
};

CHECK_OFFSET(SNDUSERDATAMSG, type, 0x00);
CHECK_OFFSET(SNDUSERDATAMSG, pdata, 0x04);
// (size at 0x08: CHECK_OFFSET(SNDUSERDATAMSG, size, ...) would clash with the CHECK_SIZE typedef in a C unit.)
CHECK_OFFSET(SNDUSERDATAMSG, unk_0C, 0x0C);
CHECK_OFFSET(SNDUSERDATAMSG, handle, 0x10);
CHECK_OFFSET(SNDUSERDATAMSG, unk_14, 0x14);
CHECK_SIZE(SNDUSERDATAMSG, 0x20);

// SNDGLOBALSTATE, retail version (0x338 [R]: sndgs 0x0050A8E8 .. the next object D_0050AC20, a separate voice table;
// On Tour 0x240): the sound system's global state, sndgs [O] (On Tour `SNDGLOBALSTATE sndgs`). [O] member names, order
// and kinds; retail places them later than On Tour because its SNDSYSOPTS / SNDSYSSET are bigger (0xD4 / 0xA0, see
// above) and it keeps retail-only blocks (the master volume byte, a variable-rate timer client list, three effect-bus
// records, the speaker layout tables). [R] SNDSYS_getopts copies sso out (0xD4 bytes), SNDSYS_setopts copies set into
// sso and keeps a 0xA0 copy in prevset (+0xD4); [U] the unit's views of sndgs (sA8E8 and the per-function G_* / S_*
// views in ealib/seg_2B4578) and the splat symbols inside it (D_0050A908 = sso.set, D_0050A9A8 = sso.vec, D_0050AA5C =
// installed, D_0050AA62 = cputypes, D_0050AAD0 = midirestore, D_0050AAD4 = streamrestore, D_0050AAD8 = chan,
// D_0050AADC = banklist, D_0050AAE0 = the first bus record, D_0050AB1C = mm, D_0050AB20 = the speaker tables,
// D_0050ABC8 = srcchancfg3d).
struct SNDGLOBALSTATE {
    struct SNDSYSOPTS sso;         // 0x000 [O]; [R] SNDSYS_getopts copies it out; SNDSYS_setopts writes set / vec
    struct SNDSYSSET prevset;      // 0x0D4 [O]; [R] SNDSYS_setopts keeps the 0xA0-byte copy here (sA8E8::saved)
    signed char installed;         // 0x174 [O]; [U] SNDSYSI_init returns at once when set and sets it; SNDSYS_restore
                                   //       tests and clears it (view G_5320::f174)
    signed char mastervol;         // 0x175 [R] SNDmastervol stores its argument (0..127) here and reapplies every
                                   //       channel's volume; SNDSYSI_init 0x7F. [O] On Tour's SNDmastervol keeps it in
                                   //       Snd::gMasterVol instead (smasterv.c), so On Tour has no member here
    volatile unsigned char incritical; // 0x176 [O] (volatile signed char at 0x169); [U] SNDSYS_entercritical /
                                   //       leavecritical count the nesting depth
    signed char numserverclients100hz; // 0x177 [O]; [U] SNDSYS_add100hzclient / remove100hzclient: the entries used in
                                   //       serverclient100hz[]; SNDSYSI_100hzserver calls them
    signed char numserverclients;  // 0x178 [O]; [U] iSNDserveraddclient / iSNDserverremoveclient: entries used in
                                   //       serverclient[]
    signed char numuserdataclients; // 0x179 [O]; [U] SNDSYS_adduserdataclient / removeuserdataclient: entries used in
                                   //       userdataclient[]
    unsigned char cputypes;        // 0x17A [O]; [U] passed as UNPACKINITPARAMS::cpu and to SFILTER_rsfinit
                                   //       (D_0050AA62)
    char unk_17B[3];               // 0x17B alignment
    short voicestotal;             // 0x17E [O]; [U] the CHANPUB count: SNDSYSI_init allocates voicestotal * 0x8C bytes
                                   //       for chan; every channel loop runs to it
    volatile unsigned int audiotick; // 0x180 [O]; [U] SNDSYSI_init 0; SNDSYSI_timerservice resets it on a new rate and
                                   //       runs SNDSYSI_100hzserver until it catches up
    void (*serverclient100hz[6])(void); // 0x184 [O]; [U] SNDSYS_add100hzclient appends, SNDSYSI_100hzserver calls
    void (*serverclient[6])(void); // 0x19C [O]; [U] iSNDserveraddclient appends
    struct SNDTIMERCLIENT* variabletimerclients; // 0x1B4 (name ours) [U] the nodes SNDSYSI_variabletimerservice
                                   //       calls when updatetick changes; retail-only
    int unk_1B8[2];                // 0x1B8
    int updatetick;                // 0x1C0 (name ours) [U] counts sound updates: the VBlank handler func_003C1980, or
                                   //       SNDDRV_thread when there is none; SNDSYSI_variabletimerservice compares it
    float unk_1C4;                 // 0x1C4 [U] SNDPLATFORM_init stores the update period in ms (16.68 / 20 / 10.67)
    void (*userdataclient[4])(void*); // 0x1C8 [O]; [U] SNDSYS_adduserdataclient appends
    void (*restorehook1)(void);    // 0x1D8 (names ours) [U] SNDSYS_restore calls each when set, first of all the
    void (*restorehook2)(void);    // 0x1DC     restore hooks; nothing in retail sets them. On Tour has four hooks
    void (*restorehook3)(void);    // 0x1E0     before bankremove (aemsrestore, aemsstopmodulebanks,
                                   //           aemsstreamrestore, eventrestore) where retail has these three
    void (*bankremove)(int);       // 0x1E4 [O]; [U] SNDbankadd installs SNDbankremove; SNDSYS_restore calls it with -1
    void (*midirestore)(void);     // 0x1E8 [O]; [U] SNDmidiinit installs SNDmidirestore, which clears it again
                                   //       (D_0050AAD0); SNDSYS_restore calls it
    void (*streamrestore)(void);   // 0x1EC [O]; [U] SNDSTRMI_create installs SNDSTRMI_destroyall (D_0050AAD4);
                                   //       SNDSYS_restore calls it
    struct CHANPUB* chan;          // 0x1F0 [O]; [U] voicestotal entries of 0x8C (D_0050AAD8)
    struct BANKLIST* banklist;     // 0x1F4 [O]; [U] maxbanks entries of 0x10, indexed by the bank handle (D_0050AADC)
    char unk_1F8[0x14];            // 0x1F8 [U] three 0x14-byte effect-bus records {short; char; int; int; float pan[2]}
    char unk_20C[0x14];            // 0x20C     for the render kinds 4 / 0x100 / 8 (func_003B8218 returns one,
    char unk_220[0x14];            // 0x220     func_003B82B8 fills it; the unit's view sA8E8Bus); retail-only, no EA name
    struct SNDMEMSTATE* mm;        // 0x234 [O]; [U] SNDMEMI_init points it at the heap (D_0050AB1C)
    char unk_238[0xA8];            // 0x238 [U] the speaker layout tables func_003C7480 restores and func_003C38E0 pans
                                   //       with (D_0050AB20); retail-only
    unsigned short srcchancfg3d[6][6]; // 0x2E0 [O]; [U] SNDSYSI_init fills row n - 1 with n channel angles (D_0050ABC8)
    void (*profileenter)(void);    // 0x328 [O]; [U] SNDDRV_thread calls it before a tick
    void (*profileleave)(void);    // 0x32C [O]; [U] SNDDRV_thread calls it after the tick
    volatile unsigned int servertick; // 0x330 [O]; [U] SNDSYSI_init 0; SNDSYSI_timerservice counts its calls (reset on
                                   //       a new rate); SNDDRV_thread publishes it to the IOP
    volatile unsigned short currenttimerhz; // 0x334 [O] (volatile short); [U] SNDSYSI_timerservice: the caller's rate
    char unk_336[2];               // 0x336 tail padding
};

CHECK_OFFSET(SNDGLOBALSTATE, sso, 0x000);
CHECK_OFFSET(SNDGLOBALSTATE, prevset, 0x0D4);
CHECK_OFFSET(SNDGLOBALSTATE, installed, 0x174);
CHECK_OFFSET(SNDGLOBALSTATE, mastervol, 0x175);
CHECK_OFFSET(SNDGLOBALSTATE, incritical, 0x176);
CHECK_OFFSET(SNDGLOBALSTATE, numserverclients100hz, 0x177);
CHECK_OFFSET(SNDGLOBALSTATE, numserverclients, 0x178);
CHECK_OFFSET(SNDGLOBALSTATE, numuserdataclients, 0x179);
CHECK_OFFSET(SNDGLOBALSTATE, cputypes, 0x17A);
CHECK_OFFSET(SNDGLOBALSTATE, unk_17B, 0x17B);
CHECK_OFFSET(SNDGLOBALSTATE, voicestotal, 0x17E);
CHECK_OFFSET(SNDGLOBALSTATE, audiotick, 0x180);
CHECK_OFFSET(SNDGLOBALSTATE, serverclient100hz, 0x184);
CHECK_OFFSET(SNDGLOBALSTATE, serverclient, 0x19C);
CHECK_OFFSET(SNDGLOBALSTATE, variabletimerclients, 0x1B4);
CHECK_OFFSET(SNDGLOBALSTATE, unk_1B8, 0x1B8);
CHECK_OFFSET(SNDGLOBALSTATE, updatetick, 0x1C0);
CHECK_OFFSET(SNDGLOBALSTATE, unk_1C4, 0x1C4);
CHECK_OFFSET(SNDGLOBALSTATE, userdataclient, 0x1C8);
CHECK_OFFSET(SNDGLOBALSTATE, restorehook1, 0x1D8);
CHECK_OFFSET(SNDGLOBALSTATE, restorehook2, 0x1DC);
CHECK_OFFSET(SNDGLOBALSTATE, restorehook3, 0x1E0);
CHECK_OFFSET(SNDGLOBALSTATE, bankremove, 0x1E4);
CHECK_OFFSET(SNDGLOBALSTATE, midirestore, 0x1E8);
CHECK_OFFSET(SNDGLOBALSTATE, streamrestore, 0x1EC);
CHECK_OFFSET(SNDGLOBALSTATE, chan, 0x1F0);
CHECK_OFFSET(SNDGLOBALSTATE, banklist, 0x1F4);
CHECK_OFFSET(SNDGLOBALSTATE, unk_1F8, 0x1F8);
CHECK_OFFSET(SNDGLOBALSTATE, unk_20C, 0x20C);
CHECK_OFFSET(SNDGLOBALSTATE, unk_220, 0x220);
CHECK_OFFSET(SNDGLOBALSTATE, mm, 0x234);
CHECK_OFFSET(SNDGLOBALSTATE, unk_238, 0x238);
CHECK_OFFSET(SNDGLOBALSTATE, srcchancfg3d, 0x2E0);
CHECK_OFFSET(SNDGLOBALSTATE, profileenter, 0x328);
CHECK_OFFSET(SNDGLOBALSTATE, profileleave, 0x32C);
CHECK_OFFSET(SNDGLOBALSTATE, servertick, 0x330);
CHECK_OFFSET(SNDGLOBALSTATE, currenttimerhz, 0x334);
CHECK_OFFSET(SNDGLOBALSTATE, unk_336, 0x336);
CHECK_SIZE(SNDGLOBALSTATE, 0x338);

extern struct SNDGLOBALSTATE sndgs; // 0x0050A8E8: the sound driver's state [O] (On Tour `SNDGLOBALSTATE sndgs`)

// SNDPACKETENTRY (0x20 [O]): one queued packet of a packet-player channel (SNDPACKETCHAN::pe[]). [R] SNDPKTPLAY_create
// sizes the entry table as (memsize - 0x9C) >> 5 more entries; [U] view E_6DE0 {int; int len:31; int flag:1; words[6]}
// has the same members in the same order.
struct SNDPACKETENTRY {
    int handle;                    // 0x00 [O]
    int numframes : 31;            // 0x04 [O]; [U] E_6DE0::len
    int continuation : 1;          // 0x04 [O]; [U] E_6DE0::flag
    void* psamples[6];             // 0x08 [O]; one per channel
};

CHECK_OFFSET(SNDPACKETENTRY, handle, 0x00);
CHECK_OFFSET(SNDPACKETENTRY, psamples, 0x08);
CHECK_SIZE(SNDPACKETENTRY, 0x20);

// SNDPACKET (0x24 [O]): a packet handed to SNDPKTPLAY_submit(int packetinstancehandle, SNDPACKET* psp) [O]; [U] retail
// SNDPKTPLAY_submit reads numframes, continuation and psamples[] (one per channel) into the channel's next
// SNDPACKETENTRY.
struct SNDPACKET {
    struct SNDSAMPLEFORMAT ssf;    // 0x00 [O]
    unsigned int numframes : 31;   // 0x04 [O]; [U] submit: the entry's numframes, added to totalframes
    unsigned int continuation : 1; // 0x04 [O]; [U] submit: the entry's continuation
    int sizeofsamples;             // 0x08 [O]
    void* psamples[6];             // 0x0C [O]; [U] submit: one sample pointer per channel
};

CHECK_OFFSET(SNDPACKET, ssf, 0x00);
CHECK_OFFSET(SNDPACKET, sizeofsamples, 0x08);
CHECK_OFFSET(SNDPACKET, psamples, 0x0C);
CHECK_SIZE(SNDPACKET, 0x24);

// SNDPACKETCHAN, retail version (0x9C with one entry [R]: SNDPKTPLAY_create sets maxpackets = (memsize - 156) >> 5; On
// Tour 0xA0): one packet-player channel (a stream's packet queue), sndpps.ppc[i]. [O] member names, order and volatile
// qualifiers; retail has no outstandingplatformframes (On Tour 0x50), so pplatformmem and every later member sit 4
// bytes lower: [R] SNDPKTPLAY_create(preleasefunc, pframesfunc, pclientdata, pmem, memsize) [O] stores pplatformmem
// 0x50, preleasefunc 0x54, pframesfunc 0x58, pclientdata 0x5C and shandle -1; [U] SNDPKTPLAYI_flushcallbackdata calls
// pframesfunc(chan, numframes, pclientdata) for a type-0 callback and preleasefunc(chan, pclientdata) otherwise (view
// Q_7080); the channel count of sampleformat (0x62) bounds the per-channel arrays (view Q_6DE0; SNDPKTPLAY_submit).
struct SNDPACKETCHAN {
    volatile int shandle;          // 0x00 [O]; [R] SNDPKTPLAY_create -1
    volatile int lastpackethandle; // 0x04 [O]
    volatile unsigned int masterReleasePacket; // 0x08 [O]
    volatile unsigned int masterCurrentPacket; // 0x0C [O]
    volatile unsigned int numPacketsProcessed[6]; // 0x10 [O]
    volatile short playpacket[6];  // 0x28 [O]
    volatile short outstandingpackets[6]; // 0x34 [O]
    volatile short maxpackets;     // 0x40 [O]; [R] SNDPKTPLAY_create
    volatile signed char masterpacketchan; // 0x42 [O]
    char pad;                      // 0x43 [O]
    volatile short releasepacket;  // 0x44 [O]
    volatile short submitpacket;   // 0x46 [O]
    volatile unsigned int totalframes; // 0x48 [O]
    volatile unsigned int outstandingframes; // 0x4C [O]
    void* pplatformmem;            // 0x50 [O] (0x54 there); [R] SNDPKTPLAY_create
    void (*preleasefunc)(int, int); // 0x54 [O] (0x58 there); [R] SNDPKTPLAY_create; [U] flushcallbackdata
    void (*pframesfunc)(int, int, int); // 0x58 [O] (0x5C there); [R] SNDPKTPLAY_create; [U] flushcallbackdata
    int pclientdata;               // 0x5C [O] (void* at 0x60 there); [R] SNDPKTPLAY_create; [U] passed to both callbacks
    struct SNDSAMPLEFORMAT sampleformat; // 0x60 [O]; [U] channels (0x62) bounds the per-channel loops
    void* ptsdata[6];              // 0x64 [O]
    struct SNDPACKETENTRY pe[1];   // 0x7C [O]; maxpackets entries
};

CHECK_OFFSET(SNDPACKETCHAN, shandle, 0x00);
CHECK_OFFSET(SNDPACKETCHAN, lastpackethandle, 0x04);
CHECK_OFFSET(SNDPACKETCHAN, masterReleasePacket, 0x08);
CHECK_OFFSET(SNDPACKETCHAN, masterCurrentPacket, 0x0C);
CHECK_OFFSET(SNDPACKETCHAN, numPacketsProcessed, 0x10);
CHECK_OFFSET(SNDPACKETCHAN, playpacket, 0x28);
CHECK_OFFSET(SNDPACKETCHAN, outstandingpackets, 0x34);
CHECK_OFFSET(SNDPACKETCHAN, maxpackets, 0x40);
CHECK_OFFSET(SNDPACKETCHAN, masterpacketchan, 0x42);
CHECK_OFFSET(SNDPACKETCHAN, pad, 0x43);
CHECK_OFFSET(SNDPACKETCHAN, releasepacket, 0x44);
CHECK_OFFSET(SNDPACKETCHAN, submitpacket, 0x46);
CHECK_OFFSET(SNDPACKETCHAN, totalframes, 0x48);
CHECK_OFFSET(SNDPACKETCHAN, outstandingframes, 0x4C);
CHECK_OFFSET(SNDPACKETCHAN, pplatformmem, 0x50);
CHECK_OFFSET(SNDPACKETCHAN, preleasefunc, 0x54);
CHECK_OFFSET(SNDPACKETCHAN, pframesfunc, 0x58);
CHECK_OFFSET(SNDPACKETCHAN, pclientdata, 0x5C);
CHECK_OFFSET(SNDPACKETCHAN, sampleformat, 0x60);
CHECK_OFFSET(SNDPACKETCHAN, ptsdata, 0x64);
CHECK_OFFSET(SNDPACKETCHAN, pe, 0x7C);
CHECK_SIZE(SNDPACKETCHAN, 0x9C);

// SNDPACKETSTATE (0x384 [O]): the packet player's state, sndpps [O] (0x005157B8; retail symbol size 8, a splat
// artefact). [U] SNDPKTPLAYI_flushcallbackdata walks spcb[0..callbackindex) and calls into ppc[chan] (view G_7080:
// {count, ev[0x60], q[]}, the same layout); SNDPKTPLAY_create takes the first free ppc[] slot.
struct SNDPACKETSTATE {
    int callbackindex;             // 0x000 [O]; [U] the queued callbacks
    struct SNDIPACKETCALLBACK spcb[96]; // 0x004 [O]
    struct SNDPACKETCHAN* ppc[32]; // 0x304 [O]; [R] SNDPKTPLAY_create
};

CHECK_OFFSET(SNDPACKETSTATE, callbackindex, 0x000);
CHECK_OFFSET(SNDPACKETSTATE, spcb, 0x004);
CHECK_OFFSET(SNDPACKETSTATE, ppc, 0x304);
CHECK_SIZE(SNDPACKETSTATE, 0x384);

#endif /* EALIB_SND_H */
