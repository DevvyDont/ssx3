#include "common.h"
#include "ealib/snd.h"
#include "ealib/hlafile.h"
#include "vtable.h"
#include "sound/ssxaudio.h"
#include "sound/songinstance.h"
#include "sound/speechinstance.h"
#include "sound/soundsys.h"
#include "sound/bankmonitor.h"
#include "sound/pathfinderplat.h"
#include "sound/speechsys.h"
#include "sound/musicsys.h"
#include "sound/streamsys.h"
#include "sound/bankmanager.h"

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_cBankMonitor);
#ifdef SKIP_ASM
struct sSndPlayerD9C8;
extern "C" sSndPlayerD9C8* cMonitor_cMonitor(sSndPlayerD9C8* self, int inchrg, int heap);
extern "C" void* cSoundSys_cSoundSys(cSoundSys* self, int a);
extern "C" void* __20cBankMonitorInstance(void* self);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_mBankMonitorInstance[];
extern void* cBankMonitor_vtbl[];

struct sBmVEntryCD08 {
    short delta;
    short index;
    void* fn;
};
struct sBmVtblCD08 {
    sBmVEntryCD08 e[4];
} __attribute__((aligned(8)));
extern const sBmVtblCD08 D_00483B88;

struct sBmVoiceCD08 {
    char pad_0x0[0xC0];
    sBmVoiceCD08() { __20cBankMonitorInstance(this); }
    void operator delete[](void* p, unsigned int size);
};

// The constructor's view of a cBankMonitor; the fields from mReEntry on carry the class's member names at the same
// offsets (sound/bankmonitor.h).
struct sBankMonitorCD08 {
    char* vbase;            // 0x0
    char pad_0x4[0x8D8 - 0x4];
    void** vtbl;            // 0x8D8
    volatile int mReEntry;  // 0x8DC
    int mDopplerEnable;     // 0x8E0
    int mPositionalEnable;  // 0x8E4
    int mBankMonitorPaused; // 0x8E8
    sBmVoiceCD08* voices;   // 0x8EC
    char vb[0x4];           // 0x8F0
};

/**
 * Constructs a bank monitor: the cMonitor part and 64 bank-sound instances ("mBankMonitorInstance").
 *
 * When @p inchrg is set it also constructs the cSoundSys virtual base at +0x8F0. Doppler and positional sound
 * start on (+0x8E0, +0x8E4) and the re-entry count at 0.
 *
 * @param inchrg  nonzero when this object is complete and must construct its virtual base
 * @param heap    the audio mode passed to the cSoundSys base (On Tour audioMode)
 * @return self
 */
extern "C" sBankMonitorCD08* cBankMonitor_cBankMonitor(sBankMonitorCD08* self, int inchrg, int heap)
{
    if (inchrg) {
        self->vbase = self->vb;
        cSoundSys_cSoundSys((cSoundSys*)self->vb, heap);
    }
    cMonitor_cMonitor((sSndPlayerD9C8*)self, 0, heap);
    ((cSoundSys*)self->vbase)->mVtbl = (void**)&D_00483B88;
    if (!inchrg) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-up (copied vtable on the stack), written out by hand.
        sBmVtblCD08 vt = D_00483B88;
        ((cSoundSys*)self->vbase)->mVtbl = (void**)&vt;
        char* base = self->vbase - 0x8F0;
        int thisAdjust = (char*)self - base;
        vt.e[1].delta = D_00483B88.e[1].delta + thisAdjust;
    }
    self->vtbl = cBankMonitor_vtbl;
    self->mReEntry = 0;
    self->mDopplerEnable = 1;
    self->mPositionalEnable = 1;
    sBmVoiceCD08** slot = &self->voices;
    *slot = new (str_mBankMonitorInstance, 0, 0) sBmVoiceCD08[64];
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor__cBankMonitor);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void _$_20cBankMonitorInstance(void* self, int flags);
struct sSndPlayerDAC8;
extern "C" void cMonitor__cMonitor(cMonitor* self, int flags);
extern "C" void cSoundSys__cSoundSys(cSoundSys* self, int flags);
extern void* cBankMonitor_vtbl[];

struct sBmVEntryE40 {
    short delta;
    short index;
    void* fn;
};
struct sBmVtblE40 {
    sBmVEntryE40 e[4];
} __attribute__((aligned(8)));
extern const sBmVtblE40 D_00483B88_E40 __asm__("D_00483B88");


struct sBankMonitorE40 {
    char* vbase;                // 0x0
    char pad_0x4[0x8D8 - 0x4];
    void** vtbl;                // 0x8D8
    char pad_0x8DC[0x8EC - 0x8DC];
    cBankMonitorInstance* voices;        // 0x8EC
};

/**
 * Destroys a bank monitor: its 64 instances, then the cMonitor part.
 *
 * With bit 1 of @p flags it also destroys the cSoundSys virtual base.
 *
 * @param flags  g++ destructor flags: bit 1 destroys the virtual base, bit 0 frees the object
 */
extern "C" void cBankMonitor__cBankMonitor(cBankMonitor* self, int flags)
{
    self->mVtbl = cBankMonitor_vtbl;
    self->mpVSoundSys->mVtbl = (void**)(&D_00483B88_E40);
    if (flags == 0) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-up (copied vtable on the stack), written out by hand.
        sBmVtblE40 vt = D_00483B88_E40;
        self->mpVSoundSys->mVtbl = (void**)(&vt);
        char* base = (char*)self->mpVSoundSys - 0x8F0;
        int thisAdjust = (char*)self - base;
        vt.e[1].delta = D_00483B88_E40.e[1].delta + thisAdjust;
    }
    if (self->mBankMonitorInstance != 0) {
        cBankMonitorInstance* bankInstance = self->mBankMonitorInstance + ((int*)self->mBankMonitorInstance)[-4];
        while (self->mBankMonitorInstance != bankInstance) {
            bankInstance--;
            _$_20cBankMonitorInstance(bankInstance, 0);
        }
        cMemMan_free((char*)self->mBankMonitorInstance - 0x10);
    }
    cMonitor__cMonitor((cMonitor*)(self), 0);
    if (flags & 2) {
        cSoundSys__cSoundSys((cSoundSys*)self->mpVSoundSys, 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_FindFreeInstance);
#ifdef SKIP_ASM
extern "C" int cBankMonitorInstance_IsFree(void*);

/**
 * Finds a free bank-sound instance, skipping some free ones.
 *
 * @param n  which free instance to return: 1 for the first (On Tour SkipCount)
 * @return the instance index, or -1 if there are not that many free
 */
int cBankMonitor::FindFreeInstance(int n)
{
    for (int i = 0; i < 64; i++) {
        if (cBankMonitorInstance_IsFree(&mBankMonitorInstance[i]) != 0) {
            if (--n == 0) {
                return i;
            }
        }
    }
    return -1;
}
#endif

#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
class cController;
class cBankMonitorInstance;
// Sound callback: a g++ 2.95 pointer to member function (8 bytes), as cSndPlayOpts::OPTS_Callback takes it
// (same type as sound/soundsys).
typedef void (cController::*sSndCallback)(cController*, cBankMonitorInstance*, float);
extern const sSndCallback D_004A37D8[];
extern const sSndCallback D_004A37E0[];
struct sSndVtSlot {
    short delta;
    short index;
    void* fn;
};
struct sBmQuad {
    int w[4];
} __attribute__((aligned(16)));

// One entry of cSndPlayOpts' option stack (mCurrent[], 0x18 bytes); byte 0 seeds mBaseVol.
struct sSndOptsRec {
    signed char vol;
    char pad1[3];
    int f4[5];
};

// One cSndPlayOpts mDelay[] entry (reset to 100 / 0 / 90 / 50 on every push).
struct sSndOptsDelay {
    int f0;
    int f4;
    int f8;
    int fC;
};

// cSndPlayOpts (prototype class, as in sound/soundsys): the play-option stack; push = cur++ and copy def into
// ch[cur] (ch = mCurrent, vals = mBaseVol). From mPosStatic on the fields carry cSndPlayOpts's member names at the
// same offsets (sound/bankmonitor.h).
struct sSndPlayOptsView {
    char* vbase;               // 0x00 virtual-base pointer
    sSndOptsRec def;           // 0x04
    volatile int cur;          // 0x1C
    sSndOptsRec* ch;           // 0x20
    sBmQuad* tgt[4];           // 0x24
    int* mPosStatic;           // 0x34
    int* vals;                 // 0x38
    int* mVolControl;          // 0x3C
    sSndCallback callbacks[4]; // 0x40
    int* mPtr;                 // 0x60
    int* mViewport;            // 0x64
    float* mVanish;            // 0x68
    float* mRadius;            // 0x6C
    sSndOptsDelay* mDelay;     // 0x70
    int* mExternalVol;         // 0x74
    int* mMapToMaxVol;         // 0x78
    int* mCloseOnEnd;          // 0x7C
    int* mTwoPlayerPan;        // 0x80
    int* mTwoPlayerSide;       // 0x84
};

// A complete cSpeechSys (the in-charge ctor builds it): g++ 2.95 puts the virtual bases after the non-virtual part.
struct cSpeechSysComplete {
    cSpeechSys mSpeechSys;          // 0x000
    cSoundSys mSoundSys;            // 0x1C0
    cBankManager mBankManager;      // 0x1C8
    cStreamSys mStreamSys;          // 0xC98
};

// A complete cMusicSys (the in-charge ctor builds it): its virtual bases follow the 0x5448-byte non-virtual part.
struct cMusicSysComplete {
    cMusicSys mMusicSys;            // 0x0000
    cSoundSys mSoundSys;            // 0x5448
    cBankManager mBankManager;      // 0x5450
    cStreamSys mStreamSys;          // 0x5F20
};
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_Create);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* __20cBankMonitorInstance(void* self);

// One of a bank monitor's 64 voices (0xC0 bytes): a cBankMonitorInstance. The fields from mCloseStateLocked on carry
// that class's member names at the same offsets (sound/bankmonitor.h; mVolControl is a float* there).
struct sBmVoice {
    int kind;               // 0x00 (1 / 2: which sound system plays it)
    int id;                 // 0x04
    int valid;              // 0x08
    int handle;             // 0x0C
    char pad10[0x58 - 0x10];
    int mCloseStateLocked;  // 0x58
    float value;            // 0x5C
    int mVolControl;        // 0x60
    int vol;                // 0x64
    sBmQuad* tgt;           // 0x68
    int f6C;                // 0x6C
    sBmQuad tgtCopy;        // 0x70
    int mPtr;               // 0x80
    int mViewport;          // 0x84
    float mVanish;          // 0x88
    float mRadius;          // 0x8C
    float mVolMultiplier;   // 0x90
    int mExternalVol;       // 0x94
    int mMapToMaxVol;       // 0x98
    int mCloseOnEnd;        // 0x9C
    int mTwoPlayerPan;      // 0xA0
    int mTwoPlayerSide;     // 0xA4
    sSndCallback callback;  // 0xA8
    char padB0[0xC0 - 0xB0];
    sBmVoice() { __20cBankMonitorInstance(this); }
    void operator delete[](void* p, unsigned int size);
};

// sBankMonitorView: a sound player (cSndPlayOpts part at 0x0, player part up to 0x8DC) plus 64 voices;
// virtual base at 0x8F0.
struct sBankMonitorView : public sSndPlayOptsView {  // this unit's model of cBankMonitor (sound/bankmonitor.h)
    char pad88[0x8D8 - 0x88];
    void** vtbl;            // 0x8D8
    volatile int busy;      // 0x8DC
    int mDopplerEnable;     // 0x8E0
    int mPositionalEnable;  // 0x8E4
    int mBankMonitorPaused; // 0x8E8
    sBmVoice* voices;       // 0x8EC
    char vb[0x4];           // 0x8F0
};
extern "C" void cBankMonitorInstance_Reset(void*);
extern "C" void bxLogPrint(const char* fmt, ...);
extern "C" int cBankMonitor_FindFreeInstance(cBankMonitor* self, int n);
extern "C" void cBankMonitorInstance_SetLastDistance(void* voice, void* mon);
extern "C" void SNDstop(int h);
extern "C" int SNDseqstop(int h);
extern char D_00483250[];

// cSndPlayOpts::OPTS_GetCallback (prototype name; out-of-line copy in bx is cSndPlayOpts_OPTS_GetCallback)
static inline sSndCallback OPTS_GetCallback_Create(sBankMonitorView* s)
{
    return s->callbacks[s->cur];
}

static inline void setCallback_Create(sBmVoice* v, sSndCallback k)
{
    v->callback = k;
}

/**
 * Starts monitoring a playing sound in a free bank monitor voice, using the current play options.
 *
 * The voice is reset (cBankMonitorInstance_Reset) and gets the handle, kind and volume, plus the current option slot's
 * target position, callback and other settings; the target is copied into the voice when the options ask for that. With
 * no free voice it logs "Out of BankMonitor instances" and releases the handle with SNDstop() (MONITOR_BANK) or
 * SNDseqstop() (MONITOR_MIDI). The busy counter (+0x8DC) is raised for the duration.
 *
 * @param id    the sound handle; negative marks the voice invalid
 * @param vol   the volume to store
 * @param kind  the kind of sound, 1 or 2 (which release call is used on failure)
 * @return the voice, or 0 when no voice was free
 */
extern "C" sBmVoice* cBankMonitor_BANKMONITOR_Create(sBankMonitorView* self, int id, int vol, int kind)
{
    sBmVoice* instance = 0;
    self->busy++;
    int idx = ((cBankMonitor*)self)->FindFreeInstance(self->busy);
    if (idx >= 0) {
        instance = &self->voices[idx];
        cBankMonitorInstance_Reset(instance);
        instance->kind = kind;
        instance->id = id;
        instance->mVolControl = self->mVolControl[self->cur];
        instance->vol = vol;
        instance->tgt = self->tgt[self->cur];
        setCallback_Create(instance, OPTS_GetCallback_Create(self));
        instance->mPtr = self->mPtr[self->cur];
        instance->mViewport = self->mViewport[self->cur];
        if (self->mPosStatic[self->cur] != 0) {
            instance->tgtCopy = *instance->tgt;
            instance->tgt = &instance->tgtCopy;
        }
        instance->mVanish = self->mVanish[self->cur];
        instance->mRadius = self->mRadius[self->cur];
        instance->mExternalVol = self->mExternalVol[self->cur];
        instance->mMapToMaxVol = self->mMapToMaxVol[self->cur];
        int closeOnEnd = self->mCloseOnEnd[self->cur];
        if (instance->mCloseStateLocked == 0)
            instance->mCloseOnEnd = closeOnEnd;
        instance->mVolMultiplier = -1.0f;
        instance->mTwoPlayerPan = self->mTwoPlayerPan[self->cur];
        instance->mTwoPlayerSide = self->mTwoPlayerSide[self->cur];
        if (self->tgt[self->cur] != 0)
            cBankMonitorInstance_SetLastDistance(instance, self);
        instance->valid = id >= 0;
    } else {
        bxLogPrint(D_00483250);
        switch (kind) {
        case MONITOR_BANK:
            SNDstop(id);
            break;
        case MONITOR_MIDI:
            SNDseqstop(id);
            break;
        }
    }
    self->busy--;
    return instance;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_Pause);
#ifdef SKIP_ASM
extern "C" void cBankMonitorInstance_Pause(void*);

/**
 * Pauses every bank-sound instance and marks the monitor paused (+0x8E8).
 */
void cBankMonitor::BANKMONITOR_Pause()
{
    mBankMonitorPaused = 1;
    for (int i = 0; i < 64; i++) {
        cBankMonitorInstance_Pause(&mBankMonitorInstance[i]);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_Resume);
#ifdef SKIP_ASM
extern "C" void cBankMonitorInstance_Resume(void*);

/**
 * Resumes every bank-sound instance and clears the monitor's paused flag (+0x8E8).
 */
void cBankMonitor::BANKMONITOR_Resume()
{
    for (int i = 0; i < 64; i++) {
        cBankMonitorInstance_Resume(&mBankMonitorInstance[i]);
    }
    mBankMonitorPaused = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_FadeOut);
#ifdef SKIP_ASM
extern "C" void* cBankMonitor_BANKMONITOR_GetHandle(cBankMonitor* self, int idx, int a2);
extern "C" void cBankMonitorInstance_Fade(void* voice, int a, int b, float v);

/**
 * Fades out one playing bank sound: cBankMonitorInstance::Fade(@p FadeTime, -1, 0) on the voice that
 * cBankMonitor_BANKMONITOR_GetHandle finds, when there is one.
 *
 * @param idx  the sound's handle (On Tour int Handle)
 * @param type  passed to GetHandle (On Tour eMonitorType type)
 * @param FadeTime  the fade time (On Tour float FadeTime)
 */
void cBankMonitor::BANKMONITOR_FadeOut(int idx, int type, float FadeTime)
{
    void* voice = BANKMONITOR_GetHandle(idx, type);
    if (voice != 0) {
        cBankMonitorInstance_Fade(voice, -1, 0, FadeTime);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_Destroy);
#ifdef SKIP_ASM
extern "C" void cBankMonitorInstance_Reset(void*);

/**
 * Destroys one bank-sound instance (cBankMonitorInstance_Reset).
 *
 * @param i  the instance index (On Tour Index)
 */
void cBankMonitor::BANKMONITOR_Destroy(int i)
{
    cBankMonitorInstance_Reset(&mBankMonitorInstance[i]);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_DestroyAll);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_Destroy(cBankMonitor* self, int i);

/**
 * Destroys all 64 of the bank monitor's monitored sounds (cBankMonitor_BANKMONITOR_Destroy on each index).
 *
 * cBankMonitor's virtual slot 2 (cBankMonitor_vtbl). A cSSXAudio method in sound/ssxAudio.cpp calls it on the shared
 * cBankManager's monitor (+0x1D8).
 */
void cBankMonitor::BANKMONITOR_DestroyAll()
{
    int i;
    for (i = 0; i < 64; i++) {
        BANKMONITOR_Destroy(i);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_DestroyAll1);
#ifdef SKIP_ASM
extern "C" void cBankMonitorInstance_Reset(void*);

struct sBmKeyD410 {
    short a;
    short id;
    union {
        int v;
        short s;
    };
};

static inline int cBankMonitor_BANKMONITOR_DestroyAll1_match(cBankMonitorInstance* e, sBmKeyD410 k)
{
    // e->mCallback viewed as a key: its index, then its vtable offset (virtual) or function word
    int r = 0;
    if (k.id == ((sBmKeyD410*)&e->mCallback)->id) {
        if ((k.id != -1 && k.s == ((sBmKeyD410*)&e->mCallback)->s) || k.v == ((sBmKeyD410*)&e->mCallback)->v) {
            r = 1;
        }
    }
    return r;
}

/**
 * Resets every monitored sound whose callback is the given pointer to member function.
 *
 * Walks the 64 cBankMonitorInstance records (mBankMonitorInstance, 0xC0 each) and calls cBankMonitorInstance_Reset on
 * each one whose callback (mCallback, a g++ 2.95 pointer to member: delta, index, then the function or vtable offset)
 * matches key: same index, and the same vtable offset (virtual) or the same function word.
 *
 * cBankMonitor's virtual slot 3 (cBankMonitor_vtbl).
 *
 * @param key  the callback to match (8-byte pointer to member function)
 */
extern "C" void cBankMonitor_BANKMONITOR_DestroyAll1(cBankMonitor* self, sBmKeyD410 key)
{
    for (int i = 0; i < 64; i++) {
        if (cBankMonitor_BANKMONITOR_DestroyAll1_match(&self->mBankMonitorInstance[i], key)) {
            cBankMonitorInstance_Reset(&self->mBankMonitorInstance[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_DestroyAllWithPtr);
#ifdef SKIP_ASM
/**
 * Destroys every bank-sound instance whose owner pointer (+0x80, On Tour mPtr) matches.
 *
 * @param id  the owner pointer to match (On Tour Ptr), as an int
 */
void cBankMonitor::BANKMONITOR_DestroyAllWithPtr(int id)
{
    for (int i = 0; i < 64; i++) {
        if ((int)mBankMonitorInstance[i].mPtr == id) {
            cBankMonitorInstance_Reset(&mBankMonitorInstance[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_GetHandle);
#ifdef SKIP_ASM

/**
 * Finds the active bank-sound instance with a given handle and monitor type.
 *
 * @param idx  the sound handle (+0x4, On Tour Handle)
 * @param key  the monitor type (+0x0; On Tour eMonitorType: 1 bank, 2 MIDI, 3 stream)
 * @return the instance, or 0 if none matches
 */
void* cBankMonitor::BANKMONITOR_GetHandle(int idx, int key)
{
    for (int i = 0; i < 64; i++) {
        if (cBankMonitorInstance_IsFree(&mBankMonitorInstance[i]) == 0
            && mBankMonitorInstance[i].mMonitorType == key
            && mBankMonitorInstance[i].mHandle == idx) {
            return &mBankMonitorInstance[i];
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_StopHandle);
#ifdef SKIP_ASM
extern "C" void* cBankMonitor_BANKMONITOR_GetHandle(cBankMonitor* self, int idx, int a2);
extern "C" void cBankMonitorInstance_Reset(void*);
extern "C" void cBankMonitorInstance_Fade(void* voice, int a, int b, float v);

/**
 * Stops the bank sound with a given handle and type, at once or with a fade.
 *
 * The names of the last two parameters are swapped against their use (On Tour: Handle, float FadeTime, type).
 *
 * @param idx       the sound handle
 * @param type  despite its name, the monitor type passed to GetHandle
 * @param FadeTime      despite its name, the fade time; 0 stops the sound at once
 */
void cBankMonitor::BANKMONITOR_StopHandle(int idx, int type, float FadeTime)
{
    void* voice = BANKMONITOR_GetHandle(idx, type);
    if (voice != 0) {
        if (FadeTime != 0.0f) {
            cBankMonitorInstance_Fade(voice, -1, 1, FadeTime);
        } else {
            cBankMonitorInstance_Reset(voice);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_StopBank);
#ifdef SKIP_ASM
extern "C" int cBankMonitorInstance_IsFree(void*);
extern "C" void cBankMonitorInstance_Reset(void*);
// PORT: same ABI as the unit's cBankMonitorInstance_Fade(void*, int, int, float); the float is
// the 2nd parameter in the original, which orders the argument moves.
void cBankMonitorInstance_Fade_f(void* voice, float v, int a, int b) __asm__("cBankMonitorInstance_Fade");


/**
 * Stops every playing sound of one bank, at once or with a fade.
 *
 * Only active bank instances (MONITOR_BANK) whose bank id (+0xC) matches are stopped.
 *
 * @param id  the bank's handle (On Tour BankHandle)
 * @param FadeTime   fade time; 0 stops at once (On Tour FadeTime)
 */
void cBankMonitor::BANKMONITOR_StopBank(int id, float FadeTime)
{
    for (int i = 0; i < 64; i++) {
        if (cBankMonitorInstance_IsFree(&mBankMonitorInstance[i]) == 0) {
            // PORT: pointer arithmetic through int
            cBankMonitorInstance* instance = (cBankMonitorInstance*)(i * 0xC0 + (int)mBankMonitorInstance);
            if (instance->mMonitorType == MONITOR_BANK && instance->mRestartInfo.bank.mBank == id) {
                if (FadeTime != 0.0f) {
                    cBankMonitorInstance_Fade_f(instance, FadeTime, -1, 1);
                } else {
                    cBankMonitorInstance_Reset(instance);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_Active);
#ifdef SKIP_ASM
extern "C" int cBankMonitorInstance_IsFree(void*);


/**
 * Counts the bank monitor's busy instances (of 64, 0xC0 bytes each) whose active word is set.
 *
 * @return the number of active instances
 */
int cBankMonitor::BANKMONITOR_Active()
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (cBankMonitorInstance_IsFree(&mBankMonitorInstance[i]) == 0) {
            // PORT: pointer arithmetic through int
            cBankMonitorInstance* instance = (cBankMonitorInstance*)(i * 0xC0 + (int)mBankMonitorInstance);
            if (instance->mActive != 0) {
                count++;
            }
        }
    }
    return count;
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_Used);
#ifdef SKIP_ASM
extern "C" int cBankMonitorInstance_IsFree(void*);

/**
 * Counts the bank monitor's busy instances (of 64): those cBankMonitorInstance::IsFree reports in use.
 *
 * @return the number of instances in use
 */
int cBankMonitor::BANKMONITOR_Used()
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (cBankMonitorInstance_IsFree(&mBankMonitorInstance[i]) == 0) {
            count++;
        }
    }
    return count;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_CameraChange);
#ifdef SKIP_ASM

/**
 * Forgets every instance's last listener distance (+0x5C = -1) after a camera cut, so no Doppler jump is
 * heard.
 */
void cBankMonitor::BANKMONITOR_CameraChange()
{
    for (int i = 0; i < 64; i++) {
        mBankMonitorInstance[i].mLastDist = -1.0f;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitorInstance_cMonitorInstance__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a cMonitorInstance: mEnabled = 0, mCountReset = -1 (free).
 *
 * cMonitor_cMonitor runs it on each of its 48 mMonitorInstance entries.
 *
 * @return self
 */
void* cMonitorInstance_cMonitorInstance(void* vself)
{
    cMonitorInstance* self = (cMonitorInstance*)vself;
    self->mEnabled = 0;
    self->mCountReset = cMonitorInstance::kFree;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", _$_16cMonitorInstance);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void _$_16cMonitorInstance(cMonitorInstance* self, int flags);

/**
 * Destroys a cMonitorInstance (g++ 2.95 destructor): nothing to clean up; frees the object when bit 0 of flags is set.
 *
 * cMonitor__cMonitor calls it with 0 on each of its 48 mMonitorInstance entries.
 */
cMonitorInstance::~cMonitorInstance()
{
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitorInstance_Create);
#ifdef SKIP_ASM
extern "C" void cMonitorInstance_Reset(cMonitorInstance*);
void cMonitorInstance_CountReset(void* self, int a1);
void cMonitorInstance_Enable(void* self);


/**
 * Sets up a monitor instance: a callback to run every @p Freq ms with up to five values.
 *
 * Resets the instance, sets the countdown, stores the callback (an 8-byte member-function pointer) and the
 * values, then enables it (cMonitorInstance_Enable, probably Enable).
 *
 * @param Freq   period of the callback in milliseconds (On Tour Freq)
 * @param key  the callback, a member-function pointer (On Tour CallbackFn)
 * @param Ptr    pointer value for the callback (mPtr, On Tour Ptr)
 * @param Data1    first data word (mData1)
 * @param Data2    second data word (mData2)
 * @param Data3    third data word (mData3)
 * @param Data4    fourth data word (mData4)
 */
// PORT: 64-bit long sound key
void cMonitorInstance::Create(int Freq, long key, int Ptr, int Data1, int Data2, int Data3, int Data4)
{
    Reset();
    CountReset(Freq);
    // PORT: the 8-byte key is copied through its in-memory layout
    *(&mCallback) = *(tSndCallback*)&key;
    mPtr = (void*)Ptr;
    mData1 = Data1;
    mData2 = Data2;
    mData3 = Data3;
    mData4 = Data4;
    this->Enable();
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitorInstance_IsFree);
#ifdef SKIP_ASM
/**
 * Tells whether a monitor instance is unused (its reset count is kFree).
 *
 * @return true when free, otherwise false
 */
bool cMonitorInstance::IsFree()
{
    return mCountReset == kFree;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitorInstance_Reset);
#ifdef SKIP_ASM
void cMonitorInstance_Disable(void* self);
void cMonitorInstance_CountReset(void* self, int a1);

/**
 * Frees a monitor instance: disables it (cMonitorInstance_Disable, probably Disable) and sets its reset count to kFree.
 */
void cMonitorInstance::Reset()
{
    Disable();
    CountReset(kFree);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitorInstance_CountReset__FPvi);
#ifdef SKIP_ASM
/**
 * Sets a monitor instance's period: mCountReset and mCountdown both become periodMs.
 *
 * cMonitorInstance_Create calls it with its Freq argument, cMonitorInstance_Reset with -1 (which marks the instance
 * free).
 *
 * @param periodMs  the callback period in ms, or -1 to free the instance
 */
void cMonitorInstance::CountReset(int periodMs)
{
    mCountReset = periodMs;
    mCountdown = periodMs;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitorInstance_Enable__FPv);
#ifdef SKIP_ASM
/**
 * Enables a monitor instance (mEnabled = 1).
 *
 * cMonitorInstance_Create calls it last.
 */
void cMonitorInstance::Enable()
{
    ((cMonitorInstance*)this)->mEnabled = 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitorInstance_Disable__FPv);
#ifdef SKIP_ASM
/**
 * Disables a monitor instance (mEnabled = 0).
 *
 * cMonitorInstance_Reset calls it before marking the instance free.
 */
void cMonitorInstance::Disable()
{
    mEnabled = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitor_Tick100Hz);
#ifdef SKIP_ASM
class cMonitor_Tick100Hz_cObj {
public:
    char data[0x8D8];
    virtual void MONITOR_Update();  // slot 1: cMonitor_vtbl holds cMonitor_MONITOR_Update there
};


/**
 * The sound driver's 100 Hz tick for the monitor: calls the current monitor's virtual slot 1 (cMonitor_MONITOR_Update).
 *
 * Calls through gMonitor's vptr (mVtbl, +0x8D8). cMonitor_MONITOR_BeginMonitoring registers it with
 * SNDSYS_add100hzclient and cMonitor__cMonitor removes it.
 */
extern "C" void cMonitor_Tick100Hz()
{
    ((cMonitor_Tick100Hz_cObj*)gMonitor)->MONITOR_Update();
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitor_cMonitor);
#ifdef SKIP_ASM
// FAKEMATCH: a per-function volatile view of fields 0x88/0x8C/0x90 orders the %hi(cMonitor_vtbl) lui after the 0x88 store; plain form 96.88%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" void* cSndPlayOpts_cSndPlayOpts(void* self, int inchrg, int heap);
extern "C" void* cSoundSys_cSoundSys(cSoundSys* self, int a);
extern void* cMonitor_vtbl[];

struct sBmVEntryD9C8 {
    short delta;
    short index;
    void* fn;
};
struct sBmVtblD9C8 {
    sBmVEntryD9C8 e[4];
} __attribute__((aligned(8)));
extern const sBmVtblD9C8 D_00483BC0;

struct sBmSlotD9C8 {
    char pad_0x0[0x2C];
};

struct sSndPlayerD9C8 {
    char* vbase;                // 0x0
    char pad_0x4[0x88 - 0x4];
    volatile int f88;                    // 0x88
    volatile int f8C;                    // 0x8C
    volatile int f90;                    // 0x90
    int f94;                    // 0x94
    sBmSlotD9C8 slots[48];      // 0x98
    void** vtbl;                // 0x8D8
    char vb[0x4];               // 0x8DC
};

/**
 * Constructs the sound monitor: its play-options part, 48 timed callback instances, and the global gMonitor.
 *
 * When @p inchrg is set it also constructs the cSoundSys virtual base at +0x8DC. The counter and the fields
 * at +0x8C/+0x90 start at 0.
 *
 * @param inchrg  nonzero when this object is complete and must construct its virtual base
 * @param heap    the audio mode passed to the bases (On Tour audioMode)
 * @return self
 */
extern "C" sSndPlayerD9C8* cMonitor_cMonitor(sSndPlayerD9C8* self, int inchrg, int heap)
{
    if (inchrg) {
        self->vbase = self->vb;
        cSoundSys_cSoundSys((cSoundSys*)self->vb, heap);
    }
    cSndPlayOpts_cSndPlayOpts(self, 0, heap);
    *(const sBmVtblD9C8**)(self->vbase + 4) = &D_00483BC0;
    if (!inchrg) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-up (copied vtable on the stack), written out by hand.
        sBmVtblD9C8 vt = D_00483BC0;
        *(sBmVtblD9C8**)(self->vbase + 4) = &vt;
        char* base = self->vbase - 0x8DC;
        int thisAdjust = (char*)self - base;
        vt.e[1].delta = D_00483BC0.e[1].delta + thisAdjust;
    }
    self->f88 = 0;
    self->f8C = 0;
    self->vtbl = cMonitor_vtbl;
    self->f90 = 0;
    {
        sBmSlotD9C8* slot = self->slots;
        for (int i = 47; i != -1; i--, slot++) cMonitorInstance_cMonitorInstance(slot);
    }
    gMonitor = (cMonitor*)((cMonitor_Tick100Hz_cObj*)self);
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitor__cMonitor);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void SNDSYS_remove100hzclient(int id);
extern "C" void cSndPlayOpts__cSndPlayOpts(void* self, int flags);
extern "C" void cSoundSys__cSoundSys(cSoundSys* self, int flags);
extern void* cMonitor_vtbl[];

struct sBmVEntryDAC8 {
    short delta;
    short index;
    void* fn;
};
struct sBmVtblDAC8 {
    sBmVEntryDAC8 e[4];
} __attribute__((aligned(8)));
extern const sBmVtblDAC8 D_00483BC0_DAC8 __asm__("D_00483BC0");



/**
 * Destroys the sound monitor: removes its timer callback (cMonitor_Tick100Hz), clears gMonitor and destroys its 48
 * instances and its play-options part.
 *
 * With bit 1 of @p flags it also destroys the cSoundSys virtual base.
 *
 * @param flags  g++ destructor flags: bit 1 destroys the virtual base, bit 0 frees the object
 */
extern "C" void cMonitor__cMonitor(cMonitor* self, int flags)
{
    cMonitorInstance* end = ((&self->mMonitorInstance[0])) + 48;
    self->mVtbl = cMonitor_vtbl;
    self->mpVSoundSys->mVtbl = (void**)(&D_00483BC0_DAC8);
    if (flags == 0) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-up (copied vtable on the stack), written out by hand.
        sBmVtblDAC8 vt = D_00483BC0_DAC8;
        self->mpVSoundSys->mVtbl = (void**)(&vt);
        char* base = (char*)self->mpVSoundSys - 0x8DC;
        int thisAdjust = (char*)self - base;
        vt.e[1].delta = D_00483BC0_DAC8.e[1].delta + thisAdjust;
    }
    // PORT: function pointer passed as int
    SNDSYS_remove100hzclient((int)cMonitor_Tick100Hz);
    gMonitor = 0;
    if (((&self->mMonitorInstance[0])) != 0) {
        cMonitorInstance* monitorInstance = end;
        while (((&self->mMonitorInstance[0])) != monitorInstance) {
            monitorInstance--;
            _$_16cMonitorInstance((cMonitorInstance*)((int*)monitorInstance), 0);
        }
    }
    cSndPlayOpts__cSndPlayOpts(((cBankMonitor*)self), 0);
    if (flags & 2) {
        cSoundSys__cSoundSys((cSoundSys*)self->mpVSoundSys, 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void cMonitor_Tick100Hz();
extern "C" void* SNDSYS_add100hzclient(void*);

//98.75%
INCLUDE_ASM("sound/bankmonitor", cMonitor_MONITOR_BeginMonitoring__FPv);
#ifdef SKIP_ASM
/**
 * Starts the bank monitor's periodic update by registering cMonitor_Tick100Hz as a sound-system client.
 *
 * cMonitor_Tick100Hz forwards to gMonitor's first virtual method; SNDSYS_add100hzclient (On Tour SNDSYS_add100hzclient) adds it to
 * the sound library's 100 Hz client list when the library's enable flag is set. The object pointer is not used.
 *
 * @return no meaningful value (SNDSYS_add100hzclient returns nothing)
 */
void* cMonitor::MONITOR_BeginMonitoring()
{
    return SNDSYS_add100hzclient((void*)cMonitor_Tick100Hz);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitor_MONITOR_Update);
#ifdef SKIP_ASM
/**
 * Counts one monitor tick (mCounter), unless the monitor is paused.
 */
void cMonitor::MONITOR_Update()
{
    if (mPaused == 0) {
        mCounter += 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMonitor_MONITOR_GetFreeInstance);
#ifdef SKIP_ASM
/**
 * Finds a free monitor instance, skipping some free ones.
 *
 * @param n  which free instance to return: 1 for the first (On Tour Count)
 * @return the instance index, or -1 if there are not that many free
 */
int cMonitor::MONITOR_GetFreeInstance(int n)
{
    for (int i = 0; i < 48; i++) {
        if (mMonitorInstance[i].IsFree() != 0) {
            if (--n == 0) {
                return i;
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitor_MONITOR_Create);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cMonitor_MONITOR_GetFreeInstance(cMonitor* self, int n);

void cMonitorInstance_Create_cb(cMonitorInstance* self, int a1, sSndCallback cb, int b, int c, int d, int e, int f)
    __asm__("cMonitorInstance_Create");

extern "C" int cMonitor_MONITOR_Create_pl(cMonitor* self, int a1, sSndCallback cb, int b, int c, int d, int e, int f)
    __asm__("cMonitor_MONITOR_Create");
/**
 * Starts a timed monitor callback: claims a free cMonitorInstance and fills it; returns its index or -1.
 *
 * Increments mReEntryCheck around the search, passes the new count to cMonitor_MONITOR_GetFreeInstance and, for a free
 * index, calls cMonitorInstance_Create on mMonitorInstance[index] with all the arguments. The callback is a pointer to
 * a cController member (tSndCallback constants such as D_004A3648); the instance calls it every freq ms while it is
 * enabled. The audio code uses it for delayed music changes, fades and checks.
 *
 * @param freq  the callback period in ms
 * @param cb  the callback (8-byte pointer to member function)
 * @param userPtr  mPtr of the instance
 * @param data1  mData1 (the callbacks read it as their argument)
 * @param data2  mData2
 * @param data3  mData3
 * @param data4  mData4
 * @return the index of the new instance in mMonitorInstance, or a negative value when none is free
 */
extern "C" int cMonitor_MONITOR_Create_pl(cMonitor* self, int freq, sSndCallback cb, int userPtr, int data1, int data2,
                                          int data3, int data4)
{
    self->mReEntryCheck = self->mReEntryCheck + 1;
    int idx = self->MONITOR_GetFreeInstance(self->mReEntryCheck);
    if (idx >= 0) {
        cMonitorInstance_Create_cb(&self->mMonitorInstance[idx], freq, cb, userPtr, data1, data2, data3, data4);
    }
    self->mReEntryCheck = self->mReEntryCheck - 1;
    return idx;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitor_MONITOR_Destroy);
#ifdef SKIP_ASM
/**
 * Resets monitor instance @p idx (mMonitorInstance[idx]) with cMonitorInstance_Reset.
 *
 * @param idx  the instance (On Tour int Index)
 */
void cMonitor::MONITOR_Destroy(int idx)
{
    mMonitorInstance[idx].Reset();
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitor_MONITOR_DestroyAll);
#ifdef SKIP_ASM
extern "C" void cMonitorInstance_Reset(cMonitorInstance*);

/**
 * Resets all 48 of the monitor's timed callbacks (cMonitorInstance_Reset on each mMonitorInstance).
 *
 * The audio system calls it on the cBankMonitor of the shared cBankManager when it purges or shuts down.
 */
void cMonitor::MONITOR_DestroyAll()
{
    char* instance = (char*)&mMonitorInstance;
    int i;
    for (i = 47; i >= 0; i--) {
        ((cMonitorInstance*)instance)->Reset();
        instance += 0x2C;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitor_MONITOR_DestroyAll1);
#ifdef SKIP_ASM
extern "C" void cMonitorInstance_Reset(cMonitorInstance*);

struct sBmKeyDDE0 {
    short a;
    short id;
    union {
        int v;
        short s;
    };
};

static inline int cMonitor_MONITOR_DestroyAll1_match(cMonitorInstance* e, sBmKeyDDE0 k)
{
    // e->mCallback viewed as a key: its index, then its vtable offset (virtual) or function word
    int r = 0;
    if (k.id == ((sBmKeyDDE0*)&e->mCallback)->id) {
        if ((k.id != -1 && k.s == ((sBmKeyDDE0*)&e->mCallback)->s) || k.v == ((sBmKeyDDE0*)&e->mCallback)->v) {
            r = 1;
        }
    }
    return r;
}

// Release-build assert: expands to an empty do { } while (0). Its loop notes keep GCC from reversing the
// slot loop into a count-down, as in the target.
#define ASSERT_DDE0(cond) do { } while (0)

// PORT: callers in this unit declare cMonitor_MONITOR_DestroyAll1(void*, long); the key is the same 8 bytes viewed as a struct.
void cMonitor_MONITOR_DestroyAll1_impl(void* self, sBmKeyDDE0 key) __asm__("cMonitor_MONITOR_DestroyAll1");

/**
 * Resets every one of the monitor's 48 timed callbacks whose callback is the given pointer to member function.
 *
 * An instance matches when its mCallback has the same index and the same vtable offset (virtual) or function word.
 * Callers use it to cancel pending callbacks such as the music fades (D_004A3838..D_004A3850) and the audio system's
 * delayed events (D_004A3648..).
 *
 * @param key  the callback to cancel (8-byte pointer to member function)
 */
void cMonitor_MONITOR_DestroyAll1_impl(void* vself, sBmKeyDDE0 key)
{
    cMonitor* self = (cMonitor*)vself;
    for (int i = 0; i < 48; i++) {
        if (cMonitor_MONITOR_DestroyAll1_match(&self->mMonitorInstance[i], key)) {
            self->mMonitorInstance[i].Reset();
        }
        ASSERT_DDE0(i < 48);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSoundSys_cSoundSys);
#ifdef SKIP_ASM
// The platform sound manager (cSoundManager). Slot names: [O] On Tour's cSoundManager declares Init, SetMode,
// Shutdown in that order, and the retail cSoundSys ctor, SetMode and dtor call slots 1, 2 and 3.
class cSoundSys_cSoundSys_cObj {
public:
    virtual void Init(int audioMode);
};

extern "C" cSoundSys_cSoundSys_cObj* cSoundManager_Construct();
extern char cSoundSys_vtbl[];

/**
 * Constructs the sound system base and makes it the current one (SoundSys).
 *
 * It creates the platform sound object (cSoundManager_Construct) and calls that object's first virtual with the mode.
 *
 * @param self       the cSoundSys
 * @param audioMode  the audio mode (On Tour eSOUNDDEF_MODE)
 * @return self
 */
extern "C" void* cSoundSys_cSoundSys(cSoundSys* self, int audioMode)
{
    self->mVtbl = (void**)cSoundSys_vtbl;
    SoundSys = (cSoundSys*)(((void*)self));
    cSoundSys_cSoundSys_cObj* obj = cSoundManager_Construct();
    *(cSoundSys_cSoundSys_cObj**)self = obj;
    obj->Init(audioMode);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSoundSys__cSoundSys);
#ifdef SKIP_ASM
// The platform sound manager (cSoundManager); slot names as in cSoundSys_cSoundSys's view.
class cSoundSys__cSoundSys_cObj {
public:
    virtual void Init();
    virtual void SetMode();
    virtual void Shutdown(int flags);
};

void operator_delete(int* ptr);
extern char cSoundSys_vtbl[];

/**
 * Destroys the sound system base: shuts down and deletes the platform sound object and clears SoundSys.
 *
 * The shutdown is the platform object's third virtual, called with @p flags.
 *
 * @param self   the cSoundSys
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cSoundSys__cSoundSys(cSoundSys* self, int flags)
{
    self->mVtbl = (void**)cSoundSys_vtbl;
    ((cSoundSys__cSoundSys_cObj*)self->pManager)->Shutdown(flags);
    operator_delete((int*)self->pManager);
    SoundSys = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSoundSys_SndAssert);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty function between cSoundSys__cSoundSys and the random-number helper; no caller found).
 */
extern "C" void cSoundSys_SndAssert(void) {

}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSoundSys_SndRand);
#ifdef SKIP_ASM
int BXrand();

/**
 * Returns a random number from BXrand().
 *
 * Declared without parameters, but callers pass the sound system as this (cSongInstance_GetRandomLoopBank and cSongInstance_GetRandomPhraseStart pass
 * SoundSys; others the cSoundSys pointer).
 *
 * @return BXrand()'s result
 */
extern "C" int cSoundSys_SndRand()
{
    return BXrand();
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSoundSys_SetMode);
#ifdef SKIP_ASM
class cSoundSys_SetMode_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8. The platform sound manager (cSoundManager); slot names as in
    // cSoundSys_cSoundSys's view.
    virtual void Init();
    virtual int SetMode(int mode);
};


/**
 * Passes a sound-mode change on to the sound manager this object wraps.
 *
 * Calls virtual slot 2 of the object stored at offset 0 with @p mode and returns its result. Every cSoundSys subclass
 * vtable keeps this as slot 2 (none overrides it).
 *
 * @param mode  the new sound mode (On Tour eSOUNDDEF_MODE mode)
 * @return the manager's result (On Tour eSOUNDMANAGER_SETMODE)
 */
int cSoundSys::SetMode(int mode)
{
    return ((cSoundSys_SetMode_cObj*)pManager)->SetMode(mode);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_getPlayer__Fi);
#ifdef SKIP_ASM
/**
 * Returns the Pathfinder sound back end's player-slot pointer D_004D3E98[slot] (24 slots).
 *
 * cSongInstance_GetStreamHandle reads its mHandle (the stream handle).
 *
 * @param slot  the slot index
 * @return the slot pointer (0 when the slot is unused)
 */
tPathPlayer* pathxSND_getPlayer(int slot)
{
    return D_004D3E98[slot];
}
#endif

extern "C" void* MEM_free(void* self);

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_alloc);
#ifdef SKIP_ASM
extern void* (*D_004A378C)(void*, int, int);
extern char str_pathxSND[];

/**
 * Allocates memory through the Pathfinder back end's allocation hook: D_004A378C("pathxSND", size, 0).
 *
 * pathxSND_init installs MEM_alloc as the hook.
 *
 * @param size  the size in bytes
 * @return the block, or 0 when no hook is installed
 */
extern "C" void* pathxSND_alloc(int size)
{
    void* block = 0;
    if (D_004A378C != 0) {
        block = D_004A378C(str_pathxSND, size, 0);
    }
    return block;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_free__FPv);
#ifdef SKIP_ASM
/**
 * Frees a block with MEM_free; pathxSND_init installs it as the Pathfinder back end's free hook (D_004A3794).
 *
 * @return MEM_free's result
 */
void* pathxSND_free(void* self)
{
    return MEM_free(self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_syncTask);
#ifdef SKIP_ASM
extern void (*D_004A3798)();

/**
 * Sync task of the Pathfinder sound back end: calls the client callback D_004A3798 when one is set.
 *
 * pathxSND_init adds it with SYNCTASK_add(pathxSND_syncTask, 5, 100); pathxSND_shutdown removes it.
 *
 * @return 0
 */
extern "C" int pathxSND_syncTask()
{
    if (D_004A3798 != 0) {
        D_004A3798();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_init);
#ifdef SKIP_ASM
extern "C" void REAL_abortmessage();
extern "C" void printf();
extern "C" void* MEM_alloc(void* a0, int a1, int a2);
extern "C" void* resizeBlock(void* block, int size);
extern "C" void SYNCTASK_add(int (*fn)(), int a, int b);
extern int D_004A379C;
extern void (*D_004A3780)();
extern void (*D_004A3784)();
extern void* (*D_004A378C)(void*, int, int);
extern void* (*D_004A3790)(void*, int);
extern void* (*D_004A3794)(void*);

/**
 * Starts the Pathfinder sound back end ("pathxSND"): clears its slot tables and installs its hooks and clients.
 *
 * Clears the 24 player-slot pointers D_004D3E98 and resets the 24 bank records D_004D3EF8 (words 0, 1, 2 = -1, word 6 =
 * 0). Stores cb in D_004A3798 and id in D_004A379C, registers id as a 100 Hz sound-system client
 * (SNDSYS_add100hzclient) and pathxSND_syncTask as a sync task (SYNCTASK_add(.., 5, 100)), and installs the hooks
 * D_004A3780 = REAL_abortmessage, D_004A3784 = printf, D_004A378C = MEM_alloc, D_004A3790 = resizeBlock,
 * D_004A3794 = pathxSND_free. Called by the Pathfinder library (func_003D25F0) when it opens its first player.
 *
 * @param cb  the callback the sync task pathxSND_syncTask calls
 * @param id  the 100 Hz client to register (a function address)
 */
extern "C" void pathxSND_init(void (*cb)(), int id)
{
    for (int i = 0; i < 24; i++) {
        D_004D3E98[i] = 0;
        D_004D3EF8[i].mPlayer = -1;
        D_004D3EF8[i].mHandle = -1;
        D_004D3EF8[i].mBankIndex = -1;
        D_004D3EF8[i].unk_18 = 0;
    }
    D_004A3798 = cb;
    D_004A379C = id;
    SNDSYS_add100hzclient((void*)id);
    SYNCTASK_add(pathxSND_syncTask, 5, 100);
    D_004A3780 = REAL_abortmessage;
    D_004A3784 = printf;
    D_004A378C = MEM_alloc;
    D_004A3790 = resizeBlock;
    D_004A3794 = pathxSND_free;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_shutdown);
#ifdef SKIP_ASM
extern "C" int pathxSND_syncTask();
extern "C" void SYNCTASK_del(int (*fn)());
extern "C" void SNDSYS_remove100hzclient(int id);
extern void (*D_004A3798)();
extern int D_004A379C;

/**
 * Stops the Pathfinder sound back end: unregisters its 100 Hz client and sync task when a callback is installed.
 *
 * Calls SNDSYS_remove100hzclient(D_004A379C) and SYNCTASK_del(pathxSND_syncTask), then clears D_004A3798 and D_004A379C.
 * Called by the Pathfinder library (PATH_closeVoice) when its last player closes.
 */
extern "C" void pathxSND_shutdown()
{
    if (D_004A3798 != 0) {
        SNDSYS_remove100hzclient(D_004A379C);
        SYNCTASK_del(pathxSND_syncTask);
        D_004A3798 = 0;
        D_004A379C = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_initPlayer);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);

/**
 * Initialises a Pathfinder player slot (0x70 bytes): zeroes it, then sets the handles to -1 and the dirty flag.
 *
 * Words +0x0, +0x4, +0x8 and +0x5C become -1, byte +0x66 (the request count) -1 and byte +0x65 1. Called by
 * pathxSND_openPlayer on a newly claimed slot.
 */
extern "C" void pathxSND_initPlayer(tPathPlayer* self)
{
    memset(self, 0, sizeof(tPathPlayer));
    self->mHandle = -1;
    self->mPatch = -1;
    self->mRequestCount = -1;
    self->mCurrentRequest = -1;
    self->mFadeTarget = -1;
    self->mDirty = 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_forgetPlayer);
#ifdef SKIP_ASM
/**
 * Clears every entry of the 24 player-slot pointers D_004D3E98 that points at the given slot.
 *
 * pathxSND_closePlayer calls it when it closes a slot.
 *
 * @param player  the player record to forget
 */
extern "C" void pathxSND_forgetPlayer(tPathPlayer* player)
{
    for (int i = 0; i < 24; i++) {
        if (D_004D3E98[i] == player) {
            D_004D3E98[i] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_findFreePlayer);
#ifdef SKIP_ASM
/**
 * Returns the index of the first unused Pathfinder player slot (D_004D3E98 entry 0), or -1 when all 24 are used.
 *
 * @return the slot index, or -1
 */
extern "C" int pathxSND_findFreePlayer(void)
{
    for (int i = 0; i < 24; i++) {
        if (D_004D3E98[i] == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_clearBank);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);

/**
 * Clears a Pathfinder bank record (40 bytes of D_004D3EF8) and sets its first three words (bank, handle, slot) to -1.
 *
 * pathxSND_closePlayer calls it for each bank of a closed player slot.
 */
extern "C" void pathxSND_clearBank(tPathBank* self)
{
    memset(self, 0, sizeof(tPathBank));
    self->mPlayer = -1;
    self->mHandle = -1;
    self->mBankIndex = -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_findBank);
#ifdef SKIP_ASM
/**
 * Finds a Pathfinder bank record (D_004D3EF8, 24 records of 0x28 bytes).
 *
 * With bank < 0 it returns the first free record (word +0x8 < 0). Otherwise it returns the first record whose
 * player-slot word (+0x8) equals bank and, unless id < 0, whose bank-index word (+0x0) equals id.
 *
 * @param bank  the player-slot index to match (record +0x8), or < 0 to find a free record
 * @param id  the bank index within that slot (record +0x0), or < 0 for any
 * @return the record index, or -8 when none matches
 */
extern "C" int pathxSND_findBank(int bank, int id)
{
    tPathBank* record = D_004D3EF8;
    for (int i = 0; i < 24; i++, record++) {
        if (bank < 0) {
            if (record->mPlayer < 0) {
                // the do/while(0) wrapper (a compiled-out macro block) is needed to match register allocation
                do {
                    return i;
                } while (0);
            }
        } else if (record->mPlayer == bank) {
            if (id < 0 || record->mBankIndex == id)
                return i;
        }
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE290);
#ifdef SKIP_ASM
/**
 * Returns 0 (a stub; no caller found).
 *
 * @return 0
 */
extern "C" int func_002AE290(void) {
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", pathxSND_addBank);
#ifdef SKIP_ASM
extern "C" int pathxSND_findBank(int a, int b);

/**
 * Registers a loaded bank for a Pathfinder player slot: fills the slot's record for that bank, or a free record.
 *
 * Looks up the record with pathxSND_findBank(a, b), else a free one; marks it used (+0x1C = 1) and stores the slot (+0x8),
 * the bank index (+0x0) and the bank handle (+0x4). Called by the Pathfinder library (PATH_requestPart), which first
 * evicts the oldest bank (pathxSND_evictOldestBank) when memory runs short.
 *
 * @param slotIndex  the player-slot index
 * @param bankName  the bank's name (passed by the library, not used)
 * @param bankIndex  the bank index within the slot
 * @param bankHandle  the loaded bank handle
 * @return c, or -13 when no record is free
 */
extern "C" int pathxSND_addBank(int slotIndex, int bankName, int bankIndex, int bankHandle)
{
    int i = pathxSND_findBank(slotIndex, bankIndex);
    if (i >= 0 || (i = pathxSND_findBank(-1, -1)) >= 0) {
        tPathBank* record = &D_004D3EF8[i];
        record->mUsed = 1;
        record->mPlayer = slotIndex;
        record->mBankIndex = bankIndex;
        record->mHandle = bankHandle;
        return bankHandle;
    }
    return -13;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", pathxSND_removeBank);
#ifdef SKIP_ASM
extern "C" int pathxSND_findBank(int a, int b);
extern "C" int SNDbankremove(int id);

/**
 * Unregisters a bank of a Pathfinder player slot: clears its record (D_004D3EF8) when it holds a bank handle.
 *
 * When the record's handle (+0x4) is set it calls SNDbankremove(-1) and resets the record (+0x14 = 0, +0x4, +0x8, +0x0,
 * +0x24 = -1, +0x1C = 0). Quirk: it passes -1 to SNDbankremove, not the record's handle. Called by pathxSND_evictOldestBank and
 * the Pathfinder library (PATH_releaseRequest).
 *
 * @param slotIndex  the player-slot index
 * @param bankIndex  the bank index within the slot
 * @return 0, or -1 when no record matches
 */
extern "C" int pathxSND_removeBank(int slotIndex, int bankIndex)
{
    int none = -1;
    int i = pathxSND_findBank(slotIndex, bankIndex);
    if (i < 0) {
        return -1;
    }
    tPathBank* record = &D_004D3EF8[i];
    if (record->mHandle >= 0) {
        SNDbankremove(-1);
        record->mMemory = 0;
        record->mHandle = none;
        record->mPlayer = none;
        record->mBankIndex = none;
        record->mUsed = 0;
        record->mLastPlayTime = none;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", pathxSND_evictOldestBank);
#ifdef SKIP_ASM
extern "C" int pathxSND_findBank(int a, int b);
extern "C" int pathxSND_removeBank(int a, int b);

// pathxSND_evictOldestBank's best index before any bank with a play time is found (name ours).
enum { kNoBankFound = -1 };

/**
 * Evicts the least recently played bank of a Pathfinder player slot.
 *
 * Walks bank indices 0, 1, .. while the slot has a record for them (pathxSND_findBank) and picks the one with the smallest
 * non-negative play time (+0x24, set by pathxSND_play from pathxSND_getTimeMs), then unregisters it with pathxSND_removeBank.
 * Called by the Pathfinder library (PATH_requestPart) before it registers a new bank.
 *
 * @param slotIndex  the player-slot index
 * @return 0 when no bank has a play time, otherwise pathxSND_removeBank's result
 */
extern "C" int pathxSND_evictOldestBank(int slotIndex)
{
    int best = 0x7FFFFFF;
    int bestIdx = kNoBankFound;
    int i = 0;
    tPathBank* tbl = D_004D3EF8;
    int id;
    while (i < 24 && (id = pathxSND_findBank(slotIndex, i)) >= 0) {
        int playTime = tbl[id].mLastPlayTime;
        if (playTime >= 0 && playTime < best) {
            best = playTime;
            bestIdx = i;
        }
        i++;
    }
    if (bestIdx == kNoBankFound) {
        return 0;
    }
    return pathxSND_removeBank(slotIndex, bestIdx);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_getPlayerSize);
#ifdef SKIP_ASM
extern "C" int SNDSTRM_overhead(int n, int m);

/**
 * Returns the memory a Pathfinder player slot needs: 0x70 + 12 per request, plus SNDSTRM_overhead for a stream.
 *
 * The stream overhead (SNDSTRM_overhead(n, m)) is added only when m > 0. The total is then rounded up to a multiple of
 * 16, always adding 1..16 bytes (an already aligned size grows by 16). Called by the Pathfinder library
 * (PATH_getPlayerMemSize).
 *
 * @param n  the number of queued requests
 * @param maxChunks  the packet-player chunk count (0 for a bank-only slot)
 * @return the size in bytes
 */
extern "C" int pathxSND_getPlayerSize(int n, int maxChunks)
{
    int size = n * 0xC + 0x70;
    if (maxChunks > 0) {
        size += SNDSTRM_overhead(n, maxChunks);
    }
    return size + (0x10 - (size & 0xF));
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_openPlayer);
#ifdef SKIP_ASM
extern "C" int pathxSND_findFreePlayer(void);
extern "C" void pathxSND_initPlayer(tPathPlayer* self);
extern "C" int SNDSTRM_create(void* desc, int n, int m, void* mem, int size);
extern "C" int SNDplaysetdef(void* desc);
extern char str_pct_s_c[];
extern "C" int sprintf(char* buf, const char* fmt, ...);

/**
 * Opens a Pathfinder player slot in the given memory: a sound stream, or a bank player when m is 0.
 *
 * Claims a free slot (pathxSND_findFreePlayer; -13 when none), points D_004D3E98[slot] at mem, initialises the 0x70-byte header
 * (pathxSND_initPlayer) and places the request table (count x 12, handles -1) after it, 16-aligned. Copies desc (24 bytes of
 * play options) to +0x30, or SNDplaysetdef's defaults, clears byte +0x35 and stores "%s" of name (11 chars) at +0x1C.
 * For m > 0 it creates the stream with SNDSTRM_create(+0x30, count, m, rest of mem) into +0x0; otherwise it sets byte
 * +0x64 (bank player). Ends with the request count in byte +0x66, +0x14 = 127 and +0x10 = 0. Called by the Pathfinder
 * library (func_003D25F0).
 *
 * @param name  the player name (11 characters kept)
 * @param desc  play options to copy, or 0 for the defaults
 * @param count  the number of queued requests
 * @param maxChunks  the packet-player chunk count; 0 makes a bank player
 * @param mem  the memory for the slot (pathxSND_getPlayerSize bytes)
 * @param size  its size in bytes
 * @return the slot index, -13 when none is free, or SNDSTRM_create's error
 */
extern "C" int pathxSND_openPlayer(char* name, SNDPLAYOPTS* desc, int count, int maxChunks, char* mem, int size)
{
    int slot = pathxSND_findFreePlayer();
    if (slot < 0)
        return -13;
    D_004D3E98[slot] = (tPathPlayer*)mem;
    mem += sizeof(tPathPlayer);
    pathxSND_initPlayer(D_004D3E98[slot]);
    tPathPlayer* player = D_004D3E98[slot];
    player->mRequests = (tPathRequest*)mem;
    mem = count * sizeof(tPathRequest) + mem;
    int off = mem - (char*)player;
    mem = mem + (0x10 - off % 16);
    size -= mem - (char*)player;
    int i;
    for (i = 0; i < count; i++)
        player->mRequests[i].mHandle = -1;
    if (desc)
        player->mPlayOpts = *desc;
    else
        SNDplaysetdef(&player->mPlayOpts);
    player->mPlayOpts.fxlevel0 = 0;
    sprintf(player->mName, str_pct_s_c, 11, name);
    if (maxChunks > 0)
        player->mHandle = SNDSTRM_create(&player->mPlayOpts, count, maxChunks, mem, size);
    else
        player->mIsBankPlayer = 1;
    if (player->mHandle < 0 && player->mIsBankPlayer == 0)
        return player->mHandle;
    player->mRequestCount = count;
    player->mDryLevel = 0x7F;
    player->mFxLevel = 0;
    return slot;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", pathxSND_closePlayer);
#ifdef SKIP_ASM
extern void* (*D_004A3794)(void*);
extern "C" void SNDSTRM_destroy(int id);

/**
 * Closes a Pathfinder player slot: frees a bank player's banks or destroys the stream, then forgets the slot.
 *
 * For a bank player (byte +0x64) it walks the slot's bank records (pathxSND_findBank(idx, -1)), passes each record's memory
 * (+0x14) to the free hook D_004A3794 and clears the record (pathxSND_clearBank); for a stream it calls SNDSTRM_destroy on
 * its handle. Then clears the slot pointer (pathxSND_forgetPlayer). Called by the Pathfinder library (PATH_closeVoice).
 *
 * @param idx  the player-slot index
 */
extern "C" void pathxSND_closePlayer(int idx)
{
    tPathPlayer* player = D_004D3E98[idx];
    if (player->mIsBankPlayer != 0) {
        int id = pathxSND_findBank(idx, -1);
        while (id >= 0) {
            tPathBank* record = &D_004D3EF8[id];
            if (D_004A3794 != 0 && record->mMemory != 0) {
                D_004A3794(record->mMemory);
            }
            pathxSND_clearBank(record);
            id = pathxSND_findBank(idx, -1);
        }
    } else {
        SNDSTRM_destroy(player->mHandle);
    }
    pathxSND_forgetPlayer(player);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_setPreload);
#ifdef SKIP_ASM
/**
 * Sets a stream player slot's preloaded data: slot +0x28 = a1 and +0x2C = a2.
 *
 * Only for an open stream slot (index < 24, not a bank player). pathxSND_play then queues offsets below that size from
 * memory (SNDSTRM_queueMemory) instead of the file. Called by the Pathfinder library (PATH_setPreload).
 *
 * @param idx  the player-slot index
 * @param preloadData  the preloaded data
 * @param preloadSize  its size in bytes
 */
extern "C" void pathxSND_setPreload(unsigned int idx, int preloadData, int preloadSize)
{
    if (idx < 24) {
        tPathPlayer* player = D_004D3E98[idx];
        if (player != 0 && player->mIsBankPlayer == 0) {
            player->mPreloadData = preloadData;
            player->mPreloadSize = preloadSize;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_play);
#ifdef SKIP_ASM
extern "C" int pathxSND_findBank(int a, int b);
// PORT: the unit defines pathxSND_getTimeMs as void; its body returns __fixunssfsi's result.
unsigned int pathxSND_getTimeMs_r(void) __asm__("pathxSND_getTimeMs");
extern "C" int pathxSND_setVolume(int idx, int v);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int SNDBANK_play(int h, int id, void* desc);
extern "C" int SNDSTRM_queuefile(int h, int a, int b, unsigned int t);
// PORT: the unit declares SNDSTRM_purge as void; the caller leaves $v0 live (int return).
int SNDSTRM_purge_r(int) __asm__("SNDSTRM_purge");
extern "C" int SNDSTRM_queueMemory(int h, int a, int b, unsigned int t);

/**
 * Plays a sound on a Pathfinder player slot: a bank patch on a bank player, or a queued file section on a stream.
 *
 * A vol >= 0 is stored in the slot's play options (+0x30) and applied (pathxSND_setVolume). Bank player: key's high 16 bits
 * select the bank (record found with pathxSND_findBank; -1 when it has no handle), the patch is (key & 0xFFFF) + a6 - 1 (a6
 * <= -1 counts as 0); SNDBANK_play inside the critical section, the record's play time (+0x24) becomes pathxSND_getTimeMs()
 * and the slot's handle the bank's; request 0 gets the sound handle and a4.
 *
 * Stream: the file offset is key * 128. With a7 set it purges the stream (SNDSTRM_purge) and clears the older requests.
 * Offsets inside the preloaded data (+0x28/+0x2C, pathxSND_setPreload) are queued from memory (SNDSTRM_queueMemory), others with
 * SNDSTRM_queuefile(stream, a1, a2, offset); on success the request table shifts down one and request 0 gets the new
 * handle and a4. Both paths set the dirty byte (+0x65). Called by the Pathfinder library (func_003D41A8).
 *
 * @param idx  the player-slot index
 * @param holdTime  the hold time for a queued file (ms; -1 holds)
 * @param fileName  the file name to queue
 * @param key  bank << 16 | patch for a bank player, or the file offset in 128-byte units for a stream
 * @param requestLength  the request's length, kept in its table entry (+0x0)
 * @param vol  a new volume, or < 0 to keep it
 * @param patchOffset  added to the patch number (bank player)
 * @param purgeFirst  nonzero to purge the stream first
 * @return the sound or request handle, or a negative error
 */
extern "C" int pathxSND_play(int idx, int holdTime, int fileName, unsigned int key, int requestLength, signed char vol, int patchOffset, int purgeFirst)
{
    int bankHandle = -1;
    // NOTE: retail rematerialises `li v0,1` right before each 0x65 store (after unrelated loads):
    // a function-scope local set early whose pseudo gets no callee-saved register (all nine are taken),
    // so reload re-creates the constant at each use.
    signed char dirty = 1;
    tPathPlayer* player = D_004D3E98[idx];
    if (vol >= 0)
    {
        player->mPlayOpts.vol = vol;
        pathxSND_setVolume(idx, vol);
    }
    int playHandle;
    if (player->mIsBankPlayer != 0)
    {
        if (patchOffset <= bankHandle)
            patchOffset = 0;
        int bankRecord = pathxSND_findBank(idx, key >> 16);
        if (bankRecord >= 0)
            bankHandle = D_004D3EF8[bankRecord].mHandle;
        if (bankHandle < 0)
            return -1;
        player->mPatch = (key & 0xFFFF) + patchOffset - 1;
        SNDSYS_entercritical();
        playHandle = SNDBANK_play(bankHandle, player->mPatch, &player->mPlayOpts);
        SNDSYS_leavecritical();
        D_004D3EF8[bankRecord].mLastPlayTime = pathxSND_getTimeMs_r();
        player->mHandle = bankHandle;
        player->mRequests[0].mHandle = playHandle;
        player->mRequests[0].mLength = requestLength;
        player->mDirty = dirty;
    }
    else
    {
        unsigned int fileOffset = key << 7;
        int j;
        if (purgeFirst)
        {
            SNDSTRM_purge_r(player->mHandle);
            for (j = player->mRequestCount - 1; j > 0; j--)
            {
                player->mRequests[j - 1].mHandle = -1;
                player->mRequests[j - 1].mLength = -1;
            }
        }
        if (player->mPreloadData && fileOffset < player->mPreloadSize)
            playHandle = SNDSTRM_queueMemory(player->mHandle, holdTime, player->mPreloadData, fileOffset);
        else
            playHandle = SNDSTRM_queuefile(player->mHandle, holdTime, fileName, fileOffset);
        if (playHandle >= 0)
        {
            for (j = player->mRequestCount - 1; j > 0; j--)
                player->mRequests[j] = player->mRequests[j - 1];
            player->mRequests[0].mHandle = playHandle;
            player->mRequests[0].mLength = requestLength;
        }
        player->mDirty = dirty;
    }
    return playHandle;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_releaseHold);
#ifdef SKIP_ASM
extern "C" void SNDSTRM_modifyhold(int requestHandle);

/**
 * Releases the newest queued request of a stream player slot (SNDSTRM_modifyhold on request 0's handle).
 *
 * Nothing for a bank player. Declared with one parameter, but the hold time passes through in the second argument
 * register (the Pathfinder library, PATHI_serviceVoices, calls it with (slot, 0), which starts playback).
 *
 * @param i  the player-slot index
 */
extern "C" void pathxSND_releaseHold(int i)
{
    tPathPlayer* player = D_004D3E98[i];
    if (player->mIsBankPlayer == 0) {
        SNDSTRM_modifyhold(player->mRequests[0].mHandle);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_getPlayed);
#ifdef SKIP_ASM
extern "C" void pathxSND_refreshAll(int a0);

/**
 * Returns a Pathfinder player slot's +0x50 status word, refreshing every slot's status first when this one is dirty.
 *
 * The refresh is pathxSND_refreshAll(0) (slot byte +0x65 set). No caller found.
 *
 * @param idx  the player-slot index
 * @return slot +0x50 (the played amount the status refresh sums)
 */
extern "C" int pathxSND_getPlayed(int idx)
{
    tPathPlayer* player = D_004D3E98[idx];
    if (player != 0) {
        if (player->mDirty != 0) {
            pathxSND_refreshAll(0);
        }
    }
    return D_004D3E98[idx]->mBuffered;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_getRequestPosition);
#ifdef SKIP_ASM
extern "C" void pathxSND_refreshAll(int a0);

/**
 * Returns a request's progress on a Pathfinder player slot, refreshing the slots' status first when it is dirty.
 *
 * With sub < 0 it returns slot +0x54 (the total the refresh adds up); otherwise request sub's position (table entry
 * +0x8) when that request has a handle. Called by the Pathfinder library (func_003D41A8, PATHI_getQueuedAmount).
 *
 * @param idx  the player-slot index
 * @param sub  the request index, or < 0 for the slot total
 * @return the position, or -8 for an unknown or finished request
 */
extern "C" int pathxSND_getRequestPosition(int idx, int sub)
{
    tPathPlayer* player = D_004D3E98[idx];
    if (player != 0 && player->mDirty != 0) {
        pathxSND_refreshAll(0);
    }
    if (sub < 0) {
        return player->mPositionTotal;
    }
    if (sub < player->mRequestCount) {
        if (player->mRequests[sub].mHandle >= 0) {
            return player->mRequests[sub].mPosition;
        }
        return -8;
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_getHandlePosition);
#ifdef SKIP_ASM
extern "C" void pathxSND_refreshAll(int a0);

/**
 * Returns the position of the request with the given handle on a Pathfinder player slot (-8 when not queued).
 *
 * Refreshes every slot's status first when this one is dirty (pathxSND_refreshAll(0)), then searches the request table for
 * handle key (entry +0x4) and returns its position (+0x8). No caller found.
 *
 * @param idx  the player-slot index
 * @param key  the request handle
 * @return the position, or -8
 */
extern "C" int pathxSND_getHandlePosition(int idx, int key)
{
    tPathPlayer* player = D_004D3E98[idx];
    if (player != 0 && player->mDirty != 0) {
        pathxSND_refreshAll(0);
    }
    for (int i = 0; i < player->mRequestCount; i++) {
        tPathRequest* request = &player->mRequests[i];
        if (request->mHandle == key) {
            return request->mPosition;
        }
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_isBusy);
#ifdef SKIP_ASM
extern "C" void pathxSND_refreshAll(int a0);

/**
 * Returns whether a Pathfinder player slot is still busy (+0x58), refreshing the slots' status first when dirty.
 *
 * Called by the Pathfinder library (PATHI_serviceVoices).
 *
 * @param idx  the player-slot index
 * @return slot +0x58, or -8 for an unused slot
 */
extern "C" int pathxSND_isBusy(int idx)
{
    if (D_004D3E98[idx] != 0) {
        if (D_004D3E98[idx]->mDirty != 0) {
            pathxSND_refreshAll(0);
        }
        return D_004D3E98[idx]->mBusy;
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_markDirty);
#ifdef SKIP_ASM
/**
 * Marks a Pathfinder player slot dirty (+0x65 = 1) so the next query refreshes its status.
 *
 * Called by the Pathfinder library (func_003CFC30).
 *
 * @param idx  the player-slot index
 * @return 1, or -8 for an unused slot
 */
extern "C" int pathxSND_markDirty(int idx)
{
    tPathPlayer* player = D_004D3E98[idx];
    if (player == 0) {
        return -8;
    }
    player->mDirty = 1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_refreshPlayer);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int SNDBANK_readPatchHeader(int h, int sub, void* hdr, void* buf, unsigned int* pos);
extern "C" int SNDtimeremaining(int h);
extern "C" int SND_getVolume(int h);
extern "C" int SNDSTRM_status(int h, void* out);
// PORT: the unit declares SNDSTRM_requeststatus(int, int); the second argument is an out-pointer
extern "C" void SNDSTRM_requeststatus(int h, void* status);
extern "C" int SNDSTRM_getVolume(int h);

struct sStrmInfo_EC58 {
    int count;
    int pad[3];
};

struct sStrmState_EC58 {
    int state;  // 0x0
    int a;      // 0x4
    int b;      // 0x8
    int c;      // 0xC
};

struct sStrmPos_EC58 {
    unsigned int samples;   // 0x0
    int pad[7];
};

struct sStrmHdr_EC58 {
    unsigned short rate;
    char pad[0xE];
};

/**
 * Refreshes one Pathfinder player slot's status and returns its play position through the two out-pointers.
 *
 * Same work as one iteration of pathxSND_refreshAll: a bank player reads the patch's sample position (SNDBANK_readPatchHeader) and the
 * time left (SNDtimeremaining x 10 ms); a stream reads SNDSTRM_status and each request's SNDSTRM_requeststatus, summing
 * positions into +0x54 and +0x50, updating busy (+0x58), the current handle (+0x5C) and the current volume (+0xC). Sets
 * +0x58 to 1 first, returns -1 for an unused slot and -2 for one without a handle.
 *
 * Likely bugs: in the stream path it tests the out-pointers themselves (outA == 0 && outB == 0) where pathxSND_refreshAll
 * tests the values, and at the end it stores the pointers, not the values, into +0x48/+0x4C. Called by the Pathfinder
 * library (PATHI_getVoiceSegment).
 *
 * @param idx  the player-slot index
 * @param outA  receives the elapsed time (ms)
 * @param outB  receives the remaining time or request position
 * @return 0, or -1 / -2 as above
 */
// PORT: the end of this function stores the out-pointers themselves into the slot (as in the original)
extern "C" int pathxSND_refreshPlayer(int idx, int* outA, int* outB)
{
    tPathPlayer* player = D_004D3E98[idx];
    *outA = 0;
    *outB = 0;
    if (player == 0) {
        return -1;
    }
    int statusResult = -8;
    player->mBusy = 1;
    if (player->mHandle < 0) {
        return -2;
    }
    player->mDirty = 1;
    SNDSYS_entercritical();
    sStrmInfo_EC58 info;
    sStrmState_EC58 st;
    sStrmHdr_EC58 hdr;
    char buf[0x70];
    sStrmPos_EC58 pos;
    if (player->mIsBankPlayer != 0) {
        if (player->mPatch >= 0 && player->mRequests->mHandle >= 0) {
            statusResult = SNDBANK_readPatchHeader(player->mHandle, player->mPatch, &hdr, buf, &pos.samples);
            if (statusResult >= 0) {
                *outB = SNDtimeremaining(player->mRequests->mHandle);
                if (*outB < 0) {
                    *outB = 0;
                    player->mRequests->mHandle = -1;
                    goto done;
                }
                *outB *= 10;
                *outA = (int)((float)pos.samples / (float)hdr.rate * 1000.0f) - *outB;
            }
            player->mRequests->mPosition = *outB;
            player->mBuffered = player->mRequests->mLength;
            player->mVolume = SND_getVolume(player->mRequests->mHandle);
            player->mCurrentRequest = player->mRequests->mHandle;
        }
    } else {
        statusResult = SNDSTRM_status(player->mHandle, &info);
        player->mBuffered = 0;
        if (info.count >= player->mRequestCount) {
            player->mBusy = 0;
        }
        for (int j = 0; j < player->mRequestCount; j++) {
            player->mRequests[j].mPosition = 0;
            if (player->mRequests[j].mHandle < 0) {
                continue;
            }
            SNDSTRM_requeststatus(player->mRequests[j].mHandle, &st);
            switch (st.state) {
            case kSNDREQUEST_Queued:
                player->mRequests[j].mPosition = player->mRequests[j].mLength;
                player->mPositionTotal += player->mRequests[j].mLength;
                player->mBuffered += st.c;
                player->mBusy = 0;
                player->mCurrentRequest = player->mRequests[j].mHandle;
                break;
            case kSNDREQUEST_NotParsed:
            case kSNDREQUEST_Playing:
                *outA = st.a;
                *outB = st.b;
                if (outA == 0 && outB == 0) {
                    *outA = 0;
                    *outB = player->mRequests[j].mLength;
                }
                player->mRequests[j].mPosition = *outB;
                player->mPositionTotal += *outB;
                player->mBuffered += st.c;
                if (st.c < st.b) {
                    player->mBusy = 0;
                }
                player->mCurrentRequest = player->mRequests[j].mHandle;
                break;
            default:
                player->mRequests[j].mHandle = -1;
                break;
            }
        }
        player->mVolume = SNDSTRM_getVolume(player->mHandle);
    }
done:
    player->mElapsedMs = (int)outA;
    player->mRemainingMs = (int)outB;
    player->mStatus = statusResult;
    player->mDirty = 0;
    SNDSYS_leavecritical();
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_refreshAll);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int SNDBANK_readPatchHeader(int h, int sub, void* hdr, void* buf, unsigned int* pos);
extern "C" int SNDtimeremaining(int h);
extern "C" int SND_getVolume(int h);
extern "C" int SNDSTRM_status(int h, void* out);
// PORT: the unit declares SNDSTRM_requeststatus(int, int); the second argument is an out-pointer
extern "C" void SNDSTRM_requeststatus(int h, void* status);
extern "C" int SNDSTRM_getVolume(int h);

struct sStrmInfo_EFA8 {
    int count;
    int pad[3];
};

struct sStrmState_EFA8 {
    int state;  // 0x0
    int a;      // 0x4
    int b;      // 0x8
    int c;      // 0xC
};

struct sStrmPos_EFA8 {
    unsigned int samples;   // 0x0
    int pad[7];
};

struct sStrmHdr_EFA8 {
    unsigned short rate;
    char pad[0xE];
};

/**
 * Refreshes the status of all 24 Pathfinder player slots (or, with reset set, just marks them dirty).
 *
 * Inside the critical section. With reset != 0 each open slot gets +0x60 = 0 and the dirty byte. Otherwise for each
 * slot: a bank player reads its patch's sample position (SNDBANK_readPatchHeader) and remaining time (SNDtimeremaining x 10 ms),
 * giving the elapsed ms (+0x48) and remaining ms (+0x4C), and its volume (+0xC, SND_getVolume); a stream reads
 * SNDSTRM_status and each request's SNDSTRM_requeststatus, adding finished and playing requests into +0x54 / +0x50,
 * dropping unknown ones, and records the current volume (+0xC, SNDSTRM_getVolume). Clears busy (+0x58) when nothing is
 * pending, stores the status result in +0x60 and clears the dirty byte (+0x65).
 *
 * The slot queries (pathxSND_getPlayed, pathxSND_getRequestPosition, pathxSND_getHandlePosition, pathxSND_isBusy, pathxSND_getStatus, pathxSND_getVolume,
 * pathxSND_setVolume) call it with 0 when their slot is dirty; the Pathfinder library calls it too (PATHI_serviceTick).
 *
 * @param reset  nonzero to only mark the slots dirty
 */
extern "C" void pathxSND_refreshAll(int reset)
{
    SNDSYS_entercritical();
    for (int i = 0; i < 24; i++) {
        tPathPlayer* player = D_004D3E98[i];
        if (player == 0) {
            continue;
        }
        if (reset != 0) {
            player->mStatus = 0;
            player->mDirty = 1;
            continue;
        }
        int statusResult = -8;
        int elapsedMs = 0;
        int remainingMs = 0;
        player->mBusy = 1;
        if (player->mHandle >= 0) {
            sStrmInfo_EFA8 info;
            sStrmState_EFA8 st;
            sStrmHdr_EFA8 hdr;
            char buf[0x70];
            sStrmPos_EFA8 pos;
            if (player->mIsBankPlayer != 0) {
                if (player->mPatch >= 0 && player->mRequests->mHandle >= 0) {
                    statusResult = SNDBANK_readPatchHeader(player->mHandle, player->mPatch, &hdr, buf, &pos.samples);
                    if (statusResult >= 0) {
                        remainingMs = SNDtimeremaining(player->mRequests->mHandle);
                        if (remainingMs < 0) {
                            remainingMs = 0;
                            player->mRequests->mHandle = -1;
                            goto done;
                        }
                        remainingMs *= 10;
                        elapsedMs = (int)((float)pos.samples / (float)hdr.rate * 1000.0f) - remainingMs;
                    }
                    player->mRequests->mPosition = remainingMs;
                    player->mBuffered = player->mRequests->mLength;
                    player->mVolume = SND_getVolume(player->mRequests->mHandle);
                    player->mCurrentRequest = player->mRequests->mHandle;
                }
            } else {
                statusResult = SNDSTRM_status(player->mHandle, &info);
                player->mBuffered = 0;
                if (info.count >= player->mRequestCount) {
                    player->mBusy = 0;
                }
                for (int j = 0; j < player->mRequestCount; j++) {
                    player->mRequests[j].mPosition = 0;
                    if (player->mRequests[j].mHandle < 0) {
                        continue;
                    }
                    SNDSTRM_requeststatus(player->mRequests[j].mHandle, &st);
                    switch (st.state) {
                    case kSNDREQUEST_Queued:
                        player->mRequests[j].mPosition = player->mRequests[j].mLength;
                        player->mPositionTotal += player->mRequests[j].mLength;
                        player->mBuffered += st.c;
                        player->mBusy = 0;
                        player->mCurrentRequest = player->mRequests[j].mHandle;
                        break;
                    case kSNDREQUEST_NotParsed:
                    case kSNDREQUEST_Playing:
                        elapsedMs = st.a;
                        remainingMs = st.b;
                        if (elapsedMs == 0 && remainingMs == 0) {
                            elapsedMs = remainingMs;
                            remainingMs = player->mRequests[j].mLength;
                        }
                        player->mRequests[j].mPosition = remainingMs;
                        player->mPositionTotal += remainingMs;
                        player->mBuffered += st.c;
                        if (st.c < st.b) {
                            player->mBusy = 0;
                        }
                        player->mCurrentRequest = player->mRequests[j].mHandle;
                        break;
                    default:
                        player->mRequests[j].mHandle = -1;
                        break;
                    }
                }
                player->mVolume = SNDSTRM_getVolume(player->mHandle);
            }
        }
    done:
        player->mElapsedMs = elapsedMs;
        player->mRemainingMs = remainingMs;
        player->mStatus = statusResult;
        player->mDirty = 0;
    }
    SNDSYS_leavecritical();
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_getStatus);
#ifdef SKIP_ASM
extern "C" void pathxSND_refreshAll(int a0);

/**
 * Returns a Pathfinder player slot's play position and status, refreshing the slots first when it is dirty.
 *
 * Called by the Pathfinder library (PATH_status).
 *
 * @param idx  the player-slot index
 * @param elapsedOut  receives the elapsed time (slot +0x48)
 * @param remainingOut  receives the remaining time (slot +0x4C)
 * @return the status result (+0x60) when nonzero, else the current handle (+0x5C); -8 for an unused slot
 */
extern "C" int pathxSND_getStatus(int idx, int* elapsedOut, int* remainingOut)
{
    tPathPlayer* player = D_004D3E98[idx];
    if (player == 0) {
        return -8;
    }
    if (player->mDirty != 0) {
        pathxSND_refreshAll(0);
    }
    *elapsedOut = player->mElapsedMs;
    *remainingOut = player->mRemainingMs;
    if (player->mStatus != 0) {
        return player->mStatus;
    }
    return player->mCurrentRequest;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_getTimeMs);
#ifdef SKIP_ASM
extern "C" int TIMER_gettick();
extern "C" void fptoui(float);
extern int TIMERhz[];

/**
 * Returns the current time in ms: TIMER_gettick() / TIMERhz[0] * 1000, converted to unsigned int.
 *
 * Declared void, but the __fixunssfsi result (fptoui) is returned in v0; callers use it (pathxSND_play stamps
 * bank play times with it, and the Pathfinder library).
 */
extern "C" void pathxSND_getTimeMs(void)
{
    fptoui((float)TIMER_gettick() / (float)TIMERhz[0] * 1000.0f);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_getBankHandle);
#ifdef SKIP_ASM
/**
 * Returns a bank player slot's current bank handle (slot +0x0), or -1 for an unused slot or a stream.
 *
 * @param slotIndex  the player-slot index
 * @return the handle, or -1
 */
extern "C" int pathxSND_getBankHandle(int slotIndex) {
    tPathPlayer* player = D_004D3E98[slotIndex];
    if (player != 0 && player->mIsBankPlayer != 0) {
        return player->mHandle;
    }
    return -1;
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_getStreamHandle);
#ifdef SKIP_ASM
/**
 * Returns a stream player slot's stream handle (slot +0x0), or -1 for an unused slot or a bank player.
 *
 * @param slotIndex  the player-slot index
 * @return the handle, or -1
 */
extern "C" int pathxSND_getStreamHandle(int slotIndex) {
    tPathPlayer* player = D_004D3E98[slotIndex];
    if (player != 0 && player->mIsBankPlayer == 0) {
        return player->mHandle;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_getVolume);
#ifdef SKIP_ASM
extern "C" void pathxSND_refreshAll(int a0);

/**
 * Returns a Pathfinder player slot's current volume (+0xC), refreshing the slots' status first when it is dirty.
 *
 * Called by the Pathfinder library (func_003D41A8, PATHI_serviceVoices, PATHI_settleFades, PATHI_startVolumeRamp).
 *
 * @param idx  the player-slot index
 * @return the volume, or -8 for an unused slot
 */
extern "C" int pathxSND_getVolume(int idx)
{
    if (D_004D3E98[idx] != 0) {
        if (D_004D3E98[idx]->mDirty != 0) {
            pathxSND_refreshAll(0);
        }
        return D_004D3E98[idx]->mVolume;
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_getDryLevel);
#ifdef SKIP_ASM
/**
 * Returns a Pathfinder player slot's dry level (+0x14, set by pathxSND_setDryLevel), or -8 for an unused slot.
 *
 * @param idx  the player-slot index
 * @return the dry level, or -8
 */
extern "C" int pathxSND_getDryLevel(int idx)
{
    tPathPlayer* player = D_004D3E98[idx];
    if (player != 0) {
        return player->mDryLevel;
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_getFxLevel);
#ifdef SKIP_ASM
/**
 * Returns a Pathfinder player slot's effect send level (+0x10, set by pathxSND_setFxLevel), or -8 for an unused slot.
 *
 * @param idx  the player-slot index
 * @return the effect level, or -8
 */
extern "C" int pathxSND_getFxLevel(int idx)
{
    tPathPlayer* player = D_004D3E98[idx];
    if (player != 0) {
        return player->mFxLevel;
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_setFxLevel);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int SNDSTRM_setFxLevel(int h, int a, int b);
extern "C" int SND_setFxLevel(int h, int a, int b);

/**
 * Sets a Pathfinder player slot's effect send level: SND_setFxLevel for a bank player, SNDSTRM_setFxLevel for a stream.
 *
 * Runs inside the critical section and records the level in slot +0x10. Called by the Pathfinder library
 * (PATHI_stepSendRamp) with bus 0.
 *
 * @param idx  the player-slot index
 * @param effectBus  the effect bus
 * @param sendLevel  the send level
 * @return the driver call's result, or -8 for an unused slot
 */
extern "C" int pathxSND_setFxLevel(int idx, int effectBus, int sendLevel)
{
    tPathPlayer* player = D_004D3E98[idx];
    int result = -8;
    if (player != 0) {
        SNDSYS_entercritical();
        if (player->mIsBankPlayer != 0) {
            result = SND_setFxLevel(player->mHandle, effectBus, sendLevel);
        } else {
            result = SNDSTRM_setFxLevel(player->mHandle, effectBus, sendLevel);
        }
        SNDSYS_leavecritical();
        player->mFxLevel = sendLevel;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_setDryLevel);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int SNDSTRM_setDryLevel(int h, int a);
extern "C" int SND_setDryLevel(int h, int a);

/**
 * Sets a Pathfinder player slot's dry level: SND_setDryLevel for a bank player, SNDSTRM_setDryLevel for a stream.
 *
 * Runs inside the critical section and records the level in slot +0x14. Called by the Pathfinder library
 * (PATHI_stepDryRamp).
 *
 * @param idx  the player-slot index
 * @param dryLevel  the dry level
 * @return the driver call's result, or -8 for an unused slot
 */
extern "C" int pathxSND_setDryLevel(int idx, int dryLevel)
{
    tPathPlayer* player = D_004D3E98[idx];
    int result = -8;
    if (player != 0) {
        SNDSYS_entercritical();
        if (player->mIsBankPlayer != 0) {
            result = SND_setDryLevel(player->mHandle, dryLevel);
        } else {
            result = SNDSTRM_setDryLevel(player->mHandle, dryLevel);
        }
        SNDSYS_leavecritical();
        player->mDryLevel = dryLevel;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_fadeVolume);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int SNDautovol(int h, int v, int c);
// PORT: the unit declares SNDSTRM_fadeVolume as void; this caller uses its result.
int SNDSTRM_fadeVolume_r(int h, int v, int c) __asm__("SNDSTRM_fadeVolume");

/**
 * Starts a volume fade on a Pathfinder player slot unless busy is set.
 *
 * Inside the critical section it records the target in slot +0x8 and fades a bank player's sound with
 * SNDautovol(handle, ms / 10 ticks, target) or a stream with SNDSTRM_fadeVolume(stream, ms, target). Called by the
 * Pathfinder library (PATHI_startVolumeFade).
 *
 * @param idx  the player-slot index
 * @param targetVolume  the target volume
 * @param fadeMs  the fade time in ms
 * @param busy  nonzero to do nothing
 * @return the driver call's result, or 0
 */
extern "C" int pathxSND_fadeVolume(int idx, int targetVolume, int fadeMs, char busy)
{
    tPathPlayer* player = D_004D3E98[idx];
    int driverResult = 0;
    if (busy == 0) {
        SNDSYS_entercritical();
        player->mFadeTarget = targetVolume;
        if (player->mIsBankPlayer != 0) {
            driverResult = SNDautovol(player->mRequests[0].mHandle, fadeMs / 10, targetVolume);
        } else {
            driverResult = SNDSTRM_fadeVolume_r(player->mHandle, fadeMs, targetVolume);
        }
        SNDSYS_leavecritical();
    }
    return driverResult;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_setVolume);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void pathxSND_refreshAll(int a0);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDvol(int h, int v);
extern "C" void SNDSTRM_setVolume(int h, int v);

/**
 * Sets a Pathfinder player slot's volume when no fade is pending; only while the audio system's
 * mPathfinderCanChangeVolume is set.
 *
 * Returns 0 at once when gSSXAudio's mPathfinderCanChangeVolume (cAudioInterface_bxAudio) is 0. Refreshes the status
 * when the slot is dirty; when the fade target (+0x8) is unset or reached it clears it and, if v differs from the
 * current volume (+0xC) and both are >= 0, applies v with SNDvol (bank player) or SNDSTRM_setVolume (stream) inside the
 * critical section and stores it. A pending fade makes it return -8. Called by pathxSND_play and the Pathfinder
 * library.
 *
 * @param idx  the player-slot index
 * @param volume  the new volume
 * @return 0, or -8 while a fade is pending
 */
extern "C" int pathxSND_setVolume(int idx, int volume)
{
    tPathPlayer* player = D_004D3E98[idx];
    int result = 0;
    if (cAudioInterface_bxAudio()->mPathfinderCanChangeVolume == 0) return 0;
    if (player->mDirty != 0) {
        pathxSND_refreshAll(0);
    }
    int cur = player->mVolume;
    if (player->mFadeTarget < 0 || player->mFadeTarget == cur) {
        player->mFadeTarget = -1;
        if (cur != volume && volume >= 0 && cur >= 0) {
            SNDSYS_entercritical();
            if (player->mIsBankPlayer != 0) {
                SNDvol(player->mRequests[0].mHandle, volume);
            } else {
                SNDSTRM_setVolume(player->mHandle, volume);
            }
            SNDSYS_leavecritical();
            player->mVolume = volume;
        }
    } else {
        result = -8;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_setPaused);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSTRM_pitchmult(int h, int v);
extern "C" void SNDpitchmult(int h, int v);

/**
 * Pauses or resumes a Pathfinder player slot by its pitch multiplier.
 *
 * With reset set it saves the play options' pitchmult (+0x3C) in +0x18 and applies 0 (stops playback); otherwise it
 * applies the saved +0x18. Uses SNDpitchmult on a bank player's request-0 sound, SNDSTRM_pitchmult on a stream, inside
 * the critical section, then marks the slot dirty. Called by the Pathfinder library (PATH_setPaused).
 *
 * @param idx  the player-slot index
 * @param reset  nonzero to pause (pitch 0), 0 to restore the saved pitch
 * @return 0
 */
extern "C" int pathxSND_setPaused(int idx, int reset)
{
    tPathPlayer* player = D_004D3E98[idx];
    unsigned short pitchMult;
    if (reset != 0) {
        pitchMult = 0;
        player->mSavedPitchMult = player->mPlayOpts.pitchmult;
    } else {
        pitchMult = player->mSavedPitchMult;
    }
    SNDSYS_entercritical();
    if (player->mIsBankPlayer != 0) {
        SNDpitchmult(player->mRequests[0].mHandle, pitchMult);
    } else {
        SNDSTRM_pitchmult(player->mHandle, pitchMult);
    }
    SNDSYS_leavecritical();
    player->mDirty = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", pathxSND_stop);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSTRM_purge(int);
extern "C" void SNDstop(int);

/**
 * Stops a Pathfinder player slot: SNDstop on a bank player's request-0 sound, SNDSTRM_purge on a stream.
 *
 * Inside the critical section; then marks the slot dirty. Called by the Pathfinder library (PATH_stopVoices,
 * PATHI_settleFades).
 *
 * @param idx  the player-slot index
 * @return 0
 */
extern "C" int pathxSND_stop(int idx)
{
    tPathPlayer* player = D_004D3E98[idx];
    SNDSYS_entercritical();
    if (player->mIsBankPlayer != 0) {
        SNDstop(player->mRequests[0].mHandle);
    } else {
        SNDSTRM_purge(player->mHandle);
    }
    SNDSYS_leavecritical();
    player->mDirty = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", SPCH_StartSpeechCallback);
#ifdef SKIP_ASM
extern "C" int cSpeechSys_PlayEvent(cSpeechSys* self, int a1);
extern int D_004A37BC;

// PORT: real signature is int(int); the unit later declares SPCH_StartSpeechCallback as void() for its callback use.
extern "C" int SPCH_StartSpeechCallback_impl(int a) __asm__("SPCH_StartSpeechCallback");
/**
 * The speech driver's start callback: plays a speech event through cSpeechSys (cSpeechSys_PlayEvent on D_004A37B4).
 *
 * Returns -1 without playing while D_004A37BC is set (cSpeechSys_FlushChannel sets it while it flushes a channel). SPCH_Init
 * registers it with the speech driver (SPCH_initDriver).
 *
 * @param speechEvent  the speech event, passed on to cSpeechSys_PlayEvent
 * @return cSpeechSys_PlayEvent's result (the started speech's handle, or -1)
 */
extern "C" int SPCH_StartSpeechCallback_impl(int speechEvent)
{
    if (D_004A37BC != 0) {
        return -1;
    }
    return D_004A37B4->PlayEvent(speechEvent);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechBank_cSpeechBank__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a cSpeechBank: mBankId = -1, mOwnsData = 0, mpData = 0 and an empty mName.
 *
 * cSpeechBank_Create runs it on cMemMan_alloc(0x4C, "SpeechBank").
 *
 * @return self
 */
void* cSpeechBank_cSpeechBank(void* vself)
{
    cSpeechBank* self = (cSpeechBank*)vself;
    int zero = 0;
    self->mOwnsData = zero;
    self->mBankId = -1;
    self->mpData = (void*)zero;
    self->mName[0] = (signed char)zero;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", _$_11cSpeechBank);
#ifdef SKIP_ASM
extern "C" void SPCH_removeTable(int bankId);

/**
 * Destroys a cSpeechBank: releases its driver bank (SPCH_removeTable(mBankId)) and frees mpData when mOwnsData.
 *
 * Deletes the object when bit 0 of flags is set. The cSpeechInstance destructor cSpeechInstance::~cSpeechInstance calls it with 3.
 */
cSpeechBank::~cSpeechBank()
{
    SPCH_removeTable(mBankId);
    if (mOwnsData != 0) {
        MEM_free(mpData);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechBank_Create);
#ifdef SKIP_ASM
extern "C" int BXFILE_exists(const char* name);
extern "C" void* FILE_load(const char* name, int a1);
extern "C" int SPCH_addTable(void* data);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strrchr(const char* s, int c);
void* cSpeechBank_cSpeechBank(void* self);
extern char str_pct_s_hdr[];
extern char str_SpeechBank[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" int sprintf(char* buf, const char* fmt, ...);

void* cSpeechBank_Create_impl(const char* name, const char* shortName, void* data) __asm__("cSpeechBank_Create");

/**
 * Creates a cSpeechBank for "<name>.hdr": loads the header unless given, registers it with the speech driver.
 *
 * Without data it returns 0 when the file is missing (BXFILE_exists), else FILE_load()s it and sets mOwnsData.
 * Allocates the bank (cMemMan_alloc(0x4C, "SpeechBank") + cSpeechBank_cSpeechBank), stores mpData, the driver bank
 * id mBankId = SPCH_addTable(data), copies name into mName and points mShortName after its last '\'. Called by
 * cSpeechInstance_AddBank.
 *
 * @param name  the bank path without extension
 * @param shortName  not used (callers pass the short name)
 * @param data  the header data already in memory, or 0 to load it
 * @return the new cSpeechBank, or 0 when the file is missing
 */
void* cSpeechBank_Create_impl(const char* name, const char* shortName, void* data)
{
    char buf[80];
    int owned = data == 0;
    sprintf(buf, str_pct_s_hdr, name);
    if (data == 0) {
        if (BXFILE_exists(buf) == 0) {
            return 0;
        }
        data = FILE_load(buf, 0);
    }
    cSpeechBank* obj = (cSpeechBank*)cSpeechBank_cSpeechBank(cMemMan_alloc(76, str_SpeechBank, 0, 0));
    obj->mOwnsData = owned;
    obj->mpData = data;
    obj->mBankId = SPCH_addTable(data);
    strcpy(obj->mName, name);
    char* lastSlash = strrchr(obj->mName, '\\');
    if (lastSlash == 0) {
        obj->mShortName = obj->mName;
    } else {
        obj->mShortName = lastSlash + 1;
    }
    return ((char*)obj);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechBank_HasBankId);
#ifdef SKIP_ASM
/**
 * Returns whether a cSpeechBank's driver bank id (mBankId) equals bankId.
 *
 * @param bankId  the bank id to compare
 * @return true on a match, else false
 */
bool cSpeechBank::HasBankId(int bankId)
{
    return bankId == mBankId;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", __15cSpeechInstanceiiiiii);
#ifdef SKIP_ASM
extern "C" void cStreamSys_STREAM_Create(void* sys, int id, int a, int b, int c);
extern "C" void cSpeechInstance_ReadConfig(cSpeechInstance* self, int a, int b, int c);

/**
 * Constructs a cSpeechInstance: one speech stream of cSpeechSys, with its event files and speech banks.
 *
 * Stores the index (mIndex), stream id (mStreamId) and @p reserved (unk_008), sets mReadyForSpeech = 1, zeroes the
 * counters and request fields and the four event-file pairs, opens the stream through cStreamSys
 * (cStreamSys_STREAM_Create(stream id, memType, 0, 0x20000)) and reads its speech.inf section (cSpeechInstance_ReadConfig(index,
 * section, loadLangHead)). cSpeechSys_CreateInstance calls it on cMemMan_alloc(0x904, "SpeechInstance").
 *
 * @param instanceIndex  the instance's index in cSpeechSys::mSpeechInstance
 * @param streamId  the cStreamSys stream id
 * @param section  the speech.inf section to read
 * @param memType  the memory type passed to the stream open
 * @param loadLangHead  nonzero to also load the language headers ("%slanghead.big")
 * @param reserved  stored in unk_008, which nothing reads (name ours; cSpeechSys_CreateInstance's callers pass -1)
 */
cSpeechInstance::cSpeechInstance(int instanceIndex, int streamId, int section, int memType, int loadLangHead,
                                 int reserved)
{
    cSpeechInstance* self = this;
    self->mIndex = instanceIndex;
    self->mStreamId = streamId;
    self->mReadyForSpeech = 1;
    self->unk_008 = reserved;
    self->mNumEventFiles = 0;
    self->mNumBanks = 0;
    self->unk_8C4 = 0;
    self->mHoldTime = 0;
    self->mPosition = 0;
    self->mUserInfo = 0;
    self->mRadius = 0;
    self->unk_00C[0] = 0;
    for (int i = 0; i < 4; i++) {
        self->mEventFile[i].mpData = 0;
        self->mEventFile[i].mOwned = 0;
    }
    cStreamSys_STREAM_Create(D_004A37B4->mpVStreamSys, self->mStreamId, memType, 0, 0x20000);
    self->ReadConfig(instanceIndex, section, loadLangHead);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", _$_15cSpeechInstance);
#ifdef SKIP_ASM
extern "C" void cStreamSys_STREAM_Destroy(void* self, int i);
extern "C" void _$_11cSpeechBank(cSpeechBank* self, int flags);
extern int cSpeechInstance_mNumTotalBanks;

/**
 * Destroys a cSpeechInstance: its speech banks, event files and header data, then closes its stream.
 *
 * Destroys each speech bank (cSpeechBank::~cSpeechBank with 3) and subtracts their count (mNumBanks) from
 * cSpeechInstance_mNumTotalBanks; for each event file frees both header blocks (mHeaderBigData) and the separately
 * loaded event data; closes the stream (cStreamSys_STREAM_Destroy on cStreamSys). Deletes the object when bit 0 of
 * flags is set. cSpeechSys_Shutdown calls it with 3.
 */
cSpeechInstance::~cSpeechInstance()
{
    int i;
    for (i = 0; i < mNumBanks; i++) {
        if (mBanks[i] != 0) {
            _$_11cSpeechBank(mBanks[i], 3);
        }
    }
    cSpeechInstance_mNumTotalBanks -= mNumBanks;
    for (i = 0; i < mNumEventFiles; i++) {
        for (int j = 0; j < 2; j++) {
            if (mHeaderBigData[i][j] != 0) {
                MEM_free(mHeaderBigData[i][j]);
            }
        }
        if (mEventFile[i].mOwned != 0) {
            MEM_free(mEventFile[i].mpData);
            mEventFile[i].mOwned = 0;
            mEventFile[i].mpData = 0;
        }
    }
    cStreamSys_STREAM_Destroy(D_004A37B4->mpVStreamSys, mStreamId);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechInstance_Update);
#ifdef SKIP_ASM
extern "C" int cSpeechInstance_CanTakeSpeech(cSpeechInstance* p);
extern "C" int SPCH_startNextRequest(int channel);
extern "C" void* SPCH_updateChannel(int channel);

class cSpeechInstance_Update_cObj {
public:
    char data[0x1BC];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void OnReadyForSpeech();  // slot 4 (name ours): cSSXAudio fills it with cSSXAudio_PlayPendingDJLine, which plays
                                      // the most urgent pending DJ line
};

/**
 * Per-frame update of one speech instance: lets the speech driver start or advance speech on channel i.
 *
 * When the stream can take speech (cSpeechInstance_CanTakeSpeech): with mReadyForSpeech set it calls cSpeechSys's virtual slot 4 (on
 * D_004A37B4) and then the driver's SPCH_startNextRequest(i), clearing mReadyForSpeech when that returns nonzero; with
 * mReadyForSpeech clear it calls SPCH_updateChannel(i) and sets mReadyForSpeech. When the stream is busy it sets
 * mReadyForSpeech. Called by cSpeechSys_Update for both instances.
 *
 * @param i  the instance's index (the speech driver channel)
 */
void cSpeechInstance::Update(int i)
{
    if (CanTakeSpeech() != 0) {
        if (mReadyForSpeech != 0) {
            ((cSpeechInstance_Update_cObj*)D_004A37B4)->OnReadyForSpeech();
            if (SPCH_startNextRequest(i) != 0) {
                mReadyForSpeech = 0;
            }
        } else {
            SPCH_updateChannel(i);
            mReadyForSpeech = 1;
        }
    } else {
        mReadyForSpeech = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechInstance_StartNextSpeech);
#ifdef SKIP_ASM
extern "C" int SPCH_startNextRequest(int channel);

// PORT: real arity is (self, channel); the unit later declares cSpeechInstance_StartNextSpeech(int)
int cSpeechInstance_StartNextSpeech_impl(cSpeechInstance* self, int channel) __asm__("cSpeechInstance_StartNextSpeech");

/**
 * Starts the next speech on a channel (SPCH_startNextRequest) and clears the instance's mReadyForSpeech when that returns
 * nonzero.
 *
 * cSpeechSys_StartNextSpeech calls it on cSpeechSys's instance for the channel.
 *
 * @param channel  the speech driver channel
 * @return SPCH_startNextRequest's result
 */
int cSpeechInstance::StartNextSpeech(int channel)
{
    int result = SPCH_startNextRequest(channel);
    if (result != 0) {
        mReadyForSpeech = 0;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechInstance_LoadEventFile);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_pct_spct_s_c[];
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void* FILE_loadz(const char* name, int flags);
extern "C" void* BIG_locate(void* big, char* name);
extern "C" int BIG_count(void* big);
extern "C" void SPCH_registerBank(void* h, int a);
char* BIG_locateentryz_2AFD40(void* big, char* name, int index, int* offset, int* size) __asm__("BIG_locateentryz");
extern char str_pct_seventdat_events_evt[];
extern "C" void cSpeechInstance_AddBank_l(cSpeechInstance* self, const char* bankPath, const char* shortName, void* headerData)
    __asm__("cSpeechInstance_AddBank");

/**
 * Loads one speech event file set for a cSpeechInstance: a headers .big with its banks, or a plain events file.
 *
 * Loads name (FILE_loadz) into the next event-file pair (mHeaderBigData[mNumEventFiles][0] for headers, [1] for
 * language headers). For the headers .big it locates "<mBasePath>eventdat\events.evt" inside it (an optional "prefix|"
 * in the name sets the prefix), registers it with the speech driver (SPCH_registerBank(.., instIndex)) and returns false
 * when it is missing. Then every other entry of the .big becomes a speech bank (cSpeechInstance_AddBank with the entry's path
 * minus its 4-character extension, its short name and its data).
 *
 * When name cannot be loaded it falls back to the standalone events file (FILE_loadz of
 * "<mBasePath>eventdat\events.evt", marked owned) and registers that. Note: it overwrites the caller's name buffer with
 * the events path. Called by cSpeechInstance_ReadConfig.
 *
 * @param isHeaders  nonzero for "%sheaders.big", 0 for "%slanghead.big"
 * @param name  the file name (overwritten)
 * @param instIndex  the instance index passed to the speech driver
 * @param section  the speech.inf section (not used)
 * @return true when loaded, false when the events data is missing
 */
bool cSpeechInstance::LoadEventFile(int isHeaders, char* name, int instIndex, int section)
{
    char buf[0x60];
    char path[0x60];
    int off;
    int sel = 0;
    if (!isHeaders)
        sel++;
    mHeaderBigData[mNumEventFiles][sel] = FILE_loadz(name, 0);
    if (mHeaderBigData[mNumEventFiles][sel] != 0) {
        buf[0] = 0;
        if (isHeaders != 0) {
            sprintf(name, str_pct_seventdat_events_evt, mBasePath);
            char* entryName = name;
            int len = strlen(name);
            int i;
            for (i = 0; i < len; i++) {
                if (name[i] == '|') {
                    int n = i + 1;
                    entryName = name + n;
                    strncpy(buf, name, n);
                    buf[n] = 0;
                    break;
                }
            }
            mEventFile[mNumEventFiles].mpData =
                BIG_locate(mHeaderBigData[mNumEventFiles][sel], entryName);
            mEventFile[mNumEventFiles].mOwned = 0;
            if (mEventFile[mNumEventFiles].mpData == 0) {
                MEM_free(mHeaderBigData[mNumEventFiles][sel]);
                mHeaderBigData[mNumEventFiles][sel] = 0;
                return false;
            }
            SPCH_registerBank(mEventFile[mNumEventFiles].mpData, instIndex);
        } else {
            buf[1] = 0;
            buf[0] = '|';
        }
        int n = BIG_count(mHeaderBigData[mNumEventFiles][sel]);
        for (int i = 0; i < n; i++) {
            char* nm = BIG_locateentryz_2AFD40(mHeaderBigData[mNumEventFiles][sel], 0, i, &off, 0);
            char* entryData = (char*)mHeaderBigData[mNumEventFiles][sel] + off;
            if (entryData != mEventFile[mNumEventFiles].mpData) {
                sprintf(path, str_pct_spct_s_c, buf, nm);
                int len = strlen(path);
                path[len - 4] = 0;
                int j = len - 4;
                while (path[j] != '\\' && j >= 0)
                    j--;
                cSpeechInstance_AddBank_l(this, path, path + j + 1, entryData);
            }
        }
    } else {
        sprintf(name, str_pct_seventdat_events_evt, mBasePath);
        mEventFile[mNumEventFiles].mpData = FILE_loadz(name, 0);
        if (mEventFile[mNumEventFiles].mpData == 0)
            return false;
        mEventFile[mNumEventFiles].mOwned = 1;
        SPCH_registerBank(mEventFile[mNumEventFiles].mpData, instIndex);
    }
    return true;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSpeechInstance_ReadConfig);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void __10FileParserPCci(void* parser, void* src, int n);
extern "C" void _$_10FileParser(void* parser, int flags);
extern "C" void FileParser_SkipSectionNames(void* parser);
extern "C" int FileParser_FindPartialSection(void* parser, int file);
extern "C" char* FileParser_GetName(void* parser);
extern "C" int FileParser_IsEOF(void* parser);
extern "C" int FileParser_AssignString(void* parser, const char* key, char* out);
void cStreamSys_STREAM_SetBigFile_2B0088(void* self, int i, char* val) __asm__("cStreamSys_STREAM_SetBigFile");
extern "C" bool cSpeechInstance_LoadEventFile(cSpeechInstance* self, int a, char* name, int b, int c);
extern "C" int strcasecmp(const char* a, const char* b);
extern void* D_004A37B0;
extern char str_BIGFILE[];
extern char str_BASEPATH[];
extern char str_BANK[];
extern char str_pct_sheaders_big[];
extern char str_pct_slanghead_big[];
extern char str_pct_spct_s_c[];
extern "C" void cSpeechInstance_AddBank(cSpeechInstance* self, const char* bankPath, const char* shortName, void* headerData);

/**
 * Reads a cSpeechInstance's speech.inf section (D_004A37B0): its .big file, base paths and extra banks.
 *
 * Parses the section whose name starts with a2 (FileParser_FindPartialSection). BIGFILE sets the stream's .big file
 * (cStreamSys_STREAM_SetBigFile on cStreamSys); each BASEPATH stores mBasePath, loads "<base>headers.big" (cSpeechInstance_LoadEventFile; stops at the
 * first failure), optionally "<base>langhead.big", and counts an event file (mNumEventFiles); BANK adds a speech bank
 * (cSpeechInstance_AddBank). Likely bug: the BANK branch reads the value into one buffer but builds the path from, and passes, an
 * uninitialised one (name). Called by the constructor cSpeechInstance::cSpeechInstance and by cSpeechSys_ReloadInstanceConfig.
 *
 * @param instanceIndex  the instance's index
 * @param section  the section to read
 * @param loadLangHead  nonzero to also load the language headers
 */
void cSpeechInstance::ReadConfig(int instanceIndex, int section, int loadLangHead)
{
    char parser[2064];
    char value[0x100];
    char bigFileName[0x40];
    char path[0x80];
    char name[0x60];
    __10FileParserPCci(parser, D_004A37B0, 0x100);
    if (FileParser_FindPartialSection(parser, section) != 0)
    {
        FileParser_SkipSectionNames(parser);
        while (FileParser_IsEOF(parser) == 0)
        {
            char* key = FileParser_GetName(parser);
            if (strcasecmp(str_BIGFILE, key) == 0)
            {
                FileParser_AssignString(parser, str_BIGFILE, bigFileName);
                cStreamSys_STREAM_SetBigFile_2B0088(D_004A37B4->mpVStreamSys, mStreamId, bigFileName);
            }
            if (strcasecmp(str_BASEPATH, key) == 0)
            {
                FileParser_AssignString(parser, str_BASEPATH, mBasePath);
                sprintf(value, str_pct_sheaders_big, mBasePath);
                if (LoadEventFile(1, value, instanceIndex, section) == 0)
                    break;
                if (loadLangHead)
                {
                    sprintf(value, str_pct_slanghead_big, mBasePath);
                    LoadEventFile(0, value, instanceIndex, section);
                }
                mNumEventFiles += 1;
            }
            if (strcasecmp(str_BANK, key) == 0)
            {
                FileParser_AssignString(parser, str_BANK, value);
                sprintf(path, str_pct_spct_s_c, mBasePath, name);
                AddBank(path, name, 0);
            }
        }
    }
    _$_10FileParser(parser, 2);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechInstance_CanTakeSpeech);
#ifdef SKIP_ASM
extern "C" int cStreamSys_STREAM_Requests(void* self, int i);
extern "C" float cStreamSys_STREAM_TimeRemaining(void* self, int i);
extern "C" int cStreamSys_STREAM_Playing(void* self, int i);

/**
 * Returns whether a speech instance's stream can take a new speech: idle, or playing with under 500 left.
 *
 * Asks cStreamSys about the instance's stream (mStreamId): state 0 (cStreamSys_STREAM_Requests) gives 1; state 1 gives
 * 1 when cStreamSys_STREAM_Playing is nonzero and cStreamSys_STREAM_TimeRemaining is below 500; anything else 0.
 *
 * @return 1 or 0
 */
int cSpeechInstance::CanTakeSpeech()
{
    int state = cStreamSys_STREAM_Requests(D_004A37B4->mpVStreamSys, mStreamId);
    if (state == 0) {
        return 1;
    }
    if (state != 1) {
        return 0;
    }
    if (cStreamSys_STREAM_Playing(D_004A37B4->mpVStreamSys, mStreamId) == 0) {
        return 0;
    }
    int ok = (int)cStreamSys_STREAM_TimeRemaining(D_004A37B4->mpVStreamSys, mStreamId) < 500;
    int canPlay = 0;
    if (ok) {
        canPlay = state;
    }
    return canPlay;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechInstance_Purge);
#ifdef SKIP_ASM
extern "C" void cStreamSys_STREAM_Purge(void* self, int i);

/**
 * Purges a speech instance's stream (cStreamSys_STREAM_Purge on cStreamSys with mStreamId).
 *
 * cSpeechSys_PurgeInstance calls it.
 *
 */
void cSpeechInstance::Purge()
{
    cStreamSys_STREAM_Purge(D_004A37B4->mpVStreamSys, mStreamId);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSpeechInstance_AddBank);
#ifdef SKIP_ASM
extern "C" cSpeechBank* cSpeechBank_Create(const char* bankPath, const char* shortName, void* headerData);
extern int cSpeechInstance_mNumTotalBanks;

/**
 * Creates a speech bank (cSpeechBank_Create) and appends it to the instance's bank list when it loaded.
 *
 * On success increments the bank count (mNumBanks) and the global bank count cSpeechInstance_mNumTotalBanks. Called by
 * cSpeechInstance_LoadEventFile and cSpeechInstance_ReadConfig.
 *
 * @param bankPath  the bank path without extension
 * @param shortName  the short name
 * @param headerData  the header data, or 0 to load it
 */
void cSpeechInstance::AddBank(const char* bankPath, const char* shortName, void* headerData)
{
    mBanks[mNumBanks] = cSpeechBank_Create(bankPath, shortName, headerData);
    if (mBanks[mNumBanks] != 0) {
        mNumBanks++;
        cSpeechInstance_mNumTotalBanks++;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", SPCH_GetBankLanguage);
#ifdef SKIP_ASM
extern "C" unsigned int strlen(const char* s);

/**
 * Returns the language of a speech bank name from its suffix: _eng 1, _frn 2, _grm 3, _spn 4, otherwise 0.
 *
 * cSpeechInstance_PlayEvent uses it to pick the .big file from cSpeechSys::mBigFileName.
 *
 * @param name  the bank name
 * @return the language index (0 = the language-independent speech.big)
 */
extern "C" int SPCH_GetBankLanguage(const char* name)
{
    int lang = 0;
    int len = strlen(name);
    if (name[len - 4] == '_') {
        if (name[len - 3] == 'e' && name[len - 2] == 'n' && name[len - 1] == 'g') {
            lang = 1;
        } else if (name[len - 3] == 'f' && name[len - 2] == 'r' && name[len - 1] == 'n') {
            lang = 2;
        } else if (name[len - 3] == 'g' && name[len - 2] == 'r' && name[len - 1] == 'm') {
            lang = 3;
        } else if (name[len - 3] == 's' && name[len - 2] == 'p' && name[len - 1] == 'n') {
            lang = 4;
        }
    }
    return lang;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSpeechInstance_PlayEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// A bank list (cSpeechInstance::cSpeechInstance ctor / cSpeechInstance::~cSpeechInstance dtor): its voices and up to 500 loaded streams.
extern char str_pct_spct_s_c[];
extern char str_data_b[];
extern char str_pct_spct_spct_s_dat[];
extern "C" bool cSpeechBank_HasBankId(cSpeechBank* self, int a1);
extern "C" int cSpeechSys_ApplyRequest_c(void* sys, int a, int b) __asm__("cSpeechSys_ApplyRequest");
extern "C" int SPCH_GetBankLanguage(const char* name);
extern "C" void cStreamSys_STREAM_SetVolControl(void* self, int i, int w);

typedef int (*Fn1_04D8)(void*, int, char*, int, int, int, int, float, int, int);

// A speech driver event (names ours, from use): cSpeechInstance_PlayEvent matches bankId against its banks' driver ids, passes
// fileOffset to cSpeechSys's play slot (cSSXAudio::PlayPositional's fileOffset) and requestKey to cSpeechSys_ApplyRequest.
struct sBmArgs_04D8 {
    int bankId;                // 0x0
    int fileOffset;            // 0x4
    char pad8[4];
    unsigned short requestKey; // 0xC
};

static inline cSpeechBank** at_04D8(cSpeechBank** base, int i)
{
    return base + i;
}

extern "C" int cMonitor_MONITOR_Create_c(void* bank, int a1, sSndCallback id, int a3, int a4, int a5, int a6, int a7) __asm__("cMonitor_MONITOR_Create");
extern "C" int cSpeechInstance_PlayEvent_impl(cSpeechInstance* self, sBmArgs_04D8* a) __asm__("cSpeechInstance_PlayEvent");

/**
 * Plays a speech event on a cSpeechInstance: finds its bank, builds the stream file name and starts it.
 *
 * Finds the bank whose driver id is the event's bank (first by index, then by search); without one, or when no pending
 * request matches (cSpeechSys_ApplyRequest(instance index, event key)), returns -1. Builds "data/<mBigFileName[language]><bank
 * name>.dat" and gets a volume id from cSpeechSys's slot 2 (GetSpeechVolumeIdentifier).
 *
 * When the stream is busy, it starts the speech with cSpeechSys's slot 1 (hold -1) and arms a monitor callback:
 * cSpeechSys_ApplyVolumeCallback (D_004A37D8) after the stream's remaining time when cStreamSys_STREAM_Playing is set, else cSpeechSys_PollStreamCallback
 * (D_004A37E0) every 200 ms, with the instance, the handle and the volume id. When it is idle it applies slot 3
 * (UpdateSpeechVolume) to the stream (cStreamSys_STREAM_SetVolControl) and starts the speech with hold mHoldTime.
 * Called by cSpeechSys_PlayEvent.
 *
 * @param speechEvent  the speech driver event (bankId, fileOffset, requestKey)
 * @return the speech handle from the play slot, or -1
 */
extern "C" int cSpeechInstance_PlayEvent_impl(cSpeechInstance* self, sBmArgs_04D8* speechEvent)
{
    int idx = -1;
    int i;
    if (self->mBanks[speechEvent->bankId]->HasBankId(speechEvent->bankId)) {
        idx = speechEvent->bankId;
    } else {
        for (i = 0; i < self->mNumBanks; i++) {
            if (self->mBanks[i]->HasBankId(speechEvent->bankId)) {
                idx = i;
            }
        }
    }
    if (idx >= 0 && cSpeechSys_ApplyRequest_c((char*)D_004A37B4, self->mIndex, speechEvent->requestKey)) {
        cSpeechBank** slot = at_04D8(self->mBanks, idx);
        int lang = SPCH_GetBankLanguage((*slot)->mName);
        char buf[0x100];
        sprintf(buf, str_pct_spct_spct_s_dat, str_data_b, (char*)&D_004A37B4->mBigFileName[0] + lang * 20,
                (*slot)->mName);
        cSpeechSys* speechSys = D_004A37B4;
        sSndVtSlot* vt0 = (sSndVtSlot*)speechSys->mVtbl;
        int volumeId = ((int (*)(void*, int, int))vt0[2].fn)(vthis(speechSys, vt0[2]), self->mIndex,
                                                             (int)(*slot)->mShortName);
        cSpeechSys* speechBase;
        sSndVtSlot* vt;
        int remainingMs = (int)cStreamSys_STREAM_TimeRemaining(D_004A37B4->mpVStreamSys, self->mStreamId);
        int playHandle;
        if (cStreamSys_STREAM_Requests(D_004A37B4->mpVStreamSys, self->mStreamId) != 0) {
            if (cStreamSys_STREAM_Playing(D_004A37B4->mpVStreamSys, self->mStreamId) != 0) {
                speechBase = D_004A37B4;
                vt = (sSndVtSlot*)speechBase->mVtbl;
                playHandle = ((Fn1_04D8)vt[1].fn)(vthis(speechBase, vt[1]), self->mStreamId, buf,
                        speechEvent->fileOffset,
                                                  self->mPosition, -1, self->mUserInfo, self->mRadius, self->mIsHuman,
                                                  self->mViewport);
                if (playHandle >= 0) {
                    cMonitor_MONITOR_Create_c(&D_004A37B4->mpVStreamSys->mpVBankManager->BankMonitor, remainingMs,
                                              D_004A37D8[0], (int)self, playHandle, volumeId, 0, 0);
                }
                return playHandle;
            }
            speechBase = D_004A37B4;
            vt = (sSndVtSlot*)speechBase->mVtbl;
            playHandle = ((Fn1_04D8)vt[1].fn)(vthis(speechBase, vt[1]), self->mStreamId, buf, speechEvent->fileOffset,
                                              self->mPosition, -1, self->mUserInfo, self->mRadius, self->mIsHuman,
                                              self->mViewport);
            if (playHandle >= 0) {
                cMonitor_MONITOR_Create_c(&D_004A37B4->mpVStreamSys->mpVBankManager->BankMonitor, 200, D_004A37E0[0],
                                          (int)self, playHandle, volumeId, 0, 0);
            }
            return playHandle;
        }
        speechBase = D_004A37B4;
        vt = (sSndVtSlot*)speechBase->mVtbl;
        int w = ((int (*)(void*, int, int, int))vt[3].fn)(vthis(speechBase, vt[3]), self->mIndex, volumeId,
                                                          self->mUserInfo);
        if (w != 0) {
            cStreamSys_STREAM_SetVolControl(D_004A37B4->mpVStreamSys, self->mStreamId, w);
        }
        cSpeechSys* speechSystem = D_004A37B4;
        sSndVtSlot* vt2 = (sSndVtSlot*)speechSystem->mVtbl;
        return ((Fn1_04D8)vt2[1].fn)(vthis(speechSystem, vt2[1]), self->mStreamId, buf, speechEvent->fileOffset,
                self->mPosition,
                                     self->mHoldTime, self->mUserInfo, self->mRadius, self->mIsHuman, self->mViewport);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechInstance_IsPlaying);
#ifdef SKIP_ASM
/**
 * Calls cStreamSys cStreamSys_STREAM_Playing on the speech instance's stream (mStreamId); the result is not returned in
 * C.
 *
 * cSpeechSys_IsPlaying uses the value left in v0.
 */
void cSpeechInstance::IsPlaying()
{
    cStreamSys_STREAM_Playing(D_004A37B4->mpVStreamSys, mStreamId);
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002B07E8);
#ifdef SKIP_ASM
/**
 * Sets a speech instance's unk_8C4.
 *
 * Called by func_002B0FE8.
 *
 * @param speechInstance  the cSpeechInstance
 * @param value  the value
 */
extern "C" void func_002B07E8(cSpeechInstance* speechInstance, int value) {
    speechInstance->unk_8C4 = value;
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechInstance_SetHoldTime);
#ifdef SKIP_ASM
/**
 * Sets a speech instance's mHoldTime (the hold cSpeechInstance_PlayEvent passes when it starts speech on an idle stream).
 *
 * Called by cSpeechSys_SetHoldTime.
 *
 * @param holdValue  the value
 */
void cSpeechInstance::SetHoldTime(int holdValue)
{
    mHoldTime = holdValue;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_cSpeechSys);
#ifdef SKIP_ASM
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" void* cStreamSys_cStreamSys(void* self, int inchrg, int banks, int count, int heap);
struct sBankSlots;
extern "C" void cSpeechSys_ClearRequests(cSpeechSys* self);
extern void* cSpeechSys_vtbl[];
extern void* cBankMonitor_vtbl[];
extern const char str_mSpeechInstance[];

struct sName17_07F8 { char s[17]; };
struct sName18_07F8 { char s[18]; } __attribute__((aligned(2)));
extern const sName17_07F8 D_00483318;
extern const sName18_07F8 D_00483330;
extern const sName17_07F8 D_00483348;
extern const sName17_07F8 D_00483360;
extern const sName18_07F8 D_00483378;

struct sBmonVEntry07F8 {
    short delta;
    short index;
    void* fn;
};
struct sBmonVtblA07F8 {
    sBmonVEntry07F8 e[4];
} __attribute__((aligned(8)));
struct sBmonVtblB07F8 {
    sBmonVEntry07F8 e[7];
} __attribute__((aligned(8)));
// The unit declares these vtables later with its own types; bind views by asm label.
extern const sBmonVtblA07F8 D_004838A0_07F8 __asm__("D_004838A0");
extern const sBmonVtblB07F8 D_004838C0_07F8 __asm__("D_004838C0");

/**
 * Constructs the speech system.
 *
 * When @p inchrg is set it builds the virtual bases first (cSpeechSysComplete): cSoundSys at +0x1C0, cBankManager at
 * +0x1C8 and cStreamSys at +0xC98. Installs the cSpeechSys vtables (cSpeechSys_vtbl at mVtbl, D_004838A0 / D_004838C0
 * in the virtual bases, adjusted on the stack when not in charge), sets mCallbackInstance -1, clears mpBankMem and
 * mCallback, stores itself in D_004A37B4, allocates the two-entry mSpeechInstance array (both empty), clears the
 * request table (cSpeechSys_ClearRequests) and copies the five speech .big file names into mBigFileName.
 *
 * @param inchrg  g++ in-charge flag: nonzero when the virtual bases are built here (the complete object)
 * @param maxBanks  maximum number of banks (On Tour MaxBanks of the cStreamSys / cBankManager constructors)
 * @param maxStreams  maximum number of streams (On Tour MaxStreams of the cStreamSys constructor)
 * @param heap  audio mode (On Tour eSOUNDDEF_MODE audioMode), passed to the base constructors
 * @return @p self
 */
// PORT: g++ 2.95 constructor of a class with virtual bases (stack vtable this-adjust fix-ups written out by hand).
extern "C" char* cSpeechSys_cSpeechSys(cSpeechSys* self, int inchrg, int maxBanks, int maxStreams, int heap)
{
    if (inchrg) {
        cSpeechSysComplete* complete = (cSpeechSysComplete*)self;
        cSoundSys* vb = &complete->mSoundSys;
        cStreamSys* vb2 = &complete->mStreamSys;
        complete->mStreamSys.mpVBankManager = &complete->mBankManager;
        self->mpVStreamSys = vb2;
        complete->mBankManager.BankMonitor.mpVSoundSys = vb;
        cSoundSys_cSoundSys(vb, heap);
        cBankManager_cBankManager(complete->mStreamSys.mpVBankManager, 0, maxBanks, heap);
        cStreamSys_cStreamSys(self->mpVStreamSys, 0, maxBanks, maxStreams, heap);
    }
    self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&D_004838A0_07F8;
    self->mpVStreamSys->mpVBankManager->BankMonitor.mVtbl = cBankMonitor_vtbl;
    self->mpVStreamSys->mpVBankManager->mVtbl = (void**)&D_004838C0_07F8;
    if (inchrg == 0) {
        sBmonVtblA07F8 soundSysVtbl = D_004838A0_07F8;
        self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&soundSysVtbl;
        char* base1 = (char*)self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys - 0x1C0;
        int soundSysAdjust = ((char*)self) - base1;
        soundSysVtbl.e[1].delta = D_004838A0_07F8.e[1].delta + soundSysAdjust;
        sBmonVtblB07F8 bankMgrVtbl = D_004838C0_07F8;
        self->mpVStreamSys->mpVBankManager->mVtbl = (void**)&bankMgrVtbl;
        char* base2 = (char*)self->mpVStreamSys->mpVBankManager - 0x1C8;
        int bankMgrAdjust = ((char*)self) - base2;
        bankMgrVtbl.e[1].delta = D_004838C0_07F8.e[1].delta + bankMgrAdjust;
    }
    self->mVtbl = cSpeechSys_vtbl;
    self->mCallbackInstance = -1;
    self->mpBankMem = 0;
    self->mCallback = 0;
    D_004A37B4 = (cSpeechSys*)(self);
    self->mSpeechInstance = new (str_mSpeechInstance, 0, 0) cSpeechInstance*[2];
    for (int i = 0; i < 2; i++)
        (self->mSpeechInstance)[i] = 0;
    self->ClearRequests();
    *(sName17_07F8*)&self->mBigFileName[0] = D_00483318;
    *(sName18_07F8*)&self->mBigFileName[1] = D_00483330;
    *(sName17_07F8*)&self->mBigFileName[2] = D_00483348;
    *(sName17_07F8*)&self->mBigFileName[3] = D_00483360;
    *(sName18_07F8*)&self->mBigFileName[4] = D_00483378;
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys__cSpeechSys);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void cBankManager__cBankManager(void* self, int flags);
extern "C" void cStreamSys__cStreamSys(void* self, int flags);
extern "C" void cSoundSys__cSoundSys(cSoundSys* self, int flags);
extern void* cSpeechSys_vtbl[];
extern void* cBankMonitor_vtbl[];

struct sBmonVEntry0AE8 {
    short delta;
    short index;
    void* fn;
};
struct sBmonVtblA0AE8 {
    sBmonVEntry0AE8 e[4];
} __attribute__((aligned(8)));
struct sBmonVtblB0AE8 {
    sBmonVEntry0AE8 e[7];
} __attribute__((aligned(8)));
extern const sBmonVtblA0AE8 D_004838A0;
extern const sBmonVtblB0AE8 D_004838C0;

/**
 * Destroys the speech system.
 *
 * Re-installs the cSpeechSys vtables, frees the mSpeechInstance array (the instances themselves are destroyed by
 * cSpeechSys_Shutdown), clears D_004A37B4, destroys the virtual bases (cStreamSys via cStreamSys__cStreamSys, cBankManager, cSoundSys)
 * when bit 1 of @p flags is set and frees the object when bit 0 is set.
 *
 * @param flags  destructor flags: bit 1 destroys the virtual bases, bit 0 frees the object
 */
// PORT: g++ 2.95 virtual-base destruction with stack vtable this-adjust fix-ups, written out by hand.
extern "C" void cSpeechSys__cSpeechSys(cSpeechSys* self, int flags)
{
    self->mVtbl = cSpeechSys_vtbl;
    self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&D_004838A0;
    self->mpVStreamSys->mpVBankManager->BankMonitor.mVtbl = cBankMonitor_vtbl;
    self->mpVStreamSys->mpVBankManager->mVtbl = (void**)&D_004838C0;
    if (flags == 0) {
        sBmonVtblA0AE8 soundSysVtbl = D_004838A0;
        self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&soundSysVtbl;
        char* base1 = (char*)self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys - 0x1C0;
        int soundSysAdjust = ((char*)self) - base1;
        soundSysVtbl.e[1].delta = D_004838A0.e[1].delta + soundSysAdjust;
        sBmonVtblB0AE8 bankMgrVtbl = D_004838C0;
        self->mpVStreamSys->mpVBankManager->mVtbl = (void**)&bankMgrVtbl;
        char* base2 = (char*)self->mpVStreamSys->mpVBankManager - 0x1C8;
        int bankMgrAdjust = ((char*)self) - base2;
        bankMgrVtbl.e[1].delta = D_004838C0.e[1].delta + bankMgrAdjust;
    }
    if (self->mSpeechInstance != 0) {
        cMemMan_free(self->mSpeechInstance);
    }
    D_004A37B4 = 0;
    if (flags & 2) {
        cStreamSys__cStreamSys(self->mpVStreamSys, 0);
        cBankManager__cBankManager(self->mpVStreamSys->mpVBankManager, 0);
        cSoundSys__cSoundSys(self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys, 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_Update);
#ifdef SKIP_ASM
struct sBankSlots;
extern "C" void cSpeechInstance_Update(cSpeechInstance*, int);
extern "C" void cSpeechSys_AgeRequests(cSpeechSys*);

/**
 * Per-frame speech update: runs cSpeechInstance_Update on both speech instances, then ages the pending requests.
 *
 * The request timers count down in cSpeechSys_AgeRequests. Called by the audio system's update on gSSXAudio + 0x5560.
 */
void cSpeechSys::Update()
{
    for (int i = 0; i < 2; i++) {
        void* instance = mSpeechInstance[i];
        if (instance != 0) {
            ((cSpeechInstance*)instance)->Update(i);
        }
    }
    AgeRequests();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", SPCH_Init);
#ifdef SKIP_ASM
extern "C" int SPCH_getBufferSize(int a, int b, int c);
// cSoundSys::SndRand() is a member function: this caller passes the shared cSoundSys as its this pointer.
int cSoundSys_SndRand_a(cSoundSys* soundSys) __asm__("cSoundSys_SndRand");
extern "C" void bxLogPrint(const char* fmt, ...);
extern "C" void SPCH_initDriver(void* fn, int seed, int buf);
extern "C" int SPCH_getTableListSize(int n);
extern "C" void* MEM_alloc(void* a0, int a1, int a2);
extern "C" void SPCH_initTableList(int n, void* p);
extern "C" void* SPCH_getCallbackBlock();
extern "C" void SPCH_StartSpeechCallback();
extern "C" int TIMER_gettick();
extern char D_00483390[];
extern char str_bank_mem[];

/**
 * Sets up the speech system's memory and callbacks.
 *
 * Allocates a 0x5622-byte buffer (SPCH_getBufferSize) and prints "SPCH_Init: rNum=%d". It passes SPCH_StartSpeechCallback, a
 * random number (cSoundSys_SndRand on the shared cSoundSys, BankMonitor.mpVSoundSys) and the buffer to SPCH_initDriver.
 * It then sizes a "bank mem" block for 500 (SPCH_getTableListSize), allocates it into mpBankMem and hands it to SPCH_initTableList.
 * The callees are unknown.
 */
extern "C" void SPCH_Init(cSpeechSys* self)
{
    int buf = SPCH_getBufferSize(22050, 0x10, 1);
    int seed = cSoundSys_SndRand_a(self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys);
    bxLogPrint(D_00483390, seed);
    SPCH_initDriver((void*)SPCH_StartSpeechCallback, seed, buf);
    int n = SPCH_getTableListSize(500);
    void* workMem = MEM_alloc(str_bank_mem, n, 0);
    self->mpBankMem = workMem;
    SPCH_initTableList(500, workMem);
    *(void**)((char*)SPCH_getCallbackBlock() + 0x8) = (void*)TIMER_gettick;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_Shutdown);
#ifdef SKIP_ASM
extern "C" void _$_15cSpeechInstance(cSpeechInstance* p, int mode);
extern "C" void SPCH_shutdown(void);

/**
 * Shuts the speech system down: destroys both speech instances and frees the speech bank buffer.
 *
 * Destroys each instance (cSpeechInstance::~cSpeechInstance with 3) and clears its slot, calls the driver's SPCH_shutdown and MEM_free()s
 * mpBankMem, clearing it. Called by the audio system's shutdown.
 */
void cSpeechSys::Shutdown()
{
    for (int i = 0; i < 2; i++) {
        void* instance = mSpeechInstance[i];
        if (instance != 0) {
            _$_15cSpeechInstance((cSpeechInstance*)instance, 3);
        }
        mSpeechInstance[i] = 0;
    }
    SPCH_shutdown();
    MEM_free(mpBankMem);
    mpBankMem = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_CanTakeSpeech);
#ifdef SKIP_ASM
extern "C" int cSpeechInstance_CanTakeSpeech(cSpeechInstance* p);

/**
 * Returns whether speech instance i can take a new speech (cSpeechInstance_CanTakeSpeech), or 0 when it does not exist.
 *
 * The script engine checks it for instance 0 before running the playing NIS list's scripts.
 *
 * @param i  the instance index (0 or 1)
 * @return 1 or 0
 */
int cSpeechSys::CanTakeSpeech(int i)
{
    void* instance = mSpeechInstance[i];
    if (instance != 0) {
        return ((cSpeechInstance*)instance)->CanTakeSpeech();
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_CreateInstance);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" cSpeechInstance* __15cSpeechInstanceiiiiii(cSpeechInstance* mem, int i, int a, int b, int c, int d, int reserved);
extern const char str_SpeechInstance[];

/**
 * Creates speech instance i: cMemMan_alloc(0x904, "SpeechInstance") constructed by cSpeechInstance::cSpeechInstance.
 *
 * Stores it in mSpeechInstance[i]. The audio system creates instance 0 for section "CHAR".
 *
 * @param i  the instance index
 * @param streamId  the cStreamSys stream id
 * @param section  the speech.inf section
 * @param memType  the memory type for the stream
 * @param loadLangHead  nonzero to load the language headers
 * @param reserved  stored in the instance's unk_008, which nothing reads (name ours; both callers pass -1)
 */
void cSpeechSys::CreateInstance(int i, int streamId, int section, int memType, int loadLangHead, int reserved)
{
    // PORT: pointer arithmetic through int
    void** slot = (void**)(i * 4 + (int)mSpeechInstance);
    *slot = __15cSpeechInstanceiiiiii((cSpeechInstance*)cMemMan_alloc(2308, str_SpeechInstance, 0, 0), i, streamId, section,
            memType, loadLangHead, reserved);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_ReloadInstanceConfig);
#ifdef SKIP_ASM
/**
 * Re-reads speech.inf section section for speech instance i (cSpeechInstance_ReadConfig without language headers).
 *
 * @param i  the instance index
 * @param section  the section to read
 */
void cSpeechSys::ReloadInstanceConfig(int i, int section)
{
    void* instance = mSpeechInstance[i];
    if (instance != 0) {
        ((cSpeechInstance*)instance)->ReadConfig(i, section, 0);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_PlayEvent);
#ifdef SKIP_ASM
extern "C" int cSpeechInstance_PlayEvent(void* p, int a1);

/**
 * Plays a speech event on the first speech instance that accepts it and calls mCallback for that instance.
 *
 * Tries instance 0 then 1 with cSpeechInstance_PlayEvent; on success, when mCallbackInstance is that instance, calls mCallback with
 * the handle. Reached from the speech driver through SPCH_StartSpeechCallback.
 *
 * @param playArgs  the speech driver event
 * @return the speech handle, or -1
 */
int cSpeechSys::PlayEvent(int playArgs)
{
    for (int i = 0; i < 2; i++) {
        void* instance = mSpeechInstance[i];
        if (instance != 0) {
            int handle = cSpeechInstance_PlayEvent(instance, playArgs);
            if (handle >= 0) {
                if (mCallbackInstance == i) {
                    (mCallback)(handle);
                }
                return handle;
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B0FE8);
#ifdef SKIP_ASM
extern "C" void func_002B07E8(cSpeechInstance* p, int a);

/**
 * Sets speech instance i's unk_8C4 (func_002B07E8) when it exists.
 *
 * @param i  the instance index
 * @param value  the value
 */
extern "C" void func_002B0FE8(cSpeechSys* self, int i, int value)
{
    void* instance = (self->mSpeechInstance)[i];
    if (instance != 0) {
        func_002B07E8((cSpeechInstance*)instance, value);
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_SetHoldTime);
#ifdef SKIP_ASM
extern "C" void cSpeechInstance_SetHoldTime(cSpeechInstance* p, int a);

/**
 * Sets speech instance i's mHoldTime (cSpeechInstance_SetHoldTime), the hold used on an idle stream, when it exists.
 *
 * @param i  the instance index
 * @param holdValue  the value
 */
void cSpeechSys::SetHoldTime(int i, int holdValue)
{
    void* instance = mSpeechInstance[i];
    if (instance != 0) {
        ((cSpeechInstance*)instance)->SetHoldTime(holdValue);
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_TriggerSpeech);
#ifdef SKIP_ASM
/**
 * Triggers the speech driver on channel idx with one-shot settings, then clears them.
 *
 * Sets the instance's mPosition = a2, mRadius = v (only when v > 0) and mUserInfo = a4, arms mCallback for idx when
 * callback is nonzero, and calls the driver's SPCH_updateChannel(idx), which can start speech (SPCH_StartSpeechCallback ->
 * cSpeechSys_PlayEvent). Afterwards mCallback / mCallbackInstance and the three fields are cleared.
 *
 * @param idx  the instance index (driver channel)
 * @param playArg  mPosition for this trigger (passed to the play slot)
 * @param callback  function to call with the started handle, or 0
 * @param volumeArg  mUserInfo for this trigger
 * @param floatArg  mRadius for this trigger, when > 0
 */
void cSpeechSys::TriggerSpeech(int idx, int playArg, void (*callback)(int), int volumeArg, float floatArg)
{
    cSpeechInstance** chans = mSpeechInstance;
    if (chans[idx] != 0) {
        chans[idx]->mPosition = playArg;
        if (floatArg > 0.0f) {
            mSpeechInstance[idx]->mRadius = floatArg;
        }
        mSpeechInstance[idx]->mUserInfo = volumeArg;
        if (callback != 0) {
            mCallback = callback;
            mCallbackInstance = idx;
        }
        SPCH_updateChannel(idx);
        mCallbackInstance = -1;
        mCallback = 0;
        mSpeechInstance[idx]->mRadius = 0.0f;
        mSpeechInstance[idx]->mPosition = 0;
        mSpeechInstance[idx]->mUserInfo = 0;
    }
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_FlushChannel);
#ifdef SKIP_ASM
extern "C" void cSpeechSys_TriggerSpeech(cSpeechSys* self, int idx, int a2, void (*a3)(int), int a4, float v);
extern int D_004A37BC;

/**
 * Flushes speech channel idx: triggers the driver four times (cSpeechSys_TriggerSpeech with zeros) while D_004A37BC is set.
 *
 * With D_004A37BC set the start callback SPCH_StartSpeechCallback refuses to play, so queued events are consumed without sound.
 *
 * @param idx  the instance index
 */
void cSpeechSys::FlushChannel(int idx)
{
    if (mSpeechInstance[idx] != 0) {
        int i = 3;
        D_004A37BC = 1;
        do {
            TriggerSpeech(idx, 0, 0, 0, 0.0f);
            i--;
        } while (i >= 0);
        D_004A37BC = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_PurgeInstance);
#ifdef SKIP_ASM
extern "C" void cSpeechInstance_Purge(cSpeechInstance* p);

/**
 * Purges speech instance i's stream (cSpeechInstance_Purge) when the instance exists.
 *
 * The audio system calls it with 0 before music changes and when it purges.
 *
 * @param i  the instance index
 */
void cSpeechSys::PurgeInstance(int i)
{
    void* instance = mSpeechInstance[i];
    if (instance != 0) {
        ((cSpeechInstance*)instance)->Purge();
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_IsPlaying);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int cSpeechInstance_IsPlaying_11E8(void* self) __asm__("cSpeechInstance_IsPlaying");

/**
 * Returns cStreamSys cStreamSys_STREAM_Playing for speech instance i's stream (through cSpeechInstance_IsPlaying), or 0 without an instance.
 *
 * @param i  the instance index
 * @return the stream query's result, or 0
 */
int cSpeechSys::IsPlaying(int i)
{
    void* instance = mSpeechInstance[i];
    if (instance != 0) {
        return cSpeechInstance_IsPlaying_11E8(instance);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_GetStreamId);
#ifdef SKIP_ASM
/**
 * Returns speech instance i's stream id (mStreamId), or -1 when the instance does not exist.
 *
 * @param i  the instance index
 * @return the stream id, or -1
 */
int cSpeechSys::GetStreamId(int i)
{
    cSpeechInstance* instance = (cSpeechInstance*)(mSpeechInstance)[i];
    if (((void*)instance) != 0) {
        return instance->mStreamId;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_ReleaseHoldCallback);
#ifdef SKIP_ASM
extern "C" void cStreamSys_STREAM_ReleaseHoldCallback(cStreamSys* streamSys);

/**
 * Ends a speech monitor callback: calls cStreamSys_STREAM_ReleaseHoldCallback on the speech system's cStreamSys (the instance stays in a1).
 *
 * Called as (cSpeechSys*, cMonitorInstance*) by cSpeechSys_ApplyVolumeCallback; returns cStreamSys_STREAM_ReleaseHoldCallback's value in v0.
 *
 */
void cSpeechSys::ReleaseHoldCallback()
{
    cStreamSys_STREAM_ReleaseHoldCallback(mpVStreamSys);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_ApplyVolumeCallback);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPacked8_1268 { long v; } __attribute__((packed));
extern const sPacked8_1268 D_004A37E0_2B1268[] __asm__("D_004A37E0");
extern "C" int cStreamSys_STREAM_GetCurrActiveSampleHandle(void* self, int i);
extern "C" void cStreamSys_STREAM_SetVolControl(void* self, int i, int val);
extern "C" void cMonitor_MONITOR_Create(cBankMonitor* a, int b, long t, void* c, int d, int e, int f, int g);
extern "C" int cSpeechSys_ReleaseHoldCallback_pp(void* self, void* msg) __asm__("cSpeechSys_ReleaseHoldCallback");
struct sVE1268 { short delta; short index; int (*fn)(void*, int, int, int); };

/**
 * Monitor callback after a speech started on a busy stream: applies its volume once the speech is current.
 *
 * msg is the cMonitorInstance (mPtr = the speech instance, mData1 = the speech handle, mData2 = the volume id). When
 * the instance's stream is not playing that handle yet (cStreamSys_STREAM_GetCurrActiveSampleHandle) it re-arms cSpeechSys_PollStreamCallback (D_004A37E0) in 200 ms
 * and returns 1. Otherwise, for a volume id other than -1, it calls cSpeechSys's slot 3 (UpdateSpeechVolume) and stores
 * a nonzero result in the stream (cStreamSys_STREAM_SetVolControl), then ends through cSpeechSys_ReleaseHoldCallback. Armed by cSpeechInstance_PlayEvent (D_004A37D8).
 *
 * @param msg  the cMonitorInstance
 * @return 1 when re-armed, else cSpeechSys_ReleaseHoldCallback's result
 */
int cSpeechSys::ApplyVolumeCallback(cMonitorInstance* msg)
{
    cSpeechInstance* ent = (cSpeechInstance*)msg->mPtr;
    int channel = D_004A37B4->GetStreamId(ent->mIndex);
    if (cStreamSys_STREAM_GetCurrActiveSampleHandle(mpVStreamSys, channel) != msg->mData1) {
        cMonitor_MONITOR_Create(&D_004A37B4->mpVStreamSys->mpVBankManager->BankMonitor, 200,
                                D_004A37E0_2B1268[0].v, msg->mPtr, msg->mData1, msg->mData2, 0, 0);
        return 1;
    }
    int slot = msg->mData2;
    if (slot != -1) {
        sVE1268* vt = (sVE1268*)mVtbl;
        int speechVolume = vt[3].fn(vthis(this, vt[3]), ent->mIndex, slot, ent->mUserInfo);
        if (speechVolume != 0) {
            cStreamSys_STREAM_SetVolControl(mpVStreamSys, ent->mStreamId, speechVolume);
        }
    }
    return cSpeechSys_ReleaseHoldCallback_pp(this, msg);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_PollStreamCallback);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPacked8_1348 { long v; } __attribute__((packed));
extern const sPacked8_1348 D_004A37D8_2B1348[] __asm__("D_004A37D8");
extern "C" int cStreamSys_STREAM_GetCurrActiveSampleHandle(void* self, int i);
extern "C" void cMonitor_MONITOR_Create(cBankMonitor* a, int b, long t, void* c, int d, int e, int f, int g);
extern "C" int cSpeechSys_ApplyVolumeCallback(cSpeechSys* self, cMonitorInstance* msg);

/**
 * Monitor callback polling a busy speech stream until the queued speech can be handled by cSpeechSys_ApplyVolumeCallback.
 *
 * msg is the cMonitorInstance (mPtr = the speech instance, mData1 / mData2 = handle and volume id). While the stream
 * state (cStreamSys_STREAM_Requests) is above 1: if cStreamSys_STREAM_Playing is set it arms cSpeechSys_ApplyVolumeCallback (D_004A37D8) after the remaining time
 * (cStreamSys_STREAM_TimeRemaining) and returns 1; if the stream is not playing the handle yet it returns 0 (poll again); otherwise it
 * hands over to cSpeechSys_ApplyVolumeCallback. Armed by cSpeechInstance_PlayEvent and cSpeechSys_ApplyVolumeCallback (D_004A37E0).
 *
 * @param msg  the cMonitorInstance
 * @return 1 when re-armed, 0 to keep polling, else cSpeechSys_ApplyVolumeCallback's result
 */
int cSpeechSys::PollStreamCallback(cMonitorInstance* msg)
{
    cSpeechInstance* ent = (cSpeechInstance*)((char*)msg->mPtr);
    if (cStreamSys_STREAM_Requests(mpVStreamSys, ent->mStreamId) - 1 > 0) {
        if (cStreamSys_STREAM_Playing(mpVStreamSys, ent->mStreamId) != 0) {
            float remainingMs = cStreamSys_STREAM_TimeRemaining(mpVStreamSys, ent->mStreamId);
            cMonitor_MONITOR_Create(&D_004A37B4->mpVStreamSys->mpVBankManager->BankMonitor, (int)remainingMs,
                                    D_004A37D8_2B1348[0].v, ent, msg->mData1, msg->mData2, 0, 0);
            return 1;
        }
        int channel = D_004A37B4->GetStreamId(ent->mIndex);
        if (cStreamSys_STREAM_GetCurrActiveSampleHandle(mpVStreamSys, channel) != msg->mData1) {
            return 0;
        }
        return ApplyVolumeCallback(msg);
    }
    return ApplyVolumeCallback(msg);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_ClearRequests);
#ifdef SKIP_ASM


/**
 * Clears all ten pending speech requests (mRequest[i].used = 0).
 *
 * cSpeechSys_cSpeechSys calls it.
 */
void cSpeechSys::ClearRequests()
{
    for (int i = 9; i >= 0; i--) {
        ((&mRequest[0]))[i].used = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_AddRequest);
#ifdef SKIP_ASM
struct sBankEntry20 {
    int used;
    int timer;
    int a;
    int b;
    int c;
    int d;
    float f;
    int e;
};

struct sBankEntries {
    char pad[0x18];
    sBankEntry20 entries[10];
};

// cSpeechSys_AddRequest's free-entry index before it has found one (name ours).
enum { kNoFreeRequest = -1 };

/**
 * Adds a pending speech request, or refreshes a matching one; requests expire after 180 frames.
 *
 * A used request with the same instance and id is refreshed (timer 180) only when refresh is set; otherwise the call
 * fails. A new request goes into the first free entry of mRequest (instance, id, object, identity, the float and the
 * callback). The audio system calls it before triggering character speech.
 *
 * @param instIndex  the speech instance index
 * @param requestId  the request id
 * @param object  the object (a rider) the speech is for
 * @param identity  its identity
 * @param callback  function stored in the request, or 0
 * @param refresh  nonzero to refresh an existing request
 * @param requestValue  a float stored in the request
 * @return true when added or refreshed, false otherwise
 */
extern "C" bool cSpeechSys_AddRequest(sBankEntries* self, int instIndex, int requestId, int object, int identity, int callback,
                              int refresh, float requestValue)
{
    int freeIdx = kNoFreeRequest;
    int i;
    for (i = 0; i < 10; i++) {
        if (self->entries[i].used != 0) {
            if (self->entries[i].a == instIndex && self->entries[i].b == requestId) {
                if (refresh == 0) {
                    return false;
                }
                self->entries[i].timer = 180;
                return true;
            }
        } else {
            if (freeIdx == kNoFreeRequest) {
                freeIdx = i;
            }
        }
    }
    if (freeIdx == kNoFreeRequest) {
        return false;
    }
    self->entries[freeIdx].used = 1;
    self->entries[freeIdx].timer = 180;
    self->entries[freeIdx].a = instIndex;
    self->entries[freeIdx].b = requestId;
    self->entries[freeIdx].c = object;
    self->entries[freeIdx].d = identity;
    self->entries[freeIdx].f = requestValue;
    self->entries[freeIdx].e = callback;
    return true;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_ApplyRequest);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
class cBMVoice_1520 {
public:
    // vptr at 0; slot N at vtable offset N*8
    virtual int v01();
    // slots 2-8: [P] the preview's cRider vtable names (ai/rider.h kRiderVt_*)
    virtual int getVelocity();
    virtual int isIntersecting();
    virtual int applyBoost();
    virtual int getPosition();
    virtual int getTransform();
    virtual int getAIIndex();
    virtual int isHuman();
};

struct sBMObj_1520 {
    char pad[0x6C0];
    char voice[4];          // 0x6C0, a cBMVoice_1520
    char pad6C4[0x870 - 0x6C4];
    int z;                  // 0x870
};

struct sBMEnt_1520 {
    int used;       // 0x18
    int pad1C;
    int id;         // 0x20
    int key;        // 0x24
    sBMObj_1520* obj;   // 0x28
    int w;          // 0x2C
    float t;        // 0x30
    int k;          // 0x34
};

struct sBMon_1520 {
    int pad0;
    cSpeechInstance** slots; // 0x4 cSpeechSys::mSpeechInstance
    char pad8[0xC - 0x8];
    int curK;               // 0xC
    int curId;              // 0x10
    char pad14[0x18 - 0x14];
    sBMEnt_1520 ents[10];   // 0x18
};

/**
 * Applies the pending speech request matching (id, key) to its speech instance; returns whether one matched.
 *
 * Reads two values from the request object's sub-object at +0x6C0 (its virtual slots 5 and 8, getPosition and
 * isHuman) and its +0x870 (mViewIndex; -1 without an object) into the instance's mPosition, mIsHuman and mViewport,
 * and copies the request's float and word into mRadius and mUserInfo. A request callback arms mCallback for that
 * instance; otherwise mCallback is cleared. Called by cSpeechInstance_PlayEvent.
 *
 * @param id  the speech instance index
 * @param key  the request id
 * @return true when a request matched, else false
 */
extern "C" bool cSpeechSys_ApplyRequest(sBMon_1520* self, int id, int key)
{
    for (int j = 0; j < 10; j++) {
        if (self->ents[j].used != 0 && self->ents[j].id == id && self->ents[j].key == key) {
            int slotFiveValue = 0;
            int slotEightValue = 0;
            int z = -1;
            if (self->ents[j].obj != 0) {
                slotFiveValue = ((cBMVoice_1520*)self->ents[j].obj->voice)->getPosition();
                slotEightValue = ((cBMVoice_1520*)self->ents[j].obj->voice)->isHuman();
                z = self->ents[j].obj->z;
            }
            self->slots[id]->mPosition = slotFiveValue;
            self->slots[id]->mIsHuman = slotEightValue;
            self->slots[id]->mViewport = z;
            self->slots[id]->mRadius = self->ents[j].t;
            self->slots[id]->mUserInfo = self->ents[j].w;
            int k = self->ents[j].k;
            if (k != 0) {
                self->curK = k;
                self->curId = id;
            } else {
                self->curK = 0;
                self->curId = -1;
            }
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_RemoveRequest);
#ifdef SKIP_ASM
struct sSlot16A0 {
    int active;     // 0x0
    int pad4;
    int idx;        // 0x8
    int key;        // 0xC
    char pad[0x10];
};

/**
 * Drops the pending speech request (idx, key) and clears mCallback and the instance's per-speech fields.
 *
 * For each used matching entry: mCallback = 0, mCallbackInstance = -1, the instance's mRadius, mPosition and
 * mUserInfo = 0, and the entry is freed.
 *
 * @param idx  the speech instance index
 * @param key  the request id
 */
void cSpeechSys::RemoveRequest(int idx, int key)
{
    sSlot16A0* slots = (sSlot16A0*)&mRequest;
    for (int i = 0; i < 10; i++) {
        if (slots[i].active != 0 && slots[i].idx == idx && slots[i].key == key) {
            mCallback = 0;
            mCallbackInstance = -1;
            mSpeechInstance[idx]->mRadius = 0.0f;
            mSpeechInstance[idx]->mPosition = 0;
            mSpeechInstance[idx]->mUserInfo = 0;
            slots[i].active = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_AgeRequests);
#ifdef SKIP_ASM
/**
 * Ages the pending speech requests: decrements each used timer and frees the request when it reaches 0.
 *
 * Called every frame by cSpeechSys_Update.
 */
void cSpeechSys::AgeRequests()
{
    for (int i = 0; i < 10; i++) {
        if (mRequest[i].used != 0) {
            if (--mRequest[i].timer <= 0) {
                mRequest[i].used = 0;
            }
        }
    }
}
#endif

extern "C" void* cSpeechInstance_StartNextSpeech(int);

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_StartNextSpeech__FPvi);
#ifdef SKIP_ASM
/**
 * Starts the next speech on a channel through its speech instance (cSpeechInstance_StartNextSpeech on mSpeechInstance[channel]).
 *
 * The audio system calls it with 0 after a music change.
 *
 * @param channel  the instance index / driver channel
 * @return cSpeechInstance_StartNextSpeech's result
 */
int cSpeechSys::StartNextSpeech(int channel)
{
    // a1 also stays in $5 as cSpeechInstance_StartNextSpeech's second argument
    return mSpeechInstance[channel]->StartNextSpeech(channel);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_OpenStream);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cStreamSys_STREAM_CreateInstance1(void* self, int idx, int heap, char* mem, int size, int n, int m);
extern "C" int cStreamSys_STREAM_GetHandle(void* self, int idx);

/**
 * Opens cStreamSys stream 2 with the given play options and memory; returns cStreamSys_STREAM_GetHandle's value for it.
 *
 * Pushes a play-option level on the shared cBankManager's cBankMonitor (the OPTS_Open sequence: copy mDefault, reset
 * the per-level arrays, delays 100/0/90/50, mMapToMaxVol 127, mCloseOnEnd 1), overwrites the new level with *entry,
 * creates the stream (cStreamSys_STREAM_CreateInstance1(stream system, 2, 0, mem, size, n, m)) and pops the level.
 *
 * @param entry  the play options (SNDPLAYOPTS) for the stream
 * @param n  the maximum queued requests
 * @param maxChunks  the maximum packet-player chunks
 * @param mem  the stream's memory
 * @param size  its size
 * @return cStreamSys_STREAM_GetHandle(stream system, 2)
 */
extern "C" int cMusicSys_OpenStream(SNDPLAYOPTS* entry, int n, int maxChunks, char* mem, int size)
{
    cBankManager* mg = gMusicSys->mpVStreamSys->mpVBankManager;
    cBankMonitor* st = &mg->BankMonitor;
    mg->BankMonitor.mInst++;
    st->mCurrent[mg->BankMonitor.mInst] = mg->BankMonitor.mDefault;
    mg->BankMonitor.mPosition[mg->BankMonitor.mInst] = 0;
    st->mPosStatic[mg->BankMonitor.mInst] = 0;
    st->mBaseVol[mg->BankMonitor.mInst] = st->mCurrent[mg->BankMonitor.mInst].vol;
    st->mVolControl[mg->BankMonitor.mInst] = 0;
    memset((char*)st + mg->BankMonitor.mInst * 8 + 0x40, 0, 8);
    st->mPtr[mg->BankMonitor.mInst] = 0;
    st->mViewport[mg->BankMonitor.mInst] = 0;
    st->mVanish[mg->BankMonitor.mInst] = 0;
    st->mRadius[mg->BankMonitor.mInst] = 0;
    st->mDelay[mg->BankMonitor.mInst].mCount = 0;
    st->mDelay[mg->BankMonitor.mInst].mTime = 100;
    st->mDelay[mg->BankMonitor.mInst].mFeedback = 90;
    st->mDelay[mg->BankMonitor.mInst].mLevel = 50;
    st->mExternalVol[mg->BankMonitor.mInst] = 0;
    st->mTwoPlayerPan[mg->BankMonitor.mInst] = 0;
    st->mTwoPlayerSide[mg->BankMonitor.mInst] = 0;
    st->mMapToMaxVol[mg->BankMonitor.mInst] = 127;
    st->mCloseOnEnd[mg->BankMonitor.mInst] = 1;
    {
        cBankManager* bankManager = gMusicSys->mpVStreamSys->mpVBankManager;
        // through a pointer to the entry: retail adds base first (mCurrent[i] = *entry puts the index first)
        SNDPLAYOPTS* level = &bankManager->BankMonitor.mCurrent[bankManager->BankMonitor.mInst];
        *level = *entry;
    }
    cStreamSys_STREAM_CreateInstance1(gMusicSys->mpVStreamSys, 2, 0, mem, size, n, maxChunks);
    gMusicSys->mpVStreamSys->mpVBankManager->BankMonitor.mInst--;
    return cStreamSys_STREAM_GetHandle(gMusicSys->mpVStreamSys, 2);
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_PurgeStream);
#ifdef SKIP_ASM

/**
 * Purges cStreamSys stream 2 (cStreamSys_STREAM_Purge on gMusicSys's stream system).
 */
extern "C" void cMusicSys_PurgeStream(void) {
    cStreamSys_STREAM_Purge(((void**)gMusicSys->mpVStreamSys), 2);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongConfiguration_cSongConfiguration);
#ifdef SKIP_ASM
extern char cSongConfiguration_vtbl[];
extern unsigned char str_empty_j[];

struct cSongConfiguration_cSongConfiguration_sElem20 {
    unsigned char b;
    char pad[0x1F];
};

/**
 * Constructs a song configuration with the music.inf defaults.
 *
 * Stores the vtable cSongConfiguration_vtbl and sets the defaults a music.inf section can override: no bank data, 4 beats per
 * measure, 2 measures per bar, 4 phrases per bank, 8 beats per phrase, phrase align 16, delay count 0 / feedback 90 /
 * time 100 / level 50, path and async levels 100, 120 BPM, duck to loops on, mSedValue and mPreview -1, mLowPass
 * 0xFFFF, mSongBig 1, and empties the path strings (first byte of str_empty_j).
 *
 * @return @p self
 */
extern "C" void* cSongConfiguration_cSongConfiguration(cSongConfiguration* self)
{
    self->mVtbl = (void**)cSongConfiguration_vtbl;
    self->mNumBankData = 0;
    self->mBeatsPerMeasure = 4;
    self->mMeasuresPerBar = 2;
    self->mPhrasesPerBank = 4;
    self->mBeatsPerPhrase = 8;
    self->mPhraseAlign = 0x10;
    self->mDelayCount = 0;
    self->mDelayFeedback = 90;
    self->mDelayTime = 100;
    self->mDelayLevel = 50;
    self->mPathlevel = 100;
    self->mAsynclevel = 100;
    self->mBPM = 120.0f;
    self->mDuckToLoops = 1;
    self->mSedValue = -1;
    self->mLowPass = 0xFFFF;
    self->mPreview = -1;
    self->mSongBig = 1;
    self->mPathData[0] = str_empty_j[0];
    self->mMusData[0] = str_empty_j[0];
    cSongConfiguration_cSongConfiguration_sElem20* bankData = (cSongConfiguration_cSongConfiguration_sElem20*)&self->mBankData;
    for (int i = 0; i >= 0; i--, bankData++) {
        bankData->b = str_empty_j[0];
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongConfiguration__cSongConfiguration);
#ifdef SKIP_ASM
extern char cSongConfiguration_vtbl[];

/**
 * Destroys a cSongConfiguration: restores its vptr (cSongConfiguration_vtbl) and frees it when bit 0 of flags is set.
 *
 * Virtual slot 1 of _vt$18cSongConfiguration; cMusicSys's destructor calls it with 3 for each song.
 *
 * @param flags  g++ 2.95 destructor flags (bit 0: operator delete the object)
 */
extern "C" void cSongConfiguration__cSongConfiguration(cSongConfiguration* self, int flags)
{
    self->mVtbl = (void**)cSongConfiguration_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_Create);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cSongInstance_cSongInstance(void* mem, int a0, int a1, int a2, int a3, char a4, int a5, int a6, int a7);
extern const char str_SongInstance[];

/**
 * Allocates and constructs a cSongInstance: cMemMan_alloc(0xC4, "SongInstance") + cSongInstance_cSongInstance.
 *
 * Called by cMusicSys_StartSong with cMusicSys's mBasePath first: the first parameter is the base path, not the song name (a1
 * is the music.inf section name).
 *
 * @param songName  the music base path (cMusicSys::mBasePath)
 * @param sectionName  the song's section name
 * @param config  the song's cSongConfiguration
 * @param playOpts  the cBankMonitor play options (cSndPlayOpts part)
 * @param baseVol  the base volume
 * @param volControl  an optional volume control, or 0
 * @param bankIndex  the cBankManager bank used for the loop overlay
 * @param syncLoad  nonzero to load and open the streams at once
 * @return the new cSongInstance
 */
extern "C" void* cSongInstance_Create(int songName, int sectionName, int config, int playOpts, char baseVol, int volControl, int bankIndex, int syncLoad)
{
    return cSongInstance_cSongInstance(cMemMan_alloc(196, str_SongInstance, 0, 0), songName, sectionName, config, playOpts, baseVol, volControl, bankIndex, syncLoad);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_cSongInstance);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* bmNew_1C50(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int PATH_getPlayerMemSize(int a, int b);
extern "C" void cMusicSys_LoadMPFFile(cMusicSys* self, void* name, int sync);
extern "C" void cSongInstance_Update(cSongInstance*, int);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_PathfinderStream[];
extern char str_PathfinderOverlay[];
extern char str_audio_music_big[];
extern char str_audio_music2_big[];
extern char str_pct_spct_spct_s_b[];
extern char str_data_b[];
extern char str_pct_spct_s_c[];

// The constructor's view of the play options (SNDPLAYOPTS, ealib/snd.h): vol and lowpasscutoff are On Tour's [O]
// member names at the same offsets.
struct sBankEntry_1C50 {
    unsigned char vol;              // 0x0
    char pad1[0x13];
    unsigned short lowpasscutoff;   // 0x14
    short pad16;
};

struct sBankDef_1C50 {
    char pad00[0x1C];
    volatile int cur;           // 0x1C
    sBankEntry_1C50* entries;   // 0x20
    char pad24[0x14];
    int* vols;                  // 0x38
};

// The constructor's view of the cSongInstance it builds; the fields carry the class's member names at the same offsets
// (include/sound/songinstance.h has the evidence for each).
struct sBankMon_1C50 {
    cSongConfiguration* mpConfig;   // 0x0
    sBankEntry_1C50 mPlayOpts;      // 0x4
    int mHandle;                    // 0x1C
    int mOverlayHandle;             // 0x20
    void* mpPathfinderStream;       // 0x24
    void* mpPathfinderOverlay;      // 0x28
    int mStreamsOpen;               // 0x2C
    int mPendingEvent;              // 0x30
    int mBankIndex;                 // 0x34
    int mLoopBankState;             // 0x38
    int mpVolumeControl;            // 0x3C (a float* in the class)
    char mBaseVolume;               // 0x40
    int mLowPassCutoff;             // 0x44
    int mFadingOut;                 // 0x48
    int mOverlayArmed;              // 0x4C
    int mOverlayRequested;          // 0x50
    int mVolumeEnabled;             // 0x54
    int mSongTag;                   // 0x58
    int mHoldEvents;                // 0x5C
    char mPath[0x40];               // 0x60
};

#define BM_MIN_1C50(a, b) ((a) < (b) ? (a) : (b))
#define BM_MAX_1C50(a, b) ((a) >= (b) ? (a) : (b))

static inline sBankEntry_1C50* bankCur_1C50(sBankDef_1C50* d)
{
    return &d->entries[d->cur];
}

// PORT: callers declare the first parameter as void*; the body takes the typed object (bound by asm label)
extern "C" void* cSongInstance_cSongInstance_body(sBankMon_1C50* self, int a0, int a1, int a2, int a3, char a4, int a5, int a6, int a7) __asm__("cSongInstance_cSongInstance");

/**
 * Constructs a cSongInstance (0xC4): the playing song's streams, buffers and settings.
 *
 * Initialises the fields (mHandle/mPendingEvent/mSongTag -1, mVolumeEnabled 1, others 0), stores the bank index
 * (mBankIndex), volume control (mpVolumeControl), base volume (mBaseVolume) and the current play options' low-pass
 * cutoff (mLowPassCutoff), and allocates the 0x37000-byte "PathfinderStream" and the PATH_getPlayerMemSize(1, 0)-byte
 * "PathfinderOverlay" buffers. Scales mBaseVol[mInst] by the song's mPathlevel / 100 (clamped 0..127) into the play
 * level's vol and copies that level into mPlayOpts.
 *
 * mPath becomes "data/" + "audio/music.big" (mSongBig 1) or "audio/music2.big" + a0. Loads the MPF file
 * "<mPath><mPathData>" (cMusicSys_LoadMPFFile) and, with a7 set, opens the streams at once (cSongInstance_Update with force). Note a1
 * is not used.
 *
 * @param basePath  the music base path
 * @param sectionName  the song's section name (not used)
 * @param songConfig  the song's cSongConfiguration
 * @param playOpts  the cBankMonitor play options (cSndPlayOpts part)
 * @param baseVol  the base volume
 * @param volControl  an optional volume control, or 0
 * @param bankIndex  the cBankManager bank index for the overlay
 * @param syncLoad  nonzero to load synchronously and open the streams
 * @return self
 */
extern "C" void* cSongInstance_cSongInstance_body(sBankMon_1C50* self, int basePath, int sectionName, int songConfig, int playOpts, char baseVol, int volControl, int bankIndex, int syncLoad)
{
    cSongConfiguration* snd = (cSongConfiguration*)songConfig;
    sBankDef_1C50* def = (sBankDef_1C50*)playOpts;
    self->mHandle = -1;
    self->mpPathfinderStream = 0;
    self->mpPathfinderOverlay = 0;
    self->mStreamsOpen = 0;
    self->mPendingEvent = -1;
    self->mLoopBankState = 0;
    self->mBankIndex = bankIndex;
    self->mpVolumeControl = volControl;
    self->mBaseVolume = baseVol;
    self->mFadingOut = 0;
    self->mOverlayArmed = 0;
    self->mOverlayRequested = 0;
    self->mVolumeEnabled = 1;
    self->mSongTag = -1;
    self->mHoldEvents = 0;
    self->mLowPassCutoff = bankCur_1C50(def)->lowpasscutoff;
    self->mpPathfinderStream = bmNew_1C50(0x37000, str_PathfinderStream, 0, 0);
    self->mpPathfinderOverlay = bmNew_1C50(PATH_getPlayerMemSize(1, 0), str_PathfinderOverlay, 0, 0);
    float scale = (float)snd->mPathlevel * 0.01f;
    def->vols[def->cur] = (int)BM_MAX_1C50(BM_MIN_1C50((float)def->vols[def->cur] * scale, 127.0f), 0.0f);
    def->entries[def->cur].vol = BM_MAX_1C50(BM_MIN_1C50(def->vols[def->cur], 127), 0);
    if (snd->mSongBig == cSongConfiguration::kMusicBig) {
        sprintf(self->mPath, str_pct_spct_spct_s_b, str_data_b, str_audio_music_big, basePath);
    } else {
        sprintf(self->mPath, str_pct_spct_spct_s_b, str_data_b, str_audio_music2_big, basePath);
    }
    self->mpConfig = snd;
    self->mPlayOpts = *bankCur_1C50(def);
    char buf[0x100];
    sprintf(buf, str_pct_spct_s_c, self->mPath, snd->mPathData);
    gMusicSys->LoadMPFFile(buf, syncLoad);
    if (syncLoad != 0) {
        ((cSongInstance*)self)->Update(1);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", _$_13cSongInstance);
#ifdef SKIP_ASM
extern "C" void cAsyncSys_ASYNCSYS_Cancel(void* self, int i);
extern "C" int PATH_closeVoice(int id);
void cMemMan_free(void*);
void operator_delete(int* ptr);

/**
 * Destroys a cSongInstance: cancels its overlay bank load, closes its Pathfinder streams and frees its buffers.
 *
 * Cancels the async load of bank mBankIndex (cAsyncSys_ASYNCSYS_Cancel), closes the main path stream (PATH_closeVoice
 * with 0x11000001) and, when the song has loop banks (mNumBankData > 0), the overlay (0x11000002), and frees the stream
 * and overlay buffers (mpPathfinderStream, mpPathfinderOverlay). Called with 3 by cMusicSys_StopSong and the cMusicSys
 * destructor.
 */
cSongInstance::~cSongInstance()
{
    cAsyncSys_ASYNCSYS_Cancel(*((void**)gMusicSys->mpVStreamSys), mBankIndex);
    PATH_closeVoice(0x11000001);
    if (mpConfig->mNumBankData > 0) {
        PATH_closeVoice(0x11000002);
    }
    if (mpPathfinderStream != 0) {
        cMemMan_free(mpPathfinderStream);
    }
    if (mpPathfinderOverlay != 0) {
        cMemMan_free(mpPathfinderOverlay);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_Pause);
#ifdef SKIP_ASM
extern "C" int cSongInstance_GetStreamHandle(cSongInstance* self);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSTRM_pitchmult(int h, int v);
extern "C" void PATH_setPaused(int cmd, int v);

/**
 * Pauses a song: sets its main stream's pitch multiplier to 0 and pauses the overlay (PATH_setPaused(.., 1)).
 *
 * The pitch change runs inside the critical section and only when the stream has a handle (cSongInstance_GetStreamHandle).
 */
void cSongInstance::Pause()
{
    SNDSYS_entercritical();
    int handle = GetStreamHandle();
    if (handle >= 0) {
        SNDSTRM_pitchmult(handle, 0);
    }
    SNDSYS_leavecritical();
    PATH_setPaused(0x11000002, 1);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_Resume);
#ifdef SKIP_ASM
extern "C" int cSongInstance_GetStreamHandle(cSongInstance* self);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSTRM_pitchmult(int h, int v);
extern "C" void PATH_setPaused(int cmd, int v);

/**
 * Resumes a song paused by cSongInstance_Pause: pitch multiplier 0x1000 and PATH_setPaused(0x11000002, 0).
 */
void cSongInstance::Resume()
{
    SNDSYS_entercritical();
    int handle = GetStreamHandle();
    if (handle >= 0) {
        SNDSTRM_pitchmult(handle, 0x1000);
    }
    SNDSYS_leavecritical();
    PATH_setPaused(0x11000002, 0);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_SendMusicEvent);
#ifdef SKIP_ASM
extern "C" void PATH_postEvent(int id, short v);

/**
 * Sends a music event to the song's Pathfinder players (PATH_postEvent(-1, v)), or parks it until they open.
 *
 * Does nothing while mHoldEvents is set unless force is. Without open streams (mStreamsOpen 0) the event is kept in
 * mPendingEvent (only when none is parked yet); cSongInstance_Update sends it with force after opening them. Reached from
 * cMusicSys's slot 3 (cMusicSys_SendMusicEvent).
 *
 * @param musicEvent  the music event (the cMusicSys state code)
 * @param force  nonzero to send even while mHoldEvents is set
 */
void cSongInstance::SendMusicEvent(int musicEvent, int force)
{
    if (mHoldEvents != 0 && force == 0) {
        return;
    }
    if (mStreamsOpen != 0) {
        PATH_postEvent(-1, musicEvent);
    } else if (mPendingEvent == cSongInstance::kNoPendingEvent) {
        mPendingEvent = musicEvent;
    }
}
#endif

extern "C" void PATH_setVolume(unsigned int);

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_SetVolume);
#ifdef SKIP_ASM
/**
 * Calls PATH_setVolume for the main path stream (0x11000001), passing the caller's second argument through.
 *
 * The C declaration has no parameters; cMusicSys_SetVolume reaches it with its value (the callers pass a volume such as 127)
 * still in the second argument register.
 */
extern "C" void cSongInstance_SetVolume()
{
    PATH_setVolume(0x11000001U);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2140);
#ifdef SKIP_ASM
extern "C" void PATH_setVoicePosition(int);

/**
 * Calls PATH_setVoicePosition for the main path stream (0x11000001), passing the caller's second argument through.
 *
 * The C declaration has no parameters; func_002B3C60 reaches it with its value still in the second argument register.
 */
extern "C" void func_002B2140(void) {
    PATH_setVoicePosition(0x11000001);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_SetLowPass);
#ifdef SKIP_ASM
extern "C" int cSongInstance_GetStreamHandle(cSongInstance* self);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSTRM_setLowPass(int h, int v);

/**
 * Sets the low-pass cutoff of the song's main stream (SNDSTRM_setLowPass), clamped to 0..0xFFFF, when it changed.
 *
 * The cutoff is cached in mLowPassCutoff (only when the stream has a handle) and applied inside the critical section.
 *
 * @param lowPass  the cutoff
 */
void cSongInstance::SetLowPass(int lowPass)
{
    if (lowPass > 0xFFFF) {
        lowPass = 0xFFFF;
    }
    if (lowPass < 0) {
        lowPass = 0;
    }
    if (lowPass != mLowPassCutoff) {
        SNDSYS_entercritical();
        int streamHandle = GetStreamHandle();
        if (streamHandle >= 0) {
            mLowPassCutoff = lowPass;
            SNDSTRM_setLowPass(streamHandle, lowPass);
        }
        SNDSYS_leavecritical();
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_Update);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cSongInstance_SendMusicEvent(cSongInstance* mon, int a1, int a2);
extern "C" void cSongInstance_SetLowPass(cSongInstance* self, int v);
extern "C" int cSongInstance_GetStreamHandle(cSongInstance* self);
extern "C" bool cMusicSys_IsMPFLoaded(cMusicSys* self, int force);
int cMusicSys_GetMPFFile(void* self);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSTRM_setVolume(int h, int v);
extern "C" void func_003D2350(int a);
extern "C" int func_003D25F0(int id, char* name, int a2, void* a3, int a4, int a5, void* buf, int size);
extern char str_pct_spct_s_c[];
extern float gMusicScaling;


/**
 * Updates a song: opens its Pathfinder streams once the MPF file is loaded, then keeps the low-pass and volume set.
 *
 * While the streams are closed (mStreamsOpen 0) it waits for the MPF load (cMusicSys_IsMPFLoaded, unless force), sets
 * mStreamsOpen, passes cMusicSys::mMPFFile to Pathfinder (func_003D2350) and opens the main path stream
 * "<mPath><mMusData>" (func_003D25F0(0x11000001, .., 500, &mPlayOpts, 2, 150, mpPathfinderStream, 0x37000)) and, with
 * loop banks, the overlay "<mPath><mBankData[0]>" (0x11000002, 10 requests, the 0x6A400-byte mpPathfinderOverlay
 * buffer); a parked event (mPendingEvent) is then sent with force (cSongInstance_SendMusicEvent).
 *
 * Once open it applies the song's mLowPass (cSongInstance_SetLowPass) and, unless fading out (mFadingOut) or mVolumeEnabled is 0,
 * sets the stream volume (SNDSTRM_setVolume) to base volume (mBaseVolume) x mPathlevel / 100 x the volume control
 * (mpVolumeControl, when set) x gMusicScaling, clamped to 0..127. Called by the constructor with force and by
 * cMusicSys_UpdateSong.
 *
 * @param mon  the cSongInstance
 * @param force  nonzero to open the streams without waiting for the async MPF load
 */
// PORT: g++ `<?` / `>?` (min/max) operators.
void cSongInstance::Update(int force)
{
    if (mStreamsOpen == 0)
    {
        if (gMusicSys->IsMPFLoaded(force) == 0)
            return;
        mStreamsOpen = 1;
        func_003D2350(gMusicSys->GetMPFFile());
        char buf[256];
        sprintf(buf, str_pct_spct_s_c, mPath, mpConfig->mMusData);
        mHandle = func_003D25F0(0x11000001, buf, 500, &mPlayOpts, 2, 150, mpPathfinderStream, 0x37000);
        if (mpConfig->mNumBankData > 0)
        {
            sprintf(buf, str_pct_spct_s_c, mPath, mpConfig->mBankData[0]);
            mOverlayHandle = func_003D25F0(0x11000002, buf, 10, &mPlayOpts, 1, 0, mpPathfinderOverlay,
                    0x6A400);
        }
        if (mPendingEvent != cSongInstance::kNoPendingEvent)
        {
            SendMusicEvent(mPendingEvent, 1);
            mPendingEvent = cSongInstance::kNoPendingEvent;
        }
        return;
    }
    SetLowPass(mpConfig->mLowPass);
    if (mFadingOut != 0)
        return;
    if (mVolumeEnabled == 0)
        return;
    float volume;
    float* volumeControl = mpVolumeControl;
    if (volumeControl)
        volume = (float)mBaseVolume * 0.009999999776482582f * (float)mpConfig->mPathlevel * *volumeControl;
    else
        volume = (float)mBaseVolume * 0.009999999776482582f * (float)mpConfig->mPathlevel;
    volume *= gMusicScaling;
    SNDSYS_entercritical();
    int streamHandle = GetStreamHandle();
    if (streamHandle >= 0)
    {
        signed char vol = (int)((volume <? 127.0f) >? 0.0f);
        SNDSTRM_setVolume(streamHandle, vol);
    }
    SNDSYS_leavecritical();
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_SetOverlayLevel);
#ifdef SKIP_ASM
extern "C" void PATH_setLevel(int cmd, int v);

/**
 * Sets the song overlay's level (PATH_setLevel(0x11000002, v), 0..100) when mVolumeEnabled is set.
 *
 * @param levelPercent  the level in percent
 */
void cSongInstance::SetOverlayLevel(signed char levelPercent)
{
    if (mVolumeEnabled != 0) {
        PATH_setLevel(0x11000002, levelPercent);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_FadeOut);
#ifdef SKIP_ASM
extern "C" int cSongInstance_GetStreamHandle(cSongInstance* self);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSTRM_fadeVolume(int h, int v, int c);

/**
 * Fades the song's main stream out to 0 over t seconds (SNDSTRM_fadeVolume) and marks it fading (mFadingOut = 1).
 *
 * The fade runs inside the critical section, only when the stream has a handle; cSongInstance_Update stops applying the volume
 * once mFadingOut is set.
 *
 * @param fadeSeconds  the fade time in seconds
 */
void cSongInstance::FadeOut(float fadeSeconds)
{
    SNDSYS_entercritical();
    int streamHandle = GetStreamHandle();
    if (streamHandle >= 0) {
        SNDSTRM_fadeVolume(streamHandle, (int)(fadeSeconds * 1000.0f), 0);
    }
    SNDSYS_leavecritical();
    mFadingOut = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetStreamHandle);
#ifdef SKIP_ASM
extern "C" int PATH_getVoiceMonitor(int id);

/**
 * Returns the song's main stream handle: the first word of the Pathfinder slot playing 0x11000001, or -1.
 *
 * The slot comes from PATH_getVoiceMonitor and pathxSND_getPlayer.
 *
 * @return the stream handle, or -1 when the main stream has no slot
 */
int cSongInstance::GetStreamHandle()
{
    int idx = PATH_getVoiceMonitor(0x11000001);
    if (idx < 0) {
        return -1;
    }
    return pathxSND_getPlayer(idx)->mHandle;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetRandomLoopBank);
#ifdef SKIP_ASM
// PORT: cSoundSys_SndRand is defined () but this caller passes the monitor.
unsigned int cSoundSys_SndRand_p(void* p) __asm__("cSoundSys_SndRand");

/**
 * Returns a random loop-bank index for the song: cSoundSys_SndRand() modulo its config's mNumBankData.
 *
 * cMusicSys_LoadRandomLoopBank uses it to pick the overlay bank.
 *
 * @return the bank index
 */
int cSongInstance::GetRandomLoopBank()
{
    return cSoundSys_SndRand_p(SoundSys) % (unsigned int)mpConfig->mNumBankData;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetRandomPhraseStart);
#ifdef SKIP_ASM
/**
 * Returns a random phrase start for the song: (SndRand % (mPhrasesPerBank - 1)) x mPhraseAlign.
 *
 * Used as mData1 of the beat-synchronised overlay callback armed by cMusicSys_ArmBeatCallback.
 *
 * @return the phrase start
 */
int cSongInstance::GetRandomPhraseStart()
{
    cSongConfiguration* config;
    unsigned int randVal = cSoundSys_SndRand_p(SoundSys);
    config = mpConfig;
    return (randVal % (unsigned int)(config->mPhrasesPerBank - 1)) * config->mPhraseAlign;
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetPathStatus);
#ifdef SKIP_ASM
extern "C" void PATH_status(int, void*);

/**
 * Returns word 9 of the main path stream's status (PATH_status(0x11000001)).
 *
 * cSSXAudio_PostMusicRequest treats -1 as no playing stream.
 *
 * @return the status word
 */
extern "C" int cSongInstance_GetPathStatus(void)
{
    int buf[10];
    PATH_status(0x11000001, buf);
    return buf[9];
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetStreamStatus);
#ifdef SKIP_ASM
extern "C" int cSongInstance_GetStreamHandle(cSongInstance* self);
extern "C" int SNDSTRM_status(int h, void* out);

// PORT: returns int; a later caller in this unit declares it void(void*, void*)
int cSongInstance_GetStreamStatus_impl(void* self, void* out) __asm__("cSongInstance_GetStreamStatus");

/**
 * Reads the main stream's status (SNDSTRM_status) into out, or returns -1 when the song has no stream handle.
 *
 * @param out  receives the stream status
 * @return SNDSTRM_status's result, or -1
 */
int cSongInstance::GetStreamStatus(void* out)
{
    return ((cSongInstance*)this)->GetStreamHandle() < 0 ? -1 : SNDSTRM_status(
            ((cSongInstance*)this)->GetStreamHandle(), out);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetRequestStatus);
#ifdef SKIP_ASM
extern "C" void cSongInstance_GetStreamStatus(void* self, void* out);
extern "C" void SNDSTRM_requeststatus(int h, void* status);

struct sBmInfo {
    int a;
    int handle;
    int c;
    int d;
};

/**
 * Reads the status of the song stream's current request (SNDSTRM_requeststatus) into the given buffer.
 *
 * Gets the stream status first (cSongInstance_GetStreamStatus) and uses its second word as the request handle.
 *
 * @param requestStatus  receives the request status (type, position, length, ...)
 */
void cSongInstance::GetRequestStatus(void* requestStatus)
{
    sBmInfo info;
    cSongInstance_GetStreamStatus(this, &info);
    SNDSTRM_requeststatus(info.handle, requestStatus);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetPlayPosition);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.


/**
 * Returns the song's play position in ms: the current request's position while it plays (kSNDREQUEST_Playing), else 0.
 *
 * Reads the request status through cSongInstance_GetRequestStatus.
 *
 * @return the position
 */
float cSongInstance::GetPlayPosition()
{
    SNDREQUESTSTATUS info;
    GetRequestStatus(&info);
    if (info.state == kSNDREQUEST_Playing) {
        return (float)info.currenttime;
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetTimeToEnd);
#ifdef SKIP_ASM

extern "C" float cSongInstance_GetBarLength(cSongInstance* self);

// PORT: the unit later declares cSongInstance_GetTimeToEnd() as void() for a caller; real signature is float(cSongInstance*).
float cSongInstance_GetTimeToEnd_impl(cSongInstance* self) __asm__("cSongInstance_GetTimeToEnd");
/**
 * Returns the current request's length while it plays (kSNDREQUEST_Playing), otherwise one bar's length
 * (cSongInstance_GetBarLength).
 *
 * @return the length in ms
 */
float cSongInstance::GetTimeToEnd()
{
    SNDREQUESTSTATUS info;
    GetRequestStatus(&info);
    if (info.state == kSNDREQUEST_Playing) {
        return (float)info.timetoend;
    }
    return GetBarLength();
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetBeatLength);
#ifdef SKIP_ASM
/**
 * Returns the song's beat length in ms: 60 / mBPM x 1000.
 *
 * @return the beat length
 */
float cSongInstance::GetBeatLength()
{
    cSongConfiguration* config = mpConfig;
    return (60.0f / config->mBPM) * 1000.0f;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetBarLength);
#ifdef SKIP_ASM
/**
 * Returns the song's bar length in ms: beat length (cSongInstance_GetBeatLength) x mBeatsPerMeasure x mMeasuresPerBar.
 *
 * @return the bar length
 */
float cSongInstance::GetBarLength()
{
    float beatMs = GetBeatLength();
    cSongConfiguration* config = mpConfig;
    return beatMs * (float)config->mBeatsPerMeasure * (float)config->mMeasuresPerBar;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B26F8);
#ifdef SKIP_ASM
extern "C" void cSongInstance_GetTimeToEnd();

/**
 * Calls cSongInstance_GetTimeToEnd with the caller's arguments still in the registers and discards the result.
 */
extern "C" void func_002B26F8(void) {
    cSongInstance_GetTimeToEnd();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetTimeToNextBeat);
#ifdef SKIP_ASM
extern "C" float cSongInstance_GetPlayPosition(cSongInstance* self);
extern "C" float fmodf(float x, float y);

/**
 * Returns the time to the song's next beat: beat length minus the position modulo the beat length (fmodf).
 *
 * @return the time in ms
 */
float cSongInstance::GetTimeToNextBeat()
{
    float beatLength = GetBeatLength();
    float playPosition = GetPlayPosition();
    return beatLength - fmodf(playPosition, GetBeatLength());
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetCurrentBeat);
#ifdef SKIP_ASM
/**
 * Returns the song's current beat number: play position (cSongInstance_GetPlayPosition) / beat length (cSongInstance_GetBeatLength).
 *
 * @return the beat index
 */
int cSongInstance::GetCurrentBeat()
{
    return (int)(GetPlayPosition() / GetBeatLength());
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_IsLoopBankReady);
#ifdef SKIP_ASM
extern "C" int cBankManager_BANK_Handle(void*, int);
extern "C" int SNDbankinuse(int);

/**
 * Returns whether the song's loop bank is ready: mOverlayArmed set, or bank mBankIndex loaded (USED) and in use.
 *
 * With mOverlayArmed clear it checks inside the critical section that the cBankManager's bank record is USED and that
 * SNDbankinuse(cBankManager_BANK_Handle(..)) is true (a voice plays from it).
 *
 * @param other  the cBankManager
 * @return true when ready, else false
 */
bool cSongInstance::IsLoopBankReady(cBankManager* other)
{
    if (mOverlayArmed == 0) {
        int slot = mBankIndex;
        int ok = 0;
        SNDSYS_entercritical();
        bool notReady = other->mBank[slot].mBankStatus != USED;
        if (!notReady) {
            ok = SNDbankinuse(cBankManager_BANK_Handle(other, slot)) == true;
        }
        SNDSYS_leavecritical();
        if (ok == 0) {
            return false;
        }
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_ApplyDelay);
#ifdef SKIP_ASM
struct sBMVec16 {
    int x;
    int y;
    int z;
    int w;
};

struct sBMTrack {
    char pad0[0x1F4];
    int cur;
    char pad1F8[0x248 - 0x1F8];
    sBMVec16* vecs;
};

struct sBMSrc {
    char pad0[0x78];
    int a;
    int b;
    int c;
};

/**
 * Copies a song's delay settings (mDelayCount, mDelayFeedback, mDelayTime) into the current play-option level.
 *
 * Writes the level's delay record in the cBankMonitor play options (+0x248 array, current level at +0x1F4): second word
 * = mDelayCount, third = mDelayFeedback, first = mDelayTime; mDelayLevel is not used. Called as cSongInstance_ApplyDelay(mCurrent,
 * cMusicSys) from seg_1BA100.
 *
 * @param src  the cSongInstance (its first word is the cSongConfiguration)
 * @param dst  the cMusicSys (its first word leads to the cBankManager)
 */
extern "C" void cSongInstance_ApplyDelay(sBMSrc** src, sBMTrack*** dst)
{
    (**dst)->vecs[(**dst)->cur].y = (*src)->a;
    (**dst)->vecs[(**dst)->cur].z = (*src)->b;
    (**dst)->vecs[(**dst)->cur].x = (*src)->c;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_cMusicSys);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
class cController;
class cBankMonitorInstance;
// Sound callback: a g++ 2.95 pointer to member function (8 bytes), as cSndPlayOpts::OPTS_Callback takes it
// (same type as sound/soundsys).
typedef void (cController::*sSndCallback)(cController*, cBankMonitorInstance*, float);
// 4-entry vtable image (the hand-expanded virtual-base ctors / dtors copy one to the stack and patch a delta).
struct sSndVtbl4 {
    sSndVtSlot e[4];
} __attribute__((aligned(8)));

// 7-entry vtable image (same use as sSndVtbl4).
struct sSndVtbl7 {
    sSndVtSlot e[7];
} __attribute__((aligned(8)));

// Virtual-base vtable images patched by the ctors / dtors below.
extern void* cMusicSys_vtbl[] __asm__("cMusicSys_vtbl");
extern const sSndVtbl4 D_00483928_v __asm__("D_00483928");
extern const sSndVtbl7 D_00483948_v __asm__("D_00483948");
// One entry of cSndPlayOpts' option stack (mCurrent[], 0x18 bytes); byte 0 seeds mBaseVol.
// One cSndPlayOpts mDelay[] entry (reset to 100 / 0 / 90 / 50 on every push).
// cSndPlayOpts (prototype class, as in sound/soundsys): the play-option stack; push = cur++ and copy def into
// ch[cur]. Field names follow sound/soundsys (ch = mCurrent, vals = mBaseVol, p70 = mDelay, ...).
// One of a sound player's 48 monitored-sound slots (0x2C bytes).
// Sound player: a cSndPlayOpts (0x88 bytes) plus 48 monitored-sound slots; virtual base at 0x8DC.
// cBankManager as this unit sees it (as in sound/soundsys): only its sound player at 0x1D8 is used here.
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSYS_entercritical(void);
extern void* D_004A37E8;
extern char str_GLOBAL[];
extern char str_TITLE[];
extern char str_ARTIST[];
extern char str_ALBUM[];
extern char str_ADDTOFE[];
extern char str_CATEGORY[];
extern char str_PATHDATA[];
extern char str_MUSDATA[];
extern char str_LOOPDATA[];
extern char str_BeatsPerMeasure[];
extern char str_MeasuresPerBar[];
extern char str_PhrasesPerBank[];
extern char str_BeatsPerPhrase[];
extern char str_PhraseAlign[];
extern char str_DelayCount[];
extern char str_DelayFeedback[];
extern char str_DelayTime[];
extern char str_DelayLevel[];
extern char str_Pathlevel[];
extern char str_Asynclevel[];
extern char str_BPM[];
extern char str_SEDVALUE[];
extern char str_LOWPASS[];
extern char str_DUCKTOLOOPS[];
extern char str_PREVIEW[];
extern char str_SONGBIG[];
extern const char str_MPF_data_file[];
extern const char str_cSongConfiguration[];
extern "C" void* FileParser_FileParser_p(void* parser, void* src, int n) __asm__("__10FileParserPCci");
extern "C" int FileParser_FindSection(void* parser, const char* name);
extern "C" char* FileParser_FindNextSection(void* parser);
extern "C" int FileParser_AssignInt(void* parser, const char* key, int* out);
extern "C" int FileParser_AssignFloat(void* parser, const char* key, float* out);

struct sLoc28C0 {
    char parser[0x810];
    char buf[0x20];
    int iA;
    int iB;
    int iC;
    float fD;
};

// One row of the per-song category grid (cMusicSys::mCategory).
static inline int* Row_28C0(int (*g)[5], int i)
{
    return g[i];
}

/**
 * Constructs cMusicSys: sets up its sound bases, then reads music.inf into the song list and song configurations.
 *
 * When in charge it constructs the virtual bases (cMusicSysComplete: cSoundSys at +0x5448, cBankManager, cStreamSys)
 * with the bank and stream counts; it installs the cSoundSys and cBankManager vtables (D_00483928, cBankMonitor_vtbl,
 * D_00483948), and when not in charge points the objects at stack copies with adjusted this deltas. Clears the counts,
 * sets the vptr cMusicSys_vtbl and gMusicSys = self, allocates the 25600-byte "MPF data file" buffer (mMPFFile), clears
 * mCategory / mCategoryCount and empties every title, artist and album.
 *
 * Parses music.inf (D_004A37E8): GLOBAL's BASEPATH goes to mBasePath; every other section (up to 64) becomes a song:
 * its bit is set in mActiveMap, its name copied to mSongName, and a cSongConfiguration (0xAC, "cSongConfiguration")
 * reads TITLE, ARTIST, ALBUM, ADDTOFE (counted in mNumFESongs), CATEGORY (< 5 sets mCategory), PATHDATA, MUSDATA,
 * LOOPDATA (appended to mBankData), BeatsPerMeasure ... BPM, SEDVALUE, LOWPASS, DUCKTOLOOPS, PREVIEW and SONGBIG.
 * Called by the cSSXAudio constructor on gSSXAudio + 0x118.
 *
 * @param inchrg  g++ 2.95 in-charge flag (nonzero: also construct the virtual bases)
 * @param maxBanks  the maximum bank count for the bases
 * @param maxStreams  the maximum stream count for cStreamSys
 * @param heap  the memory type for the bases
 * @return self
 */
// The music system's constructor (sound/musicsys.cpp in the July 2003 prototype): reads the song list.
// PORT: g++ 2.95 constructor of a class with virtual bases (stack vtable this-adjust fix-ups written out by hand).
extern "C" char* cMusicSys_cMusicSys(cMusicSys* self, int inchrg, int maxBanks, int maxStreams, int heap)
{
    if (inchrg) {
        cMusicSysComplete* complete = (cMusicSysComplete*)self;
        cSoundSys* vb = &complete->mSoundSys;
        cStreamSys* vb2 = &complete->mStreamSys;
        complete->mStreamSys.mpVBankManager = &complete->mBankManager;
        self->mpVStreamSys = vb2;
        complete->mBankManager.BankMonitor.mpVSoundSys = vb;
        cSoundSys_cSoundSys(vb, heap);
        cBankManager_cBankManager(complete->mStreamSys.mpVBankManager, 0, maxBanks, heap);
        cStreamSys_cStreamSys(self->mpVStreamSys, 0, maxBanks, maxStreams, heap);
    }
    self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&D_00483928_v;
    self->mpVStreamSys->mpVBankManager->BankMonitor.mVtbl = cBankMonitor_vtbl;
    self->mpVStreamSys->mpVBankManager->mVtbl = (void**)&D_00483948_v;
    if (inchrg == 0) {
        sSndVtbl4 soundSysVtbl = D_00483928_v;
        self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&soundSysVtbl;
        char* base1 = (char*)self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys - 0x5448;
        int soundSysDelta = ((char*)self) - base1;
        soundSysVtbl.e[1].delta = D_00483928_v.e[1].delta + soundSysDelta;
        sSndVtbl7 bankManagerVtbl = D_00483948_v;
        self->mpVStreamSys->mpVBankManager->mVtbl = (void**)&bankManagerVtbl;
        char* base2 = (char*)self->mpVStreamSys->mpVBankManager - 0x5450;
        int bankManagerDelta = ((char*)self) - base2;
        bankManagerVtbl.e[1].delta = D_00483948_v.e[1].delta + bankManagerDelta;
    }
    self->mNumSongs = 0;
    self->mNumActiveSongs = 0;
    self->mVtbl = cMusicSys_vtbl;
    self->mNumFESongs = 0;
    self->mCurrentSongNum = 0;
    *(long*)&self->mActiveMap = 0;
    self->mSongInstance = 0;
    self->mSongPlaying = 0;
    self->mpMpfFile = 0;
    self->mShortcut = 0;
    gMusicSys = self;
    self->mMPFFile = new (str_MPF_data_file, 0, 0) char[25600];
    int i = 0;
    do {
        int* categoryInts = Row_28C0((self)->mCategory, i);
        for (int j = 4; j >= 0; j--)
            categoryInts[j] = 0;
        i++;
    } while (i < 0x40);
    {
        int* categoryInts = (int*)(&self->mCategoryCount[0]);
        for (int j = 4; j >= 0; j--)
            categoryInts[j] = 0;
    }
    for (int textOffset = 0; textOffset < 6400; textOffset += 100) {
        ((char*)self->mDisplaySongName)[textOffset] = str_empty_j[0];
        ((char*)self->mDisplaySongArtist)[textOffset] = str_empty_j[0];
        ((char*)self->mDisplaySongAlbum)[textOffset] = str_empty_j[0];
    }
    sLoc28C0 musicInf;
    FileParser_FileParser_p(musicInf.parser, D_004A37E8, 0x100);
    *(int*)(musicInf.parser + 8) = *(int*)(musicInf.parser + 4);
    if (FileParser_FindSection(musicInf.parser, str_GLOBAL) != 0) {
        while (FileParser_IsEOF(musicInf.parser) == 0) {
            FileParser_GetName(musicInf.parser);
            FileParser_AssignString(musicInf.parser, str_BASEPATH, self->mBasePath);
        }
    }
    *(int*)(musicInf.parser + 8) = *(int*)(musicInf.parser + 4);
    self->mNumSongs = 0;
    self->mNumFESongs = 0;
    while (self->mNumSongs < 0x40) {
        char* tok = FileParser_FindNextSection(musicInf.parser);
        if (tok == 0)
            break;
        if (strcasecmp(tok, str_GLOBAL) == 0)
            continue;
        // The song's bit is built in a variable: the target keeps its li 1 in the loop while the
        // other constants (100) are hoisted.
        long bit = 1;
        bit <<= self->mNumSongs;
        *(long*)&self->mActiveMap |= bit;
        strcpy(self->mSongName[self->mNumSongs], tok);
        self->mSongConfig[self->mNumSongs] = (cSongConfiguration*)cSongConfiguration_cSongConfiguration(
            (cSongConfiguration*)cMemMan_alloc(172, str_cSongConfiguration, 0, 0));
        while (FileParser_IsEOF(musicInf.parser) == 0) {
            char* key = FileParser_GetName(musicInf.parser);
            if (strcasecmp(key, str_TITLE) == 0) {
                FileParser_AssignString(musicInf.parser, str_TITLE, self->mDisplaySongName[self->mNumSongs]);
            } else if (strcasecmp(key, str_ARTIST) == 0) {
                FileParser_AssignString(musicInf.parser, str_ARTIST, self->mDisplaySongArtist[self->mNumSongs]);
            } else if (strcasecmp(key, str_ALBUM) == 0) {
                FileParser_AssignString(musicInf.parser, str_ALBUM, self->mDisplaySongAlbum[self->mNumSongs]);
            } else if (strcasecmp(key, str_ADDTOFE) == 0) {
                FileParser_AssignInt(musicInf.parser, str_ADDTOFE, &musicInf.iA);
                if (musicInf.iA == true)
                    self->mNumFESongs += 1;
            } else if (strcasecmp(key, str_CATEGORY) == 0) {
                FileParser_AssignInt(musicInf.parser, str_CATEGORY, &musicInf.iB);
                if (musicInf.iB < 5)
                    (self)->mCategory[(self)->mNumSongs][musicInf.iB] = 1;
            } else if (strcasecmp(key, str_PATHDATA) == 0) {
                FileParser_AssignString(musicInf.parser, str_PATHDATA, musicInf.buf);
                strcpy(self->mSongConfig[self->mNumSongs]->mPathData, musicInf.buf);
            } else if (strcasecmp(key, str_MUSDATA) == 0) {
                FileParser_AssignString(musicInf.parser, str_MUSDATA, musicInf.buf);
                strcpy(self->mSongConfig[self->mNumSongs]->mMusData, musicInf.buf);
            } else if (strcasecmp(key, str_LOOPDATA) == 0) {
                FileParser_AssignString(musicInf.parser, str_LOOPDATA, musicInf.buf);
                strcpy(self->mSongConfig[self->mNumSongs]->mBankData[self->mSongConfig[self->mNumSongs]->mNumBankData],
                       musicInf.buf);
                self->mSongConfig[self->mNumSongs]->mNumBankData++;
            } else if (strcasecmp(key, str_BeatsPerMeasure) == 0) {
                FileParser_AssignInt(musicInf.parser, str_BeatsPerMeasure, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mBeatsPerMeasure = musicInf.iC;
            } else if (strcasecmp(key, str_MeasuresPerBar) == 0) {
                FileParser_AssignInt(musicInf.parser, str_MeasuresPerBar, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mMeasuresPerBar = musicInf.iC;
            } else if (strcasecmp(key, str_PhrasesPerBank) == 0) {
                FileParser_AssignInt(musicInf.parser, str_PhrasesPerBank, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mPhrasesPerBank = musicInf.iC;
            } else if (strcasecmp(key, str_BeatsPerPhrase) == 0) {
                FileParser_AssignInt(musicInf.parser, str_BeatsPerPhrase, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mBeatsPerPhrase = musicInf.iC;
            } else if (strcasecmp(key, str_PhraseAlign) == 0) {
                FileParser_AssignInt(musicInf.parser, str_PhraseAlign, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mPhraseAlign = musicInf.iC;
            } else if (strcasecmp(key, str_DelayCount) == 0) {
                FileParser_AssignInt(musicInf.parser, str_DelayCount, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mDelayCount = musicInf.iC;
            } else if (strcasecmp(key, str_DelayFeedback) == 0) {
                FileParser_AssignInt(musicInf.parser, str_DelayFeedback, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mDelayFeedback = musicInf.iC;
            } else if (strcasecmp(key, str_DelayTime) == 0) {
                FileParser_AssignInt(musicInf.parser, str_DelayTime, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mDelayTime = musicInf.iC;
            } else if (strcasecmp(key, str_DelayLevel) == 0) {
                FileParser_AssignInt(musicInf.parser, str_DelayLevel, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mDelayLevel = musicInf.iC;
            } else if (strcasecmp(key, str_Pathlevel) == 0) {
                FileParser_AssignInt(musicInf.parser, str_Pathlevel, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mPathlevel = musicInf.iC;
            } else if (strcasecmp(key, str_Asynclevel) == 0) {
                FileParser_AssignInt(musicInf.parser, str_Asynclevel, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mAsynclevel = musicInf.iC;
            } else if (strcasecmp(key, str_BPM) == 0) {
                FileParser_AssignFloat(musicInf.parser, str_BPM, &musicInf.fD);
                self->mSongConfig[self->mNumSongs]->mBPM = musicInf.fD;
            } else if (strcasecmp(key, str_SEDVALUE) == 0) {
                FileParser_AssignInt(musicInf.parser, str_SEDVALUE, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mSedValue = musicInf.iC;
            } else if (strcasecmp(key, str_LOWPASS) == 0) {
                FileParser_AssignInt(musicInf.parser, str_LOWPASS, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mLowPass = musicInf.iC;
            } else if (strcasecmp(key, str_DUCKTOLOOPS) == 0) {
                FileParser_AssignInt(musicInf.parser, str_DUCKTOLOOPS, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mDuckToLoops = musicInf.iC;
            } else if (strcasecmp(key, str_PREVIEW) == 0) {
                FileParser_AssignInt(musicInf.parser, str_PREVIEW, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mPreview = musicInf.iC;
            } else if (strcasecmp(key, str_SONGBIG) == 0) {
                FileParser_AssignInt(musicInf.parser, str_SONGBIG, &musicInf.iC);
                self->mSongConfig[self->mNumSongs]->mSongBig = musicInf.iC;
            }
        }
        self->mNumSongs += 1;
    }
    _$_10FileParser(musicInf.parser, 2);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys__cMusicSys);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void cBankManager__cBankManager(void* self, int flags);
extern "C" void cStreamSys__cStreamSys(void* self, int flags);
extern "C" void cSoundSys__cSoundSys(cSoundSys* self, int flags);
extern "C" void _$_13cSongInstance(cSongInstance* self, int flags);
extern void* cMusicSys_vtbl[];
extern void* cBankMonitor_vtbl[];

struct sBmonVEntry3398 {
    short delta;
    short index;
    void* fn;
};
struct sBmonVtblA3398 {
    sBmonVEntry3398 e[4];
} __attribute__((aligned(8)));
struct sBmonVtblB3398 {
    sBmonVEntry3398 e[7];
} __attribute__((aligned(8)));
extern const sBmonVtblA3398 D_00483928;
extern const sBmonVtblB3398 D_00483948;

struct sBmonVcall3398 { short delta; short index; void (*fn)(void*, int); };

/**
 * Destroys cMusicSys: deletes the song configurations, the MPF buffer and the current song, then the bases.
 *
 * Restores the vtables as the constructor does (stack copies when flags is 0), calls each song configuration's virtual
 * destructor with 3, frees mMPFFile, destroys mSongInstance (cSongInstance::~cSongInstance with 3) and clears gMusicSys. With bit 1 of flags
 * it destroys cStreamSys (cStreamSys__cStreamSys), cBankManager and cSoundSys; bit 0 deletes the object.
 *
 * @param flags  g++ 2.95 destructor flags (bit 1: destroy the virtual bases, bit 0: operator delete)
 */
// PORT: g++ 2.95 virtual-base destruction with stack vtable this-adjust fix-ups, written out by hand.
extern "C" void cMusicSys__cMusicSys(cMusicSys* self, int flags)
{
    self->mVtbl = cMusicSys_vtbl;
    self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&D_00483928;
    self->mpVStreamSys->mpVBankManager->BankMonitor.mVtbl = cBankMonitor_vtbl;
    self->mpVStreamSys->mpVBankManager->mVtbl = (void**)&D_00483948;
    if (flags == 0) {
        sBmonVtblA3398 soundSysVtbl = D_00483928;
        self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys->mVtbl = (void**)&soundSysVtbl;
        char* base1 = (char*)self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys - 0x5448;
        int soundSysDelta = ((char*)self) - base1;
        soundSysVtbl.e[1].delta = D_00483928.e[1].delta + soundSysDelta;
        sBmonVtblB3398 bankManagerVtbl = D_00483948;
        self->mpVStreamSys->mpVBankManager->mVtbl = (void**)&bankManagerVtbl;
        char* base2 = (char*)self->mpVStreamSys->mpVBankManager - 0x5450;
        int bankManagerDelta = ((char*)self) - base2;
        bankManagerVtbl.e[1].delta = D_00483948.e[1].delta + bankManagerDelta;
    }
    for (int i = 0; i < self->mNumSongs; i++) {
        cSongConfiguration* songConfig = self->mSongConfig[i];
        if (((char*)songConfig) != 0) {
            sBmonVcall3398* dtorSlot = &((sBmonVcall3398*)songConfig->mVtbl)[1];
            dtorSlot->fn(vthis(songConfig, *dtorSlot), 3);
        }
    }
    if (self->mMPFFile != 0) {
        cMemMan_free(self->mMPFFile);
    }
    if (self->mSongInstance != 0) {
        _$_13cSongInstance((cSongInstance*)(self->mSongInstance), 3);
    }
    gMusicSys = 0;
    if (flags & 2) {
        cStreamSys__cStreamSys(self->mpVStreamSys, 0);
        cBankManager__cBankManager(self->mpVStreamSys->mpVBankManager, 0);
        cSoundSys__cSoundSys(self->mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys, 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_PlaySong);
#ifdef SKIP_ASM
extern "C" void cMusicSys_StartSong(cMusicSys* self, int a1, int a2, int a3);

/**
 * cMusicSys virtual slot 2: plays a song with a volume control on its own play-option level (cMusicSys_StartSong).
 *
 * Pushes a level as cSndPlayOpts_OPTS_Open does, stores the volume control in it, starts the song and pops the level.
 * cSSXAudio's override cSSXAudio_PlaySong calls it; callers pass the song name, bank 12 and music volume control 1.
 *
 * @param songName  the song (section) name, or 0 for mCurrentSongNum
 * @param bankIndex  the cBankManager bank for its loop overlay
 * @param volumeControl  the volume control (float*)
 * @param syncLoad  nonzero to load and open the streams at once
 */
void cMusicSys::PlaySong(int songName, int bankIndex, int volumeControl, int syncLoad)
{
    cBankManager* bankManager = mpVStreamSys->mpVBankManager;
    cBankMonitor* playOpts = &bankManager->BankMonitor;
    bankManager->BankMonitor.mInst++;
    playOpts->mCurrent[bankManager->BankMonitor.mInst] = playOpts->mDefault;
    bankManager->BankMonitor.mPosition[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPosStatic[bankManager->BankMonitor.mInst] = 0;
    playOpts->mBaseVol[bankManager->BankMonitor.mInst] = playOpts->mCurrent[bankManager->BankMonitor.mInst].vol;
    playOpts->mVolControl[bankManager->BankMonitor.mInst] = 0;
    memset((char*)playOpts + bankManager->BankMonitor.mInst * 8 + 0x40, 0, 8);
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
    cBankManager* volManager = mpVStreamSys->mpVBankManager;
    volManager->BankMonitor.mVolControl[volManager->BankMonitor.mInst] = (float*)volumeControl;
    StartSong(songName, bankIndex, syncLoad);
    cBankManager* popManager = mpVStreamSys->mpVBankManager;
    popManager->BankMonitor.mInst--;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_StartSong);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
class cController;
class cBankMonitorInstance;
// Sound callback: a g++ 2.95 pointer to member function (8 bytes), as cSndPlayOpts::OPTS_Callback takes it
// (same type as sound/soundsys).
typedef void (cController::*sSndCallback)(cController*, cBankMonitorInstance*, float);
// One entry of cSndPlayOpts' option stack (mCurrent[], 0x18 bytes); byte 0 seeds mBaseVol.
// One cSndPlayOpts mDelay[] entry (reset to 100 / 0 / 90 / 50 on every push).
// cSndPlayOpts (prototype class, as in sound/soundsys): the play-option stack; push = cur++ and copy def into
// ch[cur]. Field names follow sound/soundsys (ch = mCurrent, vals = mBaseVol, p70 = mDelay, ...).
// One of a sound player's 48 monitored-sound slots (0x2C bytes).
// Sound player: a cSndPlayOpts (0x88 bytes) plus 48 monitored-sound slots; virtual base at 0x8DC.
// cBankManager as this unit sees it (as in sound/soundsys): only its sound player at 0x1D8 is used here.
struct sBankEntry64_3838 {
    char data[0x64];
};

// The speech-bank monitor (cMusicSys__cMusicSys dtor): up to 64 named banks, the selection mask and per-kind counts.
// PORT: 64-bit `long` mask (8 bytes on EE, 4 on Windows); use uint64_t off-PS2.
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSYS_entercritical(void);
extern "C" int strcmp(void*, void*);
extern "C" void cMusicSys_StopSong(cMusicSys* self);
extern "C" void cMusicSys_LoadRandomLoopBank(cMusicSys* self);
extern "C" void* cSongInstance_Create(int a0, int a1, int a2, int a3, char a4, int a5, int a6, int a7);

// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
static inline float bmMin3838(float a, float b)
{
    return a <? b;
}
static inline float bmMax3838(float a, float b)
{
    return a >? b;
}

/**
 * Starts a song: stops the current one and creates a cSongInstance for the named or current song (mSongInstance).
 *
 * Scales the play level's base volume by its volume control (clamped 0..127) and writes it to the level. With a1 it
 * searches mSongName from the end (strcmp) and stores the index in mCurrentSongNum; likely bug: an unknown name
 * leaves -1 and reads mSongConfig[-1]. Creates the song with cSongInstance_Create(mBasePath, name, config, play options, base
 * volume, volume control, a2, a3), loads its overlay bank (cMusicSys_LoadRandomLoopBank), sets mMusicState = -1 and mSongPlaying = 1.
 *
 * @param songName  the song name, or 0 for mCurrentSongNum
 * @param bankIndex  the cBankManager bank for its loop overlay
 * @param syncLoad  nonzero to load synchronously
 */
void cMusicSys::StartSong(int songName, int bankIndex, int syncLoad)
{
    cBankManager* bankManager;
    cBankMonitor* sub;
    float* pf;
    char baseVol;
    int songConfig;
    int name;
    int base;
    StopSong();
    bankManager = mpVStreamSys->mpVBankManager;
    sub = &bankManager->BankMonitor;
    baseVol = sub->mBaseVol[sub->mInst];
    pf = sub->mVolControl[sub->mInst];
    if (pf != 0) {
        float scaledVol = (float)baseVol * *pf;
        scaledVol = bmMax3838(bmMin3838(scaledVol, 127.0f), 0.0f);
        sub->mBaseVol[bankManager->BankMonitor.mInst] = (char)(int)scaledVol;
        {
            signed char* levelVol = &((sSndOptsRec*)(bankManager->BankMonitor.mInst * 24 + (int)sub->mCurrent))->vol;
            unsigned char clampedVol;
            if (sub->mBaseVol[sub->mInst] > 0x7E || sub->mBaseVol[sub->mInst] >= 0) {
                clampedVol = sub->mBaseVol[sub->mInst] < 0x7F ? (unsigned char)sub->mBaseVol[sub->mInst] : 0x7F;
            } else {
                clampedVol = 0;
            }
            *levelVol = clampedVol;
        }
    }
    if (songName != 0) {
        int idx = mNumSongs - 1;
        while (idx >= 0) {
            if (strcmp(mSongName[idx], (void*)songName) == 0) {
                break;
            }
            idx--;
        }
        mCurrentSongNum = idx;
        name = songName;
        base = (int)&mpVStreamSys->mpVBankManager->BankMonitor;
        mSongInstance = (cSongInstance*)cSongInstance_Create((int)mBasePath, name, (int)mSongConfig[idx],
                                                            base, baseVol, (int)pf, bankIndex, syncLoad);
    } else {
        int k = mCurrentSongNum;
        name = (int)mSongName[k];
        songConfig = (int)mSongConfig[k];
        base = (int)&mpVStreamSys->mpVBankManager->BankMonitor;
        mSongInstance = (cSongInstance*)cSongInstance_Create((int)mBasePath, name, songConfig, base, baseVol,
                                                            (int)pf, bankIndex, syncLoad);
    }
    LoadRandomLoopBank();
    mMusicState = -1;
    mSongPlaying = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_PauseSong);
#ifdef SKIP_ASM
extern "C" void cSongInstance_Pause(cSongInstance*);

/**
 * Pauses the current song (cSongInstance_Pause) when there is one.
 */
void cMusicSys::PauseSong()
{
    void* curSong = mSongInstance;
    if (curSong != 0) {
        ((cSongInstance*)curSong)->Pause();
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_ResumeSong);
#ifdef SKIP_ASM
extern "C" void cSongInstance_Resume(cSongInstance*);

/**
 * Resumes the current song (cSongInstance_Resume) when there is one.
 */
void cMusicSys::ResumeSong()
{
    void* curSong = mSongInstance;
    if (curSong != 0) {
        ((cSongInstance*)curSong)->Resume();
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_StopSong);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sSndKey8_3AC0 {
    long v;
} __attribute__((packed));
extern const sSndKey8_3AC0 D_004A3838[];
extern const sSndKey8_3AC0 D_004A3840[];
extern const sSndKey8_3AC0 D_004A3848[];
extern void cMonitor_MONITOR_DestroyAll1_3AC0(void* bank, long key) __asm__("cMonitor_MONITOR_DestroyAll1");
extern "C" void cAsyncSys_ASYNCSYS_Cancel(void* self, int i);
extern "C" void cBankManager_BANK_UnLoad(void* self, int i);
extern "C" void cMusicSys_ResumeSong(cMusicSys* self);
extern "C" void _$_13cSongInstance(cSongInstance* self, int flags);

/**
 * Stops the current song: cancels its callbacks, unloads its overlay bank and destroys it.
 *
 * Cancels D_004A3838, D_004A3840 and D_004A3848 (cMonitor_MONITOR_DestroyAll1), cancels and unloads bank mBankIndex
 * (cAsyncSys_ASYNCSYS_Cancel, cBankManager_BANK_UnLoad), resumes the song (cMusicSys_ResumeSong), clears mSongPlaying, destroys mCurrent
 * (cSongInstance::~cSongInstance with 3) and clears it.
 */
// PORT: 64-bit long sound keys
void cMusicSys::StopSong()
{
    cMonitor_MONITOR_DestroyAll1_3AC0(&mpVStreamSys->mpVBankManager->BankMonitor, D_004A3838[0].v);
    cMonitor_MONITOR_DestroyAll1_3AC0(&mpVStreamSys->mpVBankManager->BankMonitor, D_004A3840[0].v);
    cMonitor_MONITOR_DestroyAll1_3AC0(&mpVStreamSys->mpVBankManager->BankMonitor, D_004A3848[0].v);
    if (mSongInstance != 0) {
        cAsyncSys_ASYNCSYS_Cancel(mpVStreamSys->mpVBankManager, mSongInstance->mBankIndex);
        cBankManager_BANK_UnLoad(mpVStreamSys->mpVBankManager, mSongInstance->mBankIndex);
    }
    ResumeSong();
    mSongPlaying = 0;
    if (mSongInstance != 0) {
        _$_13cSongInstance((cSongInstance*)(mSongInstance), 3);
    }
    mSongInstance = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_SendMusicEvent);
#ifdef SKIP_ASM
extern "C" void cSongInstance_SendMusicEvent(cSongInstance* mon, int a1, int a2);

/**
 * cMusicSys virtual slot 3: sends a music event to the current song (cSongInstance_SendMusicEvent, not forced) while playing.
 *
 * @param musicEvent  the music event
 */
void cMusicSys::SendMusicEvent(int musicEvent)
{
    if (mSongPlaying != 0) {
        void* curSong = mSongInstance;
        if (curSong != 0) {
            ((cSongInstance*)curSong)->SendMusicEvent(musicEvent, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_SetMusicState);
#ifdef SKIP_ASM
struct sBankMonVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * cMusicSys virtual slot 4: changes the music state; calls virtual slot 3 and stores v in mMusicState when it differs.
 *
 * Only while a song is playing (mSongPlaying). v stays in the second argument register for the slot 3 call.
 *
 * @param musicState  the new music state
 * @return 1 when the state changed, else 0
 */
int cMusicSys::SetMusicState(int musicState)
{
    int changed = 0;
    if (mSongPlaying != 0 && musicState != mMusicState) {
        sBankMonVEntry* vt = (sBankMonVEntry*)mVtbl;
        vt[3].fn(vthis(this, vt[3]));
        mMusicState = musicState;
        changed = 1;
    }
    return changed;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_SetVolume);
#ifdef SKIP_ASM
// PORT: called with the monitor as `this`; the unit declares cSongInstance_SetVolume() with no args
void cSongInstance_SetVolume_m(void* mon) __asm__("cSongInstance_SetVolume");

/**
 * Calls cSongInstance_SetVolume on the current song while playing, passing the caller's second argument through.
 *
 * The C declaration takes only self; the callers pass a volume (0..127) as the second argument.
 */
void cMusicSys::SetVolume()
{
    if (mSongPlaying != 0) {
        void* curSong = mSongInstance;
        if (curSong != 0) {
            cSongInstance_SetVolume_m(curSong);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3C60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_002B2140_3C60(cSongInstance* song) __asm__("func_002B2140");

/**
 * Calls func_002B2140 on the current song while playing, passing the caller's second argument through.
 *
 * @param musicSys  the cMusicSys
 */
extern "C" void func_002B3C60(cMusicSys* musicSys) {
    cSongInstance* song;

    if (musicSys->mSongPlaying != 0) {
        song = musicSys->mSongInstance;
        if (song != 0) {
            func_002B2140_3C60(song);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_SetBaseVolume);
#ifdef SKIP_ASM
/**
 * Sets the current song's base volume (mBaseVolume) while a song is playing.
 *
 * @param baseVol  the base volume
 */
void cMusicSys::SetBaseVolume(signed char baseVol)
{
    if (mSongPlaying != 0) {
        cSongInstance* curSong = mSongInstance;
        if (((void*)curSong) != 0) {
            curSong->mBaseVolume = baseVol;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_SetVolumeEnabled);
#ifdef SKIP_ASM
/**
 * Sets the current song's mVolumeEnabled, which cSongInstance_Update and cSongInstance_SetOverlayLevel need set to apply volume and level.
 *
 * @param enabled  the new value
 */
void cMusicSys::SetVolumeEnabled(int enabled)
{
    cSongInstance* curSong = mSongInstance;
    if (((void*)curSong) != 0) {
        curSong->mVolumeEnabled = enabled;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_SetLowPass);
#ifdef SKIP_ASM
/**
 * Sets the current song's low-pass cutoff (cSongInstance_SetLowPass) while a song is playing.
 *
 * @param cutoff  the cutoff
 */
void cMusicSys::SetLowPass(int cutoff)
{
    if (mSongPlaying != 0) {
        void* curSong = mSongInstance;
        if (curSong != 0) {
            ((cSongInstance*)curSong)->SetLowPass(cutoff);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_SetOverlayLevel);
#ifdef SKIP_ASM
extern "C" void cSongInstance_SetOverlayLevel(cSongInstance* self, signed char v);

/**
 * Sets the current song's overlay level (cSongInstance_SetOverlayLevel) while a song is playing.
 *
 * @param levelPercent  the level in percent
 */
void cMusicSys::SetOverlayLevel(signed char levelPercent)
{
    if (mSongPlaying != 0) {
        void* curSong = mSongInstance;
        if (curSong != 0) {
            ((cSongInstance*)curSong)->SetOverlayLevel(levelPercent);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_FadeOut);
#ifdef SKIP_ASM
extern "C" void cSongInstance_FadeOut(cSongInstance* mon, float v);

/**
 * Fades the current song out over fadeTime seconds (cSongInstance_FadeOut) while a song is playing.
 *
 * @param fadeTime  the fade time in seconds
 */
void cMusicSys::FadeOut(float fadeTime)
{
    if (mSongPlaying != 0) {
        void* curSong = mSongInstance;
        if (curSong != 0) {
            ((cSongInstance*)curSong)->FadeOut(fadeTime);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_FadeBaseVolume);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPacked8_3D80 { long v; } __attribute__((packed));
extern const sPacked8_3D80 D_004A3850_3D80[] __asm__("D_004A3850");
extern void cMonitor_MONITOR_Create_3D80(cBankMonitor* a, int b, long t, int c, int d, int e, int f,
        int g) __asm__("cMonitor_MONITOR_Create");

/**
 * Starts a base-volume fade of the current song to target over duration seconds, stepped by cMusicSys_StepBaseVolumeFade.
 *
 * When the song's mBaseVolume differs it sets mFadeTarget = target, mFadeVolume = the current value and mFadeStep =
 * ((target - current) / 25) / duration (integer division first, so differences below 25 give no step) and arms
 * D_004A3850 every 40 ms.
 *
 * @param target  the target base volume
 * @param duration  the fade time in seconds
 */
void cMusicSys::FadeBaseVolume(signed char target, float duration)
{
    signed char cur;
    cSongInstance* voice = mSongInstance;
    if (((void*)voice) != 0 && target != (cur = voice->mBaseVolume)) {
        int steps = (target - cur) / 25;
        mFadeTarget = target;
        mFadeVolume = (float)cur;
        mFadeStep = (float)steps / duration;
        cMonitor_MONITOR_Create_3D80(&mpVStreamSys->mpVBankManager->BankMonitor, 40, D_004A3850_3D80[0].v,
                                     0, 0, 0, 0, 0);
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_StepBaseVolumeFade);
#ifdef SKIP_ASM
/**
 * Monitor callback stepping the base-volume fade started by cMusicSys_FadeBaseVolume (adds mFadeStep to mFadeVolume).
 *
 * Writes the new value to the current song's mBaseVolume and returns false while short of the target (mFadeTarget);
 * then writes the target and returns true. Reached through D_004A3850.
 *
 * @return true when the fade is done, else false
 */
bool cMusicSys::StepBaseVolumeFade()
{
    float step = mFadeStep;
    float cur = mFadeVolume + step;
    mFadeVolume = cur;
    if ((step > 0.0f && cur < (float)mFadeTarget)
        || (step < 0.0f && cur > (float)mFadeTarget)) {
        mSongInstance->mBaseVolume = (signed char)(int)mFadeVolume;
        return false;
    }
    signed char targetVol = mFadeTarget;
    mSongInstance->mBaseVolume = targetVol;
    return true;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_ClearActiveSongs);
#ifdef SKIP_ASM
/**
 * Empties the active song set: mActiveMap = 0, mNumActiveSongs = 0 and mCategoryCount cleared.
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
void cMusicSys::ClearActiveSongs()
{
    mActiveMap = 0;
    mNumActiveSongs = 0;
    for (int i = 4; i >= 0; i--) {
        mCategoryCount[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_AddActiveSongByName);
#ifdef SKIP_ASM
extern "C" int strstr(const char* a, const char* b);

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sBankMonFind {
    char pad0[0x124];
    char names[64][11];         // 0x124
    int nnames;                 // 0x3E4
    int count;                  // 0x3E8
    char pad3EC[0x3F8 - 0x3EC];
    ulong mask;                 // 0x3F8
    char pad400[0x420 - 0x400];
    int grid[64][5];            // 0x420
    int totals[5];              // 0x920
};

/**
 * Adds the first song whose mSongName contains name (strstr) to the active set and counts its categories.
 *
 * Sets its bit in mActiveMap and increments mNumActiveSongs and mCategoryCount for each of its mCategory flags. Does
 * nothing when 64 songs are active or none matches.
 *
 * @param name  the text to look for
 */
void cMusicSys::AddActiveSongByName(const char* name)
{
    if (mNumActiveSongs < 64) {
        for (int i = 0; i < mNumSongs; i++) {
            if (strstr(mSongName[i], name) != 0) {
                mActiveMap |= (ulong)1 << i;
                mNumActiveSongs++;
                for (int j = 0; j < 5; j++) {
                    if (mCategory[i][j] != 0) {
                        mCategoryCount[j]++;
                    }
                }
                return;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_AddActiveSong);
#ifdef SKIP_ASM

/**
 * Adds song idx to the active set: sets its mActiveMap bit, counts it and its categories (at most 64).
 *
 * @param idx  the song index
 */
void cMusicSys::AddActiveSong(int idx)
{
    if (mNumActiveSongs < 64) {
        mNumActiveSongs++;
        mActiveMap |= (ulong)1 << idx;
        for (int i = 0; i < 5; i++) {
            if (mCategory[idx][i] != 0) {
                mCategoryCount[i]++;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4060__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: an empty cMusicSys step that runs after the active-song list has been rebuilt.
 *
 * cMusicSys_SetActiveSongs calls it after setting songs from a mask, and the play-list selection in ssxAudio after loading a
 * list's SONG entries; the body is empty in this build.
 */
void func_002B4060(void* self)
{
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_getActiveMap);
#ifdef SKIP_ASM
/**
 * Returns the music system's active-song mask, mActiveMap (bit n set for each active song n).
 *
 * @param musicSys  the cMusicSys
 * @return the 64-bit active song mask
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
long cMusicSys::getActiveMap()
{
    return ((long)mActiveMap);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_SetActiveSongs);
#ifdef SKIP_ASM
// PORT: 64-bit `long` mask (8 bytes on EE); use uint64_t off-PS2.
struct sBankMonUse;
extern "C" void cMusicSys_ClearActiveSongs(cMusicSys* self);
extern "C" void cMusicSys_AddActiveSong(cMusicSys* self, int idx);
void func_002B4060(void* self);

/**
 * Sets the active songs from a mask over the first mNumFESongs songs (cMusicSys_ClearActiveSongs, then cMusicSys_AddActiveSong).
 *
 * Ends with the empty func_002B4060.
 *
 * @param mask  the 64-bit song mask
 */
void cMusicSys::SetActiveSongs(ulong mask)
{
    ClearActiveSongs();
    for (int i = 0; i < mNumFESongs; i++) {
        int bit = (mask >> i) & 1;
        if (bit) {
            AddActiveSong(i);
        }
    }
    func_002B4060(this);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_GetSongTitle);
#ifdef SKIP_ASM
struct sBankEntry64 {
    char data[0x64];
};

struct sBankMonitor934 {
    char pad[0x3f0];
    int current;                // 0x3F0
    char pad2[0x934 - 0x3f4];
    sBankEntry64 a[64];         // 0x934
    sBankEntry64 b[64];         // 0x2234
    sBankEntry64 c[64];         // 0x3B34
};

/**
 * Returns song i's title (mTitle[i]); i < 0 means the current song; 0 when there is none.
 *
 * @param i  the song index, or < 0 for mCurrentSongNum
 * @return the title
 */
extern "C" sBankEntry64* cMusicSys_GetSongTitle(cMusicSys* self, int i)
{
    if (i < 0) {
        i = self->mCurrentSongNum;
        if (i < 0) {
            return 0;
        }
    }
    return &((sBankEntry64*)&self->mDisplaySongName[0])[i];
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_GetSongArtist);
#ifdef SKIP_ASM
/**
 * Returns song i's artist (mArtist[i]); i < 0 means the current song; 0 when there is none.
 *
 * @param i  the song index, or < 0 for mCurrentSongNum
 * @return the artist
 */
extern "C" sBankEntry64* cMusicSys_GetSongArtist(cMusicSys* self, int i)
{
    if (i < 0) {
        i = self->mCurrentSongNum;
        if (i < 0) {
            return 0;
        }
    }
    return &((sBankEntry64*)&self->mDisplaySongArtist[0])[i];
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_GetSongAlbum);
#ifdef SKIP_ASM
/**
 * Returns song i's album (mAlbum[i]); i < 0 means the current song; 0 when there is none.
 *
 * @param i  the song index, or < 0 for mCurrentSongNum
 * @return the album
 */
extern "C" sBankEntry64* cMusicSys_GetSongAlbum(cMusicSys* self, int i)
{
    if (i < 0) {
        i = self->mCurrentSongNum;
        if (i < 0) {
            return 0;
        }
    }
    return &((sBankEntry64*)&self->mDisplaySongAlbum[0])[i];
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_SelectActiveSong);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cMusicSys_NextActiveSong_vii(void* self, int filter, int kind) __asm__("cMusicSys_NextActiveSong");

/**
 * Resets mCurrentSongNum to -1 and advances to the next active song n + 1 times (cMusicSys_NextActiveSong, unfiltered).
 *
 * @param n  the number of extra steps
 */
void cMusicSys::SelectActiveSong(int n)
{
    mCurrentSongNum = -1;
    cMusicSys_NextActiveSong_vii(this, 0, 0);
    for (int i = 0; i < n; i++) {
        cMusicSys_NextActiveSong_vii(this, 0, 0);
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_PrevActiveSong);
#ifdef SKIP_ASM
struct sMon41E8 {
    char pad[0x3E4];
    int count;              // 0x3E4
    char pad3E8[0x3F0 - 0x3E8];
    int current;            // 0x3F0
    char pad3F4[0x3F8 - 0x3F4];
    unsigned long mask;     // 0x3F8 (PORT: 64-bit long)
    char pad400[0x420 - 0x400];
    int table[64][5];       // 0x420
    int counts[5];          // 0x920
};

/**
 * Moves mCurrentSongNum back to the previous active song (wrapping), optionally within category kind.
 *
 * The category applies only when filter is set and mCategoryCount[kind] is nonzero.
 *
 * @param filter  nonzero to stay within the category
 * @param kind  the category (0..4)
 */
extern "C" void cMusicSys_PrevActiveSong(sMon41E8* self, int filter, int kind)
{
    if (filter == 0 || self->counts[kind] == 0) {
        do {
            if (--self->current < 0) {
                self->current = self->count - 1;
            }
        } while (!((self->mask >> self->current) & 1));
    } else {
        do {
            if (--self->current < 0) {
                self->current = self->count - 1;
            }
        } while (!((self->mask >> self->current) & 1) || self->table[self->current][kind] == 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_NextActiveSong);
#ifdef SKIP_ASM
struct cMusicSys_NextActiveSong_sMon {
    char pad[0x3E4];
    int count;              // 0x3E4
    char pad3E8[0x3F0 - 0x3E8];
    int current;            // 0x3F0
    char pad3F4[0x3F8 - 0x3F4];
    unsigned long mask;     // 0x3F8 (PORT: 64-bit long)
    char pad400[0x420 - 0x400];
    int table[64][5];       // 0x420
    int counts[5];          // 0x920
};

/**
 * Moves mCurrentSongNum on to the next active song (wrapping), optionally within category kind.
 *
 * The category applies only when filter is set and mCategoryCount[kind] is nonzero. cSSXAudio_PickRandomSong uses it to pick a
 * song.
 *
 * @param filter  nonzero to stay within the category
 * @param kind  the category (0..4)
 */
extern "C" void cMusicSys_NextActiveSong(cMusicSys_NextActiveSong_sMon* self, int filter, int kind)
{
    if (filter == 0 || self->counts[kind] == 0) {
        do {
            if (++self->current >= self->count) {
                self->current = 0;
            }
        } while (!((self->mask >> self->current) & 1));
    } else {
        do {
            if (++self->current >= self->count) {
                self->current = 0;
            }
        } while (!((self->mask >> self->current) & 1) || self->table[self->current][kind] == 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_UpdateSong);
#ifdef SKIP_ASM
extern "C" void cSongInstance_Update(cSongInstance*, int);
extern "C" void cMusicSys_StartOverlay(void*);

/**
 * Updates the current song (cSongInstance_Update without force) and starts its overlay (cMusicSys_StartOverlay) when mOverlayRequested
 * is set.
 */
void cMusicSys::UpdateSong()
{
    void* curSong = mSongInstance;
    if (curSong != 0) {
        ((cSongInstance*)curSong)->Update(0);
        if (mSongInstance->mOverlayRequested != 0) {
            cMusicSys_StartOverlay(this);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_LoadRandomLoopBank);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int cSongInstance_GetRandomLoopBank(cSongInstance* p);
extern "C" void cBankManager_BANK_AsyncLoad(void* self, int i, void* id, int a3);
extern char str_pct_spct_s_c[];


/**
 * Loads a random loop bank of the current song into its cBankManager bank (cBankManager_BANK_AsyncLoad) and sets
 * mLoopBankState to 1.
 *
 * Only when the song has loop banks; the file is "<mPath><mBankData[n]>" with n from cSongInstance_GetRandomLoopBank.
 */
void cMusicSys::LoadRandomLoopBank()
{
    char buf[256];
    cSongInstance* songInstance = (cSongInstance*)mSongInstance;
    if (((void*)songInstance) != 0) {
        int bank = songInstance->mBankIndex;
        if (songInstance->mpConfig->mNumBankData != 0) {
            int n = songInstance->GetRandomLoopBank();
            cSongInstance* currentSong = mSongInstance;
            sprintf(buf, str_pct_spct_s_c, currentSong->mPath, currentSong->mpConfig->mBankData[n]);
            cBankManager_BANK_AsyncLoad(mpVStreamSys->mpVBankManager, bank, buf, 0x100);
            mSongInstance->mLoopBankState = 1;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_ArmOverlay);
#ifdef SKIP_ASM
struct sPacked8_4470 { long v; } __attribute__((packed));
extern const sPacked8_4470 D_004A3838_4470[] __asm__("D_004A3838");
extern void cMonitor_MONITOR_Create_4470(cBankMonitor* a, int b, long t, int c, int d, int e, int f,
        int g) __asm__("cMonitor_MONITOR_Create");

/**
 * Arms the D_004A3838 callback (cMusicSys_WaitLoopBankCallback) at once and sets mOverlayArmed, when the current song's mOverlayArmed is
 * 0.
 */
// PORT: 64-bit long sound key, read unaligned from a const table
void cMusicSys::ArmOverlay()
{
    cSongInstance* songInstance = mSongInstance;
    if (((void*)songInstance) != 0 && songInstance->mOverlayArmed == 0) {
        songInstance->mOverlayArmed = 1;
        cMonitor_MONITOR_Create_4470(&mpVStreamSys->mpVBankManager->BankMonitor, 0, D_004A3838_4470[0].v,
                                     0, 0, 0, 0, 0);
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_CancelOverlay);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPacked8 { long v; } __attribute__((packed));
extern const sPacked8 D_004A3838_2B44D8[] __asm__("D_004A3838");
extern const sPacked8 D_004A3840_2B44D8[] __asm__("D_004A3840");
extern "C" void cMonitor_MONITOR_DestroyAll1(cBankMonitor* a, long t);
extern "C" void cMusicSys_ClearOverlayArmed(cMusicSys* self);

/**
 * Cancels the song's D_004A3838 and D_004A3840 callbacks (cMonitor_MONITOR_DestroyAll1) and clears mOverlayArmed
 * (cMusicSys_ClearOverlayArmed).
 *
 * Only when there is a current song.
 */
void cMusicSys::CancelOverlay()
{
    if (mSongInstance != 0) {
        cMonitor_MONITOR_DestroyAll1(&mpVStreamSys->mpVBankManager->BankMonitor, D_004A3838_2B44D8[0].v);
        cMonitor_MONITOR_DestroyAll1(&mpVStreamSys->mpVBankManager->BankMonitor, D_004A3840_2B44D8[0].v);
        ClearOverlayArmed();
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_OverlayBeatCallback);
#ifdef SKIP_ASM
class cMusicSys_OverlayBeatCallback_cObj {
public:
    int unk0;
    void* entries[(0x5440 - 4) / 4];
    // vptr at 0x5440; slot N at vtable offset N*8
    virtual int PlayNextPhrase(int monitorInst);  // slot 1 (name ours): cSSXAudio's is cSSXAudio_PlayLoopBeat, which
                                                  // plays the next loop-bank phrase
};

/**
 * Monitor callback (D_004A3840): calls cMusicSys's virtual slot 1 with the monitor instance.
 *
 * On gSSXAudio the slot is cSSXAudio_PlayLoopBeat, which plays the next loop-bank phrase.
 *
 * @param monitorInst  the cMonitorInstance
 */
void cMusicSys::OverlayBeatCallback(int monitorInst)
{
    ((cMusicSys_OverlayBeatCallback_cObj*)this)->PlayNextPhrase(monitorInst);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_ArmBeatCallback);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern const sPacked8 D_004A3840_2B4568[] __asm__("D_004A3840");
extern "C" int cSongInstance_GetCurrentBeat(cSongInstance* ent);
extern "C" int cSongInstance_GetRandomPhraseStart(cSongInstance* ent);
extern "C" void cMonitor_MONITOR_Create(cBankMonitor* a, int b, long t, void* c, int d, int e, int f, int g);

/**
 * Arms the beat-synchronised overlay callback (D_004A3840, cMusicSys_OverlayBeatCallback) for the song ent.
 *
 * The delay is the time to the next beat (cSongInstance_GetTimeToNextBeat); mPtr = mCurrent, mData1 = a random phrase start
 * (cSongInstance_GetRandomPhraseStart), mData2 = the next beat (cSongInstance_GetCurrentBeat + 1), mData3 = the beat length (cSongInstance_GetBeatLength). Only when
 * there is a current song.
 *
 * @param ent  the cSongInstance to time against
 */
void cMusicSys::ArmBeatCallback(cSongInstance* ent)
{
    if (mSongInstance != 0) {
        float beatLength = ent->GetBeatLength();
        float timeToBeat = ent->GetTimeToNextBeat();
        int nextBeat = ent->GetCurrentBeat() + 1;
        int phraseStart = ent->GetRandomPhraseStart();
        cMonitor_MONITOR_Create(&mpVStreamSys->mpVBankManager->BankMonitor, (int)timeToBeat,
                                D_004A3840_2B4568[0].v, mSongInstance, phraseStart, nextBeat, (int)beatLength, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_StartOverlay);
#ifdef SKIP_ASM
extern "C" void PATH_requestPart(int cmd, int a, int b);

static inline int cMusicSys_StartOverlay_ready(cBankInstance* e)
{
    return e->mBankStatus == 1;
}

// PORT: the unit declares cMusicSys_StartOverlay as void; it really returns 0/1.
int cMusicSys_StartOverlay_impl(void* self) __asm__("cMusicSys_StartOverlay");

/**
 * Starts the current song's overlay once its loop bank is loaded; returns whether the overlay is running.
 *
 * Sets mOverlayArmed. With mLoopBankState == 1 and the bank record in state 1 it passes the bank handle to the overlay
 * stream (PATH_requestPart(0x11000002, 0, handle)) and sets mLoopBankState = 2. Called by cMusicSys_UpdateSong and the audio
 * update.
 *
 * @return 1 when started or already running, 0 otherwise (also without a song)
 */
int cMusicSys::StartOverlay()
{
    int one = 1;
    if ((char*)mSongInstance == 0) return 0;
    mSongInstance->mOverlayArmed = one;
    int ready = 0;
    cSongInstance* curSong = mSongInstance;
    int st = curSong->mLoopBankState;
    if (st == one) {
        // PORT: pointer arithmetic through int (index first, as retail adds it)
        cBankInstance* bankInst =
            (cBankInstance*)(curSong->mBankIndex * 0x60 + (int)mpVStreamSys->mpVBankManager->mBank);
        if (cMusicSys_StartOverlay_ready(bankInst)) {
            PATH_requestPart(0x11000002, 0, bankInst->mBankHandle);
            mSongInstance->mLoopBankState = 2;
            ready = 1;
        }
    } else if (st == 2) {
        ready = one;
    }
    return ready;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_RequestOverlay);
#ifdef SKIP_ASM
/**
 * Sets the current song's mOverlayRequested, so cMusicSys_UpdateSong starts its overlay.
 */
void cMusicSys::RequestOverlay()
{
    cSongInstance* curSong = mSongInstance;
    if (((void*)curSong) != 0) {
        curSong->mOverlayRequested = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_IsOverlayArmed);
#ifdef SKIP_ASM
/**
 * Returns the current song's mOverlayArmed (set while its overlay is armed), or 0 without a song.
 *
 * @return mOverlayArmed, or 0
 */
void* cMusicSys::IsOverlayArmed()
{
    cSongInstance* inner = mSongInstance;
    if (((void*)inner) == 0) {
        return 0;
    }
    return (void*)inner->mOverlayArmed;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_ClearOverlayArmed);
#ifdef SKIP_ASM
/**
 * Clears the current song's mOverlayArmed when there is a song.
 */
void cMusicSys::ClearOverlayArmed()
{
    cSongInstance* curSong = mSongInstance;
    if (((void*)curSong) != 0) {
        curSong->mOverlayArmed = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_GetBaseVolume);
#ifdef SKIP_ASM
/**
 * Returns the current song's base volume (mBaseVolume), or 0 without a song.
 *
 * @return the base volume
 */
int cMusicSys::GetBaseVolume()
{
    cSongInstance* curSong = mSongInstance;
    if (((char*)curSong) == 0) {
        return 0;
    }
    return curSong->mBaseVolume;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_WaitLoopBankCallback);
#ifdef SKIP_ASM
extern "C" void cMusicSys_ArmBeatCallback(cMusicSys* self, cSongInstance* ent);

/**
 * Monitor callback (D_004A3838) waiting for the song's loop bank: arms the beat callback once it is loaded.
 *
 * By the bank record's mBankStatus: USED arms cMusicSys_ArmBeatCallback and returns true; LOADING returns false (call again); EMPTY
 * and others return true.
 *
 * @return true when done, false to be called again
 */
bool cMusicSys::WaitLoopBankCallback()
{
    cSongInstance* ent = mSongInstance;
    cBankInstance* table = mpVStreamSys->mpVBankManager->mBank;
    int state = table[ent->mBankIndex].mBankStatus;
    switch (state) {
    case EMPTY:
        return true;
    case USED:
        ArmBeatCallback(ent);
        return true;
    case LOADING:
        return false;
    default:
        return true;
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_CheckLoopBankCallback);
#ifdef SKIP_ASM
extern "C" int cBankManager_BANK_Handle(void*, int);
extern "C" int SNDbankinuse(int);

/**
 * Monitor callback (D_004A3848): when the song's loop bank is not loaded or not in use, clears mOverlayArmed and ends.
 *
 * The check (mBankStatus USED and SNDbankinuse true) runs inside the critical section.
 *
 * @return true when mOverlayArmed was cleared, false to be called again
 */
bool cMusicSys::CheckLoopBankCallback()
{
    int ok = 0;
    int slot = mSongInstance->mBankIndex;
    cBankManager* other = mpVStreamSys->mpVBankManager;
    SNDSYS_entercritical();
    bool notReady = other->mBank[slot].mBankStatus != USED;
    if (!notReady) {
        ok = SNDbankinuse(cBankManager_BANK_Handle(other, slot)) == true;
    }
    SNDSYS_leavecritical();
    if (ok == 0) {
        mSongInstance->mOverlayArmed = 0;
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetDuckToLoops__FPv);
#ifdef SKIP_ASM
int cSongInstance_GetDuckToLoops(void* vself);

/**
 * Returns a song's mDuckToLoops (from its cSongConfiguration).
 *
 * @return mDuckToLoops
 */
int cSongInstance::GetDuckToLoops()
{
    return mpConfig->mDuckToLoops;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_GetDuckToLoops);
#ifdef SKIP_ASM
/**
 * Returns the current song's mDuckToLoops while a song is playing, otherwise 1.
 *
 * @return mDuckToLoops, or 1
 */
int cMusicSys::GetDuckToLoops()
{
    if (mSongPlaying != 0) {
        void* curSong = mSongInstance;
        if (curSong != 0) {
            return cSongInstance_GetDuckToLoops(curSong);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongInstance_GetSedValue);
#ifdef SKIP_ASM
/**
 * Returns a song's mSedValue (from its cSongConfiguration).
 *
 * @return mSedValue
 */
int cSongInstance::GetSedValue()
{
    return mpConfig->mSedValue;
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_GetSedValue);
#ifdef SKIP_ASM
/**
 * Returns the current song's mSedValue while a song is playing, otherwise -1.
 *
 * @return mSedValue, or -1
 */
int cMusicSys::GetSedValue()
{
    if (mSongPlaying != 0) {
        cSongInstance* curSong = mSongInstance;
        if (curSong != 0) {
            return curSong->GetSedValue();
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_GetCurrentSongSedValue__FPv);
#ifdef SKIP_ASM
/**
 * Returns mSedValue of the song configuration at mCurrentSongNum (no playing song needed).
 *
 * @return mSedValue
 */
int cMusicSys::GetCurrentSongSedValue()
{
    cSongConfiguration* config = mSongConfig[mCurrentSongNum];
    return config->mSedValue;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", cMusicSys_GetTimeToNextBeat);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" float cSongInstance_GetTimeToNextBeat(cSongInstance* self);

/**
 * Returns the time to the current song's next beat (cSongInstance_GetTimeToNextBeat, in the critical section), or 0 when idle.
 *
 * The script engine's beat timer reads it.
 *
 * @return the time in ms
 */
float cMusicSys::GetTimeToNextBeat()
{
    if (mSongPlaying == 0) {
        return 0.0f;
    }
    SNDSYS_entercritical();
    float timeToBeat = mSongInstance->GetTimeToNextBeat();
    SNDSYS_leavecritical();
    return timeToBeat;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_PreviewSong);
#ifdef SKIP_ASM
class cMusicSys_PreviewSong_cObj {
public:
    int unk0;
    cSongConfiguration* entries[(0x5440 - 4) / 4]; // 0x4 cMusicSys::mSongConfig (and the rest of cMusicSys)
    // vptr at 0x5440; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void SendMusicEvent(int event);  // slot 3 (name ours): cMusicSys_SetMusicState sends a new music state and
                                             // cSSXAudio_UpdateAirTimeVolume music events 7 / 8 through it
};

/**
 * Previews song i: calls cMusicSys's virtual slot 3 with its mPreview value when that is >= 0.
 *
 * The message center's music list calls it on gSSXAudio + 0x118.
 *
 * @param i  the song index
 */
extern "C" void cMusicSys_PreviewSong(cMusicSys_PreviewSong_cObj* self, int i)
{
    int previewValue = self->entries[i]->mPreview;
    if (previewValue >= 0) {
        self->SendMusicEvent(previewValue);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_SetSongTag);
#ifdef SKIP_ASM
/**
 * Sets the current song's tag (mSongTag) while a song is playing.
 *
 * The audio code tags songs 101-103 (peak ambience), 201, 301 ("charsel") and 401 ("pktrans").
 *
 * @param songTag  the tag
 */
void cMusicSys::SetSongTag(int songTag)
{
    if (mSongPlaying != 0) {
        cSongInstance* curSong = mSongInstance;
        if (((void*)curSong) != 0) {
            curSong->mSongTag = songTag;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_GetSongTag);
#ifdef SKIP_ASM
/**
 * Returns the current song's tag (mSongTag) while a song is playing, otherwise -1.
 *
 * @return the tag, or -1
 */
int cMusicSys::GetSongTag()
{
    if (mSongPlaying != 0) {
        cSongInstance* curSong = mSongInstance;
        if (((void*)curSong) != 0) {
            return curSong->mSongTag;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_SetHoldEvents);
#ifdef SKIP_ASM
/**
 * Sets the current song's mHoldEvents, which makes cSongInstance_SendMusicEvent ignore unforced music events.
 *
 * @param holdEvents  the new value
 */
void cMusicSys::SetHoldEvents(int holdEvents)
{
    cSongInstance* curSong = mSongInstance;
    if (((void*)curSong) != 0) {
        curSong->mHoldEvents = holdEvents;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_LoadMPFFile);
#ifdef SKIP_ASM
extern "C" void ASYNCFILE_cancel(void* f);
extern "C" void ASYNCFILE_release(void* f, int a, int b);
extern "C" void* ASYNCFILE_loadat(void* name, void* buf, int size);
extern "C" void FILE_loadat(void* name, void* buf, int size);

/**
 * Loads an MPF file into mMPFFile (25600 bytes), asynchronously (ASYNCFILE_loadat) or at once (FILE_loadat).
 *
 * A pending async load (mpMpfFile) is cancelled and released first; an async load's handle is kept in mpMpfFile.
 *
 * @param name  the file name
 * @param sync  nonzero to load synchronously
 */
void cMusicSys::LoadMPFFile(void* name, int sync)
{
    if (mpMpfFile != 0) {
        ASYNCFILE_cancel(mpMpfFile);
        ASYNCFILE_release(mpMpfFile, 0, 0);
    }
    if (sync == 0) {
        mpMpfFile = ASYNCFILE_loadat(name, mMPFFile, 25600);
    } else {
        FILE_loadat(name, mMPFFile, 25600);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_IsMPFLoaded);
#ifdef SKIP_ASM
extern "C" int ASYNCFILE_getstatus(void*);

/**
 * Returns whether the MPF file can be used: force set, or the async load (mpMpfFile) reports kASYNCFILE_Done.
 *
 * @param force  nonzero to return true at once
 * @return true when the MPF file can be used
 */
bool cMusicSys::IsMPFLoaded(int force)
{
    void* asyncFile;
    if (force != 0) {
        return true;
    }
    asyncFile = mpMpfFile;
    if (asyncFile != 0 && ASYNCFILE_getstatus(asyncFile) == kASYNCFILE_Done) {
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_GetMPFFile__FPv);
#ifdef SKIP_ASM
/**
 * Returns mMPFFile, the music system's MPF data buffer.
 *
 * Only cSongInstance_Update calls it, on gMusicSys, once the MPF load has finished.
 *
 * @return mMPFFile
 */
int cMusicSys::GetMPFFile()
{
    return (int)mMPFFile;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_IsCurrentSongActive);
#ifdef SKIP_ASM
/**
 * Returns whether the song at mCurrentSongNum is active (its bit in mActiveMap).
 *
 * @return 1 or 0
 */
// PORT: needs a 64-bit return (ulong); use uint64_t off-PS2.
ulong cMusicSys::IsCurrentSongActive()
{
    return (mActiveMap >> mCurrentSongNum) & 1;
}
#endif

