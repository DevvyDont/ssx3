#ifndef SOUND_BANKMONITOR_H
#define SOUND_BANKMONITOR_H

// cMonitor and cBankMonitor: the sound system's monitors. EA source d:/ssx/code/source/sound/bankmonitor.cpp (the
// preview's assert strings; retail unit sound/bankmonitor, 0x2ACD08..0x2ADE88).
//   cSndPlayOpts : virtual cSoundSys          the play-option stack (ctor cSndPlayOpts_cSndPlayOpts 0x2A9350, in
//                                            sound/ps2soundman; non-virtual part 0x88)
//   cMonitor : cSndPlayOpts                  48 timed callbacks (cMonitorInstance); ctor cMonitor_cMonitor, dtor
//                                            cMonitor__cMonitor; vtable cMonitor_vtbl (0x483BA8); non-virtual part
//       0x8DC
//   cBankMonitor : cMonitor                  64 monitored sounds (cBankMonitorInstance, 0xC0 each, defined with the
//                                            streamsys unit's methods 0x2AB9E0..); ctor cBankMonitor_cBankMonitor,
//                                            dtor cBankMonitor__cBankMonitor; vtable cBankMonitor_vtbl (0x483B48);
//       non-virtual 0x8F0
// cSoundSys (0x8: pManager + its vptr) is a virtual base: g++ 2.95 puts it after the non-virtual part of the complete
// object (cMonitor + 0x8DC, cBankMonitor + 0x8F0; On Tour sizes 0x8E4 / 0x8F4) and gives the class a pointer to it at
// 0x0. The structs below are the NON-VIRTUAL parts, so they can be embedded where the object lives: the only
// cBankMonitor is the one inside cBankManager (+0x1D8), i.e. cSSXAudio + 0x6E70 (sound/ssxaudio.h). gMonitor is the
// current cMonitor (cMonitor_cMonitor stores this, cMonitor__cMonitor clears it; the static tick func_002AD998 calls
// its
// virtual slot 1, MONITOR_Update).
//
// A FLAT layout, like main/game.h: every field at its retail offset, bases written out inline, no C++ inheritance.
// Evidence tags:
//   [R] retail constructors (cSndPlayOpts_cSndPlayOpts allocates each per-level array with operator new[](n, "<member
//       name>"), cMonitor_cMonitor, cBankMonitor_cBankMonitor) and destructors;
//   [P] the July 2003 preview's inline accessors (MONITOR_*, BANKMONITOR_*), carried to retail by
// tools/layout_xlate.py;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cSndPlayOpts 0x94, cMonitor 0x8E4, cBankMonitor 0x8F4, cMonitorInstance 0x2C,
//       SNDPLAYOPTS 0x18, cDelayOpts 0x10): same member order; retail matches On Tour's offsets except that On Tour's
//       cMonitor members sit 4 bytes later and its bools are bytes (retail's are words).

// cBankMonitorInstance (0xC0) is one of cBankMonitor's 64 monitored sounds (a bank sample, a MIDI sequence or a stream
// playing with the current play options; 3D position / Doppler / distance volume, fades, restart when back in range).
// EA defines it in this file too: the preview's bankmonitor.cpp asserts start with its "mMonitorType == MONITOR_BANK",
// "mMonitorType == MONITOR_STREAM", "mMonitorType == MONITOR_MIDI", "!mFading", "mHandle < 0 || mFading". Retail's
// methods sit just before the splat split at 0x2ACD08 (cBankMonitorInstance_Update 0x2AB6B0, then
// cBankMonitorInstance_cBankMonitorInstance 0x2AB9E0 .. cBankMonitorInstance_Fade 0x2ACB80, splat unit
// sound/streamsys). Size 0xC0 [R] (cBankMonitor ctor: operator new[](64 x 0xC0, "mBankMonitorInstance") with
// cBankMonitorInstance_cBankMonitorInstance) [O] (On Tour 0xC0). Retail follows On Tour's member order and offsets up
// to mCloseOnEnd; On Tour's bools are bytes (mCloseOnEnd 0x9C, mTwoPlayerPan 0x9D), retail's are words, so retail's
// fields from mTwoPlayerPan on sit 4 bytes later (mCallback 0xA8, mLastPitch 0xB0 .. mLastAzimuth 0xB8), which fills
// On Tour's tail padding.

#include "layout.h"
#include "bx/math3d.h"
#include "ealib/snd.h"

struct cSoundSys;
class cController;
class cBankMonitorInstance;
struct cStreamInstance;

// A sound callback: a g++ 2.95 pointer to a cController member function (8 bytes: delta, index, function), as
// cSndPlayOpts::OPTS_Callback takes it (preview .gnu.linkonce.t.OPTS_Callback__12cSndPlayOptsPM11cControllerFP11
// cControllerP20cBankMonitorInstancef_v). sound/bankmonitor.cpp and sound/soundsys.cpp call the same type sSndCallback.
typedef void (cController::*tSndCallback)(cController*, cBankMonitorInstance*, float);

// EA audio library play options: SNDPLAYOPTS (On Tour 0x18) is in ealib/snd.h (shared with the C library unit).


// One cSndPlayOpts::mDelay level (On Tour struct cDelayOpts, 0x10); [U] reset to 100 / 0 / 90 / 50 on every push
// (func_002B35A0).
struct cDelayOpts {
    int mTime;     // 0x0 [O]
    int mCount;    // 0x4 [O]
    int mFeedback; // 0x8 [O]
    int mLevel;    // 0xC [O]
};

// One of cMonitor's 48 timed callbacks (On Tour class cMonitorInstance, 0x2C). Methods 0x2AD848..0x2AD998: ctor
// cMonitorInstance_cMonitorInstance__FPv, dtor cMonitorInstance__cMonitorInstance, Create cMonitorInstance_Create,
// IsFree cMonitorInstance_IsFree, Reset cMonitorInstance_Reset, CountReset
// cMonitorInstance_CountReset__FPvi, Enable cMonitorInstance_Enable__FPv, Disable cMonitorInstance_Disable__FPv (On
// Tour's order).
struct cMonitorInstance {
    int mEnabled;           // 0x00 [O]; [U] Enable stores 1, Disable 0
    int mCountReset;        // 0x04 [O]; [R] ctor -1; [U] CountReset stores it and mCountdown; IsFree tests == -1
    // mCountReset of an unused instance (name ours). [R] ctor -1; [U] Reset passes it to CountReset, IsFree tests it.
    enum { kFree = -1 };
    int mCountdown;         // 0x08 [O]; [U] CountReset
    tSndCallback mCallback; // 0x0C [O] (On Tour: a member-function pointer); [U] Create copies 8 bytes (sdl/sdr);
                            //       cMonitor_MONITOR_DestroyAll1 compares its index (0xE) and function (0x10)
    int mCounter;           // 0x14 [O]
    void* mPtr;             // 0x18 [O]; [U] Create stores its Ptr argument
    int mData1;             // 0x1C [O]; [U] Create
    int mData2;             // 0x20 [O]; [U] Create
    int mData3;             // 0x24 [O]; [U] Create
    int mData4;             // 0x28 [O]; [U] Create

    // Methods bound to their retail symbols by asm labels.
    void Create(int Freq, long key, int Ptr, int Data1, int Data2, int Data3, int Data4)
        __asm__("cMonitorInstance_Create");
    bool IsFree() __asm__("cMonitorInstance_IsFree");
    void Reset() __asm__("cMonitorInstance_Reset");
    void CountReset(int periodMs) __asm__("cMonitorInstance_CountReset__FPvi");
    void Disable() __asm__("cMonitorInstance_Disable__FPv");
};

// cBankMonitorInstance::mRestartInfo (On Tour: an anonymous union __C1 of three anonymous structs, members bank /
// stream / midi; the type names here are ours): what RestartSound needs to start the sound again.
struct tBankRestartInfo {           // On Tour __C1 (8); [U] cBankMonitorInstance_SetBank
    int mBank;                      // 0x0 [O]; [U] SetBank; RestartSound passes it to SNDBANK_play;
                                    //       BANKMONITOR_StopBank compares it (On Tour GetBank)
    int mPatch;                     // 0x4 [O]; [U] SetBank; RestartSound passes it to SNDBANK_play
};
struct tStreamRestartInfo {         // On Tour __C2 (0x48); [U] cBankMonitorInstance_SetStream
    cStreamInstance* mStreamID;     // 0x00 [O]; [U] SetStream; PosUpdate / PitchUpdate / VolUpdate / Fade call
                                    //       cStreamInstance_* on it
    char mFilename[64];             // 0x04 [O]; [U] SetStream strcpy()s it
    int mOffset;                    // 0x44 [O]; [U] SetStream
};
struct tMIDIRestartInfo {           // On Tour __C3 (8); [U] cBankMonitorInstance_SetMIDI
    void* mpData;                   // 0x0 [O]; [U] SetMIDI; RestartSound passes it to SNDMIDI_play
    int mBank;                      // 0x4 [O]; [U] SetMIDI; RestartSound passes it to SNDMIDI_play
};
union tRestartInfo {                // On Tour __C1 (0x48)
    tBankRestartInfo bank;          // 0x0 [O]
    tStreamRestartInfo stream;      // 0x0 [O]
    tMIDIRestartInfo midi;          // 0x0 [O]
};

// cBankMonitorInstance::mMonitorType: what the monitored sound is. [O] On Tour's enum eMonitorType, names and values
// (the preview's asserts "mMonitorType == MONITOR_BANK" / "MONITOR_STREAM" / "MONITOR_MIDI"). [U] DonePlaying,
// PosUpdate, VolUpdate, StopSound ... call SND* for 1 (a bank sample), SNDseq* / SNDMIDI_* for 2 (a MIDI sequence) and
// cStreamInstance_* for 3 (a stream); 0 is a free instance (Reset stores it, IsFree tests it).
enum eMonitorType { MONITOR_NONE = 0, MONITOR_BANK = 1, MONITOR_MIDI = 2, MONITOR_STREAM = 3 };

// One of cBankMonitor's 64 monitored sounds (On Tour class cBankMonitorInstance, 0xC0). mMonitorType is an
// eMonitorType kept as an int.
struct cController;

struct cBankMonitorInstance {
    int mMonitorType;               // 0x00 [O]; [R] ctor 0; [U] DonePlaying / PosUpdate / PitchUpdate / VolUpdate /
                                    //       StopSound / RestartSound / Fade switch on it (1 SND*, 2 SNDseq* /
                                    //       SNDMIDI_*, 3 cStreamInstance_*); Reset 0; IsFree / Update3D test 0;
                                    //       BANKMONITOR_Create stores its Type, BANKMONITOR_GetHandle / StopBank
                                    //       compare it
    int mHandle;                    // 0x04 [O]; [U] the SND / SNDseq / SNDSTRM handle every switch passes on;
                                    //       RestartSound stores the new one; Update3D -1 when a fade is done;
                                    //       BANKMONITOR_Create stores its Handle, BANKMONITOR_GetHandle compares it
    int mActive;                    // 0x08 [O] (On Tour bool); [U] StopSound 0, RestartSound 1, CheckActive tests it;
                                    //       BANKMONITOR_Create stores Handle >= 0, BANKMONITOR_Active counts it
    tRestartInfo mRestartInfo;      // 0x0C [O]; [U] SetBank / SetMIDI (0xC, 0x10), SetStream (0xC, strcpy 0x10, 0x50),
                                    //       RestartSound (0xC, 0x10), Pos/Pitch/VolUpdate and Fade (stream 0xC)
    int mFading;                    // 0x54 [O]; [R] ctor 0; [U] StopSound / Fade(-1) set it, RestartSound / Reset clear
                                    //       it, Update3D / Pause / Fade test it (preview asserts "!mFading", "mHandle <
                                    //       0 || mFading")
    int mCloseStateLocked;          // 0x58 [O] (On Tour bool); [R] ctor 0; [U] Fade(ResetMonitor) sets it, Reset clears
                                    //       it; BANKMONITOR_Create copies mCloseOnEnd only while it is clear
    float mLastDist;                // 0x5C [O]; [U] SetLastDistance (sqrt of the monitor's Calc3DPos), Update3D
                                    //       (Doppler from the previous distance, -1.0 = none); BANKMONITOR_CameraChange
                                    //       sets -1.0
    float* mVolControl;             // 0x60 [O]; [U] BANKMONITOR_Create copies mVolControl[mInst]; Update3D scales the
                                    //       volume by *it; RestartSound puts it back into the play options
    int mBaseVol;                   // 0x64 [O]; [U] BANKMONITOR_Create stores BaseVol; Update3D (the volume before
                                    //       distance attenuation)
    const t4Vector* mPosition;      // 0x68 [O]; [U] BANKMONITOR_Create copies mPosition[mInst] (or &mConstPos);
                                    //       Update3D tests it
    char unk_6C[0x4];               // 0x6C padding (mConstPos is 16-aligned); no access seen
    t4Vector mConstPos;             // 0x70 [O]; [U] BANKMONITOR_Create copies *mPosition here when mPosStatic[mInst] is
                                    //       set
    void* mPtr;                     // 0x80 [O]; [U] BANKMONITOR_Create copies mPtr[mInst];
                                    //       BANKMONITOR_DestroyAllWithPtr compares it
    int mViewport;                  // 0x84 [O]; [U] BANKMONITOR_Create copies mViewport[mInst]
    float mVanish;                  // 0x88 [O]; [U] BANKMONITOR_Create; Update3D (the Dist2Vol range, 0 -> 30.0)
    float mRadius;                  // 0x8C [O]; [U] BANKMONITOR_Create; CheckActive (0 = always active)
    float mVolMultiplier;           // 0x90 [O]; [R] ctor -1.0; [U] BANKMONITOR_Create -1.0; VolUpdate scales by it when
                                    //       >= 0
    int mExternalVol;               // 0x94 [O] (On Tour bool); [U] BANKMONITOR_Create; Update3D (use mBaseVol, skip
                                    //       CheckActive)
    int mMapToMaxVol;               // 0x98 [O]; [U] BANKMONITOR_Create; Update3D rescales by 127 / it
    int mCloseOnEnd;                // 0x9C [O] (On Tour bool 0x9C); [U] IsFree releases a done sound only when set;
                                    //       Fade(ResetMonitor) sets it; BANKMONITOR_Create
    int mTwoPlayerPan;              // 0xA0 [O] (On Tour bool 0x9D); [U] BANKMONITOR_Create copies mTwoPlayerPan[mInst]
    int mTwoPlayerSide;             // 0xA4 [O] (On Tour 0xA0); [U] BANKMONITOR_Create copies mTwoPlayerSide[mInst]
    tSndCallback mCallback;         // 0xA8 [O] (On Tour 0xA4, a member-function pointer); [U]
                                    //       cBankMonitorInstance_Update calls it on the cController (lh 0xA8 / 0xAA, lw
                                    //       0xAC); BANKMONITOR_Create copies it; BANKMONITOR_DestroyAll1 compares it
                                    //       (On Tour Calls)
    int mLastPitch;                 // 0xB0 [O] (On Tour 0xAC); [R] ctor 0x8000; [U] PitchUpdate's cache; Reset 0x8000
    int mLastVolume;                // 0xB4 [O] (On Tour 0xB0); [R] ctor -1; [U] VolUpdate's cache; Reset -1
    int mLastAzimuth;               // 0xB8 [O] (On Tour 0xB4); [R] ctor -30000; [U] PosUpdate's cache; Reset 0xFFFF
    char unk_BC[0x4];               // 0xBC tail padding (the class is 16-aligned); no access seen

    // Methods bound to their retail symbols by asm labels.
    void Update(cController* controller, float time) __asm__("cBankMonitorInstance_Update");
    int DonePlaying() __asm__("cBankMonitorInstance_DonePlaying");
    void Reset() __asm__("cBankMonitorInstance_Reset");
    bool IsFree() __asm__("cBankMonitorInstance_IsFree");
    float DistSq(float* posA, float* posB) __asm__("cBankMonitorInstance_DistSq");
    void SetBank(int bank, int patch) __asm__("cBankMonitorInstance_SetBank__FPvii");
    void SetStream(int streamInst, const char* name, int fileOffset) __asm__("cBankMonitorInstance_SetStream");
    void SetMIDI(int midiData, int bank) __asm__("cBankMonitorInstance_SetMIDI__FPvii");
    void PosUpdate(int pos, int elevation) __asm__("cBankMonitorInstance_PosUpdate");
    void PitchUpdate(int vol) __asm__("cBankMonitorInstance_PitchUpdate");
    void VolUpdate(int vol) __asm__("cBankMonitorInstance_VolUpdate");
    void StopSound() __asm__("cBankMonitorInstance_StopSound");
    bool CheckActive(float radius) __asm__("cBankMonitorInstance_CheckActive");
    void Fade(int n, int resetMonitor, float duration) __asm__("cBankMonitorInstance_Fade");

    // Fade's level that fades the sound out and stops it (name ours). [U] Fade sets mFading for -1 (a stream is
    // purged) and clamps any other level to 0..127; cBankMonitor's BANKMONITOR_FadeOut / StopHandle pass it.
    enum { kFadeStop = -1 };
};

CHECK_OFFSET(tBankRestartInfo, mBank, 0x0);
CHECK_OFFSET(tBankRestartInfo, mPatch, 0x4);
CHECK_SIZE(tBankRestartInfo, 0x8);
CHECK_OFFSET(tStreamRestartInfo, mStreamID, 0x00);
CHECK_OFFSET(tStreamRestartInfo, mFilename, 0x04);
CHECK_OFFSET(tStreamRestartInfo, mOffset, 0x44);
CHECK_SIZE(tStreamRestartInfo, 0x48);
CHECK_OFFSET(tMIDIRestartInfo, mpData, 0x0);
CHECK_OFFSET(tMIDIRestartInfo, mBank, 0x4);
CHECK_SIZE(tMIDIRestartInfo, 0x8);
CHECK_SIZE(tRestartInfo, 0x48);

CHECK_OFFSET(cBankMonitorInstance, mMonitorType, 0x00);
CHECK_OFFSET(cBankMonitorInstance, mHandle, 0x04);
CHECK_OFFSET(cBankMonitorInstance, mActive, 0x08);
CHECK_OFFSET(cBankMonitorInstance, mRestartInfo, 0x0C);
CHECK_OFFSET(cBankMonitorInstance, mFading, 0x54);
CHECK_OFFSET(cBankMonitorInstance, mCloseStateLocked, 0x58);
CHECK_OFFSET(cBankMonitorInstance, mLastDist, 0x5C);
CHECK_OFFSET(cBankMonitorInstance, mVolControl, 0x60);
CHECK_OFFSET(cBankMonitorInstance, mBaseVol, 0x64);
CHECK_OFFSET(cBankMonitorInstance, mPosition, 0x68);
CHECK_OFFSET(cBankMonitorInstance, unk_6C, 0x6C);
CHECK_OFFSET(cBankMonitorInstance, mConstPos, 0x70);
CHECK_OFFSET(cBankMonitorInstance, mPtr, 0x80);
CHECK_OFFSET(cBankMonitorInstance, mViewport, 0x84);
CHECK_OFFSET(cBankMonitorInstance, mVanish, 0x88);
CHECK_OFFSET(cBankMonitorInstance, mRadius, 0x8C);
CHECK_OFFSET(cBankMonitorInstance, mVolMultiplier, 0x90);
CHECK_OFFSET(cBankMonitorInstance, mExternalVol, 0x94);
CHECK_OFFSET(cBankMonitorInstance, mMapToMaxVol, 0x98);
CHECK_OFFSET(cBankMonitorInstance, mCloseOnEnd, 0x9C);
CHECK_OFFSET(cBankMonitorInstance, mTwoPlayerPan, 0xA0);
CHECK_OFFSET(cBankMonitorInstance, mTwoPlayerSide, 0xA4);
CHECK_OFFSET(cBankMonitorInstance, mCallback, 0xA8);
CHECK_OFFSET(cBankMonitorInstance, mLastPitch, 0xB0);
CHECK_OFFSET(cBankMonitorInstance, mLastVolume, 0xB4);
CHECK_OFFSET(cBankMonitorInstance, mLastAzimuth, 0xB8);
CHECK_OFFSET(cBankMonitorInstance, unk_BC, 0xBC);
CHECK_SIZE(cBankMonitorInstance, 0xC0);

// cSndPlayOpts on its own (its methods: cSndPlayOpts_* in bx/seg_1BA100 and sound/ps2soundman): the same non-virtual
// part
// that opens cMonitor below (0x88 bytes; the virtual base cSoundSys follows it in a complete object, so only the
// non-virtual size is checked).
struct t4Vector;

struct cSndPlayOpts {
    cSoundSys* mpVSoundSys;          // 0x000 [R] g++ 2.95 virtual-base pointer to cSoundSys (placed after the 0x88-byte
                                     //       non-virtual part in a complete object); every ctor/dtor in the chain loads
                                     //       it to patch the cSoundSys vptr
    SNDPLAYOPTS mDefault;            // 0x004 [O]; [U] OPTS push copies it into mCurrent[++mInst] (func_002B35A0)
    volatile int mInst;              // 0x01C [O] (volatile); [R] ctor; [U] every OPTS_ access indexes the arrays below
                                     //       with it
    SNDPLAYOPTS* mCurrent;           // 0x020 [R] operator new[](0x60, "mCurrent") (4 levels)
    const t4Vector* mPosition[4];    // 0x024 [O]; [U] BANKMONITOR_Create copies mPosition[mInst]
    int* mPosStatic;                 // 0x034 [R] operator new[](0x10, "mPosStatic") (On Tour bool*: retail words)
    int* mBaseVol;                   // 0x038 [R] operator new[](0x10, "mBaseVol")
    float** mVolControl;             // 0x03C [R] operator new[](0x10, "mVolControl")
    tSndCallback mCallback[4];       // 0x040 [O]; [P] OPTS_Callback stores mCallback[mInst] (sdl/sdr)
    void** mPtr;                     // 0x060 [R] operator new[](0x10, "mPtr")
    int* mViewport;                  // 0x064 [R] operator new[](0x10, "mViewport")
    float* mVanish;                  // 0x068 [R] operator new[](0x10, "mVanish")
    float* mRadius;                  // 0x06C [R] operator new[](0x10, "mRadius")
    cDelayOpts* mDelay;              // 0x070 [R] operator new[](0x40, "mDelay")
    int* mExternalVol;               // 0x074 [R] operator new[](0x10, "mExternalVol") (On Tour bool*); [P]
                                     //       OPTS_ExternalVol
    int* mMapToMaxVol;               // 0x078 [R] operator new[](0x10, "mMapToMaxVol")
    int* mCloseOnEnd;                // 0x07C [R] operator new[](0x10, "mCloseOnEnd") (On Tour bool*)
    int* mTwoPlayerPan;              // 0x080 [R] operator new[](0x10, "mTwoPlayerPan") (On Tour bool*)
    int* mTwoPlayerSide;             // 0x084 [R] operator new[](0x10, "mTwoPlayerSide")

    // Methods bound to their retail symbols by asm labels.
    int OPTS_Instance() __asm__("cSndPlayOpts_OPTS_Instance");
    int OPTS_Current() __asm__("cSndPlayOpts_OPTS_Current");
    int OPTS_GetPos() __asm__("cSndPlayOpts_OPTS_GetPos");
    int OPTS_GetStaticPos() __asm__("cSndPlayOpts_OPTS_GetStaticPos");
    int OPTS_GetVol() __asm__("cSndPlayOpts_OPTS_GetVol");
    float* OPTS_GetVolControl() __asm__("cSndPlayOpts_OPTS_GetVolControl");
    tSndCallback OPTS_GetCallback() __asm__("cSndPlayOpts_OPTS_GetCallback");
    unsigned short OPTS_GetAzimuth() __asm__("cSndPlayOpts_OPTS_GetAzimuth");
    short OPTS_GetElevation() __asm__("cSndPlayOpts_OPTS_GetElevation");
    int OPTS_GetDelay() __asm__("cSndPlayOpts_OPTS_GetDelay");
    void* OPTS_GetPtr() __asm__("cSndPlayOpts_OPTS_GetPtr");
    int OPTS_GetViewport() __asm__("cSndPlayOpts_OPTS_GetViewport");
    float OPTS_GetVanish() __asm__("cSndPlayOpts_OPTS_GetVanish");
    float OPTS_GetRadius() __asm__("cSndPlayOpts_OPTS_GetRadius");
    int OPTS_GetExternalVol() __asm__("cSndPlayOpts_OPTS_GetExternalVol");
    int OPTS_GetMapToMaxVol() __asm__("cSndPlayOpts_OPTS_GetMapToMaxVol");
    int OPTS_GetCloseOnEnd() __asm__("cSndPlayOpts_OPTS_GetCloseOnEnd");
    int OPTS_GetTwoPlayerPan() __asm__("cSndPlayOpts_OPTS_GetTwoPlayerPan");
    int OPTS_GetTwoPlayerSide() __asm__("cSndPlayOpts_OPTS_GetTwoPlayerSide");
    void OPTS_LibDefault() __asm__("cSndPlayOpts_OPTS_LibDefault");
    void OPTS_SetDefault() __asm__("cSndPlayOpts_OPTS_SetDefault");
    void OPTS_Reset() __asm__("cSndPlayOpts_OPTS_Reset");
    void OPTS_Open() __asm__("cSndPlayOpts_OPTS_Open");
    void OPTS_Close() __asm__("cSndPlayOpts_OPTS_Close");
    void OPTS_Key(signed char value) __asm__("cSndPlayOpts_OPTS_Key");
    void OPTS_Velocity(signed char value) __asm__("cSndPlayOpts_OPTS_Velocity");
    void OPTS_Vol(int vol) __asm__("cSndPlayOpts_OPTS_Vol");
    void OPTS_ScaleVol(float scale) __asm__("cSndPlayOpts_OPTS_ScaleVol");
    void OPTS_ScaleVol1(int n) __asm__("cSndPlayOpts_OPTS_ScaleVol1");
    void OPTS_Bend(signed char value) __asm__("cSndPlayOpts_OPTS_Bend");
    void OPTS_PitchMult(short value) __asm__("cSndPlayOpts_OPTS_PitchMult");
    void OPTS_DelayCount(int value) __asm__("cSndPlayOpts_OPTS_DelayCount");
    void OPTS_DelayTime(int value) __asm__("cSndPlayOpts_OPTS_DelayTime");
    void OPTS_DelayFeedback(int value) __asm__("cSndPlayOpts_OPTS_DelayFeedback");
    void OPTS_DelayLevel(int value) __asm__("cSndPlayOpts_OPTS_DelayLevel");
    void OPTS_Azimuth(short value) __asm__("cSndPlayOpts_OPTS_Azimuth");
    void OPTS_Elevation(short value) __asm__("cSndPlayOpts_OPTS_Elevation");
    void OPTS_Position(const t4Vector* position) __asm__("cSndPlayOpts_OPTS_Position");
    void OPTS_StaticPos() __asm__("cSndPlayOpts_OPTS_StaticPos");
    void OPTS_Viewport(int viewport) __asm__("cSndPlayOpts_OPTS_Viewport");
    void OPTS_LowPassCutOffFrequency(int value) __asm__("cSndPlayOpts_OPTS_LowPassCutOffFrequency");
    void OPTS_Ptr(void* userPtr) __asm__("cSndPlayOpts_OPTS_Ptr");
    void OPTS_VolControl(float* volControl) __asm__("cSndPlayOpts_OPTS_VolControl");
    void OPTS_Callback(tSndCallback callback) __asm__("cSndPlayOpts_OPTS_Callback");
    void OPTS_ExternalVol(int externalVol) __asm__("cSndPlayOpts_OPTS_ExternalVol");
    void OPTS_MapToMaxVol(int mapToMaxVol) __asm__("cSndPlayOpts_OPTS_MapToMaxVol");
    void OPTS_CloseOnEnd(int closeOnEnd) __asm__("cSndPlayOpts_OPTS_CloseOnEnd");
    void OPTS_TwoPlayerPan(int twoPlayerPan) __asm__("cSndPlayOpts_OPTS_TwoPlayerPan");
    void OPTS_TwoPlayerSide(int twoPlayerSide) __asm__("cSndPlayOpts_OPTS_TwoPlayerSide");
    void OPTS_Radius(float dist, float vanish) __asm__("cSndPlayOpts_OPTS_Radius");
};

CHECK_OFFSET(cSndPlayOpts, mDefault, 0x004);
CHECK_OFFSET(cSndPlayOpts, mInst, 0x01C);
CHECK_OFFSET(cSndPlayOpts, mCallback, 0x040);
CHECK_OFFSET(cSndPlayOpts, mTwoPlayerSide, 0x084);
CHECK_SIZE(cSndPlayOpts, 0x88);

struct cMonitor {
    cSoundSys* mpVSoundSys;          // 0x000 [R] g++ 2.95 virtual-base pointer to cSoundSys (= this + 0x8DC in a
                                     //       complete cMonitor); every ctor/dtor in the chain loads it to patch the
                                     //       cSoundSys vptr
    // --- cSndPlayOpts (non-virtual part 0x88; ctor cSndPlayOpts_cSndPlayOpts 0x2A9350; up to 4 nested option levels)
    SNDPLAYOPTS mDefault;            // 0x004 [O]; [U] OPTS push copies it into mCurrent[++mInst] (func_002B35A0)
    volatile int mInst;              // 0x01C [O] (volatile); [R] ctor; [U] every OPTS_ access indexes the arrays below
                                     //       with it
    SNDPLAYOPTS* mCurrent;           // 0x020 [R] operator new[](0x60, "mCurrent") (4 levels)
    const t4Vector* mPosition[4];    // 0x024 [O]; [U] BANKMONITOR_Create copies mPosition[mInst]
    int* mPosStatic;                 // 0x034 [R] operator new[](0x10, "mPosStatic") (On Tour bool*: retail words)
    int* mBaseVol;                   // 0x038 [R] operator new[](0x10, "mBaseVol")
    float** mVolControl;             // 0x03C [R] operator new[](0x10, "mVolControl")
    tSndCallback mCallback[4];       // 0x040 [O]; [P] OPTS_Callback stores mCallback[mInst] (sdl/sdr)
    void** mPtr;                     // 0x060 [R] operator new[](0x10, "mPtr")
    int* mViewport;                  // 0x064 [R] operator new[](0x10, "mViewport")
    float* mVanish;                  // 0x068 [R] operator new[](0x10, "mVanish")
    float* mRadius;                  // 0x06C [R] operator new[](0x10, "mRadius")
    cDelayOpts* mDelay;              // 0x070 [R] operator new[](0x40, "mDelay")
    int* mExternalVol;               // 0x074 [R] operator new[](0x10, "mExternalVol") (On Tour bool*); [P]
                                     //       OPTS_ExternalVol
    int* mMapToMaxVol;               // 0x078 [R] operator new[](0x10, "mMapToMaxVol")
    int* mCloseOnEnd;                // 0x07C [R] operator new[](0x10, "mCloseOnEnd") (On Tour bool*)
    int* mTwoPlayerPan;              // 0x080 [R] operator new[](0x10, "mTwoPlayerPan") (On Tour bool*)
    int* mTwoPlayerSide;             // 0x084 [R] operator new[](0x10, "mTwoPlayerSide")
    // --- cMonitor (On Tour: mCounter 0x8C .. mMonitorInstance 0x9C, 4 bytes later than retail)
    volatile int mCounter;           // 0x088 [P] MONITOR_Counter / MONITOR_Delta (preview 0x7C, seen); [R] ctor 0; [U]
                                     //       MONITOR_Update (cMonitor_MONITOR_Update) increments it unless paused
    volatile int mLastCounter;       // 0x08C [P] MONITOR_Delta (preview 0x80, seen); [R] ctor 0
    volatile int mReEntryCheck;      // 0x090 [O]; [R] ctor 0; [U] MONITOR_Create (cMonitor_MONITOR_Create) ++/-- around
                                     //       the slot search
    int mPaused;                     // 0x094 [P] MONITOR_Paused/Pause/Resume (preview 0x88, seen); [U]
                                     //       cMonitor_MONITOR_Update
    cMonitorInstance mMonitorInstance[48]; // 0x098 [O]; [R] ctor constructs 48 (cMonitorInstance_cMonitorInstance),
                                           //       dtor destroys them; [U]
                                           //       cMonitor_MONITOR_GetFreeInstance/cMonitor_MONITOR_Create/cMonitor_MONITOR_Destroy/cMonitor_MONITOR_DestroyAll/cMonitor_MONITOR_DestroyAll1
    void** mVtbl;                    // 0x8D8 [R] cMonitor_vtbl (cMonitor ctor/dtor); cBankMonitor_vtbl in a
                                     //       cBankMonitor (shared primary vptr)

    // Methods bound to their retail symbols by asm labels.
    void MONITOR_Pause() __asm__("cMonitor_MONITOR_Pause");
    void MONITOR_Resume() __asm__("cMonitor_MONITOR_Resume");
    int MONITOR_Paused() __asm__("cMonitor_MONITOR_Paused");
    int MONITOR_Counter() __asm__("cMonitor_MONITOR_Counter");
    float MONITOR_Delta() __asm__("cMonitor_MONITOR_Delta");
    void MONITOR_Update() __asm__("cMonitor_MONITOR_Update");
    int MONITOR_GetFreeInstance(int n) __asm__("cMonitor_MONITOR_GetFreeInstance");
    void MONITOR_Destroy(int idx) __asm__("cMonitor_MONITOR_Destroy");
    void MONITOR_DestroyAll() __asm__("cMonitor_MONITOR_DestroyAll");
};

struct cBankMonitor {
    cSoundSys* mpVSoundSys;          // 0x000 [R] virtual-base pointer (= this + 0x8F0 in a complete cBankMonitor;
                                     //       cBankManager points it at the shared cSoundSys)
    // --- cSndPlayOpts (see cMonitor)
    SNDPLAYOPTS mDefault;            // 0x004 [O]
    volatile int mInst;              // 0x01C [O]; [U] BANKMONITOR_Create indexes every array with it
    SNDPLAYOPTS* mCurrent;           // 0x020 [R] "mCurrent"
    const t4Vector* mPosition[4];    // 0x024 [O]; [U] BANKMONITOR_Create
    int* mPosStatic;                 // 0x034 [R] "mPosStatic"; [U] BANKMONITOR_Create copies the position when set
    int* mBaseVol;                   // 0x038 [R] "mBaseVol"
    float** mVolControl;             // 0x03C [R] "mVolControl"; [U] BANKMONITOR_Create
    tSndCallback mCallback[4];       // 0x040 [O]; [U] BANKMONITOR_Create (OPTS_GetCallback)
    void** mPtr;                     // 0x060 [R] "mPtr"; [U] BANKMONITOR_Create
    int* mViewport;                  // 0x064 [R] "mViewport"; [U] BANKMONITOR_Create
    float* mVanish;                  // 0x068 [R] "mVanish"; [U] BANKMONITOR_Create
    float* mRadius;                  // 0x06C [R] "mRadius"; [U] BANKMONITOR_Create
    cDelayOpts* mDelay;              // 0x070 [R] "mDelay"
    int* mExternalVol;               // 0x074 [R] "mExternalVol"; [U] BANKMONITOR_Create
    int* mMapToMaxVol;               // 0x078 [R] "mMapToMaxVol"; [U] BANKMONITOR_Create
    int* mCloseOnEnd;                // 0x07C [R] "mCloseOnEnd"; [U] BANKMONITOR_Create
    int* mTwoPlayerPan;              // 0x080 [R] "mTwoPlayerPan"; [U] BANKMONITOR_Create
    int* mTwoPlayerSide;             // 0x084 [R] "mTwoPlayerSide"; [U] BANKMONITOR_Create
    // --- cMonitor (see cMonitor)
    volatile int mCounter;           // 0x088 [P] MONITOR_Counter
    volatile int mLastCounter;       // 0x08C [P] MONITOR_Delta
    volatile int mReEntryCheck;      // 0x090 [O]
    int mPaused;                     // 0x094 [P] MONITOR_Paused
    cMonitorInstance mMonitorInstance[48]; // 0x098 [O]
    void** mVtbl;                    // 0x8D8 [R] cBankMonitor_vtbl (cBankMonitor ctor, cBankMonitor__cBankMonitor, and
                                     //       every ctor/dtor of a class that contains a cBankManager)
    // --- cBankMonitor
    volatile int mReEntry;           // 0x8DC [O] (volatile); [R] ctor 0; [U] BANKMONITOR_Create ++/-- around the
                                     //       instance search
    int mDopplerEnable;              // 0x8E0 [O] (On Tour bool); [R] ctor 1; [P] BANKMONITOR_DopplerEnabled/SetDoppler
                                     //       (preview 0x8D4, bracketed)
    int mPositionalEnable;           // 0x8E4 [O] (On Tour bool); [R] ctor 1; [P]
                                     //       BANKMONITOR_PositionalEnabled/SetPositional (preview 0x8D8, bracketed)
    int mBankMonitorPaused;          // 0x8E8 [O] cBankMonitor::mPaused (renamed: cMonitor's mPaused is at 0x94); [P]
                                     //       BANKMONITOR_Paused (preview 0x8DC, bracketed); [U] BANKMONITOR_Pause
                                     //       (cBankMonitor_BANKMONITOR_Pause) 1, Resume
                                     //       (cBankMonitor_BANKMONITOR_Resume) 0
    cBankMonitorInstance* mBankMonitorInstance; // 0x8EC [O]; [R] operator new[](64 x 0xC0, "mBankMonitorInstance"); [P]
                                                //       BANKMONITOR_GetInstance (preview 0x8E0 -> 0x8EC, seen)

    // Methods bound to their retail symbols by asm labels.
    int BANKMONITOR_GetInstance(int instance) __asm__("cBankMonitor_BANKMONITOR_GetInstance");
    int BANKMONITOR_Paused() __asm__("cBankMonitor_BANKMONITOR_Paused");
    void BANKMONITOR_SetDoppler(int enable) __asm__("cBankMonitor_BANKMONITOR_SetDoppler");
    void BANKMONITOR_SetPositional(int enable) __asm__("cBankMonitor_BANKMONITOR_SetPositional");
    int BANKMONITOR_DopplerEnabled() __asm__("cBankMonitor_BANKMONITOR_DopplerEnabled");
    int BANKMONITOR_PositionalEnabled() __asm__("cBankMonitor_BANKMONITOR_PositionalEnabled");
    int FindFreeInstance(int n) __asm__("cBankMonitor_FindFreeInstance");
    void BANKMONITOR_Pause() __asm__("cBankMonitor_BANKMONITOR_Pause");
    void BANKMONITOR_Resume() __asm__("cBankMonitor_BANKMONITOR_Resume");
    void BANKMONITOR_FadeOut(int idx, int type, float FadeTime) __asm__("cBankMonitor_BANKMONITOR_FadeOut");
    void BANKMONITOR_Destroy(int i) __asm__("cBankMonitor_BANKMONITOR_Destroy");
    void BANKMONITOR_DestroyAll() __asm__("cBankMonitor_BANKMONITOR_DestroyAll");
    void BANKMONITOR_DestroyAllWithPtr(int id) __asm__("cBankMonitor_BANKMONITOR_DestroyAllWithPtr");
    void* BANKMONITOR_GetHandle(int idx, int key) __asm__("cBankMonitor_BANKMONITOR_GetHandle");
    void BANKMONITOR_StopHandle(int idx, int type, float FadeTime) __asm__("cBankMonitor_BANKMONITOR_StopHandle");
    void BANKMONITOR_StopBank(int id, float FadeTime) __asm__("cBankMonitor_BANKMONITOR_StopBank");
    int BANKMONITOR_Active() __asm__("cBankMonitor_BANKMONITOR_Active");
    int BANKMONITOR_Used() __asm__("cBankMonitor_BANKMONITOR_Used");
    void BANKMONITOR_CameraChange() __asm__("cBankMonitor_BANKMONITOR_CameraChange");
};

CHECK_SIZE(cDelayOpts, 0x10);
CHECK_OFFSET(cMonitorInstance, mEnabled, 0x00);
CHECK_OFFSET(cMonitorInstance, mCountReset, 0x04);
CHECK_OFFSET(cMonitorInstance, mCountdown, 0x08);
CHECK_OFFSET(cMonitorInstance, mCallback, 0x0C);
CHECK_OFFSET(cMonitorInstance, mCounter, 0x14);
CHECK_OFFSET(cMonitorInstance, mPtr, 0x18);
CHECK_OFFSET(cMonitorInstance, mData1, 0x1C);
CHECK_OFFSET(cMonitorInstance, mData2, 0x20);
CHECK_OFFSET(cMonitorInstance, mData3, 0x24);
CHECK_OFFSET(cMonitorInstance, mData4, 0x28);
CHECK_SIZE(cMonitorInstance, 0x2C);

CHECK_OFFSET(cMonitor, mpVSoundSys, 0x000);
CHECK_OFFSET(cMonitor, mDefault, 0x004);
CHECK_OFFSET(cMonitor, mInst, 0x01C);
CHECK_OFFSET(cMonitor, mCurrent, 0x020);
CHECK_OFFSET(cMonitor, mPosition, 0x024);
CHECK_OFFSET(cMonitor, mPosStatic, 0x034);
CHECK_OFFSET(cMonitor, mBaseVol, 0x038);
CHECK_OFFSET(cMonitor, mVolControl, 0x03C);
CHECK_OFFSET(cMonitor, mCallback, 0x040);
CHECK_OFFSET(cMonitor, mPtr, 0x060);
CHECK_OFFSET(cMonitor, mViewport, 0x064);
CHECK_OFFSET(cMonitor, mVanish, 0x068);
CHECK_OFFSET(cMonitor, mRadius, 0x06C);
CHECK_OFFSET(cMonitor, mDelay, 0x070);
CHECK_OFFSET(cMonitor, mExternalVol, 0x074);
CHECK_OFFSET(cMonitor, mMapToMaxVol, 0x078);
CHECK_OFFSET(cMonitor, mCloseOnEnd, 0x07C);
CHECK_OFFSET(cMonitor, mTwoPlayerPan, 0x080);
CHECK_OFFSET(cMonitor, mTwoPlayerSide, 0x084);
CHECK_OFFSET(cMonitor, mCounter, 0x088);
CHECK_OFFSET(cMonitor, mLastCounter, 0x08C);
CHECK_OFFSET(cMonitor, mReEntryCheck, 0x090);
CHECK_OFFSET(cMonitor, mPaused, 0x094);
CHECK_OFFSET(cMonitor, mMonitorInstance, 0x098);
CHECK_OFFSET(cMonitor, mVtbl, 0x8D8);
CHECK_SIZE(cMonitor, 0x8DC);

CHECK_OFFSET(cBankMonitor, mpVSoundSys, 0x000);
CHECK_OFFSET(cBankMonitor, mDefault, 0x004);
CHECK_OFFSET(cBankMonitor, mInst, 0x01C);
CHECK_OFFSET(cBankMonitor, mCurrent, 0x020);
CHECK_OFFSET(cBankMonitor, mPosition, 0x024);
CHECK_OFFSET(cBankMonitor, mPosStatic, 0x034);
CHECK_OFFSET(cBankMonitor, mBaseVol, 0x038);
CHECK_OFFSET(cBankMonitor, mVolControl, 0x03C);
CHECK_OFFSET(cBankMonitor, mCallback, 0x040);
CHECK_OFFSET(cBankMonitor, mPtr, 0x060);
CHECK_OFFSET(cBankMonitor, mViewport, 0x064);
CHECK_OFFSET(cBankMonitor, mVanish, 0x068);
CHECK_OFFSET(cBankMonitor, mRadius, 0x06C);
CHECK_OFFSET(cBankMonitor, mDelay, 0x070);
CHECK_OFFSET(cBankMonitor, mExternalVol, 0x074);
CHECK_OFFSET(cBankMonitor, mMapToMaxVol, 0x078);
CHECK_OFFSET(cBankMonitor, mCloseOnEnd, 0x07C);
CHECK_OFFSET(cBankMonitor, mTwoPlayerPan, 0x080);
CHECK_OFFSET(cBankMonitor, mTwoPlayerSide, 0x084);
CHECK_OFFSET(cBankMonitor, mCounter, 0x088);
CHECK_OFFSET(cBankMonitor, mLastCounter, 0x08C);
CHECK_OFFSET(cBankMonitor, mReEntryCheck, 0x090);
CHECK_OFFSET(cBankMonitor, mPaused, 0x094);
CHECK_OFFSET(cBankMonitor, mMonitorInstance, 0x098);
CHECK_OFFSET(cBankMonitor, mVtbl, 0x8D8);
CHECK_OFFSET(cBankMonitor, mReEntry, 0x8DC);
CHECK_OFFSET(cBankMonitor, mDopplerEnable, 0x8E0);
CHECK_OFFSET(cBankMonitor, mPositionalEnable, 0x8E4);
CHECK_OFFSET(cBankMonitor, mBankMonitorPaused, 0x8E8);
CHECK_OFFSET(cBankMonitor, mBankMonitorInstance, 0x8EC);
CHECK_SIZE(cBankMonitor, 0x8F0);

// The sound monitor (On Tour: static cMonitor* gMonitor): cMonitor_cMonitor stores itself here, cMonitor__cMonitor
// clears it.
extern cMonitor* gMonitor;

#endif /* SOUND_BANKMONITOR_H */
