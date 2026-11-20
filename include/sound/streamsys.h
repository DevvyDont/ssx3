#ifndef SOUND_STREAMSYS_H
#define SOUND_STREAMSYS_H

// cStreamSys: the streamed-audio system: a table of cStreamInstance (one per stream: music, speech ...) driven by the
// STREAM_* calls (create, queue, pause / resume, release-hold, time queries).
//   cStreamSys : virtual cBankManager (: cAsyncSys, cBankMonitor, virtual cSoundSys)
// It is itself a virtual base of cMusicSys and cSpeechSys (sound/musicsys.h, sound/speechsys.h), so the one retail
// instance is the shared cStreamSys of the cSSXAudio: gSSXAudio + 0x7768 (sound/ssxaudio.h writes these three fields
// inline as mpStreamVBankManager / mNumStreams / mStreamInstance). cMusicSys / cSpeechSys reach it through their
// mpVStreamSys. EA's source is sound/streamsys.cpp (the July 2003 preview's assert strings; builds/x/v2
// segment_files); retail methods are sound/streamsys 0x002AA490 (ctor) ... 0x002AB1C8 (STREAM_TimeMult), after the
// cStreamInstance methods.
//
// Non-virtual part 0xC [R]: the ctor's in-charge branch (never taken in retail: cSSXAudio, cController, cSpeechSys
// and cMusicSys all call cStreamSys_cStreamSys(vbase, 0, ...)) puts cSoundSys at this + 0xC and cBankManager at
// this + 0x14 (sw this + 0x14 into 0x0, this + 0xC into cBankManager + 0x1D8 = 0x1EC); [U] in cSSXAudio the
// object ends at 0x7774 (then tail padding). A complete cStreamSys is 0xC + 0x8 (cSoundSys) + 0xAD0 (cBankManager) =
// 0xAE4, which is On Tour's DWARF size. cStreamSys declares no virtuals of its own (no vptr in the non-virtual part);
// its ctor / dtor store the virtual-base tables _vt$10cStreamSys$9cSoundSys (D_00483998, at cSoundSys + 0x4),
// _vt$10cStreamSys$12cBankManager (D_004839B8, at cBankManager + 0x1D4) and cBankMonitor_vtbl (cBankManager + 0xAB0).
// The struct below is the NON-VIRTUAL part, so it can be embedded where the object lives (like cMusicSys / cBankSys).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail constructor cStreamSys_cStreamSys (allocation tag "StreamArray") and destructor cStreamSys__cStreamSys;
//   [O] SSX On Tour's DWARF (class cStreamSys: mNumStreams 0x4, mStreamInstance 0x8; same offsets);
//   [U] how the retail STREAM_* methods use the field.

#include "layout.h"

struct cBankManager;
struct cStreamInstance;

struct cStreamSys {
    cBankManager* mpVBankManager;       // 0x000 [R] g++ 2.95 virtual-base pointer to cBankManager (this + 0x14 in a
                                        //       complete cStreamSys; cSSXAudio + 0x6C98); [U] STREAM_Create reaches the
                                        //       play options as mpVBankManager + 0x1D8, the ctor / dtor patch the
                                        //       base vtables through it
    int mNumStreams;                    // 0x004 [O]; [R] the ctor stores its count; [U] the loop bound of
                                        //       STREAM_Update / STREAM_Pause / STREAM_Resume and of the dtor
    cStreamInstance** mStreamInstance;  // 0x008 [O]; [R] operator new[](count * 4, "StreamArray"), zeroed by the ctor;
                                        //       the dtor destroys each instance (cStreamInstance__cStreamInstance(p,
                                        //       3))
                                        //       and frees it; [U] every STREAM_* method indexes it by stream number
};

CHECK_OFFSET(cStreamSys, mpVBankManager, 0x000);
CHECK_OFFSET(cStreamSys, mNumStreams, 0x004);
CHECK_OFFSET(cStreamSys, mStreamInstance, 0x008);
CHECK_SIZE(cStreamSys, 0xC);

#endif /* SOUND_STREAMSYS_H */
