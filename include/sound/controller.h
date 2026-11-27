#ifndef SOUND_CONTROLLER_H
#define SOUND_CONTROLLER_H

// cController: the sound controller, the base of the game's audio system cSSXAudio (sound/ssxaudio.h: cSSXAudio's
// cController part is 0x0-0x5728). It joins the crowd, music and speech systems over the shared stream system / bank
// manager / sound system and updates them each frame (cController_Update: async loads, crowd, bank monitor, music,
// speech, streams). EA's source file is not known (no preview assert names it); retail's methods are in the
// sound/streamsys unit (0x2AB200 ctor, 0x2AB478 dtor, 0x2AB7A0 BANKMONITOR_Update, 0x2AB8E8 MONITOR_Update, 0x2AB958
// Update), so the header sits in sound/. Class names: the July 2003 preview's vtables (_vt$11cController$...).
//   cController : cCrowdSys (0x0), cMusicSys (0x118), cSpeechSys (0x5560)
//   cCrowdSys : cMidiSys : cBankSys, virtual cBankManager; cMusicSys, cSpeechSys : virtual cStreamSys
//   (cStreamSys : virtual cBankManager; cBankManager : cAsyncSys, cBankMonitor, virtual cSoundSys)
// Primary vptr at 0xC: the ctor and dtor store _vt$9cCrowdSys (cCrowdSys_vtbl; slot 2 stays pure, so cController is
// abstract; cSSXAudio_vtbl replaces it). The secondary vptrs are the bases' own: cMusicSys's at 0x5558 (cMusicSys_vtbl) and
// cSpeechSys's at 0x571C (cSpeechSys_vtbl). The ctor also installs _vt$11cController$9cSoundSys / $12cBankMonitor /
// $12cBankManager (D_004837D8 / D_00483760 / D_004837A0) inside the shared virtual bases.
//
// Non-virtual part 0x5728 [R]: in a complete cController the ctor (embedded != 0) puts cSoundSys at +0x5728,
// cBankManager at +0x5730 and cStreamSys at +0x6200; in cSSXAudio (the only user, which passes embedded = 0) cSSXAudio's
// own data starts at 0x5728. The struct below is the NON-VIRTUAL part (like sound/crowdsys.h, musicsys.h, speechsys.h);
// the virtual bases are reached through the mpV* pointers.
//
// A FLAT layout, like main/game.h: every field at its retail offset, bases written out inline (cMusicSys and
// cSpeechSys as their existing non-virtual structs). Evidence tags:
//   [R] retail ctor cController_cController and dtor cController__cController;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF: cController's only own member is `int execFlag` (On Tour restructured the audio bases,
//       so only the name and type carry over).
// tools/layout_xlate.py pairs the dtor / Update (0xC, 0x118 unchanged; the vbase offsets 0x5558/0x571C moved).

#include "layout.h"
#include "sound/crowdsys.h"
#include "sound/musicsys.h"
#include "sound/speechsys.h"

struct cBankManager;

struct cController {
    // --- cCrowdSys (sound/crowdsys.h, non-virtual 0x118), with its bases cMidiSys and cBankSys
    cBankManager* mpVBankManager;   // 0x0000 [R] cBankSys: g++ 2.95 virtual-base pointer to the shared cBankManager; the
                                    //        ctor's in-charge branch stores this + 0x5730
    int* mBank;                     // 0x0004 [R] cBankSys ctor (see sound/banksys.h)
    int* mPatch;                    // 0x0008 [R] cBankSys ctor
    void** mVtbl;                   // 0x000C [R] ctor / dtor store _vt$9cCrowdSys (cCrowdSys_vtbl); cSSXAudio_vtbl in cSSXAudio
    tMidiBank* mMidiBanks;          // 0x0010 [R] cMidiSys ctor (see sound/midisys.h)
    int mNumInstances;              // 0x0014 [R] cCrowdSys ctor 0 (sound/crowdsys.h)
    cCrowdInstance* mpCrowdInstances; // 0x0018 [R] cCrowdSys ctor 0 (sound/crowdsys.h)
    tCrowdData mCrowdData[3];       // 0x001C [R] cCrowdSys ctor (sound/crowdsys.h)
    // --- cMusicSys (sound/musicsys.h, non-virtual 0x5448; ctor cMusicSys_cMusicSys(this + 0x118))
    cMusicSys MusicSys;             // 0x0118 [R] ctor cMusicSys_cMusicSys(this + 0x118, 0, ...); [U] every cController method reaches
                                    //        cBankManager as **(this + 0x118) (MusicSys.mpVStreamSys, then the cStreamSys's own
                                    //        vbase pointer); the ctor / dtor store MusicSys.mVtbl (0x5558) = cMusicSys_vtbl
    // --- cSpeechSys (sound/speechsys.h, non-virtual 0x1C0)
    cSpeechSys SpeechSys;           // 0x5560 [R] ctor cSpeechSys_cSpeechSys(this + 0x5560, 0, ...); the ctor / dtor store
                                    //        SpeechSys.mVtbl (0x571C) = cSpeechSys_vtbl; [U] cController_Update passes
                                    //        this + 0x5560 to the speech update (cSpeechSys_Update)
    // --- cController
    int execFlag;                   // 0x5720 [O] (cController's only member in On Tour); [R] ctor 0
    char unk_5724[0x4];             // 0x5724 padding (the non-virtual part ends 8-aligned; cSoundSys starts at 0x5728)

    // Methods bound to their retail symbols by asm labels.
    void BANKMONITOR_Update(float time) __asm__("cController_BANKMONITOR_Update");
    void MONITOR_Update() __asm__("cController_MONITOR_Update");
    void Update(float frameTime) __asm__("cController_Update");
};

CHECK_OFFSET(cController, mpVBankManager, 0x0000);
CHECK_OFFSET(cController, mBank, 0x0004);
CHECK_OFFSET(cController, mPatch, 0x0008);
CHECK_OFFSET(cController, mVtbl, 0x000C);
CHECK_OFFSET(cController, mMidiBanks, 0x0010);
CHECK_OFFSET(cController, mNumInstances, 0x0014);
CHECK_OFFSET(cController, mpCrowdInstances, 0x0018);
CHECK_OFFSET(cController, mCrowdData, 0x001C);
CHECK_OFFSET(cController, MusicSys, 0x0118);
CHECK_OFFSET(cController, SpeechSys, 0x5560);
CHECK_OFFSET(cController, execFlag, 0x5720);
CHECK_OFFSET(cController, unk_5724, 0x5724);
CHECK_SIZE(cController, 0x5728);

#endif /* SOUND_CONTROLLER_H */
