#ifndef SOUND_SOUNDSYS_H
#define SOUND_SOUNDSYS_H

// cSoundSys: the root of the EA sound classes: owns the platform sound manager (cPS2SoundManager) and sets the audio
// mode. It is a VIRTUAL base of cSndPlayOpts / cMonitor / cBankMonitor / cBankManager / cBankSys / cStreamSys / ...,
// so there is one per complete object: cSSXAudio + 0x6C90 (sound/ssxaudio.h). EA source
// d:/ssx/code/source/sound/soundsys.cpp (preview assert "pManager", next to "pathxSND", the EA driver path; retail
// methods head the sound/bankmonitor unit's soundsys part, 0x2ADE88..0x2ADFB0; the rest of that part is the EA
// driver glue: C functions, no this). Preview vtables _vt$9cSoundSys and _vt$<class>$9cSoundSys.
// Size 0x8 [R]: in a complete cBankSys it occupies this + 0x10 .. + 0x18 (cBankManager follows at + 0x18); [O] 0x8.
// The ctor cSoundSys_cSoundSys stores cSoundSys_vtbl (0x483C00: dtor, SetMode) at 0x4 and itself in SoundSys.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] the constructor cSoundSys_cSoundSys / destructor cSoundSys__cSoundSys;
//   [O] SSX On Tour's DWARF (class cSoundSys: cSoundManager* pManager at 0x0, size 0x8);
//   [U] how the retail methods use the field.

#include "layout.h"

struct cPS2SoundManager;

struct cSoundSys {
    cPS2SoundManager* pManager; // 0x0 [O] cSoundSys::pManager (On Tour type cSoundManager*); [R] ctor = cSoundManager::Construct() (func_002A8FA0, a cPS2SoundManager) and calls its Init; dtor calls its Shutdown and deletes it; [U] SetMode (func_002ADF80) calls its slot 2
    void** mVtbl;               // 0x4 [R] cSoundSys_vtbl (ctor/dtor); the classes that have it as a virtual base point it at their own copies (e.g. D_00483A60 in cBankSys)
};

CHECK_OFFSET(cSoundSys, pManager, 0x0);
CHECK_OFFSET(cSoundSys, mVtbl, 0x4);
CHECK_SIZE(cSoundSys, 0x8);

// The current sound system: cSoundSys_cSoundSys stores itself here, cSoundSys__cSoundSys clears it. EA name unknown.
// [U] SndRand (func_002ADF60) is called with it.
extern cSoundSys* SoundSys;         // [O] On Tour SoundSys; [R] cSoundSys_cSoundSys stores self, the dtor clears it

#endif /* SOUND_SOUNDSYS_H */
