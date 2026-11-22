#include "common.h"
#include "world/instancerecord.h"
#include "vtable.h"
#include "intersect/bxnash.h"
#include "sound/ssxaudio.h"
#include "main/game.h"
#include "sound/icepick/worldsound.h"
#include "sound/icepick/worldtriggermanager.h"
#include "ai/rider.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cWorldTriggerManager_cWorldTriggerManager(cWorldTriggerManager* self);
extern const char str_WorldTriggerManager[];

//99.88%
INCLUDE_ASM("sound/icepick/worldtriggermanager", WORLDTRIGGERMANAGER_Init__Fv);
#ifdef SKIP_ASM
/**
 * Creates the world trigger manager (gWorldTriggerManager, 0x344 bytes "WorldTriggerManager") if it does not
 * exist.
 */
void WORLDTRIGGERMANAGER_Init()
{
    if (gWorldTriggerManager == 0) {
        void* mem = cMemMan_alloc(836, str_WorldTriggerManager, 0, 0);
        gWorldTriggerManager = (cWorldTriggerManager*)(cWorldTriggerManager_cWorldTriggerManager((cWorldTriggerManager*)mem));
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", WORLDTRIGGERMANAGER_DeInit);
#ifdef SKIP_ASM
extern "C" void cWorldTriggerManager__cWorldTriggerManager(char** self, int flags);

/**
 * Destroys the global world trigger manager, if there is one, and clears the pointer.
 *
 * Calls the destructor with flags 3, so the object's memory is freed too.
 */
extern "C" void WORLDTRIGGERMANAGER_DeInit()
{
    if (gWorldTriggerManager != 0) {
        cWorldTriggerManager__cWorldTriggerManager((char**)gWorldTriggerManager, 3);
    }
    gWorldTriggerManager = 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance_cWorldTriggerInstance);
#ifdef SKIP_ASM
extern unsigned char D_004A35A8[];

/**
 * Constructs a world trigger instance (0x70 bytes): clears every field, audio handle -1, last frame -1.0.
 *
 * Sets the handle (+0x30) to -1, the position's w (+0x2C) to 1.0, the swap string (+0x40) empty and the last animation
 * frame (+0x68) to -1.0; everything else is 0. The cWorldTriggerManager constructor runs it on each of its 40
 * instances.
 *
 * @return self
 */
extern "C" void* cWorldTriggerInstance_cWorldTriggerInstance(cWorldTriggerInstance* self)
{
    self->m_iAudioHandle = -1;
    self->m_vPosition.w = 1.0f;
    self->m_bInUse = 0;
    self->m_iObjectRef = 0;
    self->m_iVolNum = 0;
    self->m_eAudioTrigger = 0;
    *(int*)&self->m_fVolume = 0;
    self->m_bPositional = 0;
    *(int*)&self->m_vPosition = 0;
    *(int*)&self->m_vPosition.y = 0;
    *(int*)&self->m_vPosition.z = 0;
    self->m_bTouch = 0;
    self->m_bCrowdTrigger = 0;
    self->m_iSwapString[0] = D_004A35A8[0];
    self->m_iLastAnimFrame = -1.0f;
    self->m_iBankReference = 0;
    self->m_iPatchReference = 0;
    self->m_bLastAnimFrameDecrease = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance__cWorldTriggerInstance);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys a world trigger instance: nothing to release; deletes the object when bit 0 of flags is set.
 *
 * cWorldTriggerManager__cWorldTriggerManager calls it with 0 for each instance.
 *
 * @param flags  g++ 2.95 destructor flags (bit 0: operator delete the object)
 */
extern "C" void cWorldTriggerInstance__cWorldTriggerInstance(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance_SameTriggerInstance);
#ifdef SKIP_ASM
/**
 * Returns whether a world trigger instance is for the given object, audio volume and viewport.
 *
 * Compares objectRef with +0x4, volNum with +0x8 and viewPort with +0x3C.
 *
 * @param objectRef  the object reference
 * @param volNum  the audio volume number within the object
 * @param viewPort  the viewport
 * @return 1 on a match, else 0
 */
extern "C" int cWorldTriggerInstance_SameTriggerInstance(cWorldTriggerInstance* self, int objectRef, int volNum, int viewPort)
{
    if (objectRef == self->m_iObjectRef
        && volNum == self->m_iVolNum
        && viewPort == self->m_iViewPort) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance_Update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sWorldTrigger70;
struct sTriggerIdCache;
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(void* self);
extern "C" char* cSSXAudio_GetFocusRider(void* self, int which);
extern "C" int func_004165A8(const void* a, const void* b);
extern "C" int func_0029B818(void* sys, void* name, int* rec);
extern "C" void func_002A7468(void* sys);
extern "C" int func_002A7678(void* sys, int a);
extern "C" void func_002A7340(void* sys, int idx, int v);
extern "C" void* cBankMonitor_BANKMONITOR_GetHandle(void* sub, int idx, int a2);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* self, int idx, int a2, float v);
extern "C" int cWorldTriggerInstance_CrowdTrigger(void* self, int idx);
extern "C" int cWorldTriggerInstance_TriggerToPatch(void* self, int idx, int* type, int* a, int* b, int c, int d);
extern "C" int cWorldTriggerInstance_SwapTriggerToString(void* self, int idx, char* dst);
extern "C" void cWorldTriggerInstance_Reset(cWorldTriggerInstance* self);
extern "C" int cWorldTriggerManager_PlayingTriggerInstance(sWorldTrigger70** self, int a1, int a2, sWorldTrigger70* except);
extern "C" int cWorldTriggerManager_SwapTriggerPlaying(sWorldTrigger70** self, int id);
extern "C" int cWorldTriggerManager_AfterCollisionTrigger(cWorldTriggerManager* self, unsigned int i);
extern "C" int cWorldTriggerManager_ObjectHit(cWorldTriggerManager* self, int id);
extern "C" int cWorldTriggerManager_SyncToAnimationTrigger(cWorldTriggerManager* self, unsigned int i);
extern "C" int cWorldTriggerManager_ObjectRefFirstFrame(cWorldTriggerManager* self, void* src, cWorldTriggerInstance* state);
extern unsigned char D_004A35A8[];

struct sSnd4C38Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd4C38E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd4C38Sub {
    void* rng;               // 0x00
    struct sSnd4C38Ch def;          // 0x04
    volatile int cur;        // 0x1C (volatile: the prototype's out-of-line OPTS_Vol reloads it at every use)
    struct sSnd4C38Ch* ch;          // 0x20
    void* tgt[4];            // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    struct sSnd4C38E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd4C38Mgr {
    char pad0[0x1D8];
    struct sSnd4C38Sub sub;         // 0x1D8
};
struct sSnd4C38Sys {
    struct sSnd4C38Mgr* mgr;
    int* a4;                 // 0x4
    int* a8;                 // 0x8
    char padC[0x118 - 0xC];
    struct sSnd4C38Mgr** p118;      // 0x118
};
struct sWt4C38 {
    int f0, f4, f8, fC;
    float f10;
    int f14;
    int pad18[2];
    char f20[0x10];
    int f30;
    int pad34;
    int f38;
    int f3C;
    char f40[0x20];
    int f60;
    int f64;
};

#define SND_4C38_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_4C38_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline void optsVol_4C38(struct sSnd4C38Mgr* m, int v)
{
    struct sSnd4C38Sub* o = &m->sub;
    o->vals[m->sub.cur] = v;
    o->ch[m->sub.cur].vol = SND_4C38_MAX(0, SND_4C38_MIN(o->vals[o->cur], 0x7F));
}

static inline void sfxBegin_4C38(struct sSnd4C38Sys* self, int a, int b)
{
    struct sSnd4C38Mgr* m = self->mgr;
    struct sSnd4C38Sub* s = &m->sub;
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

// vol/target/handle set-up shared by the play paths (SFX_Play2DVol tail)
static inline void voiceTail_4C38(struct sWt4C38* t)
{
    optsVol_4C38(*((struct sSnd4C38Sys*)cAudioInterface_bxAudio())->p118, (int)(t->f10 * 127.0f));
    struct sSnd4C38Mgr** pp = ((struct sSnd4C38Sys*)cAudioInterface_bxAudio())->p118;
    int r = cSSXAudio_GetCurrAppliedVolumePtr(cAudioInterface_bxAudio(), 5, 0);
    (*pp)->sub.p3C[(*pp)->sub.cur] = r;
    if (t->f14 != 0) {
        struct sSnd4C38Mgr** p1 = ((struct sSnd4C38Sys*)cAudioInterface_bxAudio())->p118;
        (*p1)->sub.tgt[(*p1)->sub.cur] = t->f20;
        struct sSnd4C38Mgr** p2 = ((struct sSnd4C38Sys*)cAudioInterface_bxAudio())->p118;
        (*p2)->sub.p74[(*p2)->sub.cur] = 1;
        struct sSnd4C38Mgr** p3 = ((struct sSnd4C38Sys*)cAudioInterface_bxAudio())->p118;
        (*p3)->sub.p7C[(*p3)->sub.cur] = 0;
    }
    struct sSnd4C38Mgr** p4 = ((struct sSnd4C38Sys*)cAudioInterface_bxAudio())->p118;
    (*p4)->sub.p64[(*p4)->sub.cur] = t->f3C;
}

/**
 * Per-frame update of a touched world trigger instance: starts its sound once, then keeps its volume current.
 *
 * Without a sound (+0x30 == -1): records whether it is a crowd trigger (cWorldTriggerInstance_CrowdTrigger, +0x38); a non-crowd trigger
 * whose object volume is already playing elsewhere (cWorldTriggerManager_PlayingTriggerInstance) is reset (cWorldTriggerInstance_Reset). It resolves the bank and
 * patch (cWorldTriggerInstance_TriggerToPatch, kept in +0x60 / +0x64); a no-audio trigger gets the handle -9999. A swap trigger (bank 3) needs
 * the focus rider's viewport, no other swap trigger playing (cWorldTriggerManager_SwapTriggerPlaying) and its swap string (cWorldTriggerInstance_SwapTriggerToString); its
 * string goes to func_0029B818 and it plays only when that returns 1.
 *
 * The sound plays only when the trigger is not an after-collision trigger (cWorldTriggerManager_AfterCollisionTrigger) or its object was hit
 * (cWorldTriggerManager_ObjectHit); a sync-to-animation trigger (cWorldTriggerManager_SyncToAnimationTrigger) also waits for its object's first frame (cWorldTriggerManager_ObjectRefFirstFrame).
 * Crowd triggers go to the crowd system (func_002A7468, func_002A7678); others play through cBankSys_SFX_Begin-style
 * options with volume +0x10 x 127, volume control 5, the position (+0x20) when positional and the viewport, ending in
 * cBankSys_SFX_End.
 *
 * With a sound: a sync-to-animation trigger restarts on each new first frame (stopping the old handle with a 0.1 s
 * fade); otherwise the volume is re-applied (func_002A7340 for crowd triggers, else the bank monitor instance's volume
 * through cBankMonitor_BANKMONITOR_GetHandle). Called by cWorldTriggerManager_Update for touched instances.
 *
 * @param tt  the world trigger instance
 */
extern "C" void cWorldTriggerInstance_Update(cWorldTriggerInstance* tt)
{
    struct sWt4C38* instance = (struct sWt4C38*)tt;
    int rec[3];
    int swapResult;
    if (instance->f30 == -1) {
        if (cWorldTriggerInstance_CrowdTrigger(instance, instance->fC) != 0)
            instance->f38 = 1;
        else
            instance->f38 = 0;
        if (instance->f38 == 0) {
            if (cWorldTriggerManager_PlayingTriggerInstance((sWorldTrigger70**)gWorldTriggerManager, instance->f4, instance->f8, ((sWorldTrigger70*)tt)) != 0) {
                cWorldTriggerInstance_Reset((cWorldTriggerInstance*)instance);
                return;
            }
        }
        {
            int ok = cWorldTriggerInstance_TriggerToPatch(instance, instance->fC, &rec[0], &rec[1], &rec[2], instance->f3C, 0);
            instance->f60 = rec[0];
            instance->f64 = rec[1];
            if (!ok) {
                cWorldTriggerInstance_Reset((cWorldTriggerInstance*)instance);
                return;
            }
        }
        if (rec[2] != 0) {
            instance->f30 = -9999;
            return;
        }
        swapResult = -1;
        if (rec[0] == 3) {
            if (*(int*)(cSSXAudio_GetFocusRider(cAudioInterface_bxAudio(), -1) + 0x870) != instance->f3C) {
                cWorldTriggerInstance_Reset((cWorldTriggerInstance*)instance);
                return;
            }
            swapResult = cWorldTriggerManager_SwapTriggerPlaying((sWorldTrigger70**)gWorldTriggerManager, instance->fC);
            if (swapResult > 0) {
                cWorldTriggerInstance_Reset((cWorldTriggerInstance*)instance);
                return;
            }
            if (func_004165A8(instance->f40, D_004A35A8) == 0) {
                if (cWorldTriggerInstance_SwapTriggerToString(instance, instance->fC, instance->f40) == 0) {
                    cWorldTriggerInstance_Reset((cWorldTriggerInstance*)instance);
                    return;
                }
            }
            swapResult = func_0029B818(cAudioInterface_bxAudio(), instance->f40, rec);
        }
        if (cWorldTriggerManager_AfterCollisionTrigger(gWorldTriggerManager, instance->fC) == 0 || cWorldTriggerManager_ObjectHit((cWorldTriggerManager*)((sTriggerIdCache*)gWorldTriggerManager), instance->f4) != 0) {
            if (rec[0] == 3 && swapResult != 1)
                return;
            if (cWorldTriggerManager_SyncToAnimationTrigger(gWorldTriggerManager, instance->fC) != 0) {
                if (cWorldTriggerManager_ObjectRefFirstFrame(gWorldTriggerManager, (void*)instance->f4, (cWorldTriggerInstance*)(((sWorldTrigger70*)tt))) == 0) {
                    instance->f30 = -9999;
                    return;
                }
            }
            if (instance->f38 != 0) {
                func_002A7468(cAudioInterface_bxAudio());
                voiceTail_4C38(instance);
                instance->f30 = func_002A7678(cAudioInterface_bxAudio(), rec[1]);
                return;
            }
            sfxBegin_4C38((struct sSnd4C38Sys*)cAudioInterface_bxAudio(), rec[0], rec[1]);
            voiceTail_4C38(instance);
            instance->f30 = cBankSys_SFX_End(cAudioInterface_bxAudio());
            return;
        }
        cWorldTriggerInstance_Reset((cWorldTriggerInstance*)instance);
        return;
    }
    if (cWorldTriggerManager_SyncToAnimationTrigger(gWorldTriggerManager, instance->fC) != 0 && cWorldTriggerManager_ObjectRefFirstFrame(gWorldTriggerManager, (void*)instance->f4, (cWorldTriggerInstance*)(((sWorldTrigger70*)tt))) != 0) {
        if (instance->f30 != -9999)
            cBankMonitor_BANKMONITOR_StopHandle(&(*((struct sSnd4C38Sys*)cAudioInterface_bxAudio())->p118)->sub, instance->f30, 1, 0.1f);
        sfxBegin_4C38((struct sSnd4C38Sys*)cAudioInterface_bxAudio(), instance->f60, instance->f64);
        voiceTail_4C38(instance);
        instance->f30 = cBankSys_SFX_End(cAudioInterface_bxAudio());
        return;
    }
    if (instance->f38 != 0) {
        if (instance->f30 == -9999)
            return;
        func_002A7340(cAudioInterface_bxAudio(), instance->f30, (int)(instance->f10 * 127.0f));
        return;
    }
    if (instance->f30 == -9999)
        return;
    {
        char* bankInstance = (char*)cBankMonitor_BANKMONITOR_GetHandle(&(*((struct sSnd4C38Sys*)cAudioInterface_bxAudio())->p118)->sub, instance->f30, 1);
        if (bankInstance == 0)
            return;
        *(int*)(bankInstance + 100) = (int)(instance->f10 * 127.0f);
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance_Stop);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_002A72D8(void* snd, int id, float t);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* self, int idx, int a2, float v);
extern unsigned char D_004A35A8[];
struct sWorldTrigger70;

/**
 * Stops a world audio trigger instance's sound and resets the instance.
 *
 * If the handle at +0x30 (On Tour m_iAudioHandle) is live (not -9999) it is stopped with a 0.25 s fade: through the
 * crowd system (func_002A72D8 on gSSXAudio, cAudioInterface_bxAudio) when the crowd flag at +0x38 is set, else through
 * cBankMonitor_BANKMONITOR_StopHandle on the bank monitor. Then the handle becomes -1, the swap name at +0x40 is
 * emptied (On Tour strcpy(name, "")), +0x68 (On Tour m_iLastAnimFrame) is set to -1.0f and +0x60, +0x64 and +0x6C are
 * zeroed.
 *
 * @param instance  the trigger instance (this)
 */
extern "C" void cWorldTriggerInstance_Stop(cWorldTriggerInstance* instance)
{
    if (instance->m_bCrowdTrigger != 0) {
        if (instance->m_iAudioHandle != -9999) {
            func_002A72D8(cAudioInterface_bxAudio(), instance->m_iAudioHandle, 0.25f);
        }
    } else if (instance->m_iAudioHandle != -9999) {
        cBankMonitor_BANKMONITOR_StopHandle(**(char***)&cAudioInterface_bxAudio()->MusicSys + 0x1D8, instance->m_iAudioHandle, 1, 0.25f);
    }
    instance->m_iAudioHandle = -1;
    instance->m_iSwapString[0] = D_004A35A8[0];
    instance->m_iLastAnimFrame = -1.0f;
    instance->m_iBankReference = 0;
    instance->m_iPatchReference = 0;
    instance->m_bLastAnimFrameDecrease = 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance_TriggerToPatch);
#ifdef SKIP_ASM
extern "C" int cWorldTriggerManager_TriggerToPatch(cWorldTriggerManager* self, int idx, int* type, int* a, int* b, int c, int d);

/**
 * Looks up a world trigger's bank and patch through the manager (cWorldTriggerManager_TriggerToPatch).
 *
 * The instance itself is not used; gWorldTriggerManager answers.
 *
 * @param trigger  the trigger number
 * @param bank  out: the sound bank
 * @param patch  out: the patch
 * @param noAudio  out: 1 when the trigger plays no bank sound
 * @param viewPort  the viewport
 * @param check  the lookup kind (0 general, 1 crowd, 2 swap)
 * @return cWorldTriggerManager_TriggerToPatch's result
 */
extern "C" int cWorldTriggerInstance_TriggerToPatch(void* self, int trigger, int* bank, int* patch, int* noAudio, int viewPort, int check)
{
    return cWorldTriggerManager_TriggerToPatch(gWorldTriggerManager, trigger, bank, patch, noAudio, viewPort, check);
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance_CrowdTrigger);
#ifdef SKIP_ASM
extern "C" int cWorldTriggerManager_TriggerToPatch(cWorldTriggerManager* self, int idx, int* type, int* a, int* b, int c, int d);

/**
 * Returns whether a world trigger plays from the crowd bank (bank 5 from a check-1 TriggerToPatch lookup).
 *
 * Likely bug: for a trigger type TriggerToPatch does not resolve with a nonzero check, bank is left uninitialised and
 * compared anyway.
 *
 * @param trigger  the trigger number
 * @return 1 for a crowd trigger, else 0
 */
extern "C" int cWorldTriggerInstance_CrowdTrigger(void* self, int trigger)
{
    int bank;
    int patch;
    int noAudio;
    cWorldTriggerManager_TriggerToPatch(gWorldTriggerManager, trigger, &bank, &patch, &noAudio, 0, 1);
    return bank == 5;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance_SwapTriggerToString);
#ifdef SKIP_ASM
extern "C" int cWorldTriggerManager_SwapTriggerToString(cWorldTriggerManager* self, int idx, char* dst);

/**
 * Copies a swap trigger's string into dst through the manager (cWorldTriggerManager_SwapTriggerToString on gWorldTriggerManager).
 *
 * @param idx  the trigger number
 * @param dst  receives the string
 * @return cWorldTriggerManager_SwapTriggerToString's result (1 when copied)
 */
extern "C" int cWorldTriggerInstance_SwapTriggerToString(void* self, int idx, char* dst)
{
    return cWorldTriggerManager_SwapTriggerToString(gWorldTriggerManager, idx, dst);
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance_Reset);
#ifdef SKIP_ASM
extern unsigned char D_004A35A8[];

/**
 * Resets a world trigger instance to unused: no sound (-1), untouched, empty swap string, last frame -1.0.
 *
 * Clears +0x0, +0x34, the bank and patch references (+0x60, +0x64) and +0x6C.
 */
extern "C" void cWorldTriggerInstance_Reset(cWorldTriggerInstance* self)
{
    self->m_bInUse = 0;
    self->m_iAudioHandle = -1;
    self->m_bTouch = 0;
    self->m_iSwapString[0] = D_004A35A8[0];
    self->m_iLastAnimFrame = -1.0f;
    self->m_iBankReference = 0;
    self->m_iPatchReference = 0;
    self->m_bLastAnimFrameDecrease = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_cWorldTriggerManager);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cWorldTriggerInstance_cWorldTriggerInstance(cWorldTriggerInstance* self);
extern "C" void cWorldTriggerManager_LoadTriggerInfo(void* self);
extern const char str_m_aWorldTriggerInstances[];


/**
 * Constructs the world trigger manager: resets its state, builds 40 trigger instances and loads the trigger
 * info.
 *
 * The instances are a new[] array ("m_aWorldTriggerInstances", 0x70 bytes each).
 *
 * @return @p self
 */
extern "C" void* cWorldTriggerManager_cWorldTriggerManager(cWorldTriggerManager* self)
{
    self->mNumHitObjects = 0;
    self->mLastHitObjectResult = 0;
    self->mLastHitObject = -1;
    self->mLastHitObjectChecked = -1;
    self->mMonitorTriggers = 1;
    int* mem = (int*)operator_new_tag(4496, str_m_aWorldTriggerInstances, 0, 0);
    mem[0] = 40;
    cWorldTriggerInstance* arr = (cWorldTriggerInstance*)((char*)mem + 0x10);
    cWorldTriggerInstance* trigger = arr;
    for (int i = 39; i != -1; i--, trigger++) {
        cWorldTriggerInstance_cWorldTriggerInstance((cWorldTriggerInstance*)trigger);
    }
    *(cWorldTriggerInstance**)self = arr;
    self->m_pcTriggerInfoFile = 0;
    cWorldTriggerManager_LoadTriggerInfo(self);
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager__cWorldTriggerManager);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);

/**
 * Destroys the world trigger manager: destroys its trigger instances, frees the array, then frees the object
 * if asked.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cWorldTriggerManager__cWorldTriggerManager(char** self, int flags)
{
    char* base = *self;
    if (base != 0) {
        char* entry = base + *(int*)(base - 0x10) * 0x70;
        while (*self != entry) {
            entry -= 0x70;
            cWorldTriggerInstance__cWorldTriggerInstance((int*)entry, 0);
        }
        cMemMan_free(*self - 0x10);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_LoadTriggerInfo);
#ifdef SKIP_ASM
extern "C" int BXFILE_exists(const char* name);
extern "C" char* FILE_loadpack(const char* name, int a1);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char* str_p5H;
extern char str_m_aTriggerInfoInstances[];

struct sTrigInfoEnt { int type; int* data; };

// PORT: the unit declares LoadTriggerInfo as void (the ctor ignores it); the body returns 0/1. Bound by asm label.
int cWorldTriggerManager_LoadTriggerInfo_impl(void* self) __asm__("cWorldTriggerManager_LoadTriggerInfo");

/**
 * Loads the sound trigger definitions from data/config/watrig.adl and indexes them by entry.
 *
 * Each entry's type decides its size (types 1-4 are 6, 14, 12 and 4 words); the index
 * ("m_aTriggerInfoInstances")
 * holds type and data pointer for entries 2 onwards. A missing file leaves the manager with no trigger info.
 *
 * @return 0 when the file's first byte is nonzero (rejected), otherwise 1
 */
int cWorldTriggerManager_LoadTriggerInfo_impl(void* vself)
{
    cWorldTriggerManager* self = (cWorldTriggerManager*)vself;
    if (BXFILE_exists(str_p5H) != 0) {
        char* data = FILE_loadpack(str_p5H, 0);
        self->m_pcTriggerInfoFile = data;
        if (*data != 0) {
            return 0;
        }
        self->m_iNumTriggers = *(int*)(data + 8) + 2;
        self->m_aTriggerInfo = (tTriggerTypePointer*)((sTrigInfoEnt*)operator_new_tag(self->m_iNumTriggers * 8, str_m_aTriggerInfoInstances, 0, 0));
        int* cursor = (int*)(data + 0xC);
        for (int i = 2; i < self->m_iNumTriggers; i++) {
            ((sTrigInfoEnt*)self->m_aTriggerInfo)[i].type = *cursor;
            ((sTrigInfoEnt*)self->m_aTriggerInfo)[i].data = cursor;
            int trigType = *cursor;
            if (trigType == 1) {
                cursor += 6;
            } else if (trigType == 2) {
                cursor += 14;
            } else if (trigType == 3) {
                cursor += 12;
            } else if (trigType == 4) {
                cursor += 4;
            }
        }
    } else {
        self->m_pcTriggerInfoFile = 0;
        self->m_iNumTriggers = 0;
        self->m_aTriggerInfo = 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_UnloadTriggerInfo);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv(int);
extern "C" void MEM_free(int);

/**
 * Frees the loaded trigger info: MEM_free on the file (+0x338), cMemMan_free on the table (+0x33C), count 0.
 *
 * Does nothing when no file is loaded.
 *
 * @param manager  the cWorldTriggerManager
 */
extern "C" void cWorldTriggerManager_UnloadTriggerInfo(void *manager) {
    int temp_4;
    int temp_4_2;

    temp_4 = (*(int *)((char*)(manager) + (0x338)));
    if (temp_4 != 0) {
        MEM_free(temp_4);
        temp_4_2 = (*(int *)((char*)(manager) + (0x33C)));
        (*(int *)((char*)(manager) + (0x338))) = 0;
        if (temp_4_2 != 0) {
            cMemMan_free__FPv(temp_4_2);
        }
        (*(int *)((char*)(manager) + (0x340))) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_GetFreeInstance);
#ifdef SKIP_ASM
struct sWorldTrigger70 {
    int active;
    char pad[0x30];
    int unk34;
    char pad2[0x38];
};

/**
 * Returns the first unused world trigger instance of the 40, or 0 when all are in use.
 *
 * @return the free instance, or 0
 */
extern "C" sWorldTrigger70* cWorldTriggerManager_GetFreeInstance(sWorldTrigger70** self)
{
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active == 0) {
            return &(*self)[i];
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_GetTriggerInstance);
#ifdef SKIP_ASM
extern "C" int cWorldTriggerInstance_SameTriggerInstance(cWorldTriggerInstance* self, int a1, int a2, int a3);

/**
 * Returns the in-use world trigger instance for an object, volume and viewport (cWorldTriggerInstance_SameTriggerInstance), or 0.
 *
 * @param objectRef  the object reference
 * @param volNum  the audio volume number
 * @param viewPort  the viewport
 * @return the instance, or 0
 */
extern "C" sWorldTrigger70* cWorldTriggerManager_GetTriggerInstance(sWorldTrigger70** self, int objectRef, int volNum, int viewPort)
{
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active != 0 && cWorldTriggerInstance_SameTriggerInstance((cWorldTriggerInstance*)(&(*self)[i]), objectRef, volNum, viewPort)) {
            return &(*self)[i];
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_HandleTrigger);
#ifdef SKIP_ASM
struct sWtVec3 { float x, y, z; };
struct sWtVec4 {
    float x, y, z, w;
    sWtVec4() {}
    sWtVec4(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

/**
 * Records a world audio trigger hit for this frame: finds or claims its instance, fills it and touches it.
 *
 * Only while triggers are monitored (+0x334, set by cWorldTriggerManager_MonitorTriggers). An existing instance (cWorldTriggerManager_GetTriggerInstance) gets the new
 * volume; otherwise a free one (cWorldTriggerManager_GetFreeInstance) is marked in use with the object, volume number, viewport, trigger,
 * volume, positional flag and position (w 1.0). Note: the free instance is not checked for 0. Either way +0x34
 * (touched) is set so cWorldTriggerManager_Update keeps it playing. Called by cWorldSound_TriggerGridAudioNode.
 *
 * @param objectRef  the object reference
 * @param volNum  the audio volume number in the object
 * @param viewPort  the viewport
 * @param audioTrigger  the trigger number
 * @param positional  nonzero to play at pos
 * @param volume  the volume (0..1)
 * @param pos  the sound position
 */
extern "C" void cWorldTriggerManager_HandleTrigger(sWorldTrigger70** self, int objectRef, int volNum, int viewPort, int audioTrigger, int positional, float volume, const sWtVec3& pos)
{
    sWtVec3 localPos = pos;
    if (*(int*)((char*)self + 0x334) != 0) {
        char* triggerInst = (char*)cWorldTriggerManager_GetTriggerInstance(self, objectRef, volNum, viewPort);
        if (triggerInst == 0) {
            sWtVec4 pos4;
            pos4 = sWtVec4(localPos.x, localPos.y, localPos.z, 1.0f);
            triggerInst = (char*)cWorldTriggerManager_GetFreeInstance(self);
            *(int*)(triggerInst + 0x0) = 1;
            *(int*)(triggerInst + 0x4) = objectRef;
            *(int*)(triggerInst + 0x8) = volNum;
            *(int*)(triggerInst + 0x3C) = viewPort;
            *(int*)(triggerInst + 0xC) = audioTrigger;
            *(float*)(triggerInst + 0x10) = volume;
            *(int*)(triggerInst + 0x14) = positional;
            *(sWtVec4*)(triggerInst + 0x20) = pos4;
        } else {
            *(float*)(triggerInst + 0x10) = volume;
        }
        *(int*)(triggerInst + 0x34) = 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_Update);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int func_004165A8(const void* a, const void* b);
// PORT: cSSXAudio_SwapBankStopped__FPv is an empty stub whose caller passes (mgr, name).
void cSSXAudio_SwapBankStopped_2(void* mgr, void* name) __asm__("cSSXAudio_SwapBankStopped__FPv");
extern "C" void cWorldTriggerInstance_Update(cWorldTriggerInstance* t);
extern "C" void cWorldTriggerInstance_Stop(cWorldTriggerInstance* t);
extern unsigned char D_004A35A8[];

/**
 * Per-frame update of the world triggers: stops untouched instances, updates touched ones, clears the touches.
 *
 * Skipped while the sound effects are paused (mSFXPaused). For each in-use instance: when untouched it releases a swap
 * string (cSSXAudio_SwapBankStopped), marks it unused and stops it (cWorldTriggerInstance_Stop); when touched it runs
 * cWorldTriggerInstance_Update. Called every frame from the game (func_0022C830).
 */
extern "C" void cWorldTriggerManager_Update(sWorldTrigger70** self)
{
    if (cAudioInterface_bxAudio()->mSFXPaused != 0) return;
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active == 1) {
            if ((*self)[i].unk34 == 0) {
                if (func_004165A8((char*)&(*self)[i] + 0x40, D_004A35A8)) {
                    // PORT: pointer arithmetic through int
                    cSSXAudio_SwapBankStopped_2(cAudioInterface_bxAudio(), (char*)(i * 0x70 + *(int*)self) + 0x40);
                }
                (*self)[i].active = 0;
                cWorldTriggerInstance_Stop((cWorldTriggerInstance*)(&(*self)[i]));
            } else {
                cWorldTriggerInstance_Update((cWorldTriggerInstance*)(&(*self)[i]));
            }
            (*self)[i].unk34 = 0;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_StopAllTriggers);
#ifdef SKIP_ASM
extern "C" void cWorldTriggerInstance_Stop(cWorldTriggerInstance* t);

/**
 * Stops every active world audio trigger instance.
 *
 * Walks the 40 instances in the array at this+0; each whose active flag is 1 is marked inactive and stopped with
 * cWorldTriggerInstance_Stop.
 */
extern "C" void cWorldTriggerManager_StopAllTriggers(cWorldTriggerManager* self)
{
    for (int i = 0; i < 40; i++) {
        if ((*(sWorldTrigger70**)self)[i].active == 1) {
            (*(sWorldTrigger70**)self)[i].active = 0;
            cWorldTriggerInstance_Stop((cWorldTriggerInstance*)(&(*(sWorldTrigger70**)self)[i]));
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_StopTrigger);
#ifdef SKIP_ASM
extern "C" void cWorldTriggerInstance_Stop(cWorldTriggerInstance* t);

/**
 * Stops every in-use world trigger instance playing trigger id (marks it unused, cWorldTriggerInstance_Stop).
 *
 * @param id  the trigger number
 */
extern "C" void cWorldTriggerManager_StopTrigger(sWorldTrigger70** self, int id)
{
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active == 1 && *(int*)((char*)&(*self)[i] + 0xC) == id) {
            (*self)[i].active = 0;
            cWorldTriggerInstance_Stop((cWorldTriggerInstance*)(&(*self)[i]));
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_HandleCollision);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int cWorldTriggerManager_TriggerToPatch(cWorldTriggerManager* self, int idx, int* type, int* a, int* b, int c, int d);
extern "C" int cWorldTriggerManager_HasAfterCollisionTrigger(cWorldTriggerManager* self, void* obj);
struct sTriggerIdCache;
extern "C" void cWorldTriggerManager_AddHitObject(cWorldTriggerManager* self, int id);
extern "C" int cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* rider);
extern "C" int cBankSys_SFX_End(void* self);
extern "C" void* func_00416210(void* dst, int c, int n);

struct sWtEnt5F60 {
    signed char b0;
    char pad1[3];
    int f4[5];
};

struct sWtPair5F60 {
    int a;
    int b;
};

struct sWtV16_5F60 {
    int f0;
    int f4;
    int f8;
    int fC;
};

struct sWtSub5F60 {
    int f0;
    sWtEnt5F60 cfg;         // 0x04
    volatile int cur;       // 0x1C (re-read after every store and twice back-to-back: volatile)
    sWtEnt5F60* ents;       // 0x20
    int arr24[4];           // 0x24
    int* p34;               // 0x34
    int* p38;               // 0x38
    int* p3C;               // 0x3C
    sWtPair5F60 arr40[4];   // 0x40
    int* p60;               // 0x60
    int* p64;               // 0x64
    int* p68;               // 0x68
    int* p6C;               // 0x6C
    sWtV16_5F60* p70;       // 0x70
    int* p74;               // 0x74
    int* p78;               // 0x78
    int* p7C;               // 0x7C
    int* p80;               // 0x80
    int* p84;               // 0x84
};

struct sWtTrack5F60 {
    char pad0[0x1D8];
    sWtSub5F60 sub;         // 0x1D8
};

struct sWtSnd5F60 {
    sWtTrack5F60* track;    // 0x0
    int* types;             // 0x4
    int* args;              // 0x8
    char pad[0x10C];
    sWtTrack5F60** cur;     // 0x118
};

#define MIN_5F60(a, b) ((a) < (b) ? (a) : (b))
#define MAX_5F60(a, b) ((a) < (b) ? (b) : (a))

/**
 * Plays the sound of a world trigger hit by a rider (collision): returns its handle, or -1.
 *
 * A trigger whose info type is 3 (swap) returns -1. When evt differs from the last hit object (+0x328) and has an
 * after-collision trigger (cWorldTriggerManager_HasAfterCollisionTrigger) it is added to the hit objects (cWorldTriggerManager_AddHitObject); evt becomes the last hit
 * object. Resolves the bank and patch (cWorldTriggerManager_TriggerToPatch, viewport 1); with audio it plays them like
 * cBankSys_SFX_Begin / SFX_End with volume vol, volume control 7 and, for a rider, the rider's position (+0x110) and
 * cSSXAudio_GetClosestCamera1. Called by cSSXAudio_CollisionSound.
 *
 * @param evt  the hit object (instance record), or 0
 * @param id  the trigger number
 * @param rider  the rider, or 0
 * @param vol  the volume (0..127)
 * @return the sound handle, or -1
 */
extern "C" int cWorldTriggerManager_HandleCollision(cWorldTriggerManager* self, void* evt, int id, cRider* rider, int vol)
{
    if (self->m_aTriggerInfo[id].m_TriggerType == 3)
        return -1;
    if (evt != 0)
    {
        if (evt != (void*)self->mLastHitObject)
        {
            if (cWorldTriggerManager_HasAfterCollisionTrigger(self, evt))
                cWorldTriggerManager_AddHitObject(self, (int)evt);
        }
        self->mLastHitObject = (int)evt;
    }
    int type, patch, noAudio;
    if (cWorldTriggerManager_TriggerToPatch(self, id, &type, &patch, &noAudio, 1, 0) != 0 && noAudio == 0)
    {
        sWtSnd5F60* snd = (sWtSnd5F60*)cAudioInterface_bxAudio();
        int tp = type;
        int ar = patch;
        sWtTrack5F60* bankManager = snd->track;
        sWtSub5F60* playOpts = &bankManager->sub;
        bankManager->sub.cur++;
        playOpts->ents[bankManager->sub.cur] = playOpts->cfg;
        bankManager->sub.arr24[bankManager->sub.cur] = 0;
        playOpts->p34[bankManager->sub.cur] = 0;
        playOpts->p38[bankManager->sub.cur] = playOpts->ents[bankManager->sub.cur].b0;
        playOpts->p3C[bankManager->sub.cur] = 0;
        func_00416210((char*)playOpts + bankManager->sub.cur * 8 + 0x40, 0, 8);
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
        snd->types[snd->track->sub.cur] = tp;
        snd->args[snd->track->sub.cur] = ar;
        {
            sWtTrack5F60* volManager = *((sWtSnd5F60*)cAudioInterface_bxAudio())->cur;
            sWtSub5F60* s2 = &volManager->sub;
            s2->p38[volManager->sub.cur] = vol;
            s2->ents[volManager->sub.cur].b0 = MAX_5F60(MIN_5F60(s2->p38[s2->cur], 0x7F), 0);
        }
        {
            sWtTrack5F60** pt = ((sWtSnd5F60*)cAudioInterface_bxAudio())->cur;
            int h = cSSXAudio_GetCurrAppliedVolumePtr(cAudioInterface_bxAudio(), 7, 0);
            (*pt)->sub.p3C[(*pt)->sub.cur] = h;
        }
        if (((char*)rider) != 0)
        {
            sWtTrack5F60* riderManager = *((sWtSnd5F60*)cAudioInterface_bxAudio())->cur;
            riderManager->sub.arr24[riderManager->sub.cur] = (int)(&rider->mPosition);
            sWtTrack5F60** pt = ((sWtSnd5F60*)cAudioInterface_bxAudio())->cur;
            int h = cSSXAudio_GetClosestCamera1(cAudioInterface_bxAudio(), ((char*)rider));
            (*pt)->sub.p64[(*pt)->sub.cur] = h;
            sWtTrack5F60* flagManager = *((sWtSnd5F60*)cAudioInterface_bxAudio())->cur;
            flagManager->sub.p34[flagManager->sub.cur] = 1;
        }
        return cBankSys_SFX_End(cAudioInterface_bxAudio());
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_TriggerToPatch);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int cSoundSys_SndRand(int a);
extern "C" int cWorldTriggerManager_AdvancedTriggerToPatch(cWorldTriggerManager* self, int type, int* a, int* b, int* c, int arg);

struct sWTMObj {
    char pad0[0x10];
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
    int f28;
    int f2C;
    int pad30;
    int f34;
};

struct sWTMEntry {
    int type;
    sWTMObj* obj;
};

/**
 * Looks up the sound bank and patch a world audio trigger plays.
 *
 * Clears *noAudio. A trigger number at or past the count (+0x340) gives bank 0, patch 0 and returns 0. Otherwise the
 * entry in the table at +0x33C (type, then type data; On Tour tTriggerTypePointer) decides: type 1 (On Tour Banked)
 * gives the bank (+0x10) and patch (+0x14) of its data; type 2 (Random) gives the bank (+0x10) and one of four patches
 * (+0x14 .. +0x20) picked with BXrand (cSoundSys_SndRand) by the weights at +0x24, +0x28 and +0x2C out of the total at
 * +0x34; type 3 (Swap) gives bank 3 and patch 0. Other types go to cWorldTriggerManager_AdvancedTriggerToPatch when
 * @p check is 0; with another @p check they return 1 and leave *bank and *patch unwritten.
 *
 * @param trigger   the trigger number
 * @param bank      out: the sound bank
 * @param patch     out: the patch in the bank
 * @param noAudio   out: 1 when the trigger plays no bank sound (set only by the advanced path)
 * @param viewPort  the viewport the trigger fired in
 * @param check     which lookup is wanted (On Tour eTriggerToPatchCheck: 0 general, 1 crowd, 2 swap)
 * @return 1 when the trigger was resolved, 0 for a trigger number out of range (or the advanced path's result)
 */
extern "C" int cWorldTriggerManager_TriggerToPatch(cWorldTriggerManager* self, int trigger, int* bank, int* patch, int* noAudio, int viewPort, int check)
{
    *noAudio = 0;
    if (trigger >= self->m_iNumTriggers) {
        *bank = 0;
        *patch = 0;
        return 0;
    }
    int result = 1;
    int triggerType = ((sWTMEntry*)self->m_aTriggerInfo)[trigger].type;
    if (triggerType == 3) {
        *bank = triggerType;
        *patch = 0;
    } else if (triggerType == 1) {
        *bank = ((sWTMEntry*)self->m_aTriggerInfo)[trigger].obj->f10;
        *patch = ((sWTMEntry*)self->m_aTriggerInfo)[trigger].obj->f14;
    } else if (triggerType == 2) {
        *bank = ((sWTMEntry*)self->m_aTriggerInfo)[trigger].obj->f10;
        void* audio = cAudioInterface_bxAudio();
        int rnd = cSoundSys_SndRand(*(int*)(*(char**)*(void**)((char*)audio + 0x118) + 0x1D8));
        sWTMObj* randomData = ((sWTMEntry*)self->m_aTriggerInfo)[trigger].obj;
        int pick = (unsigned)((rnd & 0x7FFF) * randomData->f34) / 0x7FFF;
        if (pick < randomData->f24) *patch = randomData->f14;
        else if (pick < randomData->f28 + randomData->f24) *patch = randomData->f18;
        else if (pick < randomData->f2C + randomData->f28 + randomData->f24) *patch = randomData->f1C;
        else *patch = randomData->f20;
    } else if (check == 0) {
        result = cWorldTriggerManager_AdvancedTriggerToPatch(self, trigger, bank, patch, noAudio, viewPort);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_AdvancedTriggerToPatch);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int func_002A34D0(void* mgr, int a1, int a2, int a3);

/**
 * Resolves an advanced world audio trigger: only trigger 77 is known, and it plays through the audio system itself.
 *
 * Clears *bank, *patch and *noAudio. For trigger 77 it sets *noAudio to 1 and returns func_002A34D0(gSSXAudio, no
 * rider, 1, @p viewPort); any other trigger returns 0.
 *
 * @param trigger   the trigger number
 * @param bank      out: the sound bank (always 0)
 * @param patch     out: the patch in the bank (always 0)
 * @param noAudio   out: 1 when the trigger plays no bank sound of its own (On Tour noaudio)
 * @param viewPort  the viewport the trigger fired in
 * @return 0, or func_002A34D0's result for trigger 77
 */
extern "C" int cWorldTriggerManager_AdvancedTriggerToPatch(cWorldTriggerManager* self, int trigger, int* bank, int* patch, int* noAudio, int viewPort)
{
    int found = 0;
    *bank = 0;
    *patch = 0;
    *noAudio = 0;
    if (trigger == 77) {
        *noAudio = 1;
        found = func_002A34D0(cAudioInterface_bxAudio(), 0, 1, viewPort);
    }
    return found;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_SwapTriggerToString);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" char* strcpy(char* dst, const char* src);
// NOTE: cWorldTriggerManager_AdvancedTriggerToString is a `return 0` stub; this caller leaves idx/out live in a1/a2 at the call,
// so the original most likely passed them through (the stub ignores them).
int cWorldTriggerManager_AdvancedTriggerToString_3(void* self, int idx, char* out) __asm__("cWorldTriggerManager_AdvancedTriggerToString__FPv");

struct sWorldTriggerInfo8 {
    int type;
    char* data;
};

/**
 * Copies a swap trigger's string: a type-3 trigger's text (+0x10 of its data); type 4 goes to cWorldTriggerManager_AdvancedTriggerToString.
 *
 * Returns 0 for a trigger number at or past the count (+0x340) and for other types.
 *
 * @param idx  the trigger number
 * @param out  receives the string
 * @return 1 when copied, else 0 (or cWorldTriggerManager_AdvancedTriggerToString's 0)
 */
extern "C" int cWorldTriggerManager_SwapTriggerToString(cWorldTriggerManager* self, int idx, char* out)
{
    if (idx < self->m_iNumTriggers) {
        sWorldTriggerInfo8* list = (sWorldTriggerInfo8*)self->m_aTriggerInfo;
        if (list[idx].type == 3) {
            strcpy(out, list[idx].data + 0x10);
            return 1;
        }
        if (list[idx].type == 4) {
            return cWorldTriggerManager_AdvancedTriggerToString_3(self, idx, out);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_AdvancedTriggerToString__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: the advanced-trigger string lookup is a stub.
 *
 * cWorldTriggerManager_SwapTriggerToString reaches it for type-4 triggers with the trigger and buffer still in the argument registers.
 *
 * @return 0
 */
int cWorldTriggerManager_AdvancedTriggerToString(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_SwapTriggerPlaying);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);
extern unsigned char D_004A35A8[];

/**
 * Counts the in-use instances that hold a swap string and play a trigger other than id.
 *
 * cWorldTriggerInstance_Update does not start a swap trigger while this is above 0.
 *
 * @param id  the trigger to leave out
 * @return the count
 */
extern "C" int cWorldTriggerManager_SwapTriggerPlaying(sWorldTrigger70** self, int id)
{
    int n = 0;
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active == 1 && func_004165A8((char*)&(*self)[i] + 0x40, D_004A35A8)) {
            if (*(int*)((char*)&(*self)[i] + 0xC) != id) n++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_PlayingTriggerInstance);
#ifdef SKIP_ASM
/**
 * Returns whether another in-use instance (not except) plays the same object and audio volume.
 *
 * @param objectRef  the object reference
 * @param volNum  the audio volume number
 * @param except  the instance to ignore
 * @return 1 when one exists, else 0
 */
extern "C" int cWorldTriggerManager_PlayingTriggerInstance(sWorldTrigger70** self, int objectRef, int volNum, sWorldTrigger70* except)
{
    for (int i = 0; i < 40; i++) {
        sWorldTrigger70* triggerInst = &(*self)[i];
        if (triggerInst->active == 1 && triggerInst != except
            && *(int*)((char*)triggerInst + 0x4) == objectRef
            && *(int*)((char*)triggerInst + 0x8) == volNum) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_HasAfterCollisionTrigger);
#ifdef SKIP_ASM
extern "C" cGame* cAudioInterface_getGame(void);
extern "C" void cWorldSound_GetAudioVolumeTriggers(cWorldSound* self, void* obj, int* o0, int* o1, int* o2, int* o3);
extern "C" int cWorldTriggerManager_AfterCollisionTrigger(cWorldTriggerManager* self, unsigned int i);

/**
 * Returns whether any of an object's four audio volumes uses an after-collision trigger (cWorldTriggerManager_AfterCollisionTrigger).
 *
 * The trigger numbers come from the game's cWorldSound (cWorldSound_GetAudioVolumeTriggers). Called by cWorldTriggerManager_HandleCollision.
 *
 * @param obj  the instance record
 * @return 1 or 0
 */
extern "C" int cWorldTriggerManager_HasAfterCollisionTrigger(cWorldTriggerManager* self, void* obj)
{
    int ids[4];
    cWorldSound_GetAudioVolumeTriggers((cWorldSound*)(cAudioInterface_getGame()->WorldSound), obj, &ids[0], &ids[1], &ids[2], &ids[3]);
    if (cWorldTriggerManager_AfterCollisionTrigger(self, ids[0])) {
        return 1;
    }
    if (cWorldTriggerManager_AfterCollisionTrigger(self, ids[1])) {
        return 1;
    }
    if (cWorldTriggerManager_AfterCollisionTrigger(self, ids[2])) {
        return 1;
    }
    return cWorldTriggerManager_AfterCollisionTrigger(self, ids[3]) != 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_AfterCollisionTrigger);
#ifdef SKIP_ASM
struct sTriggerRef {
    int id;
    void* obj;
};

/**
 * Returns whether trigger i is an after-collision trigger: its type data's +0x4 is 1 (never for i < 2).
 *
 * @param i  the trigger number
 * @return 1 or 0
 */
extern "C" int cWorldTriggerManager_AfterCollisionTrigger(cWorldTriggerManager* self, unsigned int i)
{
    if (i < 2) {
        return 0;
    }
    sTriggerRef* refs = (sTriggerRef*)self->m_aTriggerInfo;
    return *(int*)((char*)refs[i].obj + 0x4) == 1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_ObjectHit);
#ifdef SKIP_ASM
struct sTriggerIdCache {
    int unk0;
    int ids[200];       // 0x4
    int count;          // 0x324
    int unk328;
    int lastId;         // 0x32C
    int lastResult;     // 0x330
};

/**
 * Returns whether an object is in the hit-object list (+0x4, count +0x324), caching the last answer.
 *
 * The last asked object and result are kept in +0x32C / +0x330.
 *
 * @param id  the object
 * @return 1 when it was hit, else 0
 */
extern "C" int cWorldTriggerManager_ObjectHit(cWorldTriggerManager* self, int id)
{
    if (id == self->mLastHitObjectChecked) {
        return self->mLastHitObjectResult;
    }
    int i = self->mNumHitObjects - 1;
    self->mLastHitObjectChecked = id;
    for (; i >= 0; i--) {
        if (self->mHitObjects[i] == id) {
            self->mLastHitObjectResult = 1;
            return 1;
        }
    }
    self->mLastHitObjectResult = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_AddHitObject);
#ifdef SKIP_ASM
/**
 * Adds an object to the hit-object list (at most 200) when not already there and invalidates the cache.
 *
 * @param id  the object
 */
extern "C" void cWorldTriggerManager_AddHitObject(cWorldTriggerManager* self, int id)
{
    if (cWorldTriggerManager_ObjectHit(self, id) == 0) {
        if (self->mNumHitObjects < 200) {
            self->mHitObjects[self->mNumHitObjects] = id;
            self->mNumHitObjects++;
        }
        self->mLastHitObjectChecked = -1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_Reset);
#ifdef SKIP_ASM
extern "C" void cWorldTriggerManager_StopAllTriggers(cWorldTriggerManager* self);
void cWorldTriggerManager_ResetStates(void* self);

/**
 * Stops all world audio triggers and clears the manager's trigger state.
 *
 * Calls cWorldTriggerManager_StopAllTriggers then cWorldTriggerManager_ResetStates__FPv.
 */
extern "C" void cWorldTriggerManager_Reset(cWorldTriggerManager* self)
{
    cWorldTriggerManager_StopAllTriggers(self);
    cWorldTriggerManager_ResetStates(self);
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_ResetStates__FPv);
#ifdef SKIP_ASM
/**
 * Clears the world trigger manager's tracking state.
 *
 * Sets +0x324 and +0x330 to 0 and +0x328 and +0x32C to -1.
 */
void cWorldTriggerManager_ResetStates(void* vself)
{
    cWorldTriggerManager* self = (cWorldTriggerManager*)vself;
    self->mLastHitObjectResult = 0;
    self->mNumHitObjects = 0;
    self->mLastHitObject = -1;
    self->mLastHitObjectChecked = -1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_SwapTrigger);
#ifdef SKIP_ASM
extern "C" int cWorldTriggerManager_TriggerToPatch(cWorldTriggerManager* self, int idx, int* type, int* a, int* b, int c, int d);

/**
 * Returns whether a world trigger is a swap trigger (bank 3 from a check-2 TriggerToPatch lookup).
 *
 * As in cWorldTriggerInstance_CrowdTrigger, an unresolved advanced trigger leaves bank uninitialised. No caller found.
 *
 * @param trigger  the trigger number
 * @return 1 or 0
 */
extern "C" int cWorldTriggerManager_SwapTrigger(cWorldTriggerManager* self, int trigger)
{
    int bank;
    int patch;
    int noAudio;
    cWorldTriggerManager_TriggerToPatch(self, trigger, &bank, &patch, &noAudio, 0, 2);
    return bank == 3;
}
#endif
//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_MonitorTriggers);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_DelayMonitorTriggers(void* self);

/**
 * Turns world trigger monitoring on or off (+0x334), or with delay asks the audio system to turn it on later.
 *
 * With enable and delay both set it calls cSSXAudio_DelayMonitorTriggers, whose callback cSSXAudio_HandleDelayMonitorTriggers calls back with
 * (1, 0) after 250 ms.
 *
 * @param enable  nonzero to monitor triggers
 * @param delay  nonzero to enable after a delay
 */
extern "C" void cWorldTriggerManager_MonitorTriggers(cWorldTriggerManager* self, int enable, int delay)
{
    if (enable == 0 || delay == 0) {
        self->mMonitorTriggers = enable;
        return;
    }
    cSSXAudio_DelayMonitorTriggers(cAudioInterface_bxAudio());
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_SyncToAnimationTrigger);
#ifdef SKIP_ASM
/**
 * Returns whether trigger i syncs to its object's animation: its type data's +0x8 is 1 (never for i < 2).
 *
 * @param i  the trigger number
 * @return 1 or 0
 */
extern "C" int cWorldTriggerManager_SyncToAnimationTrigger(cWorldTriggerManager* self, unsigned int i)
{
    if (i < 2) {
        return 0;
    }
    sTriggerRef* refs = (sTriggerRef*)self->m_aTriggerInfo;
    return *(int*)((char*)refs[i].obj + 0x8) == 1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_ObjectRefFirstFrame);
#ifdef SKIP_ASM
struct sWtmVEntryF {
    short delta;
    short index;
    float (*fn)(void*);
};

/**
 * Returns whether an object's animation just restarted: the frame (virtual slot 30) went down and back up.
 *
 * Reads the object's animation through src+0xC, compares the frame with the instance's last frame (+0x68), returns 1
 * when the previous update saw a decrease (+0x6C) and this one does not, then stores both. Returns 0 when the object
 * has no animation.
 *
 * @param src  the object reference (instance record)
 * @param state  the world trigger instance holding the last frame
 * @return 1 on the first frame, else 0
 */
extern "C" int cWorldTriggerManager_ObjectRefFirstFrame(cWorldTriggerManager* self, void* src, cWorldTriggerInstance* state)
{
    void* obj = *(void**)((char*)src + 0xC);
    if (obj == 0) {
        return 0;
    }
    float prev = state->m_iLastAnimFrame;
    sWtmVEntryF* vt = *(sWtmVEntryF**)((char*)obj + 0xC);
    float cur = vt[30].fn(vthis(obj, vt[30]));
    int below = cur < prev;
    int triggered = state->m_bLastAnimFrameDecrease != 0 && !below;
    state->m_iLastAnimFrame = cur;
    state->m_bLastAnimFrameDecrease = below;
    return triggered;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_DuckAmbientAudioTrigger);
#ifdef SKIP_ASM

/**
 * Returns whether trigger idx ducks ambient audio: its type data's +0xC is 1 (never for idx < 2).
 *
 * No caller found.
 *
 * @param idx  the trigger number
 * @return 1 or 0
 */
extern "C" int cWorldTriggerManager_DuckAmbientAudioTrigger(cWorldTriggerManager* self, unsigned int idx)
{
    if (idx < 2U) {
        return 0;
    }
    return ((self->m_aTriggerInfo)[idx].m_pTriggerTypeStruct)->m_DuckAmbientAudio == 1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_TouchAllUsed);
#ifdef SKIP_ASM
/**
 * Marks every in-use world trigger instance touched (+0x34) so the next update keeps them all.
 *
 * Called by cSSXAudio_ResumeSFX.
 */
extern "C" void cWorldTriggerManager_TouchAllUsed(sWorldTrigger70** self)
{
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].active == 1) {
            (*self)[i].unk34 = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_cWorldSound);
#ifdef SKIP_ASM
struct sQuad6AA0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sQuad6AA0 t4vect_Zero;

struct sTriggerSlot120 {
    int unk0;
    char pad[0x10c];
    sQuad6AA0 q;        // 0x110
};

struct sTriggerSlots {
    sTriggerSlot120 slots[2];
};

/**
 * Constructs a cWorldSound: empties its two viewport overlap lists (count 0, last position zero).
 *
 * The game load allocates it (cMemMan_alloc(592, "cWorldSound")) and passes 0 as a second argument the C declaration
 * does not take.
 *
 * @return self
 */
extern "C" sTriggerSlots* cWorldSound_cWorldSound(cWorldSound* self)
{
    sTriggerSlot120* overlapList = ((sTriggerSlot120*)&self->m_OverlapLists[0]);
    for (int i = 1; i != -1; i--, overlapList++) {
        overlapList->unk0 = 0;
        overlapList->q = t4vect_Zero;
    }
    return ((sTriggerSlots*)self);
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound__cWorldSound);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys a cWorldSound: nothing to release; deletes the object when bit 0 of flags is set.
 *
 * The game unload calls it with 3.
 *
 * @param flags  g++ 2.95 destructor flags (bit 0: operator delete the object)
 */
extern "C" void cWorldSound__cWorldSound(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_InitWorldAudioFile);
#ifdef SKIP_ASM
extern "C" void func_002B6B50(void*);

/**
 * Registers a loaded world audio file: calls func_002B6B50 when a1 is set; returns 1.
 *
 * The world view calls it with the file data and its resource id when a world audio file loads.
 *
 * @param icePickData  the world audio file data, or 0
 * @return 1
 */
extern "C" int cWorldSound_InitWorldAudioFile(cWorldSound* self, int icePickData)
{
    if (icePickData != 0) {
        func_002B6B50(self);
    }
    return 1;
}
#endif

extern "C" void cWorldSound_DesolveAudioFile(void*);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_PurgeWorldAudioFile);
#ifdef SKIP_ASM
/**
 * Releases a world audio file before it is purged: calls cWorldSound_DesolveAudioFile when a1 is set.
 *
 * The world view calls it for type-13 resources (world audio files).
 *
 * @param icePickData  the world audio file data, or 0
 */
extern "C" void cWorldSound_PurgeWorldAudioFile(cWorldSound* self, int icePickData)
{
    if (icePickData != 0) {
        cWorldSound_DesolveAudioFile(self);
    }
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6B50);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_DesolveAudioFile);
#ifdef SKIP_ASM
struct sWtmEnt18 {
    char pad[0xC];
    int active;
    char pad2[0x8];
};
struct sWtmEntList {
    char pad[0xC];
    int count;
    sWtmEnt18 entries[1];
};
extern "C" void cWorldSound_RemoveInstanceFromGrid(cWorldSound* self, sWtmEnt18* e);

// PORT: the unit declares cWorldSound_DesolveAudioFile as void(void*); its callers pass a1 through.
int cWorldSound_DesolveAudioFile_impl(void* self, sWtmEntList* list) __asm__("cWorldSound_DesolveAudioFile");

/**
 * Removes every active audio instance of a world audio file from the audio grid (cWorldSound_RemoveInstanceFromGrid); returns 1.
 *
 * The C declaration has one parameter; the data pointer arrives as the second argument.
 *
 * @param list  the file data (count at +0xC, then 0x18-byte instances)
 * @return 1
 */
int cWorldSound_DesolveAudioFile_impl(void* self, sWtmEntList* list)
{
    int n = list->count;
    sWtmEnt18* entries = list->entries;
    for (int i = 0; i < n; i++) {
        sWtmEnt18* entry = &entries[i];
        if (entry->active != 0) {
            cWorldSound_RemoveInstanceFromGrid((cWorldSound*)self, entry);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_CreateAudioGrid);
#ifdef SKIP_ASM
struct sTriggerEntry18 {
    int unk0;
    int unk4;
    int unk8;
    int active;
    void* unk10;
    int unk14;
};

extern "C" void cWorldSound_AddInstanceToGrid(cWorldSound* self, sTriggerEntry18* e);

/**
 * Adds every active audio instance in a list to the audio grid (cWorldSound_AddInstanceToGrid); returns 1.
 *
 * @param icePickData  the world audio file data (not used)
 * @param entries  the 0x18-byte audio instances
 * @param n  their count
 * @return 1
 */
extern "C" int cWorldSound_CreateAudioGrid(cWorldSound* self, int icePickData, sTriggerEntry18* entries, int n)
{
    for (int i = 0; i < n; i++) {
        if (entries[i].active != 0) {
            cWorldSound_AddInstanceToGrid(self, &entries[i]);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_CalcInstAudioBoundingBox);
#ifdef SKIP_ASM
struct sWtmEnt18;

struct sVec3_2B6CF8 {
    float x, y, z;
    sVec3_2B6CF8() {}
    sVec3_2B6CF8(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

extern "C" void cWorldSound_CalcAudioVolumeBox(cWorldSound* self, int* shape, sVec3_2B6CF8* lo, sVec3_2B6CF8* hi);

/**
 * Computes an audio instance's world bounding box from its audio volumes (cWorldSound_CalcAudioVolumeBox) and its object position.
 *
 * Starts from +-10000, merges each volume record's box (records are 0x1C, 0x30, 0x30 or 0x18 bytes by type) and offsets
 * both corners by the object's position (+0x40).
 *
 * @param entry  the audio instance
 * @param lo  receives the minimum corner
 * @param hi  receives the maximum corner
 */
extern "C" void cWorldSound_CalcInstAudioBoundingBox(cWorldSound* self, sWtmEnt18* entry, sVec3_2B6CF8* lo, sVec3_2B6CF8* hi)
{
    *lo = sVec3_2B6CF8(10000.0f, 10000.0f, 10000.0f);
    *hi = sVec3_2B6CF8(-10000.0f, -10000.0f, -10000.0f);
    int* info = *(int**)((char*)entry + 0x10);
    char* base = (char*)(info + 2);
    char* cursor = base + info[0] * 4;
    for (int i = 0; i < info[1]; i++) {
        sVec3_2B6CF8 recMin;
        sVec3_2B6CF8 recMax;
        cWorldSound_CalcAudioVolumeBox(self, (int*)cursor, &recMin, &recMax);
        if (recMin.x < lo->x) lo->x = recMin.x;
        if (recMin.y < lo->y) lo->y = recMin.y;
        if (recMin.z < lo->z) lo->z = recMin.z;
        if (recMax.x > hi->x) hi->x = recMax.x;
        if (recMax.y > hi->y) hi->y = recMax.y;
        if (recMax.z > hi->z) hi->z = recMax.z;
        switch (*(int*)cursor) {
        case 0:
            cursor += 0x1C;
            break;
        case 1:
            cursor += 0x30;
            break;
        case 2:
            cursor += 0x30;
            break;
        case 3:
            cursor += 0x18;
            break;
        }
    }
    tInstanceRecord* obj = (tInstanceRecord*)(*(char**)((char*)entry + 0xC));
    lo->x += obj->m_fmTransform.m[3][0];
    lo->y += obj->m_fmTransform.m[3][1];
    lo->z += obj->m_fmTransform.m[3][2];
    hi->x += obj->m_fmTransform.m[3][0];
    hi->y += obj->m_fmTransform.m[3][1];
    hi->z += obj->m_fmTransform.m[3][2];
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_CalcAudioVolumeBox);
#ifdef SKIP_ASM
// PORT: sqrt.s (sqrtf without errno check)
static inline float wtSqrt_2B6F40(float v)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(v));
    return r;
}

static inline sVec3_2B6CF8 operator*(const sVec3_2B6CF8& v, float s)
{
    return sVec3_2B6CF8(v.x * s, v.y * s, v.z * s);
}

static inline sVec3_2B6CF8 operator+(const sVec3_2B6CF8& a, const sVec3_2B6CF8& b)
{
    return sVec3_2B6CF8(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline void operator-=(sVec3_2B6CF8& a, const sVec3_2B6CF8& b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
}

static inline void operator+=(sVec3_2B6CF8& a, const sVec3_2B6CF8& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
}

static inline sVec3_2B6CF8 Cross_2B6F40(const sVec3_2B6CF8& a, const sVec3_2B6CF8& b)
{
    return sVec3_2B6CF8(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

struct sShape_2B6F40 {
    int type;
    int id;
    float cx, cy, cz;       // 0x08
    float e0, e1, e2;       // 0x14
    float ax, ay, az;       // 0x20
};

/**
 * Computes the local bounding box of one audio volume record by its type.
 *
 * Types 0, 2 and 3 (sphere-like) use centre +- radius; type 1 builds an orthonormal basis from its axis (+0x20) and the
 * z axis and extends the centre by the three half-extents along it.
 *
 * @param shape  the volume record (type, id, centre, extents, axis)
 * @param lo  receives the minimum corner
 * @param hi  receives the maximum corner
 */
extern "C" void cWorldSound_CalcAudioVolumeBox(cWorldSound* self, int* shape, sVec3_2B6CF8* lo, sVec3_2B6CF8* hi)
{
    sShape_2B6F40* shapeRec = (sShape_2B6F40*)shape;
    switch (shapeRec->type)
    {
    case 0:
        lo->x = shapeRec->cx - shapeRec->e0;
        lo->y = shapeRec->cy - shapeRec->e0;
        lo->z = shapeRec->cz - shapeRec->e0;
        hi->x = shapeRec->cx + shapeRec->e0;
        hi->y = shapeRec->cy + shapeRec->e0;
        hi->z = shapeRec->cz + shapeRec->e0;
        break;
    case 1:
    {
        sVec3_2B6CF8 axisDir;
        sVec3_2B6CF8 sideDir;
        sVec3_2B6CF8 upDir;
        sVec3_2B6CF8 z;
        axisDir = sVec3_2B6CF8(shapeRec->ax, shapeRec->ay, shapeRec->az);
        z = sVec3_2B6CF8(0.0f, 0.0f, 1.0f);
        sideDir = Cross_2B6F40(z, axisDir);
        sideDir = sideDir * (1.0f / wtSqrt_2B6F40(sideDir.x * sideDir.x + sideDir.y * sideDir.y + sideDir.z * sideDir.z));
        upDir = Cross_2B6F40(axisDir, sideDir);
        *lo = sVec3_2B6CF8(shapeRec->cx, shapeRec->cy, shapeRec->cz);
        *hi = sVec3_2B6CF8(shapeRec->cx, shapeRec->cy, shapeRec->cz);
        *lo -= axisDir * shapeRec->e0 + sideDir * shapeRec->e1 + upDir * shapeRec->e2;
        *hi += axisDir * shapeRec->e0 + sideDir * shapeRec->e1 + upDir * shapeRec->e2;
        break;
    }
    case 2:
        lo->x = shapeRec->cx - shapeRec->e0;
        lo->y = shapeRec->cy - shapeRec->e0;
        lo->z = shapeRec->cz - shapeRec->e0;
        hi->x = shapeRec->cx + shapeRec->e0;
        hi->y = shapeRec->cy + shapeRec->e0;
        hi->z = shapeRec->cz + shapeRec->e0;
        break;
    case 3:
        lo->x = shapeRec->cx - shapeRec->e0;
        lo->y = shapeRec->cy - shapeRec->e0;
        lo->z = shapeRec->cz - shapeRec->e0;
        hi->x = shapeRec->cx + shapeRec->e0;
        hi->y = shapeRec->cy + shapeRec->e0;
        hi->z = shapeRec->cz + shapeRec->e0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_AddInstanceToGrid);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sWtVec4_7318 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sWtBox_7318 {
    sWtVec4_7318 min;
    sWtVec4_7318 max;
};
struct sWtCell_7318 {
    int level;
    int x, y, z;
};
struct sWtRoot_7318 {
    sWtCell_7318 cell;
    void* node;
};

extern "C" void cWorldSound_CalcInstAudioBoundingBox_7318(void* self, void* trig, sVec3_2B6CF8* min, sVec3_2B6CF8* max) __asm__("cWorldSound_CalcInstAudioBoundingBox");
extern "C" void nash_Nash_findkey(void* out, const void* box);
extern "C" void nash_Octant_addnode(void* root, int idx, void* item, const void* target);

static inline void insert_7318(char* w, void* item, const sWtBox_7318* box)
{
    sWtCell_7318 c;
    nash_Nash_findkey(&c, box);
    sWtRoot_7318* r = (sWtRoot_7318*)w;
    sWtRoot_7318* rx = c.x < 0 ? r + 4 : r;
    sWtRoot_7318* ry = c.y < 0 ? rx + 2 : rx;
    sWtRoot_7318* rz = c.z < 0 ? ry + 1 : ry;
    nash_Octant_addnode(rz, 2, item, &c);
}

/**
 * Inserts an audio instance into the world octree (game +0x20) by its bounding box, tagging it as type 5.
 *
 * Skips null, inactive or volume-less instances; the box comes from cWorldSound_CalcInstAudioBoundingBox and the cell from nash_Nash_findkey.
 *
 * @param entry  the audio instance
 */
extern "C" void cWorldSound_AddInstanceToGrid(cWorldSound* self, sTriggerEntry18* entry)
{
    char* trig = (char*)entry;
    if (trig == 0 || *(int*)(trig + 0xC) == 0 || *(int*)(*(char**)(trig + 0x10) + 4) == 0) {
        return;
    }
    sVec3_2B6CF8 min;
    sVec3_2B6CF8 max;
    cWorldSound_CalcInstAudioBoundingBox_7318(self, trig, &min, &max);
    char* w = (char*)cAudioInterface_getGame()->BXNash;
    *(int*)(trig + 8) = 5;
    float one = 1.0f;
    sWtBox_7318 box;
    box.min.x = min.x;
    box.min.y = min.y;
    box.min.z = min.z;
    box.min.w = one;
    box.max.x = max.x;
    box.max.y = max.y;
    box.max.z = max.z;
    box.max.w = one;
    insert_7318(w, trig, &box);
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_RemoveInstanceFromGrid);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sBox_2B7410 {
    float x0, y0, z0, w0;
    float x1, y1, z1, w1;
};
struct sCell_2B7410 {
    int pad;
    int ix, iy, iz;
};
struct sQNode_2B7410 {
    char pad[0x10];
    int* items;     // 0x10
};
extern void nash_Nash_findkey_7410(sCell_2B7410* out, sBox_2B7410* box) __asm__("nash_Nash_findkey");
extern "C" void nash_Cell_remnode(int* items, int kind, sWtmEnt18* e, sCell_2B7410* cell, sQNode_2B7410* node);
// PORT: cWorldSound_RemoveInstanceFromOverlapList (defined below) takes the entry pointer as its int id
extern "C" void cWorldSound_RemoveInstanceFromOverlapList_7410(void* self, sWtmEnt18* e) __asm__("cWorldSound_RemoveInstanceFromOverlapList");
void operator_delete(int* p);

static inline void GetCell_2B7410(sCell_2B7410* c, sBox_2B7410* b)
{
    nash_Nash_findkey_7410(c, b);
}
static inline void Remove_2B7410(sQNode_2B7410* n, sWtmEnt18* e, sCell_2B7410* c)
{
    nash_Cell_remnode(n->items, 2, e, c, n);
}
static inline char* Child_2B7410(char* p, int i, int d) { return i > -1 ? p : p + d; }
static inline bool QEmpty_2B7410(int* c)
{
    if (c[0] || c[1] || c[2] || c[3] || c[4] || c[5] || c[6] || c[7]) return false;
    for (int i = 0; i < 3; i++) {
        if (c[8 + i] != 0) return false;
    }
    return true;
}

/**
 * Removes an audio instance from the world octree and from both viewport overlap lists (cWorldSound_RemoveInstanceFromOverlapList).
 *
 * Skips null, inactive or volume-less instances; frees a cell's item list when it becomes empty.
 *
 * @param instAudio  the audio instance
 */
extern "C" void cWorldSound_RemoveInstanceFromGrid(cWorldSound* self, sWtmEnt18* instAudio)
{
    if (instAudio == 0) return;
    if (instAudio->active == 0) return;
    if (((int*)((sTriggerEntry18*)instAudio)->unk10)[1] == 0) return;
    sVec3_2B6CF8 lo;
    sVec3_2B6CF8 hi;
    cWorldSound_CalcInstAudioBoundingBox(self, instAudio, &lo, &hi);
    char* base = (char*)cAudioInterface_getGame()->BXNash;
    sBox_2B7410 box;
    box.x0 = lo.x;
    box.y0 = lo.y;
    box.z0 = lo.z;
    box.w0 = 1.0f;
    box.x1 = hi.x;
    box.y1 = hi.y;
    box.z1 = hi.z;
    box.w1 = 1.0f;
    (*(int*)(base + 0xA0))++;
    sCell_2B7410 cell;
    GetCell_2B7410(&cell, &box);
    sQNode_2B7410* n = (sQNode_2B7410*)Child_2B7410(Child_2B7410(Child_2B7410(base, cell.ix, 80), cell.iy, 40), cell.iz, 20);
    Remove_2B7410(n, instAudio, &cell);
    if (QEmpty_2B7410(n->items)) {
        operator_delete(n->items);
        n->items = 0;
    }
    cWorldSound_RemoveInstanceFromOverlapList_7410(self, instAudio);
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cAudioOverlapList_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sV4_75D0 {
    float x, y, z, w;
    // (constructors as in peak3project's fork; byte-identical for every landed user)
    sV4_75D0() {}
    sV4_75D0(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));




extern "C" cGame* cAudioInterface_getGame(void);
extern "C" void wtq_75D0(nash_privy_QueryHelper* h, void* partial, void* inside, void* pos, void* node, void* key) __asm__("func_002B83F8");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sV4_75D0 Sub_75D0(const sV4_75D0& a, const sV4_75D0& b)
{
    sV4_75D0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
// The result's mtc1 is its own statement (as in peak3project's fork): cWorldSound_TriggerGridAudioNode schedules the next lwc1 between
// mtc1 and its abs.s, which one block forbids; a C union pun instead of the mtc1 costs cAudioOverlapList_update its v1 (99.92).
static inline float Length_75D0(const sV4_75D0& v)
{
    float r;
    int t;
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
        : "=r"(t)
        : "m"(v));
    __asm__("mtc1      %1, %0" : "=f"(r) : "r"(t));
    return r;
}

/**
 * Refreshes a viewport's overlap list when the position moved more than 10 units (or on first use).
 *
 * Clears the count and queries the eight octree roots (game +0x20) around pos with func_002B83F8, then stores pos as
 * the last position. Called by cWorldSound_TriggerWorldAudio.
 *
 * @param pos  the listener position
 */
extern "C" void cAudioOverlapList_update(cWorldSound* self, const sV4_75D0* pos)
{
    if (self->m_OverlapLists[0].lastposition.w == 0.0f || Length_75D0(Sub_75D0(*pos, *(sV4_75D0*)&self->m_OverlapLists[0].lastposition)) > 10.0f) {
        self->m_OverlapLists[0].numoverlaps = 0;
        nash_Octant* root = (nash_Octant*)cAudioInterface_getGame()->BXNash;
        cWorldSound* selfList[1];
        selfList[0] = (self);
        const sV4_75D0* posPtr = pos;
        cWorldSound** pl = selfList;
        const sV4_75D0** pp = &posPtr;
        nash_privy_QueryHelper octantQuery0, octantQuery1, octantQuery2, octantQuery3, octantQuery4, octantQuery5, octantQuery6, octantQuery7;
        nash_Octant* k;
        if (((void*)root[0].rootcell)) wtq_75D0(&octantQuery0, pl, pl, pp, ((void*)root[0].rootcell), &root[0]);
        k = &root[1];
        if (((void*)root[1].rootcell)) wtq_75D0(&octantQuery1, pl, pl, pp, ((void*)k->rootcell), k);
        k = &root[2];
        if (((void*)root[2].rootcell)) wtq_75D0(&octantQuery2, pl, pl, pp, ((void*)k->rootcell), k);
        k = &root[3];
        if (((void*)root[3].rootcell)) wtq_75D0(&octantQuery3, pl, pl, pp, ((void*)k->rootcell), k);
        k = &root[4];
        if (((void*)root[4].rootcell)) wtq_75D0(&octantQuery4, pl, pl, pp, ((void*)k->rootcell), k);
        k = &root[5];
        if (((void*)root[5].rootcell)) wtq_75D0(&octantQuery5, pl, pl, pp, ((void*)k->rootcell), k);
        k = &root[6];
        if (((void*)root[6].rootcell)) wtq_75D0(&octantQuery6, pl, pl, pp, ((void*)k->rootcell), k);
        k = &root[7];
        if (((void*)root[7].rootcell)) wtq_75D0(&octantQuery7, pl, pl, pp, ((void*)k->rootcell), k);
        *(sV4_75D0*)&self->m_OverlapLists[0].lastposition = *pos;
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_RemoveInstanceFromOverlapList);
#ifdef SKIP_ASM
struct sTriggerIdSlot120 {
    int count;
    int ids[71];
};

static inline int getTriggerId(sTriggerIdSlot120* s, int j)
{
    return s->ids[j];
}

static inline void clearTriggerId(sTriggerIdSlot120* s, int j)
{
    s->ids[j] = 0;
}

/**
 * Removes an audio instance from both viewport overlap lists by clearing its entries.
 *
 * @param id  the audio instance
 */
extern "C" void cWorldSound_RemoveInstanceFromOverlapList(cWorldSound* self, int id)
{
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < ((sTriggerIdSlot120*)self)[i].count; j++) {
            if (getTriggerId(&((sTriggerIdSlot120*)self)[i], j) == id) {
                clearTriggerId(&((sTriggerIdSlot120*)self)[i], j);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_TriggerWorldAudio);
#ifdef SKIP_ASM
extern "C" void cWorldSound_TriggerGridAudioNode(void* self, const sV4_75D0* pos, int k, int id);

/**
 * Fires the world audio triggers around a position for one viewport.
 *
 * Refreshes the viewport's overlap list (slot @p k, 0x120 bytes each) for @p pos with cAudioOverlapList_update, then calls
 * cWorldSound_TriggerGridAudioNode for every non-zero trigger id in it.
 *
 * @param pos  world position to trigger around (On Tour wpos)
 * @param k  viewport index (On Tour ViewPort)
 */
extern "C" void cWorldSound_TriggerWorldAudio(cWorldSound* self, const sV4_75D0* pos, int k)
{
    cAudioOverlapList_update((cWorldSound*)((cWorldSound*)&((sTriggerIdSlot120*)self)[k]), pos);
    for (int i = 0; i < ((sTriggerIdSlot120*)self)[k].count; i++) {
        if (getTriggerId(&((sTriggerIdSlot120*)self)[k], i) != 0) {
            cWorldSound_TriggerGridAudioNode(((sTriggerIdSlot120*)self), pos, k, getTriggerId(&((sTriggerIdSlot120*)self)[k], i));
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_TriggerGridAudioNode);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sWtShape_7908 {
    int type;
    int id;
    float cx, cy, cz;   // 0x08
    float f14;          // 0x14
    float f18;          // 0x18
    float f1C;          // 0x1C
    float f20;          // 0x20
    float f24;          // 0x24
    float f28;          // 0x28
    float f2C;          // 0x2C
};

static inline sVec3_2B6CF8 operator-(const sVec3_2B6CF8& a, const sVec3_2B6CF8& b)
{
    return sVec3_2B6CF8(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline void operator*=(sVec3_2B6CF8& a, float s)
{
    a.x *= s;
    a.y *= s;
    a.z *= s;
}

static inline float Dot_7908(const sVec3_2B6CF8& a, const sVec3_2B6CF8& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Builds the orthonormal frame of a box trigger from its axis.
static inline void Basis_7908(const sWtShape_7908* s, sVec3_2B6CF8& a, sVec3_2B6CF8& b, sVec3_2B6CF8& c)
{
    sVec3_2B6CF8 z;
    a = sVec3_2B6CF8(s->f20, s->f24, s->f28);
    z = sVec3_2B6CF8(0.0f, 0.0f, 1.0f);
    b = Cross_2B6F40(z, a);
    b = b * (1.0f / wtSqrt_2B6F40(b.x * b.x + b.y * b.y + b.z * b.z));
    c = Cross_2B6F40(a, b);
}

extern "C" float wtCurve_7908(void* self, float curve, float t) __asm__("cWorldSound_CalculateVolume");
extern "C" void wtSet_7908(void* mgr, void* obj, int i, int c, int id, int flag, float v, const sVec3_2B6CF8& pos) __asm__("cWorldTriggerManager_HandleTrigger");
extern "C" float func_0031C128(float x);

static inline float Acos_7908(float x)
{
    return 1.5707964f - func_0031C128(x);
}

struct sWtTrig_7908 {
    char pad[0xC];
    char* obj;          // 0x0C
    int* info;          // 0x10
};

extern "C" void wtEval_7908(void* self, const sV4_75D0* listener, int c, sWtTrig_7908* e) __asm__("cWorldSound_TriggerGridAudioNode");

/**
 * Fires the audio volumes of one grid audio instance that contain the listener position.
 *
 * For each volume record, at the object position plus its centre: type 0 (sphere) inside the radius, type 1 (ellipsoid
 * along its axis basis) inside 1, type 2 (cone) inside the radius and the cone angle, type 3 inside the radius (volume
 * 1.0 up to 0.9, then 10 x (1 - distance)). The volume comes from cWorldSound_CalculateVolume with the record's decay curve; hits go
 * to cWorldTriggerManager_HandleTrigger (positional except type 3). Called by cWorldSound_TriggerWorldAudio for each instance in the overlap
 * list.
 *
 * @param listener  the listener position
 * @param viewPort  the viewport
 * @param instAudio  the audio instance
 */
extern "C" void wtEval_7908(void* self, const sV4_75D0* listener, int viewPort, sWtTrig_7908* instAudio)
{
    sVec3_2B6CF8 org;
    sWtShape_7908* recs[4];
    sVec3_2B6CF8 pos;
    sVec3_2B6CF8 toListener;
    int i;
    float dist;
    float volume;

    org = sVec3_2B6CF8(*(float*)(instAudio->obj + 0x40), *(float*)(instAudio->obj + 0x44), *(float*)(instAudio->obj + 0x48));
    int* info = instAudio->info;
    char* base = (char*)info + 8;
    char* shapeCursor = base + (info[0] << 2);
    int n = info[1];

    for (int j = 0; j < n; j++) {
        recs[j] = (sWtShape_7908*)shapeCursor;
        switch (*(int*)shapeCursor) {
        case 0:
            shapeCursor += 0x1C;
            break;
        case 1:
            shapeCursor += 0x30;
            break;
        case 2:
            shapeCursor += 0x30;
            break;
        case 3:
            shapeCursor += 0x18;
            break;
        }
    }

    for (i = 0; i < n; i++) {
        sWtShape_7908* shapeRec = recs[i];
        switch (shapeRec->type) {
        case 0: {
            pos = org + sVec3_2B6CF8(shapeRec->cx, shapeRec->cy, shapeRec->cz);
            sV4_75D0 diff = Sub_75D0(*listener, sV4_75D0(pos.x, pos.y, pos.z, 0.0f));
            toListener.x = diff.x;
            toListener.y = diff.y;
            toListener.z = diff.z;
            dist = __builtin_fabsf(wtSqrt_2B6F40(toListener.x * toListener.x + toListener.y * toListener.y + toListener.z * toListener.z));
            if (dist < shapeRec->f14) {
                wtSet_7908(gWorldTriggerManager, instAudio->obj, i, viewPort, shapeRec->id, 1, wtCurve_7908(self, shapeRec->f18, dist / shapeRec->f14), pos);
            }
            break;
        }
        case 1: {
            pos = org + sVec3_2B6CF8(shapeRec->cx, shapeRec->cy, shapeRec->cz);
            {
                sV4_75D0 diff = Sub_75D0(*listener, sV4_75D0(pos.x, pos.y, pos.z, 0.0f));
                toListener.x = diff.x;
                toListener.y = diff.y;
                toListener.z = diff.z;
            }
            sVec3_2B6CF8 axisDir;
            sVec3_2B6CF8 sideDir;
            sVec3_2B6CF8 cc;
            Basis_7908(shapeRec, axisDir, sideDir, cc);
            float da = Dot_7908(axisDir, toListener);
            float db = Dot_7908(sideDir, toListener);
            float dc = Dot_7908(cc, toListener);
            float ta = shapeRec->f14 != 0.0f ? (da * da) / (shapeRec->f14 * shapeRec->f14) : 0.0f;
            float tb = shapeRec->f18 != 0.0f ? (db * db) / (shapeRec->f18 * shapeRec->f18) : 0.0f;
            float tc = shapeRec->f1C != 0.0f ? (dc * dc) / (shapeRec->f1C * shapeRec->f1C) : 0.0f;
            dist = ta + tb + tc;
            if (dist < 1.0f) {
                wtSet_7908(gWorldTriggerManager, instAudio->obj, i, viewPort, shapeRec->id, 1, wtCurve_7908(self, shapeRec->f2C, dist), pos);
            }
            break;
        }
        case 2: {
            pos = org + sVec3_2B6CF8(shapeRec->cx, shapeRec->cy, shapeRec->cz);
            sVec3_2B6CF8 dir = sVec3_2B6CF8(listener->x, listener->y, listener->z) - pos;
            dist = __builtin_fabsf(wtSqrt_2B6F40(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z));
            if (dist < shapeRec->f14) {
                dir *= 1.0f / dist;
                float dot = Dot_7908(dir, sVec3_2B6CF8(shapeRec->f18, shapeRec->f1C, shapeRec->f20));
                if (dot > shapeRec->f28) {
                    volume = wtCurve_7908(self, shapeRec->f24, dist / shapeRec->f14);
                    float cone = Acos_7908(shapeRec->f28);
                    volume *= wtCurve_7908(self, shapeRec->f2C, Acos_7908(dot) / cone);
                    wtSet_7908(gWorldTriggerManager, instAudio->obj, i, viewPort, shapeRec->id, 1, volume, pos);
                }
            }
            break;
        }
        case 3: {
            pos = org + sVec3_2B6CF8(shapeRec->cx, shapeRec->cy, shapeRec->cz);
            sV4_75D0 diff = Sub_75D0(*listener, sV4_75D0(pos.x, pos.y, pos.z, 0.0f));
            dist = __builtin_fabsf(Length_75D0(diff));
            if (dist < shapeRec->f14) {
                if (dist <= 0.9f) {
                    volume = 1.0f;
                } else {
                    volume = 1.0f - dist;
                    volume *= 10.0f;
                }
                wtSet_7908(gWorldTriggerManager, instAudio->obj, i, viewPort, shapeRec->id, 0, volume, pos);
            }
            break;
        }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_GetAudioVolumeTriggers);
#ifdef SKIP_ASM
/**
 * Returns the trigger numbers of an object's (up to four) audio volumes through id0..id3 (0 when absent).
 *
 * Walks the object's audio data (+0x8C -> +0x10) record by record.
 *
 * @param obj  the instance record
 * @param id0  receives the first trigger
 * @param id1  receives the second
 * @param id2  receives the third
 * @param id3  receives the fourth
 */
extern "C" void cWorldSound_GetAudioVolumeTriggers(cWorldSound* self, void* obj, int* id0, int* id1, int* id2, int* id3)
{
    int* hdr = *(int**)(*(char**)((char*)obj + 0x8C) + 0x10);
    char* base = (char*)hdr + 8;
    char* cursor = base + (hdr[0] << 2);
    int n = hdr[1];
    int* recs[4];
    int i;
    int j;

    for (i = 0; i < n; i++) {
        recs[i] = (int*)cursor;
        switch (*(int*)cursor) {
        case 0:
            cursor += 0x1C;
            break;
        case 1:
            cursor += 0x30;
            break;
        case 2:
            cursor += 0x30;
            break;
        case 3:
            cursor += 0x18;
            break;
        }
    }

    *id0 = 0;
    *id1 = 0;
    *id2 = 0;
    *id3 = 0;

    for (j = 0; j < n; j++) {
        int recId;
        switch (*recs[0]) {
        case 0:
            recId = recs[j][1];
            break;
        case 1:
            recId = recs[j][1];
            break;
        case 2:
            recId = recs[j][1];
            break;
        case 3:
            recId = recs[j][1];
            break;
        default:
            recId = recs[j][1];
            break;
        }
        switch (j) {
        case 0:
            *id0 = recId;
            break;
        case 1:
            *id1 = recId;
            break;
        case 2:
            *id2 = recId;
            break;
        case 3:
            *id3 = recId;
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_CalculateVolume);
#ifdef SKIP_ASM
/**
 * Maps a distance ratio (0..1) to a volume with decay curve type 0-5; other types give 0.
 *
 * 0: 1 - r^2; 1: 1 - r / (1.5 - r / 2); 2: 1 - r; 3: (1 - r) / (1.5 - (1 - r) / 2); 4: (1 - r)^2; 5: 1 up to 0.7, then
 * (1 - r) x 3.33. The curve arrives as a float. Called by cWorldSound_TriggerGridAudioNode (with the cWorldSound in a0).
 *
 * @param curve  the decay curve type
 * @param ratio  the distance ratio
 * @return the volume
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" float cWorldSound_CalculateVolume(float curve, float ratio)
{
    switch ((int)curve) {
    case 0:
        ratio = ratio * ratio;
        ratio = 1.0f - ratio;
        break;
    case 1:
        ratio = ratio / (1.5f - ratio * 0.5f);
        ratio = 1.0f - ratio;
        break;
    case 2:
        ratio = 1.0f - ratio;
        break;
    case 3:
        ratio = 1.0f - ratio;
        ratio = ratio / (1.5f - ratio * 0.5f);
        break;
    case 4:
        ratio = 1.0f - ratio;
        ratio = ratio * ratio;
        break;
    case 5:
        if (ratio <= 0.7f) {
            ratio = 1.0f;
        } else {
            ratio = 1.0f - ratio;
            ratio = ratio * 3.3333332538604736f;
        }
        break;
    default:
        ratio = 0.0f;
        break;
    }
    return ratio;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B83B8);
#ifdef SKIP_ASM
/**
 * Returns self; no other effect.
 *
 * Declared variadic; no caller found.
 *
 * @return self
 */
extern "C" void* func_002B83B8(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B83F8);
#ifdef SKIP_ASM
extern "C" void func_002BADD0(void* node, void* list);

struct sOctKey83F8 {
    int lvl;
    int x;
    int y;
    int z;
};

struct sOctItem83F8 {
    sOctItem83F8* next;
    int f4;
    int type;
};

struct sOctNode83F8 {
    sOctNode83F8* child[8];
    char pad20[8];
    sOctItem83F8* items;    // 0x28
};

struct sOctList83F8 {
    int count;
    sOctItem83F8* items[1];
};

union sFloatBits83F8 {
    int i;
    float f;
};

static inline int Classify83F8(const float* v, const sOctKey83F8* key)
{
    sFloatBits83F8 u;
    u.i = (key->lvl + 0x7F) << 23;
    float size = u.f;
    if (v[0] < ((float)key->x - 0.2f) * size)
        return 1;
    if (((float)(key->x + 1) + 0.2f) * size < v[0])
        return 1;
    if (v[1] < ((float)key->y - 0.2f) * size)
        return 1;
    if (((float)(key->y + 1) + 0.2f) * size < v[1])
        return 1;
    if (v[2] < ((float)key->z - 0.2f) * size)
        return 1;
    if (((float)(key->z + 1) + 0.2f) * size < v[2])
        return 1;
    return 2;
}

/**
 * Recursively collects the octree's audio items (type 5) near a position into the crossing or inside list.
 *
 * A node whose cell (key, grown by 0.2 cells) contains the position adds its type-5 items to *partial and recurses into
 * its children down to level 11; otherwise nothing. Quirk: the cell test only returns 1 or 2, so the inside branch (all
 * items to *inside via func_002BADD0) is never taken. Called by cAudioOverlapList_update for each root.
 *
 * @param partial  the list for crossing cells
 * @param inside  the list for cells fully inside
 * @param pos  the position
 * @param node  the octree cell
 * @param key  its cell key (level, x, y, z)
 */
extern "C" void func_002B83F8(void* self, sOctList83F8** partial, sOctList83F8** inside, float** pos,
                              sOctNode83F8* node, sOctKey83F8* key)
{
    int overlap = Classify83F8(*pos, key);
    if (overlap == 0)
    {
        for (sOctItem83F8* item = node->items; item != 0; item = item->next)
        {
            if (item->type == 5)
            {
                sOctList83F8* list = *inside;
                list->items[list->count++] = item;
            }
        }
        if (node->child[0]) func_002BADD0(node->child[0], inside);
        if (node->child[1]) func_002BADD0(node->child[1], inside);
        if (node->child[2]) func_002BADD0(node->child[2], inside);
        if (node->child[3]) func_002BADD0(node->child[3], inside);
        if (node->child[4]) func_002BADD0(node->child[4], inside);
        if (node->child[5]) func_002BADD0(node->child[5], inside);
        if (node->child[6]) func_002BADD0(node->child[6], inside);
        if (node->child[7]) func_002BADD0(node->child[7], inside);
    }
    else if (overlap == 2)
    {
        for (sOctItem83F8* item = node->items; item != 0; item = item->next)
        {
            if (item->type == 5)
            {
                sOctList83F8* list = *partial;
                list->items[list->count++] = item;
            }
        }
        if (key->lvl == 11)
            return;
        sOctKey83F8 k;
        k.lvl = key->lvl - 1;
        k.x = key->x * 2;
        k.y = key->y * 2;
        k.z = key->z * 2;
        if (node->child[0]) func_002B83F8(self, partial, inside, pos, node->child[0], &k);
        k.x++;
        if (node->child[4]) func_002B83F8(self, partial, inside, pos, node->child[4], &k);
        k.y++;
        if (node->child[6]) func_002B83F8(self, partial, inside, pos, node->child[6], &k);
        k.x--;
        if (node->child[2]) func_002B83F8(self, partial, inside, pos, node->child[2], &k);
        k.z++;
        if (node->child[3]) func_002B83F8(self, partial, inside, pos, node->child[3], &k);
        k.x++;
        if (node->child[7]) func_002B83F8(self, partial, inside, pos, node->child[7], &k);
        k.y--;
        if (node->child[5]) func_002B83F8(self, partial, inside, pos, node->child[5], &k);
        k.x--;
        if (node->child[1]) func_002B83F8(self, partial, inside, pos, node->child[1], &k);
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B8818);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sColour_8818 {
    float a, b, c, d;
};

struct sKey_8818 {
    unsigned int idx : 8;
    unsigned int id : 24;
    int pad;
};

struct sPair_8818 {
    int a;
    int b;
};

extern sColour_8818 D_004D3B18;
extern sColour_8818 D_004D3B28;
extern sColour_8818 D_004D3B38;
extern sColour_8818 D_004D3B48;
extern sColour_8818 D_004D3B58;
extern sColour_8818 D_004D3B68;
extern sColour_8818 D_004D3B78;
extern sColour_8818 D_004D3B88;
extern sColour_8818 D_004D3B98;
extern sColour_8818 D_004D3BA8;
extern sColour_8818 D_004D3BB8;
extern sColour_8818 D_004D3BC8;
extern sColour_8818 D_004D3BD8;
extern sColour_8818 D_004D3BE8;
extern sColour_8818 D_004D3BF8;
extern sColour_8818 D_004D3C08;
extern sColour_8818 D_004D3C18;
extern sColour_8818 D_004D3C28;
extern sColour_8818 D_004D3C38;
extern sColour_8818 D_004D3C48;
extern sColour_8818 D_004D3C58;
extern sColour_8818 D_004D3C68;
extern sColour_8818 D_004D3C78;
extern sColour_8818 D_004D3C88;
extern sColour_8818 D_004D3C98;
extern sColour_8818 D_004D3CA8;
extern sColour_8818 D_004D3CB8;
extern sColour_8818 D_004D3CC8;
extern sColour_8818 D_004D3CD8;
extern sColour_8818 D_004D3CE8;
extern sColour_8818 D_004D3CF8;
extern sColour_8818 D_004D3D08;
extern sColour_8818 D_004D3D18;
extern sColour_8818 D_004D3D28;
extern sColour_8818 D_004D3D38;
extern sColour_8818 D_004D3D48;
extern sColour_8818 D_004D3D58;
extern sColour_8818 D_004D3D68;
extern sColour_8818 D_004D3D78;
extern sColour_8818 D_004D3D88;
extern sColour_8818 D_004D3D98;
extern sColour_8818 D_004D3DA8;
extern sColour_8818 D_004D3DB8;
extern sColour_8818 D_004D3DC8;
extern sColour_8818 D_004D3DD8;
extern sColour_8818 D_004D3DE8;
extern sColour_8818 D_004D3DF8;
extern sColour_8818 D_004D3E08;
extern sColour_8818 D_004D3E18;
extern sColour_8818 D_004D3E28;
extern sColour_8818 D_004D3E38;
extern sColour_8818 D_004D3E48;
extern sColour_8818 D_004D3E58;
extern sColour_8818 D_004D3E68;
extern sColour_8818 D_004D3E78;
extern sColour_8818 D_004D3E88;
extern sPair_8818 D_004A5260;
extern sKey_8818 D_004A5268;
extern sKey_8818 D_004A5270;
extern sKey_8818 D_004A5278;
extern sKey_8818 D_004A5280;
extern sKey_8818 D_004A5288;
extern sKey_8818 D_004A5290;
extern sKey_8818 D_004A5298;
extern sKey_8818 D_004A52A0;
extern sKey_8818 D_004A52A8;
extern sKey_8818 D_004A52B0;
extern sKey_8818 D_004A52B8;
extern sKey_8818 D_004A52C0;
extern sKey_8818 D_004A52C8;
extern sKey_8818 D_004A52D0;

static inline void Key_8818(sKey_8818* p, int i)
{
    p->idx = i;
    p->id = i;
}

static inline void Pair_8818(sPair_8818* p)
{
    p->b = 0;
    p->a = 0;
}

extern "C" void func_002B8818_impl(int init, int prio) __asm__("func_002B8818");

/**
 * Static initializer of the unit: fills the colour tables D_004D3B18.. and the 14 colour keys D_004A5268..
 *
 * Runs only for (1, 0xFFFF): sets the float colour tables, clears D_004A5260 and gives each key D_004A5268 + 8k index
 * and id k (0..13). Listed in the global constructor table.
 *
 * @param init  nonzero to initialise
 * @param prio  the initialisation priority (acts on 0xFFFF)
 */
extern "C" void func_002B8818_impl(int init, int prio)
{
    if (prio == 0xFFFF && init) {
        D_004D3B18.a = 1.0f;
        D_004D3B18.b = 1.0f;
        D_004D3B18.c = 1.0f;
        D_004D3B18.d = 0.5f;
        D_004D3B28.a = 0.4998680055141449f;
        D_004D3B28.b = 1.0f;
        D_004D3B28.c = 1.0f;
        D_004D3B28.d = 0.0f;
        D_004D3B38.a = 0.12951700389385223f;
        D_004D3B38.b = 1.0f;
        D_004D3B38.c = 1.0f;
        D_004D3B38.d = 0.0f;
        D_004D3B48.a = 0.22426900267601013f;
        D_004D3B48.b = 0.9402310252189636f;
        D_004D3B48.c = 0.5425059795379639f;
        D_004D3B48.d = 0.0004030000127386302f;
        D_004D3B58.a = 0.4652079939842224f;
        D_004D3B58.b = 0.32170501351356506f;
        D_004D3B58.c = 0.9943439960479736f;
        D_004D3B58.d = 0.0f;
        D_004D3B68.a = 0.17792600393295288f;
        D_004D3B68.b = 1.0f;
        D_004D3B68.c = 1.0f;
        D_004D3B68.d = 0.0f;
        D_004D3B78.a = 0.4411740005016327f;
        D_004D3B78.b = 0.41388601064682007f;
        D_004D3B78.c = 0.3211260139942169f;
        D_004D3B78.d = 0.4398239850997925f;
        D_004D3B88.a = 0.500997006893158f;
        D_004D3B88.b = 0.24849599599838257f;
        D_004D3B88.c = 1.0f;
        D_004D3B88.d = 1.0f;
        D_004D3B98.a = 0.27687299251556396f;
        D_004D3B98.b = 0.6395620107650757f;
        D_004D3B98.c = 0.13423900306224823f;
        D_004D3B98.d = 0.0f;
        D_004D3BA8.a = 0.8936439752578735f;
        D_004D3BA8.b = 0.5263469815254211f;
        D_004D3BA8.c = 0.5051450133323669f;
        D_004D3BA8.d = 0.8687300086021423f;
        D_004D3BB8.a = 1.0f;
        D_004D3BB8.b = 1.0f;
        D_004D3BB8.c = 1.0f;
        D_004D3BB8.d = 1.0f;
        D_004D3BC8.a = 1.0f;
        D_004D3BC8.b = 0.0f;
        D_004D3BC8.c = 0.0f;
        D_004D3BC8.d = 0.0f;
        D_004D3BD8.a = 1.0f;
        D_004D3BD8.b = 0.550000011920929f;
        D_004D3BD8.c = 0.5350000262260437f;
        D_004D3BD8.d = 0.550000011920929f;
        D_004D3BE8.a = 1.0f;
        D_004D3BE8.b = 1.0f;
        D_004D3BE8.c = 1.0f;
        D_004D3BE8.d = 1.0f;
        D_004D3BF8.a = 1.0f;
        D_004D3BF8.b = 1.0f;
        D_004D3BF8.c = 1.0f;
        D_004D3BF8.d = 1.0f;
        D_004D3C08.a = 1.0f;
        D_004D3C08.b = 0.0f;
        D_004D3C08.c = 0.0f;
        D_004D3C08.d = 0.0f;
        D_004D3C18.a = 1.0f;
        D_004D3C18.b = 0.0f;
        D_004D3C18.c = 0.0f;
        D_004D3C18.d = 0.0f;
        D_004D3C28.a = 1.0f;
        D_004D3C28.b = 1.0f;
        D_004D3C28.c = 1.0f;
        D_004D3C28.d = 1.0f;
        D_004D3C38.a = 1.0f;
        D_004D3C38.b = 0.5f;
        D_004D3C38.c = 1.0f;
        D_004D3C38.d = 0.5f;
        D_004D3C48.a = 1.0f;
        D_004D3C48.b = 0.5f;
        D_004D3C48.c = 1.0f;
        D_004D3C48.d = 0.5f;
        D_004D3C58.a = 1.0f;
        D_004D3C58.b = 0.5f;
        D_004D3C58.c = 1.0f;
        D_004D3C58.d = 0.5f;
        D_004D3C68.a = 1.0f;
        D_004D3C68.b = 0.7828119993209839f;
        D_004D3C68.c = 0.06171900033950806f;
        D_004D3C68.d = 0.03984399884939194f;
        D_004D3C78.a = 1.0f;
        D_004D3C78.b = 1.0f;
        D_004D3C78.c = 0.7988259792327881f;
        D_004D3C78.d = 0.0f;
        D_004D3C88.a = 0.9886710047721863f;
        D_004D3C88.b = 0.9230499863624573f;
        D_004D3C88.c = 0.34311699867248535f;
        D_004D3C88.d = 0.0f;
        D_004D3C98.a = 1.0f;
        D_004D3C98.b = 1.0f;
        D_004D3C98.c = 1.0f;
        D_004D3C98.d = 1.0f;
        D_004D3CA8.a = 1.0f;
        D_004D3CA8.b = 1.0f;
        D_004D3CA8.c = 0.7976430058479309f;
        D_004D3CA8.d = 0.0f;
        D_004D3CB8.a = 1.0f;
        D_004D3CB8.b = 0.9222670197486877f;
        D_004D3CB8.c = 0.34179699420928955f;
        D_004D3CB8.d = 0.0f;
        D_004D3CC8.a = 1.0f;
        D_004D3CC8.b = 0.7847639918327332f;
        D_004D3CC8.c = 0.06054700165987015f;
        D_004D3CC8.d = 0.04062499850988388f;
        D_004D3CD8.a = 1.0f;
        D_004D3CD8.b = 0.800000011920929f;
        D_004D3CD8.c = 0.44999998807907104f;
        D_004D3CD8.d = 1.0f;
        D_004D3CE8.a = 1.0f;
        D_004D3CE8.b = 0.3281260132789612f;
        D_004D3CE8.c = 1.0f;
        D_004D3CE8.d = 0.3539060056209564f;
        D_004D3CF8.a = 1.0f;
        D_004D3CF8.b = 1.0f;
        D_004D3CF8.c = 1.0f;
        D_004D3CF8.d = 1.0f;
        D_004D3D08.a = 1.0f;
        D_004D3D08.b = 1.0f;
        D_004D3D08.c = 1.0f;
        D_004D3D08.d = 1.0f;
        D_004D3D18.a = 1.0f;
        D_004D3D18.b = 0.8613290190696716f;
        D_004D3D18.c = 0.38124799728393555f;
        D_004D3D18.d = 0.0f;
        D_004D3D28.a = 1.0f;
        D_004D3D28.b = 1.0f;
        D_004D3D28.c = 1.0f;
        D_004D3D28.d = 1.0f;
        D_004D3D38.a = 1.0f;
        D_004D3D38.b = 0.3244380056858063f;
        D_004D3D38.c = 1.0f;
        D_004D3D38.d = 0.0f;
        D_004D3D48.a = 1.0f;
        D_004D3D48.b = 1.0f;
        D_004D3D48.c = 1.0f;
        D_004D3D48.d = 1.0f;
        D_004D3D58.a = 1.0f;
        D_004D3D58.b = 1.0f;
        D_004D3D58.c = 0.0f;
        D_004D3D58.d = 0.0f;
        D_004D3D68.a = 0.7007060050964355f;
        D_004D3D68.b = 0.0f;
        D_004D3D68.c = 0.0f;
        D_004D3D68.d = 0.0f;
        D_004D3D78.a = 1.0f;
        D_004D3D78.b = 0.7828530073165894f;
        D_004D3D78.c = 0.06140600144863129f;
        D_004D3D78.d = 0.04146299883723259f;
        D_004D3D88.a = 1.0f;
        D_004D3D88.b = 0.32487601041793823f;
        D_004D3D88.c = 1.0f;
        D_004D3D88.d = 0.35280099511146545f;
        D_004D3D98.a = 1.0f;
        D_004D3D98.b = 1.0f;
        D_004D3D98.c = 0.7977359890937805f;
        D_004D3D98.d = 0.0f;
        D_004D3DA8.a = 1.0f;
        D_004D3DA8.b = 0.4937489926815033f;
        D_004D3DA8.c = 1.0f;
        D_004D3DA8.d = 0.49179700016975403f;
        D_004D3DB8.a = 1.0f;
        D_004D3DB8.b = 1.0f;
        D_004D3DB8.c = 1.0f;
        D_004D3DB8.d = 1.0f;
        D_004D3DC8.a = 1.0f;
        D_004D3DC8.b = 0.800000011920929f;
        D_004D3DC8.c = 1.0f;
        D_004D3DC8.d = 0.800000011920929f;
        D_004D3DD8.a = 1.0f;
        D_004D3DD8.b = 1.0f;
        D_004D3DD8.c = 0.0f;
        D_004D3DD8.d = 0.0f;
        D_004D3DE8.a = 1.0f;
        D_004D3DE8.b = 1.0f;
        D_004D3DE8.c = 0.0f;
        D_004D3DE8.d = 0.0f;
        D_004D3DF8.a = 1.0f;
        D_004D3DF8.b = 1.0f;
        D_004D3DF8.c = 0.0f;
        D_004D3DF8.d = 0.0f;
        D_004D3E08.a = 1.0f;
        D_004D3E08.b = 0.3273420035839081f;
        D_004D3E08.c = 1.0f;
        D_004D3E08.d = 0.35390499234199524f;
        D_004D3E18.a = 1.0f;
        D_004D3E18.b = 1.0f;
        D_004D3E18.c = 0.061000000685453415f;
        D_004D3E18.d = 0.039000000804662704f;
        D_004D3E28.a = 1.0f;
        D_004D3E28.b = 1.0f;
        D_004D3E28.c = 0.7999979853630066f;
        D_004D3E28.d = 0.0f;
        D_004D3E38.a = 1.0f;
        D_004D3E38.b = 0.9589840173721313f;
        D_004D3E38.c = 0.6143789887428284f;
        D_004D3E38.d = 0.0f;
        D_004D3E48.a = 1.0f;
        D_004D3E48.b = 0.9285150170326233f;
        D_004D3E48.c = 0.3399839997291565f;
        D_004D3E48.d = 0.0f;
        D_004D3E58.a = 1.0f;
        D_004D3E58.b = 0.8738250136375427f;
        D_004D3E58.c = 0.24931700527668f;
        D_004D3E58.d = 0.017578000202775f;
        D_004D3E68.a = 1.0f;
        D_004D3E68.b = 0.7843739986419678f;
        D_004D3E68.c = 0.06132800132036209f;
        D_004D3E68.d = 0.03945299983024597f;
        D_004D3E78.a = 1.0f;
        D_004D3E78.b = 0.0f;
        D_004D3E78.c = 1.0f;
        D_004D3E78.d = 0.0f;
        D_004D3E88.a = 1.0f;
        D_004D3E88.b = 1.0f;
        D_004D3E88.c = 0.0f;
        D_004D3E88.d = 0.0f;
        Pair_8818(&D_004A5260);
        Key_8818(&D_004A5268, 0);
        Key_8818(&D_004A5270, 1);
        Key_8818(&D_004A5278, 2);
        Key_8818(&D_004A5280, 3);
        Key_8818(&D_004A5288, 4);
        Key_8818(&D_004A5290, 5);
        Key_8818(&D_004A5298, 6);
        Key_8818(&D_004A52A0, 7);
        Key_8818(&D_004A52A8, 8);
        Key_8818(&D_004A52B0, 9);
        Key_8818(&D_004A52B8, 10);
        Key_8818(&D_004A52C0, 11);
        Key_8818(&D_004A52C8, 12);
        Key_8818(&D_004A52D0, 13);
    }
}
#endif

