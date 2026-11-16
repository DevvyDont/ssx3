#ifndef SOUND_MIDISYS_H
#define SOUND_MIDISYS_H

// cMidiSys: plays MIDI sequences ("MIDx" data) from loaded banks through the shared cBankManager (MIDI_Begin pushes a
// play-option level and records the bank + sequence for it; MIDI_End plays it and hands the voice to the cBankMonitor
// as a MONITOR_MIDI instance). EA source d:/ssx/code/source/sound/midisys.cpp (preview assert strings "MIDIBANKS",
// "mpSettings", "pData[ 0 ] == 'M' && ... == 'x'"); retail methods in the sound/soundsys unit 0x2A8140..0x2A8C20.
//   cMidiSys : cBankSys (0x0), virtual cBankManager
// Non-virtual part 0x14 [R]: the ctor (cMidiSys_cMidiSys, 0x2A8140; the
// preview names its dtor cMidiSys__cMidiSys cMidiSys::<virtual slot 1>) puts cSoundSys at this + 0x14 and cBankManager at
// this + 0x1C in a complete cMidiSys, calls cBankSys_cBankSys(this, 0, ...) and stores D_00483A48 at 0xC (cBankSys's
// vtable; cMidiSys's own dtor sits in its virtual-base tables D_004839F0 / D_00483A10). The struct below is the
// NON-VIRTUAL part: cCrowdSys (sound/crowdsys.h) and so cSSXAudio start with it. The ctor stores the instance in
// D_004A3744 (EA name unknown).
//
// A FLAT layout, like main/game.h: every field at its retail offset, bases written out inline. Evidence tags:
//   [R] the constructor (0x2A8140) and destructor cMidiSys__cMidiSys; [O] SSX On Tour's DWARF (cBankSys members; On Tour has
//   no cMidiSys); [U] how the retail methods use the field.

#include "layout.h"

struct cBankManager;

// One cMidiSys::mMidiBanks level (8 bytes; member names are ours). [U] MIDI_Begin (func_002A8450) stores (bank,
// data) at [mInst]; MIDI_End (func_002A86B8) plays data from bank, passes them to cBankMonitorInstance SetMIDI
// (func_002ABD38, On Tour SetMIDI(void* pData, int Bank)) and to the delay-echo monitors.
struct tMidiBank {
    int mBank;    // 0x0 [U] cBankManager bank index (BANK_Handle'd before playing)
    void* mpData; // 0x4 [U] the MIDI sequence inside the bank (On Tour SetMIDI's pData)
};

struct cMidiSys {
    // --- cBankSys (non-virtual part 0x10; sound/banksys.h)
    cBankManager* mpVBankManager; // 0x000 [R] virtual-base pointer (this + 0x1C in a complete cMidiSys); [U] MIDI_Begin/MIDI_End reach the play options at mpVBankManager + 0x1D8
    int* mBank;                   // 0x004 [O] cBankSys::mBank; [R] cBankSys ctor "mBank"
    int* mPatch;                  // 0x008 [O] cBankSys::mPatch; [R] cBankSys ctor "mPath"
    void** mVtbl;                 // 0x00C [R] D_00483A48 (cMidiSys ctor/dtor; cBankSys's primary vtable)
    // --- cMidiSys
    tMidiBank* mMidiBanks;        // 0x010 [R] operator new(0x20, "MIDIBANKS") (4 play-option levels); dtor cMemMan_free()s it; [U] func_002A8450 / func_002A86B8
};

CHECK_OFFSET(tMidiBank, mBank, 0x0);
CHECK_OFFSET(tMidiBank, mpData, 0x4);
CHECK_SIZE(tMidiBank, 0x8);
CHECK_OFFSET(cMidiSys, mpVBankManager, 0x000);
CHECK_OFFSET(cMidiSys, mBank, 0x004);
CHECK_OFFSET(cMidiSys, mPatch, 0x008);
CHECK_OFFSET(cMidiSys, mVtbl, 0x00C);
CHECK_OFFSET(cMidiSys, mMidiBanks, 0x010);
CHECK_SIZE(cMidiSys, 0x14);

// The MIDI system: its constructor stores itself here (the cMidiSys base of the one cSSXAudio). EA name unknown.
extern cMidiSys* D_004A3744;        // [R] the cMidiSys ctor stores self, its dtor cMidiSys__cMidiSys clears it

#endif /* SOUND_MIDISYS_H */
