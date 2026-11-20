#include "common.h"
#include "sound/ps2soundman.h"
#include "sound/crowdsys.h"
#include "sound/midisys.h"
#include "sound/fileparser.h"
#include "sound/banksys.h"
#include "sound/ssxaudio.h"
#include "be/beintnewrace.h"
#include "scripter/ssxscriptengine.h"
#include "main/ssxapp.h"
#include "ai/rider.h"
#include "ai/ai.h"

//100%
INCLUDE_ASM("sound/soundsys", cBankSys_cBankSys);
#ifdef SKIP_ASM
extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

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

extern char D_00483A48[];
extern char cBankMonitor_vtbl[];
extern const char str_mBank[];
extern const char str_mPath[];
extern const sSndBsVt4 D_00483A60_bs __asm__("D_00483A60");
extern const sSndBsVt7 D_00483A80_bs __asm__("D_00483A80");

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
        char* mon = ((char*)self) + 0x10;
        *(char**)self = ((char*)self) + 0x18;
        *(char**)(((char*)self) + 0x1F0) = mon;
        cSoundSys_cSoundSys(mon, heap);
        cBankManager_cBankManager(*(void**)self, 0, count, heap);
    }
    *(const void**)(*(char**)(*(char**)self + 0x1D8) + 4) = &D_00483A60_bs;
    *(void**)(*(char**)self + 0xAB0) = cBankMonitor_vtbl;
    *(const void**)(*(char**)self + 0x1D4) = &D_00483A80_bs;
    if (!embedded) {
        sSndBsVt4 vtbl1 = D_00483A60_bs;
        *(void**)(*(char**)(*(char**)self + 0x1D8) + 4) = &vtbl1;
        char* base1 = *(char**)(*(char**)self + 0x1D8) - 0x10;
        int adj1 = ((char*)self) - base1;
        vtbl1.e[1].delta = D_00483A60_bs.e[1].delta + adj1;
        sSndBsVt7 vtbl2 = D_00483A80_bs;
        *(void**)(*(char**)self + 0x1D4) = &vtbl2;
        char* base2 = *(char**)self - 0x18;
        int adj2 = ((char*)self) - base2;
        vtbl2.e[1].delta = D_00483A80_bs.e[1].delta + adj2;
    }
    self->mVtbl = (void**)D_00483A48;
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

extern char D_00483A48[];
extern const sSndDtVt4 D_00483A60;
extern const sSndDtVt7 D_00483A80;
extern char cBankMonitor_vtbl[];

/**
 * Destroys the bank SFX system (g++ 2.95 destructor with virtual bases cSoundSys and cBankManager).
 *
 * Restores the class's vtables (D_00483A48, D_00483A60, cBankMonitor_vtbl, D_00483A80); when called from a derived
 * class (@p flags 0) it fixes up stack copies of the virtual-base tables for the offsets. Frees mBank and mPatch and
 * clears gBankSys. Flags bit 1 also destroys the virtual bases (cBankManager__cBankManager, cBankManager::~cBankManager, then
 * cSoundSys__cSoundSys); bit 0 frees the object.
 *
 * @param flags  destructor flags: bit 1 destroys the virtual bases, bit 0 frees the object
 */
// PORT: hand-expanded g++ 2.95 virtual-base dtor (vtable upcast-offset fixup on stack copies).
extern "C" void cBankSys__cBankSys(cBankSys* self, int flags)
{
    self->mVtbl = (void**)D_00483A48;
    *(void**)(*(char**)(*(char**)self + 0x1D8) + 4) = (void*)&D_00483A60;
    *(void**)(*(char**)self + 0xAB0) = cBankMonitor_vtbl;
    *(void**)(*(char**)self + 0x1D4) = (void*)&D_00483A80;
    if (flags == 0) {
        sSndDtVt4 soundSysVtbl = D_00483A60;
        *(void**)(*(char**)(*(char**)self + 0x1D8) + 4) = &soundSysVtbl;
        char* soundSysBase = *(char**)(*(char**)self + 0x1D8) - 0x10;
        int adj1 = ((char*)self) - soundSysBase;
        soundSysVtbl.e[1].delta = D_00483A60.e[1].delta + adj1;
        sSndDtVt7 bankMgrVtbl = D_00483A80;
        *(void**)(*(char**)self + 0x1D4) = &bankMgrVtbl;
        char* bankMgrBase = *(char**)self - 0x18;
        int adj2 = ((char*)self) - bankMgrBase;
        bankMgrVtbl.e[1].delta = D_00483A80.e[1].delta + adj2;
    }
    if (self->mBank) cMemMan_free(self->mBank);
    if (self->mPatch) cMemMan_free(self->mPatch);
    gBankSys = 0;
    if (flags & 2) {
        cBankManager__cBankManager(*(void**)self, 0);
        cSoundSys__cSoundSys(*(void**)(*(char**)self + 0x1D8), 0);
    }
    if (flags & 1) operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cBankSys_Play2DCallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSnd0018Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd0018E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd0018Sub {
    char pad0[4];
    sSnd0018Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd0018Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd0018E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd0018Mgr {
    char pad0[0x1D8];
    sSnd0018Sub sub;         // 0x1D8
};
struct sSnd0018Sys {
    sSnd0018Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
};
struct sSnd0018Req {
    char pad0[0x18];
    int f18;                 // 0x18
    int f1C;                 // 0x1C
    int f20;                 // 0x20
    int vol;                 // 0x24
};


#define SND_0018_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_0018_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline void snd0018AddVoice(void* vself, int a, int b)
{
    sSnd0018Sys* self = (sSnd0018Sys*)vself;
    sSnd0018Mgr* m = self->mgr;
    sSnd0018Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
// Adds a voice on the global sound system for the request, sets its target and volume (clamped to 0..127), then commits.
extern "C" int cBankSys_Play2DCallback(cBankSys* self, sSnd0018Req* req)
{
    snd0018AddVoice((sSnd0018Sys*)gBankSys, req->f1C, req->f20);
    sSnd0018Mgr* bankMgr = (sSnd0018Mgr*)gBankSys->mpVBankManager;
    bankMgr->sub.p3C[bankMgr->sub.cur] = req->f18;
    sSnd0018Mgr* bankMgrVol = (sSnd0018Mgr*)gBankSys->mpVBankManager;
    sSnd0018Sub* s2 = &bankMgrVol->sub;
    s2->vals[bankMgrVol->sub.cur] = req->vol;
    s2->ch[bankMgrVol->sub.cur].vol = SND_0018_MAX(0, SND_0018_MIN(s2->vals[s2->cur], 0x7F));
    cBankSys_SFX_End((cBankSys*)gBankSys);
    return 1;
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cBankSys_Play2DCallbackWithPitchMult);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSnd0358Ch {
    signed char vol;
    char pad1[0xB];
    unsigned short fC;       // 0x0C
    char padE[0xA];
};
struct sSnd0358E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd0358Sub {
    char pad0[4];
    sSnd0358Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd0358Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd0358E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd0358Mgr {
    char pad0[0x1D8];
    sSnd0358Sub sub;         // 0x1D8
};
struct sSnd0358Sys {
    sSnd0358Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
};
struct sSnd0358Req {
    char pad0[0x18];
    int f18;                 // 0x18
    int f1C;                 // 0x1C
    int f20;                 // 0x20
    int vol;                 // 0x24
    unsigned short f28;      // 0x28
};


#define SND_0358_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_0358_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline void snd0358AddVoice(void* vself, int a, int b)
{
    sSnd0358Sys* self = (sSnd0358Sys*)vself;
    sSnd0358Mgr* m = self->mgr;
    sSnd0358Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
// Adds a voice on the global sound system for the request, sets its target and volume (clamped to 0..127), stores its 16-bit field, then commits.
extern "C" int cBankSys_Play2DCallbackWithPitchMult(cBankSys* self, sSnd0358Req* req)
{
    snd0358AddVoice((sSnd0358Sys*)gBankSys, req->f1C, req->f20);
    sSnd0358Mgr* bankMgr = (sSnd0358Mgr*)gBankSys->mpVBankManager;
    bankMgr->sub.p3C[bankMgr->sub.cur] = req->f18;
    sSnd0358Mgr* bankMgrVol = (sSnd0358Mgr*)gBankSys->mpVBankManager;
    sSnd0358Sub* s2 = &bankMgrVol->sub;
    s2->vals[bankMgrVol->sub.cur] = req->vol;
    s2->ch[bankMgrVol->sub.cur].vol = SND_0358_MAX(0, SND_0358_MIN(s2->vals[s2->cur], 0x7F));
    sSnd0358Mgr* bankMgrPitch = (sSnd0358Mgr*)gBankSys->mpVBankManager;
    bankMgrPitch->sub.ch[bankMgrPitch->sub.cur].fC = req->f28;
    cBankSys_SFX_End((cBankSys*)gBankSys);
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cBankSys_SFX_End);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B6820(int bank, int id, void* ch);
extern "C" void* cBankMonitor_BANKMONITOR_Create(void* sub, int h, int vol, int mode);
void func_002ABCE8(void* self, int a1, int a2);
extern "C" void func_002ADCA0(void*, int, long, int, int, int, int, int);

struct sSnd06B8Key {
    long v;
} __attribute__((packed));
// 64-bit sound key view of D_004A3678 (same packed layout as the unit's sSndKey8, declared later).
extern sSnd06B8Key D_004A3678_06B8[] __asm__("D_004A3678");

struct sSnd06B8Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd06B8Ent {
    int type;
    int id;
    char pad8[0x58];
};
struct sSnd06B8Q {
    int step;
    int count;
    int f8;
    int fC;
};
struct sSnd06B8Sub {
    char pad0[4];
    sSnd06B8Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd06B8Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd06B8Q* p70;          // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd06B8Mgr {
    char pad0[0x1D8];
    sSnd06B8Sub sub;         // 0x1D8
    char pad260[0xACC - 0x260];
    sSnd06B8Ent* ents;       // 0xACC
};

#define SND_06B8_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_06B8_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SND_06B8_FMAX(a, b) ((a) >= (b) ? (a) : (b))

// On Tour: cBankInstance::GetStatus() / Handle(), cBankManager::BANK_Used(int) / BANK_Handle(int).
static inline int snd06B8Status(sSnd06B8Ent* e)
{
    return e->type;
}
static inline int snd06B8InstHandle(sSnd06B8Ent* e)
{
    return e->id;
}
static inline bool snd06B8Used(sSnd06B8Mgr* m, int id)
{
    return snd06B8Status(&m->ents[id]) == 1;
}
static inline int snd06B8Handle(sSnd06B8Mgr* m, int id)
{
    if (snd06B8Used(m, id)) return snd06B8InstHandle(&m->ents[id]);
    return -1;
}

// On Tour: cSndPlayOpts::OPTS_GetCloseOnEnd().
static inline int snd06B8CloseOnEnd(sSnd06B8Sub* o)
{
    return o->p7C[o->cur];
}

/**
 * Plays the sound effect set up since SFX_Begin and pops its play-option level (On Tour SFX_End).
 *
 * Uses the play options of the current level (mInst) in the cBankManager's cBankMonitor (mpVBankManager + 0x1D8). A
 * positioned sound (mPosition set) starts at volume 0; otherwise a volume control (mVolControl) scales mBaseVol,
 * clamped to 0..127. Plays bank mBank[mInst] / patch mPatch[mInst] (func_003B6820, between func_003B58A0 and
 * func_003B58D8). Unless the play failed with CloseOnEnd set, creates a bank monitor for the voice
 * (BANKMONITOR_Create, then func_002ABCE8) and queues the echoes from cDelayOpts: each repeat mTime later and
 * mFeedback percent quieter (starting from mLevel percent of the base volume), until mCount runs out or the volume
 * reaches 0 (func_002ADCA0, MONITOR_Create). Finally decrements mInst.
 *
 * @param vself  the bank system (this)
 * @return the voice handle, negative when the play failed
 */
extern "C" int cBankSys_SFX_End(cBankSys* vself)
{
    cBankSys* self = vself;
    sSnd06B8Mgr* bankMgr = ((sSnd06B8Mgr*)self->mpVBankManager);
    sSnd06B8Sub* s = &bankMgr->sub;
    int BaseVol = s->vals[s->cur];
    if (bankMgr->sub.tgt[bankMgr->sub.cur] != 0) {
        s->vals[bankMgr->sub.cur] = 0;
        s->ch[bankMgr->sub.cur].vol = SND_06B8_MAX(0, SND_06B8_MIN(s->vals[s->cur], 0x7F));
    } else if (s->p3C[s->cur] != 0) {
        float gain = *(float*)s->p3C[s->cur];
        s->vals[bankMgr->sub.cur] = (int)SND_06B8_FMAX(SND_06B8_MIN((float)s->vals[s->cur] * gain, 127.0f), 0.0f);
        s->ch[s->cur].vol = SND_06B8_MAX(0, SND_06B8_MIN(s->vals[s->cur], 0x7F));
    }
    int Inst = ((sSnd06B8Mgr*)self->mpVBankManager)->sub.cur;
    func_003B58A0();
    int Handle = func_003B6820(snd06B8Handle(((sSnd06B8Mgr*)self->mpVBankManager), self->mBank[Inst]), self->mPatch[Inst], &((sSnd06B8Mgr*)self->mpVBankManager)->sub.ch[((sSnd06B8Mgr*)self->mpVBankManager)->sub.cur]);
    func_003B58D8();
    if (Handle >= 0 || !snd06B8CloseOnEnd(&((sSnd06B8Mgr*)self->mpVBankManager)->sub)) {
        void* Mon = cBankMonitor_BANKMONITOR_Create(&((sSnd06B8Mgr*)self->mpVBankManager)->sub, Handle, BaseVol, 1);
        if (Mon != 0) {
            func_002ABCE8(Mon, snd06B8Handle(((sSnd06B8Mgr*)self->mpVBankManager), self->mBank[Inst]), self->mPatch[Inst]);
        }
        sSnd06B8Q* Delay = &((sSnd06B8Mgr*)self->mpVBankManager)->sub.p70[((sSnd06B8Mgr*)self->mpVBankManager)->sub.cur];
        if (Delay->count > 0) {
            int DelayTime = 0;
            int Vol = BaseVol * Delay->fC / 100;
            while (Delay->count > 0) {
                Delay->count--;
                DelayTime += Delay->step;
                Vol = Vol * Delay->f8 / 100;
                if (Vol > 0x7F) Vol = 0x7F;
                if (Vol < 0) Vol = 0;
                if (Vol == 0) break;
                sSnd06B8Sub* opts = &((sSnd06B8Mgr*)self->mpVBankManager)->sub;
                func_002ADCA0(opts, DelayTime, D_004A3678_06B8->v, opts->p3C[opts->cur], self->mBank[Inst], self->mPatch[Inst], Vol, 0);
            }
        }
    }
    ((sSnd06B8Mgr*)self->mpVBankManager)->sub.cur--;
    return Handle;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00290B58);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sCurve_290B58 {
    float key[5];       // 0x0
    float value[5];     // 0x14
};

// Piecewise-linear lookup in a 5-point curve; 0 above the last key gap.
// Named _impl: a later block declares func_00290B58 with its own curve struct.
extern "C" float func_00290B58_impl(sCurve_290B58* c, float x) __asm__("func_00290B58");
extern "C" float func_00290B58_impl(sCurve_290B58* c, float x)
{
    for (int i = 1; i < 5; i++) {
        if (c->key[i - 1] <= x && x < c->key[i]) {
            float t = (x - c->key[i - 1]) * (c->value[i] - c->value[i - 1]);
            return c->value[i - 1] + t / (c->key[i] - c->key[i - 1]);
        }
    }
    if (x < c->key[0])
        return c->value[0];
    if (c->key[4] <= x)
        return c->value[4];
    return 0.0f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_00290C10);
#ifdef SKIP_ASM
struct func_00290B58_sCurve {
    float x[5];
    float y[5];
};

extern "C" float func_00290B58(func_00290B58_sCurve* c, float v);
extern func_00290B58_sCurve gAudioTables[];

extern "C" float func_00290C10(int i, float v)
{
    return func_00290B58(&gAudioTables[i], v);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_00290C40);
#ifdef SKIP_ASM
struct sSndVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sSndVEntryVec {
    short delta;
    short index;
    sSndVec4* (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float sndVu0Length(const sSndVec4& v)
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

extern "C" float func_00290C10(int i, float v);

extern "C" int func_00290C40(void* self, void* src)
{
    char* obj = (char*)src + 0x6C0;
    sSndVEntryVec* vt = *(sSndVEntryVec**)obj;
    sSndVec4* velocity = vt[2].fn(obj + vt[2].delta);
    int level = (int)func_00290C10(3, sndVu0Length(*velocity));
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
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSnd0CC0Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd0CC0E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd0CC0Sub {
    char pad0[4];
    sSnd0CC0Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd0CC0Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd0CC0E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd0CC0Mgr {
    char pad0[0x1D8];
    sSnd0CC0Sub sub;         // 0x1D8
};

/**
 * Plays a 2D (non-positional) sound effect from a bank at the sound-effects volume.
 *
 * Pushes a play-option level on the bank manager (mpVBankManager) with default options (the inlined
 * cBankSys::SFX_Begin(Bank, Patch)) and records @p bank in mBank and @p patch in mPatch for that level; sets the
 * level's volume-control pointer to func_00287968(this, 5, 0) (On Tour GetCurrAppliedVolumePtr(5, 0)); then plays it
 * with cBankSys_SFX_End. Virtual slot 1 of cSSXAudio_vtbl, overriding cBankSys's cBankSys_SFX_Play2D.
 *
 * @param vself  the audio system (this)
 * @param bank   the sound bank (On Tour Bank)
 * @param patch  the patch in the bank (On Tour Patch)
 * @return cBankSys_SFX_End's result (the voice handle)
 */
extern "C" int cSSXAudio_SFX_Play2D(cSSXAudio* vself, int bank, int patch)
{
    cSSXAudio* self = vself;
    sSnd0CC0Mgr* bankMgr = ((sSnd0CC0Mgr*)self->mpVBankManager);
    sSnd0CC0Sub* monitor = &bankMgr->sub;
    bankMgr->sub.cur++;
    monitor->ch[bankMgr->sub.cur] = bankMgr->sub.def;
    bankMgr->sub.tgt[bankMgr->sub.cur] = 0;
    monitor->p34[bankMgr->sub.cur] = 0;
    monitor->vals[bankMgr->sub.cur] = monitor->ch[bankMgr->sub.cur].vol;
    monitor->p3C[bankMgr->sub.cur] = 0;
    func_00416210((char*)monitor + bankMgr->sub.cur * 8 + 0x40, 0, 8);
    monitor->p60[bankMgr->sub.cur] = 0;
    monitor->p64[bankMgr->sub.cur] = 0;
    monitor->p68[bankMgr->sub.cur] = 0;
    monitor->p6C[bankMgr->sub.cur] = 0;
    monitor->p70[bankMgr->sub.cur].f4 = 0;
    monitor->p70[bankMgr->sub.cur].f0 = 100;
    monitor->p70[bankMgr->sub.cur].f8 = 90;
    monitor->p70[bankMgr->sub.cur].fC = 50;
    monitor->p74[bankMgr->sub.cur] = 0;
    monitor->p80[bankMgr->sub.cur] = 0;
    monitor->p84[bankMgr->sub.cur] = 0;
    monitor->p78[bankMgr->sub.cur] = 0x7F;
    monitor->p7C[bankMgr->sub.cur] = 1;
    self->mBank[((sSnd0CC0Mgr*)self->mpVBankManager)->sub.cur] = bank;
    self->mPatch[((sSnd0CC0Mgr*)self->mpVBankManager)->sub.cur] = patch;
    sSnd0CC0Mgr** pp = ((sSnd0CC0Mgr**)self->MusicSys.mpVStreamSys);
    int volumePtr = (int)func_00287968(self, 5, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = volumePtr;
    return cBankSys_SFX_End((cBankSys*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_SetCurTrickPower);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_GetFocusRider(void* self, int which);
extern "C" int cSSXAudio_GetNumViews(void* self);

/**
 * Records the current trick power for a rider that one of the listeners follows.
 *
 * @param id  the rider (On Tour cRider* pRider)
 * @param value  trick power; stored for listener 0, or listener 1 in a two-view game
 */
extern "C" void cSSXAudio_SetCurTrickPower(cSSXAudio* self, int id, float value)
{
    if (cSSXAudio_GetFocusRider(self, 0) == id) {
        self->mCurTrickPower[0] = value;
    } else if (cSSXAudio_GetNumViews(self) == 2 && cSSXAudio_GetFocusRider(self, 1) == id) {
        self->mCurTrickPower[1] = value;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00290FD0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cRider_getMotionState(void*);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");

extern "C" int func_00290FD0(void* self, int which)
{
    int result = 3;
    char* rider = (char*)cSSXAudio_GetFocusRider_p(self, which);
    if (cRider_getMotionState(rider) != 1) {
        return 3;
    }
    float trickPower = *(float*)((char*)self + (which << 2) + 0x6074);
    int mode = *(int*)(*(char**)(rider + 0x788) + 0xAC);
    int ok = mode == 1 || mode == 3;
    float delta;
    if (ok) {
        char* st = *(char**)(rider + 0x788);
        delta = *(float*)(st + 0x98) - *(float*)(st + 0xA0);
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
INCLUDE_ASM("sound/soundsys", func_002910E0);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSnd10E0Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd10E0E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd10E0Sub {
    char pad0[4];
    sSnd10E0Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd10E0Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd10E0E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd10E0Mgr {
    char pad0[0x1D8];
    sSnd10E0Sub sub;         // 0x1D8
};
struct sSnd10E0Sys {
    sSnd10E0Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd10E0Mgr** p118;      // 0x118
};

static inline void snd10E0Init(void* vself, int a, int b)
{
    sSnd10E0Sys* self = (sSnd10E0Sys*)vself;
    sSnd10E0Mgr* m = self->mgr;
    sSnd10E0Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
}


struct sSndKey8_10E0 {
    long v;
} __attribute__((packed));
extern sSndKey8_10E0 D_004A3680[];

// PORT: 64-bit long sound key; voice target stored as int
extern "C" void func_002910E0(cSSXAudio* vself)
{
    sSnd10E0Sys* self = (sSnd10E0Sys*)vself;
    if (*(int*)((char*)self + 0x5834) != 0) {
        return;
    }
    snd10E0Init(self, 0, 0x20);
    (*self->p118)->sub.p3C[(*self->p118)->sub.cur] = (int)((char*)self + 0x5830);
    (*self->p118)->sub.p7C[(*self->p118)->sub.cur] = 0;
    sSnd10E0Mgr* m = *self->p118;
    sSndKey8_10E0 k = D_004A3680[0];
    sSnd10E0Sub* s = &m->sub;
    *(sSndKey8_10E0*)((char*)s + (m->sub.cur << 3) + 0x40) = k;
    cBankSys_SFX_End((cBankSys*)self);
    *(int*)((char*)self + 0x5834) = 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_EndWind);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVE_13D8 {
    short delta;
    short index;
    void (*fn)(void*, long);
};

extern const sSndKey8_10E0 D_004A3680_13D8[] __asm__("D_004A3680");

/**
 * Stops the wind stream and clears mWindVolume and mWindPlaying.
 */
// PORT: 64-bit long sound key
extern "C" void cSSXAudio_EndWind(cSSXAudio* self)
{
    char* sys = *(char**)self->MusicSys.mpVStreamSys;
    sVE_13D8* vt = *(sVE_13D8**)(sys + 0xAB0);
    char* obj = sys + 0x1D8;
    vt[3].fn(obj + vt[3].delta, D_004A3680_13D8->v);
    *(int*)(((char*)self) + 0x5830) = 0;
    *(int*)(((char*)self) + 0x5834) = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00291430);
#ifdef SKIP_ASM
extern "C" void func_00291430(void) {

}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayThunder);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSnd1438Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd1438E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd1438Sub {
    char pad0[4];
    sSnd1438Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd1438Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd1438E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd1438Mgr {
    char pad0[0x1D8];
    sSnd1438Sub sub;         // 0x1D8
};
struct sSnd1438Sys {
    sSnd1438Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd1438Mgr** p118;      // 0x118
};

static inline int snd1438AddVoice(void* vself, int a, int b, int type)
{
    sSnd1438Sys* self = (sSnd1438Sys*)vself;
    sSnd1438Mgr* m = self->mgr;
    sSnd1438Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSnd1438Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" void func_003B58A0();
extern "C" int func_003B8AB8(int id);
extern "C" void func_003B58D8();
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Plays a thunder clap, first stopping the previous one if it is still playing.
 *
 * On Tour's version takes a distance; this one always plays bank 8, patch 0x10.
 */
extern "C" void cSSXAudio_PlayThunder(cSSXAudio* self)
{
    func_003B58A0();
    int soundFinished = func_003B8AB8(self->mThunderHandle) != 0;
    func_003B58D8();
    if (!soundFinished) {
        cBankMonitor_BANKMONITOR_StopHandle(*(char**)self->MusicSys.mpVStreamSys + 0x1D8, self->mThunderHandle, 1, 0.0f);
    }
    self->mThunderHandle = snd1438AddVoice(self, 8, 0x10, 5);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_Surface);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cRider_getMotionState(void* rider);

// PORT: the unit declares this as int(void*, int); defined under an asm label with its real type
extern "C" int cSSXAudio_Surface_impl(void* self, void* rider) __asm__("cSSXAudio_Surface");
/**
 * Returns the sound surface class for the ground a rider is on.
 *
 * When cRider_getMotionState reports motion state 4 the surface comes from this+0x598C, otherwise from the rider's surface type
 * at +0x438. Types 2, 3, 13 -> 1; 1 -> 2; 4 -> 3; 7, 8, 18 -> 7; 9 -> 5; 10 -> 4; 11 -> 8; everything else -> 0.
 *
 * @param rider  the rider (On Tour Rider)
 * @return the surface class (0 when the type has none)
 */
extern "C" int cSSXAudio_Surface_impl(void* self, void* rider)
{
    int x;
    if (cRider_getMotionState(rider) == 4) {
        x = *(int*)((char*)self + 0x598C);
    } else {
        x = *(int*)((char*)rider + 0x438);
    }
    switch (x) {
    case 2: case 3: case 13:
        return 1;
    case 1:
        return 2;
    case 4:
        return 3;
    case 7: case 8: case 18:
        return 7;
    case 9:
        return 5;
    case 10:
        return 4;
    case 11:
        return 8;
    case -1: case 0: case 5: case 6: case 12: case 14: case 15: case 16: case 17: case 19:
        return 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002917B8);
#ifdef SKIP_ASM
extern "C" int func_002917B8(void* self, int a1)
{
    if (a1 > 3) {
        return 1;
    }
    return 0x10;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002917D0);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int func_00288A20(void* self, void* obj);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void func_002A9988(void* self, float a, float b);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_002917B8(void* self, int a1);
extern signed char D_00535C11[];

struct sSnd17D0Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd17D0E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd17D0Sub {
    char pad0[4];
    sSnd17D0Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd17D0Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd17D0E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd17D0Mgr {
    char pad0[0x1D8];
    sSnd17D0Sub sub;         // 0x1D8
};
struct sSnd17D0Sys {
    sSnd17D0Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd17D0Mgr** p118;      // 0x118
};
struct sSndKey8_17D0 {
    long v;
} __attribute__((packed));
extern sSndKey8_17D0 D_004A3688_17D0[] __asm__("D_004A3688");

// PORT: 64-bit long sound key
void func_002917D0_17D0(void* vself, void* obj) __asm__("func_002917D0");
void func_002917D0_17D0(void* vself, void* obj)
{
    sSnd17D0Sys* self = (sSnd17D0Sys*)vself;
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (!ok) {
        return;
    }
    int flag = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 2) {
        flag = cSSXAudio_ReplayMode(self) == 0;
    }
    int a = func_002917B8(self, 0);
    sSnd17D0Mgr* m = self->mgr;
    sSnd17D0Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    self->a8[self->mgr->sub.cur] = 0;
    sSnd17D0Mgr* m3 = *self->p118;
    sSnd17D0Sub* s3 = &m3->sub;
    s3->vals[m3->sub.cur] = 0;
    s3->ch[m3->sub.cur].vol = (s3->vals[s3->cur] < 0x7F && s3->vals[s3->cur] < 0) ? 0 : (s3->vals[s3->cur] < 0x7F ? (unsigned char)s3->vals[s3->cur] : 0x7F);
    sSnd17D0Mgr** pp = self->p118;
    int r = (int)func_00287968(self, 6, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    {
        sSnd17D0Mgr* m2 = *self->p118;
        m2->sub.tgt[m2->sub.cur] = (char*)obj + 0x110;
    }
    {
        sSnd17D0Mgr* m2 = *self->p118;
        sSndKey8_17D0 k = D_004A3688_17D0[0];
        sSnd17D0Sub* s2 = &m2->sub;
        *(sSndKey8_17D0*)((char*)s2 + (m2->sub.cur << 3) + 0x40) = k;
    }
    {
        sSnd17D0Mgr* m2 = *self->p118;
        m2->sub.p60[m2->sub.cur] = (int)obj;
    }
    {
        sSnd17D0Mgr** pq = self->p118;
        int q = func_00288A20(self, obj);
        (*pq)->sub.p64[(*pq)->sub.cur] = q;
    }
    func_002A9988(&(*self->p118)->sub, 100.0f, -1.0f);
    (*self->p118)->sub.p7C[(*self->p118)->sub.cur] = 0;
    if (flag) {
        (*self->p118)->sub.p80[(*self->p118)->sub.cur] = 1;
        (*self->p118)->sub.p84[(*self->p118)->sub.cur] = *(int*)((char*)obj + 0x870);
    }
    cBankSys_SFX_End((cBankSys*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00291C88);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int func_00288A20(void* self, void* obj);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void func_002A9988(void* self, float a, float b);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_002917B8(void* self, int a1);
extern signed char D_00535C11[];

struct sSnd1C88Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd1C88E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd1C88Sub {
    char pad0[4];
    sSnd1C88Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd1C88Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd1C88E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd1C88Mgr {
    char pad0[0x1D8];
    sSnd1C88Sub sub;         // 0x1D8
};
struct sSnd1C88Sys {
    sSnd1C88Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd1C88Mgr** p118;      // 0x118
};
struct sSndKey8_1C88 {
    long v;
} __attribute__((packed));
extern sSndKey8_1C88 D_004A3690_1C88[] __asm__("D_004A3690");
extern sSndKey8_1C88 D_004A3698_1C88[] __asm__("D_004A3698");

static inline void snd1C88AddVoice(sSnd1C88Sys* self, int a, int b)
{
    sSnd1C88Mgr* m = self->mgr;
    sSnd1C88Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
}

// PORT: 64-bit long sound key
void func_00291C88_1C88(void* vself, void* obj, int n) __asm__("func_00291C88");
void func_00291C88_1C88(void* vself, void* obj, int n)
{
    sSnd1C88Sys* self = (sSnd1C88Sys*)vself;
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        int flag = 0;
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C11[0] == 2) {
            flag = cSSXAudio_ReplayMode(self) == 0;
        }
        int a = func_002917B8(self, n);
        int b = n * 8 + 4;
        snd1C88AddVoice(self, a, b);
        sSnd1C88Mgr* m3 = *self->p118;
        sSnd1C88Sub* s3 = &m3->sub;
        s3->vals[m3->sub.cur] = 0;
        s3->ch[m3->sub.cur].vol = (s3->vals[s3->cur] < 0x7F && s3->vals[s3->cur] < 0) ? 0 : (s3->vals[s3->cur] < 0x7F ? (unsigned char)s3->vals[s3->cur] : 0x7F);
        sSnd1C88Mgr** pp = self->p118;
        int r = (int)func_00287968(self, 6, 0);
        (*pp)->sub.p3C[(*pp)->sub.cur] = r;
        {
            sSnd1C88Mgr* m2 = *self->p118;
            m2->sub.tgt[m2->sub.cur] = (char*)obj + 0x110;
        }
        {
            sSnd1C88Mgr* m2 = *self->p118;
            sSndKey8_1C88 k = D_004A3690_1C88[0];
            sSnd1C88Sub* s2 = &m2->sub;
            *(sSndKey8_1C88*)((char*)s2 + (m2->sub.cur << 3) + 0x40) = k;
        }
        {
            sSnd1C88Mgr* m2 = *self->p118;
            m2->sub.p60[m2->sub.cur] = (int)obj;
        }
        {
            sSnd1C88Mgr** pq = self->p118;
            int q = func_00288A20(self, obj);
            (*pq)->sub.p64[(*pq)->sub.cur] = q;
        }
        (*self->p118)->sub.p7C[(*self->p118)->sub.cur] = 0;
        func_002A9988(&(*self->p118)->sub, 100.0f, -1.0f);
        if (flag) {
            (*self->p118)->sub.p80[(*self->p118)->sub.cur] = 1;
            (*self->p118)->sub.p84[(*self->p118)->sub.cur] = *(int*)((char*)obj + 0x870);
        }
        cBankSys_SFX_End((cBankSys*)self);
    } else {
        int a = func_002917B8(self, n);
        int b = n * 8 + 6;
        snd1C88AddVoice(self, a, b);
        sSnd1C88Mgr* m3 = *self->p118;
        sSnd1C88Sub* s3 = &m3->sub;
        s3->vals[m3->sub.cur] = 0;
        s3->ch[m3->sub.cur].vol = (s3->vals[s3->cur] < 0x7F && s3->vals[s3->cur] < 0) ? 0 : (s3->vals[s3->cur] < 0x7F ? (unsigned char)s3->vals[s3->cur] : 0x7F);
        sSnd1C88Mgr** pp = self->p118;
        int r = (int)func_00287968(self, 6, 0);
        (*pp)->sub.p3C[(*pp)->sub.cur] = r;
        {
            sSnd1C88Mgr* m2 = *self->p118;
            m2->sub.tgt[m2->sub.cur] = (char*)obj + 0x110;
        }
        {
            sSnd1C88Mgr* m2 = *self->p118;
            sSndKey8_1C88 k = D_004A3698_1C88[0];
            sSnd1C88Sub* s2 = &m2->sub;
            *(sSndKey8_1C88*)((char*)s2 + (m2->sub.cur << 3) + 0x40) = k;
        }
        (*self->p118)->sub.p7C[(*self->p118)->sub.cur] = 0;
        {
            sSnd1C88Mgr* m2 = *self->p118;
            m2->sub.p60[m2->sub.cur] = (int)obj;
        }
        func_002A9988(&(*self->p118)->sub, 100.0f, -1.0f);
        cBankSys_SFX_End((cBankSys*)self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_00292508);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int func_00288A20(void* self, void* obj);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void func_002A9988(void* self, float a, float b);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_002917B8(void* self, int a1);
extern signed char D_00535C11[];

struct sSnd2508Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd2508E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd2508Sub {
    char pad0[4];
    sSnd2508Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd2508Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd2508E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd2508Mgr {
    char pad0[0x1D8];
    sSnd2508Sub sub;         // 0x1D8
};
struct sSnd2508Sys {
    sSnd2508Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd2508Mgr** p118;      // 0x118
};
struct sSndKey8_2508 {
    long v;
} __attribute__((packed));
extern sSndKey8_2508 D_004A36A0_2508[] __asm__("D_004A36A0");

// PORT: 64-bit long sound key
void func_00292508_2508(void* vself, void* obj, int n) __asm__("func_00292508");
void func_00292508_2508(void* vself, void* obj, int n)
{
    sSnd2508Sys* self = (sSnd2508Sys*)vself;
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (!ok) {
        return;
    }
    int flag = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 2) {
        flag = cSSXAudio_ReplayMode(self) == 0;
    }
    int a = func_002917B8(self, n);
    int b = n * 8 + 3;
    sSnd2508Mgr* m = self->mgr;
    sSnd2508Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSnd2508Mgr* m3 = *self->p118;
    sSnd2508Sub* s3 = &m3->sub;
    s3->vals[m3->sub.cur] = 0;
    s3->ch[m3->sub.cur].vol = (s3->vals[s3->cur] < 0x7F && s3->vals[s3->cur] < 0) ? 0 : (s3->vals[s3->cur] < 0x7F ? (unsigned char)s3->vals[s3->cur] : 0x7F);
    sSnd2508Mgr** pp = self->p118;
    int r = (int)func_00287968(self, 6, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    {
        sSnd2508Mgr* m2 = *self->p118;
        m2->sub.tgt[m2->sub.cur] = (char*)obj + 0x110;
    }
    {
        sSnd2508Mgr* m2 = *self->p118;
        sSndKey8_2508 k = D_004A36A0_2508[0];
        sSnd2508Sub* s2 = &m2->sub;
        *(sSndKey8_2508*)((char*)s2 + (m2->sub.cur << 3) + 0x40) = k;
    }
    (*self->p118)->sub.p7C[(*self->p118)->sub.cur] = 0;
    {
        sSnd2508Mgr* m2 = *self->p118;
        m2->sub.p60[m2->sub.cur] = (int)obj;
    }
    {
        sSnd2508Mgr** pq = self->p118;
        int q = func_00288A20(self, obj);
        (*pq)->sub.p64[(*pq)->sub.cur] = q;
    }
    func_002A9988(&(*self->p118)->sub, 100.0f, -1.0f);
    if (flag) {
        (*self->p118)->sub.p80[(*self->p118)->sub.cur] = 1;
        (*self->p118)->sub.p84[(*self->p118)->sub.cur] = *(int*)((char*)obj + 0x870);
    }
    cBankSys_SFX_End((cBankSys*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_BeginAllRiderSound);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getAI();
extern "C" void cSSXAudio_BeginRiderSound(cSSXAudio* self, int id);
extern "C" void func_00292B48(void* self);

/**
 * Starts the board sounds of every rider and resets the wind volume.
 *
 * Calls func_00292B48() first (which also stops the turbo sound), then BeginRiderSound() for each allocated
 * rider.
 */
extern "C" void cSSXAudio_BeginAllRiderSound(cSSXAudio* self)
{
    func_00292B48(self);
    for (int i = 0; i < ((cAI*)cAudioInterface_getAI())->NumberOfRidersAllocated; i++) {
        cSSXAudio_BeginRiderSound(self, (int)((cAI*)cAudioInterface_getAI())->Riders[i]);
    }
    *(int*)((char*)self + 0x5830) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_BeginRiderSound);
#ifdef SKIP_ASM
int cRider_getMotionState(void*);
extern "C" int cSSXAudio_Surface(void* self, int id);
extern "C" void func_002917D0(void* self, int id);
extern "C" void func_00291C88(void* self, int id, int n);
extern "C" void func_00292508(void* self, int id, int n);

/**
 * Starts a rider's board sounds: records its sound state and surface, then starts its glide and carve loops.
 *
 * @param id  the rider (passed as an int)
 */
// PORT: id is a pointer passed as int (the unit declares cSSXAudio_BeginRiderSound(void*, int)).
extern "C" void cSSXAudio_BeginRiderSound(cSSXAudio* self, int id)
{
    int motion = cRider_getMotionState((void*)id);
    int n = cSSXAudio_Surface(self, id);
    *(int*)((char*)id + 0x760) = motion;
    *(int*)((char*)id + 0x764) = motion;
    *(int*)((char*)id + 0x768) = n;
    *(int*)((char*)id + 0x76C) = n;
    func_002917D0(self, id);
    func_00291C88(self, id, n);
    func_00292508(self, id, n);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_StopRiderSound);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sSndId292AE8 {
    long v;
} __attribute__((packed));
extern const sSndId292AE8 D_004A36A8[];
extern "C" void func_002ADDE0(void* bank, long id);

struct sVE292AE8 { short delta; short index; void (*fn)(void*, int); };

struct sBank292AE8 {
    char pad0[0x8D8];
    sVE292AE8* vt;      // 0x8D8
};
struct sSys292AE8 {
    char pad0[0x1D8];
    sBank292AE8 bank;   // 0x1D8
};

/**
 * Stops a rider's sounds through the bank manager (virtual slot 4), then calls func_002ADDE0() with stream
 * key
 * D_004A36A8.
 *
 * @param Rider  the rider (On Tour cRider* Rider)
 */
// Notifies the sound bank, then (re)starts the default ambient stream.
// PORT: the 8-byte stream id is passed by value from an unaligned global (64-bit `long`).
extern "C" void cSSXAudio_StopRiderSound(cSSXAudio* self, int Rider)
{
    sSys292AE8* sys = *(sSys292AE8**)self->MusicSys.mpVStreamSys;
    sVE292AE8* vt = sys->bank.vt;
    sys = (sSys292AE8*)&sys->bank;
    vt[4].fn((char*)sys + vt[4].delta, Rider);
    func_002ADDE0(&(*(sSys292AE8**)self->MusicSys.mpVStreamSys)->bank, D_004A36A8[0].v);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00292B48);
#ifdef SKIP_ASM
struct sSndKey8B48 {
    long v;
} __attribute__((packed));
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
extern const sSndKey8B48 D_004A3688[];
extern const sSndKey8B48 D_004A3690[];
extern const sSndKey8B48 D_004A3698[];
extern const sSndKey8B48 D_004A36A0[];
// View (func_00299638 declares D_004A36A8 with the unit's sSndKey8 type).
extern const sSndKey8B48 D_004A36A8_b48[] __asm__("D_004A36A8");
extern const sSndKey8B48 D_004A36B0[];
extern const sSndKey8B48 D_004A36B8[];
extern "C" void cSSXAudio_EndWind(cSSXAudio* self);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);
extern "C" void func_002ADDE0(void* p, long key);

static inline void sndKeyCall(void* self, long key)
{
    (**(cSndObj292B48***)((char*)self + 0x118))->v03(key);
}

// PORT: 64-bit long sound keys
extern "C" void func_00292B48(void* self)
{
    cSSXAudio_EndWind((cSSXAudio*)self);
    sndKeyCall(self, D_004A3688[0].v);
    sndKeyCall(self, D_004A3690[0].v);
    sndKeyCall(self, D_004A3698[0].v);
    sndKeyCall(self, D_004A36A0[0].v);
    func_002ADDE0(**(char***)((char*)self + 0x118) + 0x1D8, D_004A36A8_b48[0].v);
    if (*(int*)((char*)self + 0x59E4) > 0) {
        cBankMonitor_BANKMONITOR_StopHandle(**(char***)((char*)self + 0x118) + 0x1D8, *(int*)((char*)self + 0x59E0), 1, 0.0f);
        *(int*)((char*)self + 0x59E4) = 0;
    }
    sndKeyCall(self, D_004A36B0[0].v);
    sndKeyCall(self, D_004A36B8[0].v);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00292CB8);
#ifdef SKIP_ASM
extern "C" void func_00293168(void*, void*, int*, int*);
extern "C" void cBankMonitorInstance_PitchUpdate(void*, int);
extern "C" void* cAudioInterface_getGame();
extern "C" int cReplay_isPauseAI(void* p);
extern int cPauseState_Flags;

extern "C" void func_00292CB8(void* self, void* obj) {
    int volume;
    int b;
    cRider* rider = *(cRider**)((char*)obj + 0x80);
    b = 0x40;
    volume = 0;
    if (cRider_getMotionState(rider) == 0) {
        func_00293168(self, rider, &volume, &b);
    }
    if (cRider_getMotionState(rider) == 2) {
        volume = 0;
    }
    int pos = rider->mFinishTime >= 0.0f;
    if (pos) {
        volume = 0;
    }
    int paused = 0;
    if ((cPauseState_Flags & 1) || cReplay_isPauseAI(*(void**)((char*)cAudioInterface_getGame() + 0x28)) != 0) {
        paused = 1;
    }
    if (paused) {
        volume = 0;
    }
    *(int*)((char*)obj + 0x64) = volume;
    cBankMonitorInstance_PitchUpdate(obj, b);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleGlide);
#ifdef SKIP_ASM
extern "C" int func_002934D0(void* self, int id, void* rider);
extern "C" void cSSXAudio_GlideProgram(cSSXAudio* self, int id, void* rider, int* out, int* size);
extern "C" void cBankMonitorInstance_Reset(void*);
extern "C" void cBankMonitorInstance_PitchUpdate(void*, int);
extern "C" void* cAudioInterface_getGame();
extern "C" int cReplay_isPauseAI(void* p);
extern int cPauseState_Flags;

/**
 * Updates a human rider's glide loop for one frame: restarts it on a surface change, else sets its volume and
 * bend.
 *
 * Called for a bank-monitor instance whose rider is at +0x80. In sound states 1 and 5 only a type-2 surface
 * is
 * switched to 9; state 2, cPauseState_Flags bit 0 and a playing replay (cReplay_isPauseAI) silence the loop. The volume
 * goes
 * to the instance's +0x64 and the bend through cBankMonitorInstance_PitchUpdate().
 *
 * @param obj  the bank-monitor instance (On Tour cBankMonitorInstance* Inst)
 */
// PORT: the rider pointer is passed to cSSXAudio_Surface/func_00291C88 as an int (the unit declares them that way).
extern "C" void cSSXAudio_HandleGlide(cSSXAudio* self, void* obj) {
    int volume;
    int bend;
    cRider* rider = *(cRider**)((char*)obj + 0x80);
    int st = cRider_getMotionState(rider);
    if (st != rider->unk_760) {
        rider->unk_760 = st;
    }
    volume = 0;
    bend = 0x40;
    if (st == 1 || st == 5) {
        if (func_002934D0(self, rider->unk_768, rider) == 2) {
            rider->unk_768 = 9;
        }
    } else {
        int id = cSSXAudio_Surface(self, (int)rider);
        if (id != rider->unk_768) {
            rider->unk_768 = id;
            cBankMonitorInstance_Reset(obj);
            func_00291C88(self, (int)rider, id);
        } else {
            cSSXAudio_GlideProgram(self, cSSXAudio_Surface(self, (int)rider), rider, &volume, &bend);
        }
    }
    if (cRider_getMotionState(rider) == 2) {
        volume = 0;
    }
    int paused = 0;
    if ((cPauseState_Flags & 1) || cReplay_isPauseAI(*(void**)((char*)cAudioInterface_getGame() + 0x28)) != 0) {
        paused = 1;
    }
    if (paused) {
        volume = 0;
    }
    *(int*)((char*)obj + 0x64) = volume;
    cBankMonitorInstance_PitchUpdate(obj, bend);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleAIGlide);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_AIGlideProgram(cSSXAudio* self, int id, void* rider, int* out, int* size);
extern "C" void cBankMonitorInstance_Reset(void*);
extern "C" void cBankMonitorInstance_PitchUpdate(void*, int);
extern "C" void* cAudioInterface_getGame();
extern "C" int cReplay_isPauseAI(void* p);
extern int cPauseState_Flags;

/**
 * Updates a computer rider's glide loop for one frame, like HandleGlide() but only in sound states 0 and 4.
 *
 * @param obj  the bank-monitor instance (On Tour cBankMonitorInstance* Inst)
 */
// PORT: the rider pointer is passed to cSSXAudio_Surface/func_00291C88 as an int (the unit declares them that way).
extern "C" void cSSXAudio_HandleAIGlide(cSSXAudio* self, void* obj) {
    int volume;
    int bend;
    cRider* rider = *(cRider**)((char*)obj + 0x80);
    int st = cRider_getMotionState(rider);
    if (st != rider->unk_760) {
        rider->unk_760 = st;
    }
    volume = 0;
    bend = 0x40;
    if (st == 0 || st == 4) {
        int id = cSSXAudio_Surface(self, (int)rider);
        if (id != rider->unk_768) {
            rider->unk_768 = id;
            cBankMonitorInstance_Reset(obj);
            func_00291C88(self, (int)rider, cSSXAudio_Surface(self, (int)rider));
        }
        cSSXAudio_AIGlideProgram(self, cSSXAudio_Surface(self, (int)rider), rider, &volume, &bend);
    }
    int paused = 0;
    if ((cPauseState_Flags & 1) || cReplay_isPauseAI(*(void**)((char*)cAudioInterface_getGame() + 0x28)) != 0) {
        paused = 1;
    }
    if (paused) {
        volume = 0;
    }
    *(int*)((char*)obj + 0x64) = volume;
    cBankMonitorInstance_PitchUpdate(obj, bend);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandleCarve);
#ifdef SKIP_ASM
extern "C" int func_002934D0(void* self, int id, void* rider);
extern "C" void cSSXAudio_CarveProgram(cSSXAudio* self, int id, void* rider, int* out, int* size);
extern "C" void cBankMonitorInstance_Reset(void*);
extern "C" void cBankMonitorInstance_PitchUpdate(void*, int);
extern "C" void* cAudioInterface_getGame();
extern "C" int cReplay_isPauseAI(void* p);
extern int cPauseState_Flags;

/**
 * Updates a rider's carve loop for one frame: restarts it on a surface change, else sets its volume and bend.
 *
 * Same structure as HandleGlide(), with the carve surface kept in the rider's +0x76C and CarveProgram().
 *
 * @param obj  the bank-monitor instance (On Tour cBankMonitorInstance* Inst)
 */
// PORT: the rider pointer is passed to cSSXAudio_Surface/func_00292508 as an int (the unit declares them that way).
extern "C" void cSSXAudio_HandleCarve(cSSXAudio* self, void* obj) {
    int volume;
    int bend;
    cRider* rider = *(cRider**)((char*)obj + 0x80);
    bend = 0x40;
    volume = 0;
    int st = cRider_getMotionState(rider);
    if (st == 1 || st == 5) {
        if (func_002934D0(self, rider->unk_76C, rider) == 2) {
            rider->unk_76C = 9;
        }
    } else {
        int id = cSSXAudio_Surface(self, (int)rider);
        if (id != rider->unk_76C) {
            rider->unk_76C = id;
            cBankMonitorInstance_Reset(obj);
            func_00292508(self, (int)rider, cSSXAudio_Surface(self, (int)rider));
        } else {
            cSSXAudio_CarveProgram(self, cSSXAudio_Surface(self, (int)rider), rider, &volume, &bend);
        }
    }
    if (cRider_getMotionState(rider) == 2) {
        volume = 0;
    }
    int paused = 0;
    if ((cPauseState_Flags & 1) || cReplay_isPauseAI(*(void**)((char*)cAudioInterface_getGame() + 0x28)) != 0) {
        paused = 1;
    }
    if (paused) {
        volume = 0;
    }
    *(int*)((char*)obj + 0x64) = volume;
    cBankMonitorInstance_PitchUpdate(obj, bend);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_00293168);
#ifdef SKIP_ASM
extern func_00290B58_sCurve D_00482AF8;
extern func_00290B58_sCurve D_00482B20;

extern "C" void func_00293168(void* self, void* vrider, int* out1, int* out2) {
    cRider* rider = (cRider*)vrider;
    float spd = rider->mAudioSpeed;
    func_00290B58_sCurve c1 = D_00482AF8;
    func_00290B58_sCurve c2 = D_00482B20;
    *out1 = (int)func_00290B58(&c1, spd);
    *out2 = (int)(func_00290B58(&c2, spd) * 4096.0f);
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_GlideProgram);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_002934D0(void* self, int id, void* rider);
void snd_dec_a(void*, void*, int*, int*) __asm__("func_00293538");
void snd_dec_b(void*, void*, int*, int*) __asm__("func_00293740");
void snd_dec_c(void*, void*, int*, int*) __asm__("func_00293948");

/**
 * Computes a human rider's glide-loop volume and pitch bend for a surface.
 *
 * Starts from volume 0 and bend 0x1000, then runs the program for the surface's type (func_002934D0: 0, 1 or
 * 2).
 *
 * @param id  surface (On Tour Surface)
 * @param rider  the rider
 * @param out  receives the volume
 * @param size  receives the pitch bend (0x1000 is no bend)
 */
extern "C" void cSSXAudio_GlideProgram(cSSXAudio* self, int id, void* rider, int* out, int* size) {
    *out = 0;
    *size = 0x1000;
    int programType = func_002934D0(self, id, rider);
    if (programType == 0) {
        snd_dec_a(self, rider, out, size);
    } else if (programType == 1) {
        snd_dec_b(self, rider, out, size);
    } else if (programType == 2) {
        snd_dec_c(self, rider, out, size);
    }
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_AIGlideProgram);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_002934D0(void* self, int id, void* rider);
void snd_dec_d(void*, void*, int*, int*) __asm__("func_00293B50");
void snd_dec_e(void*, void*, int*, int*) __asm__("func_00293B70");
void snd_dec_f(void*, void*, int*, int*) __asm__("func_00293B90");

/**
 * Computes a computer rider's glide-loop volume and pitch bend for a surface, like GlideProgram().
 *
 * @param id  surface (On Tour Surface)
 * @param rider  the rider
 * @param out  receives the volume
 * @param size  receives the pitch bend (0x1000 is no bend)
 */
extern "C" void cSSXAudio_AIGlideProgram(cSSXAudio* self, int id, void* rider, int* out, int* size) {
    *out = 0;
    *size = 0x1000;
    int programType = func_002934D0(self, id, rider);
    if (programType == 0) {
        snd_dec_d(self, rider, out, size);
    } else if (programType == 1) {
        snd_dec_e(self, rider, out, size);
    } else if (programType == 2) {
        snd_dec_f(self, rider, out, size);
    }
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_CarveProgram);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_002934D0(void* self, int id, void* rider);
void snd_dec_g(void*, void*, int*, int*) __asm__("func_00293BB0");
void snd_dec_h(void*, void*, int*, int*) __asm__("cSSXAudio_SoftCarveProgram");
void snd_dec_i(void*, void*, int*, int*) __asm__("func_00293FC8");

/**
 * Computes a rider's carve-loop volume and pitch bend for a surface, like GlideProgram().
 *
 * @param id  surface (On Tour Surface)
 * @param rider  the rider
 * @param out  receives the volume
 * @param size  receives the pitch bend (0x1000 is no bend)
 */
extern "C" void cSSXAudio_CarveProgram(cSSXAudio* self, int id, void* rider, int* out, int* size) {
    *out = 0;
    *size = 0x1000;
    int programType = func_002934D0(self, id, rider);
    if (programType == 0) {
        snd_dec_g(self, rider, out, size);
    } else if (programType == 1) {
        snd_dec_h(self, rider, out, size);
    } else if (programType == 2) {
        snd_dec_i(self, rider, out, size);
    }
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", func_002934D0);
#ifdef SKIP_ASM
extern "C" int func_002934D0(void *self, int kind, void *rider)
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
INCLUDE_ASM("sound/soundsys", func_00293538);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sSsVt_3538 { short delta; short index; int (*fn)(void*); };
extern const func_00290B58_sCurve D_00482B78;
extern const func_00290B58_sCurve D_00482BA0;

static inline float* getLevels_3538(void* self) { return (float*)((char*)self + 0x59B8); }

void func_00293538_impl(void* self, cRider* rider, int* volume, int* pitch) __asm__("func_00293538");
void func_00293538_impl(void* self, cRider* rider, int* volume, int* pitch)
{
    float level = rider->mAudioPressure;
    level *= 1.0f - (rider->mAudioEdging + rider->mAudioBraking);
    float speed = rider->mAudioSpeed;
    if (speed < 0.1f) {
        level = level * speed * 10.0f;
    }
    char* obj = (char*)&rider->mVtbl;
    sSsVt_3538* vt = *(sSsVt_3538**)obj;
    int idx = vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta);
    float prev = *(float*)((char*)getLevels_3538(self) + (idx << 2));
    float d = prev - level;
    if (__builtin_fabsf(d) > 0.1f) {
        if (level < prev) {
            level = prev - d * 0.25f;
        } else if (prev < level) {
            level = prev + (level - prev) * 0.25f;
        }
    }
    func_00290B58_sCurve c1 = D_00482B78;
    *(float*)((char*)getLevels_3538(self) + (idx << 2)) = level;
    func_00290B58_sCurve c2 = D_00482BA0;
    *volume = (int)func_00290B58(&c1, level);
    *pitch = (int)(func_00290B58(&c2, level) * 4096.0f);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_00293740);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sSsVt_3740 { short delta; short index; int (*fn)(void*); };
extern const func_00290B58_sCurve D_00482BC8;
extern const func_00290B58_sCurve D_00482BF0;

static inline float* getLevels_3740(void* self) { return (float*)((char*)self + 0x59B8); }

void func_00293740_impl(void* self, cRider* rider, int* volume, int* pitch) __asm__("func_00293740");
void func_00293740_impl(void* self, cRider* rider, int* volume, int* pitch)
{
    float level = rider->mAudioPressure;
    level *= 1.0f - (rider->mAudioEdging + rider->mAudioBraking);
    float speed = rider->mAudioSpeed;
    if (speed < 0.1f) {
        level = level * speed * 10.0f;
    }
    char* obj = (char*)&rider->mVtbl;
    sSsVt_3740* vt = *(sSsVt_3740**)obj;
    int idx = vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta);
    float prev = *(float*)((char*)getLevels_3740(self) + (idx << 2));
    float d = prev - level;
    if (__builtin_fabsf(d) > 0.1f) {
        if (level < prev) {
            level = prev - d * 0.25f;
        } else if (prev < level) {
            level = prev + (level - prev) * 0.25f;
        }
    }
    func_00290B58_sCurve c1 = D_00482BC8;
    *(float*)((char*)getLevels_3740(self) + (idx << 2)) = level;
    func_00290B58_sCurve c2 = D_00482BF0;
    *volume = (int)func_00290B58(&c1, level);
    *pitch = (int)(func_00290B58(&c2, level) * 4096.0f);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_00293948);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sSsVt_3948 { short delta; short index; int (*fn)(void*); };
extern const func_00290B58_sCurve D_00482C18;
extern const func_00290B58_sCurve D_00482C40;

static inline float* getLevels_3948(void* self) { return (float*)((char*)self + 0x59B8); }

void func_00293948_impl(void* self, cRider* rider, int* volume, int* pitch) __asm__("func_00293948");
void func_00293948_impl(void* self, cRider* rider, int* volume, int* pitch)
{
    float level = rider->mAudioPressure;
    level *= 1.0f - (rider->mAudioEdging + rider->mAudioBraking);
    float speed = rider->mAudioSpeed;
    if (speed < 0.1f) {
        level = level * speed * 10.0f;
    }
    char* obj = (char*)&rider->mVtbl;
    sSsVt_3948* vt = *(sSsVt_3948**)obj;
    int idx = vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta);
    float prev = *(float*)((char*)getLevels_3948(self) + (idx << 2));
    float d = prev - level;
    if (__builtin_fabsf(d) > 0.1f) {
        if (level < prev) {
            level = prev - d * 0.25f;
        } else if (prev < level) {
            level = prev + (level - prev) * 0.25f;
        }
    }
    func_00290B58_sCurve c1 = D_00482C18;
    *(float*)((char*)getLevels_3948(self) + (idx << 2)) = level;
    func_00290B58_sCurve c2 = D_00482C40;
    *volume = (int)func_00290B58(&c1, level);
    *pitch = (int)(func_00290B58(&c2, level) * 4096.0f);
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", func_00293B50);
#ifdef SKIP_ASM
extern "C" void func_00293538();

extern "C" void func_00293B50(void) {
    func_00293538();
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", func_00293B70);
#ifdef SKIP_ASM
extern "C" void func_00293740();

extern "C" void func_00293B70(void) {
    func_00293740();
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", func_00293B90);
#ifdef SKIP_ASM
extern "C" void func_00293948();

extern "C" void func_00293B90(void) {
    func_00293948();
}
#endif
//100%
INCLUDE_ASM("sound/soundsys", func_00293BB0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable entry (this-adjust delta, index, function) for a method taking only self, returning int.
struct sSndVtI { short delta; short index; int (*fn)(void*); };

static inline float* sndLevels5990(void* self) { return (float*)((char*)self + 0x5990); }
extern const func_00290B58_sCurve D_00482C68;
extern const func_00290B58_sCurve D_00482C90;

extern "C" void func_00293BB0(void* self, cRider* rider, int* volume, int* pitch)
{
    float pressure = rider->mAudioPressure;
    float level = rider->mAudioEdging + rider->mAudioBraking;
    char* obj = (char*)&rider->mVtbl;
    sSndVtI* vt = *(sSndVtI**)obj;
    int idx = vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta);
    float prev = *(float*)((char*)sndLevels5990(self) + (idx << 2));
    float d = prev - level;
    if (__builtin_fabsf(d) > 0.1f) {
        if (level < prev) {
            level = prev - d * 0.5f;
        } else if (prev < level) {
            level = prev + (level - prev) * 0.125f;
        }
    }
    func_00290B58_sCurve c1 = D_00482C68;
    *(float*)((char*)sndLevels5990(self) + (idx << 2)) = level;
    func_00290B58_sCurve c2 = D_00482C90;
    *volume = (int)func_00290B58(&c1, level);
    *pitch = (int)(func_00290B58(&c2, pressure) * 4096.0f);
    float speed = rider->mAudioSpeed;
    if (speed < 0.1f) {
        *volume = (int)((float)*volume * speed * 10.0f);
    }
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_SoftCarveProgram);
#ifdef SKIP_ASM
struct sSsVt_3DC0 { short delta; short index; int (*fn)(void*); };
extern const func_00290B58_sCurve D_00482CB8;
extern const func_00290B58_sCurve D_00482CE0;

static inline float* getLevels_3DC0(void* self) { return (float*)((char*)self + 0x5990); }

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
extern "C" void cSSXAudio_SoftCarveProgram(cSSXAudio* self, cRider* rider, int* volume, int* pitch)
{
    float level = rider->mAudioEdging + rider->mAudioBraking;
    float speed = rider->mAudioSpeed;
    if (speed < 0.1f) {
        level = level * speed * 10.0f;
    }
    float pressure = rider->mAudioPressure;
    char* obj = (char*)&rider->mVtbl;
    sSsVt_3DC0* vt = *(sSsVt_3DC0**)obj;
    int idx = vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta);
    float prev = *(float*)((char*)getLevels_3DC0(self) + (idx << 2));
    float diff = prev - level;
    if (__builtin_fabsf(diff) > 0.1f) {
        if (level < prev) {
            level = prev - diff * 0.5f;
        } else if (prev < level) {
            level = prev + (level - prev) * 0.125f;
        }
    }
    func_00290B58_sCurve volCurve = D_00482CB8;
    *(float*)((char*)getLevels_3DC0(self) + (idx << 2)) = level;
    func_00290B58_sCurve bendCurve = D_00482CE0;
    *volume = (int)func_00290B58(&volCurve, level);
    *pitch = (int)(func_00290B58(&bendCurve, pressure) * 4096.0f);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_00293FC8);
#ifdef SKIP_ASM
struct sSsVt_3FC8 { short delta; short index; int (*fn)(void*); };
extern const func_00290B58_sCurve D_00482D08;

extern "C" void func_00293FC8(void* self, cRider* rider, int* volume, int* pitch)
{
    float pressure = rider->mAudioPressure;
    float level = rider->mAudioEdging + rider->mAudioBraking;
    char* obj = (char*)&rider->mVtbl;
    sSsVt_3FC8* vt = *(sSsVt_3FC8**)obj;
    int idx = vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta);
    float prev = *(float*)((char*)self + (idx << 2) + 0x5990);
    float d = prev - level;
    if (__builtin_fabsf(d) > 0.1f) {
        if (level < prev) {
            level = prev - d * 0.5f;
        } else if (prev < level) {
            level = prev + (level - prev) * 0.125f;
        }
    }
    *(float*)((char*)self + (idx << 2) + 0x5990) = level;
    *volume = 0x7F;
    func_00290B58_sCurve c = D_00482D08;
    *pitch = (int)(func_00290B58(&c, pressure) * 4096.0f);
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
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int func_00288A20(void* self, void* obj);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cSSXAudio_GetFocusRider(void* self, int which);
extern "C" int cSSXAudio_Surface(void* self, int id);
void cSSXAudio_LiftWithAirTime_4170(void* self, char* obj) __asm__("cSSXAudio_LiftWithAirTime");
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_003B58A0();
extern "C" int func_003B8AB8(int id);
extern "C" void func_003B58D8();
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);
extern signed char D_00535C11[];

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
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    int r = (int)func_00287968(self, 6, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    {
        sSnd4170Mgr* m2 = *self->p118;
        m2->sub.tgt[m2->sub.cur] = (char*)obj + 0x110;
    }
    {
        sSnd4170Mgr** pq = self->p118;
        int q = func_00288A20(self, obj);
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
extern "C" void cSSXAudio_Lift(cSSXAudio* self, void* obj)
{
    int idx = -1;
    if (cSSXAudio_ReplayMode(self) != 0) {
        idx = 0;
    } else {
        bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
        if (ok) {
            idx = *(int*)((char*)obj + 0x870);
        }
    }
    if (cSSXAudio_GetFocusRider(self, idx) == (int)obj) {
        int volume = clampVol_4170((int)(*(float*)((char*)obj + 0x75C) * 127.0f), 0x40, 0x7F);
        func_003B58A0();
        int soundFinished = func_003B8AB8(self->mCharLiftHandle) != 0;
        func_003B58D8();
        if (!soundFinished) {
            cBankMonitor_BANKMONITOR_StopHandle(*(char**)self->MusicSys.mpVStreamSys + 0x1D8, self->mCharLiftHandle, 1, 0.0f);
        }
        int flag = 0;
        bool ok2 = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
        if (ok2) {
            cBE_getInterface_Fv(cBE_getBE(), 0);
            if (D_00535C11[0] == 2) {
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
extern "C" void func_00294880(void* self, int i, int v);
extern "C" void func_0029FCC8(cSSXAudio* self, void* rider);
extern "C" void func_002A7408(cCrowdSys* self, int i);
extern "C" int cSSXAudio_ReplayMode(void* self);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");

struct sSsVt4678 { short delta; short index; int (*fn)(void*); };

/**
 * Handles the air-time side of a lift-off, from the rider's air predictor.
 *
 * Unless the predictor is in state 1 or 3, it only calls func_00294880(.., 1). Otherwise it calls
 * func_00294880(.., 0) and func_0029FCC8(); for the listener's rider with more than 2 s of predicted air time
 * it
 * also calls func_002A7408(), probably a crowd reaction.
 *
 * @param rider  the rider
 */
extern "C" void cSSXAudio_LiftWithAirTime(cSSXAudio* self, char* rider)
{
    int simState = *(int*)(*(char**)(rider + 0x788) + 0xAC);
    int ok = simState == 1 || simState == 3;
    if (!ok) {
        char* riderObj = rider + 0x6C0;
        sSsVt4678* vtEntry = &(*(sSsVt4678**)riderObj)[7];
        func_00294880(self, vtEntry->fn(riderObj + vtEntry->delta), 1);
        return;
    }
    int idx = -1;
    {
        char* riderObj = rider + 0x6C0;
        sSsVt4678* vtEntry = &(*(sSsVt4678**)riderObj)[7];
        func_00294880(self, vtEntry->fn(riderObj + vtEntry->delta), 0);
    }
    float x = *(float*)(*(char**)(rider + 0x788) + 0x98);
    func_0029FCC8(self, rider);
    if (cSSXAudio_ReplayMode(self) != 0) {
        idx = 0;
    } else {
        int ok2 = *(int*)(rider + 0x874) != 0 && *(int*)(rider + 0x87C) != 0;
        if (ok2) {
            idx = *(int*)(rider + 0x870);
        }
    }
    if (cSSXAudio_GetFocusRider_p(self, idx) == rider && x > 2.0f) {
        func_002A7408((cCrowdSys*)self, idx);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UpdateLift);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00294880(void* self, int i, int v);
extern "C" void func_0029FCC8(cSSXAudio* self, void* rider);
extern "C" void func_002A7408(cCrowdSys* self, int i);
extern "C" int cSSXAudio_ReplayMode(void* self);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");



// Six rider slots at 0x59E8.
struct sSnd4860 {
    char pad[0x59E8];
    int vals[6];
};

extern "C" void cSSXAudio_LiftWithAirTime(cSSXAudio* self, char* rider);

/**
 * Runs LiftWithAirTime() for each rider flagged in mNeedLift, clearing the flags of riders that do not exist.
 */
// Refresh the six rider slots at 0x59E8, clearing those whose rider is gone.
extern "C" void cSSXAudio_UpdateLift(cSSXAudio* self)
{
    sSnd4860* audio = (sSnd4860*)self;
    for (int i = 0; i < 6; i++) {
        if (audio->vals[i] != 0 && i < ((cAI*)cAudioInterface_getAI())->NumberOfRidersAllocated) {
            char* rider = (char*)((cAI*)cAudioInterface_getAI())->Riders[i];
            if (rider != 0) {
                cSSXAudio_LiftWithAirTime(self, rider);
                continue;
            }
        }
        audio->vals[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00294860);
#ifdef SKIP_ASM
extern "C" int func_00294860(void* self, int i)
{
    if (i < 6) {
        return *(int*)((char*)self + (i << 2) + 0x59e8);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00294880);
#ifdef SKIP_ASM
extern "C" void func_00294880(void* self, int i, int needLift)
{
    if (i < 6) {
        *(int*)((char*)self + (i << 2) + 0x59e8) = needLift;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_ResetLift);
#ifdef SKIP_ASM
/**
 * Clears the six mNeedLift flags.
 */
extern "C" void cSSXAudio_ResetLift(cSSXAudio* self)
{
    int i;
    for (i = 5; i >= 0; i--) {
        *(int*)((char*)self + (i << 2) + 0x59e8) = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_Land);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cSSXAudio_GetFocusRider(void* self, int which);
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
extern "C" void func_002A7438(cCrowdSys* self, int i);
extern "C" void func_002A73A8(cCrowdSys* self, int level, int i);
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B8AB8(int id);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);
// cSSXAudio_Surface takes the rider here (the unit declares (void*, int)).
int cSSXAudio_Surface_p(void* self, cRider* rider) __asm__("cSSXAudio_Surface");
extern const func_00290B58_sCurve D_00482D30;

struct sSsVt48D0 { short delta; short index; int (*fn)(void*); };

/**
 * Plays a rider's landing sound and the crowd's reaction to it.
 *
 * When the rider is the focus rider of its view (cSSXAudio_GetFocusRider with view 0 when replaying, mViewIndex for a
 * local human, else -1) the crowd reacts through func_002A7438 (nothing gained) or func_002A73A8 at level 0, 1 or 2
 * (gain below 0.25, below 0.65, above). Computer and non-local riders farther than 100 units from the camera
 * (cSSXAudio_CameraInRange) play nothing more. The volume comes from the landing curve D_00482D30 (func_00290B58)
 * applied to the rider's +0x770, clamped to 0..127.
 *
 * A previous landing still playing (handle at +0x5FB8) is stopped. Then a play-option level is pushed with defaults
 * (the inlined SFX_Begin) for bank 10 and patch cSSXAudio_Surface's surface class * 8 + 1 for a local human, + 5
 * otherwise; the level gets the volume, the volume-control pointer
 * func_00287968(this, 6, 0), a 3D distance of 30 (func_002A9988), the rider's mPosition to follow, the listening view
 * from func_00288A20, and, for a local human when the byte at D_00535C11 is 2 and not replaying, its own viewport.
 * cBankSys_SFX_End starts it and the new handle is stored at +0x5FB8.
 *
 * @param vself          the audio system (cSSXAudio, this)
 * @param rider          the rider that landed (On Tour Rider)
 * @param powerupGained  power gained by the landing (On Tour PowerupGained); drives the crowd reaction
 */
extern "C" void cSSXAudio_Land(cSSXAudio* vself, cRider* rider, float powerupGained)
{
    sSnd17D0Sys* self = (sSnd17D0Sys*)vself;
    int idx = -1;
    if (cSSXAudio_ReplayMode(self) != 0) {
        idx = 0;
    } else {
        int ok = rider->mIsHuman != 0 && rider->mIsLocal != 0;
        if (ok) {
            idx = rider->mViewIndex;
        }
    }
    if ((cRider*)cSSXAudio_GetFocusRider(self, idx) == rider) {
        if (powerupGained <= 0.0f) {
            func_002A7438((cCrowdSys*)self, idx);
        } else if (powerupGained < 0.25f) {
            func_002A73A8((cCrowdSys*)self, 0, idx);
        } else if (powerupGained < 0.65f) {
            func_002A73A8((cCrowdSys*)self, 1, idx);
        } else {
            func_002A73A8((cCrowdSys*)self, 2, idx);
        }
    }
    char* obj = (char*)&rider->mVtbl;
    sSsVt48D0* vt = *(sSsVt48D0**)obj;
    if (vt[kRiderVt_isComputer].fn(obj + vt[kRiderVt_isComputer].delta) != 0 || rider->mIsLocal == 0) {
        sSsVt48D0* vt2 = *(sSsVt48D0**)obj;
        if (cSSXAudio_CameraInRange(self, vt2[kRiderVt_getPosition].fn(obj + vt2[kRiderVt_getPosition].delta), 100.0f) < 0) {
            return;
        }
    }
    func_00290B58_sCurve landCurve = D_00482D30;
    int vol = (int)func_00290B58(&landCurve, rider->unk_770);
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
    func_003B58A0();
    int soundFinished = func_003B8AB8(*(int*)((char*)self + 0x5FB8)) != 0;
    func_003B58D8();
    if (!soundFinished) {
        cBankMonitor_BANKMONITOR_StopHandle(&(*self->p118)->sub, *(int*)((char*)self + 0x5FB8), 1, 0.0f);
    }
    int flag = 0;
    int ok3 = rider->mIsHuman != 0 && rider->mIsLocal != 0;
    if (ok3) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C11[0] == 2) {
            flag = cSSXAudio_ReplayMode(self) == 0;
        }
    }
    int bank = 10;
    sSnd17D0Mgr* bankMgr = self->mgr;
    sSnd17D0Sub* monitor = &bankMgr->sub;
    bankMgr->sub.cur++;
    monitor->ch[bankMgr->sub.cur] = bankMgr->sub.def;
    bankMgr->sub.tgt[bankMgr->sub.cur] = 0;
    monitor->p34[bankMgr->sub.cur] = 0;
    monitor->vals[bankMgr->sub.cur] = monitor->ch[bankMgr->sub.cur].vol;
    monitor->p3C[bankMgr->sub.cur] = 0;
    func_00416210((char*)monitor + bankMgr->sub.cur * 8 + 0x40, 0, 8);
    monitor->p60[bankMgr->sub.cur] = 0;
    monitor->p64[bankMgr->sub.cur] = 0;
    monitor->p68[bankMgr->sub.cur] = 0;
    monitor->p6C[bankMgr->sub.cur] = 0;
    monitor->p70[bankMgr->sub.cur].f4 = 0;
    monitor->p70[bankMgr->sub.cur].f0 = 100;
    monitor->p70[bankMgr->sub.cur].f8 = 90;
    monitor->p70[bankMgr->sub.cur].fC = 50;
    monitor->p74[bankMgr->sub.cur] = 0;
    monitor->p80[bankMgr->sub.cur] = 0;
    monitor->p84[bankMgr->sub.cur] = 0;
    monitor->p78[bankMgr->sub.cur] = 0x7F;
    monitor->p7C[bankMgr->sub.cur] = 1;
    self->a4[self->mgr->sub.cur] = bank;
    self->a8[self->mgr->sub.cur] = patchNum;
    sSnd17D0Mgr* bankMgrVol = *self->p118;
    sSnd17D0Sub* volMonitor = &bankMgrVol->sub;
    volMonitor->vals[bankMgrVol->sub.cur] = vol;
    volMonitor->ch[bankMgrVol->sub.cur].vol = (volMonitor->vals[volMonitor->cur] < 0x7F && volMonitor->vals[volMonitor->cur] < 0) ? 0 : (volMonitor->vals[volMonitor->cur] < 0x7F ? (unsigned char)volMonitor->vals[volMonitor->cur] : 0x7F);
    sSnd17D0Mgr** pp = self->p118;
    int volumePtr = (int)func_00287968(self, 6, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = volumePtr;
    func_002A9988(&(*self->p118)->sub, 30.0f, -1.0f);
    {
        sSnd17D0Mgr* bankMgrPos = *self->p118;
        bankMgrPos->sub.tgt[bankMgrPos->sub.cur] = &rider->mPosition;
    }
    {
        sSnd17D0Mgr** pq = self->p118;
        int viewport = func_00288A20(self, rider);
        (*pq)->sub.p64[(*pq)->sub.cur] = viewport;
    }
    if (flag) {
        (*self->p118)->sub.p80[(*self->p118)->sub.cur] = 1;
        (*self->p118)->sub.p84[(*self->p118)->sub.cur] = rider->mViewIndex;
    }
    *(int*)((char*)self + 0x5FB8) = cBankSys_SFX_End((cBankSys*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00294F48);
#ifdef SKIP_ASM
struct sSndVEntry294F48 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

extern "C" int func_00294F48(void* self)
{
    sSndVEntry294F48* vt = *(sSndVEntry294F48**)((char*)self + 0xC);
    return vt[1].fn((char*)self + vt[1].delta, 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_MenuSound);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVEnt_294F78 { short delta; short index; int (*fn)(void*, int, int); };

/**
 * Plays a front-end menu sound.
 *
 * The type maps to a patch: 7 -> 0x10, 8 -> 0x11, 0/9 -> 3, 1/10 -> 4, 2/11 -> 2, 3/12 -> 1, 4/13 -> 0xD, 14 -> 0x12,
 * 15 -> 7; 5, 6 and unknown types play nothing. The patch is started through virtual slot 1 of the object at this+0xC
 * with (0, patch).
 *
 * @param cue  the menu sound type (On Tour eMenuSoundType type)
 */
extern "C" void cSSXAudio_MenuSound(void* self, int cue)
{
    int id = -1;
    switch (cue) {
    case 5: case 6:
        id = -1;
        break;
    case 7:
        id = 0x10;
        break;
    case 8:
        id = 17;
        break;
    case 0: case 9:
        id = 3;
        break;
    case 1: case 10:
        id = 4;
        break;
    case 2: case 11:
        id = 2;
        break;
    case 3: case 12:
        id = 1;
        break;
    case 4: case 13:
        id = 13;
        break;
    case 14:
        id = 18;
        break;
    case 15:
        id = 7;
        break;
    }
    if (id != -1) {
        sVEnt_294F78* vt = *(sVEnt_294F78**)((char*)self + 0xC);
        vt[1].fn((char*)self + vt[1].delta, 0, id);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayHistoryExists);
#ifdef SKIP_ASM
extern "C" void* cBankMonitor_BANKMONITOR_GetHandle(void* self, int idx, int a2);

struct sSnd5028Ring {
    int cap;
    int* data;
    int head;
    int tail;
};
struct sSnd5028Sys {
    char pad0[0x118];
    char** mgr;                 // 0x118
    char pad11C[0x597C - 0x11C];
    sSnd5028Ring ring;          // 0x597C
};

static inline int ring5028Count(sSnd5028Sys* s)
{
    return (s->ring.head - s->ring.tail + s->ring.cap) % s->ring.cap;
}
static inline int ring5028Empty(sSnd5028Sys* s)
{
    return s->ring.head == s->ring.tail;
}
static inline int ring5028Get(sSnd5028Sys* s, int i)
{
    if (ring5028Empty(s) || i >= ring5028Count(s)) return -1;
    sSnd5028Ring* r = &s->ring;
    return r->data[(s->ring.tail + i) % s->ring.cap];
}

/**
 * Tells whether the play history holds a sound for an event that is still playing.
 *
 * The history (mPlayHistory) is a FIFO of 5-int records: event, voice handle, time, looping, hull. A record
 * with
 * handle 0 counts as playing.
 *
 * @param vself  the audio system
 * @param Ptr  event key: an instance pointer or a small event ID
 * @param Hull  collision hull (0 for most events)
 * @return 1 when a matching record is found, otherwise 0
 */
extern "C" int cSSXAudio_PlayHistoryExists(cSSXAudio* vself, int Ptr, int Hull)
{
    sSnd5028Sys* self = (sSnd5028Sys*)vself;
    int n = ring5028Count(self);
    for (int i = 0; i < n; i += 5) {
        int x = ring5028Get(self, i);
        int y = ring5028Get(self, i + 4);
        if (Ptr == x && Hull == y) {
            int handle = ring5028Get(self, i + 1);
            if (handle != 0 && cBankMonitor_BANKMONITOR_GetHandle(*self->mgr + 0x1D8, handle, 1) == 0) continue;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00295208);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// (this unit's own sSnd5028Sys / sSnd5028Ring stand in for their sSSXAudioView ring; helpers renamed for this function)
static inline int sndRingCount_5208(sSnd5028Sys* s)
{
    return (s->ring.head - s->ring.tail + s->ring.cap) % s->ring.cap;
}
static inline int sndRingEmpty_5208(sSnd5028Sys* s)
{
    return s->ring.head == s->ring.tail;
}
static inline int sndRingFull_5208(sSnd5028Sys* s)
{
    return (s->ring.head + 1) % s->ring.cap == s->ring.tail;
}
static inline int sndRingPop_5208(sSnd5028Sys* s)
{
    sSnd5028Ring* r = &s->ring;
    if (sndRingEmpty_5208(s)) return -1;
    int v = r->data[r->tail];
    if (++s->ring.tail >= s->ring.cap) s->ring.tail = 0;
    return v;
}
static inline void sndRingPush_5208(sSnd5028Sys* s, int v)
{
    sSnd5028Ring* r = &s->ring;
    if (sndRingFull_5208(s)) return;
    r->data[r->head] = v;
    if (++s->ring.head >= s->ring.cap) s->ring.head = 0;
}

// Ages the queue of pending one-shot sounds (records of 5: event, handle, end time, stop flag, index): finished
// sounds are dropped, timed-out ones are stopped if flagged, the others go back into the queue.
extern "C" void func_00295208(cSSXAudio* vself)
{
    sSnd5028Sys* self = (sSnd5028Sys*)vself;
    int n = sndRingCount_5208(self) / 5;
    for (int i = 0; i < n; i++) {
        int eventId = sndRingPop_5208(self);
        int handle = sndRingPop_5208(self);
        int fireTime = sndRingPop_5208(self);
        int stop = sndRingPop_5208(self);
        int idx = sndRingPop_5208(self);
        if (handle < 0) continue;
        if (handle != 0 && cBankMonitor_BANKMONITOR_GetHandle(*self->mgr + 0x1D8, handle, 1) == 0) continue;
        char* mgr = *self->mgr;
        char* bank = mgr + 0x1D8;
        if (*(volatile int*)(mgr + 0x260) < fireTime) {
            sndRingPush_5208(self, eventId);
            sndRingPush_5208(self, handle);
            sndRingPush_5208(self, fireTime);
            sndRingPush_5208(self, stop);
            sndRingPush_5208(self, idx);
        } else if (fireTime == -1) {
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
static inline int ring5628Full(sSnd5028Sys* s)
{
    return (s->ring.head + 1) % s->ring.cap == s->ring.tail;
}
static inline int ring5628Pop(sSnd5028Sys* s)
{
    sSnd5028Ring* r = &s->ring;
    if (ring5028Empty(s)) return -1;
    int v = r->data[r->tail];
    if (++s->ring.tail >= s->ring.cap) s->ring.tail = 0;
    return v;
}
static inline void ring5628Push(sSnd5028Sys* s, int v)
{
    sSnd5028Ring* r = &s->ring;
    if (ring5628Full(s)) return;
    r->data[r->head] = v;
    if (++s->ring.head >= s->ring.cap) s->ring.head = 0;
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
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* vself, void* evt, int Handle, int Time, int Looping, int idx)
{
    sSnd5028Sys* self = (sSnd5028Sys*)vself;
    if (ring5628Full(self)) {
        ring5628Pop(self);
        cBankMonitor_BANKMONITOR_StopHandle(*self->mgr + 0x1D8, ring5628Pop(self), 1, 0.0f);
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
static inline int ring5950Full(sSnd5028Sys* s)
{
    return (s->ring.head + 1) % s->ring.cap == s->ring.tail;
}
static inline int ring5950Pop(sSnd5028Sys* s)
{
    sSnd5028Ring* r = &s->ring;
    if (ring5028Empty(s)) return -1;
    int v = r->data[r->tail];
    if (++s->ring.tail >= s->ring.cap) s->ring.tail = 0;
    return v;
}
static inline void ring5950Push(sSnd5028Sys* s, int v)
{
    sSnd5028Ring* r = &s->ring;
    if (ring5950Full(s)) return;
    r->data[r->head] = v;
    if (++s->ring.head >= s->ring.cap) s->ring.head = 0;
}

/**
 * Changes the time stamp of the play-history records for an event and hull.
 *
 * The loop runs once per int rather than once per record, so it cycles the whole FIFO five times; the result
 * is
 * the same.
 *
 * @param vself  the audio system
 * @param evt  event key to look for (On Tour SearchPtr)
 * @param NewTime  new time stamp
 * @param idx  collision hull to look for (On Tour SearchHull)
 */
extern "C" void cSSXAudio_PlayHistoryUpdateTime(cSSXAudio* vself, int evt, int NewTime, int idx)
{
    sSnd5028Sys* self = (sSnd5028Sys*)vself;
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
INCLUDE_ASM("sound/soundsys", func_00295CC8);
#ifdef SKIP_ASM
static inline int ring5CC8Full(sSnd5028Sys* s)
{
    return (s->ring.head + 1) % s->ring.cap == s->ring.tail;
}
static inline int ring5CC8Pop(sSnd5028Sys* s)
{
    sSnd5028Ring* r = &s->ring;
    if (ring5028Empty(s)) return -1;
    int v = r->data[r->tail];
    if (++s->ring.tail >= s->ring.cap) s->ring.tail = 0;
    return v;
}
static inline void ring5CC8Push(sSnd5028Sys* s, int v)
{
    sSnd5028Ring* r = &s->ring;
    if (ring5CC8Full(s)) return;
    r->data[r->head] = v;
    if (++s->ring.head >= s->ring.cap) s->ring.head = 0;
}

// Drops every queued 5-word event record whose first word is evt (stopping its voice if the
// record's fourth word is set) and requeues the rest.
extern "C" void func_00295CC8(cSSXAudio* vself, int evt)
{
    sSnd5028Sys* self = (sSnd5028Sys*)vself;
    int n = ring5028Count(self);
    for (int i = 0; i < n; i++) {
        int eventId = ring5CC8Pop(self);
        int handle = ring5CC8Pop(self);
        int fireTime = ring5CC8Pop(self);
        int stop = ring5CC8Pop(self);
        int idx = ring5CC8Pop(self);
        if (evt == eventId) {
            if (stop != 0) {
                cBankMonitor_BANKMONITOR_StopHandle(*self->mgr + 0x1D8, handle, 1, 0.0f);
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
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
// cSSXAudio_PlayHistoryExists takes the event pointer here (the unit declares (void*, int, int) later).
int cSSXAudio_PlayHistoryExists_p(void* self, void* evt, int idx) __asm__("cSSXAudio_PlayHistoryExists");
extern "C" int func_002B5F60(void* mgr, void* evt, int id, void* rider, int vol);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);
extern void* gWorldTriggerManager;

struct sSsVt6088 { short delta; short index; int (*fn)(void*); };

/**
 * Plays the collision sound of a world instance a rider has hit, scaled by the rider's speed.
 *
 * Only for instances with collision sound data (+0x8C), near a listener and not already playing for that
 * hull.
 * The sound ID is the hull's entry (or the first), played through the world trigger manager; the record is
 * kept
 * in the play history for 1000 ticks.
 *
 * @param rider  the rider
 * @param evt  the instance hit (On Tour tInstanceRecord* pInst)
 * @param idx  collision hull
 */
extern "C" void cSSXAudio_CollisionSound(cSSXAudio* self, char* rider, char* evt, int idx)
{
    if (*(void**)(evt + 0x8C) == 0) return;
    char* obj = rider + 0x6C0;
    sSsVt6088* vtEntry = &(*(sSsVt6088**)obj)[5];
    if (cSSXAudio_CameraInRange(self, vtEntry->fn(obj + vtEntry->delta), 100.0f) < 0) return;
    if (cSSXAudio_PlayHistoryExists_p(self, evt, idx) != 0) return;
    sSndVEntryVec* vt = *(sSndVEntryVec**)obj;
    sSndVec4* hitVel = vt[2].fn(obj + vt[2].delta);
    int volume = (int)func_00290C10(0, sndVu0Length(*hitVel));
    if (volume > 127) volume = 127;
    if (volume < 0) volume = 0;
    int* tbl = *(int**)(*(char**)(evt + 0x8C) + 0x10);
    if (idx >= tbl[0]) idx = 0;
    int* ids = tbl + 2;
    int* trigger = ids;
    if (ids[idx] != 0) trigger = &ids[idx];
    int handle = func_002B5F60(gWorldTriggerManager, evt, *trigger, rider, volume);
    if (handle < 0) return;
    cSSXAudio_PlayHistoryAdd(self, evt, handle, *(volatile int*)(*(char**)self->MusicSys.mpVStreamSys + 0x260) + 1000, 0, idx);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002961F0);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void func_002A73D8(cCrowdSys* self, int a1, int i);
extern "C" void func_002A7438(cCrowdSys* self, int i);
extern "C" void func_0028F108(void* self, void* rider);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");

// PORT: PS2-only inline asm (float absolute value).
static inline float absf_961F0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

extern "C" void func_002961F0(void* self, char* rider, float intensity)
{
    int idx = -1;
    if (cSSXAudio_ReplayMode(self) != 0) {
        idx = 0;
    } else {
        int ok = *(int*)(rider + 0x874) != 0 && *(int*)(rider + 0x87C) != 0;
        if (ok) {
            idx = *(int*)(rider + 0x870);
        }
    }
    if (cSSXAudio_GetFocusRider_p(self, idx) != rider) {
        return;
    }
    float magnitude = absf_961F0(intensity);
    if (magnitude <= 0.0f) {
        func_002A7438((cCrowdSys*)self, idx);
    } else if (magnitude < 0.25f) {
        func_002A73D8((cCrowdSys*)self, 0, idx);
    } else if (magnitude < 0.65f) {
        func_002A73D8((cCrowdSys*)self, 1, idx);
    } else {
        func_002A73D8((cCrowdSys*)self, 2, idx);
    }
    func_0028F108(self, rider);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00296308);
#ifdef SKIP_ASM
extern "C" void func_00296308(void) {

}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00296310);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
class cController;
class cBankMonitorInstance;
// Sound callback: a g++ 2.95 pointer to member function (8 bytes), as cSndPlayOpts::OPTS_Callback takes it.
typedef void (cController::*sSndCallback)(cController*, cBankMonitorInstance*, float);
struct sSndOptsRec {
    signed char vol;
    char pad1[0xB];
    unsigned short fC;       // 0x0C
    char padE[0xA];
};

// One cSndPlayOpts mDelay[] entry (reset to 100 / 0 / 90 / 50 on every push).
struct sSndOptsDelay {
    int f0;
    int f4;
    int f8;
    int fC;
};

#define SND_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SND_OPTS_MAX(a, b) ((a) >= (b) ? (a) : (b))

// cSndPlayOpts (prototype class): the play-option stack; push = cur++ and copy def into mCurrent[cur]. The
// allocation tags of its ctor (sound/ps2soundman) name the arrays: ch = mCurrent, p34 = mPosStatic,
// vals = mBaseVol, p3C = mVolControl, p60 = mPtr, p64 = mViewport, p68 = mVanish, p6C = mRadius, p70 = mDelay,
// p74 = mExternalVol, p78 = mMapToMaxVol, p7C = mCloseOnEnd, p80 = mTwoPlayerPan, p84 = mTwoPlayerSide.
struct cSndPlayOpts {
    void* vbase;             // 0x00 virtual-base pointer
    sSndOptsRec def;         // 0x04
    volatile int cur;        // 0x1C
    sSndOptsRec* ch;         // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    sSndCallback callbacks[4]; // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndOptsDelay* p70;      // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
    void setVal(int i, int v) {
        vals[i] = v;
    }
    // mCurrent[i].vol = the current base volume clamped to 0..127
    void update(int i) {
        ch[i].vol = SND_OPTS_MAX(SND_MIN(vals[cur], 0x7F), 0);
    }
};

// cBankManager as this unit sees it: only its cSndPlayOpts part at 0x1D8 is used here.
struct cBankManager {
    char pad0[0x1D8];
    cSndPlayOpts sub;        // 0x1D8
};

struct sSndOwner {
    char pad0[0x80];
    char* rider;             // 0x80
};
// handle lists (func_002A4BA8 clears them; func_002A4CA0 picks from one) and their timing floats.
extern "C" void* func_002A4BA8(tCrowdEventData* self);
struct sSndList5 {
    char pad0[0x18];
    sSndList5() { func_002A4BA8((tCrowdEventData*)(this)); }
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
    cBankManager** p118;     // 0x118
    char pad11C[0x597C - 0x11C];
    sSnd5028Ring ring;       // 0x597C
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
    sSndQuad2 xf;            // 0x6050 rider transform pair (func_002DA1C0 output)
    int idx;                 // 0x6070 rider index (cBeing::getAIIndex)
};

// sSSXAudioView's pending one-shot queue (an int ring of 5-word records).
static inline int sndRingCount(sSSXAudioView* s)
{
    return (s->ring.head - s->ring.tail + s->ring.cap) % s->ring.cap;
}
static inline int sndRingEmpty(sSSXAudioView* s)
{
    return s->ring.head == s->ring.tail;
}
static inline int sndRingGet(sSSXAudioView* s, int i)
{
    if (sndRingEmpty(s) || i >= sndRingCount(s)) return -1;
    sSnd5028Ring* r = &s->ring;
    return r->data[(s->ring.tail + i) % s->ring.cap];
}
static inline int sndRingFull(sSSXAudioView* s)
{
    return (s->ring.head + 1) % s->ring.cap == s->ring.tail;
}
static inline int sndRingPop(sSSXAudioView* s)
{
    sSnd5028Ring* r = &s->ring;
    if (sndRingEmpty(s)) return -1;
    int v = r->data[r->tail];
    if (++s->ring.tail >= s->ring.cap) s->ring.tail = 0;
    return v;
}
static inline void sndRingPush(sSSXAudioView* s, int v)
{
    sSnd5028Ring* r = &s->ring;
    if (sndRingFull(s)) return;
    r->data[r->head] = v;
    if (++s->ring.head >= s->ring.cap) s->ring.head = 0;
}

// cSndPlayOpts::OPTS_Callback (prototype name): sets the handler of the voice being pushed.
static inline void OPTS_Callback(cBankManager* m, sSndCallback cb)
{
    cSndPlayOpts* s = &m->sub;
    s->callbacks[m->sub.cur] = cb;
}

// Callees declared by several functions below.
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
static inline int sndVCall(char* o, int slot)
{
    sSndVtI* vt = *(sSndVtI**)o;
    return vt[slot].fn(o + vt[slot].delta);
}

extern "C" {
void* func_00416210(void* dst, int c, int n);
void* func_00287968(void* self, int a1, int a2);
int cBankSys_SFX_End(cBankSys* self);
int func_00288A20(void* self, void* obj);
int cSSXAudio_ReplayMode(void* self);
int cSSXAudio_CameraInRange(void* self, int which, float f);
unsigned int func_002ADF60(void* rng);

}

static inline int vcall_6310(char* o, int slot)
{
    struct sSndVtI* vt = *(struct sSndVtI**)o;
    return vt[slot].fn(o + vt[slot].delta);
}

static inline void optsVol_6310(struct cBankManager* m, int v)
{
    struct cSndPlayOpts* o = &m->sub;
    o->vals[m->sub.cur] = v;
    o->ch[m->sub.cur].vol = SND_MAX(0, SND_MIN(o->vals[o->cur], 0x7F));
}

static inline void sfxBegin_6310(struct sSSXAudioView* self, int a, int b)
{
    struct cBankManager* m = self->mpVBankManager;
    struct cSndPlayOpts* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    self->mBank[self->mpVBankManager->sub.cur] = a;
    self->mPatch[self->mpVBankManager->sub.cur] = b;
}

extern func_00290B58_sCurve D_00482DA0;
extern "C" void func_00296310(cSSXAudio* vself, char* obj)
{
    struct sSSXAudioView* self = (struct sSSXAudioView*)vself;
    if (vcall_6310(obj + 0x6C0, 9) != 0 || *(int*)(obj + 0x87C) == 0) {
        if (cSSXAudio_CameraInRange(self, vcall_6310(obj + 0x6C0, 5), 100.0f) < 0) {
            return;
        }
    }
    struct func_00290B58_sCurve curve = D_00482DA0;
    int vol = (int)func_00290B58(&curve, *(float*)(obj + 0x770));
    int a = 0;
    int b;
    if (vol > 0x7F) {
        vol = 0x7F;
    }
    if (vol < 0) {
        vol = 0;
    }
    int st = *(int*)(obj + 0x438);
    if (st == 2)
        goto state23;
    if (st == 3) {
state23:
        a = 10;
        b = 9;
    } else {
        b = (func_002ADF60((*self->p118)->sub.vbase) & 0x7FFF) * 3 / 0x7FFF + 0x30;
    }
    int flag = 0;
    int ok = *(int*)(obj + 0x874) && *(int*)(obj + 0x87C);
    if (ok) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C11[0] == 2) {
            flag = cSSXAudio_ReplayMode(self) == 0;
        }
    }
    sfxBegin_6310(self, a, b);
    optsVol_6310(*self->p118, vol);
    struct cBankManager** pp = self->p118;
    int r = (int)func_00287968(self, 5, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    {
        struct cBankManager* m2 = *self->p118;
        m2->sub.tgt[m2->sub.cur] = obj + 0x110;
    }
    {
        struct cBankManager** pq = self->p118;
        int q = func_00288A20(self, obj);
        (*pq)->sub.p64[(*pq)->sub.cur] = q;
    }
    if (flag) {
        (*self->p118)->sub.p80[(*self->p118)->sub.cur] = 1;
        (*self->p118)->sub.p84[(*self->p118)->sub.cur] = *(int*)(obj + 0x870);
    }
    cBankSys_SFX_End((cBankSys*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00296868);
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
// handle lists (func_002A4BA8 clears them; func_002A4CA0 picks from one) and their timing floats.
extern "C" void* func_002A4BA8(tCrowdEventData* self);
// sSSXAudioView's pending one-shot queue (an int ring of 5-word records).
// cSndPlayOpts::OPTS_Callback (prototype name): sets the handler of the voice being pushed.
// Callees declared by several functions below.
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
extern "C" unsigned int func_002ADF60(void* rng);

extern sSndCallback D_004A36B0_cb[] __asm__("D_004A36B0");
extern func_00290B58_sCurve D_00482DA0;
// Board slide sound of an owner's rider: started once per rider, volume from the speed curve.
extern "C" void func_00296868(cSSXAudio* vself, sSndOwner* t)
{
    sSSXAudioView* self = (sSSXAudioView*)vself;
    if (*(int*)(t->rider + 0x774) >= 0) {
        return;
    }
    if (sndVCall(t->rider + 0x6C0, 9) != 0 || *(int*)(t->rider + 0x87C) == 0) {
        if (cSSXAudio_CameraInRange(self, sndVCall(t->rider + 0x6C0, 5), 100.0f) < 0) {
            return;
        }
    }
    func_00290B58_sCurve curve = D_00482DA0;
    int vol = (int)func_00290B58(&curve, *(float*)(t->rider + 0x770));
    int b = 51;
    if (vol > 0x7F) {
        vol = 0x7F;
    }
    if (vol < 0) {
        vol = 0;
    }
    int flag = 0;
    bool ok = *(int*)(t->rider + 0x874) && *(int*)(t->rider + 0x87C);
    if (ok) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C11[0] == 2) {
            flag = cSSXAudio_ReplayMode(self) == 0;
        }
    }
    cBankManager* m = self->mpVBankManager;
    cSndPlayOpts* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    self->mBank[self->mpVBankManager->sub.cur] = 0;
    self->mPatch[self->mpVBankManager->sub.cur] = b;
    cBankManager* m3 = *self->p118;
    cSndPlayOpts* s3 = &m3->sub;
    s3->vals[m3->sub.cur] = vol;
    s3->ch[m3->sub.cur].vol = (s3->vals[s3->cur] < 0x7F && s3->vals[s3->cur] < 0) ? 0 : (s3->vals[s3->cur] < 0x7F ? (unsigned char)s3->vals[s3->cur] : 0x7F);
    cBankManager** pp = self->p118;
    int r = (int)func_00287968(self, 5, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    {
        cBankManager* m2 = *self->p118;
        m2->sub.tgt[m2->sub.cur] = t->rider + 0x110;
    }
    {
        cBankManager** pq = self->p118;
        int q = func_00288A20(self, t ? t->rider : 0);
        (*pq)->sub.p64[(*pq)->sub.cur] = q;
    }
    {
        cBankManager* m2 = *self->p118;
        m2->sub.p60[m2->sub.cur] = (int)t;
    }
    OPTS_Callback(*self->p118, D_004A36B0_cb[0]);
    (*self->p118)->sub.p7C[(*self->p118)->sub.cur] = 0;
    if (flag) {
        (*self->p118)->sub.p80[(*self->p118)->sub.cur] = 1;
        (*self->p118)->sub.p84[(*self->p118)->sub.cur] = *(int*)(t->rider + 0x870);
    }
    *(int*)(t->rider + 0x774) = cBankSys_SFX_End((cBankSys*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00296E18);
#ifdef SKIP_ASM
extern "C" void func_00296E18(void) {

}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00296E20);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

extern "C" void func_00296E20(void* self, void* obj)
{
    if (*(int*)((char*)obj + 0x774) >= 0) {
        cBankMonitor_BANKMONITOR_StopHandle((char*)**(void***)((char*)cAudioInterface_bxAudio() + 0x118) + 0x1D8, *(int*)((char*)obj + 0x774), 1, 0.75f);
        *(int*)((char*)obj + 0x774) = -1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00296E80);
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
// handle lists (func_002A4BA8 clears them; func_002A4CA0 picks from one) and their timing floats.
extern "C" void* func_002A4BA8(tCrowdEventData* self);
// sSSXAudioView's pending one-shot queue (an int ring of 5-word records).
// cSndPlayOpts::OPTS_Callback (prototype name): sets the handler of the voice being pushed.
// Callees declared by several functions below.
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
extern "C" unsigned int func_002ADF60(void* rng);

extern sSndCallback D_004A36B8_cb[] __asm__("D_004A36B8");
extern func_00290B58_sCurve D_00482DA0;
// Board slide sound of an owner's rider: started once per rider, volume from the speed curve.
extern "C" void func_00296E80(cSSXAudio* vself, sSndOwner* t)
{
    sSSXAudioView* self = (sSSXAudioView*)vself;
    if (*(int*)(t->rider + 0x778) >= 0) {
        return;
    }
    if (sndVCall(t->rider + 0x6C0, 9) != 0 || *(int*)(t->rider + 0x87C) == 0) {
        if (cSSXAudio_CameraInRange(self, sndVCall(t->rider + 0x6C0, 5), 100.0f) < 0) {
            return;
        }
    }
    func_00290B58_sCurve curve = D_00482DA0;
    int vol = (int)func_00290B58(&curve, *(float*)(t->rider + 0x770));
    int b = 52;
    if (vol > 0x7F) {
        vol = 0x7F;
    }
    if (vol < 0) {
        vol = 0;
    }
    int flag = 0;
    bool ok = *(int*)(t->rider + 0x874) && *(int*)(t->rider + 0x87C);
    if (ok) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C11[0] == 2) {
            flag = cSSXAudio_ReplayMode(self) == 0;
        }
    }
    cBankManager* m = self->mpVBankManager;
    cSndPlayOpts* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    self->mBank[self->mpVBankManager->sub.cur] = 0;
    self->mPatch[self->mpVBankManager->sub.cur] = b;
    cBankManager* m3 = *self->p118;
    cSndPlayOpts* s3 = &m3->sub;
    s3->vals[m3->sub.cur] = vol;
    s3->ch[m3->sub.cur].vol = (s3->vals[s3->cur] < 0x7F && s3->vals[s3->cur] < 0) ? 0 : (s3->vals[s3->cur] < 0x7F ? (unsigned char)s3->vals[s3->cur] : 0x7F);
    cBankManager** pp = self->p118;
    int r = (int)func_00287968(self, 5, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    {
        cBankManager* m2 = *self->p118;
        m2->sub.tgt[m2->sub.cur] = t->rider + 0x110;
    }
    {
        cBankManager** pq = self->p118;
        int q = func_00288A20(self, t ? t->rider : 0);
        (*pq)->sub.p64[(*pq)->sub.cur] = q;
    }
    {
        cBankManager* m2 = *self->p118;
        m2->sub.p60[m2->sub.cur] = (int)t;
    }
    OPTS_Callback(*self->p118, D_004A36B8_cb[0]);
    (*self->p118)->sub.p7C[(*self->p118)->sub.cur] = 0;
    if (flag) {
        (*self->p118)->sub.p80[(*self->p118)->sub.cur] = 1;
        (*self->p118)->sub.p84[(*self->p118)->sub.cur] = *(int*)(t->rider + 0x870);
    }
    *(int*)(t->rider + 0x778) = cBankSys_SFX_End((cBankSys*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00297430);
#ifdef SKIP_ASM
extern "C" void func_00297430(void) {

}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00297438);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

extern "C" void func_00297438(void* self, void* obj)
{
    if (*(int*)((char*)obj + 0x778) >= 0) {
        cBankMonitor_BANKMONITOR_StopHandle((char*)**(void***)((char*)cAudioInterface_bxAudio() + 0x118) + 0x1D8, *(int*)((char*)obj + 0x778), 1, 0.75f);
        *(int*)((char*)obj + 0x778) = -1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00297498);
#ifdef SKIP_ASM
extern "C" void func_00297498(void) {

}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_WorldSound);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSnd74A0Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd74A0E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd74A0Sub {
    char pad0[4];
    sSnd74A0Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd74A0Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd74A0E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd74A0Mgr {
    char pad0[0x1D8];
    sSnd74A0Sub sub;         // 0x1D8
};
struct sSnd74A0Sys {
    sSnd74A0Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd74A0Mgr** p118;      // 0x118
};

extern "C" int func_00288A20(void* self, void* obj);

#define SND_74A0_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_74A0_MAX(a, b) ((a) > (b) ? (a) : (b))

extern "C" void func_002A9988(void* self, float a, float b);
extern "C" int func_00288940(void* self, sSndVec4* pos);

static inline void snd74A0AddVoice(void* vself, int a, int b, int type)
{
    sSnd74A0Sys* self = (sSnd74A0Sys*)vself;
    sSnd74A0Mgr* m = self->mgr;
    sSnd74A0Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSnd74A0Mgr* m2 = *self->p118;
    sSnd74A0Sub* s2 = &m2->sub;
    s2->vals[m2->sub.cur] = 0x7F;
    s2->ch[m2->sub.cur].vol = SND_74A0_MAX(0, SND_74A0_MIN(s2->vals[s2->cur], 0x7F));
    sSnd74A0Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
}


extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);

/**
 * Plays a one-shot positional sound at a world instance, chosen by its patch number.
 *
 * Patch 0 plays nothing, 102 is skipped at location 0, and nothing plays while unk_62A8 is set or the
 * instance is
 * already in the play history. Patches below 100 use bank 2, 100-149 bank 8, 150-199 bank 9 and 200+ bank 4.
 *
 * @param evt  the instance (On Tour tInstanceRecord* pInst)
 * @param n  patch number
 */
extern "C" void cSSXAudio_WorldSound(cSSXAudio* self, char* evt, int n)
{
    if (n == 0) return;
    if (n == 102 && *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)) == 0) return;
    if (*(int*)((char*)self + 0x62A8) >= 0) return;
    if (cSSXAudio_PlayHistoryExists(self, (int)evt, 0) != 0) return;
    int pri = 80;
    sSndVec4 pos = *(sSndVec4*)(evt + 0x40);
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
    snd74A0AddVoice(self, type, patch, 5);
    sSnd74A0Mgr** pp;
    int closestCamera;
    sSnd74A0Sys* sys = (sSnd74A0Sys*)self;
    sSnd74A0Mgr* voiceMgr = *sys->p118;
    voiceMgr->sub.tgt[voiceMgr->sub.cur] = &pos;
    sSnd74A0Mgr* voiceMgr2 = *sys->p118;
    voiceMgr2->sub.p34[voiceMgr2->sub.cur] = 1;
    func_002A9988(&(*sys->p118)->sub, 300.0f, -1.0f);
    pp = sys->p118;
    closestCamera = func_00288940(sys, &pos);
    (*pp)->sub.p64[(*pp)->sub.cur] = closestCamera;
    sSnd74A0Mgr* voiceMgr3 = *sys->p118;
    voiceMgr3->sub.p78[voiceMgr3->sub.cur] = pri;
    int sfxHandle = cBankSys_SFX_End((cBankSys*)self);
    cSSXAudio_PlayHistoryAdd(self, evt, sfxHandle, *(volatile int*)(*(char**)self->MusicSys.mpVStreamSys + 0x260) + 50, 0, 0);
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
// handle lists (func_002A4BA8 clears them; func_002A4CA0 picks from one) and their timing floats.
extern "C" void* func_002A4BA8(tCrowdEventData* self);
// sSSXAudioView's pending one-shot queue (an int ring of 5-word records).
// cSndPlayOpts::OPTS_Callback (prototype name): sets the handler of the voice being pushed.
// Callees declared by several functions below.
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
// g++ 2.95 vtable entry for a variadic method returning char*.
struct sSndVtVar { short delta; short index; char* (*fn)(void*, ...); };
static inline void sndPushVoice(void* vself, int a, int b)
{
    cBankSys* self = (cBankSys*)vself;
    cBankManager* m = self->mpVBankManager;
    cSndPlayOpts* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    self->mBank[self->mpVBankManager->sub.cur] = a;
    self->mPatch[self->mpVBankManager->sub.cur] = b;
}

extern "C" int* cBENewRaceInterface_getLocation(void* p);
extern "C" void* cAudioInterface_getBE();

static inline int snd7950Commit(void* vself, void* tgt, float f, int pri)
{
    sSSXAudioView* self = (sSSXAudioView*)vself;
    (*self->p118)->sub.tgt[(*self->p118)->sub.cur] = tgt;
    func_002A9988(&(*self->p118)->sub, f, -1.0f);
    (*self->p118)->sub.p78[(*self->p118)->sub.cur] = pri;
    return cBankSys_SFX_End((cBankSys*)self);
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
 * @param id  mesh index to follow, or -1 for the instance's model
 */
extern "C" void cSSXAudio_AttachSound(cSSXAudio* self, char* obj, int type, int id)
{
    if (type == 0)
        return;
    if (type == 102) {
        if (*cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)) == 0)
            return;
    }
    if (*(int*)((char*)self + 0x62A8) >= 0)
        return;
    if (*(int*)(obj + 0xC) == 0)
        return;
    char* sl;
    int i;
    for (i = 0; i < 30; i++) {
        if (*(int*)((char*)self + i * 0x30 + 0x5A00) == 1 && *(char**)((char*)self + i * 0x30 + 0x5A04) == obj &&
            *(int*)((char*)self + i * 0x30 + 0x5A20) == type) {
            cBankMonitor_BANKMONITOR_StopHandle((char*)*(char**)(cBankManager**)self->MusicSys.mpVStreamSys + 0x1D8, *(int*)((char*)self + i * 0x30 + 0x5A24), 1, 0.0f);
            *(int*)((char*)self + i * 0x30 + 0x5A00) = 0;
            break;
        }
    }
    for (i = 0; i < 30; i++) {
        if (*(int*)((char*)self + i * 0x30 + 0x5A00) == 0) {
            *(int*)((char*)self + i * 0x30 + 0x5A00) = 1;
            break;
        }
    }
    if (i == 30)
        return;
    sl = (char*)self + i * 0x30;
    *(char**)(sl + 0x5A04) = obj;
    if (id == -1) {
        char* inner = *(char**)(obj + 0xC);
        sSndVtVar* vt = *(sSndVtVar**)(inner + 0xC);
        sSndVec4 pos;
        pos = *(sSndVec4*)(vt[24].fn(inner + vt[24].delta) + 0x30);
        *(sSndVec4*)(sl + 0x5A10) = pos;
    } else {
        char* inner = *(char**)(obj + 0xC);
        sSndVtVar* vt = *(sSndVtVar**)(inner + 0xC);
        if (vt[26].fn(inner + vt[26].delta) != 0) {
            char* objNode = *(char**)(obj + 0xC);
            sSndVtVar* vt2 = *(sSndVtVar**)(objNode + 0xC);
            sSndVec4 pos;
            pos = *(sSndVec4*)(vt2[29].fn(objNode + vt2[29].delta, id) + 0x30);
            *(sSndVec4*)(sl + 0x5A10) = pos;
        } else {
            sSndVec4 pos;
            pos = *(sSndVec4*)(obj + 0x40);
            *(sSndVec4*)(sl + 0x5A10) = pos;
        }
    }
    *(int*)((char*)self + i * 0x30 + 0x5A08) = id;
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
    *(int*)((char*)self + i * 0x30 + 0x5A20) = type;
    if (type == 201) {
        if (*cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)) == 8) {
            radius = 400.0f;
            pri = 50;
        }
    }
    sndPushVoice(((sSSXAudioView*)self), curBank, curPatch);
    cBankManager** pp = (cBankManager**)self->MusicSys.mpVStreamSys;
    int volumePtr = (int)func_00287968(((sSSXAudioView*)self), 5, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = volumePtr;
    *(int*)((char*)self + i * 0x30 + 0x5A24) = snd7950Commit(((sSSXAudioView*)self), (char*)self + i * 0x30 + 0x5A10, radius, pri);
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
extern "C" void cSSXAudio_DetachSound(cSSXAudio* self, int inst, int patch)
{
    for (int i = 0; i < 30; i++) {
        if (*(int*)((char*)self + i * 0x30 + 0x5A00) == 1 &&
            *(int*)((char*)self + i * 0x30 + 0x5A04) == inst &&
            *(int*)((char*)self + i * 0x30 + 0x5A20) == patch) {
            cBankMonitor_BANKMONITOR_StopHandle(*(char**)self->MusicSys.mpVStreamSys + 0x1D8,
                          *(int*)((char*)self + i * 0x30 + 0x5A24), 1, 0.0f);
            *(int*)((char*)self + i * 0x30 + 0x5A00) = 0;
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
extern "C" void cSSXAudio_InitAttachedSounds(cSSXAudio* self)
{
    int i;
    for (i = 29; i >= 0; i--) {
        *(int*)((char*)self + i * 0x30 + 0x5a00) = 0;
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

struct sSsVtFA0 { short delta; short index; char* (*fn)(void*, ...); };

/**
 * Moves each attached sound to its instance's current position, freeing it once the instance has no model.
 */
extern "C" void cSSXAudio_UpdateAttachedSounds(cSSXAudio* self)
{
    for (int i = 0; i < 30; i++) {
        char* slot = (char*)self + i * 0x30;
        if (*(int*)(slot + 0x5A00) != 1) continue;
        char* inner = *(char**)(*(char**)(slot + 0x5A04) + 0xC);
        if (inner == 0) {
            cBankMonitor_BANKMONITOR_StopHandle(*(char**)self->MusicSys.mpVStreamSys + 0x1D8, *(int*)(slot + 0x5A24), 1, 0.0f);
            *(int*)(slot + 0x5A00) = 0;
        } else if (*(int*)(slot + 0x5A08) == -1) {
            sSsVtFA0* vt = *(sSsVtFA0**)(inner + 0xC);
            sSsVec4FA0 pos;
            pos = *(sSsVec4FA0*)(vt[24].fn(inner + vt[24].delta) + 0x30);
            *(sSsVec4FA0*)(slot + 0x5A10) = pos;
        } else {
            sSsVtFA0* vt = *(sSsVtFA0**)(inner + 0xC);
            if (vt[26].fn(inner + vt[26].delta) != 0) {
                char* objNode = *(char**)(*(char**)(slot + 0x5A04) + 0xC);
                sSsVtFA0* vt2 = *(sSsVtFA0**)(objNode + 0xC);
                sSsVec4FA0 pos;
                pos = *(sSsVec4FA0*)(vt2[29].fn(objNode + vt2[29].delta, *(int*)(slot + 0x5A08)) + 0x30);
                *(sSsVec4FA0*)(slot + 0x5A10) = pos;
            } else {
                sSsVec4FA0 pos;
                pos = *(sSsVec4FA0*)(*(char**)(slot + 0x5A04) + 0x40);
                *(sSsVec4FA0*)(slot + 0x5A10) = pos;
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
extern "C" void cSSXAudio_ResetAttachedSounds(cSSXAudio* self)
{
    int i;
    for (i = 0; i < 30; i++) {
        if (*(int*)((char*)self + i * 0x30 + 0x5A00) == 1) {
            cBankMonitor_BANKMONITOR_StopHandle((char*)*(void**)self->MusicSys.mpVStreamSys + 0x1D8, *(int*)((char*)self + i * 0x30 + 0x5A24), 1, 0.0f);
            *(int*)((char*)self + i * 0x30 + 0x5A00) = 0;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_Punch);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSnd8138Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd8138E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd8138Sub {
    char pad0[4];
    sSnd8138Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd8138Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd8138E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd8138Mgr {
    char pad0[0x1D8];
    sSnd8138Sub sub;         // 0x1D8
};
struct sSnd8138Sys {
    sSnd8138Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd8138Mgr** p118;      // 0x118
};

extern "C" int cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);
extern "C" int func_00288A20(void* self, void* obj);

static inline bool snd8138Active(void* o)
{
    return *(int*)((char*)o + 0x874) && *(int*)((char*)o + 0x87C);
}

static inline int snd8138AddVoice(void* vself, void* obj, int a, int b, int type)
{
    sSnd8138Sys* self = (sSnd8138Sys*)vself;
    sSnd8138Mgr* m = self->mgr;
    sSnd8138Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSnd8138Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    sSnd8138Mgr* m2 = *self->p118;
    m2->sub.tgt[m2->sub.cur] = (char*)obj + 0x110;
    pp = self->p118;
    r = func_00288A20(self, obj);
    (*pp)->sub.p64[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

/**
 * Plays the punch sound when either rider is a local human, unless one is already in the play history (event
 * 5).
 *
 * @param obj  the rider
 * @param obj2  the opponent
 */
extern "C" void cSSXAudio_Punch(cSSXAudio* self, void* obj, void* obj2)
{
    if (!snd8138Active(obj) && !snd8138Active(obj2)) {
        return;
    }
    if (cSSXAudio_PlayHistoryExists(self, 5, 0) != 0) {
        return;
    }
    int handle = snd8138AddVoice(self, obj, 0, 90, 5);
    cSSXAudio_PlayHistoryAdd(self, (void*)5, handle, *(volatile int*)(*(char**)self->MusicSys.mpVStreamSys + 0x260) + 50, 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00298488);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int func_00288A20(void* self, void* obj);
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
int func_00288C08_8488(void* self, int which, float f) __asm__("func_00288C08");
extern "C" int cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);
extern "C" void func_002A9988(void* self, float a, float b);
extern "C" float func_00290C10(int i, float v);

struct sVec4_8488 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component add).
static inline sVec4_8488 vu0Add_8488(const sVec4_8488& a, const sVec4_8488& b)
{
    sVec4_8488 r;
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
static inline float vu0Length_8488(const sVec4_8488& v)
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

class cSs8488Obj {
public:
    virtual void v01();
    virtual sVec4_8488* v02();
    virtual void v03();
    virtual void v04();
    virtual int v05();
};
struct sSs8488RiderA {
    char pad0[0x6C0];
};
struct sSs8488Rider : public sSs8488RiderA, public cSs8488Obj {
    char pad6C4[0x87C - 0x6C4];
    int f87C;
};

struct sSnd8488Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd8488E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd8488Sub {
    char pad0[4];
    sSnd8488Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd8488Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd8488E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd8488Mgr {
    char pad0[0x1D8];
    sSnd8488Sub sub;         // 0x1D8
};
struct sSnd8488Sys {
    sSnd8488Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd8488Mgr** p118;      // 0x118
};

static inline int snd8488Add(void* vself, void* obj, int a, int b, int c)
{
    sSnd8488Sys* self = (sSnd8488Sys*)vself;
    sSnd8488Mgr* m = self->mgr;
    sSnd8488Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSnd8488Mgr* m3 = *self->p118;
    sSnd8488Sub* s3 = &m3->sub;
    s3->vals[m3->sub.cur] = c;
    s3->ch[m3->sub.cur].vol = (s3->vals[s3->cur] < 0x7F && s3->vals[s3->cur] < 0) ? 0 : (s3->vals[s3->cur] < 0x7F ? (unsigned char)s3->vals[s3->cur] : 0x7F);
    sSnd8488Mgr** pp = self->p118;
    int r = (int)func_00287968(self, 5, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    func_002A9988(&(*self->p118)->sub, 100.0f, -1.0f);
    pp = self->p118;
    r = func_00288A20(self, obj);
    (*pp)->sub.p64[(*pp)->sub.cur] = r;
    sSnd8488Mgr* m2 = *self->p118;
    m2->sub.tgt[m2->sub.cur] = (char*)obj + 0x110;
    return cBankSys_SFX_End((cBankSys*)self);
}

void func_00298488_8488(void* self, sSs8488Rider* a, sSs8488Rider* b) __asm__("func_00298488");
void func_00298488_8488(void* self, sSs8488Rider* a, sSs8488Rider* b)
{
    if (cSSXAudio_CameraInRange(self, a->v05(), 100.0f) < 0) return;
    if (func_00288C08_8488(self, a->v05(), 100.0f) == 0) return;
    bool okA = *(int*)((char*)a + 0x874) && a->f87C;
    if (!okA) {
        bool okB = *(int*)((char*)b + 0x874) && b->f87C;
        if (!okB) return;
    }
    if (cSSXAudio_PlayHistoryExists((cSSXAudio*)self, 7, 0) != 0) return;
    sVec4_8488 sum = vu0Add_8488(*a->v02(), *b->v02());
    int c = (int)func_00290C10(1, (float)(int)vu0Length_8488(sum));
    int h = snd8488Add(self, a, 0, 54, c);
    cSSXAudio_PlayHistoryAdd((cSSXAudio*)self, (void*)7, h, *(volatile int*)(**(char***)((char*)self + 0x118) + 0x260) + 50, 0, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_WallCollision);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSnd89A8Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd89A8E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd89A8Sub {
    char pad0[4];
    sSnd89A8Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd89A8Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd89A8E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd89A8Mgr {
    char pad0[0x1D8];
    sSnd89A8Sub sub;         // 0x1D8
};
struct sSnd89A8Sys {
    sSnd89A8Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd89A8Mgr** p118;      // 0x118
};

extern "C" int cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);
extern "C" int func_00288A20(void* self, void* obj);
extern "C" void func_002A9988(void* self, float a, float b);

static inline int snd89A8AddVoice(void* vself, void* obj, int a, int b, int type)
{
    sSnd89A8Sys* self = (sSnd89A8Sys*)vself;
    sSnd89A8Mgr* m = self->mgr;
    sSnd89A8Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSnd89A8Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    func_002A9988(&(*self->p118)->sub, 100.0f, -1.0f);
    pp = self->p118;
    r = func_00288A20(self, obj);
    (*pp)->sub.p64[(*pp)->sub.cur] = r;
    sSnd89A8Mgr* m2 = *self->p118;
    m2->sub.tgt[m2->sub.cur] = (char*)obj + 0x110;
    return cBankSys_SFX_End((cBankSys*)self);
}

/**
 * Plays the wall-hit sound for a local human rider, unless one is already in the play history (event 6).
 *
 * @param obj  the rider
 */
extern "C" void cSSXAudio_WallCollision(cSSXAudio* self, void* obj)
{
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (!ok) {
        return;
    }
    if (cSSXAudio_PlayHistoryExists(self, 6, 0) != 0) {
        return;
    }
    int handle = snd89A8AddVoice(self, obj, 0, 54, 5);
    cSSXAudio_PlayHistoryAdd(self, (void*)6, handle, *(volatile int*)(*(char**)self->MusicSys.mpVStreamSys + 0x260) + 50, 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_KnockdownSFX);
#ifdef SKIP_ASM
struct sSndKey8 {
    long v;
} __attribute__((packed));
extern sSndKey8 D_004A3678[];
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" void func_002ADCA0(void*, int, long, int, int, int, int, int);

/**
 * Plays the knockdown effect (patch 0x72) for a local human rider.
 *
 * @param obj  the rider
 */
// PORT: 64-bit long sound key
extern "C" void cSSXAudio_KnockdownSFX(cSSXAudio* self, void* obj)
{
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        char* bankMon = *(char**)self->MusicSys.mpVStreamSys + 0x1D8;
        sSndKey8* key = D_004A3678;
        int volumePtr = (int)func_00287968(self, 9, 0);
        func_002ADCA0(bankMon, 250, key->v, volumePtr, 0, 114, 0x7F, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00298D88);
#ifdef SKIP_ASM
extern "C" int func_00298D88(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00298D90);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
void func_002ADDE0_8D90(void* p, long key) __asm__("func_002ADDE0");

struct sSndKey8_8D90 {
    long v;
} __attribute__((packed));
extern const sSndKey8_8D90 D_004A36A8_8D90[] __asm__("D_004A36A8");

struct sSnd8D90Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd8D90E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd8D90Sub {
    char pad0[4];
    sSnd8D90Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd8D90Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd8D90E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd8D90Mgr {
    char pad0[0x1D8];
    sSnd8D90Sub sub;         // 0x1D8
};
struct sSnd8D90Sys {
    sSnd8D90Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd8D90Mgr** p118;      // 0x118
};

static inline int snd8D90Add(void* vself, int a, int b)
{
    sSnd8D90Sys* self = (sSnd8D90Sys*)vself;
    sSnd8D90Mgr* m = self->mgr;
    sSnd8D90Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSnd8D90Mgr** pp = self->p118;
    int r = (int)func_00287968(self, 9, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

static inline void snd8D90Start(sSnd8D90Sys* self, int a, int b)
{
    if (*(int*)((char*)self + 0x59E4) == 0) {
        sSnd8D90Mgr* m = self->mgr;
        sSnd8D90Sub* s = &m->sub;
        m->sub.cur++;
        s->ch[m->sub.cur] = m->sub.def;
        m->sub.tgt[m->sub.cur] = 0;
        s->p34[m->sub.cur] = 0;
        s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
        s->p3C[m->sub.cur] = 0;
        func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
        sSnd8D90Mgr** pp = self->p118;
        int r = (int)func_00287968(self, 9, 0);
        (*pp)->sub.p3C[(*pp)->sub.cur] = r;
        *(int*)((char*)self + 0x59E0) = cBankSys_SFX_End((cBankSys*)self);
    }
}

// PORT: 64-bit long sound key
extern "C" void func_00298D90(cSSXAudio* vself, void* obj, int flag)
{
    sSnd8D90Sys* self = (sSnd8D90Sys*)vself;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (!ok) return;
    const sSndKey8_8D90* key = D_004A36A8_8D90;
    func_002ADDE0_8D90(&(*self->p118)->sub, key->v);
    snd8D90Start(self, 0, 122);
    if (flag != 0) {
        snd8D90Add(self, 0, 74);
    }
    *(int*)((char*)self + 0x59E4) = *(int*)((char*)self + 0x59E4) + 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002992D8);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

extern "C" void func_002992D8(void* self, void* obj)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return;
    }
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        int n = --*(int*)((char*)self + 0x59E4);
        if (n <= 0) {
            if (n == 0) {
                cBankMonitor_BANKMONITOR_StopHandle((char*)**(void***)((char*)self + 0x118) + 0x1D8, *(int*)((char*)self + 0x59E0), 1, 0.0f);
            }
            *(int*)((char*)self + 0x59E4) = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_TurboDeny);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSnd9368Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd9368E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd9368Sub {
    char pad0[4];
    sSnd9368Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd9368Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd9368E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd9368Mgr {
    char pad0[0x1D8];
    sSnd9368Sub sub;         // 0x1D8
};

struct sSndId299368 {
    long v;
} __attribute__((packed));
extern const sSndId299368 D_004A36A8_9368[] __asm__("D_004A36A8");
extern "C" void func_002ADDE0(void* bank, long id);
extern "C" int cSSXAudio_ReplayMode(void* self);

static inline int IsReady299368(char* obj)
{
    return *(int*)(obj + 0x874) != 0 && *(int*)(obj + 0x87C) != 0;
}

static inline void AddVoice299368(cSSXAudio* self, int a, int bb, int type)
{
    sSnd9368Mgr* m = ((sSnd9368Mgr*)self->mpVBankManager);
    sSnd9368Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    self->mBank[((sSnd9368Mgr*)self->mpVBankManager)->sub.cur] = a;
    self->mPatch[((sSnd9368Mgr*)self->mpVBankManager)->sub.cur] = bb;
    sSnd9368Mgr** pp = ((sSnd9368Mgr**)self->MusicSys.mpVStreamSys);
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
}

/**
 * Plays the turbo-denied sound (bank 9, patch 0x6C) for a rider, except during a replay.
 *
 * Only for a local human rider; calls func_002ADDE0() with stream key D_004A36A8 first.
 *
 * @param obj  the rider
 */
// Starts the "crowd cheer" voice for the object if the bank is ready and nothing else is playing.
// PORT: the 8-byte stream id is passed by value from an unaligned global (64-bit `long`).
extern "C" void cSSXAudio_TurboDeny(cSSXAudio* self, char* obj)
{
    if (cSSXAudio_ReplayMode((self)) != 0) {
        return;
    }
    if (!IsReady299368(obj)) {
        return;
    }
    func_002ADDE0(&(*(sSnd9368Mgr**)self->MusicSys.mpVStreamSys)->sub, D_004A36A8_9368[0].v);
    AddVoice299368((self), 0, 108, 9);
    cBankSys_SFX_End((cBankSys*)(self));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00299638);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sSndKey8_9638 {
    long v;
} __attribute__((packed));
// 64-bit key views of two of the handlers: the key stays in a saved register across calls.
extern sSndKey8_9638 D_004A36A8_key[] __asm__("D_004A36A8");
extern "C" void func_002ADDE0_key(void* bank, long id) __asm__("func_002ADDE0");
extern sSndKey8_9638 D_004A3678_key[] __asm__("D_004A3678");
extern "C" int func_002ADCA0_key(void*, int, long, int, int, int, int, int) __asm__("func_002ADCA0");
extern "C" void func_002A3C00(cSSXAudio* self, void* obj, int flags);

// PORT: 64-bit long sound keys
// Plays the landing thud for a rider that is airborne-and-dropping, sets its board effect and restarts the ambient loop.
extern "C" void func_00299638(void* self, char* obj)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return;
    }
    bool ok = *(int*)(obj + 0x874) && *(int*)(obj + 0x87C);
    if (!ok) {
        return;
    }
    bool up = 0.0f <= *(float*)(obj + 0x470);
    if (up) {
        return;
    }
    char* p = **(char***)((char*)self + 0x118) + 0x1D8;
    sSndKey8_9638* key = D_004A3678_key;
    int r = (int)func_00287968(self, 9, 0);
    func_002ADCA0_key(p, 400, key->v, r, 0, 107, 0x7F, 0);
    int m = *(int*)(obj + 0x2F4);
    if (m == 10) {
        func_002A3C00((cSSXAudio*)self, obj, 0x10);
    } else if (m >= 5) {
        func_002A3C00((cSSXAudio*)self, obj, 0x8);
    } else {
        func_002A3C00((cSSXAudio*)self, obj, 0x4);
    }
    long id = D_004A36A8_key[0].v;
    func_002ADDE0_key(**(char***)((char*)self + 0x118) + 0x1D8, id);
    if (*(int*)((char*)self + 0x59E4) != 0) {
        return;
    }
    func_002ADCA0_key(**(char***)((char*)self + 0x118) + 0x1D8, 3000, id, (int)obj, 0, 0, 0, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_LoseMaxPowerEvent);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSnd97B8Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd97B8E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd97B8Sub {
    char pad0[4];
    sSnd97B8Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd97B8Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd97B8E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd97B8Mgr {
    char pad0[0x1D8];
    sSnd97B8Sub sub;         // 0x1D8
};
struct sSnd97B8Sys {
    sSnd97B8Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd97B8Mgr** p118;      // 0x118
};

extern "C" int func_00288A20(void* self, void* obj);

#define SND_97B8_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_97B8_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int snd97B8AddVoice(void* vself, int a, int b, int type)
{
    sSnd97B8Sys* self = (sSnd97B8Sys*)vself;
    sSnd97B8Mgr* m = self->mgr;
    sSnd97B8Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSnd97B8Mgr* m2 = *self->p118;
    sSnd97B8Sub* s2 = &m2->sub;
    s2->vals[m2->sub.cur] = 0x7F;
    s2->ch[m2->sub.cur].vol = SND_97B8_MAX(0, SND_97B8_MIN(s2->vals[s2->cur], 0x7F));
    sSnd97B8Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void* cAudioInterface_getGame();
extern "C" int cReplay_isPauseAI(void* p);
extern int cPauseState_Flags;

/**
 * Plays the lose-max-power sound for a local human rider that is still racing, while the race state (4) runs.
 *
 * Skipped during a replay and while the sound is muted (cPauseState_Flags bit 0, or the replay check).
 *
 * @param obj  the rider
 */
extern "C" void cSSXAudio_LoseMaxPowerEvent(cSSXAudio* self, void* obj)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return;
    }
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (!ok) {
        return;
    }
    int playBeeps = 1;
    int pos = *(float*)((char*)obj + 0x470) >= 0.0f;
    if (pos) playBeeps = 0;
    if ((cPauseState_Flags & 1) || cReplay_isPauseAI(*(void**)((char*)cAudioInterface_getGame() + 0x28)) != 0) playBeeps = 0;
    if (*(int*)((char*)cAudioInterface_getGame() + 0x214) != 4) playBeeps = 0;
    if (playBeeps) {
        snd97B8AddVoice(self, 0, 105, 9);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_UberRailDeny);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSnd9B70Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd9B70E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd9B70Sub {
    char pad0[4];
    sSnd9B70Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd9B70Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd9B70E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd9B70Mgr {
    char pad0[0x1D8];
    sSnd9B70Sub sub;         // 0x1D8
};
struct sSnd9B70Sys {
    sSnd9B70Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd9B70Mgr** p118;      // 0x118
};

static inline int snd9B70AddVoice(void* vself, int a, int b, int type)
{
    sSnd9B70Sys* self = (sSnd9B70Sys*)vself;
    sSnd9B70Mgr* m = self->mgr;
    sSnd9B70Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSnd9B70Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" int cSSXAudio_ReplayMode(void* self);

/**
 * Plays the uber-rail-denied sound (bank 9, patch 0x6C) for a local human rider, except during a replay.
 *
 * @param obj  the rider
 */
extern "C" void cSSXAudio_UberRailDeny(cSSXAudio* self, void* obj)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return;
    }
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        snd9B70AddVoice(self, 0, 108, 9);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_ChangePosition);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSnd9E28Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd9E28E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd9E28Sub {
    char pad0[4];
    sSnd9E28Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd9E28Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd9E28E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd9E28Mgr {
    char pad0[0x1D8];
    sSnd9E28Sub sub;         // 0x1D8
};
struct sSnd9E28Sys {
    sSnd9E28Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd9E28Mgr** p118;      // 0x118
};

static inline int snd9E28AddVoice(void* vself, int a, int b, int type)
{
    sSnd9E28Sys* self = (sSnd9E28Sys*)vself;
    sSnd9E28Mgr* m = self->mgr;
    sSnd9E28Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSnd9E28Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C10[];
extern "C" int func_002A4290(void* self);
extern "C" void* cAudioInterface_getGame();
extern "C" void* cAudioInterface_getAI();
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern "C" int cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);
extern "C" void func_002A0560(void* self, void* a, void* b);

/**
 * Reacts to a rider's race position changing: a sting for taking the lead, then a passing reaction.
 *
 * Only while the race state (4) runs, with at least two players and computers, for a local human or the
 * listener's rider (outside race mode 0 only when func_002A4290() allows). Reaching first place plays bank 9
 * patch
 * 0x6D once (event 1). Then func_002A0560() gets (rider, passed rider) when moving up, or (passing rider,
 * rider).
 *
 * @param obj  the rider
 * @param pPrevPosRider  the rider now behind (On Tour pPrevPosRider), or 0
 * @param pNextPosRider  the rider now ahead (On Tour pNextPosRider), or 0
 * @param lim  old position
 * @param n  new position (0 is first)
 */
extern "C" void cSSXAudio_ChangePosition(cSSXAudio* self, void* obj, void* pPrevPosRider, void* pNextPosRider, int lim, int n)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C10[0] != 0 && func_002A4290(self) == 0) {
        return;
    }
    if (*(int*)((char*)cAudioInterface_getGame() + 0x214) != 4) {
        return;
    }
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (!ok && cSSXAudio_GetFocusRider_p(self, -1) != obj) {
        return;
    }
    int cnt = *(int*)((char*)cAudioInterface_getAI() + 0x7C);
    if (cnt + *(int*)((char*)cAudioInterface_getAI() + 0x80) < 2) {
        return;
    }
    if (n == 0 && cSSXAudio_PlayHistoryExists(self, 1, 0) == 0 && cSSXAudio_ReplayMode(self) == 0) {
        int handle = snd9E28AddVoice(self, 0, 109, 9);
        cSSXAudio_PlayHistoryAdd(self, (void*)1, handle, *(volatile int*)(*(char**)self->MusicSys.mpVStreamSys + 0x260) + 200, 0, 0);
    }
    if (n < lim) {
        if (pPrevPosRider != 0) {
            func_002A0560(self, obj, pPrevPosRider);
        }
    } else {
        if (pNextPosRider != 0) {
            func_002A0560(self, pNextPosRider, obj);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_0029A220);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndA220Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndA220E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndA220Sub {
    char pad0[4];
    sSndA220Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndA220Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndA220E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndA220Mgr {
    char pad0[0x1D8];
    sSndA220Sub sub;         // 0x1D8
};
struct sSndA220Sys {
    sSndA220Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndA220Mgr** p118;      // 0x118
};

static inline int sndA220AddVoice(void* vself, int a, int b, int type)
{
    sSndA220Sys* self = (sSndA220Sys*)vself;
    sSndA220Mgr* m = self->mgr;
    sSndA220Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndA220Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void func_00296E20(void* self, void* obj);
extern "C" void func_00297438(void* self, void* obj);
extern "C" void func_002992D8(void* self, void* obj);
extern "C" void func_0028F108(void* self, void* rider);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");

extern "C" void func_0029A220(void* self, void* obj)
{
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok && cSSXAudio_ReplayMode(self) == 0) {
        sndA220AddVoice(self, 0, 123, 5);
    }
    func_00296E20(self, obj);
    func_00297438(self, obj);
    if (*(int*)((char*)self + 0x59E4) > 0) {
        func_002992D8(self, obj);
    }
    if (obj == cSSXAudio_GetFocusRider_p(self, -1)) {
        func_0028F108(self, obj);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_Tweak);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndA530Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndA530E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndA530Sub {
    char pad0[4];
    sSndA530Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndA530Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndA530E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndA530Mgr {
    char pad0[0x1D8];
    sSndA530Sub sub;         // 0x1D8
};
struct sSndA530Sys {
    sSndA530Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndA530Mgr** p118;      // 0x118
};

static inline int sndA530AddVoice(void* vself, int a, int b, int type)
{
    sSndA530Sys* self = (sSndA530Sys*)vself;
    sSndA530Mgr* m = self->mgr;
    sSndA530Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndA530Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

/**
 * Plays the tweak sound (bank 9, patch 0x77) for a local human rider.
 *
 * @param obj  the rider
 */
extern "C" void cSSXAudio_Tweak(cSSXAudio* self, void* obj)
{
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        sndA530AddVoice(self, 0, 119, 9);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_PointsEvent);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndA7D8Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndA7D8E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndA7D8Sub {
    char pad0[4];
    sSndA7D8Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndA7D8Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndA7D8E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndA7D8Mgr {
    char pad0[0x1D8];
    sSndA7D8Sub sub;         // 0x1D8
};
struct sSndA7D8Sys {
    sSndA7D8Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndA7D8Mgr** p118;      // 0x118
};

static inline int sndA7D8AddVoice(void* vself, int a, int b, int type)
{
    sSndA7D8Sys* self = (sSndA7D8Sys*)vself;
    sSndA7D8Mgr* m = self->mgr;
    sSndA7D8Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndA7D8Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void func_003B58A0();
extern "C" int func_003B8AB8(int id);
extern "C" void func_003B58D8();
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
extern "C" void cSSXAudio_PointsEvent(cSSXAudio* self, void* obj, int n)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return;
    }
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        int curPoints = n % 100;
        if (curPoints < self->mPointsLastValue) {
            func_003B58A0();
            int soundFinished = func_003B8AB8(self->mPointsEventHandle) != 0;
            func_003B58D8();
            if (!soundFinished) {
                cBankMonitor_BANKMONITOR_StopHandle(*(char**)self->MusicSys.mpVStreamSys + 0x1D8, self->mPointsEventHandle, 1, 0.0f);
            }
            self->mPointsEventHandle = sndA7D8AddVoice(self, 0, 77, 9);
        }
        self->mPointsLastValue = curPoints;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029AB08);
#ifdef SKIP_ASM
extern "C" void func_0029AB08(void* self, void* obj)
{
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        *(short*)((char*)self + (*(int*)((char*)obj + 0x870) << 1) + 0x5FE8) = 0x1000;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029AB40);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void* cAudioInterface_getGame();
extern "C" void* cAudioInterface_getAI();
extern "C" int cReplay_isPauseAI(void* p);
// PORT: func_002ADCA0 returns int; the unit declares it void elsewhere.
extern "C" int func_002ADCA0_AB40(void*, int, long, int, int, int, int, int) __asm__("func_002ADCA0");
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C11[];
extern int cPauseState_Flags;

struct sSndKey8_AB40 {
    long v;
} __attribute__((packed));
extern const sSndKey8_AB40 D_004A36C0_AB40[] __asm__("D_004A36C0");

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

struct sMon_AB40 {
    char pad0[4];
    float f4;
    float f8;
};
struct sMon1B0_AB40 {
    char pad[0x3A8];
    sMon_AB40 m;
};
struct sMon790_AB40 {
    char pad[0x1B0];
    sMon1B0_AB40* p1B0;
};
struct sRider_AB40 {
    char pad0[0x2F0];
    float f2F0;
    char pad2F4[0x470 - 0x2F4];
    float f470;
    char pad474[0x790 - 0x474];
    sMon790_AB40* p790;
    char pad794[0x870 - 0x794];
    int f870;
    int f874;
    int f878;
    int f87C;
};

struct sSndAB40Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndAB40E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndAB40Sub {
    char pad0[4];
    sSndAB40Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndAB40Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndAB40E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndAB40Mgr {
    char pad0[0x1D8];
    sSndAB40Sub sub;         // 0x1D8
};
struct sSndAB40Sys {
    sSndAB40Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndAB40Mgr** p118;      // 0x118
    char pad11C[0x5FE0 - 0x11C];
    int arr1[2];             // 0x5FE0
    unsigned short arr2[2];  // 0x5FE8
    int arr3[2];             // 0x5FEC
};

static inline int sndAB40Add(void* vself, int a, int b, int type)
{
    sSndAB40Sys* self = (sSndAB40Sys*)vself;
    sSndAB40Mgr* m = self->mgr;
    sSndAB40Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndAB40Mgr* m2 = *self->p118;
    sSndAB40Sub* s2 = &m2->sub;
    s2->vals[m2->sub.cur] = 0x7F;
    s2->ch[m2->sub.cur].vol = SND_AB40_MAX(0, SND_AB40_MIN(s2->vals[s2->cur], 0x7F));
    sSndAB40Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

// PORT: 64-bit long sound key; PS2-only FPU asm
extern "C" void func_0029AB40(cSSXAudio* vself)
{
    sSndAB40Sys* self = (sSndAB40Sys*)vself;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int n = 1;
    if (D_00535C11[0] == 2) n = 2;
    for (int i = 0; i < n; i++) {
        sRider_AB40* r0 = *(sRider_AB40**)((char*)cAudioInterface_getAI() + (i << 2) + 0x28);
        bool ok = r0->f874 && r0->f87C;
        if (!ok) continue;
        sRider_AB40* rider = *(sRider_AB40**)((char*)cAudioInterface_getAI() + (i << 2) + 0x28);
        sMon1B0_AB40* mon = rider->p790->p1B0;
        sMon_AB40* mp = mon ? &mon->m : 0;
        int v = (int)ffloor_AB40(mp->f8 / mp->f4 * 10.0f);
        if (self->arr1[i] < v && v > 0) {
            void* sub = &(*self->p118)->sub;
            int h = (int)func_00287968(self, 9, 0);
            func_002ADCA0_AB40(sub, 150, D_004A36C0_AB40[0].v, h, 0, 101, 0x7F, self->arr2[rider->f870]);
            self->arr2[rider->f870] += 0x100;
        }
        self->arr1[i] = v;
        int w = (int)ffloor_AB40(rider->f2F0 * 2.0f);
        if (w > 0) {
            if (w == self->arr3[i] - 1) {
                if (w >= 13) {
                } else if ((w & 1) && w >= 5) {
                } else {
                    int c = 1;
                    int pos = rider->f470 >= 0.0f;
                    if (pos) c = 0;
                    if ((cPauseState_Flags & 1) || cReplay_isPauseAI(*(void**)((char*)cAudioInterface_getGame() + 0x28)) != 0) c = 0;
                    if (*(int*)((char*)cAudioInterface_getGame() + 0x214) != 4) c = 0;
                    if (c) {
                        sndAB40Add(self, 0, 104, 9);
                    }
                }
            }
        }
        self->arr3[i] = w;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_StartUberSpin);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndB0E0Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndB0E0E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndB0E0Sub {
    char pad0[4];
    sSndB0E0Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndB0E0Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndB0E0E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndB0E0Mgr {
    char pad0[0x1D8];
    sSndB0E0Sub sub;         // 0x1D8
};
struct sSndB0E0Sys {
    sSndB0E0Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndB0E0Mgr** p118;      // 0x118
};

static inline int sndB0E0AddVoice(void* vself, int a, int b, int type)
{
    sSndB0E0Sys* self = (sSndB0E0Sys*)vself;
    sSndB0E0Mgr* m = self->mgr;
    sSndB0E0Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndB0E0Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Starts the uber-spin sound for a local human rider, replacing one that is playing; not during a replay.
 *
 * @param obj  the rider
 */
extern "C" void cSSXAudio_StartUberSpin(cSSXAudio* self, void* obj)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return;
    }
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        if (self->mUberSpinHandle != -1) {
            cBankMonitor_BANKMONITOR_StopHandle(*(char**)self->MusicSys.mpVStreamSys + 0x1D8, self->mUberSpinHandle, 1, 0.0f);
        }
        self->mUberSpinHandle = sndB0E0AddVoice(self, 0, 102, 9);
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
extern "C" void cSSXAudio_EndUberSpin(cSSXAudio* self, void* obj)
{
    if (obj != 0) {
        int ok = 0;
        if (*(int*)((char*)obj + 0x874) != 0) {
            ok = *(int*)((char*)obj + 0x87C) != 0;
        }
        if (!ok) {
            return;
        }
    }
    if (self->mUberSpinHandle != -1) {
        cBankMonitor_BANKMONITOR_StopHandle(*(char**)self->MusicSys.mpVStreamSys + 0x1D8, self->mUberSpinHandle, 1, 0.0f);
        self->mUberSpinHandle = -1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_LandedUberTrick);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndB430Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndB430E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndB430Sub {
    char pad0[4];
    sSndB430Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndB430Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndB430E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndB430Mgr {
    char pad0[0x1D8];
    sSndB430Sub sub;         // 0x1D8
};
struct sSndB430Sys {
    sSndB430Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndB430Mgr** p118;      // 0x118
};

static inline int sndB430AddVoice(void* vself, int a, int b, int type)
{
    sSndB430Sys* self = (sSndB430Sys*)vself;
    sSndB430Mgr* m = self->mgr;
    sSndB430Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndB430Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void func_002A3DE0(cSSXAudio* self, void* obj, int ev);

/**
 * Plays the landed-uber-trick sound for a local human rider, except during a replay.
 *
 * When the count goes from below 4 to 4-8, also calls func_002A3DE0(rider, 1).
 *
 * @param obj  the rider
 * @param iNum  number of uber tricks just landed (On Tour iNum)
 * @param iPrevious  number landed before (On Tour iPrevious)
 */
extern "C" void cSSXAudio_LandedUberTrick(cSSXAudio* self, void* obj, int iNum, int iPrevious)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return;
    }
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        sndB430AddVoice(self, 0, 103, 9);
        int total = iPrevious + iNum;
        if (iPrevious < 4) {
            if (total >= 4) {
                if (total < 9) {
                    func_002A3DE0(self, obj, 1);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029B738);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cSSXAudio_GetFocusRider(void* self, int which);
extern "C" void* cAudioInterface_getGame();
extern "C" void func_002A3DE0(cSSXAudio* self, void* obj, int ev);

extern "C" void func_0029B738(void* self)
{
    if (cSSXAudio_ReplayMode(self) != 0) return;
    // PORT: cSSXAudio_GetFocusRider returns a rider pointer as int
    char* localRider = (char*)cSSXAudio_GetFocusRider(self, -1);
    if (*(int*)(localRider + 0x870) == *(int*)((char*)self + 0x5820)) {
        if (*(int*)(localRider + 0x2F4) == 10 && *(int*)((char*)self + 0x581C) != *(int*)(localRider + 0x2F4) &&
            *(int*)((char*)cAudioInterface_getGame() + 0x214) == 4) {
            func_002A3DE0((cSSXAudio*)self, localRider, 2);
        }
    } else {
        *(int*)((char*)self + 0x5820) = *(int*)(localRider + 0x870);
    }
    *(int*)((char*)self + 0x581C) = *(int*)(localRider + 0x2F4);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_LandedMonsterCombo);
#ifdef SKIP_ASM
extern "C" void func_002A3DE0(cSSXAudio* self, void* obj, int ev);

/**
 * Reports a landed monster combo by a local human rider: func_002A3DE0(rider, 8).
 *
 * @param obj  the rider
 */
extern "C" void cSSXAudio_LandedMonsterCombo(cSSXAudio* self, void* obj)
{
    int ok = 0;
    if (*(int*)((char*)obj + 0x874) != 0) {
        ok = *(int*)((char*)obj + 0x87C) != 0;
    }
    if (ok) {
        func_002A3DE0(self, obj, 8);
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029B818);

//100%
INCLUDE_ASM("sound/soundsys", func_0029B960__FPv);
#ifdef SKIP_ASM
void func_0029B960(void* self)
{
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_BindingsOn);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndB968Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndB968E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndB968Sub {
    char pad0[4];
    sSndB968Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndB968Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndB968E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndB968Mgr {
    char pad0[0x1D8];
    sSndB968Sub sub;         // 0x1D8
};
struct sSndB968Sys {
    sSndB968Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndB968Mgr** p118;      // 0x118
};

extern "C" int func_00288A20(void* self, void* obj);

#define SND_B968_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_B968_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int sndB968AddVoice(void* vself, void* obj, int a, int b, int type)
{
    sSndB968Sys* self = (sSndB968Sys*)vself;
    sSndB968Mgr* m = self->mgr;
    sSndB968Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndB968Mgr* m2 = *self->p118;
    sSndB968Sub* s2 = &m2->sub;
    s2->vals[m2->sub.cur] = 0x7F;
    s2->ch[m2->sub.cur].vol = SND_B968_MAX(0, SND_B968_MIN(s2->vals[s2->cur], 0x7F));
    sSndB968Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    sSndB968Mgr* m3 = *self->p118;
    m3->sub.tgt[m3->sub.cur] = (char*)obj + 0x110;
    pp = self->p118;
    r = func_00288A20(self, obj);
    (*pp)->sub.p64[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

/**
 * Plays the bindings-on sound (bank 5, patch 0x50) at a local human rider.
 *
 * @param obj  the rider
 */
extern "C" void cSSXAudio_BindingsOn(cSSXAudio* self, void* obj)
{
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        sndB968AddVoice(self, obj, 0, 80, 5);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_BindingsOff);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndBCF8Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndBCF8E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndBCF8Sub {
    char pad0[4];
    sSndBCF8Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndBCF8Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndBCF8E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndBCF8Mgr {
    char pad0[0x1D8];
    sSndBCF8Sub sub;         // 0x1D8
};
struct sSndBCF8Sys {
    sSndBCF8Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndBCF8Mgr** p118;      // 0x118
};

extern "C" int func_00288A20(void* self, void* obj);

#define SND_BCF8_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_BCF8_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int sndBCF8AddVoice(void* vself, void* obj, int a, int b, int type)
{
    sSndBCF8Sys* self = (sSndBCF8Sys*)vself;
    sSndBCF8Mgr* m = self->mgr;
    sSndBCF8Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndBCF8Mgr* m2 = *self->p118;
    sSndBCF8Sub* s2 = &m2->sub;
    s2->vals[m2->sub.cur] = 0x7F;
    s2->ch[m2->sub.cur].vol = SND_BCF8_MAX(0, SND_BCF8_MIN(s2->vals[s2->cur], 0x7F));
    sSndBCF8Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    sSndBCF8Mgr* m3 = *self->p118;
    m3->sub.tgt[m3->sub.cur] = (char*)obj + 0x110;
    pp = self->p118;
    r = func_00288A20(self, obj);
    (*pp)->sub.p64[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

/**
 * Plays the bindings-off sound (bank 5, patch 0x51) at a local human rider.
 *
 * @param obj  the rider
 */
extern "C" void cSSXAudio_BindingsOff(cSSXAudio* self, void* obj)
{
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        sndBCF8AddVoice(self, obj, 0, 81, 5);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_UndoBindings);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndC088Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndC088E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndC088Sub {
    char pad0[4];
    sSndC088Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndC088Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndC088E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndC088Mgr {
    char pad0[0x1D8];
    sSndC088Sub sub;         // 0x1D8
};
struct sSndC088Sys {
    sSndC088Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndC088Mgr** p118;      // 0x118
};

extern "C" int func_00288A20(void* self, void* obj);

#define SND_C088_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_C088_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int sndC088AddVoice(void* vself, void* obj, int a, int b, int type)
{
    sSndC088Sys* self = (sSndC088Sys*)vself;
    sSndC088Mgr* m = self->mgr;
    sSndC088Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndC088Mgr* m2 = *self->p118;
    sSndC088Sub* s2 = &m2->sub;
    s2->vals[m2->sub.cur] = 0x7F;
    s2->ch[m2->sub.cur].vol = SND_C088_MAX(0, SND_C088_MIN(s2->vals[s2->cur], 0x7F));
    sSndC088Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    sSndC088Mgr* m3 = *self->p118;
    m3->sub.tgt[m3->sub.cur] = (char*)obj + 0x110;
    pp = self->p118;
    r = func_00288A20(self, obj);
    (*pp)->sub.p64[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

/**
 * Plays the undo-bindings sound (bank 5, patch 0x52) at a local human rider.
 *
 * @param obj  the rider
 */
extern "C" void cSSXAudio_UndoBindings(cSSXAudio* self, void* obj)
{
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        sndC088AddVoice(self, obj, 0, 82, 5);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029C418__FPv);
#ifdef SKIP_ASM
void func_0029C418(void* self)
{
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_0029C420);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndC420Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndC420E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndC420Sub {
    char pad0[4];
    sSndC420Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndC420Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndC420E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndC420Mgr {
    char pad0[0x1D8];
    sSndC420Sub sub;         // 0x1D8
};
struct sSndC420Sys {
    sSndC420Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndC420Mgr** p118;      // 0x118
};

static inline int sndC420AddVoice(void* vself, char* mon, int a, int b, int type)
{
    sSndC420Sys* self = (sSndC420Sys*)vself;
    sSndC420Mgr* m = self->mgr;
    sSndC420Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndC420Mgr** pp = *(sSndC420Mgr***)mon;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int func_002B49E0(void*);
extern "C" void func_002B3D48(void*, float);
extern "C" void func_0028D488(void* self);
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_002B3A70(void*);

extern "C" void func_0029C420(void* self, int mode)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return;
    }
    char* mon = (char*)self + 0x118;
    sndC420AddVoice(self, mon, 0, 78, 5);
    if (mode == 3) {
        int st = func_002B49E0((char*)self + 0x118);
        if (st == 201 || st == 1 || st == 2 || st == 3) {
            if (*(int*)((char*)self + 0x627C) != 0) {
                func_002B3D48((char*)self + 0x118, 2.0f);
            }
        }
    }
    if (mode == 1) {
        int st = func_002B49E0((char*)self + 0x118);
        if (*(int*)((char*)self + 0x530) == 0 || st == 201 || st == 1 || st == 2 || st == 3) {
            if (*(int*)((char*)self + 0x627C) != 0) {
                func_0028D488(self);
                func_0028CF98(self, 0, 0, -1, 0);
                func_002B3A70((char*)self + 0x118);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_0029C7B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void* cAudioInterface_getGame();
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
void func_002B3A98_C7B0(void* p) __asm__("func_002B3A98");
void func_0028DF08_C7B0(void* p) __asm__("func_0028DF08__FPv");
void func_002B1758(void* self, int a1);
int func_002B4908(void* self);
extern "C" void func_002A3170(void* self);
extern "C" void func_002A3400(cSSXAudio* self);
extern "C" int func_002A4078(void* self);
extern "C" int func_002A40E8(void* self);
extern "C" void cSSXAudio_PlayHistoryUpdateTime(cSSXAudio* self, int a, int b, int c);
extern signed char D_00535C11[];

struct sSnd7B0Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd7B0E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd7B0Sub {
    char pad0[4];
    sSnd7B0Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd7B0Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd7B0E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd7B0Mgr {
    char pad0[0x1D8];
    sSnd7B0Sub sub;         // 0x1D8
};
struct sSnd7B0Sys {
    sSnd7B0Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSnd7B0Mgr** p118;      // 0x118
};
struct sSs7B0PreC { char pad[0xC]; };
class sSs7B0Self : public sSs7B0PreC {
public:
    virtual void v01(int a, int b);
};
struct sSs7B0Pre118 { char pad[0x5440]; };
class sSs7B0Obj : public sSs7B0Pre118 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04(int a);
};

static inline int snd7B0Add(void* vself, int a, int b)
{
    sSnd7B0Sys* self = (sSnd7B0Sys*)vself;
        sSnd7B0Mgr* m = self->mgr;
        sSnd7B0Sub* s = &m->sub;
        m->sub.cur++;
        s->ch[m->sub.cur] = m->sub.def;
        m->sub.tgt[m->sub.cur] = 0;
        s->p34[m->sub.cur] = 0;
        s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
        s->p3C[m->sub.cur] = 0;
        func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
        sSnd7B0Mgr* m3 = *self->p118;
        sSnd7B0Sub* s3 = &m3->sub;
        s3->vals[m3->sub.cur] = 0x7F;
        s3->ch[m3->sub.cur].vol = (s3->vals[s3->cur] < 0x7F && s3->vals[s3->cur] < 0) ? 0 : (s3->vals[s3->cur] < 0x7F ? (unsigned char)s3->vals[s3->cur] : 0x7F);
        sSnd7B0Mgr** pp = self->p118;
        int r = (int)func_00287968(self, 9, 0);
        (*pp)->sub.p3C[(*pp)->sub.cur] = r;
        return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" void func_0029C7B0(void* vself)
{
    sSnd7B0Sys* self = (sSnd7B0Sys*)vself;
    char* base = (char*)vself;
    func_002B3A98_C7B0(base + 0x118);
    func_0028DF08_C7B0(vself);
    *(int*)(base + 0x582C) = *(int*)(*(int*)((char*)cAudioInterface_getGame() + 0xC) + 8);
    ((sSs7B0Self*)vself)->v01(0, 98);
    if (cSSXAudio_ReplayMode(vself) == 0) {
        snd7B0Add(vself, 0, 94);
    }
    if (*(int*)(base + 0x627C) != 0) {
        ((sSs7B0Obj*)(base + 0x118))->v04(0);
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cSSXAudio_PlayHistoryExists((cSSXAudio*)vself, 0, 0) == 0) {
        int m = D_00535C11[0];
        if (m == 1) {
            func_002A3170(vself);
        } else if (m == 2) {
            func_002A3170(vself);
        } else {
            int* p = (int*)SSXApp->GameModeMan;
            if (p[152 / 4] != 0) {
                func_002A3400((cSSXAudio*)vself);
                func_002B1758(base + 0x5560, 0);
                if (func_002A4078(vself) != 0 || func_002A40E8(vself) != 0) {
                    *(int*)(base + 0x5774) = 1;
                    *(int*)(base + 0x5778) = func_002B4908(base + 0x118);
                }
            } else {
                int v = p[0];
                if (v == 1) {
                    *(int*)(base + 0x5774) = 1;
                    *(int*)(base + 0x5778) = func_002B4908(base + 0x118);
                } else if (v == 2) {
                    if (func_002A4078(vself) != 0 || func_002A40E8(vself) != 0) {
                        *(int*)(base + 0x5774) = 1;
                        *(int*)(base + 0x5778) = func_002B4908(base + 0x118);
                    }
                }
            }
        }
    }
    if (*(int*)(base + 0x627C) == 0) {
        if (cSSXAudio_ReplayMode(vself) == 0) {
            *(int*)(base + 0x627C) = 1;
            ((sSs7B0Obj*)(base + 0x118))->v04(37);
        } else if (*(int*)(base + 0x6298) != 0) {
            ((sSs7B0Obj*)(base + 0x118))->v04(37);
        }
    }
    cSSXAudio_PlayHistoryUpdateTime((cSSXAudio*)vself, 0, *(volatile int*)(**(char***)(base + 0x118) + 0x260) + 50, 0);
    *(int*)(base + 0x5824) = *(int*)SSXApp->GameModeMan;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UpdateTimedEvent);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" void* cAudioInterface_getAI();
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" void func_002ADCA0(void*, int, long, int, int, int, int, int);
// PORT: func_004139F8 is libgcc's float -> unsigned conversion (__fixunssfsi).
extern "C" int func_004139F8(float f);
extern int D_005308D0[];
extern sSndKey8 D_004A3678[];

/**
 * Plays the countdown beep near the end of a timed event.
 *
 * Only when the game mode is timed (GameModeMan + 0x88) and D_005308D0 bit 9 is clear, never during a replay.
 * Beeps once at 10, 8 and 6 seconds left and every second from 4 down to 1.
 */
// PORT: 64-bit long sound key
extern "C" void cSSXAudio_UpdateTimedEvent(cSSXAudio* self)
{
    if (cSSXAudio_ReplayMode(((char*)self)) != 0) {
        return;
    }
    char* gameModeMan = (char*)SSXApp->GameModeMan;
    if (gameModeMan == 0) {
        return;
    }
    int ok = 0;
    if (*(int*)(gameModeMan + 0x88) != 0) {
        ok = ((D_005308D0[0] >> 9) & 1) ^ 1;
    }
    if (!ok) {
        return;
    }
    int elapsedSec = (int)((float)*(int*)((char*)cAudioInterface_getAI() + 0xC) * 0.01666666753590107f);
    int secsLeft = (func_004139F8((float)*(unsigned int*)(gameModeMan + 0x78) * 0.01666666753590107f) - elapsedSec) % 3600;
    if (secsLeft <= 0) {
        return;
    }
    if (secsLeft != self->mLastTime && secsLeft < 11 && (secsLeft == 10 || secsLeft == 8 || secsLeft == 6 || secsLeft < 5)) {
        char* bankMon = *(char**)self->MusicSys.mpVStreamSys + 0x1D8;
        sSndKey8* key = D_004A3678;
        int volumePtr = (int)func_00287968(((char*)self), 9, 0);
        func_002ADCA0(bankMon, 450, key->v, volumePtr, 0, 78, 0x7F, 0);
    }
    self->mLastTime = secsLeft;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_PauseSFX);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_Pause(void*);

/**
 * Pauses all sound effects in the bank monitor, once.
 */
extern "C" void cSSXAudio_PauseSFX(cSSXAudio* self)
{
    if (self->mSFXPaused == 0) {
        self->mSFXPaused = 1;
        *(int*)((char*)*(void**)self->MusicSys.mpVStreamSys + 0x26C) = 1;
        cBankMonitor_BANKMONITOR_Pause((char*)*(void**)self->MusicSys.mpVStreamSys + 0x1D8);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_ResumeSFX);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_Resume(void* self);
extern "C" void cBankMonitor_BANKMONITOR_CameraChange(void* self);
extern "C" void func_002B6A68(void* self);
extern void* gWorldTriggerManager;

/**
 * Resumes the paused sound effects and lets the bank monitor and world triggers catch up with the camera.
 */
extern "C" void cSSXAudio_ResumeSFX(cSSXAudio* self)
{
    if (self->mSFXPaused != 0) {
        self->mSFXPaused = 0;
        cBankMonitor_BANKMONITOR_Resume(*(char**)self->MusicSys.mpVStreamSys + 0x1D8);
        *(int*)(*(char**)self->MusicSys.mpVStreamSys + 0x26C) = 0;
        cBankMonitor_BANKMONITOR_CameraChange(*(char**)self->MusicSys.mpVStreamSys + 0x1D8);
        func_002B6A68(gWorldTriggerManager);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayIconSound);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndCED8Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndCED8E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndCED8Sub {
    char pad0[4];
    sSndCED8Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndCED8Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndCED8E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndCED8Mgr {
    char pad0[0x1D8];
    sSndCED8Sub sub;         // 0x1D8
};
struct sSndCED8Sys {
    sSndCED8Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndCED8Mgr** p118;      // 0x118
};

static inline int sndCED8AddVoice(void* vself, int a, int b, int type)
{
    sSndCED8Sys* self = (sSndCED8Sys*)vself;
    sSndCED8Mgr* m = self->mgr;
    sSndCED8Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndCED8Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" void func_002A3EB8(cSSXAudio* self, void* obj, int n);

/**
 * Plays the sound for a collected icon, and reports multiplier pickups.
 *
 * Icon types 0-3 play patches 0x70, 0x71, 0x75 and 0x76 (others 0x74). For type 2 a multiplier of 2, 3, 5 or
 * 10
 * also calls func_002A3EB8() with 1, 2, 4 or 8.
 *
 * @param kind  icon type (On Tour eIconType)
 * @param obj  the rider, or 0; a rider that is not a local human plays nothing
 * @param x  the icon's multiplier
 */
extern "C" void cSSXAudio_PlayIconSound(cSSXAudio* self, int kind, void* obj, float x)
{
    if (obj != 0) {
        bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
        if (!ok) {
            return;
        }
    }
    int id = 116;
    if (kind == 0) {
        id = 0x70;
    } else if (kind == 1) {
        id = 113;
    } else if (kind == 2) {
        id = 117;
    } else if (kind == 3) {
        id = 118;
    }
    sndCED8AddVoice(self, 0, id, 9);
    if (kind == 2) {
        if (x == 2.0f) {
            func_002A3EB8(self, obj, 1);
        } else if (x == 3.0f) {
            func_002A3EB8(self, obj, 2);
        } else if (x == 5.0f) {
            func_002A3EB8(self, obj, 4);
        } else if (x == 10.0f) {
            func_002A3EB8(self, obj, 8);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UpdateTrackBanks);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getAI();
extern "C" void func_0029D370(void* self);
extern "C" void cSSXAudio_StopTone(cSSXAudio* self, float v);
extern "C" void cBankMonitor_BANKMONITOR_StopBank(void* self, int id, float v);

static inline int sndIsOn(char* t)
{
    return *(int*)t == 1;
}

/**
 * Follows the first rider's ground and switches the track sound banks when it changes.
 *
 * When the low byte of its mGroundPolyId differs from mLastPatchRID, a ground material below 22 calls
 * func_0029D370(); other materials stop the tone and both track banks with a 5-second fade.
 */
extern "C" void cSSXAudio_UpdateTrackBanks(cSSXAudio* self)
{
    char* rider = *(char**)((char*)cAudioInterface_getAI() + 0x28);
    int patchRID = *(int*)(rider + 0x430);
    int ok = patchRID != -1;
    if (ok) {
        if ((patchRID & 0xFF) != *(unsigned char*)&self->mLastPatchRID) {
            self->mLastPatchRID = (unsigned int)patchRID;
            if (*(int*)(rider + 0x434) < 22) {
                func_0029D370(self);
            } else {
                cSSXAudio_StopTone(self, 5.029983997344971f);

                char* bankMgr = *(char**)self->MusicSys.mpVStreamSys;
                char* bm = bankMgr + 0x1D8;
                char* cfg = *(char**)(bankMgr + 0xACC);
                int id = sndIsOn(cfg + 0x300);
                if (id) id = *(int*)(cfg + 0x304); else id = -1;
                cBankMonitor_BANKMONITOR_StopBank(bm, id, 5.029983997344971f);

                char* bankMgr2 = *(char**)self->MusicSys.mpVStreamSys;
                char* bm2 = bankMgr2 + 0x1D8;
                char* cfg2 = *(char**)(bankMgr2 + 0xACC);
                int id2 = sndIsOn(cfg2 + 0x360);
                if (id2) id2 = *(int*)(cfg2 + 0x364); else id2 = -1;
                cBankMonitor_BANKMONITOR_StopBank(bm2, id2, 5.029983997344971f);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_0029D370);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndD370Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndD370E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndD370Sub {
    char pad0[4];
    sSndD370Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndD370Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndD370E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndD370Mgr {
    char pad0[0x1D8];
    sSndD370Sub sub;         // 0x1D8
};
struct sSndD370Sys {
    sSndD370Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndD370Mgr** p118;      // 0x118
};

static inline int sndD370AddVoice(void* vself, int a, int b)
{
    sSndD370Sys* self = (sSndD370Sys*)vself;
    sSndD370Mgr* m = self->mgr;
    sSndD370Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndD370Mgr** pp = self->p118;
    int r = (int)func_00287968(self, 5, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" void func_0029D370(void* self)
{
    if (*(int*)((char*)self + 0x6C74) != 0) {
        *(int*)((char*)self + 0x5FC8) = sndD370AddVoice(self, 9, 0);
    }
    *(int*)((char*)self + 0x5FC4) = 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_UpdateTone);
#ifdef SKIP_ASM
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" int func_003B8AB8(int h);
extern "C" void cSSXAudio_StopTone(cSSXAudio* self, float v);
extern "C" void func_0029D370(void* self);

/**
 * Restarts the track tone (func_0029D370) once the playing tone voice has finished.
 */
extern "C" void cSSXAudio_UpdateTone(cSSXAudio* self)
{
    if (self->mPlayingTone != 0) {
        func_003B58A0();
        int done = func_003B8AB8(self->mToneHandle) != 0;
        func_003B58D8();
        if (done) {
            cSSXAudio_StopTone(self, 0.0f);
            func_0029D370(self);
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
extern "C" void cSSXAudio_StopTone(cSSXAudio* self, float FadeTime)
{
    if (self->mPlayingTone != 0) {
        int idx = self->mToneHandle;
        if (idx >= 0) {
            cBankMonitor_BANKMONITOR_StopHandle((char*)*(void**)self->MusicSys.mpVStreamSys + 0x1D8, idx, 1, FadeTime);
            self->mToneHandle = -1;
        }
        self->mPlayingTone = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029D6D0__FPv);
#ifdef SKIP_ASM
void func_0029D6D0(void* self)
{
    *(int*)((char*)self + 0x5FD8) = 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029D6E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: a non-empty `do { ... } while (0)` around the final monitor block, and a second pointer local `mon` (self + 0x118, the same address as `bank`) for the vt[4] call; plain form 97.52% UNPOSITIONED (99.61% without the wrapper only, 96.72% UNPOSITIONED without `mon` only). Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
// OPTS_Callback handlers handed to cSndPlayOpts: 8-byte pointer-to-member constants.
extern sSndCallback D_004A3648[];

extern sSndCallback D_004A3650_D6E0[] __asm__("D_004A3650");

extern sSndCallback D_004A3658[];

// g++ 2.95 vtable entry for a method taking (self, int).
struct sSndVtVI_D6E0 { short delta; short index; void (*fn)(void*, int); };

extern "C" void* cAudioInterface_getAI();

extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);

extern "C" void func_0028D488(void* self);

extern "C" void cSSXAudio_EndUberSpin_D6E0(void* self, void* obj) __asm__("cSSXAudio_EndUberSpin");

extern "C" int func_002B49E0(void*);

// cBE interface lookup: cBE_getInterface__Fv takes (be, kind) despite its mangled name.
extern "C" void* cBE_getBE();

extern "C" void* cBE_getInterface__Fv(void* be, int kind);

extern "C" void func_002ADDE0_D6E0(void* bank, sSndCallback key) __asm__("func_002ADDE0");

extern "C" int func_002B46E8(void* monitor);
extern "C" void func_002B4708(void* monitor);
extern "C" int func_002B4620(void* monitor);
extern "C" int cBEMissionInterface_getAudioType(void* iface, int mission);
extern "C" int func_0028D960(void* self);
extern "C" int func_0028DEF0_D6E0(void* self, int a1) __asm__("func_0028DEF0");

// Mission start: resets the rider sounds, stops the front-end bank loops and picks the mission's music track.
extern "C" void func_0029D6E0(char* self, int mission)
{
    char* bank = self + 0x118;
    *(int*)(self + 0x5FD8) = 0;
    func_00296E20(self, *(void**)((char*)cAudioInterface_getAI() + 0x28));
    func_00297438(self, *(void**)((char*)cAudioInterface_getAI() + 0x28));
    cSSXAudio_EndUberSpin_D6E0(self, 0);
    if (func_002B46E8(bank) != 0) {
        if (func_0028D960(self) != 0) {
            sSndVtVI_D6E0* vt = *(sSndVtVI_D6E0**)(self + 0x5558);
            vt[3].fn(bank + vt[3].delta, 8);
        }
        func_002B4708(bank);
    }
    if (*(int*)(self + 0x608C) == 2) {
        return;
    }
    if ((unsigned int)(func_002B49E0(self + 0x118) - 1) < 3) {
        func_002ADDE0_D6E0(**(char***)(self + 0x118) + 0x1D8, D_004A3648[0]);
        func_002ADDE0_D6E0(**(char***)(self + 0x118) + 0x1D8, D_004A3650_D6E0[0]);
        func_002ADDE0_D6E0(**(char***)(self + 0x118) + 0x1D8, D_004A3658[0]);
        func_0028DEF0_D6E0(self, 18);
        func_0028D488(self);
        func_0028CF98(self, 0, 0, -1, 0);
        return;
    }
    int track = -1;
    int kind = cBEMissionInterface_getAudioType(cBE_getInterface__Fv(cBE_getBE(), 10), mission);
    *(int*)(self + 0x5FD0) = kind;
    switch (kind) {
    case 1: track = 33; break;
    case 2: track = 34; break;
    case 3: track = 38; break;
    case 0:
    case 4: track = -1; break;
    }
    do {
        if (func_002B4620(self + 0x118) != 0) {
            sSndVtVI_D6E0* vt = *(sSndVtVI_D6E0**)(self + 0x5558);
            char* mon = self + 0x118;
            ((int (*)(void*, int))vt[4].fn)(mon + vt[4].delta, track);
            *(int*)(self + 0x5FD4) = 1;
        }
        func_002B4708(self + 0x118);
    } while (0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_0029D8E0);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndD8E0Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndD8E0E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndD8E0Sub {
    char pad0[4];
    sSndD8E0Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndD8E0Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndD8E0E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndD8E0Mgr {
    char pad0[0x1D8];
    sSndD8E0Sub sub;         // 0x1D8
};
struct sSndD8E0Sys {
    sSndD8E0Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndD8E0Mgr** p118;      // 0x118
};

static inline int sndD8E0AddVoice(void* vself, int a, int b, int type)
{
    sSndD8E0Sys* self = (sSndD8E0Sys*)vself;
    sSndD8E0Mgr* m = self->mgr;
    sSndD8E0Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndD8E0Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" void* cAudioInterface_getAI();
extern "C" void func_002A3C00(cSSXAudio* self, void* obj, int a2);

struct sSndVtEntD8E0 { short delta; short index; void (*fn)(void*, int); };

extern "C" void func_0029D8E0(void* self)
{
    if (*(int*)((char*)self + 0x5FD4) != 0) {
        char* musicSys = (char*)self + 0x118;
        sSndVtEntD8E0* entry = &(*(sSndVtEntD8E0**)((char*)self + 0x5558))[4];
        entry->fn(musicSys + entry->delta, 39);
        *(int*)((char*)self + 0x5FD0) = 0;
        *(int*)((char*)self + 0x5FD4) = 0;
    }
    sndD8E0AddVoice(self, 0, 109, 9);
    func_002A3C00((cSSXAudio*)self, *(void**)((char*)cAudioInterface_getAI() + 0x28), 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_BigChallengeFailed);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_MenuSound(void* self, int id);
extern "C" void func_00296E20(void* self, void* obj);
extern "C" void func_00297438(void* self, void* obj);
extern "C" void cSSXAudio_EndUberSpin(cSSXAudio* self, void* obj);

struct sSndVtEnt { short delta; short index; void (*fn)(void*, int); };

/**
 * Ends a big-challenge's audio when the challenge is failed or quit.
 *
 * Only acts while the challenge flag at +0x5FD4 is set: unless @p keep is set it plays menu sound 0xE (cSSXAudio_MenuSound),
 * stops the AI rider's wipeout slide and tumble sounds (func_00296E20, func_00297438) and the uber-spin
 * (cSSXAudio_EndUberSpin), calls virtual slot 4 of the object at +0x118 with 0x27, and clears +0x5FD0 and +0x5FD4.
 *
 * @param keep  On Tour's Quit: nonzero skips the failure sound
 */
extern "C" void cSSXAudio_BigChallengeFailed(void* self, int keep)
{
    if (*(int*)((char*)self + 0x5FD4) != 0) {
        if (keep == 0) {
            cSSXAudio_MenuSound(self, 0xE);
        }
        func_00296E20(self, *(void**)((char*)cAudioInterface_getAI() + 0x28));
        func_00297438(self, *(void**)((char*)cAudioInterface_getAI() + 0x28));
        cSSXAudio_EndUberSpin((cSSXAudio*)self, 0);
        char* musicSys = (char*)self + 0x118;
        sSndVtEnt* vtblEntry = &(*(sSndVtEnt**)((char*)self + 0x5558))[4];
        vtblEntry->fn(musicSys + vtblEntry->delta, 39);
        *(int*)((char*)self + 0x5FD0) = 0;
        *(int*)((char*)self + 0x5FD4) = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_HandPlantSound);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndDC48Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndDC48E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndDC48Sub {
    char pad0[4];
    sSndDC48Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndDC48Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndDC48E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndDC48Mgr {
    char pad0[0x1D8];
    sSndDC48Sub sub;         // 0x1D8
};
struct sSndDC48Sys {
    sSndDC48Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndDC48Mgr** p118;      // 0x118
};

static inline int sndDC48AddVoice(void* vself, int a, int b, int type)
{
    sSndDC48Sys* self = (sSndDC48Sys*)vself;
    sSndDC48Mgr* m = self->mgr;
    sSndDC48Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndDC48Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

/**
 * Plays the handplant sound (bank 9, patch 0x61) for a local human rider.
 *
 * @param obj  the rider
 */
extern "C" void cSSXAudio_HandPlantSound(cSSXAudio* self, void* obj)
{
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        sndDC48AddVoice(self, 0, 97, 9);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029DEF0);
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
// handle lists (func_002A4BA8 clears them; func_002A4CA0 picks from one) and their timing floats.
extern "C" void* func_002A4BA8(tCrowdEventData* self);
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
    cSndPlayOpts* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    self->mBank[self->mpVBankManager->sub.cur] = a;
    self->mPatch[self->mpVBankManager->sub.cur] = b;
}

extern "C" int func_0029E438(void* self, void* blk);
struct sQDEF0 {
    int w[4];
} __attribute__((aligned(16)));
struct sV3DEF0 {
    float x, y, z;
};
struct sOutDEF0 {
    sQDEF0 q[2];
};
extern "C" void func_002DA1C0_c(sOutDEF0* out, sV3DEF0* in) __asm__("func_002DA1C0");

extern sSndCallback D_004A36D0_cb[] __asm__("D_004A36D0");
// Grind/ride loop sound: reference-counted start (pushes the voice at the rider's position) and stop.
extern "C" void func_0029DEF0(cSSXAudio* vself, int on)
{
    sSSXAudioView* self = (sSSXAudioView*)vself;
    if (on != 0) {
        if (++self->users == 1) {
            int id = -1;
            if (cSSXAudio_ReplayMode(self) != 0) {
                id = 0;
            }
            char* obj = (char*)cSSXAudio_GetFocusRider(self, id);
            self->idx = sndVCall(obj + 0x6C0, 7);
            {
                sV3DEF0* p = (sV3DEF0*)sndVCall(obj + 0x6C0, 5);
                sOutDEF0 out;
                sV3DEF0 pos;
                pos.x = p->x;
                pos.y = p->y;
                pos.z = p->z;
                func_002DA1C0_c(&out, &pos);
                *(sOutDEF0*)&self->xf = out;
            }
            sndBeginVoice(self, 8, 2);
            cBankManager** p0 = self->p118;
            int v = func_0029E438(self, (char*)&self->xf);
            cBankManager* m3 = *p0;
            cSndPlayOpts* s3 = &m3->sub;
            s3->vals[m3->sub.cur] = v;
            s3->ch[m3->sub.cur].vol = (s3->vals[s3->cur] < 0x7F && s3->vals[s3->cur] < 0) ? 0 : (s3->vals[s3->cur] < 0x7F ? (unsigned char)s3->vals[s3->cur] : 0x7F);
            cBankManager** pp = self->p118;
            int r = (int)func_00287968(self, 5, 0);
            (*pp)->sub.p3C[(*pp)->sub.cur] = r;
            {
                cBankManager* m2 = *self->p118;
                m2->sub.tgt[m2->sub.cur] = &self->xf;
            }
            func_002A9988(&(*self->p118)->sub, 100.0f, -1.0f);
            OPTS_Callback(*self->p118, D_004A36D0_cb[0]);
            {
                cBankManager** pq = self->p118;
                int q = func_00288A20(self, obj);
                (*pq)->sub.p64[(*pq)->sub.cur] = q;
            }
            (*self->p118)->sub.p74[(*self->p118)->sub.cur] = 1;
            self->handle5FF4 = cBankSys_SFX_End((cBankSys*)self);
        }
    } else {
        if (--self->users == 0) {
            cBankMonitor_BANKMONITOR_StopHandle(&(*self->p118)->sub, self->handle5FF4, 1, 2.0f);
            cBankManager* mg = *self->p118;
            sSndVtCb* vt = *(sSndVtCb**)((char*)mg + 0xAB0);
            char* sub = (char*)&mg->sub;
            vt[3].fn(sub + vt[3].delta, D_004A36D0_cb[0]);
            for (int i = 2; i >= 0; i--) {
                self->slots[i] = -1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029E438);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Converts the sound's volume (percent attenuation at 0x14, gain at 0x10) to a 0..127 level.
extern "C" int func_0029E438(void* self, void* snd_)
{
    char* snd = (char*)snd_;
    float volume = (100.0f - *(float*)(snd + 0x14) * 0.01f) * 1.27f * *(float*)(snd + 0x10);
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
INCLUDE_ASM("sound/soundsys", func_0029E4A0);
#ifdef SKIP_ASM
extern "C" int func_0029E438(void *, void *);
extern "C" void func_002DA1C0(void *, float *);

struct sVEntryP_29E4A0 { short delta; short index; float* (*fn)(void*); };

extern "C" void func_0029E4A0(cSSXAudio* ssxAudio, void *voice) {
    if (ssxAudio->mpAvalancheRiderIndex < *(int *)((char*)cAudioInterface_getAI() + 0x78)) {
        tAvalanchePositionalData positional;
        float riderPos[3];
        tAvalanchePositionalData* dst = &(*(&ssxAudio->mAvalancheData));
        cRider* rider = *(cRider**)((char*)cAudioInterface_getAI() + (ssxAudio->mpAvalancheRiderIndex << 2) + 0x28);
        char* obj = (char*)&rider->mVtbl;
        sVEntryP_29E4A0* vt = *(sVEntryP_29E4A0**)obj;
        float* pos = vt[kRiderVt_getPosition].fn(obj + vt[kRiderVt_getPosition].delta);
        riderPos[0] = pos[0];
        riderPos[1] = pos[1];
        riderPos[2] = pos[2];
        func_002DA1C0(&positional, riderPos);
        (*(&ssxAudio->mAvalancheData)) = positional;
        *(int *)((char*)voice + 0x64) = func_0029E438((ssxAudio), dst);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029E560__FPv);
#ifdef SKIP_ASM
void func_0029E560(void* self)
{
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029E568);
#ifdef SKIP_ASM
extern "C" void func_0029E568(void) {

}
#endif
//100%
INCLUDE_ASM("sound/soundsys", func_0029E570);
#ifdef SKIP_ASM
extern "C" void func_0029E570(void) {

}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029E578);
#ifdef SKIP_ASM
extern "C" void func_0029E578(void* self, void* rider)
{
    int idx = *(int*)((char*)rider + 0x870);
    self = (char*)self + idx * 4;
    *(int*)((char*)self + 0x6080) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayWipeoutRecoverHit);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndE590Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndE590E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndE590Sub {
    char pad0[4];
    sSndE590Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndE590Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndE590E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndE590Mgr {
    char pad0[0x1D8];
    sSndE590Sub sub;         // 0x1D8
};
struct sSndE590Sys {
    sSndE590Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndE590Mgr** p118;      // 0x118
};

extern "C" int func_00288A20(void* self, void* obj);

#define SND_E590_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_E590_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int sndE590AddVoice(void* vself, int a, int b, int type, unsigned short pitch)
{
    sSndE590Sys* self = (sSndE590Sys*)vself;
    sSndE590Mgr* m = self->mgr;
    sSndE590Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndE590Mgr* m2 = *self->p118;
    sSndE590Sub* s2 = &m2->sub;
    s2->vals[m2->sub.cur] = 0x7F;
    s2->ch[m2->sub.cur].vol = SND_E590_MAX(0, SND_E590_MIN(s2->vals[s2->cur], 0x7F));
    sSndE590Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    sSndE590Mgr* m3 = *self->p118;
    *(unsigned short*)((char*)&m3->sub.ch[m3->sub.cur] + 0xC) = pitch;
    return cBankSys_SFX_End((cBankSys*)self);
}


extern "C" int cSSXAudio_ReplayMode(void* self);

static inline char* sndE590Tbl(void* self)
{
    return (char*)self + 0x6080;
}

/**
 * Plays a wipeout-recovery button hit for a local human rider, pitched up as the recovery progresses.
 *
 * The pitch uses the lower of @p x and the previous value for that view; @p x is then stored. Not during a
 * replay.
 *
 * @param obj  the rider
 * @param x  recovery progress (On Tour percent)
 */
extern "C" void cSSXAudio_PlayWipeoutRecoverHit(cSSXAudio* self, void* obj, float x)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return;
    }
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        int idx = *(int*)((char*)obj + 0x870);
        float prev = *(float*)(sndE590Tbl(self) + (idx << 2));
        unsigned short pitch = (int)((x <? prev) * 5.0f) * 80 + 0x1000;
        sndE590AddVoice(self, 0, 95, 9, pitch);
        *(float*)(sndE590Tbl(self) + (idx << 2)) = x;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayWipeoutRecoverFull);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sSndE970Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSndE970E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSndE970Sub {
    char pad0[4];
    sSndE970Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSndE970Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSndE970E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSndE970Mgr {
    char pad0[0x1D8];
    sSndE970Sub sub;         // 0x1D8
};
struct sSndE970Sys {
    sSndE970Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    sSndE970Mgr** p118;      // 0x118
};

extern "C" int func_00288A20(void* self, void* obj);

#define SND_E970_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_E970_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline int sndE970AddVoice(void* vself, int a, int b, int type)
{
    sSndE970Sys* self = (sSndE970Sys*)vself;
    sSndE970Mgr* m = self->mgr;
    sSndE970Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    sSndE970Mgr* m2 = *self->p118;
    sSndE970Sub* s2 = &m2->sub;
    s2->vals[m2->sub.cur] = 0x7F;
    s2->ch[m2->sub.cur].vol = SND_E970_MAX(0, SND_E970_MIN(s2->vals[s2->cur], 0x7F));
    sSndE970Mgr** pp = self->p118;
    int r = (int)func_00287968(self, type, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    return cBankSys_SFX_End((cBankSys*)self);
}

extern "C" int cSSXAudio_ReplayMode(void* self);

static inline char* sndE970Tbl(void* self)
{
    return (char*)self + 0x6080;
}

/**
 * Plays the full-recovery sound once per recovery for a local human rider, except during a replay.
 *
 * @param obj  the rider
 */
extern "C" void cSSXAudio_PlayWipeoutRecoverFull(cSSXAudio* self, void* obj)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return;
    }
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        int idx = *(int*)((char*)obj + 0x870);
        if (*(float*)(sndE970Tbl(self) + (idx << 2)) == 1.0f) {
            return;
        }
        sndE970AddVoice(self, 0, 0x60, 9);
        *(float*)(sndE970Tbl(self) + (idx << 2)) = 1.0f;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_0029ED18);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getAI();
extern "C" void cSSXAudio_EndUberSpin(cSSXAudio* self, void* obj);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

extern "C" void func_0029ED18(void* self)
{
    func_00296E20(self, *(void**)((char*)cAudioInterface_getAI() + 0x28));
    func_00297438(self, *(void**)((char*)cAudioInterface_getAI() + 0x28));
    cSSXAudio_EndUberSpin((cSSXAudio*)self, 0);
    if (*(int*)((char*)self + 0x59E4) > 0) {
        cBankMonitor_BANKMONITOR_StopHandle(**(char***)((char*)self + 0x118) + 0x1D8, *(int*)((char*)self + 0x59E0), 1, 0.0f);
        *(int*)((char*)self + 0x59E4) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029ED90);
#ifdef SKIP_ASM
extern "C" int func_0029F198(void* self);
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" int BXFILE_exists(const char* name);
extern "C" void SPCH_Init(void* self);
extern "C" void func_002B0DA0(void* self);
extern "C" void func_002B0E60(void* self, int a, int b, char* name, int c, int d, int e);
extern "C" void func_003DED50(const char* name, int a, int b, void** h);
extern "C" void func_003DEDC0(void* h, int a);
extern char str_data_audio_french_big[];
extern char str_data_audio_german_big[];
extern char str_data_audio_spanish_big[];
extern char str_data_audio_english_big[];
extern char str_CHAR[];

static inline void sndStreamLoad(const char* name, void** h)
{
    if (BXFILE_exists(name)) {
        func_003DED50(name, 0, 100, h);
    }
}

extern "C" void func_0029ED90(char* self, int restart)
{
    int mode = func_0029F198(self);
    if (mode == *(int*)(self + 0x572C)) {
        return;
    }
    if (restart) {
        func_002B0DA0(self + 0x5560);
    }
    void** streamHandle = (void**)(self + 0x5728);
    if (*streamHandle) {
        func_003DEDC0(*streamHandle, 100);
    }
    if (mode == 2) {
        sndStreamLoad(str_data_audio_french_big, streamHandle);
    } else if (mode == 4) {
        sndStreamLoad(str_data_audio_german_big, streamHandle);
    } else if (mode == 8) {
        sndStreamLoad(str_data_audio_spanish_big, streamHandle);
    } else {
        sndStreamLoad(str_data_audio_english_big, streamHandle);
    }
    *(int*)(self + 0x572C) = mode;
    if (restart) {
        SPCH_Init(self + 0x5560);
        func_002B0E60(self + 0x5560, 0, 0, str_CHAR, (int)func_00287968(self, 4, 0), 1, -1);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029EEE0);
#ifdef SKIP_ASM
extern "C" int cAudioInterface_isInGame();

extern "C" int func_0029EEE0(void* self, int instanceIndex, const char* name)
{
    if (instanceIndex == 0) {
        if (name[0] == 'D' && name[1] == 'J' && name[2] == '_') {
            if (cAudioInterface_isInGame() != 0) {
                *(int*)((char*)self + 0x5738) = 2;
            } else {
                *(int*)((char*)self + 0x5738) = 11;
            }
        } else if (name[0] == 'P' && name[1] == 'A' && name[2] == '_') {
            *(int*)((char*)self + 0x5738) = 3;
        } else if (name[0] == 'A' && name[1] == 'r' && name[2] == 'c' && name[3] == 'a' && name[4] == 'd' && name[5] == 'e') {
            *(int*)((char*)self + 0x5738) = 10;
        } else {
            *(int*)((char*)self + 0x5738) = 4;
        }
        return *(int*)((char*)self + 0x5738);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F000);
#ifdef SKIP_ASM
extern "C" void func_00287F00(void* self, int a1, float f0, float f1);

extern "C" void* func_0029F000(cSSXAudio* self, int instanceIndex, int type, int a3)
{
    if (instanceIndex == 0) {
        if (type == 2) {
            func_00287F00(self, 0, 0.9999024868011475f, 0.6499993801116943f);
        }
        if (type == 11) {
            return (char*)self + 0x5730;
        }
        return func_00287968(self, type, a3);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F088);
#ifdef SKIP_ASM
extern "C" int func_0029F088(void* self, int instanceIndex)
{
    if (instanceIndex == 0) {
        return *(int*)((char*)self + 0x5738);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F0A0);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int ch);

extern "C" int func_0029F0A0(void* self)
{
    if (func_00287920(self, 4) == 0.0f) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F0D8);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" float func_00287920(void* self, int ch);

extern "C" int func_0029F0D8(void* self)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        return 0;
    }
    if (func_00287920(self, 2) == 0.0f) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F128);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int ch);

extern "C" int func_0029F128(void* self)
{
    if (func_00287920(self, 3) == 0.0f) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F160);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int ch);

extern "C" int func_0029F160(void* self)
{
    if (func_00287920(self, 10) == 0.0f) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F198);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

struct sSsOpts9F198 {
    unsigned int w0;
    int data[0x288 / 4 - 1];
};
extern sSsOpts9F198 D_00535610_ss9F198 __asm__("cBEOptionInterface_mOption");

extern "C" int func_0029F198(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 4);
    sSsOpts9F198 o = D_00535610_ss9F198;
    unsigned m = o.w0 >> 22;
    m &= 7;
    switch (m) {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 4;
    case 3:
        return 8;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F2B0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: the unit declares this as void(int, int); defined under an asm label with its real type
extern "C" int func_0029F2B0_impl(unsigned int idx, int flag) __asm__("func_0029F2B0");
extern "C" int func_0029F2B0_impl(unsigned int idx, int flag)
{
    if (flag == 0) {
        switch (idx) {
        case 6: return 1;
        case 3: return 2;
        case 0: return 4;
        case 8: return 8;
        case 1: return 0x10;
        case 4: return 0x20;
        case 9: return 0x80;
        case 7: return 0x40;
        case 5: return 0x100;
        case 2: return 0x200;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_0029F330);
#ifdef SKIP_ASM
extern "C" int cRider_getIdentity(void*);
extern "C" int cRider_getCheatIdentity(void*);
extern "C" void func_0029F2B0(int, int);

extern "C" void func_0029F330(void* self)
{
    int identity = cRider_getIdentity(self);
    func_0029F2B0(identity, cRider_getCheatIdentity(self));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F378);
#ifdef SKIP_ASM
extern "C" int func_0029F378(void* self, void* obj)
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
INCLUDE_ASM("sound/soundsys", func_0029F3F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_0028BC58(void* bank, int slot, char* name, int a3);
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

extern "C" void func_0029F3F8(cSSXAudio* self)
{
    char buf[64];
    switch (func_0029F378(((char*)self), *(void**)((char*)cAudioInterface_getAI() + 0x28))) {
    case 6: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_eli_bnk); break;
    case 3: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_mac_bnk); break;
    case 0: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_mob_bnk); break;
    case 8: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_psy_bnk); break;
    case 1: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_kao_bnk); break;
    case 4: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_zoe_bnk); break;
    case 9: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_vig_bnk); break;
    case 7: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_nat_bnk); break;
    case 5: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_grf_bnk); break;
    case 2: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_ari_bnk); break;
    default: sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_none_bnk); break;
    }
    func_0028BC58(*(void**)self->MusicSys.mpVStreamSys, 0xE, buf, 0);
    sprintf(buf, str_pct_spct_s_c, str_data_audio, str_GRNT_AI_bnk);
    func_0028BC58(*(void**)self->MusicSys.mpVStreamSys, 0xF, buf, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F5E0);
#ifdef SKIP_ASM
extern "C" void func_0028BCE8(void* self, int i);

extern "C" void func_0029F5E0(cSSXAudio* self)
{
    func_0028BCE8(*(void**)self->MusicSys.mpVStreamSys, 0xE);
    func_0028BCE8(*(void**)self->MusicSys.mpVStreamSys, 0xF);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F620);
#ifdef SKIP_ASM
struct sSndVEntry29F620 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_0029F620(void* self, void* obj)
{
    void* sub = (char*)obj + 0x6C0;
    sSndVEntry29F620* vt = *(sSndVEntry29F620**)sub;
    if (vt[7].fn((char*)sub + vt[7].delta) == 0) {
        return 0xE;
    }
    return 0xF;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cSSXAudio_PlayGrunt);
#ifdef SKIP_ASM
extern "C" int func_002A10C0(cSSXAudio* self, int id);
extern "C" int func_002A1028(cSSXAudio* self, int id);
extern "C" unsigned int func_002ADF60(void* mon);
extern "C" void cSSXAudio_PlayHistoryAdd(cSSXAudio* self, void* evt, int h, int t, int a, int idx);
extern "C" int cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" int cBankSys_SFX_End(cBankSys* self);
extern "C" int func_00288A20(void* self, void* obj);

struct sSsVtF660 { short delta; short index; void* (*fn)(void*); };

/**
 * Plays a rider's grunt as a positional voice and records it in the play history (event 3).
 *
 * Skipped when speech is off, during a replay, after the rider has finished, or while another grunt plays; a
 * computer rider needs @p allowAI, or func_002A10C0() to allow it. The sample depends on the character and the
 * length
 * (character kind 14 picks randomly, or cycles for other lengths); local humans get a shorter range.
 *
 * @param vself  the audio system
 * @param vrider  the rider
 * @param length  grunt length (On Tour eCharGruntLength)
 * @param allowAI  nonzero to allow computer riders (On Tour allowAI)
 */
extern "C" void cSSXAudio_PlayGrunt(cSSXAudio* vself, void* vrider, int length, int allowAI)
{
    sSnd17D0Sys* self = (sSnd17D0Sys*)vself;
    cRider* rider = (cRider*)vrider;
    if (func_0029F0A0(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int ok = rider->mIsHuman != 0 && rider->mIsLocal != 0;
    if (!ok && allowAI == 0) {
        if (func_002A10C0((cSSXAudio*)self, cRider_getIdentity(rider)) != 0) return;
    }
    int past = rider->mFinishTime >= 0.0f;
    if (past) return;
    if (cSSXAudio_PlayHistoryExists((cSSXAudio*)self, 3, 0) != 0) return;
    func_002A1028((cSSXAudio*)self, cRider_getIdentity(rider));
    int kind = func_0029F620(self, rider);
    int prio;
    if (kind == 0xE) {
        if (length == 1) {
            prio = (func_002ADF60(*(void**)((char*)*self->p118 + 0x1D8)) & 0x7FFF) * 3 / 0x7FFF;
        } else if (length == 0) {
            prio = (func_002ADF60(*(void**)((char*)*self->p118 + 0x1D8)) & 0x7FFF) * 3 / 0x7FFF + 3;
        } else {
            prio = *(int*)((char*)self + 0x5734);
            *(int*)((char*)self + 0x5734) = prio + 1;
            if (prio + 1 >= 6) {
                *(int*)((char*)self + 0x5734) = 0;
            }
        }
    } else if (length == 1) {
        prio = func_0029F378(self, rider) * 2 + 1;
    } else {
        prio = func_0029F378(self, rider) * 2;
    }
    int flag = 0;
    int ok2 = rider->mIsHuman != 0 && rider->mIsLocal != 0;
    if (ok2) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C11[0] == 2) {
            flag = cSSXAudio_ReplayMode(self) == 0;
        }
    }
    int vol = func_00290C40(self, rider);
    sSnd17D0Mgr* voiceMgr = self->mgr;
    sSnd17D0Sub* voiceSub = &voiceMgr->sub;
    voiceMgr->sub.cur++;
    voiceSub->ch[voiceMgr->sub.cur] = voiceMgr->sub.def;
    voiceMgr->sub.tgt[voiceMgr->sub.cur] = 0;
    voiceSub->p34[voiceMgr->sub.cur] = 0;
    voiceSub->vals[voiceMgr->sub.cur] = voiceSub->ch[voiceMgr->sub.cur].vol;
    voiceSub->p3C[voiceMgr->sub.cur] = 0;
    func_00416210((char*)voiceSub + voiceMgr->sub.cur * 8 + 0x40, 0, 8);
    voiceSub->p60[voiceMgr->sub.cur] = 0;
    voiceSub->p64[voiceMgr->sub.cur] = 0;
    voiceSub->p68[voiceMgr->sub.cur] = 0;
    voiceSub->p6C[voiceMgr->sub.cur] = 0;
    voiceSub->p70[voiceMgr->sub.cur].f4 = 0;
    voiceSub->p70[voiceMgr->sub.cur].f0 = 100;
    voiceSub->p70[voiceMgr->sub.cur].f8 = 90;
    voiceSub->p70[voiceMgr->sub.cur].fC = 50;
    voiceSub->p74[voiceMgr->sub.cur] = 0;
    voiceSub->p80[voiceMgr->sub.cur] = 0;
    voiceSub->p84[voiceMgr->sub.cur] = 0;
    voiceSub->p78[voiceMgr->sub.cur] = 0x7F;
    voiceSub->p7C[voiceMgr->sub.cur] = 1;
    self->a4[self->mgr->sub.cur] = kind;
    self->a8[self->mgr->sub.cur] = prio;
    sSnd17D0Mgr* curVoiceMgr = *self->p118;
    sSnd17D0Sub* curVoiceSub = &curVoiceMgr->sub;
    curVoiceSub->vals[curVoiceMgr->sub.cur] = vol;
    curVoiceSub->ch[curVoiceMgr->sub.cur].vol = (curVoiceSub->vals[curVoiceSub->cur] < 0x7F && curVoiceSub->vals[curVoiceSub->cur] < 0) ? 0 : (curVoiceSub->vals[curVoiceSub->cur] < 0x7F ? (unsigned char)curVoiceSub->vals[curVoiceSub->cur] : 0x7F);
    {
        sSnd17D0Mgr** pq = self->p118;
        int closestCamera = func_00288A20(self, rider);
        (*pq)->sub.p64[(*pq)->sub.cur] = closestCamera;
    }
    {
        sSnd17D0Mgr** pp = self->p118;
        int volumePtr = (int)func_00287968(self, 4, cRider_getIdentity(rider));
        (*pp)->sub.p3C[(*pp)->sub.cur] = volumePtr;
    }
    int ok3 = rider->mIsHuman != 0 && rider->mIsLocal != 0;
    if (ok3) {
        func_002A9988(&(*self->p118)->sub, 29.997713088989258f, -1.0f);
    } else {
        func_002A9988(&(*self->p118)->sub, 100.0f, -1.0f);
    }
    {
        sSnd17D0Mgr** pt = self->p118;
        char* obj = (char*)&rider->mVtbl;
        sSsVtF660* vt = *(sSsVtF660**)obj;
        void* pos = vt[kRiderVt_getPosition].fn(obj + vt[kRiderVt_getPosition].delta);
        (*pt)->sub.tgt[(*pt)->sub.cur] = pos;
    }
    if (flag) {
        (*self->p118)->sub.p80[(*self->p118)->sub.cur] = 1;
        (*self->p118)->sub.p84[(*self->p118)->sub.cur] = rider->mViewIndex;
    }
    int sfxHandle = cBankSys_SFX_End((cBankSys*)self);
    cSSXAudio_PlayHistoryAdd((cSSXAudio*)self, (void*)3, sfxHandle, *(volatile int*)(**(char***)((char*)self + 0x118) + 0x260) + 50, 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029FCC8);
#ifdef SKIP_ASM
extern "C" int func_002A3FE8(void* self);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cAudioInterface_getBE();
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
extern "C" int func_002B0E28(void* self, int i);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern "C" int cRider_getIdentity(void*);
// PORT: the unit defines func_0029F330 as void; it returns its tail call's result.
int func_0029F330_i(void* self) __asm__("func_0029F330");
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
// PORT: D_004A482C is called with 4 args here (the unit's later declaration has 3).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");
extern float D_004A36D8;

class cSsFCC8Obj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual int v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int v09();
};
struct sSsFCC8RiderA {
    char pad0[0x470];
    float f470;
    char pad474[0x6C0 - 0x474];
};
struct sSsFCC8Rider : public sSsFCC8RiderA, public cSsFCC8Obj {
    char pad6C4[0x788 - 0x6C4];
    char* p788;
    char pad78C[0x87C - 0x78C];
    int f87C;
};

extern "C" void func_0029FCC8(cSSXAudio* self, void* vrider)
{
    sSsFCC8Rider* rider = (sSsFCC8Rider*)vrider;
    if (func_0029F0A0(self) == 0) return;
    if (func_002A3FE8(self) != 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int pos = rider->f470 >= 0.0f;
    if (pos) return;
    if (*(float*)(rider->p788 + 0x98) < 4.000256061553955f) return;
    int chk = 0;
    if (rider->v09() != 0 || rider->f87C == 0) chk = 1;
    if (chk && cSSXAudio_GetFocusRider_p(self, -1) != rider) return;
    float v = rider->f470;
    float zero = 0.0f;
    int pos2 = v >= zero;
    if (pos2) return;
    char* snd = (char*)self + 0x5560;
    if (func_002B0E28(snd, 0) == 0) return;
    int r = func_0029F330_i(rider);
    if (cSSXAudio_CameraInRange(self, rider->v05(), D_004A36D8) < 0) return;
    int sel = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
    if (sel == 1) {
        if (func_002B1458(snd, 0, 8324, (int)rider, cRider_getIdentity(rider), 0, 0, zero) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8324), 2, r, 3);
        }
    } else if (sel == 2) {
        if (func_002B1458(snd, 0, 8323, (int)rider, cRider_getIdentity(rider), 0, 0, zero) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8323), 2, r, 3);
        }
    } else {
        if (func_002B1458(snd, 0, 8337, (int)rider, cRider_getIdentity(rider), 0, 0, zero) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8337), 2, r, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029FF80);
#ifdef SKIP_ASM
extern "C" int func_0029F0A0(void* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cAudioInterface_getBE();
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern "C" int cRider_getIdentity(void*);
// PORT: the unit defines func_0029F330 as void; it returns its tail call's result.
int func_0029F330_i(void* self) __asm__("func_0029F330");
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern void (*D_004A482C)(void*, int, int);
extern float D_004A36D8;

class cSsFF80Obj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual int v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int v09();
};
struct sSsFF80RiderA {
    char pad0[0x470];
    float f470;
    char pad474[0x6C0 - 0x474];
};
struct sSsFF80Rider : public sSsFF80RiderA, public cSsFF80Obj {
    char pad6C4[0x87C - 0x6C4];
    int f87C;
};

extern "C" void func_0029FF80(cSSXAudio* self, sSsFF80Rider* rider, int alt)
{
    if (func_0029F0A0(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int pos = rider->f470 >= 0.0f;
    if (pos) return;
    int chk = 0;
    if (rider->v09() != 0 || rider->f87C == 0) chk = 1;
    if (chk && cSSXAudio_GetFocusRider_p(self, -1) != rider) return;
    int r = func_0029F330_i(rider);
    if (cSSXAudio_CameraInRange(self, rider->v05(), D_004A36D8) < 0) return;
    if (alt != 0) {
        int sel = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
        if (sel == 1) {
            if (func_002B1458((char*)self + 0x5560, 0, 8336, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
                D_004A482C(func_003D8008(1, 0, 8336), 1, r);
            }
        } else if (sel == 2) {
            if (func_002B1458((char*)self + 0x5560, 0, 8347, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
                D_004A482C(func_003D8008(1, 0, 8347), 1, r);
            }
        } else {
            if (func_002B1458((char*)self + 0x5560, 0, 8349, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
                D_004A482C(func_003D8008(1, 0, 8349), 1, r);
            }
        }
    } else {
        int sel = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
        if (sel == 1) {
            if (func_002B1458((char*)self + 0x5560, 0, 8322, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
                D_004A482C(func_003D8008(1, 0, 8322), 1, r);
            }
        } else if (sel == 2) {
            if (func_002B1458((char*)self + 0x5560, 0, 8346, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
                D_004A482C(func_003D8008(1, 0, 8346), 1, r);
            }
        } else {
            if (func_002B1458((char*)self + 0x5560, 0, 8348, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
                D_004A482C(func_003D8008(1, 0, 8348), 1, r);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A02D8);
#ifdef SKIP_ASM
extern "C" int func_002A3F90(void* self);
extern "C" int func_002A4040(void* self);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cAudioInterface_getBE();
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
extern "C" int func_002B0E28(void* self, int i);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern "C" int cRider_getIdentity(void*);
// PORT: the unit defines func_0029F330 as void; it returns its tail call's result.
int func_0029F330_i(void* self) __asm__("func_0029F330");
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
// PORT: D_004A482C is called with 4 args here (the unit's later declaration has 3).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");
extern float D_004A36D8;

class cSs02D8Obj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual int v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int v09();
};
struct sSs02D8RiderA {
    char pad0[0x470];
    float f470;
    char pad474[0x6C0 - 0x474];
};
struct sSs02D8Rider : public sSs02D8RiderA, public cSs02D8Obj {
    char pad6C4[0x87C - 0x6C4];
    int f87C;
};

extern "C" void func_002A02D8(cSSXAudio* self, sSs02D8Rider* rider)
{
    if (func_0029F0A0(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int pos = rider->f470 >= 0.0f;
    if (pos) return;
    int chk = 0;
    if (rider->v09() != 0 || rider->f87C == 0) chk = 1;
    if (chk && cSSXAudio_GetFocusRider_p(self, -1) != rider) return;
    if (func_002B0E28((char*)self + 0x5560, 0) == 0) return;
    int r = func_0029F330_i(rider);
    int mode;
    if (func_002A3F90(self) != 0 || func_002A4040(self) != 0) {
        mode = 1;
    } else {
        mode = 3;
    }
    if (cSSXAudio_CameraInRange(self, rider->v05(), D_004A36D8) < 0) return;
    int sel = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
    if (sel == 1) {
        if (func_002B1458((char*)self + 0x5560, 0, 8328, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8328), 2, r, mode);
        }
    } else if (sel == 2) {
        if (func_002B1458((char*)self + 0x5560, 0, 8352, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8352), 2, r, mode);
        }
    } else {
        if (func_002B1458((char*)self + 0x5560, 0, 8353, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8353), 2, r, mode);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A0560);
#ifdef SKIP_ASM
extern "C" int func_0029F0A0(void* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cRider_getCheatIdentity(void*);
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
extern "C" int func_002B0E28(void* self, int i);
int func_002B1458_f(void* self, int a, int b, int c, int d, float f, int e, int refresh) __asm__("func_002B1458");
extern "C" void* func_003D8008(int a, int b, int c);
extern "C" int cRider_getIdentity(void*);
extern "C" void* cAudioInterface_getGame();
// PORT: the unit defines func_0029F330/func_002A1D48 as void; they return their tail call's result.
int func_0029F330_i(void* self) __asm__("func_0029F330");
int func_002A1D48_i(void* self, void* obj) __asm__("func_002A1D48");
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
unsigned func_002A1820_0560(void* self, void* a, void* b) __asm__("func_002A1820");
void fSinCos_0560(float a, float* x, float* y) __asm__("fSinCos");
// PORT: D_004A482C is called with 4 and 5 args here (the unit's later declaration has 3).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");
extern float D_004A36D8;
static inline int sndA30Mode0560(int m)
{
    return ScriptEngine->mPlayingNISList == m;
}

struct sVec4_0560 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_0560(const sVec4_0560& a, const sVec4_0560& b)
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

class cSs0560Obj {
public:
    virtual void v01();
    virtual sVec4_0560* v02();
    virtual void v03();
    virtual void v04();
    virtual int v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int v09();
};
struct sSs0560RiderA {
    char pad0[0x470];
    float f470;
    char pad474[0x4CC - 0x474];
    float f4CC;
    char pad4D0[0x6C0 - 0x4D0];
};
struct sSs0560Rider : public sSs0560RiderA, public cSs0560Obj {
    char pad6C4[0x87C - 0x6C4];
    int f87C;
};

void func_002A0560_0560(void* self, sSs0560Rider* a, sSs0560Rider* b) __asm__("func_002A0560");
void func_002A0560_0560(void* self, sSs0560Rider* a, sSs0560Rider* b)
{
    if (func_0029F0A0(self) == 0) return;
    if (!sndA30Mode0560(2)) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cRider_getCheatIdentity(a) != 0) return;
    if (cRider_getCheatIdentity(b) != 0) return;
    if (*(int*)(*(int*)((char*)cAudioInterface_getGame() + 0xC) + 8) - *(int*)((char*)self + 0x582C) < 600) return;
    int posA = a->f470 >= 0.0f;
    if (posA) return;
    int posB = b->f470 >= 0.0f;
    if (posB) return;
    int chkA = 0;
    if (a->v09() != 0 || a->f87C == 0) chkA = 1;
    int chkB = 0;
    if (b->v09() != 0 || b->f87C == 0) chkB = 1;
    void* pl = cSSXAudio_GetFocusRider_p(self, -1);
    if (chkA && chkB && a != pl && b != pl) return;
    char* snd = (char*)self + 0x5560;
    if (func_002B0E28(snd, 0) == 0) return;
    int okA = cSSXAudio_CameraInRange(self, a->v05(), D_004A36D8);
    if (okA < 0) return;
    int okB = cSSXAudio_CameraInRange(self, b->v05(), D_004A36D8);
    if (okA != okB) return;
    float t0, t1;
    fSinCos_0560(b->f4CC, &t0, &t1);
    sVec4_0560 v;
    v.x = t1;
    v.y = t0;
    v.z = 0;
    v.w = 0;
    float d = vu0Dot_0560(v, *a->v02());
    d -= vu0Dot_0560(v, *b->v02());
    if (!(d > 670.55999755859375f)) return;
    if (!(d < 1788.159912109375f)) return;
    int mode = 1;
    int rA = func_0029F330_i(a);
    int s = func_002A1D48_i(self, b);
    int rB = func_0029F330_i(b);
    if (d >= 1341.1199951171875f) mode = 2;
    unsigned sel = func_002A1820_0560(self, a, b);
    if (sel == 2) {
        d = 0.0f;
        if (func_002B1458_f(snd, 0, 8321, (int)a, cRider_getIdentity(a), d, 0, 0) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8321), 2, rA, mode);
        }
        if (func_002B1458_f(snd, 0, 8319, (int)b, cRider_getIdentity(b), d, 0, 0) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8319), 2, rB, mode);
        }
    } else if (sel < 2) {
        d = 0.0f;
        if (func_002B1458_f(snd, 0, 8320, (int)a, cRider_getIdentity(a), d, 0, 0) != 0) {
            s |= 0x1000;
            D_004A482C_5(func_003D8008(1, 0, 8320), 3, rA, mode, s);
        }
        if (func_002B1458_f(snd, 0, 8317, (int)b, cRider_getIdentity(b), d, 0, 0) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8317), 2, rB, mode);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A0A30);
#ifdef SKIP_ASM
extern "C" int func_0029F0A0(void* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int cRider_getCheatIdentity(void*);
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
extern "C" int func_002B0E28(void* self, int i);
// func_002B1458 with the float argument in its real position (same registers).
int func_002B1458_f(void* self, int a, int b, int c, int d, float f, int e, int refresh) __asm__("func_002B1458");
extern "C" void* func_003D8008(int a, int b, int c);
extern "C" int cRider_getIdentity(void*);
extern "C" void cSSXAudio_PlayGrunt(cSSXAudio* self, void* rider, int a, int b);
// PORT: the unit defines func_0029F330 as void; it returns its tail call's result.
int func_0029F330_i(void* self) __asm__("func_0029F330");
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
// PORT: D_004A482C is called with 4 args here (the unit's later declaration has 3).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");
extern float D_004A36D8;
static inline int sndA30Mode(int m)
{
    return ScriptEngine->mPlayingNISList == m;
}

class cSs0A30Obj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual int v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int v09();
};
struct sSs0A30RiderA {
    char pad0[0x470];
    float f470;
    char pad474[0x6C0 - 0x474];
};
struct sSs0A30Rider : public sSs0A30RiderA, public cSs0A30Obj {
    char pad6C4[0x87C - 0x6C4];
    int f87C;
};
struct sSs1820Rider;
extern "C" int func_002A1820(void* self, sSs1820Rider* a, sSs1820Rider* b);

extern "C" void func_002A0A30(cSSXAudio* self, sSs0A30Rider* a, sSs0A30Rider* b, int p7, int p8)
{
    if (func_0029F0A0(self) == 0) return;
    if (!sndA30Mode(2)) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cRider_getCheatIdentity(a) != 0) return;
    if (cRider_getCheatIdentity(b) != 0) return;
    int posA = a->f470 >= 0.0f;
    if (posA) return;
    int posB = b->f470 >= 0.0f;
    if (posB) return;
    int chkA = 0;
    if (a->v09() != 0 || a->f87C == 0) chkA = 1;
    int chkB = 0;
    if (b->v09() != 0 || b->f87C == 0) chkB = 1;
    void* pl = cSSXAudio_GetFocusRider_p(self, -1);
    if (chkA && chkB && a != pl && b != pl) return;
    cSSXAudio_PlayGrunt(self, b, 0, 1);
    char* snd = (char*)self + 0x5560;
    if (func_002B0E28(snd, 0) == 0) return;
    int okB = cSSXAudio_CameraInRange(self, b->v05(), D_004A36D8) >= 0;
    int okA = cSSXAudio_CameraInRange(self, a->v05(), D_004A36D8) >= 0;
    if (!okB && !okA) return;
    int rA = func_0029F330_i(a);
    int rB = func_0029F330_i(b);
    if (p8 != 0) return;
    if (p7 != 0) return;
    int sel = func_002A1820(self, (sSs1820Rider*)a, (sSs1820Rider*)b);
    if (sel == 2) {
        if (func_002B1458_f(snd, 0, 8335, (int)b, cRider_getIdentity(b), 0.0f, 0, 0) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8335), 2, rB, 3);
        }
        if (func_002B1458_f(snd, 0, 8330, (int)a, cRider_getIdentity(a), 0.0f, 0, 0) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8330), 2, rA, 3);
        }
    } else if (sel == 1) {
        if (func_002B1458_f(snd, 0, 8318, (int)b, cRider_getIdentity(b), 0.0f, 0, 0) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8318), 2, rB, 3);
        }
        if (func_002B1458_f(snd, 0, 8330, (int)a, cRider_getIdentity(a), 0.0f, 0, 0) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8330), 2, rA, 3);
        }
    } else if (sel == 0) {
        if (func_002B1458_f(snd, 0, 8312, (int)b, cRider_getIdentity(b), 0.0f, 0, 0) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8312), 2, rB, 3);
        }
        if (func_002B1458_f(snd, 0, 8334, (int)a, cRider_getIdentity(a), 0.0f, 0, 0) != 0) {
            D_004A482C_4(func_003D8008(1, 0, 8334), 2, rA, 3);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A0E70);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
extern "C" int func_002B0E28(void* self, int i);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern "C" int cRider_getIdentity(void*);
extern "C" void cSSXAudio_PlayGrunt(cSSXAudio* self, void* rider, int a, int b);
// PORT: the unit defines func_0029F330 as void; it returns its tail call's result.
int func_0029F330_i(void* self) __asm__("func_0029F330");
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern void (*D_004A482C)(void*, int, int);
extern float D_004A36D8;

class cSs0E70Obj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual int v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int v09();
};
struct sSs0E70RiderA {
    char pad0[0x470];
    float f470;
    char pad474[0x6C0 - 0x474];
};
struct sSs0E70Rider : public sSs0E70RiderA, public cSs0E70Obj {
    char pad6C4[0x874 - 0x6C4];
    int f874;
    int f878;
    int f87C;
};

extern "C" void func_002A0E70(cSSXAudio* self, sSs0E70Rider* rider, int play)
{
    if (func_0029F0A0(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int chk = 0;
    if (rider->v09() != 0 || rider->f87C == 0) chk = 1;
    if (chk && cSSXAudio_GetFocusRider_p(self, -1) != rider) return;
    float v = rider->f470;
    float zero = 0.0f;
    int pos = v >= zero;
    if (pos) return;
    char* snd = (char*)self + 0x5560;
    if (func_002B0E28(snd, 0) == 0) return;
    if (cSSXAudio_CameraInRange(self, rider->v05(), D_004A36D8) < 0) return;
    cSSXAudio_PlayGrunt(self, rider, 2, 0);
    if (play == 0) return;
    int ok = rider->f874 != 0 && rider->f87C != 0;
    if (!ok) return;
    int r = func_0029F330_i(rider);
    if (func_002B1458(snd, 0, 8311, (int)rider, cRider_getIdentity(rider), 0, 0, zero) == 0) return;
    D_004A482C(func_003D8008(1, 0, 8311), 1, r);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1028);
#ifdef SKIP_ASM
extern "C" int func_002B1220(void* self, int i);
extern "C" int func_002AB150(void* self, int i);
extern "C" void* cBankMonitor_BANKMONITOR_GetHandle(void* self, int idx, int a2);
extern "C" void func_002B11B0(void*, int);

extern "C" int func_002A1028(cSSXAudio* self, int id)
{
    int idx = func_002AB150(self->MusicSys.mpVStreamSys, func_002B1220((char*)self + 0x5560, 0));
    if (idx < 0) {
        return 1;
    }
    void* voice = cBankMonitor_BANKMONITOR_GetHandle(*(char**)self->MusicSys.mpVStreamSys + 0x1D8, idx, 3);
    if (voice == 0) {
        return 0;
    }
    if (*(int*)((char*)voice + 0x80) == id) {
        func_002B11B0((char*)self + 0x5560, 0);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A10C0);
#ifdef SKIP_ASM
extern "C" int func_002B1220(void* self, int i);
extern "C" int func_002AB150(void* self, int i);
extern "C" void* cBankMonitor_BANKMONITOR_GetHandle(void* self, int idx, int a2);

extern "C" int func_002A10C0(cSSXAudio* self, int id)
{
    int idx = func_002AB150(self->MusicSys.mpVStreamSys, func_002B1220((char*)self + 0x5560, 0));
    if (idx < 0) {
        return 0;
    }
    void* voice = cBankMonitor_BANKMONITOR_GetHandle(*(char**)self->MusicSys.mpVStreamSys + 0x1D8, idx, 3);
    if (voice == 0) {
        return 0;
    }
    return *(int*)((char*)voice + 0x80) == id;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1138);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
// PORT: the unit defines func_0029F330/func_002A1D48 as void; they return their tail call's result.
int func_0029F330_i(void* self) __asm__("func_0029F330");
int func_002A1D48_i(void* self, void* obj) __asm__("func_002A1D48");
// PORT: D_004A482C is called with 5 args here (the unit's later declaration has 3).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");
extern float D_004A36D8;

struct sSsVt1138 { short delta; short index; int (*fn)(void*); };

// PORT: callers declare func_002A1138 as (void*, int, int); the body takes (self, rider, obj).
void func_002A1138_impl(void* self, char* rider, void* obj) __asm__("func_002A1138");

void func_002A1138_impl(void* self, char* rider, void* obj)
{
    if (func_0029F0A0(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (rider == 0) rider = *(char**)((char*)cAudioInterface_getAI() + 0x2C);
    if (obj == 0) obj = *(void**)((char*)cAudioInterface_getAI() + 0x28);
    char* o = rider + 0x6C0;
    sSsVt1138* e = &(*(sSsVt1138**)o)[5];
    if (cSSXAudio_CameraInRange(self, e->fn(o + e->delta), D_004A36D8) < 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 8403, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) == 0) return;
    int r = func_0029F330_i(rider);
    int s = func_002A1D48_i(self, obj);
    D_004A482C_5(func_003D8008(1, 0, 8403), 3, r, s, 4);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1280);
#ifdef SKIP_ASM
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern "C" int cSSXAudio_CameraInRange(void* self, int which, float f);
// PORT: the unit defines func_0029F330/func_002A1D48 as void; they return their tail call's result.
int func_0029F330_i(void* self) __asm__("func_0029F330");
int func_002A1D48_i(void* self, void* obj) __asm__("func_002A1D48");
// PORT: D_004A482C is called with 4 args here (the unit's later declaration has 3).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");
extern float D_004A36D8;

struct sSsVt1280 { short delta; short index; int (*fn)(void*); };

// PORT: callers declare func_002A1280 as (void*, int, int); the body takes (self, rider, obj).
void func_002A1280_impl(void* self, char* rider, void* obj) __asm__("func_002A1280");

void func_002A1280_impl(void* self, char* rider, void* obj)
{
    if (func_0029F0A0(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    char* o = rider + 0x6C0;
    sSsVt1280* e = &(*(sSsVt1280**)o)[5];
    if (cSSXAudio_CameraInRange(self, e->fn(o + e->delta), D_004A36D8) < 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 8404, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) == 0) return;
    int r = func_0029F330_i(rider);
    int s = func_002A1D48_i(self, obj);
    D_004A482C_4(func_003D8008(1, 0, 8404), 2, r, s);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1388);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getAI();
extern "C" void func_002A1400(void* self, int a);

extern "C" int func_002A1388(cSSXAudio* self, void* obj)
{
    int i = *(int*)((char*)obj + 0x1C);
    int id;
    if (i >= 0 && i < *(int*)((char*)cAudioInterface_getAI() + 0x78)) {
        id = *(int*)((char*)cAudioInterface_getAI() + (i << 2) + 0x28);
    } else {
        id = *(int*)((char*)cAudioInterface_getAI() + 0x28);
    }
    func_002A1400(self, id);
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1400);
#ifdef SKIP_ASM
extern "C" int func_002A1E68(void* self, void* rider);
// PORT: callers pass self to func_002A4248 (the unit's later definition takes no args).
int func_002A4248_s(void* self) __asm__("func_002A4248");
extern signed char D_00535C11[];

// PORT: callers declare func_002A1400 as (void*, int); the body takes (self, rider).
void func_002A1400_impl(void* self, char* rider) __asm__("func_002A1400");

void func_002A1400_impl(void* self, char* rider)
{
    if (func_0029F0A0(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    char* o = rider + 0x6C0;
    sSsVt1280* e = &(*(sSsVt1280**)o)[5];
    if (cSSXAudio_CameraInRange(self, e->fn(o + e->delta), D_004A36D8) < 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 8377, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) == 0) return;
    int r = func_0029F330_i(rider);
    int k = func_002A1E68(self, rider);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int v;
    if (k == 1) {
        v = 1;
    } else if (func_002A4248_s(self) != 0) {
        v = 2;
    } else if (D_00535C11[0] == 2) {
        v = 2;
    } else if (k == 2 || k == 4) {
        v = 4;
    } else {
        v = 2;
    }
    D_004A482C_4(func_003D8008(1, 0, 8377), 2, r, v);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1560);
#ifdef SKIP_ASM
extern "C" int func_002B0E28(void* self, int i);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A1560(cSSXAudio* self, char* rider)
{
    if (func_0029F0A0(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    char* o = rider + 0x6C0;
    int check = 0;
    sSsVt1280* e9 = &(*(sSsVt1280**)o)[9];
    if (e9->fn(o + e9->delta) != 0 || *(int*)(rider + 0x87C) == 0) check = 1;
    if (check && cSSXAudio_GetFocusRider_p(self, -1) != rider) return;
    char* bm = (char*)self + 0x5560;
    if (func_002B0E28(bm, 0) == 0) return;
    char* o2 = rider + 0x6C0;
    sSsVt1280* e = &(*(sSsVt1280**)o2)[5];
    if (cSSXAudio_CameraInRange(self, e->fn(o2 + e->delta), D_004A36D8) < 0) return;
    if (func_002B1458(bm, 0, 8376, (int)rider, cRider_getIdentity(rider), 0, 0, 0.0f) == 0) return;
    int r = func_0029F330_i(rider);
    D_004A482C(func_003D8008(1, 0, 8376), 1, r);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A16B0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_0029F0A0(void* self);
extern "C" int func_002B0E28(void* self, int i);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
// func_0029F2B0 returns a value (the unit's earlier declaration says void).
int func_0029F2B0_r(int a, int b) __asm__("func_0029F2B0");
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);
extern signed char D_00535C11[];

extern "C" void func_002A16B0(cSSXAudio* self, int a1)
{
    if (func_0029F0A0(self) != 0) {
        char* bm = (char*)self + 0x5560;
        if (func_002B0E28(bm, 0) != 0) {
            cBE_getInterface_Fv(cBE_getBE(), 0);
            if (D_00535C11[0] != 0) {
                if (func_002B1458(bm, 0, 8380, 0, a1, 0, 1, 0.0f) != 0) {
                    int r = func_0029F2B0_r(a1, 0);
                    D_004A482C(func_003D8008(1, 0, 8380), 1, r);
                }
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A1778);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0A0(void* self);
extern "C" int func_002B0E28(void* self, int i);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
// func_0029F2B0 returns a value (the unit's earlier declaration says void).
int func_0029F2B0_r(int a, int b) __asm__("func_0029F2B0");
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A1778(cSSXAudio* self, int a1)
{
    if (func_0029F0A0(self) != 0) {
        char* bm = (char*)self + 0x5560;
        if (func_002B0E28(bm, 0) != 0) {
            if (func_002B1458(bm, 0, 8381, 0, a1, 0, 1, 0.0f) != 0) {
                int r = func_0029F2B0_r(a1, 0);
                D_004A482C(func_003D8008(1, 0, 8381), 1, r);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1820);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getAI();
void* cBEAggressionInterface_getThis();
extern "C" int cBEAggressionInterface_getAggression(void* ag, int a, int b);
extern "C" int cBEAggressionInterface_getState(void* ag, int a, int b);

class cSs1820Obj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int v07();
};
struct sSs1820RiderA {
    char pad0[0x6C0];
};
struct sSs1820Rider : public sSs1820RiderA, public cSs1820Obj {
    char pad6C4[0x874 - 0x6C4];
    int f874;
    int f878;
    int f87C;
};

extern "C" int func_002A1820(void* self, sSs1820Rider* a, sSs1820Rider* b)
{
    if (*(int*)((char*)cAudioInterface_getAI() + 0x78) == 2) return 2;
    int oka = a->f874 != 0 && a->f87C != 0;
    if (oka) {
        int okb = b->f874 != 0 && b->f87C != 0;
        if (okb) return 2;
    }
    int r = 2;
    int c1 = cBEAggressionInterface_getAggression(cBEAggressionInterface_getThis(), a->v07(), b->v07());
    int c2 = cBEAggressionInterface_getAggression(cBEAggressionInterface_getThis(), b->v07(), a->v07());
    if (c1 < 2 && c2 < 2) {
        int t = cBEAggressionInterface_getState(cBEAggressionInterface_getThis(), a->v07(), b->v07());
        if (t == 0) r = 0;
        else if (t == 1) r = 1;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A19D8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_002A31C0(cSSXAudio* self);
extern "C" void func_002A32B0(cSSXAudio* self);
extern "C" void func_002A3358(cSSXAudio* self);
extern "C" void func_002A3860(cSSXAudio* self);
extern "C" void func_002A2018(cSSXAudio* self);
extern "C" void func_002A2188(cSSXAudio* self);
extern "C" void func_002A2568(cSSXAudio* self, int a1);
extern "C" void func_002A2638(cSSXAudio* self);
extern "C" void func_002A39E0(cSSXAudio* self);
extern "C" void func_002A3708(cSSXAudio* self, char* rider);
extern const sSndKey8 D_004A3648_19D8[] __asm__("D_004A3648");

// PORT: 64-bit long sound key
// Front-end sound event dispatcher; returns 1 if the event was handled.
extern "C" int func_002A19D8(cSSXAudio* self, int ev)
{
    if (ev == 0) {
        func_002A31C0(self);
        if (*(int*)((char*)self + 0x530) == 0) {
            func_0028D488(self);
            func_002ADCA0(*(char**)self->MusicSys.mpVStreamSys + 0x1D8, 10, D_004A3648_19D8[0].v, 0, 3, 1, 0, 0);
        }
    } else if (ev == 1) {
        func_002A32B0(self);
    } else if (ev == 2) {
        func_002A3358(self);
    } else if (ev == 3) {
        func_002A3400(self);
    } else if (ev == 7) {
        func_002A3860(self);
    } else if (ev == 8) {
        func_002A2018(self);
    } else if (ev == 10) {
        func_002A2188(self);
    } else if (ev == 12) {
        func_002A2568(self, -1);
    } else if (ev == 14) {
        func_002A2638(self);
    } else if (ev == 15) {
        func_002A39E0(self);
    } else if (ev == 5) {
        func_002A3708(self, *(char**)((char*)cAudioInterface_getAI() + 0x28));
    } else {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1B58);
#ifdef SKIP_ASM
extern "C" void func_002A1400(void* self, int a);

extern "C" int func_002A1B58(cSSXAudio* self, int msg, int rider)
{
    if (msg == 2) {
        func_002A1400(self, rider);
        return 1;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A1B88);
#ifdef SKIP_ASM
extern "C" void func_002A1138(void* self, int a, int b);
extern "C" void func_002A1280(void* self, int a, int b);

extern "C" int func_002A1B88(cSSXAudio* self, int type, int rider, int otherRider)
{
    if (type == 0) {
        func_002A1138(self, rider, otherRider);
        return 1;
    }
    if (type == 1) {
        func_002A1280(self, rider, otherRider);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1BD0__FPv);
#ifdef SKIP_ASM
int func_002A1BD0(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1BD8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Maps a sound id (and, for the combined ids 0x11..0x15, a sub-id) to its channel bit.
extern "C" int func_002A1BD8(void* self, int location, int b)
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
    if (b != -1) {
        if (location == 17) {
            if (b == 0xE) return 0x1;
            if (b == 0xF) return 0x40;
            if (b == 0x10) return 0x200;
        } else if (location == 18) {
            if (b == 0xE) return 0x2;
            if (b == 17) return 0x1000;
        } else if (location == 19) {
            if (b == 0xE) return 0x4;
            if (b == 0x10) return 0x400;
            if (b == 17) return 0x2000;
        } else if (location == 20) {
            if (b == 0xE) return 0x8;
            if (b == 0xF) return 0x80;
        } else if (location == 21) {
            if (b == 0xE) return 0x10;
            if (b == 0xF) return 0x100;
            if (b == 0x10) return 0x800;
            if (b == 17) return 0x4000;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1D48);
#ifdef SKIP_ASM
extern "C" int cRider_getIdentity(void*);
extern "C" int cRider_getCheatIdentity(void*);
extern "C" void func_002A1DA0(void* self, int a, int b);

extern "C" void func_002A1D48(void* self, void* obj)
{
    int identity = cRider_getIdentity(obj);
    func_002A1DA0(self, identity, cRider_getCheatIdentity(obj));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1DA0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: the unit declares this as void(void*, int, int); defined under an asm label with its real type
extern "C" int func_002A1DA0_impl(void* self, unsigned int idx, int flag) __asm__("func_002A1DA0");
extern "C" int func_002A1DA0_impl(void* self, unsigned int idx, int flag)
{
    if (flag == 0) {
        switch (idx) {
        case 6: return 1;
        case 3: return 2;
        case 0: return 4;
        case 8: return 8;
        case 1: return 0x10;
        case 4: return 0x20;
        case 9: return 0x40;
        case 7: return 0x80;
        case 5: return 0x100;
        case 2: return 0x200;
        }
    }
    return 0x1000;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1E20);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_002A1E20(void* self, unsigned int x)
{
    switch (x) {
    case 0: case 1: case 5: case 8: case 11: case 14: case 17: case 18:
        return 1;
    case 2: case 3: case 6: case 9: case 12: case 15: case 19: case 20:
        return 2;
    case 4: case 7: case 10: case 13: case 16: case 21:
        return 4;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1E68);
#ifdef SKIP_ASM
extern int D_00536730[];

extern "C" int func_002A1E68(void *self, void *obj)
{
    int r;
    int v;

    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 2) {
        if (func_002A4078(self) != 0) {
            v = *(int *)((char *)obj + 0xEC);
            if (v == 0) {
                r = 1;
            } else if (v == 1) {
                r = 2;
            } else if (v == 2) {
                r = 4;
            } else if (v == 3) {
                r = 8;
            } else {
                r = (v == 4) ? 0x10 : 0x200;
            }
        } else {
            v = *(int *)((char *)obj + 0xEC);
            r = (v == 0) ? 1 : 2;
        }
        return r;
    } else {
        // Retail selects on a condition the compiler had folded to 1 (`li 1; movn` in case 5): a local flag that is
        // constant here, e.g. a feature toggle. Evaluating `mode != 2` instead gives a run-time xori (99.00).
        int notTwo = 1;
        switch (D_00536730[0]) {
        case 0:
            r = 1;
            break;
        case 1:
            r = 2;
            break;
        case 2:
            r = 4;
            break;
        case 3:
            r = 8;
            break;
        case 4:
            r = 0x10;
            break;
        case 5:
            r = notTwo ? 0x200 : 0x20;
            break;
        case 6:
            r = 0x40;
            break;
        case 7:
            r = 0x80;
            break;
        case 8:
            r = 0x100;
            break;
        case 9:
            r = 0x200;
            break;
        default:
            r = 0;
            break;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1FA0);
#ifdef SKIP_ASM
extern "C" int func_002A1FA0(void *self, void *obj)
{
    if (cRider_getCheatIdentity(obj) == 0) {
        switch (cRider_getIdentity(obj)) {
        case 1:
        case 2:
        case 4:
        case 6:
            return 2;
        case 0:
        case 3:
        case 5:
        case 7:
        case 8:
        case 9:
            return 1;
        default:
            return 0;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2018);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2018(cSSXAudio* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8392, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_4(func_003D8008(1, 0, 8392), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A20D0);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A20D0(cSSXAudio* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8393, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_4(func_003D8008(1, 0, 8393), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2188);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getAI();
// func_002A1D48 returns its tail call's value (the unit defines it as void).
int func_002A1D48_r(void* self, void* obj) __asm__("func_002A1D48");
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2188(cSSXAudio* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8394, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int a = func_002A1D48_r(self, *(void**)((char*)cAudioInterface_getAI() + 0x28));
            int m = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_5(func_003D8008(1, 0, 8394), 3, r, a, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2260);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2260(cSSXAudio* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8423, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_4(func_003D8008(1, 0, 8423), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2318);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2318(cSSXAudio* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8424, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_4(func_003D8008(1, 0, 8424), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A23D0);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A23D0(cSSXAudio* self, int mode)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8425, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m;
            if (mode == -1) {
                m = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            } else {
                m = func_002A1E20(self, mode);
            }
            D_004A482C_4(func_003D8008(1, 0, 8425), 2, r, m);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A24B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A24B0(cSSXAudio* self, int a1)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8396, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1BD8(self, a1, -1);
            D_004A482C_5(func_003D8008(1, 0, 8396), 3, r, m, 3);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A2568);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2568(cSSXAudio* self, int a1)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8396, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1BD8(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)), a1);
            D_004A482C_5(func_003D8008(1, 0, 8396), 3, r, m, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2638);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2638(cSSXAudio* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8378, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_4(func_003D8008(1, 0, 8378), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A26F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0D8(void* self);
extern "C" void func_002B11B0(void*, int);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_003D8008(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A26F0(void* self, int a, int b)
{
    if (func_0029F0D8(self) != 0) {
        char* bm = (char*)self + 0x5560;
        func_002B11B0(bm, 0);
        if (func_002B1458(bm, 0, 8421, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int x = 2;
            if (a) x = 1;
            if (*(int*)((char*)self + 0x5794) != 0) {
                *(int*)((char*)self + 0x5794) = 0;
            }
            int y = 2;
            if (!b) y = 1;
            D_004A482C_5(func_003D8008(1, 0, 8421), 3, r, x, y);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A27D8);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A27D8(cSSXAudio* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8422, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            D_004A482C(func_003D8008(1, 0, 8422), 1, r);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2860);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_003D8008(int a, int b, int c);
// 6-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_6)(void*, int, int, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2860(cSSXAudio* self, int n)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8399, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
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
            D_004A482C_6(func_003D8008(1, 0, 8399), 4, r, 3, lo, hi);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2938);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getAI();
// func_002A1D48 returns its tail call's value (the unit defines it as void).
int func_002A1D48_r(void* self, void* obj) __asm__("func_002A1D48");
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2938(cSSXAudio* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8450, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int a = func_002A1D48_r(self, *(void**)((char*)cAudioInterface_getAI() + 0x28));
            int m = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_5(func_003D8008(1, 0, 8450), 3, r, a, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2A10);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getRival(void*);
// func_002A1DA0 returns a value (the unit declares it void).
int func_002A1DA0_r(void* self, int a, int b) __asm__("func_002A1DA0");
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2A10(cSSXAudio* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8459, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1DA0_r(self, cBENewRaceInterface_getRival(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)), 0);
            D_004A482C_4(func_003D8008(1, 0, 8459), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2AD0);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2AD0(cSSXAudio* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8491, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
            D_004A482C_4(func_003D8008(1, 0, 8491), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2B88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2B88(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8492, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int v = *(int*)((char*)self + 0x5780);
            D_004A482C_4(func_003D8008(1, 0, 8492), 2, r, v);
        }
        *(int*)((char*)self + 0x5780) = 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A2C30);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2C30(cSSXAudio* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8493, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1BD8(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)), -1);
            D_004A482C_4(func_003D8008(1, 0, 8493), 2, r, m);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A2CF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2CF0(cSSXAudio* self, int a1)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8494, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1BD8(self, a1, -1);
            D_004A482C_4(func_003D8008(1, 0, 8494), 2, r, m);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A2DA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2DA0(cSSXAudio* self, int a1)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8495, 0, 10, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1BD8(self, a1, -1);
            D_004A482C_4(func_003D8008(1, 0, 8495), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2E50);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_002A4770(void* self);
extern "C" void func_002A2188(cSSXAudio* self);
extern "C" void func_002A23D0(cSSXAudio* self, int a1);
extern "C" void func_002A2018(cSSXAudio* self);
extern "C" void func_002A2A10(cSSXAudio* self);
extern "C" void func_002A2B88(void* self);
extern "C" void func_002A2260(cSSXAudio* self);
extern "C" void func_002A2318(cSSXAudio* self);
extern "C" void func_002A20D0(cSSXAudio* self);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void* cAudioInterface_getBE();
extern "C" unsigned int func_002ADF60(void* rng);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
// PORT: declared signed char elsewhere; the caller here sign-extends an int result itself.
int cBENewPlayerInterface_getRiderCharID_i(void* self, int riderIndex) __asm__("cBENewPlayerInterface_getRiderCharID__FPvi");
int cBERewardInterface_getTrackMedal(void* self, int a1, int ch, int a3, int track);
void func_002B1758(void* snd, int a1);

// Front-end music: picks the next track (a queued one, else a random one not played yet in this round of
// 5 or 8 tracks; the bonus track joins once both medals are won), marks it played and starts it.
extern "C" void func_002A2E50(void* vself, int bonus)
{
    char* self = (char*)vself;
    int pick = 0;
    if (*(int*)(self + 0x5790) == 0 && *(int*)(self + 0x6258) == 0) {
        *(int*)(self + 0x5790) = 1;
        *(int*)(self + 0x5798) = 3;
        *(unsigned int*)(self + 0x573C) |= 0x80;
        *(int*)(self + 0x578C) = 0;
    } else {
        if (func_002A4770(self) != 0) {
            return;
        }
        int queued = *(int*)(self + 0x5798);
        if (queued != -1 && *(int*)(self + 0x6258) == 0) {
            pick = queued;
            *(int*)(self + 0x5798) = -1;
        } else {
            int n = bonus == 0 ? 8 : 5;
            int left = 0;
            for (int i = 0; i < n; i++) {
                left += ((*(int*)(self + 0x573C) >> i) ^ 1) & 1;
            }
            if (left == 0) {
                *(unsigned int*)(self + 0x573C) = 0;
                left = n;
                int track = 0xE;
                void* players = cBE_getInterface_Fv(cBE_getBE(), 1);
                signed char ch = cBENewPlayerInterface_getRiderCharID_i(players, cBENewPlayerInterface_getHumanRider(players, 0));
                int sel = func_002A1E20(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)));
                if (sel != 1) {
                    track = sel == 2 ? 0xF : 0x10;
                }
                void* rewards = cBE_getInterface_Fv(cBE_getBE(), 13);
                int gold = cBERewardInterface_getTrackMedal(rewards, 0, ch, 4, track) != -1;
                int plat = cBERewardInterface_getTrackMedal(rewards, 0, ch, 5, track) != -1;
                if (gold && plat) {
                    *(unsigned int*)(self + 0x573C) |= 8;
                }
            }
            int nthUnplayed = (func_002ADF60(*(void**)(**(char***)(self + 0x118) + 0x1D8)) & 0x7FFF) * left / 0x7FFF + 1;
            int cnt = 0;
            while (cnt < nthUnplayed) {
                cnt += ((*(int*)(self + 0x573C) >> pick) ^ 1) & 1;
                pick += cnt < nthUnplayed;
            }
        }
    }
    *(unsigned int*)(self + 0x573C) |= 1 << pick;
    char* snd = self + 0x5560;
    switch (pick) {
    case 0:
        func_002A2188((cSSXAudio*)self);
        break;
    case 1:
        func_002A23D0((cSSXAudio*)self, -1);
        break;
    case 2:
        func_002A2018((cSSXAudio*)self);
        break;
    case 3:
        func_002A2A10((cSSXAudio*)self);
        break;
    case 4:
        func_002A2B88(self);
        break;
    case 5:
        func_002A2260((cSSXAudio*)self);
        break;
    case 6:
        func_002A2318((cSSXAudio*)self);
        break;
    case 7:
        func_002A20D0((cSSXAudio*)self);
        break;
    }
    func_002B1758(snd, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A3170);
#ifdef SKIP_ASM
extern "C" void func_002A26F0(void* self, int a, int b);
void func_002B1758(void* self, int a1);
int func_002B4908(void* self);

extern "C" void func_002A3170(void* self)
{
    func_002A26F0(self, 0, 0);
    func_002B1758((char*)self + 0x5560, 0);
    *(int*)((char*)self + 0x5774) = 1;
    *(int*)((char*)self + 0x5778) = func_002B4908((char*)self + 0x118);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A31C0);
#ifdef SKIP_ASM
extern "C" int func_0029F128(void* self);
extern "C" int func_002A4040(void* self);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C11[];
extern "C" int cSSXAudio_PlayHistoryExists(cSSXAudio* self, int a, int b);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A31C0(cSSXAudio* self)
{
    if (func_0029F128(self) == 0) return;
    if (func_002A4040(self) != 0) return;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] != 0) return;
    if (*(int*)SSXApp->GameModeMan < 2) {
        if (cSSXAudio_PlayHistoryExists(self, 0, 0) != 0) return;
        if (func_002B1458((char*)self + 0x5560, 0, 8384, 0, 11, 0, 0, 0.0f) == 0) return;
        int r = func_0029F198(self);
        D_004A482C_4(func_003D8008(1, 0, 8384), 2, r, 1);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A32B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F128(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getAI();
// func_002A1D48 returns its tail call's value (the unit defines it as void).
int func_002A1D48_r(void* self, void* obj) __asm__("func_002A1D48");
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A32B0(cSSXAudio* self)
{
    if (func_0029F128(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8385, 0, 11, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int a = func_002A1D48_r(self, *(void**)((char*)cAudioInterface_getAI() + 0x28));
            D_004A482C_4(func_003D8008(1, 0, 8385), 2, r, a);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3358);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F128(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getAI();
// func_002A1D48 returns its tail call's value (the unit defines it as void).
int func_002A1D48_r(void* self, void* obj) __asm__("func_002A1D48");
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A3358(cSSXAudio* self)
{
    if (func_0029F128(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8386, 0, 11, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int a = func_002A1D48_r(self, *(void**)((char*)cAudioInterface_getAI() + 0x28));
            D_004A482C_4(func_003D8008(1, 0, 8386), 2, r, a);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A3400);
#ifdef SKIP_ASM
extern "C" int func_0029F128(void* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A3400(cSSXAudio* self)
{
    if (func_0029F128(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 8387, 0, 11, 0, 0, 0.0f) == 0) return;
    int r = func_0029F198(self);
    int m = func_002A1BD8(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)), -1);
    D_004A482C_4(func_003D8008(1, 0, 8387), 2, r, m);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A34D0);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_002ADCA0_i(void*, int, long, int, int, int, int, int) __asm__("func_002ADCA0");
struct sPacked8_34D0 {
    long v;
} __attribute__((packed));
extern sPacked8_34D0 D_004A36E8_p[] __asm__("D_004A36E8");
extern "C" int func_002A4248_p(void *) __asm__("func_002A4248");
extern "C" int func_002A42C8_p(void *) __asm__("func_002A42C8");

struct sSsVt_34D0 { short delta; short index; int (*fn)(void *); };

extern "C" int func_002A34D0(cSSXAudio* vself, cRider *rider, int flag, int which)
{
    int sel;
    int a;
    int b;
    int c;

    if (func_0029F128(((char*)vself)) == 0) {
        return 1;
    }
    if (cSSXAudio_ReplayMode(((char*)vself)) != 0) {
        return 1;
    }
    if (func_002A4248_p(((char*)vself)) != 0 && rider != 0) {
        sPacked8_34D0 *key = &D_004A36E8_p[0];
        char *obj = (char *)&rider->mVtbl;
        sSsVt_34D0 *vt = *(sSsVt_34D0 **)obj;
        char *dst = *(char**)vself->MusicSys.mpVStreamSys + 0x1D8;
        int idx = vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta);
        func_002ADCA0_i(dst, 1000, key->v, 0, idx, 0, 0, 0);
        return 1;
    }
    if (func_002A42C8_p(((char*)vself)) != 0) {
        return 1;
    }
    if (flag != 0) {
        if (func_002B0E28(((char*)vself) + 0x5560, 0) == 0) {
            return 0;
        }
    }
    sel = 1;
    if (rider == 0) {
        if (func_002A4078(((char*)vself)) == 0) {
            return 1;
        }
        if (func_002A4040(((char*)vself)) != 0) {
            return 1;
        }
        sel = 0;
        rider = (cRider *)cSSXAudio_GetFocusRider_p(((char*)vself), which);
    }
    if (cRider_getCheatIdentity(rider) != 0) {
        return 1;
    }
    if (func_002B1458_f(((char*)vself) + 0x5560, 0, 8388, 0, 11, 0.0f, 0, 0) == 0) {
        return 1;
    }
    a = func_0029F198(((char*)vself));
    b = func_002A1D48_i(((char*)vself), rider);
    if (sel != 0) {
        c = func_002A1E68(((char*)vself), rider);
    } else {
        switch ((unsigned int)rider->unk_0EC) {
        case 0:
            c = 1;
            break;
        case 1:
            c = 2;
            break;
        case 2:
            c = 4;
            break;
        case 3:
            c = 8;
            break;
        case 4:
            c = 0x10;
            break;
        case 5:
            c = 0x200;
            break;
        default:
            c = 0;
            break;
        }
    }
    D_004A482C_5(func_003D8008(1, 0, 8388), 3, a, b, c);
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3708);
#ifdef SKIP_ASM
extern "C" int func_0029F128(void* self);
extern "C" int cRider_getCheatIdentity(void*);
extern "C" int func_0029F198(void* self);
extern "C" int func_002A1E68(void* self, void* rider);
// PORT: callers pass self to func_002A4248 (the unit's later definition takes no args).
int func_002A4248_s(void* self) __asm__("func_002A4248");
// PORT: D_004A482C is called with 5 args here (the unit's later declaration has 3).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");
extern sSndKey8 D_004A36E8[];

// PORT: 64-bit long sound key
extern "C" void func_002A3708(cSSXAudio* self, char* rider)
{
    if (func_0029F128(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (cRider_getCheatIdentity(rider) != 0) return;
    if (func_002A4248_s(self) != 0) {
        char* o = rider + 0x6C0;
        char* p = *(char**)self->MusicSys.mpVStreamSys + 0x1D8;
        sSndKey8* key = D_004A36E8;
        sSsVt1280* e = &(*(sSsVt1280**)o)[7];
        int v = e->fn(o + e->delta);
        func_002ADCA0(p, 1000, key->v, 0, v, 0, 0, 0);
        return;
    }
    if (func_002B1458((char*)self + 0x5560, 0, 8389, 0, 11, 0, 0, 0.0f) == 0) return;
    int a = func_0029F198(self);
    int b = func_002A1D48_i(self, rider);
    int c = func_002A1E68(self, rider);
    D_004A482C_5(func_003D8008(1, 0, 8389), 3, a, b, c);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3860);
#ifdef SKIP_ASM
#include "fakematch.h"
extern "C" int func_0029F128(void* self);
extern "C" int func_0029F198(void* self);
// PORT: D_004A482C is called with 5 args here (the unit's later declaration has 3).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");
extern int D_00536730[];

struct sSndRiderList3860 {
    void* r[8];
};

// FAKEMATCH: FM_OPAQUE(lst) after `lst = cAudioInterface_getAI() + 0x28` (keeps combine from folding +0x28 into the
// lw offset; retail computes addiu 0x28 / addu / lw 0); plain form 96.86%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" void func_002A3860(cSSXAudio* self)
{
    if (func_0029F128(self) == 0) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    int n = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    signed char* g = (signed char*)((char*)&cBENewRaceInterface_newRace);
    if (g[0x49] == 2) {
        n = D_00536730[1] < D_00536730[0];
    }
    if (func_002B1458((char*)self + 0x5560, 0, 8391, 0, 11, 0, 0, 0.0f) == 0) return;
    int a = func_0029F198(self);
    sSndRiderList3860* lst = (sSndRiderList3860*)((char*)cAudioInterface_getAI() + 0x28);
    FM_OPAQUE(lst);
    int b = func_002A1D48_i(self, lst->r[n]);
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 13);
    if (g[0x49] != 2 && *(int*)(iface + 0x14) == 0) {
        n = 8;
    } else {
        n = D_00536730[n];
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
    D_004A482C_5(func_003D8008(1, 0, 8391), 3, a, b, n);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A39E0);
#ifdef SKIP_ASM
extern "C" int func_0029F128(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A39E0(cSSXAudio* self)
{
    if (func_0029F128(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 8379, 0, 11, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1BD8(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0)), -1);
            D_004A482C_4(func_003D8008(1, 0, 8379), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3AA0);
#ifdef SKIP_ASM
extern "C" int func_003B7C40(int, void *);
extern "C" void func_002A73A8(cCrowdSys* self, int a1, int i);

extern "C" int func_002A3AA0(void* self, void* arg1) {
    int st[4];
    func_003B58A0();
    int status = func_003B7C40(*(int *)((char*)arg1 + 0x1C), st);
    func_003B58D8();
    if (status >= 0) {
        if (st[0] == 3) {
            func_002A73A8((cCrowdSys*)self, 0, 0);
            return 1;
        }
        return 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3B18);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F160(void* self);
// cSSXAudio_GetFocusRider returns an object pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A3B18(cSSXAudio* self, void* obj, int a2)
{
    if (func_0029F160(self) == 0) return;
    if (obj == 0) return;
    int done = *(float*)((char*)obj + 0x470) >= 0.0f;
    if (done) return;
    if (obj != cSSXAudio_GetFocusRider_p(self, -1)) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 8359, 0, 0xC, 0, 0, 0.0f) != 0) {
        D_004A482C(func_003D8008(1, 0, 8359), 1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3C00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F160(void* self);
// cSSXAudio_GetFocusRider returns an object pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A3C00(cSSXAudio* self, void* obj, int a2)
{
    if (func_0029F160(self) == 0) return;
    if (obj == 0) return;
    if (a2 != 2) {
        int done = *(float*)((char*)obj + 0x470) >= 0.0f;
        if (done) return;
    }
    if (obj != cSSXAudio_GetFocusRider_p(self, -1)) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 8360, 0, 0xC, 0, 0, 0.0f) != 0) {
        D_004A482C(func_003D8008(1, 0, 8360), 1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3CE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void (*D_004A482C)(void*, int, int);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");

extern "C" void func_002A3CE8(cSSXAudio* self, char* rider, int n)
{
    float zero;
    if (func_0029F160(self) == 0) return;
    if (rider == 0) return;
    float v = *(float*)(rider + 0x470);
    zero = 0.0f;
    int done = v >= zero;
    if (done) return;
    if (n != 1 && rider != cSSXAudio_GetFocusRider_p(self, -1)) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (n == 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 8361, 0, 0xC, 0, 0, zero) != 0) {
        D_004A482C(func_003D8008(1, 0, 8361), 1, n);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3DE0);
#ifdef SKIP_ASM
extern "C" int func_0029F160(void* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A3DE0(cSSXAudio* self, void* obj, int ev)
{
    if (func_0029F160(self) == 0) return;
    if (obj == 0) return;
    int done = *(float*)((char*)obj + 0x470) >= 0.0f;
    if (done) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 8499, 0, 0xC, 0, 0, 0.0f) != 0) {
        D_004A482C(func_003D8008(1, 0, 8499), 1, ev);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3EB8);
#ifdef SKIP_ASM
extern "C" int func_0029F160(void* self);
extern "C" int cSSXAudio_ReplayMode(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A3EB8(cSSXAudio* self, void* obj, int ev)
{
    if (func_0029F160(self) == 0) return;
    if (obj == 0) return;
    int done = *(float*)((char*)obj + 0x470) >= 0.0f;
    if (done) return;
    if (cSSXAudio_ReplayMode(self) != 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 8517, 0, 0xC, 0, 0, 0.0f) != 0) {
        D_004A482C(func_003D8008(1, 0, 8517), 1, ev);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3F90);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A3FD8(void* self, unsigned int a1);

extern "C" int func_002A3F90(void* self)
{
    return func_002A3FD8(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3FD8);
#ifdef SKIP_ASM
extern "C" int func_002A3FD8(void* self, unsigned int location)
{
    return location - 17 < 5;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3FE8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A4030(void* self, unsigned int a1);

extern "C" int func_002A3FE8(void* self)
{
    return func_002A4030(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4030);
#ifdef SKIP_ASM
extern "C" int func_002A4030(void* self, unsigned int location)
{
    return location - 0xe < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4040);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C10[];

extern "C" int func_002A4040(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    return D_00535C10[0] == 4;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4078);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A40E0(void* self, unsigned int a1);
extern signed char D_00535C10[];

extern "C" int func_002A4078(void* self)
{
    if (func_002A40E0(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0))) != 0) {
        return 1;
    }
    if (D_00535C10[0] == 0) {
        return 1;
    }
    return D_00535C10[0] == 5;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A40E0);
#ifdef SKIP_ASM
extern "C" int func_002A40E0(void* self, unsigned int location)
{
    return location < 5;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A40E8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A4158(void* self, unsigned int a1);
extern signed char D_00535C10[];

extern "C" int func_002A40E8(void* self)
{
    if (func_002A4158(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0))) != 0) {
        return 1;
    }
    if (D_00535C10[0] == 1) {
        return 1;
    }
    int r = 0;
    if (D_00535C10[0] == 6) {
        r = 1;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4158);
#ifdef SKIP_ASM
extern "C" int func_002A4158(void* self, unsigned int location)
{
    return location - 5 < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4168);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A41C8(void* self, unsigned int a1);
extern signed char D_00535C10[];

extern "C" int func_002A4168(void* self)
{
    if (func_002A41C8(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0))) != 0) {
        return 1;
    }
    return D_00535C10[0] == 2;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A41C8);
#ifdef SKIP_ASM
extern "C" int func_002A41C8(void* self, unsigned int location)
{
    return location - 8 < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A41D8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int func_002A4238(void* self, unsigned int a1);
extern signed char D_00535C10[];

extern "C" int func_002A41D8(void* self)
{
    if (func_002A4238(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0))) != 0) {
        return 1;
    }
    return D_00535C10[0] == 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4238);
#ifdef SKIP_ASM
extern "C" int func_002A4238(void* self, unsigned int location)
{
    return location - 11 < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4248);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C12[];

extern "C" int func_002A4248(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    switch (D_00535C12[0]) {
    case 4:
        return 1;
    case 5:
        return 1;
    default:
        return 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4290);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C12[];

extern "C" int func_002A4290(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    return D_00535C12[0] == 4;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A42C8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C12[];

extern "C" int func_002A42C8(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C12[0] >= 6) {
        if (D_00535C12[0] <= 11) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4318);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C12[];

extern "C" int func_002A4318(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C12[0] >= 6) {
        if (D_00535C12[0] <= 8) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4368);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C12[];

extern "C" int func_002A4368(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C12[0] >= 9) {
        if (D_00535C12[0] <= 11) {
            return 1;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A43B8);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getAI();
extern "C" void func_002A1138(void* self, int a, int b);
extern "C" void func_002A1400(void* self, int a);
extern "C" void func_002A26F0(void* self, int a, int b);
extern "C" void func_002A2568(cSSXAudio* self, int a);
extern "C" void func_002A2638(cSSXAudio* self);
extern "C" void func_002A27D8(cSSXAudio* self);
extern "C" void func_002A2860(cSSXAudio* self, int a);
extern "C" void func_002A2AD0(cSSXAudio* self);
extern "C" void func_002A2B88(void* self);
extern "C" void func_002A2C30(cSSXAudio* self);
extern "C" void func_002A2E50(void* self, int a);

extern "C" void func_002A43B8(char* self)
{
    if (*(int*)(self + 0x5768) != 0) {
        func_002A26F0(self, 0, 0);
        *(int*)(self + 0x5768) = 0;
    } else if (*(int*)(self + 0x5784) != 0) {
        func_002A2AD0((cSSXAudio*)self);
        *(int*)(self + 0x5784) = 0;
    } else if (*(int*)(self + 0x576C) != 0) {
        func_002A2638((cSSXAudio*)self);
        *(int*)(self + 0x576C) = 0;
    } else if (*(int*)(self + 0x577C) != 0) {
        func_002A2B88(self);
        *(int*)(self + 0x577C) = 0;
    } else if (*(int*)(self + 0x5770) != 0) {
        func_002A2C30((cSSXAudio*)self);
        *(int*)(self + 0x5770) = 0;
    } else if (*(int*)(self + 0x5788) != 0) {
        func_002A1138(self, 0, 0);
        *(int*)(self + 0x5788) = 0;
    } else if (*(int*)(self + 0x5774) != 0) {
        func_002A2860((cSSXAudio*)self, *(int*)(self + 0x5778));
        *(int*)(self + 0x5774) = 0;
    } else if (*(int*)(self + 0x574C) != 0) {
        func_002A2E50(self, 0);
        *(int*)(self + 0x574C) = 0;
    } else if (*(int*)(self + 0x5750) != 0) {
        func_002A2E50(self, 0);
        *(int*)(self + 0x5750) = 0;
    } else if (*(int*)(self + 0x5754) != 0) {
        func_002A2E50(self, 1);
        *(int*)(self + 0x5754) = 0;
    } else if (*(int*)(self + 0x5758) != 0) {
        func_002A2568((cSSXAudio*)self, *(int*)(self + 0x575C));
        *(int*)(self + 0x5758) = 0;
    } else if (*(int*)(self + 0x5740) != 0) {
        func_002A27D8((cSSXAudio*)self);
        *(int*)(self + 0x5740) = 0;
    }
    if (*(int*)(self + 0x5744) != 0) {
        char* aiMan = (char*)cAudioInterface_getAI();
        int rider = *(int*)(aiMan + (*(int*)(self + 0x5748) << 2) + 0x28);
        if (rider != 0) {
            func_002A1400(self, rider);
        }
        *(int*)(self + 0x5744) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4550);
#ifdef SKIP_ASM
extern "C" void func_002A4550(void* self)
{
    *(int*)((char*)self + 0x5740) = 0;
    *(int*)((char*)self + 0x5744) = 0;
    *(int*)((char*)self + 0x574C) = 0;
    *(int*)((char*)self + 0x5750) = 0;
    *(int*)((char*)self + 0x5754) = 0;
    *(int*)((char*)self + 0x5758) = 0;
    *(int*)((char*)self + 0x5760) = 0;
    *(int*)((char*)self + 0x5768) = 0;
    *(int*)((char*)self + 0x576C) = 0;
    *(int*)((char*)self + 0x5770) = 0;
    *(int*)((char*)self + 0x5774) = 0;
    *(int*)((char*)self + 0x577C) = 0;
    *(int*)((char*)self + 0x5784) = 0;
    *(int*)((char*)self + 0x5788) = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4590);
#ifdef SKIP_ASM
extern "C" void func_002A4590(void* self, int flag)
{
    *(int*)((char*)self + 0x57F8) = 0;
    *(int*)((char*)self + 0x57FC) = 0;
    *(int*)((char*)self + 0x5804) = 0;
    *(int*)((char*)self + 0x5808) = 0;
    *(int*)((char*)self + 0x5800) = -1;
    *(int*)((char*)self + 0x5810) = -1;
    if (flag) {
        *(int*)((char*)self + 0x580C) = -1;
    }
    *(int*)((char*)self + 0x5814) = 23;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A45C0);
#ifdef SKIP_ASM
extern "C" void func_002A4590(void* self, int flag);
extern "C" void* cAudioInterface_getAI();
extern "C" int func_002A4078(void* self);
extern "C" void func_002A4660(void* self, int id, int a, int b, int c);
extern int D_00536730[];

extern "C" void func_002A45C0(cSSXAudio* self)
{
    if (*(int*)SSXApp->GameModeMan == 1) {
        func_002A4590(self, 0);
    }
    int id = D_00536730[0];
    char* rider = *(char**)((char*)cAudioInterface_getAI() + 0x28);
    int a;
    if (func_002A4078(self) != 0) {
        a = *(int*)(*(char**)(rider + 0x790) + 0x128);
    } else {
        a = 0;
    }
    func_002A4660(self, id, a, *(int*)(*(char**)(rider + 0x790) + 0x114), *(int*)(rider + 0x480) ^ 1);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4660);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);

extern "C" void func_002A4660(void* self, int a1, int a2, int a3, int a4)
{
    char* gameModeMan = (char*)SSXApp->GameModeMan;
    if (*(int*)(gameModeMan + 0x98) != 0 || *(int*)(gameModeMan + 0x70) <= *(int*)gameModeMan) {
        *(int*)((char*)self + 0x57F8) = 1;
        if (a4 != 0 && *(int*)gameModeMan == 3) {
            *(int*)((char*)self + 0x57FC) = 1;
        }
        *(int*)((char*)self + 0x5800) = a1;
        *(int*)((char*)self + 0x5810) = *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cAudioInterface_getBE(), 0));
    }
    *(int*)((char*)self + 0x5804) += a2;
    *(int*)((char*)self + 0x5808) += a3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4718);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);

extern "C" int func_002A4718(void* self)
{
    int id = *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0));
    int cur = *(int*)((char*)self + 0x5814);
    if (id == cur || cur == 23) {
        return 0;
    }
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A4770);
#ifdef SKIP_ASM
extern "C" int func_002A40E0(void* self, unsigned int a1);
extern "C" int func_002A4158(void* self, unsigned int a1);
extern "C" int func_002A4238(void* self, unsigned int a1);
extern "C" int func_002A4030(void* self, unsigned int a1);
extern "C" int func_002A3FD8(void* self, unsigned int a1);
extern "C" void func_002A2E50(void* self, int a);
// bank-monitor method: returns BXrand()
extern "C" unsigned int func_002ADF60(void* mon);

extern "C" int func_002A4770(void* self)
{
    char* audio = (char*)self;
    if (*(int*)(audio + 0x57F8) != 0) {
        *(int*)(audio + 0x57F8) = 0;
        int flags[3];
        int i;
        for (i = 2; i >= 0; i--) {
            flags[i] = 0;
        }
        int n = 0;
        if (*(int*)(audio + 0x57FC) != 0) {
            if (*(int*)(audio + 0x5800) == 0) {
                flags[0] = 1;
                n = 1;
            }
            if (*(int*)(audio + 0x5804) >= 5) {
                flags[1] = 1;
                n++;
            }
            int lim;
            if (func_002A40E0(self, *(int*)(audio + 0x5810))) {
                lim = 27;
            } else if (func_002A4158(self, *(int*)(audio + 0x5810))) {
                lim = 27;
            } else {
                lim = func_002A4238(self, *(int*)(audio + 0x5810)) ? 27 : 0x18;
            }
            if (*(int*)(audio + 0x5808) >= lim) {
                flags[2] = 1;
                n++;
            }
        }
        *(int*)(audio + 0x57FC) = 0;
        if (n == 0) {
            if (*(int*)(audio + 0x6254) != 0) {
                int cur = *(int*)(audio + 0x5814);
                if (cur != 23) {
                    if (func_002A3FD8(self, cur)) {
                        func_002A2E50(self, 0);
                        return 1;
                    }
                    if (func_002A4030(self, *(int*)(audio + 0x5814))) {
                        func_002A23D0((cSSXAudio*)self, *(int*)(audio + 0x5814));
                        return 1;
                    }
                    func_002A24B0((cSSXAudio*)self, *(int*)(audio + 0x5814));
                    return 1;
                }
            }
            return 0;
        }
        if (n >= 2) {
            int last = *(int*)(audio + 0x580C);
            if (last != -1 && flags[last] != 0) {
                flags[last] = 0;
                n--;
            }
        }
        int pick = -1;
        int k = (func_002ADF60(*(void**)(**(char***)(audio + 0x118) + 0x1D8)) & 0x7FFF) * n / 0x7FFF + 1;
        while (k > 0) {
            pick++;
            if (flags[pick]) {
                k--;
            }
        }
        switch (pick) {
        case 0:
            func_002A2938((cSSXAudio*)self);
            break;
        case 1:
            func_002A2CF0((cSSXAudio*)self, *(int*)(audio + 0x5810));
            break;
        case 2:
            func_002A2DA0((cSSXAudio*)self, *(int*)(audio + 0x5810));
            break;
        }
        *(int*)(audio + 0x580C) = pick;
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A49E8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sSndId2A49E8 {
    long v;
} __attribute__((packed));
extern const sSndId2A49E8 D_004A36F0[];
extern "C" void func_002ADCA0(void* bank, int a1, long id, int a3, int a4, int a5, int a6, int a7);

// Flags the transition and fades the ambient stream back in.
// PORT: the 8-byte stream id is passed by value from an unaligned global (64-bit `long`).
extern "C" void func_002A49E8(char* self)
{
    *(int*)(self + 0x5818) = 1;
    func_002ADCA0(**(char***)(self + 0x118) + 0x1D8, 30000, D_004A36F0[0].v, 0, 0, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4A38);
#ifdef SKIP_ASM
extern "C" int func_002A10C0(cSSXAudio* self, int id);

extern "C" int func_002A4A38(void* self)
{
    if (*(int*)((char*)self + 0x5818) != 0) {
        return func_002A10C0((cSSXAudio*)self, 10) != 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4A68);
#ifdef SKIP_ASM
extern "C" int func_002A4A68(void *arg0) {
    (*(int *)((char*)(arg0) + (0x5818))) = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4A78);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sSndKey8_4A78 {
    long v;
} __attribute__((packed));
extern const sSndKey8_4A78 D_004A3650[];
extern "C" void func_002ADCA0(void*, int, long, int, int, int, int, int);
extern "C" int func_0028DEF0(void* self, int a1);

// PORT: 64-bit long sound key
extern "C" void func_002A4A78(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (*(int*)((char*)self + 0x578C) == 0 && D_00535C11[0] == 0) {
        if (func_002A3F90(self) != 0) {
            func_0028DEF0(self, 13);
            func_002ADCA0(**(char***)((char*)self + 0x118) + 0x1D8, 2, D_004A3650->v, 0, 2, 0, 0, 0);
        } else {
            char* p = **(char***)((char*)self + 0x118) + 0x1D8;
            const sSndKey8_4A78* key = D_004A3650;
            int r = func_002B4908((char*)self + 0x118);
            func_002ADCA0(p, 2, key->v, 0, 4, r, 0, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4B68);
#ifdef SKIP_ASM
extern "C" int func_002B49E0(void*);
extern "C" void func_002B11B0(void*, int);

extern "C" void func_002A4B68(cSSXAudio* self)
{
    if (func_002B49E0((char*)self + 0x118) == 401) {
        func_002B11B0((char*)self + 0x5560, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4BA8);
#ifdef SKIP_ASM
extern "C" void* func_002A4BA8(tCrowdEventData* self)
{
    int i;
    *(int*)self = 0;
    for (i = 4; i >= 0; i--) {
        *(int*)((char*)self + (i << 2) + 4) = 0;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4BE0);
#ifdef SKIP_ASM
extern "C" void func_002A4CF8(tCrowdEventData*);
void operator_delete(int*);

extern "C" void func_002A4BE0(tCrowdEventData* self, int flags)
{
    func_002A4CF8(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4C28);
#ifdef SKIP_ASM
struct sSndHandleList5 {
    int count;
    int items[5];
};
extern "C" int FILE_loadz(int a, int b);

extern "C" int func_002A4C28(sSndHandleList5* list, int name, int flags)
{
    if (list->count >= 5 || (list->items[list->count] = FILE_loadz(name, flags), list->items[list->count] == 0)) {
        return 0;
    }
    list->count++;
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4CA0);
#ifdef SKIP_ASM
unsigned int BXrand();

struct sSndRandList {
    unsigned int count;
    int items[1];
};

extern "C" int func_002A4CA0(sSndRandList* list)
{
    if (list->count == 0) {
        return 0;
    }
    return list->items[BXrand() % list->count];
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4CF8);
#ifdef SKIP_ASM
void cMemMan_free(void*);

extern "C" void func_002A4CF8(tCrowdEventData* self)
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
INCLUDE_ASM("sound/soundsys", func_002A4D68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct func_002A4D68_sTriple {
    int a;
    int b;
    int c;
};

struct func_002A4D68_sChan {
    int e0;
    int e4;
    int e8;
    char pad[0x20 - 0xC];
    int index;      // +0x20
    char pad2[0x30 - 0x24];
    func_002A4D68_sChan() {}
};

struct func_002A4D68_sObj {
    int f0;
    char pad4[0x10 - 0x4];
    int f10;
    int f14;
    int f18;
    int f1C;
    func_002A4D68_sTriple triples[15];  // 0x20
    int fD4;
    int fD8;
    int fDC;
    func_002A4D68_sChan chans[3];       // 0xE0
    int f170;
    int f174;
    int f178;
    int f17C;
    int f180;
    int f184;
    int f188;
    int f18C;
    func_002A4D68_sObj() __asm__("func_002A4D68");
};

func_002A4D68_sObj::func_002A4D68_sObj()
    : f0(-1), f10(0), f14(0), f18(0), f1C(0), fD4(0), fD8(-1),
      f170(0), f174(-1), f178(0), f17C(0), f180(0), f184(3), f188(0), f18C(0)
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
INCLUDE_ASM("sound/soundsys", func_002A4E38);
#ifdef SKIP_ASM
extern "C" void func_002A6848(cCrowdInstance* self);
extern "C" void func_002A6430(cCrowdInstance* self, float v);
void operator_delete(int*);

extern "C" void func_002A4E38(cCrowdInstance* self, int flags)
{
    func_002A6848(self);
    func_002A6430(self, 0.0f);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4E88);
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
// handle lists (func_002A4BA8 clears them; func_002A4CA0 picks from one) and their timing floats.
extern "C" void* func_002A4BA8(tCrowdEventData* self);
extern sSndCallback D_004A3700_cb[] __asm__("D_004A3700");
extern "C" void func_002A8450_c(sSndSys8140* self, int a, int b) __asm__("func_002A8450");
extern "C" int func_002A86B8_c(sSndSys8140* self) __asm__("func_002A86B8");
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
    cSndPlayOpts* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    s->p7C[m->sub.cur] = closeOnEnd;
    self->mBank[self->mpVBankManager->sub.cur] = a;
    self->mPatch[self->mpVBankManager->sub.cur] = b;
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
    int v4;                  // 0x04
    sCrowdGrp4E88* grp;      // 0x08
};
struct sCrowd4E88 {
    int mViewPort;           // 0x000
    char pad4[0xC];
    sCrowdVec vec10;          // 0x010
    sCrowdSlot4E88 slots[15]; // 0x020
    int count;               // 0x0D4
    int fD8;                 // 0x0D8
    char padDC[4];
    sCrowdGrp4E88 groups[3]; // 0x0E0
    int mode;                // 0x170
    int handle;              // 0x174
    int vol;                 // 0x178
    int f17C;                // 0x17C
    int state;               // 0x180
    char pad184[8];
    int f18C;                // 0x18C
};
// The voice cBankMonitor_BANKMONITOR_GetHandle hands back (only its volume is set here).
struct sCrowdVoice {
    char pad0[0x64];
    int vol;                 // 0x64
};
struct sCrowdVt4E88 { short delta; short index; int (*fn)(void*, int, int*, int*); };
extern "C" int func_002A4CA0(sSndRandList* list);

// func_002A4E88 is declared with a void* parameter further down (the crowd update loop).
void func_002A4E88_c(sCrowd4E88* self) __asm__("func_002A4E88");
void func_002A4E88_c(sCrowd4E88* self)
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
    int best = pCurrCrowdObjectInfo->v4;
    chanPos[pCurrAmbientInfo->idx] = mean;
    chanDist[pCurrAmbientInfo->idx] = pCurrCrowdObjectInfo->v4;
    for (i = 1; i < self->count; i++) {
        do {
            pCurrCrowdObjectInfo = &self->slots[k];
            pCurrAmbientInfo = pCurrCrowdObjectInfo->grp;
            k++;
            if (pCurrCrowdObjectInfo->pCrowdObjectPos != 0) {
                break;
            }
        } while (k < 15);
        float w = (float)pCurrCrowdObjectInfo->v4 / (float)(best + pCurrCrowdObjectInfo->v4);
        mean[0] += ((*pCurrCrowdObjectInfo->pCrowdObjectPos)[0] - mean[0]) * w;
        mean[1] += ((*pCurrCrowdObjectInfo->pCrowdObjectPos)[1] - mean[1]) * w;
        mean[2] += ((*pCurrCrowdObjectInfo->pCrowdObjectPos)[2] - mean[2]) * w;
        if (best < pCurrCrowdObjectInfo->v4) {
            bonus += best >> 2;
            best = pCurrCrowdObjectInfo->v4;
        } else {
            bonus += pCurrCrowdObjectInfo->v4 >> 2;
        }
        if (self->mode >= 2) {
            w = (float)pCurrCrowdObjectInfo->v4 / (float)(chanDist[pCurrAmbientInfo->idx] + pCurrCrowdObjectInfo->v4);
            chanPos[pCurrAmbientInfo->idx][0] += ((*pCurrCrowdObjectInfo->pCrowdObjectPos)[0] - chanPos[pCurrAmbientInfo->idx][0]) * w;
            chanPos[pCurrAmbientInfo->idx][1] += ((*pCurrCrowdObjectInfo->pCrowdObjectPos)[1] - chanPos[pCurrAmbientInfo->idx][1]) * w;
            chanPos[pCurrAmbientInfo->idx][2] += ((*pCurrCrowdObjectInfo->pCrowdObjectPos)[2] - chanPos[pCurrAmbientInfo->idx][2]) * w;
            if (chanDist[pCurrAmbientInfo->idx] < pCurrCrowdObjectInfo->v4) {
                chanSum[pCurrAmbientInfo->idx] += chanDist[pCurrAmbientInfo->idx] >> 2;
                chanDist[pCurrAmbientInfo->idx] = pCurrCrowdObjectInfo->v4;
            } else {
                chanSum[pCurrAmbientInfo->idx] += pCurrCrowdObjectInfo->v4 >> 2;
            }
        }
    }
    best += bonus;
    if (self->mode == 1) {
        if (pCurrAmbientInfo->handle >= 0) {
            sCrowdVec* pos = &pCurrAmbientInfo->vec;
            pCurrAmbientInfo->vec = mean;
            sCrowdVoice* h = (sCrowdVoice*)cBankMonitor_BANKMONITOR_GetHandle(&D_004A36F8->mpVBankManager->sub, pCurrAmbientInfo->handle, 1);
            if (h == 0) {
                crowdBegin((sSndSys69A0*)D_004A36F8, self->fD8, pCurrAmbientInfo->key, 1);
                {
                    cBankManager* m2 = D_004A36F8->mpVBankManager;
                    m2->sub.tgt[m2->sub.cur] = pos;
                }
                {
                    cBankManager* m2 = D_004A36F8->mpVBankManager;
                    cSndPlayOpts* ss = &m2->sub;
                    ss->vals[m2->sub.cur] = best;
                    ss->ch[m2->sub.cur].vol = SND_MAX(0, SND_MIN(ss->vals[ss->cur], 0x7F));
                }
                {
                    cBankManager* m2 = D_004A36F8->mpVBankManager;
                    m2->sub.p3C[m2->sub.cur] = self->f17C;
                }
                {
                    cBankManager* m2 = D_004A36F8->mpVBankManager;
                    m2->sub.p74[m2->sub.cur] = 1;
                }
                OPTS_Callback(D_004A36F8->mpVBankManager, D_004A3700_cb[0]);
                {
                    cBankManager* m2 = D_004A36F8->mpVBankManager;
                    m2->sub.p60[m2->sub.cur] = (int)self;
                }
                {
                    cBankManager* m2 = D_004A36F8->mpVBankManager;
                    m2->sub.p7C[m2->sub.cur] = 0;
                }
                {
                    cBankManager* m2 = D_004A36F8->mpVBankManager;
                    m2->sub.p64[m2->sub.cur] = self->mViewPort;
                }
                pCurrAmbientInfo->handle = cBankSys_SFX_End((cBankSys*)D_004A36F8);
            } else {
                h->vol = best;
            }
        }
    } else {
        for (i = 0; i < 3; i++) {
            pCurrAmbientInfo = &self->groups[i];
            if (pCurrAmbientInfo->cnt != 0 && pCurrAmbientInfo->handle >= 0) {
                sCrowdVec* pos = &pCurrAmbientInfo->vec;
                pCurrAmbientInfo->vec = chanPos[pCurrAmbientInfo->idx];
                sCrowdVoice* h = (sCrowdVoice*)cBankMonitor_BANKMONITOR_GetHandle(&D_004A36F8->mpVBankManager->sub, pCurrAmbientInfo->handle, 1);
                if (h == 0) {
                    crowdBegin((sSndSys69A0*)D_004A36F8, self->fD8, pCurrAmbientInfo->key, 1);
                    {
                        cBankManager* m2 = D_004A36F8->mpVBankManager;
                        m2->sub.tgt[m2->sub.cur] = pos;
                    }
                    {
                        cBankManager* m2 = D_004A36F8->mpVBankManager;
                        cSndPlayOpts* ss = &m2->sub;
                        ss->vals[m2->sub.cur] = chanDist[pCurrAmbientInfo->idx] + chanSum[pCurrAmbientInfo->idx];
                        ss->ch[m2->sub.cur].vol = SND_MAX(0, SND_MIN(ss->vals[ss->cur], 0x7F));
                    }
                    {
                        cBankManager* m2 = D_004A36F8->mpVBankManager;
                        m2->sub.p3C[m2->sub.cur] = self->f17C;
                    }
                    {
                        cBankManager* m2 = D_004A36F8->mpVBankManager;
                        m2->sub.p74[m2->sub.cur] = 1;
                    }
                    {
                        cBankManager* m2 = D_004A36F8->mpVBankManager;
                        m2->sub.p64[m2->sub.cur] = self->mViewPort;
                    }
                    OPTS_Callback(D_004A36F8->mpVBankManager, D_004A3700_cb[0]);
                    {
                        cBankManager* m2 = D_004A36F8->mpVBankManager;
                        m2->sub.p60[m2->sub.cur] = (int)self;
                    }
                    {
                        cBankManager* m2 = D_004A36F8->mpVBankManager;
                        m2->sub.p7C[m2->sub.cur] = 0;
                    }
                    pCurrAmbientInfo->handle = cBankSys_SFX_End((cBankSys*)D_004A36F8);
                } else {
                    h->vol = chanDist[pCurrAmbientInfo->idx] + chanSum[pCurrAmbientInfo->idx];
                }
            }
        }
    }
    int outA = 0x1000;
    int outB = 0x1000;
    if (self->state != 0) {
        cCrowdSys* sys = D_004A36F8;
        sCrowdVt4E88* vt = (sCrowdVt4E88*)((void*)sys->mVtbl);
        int r = vt[2].fn((char*)sys + vt[2].delta, self->mViewPort, &outA, &outB);
        if (r == 1 || r == 2) {
            self->f18C = r;
        }
        if (self->state == 1 && r != 3) {
            func_002A6848((cCrowdInstance*)self);
            int e = func_002A4CA0((sSndRandList*)&((sSndSys69A0*)D_004A36F8)->ent[r].c);
            if (e != 0) {
                func_002A8450_c((sSndSys69A0*)D_004A36F8, self->fD8, e);
                {
                    cBankManager* m2 = D_004A36F8->mpVBankManager;
                    m2->sub.tgt[m2->sub.cur] = &self->vec10;
                }
                {
                    cBankManager* m2 = D_004A36F8->mpVBankManager;
                    cSndPlayOpts* ss = &m2->sub;
                    ss->vals[m2->sub.cur] = best * outB >> 12;
                    ss->ch[m2->sub.cur].vol = SND_MAX(0, SND_MIN(ss->vals[ss->cur], 0x7F));
                }
                {
                    cBankManager* m2 = D_004A36F8->mpVBankManager;
                    m2->sub.p3C[m2->sub.cur] = self->f17C;
                }
                {
                    cBankManager* m2 = D_004A36F8->mpVBankManager;
                    m2->sub.p74[m2->sub.cur] = 1;
                }
                {
                    cBankManager* m2 = D_004A36F8->mpVBankManager;
                    m2->sub.p64[m2->sub.cur] = self->mViewPort;
                }
                self->handle = func_002A86B8_c((sSndSys69A0*)D_004A36F8);
                if (self->handle >= 0) {
                    self->state = 2;
                }
            }
        }
    }
    if (self->handle >= 0) {
        sCrowdVoice* h = (sCrowdVoice*)cBankMonitor_BANKMONITOR_GetHandle(&D_004A36F8->mpVBankManager->sub, self->handle, 2);
        if (h == 0) {
            func_002A6848((cCrowdInstance*)self);
            self->f18C = 0;
        } else {
            int v = best;
            if (self->state == 2) {
                cBankMonitorInstance_PitchUpdate(h, outA);
                v = best * outB >> 12;
            }
            h->vol = v;
        }
    }
    self->vol = best;
    self->vec10 = mean;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A5CD8);
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

extern "C" void func_002A5CD8(s2A5CD8* self, int i, int newValue)
{
    if (i >= 0) {
        if (self->entries[i].active) {
            self->entries[i].value = newValue;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A5D08);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" int cBankSys_SFX_End(cBankSys* self);

struct sVec4_5D08 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sPoolGroup5D08 {
    int key;
    int handle;
    int cnt;
    int pad0C;
    sVec4_5D08 vec;
    char pad20[0x10];
};
struct sPoolSlot5D08 {
    sVec4_5D08* p;
    int v4;
    sPoolGroup5D08* grp;
};
struct sPool5D08 {
    int f0;
    char pad4[0xC];
    sVec4_5D08 vec10;
    sPoolSlot5D08 slots[15];
    int count;
    int fD8;
    char padDC[4];
    sPoolGroup5D08 groups[3];
    int f170;
    char pad174[8];
    int f17C;
};

struct sSnd5D08Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd5D08E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd5D08Sub {
    char pad0[4];
    sSnd5D08Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd5D08Ch* ch;          // 0x20
    sVec4_5D08* tgt[4];      // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd5D08E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd5D08Mgr {
    char pad0[0x1D8];
    sSnd5D08Sub sub;         // 0x1D8
};
struct sSnd5D08Sys {
    sSnd5D08Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
};
struct sSndKey8_5D08 {
    long v;
} __attribute__((packed));
extern sSndKey8_5D08 D_004A3700_5D08[] __asm__("D_004A3700");

#define SND_5D08_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_5D08_MAX(a, b) ((a) > (b) ? (a) : (b))

// PORT: 64-bit long sound key
int func_002A5D08_5D08(sPool5D08* self, sSnd5D08Sub* P, int key) __asm__("func_002A5D08");
int func_002A5D08_5D08(sPool5D08* self, sSnd5D08Sub* P, int key)
{
    int idx = -1;
    int cnt = self->count;
    if (cnt >= 15 || P->tgt[P->cur] == 0) {
        return -1;
    }
    sPoolSlot5D08* found = 0;
    int i;
    for (i = 0; i < 15; i++) {
        if (found == 0 && self->slots[i].p == 0) {
            found = &self->slots[i];
            idx = i;
        }
    }
    self->count++;
    found->p = P->tgt[P->cur];
    found->v4 = P->vals[P->cur];
    i = 0;
    do {
        if (self->groups[i].key == key) {
            self->groups[i].cnt++;
            found->grp = &self->groups[i];
            break;
        }
        i++;
    } while (i < 3);
    if (self->count == 1) {
        self->f0 = P->p64[P->cur];
        self->f17C = P->p3C[P->cur];
        self->vec10 = *found->p;
    }
    if (found->grp == 0) {
        int h = 0;
        do {
            if (self->groups[h].cnt == 0) {
                found->grp = &self->groups[h];
                self->groups[h].key = key;
                self->groups[h].cnt++;
                self->f170++;
                break;
            }
            h++;
        } while (h < 3);
        found->grp->vec = *found->p;
        cCrowdSys* sys = D_004A36F8;
        sSnd5D08Mgr* m = (sSnd5D08Mgr*)sys->mpVBankManager;
        int a = self->fD8;
        sSnd5D08Sub* s = &m->sub;
        m->sub.cur++;
        s->ch[m->sub.cur] = m->sub.def;
        m->sub.tgt[m->sub.cur] = 0;
        s->p34[m->sub.cur] = 0;
        s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
        s->p3C[m->sub.cur] = 0;
        func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
        sys->mBank[((sSnd5D08Mgr*)sys->mpVBankManager)->sub.cur] = a;
        sys->mPatch[((sSnd5D08Mgr*)sys->mpVBankManager)->sub.cur] = key;
        {
            cCrowdSys* crowdSys = D_004A36F8;
            ((sSnd5D08Mgr*)crowdSys->mpVBankManager)->sub.tgt[((sSnd5D08Mgr*)crowdSys->mpVBankManager)->sub.cur] = &found->grp->vec;
            sSnd5D08Mgr* m2 = (sSnd5D08Mgr*)crowdSys->mpVBankManager;
            sSnd5D08Sub* ss = &m2->sub;
            ss->vals[m2->sub.cur] = found->v4;
            ss->ch[m2->sub.cur].vol = SND_5D08_MAX(0, SND_5D08_MIN(ss->vals[ss->cur], 0x7F));
        }
        {
            sSnd5D08Mgr* m2 = (sSnd5D08Mgr*)D_004A36F8->mpVBankManager;
            m2->sub.p3C[m2->sub.cur] = self->f17C;
        }
        {
            sSnd5D08Mgr* m2 = (sSnd5D08Mgr*)D_004A36F8->mpVBankManager;
            m2->sub.p74[m2->sub.cur] = 1;
        }
        {
            sSnd5D08Mgr* m2 = (sSnd5D08Mgr*)D_004A36F8->mpVBankManager;
            m2->sub.p64[m2->sub.cur] = self->f0;
        }
        {
            sSnd5D08Mgr* m2 = (sSnd5D08Mgr*)D_004A36F8->mpVBankManager;
            sSndKey8_5D08 k = D_004A3700_5D08[0];
            sSnd5D08Sub* s4 = &m2->sub;
            *(sSndKey8_5D08*)((char*)s4 + (m2->sub.cur << 3) + 0x40) = k;
        }
        {
            sSnd5D08Mgr* m2 = (sSnd5D08Mgr*)D_004A36F8->mpVBankManager;
            m2->sub.p60[m2->sub.cur] = (int)self;
        }
        {
            sSnd5D08Mgr* m2 = (sSnd5D08Mgr*)D_004A36F8->mpVBankManager;
            m2->sub.p7C[m2->sub.cur] = 0;
        }
        found->grp->handle = cBankSys_SFX_End((cBankSys*)D_004A36F8);
    }
    return idx;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A62F0);
#ifdef SKIP_ASM

struct sSsVoice62F0 { int f0; int id; int refs; };
struct sSsEnt62F0 { int active; int f4; sSsVoice62F0* v; };
struct sSsMgr62F0 {
    int f0;
    char pad4[0x20 - 0x4];
    sSsEnt62F0 ent[15];         // 0x20
    int nactive;                // 0xD4
    char padD8[0x170 - 0xD8];
    int nvoices;                // 0x170
    int id;                     // 0x174
    int f178;                   // 0x178
    int f17C;
    int state;                  // 0x180
    int f184;
    float vol;                  // 0x188
    int f18C;                   // 0x18C
};

extern "C" void func_002A62F0(cCrowdInstance* vself, int idx, float fade)
{
    sSsMgr62F0* self = (sSsMgr62F0*)vself;
    if (idx < 0) return;
    if (self->ent[idx].active == 0) return;
    sSsVoice62F0* v = self->ent[idx].v;
    self->nactive--;
    self->ent[idx].active = 0;
    self->ent[idx].v = 0;
    if (--v->refs == 0) {
        if (v->id >= 0) {
            cBankMonitor_BANKMONITOR_StopHandle((char*)D_004A36F8->mpVBankManager + 0x1D8, v->id, 1, fade);
        }
        v->id = -1;
        v->f0 = -1;
        self->f18C = 0;
        self->nvoices--;
    }
    if (self->nactive != 0) return;
    if (self->vol > 0.25f) self->vol = 0.25f;
    if (self->id >= 0) {
        if (fade == 0.0f) {
            cBankMonitor_BANKMONITOR_StopHandle((char*)D_004A36F8->mpVBankManager + 0x1D8, self->id, 2, fade);
        }
        self->id = -1;
        if (self->state == 2) self->state = 1;
    }
    self->f178 = 0;
    self->f0 = -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A6430);
#ifdef SKIP_ASM
extern "C" void func_002A62F0(cCrowdInstance* voice, int b, float v);

extern "C" void func_002A6430(cCrowdInstance* self, float fade)
{
    for (int i = 0; i < 15; i++) {
        if (*(int*)((char*)self + 0x20 + i * 0xC) != 0) {
            func_002A62F0(self, i, fade);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A64A0);
#ifdef SKIP_ASM
extern "C" void func_002A6848(cCrowdInstance* self);
extern "C" int func_002A4CA0(sSndRandList* list);
extern "C" void func_002A8450(char** sys, int a, int b);
extern "C" int func_002A86B8(char** sys);

struct sSnd64A0Sys {
    char pad0[0x1C];
    tCrowdData ent[1];
};
struct sSnd64A0Ch {
    unsigned char vol;
    char pad1[0x17];
};

#define S64_MIN(a, b) ((a) < (b) ? (a) : (b))
#define S64_MAX(a, b) ((a) >= (b) ? (a) : (b))

struct sSnd64A0Sub {
    char pad0[0x1C];
    volatile int cur;   // 0x1C
    sSnd64A0Ch* ch;     // 0x20
    void* tgt[5];       // 0x24
    int* vals;          // 0x38
    int* p3C;           // 0x3C
    char pad40[0x64 - 0x40];
    int* p64;           // 0x64
    char pad68[0x74 - 0x68];
    int* p74;           // 0x74
    void setVal(int i, int v) {
        vals[i] = v;
    }
    void update(int i) {
        ch[i].vol = S64_MAX(S64_MIN(vals[cur], 0x7F), 0);
    }
};
struct sSnd64A0Mgr {
    char pad0[0x1D8];
    sSnd64A0Sub sub;    // 0x1D8
};

extern "C" void func_002A64A0(cCrowdInstance* vself, int idx)
{
    func_002A6848(vself);
    if (vself->mNumCrowdObjects == 0) return;
    tCrowdData* crowdData = &((&D_004A36F8->mCrowdData[0]))[idx];
    vself->mAmbientCheerIntensity = idx;
    vself->unk_188 = crowdData->mAttack + crowdData->mDecay;
    int r = func_002A4CA0((sSndRandList*)crowdData);
    if (r == 0) return;
    func_002A8450((char**)D_004A36F8, vself->unk_0D8, r);
    sSnd64A0Mgr* m1 = (sSnd64A0Mgr*)D_004A36F8->mpVBankManager;
    m1->sub.tgt[m1->sub.cur] = ((char*)vself) + 0x10;
    sSnd64A0Mgr* m2 = (sSnd64A0Mgr*)D_004A36F8->mpVBankManager;
    m2->sub.setVal(m2->sub.cur, vself->unk_178);
    m2->sub.update(m2->sub.cur);
    sSnd64A0Mgr* m3 = (sSnd64A0Mgr*)D_004A36F8->mpVBankManager;
    m3->sub.p3C[m3->sub.cur] = (int)vself->unk_17C;
    sSnd64A0Mgr* m4 = (sSnd64A0Mgr*)D_004A36F8->mpVBankManager;
    m4->sub.p74[m4->sub.cur] = 1;
    sSnd64A0Mgr* m5 = (sSnd64A0Mgr*)D_004A36F8->mpVBankManager;
    m5->sub.p64[m5->sub.cur] = *(int*)vself;
    vself->unk_174 = func_002A86B8((char**)D_004A36F8);
    vself->unk_18C = idx + 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A6648);
#ifdef SKIP_ASM
extern "C" void func_002A6848(cCrowdInstance* self);
extern "C" int func_002A4CA0(sSndRandList* list);
extern "C" void func_002A8450(char** sys, int a, int b);
extern "C" int func_002A86B8(char** sys);

struct sSnd6648Sys {
    char pad0[0x1C];
    tCrowdData ent[1];
};
struct sSnd6648Ch {
    unsigned char vol;
    char pad1[0x17];
};

#define S66_MIN(a, b) ((a) < (b) ? (a) : (b))
#define S66_MAX(a, b) ((a) >= (b) ? (a) : (b))

struct sSnd6648Sub {
    char pad0[0x1C];
    volatile int cur;   // 0x1C
    sSnd6648Ch* ch;     // 0x20
    void* tgt[5];       // 0x24
    int* vals;          // 0x38
    int* p3C;           // 0x3C
    char pad40[0x64 - 0x40];
    int* p64;           // 0x64
    char pad68[0x74 - 0x68];
    int* p74;           // 0x74
    void setVal(int i, int v) {
        vals[i] = v;
    }
    void update(int i) {
        ch[i].vol = S66_MAX(S66_MIN(vals[cur], 0x7F), 0);
    }
};
struct sSnd6648Mgr {
    char pad0[0x1D8];
    sSnd6648Sub sub;    // 0x1D8
};

extern "C" void func_002A6648(cCrowdInstance* vself, int idx)
{
    func_002A6848(vself);
    if (vself->mNumCrowdObjects == 0) return;
    tCrowdData* crowdData = &((&D_004A36F8->mCrowdData[0]))[idx];
    vself->mAmbientCheerIntensity = idx;
    vself->unk_188 = crowdData->mAttack + crowdData->mDecay;
    int r = func_002A4CA0((sSndRandList*)((char*)crowdData + 0x18));
    if (r == 0) return;
    func_002A8450((char**)D_004A36F8, vself->unk_0D8, r);
    sSnd6648Mgr* m1 = (sSnd6648Mgr*)D_004A36F8->mpVBankManager;
    m1->sub.tgt[m1->sub.cur] = ((char*)vself) + 0x10;
    sSnd6648Mgr* m2 = (sSnd6648Mgr*)D_004A36F8->mpVBankManager;
    m2->sub.setVal(m2->sub.cur, vself->unk_178);
    m2->sub.update(m2->sub.cur);
    sSnd6648Mgr* m3 = (sSnd6648Mgr*)D_004A36F8->mpVBankManager;
    m3->sub.p3C[m3->sub.cur] = (int)vself->unk_17C;
    sSnd6648Mgr* m4 = (sSnd6648Mgr*)D_004A36F8->mpVBankManager;
    m4->sub.p74[m4->sub.cur] = 1;
    sSnd6648Mgr* m5 = (sSnd6648Mgr*)D_004A36F8->mpVBankManager;
    m5->sub.p64[m5->sub.cur] = *(int*)vself;
    vself->unk_174 = func_002A86B8((char**)D_004A36F8);
    vself->unk_18C = ~idx;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A67F0);
#ifdef SKIP_ASM
extern "C" void func_002A67F0(cCrowdInstance* self)
{
    if (self->unk_180 == 0) {
        self->unk_180 = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A6808);
#ifdef SKIP_ASM
extern "C" void func_002A6848(cCrowdInstance* self);

extern "C" void func_002A6808(cCrowdInstance* self)
{
    if (self->unk_180 == 2) {
        func_002A6848(self);
    }
    self->unk_180 = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A6848);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

extern "C" void func_002A6848(cCrowdInstance* self)
{
    float lim = 0.25f;
    if (lim < self->unk_188) {
        self->unk_188 = lim;
    }
    self->unk_180 = 0;
    if (self->unk_174 >= 0) {
        cBankMonitor_BANKMONITOR_StopHandle((char*)D_004A36F8->mpVBankManager + 0x1D8, self->unk_174, 2, 0.0f);
        self->unk_174 = -1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A68B0);
#ifdef SKIP_ASM
extern "C" void cBankMonitorInstance_Reset(void*);
extern "C" void func_003BC6B8(int, int);

extern "C" void func_002A68B0(cCrowdInstance* self, char* obj, float dt) {
    if (*(int*)(obj + 4) < 0) {
        cBankMonitorInstance_Reset(obj);
        return;
    }
    float t = self->unk_188;
    if (!(t <= 0.0f)) {
        int idx = self->mAmbientCheerIntensity;
        cCrowdSys* base = D_004A36F8;
        float rem = t - dt;
        self->unk_188 = rem;
        char* e = (char*)base + (idx * 0x54 + 0x1C);
        float f = *(float*)(e + 0x4C);
        int vol = 0x40;
        if (f < rem) {
            float r = *(float*)(e + 0x50) * (*(float*)(e + 0x48) - (rem - f)) / *(float*)(e + 0x48);
            vol = (int)r + 0x40;
        } else if (rem > 0.0f) {
            float r = *(float*)(e + 0x50) * rem / f;
            vol = (int)r + 0x40;
        }
        func_003B58A0();
        func_003BC6B8(*(int*)(obj + 4), vol);
        func_003B58D8();
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdSys_cCrowdSys);
#ifdef SKIP_ASM
extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" char* cMidiSys_cMidiSys(cMidiSys* self, int embedded, int count, int heap);
extern "C" void* func_002A4BA8(tCrowdEventData* self);

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
    sSnd69List() { func_002A4BA8((tCrowdEventData*)(this)); }
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

extern char D_004837F8[];
extern char cBankMonitor_vtbl[];
extern const sSnd69Vt4 D_00483818_69 __asm__("D_00483818");
extern const sSnd69Vt7 D_00483838_69 __asm__("D_00483838");


/**
 * Constructs the crowd sound system.
 *
 * When @p embedded is set (a complete object) it builds the virtual bases first: cSoundSys at +0x118 and cBankManager
 * at +0x120. Then constructs the cMidiSys base (cMidiSys_cMidiSys), installs the cCrowdSys vtables (D_004837F8 at
 * mVtbl, D_00483818 / D_00483838 in the virtual bases, adjusted on the stack when not in charge), clears mNumInstances
 * and mpCrowdInstances, constructs the three tCrowdData sample lists (mCrowdData) and stores itself in D_004A36F8.
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
        char* mon = ((char*)self) + 0x118;
        *(char**)self = ((char*)self) + 0x120;
        *(char**)(((char*)self) + 0x2F8) = mon;
        cSoundSys_cSoundSys(mon, heap);
        cBankManager_cBankManager(*(void**)self, 0, count, heap);
    }
    cMidiSys_cMidiSys((cMidiSys*)self, 0, count, heap);
    *(const void**)(*(char**)(*(char**)self + 0x1D8) + 4) = &D_00483818_69;
    *(void**)(*(char**)self + 0xAB0) = cBankMonitor_vtbl;
    *(const void**)(*(char**)self + 0x1D4) = &D_00483838_69;
    if (!embedded) {
        sSnd69Vt4 soundSysVtbl = D_00483818_69;
        *(void**)(*(char**)(*(char**)self + 0x1D8) + 4) = &soundSysVtbl;
        char* soundSysBase = *(char**)(*(char**)self + 0x1D8) - 0x118;
        int adj1 = ((char*)self) - soundSysBase;
        soundSysVtbl.e[1].delta = D_00483818_69.e[1].delta + adj1;
        sSnd69Vt7 bankMgrVtbl = D_00483838_69;
        *(void**)(*(char**)self + 0x1D4) = &bankMgrVtbl;
        char* bankMgrBase = *(char**)self - 0x120;
        int adj2 = ((char*)self) - bankMgrBase;
        bankMgrVtbl.e[1].delta = D_00483838_69.e[1].delta + adj2;
    }
    self->mVtbl = (void**)D_004837F8;
    self->mNumInstances = 0;
    self->mpCrowdInstances = 0;
    new (((char*)self) + 0x1C) sSnd69Ent[3];
    D_004A36F8 = (cCrowdSys*)((char**)self);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cCrowdSys__cCrowdSys);
#ifdef SKIP_ASM
extern "C" void cBankManager__cBankManager(void* self, int flags);
extern "C" void cSoundSys__cSoundSys(void* self, int flags);
void operator_delete(int*);
extern "C" void func_002A4BE0(tCrowdEventData* self, int flags);
struct sSsMgr6F38;
extern "C" void func_002A6F38(sSsMgr6F38* self);
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

extern char D_004837F8[];
extern char cBankMonitor_vtbl[];
extern const sSnd6BVt4 D_00483818_6B __asm__("D_00483818");
extern const sSnd6BVt7 D_00483838_6B __asm__("D_00483838");

/**
 * Destroys the crowd sound system.
 *
 * Re-installs the cCrowdSys vtables, releases the crowd instances and sample banks (func_002A6F38), clears D_004A36F8,
 * destroys the three tCrowdData sample lists in reverse order (func_002A4BE0), runs the cMidiSys base destructor
 * cMidiSys__cMidiSys, destroys the virtual bases (cBankManager, cSoundSys) when bit 1 of @p flags is set and frees the
 * object when bit 0 is set.
 *
 * @param flags  destructor flags: bit 1 destroys the virtual bases, bit 0 frees the object
 */
// PORT: hand-expanded g++ 2.95 virtual-base dtor (vtable upcast-offset fixup on stack copies).
extern "C" void cCrowdSys__cCrowdSys(cCrowdSys* self, int flags)
{
    self->mVtbl = (void**)D_004837F8;
    *(const void**)(*(char**)(*(char**)self + 0x1D8) + 4) = &D_00483818_6B;
    *(void**)(*(char**)self + 0xAB0) = cBankMonitor_vtbl;
    *(const void**)(*(char**)self + 0x1D4) = &D_00483838_6B;
    if (flags == 0) {
        sSnd6BVt4 soundSysVtbl = D_00483818_6B;
        *(void**)(*(char**)(*(char**)self + 0x1D8) + 4) = &soundSysVtbl;
        char* soundSysBase = *(char**)(*(char**)self + 0x1D8) - 0x118;
        int adj1 = ((char*)self) - soundSysBase;
        soundSysVtbl.e[1].delta = D_00483818_6B.e[1].delta + adj1;
        sSnd6BVt7 bankMgrVtbl = D_00483838_6B;
        *(void**)(*(char**)self + 0x1D4) = &bankMgrVtbl;
        char* bankMgrBase = *(char**)self - 0x120;
        int adj2 = ((char*)self) - bankMgrBase;
        bankMgrVtbl.e[1].delta = D_00483838_6B.e[1].delta + adj2;
    }
    func_002A6F38((sSsMgr6F38*)self);
    D_004A36F8 = 0;
    if (((char*)self) + 0x1C != 0) {
        char* crowdEntry = ((char*)self) + 0x1C + 3 * 84;
        while (((char*)self) + 0x1C != crowdEntry) {
            crowdEntry -= 0x54;
            func_002A4BE0((tCrowdEventData*)(crowdEntry + 0x30), 2);
            func_002A4BE0((tCrowdEventData*)(crowdEntry + 0x18), 2);
            func_002A4BE0((tCrowdEventData*)crowdEntry, 2);
        }
    }
    cMidiSys__cMidiSys((cMidiSys*)self, 0);
    if (flags & 2) {
        cBankManager__cBankManager(*(void**)self, 0);
        cSoundSys__cSoundSys(*(void**)(*(char**)self + 0x1D8), 0);
    }
    if (flags & 1) operator_delete((int*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A6D18);
#ifdef SKIP_ASM
extern "C" void func_002A4E88(void* voice);

extern "C" void func_002A6D18(cCrowdSys* self)
{
    for (int i = 0; i < self->mNumInstances; i++) {
        func_002A4E88((char*)self->mpCrowdInstances + i * 0x190);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A6D78);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_002A4CF8(tCrowdEventData*);
struct sSndFileBuf;
extern "C" sSndFileBuf* FileParser_FileParser(FileParser* self, const char* name, int flags);
extern "C" int FileParser_FindSection(FileParser* self, const char* name);
extern "C" void func_002A7040(cCrowdSys* self, void* parser);
extern "C" void FileParser__FileParser(FileParser* self, int flags);
extern const char str_CrowdInstances[];
extern const char str_GLOBAL[];

struct sSnd6D78Voice : public func_002A4D68_sObj {
    void operator delete[](void* p, unsigned int size);
};
struct sSnd6D78List {
    char pad0[0x18];
};
struct sSnd6D78Ent {
    sSnd6D78List a;
    sSnd6D78List b;
    sSnd6D78List c;
    float f48;
    float f4C;
    int f50;
};
struct sSnd6D78Self {
    char pad0[0x14];
    int count;                  // 0x14
    sSnd6D78Voice* voices;      // 0x18
    sSnd6D78Ent ent[3];         // 0x1C
};
struct sSnd6D78Parser {
    int size;
    char* data;
    char* cur;
    char token[0x800];
};

extern "C" void func_002A6D78(sSnd6D78Self* self, const char* name, const char* key, int n, int v)
{
    self->count = n;
    sSnd6D78Voice*& slot = self->voices;
    slot = new (str_CrowdInstances, 0, 0) sSnd6D78Voice[n];
    for (int i = 0; i < 3; i++) {
        self->ent[i].f48 = 0.0f;
        self->ent[i].f4C = 0.0f;
        self->ent[i].f50 = 0;
        func_002A4CF8((tCrowdEventData*)(&self->ent[i].c));
        func_002A4CF8((tCrowdEventData*)(&self->ent[i].b));
        func_002A4CF8((tCrowdEventData*)(&self->ent[i].a));
    }
    for (int i = 0; i < n; i++) {
        self->voices[i].fD8 = v;
    }
    sSnd6D78Parser p;
    FileParser_FileParser((FileParser*)((sSndFileBuf*)&p), name, 0x100);
    p.cur = p.data;
    if (FileParser_FindSection((FileParser*)(&p), str_GLOBAL)) func_002A7040((cCrowdSys*)self, &p);
    p.cur = p.data;
    if (FileParser_FindSection((FileParser*)(&p), key)) func_002A7040((cCrowdSys*)self, &p);
    FileParser__FileParser((FileParser*)(&p), 2);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A6F38);
#ifdef SKIP_ASM
extern "C" void func_002A7718(cCrowdSys* self);
extern "C" void func_002A4E38(cCrowdInstance* p, int flags);

struct sSsChan6F38 {
    char a[0x18];               // 0x00
    char b[0x18];               // 0x18
    char c[0x18];               // 0x30
    int f48;
    int f4C;
    int f50;
};
struct sSsMgr6F38 {
    char pad0[0x14];
    int count;                  // 0x14
    char* arr;                  // 0x18
    sSsChan6F38 chans[3];       // 0x1C
};

extern "C" void func_002A6F38(sSsMgr6F38* self)
{
    func_002A7718((cCrowdSys*)self);
    char* base = self->arr;
    if (base != 0) {
        char* p = base + *(int*)(base - 0x10) * 400;
        while (self->arr != p) {
            p -= 0x190;
            func_002A4E38((cCrowdInstance*)p, 0);
        }
        cMemMan_free(self->arr - 0x10);
        self->arr = 0;
        self->count = 0;
    }
    for (int i = 0; i < 3; i++) {
        self->chans[i].f48 = 0;
        self->chans[i].f4C = 0;
        self->chans[i].f50 = 0;
        func_002A4CF8((tCrowdEventData*)self->chans[i].c);
        func_002A4CF8((tCrowdEventData*)self->chans[i].b);
        func_002A4CF8((tCrowdEventData*)self->chans[i].a);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A7040);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int FileParser_IsEOF(FileParser* self);
extern "C" int FileParser_AssignString(FileParser* self, const char* name, char* out);
extern "C" int FileParser_AssignFloat(FileParser* self, const char* name, float* out);
// FileParser_GetName returns the token buffer (the unit declares it void elsewhere).
extern "C" const char* FileParser_GetName_s(void* self) __asm__("FileParser_GetName");
extern const char str_TRICKpct_d[];
extern const char str_FALLpct_d[];
extern const char str_ANTICIPATEpct_d[];
extern const char str_ATTACKpct_d[];
extern const char str_DECAYpct_d[];
extern const char str_MAXBENDpct_d[];

extern "C" void func_002A7040(cCrowdSys* self, void* parser)
{
    sSnd6D78Self* s = (sSnd6D78Self*)self;
    char val[0x200];
    char key[0x200];
    while (!FileParser_IsEOF((FileParser*)parser)) {
        const char* name = FileParser_GetName_s(parser);
        for (int i = 0; i < 3; i++) {
            sprintf(key, str_TRICKpct_d, i + 1);
            if (strcasecmp(name, key) == 0) {
                FileParser_AssignString((FileParser*)parser, name, val);
                func_002A4C28((sSndHandleList5*)&s->ent[i].a, (int)val, 0);
                break;
            }
            sprintf(key, str_FALLpct_d, i + 1);
            if (strcasecmp(name, key) == 0) {
                FileParser_AssignString((FileParser*)parser, name, val);
                func_002A4C28((sSndHandleList5*)&s->ent[i].b, (int)val, 0);
                break;
            }
            sprintf(key, str_ANTICIPATEpct_d, i + 1);
            if (strcasecmp(name, key) == 0) {
                FileParser_AssignString((FileParser*)parser, name, val);
                func_002A4C28((sSndHandleList5*)&s->ent[i].c, (int)val, 0);
                break;
            }
            sprintf(key, str_ATTACKpct_d, i + 1);
            if (strcasecmp(name, key) == 0) {
                FileParser_AssignFloat((FileParser*)parser, name, &s->ent[i].f48);
                break;
            }
            sprintf(key, str_DECAYpct_d, i + 1);
            if (strcasecmp(name, key) == 0) {
                FileParser_AssignFloat((FileParser*)parser, name, &s->ent[i].f4C);
                break;
            }
            sprintf(key, str_MAXBENDpct_d, i + 1);
            if (strcasecmp(name, key) == 0) {
                FileParser_AssignFloat((FileParser*)parser, name, (float*)&s->ent[i].f50);
                break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A72C0);
#ifdef SKIP_ASM
extern "C" void func_002A72C0(cCrowdSys* self, int packedId, int* outInstance, int* outSlot)
{
    *outInstance = packedId & 0xff;
    *outSlot = packedId >> 8;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A72D8);
#ifdef SKIP_ASM
extern "C" void func_002A62F0(cCrowdInstance* voice, int b, float v);

extern "C" void func_002A72D8(cCrowdSys* self, int id, float fade)
{
    if (id >= 0 && self->mpCrowdInstances != 0) {
        int instance;
        int slot;
        func_002A72C0(self, id, &instance, &slot);
        func_002A62F0((cCrowdInstance*)((char*)self->mpCrowdInstances + instance * 0x190), slot, fade);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7340);
#ifdef SKIP_ASM
extern "C" void func_002A7340(cCrowdSys* self, int id, int value)
{
    if (id >= 0 && self->mpCrowdInstances != 0) {
        int instance;
        int slot;
        func_002A72C0(self, id, &instance, &slot);
        func_002A5CD8((s2A5CD8*)((char*)self->mpCrowdInstances + instance * 0x190), slot, value);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A73A8);
#ifdef SKIP_ASM
extern "C" void func_002A64A0(cCrowdInstance* voice, int a1);


extern "C" void func_002A73A8(cCrowdSys* self, int slot, int i)
{
    cCrowdInstance* crowdInstance = self->mpCrowdInstances;
    if (crowdInstance != 0) {
        func_002A64A0((cCrowdInstance*)(&crowdInstance[i]), slot);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A73D8);
#ifdef SKIP_ASM
extern "C" void func_002A6648(cCrowdInstance* voice, int a1);

extern "C" void func_002A73D8(cCrowdSys* self, int slot, int i)
{
    cCrowdInstance* crowdInstance = self->mpCrowdInstances;
    if (crowdInstance != 0) {
        func_002A6648((cCrowdInstance*)(&crowdInstance[i]), slot);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7408);
#ifdef SKIP_ASM
extern "C" void func_002A67F0(cCrowdInstance* voice);

extern "C" void func_002A7408(cCrowdSys* self, int i)
{
    cCrowdInstance* crowdInstance = self->mpCrowdInstances;
    if (crowdInstance != 0) {
        func_002A67F0((cCrowdInstance*)(&crowdInstance[i]));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7438);
#ifdef SKIP_ASM
extern "C" void func_002A6808(cCrowdInstance* voice);

extern "C" void func_002A7438(cCrowdSys* self, int i)
{
    cCrowdInstance* crowdInstance = self->mpCrowdInstances;
    if (crowdInstance != 0) {
        func_002A6808((cCrowdInstance*)(&crowdInstance[i]));
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A7468);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);

struct sSnd7468Ch {
    signed char vol;
    char pad1[0x17];
};
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
struct sSnd7468Sub {
    char pad0[0x4];
    sSnd7468Ch def;         // 0x04
    volatile int cur;       // 0x1C
    sSnd7468Ch* ch;         // 0x20
    void* tgt[4];           // 0x24
    int* p34;               // 0x34
    int* vals;              // 0x38
    int* p3C;               // 0x3C
    sSnd7468Pair pr[4];     // 0x40
    int* p60;               // 0x60
    int* p64;               // 0x64
    int* p68;               // 0x68
    int* p6C;               // 0x6C
    sSnd7468Env* env;       // 0x70
    int* p74;               // 0x74
    int* p78;               // 0x78
    int* p7C;               // 0x7C
    int* p80;               // 0x80
    int* p84;               // 0x84
};
struct sSnd7468Mgr {
    char pad0[0x1D8];
    sSnd7468Sub sub;        // 0x1D8
};

extern "C" void func_002A7468(char** sys)
{
    sSnd7468Mgr* m = *(sSnd7468Mgr**)sys;
    sSnd7468Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = s->def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
    s->p60[m->sub.cur] = 0;
    s->p64[m->sub.cur] = 0;
    s->p68[m->sub.cur] = 0;
    s->p6C[m->sub.cur] = 0;
    s->env[m->sub.cur].b = 0;
    s->env[m->sub.cur].a = 100;
    s->env[m->sub.cur].c = 90;
    s->env[m->sub.cur].d = 50;
    s->p74[m->sub.cur] = 0;
    s->p80[m->sub.cur] = 0;
    s->p84[m->sub.cur] = 0;
    s->p78[m->sub.cur] = 0x7F;
    s->p7C[m->sub.cur] = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7678);
#ifdef SKIP_ASM
extern "C" int func_002A5D08(void* voice, void* bank, int a1);

extern "C" int func_002A7678(cCrowdSys* self, int a1)
{
    cCrowdInstance* crowdInstance = self->mpCrowdInstances;
    if (crowdInstance == 0) {
        return -1;
    }
    char* bankMonitor = *(char**)self + 0x1D8;
    int packedId = func_002A5D08(&crowdInstance[(*(int**)(bankMonitor + 0x64))[*(int*)(bankMonitor + 0x1C)]], bankMonitor, a1);
    if (packedId >= 0) {
        char* monitor = *(char**)self + 0x1D8;
        packedId = (packedId << 8) | (*(int**)(monitor + 0x64))[*(int*)(monitor + 0x1C)];
    }
    (*(int*)(*(char**)self + 0x1F4))--;
    return packedId;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A7718);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sSndKey8_7718 {
    long v;
} __attribute__((packed));
extern const sSndKey8_7718 D_004A3700[];
extern "C" void func_002A6430(cCrowdInstance* voice, float a1);

struct sVE_7718 {
    short delta;
    short index;
    void (*fn)(void*, long);
};

// PORT: 64-bit long sound key
extern "C" void func_002A7718(cCrowdSys* self)
{
    for (int i = 0; i < self->mNumInstances; i++) {
        func_002A6430((cCrowdInstance*)((char*)self->mpCrowdInstances + i * 0x190), 0.0f);
    }
    char* sys = *(char**)self;
    sVE_7718* vt = *(sVE_7718**)(sys + 0xAB0);
    char* obj = sys + 0x1D8;
    vt[3].fn(obj + vt[3].delta, D_004A3700->v);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A77A8);
#ifdef SKIP_ASM
extern "C" void func_002A68B0(cCrowdInstance* self, char* obj, float dt);

extern "C" void func_002A77A8(cCrowdSys* self, char* obj, float dt)
{
    func_002A68B0((cCrowdInstance*)(*(char**)(obj + 0x80)), obj, dt);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A77C8);
#ifdef SKIP_ASM

extern "C" int func_002A77C8(cCrowdSys* self, int i)
{
    if (i < self->mNumInstances) {
        return (self->mpCrowdInstances)[i].unk_18C;
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

struct sSndFileBuf {
    int size;
    char* data;
    char* cur;
};

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
extern "C" sSndFileBuf* FileParser_FileParser(FileParser* self, const char* name, int flags)
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
    return ((sSndFileBuf*)self);
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
signed char FileParser_GetChar(void* self)
{
    return *(*(signed char**)((char*)self + 0x8))++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_SkipSpace);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char FileParser_GetChar(void* self);
extern "C" int FileParser_SkipComment(FileParser* self);

// FileParser_SkipSpace returns int (its body leaves a result in v0); the unit declares it void.
int FileParser_SkipSpace_r(void*) __asm__("FileParser_SkipSpace");

/**
 * Skips whitespace and comments (On Tour SkipSpace).
 *
 * Stops at the first other character; '#' and ';' comments are skipped to the end of the line (SkipComment).
 *
 * @return 1 when the data ends or the next character is '[' (a section header), else 0
 */
int FileParser_SkipSpace_r(void* self)
{
    while (**(signed char**)((char*)self + 0x8) != 0) {
        if (FileParser_SkipComment((FileParser*)self) == 1) return 1;
        signed char curChar = **(signed char**)((char*)self + 0x8);
        if (curChar != ' ' && curChar != '\t' && curChar != '\n' && curChar != '\r') {
            int atSection = 0;
            if (curChar == '[') atSection = 1;
            return atSection;
        }
        FileParser_GetChar(self);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", FileParser_SkipComment);
#ifdef SKIP_ASM
extern "C" int FileParser_FindEOL(FileParser* self);

/**
 * Skips a '#' or ';' comment up to (not past) the end of its line (FindEOL).
 *
 * @return 1 at the end of the data, else 0
 */
extern "C" int FileParser_SkipComment(FileParser* self)
{
    signed char curChar = *(signed char*)self->mPtr;
    if (curChar != 0) {
        if (curChar == '#' || curChar == ';') {
            int atEnd = FileParser_FindEOL(self);
            if (atEnd != 0) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
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
extern "C" void FileParser_SkipSectionNames(FileParser* self)
{
    FileParser_SkipSpace_r(self);
    while (*(signed char*)self->mPtr == '[') {
        FileParser_FindNextSection(self);
        FileParser_SkipSpace_r(self);
    }
    FileParser_IsEOF(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_FindEOL);
#ifdef SKIP_ASM
/**
 * Advances mPtr to the next '\r' or '\n' without consuming it.
 *
 * @return 0 at the end of a line, 1 at the end of the data
 */
extern "C" int FileParser_FindEOL(FileParser* self)
{
    while (*(signed char*)self->mPtr != 0) {
        signed char curChar = *(signed char*)self->mPtr;
        if (curChar == '\r') {
            return 0;
        }
        if (curChar == '\n') {
            return 0;
        }
        FileParser_GetChar(self);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", FileParser_FindNextSection);
#ifdef SKIP_ASM
signed char FileParser_GetChar(void* self);
extern "C" int FileParser_SkipComment(FileParser* self);
extern "C" int FileParser_ExpectToken(FileParser* self, const char* s);
extern const char D_004A3720[];

/**
 * Advances to the next section header "[name]" and returns its name (On Tour FindNextSection).
 *
 * Skips comments, reads characters until a '[', copies the name into mTempStr up to ']' and consumes the ']'
 * (ExpectToken). The copy has no length limit for the 2048-byte mTempStr and does not stop at the end of the data, so
 * a '[' without ']' runs past the buffer.
 *
 * @return mTempStr holding the name, or 0 when no further section exists
 */
extern "C" char* FileParser_FindNextSection(FileParser* self)
{
    while (*(signed char*)self->mPtr != 0) {
        FileParser_SkipComment(self);
        if (FileParser_GetChar(self) == '[') {
            int n = 0;
            if (*(signed char*)self->mPtr != ']') {
                do {
                    char* dst = (char*)self + 0xC;
                    dst[n] = FileParser_GetChar(self);
                    n++;
                } while (*(signed char*)self->mPtr != ']');
            }
            if (FileParser_ExpectToken(self, D_004A3720) == 0) {
                char* buf = (char*)self + 0xC;
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
 * @return 1 when found, 0 at the end of the data
 */
extern "C" int FileParser_FindSection(FileParser* self, const char* name)
{
    char* curName;
    while (*(signed char*)self->mPtr != 0 && (curName = FileParser_FindNextSection(self)) != 0) {
        if (strcasecmp(curName, name) == 0) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", FileParser_FindPartialSection);
#ifdef SKIP_ASM
extern "C" char* FileParser_FindNextSection(FileParser*);
extern "C" char* func_0041AD80(const char* s, const char* sub);

/**
 * Advances to the first section whose name and @p name match as prefixes either way (one starts with the other, via
 * strstr).
 *
 * @param name  the section name to match (On Tour Section)
 * @return 1 when found, 0 at the end of the data
 */
extern "C" int FileParser_FindPartialSection(FileParser* self, char* name)
{
    char* tok;
    while (*self->mPtr != 0 && (tok = FileParser_FindNextSection(self)) != 0) {
        if (func_0041AD80(name, tok) == name || func_0041AD80(tok, name) == tok) {
            return 1;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_GetString);
#ifdef SKIP_ASM
signed char FileParser_GetChar(void* self);
extern "C" void FileParser_SkipSpace(void*);
extern "C" int FileParser_ExpectToken(FileParser* self, const char* s);
extern const char D_004A3728[];

/**
 * Reads a quoted string: skips whitespace, consumes the opening '"', copies characters into mTempStr up to the
 * closing '"' and consumes it. Neither ExpectToken result is checked, and the copy has no length or end-of-data
 * limit.
 *
 * @return mTempStr holding the string
 */
extern "C" char* FileParser_GetString(FileParser* self)
{
    int n = 0;
    FileParser_SkipSpace(self);
    FileParser_ExpectToken(self, D_004A3728);
    if (*(signed char*)self->mPtr != '"') {
        char* dst = (char*)self + 0xC;
        do {
            dst[n] = FileParser_GetChar(self);
            n++;
        } while (*(signed char*)self->mPtr != '"');
    }
    FileParser_ExpectToken(self, D_004A3728);
    char* buf = (char*)self + 0xC;
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
extern "C" char* FileParser_GetName_t(void* self)
{
    int n = 0;
    FileParser_SkipSpace_r(self);
    while (**(signed char**)((char*)self + 0x8) != 0 && **(signed char**)((char*)self + 0x8) != ' ' &&
           **(signed char**)((char*)self + 0x8) != '\t' && **(signed char**)((char*)self + 0x8) != '\n' &&
           **(signed char**)((char*)self + 0x8) != '\r') {
        char* dst = (char*)self + 0xC;
        dst[n] = FileParser_GetChar(self);
        n++;
    }
    char* buf = (char*)self + 0xC;
    buf[n] = 0;
    return buf;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_GetFloat);
#ifdef SKIP_ASM
extern "C" void FileParser_SkipSpace(void*);
extern "C" void FileParser_GetName(void*);
extern "C" int func_004178B0(const char* s, const char* fmt, ...);
extern char str_pct_f[];

/**
 * Reads a token (GetName) and converts it with sscanf("%f"). The value is left uninitialised when the conversion
 * fails.
 *
 * @return the number read
 */
extern "C" float FileParser_GetFloat(FileParser* self)
{
    float value;
    FileParser_SkipSpace(self);
    FileParser_GetName(self);
    func_004178B0((char*)self + 0xC, str_pct_f, &value);
    return value;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_GetInt);
#ifdef SKIP_ASM
extern "C" void FileParser_SkipSpace(void*);
extern "C" void FileParser_GetName(void*);
extern "C" int func_004178B0(const char* s, const char* fmt, ...);
extern char str_pct_d_h[];

/**
 * Reads a token (GetName) and converts it with sscanf("%d"). The value is left uninitialised when the conversion
 * fails.
 *
 * @return the number read
 */
extern "C" int FileParser_GetInt(FileParser* self)
{
    int value;
    FileParser_SkipSpace(self);
    FileParser_GetName(self);
    func_004178B0((char*)self + 0xC, str_pct_d_h, &value);
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
extern "C" int FileParser_ExpectToken(FileParser* self, const char* name)
{
    int i = 0;
    FileParser_SkipSpace_r(self);
    while (*self->mPtr != 0 && name[i] != 0) {
        if (FileParser_GetChar(self) != name[i]) {
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
extern "C" int FileParser_IsEOF(FileParser* self)
{
    if (self->mPtr - self->mBasePtr >= self->mFileSize) {
        return 1;
    }
    FileParser_SkipSpace_r(self);
    signed char nextChar = *(signed char*)self->mPtr;
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
extern char D_004A3740[];

/**
 * Reads "= \"value\"" into @p out when the key last read (mTempStr) equals @p name, ignoring case.
 *
 * @param name  the key to accept (On Tour ID)
 * @param out  receives the string (On Tour Dest); unbounded strcpy
 * @return 1 when assigned, 0 when the key differs or no '=' follows
 */
extern "C" int FileParser_AssignString(FileParser* self, const char* name, char* out)
{
    if (strcasecmp((char*)self + 0xC, name) != 0) {
        return 0;
    }
    if (FileParser_ExpectToken(self, D_004A3740) != 0) {
        return 0;
    }
    char* str = FileParser_GetString(self);
    if (str == 0) {
        return 0;
    }
    strcpy(out, str);
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_AssignFloat);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int FileParser_ExpectToken(FileParser* self, const char* s);
extern "C" float FileParser_GetFloat(FileParser* self);
extern char D_004A3740[];

/**
 * Reads "= value" as a float into @p out when the key last read (mTempStr) equals @p name, ignoring case.
 *
 * @param name  the key to accept (On Tour ID)
 * @param out  receives the value (On Tour Dest)
 * @return 1 when assigned, 0 when the key differs or no '=' follows
 */
extern "C" int FileParser_AssignFloat(FileParser* self, const char* name, float* out)
{
    if (strcasecmp((char*)self + 0xC, name) != 0) {
        return 0;
    }
    if (FileParser_ExpectToken(self, D_004A3740) != 0) {
        return 0;
    }
    *out = FileParser_GetFloat(self);
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", FileParser_AssignInt);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int FileParser_ExpectToken(FileParser* self, const char* s);
extern "C" int FileParser_GetInt(FileParser* self);
extern char D_004A3740[];

/**
 * Reads "= value" as an int into @p out when the key last read (mTempStr) equals @p name, ignoring case.
 *
 * @param name  the key to accept (On Tour ID)
 * @param out  receives the value (On Tour Dest)
 * @return 1 when assigned, 0 when the key differs or no '=' follows
 */
extern "C" int FileParser_AssignInt(FileParser* self, const char* name, int* out)
{
    if (strcasecmp((char*)self + 0xC, name) != 0) {
        return 0;
    }
    if (FileParser_ExpectToken(self, D_004A3740) != 0) {
        return 0;
    }
    *out = FileParser_GetInt(self);
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", cMidiSys_cMidiSys);
#ifdef SKIP_ASM
extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" char* cBankSys_cBankSys(cBankSys* self, int embedded, int count, int heap);
extern "C" void func_003B97D8();
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

extern char D_00483A48[];
extern char cBankMonitor_vtbl[];
extern const char str_MIDIBANKS[];
extern const sSndCtVt4 D_004839F0_ct __asm__("D_004839F0");
extern const sSndCtVt7 D_00483A10_ct __asm__("D_00483A10");

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
        char* mon = ((char*)self) + 0x14;
        *(char**)self = ((char*)self) + 0x1C;
        *(char**)(((char*)self) + 0x1F4) = mon;
        cSoundSys_cSoundSys(mon, heap);
        cBankManager_cBankManager(*(void**)self, 0, count, heap);
    }
    cBankSys_cBankSys((cBankSys*)self, 0, count, heap);
    *(const void**)(*(char**)(*(char**)self + 0x1D8) + 4) = &D_004839F0_ct;
    *(void**)(*(char**)self + 0xAB0) = cBankMonitor_vtbl;
    *(const void**)(*(char**)self + 0x1D4) = &D_00483A10_ct;
    if (!embedded) {
        sSndCtVt4 vtbl1 = D_004839F0_ct;
        *(void**)(*(char**)(*(char**)self + 0x1D8) + 4) = &vtbl1;
        char* base1 = *(char**)(*(char**)self + 0x1D8) - 0x14;
        int adj1 = ((char*)self) - base1;
        vtbl1.e[1].delta = D_004839F0_ct.e[1].delta + adj1;
        sSndCtVt7 vtbl2 = D_00483A10_ct;
        *(void**)(*(char**)self + 0x1D4) = &vtbl2;
        char* base2 = *(char**)self - 0x1C;
        int adj2 = ((char*)self) - base2;
        vtbl2.e[1].delta = D_00483A10_ct.e[1].delta + adj2;
    }
    self->mVtbl = (void**)D_00483A48;
    func_003B97D8();
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
extern "C" void func_003B9880();

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

extern char D_00483A48[];
extern const sSndDtBVt4 D_004839F0;
extern const sSndDtBVt7 D_00483A10;
extern char cBankMonitor_vtbl[];

/**
 * Destroys the MIDI sound system.
 *
 * Re-installs the cMidiSys vtables (D_00483A48 at mVtbl, D_004839F0 / D_00483A10 in the virtual bases, adjusted on the
 * stack when not in charge), frees mMidiBanks, clears D_004A3744 (the instance the ctor stored), calls func_003B9880,
 * runs the cBankSys base destructor, destroys the virtual bases when bit 1 of @p flags is set and frees the object when
 * bit 0 is set. The matching constructor is cMidiSys_cMidiSys.
 *
 * @param flags  destructor flags: bit 1 destroys the virtual bases, bit 0 frees the object
 */
// PORT: hand-expanded g++ 2.95 virtual-base dtor (vtable upcast-offset fixup on stack copies).
extern "C" void cMidiSys__cMidiSys(cMidiSys* self, int flags)
{
    self->mVtbl = (void**)D_00483A48;
    *(void**)(*(char**)(*(char**)self + 0x1D8) + 4) = (void*)&D_004839F0;
    *(void**)(*(char**)self + 0xAB0) = cBankMonitor_vtbl;
    *(void**)(*(char**)self + 0x1D4) = (void*)&D_00483A10;
    if (flags == 0) {
        sSndDtBVt4 soundSysVtbl = D_004839F0;
        *(void**)(*(char**)(*(char**)self + 0x1D8) + 4) = &soundSysVtbl;
        char* soundSysBase = *(char**)(*(char**)self + 0x1D8) - 0x14;
        int adj1 = ((char*)self) - soundSysBase;
        soundSysVtbl.e[1].delta = D_004839F0.e[1].delta + adj1;
        sSndDtBVt7 bankMgrVtbl = D_00483A10;
        *(void**)(*(char**)self + 0x1D4) = &bankMgrVtbl;
        char* bankMgrBase = *(char**)self - 0x1C;
        int adj2 = ((char*)self) - bankMgrBase;
        bankMgrVtbl.e[1].delta = D_00483A10.e[1].delta + adj2;
    }
    if (self->mMidiBanks) cMemMan_free(self->mMidiBanks);
    D_004A3744 = 0;
    func_003B9880();
    cBankSys__cBankSys((cBankSys*)self, 0);
    if (flags & 2) {
        cBankManager__cBankManager(*(void**)self, 0);
        cSoundSys__cSoundSys(*(void**)(*(char**)self + 0x1D8), 0);
    }
    if (flags & 1) operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A8450);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);

struct sSnd8450Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd8450E8 {
    int a;
    int b;
};
struct sSnd8450E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd8450Sub {
    char pad0[4];
    sSnd8450Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd8450Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    sSnd8450E8 e40[4];       // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sSnd8450E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd8450Mgr {
    char pad0[0x1D8];
    sSnd8450Sub sub;         // 0x1D8
};
struct sSnd8450Sys {
    sSnd8450Mgr* mgr;
    char pad4[0xC];
    sSnd8450E8* e10;         // 0x10
};

extern "C" void func_002A8450(char** sys, int a, int b)
{
    sSnd8450Sys* self = (sSnd8450Sys*)sys;
    sSnd8450Mgr* m = self->mgr;
    sSnd8450Sub* s = &m->sub;
    m->sub.cur++;
    s->ch[m->sub.cur] = m->sub.def;
    m->sub.tgt[m->sub.cur] = 0;
    s->p34[m->sub.cur] = 0;
    s->vals[m->sub.cur] = s->ch[m->sub.cur].vol;
    s->p3C[m->sub.cur] = 0;
    func_00416210((char*)s + m->sub.cur * 8 + 0x40, 0, 8);
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
    self->e10[self->mgr->sub.cur].a = a;
    self->e10[self->mgr->sub.cur].b = b;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A86B8);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B9A00(void* ch, int id, int bank, int a3);
extern "C" void* cBankMonitor_BANKMONITOR_Create(void* sub, int h, int vol, int mode);
void func_002ABD38(void* self, int a1, int a2);
extern "C" void func_002ADCA0(void*, int, long, int, int, int, int, int);

struct sSnd86B8Key {
    long v;
} __attribute__((packed));
// PORT: 64-bit long sound key (view of D_004A3748, declared later in the unit).
extern const sSnd86B8Key D_004A3748_86B8[] __asm__("D_004A3748");

struct sSnd86B8Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd86B8Ent {
    int type;
    int id;
    char pad8[0x58];
};
struct sSnd86B8Q {
    int step;
    int count;
    int f8;
    int fC;
};
struct sSnd86B8Sub {
    char pad0[4];
    sSnd86B8Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd86B8Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    float* p6C;              // 0x6C
    sSnd86B8Q* p70;          // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd86B8Mgr {
    char pad0[0x1D8];
    sSnd86B8Sub sub;         // 0x1D8
    char pad260[0xACC - 0x260];
    sSnd86B8Ent* ents;       // 0xACC
};
struct sSnd86B8Pair {
    int id;
    int f4;
};
struct sSnd86B8Sys {
    sSnd86B8Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[4];
    sSnd86B8Pair* p10;       // 0x10
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
static inline int snd86B8Status(sSnd86B8Ent* e)
{
    return e->type;
}
static inline int snd86B8InstHandle(sSnd86B8Ent* e)
{
    return e->id;
}
static inline bool snd86B8Used(sSnd86B8Mgr* m, int id)
{
    return snd86B8Status(&m->ents[id]) == 1;
}
static inline int snd86B8Handle(sSnd86B8Mgr* m, int id)
{
    if (snd86B8Used(m, id)) return snd86B8InstHandle(&m->ents[id]);
    return -1;
}

extern "C" int func_002A86B8(char** vself)
{
    sSnd86B8Sys* self = (sSnd86B8Sys*)vself;
    sSnd86B8Mgr* m = self->mgr;
    sSnd86B8Sub* s = &m->sub;
    int BaseVol = s->vals[s->cur];
    if (m->sub.tgt[m->sub.cur] != 0) {
        s->vals[m->sub.cur] = 0;
        s->ch[m->sub.cur].vol = SND_86B8_MAX(0, SND_86B8_MIN(s->vals[s->cur], 0x7F));
    } else if (s->p3C[s->cur] != 0) {
        float gain = *(float*)s->p3C[s->cur];
        s->vals[m->sub.cur] = (int)SND_86B8_FMAX(SND_86B8_MIN((float)s->vals[s->cur] * gain, 127.0f), 0.0f);
        s->ch[s->cur].vol = SND_86B8_MAX(0, SND_86B8_MIN(s->vals[s->cur], 0x7F));
    }
    int Inst = self->mgr->sub.cur;
    int Handle = -1;
    int bank = snd86B8Handle(self->mgr, self->p10[Inst].id);
    sSnd86B8Sub* s2 = &self->mgr->sub;
    if (s2->p6C[s2->cur] == 0.0f && bank >= 0) {
        int sound = self->p10[Inst].f4;
        func_003B58A0();
        Handle = func_003B9A00(&self->mgr->sub.ch[self->mgr->sub.cur], sound, bank, -1);
        func_003B58D8();
    }
    void* Mon = cBankMonitor_BANKMONITOR_Create(&self->mgr->sub, Handle, BaseVol, 2);
    if (Mon != 0) {
        func_002ABD38(Mon, self->p10[Inst].f4, bank);
    }
    sSnd86B8Q* Delay = &self->mgr->sub.p70[self->mgr->sub.cur];
    if (Delay->count > 0) {
        int DelayTime = 0;
        int Vol = BaseVol * Delay->fC / 100;
        while (Delay->count > 0) {
            Delay->count--;
            DelayTime += Delay->step;
            Vol = Vol * Delay->f8 / 100;
            if (Vol > 0x7F) Vol = 0x7F;
            if (Vol < 0) Vol = 0;
            if (Vol == 0) break;
            sSnd86B8Sub* opts = &self->mgr->sub;
            func_002ADCA0(opts, DelayTime, D_004A3748_86B8->v, opts->p3C[opts->cur], self->p10[Inst].id, self->p10[Inst].f4, Vol, 0);
        }
    }
    self->mgr->sub.cur--;
    return Handle;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A8B28);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_002A86B8(char** sys);

extern "C" int func_002A8B28(cMidiSys* midiSys, char* obj)
{
    func_002A8450((char**)D_004A3744, *(int*)(obj + 0x1C), *(int*)(obj + 0x20));
    sSnd8450Mgr* m = (sSnd8450Mgr*)D_004A3744->mpVBankManager;
    m->sub.p3C[m->sub.cur] = *(int*)(obj + 0x18);
    sSnd8450Mgr* m2 = (sSnd8450Mgr*)D_004A3744->mpVBankManager;
    sSnd8450Sub* s = &m2->sub;
    s->vals[m2->sub.cur] = *(int*)(obj + 0x24);
#define SND_X (s->vals[s->cur])
#define SND_MIN127 (SND_X < 0x7F ? SND_X : 0x7F)
    s->ch[m2->sub.cur].vol = SND_MIN127 < 0 ? 0 : SND_MIN127;
    func_002A86B8((char**)D_004A3744);
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_cPS2SoundManager);
#ifdef SKIP_ASM
extern char D_004835F8[];

/**
 * Constructs the PS2 sound manager.
 *
 * Stores the vtable D_004835F8 and resets the state: no mSoundHeap, driver mode unk_008 0, mMainMemHighAddr -1, empty
 * unused-node and free-block lists (unk_310 / unk_318 -1) and load limits off (unk_31C 0). The nodes are set up later
 * by cPS2SoundManager_Init.
 *
 * @return @p self
 */
extern "C" void* cPS2SoundManager_cPS2SoundManager(cPS2SoundManager* self)
{
    *(void**)self = D_004835F8;
    self->mSoundHeap = 0;
    self->unk_008 = 0;
    self->mMainMemHighAddr = -1;
    self->unk_310 = -1;
    self->unk_31C = 0;
    self->unk_318 = -1;
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
 * Restores the class vtable (D_004835F8), frees mSoundHeap with cMemMan_free when set, and deletes the object when bit
 * 0 of @p flags is set.
 *
 * @param flags  g++ 2.95 in-charge flags; bit 0 deletes the object
 */
extern "C" void cPS2SoundManager__cPS2SoundManager(cPS2SoundManager* self, int flags)
{
    *(void**)self = D_004835F8;
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
extern "C" void func_003B5478();
extern "C" void func_003B5158(void* buf, int size, int flags);
extern "C" int func_003B5B68(int* out);
extern const char str_mSoundHeap[];

struct sSndVtE8CB0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};



/**
 * Initialises the PS2 sound manager: allocates its sound heap and resets the sound-memory node list.
 *
 * Allocates mSoundHeap (0x32000 bytes, tag "mSoundHeap"), calls func_003B5478, applies @p mode through virtual slot 2
 * (SetMode), registers the heap with func_003B5158 (flags 0x80303), then links the 32 sound-memory nodes into a free
 * list (each with no address range and size 0; free head 31, +0x318 = -1) and records the main-memory high address from
 * func_003B5B68 in mMainMemHighAddr.
 *
 * @param mode  the sound mode (On Tour eSOUNDDEF_MODE mode)
 */
extern "C" void cPS2SoundManager_Init(cPS2SoundManager* self, int mode)
{
    self->mSoundHeap = operator_new_tag(0x32000, str_mSoundHeap, 0, 0);
    func_003B5478();
    sSndVtE8CB0* vt = ((sSndVtE8CB0*)self->mVtbl);
    vt[2].fn((char*)self + vt[2].delta, mode);
    func_003B5158(self->mSoundHeap, 0x32000, 0x80303);
    self->unk_310 = 31;
    self->unk_318 = -1;
    for (int i = 31; i >= 0; i--) {
        self->mNodes[i].index = i;
        self->mNodes[i].lowAddr = -1;
        self->mNodes[i].highAddr = -1;
        self->mNodes[i].size = 0;
        self->mNodes[i].state = 0;
        self->mNodes[i].next = i - 1;
    }
    self->unk_31C = 0;
    int blockAddr = 0;
    int blockSize = func_003B5B68(&blockAddr);
    self->mMainMemHighAddr = blockAddr + blockSize;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_SetMode);
#ifdef SKIP_ASM
extern "C" void func_003B4DD8(void* cfg);
extern "C" void func_003B4FC0(void* cfg);
extern "C" void func_003BC930();

struct sSnd8D90Cfg {
    char pad0[0x28];
    short f28;
    unsigned short f2A;
    short f2C;
    unsigned char f2E;
    unsigned char f2F;
    unsigned char f30;
    char pad31[0x3F - 0x31];
    unsigned char f3F;
    unsigned char f40;
    unsigned char f41;
    char pad42[0x47 - 0x42];
    unsigned char f47;
    char pad48;
    unsigned char f49;
    char pad4A[0x4C - 0x4A];
    unsigned char f4C;
    char pad4D;
    short f4E;
    short f50;
    short f52;
    short f54;
    short f56;
    short f58;
    char pad5A[0xE0 - 0x5A];
};

/**
 * Switches the sound driver configuration for an audio mode.
 *
 * Fills a driver configuration (func_003B4DD8 defaults, then rates 36000/24000 and fixed levels) with per-mode voice
 * and buffer counts: mode 3 (calls func_003BC930, records unk_008 = 2), mode 2 and modes 0/1 (record unk_008 = 1), and
 * applies it with func_003B4FC0; any other mode applies the defaults. Likely bug: the early return tests the other
 * group's value (mode 3 returns 1 when unk_008 is 1, modes 0-2 when it is 2), so re-applying the current setting goes
 * through and switching between the two groups never does.
 *
 * @param mode  the audio mode (On Tour eSOUNDDEF_MODE mode)
 * @return 1 when the call was refused, 2 when the configuration was applied (On Tour eSOUNDMANAGER_SETMODE)
 */
extern "C" int cPS2SoundManager_SetMode(cPS2SoundManager* self, int mode)
{
    sSnd8D90Cfg cfg;
    func_003B4DD8(&cfg);
    cfg.f2A = 36000;
    cfg.f2C = 24000;
    cfg.f3F = 0;
    cfg.f40 = 50;
    cfg.f41 = 50;
    cfg.f28 = 20;
    switch (mode) {
    case 3:
        if (self->unk_008 == 1) return 1;
        cfg.f2E = 0x10;
        cfg.f2F = 8;
        cfg.f30 = 0x18;
        cfg.f47 = 4;
        cfg.f4C = 3;
        cfg.f4E = 40;
        cfg.f50 = 288;
        cfg.f52 = 36;
        cfg.f49 = 2;
        func_003BC930();
        self->unk_008 = 2;
        break;
    case 2:
        if (self->unk_008 == 2) return 1;
        cfg.f2E = 0x10;
        cfg.f2F = 8;
        cfg.f30 = 0x30;
        cfg.f49 = 1;
        cfg.f47 = 2;
        cfg.f4C = 6;
        cfg.f4E = 72;
        cfg.f50 = 320;
        cfg.f52 = 68;
        cfg.f54 = 40;
        cfg.f56 = 288;
        cfg.f58 = 36;
        self->unk_008 = 1;
        break;
    case 0:
    case 1:
        if (self->unk_008 == 2) return 1;
        cfg.f2E = 0x10;
        cfg.f2F = 8;
        cfg.f30 = 0x30;
        cfg.f49 = 1;
        cfg.f47 = 2;
        cfg.f4C = 3;
        cfg.f4E = 40;
        cfg.f50 = 288;
        cfg.f52 = 36;
        self->unk_008 = 1;
        break;
    }
    func_003B4FC0(&cfg);
    return 2;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_Shutdown);
#ifdef SKIP_ASM
extern "C" void func_003B5B20(int a, int b);
extern "C" void func_003B5320();
void cMemMan_free(void*);

/**
 * Shuts the PS2 sound manager down and releases its sound heap.
 *
 * Clears the sound-memory load limits (func_003B5B20(-1, -1)), restores the sound library (func_003B5320), frees
 * mSoundHeap if set and zeroes it.
 */
extern "C" void cPS2SoundManager_Shutdown(cPS2SoundManager* self)
{
    void* soundHeap;
    func_003B5B20(-1, -1);
    func_003B5320();
    soundHeap = self->mSoundHeap;
    if (soundHeap != 0) {
        cMemMan_free(soundHeap);
    }
    self->mSoundHeap = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A8FA0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cPS2SoundManager_cPS2SoundManager(cPS2SoundManager* self);
extern const char str_PS2SoundManager[];

extern "C" void* func_002A8FA0()
{
    return cPS2SoundManager_cPS2SoundManager((cPS2SoundManager*)cMemMan_alloc(800, str_PS2SoundManager, 0, 0));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_AllocSoundMem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003B5B20(int a, int b);
extern "C" int func_003B5B68(int* out);
// The unit defines func_002A92B8 with its own entry/pool view types; bind this unit's struct view.
tPS2SoundMemNode* func_002A92B8_v(cPS2SoundManager* self) __asm__("func_002A92B8");

/**
 * Allocates a block of sound memory and returns its handle.
 *
 * First walks the free-block list (unk_318, sorted by address): a block of exactly @p size is marked used and unlinked,
 * a bigger one is split (a new node from func_002A92B8 takes its top @p size bytes). Otherwise carves the block off the
 * top of main sound memory: fails with -1 when func_003B5B68 reports less free memory than @p size, with -2 while load
 * limits are on (unk_31C), else lowers mMainMemHighAddr by @p size and tells the driver (func_003B5B20). No check that
 * func_002A92B8 found a node. Likely bug: an exact fit at the head of the free list is never unlinked (unk_318 keeps
 * pointing at the now-used node, whose next is cleared), so the rest of the free list is lost and the next request of
 * that size gets the same block again.
 *
 * @param size  bytes wanted
 * @return the node index (the memory handle), -1 when out of memory, -2 while load limits are set
 */
extern "C" int cPS2SoundManager_AllocSoundMem(cPS2SoundManager* self, int size)
{
    int i = self->unk_318;
    if (i >= 0) {
        tPS2SoundMemNode* prev = 0;
        tPS2SoundMemNode* ps2SoundMemNode = &((&self->mNodes[0]))[i];
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
                tPS2SoundMemNode* n = func_002A92B8_v((self));
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
            ps2SoundMemNode = &((&self->mNodes[0]))[ps2SoundMemNode->next];
        }
    }
    int avail = 0;
    if (func_003B5B68(&avail) < size) {
        return -1;
    }
    if (self->unk_31C != 0) {
        return -2;
    }
    tPS2SoundMemNode* n = func_002A92B8_v((self));
    int top = self->mMainMemHighAddr;
    n->state = 2;
    n->lowAddr = top - size;
    n->next = -1;
    n->highAddr = top;
    n->size = size;
    func_003B5B20(-1, self->mMainMemHighAddr -= size);
    return n->index;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_DeallocSoundMem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_002A92F8_v(cPS2SoundManager* self, tPS2SoundMemNode* e) __asm__("func_002A92F8");
/**
 * Frees a block of sound memory allocated by cPS2SoundManager_AllocSoundMem (cPS2SoundManager_AllocSoundMem).
 *
 * Marks node @p memHandle free. A block at mMainMemHighAddr goes back to main memory (func_002A92F8), followed by every free
 * block that now sits at the new top; any other block is inserted into the free-block list (unk_318) in address order.
 *
 * @param memHandle  the memory handle (node index) AllocSoundMem returned (On Tour memHandle)
 */
extern "C" void cPS2SoundManager_DeallocSoundMem(cPS2SoundManager* self, int memHandle)
{
    tPS2SoundMemNode* n = &((&self->mNodes[0]))[memHandle];
    n->state = 1;
    if (n->lowAddr == self->mMainMemHighAddr) {
        func_002A92F8_v((self), n);
        while (self->unk_318 >= 0) {
            tPS2SoundMemNode* ps2SoundMemNode = &((&self->mNodes[0]))[self->unk_318];
            if (ps2SoundMemNode->lowAddr != self->mMainMemHighAddr) return;
            self->unk_318 = ps2SoundMemNode->next;
            func_002A92F8_v((self), ps2SoundMemNode);
        }
    } else if (self->unk_318 < 0) {
        n->next = self->unk_318;
        self->unk_318 = n->index;
    } else {
        tPS2SoundMemNode* prev = 0;
        tPS2SoundMemNode* freeNode = &((&self->mNodes[0]))[self->unk_318];
        for (;;) {
            if (n->lowAddr < freeNode->lowAddr) {
                if (prev) {
                    n->next = prev->next;
                    prev->next = n->index;
                } else {
                    n->next = self->unk_318;
                    self->unk_318 = n->index;
                }
                return;
            }
            if (freeNode->next < 0) {
                freeNode->next = n->index;
                n->next = -1;
                return;
            }
            prev = freeNode;
            freeNode = &((&self->mNodes[0]))[freeNode->next];
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_SetSoundMemLoadLimits);
#ifdef SKIP_ASM
extern "C" void func_003B5B20(int a, int b);


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
 * Sets +0x31C to 1 and calls func_003B5B20 with the node's lowAddr and highAddr.
 *
 * @param memHandle  index of the sound-memory node
 */
extern "C" void cPS2SoundManager_SetSoundMemLoadLimits(cPS2SoundManager* self, int memHandle)
{
    self->unk_31C = 1;
    tPS2SoundMemNode* ps2SoundMemNode = &((&self->mNodes[0]))[memHandle];
    func_003B5B20(ps2SoundMemNode->lowAddr, ps2SoundMemNode->highAddr);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", cPS2SoundManager_RestoreSoundMemLoadLimits);
#ifdef SKIP_ASM
extern "C" void func_003B5B20(int a, int b);

/**
 * Lifts the sound-memory load limit set by cPS2SoundManager_SetSoundMemLoadLimits.
 *
 * Calls func_003B5B20(-1, mMainMemHighAddr) and clears +0x31C.
 */
extern "C" void cPS2SoundManager_RestoreSoundMemLoadLimits(cPS2SoundManager* self)
{
    func_003B5B20(-1, self->mMainMemHighAddr);
    self->unk_31C = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A92B8);
#ifdef SKIP_ASM

struct s2A92B8 {
    char pad[0xC];
    tPS2SoundMemNode entries[32];
    int unk30C;
    int freeHead;
};

extern "C" tPS2SoundMemNode* func_002A92B8(cPS2SoundManager* self)
{
    int i = self->unk_310;
    if (i < 0) {
        return 0;
    }
    tPS2SoundMemNode* ps2SoundMemNode = &((&self->mNodes[0]))[i];
    self->unk_310 = ps2SoundMemNode->next;
    ps2SoundMemNode->next = -1;
    return ps2SoundMemNode;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A92F8);
#ifdef SKIP_ASM
extern "C" void func_003B5B20(int a, int b);

struct s2A92F8Entry {
    int index;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int next;
};

extern "C" void func_002A92F8(cPS2SoundManager* self, s2A92F8Entry* e)
{
    self->mMainMemHighAddr = e->unk8;
    e->unk4 = -1;
    e->unk8 = -1;
    e->unkC = 0;
    e->unk10 = 0;
    e->next = self->unk_310;
    self->unk_310 = e->index;
    if (self->unk_31C == 0) {
        func_003B5B20(-1, self->mMainMemHighAddr);
    }
}
#endif

