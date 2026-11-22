#include "common.h"
#include "ealib/snd.h"
#include "vtable.h"
#include "sound/ssxaudio.h"
#include "sound/songinstance.h"
#include "sound/speechinstance.h"
#include "sound/soundsys.h"
#include "sound/bankmonitor.h"
#include "sound/speechsys.h"
#include "sound/musicsys.h"

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_cBankMonitor);
#ifdef SKIP_ASM
struct sSndPlayerD9C8;
extern "C" sSndPlayerD9C8* cMonitor_cMonitor(sSndPlayerD9C8* self, int inchrg, int heap);
extern "C" void* cSoundSys_cSoundSys(cSoundSys* self, int a);
extern "C" void* cBankMonitorInstance_cBankMonitorInstance(void* self);
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
    sBmVoiceCD08() { cBankMonitorInstance_cBankMonitorInstance(this); }
    void operator delete[](void* p, unsigned int size);
};

struct sBankMonitorCD08 {
    char* vbase;                // 0x0
    char pad_0x4[0x8D8 - 0x4];
    void** vtbl;                // 0x8D8
    volatile int f8DC;          // 0x8DC
    int f8E0;                   // 0x8E0
    int f8E4;                   // 0x8E4
    int f8E8;                   // 0x8E8
    sBmVoiceCD08* voices;       // 0x8EC
    char vb[0x4];               // 0x8F0
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
    self->f8DC = 0;
    self->f8E0 = 1;
    self->f8E4 = 1;
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
extern "C" void cBankMonitorInstance__cBankMonitorInstance(void* self, int flags);
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
            cBankMonitorInstance__cBankMonitorInstance(bankInstance, 0);
        }
        cMemMan_free((char*)self->mBankMonitorInstance - 0x10);
    }
    cMonitor__cMonitor((cMonitor*)((sSndPlayerDAC8*)self), 0);
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
extern "C" int cBankMonitor_FindFreeInstance(cBankMonitor* self, int n)
{
    for (int i = 0; i < 64; i++) {
        if (cBankMonitorInstance_IsFree(&self->mBankMonitorInstance[i]) != 0) {
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
// ch[cur]. Field names follow sound/soundsys (ch = mCurrent, vals = mBaseVol, p70 = mDelay, ...).
struct sSndPlayOptsView {
    char* vbase;                // 0x00 virtual-base pointer
    sSndOptsRec def;            // 0x04
    volatile int cur;           // 0x1C
    sSndOptsRec* ch;            // 0x20
    sBmQuad* tgt[4];            // 0x24
    int* p34;                   // 0x34
    int* vals;                  // 0x38
    int* p3C;                   // 0x3C
    sSndCallback callbacks[4];  // 0x40
    int* p60;                   // 0x60
    int* p64;                   // 0x64
    float* p68;                 // 0x68
    float* p6C;                 // 0x6C
    sSndOptsDelay* p70;         // 0x70
    int* p74;                   // 0x74
    int* p78;                   // 0x78
    int* p7C;                   // 0x7C
    int* p80;                   // 0x80
    int* p84;                   // 0x84
};

// One of a sound player's 48 monitored-sound slots (0x2C bytes).
struct sBmSlot {
    char pad00[0xC];
    sSndCallback callback;      // 0x0C
    char pad14[0x18 - 0x14];
    int f18;                    // 0x18
    int f1C;                    // 0x1C
    int f20;                    // 0x20
    int f24;                    // 0x24
    int f28;                    // 0x28
};

// Sound player: a cSndPlayOpts (0x88 bytes) plus 48 monitored-sound slots; virtual base at 0x8DC.
struct sSndPlayer : public sSndPlayOptsView {
    volatile int f88;           // 0x88
    int f8C;                    // 0x8C
    volatile int busy;          // 0x90
    char pad_0x94[0x4];
    sBmSlot slots[48];          // 0x98
    void** vtbl;                // 0x8D8
    char vb[0x4];               // 0x8DC
};

// cBankManager as this unit sees it (as in sound/soundsys): only its sound player at 0x1D8 is used here.
struct cBankManager {
    char pad0[0x1D8];
    sSndPlayer sub;             // 0x1D8
};
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_Create);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBankMonitorInstance_cBankMonitorInstance(void* self);

// One of a bank monitor's 64 voices (0xC0 bytes).
struct sBmVoice {
    int kind;                   // 0x00 (1 / 2: which sound system plays it)
    int id;                     // 0x04
    int valid;                  // 0x08
    int handle;                 // 0x0C
    char pad10[0x58 - 0x10];
    int f58;                    // 0x58
    float value;                // 0x5C
    int f60;                    // 0x60
    int vol;                    // 0x64
    sBmQuad* tgt;               // 0x68
    int f6C;                    // 0x6C
    sBmQuad tgtCopy;            // 0x70
    int f80;                    // 0x80
    int f84;                    // 0x84
    float f88;                  // 0x88
    float f8C;                  // 0x8C
    float f90;                  // 0x90
    int f94;                    // 0x94
    int f98;                    // 0x98
    int f9C;                    // 0x9C
    int fA0;                    // 0xA0
    int fA4;                    // 0xA4
    sSndCallback callback;      // 0xA8
    char padB0[0xC0 - 0xB0];
    sBmVoice() { cBankMonitorInstance_cBankMonitorInstance(this); }
    void operator delete[](void* p, unsigned int size);
};

// sBankMonitorView: a sound player (cSndPlayOpts part at 0x0, player part up to 0x8DC) plus 64 voices;
// virtual base at 0x8F0.
struct sBankMonitorView : public sSndPlayOptsView {  // this unit's model of cBankMonitor (sound/bankmonitor.h)
    char pad88[0x8D8 - 0x88];
    void** vtbl;                // 0x8D8
    volatile int busy;          // 0x8DC
    int f8E0;                   // 0x8E0
    int f8E4;                   // 0x8E4
    int f8E8;                   // 0x8E8
    sBmVoice* voices;           // 0x8EC
    char vb[0x4];               // 0x8F0
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
 * target position, callback and other settings; the target is copied into the voice when the options ask for
 * that. With no free voice it logs "Out of BankMonitor instances" and releases the handle with SNDstop()
 * (kind 1) or SNDseqstop() (kind 2). The busy counter (+0x8DC) is raised for the duration.
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
    int idx = cBankMonitor_FindFreeInstance((cBankMonitor*)self, self->busy);
    if (idx >= 0) {
        instance = &self->voices[idx];
        cBankMonitorInstance_Reset(instance);
        instance->kind = kind;
        instance->id = id;
        instance->f60 = self->p3C[self->cur];
        instance->vol = vol;
        instance->tgt = self->tgt[self->cur];
        setCallback_Create(instance, OPTS_GetCallback_Create(self));
        instance->f80 = self->p60[self->cur];
        instance->f84 = self->p64[self->cur];
        if (self->p34[self->cur] != 0) {
            instance->tgtCopy = *instance->tgt;
            instance->tgt = &instance->tgtCopy;
        }
        instance->f88 = self->p68[self->cur];
        instance->f8C = self->p6C[self->cur];
        instance->f94 = self->p74[self->cur];
        instance->f98 = self->p78[self->cur];
        int f9C = self->p7C[self->cur];
        if (instance->f58 == 0)
            instance->f9C = f9C;
        instance->f90 = -1.0f;
        instance->fA0 = self->p80[self->cur];
        instance->fA4 = self->p84[self->cur];
        if (self->tgt[self->cur] != 0)
            cBankMonitorInstance_SetLastDistance(instance, self);
        instance->valid = id >= 0;
    } else {
        bxLogPrint(D_00483250);
        switch (kind) {
        case 1:
            SNDstop(id);
            break;
        case 2:
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
extern "C" void cBankMonitor_BANKMONITOR_Pause(cBankMonitor* self)
{
    self->mBankMonitorPaused = 1;
    for (int i = 0; i < 64; i++) {
        cBankMonitorInstance_Pause(&self->mBankMonitorInstance[i]);
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
extern "C" void cBankMonitor_BANKMONITOR_Resume(cBankMonitor* self)
{
    for (int i = 0; i < 64; i++) {
        cBankMonitorInstance_Resume(&self->mBankMonitorInstance[i]);
    }
    self->mBankMonitorPaused = 0;
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
extern "C" void cBankMonitor_BANKMONITOR_FadeOut(cBankMonitor* self, int idx, int type, float FadeTime)
{
    void* voice = cBankMonitor_BANKMONITOR_GetHandle(self, idx, type);
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
extern "C" void cBankMonitor_BANKMONITOR_Destroy(cBankMonitor* self, int i)
{
    cBankMonitorInstance_Reset(&self->mBankMonitorInstance[i]);
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
extern "C" void cBankMonitor_BANKMONITOR_DestroyAll(cBankMonitor* self)
{
    int i;
    for (i = 0; i < 64; i++) {
        cBankMonitor_BANKMONITOR_Destroy(self, i);
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

static inline int cBankMonitor_BANKMONITOR_DestroyAll1_match(char* e, sBmKeyD410 k)
{
    int r = 0;
    if (k.id == *(short*)(e + 0xAA)) {
        if ((k.id != -1 && k.s == *(short*)(e + 0xAC)) || k.v == *(int*)(e + 0xAC)) {
            r = 1;
        }
    }
    return r;
}

/**
 * Resets every monitored sound whose callback is the given pointer to member function.
 *
 * Walks the 64 cBankMonitorInstance records (mBankMonitorInstance, 0xC0 each) and calls cBankMonitorInstance_Reset on
 * each one whose callback (instance +0xA8, a g++ 2.95 pointer to member: index at +0xAA, function or vtable offset at
 * +0xAC) matches key: same index, and the same vtable offset (virtual) or the same function word.
 *
 * cBankMonitor's virtual slot 3 (cBankMonitor_vtbl).
 *
 * @param key  the callback to match (8-byte pointer to member function)
 */
extern "C" void cBankMonitor_BANKMONITOR_DestroyAll1(cBankMonitor* self, sBmKeyD410 key)
{
    for (int i = 0; i < 64; i++) {
        if (cBankMonitor_BANKMONITOR_DestroyAll1_match((char*)&self->mBankMonitorInstance[i], key)) {
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
extern "C" void cBankMonitor_BANKMONITOR_DestroyAllWithPtr(cBankMonitor* self, int id)
{
    for (int i = 0; i < 64; i++) {
        if ((int)self->mBankMonitorInstance[i].mPtr == id) {
            cBankMonitorInstance_Reset(&self->mBankMonitorInstance[i]);
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
extern "C" void* cBankMonitor_BANKMONITOR_GetHandle(cBankMonitor* self, int idx, int key)
{
    for (int i = 0; i < 64; i++) {
        if (cBankMonitorInstance_IsFree(&self->mBankMonitorInstance[i]) == 0
            && (self->mBankMonitorInstance)[i].mMonitorType == key
            && (self->mBankMonitorInstance)[i].mHandle == idx) {
            return &(self->mBankMonitorInstance)[i];
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
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(cBankMonitor* self, int idx, int type, float FadeTime)
{
    void* voice = cBankMonitor_BANKMONITOR_GetHandle(self, idx, type);
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
 * Only active bank instances (type 1) whose bank id (+0xC) matches are stopped.
 *
 * @param id  the bank's handle (On Tour BankHandle)
 * @param FadeTime   fade time; 0 stops at once (On Tour FadeTime)
 */
extern "C" void cBankMonitor_BANKMONITOR_StopBank(cBankMonitor* self, int id, float FadeTime)
{
    for (int i = 0; i < 64; i++) {
        if (cBankMonitorInstance_IsFree(&self->mBankMonitorInstance[i]) == 0) {
            // PORT: pointer arithmetic through int
            cBankMonitorInstance* instance = (cBankMonitorInstance*)(i * 0xC0 + (int)self->mBankMonitorInstance);
            if (instance->mMonitorType == 1 && instance->mRestartInfo.bank.mBank == id) {
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
extern "C" int cBankMonitor_BANKMONITOR_Active(cBankMonitor* self)
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (cBankMonitorInstance_IsFree(&self->mBankMonitorInstance[i]) == 0) {
            // PORT: pointer arithmetic through int
            cBankMonitorInstance* instance = (cBankMonitorInstance*)(i * 0xC0 + (int)self->mBankMonitorInstance);
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
extern "C" int cBankMonitor_BANKMONITOR_Used(cBankMonitor* self)
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (cBankMonitorInstance_IsFree(&self->mBankMonitorInstance[i]) == 0) {
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
extern "C" void cBankMonitor_BANKMONITOR_CameraChange(cBankMonitor* self)
{
    for (int i = 0; i < 64; i++) {
        (self->mBankMonitorInstance)[i].mLastDist = -1.0f;
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
    *(int*)self = 0;
    self->mCountReset = -1;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitorInstance__cMonitorInstance);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys a cMonitorInstance (g++ 2.95 destructor): nothing to clean up; frees the object when bit 0 of flags is set.
 *
 * cMonitor__cMonitor calls it with 0 on each of its 48 mMonitorInstance entries.
 *
 * @param flags  g++ 2.95 destructor flags (bit 0: operator delete the object)
 */
extern "C" void cMonitorInstance__cMonitorInstance(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
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
extern "C" void cMonitorInstance_Create(cMonitorInstance* self, int Freq, long key, int Ptr, int Data1, int Data2, int Data3, int Data4)
{
    cMonitorInstance_Reset(self);
    cMonitorInstance_CountReset(self, Freq);
    // PORT: the 8-byte key is copied through its in-memory layout
    *(&self->mCallback) = *(tSndCallback*)&key;
    self->mPtr = (void*)Ptr;
    self->mData1 = Data1;
    self->mData2 = Data2;
    self->mData3 = Data3;
    self->mData4 = Data4;
    cMonitorInstance_Enable(self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitorInstance_IsFree);
#ifdef SKIP_ASM
/**
 * Tells whether a monitor instance is unused (its reset count is -1).
 *
 * @return 1 when free, otherwise 0
 */
extern "C" int cMonitorInstance_IsFree(cMonitorInstance* self)
{
    return self->mCountReset == -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitorInstance_Reset);
#ifdef SKIP_ASM
void cMonitorInstance_Disable(void* self);
void cMonitorInstance_CountReset(void* self, int a1);

/**
 * Frees a monitor instance: disables it (cMonitorInstance_Disable, probably Disable) and sets its reset count to -1.
 */
extern "C" void cMonitorInstance_Reset(cMonitorInstance* self)
{
    cMonitorInstance_Disable(self);
    cMonitorInstance_CountReset(self, -1);
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
void cMonitorInstance_CountReset(void* vself, int periodMs)
{
    cMonitorInstance* self = (cMonitorInstance*)vself;
    self->mCountReset = periodMs;
    self->mCountdown = periodMs;
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
void cMonitorInstance_Enable(void* self)
{
    *(int*)self = 1;
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
void cMonitorInstance_Disable(void* vself)
{
    cMonitorInstance* self = (cMonitorInstance*)vself;
    self->mEnabled = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD998);
#ifdef SKIP_ASM
class func_002AD998_cObj {
public:
    char data[0x8D8];
    virtual void v01();
};


/**
 * The sound driver's 100 Hz tick for the monitor: calls the current monitor's virtual slot 1 (cMonitor_MONITOR_Update).
 *
 * Calls through gMonitor's vptr (mVtbl, +0x8D8). cMonitor_MONITOR_BeginMonitoring registers it with
 * SNDSYS_add100hzclient and cMonitor__cMonitor removes it.
 */
extern "C" void func_002AD998()
{
    ((func_002AD998_cObj*)gMonitor)->v01();
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
    gMonitor = (cMonitor*)((func_002AD998_cObj*)self);
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


struct sSndPlayerDAC8 {
    char* vbase;                // 0x0
    char pad_0x4[0x98 - 0x4];
    cMonitorInstance slots[48];      // 0x98
    void** vtbl;                // 0x8D8
};

/**
 * Destroys the sound monitor: removes its timer callback (func_002AD998), clears gMonitor and destroys its 48
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
    SNDSYS_remove100hzclient((int)func_002AD998);
    gMonitor = 0;
    if (((&self->mMonitorInstance[0])) != 0) {
        cMonitorInstance* monitorInstance = end;
        while (((&self->mMonitorInstance[0])) != monitorInstance) {
            monitorInstance--;
            cMonitorInstance__cMonitorInstance((int*)monitorInstance, 0);
        }
    }
    cSndPlayOpts__cSndPlayOpts(((sSndPlayerDAC8*)self), 0);
    if (flags & 2) {
        cSoundSys__cSoundSys((cSoundSys*)self->mpVSoundSys, 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void func_002AD998();
extern "C" void* SNDSYS_add100hzclient(void*);

//98.75%
INCLUDE_ASM("sound/bankmonitor", cMonitor_MONITOR_BeginMonitoring__FPv);
#ifdef SKIP_ASM
/**
 * Starts the bank monitor's periodic update by registering func_002AD998 as a sound-system client.
 *
 * func_002AD998 forwards to gMonitor's first virtual method; SNDSYS_add100hzclient (On Tour SNDSYS_add100hzclient) adds it to
 * the sound library's 100 Hz client list when the library's enable flag is set. The object pointer is not used.
 *
 * @return no meaningful value (SNDSYS_add100hzclient returns nothing)
 */
void* cMonitor_MONITOR_BeginMonitoring(void* self)
{
    return SNDSYS_add100hzclient((void*)func_002AD998);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitor_MONITOR_Update);
#ifdef SKIP_ASM
/**
 * Counts one monitor tick (mCounter), unless the monitor is paused.
 */
extern "C" void cMonitor_MONITOR_Update(cMonitor* self)
{
    if (self->mPaused == 0) {
        self->mCounter += 1;
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
extern "C" int cMonitor_MONITOR_GetFreeInstance(cMonitor* self, int n)
{
    for (int i = 0; i < 48; i++) {
        if (cMonitorInstance_IsFree(&self->mMonitorInstance[i]) != 0) {
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

void cMonitorInstance_Create_cb(void* self, int a1, sSndCallback cb, int b, int c, int d, int e, int f) __asm__("cMonitorInstance_Create");

extern "C" int cMonitor_MONITOR_Create_pl(sSndPlayer* self, int a1, sSndCallback cb, int b, int c, int d, int e, int f) __asm__("cMonitor_MONITOR_Create");
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
extern "C" int cMonitor_MONITOR_Create_pl(sSndPlayer* self, int freq, sSndCallback cb, int userPtr, int data1, int data2, int data3, int data4)
{
    self->busy = self->busy + 1;
    int idx = cMonitor_MONITOR_GetFreeInstance((cMonitor*)self, self->busy);
    if (idx >= 0) {
        cMonitorInstance_Create_cb((char*)self + 0x98 + idx * 44, freq, cb, userPtr, data1, data2, data3, data4);
    }
    self->busy = self->busy - 1;
    return idx;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMonitor_MONITOR_Destroy);
#ifdef SKIP_ASM
/**
 * Resets monitor instance @p idx (0x2C bytes each from +0x98) with cMonitorInstance_Reset.
 *
 * @param idx  the instance (On Tour int Index)
 */
extern "C" void cMonitor_MONITOR_Destroy(cMonitor* self, int idx)
{
    int off = idx * 44 + 152;
    cMonitorInstance_Reset((cMonitorInstance*)(((char*)self) + off));
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
extern "C" void cMonitor_MONITOR_DestroyAll(cMonitor* self)
{
    char* instance = (char*)&self->mMonitorInstance;
    int i;
    for (i = 47; i >= 0; i--) {
        cMonitorInstance_Reset((cMonitorInstance*)instance);
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

static inline int cMonitor_MONITOR_DestroyAll1_match(char* e, sBmKeyDDE0 k)
{
    int r = 0;
    if (k.id == *(short*)(e + 0xE)) {
        if ((k.id != -1 && k.s == *(short*)(e + 0x10)) || k.v == *(int*)(e + 0x10)) {
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
        if (cMonitor_MONITOR_DestroyAll1_match((char*)&self->mMonitorInstance[i], key)) {
            cMonitorInstance_Reset(&self->mMonitorInstance[i]);
        }
        ASSERT_DDE0(i < 48);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSoundSys_cSoundSys);
#ifdef SKIP_ASM
class cSoundSys_cSoundSys_cObj {
public:
    virtual void v01(int a);
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
    obj->v01(audioMode);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSoundSys__cSoundSys);
#ifdef SKIP_ASM
class cSoundSys__cSoundSys_cObj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03(int a);
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
    (*(cSoundSys__cSoundSys_cObj**)self)->v03(flags);
    operator_delete(*(int**)self);
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
 * Declared without parameters, but callers pass the sound system as this (func_002B24C0 and func_002B2500 pass
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
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(int a);
};

struct cSoundSys_SetMode_sHolder {
    cSoundSys_SetMode_cObj* obj;
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
extern "C" int cSoundSys_SetMode(cSoundSys_SetMode_sHolder* self, int mode)
{
    return self->obj->v02(mode);
}
#endif

extern void* D_004D3E98[];

//100%
INCLUDE_ASM("sound/bankmonitor", func_002ADFB0__Fi);
#ifdef SKIP_ASM
/**
 * Returns the Pathfinder sound back end's player-slot pointer D_004D3E98[slot] (24 slots).
 *
 * func_002B2488 reads the slot's first word (its sound handle).
 *
 * @param slot  the slot index
 * @return the slot pointer (0 when the slot is unused)
 */
int func_002ADFB0(int slot)
{
    return *(int*)((char*)(void*)D_004D3E98 + slot * 4);
}
#endif

extern "C" void* MEM_free(void* self);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002ADFC8);
#ifdef SKIP_ASM
extern void* (*D_004A378C)(void*, int, int);
extern char str_pathxSND[];

/**
 * Allocates memory through the Pathfinder back end's allocation hook: D_004A378C("pathxSND", size, 0).
 *
 * func_002AE048 installs MEM_alloc as the hook.
 *
 * @param size  the size in bytes
 * @return the block, or 0 when no hook is installed
 */
extern "C" void* func_002ADFC8(int size)
{
    void* block = 0;
    if (D_004A378C != 0) {
        block = D_004A378C(str_pathxSND, size, 0);
    }
    return block;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE000__FPv);
#ifdef SKIP_ASM
/**
 * Frees a block with MEM_free; func_002AE048 installs it as the Pathfinder back end's free hook (D_004A3794).
 *
 * @return MEM_free's result
 */
void* func_002AE000(void* self)
{
    return MEM_free(self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE020);
#ifdef SKIP_ASM
extern void (*D_004A3798)();

/**
 * Sync task of the Pathfinder sound back end: calls the client callback D_004A3798 when one is set.
 *
 * func_002AE048 adds it with SYNCTASK_add(func_002AE020, 5, 100); func_002AE100 removes it.
 *
 * @return 0
 */
extern "C" int func_002AE020()
{
    if (D_004A3798 != 0) {
        D_004A3798();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE048);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
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

struct sBankMonSlot28_E048 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
};
// D_004D3EF8's own type is defined later in the unit; bind a same-layout view.
extern sBankMonSlot28_E048 D_004D3EF8_E048[] __asm__("D_004D3EF8");

/**
 * Starts the Pathfinder sound back end ("pathxSND"): clears its slot tables and installs its hooks and clients.
 *
 * Clears the 24 player-slot pointers D_004D3E98 and resets the 24 bank records D_004D3EF8 (words 0, 1, 2 = -1, word 6 =
 * 0). Stores cb in D_004A3798 and id in D_004A379C, registers id as a 100 Hz sound-system client
 * (SNDSYS_add100hzclient) and func_002AE020 as a sync task (SYNCTASK_add(.., 5, 100)), and installs the hooks
 * D_004A3780 = REAL_abortmessage, D_004A3784 = printf, D_004A378C = MEM_alloc, D_004A3790 = resizeBlock,
 * D_004A3794 = func_002AE000. Called by the Pathfinder library (func_003D25F0) when it opens its first player.
 *
 * @param cb  the callback the sync task func_002AE020 calls
 * @param id  the 100 Hz client to register (a function address)
 */
extern "C" void func_002AE048(void (*cb)(), int id)
{
    for (int i = 0; i < 24; i++) {
        D_004D3E98[i] = 0;
        D_004D3EF8_E048[i].f8 = -1;
        D_004D3EF8_E048[i].f4 = -1;
        D_004D3EF8_E048[i].f0 = -1;
        D_004D3EF8_E048[i].f18 = 0;
    }
    D_004A3798 = cb;
    D_004A379C = id;
    SNDSYS_add100hzclient((void*)id);
    SYNCTASK_add(func_002AE020, 5, 100);
    D_004A3780 = REAL_abortmessage;
    D_004A3784 = printf;
    D_004A378C = MEM_alloc;
    D_004A3790 = resizeBlock;
    D_004A3794 = func_002AE000;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE100);
#ifdef SKIP_ASM
extern "C" int func_002AE020();
extern "C" void SYNCTASK_del(int (*fn)());
extern "C" void SNDSYS_remove100hzclient(int id);
extern void (*D_004A3798)();
extern int D_004A379C;

/**
 * Stops the Pathfinder sound back end: unregisters its 100 Hz client and sync task when a callback is installed.
 *
 * Calls SNDSYS_remove100hzclient(D_004A379C) and SYNCTASK_del(func_002AE020), then clears D_004A3798 and D_004A379C.
 * Called by the Pathfinder library (func_003D2A68) when its last player closes.
 */
extern "C" void func_002AE100()
{
    if (D_004A3798 != 0) {
        SNDSYS_remove100hzclient(D_004A379C);
        SYNCTASK_del(func_002AE020);
        D_004A3798 = 0;
        D_004A379C = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE138);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);

/**
 * Initialises a Pathfinder player slot (0x70 bytes): zeroes it, then sets the handles to -1 and the dirty flag.
 *
 * Words +0x0, +0x4, +0x8 and +0x5C become -1, byte +0x66 (the request count) -1 and byte +0x65 1. Called by
 * func_002AE4C0 on a newly claimed slot.
 */
extern "C" void func_002AE138(void* self)
{
    memset(self, 0, 0x70);
    *(int*)((char*)self + 0x0) = -1;
    *(int*)((char*)self + 0x4) = -1;
    *(char*)((char*)self + 0x66) = -1;
    *(int*)((char*)self + 0x5C) = -1;
    *(int*)((char*)self + 0x8) = -1;
    *(char*)((char*)self + 0x65) = 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE188);
#ifdef SKIP_ASM
/**
 * Clears every entry of the 24 player-slot pointers D_004D3E98 that points at the given slot.
 *
 * func_002AE690 calls it when it closes a slot.
 *
 * @param slotMemory  the slot memory to forget
 */
extern "C" void func_002AE188(void* slotMemory)
{
    for (int i = 0; i < 24; i++) {
        if (D_004D3E98[i] == slotMemory) {
            D_004D3E98[i] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE1B8);
#ifdef SKIP_ASM
/**
 * Returns the index of the first unused Pathfinder player slot (D_004D3E98 entry 0), or -1 when all 24 are used.
 *
 * @return the slot index, or -1
 */
extern "C" int func_002AE1B8(void)
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
INCLUDE_ASM("sound/bankmonitor", func_002AE1F0);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);

/**
 * Clears a Pathfinder bank record (40 bytes of D_004D3EF8) and sets its first three words (bank, handle, slot) to -1.
 *
 * func_002AE690 calls it for each bank of a closed player slot.
 */
extern "C" void func_002AE1F0(int* self)
{
    memset(self, 0, 40);
    self[2] = -1;
    self[1] = -1;
    self[0] = -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE230);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sBankSlot_2AE230 {
    int id;         // 0x0
    int pad4;
    int bank;       // 0x8
    char pad[0x28 - 0xC];
};
// D_004D3EF8's own type is defined later in the unit; bind a same-layout view.
extern sBankSlot_2AE230 D_004D3EF8_2AE230[24] __asm__("D_004D3EF8");

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
extern "C" int func_002AE230(int bank, int id)
{
    sBankSlot_2AE230* record = D_004D3EF8_2AE230;
    for (int i = 0; i < 24; i++, record++) {
        if (bank < 0) {
            if (record->bank < 0) {
                // the do/while(0) wrapper (a compiled-out macro block) is needed to match register allocation
                do {
                    return i;
                } while (0);
            }
        } else if (record->bank == bank) {
            if (id < 0 || record->id == id)
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
INCLUDE_ASM("sound/bankmonitor", func_002AE298);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sBankMonSlot28v {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
};
// sBankMonSlot28 / D_004D3EF8 are defined later in the unit; bind a same-layout view.
extern sBankMonSlot28v D_004D3EF8_v[] __asm__("D_004D3EF8");
extern "C" int func_002AE230(int a, int b);

/**
 * Registers a loaded bank for a Pathfinder player slot: fills the slot's record for that bank, or a free record.
 *
 * Looks up the record with func_002AE230(a, b), else a free one; marks it used (+0x1C = 1) and stores the slot (+0x8),
 * the bank index (+0x0) and the bank handle (+0x4). Called by the Pathfinder library (func_003D2DF8), which first
 * evicts the oldest bank (func_002AE3A8) when memory runs short.
 *
 * @param slotIndex  the player-slot index
 * @param bankName  the bank's name (passed by the library, not used)
 * @param bankIndex  the bank index within the slot
 * @param bankHandle  the loaded bank handle
 * @return c, or -13 when no record is free
 */
extern "C" int func_002AE298(int slotIndex, int bankName, int bankIndex, int bankHandle)
{
    int i = func_002AE230(slotIndex, bankIndex);
    if (i >= 0 || (i = func_002AE230(-1, -1)) >= 0) {
        sBankMonSlot28v* record = &D_004D3EF8_v[i];
        record->f1C = 1;
        record->f8 = slotIndex;
        record->f0 = bankIndex;
        record->f4 = bankHandle;
        return bankHandle;
    }
    return -13;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002AE328);
#ifdef SKIP_ASM
struct sBankMonSlot28 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
};
extern sBankMonSlot28 D_004D3EF8[];
extern "C" int func_002AE230(int a, int b);
extern "C" int SNDbankremove(int id);

/**
 * Unregisters a bank of a Pathfinder player slot: clears its record (D_004D3EF8) when it holds a bank handle.
 *
 * When the record's handle (+0x4) is set it calls SNDbankremove(-1) and resets the record (+0x14 = 0, +0x4, +0x8, +0x0,
 * +0x24 = -1, +0x1C = 0). Quirk: it passes -1 to SNDbankremove, not the record's handle. Called by func_002AE3A8 and
 * the Pathfinder library (func_003D3140).
 *
 * @param slotIndex  the player-slot index
 * @param bankIndex  the bank index within the slot
 * @return 0, or -1 when no record matches
 */
extern "C" int func_002AE328(int slotIndex, int bankIndex)
{
    int none = -1;
    int i = func_002AE230(slotIndex, bankIndex);
    if (i < 0) {
        return -1;
    }
    sBankMonSlot28* record = &D_004D3EF8[i];
    if (record->f4 >= 0) {
        SNDbankremove(-1);
        record->f14 = 0;
        record->f4 = none;
        record->f8 = none;
        record->f0 = none;
        record->f1C = 0;
        record->f24 = none;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002AE3A8);
#ifdef SKIP_ASM
extern sBankMonSlot28 D_004D3EF8[];
extern "C" int func_002AE230(int a, int b);
extern "C" int func_002AE328(int a, int b);

/**
 * Evicts the least recently played bank of a Pathfinder player slot.
 *
 * Walks bank indices 0, 1, .. while the slot has a record for them (func_002AE230) and picks the one with the smallest
 * non-negative play time (+0x24, set by func_002AE7A8 from func_002AF370), then unregisters it with func_002AE328.
 * Called by the Pathfinder library (func_003D2DF8) before it registers a new bank.
 *
 * @param slotIndex  the player-slot index
 * @return 0 when no bank has a play time, otherwise func_002AE328's result
 */
extern "C" int func_002AE3A8(int slotIndex)
{
    int best = 0x7FFFFFF;
    int bestIdx = -1;
    int i = 0;
    sBankMonSlot28* tbl = D_004D3EF8;
    int id;
    while (i < 24 && (id = func_002AE230(slotIndex, i)) >= 0) {
        int playTime = tbl[id].f24;
        if (playTime >= 0 && playTime < best) {
            best = playTime;
            bestIdx = i;
        }
        i++;
    }
    if (bestIdx == -1) {
        return 0;
    }
    return func_002AE328(slotIndex, bestIdx);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE478);
#ifdef SKIP_ASM
extern "C" int SNDSTRM_overhead(int n, int m);

/**
 * Returns the memory a Pathfinder player slot needs: 0x70 + 12 per request, plus SNDSTRM_overhead for a stream.
 *
 * The stream overhead (SNDSTRM_overhead(n, m)) is added only when m > 0. The total is then rounded up to a multiple of
 * 16, always adding 1..16 bytes (an already aligned size grows by 16). Called by the Pathfinder library
 * (func_003D3058).
 *
 * @param n  the number of queued requests
 * @param maxChunks  the packet-player chunk count (0 for a bank-only slot)
 * @return the size in bytes
 */
extern "C" int func_002AE478(int n, int maxChunks)
{
    int size = n * 0xC + 0x70;
    if (maxChunks > 0) {
        size += SNDSTRM_overhead(n, maxChunks);
    }
    return size + (0x10 - (size & 0xF));
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE4C0);
#ifdef SKIP_ASM
struct sBankDesc_2AE4C0 { char c[24]; };
struct sBankEnt_2AE4C0 { int f0; int f4; int f8; };
extern "C" int func_002AE1B8(void);
extern "C" void func_002AE138(void* self);
extern "C" int SNDSTRM_create(void* desc, int n, int m, void* mem, int size);
extern "C" int SNDplaysetdef(void* desc);
extern char str_pct_s_c[];
extern "C" int sprintf(char* buf, const char* fmt, ...);

/**
 * Opens a Pathfinder player slot in the given memory: a sound stream, or a bank player when m is 0.
 *
 * Claims a free slot (func_002AE1B8; -13 when none), points D_004D3E98[slot] at mem, initialises the 0x70-byte header
 * (func_002AE138) and places the request table (count x 12, handles -1) after it, 16-aligned. Copies desc (24 bytes of
 * play options) to +0x30, or SNDplaysetdef's defaults, clears byte +0x35 and stores "%s" of name (11 chars) at +0x1C.
 * For m > 0 it creates the stream with SNDSTRM_create(+0x30, count, m, rest of mem) into +0x0; otherwise it sets byte
 * +0x64 (bank player). Ends with the request count in byte +0x66, +0x14 = 127 and +0x10 = 0. Called by the Pathfinder
 * library (func_003D25F0).
 *
 * @param name  the player name (11 characters kept)
 * @param desc  play options to copy, or 0 for the defaults
 * @param count  the number of queued requests
 * @param maxChunks  the packet-player chunk count; 0 makes a bank player
 * @param mem  the memory for the slot (func_002AE478 bytes)
 * @param size  its size in bytes
 * @return the slot index, -13 when none is free, or SNDSTRM_create's error
 */
extern "C" int func_002AE4C0(char* name, sBankDesc_2AE4C0* desc, int count, int maxChunks, char* mem, int size)
{
    int slot = func_002AE1B8();
    if (slot < 0)
        return -13;
    D_004D3E98[slot] = mem;
    mem += 0x70;
    func_002AE138(D_004D3E98[slot]);
    char* slotData = (char*)D_004D3E98[slot];
    *(char**)(slotData + 0x6C) = mem;
    mem = count * 12 + mem;
    int off = mem - slotData;
    mem = mem + (0x10 - off % 16);
    size -= mem - slotData;
    int i;
    for (i = 0; i < count; i++)
        (*(sBankEnt_2AE4C0**)(slotData + 0x6C))[i].f4 = -1;
    if (desc)
        *(sBankDesc_2AE4C0*)(slotData + 0x30) = *desc;
    else
        SNDplaysetdef(slotData + 0x30);
    *(char*)(slotData + 0x35) = 0;
    sprintf(slotData + 0x1C, str_pct_s_c, 11, name);
    if (maxChunks > 0)
        *(int*)(slotData + 0x0) = SNDSTRM_create(slotData + 0x30, count, maxChunks, mem, size);
    else
        *(char*)(slotData + 0x64) = 1;
    if (*(int*)(slotData + 0x0) < 0 && *(signed char*)(slotData + 0x64) == 0)
        return *(int*)(slotData + 0x0);
    *(char*)(slotData + 0x66) = count;
    *(int*)(slotData + 0x14) = 0x7F;
    *(int*)(slotData + 0x10) = 0;
    return slot;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002AE690);
#ifdef SKIP_ASM
extern void* (*D_004A3794)(void*);
extern "C" void SNDSTRM_destroy(int id);

/**
 * Closes a Pathfinder player slot: frees a bank player's banks or destroys the stream, then forgets the slot.
 *
 * For a bank player (byte +0x64) it walks the slot's bank records (func_002AE230(idx, -1)), passes each record's memory
 * (+0x14) to the free hook D_004A3794 and clears the record (func_002AE1F0); for a stream it calls SNDSTRM_destroy on
 * its handle. Then clears the slot pointer (func_002AE188). Called by the Pathfinder library (func_003D2A68).
 *
 * @param idx  the player-slot index
 */
extern "C" void func_002AE690(int idx)
{
    void* slotData = D_004D3E98[idx];
    if (*(signed char*)((char*)slotData + 0x64) != 0) {
        int id = func_002AE230(idx, -1);
        while (id >= 0) {
            sBankMonSlot28* record = &D_004D3EF8[id];
            if (D_004A3794 != 0 && record->f14 != 0) {
                D_004A3794((void*)record->f14);
            }
            func_002AE1F0((int*)record);
            id = func_002AE230(idx, -1);
        }
    } else {
        SNDSTRM_destroy(*(int*)slotData);
    }
    func_002AE188(slotData);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE768);
#ifdef SKIP_ASM
/**
 * Sets a stream player slot's preloaded data: slot +0x28 = a1 and +0x2C = a2.
 *
 * Only for an open stream slot (index < 24, not a bank player). func_002AE7A8 then queues offsets below that size from
 * memory (func_003B7E10) instead of the file. Called by the Pathfinder library (func_003D30A8).
 *
 * @param idx  the player-slot index
 * @param preloadData  the preloaded data
 * @param preloadSize  its size in bytes
 */
extern "C" void func_002AE768(unsigned int idx, int preloadData, int preloadSize)
{
    if (idx < 24) {
        void* slotData = D_004D3E98[idx];
        if (slotData != 0 && *(signed char*)((char*)slotData + 0x64) == 0) {
            *(int*)((char*)slotData + 0x28) = preloadData;
            *(int*)((char*)slotData + 0x2c) = preloadSize;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE7A8);
#ifdef SKIP_ASM
struct sBankEnt_2AE7A8 { int f0; int f4; int f8; };
extern "C" int func_002AE230(int a, int b);
// PORT: the unit defines func_002AF370 as void; its body returns __fixunssfsi's result.
unsigned int func_002AF370_r(void) __asm__("func_002AF370");
extern "C" int func_002AF6C0(int idx, int v);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int SNDBANK_play(int h, int id, void* desc);
extern "C" int SNDSTRM_queuefile(int h, int a, int b, unsigned int t);
// PORT: the unit declares SNDSTRM_purge as void; the caller leaves $v0 live (int return).
int SNDSTRM_purge_r(int) __asm__("SNDSTRM_purge");
extern "C" int func_003B7E10(int h, int a, int b, unsigned int t);

/**
 * Plays a sound on a Pathfinder player slot: a bank patch on a bank player, or a queued file section on a stream.
 *
 * A vol >= 0 is stored in the slot's play options (+0x30) and applied (func_002AF6C0). Bank player: key's high 16 bits
 * select the bank (record found with func_002AE230; -1 when it has no handle), the patch is (key & 0xFFFF) + a6 - 1 (a6
 * <= -1 counts as 0); SNDBANK_play inside the critical section, the record's play time (+0x24) becomes func_002AF370()
 * and the slot's handle the bank's; request 0 gets the sound handle and a4.
 *
 * Stream: the file offset is key * 128. With a7 set it purges the stream (SNDSTRM_purge) and clears the older requests.
 * Offsets inside the preloaded data (+0x28/+0x2C, func_002AE768) are queued from memory (func_003B7E10), others with
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
extern "C" int func_002AE7A8(int idx, int holdTime, int fileName, unsigned int key, int requestLength, signed char vol, int patchOffset, int purgeFirst)
{
    int bankHandle = -1;
    // NOTE: retail rematerialises `li v0,1` right before each 0x65 store (after unrelated loads):
    // a function-scope local set early whose pseudo gets no callee-saved register (all nine are taken),
    // so reload re-creates the constant at each use.
    signed char dirty = 1;
    char* slotData = (char*)D_004D3E98[idx];
    if (vol >= 0)
    {
        *(signed char*)(slotData + 0x30) = vol;
        func_002AF6C0(idx, vol);
    }
    int playHandle;
    if (*(signed char*)(slotData + 0x64) != 0)
    {
        if (patchOffset <= bankHandle)
            patchOffset = 0;
        int bankRecord = func_002AE230(idx, key >> 16);
        if (bankRecord >= 0)
            bankHandle = D_004D3EF8[bankRecord].f4;
        if (bankHandle < 0)
            return -1;
        *(int*)(slotData + 4) = (key & 0xFFFF) + patchOffset - 1;
        SNDSYS_entercritical();
        playHandle = SNDBANK_play(bankHandle, *(int*)(slotData + 4), slotData + 0x30);
        SNDSYS_leavecritical();
        D_004D3EF8[bankRecord].f24 = func_002AF370_r();
        *(int*)(slotData + 0) = bankHandle;
        (*(sBankEnt_2AE7A8**)(slotData + 0x6C))[0].f4 = playHandle;
        (*(sBankEnt_2AE7A8**)(slotData + 0x6C))[0].f0 = requestLength;
        *(signed char*)(slotData + 0x65) = dirty;
    }
    else
    {
        unsigned int fileOffset = key << 7;
        int j;
        if (purgeFirst)
        {
            SNDSTRM_purge_r(*(int*)(slotData + 0));
            for (j = *(signed char*)(slotData + 0x66) - 1; j > 0; j--)
            {
                (*(sBankEnt_2AE7A8**)(slotData + 0x6C))[j - 1].f4 = -1;
                (*(sBankEnt_2AE7A8**)(slotData + 0x6C))[j - 1].f0 = -1;
            }
        }
        if (*(int*)(slotData + 0x28) && fileOffset < *(unsigned int*)(slotData + 0x2C))
            playHandle = func_003B7E10(*(int*)(slotData + 0), holdTime, *(int*)(slotData + 0x28), fileOffset);
        else
            playHandle = SNDSTRM_queuefile(*(int*)(slotData + 0), holdTime, fileName, fileOffset);
        if (playHandle >= 0)
        {
            for (j = *(signed char*)(slotData + 0x66) - 1; j > 0; j--)
                (*(sBankEnt_2AE7A8**)(slotData + 0x6C))[j] = (*(sBankEnt_2AE7A8**)(slotData + 0x6C))[j - 1];
            (*(sBankEnt_2AE7A8**)(slotData + 0x6C))[0].f4 = playHandle;
            (*(sBankEnt_2AE7A8**)(slotData + 0x6C))[0].f0 = requestLength;
        }
        *(signed char*)(slotData + 0x65) = dirty;
    }
    return playHandle;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE9F8);
#ifdef SKIP_ASM
extern "C" void SNDSTRM_modifyhold(void*);

/**
 * Releases the newest queued request of a stream player slot (SNDSTRM_modifyhold on request 0's handle).
 *
 * Nothing for a bank player. Declared with one parameter, but the hold time passes through in the second argument
 * register (the Pathfinder library, func_003D4F10, calls it with (slot, 0), which starts playback).
 *
 * @param i  the player-slot index
 */
extern "C" void func_002AE9F8(int i)
{
    void* slotData = D_004D3E98[i];
    if (*(signed char*)((char*)slotData + 0x64) == 0) {
        SNDSTRM_modifyhold(*(void**)((char*)*(void**)((char*)slotData + 0x6C) + 0x4));
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AEA38);
#ifdef SKIP_ASM
extern "C" void func_002AEFA8(int a0);

/**
 * Returns a Pathfinder player slot's +0x50 status word, refreshing every slot's status first when this one is dirty.
 *
 * The refresh is func_002AEFA8(0) (slot byte +0x65 set). No caller found.
 *
 * @param idx  the player-slot index
 * @return slot +0x50 (the played amount the status refresh sums)
 */
extern "C" int func_002AEA38(int idx)
{
    void* slotData = D_004D3E98[idx];
    if (slotData != 0) {
        if (*(char*)((char*)slotData + 0x65) != 0) {
            func_002AEFA8(0);
        }
    }
    return *(int*)((char*)D_004D3E98[idx] + 0x50);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AEAA8);
#ifdef SKIP_ASM
extern "C" void func_002AEFA8(int a0);

struct sMonEntC { int f0; int f4; int f8; };

/**
 * Returns a request's progress on a Pathfinder player slot, refreshing the slots' status first when it is dirty.
 *
 * With sub < 0 it returns slot +0x54 (the total the refresh adds up); otherwise request sub's position (table entry
 * +0x8) when that request has a handle. Called by the Pathfinder library (func_003D41A8, func_003D4950).
 *
 * @param idx  the player-slot index
 * @param sub  the request index, or < 0 for the slot total
 * @return the position, or -8 for an unknown or finished request
 */
extern "C" int func_002AEAA8(int idx, int sub)
{
    void* slotData = D_004D3E98[idx];
    if (slotData != 0 && *(signed char*)((char*)slotData + 0x65) != 0) {
        func_002AEFA8(0);
    }
    if (sub < 0) {
        return *(int*)((char*)slotData + 0x54);
    }
    if (sub < *(signed char*)((char*)slotData + 0x66)) {
        if ((*(sMonEntC**)((char*)slotData + 0x6C))[sub].f4 >= 0) {
            return (*(sMonEntC**)((char*)slotData + 0x6C))[sub].f8;
        }
        return -8;
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AEB40);
#ifdef SKIP_ASM
extern "C" void func_002AEFA8(int a0);

struct sEntryEB40 { int pad0; int key; int value; };

/**
 * Returns the position of the request with the given handle on a Pathfinder player slot (-8 when not queued).
 *
 * Refreshes every slot's status first when this one is dirty (func_002AEFA8(0)), then searches the request table for
 * handle key (entry +0x4) and returns its position (+0x8). No caller found.
 *
 * @param idx  the player-slot index
 * @param key  the request handle
 * @return the position, or -8
 */
extern "C" int func_002AEB40(int idx, int key)
{
    void* ch = D_004D3E98[idx];
    if (ch != 0 && *(char*)((char*)ch + 0x65) != 0) {
        func_002AEFA8(0);
    }
    for (int i = 0; i < *(signed char*)((char*)ch + 0x66); i++) {
        sEntryEB40* request = &(*(sEntryEB40**)((char*)ch + 0x6C))[i];
        if (request->key == key) {
            return request->value;
        }
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AEBD0);
#ifdef SKIP_ASM
extern "C" void func_002AEFA8(int a0);

/**
 * Returns whether a Pathfinder player slot is still busy (+0x58), refreshing the slots' status first when dirty.
 *
 * Called by the Pathfinder library (func_003D4F10).
 *
 * @param idx  the player-slot index
 * @return slot +0x58, or -8 for an unused slot
 */
extern "C" int func_002AEBD0(int idx)
{
    if (D_004D3E98[idx] != 0) {
        if (*(signed char*)((char*)D_004D3E98[idx] + 0x65) != 0) {
            func_002AEFA8(0);
        }
        return *(int*)((char*)D_004D3E98[idx] + 0x58);
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AEC28);
#ifdef SKIP_ASM
/**
 * Marks a Pathfinder player slot dirty (+0x65 = 1) so the next query refreshes its status.
 *
 * Called by the Pathfinder library (func_003CFC30).
 *
 * @param idx  the player-slot index
 * @return 1, or -8 for an unused slot
 */
extern "C" int func_002AEC28(int idx)
{
    void* slotData = D_004D3E98[idx];
    if (slotData == 0) {
        return -8;
    }
    *(char*)((char*)slotData + 0x65) = 1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AEC58);
#ifdef SKIP_ASM
extern void* D_004D3E98[];
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int func_003B6670(int h, int sub, void* hdr, void* buf, unsigned int* pos);
extern "C" int SNDtimeremaining(int h);
extern "C" int func_003B86C0(int h);
extern "C" int SNDSTRM_status(int h, void* out);
// PORT: the unit declares SNDSTRM_requeststatus(int, int); the second argument is an out-pointer
extern "C" void SNDSTRM_requeststatus(int h, int v);
extern "C" int func_003B7F18(int h);

struct sMonSub_EC58 {
    int size;   // 0x0
    int id;     // 0x4
    int pos;    // 0x8
};

struct sMonSlot_EC58 {
    int h;              // 0x0
    int sub;            // 0x4
    char pad08[0x4];
    int status;         // 0xC
    char pad10[0x38];
    int f48;            // 0x48
    int f4C;            // 0x4C
    int f50;            // 0x50
    int f54;            // 0x54
    int busy;           // 0x58
    int f5C;            // 0x5C
    int f60;            // 0x60
    signed char isMem;  // 0x64
    signed char dirty;  // 0x65
    signed char count;  // 0x66
    char pad67[0x5];
    sMonSub_EC58* subs; // 0x6C
};

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
 * Same work as one iteration of func_002AEFA8: a bank player reads the patch's sample position (func_003B6670) and the
 * time left (SNDtimeremaining x 10 ms); a stream reads SNDSTRM_status and each request's SNDSTRM_requeststatus, summing
 * positions into +0x54 and +0x50, updating busy (+0x58), the current handle (+0x5C) and the current volume (+0xC). Sets
 * +0x58 to 1 first, returns -1 for an unused slot and -2 for one without a handle.
 *
 * Likely bugs: in the stream path it tests the out-pointers themselves (outA == 0 && outB == 0) where func_002AEFA8
 * tests the values, and at the end it stores the pointers, not the values, into +0x48/+0x4C. Called by the Pathfinder
 * library (func_003D32B8).
 *
 * @param idx  the player-slot index
 * @param outA  receives the elapsed time (ms)
 * @param outB  receives the remaining time or request position
 * @return 0, or -1 / -2 as above
 */
// PORT: the end of this function stores the out-pointers themselves into the slot (as in the original)
extern "C" int func_002AEC58(int idx, int* outA, int* outB)
{
    sMonSlot_EC58* slotData = (sMonSlot_EC58*)D_004D3E98[idx];
    *outA = 0;
    *outB = 0;
    if (slotData == 0) {
        return -1;
    }
    int statusResult = -8;
    slotData->busy = 1;
    if (slotData->h < 0) {
        return -2;
    }
    slotData->dirty = 1;
    SNDSYS_entercritical();
    sStrmInfo_EC58 info;
    sStrmState_EC58 st;
    sStrmHdr_EC58 hdr;
    char buf[0x70];
    sStrmPos_EC58 pos;
    if (slotData->isMem != 0) {
        if (slotData->sub >= 0 && slotData->subs->id >= 0) {
            statusResult = func_003B6670(slotData->h, slotData->sub, &hdr, buf, &pos.samples);
            if (statusResult >= 0) {
                *outB = SNDtimeremaining(slotData->subs->id);
                if (*outB < 0) {
                    *outB = 0;
                    slotData->subs->id = -1;
                    goto done;
                }
                *outB *= 10;
                *outA = (int)((float)pos.samples / (float)hdr.rate * 1000.0f) - *outB;
            }
            slotData->subs->pos = *outB;
            slotData->f50 = slotData->subs->size;
            slotData->status = func_003B86C0(slotData->subs->id);
            slotData->f5C = slotData->subs->id;
        }
    } else {
        statusResult = SNDSTRM_status(slotData->h, &info);
        slotData->f50 = 0;
        if (info.count >= slotData->count) {
            slotData->busy = 0;
        }
        for (int j = 0; j < slotData->count; j++) {
            slotData->subs[j].pos = 0;
            if (slotData->subs[j].id < 0) {
                continue;
            }
            SNDSTRM_requeststatus(slotData->subs[j].id, (int)&st);
            switch (st.state) {
            case 1:
                slotData->subs[j].pos = slotData->subs[j].size;
                slotData->f54 += slotData->subs[j].size;
                slotData->f50 += st.c;
                slotData->busy = 0;
                slotData->f5C = slotData->subs[j].id;
                break;
            case 0:
            case 2:
                *outA = st.a;
                *outB = st.b;
                if (outA == 0 && outB == 0) {
                    *outA = 0;
                    *outB = slotData->subs[j].size;
                }
                slotData->subs[j].pos = *outB;
                slotData->f54 += *outB;
                slotData->f50 += st.c;
                if (st.c < st.b) {
                    slotData->busy = 0;
                }
                slotData->f5C = slotData->subs[j].id;
                break;
            default:
                slotData->subs[j].id = -1;
                break;
            }
        }
        slotData->status = func_003B7F18(slotData->h);
    }
done:
    slotData->f48 = (int)outA;
    slotData->f4C = (int)outB;
    slotData->f60 = statusResult;
    slotData->dirty = 0;
    SNDSYS_leavecritical();
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AEFA8);
#ifdef SKIP_ASM
extern void* D_004D3E98[];
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int func_003B6670(int h, int sub, void* hdr, void* buf, unsigned int* pos);
extern "C" int SNDtimeremaining(int h);
extern "C" int func_003B86C0(int h);
extern "C" int SNDSTRM_status(int h, void* out);
// PORT: the unit declares SNDSTRM_requeststatus(int, int); the second argument is an out-pointer
extern "C" void SNDSTRM_requeststatus(int h, int v);
extern "C" int func_003B7F18(int h);

struct sMonSub_EFA8 {
    int size;   // 0x0
    int id;     // 0x4
    int pos;    // 0x8
};

struct sMonSlot_EFA8 {
    int h;              // 0x0
    int sub;            // 0x4
    char pad08[0x4];
    int status;         // 0xC
    char pad10[0x38];
    int f48;            // 0x48
    int f4C;            // 0x4C
    int f50;            // 0x50
    int f54;            // 0x54
    int busy;           // 0x58
    int f5C;            // 0x5C
    int f60;            // 0x60
    signed char isMem;  // 0x64
    signed char dirty;  // 0x65
    signed char count;  // 0x66
    char pad67[0x5];
    sMonSub_EFA8* subs; // 0x6C
};

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
 * slot: a bank player reads its patch's sample position (func_003B6670) and remaining time (SNDtimeremaining x 10 ms),
 * giving the elapsed ms (+0x48) and remaining ms (+0x4C), and its volume (+0xC, func_003B86C0); a stream reads
 * SNDSTRM_status and each request's SNDSTRM_requeststatus, adding finished and playing requests into +0x54 / +0x50,
 * dropping unknown ones, and records the current volume (+0xC, func_003B7F18). Clears busy (+0x58) when nothing is
 * pending, stores the status result in +0x60 and clears the dirty byte (+0x65).
 *
 * The slot queries (func_002AEA38, func_002AEAA8, func_002AEB40, func_002AEBD0, func_002AF2F0, func_002AF428,
 * func_002AF6C0) call it with 0 when their slot is dirty; the Pathfinder library calls it too (func_003D5128).
 *
 * @param reset  nonzero to only mark the slots dirty
 */
extern "C" void func_002AEFA8(int reset)
{
    SNDSYS_entercritical();
    for (int i = 0; i < 24; i++) {
        sMonSlot_EFA8* slotData = (sMonSlot_EFA8*)D_004D3E98[i];
        if (slotData == 0) {
            continue;
        }
        if (reset != 0) {
            slotData->f60 = 0;
            slotData->dirty = 1;
            continue;
        }
        int f60 = -8;
        int elapsedMs = 0;
        int f4C = 0;
        slotData->busy = 1;
        if (slotData->h >= 0) {
            sStrmInfo_EFA8 info;
            sStrmState_EFA8 st;
            sStrmHdr_EFA8 hdr;
            char buf[0x70];
            sStrmPos_EFA8 pos;
            if (slotData->isMem != 0) {
                if (slotData->sub >= 0 && slotData->subs->id >= 0) {
                    f60 = func_003B6670(slotData->h, slotData->sub, &hdr, buf, &pos.samples);
                    if (f60 >= 0) {
                        f4C = SNDtimeremaining(slotData->subs->id);
                        if (f4C < 0) {
                            f4C = 0;
                            slotData->subs->id = -1;
                            goto done;
                        }
                        f4C *= 10;
                        elapsedMs = (int)((float)pos.samples / (float)hdr.rate * 1000.0f) - f4C;
                    }
                    slotData->subs->pos = f4C;
                    slotData->f50 = slotData->subs->size;
                    slotData->status = func_003B86C0(slotData->subs->id);
                    slotData->f5C = slotData->subs->id;
                }
            } else {
                f60 = SNDSTRM_status(slotData->h, &info);
                slotData->f50 = 0;
                if (info.count >= slotData->count) {
                    slotData->busy = 0;
                }
                for (int j = 0; j < slotData->count; j++) {
                    slotData->subs[j].pos = 0;
                    if (slotData->subs[j].id < 0) {
                        continue;
                    }
                    SNDSTRM_requeststatus(slotData->subs[j].id, (int)&st);
                    switch (st.state) {
                    case 1:
                        slotData->subs[j].pos = slotData->subs[j].size;
                        slotData->f54 += slotData->subs[j].size;
                        slotData->f50 += st.c;
                        slotData->busy = 0;
                        slotData->f5C = slotData->subs[j].id;
                        break;
                    case 0:
                    case 2:
                        elapsedMs = st.a;
                        f4C = st.b;
                        if (elapsedMs == 0 && f4C == 0) {
                            elapsedMs = f4C;
                            f4C = slotData->subs[j].size;
                        }
                        slotData->subs[j].pos = f4C;
                        slotData->f54 += f4C;
                        slotData->f50 += st.c;
                        if (st.c < st.b) {
                            slotData->busy = 0;
                        }
                        slotData->f5C = slotData->subs[j].id;
                        break;
                    default:
                        slotData->subs[j].id = -1;
                        break;
                    }
                }
                slotData->status = func_003B7F18(slotData->h);
            }
        }
    done:
        slotData->f48 = elapsedMs;
        slotData->f4C = f4C;
        slotData->f60 = f60;
        slotData->dirty = 0;
    }
    SNDSYS_leavecritical();
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF2F0);
#ifdef SKIP_ASM
extern "C" void func_002AEFA8(int a0);

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
extern "C" int func_002AF2F0(int idx, int* elapsedOut, int* remainingOut)
{
    void* slotData = D_004D3E98[idx];
    if (slotData == 0) {
        return -8;
    }
    if (*(signed char*)((char*)slotData + 0x65) != 0) {
        func_002AEFA8(0);
    }
    *elapsedOut = *(int*)((char*)slotData + 0x48);
    *remainingOut = *(int*)((char*)slotData + 0x4C);
    if (*(int*)((char*)slotData + 0x60) != 0) {
        return *(int*)((char*)slotData + 0x60);
    }
    return *(int*)((char*)slotData + 0x5C);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF370);
#ifdef SKIP_ASM
extern "C" int TIMER_gettick();
extern "C" void fptoui(float);
extern int TIMERhz[];

/**
 * Returns the current time in ms: TIMER_gettick() / TIMERhz[0] * 1000, converted to unsigned int.
 *
 * Declared void, but the __fixunssfsi result (fptoui) is returned in v0; callers use it (func_002AE7A8 stamps
 * bank play times with it, and the Pathfinder library).
 */
extern "C" void func_002AF370(void)
{
    fptoui((float)TIMER_gettick() / (float)TIMERhz[0] * 1000.0f);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF3B8);
#ifdef SKIP_ASM
/**
 * Returns a bank player slot's current bank handle (slot +0x0), or -1 for an unused slot or a stream.
 *
 * @param slotIndex  the player-slot index
 * @return the handle, or -1
 */
extern "C" int func_002AF3B8(int slotIndex) {
    void *temp_4;

    temp_4 = D_004D3E98[slotIndex];
    if ((temp_4 != 0) && ((*(signed char *)((char*)(temp_4) + (0x64))) != 0)) {
        return (*(int *)((char*)(temp_4) + (0)));
    }
    return -1;
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF3F0);
#ifdef SKIP_ASM
/**
 * Returns a stream player slot's stream handle (slot +0x0), or -1 for an unused slot or a bank player.
 *
 * @param slotIndex  the player-slot index
 * @return the handle, or -1
 */
extern "C" int func_002AF3F0(int slotIndex) {
    void *temp_4;

    temp_4 = D_004D3E98[slotIndex];
    if ((temp_4 != 0) && ((*(signed char *)((char*)(temp_4) + (0x64))) == 0)) {
        return (*(int *)((char*)(temp_4) + (0)));
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF428);
#ifdef SKIP_ASM
extern "C" void func_002AEFA8(int a0);

/**
 * Returns a Pathfinder player slot's current volume (+0xC), refreshing the slots' status first when it is dirty.
 *
 * Called by the Pathfinder library (func_003D41A8, func_003D4F10, func_003D5968, func_003D6378).
 *
 * @param idx  the player-slot index
 * @return the volume, or -8 for an unused slot
 */
extern "C" int func_002AF428(int idx)
{
    if (D_004D3E98[idx] != 0) {
        if (*(signed char*)((char*)D_004D3E98[idx] + 0x65) != 0) {
            func_002AEFA8(0);
        }
        return *(int*)((char*)D_004D3E98[idx] + 0xC);
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF480);
#ifdef SKIP_ASM
/**
 * Returns a Pathfinder player slot's dry level (+0x14, set by func_002AF580), or -8 for an unused slot.
 *
 * @param idx  the player-slot index
 * @return the dry level, or -8
 */
extern "C" int func_002AF480(int idx)
{
    void* slotData = D_004D3E98[idx];
    if (slotData != 0) {
        return *(int*)((char*)slotData + 0x14);
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF4B0);
#ifdef SKIP_ASM
/**
 * Returns a Pathfinder player slot's effect send level (+0x10, set by func_002AF4E0), or -8 for an unused slot.
 *
 * @param idx  the player-slot index
 * @return the effect level, or -8
 */
extern "C" int func_002AF4B0(int idx)
{
    void* slotData = D_004D3E98[idx];
    if (slotData != 0) {
        return *(int*)((char*)slotData + 0x10);
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF4E0);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int func_003B8050(void* h, int a, int b);
extern "C" int func_003B88C8(void* h, int a, int b);

/**
 * Sets a Pathfinder player slot's effect send level: func_003B88C8 for a bank player, func_003B8050 for a stream.
 *
 * Runs inside the critical section and records the level in slot +0x10. Called by the Pathfinder library
 * (func_003D5CC0) with bus 0.
 *
 * @param idx  the player-slot index
 * @param effectBus  the effect bus
 * @param sendLevel  the send level
 * @return the driver call's result, or -8 for an unused slot
 */
extern "C" int func_002AF4E0(int idx, int effectBus, int sendLevel)
{
    void* slotData = D_004D3E98[idx];
    int result = -8;
    if (slotData != 0) {
        SNDSYS_entercritical();
        if (*(signed char*)((char*)slotData + 0x64) != 0) {
            result = func_003B88C8(*(void**)slotData, effectBus, sendLevel);
        } else {
            result = func_003B8050(*(void**)slotData, effectBus, sendLevel);
        }
        SNDSYS_leavecritical();
        *(int*)((char*)slotData + 0x10) = sendLevel;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF580);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int func_003B8010(void* h, int a);
extern "C" int func_003B8700(void* h, int a);

/**
 * Sets a Pathfinder player slot's dry level: func_003B8700 for a bank player, func_003B8010 for a stream.
 *
 * Runs inside the critical section and records the level in slot +0x14. Called by the Pathfinder library
 * (func_003D5EE8).
 *
 * @param idx  the player-slot index
 * @param dryLevel  the dry level
 * @return the driver call's result, or -8 for an unused slot
 */
extern "C" int func_002AF580(int idx, int dryLevel)
{
    void* slotData = D_004D3E98[idx];
    int result = -8;
    if (slotData != 0) {
        SNDSYS_entercritical();
        if (*(signed char*)((char*)slotData + 0x64) != 0) {
            result = func_003B8700(*(void**)slotData, dryLevel);
        } else {
            result = func_003B8010(*(void**)slotData, dryLevel);
        }
        SNDSYS_leavecritical();
        *(int*)((char*)slotData + 0x14) = dryLevel;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF608);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" int SNDautovol(int h, int v, int c);
// PORT: the unit declares func_003B7EB0 as void; this caller uses its result.
int func_003B7EB0_r(int h, int v, int c) __asm__("func_003B7EB0");

/**
 * Starts a volume fade on a Pathfinder player slot unless busy is set.
 *
 * Inside the critical section it records the target in slot +0x8 and fades a bank player's sound with
 * SNDautovol(handle, ms / 10 ticks, target) or a stream with func_003B7EB0(stream, ms, target). Called by the
 * Pathfinder library (func_003D6318).
 *
 * @param idx  the player-slot index
 * @param targetVolume  the target volume
 * @param fadeMs  the fade time in ms
 * @param busy  nonzero to do nothing
 * @return the driver call's result, or 0
 */
extern "C" int func_002AF608(int idx, int targetVolume, int fadeMs, char busy)
{
    void* slotData = D_004D3E98[idx];
    int driverResult = 0;
    if (busy == 0) {
        SNDSYS_entercritical();
        *(int*)((char*)slotData + 0x8) = targetVolume;
        if (*(signed char*)((char*)slotData + 0x64) != 0) {
            driverResult = SNDautovol(*(int*)((char*)*(void**)((char*)slotData + 0x6C) + 0x4), fadeMs / 10, targetVolume);
        } else {
            driverResult = func_003B7EB0_r(*(int*)slotData, fadeMs, targetVolume);
        }
        SNDSYS_leavecritical();
    }
    return driverResult;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF6C0);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_002AEFA8(int a0);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDvol(int h, int v);
extern "C" void func_003B7E70(int h, int v);

/**
 * Sets a Pathfinder player slot's volume when no fade is pending; only while the audio system's unk_6280 is set.
 *
 * Returns 0 at once when gSSXAudio's unk_6280 (cAudioInterface_bxAudio) is 0. Refreshes the status when the slot is
 * dirty; when the fade target (+0x8) is unset or reached it clears it and, if v differs from the current volume (+0xC)
 * and both are >= 0, applies v with SNDvol (bank player) or func_003B7E70 (stream) inside the critical section and
 * stores it. A pending fade makes it return -8. Called by func_002AE7A8 and the Pathfinder library.
 *
 * @param idx  the player-slot index
 * @param volume  the new volume
 * @return 0, or -8 while a fade is pending
 */
extern "C" int func_002AF6C0(int idx, int volume)
{
    void* slotData = D_004D3E98[idx];
    int result = 0;
    if (cAudioInterface_bxAudio()->mPathfinderCanChangeVolume == 0) return 0;
    if (*(signed char*)((char*)slotData + 0x65) != 0) {
        func_002AEFA8(0);
    }
    int cur = *(int*)((char*)slotData + 0xC);
    if (*(int*)((char*)slotData + 0x8) < 0 || *(int*)((char*)slotData + 0x8) == cur) {
        *(int*)((char*)slotData + 0x8) = -1;
        if (cur != volume && volume >= 0 && cur >= 0) {
            SNDSYS_entercritical();
            if (*(signed char*)((char*)slotData + 0x64) != 0) {
                SNDvol(*(int*)((char*)*(void**)((char*)slotData + 0x6C) + 0x4), volume);
            } else {
                func_003B7E70(*(int*)slotData, volume);
            }
            SNDSYS_leavecritical();
            *(int*)((char*)slotData + 0xC) = volume;
        }
    } else {
        result = -8;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF7A0);
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
 * the critical section, then marks the slot dirty. Called by the Pathfinder library (func_003D1038).
 *
 * @param idx  the player-slot index
 * @param reset  nonzero to pause (pitch 0), 0 to restore the saved pitch
 * @return 0
 */
extern "C" int func_002AF7A0(int idx, int reset)
{
    void* slotData = D_004D3E98[idx];
    unsigned short pitchMult;
    if (reset != 0) {
        pitchMult = 0;
        *(int*)((char*)slotData + 0x18) = *(unsigned short*)((char*)slotData + 0x3C);
    } else {
        pitchMult = *(int*)((char*)slotData + 0x18);
    }
    SNDSYS_entercritical();
    if (*(signed char*)((char*)slotData + 0x64) != 0) {
        SNDpitchmult(*(int*)((char*)*(void**)((char*)slotData + 0x6C) + 4), pitchMult);
    } else {
        SNDSTRM_pitchmult(*(int*)slotData, pitchMult);
    }
    SNDSYS_leavecritical();
    *(char*)((char*)slotData + 0x65) = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF838);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSTRM_purge(int);
extern "C" void SNDstop(int);

/**
 * Stops a Pathfinder player slot: SNDstop on a bank player's request-0 sound, SNDSTRM_purge on a stream.
 *
 * Inside the critical section; then marks the slot dirty. Called by the Pathfinder library (func_003D1470,
 * func_003D5968).
 *
 * @param idx  the player-slot index
 * @return 0
 */
extern "C" int func_002AF838(int idx)
{
    void* slotData = D_004D3E98[idx];
    SNDSYS_entercritical();
    if (*(signed char*)((char*)slotData + 0x64) != 0) {
        SNDstop(*(int*)((char*)*(void**)((char*)slotData + 0x6C) + 0x4));
    } else {
        SNDSTRM_purge(*(int*)((char*)slotData + 0x0));
    }
    SNDSYS_leavecritical();
    *(char*)((char*)slotData + 0x65) = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF8A8);
#ifdef SKIP_ASM
extern "C" int func_002B0F48(cSpeechSys* self, int a1);
extern int D_004A37BC;

// PORT: real signature is int(int); the unit later declares func_002AF8A8 as void() for its callback use.
extern "C" int func_002AF8A8_impl(int a) __asm__("func_002AF8A8");
/**
 * The speech driver's start callback: plays a speech event through cSpeechSys (func_002B0F48 on D_004A37B4).
 *
 * Returns -1 without playing while D_004A37BC is set (func_002B1128 sets it while it flushes a channel). SPCH_Init
 * registers it with the speech driver (func_003D6840).
 *
 * @param speechEvent  the speech event, passed on to func_002B0F48
 * @return func_002B0F48's result (the started speech's handle, or -1)
 */
extern "C" int func_002AF8A8_impl(int speechEvent)
{
    if (D_004A37BC != 0) {
        return -1;
    }
    return func_002B0F48((cSpeechSys*)D_004A37B4, speechEvent);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechBank_cSpeechBank__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a cSpeechBank: unk_00 = -1, mOwnsData = 0, mpData = 0 and an empty mName.
 *
 * func_002AF960 runs it on cMemMan_alloc(0x4C, "SpeechBank").
 *
 * @return self
 */
void* cSpeechBank_cSpeechBank(void* vself)
{
    cSpeechBank* self = (cSpeechBank*)vself;
    int zero = 0;
    self->mOwnsData = zero;
    *(int*)self = -1;
    self->mpData = (void*)zero;
    self->mName[0] = (signed char)zero;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechBank__cSpeechBank);
#ifdef SKIP_ASM
extern "C" void func_003D6CA0(void* a0);

/**
 * Destroys a cSpeechBank: releases its driver bank (func_003D6CA0(unk_00)) and frees mpData when mOwnsData.
 *
 * Deletes the object when bit 0 of flags is set. The cSpeechInstance destructor func_002AFB38 calls it with 3.
 *
 * @param flags  g++ 2.95 destructor flags (bit 0: operator delete the object)
 */
extern "C" void cSpeechBank__cSpeechBank(cSpeechBank* self, int flags)
{
    func_003D6CA0(*(void**)self);
    if (self->mOwnsData != 0) {
        MEM_free(self->mpData);
    }
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF960);
#ifdef SKIP_ASM
extern "C" int BXFILE_exists(const char* name);
extern "C" void* FILE_load(const char* name, int a1);
extern "C" int func_003D69F0(void* data);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strrchr(const char* s, int c);
void* cSpeechBank_cSpeechBank(void* self);
extern char str_pct_s_hdr[];
extern char str_SpeechBank[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" int sprintf(char* buf, const char* fmt, ...);

// PORT: callers declare func_002AF960 as int(int, int, int); the body is (name, unused, data) returning an object pointer.
void* func_002AF960_impl(const char* name, int unused, void* data) __asm__("func_002AF960");

/**
 * Creates a cSpeechBank for "<name>.hdr": loads the header unless given, registers it with the speech driver.
 *
 * Without data it returns 0 when the file is missing (BXFILE_exists), else FILE_load()s it and sets mOwnsData.
 * Allocates the bank (cMemMan_alloc(0x4C, "SpeechBank") + cSpeechBank_cSpeechBank), stores mpData, the driver bank id unk_00 =
 * func_003D69F0(data), copies name into mName and points mShortName after its last '\'. Called by func_002B0348.
 *
 * @param name  the bank path without extension
 * @param shortName  not used (callers pass the short name)
 * @param data  the header data already in memory, or 0 to load it
 * @return the new cSpeechBank, or 0 when the file is missing
 */
void* func_002AF960_impl(const char* name, int shortName, void* data)
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
    *(int*)obj = func_003D69F0(data);
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
INCLUDE_ASM("sound/bankmonitor", func_002AFA58);
#ifdef SKIP_ASM
/**
 * Returns whether a cSpeechBank's driver bank id (unk_00) equals bankId.
 *
 * @param bankId  the bank id to compare
 * @return 1 on a match, else 0
 */
extern "C" int func_002AFA58(cSpeechBank* self, int bankId)
{
    return bankId == *(int*)self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AFA68);
#ifdef SKIP_ASM
extern "C" void cStreamSys_STREAM_Create(void* sys, int id, int a, int b, int c);
extern "C" void func_002B0088(cSpeechInstance* self, int a, int b, int c);

struct sBmPair_FA68 {
    int a;
    int b;
};

struct sBmStream_FA68 {
    int f0;                     // 0x0
    int f4;                     // 0x4
    int f8;                     // 0x8
    char fC;                    // 0xC
    char pad_0xD[0xCC - 0xD];
    sBmPair_FA68 pairs[4];      // 0xCC
    int fEC;                    // 0xEC
    int fF0;                    // 0xF0
    char pad_0xF4[0x8C4 - 0xF4];
    int f8C4;                   // 0x8C4
    int f8C8;                   // 0x8C8
    int f8CC;                   // 0x8CC
    int f8D0;                   // 0x8D0
    char pad_0x8D4[0x8F4 - 0x8D4];
    int f8F4;                   // 0x8F4
    char pad_0x8F8[0x900 - 0x8F8];
    int f900;                   // 0x900
};

/**
 * Constructs a cSpeechInstance: one speech stream of cSpeechSys, with its event files and speech banks.
 *
 * Stores the index (unk_000), stream id (unk_004) and a6 (unk_008), sets unk_900 = 1, zeroes the counters and request
 * fields and the four event-file pairs, opens the stream through cStreamSys (cStreamSys_STREAM_Create(stream id, memType, 0,
 * 0x20000)) and reads its speech.inf section (func_002B0088(index, section, loadLangHead)). func_002B0E60 calls it on
 * cMemMan_alloc(0x904, "SpeechInstance").
 *
 * @param mem  the memory for the object
 * @param instanceIndex  the instance's index in cSpeechSys::mSpeechInstance
 * @param streamId  the cStreamSys stream id
 * @param section  the speech.inf section to read
 * @param memType  the memory type passed to the stream open
 * @param loadLangHead  nonzero to also load the language headers ("%slanghead.big")
 * @param a6  stored in unk_008 (no reader found)
 * @return mem
 */
extern "C" void* func_002AFA68(cSpeechInstance* mem, int instanceIndex, int streamId, int section, int memType, int loadLangHead, int a6)
{
    sBmStream_FA68* self = (sBmStream_FA68*)mem;
    self->f0 = instanceIndex;
    self->f4 = streamId;
    self->f900 = 1;
    self->f8 = a6;
    self->fEC = 0;
    self->fF0 = 0;
    self->f8C4 = 0;
    self->f8C8 = 0;
    self->f8CC = 0;
    self->f8D0 = 0;
    self->f8F4 = 0;
    self->fC = 0;
    for (int i = 0; i < 4; i++) {
        self->pairs[i].a = 0;
        self->pairs[i].b = 0;
    }
    cStreamSys_STREAM_Create(D_004A37B4->mpVStreamSys, self->f4, memType, 0, 0x20000);
    func_002B0088((cSpeechInstance*)self, instanceIndex, section, loadLangHead);
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AFB38);
#ifdef SKIP_ASM
extern "C" void cStreamSys_STREAM_Destroy(void* self, int i);
extern "C" void cSpeechBank__cSpeechBank(cSpeechBank* self, int flags);
extern int cSpeechInstance_mNumTotalBanks;

struct sBmVoiceFB38 {
    void* handle;           // 0x0
    int active;             // 0x4
};

struct sBmPairFB38 {
    void* p[2];
};

struct sBankMonFB38 {
    int field_0x0;
    int id;                         // 0x4
    char pad_0x8[0xC4];
    sBmVoiceFB38 voices[(0xEC - 0xCC) / 8];   // 0xCC
    int voiceCount;                 // 0xEC
    int streamCount;                // 0xF0
    int* streams[(0x8D4 - 0xF4) / 4];         // 0xF4
    sBmPairFB38 pairs[1];           // 0x8D4
};

/**
 * Destroys a cSpeechInstance: its speech banks, event files and header data, then closes its stream.
 *
 * Destroys each speech bank (cSpeechBank__cSpeechBank with 3) and subtracts their count (unk_0F0) from cSpeechInstance_mNumTotalBanks; for each event
 * file frees both header blocks (unk_8D4) and the separately loaded event data; closes the stream (cStreamSys_STREAM_Destroy on
 * cStreamSys). Deletes the object when bit 0 of flags is set. func_002B0DA0 calls it with 3.
 *
 * @param selfp  the cSpeechInstance
 * @param flags  g++ 2.95 destructor flags (bit 0: operator delete the object)
 */
extern "C" void func_002AFB38(cSpeechInstance* selfp, int flags)
{
    sBankMonFB38* self = (sBankMonFB38*)selfp;
    int i;
    for (i = 0; i < self->streamCount; i++) {
        if (self->streams[i] != 0) {
            cSpeechBank__cSpeechBank((cSpeechBank*)self->streams[i], 3);
        }
    }
    cSpeechInstance_mNumTotalBanks -= self->streamCount;
    for (i = 0; i < self->voiceCount; i++) {
        for (int j = 0; j < 2; j++) {
            if (self->pairs[i].p[j] != 0) {
                MEM_free(self->pairs[i].p[j]);
            }
        }
        if (self->voices[i].active != 0) {
            MEM_free(self->voices[i].handle);
            self->voices[i].active = 0;
            self->voices[i].handle = 0;
        }
    }
    cStreamSys_STREAM_Destroy(D_004A37B4->mpVStreamSys, self->id);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AFC88);
#ifdef SKIP_ASM
extern "C" int func_002B0290(cSpeechInstance* p);
extern "C" void* func_003D7EC8(void* p);
extern "C" void* func_003D8040(void* p);

class func_002AFC88_cObj {
public:
    char data[0x1BC];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
};

/**
 * Per-frame update of one speech instance: lets the speech driver start or advance speech on channel i.
 *
 * When the stream can take speech (func_002B0290): with unk_900 set it calls cSpeechSys's virtual slot 4 (on
 * D_004A37B4) and then the driver's func_003D7EC8(i), clearing unk_900 when that returns nonzero; with unk_900 clear it
 * calls func_003D8040(i) and sets unk_900. When the stream is busy it sets unk_900. Called by func_002B0C78 for both
 * instances.
 *
 * @param i  the instance's index (the speech driver channel)
 */
// PORT: i is passed through as a pointer-sized handle
extern "C" void func_002AFC88(cSpeechInstance* self, int i)
{
    if (func_002B0290(self) != 0) {
        if (self->unk_900 != 0) {
            ((func_002AFC88_cObj*)D_004A37B4)->v04();
            if (func_003D7EC8((void*)i) != 0) {
                self->unk_900 = 0;
            }
        } else {
            func_003D8040((void*)i);
            self->unk_900 = 1;
        }
    } else {
        self->unk_900 = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AFD10);
#ifdef SKIP_ASM
extern "C" void* func_003D7EC8(void* p);

// PORT: real arity is (self, p); the unit later declares func_002AFD10(int)
void* func_002AFD10_impl(void* self, void* p) __asm__("func_002AFD10");

/**
 * Starts the next speech on a channel (func_003D7EC8) and clears the instance's unk_900 when that returns nonzero.
 *
 * func_002B1758 calls it on cSpeechSys's instance for the channel.
 *
 * @param channel  the speech driver channel
 * @return func_003D7EC8's result
 */
void* func_002AFD10_impl(void* self, void* channel)
{
    void* result = func_003D7EC8(channel);
    if (result != 0) {
        *(int*)((char*)self + 0x900) = 0;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AFD40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sBmObj {
    char pad[0x40];
    int f40;                    // 0x40
};

struct sBmVoicePair {
    void* handle;               // 0x0
    int active;                 // 0x4
};

struct sBmHandlePair {
    void* p[2];
};

// A bank list (func_002AFA68 ctor / func_002AFB38 dtor): its voices and up to 500 loaded streams.
struct sBankList {
    int f0;                     // 0x0
    int id;                     // 0x4
    int f8;                     // 0x8
    char fC;                    // 0xC
    char pad_0xD[0xCC - 0xD];
    sBmVoicePair voices[4];     // 0xCC
    int voiceCount;             // 0xEC
    int count;                  // 0xF0
    sBmObj* streams[(0x8C4 - 0xF4) / 4];  // 0xF4
    int f8C4;                   // 0x8C4
    int f8C8;                   // 0x8C8
    int f8CC;                   // 0x8CC
    int f8D0;                   // 0x8D0
    sBmHandlePair pairs[4];     // 0x8D4
    float f8F4;                 // 0x8F4
    int f8F8;                   // 0x8F8
    int f8FC;                   // 0x8FC
    int f900;                   // 0x900
};
extern char str_pct_spct_s_c[];
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void* FILE_loadz(const char* name, int flags);
extern "C" void* BIG_locate(void* big, char* name);
extern "C" int BIG_count(void* big);
extern "C" void func_003D66C0(void* h, int a);
char* BIG_locateentryz_2AFD40(void* big, char* name, int index, int* offset, int* size) __asm__("BIG_locateentryz");
extern char str_pct_seventdat_events_evt[];
extern "C" void func_002B0348_l(sBankList* self, int a, int b, int c) __asm__("func_002B0348");

/**
 * Loads one speech event file set for a cSpeechInstance: a headers .big with its banks, or a plain events file.
 *
 * Loads name (FILE_loadz) into the next event-file pair (unk_8D4[unk_0EC][0] for headers, [1] for language headers).
 * For the headers .big it locates "<mBasePath>eventdat\events.evt" inside it (an optional "prefix|" in the name sets
 * the prefix), registers it with the speech driver (func_003D66C0(.., instIndex)) and returns 0 when it is missing.
 * Then every other entry of the .big becomes a speech bank (func_002B0348 with the entry's path minus its 4-character
 * extension, its short name and its data).
 *
 * When name cannot be loaded it falls back to the standalone events file (FILE_loadz of
 * "<mBasePath>eventdat\events.evt", marked owned) and registers that. Note: it overwrites the caller's name buffer with
 * the events path. Called by func_002B0088.
 *
 * @param selfp  the cSpeechInstance
 * @param isHeaders  nonzero for "%sheaders.big", 0 for "%slanghead.big"
 * @param name  the file name (overwritten)
 * @param instIndex  the instance index passed to the speech driver
 * @param section  the speech.inf section (not used)
 * @return 1 when loaded, 0 when the events data is missing
 */
extern "C" int func_002AFD40(cSpeechInstance* selfp, int isHeaders, char* name, int instIndex, int section)
{
    sBankList* self = (sBankList*)selfp;
    char buf[0x60];
    char path[0x60];
    int off;
    int sel = 0;
    if (!isHeaders)
        sel++;
    self->pairs[self->voiceCount].p[sel] = FILE_loadz(name, 0);
    if (self->pairs[self->voiceCount].p[sel] != 0) {
        buf[0] = 0;
        if (isHeaders != 0) {
            sprintf(name, str_pct_seventdat_events_evt, (char*)self + 0x6C);
            char* entryName = name;
            int len = strlen(name);
            int i;
            for (i = 0; i < len; i++) {
                if (name[i] == 0x7C) {
                    int n = i + 1;
                    entryName = name + n;
                    strncpy(buf, name, n);
                    buf[n] = 0;
                    break;
                }
            }
            self->voices[self->voiceCount].handle = BIG_locate(self->pairs[self->voiceCount].p[sel], entryName);
            self->voices[self->voiceCount].active = 0;
            if (self->voices[self->voiceCount].handle == 0) {
                MEM_free(self->pairs[self->voiceCount].p[sel]);
                self->pairs[self->voiceCount].p[sel] = 0;
                return 0;
            }
            func_003D66C0(self->voices[self->voiceCount].handle, instIndex);
        } else {
            buf[1] = 0;
            buf[0] = 0x7C;
        }
        int n = BIG_count(self->pairs[self->voiceCount].p[sel]);
        for (int i = 0; i < n; i++) {
            char* nm = BIG_locateentryz_2AFD40(self->pairs[self->voiceCount].p[sel], 0, i, &off, 0);
            char* entryData = (char*)self->pairs[self->voiceCount].p[sel] + off;
            if (entryData != self->voices[self->voiceCount].handle) {
                sprintf(path, str_pct_spct_s_c, buf, nm);
                int len = strlen(path);
                path[len - 4] = 0;
                int j = len - 4;
                while (path[j] != 92 && j >= 0)
                    j--;
                func_002B0348_l((sBankList*)self, (int)path, (int)(path + j + 1), (int)entryData);
            }
        }
    } else {
        sprintf(name, str_pct_seventdat_events_evt, (char*)self + 0x6C);
        self->voices[self->voiceCount].handle = FILE_loadz(name, 0);
        if (self->voices[self->voiceCount].handle == 0)
            return 0;
        self->voices[self->voiceCount].active = 1;
        func_003D66C0(self->voices[self->voiceCount].handle, instIndex);
    }
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B0088);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void FileParser_FileParser(void* parser, void* src, int n);
extern "C" void FileParser__FileParser(void* parser, int flags);
extern "C" void FileParser_SkipSectionNames(void* parser);
extern "C" int FileParser_FindPartialSection(void* parser, int file);
extern "C" char* FileParser_GetName(void* parser);
extern "C" int FileParser_IsEOF(void* parser);
extern "C" int FileParser_AssignString(void* parser, const char* key, char* out);
void cStreamSys_STREAM_SetBigFile_2B0088(void* self, int i, char* val) __asm__("cStreamSys_STREAM_SetBigFile");
extern "C" int func_002AFD40(cSpeechInstance* self, int a, char* name, int b, int c);
extern "C" int strcasecmp(const char* a, const char* b);
extern void* D_004A37B0;
extern char str_BIGFILE[];
extern char str_BASEPATH[];
extern char str_BANK[];
extern char str_pct_sheaders_big[];
extern char str_pct_slanghead_big[];
extern char str_pct_spct_s_c[];
struct sBankList_0348;
extern "C" void func_002B0348(sBankList_0348* self, int a, int b, int c);

/**
 * Reads a cSpeechInstance's speech.inf section (D_004A37B0): its .big file, base paths and extra banks.
 *
 * Parses the section whose name starts with a2 (FileParser_FindPartialSection). BIGFILE sets the stream's .big file
 * (cStreamSys_STREAM_SetBigFile on cStreamSys); each BASEPATH stores mBasePath, loads "<base>headers.big" (func_002AFD40; stops at the
 * first failure), optionally "<base>langhead.big", and counts an event file (unk_0EC); BANK adds a speech bank
 * (func_002B0348). Likely bug: the BANK branch reads the value into one buffer but builds the path from, and passes, an
 * uninitialised one (name). Called by the constructor func_002AFA68 and by func_002B0F10.
 *
 * @param speechInstance  the cSpeechInstance
 * @param instanceIndex  the instance's index
 * @param section  the section to read
 * @param loadLangHead  nonzero to also load the language headers
 */
extern "C" void func_002B0088(cSpeechInstance* speechInstance, int instanceIndex, int section, int loadLangHead)
{
    char parser[2064];
    char value[0x100];
    char bigFileName[0x40];
    char path[0x80];
    char name[0x60];
    FileParser_FileParser(parser, D_004A37B0, 0x100);
    if (FileParser_FindPartialSection(parser, section) != 0)
    {
        FileParser_SkipSectionNames(parser);
        while (FileParser_IsEOF(parser) == 0)
        {
            char* key = FileParser_GetName(parser);
            if (strcasecmp(str_BIGFILE, key) == 0)
            {
                FileParser_AssignString(parser, str_BIGFILE, bigFileName);
                cStreamSys_STREAM_SetBigFile_2B0088(D_004A37B4->mpVStreamSys, speechInstance->unk_004, bigFileName);
            }
            if (strcasecmp(str_BASEPATH, key) == 0)
            {
                FileParser_AssignString(parser, str_BASEPATH, speechInstance->mBasePath);
                sprintf(value, str_pct_sheaders_big, speechInstance->mBasePath);
                if (func_002AFD40(speechInstance, 1, value, instanceIndex, section) == 0)
                    break;
                if (loadLangHead)
                {
                    sprintf(value, str_pct_slanghead_big, speechInstance->mBasePath);
                    func_002AFD40(speechInstance, 0, value, instanceIndex, section);
                }
                speechInstance->unk_0EC += 1;
            }
            if (strcasecmp(str_BANK, key) == 0)
            {
                FileParser_AssignString(parser, str_BANK, value);
                sprintf(path, str_pct_spct_s_c, speechInstance->mBasePath, name);
                func_002B0348((sBankList_0348*)speechInstance, (int)path, (int)name, 0);
            }
        }
    }
    FileParser__FileParser(parser, 2);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B0290);
#ifdef SKIP_ASM
extern "C" int cStreamSys_STREAM_Requests(void* self, int i);
extern "C" float cStreamSys_STREAM_TimeRemaining(void* self, int i);
extern "C" int cStreamSys_STREAM_Playing(void* self, int i);

/**
 * Returns whether a speech instance's stream can take a new speech: idle, or playing with under 500 left.
 *
 * Asks cStreamSys about the instance's stream (unk_004): state 0 (cStreamSys_STREAM_Requests) gives 1; state 1 gives 1 when
 * cStreamSys_STREAM_Playing is nonzero and cStreamSys_STREAM_TimeRemaining is below 500; anything else 0.
 *
 * @param speechInstance  the cSpeechInstance
 * @return 1 or 0
 */
extern "C" int func_002B0290(cSpeechInstance* speechInstance)
{
    int state = cStreamSys_STREAM_Requests(D_004A37B4->mpVStreamSys, speechInstance->unk_004);
    if (state == 0) {
        return 1;
    }
    if (state != 1) {
        return 0;
    }
    if (cStreamSys_STREAM_Playing(D_004A37B4->mpVStreamSys, speechInstance->unk_004) == 0) {
        return 0;
    }
    int ok = (int)cStreamSys_STREAM_TimeRemaining(D_004A37B4->mpVStreamSys, speechInstance->unk_004) < 500;
    int canPlay = 0;
    if (ok) {
        canPlay = state;
    }
    return canPlay;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B0320);
#ifdef SKIP_ASM
extern "C" void cStreamSys_STREAM_Purge(void* self, int i);

/**
 * Purges a speech instance's stream (cStreamSys_STREAM_Purge on cStreamSys with unk_004).
 *
 * func_002B11B0 calls it.
 *
 * @param speechInstance  the cSpeechInstance
 */
extern "C" void func_002B0320(cSpeechInstance* speechInstance)
{
    cStreamSys_STREAM_Purge(D_004A37B4->mpVStreamSys, speechInstance->unk_004);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B0348);
#ifdef SKIP_ASM
extern "C" int func_002AF960(int a, int b, int c);
extern int cSpeechInstance_mNumTotalBanks;

struct sBankList_0348 {
    char pad[0xF0];
    int count;     // 0xF0
    int ids[1];    // 0xF4
};

/**
 * Creates a speech bank (func_002AF960) and appends it to the instance's bank list when it loaded.
 *
 * On success increments the bank count (unk_0F0) and the global bank count cSpeechInstance_mNumTotalBanks. Called by func_002AFD40 and
 * func_002B0088.
 *
 * @param bankPath  the bank path without extension
 * @param shortName  the short name
 * @param headerData  the header data, or 0 to load it
 */
extern "C" void func_002B0348(sBankList_0348* self, int bankPath, int shortName, int headerData)
{
    self->ids[self->count] = func_002AF960(bankPath, shortName, headerData);
    if (self->ids[self->count] != 0) {
        self->count++;
        cSpeechInstance_mNumTotalBanks++;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B03B8);
#ifdef SKIP_ASM
extern "C" unsigned int strlen(const char* s);

/**
 * Returns the language of a speech bank name from its suffix: _eng 1, _frn 2, _grm 3, _spn 4, otherwise 0.
 *
 * func_002B04D8 uses it to pick the .big file from cSpeechSys::mBigFileName.
 *
 * @param name  the bank name
 * @return the language index (0 = the language-independent speech.big)
 */
extern "C" int func_002B03B8(const char* name)
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
INCLUDE_ASM("sound/bankmonitor", func_002B04D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// A bank list (func_002AFA68 ctor / func_002AFB38 dtor): its voices and up to 500 loaded streams.
extern char str_pct_spct_s_c[];
extern char str_data_b[];
extern char str_pct_spct_spct_s_dat[];
extern "C" int func_002AFA58(cSpeechBank* self, int a1);
extern "C" int func_002B1520_c(void* sys, int a, int b) __asm__("func_002B1520");
extern "C" int func_002B03B8(const char* name);
extern "C" void cStreamSys_STREAM_SetVolControl(void* self, int i, int w);

typedef int (*Fn1_04D8)(void*, int, char*, int, int, int, int, float, int, int);

struct sBmArgs_04D8 {
    int f0;
    int f4;
    char pad8[4];
    unsigned short fC;
};

static inline sBmObj** at_04D8(sBmObj** base, int i)
{
    return base + i;
}

extern "C" int cMonitor_MONITOR_Create_c(void* bank, int a1, sSndCallback id, int a3, int a4, int a5, int a6, int a7) __asm__("cMonitor_MONITOR_Create");
extern "C" int func_002B04D8_impl(sBankList* self, sBmArgs_04D8* a) __asm__("func_002B04D8");

/**
 * Plays a speech event on a cSpeechInstance: finds its bank, builds the stream file name and starts it.
 *
 * Finds the bank whose driver id is the event's bank (first by index, then by search); without one, or when no pending
 * request matches (func_002B1520(instance index, event key)), returns -1. Builds "data/<mBigFileName[language]><bank
 * name>.dat" and gets a volume id from cSpeechSys's slot 2 (GetSpeechVolumeIdentifier).
 *
 * When the stream is busy, it starts the speech with cSpeechSys's slot 1 (hold -1) and arms a monitor callback:
 * func_002B1268 (D_004A37D8) after the stream's remaining time when cStreamSys_STREAM_Playing is set, else func_002B1348
 * (D_004A37E0) every 200 ms, with the instance, the handle and the volume id. When it is idle it applies slot 3
 * (UpdateSpeechVolume) to the stream (cStreamSys_STREAM_SetVolControl) and starts the speech with hold unk_8C8. Called by func_002B0F48.
 *
 * @param speechEvent  the speech driver event (bank id, play argument, request key)
 * @return the speech handle from the play slot, or -1
 */
extern "C" int func_002B04D8_impl(sBankList* self, sBmArgs_04D8* speechEvent)
{
    int idx = -1;
    int i;
    if (func_002AFA58((cSpeechBank*)self->streams[speechEvent->f0], speechEvent->f0)) {
        idx = speechEvent->f0;
    } else {
        for (i = 0; i < self->count; i++) {
            if (func_002AFA58((cSpeechBank*)self->streams[i], speechEvent->f0)) {
                idx = i;
            }
        }
    }
    if (idx >= 0 && func_002B1520_c((char*)D_004A37B4, self->f0, speechEvent->fC)) {
        sBmObj** slot = at_04D8(self->streams, idx);
        int lang = func_002B03B8((char*)*slot + 4);
        char buf[0x100];
        sprintf(buf, str_pct_spct_spct_s_dat, str_data_b, (char*)&D_004A37B4->mBigFileName[0] + lang * 20, (char*)*slot + 4);
        cSpeechSys* speechSys = D_004A37B4;
        sSndVtSlot* vt0 = (sSndVtSlot*)speechSys->mVtbl;
        int volumeId = ((int (*)(void*, int, int))vt0[2].fn)(vthis(speechSys, vt0[2]), self->f0, (*slot)->f40);
        char* speechBase; sSndVtSlot* vt;
        int remainingMs = (int)cStreamSys_STREAM_TimeRemaining(D_004A37B4->mpVStreamSys, self->id);
        int playHandle;
        if (cStreamSys_STREAM_Requests(D_004A37B4->mpVStreamSys, self->id) != 0) {
            if (cStreamSys_STREAM_Playing(D_004A37B4->mpVStreamSys, self->id) != 0) {
                speechBase = (char*)D_004A37B4;
                vt = *(sSndVtSlot**)(speechBase + 0x1BC);
                playHandle = ((Fn1_04D8)vt[1].fn)(vthis(speechBase, vt[1]), self->id, buf, speechEvent->f4, *(int*)((char*)self + 0x8CC), -1, *(int*)((char*)self + 0x8D0), *(float*)((char*)self + 0x8F4), *(int*)((char*)self + 0x8F8), *(int*)((char*)self + 0x8FC));
                if (playHandle >= 0) {
                    cMonitor_MONITOR_Create_c(&(*((cBankManager**)D_004A37B4->mpVStreamSys))->sub, remainingMs, D_004A37D8[0], (int)self, playHandle, volumeId, 0, 0);
                }
                return playHandle;
            }
            speechBase = (char*)D_004A37B4;
            vt = *(sSndVtSlot**)(speechBase + 0x1BC);
            playHandle = ((Fn1_04D8)vt[1].fn)(vthis(speechBase, vt[1]), self->id, buf, speechEvent->f4, *(int*)((char*)self + 0x8CC), -1, *(int*)((char*)self + 0x8D0), *(float*)((char*)self + 0x8F4), *(int*)((char*)self + 0x8F8), *(int*)((char*)self + 0x8FC));
            if (playHandle >= 0) {
                cMonitor_MONITOR_Create_c(&(*((cBankManager**)D_004A37B4->mpVStreamSys))->sub, 200, D_004A37E0[0], (int)self, playHandle, volumeId, 0, 0);
            }
            return playHandle;
        }
        speechBase = (char*)D_004A37B4;
        vt = *(sSndVtSlot**)(speechBase + 0x1BC);
        int w = ((int (*)(void*, int, int, int))vt[3].fn)(vthis(speechBase, vt[3]), self->f0, volumeId, *(int*)((char*)self + 0x8D0));
        if (w != 0) {
            cStreamSys_STREAM_SetVolControl(D_004A37B4->mpVStreamSys, self->id, w);
        }
        cSpeechSys* speechSystem = D_004A37B4;
        sSndVtSlot* vt2 = (sSndVtSlot*)speechSystem->mVtbl;
        return ((Fn1_04D8)vt2[1].fn)(vthis(speechSystem, vt2[1]), self->id, buf, speechEvent->f4, *(int*)((char*)self + 0x8CC), *(int*)((char*)self + 0x8C8), *(int*)((char*)self + 0x8D0), *(float*)((char*)self + 0x8F4), *(int*)((char*)self + 0x8F8), *(int*)((char*)self + 0x8FC));
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B07C0);
#ifdef SKIP_ASM
/**
 * Calls cStreamSys cStreamSys_STREAM_Playing on the speech instance's stream (unk_004); the result is not returned in C.
 *
 * func_002B11E8 uses the value left in v0.
 */
extern "C" void func_002B07C0(cSpeechInstance* self)
{
    cStreamSys_STREAM_Playing(D_004A37B4->mpVStreamSys, self->unk_004);
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
    (speechInstance->unk_8C4) = value;
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002B07F0);
#ifdef SKIP_ASM
/**
 * Sets a speech instance's unk_8C8 (the hold func_002B04D8 passes when it starts speech on an idle stream).
 *
 * Called by func_002B1020.
 *
 * @param speechInstance  the cSpeechInstance
 * @param holdValue  the value
 */
extern "C" void func_002B07F0(cSpeechInstance* speechInstance, int holdValue) {
    (speechInstance->unk_8C8) = holdValue;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSpeechSys_cSpeechSys);
#ifdef SKIP_ASM
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" void* cStreamSys_cStreamSys(void* self, int inchrg, int banks, int count, int heap);
struct sBankSlots;
extern "C" void func_002B1428(cSpeechSys* self);
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
 * When @p inchrg is set it builds the virtual bases first: cSoundSys at +0x1C0, cBankManager at +0x1C8 and cStreamSys
 * at +0xC98. Installs the cSpeechSys vtables (cSpeechSys_vtbl at mVtbl, D_004838A0 / D_004838C0 in the virtual bases,
 * adjusted on the stack when not in charge), sets mCallbackInstance -1, clears unk_008 and mCallback, stores itself in
 * D_004A37B4, allocates the two-entry mSpeechInstance array (both empty), clears the request table (func_002B1428) and
 * copies the five speech .big file names into mBigFileName.
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
        char* vb = ((char*)self) + 0x1C0;
        char* vb2 = ((char*)self) + 0xC98;
        *(char**)(((char*)self) + 0xC98) = ((char*)self) + 0x1C8;
        *(char**)self = vb2;
        *(char**)(((char*)self) + 0x3A0) = vb;
        cSoundSys_cSoundSys((cSoundSys*)vb, heap);
        cBankManager_cBankManager(*(void**)(((char*)self) + 0xC98), 0, maxBanks, heap);
        cStreamSys_cStreamSys(*(void**)self, 0, maxBanks, maxStreams, heap);
    }
    *(const sBmonVtblA07F8**)(*(char**)(**(char***)self + 0x1D8) + 4) = &D_004838A0_07F8;
    *(void***)(**(char***)self + 0xAB0) = cBankMonitor_vtbl;
    *(const sBmonVtblB07F8**)(**(char***)self + 0x1D4) = &D_004838C0_07F8;
    if (inchrg == 0) {
        sBmonVtblA07F8 soundSysVtbl = D_004838A0_07F8;
        *(sBmonVtblA07F8**)(*(char**)(**(char***)self + 0x1D8) + 4) = &soundSysVtbl;
        char* base1 = *(char**)(**(char***)self + 0x1D8) - 0x1C0;
        int soundSysAdjust = ((char*)self) - base1;
        soundSysVtbl.e[1].delta = D_004838A0_07F8.e[1].delta + soundSysAdjust;
        sBmonVtblB07F8 bankMgrVtbl = D_004838C0_07F8;
        *(sBmonVtblB07F8**)(**(char***)self + 0x1D4) = &bankMgrVtbl;
        char* base2 = **(char***)self - 0x1C8;
        int bankMgrAdjust = ((char*)self) - base2;
        bankMgrVtbl.e[1].delta = D_004838C0_07F8.e[1].delta + bankMgrAdjust;
    }
    self->mVtbl = cSpeechSys_vtbl;
    self->mCallbackInstance = -1;
    self->unk_008 = 0;
    self->mCallback = 0;
    D_004A37B4 = (cSpeechSys*)(self);
    self->mSpeechInstance = new (str_mSpeechInstance, 0, 0) void*[2];
    for (int i = 0; i < 2; i++)
        (self->mSpeechInstance)[i] = 0;
    func_002B1428(self);
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
 * func_002B0DA0), clears D_004A37B4, destroys the virtual bases (cStreamSys via cStreamSys__cStreamSys, cBankManager, cSoundSys)
 * when bit 1 of @p flags is set and frees the object when bit 0 is set.
 *
 * @param flags  destructor flags: bit 1 destroys the virtual bases, bit 0 frees the object
 */
// PORT: g++ 2.95 virtual-base destruction with stack vtable this-adjust fix-ups, written out by hand.
extern "C" void cSpeechSys__cSpeechSys(cSpeechSys* self, int flags)
{
    self->mVtbl = cSpeechSys_vtbl;
    *(const sBmonVtblA0AE8**)(*(char**)(**(char***)self + 0x1D8) + 4) = &D_004838A0;
    *(void***)(**(char***)self + 0xAB0) = cBankMonitor_vtbl;
    *(const sBmonVtblB0AE8**)(**(char***)self + 0x1D4) = &D_004838C0;
    if (flags == 0) {
        sBmonVtblA0AE8 soundSysVtbl = D_004838A0;
        *(sBmonVtblA0AE8**)(*(char**)(**(char***)self + 0x1D8) + 4) = &soundSysVtbl;
        char* base1 = *(char**)(**(char***)self + 0x1D8) - 0x1C0;
        int soundSysAdjust = ((char*)self) - base1;
        soundSysVtbl.e[1].delta = D_004838A0.e[1].delta + soundSysAdjust;
        sBmonVtblB0AE8 bankMgrVtbl = D_004838C0;
        *(sBmonVtblB0AE8**)(**(char***)self + 0x1D4) = &bankMgrVtbl;
        char* base2 = **(char***)self - 0x1C8;
        int bankMgrAdjust = ((char*)self) - base2;
        bankMgrVtbl.e[1].delta = D_004838C0.e[1].delta + bankMgrAdjust;
    }
    if (self->mSpeechInstance != 0) {
        cMemMan_free(self->mSpeechInstance);
    }
    D_004A37B4 = 0;
    if (flags & 2) {
        cStreamSys__cStreamSys(*(void**)self, 0);
        cBankManager__cBankManager(**(void***)self, 0);
        cSoundSys__cSoundSys((cSoundSys*)(*(void**)(**(char***)self + 0x1D8)), 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B0C78);
#ifdef SKIP_ASM
struct sBankSlots;
extern "C" void func_002AFC88(cSpeechInstance*, int);
extern "C" void func_002B1720(cSpeechSys*);

/**
 * Per-frame speech update: runs func_002AFC88 on both speech instances, then ages the pending requests.
 *
 * The request timers count down in func_002B1720. Called by the audio system's update on gSSXAudio + 0x5560.
 */
extern "C" void func_002B0C78(cSpeechSys* self)
{
    for (int i = 0; i < 2; i++) {
        void* instance = (self->mSpeechInstance)[i];
        if (instance != 0) {
            func_002AFC88((cSpeechInstance*)instance, i);
        }
    }
    func_002B1720(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", SPCH_Init);
#ifdef SKIP_ASM
extern "C" int func_003D6778(int a, int b, int c);
// PORT: the unit defines cSoundSys_SndRand() with no args; this caller passes one.
int cSoundSys_SndRand_a(int seed) __asm__("cSoundSys_SndRand");
extern "C" void bxLogPrint(const char* fmt, ...);
extern "C" void func_003D6840(void* fn, int seed, int buf);
extern "C" int func_003D6C58(int n);
extern "C" void* MEM_alloc(void* a0, int a1, int a2);
extern "C" void func_003D6C60(int n, void* p);
extern "C" void* func_003D6768();
extern "C" void func_002AF8A8();
extern "C" int TIMER_gettick();
extern char D_00483390[];
extern char str_bank_mem[];

/**
 * Sets up the speech system's memory and callbacks.
 *
 * Allocates a 0x5622-byte buffer (func_003D6778) and prints "SPCH_Init: rNum=%d". It passes func_002AF8A8, a
 * value read through the bank manager and the buffer to func_003D6840. It then sizes a "bank mem" block for
 * 500 (func_003D6C58), allocates it into +0x8 and hands it to func_003D6C60. The callees are unknown.
 */
extern "C" void SPCH_Init(cSpeechSys* self)
{
    int buf = func_003D6778(22050, 0x10, 1);
    int seed = cSoundSys_SndRand_a(*(int*)(**(char***)self + 0x1D8));
    bxLogPrint(D_00483390, seed);
    func_003D6840((void*)func_002AF8A8, seed, buf);
    int n = func_003D6C58(500);
    void* workMem = MEM_alloc(str_bank_mem, n, 0);
    self->unk_008 = workMem;
    func_003D6C60(500, workMem);
    *(void**)((char*)func_003D6768() + 0x8) = (void*)TIMER_gettick;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B0DA0);
#ifdef SKIP_ASM
extern "C" void func_002AFB38(cSpeechInstance* p, int mode);
extern "C" void func_003D67F8(void);

/**
 * Shuts the speech system down: destroys both speech instances and frees the speech bank buffer.
 *
 * Destroys each instance (func_002AFB38 with 3) and clears its slot, calls the driver's func_003D67F8 and MEM_free()s
 * unk_008, clearing it. Called by the audio system's shutdown.
 */
extern "C" void func_002B0DA0(cSpeechSys* self)
{
    for (int i = 0; i < 2; i++) {
        void* instance = (self->mSpeechInstance)[i];
        if (instance != 0) {
            func_002AFB38((cSpeechInstance*)instance, 3);
        }
        (self->mSpeechInstance)[i] = 0;
    }
    func_003D67F8();
    MEM_free(self->unk_008);
    self->unk_008 = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B0E28);
#ifdef SKIP_ASM
extern "C" int func_002B0290(cSpeechInstance* p);

/**
 * Returns whether speech instance i can take a new speech (func_002B0290), or 0 when it does not exist.
 *
 * The script engine checks it for instance 0 before running the playing NIS list's scripts.
 *
 * @param i  the instance index (0 or 1)
 * @return 1 or 0
 */
extern "C" int func_002B0E28(cSpeechSys* self, int i)
{
    void* instance = (self->mSpeechInstance)[i];
    if (instance != 0) {
        return func_002B0290((cSpeechInstance*)instance);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B0E60);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002AFA68(cSpeechInstance* mem, int i, int a, int b, int c, int d, int e);
extern const char str_SpeechInstance[];

/**
 * Creates speech instance i: cMemMan_alloc(0x904, "SpeechInstance") constructed by func_002AFA68.
 *
 * Stores it in mSpeechInstance[i]. The audio system creates instance 0 for section "CHAR".
 *
 * @param i  the instance index
 * @param streamId  the cStreamSys stream id
 * @param section  the speech.inf section
 * @param memType  the memory type for the stream
 * @param loadLangHead  nonzero to load the language headers
 * @param e  stored in the instance (unk_008)
 */
extern "C" void func_002B0E60(cSpeechSys* self, int i, int streamId, int section, int memType, int loadLangHead, int e)
{
    // PORT: pointer arithmetic through int
    void** slot = (void**)(i * 4 + (int)self->mSpeechInstance);
    *slot = func_002AFA68((cSpeechInstance*)cMemMan_alloc(2308, str_SpeechInstance, 0, 0), i, streamId, section, memType, loadLangHead, e);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B0F10);
#ifdef SKIP_ASM
/**
 * Re-reads speech.inf section section for speech instance i (func_002B0088 without language headers).
 *
 * @param i  the instance index
 * @param section  the section to read
 */
extern "C" void func_002B0F10(cSpeechSys* self, int i, int section)
{
    void* instance = (self->mSpeechInstance)[i];
    if (instance != 0) {
        func_002B0088((cSpeechInstance*)instance, i, section, 0);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B0F48);
#ifdef SKIP_ASM
extern "C" int func_002B04D8(void* p, int a1);

/**
 * Plays a speech event on the first speech instance that accepts it and calls mCallback for that instance.
 *
 * Tries instance 0 then 1 with func_002B04D8; on success, when mCallbackInstance is that instance, calls mCallback with
 * the handle. Reached from the speech driver through func_002AF8A8.
 *
 * @param playArgs  the speech driver event
 * @return the speech handle, or -1
 */
extern "C" int func_002B0F48(cSpeechSys* self, int playArgs)
{
    for (int i = 0; i < 2; i++) {
        void* instance = (self->mSpeechInstance)[i];
        if (instance != 0) {
            int handle = func_002B04D8(instance, playArgs);
            if (handle >= 0) {
                if (self->mCallbackInstance == i) {
                    (self->mCallback)(handle);
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
INCLUDE_ASM("sound/bankmonitor", func_002B1020);
#ifdef SKIP_ASM
extern "C" void func_002B07F0(cSpeechInstance* p, int a);

/**
 * Sets speech instance i's unk_8C8 (func_002B07F0), the hold used on an idle stream, when it exists.
 *
 * @param i  the instance index
 * @param holdValue  the value
 */
extern "C" void func_002B1020(cSpeechSys* self, int i, int holdValue)
{
    void* instance = (self->mSpeechInstance)[i];
    if (instance != 0) {
        func_002B07F0((cSpeechInstance*)instance, holdValue);
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1058);
#ifdef SKIP_ASM
/**
 * Triggers the speech driver on channel idx with one-shot settings, then clears them.
 *
 * Sets the instance's unk_8CC = a2, unk_8F4 = v (only when v > 0) and unk_8D0 = a4, arms mCallback for idx when
 * callback is nonzero, and calls the driver's func_003D8040(idx), which can start speech (func_002AF8A8 ->
 * func_002B0F48). Afterwards mCallback / mCallbackInstance and the three fields are cleared.
 *
 * @param idx  the instance index (driver channel)
 * @param playArg  unk_8CC for this trigger (passed to the play slot)
 * @param callback  function to call with the started handle, or 0
 * @param volumeArg  unk_8D0 for this trigger
 * @param floatArg  unk_8F4 for this trigger, when > 0
 */
extern "C" void func_002B1058(cSpeechSys* self, int idx, int playArg, int callback, int volumeArg, float floatArg)
{
    int** chans = (int**)self->mSpeechInstance;
    if (chans[idx] != 0) {
        *(int*)((char*)chans[idx] + 0x8CC) = playArg;
        if (floatArg > 0.0f) {
            *(float*)((char*)((int**)self->mSpeechInstance)[idx] + 0x8F4) = floatArg;
        }
        *(int*)((char*)((int**)self->mSpeechInstance)[idx] + 0x8D0) = volumeArg;
        if (callback != 0) {
            self->mCallback = (void (*)(int))callback;
            self->mCallbackInstance = idx;
        }
        func_003D8040((void*)idx);
        self->mCallbackInstance = -1;
        self->mCallback = 0;
        *(float*)((char*)((int**)self->mSpeechInstance)[idx] + 0x8F4) = 0.0f;
        *(int*)((char*)((int**)self->mSpeechInstance)[idx] + 0x8CC) = 0;
        *(int*)((char*)((int**)self->mSpeechInstance)[idx] + 0x8D0) = 0;
    }
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B1128);
#ifdef SKIP_ASM
extern "C" void func_002B1058(cSpeechSys* self, int idx, int a2, int a3, int a4, float v);
extern int D_004A37BC;

/**
 * Flushes speech channel idx: triggers the driver four times (func_002B1058 with zeros) while D_004A37BC is set.
 *
 * With D_004A37BC set the start callback func_002AF8A8 refuses to play, so queued events are consumed without sound.
 *
 * @param idx  the instance index
 */
extern "C" void func_002B1128(cSpeechSys* self, int idx)
{
    if (((int*)self->mSpeechInstance)[idx] != 0) {
        int i = 3;
        D_004A37BC = 1;
        do {
            func_002B1058(self, idx, 0, 0, 0, 0.0f);
            i--;
        } while (i >= 0);
        D_004A37BC = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B11B0);
#ifdef SKIP_ASM
extern "C" void func_002B0320(cSpeechInstance* p);

/**
 * Purges speech instance i's stream (func_002B0320) when the instance exists.
 *
 * The audio system calls it with 0 before music changes and when it purges.
 *
 * @param i  the instance index
 */
extern "C" void func_002B11B0(cSpeechSys* self, int i)
{
    void* instance = (self->mSpeechInstance)[i];
    if (instance != 0) {
        func_002B0320((cSpeechInstance*)instance);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B11E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_002B07C0_11E8(void* self) __asm__("func_002B07C0");

/**
 * Returns cStreamSys cStreamSys_STREAM_Playing for speech instance i's stream (through func_002B07C0), or 0 without an instance.
 *
 * @param i  the instance index
 * @return the stream query's result, or 0
 */
extern "C" int func_002B11E8(cSpeechSys* self, int i)
{
    void* instance = (self->mSpeechInstance)[i];
    if (instance != 0) {
        return func_002B07C0_11E8(instance);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1220);
#ifdef SKIP_ASM
/**
 * Returns speech instance i's stream id (unk_004), or -1 when the instance does not exist.
 *
 * @param i  the instance index
 * @return the stream id, or -1
 */
extern "C" int func_002B1220(cSpeechSys* self, int i)
{
    cSpeechInstance* instance = (cSpeechInstance*)(self->mSpeechInstance)[i];
    if (((void*)instance) != 0) {
        return instance->unk_004;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1248);
#ifdef SKIP_ASM
extern "C" void cStreamSys_STREAM_ReleaseHoldCallback(int);

/**
 * Ends a speech monitor callback: calls cStreamSys_STREAM_ReleaseHoldCallback on the speech system's cStreamSys (the instance stays in a1).
 *
 * Called as (cSpeechSys*, cMonitorInstance*) by func_002B1268; returns cStreamSys_STREAM_ReleaseHoldCallback's value in v0.
 *
 * @param speechSys  the cSpeechSys (its first word is the cStreamSys pointer)
 */
extern "C" void func_002B1248(int *speechSys) {
    cStreamSys_STREAM_ReleaseHoldCallback(*speechSys);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B1268);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPacked8_1268 { long v; } __attribute__((packed));
extern const sPacked8_1268 D_004A37E0_2B1268[] __asm__("D_004A37E0");
extern "C" int cStreamSys_STREAM_GetCurrActiveSampleHandle(void* self, int i);
extern "C" void cStreamSys_STREAM_SetVolControl(void* self, int i, int val);
extern "C" void cMonitor_MONITOR_Create(int a, int b, long t, void* c, int d, int e, int f, int g);
extern "C" int func_002B1248_pp(void* self, void* msg) __asm__("func_002B1248");
struct sVE1268 { short delta; short index; int (*fn)(void*, int, int, int); };

/**
 * Monitor callback after a speech started on a busy stream: applies its volume once the speech is current.
 *
 * msg is the cMonitorInstance (mPtr = the speech instance, mData1 = the speech handle, mData2 = the volume id). When
 * the instance's stream is not playing that handle yet (cStreamSys_STREAM_GetCurrActiveSampleHandle) it re-arms func_002B1348 (D_004A37E0) in 200 ms
 * and returns 1. Otherwise, for a volume id other than -1, it calls cSpeechSys's slot 3 (UpdateSpeechVolume) and stores
 * a nonzero result in the stream (cStreamSys_STREAM_SetVolControl), then ends through func_002B1248. Armed by func_002B04D8 (D_004A37D8).
 *
 * @param msg  the cMonitorInstance
 * @return 1 when re-armed, else func_002B1248's result
 */
extern "C" int func_002B1268(cSpeechSys* self, cMonitorInstance* msg)
{
    char* ent = (char*)msg->mPtr;
    if (cStreamSys_STREAM_GetCurrActiveSampleHandle(*(void**)self, func_002B1220((cSpeechSys*)D_004A37B4, *(int*)ent)) != msg->mData1) {
        cMonitor_MONITOR_Create(*((int*)D_004A37B4->mpVStreamSys) + 472, 200, D_004A37E0_2B1268[0].v, msg->mPtr, msg->mData1, msg->mData2, 0, 0);
        return 1;
    }
    int slot = msg->mData2;
    if (slot != -1) {
        sVE1268* vt = (sVE1268*)self->mVtbl;
        int speechVolume = vt[3].fn(vthis(self, vt[3]), *(int*)ent, slot, *(int*)(ent + 0x8D0));
        if (speechVolume != 0) {
            cStreamSys_STREAM_SetVolControl(*(void**)self, *(int*)(ent + 4), speechVolume);
        }
    }
    return func_002B1248_pp(self, msg);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B1348);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPacked8_1348 { long v; } __attribute__((packed));
extern const sPacked8_1348 D_004A37D8_2B1348[] __asm__("D_004A37D8");
extern "C" int cStreamSys_STREAM_GetCurrActiveSampleHandle(void* self, int i);
extern "C" void cMonitor_MONITOR_Create(int a, int b, long t, void* c, int d, int e, int f, int g);
extern "C" int func_002B1268(cSpeechSys* self, cMonitorInstance* msg);

/**
 * Monitor callback polling a busy speech stream until the queued speech can be handled by func_002B1268.
 *
 * msg is the cMonitorInstance (mPtr = the speech instance, mData1 / mData2 = handle and volume id). While the stream
 * state (cStreamSys_STREAM_Requests) is above 1: if cStreamSys_STREAM_Playing is set it arms func_002B1268 (D_004A37D8) after the remaining time
 * (cStreamSys_STREAM_TimeRemaining) and returns 1; if the stream is not playing the handle yet it returns 0 (poll again); otherwise it
 * hands over to func_002B1268. Armed by func_002B04D8 and func_002B1268 (D_004A37E0).
 *
 * @param msg  the cMonitorInstance
 * @return 1 when re-armed, 0 to keep polling, else func_002B1268's result
 */
extern "C" int func_002B1348(cSpeechSys* self, cMonitorInstance* msg)
{
    cSpeechInstance* ent = (cSpeechInstance*)((char*)msg->mPtr);
    if (cStreamSys_STREAM_Requests(*(void**)self, ent->unk_004) - 1 > 0) {
        if (cStreamSys_STREAM_Playing(*(void**)self, ent->unk_004) != 0) {
            float remainingMs = cStreamSys_STREAM_TimeRemaining(*(void**)self, ent->unk_004);
            cMonitor_MONITOR_Create(*((int*)D_004A37B4->mpVStreamSys) + 472, (int)remainingMs, D_004A37D8_2B1348[0].v, ent, msg->mData1, msg->mData2, 0, 0);
            return 1;
        }
        if (cStreamSys_STREAM_GetCurrActiveSampleHandle(*(void**)self, func_002B1220((cSpeechSys*)D_004A37B4, *(int*)ent)) != msg->mData1) {
            return 0;
        }
        return func_002B1268(self, msg);
    }
    return func_002B1268(self, msg);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1428);
#ifdef SKIP_ASM


/**
 * Clears all ten pending speech requests (mRequest[i].used = 0).
 *
 * cSpeechSys_cSpeechSys calls it.
 */
extern "C" void func_002B1428(cSpeechSys* self)
{
    for (int i = 9; i >= 0; i--) {
        ((&self->mRequest[0]))[i].used = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1458);
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
 * @return 1 when added or refreshed, 0 otherwise
 */
extern "C" int func_002B1458(sBankEntries* self, int instIndex, int requestId, int object, int identity, int callback, int refresh, float requestValue)
{
    int freeIdx = -1;
    int i;
    for (i = 0; i < 10; i++) {
        if (self->entries[i].used != 0) {
            if (self->entries[i].a == instIndex && self->entries[i].b == requestId) {
                if (refresh == 0) {
                    return 0;
                }
                self->entries[i].timer = 180;
                return 1;
            }
        } else {
            if (freeIdx == -1) {
                freeIdx = i;
            }
        }
    }
    if (freeIdx == -1) {
        return 0;
    }
    self->entries[freeIdx].used = 1;
    self->entries[freeIdx].timer = 180;
    self->entries[freeIdx].a = instIndex;
    self->entries[freeIdx].b = requestId;
    self->entries[freeIdx].c = object;
    self->entries[freeIdx].d = identity;
    self->entries[freeIdx].f = requestValue;
    self->entries[freeIdx].e = callback;
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1520);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
class cBMVoice_1520 {
public:
    // vptr at 0; slot N at vtable offset N*8
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual int v08();
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
    char** slots;           // 0x4
    char pad8[0xC - 0x8];
    int curK;               // 0xC
    int curId;              // 0x10
    char pad14[0x18 - 0x14];
    sBMEnt_1520 ents[10];   // 0x18
};

/**
 * Applies the pending speech request matching (id, key) to its speech instance; returns whether one matched.
 *
 * Reads two values from the request object's sub-object at +0x6C0 (its virtual slots 5 and 8) and its +0x870 (-1
 * without an object) into the instance's unk_8CC, unk_8F8 and unk_8FC, and copies the request's float and word into
 * unk_8F4 and unk_8D0. A request callback arms mCallback for that instance; otherwise mCallback is cleared. Called by
 * func_002B04D8.
 *
 * @param id  the speech instance index
 * @param key  the request id
 * @return 1 when a request matched, else 0
 */
extern "C" int func_002B1520(sBMon_1520* self, int id, int key)
{
    for (int j = 0; j < 10; j++) {
        if (self->ents[j].used != 0 && self->ents[j].id == id && self->ents[j].key == key) {
            int slotFiveValue = 0;
            int slotEightValue = 0;
            int z = -1;
            if (self->ents[j].obj != 0) {
                slotFiveValue = ((cBMVoice_1520*)self->ents[j].obj->voice)->v05();
                slotEightValue = ((cBMVoice_1520*)self->ents[j].obj->voice)->v08();
                z = self->ents[j].obj->z;
            }
            *(int*)(self->slots[id] + 0x8CC) = slotFiveValue;
            *(int*)(self->slots[id] + 0x8F8) = slotEightValue;
            *(int*)(self->slots[id] + 0x8FC) = z;
            *(float*)(self->slots[id] + 0x8F4) = self->ents[j].t;
            *(int*)(self->slots[id] + 0x8D0) = self->ents[j].w;
            int k = self->ents[j].k;
            if (k != 0) {
                self->curK = k;
                self->curId = id;
            } else {
                self->curK = 0;
                self->curId = -1;
            }
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B16A0);
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
 * For each used matching entry: mCallback = 0, mCallbackInstance = -1, the instance's unk_8F4, unk_8CC and unk_8D0 = 0,
 * and the entry is freed.
 *
 * @param idx  the speech instance index
 * @param key  the request id
 */
extern "C" void func_002B16A0(cSpeechSys* self, int idx, int key)
{
    sSlot16A0* slots = (sSlot16A0*)&self->mRequest;
    for (int i = 0; i < 10; i++) {
        if (slots[i].active != 0 && slots[i].idx == idx && slots[i].key == key) {
            self->mCallback = 0;
            self->mCallbackInstance = -1;
            *(int*)((char*)((int*)self->mSpeechInstance)[idx] + 0x8F4) = 0;
            *(int*)((char*)((int*)self->mSpeechInstance)[idx] + 0x8CC) = 0;
            *(int*)((char*)((int*)self->mSpeechInstance)[idx] + 0x8D0) = 0;
            slots[i].active = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1720);
#ifdef SKIP_ASM
/**
 * Ages the pending speech requests: decrements each used timer and frees the request when it reaches 0.
 *
 * Called every frame by func_002B0C78.
 */
extern "C" void func_002B1720(cSpeechSys* self)
{
    for (int i = 0; i < 10; i++) {
        if (self->mRequest[i].used != 0) {
            if (--self->mRequest[i].timer <= 0) {
                self->mRequest[i].used = 0;
            }
        }
    }
}
#endif

extern "C" void* func_002AFD10(int);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1758__FPvi);
#ifdef SKIP_ASM
/**
 * Starts the next speech on a channel through its speech instance (func_002AFD10 on mSpeechInstance[channel]).
 *
 * The audio system calls it with 0 after a music change.
 *
 * @param channel  the instance index / driver channel
 * @return func_002AFD10's result
 */
void* func_002B1758(void* vself, int channel)
{
    cSpeechSys* self = (cSpeechSys*)vself;
    // a1 also stays in $5 as func_002AFD10's second argument
    return func_002AFD10_impl((void*)((int*)self->mSpeechInstance)[channel], (void*)channel);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1780);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cStreamSys_STREAM_CreateInstance1(void* self, int idx, int heap, char* mem, int size, int n, int m);
extern "C" int cStreamSys_STREAM_GetHandle(void* self, int idx);

struct sEntry_1780 {
    signed char kind;
    char pad[0x17];
};
struct sQuad_1780 {
    int a;
    int b;
    int c;
    int d;
};
// Same layout as sStack_A910 (streamsys) / sStack_C430 (bankmanager): a cSndPlayOpts-style state stack.
struct sStack_1780 {
    int field_0x0;                  // 0x1D8
    sEntry_1780 cur;                // 0x1DC
    volatile int depth;             // 0x1F4
    sEntry_1780* entries;           // 0x1F8
    int flags[4];                   // 0x1FC
    int* a34;
    int* a38;
    int* a3C;
    char a40[4][8];
    int* a60;
    int* a64;
    int* a68;
    int* a6C;
    sQuad_1780* a70;
    int* a74;
    int* a78;
    int* a7C;
    int* a80;
    int* a84;
};
struct sMgr_1780 {
    char pad_0x0[0x1D8];
    sStack_1780 st;                 // 0x1D8
};

// The stream system: *gMusicSys is the system object, whose first word points at the manager.

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
extern "C" int func_002B1780(sEntry_1780* entry, int n, int maxChunks, char* mem, int size)
{
    sMgr_1780* mg = *((sMgr_1780**)gMusicSys->mpVStreamSys);
    sStack_1780* st = &mg->st;
    mg->st.depth++;
    st->entries[mg->st.depth] = mg->st.cur;
    mg->st.flags[mg->st.depth] = 0;
    st->a34[mg->st.depth] = 0;
    st->a38[mg->st.depth] = st->entries[mg->st.depth].kind;
    st->a3C[mg->st.depth] = 0;
    memset((char*)st + mg->st.depth * 8 + 0x40, 0, 8);
    st->a60[mg->st.depth] = 0;
    st->a64[mg->st.depth] = 0;
    st->a68[mg->st.depth] = 0;
    st->a6C[mg->st.depth] = 0;
    st->a70[mg->st.depth].b = 0;
    st->a70[mg->st.depth].a = 100;
    st->a70[mg->st.depth].c = 90;
    st->a70[mg->st.depth].d = 50;
    st->a74[mg->st.depth] = 0;
    st->a80[mg->st.depth] = 0;
    st->a84[mg->st.depth] = 0;
    st->a78[mg->st.depth] = 127;
    st->a7C[mg->st.depth] = 1;
    {
        sMgr_1780* bankManager = *((sMgr_1780**)gMusicSys->mpVStreamSys);
        int off = bankManager->st.depth * (int)sizeof(sEntry_1780);
        *(sEntry_1780*)((char*)bankManager->st.entries + off) = *entry;
    }
    cStreamSys_STREAM_CreateInstance1(((sMgr_1780**)gMusicSys->mpVStreamSys), 2, 0, mem, size, n, maxChunks);
    (*((sMgr_1780**)gMusicSys->mpVStreamSys))->st.depth--;
    return cStreamSys_STREAM_GetHandle(((sMgr_1780**)gMusicSys->mpVStreamSys), 2);
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1A70);
#ifdef SKIP_ASM

/**
 * Purges cStreamSys stream 2 (cStreamSys_STREAM_Purge on gMusicSys's stream system).
 */
extern "C" void func_002B1A70(void) {
    cStreamSys_STREAM_Purge(((void**)gMusicSys->mpVStreamSys), 2);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongConfiguration_cSongConfiguration);
#ifdef SKIP_ASM
extern char D_00483980[];
extern unsigned char D_004A35A8[];

struct cSongConfiguration_cSongConfiguration_sElem20 {
    unsigned char b;
    char pad[0x1F];
};

/**
 * Constructs a song configuration with the music.inf defaults.
 *
 * Stores the vtable D_00483980 and sets the defaults a music.inf section can override: no bank data, 4 beats per
 * measure, 2 measures per bar, 4 phrases per bank, 8 beats per phrase, phrase align 16, delay count 0 / feedback 90 /
 * time 100 / level 50, path and async levels 100, 120 BPM, duck to loops on, mSedValue and mPreview -1, mLowPass
 * 0xFFFF, mSongBig 1, and empties the path strings (first byte of D_004A35A8).
 *
 * @return @p self
 */
extern "C" void* cSongConfiguration_cSongConfiguration(cSongConfiguration* self)
{
    self->mVtbl = (void**)D_00483980;
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
    self->mPathData[0] = D_004A35A8[0];
    self->mMusData[0] = D_004A35A8[0];
    cSongConfiguration_cSongConfiguration_sElem20* bankData = (cSongConfiguration_cSongConfiguration_sElem20*)&self->mBankData;
    for (int i = 0; i >= 0; i--, bankData++) {
        bankData->b = D_004A35A8[0];
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cSongConfiguration__cSongConfiguration);
#ifdef SKIP_ASM
extern char D_00483980[];

/**
 * Destroys a cSongConfiguration: restores its vptr (D_00483980) and frees it when bit 0 of flags is set.
 *
 * Virtual slot 1 of _vt$18cSongConfiguration; cMusicSys's destructor calls it with 3 for each song.
 *
 * @param flags  g++ 2.95 destructor flags (bit 0: operator delete the object)
 */
extern "C" void cSongConfiguration__cSongConfiguration(cSongConfiguration* self, int flags)
{
    self->mVtbl = (void**)D_00483980;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1B98);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002B1C50(void* mem, int a0, int a1, int a2, int a3, char a4, int a5, int a6, int a7);
extern const char str_SongInstance[];

/**
 * Allocates and constructs a cSongInstance: cMemMan_alloc(0xC4, "SongInstance") + func_002B1C50.
 *
 * Called by func_002B3838 with cMusicSys's mBasePath first: the first parameter is the base path, not the song name (a1
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
extern "C" void* func_002B1B98(int songName, int sectionName, int config, int playOpts, char baseVol, int volControl, int bankIndex, int syncLoad)
{
    return func_002B1C50(cMemMan_alloc(196, str_SongInstance, 0, 0), songName, sectionName, config, playOpts, baseVol, volControl, bankIndex, syncLoad);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1C50);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* bmNew_1C50(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int func_003D3058(int a, int b);
extern "C" void func_002B4A20(cMusicSys* self, void* name, int sync);
extern "C" void func_002B21E0(cSongInstance*, int);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_PathfinderStream[];
extern char str_PathfinderOverlay[];
extern char str_audio_music_big[];
extern char str_audio_music2_big[];
extern char str_pct_spct_spct_s_b[];
extern char str_data_b[];
extern char str_pct_spct_s_c[];

struct sBankEntry_1C50 {
    unsigned char vol;      // 0x0
    char pad1[0x13];
    unsigned short f14;     // 0x14
    short pad16;
};

struct sBankDef_1C50 {
    char pad00[0x1C];
    volatile int cur;           // 0x1C
    sBankEntry_1C50* entries;   // 0x20
    char pad24[0x14];
    int* vols;                  // 0x38
};

struct sBankMon_1C50 {
    char* snd;                  // 0x0
    sBankEntry_1C50 entry;      // 0x4
    int f1C;                    // 0x1C
    int f20;
    void* buf24;                // 0x24
    void* buf28;                // 0x28
    int f2C;                    // 0x2C
    int f30;                    // 0x30
    int f34;                    // 0x34
    int f38;                    // 0x38
    int f3C;                    // 0x3C
    char f40;                   // 0x40
    int f44;                    // 0x44
    int f48;                    // 0x48
    int f4C;                    // 0x4C
    int f50;                    // 0x50
    int f54;                    // 0x54
    int f58;                    // 0x58
    int f5C;                    // 0x5C
    char name[0x40];            // 0x60
};

#define BM_MIN_1C50(a, b) ((a) < (b) ? (a) : (b))
#define BM_MAX_1C50(a, b) ((a) >= (b) ? (a) : (b))

static inline sBankEntry_1C50* bankCur_1C50(sBankDef_1C50* d)
{
    return &d->entries[d->cur];
}

// PORT: callers declare the first parameter as void*; the body takes the typed object (bound by asm label)
extern "C" void* func_002B1C50_body(sBankMon_1C50* self, int a0, int a1, int a2, int a3, char a4, int a5, int a6, int a7) __asm__("func_002B1C50");

/**
 * Constructs a cSongInstance (0xC4): the playing song's streams, buffers and settings.
 *
 * Initialises the fields (unk_1C/unk_30/unk_58 -1, unk_54 1, others 0), stores the bank index (unk_34), volume control
 * (unk_3C), base volume (unk_40) and the current play options' low-pass cutoff (unk_44), and allocates the 0x37000-byte
 * "PathfinderStream" and the func_003D3058(1, 0)-byte "PathfinderOverlay" buffers. Scales mBaseVol[mInst] by the song's
 * mPathlevel / 100 (clamped 0..127) into the play level's vol and copies that level into mPlayOpts.
 *
 * mPath becomes "data/" + "audio/music.big" (mSongBig 1) or "audio/music2.big" + a0. Loads the MPF file
 * "<mPath><mPathData>" (func_002B4A20) and, with a7 set, opens the streams at once (func_002B21E0 with force). Note a1
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
extern "C" void* func_002B1C50_body(sBankMon_1C50* self, int basePath, int sectionName, int songConfig, int playOpts, char baseVol, int volControl, int bankIndex, int syncLoad)
{
    char* snd = (char*)songConfig;
    sBankDef_1C50* def = (sBankDef_1C50*)playOpts;
    self->f1C = -1;
    self->buf24 = 0;
    self->buf28 = 0;
    self->f2C = 0;
    self->f30 = -1;
    self->f38 = 0;
    self->f34 = bankIndex;
    self->f3C = volControl;
    self->f40 = baseVol;
    self->f48 = 0;
    self->f4C = 0;
    self->f50 = 0;
    self->f54 = 1;
    self->f58 = -1;
    self->f5C = 0;
    self->f44 = bankCur_1C50(def)->f14;
    self->buf24 = bmNew_1C50(0x37000, str_PathfinderStream, 0, 0);
    self->buf28 = bmNew_1C50(func_003D3058(1, 0), str_PathfinderOverlay, 0, 0);
    float scale = (float)*(int*)(snd + 0x88) * 0.01f;
    def->vols[def->cur] = (int)BM_MAX_1C50(BM_MIN_1C50((float)def->vols[def->cur] * scale, 127.0f), 0.0f);
    def->entries[def->cur].vol = BM_MAX_1C50(BM_MIN_1C50(def->vols[def->cur], 127), 0);
    if (*(int*)(snd + 0xA4) == 1) {
        sprintf(self->name, str_pct_spct_spct_s_b, str_data_b, str_audio_music_big, basePath);
    } else {
        sprintf(self->name, str_pct_spct_spct_s_b, str_data_b, str_audio_music2_big, basePath);
    }
    self->snd = snd;
    self->entry = *bankCur_1C50(def);
    char buf[0x100];
    sprintf(buf, str_pct_spct_s_c, self->name, snd);
    func_002B4A20((cMusicSys*)((void***)gMusicSys), buf, syncLoad);
    if (syncLoad != 0) {
        func_002B21E0((cSongInstance*)self, 1);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1F78);
#ifdef SKIP_ASM
extern "C" void cAsyncSys_ASYNCSYS_Cancel(void* self, int i);
extern "C" int func_003D2A68(int id);
void cMemMan_free(void*);
void operator_delete(int* ptr);

/**
 * Destroys a cSongInstance: cancels its overlay bank load, closes its Pathfinder streams and frees its buffers.
 *
 * Cancels the async load of bank unk_34 (cAsyncSys_ASYNCSYS_Cancel), closes the main path stream (func_003D2A68 with
 * 0x11000001) and, when the song has loop banks (mNumBankData > 0), the overlay (0x11000002), and frees the stream and
 * overlay buffers (unk_24, unk_28). Called with 3 by func_002B3AC0 and the cMusicSys destructor.
 *
 * @param flags  g++ 2.95 destructor flags (bit 0: operator delete the object)
 */
extern "C" void func_002B1F78(cSongInstance* self, int flags)
{
    cAsyncSys_ASYNCSYS_Cancel(*((void**)gMusicSys->mpVStreamSys), self->unk_34);
    func_003D2A68(0x11000001);
    if (self->mpConfig->mNumBankData > 0) {
        func_003D2A68(0x11000002);
    }
    if (self->unk_24 != 0) {
        cMemMan_free(self->unk_24);
    }
    if (self->unk_28 != 0) {
        cMemMan_free(self->unk_28);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2018);
#ifdef SKIP_ASM
extern "C" int func_002B2488(cSongInstance* self);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSTRM_pitchmult(int h, int v);
extern "C" void func_003D1038(int cmd, int v);

/**
 * Pauses a song: sets its main stream's pitch multiplier to 0 and pauses the overlay (func_003D1038(.., 1)).
 *
 * The pitch change runs inside the critical section and only when the stream has a handle (func_002B2488).
 */
extern "C" void func_002B2018(cSongInstance* self)
{
    SNDSYS_entercritical();
    int handle = func_002B2488(self);
    if (handle >= 0) {
        SNDSTRM_pitchmult(handle, 0);
    }
    SNDSYS_leavecritical();
    func_003D1038(0x11000002, 1);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2070);
#ifdef SKIP_ASM
extern "C" int func_002B2488(cSongInstance* self);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSTRM_pitchmult(int h, int v);
extern "C" void func_003D1038(int cmd, int v);

/**
 * Resumes a song paused by func_002B2018: pitch multiplier 0x1000 and func_003D1038(0x11000002, 0).
 */
extern "C" void func_002B2070(cSongInstance* self)
{
    SNDSYS_entercritical();
    int handle = func_002B2488(self);
    if (handle >= 0) {
        SNDSTRM_pitchmult(handle, 0x1000);
    }
    SNDSYS_leavecritical();
    func_003D1038(0x11000002, 0);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B20C8);
#ifdef SKIP_ASM
extern "C" void func_003D16F0(int id, short v);

/**
 * Sends a music event to the song's Pathfinder players (func_003D16F0(-1, v)), or parks it until they open.
 *
 * Does nothing while unk_5C is set unless force is. Without open streams (unk_2C 0) the event is kept in unk_30 (only
 * when none is parked yet); func_002B21E0 sends it with force after opening them. Reached from cMusicSys's slot 3
 * (func_002B3B88).
 *
 * @param musicEvent  the music event (the cMusicSys state code)
 * @param force  nonzero to send even while unk_5C is set
 */
extern "C" void func_002B20C8(cSongInstance* self, int musicEvent, int force)
{
    if (self->unk_5C != 0 && force == 0) {
        return;
    }
    if (self->unk_2C != 0) {
        func_003D16F0(-1, musicEvent);
    } else if (self->unk_30 == -1) {
        self->unk_30 = musicEvent;
    }
}
#endif

extern "C" void func_003D0D70(unsigned int);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2120);
#ifdef SKIP_ASM
/**
 * Calls func_003D0D70 for the main path stream (0x11000001), passing the caller's second argument through.
 *
 * The C declaration has no parameters; func_002B3C28 reaches it with its value (the callers pass a volume such as 127)
 * still in the second argument register.
 */
extern "C" void func_002B2120()
{
    func_003D0D70(0x11000001U);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2140);
#ifdef SKIP_ASM
extern "C" void func_003D15C8(int);

/**
 * Calls func_003D15C8 for the main path stream (0x11000001), passing the caller's second argument through.
 *
 * The C declaration has no parameters; func_002B3C60 reaches it with its value still in the second argument register.
 */
extern "C" void func_002B2140(void) {
    func_003D15C8(0x11000001);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2160);
#ifdef SKIP_ASM
extern "C" int func_002B2488(cSongInstance* self);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void func_003B80A0(int h, int v);

/**
 * Sets the low-pass cutoff of the song's main stream (func_003B80A0), clamped to 0..0xFFFF, when it changed.
 *
 * The cutoff is cached in unk_44 (only when the stream has a handle) and applied inside the critical section.
 *
 * @param lowPass  the cutoff
 */
extern "C" void func_002B2160(cSongInstance* self, int lowPass)
{
    if (lowPass > 0xFFFF) {
        lowPass = 0xFFFF;
    }
    if (lowPass < 0) {
        lowPass = 0;
    }
    if (lowPass != self->unk_44) {
        SNDSYS_entercritical();
        int streamHandle = func_002B2488(self);
        if (streamHandle >= 0) {
            self->unk_44 = lowPass;
            func_003B80A0(streamHandle, lowPass);
        }
        SNDSYS_leavecritical();
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B21E0);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_002B20C8(cSongInstance* mon, int a1, int a2);
extern "C" void func_002B2160(cSongInstance* self, int v);
extern "C" int func_002B2488(cSongInstance* self);
extern "C" int func_002B4AA0(cMusicSys* self, int force);
int cMusicSys_GetMPFFile(void* self);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void func_003B7E70(int h, int v);
extern "C" void func_003D2350(int a);
extern "C" int func_003D25F0(int id, char* name, int a2, void* a3, int a4, int a5, void* buf, int size);
extern char str_pct_spct_s_c[];
extern float gMusicScaling;


/**
 * Updates a song: opens its Pathfinder streams once the MPF file is loaded, then keeps the low-pass and volume set.
 *
 * While the streams are closed (unk_2C 0) it waits for the MPF load (func_002B4AA0, unless force), sets unk_2C, passes
 * cMusicSys::mMPFFile to Pathfinder (func_003D2350) and opens the main path stream "<mPath><mMusData>"
 * (func_003D25F0(0x11000001, .., 500, &mPlayOpts, 2, 150, unk_24, 0x37000)) and, with loop banks, the overlay
 * "<mPath><mBankData[0]>" (0x11000002, 10 requests, the 0x6A400-byte unk_28 buffer); a parked event (unk_30) is then
 * sent with force (func_002B20C8).
 *
 * Once open it applies the song's mLowPass (func_002B2160) and, unless fading out (unk_48) or unk_54 is 0, sets the
 * stream volume (func_003B7E70) to base volume (unk_40) x mPathlevel / 100 x the volume control (unk_3C, when set) x
 * gMusicScaling, clamped to 0..127. Called by the constructor with force and by func_002B4388.
 *
 * @param mon  the cSongInstance
 * @param force  nonzero to open the streams without waiting for the async MPF load
 */
// PORT: g++ `<?` / `>?` (min/max) operators.
extern "C" void func_002B21E0(cSongInstance* mon, int force)
{
    if (mon->unk_2C == 0)
    {
        if (func_002B4AA0((cMusicSys*)((void***)gMusicSys), force) == 0)
            return;
        (mon)->unk_2C = 1;
        func_003D2350(cMusicSys_GetMPFFile((void***)gMusicSys));
        char buf[256];
        sprintf(buf, str_pct_spct_s_c, mon->mPath, *(char**)mon + 0x20);
        mon->mHandle = func_003D25F0(0x11000001, buf, 500, &mon->mPlayOpts, 2, 150, mon->unk_24, 0x37000);
        if (*(int*)(*(char**)mon + 0x40) > 0)
        {
            sprintf(buf, str_pct_spct_s_c, mon->mPath, *(char**)mon + 0x44);
            mon->unk_20 = func_003D25F0(0x11000002, buf, 10, &mon->mPlayOpts, 1, 0, mon->unk_28, 0x6A400);
        }
        if (mon->unk_30 != -1)
        {
            func_002B20C8(mon, mon->unk_30, 1);
            mon->unk_30 = -1;
        }
        return;
    }
    func_002B2160(mon, *(int*)(*(char**)mon + 0x9C));
    if (mon->unk_48 != 0)
        return;
    if (mon->unk_54 == 0)
        return;
    float volume;
    float* volumeControl = mon->unk_3C;
    if (volumeControl)
        volume = (float)mon->unk_40 * 0.009999999776482582f * (float)*(int*)(*(char**)mon + 0x88) * *volumeControl;
    else
        volume = (float)mon->unk_40 * 0.009999999776482582f * (float)*(int*)(*(char**)mon + 0x88);
    volume *= gMusicScaling;
    SNDSYS_entercritical();
    int streamHandle = func_002B2488(mon);
    if (streamHandle >= 0)
    {
        signed char vol = (int)((volume <? 127.0f) >? 0.0f);
        func_003B7E70(streamHandle, vol);
    }
    SNDSYS_leavecritical();
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B23E8);
#ifdef SKIP_ASM
extern "C" void func_003D5508(int cmd, int v);

/**
 * Sets the song overlay's level (func_003D5508(0x11000002, v), 0..100) when unk_54 is set.
 *
 * @param levelPercent  the level in percent
 */
extern "C" void func_002B23E8(cSongInstance* self, signed char levelPercent)
{
    if (self->unk_54 != 0) {
        func_003D5508(0x11000002, levelPercent);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2418);
#ifdef SKIP_ASM
extern "C" int func_002B2488(cSongInstance* self);
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" void func_003B7EB0(int h, int v, int c);

/**
 * Fades the song's main stream out to 0 over t seconds (func_003B7EB0) and marks it fading (unk_48 = 1).
 *
 * The fade runs inside the critical section, only when the stream has a handle; func_002B21E0 stops applying the volume
 * once unk_48 is set.
 *
 * @param fadeSeconds  the fade time in seconds
 */
extern "C" void func_002B2418(cSongInstance* self, float fadeSeconds)
{
    SNDSYS_entercritical();
    int streamHandle = func_002B2488(self);
    if (streamHandle >= 0) {
        func_003B7EB0(streamHandle, (int)(fadeSeconds * 1000.0f), 0);
    }
    SNDSYS_leavecritical();
    self->unk_48 = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B2488);
#ifdef SKIP_ASM
extern "C" int func_003D3030(int id);

/**
 * Returns the song's main stream handle: the first word of the Pathfinder slot playing 0x11000001, or -1.
 *
 * The slot comes from func_003D3030 and func_002ADFB0.
 *
 * @return the stream handle, or -1 when the main stream has no slot
 */
extern "C" int func_002B2488(cSongInstance* self)
{
    int idx = func_003D3030(0x11000001);
    if (idx < 0) {
        return -1;
    }
    // PORT: func_002ADFB0 returns a pointer held in int
    return *(int*)func_002ADFB0(idx);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B24C0);
#ifdef SKIP_ASM
// PORT: cSoundSys_SndRand is defined () but this caller passes the monitor.
unsigned int cSoundSys_SndRand_p(void* p) __asm__("cSoundSys_SndRand");

/**
 * Returns a random loop-bank index for the song: cSoundSys_SndRand() modulo its config's mNumBankData.
 *
 * func_002B43D8 uses it to pick the overlay bank.
 *
 * @param songInstance  the cSongInstance
 * @return the bank index
 */
extern "C" int func_002B24C0(cSongInstance* songInstance)
{
    return cSoundSys_SndRand_p(SoundSys) % (unsigned int)songInstance->mpConfig->mNumBankData;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2500);
#ifdef SKIP_ASM
/**
 * Returns a random phrase start for the song: (SndRand % (mPhrasesPerBank - 1)) x mPhraseAlign.
 *
 * Used as mData1 of the beat-synchronised overlay callback armed by func_002B4568.
 *
 * @return the phrase start
 */
extern "C" int func_002B2500(cSongInstance* self)
{
    cSongConfiguration* config;
    unsigned int randVal = cSoundSys_SndRand_p(SoundSys);
    config = self->mpConfig;
    return (randVal % (unsigned int)(config->mPhrasesPerBank - 1)) * config->mPhraseAlign;
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2550);
#ifdef SKIP_ASM
extern "C" void PATH_status(int, void*);

/**
 * Returns word 9 of the main path stream's status (PATH_status(0x11000001)).
 *
 * func_0028E088 treats -1 as no playing stream.
 *
 * @return the status word
 */
extern "C" int func_002B2550(void)
{
    int buf[10];
    PATH_status(0x11000001, buf);
    return buf[9];
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2578);
#ifdef SKIP_ASM
extern "C" int func_002B2488(cSongInstance* self);
extern "C" int SNDSTRM_status(int h, void* out);

// PORT: returns int; a later caller in this unit declares it void(void*, void*)
int func_002B2578_impl(void* self, void* out) __asm__("func_002B2578");

/**
 * Reads the main stream's status (SNDSTRM_status) into out, or returns -1 when the song has no stream handle.
 *
 * @param out  receives the stream status
 * @return SNDSTRM_status's result, or -1
 */
int func_002B2578_impl(void* self, void* out)
{
    return func_002B2488((cSongInstance*)self) < 0 ? -1 : SNDSTRM_status(func_002B2488((cSongInstance*)self), out);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B25C8);
#ifdef SKIP_ASM
extern "C" void func_002B2578(void* self, void* out);
extern "C" void SNDSTRM_requeststatus(int h, int v);

struct sBmInfo {
    int a;
    int handle;
    int c;
    int d;
};

/**
 * Reads the status of the song stream's current request (SNDSTRM_requeststatus) into the given buffer.
 *
 * Gets the stream status first (func_002B2578) and uses its second word as the request handle. The parameter is
 * declared int but is the out buffer.
 *
 * @param requestStatus  receives the request status (type, position, length, ...)
 */
extern "C" void func_002B25C8(cSongInstance* self, int requestStatus)
{
    sBmInfo info;
    func_002B2578(self, &info);
    SNDSTRM_requeststatus(info.handle, requestStatus);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B2600);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit declares func_002B25C8(void*, int); its second arg is really an out pointer.
void func_002B25C8_out(void* self, void* out) __asm__("func_002B25C8");


/**
 * Returns the song's play position in ms: the current request's position when its status type is 2, else 0.
 *
 * Reads the request status through func_002B25C8.
 *
 * @return the position
 */
extern "C" float func_002B2600(cSongInstance* self)
{
    SNDREQUESTSTATUS info;
    func_002B25C8_out(self, &info);
    if (info.state == 2) {
        return (float)info.currenttime;
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2640);
#ifdef SKIP_ASM

extern "C" float func_002B26B8(cSongInstance* self);

// PORT: the unit later declares func_002B2640() as void() for a caller; real signature is float(void*).
float func_002B2640_impl(void* self) __asm__("func_002B2640");
/**
 * Returns the current request's length when its status type is 2, otherwise one bar's length (func_002B26B8).
 *
 * @return the length in ms
 */
float func_002B2640_impl(void* self)
{
    SNDREQUESTSTATUS info;
    func_002B25C8_out(self, &info);
    if (info.state == 2) {
        return (float)info.timetoend;
    }
    return func_002B26B8((cSongInstance*)self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2690);
#ifdef SKIP_ASM
/**
 * Returns the song's beat length in ms: 60 / mBPM x 1000.
 *
 * @return the beat length
 */
extern "C" float func_002B2690(cSongInstance* self)
{
    cSongConfiguration* config = self->mpConfig;
    return (60.0f / config->mBPM) * 1000.0f;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B26B8);
#ifdef SKIP_ASM
/**
 * Returns the song's bar length in ms: beat length (func_002B2690) x mBeatsPerMeasure x mMeasuresPerBar.
 *
 * @return the bar length
 */
extern "C" float func_002B26B8(cSongInstance* self)
{
    float beatMs = func_002B2690(self);
    cSongConfiguration* config = self->mpConfig;
    return beatMs * (float)config->mBeatsPerMeasure * (float)config->mMeasuresPerBar;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B26F8);
#ifdef SKIP_ASM
extern "C" void func_002B2640();

/**
 * Calls func_002B2640 with the caller's arguments still in the registers and discards the result.
 */
extern "C" void func_002B26F8(void) {
    func_002B2640();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B2718);
#ifdef SKIP_ASM
extern "C" float func_002B2600(cSongInstance* self);
extern "C" float fmodf(float x, float y);

/**
 * Returns the time to the song's next beat: beat length minus the position modulo the beat length (fmodf).
 *
 * @return the time in ms
 */
extern "C" float func_002B2718(cSongInstance* self)
{
    float beatLength = func_002B2690(self);
    float playPosition = func_002B2600(self);
    return beatLength - fmodf(playPosition, func_002B2690(self));
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2778);
#ifdef SKIP_ASM
/**
 * Returns the song's current beat number: play position (func_002B2600) / beat length (func_002B2690).
 *
 * @return the beat index
 */
extern "C" int func_002B2778(cSongInstance* self)
{
    return (int)(func_002B2600(self) / func_002B2690(self));
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002B27C0);
#ifdef SKIP_ASM
extern "C" int cBankManager_BANK_Handle(void*, int);
extern "C" int SNDbankinuse(int);

/**
 * Returns whether the song's loop bank is ready: unk_4C set, or bank unk_34 loaded (state 1) and in use.
 *
 * With unk_4C clear it checks inside the critical section that the cBankManager's bank record is in state 1 and that
 * SNDbankinuse(cBankManager_BANK_Handle(..)) is 1.
 *
 * @param other  the cBankManager
 * @return 1 when ready, else 0
 */
extern "C" int func_002B27C0(cSongInstance* self, void* other)
{
    if (self->unk_4C == 0) {
        int slot = self->unk_34;
        int ok = 0;
        SNDSYS_entercritical();
        bool notReady = *(int*)(slot * 0x60 + *(int*)((char*)other + 0xACC)) != 1;
        if (!notReady) {
            ok = SNDbankinuse(cBankManager_BANK_Handle(other, slot)) == 1;
        }
        SNDSYS_leavecritical();
        if (ok == 0) {
            return 0;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2850);
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
 * = mDelayCount, third = mDelayFeedback, first = mDelayTime; mDelayLevel is not used. Called as func_002B2850(mCurrent,
 * cMusicSys) from seg_1BA100.
 *
 * @param src  the cSongInstance (its first word is the cSongConfiguration)
 * @param dst  the cMusicSys (its first word leads to the cBankManager)
 */
extern "C" void func_002B2850(sBMSrc** src, sBMTrack*** dst)
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
struct sBankEntry64_28C0 {
    char data[0x64];
};

// The speech-bank monitor (func_002B3398 dtor): up to 64 named banks, the selection mask and per-kind counts.
// PORT: 64-bit `long` mask (8 bytes on EE, 4 on Windows); use uint64_t off-PS2.
struct sSpeechMon {
    cBankManager** pp;          // 0x000
    int arr[64];                // 0x004
    char obj[0x20];             // 0x104
    char names[64][11];         // 0x124
    int count;                  // 0x3E4 (number of names)
    int selected;               // 0x3E8
    int f3EC;                   // 0x3EC
    int current;                // 0x3F0
    int pad3F4;
    ulong mask;                 // 0x3F8
    char pad400[0x40C - 0x400];
    int res;                    // 0x40C
    char pad410[0x420 - 0x410];
    int grid[64][5];            // 0x420
    int totals[5];              // 0x920
    sBankEntry64_28C0 a[64];         // 0x934
    sBankEntry64_28C0 b[64];         // 0x2234
    sBankEntry64_28C0 c[64];         // 0x3B34
};
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
extern "C" void* FileParser_FileParser_p(void* parser, void* src, int n) __asm__("FileParser_FileParser");
extern "C" int FileParser_FindSection(void* parser, const char* name);
extern "C" char* FileParser_FindNextSection(void* parser);
extern "C" int FileParser_AssignInt(void* parser, const char* key, int* out);
extern "C" int FileParser_AssignFloat(void* parser, const char* key, float* out);

// One song's configuration (cSongConfiguration_cSongConfiguration ctor, 0xAC bytes).
struct sSongCfg28C0 {
    char name[0x20];            // 0x00
    char file[0x20];            // 0x20
    int numBankData;            // 0x40
    char bankData[1][0x20];     // 0x44
};
struct sBank28C0 {
    int p0;
    char* ents[0x40];
};

struct sLoc28C0 {
    char parser[0x810];
    char buf[0x20];
    int iA;
    int iB;
    int iC;
    float fD;
};

// One row of the per-song grid (sSpeechMon::grid).
static inline int* Row_28C0(int (*g)[5], int i)
{
    return g[i];
}

/**
 * Constructs cMusicSys: sets up its sound bases, then reads music.inf into the song list and song configurations.
 *
 * When in charge it constructs the virtual bases (cSoundSys at +0x5448, cBankManager, cStreamSys) with the bank and
 * stream counts; it installs the cSoundSys and cBankManager vtables (D_00483928, cBankMonitor_vtbl, D_00483948), and
 * when not in charge points the objects at stack copies with adjusted this deltas. Clears the counts, sets the vptr
 * cMusicSys_vtbl and gMusicSys = self, allocates the 25600-byte "MPF data file" buffer (mMPFFile), clears mCategory /
 * mCategoryCount and empties every title, artist and album.
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
extern "C" char* cMusicSys_cMusicSys(char* self, int inchrg, int maxBanks, int maxStreams, int heap)
{
    if (inchrg) {
        char* vb = self + 0x5448;
        char* vb2 = self + 0x5F20;
        *(char**)(self + 0x5F20) = self + 0x5450;
        *(char**)self = vb2;
        *(char**)(self + 0x5628) = vb;
        cSoundSys_cSoundSys((cSoundSys*)vb, heap);
        cBankManager_cBankManager(*(void**)(self + 0x5F20), 0, maxBanks, heap);
        cStreamSys_cStreamSys(*(void**)self, 0, maxBanks, maxStreams, heap);
    }
    *(const sSndVtbl4**)(*(char**)(**(char***)self + 0x1D8) + 4) = &D_00483928_v;
    *(void***)(**(char***)self + 0xAB0) = cBankMonitor_vtbl;
    *(const sSndVtbl7**)(**(char***)self + 0x1D4) = &D_00483948_v;
    if (inchrg == 0) {
        sSndVtbl4 soundSysVtbl = D_00483928_v;
        *(sSndVtbl4**)(*(char**)(**(char***)self + 0x1D8) + 4) = &soundSysVtbl;
        char* base1 = *(char**)(**(char***)self + 0x1D8) - 0x5448;
        int soundSysDelta = self - base1;
        soundSysVtbl.e[1].delta = D_00483928_v.e[1].delta + soundSysDelta;
        sSndVtbl7 bankManagerVtbl = D_00483948_v;
        *(sSndVtbl7**)(**(char***)self + 0x1D4) = &bankManagerVtbl;
        char* base2 = **(char***)self - 0x5450;
        int bankManagerDelta = self - base2;
        bankManagerVtbl.e[1].delta = D_00483948_v.e[1].delta + bankManagerDelta;
    }
    *(int*)(self + 0x3E4) = 0;
    *(int*)(self + 0x3E8) = 0;
    *(void***)(self + 0x5440) = cMusicSys_vtbl;
    *(int*)(self + 0x3EC) = 0;
    *(int*)(self + 0x3F0) = 0;
    *(long*)(self + 0x3F8) = 0;
    *(int*)(self + 0x408) = 0;
    *(int*)(self + 0x418) = 0;
    *(int*)(self + 0x5438) = 0;
    *(int*)(self + 0x543C) = 0;
    gMusicSys = (cMusicSys*)((void***)self);
    *(char**)(self + 0x5434) = new (str_MPF_data_file, 0, 0) char[25600];
    sBank28C0* sv = (sBank28C0*)self;
    int i = 0;
    do {
        int* categoryInts = Row_28C0(((sSpeechMon*)self)->grid, i);
        for (int j = 4; j >= 0; j--)
            categoryInts[j] = 0;
        i++;
    } while (i < 0x40);
    {
        int* categoryInts = (int*)(self + 0x920);
        for (int j = 4; j >= 0; j--)
            categoryInts[j] = 0;
    }
    for (int textOffset = 0; textOffset < 6400; textOffset += 100) {
        self[textOffset + 2356] = D_004A35A8[0];
        self[textOffset + 8756] = D_004A35A8[0];
        self[textOffset + 15156] = D_004A35A8[0];
    }
    sLoc28C0 musicInf;
    FileParser_FileParser_p(musicInf.parser, D_004A37E8, 0x100);
    *(int*)(musicInf.parser + 8) = *(int*)(musicInf.parser + 4);
    if (FileParser_FindSection(musicInf.parser, str_GLOBAL) != 0) {
        while (FileParser_IsEOF(musicInf.parser) == 0) {
            FileParser_GetName(musicInf.parser);
            FileParser_AssignString(musicInf.parser, str_BASEPATH, self + 0x104);
        }
    }
    *(int*)(musicInf.parser + 8) = *(int*)(musicInf.parser + 4);
    *(int*)(self + 0x3E4) = 0;
    *(int*)(self + 0x3EC) = 0;
    while (*(int*)(self + 0x3E4) < 0x40) {
        char* tok = FileParser_FindNextSection(musicInf.parser);
        if (tok == 0)
            break;
        if (strcasecmp(tok, str_GLOBAL) == 0)
            continue;
        // The song's bit is built in a variable: the target keeps its li 1 in the loop while the
        // other constants (100) are hoisted.
        long bit = 1;
        bit <<= *(int*)(self + 0x3E4);
        *(long*)(self + 0x3F8) |= bit;
        strcpy(self + (*(int*)(self + 0x3E4) * 11 + 292), tok);
        sv->ents[*(int*)(self + 0x3E4)] = (char*)cSongConfiguration_cSongConfiguration((cSongConfiguration*)cMemMan_alloc(172, str_cSongConfiguration, 0, 0));
        while (FileParser_IsEOF(musicInf.parser) == 0) {
            char* key = FileParser_GetName(musicInf.parser);
            if (strcasecmp(key, str_TITLE) == 0) {
                FileParser_AssignString(musicInf.parser, str_TITLE, self + (*(int*)(self + 0x3E4) * 100 + 2356));
            } else if (strcasecmp(key, str_ARTIST) == 0) {
                FileParser_AssignString(musicInf.parser, str_ARTIST, self + (*(int*)(self + 0x3E4) * 100 + 8756));
            } else if (strcasecmp(key, str_ALBUM) == 0) {
                FileParser_AssignString(musicInf.parser, str_ALBUM, self + (*(int*)(self + 0x3E4) * 100 + 15156));
            } else if (strcasecmp(key, str_ADDTOFE) == 0) {
                FileParser_AssignInt(musicInf.parser, str_ADDTOFE, &musicInf.iA);
                if (musicInf.iA == 1)
                    *(int*)(self + 0x3EC) += 1;
            } else if (strcasecmp(key, str_CATEGORY) == 0) {
                FileParser_AssignInt(musicInf.parser, str_CATEGORY, &musicInf.iB);
                if (musicInf.iB < 5)
                    ((sSpeechMon*)self)->grid[((sSpeechMon*)self)->count][musicInf.iB] = 1;
            } else if (strcasecmp(key, str_PATHDATA) == 0) {
                FileParser_AssignString(musicInf.parser, str_PATHDATA, musicInf.buf);
                strcpy(sv->ents[*(int*)(self + 0x3E4)], musicInf.buf);
            } else if (strcasecmp(key, str_MUSDATA) == 0) {
                FileParser_AssignString(musicInf.parser, str_MUSDATA, musicInf.buf);
                strcpy(sv->ents[*(int*)(self + 0x3E4)] + 0x20, musicInf.buf);
            } else if (strcasecmp(key, str_LOOPDATA) == 0) {
                FileParser_AssignString(musicInf.parser, str_LOOPDATA, musicInf.buf);
                strcpy(((sSongCfg28C0*)sv->ents[*(int*)(self + 0x3E4)])->bankData[((sSongCfg28C0*)sv->ents[*(int*)(self + 0x3E4)])->numBankData], musicInf.buf);
                ((sSongCfg28C0*)sv->ents[*(int*)(self + 0x3E4)])->numBankData++;
            } else if (strcasecmp(key, str_BeatsPerMeasure) == 0) {
                FileParser_AssignInt(musicInf.parser, str_BeatsPerMeasure, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x64) = musicInf.iC;
            } else if (strcasecmp(key, str_MeasuresPerBar) == 0) {
                FileParser_AssignInt(musicInf.parser, str_MeasuresPerBar, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x68) = musicInf.iC;
            } else if (strcasecmp(key, str_PhrasesPerBank) == 0) {
                FileParser_AssignInt(musicInf.parser, str_PhrasesPerBank, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x6C) = musicInf.iC;
            } else if (strcasecmp(key, str_BeatsPerPhrase) == 0) {
                FileParser_AssignInt(musicInf.parser, str_BeatsPerPhrase, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x70) = musicInf.iC;
            } else if (strcasecmp(key, str_PhraseAlign) == 0) {
                FileParser_AssignInt(musicInf.parser, str_PhraseAlign, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x74) = musicInf.iC;
            } else if (strcasecmp(key, str_DelayCount) == 0) {
                FileParser_AssignInt(musicInf.parser, str_DelayCount, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x78) = musicInf.iC;
            } else if (strcasecmp(key, str_DelayFeedback) == 0) {
                FileParser_AssignInt(musicInf.parser, str_DelayFeedback, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x7C) = musicInf.iC;
            } else if (strcasecmp(key, str_DelayTime) == 0) {
                FileParser_AssignInt(musicInf.parser, str_DelayTime, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x80) = musicInf.iC;
            } else if (strcasecmp(key, str_DelayLevel) == 0) {
                FileParser_AssignInt(musicInf.parser, str_DelayLevel, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x84) = musicInf.iC;
            } else if (strcasecmp(key, str_Pathlevel) == 0) {
                FileParser_AssignInt(musicInf.parser, str_Pathlevel, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x88) = musicInf.iC;
            } else if (strcasecmp(key, str_Asynclevel) == 0) {
                FileParser_AssignInt(musicInf.parser, str_Asynclevel, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x8C) = musicInf.iC;
            } else if (strcasecmp(key, str_BPM) == 0) {
                FileParser_AssignFloat(musicInf.parser, str_BPM, &musicInf.fD);
                *(float*)(sv->ents[*(int*)(self + 0x3E4)] + 0x90) = musicInf.fD;
            } else if (strcasecmp(key, str_SEDVALUE) == 0) {
                FileParser_AssignInt(musicInf.parser, str_SEDVALUE, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x98) = musicInf.iC;
            } else if (strcasecmp(key, str_LOWPASS) == 0) {
                FileParser_AssignInt(musicInf.parser, str_LOWPASS, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x9C) = musicInf.iC;
            } else if (strcasecmp(key, str_DUCKTOLOOPS) == 0) {
                FileParser_AssignInt(musicInf.parser, str_DUCKTOLOOPS, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0x94) = musicInf.iC;
            } else if (strcasecmp(key, str_PREVIEW) == 0) {
                FileParser_AssignInt(musicInf.parser, str_PREVIEW, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0xA0) = musicInf.iC;
            } else if (strcasecmp(key, str_SONGBIG) == 0) {
                FileParser_AssignInt(musicInf.parser, str_SONGBIG, &musicInf.iC);
                *(int*)(sv->ents[*(int*)(self + 0x3E4)] + 0xA4) = musicInf.iC;
            }
        }
        *(int*)(self + 0x3E4) += 1;
    }
    FileParser__FileParser(musicInf.parser, 2);
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3398);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void cBankManager__cBankManager(void* self, int flags);
extern "C" void cStreamSys__cStreamSys(void* self, int flags);
extern "C" void cSoundSys__cSoundSys(cSoundSys* self, int flags);
extern "C" void func_002B1F78(cSongInstance* self, int flags);
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
 * destructor with 3, frees mMPFFile, destroys mSongInstance (func_002B1F78 with 3) and clears gMusicSys. With bit 1 of flags
 * it destroys cStreamSys (cStreamSys__cStreamSys), cBankManager and cSoundSys; bit 0 deletes the object.
 *
 * @param flags  g++ 2.95 destructor flags (bit 1: destroy the virtual bases, bit 0: operator delete)
 */
// PORT: g++ 2.95 virtual-base destruction with stack vtable this-adjust fix-ups, written out by hand.
extern "C" void func_002B3398(cMusicSys* self, int flags)
{
    self->mVtbl = cMusicSys_vtbl;
    *(const sBmonVtblA3398**)(*(char**)(**(char***)self + 0x1D8) + 4) = &D_00483928;
    *(void***)(**(char***)self + 0xAB0) = cBankMonitor_vtbl;
    *(const sBmonVtblB3398**)(**(char***)self + 0x1D4) = &D_00483948;
    if (flags == 0) {
        sBmonVtblA3398 soundSysVtbl = D_00483928;
        *(sBmonVtblA3398**)(*(char**)(**(char***)self + 0x1D8) + 4) = &soundSysVtbl;
        char* base1 = *(char**)(**(char***)self + 0x1D8) - 0x5448;
        int soundSysDelta = ((char*)self) - base1;
        soundSysVtbl.e[1].delta = D_00483928.e[1].delta + soundSysDelta;
        sBmonVtblB3398 bankManagerVtbl = D_00483948;
        *(sBmonVtblB3398**)(**(char***)self + 0x1D4) = &bankManagerVtbl;
        char* base2 = **(char***)self - 0x5450;
        int bankManagerDelta = ((char*)self) - base2;
        bankManagerVtbl.e[1].delta = D_00483948.e[1].delta + bankManagerDelta;
    }
    for (int i = 0; i < self->mNumSongs; i++) {
        char* songConfig = ((char**)(&self->mSongConfig[0]))[i];
        if (songConfig != 0) {
            sBmonVcall3398* dtorSlot = &(*(sBmonVcall3398**)(songConfig + 0xA8))[1];
            dtorSlot->fn(vthis(songConfig, *dtorSlot), 3);
        }
    }
    if (self->mMPFFile != 0) {
        cMemMan_free(self->mMPFFile);
    }
    if (self->mSongInstance != 0) {
        func_002B1F78((cSongInstance*)(self->mSongInstance), 3);
    }
    gMusicSys = 0;
    if (flags & 2) {
        cStreamSys__cStreamSys(*(void**)self, 0);
        cBankManager__cBankManager(**(void***)self, 0);
        cSoundSys__cSoundSys((cSoundSys*)(*(void**)(**(char***)self + 0x1D8)), 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B35A0);
#ifdef SKIP_ASM
extern "C" void func_002B3838(cMusicSys* self, int a1, int a2, int a3);

struct sBmEnt35A0 {
    signed char b0;
    char pad1[3];
    int f4[5];
};

struct sBmPair35A0 {
    int a;
    int b;
};

struct sBmV16_35A0 {
    int f0;
    int f4;
    int f8;
    int fC;
};

struct sBmSub35A0 {
    int f0;
    sBmEnt35A0 cfg;         // 0x04
    volatile int cur;       // 0x1C (target re-reads it after every store and twice back-to-back: volatile)
    sBmEnt35A0* ents;       // 0x20
    int arr24[4];           // 0x24
    int* p34;               // 0x34
    int* p38;               // 0x38
    int* p3C;               // 0x3C
    sBmPair35A0 arr40[4];   // 0x40
    int* p60;               // 0x60
    int* p64;               // 0x64
    int* p68;               // 0x68
    int* p6C;               // 0x6C
    sBmV16_35A0* p70;       // 0x70
    int* p74;               // 0x74
    int* p78;               // 0x78
    int* p7C;               // 0x7C
    int* p80;               // 0x80
    int* p84;               // 0x84
};

struct sBmTrack35A0 {
    char pad0[0x1D8];
    sBmSub35A0 sub;         // 0x1D8
};

/**
 * cMusicSys virtual slot 2: plays a song with a volume control on its own play-option level (func_002B3838).
 *
 * Pushes a level as cSndPlayOpts_OPTS_Open does, stores the volume control in it, starts the song and pops the level.
 * cSSXAudio's override func_0028F478 calls it; callers pass the song name, bank 12 and music volume control 1.
 *
 * @param songName  the song (section) name, or 0 for mCurrentSongNum
 * @param bankIndex  the cBankManager bank for its loop overlay
 * @param volumeControl  the volume control (float*)
 * @param syncLoad  nonzero to load and open the streams at once
 */
extern "C" void func_002B35A0(sBmTrack35A0*** self, int songName, int bankIndex, int volumeControl, int syncLoad)
{
    sBmTrack35A0* bankManager = **self;
    sBmSub35A0* playOpts = &bankManager->sub;
    bankManager->sub.cur++;
    playOpts->ents[bankManager->sub.cur] = playOpts->cfg;
    bankManager->sub.arr24[bankManager->sub.cur] = 0;
    playOpts->p34[bankManager->sub.cur] = 0;
    playOpts->p38[bankManager->sub.cur] = playOpts->ents[bankManager->sub.cur].b0;
    playOpts->p3C[bankManager->sub.cur] = 0;
    memset((char*)playOpts + bankManager->sub.cur * 8 + 0x40, 0, 8);
    playOpts->p60[bankManager->sub.cur] = 0;
    playOpts->p64[bankManager->sub.cur] = 0;
    playOpts->p68[bankManager->sub.cur] = 0;
    playOpts->p6C[bankManager->sub.cur] = 0;
    playOpts->p70[bankManager->sub.cur].f4 = 0;
    playOpts->p70[bankManager->sub.cur].f0 = 100;
    playOpts->p70[bankManager->sub.cur].f8 = 90;
    playOpts->p70[bankManager->sub.cur].fC = 50;
    playOpts->p74[bankManager->sub.cur] = 0;
    playOpts->p80[bankManager->sub.cur] = 0;
    playOpts->p84[bankManager->sub.cur] = 0;
    playOpts->p78[bankManager->sub.cur] = 0x7F;
    playOpts->p7C[bankManager->sub.cur] = 1;
    sBmTrack35A0* volManager = **self;
    volManager->sub.p3C[volManager->sub.cur] = volumeControl;
    func_002B3838((cMusicSys*)self, songName, bankIndex, syncLoad);
    sBmTrack35A0* popManager = **self;
    popManager->sub.cur--;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3838);
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

// The speech-bank monitor (func_002B3398 dtor): up to 64 named banks, the selection mask and per-kind counts.
// PORT: 64-bit `long` mask (8 bytes on EE, 4 on Windows); use uint64_t off-PS2.
extern "C" void SNDSYS_leavecritical(void);
extern "C" void SNDSYS_entercritical(void);
extern "C" int strcmp(void*, void*);
extern "C" void func_002B3AC0(cMusicSys* self);
extern "C" void func_002B43D8(cMusicSys* self);
extern "C" void* func_002B1B98(int a0, int a1, int a2, int a3, char a4, int a5, int a6, int a7);

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
 * leaves -1 and reads mSongConfig[-1]. Creates the song with func_002B1B98(mBasePath, name, config, play options, base
 * volume, volume control, a2, a3), loads its overlay bank (func_002B43D8), sets unk_41C = -1 and mSongPlaying = 1.
 *
 * @param self_  the cMusicSys
 * @param songName  the song name, or 0 for mCurrentSongNum
 * @param bankIndex  the cBankManager bank for its loop overlay
 * @param syncLoad  nonzero to load synchronously
 */
extern "C" void func_002B3838(cMusicSys* self_, int songName, int bankIndex, int syncLoad)
{
    sSpeechMon* self = (sSpeechMon*)self_;
    cBankManager* bankManager;
    sSndPlayer* sub;
    float* pf;
    char baseVol;
    int songConfig;
    int name;
    int base;
    func_002B3AC0((cMusicSys*)self);
    bankManager = *self->pp;
    sub = &bankManager->sub;
    baseVol = sub->vals[sub->cur];
    pf = (float*)sub->p3C[sub->cur];
    if (pf != 0) {
        float scaledVol = (float)baseVol * *pf;
        scaledVol = bmMax3838(bmMin3838(scaledVol, 127.0f), 0.0f);
        sub->vals[bankManager->sub.cur] = (char)(int)scaledVol;
        {
            signed char* levelVol = &((sSndOptsRec*)(bankManager->sub.cur * 24 + (int)sub->ch))->vol;
            unsigned char clampedVol;
            if (sub->vals[sub->cur] > 0x7E || sub->vals[sub->cur] >= 0) {
                clampedVol = sub->vals[sub->cur] < 0x7F ? (unsigned char)sub->vals[sub->cur] : 0x7F;
            } else {
                clampedVol = 0;
            }
            *levelVol = clampedVol;
        }
    }
    if (songName != 0) {
        int idx = self->count - 1;
        while (idx >= 0) {
            if (strcmp(self->names[idx], (void*)songName) == 0) {
                break;
            }
            idx--;
        }
        self->current = idx;
        name = songName;
        base = (int)&(*self->pp)->sub;
        *(int*)((char*)self + 0x408) = (int)func_002B1B98((int)self->obj, name, self->arr[idx], base, baseVol, (int)pf, bankIndex, syncLoad);
    } else {
        int k = self->current;
        name = (int)self->names[k];
        songConfig = self->arr[k];
        base = (int)&(*self->pp)->sub;
        *(int*)((char*)self + 0x408) = (int)func_002B1B98((int)self->obj, name, songConfig, base, baseVol, (int)pf, bankIndex, syncLoad);
    }
    func_002B43D8((cMusicSys*)self);
    *(int*)((char*)self + 0x41C) = -1;
    *(int*)((char*)self + 0x418) = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3A70);
#ifdef SKIP_ASM
extern "C" void func_002B2018(cSongInstance*);

/**
 * Pauses the current song (func_002B2018) when there is one.
 */
extern "C" void func_002B3A70(cMusicSys* self)
{
    void* curSong = self->mSongInstance;
    if (curSong != 0) {
        func_002B2018((cSongInstance*)curSong);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3A98);
#ifdef SKIP_ASM
extern "C" void func_002B2070(cSongInstance*);

/**
 * Resumes the current song (func_002B2070) when there is one.
 */
extern "C" void func_002B3A98(cMusicSys* self)
{
    void* curSong = self->mSongInstance;
    if (curSong != 0) {
        func_002B2070((cSongInstance*)curSong);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3AC0);
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
extern "C" void func_002B3A98(cMusicSys* self);
extern "C" void func_002B1F78(cSongInstance* self, int flags);

/**
 * Stops the current song: cancels its callbacks, unloads its overlay bank and destroys it.
 *
 * Cancels D_004A3838, D_004A3840 and D_004A3848 (cMonitor_MONITOR_DestroyAll1), cancels and unloads bank unk_34
 * (cAsyncSys_ASYNCSYS_Cancel, cBankManager_BANK_UnLoad), resumes the song (func_002B3A98), clears mSongPlaying, destroys mCurrent
 * (func_002B1F78 with 3) and clears it.
 */
// PORT: 64-bit long sound keys
extern "C" void func_002B3AC0(cMusicSys* self)
{
    cMonitor_MONITOR_DestroyAll1_3AC0(**(char***)self + 0x1D8, D_004A3838[0].v);
    cMonitor_MONITOR_DestroyAll1_3AC0(**(char***)self + 0x1D8, D_004A3840[0].v);
    cMonitor_MONITOR_DestroyAll1_3AC0(**(char***)self + 0x1D8, D_004A3848[0].v);
    if (self->mSongInstance != 0) {
        cAsyncSys_ASYNCSYS_Cancel(**(void***)self, self->mSongInstance->unk_34);
        cBankManager_BANK_UnLoad(**(void***)self, self->mSongInstance->unk_34);
    }
    func_002B3A98(self);
    self->mSongPlaying = 0;
    if (self->mSongInstance != 0) {
        func_002B1F78((cSongInstance*)(self->mSongInstance), 3);
    }
    self->mSongInstance = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3B88);
#ifdef SKIP_ASM
extern "C" void func_002B20C8(cSongInstance* mon, int a1, int a2);

/**
 * cMusicSys virtual slot 3: sends a music event to the current song (func_002B20C8, not forced) while playing.
 *
 * @param musicEvent  the music event
 */
extern "C" void func_002B3B88(cMusicSys* self, int musicEvent)
{
    if (self->mSongPlaying != 0) {
        void* curSong = self->mSongInstance;
        if (curSong != 0) {
            func_002B20C8((cSongInstance*)curSong, musicEvent, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3BC0);
#ifdef SKIP_ASM
struct sBankMonVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * cMusicSys virtual slot 4: changes the music state; calls virtual slot 3 and stores v in unk_41C when it differs.
 *
 * Only while a song is playing (mSongPlaying). v stays in the second argument register for the slot 3 call.
 *
 * @param musicState  the new music state
 * @return 1 when the state changed, else 0
 */
extern "C" int func_002B3BC0(cMusicSys* self, int musicState)
{
    int changed = 0;
    if (self->mSongPlaying != 0 && musicState != self->unk_41C) {
        sBankMonVEntry* vt = (sBankMonVEntry*)self->mVtbl;
        vt[3].fn(vthis(self, vt[3]));
        self->unk_41C = musicState;
        changed = 1;
    }
    return changed;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3C28);
#ifdef SKIP_ASM
// PORT: called with the monitor as `this`; the unit declares func_002B2120() with no args
void func_002B2120_m(void* mon) __asm__("func_002B2120");

/**
 * Calls func_002B2120 on the current song while playing, passing the caller's second argument through.
 *
 * The C declaration takes only self; the callers pass a volume (0..127) as the second argument.
 */
extern "C" void func_002B3C28(cMusicSys* self)
{
    if (self->mSongPlaying != 0) {
        void* curSong = self->mSongInstance;
        if (curSong != 0) {
            func_002B2120_m(curSong);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3C60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_002B2140_3C60(int) __asm__("func_002B2140");

/**
 * Calls func_002B2140 on the current song while playing, passing the caller's second argument through.
 *
 * @param musicSys  the cMusicSys
 */
extern "C" void func_002B3C60(cMusicSys* musicSys) {
    int temp_4;

    if ((musicSys->mSongPlaying) != 0) {
        temp_4 = ((int)musicSys->mSongInstance);
        if (temp_4 != 0) {
            func_002B2140_3C60(temp_4);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3C98);
#ifdef SKIP_ASM
/**
 * Sets the current song's base volume (unk_40) while a song is playing.
 *
 * @param baseVol  the base volume
 */
extern "C" void func_002B3C98(cMusicSys* self, signed char baseVol)
{
    if (self->mSongPlaying != 0) {
        cSongInstance* curSong = self->mSongInstance;
        if (((void*)curSong) != 0) {
            curSong->unk_40 = baseVol;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3CC0);
#ifdef SKIP_ASM
/**
 * Sets the current song's unk_54, which func_002B21E0 and func_002B23E8 need set to apply volume and level.
 *
 * @param enabled  the new value
 */
extern "C" void func_002B3CC0(cMusicSys* self, int enabled)
{
    cSongInstance* curSong = self->mSongInstance;
    if (((void*)curSong) != 0) {
        curSong->unk_54 = enabled;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3CD8);
#ifdef SKIP_ASM
/**
 * Sets the current song's low-pass cutoff (func_002B2160) while a song is playing.
 *
 * @param cutoff  the cutoff
 */
extern "C" void func_002B3CD8(cMusicSys* self, int cutoff)
{
    if (self->mSongPlaying != 0) {
        void* curSong = self->mSongInstance;
        if (curSong != 0) {
            func_002B2160((cSongInstance*)curSong, cutoff);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3D10);
#ifdef SKIP_ASM
extern "C" void func_002B23E8(cSongInstance* self, signed char v);

/**
 * Sets the current song's overlay level (func_002B23E8) while a song is playing.
 *
 * @param levelPercent  the level in percent
 */
extern "C" void func_002B3D10(cMusicSys* self, signed char levelPercent)
{
    if (self->mSongPlaying != 0) {
        void* curSong = self->mSongInstance;
        if (curSong != 0) {
            func_002B23E8((cSongInstance*)curSong, levelPercent);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3D48);
#ifdef SKIP_ASM
extern "C" void func_002B2418(cSongInstance* mon, float v);

/**
 * Fades the current song out over fadeTime seconds (func_002B2418) while a song is playing.
 *
 * @param fadeTime  the fade time in seconds
 */
extern "C" void func_002B3D48(cMusicSys* self, float fadeTime)
{
    if (self->mSongPlaying != 0) {
        void* curSong = self->mSongInstance;
        if (curSong != 0) {
            func_002B2418((cSongInstance*)curSong, fadeTime);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3D80);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPacked8_3D80 { long v; } __attribute__((packed));
extern const sPacked8_3D80 D_004A3850_3D80[] __asm__("D_004A3850");
extern void cMonitor_MONITOR_Create_3D80(int a, int b, long t, int c, int d, int e, int f, int g) __asm__("cMonitor_MONITOR_Create");

/**
 * Starts a base-volume fade of the current song to target over duration seconds, stepped by func_002B3E20.
 *
 * When the song's unk_40 differs it sets unk_414 = target, unk_410 = the current value and unk_40C = ((target -
 * current) / 25) / duration (integer division first, so differences below 25 give no step) and arms D_004A3850 every 40
 * ms.
 *
 * @param target  the target base volume
 * @param duration  the fade time in seconds
 */
extern "C" void func_002B3D80(cMusicSys* self, signed char target, float duration)
{
    signed char cur;
    cSongInstance* voice = self->mSongInstance;
    if (((void*)voice) != 0 && target != (cur = voice->unk_40)) {
        int steps = (target - cur) / 25;
        self->unk_414 = target;
        self->unk_410 = (float)cur;
        self->unk_40C = (float)steps / duration;
        cMonitor_MONITOR_Create_3D80(**(int**)self + 472, 40, D_004A3850_3D80[0].v, 0, 0, 0, 0, 0);
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3E20);
#ifdef SKIP_ASM
/**
 * Monitor callback stepping the base-volume fade started by func_002B3D80 (adds unk_40C to unk_410).
 *
 * Writes the new value to the current song's unk_40 and returns 0 while short of the target (unk_414); then writes the
 * target and returns 1. Reached through D_004A3850.
 *
 * @return 1 when the fade is done, else 0
 */
extern "C" int func_002B3E20(cMusicSys* self)
{
    float step = self->unk_40C;
    float cur = self->unk_410 + step;
    self->unk_410 = cur;
    if ((step > 0.0f && cur < (float)self->unk_414)
        || (step < 0.0f && cur > (float)self->unk_414)) {
        self->mSongInstance->unk_40 = (signed char)(int)self->unk_410;
        return 0;
    }
    signed char targetVol = self->unk_414;
    self->mSongInstance->unk_40 = targetVol;
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3EB0);
#ifdef SKIP_ASM
/**
 * Empties the active song set: mActiveMap = 0, mNumActiveSongs = 0 and mCategoryCount cleared.
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_002B3EB0(cMusicSys* self)
{
    self->mActiveMap = 0;
    self->mNumActiveSongs = 0;
    for (int i = 4; i >= 0; i--) {
        ((int*)&self->mCategoryCount)[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3EE8);
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
extern "C" void func_002B3EE8(cMusicSys* self, const char* name)
{
    if (self->mNumActiveSongs < 64) {
        for (int i = 0; i < self->mNumSongs; i++) {
            if (strstr(self->mSongName[i], name) != 0) {
                self->mActiveMap |= (ulong)1 << i;
                self->mNumActiveSongs++;
                for (int j = 0; j < 5; j++) {
                    if (self->mCategory[i][j] != 0) {
                        self->mCategoryCount[j]++;
                    }
                }
                return;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3FE8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sBankMonUse {
    char pad0[0x3E8];
    int count;                  // 0x3E8
    char pad3EC[0x3F8 - 0x3EC];
    ulong mask;                 // 0x3F8
    char pad400[0x420 - 0x400];
    int grid[64][5];            // 0x420
    int totals[5];              // 0x920
};

/**
 * Adds song idx to the active set: sets its mActiveMap bit, counts it and its categories (at most 64).
 *
 * @param idx  the song index
 */
extern "C" void func_002B3FE8(cMusicSys* self, int idx)
{
    if (self->mNumActiveSongs < 64) {
        self->mNumActiveSongs++;
        self->mActiveMap |= (ulong)1 << idx;
        for (int i = 0; i < 5; i++) {
            if (self->mCategory[idx][i] != 0) {
                self->mCategoryCount[i]++;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4060__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 */
void func_002B4060(void* self)
{
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_getActiveMap);
#ifdef SKIP_ASM
/**
 * Returns mActiveMap.
 *
 * @param musicSys  the cMusicSys
 * @return the 64-bit active song mask
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long cMusicSys_getActiveMap(cMusicSys* musicSys) {
    return ((long)musicSys->mActiveMap);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B4070);
#ifdef SKIP_ASM
// PORT: 64-bit `long` mask (8 bytes on EE); use uint64_t off-PS2.
struct sBankMonUse;
extern "C" void func_002B3EB0(cMusicSys* self);
extern "C" void func_002B3FE8(cMusicSys* self, int idx);
void func_002B4060(void* self);

/**
 * Sets the active songs from a mask over the first mNumFESongs songs (func_002B3EB0, then func_002B3FE8).
 *
 * Ends with the empty func_002B4060.
 *
 * @param mask  the 64-bit song mask
 */
extern "C" void func_002B4070(cMusicSys* self, ulong mask)
{
    func_002B3EB0(self);
    for (int i = 0; i < self->mNumFESongs; i++) {
        int bit = (mask >> i) & 1;
        if (bit) {
            func_002B3FE8((cMusicSys*)((sBankMonUse*)self), i);
        }
    }
    func_002B4060(self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B40F0);
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
extern "C" sBankEntry64* func_002B40F0(cMusicSys* self, int i)
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
INCLUDE_ASM("sound/bankmonitor", func_002B4120);
#ifdef SKIP_ASM
/**
 * Returns song i's artist (mArtist[i]); i < 0 means the current song; 0 when there is none.
 *
 * @param i  the song index, or < 0 for mCurrentSongNum
 * @return the artist
 */
extern "C" sBankEntry64* func_002B4120(cMusicSys* self, int i)
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
INCLUDE_ASM("sound/bankmonitor", func_002B4150);
#ifdef SKIP_ASM
/**
 * Returns song i's album (mAlbum[i]); i < 0 means the current song; 0 when there is none.
 *
 * @param i  the song index, or < 0 for mCurrentSongNum
 * @return the album
 */
extern "C" sBankEntry64* func_002B4150(cMusicSys* self, int i)
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
INCLUDE_ASM("sound/bankmonitor", func_002B4180);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_002B42B8_vii(void* self, int filter, int kind) __asm__("func_002B42B8");

/**
 * Resets mCurrentSongNum to -1 and advances to the next active song n + 1 times (func_002B42B8, unfiltered).
 *
 * @param n  the number of extra steps
 */
extern "C" void func_002B4180(cMusicSys* self, int n)
{
    self->mCurrentSongNum = -1;
    func_002B42B8_vii(self, 0, 0);
    for (int i = 0; i < n; i++) {
        func_002B42B8_vii(self, 0, 0);
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002B41E8);
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
extern "C" void func_002B41E8(sMon41E8* self, int filter, int kind)
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
INCLUDE_ASM("sound/bankmonitor", func_002B42B8);
#ifdef SKIP_ASM
struct func_002B42B8_sMon {
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
 * The category applies only when filter is set and mCategoryCount[kind] is nonzero. func_0028D488 uses it to pick a
 * song.
 *
 * @param filter  nonzero to stay within the category
 * @param kind  the category (0..4)
 */
extern "C" void func_002B42B8(func_002B42B8_sMon* self, int filter, int kind)
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
INCLUDE_ASM("sound/bankmonitor", func_002B4388);
#ifdef SKIP_ASM
extern "C" void func_002B21E0(cSongInstance*, int);
extern "C" void func_002B4620(void*);

/**
 * Updates the current song (func_002B21E0 without force) and starts its overlay (func_002B4620) when unk_50 is set.
 */
extern "C" void func_002B4388(cMusicSys* self)
{
    void* curSong = self->mSongInstance;
    if (curSong != 0) {
        func_002B21E0((cSongInstance*)curSong, 0);
        if (self->mSongInstance->unk_50 != 0) {
            func_002B4620(self);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B43D8);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_002B24C0(cSongInstance* p);
extern "C" void cBankManager_BANK_AsyncLoad(void* self, int i, void* id, int a3);
extern char str_pct_spct_s_c[];

struct sBankMonHdr { char pad[0x40]; int f40; char names[1][0x20]; };

/**
 * Loads a random loop bank of the current song into its cBankManager bank (cBankManager_BANK_AsyncLoad) and sets unk_38 to 1.
 *
 * Only when the song has loop banks; the file is "<mPath><mBankData[n]>" with n from func_002B24C0.
 */
extern "C" void func_002B43D8(cMusicSys* self)
{
    char buf[256];
    cSongInstance* songInstance = (cSongInstance*)self->mSongInstance;
    if (((void*)songInstance) != 0) {
        int bank = songInstance->unk_34;
        if ((*(sBankMonHdr**)songInstance)->f40 != 0) {
            int n = func_002B24C0((cSongInstance*)songInstance);
            cSongInstance* currentSong = self->mSongInstance;
            sprintf(buf, str_pct_spct_s_c, currentSong->mPath, (*(sBankMonHdr**)currentSong)->names[n]);
            cBankManager_BANK_AsyncLoad(**(void***)self, bank, buf, 0x100);
            self->mSongInstance->unk_38 = 1;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B4470);
#ifdef SKIP_ASM
struct sPacked8_4470 { long v; } __attribute__((packed));
extern const sPacked8_4470 D_004A3838_4470[] __asm__("D_004A3838");
extern void cMonitor_MONITOR_Create_4470(int a, int b, long t, int c, int d, int e, int f, int g) __asm__("cMonitor_MONITOR_Create");

/**
 * Arms the D_004A3838 callback (func_002B4740) at once and sets unk_4C, when the current song's unk_4C is 0.
 */
// PORT: 64-bit long sound key, read unaligned from a const table
extern "C" void func_002B4470(cMusicSys* self)
{
    cSongInstance* songInstance = self->mSongInstance;
    if (((void*)songInstance) != 0 && songInstance->unk_4C == 0) {
        songInstance->unk_4C = 1;
        cMonitor_MONITOR_Create_4470(*(int*)(*(int*)self) + 472, 0, D_004A3838_4470[0].v, 0, 0, 0, 0, 0);
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002B44D8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPacked8 { long v; } __attribute__((packed));
extern const sPacked8 D_004A3838_2B44D8[] __asm__("D_004A3838");
extern const sPacked8 D_004A3840_2B44D8[] __asm__("D_004A3840");
extern "C" void cMonitor_MONITOR_DestroyAll1(int a, long t);
extern "C" void func_002B4708(cMusicSys* self);

/**
 * Cancels the song's D_004A3838 and D_004A3840 callbacks (cMonitor_MONITOR_DestroyAll1) and clears unk_4C (func_002B4708).
 *
 * Only when there is a current song.
 */
extern "C" void func_002B44D8(cMusicSys* self)
{
    if (self->mSongInstance != 0) {
        cMonitor_MONITOR_DestroyAll1(*(int*)(*(int*)self) + 472, D_004A3838_2B44D8[0].v);
        cMonitor_MONITOR_DestroyAll1(*(int*)(*(int*)self) + 472, D_004A3840_2B44D8[0].v);
        func_002B4708(self);
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4540);
#ifdef SKIP_ASM
class func_002B4540_cObj {
public:
    int unk0;
    void* entries[(0x5440 - 4) / 4];
    // vptr at 0x5440; slot N at vtable offset N*8
    virtual int v01(int a);
};

/**
 * Monitor callback (D_004A3840): calls cMusicSys's virtual slot 1 with the monitor instance.
 *
 * On gSSXAudio the slot is func_0028C430, which plays the next loop-bank phrase.
 *
 * @param monitorInst  the cMonitorInstance
 */
extern "C" void func_002B4540(cMusicSys* self, int monitorInst)
{
    ((func_002B4540_cObj*)self)->v01(monitorInst);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B4568);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern const sPacked8 D_004A3840_2B4568[] __asm__("D_004A3840");
extern "C" int func_002B2778(cSongInstance* ent);
extern "C" int func_002B2500(cSongInstance* ent);
extern "C" void cMonitor_MONITOR_Create(int a, int b, long t, void* c, int d, int e, int f, int g);

/**
 * Arms the beat-synchronised overlay callback (D_004A3840, func_002B4540) for the song ent.
 *
 * The delay is the time to the next beat (func_002B2718); mPtr = mCurrent, mData1 = a random phrase start
 * (func_002B2500), mData2 = the next beat (func_002B2778 + 1), mData3 = the beat length (func_002B2690). Only when
 * there is a current song.
 *
 * @param ent  the cSongInstance to time against
 */
extern "C" void func_002B4568(cMusicSys* self, void* ent)
{
    if (self->mSongInstance != 0) {
        float beatLength = func_002B2690((cSongInstance*)ent);
        float timeToBeat = func_002B2718((cSongInstance*)ent);
        int nextBeat = func_002B2778((cSongInstance*)ent) + 1;
        int phraseStart = func_002B2500((cSongInstance*)ent);
        cMonitor_MONITOR_Create(**(int**)self + 472, (int)timeToBeat, D_004A3840_2B4568[0].v, self->mSongInstance, phraseStart, nextBeat, (int)beatLength, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4620);
#ifdef SKIP_ASM
extern "C" void func_003D2DF8(int cmd, int a, int b);

struct sBmStream4620 {
    int state;      // 0x0
    int handle;     // 0x4
    char pad[0x60 - 8];
};

static inline int func_002B4620_ready(sBmStream4620* e)
{
    return e->state == 1;
}

// PORT: the unit declares func_002B4620 as void; it really returns 0/1.
int func_002B4620_impl(void* self) __asm__("func_002B4620");

/**
 * Starts the current song's overlay once its loop bank is loaded; returns whether the overlay is running.
 *
 * Sets unk_4C. With unk_38 == 1 and the bank record in state 1 it passes the bank handle to the overlay stream
 * (func_003D2DF8(0x11000002, 0, handle)) and sets unk_38 = 2. Called by func_002B4388 and the audio update.
 *
 * @return 1 when started or already running, 0 otherwise (also without a song)
 */
int func_002B4620_impl(void* vself)
{
    cMusicSys* self = (cMusicSys*)vself;
    int one = 1;
    if ((char*)self->mSongInstance == 0) return 0;
    self->mSongInstance->unk_4C = one;
    int ready = 0;
    char* curSong = (char*)self->mSongInstance;
    int st = *(int*)(curSong + 0x38);
    if (st == one) {
        // PORT: pointer arithmetic through int
        sBmStream4620* bankInst = (sBmStream4620*)(*(int*)(curSong + 0x34) * 0x60 + *(int*)(**(char***)self + 0xACC));
        if (func_002B4620_ready(bankInst)) {
            func_003D2DF8(0x11000002, 0, bankInst->handle);
            self->mSongInstance->unk_38 = 2;
            ready = 1;
        }
    } else if (st == 2) {
        ready = one;
    }
    return ready;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B46D0);
#ifdef SKIP_ASM
/**
 * Sets the current song's unk_50, so func_002B4388 starts its overlay.
 */
extern "C" void func_002B46D0(cMusicSys* self)
{
    cSongInstance* curSong = self->mSongInstance;
    if (((void*)curSong) != 0) {
        curSong->unk_50 = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B46E8);
#ifdef SKIP_ASM
/**
 * Returns the current song's unk_4C (set while its overlay is armed), or 0 without a song.
 *
 * @return unk_4C, or 0
 */
extern "C" void* func_002B46E8(cMusicSys* self)
{
    void* inner = self->mSongInstance;
    if (inner == 0) {
        return 0;
    }
    return *(void**)((char*)inner + 0x4C);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4708);
#ifdef SKIP_ASM
/**
 * Clears the current song's unk_4C when there is a song.
 */
extern "C" void func_002B4708(cMusicSys* self)
{
    cSongInstance* curSong = self->mSongInstance;
    if (((void*)curSong) != 0) {
        curSong->unk_4C = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4720);
#ifdef SKIP_ASM
/**
 * Returns the current song's base volume (unk_40), or 0 without a song.
 *
 * @return the base volume
 */
extern "C" int func_002B4720(cMusicSys* self)
{
    cSongInstance* curSong = self->mSongInstance;
    if (((char*)curSong) == 0) {
        return 0;
    }
    return curSong->unk_40;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B4740);
#ifdef SKIP_ASM
extern "C" void func_002B4568(cMusicSys* self, void* ent);

/**
 * Monitor callback (D_004A3838) waiting for the song's loop bank: arms the beat callback once it is loaded.
 *
 * By the bank record's state: 1 arms func_002B4568 and returns 1; 2 returns 0 (call again); 0 and others return 1.
 *
 * @return 1 when done, 0 to be called again
 */
extern "C" int func_002B4740(cMusicSys* self)
{
    void* ent = self->mSongInstance;
    char* table = *(char**)((char*)**(void***)self + 0xACC);
    int state = *(int*)(table + *(int*)((char*)ent + 0x34) * 0x60);
    switch (state) {
    case 0:
        return 1;
    case 1:
        func_002B4568(self, ent);
        return 1;
    case 2:
        return 0;
    default:
        return 1;
    }
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002B47C0);
#ifdef SKIP_ASM
extern "C" int cBankManager_BANK_Handle(void*, int);
extern "C" int SNDbankinuse(int);

/**
 * Monitor callback (D_004A3848): when the song's loop bank is not loaded or not in use, clears unk_4C and ends.
 *
 * The check (state 1 and SNDbankinuse == 1) runs inside the critical section.
 *
 * @return 1 when unk_4C was cleared, 0 to be called again
 */
extern "C" int func_002B47C0(cMusicSys* self)
{
    int ok = 0;
    int slot = self->mSongInstance->unk_34;
    void* other = **(void***)self;
    SNDSYS_entercritical();
    bool notReady = *(int*)(slot * 0x60 + *(int*)((char*)other + 0xACC)) != 1;
    if (!notReady) {
        ok = SNDbankinuse(cBankManager_BANK_Handle(other, slot)) == 1;
    }
    SNDSYS_leavecritical();
    if (ok == 0) {
        self->mSongInstance->unk_4C = 0;
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4868__FPv);
#ifdef SKIP_ASM
/**
 * Returns a song's mDuckToLoops (from its cSongConfiguration).
 *
 * @return mDuckToLoops
 */
int func_002B4868(void* vself)
{
    cSongInstance* self = (cSongInstance*)vself;
    return *(int*)((char*)*(void**)self + 0x94);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B4878);
#ifdef SKIP_ASM
/**
 * Returns the current song's mDuckToLoops while a song is playing, otherwise 1.
 *
 * @return mDuckToLoops, or 1
 */
extern "C" int func_002B4878(cMusicSys* self)
{
    if (self->mSongPlaying != 0) {
        void* curSong = self->mSongInstance;
        if (curSong != 0) {
            return func_002B4868(curSong);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B48B8);
#ifdef SKIP_ASM
/**
 * Returns a song's mSedValue (from its cSongConfiguration).
 *
 * @param song  the cSongInstance
 * @return mSedValue
 */
extern "C" int func_002B48B8(cSongInstance *song) {
    return song->mpConfig->mSedValue;
}
#endif
//100%
INCLUDE_ASM("sound/bankmonitor", func_002B48C8);
#ifdef SKIP_ASM
/**
 * Returns the current song's mSedValue while a song is playing, otherwise -1.
 *
 * @return mSedValue, or -1
 */
extern "C" int func_002B48C8(cMusicSys* self)
{
    if (self->mSongPlaying != 0) {
        cSongInstance* curSong = self->mSongInstance;
        if (curSong != 0) {
            return func_002B48B8(curSong);
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4908__FPv);
#ifdef SKIP_ASM
/**
 * Returns mSedValue of the song configuration at mCurrentSongNum (no playing song needed).
 *
 * @return mSedValue
 */
int func_002B4908(void* vself)
{
    cMusicSys* self = (cMusicSys*)vself;
    void* config = (void*)self->mSongConfig[self->mCurrentSongNum];
    return *(int*)((char*)config + 0x98);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B4920);
#ifdef SKIP_ASM
extern "C" void SNDSYS_entercritical(void);
extern "C" void SNDSYS_leavecritical(void);
extern "C" float func_002B2718(cSongInstance* self);

/**
 * Returns the time to the current song's next beat (func_002B2718, in the critical section), or 0 when idle.
 *
 * The script engine's beat timer reads it.
 *
 * @return the time in ms
 */
extern "C" float func_002B4920(cMusicSys* self)
{
    if (self->mSongPlaying == 0) {
        return 0.0f;
    }
    SNDSYS_entercritical();
    float timeToBeat = func_002B2718((cSongInstance*)(self->mSongInstance));
    SNDSYS_leavecritical();
    return timeToBeat;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4978);
#ifdef SKIP_ASM
class func_002B4978_cObj {
public:
    int unk0;
    void* entries[(0x5440 - 4) / 4];
    // vptr at 0x5440; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03(int a);
};

/**
 * Previews song i: calls cMusicSys's virtual slot 3 with its mPreview value when that is >= 0.
 *
 * The message center's music list calls it on gSSXAudio + 0x118.
 *
 * @param i  the song index
 */
extern "C" void func_002B4978(func_002B4978_cObj* self, int i)
{
    int previewValue = *(int*)((char*)self->entries[i] + 0xA0);
    if (previewValue >= 0) {
        self->v03(previewValue);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B49C0);
#ifdef SKIP_ASM
/**
 * Sets the current song's tag (unk_58) while a song is playing.
 *
 * The audio code tags songs 101-103 (peak ambience), 201, 301 ("charsel") and 401 ("pktrans").
 *
 * @param songTag  the tag
 */
extern "C" void func_002B49C0(cMusicSys* self, int songTag)
{
    if (self->mSongPlaying != 0) {
        cSongInstance* curSong = self->mSongInstance;
        if (((void*)curSong) != 0) {
            curSong->unk_58 = songTag;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B49E0);
#ifdef SKIP_ASM
/**
 * Returns the current song's tag (unk_58) while a song is playing, otherwise -1.
 *
 * @return the tag, or -1
 */
extern "C" int func_002B49E0(cMusicSys* self)
{
    if (self->mSongPlaying != 0) {
        cSongInstance* curSong = self->mSongInstance;
        if (((void*)curSong) != 0) {
            return curSong->unk_58;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4A08);
#ifdef SKIP_ASM
/**
 * Sets the current song's unk_5C, which makes func_002B20C8 ignore unforced music events.
 *
 * @param holdEvents  the new value
 */
extern "C" void func_002B4A08(cMusicSys* self, int holdEvents)
{
    cSongInstance* curSong = self->mSongInstance;
    if (((void*)curSong) != 0) {
        curSong->unk_5C = holdEvents;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4A20);
#ifdef SKIP_ASM
extern "C" void ASYNCFILE_cancel(void* f);
extern "C" void ASYNCFILE_release(void* f, int a, int b);
extern "C" void* ASYNCFILE_loadat(void* name, void* buf, int size);
extern "C" void FILE_loadat(void* name, void* buf, int size);

/**
 * Loads an MPF file into mMPFFile (25600 bytes), asynchronously (ASYNCFILE_loadat) or at once (FILE_loadat).
 *
 * A pending async load (unk_5438) is cancelled and released first; an async load's handle is kept in unk_5438.
 *
 * @param name  the file name
 * @param sync  nonzero to load synchronously
 */
extern "C" void func_002B4A20(cMusicSys* self, void* name, int sync)
{
    if (self->unk_5438 != 0) {
        ASYNCFILE_cancel(self->unk_5438);
        ASYNCFILE_release(self->unk_5438, 0, 0);
    }
    if (sync == 0) {
        self->unk_5438 = ASYNCFILE_loadat(name, self->mMPFFile, 25600);
    } else {
        FILE_loadat(name, self->mMPFFile, 25600);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4AA0);
#ifdef SKIP_ASM
extern "C" int ASYNCFILE_getstatus(void*);

/**
 * Returns whether the MPF file can be used: force set, or the async load (unk_5438) reports status 1.
 *
 * @param force  nonzero to return 1 at once
 * @return 1 or 0
 */
extern "C" int func_002B4AA0(cMusicSys* self, int force)
{
    void* asyncFile;
    if (force != 0) {
        return 1;
    }
    asyncFile = self->unk_5438;
    if (asyncFile != 0 && ASYNCFILE_getstatus(asyncFile) == 1) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", cMusicSys_GetMPFFile__FPv);
#ifdef SKIP_ASM
/**
 * Returns mMPFFile, the music system's MPF data buffer.
 *
 * Only func_002B21E0 calls it, on gMusicSys, once the MPF load has finished.
 *
 * @return mMPFFile
 */
int cMusicSys_GetMPFFile(void* vself)
{
    cMusicSys* self = (cMusicSys*)vself;
    return (int)self->mMPFFile;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4AF0);
#ifdef SKIP_ASM
/**
 * Returns whether the song at mCurrentSongNum is active (its bit in mActiveMap).
 *
 * @return 1 or 0
 */
// PORT: needs a 64-bit return (ulong); use uint64_t off-PS2.
extern "C" ulong func_002B4AF0(cMusicSys* self)
{
    return (self->mActiveMap >> self->mCurrentSongNum) & 1;
}
#endif

