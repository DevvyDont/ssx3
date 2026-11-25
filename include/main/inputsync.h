#ifndef MAIN_INPUTSYNC_H
#define MAIN_INPUTSYNC_H

// cInputSync: the online race's lockstep input synchroniser. Each human rider's input is wrapped in a "SyncedInput"
// whose pad is fed from a ring of PadSamples: local pads are sampled, compressed and sent with cGameComm_writePacket
// (preupdate / sendClientInput), remote pads are filled from cGameComm_readPacket, and aiupdate hands every pad its
// next sample once all of them have one (aiinputready). EA's source: d:/ssx/code/source/main/inputsync.cpp (preview
// assert path); retail code is in the decomp's unit main/gamestate.cpp (0x00237060 .. 0x00237CA8).
// Size 0x44: [R] cGame_load does operator new(68, "InputSync", 0, 0) + cInputSync_cInputSync(p, latencyFrames,
// cGameComm_mGameComm->mNumSamples) and keeps it in cGame::InputSync (main/game.h, 0x7C); cGame_purge deletes it with
// cInputSync__cInputSync(p, 3). 0x44 = 0x14 + 2 * 0x18: two synced pads (one per human player).
// No base classes, no vtable (the destructor is called directly; no vptr store anywhere).
//
// A FLAT layout, like main/game.h. Tags: [R] the constructor cInputSync_cInputSync / destructor cInputSync__cInputSync
// and the allocation above; [U] how retail code uses the field (main/gamestate.cpp, the asm); [O] SSX On Tour's DWARF
// (class cInputSync, size 0x8C): same member names and order, but On Tour adds `bool mDisabled` before sendlimit, a
// `sendpos` in SyncedPad (0x1C there), four synced pads and `bool mbForceFirstUpdate`, so its offsets are not used.
// No preview evidence (no accessors).

#include "layout.h"

struct cInputPad;   // the pad a SyncedPad reads from / writes to (cInputPad_update; 0x2A4 bytes, first word = button count)
struct cInput;      // 8 bytes: {cInputPad* pad, cInputMap* map} (cInput_setPad / cInput_setMap)

// One input sample: a button count and the button values. [O] struct PadSample (size 0x64, same members);
// [R] createlocalinput / createremoteinput allocate latency * 100 bytes and set NumButtons = 0x18 in each.
struct cInputSync_PadSample {
    int NumButtons;                 // 0x000 [O]; [R] 0x18 (24) in each new sample; [U] aiupdate passes it to cInputPad_update
    float Buttons[24];              // 0x004 [O]; [U] preupdate fills it from the source pad's button values
};
CHECK_OFFSET(cInputSync_PadSample, NumButtons, 0x0);
CHECK_OFFSET(cInputSync_PadSample, Buttons, 0x4);
CHECK_SIZE(cInputSync_PadSample, 0x64);

// One compressed input sample, as sent over the network: the button count, a bit mask of the non-zero buttons and
// the non-zero values only, one byte each. [O] struct CompressedSample (On Tour 0x1D: an extra `unsigned char
// PlayerID` first, which retail does not have); [U] cInputSync_compressinput / decompressinput: count at 0x0, mask
// bytes 0x1-0x3 (24 bits, one per button), kept bytes from 0x4, return value 4 + the number kept; [R]
// sendClientInput / preupdate build it in a stack buffer and send 4 + n bytes with cGameComm_writePacket.
struct cInputSync_CompressedSample {
    unsigned char NumButtons;           // 0x000 [O]; [U] compressinput stores the sample's NumButtons, decompressinput
                                        //       restores it
    unsigned char NonZeroBitBytes[3];   // 0x001 [O]; [U] bit i of the 24 = button i is non-zero
    unsigned char NonZeroBytes[24];     // 0x004 [O]; [U] the non-zero buttons' values (value * 255), in button order
};
CHECK_OFFSET(cInputSync_CompressedSample, NumButtons, 0x0);
CHECK_OFFSET(cInputSync_CompressedSample, NonZeroBitBytes, 0x1);
CHECK_OFFSET(cInputSync_CompressedSample, NonZeroBytes, 0x4);
CHECK_SIZE(cInputSync_CompressedSample, 0x1C);

// One synced pad. [O] struct SyncedPad (On Tour 0x1C: it has an extra `int sendpos` after writepos); retail stride 0x18
// ([U] createlocalinput / aiupdate / preupdate / the destructor index `self + 0x14 + i * 0x18`).
struct cInputSync_SyncedPad {
    cInputSync_PadSample* samples;  // 0x000 [O]; [R] "InputSamples" ring of `latency` samples (createlocal/remoteinput);
                                    //       [U] the destructor cMemMan_frees it; preupdate writes, aiupdate reads it
    int readpos;                    // 0x004 [O]; [R] 0; [U] aiupdate reads samples[readpos % latency] and advances it;
                                    //       aiinputready: no sample when readpos == writepos
    int writepos;                   // 0x008 [O]; [R] = the owner's sendlimit; [U] preupdate advances it per sample written
    cInputPad* sourcepad;           // 0x00C [O]; [R] the real input's pad (createlocalinput), 0 for a remote pad
                                    //       (createremoteinput); [U] preupdate: local when nonzero
    cInputPad* destinationpad;      // 0x010 [O]; [R] cMemMan_alloc(676, "SyncedPad"), first word zeroed; [U] aiupdate
                                    //       cInputPad_update's it; the destructor deletes it
    cInput* input;                  // 0x014 [O]; [R] cMemMan_alloc(8, "SyncedInput") with setPad(destinationpad) and the
                                    //       real input's map; returned to cAI_InitPlayers; [U] the destructor deletes it
};
CHECK_OFFSET(cInputSync_SyncedPad, samples, 0x0);
CHECK_OFFSET(cInputSync_SyncedPad, readpos, 0x4);
CHECK_OFFSET(cInputSync_SyncedPad, writepos, 0x8);
CHECK_OFFSET(cInputSync_SyncedPad, sourcepad, 0xC);
CHECK_OFFSET(cInputSync_SyncedPad, destinationpad, 0x10);
CHECK_OFFSET(cInputSync_SyncedPad, input, 0x14);
CHECK_SIZE(cInputSync_SyncedPad, 0x18);

struct cInputSync {
    int sendlimit;                  // 0x000 [O]; [R] ctor: l * n; [U] preupdate recomputes it from cGameComm_getLatency
                                    //       (at least 2 samples, capped at latency); createlocal/remoteinput start writepos at it
    int latency;                    // 0x004 [O]; [R] ctor: l, then (60 / n) * n; [U] the sample ring's size (modulus in
                                    //       preupdate / aiupdate; createlocal/remoteinput allocate latency samples)
    int numsamples;                 // 0x008 [O]; [R] ctor: n (cGameComm::mNumSamples); [U] preupdate writes each sample n times
    int tick;                       // 0x00C [O]; [R] ctor zeroes it; [U] preupdate counts frames, samples every n-th
    int numsyncedpads;              // 0x010 [O]; [R] ctor zeroes it; [U] createlocal/remoteinput take syncedpads[numsyncedpads++];
                                    //       preupdate / aiinputready / aiupdate / the destructor loop over it
    cInputSync_SyncedPad syncedpads[2]; // 0x014 [O] (On Tour: 4); [R] 0x44 - 0x14 = 2 * 0x18

    // Methods bound to their retail symbols by asm labels.
    void* createlocalinput(int* realinput) __asm__("cInputSync_createlocalinput");
    void* createremoteinput(int realinputmap) __asm__("cInputSync_createremoteinput");
    void preupdate() __asm__("cInputSync_preupdate");
    int aiinputready() __asm__("cInputSync_aiinputready");
    int aiupdate() __asm__("cInputSync_aiupdate");
    void sendClientInput(cInputSync_PadSample* src) __asm__("cInputSync_sendClientInput");
    int compressinput(cInputSync_PadSample* src, cInputSync_CompressedSample* dst) __asm__("cInputSync_compressinput");
    void decompressinput(cInputSync_PadSample* out, char* buf) __asm__("cInputSync_decompressinput");
};
CHECK_OFFSET(cInputSync, sendlimit, 0x0);
CHECK_OFFSET(cInputSync, latency, 0x4);
CHECK_OFFSET(cInputSync, numsamples, 0x8);
CHECK_OFFSET(cInputSync, tick, 0xC);
CHECK_OFFSET(cInputSync, numsyncedpads, 0x10);
CHECK_OFFSET(cInputSync, syncedpads, 0x14);
CHECK_SIZE(cInputSync, 0x44);

#endif /* MAIN_INPUTSYNC_H */
