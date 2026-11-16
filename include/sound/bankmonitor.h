#ifndef SOUND_BANKMONITOR_H
#define SOUND_BANKMONITOR_H

// cMonitor and cBankMonitor: the sound system's monitors. EA source d:/ssx/code/source/sound/bankmonitor.cpp (the
// preview's assert strings; retail unit sound/bankmonitor, 0x2ACD08..0x2ADE88).
//   cSndPlayOpts : virtual cSoundSys          the play-option stack (ctor cSndPlayOpts_cSndPlayOpts 0x2A9350, in
//                                            sound/ps2soundman; non-virtual part 0x88)
//   cMonitor : cSndPlayOpts                  48 timed callbacks (cMonitorInstance); ctor cMonitor_cMonitor, dtor
//                                            func_002ADAC8; vtable cMonitor_vtbl (0x483BA8); non-virtual part 0x8DC
//   cBankMonitor : cMonitor                  64 monitored sounds (cBankMonitorInstance, 0xC0 each, defined with the
//                                            streamsys unit's methods 0x2AB9E0..); ctor cBankMonitor_cBankMonitor,
//                                            dtor func_002ACE40; vtable cBankMonitor_vtbl (0x483B48); non-virtual 0x8F0
// cSoundSys (0x8: pManager + its vptr) is a virtual base: g++ 2.95 puts it after the non-virtual part of the complete
// object (cMonitor + 0x8DC, cBankMonitor + 0x8F0; On Tour sizes 0x8E4 / 0x8F4) and gives the class a pointer to it at
// 0x0. The structs below are the NON-VIRTUAL parts, so they can be embedded where the object lives: the only
// cBankMonitor is the one inside cBankManager (+0x1D8), i.e. cSSXAudio + 0x6E70 (sound/ssxaudio.h). gMonitor is the
// current cMonitor (cMonitor_cMonitor stores this, func_002ADAC8 clears it; the static tick func_002AD998 calls its
// virtual slot 1, MONITOR_Update).
//
// A FLAT layout, like main/game.h: every field at its retail offset, bases written out inline, no C++ inheritance.
// Evidence tags:
//   [R] retail constructors (cSndPlayOpts_cSndPlayOpts allocates each per-level array with operator new[](n, "<member
//       name>"), cMonitor_cMonitor, cBankMonitor_cBankMonitor) and destructors;
//   [P] the July 2003 preview's inline accessors (MONITOR_*, BANKMONITOR_*), carried to retail by tools/layout_xlate.py;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cSndPlayOpts 0x94, cMonitor 0x8E4, cBankMonitor 0x8F4, cMonitorInstance 0x2C,
//       SNDPLAYOPTS 0x18, cDelayOpts 0x10): same member order; retail matches On Tour's offsets except that On Tour's
//       cMonitor members sit 4 bytes later and its bools are bytes (retail's are words).

#include "layout.h"
#include "bx/math3d.h"

struct cSoundSys;
class cController;
class cBankMonitorInstance;

// A sound callback: a g++ 2.95 pointer to a cController member function (8 bytes: delta, index, function), as
// cSndPlayOpts::OPTS_Callback takes it (preview .gnu.linkonce.t.OPTS_Callback__12cSndPlayOptsPM11cControllerFP11
// cControllerP20cBankMonitorInstancef_v). sound/bankmonitor.cpp and sound/soundsys.cpp call the same type sSndCallback.
typedef void (cController::*tSndCallback)(cController*, cBankMonitorInstance*, float);

// EA audio library play options (On Tour struct SNDPLAYOPTS, 0x18).
struct SNDPLAYOPTS {
    signed char vol;               // 0x00 [O]; [U] BANKMONITOR/MUSIC code seeds mBaseVol[mInst] from it (func_002B35A0)
    signed char bend;              // 0x01 [O]
    signed char keynum;            // 0x02 [O]
    signed char velocity;          // 0x03 [O]
    signed char drylevel;          // 0x04 [O]
    signed char fxlevel0;          // 0x05 [O]
    char pad[2];                   // 0x06 [O]
    unsigned short azimuth;        // 0x08 [O]
    short elevation;               // 0x0A [O]
    unsigned short pitchmult;      // 0x0C [O]
    unsigned short timemult;       // 0x0E [O]
    unsigned short tempomult;      // 0x10 [O]
    unsigned short pad2;           // 0x12 [O]
    unsigned short lowpasscutoff;  // 0x14 [O]
    unsigned short highpasscutoff; // 0x16 [O]
};

// One cSndPlayOpts::mDelay level (On Tour struct cDelayOpts, 0x10); [U] reset to 100 / 0 / 90 / 50 on every push
// (func_002B35A0).
struct cDelayOpts {
    int mTime;     // 0x0 [O]
    int mCount;    // 0x4 [O]
    int mFeedback; // 0x8 [O]
    int mLevel;    // 0xC [O]
};

// One of cMonitor's 48 timed callbacks (On Tour class cMonitorInstance, 0x2C). Methods 0x2AD848..0x2AD998: ctor
// func_002AD848__FPv, dtor func_002AD860, Create func_002AD888, IsFree func_002AD930, Reset func_002AD940, CountReset
// func_002AD970__FPvi, Enable func_002AD980__FPv, Disable func_002AD990__FPv (On Tour's order).
struct cMonitorInstance {
    int mEnabled;           // 0x00 [O]; [U] Enable stores 1, Disable 0
    int mCountReset;        // 0x04 [O]; [R] ctor -1; [U] CountReset stores it and mCountdown; IsFree tests == -1
    int mCountdown;         // 0x08 [O]; [U] CountReset
    tSndCallback mCallback; // 0x0C [O] (On Tour: a member-function pointer); [U] Create copies 8 bytes (sdl/sdr); func_002ADDE0 compares its index (0xE) and function (0x10)
    int mCounter;           // 0x14 [O]
    void* mPtr;             // 0x18 [O]; [U] Create stores its Ptr argument
    int mData1;             // 0x1C [O]; [U] Create
    int mData2;             // 0x20 [O]; [U] Create
    int mData3;             // 0x24 [O]; [U] Create
    int mData4;             // 0x28 [O]; [U] Create
};

struct cMonitor {
    cSoundSys* mpVSoundSys;          // 0x000 [R] g++ 2.95 virtual-base pointer to cSoundSys (= this + 0x8DC in a complete cMonitor); every ctor/dtor in the chain loads it to patch the cSoundSys vptr
    // --- cSndPlayOpts (non-virtual part 0x88; ctor cSndPlayOpts_cSndPlayOpts 0x2A9350; up to 4 nested option levels)
    SNDPLAYOPTS mDefault;            // 0x004 [O]; [U] OPTS push copies it into mCurrent[++mInst] (func_002B35A0)
    volatile int mInst;              // 0x01C [O] (volatile); [R] ctor; [U] every OPTS_ access indexes the arrays below with it
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
    int* mExternalVol;               // 0x074 [R] operator new[](0x10, "mExternalVol") (On Tour bool*); [P] OPTS_ExternalVol
    int* mMapToMaxVol;               // 0x078 [R] operator new[](0x10, "mMapToMaxVol")
    int* mCloseOnEnd;                // 0x07C [R] operator new[](0x10, "mCloseOnEnd") (On Tour bool*)
    int* mTwoPlayerPan;              // 0x080 [R] operator new[](0x10, "mTwoPlayerPan") (On Tour bool*)
    int* mTwoPlayerSide;             // 0x084 [R] operator new[](0x10, "mTwoPlayerSide")
    // --- cMonitor (On Tour: mCounter 0x8C .. mMonitorInstance 0x9C, 4 bytes later than retail)
    volatile int mCounter;           // 0x088 [P] MONITOR_Counter / MONITOR_Delta (preview 0x7C, seen); [R] ctor 0; [U] MONITOR_Update (func_002ADC10) increments it unless paused
    volatile int mLastCounter;       // 0x08C [P] MONITOR_Delta (preview 0x80, seen); [R] ctor 0
    volatile int mReEntryCheck;      // 0x090 [O]; [R] ctor 0; [U] MONITOR_Create (func_002ADCA0) ++/-- around the slot search
    int mPaused;                     // 0x094 [P] MONITOR_Paused/Pause/Resume (preview 0x88, seen); [U] func_002ADC10
    cMonitorInstance mMonitorInstance[48]; // 0x098 [O]; [R] ctor constructs 48 (func_002AD848), dtor destroys them; [U] func_002ADC30/func_002ADCA0/func_002ADD78/func_002ADDA0/func_002ADDE0
    void** mVtbl;                    // 0x8D8 [R] cMonitor_vtbl (cMonitor ctor/dtor); cBankMonitor_vtbl in a cBankMonitor (shared primary vptr)
};

struct cBankMonitor {
    cSoundSys* mpVSoundSys;          // 0x000 [R] virtual-base pointer (= this + 0x8F0 in a complete cBankMonitor; cBankManager points it at the shared cSoundSys)
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
    void** mVtbl;                    // 0x8D8 [R] cBankMonitor_vtbl (cBankMonitor ctor, func_002ACE40, and every ctor/dtor of a class that contains a cBankManager)
    // --- cBankMonitor
    volatile int mReEntry;           // 0x8DC [O] (volatile); [R] ctor 0; [U] BANKMONITOR_Create ++/-- around the instance search
    int mDopplerEnable;              // 0x8E0 [O] (On Tour bool); [R] ctor 1; [P] BANKMONITOR_DopplerEnabled/SetDoppler (preview 0x8D4, bracketed)
    int mPositionalEnable;           // 0x8E4 [O] (On Tour bool); [R] ctor 1; [P] BANKMONITOR_PositionalEnabled/SetPositional (preview 0x8D8, bracketed)
    int mBankMonitorPaused;          // 0x8E8 [O] cBankMonitor::mPaused (renamed: cMonitor's mPaused is at 0x94); [P] BANKMONITOR_Paused (preview 0x8DC, bracketed); [U] BANKMONITOR_Pause (func_002AD2A8) 1, Resume (func_002AD300) 0
    cBankMonitorInstance* mBankMonitorInstance; // 0x8EC [O]; [R] operator new[](64 x 0xC0, "mBankMonitorInstance"); [P] BANKMONITOR_GetInstance (preview 0x8E0 -> 0x8EC, seen)
};

CHECK_OFFSET(SNDPLAYOPTS, azimuth, 0x08);
CHECK_OFFSET(SNDPLAYOPTS, lowpasscutoff, 0x14);
CHECK_SIZE(SNDPLAYOPTS, 0x18);
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

// The sound monitor (On Tour: static cMonitor* gMonitor): cMonitor_cMonitor stores itself here, func_002ADAC8
// clears it.
extern cMonitor* gMonitor;

#endif /* SOUND_BANKMONITOR_H */
