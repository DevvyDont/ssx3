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
//   cController : cCrowdSys (0x0), cMusicSys (0x118), cSpeechSys (0x5560)     (ctor cController_cController, dtor cController__cController)
//   cCrowdSys : cMidiSys : cBankSys : virtual cBankManager                    (ctors cCrowdSys_cCrowdSys / 0x2A8140 / 0x28FD00)
//   cMusicSys, cSpeechSys : virtual cStreamSys : virtual cBankManager
//   cBankManager : cAsyncSys (0x0), cBankMonitor (0x1D8), virtual cSoundSys; cBankMonitor : cMonitor : virtual cSoundSys
// g++ 2.95 places the shared virtual bases at the end: cSoundSys 0x6C90, cBankManager 0x6C98, cStreamSys 0x7768, and
// gives each class with virtual bases a pointer to them (the mpV* fields; *(this + 0x118) is the usual way to cBankManager:
// **(char***)(self + 0x118) + 0x1D8 is the cBankMonitor). The cMidiSys constructor is cMidiSys_cMidiSys (0x2A8140,
// formerly misnamed cController_cController); the real cController constructor is cController_cController.
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
#include "sound/asyncsys.h" // cCircBuffer
#include "sound/soundsys.h" // eSOUNDDEF_MODE, eSOUNDMANAGER_SETMODE
#include "bx/math3d.h"
#include "sound/musicsys.h"
#include "sound/speechsys.h"
#include "sound/bankmonitor.h"
#include "sound/crowdsys.h"

struct cBankManager;
struct cStreamSys;
struct cSoundSys;
struct tInstanceRecord;
struct tWorldPainterQuery; // worldpainter/worldpainterquery.h

// AudioTable: a 5-point piecewise-linear curve (keys x, values y) for volumes and pitches. [O] On Tour struct
// AudioTable (0x28, all_in_one_sound.cpp) with TABLE_Scale(AudioTable* table, float input) and TABLE_Scale(int
// tableIndex, float input) over AudioTable gAudioTables[4]; [R] retail's pair has the same shapes: func_00290B58 walks
// x[0..4] and interpolates y (TABLE_Scale(AudioTable*, float)), func_00290C10(i, v) = func_00290B58(&gAudioTables[i], v)
// (gAudioTables = 0x00445898); [U] the cSSXAudio *Program methods pass their curves (D_00482AF8..D_00482DA0) to it.
struct AudioTable {
    float x[5];                     // 0x00 [O]; [U] the keys, ascending
    float y[5];                     // 0x14 [O]; [U] the value at each key
};

CHECK_OFFSET(AudioTable, x, 0x00);
CHECK_OFFSET(AudioTable, y, 0x14);
CHECK_SIZE(AudioTable, 0x28);

// On Tour's tAttachSoundStruct (0x30), a sound attached to a world instance mesh.
struct tAttachSoundStruct {
    // mesh when the sound follows the instance's model instead of one mesh (name ours). [U] cSSXAudio_AttachSound
    // takes the model's root transform for -1, cSSXAudio_UpdateAttachedSounds tests it the same way.
    enum { kModelMesh = -1 };
    int used;       // 0x00 [O] (On Tour bool); [U] == 1 tests (cSSXAudio_DetachSound)
    tInstanceRecord* inst; // 0x04 [O]; [U] cSSXAudio_AttachSound stores it, cSSXAudio_UpdateAttachedSounds follows
                           //      its m_pActiveNode
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
    // A level that keeps the control where it is (name ours). [U] cSSXAudio_MixIntToFloat returns the control's current
    // level for a negative value, and cSSXAudio_MixVolume fades only the controls whose level is not -1.
    enum { kKeepLevel = -1 };

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

CHECK_OFFSET(tAttachSoundStruct, pos, 0x10);
CHECK_OFFSET(tAttachSoundStruct, patch, 0x20);
CHECK_OFFSET(tAttachSoundStruct, handle, 0x24);
CHECK_SIZE(tAttachSoundStruct, 0x30);
CHECK_SIZE(tAvalanchePositionalData, 0x20);
CHECK_SIZE(tSoundVolume, 0x10);
CHECK_SIZE(tAudioMix, 0x28);

struct cBankMonitorInstance;
struct cRider;
struct cWipeoutControl;

struct SNDPLAYOPTS;
struct t4Vector;
struct tWPInGameData;

// A volume control (the index into cSSXAudio::mSoundVolume, mDuckLevel, mMixLevel). [O] On Tour's enum eVolumeControl,
// names and values; [U] cSSXAudio_SetCurrVolume scales 1..10 by gMusicScaling, gDJSpchScaling, gPASpchScaling,
// gCharSpchScaling, gSFXScaling, gBoardScaling, gCollisionScaling, gAmbientScaling, gArcadeSFXScaling and
// gArcadeSpeechScaling, sets the library master volume for 0 and the per-character levels for 4.
enum eVolumeControl {
    keVolumeControl_All = -1, keVolumeControl_Master = 0, keVolumeControl_Music = 1, keVolumeControl_DJ = 2,
    keVolumeControl_PA = 3, keVolumeControl_Character = 4, keVolumeControl_SFX = 5, keVolumeControl_Board = 6,
    keVolumeControl_Collision = 7, keVolumeControl_Ambient = 8, keVolumeControl_ArcadeSFX = 9,
    keVolumeControl_ArcadeSpeech = 10, keVolumeControl_Num = 11
};

// What the audio system updates each frame (cSSXAudio::mAudioUpdateState). [O] On Tour's enum eUpdateState, names and
// values; [U] cSSXAudio_FrontEndLoad sets 1, FrontEndPurge / InGameDeInit 0, StartEvent 6, and cSSXAudio_Update runs
// the in-game updates only in 6.
enum eUpdateState {
    DEFAULT_UPDATE = 0, FE_UPDATE = 1, MUSIC_UPDATE = 2, PREGAME_UPDATE = 3, PRERACE_UPDATE = 4, PRECOUNT_UPDATE = 5,
    INGAME_UPDATE = 6
};

// cSSXAudio::mDuckStream with no stream ducking the others (name ours). [R] ctor 3; [U] cSSXAudio_DuckToStream(3)
// restores the ducked controls and stores 3, cSSXAudio_Update watches the stream only while it is not 3.
enum { kNoDuckStream = 3 };

// cSSXAudio_GetFocusRider's two special arguments (names ours). [U] -1 picks the local human rider with the best
// RaceRanking, -2 the first local human; any other value is a viewport.
enum { kFocusLeadingHuman = -1, kFocusFirstHuman = -2 };

// The sound bank of the loading-screen loop (name ours). [U] func_0028F768 plays patch 0 of bank 13, and
// cSSXAudio_InGamePurge unloads every bank below 17 except 13.
enum { kLoadingLoopBank = 13 };

// A sound preset (cSSXAudio::mPreset; names ours, after what cSSXAudio_SetPreset does with it). [U] 0 sets every
// control 1..10 to 10/11, 1 silences the speech controls (DJ, PA, Character, ArcadeSFX, ArcadeSpeech), 2 the music,
// 3 keeps only the effects (SFX, Board, Collision, Ambient), 4 keeps only the music, 5 silences everything; 4 and 5
// also turn the arcade sounds off. cSSXAudio_NextPreset / PrevPreset wrap between 0 and 5.
enum eSoundPreset {
    kSOUNDPRESET_All = 0, kSOUNDPRESET_NoSpeech = 1, kSOUNDPRESET_NoMusic = 2, kSOUNDPRESET_EffectsOnly = 3,
    kSOUNDPRESET_MusicOnly = 4, kSOUNDPRESET_Silent = 5, kSOUNDPRESET_Num = 6
};

// What cSSXAudio_InitSoundLevels sets the level scales for. [O] On Tour's enum eGameAudioStates, names and values; [U]
// cSSXAudio_FrontEndLoad passes 0 (all scales 1), cSSXAudio_InGameLoad 1 (the in-game balance).
enum eGameAudioStates { eGameAudioState_FE = 0, eGameAudioState_Mountain = 1 };

// A grunt's length (cSSXAudio_PlayGrunt's length). [O] On Tour's enum eCharGruntLength, names and values; [U] PlayGrunt
// picks the character's odd patch for Long and its even one otherwise, and for character kind 14 a random patch of
// 0-2 for Long, 3-5 for Short and the next of six (mGruntPatchCycle) for any other length.
enum eCharGruntLength { keCharGruntLength_Short = 0, keCharGruntLength_Long = 1, keCharGruntLength_Cycle = 2 };

// The animation sound tags cSSXAudio_PlayAniTag handles (names ours, after the call each makes): [U] 80
// cSSXAudio_BindingsOff, 81 cSSXAudio_BindingsOn, 82 cSSXAudio_UndoBindings.
enum { kANITAG_BindingsOff = 80, kANITAG_BindingsOn = 81, kANITAG_UndoBindings = 82 };

// Which glide / carve program a surface uses (cSSXAudio_GetSurfaceProgramType's result). [O] On Tour's enum
// eSurfaceProgramType, names and values; [U] cSSXAudio_GlideProgram, AIGlideProgram and CarveProgram run their Hard*,
// Soft* and Rail* programs for 0, 1 and 2.
enum eSurfaceProgramType { keSurfaceProgram_Hard = 0, keSurfaceProgram_Soft = 1, keSurfaceProgram_Rail = 2 };

// A menu sound cue (cSSXAudio_MenuSound's argument). [O] On Tour's enum eMenuSoundType, names and values; [U] retail's
// cSSXAudio_MenuSound plays the same sound for each front-end cue and its in-game twin (0 / 9 .. 4 / 13), nothing for
// 5 and 6, and its own sounds for 7, 8, 14 and 15.
enum eMenuSoundType {
    keMenuSoundType_FEForward = 0, keMenuSoundType_FEBack = 1, keMenuSoundType_FELeftRight = 2,
    keMenuSoundType_FEUpDown = 3, keMenuSoundType_FEError = 4, keMenuSoundType_FEScrollText = 5,
    keMenuSoundType_FESelectText = 6, keMenuSoundType_FEChallenge = 7, keMenuSoundType_FEMail = 8,
    keMenuSoundType_IGForward = 9, keMenuSoundType_IGBack = 10, keMenuSoundType_IGLeftRight = 11,
    keMenuSoundType_IGUpDown = 12, keMenuSoundType_IGError = 13, keMenuSoundType_IGOverlayPopup = 14,
    keMenuSoundType_SnowSwoosh = 15, keMenuSoundType_Num = 16
};
// cSSXAudio_MenuSound's sound id for a cue without a sound (name ours). [U] it starts at -1, keeps it for
// keMenuSoundType_FEScrollText / FESelectText and plays nothing when it is still -1.
enum { kNoMenuSoundID = -1 };

// A pickup icon (cSSXAudio_PlayIconSound's kind). [O] On Tour's enum eIconType, names and values; [U] retail plays a
// sound per kind 0..3 and, for 2, a trick-multiplier line by the icon's multiplier (2, 3, 5 or 10).
enum eIconType {
    keIcon_SpeedBoost = 0, keIcon_TrickBoost = 1, keIcon_TrickMultiplier = 2, keIcon_Points = 3, keIcon_Mission = 4,
    keIcon_Collectable = 5
};

// The speech language as a bit (func_0029F198's result, cSSXAudio::unk_572C; names ours). [U] func_0029ED90 opens
// data/audio/french.big for 2, german.big for 4, spanish.big for 8 and english.big otherwise; the speech hooks get it
// as the request's language bit.
enum eSpeechLanguageBit {
    kSPEECHLANG_English = 1, kSPEECHLANG_French = 2, kSPEECHLANG_German = 4, kSPEECHLANG_Spanish = 8
};

// The options' language index (eOptionSpeechLanguage, tBEOptionDB::iLanguage) lives in be/beintoption.h.

// The radio mode (cSSXAudio::mRadioModeID). [O] On Tour's enum eRadioModeID, names and values. [U] func_0028BF78 plays
// the default song map in 0, the radio playlist (mRadioPlayList) in 1 and 3, and the peak ambience in 2
// (cSSXAudio_NeedWPAmbience tests 2); func_0028D960 lets the music system pick songs in 0, 1 and 3.
enum eRadioModeID {
    kRADIOMODE_RADIO = 0, kRADIOMODE_REQUESTLINE = 1, kRADIOMODE_MOUNTAIN = 2, kRADIOMODE_PLAYLIST = 3,
    kRADIOMODE_NUM = 4
};

// The peak of a race location as func_002A1E20 returns it, one bit per peak (names ours). [U] func_002A1E20 maps
// locations 0, 1, 5, 8, 11, 14, 17, 18 to 1, locations 2, 3, 6, 9, 12, 15, 19, 20 to 2, and 4, 7, 10, 13, 16, 21 to 4
// (0 for any other id); its callers pick "Peak1" for 1, "Peak2" for 2 and "Peak3" otherwise. func_0028CF98 takes one
// as its peak argument, or -1 to look it up from the race location.
enum ePeakBit {
    kPEAKBIT_FromLocation = -1, kPEAKBIT_None = 0, kPEAKBIT_Peak1 = 1, kPEAKBIT_Peak2 = 2, kPEAKBIT_Peak3 = 4
};

// cSSXAudio::mMusicRampState (names ours). [U] cSSXAudio_InGameInit stores 0; func_0028F000 starts a volume ramp
// (mMusicRampCounter = 0) whenever it is not 2 and sets it to 2.
enum eMusicRampState { kMUSICRAMP_Idle = 0, kMUSICRAMP_Ramping = 2 };

// cSSXAudio::mMusicEventOwner with no owner (name ours). [U] func_0028CF98 and func_0028D7D8 store -1;
// func_0028D630 lets only the focus rider claim the event then, func_0028D740 treats it as nobody.
enum { kNoMusicEventOwner = -1 };

struct cSSXAudio {
    // --- cBankSys (non-virtual part 0x10; ctor cBankSys_cBankSys 0x28FD00, dtor cBankSys__cBankSys; vtable cBankSys_vtbl)
    cBankManager* mpVBankManager;       // 0x000 [R] g++ 2.95 virtual-base pointer to the shared cBankManager (= this + 0x6C98); every ctor in the chain stores it; [U] the inlined SFX_Begin/SFX_Play2D path loads it (39 methods)
    int* mBank;                         // 0x004 [O] cBankSys::mBank; [R] cBankSys ctor: operator new(0x10, "mBank")
    int* mPatch;                        // 0x008 [O] cBankSys::mPatch; [R] operator new(0x10, "mPath") (EA's tag spelling)
    void** mVtbl;                       // 0x00C [R] primary vptr: cSSXAudio_vtbl (cSSXAudio ctor/dtor), cCrowdSys_vtbl (cController/cCrowdSys), cBankSys_vtbl (cBankSys/cMidiSys)
    // --- cMidiSys (EA name from the preview: cMidiSys__cMidiSys = cMidiSys's virtual slot 1, the dtor; its ctor is cMidiSys_cMidiSys 0x2A8140)
    void* mMidiBanks;                   // 0x010 [R] operator new(0x20, "MIDIBANKS") in the cMidiSys ctor (named after the tag); [U] func_002A8450/func_002A86B8
    // --- cCrowdSys (preview: cCrowdSys__cCrowdSys = cCrowdSys's dtor; ctor cCrowdSys_cCrowdSys stores the instance in gpCrowdSys)
    int mNumCrowdInstances;             // 0x014 [R] cCrowdSys ctor 0; [U] func_002A6D78 stores n and allocates n mCrowdInstances; func_002A6D18/func_002A7718/func_002A77C8 loop to it
    void* mCrowdInstances;              // 0x018 [R] cCrowdSys ctor 0; func_002A6D78: operator new[](n, "CrowdInstances"); [U] func_002A72D8..func_002A77C8
    tCrowdData mCrowdData[3];           // 0x01C [R] ctor constructs 3 (func_002A4BA8 x3 each); [P] CROWD_GetData(eCrowdIntensity) returns this + 0x1C + i * 0x54
    // --- cMusicSys (preview: func_002B3398 = cMusicSys's dtor; ctor cMusicSys_cMusicSys(this + 0x118); non-virtual part 0x5448, its methods in sound/bankmonitor.cpp take this + 0x118)
    cMusicSys MusicSys;                 // 0x118 cMusicSys base (sound/musicsys.h; non-virtual part 0x5448)
    // --- cSpeechSys (preview: cSpeechSys__cSpeechSys = cSpeechSys's dtor; ctor cSpeechSys_cSpeechSys(this + 0x5560) stores the instance in D_004A37B4; non-virtual part 0x1C0)
    cSpeechSys SpeechSys;               // 0x5560 cSpeechSys base (sound/speechsys.h; non-virtual part 0x1C0)
    // --- cController (preview: cController__cController = cController's virtual slot 1; ctor cController_cController = cCrowdSys + cMusicSys + cSpeechSys)
    int unk_5720;                       // 0x5720 [R] cController ctor 0
    char unk_5724[0x4];                 // 0x5724 padding (cController's non-virtual part ends 8-aligned)
    // --- cSSXAudio (own fields 0x5728..0x6C90; fields from mGruntCycle on follow On Tour's cSSXAudio member order, retail-only fields before it)
    void* unk_5728;                     // 0x5728 [R] ctor 0; dtor closes it with FILESYS_delbigsync(h, 100); [U] func_0029ED90 opens a language stream file into it
    int unk_572C;                       // 0x572C [R] ctor -1; [U] func_0029ED90: language mode of the stream in unk_5728
    float mFrontEndDJVolume;            // 0x5730 (name ours) [R] ctor 1.0f; [U] cSSXAudio_UpdateSpeechVolume returns
                                        //        &it as the volume control of a front-end DJ line (class 11), so those
                                        //        play at 1.0
    int mGruntPatchCycle;               // 0x5734 (name ours) [R] ctor 0; [U] cSSXAudio_PlayGrunt: the patch of a
                                        //        cycling grunt, advanced after each use and wrapped to 0 at 6
    // mSpeechVolumeID's class for a front-end DJ line, one past the eVolumeControl values (name ours). [U]
    // cSSXAudio_GetSpeechVolumeIdentifier stores 11 for those files; cSSXAudio_UpdateSpeechVolume answers it with
    // mFrontEndDJVolume.
    enum { kFrontEndDJVolumeClass = 11 };
    int mSpeechVolumeID;                // 0x5738 (name ours) [U] cSSXAudio_GetSpeechVolumeIdentifier stores speech
                                        //        instance 0's volume class from the file name prefix (DJ_ 2/11, PA_ 3,
                                        //        Arcade 10, else 4: eVolumeControl, 11 past it for the front-end DJ);
                                        //        func_0029F088 returns it
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
    int mSongAnnouncePending;           // 0x5774 (name ours) [R] ctor; [U] the DJ's pending song announcement: set with
                                        //        mSongAnnounceNum by cSSXAudio_CountdownGateDrop, func_002A3170 and
                                        //        cSSXAudio_StartEventWithNoStartGate; func_002A43B8 plays
                                        //        func_002A2860(mSongAnnounceNum) and clears it; reset by func_002A4550
    int mSongAnnounceNum;               // 0x5778 (name ours) [U] the song number func_002A43B8 passes to func_002A2860
                                        //        (the DJ's song announcement); stored from func_002B4908 (the current
                                        //        song) with mSongAnnouncePending
    int unk_577C;                       // 0x577C [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5780;                       // 0x5780 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int unk_5784;                       // 0x5784 [R] ctor; [U] reset by func_002A4550, read by func_002A43B8
    int mRiderRemarkPending;            // 0x5788 (name ours) [R] ctor; [U] cSSXAudio_StartEventWithNoStartGate sets it
                                        //        in game events 4 and 5; func_002A43B8 then plays func_002A1138(0, 0)
                                        //        (speech line 8403, a rider about another) and clears it; reset by
                                        //        func_002A4550
    int unk_578C;                       // 0x578C [R] ctor 0; [U] func_002A2E50, func_002A4A78; retail accessor 0x2BAD48 (lw)
    int unk_5790;                       // 0x5790 [R] ctor 1; [U] func_002A2E50; retail accessor 0x2BAD50 (lw)
    int unk_5794;                       // 0x5794 [R] ctor 1; [U] func_002A26F0
    int unk_5798;                       // 0x5798 [R] ctor -1; [U] func_002A2E50
    int mFirstTimeOnTrack[23];          // 0x579C (name ours) [R] ctor zeroes 23; [U] cSSXAudio_InGameInit fills entry i
                                        //        with cBENewPlayerInterface_isFirstTimeOnTrack(rider, i) for the 23
                                        //        tracks; func_0028E548 tests entries for 1 and clears them,
                                        //        func_0028E8C0 tests [mTransportTrack] != 1
    int unk_57F8;                       // 0x57F8 [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_57FC;                       // 0x57FC [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_5800;                       // 0x5800 [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_5804;                       // 0x5804 [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_5808;                       // 0x5808 [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_580C;                       // 0x580C [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_5810;                       // 0x5810 [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_5814;                       // 0x5814 [U] set by func_002A4590, used by func_002A4660/func_002A4718/func_002A4770
    int unk_5818;                       // 0x5818 [R] ctor 0; [U] func_002A49E8/func_002A4A38/func_002A4A68
    int mLastUberLevel;                 // 0x581C (name ours) [R] ctor -1; [U] cSSXAudio_UpdateLandedUber: the focus
                                        //        rider's mUberLevel last frame (the DJ event fires when it becomes 10)
    int mUberViewIndex;                 // 0x5820 (name ours) [R] ctor 0; [U] cSSXAudio_UpdateLandedUber: the focus
                                        //        rider's mViewIndex; a change only records the new view
    int mGateDropHeat;                  // 0x5824 (name ours) [U] cSSXAudio_CountdownGateDrop stores cGameModeMan
                                        //        iHeatNum; cSSXAudio_PartialRestart compares it with iNextHeat
    int mStopSpeechOnResume;            // 0x5828 (name ours) [R] ctor 0; [U] func_0028FAE0 (map-screen music restart)
                                        //        sets it; cSSXAudio_ResumeAudio then stops speech instance 0, clears
                                        //        the pending speech flags (func_002A4550) and clears it
    int mGruntCycle;                    // 0x582C [O] first own member; [U] cSSXAudio_StartEvent/cSSXAudio_StartEventWithNoStartGate set it, func_002A0560 reads it
    float mWindVolume;                  // 0x5830 [O]; [U] cSSXAudio_BeginWind passes &it as the stream's volume control, cSSXAudio_EndWind and cSSXAudio_BeginAllRiderSound clear it
    int mWindPlaying;                   // 0x5834 [O] (On Tour bool); [U] BeginWind/EndWind
    char mUsedBank[64];                 // 0x5838 [O]; [U] cSSXAudio_InGameLoad clears [0], func_0029B818 writes it
    int mNextUsedBank;                  // 0x5878 [O]; [U] cSSXAudio_InGameLoad zeroes it
    char mSwapBasePath[128];            // 0x587C [O]; [U] cSSXAudio_InGameLoad builds it
    char mAemsSysName[128];             // 0x58FC [O]; placed by order and the cCircBuffer at 0x597C
    // A play-history record's time when it has none (name ours). [U] cSSXAudio_RestartEvent / PartialRestart add
    // event 0 with -1 (cSSXAudio_PlayHistoryAdd), and cSSXAudio_PlayHistoryUpdate keeps such a record until its sound
    // ends.
    enum { kPlayHistoryNoTimeout = -1 };
    cCircBuffer mPlayHistory;           // 0x597C [O]; [R] ctor: ArraySize 200, Array = operator new(800, "CircFIFOBuff"), Head/Tail 0; dtor frees Array; [U] cSSXAudio_PlayHistoryExists..cSSXAudio_PlayHistoryTimeout (PlayHistory*)
    int mRailType;                      // 0x598C [O]; [P] SetRailSurface(int) (preview 0x5A68; retail accessor 0x2BAD58 sw 0x598C); [U] cSSXAudio_Surface reads it for a rider on a rail
    float mLastCarveEdging[10];         // 0x5990 [O]; [U] cSSXAudio_HardCarveProgram/cSSXAudio_SoftCarveProgram (carve programs) index it
    float mLastGlide[10];               // 0x59B8 [O]; [U] cSSXAudio_HardGlideProgram/cSSXAudio_SoftGlideProgram/cSSXAudio_RailGlideProgram (glide programs)
    int mTurboHandle;                   // 0x59E0 [O]; [U] cSSXAudio_TurboOn sets it, cSSXAudio_TurboOff/func_0029ED18/cSSXAudio_StopAllRiderSounds stop it
    int mTurboRef;                      // 0x59E4 [O]; [U] TurboOn increments, cSSXAudio_TurboOff decrements
    int mNeedLift[6];                   // 0x59E8 [O] (On Tour bool[6]); [U] cSSXAudio_UpdateLift, cSSXAudio_ResetLift clears 6 words
    tAttachSoundStruct AttachedSounds[30]; // 0x5A00 [O] (On Tour's name); [R] ctor's 30-element empty-ctor loop; [U] cSSXAudio_AttachSound/cSSXAudio_DetachSound/cSSXAudio_InitAttachedSounds/cSSXAudio_ResetAttachedSounds walk 30 x 0x30 from 0x5A00
    tWorldPainterQuery* mpAmbienceQuery[2]; // 0x5FA0 [O]; [U] cSSXAudio_InGameInit stores,
                                        //        cSSXAudio_InGameDeInit frees,
                                        //        cSSXAudio_UpdateAudioWorldPainter queries
    int mPointsLastValue;               // 0x5FA8 [O]; [U] cSSXAudio_PointsEvent
    int mPointsEventHandle;             // 0x5FAC [O]; [U] cSSXAudio_PointsEvent
    int mSFXPaused;                     // 0x5FB0 [O] (On Tour bool); [R] ctor-time 0; [U] cSSXAudio_PauseSFX sets, cSSXAudio_ResumeSFX reads; retail accessor 0x2BAD60 (lw)
    int mAudioPaused;                   // 0x5FB4 [O] (On Tour bool); [U] cSSXAudio_PauseAudio/cSSXAudio_ResumeAudio (Pause/ResumeAudio), cSSXAudio_Update tests it
    int mCharLandHandle;                // 0x5FB8 [O]; [U] cSSXAudio_Land
    int mCharLiftHandle;                // 0x5FBC [O]; [U] cSSXAudio_Lift
    unsigned int mLastPatchRID;         // 0x5FC0 [O] tResourceID (4 bytes); [R] ctor 0xFFFFFFFF; [U] cSSXAudio_InGameInit stores byte 0 (sb), cSSXAudio_UpdateTrackBanks compares byte 0 (lbu) and stores the word
    int mPlayingTone;                   // 0x5FC4 [O] (On Tour bool); [U] cSSXAudio_StartTone/cSSXAudio_UpdateTone/cSSXAudio_StopTone (Start/Update/StopTone)
    int mToneHandle;                    // 0x5FC8 [O]; [R] ctor -1; [U] tone functions
    int mThunderHandle;                 // 0x5FCC [O]; [U] cSSXAudio_PlayThunder
    int mCurBigChallengeType;           // 0x5FD0 [O] eBEAudioType; [U] cSSXAudio_BigChallengeBegin stores it; cSSXAudio_BigChallengeComplete (the
                                        //        completion) clears it
    int mInBigChallenge;                // 0x5FD4 [O] (On Tour bool); [R] ctor 0; [U] cSSXAudio_BigChallengeComplete, cSSXAudio_BigChallengeFailed
    int mBigChallengeStarting;          // 0x5FD8 [O] (On Tour bool); [R] ctor 0; [U] cSSXAudio_SetBigChallengeStarting, cSSXAudio_BigChallengeBegin
    // mUberSpinHandle with no spin sound playing (name ours). [R] ctor -1; [U] cSSXAudio_StartUberSpin /
    // cSSXAudio_EndUberSpin stop the sound only when it differs.
    enum { kNoUberSpinHandle = -1 };
    int mUberSpinHandle;                // 0x5FDC [O]; [R] ctor -1; [U] cSSXAudio_StartUberSpin/cSSXAudio_EndUberSpin (Start/EndUberSpin)
    int mBoostBarPreviousValue[2];      // 0x5FE0 [O]; [R] ctor zeroes both; [U] cSSXAudio_UpdateBoostBar (the per-frame boost-bar
                                        //        update)
    unsigned short mBoostBarPitchMult[2]; // 0x5FE8 [O]; [U] cSSXAudio_UpdateBoostBar
    int mUberSecPreviousValue[2];       // 0x5FEC [O]; [R] ctor zeroes both
    int mAvalancheLoopHandle;           // 0x5FF4 [O]; [U] cSSXAudio_Avalanche
    char unk_5FF8[0x8];                 // 0x5FF8 padding (mAvalancheHitPositions is 16-aligned)
    t4Vector mAvalancheHitPositions[3]; // 0x6000 [O]; [R] ctor's 3-element empty-ctor loop (t4Vector)
    int mAvalancheHitHandles[3];        // 0x6030 [O]; [R] ctor sets 3 x -1
    int mAvalancheHitIndex;             // 0x603C [O]; [R] ctor -1
    int mAvalancheInstanceCount;        // 0x6040 [O]; [R] ctor 0; [U] cSSXAudio_Avalanche
    char unk_6044[0xC];                 // 0x6044 padding (mAvalancheData is 16-aligned)
    tAvalanchePositionalData mAvalancheData; // 0x6050 [O]; [U] cSSXAudio_Avalanche/cSSXAudio_HandleAvalanche copy it with two sq
    int mpAvalancheRiderIndex;          // 0x6070 [O]; [U] cSSXAudio_Avalanche/cSSXAudio_HandleAvalanche
    float mCurTrickPower[2];            // 0x6074 [O]; [U] cSSXAudio_SetCurTrickPower swc1, cSSXAudio_ResetAudioVariables reads [0] and zero-stores both
    int mLastTime;                      // 0x607C [O]; [U] cSSXAudio_UpdateTimedEvent
    float mLastWipeoutRecoverPercent[2]; // 0x6080 [O]; [U] cSSXAudio_ResetWipeoutRecoverHit/cSSXAudio_PlayWipeoutRecoverHit/cSSXAudio_PlayWipeoutRecoverFull (wipeout recover hits)
    int mPausedSFXForMapScreen;         // 0x6088 [O] bool (a word in retail; On Tour's member right after
                                        //        mLastWipeoutRecoverPercent, before mAudioMode); [U] func_0028F5B8
                                        //        (cUITemplate_MAP's map-screen entry) clears it, then sets it to 1 when
                                        //        it pauses the SFX (cSSXAudio_PauseSFX); func_0028F678 (map-screen exit)
                                        //        resumes them (cSSXAudio_ResumeSFX) only when set, and clears it
    int mRadioModeID;                       // 0x608C [R] ctor 0; [U] func_0028BF78 sets it (switching the radio mode
                                        //        restarts the music) and 11 methods read it (cSSXAudio_FrontEndLoad,
                                        //        cSSXAudio_FrontEndPurge, cSSXAudio_NeedWPAmbience, func_0028CF98,
                                        //        ...); retail accessor 0x2BAD68; the radio mode: values 0..3 as
                                        //        On Tour's eRadioModeID (cSSXAudio_NeedWPAmbience tests 2,
                                        //        kRADIOMODE_MOUNTAIN); the preview's GetRadioModeID (0x6134; the paired NeedWPAmbience reads it)
    int mFrontEndRadioModeID;           // 0x6090 (name ours) [R] ctor 0; [U] cSSXAudio_FrontEndLoad copies mRadioModeID
                                        //        into it, cSSXAudio_FrontEndPurge stops the stream (instead of a fade)
                                        //        when it or mRadioModeID is 2
    char unk_6094[0x4];                 // 0x6094 no retail access seen (or padding)
    unsigned long mRadioPlayList;       // 0x6098 PORT: 64-bit long; [P] GetRadioPlayList (preview 0x6138, retail 0x2BAD70 ld 0x6098; layout_xlate pair-seen); [R] ctor sd 0
    float unk_60A0;                     // 0x60A0 [U] func_0028C8C8: the song's overlay level during a long jump
    char mPlayListName[20][20];         // 0x60A4 [P] GetPlayListName(int) returns this + 0x60A4 + i * 20 (retail 0x2BAD80; preview 0x6144)
    int mNumPlayLists;                  // 0x6234 [P] GetNumPlayLists (preview 0x62D4, same -0xA0 shift; retail accessor 0x2BAD78); [U] func_00289470 counts them
    int mPlayListIndex;                 // 0x6238 [P] GetPlayListIndex (preview 0x62D8; retail accessor 0x2BAD98); [U] func_00289520 stores it
    int unk_623C;                       // 0x623C [U] the running rider music event: func_0028D630 starts it,
                                        //        func_0028D898 returns it, func_0028D740 / func_0028D7D8 end it,
                                        //        func_0028CF98 clears it
    int unk_6240;                       // 0x6240 [U] the event to follow it: func_0028D740 records it
    float unk_6244;                     // 0x6244 [U] func_0028C8C8: the air time, cached on the jump's first frame
    float unk_6248;                     // 0x6248 [U] func_0028C8C8: the largest current air time of the jump
    float unk_624C;                     // 0x624C [U] func_0028C8C8: the volume scale's air time, set on the
                                        //        jump's first frame
    int unk_6250;                       // 0x6250 [R] ctor 0; [U] func_0028C8C8 counts it down, func_0028C8C0 sets
                                        //        it; nothing reads it otherwise
    int unk_6254;                       // 0x6254 [R] ctor 0; [U] func_002A4770; retail accessor 0x2BAD30 sets it to 1
    int unk_6258;                       // 0x6258 [U] func_002A2E50
    tWorldPainterQuery* mpMusicTriggerQuery[2]; // 0x625C (name ours) [U] cSSXAudio_InGameInit creates two
                                        //        kWPR_MusicTrigger queries, cSSXAudio_InGameDeInit frees them,
                                        //        cSSXAudio_UpdateAudioWorldPainter queries them
                                        //        (result to func_0028D988)
    int mLastMusicTriggerType;          // 0x6264 (name ours) [U] func_0028D988 compares the music-trigger region's
                                        //        m_Type (its query result's slot 1) with it and stores it;
                                        //        cSSXAudio_InGameInit = -1
    int mLastMusicTriggerValue;         // 0x6268 (name ours) [U] func_0028D988 compares the music-trigger region's
                                        //        m_Value (its query result's slot 2) with it and stores it;
                                        //        cSSXAudio_InGameInit = -1
    int unk_626C;                       // 0x626C [U] cSSXAudio_InGameInit stores
    int mMusicTriggerReady;             // 0x6270 (name ours) [U] cSSXAudio_InGameInit = 1; func_0028D988 sets it on
                                        //        leaving a type 1 / 2 trigger region (type -1), acts on a state-11
                                        //        trigger only while it is set, and clears it after acting
    int mMusicRequestPending;           // 0x6274 (name ours) [U] cSSXAudio_InGameInit stores; func_0028E088 sets 1 with
                                        //        mMusicRequest, func_0028E100 acts while set and clears it (the shift
                                        //        here is -0x94: the preview's PathfinderCanChangeVolume is
                                        //        mPathfinderCanChangeVolume at 0x6280)
    int mMusicRequest;                  // 0x6278 (name ours) [U] func_0028E088 posts the request code (its event's
                                        //        mData1), func_0028E100 carries it out (0 peak theme, 2 location
                                        //        song, 4 stop, else event 36 / 0). Not the preview's
                                        //        SetTransportTrack field: that is mTransportTrack, shift -0x94
    int mRaceMusicOn;                   // 0x627C (name ours) [R] ctor 1; [U] cSSXAudio_InGameLoad = 1;
                                        //        cSSXAudio_OnFinishEvent clears it when it plays the between-heats
                                        //        track 36; cSSXAudio_CountdownBeep starts the race music only when set;
                                        //        cSSXAudio_CountdownGateDrop sets it (music state 37) when clear;
                                        //        func_0028E8C0 sets it on transport start, tests it on transport end
    int mPathfinderCanChangeVolume;                       // 0x6280 [P] PathfinderCanChangeVolume__9cSSXAudio (preview 0x6314; func_002AF6C0 = preview 0x281170); retail accessor 0x2BADA0 (lw); [U] the Pathfinder volume callback applies a volume only when set
    int mTransportTrack;                       // 0x6284 [P] SetTransportTrack__9cSSXAudio13eBELocationID (preview 0x6318: both ctors store 23 there, kMAX_LOCATIONS); [R] ctor 23; [U] func_0028F558 stores the map screen's location
    int mMusicRampState;                // 0x6288 (name ours) [U] cSSXAudio_InGameInit = 0; func_0028F000 starts a
                                        //        volume ramp (mMusicRampCounter = 0) when it is not 2 and sets it to 2
    int mMusicRampCounter;              // 0x628C (name ours) [U] func_0028F000: zeroed when a volume ramp starts
                                        //        (mMusicRampState = 2), then counted up to the tier's 800 / 1000 / 1200
    int mFrontEndMusicMode;             // 0x6290 (name ours) [U] func_0028F140 switches the front-end music to a mode
                                        //        (0-9) only when it differs from this, then stores it;
                                        //        cSSXAudio_FrontEndLoad stores 10 (no mode, so the next switch
                                        //        applies); func_0028FC58 restores it
    int unk_6294;                       // 0x6294 [R] ctor 0
    int mReplaySongPlaying;             // 0x6298 (name ours) [R] ctor 0; [U] func_0028F200 (replay music) sets it when
                                        //        a song is playing, func_0028F2C0 then sets music state 36 and clears
                                        //        it; cSSXAudio_CountdownGateDrop sets state 37 in a replay when set
    int mLastMusicState;                // 0x629C (name ours) [U] func_0028DEF0 stores its argument (11, 13, 18) and
                                        //        reports whether it changed; func_0028DF08 resets it to -1
    int mMusicEventOwner;               // 0x62A0 (name ours) [U] the AI index (cRider virtual slot 7) of the rider
                                        //        whose music event runs, -1 for none: func_0028CF98 resets it,
                                        //        func_0028D630 claims it, func_0028D740 / func_0028D7D8 test and release
    int unk_62A4;                       // 0x62A4 [R] ctor -1
    int mLoadingLoopHandle;             // 0x62A8 (name ours) [R] ctor -1; [U] the loading-screen loop's voice handle
                                        //        (bank 13, patch 0): func_0028F768 starts it, func_0028FA98 stops it
                                        //        (-1 = none); cSSXAudio_WorldSound and cSSXAudio_AttachSound play
                                        //        nothing while it is >= 0
    int mSavedSongNum;                  // 0x62AC (name ours) [U] func_0028CDF8 saves MusicSys.mCurrentSongNum before
                                        //        the podium tune; func_0028F200 (replay music) restores it
    int mAudioMode;                     // 0x62B0 [O] eSOUNDDEF_MODE; [R] ctor stores its audioMode argument; [U] cSSXAudio_SetMode stores, cSSXAudio_Calc3DPos and cSSXAudio_GetCurrentSoundMode (GetMode) read
    int mArcadeSFX;                     // 0x62B4 [O] (On Tour bool, after mAudioMode); [R] ctor 0; [U] cSSXAudio_SetArcadeSFX stores it (and sets volume controls 9/10); cSSXAudio_SetSFXVolume re-applies controls 9/10 only when set
    int mDJSpeech;                      // 0x62B8 (name ours) [R] ctor 0; [U] cSSXAudio_SetDJSpeech stores 1 (DJ volume
                                        //        control 2 on) or 0; func_00287410 re-applies control 2 only when set
                                        //        (and the radio mode is not 2); retail accessor func_002BADB0 returns
                                        //        it
    tSoundVolume mSoundVolume[11];      // 0x62BC [O]; [U] cSSXAudio_DuckVolume: entry j curVolume * duck -> duckedVolume, * mMixLevel[j] -> appliedVolume
    float mCharacterVolume[10];         // 0x636C (name ours) per-character voice level, indexed Moby .. Deiter: [U] DuckVolume/MixVolume (cSSXAudio_DuckVolume/cSSXAudio_UpdateDuck/cSSXAudio_MixVolume/cSSXAudio_UpdateMix): mSoundVolume[4].appliedVolume times constants gMobyScaling..
    float mDuckLevel[11];               // 0x6394 [O]; [R] ctor 11 x 1.0f; [U] DuckVolume writes [j]
    int mDucking;                       // 0x63C0 [O] (On Tour bool); [R] ctor 0; [U] DuckVolume sets it when fading
    unsigned int mCurDuck;              // 0x63C4 [O]; [U] DuckVolume stores the mask
    float mDuckStep;                    // 0x63C8 [O]; [U] DuckVolume
    float mDuckFinalValue;              // 0x63CC [O]; [U] DuckVolume
    float mDuckCurValue;                // 0x63D0 [O]; [R] ctor 1.0f; [U] DuckVolume
    int mDuckStream;                    // 0x63D4 [O]; [R] ctor 3; [U] cSSXAudio_Update
    float mDuckScale;                   // 0x63D8 [O]; [R] ctor 1.0f
    int mStreamIsDucked;                // 0x63DC [O] (On Tour bool); [R] ctor 0; [U] cSSXAudio_DuckToStream
    int mDuckedStreamHasStarted;        // 0x63E0 [O] (On Tour bool); [U] cSSXAudio_Update
    float mMixLevel[11];                // 0x63E4 [O]; [R] ctor 11 x 1.0f; [U] DuckVolume, cSSXAudio_MixVolume
    int mMixing;                        // 0x6410 [O] (On Tour bool); [U] cSSXAudio_MixIntToFloat, cSSXAudio_UpdateMix
    float mMixStep[11];                 // 0x6414 [O]; [U] cSSXAudio_UpdateMix
    float mMixFinalValue[11];           // 0x6440 [O]; placed by order
    int mLastMix;                       // 0x646C [O]; [R] ctor -1; [U] cSSXAudio_MixVolume
    int mPreset;                        // 0x6470 [O]; [P] GetPreset (preview 0x64DC; retail accessor 0x2BADC0 lw 0x6470); [U] cSSXAudio_SetPreset, cSSXAudio_PrevPreset/cSSXAudio_NextPreset
    int mAudioUpdateState;              // 0x6474 [O]; [P] GetAudioUpdateState (preview 0x64E4; retail accessor 0x2BADC8); [U] cSSXAudio_SetAudioUpdateState, cSSXAudio_Update
    int mNumAudioMixes;                 // 0x6478 [O]; [U] cSSXAudio_LoadMixes
    tAudioMix mAudioMixes[25];          // 0x647C [O]; [U] cSSXAudio_LoadMixes fills 25 x 0x28
    int mRMSFrameTick;                  // 0x6864 [O]; [U] cSSXAudio_ApplyRMSData
    char mRMSData[1000];                // 0x6868 [O]; placed by order (0x6868 + 1000 = mRMSSize)
    int mRMSSize;                       // 0x6C50 [O]; [U] cSSXAudio_ApplyRMSData
    int mRMSDataFrameRate;              // 0x6C54 [O]; [R] ctor 60
    int mValidRMSData;                  // 0x6C58 [O] (On Tour bool); [R] ctor 0; [U] cSSXAudio_ApplyRMSData/cSSXAudio_CancelRMSData
    float mLastRMSDuckLevel;            // 0x6C5C [O]; [U] cSSXAudio_DuckByRMS
    int mRMSStepFrame;                  // 0x6C60 [O]; [R] ctor 1
    float mRMSStepValue;                // 0x6C64 [O]; [U] cSSXAudio_ApplyRMSData/cSSXAudio_CancelRMSData
    float mRMSCurValue;                 // 0x6C68 [O]
    float mRMSFinalValue;               // 0x6C6C [O]
    int mTrackBankLoaded[2];            // 0x6C70 [O] (On Tour bool[2]); [R] ctor zeroes both; [U] cSSXAudio_StartTone reads [1]
    int mTrackBankLoadedSection[2];     // 0x6C78 [O]; [R] ctor sets both -1
    tWorldPainterQuery* mpMixQuery[2];  // 0x6C80 [O]; [U] cSSXAudio_InGameInit stores, cSSXAudio_InGameDeInit frees,
                                        //        cSSXAudio_UpdateAudioWorldPainter queries
    int mPlayIntroMovieWithDJ;          // 0x6C88 [O] (On Tour bool, the last member); [R] ctor 1; [U] cSSXAudio_PlayIntroMovieWithDJ
    char unk_6C8C[0x4];                 // 0x6C8C no retail access seen (or padding)
    // --- virtual base cSoundSys (0x8; ctor cSoundSys_cSoundSys(this + 0x6C90))
    void* pManager;                     // 0x6C90 [O] cSoundSys::pManager; [R] cSoundSys ctor stores cSoundManager_Construct() (the PS2 sound manager)
    void** mSoundSysVtbl;               // 0x6C94 [R] cSoundSys vptr; the ctors in the chain point it at their stack-patched copies
    // --- virtual base cBankManager (0xAD0; ctor cBankManager_cBankManager(this + 0x6C98)): cAsyncSys at 0, cBankMonitor at 0x1D8
    char mAsyncSys[468];                // 0x6C98 cAsyncSys data (On Tour 0x1D4: mHandle .. mBufferSize; ctor cAsyncSys_cAsyncSys)
    void** mBankManagerVtbl;            // 0x6E6C [R] cBankManager ctor stores cBankManager_vtbl (cAsyncSys's vptr slot, cBankManager+0x1D4)
    cBankMonitor BankMonitor;           // 0x6E70 cBankManager's cBankMonitor (sound/bankmonitor.h; non-virtual part 0x8F0)
    int mMaxBanks;                      // 0x7760 [O] cBankManager::mMaxBanks; [R] cBankManager ctor stores its count argument
    void* mBankManagerBank;             // 0x7764 [O] cBankManager::mBank (renamed: cBankSys::mBank at 0x4); [R] operator new[](count x 0x60)
    // --- virtual base cStreamSys (0xC; ctor cStreamSys_cStreamSys(this + 0x7768))
    cBankManager* mpStreamVBankManager; // 0x7768 [R] cStreamSys's virtual-base pointer (= this + 0x6C98)
    int mNumStreams;                    // 0x776C [O]; [R] cStreamSys ctor stores its count
    void** mStreamInstance;             // 0x7770 [O] cStreamInstance**; [R] operator new[](count x 4, "StreamArray")
    char unk_7774[0xC];                 // 0x7774 tail padding (the class is 16-aligned: t4Vector members)

    // Methods bound to their retail symbols by asm labels.
    void SetRailSurface(int railType) __asm__("cSSXAudio_SetRailSurface");
    int GetSFXPaused() __asm__("cSSXAudio_GetSFXPaused");
    int GetRadioModeID() __asm__("cSSXAudio_GetRadioModeID");
    long GetRadioPlayList() __asm__("cSSXAudio_GetRadioPlayList");
    int GetNumPlayLists() __asm__("cSSXAudio_GetNumPlayLists");
    char* GetPlayListName(int i) __asm__("cSSXAudio_GetPlayListName");
    int GetPlayListIndex() __asm__("cSSXAudio_GetPlayListIndex");
    int GetMode() __asm__("cSSXAudio_GetMode");
    int GetArcadeSFX() __asm__("cSSXAudio_GetArcadeSFX");
    int GetPreset() __asm__("cSSXAudio_GetPreset");
    int GetAudioUpdateState() __asm__("cSSXAudio_GetAudioUpdateState");
    int GetGruntVol(cRider* src) __asm__("cSSXAudio_GetGruntVol");
    int SFX_Play2D(int bank, int patch) __asm__("cSSXAudio_SFX_Play2D");
    void SetCurTrickPower(cRider* rider, float value) __asm__("cSSXAudio_SetCurTrickPower");
    void BeginWind() __asm__("cSSXAudio_BeginWind");
    void EndWind() __asm__("cSSXAudio_EndWind");
    void PlayThunder() __asm__("cSSXAudio_PlayThunder");
    int GetBoardBank(int surfaceType) __asm__("cSSXAudio_GetBoardBank");
    void BeginAllRiderSound() __asm__("cSSXAudio_BeginAllRiderSound");
    void BeginRiderSound(cRider* rider) __asm__("cSSXAudio_BeginRiderSound");
    void StopRiderSound(int Rider) __asm__("cSSXAudio_StopRiderSound");
    void StopAllRiderSounds() __asm__("cSSXAudio_StopAllRiderSounds");
    void HandleChatter(cBankMonitorInstance* obj) __asm__("cSSXAudio_HandleChatter");
    void HandleGlide(cBankMonitorInstance* obj) __asm__("cSSXAudio_HandleGlide");
    void HandleAIGlide(cBankMonitorInstance* obj) __asm__("cSSXAudio_HandleAIGlide");
    void HandleCarve(cBankMonitorInstance* obj) __asm__("cSSXAudio_HandleCarve");
    void ChatterProgram(cRider* vrider, int* out1, int* out2) __asm__("cSSXAudio_ChatterProgram");
    void GlideProgram(int id, cRider* rider, int* out, int* size) __asm__("cSSXAudio_GlideProgram");
    void AIGlideProgram(int id, cRider* rider, int* out, int* size) __asm__("cSSXAudio_AIGlideProgram");
    void CarveProgram(int id, cRider* rider, int* out, int* size) __asm__("cSSXAudio_CarveProgram");
    int GetSurfaceProgramType(int kind, cRider* rider) __asm__("cSSXAudio_GetSurfaceProgramType");
    // [R] cSSXAudio_Surface: the surface sound id under @p rider (mRailType for a rider on a rail); asm
    int Surface(cRider* rider) __asm__("cSSXAudio_Surface");
    void HardCarveProgram(cRider* rider, int* volume, int* pitch) __asm__("cSSXAudio_HardCarveProgram");
    void SoftCarveProgram(cRider* rider, int* volume, int* pitch) __asm__("cSSXAudio_SoftCarveProgram");
    void RailCarveProgram(cRider* rider, int* volume, int* pitch) __asm__("cSSXAudio_RailCarveProgram");
    void LiftWithAirTime(cRider* rider) __asm__("cSSXAudio_LiftWithAirTime");
    void UpdateLift() __asm__("cSSXAudio_UpdateLift");
    int GetNeedLift(int i) __asm__("cSSXAudio_GetNeedLift");
    void SetNeedLift(int i, int needLift) __asm__("cSSXAudio_SetNeedLift");
    void ResetLift() __asm__("cSSXAudio_ResetLift");
    void Land(cRider* rider, float powerupGained) __asm__("cSSXAudio_Land");
    int PlayPauseSound() __asm__("cSSXAudio_PlayPauseSound");
    void MenuSound(int cue) __asm__("cSSXAudio_MenuSound");
    bool PlayHistoryExists(int Ptr, int Hull) __asm__("cSSXAudio_PlayHistoryExists");
    void PlayHistoryUpdate() __asm__("cSSXAudio_PlayHistoryUpdate");
    void PlayHistoryAdd(void* evt, int Handle, int Time, int Looping, int idx) __asm__("cSSXAudio_PlayHistoryAdd");
    void PlayHistoryUpdateTime(int evt, int NewTime, int idx) __asm__("cSSXAudio_PlayHistoryUpdateTime");
    void PlayHistoryTimeout(int evt) __asm__("cSSXAudio_PlayHistoryTimeout");
    void CollisionSound(cRider* rider, tInstanceRecord* evt, int idx) __asm__("cSSXAudio_CollisionSound");
    void Wipeout(cRider* rider, float intensity) __asm__("cSSXAudio_Wipeout");
    void WipeoutHit(cRider* obj) __asm__("cSSXAudio_WipeoutHit");
    void WipeoutSlideStart(cWipeoutControl* wipeoutControl) __asm__("cSSXAudio_WipeoutSlideStart");
    void WipeoutSlideEnd(cRider* obj) __asm__("cSSXAudio_WipeoutSlideEnd");
    void WipeoutTumbleStart(cWipeoutControl* wipeoutControl) __asm__("cSSXAudio_WipeoutTumbleStart");
    void WipeoutTumbleEnd(cRider* obj) __asm__("cSSXAudio_WipeoutTumbleEnd");
    void WorldSound(tInstanceRecord* evt, int n) __asm__("cSSXAudio_WorldSound");
    void AttachSound(tInstanceRecord* obj, int type, int id) __asm__("cSSXAudio_AttachSound");
    void DetachSound(tInstanceRecord* inst, int patch) __asm__("cSSXAudio_DetachSound");
    void InitAttachedSounds() __asm__("cSSXAudio_InitAttachedSounds");
    void UpdateAttachedSounds() __asm__("cSSXAudio_UpdateAttachedSounds");
    void ResetAttachedSounds() __asm__("cSSXAudio_ResetAttachedSounds");
    void Punch(cRider* obj, cRider* obj2) __asm__("cSSXAudio_Punch");
    void WallCollision(cRider* obj) __asm__("cSSXAudio_WallCollision");
    void KnockdownSFX(cRider* obj) __asm__("cSSXAudio_KnockdownSFX");
    void TurboOn(cRider* obj, int flag) __asm__("cSSXAudio_TurboOn");
    void TurboDeny(cRider* obj) __asm__("cSSXAudio_TurboDeny");
    void MaxPowerEvent(cRider* obj) __asm__("cSSXAudio_MaxPowerEvent");
    void LoseMaxPowerEvent(cRider* obj) __asm__("cSSXAudio_LoseMaxPowerEvent");
    void UberRailDeny(cRider* obj) __asm__("cSSXAudio_UberRailDeny");
    void ChangePosition(cRider* obj, cRider* pPrevPosRider, cRider* pNextPosRider, int lim, int n)
        __asm__("cSSXAudio_ChangePosition");
    void PlayerResetEvent(cRider* obj) __asm__("cSSXAudio_PlayerResetEvent");
    void Tweak(cRider* obj) __asm__("cSSXAudio_Tweak");
    void PointsEvent(cRider* obj, int n) __asm__("cSSXAudio_PointsEvent");
    void BoostBar(cRider* obj) __asm__("cSSXAudio_BoostBar");
    void UpdateBoostBar() __asm__("cSSXAudio_UpdateBoostBar");
    void StartUberSpin(cRider* obj) __asm__("cSSXAudio_StartUberSpin");
    void EndUberSpin(cRider* obj) __asm__("cSSXAudio_EndUberSpin");
    void LandedUberTrick(cRider* obj, int iNum, int iPrevious) __asm__("cSSXAudio_LandedUberTrick");
    void UpdateLandedUber() __asm__("cSSXAudio_UpdateLandedUber");
    void LandedMonsterCombo(cRider* obj) __asm__("cSSXAudio_LandedMonsterCombo");
    void BindingsOn(cRider* obj) __asm__("cSSXAudio_BindingsOn");
    void BindingsOff(cRider* obj) __asm__("cSSXAudio_BindingsOff");
    void UndoBindings(cRider* obj) __asm__("cSSXAudio_UndoBindings");
    void CountdownBeep(int mode) __asm__("cSSXAudio_CountdownBeep");
    void CountdownGateDrop() __asm__("cSSXAudio_CountdownGateDrop");
    void UpdateTimedEvent() __asm__("cSSXAudio_UpdateTimedEvent");
    void PauseSFX() __asm__("cSSXAudio_PauseSFX");
    void ResumeSFX() __asm__("cSSXAudio_ResumeSFX");
    void PlayIconSound(int kind, cRider* obj, float x) __asm__("cSSXAudio_PlayIconSound");
    void UpdateTrackBanks() __asm__("cSSXAudio_UpdateTrackBanks");
    void StartTone() __asm__("cSSXAudio_StartTone");
    void UpdateTone() __asm__("cSSXAudio_UpdateTone");
    void StopTone(float FadeTime) __asm__("cSSXAudio_StopTone");
    void SetBigChallengeStarting() __asm__("cSSXAudio_SetBigChallengeStarting__FPv");
    void BigChallengeComplete() __asm__("cSSXAudio_BigChallengeComplete");
    void BigChallengeFailed(int keep) __asm__("cSSXAudio_BigChallengeFailed");
    void HandPlantSound(cRider* obj) __asm__("cSSXAudio_HandPlantSound");
    void Avalanche(int on) __asm__("cSSXAudio_Avalanche");
    int GetAvalancheVolume(tAvalanchePositionalData* snd_) __asm__("cSSXAudio_GetAvalancheVolume");
    void HandleAvalanche(cBankMonitorInstance* voice) __asm__("cSSXAudio_HandleAvalanche");
    void ResetWipeoutRecoverHit(cRider* rider) __asm__("cSSXAudio_ResetWipeoutRecoverHit");
    void PlayWipeoutRecoverHit(cRider* obj, float x) __asm__("cSSXAudio_PlayWipeoutRecoverHit");
    void PlayWipeoutRecoverFull(cRider* obj) __asm__("cSSXAudio_PlayWipeoutRecoverFull");
    int GetSpeechVolumeIdentifier(int instanceIndex, const char* name) __asm__("cSSXAudio_GetSpeechVolumeIdentifier");
    void* UpdateSpeechVolume(int instanceIndex, int type, int charIndex) __asm__("cSSXAudio_UpdateSpeechVolume");
    void PlayGrunt(cRider* vrider, int length, int allowAI) __asm__("cSSXAudio_PlayGrunt");
    int SetMode(int value) __asm__("cSSXAudio_SetMode");
    int PlayPositional(int streamIndex, const char* fileName, int fileOffset, int position, int holdTime, int userInfo,
                       int isHuman, float radius, int viewport)
        __asm__("cSSXAudio_PlayPositional");
    float Calc3DPos(cBankMonitorInstance* source, SNDPLAYOPTS* outAngles) __asm__("cSSXAudio_Calc3DPos");
    void SetAudioUpdateState(int State) __asm__("cSSXAudio_SetAudioUpdateState");
    int GetFocusRider(int which) __asm__("cSSXAudio_GetFocusRider");
    void BANKMONITOR_DestroyAll() __asm__("cSSXAudio_BANKMONITOR_DestroyAll");
    void FrontEndLoad() __asm__("cSSXAudio_FrontEndLoad");
    void FrontEndFadeSong() __asm__("cSSXAudio_FrontEndFadeSong");
    void FrontEndPurge() __asm__("cSSXAudio_FrontEndPurge");
    void InGameLoad() __asm__("cSSXAudio_InGameLoad");
    void InGameInit() __asm__("cSSXAudio_InGameInit");
    void InGameDeInit() __asm__("cSSXAudio_InGameDeInit");
    void InGamePurge() __asm__("cSSXAudio_InGamePurge");
    void AttachAudioBank(unsigned int id, int on) __asm__("cSSXAudio_AttachAudioBank");
    void UnAttachAudioBank(unsigned int handle) __asm__("cSSXAudio_UnAttachAudioBank");
    void StartEvent() __asm__("cSSXAudio_StartEvent");
    void OnFinishEvent(cRider* rider) __asm__("cSSXAudio_OnFinishEvent");
    void RestartEvent() __asm__("cSSXAudio_RestartEvent");
    void PartialRestart() __asm__("cSSXAudio_PartialRestart");
    void ResetAudioVariables() __asm__("cSSXAudio_ResetAudioVariables");
    void StartEventWithNoStartGate() __asm__("cSSXAudio_StartEventWithNoStartGate");
    void SetSFXVolume() __asm__("cSSXAudio_SetSFXVolume");
    void SetDJSpeech(int on) __asm__("cSSXAudio_SetDJSpeech");
    void SetArcadeSFX(int on) __asm__("cSSXAudio_SetArcadeSFX");
    int GetSFXVolume() __asm__("cSSXAudio_GetSFXVolume");
    void SetCurrVolume(float volume, int ch) __asm__("cSSXAudio_SetCurrVolume");
    float GetCurrSetVolume(int volumeIndex) __asm__("cSSXAudio_GetCurrSetVolume");
    float GetCurrAppliedVolume(int volumeIndex, int charIndex) __asm__("cSSXAudio_GetCurrAppliedVolume");
    void* GetCurrAppliedVolumePtr(int volumeIndex, int charIndex) __asm__("cSSXAudio_GetCurrAppliedVolumePtr");
    char* GetCurrSetVolumePtr(int idx) __asm__("cSSXAudio_GetCurrSetVolumePtr");
    void IncCurrVolume(int idx) __asm__("cSSXAudio_IncCurrVolume");
    void DecCurrVolume(int idx) __asm__("cSSXAudio_DecCurrVolume");
    void DuckVolume(int mask, float finalLevel, float time) __asm__("cSSXAudio_DuckVolume");
    void UpdateDuck() __asm__("cSSXAudio_UpdateDuck");
    void DuckToStream(int duckStream, float fadeTime, float duckLevel) __asm__("cSSXAudio_DuckToStream");
    void LoadMixes() __asm__("cSSXAudio_LoadMixes");
    float MixIntToFloat(int i, int pct) __asm__("cSSXAudio_MixIntToFloat");
    void MixVolume(int id) __asm__("cSSXAudio_MixVolume");
    void ChangeMixFinalAndStepValues(int i, int smooth, float target, float time)
        __asm__("cSSXAudio_ChangeMixFinalAndStepValues");
    void UpdateMix() __asm__("cSSXAudio_UpdateMix");
    int GetClosestCamera1(cRider* obj) __asm__("cSSXAudio_GetClosestCamera1");
    bool ReplayMode() __asm__("cSSXAudio_ReplayMode");
    int CameraInRange(t4Vector* pos, float radius) __asm__("cSSXAudio_CameraInRange");
    bool InCameraView(t4Vector* pos, float radius) __asm__("cSSXAudio_InCameraView");
    void InitSoundLevels(int mode) __asm__("cSSXAudio_InitSoundLevels");
    void SetPreset(int mode) __asm__("cSSXAudio_SetPreset");
    void PrevPreset() __asm__("cSSXAudio_PrevPreset");
    void NextPreset() __asm__("cSSXAudio_NextPreset");
    void DelayMonitorTriggers() __asm__("cSSXAudio_DelayMonitorTriggers");
    void ApplyRMSData() __asm__("cSSXAudio_ApplyRMSData");
    void DuckByRMS(float duckLevel) __asm__("cSSXAudio_DuckByRMS");
    void CancelRMSData() __asm__("cSSXAudio_CancelRMSData");
    void UpdateAudioWorldPainter(cRider* rider) __asm__("cSSXAudio_UpdateAudioWorldPainter");
    bool NeedWPAmbience() __asm__("cSSXAudio_NeedWPAmbience");
    void ReactToWPAmbienceEvent(cRider* rider, tWPInGameData* obj) __asm__("cSSXAudio_ReactToWPAmbienceEvent");
    void PlayAniTag(cRider* rider, int type) __asm__("cSSXAudio_PlayAniTag");
    void PauseAudio() __asm__("cSSXAudio_PauseAudio");
    void ResumeAudio() __asm__("cSSXAudio_ResumeAudio");
    int GetCharacterLevel(cRider* src) __asm__("cSSXAudio_GetCharacterLevel");
    void ReactToWPMixEvent(int id, tWPInGameData* obj) __asm__("cSSXAudio_ReactToWPMixEvent");
    int BEAudioModeToAudioMode(unsigned char beMode) __asm__("cSSXAudio_BEAudioModeToAudioMode");
    void* AudioModeToBEAudioMode(int audioMode) __asm__("cSSXAudio_AudioModeToBEAudioMode");
    void* GetCurrentSoundMode() __asm__("cSSXAudio_GetCurrentSoundMode__FPv");
    int PlayIntroMovieWithDJ() __asm__("cSSXAudio_PlayIntroMovieWithDJ__FPv");
    void SwapBankStopped() __asm__("cSSXAudio_SwapBankStopped__FPv");
    void CountdownStart() __asm__("cSSXAudio_CountdownStart__FPv");
    void AvalancheGroundHit() __asm__("cSSXAudio_AvalancheGroundHit__FPv");
    void* GetCameraPos(int viewport) __asm__("cSSXAudio_GetCameraPos__FPvi");
    void HandleStreamChunk() __asm__("cSSXAudio_HandleStreamChunk__FPv");
    void RiderCreated() __asm__("cSSXAudio_RiderCreated__FPv");
    void* RiderDeleted() __asm__("cSSXAudio_RiderDeleted__FPv");
    void BeginChatter(cRider* obj) __asm__("cSSXAudio_BeginChatter");
    void BeginGlide(cRider* obj, int n) __asm__("cSSXAudio_BeginGlide");
    void BeginCarve(cRider* obj, int n) __asm__("cSSXAudio_BeginCarve");
    void HardGlideProgram(cRider* rider, int* volume, int* pitch) __asm__("cSSXAudio_HardGlideProgram");
    void SoftGlideProgram(cRider* rider, int* volume, int* pitch) __asm__("cSSXAudio_SoftGlideProgram");
    void RailGlideProgram(cRider* rider, int* volume, int* pitch) __asm__("cSSXAudio_RailGlideProgram");
    void RiderCollision(cRider* rider, cRider* opponent) __asm__("cSSXAudio_RiderCollision");
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
CHECK_OFFSET(cSSXAudio, mFrontEndDJVolume, 0x5730);
CHECK_OFFSET(cSSXAudio, mGruntPatchCycle, 0x5734);
CHECK_OFFSET(cSSXAudio, mSpeechVolumeID, 0x5738);
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
CHECK_OFFSET(cSSXAudio, mSongAnnouncePending, 0x5774);
CHECK_OFFSET(cSSXAudio, mSongAnnounceNum, 0x5778);
CHECK_OFFSET(cSSXAudio, unk_577C, 0x577C);
CHECK_OFFSET(cSSXAudio, unk_5780, 0x5780);
CHECK_OFFSET(cSSXAudio, unk_5784, 0x5784);
CHECK_OFFSET(cSSXAudio, mRiderRemarkPending, 0x5788);
CHECK_OFFSET(cSSXAudio, unk_578C, 0x578C);
CHECK_OFFSET(cSSXAudio, unk_5790, 0x5790);
CHECK_OFFSET(cSSXAudio, unk_5794, 0x5794);
CHECK_OFFSET(cSSXAudio, unk_5798, 0x5798);
CHECK_OFFSET(cSSXAudio, mFirstTimeOnTrack, 0x579C);
CHECK_OFFSET(cSSXAudio, unk_57F8, 0x57F8);
CHECK_OFFSET(cSSXAudio, unk_57FC, 0x57FC);
CHECK_OFFSET(cSSXAudio, unk_5800, 0x5800);
CHECK_OFFSET(cSSXAudio, unk_5804, 0x5804);
CHECK_OFFSET(cSSXAudio, unk_5808, 0x5808);
CHECK_OFFSET(cSSXAudio, unk_580C, 0x580C);
CHECK_OFFSET(cSSXAudio, unk_5810, 0x5810);
CHECK_OFFSET(cSSXAudio, unk_5814, 0x5814);
CHECK_OFFSET(cSSXAudio, unk_5818, 0x5818);
CHECK_OFFSET(cSSXAudio, mLastUberLevel, 0x581C);
CHECK_OFFSET(cSSXAudio, mUberViewIndex, 0x5820);
CHECK_OFFSET(cSSXAudio, mGateDropHeat, 0x5824);
CHECK_OFFSET(cSSXAudio, mStopSpeechOnResume, 0x5828);
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
CHECK_OFFSET(cSSXAudio, mPausedSFXForMapScreen, 0x6088);
CHECK_OFFSET(cSSXAudio, mRadioModeID, 0x608C);
CHECK_OFFSET(cSSXAudio, mFrontEndRadioModeID, 0x6090);
CHECK_OFFSET(cSSXAudio, unk_6094, 0x6094);
CHECK_OFFSET(cSSXAudio, mRadioPlayList, 0x6098);
CHECK_OFFSET(cSSXAudio, unk_60A0, 0x60A0);
CHECK_OFFSET(cSSXAudio, mPlayListName, 0x60A4);
CHECK_OFFSET(cSSXAudio, mNumPlayLists, 0x6234);
CHECK_OFFSET(cSSXAudio, mPlayListIndex, 0x6238);
CHECK_OFFSET(cSSXAudio, unk_623C, 0x623C);
CHECK_OFFSET(cSSXAudio, unk_6240, 0x6240);
CHECK_OFFSET(cSSXAudio, unk_6244, 0x6244);
CHECK_OFFSET(cSSXAudio, unk_6248, 0x6248);
CHECK_OFFSET(cSSXAudio, unk_624C, 0x624C);
CHECK_OFFSET(cSSXAudio, unk_6250, 0x6250);
CHECK_OFFSET(cSSXAudio, unk_6254, 0x6254);
CHECK_OFFSET(cSSXAudio, unk_6258, 0x6258);
CHECK_OFFSET(cSSXAudio, mpMusicTriggerQuery, 0x625C);
CHECK_OFFSET(cSSXAudio, mLastMusicTriggerType, 0x6264);
CHECK_OFFSET(cSSXAudio, mLastMusicTriggerValue, 0x6268);
CHECK_OFFSET(cSSXAudio, unk_626C, 0x626C);
CHECK_OFFSET(cSSXAudio, mMusicTriggerReady, 0x6270);
CHECK_OFFSET(cSSXAudio, mMusicRequestPending, 0x6274);
CHECK_OFFSET(cSSXAudio, mMusicRequest, 0x6278);
CHECK_OFFSET(cSSXAudio, mRaceMusicOn, 0x627C);
CHECK_OFFSET(cSSXAudio, mPathfinderCanChangeVolume, 0x6280);
CHECK_OFFSET(cSSXAudio, mTransportTrack, 0x6284);
CHECK_OFFSET(cSSXAudio, mMusicRampState, 0x6288);
CHECK_OFFSET(cSSXAudio, mMusicRampCounter, 0x628C);
CHECK_OFFSET(cSSXAudio, mFrontEndMusicMode, 0x6290);
CHECK_OFFSET(cSSXAudio, unk_6294, 0x6294);
CHECK_OFFSET(cSSXAudio, mReplaySongPlaying, 0x6298);
CHECK_OFFSET(cSSXAudio, mLastMusicState, 0x629C);
CHECK_OFFSET(cSSXAudio, mMusicEventOwner, 0x62A0);
CHECK_OFFSET(cSSXAudio, unk_62A4, 0x62A4);
CHECK_OFFSET(cSSXAudio, mLoadingLoopHandle, 0x62A8);
CHECK_OFFSET(cSSXAudio, mSavedSongNum, 0x62AC);
CHECK_OFFSET(cSSXAudio, mAudioMode, 0x62B0);
CHECK_OFFSET(cSSXAudio, mArcadeSFX, 0x62B4);
CHECK_OFFSET(cSSXAudio, mDJSpeech, 0x62B8);
CHECK_OFFSET(cSSXAudio, mSoundVolume, 0x62BC);
CHECK_OFFSET(cSSXAudio, mCharacterVolume, 0x636C);
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
