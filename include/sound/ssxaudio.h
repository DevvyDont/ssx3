#ifndef SOUND_SSXAUDIO_H
#define SOUND_SSXAUDIO_H

// cSSXAudio: the game's audio system (SFX, rider board/carve sounds, wind, crowd, music, speech/DJ, mixes and ducking).
// One instance, gSSXAudio (D_004A3500), created by SSXAUDIO_Init: cMemMan_alloc(0x7780, "SSXAudioSystem", ...) +
// cSSXAudio_cSSXAudio(mem, 1, 0x1C banks, 3 streams, audioMode). Size 0x7780 [R]. Retail vtable cSSXAudio_vtbl (0x004836B8).
// EA's source path is unknown (the preview has no cSSXAudio assert); the methods live in sound/ssxAudio.cpp and
// sound/soundsys.cpp (0x290B58..0x2A4B68).
//
// Hierarchy (retail; class names from the July 2003 preview's vtable/mangled names, see builds/x/v2/name_map_v2.tsv):
//   cSSXAudio : cController
//   cController : cCrowdSys (0x0), cMusicSys (0x118), cSpeechSys (0x5560)     (ctor func_002AB200, dtor func_002AB478)
//   cCrowdSys : cMidiSys : cBankSys : virtual cBankManager                    (ctors func_002A69A0 / 0x2A8140 / 0x28FD00)
//   cMusicSys, cSpeechSys : virtual cStreamSys : virtual cBankManager
//   cBankManager : cAsyncSys (0x0), cBankMonitor (0x1D8), virtual cSoundSys; cBankMonitor : cMonitor : virtual cSoundSys
// g++ 2.95 places the shared virtual bases at the end: cSoundSys 0x6C90, cBankManager 0x6C98, cStreamSys 0x7768, and
// gives each class with virtual bases a pointer to them (the mpV* fields; *(this + 0x118) is the usual way to cBankManager:
// **(char***)(self + 0x118) + 0x1D8 is the cBankMonitor). Note: our symbol cController_cController (0x2A8140) is the
// cMidiSys constructor; the real cController constructor is func_002AB200.
//
// A FLAT layout, like main/game.h: every field at its retail offset, bases written out inline, no C++ inheritance.
// Evidence tags:
//   [R] retail constructors (cSSXAudio_cSSXAudio, the base ctors named above), the destructor cSSXAudio__cSSXAudio, tags;
//   [P] the July 2003 preview's out-of-line accessors (retail keeps the same accessors at 0x2BAC30..0x2BADD0);
//   [U] how retail code uses the field (asm of the 220 cSSXAudio methods);
//   [O] SSX On Tour's DWARF (class cSSXAudio, 0x1D00): from mGruntCycle (0x582C) to mPlayIntroMovieWithDJ (0x6C88) retail
//       has On Tour's member order, with retail-only radio/playlist fields after mLastWipeoutRecoverPercent and before
//       mAudioMode. On Tour's bools are 1 byte, retail's are words. cMusicSys/cSpeechSys internals are left raw here.

#include "layout.h"
#include "bx/math3d.h"
#include "sound/musicsys.h"
#include "sound/speechsys.h"
#include "sound/bankmonitor.h"

struct cBankManager;
struct cStreamSys;
struct cSoundSys;

// cCircBuffer (On Tour class, 0x10): a FIFO of ints.
struct cCircBuffer {
    int ArraySize; // 0x0 [O]; [R] cSSXAudio ctor stores 200 into mPlayHistory's
    int* Array;    // 0x4 [O]; [R] operator new(ArraySize * 4, "CircFIFOBuff")
    int Head;      // 0x8 [O]
    int Tail;      // 0xC [O]
};

// One cCrowdSys intensity slot (0x54; [P] cCrowdSys::CROWD_GetData(eCrowdIntensity) returns &mCrowdData[i]).
struct tCrowdData {
    char unk_00[3][0x18]; // 0x00 three 0x18-byte lists, each constructed by func_002A4BA8
    float unk_48;         // 0x48
    float unk_4C;         // 0x4C
    int unk_50;           // 0x50
};

// On Tour's tAttachSoundStruct (0x30), a sound attached to a world instance mesh.
struct tAttachSoundStruct {
    int used;       // 0x00 [O] (On Tour bool); [U] == 1 tests (func_00297EB8)
    void* inst;     // 0x04 [O] tInstanceRecord*
    int mesh;       // 0x08 [O]
    char unk_0C[0x4];
    t4Vector pos;   // 0x10 [O]
    int patch;      // 0x20 [O]
    int handle;     // 0x24 [O]; [U] passed to cBankMonitor_BANKMONITOR_StopHandle (stop)
    char unk_28[0x8];
};

// On Tour's tAvalanchePositionalData (0x20).
struct tAvalanchePositionalData {
    t4Vector m_averagePosition; // 0x00 [O]
    float m_averageScale;       // 0x10 [O]
    float m_distance;           // 0x14 [O]
    int m_numNodes;             // 0x18 [O]
    int m_isRockAvalanche;      // 0x1C [O] (On Tour bool)
};

// On Tour's tSoundVolume (0x10), one volume control.
struct tSoundVolume {
    float setVolume;     // 0x0 [O]
    float curVolume;     // 0x4 [O]; [U] DuckVolume reads it
    float duckedVolume;  // 0x8 [O]; [U] = curVolume * mDuckLevel[j]
    float appliedVolume; // 0xC [O]; [U] = duckedVolume * mMixLevel[j]
};

// On Tour's tAudioMix (0x28), one mix preset read by cSSXAudio_LoadMixes.
struct tAudioMix {
    int musicLevel;         // 0x00 [O]
    int djLevel;            // 0x04 [O]
    int paLevel;            // 0x08 [O]
    int characterLevel;     // 0x0C [O]
    int boardLevel;         // 0x10 [O]
    int collisionLevel;     // 0x14 [O]
    int ambientLevel;       // 0x18 [O]
    int arcadeSFXLevel;     // 0x1C [O]
    int arcadeSpeechLevel;  // 0x20 [O]
    float time;             // 0x24 [O]
};

CHECK_SIZE(cCircBuffer, 0x10);
CHECK_SIZE(tCrowdData, 0x54);
CHECK_OFFSET(tAttachSoundStruct, pos, 0x10);
CHECK_OFFSET(tAttachSoundStruct, patch, 0x20);
CHECK_OFFSET(tAttachSoundStruct, handle, 0x24);
CHECK_SIZE(tAttachSoundStruct, 0x30);
CHECK_SIZE(tAvalanchePositionalData, 0x20);
CHECK_SIZE(tSoundVolume, 0x10);
CHECK_SIZE(tAudioMix, 0x28);

struct cSSXAudio {
    // --- cBankSys (non-virtual part 0x10; ctor cBankSys_cBankSys 0x28FD00, dtor func_0028FEA0; vtable D_00483A48)
    cBankManager* mpVBankManager;       // 0x000 [R] g++ 2.95 virtual-base pointer to the shared cBankManager (= this + 0x6C98); every ctor in the chain stores it; [U] the inlined SFX_Begin/SFX_Play2D path loads it (39 methods)
    int* mBank;                         // 0x004 [O] cBankSys::mBank; [R] cBankSys ctor: operator new(0x10, "mBank")
    int* mPatch;                        // 0x008 [O] cBankSys::mPatch; [R] operator new(0x10, "mPath") (EA's tag spelling)
    void** mVtbl;                       // 0x00C [R] primary vptr: cSSXAudio_vtbl (cSSXAudio ctor/dtor), D_004837F8 (cController/cCrowdSys), D_00483A48 (cBankSys/cMidiSys)
    // --- cMidiSys (EA name from the preview: func_002A82D8 = cMidiSys's virtual slot 1, the dtor; its ctor is the one our symbols call cController_cController 0x2A8140)
    void* mMidiBanks;                   // 0x010 [R] operator new(0x20, "MIDIBANKS") in the cMidiSys ctor (named after the tag); [U] func_002A8450/func_002A86B8
    // --- cCrowdSys (preview: func_002A6B50 = cCrowdSys's dtor; ctor func_002A69A0 stores the instance in D_004A36F8)
    int mNumCrowdInstances;             // 0x014 [R] cCrowdSys ctor 0; [U] func_002A6D78 stores n and allocates n mCrowdInstances; func_002A6D18/func_002A7718/func_002A77C8 loop to it
    void* mCrowdInstances;              // 0x018 [R] cCrowdSys ctor 0; func_002A6D78: operator new[](n, "CrowdInstances"); [U] func_002A72D8..func_002A77C8
    tCrowdData mCrowdData[3];           // 0x01C [R] ctor constructs 3 (func_002A4BA8 x3 each); [P] CROWD_GetData(eCrowdIntensity) returns this + 0x1C + i * 0x54
    // --- cMusicSys (preview: func_002B3398 = cMusicSys's dtor; ctor func_002B28C0(this + 0x118); non-virtual part 0x5448, its methods in sound/bankmonitor.cpp take this + 0x118)
    cMusicSys MusicSys;                 // 0x118 cMusicSys base (sound/musicsys.h; non-virtual part 0x5448)
    // --- cSpeechSys (preview: func_002B0AE8 = cSpeechSys's dtor; ctor func_002B07F8(this + 0x5560) stores the instance in D_004A37B4; non-virtual part 0x1C0)
    cSpeechSys SpeechSys;               // 0x5560 cSpeechSys base (sound/speechsys.h; non-virtual part 0x1C0)
    // --- cController (preview: func_002AB478 = cController's virtual slot 1; ctor func_002AB200 = cCrowdSys + cMusicSys + cSpeechSys)
    int unk_5720;                       // 0x5720 [R] cController ctor 0
    char unk_5724[0x4];                 // 0x5724 padding (cController's non-virtual part ends 8-aligned)
    // --- cSSXAudio (own fields 0x5728..0x6C90; fields from mGruntCycle on follow On Tour's cSSXAudio member order, retail-only fields before it)
    void* unk_5728;                     // 0x5728 [R] ctor 0; dtor closes it with func_003DEDC0(h, 100); [U] func_0029ED90 opens a language stream file into it
    int unk_572C;                       // 0x572C [R] ctor -1; [U] func_0029ED90: language mode of the stream in unk_5728
    float unk_5730;                     // 0x5730 [R] ctor 1.0f; [U] func_0029F000 returns &it as the volume control of speech type 0xB
    int unk_5734;                       // 0x5734 [R] ctor 0; [U] func_0029F660
    int unk_5738;                       // 0x5738 [U] func_0029EEE0 sets it from a speech name prefix (DJ_ 2/11, PA_ 3, Arcade 10, else 4); func_0029F088 returns it
    int unk_573C;                       // 0x573C [U] func_002A2E50
    int unk_5740;                       // 0x5740 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5744;                       // 0x5744 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5748;                       // 0x5748 no retail access seen
    int unk_574C;                       // 0x574C [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5750;                       // 0x5750 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5754;                       // 0x5754 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5758;                       // 0x5758 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_575C;                       // 0x575C [U] func_002A43B8
    int unk_5760;                       // 0x5760 [U] func_002A4550
    int unk_5764;                       // 0x5764 [R] ctor -1
    int unk_5768;                       // 0x5768 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_576C;                       // 0x576C [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5770;                       // 0x5770 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5774;                       // 0x5774 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5778;                       // 0x5778 [U] func_0029C7B0/func_002A3170/func_002A43B8
    int unk_577C;                       // 0x577C [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5780;                       // 0x5780 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5784;                       // 0x5784 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5788;                       // 0x5788 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_578C;                       // 0x578C [R] ctor 0; [U] func_002A2E50, func_002A4A78; retail accessor 0x2BAD48 (lw)
    int unk_5790;                       // 0x5790 [R] ctor 1; [U] func_002A2E50; retail accessor 0x2BAD50 (lw)
    int unk_5794;                       // 0x5794 [R] ctor 1; [U] func_002A26F0
    int unk_5798;                       // 0x5798 [R] ctor -1; [U] func_002A2E50
    int unk_579C[23];                   // 0x579C [R] ctor zeroes 23; [U] func_002867E8 passes its address
    int unk_57F8;                       // 0x57F8 [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_57FC;                       // 0x57FC [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_5800;                       // 0x5800 [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_5804;                       // 0x5804 [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_5808;                       // 0x5808 [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_580C;                       // 0x580C [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_5810;                       // 0x5810 [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_5814;                       // 0x5814 [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_5818;                       // 0x5818 [R] ctor 0; [U] func_002A49E8/func_002A4A38/func_002A4A68
    int unk_581C;                       // 0x581C [R] ctor -1; [U] func_0029B738
    int unk_5820;                       // 0x5820 [R] ctor 0; [U] func_0029B738
    int unk_5824;                       // 0x5824 [U] func_0029C7B0
    int unk_5828;                       // 0x5828 [R] ctor 0
    int mGruntCycle;                    // 0x582C [O] first own member; [U] func_00286E20/func_002872A8 set it, func_002A0560 reads it
    float mWindVolume;                  // 0x5830 [O]; [U] func_002910E0 (BeginWind) passes &it as the stream's volume control, func_002913D8 (EndWind) and func_002929D8 clear it
    int mWindPlaying;                   // 0x5834 [O] (On Tour bool); [U] BeginWind/EndWind
    char mUsedBank[64];                 // 0x5838 [O]; [U] cSSXAudio_FrontEndLoad clears [0], func_0029B818 writes it
    int mNextUsedBank;                  // 0x5878 [O]; [U] cSSXAudio_FrontEndLoad zeroes it
    char mSwapBasePath[128];            // 0x587C [O]; [U] cSSXAudio_FrontEndLoad builds it
    char mAemsSysName[128];             // 0x58FC [O]; placed by order and the cCircBuffer at 0x597C
    cCircBuffer mPlayHistory;           // 0x597C [O]; [R] ctor: ArraySize 200, Array = operator new(800, "CircFIFOBuff"), Head/Tail 0; dtor frees Array; [U] cSSXAudio_PlayHistoryExists..func_00295CC8 (PlayHistory*)
    int mRailType;                      // 0x598C [O]; [P] SetRailSurface(int) (preview 0x5A68; retail accessor 0x2BAD58 sw 0x598C); [U] func_00291710 (Surface) reads it for a rider on a rail
    float mLastCarveEdging[10];         // 0x5990 [O]; [U] func_00293BB0/func_00293DC0 (carve programs) index it
    float mLastGlide[10];               // 0x59B8 [O]; [U] func_00293538/func_00293740/func_00293948 (glide programs)
    int mTurboHandle;                   // 0x59E0 [O]; [U] func_00298D90 (TurboOn) sets it, func_002992D8/func_0029ED18/func_00292B48 stop it
    int mTurboRef;                      // 0x59E4 [O]; [U] TurboOn increments, func_002992D8 decrements
    int mNeedLift[6];                   // 0x59E8 [O] (On Tour bool[6]); [U] func_002947B0, func_002948A0 (ResetLift) clears 6 words
    tAttachSoundStruct AttachedSounds[30]; // 0x5A00 [O] (On Tour's name); [R] ctor's 30-element empty-ctor loop; [U] func_00297950/func_00297EB8/func_00297F70/func_002980B0 walk 30 x 0x30 from 0x5A00
    void* mpAmbienceQuery[2];           // 0x5FA0 [O] tWorldPainterQuery*[2]; [U] func_002867E8 stores, cSSXAudio_InGameDeInit frees
    int mPointsLastValue;               // 0x5FA8 [O]; [U] func_0029A7D8 (PointsEvent)
    int mPointsEventHandle;             // 0x5FAC [O]; [U] func_0029A7D8
    int mSFXPaused;                     // 0x5FB0 [O] (On Tour bool); [R] ctor-time 0; [U] cSSXAudio_PauseSFX (PauseSFX) sets, func_0029CE70 reads; retail accessor 0x2BAD60 (lw)
    int mAudioPaused;                   // 0x5FB4 [O] (On Tour bool); [U] func_00289B70/func_00289BB8 (Pause/ResumeAudio), func_00285BF8 (Update) tests it
    int mCharLandHandle;                // 0x5FB8 [O]; [U] func_002948D0
    int mCharLiftHandle;                // 0x5FBC [O]; [U] cSSXAudio_Lift
    unsigned int mLastPatchRID;         // 0x5FC0 [O] tResourceID (4 bytes); [R] ctor 0xFFFFFFFF; [U] func_002867E8 stores byte 0 (sb), func_0029D290 compares byte 0 (lbu) and stores the word
    int mPlayingTone;                   // 0x5FC4 [O] (On Tour bool); [U] func_0029D370/func_0029D610/func_0029D678 (Start/Update/StopTone)
    int mToneHandle;                    // 0x5FC8 [O]; [R] ctor -1; [U] tone functions
    int mThunderHandle;                 // 0x5FCC [O]; [U] func_00291438
    int mCurBigChallengeType;           // 0x5FD0 [O] eBEAudioType; [U] func_0029D8E0 (BigChallengeBegin) stores it
    int mInBigChallenge;                // 0x5FD4 [O] (On Tour bool); [R] ctor 0; [U] func_0029D8E0, func_0029DBB0
    int mBigChallengeStarting;          // 0x5FD8 [O] (On Tour bool); [R] ctor 0; [U] func_0029D6D0 (SetBigChallengeStarting), func_0029D6E0
    int mUberSpinHandle;                // 0x5FDC [O]; [R] ctor -1; [U] func_0029B0E0/func_0029B3C0 (Start/EndUberSpin)
    int mBoostBarPreviousValue[2];      // 0x5FE0 [O]; [R] ctor zeroes both; [U] func_0029AB40 (BoostBar)
    unsigned short mBoostBarPitchMult[2]; // 0x5FE8 [O]; [U] func_0029AB40
    int mUberSecPreviousValue[2];       // 0x5FEC [O]; [R] ctor zeroes both
    int mAvalancheLoopHandle;           // 0x5FF4 [O]; [U] func_0029DEF0 (Avalanche)
    char unk_5FF8[0x8];                 // 0x5FF8 padding (mAvalancheHitPositions is 16-aligned)
    t4Vector mAvalancheHitPositions[3]; // 0x6000 [O]; [R] ctor's 3-element empty-ctor loop (t4Vector)
    int mAvalancheHitHandles[3];        // 0x6030 [O]; [R] ctor sets 3 x -1
    int mAvalancheHitIndex;             // 0x603C [O]; [R] ctor -1
    int mAvalancheInstanceCount;        // 0x6040 [O]; [R] ctor 0; [U] func_0029DEF0
    char unk_6044[0xC];                 // 0x6044 padding (mAvalancheData is 16-aligned)
    tAvalanchePositionalData mAvalancheData; // 0x6050 [O]; [U] func_0029DEF0/func_0029E4A0 copy it with two sq
    int mpAvalancheRiderIndex;          // 0x6070 [O]; [U] func_0029DEF0/func_0029E4A0
    float mCurTrickPower[2];            // 0x6074 [O]; [U] func_00290F58 (SetCurTrickPower) swc1, func_002871B0 reads [0] and zero-stores both
    int mLastTime;                      // 0x607C [O]; [U] func_0029CCA8
    float mLastWipeoutRecoverPercent[2]; // 0x6080 [O]; [U] func_0029E578/func_0029E590/func_0029E970 (wipeout recover hits)
    char unk_6088[0x4];                 // 0x6088 no retail access seen (On Tour's mPausedSFXForMapScreen sits between mLastWipeoutRecoverPercent and mAudioMode: here or at 0x62AC)
    int unk_608C;                       // 0x608C [R] ctor 0; [U] read by 6 methods (func_00285FB0, func_00286200, func_00287410, func_00287558, func_002899E8, func_0029D6E0), == 2 selects a radio path; retail accessor 0x2BAD68 (lw); probably the preview's GetRadioModeID (0x6134) field
    int unk_6090;                       // 0x6090 [R] ctor 0; [U] func_00285FB0 copies unk_608C into it, func_00286200 tests == 2
    char unk_6094[0x4];                 // 0x6094 no retail access seen (or padding)
    unsigned long mRadioPlayList;       // 0x6098 PORT: 64-bit long; [P] GetRadioPlayList (preview 0x6138, retail 0x2BAD70 ld 0x6098; layout_xlate pair-seen); [R] ctor sd 0
    char unk_60A0[0x4];                 // 0x60A0 no retail access seen (or padding)
    char mPlayListName[20][20];         // 0x60A4 [P] GetPlayListName(int) returns this + 0x60A4 + i * 20 (retail 0x2BAD80; preview 0x6144)
    int mNumPlayLists;                  // 0x6234 [P] GetNumPlayLists (preview 0x62D4, same -0xA0 shift; retail accessor 0x2BAD78); [U] func_00289470 counts them
    int mPlayListIndex;                 // 0x6238 [P] GetPlayListIndex (preview 0x62D8; retail accessor 0x2BAD98); [U] func_00289520 stores it
    char unk_623C[0x14];                // 0x623C no retail access seen (or padding)
    int unk_6250;                       // 0x6250 [R] ctor 0
    int unk_6254;                       // 0x6254 [R] ctor 0; [U] func_002A4770; retail accessor 0x2BAD30 sets it to 1
    int unk_6258;                       // 0x6258 [U] func_002A2E50
    void* unk_625C[2];                  // 0x625C [U] func_002867E8 (InGameInit) creates two objects, cSSXAudio_InGameDeInit (InGameDeInit) frees them
    int unk_6264;                       // 0x6264 [U] func_002867E8 stores
    int unk_6268;                       // 0x6268 [U] func_002867E8 stores
    int unk_626C;                       // 0x626C [U] func_002867E8 stores
    int unk_6270;                       // 0x6270 [U] func_002867E8 stores
    int unk_6274;                       // 0x6274 [U] func_002867E8 stores; maybe the preview's PathfinderCanChangeVolume (0x6314 - 0xA0)
    char unk_6278[0x4];                 // 0x6278 no retail access seen (maybe the preview's SetTransportTrack field, 0x6318 - 0xA0)
    int unk_627C;                       // 0x627C [R] ctor 1; [U] cSSXAudio_FrontEndLoad, func_0029C420, func_0029C7B0
    int unk_6280;                       // 0x6280 retail accessor 0x2BADA0 (lw)
    int unk_6284;                       // 0x6284 [R] ctor 23
    int unk_6288;                       // 0x6288 [U] func_002867E8
    char unk_628C[0x4];                 // 0x628C no retail access seen (or padding)
    int unk_6290;                       // 0x6290 [U] func_00285FB0 stores 10
    int unk_6294;                       // 0x6294 [R] ctor 0
    int unk_6298;                       // 0x6298 [R] ctor 0; [U] func_0029C7B0
    char unk_629C[0x8];                 // 0x629C no retail access seen (or padding)
    int unk_62A4;                       // 0x62A4 [R] ctor -1
    int unk_62A8;                       // 0x62A8 [R] ctor -1; [U] func_002974A0, func_00297950
    char unk_62AC[0x4];                 // 0x62AC no retail access seen (see 0x6088)
    int mAudioMode;                     // 0x62B0 [O] eSOUNDDEF_MODE; [R] ctor stores its audioMode argument; [U] cSSXAudio_SetMode stores, func_00285930 and func_00289DC0 (GetMode) read
    int mArcadeSFX;                     // 0x62B4 [O] (On Tour bool, after mAudioMode); [R] ctor 0; [U] func_002875D0 (SetArcadeSFX) / func_00287488 (GetArcadeSFX)
    int unk_62B8;                       // 0x62B8 [R] ctor 0; [U] func_00287558 (SetSFXVolume) stores, func_00287410 (GetSFXVolume) reads
    tSoundVolume mSoundVolume[11];      // 0x62BC [O]; [U] func_00287A10 (DuckVolume): entry j curVolume * duck -> duckedVolume, * mMixLevel[j] -> appliedVolume
    float unk_636C[10];                 // 0x636C [U] DuckVolume/MixVolume (func_00287A10/func_00287C48/func_002883B0/func_002887F8): mSoundVolume[4].appliedVolume times constants D_004A352C..
    float mDuckLevel[11];               // 0x6394 [O]; [R] ctor 11 x 1.0f; [U] DuckVolume writes [j]
    int mDucking;                       // 0x63C0 [O] (On Tour bool); [R] ctor 0; [U] DuckVolume sets it when fading
    unsigned int mCurDuck;              // 0x63C4 [O]; [U] DuckVolume stores the mask
    float mDuckStep;                    // 0x63C8 [O]; [U] DuckVolume
    float mDuckFinalValue;              // 0x63CC [O]; [U] DuckVolume
    float mDuckCurValue;                // 0x63D0 [O]; [R] ctor 1.0f; [U] DuckVolume
    int mDuckStream;                    // 0x63D4 [O]; [R] ctor 3; [U] func_00285BF8
    float mDuckScale;                   // 0x63D8 [O]; [R] ctor 1.0f
    int mStreamIsDucked;                // 0x63DC [O] (On Tour bool); [R] ctor 0; [U] func_00287F00
    int mDuckedStreamHasStarted;        // 0x63E0 [O] (On Tour bool); [U] func_00285BF8
    float mMixLevel[11];                // 0x63E4 [O]; [R] ctor 11 x 1.0f; [U] DuckVolume, func_002883B0
    int mMixing;                        // 0x6410 [O] (On Tour bool); [U] func_00288370, func_002887F8
    float mMixStep[11];                 // 0x6414 [O]; [U] func_002887F8
    float mMixFinalValue[11];           // 0x6440 [O]; placed by order
    int mLastMix;                       // 0x646C [O]; [R] ctor -1; [U] func_002883B0 (MixVolume)
    int mPreset;                        // 0x6470 [O]; [P] GetPreset (preview 0x64DC; retail accessor 0x2BADC0 lw 0x6470); [U] func_00288F60 (SetPreset), func_00289398/func_002893C8
    int mAudioUpdateState;              // 0x6474 [O]; [P] GetAudioUpdateState (preview 0x64E4; retail accessor 0x2BADC8); [U] func_00285BE0 (SetAudioUpdateState), func_00285BF8
    int mNumAudioMixes;                 // 0x6478 [O]; [U] cSSXAudio_LoadMixes
    tAudioMix mAudioMixes[25];          // 0x647C [O]; [U] cSSXAudio_LoadMixes fills 25 x 0x28
    int mRMSFrameTick;                  // 0x6864 [O]; [U] func_00289688
    char mRMSData[1000];                // 0x6868 [O]; placed by order (0x6868 + 1000 = mRMSSize)
    int mRMSSize;                       // 0x6C50 [O]; [U] func_00289688
    int mRMSDataFrameRate;              // 0x6C54 [O]; [R] ctor 60
    int mValidRMSData;                  // 0x6C58 [O] (On Tour bool); [R] ctor 0; [U] func_00289688/func_00289880
    float mLastRMSDuckLevel;            // 0x6C5C [O]; [U] func_00289830
    int mRMSStepFrame;                  // 0x6C60 [O]; [R] ctor 1
    float mRMSStepValue;                // 0x6C64 [O]; [U] func_00289688/func_00289880
    float mRMSCurValue;                 // 0x6C68 [O]
    float mRMSFinalValue;               // 0x6C6C [O]
    int mTrackBankLoaded[2];            // 0x6C70 [O] (On Tour bool[2]); [R] ctor zeroes both; [U] func_0029D370 reads [1]
    int mTrackBankLoadedSection[2];     // 0x6C78 [O]; [R] ctor sets both -1
    void* mpMixQuery[2];                // 0x6C80 [O] tWorldPainterQuery*[2]; [U] func_002867E8 stores, cSSXAudio_InGameDeInit frees
    int mPlayIntroMovieWithDJ;          // 0x6C88 [O] (On Tour bool, the last member); [R] ctor 1; [U] func_00289DE0
    char unk_6C8C[0x4];                 // 0x6C8C no retail access seen (or padding)
    // --- virtual base cSoundSys (0x8; ctor cSoundSys_cSoundSys(this + 0x6C90))
    void* pManager;                     // 0x6C90 [O] cSoundSys::pManager; [R] cSoundSys ctor stores func_002A8FA0() (the PS2 sound manager)
    void** mSoundSysVtbl;               // 0x6C94 [R] cSoundSys vptr; the ctors in the chain point it at their stack-patched copies
    // --- virtual base cBankManager (0xAD0; ctor cBankManager_cBankManager(this + 0x6C98)): cAsyncSys at 0, cBankMonitor at 0x1D8
    char mAsyncSys[468];                // 0x6C98 cAsyncSys data (On Tour 0x1D4: mHandle .. mBufferSize; ctor func_0028A058)
    void** mBankManagerVtbl;            // 0x6E6C [R] cBankManager ctor stores D_00483AD8 (cAsyncSys's vptr slot, cBankManager+0x1D4)
    cBankMonitor BankMonitor;           // 0x6E70 cBankManager's cBankMonitor (sound/bankmonitor.h; non-virtual part 0x8F0)
    int mMaxBanks;                      // 0x7760 [O] cBankManager::mMaxBanks; [R] cBankManager ctor stores its count argument
    void* mBankManagerBank;             // 0x7764 [O] cBankManager::mBank (renamed: cBankSys::mBank at 0x4); [R] operator new[](count x 0x60)
    // --- virtual base cStreamSys (0xC; ctor cStreamSys_cStreamSys(this + 0x7768))
    cBankManager* mpStreamVBankManager; // 0x7768 [R] cStreamSys's virtual-base pointer (= this + 0x6C98)
    int mNumStreams;                    // 0x776C [O]; [R] cStreamSys ctor stores its count
    void** mStreamInstance;             // 0x7770 [O] cStreamInstance**; [R] operator new[](count x 4, "StreamArray")
    char unk_7774[0xC];                 // 0x7774 tail padding (the class is 16-aligned: t4Vector members)
};

CHECK_OFFSET(cSSXAudio, mpVBankManager, 0x0);
CHECK_OFFSET(cSSXAudio, mBank, 0x4);
CHECK_OFFSET(cSSXAudio, mPatch, 0x8);
CHECK_OFFSET(cSSXAudio, mVtbl, 0xC);
CHECK_OFFSET(cSSXAudio, mMidiBanks, 0x10);
CHECK_OFFSET(cSSXAudio, mNumCrowdInstances, 0x14);
CHECK_OFFSET(cSSXAudio, mCrowdInstances, 0x18);
CHECK_OFFSET(cSSXAudio, mCrowdData, 0x1C);
CHECK_OFFSET(cSSXAudio, MusicSys, 0x118);
CHECK_OFFSET(cSSXAudio, SpeechSys, 0x5560);
CHECK_OFFSET(cSSXAudio, unk_5720, 0x5720);
CHECK_OFFSET(cSSXAudio, unk_5724, 0x5724);
CHECK_OFFSET(cSSXAudio, unk_5728, 0x5728);
CHECK_OFFSET(cSSXAudio, unk_572C, 0x572C);
CHECK_OFFSET(cSSXAudio, unk_5730, 0x5730);
CHECK_OFFSET(cSSXAudio, unk_5734, 0x5734);
CHECK_OFFSET(cSSXAudio, unk_5738, 0x5738);
CHECK_OFFSET(cSSXAudio, unk_573C, 0x573C);
CHECK_OFFSET(cSSXAudio, unk_5740, 0x5740);
CHECK_OFFSET(cSSXAudio, unk_5744, 0x5744);
CHECK_OFFSET(cSSXAudio, unk_5748, 0x5748);
CHECK_OFFSET(cSSXAudio, unk_574C, 0x574C);
CHECK_OFFSET(cSSXAudio, unk_5750, 0x5750);
CHECK_OFFSET(cSSXAudio, unk_5754, 0x5754);
CHECK_OFFSET(cSSXAudio, unk_5758, 0x5758);
CHECK_OFFSET(cSSXAudio, unk_575C, 0x575C);
CHECK_OFFSET(cSSXAudio, unk_5760, 0x5760);
CHECK_OFFSET(cSSXAudio, unk_5764, 0x5764);
CHECK_OFFSET(cSSXAudio, unk_5768, 0x5768);
CHECK_OFFSET(cSSXAudio, unk_576C, 0x576C);
CHECK_OFFSET(cSSXAudio, unk_5770, 0x5770);
CHECK_OFFSET(cSSXAudio, unk_5774, 0x5774);
CHECK_OFFSET(cSSXAudio, unk_5778, 0x5778);
CHECK_OFFSET(cSSXAudio, unk_577C, 0x577C);
CHECK_OFFSET(cSSXAudio, unk_5780, 0x5780);
CHECK_OFFSET(cSSXAudio, unk_5784, 0x5784);
CHECK_OFFSET(cSSXAudio, unk_5788, 0x5788);
CHECK_OFFSET(cSSXAudio, unk_578C, 0x578C);
CHECK_OFFSET(cSSXAudio, unk_5790, 0x5790);
CHECK_OFFSET(cSSXAudio, unk_5794, 0x5794);
CHECK_OFFSET(cSSXAudio, unk_5798, 0x5798);
CHECK_OFFSET(cSSXAudio, unk_579C, 0x579C);
CHECK_OFFSET(cSSXAudio, unk_57F8, 0x57F8);
CHECK_OFFSET(cSSXAudio, unk_57FC, 0x57FC);
CHECK_OFFSET(cSSXAudio, unk_5800, 0x5800);
CHECK_OFFSET(cSSXAudio, unk_5804, 0x5804);
CHECK_OFFSET(cSSXAudio, unk_5808, 0x5808);
CHECK_OFFSET(cSSXAudio, unk_580C, 0x580C);
CHECK_OFFSET(cSSXAudio, unk_5810, 0x5810);
CHECK_OFFSET(cSSXAudio, unk_5814, 0x5814);
CHECK_OFFSET(cSSXAudio, unk_5818, 0x5818);
CHECK_OFFSET(cSSXAudio, unk_581C, 0x581C);
CHECK_OFFSET(cSSXAudio, unk_5820, 0x5820);
CHECK_OFFSET(cSSXAudio, unk_5824, 0x5824);
CHECK_OFFSET(cSSXAudio, unk_5828, 0x5828);
CHECK_OFFSET(cSSXAudio, mGruntCycle, 0x582C);
CHECK_OFFSET(cSSXAudio, mWindVolume, 0x5830);
CHECK_OFFSET(cSSXAudio, mWindPlaying, 0x5834);
CHECK_OFFSET(cSSXAudio, mUsedBank, 0x5838);
CHECK_OFFSET(cSSXAudio, mNextUsedBank, 0x5878);
CHECK_OFFSET(cSSXAudio, mSwapBasePath, 0x587C);
CHECK_OFFSET(cSSXAudio, mAemsSysName, 0x58FC);
CHECK_OFFSET(cSSXAudio, mPlayHistory, 0x597C);
CHECK_OFFSET(cSSXAudio, mRailType, 0x598C);
CHECK_OFFSET(cSSXAudio, mLastCarveEdging, 0x5990);
CHECK_OFFSET(cSSXAudio, mLastGlide, 0x59B8);
CHECK_OFFSET(cSSXAudio, mTurboHandle, 0x59E0);
CHECK_OFFSET(cSSXAudio, mTurboRef, 0x59E4);
CHECK_OFFSET(cSSXAudio, mNeedLift, 0x59E8);
CHECK_OFFSET(cSSXAudio, AttachedSounds, 0x5A00);
CHECK_OFFSET(cSSXAudio, mpAmbienceQuery, 0x5FA0);
CHECK_OFFSET(cSSXAudio, mPointsLastValue, 0x5FA8);
CHECK_OFFSET(cSSXAudio, mPointsEventHandle, 0x5FAC);
CHECK_OFFSET(cSSXAudio, mSFXPaused, 0x5FB0);
CHECK_OFFSET(cSSXAudio, mAudioPaused, 0x5FB4);
CHECK_OFFSET(cSSXAudio, mCharLandHandle, 0x5FB8);
CHECK_OFFSET(cSSXAudio, mCharLiftHandle, 0x5FBC);
CHECK_OFFSET(cSSXAudio, mLastPatchRID, 0x5FC0);
CHECK_OFFSET(cSSXAudio, mPlayingTone, 0x5FC4);
CHECK_OFFSET(cSSXAudio, mToneHandle, 0x5FC8);
CHECK_OFFSET(cSSXAudio, mThunderHandle, 0x5FCC);
CHECK_OFFSET(cSSXAudio, mCurBigChallengeType, 0x5FD0);
CHECK_OFFSET(cSSXAudio, mInBigChallenge, 0x5FD4);
CHECK_OFFSET(cSSXAudio, mBigChallengeStarting, 0x5FD8);
CHECK_OFFSET(cSSXAudio, mUberSpinHandle, 0x5FDC);
CHECK_OFFSET(cSSXAudio, mBoostBarPreviousValue, 0x5FE0);
CHECK_OFFSET(cSSXAudio, mBoostBarPitchMult, 0x5FE8);
CHECK_OFFSET(cSSXAudio, mUberSecPreviousValue, 0x5FEC);
CHECK_OFFSET(cSSXAudio, mAvalancheLoopHandle, 0x5FF4);
CHECK_OFFSET(cSSXAudio, unk_5FF8, 0x5FF8);
CHECK_OFFSET(cSSXAudio, mAvalancheHitPositions, 0x6000);
CHECK_OFFSET(cSSXAudio, mAvalancheHitHandles, 0x6030);
CHECK_OFFSET(cSSXAudio, mAvalancheHitIndex, 0x603C);
CHECK_OFFSET(cSSXAudio, mAvalancheInstanceCount, 0x6040);
CHECK_OFFSET(cSSXAudio, unk_6044, 0x6044);
CHECK_OFFSET(cSSXAudio, mAvalancheData, 0x6050);
CHECK_OFFSET(cSSXAudio, mpAvalancheRiderIndex, 0x6070);
CHECK_OFFSET(cSSXAudio, mCurTrickPower, 0x6074);
CHECK_OFFSET(cSSXAudio, mLastTime, 0x607C);
CHECK_OFFSET(cSSXAudio, mLastWipeoutRecoverPercent, 0x6080);
CHECK_OFFSET(cSSXAudio, unk_6088, 0x6088);
CHECK_OFFSET(cSSXAudio, unk_608C, 0x608C);
CHECK_OFFSET(cSSXAudio, unk_6090, 0x6090);
CHECK_OFFSET(cSSXAudio, unk_6094, 0x6094);
CHECK_OFFSET(cSSXAudio, mRadioPlayList, 0x6098);
CHECK_OFFSET(cSSXAudio, unk_60A0, 0x60A0);
CHECK_OFFSET(cSSXAudio, mPlayListName, 0x60A4);
CHECK_OFFSET(cSSXAudio, mNumPlayLists, 0x6234);
CHECK_OFFSET(cSSXAudio, mPlayListIndex, 0x6238);
CHECK_OFFSET(cSSXAudio, unk_623C, 0x623C);
CHECK_OFFSET(cSSXAudio, unk_6250, 0x6250);
CHECK_OFFSET(cSSXAudio, unk_6254, 0x6254);
CHECK_OFFSET(cSSXAudio, unk_6258, 0x6258);
CHECK_OFFSET(cSSXAudio, unk_625C, 0x625C);
CHECK_OFFSET(cSSXAudio, unk_6264, 0x6264);
CHECK_OFFSET(cSSXAudio, unk_6268, 0x6268);
CHECK_OFFSET(cSSXAudio, unk_626C, 0x626C);
CHECK_OFFSET(cSSXAudio, unk_6270, 0x6270);
CHECK_OFFSET(cSSXAudio, unk_6274, 0x6274);
CHECK_OFFSET(cSSXAudio, unk_6278, 0x6278);
CHECK_OFFSET(cSSXAudio, unk_627C, 0x627C);
CHECK_OFFSET(cSSXAudio, unk_6280, 0x6280);
CHECK_OFFSET(cSSXAudio, unk_6284, 0x6284);
CHECK_OFFSET(cSSXAudio, unk_6288, 0x6288);
CHECK_OFFSET(cSSXAudio, unk_628C, 0x628C);
CHECK_OFFSET(cSSXAudio, unk_6290, 0x6290);
CHECK_OFFSET(cSSXAudio, unk_6294, 0x6294);
CHECK_OFFSET(cSSXAudio, unk_6298, 0x6298);
CHECK_OFFSET(cSSXAudio, unk_629C, 0x629C);
CHECK_OFFSET(cSSXAudio, unk_62A4, 0x62A4);
CHECK_OFFSET(cSSXAudio, unk_62A8, 0x62A8);
CHECK_OFFSET(cSSXAudio, unk_62AC, 0x62AC);
CHECK_OFFSET(cSSXAudio, mAudioMode, 0x62B0);
CHECK_OFFSET(cSSXAudio, mArcadeSFX, 0x62B4);
CHECK_OFFSET(cSSXAudio, unk_62B8, 0x62B8);
CHECK_OFFSET(cSSXAudio, mSoundVolume, 0x62BC);
CHECK_OFFSET(cSSXAudio, unk_636C, 0x636C);
CHECK_OFFSET(cSSXAudio, mDuckLevel, 0x6394);
CHECK_OFFSET(cSSXAudio, mDucking, 0x63C0);
CHECK_OFFSET(cSSXAudio, mCurDuck, 0x63C4);
CHECK_OFFSET(cSSXAudio, mDuckStep, 0x63C8);
CHECK_OFFSET(cSSXAudio, mDuckFinalValue, 0x63CC);
CHECK_OFFSET(cSSXAudio, mDuckCurValue, 0x63D0);
CHECK_OFFSET(cSSXAudio, mDuckStream, 0x63D4);
CHECK_OFFSET(cSSXAudio, mDuckScale, 0x63D8);
CHECK_OFFSET(cSSXAudio, mStreamIsDucked, 0x63DC);
CHECK_OFFSET(cSSXAudio, mDuckedStreamHasStarted, 0x63E0);
CHECK_OFFSET(cSSXAudio, mMixLevel, 0x63E4);
CHECK_OFFSET(cSSXAudio, mMixing, 0x6410);
CHECK_OFFSET(cSSXAudio, mMixStep, 0x6414);
CHECK_OFFSET(cSSXAudio, mMixFinalValue, 0x6440);
CHECK_OFFSET(cSSXAudio, mLastMix, 0x646C);
CHECK_OFFSET(cSSXAudio, mPreset, 0x6470);
CHECK_OFFSET(cSSXAudio, mAudioUpdateState, 0x6474);
CHECK_OFFSET(cSSXAudio, mNumAudioMixes, 0x6478);
CHECK_OFFSET(cSSXAudio, mAudioMixes, 0x647C);
CHECK_OFFSET(cSSXAudio, mRMSFrameTick, 0x6864);
CHECK_OFFSET(cSSXAudio, mRMSData, 0x6868);
CHECK_OFFSET(cSSXAudio, mRMSSize, 0x6C50);
CHECK_OFFSET(cSSXAudio, mRMSDataFrameRate, 0x6C54);
CHECK_OFFSET(cSSXAudio, mValidRMSData, 0x6C58);
CHECK_OFFSET(cSSXAudio, mLastRMSDuckLevel, 0x6C5C);
CHECK_OFFSET(cSSXAudio, mRMSStepFrame, 0x6C60);
CHECK_OFFSET(cSSXAudio, mRMSStepValue, 0x6C64);
CHECK_OFFSET(cSSXAudio, mRMSCurValue, 0x6C68);
CHECK_OFFSET(cSSXAudio, mRMSFinalValue, 0x6C6C);
CHECK_OFFSET(cSSXAudio, mTrackBankLoaded, 0x6C70);
CHECK_OFFSET(cSSXAudio, mTrackBankLoadedSection, 0x6C78);
CHECK_OFFSET(cSSXAudio, mpMixQuery, 0x6C80);
CHECK_OFFSET(cSSXAudio, mPlayIntroMovieWithDJ, 0x6C88);
CHECK_OFFSET(cSSXAudio, unk_6C8C, 0x6C8C);
CHECK_OFFSET(cSSXAudio, pManager, 0x6C90);
CHECK_OFFSET(cSSXAudio, mSoundSysVtbl, 0x6C94);
CHECK_OFFSET(cSSXAudio, mAsyncSys, 0x6C98);
CHECK_OFFSET(cSSXAudio, mBankManagerVtbl, 0x6E6C);
CHECK_OFFSET(cSSXAudio, BankMonitor, 0x6E70);
CHECK_OFFSET(cSSXAudio, mMaxBanks, 0x7760);
CHECK_OFFSET(cSSXAudio, mBankManagerBank, 0x7764);
CHECK_OFFSET(cSSXAudio, mpStreamVBankManager, 0x7768);
CHECK_OFFSET(cSSXAudio, mNumStreams, 0x776C);
CHECK_OFFSET(cSSXAudio, mStreamInstance, 0x7770);
CHECK_OFFSET(cSSXAudio, unk_7774, 0x7774);
CHECK_SIZE(cSSXAudio, 0x7780);

// The one audio system, created by SSXAUDIO_Init.
extern cSSXAudio* gSSXAudio;

#endif /* SOUND_SSXAUDIO_H */
