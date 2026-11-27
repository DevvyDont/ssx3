#include "vtable.h"
#include "common.h"
#include "bx/math3d.h"
#include "be/beinterface.h"
#include "be/bedbprofile.h"
#include "main/gamemode.h"
#include "ealib/snd.h"
#include "sound/bankmonitor.h"
#include "sound/bankmanager.h"
#include "sound/soundsys.h"
#include "sound/streamsys.h"
#include "world/instancerecord.h"
#include "main/game.h"
#include "sound/icepick/worldtriggermanager.h"
#include "sound/ps2soundman.h"
#include "sound/crowdsys.h"
#include "sound/midisys.h"
#include "sound/fileparser.h"
#include "sound/banksys.h"
#include "sound/ssxaudio.h"
#include "be/beintoption.h"
#include "be/beintnewrace.h"
#include "scripter/ssxscriptengine.h"
#include "main/ssxapp.h"
#include "ai/rider.h"
#include "ai/ai.h"
#include "ai/airpredictor.h"
#include "ai/riderhandler.h"
#include "object/instancenode.h"
//100%
INCLUDE_ASM("sound/soundsys", cBankSys_cBankSys);
#ifdef SKIP_ASM
class cController;
class cBankMonitorInstance;
// Sound callback: a g++ 2.95 pointer to member function (8 bytes), as cSndPlayOpts::OPTS_Callback takes it.
typedef void (cController::*sSndCallback)(cController*, cBankMonitorInstance*, float);
// A sound callback key as the monitor functions take it: the 8-byte tSndCallback constant (D_004A3678 ...) read as one
// 64-bit word, unaligned (ldl/ldr), and passed in one register.
struct sSndKey8 {
    long v;
} __attribute__((packed));
#define SND_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SND_OPTS_MAX(a, b) ((a) >= (b) ? (a) : (b))

extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
#include "ai/ridermetrix.h"
struct sSndBsVtEnt {
    short delta;
    short index;
    void* fn;
};
struct sSndBsVt4 {
    sSndBsVtEnt e[4];
} __attribute__((aligned(8)));
struct sSndBsVt7 {
    sSndBsVtEnt e[7];
} __attribute__((aligned(8)));

extern char cBankSys_vtbl[];
extern char cBankMonitor_vtbl[];
extern const char str_mBank[];
extern const char str_mPath[];
extern const sSndBsVt4 D_00483A60_bs __asm__("D_00483A60");
extern const sSndBsVt7 D_00483A80_bs __asm__("D_00483A80");
// A complete cBankSys (the in-charge ctor builds it): g++ 2.95 puts the virtual bases after the non-virtual part.
struct cBankSysComplete {
    cBankSys mBankSys;           // 0x00
    cSoundSys mSoundSys;         // 0x10
    cBankManager mBankManager;   // 0x18
};

/**
 * Constructs the cBankSys part of the audio system and allocates its bank and patch tables.
 *
 * When @p embedded is set it first builds the shared virtual bases (cSoundSys, cBankManager); otherwise it
 * patches
 * stack copies of the base vtables with this object's offset. Records the instance in gBankSys and
 * allocates
 * the 16-byte "mBank" and "mPath" tables.
 *
 * @param embedded  g++ in-charge flag: nonzero when the virtual bases are built here (the complete object)
 * @param count  maximum number of banks (On Tour MaxBanks)
 * @param heap  audio mode (On Tour eSOUNDDEF_MODE audioMode), passed to the base constructors
 * @return @p self
 */
// PORT: hand-expanded g++ 2.95 virtual-base ctor (vtable upcast-offset fixup on stack copies).
extern "C" char* cBankSys_cBankSys(cBankSys* self, int embedded, int count, int heap)
{
    if (embedded) {
        cBankSysComplete* complete = (cBankSysComplete*)self;
        cSoundSys* soundSys = &complete->mSoundSys;
        self->mpVBankManager = &complete->mBankManager;
        complete->mBankManager.BankMonitor.mpVSoundSys = soundSys;
        cSoundSys_cSoundSys(soundSys, heap);
        cBankManager_cBankManager(self->mpVBankManager, 0, count, heap);
    }
    self->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&D_00483A60_bs;
    self->mpVBankManager->BankMonitor.mVtbl = (void**)cBankMonitor_vtbl;
    self->mpVBankManager->mVtbl = (void**)&D_00483A80_bs;
    if (!embedded) {
        sSndBsVt4 vtbl1 = D_00483A60_bs;
        self->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&vtbl1;
        char* base1 = (char*)self->mpVBankManager->BankMonitor.mpVSoundSys - 0x10;
        int adj1 = ((char*)self) - base1;
        vtbl1.e[1].delta = D_00483A60_bs.e[1].delta + adj1;
        sSndBsVt7 vtbl2 = D_00483A80_bs;
        self->mpVBankManager->mVtbl = (void**)&vtbl2;
        char* base2 = (char*)self->mpVBankManager - 0x18;
        int adj2 = ((char*)self) - base2;
        vtbl2.e[1].delta = D_00483A80_bs.e[1].delta + adj2;
    }
    self->mVtbl = (void**)cBankSys_vtbl;
    gBankSys = (cBankSys*)((int)self);
    self->mBank = (int*)(new (str_mBank, 0, 0) char[0x10]);
    self->mPatch = (int*)(new (str_mPath, 0, 0) char[0x10]);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cBankSys__cBankSys);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);
extern "C" void cBankManager__cBankManager(void* self, int flags);
extern "C" void cSoundSys__cSoundSys(void* self, int flags);

struct sSndDtVtEnt {
    short delta;
    short index;
    void* fn;
};
struct sSndDtVt4 {
    sSndDtVtEnt e[4];
} __attribute__((aligned(8)));
struct sSndDtVt7 {
    sSndDtVtEnt e[7];
} __attribute__((aligned(8)));

extern char cBankSys_vtbl[];
extern const sSndDtVt4 D_00483A60;
extern const sSndDtVt7 D_00483A80;
extern char cBankMonitor_vtbl[];

/**
 * Destroys the bank SFX system (g++ 2.95 destructor with virtual bases cSoundSys and cBankManager).
 *
 * Restores the class's vtables (cBankSys_vtbl, D_00483A60, cBankMonitor_vtbl, D_00483A80); when called from a derived
 * class (@p flags 0) it fixes up stack copies of the virtual-base tables for the offsets. Frees mBank and mPatch and
 * clears gBankSys. Flags bit 1 also destroys the virtual bases (cBankManager__cBankManager, cBankManager::~cBankManager, then
 * cSoundSys__cSoundSys); bit 0 frees the object.
 *
 * @param flags  destructor flags: bit 1 destroys the virtual bases, bit 0 frees the object
 */
// PORT: hand-expanded g++ 2.95 virtual-base dtor (vtable upcast-offset fixup on stack copies).
extern "C" void cBankSys__cBankSys(cBankSys* self, int flags)
{
    self->mVtbl = (void**)cBankSys_vtbl;
    self->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&D_00483A60;
    self->mpVBankManager->BankMonitor.mVtbl = (void**)cBankMonitor_vtbl;
    self->mpVBankManager->mVtbl = (void**)&D_00483A80;
    if (flags == 0) {
        sSndDtVt4 soundSysVtbl = D_00483A60;
        self->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&soundSysVtbl;
        char* soundSysBase = (char*)self->mpVBankManager->BankMonitor.mpVSoundSys - 0x10;
        int adj1 = ((char*)self) - soundSysBase;
        soundSysVtbl.e[1].delta = D_00483A60.e[1].delta + adj1;
        sSndDtVt7 bankMgrVtbl = D_00483A80;
        self->mpVBankManager->mVtbl = (void**)&bankMgrVtbl;
        char* bankMgrBase = (char*)self->mpVBankManager - 0x18;
        int adj2 = ((char*)self) - bankMgrBase;
        bankMgrVtbl.e[1].delta = D_00483A80.e[1].delta + adj2;
    }
    if (self->mBank) cMemMan_free(self->mBank);
    if (self->mPatch) cMemMan_free(self->mPatch);
    gBankSys = 0;
    if (flags & 2) {
        cBankManager__cBankManager(self->mpVBankManager, 0);
        cSoundSys__cSoundSys(self->mpVBankManager->BankMonitor.mpVSoundSys, 0);
    }
    if (flags & 1) operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cBankSys_Play2DCallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* memset(void* dst, int c, int n);
extern "C" int cBankSys_SFX_End(cBankSys* self);



#define SND_0018_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_0018_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline void snd0018AddVoice(void* vself, int a, int b)
{
    cBankSys* self = (cBankSys*)vself;
    cBankManager* m = ((cBankManager*)self->mpVBankManager);
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = a;
    self->mPatch[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = b;
}

/**
 * Monitor callback that plays a delayed 2D sound effect (On Tour Play2DCallback(cMonitorInstance* Inst)).
 *
 * Works on gBankSys rather than @p self: SFX_Begin with the instance's bank and patch (mData1 / mData2), sets the
 * volume control pointer (mVolControl) from mPtr and the base volume from mData3 (the voice volume clamped to
 * 0..127), then SFX_End plays it.
 *
 * @param req  the cMonitorInstance that fired (On Tour Inst)
 * @return 1
 */
int cBankSys::Play2DCallback(cMonitorInstance* req)
{
    snd0018AddVoice(gBankSys, req->mData1, req->mData2);
    cBankManager* bankMgr = gBankSys->mpVBankManager;
    bankMgr->BankMonitor.mVolControl[bankMgr->BankMonitor.mInst] = (float*)req->mPtr;
    cBankManager* bankMgrVol = gBankSys->mpVBankManager;
    cBankMonitor* playOpts = &bankMgrVol->BankMonitor;
    playOpts->mBaseVol[bankMgrVol->BankMonitor.mInst] = req->mData3;
    playOpts->mCurrent[bankMgrVol->BankMonitor.mInst].vol =
        SND_0018_MAX(0, SND_0018_MIN(playOpts->mBaseVol[playOpts->mInst], 0x7F));
    gBankSys->SFX_End();
    return 1;
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cBankSys_Play2DCallbackWithPitchMult);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* memset(void* dst, int c, int n);
extern "C" int cBankSys_SFX_End(cBankSys* self);



#define SND_0358_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_0358_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline void snd0358AddVoice(void* vself, int a, int b)
{
    cBankSys* self = (cBankSys*)vself;
    cBankManager* m = ((cBankManager*)self->mpVBankManager);
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = a;
    self->mPatch[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = b;
}

/**
 * Monitor callback that plays a delayed 2D sound effect with a pitch multiplier (On Tour
 * Play2DCallbackWithPitchMult(cMonitorInstance* Inst)).
 *
 * Same as cBankSys_Play2DCallback (bank / patch from mData1 / mData2, volume control from mPtr, base volume from
 * mData3), and also stores mData4 in the current play options' pitch field (+0xC) before SFX_End. Works on gBankSys
 * rather than @p self.
 *
 * @param req  the cMonitorInstance that fired (On Tour Inst)
 * @return 1
 */
int cBankSys::Play2DCallbackWithPitchMult(cMonitorInstance* req)
{
    snd0358AddVoice(gBankSys, req->mData1, req->mData2);
    cBankManager* bankMgr = gBankSys->mpVBankManager;
    bankMgr->BankMonitor.mVolControl[bankMgr->BankMonitor.mInst] = (float*)req->mPtr;
    cBankManager* bankMgrVol = gBankSys->mpVBankManager;
    cBankMonitor* playOpts = &bankMgrVol->BankMonitor;
    playOpts->mBaseVol[bankMgrVol->BankMonitor.mInst] = req->mData3;
    playOpts->mCurrent[bankMgrVol->BankMonitor.mInst].vol =
        SND_0358_MAX(0, SND_0358_MIN(playOpts->mBaseVol[playOpts->mInst], 0x7F));
    cBankManager* bankMgrPitch = gBankSys->mpVBankManager;
    bankMgrPitch->BankMonitor.mCurrent[bankMgrPitch->BankMonitor.mInst].pitchmult = req->mData4;
    gBankSys->SFX_End();
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cBankSys_SFX_End);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" int SNDBANK_play(int bank, int id, void* ch);
extern "C" void* cBankMonitor_BANKMONITOR_Create(void* sub, int h, int vol, int mode);
void cBankMonitorInstance_SetBank(void* self, int a1, int a2);
extern "C" void cMonitor_MONITOR_Create(void*, int, long, void*, int, int, int, int);

// 64-bit sound key view of D_004A3678 (same packed layout as the unit's sSndKey8, declared later).
extern sSndKey8 D_004A3678_06B8[] __asm__("D_004A3678");


#define SND_06B8_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_06B8_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SND_06B8_FMAX(a, b) ((a) >= (b) ? (a) : (b))

// On Tour: cBankInstance::GetStatus() / Handle(), cBankManager::BANK_Used(int) / BANK_Handle(int).
static inline int snd06B8Status(cBankInstance* e)
{
    return e->mBankStatus;
}
static inline int snd06B8InstHandle(cBankInstance* e)
{
    return e->mBankHandle;
}
static inline bool snd06B8Used(cBankManager* m, int id)
{
    return snd06B8Status(&m->mBank[id]) == 1;
}
static inline int snd06B8Handle(cBankManager* m, int id)
{
    if (snd06B8Used(m, id)) return snd06B8InstHandle(&m->mBank[id]);
    return -1;
}

// On Tour: cSndPlayOpts::OPTS_GetCloseOnEnd().
static inline int snd06B8CloseOnEnd(cBankMonitor* o)
{
    return o->mCloseOnEnd[o->mInst];
}

/**
 * Plays the sound effect set up since SFX_Begin and pops its play-option level (On Tour SFX_End).
 *
 * Uses the play options of the current level (mInst) in the cBankManager's cBankMonitor (mpVBankManager + 0x1D8). A
 * positioned sound (mPosition set) starts at volume 0; otherwise a volume control (mVolControl) scales mBaseVol,
 * clamped to 0..127. Plays bank mBank[mInst] / patch mPatch[mInst] (SNDBANK_play, between SNDSYS_entercritical and
 * SNDSYS_leavecritical). Unless the play failed with CloseOnEnd set, creates a bank monitor for the voice
 * (BANKMONITOR_Create, then cBankMonitorInstance_SetBank) and queues the echoes from cDelayOpts: each repeat mTime later and
 * mFeedback percent quieter (starting from mLevel percent of the base volume), until mCount runs out or the volume
 * reaches 0 (cMonitor_MONITOR_Create, MONITOR_Create). Finally decrements mInst.
 *
 * @return the voice handle, negative when the play failed
 */
int cBankSys::SFX_End()
{
    cBankSys* self = this;
    cBankManager* bankMgr = ((cBankManager*)self->mpVBankManager);
    cBankMonitor* playOpts = &bankMgr->BankMonitor;
    int BaseVol = playOpts->mBaseVol[playOpts->mInst];
    if (bankMgr->BankMonitor.mPosition[bankMgr->BankMonitor.mInst] != 0) {
        playOpts->mBaseVol[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mCurrent[bankMgr->BankMonitor.mInst].vol = SND_06B8_MAX(0,
                SND_06B8_MIN(playOpts->mBaseVol[playOpts->mInst], 0x7F));
    } else if (playOpts->mVolControl[playOpts->mInst] != 0) {
        float gain = *(float*)playOpts->mVolControl[playOpts->mInst];
        playOpts->mBaseVol[bankMgr->BankMonitor.mInst] =
                (int)SND_06B8_FMAX(SND_06B8_MIN((float)playOpts->mBaseVol[playOpts->mInst] * gain, 127.0f), 0.0f);
        playOpts->mCurrent[playOpts->mInst].vol = SND_06B8_MAX(0, SND_06B8_MIN(playOpts->mBaseVol[playOpts->mInst],
                0x7F));
    }
    int Inst = ((cBankManager*)self->mpVBankManager)->BankMonitor.mInst;
    SNDSYS_entercritical();
    int Handle = SNDBANK_play(snd06B8Handle(((cBankManager*)self->mpVBankManager), self->mBank[Inst]),
            self->mPatch[Inst],
            &((cBankManager*)self->mpVBankManager)->BankMonitor.mCurrent[
                    ((cBankManager*)self->mpVBankManager)->BankMonitor.mInst]);
    SNDSYS_leavecritical();
    if (Handle >= 0 || !snd06B8CloseOnEnd(&((cBankManager*)self->mpVBankManager)->BankMonitor)) {
        void* Mon = cBankMonitor_BANKMONITOR_Create(&((cBankManager*)self->mpVBankManager)->BankMonitor, Handle,
                BaseVol, 1);
        if (Mon != 0) {
            cBankMonitorInstance_SetBank(Mon, snd06B8Handle(((cBankManager*)self->mpVBankManager), self->mBank[Inst]),
                    self->mPatch[Inst]);
        }
        cDelayOpts* Delay = &((cBankManager*)self->mpVBankManager)->BankMonitor.mDelay[
                ((cBankManager*)self->mpVBankManager)->BankMonitor.mInst];
        if (Delay->mCount > 0) {
            int DelayTime = 0;
            int Vol = BaseVol * Delay->mLevel / 100;
            while (Delay->mCount > 0) {
                Delay->mCount--;
                DelayTime += Delay->mTime;
                Vol = Vol * Delay->mFeedback / 100;
                if (Vol > 0x7F) Vol = 0x7F;
                if (Vol < 0) Vol = 0;
                if (Vol == 0) break;
                cBankMonitor* opts = &((cBankManager*)self->mpVBankManager)->BankMonitor;
                cMonitor_MONITOR_Create(opts, DelayTime, D_004A3678_06B8->v, opts->mVolControl[opts->mInst],
                        self->mBank[Inst], self->mPatch[Inst], Vol, 0);
            }
        }
    }
    ((cBankManager*)self->mpVBankManager)->BankMonitor.mInst--;
    return Handle;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", AudioTable_evaluate);
#ifdef SKIP_ASM

// Named _impl: a later block declares AudioTable_evaluate with its own curve struct.
extern "C" float AudioTable_evaluate_impl(AudioTable* c, float x) __asm__("AudioTable_evaluate");
/**
 * Evaluates a 5-point piecewise-linear curve at a value, clamping to the end values outside its keys.
 *
 * The curve holds 5 ascending keys at +0x0 and their values at +0x14. Between two keys it interpolates linearly; below
 * the first key it returns the first value and from the last key on the last value. It returns 0 only when no case
 * applies (a NaN input or unsorted keys). The audio code keeps its volume and pitch curves in D_00482AF8..D_00482DA0
 * and gAudioTables (AudioTable_evaluateByIndex).
 *
 * @param curvePoints  the curve
 * @param x  the input value
 * @return the curve's value at @p x
 */
extern "C" float AudioTable_evaluate_impl(AudioTable* curvePoints, float x)
{
    for (int i = 1; i < 5; i++) {
        if (curvePoints->x[i - 1] <= x && x < curvePoints->x[i]) {
            float scaledRise = (x - curvePoints->x[i - 1]) * (curvePoints->y[i] - curvePoints->y[i - 1]);
            return curvePoints->y[i - 1] + scaledRise / (curvePoints->x[i] - curvePoints->x[i - 1]);
        }
    }
    if (x < curvePoints->x[0])
        return curvePoints->y[0];
    if (curvePoints->x[4] <= x)
        return curvePoints->y[4];
    return 0.0f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", AudioTable_evaluateByIndex);
#ifdef SKIP_ASM

extern "C" float AudioTable_evaluate(AudioTable* c, float v);
extern AudioTable gAudioTables[];

/**
 * Evaluates audio curve gAudioTables[i] at a value (AudioTable_evaluate).
 *
 * cSSXAudio_GetGruntVol uses curve 3.
 *
 * @param i  the gAudioTables index
 * @param inputValue  the input value
 * @return the curve's value
 */
extern "C" float AudioTable_evaluateByIndex(int i, float inputValue)
{
    return AudioTable_evaluate(&gAudioTables[i], inputValue);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetGruntVol);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float sndVu0Length(const t4Vector& v)
{
    float r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %0, $vi22\n"
        : "=r"(r)
        : "m"(v));
    return r;
}

extern "C" float AudioTable_evaluateByIndex(int i, float v);

/**
 * Returns a rider's grunt volume (0..127) from the length of its velocity, through audio curve 3.
 *
 * Gets the rider's velocity (getVelocity), takes its 4-component length (VU0) and maps
 * it through AudioTable_evaluateByIndex(3, ...), clamped to 0..127. cSSXAudio_PlayGrunt uses it as the grunt's volume.
 *
 * @param src  the rider
 * @return the volume, 0..127
 */
int cSSXAudio::GetGruntVol(cRider* src)
{
    t4Vector* velocity = (t4Vector*)src->getVelocity();
    int level = (int)AudioTable_evaluateByIndex(3, sndVu0Length(*velocity));
    if (level > 127) {
        level = 127;
    }
    if (level < 0) {
        level = 0;
    }
    return level;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SFX_Play2D);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

/**
 * Plays a 2D (non-positional) sound effect from a bank at the sound-effects volume.
 *
 * Pushes a play-option level on the bank manager (mpVBankManager) with default options (the inlined
 * cBankSys::SFX_Begin(Bank, Patch)) and records @p bank in mBank and @p patch in mPatch for that level; sets the
 * level's volume-control pointer to cSSXAudio_GetCurrAppliedVolumePtr(this, 5, 0) (On Tour GetCurrAppliedVolumePtr(5, 0)); then plays it
 * with cBankSys_SFX_End. Virtual slot 1 of cSSXAudio_vtbl, overriding cBankSys's cBankSys_SFX_Play2D.
 *
 * @param bank   the sound bank (On Tour Bank)
 * @param patch  the patch in the bank (On Tour Patch)
 * @return cBankSys_SFX_End's result (the voice handle)
 */
int cSSXAudio::SFX_Play2D(int bank, int patch)
{
    cSSXAudio* self = this;
    snd0018AddVoice(self, bank, patch);
    cStreamSys* streamSys = self->MusicSys.mpVStreamSys;
    float* volumePtr = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 5, 0);
    streamSys->mpVBankManager->BankMonitor.mVolControl[streamSys->mpVBankManager->BankMonitor.mInst] = volumePtr;
    return ((cBankSys*)self)->SFX_End();
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SetCurTrickPower);
#ifdef SKIP_ASM
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern "C" int cSSXAudio_GetNumViews(void* self);

/**
 * Records the current trick power for a rider that one of the listeners follows.
 *
 * @param rider  the rider (On Tour cRider* pRider)
 * @param value  trick power; stored for listener 0, or listener 1 in a two-view game
 */
void cSSXAudio::SetCurTrickPower(cRider* rider, float value)
{
    if (cSSXAudio_GetFocusRider_p(this, 0) == rider) {
        mCurTrickPower[0] = value;
    } else if (cSSXAudio_GetNumViews(this) == 2 && cSSXAudio_GetFocusRider_p(this, 1) == rider) {
        mCurTrickPower[1] = value;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetTrickPowerLevel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cRider_getMotionState(void*);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");

/**
 * Returns a trick-power level for a view's focus rider: 0, 1 or 2 by mCurTrickPower, or 3 when none applies.
 *
 * Takes the focus rider of view @p which (cSSXAudio_GetFocusRider) and returns 3 unless its motion state is MS_Air. The
 * rider's mAirPredictor gives a gap (TotalAirTime minus CurAirTime) when its SimState is kSS_SimDone or
 * kSS_SimCheckObjectsDone, else 4. When the gap is at most 3 and mCurTrickPower[@p which] is above 0 the result is 0
 * below 0.25, 1 below 0.5 and 2 otherwise; every other case returns 3. Virtual slot 2 of cSSXAudio_vtbl.
 *
 * @param which  the view index (also indexes mCurTrickPower)
 * @return 0, 1 or 2 by trick power, or 3
 */
extern "C" int cSSXAudio_GetTrickPowerLevel(cSSXAudio* self, int which)
{
    int result = 3;
    cRider* rider = cSSXAudio_GetFocusRider_p(self, which);
    if (cRider_getMotionState(rider) != MS_Air) {
        return 3;
    }
    float trickPower = self->mCurTrickPower[which];
    int mode = rider->mAirPredictor->SimState;
    int ok = mode == cAirPredictor::kSS_SimDone || mode == cAirPredictor::kSS_SimCheckObjectsDone;
    float delta;
    if (ok) {
        cAirPredictor* airPredictor = rider->mAirPredictor;
        delta = airPredictor->TotalAirTime - airPredictor->CurAirTime;
    } else {
        delta = 4.0f;
    }
    if (delta <= 3.0f && trickPower > 0.0f) {
        if (trickPower < 0.25f) {
            result = 0;
        } else if (trickPower < 0.5f) {
            result = 1;
        } else {
            result = 2;
        }
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_BeginWind);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline void snd10E0Init(void* vself, int a, int b)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = ((cBankManager*)self->mpVBankManager);
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = a;
    self->mPatch[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = b;
}


extern tSndCallback D_004A3680[];

/**
 * Starts the looping wind sound once (bank 0, patch 0x20) with mWindVolume as its volume control.
 *
 * Does nothing while mWindPlaying is set. Opens a play-option level with the default options (the inlined SFX_Begin)
 * for bank 0, patch 0x20, points the level's volume control at mWindVolume, clears its +0x7C option, arms the callback
 * key D_004A3680 (cSSXAudio_HandleWind, empty), plays it with cBankSys_SFX_End and sets mWindPlaying. The handle is not kept:
 * cSSXAudio_EndWind stops the sound through the same callback key. Called by cSSXAudio_UpdateAirTimeVolume.
 *
 * @param vself  the audio system (cSSXAudio)
 */
// PORT: 64-bit long sound key; voice target stored as int
void cSSXAudio::BeginWind()
{
    cSSXAudio* self = this;
    if (self->mWindPlaying != 0) {
        return;
    }
    snd10E0Init(self, 0, 0x20);
    self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mVolControl[
            self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = &self->mWindVolume;
    self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCloseOnEnd[
            self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = 0;
    cBankManager* bankManager = self->MusicSys.mpVStreamSys->mpVBankManager;
    tSndCallback k = D_004A3680[0];
    cBankMonitor* playOpts = &bankManager->BankMonitor;
    playOpts->mCallback[bankManager->BankMonitor.mInst] = k;
    ((cBankSys*)self)->SFX_End();
    self->mWindPlaying = 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_EndWind);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

extern const sSndKey8 D_004A3680_13D8[] __asm__("D_004A3680");

/**
 * Stops the wind stream and clears mWindVolume and mWindPlaying.
 */
// PORT: 64-bit long sound key
void cSSXAudio::EndWind()
{
    cBankManager* bankManager = MusicSys.mpVStreamSys->mpVBankManager;
    sVEntry<void(void*, long)>* vt = (sVEntry<void(void*, long)>*)bankManager->BankMonitor.mVtbl;
    cBankMonitor* monitor = &bankManager->BankMonitor;
    vt[3].fn(vthis(monitor, vt[3]), D_004A3680_13D8->v);
    mWindVolume = 0.0f;
    mWindPlaying = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleWind);
#ifdef SKIP_ASM
/**
 * Does nothing: the monitor callback of the wind loop (key D_004A3680, armed by cSSXAudio_BeginWind).
 *
 * cSSXAudio_EndWind cancels the loop by this key.
 */
extern "C" void cSSXAudio_HandleWind(void) {

}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayThunder);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int snd1438AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

extern "C" void SNDSYS_entercritical();
extern "C" int SNDover(int id);
extern "C" void SNDSYS_leavecritical();
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Plays a thunder clap, first stopping the previous one if it is still playing.
 *
 * On Tour's version takes a distance; this one always plays bank 8, patch 0x10.
 */
void cSSXAudio::PlayThunder()
{
    SNDSYS_entercritical();
    int soundFinished = SNDover(mThunderHandle) != 0;
    SNDSYS_leavecritical();
    if (!soundFinished) {
        cBankMonitor_BANKMONITOR_StopHandle(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                            mThunderHandle, 1, 0.0f);
    }
    mThunderHandle = snd1438AddVoice(this, 8, 0x10, 5);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_Surface);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cRider_getMotionState(void* rider);

// PORT: the unit declares this as int(void*, int); defined under an asm label with its real type
extern "C" int cSSXAudio_Surface_impl(cSSXAudio* self, cRider* rider) __asm__("cSSXAudio_Surface");
/**
 * Returns the sound surface class for the ground a rider is on.
 *
 * When cRider_getMotionState reports MS_Railslide the surface comes from mRailType, otherwise from the rider's
 * mSurfaceType (eSurfaceType). Powder, deep powder and impassable -> 1; loose snow -> 2; ice -> 3; rock, gravel and
 * wipe-out rock -> 7; wood -> 5; metal -> 4; glass -> 8; every other type (all listed, eST_Undefined and eST_Num too)
 * -> 0.
 *
 * @param rider  the rider (On Tour Rider)
 * @return the surface class (0 when the type has none)
 */
extern "C" int cSSXAudio_Surface_impl(cSSXAudio* self, cRider* rider)
{
    int x;
    if (cRider_getMotionState(rider) == MS_Railslide) {
        x = self->mRailType;
    } else {
        x = rider->mSurfaceType;
    }
    switch (x) {
    case eST_Snow_Powder: case eST_Snow_DeepPowder: case eST_Impassable:
        return 1;
    case eST_Snow_Loose:
        return 2;
    case eST_Ice_Surface:
        return 3;
    case eST_Rock_Surface: case eST_Gravel_Surface: case eST_Rock_WipeOut:
        return 7;
    case eST_Wood_Surface:
        return 5;
    case eST_Metal_Surface:
        return 4;
    case eST_Glass_Surface:
        return 8;
    case eST_Undefined: case eST_Snow_Packed: case eST_Water_Surface: case eST_Slush_Surface: case eST_Sand_Surface:
    case eST_BlackIce: case eST_Grass: case eST_Pavement: case eST_Concrete: case eST_Num:
        return 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetBoardBank);
#ifdef SKIP_ASM
/**
 * Returns the board sound bank for a surface: 1 (BOARD) above 3, else 0x10 (SPUBOARD).
 *
 * cSSXAudio_BeginChatter asks with 0; cSSXAudio_BeginGlide and cSSXAudio_BeginCarve pass the rider's surface, whose patches start at surface *
 * 8.
 *
 * @param surfaceType  the surface
 * @return the bank number
 */
int cSSXAudio::GetBoardBank(int surfaceType)
{
    if (surfaceType > 3) {
        return 1;
    }
    return 0x10;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_BeginChatter);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void cSndPlayOpts_OPTS_Radius(void* self, float a, float b);
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cSSXAudio_GetBoardBank(cSSXAudio* self, int a1);

extern tSndCallback D_004A3688_17D0[] __asm__("D_004A3688");

// PORT: 64-bit long sound key
/**
 * Starts a local human rider's chatter loop: board-bank patch 0, following the rider, with cSSXAudio_HandleChatter as callback.
 *
 * Only for a local human (mIsHuman and mIsLocal set). Opens a play-option level for bank cSSXAudio_GetBoardBank(0) and
 * patch 0 (the inlined SFX_Begin), sets volume 0 and volume control cSSXAudio_GetCurrAppliedVolumePtr(6, 0), follows
 * the rider's position (mPosition), arms callback key D_004A3688 (cSSXAudio_HandleChatter) with the rider as user data,
 * sets the listening view (cSSXAudio_GetClosestCamera1) and a 3D distance of 100 (cSndPlayOpts_OPTS_Radius), clears the
 * +0x7C option and, when newRace.cGamePath is kGAMEPATH_MULTIPLAYER and not replaying, gives the level the rider's own
 * viewport (mViewIndex). cBankSys_SFX_End plays it; the handle is not kept. Called by cSSXAudio_BeginRiderSound;
 * cSSXAudio_StopAllRiderSounds cancels the callback key.
 *
 * @param obj  the rider
 */
void cSSXAudio::BeginChatter(cRider* obj)
{
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (!ok) {
        return;
    }
    int flag = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        flag = cSSXAudio_ReplayMode(this) == 0;
    }
    int boardBank = GetBoardBank(0);
    cBankManager* bankManager = ((cBankManager*)mpVBankManager);
    cBankMonitor* playOpts = &bankManager->BankMonitor;
    bankManager->BankMonitor.mInst++;
    playOpts->mCurrent[bankManager->BankMonitor.mInst] = bankManager->BankMonitor.mDefault;
    bankManager->BankMonitor.mPosition[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPosStatic[bankManager->BankMonitor.mInst] = 0;
    playOpts->mBaseVol[bankManager->BankMonitor.mInst] = playOpts->mCurrent[bankManager->BankMonitor.mInst].vol;
    playOpts->mVolControl[bankManager->BankMonitor.mInst] = 0;
    playOpts->mCallback[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPtr[bankManager->BankMonitor.mInst] = 0;
    playOpts->mViewport[bankManager->BankMonitor.mInst] = 0;
    playOpts->mVanish[bankManager->BankMonitor.mInst] = 0;
    playOpts->mRadius[bankManager->BankMonitor.mInst] = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mCount = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mTime = 100;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mFeedback = 90;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mLevel = 50;
    playOpts->mExternalVol[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerPan[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerSide[bankManager->BankMonitor.mInst] = 0;
    playOpts->mMapToMaxVol[bankManager->BankMonitor.mInst] = 0x7F;
    playOpts->mCloseOnEnd[bankManager->BankMonitor.mInst] = 1;
    mBank[((cBankManager*)mpVBankManager)->BankMonitor.mInst] = boardBank;
    mPatch[((cBankManager*)mpVBankManager)->BankMonitor.mInst] = 0;
    cBankManager* volumeManager = MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* volumeOpts = &volumeManager->BankMonitor;
    volumeOpts->mBaseVol[volumeManager->BankMonitor.mInst] = 0;
    volumeOpts->mCurrent[volumeManager->BankMonitor.mInst].vol
            = (volumeOpts->mBaseVol[volumeOpts->mInst] < 0x7F && volumeOpts->mBaseVol[volumeOpts->mInst] < 0) ? 0
            : (volumeOpts->mBaseVol[volumeOpts->mInst] < 0x7F ? (unsigned char)volumeOpts->mBaseVol[volumeOpts->mInst]
            : 0x7F);
    cBankManager** pp = (&MusicSys.mpVStreamSys->mpVBankManager);
    float* volumeControl = (float*)cSSXAudio_GetCurrAppliedVolumePtr(this, 6, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = volumeControl;
    {
        cBankManager* optsManager = MusicSys.mpVStreamSys->mpVBankManager;
        optsManager->BankMonitor.mPosition[optsManager->BankMonitor.mInst] = &obj->mPosition;
    }
    {
        cBankManager* optsManager = MusicSys.mpVStreamSys->mpVBankManager;
        tSndCallback k = D_004A3688_17D0[0];
        cBankMonitor* callbackOpts = &optsManager->BankMonitor;
        callbackOpts->mCallback[optsManager->BankMonitor.mInst] = k;
    }
    {
        cBankManager* optsManager = MusicSys.mpVStreamSys->mpVBankManager;
        optsManager->BankMonitor.mPtr[optsManager->BankMonitor.mInst] = obj;
    }
    {
        cBankManager** pq = (&MusicSys.mpVStreamSys->mpVBankManager);
        int listenerView = cSSXAudio_GetClosestCamera1(this, obj);
        (*pq)->BankMonitor.mViewport[(*pq)->BankMonitor.mInst] = listenerView;
    }
    cSndPlayOpts_OPTS_Radius(&(MusicSys.mpVStreamSys->mpVBankManager)->BankMonitor, 100.0f, -1.0f);
    MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCloseOnEnd[
            MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = 0;
    if (flag) {
        MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mTwoPlayerPan[
                MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = 1;
        MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mTwoPlayerSide[
                MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] =
            obj->mViewIndex;
    }
    ((cBankSys*)this)->SFX_End();
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_BeginGlide);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void cSndPlayOpts_OPTS_Radius(void* self, float a, float b);
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cSSXAudio_GetBoardBank(cSSXAudio* self, int a1);

extern tSndCallback D_004A3690_1C88[] __asm__("D_004A3690");
extern tSndCallback D_004A3698_1C88[] __asm__("D_004A3698");

static inline void snd1C88AddVoice(cSSXAudio* self, int a, int b)
{
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
}

// PORT: 64-bit long sound key
/**
 * Starts a rider's glide loop on a surface: board-bank patch n * 8 + 4 for a local human, n * 8 + 6 for others.
 *
 * Both branches open a play-option level for bank cSSXAudio_GetBoardBank(@p n) with volume 0, volume control
 * cSSXAudio_GetCurrAppliedVolumePtr(6, 0), the rider's position (mPosition) to follow, the rider as user data, the
 * +0x7C option cleared and a 3D distance of 100 (cSndPlayOpts_OPTS_Radius). A local human (mIsHuman and mIsLocal set)
 * gets callback key D_004A3690 (cSSXAudio_HandleGlide), the listening view (cSSXAudio_GetClosestCamera1) and, when
 * newRace.cGamePath is kGAMEPATH_MULTIPLAYER and not replaying, its own viewport (mViewIndex); other riders get
 * D_004A3698 (cSSXAudio_HandleAIGlide) and no listening view. cBankSys_SFX_End plays it; the handle is not kept.
 *
 * Called by cSSXAudio_BeginRiderSound, and by cSSXAudio_HandleGlide / cSSXAudio_HandleAIGlide with the new surface when
 * the rider's surface changes.
 *
 * @param obj  the rider
 * @param n  the surface (its patches start at n * 8)
 */
void cSSXAudio::BeginGlide(cRider* obj, int n)
{
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        int flag = 0;
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
            flag = cSSXAudio_ReplayMode(this) == 0;
        }
        int boardBank = ((cSSXAudio*)this)->GetBoardBank(n);
        int patchNum = n * 8 + 4;
        snd1C88AddVoice(this, boardBank, patchNum);
        cBankManager* volumeManager = MusicSys.mpVStreamSys->mpVBankManager;
        cBankMonitor* volumeOpts = &volumeManager->BankMonitor;
        volumeOpts->mBaseVol[volumeManager->BankMonitor.mInst] = 0;
        volumeOpts->mCurrent[volumeManager->BankMonitor.mInst].vol
                = (volumeOpts->mBaseVol[volumeOpts->mInst] < 0x7F && volumeOpts->mBaseVol[volumeOpts->mInst] < 0) ? 0
                : (volumeOpts->mBaseVol[volumeOpts->mInst] < 0x7F
                ? (unsigned char)volumeOpts->mBaseVol[volumeOpts->mInst] : 0x7F);
        cBankManager** pp = &MusicSys.mpVStreamSys->mpVBankManager;
        float* volumeControl = (float*)cSSXAudio_GetCurrAppliedVolumePtr(this, 6, 0);
        (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = volumeControl;
        {
            cBankManager* optsManager = MusicSys.mpVStreamSys->mpVBankManager;
            optsManager->BankMonitor.mPosition[optsManager->BankMonitor.mInst] = &obj->mPosition;
        }
        {
            cBankManager* optsManager = MusicSys.mpVStreamSys->mpVBankManager;
            tSndCallback k = D_004A3690_1C88[0];
            cBankMonitor* callbackOpts = &optsManager->BankMonitor;
            callbackOpts->mCallback[optsManager->BankMonitor.mInst] = k;
        }
        {
            cBankManager* optsManager = MusicSys.mpVStreamSys->mpVBankManager;
            optsManager->BankMonitor.mPtr[optsManager->BankMonitor.mInst] = obj;
        }
        {
            cBankManager** pq = &MusicSys.mpVStreamSys->mpVBankManager;
            int listenerView = cSSXAudio_GetClosestCamera1(this, obj);
            (*pq)->BankMonitor.mViewport[(*pq)->BankMonitor.mInst] = listenerView;
        }
        MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCloseOnEnd[
                MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = 0;
        cSndPlayOpts_OPTS_Radius(&(MusicSys.mpVStreamSys->mpVBankManager)->BankMonitor, 100.0f, -1.0f);
        if (flag) {
            MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mTwoPlayerPan[
                    MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = 1;
            MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mTwoPlayerSide[
                    MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = obj->mViewIndex;
        }
        ((cBankSys*)this)->SFX_End();
    } else {
        int boardBank = ((cSSXAudio*)this)->GetBoardBank(n);
        int patchNum = n * 8 + 6;
        snd1C88AddVoice(this, boardBank, patchNum);
        cBankManager* volumeManager = MusicSys.mpVStreamSys->mpVBankManager;
        cBankMonitor* volumeOpts = &volumeManager->BankMonitor;
        volumeOpts->mBaseVol[volumeManager->BankMonitor.mInst] = 0;
        volumeOpts->mCurrent[volumeManager->BankMonitor.mInst].vol
                = (volumeOpts->mBaseVol[volumeOpts->mInst] < 0x7F && volumeOpts->mBaseVol[volumeOpts->mInst] < 0) ? 0
                : (volumeOpts->mBaseVol[volumeOpts->mInst] < 0x7F
                ? (unsigned char)volumeOpts->mBaseVol[volumeOpts->mInst] : 0x7F);
        cBankManager** pp = &MusicSys.mpVStreamSys->mpVBankManager;
        float* volumeControl = (float*)cSSXAudio_GetCurrAppliedVolumePtr(this, 6, 0);
        (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = volumeControl;
        {
            cBankManager* optsManager = MusicSys.mpVStreamSys->mpVBankManager;
            optsManager->BankMonitor.mPosition[optsManager->BankMonitor.mInst] = &obj->mPosition;
        }
        {
            cBankManager* optsManager = MusicSys.mpVStreamSys->mpVBankManager;
            tSndCallback k = D_004A3698_1C88[0];
            cBankMonitor* callbackOpts = &optsManager->BankMonitor;
            callbackOpts->mCallback[optsManager->BankMonitor.mInst] = k;
        }
        MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCloseOnEnd[
                MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = 0;
        {
            cBankManager* optsManager = MusicSys.mpVStreamSys->mpVBankManager;
            optsManager->BankMonitor.mPtr[optsManager->BankMonitor.mInst] = obj;
        }
        cSndPlayOpts_OPTS_Radius(&(MusicSys.mpVStreamSys->mpVBankManager)->BankMonitor, 100.0f, -1.0f);
        ((cBankSys*)this)->SFX_End();
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_BeginCarve);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void cSndPlayOpts_OPTS_Radius(void* self, float a, float b);
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cSSXAudio_GetBoardBank(cSSXAudio* self, int a1);

extern tSndCallback D_004A36A0_2508[] __asm__("D_004A36A0");

// PORT: 64-bit long sound key
/**
 * Starts a local human rider's carve loop on a surface: board-bank patch n * 8 + 3 with cSSXAudio_HandleCarve.
 *
 * Only for a local human (mIsHuman and mIsLocal set). Opens a play-option level for bank cSSXAudio_GetBoardBank(@p n)
 * and patch @p n * 8 + 3 with volume 0, volume control cSSXAudio_GetCurrAppliedVolumePtr(6, 0), the rider's position
 * (mPosition) to follow, callback key D_004A36A0 (cSSXAudio_HandleCarve), the +0x7C option cleared, the rider as user
 * data, the listening view (cSSXAudio_GetClosestCamera1), a 3D distance of 100 (cSndPlayOpts_OPTS_Radius) and, when
 * newRace.cGamePath is kGAMEPATH_MULTIPLAYER and not replaying, its own viewport (mViewIndex). cBankSys_SFX_End plays
 * it; the handle is not kept. Called by cSSXAudio_BeginRiderSound, and by cSSXAudio_HandleCarve on a surface change.
 *
 * @param obj  the rider
 * @param n  the surface (its patches start at n * 8)
 */
void cSSXAudio::BeginCarve(cRider* obj, int n)
{
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (!ok) {
        return;
    }
    int flag = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        flag = cSSXAudio_ReplayMode(this) == 0;
    }
    int boardBank = ((cSSXAudio*)this)->GetBoardBank(n);
    int patchNum = n * 8 + 3;
    cBankManager* bankManager = mpVBankManager;
    cBankMonitor* playOpts = &bankManager->BankMonitor;
    bankManager->BankMonitor.mInst++;
    playOpts->mCurrent[bankManager->BankMonitor.mInst] = bankManager->BankMonitor.mDefault;
    bankManager->BankMonitor.mPosition[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPosStatic[bankManager->BankMonitor.mInst] = 0;
    playOpts->mBaseVol[bankManager->BankMonitor.mInst] = playOpts->mCurrent[bankManager->BankMonitor.mInst].vol;
    playOpts->mVolControl[bankManager->BankMonitor.mInst] = 0;
    playOpts->mCallback[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPtr[bankManager->BankMonitor.mInst] = 0;
    playOpts->mViewport[bankManager->BankMonitor.mInst] = 0;
    playOpts->mVanish[bankManager->BankMonitor.mInst] = 0;
    playOpts->mRadius[bankManager->BankMonitor.mInst] = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mCount = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mTime = 100;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mFeedback = 90;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mLevel = 50;
    playOpts->mExternalVol[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerPan[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerSide[bankManager->BankMonitor.mInst] = 0;
    playOpts->mMapToMaxVol[bankManager->BankMonitor.mInst] = 0x7F;
    playOpts->mCloseOnEnd[bankManager->BankMonitor.mInst] = 1;
    mBank[mpVBankManager->BankMonitor.mInst] = boardBank;
    mPatch[mpVBankManager->BankMonitor.mInst] = patchNum;
    cBankManager* volumeManager = MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* volumeOpts = &volumeManager->BankMonitor;
    volumeOpts->mBaseVol[volumeManager->BankMonitor.mInst] = 0;
    volumeOpts->mCurrent[volumeManager->BankMonitor.mInst].vol
            = (volumeOpts->mBaseVol[volumeOpts->mInst] < 0x7F && volumeOpts->mBaseVol[volumeOpts->mInst] < 0) ? 0
            : (volumeOpts->mBaseVol[volumeOpts->mInst] < 0x7F ? (unsigned char)volumeOpts->mBaseVol[volumeOpts->mInst]
            : 0x7F);
    cBankManager** pp = &MusicSys.mpVStreamSys->mpVBankManager;
    float* volumeControl = (float*)cSSXAudio_GetCurrAppliedVolumePtr(this, 6, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = volumeControl;
    {
        cBankManager* optsManager = MusicSys.mpVStreamSys->mpVBankManager;
        optsManager->BankMonitor.mPosition[optsManager->BankMonitor.mInst] = &obj->mPosition;
    }
    {
        cBankManager* optsManager = MusicSys.mpVStreamSys->mpVBankManager;
        tSndCallback k = D_004A36A0_2508[0];
        cBankMonitor* callbackOpts = &optsManager->BankMonitor;
        callbackOpts->mCallback[optsManager->BankMonitor.mInst] = k;
    }
    MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCloseOnEnd[
            MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = 0;
    {
        cBankManager* optsManager = MusicSys.mpVStreamSys->mpVBankManager;
        optsManager->BankMonitor.mPtr[optsManager->BankMonitor.mInst] = obj;
    }
    {
        cBankManager** pq = &MusicSys.mpVStreamSys->mpVBankManager;
        int listenerView = cSSXAudio_GetClosestCamera1(this, obj);
        (*pq)->BankMonitor.mViewport[(*pq)->BankMonitor.mInst] = listenerView;
    }
    cSndPlayOpts_OPTS_Radius(&(MusicSys.mpVStreamSys->mpVBankManager)->BankMonitor, 100.0f, -1.0f);
    if (flag) {
        MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mTwoPlayerPan[
                MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = 1;
        MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mTwoPlayerSide[
                MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = obj->mViewIndex;
    }
    ((cBankSys*)this)->SFX_End();
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_BeginAllRiderSound);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getAI();
extern "C" void cSSXAudio_BeginRiderSound(cSSXAudio* self, cRider* rider);
extern "C" void cSSXAudio_StopAllRiderSounds(cSSXAudio* self);

/**
 * Starts the board sounds of every rider and resets the wind volume.
 *
 * Calls cSSXAudio_StopAllRiderSounds() first (which also stops the turbo sound), then BeginRiderSound() for each allocated
 * rider.
 */
void cSSXAudio::BeginAllRiderSound()
{
    StopAllRiderSounds();
    for (int i = 0; i < ((cAI*)cAudioInterface_getAI())->NumberOfRidersAllocated; i++) {
        BeginRiderSound(((cAI*)cAudioInterface_getAI())->Riders[i]);
    }
    mWindVolume = 0.0f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_BeginRiderSound);
#ifdef SKIP_ASM
int cRider_getMotionState(void*);
extern "C" int cSSXAudio_Surface(void* self, int id);
extern "C" void cSSXAudio_BeginChatter(void* self, cRider* rider);
extern "C" void cSSXAudio_BeginGlide(void* self, cRider* rider, int n);
extern "C" void cSSXAudio_BeginCarve(void* self, cRider* rider, int n);

/**
 * Starts a rider's board sounds: records its sound state and surface, then starts its glide and carve loops.
 *
 * @param rider  the rider
 */
void cSSXAudio::BeginRiderSound(cRider* rider)
{
    int motion = cRider_getMotionState(rider);
    int n = Surface(rider);
    rider->LastMotionType[0] = motion;
    rider->LastMotionType[1] = motion;
    rider->LastSnowType[0] = n;
    rider->LastSnowType[1] = n;
    cSSXAudio_BeginChatter(this, rider);
    cSSXAudio_BeginGlide(this, rider, n);
    cSSXAudio_BeginCarve(this, rider, n);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_StopRiderSound);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern const sSndKey8 D_004A36A8[];
extern "C" void cMonitor_MONITOR_DestroyAll1(void* bank, long id);


/**
 * Stops a rider's sounds through the bank manager (virtual slot 4), then calls cMonitor_MONITOR_DestroyAll1() with stream
 * key
 * D_004A36A8.
 *
 * @param Rider  the rider (On Tour cRider* Rider)
 */
// PORT: the 8-byte stream id is passed by value from an unaligned global (64-bit `long`).
void cSSXAudio::StopRiderSound(int Rider)
{
    cBankManager* bankManager = MusicSys.mpVStreamSys->mpVBankManager;
    sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)bankManager->BankMonitor.mVtbl;
    cBankMonitor* monitor = &bankManager->BankMonitor;
    vt[4].fn(vthis(monitor, vt[4]), Rider);
    cMonitor_MONITOR_DestroyAll1(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, D_004A36A8[0].v);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_StopAllRiderSounds);
#ifdef SKIP_ASM
class cSndSub292B48 {
public:
    char pad[0x8D8];
    virtual void v01();
    virtual void v02();
    virtual void v03(long key);
};
class cSndBase292B48 {
public:
    char pad[0x1D4];
    virtual void b01();
};
class cSndObj292B48 : public cSndBase292B48, public cSndSub292B48 {
};
extern const sSndKey8 D_004A3688[];
extern const sSndKey8 D_004A3690[];
extern const sSndKey8 D_004A3698[];
extern const sSndKey8 D_004A36A0[];
// View (cSSXAudio_MaxPowerEvent declares D_004A36A8 with the unit's sSndKey8 type).
extern const sSndKey8 D_004A36A8_b48[] __asm__("D_004A36A8");
extern const sSndKey8 D_004A36B0[];
extern const sSndKey8 D_004A36B8[];
extern "C" void cSSXAudio_EndWind(cSSXAudio* self);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);
extern "C" void cMonitor_MONITOR_DestroyAll1(void* p, long key);

static inline void sndKeyCall(cSSXAudio* self, long key)
{
    ((cSndObj292B48*)self->MusicSys.mpVStreamSys->mpVBankManager)->v03(key);
}

/**
 * Stops every rider sound loop: the wind, the chatter, glide and carve loops, the turbo sound and the wipeout loops.
 *
 * Calls cSSXAudio_EndWind, then cancels the monitors armed with the callback keys D_004A3688 (cSSXAudio_HandleChatter),
 * D_004A3690 (cSSXAudio_HandleGlide), D_004A3698 (cSSXAudio_HandleAIGlide) and D_004A36A0 (cSSXAudio_HandleCarve)
 * through virtual slot 3 of the bank manager (MusicSys.mpVStreamSys->mpVBankManager), and D_004A36A8
 * (cSSXAudio_TurboUseCheck) with cMonitor_MONITOR_DestroyAll1. When mTurboRef is positive it stops mTurboHandle
 * (cBankMonitor_BANKMONITOR_StopHandle, type 1, no fade) and clears mTurboRef. Last it cancels D_004A36B0
 * (cSSXAudio_HandleWipeoutSlide) and D_004A36B8 (cSSXAudio_HandleWipeoutTumble).
 */
// PORT: 64-bit long sound keys
void cSSXAudio::StopAllRiderSounds()
{
    EndWind();
    sndKeyCall(this, D_004A3688[0].v);
    sndKeyCall(this, D_004A3690[0].v);
    sndKeyCall(this, D_004A3698[0].v);
    sndKeyCall(this, D_004A36A0[0].v);
    cMonitor_MONITOR_DestroyAll1(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, D_004A36A8_b48[0].v);
    if (mTurboRef > 0) {
        cBankMonitor_BANKMONITOR_StopHandle(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                            mTurboHandle, 1, 0.0f);
        mTurboRef = 0;
    }
    sndKeyCall(this, D_004A36B0[0].v);
    sndKeyCall(this, D_004A36B8[0].v);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleChatter);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_ChatterProgram(cSSXAudio*, cRider*, int*, int*);
extern "C" void cBankMonitorInstance_PitchUpdate(void*, int);
extern "C" cGame* cAudioInterface_getGame();
extern "C" int cReplay_isPauseAI(void* p);
extern int cPauseState_Flags;

/**
 * Updates a chatter loop for one frame: in motion state MS_Ground its volume and bend come from
 * cSSXAudio_ChatterProgram.
 *
 * The instance's rider is its mPtr (its user data). Starts from volume 0 and bend 0x40; in MS_Ground
 * cSSXAudio_ChatterProgram fills both. MS_Wipeout, a finished rider (mFinishTime >= 0), cPauseState_Flags bit 0 or a
 * paused replay (cReplay_isPauseAI) force volume 0. Stores the volume in the instance's mBaseVol and passes the bend to
 * cBankMonitorInstance_PitchUpdate. Monitor callback of key D_004A3688, armed by cSSXAudio_BeginChatter.
 *
 * @param obj  the bank-monitor instance
 */
void cSSXAudio::HandleChatter(cBankMonitorInstance* obj)
{
    int volume;
    int pitchBend;
    cRider* rider = (cRider*)obj->mPtr;
    pitchBend = 0x40;
    volume = 0;
    if (cRider_getMotionState(rider) == MS_Ground) {
        ChatterProgram(rider, &volume, &pitchBend);
    }
    if (cRider_getMotionState(rider) == MS_Wipeout) {
        volume = 0;
    }
    int pos = rider->mFinishTime >= 0.0f;
    if (pos) {
        volume = 0;
    }
    int paused = 0;
    if ((cPauseState_Flags & 1) || cReplay_isPauseAI(cAudioInterface_getGame()->Replay) != 0) {
        paused = 1;
    }
    if (paused) {
        volume = 0;
    }
    obj->mBaseVol = volume;
    cBankMonitorInstance_PitchUpdate(obj, pitchBend);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleGlide);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_GetSurfaceProgramType(cSSXAudio* self, int id, cRider* rider);
extern "C" void cSSXAudio_GlideProgram(cSSXAudio* self, int id, cRider* rider, int* out, int* size);
extern "C" void cBankMonitorInstance_Reset(void*);
extern "C" void cBankMonitorInstance_PitchUpdate(void*, int);
extern "C" cGame* cAudioInterface_getGame();
extern "C" int cReplay_isPauseAI(void* p);
extern int cPauseState_Flags;

/**
 * Updates a human rider's glide loop for one frame: restarts it on a surface change, else sets its volume and
 * bend.
 *
 * Called for a bank-monitor instance whose rider is its mPtr. In motion states MS_Air and MS_Handplant only a surface
 * whose program type is keSurfaceProgram_Rail is switched to 9; MS_Wipeout, cPauseState_Flags bit 0 and a playing
 * replay (cReplay_isPauseAI) silence the loop. The volume goes to the instance's mBaseVol and the bend through
 * cBankMonitorInstance_PitchUpdate().
 *
 * @param obj  the bank-monitor instance (On Tour cBankMonitorInstance* Inst)
 */
// PORT: the rider pointer is passed to cSSXAudio_Surface/cSSXAudio_BeginGlide as an int (the unit declares them that
// way).
void cSSXAudio::HandleGlide(cBankMonitorInstance* obj)
{
    int volume;
    int bend;
    cRider* rider = (cRider*)obj->mPtr;
    int st = cRider_getMotionState(rider);
    if (st != rider->LastMotionType[0]) {
        rider->LastMotionType[0] = st;
    }
    volume = 0;
    bend = 0x40;
    if (st == MS_Air || st == MS_Handplant) {
        if (GetSurfaceProgramType(rider->LastSnowType[0], rider) == keSurfaceProgram_Rail) {
            rider->LastSnowType[0] = 9;
        }
    } else {
        int id = Surface(rider);
        if (id != rider->LastSnowType[0]) {
            rider->LastSnowType[0] = id;
            cBankMonitorInstance_Reset(obj);
            cSSXAudio_BeginGlide(this, rider, id);
        } else {
            GlideProgram(Surface(rider), rider, &volume, &bend);
        }
    }
    if (cRider_getMotionState(rider) == MS_Wipeout) {
        volume = 0;
    }
    int paused = 0;
    if ((cPauseState_Flags & 1) || cReplay_isPauseAI(cAudioInterface_getGame()->Replay) != 0) {
        paused = 1;
    }
    if (paused) {
        volume = 0;
    }
    obj->mBaseVol = volume;
    cBankMonitorInstance_PitchUpdate(obj, bend);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleAIGlide);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_AIGlideProgram(cSSXAudio* self, int id, cRider* rider, int* out, int* size);
extern "C" void cBankMonitorInstance_Reset(void*);
extern "C" void cBankMonitorInstance_PitchUpdate(void*, int);
extern "C" cGame* cAudioInterface_getGame();
extern "C" int cReplay_isPauseAI(void* p);
extern int cPauseState_Flags;

/**
 * Updates a computer rider's glide loop for one frame, like HandleGlide() but only in motion states MS_Ground and
 * MS_Railslide.
 *
 * @param obj  the bank-monitor instance (On Tour cBankMonitorInstance* Inst)
 */
// PORT: the rider pointer is passed to cSSXAudio_Surface/cSSXAudio_BeginGlide as an int (the unit declares them that
// way).
void cSSXAudio::HandleAIGlide(cBankMonitorInstance* obj)
{
    int volume;
    int bend;
    cRider* rider = (cRider*)obj->mPtr;
    int st = cRider_getMotionState(rider);
    if (st != rider->LastMotionType[0]) {
        rider->LastMotionType[0] = st;
    }
    volume = 0;
    bend = 0x40;
    if (st == MS_Ground || st == MS_Railslide) {
        int id = Surface(rider);
        if (id != rider->LastSnowType[0]) {
            rider->LastSnowType[0] = id;
            cBankMonitorInstance_Reset(obj);
            cSSXAudio_BeginGlide(this, rider, Surface(rider));
        }
        AIGlideProgram(Surface(rider), rider, &volume, &bend);
    }
    int paused = 0;
    if ((cPauseState_Flags & 1) || cReplay_isPauseAI(cAudioInterface_getGame()->Replay) != 0) {
        paused = 1;
    }
    if (paused) {
        volume = 0;
    }
    obj->mBaseVol = volume;
    cBankMonitorInstance_PitchUpdate(obj, bend);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleCarve);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_GetSurfaceProgramType(cSSXAudio* self, int id, cRider* rider);
extern "C" void cSSXAudio_CarveProgram(cSSXAudio* self, int id, cRider* rider, int* out, int* size);
extern "C" void cBankMonitorInstance_Reset(void*);
extern "C" void cBankMonitorInstance_PitchUpdate(void*, int);
extern "C" cGame* cAudioInterface_getGame();
extern "C" int cReplay_isPauseAI(void* p);
extern int cPauseState_Flags;

/**
 * Updates a rider's carve loop for one frame: restarts it on a surface change, else sets its volume and bend.
 *
 * Same structure as HandleGlide(), with the carve surface kept in the rider's +0x76C and CarveProgram().
 *
 * @param obj  the bank-monitor instance (On Tour cBankMonitorInstance* Inst)
 */
// PORT: the rider pointer is passed to cSSXAudio_Surface/cSSXAudio_BeginCarve as an int (the unit declares them that
// way).
void cSSXAudio::HandleCarve(cBankMonitorInstance* obj)
{
    int volume;
    int bend;
    cRider* rider = (cRider*)obj->mPtr;
    bend = 0x40;
    volume = 0;
    int st = cRider_getMotionState(rider);
    if (st == MS_Air || st == MS_Handplant) {
        if (GetSurfaceProgramType(rider->LastSnowType[1], rider) == keSurfaceProgram_Rail) {
            rider->LastSnowType[1] = 9;
        }
    } else {
        int id = Surface(rider);
        if (id != rider->LastSnowType[1]) {
            rider->LastSnowType[1] = id;
            cBankMonitorInstance_Reset(obj);
            cSSXAudio_BeginCarve(this, rider, Surface(rider));
        } else {
            CarveProgram(Surface(rider), rider, &volume, &bend);
        }
    }
    if (cRider_getMotionState(rider) == MS_Wipeout) {
        volume = 0;
    }
    int paused = 0;
    if ((cPauseState_Flags & 1) || cReplay_isPauseAI(cAudioInterface_getGame()->Replay) != 0) {
        paused = 1;
    }
    if (paused) {
        volume = 0;
    }
    obj->mBaseVol = volume;
    cBankMonitorInstance_PitchUpdate(obj, bend);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_ChatterProgram);
#ifdef SKIP_ASM
extern AudioTable D_00482AF8;
extern AudioTable D_00482B20;

/**
 * Computes a chatter loop's volume and pitch bend from the rider's audio speed through two curves.
 *
 * The volume is curve D_00482AF8 at mAudioSpeed (AudioTable_evaluate) and the bend is curve D_00482B20 at the same speed
 * times 4096 (0x1000 is no bend). Called by cSSXAudio_HandleChatter.
 *
 * @param rider  the rider
 * @param out1  receives the volume
 * @param out2  receives the pitch bend
 */
void cSSXAudio::ChatterProgram(cRider* rider, int* out1, int* out2)
{
    float spd = rider->mAudioSpeed;
    AudioTable volumeCurve = D_00482AF8;
    AudioTable bendCurve = D_00482B20;
    *out1 = (int)AudioTable_evaluate(&volumeCurve, spd);
    *out2 = (int)(AudioTable_evaluate(&bendCurve, spd) * 4096.0f);
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GlideProgram);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cSSXAudio_GetSurfaceProgramType(cSSXAudio* self, int id, cRider* rider);
void snd_dec_a(void*, void*, int*, int*) __asm__("cSSXAudio_HardGlideProgram");
void snd_dec_b(void*, void*, int*, int*) __asm__("cSSXAudio_SoftGlideProgram");
void snd_dec_c(void*, void*, int*, int*) __asm__("cSSXAudio_RailGlideProgram");

/**
 * Computes a human rider's glide-loop volume and pitch bend for a surface.
 *
 * Starts from volume 0 and bend 0x1000, then runs the program for the surface's type (cSSXAudio_GetSurfaceProgramType:
 * keSurfaceProgram_Hard, keSurfaceProgram_Soft or keSurfaceProgram_Rail).
 *
 * @param id  surface (On Tour Surface)
 * @param rider  the rider
 * @param out  receives the volume
 * @param size  receives the pitch bend (0x1000 is no bend)
 */
void cSSXAudio::GlideProgram(int id, cRider* rider, int* out, int* size)
{
    *out = 0;
    *size = 0x1000;
    int programType = GetSurfaceProgramType(id, rider);
    if (programType == keSurfaceProgram_Hard) {
        snd_dec_a(this, rider, out, size);
    } else if (programType == keSurfaceProgram_Soft) {
        snd_dec_b(this, rider, out, size);
    } else if (programType == keSurfaceProgram_Rail) {
        snd_dec_c(this, rider, out, size);
    }
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_AIGlideProgram);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cSSXAudio_GetSurfaceProgramType(cSSXAudio* self, int id, cRider* rider);
void snd_dec_d(void*, void*, int*, int*) __asm__("cSSXAudio_AIHardGlideProgram");
void snd_dec_e(void*, void*, int*, int*) __asm__("cSSXAudio_AISoftGlideProgram");
void snd_dec_f(void*, void*, int*, int*) __asm__("cSSXAudio_AIRailGlideProgram");

/**
 * Computes a computer rider's glide-loop volume and pitch bend for a surface, like GlideProgram().
 *
 * @param id  surface (On Tour Surface)
 * @param rider  the rider
 * @param out  receives the volume
 * @param size  receives the pitch bend (0x1000 is no bend)
 */
void cSSXAudio::AIGlideProgram(int id, cRider* rider, int* out, int* size)
{
    *out = 0;
    *size = 0x1000;
    int programType = GetSurfaceProgramType(id, rider);
    if (programType == keSurfaceProgram_Hard) {
        snd_dec_d(this, rider, out, size);
    } else if (programType == keSurfaceProgram_Soft) {
        snd_dec_e(this, rider, out, size);
    } else if (programType == keSurfaceProgram_Rail) {
        snd_dec_f(this, rider, out, size);
    }
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_CarveProgram);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cSSXAudio_GetSurfaceProgramType(cSSXAudio* self, int id, cRider* rider);
void snd_dec_g(void*, void*, int*, int*) __asm__("cSSXAudio_HardCarveProgram");
void snd_dec_h(void*, void*, int*, int*) __asm__("cSSXAudio_SoftCarveProgram");
void snd_dec_i(void*, void*, int*, int*) __asm__("cSSXAudio_RailCarveProgram");

/**
 * Computes a rider's carve-loop volume and pitch bend for a surface, like GlideProgram().
 *
 * @param id  surface (On Tour Surface)
 * @param rider  the rider
 * @param out  receives the volume
 * @param size  receives the pitch bend (0x1000 is no bend)
 */
void cSSXAudio::CarveProgram(int id, cRider* rider, int* out, int* size)
{
    *out = 0;
    *size = 0x1000;
    int programType = GetSurfaceProgramType(id, rider);
    if (programType == keSurfaceProgram_Hard) {
        snd_dec_g(this, rider, out, size);
    } else if (programType == keSurfaceProgram_Soft) {
        snd_dec_h(this, rider, out, size);
    } else if (programType == keSurfaceProgram_Rail) {
        snd_dec_i(this, rider, out, size);
    }
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetSurfaceProgramType);
#ifdef SKIP_ASM
/**
 * Classifies a surface for the glide and carve sound programs: 0, 1 or 2.
 *
 * Surfaces 1 and 2 give 1 and surfaces 4 to 8 give 2; surface 3 gives 2 only while the rider's motion state
 * (cRider_getMotionState) is 4, else 0; 0 and any other value give 0. cSSXAudio_GlideProgram, cSSXAudio_AIGlideProgram
 * and cSSXAudio_CarveProgram pick one of three programs by the result, and cSSXAudio_HandleGlide switches the rider's
 * surface to 9 when it is 2 in motion states 1 and 5.
 *
 * @param kind  the surface
 * @param rider  the rider
 * @return the program type: 0, 1 or 2
 */
int cSSXAudio::GetSurfaceProgramType(int kind, cRider* rider)
{
    int result;

    switch (kind) {
    case 3:
        result = ((cRider_getMotionState(rider) ^ 4) != 0) ? 0 : 2;
        break;
    case 1:
    case 2:
        result = 1;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        result = 2;
        break;
    case 0:
    default:
        result = 0;
        break;
    }
    return result;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_HardGlideProgram);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern const AudioTable D_00482B78;
extern const AudioTable D_00482BA0;

/**
 * Computes a rider's glide volume and bend for surface program 0 from its pressure, edging, braking and speed.
 *
 * The level is mAudioPressure * (1 - (mAudioEdging + mAudioBraking)), scaled by mAudioSpeed * 10 below speed 0.1. When
 * it differs from the rider's last glide level (mLastGlide at the rider's AI index) by more than 0.1 it moves only a
 * quarter of the way there. The level is stored back; the volume is curve D_00482B78 at the level and the bend curve
 * D_00482BA0 at the level times 4096. cSSXAudio_GlideProgram runs it for program type 0 and cSSXAudio_AIHardGlideProgram for computer
 * riders.
 *
 * @param rider  the rider
 * @param volume  receives the volume
 * @param pitch  receives the pitch bend
 */
void cSSXAudio::HardGlideProgram(cRider* rider, int* volume, int* pitch)
{
    float level = rider->mAudioPressure;
    level *= 1.0f - (rider->mAudioEdging + rider->mAudioBraking);
    float speed = rider->mAudioSpeed;
    if (speed < 0.1f) {
        level = level * speed * 10.0f;
    }
    int idx = rider->getAIIndex();
    float prev = mLastGlide[idx];
    float levelDiff = prev - level;
    if (__builtin_fabsf(levelDiff) > 0.1f) {
        if (level < prev) {
            level = prev - levelDiff * 0.25f;
        } else if (prev < level) {
            level = prev + (level - prev) * 0.25f;
        }
    }
    AudioTable volumeCurve = D_00482B78;
    mLastGlide[idx] = level;
    AudioTable bendCurve = D_00482BA0;
    *volume = (int)AudioTable_evaluate(&volumeCurve, level);
    *pitch = (int)(AudioTable_evaluate(&bendCurve, level) * 4096.0f);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SoftGlideProgram);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern const AudioTable D_00482BC8;
extern const AudioTable D_00482BF0;

/**
 * Computes a rider's glide volume and bend for surface program 1 from its pressure, edging, braking and speed.
 *
 * The level is mAudioPressure * (1 - (mAudioEdging + mAudioBraking)), scaled by mAudioSpeed * 10 below speed 0.1. When
 * it differs from the rider's last glide level (mLastGlide at the rider's AI index) by more than 0.1 it moves only a
 * quarter of the way there. The level is stored back; the volume is curve D_00482BC8 at the level and the bend curve
 * D_00482BF0 at the level times 4096. cSSXAudio_GlideProgram runs it for program type 1 and cSSXAudio_AISoftGlideProgram for computer
 * riders.
 *
 * @param rider  the rider
 * @param volume  receives the volume
 * @param pitch  receives the pitch bend
 */
void cSSXAudio::SoftGlideProgram(cRider* rider, int* volume, int* pitch)
{
    float level = rider->mAudioPressure;
    level *= 1.0f - (rider->mAudioEdging + rider->mAudioBraking);
    float speed = rider->mAudioSpeed;
    if (speed < 0.1f) {
        level = level * speed * 10.0f;
    }
    int idx = rider->getAIIndex();
    float prev = mLastGlide[idx];
    float levelDiff = prev - level;
    if (__builtin_fabsf(levelDiff) > 0.1f) {
        if (level < prev) {
            level = prev - levelDiff * 0.25f;
        } else if (prev < level) {
            level = prev + (level - prev) * 0.25f;
        }
    }
    AudioTable volumeCurve = D_00482BC8;
    mLastGlide[idx] = level;
    AudioTable bendCurve = D_00482BF0;
    *volume = (int)AudioTable_evaluate(&volumeCurve, level);
    *pitch = (int)(AudioTable_evaluate(&bendCurve, level) * 4096.0f);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_RailGlideProgram);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern const AudioTable D_00482C18;
extern const AudioTable D_00482C40;

/**
 * Computes a rider's glide volume and bend for surface program 2 from its pressure, edging, braking and speed.
 *
 * The level is mAudioPressure * (1 - (mAudioEdging + mAudioBraking)), scaled by mAudioSpeed * 10 below speed 0.1. When
 * it differs from the rider's last glide level (mLastGlide at the rider's AI index) by more than 0.1 it moves only a
 * quarter of the way there. The level is stored back; the volume is curve D_00482C18 at the level and the bend curve
 * D_00482C40 at the level times 4096. cSSXAudio_GlideProgram runs it for program type 2 and cSSXAudio_AIRailGlideProgram for computer
 * riders.
 *
 * @param rider  the rider
 * @param volume  receives the volume
 * @param pitch  receives the pitch bend
 */
void cSSXAudio::RailGlideProgram(cRider* rider, int* volume, int* pitch)
{
    float level = rider->mAudioPressure;
    level *= 1.0f - (rider->mAudioEdging + rider->mAudioBraking);
    float speed = rider->mAudioSpeed;
    if (speed < 0.1f) {
        level = level * speed * 10.0f;
    }
    int idx = rider->getAIIndex();
    float prev = mLastGlide[idx];
    float levelDiff = prev - level;
    if (__builtin_fabsf(levelDiff) > 0.1f) {
        if (level < prev) {
            level = prev - levelDiff * 0.25f;
        } else if (prev < level) {
            level = prev + (level - prev) * 0.25f;
        }
    }
    AudioTable volumeCurve = D_00482C18;
    mLastGlide[idx] = level;
    AudioTable bendCurve = D_00482C40;
    *volume = (int)AudioTable_evaluate(&volumeCurve, level);
    *pitch = (int)(AudioTable_evaluate(&bendCurve, level) * 4096.0f);
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_AIHardGlideProgram);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_HardGlideProgram();

/**
 * Runs the type-0 glide program cSSXAudio_HardGlideProgram for a computer rider, passing its arguments through.
 *
 * Declared without parameters, it calls cSSXAudio_HardGlideProgram with the argument registers it was given;
 * cSSXAudio_AIGlideProgram calls it as (this, rider, volume, bend) for program type 0.
 */
extern "C" void cSSXAudio_AIHardGlideProgram(void) {
    cSSXAudio_HardGlideProgram();
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_AISoftGlideProgram);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_SoftGlideProgram();

/**
 * Runs the type-1 glide program cSSXAudio_SoftGlideProgram for a computer rider, passing its arguments through.
 *
 * Declared without parameters, it calls cSSXAudio_SoftGlideProgram with the argument registers it was given;
 * cSSXAudio_AIGlideProgram calls it as (this, rider, volume, bend) for program type 1.
 */
extern "C" void cSSXAudio_AISoftGlideProgram(void) {
    cSSXAudio_SoftGlideProgram();
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_AIRailGlideProgram);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_RailGlideProgram();

/**
 * Runs the type-2 glide program cSSXAudio_RailGlideProgram for a computer rider, passing its arguments through.
 *
 * Declared without parameters, it calls cSSXAudio_RailGlideProgram with the argument registers it was given;
 * cSSXAudio_AIGlideProgram calls it as (this, rider, volume, bend) for program type 2.
 */
extern "C" void cSSXAudio_AIRailGlideProgram(void) {
    cSSXAudio_RailGlideProgram();
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_HardCarveProgram);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
static inline float* sndLevels5990(cSSXAudio* self) { return self->mLastCarveEdging; }
extern const AudioTable D_00482C68;
extern const AudioTable D_00482C90;

/**
 * Computes a rider's carve volume and bend for surface program 0 from its edging, braking, pressure and speed.
 *
 * The level is mAudioEdging + mAudioBraking. When it differs from the rider's last carve level (mLastCarveEdging at the
 * rider's AI index) by more than 0.1 it falls by half the gap or rises by an eighth of it. The level is stored back;
 * the volume is curve D_00482C68 at the level and the bend curve D_00482C90 at mAudioPressure times 4096. Below
 * mAudioSpeed 0.1 the volume is scaled by speed * 10. cSSXAudio_CarveProgram runs it for program type 0.
 *
 * @param rider  the rider
 * @param volume  receives the volume
 * @param pitch  receives the pitch bend
 */
void cSSXAudio::HardCarveProgram(cRider* rider, int* volume, int* pitch)
{
    float pressure = rider->mAudioPressure;
    float level = rider->mAudioEdging + rider->mAudioBraking;
    int idx = rider->getAIIndex();
    float prev = sndLevels5990(this)[idx];
    float levelDiff = prev - level;
    if (__builtin_fabsf(levelDiff) > 0.1f) {
        if (level < prev) {
            level = prev - levelDiff * 0.5f;
        } else if (prev < level) {
            level = prev + (level - prev) * 0.125f;
        }
    }
    AudioTable volumeCurve = D_00482C68;
    sndLevels5990(this)[idx] = level;
    AudioTable bendCurve = D_00482C90;
    *volume = (int)AudioTable_evaluate(&volumeCurve, level);
    *pitch = (int)(AudioTable_evaluate(&bendCurve, pressure) * 4096.0f);
    float speed = rider->mAudioSpeed;
    if (speed < 0.1f) {
        *volume = (int)((float)*volume * speed * 10.0f);
    }
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SoftCarveProgram);
#ifdef SKIP_ASM
extern const AudioTable D_00482CB8;
extern const AudioTable D_00482CE0;

/**
 * Computes the carve volume and bend on a soft surface from the rider's edging, braking and pressure.
 *
 * The level (edging + braking, scaled down below speed 0.1) is smoothed per AI index once it moves by more
 * than
 * 0.1: halfway on a drop, an eighth on a rise. Volume and bend come from two curves (D_00482CB8, D_00482CE0).
 *
 * @param rider  the rider
 * @param volume  receives the volume
 * @param pitch  receives the pitch bend (curve value times 4096)
 */
void cSSXAudio::SoftCarveProgram(cRider* rider, int* volume, int* pitch)
{
    float level = rider->mAudioEdging + rider->mAudioBraking;
    float speed = rider->mAudioSpeed;
    if (speed < 0.1f) {
        level = level * speed * 10.0f;
    }
    float pressure = rider->mAudioPressure;
    int idx = rider->getAIIndex();
    float prev = mLastCarveEdging[idx];
    float diff = prev - level;
    if (__builtin_fabsf(diff) > 0.1f) {
        if (level < prev) {
            level = prev - diff * 0.5f;
        } else if (prev < level) {
            level = prev + (level - prev) * 0.125f;
        }
    }
    AudioTable volCurve = D_00482CB8;
    mLastCarveEdging[idx] = level;
    AudioTable bendCurve = D_00482CE0;
    *volume = (int)AudioTable_evaluate(&volCurve, level);
    *pitch = (int)(AudioTable_evaluate(&bendCurve, pressure) * 4096.0f);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_RailCarveProgram);
#ifdef SKIP_ASM
extern const AudioTable D_00482D08;

/**
 * Computes a rider's carve volume and bend for surface program 2: full volume, with the bend from its pressure.
 *
 * Smooths and stores the edging + braking level in mLastCarveEdging as cSSXAudio_HardCarveProgram does, but does not use it: the
 * volume is 127 and the bend is curve D_00482D08 at mAudioPressure times 4096. Below mAudioSpeed 0.1 the volume is
 * scaled by speed * 10. cSSXAudio_CarveProgram runs it for program type 2.
 *
 * @param rider  the rider
 * @param volume  receives the volume
 * @param pitch  receives the pitch bend
 */
void cSSXAudio::RailCarveProgram(cRider* rider, int* volume, int* pitch)
{
    float pressure = rider->mAudioPressure;
    float level = rider->mAudioEdging + rider->mAudioBraking;
    int idx = rider->getAIIndex();
    float prev = mLastCarveEdging[idx];
    float levelDiff = prev - level;
    if (__builtin_fabsf(levelDiff) > 0.1f) {
        if (level < prev) {
            level = prev - levelDiff * 0.5f;
        } else if (prev < level) {
            level = prev + (level - prev) * 0.125f;
        }
    }
    mLastCarveEdging[idx] = level;
    *volume = 0x7F;
    AudioTable bendCurve = D_00482D08;
    *pitch = (int)(AudioTable_evaluate(&bendCurve, pressure) * 4096.0f);
    float speed = rider->mAudioSpeed;
    if (speed < 0.1f) {
        *volume = (int)((float)*volume * speed * 10.0f);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_Lift);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
#include "fakematch.h"
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cSSXAudio_GetFocusRider(void* self, int which);
extern "C" int cSSXAudio_Surface(void* self, int id);
void cSSXAudio_LiftWithAirTime_4170(void* self, char* obj) __asm__("cSSXAudio_LiftWithAirTime");
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void SNDSYS_entercritical();
extern "C" int SNDover(int id);
extern "C" void SNDSYS_leavecritical();
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

struct sSnd4170Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd4170E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd4170Sub {
    char pad0[4];
    sSnd4170Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd4170Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd4170E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd4170Mgr {
    char pad0[0x1D8];
    sSnd4170Sub sub;         // 0x1D8
};
struct sSnd4170Sys {
    sSnd4170Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd4170Mgr** p118;      // 0x118
};

static inline int snd4170Add(void* vself, void* obj, int a, int b, int c, int flag)
{
    sSnd4170Sys* self = (sSnd4170Sys*)vself;
    sSnd4170Mgr* m = self->mgr;
    sSnd4170Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    memset((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
    s->p60[m->sub.cur] = 0;
    s->p64[m->sub.cur] = 0;
    s->p68[m->sub.cur] = 0;
    s->p6C[m->sub.cur] = 0;
    s->p70[m->sub.cur].f4 = 0;
    s->p70[m->sub.cur].f0 = 100;
    s->p70[m->sub.cur].f8 = 90;
    s->p70[m->sub.cur].fC = 50;
    s->p74[m->sub.cur] = 0;
    s->p80[m->sub.cur] = 0;
    s->p84[m->sub.cur] = 0;
    s->p78[m->sub.cur] = 0x7F;
    s->p7C[m->sub.cur] = 1;
    self->a4[self->mgr->sub.cur] = a;
    self->a8[self->mgr->sub.cur] = b;
    sSnd4170Mgr* m3 = *self->p118;
    sSnd4170Sub* s3 = &m3->sub;
    s3->vals[m3->sub.cur] = c;
    s3->ch[m3->sub.cur].vol = (s3->vals[s3->cur] < 0x7F && s3->vals[s3->cur] < 0) ? 0 : (s3->vals[s3->cur] < 0x7F ? (unsigned char)s3->vals[s3->cur] : 0x7F);
    sSnd4170Mgr** pp = self->p118;
    int r = (int)cSSXAudio_GetCurrAppliedVolumePtr(self, 6, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    {
        sSnd4170Mgr* m2 = *self->p118;
        m2->sub.tgt[m2->sub.cur] = (char*)obj + 0x110;
    }
    {
        sSnd4170Mgr** pq = self->p118;
        int q = cSSXAudio_GetClosestCamera1(self, obj);
        (*pq)->sub.p64[(*pq)->sub.cur] = q;
    }
    if (flag) {
        (*self->p118)->sub.p80[(*self->p118)->sub.cur] = 1;
        (*self->p118)->sub.p84[(*self->p118)->sub.cur] = *(int*)((char*)obj + 0x870);
    }
    return cBankSys_SFX_End((cBankSys*)self);
}

static inline int clampVol_4170(int v, int lo, int hi)
{
    bool belowHi = v < hi;
    if (belowHi && v < lo) {
        return lo;
    }
    if (belowHi) {
        return v;
    }
    FM_BARRIER(); // FAKEMATCH: see cSSXAudio_Lift
    return hi;
}

/**
 * Plays the lift-off sound when the listener's rider leaves the ground, then runs LiftWithAirTime().
 *
 * The volume follows mAudioSpeed (64-127); the previous lift voice is stopped first, and the patch depends on
 * the
 * surface (bank 10, surface * 8 + 2). In an online game a local human's voice is tied to its view.
 *
 * @param obj  the rider
 */
// FAKEMATCH: per-function volatile view (`volatile int cur` in sSnd4170Sub) plus FM_BARRIER() before clampVol_4170's
// `return hi` (stops jump1 turning the second belowHi test into a movn; retail keeps the branch); plain form 90.49%
// (99.07% with the volatile view alone).
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" void cSSXAudio_Lift(cSSXAudio* self, cRider* obj)
{
    int idx = -1;
    if (cSSXAudio_ReplayMode(self) != 0) {
        idx = 0;
    } else {
        bool ok = obj->mIsHuman && obj->mIsLocal;
        if (ok) {
            idx = obj->mViewIndex;
        }
    }
    if (cSSXAudio_GetFocusRider(self, idx) == (int)obj) {
        int volume = clampVol_4170((int)(obj->mAudioSpeed * 127.0f), 0x40, 0x7F);
        SNDSYS_entercritical();
        int soundFinished = SNDover(self->mCharLiftHandle) != 0;
        SNDSYS_leavecritical();
        if (!soundFinished) {
            cBankMonitor_BANKMONITOR_StopHandle(*(char**)self->MusicSys.mpVStreamSys + 0x1D8, self->mCharLiftHandle, 1, 0.0f);
        }
        int flag = 0;
        bool ok2 = obj->mIsHuman && obj->mIsLocal;
        if (ok2) {
            cBE_getInterface_Fv(cBE_getBE(), 0);
            if (cBENewRaceInterface_newRace.cGamePath == 2) {
                flag = cSSXAudio_ReplayMode(self) == 0;
            }
        }
        int surface = cSSXAudio_Surface(self, (int)obj);
        int patch = surface * 8 + 2;
        self->mCharLiftHandle = snd4170Add(self, obj, 10, patch, volume, flag);
    }
    cSSXAudio_LiftWithAirTime_4170(self, (char*)obj);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_LiftWithAirTime);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cSSXAudio_SetNeedLift(cSSXAudio* self, int i, int v);
extern "C" void cSSXAudio_SpeakBigAir(cSSXAudio* self, cRider* rider);
extern "C" void cCrowdSys_RequestAnticipation(cCrowdSys* self, int i);
extern "C" int cSSXAudio_ReplayMode(void* self);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");


/**
 * Handles the air-time side of a lift-off, from the rider's air predictor.
 *
 * Unless the predictor is in state kSS_SimDone or kSS_SimCheckObjectsDone, it only calls cSSXAudio_SetNeedLift(.., 1).
 * Otherwise it calls cSSXAudio_SetNeedLift(.., 0) and cSSXAudio_SpeakBigAir(); for the listener's rider with more than 2 s of
 * predicted air time it also calls cCrowdSys_RequestAnticipation(), probably a crowd reaction.
 *
 * @param rider  the rider
 */
void cSSXAudio::LiftWithAirTime(cRider* rider)
{
    int simState = rider->mAirPredictor->SimState;
    int ok = simState == cAirPredictor::kSS_SimDone || simState == cAirPredictor::kSS_SimCheckObjectsDone;
    if (!ok) {
        SetNeedLift(rider->getAIIndex(), 1);
        return;
    }
    int idx = -1;
    {
        SetNeedLift(rider->getAIIndex(), 0);
    }
    float x = rider->mAirPredictor->TotalAirTime;
    cSSXAudio_SpeakBigAir(this, (cRider*)(((char*)rider)));
    if (cSSXAudio_ReplayMode(this) != 0) {
        idx = 0;
    } else {
        int ok2 = rider->mIsHuman != 0 && rider->mIsLocal != 0;
        if (ok2) {
            idx = rider->mViewIndex;
        }
    }
    if (cSSXAudio_GetFocusRider_p(this, idx) == rider && x > 2.0f) {
        cCrowdSys_RequestAnticipation((cCrowdSys*)this, idx);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UpdateLift);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cSSXAudio_SetNeedLift(cSSXAudio* self, int i, int v);
extern "C" void cSSXAudio_SpeakBigAir(cSSXAudio* self, cRider* rider);
extern "C" void cCrowdSys_RequestAnticipation(cCrowdSys* self, int i);
extern "C" int cSSXAudio_ReplayMode(void* self);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");




extern "C" void cSSXAudio_LiftWithAirTime(cSSXAudio* self, cRider* rider);

/**
 * Runs LiftWithAirTime() for each rider flagged in mNeedLift, clearing the flags of riders that do not exist.
 */
void cSSXAudio::UpdateLift()
{
    cSSXAudio* audio = this;
    for (int i = 0; i < 6; i++) {
        if (audio->mNeedLift[i] != 0 && i < ((cAI*)cAudioInterface_getAI())->NumberOfRidersAllocated) {
            char* rider = (char*)((cAI*)cAudioInterface_getAI())->Riders[i];
            if (rider != 0) {
                LiftWithAirTime((cRider*)rider);
                continue;
            }
        }
        audio->mNeedLift[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetNeedLift);
#ifdef SKIP_ASM
/**
 * Returns mNeedLift[i] for a rider index below 6, else 0.
 *
 * Nothing calls it directly; cSSXAudio_SetNeedLift sets the flags and cSSXAudio_ResetLift clears them.
 *
 * @param i  the rider index
 * @return the flag, or 0 for an index of 6 or more
 */
int cSSXAudio::GetNeedLift(int i)
{
    if (i < 6) {
        return mNeedLift[i];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SetNeedLift);
#ifdef SKIP_ASM
/**
 * Sets mNeedLift[i] for a rider index below 6; larger indexes are ignored.
 *
 * cSSXAudio_LiftWithAirTime calls it with the rider's AI index and 1 or 0.
 *
 * @param i  the rider index
 * @param needLift  the new flag
 */
void cSSXAudio::SetNeedLift(int i, int needLift)
{
    if (i < 6) {
        mNeedLift[i] = needLift;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_ResetLift);
#ifdef SKIP_ASM
/**
 * Clears the six mNeedLift flags.
 */
void cSSXAudio::ResetLift()
{
    int i;
    for (i = 5; i >= 0; i--) {
        mNeedLift[i] = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_Land);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
extern "C" void cCrowdSys_CancelAnticipation(cCrowdSys* self, int i);
extern "C" void cCrowdSys_PlayTrickCheer(cCrowdSys* self, int level, int i);
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" int SNDover(int id);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);
// cSSXAudio_Surface takes the rider here (the unit declares (void*, int)).
int cSSXAudio_Surface_p(void* self, cRider* rider) __asm__("cSSXAudio_Surface");
extern const AudioTable D_00482D30;

/**
 * Plays a rider's landing sound and the crowd's reaction to it.
 *
 * When the rider is the focus rider of its view (cSSXAudio_GetFocusRider with view 0 when replaying, mViewIndex for a
 * local human, else -1) the crowd reacts through cCrowdSys_CancelAnticipation (nothing gained) or cCrowdSys_PlayTrickCheer at level 0, 1 or 2
 * (gain below 0.25, below 0.65, above). Computer and non-local riders farther than 100 units from the camera
 * (cSSXAudio_CameraInRange) play nothing more. The volume comes from the landing curve D_00482D30 (AudioTable_evaluate)
 * applied to the rider's +0x770, clamped to 0..127.
 *
 * A previous landing still playing (handle at +0x5FB8) is stopped. Then a play-option level is pushed with defaults
 * (the inlined SFX_Begin) for bank 10 and patch cSSXAudio_Surface's surface class * 8 + 1 for a local human, + 5
 * otherwise; the level gets the volume, the volume-control pointer cSSXAudio_GetCurrAppliedVolumePtr(this, 6, 0), a 3D
 * distance of 30 (cSndPlayOpts_OPTS_Radius), the rider's mPosition to follow, the listening view from
 * cSSXAudio_GetClosestCamera1, and, for a local human when newRace.cGamePath is kGAMEPATH_MULTIPLAYER and not
 * replaying, its own viewport. cBankSys_SFX_End starts it and the new handle is stored at +0x5FB8.
 *
 * @param rider          the rider that landed (On Tour Rider)
 * @param powerupGained  power gained by the landing (On Tour PowerupGained); drives the crowd reaction
 */
void cSSXAudio::Land(cRider* rider, float powerupGained)
{
    cSSXAudio* self = this;
    int idx = -1;
    if (cSSXAudio_ReplayMode(self) != 0) {
        idx = 0;
    } else {
        int ok = rider->mIsHuman != 0 && rider->mIsLocal != 0;
        if (ok) {
            idx = rider->mViewIndex;
        }
    }
    if (cSSXAudio_GetFocusRider_p(self, idx) == rider) {
        if (powerupGained <= 0.0f) {
            cCrowdSys_CancelAnticipation((cCrowdSys*)self, idx);
        } else if (powerupGained < 0.25f) {
            cCrowdSys_PlayTrickCheer((cCrowdSys*)self, 0, idx);
        } else if (powerupGained < 0.65f) {
            cCrowdSys_PlayTrickCheer((cCrowdSys*)self, 1, idx);
        } else {
            cCrowdSys_PlayTrickCheer((cCrowdSys*)self, 2, idx);
        }
    }
    if (rider->isComputer() != 0 || rider->mIsLocal == 0) {
        if (cSSXAudio_CameraInRange(self, rider->getPosition(), 100.0f) < 0) {
            return;
        }
    }
    AudioTable landCurve = D_00482D30;
    int vol = (int)AudioTable_evaluate(&landCurve, rider->LastLandSpeed);
    if (vol > 127) {
        vol = 127;
    }
    if (vol < 0) {
        vol = 0;
    }
    int patchNum;
    int local = rider->mIsHuman != 0 && rider->mIsLocal != 0;
    if (local) {
        patchNum = cSSXAudio_Surface_p(self, rider) * 8 + 1;
    } else {
        patchNum = cSSXAudio_Surface_p(self, rider) * 8 + 5;
    }
    SNDSYS_entercritical();
    int soundFinished = SNDover(self->mCharLandHandle) != 0;
    SNDSYS_leavecritical();
    if (!soundFinished) {
        cBankMonitor_BANKMONITOR_StopHandle(&(self->MusicSys.mpVStreamSys->mpVBankManager)->BankMonitor,
                self->mCharLandHandle, 1, 0.0f);
    }
    int flag = 0;
    int ok3 = rider->mIsHuman != 0 && rider->mIsLocal != 0;
    if (ok3) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
            flag = cSSXAudio_ReplayMode(self) == 0;
        }
    }
    int bank = 10;
    cBankManager* bankMgr = ((cBankManager*)self->mpVBankManager);
    cBankMonitor* monitor = &bankMgr->BankMonitor;
    bankMgr->BankMonitor.mInst++;
    monitor->mCurrent[bankMgr->BankMonitor.mInst] = bankMgr->BankMonitor.mDefault;
    bankMgr->BankMonitor.mPosition[bankMgr->BankMonitor.mInst] = 0;
    monitor->mPosStatic[bankMgr->BankMonitor.mInst] = 0;
    monitor->mBaseVol[bankMgr->BankMonitor.mInst] = monitor->mCurrent[bankMgr->BankMonitor.mInst].vol;
    monitor->mVolControl[bankMgr->BankMonitor.mInst] = 0;
    monitor->mCallback[bankMgr->BankMonitor.mInst] = 0;
    monitor->mPtr[bankMgr->BankMonitor.mInst] = 0;
    monitor->mViewport[bankMgr->BankMonitor.mInst] = 0;
    monitor->mVanish[bankMgr->BankMonitor.mInst] = 0;
    monitor->mRadius[bankMgr->BankMonitor.mInst] = 0;
    monitor->mDelay[bankMgr->BankMonitor.mInst].mCount = 0;
    monitor->mDelay[bankMgr->BankMonitor.mInst].mTime = 100;
    monitor->mDelay[bankMgr->BankMonitor.mInst].mFeedback = 90;
    monitor->mDelay[bankMgr->BankMonitor.mInst].mLevel = 50;
    monitor->mExternalVol[bankMgr->BankMonitor.mInst] = 0;
    monitor->mTwoPlayerPan[bankMgr->BankMonitor.mInst] = 0;
    monitor->mTwoPlayerSide[bankMgr->BankMonitor.mInst] = 0;
    monitor->mMapToMaxVol[bankMgr->BankMonitor.mInst] = 0x7F;
    monitor->mCloseOnEnd[bankMgr->BankMonitor.mInst] = 1;
    self->mBank[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = bank;
    self->mPatch[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = patchNum;
    cBankManager* bankMgrVol = self->MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* volMonitor = &bankMgrVol->BankMonitor;
    volMonitor->mBaseVol[bankMgrVol->BankMonitor.mInst] = vol;
    volMonitor->mCurrent[bankMgrVol->BankMonitor.mInst].vol
            = (volMonitor->mBaseVol[volMonitor->mInst] < 0x7F && volMonitor->mBaseVol[volMonitor->mInst] < 0) ? 0
            : (volMonitor->mBaseVol[volMonitor->mInst] < 0x7F ? (unsigned char)volMonitor->mBaseVol[volMonitor->mInst]
            : 0x7F);
    cBankManager** pp = (&self->MusicSys.mpVStreamSys->mpVBankManager);
    float* volumePtr = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 6, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = volumePtr;
    cSndPlayOpts_OPTS_Radius(&(self->MusicSys.mpVStreamSys->mpVBankManager)->BankMonitor, 30.0f, -1.0f);
    {
        cBankManager* bankMgrPos = self->MusicSys.mpVStreamSys->mpVBankManager;
        bankMgrPos->BankMonitor.mPosition[bankMgrPos->BankMonitor.mInst] = &rider->mPosition;
    }
    {
        cBankManager** pq = (&self->MusicSys.mpVStreamSys->mpVBankManager);
        int viewport = cSSXAudio_GetClosestCamera1(self, rider);
        (*pq)->BankMonitor.mViewport[(*pq)->BankMonitor.mInst] = viewport;
    }
    if (flag) {
        self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mTwoPlayerPan[
                self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = 1;
        self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mTwoPlayerSide[
                self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = rider->mViewIndex;
    }
    self->mCharLandHandle = ((cBankSys*)self)->SFX_End();
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayPauseSound);
#ifdef SKIP_ASM

/**
 * Plays bank 0, patch 0 as a 2D sound through virtual slot 1 of cSSXAudio_vtbl (cSSXAudio_SFX_Play2D).
 *
 * Called with the audio system when the pause menu opens, right after cSSXAudio_PauseAudio.
 *
 * @return the voice handle from the virtual call
 */
int cSSXAudio::PlayPauseSound()
{
    sVEntry<int(void*, int, int)>* vt = (sVEntry<int(void*, int, int)>*)mVtbl;
    return vt[1].fn(vthis(this, vt[1]), 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_MenuSound);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

/**
 * Plays a front-end menu sound.
 *
 * The cue (eMenuSoundType) maps to a patch: FEChallenge -> 0x10, FEMail -> 0x11, FEForward / IGForward -> 3, FEBack /
 * IGBack -> 4, FELeftRight / IGLeftRight -> 2, FEUpDown / IGUpDown -> 1, FEError / IGError -> 0xD, IGOverlayPopup ->
 * 0x12, SnowSwoosh -> 7; FEScrollText, FESelectText and unknown cues play nothing (kNoMenuSoundID). The patch is
 * started through virtual slot 1 of the object at this+0xC with (0, patch).
 *
 * @param cue  the menu sound type (On Tour eMenuSoundType type)
 */
void cSSXAudio::MenuSound(int cue)
{
    int id = kNoMenuSoundID;
    switch (cue) {
    case keMenuSoundType_FEScrollText: case keMenuSoundType_FESelectText:
        id = kNoMenuSoundID;
        break;
    case keMenuSoundType_FEChallenge:
        id = 0x10;
        break;
    case keMenuSoundType_FEMail:
        id = 17;
        break;
    case keMenuSoundType_FEForward: case keMenuSoundType_IGForward:
        id = 3;
        break;
    case keMenuSoundType_FEBack: case keMenuSoundType_IGBack:
        id = 4;
        break;
    case keMenuSoundType_FELeftRight: case keMenuSoundType_IGLeftRight:
        id = 2;
        break;
    case keMenuSoundType_FEUpDown: case keMenuSoundType_IGUpDown:
        id = 1;
        break;
    case keMenuSoundType_FEError: case keMenuSoundType_IGError:
        id = 13;
        break;
    case keMenuSoundType_IGOverlayPopup:
        id = 18;
        break;
    case keMenuSoundType_SnowSwoosh:
        id = 7;
        break;
    }
    if (id != kNoMenuSoundID) {
        sVEntry<int(void*, int, int)>* vt = (sVEntry<int(void*, int, int)>*)mVtbl;
        vt[1].fn(vthis(this, vt[1]), 0, id);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayHistoryExists);
#ifdef SKIP_ASM
extern "C" cBankMonitorInstance* cBankMonitor_BANKMONITOR_GetHandle(void* self, int idx, int a2);

static inline int ring5028Count(cSSXAudio* s)
{
    return (s->mPlayHistory.Head - s->mPlayHistory.Tail + s->mPlayHistory.ArraySize) % s->mPlayHistory.ArraySize;
}
static inline int ring5028Empty(cSSXAudio* s)
{
    return s->mPlayHistory.Head == s->mPlayHistory.Tail;
}
static inline int ring5028Get(cSSXAudio* s, int i)
{
    if (ring5028Empty(s) || i >= ring5028Count(s)) return -1;
    cCircBuffer* r = &s->mPlayHistory;
    return r->Array[(s->mPlayHistory.Tail + i) % s->mPlayHistory.ArraySize];
}

/**
 * Tells whether the play history holds a sound for an event that is still playing.
 *
 * The history (mPlayHistory) is a FIFO of 5-int records: event, voice handle, time, looping, hull. A record
 * with
 * handle 0 counts as playing.
 *
 * @param Ptr  event key: an instance pointer or a small event ID
 * @param Hull  collision hull (0 for most events)
 * @return true when a matching record is found, otherwise false
 */
bool cSSXAudio::PlayHistoryExists(int Ptr, int Hull)
{
    cSSXAudio* self = this;
    int n = ring5028Count(self);
    for (int i = 0; i < n; i += 5) {
        int x = ring5028Get(self, i);
        int y = ring5028Get(self, i + 4);
        if (Ptr == x && Hull == y) {
            int handle = ring5028Get(self, i + 1);
            if (handle != 0 &&
                cBankMonitor_BANKMONITOR_GetHandle(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                   handle, 1) == 0) continue;
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayHistoryUpdate);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// (helpers renamed for this function)
static inline int sndRingCount_5208(cSSXAudio* s)
{
    return (s->mPlayHistory.Head - s->mPlayHistory.Tail + s->mPlayHistory.ArraySize) % s->mPlayHistory.ArraySize;
}
static inline int sndRingEmpty_5208(cSSXAudio* s)
{
    return s->mPlayHistory.Head == s->mPlayHistory.Tail;
}
static inline int sndRingFull_5208(cSSXAudio* s)
{
    return (s->mPlayHistory.Head + 1) % s->mPlayHistory.ArraySize == s->mPlayHistory.Tail;
}
static inline int sndRingPop_5208(cSSXAudio* s)
{
    cCircBuffer* r = &s->mPlayHistory;
    if (sndRingEmpty_5208(s)) return -1;
    int v = r->Array[r->Tail];
    if (++s->mPlayHistory.Tail >= s->mPlayHistory.ArraySize) s->mPlayHistory.Tail = 0;
    return v;
}
static inline void sndRingPush_5208(cSSXAudio* s, int v)
{
    cCircBuffer* r = &s->mPlayHistory;
    if (sndRingFull_5208(s)) return;
    r->Array[r->Head] = v;
    if (++s->mPlayHistory.Head >= s->mPlayHistory.ArraySize) s->mPlayHistory.Head = 0;
}

/**
 * Ages the play history: drops records whose sound has ended or whose time has passed, stopping looping ones.
 *
 * Walks mPlayHistory once, popping each 5-word record (event key, voice handle, time, looping flag, hull). A record
 * with a negative handle, or with a nonzero handle the bank monitor no longer has (cBankMonitor_BANKMONITOR_GetHandle,
 * type 1), is dropped. A record whose time is still ahead of the bank clock (the bank monitor's mCounter) or is
 * kPlayHistoryNoTimeout goes back into the queue; any other is dropped, its voice stopped first
 * (cBankMonitor_BANKMONITOR_StopHandle, type 1, no fade) when the looping flag is set. Called by cSSXAudio_Update.
 *
 */
void cSSXAudio::PlayHistoryUpdate()
{
    cSSXAudio* self = this;
    int n = sndRingCount_5208(self) / 5;
    for (int i = 0; i < n; i++) {
        int eventId = sndRingPop_5208(self);
        int handle = sndRingPop_5208(self);
        int fireTime = sndRingPop_5208(self);
        int stop = sndRingPop_5208(self);
        int idx = sndRingPop_5208(self);
        if (handle < 0) continue;
        if (handle != 0 && cBankMonitor_BANKMONITOR_GetHandle(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                              handle, 1) == 0) continue;
        cBankManager* mgr = self->MusicSys.mpVStreamSys->mpVBankManager;
        cBankMonitor* bank = &mgr->BankMonitor;
        if (mgr->BankMonitor.mCounter < fireTime) {
            sndRingPush_5208(self, eventId);
            sndRingPush_5208(self, handle);
            sndRingPush_5208(self, fireTime);
            sndRingPush_5208(self, stop);
            sndRingPush_5208(self, idx);
        } else if (fireTime == kPlayHistoryNoTimeout) {
            sndRingPush_5208(self, eventId);
            sndRingPush_5208(self, handle);
            sndRingPush_5208(self, fireTime);
            sndRingPush_5208(self, stop);
            sndRingPush_5208(self, idx);
        } else if (stop != 0) {
            cBankMonitor_BANKMONITOR_StopHandle(bank, handle, 1, 0.0f);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayHistoryAdd);
#ifdef SKIP_ASM
static inline int ring5628Full(cSSXAudio* s)
{
    return (s->mPlayHistory.Head + 1) % s->mPlayHistory.ArraySize == s->mPlayHistory.Tail;
}
static inline int ring5628Pop(cSSXAudio* s)
{
    cCircBuffer* r = &s->mPlayHistory;
    if (ring5028Empty(s)) return -1;
    int v = r->Array[r->Tail];
    if (++s->mPlayHistory.Tail >= s->mPlayHistory.ArraySize) s->mPlayHistory.Tail = 0;
    return v;
}
static inline void ring5628Push(cSSXAudio* s, int v)
{
    cCircBuffer* r = &s->mPlayHistory;
    if (ring5628Full(s)) return;
    r->Array[r->Head] = v;
    if (++s->mPlayHistory.Head >= s->mPlayHistory.ArraySize) s->mPlayHistory.Head = 0;
}

/**
 * Adds a record to the play history, dropping (and stopping) the oldest record when the history is full.
 *
 * @param vself  the audio system
 * @param evt  event key: an instance pointer or a small event ID
 * @param Handle  the voice handle
 * @param Time  time stamp stored with the record (callers pass the bank clock plus a hold time)
 * @param Looping  looping flag
 * @param idx  collision hull (On Tour Hull)
 */
// PORT: the event pointer is stored in an int ring slot.
void cSSXAudio::PlayHistoryAdd(void* evt, int Handle, int Time, int Looping, int idx)
{
    // ORIGINAL-PTR: On Tour's DWARF declares cSSXAudio::PlayHistoryAdd(int Ptr, int Handle, int Time, int Looping,
    // int Hull) and its play history is a cCircBuffer of ints (int* Array): EA stored the pointer as an int.

    cSSXAudio* self = this;
    if (ring5628Full(self)) {
        ring5628Pop(self);
        cBankMonitor_BANKMONITOR_StopHandle(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                            ring5628Pop(self), 1, 0.0f);
        ring5628Pop(self);
        ring5628Pop(self);
        ring5628Pop(self);
    }
    ring5628Push(self, (int)evt);
    ring5628Push(self, Handle);
    ring5628Push(self, Time);
    ring5628Push(self, Looping);
    ring5628Push(self, idx);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayHistoryUpdateTime);
#ifdef SKIP_ASM
static inline int ring5950Full(cSSXAudio* s)
{
    return (s->mPlayHistory.Head + 1) % s->mPlayHistory.ArraySize == s->mPlayHistory.Tail;
}
static inline int ring5950Pop(cSSXAudio* s)
{
    cCircBuffer* r = &s->mPlayHistory;
    if (ring5028Empty(s)) return -1;
    int v = r->Array[r->Tail];
    if (++s->mPlayHistory.Tail >= s->mPlayHistory.ArraySize) s->mPlayHistory.Tail = 0;
    return v;
}
static inline void ring5950Push(cSSXAudio* s, int v)
{
    cCircBuffer* r = &s->mPlayHistory;
    if (ring5950Full(s)) return;
    r->Array[r->Head] = v;
    if (++s->mPlayHistory.Head >= s->mPlayHistory.ArraySize) s->mPlayHistory.Head = 0;
}

/**
 * Changes the time stamp of the play-history records for an event and hull.
 *
 * The loop runs once per int rather than once per record, so it cycles the whole FIFO five times; the result
 * is
 * the same.
 *
 * @param evt  event key to look for (On Tour SearchPtr)
 * @param NewTime  new time stamp
 * @param idx  collision hull to look for (On Tour SearchHull)
 */
void cSSXAudio::PlayHistoryUpdateTime(int evt, int NewTime, int idx)
{
    cSSXAudio* self = this;
    int n = ring5028Count(self);
    for (int i = 0; i < n; i++) {
        int histPtr = ring5950Pop(self);
        int handle = ring5950Pop(self);
        int time = ring5950Pop(self);
        int looping = ring5950Pop(self);
        int hull = ring5950Pop(self);
        if (evt == histPtr && idx == hull) time = NewTime;
        ring5950Push(self, histPtr);
        ring5950Push(self, handle);
        ring5950Push(self, time);
        ring5950Push(self, looping);
        ring5950Push(self, hull);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayHistoryTimeout);
#ifdef SKIP_ASM
static inline int ring5CC8Full(cSSXAudio* s)
{
    return (s->mPlayHistory.Head + 1) % s->mPlayHistory.ArraySize == s->mPlayHistory.Tail;
}
static inline int ring5CC8Pop(cSSXAudio* s)
{
    cCircBuffer* r = &s->mPlayHistory;
    if (ring5028Empty(s)) return -1;
    int v = r->Array[r->Tail];
    if (++s->mPlayHistory.Tail >= s->mPlayHistory.ArraySize) s->mPlayHistory.Tail = 0;
    return v;
}
static inline void ring5CC8Push(cSSXAudio* s, int v)
{
    cCircBuffer* r = &s->mPlayHistory;
    if (ring5CC8Full(s)) return;
    r->Array[r->Head] = v;
    if (++s->mPlayHistory.Head >= s->mPlayHistory.ArraySize) s->mPlayHistory.Head = 0;
}

/**
 * Removes every play-history record for an event key, stopping its voice first when the record is looping.
 *
 * Pops each 5-word record of mPlayHistory: one whose event key is @p evt is dropped, after
 * cBankMonitor_BANKMONITOR_StopHandle (type 1, no fade) on its handle when its looping flag is set; the others go back
 * into the queue. Likely bug: the loop count is the number of words in the queue, not of records (cSSXAudio_PlayHistoryUpdate divides
 * by 5), so it cycles the queue five times; the outcome is the same. Nothing calls it directly.
 *
 * @param evt  the event key (an instance pointer or event id)
 */
void cSSXAudio::PlayHistoryTimeout(int evt)
{
    cSSXAudio* self = this;
    int n = ring5028Count(self);
    for (int i = 0; i < n; i++) {
        int eventId = ring5CC8Pop(self);
        int handle = ring5CC8Pop(self);
        int fireTime = ring5CC8Pop(self);
        int stop = ring5CC8Pop(self);
        int idx = ring5CC8Pop(self);
        if (evt == eventId) {
            if (stop != 0) {
                cBankMonitor_BANKMONITOR_StopHandle(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                    handle, 1, 0.0f);
            }
        } else {
            ring5CC8Push(self, eventId);
            ring5CC8Push(self, handle);
            ring5CC8Push(self, fireTime);
            ring5CC8Push(self, stop);
            ring5CC8Push(self, idx);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_CollisionSound);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
// cSSXAudio_PlayHistoryExists takes the event pointer here (the unit declares (void*, int, int) later).
bool cSSXAudio_PlayHistoryExists_p(void* self, void* evt, int idx) __asm__("cSSXAudio_PlayHistoryExists");
extern "C" int cWorldTriggerManager_HandleCollision(void* mgr, void* evt, int id, void* rider, int vol);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);


/**
 * Plays the collision sound of a world instance a rider has hit, scaled by the rider's speed.
 *
 * Only for instances with collision sound data (m_pInstanceAudio), near a listener and not already playing for that
 * hull.
 * The sound ID is the hull's entry (or the first), played through the world trigger manager; the record is
 * kept
 * in the play history for 1000 ticks.
 *
 * @param rider  the rider
 * @param evt  the instance hit (On Tour tInstanceRecord* pInst)
 * @param idx  collision hull
 */
void cSSXAudio::CollisionSound(cRider* rider, tInstanceRecord* evt, int idx)
{
    if (evt->m_pInstanceAudio == 0) return;
    if (cSSXAudio_CameraInRange(this, rider->getPosition(), 100.0f) < 0) return;
    if (cSSXAudio_PlayHistoryExists_p(this, evt, idx) != 0) return;
    t4Vector* hitVel = (t4Vector*)rider->getVelocity();
    int volume = (int)AudioTable_evaluateByIndex(0, sndVu0Length(*hitVel));
    if (volume > 127) volume = 127;
    if (volume < 0) volume = 0;
    int* tbl = *(int**)((char*)evt->m_pInstanceAudio + 0x10);
    if (idx >= tbl[0]) idx = 0;
    int* ids = tbl + 2;
    int* trigger = ids;
    if (ids[idx] != 0) trigger = &ids[idx];
    int handle = cWorldTriggerManager_HandleCollision(gWorldTriggerManager, evt, *trigger, ((char*)rider), volume);
    if (handle < 0) return;
    PlayHistoryAdd(evt, handle,
                             MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCounter + 1000, 0, idx);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_Wipeout);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void cCrowdSys_PlayFallCheer(cCrowdSys* self, int a1, int i);
extern "C" void cCrowdSys_CancelAnticipation(cCrowdSys* self, int i);
extern "C" void func_0028F108(void* self, void* rider);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");

// PORT: PS2-only inline asm (float absolute value).
static inline float absf_961F0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Makes the crowd react to a rider's wipeout by the size of its boost loss, then calls func_0028F108.
 *
 * The view is 0 when replaying, the rider's viewport (+0x870) for a local human, else -1; nothing happens unless the
 * rider is that view's focus rider (cSSXAudio_GetFocusRider). With |@p intensity| at 0 or below the crowd gets
 * cCrowdSys_CancelAnticipation, else cCrowdSys_PlayFallCheer with level 0 (below 0.25), 1 (below 0.65) or 2. Then func_0028F108(rider).
 * cRider_flagEventWipeout calls it with the boost from cRiderMetrix_evWipeout.
 *
 * @param rider  the rider
 * @param intensity  the boost change from cRiderMetrix_evWipeout
 */
void cSSXAudio::Wipeout(cRider* rider, float intensity)
{
    int idx = -1;
    if (cSSXAudio_ReplayMode(this) != 0) {
        idx = 0;
    } else {
        int ok = rider->mIsHuman != 0 && rider->mIsLocal != 0;
        if (ok) {
            idx = rider->mViewIndex;
        }
    }
    if (cSSXAudio_GetFocusRider_p(this, idx) != rider) {
        return;
    }
    float magnitude = absf_961F0(intensity);
    if (magnitude <= 0.0f) {
        cCrowdSys_CancelAnticipation((cCrowdSys*)this, idx);
    } else if (magnitude < 0.25f) {
        cCrowdSys_PlayFallCheer((cCrowdSys*)this, 0, idx);
    } else if (magnitude < 0.65f) {
        cCrowdSys_PlayFallCheer((cCrowdSys*)this, 1, idx);
    } else {
        cCrowdSys_PlayFallCheer((cCrowdSys*)this, 2, idx);
    }
    func_0028F108(this, ((char*)rider));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_WipeoutStart);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * No code references it; it sits between cSSXAudio_Wipeout and cSSXAudio_WipeoutHit.
 */
extern "C" void cSSXAudio_WipeoutStart(void) {

}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_WipeoutHit);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// handle lists (tCrowdEventData_tCrowdEventData clears them; tCrowdEventData_GetRandomSample picks from one) and their timing floats.
extern "C" void* tCrowdEventData_tCrowdEventData(tCrowdEventData* self);
struct sSndList5 {
    char pad0[0x18];
    sSndList5() { tCrowdEventData_tCrowdEventData((tCrowdEventData*)(this)); }
};
struct sSndRandSlot {
    sSndList5 a;
    sSndList5 b;
    sSndList5 c;
    float f48;
    float f4C;
    int f50;
    void* operator new[](unsigned int, void* p) { return p; }
};
struct sSndPair {
    int a;
    int b;
};
struct sSndSys8140 : public cBankSys {
    sSndPair* e10;           // 0x10
};
struct sSndSys69A0 : public sSndSys8140 {
    int f14;                 // 0x14
    int f18;                 // 0x18
    sSndRandSlot ent[3];     // 0x1C
};

struct sSndQuad2 {
    t4Vector a;
    t4Vector b;
};
struct sSSXAudioView : public sSndSys69A0 {  // a view of cSSXAudio (sound/ssxaudio.h) as this unit first modelled it
    cBankManager** mpVStreamSys; // 0x118 MusicSys.mpVStreamSys (sound/musicsys.h); typed as the address of the
                                 //       cStreamSys's first word, its mpVBankManager
    char pad11C[0x597C - 0x11C];
    cCircBuffer mPlayHistory; // 0x597C
    char pad598C[0x59E0 - 0x598C];
    int handle59E0;          // 0x59E0
    int count59E4;           // 0x59E4
    char pad59E8[0x5FB8 - 0x59E8];
    int handle5FB8;          // 0x5FB8
    char pad5FBC[0x5FE0 - 0x5FBC];
    int f5FE0[2];            // 0x5FE0
    unsigned short f5FE8[2]; // 0x5FE8
    int f5FEC[2];            // 0x5FEC
    int handle5FF4;          // 0x5FF4
    char pad5FF8[0x6030 - 0x5FF8];
    int slots[3];            // 0x6030
    char pad603C[0x6040 - 0x603C];
    int users;               // 0x6040
    char pad6044[0x6050 - 0x6044];
    sSndQuad2 xf;            // 0x6050 rider transform pair (cAvalanche_getAvalancheDataByPosition output)
    int idx;                 // 0x6070 rider index (cBeing::getAIIndex)
};

// cSndPlayOpts::OPTS_Callback (prototype name): sets the handler of the voice being pushed.
static inline void OPTS_Callback(cBankManager* m, sSndCallback cb)
{
    cBankMonitor* s = &m->BankMonitor;
    s->mCallback[m->BankMonitor.mInst] = cb;
}

// Callees declared by several functions below.
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" {
void* memset(void* dst, int c, int n);
void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
int cBankSys_SFX_End(cBankSys* self);
int cSSXAudio_GetClosestCamera1(void* self, void* obj);
int cSSXAudio_ReplayMode(void* self);
int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
unsigned int cSoundSys_SndRand(void* rng);

}

static inline void optsVol_6310(struct cBankManager* m, int v)
{
    struct cBankMonitor* o = &m->BankMonitor;
    o->mBaseVol[m->BankMonitor.mInst] = v;
    o->mCurrent[m->BankMonitor.mInst].vol = SND_MAX(0, SND_MIN(o->mBaseVol[o->mInst], 0x7F));
}

static inline void sfxBegin_6310(struct sSSXAudioView* self, int a, int b)
{
    struct cBankManager* m = self->mpVBankManager;
    struct cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
}

extern AudioTable D_00482DA0;
/**
 * Plays a rider's wipeout impact sound: bank 10 patch 9 in states 2 and 3, else a random bank-0 patch from 48.
 *
 * Computer and non-local riders (isComputer, or mIsLocal clear) farther than 100 units from every camera
 * (cSSXAudio_CameraInRange) play nothing. The volume is curve D_00482DA0 at the rider's LastLandSpeed, clamped to
 * 0..127. When the rider's mSurfaceType is eST_Snow_Powder or eST_Snow_DeepPowder the sound is bank 10, patch 9;
 * otherwise bank 0, patch 48 + (cSoundSys_SndRand & 0x7FFF) * 3 / 0x7FFF. Opens a play-option level (the inlined
 * SFX_Begin) with that volume, volume control cSSXAudio_GetCurrAppliedVolumePtr(5, 0), the rider's mPosition to follow
 * and the listening view (cSSXAudio_GetClosestCamera1), plus the rider's mViewIndex for a local human when
 * newRace.cGamePath is kGAMEPATH_MULTIPLAYER and not replaying. cBankSys_SFX_End plays it; the handle is not kept.
 *
 * Likely bug: a random value of 0x7FFF gives offset 3, patch 51, which is the wipeout-slide sound of cSSXAudio_WipeoutSlideStart.
 * Called by cRider_flagEventWipeout.
 *
 * @param obj  the rider
 */
void cSSXAudio::WipeoutHit(cRider* obj)
{
    struct sSSXAudioView* self = (struct sSSXAudioView*)this;
    if (obj->isComputer() || obj->mIsLocal == 0) {
        if (cSSXAudio_CameraInRange(self, obj->getPosition(), 100.0f) < 0) {
            return;
        }
    }
    struct AudioTable curve = D_00482DA0;
    int vol = (int)AudioTable_evaluate(&curve, obj->LastLandSpeed);
    int bankNum = 0;
    int patchNum;
    if (vol > 0x7F) {
        vol = 0x7F;
    }
    if (vol < 0) {
        vol = 0;
    }
    int st = obj->mSurfaceType;
    if (st == eST_Snow_Powder)
        goto state23;
    if (st == eST_Snow_DeepPowder) {
state23:
        bankNum = 10;
        patchNum = 9;
    } else {
        patchNum = (cSoundSys_SndRand((*self->mpVStreamSys)->BankMonitor.mpVSoundSys) & 0x7FFF) * 3 / 0x7FFF + 0x30;
    }
    int flag = 0;
    int ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
            flag = cSSXAudio_ReplayMode(self) == 0;
        }
    }
    sfxBegin_6310(self, bankNum, patchNum);
    optsVol_6310(*self->mpVStreamSys, vol);
    struct cBankManager** pp = self->mpVStreamSys;
    float* volumeControl = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 5, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = volumeControl;
    {
        struct cBankManager* optsManager = *self->mpVStreamSys;
        optsManager->BankMonitor.mPosition[optsManager->BankMonitor.mInst] = &obj->mPosition;
    }
    {
        struct cBankManager** pq = self->mpVStreamSys;
        int listenerView = cSSXAudio_GetClosestCamera1(self, ((char*)obj));
        (*pq)->BankMonitor.mViewport[(*pq)->BankMonitor.mInst] = listenerView;
    }
    if (flag) {
        (*self->mpVStreamSys)->BankMonitor.mTwoPlayerPan[(*self->mpVStreamSys)->BankMonitor.mInst] = 1;
        (*self->mpVStreamSys)->BankMonitor.mTwoPlayerSide[(*self->mpVStreamSys)->BankMonitor.mInst] = obj->mViewIndex;
    }
    self->SFX_End();
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_WipeoutSlideStart);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
class cController;
class cBankMonitorInstance;
// Sound callback: a g++ 2.95 pointer to member function (8 bytes), as cSndPlayOpts::OPTS_Callback takes it.
typedef void (cController::*sSndCallback)(cController*, cBankMonitorInstance*, float);
// One cSndPlayOpts mDelay[] entry (reset to 100 / 0 / 90 / 50 on every push).
#define SND_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SND_OPTS_MAX(a, b) ((a) >= (b) ? (a) : (b))

// cSndPlayOpts (prototype class): the play-option stack; push = cur++ and copy def into mCurrent[cur]. The
// allocation tags of its ctor (sound/ps2soundman) name the arrays: ch = mCurrent, p34 = mPosStatic,
// vals = mBaseVol, p3C = mVolControl, p60 = mPtr, p64 = mViewport, p68 = mVanish, p6C = mRadius, p70 = mDelay,
// p74 = mExternalVol, p78 = mMapToMaxVol, p7C = mCloseOnEnd, p80 = mTwoPlayerPan, p84 = mTwoPlayerSide.
// cBankManager as this unit sees it: only its cSndPlayOpts part at 0x1D8 is used here.
// handle lists (tCrowdEventData_tCrowdEventData clears them; tCrowdEventData_GetRandomSample picks from one) and their timing floats.
extern "C" void* tCrowdEventData_tCrowdEventData(tCrowdEventData* self);
// sSSXAudioView's pending one-shot queue (an int ring of 5-word records).
// cSndPlayOpts::OPTS_Callback (prototype name): sets the handler of the voice being pushed.
// Callees declared by several functions below.
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
extern "C" unsigned int cSoundSys_SndRand(void* rng);

extern sSndCallback D_004A36B0_cb[] __asm__("D_004A36B0");
extern AudioTable D_00482DA0;
/**
 * Starts a wiping-out rider's slide loop (bank 0, patch 51) unless it is already playing.
 *
 * Does nothing while the slide handle (mWipeoutSlideHandle of @p wipeoutControl's mRider) is 0 or more. Computer and
 * non-local riders farther than 100 units from every camera (cSSXAudio_CameraInRange) get nothing. The volume is curve
 * D_00482DA0 at the rider's LastLandSpeed, clamped to 0..127. The play-option level (the inlined SFX_Begin) gets that
 * volume, volume control cSSXAudio_GetCurrAppliedVolumePtr(5, 0), the rider's mPosition to follow, the listening view
 * (cSSXAudio_GetClosestCamera1), @p wipeoutControl as user data, callback key D_004A36B0 (cSSXAudio_HandleWipeoutSlide,
 * empty), the +0x7C option cleared and, for a local human when the byte at newRace.cGamePath is kGAMEPATH_MULTIPLAYER
 * and not replaying, the rider's mViewIndex. cBankSys_SFX_End's handle goes to the rider's mWipeoutSlideHandle;
 * cSSXAudio_WipeoutSlideEnd stops it.
 *
 * The wipeout control calls it when the rider's animation category is 29 or 25.
 *
 * @param wipeoutControl  the wipeout control (its rider is mRider)
 */
void cSSXAudio::WipeoutSlideStart(cWipeoutControl* wipeoutControl)
{
    sSSXAudioView* self = (sSSXAudioView*)this;
    if (wipeoutControl->mRider->mWipeoutSlideHandle >= 0) {
        return;
    }
    if (wipeoutControl->mRider->isComputer() || wipeoutControl->mRider->mIsLocal == 0) {
        if (cSSXAudio_CameraInRange(self, wipeoutControl->mRider->getPosition(), 100.0f) < 0) {
            return;
        }
    }
    AudioTable curve = D_00482DA0;
    int vol = (int)AudioTable_evaluate(&curve, wipeoutControl->mRider->LastLandSpeed);
    int patchNum = 51;
    if (vol > 0x7F) {
        vol = 0x7F;
    }
    if (vol < 0) {
        vol = 0;
    }
    int flag = 0;
    bool ok = wipeoutControl->mRider->mIsHuman && wipeoutControl->mRider->mIsLocal;
    if (ok) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
            flag = cSSXAudio_ReplayMode(self) == 0;
        }
    }
    cBankManager* bankManager = self->mpVBankManager;
    cBankMonitor* playOpts = &bankManager->BankMonitor;
    bankManager->BankMonitor.mInst++;
    playOpts->mCurrent[bankManager->BankMonitor.mInst] = bankManager->BankMonitor.mDefault;
    bankManager->BankMonitor.mPosition[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPosStatic[bankManager->BankMonitor.mInst] = 0;
    playOpts->mBaseVol[bankManager->BankMonitor.mInst] = playOpts->mCurrent[bankManager->BankMonitor.mInst].vol;
    playOpts->mVolControl[bankManager->BankMonitor.mInst] = 0;
    playOpts->mCallback[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPtr[bankManager->BankMonitor.mInst] = 0;
    playOpts->mViewport[bankManager->BankMonitor.mInst] = 0;
    playOpts->mVanish[bankManager->BankMonitor.mInst] = 0;
    playOpts->mRadius[bankManager->BankMonitor.mInst] = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mCount = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mTime = 100;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mFeedback = 90;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mLevel = 50;
    playOpts->mExternalVol[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerPan[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerSide[bankManager->BankMonitor.mInst] = 0;
    playOpts->mMapToMaxVol[bankManager->BankMonitor.mInst] = 0x7F;
    playOpts->mCloseOnEnd[bankManager->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = 0;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = patchNum;
    cBankManager* volumeManager = *self->mpVStreamSys;
    cBankMonitor* volumeOpts = &volumeManager->BankMonitor;
    volumeOpts->mBaseVol[volumeManager->BankMonitor.mInst] = vol;
    volumeOpts->mCurrent[volumeManager->BankMonitor.mInst].vol
            = (volumeOpts->mBaseVol[volumeOpts->mInst] < 0x7F && volumeOpts->mBaseVol[volumeOpts->mInst] < 0) ? 0
            : (volumeOpts->mBaseVol[volumeOpts->mInst] < 0x7F ? (unsigned char)volumeOpts->mBaseVol[volumeOpts->mInst]
            : 0x7F);
    cBankManager** pp = self->mpVStreamSys;
    float* volumeControl = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 5, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = volumeControl;
    {
        cBankManager* optsManager = *self->mpVStreamSys;
        optsManager->BankMonitor.mPosition[optsManager->BankMonitor.mInst] = &wipeoutControl->mRider->mPosition;
    }
    {
        cBankManager** pq = self->mpVStreamSys;
        int listenerView = cSSXAudio_GetClosestCamera1(self, wipeoutControl ? wipeoutControl->mRider : 0);
        (*pq)->BankMonitor.mViewport[(*pq)->BankMonitor.mInst] = listenerView;
    }
    {
        cBankManager* optsManager = *self->mpVStreamSys;
        optsManager->BankMonitor.mPtr[optsManager->BankMonitor.mInst] = wipeoutControl;
    }
    OPTS_Callback(*self->mpVStreamSys, D_004A36B0_cb[0]);
    (*self->mpVStreamSys)->BankMonitor.mCloseOnEnd[(*self->mpVStreamSys)->BankMonitor.mInst] = 0;
    if (flag) {
        (*self->mpVStreamSys)->BankMonitor.mTwoPlayerPan[(*self->mpVStreamSys)->BankMonitor.mInst] = 1;
        (*self->mpVStreamSys)->BankMonitor.mTwoPlayerSide[(*self->mpVStreamSys)->BankMonitor.mInst]
                = wipeoutControl->mRider->mViewIndex;
    }
    wipeoutControl->mRider->mWipeoutSlideHandle = self->SFX_End();
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleWipeoutSlide);
#ifdef SKIP_ASM
/**
 * Does nothing: the monitor callback of the wipeout-slide loop (key D_004A36B0, armed by cSSXAudio_WipeoutSlideStart).
 *
 * cSSXAudio_StopAllRiderSounds cancels monitors by this key.
 */
extern "C" void cSSXAudio_HandleWipeoutSlide(void) {

}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_WipeoutSlideEnd);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Stops a rider's wipeout-slide loop with a 0.75 fade and clears its handle.
 *
 * When the rider's mWipeoutSlideHandle is 0 or more it calls cBankMonitor_BANKMONITOR_StopHandle (type 1, fade 0.75) on
 * the bank monitor of cAudioInterface_bxAudio(), not of @p self, and sets the handle to -1. Called by the wipeout
 * control with its rider, and by the audio code for rider 0 (cSSXAudio_HandleMusicEvent among others).
 *
 * @param obj  the rider
 */
void cSSXAudio::WipeoutSlideEnd(cRider* obj)
{
    if (obj->mWipeoutSlideHandle >= 0) {
        cBankMonitor_BANKMONITOR_StopHandle(
            &cAudioInterface_bxAudio()->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
            obj->mWipeoutSlideHandle, 1, 0.75f);
        obj->mWipeoutSlideHandle = -1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_WipeoutTumbleStart);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
class cController;
class cBankMonitorInstance;
// Sound callback: a g++ 2.95 pointer to member function (8 bytes), as cSndPlayOpts::OPTS_Callback takes it.
typedef void (cController::*sSndCallback)(cController*, cBankMonitorInstance*, float);
// One cSndPlayOpts mDelay[] entry (reset to 100 / 0 / 90 / 50 on every push).
#define SND_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SND_OPTS_MAX(a, b) ((a) >= (b) ? (a) : (b))

// cSndPlayOpts (prototype class): the play-option stack; push = cur++ and copy def into mCurrent[cur]. The
// allocation tags of its ctor (sound/ps2soundman) name the arrays: ch = mCurrent, p34 = mPosStatic,
// vals = mBaseVol, p3C = mVolControl, p60 = mPtr, p64 = mViewport, p68 = mVanish, p6C = mRadius, p70 = mDelay,
// p74 = mExternalVol, p78 = mMapToMaxVol, p7C = mCloseOnEnd, p80 = mTwoPlayerPan, p84 = mTwoPlayerSide.
// cBankManager as this unit sees it: only its cSndPlayOpts part at 0x1D8 is used here.
// handle lists (tCrowdEventData_tCrowdEventData clears them; tCrowdEventData_GetRandomSample picks from one) and their timing floats.
extern "C" void* tCrowdEventData_tCrowdEventData(tCrowdEventData* self);
// sSSXAudioView's pending one-shot queue (an int ring of 5-word records).
// cSndPlayOpts::OPTS_Callback (prototype name): sets the handler of the voice being pushed.
// Callees declared by several functions below.
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
extern "C" unsigned int cSoundSys_SndRand(void* rng);

extern sSndCallback D_004A36B8_cb[] __asm__("D_004A36B8");
extern AudioTable D_00482DA0;
/**
 * Starts the wipeout tumble sound of the rider a wipeout control drives, unless one is already playing.
 *
 * Does nothing while the rider's mWipeoutTumbleHandle is set (>= 0). Computer and non-local riders more than 100 units
 * from every camera (cSSXAudio_CameraInRange on the position from cRider vtable slot 5) are skipped. The volume is the
 * curve D_00482DA0 (AudioTable_evaluate) of the rider's LastLandSpeed, clamped to 0..127. A play-option level is pushed with
 * defaults (the inlined SFX_Begin) for bank 0, patch 52 and gets that volume, the volume control
 * cSSXAudio_GetCurrAppliedVolumePtr(vself, 5, 0), the rider's mPosition to follow, the view from
 * cSSXAudio_GetClosestCamera1, @p wipeoutControl as its pointer, the empty callback cSSXAudio_HandleWipeoutTumble
 * (pointer to member D_004A36B8) and mCloseOnEnd 0. For a local human (mIsHuman and mIsLocal) when the new-race game
 * path (newRace.cGamePath) is kGAMEPATH_MULTIPLAYER and not replaying, it also gets two-player pan on side mViewIndex.
 * cBankSys_SFX_End starts it and the handle is stored in mWipeoutTumbleHandle.
 *
 * @param wipeoutControl  the rider's wipeout control (its rider is mRider)
 */
void cSSXAudio::WipeoutTumbleStart(cWipeoutControl* wipeoutControl)
{
    sSSXAudioView* self = (sSSXAudioView*)this;
    if (wipeoutControl->mRider->mWipeoutTumbleHandle >= 0) {
        return;
    }
    if (wipeoutControl->mRider->isComputer() || wipeoutControl->mRider->mIsLocal == 0) {
        if (cSSXAudio_CameraInRange(self, wipeoutControl->mRider->getPosition(), 100.0f) < 0) {
            return;
        }
    }
    AudioTable curve = D_00482DA0;
    int vol = (int)AudioTable_evaluate(&curve, wipeoutControl->mRider->LastLandSpeed);
    int patchNum = 52;
    if (vol > 0x7F) {
        vol = 0x7F;
    }
    if (vol < 0) {
        vol = 0;
    }
    int flag = 0;
    bool ok = wipeoutControl->mRider->mIsHuman && wipeoutControl->mRider->mIsLocal;
    if (ok) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
            flag = cSSXAudio_ReplayMode(self) == 0;
        }
    }
    cBankManager* bankManager = self->mpVBankManager;
    cBankMonitor* playOpts = &bankManager->BankMonitor;
    bankManager->BankMonitor.mInst++;
    playOpts->mCurrent[bankManager->BankMonitor.mInst] = bankManager->BankMonitor.mDefault;
    bankManager->BankMonitor.mPosition[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPosStatic[bankManager->BankMonitor.mInst] = 0;
    playOpts->mBaseVol[bankManager->BankMonitor.mInst] = playOpts->mCurrent[bankManager->BankMonitor.mInst].vol;
    playOpts->mVolControl[bankManager->BankMonitor.mInst] = 0;
    playOpts->mCallback[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPtr[bankManager->BankMonitor.mInst] = 0;
    playOpts->mViewport[bankManager->BankMonitor.mInst] = 0;
    playOpts->mVanish[bankManager->BankMonitor.mInst] = 0;
    playOpts->mRadius[bankManager->BankMonitor.mInst] = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mCount = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mTime = 100;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mFeedback = 90;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mLevel = 50;
    playOpts->mExternalVol[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerPan[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerSide[bankManager->BankMonitor.mInst] = 0;
    playOpts->mMapToMaxVol[bankManager->BankMonitor.mInst] = 0x7F;
    playOpts->mCloseOnEnd[bankManager->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = 0;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = patchNum;
    cBankManager* volManager = *self->mpVStreamSys;
    cBankMonitor* volOpts = &volManager->BankMonitor;
    volOpts->mBaseVol[volManager->BankMonitor.mInst] = vol;
    volOpts->mCurrent[volManager->BankMonitor.mInst].vol
            = (volOpts->mBaseVol[volOpts->mInst] < 0x7F && volOpts->mBaseVol[volOpts->mInst] < 0) ? 0
            : (volOpts->mBaseVol[volOpts->mInst] < 0x7F ? (unsigned char)volOpts->mBaseVol[volOpts->mInst] : 0x7F);
    cBankManager** pp = self->mpVStreamSys;
    float* volControl = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 5, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = volControl;
    {
        cBankManager* levelManager = *self->mpVStreamSys;
        levelManager->BankMonitor.mPosition[levelManager->BankMonitor.mInst] = &wipeoutControl->mRider->mPosition;
    }
    {
        cBankManager** pq = self->mpVStreamSys;
        int viewIndex = cSSXAudio_GetClosestCamera1(self, wipeoutControl ? wipeoutControl->mRider : 0);
        (*pq)->BankMonitor.mViewport[(*pq)->BankMonitor.mInst] = viewIndex;
    }
    {
        cBankManager* levelManager = *self->mpVStreamSys;
        levelManager->BankMonitor.mPtr[levelManager->BankMonitor.mInst] = wipeoutControl;
    }
    OPTS_Callback(*self->mpVStreamSys, D_004A36B8_cb[0]);
    (*self->mpVStreamSys)->BankMonitor.mCloseOnEnd[(*self->mpVStreamSys)->BankMonitor.mInst] = 0;
    if (flag) {
        (*self->mpVStreamSys)->BankMonitor.mTwoPlayerPan[(*self->mpVStreamSys)->BankMonitor.mInst] = 1;
        (*self->mpVStreamSys)->BankMonitor.mTwoPlayerSide[(*self->mpVStreamSys)->BankMonitor.mInst]
                = wipeoutControl->mRider->mViewIndex;
    }
    wipeoutControl->mRider->mWipeoutTumbleHandle = self->SFX_End();
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleWipeoutTumble);
#ifdef SKIP_ASM
/**
 * Does nothing: the callback of the wipeout tumble sound.
 *
 * cSSXAudio_WipeoutTumbleStart installs it (pointer to member D_004A36B8) as the play-option callback of the tumble voice.
 */
extern "C" void cSSXAudio_HandleWipeoutTumble(void) {

}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_WipeoutTumbleEnd);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Stops a rider's wipeout tumble sound, if one is playing.
 *
 * When the rider's mWipeoutTumbleHandle is set (>= 0) it is stopped through cBankMonitor_BANKMONITOR_StopHandle (stop
 * mode 1, 0.75) on the cBankMonitor of cAudioInterface_bxAudio() (not
 * @p self), and the handle is reset to -1.
 *
 * @param obj  the rider (cRider)
 */
void cSSXAudio::WipeoutTumbleEnd(cRider* obj)
{
    if (obj->mWipeoutTumbleHandle >= 0) {
        cBankMonitor_BANKMONITOR_StopHandle(
            &cAudioInterface_bxAudio()->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
            obj->mWipeoutTumbleHandle, 1, 0.75f);
        obj->mWipeoutTumbleHandle = -1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_WipeoutEnd);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty function; the retail image has no call or data reference to it).
 */
extern "C" void cSSXAudio_WipeoutEnd(void) {

}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_WorldSound);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);

#define SND_74A0_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_74A0_MAX(a, b) ((a) > (b) ? (a) : (b))

extern "C" void cSndPlayOpts_OPTS_Radius(void* self, float a, float b);
extern "C" int cSSXAudio_GetClosestCamera(void* self, t4Vector* pos);

static inline void snd74A0AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = ((cBankManager*)self->mpVBankManager);
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = a;
    self->mPatch[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = b;
    cBankManager* m2 = self->MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* s2 = &m2->BankMonitor;
    s2->mBaseVol[m2->BankMonitor.mInst] = 0x7F;
    s2->mCurrent[m2->BankMonitor.mInst].vol = SND_74A0_MAX(0, SND_74A0_MIN(s2->mBaseVol[s2->mInst], 0x7F));
    cBankManager** pp = (&self->MusicSys.mpVStreamSys->mpVBankManager);
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
}


extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" bool cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);

/**
 * Plays a one-shot positional sound at a world instance, chosen by its patch number.
 *
 * Patch 0 plays nothing, 102 is skipped at location 0, and nothing plays while mLoadingLoopHandle is set or the
 * instance is
 * already in the play history. Patches below 100 use bank 2, 100-149 bank 8, 150-199 bank 9 and 200+ bank 4.
 *
 * @param evt  the instance (On Tour tInstanceRecord* pInst)
 * @param n  patch number
 */
void cSSXAudio::WorldSound(tInstanceRecord* evt, int n)
{
    // ORIGINAL-PTR: On Tour's DWARF declares cSSXAudio::PlayHistoryExists(int Ptr, int Hull): EA passed the
    // instance pointer as an int key.

    if (n == 0) return;
    if (n == 102 && *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)) == 0) return;
    if (mLoadingLoopHandle >= 0) return;
    if (PlayHistoryExists((int)evt, 0) != 0) return;
    int pri = 80;
    t4Vector pos = *(t4Vector*)evt->m_fmTransform.m[3];
    int type = 2;
    int patch;
    if (n < 100) {
        patch = n;
    } else if (n < 150) {
        patch = n - 100;
        type = 8;
    } else if (n < 200) {
        patch = n - 150;
        type = 9;
    } else {
        patch = n - 200;
        type = 4;
    }
    snd74A0AddVoice(this, type, patch, 5);
    cStreamSys* streamSys;
    int closestCamera;
    cSSXAudio* sys = this;
    cBankManager* voiceMgr = sys->MusicSys.mpVStreamSys->mpVBankManager;
    voiceMgr->BankMonitor.mPosition[voiceMgr->BankMonitor.mInst] = &pos;
    cBankManager* voiceMgr2 = sys->MusicSys.mpVStreamSys->mpVBankManager;
    voiceMgr2->BankMonitor.mPosStatic[voiceMgr2->BankMonitor.mInst] = 1;
    cSndPlayOpts_OPTS_Radius(&sys->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, 300.0f, -1.0f);
    streamSys = sys->MusicSys.mpVStreamSys;
    closestCamera = cSSXAudio_GetClosestCamera(sys, &pos);
    streamSys->mpVBankManager->BankMonitor.mViewport[streamSys->mpVBankManager->BankMonitor.mInst] = closestCamera;
    cBankManager* voiceMgr3 = sys->MusicSys.mpVStreamSys->mpVBankManager;
    voiceMgr3->BankMonitor.mMapToMaxVol[voiceMgr3->BankMonitor.mInst] = pri;
    int sfxHandle = ((cBankSys*)this)->SFX_End();
    PlayHistoryAdd(evt, sfxHandle,
                             MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCounter + 50, 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_AttachSound);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
class cController;
class cBankMonitorInstance;
// Sound callback: a g++ 2.95 pointer to member function (8 bytes), as cSndPlayOpts::OPTS_Callback takes it.
typedef void (cController::*sSndCallback)(cController*, cBankMonitorInstance*, float);
// One cSndPlayOpts mDelay[] entry (reset to 100 / 0 / 90 / 50 on every push).
#define SND_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SND_OPTS_MAX(a, b) ((a) >= (b) ? (a) : (b))

// cSndPlayOpts (prototype class): the play-option stack; push = cur++ and copy def into mCurrent[cur]. The
// allocation tags of its ctor (sound/ps2soundman) name the arrays: ch = mCurrent, p34 = mPosStatic,
// vals = mBaseVol, p3C = mVolControl, p60 = mPtr, p64 = mViewport, p68 = mVanish, p6C = mRadius, p70 = mDelay,
// p74 = mExternalVol, p78 = mMapToMaxVol, p7C = mCloseOnEnd, p80 = mTwoPlayerPan, p84 = mTwoPlayerSide.
// cBankManager as this unit sees it: only its cSndPlayOpts part at 0x1D8 is used here.
// handle lists (tCrowdEventData_tCrowdEventData clears them; tCrowdEventData_GetRandomSample picks from one) and their timing floats.
extern "C" void* tCrowdEventData_tCrowdEventData(tCrowdEventData* self);
// sSSXAudioView's pending one-shot queue (an int ring of 5-word records).
// cSndPlayOpts::OPTS_Callback (prototype name): sets the handler of the voice being pushed.
// Callees declared by several functions below.
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
static inline void sndPushVoice(void* vself, int a, int b)
{
    cBankSys* self = (cBankSys*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
}

extern "C" int* cBENewRaceInterface_getLocation(void* p);
extern "C" void* cAudioInterface_getBE();

static inline int snd7950Commit(void* vself, void* tgt, float f, int pri)
{
    sSSXAudioView* self = (sSSXAudioView*)vself;
    (*self->mpVStreamSys)->BankMonitor.mPosition[(*self->mpVStreamSys)->BankMonitor.mInst] = (const t4Vector*)tgt;
    cSndPlayOpts_OPTS_Radius(&(*self->mpVStreamSys)->BankMonitor, f, -1.0f);
    (*self->mpVStreamSys)->BankMonitor.mMapToMaxVol[(*self->mpVStreamSys)->BankMonitor.mInst] = pri;
    return self->SFX_End();
}

/**
 * Attaches a positional sound to a world instance (or one mesh of it) in one of the 30 attachment slots.
 *
 * Uses the same patch filters and bank ranges as WorldSound(); an existing attachment of the same patch to
 * the
 * instance is replaced, and nothing happens when all slots are used. Patch 201 at location 8 gets a longer
 * range
 * and lower priority.
 *
 * @param obj  the instance (On Tour tInstanceRecord* pInst)
 * @param type  patch number
 * @param id  mesh index to follow, or tAttachSoundStruct::kModelMesh for the instance's model
 */
void cSSXAudio::AttachSound(tInstanceRecord* obj, int type, int id)
{
    if (type == 0)
        return;
    if (type == 102) {
        if (*cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)) == 0)
            return;
    }
    if (mLoadingLoopHandle >= 0)
        return;
    if (obj->m_pActiveNode == 0)
        return;
    int i;
    for (i = 0; i < 30; i++) {
        if (AttachedSounds[i].used == true && AttachedSounds[i].inst == obj &&
            AttachedSounds[i].patch == type) {
            cBankMonitor_BANKMONITOR_StopHandle(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                AttachedSounds[i].handle, 1, 0.0f);
            AttachedSounds[i].used = 0;
            break;
        }
    }
    for (i = 0; i < 30; i++) {
        if (AttachedSounds[i].used == 0) {
            AttachedSounds[i].used = 1;
            break;
        }
    }
    if (i == 30)
        return;
    AttachedSounds[i].inst = obj;
    if (id == tAttachSoundStruct::kModelMesh) {
        t4Vector pos;
        pos = obj->m_pActiveNode->getRootTransform()->row[3];
        AttachedSounds[i].pos = pos;
    } else {
        if (obj->m_pActiveNode->hasAnimMats()) {
            t4Vector pos;
            pos = obj->m_pActiveNode->getAnimMat(id)->row[3];
            AttachedSounds[i].pos = pos;
        } else {
            t4Vector pos;
            pos = *(t4Vector*)&obj->m_fmTransform.m[3][0];
            AttachedSounds[i].pos = pos;
        }
    }
    AttachedSounds[i].mesh = id;
    int curBank;
    int curPatch;
    if (type < 100) {
        curBank = 2;
        curPatch = type;
    } else if (type < 150) {
        curBank = 8;
        curPatch = type - 100;
    } else if (type < 200) {
        curBank = 9;
        curPatch = type - 150;
    } else {
        curBank = 4;
        curPatch = type - 200;
    }
    float radius = 300.0f;
    int pri = 80;
    AttachedSounds[i].patch = type;
    if (type == 201) {
        if (*cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)) == 8) {
            radius = 400.0f;
            pri = 50;
        }
    }
    sndPushVoice(((sSSXAudioView*)this), curBank, curPatch);
    cStreamSys* streamSys = MusicSys.mpVStreamSys;
    float* volumePtr = (float*)cSSXAudio_GetCurrAppliedVolumePtr(((sSSXAudioView*)this), 5, 0);
    streamSys->mpVBankManager->BankMonitor.mVolControl[streamSys->mpVBankManager->BankMonitor.mInst] = volumePtr;
    AttachedSounds[i].handle = snd7950Commit(((sSSXAudioView*)this), &AttachedSounds[i].pos, radius, pri);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_DetachSound);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Stops and frees every attached sound with the given instance and patch.
 *
 * @param inst  the instance (On Tour tInstanceRecord* pInst)
 * @param patch  patch number
 */
void cSSXAudio::DetachSound(tInstanceRecord* inst, int patch)
{
    for (int i = 0; i < 30; i++) {
        if (AttachedSounds[i].used == true &&
            AttachedSounds[i].inst == inst &&
            AttachedSounds[i].patch == patch) {
            cBankMonitor_BANKMONITOR_StopHandle(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                AttachedSounds[i].handle, 1, 0.0f);
            AttachedSounds[i].used = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_InitAttachedSounds);
#ifdef SKIP_ASM
/**
 * Marks all 30 attached-sound slots unused.
 */
void cSSXAudio::InitAttachedSounds()
{
    int i;
    for (i = 29; i >= 0; i--) {
        AttachedSounds[i].used = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UpdateAttachedSounds);
#ifdef SKIP_ASM
struct sSsVec4FA0 {
    float x, y, z, w;
    sSsVec4FA0() {}
} __attribute__((aligned(16)));

/**
 * Moves each attached sound to its instance's current position, freeing it once the instance has no model.
 */
void cSSXAudio::UpdateAttachedSounds()
{
    for (int i = 0; i < 30; i++) {
        if (AttachedSounds[i].used != true) continue;
        cInstanceNode* node = AttachedSounds[i].inst->m_pActiveNode;
        if (node == 0) {
            cBankMonitor_BANKMONITOR_StopHandle(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                AttachedSounds[i].handle, 1, 0.0f);
            AttachedSounds[i].used = 0;
        } else if (AttachedSounds[i].mesh == tAttachSoundStruct::kModelMesh) {
            sSsVec4FA0 pos;
            pos = *(sSsVec4FA0*)&node->getRootTransform()->row[3];
            *(sSsVec4FA0*)&AttachedSounds[i].pos = pos;
        } else {
            if (node->hasAnimMats()) {
                cInstanceNode* meshNode = AttachedSounds[i].inst->m_pActiveNode;
                sSsVec4FA0 pos;
                pos = *(sSsVec4FA0*)&meshNode->getAnimMat(AttachedSounds[i].mesh)->row[3];
                *(sSsVec4FA0*)&AttachedSounds[i].pos = pos;
            } else {
                sSsVec4FA0 pos;
                pos = *(sSsVec4FA0*)AttachedSounds[i].inst->m_fmTransform.m[3];
                *(sSsVec4FA0*)&AttachedSounds[i].pos = pos;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_ResetAttachedSounds);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Stops every attached sound and frees its slot.
 */
void cSSXAudio::ResetAttachedSounds()
{
    int i;
    for (i = 0; i < 30; i++) {
        if (AttachedSounds[i].used == true) {
            cBankMonitor_BANKMONITOR_StopHandle(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                AttachedSounds[i].handle, 1, 0.0f);
            AttachedSounds[i].used = 0;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_Punch);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


extern "C" bool cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);
extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);

static inline bool snd8138Active(cRider* o)
{
    return o->mIsHuman && o->mIsLocal;
}

static inline int snd8138AddVoice(void* vself, cRider* obj, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    cBankManager* m2 = self->MusicSys.mpVStreamSys->mpVBankManager;
    m2->BankMonitor.mPosition[m2->BankMonitor.mInst] = &obj->mPosition;
    pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    int listenerView = cSSXAudio_GetClosestCamera1(self, obj);
    (*pp)->BankMonitor.mViewport[(*pp)->BankMonitor.mInst] = listenerView;
    return ((cBankSys*)self)->SFX_End();
}

/**
 * Plays the punch sound when either rider is a local human, unless one is already in the play history (event
 * 5).
 *
 * @param obj  the rider
 * @param obj2  the opponent
 */
void cSSXAudio::Punch(cRider* obj, cRider* obj2)
{
    if (!snd8138Active(obj) && !snd8138Active(obj2)) {
        return;
    }
    if (PlayHistoryExists(5, 0) != 0) {
        return;
    }
    int handle = snd8138AddVoice(this, obj, 0, 90, 5);
    PlayHistoryAdd((void*)5, handle,
                             MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCounter + 50, 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_RiderCollision);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
int cSSXAudio_InCameraView_8488(void* self, t4Vector* pos, float radius) __asm__("cSSXAudio_InCameraView");
extern "C" bool cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);
extern "C" void cSndPlayOpts_OPTS_Radius(void* self, float a, float b);
extern "C" float AudioTable_evaluateByIndex(int i, float v);


// PORT: PS2-only VU0 inline asm (4-component add).
static inline t4Vector vu0Add_8488(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_8488(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}



static inline int snd8488Add(void* vself, cRider* obj, int a, int b, int c)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager* m3 = self->MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* s3 = &m3->BankMonitor;
    s3->mBaseVol[m3->BankMonitor.mInst] = c;
    s3->mCurrent[m3->BankMonitor.mInst].vol = (s3->mBaseVol[s3->mInst] < 0x7F && s3->mBaseVol[s3->mInst] < 0) ? 0
            : (s3->mBaseVol[s3->mInst] < 0x7F ? (unsigned char)s3->mBaseVol[s3->mInst] : 0x7F);
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 5, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    cSndPlayOpts_OPTS_Radius(&(self->MusicSys.mpVStreamSys->mpVBankManager)->BankMonitor, 100.0f, -1.0f);
    pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    int listenerView = cSSXAudio_GetClosestCamera1(self, obj);
    (*pp)->BankMonitor.mViewport[(*pp)->BankMonitor.mInst] = listenerView;
    cBankManager* m2 = self->MusicSys.mpVStreamSys->mpVBankManager;
    m2->BankMonitor.mPosition[m2->BankMonitor.mInst] = &obj->mPosition;
    return ((cBankSys*)self)->SFX_End();
}

/**
 * Plays the collision sound of two riders that bump into each other, near a camera.
 *
 * Plays nothing when @p rider is more than 100 units from every camera (cSSXAudio_CameraInRange) or outside every camera
 * view (cSSXAudio_InCameraView), when neither rider is a local human (mIsHuman and mIsLocal), or while play-history event 7 is
 * still live (cSSXAudio_PlayHistoryExists). The volume is AudioTable_evaluateByIndex(1, length of the sum of both riders'
 * velocities, from cRider vtable slot 2). A play-option level is pushed with defaults for bank 0, patch 54 with that
 * volume, the volume control cSSXAudio_GetCurrAppliedVolumePtr(self, 5, 0), a 3D radius of 100 (cSndPlayOpts_OPTS_Radius), the view of @p rider
 * (cSSXAudio_GetClosestCamera1) and @p rider's mPosition to follow; cBankSys_SFX_End starts it and the handle goes into the play history
 * as event 7 for 50 ms past the monitor clock (cSSXAudio_PlayHistoryAdd).
 *
 * @param rider  the rider whose position the sound follows
 * @param opponent  the other rider
 */
void cSSXAudio::RiderCollision(cRider* rider, cRider* opponent)
{
    if (cSSXAudio_CameraInRange(this, rider->getPosition(), 100.0f) < 0) return;
    if (cSSXAudio_InCameraView_8488(this, rider->getPosition(), 100.0f) == 0) return;
    bool okA = rider->mIsHuman && rider->mIsLocal;
    if (!okA) {
        bool okB = opponent->mIsHuman && opponent->mIsLocal;
        if (!okB) return;
    }
    if (PlayHistoryExists(7, 0) != 0) return;
    t4Vector sum = vu0Add_8488(*rider->getVelocity(), *opponent->getVelocity());
    int volume = (int)AudioTable_evaluateByIndex(1, (float)(int)vu0Length_8488(sum));
    int handle = snd8488Add(this, rider, 0, 54, volume);
    PlayHistoryAdd((void*)7, handle,
                             MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCounter + 50, 0, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_WallCollision);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


extern "C" bool cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);
extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);
extern "C" void cSndPlayOpts_OPTS_Radius(void* self, float a, float b);

static inline int snd89A8AddVoice(void* vself, cRider* obj, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    cSndPlayOpts_OPTS_Radius(&(self->MusicSys.mpVStreamSys->mpVBankManager)->BankMonitor, 100.0f, -1.0f);
    pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    int listenerView = cSSXAudio_GetClosestCamera1(self, obj);
    (*pp)->BankMonitor.mViewport[(*pp)->BankMonitor.mInst] = listenerView;
    cBankManager* m2 = self->MusicSys.mpVStreamSys->mpVBankManager;
    m2->BankMonitor.mPosition[m2->BankMonitor.mInst] = &obj->mPosition;
    return ((cBankSys*)self)->SFX_End();
}

/**
 * Plays the wall-hit sound for a local human rider, unless one is already in the play history (event 6).
 *
 * @param obj  the rider
 */
void cSSXAudio::WallCollision(cRider* obj)
{
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (!ok) {
        return;
    }
    if (PlayHistoryExists(6, 0) != 0) {
        return;
    }
    int handle = snd89A8AddVoice(this, obj, 0, 54, 5);
    PlayHistoryAdd((void*)6, handle,
                             MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCounter + 50, 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_KnockdownSFX);
#ifdef SKIP_ASM
extern sSndKey8 D_004A3678[];
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" void cMonitor_MONITOR_Create(void*, int, long, void*, int, int, int, int);

/**
 * Plays the knockdown effect (patch 0x72) for a local human rider.
 *
 * @param obj  the rider
 */
// PORT: 64-bit long sound key
void cSSXAudio::KnockdownSFX(cRider* obj)
{
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        cBankMonitor* bankMon = &MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
        sSndKey8* key = D_004A3678;
        float* volumePtr = (float*)cSSXAudio_GetCurrAppliedVolumePtr(this, 9, 0);
        cMonitor_MONITOR_Create(bankMon, 250, key->v, volumePtr, 0, 114, 0x7F, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_TurboUseCheck);
#ifdef SKIP_ASM
/**
 * Returns 1: the monitor callback of the turbo sound.
 *
 * Its pointer to member is D_004A36A8: cSSXAudio_TurboOn and cSSXAudio_MaxPowerEvent destroy every monitor that uses it
 * (cMonitor_MONITOR_DestroyAll1), and cSSXAudio_MaxPowerEvent creates a 3-second one with it.
 *
 * @return 1
 */
extern "C" int cSSXAudio_TurboUseCheck(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_TurboOn);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
void cMonitor_MONITOR_DestroyAll1_8D90(void* p, long key) __asm__("cMonitor_MONITOR_DestroyAll1");

extern const sSndKey8 D_004A36A8_8D90[] __asm__("D_004A36A8");


static inline int snd8D90Add(void* vself, int a, int b)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = ((cBankManager*)self->mpVBankManager);
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = a;
    self->mPatch[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = b;
    cBankManager** pp = (&self->MusicSys.mpVStreamSys->mpVBankManager);
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 9, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

static inline void snd8D90Start(cSSXAudio* self, int a, int b)
{
    if (self->mTurboRef == 0) {
        cBankManager* m = ((cBankManager*)self->mpVBankManager);
        cBankMonitor* s = &m->BankMonitor;
        m->BankMonitor.mInst++;
        s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
        m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
        s->mPosStatic[m->BankMonitor.mInst] = 0;
        s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
        s->mVolControl[m->BankMonitor.mInst] = 0;
        s->mCallback[m->BankMonitor.mInst] = 0;
        s->mPtr[m->BankMonitor.mInst] = 0;
        s->mViewport[m->BankMonitor.mInst] = 0;
        s->mVanish[m->BankMonitor.mInst] = 0;
        s->mRadius[m->BankMonitor.mInst] = 0;
        s->mDelay[m->BankMonitor.mInst].mCount = 0;
        s->mDelay[m->BankMonitor.mInst].mTime = 100;
        s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
        s->mDelay[m->BankMonitor.mInst].mLevel = 50;
        s->mExternalVol[m->BankMonitor.mInst] = 0;
        s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
        s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
        s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
        s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
        self->mBank[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = a;
        self->mPatch[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = b;
        cBankManager** pp = (&self->MusicSys.mpVStreamSys->mpVBankManager);
        float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 9, 0);
        (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
        self->mTurboHandle = ((cBankSys*)self)->SFX_End();
    }
}

/**
 * Starts the turbo (boost) sound for a local human rider and counts the request.
 *
 * Does nothing while replaying or for a rider that is not a local human (mIsHuman and mIsLocal). Destroys every monitor
 * that uses the callback D_004A36A8 (cSSXAudio_TurboUseCheck) through cMonitor_MONITOR_DestroyAll1. When mTurboRef is 0 it starts bank 0, patch
 * 122 with the volume control cSSXAudio_GetCurrAppliedVolumePtr(this, 9, 0) and keeps its handle in mTurboHandle; when
 * @p flag is set it also plays the one-shot bank 0, patch 74 with the same control. mTurboRef then grows by 1
 * (cSSXAudio_TurboOff counts it down).
 *
 * @param vself  the audio system
 * @param obj  the rider (cRider)
 * @param flag  nonzero when the boost has just been switched on (adds the one-shot)
 */
// PORT: 64-bit long sound key
void cSSXAudio::TurboOn(cRider* obj, int flag)
{
    cSSXAudio* self = this;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (!ok) return;
    const sSndKey8* key = D_004A36A8_8D90;
    cMonitor_MONITOR_DestroyAll1_8D90(&(self->MusicSys.mpVStreamSys->mpVBankManager)->BankMonitor, key->v);
    snd8D90Start(self, 0, 122);
    if (flag != 0) {
        snd8D90Add(self, 0, 74);
    }
    self->mTurboRef = self->mTurboRef + 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_TurboOff);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Releases one turbo-sound request of a local human rider and stops the sound when none is left.
 *
 * Does nothing while replaying or for a rider that is not a local human. Decrements mTurboRef; when it reaches 0
 * mTurboHandle is stopped (cBankMonitor_BANKMONITOR_StopHandle, mode 1, fade 0), and a count at or below 0 is reset to
 * 0.
 *
 * @param obj  the rider (cRider)
 */
void cSSXAudio::TurboOff(cRider* obj)
{
    if (cSSXAudio_ReplayMode(this) != 0) {
        return;
    }
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        int n = --mTurboRef;
        if (n <= 0) {
            if (n == 0) {
                cBankMonitor_BANKMONITOR_StopHandle(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                    mTurboHandle, 1, 0.0f);
            }
            mTurboRef = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_TurboDeny);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


extern const sSndKey8 D_004A36A8_9368[] __asm__("D_004A36A8");
extern "C" void cMonitor_MONITOR_DestroyAll1(void* bank, long id);
extern "C" int cSSXAudio_ReplayMode(void* self);

static inline int IsReady299368(cRider* obj)
{
    return obj->mIsHuman != 0 && obj->mIsLocal != 0;
}

static inline void AddVoice299368(cSSXAudio* self, int a, int bb, int type)
{
    cBankManager* m = ((cBankManager*)self->mpVBankManager);
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = a;
    self->mPatch[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = bb;
    cBankManager** pp = ((cBankManager**)self->MusicSys.mpVStreamSys);
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
}

/**
 * Plays the turbo-denied sound (bank 9, patch 0x6C) for a rider, except during a replay.
 *
 * Only for a local human rider; calls cMonitor_MONITOR_DestroyAll1() with stream key D_004A36A8 first.
 *
 * @param obj  the rider
 */
// PORT: the 8-byte stream id is passed by value from an unaligned global (64-bit `long`).
void cSSXAudio::TurboDeny(cRider* obj)
{
    if (cSSXAudio_ReplayMode((this)) != 0) {
        return;
    }
    if (!IsReady299368(obj)) {
        return;
    }
    cMonitor_MONITOR_DestroyAll1(&(*(cBankManager**)MusicSys.mpVStreamSys)->BankMonitor, D_004A36A8_9368[0].v);
    AddVoice299368((this), 0, 108, 9);
    ((cBankSys*)(this))->SFX_End();
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_MaxPowerEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// 64-bit key views of two of the handlers: the key stays in a saved register across calls.
extern sSndKey8 D_004A36A8_key[] __asm__("D_004A36A8");
extern "C" void cMonitor_MONITOR_DestroyAll1_key(void* bank, long id) __asm__("cMonitor_MONITOR_DestroyAll1");
extern sSndKey8 D_004A3678_key[] __asm__("D_004A3678");
extern "C" int cMonitor_MONITOR_Create_key(void*, int, long, void*, int, int, int,
        int) __asm__("cMonitor_MONITOR_Create");
extern "C" void cSSXAudio_SpeakArcadeResult(cSSXAudio* self, cRider* obj, int flags);

/**
 * Reacts to a local human rider reaching maximum boost power: a delayed chime, a DJ line and a turbo-use monitor.
 *
 * Does nothing while replaying, for a rider that is not a local human, or once the rider is past the finish line
 * (mFinishTime >= 0). Creates a 400 ms monitor (cMonitor_MONITOR_Create) that plays bank 0, patch 107 at volume 127
 * through cBankSys_Play2DCallback (D_004A3678) with the volume control cSSXAudio_GetCurrAppliedVolumePtr(self, 9, 0).
 * Then raises the DJ event cSSXAudio_SpeakArcadeResult with 0x10 when mUberLevel is eUS_SuperUberMax, 8 when it is 5 or more, else 4.
 * Finally destroys the monitors of cSSXAudio_TurboUseCheck (D_004A36A8) and, when mTurboRef is 0, creates a 3000 ms one
 * with the rider as its pointer. The peak3 comment above it ("landing thud ... airborne-and-dropping") is wrong: +0x470
 * is mFinishTime.
 *
 * @param obj  the rider (cRider)
 */
// PORT: 64-bit long sound keys
void cSSXAudio::MaxPowerEvent(cRider* obj)
{
    if (cSSXAudio_ReplayMode(this) != 0) {
        return;
    }
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (!ok) {
        return;
    }
    bool up = 0.0f <= obj->mFinishTime;
    if (up) {
        return;
    }
    cBankMonitor* bankMonitor = &MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
    sSndKey8* key = D_004A3678_key;
    float* volControl = (float*)cSSXAudio_GetCurrAppliedVolumePtr(this, 9, 0);
    cMonitor_MONITOR_Create_key(bankMonitor, 400, key->v, volControl, 0, 107, 0x7F, 0);
    int uberLevel = obj->mUberLevel;
    if (uberLevel == eUS_SuperUberMax) {
        cSSXAudio_SpeakArcadeResult((cSSXAudio*)this, (cRider*)(((char*)obj)), 0x10);
    } else if (uberLevel >= 5) {
        cSSXAudio_SpeakArcadeResult((cSSXAudio*)this, (cRider*)(((char*)obj)), 0x8);
    } else {
        cSSXAudio_SpeakArcadeResult((cSSXAudio*)this, (cRider*)(((char*)obj)), 0x4);
    }
    long id = D_004A36A8_key[0].v;
    cMonitor_MONITOR_DestroyAll1_key(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, id);
    if (mTurboRef != 0) {
        return;
    }
    cMonitor_MONITOR_Create_key(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                3000, id, obj, 0, 0, 0, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_LoseMaxPowerEvent);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);

#define SND_97B8_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_97B8_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int snd97B8AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager* m2 = self->MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* s2 = &m2->BankMonitor;
    s2->mBaseVol[m2->BankMonitor.mInst] = 0x7F;
    s2->mCurrent[m2->BankMonitor.mInst].vol = SND_97B8_MAX(0, SND_97B8_MIN(s2->mBaseVol[s2->mInst], 0x7F));
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" cGame* cAudioInterface_getGame();
extern "C" int cReplay_isPauseAI(void* p);
extern int cPauseState_Flags;

/**
 * Plays the lose-max-power sound for a local human rider that is still racing, while the game-flow state kGFSM_Riding
 * runs.
 *
 * Skipped during a replay and while the sound is muted (cPauseState_Flags bit 0, or the replay check).
 *
 * @param obj  the rider
 */
void cSSXAudio::LoseMaxPowerEvent(cRider* obj)
{
    if (cSSXAudio_ReplayMode(this) != 0) {
        return;
    }
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (!ok) {
        return;
    }
    int playBeeps = 1;
    int pos = obj->mFinishTime >= 0.0f;
    if (pos) playBeeps = 0;
    if ((cPauseState_Flags & 1) || cReplay_isPauseAI(cAudioInterface_getGame()->Replay) != 0) playBeeps = 0;
    if (cAudioInterface_getGame()->mCurrentStateID != kGFSM_Riding) playBeeps = 0;
    if (playBeeps) {
        snd97B8AddVoice(this, 0, 105, 9);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_UberRailDeny);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int snd9B70AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

extern "C" int cSSXAudio_ReplayMode(void* self);

/**
 * Plays the uber-rail-denied sound (bank 9, patch 0x6C) for a local human rider, except during a replay.
 *
 * @param obj  the rider
 */
void cSSXAudio::UberRailDeny(cRider* obj)
{
    if (cSSXAudio_ReplayMode(this) != 0) {
        return;
    }
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        snd9B70AddVoice(this, 0, 108, 9);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_ChangePosition);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int snd9E28AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" bool cSSXAudio_IsRivalTimeEvent(cSSXAudio* self);
extern "C" cGame* cAudioInterface_getGame();
extern "C" void* cAudioInterface_getAI();
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern "C" bool cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);
extern "C" void cSSXAudio_SpeakOvertake(void* self, void* a, void* b);

/**
 * Reacts to a rider's race position changing: a sting for taking the lead, then a passing reaction.
 *
 * Only while the game-flow state kGFSM_Riding runs, with at least two players and computers, for a local human or the
 * listener's rider (outside game mode kGAMEMODE_RACE only when cSSXAudio_IsRivalTimeEvent() allows). Reaching first place plays bank
 * 9 patch 0x6D once (event 1). Then cSSXAudio_SpeakOvertake() gets (rider, passed rider) when moving up, or (passing rider,
 * rider).
 *
 * @param obj  the rider
 * @param pPrevPosRider  the rider now behind (On Tour pPrevPosRider), or 0
 * @param pNextPosRider  the rider now ahead (On Tour pNextPosRider), or 0
 * @param lim  old position
 * @param n  new position (0 is first)
 */
void cSSXAudio::ChangePosition(cRider* obj, cRider* pPrevPosRider, cRider* pNextPosRider, int lim, int n)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGameMode != 0 && cSSXAudio_IsRivalTimeEvent(this) == 0) {
        return;
    }
    if (cAudioInterface_getGame()->mCurrentStateID != kGFSM_Riding) {
        return;
    }
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (!ok && cSSXAudio_GetFocusRider_p(this, -1) != ((void*)obj)) {
        return;
    }
    int cnt = ((cAI*)cAudioInterface_getAI())->NumberOfPlayersAllocated;
    if (cnt + ((cAI*)cAudioInterface_getAI())->NumberOfComputersAllocated < 2) {
        return;
    }
    if (n == 0 && PlayHistoryExists(1, 0) == 0 && cSSXAudio_ReplayMode(this) == 0) {
        int handle = snd9E28AddVoice(this, 0, 109, 9);
        PlayHistoryAdd((void*)1, handle,
                                 MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCounter + 200, 0, 0);
    }
    if (n < lim) {
        if (((void*)pPrevPosRider) != 0) {
            cSSXAudio_SpeakOvertake(this, obj, pPrevPosRider);
        }
    } else {
        if (((void*)pNextPosRider) != 0) {
            cSSXAudio_SpeakOvertake(this, pNextPosRider, obj);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayerResetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int sndA220AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void cSSXAudio_WipeoutSlideEnd(cSSXAudio* self, cRider* obj);
extern "C" void cSSXAudio_WipeoutTumbleEnd(cSSXAudio* self, cRider* obj);
extern "C" void cSSXAudio_TurboOff(cSSXAudio* self, cRider* obj);
extern "C" void func_0028F108(void* self, void* rider);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");

/**
 * Resets a rider's sounds when the rider is put back on the course.
 *
 * For a local human when not replaying, plays bank 0, patch 123 with the volume control cSSXAudio_GetCurrAppliedVolumePtr(self, 5, 0). Then
 * stops the wipeout slide and tumble sounds (cSSXAudio_WipeoutSlideEnd, cSSXAudio_WipeoutTumbleEnd), releases the turbo sound (cSSXAudio_TurboOff)
 * while mTurboRef is above 0, and calls func_0028F108 when the rider is the focus rider (cSSXAudio_GetFocusRider(self,
 * -1)).
 *
 * @param obj  the rider (cRider)
 */
void cSSXAudio::PlayerResetEvent(cRider* obj)
{
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok && cSSXAudio_ReplayMode(this) == 0) {
        sndA220AddVoice(this, 0, 123, 5);
    }
    WipeoutSlideEnd(obj);
    WipeoutTumbleEnd(obj);
    if (mTurboRef > 0) {
        this->TurboOff(obj);
    }
    if (((void*)obj) == cSSXAudio_GetFocusRider_p(this, -1)) {
        func_0028F108(this, obj);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_Tweak);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int sndA530AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

/**
 * Plays the tweak sound (bank 9, patch 0x77) for a local human rider.
 *
 * @param obj  the rider
 */
void cSSXAudio::Tweak(cRider* obj)
{
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        sndA530AddVoice(this, 0, 119, 9);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_PointsEvent);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int sndA7D8AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void SNDSYS_entercritical();
extern "C" int SNDover(int id);
extern "C" void SNDSYS_leavecritical();
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Plays the points sound when a local human's score crosses a multiple of 100, except during a replay.
 *
 * The sound plays when points % 100 drops below the last value seen, stopping the previous points sound
 * first.
 *
 * @param obj  the rider
 * @param n  the rider's points
 */
void cSSXAudio::PointsEvent(cRider* obj, int n)
{
    if (cSSXAudio_ReplayMode(this) != 0) {
        return;
    }
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        int curPoints = n % 100;
        if (curPoints < mPointsLastValue) {
            SNDSYS_entercritical();
            int soundFinished = SNDover(mPointsEventHandle) != 0;
            SNDSYS_leavecritical();
            if (!soundFinished) {
                cBankMonitor_BANKMONITOR_StopHandle(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                    mPointsEventHandle, 1, 0.0f);
            }
            mPointsEventHandle = sndA7D8AddVoice(this, 0, 77, 9);
        }
        mPointsLastValue = curPoints;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_BoostBar);
#ifdef SKIP_ASM
/**
 * Resets the boost-bar tick pitch of a local human rider's view to its base, 0x1000.
 *
 * For a local human (mIsHuman and mIsLocal) sets mBoostBarPitchMult[mViewIndex] to 0x1000; cSSXAudio_UpdateBoostBar raises it by
 * 0x100 for every tick it plays while the bar fills. Its caller (cComputer, ai/computer.cpp) passes the bar's previous
 * value and the amount added as two more arguments, which the code does not use.
 *
 * @param obj  the rider (cRider)
 */
void cSSXAudio::BoostBar(cRider* obj)
{
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        mBoostBarPitchMult[obj->mViewIndex] = 0x1000;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UpdateBoostBar);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" cGame* cAudioInterface_getGame();
extern "C" void* cAudioInterface_getAI();
extern "C" int cReplay_isPauseAI(void* p);
// PORT: cMonitor_MONITOR_Create returns int; the unit declares it void elsewhere.
extern "C" int cMonitor_MONITOR_Create_AB40(void*, int, long, void*, int, int, int,
        int) __asm__("cMonitor_MONITOR_Create");
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern int cPauseState_Flags;

extern const sSndKey8 D_004A36C0_AB40[] __asm__("D_004A36C0");

#define SND_AB40_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_AB40_MAX(a, b) ((a) > (b) ? (a) : (b))

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_AB40(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

// Views of the rider's HUD effects: cRiderMetrix::HUDfx (0x1B0) points at its cHUDFX array (0x9C each), and the
// boost bar reads entry 6 (+0x3A8). MaxDecay / CurDecay are cHUDFX's [O] names at the same offsets (ai/hudfx.h).
struct sMon_AB40 {
    char pad0[4];
    float MaxDecay;     // 0x4
    float CurDecay;     // 0x8
};
struct sMon1B0_AB40 {
    char pad[0x3A8];
    sMon_AB40 m;
};
struct sMon790_AB40 {
    char pad[0x1B0];
    sMon1B0_AB40* HUDfx;  // 0x1B0
};


static inline int sndAB40Add(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = ((cBankManager*)self->mpVBankManager);
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = a;
    self->mPatch[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = b;
    cBankManager* m2 = self->MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* s2 = &m2->BankMonitor;
    s2->mBaseVol[m2->BankMonitor.mInst] = 0x7F;
    s2->mCurrent[m2->BankMonitor.mInst].vol = SND_AB40_MAX(0, SND_AB40_MIN(s2->mBaseVol[s2->mInst], 0x7F));
    cBankManager** pp = (&self->MusicSys.mpVStreamSys->mpVBankManager);
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

/**
 * Plays the boost-bar sounds of each local view: a rising tick as the bar fills and a countdown beep as uber runs out.
 *
 * Does nothing while replaying. Handles riders 0..1 in the game path kGAMEPATH_MULTIPLAYER (newRace.cGamePath), else
 * rider 0; riders that are not local humans are skipped. The bar fill in tenths is floor(10 * CurDecay / MaxDecay) of
 * the rider's metrix HUD fx (mMetrix + 0x1B0, + 0x3A8); when it rose above mBoostBarPreviousValue[i] (and is above 0) a
 * 150 ms monitor (cMonitor_MONITOR_Create, callback cBankSys_Play2DCallbackWithPitchMult through D_004A36C0) plays bank
 * 0, patch 101 at volume 127 with the volume control cSSXAudio_GetCurrAppliedVolumePtr(self, 9, 0) and pitch
 * mBoostBarPitchMult[mViewIndex], which then grows by 0x100. The half-seconds left of uber, floor(mUberTimer * 2), are
 * kept in mUberSecPreviousValue[i]; when they drop by one to a value below 13 that is even or below 5, and the rider is
 * not past the finish line, the game is not paused (cPauseState_Flags bit 0, cReplay_isPauseAI) and the game state is
 * kGFSM_Riding, bank 0, patch 104 plays at volume 127.
 *
 * @param vself  the audio system
 */
// PORT: 64-bit long sound key; PS2-only FPU asm
void cSSXAudio::UpdateBoostBar()
{
    cSSXAudio* self = this;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int n = 1;
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) n = 2;
    for (int i = 0; i < n; i++) {
        cRider* viewRider = ((cAI*)cAudioInterface_getAI())->Riders[i];
        bool ok = viewRider->mIsHuman && viewRider->mIsLocal;
        if (!ok) continue;
        cRider* rider = ((cAI*)cAudioInterface_getAI())->Riders[i];
        sMon1B0_AB40* mon = ((sMon790_AB40*)rider->mMetrix)->HUDfx;
        sMon_AB40* mp = mon ? &mon->m : 0;
        int barTenths = (int)ffloor_AB40(mp->CurDecay / mp->MaxDecay * 10.0f);
        if (self->mBoostBarPreviousValue[i] < barTenths && barTenths > 0) {
            void* sub = &(self->MusicSys.mpVStreamSys->mpVBankManager)->BankMonitor;
            float* volControl = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 9, 0);
            cMonitor_MONITOR_Create_AB40(sub, 150, D_004A36C0_AB40[0].v, volControl, 0, 101, 0x7F, self->mBoostBarPitchMult[rider->mViewIndex]);
            self->mBoostBarPitchMult[rider->mViewIndex] += 0x100;
        }
        self->mBoostBarPreviousValue[i] = barTenths;
        int w = (int)ffloor_AB40(rider->mUberTimer * 2.0f);
        if (w > 0) {
            if (w == self->mUberSecPreviousValue[i] - 1) {
                if (w >= 13) {
                } else if ((w & 1) && w >= 5) {
                } else {
                    int playBeep = 1;
                    int pos = rider->mFinishTime >= 0.0f;
                    if (pos) playBeep = 0;
                    if ((cPauseState_Flags & 1) || cReplay_isPauseAI(cAudioInterface_getGame()->Replay) != 0) playBeep = 0;
                    if (cAudioInterface_getGame()->mCurrentStateID != kGFSM_Riding) playBeep = 0;
                    if (playBeep) {
                        sndAB40Add(self, 0, 104, 9);
                    }
                }
            }
        }
        self->mUberSecPreviousValue[i] = w;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_StartUberSpin);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int sndB0E0AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Starts the uber-spin sound for a local human rider, replacing one that is playing (mUberSpinHandle not
 * kNoUberSpinHandle); not during a replay.
 *
 * @param obj  the rider
 */
void cSSXAudio::StartUberSpin(cRider* obj)
{
    if (cSSXAudio_ReplayMode(this) != 0) {
        return;
    }
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        if (mUberSpinHandle != kNoUberSpinHandle) {
            cBankMonitor_BANKMONITOR_StopHandle(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                mUberSpinHandle, 1, 0.0f);
        }
        mUberSpinHandle = sndB0E0AddVoice(this, 0, 102, 9);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_EndUberSpin);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Stops the uber-spin sound, for a local human rider or when @p obj is 0.
 *
 * @param obj  the rider, or 0
 */
void cSSXAudio::EndUberSpin(cRider* obj)
{
    if (((void*)obj) != 0) {
        int ok = 0;
        if (obj->mIsHuman != 0) {
            ok = obj->mIsLocal != 0;
        }
        if (!ok) {
            return;
        }
    }
    if (mUberSpinHandle != kNoUberSpinHandle) {
        cBankMonitor_BANKMONITOR_StopHandle(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                            mUberSpinHandle, 1, 0.0f);
        mUberSpinHandle = -1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_LandedUberTrick);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int sndB430AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void cSSXAudio_SpeakArcadeUberMilestone(cSSXAudio* self, cRider* obj, int ev);

/**
 * Plays the landed-uber-trick sound for a local human rider, except during a replay.
 *
 * When the count goes from below 4 to 4-8, also calls cSSXAudio_SpeakArcadeUberMilestone(rider, 1).
 *
 * @param obj  the rider
 * @param iNum  number of uber tricks just landed (On Tour iNum)
 * @param iPrevious  number landed before (On Tour iPrevious)
 */
void cSSXAudio::LandedUberTrick(cRider* obj, int iNum, int iPrevious)
{
    if (cSSXAudio_ReplayMode(this) != 0) {
        return;
    }
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        sndB430AddVoice(this, 0, 103, 9);
        int total = iPrevious + iNum;
        if (iPrevious < 4) {
            if (total >= 4) {
                if (total < 9) {
                    cSSXAudio_SpeakArcadeUberMilestone(this, obj, 1);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UpdateLandedUber);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_ReplayMode(void* self);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern "C" cGame* cAudioInterface_getGame();
extern "C" void cSSXAudio_SpeakArcadeUberMilestone(cSSXAudio* self, cRider* obj, int ev);

/**
 * Raises the DJ event cSSXAudio_SpeakArcadeUberMilestone when the focus rider's uber level reaches eUS_SuperUberMax.
 *
 * Does nothing while replaying. Takes the focus rider (cSSXAudio_GetFocusRider(self, -1)); when its mViewIndex differs
 * from the one remembered in mUberViewIndex it only records the new view. Otherwise, when the rider's mUberLevel is
 * eUS_SuperUberMax, differs from the level remembered in mLastUberLevel and the game state is kGFSM_Riding, it calls
 * cSSXAudio_SpeakArcadeUberMilestone(self, rider, 2). The level is remembered in mLastUberLevel every time.
 */
void cSSXAudio::UpdateLandedUber()
{
    if (cSSXAudio_ReplayMode(this) != 0) return;
    // PORT: cSSXAudio_GetFocusRider returns a rider pointer as int
    cRider* localRider = cSSXAudio_GetFocusRider_p(this, -1);
    if (localRider->mViewIndex == mUberViewIndex) {
        if (localRider->mUberLevel == eUS_SuperUberMax && mLastUberLevel != localRider->mUberLevel &&
            cAudioInterface_getGame()->mCurrentStateID == kGFSM_Riding) {
            cSSXAudio_SpeakArcadeUberMilestone((cSSXAudio*)this, (cRider*)localRider, 2);
        }
    } else {
        mUberViewIndex = localRider->mViewIndex;
    }
    mLastUberLevel = localRider->mUberLevel;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_LandedMonsterCombo);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_SpeakArcadeUberMilestone(cSSXAudio* self, cRider* obj, int ev);

/**
 * Reports a landed monster combo by a local human rider: cSSXAudio_SpeakArcadeUberMilestone(rider, 8).
 *
 * @param obj  the rider
 */
void cSSXAudio::LandedMonsterCombo(cRider* obj)
{
    int ok = 0;
    if (obj->mIsHuman != 0) {
        ok = obj->mIsLocal != 0;
    }
    if (ok) {
        cSSXAudio_SpeakArcadeUberMilestone(this, obj, 8);
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029B818);

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SwapBankStopped__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: called with a world trigger's swap-bank name when the trigger stops.
 *
 * cWorldTriggerManager_Update (the world-trigger update) calls it for each stopped trigger whose name (+0x40) is not
 * str_empty_j; func_0029B818 handles the same name when a trigger starts.
 */
void cSSXAudio::SwapBankStopped()
{
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_BindingsOn);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);

#define SND_B968_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_B968_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int sndB968AddVoice(void* vself, cRider* obj, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager* m2 = self->MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* s2 = &m2->BankMonitor;
    s2->mBaseVol[m2->BankMonitor.mInst] = 0x7F;
    s2->mCurrent[m2->BankMonitor.mInst].vol = SND_B968_MAX(0, SND_B968_MIN(s2->mBaseVol[s2->mInst], 0x7F));
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    cBankManager* m3 = self->MusicSys.mpVStreamSys->mpVBankManager;
    m3->BankMonitor.mPosition[m3->BankMonitor.mInst] = &obj->mPosition;
    pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    int listenerView = cSSXAudio_GetClosestCamera1(self, obj);
    (*pp)->BankMonitor.mViewport[(*pp)->BankMonitor.mInst] = listenerView;
    return ((cBankSys*)self)->SFX_End();
}

/**
 * Plays the bindings-on sound (bank 5, patch 0x50) at a local human rider.
 *
 * @param obj  the rider
 */
void cSSXAudio::BindingsOn(cRider* obj)
{
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        sndB968AddVoice(this, obj, 0, 80, 5);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_BindingsOff);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);

#define SND_BCF8_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_BCF8_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int sndBCF8AddVoice(void* vself, cRider* obj, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager* m2 = self->MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* s2 = &m2->BankMonitor;
    s2->mBaseVol[m2->BankMonitor.mInst] = 0x7F;
    s2->mCurrent[m2->BankMonitor.mInst].vol = SND_BCF8_MAX(0, SND_BCF8_MIN(s2->mBaseVol[s2->mInst], 0x7F));
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    cBankManager* m3 = self->MusicSys.mpVStreamSys->mpVBankManager;
    m3->BankMonitor.mPosition[m3->BankMonitor.mInst] = &obj->mPosition;
    pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    int listenerView = cSSXAudio_GetClosestCamera1(self, obj);
    (*pp)->BankMonitor.mViewport[(*pp)->BankMonitor.mInst] = listenerView;
    return ((cBankSys*)self)->SFX_End();
}

/**
 * Plays the bindings-off sound (bank 5, patch 0x51) at a local human rider.
 *
 * @param obj  the rider
 */
void cSSXAudio::BindingsOff(cRider* obj)
{
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        sndBCF8AddVoice(this, obj, 0, 81, 5);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_UndoBindings);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);

#define SND_C088_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_C088_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int sndC088AddVoice(void* vself, cRider* obj, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager* m2 = self->MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* s2 = &m2->BankMonitor;
    s2->mBaseVol[m2->BankMonitor.mInst] = 0x7F;
    s2->mCurrent[m2->BankMonitor.mInst].vol = SND_C088_MAX(0, SND_C088_MIN(s2->mBaseVol[s2->mInst], 0x7F));
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    cBankManager* m3 = self->MusicSys.mpVStreamSys->mpVBankManager;
    m3->BankMonitor.mPosition[m3->BankMonitor.mInst] = &obj->mPosition;
    pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    int listenerView = cSSXAudio_GetClosestCamera1(self, obj);
    (*pp)->BankMonitor.mViewport[(*pp)->BankMonitor.mInst] = listenerView;
    return ((cBankSys*)self)->SFX_End();
}

/**
 * Plays the undo-bindings sound (bank 5, patch 0x52) at a local human rider.
 *
 * @param obj  the rider
 */
void cSSXAudio::UndoBindings(cRider* obj)
{
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        sndC088AddVoice(this, obj, 0, 82, 5);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_CountdownStart__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: called when the start-gate countdown begins.
 *
 * cGFGateState_gainFocus calls it right before cSSXAudio_CountdownBeep(audio, 3), the first countdown beep.
 */
void cSSXAudio::CountdownStart()
{
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_CountdownBeep);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int sndC420AddVoice(void* vself, char* mon, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = *(cBankManager***)mon;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cMusicSys_GetSongTag(void*);
extern "C" void cMusicSys_FadeOut(void*, float);
extern "C" void cSSXAudio_PickRandomSong(void* self);
extern "C" void cSSXAudio_StartLocationMusic(void* self, int a1, int a2, int a3, int a4);
extern "C" void cMusicSys_PauseSong(void*);

/**
 * Plays a start-gate countdown beep and, on the first and last beeps, prepares the race music.
 *
 * Does nothing while replaying. Plays bank 0, patch 78 with the volume control cSSXAudio_GetCurrAppliedVolumePtr(self,
 * 5, 0). On beep 3, when the current song's tag (cMusicSys_GetSongTag) is kSONGTAG_CharTune or kSONGTAG_Peak1 .. 3 and
 * mRaceMusicOn is set, fades it over 2 seconds (cMusicSys_FadeOut). On beep 1, when no song is playing
 * (MusicSys.mSongPlaying) or the state is one of those, and mRaceMusicOn is set, it calls cSSXAudio_PickRandomSong,
 * cSSXAudio_StartLocationMusic(self, 0, 0, kPEAKBIT_FromLocation, 0) and cMusicSys_PauseSong on the music system.
 *
 * @param mode  the countdown second (3, 2, 1; cGFGateState passes 3 first, then each whole second left)
 */
void cSSXAudio::CountdownBeep(int mode)
{
    if (cSSXAudio_ReplayMode(this) != 0) {
        return;
    }
    char* mon = (char*)&MusicSys;
    sndC420AddVoice(this, mon, 0, 78, 5);
    if (mode == 3) {
        int st = cMusicSys_GetSongTag((char*)&MusicSys);
        if (st == kSONGTAG_CharTune || st == kSONGTAG_Peak1 || st == kSONGTAG_Peak2 || st == kSONGTAG_Peak3) {
            if (mRaceMusicOn != 0) {
                cMusicSys_FadeOut((char*)&MusicSys, 2.0f);
            }
        }
    }
    if (mode == 1) {
        int st = cMusicSys_GetSongTag((char*)&MusicSys);
        if (MusicSys.mSongPlaying == 0 || st == kSONGTAG_CharTune || st == kSONGTAG_Peak1 || st == kSONGTAG_Peak2
            || st == kSONGTAG_Peak3) {
            if (mRaceMusicOn != 0) {
                cSSXAudio_PickRandomSong(this);
                cSSXAudio_StartLocationMusic(this, 0, 0, kPEAKBIT_FromLocation, 0);
                cMusicSys_PauseSong((char*)&MusicSys);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_CountdownGateDrop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" cGame* cAudioInterface_getGame();
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" bool cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
void cMusicSys_ResumeSong_C7B0(void* p) __asm__("cMusicSys_ResumeSong");
void cSSXAudio_ResetLastMusicState_C7B0(void* p) __asm__("cSSXAudio_ResetLastMusicState__FPv");
void cSpeechSys_StartNextSpeech(void* self, int a1);
int cMusicSys_GetCurrentSongSedValue(void* self);
extern "C" void cSSXAudio_StartDJIntro(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakPALine8387(cSSXAudio* self);
extern "C" bool cSSXAudio_IsRaceEvent(void* self);
extern "C" int cSSXAudio_IsSlopestyleEvent(cSSXAudio* self);
extern "C" void cSSXAudio_PlayHistoryUpdateTime(cSSXAudio* self, int a, int b, int c);

struct sSs7B0PreC { char pad[0xC]; };
class sSs7B0Self : public sSs7B0PreC {
public:
    virtual void SFX_Play2D(int bank, int patch); // slot 1: cSSXAudio_SFX_Play2D
};
struct sSs7B0Pre118 { char pad[0x5440]; };
class sSs7B0Obj : public sSs7B0Pre118 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void SetMusicState(int state); // slot 4: cSSXAudio's override cSSXAudio_SetMusicState sets the music state
};

static inline int snd7B0Add(void* vself, int a, int b)
{
    cSSXAudio* self = (cSSXAudio*)vself;
        cBankManager* m = ((cBankManager*)self->mpVBankManager);
        cBankMonitor* s = &m->BankMonitor;
        m->BankMonitor.mInst++;
        s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
        m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
        s->mPosStatic[m->BankMonitor.mInst] = 0;
        s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
        s->mVolControl[m->BankMonitor.mInst] = 0;
        s->mCallback[m->BankMonitor.mInst] = 0;
        s->mPtr[m->BankMonitor.mInst] = 0;
        s->mViewport[m->BankMonitor.mInst] = 0;
        s->mVanish[m->BankMonitor.mInst] = 0;
        s->mRadius[m->BankMonitor.mInst] = 0;
        s->mDelay[m->BankMonitor.mInst].mCount = 0;
        s->mDelay[m->BankMonitor.mInst].mTime = 100;
        s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
        s->mDelay[m->BankMonitor.mInst].mLevel = 50;
        s->mExternalVol[m->BankMonitor.mInst] = 0;
        s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
        s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
        s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
        s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
        self->mBank[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = a;
        self->mPatch[((cBankManager*)self->mpVBankManager)->BankMonitor.mInst] = b;
        cBankManager* m3 = self->MusicSys.mpVStreamSys->mpVBankManager;
        cBankMonitor* s3 = &m3->BankMonitor;
        s3->mBaseVol[m3->BankMonitor.mInst] = 0x7F;
        s3->mCurrent[m3->BankMonitor.mInst].vol = (s3->mBaseVol[s3->mInst] < 0x7F && s3->mBaseVol[s3->mInst] < 0) ? 0
                : (s3->mBaseVol[s3->mInst] < 0x7F ? (unsigned char)s3->mBaseVol[s3->mInst] : 0x7F);
        cBankManager** pp = (&self->MusicSys.mpVStreamSys->mpVBankManager);
        float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 9, 0);
        (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
        return ((cBankSys*)self)->SFX_End();
}

/**
 * Plays the start-gate drop and starts the race's music and opening DJ line.
 *
 * Calls cMusicSys_ResumeSong on the music system and cSSXAudio_ResetLastMusicState (mLastMusicState = -1), stores the cAI's AITick in
 * mGruntCycle and plays bank 0, patch 98 through this object's vtable slot 1; when not replaying also bank 0, patch 94
 * at volume 127 with the volume control cSSXAudio_GetCurrAppliedVolumePtr(vself, 9, 0). With mRaceMusicOn set the music
 * system's vtable slot 4 gets 0. Unless play-history event 0 is live: in the game paths kGAMEPATH_SINGLEEVENT and
 * kGAMEPATH_MULTIPLAYER (newRace.cGamePath) cSSXAudio_StartDJIntro; otherwise on the last heat (cGameModeMan bIsLastHeat)
 * cSSXAudio_SpeakPALine8387, cSpeechSys_StartNextSpeech(SpeechSys, 0) and, when cSSXAudio_IsRaceEvent or cSSXAudio_IsSlopestyleEvent is nonzero, mSongAnnouncePending
 * = 1 and mSongAnnounceNum = the current song's value from cMusicSys_GetCurrentSongSedValue; in heat kHeat1 the latter unconditionally, in
 * kHeat2 under the same test. When mRaceMusicOn is 0 it is set to 1 and music slot 4 gets 37 (not replaying), or slot 4
 * gets 37 when replaying with mReplaySongPlaying set. Finally play-history event 0 is moved to 50 ms past the monitor
 * clock and mGateDropHeat records iHeatNum.
 *
 */
void cSSXAudio::CountdownGateDrop()
{
    cSSXAudio* self = this;
    cSSXAudio* base = (cSSXAudio*)this;
    cMusicSys_ResumeSong_C7B0(&base->MusicSys);
    cSSXAudio_ResetLastMusicState_C7B0(this);
    base->mGruntCycle = cAudioInterface_getGame()->AI->AITick;
    ((sSs7B0Self*)this)->SFX_Play2D(0, 98);
    if (cSSXAudio_ReplayMode(this) == 0) {
        snd7B0Add(this, 0, 94);
    }
    if (base->mRaceMusicOn != 0) {
        ((sSs7B0Obj*)(&base->MusicSys))->SetMusicState(0);
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (PlayHistoryExists(0, 0) == 0) {
        int gamePath = cBENewRaceInterface_newRace.cGamePath;
        if (gamePath == kGAMEPATH_SINGLEEVENT) {
            cSSXAudio_StartDJIntro((cSSXAudio*)this);
        } else if (gamePath == kGAMEPATH_MULTIPLAYER) {
            cSSXAudio_StartDJIntro((cSSXAudio*)this);
        } else {
            int* gameModeMan = (int*)SSXApp->GameModeMan;
            if (gameModeMan[152 / 4] != 0) {
                cSSXAudio_SpeakPALine8387((cSSXAudio*)this);
                cSpeechSys_StartNextSpeech(&base->SpeechSys, 0);
                if (cSSXAudio_IsRaceEvent(this) != 0 || cSSXAudio_IsSlopestyleEvent(this) != 0) {
                    base->mSongAnnouncePending = 1;
                    base->mSongAnnounceNum = cMusicSys_GetCurrentSongSedValue(&base->MusicSys);
                }
            } else {
                int heatNum = gameModeMan[0];
                if (heatNum == cGameModeMan::kHeat1) {
                    base->mSongAnnouncePending = 1;
                    base->mSongAnnounceNum = cMusicSys_GetCurrentSongSedValue(&base->MusicSys);
                } else if (heatNum == cGameModeMan::kHeat2) {
                    if (cSSXAudio_IsRaceEvent(this) != 0 || cSSXAudio_IsSlopestyleEvent(this) != 0) {
                        base->mSongAnnouncePending = 1;
                        base->mSongAnnounceNum = cMusicSys_GetCurrentSongSedValue(&base->MusicSys);
                    }
                }
            }
        }
    }
    if (base->mRaceMusicOn == 0) {
        if (cSSXAudio_ReplayMode(this) == 0) {
            base->mRaceMusicOn = 1;
            ((sSs7B0Obj*)(&base->MusicSys))->SetMusicState(37);
        } else if (base->mReplaySongPlaying != 0) {
            ((sSs7B0Obj*)(&base->MusicSys))->SetMusicState(37);
        }
    }
    PlayHistoryUpdateTime(0,
                                    base->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCounter + 50, 0);
    base->mGateDropHeat = *(int*)SSXApp->GameModeMan;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UpdateTimedEvent);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void* cAudioInterface_getAI();
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" void cMonitor_MONITOR_Create(void*, int, long, void*, int, int, int, int);
// PORT: fptoui is libgcc's float -> unsigned conversion (__fixunssfsi).
extern "C" int fptoui(float f);
extern int D_005308D0[];
extern sSndKey8 D_004A3678[];

/**
 * Plays the countdown beep near the end of a timed event.
 *
 * Only when the game mode is timed (GameModeMan + 0x88) and D_005308D0 bit 9 is clear, never during a replay.
 * Beeps once at 10, 8 and 6 seconds left and every second from 4 down to 1.
 */
// PORT: 64-bit long sound key
void cSSXAudio::UpdateTimedEvent()
{
    if (cSSXAudio_ReplayMode(((char*)this)) != 0) {
        return;
    }
    cGameModeMan* gameModeMan = SSXApp->GameModeMan;
    if (((char*)gameModeMan) == 0) {
        return;
    }
    int ok = 0;
    if (gameModeMan->bIsTimedEvent != 0) {
        ok = ((D_005308D0[0] >> 9) & 1) ^ 1;
    }
    if (!ok) {
        return;
    }
    int elapsedSec = (int)((float)((cAI*)cAudioInterface_getAI())->EventTimerTick * 0.01666666753590107f);
    int secsLeft = (fptoui((float)gameModeMan->uiTimeLimit * 0.01666666753590107f) - elapsedSec) % 3600;
    if (secsLeft <= 0) {
        return;
    }
    if (secsLeft != mLastTime && secsLeft < 11 && (secsLeft == 10 || secsLeft == 8 || secsLeft == 6 || secsLeft < 5)) {
        cBankMonitor* bankMon = &MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
        sSndKey8* key = D_004A3678;
        float* volumePtr = (float*)cSSXAudio_GetCurrAppliedVolumePtr(((char*)this), 9, 0);
        cMonitor_MONITOR_Create(bankMon, 450, key->v, volumePtr, 0, 78, 0x7F, 0);
    }
    mLastTime = secsLeft;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_PauseSFX);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_Pause(void*);

/**
 * Pauses all sound effects in the bank monitor, once.
 */
void cSSXAudio::PauseSFX()
{
    if (mSFXPaused == 0) {
        mSFXPaused = 1;
        MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mPaused = 1;
        cBankMonitor_BANKMONITOR_Pause(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_ResumeSFX);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_Resume(void* self);
extern "C" void cBankMonitor_BANKMONITOR_CameraChange(void* self);
extern "C" void cWorldTriggerManager_TouchAllUsed(void* self);

/**
 * Resumes the paused sound effects and lets the bank monitor and world triggers catch up with the camera.
 */
void cSSXAudio::ResumeSFX()
{
    if (mSFXPaused != 0) {
        mSFXPaused = 0;
        cBankMonitor_BANKMONITOR_Resume(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor);
        MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mPaused = 0;
        cBankMonitor_BANKMONITOR_CameraChange(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor);
        cWorldTriggerManager_TouchAllUsed(gWorldTriggerManager);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayIconSound);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int sndCED8AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

extern "C" void cSSXAudio_SpeakArcadeMultiplier(cSSXAudio* self, cRider* obj, int n);

/**
 * Plays the sound for a collected icon, and reports multiplier pickups.
 *
 * Icon types keIcon_SpeedBoost, keIcon_TrickBoost, keIcon_TrickMultiplier and keIcon_Points play patches 0x70, 0x71,
 * 0x75 and 0x76 (others 0x74). For keIcon_TrickMultiplier a multiplier of 2, 3, 5 or 10 also calls cSSXAudio_SpeakArcadeMultiplier() with
 * 1, 2, 4 or 8.
 *
 * @param kind  icon type (On Tour eIconType)
 * @param obj  the rider, or 0; a rider that is not a local human plays nothing
 * @param x  the icon's multiplier
 */
void cSSXAudio::PlayIconSound(int kind, cRider* obj, float x)
{
    if (((void*)obj) != 0) {
        bool ok = obj->mIsHuman && obj->mIsLocal;
        if (!ok) {
            return;
        }
    }
    int id = 116;
    if (kind == keIcon_SpeedBoost) {
        id = 0x70;
    } else if (kind == keIcon_TrickBoost) {
        id = 113;
    } else if (kind == keIcon_TrickMultiplier) {
        id = 117;
    } else if (kind == keIcon_Points) {
        id = 118;
    }
    sndCED8AddVoice(this, 0, id, 9);
    if (kind == keIcon_TrickMultiplier) {
        if (x == 2.0f) {
            cSSXAudio_SpeakArcadeMultiplier(this, obj, 1);
        } else if (x == 3.0f) {
            cSSXAudio_SpeakArcadeMultiplier(this, obj, 2);
        } else if (x == 5.0f) {
            cSSXAudio_SpeakArcadeMultiplier(this, obj, 4);
        } else if (x == 10.0f) {
            cSSXAudio_SpeakArcadeMultiplier(this, obj, 8);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UpdateTrackBanks);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getAI();
extern "C" void cSSXAudio_StartTone(cSSXAudio* self);
extern "C" void cSSXAudio_StopTone(cSSXAudio* self, float v);
extern "C" void cBankMonitor_BANKMONITOR_StopBank(void* self, int id, float v);

// On Tour cBankManager::BANK_Used(int): the bank slot holds a loaded bank.
static inline int sndIsOn(cBankInstance* bank)
{
    return bank->mBankStatus == USED;
}

/**
 * Follows the first rider's ground and switches the track sound banks when it changes.
 *
 * When the low byte of its mGroundPolyId differs from mLastPatchRID, a ground material below 22 calls
 * cSSXAudio_StartTone(); other materials stop the tone and both track banks with a 5-second fade.
 */
void cSSXAudio::UpdateTrackBanks()
{
    cRider* rider = ((cAI*)cAudioInterface_getAI())->Riders[0];
    int patchRID = (int)rider->mGroundPolyId;
    int ok = patchRID != -1;
    if (ok) {
        if ((patchRID & 0xFF) != (mLastPatchRID & 0xFF)) {
            mLastPatchRID = (unsigned int)patchRID;
            if (rider->mGroundMaterial < 22) {
                StartTone();
            } else {
                StopTone(5.029983997344971f);

                cBankManager* bankMgr = MusicSys.mpVStreamSys->mpVBankManager;
                cBankMonitor* bm = &bankMgr->BankMonitor;
                cBankInstance* banks = bankMgr->mBank;
                int id = sndIsOn(&banks[8]);
                if (id) id = banks[8].mBankHandle; else id = -1;
                cBankMonitor_BANKMONITOR_StopBank(bm, id, 5.029983997344971f);

                cBankManager* bankMgr2 = MusicSys.mpVStreamSys->mpVBankManager;
                cBankMonitor* bm2 = &bankMgr2->BankMonitor;
                cBankInstance* banks2 = bankMgr2->mBank;
                int id2 = sndIsOn(&banks2[9]);
                if (id2) id2 = banks2[9].mBankHandle; else id2 = -1;
                cBankMonitor_BANKMONITOR_StopBank(bm2, id2, 5.029983997344971f);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_StartTone);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int sndD370AddVoice(void* vself, int a, int b)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 5, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

/**
 * Starts the track tone: plays bank 9, patch 0 when the track bank is loaded, and marks the tone as playing.
 *
 * When mTrackBankLoaded[1] is set the sound starts with the volume control cSSXAudio_GetCurrAppliedVolumePtr(self, 5, 0) and its handle
 * goes to mToneHandle. mPlayingTone is set to 1 either way. cSSXAudio_UpdateTrackBanks and cSSXAudio_UpdateTone call it
 * (the latter once the previous tone voice has finished).
 */
void cSSXAudio::StartTone()
{
    if (mTrackBankLoaded[1] != 0) {
        mToneHandle = sndD370AddVoice(this, 9, 0);
    }
    mPlayingTone = 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UpdateTone);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int SNDover(int h);
extern "C" void cSSXAudio_StopTone(cSSXAudio* self, float v);
extern "C" void cSSXAudio_StartTone(cSSXAudio* self);

/**
 * Restarts the track tone (cSSXAudio_StartTone) once the playing tone voice has finished.
 */
void cSSXAudio::UpdateTone()
{
    if (mPlayingTone != 0) {
        SNDSYS_entercritical();
        int done = SNDover(mToneHandle) != 0;
        SNDSYS_leavecritical();
        if (done) {
            StopTone(0.0f);
            StartTone();
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_StopTone);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Stops the track tone if one is playing.
 *
 * @param FadeTime  fade-out time in seconds (On Tour FadeTime)
 */
void cSSXAudio::StopTone(float FadeTime)
{
    if (mPlayingTone != 0) {
        int idx = mToneHandle;
        if (idx >= 0) {
            cBankMonitor_BANKMONITOR_StopHandle(&MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                idx, 1, FadeTime);
            mToneHandle = -1;
        }
        mPlayingTone = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SetBigChallengeStarting__FPv);
#ifdef SKIP_ASM
/**
 * Sets mBigChallengeStarting to 1.
 *
 * The lodge overlay (fe/ovstateenterlodge.cpp) calls it when a big challenge is being started; cSSXAudio_BigChallengeBegin clears it.
 */
void cSSXAudio::SetBigChallengeStarting()
{
    mBigChallengeStarting = 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_BigChallengeBegin);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: a non-empty `do { ... } while (0)` around the final monitor block, and a second pointer local `mon` (self
// + 0x118, the same address as `bank`) for the vt[4] call; plain form 97.52% UNPOSITIONED (99.61% without the wrapper
// only, 96.72% UNPOSITIONED without `mon` only). Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
// OPTS_Callback handlers handed to cSndPlayOpts: 8-byte pointer-to-member constants.
extern sSndCallback D_004A3648[];

extern sSndCallback D_004A3650_D6E0[] __asm__("D_004A3650");

extern sSndCallback D_004A3658[];

// g++ 2.95 vtable entry for a method taking (self, int).
struct sSndVtVI_D6E0 { short delta; short index; void (*fn)(void*, int); };

extern "C" void* cAudioInterface_getAI();

extern "C" void cSSXAudio_StartLocationMusic(void* self, int a1, int a2, int a3, int a4);

extern "C" void cSSXAudio_PickRandomSong(void* self);

extern "C" void cSSXAudio_EndUberSpin_D6E0(void* self, void* obj) __asm__("cSSXAudio_EndUberSpin");

extern "C" int cMusicSys_GetSongTag(void*);

// cBE interface lookup: cBE_getInterface__Fv takes (be, kind) despite its mangled name.
extern "C" void* cBE_getBE();

extern "C" cBEInterface* cBE_getInterface__Fv(void* be, int kind);

extern "C" void cMonitor_MONITOR_DestroyAll1_D6E0(void* bank, sSndCallback key) __asm__("cMonitor_MONITOR_DestroyAll1");

extern "C" int cMusicSys_IsOverlayArmed(void* monitor);
extern "C" void cMusicSys_ClearOverlayArmed(void* monitor);
extern "C" int cMusicSys_StartOverlay(void* monitor);
extern "C" int cBEMissionInterface_getAudioType(void* iface, int mission);
extern "C" int cSSXAudio_IsMusicRadioMode(void* self);
extern "C" int cSSXAudio_SetLastMusicState_D6E0(void* self, int a1) __asm__("cSSXAudio_SetLastMusicState");

/**
 * Starts the audio of a big challenge (mission): stops rider 0's wipeout and uber-spin sounds and picks the music.
 *
 * Clears mBigChallengeStarting and stops rider 0's wipeout slide and tumble sounds (cSSXAudio_WipeoutSlideEnd, cSSXAudio_WipeoutTumbleEnd) and
 * its uber spin (cSSXAudio_EndUberSpin). When cMusicSys_IsOverlayArmed on the music system is nonzero, calls the music system's
 * vtable slot 3 with 8 if cSSXAudio_IsMusicRadioMode is nonzero, then cMusicSys_ClearOverlayArmed. With unk_608C == 2 (a radio mode) it stops
 * there. When the current song's state (cMusicSys_GetSongTag) is 1..3 it destroys the monitors of the three front-end
 * callbacks D_004A3648, D_004A3650 and D_004A3658 (cMonitor_MONITOR_DestroyAll1), calls cSSXAudio_SetLastMusicState(self, 18), cSSXAudio_PickRandomSong and
 * cSSXAudio_StartLocationMusic(self, 0, 0, -1, 0), and returns. Otherwise it stores the mission's audio type
 * (cBEMissionInterface_getAudioType) in mCurBigChallengeType and maps it to a track (1: 33, 2: 34, 3: 38, 0 and 4: -1);
 * when cMusicSys_StartOverlay is nonzero the music system's vtable slot 4 gets the track and mInBigChallenge is set;
 * cMusicSys_ClearOverlayArmed follows.
 *
 * @param mission  the mission id (the script's LocalMissionId)
 */
extern "C" void cSSXAudio_BigChallengeBegin(cSSXAudio* self, int mission)
{
    char* bank = (char*)&self->MusicSys;
    self->mBigChallengeStarting = 0;
    cSSXAudio_WipeoutSlideEnd((cSSXAudio*)self, ((cAI*)cAudioInterface_getAI())->Riders[0]);
    cSSXAudio_WipeoutTumbleEnd((cSSXAudio*)self, ((cAI*)cAudioInterface_getAI())->Riders[0]);
    cSSXAudio_EndUberSpin_D6E0(((char*)self), 0);
    if (cMusicSys_IsOverlayArmed(bank) != 0) {
        if (cSSXAudio_IsMusicRadioMode(((char*)self)) != 0) {
            sSndVtVI_D6E0* vt = (sSndVtVI_D6E0*)self->MusicSys.mVtbl;
            vt[3].fn(vthis(bank, vt[3]), 8);
        }
        cMusicSys_ClearOverlayArmed(bank);
    }
    if (self->mRadioModeID == 2) {
        return;
    }
    if ((unsigned int)(cMusicSys_GetSongTag(&self->MusicSys) - 1) < 3) {
        cMonitor_MONITOR_DestroyAll1_D6E0(**(char***)&self->MusicSys + 0x1D8, D_004A3648[0]);
        cMonitor_MONITOR_DestroyAll1_D6E0(**(char***)&self->MusicSys + 0x1D8, D_004A3650_D6E0[0]);
        cMonitor_MONITOR_DestroyAll1_D6E0(**(char***)&self->MusicSys + 0x1D8, D_004A3658[0]);
        cSSXAudio_SetLastMusicState_D6E0(((char*)self), 18);
        cSSXAudio_PickRandomSong(((char*)self));
        cSSXAudio_StartLocationMusic(((char*)self), 0, 0, -1, 0);
        return;
    }
    int track = -1;
    int kind = cBEMissionInterface_getAudioType(cBE_getInterface__Fv(cBE_getBE(), 10), mission);
    self->mCurBigChallengeType = kind;
    switch (kind) {
    case 1: track = 33; break;
    case 2: track = 34; break;
    case 3: track = 38; break;
    case 0:
    case 4: track = -1; break;
    }
    do {
        if (cMusicSys_StartOverlay(&self->MusicSys) != 0) {
            sSndVtVI_D6E0* vt = (sSndVtVI_D6E0*)self->MusicSys.mVtbl;
            char* mon = (char*)&self->MusicSys;
            ((int (*)(void*, int))vt[4].fn)(vthis(mon, vt[4]), track);
            self->mInBigChallenge = 1;
        }
        cMusicSys_ClearOverlayArmed(&self->MusicSys);
    } while (0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_BigChallengeComplete);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int sndD8E0AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

extern "C" void* cAudioInterface_getAI();
extern "C" void cSSXAudio_SpeakArcadeResult(cSSXAudio* self, cRider* obj, int a2);

struct sSndVtEntD8E0 { short delta; short index; void (*fn)(void*, int); };

/**
 * Plays the big-challenge (mission) completion audio.
 *
 * When mInBigChallenge is set, the music system's vtable slot 4 gets track 39 and mCurBigChallengeType and
 * mInBigChallenge are cleared. Then bank 0, patch 109 plays with the volume control cSSXAudio_GetCurrAppliedVolumePtr(self, 9, 0) and the
 * DJ event cSSXAudio_SpeakArcadeResult is raised for rider 0 with 1. cWScriptMission_processComplete calls it.
 */
void cSSXAudio::BigChallengeComplete()
{
    if (mInBigChallenge != 0) {
        char* musicSys = (char*)&MusicSys;
        sSndVtEntD8E0* entry = &((sSndVtEntD8E0*)MusicSys.mVtbl)[4];
        entry->fn(vthis(musicSys, *entry), 39);
        mCurBigChallengeType = 0;
        mInBigChallenge = 0;
    }
    sndD8E0AddVoice(this, 0, 109, 9);
    cSSXAudio_SpeakArcadeResult((cSSXAudio*)this, ((cAI*)cAudioInterface_getAI())->Riders[0], 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_BigChallengeFailed);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_MenuSound(cSSXAudio* self, int id);
extern "C" void cSSXAudio_WipeoutSlideEnd(cSSXAudio* self, cRider* obj);
extern "C" void cSSXAudio_WipeoutTumbleEnd(cSSXAudio* self, cRider* obj);
extern "C" void cSSXAudio_EndUberSpin(cSSXAudio* self, cRider* obj);

struct sSndVtEnt { short delta; short index; void (*fn)(void*, int); };

/**
 * Ends a big-challenge's audio when the challenge is failed or quit.
 *
 * Only acts while mInBigChallenge is set: unless @p keep is set it plays menu sound 0xE (cSSXAudio_MenuSound), stops
 * rider 0's wipeout slide and tumble sounds (cSSXAudio_WipeoutSlideEnd, cSSXAudio_WipeoutTumbleEnd) and the uber-spin
 * (cSSXAudio_EndUberSpin), calls virtual slot 4 of MusicSys with track 39, and clears mCurBigChallengeType and
 * mInBigChallenge.
 *
 * @param keep  On Tour's Quit: nonzero skips the failure sound
 */
void cSSXAudio::BigChallengeFailed(int keep)
{
    if (mInBigChallenge != 0) {
        if (keep == 0) {
            MenuSound(0xE);
        }
        WipeoutSlideEnd(((cAI*)cAudioInterface_getAI())->Riders[0]);
        WipeoutTumbleEnd(((cAI*)cAudioInterface_getAI())->Riders[0]);
        EndUberSpin(0);
        char* musicSys = (char*)&MusicSys;
        sSndVtEnt* vtblEntry = &((sSndVtEnt*)MusicSys.mVtbl)[4];
        vtblEntry->fn(vthis(musicSys, *vtblEntry), 39);
        mCurBigChallengeType = 0;
        mInBigChallenge = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandPlantSound);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


static inline int sndDC48AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

/**
 * Plays the handplant sound (bank 9, patch 0x61) for a local human rider.
 *
 * @param obj  the rider
 */
void cSSXAudio::HandPlantSound(cRider* obj)
{
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        sndDC48AddVoice(this, 0, 97, 9);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_Avalanche);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
class cController;
class cBankMonitorInstance;
// Sound callback: a g++ 2.95 pointer to member function (8 bytes), as cSndPlayOpts::OPTS_Callback takes it.
typedef void (cController::*sSndCallback)(cController*, cBankMonitorInstance*, float);
// One cSndPlayOpts mDelay[] entry (reset to 100 / 0 / 90 / 50 on every push).
#define SND_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SND_OPTS_MAX(a, b) ((a) >= (b) ? (a) : (b))

// cSndPlayOpts (prototype class): the play-option stack; push = cur++ and copy def into mCurrent[cur]. The
// allocation tags of its ctor (sound/ps2soundman) name the arrays: ch = mCurrent, p34 = mPosStatic,
// vals = mBaseVol, p3C = mVolControl, p60 = mPtr, p64 = mViewport, p68 = mVanish, p6C = mRadius, p70 = mDelay,
// p74 = mExternalVol, p78 = mMapToMaxVol, p7C = mCloseOnEnd, p80 = mTwoPlayerPan, p84 = mTwoPlayerSide.
// cBankManager as this unit sees it: only its cSndPlayOpts part at 0x1D8 is used here.
// handle lists (tCrowdEventData_tCrowdEventData clears them; tCrowdEventData_GetRandomSample picks from one) and their timing floats.
extern "C" void* tCrowdEventData_tCrowdEventData(tCrowdEventData* self);
// sSSXAudioView's pending one-shot queue (an int ring of 5-word records).
// cSndPlayOpts::OPTS_Callback (prototype name): sets the handler of the voice being pushed.
// Callees declared by several functions below.
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
// g++ 2.95 vtable entry for a method taking (self, int).
struct sSndVtVI { short delta; short index; void (*fn)(void*, int); };

// g++ 2.95 vtable entry for a method taking (self, sSndCallback).
struct sSndVtCb { short delta; short index; void (*fn)(void*, sSndCallback); };
static inline void sndBeginVoice(sSSXAudioView* self, int a, int b)
{
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
}

extern "C" int cSSXAudio_GetAvalancheVolume(cSSXAudio* self, tAvalanchePositionalData* blk);
struct sQDEF0 {
    int w[4];
} __attribute__((aligned(16)));
struct sOutDEF0 {
    sQDEF0 q[2];
};
extern "C" void cAvalanche_getAvalancheDataByPosition_c(sOutDEF0* out, t3Vector* in) __asm__("cAvalanche_getAvalancheDataByPosition");

extern sSndCallback D_004A36D0_cb[] __asm__("D_004A36D0");
/**
 * Starts (reference-counted) or stops the avalanche rumble loop that follows the avalanche near the focus rider.
 *
 * With @p on set, the first start (mAvalancheInstanceCount becomes 1) takes the focus rider (view 0 when replaying),
 * keeps its AI index (cRider vtable slot 7) in mpAvalancheRiderIndex, fills mAvalancheData from its position (slot 5)
 * through cAvalanche_getAvalancheDataByPosition and plays bank 8, patch 2 with the volume cSSXAudio_GetAvalancheVolume(mAvalancheData), the volume control
 * cSSXAudio_GetCurrAppliedVolumePtr(vself, 5, 0), mAvalancheData's position to follow, a 3D radius of 100 (cSndPlayOpts_OPTS_Radius), the callback
 * cSSXAudio_HandleAvalanche (D_004A36D0), the rider's view (cSSXAudio_GetClosestCamera1) and mExternalVol 1; the handle goes to
 * mAvalancheLoopHandle. With @p on clear, the last stop (count back to 0) stops that handle (mode 1, fade 2.0),
 * destroys the bank monitor's instances with that callback (its vtable slot 3) and resets mAvalancheHitHandles[0..2] to
 * -1. The peak3 comment above it ("Grind/ride loop sound") is wrong: visualfx/avalanche.cpp calls it.
 *
 * @param on  nonzero to start, 0 to stop
 */
void cSSXAudio::Avalanche(int on)
{
    sSSXAudioView* self = (sSSXAudioView*)this;
    if (on != 0) {
        if (++self->users == 1) {
            int id = -1;
            if (cSSXAudio_ReplayMode(self) != 0) {
                id = 0;
            }
            cRider* rider = cSSXAudio_GetFocusRider_p(self, id);
            self->idx = rider->getAIIndex();
            {
                t3Vector* riderPos = (t3Vector*)rider->getPosition();
                sOutDEF0 out;
                t3Vector pos;
                pos.x = riderPos->x;
                pos.y = riderPos->y;
                pos.z = riderPos->z;
                cAvalanche_getAvalancheDataByPosition_c(&out, &pos);
                *(sOutDEF0*)&self->xf = out;
            }
            sndBeginVoice(self, 8, 2);
            cBankManager** bankManagerRef = self->mpVStreamSys;
            int volume = ((cSSXAudio*)self)->GetAvalancheVolume((tAvalanchePositionalData*)&self->xf);
            cBankManager* volManager = *bankManagerRef;
            cBankMonitor* volOpts = &volManager->BankMonitor;
            volOpts->mBaseVol[volManager->BankMonitor.mInst] = volume;
            volOpts->mCurrent[volManager->BankMonitor.mInst].vol
                    = (volOpts->mBaseVol[volOpts->mInst] < 0x7F && volOpts->mBaseVol[volOpts->mInst] < 0) ? 0
                    : (volOpts->mBaseVol[volOpts->mInst] < 0x7F ? (unsigned char)volOpts->mBaseVol[volOpts->mInst]
                    : 0x7F);
            cBankManager** pp = self->mpVStreamSys;
            float* volControl = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 5, 0);
            (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = volControl;
            {
                cBankManager* levelManager = *self->mpVStreamSys;
                levelManager->BankMonitor.mPosition[levelManager->BankMonitor.mInst] = (const t4Vector*)&self->xf;
            }
            cSndPlayOpts_OPTS_Radius(&(*self->mpVStreamSys)->BankMonitor, 100.0f, -1.0f);
            OPTS_Callback(*self->mpVStreamSys, D_004A36D0_cb[0]);
            {
                cBankManager** pq = self->mpVStreamSys;
                int viewIndex = cSSXAudio_GetClosestCamera1(self, rider);
                (*pq)->BankMonitor.mViewport[(*pq)->BankMonitor.mInst] = viewIndex;
            }
            (*self->mpVStreamSys)->BankMonitor.mExternalVol[(*self->mpVStreamSys)->BankMonitor.mInst] = 1;
            self->handle5FF4 = self->SFX_End();
        }
    } else {
        if (--self->users == 0) {
            cBankMonitor_BANKMONITOR_StopHandle(&(*self->mpVStreamSys)->BankMonitor, self->handle5FF4, 1, 2.0f);
            cBankManager* mg = *self->mpVStreamSys;
            sSndVtCb* vt = (sSndVtCb*)mg->BankMonitor.mVtbl;
            char* sub = (char*)&mg->BankMonitor;
            vt[3].fn(vthis(sub, vt[3]), D_004A36D0_cb[0]);
            for (int i = 2; i >= 0; i--) {
                self->slots[i] = -1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetAvalancheVolume);
#ifdef SKIP_ASM
/**
 * Returns the avalanche loop volume (0..127) for an avalanche's positional data.
 *
 * Computes (100 - m_distance * 0.01) * 1.27 * m_averageScale, scales it by 8.466667 and clamps the integer to 0..127.
 *
 * @param snd_  the avalanche data (tAvalanchePositionalData: m_averageScale at 0x10, m_distance at 0x14)
 * @return the volume, 0..127
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int cSSXAudio::GetAvalancheVolume(tAvalanchePositionalData* snd_)
{
    float volume = (100.0f - snd_->m_distance * 0.01f) * 1.27f * snd_->m_averageScale;
    int n = (int)(volume * 8.466667f);
    if (n > 127)
        n = 127;
    if (n < 0)
        n = 0;
    volume = n;
    return (int)volume;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleAvalanche);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_GetAvalancheVolume(cSSXAudio*, tAvalanchePositionalData*);
extern "C" void cAvalanche_getAvalancheDataByPosition(void *, float *);


/**
 * Updates the avalanche loop's voice: refreshes mAvalancheData around the avalanche rider and resets the volume.
 *
 * When mpAvalancheRiderIndex is below the cAI's NumberOfRidersAllocated, rebuilds mAvalancheData from that rider's
 * position (getPosition, through cAvalanche_getAvalancheDataByPosition) and stores
 * cSSXAudio_GetAvalancheVolume(mAvalancheData) into the voice's mBaseVol (its volume). cSSXAudio_Avalanche installs it
 * as the loop's callback (D_004A36D0).
 *
 * @param voice  the playing voice (cBankMonitorInstance)
 */
void cSSXAudio::HandleAvalanche(cBankMonitorInstance* voice)
{
    if (mpAvalancheRiderIndex < ((cAI*)cAudioInterface_getAI())->NumberOfRidersAllocated) {
        tAvalanchePositionalData positional;
        float riderPos[3];
        tAvalanchePositionalData* dst = &(*(&mAvalancheData));
        cRider* rider = ((cAI*)cAudioInterface_getAI())->Riders[mpAvalancheRiderIndex];
        float* pos = (float*)rider->getPosition();
        riderPos[0] = pos[0];
        riderPos[1] = pos[1];
        riderPos[2] = pos[2];
        cAvalanche_getAvalancheDataByPosition(&positional, riderPos);
        (*(&mAvalancheData)) = positional;
        voice->mBaseVol = GetAvalancheVolume(dst);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_AvalancheGroundHit__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: called with the position of each avalanche ground hit.
 *
 * tActiveAvalanche_update calls it for every avalanche event that fires and every sound cue it reaches.
 */
void cSSXAudio::AvalancheGroundHit()
{
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_AvalancheObjectHit);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty function; the retail image has no call or data reference to it).
 */
extern "C" void cSSXAudio_AvalancheObjectHit(void) {

}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_BonusSFX);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty function; the retail image has no call or data reference to it).
 */
extern "C" void cSSXAudio_BonusSFX(void) {

}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_ResetWipeoutRecoverHit);
#ifdef SKIP_ASM
/**
 * Resets the wipeout-recover progress of a rider's view: mLastWipeoutRecoverPercent[mViewIndex] = 0.
 *
 * cWipeoutControl_gainFocus calls it when a wipeout starts; cSSXAudio_PlayWipeoutRecoverHit compares against it.
 *
 * @param rider  the rider (cRider)
 */
void cSSXAudio::ResetWipeoutRecoverHit(cRider* rider)
{
    int idx = rider->mViewIndex;
    mLastWipeoutRecoverPercent[idx] = 0;

}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayWipeoutRecoverHit);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);

#define SND_E590_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_E590_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int sndE590AddVoice(void* vself, int a, int b, int type, unsigned short pitch)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager* m2 = self->MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* s2 = &m2->BankMonitor;
    s2->mBaseVol[m2->BankMonitor.mInst] = 0x7F;
    s2->mCurrent[m2->BankMonitor.mInst].vol = SND_E590_MAX(0, SND_E590_MIN(s2->mBaseVol[s2->mInst], 0x7F));
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    cBankManager* m3 = self->MusicSys.mpVStreamSys->mpVBankManager;
    m3->BankMonitor.mCurrent[m3->BankMonitor.mInst].pitchmult = pitch;
    return ((cBankSys*)self)->SFX_End();
}


extern "C" int cSSXAudio_ReplayMode(void* self);

/**
 * Plays a wipeout-recovery button hit for a local human rider, pitched up as the recovery progresses.
 *
 * The pitch uses the lower of @p x and the previous value for that view; @p x is then stored. Not during a
 * replay.
 *
 * @param obj  the rider
 * @param x  recovery progress (On Tour percent)
 */
void cSSXAudio::PlayWipeoutRecoverHit(cRider* obj, float x)
{
    if (cSSXAudio_ReplayMode(this) != 0) {
        return;
    }
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        int idx = obj->mViewIndex;
        float prev = mLastWipeoutRecoverPercent[idx];
        unsigned short pitch = (int)((x <? prev) * 5.0f) * 80 + 0x1000;
        sndE590AddVoice(this, 0, 95, 9, pitch);
        mLastWipeoutRecoverPercent[idx] = x;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayWipeoutRecoverFull);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);


extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);

#define SND_E970_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_E970_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int sndE970AddVoice(void* vself, int a, int b, int type)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = 1;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
    cBankManager* m2 = self->MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* s2 = &m2->BankMonitor;
    s2->mBaseVol[m2->BankMonitor.mInst] = 0x7F;
    s2->mCurrent[m2->BankMonitor.mInst].vol = SND_E970_MAX(0, SND_E970_MIN(s2->mBaseVol[s2->mInst], 0x7F));
    cBankManager** pp = &self->MusicSys.mpVStreamSys->mpVBankManager;
    float* r = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, type, 0);
    (*pp)->BankMonitor.mVolControl[(*pp)->BankMonitor.mInst] = r;
    return ((cBankSys*)self)->SFX_End();
}

extern "C" int cSSXAudio_ReplayMode(void* self);

/**
 * Plays the full-recovery sound once per recovery for a local human rider, except during a replay.
 *
 * @param obj  the rider
 */
void cSSXAudio::PlayWipeoutRecoverFull(cRider* obj)
{
    if (cSSXAudio_ReplayMode(this) != 0) {
        return;
    }
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        int idx = obj->mViewIndex;
        if (mLastWipeoutRecoverPercent[idx] == 1.0f) {
            return;
        }
        sndE970AddVoice(this, 0, 0x60, 9);
        mLastWipeoutRecoverPercent[idx] = 1.0f;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_StopRiderLoops);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getAI();
extern "C" void cSSXAudio_EndUberSpin(cSSXAudio* self, cRider* obj);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Stops rider 0's looping sounds: the wipeout slide and tumble, the uber spin and the turbo.
 *
 * Calls cSSXAudio_WipeoutSlideEnd, cSSXAudio_WipeoutTumbleEnd and cSSXAudio_EndUberSpin for the cAI's rider 0; while mTurboRef is above 0 it
 * stops mTurboHandle (cBankMonitor_BANKMONITOR_StopHandle, mode 1, fade 0) and clears mTurboRef. cGame_update and
 * cGame_updateUI call it when they bring up overlay 34.
 */
extern "C" void cSSXAudio_StopRiderLoops(cSSXAudio* self)
{
    self->WipeoutSlideEnd(((cAI*)cAudioInterface_getAI())->Riders[0]);
    self->WipeoutTumbleEnd(((cAI*)cAudioInterface_getAI())->Riders[0]);
    self->EndUberSpin(0);
    if (self->mTurboRef > 0) {
        cBankMonitor_BANKMONITOR_StopHandle(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                            self->mTurboHandle, 1, 0.0f);
        self->mTurboRef = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_OpenSpeechLanguage);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int BXFILE_exists(const char* name);
extern "C" void SPCH_Init(void* self);
extern "C" void cSpeechSys_Shutdown(void* self);
extern "C" void cSpeechSys_CreateInstance(void* self, int a, int b, char* name, float* volumeControl, int d, int e);
extern "C" void FILESYS_addbigsync(const char* name, int a, int b, void** h);
extern "C" void FILESYS_delbigsync(void* h, int a);
extern char str_data_audio_french_big[];
extern char str_data_audio_german_big[];
extern char str_data_audio_spanish_big[];
extern char str_data_audio_english_big[];
extern char str_CHAR[];

static inline void sndStreamLoad(const char* name, void** h)
{
    if (BXFILE_exists(name)) {
        FILESYS_addbigsync(name, 0, 100, h);
    }
}

/**
 * Opens the speech .big file of the language chosen in the options, and optionally restarts the speech system on it.
 *
 * Takes the language from cSSXAudio_GetSpeechLanguage and returns when it equals mSpeechLanguage (the language already open). With
 * @p restart set it first calls cSpeechSys_Shutdown on the speech system (SpeechSys, frees its bank buffer). The open stream
 * in mLanguageBigFile is closed (FILESYS_delbigsync(h, 100)), then "data/audio/french.big" (kSPEECHLANG_French),
 * "german.big" (kSPEECHLANG_German), "spanish.big" (kSPEECHLANG_Spanish) or "english.big" (otherwise) is opened into
 * mLanguageBigFile through FILESYS_addbigsync when BXFILE_exists finds it, and the language is recorded in
 * mSpeechLanguage. With @p restart set it then re-initialises the speech system (SPCH_Init) and starts speech instance
 * 0 on "CHAR" with the Character volume control cSSXAudio_GetCurrAppliedVolumePtr(self, 4, 0) (cSpeechSys_CreateInstance).
 *
 * @param restart  nonzero when the speech system is already running (the options screen); 0 from the constructor
 */
extern "C" void cSSXAudio_OpenSpeechLanguage(cSSXAudio* self, int restart)
{
    int mode = cSSXAudio_GetSpeechLanguage((cSSXAudio*)(((char*)self)));
    if (mode == self->mSpeechLanguage) {
        return;
    }
    if (restart) {
        cSpeechSys_Shutdown(&self->SpeechSys);
    }
    void** streamHandle = (void**)(&self->mLanguageBigFile);
    if (*streamHandle) {
        FILESYS_delbigsync(*streamHandle, 100);
    }
    if (mode == kSPEECHLANG_French) {
        sndStreamLoad(str_data_audio_french_big, streamHandle);
    } else if (mode == kSPEECHLANG_German) {
        sndStreamLoad(str_data_audio_german_big, streamHandle);
    } else if (mode == kSPEECHLANG_Spanish) {
        sndStreamLoad(str_data_audio_spanish_big, streamHandle);
    } else {
        sndStreamLoad(str_data_audio_english_big, streamHandle);
    }
    self->mSpeechLanguage = mode;
    if (restart) {
        SPCH_Init(&self->SpeechSys);
        cSpeechSys_CreateInstance(&self->SpeechSys, 0, 0, str_CHAR, (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 4, 0), 1, -1);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetSpeechVolumeIdentifier);
#ifdef SKIP_ASM
extern "C" int cAudioInterface_isInGame();

/**
 * Classifies a speech file by its name for speech instance 0, records the class in mSpeechVolumeID and returns it.
 *
 * Names starting "DJ_" give 2 (the DJ volume control) in game and 11 in the front end, "PA_" gives 3 (PA), "Arcade"
 * gives 10 (ArcadeSpeech) and anything else 4 (Character). Other instances return -1, as the cSpeechSys base does. It
 * fills slot 2 of cSSXAudio's cSpeechSys vtable (D_004836D8), where cSpeechSys has
 * cSpeechSys_GetSpeechVolumeIdentifier; cSSXAudio_UpdateSpeechVolume maps the class to a volume.
 *
 * @param instanceIndex  the speech instance (0 is the only one handled)
 * @param name  the speech file name
 * @return the volume class, or -1
 */
int cSSXAudio::GetSpeechVolumeIdentifier(int instanceIndex, const char* name)
{
    if (instanceIndex == 0) {
        if (name[0] == 'D' && name[1] == 'J' && name[2] == '_') {
            if (cAudioInterface_isInGame() != 0) {
                mSpeechVolumeID = 2;
            } else {
                mSpeechVolumeID = 11;
            }
        } else if (name[0] == 'P' && name[1] == 'A' && name[2] == '_') {
            mSpeechVolumeID = 3;
        } else if (name[0] == 'A' && name[1] == 'r' && name[2] == 'c' && name[3] == 'a' && name[4] == 'd' && name[5] == 'e') {
            mSpeechVolumeID = 10;
        } else {
            mSpeechVolumeID = 4;
        }
        return mSpeechVolumeID;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UpdateSpeechVolume);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_DuckToStream(void* self, int a1, float f0, float f1);

/**
 * Returns the volume control a speech of a given class plays at, for speech instance 0 (0 for other instances).
 *
 * keVolumeControl_DJ also ducks stream 0 to 0.65 over about a second (cSSXAudio_DuckToStream(self, 0, 0.9999, 0.65)).
 * kFrontEndDJVolumeClass (a DJ line in the front end) returns &mFrontEndDJVolume, a fixed 1.0; every other class
 * returns cSSXAudio_GetCurrAppliedVolumePtr(self, type, a3), the applied volume of that control (per character for
 * keVolumeControl_Character). It fills slot 3 of cSSXAudio's cSpeechSys vtable (D_004836D8), where cSpeechSys has
 * cSpeechSys_UpdateSpeechVolume (which returns 0).
 *
 * @param instanceIndex  the speech instance (0 is the only one handled)
 * @param type  the volume class from cSSXAudio_GetSpeechVolumeIdentifier
 * @param charIndex  the character index passed on to cSSXAudio_GetCurrAppliedVolumePtr
 * @return a pointer to the volume (float), or 0
 */
void* cSSXAudio::UpdateSpeechVolume(int instanceIndex, int type, int charIndex)
{
    if (instanceIndex == 0) {
        if (type == keVolumeControl_DJ) {
            cSSXAudio_DuckToStream(this, 0, 0.9999024868011475f, 0.6499993801116943f);
        }
        if (type == kFrontEndDJVolumeClass) {
            return (char*)&mFrontEndDJVolume;
        }
        return cSSXAudio_GetCurrAppliedVolumePtr(this, type, charIndex);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetLastSpeechVolumeID);
#ifdef SKIP_ASM
/**
 * Returns the speech volume class last recorded for speech instance 0 (mSpeechVolumeID, set by
 * cSSXAudio_GetSpeechVolumeIdentifier), else -1.
 *
 * cSSXAudio_ApplyRMSData reads it to pick the RMS duck level.
 *
 * @param instanceIndex  the speech instance
 * @return the class (2 DJ, 3 PA, 4 Character, 10 ArcadeSpeech, 11 front-end DJ), or -1 for other instances
 */
extern "C" int cSSXAudio_GetLastSpeechVolumeID(cSSXAudio* self, int instanceIndex)
{
    if (instanceIndex == 0) {
        return self->mSpeechVolumeID;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsCharacterSpeechOn);
#ifdef SKIP_ASM
extern "C" float cSSXAudio_GetCurrSetVolume(void* self, int ch);

/**
 * Tells whether character speech is audible: the set volume of control 4 (Character) is not 0.
 *
 * Every character speech event (cSSXAudio_PlayGrunt, cSSXAudio_SpeakBigAir .. cSSXAudio_SpeakCustomize) checks it first.
 *
 * @return true when the Character volume (cSSXAudio_GetCurrSetVolume(self, 4)) is nonzero, else false
 */
extern "C" bool cSSXAudio_IsCharacterSpeechOn(void* self)
{
    if (cSSXAudio_GetCurrSetVolume(self, 4) == 0.0f) {
        return false;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsDJSpeechOn);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" float cSSXAudio_GetCurrSetVolume(void* self, int ch);

/**
 * Tells whether DJ speech may play: not replaying and the set volume of control 2 (DJ) is not 0.
 *
 * The DJ speech events cSSXAudio_SpeakDJLine8392 .. cSSXAudio_SpeakDJUberTricksResult check it first.
 *
 * @return true when allowed, else false
 */
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return false;
    }
    if (cSSXAudio_GetCurrSetVolume(self, 2) == 0.0f) {
        return false;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsPASpeechOn);
#ifdef SKIP_ASM
extern "C" float cSSXAudio_GetCurrSetVolume(void* self, int ch);

/**
 * Tells whether PA speech is audible: the set volume of control 3 (PA) is not 0.
 *
 * The PA speech events cSSXAudio_SpeakPAEventStart .. cSSXAudio_SpeakPALine8379 check it first.
 *
 * @return true when the PA volume (cSSXAudio_GetCurrSetVolume(self, 3)) is nonzero, else false
 */
extern "C" bool cSSXAudio_IsPASpeechOn(cSSXAudio* self)
{
    if (cSSXAudio_GetCurrSetVolume(self, 3) == 0.0f) {
        return false;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsArcadeSpeechOn);
#ifdef SKIP_ASM
extern "C" float cSSXAudio_GetCurrSetVolume(void* self, int ch);

/**
 * Tells whether arcade speech is audible: the set volume of control 10 (ArcadeSpeech) is not 0.
 *
 * The arcade announcer events cSSXAudio_SpeakArcadeBoost .. cSSXAudio_SpeakArcadeMultiplier check it first.
 *
 * @return true when the ArcadeSpeech volume (cSSXAudio_GetCurrSetVolume(self, 10)) is nonzero, else false
 */
extern "C" bool cSSXAudio_IsArcadeSpeechOn(cSSXAudio* self)
{
    if (cSSXAudio_GetCurrSetVolume(self, 10) == 0.0f) {
        return false;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetSpeechLanguage);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

// cBEOptionInterface_mOption as this function reads it (a tBEOptionDB, be/beintoption.h; reading its iLanguage
// bit-field directly changes the switch's code). bitFieldWord (name ours) is that struct's first word of bit-fields.
struct sSsOpts9F198 {
    unsigned int bitFieldWord;
    int data[0x288 / 4 - 1];
};
extern sSsOpts9F198 cBEOptionInterface_mOption_ss9F198 __asm__("cBEOptionInterface_mOption");

/**
 * Returns the speech language chosen in the options as a bit (eSpeechLanguageBit): 1 English, 2 French, 4 German,
 * 8 Spanish.
 *
 * Reads bits 22..24 of the first word of cBEOptionInterface_mOption (after cBE_getInterface(kind 4)), an
 * eOptionSpeechLanguage: kOPTLANG_English .. kOPTLANG_Spanish give kSPEECHLANG_English .. kSPEECHLANG_Spanish, anything
 * else kSPEECHLANG_English.
 *
 * @return the language bit
 */
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 4);
    sSsOpts9F198 optionCopy = cBEOptionInterface_mOption_ss9F198;
    unsigned languageIndex = optionCopy.bitFieldWord >> 22;
    languageIndex &= 7;
    switch (languageIndex) {
    case kOPTLANG_English:
        return kSPEECHLANG_English;
    case kOPTLANG_French:
        return kSPEECHLANG_French;
    case kOPTLANG_German:
        return kSPEECHLANG_German;
    case kOPTLANG_Spanish:
        return kSPEECHLANG_Spanish;
    }
    return kSPEECHLANG_English;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetCharacterSpeechBit);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: the unit declares this as void(int, int); defined under an asm label with its real type
extern "C" int cSSXAudio_GetCharacterSpeechBit_impl(unsigned int idx, int flag) __asm__("cSSXAudio_GetCharacterSpeechBit");
/**
 * Returns the speech-filter bit of a character identity, or 0 for a cheat identity.
 *
 * With @p flag 0 the identities (eBECharacters) map to kCHAR_ELISE 0x1, kCHAR_MAC 0x2, kCHAR_MOBY 0x4, kCHAR_PSYMON
 * 0x8, kCHAR_KAORI 0x10, kCHAR_ZOE 0x20, kCHAR_NATE 0x40, kCHAR_VIGGO 0x80, kCHAR_GRIFF 0x100 and kCHAR_ALLEGRA 0x200
 * (cSSXAudio_LoadGruntBanks's grunt banks agree); other identities and a nonzero @p flag give 0. The unit declares it void(int,
 * int).
 *
 * @param idx  the character identity (cRider_getIdentity)
 * @param flag  the cheat identity (cRider_getCheatIdentity); nonzero gives 0
 * @return the character's bit, or 0
 */
extern "C" int cSSXAudio_GetCharacterSpeechBit_impl(unsigned int idx, int flag)
{
    if (flag == 0) {
        switch (idx) {
        case kCHAR_ELISE: return 1;
        case kCHAR_MAC: return 2;
        case kCHAR_MOBY: return 4;
        case kCHAR_PSYMON: return 8;
        case kCHAR_KAORI: return 0x10;
        case kCHAR_ZOE: return 0x20;
        case kCHAR_VIGGO: return 0x80;
        case kCHAR_NATE: return 0x40;
        case kCHAR_GRIFF: return 0x100;
        case kCHAR_ALLEGRA: return 0x200;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetRiderSpeechBit);
#ifdef SKIP_ASM
extern "C" int cRider_getIdentity(void*);
extern "C" int cRider_getCheatIdentity(void*);
extern "C" void cSSXAudio_GetCharacterSpeechBit(int, int);

/**
 * Returns a rider's speech-filter bit: cSSXAudio_GetCharacterSpeechBit of its identity and cheat identity (declared void).
 *
 * Its argument is the rider (cRider), not the audio system: the character speech events pass the rider and use the
 * value left in v0.
 */
extern "C" void cSSXAudio_GetRiderSpeechBit(void* self)
{
    int identity = cRider_getIdentity(self);
    cSSXAudio_GetCharacterSpeechBit(identity, cRider_getCheatIdentity(self));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetGruntCharacter);
#ifdef SKIP_ASM
/**
 * Returns the character whose grunt bank a rider uses.
 *
 * Without a cheat identity it is the rider's identity (cRider_getIdentity); cheat identities 14, 16 and 21 use 6 (eli)
 * and all other cheat identities 7 (nat).
 *
 * @param obj  the rider (cRider)
 * @return the character identity for the grunt bank
 */
extern "C" int cSSXAudio_GetGruntCharacter(cSSXAudio* self, cRider* obj)
{
    switch (cRider_getCheatIdentity(obj)) {
    case 0:
        return cRider_getIdentity(obj);
    case 14:
    case 16:
    case 21:
        return 6;
    default:
        return 7;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_LoadGruntBanks);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cBankManager_BANK_SyncLoad(void* bank, int slot, char* name, int a3);
extern char str_pct_spct_s_c[];
extern char str_data_audio[];
extern char str_GRNT_eli_bnk[];
extern char str_GRNT_mac_bnk[];
extern char str_GRNT_mob_bnk[];
extern char str_GRNT_psy_bnk[];
extern char str_GRNT_kao_bnk[];
extern char str_GRNT_zoe_bnk[];
extern char str_GRNT_vig_bnk[];
extern char str_GRNT_nat_bnk[];
extern char str_GRNT_grf_bnk[];
extern char str_GRNT_ari_bnk[];
extern char str_GRNT_none_bnk[];
extern char str_GRNT_AI_bnk[];

/**
 * Loads the grunt banks: rider 0's character into bank slot 14 and the shared AI grunts into slot 15.
 *
 * Picks "data/audio/GRNT_<char>.bnk" from cSSXAudio_GetGruntCharacter for the cAI's rider 0 (eli, mac, mob, psy, kao, zoe, vig, nat,
 * grf or ari, "GRNT_none.bnk" for anything else) and "data/audio/GRNT_AI.bnk", and loads them with cBankManager_BANK_SyncLoad on the
 * cBankManager (synchronous, slot 14 and 15). cSSXAudio_InGameLoad calls it.
 */
extern "C" void cSSXAudio_LoadGruntBanks(cSSXAudio* self)
{
    char buf[64];
    switch (cSSXAudio_GetGruntCharacter((cSSXAudio*)(((char*)self)), ((cAI*)cAudioInterface_getAI())->Riders[0])) {
    case kCHAR_ELISE: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_eli_bnk); break;
    case kCHAR_MAC: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_mac_bnk); break;
    case kCHAR_MOBY: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_mob_bnk); break;
    case kCHAR_PSYMON: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_psy_bnk); break;
    case kCHAR_KAORI: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_kao_bnk); break;
    case kCHAR_ZOE: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_zoe_bnk); break;
    case kCHAR_VIGGO: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_vig_bnk); break;
    case kCHAR_NATE: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_nat_bnk); break;
    case kCHAR_GRIFF: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_grf_bnk); break;
    case kCHAR_ALLEGRA: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_ari_bnk); break;
    default: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_none_bnk); break;
    }
    cBankManager_BANK_SyncLoad(*(void**)self->MusicSys.mpVStreamSys, 0xE, buf, 0);
    sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_AI_bnk);
    cBankManager_BANK_SyncLoad(*(void**)self->MusicSys.mpVStreamSys, 0xF, buf, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UnloadGruntBanks);
#ifdef SKIP_ASM
extern "C" void cBankManager_BANK_UnLoad(void* self, int i);

/**
 * Unloads the grunt banks in bank slots 14 and 15 (cBankManager_BANK_UnLoad); cSSXAudio_InGamePurge calls it.
 */
extern "C" void cSSXAudio_UnloadGruntBanks(cSSXAudio* self)
{
    cBankManager_BANK_UnLoad(*(void**)self->MusicSys.mpVStreamSys, 0xE);
    cBankManager_BANK_UnLoad(*(void**)self->MusicSys.mpVStreamSys, 0xF);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetGruntBank);
#ifdef SKIP_ASM
/**
 * Returns the bank slot of a rider's grunts: 14 (its own character bank) for rider 0, 15 (the AI grunts) otherwise.
 *
 * Tests the rider's AI index (cRider vtable slot 7). cSSXAudio_PlayGrunt uses it.
 *
 * @param obj  the rider (cRider)
 * @return 14 or 15
 */
extern "C" int cSSXAudio_GetGruntBank(cSSXAudio* self, cRider* obj)
{
    if (obj->getAIIndex() == 0) {
        return 0xE;
    }
    return 0xF;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayGrunt);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsCharacterSpeaking(cSSXAudio* self, int id);
extern "C" bool cSSXAudio_StopCharacterSpeech(cSSXAudio* self, int id);
extern "C" unsigned int cSoundSys_SndRand(void* mon);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);
extern "C" bool cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* obj);

/**
 * Plays a rider's grunt as a positional voice and records it in the play history (event 3).
 *
 * Skipped when speech is off, during a replay, after the rider has finished, or while another grunt plays; a
 * computer rider needs @p allowAI, or cSSXAudio_IsCharacterSpeaking() to allow it. The sample depends on the character and the
 * length (character kind 14 picks randomly for keCharGruntLength_Long and keCharGruntLength_Short, or cycles for other
 * lengths); local humans get a shorter range.
 *
 * @param vrider  the rider
 * @param length  grunt length (eCharGruntLength)
 * @param allowAI  nonzero to allow computer riders (On Tour allowAI)
 */
void cSSXAudio::PlayGrunt(cRider* vrider, int length, int allowAI)
{
    cSSXAudio* self = this;
    cRider* rider = (cRider*)vrider;
    if (cSSXAudio_IsCharacterSpeechOn(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int ok = rider->mIsHuman != 0 && rider->mIsLocal != 0;
    if (!ok && allowAI == 0) {
        if (cSSXAudio_IsCharacterSpeaking((cSSXAudio*)self, cRider_getIdentity(rider)) != 0) return;
    }
    int past = rider->mFinishTime >= 0.0f;
    if (past) return;
    if (self->PlayHistoryExists(3, 0) != 0) return;
    cSSXAudio_StopCharacterSpeech((cSSXAudio*)self, cRider_getIdentity(rider));
    int kind = cSSXAudio_GetGruntBank(self, rider);
    int prio;
    if (kind == 0xE) {
        if (length == keCharGruntLength_Long) {
            prio = (cSoundSys_SndRand(self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys) & 0x7FFF)
                   * 3 / 0x7FFF;
        } else if (length == keCharGruntLength_Short) {
            prio = (cSoundSys_SndRand(self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys) & 0x7FFF)
                   * 3 / 0x7FFF + 3;
        } else {
            prio = self->mGruntPatchCycle;
            self->mGruntPatchCycle = prio + 1;
            if (prio + 1 >= 6) {
                self->mGruntPatchCycle = 0;
            }
        }
    } else if (length == keCharGruntLength_Long) {
        prio = cSSXAudio_GetGruntCharacter(self, rider) * 2 + 1;
    } else {
        prio = cSSXAudio_GetGruntCharacter(self, rider) * 2;
    }
    int flag = 0;
    int ok2 = rider->mIsHuman != 0 && rider->mIsLocal != 0;
    if (ok2) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
            flag = cSSXAudio_ReplayMode(self) == 0;
        }
    }
    int vol = self->GetGruntVol(rider);
    snd0018AddVoice(self, kind, prio);
    cBankManager* curVoiceMgr = self->MusicSys.mpVStreamSys->mpVBankManager;
    cBankMonitor* curVoiceSub = &curVoiceMgr->BankMonitor;
    curVoiceSub->mBaseVol[curVoiceMgr->BankMonitor.mInst] = vol;
    curVoiceSub->mCurrent[curVoiceMgr->BankMonitor.mInst].vol =
        (curVoiceSub->mBaseVol[curVoiceSub->mInst] < 0x7F && curVoiceSub->mBaseVol[curVoiceSub->mInst] < 0)
            ? 0
            : (curVoiceSub->mBaseVol[curVoiceSub->mInst] < 0x7F
                   ? (unsigned char)curVoiceSub->mBaseVol[curVoiceSub->mInst]
                   : 0x7F);
    {
        cStreamSys* pq = self->MusicSys.mpVStreamSys;
        int closestCamera = cSSXAudio_GetClosestCamera1(self, rider);
        pq->mpVBankManager->BankMonitor.mViewport[pq->mpVBankManager->BankMonitor.mInst] = closestCamera;
    }
    {
        cStreamSys* pp = self->MusicSys.mpVStreamSys;
        float* volumePtr = (float*)cSSXAudio_GetCurrAppliedVolumePtr(self, 4, cRider_getIdentity(rider));
        pp->mpVBankManager->BankMonitor.mVolControl[pp->mpVBankManager->BankMonitor.mInst] = volumePtr;
    }
    int ok3 = rider->mIsHuman != 0 && rider->mIsLocal != 0;
    if (ok3) {
        cSndPlayOpts_OPTS_Radius(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, 29.997713088989258f, -1.0f);
    } else {
        cSndPlayOpts_OPTS_Radius(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, 100.0f, -1.0f);
    }
    {
        cStreamSys* pt = self->MusicSys.mpVStreamSys;
        t4Vector* pos = rider->getPosition();
        pt->mpVBankManager->BankMonitor.mPosition[pt->mpVBankManager->BankMonitor.mInst] = pos;
    }
    if (flag) {
        self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor
            .mTwoPlayerPan[self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = 1;
        self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor
            .mTwoPlayerSide[self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mInst] = rider->mViewIndex;
    }
    int sfxHandle = ((cBankSys*)self)->SFX_End();
    self->PlayHistoryAdd((void*)3, sfxHandle,
                             self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mCounter + 50, 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakBigAir);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_IsInBackCountry(cSSXAudio* self);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cAudioInterface_getBE();
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
extern "C" int cSpeechSys_CanTakeSpeech(void* self, int i);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern "C" int cRider_getIdentity(void*);
// PORT: the unit defines cSSXAudio_GetRiderSpeechBit as void; it returns its tail call's result.
int cSSXAudio_GetRiderSpeechBit_i(void* self) __asm__("cSSXAudio_GetRiderSpeechBit");
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
// PORT: D_004A482C is called with 4 args here (the unit's later declaration has 3).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");
extern float D_004A36D8;


/**
 * Requests a rider's big-air speech line when its predicted air time is at least about 4 seconds.
 *
 * Needs the Character volume on (cSSXAudio_IsCharacterSpeechOn), a location outside the back-country 14..16 (cSSXAudio_IsInBackCountry), no replay,
 * a rider not past the finish line, the air predictor's +0x98 (mAirPredictor) of at least 4.000256, and for a computer
 * or non-local rider that it is the focus rider. Then speech instance 0 must be ready (cSpeechSys_CanTakeSpeech) and the rider
 * within 30 units of a camera (D_004A36D8). Speech 8324, 8323 or 8337 (peak 1, 2 or 3 of the location, from
 * cSSXAudio_GetLocationPeakBit) is requested for the rider on speech instance 0 (cSpeechSys_AddRequest); when queued, the speech hook
 * D_004A482C gets its packed id (SPCH_makeKey(1, 0, id)), 2 arguments, the rider's speech bit (cSSXAudio_GetRiderSpeechBit) and 3.
 * cSSXAudio_LiftWithAirTime calls it.
 *
 * @param rider  the rider
 */
extern "C" void cSSXAudio_SpeakBigAir(cSSXAudio* self, cRider* rider)
{
    if (cSSXAudio_IsCharacterSpeechOn(self) == 0) return;
    if (cSSXAudio_IsInBackCountry(self) != 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int pos = rider->mFinishTime >= 0.0f;
    if (pos) return;
    if (rider->mAirPredictor->TotalAirTime < 4.000256061553955f) return;
    int chk = 0;
    if (rider->isComputer() != 0 || rider->mIsLocal == 0) chk = 1;
    if (chk && cSSXAudio_GetFocusRider_p(self, -1) != rider) return;
    float finishTime = rider->mFinishTime;
    float zero = 0.0f;
    int pos2 = finishTime >= zero;
    if (pos2) return;
    char* snd = (char*)&self->SpeechSys;
    if (cSpeechSys_CanTakeSpeech(snd, 0) == 0) return;
    int speakerBit = cSSXAudio_GetRiderSpeechBit_i(rider);
    if (cSSXAudio_CameraInRange(self, rider->getPosition(), D_004A36D8) < 0) return;
    int sel = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
    if (sel == 1) {
        if (cSpeechSys_AddRequest(snd, 0, 8324, rider, cRider_getIdentity(rider), 0, 0, zero) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8324), 2, speakerBit, 3);
        }
    } else if (sel == 2) {
        if (cSpeechSys_AddRequest(snd, 0, 8323, rider, cRider_getIdentity(rider), 0, 0, zero) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8323), 2, speakerBit, 3);
        }
    } else {
        if (cSpeechSys_AddRequest(snd, 0, 8337, rider, cRider_getIdentity(rider), 0, 0, zero) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8337), 2, speakerBit, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakCheer);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsCharacterSpeechOn(void* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cAudioInterface_getBE();
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern "C" int cRider_getIdentity(void*);
// PORT: the unit defines cSSXAudio_GetRiderSpeechBit as void; it returns its tail call's result.
int cSSXAudio_GetRiderSpeechBit_i(void* self) __asm__("cSSXAudio_GetRiderSpeechBit");
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern void (*D_004A482C)(void*, int, int);
extern float D_004A36D8;


/**
 * Requests a rider's positive-reaction speech line (a cheer), the stronger set when @p alt is set.
 *
 * Needs the Character volume on (cSSXAudio_IsCharacterSpeechOn), no replay, a rider not past the finish line, and for a computer or
 * non-local rider that it is the focus rider; the rider must be within 30 units of a camera. By the location's peak
 * (cSSXAudio_GetLocationPeakBit: 1, 2, other) it requests speech 8336/8347/8349 with @p alt set, else 8322/8346/8348, for the rider on
 * speech instance 0 (cSpeechSys_AddRequest) and, when queued, passes the packed id and the rider's speech bit (cSSXAudio_GetRiderSpeechBit)
 * to the speech hook D_004A482C. cRider_checkForReaction calls it for reaction types 1 (alt 0) and 2 (alt 1).
 *
 * @param rider  the rider (cRider)
 * @param alt  nonzero for the uber reaction (type 2)
 */
extern "C" void cSSXAudio_SpeakCheer(cSSXAudio* self, cRider* rider, int alt)
{
    if (cSSXAudio_IsCharacterSpeechOn(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int pos = rider->mFinishTime >= 0.0f;
    if (pos) return;
    int chk = 0;
    if (rider->isComputer() != 0 || rider->mIsLocal == 0) chk = 1;
    if (chk && cSSXAudio_GetFocusRider_p(self, -1) != rider) return;
    int speakerBit = cSSXAudio_GetRiderSpeechBit_i(rider);
    if (cSSXAudio_CameraInRange(self, rider->getPosition(), D_004A36D8) < 0) return;
    if (alt != 0) {
        int sel = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
        if (sel == 1) {
            if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8336, rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
                D_004A482C(SPCH_makeKey(1, 0, 8336), 1, speakerBit);
            }
        } else if (sel == 2) {
            if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8347, rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
                D_004A482C(SPCH_makeKey(1, 0, 8347), 1, speakerBit);
            }
        } else {
            if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8349, rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
                D_004A482C(SPCH_makeKey(1, 0, 8349), 1, speakerBit);
            }
        }
    } else {
        int sel = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
        if (sel == 1) {
            if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8322, rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
                D_004A482C(SPCH_makeKey(1, 0, 8322), 1, speakerBit);
            }
        } else if (sel == 2) {
            if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8346, rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
                D_004A482C(SPCH_makeKey(1, 0, 8346), 1, speakerBit);
            }
        } else {
            if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8348, rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
                D_004A482C(SPCH_makeKey(1, 0, 8348), 1, speakerBit);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakWipeout);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_IsAtHub(cSSXAudio* self);
extern "C" bool cSSXAudio_IsFreeride(cSSXAudio* self);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cAudioInterface_getBE();
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
extern "C" int cSpeechSys_CanTakeSpeech(void* self, int i);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern "C" int cRider_getIdentity(void*);
// PORT: the unit defines cSSXAudio_GetRiderSpeechBit as void; it returns its tail call's result.
int cSSXAudio_GetRiderSpeechBit_i(void* self) __asm__("cSSXAudio_GetRiderSpeechBit");
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
// PORT: D_004A482C is called with 4 args here (the unit's later declaration has 3).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");
extern float D_004A36D8;


/**
 * Requests a rider's wipeout speech line.
 *
 * Needs the Character volume on (cSSXAudio_IsCharacterSpeechOn), no replay, a rider not past the finish line, for a computer or
 * non-local rider that it is the focus rider, speech instance 0 ready (cSpeechSys_CanTakeSpeech) and the rider within 30 units of
 * a camera. By the location's peak (cSSXAudio_GetLocationPeakBit: 1, 2, other) it requests speech 8328, 8352 or 8353 for the rider
 * and, when queued, passes the packed id, the rider's speech bit and a mode to the speech hook D_004A482C: 1 at the
 * locations 17..21 (cSSXAudio_IsAtHub) or in game mode 4 (cSSXAudio_IsFreeride), else 3. cWipeoutControl_loseFocus calls it.
 *
 * @param rider  the rider (cRider)
 */
extern "C" void cSSXAudio_SpeakWipeout(cSSXAudio* self, cRider* rider)
{
    if (cSSXAudio_IsCharacterSpeechOn(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int pos = rider->mFinishTime >= 0.0f;
    if (pos) return;
    int chk = 0;
    if (rider->isComputer() != 0 || rider->mIsLocal == 0) chk = 1;
    if (chk && cSSXAudio_GetFocusRider_p(self, -1) != rider) return;
    if (cSpeechSys_CanTakeSpeech((char*)&self->SpeechSys, 0) == 0) return;
    int speakerBit = cSSXAudio_GetRiderSpeechBit_i(rider);
    int mode;
    if (cSSXAudio_IsAtHub(self) != 0 || cSSXAudio_IsFreeride(self) != 0) {
        mode = 1;
    } else {
        mode = 3;
    }
    if (cSSXAudio_CameraInRange(self, rider->getPosition(), D_004A36D8) < 0) return;
    int sel = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
    if (sel == 1) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8328, rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8328), 2, speakerBit, mode);
        }
    } else if (sel == 2) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8352, rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8352), 2, speakerBit, mode);
        }
    } else {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8353, rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8353), 2, speakerBit, mode);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakOvertake);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsCharacterSpeechOn(void* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cRider_getCheatIdentity(void*);
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
extern "C" int cSpeechSys_CanTakeSpeech(void* self, int i);
int cSpeechSys_AddRequest_f(void* self, int a, int b, cRider* rider, int d, float f, int e,
        int refresh) __asm__("cSpeechSys_AddRequest");
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern "C" int cRider_getIdentity(void*);
extern "C" cGame* cAudioInterface_getGame();
// PORT: the unit defines cSSXAudio_GetRiderSpeechBit/cSSXAudio_GetRiderSubjectBit as void; they return their tail call's result.
int cSSXAudio_GetRiderSpeechBit_i(void* self) __asm__("cSSXAudio_GetRiderSpeechBit");
int cSSXAudio_GetRiderSubjectBit_i(void* self, void* obj) __asm__("cSSXAudio_GetRiderSubjectBit");
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
unsigned cSSXAudio_GetRiderRelation_0560(void* self, void* a, void* b) __asm__("cSSXAudio_GetRiderRelation");
void fSinCos_0560(float a, float* x, float* y) __asm__("fSinCos");
// PORT: D_004A482C is called with 4 and 5 args here (the unit's later declaration has 3).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");
extern float D_004A36D8;
static inline int sndA30Mode0560(int m)
{
    return ScriptEngine->mPlayingNISList == m;
}


// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_0560(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}


void cSSXAudio_SpeakOvertake_0560(cSSXAudio* self, cRider* a, cRider* b) __asm__("cSSXAudio_SpeakOvertake");
/**
 * Requests the speech of two riders when one overtakes the other fast enough: a taunt and a reply.
 *
 * Needs the Character volume on, no NIS playing (ScriptEngine->mPlayingNISList == 2), no replay, no cheat identity on
 * either rider, at least 600 AI ticks since mGruntCycle, neither rider past the finish line, and one of them the focus
 * rider or a local human. Speech instance 0 must be ready, @p passer within 30 units of a camera and @p passed in the same camera
 * range. The closing speed is @p passer's velocity minus @p passed's velocity along @p passed's heading (the angle at +0x4CC,
 * fSinCos); it must lie between 670.56 and 1788.16 (15 and 40 mph in cm/s), and from 1341.12 (30 mph) the mode is 2,
 * else 1. With the relation cSSXAudio_GetRiderRelation == 2, speech 8321 is requested for @p passer and 8319 for @p passed; with 0 or 1, 8320
 * for @p passer (its event also gets @p passed's character bit cSSXAudio_GetRiderSubjectBit ORed with 0x1000) and 8317 for @p passed. Each queued
 * request goes to the speech hook D_004A482C with the speaker's bit and the mode.
 *
 * @param passer  the rider that passes
 * @param passed  the rider being passed
 */
void cSSXAudio_SpeakOvertake_0560(cSSXAudio* self, cRider* passer, cRider* passed)
{
    if (cSSXAudio_IsCharacterSpeechOn(self) == 0) return;
    if (!sndA30Mode0560(2)) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cRider_getCheatIdentity(passer) != 0) return;
    if (cRider_getCheatIdentity(passed) != 0) return;
    if (cAudioInterface_getGame()->AI->AITick - self->mGruntCycle < 600) return;
    int posA = passer->mFinishTime >= 0.0f;
    if (posA) return;
    int posB = passed->mFinishTime >= 0.0f;
    if (posB) return;
    int chkA = 0;
    if (passer->isComputer() != 0 || passer->mIsLocal == 0) chkA = 1;
    int chkB = 0;
    if (passed->isComputer() != 0 || passed->mIsLocal == 0) chkB = 1;
    void* pl = cSSXAudio_GetFocusRider_p(self, -1);
    if (chkA && chkB && passer != pl && passed != pl) return;
    cSpeechSys* snd = &self->SpeechSys;
    if (cSpeechSys_CanTakeSpeech(snd, 0) == 0) return;
    int okA = cSSXAudio_CameraInRange(self, passer->getPosition(), D_004A36D8);
    if (okA < 0) return;
    int okB = cSSXAudio_CameraInRange(self, passed->getPosition(), D_004A36D8);
    if (okA != okB) return;
    float headingSin, headingCos;
    fSinCos_0560(passed->PathHeading, &headingSin, &headingCos);
    t4Vector headingDir;
    headingDir.x = headingCos;
    headingDir.y = headingSin;
    headingDir.z = 0;
    headingDir.w = 0;
    float closingSpeed = vu0Dot_0560(headingDir, *passer->getVelocity());
    closingSpeed -= vu0Dot_0560(headingDir, *passed->getVelocity());
    if (!(closingSpeed > 670.55999755859375f)) return;
    if (!(closingSpeed < 1788.159912109375f)) return;
    int mode = 1;
    int rA = cSSXAudio_GetRiderSpeechBit_i(passer);
    int passedBit = cSSXAudio_GetRiderSubjectBit_i(self, passed);
    int rB = cSSXAudio_GetRiderSpeechBit_i(passed);
    if (closingSpeed >= 1341.1199951171875f) mode = 2;
    unsigned sel = cSSXAudio_GetRiderRelation_0560(self, passer, passed);
    if (sel == 2) {
        closingSpeed = 0.0f;
        if (cSpeechSys_AddRequest_f(snd, 0, 8321, passer, cRider_getIdentity(passer), closingSpeed, 0, 0) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8321), 2, rA, mode);
        }
        if (cSpeechSys_AddRequest_f(snd, 0, 8319, passed, cRider_getIdentity(passed), closingSpeed, 0, 0) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8319), 2, rB, mode);
        }
    } else if (sel < 2) {
        closingSpeed = 0.0f;
        if (cSpeechSys_AddRequest_f(snd, 0, 8320, passer, cRider_getIdentity(passer), closingSpeed, 0, 0) != 0) {
            passedBit |= 0x1000;
            D_004A482C_5(SPCH_makeKey(1, 0, 8320), 3, rA, mode, passedBit);
        }
        if (cSpeechSys_AddRequest_f(snd, 0, 8317, passed, cRider_getIdentity(passed), closingSpeed, 0, 0) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8317), 2, rB, mode);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakRiderHit);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsCharacterSpeechOn(void* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cRider_getCheatIdentity(void*);
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
extern "C" int cSpeechSys_CanTakeSpeech(void* self, int i);
// cSpeechSys_AddRequest with the float argument in its real position (same registers).
int cSpeechSys_AddRequest_f(void* self, int a, int b, cRider* rider, int d, float f, int e,
        int refresh) __asm__("cSpeechSys_AddRequest");
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern "C" int cRider_getIdentity(void*);
extern "C" void cSSXAudio_PlayGrunt(cSSXAudio* self, cRider* rider, int a, int b);
// PORT: the unit defines cSSXAudio_GetRiderSpeechBit as void; it returns its tail call's result.
int cSSXAudio_GetRiderSpeechBit_i(void* self) __asm__("cSSXAudio_GetRiderSpeechBit");
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
// PORT: D_004A482C is called with 4 args here (the unit's later declaration has 3).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");
extern float D_004A36D8;
static inline int sndA30Mode(int m)
{
    return ScriptEngine->mPlayingNISList == m;
}

extern "C" int cSSXAudio_GetRiderRelation(cSSXAudio* self, cRider* a, cRider* b);

/**
 * Plays the victim's grunt for a rider-on-rider hit and, for a plain punch, the attacker's and victim's speech.
 *
 * Needs the Character volume on, no NIS playing, no replay, no cheat identity on either rider, neither past the finish
 * line and one of them the focus rider or a local human. Then @p victim grunts (cSSXAudio_PlayGrunt(self, b, 0, 1)). The
 * speech needs speech instance 0 ready, either rider within 30 units of a camera, and @p isBump and @p isHardHit both 0. By the
 * riders' relation (cSSXAudio_GetRiderRelation) it requests 8335 for @p victim and 8330 for @p attacker (2), 8318 and 8330 (1), or 8312 and 8334
 * (0); each queued request goes to the speech hook D_004A482C with the speaker's bit and 3. cComputer's bump, knockdown
 * and punch reactions call it.
 *
 * @param attacker  the attacking rider
 * @param victim  the rider that was hit
 * @param isBump  1 for a bump, 0 for a punch
 * @param isHardHit  1 when the hit knocks the rider down (or is the harder kind), else 0
 */
extern "C" void cSSXAudio_SpeakRiderHit(cSSXAudio* self, cRider* attacker, cRider* victim, int isBump, int isHardHit)
{
    if (cSSXAudio_IsCharacterSpeechOn(self) == 0) return;
    if (!sndA30Mode(2)) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cRider_getCheatIdentity(attacker) != 0) return;
    if (cRider_getCheatIdentity(victim) != 0) return;
    int posA = attacker->mFinishTime >= 0.0f;
    if (posA) return;
    int posB = victim->mFinishTime >= 0.0f;
    if (posB) return;
    int chkA = 0;
    if (attacker->isComputer() != 0 || attacker->mIsLocal == 0) chkA = 1;
    int chkB = 0;
    if (victim->isComputer() != 0 || victim->mIsLocal == 0) chkB = 1;
    void* pl = cSSXAudio_GetFocusRider_p(self, -1);
    if (chkA && chkB && attacker != pl && victim != pl) return;
    self->PlayGrunt((cRider*)victim, 0, 1);
    char* snd = (char*)&self->SpeechSys;
    if (cSpeechSys_CanTakeSpeech(snd, 0) == 0) return;
    int okB = cSSXAudio_CameraInRange(self, victim->getPosition(), D_004A36D8) >= 0;
    int okA = cSSXAudio_CameraInRange(self, attacker->getPosition(), D_004A36D8) >= 0;
    if (!okB && !okA) return;
    int rA = cSSXAudio_GetRiderSpeechBit_i(attacker);
    int rB = cSSXAudio_GetRiderSpeechBit_i(victim);
    if (isHardHit != 0) return;
    if (isBump != 0) return;
    int sel = cSSXAudio_GetRiderRelation(self, attacker, victim);
    if (sel == 2) {
        if (cSpeechSys_AddRequest_f(snd, 0, 8335, victim, cRider_getIdentity(victim), 0.0f, 0, 0) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8335), 2, rB, 3);
        }
        if (cSpeechSys_AddRequest_f(snd, 0, 8330, attacker, cRider_getIdentity(attacker), 0.0f, 0, 0) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8330), 2, rA, 3);
        }
    } else if (sel == 1) {
        if (cSpeechSys_AddRequest_f(snd, 0, 8318, victim, cRider_getIdentity(victim), 0.0f, 0, 0) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8318), 2, rB, 3);
        }
        if (cSpeechSys_AddRequest_f(snd, 0, 8330, attacker, cRider_getIdentity(attacker), 0.0f, 0, 0) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8330), 2, rA, 3);
        }
    } else if (sel == 0) {
        if (cSpeechSys_AddRequest_f(snd, 0, 8312, victim, cRider_getIdentity(victim), 0.0f, 0, 0) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8312), 2, rB, 3);
        }
        if (cSpeechSys_AddRequest_f(snd, 0, 8334, attacker, cRider_getIdentity(attacker), 0.0f, 0, 0) != 0) {
            D_004A482C_4(SPCH_makeKey(1, 0, 8334), 2, rA, 3);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakBump);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
extern "C" int cSpeechSys_CanTakeSpeech(void* self, int i);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern "C" int cRider_getIdentity(void*);
extern "C" void cSSXAudio_PlayGrunt(cSSXAudio* self, cRider* rider, int a, int b);
// PORT: the unit defines cSSXAudio_GetRiderSpeechBit as void; it returns its tail call's result.
int cSSXAudio_GetRiderSpeechBit_i(void* self) __asm__("cSSXAudio_GetRiderSpeechBit");
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern void (*D_004A482C)(void*, int, int);
extern float D_004A36D8;


/**
 * Plays a rider's bump grunt and, for a local human when asked, its bump speech line (8311).
 *
 * Needs the Character volume on, no replay, for a computer or non-local rider that it is the focus rider, a rider not
 * past the finish line, speech instance 0 ready and the rider within 30 units of a camera. Plays
 * cSSXAudio_PlayGrunt(self, rider, 2, 0); with @p play set and a local human rider it then requests speech 8311 and,
 * when queued, passes its packed id and the rider's speech bit to the speech hook D_004A482C.
 *
 * @param rider  the rider (cRider)
 * @param play  nonzero to also request the speech line
 */
extern "C" void cSSXAudio_SpeakBump(cSSXAudio* self, cRider* rider, int play)
{
    if (cSSXAudio_IsCharacterSpeechOn(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int chk = 0;
    if (rider->isComputer() != 0 || rider->mIsLocal == 0) chk = 1;
    if (chk && cSSXAudio_GetFocusRider_p(self, -1) != rider) return;
    float finishTime = rider->mFinishTime;
    float zero = 0.0f;
    int pos = finishTime >= zero;
    if (pos) return;
    char* snd = (char*)&self->SpeechSys;
    if (cSpeechSys_CanTakeSpeech(snd, 0) == 0) return;
    if (cSSXAudio_CameraInRange(self, rider->getPosition(), D_004A36D8) < 0) return;
    self->PlayGrunt((cRider*)rider, 2, 0);
    if (play == 0) return;
    int ok = rider->mIsHuman != 0 && rider->mIsLocal != 0;
    if (!ok) return;
    int speakerBit = cSSXAudio_GetRiderSpeechBit_i(rider);
    if (cSpeechSys_AddRequest(snd, 0, 8311, rider, cRider_getIdentity(rider), 0, 0, zero) == 0) return;
    D_004A482C(SPCH_makeKey(1, 0, 8311), 1, speakerBit);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_StopCharacterSpeech);
#ifdef SKIP_ASM
extern "C" int cSpeechSys_GetStreamId(void* self, int i);
extern "C" int cStreamSys_STREAM_GetCurrActiveSampleHandle(void* self, int i);
extern "C" cBankMonitorInstance* cBankMonitor_BANKMONITOR_GetHandle(void* self, int idx, int a2);
extern "C" void cSpeechSys_PurgeInstance(void*, int);

/**
 * Stops speech instance 0 when it is saying a line for the given character identity; returns whether nothing blocks.
 *
 * Maps speech instance 0's stream (cSpeechSys_GetStreamId) to a stream index (cStreamSys_STREAM_GetCurrActiveSampleHandle); none
 * (< 0) returns true. The bank monitor's voice for that index (cBankMonitor_BANKMONITOR_GetHandle(.., 3)) missing
 * returns false. When its mPtr equals @p id the speech is stopped (cSpeechSys_PurgeInstance) and true is returned, otherwise
 * false. cSSXAudio_PlayGrunt calls it.
 *
 * @param id  the character identity
 * @return true when no speech is playing or the character's speech was stopped, else false
 */
extern "C" bool cSSXAudio_StopCharacterSpeech(cSSXAudio* self, int id)
{
    int idx = cStreamSys_STREAM_GetCurrActiveSampleHandle(self->MusicSys.mpVStreamSys, cSpeechSys_GetStreamId((char*)&self->SpeechSys, 0));
    if (idx < 0) {
        return true;
    }
    cBankMonitorInstance* voice =
        cBankMonitor_BANKMONITOR_GetHandle(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, idx, 3);
    if (voice == 0) {
        return false;
    }
    if ((int)voice->mPtr == id) {
        cSpeechSys_PurgeInstance((char*)&self->SpeechSys, 0);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsCharacterSpeaking);
#ifdef SKIP_ASM
extern "C" int cSpeechSys_GetStreamId(void* self, int i);
extern "C" int cStreamSys_STREAM_GetCurrActiveSampleHandle(void* self, int i);
extern "C" cBankMonitorInstance* cBankMonitor_BANKMONITOR_GetHandle(void* self, int idx, int a2);

/**
 * Tells whether speech instance 0 is saying a line for the given character identity.
 *
 * Maps speech instance 0's stream (cSpeechSys_GetStreamId) to a stream index (cStreamSys_STREAM_GetCurrActiveSampleHandle) and
 * compares the mPtr of the bank monitor's voice for it (cBankMonitor_BANKMONITOR_GetHandle(.., 3)) with @p id; no
 * stream or no voice gives false. cSSXAudio_PlayGrunt, cSSXAudio_SwitchToNextSong and cSSXAudio_IsScriptedDJLinePlaying (with 10) use it.
 *
 * @param id  the character identity
 * @return true when that character's speech is playing, else false
 */
extern "C" bool cSSXAudio_IsCharacterSpeaking(cSSXAudio* self, int id)
{
    int idx = cStreamSys_STREAM_GetCurrActiveSampleHandle(self->MusicSys.mpVStreamSys, cSpeechSys_GetStreamId((char*)&self->SpeechSys, 0));
    if (idx < 0) {
        return false;
    }
    cBankMonitorInstance* voice =
        cBankMonitor_BANKMONITOR_GetHandle(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, idx, 3);
    if (voice == 0) {
        return false;
    }
    return (int)voice->mPtr == id;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakScriptEventType0);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// PORT: the unit defines cSSXAudio_GetRiderSpeechBit/cSSXAudio_GetRiderSubjectBit as void; they return their tail call's result.
int cSSXAudio_GetRiderSpeechBit_i(void* self) __asm__("cSSXAudio_GetRiderSpeechBit");
int cSSXAudio_GetRiderSubjectBit_i(void* self, void* obj) __asm__("cSSXAudio_GetRiderSubjectBit");
// PORT: D_004A482C is called with 5 args here (the unit's later declaration has 3).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");
extern float D_004A36D8;

// PORT: callers declare cSSXAudio_SpeakScriptEventType0 as (void*, int, int); the body takes (self, rider, obj).
void cSSXAudio_SpeakScriptEventType0_impl(cSSXAudio* self, cRider* rider, void* obj) __asm__("cSSXAudio_SpeakScriptEventType0");

/**
 * Requests speech line 8403 for a rider, about another rider (script speech event type 0).
 *
 * Needs the Character volume on and no replay; @p rider defaults to the cAI's rider 1 and @p obj to rider 0. The rider
 * must be within 30 units of a camera (position from getPosition). When the request is queued on speech
 * instance 0 (cSpeechSys_AddRequest), the speech hook D_004A482C gets its packed id, 3 arguments, the rider's speech bit
 * (cSSXAudio_GetRiderSpeechBit), @p obj's character bit (cSSXAudio_GetRiderSubjectBit) and 4. cSSXAudio_HandleSubjectSpeechEvent and cSSXAudio_PlayPendingDJLine call it.
 *
 * @param rider  the speaking rider, or 0 for rider 1
 * @param obj  the rider spoken about, or 0 for rider 0
 */
void cSSXAudio_SpeakScriptEventType0_impl(cSSXAudio* self, cRider* rider, void* obj)
{
    if (cSSXAudio_IsCharacterSpeechOn(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (rider == 0) rider = ((cAI*)cAudioInterface_getAI())->Riders[1];
    if (obj == 0) obj = ((cAI*)cAudioInterface_getAI())->Riders[0];
    if (cSSXAudio_CameraInRange(self, rider->getPosition(), D_004A36D8) < 0) return;
    if (cSpeechSys_AddRequest(&self->SpeechSys, 0, 8403, rider, cRider_getIdentity(rider), 0, 0, 0.0f) == 0) return;
    int speakerBit = cSSXAudio_GetRiderSpeechBit_i(rider);
    int subjectBit = cSSXAudio_GetRiderSubjectBit_i(self, obj);
    D_004A482C_5(SPCH_makeKey(1, 0, 8403), 3, speakerBit, subjectBit, 4);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakScriptEventType1);
#ifdef SKIP_ASM
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern "C" int cSSXAudio_CameraInRange(void* self, t4Vector* pos, float radius);
// PORT: the unit defines cSSXAudio_GetRiderSpeechBit/cSSXAudio_GetRiderSubjectBit as void; they return their tail call's result.
int cSSXAudio_GetRiderSpeechBit_i(void* self) __asm__("cSSXAudio_GetRiderSpeechBit");
int cSSXAudio_GetRiderSubjectBit_i(void* self, void* obj) __asm__("cSSXAudio_GetRiderSubjectBit");
// PORT: D_004A482C is called with 4 args here (the unit's later declaration has 3).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");
extern float D_004A36D8;

// PORT: callers declare cSSXAudio_SpeakScriptEventType1 as (void*, int, int); the body takes (self, rider, obj).
void cSSXAudio_SpeakScriptEventType1_impl(cSSXAudio* self, cRider* rider, void* obj) __asm__("cSSXAudio_SpeakScriptEventType1");

/**
 * Requests speech line 8404 for a rider, about another rider (script speech event type 1).
 *
 * Needs the Character volume on, no replay and @p rider within 30 units of a camera. When the request is queued on
 * speech instance 0 (cSpeechSys_AddRequest), the speech hook D_004A482C gets its packed id, the rider's speech bit
 * (cSSXAudio_GetRiderSpeechBit) and @p obj's character bit (cSSXAudio_GetRiderSubjectBit). cSSXAudio_HandleSubjectSpeechEvent calls it.
 *
 * @param rider  the speaking rider
 * @param obj  the rider spoken about
 */
void cSSXAudio_SpeakScriptEventType1_impl(cSSXAudio* self, cRider* rider, void* obj)
{
    if (cSSXAudio_IsCharacterSpeechOn(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cSSXAudio_CameraInRange(self, rider->getPosition(), D_004A36D8) < 0) return;
    if (cSpeechSys_AddRequest(&self->SpeechSys, 0, 8404, rider, cRider_getIdentity(rider), 0, 0, 0.0f) == 0) return;
    int speakerBit = cSSXAudio_GetRiderSpeechBit_i(rider);
    int subjectBit = cSSXAudio_GetRiderSubjectBit_i(self, obj);
    D_004A482C_4(SPCH_makeKey(1, 0, 8404), 2, speakerBit, subjectBit);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakPlaceCallback);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getAI();
extern "C" void cSSXAudio_SpeakPlace(void* self, cRider* rider);

/**
 * Monitor callback: requests the place speech line (cSSXAudio_SpeakPlace) for the rider whose index the monitor carries.
 *
 * The index is the monitor instance's +0x1C; one outside 0..NumberOfRidersAllocated - 1 uses rider 0. Its pointer to
 * member is D_004A36E8, which cSSXAudio_SpeakPAPlace and cSSXAudio_SpeakPARiderPlace pass to cMonitor_MONITOR_Create (cMonitor's create).
 *
 * @param obj  the monitor instance (cMonitorInstance)
 * @return 1
 */
extern "C" int cSSXAudio_SpeakPlaceCallback(cSSXAudio* self, cMonitorInstance* obj)
{
    int i = obj->mData1;
    cRider* rider;
    if (i >= 0 && i < ((cAI*)cAudioInterface_getAI())->NumberOfRidersAllocated) {
        rider = ((cAI*)cAudioInterface_getAI())->Riders[i];
    } else {
        rider = ((cAI*)cAudioInterface_getAI())->Riders[0];
    }
    cSSXAudio_SpeakPlace(self, rider);
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakPlace);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_GetRiderPlaceBit(void* self, cRider* rider);
// PORT: callers pass self to cSSXAudio_IsRivalEvent (the unit's later definition takes no args).
bool cSSXAudio_IsRivalEvent_s(void* self) __asm__("cSSXAudio_IsRivalEvent");

// PORT: callers declare cSSXAudio_SpeakPlace as (void*, int); the body takes (self, rider).
void cSSXAudio_SpeakPlace_impl(cSSXAudio* self, cRider* rider) __asm__("cSSXAudio_SpeakPlace");

/**
 * Requests a rider's place speech line (8377) with the place class from cSSXAudio_GetRiderPlaceBit.
 *
 * Needs the Character volume on, no replay and the rider within 30 units of a camera. When the request is queued on
 * speech instance 0, the speech hook D_004A482C gets its packed id, the rider's speech bit and a mode: 1 when the place
 * bit is 1 (first), 2 when cSSXAudio_IsRivalEvent is nonzero or in the game path kGAMEPATH_MULTIPLAYER (newRace.cGamePath), 4
 * for the bits 2 and 4, else 2. cSSXAudio_SpeakPlaceCallback, cSSXAudio_HandleRiderSpeechMessage and cSSXAudio_PlayPendingDJLine call it.
 *
 * @param rider  the rider (cRider)
 */
void cSSXAudio_SpeakPlace_impl(cSSXAudio* self, cRider* rider)
{
    if (cSSXAudio_IsCharacterSpeechOn(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cSSXAudio_CameraInRange(self, rider->getPosition(), D_004A36D8) < 0) return;
    if (cSpeechSys_AddRequest(&self->SpeechSys, 0, 8377, rider, cRider_getIdentity(rider), 0, 0, 0.0f) == 0) return;
    int speakerBit = cSSXAudio_GetRiderSpeechBit_i(rider);
    int k = cSSXAudio_GetRiderPlaceBit(self, rider);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int placeMode;
    if (k == 1) {
        placeMode = 1;
    } else if (cSSXAudio_IsRivalEvent_s(self) != 0) {
        placeMode = 2;
    } else if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        placeMode = 2;
    } else if (k == 2 || k == 4) {
        placeMode = 4;
    } else {
        placeMode = 2;
    }
    D_004A482C_4(SPCH_makeKey(1, 0, 8377), 2, speakerBit, placeMode);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakOffBalance);
#ifdef SKIP_ASM
extern "C" int cSpeechSys_CanTakeSpeech(void* self, int i);
extern void (*D_004A482C)(void*, int, int);

/**
 * Requests a rider's off-balance speech line (8376) during a press.
 *
 * Needs the Character volume on, no replay, for a computer or non-local rider that it is the focus rider, speech
 * instance 0 ready and the rider within 30 units of a camera. When queued, the speech hook D_004A482C gets its packed
 * id and the rider's speech bit. The press-balance handler in ai/ai.cpp calls it while the press depth (mSlewed[11]) is
 * above 0.8 (its doc calls it the Whooh speech).
 *
 * @param rider  the rider (cRider)
 */
extern "C" void cSSXAudio_SpeakOffBalance(cSSXAudio* self, cRider* rider)
{
    if (cSSXAudio_IsCharacterSpeechOn(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int check = 0;
    if (rider->isComputer() != 0 || rider->mIsLocal == 0) check = 1;
    if (check && cSSXAudio_GetFocusRider_p(self, -1) != rider) return;
    char* bm = (char*)&self->SpeechSys;
    if (cSpeechSys_CanTakeSpeech(bm, 0) == 0) return;
    if (cSSXAudio_CameraInRange(self, rider->getPosition(), D_004A36D8) < 0) return;
    if (cSpeechSys_AddRequest(bm, 0, 8376, rider, cRider_getIdentity(rider), 0, 0, 0.0f) == 0) return;
    int speakerBit = cSSXAudio_GetRiderSpeechBit_i(rider);
    D_004A482C(SPCH_makeKey(1, 0, 8376), 1, speakerBit);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakCharacterSelect);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" bool cSSXAudio_IsCharacterSpeechOn(void* self);
extern "C" int cSpeechSys_CanTakeSpeech(void* self, int i);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
// cSSXAudio_GetCharacterSpeechBit returns a value (the unit's earlier declaration says void).
int cSSXAudio_GetCharacterSpeechBit_r(int a, int b) __asm__("cSSXAudio_GetCharacterSpeechBit");
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

/**
 * Requests the character-select speech line (8380) for a character identity, outside the world-circuit game path.
 *
 * Needs the Character volume on, speech instance 0 ready and a game path (newRace.cGamePath) other than 0. The request has no
 * rider, the identity and refresh 1; when queued, the speech hook D_004A482C gets its packed id and the identity's
 * speech bit (cSSXAudio_GetCharacterSpeechBit(identity, 0)). The front-end rider manager calls it.
 *
 * @param identity  the character identity
 */
extern "C" void cSSXAudio_SpeakCharacterSelect(cSSXAudio* self, int identity)
{
    if (cSSXAudio_IsCharacterSpeechOn(self) != 0) {
        char* bm = (char*)&self->SpeechSys;
        if (cSpeechSys_CanTakeSpeech(bm, 0) != 0) {
            cBE_getInterface_Fv(cBE_getBE(), 0);
            if (cBENewRaceInterface_newRace.cGamePath != 0) {
                if (cSpeechSys_AddRequest(bm, 0, 8380, 0, identity, 0, 1, 0.0f) != 0) {
                    int speakerBit = cSSXAudio_GetCharacterSpeechBit_r(identity, 0);
                    D_004A482C(SPCH_makeKey(1, 0, 8380), 1, speakerBit);
                }
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakCustomize);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cSSXAudio_IsCharacterSpeechOn(void* self);
extern "C" int cSpeechSys_CanTakeSpeech(void* self, int i);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
// cSSXAudio_GetCharacterSpeechBit returns a value (the unit's earlier declaration says void).
int cSSXAudio_GetCharacterSpeechBit_r(int a, int b) __asm__("cSSXAudio_GetCharacterSpeechBit");
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

/**
 * Requests the customize speech line (8381) for a character identity.
 *
 * Needs the Character volume on and speech instance 0 ready. The request has no rider, the identity and refresh 1; when
 * queued, the speech hook D_004A482C gets its packed id and the identity's speech bit (cSSXAudio_GetCharacterSpeechBit(identity, 0)). The
 * front-end rider manager calls it for the equip screen's customize request.
 *
 * @param identity  the character identity
 */
extern "C" void cSSXAudio_SpeakCustomize(cSSXAudio* self, int identity)
{
    if (cSSXAudio_IsCharacterSpeechOn(self) != 0) {
        char* bm = (char*)&self->SpeechSys;
        if (cSpeechSys_CanTakeSpeech(bm, 0) != 0) {
            if (cSpeechSys_AddRequest(bm, 0, 8381, 0, identity, 0, 1, 0.0f) != 0) {
                int speakerBit = cSSXAudio_GetCharacterSpeechBit_r(identity, 0);
                D_004A482C(SPCH_makeKey(1, 0, 8381), 1, speakerBit);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetRiderRelation);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getAI();
void* cBEAggressionInterface_getThis();
extern "C" int cBEAggressionInterface_getAggression(void* ag, int a, int b);
extern "C" int cBEAggressionInterface_getState(void* ag, int a, int b);


/**
 * Returns the relation of two riders for their speech: 0 or 1 from their rivalry state, 2 for none.
 *
 * Gives 2 with only 2 riders allocated or when both are local humans. Otherwise, when both aggression levels
 * (cBEAggressionInterface_getAggression, each way between the riders' AI indices from cRider vtable slot 7) are below
 * 2, the state cBEAggressionInterface_getState(a, b) 0 or 1 is returned; anything else gives 2.
 *
 * @param firstRider  the first rider (the attacker or passer)
 * @param secondRider  the second rider
 * @return 0, 1 or 2
 */
extern "C" int cSSXAudio_GetRiderRelation(cSSXAudio* self, cRider* firstRider, cRider* secondRider)
{
    if (((cAI*)cAudioInterface_getAI())->NumberOfRidersAllocated == 2) return 2;
    int oka = firstRider->mIsHuman != 0 && firstRider->mIsLocal != 0;
    if (oka) {
        int okb = secondRider->mIsHuman != 0 && secondRider->mIsLocal != 0;
        if (okb) return 2;
    }
    int relation = 2;
    int aggressionAB = cBEAggressionInterface_getAggression(cBEAggressionInterface_getThis(), firstRider->getAIIndex(), secondRider->getAIIndex());
    int aggressionBA = cBEAggressionInterface_getAggression(cBEAggressionInterface_getThis(), secondRider->getAIIndex(), firstRider->getAIIndex());
    if (aggressionAB < 2 && aggressionBA < 2) {
        int rivalState = cBEAggressionInterface_getState(cBEAggressionInterface_getThis(), firstRider->getAIIndex(), secondRider->getAIIndex());
        if (rivalState == 0) relation = 0;
        else if (rivalState == 1) relation = 1;
    }
    return relation;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayScriptSpeechEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cSSXAudio_SpeakPAEventStart(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakPALine8385(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakPALine8386(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakPAResult(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJLine8392(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJLine8394(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJCurrentLocation(cSSXAudio* self, int a1);
extern "C" void cSSXAudio_SpeakDJLine8378(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakPALine8379(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakPARiderPlace(cSSXAudio* self, cRider* rider);
extern const sSndKey8 D_004A3648_19D8[] __asm__("D_004A3648");

/**
 * Plays a script-triggered speech event by id; returns whether the id is handled.
 *
 * 0: cSSXAudio_SpeakPAEventStart, and when no song is playing (MusicSys.mSongPlaying) cSSXAudio_PickRandomSong and a 10 ms monitor
 * (cMonitor_MONITOR_Create) with the callback D_004A3648 and data (0, 3, 1, 0, 0); 1: cSSXAudio_SpeakPALine8385; 2: cSSXAudio_SpeakPALine8386; 3:
 * cSSXAudio_SpeakPALine8387; 5: cSSXAudio_SpeakPARiderPlace for rider 0; 7: cSSXAudio_SpeakPAResult; 8: cSSXAudio_SpeakDJLine8392; 10: cSSXAudio_SpeakDJLine8394; 12:
 * cSSXAudio_SpeakDJCurrentLocation(self, -1); 14: cSSXAudio_SpeakDJLine8378; 15: cSSXAudio_SpeakPALine8379. The script engine's speech events call it.
 *
 * @param ev  the event id
 * @return true when handled, else false
 */
// PORT: 64-bit long sound key
extern "C" bool cSSXAudio_PlayScriptSpeechEvent(cSSXAudio* self, int ev)
{
    if (ev == 0) {
        cSSXAudio_SpeakPAEventStart(self);
        if (self->MusicSys.mSongPlaying == 0) {
            cSSXAudio_PickRandomSong(self);
            cMonitor_MONITOR_Create(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                    10, D_004A3648_19D8[0].v, 0, 3, 1, 0, 0);
        }
    } else if (ev == 1) {
        cSSXAudio_SpeakPALine8385(self);
    } else if (ev == 2) {
        cSSXAudio_SpeakPALine8386(self);
    } else if (ev == 3) {
        cSSXAudio_SpeakPALine8387(self);
    } else if (ev == 7) {
        cSSXAudio_SpeakPAResult(self);
    } else if (ev == 8) {
        cSSXAudio_SpeakDJLine8392(self);
    } else if (ev == 10) {
        cSSXAudio_SpeakDJLine8394(self);
    } else if (ev == 12) {
        cSSXAudio_SpeakDJCurrentLocation(self, -1);
    } else if (ev == 14) {
        cSSXAudio_SpeakDJLine8378(self);
    } else if (ev == 15) {
        cSSXAudio_SpeakPALine8379(self);
    } else if (ev == 5) {
        cSSXAudio_SpeakPARiderPlace(self, (cRider*)((char*)((cAI*)cAudioInterface_getAI())->Riders[0]));
    } else {
        return false;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleRiderSpeechMessage);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_SpeakPlace(void* self, cRider* rider);

/**
 * Handles a rider's anim speech message: 2 requests the place line (cSSXAudio_SpeakPlace) and returns true, others return
 * false.
 *
 * cRider sends it the event ids below 0x64.
 *
 * @param msg  the message id
 * @param rider  the rider
 * @return true when handled, else false
 */
extern "C" bool cSSXAudio_HandleRiderSpeechMessage(cSSXAudio* self, int msg, cRider* rider)
{
    if (msg == 2) {
        cSSXAudio_SpeakPlace(self, rider);
        return true;
    }
    return false;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleSubjectSpeechEvent);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_SpeakScriptEventType0(void* self, int a, int b);
extern "C" void cSSXAudio_SpeakScriptEventType1(void* self, int a, int b);

/**
 * Handles a script speech event about a subject rider: type 0 plays cSSXAudio_SpeakScriptEventType0, type 1 cSSXAudio_SpeakScriptEventType1.
 *
 * cRider passes the rider and the event's subject rider (cSSXScriptEngine_GetSubjectRider).
 *
 * @param type  the event type
 * @param rider  the speaking rider (as int)
 * @param otherRider  the subject rider (as int)
 * @return true when handled, else false
 */
extern "C" bool cSSXAudio_HandleSubjectSpeechEvent(cSSXAudio* self, int type, int rider, int otherRider)
{
    if (type == 0) {
        cSSXAudio_SpeakScriptEventType0(self, rider, otherRider);
        return true;
    }
    if (type == 1) {
        cSSXAudio_SpeakScriptEventType1(self, rider, otherRider);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1BD0__FPv);
#ifdef SKIP_ASM
/**
 * Returns 1; cRider_OnEndObjectUse and cScriptObjEvent_OnEndObjectUse call it when speech events fired during an NIS.
 *
 * @return 1
 */
int func_002A1BD0(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetLocationSpeechBit);
#ifdef SKIP_ASM
/**
 * Returns the bit of a location for the location-dependent speech; the combined locations 17..21 also need a part.
 *
 * Locations 0..4 give 0x1..0x10 and 5..16 give 0x40..0x20000 (0x20 is skipped). For 17..21 @p subLocation picks the part (14, 15,
 * 16 or 17): 17 gives 0x1 / 0x40 / 0x200 for 14 / 15 / 16, 18 gives 0x2 / 0x1000 for 14 / 17, 19 gives 0x4 / 0x400 /
 * 0x2000 for 14 / 16 / 17, 20 gives 0x8 / 0x80 for 14 / 15, and 21 gives 0x10 / 0x100 / 0x800 / 0x4000 for 14 / 15 / 16
 * / 17. Anything else, or @p subLocation of -1, gives 0.
 *
 * @param location  the location id
 * @param subLocation  the part of a combined location, or -1
 * @return the bit, or 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cSSXAudio_GetLocationSpeechBit(cSSXAudio* self, int location, int subLocation)
{
    switch (location) {
    case 0: return 0x1;
    case 1: return 0x2;
    case 2: return 0x4;
    case 3: return 0x8;
    case 4: return 0x10;
    case 5: return 0x40;
    case 6: return 0x80;
    case 7: return 0x100;
    case 8: return 0x200;
    case 9: return 0x400;
    case 10: return 0x800;
    case 11: return 0x1000;
    case 12: return 0x2000;
    case 13: return 0x4000;
    case 14: return 0x8000;
    case 15: return 0x10000;
    case 16: return 0x20000;
    }
    if (subLocation != -1) {
        if (location == 17) {
            if (subLocation == 0xE) return 0x1;
            if (subLocation == 0xF) return 0x40;
            if (subLocation == 0x10) return 0x200;
        } else if (location == 18) {
            if (subLocation == 0xE) return 0x2;
            if (subLocation == 17) return 0x1000;
        } else if (location == 19) {
            if (subLocation == 0xE) return 0x4;
            if (subLocation == 0x10) return 0x400;
            if (subLocation == 17) return 0x2000;
        } else if (location == 20) {
            if (subLocation == 0xE) return 0x8;
            if (subLocation == 0xF) return 0x80;
        } else if (location == 21) {
            if (subLocation == 0xE) return 0x10;
            if (subLocation == 0xF) return 0x100;
            if (subLocation == 0x10) return 0x800;
            if (subLocation == 17) return 0x4000;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetRiderSubjectBit);
#ifdef SKIP_ASM
extern "C" int cRider_getIdentity(void*);
extern "C" int cRider_getCheatIdentity(void*);
extern "C" void cSSXAudio_GetSubjectCharacterBit(void* self, int a, int b);

/**
 * Returns the speech character bit of a rider: cSSXAudio_GetSubjectCharacterBit of its identity and cheat identity (declared void).
 *
 * @param obj  the rider (cRider)
 */
extern "C" void cSSXAudio_GetRiderSubjectBit(void* self, void* obj)
{
    int identity = cRider_getIdentity(obj);
    cSSXAudio_GetSubjectCharacterBit(self, identity, cRider_getCheatIdentity(obj));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetSubjectCharacterBit);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: the unit declares this as void(void*, int, int); defined under an asm label with its real type
extern "C" int cSSXAudio_GetSubjectCharacterBit_impl(void* self, unsigned int idx, int flag) __asm__("cSSXAudio_GetSubjectCharacterBit");
/**
 * Returns the character bit of an identity used for the rider spoken about, or 0x1000 for a cheat identity.
 *
 * With @p flag 0: kCHAR_ELISE gives 0x1, kCHAR_MAC 0x2, kCHAR_MOBY 0x4, kCHAR_PSYMON 0x8, kCHAR_KAORI 0x10, kCHAR_ZOE
 * 0x20, kCHAR_VIGGO 0x40, kCHAR_NATE 0x80, kCHAR_GRIFF 0x100, kCHAR_ALLEGRA 0x200 (unlike cSSXAudio_GetCharacterSpeechBit, kCHAR_VIGGO
 * and kCHAR_NATE swap bits); anything else gives 0x1000. The unit declares it void(void*, int, int).
 *
 * @param idx  the character identity
 * @param flag  the cheat identity; nonzero gives 0x1000
 * @return the character's bit
 */
extern "C" int cSSXAudio_GetSubjectCharacterBit_impl(void* self, unsigned int idx, int flag)
{
    if (flag == 0) {
        switch (idx) {
        case kCHAR_ELISE: return 1;
        case kCHAR_MAC: return 2;
        case kCHAR_MOBY: return 4;
        case kCHAR_PSYMON: return 8;
        case kCHAR_KAORI: return 0x10;
        case kCHAR_ZOE: return 0x20;
        case kCHAR_VIGGO: return 0x40;
        case kCHAR_NATE: return 0x80;
        case kCHAR_GRIFF: return 0x100;
        case kCHAR_ALLEGRA: return 0x200;
        }
    }
    return 0x1000;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetLocationPeakBit);
#ifdef SKIP_ASM
/**
 * Returns the peak bit (ePeakBit) of a location: kPEAKBIT_Peak1, kPEAKBIT_Peak2 or kPEAKBIT_Peak3, kPEAKBIT_None for
 * unknown ids.
 *
 * Peak 1: ARA1, BRA2, ASS1, ABA1, BHP1, ABC1, HUB_A, HUB_B; peak 2: CRA3, DRA4, DSS2, CBA2, CHP2, DBC2, HUB_C, HUB_D;
 * peak 3: ERA5, ESS3, EBA3, EHP3, EBC3, HUB_E (the *BC* locations are the peaks' back-country).
 *
 * @param x  the location id
 * @return the peak bit, or kPEAKBIT_None
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int x)
{
    switch (x) {
    case kLOCATION_ARA1: case kLOCATION_BRA2: case kLOCATION_ASS1: case kLOCATION_ABA1: case kLOCATION_BHP1:
    case kLOCATION_ABC1: case kLOCATION_HUB_A: case kLOCATION_HUB_B:
        return kPEAKBIT_Peak1;
    case kLOCATION_CRA3: case kLOCATION_DRA4: case kLOCATION_DSS2: case kLOCATION_CBA2: case kLOCATION_CHP2:
    case kLOCATION_DBC2: case kLOCATION_HUB_C: case kLOCATION_HUB_D:
        return kPEAKBIT_Peak2;
    case kLOCATION_ERA5: case kLOCATION_ESS3: case kLOCATION_EBA3: case kLOCATION_EHP3: case kLOCATION_EBC3:
    case kLOCATION_HUB_E:
        return kPEAKBIT_Peak3;
    }
    return kPEAKBIT_None;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetRiderPlaceBit);
#ifdef SKIP_ASM
extern int cGameModeMan_iRanking[];

/**
 * Returns a rider's place in the race as a bit (1 first, 2 second, ...), for the place speech.
 *
 * In the game path kGAMEPATH_MULTIPLAYER (newRace.cGamePath): when cSSXAudio_IsRaceEvent is nonzero, the rider's RaceRanking
 * (its place) 0..4 gives 1, 2, 4, 8, 0x10 and anything else 0x200; otherwise 1 for place 0 and 2 for the rest. In the
 * other paths rider 0's rank cGameModeMan_iRanking[0] gives 0: 1, 1: 2, 2: 4, 3: 8, 4: 0x10, 5: 0x200, 6: 0x40, 7:
 * 0x80, 8: 0x100, 9: 0x200, else 0 (the @p obj rider is not used there).
 *
 * @param obj  the rider (cRider)
 * @return the place bit
 */
extern "C" int cSSXAudio_GetRiderPlaceBit(void *self, cRider* obj)
{
    int placeBit;
    int racePlace;

    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        if (cSSXAudio_IsRaceEvent(self) != 0) {
            racePlace = obj->RaceRanking;
            if (racePlace == 0) {
                placeBit = 1;
            } else if (racePlace == 1) {
                placeBit = 2;
            } else if (racePlace == 2) {
                placeBit = 4;
            } else if (racePlace == 3) {
                placeBit = 8;
            } else {
                placeBit = (racePlace == 4) ? 0x10 : 0x200;
            }
        } else {
            racePlace = obj->RaceRanking;
            placeBit = (racePlace == 0) ? 1 : 2;
        }
        return placeBit;
    } else {
        // Retail selects on a condition the compiler had folded to 1 (`li 1; movn` in case 5): a local flag that is
        // constant here, e.g. a feature toggle. Evaluating `mode != 2` instead gives a run-time xori (99.00).
        int notTwo = 1;
        switch (cGameModeMan_iRanking[0]) {
        case 0:
            placeBit = 1;
            break;
        case 1:
            placeBit = 2;
            break;
        case 2:
            placeBit = 4;
            break;
        case 3:
            placeBit = 8;
            break;
        case 4:
            placeBit = 0x10;
            break;
        case 5:
            placeBit = notTwo ? 0x200 : 0x20;
            break;
        case 6:
            placeBit = 0x40;
            break;
        case 7:
            placeBit = 0x80;
            break;
        case 8:
            placeBit = 0x100;
            break;
        case 9:
            placeBit = 0x200;
            break;
        default:
            placeBit = 0;
            break;
        }
    }
    return placeBit;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GetRiderCharacterGroup);
#ifdef SKIP_ASM
/**
 * Returns a character group of a rider for speech: 2 for kCHAR_KAORI, kCHAR_ALLEGRA, kCHAR_ZOE and kCHAR_ELISE, 1 for
 * the other six characters, else 0.
 *
 * A rider with a cheat identity gives 0. (By cSSXAudio_LoadGruntBanks's grunt banks 1, 2, 4, 6 are kao, ari, zoe, eli: the women.)
 *
 * @param obj  the rider (cRider)
 * @return 1, 2 or 0
 */
extern "C" int cSSXAudio_GetRiderCharacterGroup(void *self, void *obj)
{
    if (cRider_getCheatIdentity(obj) == 0) {
        switch (cRider_getIdentity(obj)) {
        case kCHAR_KAORI:
        case kCHAR_ALLEGRA:
        case kCHAR_ZOE:
        case kCHAR_ELISE:
            return 2;
        case kCHAR_MOBY:
        case kCHAR_MAC:
        case kCHAR_GRIFF:
        case kCHAR_NATE:
        case kCHAR_PSYMON:
        case kCHAR_VIGGO:
            return 1;
        default:
            return 0;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJLine8392);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8392 with the language and the location's peak.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn). The request goes to speech instance 0 with identity 10 (the DJ,
 * cSpeechSys_AddRequest); when queued, the speech hook D_004A482C gets its packed id (SPCH_makeKey(1, 0, 8392)), the language
 * bit (cSSXAudio_GetSpeechLanguage) and the peak bit of the location (cSSXAudio_GetLocationPeakBit). cSSXAudio_PlayScriptSpeechEvent (event 8) and cSSXAudio_SpeakDJBetweenEvents (pick
 * 2) call it.
 */
extern "C" void cSSXAudio_SpeakDJLine8392(cSSXAudio* self)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8392, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int peakBit = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_4(SPCH_makeKey(1, 0, 8392), 2, languageBit, peakBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJLine8393);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8393 with the language and the location's peak.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit (cSSXAudio_GetSpeechLanguage) and the peak bit (cSSXAudio_GetLocationPeakBit).
 * cSSXAudio_SpeakDJBetweenEvents calls it (pick 7).
 */
extern "C" void cSSXAudio_SpeakDJLine8393(cSSXAudio* self)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8393, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int peakBit = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_4(SPCH_makeKey(1, 0, 8393), 2, languageBit, peakBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJLine8394);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getAI();
// cSSXAudio_GetRiderSubjectBit returns its tail call's value (the unit defines it as void).
int cSSXAudio_GetRiderSubjectBit_r(void* self, void* obj) __asm__("cSSXAudio_GetRiderSubjectBit");
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8394 about rider 0's character, with the language and the location's peak.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit, rider 0's character bit (cSSXAudio_GetRiderSubjectBit) and the peak
 * bit. cSSXAudio_PlayScriptSpeechEvent (event 10) and cSSXAudio_SpeakDJBetweenEvents (pick 0) call it.
 */
extern "C" void cSSXAudio_SpeakDJLine8394(cSSXAudio* self)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8394, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int riderBit = cSSXAudio_GetRiderSubjectBit_r(self, ((cAI*)cAudioInterface_getAI())->Riders[0]);
            int peakBit = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_5(SPCH_makeKey(1, 0, 8394), 3, languageBit, riderBit, peakBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJLine8423);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8423 with the language and the location's peak.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and the peak bit. cSSXAudio_SpeakDJBetweenEvents calls it (pick 5).
 */
extern "C" void cSSXAudio_SpeakDJLine8423(cSSXAudio* self)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8423, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int peakBit = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_4(SPCH_makeKey(1, 0, 8423), 2, languageBit, peakBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJLine8424);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8424 with the language and the location's peak.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and the peak bit. cSSXAudio_SpeakDJBetweenEvents calls it (pick 6).
 */
extern "C" void cSSXAudio_SpeakDJLine8424(cSSXAudio* self)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8424, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int peakBit = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_4(SPCH_makeKey(1, 0, 8424), 2, languageBit, peakBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJLine8425);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

// cSSXAudio_SpeakDJLine8425's location argument for the current location (name ours). [U] it then reads the location from the
// new-race interface (cBENewRaceInterface_getLocation).
enum { kCurrentLocation = -1 };

/**
 * Requests DJ speech line 8425 with the language and the peak of a location (the current one for kCurrentLocation).
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and the peak bit (cSSXAudio_GetLocationPeakBit) of @p mode, or of the
 * current location when @p mode is -1. cSSXAudio_SpeakDJBetweenEvents (pick 1) and cSSXAudio_SpeakDJEventResult (a back-country location) call it.
 *
 * @param mode  a location id, or -1 for the current location
 */
extern "C" void cSSXAudio_SpeakDJLine8425(cSSXAudio* self, int mode)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8425, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int peakBit;
            if (mode == kCurrentLocation) {
                peakBit = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            } else {
                peakBit = cSSXAudio_GetLocationPeakBit(self, mode);
            }
            D_004A482C_4(SPCH_makeKey(1, 0, 8425), 2, languageBit, peakBit);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJLocation);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" int cSSXAudio_GetLocationSpeechBit(cSSXAudio* self, int a, int b);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8396 about a location, with mode 3.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit, the location bit cSSXAudio_GetLocationSpeechBit(@p location, -1) and 3.
 * cSSXAudio_SpeakDJEventResult calls it.
 *
 * @param location  the location id
 */
extern "C" void cSSXAudio_SpeakDJLocation(cSSXAudio* self, int location)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8396, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int locationBit = cSSXAudio_GetLocationSpeechBit(self, location, -1);
            D_004A482C_5(SPCH_makeKey(1, 0, 8396), 3, languageBit, locationBit, 3);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJCurrentLocation);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationSpeechBit(cSSXAudio* self, int a, int b);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8396 about the current location (or a part of a combined location), with mode 3.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit, the bit cSSXAudio_GetLocationSpeechBit(current location, @p subLocation) and 3.
 * cSSXAudio_PlayScriptSpeechEvent (event 12, with -1) and cSSXAudio_PlayPendingDJLine (with mPendingLocationPart) call it.
 *
 * @param subLocation  the part of a combined location (cSSXAudio_GetLocationSpeechBit), or -1
 */
extern "C" void cSSXAudio_SpeakDJCurrentLocation(cSSXAudio* self, int subLocation)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8396, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int locationBit = cSSXAudio_GetLocationSpeechBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)), subLocation);
            D_004A482C_5(SPCH_makeKey(1, 0, 8396), 3, languageBit, locationBit, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJLine8378);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8378 with the language and the location's peak.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and the peak bit. cSSXAudio_PlayScriptSpeechEvent (event 14) and
 * cSSXAudio_PlayPendingDJLine (pending flag mPendingPeakLine8378) call it.
 */
extern "C" void cSSXAudio_SpeakDJLine8378(cSSXAudio* self)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8378, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int peakBit = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_4(SPCH_makeKey(1, 0, 8378), 2, languageBit, peakBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJChoiceLine);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" void cSpeechSys_PurgeInstance(void*, int);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

/**
 * Interrupts speech instance 0 and requests DJ speech line 8421 with two yes/no choices.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn). Stops speech instance 0 (cSpeechSys_PurgeInstance), then requests 8421 with identity
 * 10; when queued, clears mChoiceLineFirstTime and the speech hook D_004A482C gets its packed id, the language bit, 1
 * when @p firstChoice is set (else 2) and 2 when @p secondChoice is set (else 1). The music code
 * (cSSXAudio_StartEventWithNoStartGate, cSSXAudio_HandleMusicRequest, cSSXAudio_HandleMusicEvent), cSSXAudio_StartDJIntro and cSSXAudio_PlayPendingDJLine call it, mostly
 * with (0, 0).
 *
 * @param firstChoice  the first choice
 * @param secondChoice  the second choice
 */
extern "C" void cSSXAudio_SpeakDJChoiceLine(cSSXAudio* self, int firstChoice, int secondChoice)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        char* bm = (char*)&self->SpeechSys;
        cSpeechSys_PurgeInstance(bm, 0);
        if (cSpeechSys_AddRequest(bm, 0, 8421, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int x = 2;
            if (firstChoice) x = 1;
            if (self->mChoiceLineFirstTime != 0) {
                self->mChoiceLineFirstTime = 0;
            }
            int y = 2;
            if (!secondChoice) y = 1;
            D_004A482C_5(SPCH_makeKey(1, 0, 8421), 3, languageBit, x, y);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJLine8422);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

/**
 * Requests DJ speech line 8422 with the language.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id and the language bit. cSSXAudio_PlayPendingDJLine calls it for the pending flag
 * mPendingLine8422.
 */
extern "C" void cSSXAudio_SpeakDJLine8422(cSSXAudio* self)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8422, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            D_004A482C(SPCH_makeKey(1, 0, 8422), 1, languageBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJSongAnnouncement);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 6-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_6)(void*, int, int, int, int, int) __asm__("D_004A482C");

/**
 * Requests the DJ's song announcement (speech 8399) for a song number.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, 4 arguments: the language bit, 3, and the song as one bit in two words: 1
 * << n in the first for n below 100, 1 << (n - 100) in the second from 100 on, none for a negative n or 999.
 * cSSXAudio_PlayPendingDJLine calls it with mSongAnnounceNum (the song value cSSXAudio_CountdownGateDrop and cSSXAudio_StartDJIntro store from
 * cMusicSys_GetCurrentSongSedValue).
 *
 * @param n  the song number, or -1 / 999 for none
 */
extern "C" void cSSXAudio_SpeakDJSongAnnouncement(cSSXAudio* self, int n)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8399, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int lo = 0;
            int hi = 0;
            if (n >= 0 && n != 999) {
                if (n < 100) {
                    lo = 1 << n;
                } else {
                    n -= 100;
                    hi = 1 << n;
                }
            }
            D_004A482C_6(SPCH_makeKey(1, 0, 8399), 4, languageBit, 3, lo, hi);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJWinner);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getAI();
// cSSXAudio_GetRiderSubjectBit returns its tail call's value (the unit defines it as void).
int cSSXAudio_GetRiderSubjectBit_r(void* self, void* obj) __asm__("cSSXAudio_GetRiderSubjectBit");
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8450 about rider 0's character, with the language and the location's peak.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit, rider 0's character bit (cSSXAudio_GetRiderSubjectBit) and the peak
 * bit. cSSXAudio_SpeakDJEventResult calls it when rider 0 finished first.
 */
extern "C" void cSSXAudio_SpeakDJWinner(cSSXAudio* self)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8450, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int riderBit = cSSXAudio_GetRiderSubjectBit_r(self, ((cAI*)cAudioInterface_getAI())->Riders[0]);
            int peakBit = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_5(SPCH_makeKey(1, 0, 8450), 3, languageBit, riderBit, peakBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJRival);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getRival(void*);
// cSSXAudio_GetSubjectCharacterBit returns a value (the unit declares it void).
int cSSXAudio_GetSubjectCharacterBit_r(void* self, int a, int b) __asm__("cSSXAudio_GetSubjectCharacterBit");
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8459 about the race's rival character.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and the character bit (cSSXAudio_GetSubjectCharacterBit) of
 * cBENewRaceInterface_getRival. cSSXAudio_SpeakDJBetweenEvents calls it (pick 3).
 */
extern "C" void cSSXAudio_SpeakDJRival(cSSXAudio* self)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8459, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int rivalBit = cSSXAudio_GetSubjectCharacterBit_r(self, cBENewRaceInterface_getRival(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)), 0);
            D_004A482C_4(SPCH_makeKey(1, 0, 8459), 2, languageBit, rivalBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJLine8491);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8491 with the language and the location's peak.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and the peak bit. cSSXAudio_PlayPendingDJLine calls it for the pending
 * flag mPendingPeakLine8491.
 */
extern "C" void cSSXAudio_SpeakDJLine8491(cSSXAudio* self)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8491, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int peakBit = cSSXAudio_GetLocationPeakBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_4(SPCH_makeKey(1, 0, 8491), 2, languageBit, peakBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJLine8492);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8492, telling it whether it was played before (mLine8492Played), and marks it played.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and mLine8492Played. mLine8492Played is then set to 1
 * even when the request was not queued. cSSXAudio_SpeakDJBetweenEvents (pick 4) and cSSXAudio_PlayPendingDJLine (pending flag mPendingLine8492) call
 * it.
 */
extern "C" void cSSXAudio_SpeakDJLine8492(cSSXAudio* self)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest(&self->SpeechSys, 0, 8492, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int playedBefore = self->mLine8492Played;
            D_004A482C_4(SPCH_makeKey(1, 0, 8492), 2, languageBit, playedBefore);
        }
        self->mLine8492Played = 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJLine8493);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationSpeechBit(cSSXAudio* self, int a, int b);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8493 about the current location.
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and the location bit cSSXAudio_GetLocationSpeechBit(location, -1).
 * cSSXAudio_PlayPendingDJLine calls it for the pending flag mPendingLocationLine.
 */
extern "C" void cSSXAudio_SpeakDJLine8493(cSSXAudio* self)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8493, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int locationBit = cSSXAudio_GetLocationSpeechBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)), -1);
            D_004A482C_4(SPCH_makeKey(1, 0, 8493), 2, languageBit, locationBit);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJFragsResult);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" int cSSXAudio_GetLocationSpeechBit(cSSXAudio* self, int a, int b);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8494 about a location (rider 0's frags after an event).
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and the location bit cSSXAudio_GetLocationSpeechBit(@p location, -1).
 * cSSXAudio_SpeakDJEventResult calls it for the frag result (5 or more frags).
 *
 * @param location  the location id
 */
extern "C" void cSSXAudio_SpeakDJFragsResult(cSSXAudio* self, int location)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8494, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int locationBit = cSSXAudio_GetLocationSpeechBit(self, location, -1);
            D_004A482C_4(SPCH_makeKey(1, 0, 8494), 2, languageBit, locationBit);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJUberTricksResult);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cSSXAudio_IsDJSpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" int cSSXAudio_GetLocationSpeechBit(cSSXAudio* self, int a, int b);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests DJ speech line 8495 about a location (rider 0's uber tricks after an event).
 *
 * Needs DJ speech allowed (cSSXAudio_IsDJSpeechOn); the request goes to speech instance 0 with identity 10 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and the location bit cSSXAudio_GetLocationSpeechBit(@p location, -1).
 * cSSXAudio_SpeakDJEventResult calls it for the uber-trick result.
 *
 * @param location  the location id
 */
extern "C" void cSSXAudio_SpeakDJUberTricksResult(cSSXAudio* self, int location)
{
    if (cSSXAudio_IsDJSpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8495, 0, 10, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int locationBit = cSSXAudio_GetLocationSpeechBit(self, location, -1);
            D_004A482C_4(SPCH_makeKey(1, 0, 8495), 2, languageBit, locationBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJBetweenEvents);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" bool cSSXAudio_SpeakDJEventResult(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJLine8394(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJLine8425(cSSXAudio* self, int a1);
extern "C" void cSSXAudio_SpeakDJLine8392(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJRival(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJLine8492(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJLine8423(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJLine8424(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJLine8393(cSSXAudio* self);
extern "C" int cSSXAudio_GetLocationPeakBit(cSSXAudio* self, unsigned int a1);
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cAudioInterface_getBE();
extern "C" unsigned int cSoundSys_SndRand(void* rng);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
// PORT: declared signed char elsewhere; the caller here sign-extends an int result itself.
int cBENewPlayerInterface_getRiderCharID_i(void* self, int riderIndex) __asm__("cBENewPlayerInterface_getRiderCharID__FPvi");
int cBERewardInterface_getTrackMedal(void* self, int a1, int ch, int a3, int track);
void cSpeechSys_StartNextSpeech(void* snd, int a1);

/**
 * Picks and plays the DJ's next between-events line, avoiding lines already used.
 *
 * The first time (mPeakIntroDone and mBonusLineQueued both 0) it sets mPeakIntroDone, queues pick 3 for next time
 * (mQueuedBetweenEventsPick), marks pick 7 used in the mask mUsedBetweenEventsPicks, clears mFirstTimeOnPeak and plays
 * pick 0. Otherwise it returns at once when cSSXAudio_SpeakDJEventResult played an event-result line; a queued pick is used next (when
 * mBonusLineQueued is 0); else one of the first 8 picks (5 with
 * @p bonus) not yet set in mUsedBetweenEventsPicks is chosen at random (cSoundSys_SndRand). When all are used the mask
 * is cleared, and bit 3 is pre-set when the human rider's character holds both the medal 4 and 5 of the peak's track
 * 14, 15 or 16 (cBERewardInterface_getTrackMedal). The pick is marked in mUsedBetweenEventsPicks and played: 0
 * cSSXAudio_SpeakDJLine8394, 1 cSSXAudio_SpeakDJLine8425(-1), 2 cSSXAudio_SpeakDJLine8392, 3 cSSXAudio_SpeakDJRival, 4 cSSXAudio_SpeakDJLine8492, 5 cSSXAudio_SpeakDJLine8423, 6
 * cSSXAudio_SpeakDJLine8424, 7 cSSXAudio_SpeakDJLine8393. Finally cSpeechSys_StartNextSpeech(SpeechSys, 0). cSSXAudio_PlayPendingDJLine and cSSXAudio_SpeakDJEventResult call it.
 *
 * @param vself  the audio system
 * @param bonus  nonzero to choose among the first 5 lines only
 */
extern "C" void cSSXAudio_SpeakDJBetweenEvents(cSSXAudio* vself, int bonus)
{
    int pick = 0;
    if (vself->mPeakIntroDone == 0 && vself->mBonusLineQueued == 0) {
        vself->mPeakIntroDone = 1;
        vself->mQueuedBetweenEventsPick = 3;
        vself->mUsedBetweenEventsPicks |= 0x80;
        vself->mFirstTimeOnPeak = 0;
    } else {
        if (cSSXAudio_SpeakDJEventResult(vself) != 0) {
            return;
        }
        int queued = vself->mQueuedBetweenEventsPick;
        if (queued != -1 && vself->mBonusLineQueued == 0) {
            pick = queued;
            vself->mQueuedBetweenEventsPick = -1;
        } else {
            int n = bonus == 0 ? 8 : 5;
            int left = 0;
            for (int i = 0; i < n; i++) {
                left += ((vself->mUsedBetweenEventsPicks >> i) ^ 1) & 1;
            }
            if (left == 0) {
                vself->mUsedBetweenEventsPicks = (int)0;
                left = n;
                int track = 0xE;
                void* players = cBE_getInterface_Fv(cBE_getBE(), 1);
                signed char ch = cBENewPlayerInterface_getRiderCharID_i(players, cBENewPlayerInterface_getHumanRider(players, 0));
                int sel = cSSXAudio_GetLocationPeakBit((cSSXAudio*)(((char*)vself)), *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
                if (sel != 1) {
                    track = sel == 2 ? 0xF : 0x10;
                }
                void* rewards = cBE_getInterface_Fv(cBE_getBE(), 13);
                int gold = cBERewardInterface_getTrackMedal(rewards, 0, ch, 4, track) != -1;
                int plat = cBERewardInterface_getTrackMedal(rewards, 0, ch, 5, track) != -1;
                if (gold && plat) {
                    vself->mUsedBetweenEventsPicks |= 8;
                }
            }
            int nthUnplayed =
                (cSoundSys_SndRand(vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys) & 0x7FFF)
                * left / 0x7FFF + 1;
            int cnt = 0;
            while (cnt < nthUnplayed) {
                cnt += ((vself->mUsedBetweenEventsPicks >> pick) ^ 1) & 1;
                pick += cnt < nthUnplayed;
            }
        }
    }
    vself->mUsedBetweenEventsPicks |= 1 << pick;
    char* snd = (char*)&vself->SpeechSys;
    switch (pick) {
    case 0:
        cSSXAudio_SpeakDJLine8394((cSSXAudio*)vself);
        break;
    case 1:
        cSSXAudio_SpeakDJLine8425((cSSXAudio*)vself, -1);
        break;
    case 2:
        cSSXAudio_SpeakDJLine8392((cSSXAudio*)vself);
        break;
    case 3:
        cSSXAudio_SpeakDJRival((cSSXAudio*)vself);
        break;
    case 4:
        cSSXAudio_SpeakDJLine8492(vself);
        break;
    case 5:
        cSSXAudio_SpeakDJLine8423((cSSXAudio*)vself);
        break;
    case 6:
        cSSXAudio_SpeakDJLine8424((cSSXAudio*)vself);
        break;
    case 7:
        cSSXAudio_SpeakDJLine8393((cSSXAudio*)vself);
        break;
    }
    cSpeechSys_StartNextSpeech(snd, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_StartDJIntro);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_SpeakDJChoiceLine(cSSXAudio* self, int a, int b);
void cSpeechSys_StartNextSpeech(void* self, int a1);
int cMusicSys_GetCurrentSongSedValue(void* self);

/**
 * Starts the DJ intro of a race: DJ line 8421 with (0, 0), then queues the song announcement.
 *
 * Calls cSSXAudio_SpeakDJChoiceLine(self, 0, 0) and cSpeechSys_StartNextSpeech(SpeechSys, 0), then sets mSongAnnouncePending (pending
 * announcement) and stores the current song's value cMusicSys_GetCurrentSongSedValue in mSongAnnounceNum for cSSXAudio_PlayPendingDJLine /
 * cSSXAudio_SpeakDJSongAnnouncement. cSSXAudio_CountdownGateDrop calls it in game paths 1 and 2.
 */
extern "C" void cSSXAudio_StartDJIntro(cSSXAudio* self)
{
    cSSXAudio_SpeakDJChoiceLine(self, 0, 0);
    cSpeechSys_StartNextSpeech(&self->SpeechSys, 0);
    self->mSongAnnouncePending = 1;
    self->mSongAnnounceNum = cMusicSys_GetCurrentSongSedValue(&self->MusicSys);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakPAEventStart);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsPASpeechOn(cSSXAudio* self);
extern "C" bool cSSXAudio_IsFreeride(cSSXAudio* self);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" bool cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests PA speech line 8384 at the start of a single-player event's first heat.
 *
 * Needs PA speech audible (cSSXAudio_IsPASpeechOn), a game mode other than 4 (cSSXAudio_IsFreeride), game path 0 (newRace.cGamePath) and a
 * cGameModeMan iHeatNum below 2, with play-history event 0 not live. The request goes to speech instance 0 with
 * identity 11 (the PA); when queued, the speech hook D_004A482C gets its packed id, the language bit and 1.
 * cSSXAudio_PlayScriptSpeechEvent (event 0) and two front-end functions call it.
 */
extern "C" void cSSXAudio_SpeakPAEventStart(cSSXAudio* self)
{
    if (cSSXAudio_IsPASpeechOn(self) == 0) return;
    if (cSSXAudio_IsFreeride(self) != 0) return;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath != 0) return;
    if (*(int*)SSXApp->GameModeMan < 2) {
        if (self->PlayHistoryExists(0, 0) != 0) return;
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8384, 0, 11, 0, 0, 0.0f) == 0) return;
        int languageBit = cSSXAudio_GetSpeechLanguage(self);
        D_004A482C_4(SPCH_makeKey(1, 0, 8384), 2, languageBit, 1);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakPALine8385);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cSSXAudio_IsPASpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getAI();
// cSSXAudio_GetRiderSubjectBit returns its tail call's value (the unit defines it as void).
int cSSXAudio_GetRiderSubjectBit_r(void* self, void* obj) __asm__("cSSXAudio_GetRiderSubjectBit");
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests PA speech line 8385 about rider 0's character.
 *
 * Needs PA speech audible (cSSXAudio_IsPASpeechOn); the request goes to speech instance 0 with identity 11 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and rider 0's character bit (cSSXAudio_GetRiderSubjectBit).
 * cSSXAudio_PlayScriptSpeechEvent calls it (event 1).
 */
extern "C" void cSSXAudio_SpeakPALine8385(cSSXAudio* self)
{
    if (cSSXAudio_IsPASpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8385, 0, 11, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int riderBit = cSSXAudio_GetRiderSubjectBit_r(self, ((cAI*)cAudioInterface_getAI())->Riders[0]);
            D_004A482C_4(SPCH_makeKey(1, 0, 8385), 2, languageBit, riderBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakPALine8386);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cSSXAudio_IsPASpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getAI();
// cSSXAudio_GetRiderSubjectBit returns its tail call's value (the unit defines it as void).
int cSSXAudio_GetRiderSubjectBit_r(void* self, void* obj) __asm__("cSSXAudio_GetRiderSubjectBit");
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests PA speech line 8386 about rider 0's character.
 *
 * Needs PA speech audible (cSSXAudio_IsPASpeechOn); the request goes to speech instance 0 with identity 11 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and rider 0's character bit (cSSXAudio_GetRiderSubjectBit).
 * cSSXAudio_PlayScriptSpeechEvent calls it (event 2).
 */
extern "C" void cSSXAudio_SpeakPALine8386(cSSXAudio* self)
{
    if (cSSXAudio_IsPASpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8386, 0, 11, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int riderBit = cSSXAudio_GetRiderSubjectBit_r(self, ((cAI*)cAudioInterface_getAI())->Riders[0]);
            D_004A482C_4(SPCH_makeKey(1, 0, 8386), 2, languageBit, riderBit);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakPALine8387);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsPASpeechOn(cSSXAudio* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationSpeechBit(cSSXAudio* self, int a, int b);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests PA speech line 8387 about the current location.
 *
 * Needs PA speech audible (cSSXAudio_IsPASpeechOn) and no replay; the request goes to speech instance 0 with identity 11 and,
 * when queued, the speech hook D_004A482C gets its packed id, the language bit and the location bit
 * cSSXAudio_GetLocationSpeechBit(location, -1). cSSXAudio_PlayScriptSpeechEvent (event 3) and cSSXAudio_CountdownGateDrop (the last heat) call it.
 */
extern "C" void cSSXAudio_SpeakPALine8387(cSSXAudio* self)
{
    if (cSSXAudio_IsPASpeechOn(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8387, 0, 11, 0, 0, 0.0f) == 0) return;
    int languageBit = cSSXAudio_GetSpeechLanguage(self);
    int locationBit = cSSXAudio_GetLocationSpeechBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)), -1);
    D_004A482C_4(SPCH_makeKey(1, 0, 8387), 2, languageBit, locationBit);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakPAPlace);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cMonitor_MONITOR_Create_i(void*, int, long, void*, int, int, int,
        int) __asm__("cMonitor_MONITOR_Create");
extern sSndKey8 D_004A36E8_p[] __asm__("D_004A36E8");
extern "C" int cSSXAudio_IsRivalEvent_p(void *) __asm__("cSSXAudio_IsRivalEvent");
extern "C" int cSSXAudio_IsPeakChallengeEvent_p(void *) __asm__("cSSXAudio_IsPeakChallengeEvent");

/**
 * Requests the PA's place announcement (speech 8388) for a rider, or schedules it for the rival challenges.
 *
 * Returns true without speech when PA speech is muted or replaying. In the game events kGAMEEVENT_RIVALTIME and
 * kGAMEEVENT_RIVALPOINTS (cSSXAudio_IsRivalEvent) with a rider it creates a 1000 ms monitor (cMonitor_MONITOR_Create) with the
 * callback D_004A36E8 (cSSXAudio_SpeakPlaceCallback) and the rider's AI index, and returns true. Events 6..11 (cSSXAudio_IsPeakChallengeEvent) return
 * true. With @p flag set, speech instance 0 must be ready (cSpeechSys_CanTakeSpeech), else false is returned. Without a rider, the
 * location must pass cSSXAudio_IsRaceEvent and the game mode must not be kGAMEMODE_FREERIDE, and the focus rider of view @p
 * which is used, its place bit coming from RaceRanking (0..4: 1..0x10, 5: 0x200). Riders with a cheat identity give
 * true. When request 8388 (identity 11) is queued, the speech hook D_004A482C gets its packed id, the language bit, the
 * rider's character bit (cSSXAudio_GetRiderSubjectBit) and the place bit (cSSXAudio_GetRiderPlaceBit for a given rider).
 *
 * @param vself  the audio system
 * @param rider  the rider, or 0 for the focus rider of @p which
 * @param flag  nonzero to require speech instance 0 to be ready
 * @param which  the view whose focus rider is used without a rider
 * @return false when speech instance 0 was busy, else true
 */
extern "C" bool cSSXAudio_SpeakPAPlace(cSSXAudio* vself, cRider *rider, int flag, int which)
{
    int sel;
    int languageBit;
    int riderBit;
    int placeBit;

    if (cSSXAudio_IsPASpeechOn((cSSXAudio*)(((char*)vself))) == 0) {
        return true;
    }
    if (cSSXAudio_ReplayMode(((char*)vself)) != 0) {
        return true;
    }
    if (cSSXAudio_IsRivalEvent_p(((char*)vself)) != 0 && rider != 0) {
        sSndKey8 *key = &D_004A36E8_p[0];
        cBeing& being = *rider;
        cBankMonitor* dst = &vself->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
        int idx = being.getAIIndex();
        cMonitor_MONITOR_Create_i(dst, 1000, key->v, 0, idx, 0, 0, 0);
        return true;
    }
    if (cSSXAudio_IsPeakChallengeEvent_p(((char*)vself)) != 0) {
        return true;
    }
    if (flag != 0) {
        if (cSpeechSys_CanTakeSpeech(&vself->SpeechSys, 0) == 0) {
            return false;
        }
    }
    sel = 1;
    if (rider == 0) {
        if (cSSXAudio_IsRaceEvent(((char*)vself)) == 0) {
            return true;
        }
        if (cSSXAudio_IsFreeride((cSSXAudio*)(((char*)vself))) != 0) {
            return true;
        }
        sel = 0;
        rider = (cRider *)cSSXAudio_GetFocusRider_p(((char*)vself), which);
    }
    if (cRider_getCheatIdentity(rider) != 0) {
        return true;
    }
    if (cSpeechSys_AddRequest_f(&vself->SpeechSys, 0, 8388, 0, 11, 0.0f, 0, 0) == 0) {
        return true;
    }
    languageBit = cSSXAudio_GetSpeechLanguage((cSSXAudio*)(((char*)vself)));
    riderBit = cSSXAudio_GetRiderSubjectBit_i(((char*)vself), rider);
    if (sel != 0) {
        placeBit = cSSXAudio_GetRiderPlaceBit(((char*)vself), rider);
    } else {
        switch ((unsigned int)rider->RaceRanking) {
        case 0:
            placeBit = 1;
            break;
        case 1:
            placeBit = 2;
            break;
        case 2:
            placeBit = 4;
            break;
        case 3:
            placeBit = 8;
            break;
        case 4:
            placeBit = 0x10;
            break;
        case 5:
            placeBit = 0x200;
            break;
        default:
            placeBit = 0;
            break;
        }
    }
    D_004A482C_5(SPCH_makeKey(1, 0, 8388), 3, languageBit, riderBit, placeBit);
    return true;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakPARiderPlace);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsPASpeechOn(cSSXAudio* self);
extern "C" int cRider_getCheatIdentity(void*);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" int cSSXAudio_GetRiderPlaceBit(void* self, cRider* rider);
// PORT: callers pass self to cSSXAudio_IsRivalEvent (the unit's later definition takes no args).
bool cSSXAudio_IsRivalEvent_s(void* self) __asm__("cSSXAudio_IsRivalEvent");
// PORT: D_004A482C is called with 5 args here (the unit's later declaration has 3).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");
extern sSndKey8 D_004A36E8[];

/**
 * Requests the PA's speech line 8389 about a rider and its place, or schedules the place line for event types 4 and 5.
 *
 * Needs PA speech audible, no replay and no cheat identity. In game events 4 and 5 (cSSXAudio_IsRivalEvent) it only creates a
 * 1000 ms monitor (cMonitor_MONITOR_Create) with the callback D_004A36E8 (cSSXAudio_SpeakPlaceCallback) and the rider's AI index
 * (getAIIndex). Otherwise, when request 8389 (identity 11) is queued, the speech hook D_004A482C gets its packed id,
 * the language bit, the rider's character bit (cSSXAudio_GetRiderSubjectBit) and its place bit (cSSXAudio_GetRiderPlaceBit).
 *
 * @param rider  the rider (cRider)
 */
// PORT: 64-bit long sound key
extern "C" void cSSXAudio_SpeakPARiderPlace(cSSXAudio* self, cRider* rider)
{
    if (cSSXAudio_IsPASpeechOn(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cRider_getCheatIdentity(rider) != 0) return;
    if (cSSXAudio_IsRivalEvent_s(self) != 0) {
        cBeing& being = *rider;
        cBankMonitor* bankMonitor = &self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
        sSndKey8* key = D_004A36E8;
        int aiIndex = being.getAIIndex();
        cMonitor_MONITOR_Create(bankMonitor, 1000, key->v, 0, aiIndex, 0, 0, 0);
        return;
    }
    if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8389, 0, 11, 0, 0, 0.0f) == 0) return;
    int languageBit = cSSXAudio_GetSpeechLanguage(self);
    int riderBit = cSSXAudio_GetRiderSubjectBit_i(self, rider);
    int placeBit = cSSXAudio_GetRiderPlaceBit(self, rider);
    D_004A482C_5(SPCH_makeKey(1, 0, 8389), 3, languageBit, riderBit, placeBit);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakPAResult);
#ifdef SKIP_ASM
#include "fakematch.h"
#include "object/being.h"
extern "C" bool cSSXAudio_IsPASpeechOn(cSSXAudio* self);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
// PORT: D_004A482C is called with 5 args here (the unit's later declaration has 3).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");
extern int cGameModeMan_iRanking[];

struct sSndRiderList3860 {
    void* r[8];
};

/**
 * Requests PA speech line 8391 announcing a result: the leading human rider's character and its place.
 *
 * Needs PA speech audible and no replay. In the multiplayer game path the second human is used when it ranks ahead
 * (cGameModeMan_iRanking[1] < cGameModeMan_iRanking[0]), else rider 0. When request 8391 (identity 11) is queued, the speech hook D_004A482C
 * gets its packed id, the language bit, that rider's character bit (cSSXAudio_GetRiderSubjectBit, from the cAI's rider list at +0x28)
 * and a place value: 8 outside the multiplayer path when the reward interface's +0x14 is 0, else 1, 2 or 4 for rank 0,
 * 1 or 2 and 0 for any other rank. cSSXAudio_PlayScriptSpeechEvent calls it (event 7).
 */
// FAKEMATCH: FM_OPAQUE(lst) after `lst = cAudioInterface_getAI() + 0x28` (keeps combine from folding +0x28 into the
// lw offset; retail computes addiu 0x28 / addu / lw 0); plain form 96.86%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" void cSSXAudio_SpeakPAResult(cSSXAudio* self)
{
    if (cSSXAudio_IsPASpeechOn(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int n = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    signed char* newRaceBytes = (signed char*)((char*)&cBENewRaceInterface_newRace);
    if (newRaceBytes[0x49] == 2) {
        n = cGameModeMan_iRanking[1] < cGameModeMan_iRanking[0];
    }
    if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8391, 0, 11, 0, 0, 0.0f) == 0) return;
    int languageBit = cSSXAudio_GetSpeechLanguage(self);
    sSndRiderList3860* lst = (sSndRiderList3860*)((char*)cAudioInterface_getAI() + 0x28);
    FM_OPAQUE(lst);
    int riderBit = cSSXAudio_GetRiderSubjectBit_i(self, lst->r[n]);
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 13);
    if (newRaceBytes[0x49] != 2 && *(int*)(iface + 0x14) == 0) {
        n = 8;
    } else {
        n = cGameModeMan_iRanking[n];
        if (n == 0) {
            n = 1;
        } else if (n == 1) {
            n = 2;
        } else {
            int z = n != 2;
            n = 4;
            if (z) n = 0;
        }
    }
    D_004A482C_5(SPCH_makeKey(1, 0, 8391), 3, languageBit, riderBit, n);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakPALine8379);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsPASpeechOn(cSSXAudio* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" int cSSXAudio_GetSpeechLanguage(cSSXAudio* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_GetLocationSpeechBit(cSSXAudio* self, int a, int b);
extern "C" void* SPCH_makeKey(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

/**
 * Requests PA speech line 8379 about the current location.
 *
 * Needs PA speech audible (cSSXAudio_IsPASpeechOn); the request goes to speech instance 0 with identity 11 and, when queued, the
 * speech hook D_004A482C gets its packed id, the language bit and the location bit cSSXAudio_GetLocationSpeechBit(location, -1).
 * cSSXAudio_PlayScriptSpeechEvent (event 15) and cSSXAudio_StartEvent call it.
 */
extern "C" void cSSXAudio_SpeakPALine8379(cSSXAudio* self)
{
    if (cSSXAudio_IsPASpeechOn(self) != 0) {
        if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8379, 0, 11, 0, 0, 0.0f) != 0) {
            int languageBit = cSSXAudio_GetSpeechLanguage(self);
            int locationBit = cSSXAudio_GetLocationSpeechBit(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)), -1);
            D_004A482C_4(SPCH_makeKey(1, 0, 8379), 2, languageBit, locationBit);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_CheerOnStreamDoneCallback);
#ifdef SKIP_ASM
extern "C" int SNDSTRM_requeststatus(int, void *);
extern "C" void cCrowdSys_PlayTrickCheer(cCrowdSys* self, int a1, int i);

/**
 * Raises the crowd level 0 reaction when a monitored stream has finished; returns whether it did.
 *
 * Asks the sound driver for the status of the stream whose handle is at @p monitorInst + 0x1C (SNDSTRM_requeststatus
 * inside SNDSYS_entercritical / SNDSYS_leavecritical); a status word of kSNDREQUEST_Done calls cCrowdSys_PlayTrickCheer(self, 0,
 * 0) and returns true. The retail image has no direct reference to it.
 *
 * @param monitorInst  the monitor instance whose +0x1C holds the stream handle
 * @return true when the stream reported kSNDREQUEST_Done, else false
 */
extern "C" bool cSSXAudio_CheerOnStreamDoneCallback(void* self, cMonitorInstance* monitorInst) {
    int st[4];
    SNDSYS_entercritical();
    int status = SNDSTRM_requeststatus(monitorInst->mData1, st);
    SNDSYS_leavecritical();
    if (status >= 0) {
        if (st[0] == kSNDREQUEST_Done) {
            cCrowdSys_PlayTrickCheer((cCrowdSys*)self, 0, 0);
            return true;
        }
        return false;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakArcadeBoost);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cSSXAudio_IsArcadeSpeechOn(cSSXAudio* self);
// cSSXAudio_GetFocusRider returns an object pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

/**
 * Raises arcade announcer event 8359 with a value, for the focus rider before the finish line.
 *
 * Needs ArcadeSpeech audible (cSSXAudio_IsArcadeSpeechOn), a rider that is the focus rider and not past the finish line, and no
 * replay. When request 8359 (speech instance 0, identity 12) is queued, the speech hook D_004A482C gets its packed id
 * and @p eventValue. cRider_flagEventSpeedBoost and cRider_flagEventTrickBoost call it.
 *
 * @param obj  the rider (cRider)
 * @param eventValue  the value passed to the event
 */
extern "C" void cSSXAudio_SpeakArcadeBoost(cSSXAudio* self, cRider* obj, int eventValue)
{
    if (cSSXAudio_IsArcadeSpeechOn(self) == 0) return;
    if (((void*)obj) == 0) return;
    int done = obj->mFinishTime >= 0.0f;
    if (done) return;
    if (((void*)obj) != cSSXAudio_GetFocusRider_p(self, -1)) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8359, 0, 0xC, 0, 0, 0.0f) != 0) {
        D_004A482C(SPCH_makeKey(1, 0, 8359), 1, eventValue);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakArcadeResult);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cSSXAudio_IsArcadeSpeechOn(cSSXAudio* self);
// cSSXAudio_GetFocusRider returns an object pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

/**
 * Raises arcade announcer event 8360 (an event result) for the focus rider.
 *
 * Needs ArcadeSpeech audible (cSSXAudio_IsArcadeSpeechOn), a rider that is the focus rider and no replay; unless @p resultBit is 2 the
 * rider must not be past the finish line. When request 8360 (speech instance 0, identity 12) is queued, the speech hook
 * D_004A482C gets its packed id and @p resultBit. Values seen: 1 big challenge complete (cSSXAudio_BigChallengeComplete, rider 0), 2 time ran
 * out (cRider_updateFinishTest, after the did-not-finish finish), 4 / 8 / 0x10 max power at an uber level below 5, 5..9
 * or 10 (cSSXAudio_MaxPowerEvent).
 *
 * @param obj  the rider (cRider)
 * @param resultBit  the result bit passed to the event
 */
extern "C" void cSSXAudio_SpeakArcadeResult(cSSXAudio* self, cRider* obj, int resultBit)
{
    if (cSSXAudio_IsArcadeSpeechOn(self) == 0) return;
    if (((void*)obj) == 0) return;
    if (resultBit != 2) {
        int done = obj->mFinishTime >= 0.0f;
        if (done) return;
    }
    if (((void*)obj) != cSSXAudio_GetFocusRider_p(self, -1)) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8360, 0, 0xC, 0, 0, 0.0f) != 0) {
        D_004A482C(SPCH_makeKey(1, 0, 8360), 1, resultBit);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakArcadeCheckpoint);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void (*D_004A482C)(void*, int, int);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
cRider* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");

/**
 * Raises arcade announcer event 8361 (a checkpoint reached) for a rider before the finish line.
 *
 * Needs ArcadeSpeech audible (cSSXAudio_IsArcadeSpeechOn), a rider not past the finish line, no replay and @p n nonzero; with
 * @p n other than 1 the rider must also be the focus rider. When request 8361 (speech instance 0, identity 12) is
 * queued, the speech hook D_004A482C gets its packed id and @p n. cRiderMetrix_evCheckpoint calls it with 1 when a
 * checkpoint adds time (cGameModeMan_riderCheckpointHit).
 *
 * @param rider  the rider (cRider)
 * @param n  the value passed to the event (1 from the checkpoint); 0 does nothing
 */
extern "C" void cSSXAudio_SpeakArcadeCheckpoint(cSSXAudio* self, cRider* rider, int n)
{
    float zero;
    if (cSSXAudio_IsArcadeSpeechOn(self) == 0) return;
    if (((char*)rider) == 0) return;
    float finishTime = rider->mFinishTime;
    zero = 0.0f;
    int done = finishTime >= zero;
    if (done) return;
    if (n != 1 && rider != cSSXAudio_GetFocusRider_p(self, -1)) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (n == 0) return;
    if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8361, 0, 0xC, 0, 0, zero) != 0) {
        D_004A482C(SPCH_makeKey(1, 0, 8361), 1, n);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakArcadeUberMilestone);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsArcadeSpeechOn(cSSXAudio* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

/**
 * Raises arcade announcer event 8499 (an uber trick milestone) for a rider before the finish line.
 *
 * Needs ArcadeSpeech audible (cSSXAudio_IsArcadeSpeechOn), a rider not past the finish line and no replay. When request 8499 (speech
 * instance 0, identity 12) is queued, the speech hook D_004A482C gets its packed id and @p ev: 1 when the landed uber
 * tricks pass 4 (cSSXAudio_LandedUberTrick), 2 when the focus rider's uber level reaches 10 (cSSXAudio_UpdateLandedUber), 8 for a
 * monster combo (cSSXAudio_LandedMonsterCombo).
 *
 * @param obj  the rider (cRider)
 * @param ev  the milestone bit passed to the event
 */
extern "C" void cSSXAudio_SpeakArcadeUberMilestone(cSSXAudio* self, cRider* obj, int ev)
{
    if (cSSXAudio_IsArcadeSpeechOn(self) == 0) return;
    if (((void*)obj) == 0) return;
    int done = obj->mFinishTime >= 0.0f;
    if (done) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8499, 0, 0xC, 0, 0, 0.0f) != 0) {
        D_004A482C(SPCH_makeKey(1, 0, 8499), 1, ev);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakArcadeMultiplier);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsArcadeSpeechOn(cSSXAudio* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cSpeechSys_AddRequest(void* self, int a, int b, cRider* rider, int d, int e, int refresh, float f);
extern "C" void* SPCH_makeKey(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

/**
 * Raises arcade announcer event 8517 (a multiplier icon collected) for a rider before the finish line.
 *
 * Needs ArcadeSpeech audible (cSSXAudio_IsArcadeSpeechOn), a rider not past the finish line and no replay. When request 8517 (speech
 * instance 0, identity 12) is queued, the speech hook D_004A482C gets its packed id and @p ev. cSSXAudio_PlayIconSound
 * passes 1, 2, 4 or 8 for a multiplier icon of 2, 3, 5 or 10.
 *
 * @param obj  the rider (cRider)
 * @param ev  the multiplier bit passed to the event
 */
extern "C" void cSSXAudio_SpeakArcadeMultiplier(cSSXAudio* self, cRider* obj, int ev)
{
    if (cSSXAudio_IsArcadeSpeechOn(self) == 0) return;
    if (((void*)obj) == 0) return;
    int done = obj->mFinishTime >= 0.0f;
    if (done) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cSpeechSys_AddRequest((char*)&self->SpeechSys, 0, 8517, 0, 0xC, 0, 0, 0.0f) != 0) {
        D_004A482C(SPCH_makeKey(1, 0, 8517), 1, ev);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsAtHub);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" bool cSSXAudio_IsHubLocation(cSSXAudio* self, unsigned int a1);

/**
 * Tells whether the current location (cBENewRaceInterface_getLocation) is one of 17..21, the hubs kLOCATION_HUB_A ..
 * kLOCATION_HUB_E (cSSXAudio_IsHubLocation).
 *
 * @return 1 or 0
 */
extern "C" int cSSXAudio_IsAtHub(cSSXAudio* self)
{
    return cSSXAudio_IsHubLocation(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsHubLocation);
#ifdef SKIP_ASM
/**
 * Tells whether a location id is one of 17..21 (kLOCATION_HUB_A .. kLOCATION_HUB_E), the locations that span several
 * peaks (cSSXAudio_GetLocationSpeechBit splits them).
 *
 * @param location  the location id
 * @return true or false
 */
extern "C" bool cSSXAudio_IsHubLocation(cSSXAudio* self, unsigned int location)
{
    return location - 17 < 5;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsInBackCountry);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" bool cSSXAudio_IsBackCountryLocation(cSSXAudio* self, unsigned int a1);

/**
 * Tells whether the current location (cBENewRaceInterface_getLocation) is one of 14..16, the back-country locations
 * kLOCATION_ABC1 .. kLOCATION_EBC3 (cSSXAudio_IsBackCountryLocation).
 *
 * @return 1 or 0
 */
extern "C" int cSSXAudio_IsInBackCountry(cSSXAudio* self)
{
    return cSSXAudio_IsBackCountryLocation(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsBackCountryLocation);
#ifdef SKIP_ASM
/**
 * Tells whether a location id is one of 14..16 (kLOCATION_ABC1 .. kLOCATION_EBC3), one per peak (cSSXAudio_GetLocationPeakBit gives
 * them peaks 1, 2 and 3).
 *
 * @param location  the location id
 * @return true or false
 */
extern "C" bool cSSXAudio_IsBackCountryLocation(cSSXAudio* self, unsigned int location)
{
    return location - 0xe < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsFreeride);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Tells whether the race's game mode (cBENewRaceInterface_newRace.cGameMode) is kGAMEMODE_FREERIDE.
 *
 * @return true or false
 */
extern "C" bool cSSXAudio_IsFreeride(cSSXAudio* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    return cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_FREERIDE;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsRaceEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" bool cSSXAudio_IsRaceLocation(void* self, unsigned int a1);

/**
 * Tells whether the race is of the first kind: a location in 0..4 (the race courses, cSSXAudio_IsRaceLocation) or game mode
 * kGAMEMODE_RACE or kGAMEMODE_TIMECHALLENGE.
 *
 * @return true or false
 */
extern "C" bool cSSXAudio_IsRaceEvent(void* self)
{
    if (cSSXAudio_IsRaceLocation(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0))) != 0) {
        return true;
    }
    if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_RACE) {
        return true;
    }
    return cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_TIMECHALLENGE;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsRaceLocation);
#ifdef SKIP_ASM
/**
 * Tells whether a location id is one of 0..4 (the race courses, which cSSXAudio_IsRaceEvent pairs with game modes
 * kGAMEMODE_RACE and kGAMEMODE_TIMECHALLENGE).
 *
 * @param location  the location id
 * @return true or false
 */
extern "C" bool cSSXAudio_IsRaceLocation(void* self, unsigned int location)
{
    return location < 5;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsSlopestyleEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" bool cSSXAudio_IsSlopestyleLocation(cSSXAudio* self, unsigned int a1);

/**
 * Tells whether the race is of the second kind: a location in 5..7 (the slopestyle courses, cSSXAudio_IsSlopestyleLocation) or game mode
 * kGAMEMODE_SLOPESTYLE or kGAMEMODE_PNTSCHALLENGE.
 *
 * @return 1 or 0
 */
extern "C" int cSSXAudio_IsSlopestyleEvent(cSSXAudio* self)
{
    if (cSSXAudio_IsSlopestyleLocation(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0))) != 0) {
        return 1;
    }
    if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_SLOPESTYLE) {
        return 1;
    }
    int isMode6 = 0;
    if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_PNTSCHALLENGE) {
        isMode6 = 1;
    }
    return isMode6;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsSlopestyleLocation);
#ifdef SKIP_ASM
/**
 * Tells whether a location id is one of 5..7 (the slopestyle courses, which cSSXAudio_IsSlopestyleEvent pairs with game modes
 * kGAMEMODE_SLOPESTYLE and kGAMEMODE_PNTSCHALLENGE).
 *
 * @param location  the location id
 * @return true or false
 */
extern "C" bool cSSXAudio_IsSlopestyleLocation(cSSXAudio* self, unsigned int location)
{
    return location - 5 < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsBigAirEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" bool cSSXAudio_IsBigAirLocation(void* self, unsigned int a1);

/**
 * Tells whether the race is of the third kind: a location in 8..10 (the big-air courses, cSSXAudio_IsBigAirLocation) or game mode
 * kGAMEMODE_BIGAIR.
 *
 * @return true or false
 */
extern "C" bool cSSXAudio_IsBigAirEvent(void* self)
{
    if (cSSXAudio_IsBigAirLocation(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0))) != 0) {
        return true;
    }
    return cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_BIGAIR;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsBigAirLocation);
#ifdef SKIP_ASM
/**
 * Tells whether a location id is one of 8..10 (the big-air courses, which cSSXAudio_IsBigAirEvent pairs with game mode
 * kGAMEMODE_BIGAIR).
 *
 * @param location  the location id
 * @return true or false
 */
extern "C" bool cSSXAudio_IsBigAirLocation(void* self, unsigned int location)
{
    return location - 8 < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsHalfpipeEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" bool cSSXAudio_IsHalfpipeLocation(void* self, unsigned int a1);

/**
 * Tells whether the race is of the fourth kind: a location in 11..13 (the half pipes, cSSXAudio_IsHalfpipeLocation) or game mode
 * kGAMEMODE_HALFPIPE.
 *
 * @return true or false
 */
extern "C" bool cSSXAudio_IsHalfpipeEvent(void* self)
{
    if (cSSXAudio_IsHalfpipeLocation(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0))) != 0) {
        return true;
    }
    return cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_HALFPIPE;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsHalfpipeLocation);
#ifdef SKIP_ASM
/**
 * Tells whether a location id is one of 11..13 (the half pipes, which cSSXAudio_IsHalfpipeEvent pairs with game mode
 * kGAMEMODE_HALFPIPE).
 *
 * @param location  the location id
 * @return true or false
 */
extern "C" bool cSSXAudio_IsHalfpipeLocation(void* self, unsigned int location)
{
    return location - 11 < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsRivalEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Tells whether the race's game event (cBENewRaceInterface_newRace.cGameEvent) is kGAMEEVENT_RIVALTIME or
 * kGAMEEVENT_RIVALPOINTS.
 *
 * In those events the place lines are deferred through a monitor (cSSXAudio_SpeakPAPlace, cSSXAudio_SpeakPARiderPlace) and cSSXAudio_SpeakPlace uses
 * place mode 2.
 *
 * @return true or false
 */
extern "C" bool cSSXAudio_IsRivalEvent(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    switch (cBENewRaceInterface_newRace.cGameEvent) {
    case kGAMEEVENT_RIVALTIME:
        return true;
    case kGAMEEVENT_RIVALPOINTS:
        return true;
    default:
        return false;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsRivalTimeEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Tells whether the race's game event (newRace.cGameEvent) is kGAMEEVENT_RIVALTIME; cSSXAudio_ChangePosition uses it
 * outside game mode kGAMEMODE_RACE.
 *
 * @return true or false
 */
extern "C" bool cSSXAudio_IsRivalTimeEvent(cSSXAudio* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    return cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_RIVALTIME;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsPeakChallengeEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Tells whether the race's game event (newRace.cGameEvent) is one of 6..11, kGAMEEVENT_TIME_PEAK1 ..
 * kGAMEEVENT_POINTS_PEAK3 (cSSXAudio_IsPeakTimeEvent and cSSXAudio_IsPeakPointsEvent split them).
 *
 * In these events cSSXAudio_SpeakPAPlace makes no place announcement; the music code checks it too.
 *
 * @return true or false
 */
extern "C" bool cSSXAudio_IsPeakChallengeEvent(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGameEvent >= 6) {
        if (cBENewRaceInterface_newRace.cGameEvent <= 11) {
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsPeakTimeEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Tells whether the race's game event (newRace.cGameEvent) is one of 6..8, kGAMEEVENT_TIME_PEAK1 ..
 * kGAMEEVENT_TIME_PEAK3; cSSXAudio_GetSongCategory uses it.
 *
 * @return true or false
 */
extern "C" bool cSSXAudio_IsPeakTimeEvent(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGameEvent >= 6) {
        if (cBENewRaceInterface_newRace.cGameEvent <= 8) {
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsPeakPointsEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Tells whether the race's game event (newRace.cGameEvent) is one of 9..11, kGAMEEVENT_POINTS_PEAK1 ..
 * kGAMEEVENT_POINTS_PEAK3; cSSXAudio_GetSongCategory uses it.
 *
 * @return true or false
 */
extern "C" bool cSSXAudio_IsPeakPointsEvent(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGameEvent >= 9) {
        if (cBENewRaceInterface_newRace.cGameEvent <= 11) {
            return true;
        }
    }
    return false;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayPendingDJLine);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getAI();
extern "C" void cSSXAudio_SpeakScriptEventType0(void* self, int a, int b);
extern "C" void cSSXAudio_SpeakPlace(void* self, cRider* rider);
extern "C" void cSSXAudio_SpeakDJChoiceLine(cSSXAudio* self, int a, int b);
extern "C" void cSSXAudio_SpeakDJCurrentLocation(cSSXAudio* self, int a);
extern "C" void cSSXAudio_SpeakDJLine8378(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJLine8422(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJSongAnnouncement(cSSXAudio* self, int a);
extern "C" void cSSXAudio_SpeakDJLine8491(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJLine8492(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJLine8493(cSSXAudio* self);
extern "C" void cSSXAudio_SpeakDJBetweenEvents(cSSXAudio* self, int a);

/**
 * Plays the most urgent pending DJ line when speech instance 0 is free, then any pending place line.
 *
 * Fills slot 4 of cSSXAudio's cSpeechSys vtable (D_004836D8); cSpeechInstance_Update calls it while the speech instance is idle.
 * The first set pending flag, in this order, is cleared and its line played: mPendingChoiceLine cSSXAudio_SpeakDJChoiceLine(0, 0),
 * mPendingPeakLine8491 cSSXAudio_SpeakDJLine8491, mPendingPeakLine8378 cSSXAudio_SpeakDJLine8378, mPendingLine8492 cSSXAudio_SpeakDJLine8492,
 * mPendingLocationLine cSSXAudio_SpeakDJLine8493, mRiderRemarkPending cSSXAudio_SpeakScriptEventType0(0, 0), mSongAnnouncePending
 * cSSXAudio_SpeakDJSongAnnouncement(mSongAnnounceNum), mPendingBetweenEventsLine and mPendingBetweenEventsLine2 cSSXAudio_SpeakDJBetweenEvents(0),
 * mPendingBonusBetweenEventsLine cSSXAudio_SpeakDJBetweenEvents(1), mPendingLocationPartLine cSSXAudio_SpeakDJCurrentLocation(mPendingLocationPart),
 * mPendingLine8422 cSSXAudio_SpeakDJLine8422. Independently, mPendingPlaceLine requests the place line cSSXAudio_SpeakPlace for the cAI's
 * rider mPlaceLineRider (when there is one) and is cleared.
 */
extern "C" void cSSXAudio_PlayPendingDJLine(cSSXAudio* self)
{
    if (self->mPendingChoiceLine != 0) {
        cSSXAudio_SpeakDJChoiceLine(self, 0, 0);
        self->mPendingChoiceLine = 0;
    } else if (self->mPendingPeakLine8491 != 0) {
        cSSXAudio_SpeakDJLine8491((cSSXAudio*)self);
        self->mPendingPeakLine8491 = 0;
    } else if (self->mPendingPeakLine8378 != 0) {
        cSSXAudio_SpeakDJLine8378((cSSXAudio*)self);
        self->mPendingPeakLine8378 = 0;
    } else if (self->mPendingLine8492 != 0) {
        cSSXAudio_SpeakDJLine8492(self);
        self->mPendingLine8492 = 0;
    } else if (self->mPendingLocationLine != 0) {
        cSSXAudio_SpeakDJLine8493((cSSXAudio*)self);
        self->mPendingLocationLine = 0;
    } else if (self->mRiderRemarkPending != 0) {
        cSSXAudio_SpeakScriptEventType0(((char*)self), 0, 0);
        self->mRiderRemarkPending = 0;
    } else if (self->mSongAnnouncePending != 0) {
        cSSXAudio_SpeakDJSongAnnouncement((cSSXAudio*)self, self->mSongAnnounceNum);
        self->mSongAnnouncePending = 0;
    } else if (self->mPendingBetweenEventsLine != 0) {
        cSSXAudio_SpeakDJBetweenEvents(self, 0);
        self->mPendingBetweenEventsLine = 0;
    } else if (self->mPendingBetweenEventsLine2 != 0) {
        cSSXAudio_SpeakDJBetweenEvents(self, 0);
        self->mPendingBetweenEventsLine2 = 0;
    } else if (self->mPendingBonusBetweenEventsLine != 0) {
        cSSXAudio_SpeakDJBetweenEvents(self, 1);
        self->mPendingBonusBetweenEventsLine = 0;
    } else if (self->mPendingLocationPartLine != 0) {
        cSSXAudio_SpeakDJCurrentLocation((cSSXAudio*)self, self->mPendingLocationPart);
        self->mPendingLocationPartLine = 0;
    } else if (self->mPendingLine8422 != 0) {
        cSSXAudio_SpeakDJLine8422((cSSXAudio*)self);
        self->mPendingLine8422 = 0;
    }
    if (self->mPendingPlaceLine != 0) {
        cRider* rider = ((cAI*)cAudioInterface_getAI())->Riders[self->mPlaceLineRider];
        if (rider != 0) {
            cSSXAudio_SpeakPlace(self, rider);
        }
        self->mPendingPlaceLine = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_ClearPendingLines);
#ifdef SKIP_ASM
/**
 * Clears every pending DJ / place line flag that cSSXAudio_PlayPendingDJLine checks (mPendingLine8422 .. mRiderRemarkPending, plus
 * mPendingOtherLine).
 *
 * cSSXAudio_InGameDeInit, cSSXAudio_ResumeAudio and cSSXAudio_HandleMusicEvent call it.
 */
extern "C" void cSSXAudio_ClearPendingLines(cSSXAudio* self)
{
    self->mPendingLine8422 = 0;
    self->mPendingPlaceLine = 0;
    self->mPendingBetweenEventsLine = 0;
    self->mPendingBetweenEventsLine2 = 0;
    self->mPendingBonusBetweenEventsLine = 0;
    self->mPendingLocationPartLine = 0;
    self->mPendingOtherLine = 0;
    self->mPendingChoiceLine = 0;
    self->mPendingPeakLine8378 = 0;
    self->mPendingLocationLine = 0;
    self->mSongAnnouncePending = 0;
    self->mPendingLine8492 = 0;
    self->mPendingPeakLine8491 = 0;
    self->mRiderRemarkPending = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_ResetEventResult);
#ifdef SKIP_ASM
/**
 * Resets the event-result tracking the DJ comments on after an event.
 *
 * Clears the pending flag mEventResultPending, the allow flag mEventFinished and the frag and uber-trick sums
 * (mEventResultFrags, mEventResultUberTricks), sets the place mEventResultPlace and the location mEventResultLocation
 * to -1, the last location mLastTransportTrack to 23 (none) and, with @p flag set, the last result line
 * mLastEventResultLine to -1. The constructor calls it with 1 and cSSXAudio_RecordHeatResult with 0 in heat 1.
 *
 * @param flag  nonzero to also forget the last result line played
 */
extern "C" void cSSXAudio_ResetEventResult(cSSXAudio* self, int flag)
{
    self->mEventResultPending = 0;
    self->mEventFinished = 0;
    self->mEventResultFrags = 0;
    self->mEventResultUberTricks = 0;
    self->mEventResultPlace = -1;
    self->mEventResultLocation = -1;
    if (flag) {
        self->mLastEventResultLine = -1;
    }
    self->mLastTransportTrack = 23;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_RecordHeatResult);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_ResetEventResult(cSSXAudio* self, int flag);
extern "C" void* cAudioInterface_getAI();
extern "C" bool cSSXAudio_IsRaceEvent(void* self);
extern "C" void cSSXAudio_AddHeatResult(cSSXAudio* self, int id, int a, int b, int c);
extern int cGameModeMan_iRanking[];

/**
 * Records rider 0's result of the heat for the DJ's event-result lines (cSSXAudio_AddHeatResult).
 *
 * In the first heat (cGameModeMan iHeatNum kHeat1) it first resets the tracking (cSSXAudio_ResetEventResult(self, 0)). Passes rider
 * 0's rank cGameModeMan_iRanking[0], its frags (the metrix Totals.NumFrags, only when cSSXAudio_IsRaceEvent holds, else 0), its
 * uber tricks (Totals.NumUberTricks) and whether it finished (mDidNotFinish == 0). cSSXAudio_OnFinishEvent calls it.
 */
extern "C" void cSSXAudio_RecordHeatResult(cSSXAudio* self)
{
    if (SSXApp->GameModeMan->iHeatNum == cGameModeMan::kHeat1) {
        cSSXAudio_ResetEventResult(self, 0);
    }
    int id = cGameModeMan_iRanking[0];
    cRider* rider = ((cAI*)cAudioInterface_getAI())->Riders[0];
    int fragCount;
    if (cSSXAudio_IsRaceEvent(self) != 0) {
        fragCount = rider->mMetrix->Totals.NumFrags;
    } else {
        fragCount = 0;
    }
    cSSXAudio_AddHeatResult(self, id, fragCount, rider->mMetrix->Totals.NumUberTricks, rider->mDidNotFinish ^ 1);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_AddHeatResult);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);

/**
 * Adds one heat's result to the event-result tracking, and on the event's last heat marks the result as pending.
 *
 * On the last heat (bIsLastHeat, or iNextHeat not above iHeatNum) it sets mEventResultPending, sets mEventFinished when
 * @p finished is set and the heat is cGameModeMan::kFinalHeat, and stores the place (mEventResultPlace) and the current
 * location (mEventResultLocation). The frag and uber-trick counts are always added to mEventResultFrags and
 * mEventResultUberTricks. cSSXAudio_SpeakDJEventResult picks the line from these.
 *
 * @param rank  the rider's rank (0 = first)
 * @param frags  the frags of the heat
 * @param uberTricks  the uber tricks of the heat
 * @param finished  nonzero when the rider finished
 */
extern "C" void cSSXAudio_AddHeatResult(cSSXAudio* self, int rank, int frags, int uberTricks, int finished)
{
    cGameModeMan* gameModeMan = SSXApp->GameModeMan;
    if (gameModeMan->bIsLastHeat != 0 || gameModeMan->iNextHeat <= gameModeMan->iHeatNum) {
        self->mEventResultPending = 1;
        if (finished != 0 && gameModeMan->iHeatNum == cGameModeMan::kFinalHeat) {
            self->mEventFinished = 1;
        }
        self->mEventResultPlace = rank;
        self->mEventResultLocation = *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0));
    }
    self->mEventResultFrags += frags;
    self->mEventResultUberTricks += uberTricks;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_HasTransportTrackChanged);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);

/**
 * Tells whether the track recorded at the last transport start (mLastTransportTrack) is set (not kMAX_LOCATIONS) and
 * differs from the current one.
 *
 * cSSXAudio_HandleMusicEvent uses it.
 *
 * @return true or false
 */
extern "C" bool cSSXAudio_HasTransportTrackChanged(cSSXAudio* self)
{
    int id = *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0));
    int cur = self->mLastTransportTrack;
    if (id == cur || cur == kMAX_LOCATIONS) {
        return false;
    }
    return true;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SpeakDJEventResult);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsRaceLocation(void* self, unsigned int a1);
extern "C" bool cSSXAudio_IsSlopestyleLocation(cSSXAudio* self, unsigned int a1);
extern "C" bool cSSXAudio_IsHalfpipeLocation(void* self, unsigned int a1);
extern "C" bool cSSXAudio_IsBackCountryLocation(cSSXAudio* self, unsigned int a1);
extern "C" bool cSSXAudio_IsHubLocation(cSSXAudio* self, unsigned int a1);
extern "C" void cSSXAudio_SpeakDJBetweenEvents(cSSXAudio* self, int a);
// bank-monitor method: returns BXrand()
extern "C" unsigned int cSoundSys_SndRand(void* mon);

/**
 * Plays the DJ's comment on the last event's result when one is pending; returns whether a line was played.
 *
 * With mEventResultPending set it is cleared and up to three results qualify when mEventFinished (the rider finished
 * the final heat) allows: first place (mEventResultPlace == 0), 5 or more frags (mEventResultFrags), or at least 27
 * uber tricks (24 outside the locations 0..7 and 11..13; mEventResultUberTricks). None qualifying: when mInTransport is
 * set and a last location mLastTransportTrack is known, it plays cSSXAudio_SpeakDJBetweenEvents(0) for locations 17..21, cSSXAudio_SpeakDJLine8425
 * for 14..16, else cSSXAudio_SpeakDJLocation, and returns true (false otherwise). With two or more, the result played last time
 * (mLastEventResultLine) is dropped. One is picked at random (cSoundSys_SndRand): cSSXAudio_SpeakDJWinner (first place),
 * cSSXAudio_SpeakDJFragsResult (frags) or cSSXAudio_SpeakDJUberTricksResult (uber tricks) with mEventResultLocation, and remembered in
 * mLastEventResultLine. cSSXAudio_SpeakDJBetweenEvents calls it before choosing a between-events line.
 *
 * @return true when a line was played, else false
 */
extern "C" bool cSSXAudio_SpeakDJEventResult(cSSXAudio* self)
{
    if (self->mEventResultPending != 0) {
        self->mEventResultPending = 0;
        int flags[3];
        int i;
        for (i = 2; i >= 0; i--) {
            flags[i] = 0;
        }
        int n = 0;
        if (self->mEventFinished != 0) {
            if (self->mEventResultPlace == 0) {
                flags[0] = 1;
                n = 1;
            }
            if (self->mEventResultFrags >= 5) {
                flags[1] = 1;
                n++;
            }
            int lim;
            if (cSSXAudio_IsRaceLocation(self, self->mEventResultLocation)) {
                lim = 27;
            } else if (cSSXAudio_IsSlopestyleLocation(self, self->mEventResultLocation)) {
                lim = 27;
            } else {
                lim = cSSXAudio_IsHalfpipeLocation(self, self->mEventResultLocation) ? 27 : 0x18;
            }
            if (self->mEventResultUberTricks >= lim) {
                flags[2] = 1;
                n++;
            }
        }
        self->mEventFinished = 0;
        if (n == 0) {
            if (self->mInTransport != 0) {
                int cur = self->mLastTransportTrack;
                if (cur != kMAX_LOCATIONS) {
                    if (cSSXAudio_IsHubLocation(self, cur)) {
                        cSSXAudio_SpeakDJBetweenEvents(self, 0);
                        return true;
                    }
                    if (cSSXAudio_IsBackCountryLocation(self, self->mLastTransportTrack)) {
                        cSSXAudio_SpeakDJLine8425((cSSXAudio*)self, self->mLastTransportTrack);
                        return true;
                    }
                    cSSXAudio_SpeakDJLocation((cSSXAudio*)self, self->mLastTransportTrack);
                    return true;
                }
            }
            return false;
        }
        if (n >= 2) {
            int last = self->mLastEventResultLine;
            if (last != -1 && flags[last] != 0) {
                flags[last] = 0;
                n--;
            }
        }
        int pick = -1;
        int k = (cSoundSys_SndRand(self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys) & 0x7FFF)
                * n / 0x7FFF + 1;
        while (k > 0) {
            pick++;
            if (flags[pick]) {
                k--;
            }
        }
        switch (pick) {
        case 0:
            cSSXAudio_SpeakDJWinner((cSSXAudio*)self);
            break;
        case 1:
            cSSXAudio_SpeakDJFragsResult((cSSXAudio*)self, self->mEventResultLocation);
            break;
        case 2:
            cSSXAudio_SpeakDJUberTricksResult((cSSXAudio*)self, self->mEventResultLocation);
            break;
        }
        self->mLastEventResultLine = pick;
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_BeginScriptedDJLine);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern const sSndKey8 D_004A36F0[];
extern "C" void cMonitor_MONITOR_Create(void* bank, int a1, long id, void* a3, int a4, int a5, int a6, int a7);

/**
 * Marks a scripted DJ moment for 30 seconds: sets mScriptedDJLine and starts a 30000 ms monitor that clears it.
 *
 * The monitor (cMonitor_MONITOR_Create) has the callback D_004A36F0 (cSSXAudio_EndScriptedDJLineCallback). While mScriptedDJLine is set,
 * cSSXAudio_IsScriptedDJLinePlaying reports a playing DJ line, so cSSXAudio_HandleMusicEvent leaves speech instance 0 alone. The script command
 * SectionFunc calls it with its action 3 (cGame_setState(game, 0xE, 2, 0)).
 */
// PORT: the 8-byte stream id is passed by value from an unaligned global (64-bit `long`).
extern "C" void cSSXAudio_BeginScriptedDJLine(cSSXAudio* self)
{
    self->mScriptedDJLine = 1;
    cMonitor_MONITOR_Create(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                            30000, D_004A36F0[0].v, 0, 0, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_IsScriptedDJLinePlaying);
#ifdef SKIP_ASM
extern "C" bool cSSXAudio_IsCharacterSpeaking(cSSXAudio* self, int id);

/**
 * Tells whether a scripted DJ line is playing: mScriptedDJLine is set (cSSXAudio_BeginScriptedDJLine) and speech instance 0 says
 * identity 10.
 *
 * Uses cSSXAudio_IsCharacterSpeaking(self, 10). cSSXAudio_HandleMusicEvent checks it before it interrupts the speech.
 *
 * @return true or false
 */
extern "C" bool cSSXAudio_IsScriptedDJLinePlaying(cSSXAudio* self)
{
    if (self->mScriptedDJLine != 0) {
        return cSSXAudio_IsCharacterSpeaking((cSSXAudio*)self, 10) != 0;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_EndScriptedDJLineCallback);
#ifdef SKIP_ASM
/**
 * Monitor callback: ends the scripted DJ moment by clearing mScriptedDJLine; returns 1.
 *
 * Its pointer to member is D_004A36F0, which cSSXAudio_BeginScriptedDJLine installs with a 30 s delay.
 *
 * @param ssxAudio  the audio system (cSSXAudio)
 * @return 1
 */
extern "C" int cSSXAudio_EndScriptedDJLineCallback(cSSXAudio* ssxAudio) {
    ssxAudio->mScriptedDJLine = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_ScheduleDJOpening);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern const sSndKey8 D_004A3650[];
extern "C" void cMonitor_MONITOR_Create(void*, int, long, void*, int, int, int, int);
extern "C" int cSSXAudio_SetLastMusicState(void* self, int a1);

/**
 * Schedules the DJ's opening for the event in the world-circuit path, 2 ms later through cSSXAudio_HandleMusicChangeEvent.
 *
 * Only when mFirstTimeOnPeak is 0 and the game path (newRace.cGamePath) is 0. At the locations 17..21 (cSSXAudio_IsAtHub)
 * it sets mLastMusicState to 13 (cSSXAudio_SetLastMusicState) and creates a monitor (cMonitor_MONITOR_Create) with the callback
 * D_004A3650 (cSSXAudio_HandleMusicChangeEvent) and kind 2; elsewhere the kind is 4 with the current song value (cMusicSys_GetCurrentSongSedValue) as its
 * argument. cSSXAudio_InGameInit calls it.
 */
// PORT: 64-bit long sound key
extern "C" void cSSXAudio_ScheduleDJOpening(cSSXAudio* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (self->mFirstTimeOnPeak == 0 && cBENewRaceInterface_newRace.cGamePath == 0) {
        if (cSSXAudio_IsAtHub(self) != 0) {
            cSSXAudio_SetLastMusicState(self, 13);
            cMonitor_MONITOR_Create(&self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                    2, D_004A3650->v, 0, 2, 0, 0, 0);
        } else {
            cBankMonitor* bankMonitor = &self->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor;
            const sSndKey8* key = D_004A3650;
            int songValue = cMusicSys_GetCurrentSongSedValue(&self->MusicSys);
            cMonitor_MONITOR_Create(bankMonitor, 2, key->v, 0, 4, songValue, 0, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_StopSpeechForPkTrans);
#ifdef SKIP_ASM
extern "C" int cMusicSys_GetSongTag(void*);
extern "C" void cSpeechSys_PurgeInstance(void*, int);

/**
 * Stops speech instance 0 (cSpeechSys_PurgeInstance) when the current song's tag (cMusicSys_GetSongTag) is kSONGTAG_PkTrans.
 *
 * cSSXAudio_HandleMusicEvent sets that tag when it starts the "pktrans" stream. cGFTrackStartState_gainFocus calls it.
 */
extern "C" void cSSXAudio_StopSpeechForPkTrans(cSSXAudio* self)
{
    if (cMusicSys_GetSongTag((char*)&self->MusicSys) == kSONGTAG_PkTrans) {
        cSpeechSys_PurgeInstance((char*)&self->SpeechSys, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", tCrowdEventData_tCrowdEventData);
#ifdef SKIP_ASM
/**
 * Constructs a crowd sample list: no samples, all 5 pointers cleared.
 *
 * cCrowdSys_cCrowdSys runs it for the TRICK, FALL and ANTICIPATE lists of each of the 3 intensities.
 *
 * @return this
 */
extern "C" void* tCrowdEventData_tCrowdEventData(tCrowdEventData* self)
{
    int i;
    self->mNumEventData = 0;
    for (i = 4; i >= 0; i--) {
        self->mpData[i] = 0;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", tCrowdEventData__tCrowdEventData);
#ifdef SKIP_ASM
extern "C" void tCrowdEventData_FreeSamples(tCrowdEventData*);
void operator_delete(int*);

/**
 * Destroys a crowd sample list: frees its samples (tCrowdEventData_FreeSamples) and, with bit 0 of @p flags, the list itself.
 *
 * cCrowdSys__cCrowdSys runs it for each list.
 *
 * @param flags  bit 0 set to also delete the object
 */
extern "C" void tCrowdEventData__tCrowdEventData(tCrowdEventData* self, int flags)
{
    tCrowdEventData_FreeSamples(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", tCrowdEventData_LoadSample);
#ifdef SKIP_ASM
extern "C" void* FILE_loadz(const char* name, int flags);

/**
 * Loads one crowd sample file (FILE_loadz) into the next free slot of a sample list; returns whether it was added.
 *
 * A full list (5 samples) or a failed load returns false. cCrowdSys_ReadSection calls it for the TRICK, FALL and ANTICIPATE
 * entries of crowd.inf.
 *
 * @param list  the sample list (tCrowdEventData)
 * @param name  the file name
 * @param flags  the FILE_loadz flags (0)
 * @return true when the sample was added, else false
 */
extern "C" bool tCrowdEventData_LoadSample(tCrowdEventData* list, const char* name, int flags)
{
    if (list->mNumEventData >= 5 ||
        (list->mpData[list->mNumEventData] = FILE_loadz(name, flags), list->mpData[list->mNumEventData] == 0)) {
        return false;
    }
    list->mNumEventData++;
    return true;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", tCrowdEventData_GetRandomSample);
#ifdef SKIP_ASM
unsigned int BXrand();

struct sSndRandList {
    unsigned int count;
    int items[1];
};

/**
 * Picks a random sample of a crowd sample list (items[BXrand() % count]), or 0 for an empty list.
 *
 * @param list  the sample list (tCrowdEventData)
 * @return the sample data, or 0
 */
extern "C" int tCrowdEventData_GetRandomSample(sSndRandList* list)
{
    if (list->count == 0) {
        return 0;
    }
    return list->items[BXrand() % list->count];
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", tCrowdEventData_FreeSamples);
#ifdef SKIP_ASM
void cMemMan_free(void*);

/**
 * Frees every sample of a crowd sample list (cMemMan_free) and empties it.
 */
extern "C" void tCrowdEventData_FreeSamples(tCrowdEventData* self)
{
    tCrowdEventData* list = self;
    for (int i = 0; i < list->mNumEventData; i++) {
        if (list->mpData[i] != 0) {
            cMemMan_free(list->mpData[i]);
        }
        list->mpData[i] = 0;
    }
    list->mNumEventData = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdInstance_cCrowdInstance);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cCrowdInstance_cCrowdInstance_sTriple {
    int a;
    int b;
    int c;
};

struct cCrowdInstance_cCrowdInstance_sChan {
    int e0;
    int e4;
    int e8;
    char pad[0x20 - 0xC];
    int index;      // +0x20
    char pad2[0x30 - 0x24];
    cCrowdInstance_cCrowdInstance_sChan() {}
};

// The constructor's view of a cCrowdInstance (sound/crowdsys.h): the named fields carry the class's member names at
// the same offsets (f10..f1C are the words of mCrowdPos, all ints here as the constructor stores them).
struct cCrowdInstance_cCrowdInstance_sObj {
    int mViewPort;
    char pad4[0x10 - 0x4];
    int f10;
    int f14;
    int f18;
    int f1C;
    cCrowdInstance_cCrowdInstance_sTriple triples[15];  // 0x20
    int mNumCrowdObjects;
    int mCrowdBank;
    int fDC;
    cCrowdInstance_cCrowdInstance_sChan chans[3];       // 0xE0
    int mNumCrowdAmbience;
    int mCheerHandle;
    int mLoudness;
    int mpVolControl;
    int mCheerState;
    int mAmbientCheerIntensity;
    int mCheerTimeLeft;
    int mCurrentCheer;
    cCrowdInstance_cCrowdInstance_sObj() __asm__("cCrowdInstance_cCrowdInstance");
};

/**
 * Constructs a crowd instance: no viewport, no crowd objects or ambient loops, no cheer.
 *
 * Sets mViewPort, mCrowdBank and the cheer handle mCheerHandle to -1, mAmbientCheerIntensity to 3 (none), the averaged
 * position, mNumCrowdObjects, mNumCrowdAmbience and the cheer fields to 0, clears the 15 crowd object slots and gives
 * the 3 ambient loops their index (index) and sampleID / handle -1. cCrowdSys_Init runs it on each element of its
 * "CrowdInstances" array.
 */
cCrowdInstance_cCrowdInstance_sObj::cCrowdInstance_cCrowdInstance_sObj()
    : mViewPort(-1), f10(0), f14(0), f18(0), f1C(0), mNumCrowdObjects(0), mCrowdBank(-1),
      mNumCrowdAmbience(0), mCheerHandle(-1), mLoudness(0), mpVolControl(0), mCheerState(0), mAmbientCheerIntensity(3),
      mCheerTimeLeft(0), mCurrentCheer(0)
{
    int i;
    for (i = 0; i < 15; i++) {
        triples[i].a = 0;
        triples[i].b = 0;
        triples[i].c = 0;
    }
    for (i = 0; i < 3; i++) {
        *(int*)((char*)this + i * 0x30 + 0x100) = i;
        *(int*)((char*)this + i * 0x30 + 0xE0) = -1;
        *(int*)((char*)this + i * 0x30 + 0xE4) = -1;
        *(int*)((char*)this + i * 0x30 + 0xE8) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdInstance__cCrowdInstance);
#ifdef SKIP_ASM
extern "C" void cCrowdInstance_StopCheer(cCrowdInstance* self);
extern "C" void cCrowdInstance_RemoveAllObjects(cCrowdInstance* self, float v);
void operator_delete(int*);

/**
 * Destroys a crowd instance: stops its cheer (cCrowdInstance_StopCheer), removes all its crowd objects (cCrowdInstance_RemoveAllObjects(self, 0))
 * and, with bit 0 of @p flags, deletes it.
 *
 * cCrowdSys_Shutdown runs it on each element of the "CrowdInstances" array, from the last one down.
 *
 * @param flags  bit 0 set to also delete the object
 */
extern "C" void cCrowdInstance__cCrowdInstance(cCrowdInstance* self, int flags)
{
    cCrowdInstance_StopCheer(self);
    cCrowdInstance_RemoveAllObjects(self, 0.0f);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdInstance_Update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
class cController;
class cBankMonitorInstance;
// Sound callback: a g++ 2.95 pointer to member function (8 bytes), as cSndPlayOpts::OPTS_Callback takes it.
typedef void (cController::*sSndCallback)(cController*, cBankMonitorInstance*, float);
// One cSndPlayOpts mDelay[] entry (reset to 100 / 0 / 90 / 50 on every push).
#define SND_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SND_OPTS_MAX(a, b) ((a) >= (b) ? (a) : (b))

// cSndPlayOpts (prototype class): the play-option stack; push = cur++ and copy def into mCurrent[cur]. The
// allocation tags of its ctor (sound/ps2soundman) name the arrays: ch = mCurrent, p34 = mPosStatic,
// vals = mBaseVol, p3C = mVolControl, p60 = mPtr, p64 = mViewport, p68 = mVanish, p6C = mRadius, p70 = mDelay,
// p74 = mExternalVol, p78 = mMapToMaxVol, p7C = mCloseOnEnd, p80 = mTwoPlayerPan, p84 = mTwoPlayerSide.
// cBankManager as this unit sees it: only its cSndPlayOpts part at 0x1D8 is used here.
// handle lists (tCrowdEventData_tCrowdEventData clears them; tCrowdEventData_GetRandomSample picks from one) and their timing floats.
extern "C" void* tCrowdEventData_tCrowdEventData(tCrowdEventData* self);
extern sSndCallback D_004A3700_cb[] __asm__("D_004A3700");
extern "C" void cMidiSys_OpenMidiOpts_c(sSndSys8140* self, int a, int b) __asm__("cMidiSys_OpenMidiOpts");
extern "C" int cMidiSys_PlayMidi_c(sSndSys8140* self) __asm__("cMidiSys_PlayMidi");
// cSndPlayOpts::OPTS_Callback (prototype name): sets the handler of the voice being pushed.
// A crowd position vector (16-byte aligned, empty ctor, component access by index).
struct sCrowdVec {
    float x, y, z, w;
    sCrowdVec() {}
    float& operator[](int i) { return (&x)[i]; }
} __attribute__((aligned(16)));

// cBankSys::SFX_Begin as the crowd code uses it: pushes a voice (cSndPlayOpts push + the a4 / a8 values) with
// its mCloseOnEnd value as a parameter (the target keeps that 1 in a spilled pseudo that reload rematerialises
// after the mDelay stores; with a literal it is a fresh li).
static inline void crowdBegin(void* vself, int a, int b, int closeOnEnd)
{
    cBankSys* self = (cBankSys*)vself;
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    s->mCallback[m->BankMonitor.mInst] = 0;
    s->mPtr[m->BankMonitor.mInst] = 0;
    s->mViewport[m->BankMonitor.mInst] = 0;
    s->mVanish[m->BankMonitor.mInst] = 0;
    s->mRadius[m->BankMonitor.mInst] = 0;
    s->mDelay[m->BankMonitor.mInst].mCount = 0;
    s->mDelay[m->BankMonitor.mInst].mTime = 100;
    s->mDelay[m->BankMonitor.mInst].mFeedback = 90;
    s->mDelay[m->BankMonitor.mInst].mLevel = 50;
    s->mExternalVol[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerPan[m->BankMonitor.mInst] = 0;
    s->mTwoPlayerSide[m->BankMonitor.mInst] = 0;
    s->mMapToMaxVol[m->BankMonitor.mInst] = 0x7F;
    s->mCloseOnEnd[m->BankMonitor.mInst] = closeOnEnd;
    self->mBank[self->mpVBankManager->BankMonitor.mInst] = a;
    self->mPatch[self->mpVBankManager->BankMonitor.mInst] = b;
}

// The crowd sound object (sound/crowdsys.cpp in the prototype; member names from its asserts where known).
struct sCrowdGrp4E88 {
    int key;                 // 0x00
    int handle;              // 0x04
    int cnt;                 // 0x08
    int pad0C;
    sCrowdVec vec;            // 0x10
    int idx;                 // 0x20
    char pad24[0xC];
};
struct sCrowdSlot4E88 {
    sCrowdVec* pCrowdObjectPos; // 0x00
    int loudness;                  // 0x04
    sCrowdGrp4E88* grp;      // 0x08
};
struct sCrowd4E88 {
    int mViewPort;           // 0x000
    char pad4[0xC];
    sCrowdVec vec10;          // 0x010
    sCrowdSlot4E88 slots[15]; // 0x020
    int count;               // 0x0D4
    int mCrowdBank;                 // 0x0D8
    char padDC[4];
    sCrowdGrp4E88 groups[3]; // 0x0E0
    int mode;                // 0x170
    int handle;              // 0x174
    int vol;                 // 0x178
    float* mpVolControl;                // 0x17C
    int state;               // 0x180
    char pad184[8];
    int mCurrentCheer;                // 0x18C
};
// The voice cBankMonitor_BANKMONITOR_GetHandle hands back (only its volume is set here).
struct sCrowdVoice {
    char pad0[0x64];
    int vol;                 // 0x64
};
struct sCrowdVt4E88 { short delta; short index; int (*fn)(void*, int, int*, int*); };
extern "C" int tCrowdEventData_GetRandomSample(sSndRandList* list);

// cCrowdInstance_Update is declared with a void* parameter further down (the crowd update loop).
void cCrowdInstance_Update_c(sCrowd4E88* self) __asm__("cCrowdInstance_Update");
/**
 * Updates a crowd instance each frame: its averaged position and loudness, its ambient loops and its cheer voice.
 *
 * With no crowd objects it does nothing. The objects' positions are averaged, each weighted by its loudness against the
 * loudest so far, and every quieter object adds a quarter of its loudness; with two or more ambient loops
 * (mNumCrowdAmbience) the same is done per loop. With one loop, that loop follows the overall average at the overall
 * loudness; otherwise each used loop follows its own. A loop whose voice is gone is restarted (bank mCrowdBank, its
 * sampleID, the vol control mpVolControl, viewport mViewPort, callback D_004A3700); a live one just gets the volume.
 * When a cheer is wanted (mCheerState), the crowd system's vtable slot 2 (in cSSXAudio cSSXAudio_GetTrickPowerLevel) gives the
 * anticipation intensity 0..2 or 3 (none); 1 and 2 are kept in mCurrentCheer, and state 1 with an intensity starts a
 * random ANTICIPATE sample of it (cMidiSys_OpenMidiOpts / cMidiSys_PlayMidi) at the averaged position, moving to state 2. A cheer
 * voice that has ended is stopped (cCrowdInstance_StopCheer) and mCurrentCheer cleared; a live one gets the loudness. The loudness
 * and position are stored in mLoudness and mCrowdPos. cCrowdSys_Update calls it for every instance.
 */
void cCrowdInstance_Update_c(sCrowd4E88* self)
{
    if (self->count == 0) {
        return;
    }
    int k = 0;
    int bonus = 0;
    sCrowdVec mean;
    mean[0] = 0;
    mean[1] = 0;
    mean[2] = 0;
    mean[3] = 0;
    sCrowdVec chanPos[3];
    int chanDist[3];
    int chanSum[3];
    int i;
    for (i = 0; i < 3; i++) {
        chanPos[i][0] = 0;
        chanPos[i][1] = 0;
        chanPos[i][2] = 0;
        chanPos[i][3] = 0;
        chanDist[i] = 0;
        chanSum[i] = 0;
    }
    sCrowdSlot4E88* pCurrCrowdObjectInfo;
    sCrowdGrp4E88* pCurrAmbientInfo;
    do {
        pCurrCrowdObjectInfo = &self->slots[k];
        pCurrAmbientInfo = pCurrCrowdObjectInfo->grp;
        k++;
        if (pCurrCrowdObjectInfo->pCrowdObjectPos != 0) {
            break;
        }
    } while (k < 15);
    mean = *pCurrCrowdObjectInfo->pCrowdObjectPos;
    int best = pCurrCrowdObjectInfo->loudness;
    chanPos[pCurrAmbientInfo->idx] = mean;
    chanDist[pCurrAmbientInfo->idx] = pCurrCrowdObjectInfo->loudness;
    for (i = 1; i < self->count; i++) {
        do {
            pCurrCrowdObjectInfo = &self->slots[k];
            pCurrAmbientInfo = pCurrCrowdObjectInfo->grp;
            k++;
            if (pCurrCrowdObjectInfo->pCrowdObjectPos != 0) {
                break;
            }
        } while (k < 15);
        float w = (float)pCurrCrowdObjectInfo->loudness / (float)(best + pCurrCrowdObjectInfo->loudness);
        mean[0] += ((*pCurrCrowdObjectInfo->pCrowdObjectPos)[0] - mean[0]) * w;
        mean[1] += ((*pCurrCrowdObjectInfo->pCrowdObjectPos)[1] - mean[1]) * w;
        mean[2] += ((*pCurrCrowdObjectInfo->pCrowdObjectPos)[2] - mean[2]) * w;
        if (best < pCurrCrowdObjectInfo->loudness) {
            bonus += best >> 2;
            best = pCurrCrowdObjectInfo->loudness;
        } else {
            bonus += pCurrCrowdObjectInfo->loudness >> 2;
        }
        if (self->mode >= 2) {
            w = (float)pCurrCrowdObjectInfo->loudness /
                (float)(chanDist[pCurrAmbientInfo->idx] + pCurrCrowdObjectInfo->loudness);
            chanPos[pCurrAmbientInfo->idx][0] += ((*pCurrCrowdObjectInfo->pCrowdObjectPos)[0] - chanPos[pCurrAmbientInfo->idx][0]) * w;
            chanPos[pCurrAmbientInfo->idx][1] += ((*pCurrCrowdObjectInfo->pCrowdObjectPos)[1] - chanPos[pCurrAmbientInfo->idx][1]) * w;
            chanPos[pCurrAmbientInfo->idx][2] += ((*pCurrCrowdObjectInfo->pCrowdObjectPos)[2] - chanPos[pCurrAmbientInfo->idx][2]) * w;
            if (chanDist[pCurrAmbientInfo->idx] < pCurrCrowdObjectInfo->loudness) {
                chanSum[pCurrAmbientInfo->idx] += chanDist[pCurrAmbientInfo->idx] >> 2;
                chanDist[pCurrAmbientInfo->idx] = pCurrCrowdObjectInfo->loudness;
            } else {
                chanSum[pCurrAmbientInfo->idx] += pCurrCrowdObjectInfo->loudness >> 2;
            }
        }
    }
    best += bonus;
    if (self->mode == 1) {
        if (pCurrAmbientInfo->handle >= 0) {
            sCrowdVec* pos = &pCurrAmbientInfo->vec;
            pCurrAmbientInfo->vec = mean;
            sCrowdVoice* voiceInst
                    = (sCrowdVoice*)cBankMonitor_BANKMONITOR_GetHandle(&gpCrowdSys->mpVBankManager->BankMonitor,
                    pCurrAmbientInfo->handle, 1);
            if (voiceInst == 0) {
                crowdBegin((sSndSys69A0*)gpCrowdSys, self->mCrowdBank, pCurrAmbientInfo->key, 1);
                {
                    cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                    bankMgr->BankMonitor.mPosition[bankMgr->BankMonitor.mInst] = (const t4Vector*)pos;
                }
                {
                    cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                    cBankMonitor* ss = &bankMgr->BankMonitor;
                    ss->mBaseVol[bankMgr->BankMonitor.mInst] = best;
                    ss->mCurrent[bankMgr->BankMonitor.mInst].vol = SND_MAX(0, SND_MIN(ss->mBaseVol[ss->mInst], 0x7F));
                }
                {
                    cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                    bankMgr->BankMonitor.mVolControl[bankMgr->BankMonitor.mInst] = self->mpVolControl;
                }
                {
                    cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                    bankMgr->BankMonitor.mExternalVol[bankMgr->BankMonitor.mInst] = 1;
                }
                OPTS_Callback(gpCrowdSys->mpVBankManager, D_004A3700_cb[0]);
                {
                    cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                    bankMgr->BankMonitor.mPtr[bankMgr->BankMonitor.mInst] = self;
                }
                {
                    cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                    bankMgr->BankMonitor.mCloseOnEnd[bankMgr->BankMonitor.mInst] = 0;
                }
                {
                    cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                    bankMgr->BankMonitor.mViewport[bankMgr->BankMonitor.mInst] = self->mViewPort;
                }
                pCurrAmbientInfo->handle = ((cBankSys*)gpCrowdSys)->SFX_End();
            } else {
                voiceInst->vol = best;
            }
        }
    } else {
        for (i = 0; i < 3; i++) {
            pCurrAmbientInfo = &self->groups[i];
            if (pCurrAmbientInfo->cnt != 0 && pCurrAmbientInfo->handle >= 0) {
                sCrowdVec* pos = &pCurrAmbientInfo->vec;
                pCurrAmbientInfo->vec = chanPos[pCurrAmbientInfo->idx];
                sCrowdVoice* voiceInst
                        = (sCrowdVoice*)cBankMonitor_BANKMONITOR_GetHandle(&gpCrowdSys->mpVBankManager->BankMonitor,
                        pCurrAmbientInfo->handle, 1);
                if (voiceInst == 0) {
                    crowdBegin((sSndSys69A0*)gpCrowdSys, self->mCrowdBank, pCurrAmbientInfo->key, 1);
                    {
                        cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                        bankMgr->BankMonitor.mPosition[bankMgr->BankMonitor.mInst] = (const t4Vector*)pos;
                    }
                    {
                        cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                        cBankMonitor* ss = &bankMgr->BankMonitor;
                        ss->mBaseVol[bankMgr->BankMonitor.mInst] = chanDist[pCurrAmbientInfo->idx]
                                + chanSum[pCurrAmbientInfo->idx];
                        ss->mCurrent[bankMgr->BankMonitor.mInst].vol = SND_MAX(0, SND_MIN(ss->mBaseVol[ss->mInst],
                                0x7F));
                    }
                    {
                        cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                        bankMgr->BankMonitor.mVolControl[bankMgr->BankMonitor.mInst] = self->mpVolControl;
                    }
                    {
                        cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                        bankMgr->BankMonitor.mExternalVol[bankMgr->BankMonitor.mInst] = 1;
                    }
                    {
                        cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                        bankMgr->BankMonitor.mViewport[bankMgr->BankMonitor.mInst] = self->mViewPort;
                    }
                    OPTS_Callback(gpCrowdSys->mpVBankManager, D_004A3700_cb[0]);
                    {
                        cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                        bankMgr->BankMonitor.mPtr[bankMgr->BankMonitor.mInst] = self;
                    }
                    {
                        cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                        bankMgr->BankMonitor.mCloseOnEnd[bankMgr->BankMonitor.mInst] = 0;
                    }
                    pCurrAmbientInfo->handle = ((cBankSys*)gpCrowdSys)->SFX_End();
                } else {
                    voiceInst->vol = chanDist[pCurrAmbientInfo->idx] + chanSum[pCurrAmbientInfo->idx];
                }
            }
        }
    }
    int outA = 0x1000;
    int outB = 0x1000;
    if (self->state != 0) {
        cCrowdSys* sys = gpCrowdSys;
        sCrowdVt4E88* vt = (sCrowdVt4E88*)((void*)sys->mVtbl);
        int anticipateLevel = vt[2].fn(vthis(sys, vt[2]), self->mViewPort, &outA, &outB);
        if (anticipateLevel == 1 || anticipateLevel == 2) {
            self->mCurrentCheer = anticipateLevel;
        }
        if (self->state == 1 && anticipateLevel != 3) {
            cCrowdInstance_StopCheer((cCrowdInstance*)self);
            int cheerSample = tCrowdEventData_GetRandomSample((sSndRandList*)&((sSndSys69A0*)gpCrowdSys)->ent[anticipateLevel].c);
            if (cheerSample != 0) {
                cMidiSys_OpenMidiOpts_c((sSndSys69A0*)gpCrowdSys, self->mCrowdBank, cheerSample);
                {
                    cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                    bankMgr->BankMonitor.mPosition[bankMgr->BankMonitor.mInst] = (const t4Vector*)&self->vec10;
                }
                {
                    cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                    cBankMonitor* ss = &bankMgr->BankMonitor;
                    ss->mBaseVol[bankMgr->BankMonitor.mInst] = best * outB >> 12;
                    ss->mCurrent[bankMgr->BankMonitor.mInst].vol = SND_MAX(0, SND_MIN(ss->mBaseVol[ss->mInst], 0x7F));
                }
                {
                    cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                    bankMgr->BankMonitor.mVolControl[bankMgr->BankMonitor.mInst] = self->mpVolControl;
                }
                {
                    cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                    bankMgr->BankMonitor.mExternalVol[bankMgr->BankMonitor.mInst] = 1;
                }
                {
                    cBankManager* bankMgr = gpCrowdSys->mpVBankManager;
                    bankMgr->BankMonitor.mViewport[bankMgr->BankMonitor.mInst] = self->mViewPort;
                }
                self->handle = cMidiSys_PlayMidi_c((sSndSys69A0*)gpCrowdSys);
                if (self->handle >= 0) {
                    self->state = 2;
                }
            }
        }
    }
    if (self->handle >= 0) {
        sCrowdVoice* voiceInst
                = (sCrowdVoice*)cBankMonitor_BANKMONITOR_GetHandle(&gpCrowdSys->mpVBankManager->BankMonitor,
                self->handle, 2);
        if (voiceInst == 0) {
            cCrowdInstance_StopCheer((cCrowdInstance*)self);
            self->mCurrentCheer = 0;
        } else {
            int cheerVolume = best;
            if (self->state == 2) {
                cBankMonitorInstance_PitchUpdate(voiceInst, outA);
                cheerVolume = best * outB >> 12;
            }
            voiceInst->vol = cheerVolume;
        }
    }
    self->vol = best;
    self->vec10 = mean;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdInstance_SetObjectLoudness);
#ifdef SKIP_ASM
struct s2A5CD8Entry {
    int active;
    int value;
    int pad;
};

struct s2A5CD8 {
    char pad[0x20];
    s2A5CD8Entry entries[1];
};

/**
 * Sets the loudness of one crowd object of a crowd instance, when that slot is in use.
 *
 * cCrowdSys_SetObjectLoudness calls it for a world trigger's crowd.
 *
 * @param i  the crowd object slot, or a negative value for none
 * @param newValue  the loudness, 0..127
 */
extern "C" void cCrowdInstance_SetObjectLoudness(s2A5CD8* self, int i, int newValue)
{
    if (i >= 0) {
        if (self->entries[i].active) {
            self->entries[i].value = newValue;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cCrowdInstance_AddObject);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" int cBankSys_SFX_End(cBankSys* self);


struct sSnd5D08Sys {
    cBankManager* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
};
extern tSndCallback D_004A3700_5D08[] __asm__("D_004A3700");

#define SND_5D08_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_5D08_MAX(a, b) ((a) > (b) ? (a) : (b))

// PORT: 64-bit long sound key
int cCrowdInstance_AddObject_5D08(cCrowdInstance* self, cBankMonitor* P, int key) __asm__("cCrowdInstance_AddObject");
/**
 * Adds a crowd object to a crowd instance and starts or joins the ambient loop of its sample; returns its slot.
 *
 * Fails (-1) with 15 objects already or no position target in the current play-option level @p objectOpts. The object takes a
 * free slot with the level's position pointer and volume (as its loudness). It joins the ambient loop playing the same
 * sample (@p key); the first object also sets the instance's viewport, vol control and averaged position. Without a
 * matching loop it takes a free one (mNumCrowdAmbience grows), places it at the object and starts the sample from the
 * instance's bank mCrowdBank at the loop's position, with the object's loudness, the vol control, external volume, the
 * viewport and the callback D_004A3700, keeping the handle. cCrowdSys_AddObject calls it.
 *
 * @param objectOpts  the current play-option level (cBankManager + 0x1D8) that describes the object
 * @param key  the ambient sample (patch) of the object
 * @return the crowd object slot, or -1
 */
int cCrowdInstance_AddObject_5D08(cCrowdInstance* self, cBankMonitor* objectOpts, int key)
{
    int idx = -1;
    int cnt = self->mNumCrowdObjects;
    if (cnt >= 15 || objectOpts->mPosition[objectOpts->mInst] == 0) {
        return -1;
    }
    tCrowdObjectInfo* found = 0;
    int i;
    for (i = 0; i < 15; i++) {
        if (found == 0 && ((t4Vector*)self->mCrowdObjectsInfo[i].pCrowdObjectPos) == 0) {
            found = &self->mCrowdObjectsInfo[i];
            idx = i;
        }
    }
    self->mNumCrowdObjects++;
    found->pCrowdObjectPos = objectOpts->mPosition[objectOpts->mInst];
    found->loudness = objectOpts->mBaseVol[objectOpts->mInst];
    i = 0;
    do {
        if (self->mCrowdAmbientInfo[i].sampleID == key) {
            self->mCrowdAmbientInfo[i].numObjects++;
            found->pInfo = &self->mCrowdAmbientInfo[i];
            break;
        }
        i++;
    } while (i < 3);
    if (self->mNumCrowdObjects == 1) {
        self->mViewPort = objectOpts->mViewport[objectOpts->mInst];
        self->mpVolControl = objectOpts->mVolControl[objectOpts->mInst];
        self->mCrowdPos = *((t4Vector*)found->pCrowdObjectPos);
    }
    if (found->pInfo == 0) {
        int groupIndex = 0;
        do {
            if (self->mCrowdAmbientInfo[groupIndex].numObjects == 0) {
                found->pInfo = &self->mCrowdAmbientInfo[groupIndex];
                self->mCrowdAmbientInfo[groupIndex].sampleID = key;
                self->mCrowdAmbientInfo[groupIndex].numObjects++;
                self->mNumCrowdAmbience++;
                break;
            }
            groupIndex++;
        } while (groupIndex < 3);
        found->pInfo->averagePos = *((t4Vector*)found->pCrowdObjectPos);
        cCrowdSys* sys = gpCrowdSys;
        cBankManager* bankMgr = (cBankManager*)sys->mpVBankManager;
        int crowdBank = self->mCrowdBank;
        cBankMonitor* playOpts = &bankMgr->BankMonitor;
        bankMgr->BankMonitor.mInst++;
        playOpts->mCurrent[bankMgr->BankMonitor.mInst] = bankMgr->BankMonitor.mDefault;
        bankMgr->BankMonitor.mPosition[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mPosStatic[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mBaseVol[bankMgr->BankMonitor.mInst] = playOpts->mCurrent[bankMgr->BankMonitor.mInst].vol;
        playOpts->mVolControl[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mCallback[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mPtr[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mViewport[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mVanish[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mRadius[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mDelay[bankMgr->BankMonitor.mInst].mCount = 0;
        playOpts->mDelay[bankMgr->BankMonitor.mInst].mTime = 100;
        playOpts->mDelay[bankMgr->BankMonitor.mInst].mFeedback = 90;
        playOpts->mDelay[bankMgr->BankMonitor.mInst].mLevel = 50;
        playOpts->mExternalVol[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mTwoPlayerPan[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mTwoPlayerSide[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mMapToMaxVol[bankMgr->BankMonitor.mInst] = 0x7F;
        playOpts->mCloseOnEnd[bankMgr->BankMonitor.mInst] = 1;
        sys->mBank[((cBankManager*)sys->mpVBankManager)->BankMonitor.mInst] = crowdBank;
        sys->mPatch[((cBankManager*)sys->mpVBankManager)->BankMonitor.mInst] = key;
        {
            cCrowdSys* crowdSys = gpCrowdSys;
            ((cBankManager*)crowdSys->mpVBankManager)->BankMonitor.mPosition[
                    ((cBankManager*)crowdSys->mpVBankManager)->BankMonitor.mInst] = &found->pInfo->averagePos;
            cBankManager* curBankMgr = (cBankManager*)crowdSys->mpVBankManager;
            cBankMonitor* ss = &curBankMgr->BankMonitor;
            ss->mBaseVol[curBankMgr->BankMonitor.mInst] = found->loudness;
            ss->mCurrent[curBankMgr->BankMonitor.mInst].vol = SND_5D08_MAX(0, SND_5D08_MIN(ss->mBaseVol[ss->mInst],
                    0x7F));
        }
        {
            cBankManager* curBankMgr = (cBankManager*)gpCrowdSys->mpVBankManager;
            curBankMgr->BankMonitor.mVolControl[curBankMgr->BankMonitor.mInst] = self->mpVolControl;
        }
        {
            cBankManager* curBankMgr = (cBankManager*)gpCrowdSys->mpVBankManager;
            curBankMgr->BankMonitor.mExternalVol[curBankMgr->BankMonitor.mInst] = 1;
        }
        {
            cBankManager* curBankMgr = (cBankManager*)gpCrowdSys->mpVBankManager;
            curBankMgr->BankMonitor.mViewport[curBankMgr->BankMonitor.mInst] = self->mViewPort;
        }
        {
            cBankManager* curBankMgr = (cBankManager*)gpCrowdSys->mpVBankManager;
            tSndCallback k = D_004A3700_5D08[0];
            cBankMonitor* callbackOpts = &curBankMgr->BankMonitor;
            callbackOpts->mCallback[curBankMgr->BankMonitor.mInst] = k;
        }
        {
            cBankManager* curBankMgr = (cBankManager*)gpCrowdSys->mpVBankManager;
            curBankMgr->BankMonitor.mPtr[curBankMgr->BankMonitor.mInst] = self;
        }
        {
            cBankManager* curBankMgr = (cBankManager*)gpCrowdSys->mpVBankManager;
            curBankMgr->BankMonitor.mCloseOnEnd[curBankMgr->BankMonitor.mInst] = 0;
        }
        found->pInfo->handle = ((cBankSys*)gpCrowdSys)->SFX_End();
    }
    return idx;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdInstance_RemoveObject);
#ifdef SKIP_ASM

struct sSsVoice62F0 { int sampleID; int id; int refs; };
struct sSsEnt62F0 { int active; int f4; sSsVoice62F0* v; };
struct sSsMgr62F0 {
    int mViewPort;
    char pad4[0x20 - 0x4];
    sSsEnt62F0 ent[15];         // 0x20
    int nactive;                // 0xD4
    char padD8[0x170 - 0xD8];
    int nvoices;                // 0x170
    int id;                     // 0x174
    int mLoudness;                   // 0x178
    int f17C;
    int state;                  // 0x180
    int f184;
    float vol;                  // 0x188
    int mCurrentCheer;                   // 0x18C
};

/**
 * Removes a crowd object from a crowd instance, stopping its ambient loop when no object feeds it any more.
 *
 * Does nothing for a negative or unused slot. The slot is freed and its loop's numObjects drops; at 0 the loop's voice
 * is stopped (mode 1 with @p fade), its sampleID and handle reset to -1, mCurrentCheer cleared and mNumCrowdAmbience
 * lowered. When the last object goes, the cheer time mCheerTimeLeft is capped at 0.25, the cheer voice is stopped (only
 * when
 * @p fade is 0) and forgotten, a playing anticipation (state 2) drops back to 1, and mLoudness and mViewPort are reset.
 * cCrowdInstance_RemoveAllObjects and cCrowdSys_RemoveObject call it.
 *
 * @param vself  the crowd instance
 * @param idx  the crowd object slot
 * @param fade  the fade time for the loop's voice
 */
extern "C" void cCrowdInstance_RemoveObject(cCrowdInstance* vself, int idx, float fade)
{
    sSsMgr62F0* self = (sSsMgr62F0*)vself;
    if (idx < 0) return;
    if (self->ent[idx].active == 0) return;
    sSsVoice62F0* ambientInfo = self->ent[idx].v;
    self->nactive--;
    self->ent[idx].active = 0;
    self->ent[idx].v = 0;
    if (--ambientInfo->refs == 0) {
        if (ambientInfo->id >= 0) {
            cBankMonitor_BANKMONITOR_StopHandle(&gpCrowdSys->mpVBankManager->BankMonitor, ambientInfo->id, 1, fade);
        }
        ambientInfo->id = -1;
        ambientInfo->sampleID = -1;
        self->mCurrentCheer = 0;
        self->nvoices--;
    }
    if (self->nactive != 0) return;
    if (self->vol > 0.25f) self->vol = 0.25f;
    if (self->id >= 0) {
        if (fade == 0.0f) {
            cBankMonitor_BANKMONITOR_StopHandle(&gpCrowdSys->mpVBankManager->BankMonitor, self->id, 2, fade);
        }
        self->id = -1;
        if (self->state == 2) self->state = 1;
    }
    self->mLoudness = 0;
    self->mViewPort = -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cCrowdInstance_RemoveAllObjects);
#ifdef SKIP_ASM
extern "C" void cCrowdInstance_RemoveObject(cCrowdInstance* voice, int b, float v);

/**
 * Removes every crowd object of a crowd instance (cCrowdInstance_RemoveObject on each used slot) with the given fade.
 *
 * @param fade  the fade time for the ambient loops
 */
extern "C" void cCrowdInstance_RemoveAllObjects(cCrowdInstance* self, float fade)
{
    for (int i = 0; i < 15; i++) {
        if (self->mCrowdObjectsInfo[i].pCrowdObjectPos != 0) {
            cCrowdInstance_RemoveObject(self, i, fade);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdInstance_PlayTrickCheer);
#ifdef SKIP_ASM
extern "C" void cCrowdInstance_StopCheer(cCrowdInstance* self);
extern "C" int tCrowdEventData_GetRandomSample(sSndRandList* list);
extern "C" void cMidiSys_OpenMidiOpts(char** sys, int a, int b);
extern "C" int cMidiSys_PlayMidi(char** sys);


#define S64_MIN(a, b) ((a) < (b) ? (a) : (b))
#define S64_MAX(a, b) ((a) >= (b) ? (a) : (b))

// cSndPlayOpts::OPTS_Vol as the crowd code inlines it: the level's base volume and its mCurrent volume, clamped to
// 0..127.
static inline void crowdOptsSetVol64A0(cBankMonitor* o, int i, int v)
{
    o->mBaseVol[i] = v;
}
static inline void crowdOptsUpdate64A0(cBankMonitor* o, int i)
{
    o->mCurrent[i].vol = S64_MAX(S64_MIN(o->mBaseVol[o->mInst], 0x7F), 0);
}

/**
 * Plays a TRICK cheer of a crowd instance at an intensity: a random sample of that intensity's TRICK list.
 *
 * Stops the current cheer (cCrowdInstance_StopCheer). With crowd objects present it records the intensity in
 * mAmbientCheerIntensity, sets the cheer time mCheerTimeLeft to mAttack + mDecay and, when the list has a sample, plays
 * it from bank mCrowdBank (cMidiSys_OpenMidiOpts / cMidiSys_PlayMidi) at the instance's averaged position, loudness mLoudness, vol
 * control mpVolControl, external volume and viewport mViewPort; the handle goes to mCheerHandle and mCurrentCheer
 * becomes intensity + 1. cCrowdSys_PlayTrickCheer calls it.
 *
 * @param vself  the crowd instance
 * @param idx  the crowd intensity (0..2)
 */
extern "C" void cCrowdInstance_PlayTrickCheer(cCrowdInstance* vself, int idx)
{
    cCrowdInstance_StopCheer(vself);
    if (vself->mNumCrowdObjects == 0) return;
    tCrowdData* crowdData = &((&gpCrowdSys->mCrowdData[0]))[idx];
    vself->mAmbientCheerIntensity = idx;
    vself->mCheerTimeLeft = crowdData->mAttack + crowdData->mDecay;
    int cheerSample = tCrowdEventData_GetRandomSample((sSndRandList*)crowdData);
    if (cheerSample == 0) return;
    cMidiSys_OpenMidiOpts((char**)gpCrowdSys, vself->mCrowdBank, cheerSample);
    cBankManager* posMgr = gpCrowdSys->mpVBankManager;
    posMgr->BankMonitor.mPosition[posMgr->BankMonitor.mInst] = &vself->mCrowdPos;
    cBankManager* volMgr = gpCrowdSys->mpVBankManager;
    crowdOptsSetVol64A0(&volMgr->BankMonitor, volMgr->BankMonitor.mInst, vself->mLoudness);
    crowdOptsUpdate64A0(&volMgr->BankMonitor, volMgr->BankMonitor.mInst);
    cBankManager* ctlMgr = gpCrowdSys->mpVBankManager;
    ctlMgr->BankMonitor.mVolControl[ctlMgr->BankMonitor.mInst] = vself->mpVolControl;
    cBankManager* extMgr = gpCrowdSys->mpVBankManager;
    extMgr->BankMonitor.mExternalVol[extMgr->BankMonitor.mInst] = 1;
    cBankManager* viewMgr = gpCrowdSys->mpVBankManager;
    viewMgr->BankMonitor.mViewport[viewMgr->BankMonitor.mInst] = vself->mViewPort;
    vself->mCheerHandle = cMidiSys_PlayMidi((char**)gpCrowdSys);
    vself->mCurrentCheer = idx + 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdInstance_PlayFallCheer);
#ifdef SKIP_ASM
extern "C" void cCrowdInstance_StopCheer(cCrowdInstance* self);
extern "C" int tCrowdEventData_GetRandomSample(sSndRandList* list);
extern "C" void cMidiSys_OpenMidiOpts(char** sys, int a, int b);
extern "C" int cMidiSys_PlayMidi(char** sys);


#define S66_MIN(a, b) ((a) < (b) ? (a) : (b))
#define S66_MAX(a, b) ((a) >= (b) ? (a) : (b))

// cSndPlayOpts::OPTS_Vol as the crowd code inlines it: the level's base volume and its mCurrent volume, clamped to
// 0..127.
static inline void crowdOptsSetVol6648(cBankMonitor* o, int i, int v)
{
    o->mBaseVol[i] = v;
}
static inline void crowdOptsUpdate6648(cBankMonitor* o, int i)
{
    o->mCurrent[i].vol = S66_MAX(S66_MIN(o->mBaseVol[o->mInst], 0x7F), 0);
}

/**
 * Plays a FALL cheer of a crowd instance at an intensity: a random sample of that intensity's FALL list.
 *
 * As cCrowdInstance_PlayTrickCheer with the FALL list (tCrowdData + 0x18); mCurrentCheer becomes ~intensity (negative). cCrowdSys_PlayFallCheer
 * calls it.
 *
 * @param vself  the crowd instance
 * @param idx  the crowd intensity (0..2)
 */
extern "C" void cCrowdInstance_PlayFallCheer(cCrowdInstance* vself, int idx)
{
    cCrowdInstance_StopCheer(vself);
    if (vself->mNumCrowdObjects == 0) return;
    tCrowdData* crowdData = &((&gpCrowdSys->mCrowdData[0]))[idx];
    vself->mAmbientCheerIntensity = idx;
    vself->mCheerTimeLeft = crowdData->mAttack + crowdData->mDecay;
    int cheerSample = tCrowdEventData_GetRandomSample((sSndRandList*)&crowdData->mFall);
    if (cheerSample == 0) return;
    cMidiSys_OpenMidiOpts((char**)gpCrowdSys, vself->mCrowdBank, cheerSample);
    cBankManager* posMgr = gpCrowdSys->mpVBankManager;
    posMgr->BankMonitor.mPosition[posMgr->BankMonitor.mInst] = &vself->mCrowdPos;
    cBankManager* volMgr = gpCrowdSys->mpVBankManager;
    crowdOptsSetVol6648(&volMgr->BankMonitor, volMgr->BankMonitor.mInst, vself->mLoudness);
    crowdOptsUpdate6648(&volMgr->BankMonitor, volMgr->BankMonitor.mInst);
    cBankManager* ctlMgr = gpCrowdSys->mpVBankManager;
    ctlMgr->BankMonitor.mVolControl[ctlMgr->BankMonitor.mInst] = vself->mpVolControl;
    cBankManager* extMgr = gpCrowdSys->mpVBankManager;
    extMgr->BankMonitor.mExternalVol[extMgr->BankMonitor.mInst] = 1;
    cBankManager* viewMgr = gpCrowdSys->mpVBankManager;
    viewMgr->BankMonitor.mViewport[viewMgr->BankMonitor.mInst] = vself->mViewPort;
    vself->mCheerHandle = cMidiSys_PlayMidi((char**)gpCrowdSys);
    vself->mCurrentCheer = ~idx;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdInstance_RequestAnticipation);
#ifdef SKIP_ASM
/**
 * Asks a crowd instance for an anticipation cheer: the cheer state mCheerState goes from 0 to 1 (cCrowdInstance_Update starts
 * it).
 */
extern "C" void cCrowdInstance_RequestAnticipation(cCrowdInstance* self)
{
    if (self->mCheerState == 0) {
        self->mCheerState = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdInstance_CancelAnticipation);
#ifdef SKIP_ASM
extern "C" void cCrowdInstance_StopCheer(cCrowdInstance* self);

/**
 * Cancels a crowd instance's anticipation: stops the cheer when one is playing (state 2) and sets the state to 0.
 */
extern "C" void cCrowdInstance_CancelAnticipation(cCrowdInstance* self)
{
    if (self->mCheerState == 2) {
        cCrowdInstance_StopCheer(self);
    }
    self->mCheerState = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdInstance_StopCheer);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Stops a crowd instance's cheer: caps the cheer time mCheerTimeLeft at 0.25, clears the state and stops the voice
 * mCheerHandle.
 *
 * The voice is stopped with cBankMonitor_BANKMONITOR_StopHandle (mode 2, fade 0) and mCheerHandle reset to -1.
 */
extern "C" void cCrowdInstance_StopCheer(cCrowdInstance* self)
{
    float lim = 0.25f;
    if (lim < self->mCheerTimeLeft) {
        self->mCheerTimeLeft = lim;
    }
    self->mCheerState = 0;
    if (self->mCheerHandle >= 0) {
        cBankMonitor_BANKMONITOR_StopHandle(&gpCrowdSys->mpVBankManager->BankMonitor, self->mCheerHandle, 2, 0.0f);
        self->mCheerHandle = -1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdInstance_UpdateLoopPitch);
#ifdef SKIP_ASM
extern "C" void cBankMonitorInstance_Reset(void*);
extern "C" void SNDpitchbend(int, int);

/**
 * Bends the pitch of a crowd ambient loop over a cheer's attack and decay, or resets the monitor once the voice is
 * gone.
 *
 * A voice handle (+4) below 0 resets the instance (cBankMonitorInstance_Reset). Otherwise, while the cheer time
 * mCheerTimeLeft is above 0 it is lowered by @p dt and the pitch bend (SNDpitchbend, in a sound-driver critical
 * section) is set from the intensity's tCrowdData: rising to 0x40 + mMaxBend during mAttack, then falling back to 0x40
 * over mDecay. cCrowdSys_LoopPitchCallback (the loops' callback D_004A3700) calls it.
 *
 * @param obj  the voice's monitor instance (cBankMonitorInstance)
 * @param dt  the elapsed time
 */
extern "C" void cCrowdInstance_UpdateLoopPitch(cCrowdInstance* self, cBankMonitorInstance* obj, float dt) {
    if (obj->mHandle < 0) {
        cBankMonitorInstance_Reset(obj);
        return;
    }
    float timeLeft = self->mCheerTimeLeft;
    if (!(timeLeft <= 0.0f)) {
        int idx = self->mAmbientCheerIntensity;
        cCrowdSys* base = gpCrowdSys;
        float rem = timeLeft - dt;
        self->mCheerTimeLeft = rem;
        tCrowdData* crowdData = &base->mCrowdData[idx];
        float decayTime = crowdData->mDecay;
        int vol = 0x40;
        if (decayTime < rem) {
            float bendOffset = crowdData->mMaxBend * (crowdData->mAttack - (rem - decayTime)) / crowdData->mAttack;
            vol = (int)bendOffset + 0x40;
        } else if (rem > 0.0f) {
            float bendOffset = crowdData->mMaxBend * rem / decayTime;
            vol = (int)bendOffset + 0x40;
        }
        SNDSYS_entercritical();
        SNDpitchbend(obj->mHandle, vol);
        SNDSYS_leavecritical();
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdSys_cCrowdSys);
#ifdef SKIP_ASM
extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" char* cMidiSys_cMidiSys(cMidiSys* self, int embedded, int count, int heap);
extern "C" void* tCrowdEventData_tCrowdEventData(tCrowdEventData* self);

struct sSnd69VtEnt {
    short delta;
    short index;
    void* fn;
};
struct sSnd69Vt4 {
    sSnd69VtEnt e[4];
} __attribute__((aligned(8)));
struct sSnd69Vt7 {
    sSnd69VtEnt e[7];
} __attribute__((aligned(8)));

struct sSnd69List {
    char pad0[0x18];
    sSnd69List() { tCrowdEventData_tCrowdEventData((tCrowdEventData*)(this)); }
};
struct sSnd69Ent {
    sSnd69List a;
    sSnd69List b;
    sSnd69List c;
    float f48;
    float f4C;
    int f50;
    void* operator new[](unsigned int, void* p) { return p; }
};

extern char cCrowdSys_vtbl[];
extern char cBankMonitor_vtbl[];
extern const sSnd69Vt4 D_00483818_69 __asm__("D_00483818");
// A complete cCrowdSys (the in-charge ctor builds it): g++ 2.95 puts the virtual bases after the non-virtual part.
struct cCrowdSysComplete {
    cCrowdSys mCrowdSys;         // 0x000
    cSoundSys mSoundSys;         // 0x118
    cBankManager mBankManager;   // 0x120
};
extern const sSnd69Vt7 D_00483838_69 __asm__("D_00483838");


/**
 * Constructs the crowd sound system.
 *
 * When @p embedded is set (a complete object) it builds the virtual bases first: cSoundSys at +0x118 and cBankManager
 * at +0x120. Then constructs the cMidiSys base (cMidiSys_cMidiSys), installs the cCrowdSys vtables (cCrowdSys_vtbl at
 * mVtbl, D_00483818 / D_00483838 in the virtual bases, adjusted on the stack when not in charge), clears mNumInstances
 * and mpCrowdInstances, constructs the three tCrowdData sample lists (mCrowdData) and stores itself in gpCrowdSys.
 *
 * @param embedded  g++ in-charge flag: nonzero when the virtual bases are built here (the complete object)
 * @param count  maximum number of banks (On Tour MaxBanks of the base constructors)
 * @param maxStreams  not used
 * @param heap  audio mode (On Tour eSOUNDDEF_MODE audioMode), passed to the base constructors
 * @return @p self
 */
// PORT: hand-expanded g++ 2.95 virtual-base ctor (vtable upcast-offset fixup on stack copies).
extern "C" char* cCrowdSys_cCrowdSys(cCrowdSys* self, int embedded, int count, int maxStreams, int heap)
{
    if (embedded) {
        cCrowdSysComplete* complete = (cCrowdSysComplete*)self;
        cSoundSys* soundSys = &complete->mSoundSys;
        self->mpVBankManager = &complete->mBankManager;
        complete->mBankManager.BankMonitor.mpVSoundSys = soundSys;
        cSoundSys_cSoundSys(soundSys, heap);
        cBankManager_cBankManager(self->mpVBankManager, 0, count, heap);
    }
    cMidiSys_cMidiSys((cMidiSys*)self, 0, count, heap);
    self->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&D_00483818_69;
    self->mpVBankManager->BankMonitor.mVtbl = (void**)cBankMonitor_vtbl;
    self->mpVBankManager->mVtbl = (void**)&D_00483838_69;
    if (!embedded) {
        sSnd69Vt4 soundSysVtbl = D_00483818_69;
        self->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&soundSysVtbl;
        char* soundSysBase = (char*)self->mpVBankManager->BankMonitor.mpVSoundSys - 0x118;
        int adj1 = ((char*)self) - soundSysBase;
        soundSysVtbl.e[1].delta = D_00483818_69.e[1].delta + adj1;
        sSnd69Vt7 bankMgrVtbl = D_00483838_69;
        self->mpVBankManager->mVtbl = (void**)&bankMgrVtbl;
        char* bankMgrBase = (char*)self->mpVBankManager - 0x120;
        int adj2 = ((char*)self) - bankMgrBase;
        bankMgrVtbl.e[1].delta = D_00483838_69.e[1].delta + adj2;
    }
    self->mVtbl = (void**)cCrowdSys_vtbl;
    self->mNumInstances = 0;
    self->mpCrowdInstances = 0;
    new ((char*)&self->mCrowdData[0]) sSnd69Ent[3];
    gpCrowdSys = (cCrowdSys*)((char**)self);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdSys__cCrowdSys);
#ifdef SKIP_ASM
extern "C" void cBankManager__cBankManager(void* self, int flags);
extern "C" void cSoundSys__cSoundSys(void* self, int flags);
void operator_delete(int*);
extern "C" void tCrowdEventData__tCrowdEventData(tCrowdEventData* self, int flags);
extern "C" void cCrowdSys_Shutdown(cCrowdSys* self);
extern "C" void cMidiSys__cMidiSys(cMidiSys* self, int flags);

struct sSnd6BVtEnt {
    short delta;
    short index;
    void* fn;
};
struct sSnd6BVt4 {
    sSnd6BVtEnt e[4];
} __attribute__((aligned(8)));
struct sSnd6BVt7 {
    sSnd6BVtEnt e[7];
} __attribute__((aligned(8)));

extern char cCrowdSys_vtbl[];
extern char cBankMonitor_vtbl[];
extern const sSnd6BVt4 D_00483818_6B __asm__("D_00483818");
extern const sSnd6BVt7 D_00483838_6B __asm__("D_00483838");

/**
 * Destroys the crowd sound system.
 *
 * Re-installs the cCrowdSys vtables, releases the crowd instances and sample banks (cCrowdSys_Shutdown), clears gpCrowdSys,
 * destroys the three tCrowdData sample lists in reverse order (tCrowdEventData__tCrowdEventData), runs the cMidiSys base destructor
 * cMidiSys__cMidiSys, destroys the virtual bases (cBankManager, cSoundSys) when bit 1 of @p flags is set and frees the
 * object when bit 0 is set.
 *
 * @param flags  destructor flags: bit 1 destroys the virtual bases, bit 0 frees the object
 */
// PORT: hand-expanded g++ 2.95 virtual-base dtor (vtable upcast-offset fixup on stack copies).
extern "C" void cCrowdSys__cCrowdSys(cCrowdSys* self, int flags)
{
    self->mVtbl = (void**)cCrowdSys_vtbl;
    self->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&D_00483818_6B;
    self->mpVBankManager->BankMonitor.mVtbl = (void**)cBankMonitor_vtbl;
    self->mpVBankManager->mVtbl = (void**)&D_00483838_6B;
    if (flags == 0) {
        sSnd6BVt4 soundSysVtbl = D_00483818_6B;
        self->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&soundSysVtbl;
        char* soundSysBase = (char*)self->mpVBankManager->BankMonitor.mpVSoundSys - 0x118;
        int adj1 = ((char*)self) - soundSysBase;
        soundSysVtbl.e[1].delta = D_00483818_6B.e[1].delta + adj1;
        sSnd6BVt7 bankMgrVtbl = D_00483838_6B;
        self->mpVBankManager->mVtbl = (void**)&bankMgrVtbl;
        char* bankMgrBase = (char*)self->mpVBankManager - 0x120;
        int adj2 = ((char*)self) - bankMgrBase;
        bankMgrVtbl.e[1].delta = D_00483838_6B.e[1].delta + adj2;
    }
    cCrowdSys_Shutdown(self);
    gpCrowdSys = 0;
    if (self->mCrowdData != 0) {
        tCrowdData* crowdEntry = self->mCrowdData + 3;
        while (self->mCrowdData != crowdEntry) {
            crowdEntry--;
            tCrowdEventData__tCrowdEventData(&crowdEntry->mAnticipate, 2);
            tCrowdEventData__tCrowdEventData(&crowdEntry->mFall, 2);
            tCrowdEventData__tCrowdEventData(&crowdEntry->mTrick, 2);
        }
    }
    cMidiSys__cMidiSys((cMidiSys*)self, 0);
    if (flags & 2) {
        cBankManager__cBankManager(self->mpVBankManager, 0);
        cSoundSys__cSoundSys(self->mpVBankManager->BankMonitor.mpVSoundSys, 0);
    }
    if (flags & 1) operator_delete((int*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cCrowdSys_Update);
#ifdef SKIP_ASM
extern "C" void cCrowdInstance_Update(void* voice);

/**
 * Updates every crowd instance (cCrowdInstance_Update); the audio update cController_Update calls it each frame.
 */
extern "C" void cCrowdSys_Update(cCrowdSys* self)
{
    for (int i = 0; i < self->mNumInstances; i++) {
        cCrowdInstance_Update(&self->mpCrowdInstances[i]);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdSys_Init);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void tCrowdEventData_FreeSamples(tCrowdEventData*);
struct sSndFileBuf;
extern "C" FileParser* FileParser_FileParser(FileParser* self, const char* name, int flags);
extern "C" bool FileParser_FindSection(FileParser* self, const char* name);
extern "C" void cCrowdSys_ReadSection(cCrowdSys* self, void* parser);
extern "C" void FileParser__FileParser(FileParser* self, int flags);
extern const char str_CrowdInstances[];
extern const char str_GLOBAL[];

struct sSnd6D78Voice : public cCrowdInstance_cCrowdInstance_sObj {
    void operator delete[](void* p, unsigned int size);
};
struct sSnd6D78Self {
    char pad0[0x14];
    int count;                  // 0x14
    sSnd6D78Voice* voices;      // 0x18
    tCrowdData ent[3];          // 0x1C (cCrowdSys::mCrowdData)
};
struct sSnd6D78Parser {
    int size;
    char* data;
    char* cur;
    char token[0x800];
};

/**
 * Sets up the crowd system: @p n crowd instances, all sample lists cleared, then crowd.inf's GLOBAL and level sections.
 *
 * Allocates the instances (operator new[], "CrowdInstances") into mpCrowdInstances, clears every intensity's mAttack,
 * mDecay, mMaxBend and sample lists, gives each instance the cheer bank @p cheerBank (unk_0D8), and reads the file
 * @p name with a FileParser: section "GLOBAL" and then section @p key go through cCrowdSys_ReadSection. cSSXAudio_InGameLoad
 * calls it with 2 instances in the multiplayer game path, else 1, and bank 5.
 *
 * @param name  the crowd.inf path
 * @param key  the section of the level
 * @param n  the number of crowd instances (viewports)
 * @param cheerBank  the bank the cheers play from
 */
extern "C" void cCrowdSys_Init(sSnd6D78Self* self, const char* name, const char* key, int n, int cheerBank)
{
    self->count = n;
    sSnd6D78Voice*& slot = self->voices;
    slot = new (str_CrowdInstances, 0, 0) sSnd6D78Voice[n];
    for (int i = 0; i < 3; i++) {
        self->ent[i].mAttack = 0.0f;
        self->ent[i].mDecay = 0.0f;
        self->ent[i].mMaxBend = 0.0f;
        tCrowdEventData_FreeSamples(&self->ent[i].mAnticipate);
        tCrowdEventData_FreeSamples(&self->ent[i].mFall);
        tCrowdEventData_FreeSamples(&self->ent[i].mTrick);
    }
    for (int i = 0; i < n; i++) {
        self->voices[i].mCrowdBank = cheerBank;
    }
    sSnd6D78Parser infParser;
    FileParser_FileParser((FileParser*)((FileParser*)&infParser), name, 0x100);
    infParser.cur = infParser.data;
    if (FileParser_FindSection((FileParser*)(&infParser), str_GLOBAL)) cCrowdSys_ReadSection((cCrowdSys*)self, &infParser);
    infParser.cur = infParser.data;
    if (FileParser_FindSection((FileParser*)(&infParser), key)) cCrowdSys_ReadSection((cCrowdSys*)self, &infParser);
    FileParser__FileParser((FileParser*)(&infParser), 2);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdSys_Shutdown);
#ifdef SKIP_ASM
extern "C" void cCrowdSys_RemoveAllObjects(cCrowdSys* self);
extern "C" void cCrowdInstance__cCrowdInstance(cCrowdInstance* p, int flags);

/**
 * Shuts the crowd system down: removes all crowd objects, destroys the crowd instances and frees the sample lists.
 *
 * Calls cCrowdSys_RemoveAllObjects, destroys each instance from the last one down (cCrowdInstance__cCrowdInstance(.., 0)), frees the array
 * (cMemMan_free at its -0x10 header) and clears mpCrowdInstances and mNumInstances, then clears every intensity's
 * mAttack, mDecay, mMaxBend and its ANTICIPATE, FALL and TRICK lists (tCrowdEventData_FreeSamples). cSSXAudio_InGamePurge and
 * cCrowdSys__cCrowdSys call it.
 */
extern "C" void cCrowdSys_Shutdown(cCrowdSys* self)
{
    cCrowdSys_RemoveAllObjects(self);
    cCrowdInstance* base = self->mpCrowdInstances;
    if (base != 0) {
        cCrowdInstance* curInstance = base + *(int*)((char*)base - 0x10);
        while (self->mpCrowdInstances != curInstance) {
            curInstance--;
            cCrowdInstance__cCrowdInstance(curInstance, 0);
        }
        cMemMan_free((char*)self->mpCrowdInstances - 0x10);
        self->mpCrowdInstances = 0;
        self->mNumInstances = 0;
    }
    for (int i = 0; i < 3; i++) {
        self->mCrowdData[i].mAttack = 0.0f;
        self->mCrowdData[i].mDecay = 0.0f;
        self->mCrowdData[i].mMaxBend = 0.0f;
        tCrowdEventData_FreeSamples(&self->mCrowdData[i].mAnticipate);
        tCrowdEventData_FreeSamples(&self->mCrowdData[i].mFall);
        tCrowdEventData_FreeSamples(&self->mCrowdData[i].mTrick);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdSys_ReadSection);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int FileParser_IsEOF(FileParser* self);
extern "C" bool FileParser_AssignString(FileParser* self, const char* name, char* out);
extern "C" bool FileParser_AssignFloat(FileParser* self, const char* name, float* out);
// FileParser_GetName returns the token buffer (the unit declares it void elsewhere).
extern "C" const char* FileParser_GetName_s(void* self) __asm__("FileParser_GetName");
extern const char str_TRICKpct_d[];
extern const char str_FALLpct_d[];
extern const char str_ANTICIPATEpct_d[];
extern const char str_ATTACKpct_d[];
extern const char str_DECAYpct_d[];
extern const char str_MAXBENDpct_d[];

/**
 * Reads the crowd entries of one crowd.inf section until the end of the file.
 *
 * For each name, intensities 1..3 are tried: TRICK<n>, FALL<n> and ANTICIPATE<n> load a sample file into that list
 * (tCrowdEventData_LoadSample); ATTACK<n>, DECAY<n> and MAXBEND<n> set mAttack, mDecay and mMaxBend (FileParser_AssignFloat). Names
 * compare case-insensitively; others are skipped.
 *
 * @param parser  the FileParser positioned in the section
 */
extern "C" void cCrowdSys_ReadSection(cCrowdSys* self, void* parser)
{
    cCrowdSys* crowdSys = self;
    char val[0x200];
    char key[0x200];
    while (!((FileParser*)parser)->IsEOF()) {
        const char* name = FileParser_GetName_s(parser);
        for (int i = 0; i < 3; i++) {
            sprintf(key, str_TRICKpct_d, i + 1);
            if (strcasecmp(name, key) == 0) {
                ((FileParser*)parser)->AssignString(name, val);
                tCrowdEventData_LoadSample(&crowdSys->mCrowdData[i].mTrick, val, 0);
                break;
            }
            sprintf(key, str_FALLpct_d, i + 1);
            if (strcasecmp(name, key) == 0) {
                ((FileParser*)parser)->AssignString(name, val);
                tCrowdEventData_LoadSample(&crowdSys->mCrowdData[i].mFall, val, 0);
                break;
            }
            sprintf(key, str_ANTICIPATEpct_d, i + 1);
            if (strcasecmp(name, key) == 0) {
                ((FileParser*)parser)->AssignString(name, val);
                tCrowdEventData_LoadSample(&crowdSys->mCrowdData[i].mAnticipate, val, 0);
                break;
            }
            sprintf(key, str_ATTACKpct_d, i + 1);
            if (strcasecmp(name, key) == 0) {
                ((FileParser*)parser)->AssignFloat(name, &crowdSys->mCrowdData[i].mAttack);
                break;
            }
            sprintf(key, str_DECAYpct_d, i + 1);
            if (strcasecmp(name, key) == 0) {
                ((FileParser*)parser)->AssignFloat(name, &crowdSys->mCrowdData[i].mDecay);
                break;
            }
            sprintf(key, str_MAXBENDpct_d, i + 1);
            if (strcasecmp(name, key) == 0) {
                ((FileParser*)parser)->AssignFloat(name, &crowdSys->mCrowdData[i].mMaxBend);
                break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdSys_UnpackHandle);
#ifdef SKIP_ASM
/**
 * Splits a crowd object handle into its crowd instance (low 8 bits) and object slot (the rest).
 *
 * @param packedId  the handle from cCrowdSys_AddObject
 * @param outInstance  receives the crowd instance (viewport)
 * @param outSlot  receives the object slot
 */
extern "C" void cCrowdSys_UnpackHandle(cCrowdSys* self, int packedId, int* outInstance, int* outSlot)
{
    *outInstance = packedId & 0xff;
    *outSlot = packedId >> 8;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cCrowdSys_RemoveObject);
#ifdef SKIP_ASM
extern "C" void cCrowdInstance_RemoveObject(cCrowdInstance* voice, int b, float v);

/**
 * Removes a crowd object by its handle (cCrowdInstance_RemoveObject), fading its ambient loop over @p fade.
 *
 * cWorldTriggerInstance_Stop calls it with 0.25 for a trigger with the crowd flag.
 *
 * @param id  the crowd object handle (cCrowdSys_AddObject), or negative for none
 * @param fade  the fade time
 */
extern "C" void cCrowdSys_RemoveObject(cCrowdSys* self, int id, float fade)
{
    if (id >= 0 && self->mpCrowdInstances != 0) {
        int instance;
        int slot;
        cCrowdSys_UnpackHandle(self, id, &instance, &slot);
        cCrowdInstance_RemoveObject(&self->mpCrowdInstances[instance], slot, fade);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cCrowdSys_SetObjectLoudness);
#ifdef SKIP_ASM
/**
 * Sets the loudness of a crowd object by its handle (cCrowdInstance_SetObjectLoudness).
 *
 * The world trigger update cWorldTriggerInstance_Update passes the trigger's level * 127.
 *
 * @param id  the crowd object handle (cCrowdSys_AddObject), or negative for none
 * @param value  the loudness, 0..127
 */
extern "C" void cCrowdSys_SetObjectLoudness(cCrowdSys* self, int id, int value)
{
    if (id >= 0 && self->mpCrowdInstances != 0) {
        int instance;
        int slot;
        cCrowdSys_UnpackHandle(self, id, &instance, &slot);
        cCrowdInstance_SetObjectLoudness((s2A5CD8*)&self->mpCrowdInstances[instance], slot, value);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cCrowdSys_PlayTrickCheer);
#ifdef SKIP_ASM
extern "C" void cCrowdInstance_PlayTrickCheer(cCrowdInstance* voice, int a1);


/**
 * Plays a TRICK cheer at an intensity on the crowd instance of a viewport (cCrowdInstance_PlayTrickCheer), when instances exist.
 *
 * cSSXAudio_Land uses intensities 0..2 by the landed trick, and cSSXAudio_CheerOnStreamDoneCallback and cSSXAudio_OnFinishEvent use 0.
 *
 * @param slot  the crowd intensity (0..2)
 * @param i  the crowd instance (viewport)
 */
extern "C" void cCrowdSys_PlayTrickCheer(cCrowdSys* self, int slot, int i)
{
    cCrowdInstance* crowdInstance = self->mpCrowdInstances;
    if (crowdInstance != 0) {
        cCrowdInstance_PlayTrickCheer((cCrowdInstance*)(&crowdInstance[i]), slot);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cCrowdSys_PlayFallCheer);
#ifdef SKIP_ASM
extern "C" void cCrowdInstance_PlayFallCheer(cCrowdInstance* voice, int a1);

/**
 * Plays a FALL cheer at an intensity on the crowd instance of a viewport (cCrowdInstance_PlayFallCheer), when instances exist.
 *
 * cSSXAudio_Wipeout (a wipeout) uses intensities 0..2.
 *
 * @param slot  the crowd intensity (0..2)
 * @param i  the crowd instance (viewport)
 */
extern "C" void cCrowdSys_PlayFallCheer(cCrowdSys* self, int slot, int i)
{
    cCrowdInstance* crowdInstance = self->mpCrowdInstances;
    if (crowdInstance != 0) {
        cCrowdInstance_PlayFallCheer((cCrowdInstance*)(&crowdInstance[i]), slot);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cCrowdSys_RequestAnticipation);
#ifdef SKIP_ASM
extern "C" void cCrowdInstance_RequestAnticipation(cCrowdInstance* voice);

/**
 * Asks the crowd instance of a viewport for an anticipation cheer (cCrowdInstance_RequestAnticipation), when instances exist.
 *
 * cSSXAudio_LiftWithAirTime calls it as the rider leaves the ground.
 *
 * @param i  the crowd instance (viewport)
 */
extern "C" void cCrowdSys_RequestAnticipation(cCrowdSys* self, int i)
{
    cCrowdInstance* crowdInstance = self->mpCrowdInstances;
    if (crowdInstance != 0) {
        cCrowdInstance_RequestAnticipation((cCrowdInstance*)(&crowdInstance[i]));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cCrowdSys_CancelAnticipation);
#ifdef SKIP_ASM
extern "C" void cCrowdInstance_CancelAnticipation(cCrowdInstance* voice);

/**
 * Cancels the anticipation cheer of a viewport's crowd instance (cCrowdInstance_CancelAnticipation), when instances exist.
 *
 * cSSXAudio_Land (nothing gained) and cSSXAudio_Wipeout call it.
 *
 * @param i  the crowd instance (viewport)
 */
extern "C" void cCrowdSys_CancelAnticipation(cCrowdSys* self, int i)
{
    cCrowdInstance* crowdInstance = self->mpCrowdInstances;
    if (crowdInstance != 0) {
        cCrowdInstance_CancelAnticipation((cCrowdInstance*)(&crowdInstance[i]));
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdSys_OpenObjectOpts);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);

struct sSnd7468Env {
    int a;
    int b;
    int c;
    int d;
};
struct sSnd7468Pair {
    int a;
    int b;
};

/**
 * Opens a play-option level for a crowd object: pushes a level of the bank manager's options with the defaults.
 *
 * Copies the default channel, clears the position, volume, callback and other options, sets the delay to {100, 0, 90,
 * 50}, mMapToMaxVol to 0x7F and mCloseOnEnd to 1 (the push of cBankSys's SFX_Begin, without a bank and patch). The
 * world trigger update cWorldTriggerInstance_Update fills it and adds the object with cCrowdSys_AddObject, which pops it.
 *
 * @param sys  the crowd system (its first word is the cBankManager)
 */
extern "C" void cCrowdSys_OpenObjectOpts(char** sys)
{
    cBankManager* bankMgr = *(cBankManager**)sys;
    cBankMonitor* playOpts = &bankMgr->BankMonitor;
    bankMgr->BankMonitor.mInst++;
    playOpts->mCurrent[bankMgr->BankMonitor.mInst] = playOpts->mDefault;
    bankMgr->BankMonitor.mPosition[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mPosStatic[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mBaseVol[bankMgr->BankMonitor.mInst] = playOpts->mCurrent[bankMgr->BankMonitor.mInst].vol;
    playOpts->mVolControl[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mCallback[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mPtr[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mViewport[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mVanish[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mRadius[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mDelay[bankMgr->BankMonitor.mInst].mCount = 0;
    playOpts->mDelay[bankMgr->BankMonitor.mInst].mTime = 100;
    playOpts->mDelay[bankMgr->BankMonitor.mInst].mFeedback = 90;
    playOpts->mDelay[bankMgr->BankMonitor.mInst].mLevel = 50;
    playOpts->mExternalVol[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerPan[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerSide[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mMapToMaxVol[bankMgr->BankMonitor.mInst] = 0x7F;
    playOpts->mCloseOnEnd[bankMgr->BankMonitor.mInst] = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cCrowdSys_AddObject);
#ifdef SKIP_ASM
extern "C" int cCrowdInstance_AddObject(void* voice, void* bank, int a1);

/**
 * Adds a crowd object from the current play-option level to the crowd instance of that level's viewport.
 *
 * Calls cCrowdInstance_AddObject on instance mViewport[cur] with the options and @p ambientPatch, then pops the level. Returns the handle
 * (slot << 8 | viewport) or -1 (no instances, or no free slot). The world trigger update cWorldTriggerInstance_Update keeps it.
 *
 * @param ambientPatch  the ambient sample (patch) of the object
 * @return the crowd object handle, or -1
 */
extern "C" int cCrowdSys_AddObject(cCrowdSys* self, int ambientPatch)
{
    cCrowdInstance* crowdInstance = self->mpCrowdInstances;
    if (crowdInstance == 0) {
        return -1;
    }
    cBankMonitor* bankMonitor = &self->mpVBankManager->BankMonitor;
    int packedId = cCrowdInstance_AddObject(&crowdInstance[bankMonitor->mViewport[bankMonitor->mInst]], bankMonitor, ambientPatch);
    if (packedId >= 0) {
        cBankMonitor* monitor = &self->mpVBankManager->BankMonitor;
        packedId = (packedId << 8) | monitor->mViewport[monitor->mInst];
    }
    self->mpVBankManager->BankMonitor.mInst--;
    return packedId;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdSys_RemoveAllObjects);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern const sSndKey8 D_004A3700[];
extern "C" void cCrowdInstance_RemoveAllObjects(cCrowdInstance* voice, float a1);


/**
 * Removes every crowd object of every crowd instance and destroys the bank monitor instances of the crowd callback.
 *
 * Calls cCrowdInstance_RemoveAllObjects(instance, 0) for each instance, then the bank monitor's vtable slot 3 with the callback key
 * D_004A3700 (cCrowdSys_LoopPitchCallback). cSSXAudio_BANKMONITOR_DestroyAll and cCrowdSys_Shutdown call it.
 */
// PORT: 64-bit long sound key
extern "C" void cCrowdSys_RemoveAllObjects(cCrowdSys* self)
{
    for (int i = 0; i < self->mNumInstances; i++) {
        cCrowdInstance_RemoveAllObjects(&self->mpCrowdInstances[i], 0.0f);
    }
    cBankManager* bankManager = self->mpVBankManager;
    sVEntry<void(void*, long)>* vt = (sVEntry<void(void*, long)>*)bankManager->BankMonitor.mVtbl;
    cBankMonitor* monitor = &bankManager->BankMonitor;
    vt[3].fn(vthis(monitor, vt[3]), D_004A3700->v);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdSys_LoopPitchCallback);
#ifdef SKIP_ASM
extern "C" void cCrowdInstance_UpdateLoopPitch(cCrowdInstance* self, cBankMonitorInstance* obj, float dt);

/**
 * Monitor callback of the crowd voices: forwards to cCrowdInstance_UpdateLoopPitch for the crowd instance stored in the voice's mPtr.
 *
 * Its pointer to member is D_004A3700, which cCrowdInstance_Update, cCrowdInstance_AddObject install on the loop voices.
 *
 * @param obj  the voice's monitor instance
 * @param dt  the elapsed time
 */
extern "C" void cCrowdSys_LoopPitchCallback(cCrowdSys* self, cBankMonitorInstance* obj, float dt)
{
    cCrowdInstance_UpdateLoopPitch((cCrowdInstance*)((char*)obj->mPtr), obj, dt);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdSys_GetCurrentCheer);
#ifdef SKIP_ASM

/**
 * Returns the cheer value unk_18C of a crowd instance, or 0 for an index past mNumInstances.
 *
 * The value is intensity + 1 for a TRICK cheer, ~intensity for a FALL cheer, the anticipation intensity 1 or 2, or 0.
 * cCrowdMan2d_spawnMarkers (main/ssxapp.cpp) reads it for instance 0.
 *
 * @param i  the crowd instance (viewport)
 * @return the cheer value
 */
extern "C" int cCrowdSys_GetCurrentCheer(cCrowdSys* self, int i)
{
    if (i < self->mNumInstances) {
        return (self->mpCrowdInstances)[i].mCurrentCheer;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", FileParser_FileParser);
#ifdef SKIP_ASM
extern "C" int BXFILE_size(const char* name);
extern "C" int FILE_loadat(const char* name, void* buf, int size);
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");


/**
 * Loads a whole file into memory for parsing, with a terminating 0 after it.
 *
 * Takes the size (BXFILE_size); when nonzero, allocates size + 1 bytes tagged with the file name, loads the file
 * there (FILE_loadat) and points mPtr at its start. An empty or missing file leaves mBasePtr and mPtr 0.
 *
 * @param name  the file (On Tour Filename)
 * @param flags  the allocation flags (On Tour int MemType)
 * @return @p self
 */
extern "C" FileParser* FileParser_FileParser(FileParser* self, const char* name, int flags)
{
    self->mBasePtr = 0;
    self->mPtr = 0;
    self->mFileSize = BXFILE_size(name);
    if (self->mFileSize != 0) {
        self->mBasePtr = (char*)operator_new_tag(self->mFileSize + 1, name, flags, 0);
        FILE_loadat(name, self->mBasePtr, self->mFileSize + 1);
        self->mPtr = self->mBasePtr;
        self->mBasePtr[self->mFileSize] = 0;
    }
    return (self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", FileParser__FileParser);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

/**
 * Destroys a file parser: frees the loaded file (mBasePtr), then the object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void FileParser__FileParser(FileParser* self, int flags)
{
    void* basePtr = self->mBasePtr;
    if (basePtr != 0) {
        cMemMan_free(basePtr);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", FileParser_GetChar__FPv);
#ifdef SKIP_ASM
/**
 * Returns the character at mPtr and advances mPtr (On Tour GetChar).
 *
 * @return the character read
 */
signed char FileParser::GetChar()
{
    return *mPtr++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_SkipSpace);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char FileParser_GetChar(void* self);
extern "C" bool FileParser_SkipComment(FileParser* self);

// FileParser_SkipSpace returns int (its body leaves a result in v0); the unit declares it void.
int FileParser_SkipSpace_r(void*) __asm__("FileParser_SkipSpace");

/**
 * Skips whitespace and comments (On Tour SkipSpace).
 *
 * Stops at the first other character; '#' and ';' comments are skipped to the end of the line (SkipComment).
 *
 * @return 1 when the data ends or the next character is '[' (a section header), else 0
 */
int FileParser::SkipSpace()
{
    while (*(signed char*)mPtr != 0) {
        if (SkipComment() == true) return 1;
        signed char curChar = *(signed char*)mPtr;
        if (curChar != ' ' && curChar != '\t' && curChar != '\n' && curChar != '\r') {
            int atSection = 0;
            if (curChar == '[') atSection = 1;
            return atSection;
        }
        GetChar();
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", FileParser_SkipComment);
#ifdef SKIP_ASM
extern "C" bool FileParser_FindEOL(FileParser* self);

/**
 * Skips a '#' or ';' comment up to (not past) the end of its line (FindEOL).
 *
 * @return true at the end of the data, else false
 */
bool FileParser::SkipComment()
{
    signed char curChar = *(signed char*)mPtr;
    if (curChar != 0) {
        if (curChar == '#' || curChar == ';') {
            int atEnd = FindEOL();
            if (atEnd != 0) {
                return true;
            }
        }
        return false;
    }
    return true;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_SkipSectionNames);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// FileParser_SkipSpace returns int (its body leaves a result in v0); the unit declares it void.
int FileParser_SkipSpace_r(void*) __asm__("FileParser_SkipSpace");
extern "C" char* FileParser_FindNextSection(FileParser*);
extern "C" int FileParser_IsEOF(FileParser*);

/**
 * Skips any section headers ("[...]") at the parse position along with the whitespace around them, then calls IsEOF.
 *
 * Declared void, but On Tour's SkipSectionNames returns int: the IsEOF result is left in $v0.
 */
void FileParser::SkipSectionNames()
{
    this->SkipSpace();
    while (*(signed char*)mPtr == '[') {
        FindNextSection();
        this->SkipSpace();
    }
    IsEOF();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_FindEOL);
#ifdef SKIP_ASM
/**
 * Advances mPtr to the next '\r' or '\n' without consuming it.
 *
 * @return false at the end of a line, true at the end of the data
 */
bool FileParser::FindEOL()
{
    while (*(signed char*)mPtr != 0) {
        signed char curChar = *(signed char*)mPtr;
        if (curChar == '\r') {
            return false;
        }
        if (curChar == '\n') {
            return false;
        }
        this->GetChar();
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", FileParser_FindNextSection);
#ifdef SKIP_ASM
signed char FileParser_GetChar(void* self);
extern "C" bool FileParser_SkipComment(FileParser* self);
extern "C" int FileParser_ExpectToken(FileParser* self, const char* s);
extern const char str_rbracket[];

/**
 * Advances to the next section header "[name]" and returns its name (On Tour FindNextSection).
 *
 * Skips comments, reads characters until a '[', copies the name into mTempStr up to ']' and consumes the ']'
 * (ExpectToken). The copy has no length limit for the 2048-byte mTempStr and does not stop at the end of the data, so
 * a '[' without ']' runs past the buffer.
 *
 * @return mTempStr holding the name, or 0 when no further section exists
 */
char* FileParser::FindNextSection()
{
    while (*(signed char*)mPtr != 0) {
        SkipComment();
        if (this->GetChar() == '[') {
            int n = 0;
            if (*(signed char*)mPtr != ']') {
                do {
                    char* dst = (char*)&mTempStr;
                    dst[n] = this->GetChar();
                    n++;
                } while (*(signed char*)mPtr != ']');
            }
            if (ExpectToken(str_rbracket) == 0) {
                char* buf = (char*)&mTempStr;
                buf[n] = 0;
                return buf;
            }
            break;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", FileParser_FindSection);
#ifdef SKIP_ASM
extern "C" char* FileParser_FindNextSection(FileParser*);
extern "C" int strcasecmp(const char* a, const char* b);

/**
 * Advances to the section whose name equals @p name, ignoring case (strcasecmp).
 *
 * @param name  the section name (On Tour Section)
 * @return true when found, false at the end of the data
 */
bool FileParser::FindSection(const char* name)
{
    char* curName;
    while (*(signed char*)mPtr != 0 && (curName = FindNextSection()) != 0) {
        if (strcasecmp(curName, name) == 0) {
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", FileParser_FindPartialSection);
#ifdef SKIP_ASM
extern "C" char* FileParser_FindNextSection(FileParser*);
extern "C" char* strstr(const char* s, const char* sub);

/**
 * Advances to the first section whose name and @p name match as prefixes either way (one starts with the other, via
 * strstr).
 *
 * @param name  the section name to match (On Tour Section)
 * @return true when found, false at the end of the data
 */
bool FileParser::FindPartialSection(char* name)
{
    char* tok;
    while (*mPtr != 0 && (tok = FindNextSection()) != 0) {
        if (strstr(name, tok) == name || strstr(tok, name) == tok) {
            return true;
        }
    }
    return false;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_GetString);
#ifdef SKIP_ASM
signed char FileParser_GetChar(void* self);
extern "C" void FileParser_SkipSpace(void*);
extern "C" int FileParser_ExpectToken(FileParser* self, const char* s);
extern const char str_dquote[];

/**
 * Reads a quoted string: skips whitespace, consumes the opening '"', copies characters into mTempStr up to the
 * closing '"' and consumes it. Neither ExpectToken result is checked, and the copy has no length or end-of-data
 * limit.
 *
 * @return mTempStr holding the string
 */
char* FileParser::GetString()
{
    int n = 0;
    FileParser_SkipSpace(this);
    ExpectToken(str_dquote);
    if (*(signed char*)mPtr != '"') {
        char* dst = (char*)&mTempStr;
        do {
            dst[n] = this->GetChar();
            n++;
        } while (*(signed char*)mPtr != '"');
    }
    ExpectToken(str_dquote);
    char* buf = (char*)&mTempStr;
    buf[n] = 0;
    return buf;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", FileParser_GetName);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" char* FileParser_GetName_t(void* self) __asm__("FileParser_GetName");
/**
 * Reads a bare token: skips whitespace and comments, then copies characters into mTempStr up to whitespace or the end
 * of the data.
 *
 * @return mTempStr holding the token
 */
char* FileParser::GetName()
{
    int n = 0;
    SkipSpace();
    while (*(signed char*)mPtr != 0 && *(signed char*)mPtr != ' ' &&
           *(signed char*)mPtr != '\t' && *(signed char*)mPtr != '\n' &&
           *(signed char*)mPtr != '\r') {
        char* dst = mTempStr;
        dst[n] = GetChar();
        n++;
    }
    char* buf = mTempStr;
    buf[n] = 0;
    return buf;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_GetFloat);
#ifdef SKIP_ASM
extern "C" void FileParser_SkipSpace(void*);
extern "C" void FileParser_GetName(void*);
extern "C" int sscanf(const char* s, const char* fmt, ...);
extern char str_pct_f[];

/**
 * Reads a token (GetName) and converts it with sscanf("%f"). The value is left uninitialised when the conversion
 * fails.
 *
 * @return the number read
 */
float FileParser::GetFloat()
{
    float value;
    FileParser_SkipSpace(this);
    FileParser_GetName(this);
    sscanf((char*)&mTempStr, str_pct_f, &value);
    return value;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_GetInt);
#ifdef SKIP_ASM
extern "C" void FileParser_SkipSpace(void*);
extern "C" void FileParser_GetName(void*);
extern "C" int sscanf(const char* s, const char* fmt, ...);
extern char str_pct_d_h[];

/**
 * Reads a token (GetName) and converts it with sscanf("%d"). The value is left uninitialised when the conversion
 * fails.
 *
 * @return the number read
 */
int FileParser::GetInt()
{
    int value;
    FileParser_SkipSpace(this);
    FileParser_GetName(this);
    sscanf((char*)&mTempStr, str_pct_d_h, &value);
    return value;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_ExpectToken);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char FileParser_GetChar(void* self);
int FileParser_SkipSpace_r(void*) __asm__("FileParser_SkipSpace");

/**
 * Skips whitespace and comments, then consumes characters while they match @p name.
 *
 * @param name  the expected token (On Tour Token)
 * @return 0 when the whole token matched; 1 on a mismatch; the next expected character when the data ended first
 */
int FileParser::ExpectToken(const char* name)
{
    int i = 0;
    this->SkipSpace();
    while (*mPtr != 0 && name[i] != 0) {
        if (this->GetChar() != name[i]) {
            return 1;
        }
        i++;
    }
    return name[i];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_IsEOF);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int FileParser_SkipSpace_r(void*) __asm__("FileParser_SkipSpace");

/**
 * Tells whether the current section is finished: true at the end of the file (mPtr - mBasePtr >= mFileSize) or when,
 * after whitespace and comments, the data ends or a new section ('[') starts.
 *
 * @return 1 when finished, else 0
 */
int FileParser::IsEOF()
{
    if (mPtr - mBasePtr >= mFileSize) {
        return 1;
    }
    this->SkipSpace();
    signed char nextChar = *(signed char*)mPtr;
    int finished = 0;
    if (nextChar == 0 || nextChar == '[') {
        finished = 1;
    }
    return finished;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_AssignString);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int FileParser_ExpectToken(FileParser* self, const char* s);
extern "C" char* FileParser_GetString(FileParser* self);
extern "C" char* strcpy(char* dst, const char* src);
extern char str_eq[];

/**
 * Reads "= \"value\"" into @p out when the key last read (mTempStr) equals @p name, ignoring case.
 *
 * @param name  the key to accept (On Tour ID)
 * @param out  receives the string (On Tour Dest); unbounded strcpy
 * @return true when assigned, false when the key differs or no '=' follows
 */
bool FileParser::AssignString(const char* name, char* out)
{
    if (strcasecmp((char*)&mTempStr, name) != 0) {
        return false;
    }
    if (ExpectToken(str_eq) != 0) {
        return false;
    }
    char* str = GetString();
    if (str == 0) {
        return false;
    }
    strcpy(out, str);
    return true;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_AssignFloat);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int FileParser_ExpectToken(FileParser* self, const char* s);
extern "C" float FileParser_GetFloat(FileParser* self);
extern char str_eq[];

/**
 * Reads "= value" as a float into @p out when the key last read (mTempStr) equals @p name, ignoring case.
 *
 * @param name  the key to accept (On Tour ID)
 * @param out  receives the value (On Tour Dest)
 * @return true when assigned, false when the key differs or no '=' follows
 */
bool FileParser::AssignFloat(const char* name, float* out)
{
    if (strcasecmp((char*)&mTempStr, name) != 0) {
        return false;
    }
    if (ExpectToken(str_eq) != 0) {
        return false;
    }
    *out = GetFloat();
    return true;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_AssignInt);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int FileParser_ExpectToken(FileParser* self, const char* s);
extern "C" int FileParser_GetInt(FileParser* self);
extern char str_eq[];

/**
 * Reads "= value" as an int into @p out when the key last read (mTempStr) equals @p name, ignoring case.
 *
 * @param name  the key to accept (On Tour ID)
 * @param out  receives the value (On Tour Dest)
 * @return true when assigned, false when the key differs or no '=' follows
 */
bool FileParser::AssignInt(const char* name, int* out)
{
    if (strcasecmp((char*)&mTempStr, name) != 0) {
        return false;
    }
    if (ExpectToken(str_eq) != 0) {
        return false;
    }
    *out = GetInt();
    return true;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cMidiSys_cMidiSys);
#ifdef SKIP_ASM
extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" char* cBankSys_cBankSys(cBankSys* self, int embedded, int count, int heap);
extern "C" void SNDmidiinit();
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

struct sSndCtVtEnt {
    short delta;
    short index;
    void* fn;
};
struct sSndCtVt4 {
    sSndCtVtEnt e[4];
} __attribute__((aligned(8)));
struct sSndCtVt7 {
    sSndCtVtEnt e[7];
} __attribute__((aligned(8)));

extern char cBankSys_vtbl[];
extern char cBankMonitor_vtbl[];
extern const char str_MIDIBANKS[];
extern const sSndCtVt4 D_004839F0_ct __asm__("D_004839F0");
extern const sSndCtVt7 D_00483A10_ct __asm__("D_00483A10");
// A complete cMidiSys (the in-charge ctor builds it): g++ 2.95 puts the virtual bases after the non-virtual part.
struct cMidiSysComplete {
    cMidiSys mMidiSys;           // 0x00
    cSoundSys mSoundSys;         // 0x14
    cBankManager mBankManager;   // 0x1C
};

/**
 * Constructs a cMidiSys, the MIDI part of the audio system.
 *
 * Builds cBankSys first (and, with @p embedded, the shared virtual bases), fixes up the base vtables, then
 * allocates the 0x20-byte "MIDIBANKS" table and records the instance in D_004A3744.
 *
 * @param embedded  g++ in-charge flag: nonzero when the virtual bases are built here (the complete object)
 * @param count  maximum number of banks
 * @param heap  audio mode (eSOUNDDEF_MODE), passed to the base constructors
 * @return @p self
 */
// PORT: hand-expanded g++ 2.95 virtual-base ctor (vtable upcast-offset fixup on stack copies).
extern "C" char* cMidiSys_cMidiSys(cMidiSys* self, int embedded, int count, int heap)
{
    if (embedded) {
        cMidiSysComplete* complete = (cMidiSysComplete*)self;
        cSoundSys* soundSys = &complete->mSoundSys;
        self->mpVBankManager = &complete->mBankManager;
        complete->mBankManager.BankMonitor.mpVSoundSys = soundSys;
        cSoundSys_cSoundSys(soundSys, heap);
        cBankManager_cBankManager(self->mpVBankManager, 0, count, heap);
    }
    cBankSys_cBankSys((cBankSys*)self, 0, count, heap);
    self->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&D_004839F0_ct;
    self->mpVBankManager->BankMonitor.mVtbl = (void**)cBankMonitor_vtbl;
    self->mpVBankManager->mVtbl = (void**)&D_00483A10_ct;
    if (!embedded) {
        sSndCtVt4 vtbl1 = D_004839F0_ct;
        self->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&vtbl1;
        char* base1 = (char*)self->mpVBankManager->BankMonitor.mpVSoundSys - 0x14;
        int adj1 = ((char*)self) - base1;
        vtbl1.e[1].delta = D_004839F0_ct.e[1].delta + adj1;
        sSndCtVt7 vtbl2 = D_00483A10_ct;
        self->mpVBankManager->mVtbl = (void**)&vtbl2;
        char* base2 = (char*)self->mpVBankManager - 0x1C;
        int adj2 = ((char*)self) - base2;
        vtbl2.e[1].delta = D_00483A10_ct.e[1].delta + adj2;
    }
    self->mVtbl = (void**)cBankSys_vtbl;
    SNDmidiinit();
    self->mMidiBanks = (tMidiBank*)(new (str_MIDIBANKS, 0, 0) char[0x20]) ;
    D_004A3744 = (cMidiSys*)((int)self);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cMidiSys__cMidiSys);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);
extern "C" void cBankManager__cBankManager(void* self, int flags);
extern "C" void cSoundSys__cSoundSys(void* self, int flags);
extern "C" void cBankSys__cBankSys(cBankSys* self, int flags);
extern "C" void SNDmidirestore();

struct sSndDtBVtEnt {
    short delta;
    short index;
    void* fn;
};
struct sSndDtBVt4 {
    sSndDtBVtEnt e[4];
} __attribute__((aligned(8)));
struct sSndDtBVt7 {
    sSndDtBVtEnt e[7];
} __attribute__((aligned(8)));

extern char cBankSys_vtbl[];
extern const sSndDtBVt4 D_004839F0;
extern const sSndDtBVt7 D_00483A10;
extern char cBankMonitor_vtbl[];

/**
 * Destroys the MIDI sound system.
 *
 * Re-installs the cMidiSys vtables (cBankSys_vtbl at mVtbl, D_004839F0 / D_00483A10 in the virtual bases, adjusted on the
 * stack when not in charge), frees mMidiBanks, clears D_004A3744 (the instance the ctor stored), calls SNDmidirestore,
 * runs the cBankSys base destructor, destroys the virtual bases when bit 1 of @p flags is set and frees the object when
 * bit 0 is set. The matching constructor is cMidiSys_cMidiSys.
 *
 * @param flags  destructor flags: bit 1 destroys the virtual bases, bit 0 frees the object
 */
// PORT: hand-expanded g++ 2.95 virtual-base dtor (vtable upcast-offset fixup on stack copies).
extern "C" void cMidiSys__cMidiSys(cMidiSys* self, int flags)
{
    self->mVtbl = (void**)cBankSys_vtbl;
    self->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&D_004839F0;
    self->mpVBankManager->BankMonitor.mVtbl = (void**)cBankMonitor_vtbl;
    self->mpVBankManager->mVtbl = (void**)&D_00483A10;
    if (flags == 0) {
        sSndDtBVt4 soundSysVtbl = D_004839F0;
        self->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&soundSysVtbl;
        char* soundSysBase = (char*)self->mpVBankManager->BankMonitor.mpVSoundSys - 0x14;
        int adj1 = ((char*)self) - soundSysBase;
        soundSysVtbl.e[1].delta = D_004839F0.e[1].delta + adj1;
        sSndDtBVt7 bankMgrVtbl = D_00483A10;
        self->mpVBankManager->mVtbl = (void**)&bankMgrVtbl;
        char* bankMgrBase = (char*)self->mpVBankManager - 0x1C;
        int adj2 = ((char*)self) - bankMgrBase;
        bankMgrVtbl.e[1].delta = D_00483A10.e[1].delta + adj2;
    }
    if (self->mMidiBanks) cMemMan_free(self->mMidiBanks);
    D_004A3744 = 0;
    SNDmidirestore();
    cBankSys__cBankSys((cBankSys*)self, 0);
    if (flags & 2) {
        cBankManager__cBankManager(self->mpVBankManager, 0);
        cSoundSys__cSoundSys(self->mpVBankManager->BankMonitor.mpVSoundSys, 0);
    }
    if (flags & 1) operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cMidiSys_OpenMidiOpts);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);

struct sSnd8450E8 {
    int mBank;
    int mpData;
};
struct sSnd8450Sys {
    cBankManager* mpVBankManager;
    char pad4[0xC];
    sSnd8450E8* mMidiBanks;         // 0x10
};

/**
 * Opens a play-option level for a MIDI sequence: pushes a level with the defaults and records the bank and sequence.
 *
 * Same push as cCrowdSys_OpenObjectOpts (default channel, delay {100, 0, 90, 50}, mMapToMaxVol 0x7F, mCloseOnEnd 1); then
 * mMidiBanks[level] gets (@p bankIndex, @p midiData). cMidiSys_PlayMidi plays it. The crowd cheers and loops and cMidiSys_EchoCallback use it.
 *
 * @param sys  the MIDI system (cMidiSys; the crowd system or D_004A3744)
 * @param bankIndex  the bank (cBankManager bank index)
 * @param midiData  the MIDI sequence data in that bank
 */
extern "C" void cMidiSys_OpenMidiOpts(char** sys, int bankIndex, int midiData)
{
    sSnd8450Sys* self = (sSnd8450Sys*)sys;
    cBankManager* bankMgr = self->mpVBankManager;
    cBankMonitor* playOpts = &bankMgr->BankMonitor;
    bankMgr->BankMonitor.mInst++;
    playOpts->mCurrent[bankMgr->BankMonitor.mInst] = bankMgr->BankMonitor.mDefault;
    bankMgr->BankMonitor.mPosition[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mPosStatic[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mBaseVol[bankMgr->BankMonitor.mInst] = playOpts->mCurrent[bankMgr->BankMonitor.mInst].vol;
    playOpts->mVolControl[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mCallback[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mPtr[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mViewport[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mVanish[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mRadius[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mDelay[bankMgr->BankMonitor.mInst].mCount = 0;
    playOpts->mDelay[bankMgr->BankMonitor.mInst].mTime = 100;
    playOpts->mDelay[bankMgr->BankMonitor.mInst].mFeedback = 90;
    playOpts->mDelay[bankMgr->BankMonitor.mInst].mLevel = 50;
    playOpts->mExternalVol[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerPan[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerSide[bankMgr->BankMonitor.mInst] = 0;
    playOpts->mMapToMaxVol[bankMgr->BankMonitor.mInst] = 0x7F;
    playOpts->mCloseOnEnd[bankMgr->BankMonitor.mInst] = 1;
    self->mMidiBanks[self->mpVBankManager->BankMonitor.mInst].mBank = bankIndex;
    self->mMidiBanks[self->mpVBankManager->BankMonitor.mInst].mpData = midiData;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cMidiSys_PlayMidi);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" int SNDMIDI_play(void* ch, int id, int bank, int a3);
extern "C" void* cBankMonitor_BANKMONITOR_Create(void* sub, int h, int vol, int mode);
void cBankMonitorInstance_SetMIDI(void* self, int a1, int a2);
extern "C" void cMonitor_MONITOR_Create(void*, int, long, void*, int, int, int, int);

// PORT: 64-bit long sound key (view of D_004A3748, declared later in the unit).
extern const sSndKey8 D_004A3748_86B8[] __asm__("D_004A3748");

struct sSnd86B8Pair {
    int mBank;
    int mpData;
};
struct sSnd86B8Sys {
    cBankManager* mpVBankManager;
    int* mBank;                 // 0x4
    int* mPatch;                 // 0x8
    char padC[4];
    sSnd86B8Pair* mMidiBanks;       // 0x10
};

#define SND_86B8_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_86B8_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SND_86B8_FMAX(a, b) ((a) >= (b) ? (a) : (b))

static inline int snd86B8Clamp(int v)
{
    if (v > 0x7F) v = 0x7F;
    if (v < 0) v = 0;
    return v;
}
// On Tour: cBankInstance::GetStatus() / Handle(), cBankManager::BANK_Used(int) / BANK_Handle(int).
static inline int snd86B8Status(cBankInstance* e)
{
    return e->mBankStatus;
}
static inline int snd86B8InstHandle(cBankInstance* e)
{
    return e->mBankHandle;
}
static inline bool snd86B8Used(cBankManager* m, int id)
{
    return snd86B8Status(&m->mBank[id]) == 1;
}
static inline int snd86B8Handle(cBankManager* m, int id)
{
    if (snd86B8Used(m, id)) return snd86B8InstHandle(&m->mBank[id]);
    return -1;
}

/**
 * Plays the MIDI sequence of the current play-option level, registers its voice and its echoes, and pops the level.
 *
 * A positioned sound starts at volume 0 (the monitor sets it); otherwise the base volume is scaled by the vol control
 * (clamped 0..127). When the level has no 3D radius (mRadius 0) and the bank is loaded, SNDMIDI_play starts the
 * sequence now (in a sound-driver critical section). The voice goes to cBankMonitor_BANKMONITOR_Create(.., type 2) and
 * cBankMonitorInstance_SetMIDI records the sequence and bank on it. Each delay echo (count, step, decay % and first-echo %) adds a
 * monitor (cMonitor_MONITOR_Create, callback D_004A3748 = cMidiSys_EchoCallback) at the accumulated delay and decayed volume, until the
 * volume reaches 0. Pops the level (cur--).
 *
 * @param vself  the MIDI system (cMidiSys)
 * @return the voice handle, or -1
 */
extern "C" int cMidiSys_PlayMidi(char** vself)
{
    sSnd86B8Sys* self = (sSnd86B8Sys*)vself;
    cBankManager* bankMgr = self->mpVBankManager;
    cBankMonitor* playOpts = &bankMgr->BankMonitor;
    int BaseVol = playOpts->mBaseVol[playOpts->mInst];
    if (bankMgr->BankMonitor.mPosition[bankMgr->BankMonitor.mInst] != 0) {
        playOpts->mBaseVol[bankMgr->BankMonitor.mInst] = 0;
        playOpts->mCurrent[bankMgr->BankMonitor.mInst].vol = SND_86B8_MAX(0,
                SND_86B8_MIN(playOpts->mBaseVol[playOpts->mInst], 0x7F));
    } else if (playOpts->mVolControl[playOpts->mInst] != 0) {
        float gain = *(float*)playOpts->mVolControl[playOpts->mInst];
        playOpts->mBaseVol[bankMgr->BankMonitor.mInst] =
                (int)SND_86B8_FMAX(SND_86B8_MIN((float)playOpts->mBaseVol[playOpts->mInst] * gain, 127.0f), 0.0f);
        playOpts->mCurrent[playOpts->mInst].vol = SND_86B8_MAX(0, SND_86B8_MIN(playOpts->mBaseVol[playOpts->mInst],
                0x7F));
    }
    int Inst = self->mpVBankManager->BankMonitor.mInst;
    int Handle = -1;
    int bank = snd86B8Handle(self->mpVBankManager, self->mMidiBanks[Inst].mBank);
    cBankMonitor* levelOpts = &self->mpVBankManager->BankMonitor;
    if (levelOpts->mRadius[levelOpts->mInst] == 0.0f && bank >= 0) {
        int sound = self->mMidiBanks[Inst].mpData;
        SNDSYS_entercritical();
        Handle = SNDMIDI_play(&self->mpVBankManager->BankMonitor.mCurrent[self->mpVBankManager->BankMonitor.mInst],
                sound, bank, -1);
        SNDSYS_leavecritical();
    }
    void* Mon = cBankMonitor_BANKMONITOR_Create(&self->mpVBankManager->BankMonitor, Handle, BaseVol, 2);
    if (Mon != 0) {
        cBankMonitorInstance_SetMIDI(Mon, self->mMidiBanks[Inst].mpData, bank);
    }
    cDelayOpts* Delay = &self->mpVBankManager->BankMonitor.mDelay[self->mpVBankManager->BankMonitor.mInst];
    if (Delay->mCount > 0) {
        int DelayTime = 0;
        int Vol = BaseVol * Delay->mLevel / 100;
        while (Delay->mCount > 0) {
            Delay->mCount--;
            DelayTime += Delay->mTime;
            Vol = Vol * Delay->mFeedback / 100;
            if (Vol > 0x7F) Vol = 0x7F;
            if (Vol < 0) Vol = 0;
            if (Vol == 0) break;
            cBankMonitor* opts = &self->mpVBankManager->BankMonitor;
            cMonitor_MONITOR_Create(opts, DelayTime, D_004A3748_86B8->v, opts->mVolControl[opts->mInst],
                    self->mMidiBanks[Inst].mBank, self->mMidiBanks[Inst].mpData, Vol, 0);
        }
    }
    self->mpVBankManager->BankMonitor.mInst--;
    return Handle;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cMidiSys_EchoCallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cMidiSys_PlayMidi(char** sys);

/**
 * Monitor callback for a MIDI echo: plays the monitor's sequence again at its volume through D_004A3744.
 *
 * Opens a level (cMidiSys_OpenMidiOpts) with the bank (+0x1C) and sequence (+0x20) of the monitor instance, sets the vol
 * control (+0x18) and the volume (+0x24, clamped to 0x7F and 0) and plays it (cMidiSys_PlayMidi). Its pointer to member is
 * D_004A3748, which cMidiSys_PlayMidi installs for each echo.
 *
 * @param midiSys  the MIDI system
 * @param obj  the monitor instance
 * @return 1
 */
extern "C" int cMidiSys_EchoCallback(cMidiSys* midiSys, cMonitorInstance* obj)
{
    cMidiSys_OpenMidiOpts((char**)D_004A3744, obj->mData1, obj->mData2);
    cBankManager* ctlMgr = (cBankManager*)D_004A3744->mpVBankManager;
    ctlMgr->BankMonitor.mVolControl[ctlMgr->BankMonitor.mInst] = (float*)obj->mPtr;
    cBankManager* volMgr = (cBankManager*)D_004A3744->mpVBankManager;
    cBankMonitor* playOpts = &volMgr->BankMonitor;
    playOpts->mBaseVol[volMgr->BankMonitor.mInst] = obj->mData3;
#define SND_X (playOpts->mBaseVol[playOpts->mInst])
#define SND_MIN127 (SND_X < 0x7F ? SND_X : 0x7F)
    playOpts->mCurrent[volMgr->BankMonitor.mInst].vol = SND_MIN127 < 0 ? 0 : SND_MIN127;
    cMidiSys_PlayMidi((char**)D_004A3744);
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_cPS2SoundManager);
#ifdef SKIP_ASM
extern char cPS2SoundManager_vtbl[];

/**
 * Constructs the PS2 sound manager.
 *
 * Stores the vtable cPS2SoundManager_vtbl and resets the state: no mSoundHeap, driver mode mModeGroup 0,
 * mMainMemHighAddr -1, empty unused-node and free-block lists (mUnusedNodeHead / mFreeNodeHead -1) and load limits off
 * (mMemLoadLimited 0). The nodes are set up later by cPS2SoundManager_Init.
 *
 * @return @p self
 */
extern "C" void* cPS2SoundManager_cPS2SoundManager(cPS2SoundManager* self)
{
    *(void**)self = cPS2SoundManager_vtbl;
    self->mSoundHeap = 0;
    self->mModeGroup = 0;
    self->mMainMemHighAddr = -1;
    self->mUnusedNodeHead = -1;
    self->mMemLoadLimited = 0;
    self->mFreeNodeHead = -1;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager__cPS2SoundManager);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

/**
 * Destroys the PS2 sound manager, freeing its sound heap.
 *
 * Restores the class vtable (cPS2SoundManager_vtbl), frees mSoundHeap with cMemMan_free when set, and deletes the object when bit
 * 0 of @p flags is set.
 *
 * @param flags  g++ 2.95 in-charge flags; bit 0 deletes the object
 */
extern "C" void cPS2SoundManager__cPS2SoundManager(cPS2SoundManager* self, int flags)
{
    *(void**)self = cPS2SoundManager_vtbl;
    void* soundHeap = self->mSoundHeap;
    if (soundHeap != 0) {
        cMemMan_free(soundHeap);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_Init);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void System_VectorToReal5();
extern "C" void SNDSYSI_init(void* buf, int size, int flags);
extern "C" int SNDSYS_getLargestUnusedMem(int* out);
extern const char str_mSoundHeap[];

struct sSndVtE8CB0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};



/**
 * Initialises the PS2 sound manager: allocates its sound heap and resets the sound-memory node list.
 *
 * Allocates mSoundHeap (0x32000 bytes, tag "mSoundHeap"), calls System_VectorToReal5, applies @p mode through virtual slot 2
 * (SetMode), registers the heap with SNDSYSI_init (flags 0x80303), then links the 32 sound-memory nodes into a free
 * list (each with no address range and size 0; free head 31, +0x318 = -1) and records the main-memory high address from
 * SNDSYS_getLargestUnusedMem in mMainMemHighAddr.
 *
 * @param mode  the sound mode (On Tour eSOUNDDEF_MODE mode)
 */
void cPS2SoundManager::Init(int mode)
{
    mSoundHeap = operator_new_tag(0x32000, str_mSoundHeap, 0, 0);
    System_VectorToReal5();
    sSndVtE8CB0* vt = ((sSndVtE8CB0*)mVtbl);
    vt[2].fn(vthis(this, vt[2]), mode);
    SNDSYSI_init(mSoundHeap, 0x32000, 0x80303);
    mUnusedNodeHead = 31;
    mFreeNodeHead = -1;
    for (int i = 31; i >= 0; i--) {
        mNodes[i].index = i;
        mNodes[i].lowAddr = -1;
        mNodes[i].highAddr = -1;
        mNodes[i].size = 0;
        mNodes[i].state = 0;
        mNodes[i].next = i - 1;
    }
    mMemLoadLimited = 0;
    int blockAddr = 0;
    int blockSize = SNDSYS_getLargestUnusedMem(&blockAddr);
    mMainMemHighAddr = blockAddr + blockSize;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_SetMode);
#ifdef SKIP_ASM
extern "C" void SNDSYS_getopts(void* cfg);
extern "C" void SNDSYS_setopts(void* cfg);
extern "C" void DTSENC_install();


/**
 * Switches the sound driver configuration for an audio mode.
 *
 * Fills a driver configuration (SNDSYS_getopts defaults, then rates 36000/24000 and fixed levels) with per-mode voice
 * and buffer counts: mode 3 (calls DTSENC_install, records mModeGroup = 2), mode 2 and modes 0/1 (record mModeGroup =
 * 1), and applies it with SNDSYS_setopts; any other mode applies the defaults. Likely bug: the early return tests the
 * other group's value (mode 3 returns 1 when mModeGroup is 1, modes 0-2 when it is 2), so re-applying the current
 * setting goes through and switching between the two groups never does.
 *
 * @param mode  the audio mode (On Tour eSOUNDDEF_MODE mode)
 * @return 1 when the call was refused, 2 when the configuration was applied (On Tour eSOUNDMANAGER_SETMODE)
 */
int cPS2SoundManager::SetMode(int mode)
{
    SNDSYSOPTS cfg;
    SNDSYS_getopts(&cfg);
    cfg.set.outputrate = 36000;
    cfg.set.outputrateiopcpu = 24000;
    cfg.set.loadiopmodules = 0;
    cfg.set.resamplequality = 50;
    cfg.set.iopcpuresamplequality = 50;
    cfg.set.maxbanks = 20;
    switch (mode) {
    case 3:
        if (mModeGroup == 1) return 1;
        cfg.set.voicesmaincpu = 0x10;
        cfg.set.voicesiopcpu = 8;
        cfg.set.voicesspu = 0x18;
        cfg.set.outputchannels = 4;
        cfg.set.numrendermodes = 3;
        cfg.set.rendermode[0] = 40;
        cfg.set.rendermode[1] = 288;
        cfg.set.rendermode[2] = 36;
        cfg.set.outputspdifmode = 2;
        DTSENC_install();
        mModeGroup = 2;
        break;
    case 2:
        if (mModeGroup == 2) return 1;
        cfg.set.voicesmaincpu = 0x10;
        cfg.set.voicesiopcpu = 8;
        cfg.set.voicesspu = 0x30;
        cfg.set.outputspdifmode = 1;
        cfg.set.outputchannels = 2;
        cfg.set.numrendermodes = 6;
        cfg.set.rendermode[0] = 72;
        cfg.set.rendermode[1] = 320;
        cfg.set.rendermode[2] = 68;
        cfg.set.rendermode[3] = 40;
        cfg.set.rendermode[4] = 288;
        cfg.set.rendermode[5] = 36;
        mModeGroup = 1;
        break;
    case 0:
    case 1:
        if (mModeGroup == 2) return 1;
        cfg.set.voicesmaincpu = 0x10;
        cfg.set.voicesiopcpu = 8;
        cfg.set.voicesspu = 0x30;
        cfg.set.outputspdifmode = 1;
        cfg.set.outputchannels = 2;
        cfg.set.numrendermodes = 3;
        cfg.set.rendermode[0] = 40;
        cfg.set.rendermode[1] = 288;
        cfg.set.rendermode[2] = 36;
        mModeGroup = 1;
        break;
    }
    SNDSYS_setopts(&cfg);
    return 2;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_Shutdown);
#ifdef SKIP_ASM
extern "C" void SNDSYS_setMemLimits(int a, int b);
extern "C" void SNDSYS_restore();
void cMemMan_free(void*);

/**
 * Shuts the PS2 sound manager down and releases its sound heap.
 *
 * Clears the sound-memory load limits (SNDSYS_setMemLimits(-1, -1)), restores the sound library (SNDSYS_restore), frees
 * mSoundHeap if set and zeroes it.
 */
void cPS2SoundManager::Shutdown()
{
    void* soundHeap;
    SNDSYS_setMemLimits(-1, -1);
    SNDSYS_restore();
    soundHeap = mSoundHeap;
    if (soundHeap != 0) {
        cMemMan_free(soundHeap);
    }
    mSoundHeap = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSoundManager_Construct);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cPS2SoundManager_cPS2SoundManager(cPS2SoundManager* self);
extern const char str_PS2SoundManager[];

/**
 * Creates the platform sound manager: allocates 800 bytes ("PS2SoundManager") and runs
 * cPS2SoundManager_cPS2SoundManager.
 *
 * cSoundSys_cSoundSys calls it.
 *
 * @return the new cPS2SoundManager
 */
extern "C" void* cSoundManager_Construct()
{
    return cPS2SoundManager_cPS2SoundManager((cPS2SoundManager*)cMemMan_alloc(800, str_PS2SoundManager, 0, 0));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_AllocSoundMem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void SNDSYS_setMemLimits(int a, int b);
extern "C" int SNDSYS_getLargestUnusedMem(int* out);
// The unit defines cPS2SoundManager_TakeUnusedNode with its own entry/pool view types; bind this unit's struct view.
tPS2SoundMemNode* cPS2SoundManager_TakeUnusedNode_v(cPS2SoundManager* self) __asm__("cPS2SoundManager_TakeUnusedNode");

/**
 * Allocates a block of sound memory and returns its handle.
 *
 * First walks the free-block list (mFreeNodeHead, sorted by address): a block of exactly @p size is marked used and
 * unlinked, a bigger one is split (a new node from cPS2SoundManager_TakeUnusedNode takes its top @p size bytes). Otherwise carves the
 * block off the top of main sound memory: fails with -1 when SNDSYS_getLargestUnusedMem reports less free memory than @p size, with
 * -2 while load limits are on (mMemLoadLimited), else lowers mMainMemHighAddr by @p size and tells the driver
 * (SNDSYS_setMemLimits). No check that cPS2SoundManager_TakeUnusedNode found a node. Likely bug: an exact fit at the head of the free list is
 * never unlinked (mFreeNodeHead keeps pointing at the now-used node, whose next is cleared), so the rest of the free
 * list is lost and the next request of that size gets the same block again.
 *
 * @param size  bytes wanted
 * @return the node index (the memory handle), -1 when out of memory, -2 while load limits are set
 */
int cPS2SoundManager::AllocSoundMem(int size)
{
    int i = mFreeNodeHead;
    if (i >= 0) {
        tPS2SoundMemNode* prev = 0;
        tPS2SoundMemNode* ps2SoundMemNode = &((&mNodes[0]))[i];
        for (;;) {
            if (ps2SoundMemNode->size == size) {
                ps2SoundMemNode->state = 2;
                if (prev) {
                    prev->next = ps2SoundMemNode->next;
                }
                ps2SoundMemNode->next = -1;
                return ps2SoundMemNode->index;
            }
            if (size < ps2SoundMemNode->size) {
                tPS2SoundMemNode* n = cPS2SoundManager_TakeUnusedNode_v((this));
                int top = ps2SoundMemNode->highAddr;
                n->state = 2;
                n->next = -1;
                n->highAddr = top;
                n->lowAddr = top - size;
                n->size = size;
                ps2SoundMemNode->highAddr = top - size;
                ps2SoundMemNode->size -= size;
                return n->index;
            }
            if (ps2SoundMemNode->next < 0) break;
            prev = ps2SoundMemNode;
            ps2SoundMemNode = &((&mNodes[0]))[ps2SoundMemNode->next];
        }
    }
    int avail = 0;
    if (SNDSYS_getLargestUnusedMem(&avail) < size) {
        return -1;
    }
    if (mMemLoadLimited != 0) {
        return -2;
    }
    tPS2SoundMemNode* n = cPS2SoundManager_TakeUnusedNode_v((this));
    int top = mMainMemHighAddr;
    n->state = 2;
    n->lowAddr = top - size;
    n->next = -1;
    n->highAddr = top;
    n->size = size;
    SNDSYS_setMemLimits(-1, mMainMemHighAddr -= size);
    return n->index;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_DeallocSoundMem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cPS2SoundManager_FreeTopBlock_v(cPS2SoundManager* self, tPS2SoundMemNode* e) __asm__("cPS2SoundManager_FreeTopBlock");
/**
 * Frees a block of sound memory allocated by cPS2SoundManager_AllocSoundMem (cPS2SoundManager_AllocSoundMem).
 *
 * Marks node @p memHandle free. A block at mMainMemHighAddr goes back to main memory (cPS2SoundManager_FreeTopBlock), followed by every
 * free block that now sits at the new top; any other block is inserted into the free-block list (mFreeNodeHead) in
 * address order.
 *
 * @param memHandle  the memory handle (node index) AllocSoundMem returned (On Tour memHandle)
 */
void cPS2SoundManager::DeallocSoundMem(int memHandle)
{
    tPS2SoundMemNode* n = &((&mNodes[0]))[memHandle];
    n->state = 1;
    if (n->lowAddr == mMainMemHighAddr) {
        cPS2SoundManager_FreeTopBlock_v((this), n);
        while (mFreeNodeHead >= 0) {
            tPS2SoundMemNode* ps2SoundMemNode = &((&mNodes[0]))[mFreeNodeHead];
            if (ps2SoundMemNode->lowAddr != mMainMemHighAddr) return;
            mFreeNodeHead = ps2SoundMemNode->next;
            cPS2SoundManager_FreeTopBlock_v((this), ps2SoundMemNode);
        }
    } else if (mFreeNodeHead < 0) {
        n->next = mFreeNodeHead;
        mFreeNodeHead = n->index;
    } else {
        tPS2SoundMemNode* prev = 0;
        tPS2SoundMemNode* freeNode = &((&mNodes[0]))[mFreeNodeHead];
        for (;;) {
            if (n->lowAddr < freeNode->lowAddr) {
                if (prev) {
                    n->next = prev->next;
                    prev->next = n->index;
                } else {
                    n->next = mFreeNodeHead;
                    mFreeNodeHead = n->index;
                }
                return;
            }
            if (freeNode->next < 0) {
                freeNode->next = n->index;
                n->next = -1;
                return;
            }
            prev = freeNode;
            freeNode = &((&mNodes[0]))[freeNode->next];
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_SetSoundMemLoadLimits);
#ifdef SKIP_ASM
extern "C" void SNDSYS_setMemLimits(int a, int b);


struct sSndPool2A9250 {
    char pad[0xC];
    tPS2SoundMemNode entries[32];
    int unk30C;
    int freeHead;
    char pad2[0x31C - 0x314];
    int unk31C;
};

/**
 * Restricts sound-memory loading to one memory node's address range.
 *
 * Sets +0x31C to 1 and calls SNDSYS_setMemLimits with the node's lowAddr and highAddr.
 *
 * @param memHandle  index of the sound-memory node
 */
void cPS2SoundManager::SetSoundMemLoadLimits(int memHandle)
{
    mMemLoadLimited = 1;
    tPS2SoundMemNode* ps2SoundMemNode = &((&mNodes[0]))[memHandle];
    SNDSYS_setMemLimits(ps2SoundMemNode->lowAddr, ps2SoundMemNode->highAddr);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_RestoreSoundMemLoadLimits);
#ifdef SKIP_ASM
extern "C" void SNDSYS_setMemLimits(int a, int b);

/**
 * Lifts the sound-memory load limit set by cPS2SoundManager_SetSoundMemLoadLimits.
 *
 * Calls SNDSYS_setMemLimits(-1, mMainMemHighAddr) and clears +0x31C.
 */
void cPS2SoundManager::RestoreSoundMemLoadLimits()
{
    SNDSYS_setMemLimits(-1, mMainMemHighAddr);
    mMemLoadLimited = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_TakeUnusedNode);
#ifdef SKIP_ASM

struct s2A92B8 {
    char pad[0xC];
    tPS2SoundMemNode entries[32];
    int unk30C;
    int freeHead;
};

/**
 * Takes a node from the PS2 sound manager's chain of unused memory nodes (mUnusedNodeHead), or returns 0 when none is
 * left.
 *
 * The node is unlinked (next = -1). cPS2SoundManager_AllocSoundMem uses it.
 *
 * @return the node (tPS2SoundMemNode), or 0
 */
extern "C" tPS2SoundMemNode* cPS2SoundManager_TakeUnusedNode(cPS2SoundManager* self)
{
    int i = self->mUnusedNodeHead;
    if (i < 0) {
        return 0;
    }
    tPS2SoundMemNode* ps2SoundMemNode = &((&self->mNodes[0]))[i];
    self->mUnusedNodeHead = ps2SoundMemNode->next;
    ps2SoundMemNode->next = -1;
    return ps2SoundMemNode;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_FreeTopBlock);
#ifdef SKIP_ASM
extern "C" void SNDSYS_setMemLimits(int a, int b);

/**
 * Returns the block at the top of main sound memory to the pool and puts its node back on the unused chain.
 *
 * mMainMemHighAddr becomes the node's highAddr; the node's lowAddr / highAddr become -1, size and state 0 (unused), and
 * it is pushed on mUnusedNodeHead. Unless load limits are set (mMemLoadLimited), SNDSYS_setMemLimits(-1, mMainMemHighAddr)
 * applies the new limit. cPS2SoundManager_DeallocSoundMem calls it.
 *
 * @param memNode  the memory node
 */
extern "C" void cPS2SoundManager_FreeTopBlock(cPS2SoundManager* self, tPS2SoundMemNode* memNode)
{
    self->mMainMemHighAddr = memNode->highAddr;
    memNode->lowAddr = -1;
    memNode->highAddr = -1;
    memNode->size = 0;
    memNode->state = 0;
    memNode->next = self->mUnusedNodeHead;
    self->mUnusedNodeHead = memNode->index;
    if (self->mMemLoadLimited == 0) {
        SNDSYS_setMemLimits(-1, self->mMainMemHighAddr);
    }
}
#endif

