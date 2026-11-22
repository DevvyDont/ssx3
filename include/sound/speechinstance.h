#ifndef SOUND_SPEECHINSTANCE_H
#define SOUND_SPEECHINSTANCE_H

// cSpeechInstance and cSpeechBank: the per-stream halves of cSpeechSys (sound/speechsys.h). cSpeechSys owns two
// cSpeechInstance (cSpeechSys::mSpeechInstance[2], one per speech stream); each loads the speech event files and
// up to 500 speech banks (cSpeechBank, one per .hdr file) listed by data/config/speech.inf and plays requests on
// its cStreamSys stream. EA source d:/ssx/code/source/sound/speechsys.cpp (preview strings "SpeechBank",
// "SpeechInstance", "%sheaders.big", "%seventdat\events.evt", "mNumTotalBanks >= 0"); retail methods in the
// sound/bankmonitor unit 0x2AF8A8..0x2B07F8, just before cSpeechSys's. Neither class has a vtable.
// Class names from the allocation tags "SpeechInstance" / "SpeechBank". On Tour has no speech system.
// Sizes [R]: cSpeechSys func_002B0E60 = func_002AFA68(cMemMan_alloc(0x904, "SpeechInstance")); func_002AF960 =
// cSpeechBank_cSpeechBank(cMemMan_alloc(0x4C, "SpeechBank")).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] constructors func_002AFA68 / cSpeechBank_cSpeechBank, destructors func_002AFB38 / cSpeechBank__cSpeechBank, the speech.inf keys
//       func_002B0088 reads;
//   [U] how the retail methods use the field. Names marked "(our name)" describe that use; the rest are unk_.

#include "layout.h"

// cSpeechBank (0x4C): one loaded speech bank header (a "<name>.hdr" file).
struct cSpeechBank {
    int unk_00;        // 0x00 [R] ctor -1; func_002AF960 = func_003D69F0(data) (the driver's bank id); [U] func_002AFA58 compares it with a requested bank id; dtor func_003D6CA0()s it
    char mName[0x3C];  // 0x04 (our name) [R] ctor clears byte 0; func_002AF960 strcpy()s the bank path; [U] func_002B04D8 builds the stream file name from it and its language suffix (func_002B03B8)
    char* mShortName;  // 0x40 (our name) [R] func_002AF960: the part of mName after the last '\\'; [U] func_002B04D8 passes it to cSpeechSys's vtable slot 2
    void* mpData;      // 0x44 (our name) [R] ctor 0; func_002AF960: the .hdr data (passed in, or FILE_load()ed); dtor MEM_free()s it when mOwnsData
    int mOwnsData;     // 0x48 (our name) [R] ctor 0; func_002AF960 1 when it loaded mpData itself
};

// One speech event file of a cSpeechInstance (8 bytes; our names).
struct tSpeechEventFile {
    void* mpData; // 0x0 [U] func_002AFD40: the event data (located inside the loaded headers .big, or FILE_loadz("%seventdat\events.evt")); registered with func_003D66C0
    int mOwned;   // 0x4 [U] func_002AFD40 1 when loaded on its own; dtor MEM_free()s mpData then
};

// cSpeechInstance (0x904): one speech stream.
struct cSpeechInstance {
    int unk_000;                       // 0x000 [R] ctor arg 1 = the instance's index in cSpeechSys::mSpeechInstance; [U] func_002B04D8 passes it to cSpeechSys (request match func_002B1520, vtable slots 2/3)
    int unk_004;                       // 0x004 [R] ctor arg 2; [U] the cStreamSys stream id: STREAM_ calls (cStreamSys_STREAM_Create in the ctor, cStreamSys_STREAM_Destroy in the dtor, cStreamSys_STREAM_Requests / cStreamSys_STREAM_Purge / cStreamSys_STREAM_TimeRemaining / cStreamSys_STREAM_Playing / cStreamSys_STREAM_SetVolControl) and STREAM_SetBigFile (cStreamSys_STREAM_SetBigFile, speech.inf BIGFILE)
    int unk_008;                       // 0x008 [R] ctor arg 6; no read found
    char unk_00C[0x60];                // 0x00C [R] ctor clears byte 0 (an empty string); no other access found
    char mBasePath[0x60];              // 0x06C [R] speech.inf BASEPATH (func_002B0088 AssignString); [U] func_002AFD40 / func_002B0088 sprintf the .big / events / bank paths from it
    tSpeechEventFile mEventFile[4];    // 0x0CC [R] ctor zeroes 4; [U] func_002AFD40 fills [unk_0EC]; dtor frees
    int unk_0EC;                       // 0x0EC [R] ctor 0; [U] number of event files loaded (func_002B0088 ++ per speech.inf BASEPATH entry); dtor loop bound
    int unk_0F0;                       // 0x0F0 [R] ctor 0; [U] number of speech banks in unk_0F4 (func_002B0348 ++, cSpeechInstance_mNumTotalBanks counts them all); dtor / func_002B04D8 loop bound
    cSpeechBank* unk_0F4[500];         // 0x0F4 [U] func_002B0348 appends func_002AF960(...); func_002B04D8 searches by bank id; dtor destroys them (cSpeechBank__cSpeechBank)
    int unk_8C4;                       // 0x8C4 [R] ctor 0; [U] set by func_002B07E8 (cSpeechSys func_002B0FE8)
    int unk_8C8;                       // 0x8C8 [R] ctor 0; [U] set by func_002B07F0 (cSpeechSys func_002B1020); func_002B04D8 passes it to cSpeechSys's play slot
    int unk_8CC;                       // 0x8CC [R] ctor 0; [U] cSpeechSys func_002B1058 / func_002B1520 set it; func_002B04D8 passes it to the play slot
    int unk_8D0;                       // 0x8D0 [R] ctor 0; [U] as unk_8CC (tSpeechRequest::unk_14)
    void* unk_8D4[4][2];               // 0x8D4 [U] func_002AFD40: per event file, the loaded "%sheaders.big" ([.][0]) / "%slanghead.big" ([.][1]) data; dtor MEM_free()s them
    float unk_8F4;                     // 0x8F4 [R] ctor 0 (sw zero); [U] cSpeechSys func_002B1058 / func_002B1520 store a float; func_002B04D8 lwc1 (play slot float argument)
    int unk_8F8;                       // 0x8F8 [U] cSpeechSys func_002B1520; func_002B04D8 passes it to the play slot
    int unk_8FC;                       // 0x8FC [U] as unk_8F8
    int unk_900;                       // 0x900 [R] ctor 1; [U] func_002AFC88 / func_002AFD10 toggle it around func_003D7EC8 / func_003D8040 (stream pause state)
};

CHECK_OFFSET(cSpeechBank, unk_00, 0x00);
CHECK_OFFSET(cSpeechBank, mName, 0x04);
CHECK_OFFSET(cSpeechBank, mShortName, 0x40);
CHECK_OFFSET(cSpeechBank, mpData, 0x44);
CHECK_OFFSET(cSpeechBank, mOwnsData, 0x48);
CHECK_SIZE(cSpeechBank, 0x4C);
CHECK_OFFSET(tSpeechEventFile, mpData, 0x0);
CHECK_OFFSET(tSpeechEventFile, mOwned, 0x4);
CHECK_SIZE(tSpeechEventFile, 0x8);
CHECK_OFFSET(cSpeechInstance, unk_000, 0x000);
CHECK_OFFSET(cSpeechInstance, unk_004, 0x004);
CHECK_OFFSET(cSpeechInstance, unk_008, 0x008);
CHECK_OFFSET(cSpeechInstance, unk_00C, 0x00C);
CHECK_OFFSET(cSpeechInstance, mBasePath, 0x06C);
CHECK_OFFSET(cSpeechInstance, mEventFile, 0x0CC);
CHECK_OFFSET(cSpeechInstance, unk_0EC, 0x0EC);
CHECK_OFFSET(cSpeechInstance, unk_0F0, 0x0F0);
CHECK_OFFSET(cSpeechInstance, unk_0F4, 0x0F4);
CHECK_OFFSET(cSpeechInstance, unk_8C4, 0x8C4);
CHECK_OFFSET(cSpeechInstance, unk_8C8, 0x8C8);
CHECK_OFFSET(cSpeechInstance, unk_8CC, 0x8CC);
CHECK_OFFSET(cSpeechInstance, unk_8D0, 0x8D0);
CHECK_OFFSET(cSpeechInstance, unk_8D4, 0x8D4);
CHECK_OFFSET(cSpeechInstance, unk_8F4, 0x8F4);
CHECK_OFFSET(cSpeechInstance, unk_8F8, 0x8F8);
CHECK_OFFSET(cSpeechInstance, unk_8FC, 0x8FC);
CHECK_OFFSET(cSpeechInstance, unk_900, 0x900);
CHECK_SIZE(cSpeechInstance, 0x904);

#endif /* SOUND_SPEECHINSTANCE_H */
