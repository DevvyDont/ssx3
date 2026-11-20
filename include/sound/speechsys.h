#ifndef SOUND_SPEECHSYS_H
#define SOUND_SPEECHSYS_H

// cSpeechSys: the speech (DJ / PA / character) streaming system: two speech instances (one per stream), the per-
// language speech .big files, and a short table of pending speech requests. EA source
// d:/ssx/code/source/sound/speechsys.cpp (preview assert strings); retail methods are in the sound/bankmonitor unit
// (0x2B07F8..0x2B1780) plus out-of-line inline functions in bx/seg_1BA100 (0x2BACF0..0x2BAD14).
// One instance: the cSpeechSys base of cController, i.e. gSSXAudio + 0x5560 (sound/ssxaudio.h), built by the
// cController ctor cSpeechSys_cSpeechSys(this + 0x5560, 0, ...); D_004A37B4 points at it.
//   cSpeechSys : virtual cStreamSys (: virtual cBankManager : cAsyncSys, cBankMonitor, virtual cSoundSys)
// Non-virtual part 0x1C0 [R]: in a complete cSpeechSys the ctor (inchrg != 0) puts cSoundSys at +0x1C0, cBankManager
// at +0x1C8 and cStreamSys at +0xC98; in cSSXAudio the cController data starts at 0x5560 + 0x1C0 = 0x5720. Vtable
// cSpeechSys_vtbl (cSpeechSys / cController), D_004836D8 (cSSXAudio). The struct below is the NON-VIRTUAL part.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance. Evidence tags:
//   [R] retail constructor cSpeechSys_cSpeechSys and destructor cSpeechSys__cSpeechSys;
//   [U] how retail code uses the field.
// The preview's cSpeechSys accessors (UpdateSpeechVolume, GetSpeechVolumeIdentifier) touch no field; layout_xlate.py
// pairs only the vptr (preview 0x158 -> retail 0x1BC: retail added 0x64 bytes, the .big-file name table fits that).
// On Tour has no cSpeechSys. Names come from allocation tags where they exist, otherwise unk_.

#include "layout.h"

struct cStreamSys;

// One pending speech request (0x20; our name). [U] func_002B1458 adds one (timer 180 frames; refreshes a match),
// func_002B1720 counts timers down, func_002B1520 applies a matching request to its speech instance
// (instance + 0x8CC/0x8D0/0x8F4/0x8F8/0x8FC), func_002B16A0 drops it.
struct tSpeechRequest { // member names are ours, from use
    int used;       // 0x00 [U] func_002B1458 1; func_002B1428 clears all; func_002B16A0 0
    int timer;      // 0x04 [U] func_002B1458 180; func_002B1720 decrements, frees at 0
    int instance;   // 0x08 [U] speech instance index (0..1) the request is for (func_002B1458/func_002B1520 match on it)
    int id;         // 0x0C [U] request key (matched with instance)
    void* object;   // 0x10 [U] func_002B1520 calls its virtual slots (object + 0x6C0) and reads object + 0x870
    int unk_14;     // 0x14 [U] func_002B1520 -> instance + 0x8D0
    float unk_18;   // 0x18 [U] func_002B1520 -> instance + 0x8F4
    void (*callback)(int); // 0x1C [U] func_002B1458 stores it; func_002B1520 copies it to mCallback (0 clears mCallback / mCallbackInstance)
};

struct cSpeechSys {
    cStreamSys* mpVStreamSys;           // 0x000 [R] g++ 2.95 virtual-base pointer to cStreamSys (= this + 0xC98 in a complete object; cSSXAudio + 0x7768); [U] methods reach cBankManager as **(this) and its cBankMonitor as **(this) + 0x1D8
    void** mSpeechInstance;             // 0x004 [R] operator new[](2 x 4, "mSpeechInstance"), both 0; [U] func_002B0E60 fills slot i with cMemMan_alloc(0x904, "SpeechInstance") + ctor func_002AFA68; the dtor frees the array
    void* unk_008;                      // 0x008 [U] SPCH_Init: MEM_alloc("bank mem", n, 0) (the speech bank buffer); func_002B0DA0 MEM_free()s and clears it
    void (*mCallback)(int);             // 0x00C (our name) [R] ctor 0; [U] func_002B0F48 calls it with the started speech's handle (result unused); func_002B1058/func_002B1520 store it (as int)
    int mCallbackInstance;              // 0x010 (our name) [R] ctor -1; [U] the speech instance index mCallback is armed for (func_002B0F48 compares, func_002B1058 sets/clears)
    char unk_014[0x4];                  // 0x014 no access found
    tSpeechRequest mRequest[10];        // 0x018 [R] ctor clears every used flag (func_002B1428); [U] func_002B1458/func_002B1520/func_002B16A0/func_002B1720
    char mBigFileName[5][0x14];         // 0x158 [R] ctor copies "audio/speech.big", "audio/english.big", "audio/french.big", "audio/german.big", "audio/spanish.big"; [U] retail accessor func_002BAD00 returns this + 0x158 + 20 * i
    void** mVtbl;                       // 0x1BC [R] cSpeechSys vptr: cSpeechSys_vtbl (cSpeechSys ctor/dtor), D_004836D8 in cSSXAudio; [U] func_002B1268 calls slot 3 through it
};

CHECK_OFFSET(tSpeechRequest, used, 0x00);
CHECK_OFFSET(tSpeechRequest, timer, 0x04);
CHECK_OFFSET(tSpeechRequest, instance, 0x08);
CHECK_OFFSET(tSpeechRequest, id, 0x0C);
CHECK_OFFSET(tSpeechRequest, object, 0x10);
CHECK_OFFSET(tSpeechRequest, unk_14, 0x14);
CHECK_OFFSET(tSpeechRequest, unk_18, 0x18);
CHECK_OFFSET(tSpeechRequest, callback, 0x1C);
CHECK_SIZE(tSpeechRequest, 0x20);

CHECK_OFFSET(cSpeechSys, mpVStreamSys, 0x000);
CHECK_OFFSET(cSpeechSys, mSpeechInstance, 0x004);
CHECK_OFFSET(cSpeechSys, unk_008, 0x008);
CHECK_OFFSET(cSpeechSys, mCallback, 0x00C);
CHECK_OFFSET(cSpeechSys, mCallbackInstance, 0x010);
CHECK_OFFSET(cSpeechSys, unk_014, 0x014);
CHECK_OFFSET(cSpeechSys, mRequest, 0x018);
CHECK_OFFSET(cSpeechSys, mBigFileName, 0x158);
CHECK_OFFSET(cSpeechSys, mVtbl, 0x1BC);
CHECK_SIZE(cSpeechSys, 0x1C0);

// The speech system inside the audio system (cSSXAudio::SpeechSys): its constructor stores itself here, its
// destructor clears it. EA's name for it is unknown.
extern cSpeechSys* D_004A37B4;

#endif /* SOUND_SPEECHSYS_H */
