#include "common.h"
#include "bx/math3d.h"
#include "world/instancerecord.h"
#include "vtable.h"
#include "intersect/bxnash.h"
#include "sound/ssxaudio.h"
#include "sound/streamsys.h"
#include "sound/bankmanager.h"
#include "main/game.h"
#include "sound/icepick/worldsound.h"
#include "sound/icepick/worldtriggermanager.h"
#include "ai/rider.h"
#include "object/instancenode.h"
#include "world/worldview.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* __20cWorldTriggerManager(cWorldTriggerManager* self);
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
        gWorldTriggerManager = (cWorldTriggerManager*)(__20cWorldTriggerManager((cWorldTriggerManager*)mem));
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", WORLDTRIGGERMANAGER_DeInit);
#ifdef SKIP_ASM
extern "C" void _$_20cWorldTriggerManager(cWorldTriggerManager* self, int flags);

/**
 * Destroys the global world trigger manager, if there is one, and clears the pointer.
 *
 * Calls the destructor with flags 3, so the object's memory is freed too.
 */
extern "C" void WORLDTRIGGERMANAGER_DeInit()
{
    delete gWorldTriggerManager;
    gWorldTriggerManager = 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", __21cWorldTriggerInstance);
#ifdef SKIP_ASM
extern unsigned char str_empty_j[];

/**
 * Constructs a world trigger instance (0x70 bytes): clears every field, audio handle -1, last frame -1.0.
 *
 * Sets the handle (+0x30) to -1, the position's w (+0x2C) to 1.0, the swap string (+0x40) empty and the last animation
 * frame (+0x68) to -1.0; everything else is 0. The cWorldTriggerManager constructor runs it on each of its 40
 * instances.
 */
cWorldTriggerInstance::cWorldTriggerInstance()
{
    m_iAudioHandle = -1;
    m_vPosition.w = 1.0f;
    m_bInUse = 0;
    m_iObjectRef = 0;
    m_iVolNum = 0;
    m_eAudioTrigger = 0;
    m_fVolume = 0.0f;
    m_bPositional = 0;
    m_vPosition.x = 0.0f;
    m_vPosition.y = 0.0f;
    m_vPosition.z = 0.0f;
    m_bTouch = 0;
    m_bCrowdTrigger = 0;
    m_iSwapString[0] = str_empty_j[0];
    m_iLastAnimFrame = -1.0f;
    m_iBankReference = 0;
    m_iPatchReference = 0;
    m_bLastAnimFrameDecrease = 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", _$_21cWorldTriggerInstance);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void _$_21cWorldTriggerInstance(cWorldTriggerInstance* self, int flags);

/**
 * Destroys a world trigger instance: nothing to release; deletes the object when bit 0 of flags is set.
 *
 * cWorldTriggerManager::~cWorldTriggerManager calls it with 0 for each instance.
 */
cWorldTriggerInstance::~cWorldTriggerInstance()
{
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
 * @return true on a match, else false
 */
bool cWorldTriggerInstance::SameTriggerInstance(int objectRef, int volNum, int viewPort)
{
    if (objectRef == m_iObjectRef
        && volNum == m_iVolNum
        && viewPort == m_iViewPort) {
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance_Update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sWorldTrigger70;
struct sTriggerIdCache;
extern "C" void* memset(void* dst, int c, int n);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" float* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(void* self);
extern "C" cRider* cSSXAudio_GetFocusRider(void* self, int which);
extern "C" int strcmp(const void* a, const void* b);
extern "C" int func_0029B818(void* sys, void* name, int* rec);
extern "C" void cCrowdSys_OpenObjectOpts(void* sys);
extern "C" int cCrowdSys_AddObject(void* sys, int a);
extern "C" void cCrowdSys_SetObjectLoudness(void* sys, int idx, int v);
extern "C" cBankMonitorInstance* cBankMonitor_BANKMONITOR_GetHandle(void* sub, int idx, int a2);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* self, int idx, int a2, float v);
extern "C" int cWorldTriggerInstance_CrowdTrigger(cWorldTriggerInstance* self, int idx);
extern "C" int cWorldTriggerInstance_TriggerToPatch(cWorldTriggerInstance* self, int idx, int* type, int* a, int* b, int c, int d);
extern "C" int cWorldTriggerInstance_SwapTriggerToString(cWorldTriggerInstance* self, int idx, char* dst);
extern "C" void cWorldTriggerInstance_Reset(cWorldTriggerInstance* self);
extern "C" bool cWorldTriggerManager_PlayingTriggerInstance(cWorldTriggerInstance** self, int a1, int a2,
                                                            cWorldTriggerInstance* except);
extern "C" int cWorldTriggerManager_SwapTriggerPlaying(cWorldTriggerInstance** self, int id);
extern "C" int cWorldTriggerManager_AfterCollisionTrigger(cWorldTriggerManager* self, unsigned int i);
extern "C" int cWorldTriggerManager_ObjectHit(cWorldTriggerManager* self, int id);
extern "C" int cWorldTriggerManager_SyncToAnimationTrigger(cWorldTriggerManager* self, unsigned int i);
extern "C" int cWorldTriggerManager_ObjectRefFirstFrame(cWorldTriggerManager* self, int objectRef,
    cWorldTriggerInstance* state);
extern unsigned char str_empty_j[];

#define SND_4C38_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SND_4C38_MAX(a, b) ((a) > (b) ? (a) : (b))

// cSndPlayOpts::OPTS_Vol, inlined: the level's base volume and its clamped play volume.
static inline void optsVol_4C38(cBankManager* m, int v)
{
    cBankMonitor* o = &m->BankMonitor;
    o->mBaseVol[m->BankMonitor.mInst] = v;
    o->mCurrent[m->BankMonitor.mInst].vol = SND_4C38_MAX(0, SND_4C38_MIN(o->mBaseVol[o->mInst], 0x7F));
}

// cBankSys::SFX_Begin(bank, patch), inlined: a new play-option level with the defaults, and its bank and patch.
static inline void sfxBegin_4C38(cSSXAudio* self, int a, int b)
{
    cBankManager* m = self->mpVBankManager;
    cBankMonitor* s = &m->BankMonitor;
    m->BankMonitor.mInst++;
    s->mCurrent[m->BankMonitor.mInst] = m->BankMonitor.mDefault;
    m->BankMonitor.mPosition[m->BankMonitor.mInst] = 0;
    s->mPosStatic[m->BankMonitor.mInst] = 0;
    s->mBaseVol[m->BankMonitor.mInst] = s->mCurrent[m->BankMonitor.mInst].vol;
    s->mVolControl[m->BankMonitor.mInst] = 0;
    // a null member-function pointer: g++ 2.95 clears the 8 bytes with memset(&mCallback[mInst], 0, 8)
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

// vol/target/handle set-up shared by the play paths (SFX_Play2DVol tail)
static inline void voiceTail_4C38(cWorldTriggerInstance* t)
{
    optsVol_4C38(cAudioInterface_bxAudio()->MusicSys.mpVStreamSys->mpVBankManager, (int)(t->m_fVolume * 127.0f));
    cStreamSys* pp = cAudioInterface_bxAudio()->MusicSys.mpVStreamSys;
    float* r = cSSXAudio_GetCurrAppliedVolumePtr(cAudioInterface_bxAudio(), 5, 0);
    pp->mpVBankManager->BankMonitor.mVolControl[pp->mpVBankManager->BankMonitor.mInst] = r;
    if (t->m_bPositional != 0) {
        cStreamSys* p1 = cAudioInterface_bxAudio()->MusicSys.mpVStreamSys;
        p1->mpVBankManager->BankMonitor.mPosition[p1->mpVBankManager->BankMonitor.mInst] = &t->m_vPosition;
        cStreamSys* p2 = cAudioInterface_bxAudio()->MusicSys.mpVStreamSys;
        p2->mpVBankManager->BankMonitor.mExternalVol[p2->mpVBankManager->BankMonitor.mInst] = 1;
        cStreamSys* p3 = cAudioInterface_bxAudio()->MusicSys.mpVStreamSys;
        p3->mpVBankManager->BankMonitor.mCloseOnEnd[p3->mpVBankManager->BankMonitor.mInst] = 0;
    }
    cStreamSys* p4 = cAudioInterface_bxAudio()->MusicSys.mpVStreamSys;
    p4->mpVBankManager->BankMonitor.mViewport[p4->mpVBankManager->BankMonitor.mInst] = t->m_iViewPort;
}

/**
 * Per-frame update of a touched world trigger instance: starts its sound once, then keeps its volume current.
 *
 * Without a sound (+0x30 == kAudioNotStarted): records whether it is a crowd trigger
 * (cWorldTriggerInstance_CrowdTrigger, +0x38); a non-crowd trigger whose object volume is already playing elsewhere
 * (cWorldTriggerManager_PlayingTriggerInstance) is reset (cWorldTriggerInstance_Reset). It resolves the bank and patch
 * (cWorldTriggerInstance_TriggerToPatch, kept in +0x60 / +0x64); a no-audio trigger gets the handle kAudioNone. A swap
 * trigger (kSwapTriggerBank) needs the focus rider's viewport, no other swap trigger playing
 * (cWorldTriggerManager_SwapTriggerPlaying) and its swap string (cWorldTriggerInstance_SwapTriggerToString); its string
 * goes to func_0029B818 and it plays only when that returns 1.
 *
 * The sound plays only when the trigger is not an after-collision trigger (cWorldTriggerManager_AfterCollisionTrigger) or its object was hit
 * (cWorldTriggerManager_ObjectHit); a sync-to-animation trigger (cWorldTriggerManager_SyncToAnimationTrigger) also waits for its object's first frame (cWorldTriggerManager_ObjectRefFirstFrame).
 * Crowd triggers go to the crowd system (cCrowdSys_OpenObjectOpts, cCrowdSys_AddObject); others play through cBankSys_SFX_Begin-style
 * options with volume +0x10 x 127, volume control 5, the position (+0x20) when positional and the viewport, ending in
 * cBankSys_SFX_End.
 *
 * With a sound: a sync-to-animation trigger restarts on each new first frame (stopping the old handle with a 0.1 s
 * fade); otherwise the volume is re-applied (cCrowdSys_SetObjectLoudness for crowd triggers, else the bank monitor instance's volume
 * through cBankMonitor_BANKMONITOR_GetHandle). Called by cWorldTriggerManager_Update for touched instances.
 *
 */
void cWorldTriggerInstance::Update()
{
    int rec[3];
    int swapResult;
    if (m_iAudioHandle == cWorldTriggerInstance::kAudioNotStarted) {
        if (CrowdTrigger(m_eAudioTrigger) != 0)
            m_bCrowdTrigger = 1;
        else
            m_bCrowdTrigger = 0;
        if (m_bCrowdTrigger == 0) {
            if (cWorldTriggerManager_PlayingTriggerInstance((cWorldTriggerInstance**)gWorldTriggerManager,
                    m_iObjectRef, m_iVolNum, this) != 0) {
                Reset();
                return;
            }
        }
        {
            int ok = TriggerToPatch(m_eAudioTrigger, &rec[0], &rec[1], &rec[2], m_iViewPort, 0);
            m_iBankReference = rec[0];
            m_iPatchReference = rec[1];
            if (!ok) {
                Reset();
                return;
            }
        }
        if (rec[2] != 0) {
            m_iAudioHandle = cWorldTriggerInstance::kAudioNone;
            return;
        }
        swapResult = -1;
        if (rec[0] == kSwapTriggerBank) {
            if (cSSXAudio_GetFocusRider(cAudioInterface_bxAudio(), -1)->mViewIndex != m_iViewPort) {
                Reset();
                return;
            }
            swapResult = cWorldTriggerManager_SwapTriggerPlaying((cWorldTriggerInstance**)gWorldTriggerManager,
                    m_eAudioTrigger);
            if (swapResult > 0) {
                Reset();
                return;
            }
            if (strcmp(m_iSwapString, str_empty_j) == 0) {
                if (SwapTriggerToString(m_eAudioTrigger, m_iSwapString) == 0) {
                    Reset();
                    return;
                }
            }
            swapResult = func_0029B818(cAudioInterface_bxAudio(), m_iSwapString, rec);
        }
        if (gWorldTriggerManager->AfterCollisionTrigger(m_eAudioTrigger) == 0
            || gWorldTriggerManager->ObjectHit(m_iObjectRef) != 0) {
            if (rec[0] == kSwapTriggerBank && swapResult != 1)
                return;
            if (gWorldTriggerManager->SyncToAnimationTrigger(m_eAudioTrigger) != 0) {
                if (gWorldTriggerManager->ObjectRefFirstFrame(m_iObjectRef, this) == 0) {
                    m_iAudioHandle = cWorldTriggerInstance::kAudioNone;
                    return;
                }
            }
            if (m_bCrowdTrigger != 0) {
                cCrowdSys_OpenObjectOpts(cAudioInterface_bxAudio());
                voiceTail_4C38(this);
                m_iAudioHandle = cCrowdSys_AddObject(cAudioInterface_bxAudio(), rec[1]);
                return;
            }
            sfxBegin_4C38(cAudioInterface_bxAudio(), rec[0], rec[1]);
            voiceTail_4C38(this);
            m_iAudioHandle = cBankSys_SFX_End(cAudioInterface_bxAudio());
            return;
        }
        Reset();
        return;
    }
    if (gWorldTriggerManager->SyncToAnimationTrigger(m_eAudioTrigger) != 0
        && gWorldTriggerManager->ObjectRefFirstFrame(m_iObjectRef, this) != 0) {
        if (m_iAudioHandle != cWorldTriggerInstance::kAudioNone)
            cBankMonitor_BANKMONITOR_StopHandle(
                    &cAudioInterface_bxAudio()->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                    m_iAudioHandle, 1, 0.1f);
        sfxBegin_4C38(cAudioInterface_bxAudio(), m_iBankReference, m_iPatchReference);
        voiceTail_4C38(this);
        m_iAudioHandle = cBankSys_SFX_End(cAudioInterface_bxAudio());
        return;
    }
    if (m_bCrowdTrigger != 0) {
        if (m_iAudioHandle == cWorldTriggerInstance::kAudioNone)
            return;
        cCrowdSys_SetObjectLoudness(cAudioInterface_bxAudio(), m_iAudioHandle, (int)(m_fVolume * 127.0f));
        return;
    }
    if (m_iAudioHandle == cWorldTriggerInstance::kAudioNone)
        return;
    {
        cBankMonitorInstance* bankInstance = cBankMonitor_BANKMONITOR_GetHandle(
                &cAudioInterface_bxAudio()->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, m_iAudioHandle, 1);
        if (bankInstance == 0)
            return;
        bankInstance->mBaseVol = (int)(m_fVolume * 127.0f);
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance_Stop);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cCrowdSys_RemoveObject(void* snd, int id, float t);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* self, int idx, int a2, float v);
extern unsigned char str_empty_j[];
struct sWorldTrigger70;

/**
 * Stops a world audio trigger instance's sound and resets the instance.
 *
 * If the handle at +0x30 (On Tour m_iAudioHandle) is live (not kAudioNone) it is stopped with a 0.25 s fade: through
 * the crowd system (cCrowdSys_RemoveObject on gSSXAudio, cAudioInterface_bxAudio) when the crowd flag at +0x38 is set, else
 * through cBankMonitor_BANKMONITOR_StopHandle on the bank monitor. Then the handle becomes kAudioNotStarted, the swap
 * name at +0x40 is emptied (On Tour strcpy(name, "")), +0x68 (On Tour m_iLastAnimFrame) is set to -1.0f and +0x60,
 * +0x64 and +0x6C are zeroed.
 *
 */
void cWorldTriggerInstance::Stop()
{
    if (m_bCrowdTrigger != 0) {
        if (m_iAudioHandle != kAudioNone) {
            cCrowdSys_RemoveObject(cAudioInterface_bxAudio(), m_iAudioHandle, 0.25f);
        }
    } else if (m_iAudioHandle != kAudioNone) {
        cBankMonitor_BANKMONITOR_StopHandle(
                &cAudioInterface_bxAudio()->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                m_iAudioHandle, 1, 0.25f);
    }
    m_iAudioHandle = kAudioNotStarted;
    m_iSwapString[0] = str_empty_j[0];
    m_iLastAnimFrame = -1.0f;
    m_iBankReference = 0;
    m_iPatchReference = 0;
    m_bLastAnimFrameDecrease = 0;
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
int cWorldTriggerInstance::TriggerToPatch(int trigger, int* bank, int* patch, int* noAudio, int viewPort, int check)
{
    return gWorldTriggerManager->TriggerToPatch(trigger, bank, patch, noAudio, viewPort, check);
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance_CrowdTrigger);
#ifdef SKIP_ASM
extern "C" int cWorldTriggerManager_TriggerToPatch(cWorldTriggerManager* self, int idx, int* type, int* a, int* b, int c, int d);

/**
 * Returns whether a world trigger plays from the crowd bank (kCrowdTriggerBank from an eTriggerToPatchCheck_Crowd
 * TriggerToPatch lookup).
 *
 * Likely bug: for a trigger type TriggerToPatch does not resolve with a nonzero check, bank is left uninitialised and
 * compared anyway.
 *
 * @param trigger  the trigger number
 * @return true for a crowd trigger, else false
 */
bool cWorldTriggerInstance::CrowdTrigger(int trigger)
{
    int bank;
    int patch;
    int noAudio;
    gWorldTriggerManager->TriggerToPatch(trigger, &bank, &patch, &noAudio, 0, eTriggerToPatchCheck_Crowd);
    return bank == kCrowdTriggerBank;
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
int cWorldTriggerInstance::SwapTriggerToString(int idx, char* dst)
{
    return gWorldTriggerManager->SwapTriggerToString(idx, dst);
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerInstance_Reset);
#ifdef SKIP_ASM
extern unsigned char str_empty_j[];

/**
 * Resets a world trigger instance to unused: no sound (-1), untouched, empty swap string, last frame -1.0.
 *
 * Clears +0x0, +0x34, the bank and patch references (+0x60, +0x64) and +0x6C.
 */
void cWorldTriggerInstance::Reset()
{
    m_bInUse = 0;
    m_iAudioHandle = -1;
    m_bTouch = 0;
    m_iSwapString[0] = str_empty_j[0];
    m_iLastAnimFrame = -1.0f;
    m_iBankReference = 0;
    m_iPatchReference = 0;
    m_bLastAnimFrameDecrease = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", __20cWorldTriggerManager);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* __21cWorldTriggerInstance(cWorldTriggerInstance* self);
extern "C" void cWorldTriggerManager_LoadTriggerInfo(void* self);
extern const char str_m_aWorldTriggerInstances[];

// The 16-byte header g++ 2.95's operator new[] keeps in front of an array of objects with a destructor: the element
// count, then padding (the elements stay 16-byte aligned).
struct tArrayNewCookie {
    int count;
    int pad[3];
};

/**
 * Constructs the world trigger manager: resets its state, builds 40 trigger instances and loads the trigger
 * info.
 *
 * The instances are a new[] array ("m_aWorldTriggerInstances", 0x70 bytes each, behind the array cookie).
 */
cWorldTriggerManager::cWorldTriggerManager()
{
    mNumHitObjects = 0;
    mLastHitObjectResult = 0;
    mLastHitObject = -1;
    mLastHitObjectChecked = -1;
    mMonitorTriggers = 1;
    tArrayNewCookie* cookie = (tArrayNewCookie*)operator_new_tag(4496, str_m_aWorldTriggerInstances, 0, 0);
    cookie->count = 40;
    cWorldTriggerInstance* arr = (cWorldTriggerInstance*)&cookie[1];
    cWorldTriggerInstance* trigger = arr;
    for (int i = 39; i != -1; i--, trigger++) {
        __21cWorldTriggerInstance((cWorldTriggerInstance*)trigger);
    }
    m_aWorldTriggerInstances = arr;
    m_pcTriggerInfoFile = 0;
    cWorldTriggerManager_LoadTriggerInfo(this);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", _$_20cWorldTriggerManager);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);

/**
 * Destroys the world trigger manager: destroys its trigger instances, frees the array, then frees the object
 * if asked.
 */
cWorldTriggerManager::~cWorldTriggerManager()
{
    cWorldTriggerInstance* instances = m_aWorldTriggerInstances;
    if (instances != 0) {
        tArrayNewCookie* cookies = (tArrayNewCookie*)instances;
        cWorldTriggerInstance* entry = instances + cookies[-1].count;
        while (m_aWorldTriggerInstances != entry) {
            entry--;
            _$_21cWorldTriggerInstance(entry, 0);
        }
        tArrayNewCookie* array = (tArrayNewCookie*)m_aWorldTriggerInstances;
        cMemMan_free(&array[-1]);
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

// PORT: the unit declares LoadTriggerInfo as void (the ctor ignores it); the body returns 0/1. Bound by asm label.

/**
 * Loads the sound trigger definitions from data/config/watrig.adl and indexes them by entry.
 *
 * Each entry's type decides its size (eTRIGGERTYPE_Banked, _Random, _Swap and _Advanced are 6, 14, 12 and 4 words); the
 * index ("m_aTriggerInfoInstances") holds type and data pointer for entries 2 onwards. A missing file leaves the
 * manager with no trigger info.
 *
 * @return false when the file's first byte is nonzero (rejected), otherwise true
 */
bool cWorldTriggerManager::LoadTriggerInfo()
{
    if (BXFILE_exists(str_p5H) != 0) {
        char* data = FILE_loadpack(str_p5H, 0);
        tTriggerInfoFileHeader* file = (tTriggerInfoFileHeader*)data;
        m_pcTriggerInfoFile = data;
        if (file->mFormatFlag != 0) {
            return false;
        }
        m_iNumTriggers = file->mNumTriggers + 2;
        m_aTriggerInfo = (tTriggerTypePointer*)operator_new_tag(m_iNumTriggers * 8,
            str_m_aTriggerInfoInstances, 0, 0);
        int* cursor = file->mRecords;
        for (int i = 2; i < m_iNumTriggers; i++) {
            m_aTriggerInfo[i].m_TriggerType = *cursor;
            m_aTriggerInfo[i].m_pTriggerTypeStruct = (tTriggerType*)cursor;
            int trigType = *cursor;
            if (trigType == eTRIGGERTYPE_Banked) {
                cursor += 6;
            } else if (trigType == eTRIGGERTYPE_Random) {
                cursor += 14;
            } else if (trigType == eTRIGGERTYPE_Swap) {
                cursor += 12;
            } else if (trigType == eTRIGGERTYPE_Advanced) {
                cursor += 4;
            }
        }
    } else {
        m_pcTriggerInfoFile = 0;
        m_iNumTriggers = 0;
        m_aTriggerInfo = 0;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_UnloadTriggerInfo);
#ifdef SKIP_ASM
extern "C" void MEM_free(void* p);

/**
 * Frees the loaded trigger info: MEM_free on the file (+0x338), cMemMan_free on the table (+0x33C), count 0.
 *
 * Does nothing when no file is loaded.
 *
 */
void cWorldTriggerManager::UnloadTriggerInfo()
{
    char* file = m_pcTriggerInfoFile;
    if (file != 0) {
        MEM_free(file);
        tTriggerTypePointer* table = m_aTriggerInfo;
        m_pcTriggerInfoFile = 0;
        if (table != 0) {
            cMemMan_free(table);
        }
        m_iNumTriggers = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_GetFreeInstance);
#ifdef SKIP_ASM

/**
 * Returns the first unused world trigger instance of the 40, or 0 when all are in use.
 *
 * @return the free instance, or 0
 */
cWorldTriggerInstance* cWorldTriggerManager::GetFreeInstance()
{
    for (int i = 0; i < 40; i++) {
        if (m_aWorldTriggerInstances[i].m_bInUse == 0) {
            return &m_aWorldTriggerInstances[i];
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
cWorldTriggerInstance* cWorldTriggerManager::GetTriggerInstance(int objectRef, int volNum, int viewPort)
{
    for (int i = 0; i < 40; i++) {
        if (m_aWorldTriggerInstances[i].m_bInUse != 0
            && m_aWorldTriggerInstances[i].SameTriggerInstance(objectRef, volNum, viewPort)) {
            return &m_aWorldTriggerInstances[i];
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_HandleTrigger);
#ifdef SKIP_ASM
// A t4Vector built from its four components (the instance's position).
struct sWtVec4 : t4Vector {
    sWtVec4() {}
    sWtVec4(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

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
void cWorldTriggerManager::HandleTrigger(int objectRef, int volNum, int viewPort, int audioTrigger, int positional,
                                         float volume, const t3Vector& pos)
{
    t3Vector localPos = pos;
    if (mMonitorTriggers != 0) {
        cWorldTriggerInstance* triggerInst = (cWorldTriggerInstance*)GetTriggerInstance(objectRef, volNum, viewPort);
        if (((char*)triggerInst) == 0) {
            sWtVec4 pos4;
            pos4 = sWtVec4(localPos.x, localPos.y, localPos.z, 1.0f);
            triggerInst = (cWorldTriggerInstance*)GetFreeInstance();
            triggerInst->m_bInUse = 1;
            triggerInst->m_iObjectRef = objectRef;
            triggerInst->m_iVolNum = volNum;
            triggerInst->m_iViewPort = viewPort;
            triggerInst->m_eAudioTrigger = audioTrigger;
            triggerInst->m_fVolume = volume;
            triggerInst->m_bPositional = positional;
            triggerInst->m_vPosition = pos4;
        } else {
            triggerInst->m_fVolume = volume;
        }
        triggerInst->m_bTouch = 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_Update);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int strcmp(const void* a, const void* b);
// PORT: cSSXAudio_SwapBankStopped__FPv is an empty stub whose caller passes (mgr, name).
void cSSXAudio_SwapBankStopped_2(void* mgr, void* name) __asm__("cSSXAudio_SwapBankStopped__FPv");
extern "C" void cWorldTriggerInstance_Update(cWorldTriggerInstance* t);
extern "C" void cWorldTriggerInstance_Stop(cWorldTriggerInstance* t);
extern unsigned char str_empty_j[];

// Tells the audio system the instance's swap bank stopped (cSSXAudio_SwapBankStopped with its swap string). An inline
// function's argument keeps retail's index-first address add for &m_aWorldTriggerInstances[i].
static inline void swapBankStopped(cSSXAudio* audio, cWorldTriggerInstance* inst)
{
    cSSXAudio_SwapBankStopped_2(audio, inst->m_iSwapString);
}

/**
 * Per-frame update of the world triggers: stops untouched instances, updates touched ones, clears the touches.
 *
 * Skipped while the sound effects are paused (mSFXPaused). For each in-use instance: when untouched it releases a swap
 * string (cSSXAudio_SwapBankStopped), marks it unused and stops it (cWorldTriggerInstance_Stop); when touched it runs
 * cWorldTriggerInstance_Update. Called every frame from the game (cGame_renderVideo).
 */
void cWorldTriggerManager::Update()
{
    if (cAudioInterface_bxAudio()->mSFXPaused != 0) return;
    for (int i = 0; i < 40; i++) {
        if (m_aWorldTriggerInstances[i].m_bInUse == true) {
            if (m_aWorldTriggerInstances[i].m_bTouch == 0) {
                if (strcmp((&m_aWorldTriggerInstances[i])->m_iSwapString, str_empty_j)) {
                    swapBankStopped(cAudioInterface_bxAudio(), &m_aWorldTriggerInstances[i]);
                }
                m_aWorldTriggerInstances[i].m_bInUse = 0;
                m_aWorldTriggerInstances[i].Stop();
            } else {
                m_aWorldTriggerInstances[i].Update();
            }
            m_aWorldTriggerInstances[i].m_bTouch = 0;
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
 * Walks the 40 instances in the array at this+0; each whose active flag (m_bInUse) is set is marked inactive and
 * stopped with cWorldTriggerInstance_Stop.
 */
void cWorldTriggerManager::StopAllTriggers()
{
    for (int i = 0; i < 40; i++) {
        if (m_aWorldTriggerInstances[i].m_bInUse == true) {
            m_aWorldTriggerInstances[i].m_bInUse = 0;
            m_aWorldTriggerInstances[i].Stop();
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
void cWorldTriggerManager::StopTrigger(int id)
{
    for (int i = 0; i < 40; i++) {
        if (m_aWorldTriggerInstances[i].m_bInUse == true
            && (&m_aWorldTriggerInstances[i])->m_eAudioTrigger == id) {
            m_aWorldTriggerInstances[i].m_bInUse = 0;
            m_aWorldTriggerInstances[i].Stop();
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_HandleCollision);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int cWorldTriggerManager_TriggerToPatch(cWorldTriggerManager* self, int idx, int* type, int* a, int* b, int c, int d);
extern "C" int cWorldTriggerManager_HasAfterCollisionTrigger(cWorldTriggerManager* self, int inst);
struct sTriggerIdCache;
extern "C" void cWorldTriggerManager_AddHitObject(cWorldTriggerManager* self, int id);
float* cSSXAudio_GetCurrAppliedVolumePtr_f(void* self, int a1, int a2) __asm__("cSSXAudio_GetCurrAppliedVolumePtr");
extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* rider);
extern "C" int cBankSys_SFX_End(void* self);
extern "C" void* memset(void* dst, int c, int n);

#define MIN_5F60(a, b) ((a) < (b) ? (a) : (b))
#define MAX_5F60(a, b) ((a) < (b) ? (b) : (a))

/**
 * Plays the sound of a world trigger hit by a rider (collision): returns its handle, or -1.
 *
 * A trigger whose info type is eTRIGGERTYPE_Swap returns -1. When inst differs from the last hit object (+0x328) and
 * has an after-collision trigger (cWorldTriggerManager_HasAfterCollisionTrigger) it is added to the hit objects
 * (cWorldTriggerManager_AddHitObject); inst becomes the last hit object. Resolves the bank and patch
 * (cWorldTriggerManager_TriggerToPatch, viewport 1); with audio it plays them like cBankSys_SFX_Begin / SFX_End with
 * volume vol, volume control 7 and, for a rider, the rider's position (+0x110) and cSSXAudio_GetClosestCamera1. Called
 * by cSSXAudio_CollisionSound.
 *
 * @param inst  the hit object (its instance record, kept as an int as On Tour's Inst), or 0
 * @param id  the trigger number
 * @param rider  the rider, or 0
 * @param vol  the volume (0..127)
 * @return the sound handle, or -1
 */
int cWorldTriggerManager::HandleCollision(int inst, int id, cRider* rider, int vol)
{
    if (m_aTriggerInfo[id].m_TriggerType == eTRIGGERTYPE_Swap)
        return -1;
    if (inst != 0)
    {
        if (inst != mLastHitObject)
        {
            if (HasAfterCollisionTrigger(inst))
                AddHitObject(inst);
        }
        mLastHitObject = inst;
    }
    int type, patch, noAudio;
    if (TriggerToPatch(id, &type, &patch, &noAudio, 1, 0) != 0 && noAudio == 0)
    {
        cSSXAudio* snd = cAudioInterface_bxAudio();
        int tp = type;
        int ar = patch;
        cBankManager* bankManager = snd->mpVBankManager;
        cBankMonitor* playOpts = &bankManager->BankMonitor;
        bankManager->BankMonitor.mInst++;
        playOpts->mCurrent[bankManager->BankMonitor.mInst] = playOpts->mDefault;
        bankManager->BankMonitor.mPosition[bankManager->BankMonitor.mInst] = 0;
        playOpts->mPosStatic[bankManager->BankMonitor.mInst] = 0;
        playOpts->mBaseVol[bankManager->BankMonitor.mInst] = playOpts->mCurrent[bankManager->BankMonitor.mInst].vol;
        playOpts->mVolControl[bankManager->BankMonitor.mInst] = 0;
        // a null member-function pointer: g++ 2.95 clears the 8 bytes with memset(&mCallback[mInst], 0, 8)
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
        snd->mBank[snd->mpVBankManager->BankMonitor.mInst] = tp;
        snd->mPatch[snd->mpVBankManager->BankMonitor.mInst] = ar;
        {
            cBankManager* volManager = cAudioInterface_bxAudio()->MusicSys.mpVStreamSys->mpVBankManager;
            cBankMonitor* s2 = &volManager->BankMonitor;
            s2->mBaseVol[volManager->BankMonitor.mInst] = vol;
            s2->mCurrent[volManager->BankMonitor.mInst].vol = MAX_5F60(MIN_5F60(s2->mBaseVol[s2->mInst], 0x7F), 0);
        }
        {
            cStreamSys* pt = cAudioInterface_bxAudio()->MusicSys.mpVStreamSys;
            float* h = cSSXAudio_GetCurrAppliedVolumePtr_f(cAudioInterface_bxAudio(), 7, 0);
            pt->mpVBankManager->BankMonitor.mVolControl[pt->mpVBankManager->BankMonitor.mInst] = h;
        }
        if (rider != 0)
        {
            cBankManager* riderManager = cAudioInterface_bxAudio()->MusicSys.mpVStreamSys->mpVBankManager;
            riderManager->BankMonitor.mPosition[riderManager->BankMonitor.mInst] = &rider->mPosition;
            cStreamSys* pt = cAudioInterface_bxAudio()->MusicSys.mpVStreamSys;
            int h = cSSXAudio_GetClosestCamera1(cAudioInterface_bxAudio(), rider);
            pt->mpVBankManager->BankMonitor.mViewport[pt->mpVBankManager->BankMonitor.mInst] = h;
            cBankManager* flagManager = cAudioInterface_bxAudio()->MusicSys.mpVStreamSys->mpVBankManager;
            flagManager->BankMonitor.mPosStatic[flagManager->BankMonitor.mInst] = 1;
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
extern "C" int cSoundSys_SndRand(cSoundSys* soundSys);
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
 * entry in the table at +0x33C (type, then type data; On Tour tTriggerTypePointer) decides: eTRIGGERTYPE_Banked gives
 * the bank (+0x10) and patch (+0x14) of its data; eTRIGGERTYPE_Random gives the bank (+0x10) and one of four patches
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
int cWorldTriggerManager::TriggerToPatch(int trigger, int* bank, int* patch, int* noAudio, int viewPort, int check)
{
    *noAudio = 0;
    if (trigger >= m_iNumTriggers) {
        *bank = 0;
        *patch = 0;
        return 0;
    }
    int result = 1;
    int triggerType = ((sWTMEntry*)m_aTriggerInfo)[trigger].type;
    if (triggerType == eTRIGGERTYPE_Swap) {
        *bank = triggerType;
        *patch = 0;
    } else if (triggerType == eTRIGGERTYPE_Banked) {
        *bank = ((sWTMEntry*)m_aTriggerInfo)[trigger].obj->f10;
        *patch = ((sWTMEntry*)m_aTriggerInfo)[trigger].obj->f14;
    } else if (triggerType == eTRIGGERTYPE_Random) {
        *bank = ((sWTMEntry*)m_aTriggerInfo)[trigger].obj->f10;
        cSSXAudio* audio = cAudioInterface_bxAudio();
        int rnd = cSoundSys_SndRand(audio->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor.mpVSoundSys);
        sWTMObj* randomData = ((sWTMEntry*)m_aTriggerInfo)[trigger].obj;
        int pick = (unsigned)((rnd & 0x7FFF) * randomData->f34) / 0x7FFF;
        if (pick < randomData->f24) *patch = randomData->f14;
        else if (pick < randomData->f28 + randomData->f24) *patch = randomData->f18;
        else if (pick < randomData->f2C + randomData->f28 + randomData->f24) *patch = randomData->f1C;
        else *patch = randomData->f20;
    } else if (check == eTriggerToPatchCheck_General) {
        result = AdvancedTriggerToPatch(trigger, bank, patch, noAudio, viewPort);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_AdvancedTriggerToPatch);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int cSSXAudio_SpeakPAPlace(void* mgr, int a1, int a2, int a3);

/**
 * Resolves an advanced world audio trigger: only kAUDIO_TRIGGER_PA_Trigger (77) is known, and it plays through the
 * audio system itself.
 *
 * Clears *bank, *patch and *noAudio. For kAUDIO_TRIGGER_PA_Trigger it sets *noAudio to 1 and returns
 * cSSXAudio_SpeakPAPlace(gSSXAudio, no rider, 1, @p viewPort); any other trigger returns 0.
 *
 * @param trigger   the trigger number
 * @param bank      out: the sound bank (always 0)
 * @param patch     out: the patch in the bank (always 0)
 * @param noAudio   out: 1 when the trigger plays no bank sound of its own (On Tour noaudio)
 * @param viewPort  the viewport the trigger fired in
 * @return 0, or cSSXAudio_SpeakPAPlace's result for kAUDIO_TRIGGER_PA_Trigger
 */
int cWorldTriggerManager::AdvancedTriggerToPatch(int trigger, int* bank, int* patch, int* noAudio, int viewPort)
{
    int found = 0;
    *bank = 0;
    *patch = 0;
    *noAudio = 0;
    if (trigger == kAUDIO_TRIGGER_PA_Trigger) {
        *noAudio = 1;
        found = cSSXAudio_SpeakPAPlace(cAudioInterface_bxAudio(), 0, 1, viewPort);
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
 * Copies a swap trigger's string: an eTRIGGERTYPE_Swap trigger's text (+0x10 of its data); eTRIGGERTYPE_Advanced goes
 * to cWorldTriggerManager_AdvancedTriggerToString.
 *
 * Returns 0 for a trigger number at or past the count (+0x340) and for other types.
 *
 * @param idx  the trigger number
 * @param out  receives the string
 * @return 1 when copied, else 0 (or cWorldTriggerManager_AdvancedTriggerToString's 0)
 */
int cWorldTriggerManager::SwapTriggerToString(int idx, char* out)
{
    if (idx < m_iNumTriggers) {
        sWorldTriggerInfo8* list = (sWorldTriggerInfo8*)m_aTriggerInfo;
        if (list[idx].type == eTRIGGERTYPE_Swap) {
            strcpy(out, list[idx].data + 0x10);
            return 1;
        }
        if (list[idx].type == eTRIGGERTYPE_Advanced) {
            return cWorldTriggerManager_AdvancedTriggerToString_3(this, idx, out);
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
int cWorldTriggerManager::AdvancedTriggerToString()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_SwapTriggerPlaying);
#ifdef SKIP_ASM
extern "C" int strcmp(const void* a, const void* b);
extern unsigned char str_empty_j[];

/**
 * Counts the in-use instances that hold a swap string and play a trigger other than id.
 *
 * cWorldTriggerInstance_Update does not start a swap trigger while this is above 0.
 *
 * @param id  the trigger to leave out
 * @return the count
 */
extern "C" int cWorldTriggerManager_SwapTriggerPlaying(cWorldTriggerInstance** self, int id)
{
    int n = 0;
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].m_bInUse == true && strcmp((&(*self)[i])->m_iSwapString, str_empty_j)) {
            if ((&(*self)[i])->m_eAudioTrigger != id) n++;
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
 * @return true when one exists, else false
 */
extern "C" bool cWorldTriggerManager_PlayingTriggerInstance(cWorldTriggerInstance** self, int objectRef, int volNum,
                                                            cWorldTriggerInstance* except)
{
    for (int i = 0; i < 40; i++) {
        cWorldTriggerInstance* triggerInst = &(*self)[i];
        if (triggerInst->m_bInUse == true && triggerInst != except
            && triggerInst->m_iObjectRef == objectRef
            && triggerInst->m_iVolNum == volNum) {
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_HasAfterCollisionTrigger);
#ifdef SKIP_ASM
extern "C" cGame* cAudioInterface_getGame(void);
extern "C" void cWorldSound_GetAudioVolumeTriggers(cWorldSound* self, tInstanceRecord* obj, int* o0, int* o1, int* o2,
    int* o3);
extern "C" int cWorldTriggerManager_AfterCollisionTrigger(cWorldTriggerManager* self, unsigned int i);

/**
 * Returns whether any of an object's four audio volumes uses an after-collision trigger (cWorldTriggerManager_AfterCollisionTrigger).
 *
 * The trigger numbers come from the game's cWorldSound (cWorldSound_GetAudioVolumeTriggers). Called by cWorldTriggerManager_HandleCollision.
 *
 * @param inst  the instance record, as an int (On Tour pInst)
 * @return true when one of them is
 */
bool cWorldTriggerManager::HasAfterCollisionTrigger(int inst)
{
    // ORIGINAL-PTR: On Tour's HasAfterCollisionTrigger(int pInst) passes the int object handle on to
    // cWorldSound::GetAudioVolumeTriggers(tInstanceRecord* pInst, ...).
    int ids[4];
    cAudioInterface_getGame()->WorldSound->GetAudioVolumeTriggers((tInstanceRecord*)inst,
        &ids[0], &ids[1], &ids[2], &ids[3]);
    if (AfterCollisionTrigger(ids[0])) {
        return true;
    }
    if (AfterCollisionTrigger(ids[1])) {
        return true;
    }
    if (AfterCollisionTrigger(ids[2])) {
        return true;
    }
    return AfterCollisionTrigger(ids[3]) != 0;
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
 * Returns whether trigger i is an after-collision trigger: its m_HitObjectTrigger is set (never for i < 2).
 *
 * @param i  the trigger number
 * @return true for an after-collision trigger
 */
bool cWorldTriggerManager::AfterCollisionTrigger(unsigned int i)
{
    if (i < 2) {
        return false;
    }
    tTriggerTypePointer* refs = m_aTriggerInfo;
    return refs[i].m_pTriggerTypeStruct->m_HitObjectTrigger == true;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_ObjectHit);
#ifdef SKIP_ASM

/**
 * Returns whether an object is in the hit-object list (+0x4, count +0x324), caching the last answer.
 *
 * The last asked object and result are kept in +0x32C / +0x330.
 *
 * @param id  the object
 * @return 1 when it was hit, else 0
 */
int cWorldTriggerManager::ObjectHit(int id)
{
    if (id == mLastHitObjectChecked) {
        return mLastHitObjectResult;
    }
    int i = mNumHitObjects - 1;
    mLastHitObjectChecked = id;
    for (; i >= 0; i--) {
        if (mHitObjects[i] == id) {
            mLastHitObjectResult = 1;
            return 1;
        }
    }
    mLastHitObjectResult = 0;
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
void cWorldTriggerManager::AddHitObject(int id)
{
    if (ObjectHit(id) == 0) {
        if (mNumHitObjects < 200) {
            mHitObjects[mNumHitObjects] = id;
            mNumHitObjects++;
        }
        mLastHitObjectChecked = -1;
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
void cWorldTriggerManager::Reset()
{
    StopAllTriggers();
    ResetStates();
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
void cWorldTriggerManager::ResetStates()
{
    mLastHitObjectResult = 0;
    mNumHitObjects = 0;
    mLastHitObject = -1;
    mLastHitObjectChecked = -1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_SwapTrigger);
#ifdef SKIP_ASM
extern "C" int cWorldTriggerManager_TriggerToPatch(cWorldTriggerManager* self, int idx, int* type, int* a, int* b, int c, int d);

/**
 * Returns whether a world trigger is a swap trigger (kSwapTriggerBank from an eTriggerToPatchCheck_Swap TriggerToPatch
 * lookup).
 *
 * As in cWorldTriggerInstance_CrowdTrigger, an unresolved advanced trigger leaves bank uninitialised. No caller found.
 *
 * @param trigger  the trigger number
 * @return true for a swap trigger
 */
bool cWorldTriggerManager::SwapTrigger(int trigger)
{
    int bank;
    int patch;
    int noAudio;
    TriggerToPatch(trigger, &bank, &patch, &noAudio, 0, eTriggerToPatchCheck_Swap);
    return bank == kSwapTriggerBank;
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
void cWorldTriggerManager::MonitorTriggers(int enable, int delay)
{
    if (enable == 0 || delay == 0) {
        mMonitorTriggers = enable;
        return;
    }
    cSSXAudio_DelayMonitorTriggers(cAudioInterface_bxAudio());
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_SyncToAnimationTrigger);
#ifdef SKIP_ASM
/**
 * Returns whether trigger i syncs to its object's animation: its m_SyncToAnimation is set (never for i < 2).
 *
 * @param i  the trigger number
 * @return true for a sync-to-animation trigger
 */
bool cWorldTriggerManager::SyncToAnimationTrigger(unsigned int i)
{
    if (i < 2) {
        return false;
    }
    tTriggerTypePointer* refs = m_aTriggerInfo;
    return refs[i].m_pTriggerTypeStruct->m_SyncToAnimation == true;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldTriggerManager_ObjectRefFirstFrame);
#ifdef SKIP_ASM
/**
 * Returns whether an object's animation just restarted: the frame (virtual slot 30) went down and back up.
 *
 * Reads the frame of the record's active node (m_pActiveNode->getAnimFrame()), compares it with the instance's
 * m_iLastAnimFrame, returns 1 when the previous update saw a decrease (m_bLastAnimFrameDecrease) and this one does not,
 * then stores both. Returns 0 when the record has no active node.
 *
 * @param objectRef  the object reference (its instance record, as an int handle)
 * @param state  the world trigger instance holding the last frame
 * @return 1 on the first frame, else 0
 */
int cWorldTriggerManager::ObjectRefFirstFrame(int objectRef, cWorldTriggerInstance* state)
{
    // ORIGINAL-PTR: On Tour's ObjectRefFirstFrame(int ObjectRef, cWorldTriggerInstance* Inst) takes the object as the
    // int handle cWorldTriggerInstance::m_iObjectRef holds (On Tour GetObjectRef() returns int).
    cInstanceNode* obj = ((tInstanceRecord*)objectRef)->m_pActiveNode;
    if (obj == 0) {
        return 0;
    }
    float prev = state->m_iLastAnimFrame;
    float cur = obj->getAnimFrame();
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
 * Returns whether trigger idx ducks ambient audio: its m_DuckAmbientAudio is set (never for idx < 2).
 *
 * No caller found.
 *
 * @param idx  the trigger number
 * @return true when the trigger ducks ambient audio
 */
bool cWorldTriggerManager::DuckAmbientAudioTrigger(unsigned int idx)
{
    if (idx < 2U) {
        return false;
    }
    return (m_aTriggerInfo[idx].m_pTriggerTypeStruct)->m_DuckAmbientAudio == true;
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
extern "C" void cWorldTriggerManager_TouchAllUsed(cWorldTriggerInstance** self)
{
    for (int i = 0; i < 40; i++) {
        if ((*self)[i].m_bInUse == true) {
            (*self)[i].m_bTouch = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", __11cWorldSound);
#ifdef SKIP_ASM

extern t4Vector t4vect_Zero;



/**
 * Constructs a cWorldSound: empties its two viewport overlap lists (count 0, last position zero).
 *
 * The game load allocates it (cMemMan_alloc(592, "cWorldSound")) and passes 0 as a second argument the C declaration
 * does not take.
 */
cWorldSound::cWorldSound()
{
    cAudioOverlapList* overlapList = ((&m_OverlapLists[0]));
    for (int i = 1; i != -1; i--, overlapList++) {
        overlapList->numoverlaps = 0;
        overlapList->lastposition = t4vect_Zero;
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", _$_11cWorldSound);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys a cWorldSound: nothing to release; deletes the object when bit 0 of flags is set.
 *
 * The game unload calls it with 3.
 */
cWorldSound::~cWorldSound()
{
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
int cWorldSound::InitWorldAudioFile(int icePickData)
{
    if (icePickData != 0) {
        func_002B6B50(this);
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
void cWorldSound::PurgeWorldAudioFile(int icePickData)
{
    if (icePickData != 0) {
        cWorldSound_DesolveAudioFile(this);
    }
}
#endif

INCLUDE_ASM("sound/icepick/worldtriggermanager", func_002B6B50);

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_DesolveAudioFile);
#ifdef SKIP_ASM
struct sWtmEntList {
    char pad[0xC];
    int count;
    tInstanceAudio entries[1];
};
extern "C" void cWorldSound_RemoveInstanceFromGrid(cWorldSound* self, tInstanceAudio* e);

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
    tInstanceAudio* entries = list->entries;
    for (int i = 0; i < n; i++) {
        tInstanceAudio* entry = &entries[i];
        if (entry->mpInstance != 0) {
            ((cWorldSound*)self)->RemoveInstanceFromGrid(entry);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_CreateAudioGrid);
#ifdef SKIP_ASM
extern "C" void cWorldSound_AddInstanceToGrid(cWorldSound* self, tInstanceAudio* e);

/**
 * Adds every active audio instance in a list to the audio grid (cWorldSound_AddInstanceToGrid); returns 1.
 *
 * @param icePickData  the world audio file data (not used)
 * @param entries  the 0x18-byte audio instances
 * @param n  their count
 * @return 1
 */
int cWorldSound::CreateAudioGrid(int icePickData, tInstanceAudio* entries, int n)
{
    for (int i = 0; i < n; i++) {
        if (entries[i].mpInstance != 0) {
            AddInstanceToGrid(&entries[i]);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_CalcInstAudioBoundingBox);
#ifdef SKIP_ASM
// The 3-float vector these functions build by value: t3Vector plus its constructors.
struct tWtVector3 : t3Vector {
    tWtVector3() {}
    tWtVector3(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};

extern "C" void cWorldSound_CalcAudioVolumeBox(cWorldSound* self, tAudioVolume* volume, tWtVector3* lo, tWtVector3* hi);

/**
 * Computes an audio instance's world bounding box from its audio volumes (cWorldSound_CalcAudioVolumeBox) and its object position.
 *
 * Starts from +-10000, merges each volume record's box (the records follow the collision triggers after the
 * tAudioData header; 0x1C, 0x30, 0x30 or 0x18 bytes by type) and offsets both corners by the instance's position
 * (mpInstance's transform).
 *
 * @param entry  the audio instance
 * @param lo  receives the minimum corner
 * @param hi  receives the maximum corner
 */
void cWorldSound::CalcInstAudioBoundingBox(tInstanceAudio* entry, t3Vector* lo, t3Vector* hi)
{
    *lo = tWtVector3(10000.0f, 10000.0f, 10000.0f);
    *hi = tWtVector3(-10000.0f, -10000.0f, -10000.0f);
    tAudioData* info = entry->mpAudioData;
    char* base = (char*)&info[1]; // the collision triggers follow the header
    char* cursor = base + info->m_iNumCollisionTriggers * 4;
    for (int i = 0; i < info->m_iNumVolumes; i++) {
        tWtVector3 recMin;
        tWtVector3 recMax;
        cWorldSound_CalcAudioVolumeBox(this, (tAudioVolume*)cursor, &recMin, &recMax);
        if (recMin.x < lo->x) lo->x = recMin.x;
        if (recMin.y < lo->y) lo->y = recMin.y;
        if (recMin.z < lo->z) lo->z = recMin.z;
        if (recMax.x > hi->x) hi->x = recMax.x;
        if (recMax.y > hi->y) hi->y = recMax.y;
        if (recMax.z > hi->z) hi->z = recMax.z;
        switch (*(int*)cursor) {
        case kAUDIO_VOLUME_SPHERE:
            cursor += 0x1C;
            break;
        case kAUDIO_VOLUME_ELLIPSE:
            cursor += 0x30;
            break;
        case kAUDIO_VOLUME_CONE:
            cursor += 0x30;
            break;
        case kAUDIO_VOLUME_AMBIENT:
            cursor += 0x18;
            break;
        }
    }
    tInstanceRecord* obj = entry->mpInstance;
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

static inline tWtVector3 operator*(const tWtVector3& v, float s)
{
    return tWtVector3(v.x * s, v.y * s, v.z * s);
}

static inline tWtVector3 operator+(const tWtVector3& a, const tWtVector3& b)
{
    return tWtVector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline void operator-=(tWtVector3& a, const tWtVector3& b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
}

static inline void operator+=(tWtVector3& a, const tWtVector3& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
}

static inline tWtVector3 Cross_2B6F40(const tWtVector3& a, const tWtVector3& b)
{
    return tWtVector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

/**
 * Computes the local bounding box of one audio volume record by its type.
 *
 * Spheres, cones and ambient volumes (kAUDIO_VOLUME_SPHERE, _CONE and _AMBIENT) use centre +- radius; an ellipse
 * (kAUDIO_VOLUME_ELLIPSE) builds an orthonormal basis from its axis (m_f3dir) and the z axis and extends the centre by
 * its three half-extents along it.
 *
 * @param volume  the volume record (tAudioVolumeSphere / Ellipse / Cone / Ambient by m_eType)
 * @param lo  receives the minimum corner
 * @param hi  receives the maximum corner
 */
extern "C" void cWorldSound_CalcAudioVolumeBox(cWorldSound* self, tAudioVolume* volume, tWtVector3* lo, tWtVector3* hi)
{
    switch (volume->m_eType)
    {
    case kAUDIO_VOLUME_SPHERE:
    {
        tAudioVolumeSphere* sphere = (tAudioVolumeSphere*)volume;
        lo->x = sphere->m_f3pos[0] - sphere->m_fRadius;
        lo->y = sphere->m_f3pos[1] - sphere->m_fRadius;
        lo->z = sphere->m_f3pos[2] - sphere->m_fRadius;
        hi->x = sphere->m_f3pos[0] + sphere->m_fRadius;
        hi->y = sphere->m_f3pos[1] + sphere->m_fRadius;
        hi->z = sphere->m_f3pos[2] + sphere->m_fRadius;
        break;
    }
    case kAUDIO_VOLUME_ELLIPSE:
    {
        tAudioVolumeEllipse* ellipse = (tAudioVolumeEllipse*)volume;
        tWtVector3 axisDir;
        tWtVector3 sideDir;
        tWtVector3 upDir;
        tWtVector3 z;
        axisDir = tWtVector3(ellipse->m_f3dir[0], ellipse->m_f3dir[1], ellipse->m_f3dir[2]);
        z = tWtVector3(0.0f, 0.0f, 1.0f);
        sideDir = Cross_2B6F40(z, axisDir);
        sideDir = sideDir * (1.0f / wtSqrt_2B6F40(sideDir.x * sideDir.x + sideDir.y * sideDir.y + sideDir.z * sideDir.z));
        upDir = Cross_2B6F40(axisDir, sideDir);
        *lo = tWtVector3(ellipse->m_f3pos[0], ellipse->m_f3pos[1], ellipse->m_f3pos[2]);
        *hi = tWtVector3(ellipse->m_f3pos[0], ellipse->m_f3pos[1], ellipse->m_f3pos[2]);
        *lo -= axisDir * ellipse->m_f3radius[0] + sideDir * ellipse->m_f3radius[1] + upDir * ellipse->m_f3radius[2];
        *hi += axisDir * ellipse->m_f3radius[0] + sideDir * ellipse->m_f3radius[1] + upDir * ellipse->m_f3radius[2];
        break;
    }
    case kAUDIO_VOLUME_CONE:
    {
        tAudioVolumeCone* cone = (tAudioVolumeCone*)volume;
        lo->x = cone->m_f3pos[0] - cone->m_fRadius;
        lo->y = cone->m_f3pos[1] - cone->m_fRadius;
        lo->z = cone->m_f3pos[2] - cone->m_fRadius;
        hi->x = cone->m_f3pos[0] + cone->m_fRadius;
        hi->y = cone->m_f3pos[1] + cone->m_fRadius;
        hi->z = cone->m_f3pos[2] + cone->m_fRadius;
        break;
    }
    case kAUDIO_VOLUME_AMBIENT:
    {
        tAudioVolumeAmbient* ambient = (tAudioVolumeAmbient*)volume;
        lo->x = ambient->m_f3pos[0] - ambient->m_fRadius;
        lo->y = ambient->m_f3pos[1] - ambient->m_fRadius;
        lo->z = ambient->m_f3pos[2] - ambient->m_fRadius;
        hi->x = ambient->m_f3pos[0] + ambient->m_fRadius;
        hi->y = ambient->m_f3pos[1] + ambient->m_fRadius;
        hi->z = ambient->m_f3pos[2] + ambient->m_fRadius;
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_AddInstanceToGrid);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sWtCell_7318 {
    int level;
    int x, y, z;
};
struct sWtRoot_7318 {
    sWtCell_7318 cell;
    void* node;
};

extern "C" void nash_Nash_findkey(void* out, const void* box);
extern "C" void nash_Octant_addnode(void* root, int idx, void* item, const void* target);

static inline void insert_7318(char* w, void* item, const tBoundBox* box)
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
 * Inserts an audio instance into the world octree (the game's BXNash) by its bounding box, tagging its space node as
 * type 5 (kSNT_AudioVolume).
 *
 * Skips null, inactive or volume-less instances; the box comes from cWorldSound_CalcInstAudioBoundingBox and the cell from nash_Nash_findkey.
 *
 * @param entry  the audio instance
 */
void cWorldSound::AddInstanceToGrid(tInstanceAudio* entry)
{
    tInstanceAudio* trig = entry;
    if (trig == 0 || trig->mpInstance == 0 || trig->mpAudioData->m_iNumVolumes == 0) {
        return;
    }
    tWtVector3 min;
    tWtVector3 max;
    CalcInstAudioBoundingBox(trig, &min, &max);
    char* w = (char*)cAudioInterface_getGame()->BXNash;
    trig->mSpaceNode.type = 5; // kSNT_AudioVolume
    float one = 1.0f;
    tBoundBox box;
    box.MinPoint.x = min.x;
    box.MinPoint.y = min.y;
    box.MinPoint.z = min.z;
    box.MinPoint.w = one;
    box.MaxPoint.x = max.x;
    box.MaxPoint.y = max.y;
    box.MaxPoint.z = max.z;
    box.MaxPoint.w = one;
    insert_7318(w, trig, &box);
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_RemoveInstanceFromGrid);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void nash_Cell_remnode(nash_Cell* cell, int kind, tInstanceAudio* item, nash_CellKey* target,
                                  nash_CellKey* cur);
extern "C" void cWorldSound_RemoveInstanceFromOverlapList(cWorldSound* self, tInstanceAudio* pInst);
void operator_delete(int* p);

static inline void GetCell_2B7410(nash_CellKey* key, tBoundBox* box)
{
    nash_Nash_findkey(key, box);
}
static inline void Remove_2B7410(nash_Octant* octant, tInstanceAudio* e, nash_CellKey* key)
{
    nash_Cell_remnode(octant->rootcell, 2, e, key, &octant->rootkey);
}
// The octant on one axis of the key: the first for a coordinate >= 0, else the one @p step octants on.
static inline nash_Octant* Child_2B7410(nash_Octant* octant, int coord, int step)
{
    return coord > -1 ? octant : octant + step;
}

/**
 * Removes an audio instance from the world octree and from both viewport overlap lists (cWorldSound_RemoveInstanceFromOverlapList).
 *
 * Skips null, inactive or volume-less instances; frees a cell's item list when it becomes empty.
 *
 * @param instAudio  the audio instance
 */
void cWorldSound::RemoveInstanceFromGrid(tInstanceAudio* instAudio)
{
    if (instAudio == 0) return;
    if (instAudio->mpInstance == 0) return;
    if (instAudio->mpAudioData->m_iNumVolumes == 0) return;
    tWtVector3 lo;
    tWtVector3 hi;
    CalcInstAudioBoundingBox(instAudio, &lo, &hi);
    cBXNash* base = cAudioInterface_getGame()->BXNash;
    tBoundBox box;
    box.MinPoint.x = lo.x;
    box.MinPoint.y = lo.y;
    box.MinPoint.z = lo.z;
    box.MinPoint.w = 1.0f;
    box.MaxPoint.x = hi.x;
    box.MaxPoint.y = hi.y;
    box.MaxPoint.z = hi.z;
    box.MaxPoint.w = 1.0f;
    base->deletecount++;
    nash_CellKey cell;
    GetCell_2B7410(&cell, &box);
    nash_Octant* octant = Child_2B7410(Child_2B7410(Child_2B7410(&base->octant[0][0][0], cell.x, 4), cell.y, 2), cell.z,
            1);
    Remove_2B7410(octant, instAudio, &cell);
    if (octant->rootcell->empty()) {
        operator_delete((int*)octant->rootcell);
        octant->rootcell = 0;
    }
    RemoveInstanceFromOverlapList(instAudio);
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cAudioOverlapList_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// The 4-float vector these functions pass and build by value: t4Vector plus the two constructors (as in peak3project's
// fork).
struct tWtVector4 : t4Vector {
    tWtVector4() {}
    tWtVector4(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};




extern "C" cGame* cAudioInterface_getGame(void);
extern "C" void wtq_75D0(nash_privy_QueryHelper* h, void* partial, void* inside, void* pos, void* node, void* key) __asm__("nash_privy_QueryHelper_help1");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline tWtVector4 Sub_75D0(const t4Vector& a, const t4Vector& b)
{
    tWtVector4 r;
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
static inline float Length_75D0(const t4Vector& v)
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
 * Clears the count and queries the eight octree roots (game +0x20) around pos with nash_privy_QueryHelper_help1, then stores pos as
 * the last position. Called by cWorldSound_TriggerWorldAudio.
 *
 * @param pos  the listener position
 */
void cAudioOverlapList::update(const t4Vector* pos)
{
    if (lastposition.w == 0.0f || Length_75D0(Sub_75D0(*pos, lastposition)) > 10.0f) {
        numoverlaps = 0;
        nash_Octant* root = (nash_Octant*)cAudioInterface_getGame()->BXNash;
        cAudioOverlapList* selfList[1];
        selfList[0] = (this);
        const t4Vector* posPtr = pos;
        cAudioOverlapList** pl = selfList;
        const t4Vector** pp = &posPtr;
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
        lastposition = *pos;
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_RemoveInstanceFromOverlapList);
#ifdef SKIP_ASM
// On Tour cAudioOverlapList::getoverlap(int i).
static inline tInstanceAudio* getOverlap(cAudioOverlapList* list, int j)
{
    return list->overlaps[j];
}

static inline void clearOverlap(cAudioOverlapList* list, int j)
{
    list->overlaps[j] = 0;
}

/**
 * Removes an audio instance from both viewport overlap lists by clearing its entries.
 *
 * @param pInst  the audio instance
 */
void cWorldSound::RemoveInstanceFromOverlapList(tInstanceAudio* pInst)
{
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < m_OverlapLists[i].numoverlaps; j++) {
            if (getOverlap(&m_OverlapLists[i], j) == pInst) {
                clearOverlap(&m_OverlapLists[i], j);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_TriggerWorldAudio);
#ifdef SKIP_ASM
extern "C" void cWorldSound_TriggerGridAudioNode(void* self, const t4Vector* pos, int k, tInstanceAudio* pInstA);

/**
 * Fires the world audio triggers around a position for one viewport.
 *
 * Refreshes the viewport's overlap list (slot @p k, 0x120 bytes each) for @p pos with cAudioOverlapList_update, then calls
 * cWorldSound_TriggerGridAudioNode for every non-zero trigger id in it.
 *
 * @param pos  world position to trigger around (On Tour wpos)
 * @param k  viewport index (On Tour ViewPort)
 */
void cWorldSound::TriggerWorldAudio(const t4Vector* pos, int k)
{
    m_OverlapLists[k].update(pos);
    for (int i = 0; i < m_OverlapLists[k].numoverlaps; i++) {
        if (getOverlap(&m_OverlapLists[k], i) != 0) {
            cWorldSound_TriggerGridAudioNode(this, pos, k, getOverlap(&m_OverlapLists[k], i));
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", cWorldSound_TriggerGridAudioNode);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
static inline tWtVector3 operator-(const tWtVector3& a, const tWtVector3& b)
{
    return tWtVector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline void operator*=(tWtVector3& a, float s)
{
    a.x *= s;
    a.y *= s;
    a.z *= s;
}

static inline float Dot_7908(const tWtVector3& a, const tWtVector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Builds the orthonormal frame of a box trigger from its axis.
static inline void Basis_7908(const tAudioVolumeEllipse* s, tWtVector3& a, tWtVector3& b, tWtVector3& c)
{
    tWtVector3 z;
    a = tWtVector3(s->m_f3dir[0], s->m_f3dir[1], s->m_f3dir[2]);
    z = tWtVector3(0.0f, 0.0f, 1.0f);
    b = Cross_2B6F40(z, a);
    b = b * (1.0f / wtSqrt_2B6F40(b.x * b.x + b.y * b.y + b.z * b.z));
    c = Cross_2B6F40(a, b);
}

extern "C" float wtCurve_7908(void* self, float curve, float t) __asm__("cWorldSound_CalculateVolume");
extern "C" void wtSet_7908(void* mgr, void* obj, int i, int c, int id, int flag, float v, const tWtVector3& pos)
    __asm__("cWorldTriggerManager_HandleTrigger");
extern "C" float func_0031C128(float x);

static inline float Acos_7908(float x)
{
    return 1.5707964f - func_0031C128(x);
}

extern "C" void wtEval_7908(void* self, const tWtVector4* listener, int c, tInstanceAudio* e)
    __asm__("cWorldSound_TriggerGridAudioNode");

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
extern "C" void wtEval_7908(void* self, const tWtVector4* listener, int viewPort, tInstanceAudio* instAudio)
{
    tWtVector3 org;
    tAudioVolume* recs[4];
    tWtVector3 pos;
    tWtVector3 toListener;
    int i;
    float dist;
    float volume;

    org = tWtVector3(instAudio->mpInstance->m_fmTransform.m[3][0], instAudio->mpInstance->m_fmTransform.m[3][1],
        instAudio->mpInstance->m_fmTransform.m[3][2]);
    tAudioData* info = instAudio->mpAudioData;
    char* base = (char*)&info[1]; // the collision triggers follow the header
    char* shapeCursor = base + (info->m_iNumCollisionTriggers << 2);
    int n = info->m_iNumVolumes;

    for (int j = 0; j < n; j++) {
        recs[j] = (tAudioVolume*)shapeCursor;
        switch (*(int*)shapeCursor) {
        case kAUDIO_VOLUME_SPHERE:
            shapeCursor += 0x1C;
            break;
        case kAUDIO_VOLUME_ELLIPSE:
            shapeCursor += 0x30;
            break;
        case kAUDIO_VOLUME_CONE:
            shapeCursor += 0x30;
            break;
        case kAUDIO_VOLUME_AMBIENT:
            shapeCursor += 0x18;
            break;
        }
    }

    for (i = 0; i < n; i++) {
        tAudioVolume* shapeRec = recs[i];
        switch (shapeRec->m_eType) {
        case kAUDIO_VOLUME_SPHERE: {
            tAudioVolumeSphere* sphere = (tAudioVolumeSphere*)shapeRec;
            pos = org + tWtVector3(sphere->m_f3pos[0], sphere->m_f3pos[1], sphere->m_f3pos[2]);
            tWtVector4 diff = Sub_75D0(*listener, tWtVector4(pos.x, pos.y, pos.z, 0.0f));
            toListener.x = diff.x;
            toListener.y = diff.y;
            toListener.z = diff.z;
            dist = __builtin_fabsf(wtSqrt_2B6F40(toListener.x * toListener.x + toListener.y * toListener.y + toListener.z * toListener.z));
            if (dist < sphere->m_fRadius) {
                wtSet_7908(gWorldTriggerManager, instAudio->mpInstance, i, viewPort, sphere->m_eTrigger, 1,
                        wtCurve_7908(self, sphere->m_fDecayRate, dist / sphere->m_fRadius), pos);
            }
            break;
        }
        case kAUDIO_VOLUME_ELLIPSE: {
            tAudioVolumeEllipse* ellipse = (tAudioVolumeEllipse*)shapeRec;
            pos = org + tWtVector3(ellipse->m_f3pos[0], ellipse->m_f3pos[1], ellipse->m_f3pos[2]);
            {
                tWtVector4 diff = Sub_75D0(*listener, tWtVector4(pos.x, pos.y, pos.z, 0.0f));
                toListener.x = diff.x;
                toListener.y = diff.y;
                toListener.z = diff.z;
            }
            tWtVector3 axisDir;
            tWtVector3 sideDir;
            tWtVector3 cc;
            Basis_7908(ellipse, axisDir, sideDir, cc);
            float da = Dot_7908(axisDir, toListener);
            float db = Dot_7908(sideDir, toListener);
            float dc = Dot_7908(cc, toListener);
            float ta = ellipse->m_f3radius[0] != 0.0f ? (da * da) / (ellipse->m_f3radius[0] * ellipse->m_f3radius[0])
                    : 0.0f;
            float tb = ellipse->m_f3radius[1] != 0.0f ? (db * db) / (ellipse->m_f3radius[1] * ellipse->m_f3radius[1])
                    : 0.0f;
            float tc = ellipse->m_f3radius[2] != 0.0f ? (dc * dc) / (ellipse->m_f3radius[2] * ellipse->m_f3radius[2])
                    : 0.0f;
            dist = ta + tb + tc;
            if (dist < 1.0f) {
                wtSet_7908(gWorldTriggerManager, instAudio->mpInstance, i, viewPort, ellipse->m_eTrigger, 1,
                        wtCurve_7908(self, ellipse->m_fDecayRate, dist), pos);
            }
            break;
        }
        case kAUDIO_VOLUME_CONE: {
            tAudioVolumeCone* coneRec = (tAudioVolumeCone*)shapeRec;
            pos = org + tWtVector3(coneRec->m_f3pos[0], coneRec->m_f3pos[1], coneRec->m_f3pos[2]);
            tWtVector3 dir = tWtVector3(listener->x, listener->y, listener->z) - pos;
            dist = __builtin_fabsf(wtSqrt_2B6F40(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z));
            if (dist < coneRec->m_fRadius) {
                dir *= 1.0f / dist;
                float dot = Dot_7908(dir, tWtVector3(coneRec->m_f3dir[0], coneRec->m_f3dir[1], coneRec->m_f3dir[2]));
                if (dot > coneRec->m_fcoscone) {
                    volume = wtCurve_7908(self, coneRec->m_fDecayRate, dist / coneRec->m_fRadius);
                    float cone = Acos_7908(coneRec->m_fcoscone);
                    volume *= wtCurve_7908(self, coneRec->m_fDropOff, Acos_7908(dot) / cone);
                    wtSet_7908(gWorldTriggerManager, instAudio->mpInstance, i, viewPort, coneRec->m_eTrigger, 1,
                            volume, pos);
                }
            }
            break;
        }
        case kAUDIO_VOLUME_AMBIENT: {
            tAudioVolumeAmbient* ambient = (tAudioVolumeAmbient*)shapeRec;
            pos = org + tWtVector3(ambient->m_f3pos[0], ambient->m_f3pos[1], ambient->m_f3pos[2]);
            tWtVector4 diff = Sub_75D0(*listener, tWtVector4(pos.x, pos.y, pos.z, 0.0f));
            dist = __builtin_fabsf(Length_75D0(diff));
            if (dist < ambient->m_fRadius) {
                if (dist <= 0.9f) {
                    volume = 1.0f;
                } else {
                    volume = 1.0f - dist;
                    volume *= 10.0f;
                }
                wtSet_7908(gWorldTriggerManager, instAudio->mpInstance, i, viewPort, ambient->m_eTrigger, 0, volume,
                        pos);
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
 * Walks the object's audio data (m_pInstanceAudio->mpAudioData) record by record.
 *
 * @param obj  the instance record
 * @param id0  receives the first trigger
 * @param id1  receives the second
 * @param id2  receives the third
 * @param id3  receives the fourth
 */
void cWorldSound::GetAudioVolumeTriggers(tInstanceRecord* obj, int* id0, int* id1, int* id2, int* id3)
{
    tAudioData* hdr = obj->m_pInstanceAudio->mpAudioData;
    char* base = (char*)&hdr[1]; // the collision triggers follow the header
    char* cursor = base + (hdr->m_iNumCollisionTriggers << 2);
    int n = hdr->m_iNumVolumes;
    int* recs[4];
    int i;
    int j;

    for (i = 0; i < n; i++) {
        recs[i] = (int*)cursor;
        switch (*(int*)cursor) {
        case kAUDIO_VOLUME_SPHERE:
            cursor += 0x1C;
            break;
        case kAUDIO_VOLUME_ELLIPSE:
            cursor += 0x30;
            break;
        case kAUDIO_VOLUME_CONE:
            cursor += 0x30;
            break;
        case kAUDIO_VOLUME_AMBIENT:
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
        case kAUDIO_VOLUME_SPHERE:
            recId = recs[j][1];
            break;
        case kAUDIO_VOLUME_ELLIPSE:
            recId = recs[j][1];
            break;
        case kAUDIO_VOLUME_CONE:
            recId = recs[j][1];
            break;
        case kAUDIO_VOLUME_AMBIENT:
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
INCLUDE_ASM("sound/icepick/worldtriggermanager", nash_privy_QueryHelper_help1);
#ifdef SKIP_ASM
extern "C" void octreeNode_collectType5Items(void* node, void* list);

union sFloatBits83F8 {
    int i;
    float f;
};

// How a position relates to an octree cell, Classify83F8's result (names ours): nash_privy_QueryHelper_help1 sends a cell's items to
// the inside list for kOctInside and to the crossing list for kOctCrossing; Classify83F8 never reports kOctInside.
enum eOctOverlap83F8 { kOctInside = 0, kOctOutside = 1, kOctCrossing = 2 };

static inline int Classify83F8(const t4Vector* v, const nash_CellKey* key)
{
    sFloatBits83F8 u;
    u.i = (key->level + 0x7F) << 23;
    float size = u.f;
    if (v->x < ((float)key->x - 0.2f) * size)
        return kOctOutside;
    if (((float)(key->x + 1) + 0.2f) * size < v->x)
        return kOctOutside;
    if (v->y < ((float)key->y - 0.2f) * size)
        return kOctOutside;
    if (((float)(key->y + 1) + 0.2f) * size < v->y)
        return kOctOutside;
    if (v->z < ((float)key->z - 0.2f) * size)
        return kOctOutside;
    if (((float)(key->z + 1) + 0.2f) * size < v->z)
        return kOctOutside;
    return kOctCrossing;
}

/**
 * Recursively collects the octree's audio items (kSNT_AudioVolume) near a position into the crossing or inside list.
 *
 * A node whose cell (key, grown by 0.2 cells) contains the position adds its kSNT_AudioVolume items to *partial and
 * recurses into its children down to level 11; otherwise nothing. Quirk: the cell test only returns 1 or 2, so the
 * inside branch (all items to *inside via octreeNode_collectType5Items) is never taken. Called by cAudioOverlapList_update for each
 * root.
 *
 * @param partial  the callback (overlap list) for crossing cells (On Tour oncross)
 * @param inside  the callback (overlap list) for cells fully inside (On Tour oninside)
 * @param pos  the point test: the position
 * @param node  the octree cell
 * @param key  its cell key (level, x, y, z)
 */
void nash_privy_QueryHelper::help1(AudioOverlapCallback* partial, AudioOverlapCallback* inside,
                                   PointProximityTest* pos, nash_Cell* node, nash_CellKey* key)
{
    int overlap = Classify83F8(pos->position, key);
    if (overlap == kOctInside)
    {
        for (nash_Node* item = node->nodesinside[2].head; item != 0; item = item->next)
        {
            if (((cTypedSpaceNode*)item)->type == kSNT_AudioVolume)
            {
                cAudioOverlapList* list = inside->overlaplist;
                list->overlaps[list->numoverlaps++] = (tInstanceAudio*)item;
            }
        }
        if (node->children[0][0][0]) octreeNode_collectType5Items(node->children[0][0][0], inside);
        if (node->children[0][0][1]) octreeNode_collectType5Items(node->children[0][0][1], inside);
        if (node->children[0][1][0]) octreeNode_collectType5Items(node->children[0][1][0], inside);
        if (node->children[0][1][1]) octreeNode_collectType5Items(node->children[0][1][1], inside);
        if (node->children[1][0][0]) octreeNode_collectType5Items(node->children[1][0][0], inside);
        if (node->children[1][0][1]) octreeNode_collectType5Items(node->children[1][0][1], inside);
        if (node->children[1][1][0]) octreeNode_collectType5Items(node->children[1][1][0], inside);
        if (node->children[1][1][1]) octreeNode_collectType5Items(node->children[1][1][1], inside);
    }
    else if (overlap == kOctCrossing)
    {
        for (nash_Node* item = node->nodesinside[2].head; item != 0; item = item->next)
        {
            if (((cTypedSpaceNode*)item)->type == kSNT_AudioVolume)
            {
                cAudioOverlapList* list = partial->overlaplist;
                list->overlaps[list->numoverlaps++] = (tInstanceAudio*)item;
            }
        }
        if (key->level == nash_CellKey::kLeafLevel)
            return;
        nash_CellKey k;
        k.level = key->level - 1;
        k.x = key->x * 2;
        k.y = key->y * 2;
        k.z = key->z * 2;
        if (node->children[0][0][0]) help1(partial, inside, pos, node->children[0][0][0], &k);
        k.x++;
        if (node->children[1][0][0]) help1(partial, inside, pos, node->children[1][0][0], &k);
        k.y++;
        if (node->children[1][1][0]) help1(partial, inside, pos, node->children[1][1][0], &k);
        k.x--;
        if (node->children[0][1][0]) help1(partial, inside, pos, node->children[0][1][0], &k);
        k.z++;
        if (node->children[0][1][1]) help1(partial, inside, pos, node->children[0][1][1], &k);
        k.x++;
        if (node->children[1][1][1]) help1(partial, inside, pos, node->children[1][1][1], &k);
        k.y--;
        if (node->children[1][0][1]) help1(partial, inside, pos, node->children[1][0][1], &k);
        k.x--;
        if (node->children[0][0][1]) help1(partial, inside, pos, node->children[0][0][1], &k);
    }
}
#endif

//100%
INCLUDE_ASM("sound/icepick/worldtriggermanager", __sti__all_in_one_sound_cpp);
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

extern sColour_8818 cfm_sav_Render_Halo_SUN_kSunColour_3;
extern sColour_8818 cfm_sav_Render_Halo_SUN_LENS0_kLens0Colour_3;
extern sColour_8818 cfm_sav_Render_Halo_SUN_LENS1_kLens1Colour_3;
extern sColour_8818 cfm_sav_Render_Halo_SUN_LENS2_kLens2Colour_3;
extern sColour_8818 cfm_sav_Render_Halo_SUN_LENS3_kLens3Colour_3;
extern sColour_8818 cfm_sav_Render_Halo_SUN_LENS4_kLens4Colour_3;
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
extern sKey_8818 kWPR_MusicTrigger_2;
extern sKey_8818 kWPR_Mix_2;
extern sKey_8818 kWPR_Ambience_2;
extern sKey_8818 kWPR_Speech_2;
extern sKey_8818 kWPR_Camera_2;
extern sKey_8818 kWPR_Fog_2;
extern sKey_8818 kWPR_LightGlow_2;
extern sKey_8818 kWPR_ScreenTint_2;
extern sKey_8818 kWPR_SkyBox_2;
extern sKey_8818 kWPR_Sun_2;
extern sKey_8818 kWPR_Surface_2;
extern sKey_8818 kWPR_Lighting_2;
extern sKey_8818 kWPR_Weather_2;
extern sKey_8818 kWPR_Danger_2;

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

extern "C" void __sti__all_in_one_sound_cpp_impl(int init, int prio) __asm__("__sti__all_in_one_sound_cpp");

/**
 * Static initializer of the unit: fills the colour tables cfm_sav_Render_Halo_SUN_kSunColour_3.. and the 14 colour keys kWPR_MusicTrigger_2..
 *
 * Runs only for (1, DEFAULT_INIT_PRIORITY): sets the float colour tables, clears D_004A5260 and gives each key
 * kWPR_MusicTrigger_2 + 8k index and id k (0..13). Listed in the global constructor table.
 *
 * @param init  nonzero to initialise
 * @param prio  the initialisation priority (acts on DEFAULT_INIT_PRIORITY, 65535)
 */
extern "C" void __sti__all_in_one_sound_cpp_impl(int init, int prio)
{
    if (prio == DEFAULT_INIT_PRIORITY && init) {
        cfm_sav_Render_Halo_SUN_kSunColour_3.a = 1.0f;
        cfm_sav_Render_Halo_SUN_kSunColour_3.b = 1.0f;
        cfm_sav_Render_Halo_SUN_kSunColour_3.c = 1.0f;
        cfm_sav_Render_Halo_SUN_kSunColour_3.d = 0.5f;
        cfm_sav_Render_Halo_SUN_LENS0_kLens0Colour_3.a = 0.4998680055141449f;
        cfm_sav_Render_Halo_SUN_LENS0_kLens0Colour_3.b = 1.0f;
        cfm_sav_Render_Halo_SUN_LENS0_kLens0Colour_3.c = 1.0f;
        cfm_sav_Render_Halo_SUN_LENS0_kLens0Colour_3.d = 0.0f;
        cfm_sav_Render_Halo_SUN_LENS1_kLens1Colour_3.a = 0.12951700389385223f;
        cfm_sav_Render_Halo_SUN_LENS1_kLens1Colour_3.b = 1.0f;
        cfm_sav_Render_Halo_SUN_LENS1_kLens1Colour_3.c = 1.0f;
        cfm_sav_Render_Halo_SUN_LENS1_kLens1Colour_3.d = 0.0f;
        cfm_sav_Render_Halo_SUN_LENS2_kLens2Colour_3.a = 0.22426900267601013f;
        cfm_sav_Render_Halo_SUN_LENS2_kLens2Colour_3.b = 0.9402310252189636f;
        cfm_sav_Render_Halo_SUN_LENS2_kLens2Colour_3.c = 0.5425059795379639f;
        cfm_sav_Render_Halo_SUN_LENS2_kLens2Colour_3.d = 0.0004030000127386302f;
        cfm_sav_Render_Halo_SUN_LENS3_kLens3Colour_3.a = 0.4652079939842224f;
        cfm_sav_Render_Halo_SUN_LENS3_kLens3Colour_3.b = 0.32170501351356506f;
        cfm_sav_Render_Halo_SUN_LENS3_kLens3Colour_3.c = 0.9943439960479736f;
        cfm_sav_Render_Halo_SUN_LENS3_kLens3Colour_3.d = 0.0f;
        cfm_sav_Render_Halo_SUN_LENS4_kLens4Colour_3.a = 0.17792600393295288f;
        cfm_sav_Render_Halo_SUN_LENS4_kLens4Colour_3.b = 1.0f;
        cfm_sav_Render_Halo_SUN_LENS4_kLens4Colour_3.c = 1.0f;
        cfm_sav_Render_Halo_SUN_LENS4_kLens4Colour_3.d = 0.0f;
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
        Key_8818(&kWPR_MusicTrigger_2, 0);
        Key_8818(&kWPR_Mix_2, 1);
        Key_8818(&kWPR_Ambience_2, 2);
        Key_8818(&kWPR_Speech_2, 3);
        Key_8818(&kWPR_Camera_2, 4);
        Key_8818(&kWPR_Fog_2, 5);
        Key_8818(&kWPR_LightGlow_2, 6);
        Key_8818(&kWPR_ScreenTint_2, 7);
        Key_8818(&kWPR_SkyBox_2, 8);
        Key_8818(&kWPR_Sun_2, 9);
        Key_8818(&kWPR_Surface_2, 10);
        Key_8818(&kWPR_Lighting_2, 11);
        Key_8818(&kWPR_Weather_2, 12);
        Key_8818(&kWPR_Danger_2, 13);
    }
}
#endif

