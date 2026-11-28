#ifndef SOUND_SOUNDSYS_H
#define SOUND_SOUNDSYS_H
#pragma interface

// cSoundSys: the root of the EA sound classes: owns the platform sound manager (cPS2SoundManager) and sets the audio
// mode. It is a VIRTUAL base of cSndPlayOpts / cMonitor / cBankMonitor / cBankManager / cBankSys / cStreamSys / ...,
// so there is one per complete object: cSSXAudio + 0x6C90 (sound/ssxaudio.h). EA source
// d:/ssx/code/source/sound/soundsys.cpp (preview assert "pManager", next to "pathxSND", the EA driver path; retail
// methods head the sound/bankmonitor unit's soundsys part, 0x2ADE88..0x2ADFB0; the rest of that part is the EA
// driver glue: C functions, no this). Preview vtables _vt$9cSoundSys and _vt$<class>$9cSoundSys.
// Size 0x8 [R]: in a complete cBankSys it occupies this + 0x10 .. + 0x18 (cBankManager follows at + 0x18); [O] 0x8.
// The ctor cSoundSys_cSoundSys stores cSoundSys_vtbl (0x483C00: dtor, SetMode) at 0x4 and itself in SoundSys.
//
// A real C++ class (`#pragma interface`: no unit emits its vtable; the equate after the class binds g++'s name to the
// retail table); the classes that have it as a virtual base still reach it through their flat views. Evidence tags:
//   [R] the constructor cSoundSys_cSoundSys / destructor cSoundSys__cSoundSys;
//   [O] SSX On Tour's DWARF (class cSoundSys: cSoundManager* pManager at 0x0, size 0x8);
//   [U] how the retail methods use the field.

#include "layout.h"
#include "bx/memman.h"     // the destructor's delete (__builtin_delete)

struct cPS2SoundManager;

// The sound output mode (SetMode's argument, cSSXAudio::mAudioMode). [O] On Tour's enum eSOUNDDEF_MODE, names and
// values. Retail also has a fourth mode, 3 (cPS2SoundManager_SetMode: four output channels and S/PDIF mode 2; the
// cSSXAudio mode converters pass it through), that On Tour does not name, so it stays a number.
enum eSOUNDDEF_MODE {
    keSOUNDDEF_MODE_MONO = 0, keSOUNDDEF_MODE_STEREO = 1, keSOUNDDEF_MODE_DOLBYSURROUND = 2
};

// SetMode's result. [O] On Tour's enum eSOUNDMANAGER_SETMODE, names and values; [U] cSSXAudio_SetMode keeps the new
// mode only on SUCCESS, and cPS2SoundManager_SetMode returns 2 once it applied a configuration, 1 when it refused.
enum eSOUNDMANAGER_SETMODE {
    keSOUNDMANAGER_SETMODE_FAIL = 0, keSOUNDMANAGER_SETMODE_RESTART = 1, keSOUNDMANAGER_SETMODE_SUCCESS = 2
};

struct cSoundSys {
    cPS2SoundManager* pManager; // 0x0 [O] cSoundSys::pManager (On Tour type cSoundManager*); [R] ctor = cSoundManager::Construct() (cSoundManager_Construct, a cPS2SoundManager) and calls its Init; dtor calls its Shutdown and deletes it; [U] SetMode (cSoundSys_SetMode) calls its slot 2
    //   the vptr 0x4 [R] cSoundSys_vtbl (ctor/dtor); the classes that have it as a virtual base point it at their own
    //       copies (e.g. D_00483A60 in cBankSys)

    // Methods bound to their retail symbols by asm labels; the virtuals in cSoundSys_vtbl's slot order.
    cSoundSys(int audioMode) __asm__("cSoundSys_cSoundSys");
    virtual ~cSoundSys();                                           // 1 (_$_9cSoundSys)
    virtual int SetMode(int mode) __asm__("cSoundSys_SetMode");     // 2
};

CHECK_OFFSET(cSoundSys, pManager, 0x0);
CHECK_SIZE(cSoundSys, 0x8);
__asm__("_vt$9cSoundSys = cSoundSys_vtbl");   // g++'s name for the retail vtable

// The current sound system: cSoundSys_cSoundSys stores itself here, cSoundSys__cSoundSys clears it. EA name unknown.
// [U] SndRand (cSoundSys_SndRand) is called with it.
extern cSoundSys* SoundSys;         // [O] On Tour SoundSys; [R] cSoundSys_cSoundSys stores self, the dtor clears it

#endif /* SOUND_SOUNDSYS_H */
