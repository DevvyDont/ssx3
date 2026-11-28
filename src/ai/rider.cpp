#include "vtable.h"
#include "common.h"
#include "ai/controlcmd.h"
#include "world/instancerecord.h"
#include "world/patchrecord.h"
#include "be/beinterface.h"
#include "camera/race/racecontroller.h"
#include "ui/uithing.h"
#include "render/irradiance.h"
#include "render/font.h"
#include "intersect/overlaplist.h"
#include "intersect/worldline.h"
#include "be/bedbprofile.h"
#include "ai/airpredictor.h"
#include "ai/ridermetrix.h"
#include "animation/animsequencer.h"
#include "camera/camera.h"
#include "ai/rideranim.h"
#include "render/playerlights.h"
#include "render/occlusionman.h"
#include "ai/riderhandler.h"
#include "main/gamemode.h"
#include "animation/mdfarchive.h"
#include "world/worldview.h"
#include "sound/ssxaudio.h"
#include "sound/streamsys.h"
#include "sound/bankmanager.h"
#include "main/gameviewman.h"
#include "main/sectionman.h"
#include "ai/player.h"
#include "bx/math3d.h"
#include "render/ps2graphicsman.h"
#include "animation/animmodel.h"
#include "be/beintnewrace.h"
#include "online/gamecomm.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"
#include "ai/ai.h"
#include "visualfx/transitionman.h"
#include "camera/cameracontroller.h"
#include "object/instancenode.h"
#include "inseq/scriptplayerentity.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("ai/rider", cRider_cRider);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void cRiderState_cRiderState(void* self);
extern "C" void* cScriptObjRider_cScriptObjRider(void* self);
extern "C" void* cAttachedEmitterFX_cAttachedEmitterFX(void* mem, void* rider);
extern "C" void* cBoltOnFXInterface_cBoltOnFXInterface(void* self, cRider* rider, void* emitterOwner);
void* cPlayerShadow_cPlayerShadow(void* self);
extern "C" void cScriptObject_InitScriptObject(void* rig);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
// PORT: cSSXAudio_RiderCreated is declared (void*) but takes (mgr, rider).
void cSSXAudio_RiderCreated_impl(void* mgr, void* rider) __asm__("cSSXAudio_RiderCreated__FPv");
extern "C" void* MEM_fill(void* dst, int value, int size);
// PORT: memset-like; called as void here.
extern "C" void MEM_fill_v(void* dst, int value, int size) __asm__("MEM_fill");
extern void* cRider_vtbl[];
extern char cRider_cScriptObjRider_vtbl[];
extern void* D_00459BD0[];
extern void* cBeing_vtbl[];
extern const char str_Track_path_intersect_info[];
extern const char str_AI_path_intersect_info[];
extern const char str_OverlapList[];
extern const char str_AttachedEmitterFX[];
extern const char str_cBoltOnFXInterface[];
extern const char str_Line_Intersect_Hint[];
extern const char str_Wall_Intersect_Hint[];
extern const char str_BoaderShadow[];

extern t2Vector t2vect_Zero[];


// PORT: the unit declares cRider_cRider as void, but the ctor returns this; bind by asm label.
extern "C" void* cRider_cRider_impl(cRider* self) __asm__("cRider_cRider");
/**
 * Constructs a rider: its bases and vtables, default fields and the helper objects it owns.
 *
 * Runs cRiderState_cRiderState (clears the slewed channels and other base data) and the cScriptObjRider base
 * constructor (cScriptObjRider_cScriptObjRider), installs the cRider, cScriptObjRider and cScriptObject vtables, sets mAIIndex,
 * mViewIndex and mSafeRenderTick to -1, mGroundMaterial to 0x31, mIsLocal and mRenderFlag to 1, and registers
 * the script object (cScriptObject_InitScriptObject).
 *
 * Allocates the track and AI path intersect infos, the "OverlapList" (bound to the game's Nash grid), the
 * attached-emitter and bolt-on FX, the line and wall intersect hints and the "BoaderShadow", and calls the
 * empty audio hook cSSXAudio_RiderCreated. Boost defaults: mode 1, source mask 31, uber flags 3, max speed 4000, cheat
 * steer align 0.18, cheat speed lerp 0.015.
 *
 * @return @p self
 */
extern "C" void* cRider_cRider_impl(cRider* self)
{
    unsigned int none = 0xFFFFFFFF;
    int one = 1;
    cRiderState_cRiderState(self);
    VPTR_AT(self, sizeof(cRiderState)) = cBeing_vtbl;
    cScriptObjRider_cScriptObjRider(&self->mScriptObjRiderVtbl);
    self->mScriptObjectVtbl = D_00459BD0;
    self->mAIIndex = -1;
    self->mViewIndex = -1;
    self->mSafeRenderTick = -1;
    self->mScriptObjRiderVtbl = cRider_cScriptObjRider_vtbl;
    VPTR_AT(self, sizeof(cRiderState)) = cRider_vtbl;
    self->mGroundMaterial = 49;
    self->mLoadStep = 0;
    self->mRenderFlag = one;
    self->mIsHuman = 0;
    self->mIsActor = 0;
    self->mIsLocal = one;
    self->mRiderAnim = 0;
    self->mAnimModel = 0;
    self->mSphereTree = 0;
    self->InTunnel = 0;
    self->mGroundPolyId = none;
    self->PatchUV = t2vect_Zero[0];
    self->mTrackPathHint = (tPathIntersectInfo*)cMemMan_alloc(0x20, str_Track_path_intersect_info, 0, 0);
    self->mAIPathHint = (tPathIntersectInfo*)cMemMan_alloc(0x20, str_AI_path_intersect_info, 0, 0);
    cOverlapList* aux = new (str_OverlapList, 0, 0) cOverlapList;
    cBXNash* nash = SSXApp->Game->BXNash;  // the overlap list queries the game's Nash collision grid
    aux->nash = nash;
    aux->deletecount = 0;
    aux->numinstances = none;
    aux->numpatches = none;
    aux->numtyped = none;
    self->mOverlapList = aux;
    self->mAttachedEmitterFX = cAttachedEmitterFX_cAttachedEmitterFX(cMemMan_alloc(168, str_AttachedEmitterFX, 0, 0), self);
    self->mBoltOnFX = cBoltOnFXInterface_cBoltOnFXInterface(cMemMan_alloc(200, str_cBoltOnFXInterface, 0, 0), self,
                                                            self->mAttachedEmitterFX);
    self->mLineHint = (tIntHint*)cMemMan_alloc(0x10, str_Line_Intersect_Hint, 0, 0);
    self->mWallHint = (tIntHint*)cMemMan_alloc(0x10, str_Wall_Intersect_Hint, 0, 0);
    MEM_fill_v(self->ActiveLights, 0, 0x20);
    self->mFocusBox = 0;
    self->DangerLevel = 0;
    self->mIsInNIS = 0;
    self->mHasTriggeredSpeechEvents = 0;
    self->mHiddenNIS = 0;
    *(cQuad128*)&self->mScriptCurrPosOffset = 0;
    self->mScriptCurrRotX = 0;
    self->mScriptCurrRotY = 0;
    self->mScriptCurrRotZ = 0;
    self->mNISShowBoardVisualFX = 0;
    self->mOnGroundNIS = one;
    self->mLastSpeed = 0;
    int i;
    for (i = 3; i >= 0; i--) {
        self->mSoundHandles[i] = -1;
    }
    int invalidHandle = -1;
    self->mShadow = cPlayerShadow_cPlayerShadow(cMemMan_alloc(416, str_BoaderShadow, 0, 0));
    self->unk_450 = invalidHandle;
    self->mShadowRendered = 0;
    self->mIsOnScreen = 0;
    self->mUpdateMinAnimOnly = 0;
    cScriptObject_InitScriptObject(self->unk_6DC);
    self->mWipeoutSlideHandle = invalidHandle;
    self->mWipeoutTumbleHandle = invalidHandle;
    cSSXAudio_RiderCreated_impl(cAudioInterface_bxAudio(), self);
    self->mBoostMeterStart = 0.0f;
    self->mBoostMode = 1;
    self->mWipeoutRecoveryMode = 1;
    self->mStartHandicap = 0;
    self->mStatsIndex = 0;
    self->mBoostSourceMask = 31;
    self->mUberFlags = 3;
    self->mAllowNaturalAir = 1;
    self->mBoostMaxSpeed = 4000.0f;
    self->mCheatSteerAlign = 0.18000000715255737f;
    self->mCheatSpeedLerp = 0.014999999664723873f;
    return self;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider__cRider);
#ifdef SKIP_ASM
struct sVE_11B978 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
extern "C" cSSXAudio* cAudioInterface_bxAudio();
// PORT: cSSXAudio_RiderDeleted takes (mgr, rider); the mangled name says one void* param.
void cSSXAudio_RiderDeleted_impl(void* mgr, void* rider) __asm__("cSSXAudio_RiderDeleted__FPv");
extern "C" void cScriptObject_DeInitScriptObject(void* p);
extern "C" void cScriptObject__cScriptObject(void* p, int flags);
extern "C" void cAnimModel__cAnimModel(void* p, int flags);
extern "C" void cRiderSphereTree__cRiderSphereTree(void* p, int flags);
extern "C" void _$_13cAirPredictor(void* p, int flags);
extern "C" void cFocusMan_removebox(void* a, void* p);
extern "C" void cRiderMetrix__cRiderMetrix(void* p, int flags);
extern "C" void _$_13cPlayerShadow(void* p, int flags);
void operator_delete(int* p);
extern void* cRider_vtbl[];
extern char cRider_cScriptObjRider_vtbl[];
extern void* D_00459BD0[];
extern void* cScriptObjRider_cScriptObject_vtbl[];
extern void* cScriptObjRider_vtbl[];
extern void* cBeing_vtbl[];

static inline void vdel_11B978(char* o, int off)
{
    sVE_11B978* vt = *(sVE_11B978**)(o + off);
    vt[1].fn(vthis(o, vt[1]), 3);
}

/**
 * Destroys a rider: frees the helper objects it owns, then unwinds its base classes.
 *
 * Reinstalls the cRider vtables, stops the rider's sounds (cSSXAudio_RiderDeleted), deinitialises the script object
 * and deletes mRiderAnim, mAnimModel, mSphereTree, the path hints, mAirPredictor, the focus box (after taking
 * it out of the AI's focus manager), mMetrix, the overlap list, the wall and line hints, the bolt-on and
 * attached-emitter FX and mShadow. Then restores the base vtables, runs cScriptObject__cScriptObject() and
 * puts back cBeing's vtable.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the rider itself
 */
extern "C" void cRider__cRider(cRider* self, int flags)
{
    self->mScriptObjectVtbl = D_00459BD0;
    self->mScriptObjRiderVtbl = cRider_cScriptObjRider_vtbl;
    VPTR_AT(self, sizeof(cRiderState)) = cRider_vtbl;
    cSSXAudio_RiderDeleted_impl(cAudioInterface_bxAudio(), self);
    cScriptObject_DeInitScriptObject(self->unk_6DC);
    char* polyMember = (char*)self->mRiderAnim;
    if (polyMember != 0)
        vdel_11B978(polyMember, 88);
    void* member = self->mAnimModel;
    if (member != 0)
        cAnimModel__cAnimModel(member, 3);
    member = self->mSphereTree;
    if (member != 0)
        cRiderSphereTree__cRiderSphereTree(member, 3);
    operator_delete((int*)self->mTrackPathHint);
    operator_delete((int*)self->mAIPathHint);
    member = self->mAirPredictor;
    if (member != 0)
        _$_13cAirPredictor(member, 3);
    void* box = self->mFocusBox;
    if (box != 0)
        cFocusMan_removebox(SSXApp->Game->AI->FocusMan, box);
    operator_delete((int*)self->mFocusBox);
    member = self->mMetrix;
    if (member != 0)
        cRiderMetrix__cRiderMetrix(member, 3);
    operator_delete((int*)self->mOverlapList);
    operator_delete((int*)self->mWallHint);
    operator_delete((int*)self->mLineHint);
    polyMember = (char*)self->mBoltOnFX;
    if (polyMember != 0)
        vdel_11B978(polyMember, 196);
    polyMember = (char*)self->mAttachedEmitterFX;
    if (polyMember != 0)
        vdel_11B978(polyMember, 164);
    member = self->mShadow;
    if (member != 0)
        _$_13cPlayerShadow(member, 3);
    self->mScriptObjectVtbl = cScriptObjRider_cScriptObject_vtbl;
    self->mScriptObjRiderVtbl = cScriptObjRider_vtbl;
    cScriptObject__cScriptObject(self->unk_6DC, 0);
    VPTR_AT(self, sizeof(cRiderState)) = cBeing_vtbl;
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_addFocusBox);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_FocusBox[];
extern "C" void cFocusMan_addbox(void* self, void* box);

/**
 * Gives the rider a focus box and registers it with the AI's focus manager.
 *
 * Allocates a 16-byte "FocusBox" into mFocusBox and adds it to Game->AI->FocusMan with cFocusMan_addbox, which
 * appends to the manager's 6-entry list without a bounds check. cRider_updateFocus() keeps the box at
 * mPosition.
 */
void cRider::addFocusBox()
{
    void* box = cMemMan_alloc(0x10, str_FocusBox, 0, 0);
    mFocusBox = box;
    cFocusMan_addbox(SSXApp->Game->AI->FocusMan, box);
}
#endif

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* __12cRiderMetrix(void* mem);
extern "C" void cRiderMetrix_linkToRider(void* metrix);
extern const char str_RiderMetrix[];

//100%
INCLUDE_ASM("ai/rider", cRider_addRiderMetrix__FP6cRider);
#ifdef SKIP_ASM
// PORT: cRiderMetrix_linkToRider really takes (metrix, rider); the unit declares one arg.
extern "C" void cRiderMetrix_linkToRider_impl(void* metrix, cRider* rider) __asm__("cRiderMetrix_linkToRider");

/**
 * Creates the rider's cRiderMetrix (trick and score tracking) and links it to the rider.
 *
 * Allocates 0x1CC bytes tagged "RiderMetrix", constructs it with cRiderMetrix::cRiderMetrix(), stores it in
 * mMetrix and calls cRiderMetrix_linkToRider(metrix, self).
 */
void cRider::addRiderMetrix()
{
    void* mem = cMemMan_alloc(460, str_RiderMetrix, 0, 0);
    void* metrix = __12cRiderMetrix(mem);
    mMetrix = (cRiderMetrix*)metrix;
    cRiderMetrix_linkToRider_impl(metrix, this);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_collectWornParts);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBELibrary_getCharacterID(int);
extern "C" int cBELibrary_getCheatCharacterID(int);
// PORT: cBEBoltInterface_getBoltDB is declared (void*) but never reads its argument; called here with none
void* cBEBoltInterface_getBoltDB_noarg() __asm__("cBEBoltInterface_getBoltDB__FPv");
int cBEBoltDB_getNumCharBolts(void* self, int i);
extern "C" void* cBEBoltDB_getCharBolts(void* self, int i);
extern "C" void* cBEBoltInterface_getRiderCharProfile(void* iface, int rider);

/**
 * Fills @p list with the BE part entries the rider wears and returns how many.
 *
 * Uses the rider's character (cBELibrary_getCharacterID with mAIIndex) and the parts DB (cBEBoltInterface_getBoltDB, entry list
 * cBEBoltDB_getCharBolts, count cBEBoltDB_getNumCharBolts; entries are 0x38 bytes). A cheat character (cBELibrary_getCheatCharacterID
 * non-zero) takes all of its own entries. Otherwise each entry's slot in the rider's BE data (cBEBoltInterface_getRiderCharProfile on
 * interface 9: map at +0x288 indexed by the entry's +4 short, slots of 4 bytes from +0x290) is taken when the slot's
 * flags have bit 0x10. Likely bug (retail too): an entry whose map value is negative gets a null slot whose flags are
 * still read (address 2). Called by cRider_parsePartsFromBuf with a 52-entry list.
 *
 * @param list  receives the entry pointers
 * @return the number of entries written
 */
int cRider::collectWornParts(tBEBoltDef** list)
{
    cRider* self = (cRider*)this;
    int n = 0;
    void* iface = cBE_getInterface(cBE_getBE(), 9);
    int ch = cBELibrary_getCharacterID(self->mAIIndex);
    int other = cBELibrary_getCheatCharacterID(self->mAIIndex);
    void* db = cBEBoltInterface_getBoltDB_noarg();
    if (other != 0)
    {
        ch = other;
        tBEBoltDef* part = (tBEBoltDef*)cBEBoltDB_getCharBolts(db, ch);
        int cnt = cBEBoltDB_getNumCharBolts(db, ch);
        for (int i = 0; i < cnt; i++)
        {
            list[n] = part;
            n++;
            part++;
        }
    }
    else
    {
        cBECharProfileDB* riderData = (cBECharProfileDB*)cBEBoltInterface_getRiderCharProfile(iface, self->mAIIndex);
        int cnt = cBEBoltDB_getNumCharBolts(db, ch);
        tBEBoltDef* part = (tBEBoltDef*)cBEBoltDB_getCharBolts(db, ch);
        for (int i = 0; i < cnt; i++, part++)
        {
            short idx = riderData->mpBoltsMap[part->id];
            tBEProfileBolt* slot;
            if (idx >= 0)
                slot = &((&riderData->mBolts[0]))[idx];
            else
                slot = 0;
            if (slot->flags & 0x10)
            {
                list[n] = part;
                n++;
            }
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_addPartModelsToLoadList);
#ifdef SKIP_ASM
extern "C" int strcmp(const void* a, const void* b);
extern "C" char* strcpy(char* dst, const char* src);
void* compareLoadListNames(void* self);
extern "C" void qsort(void* base, int n, int size, void* (*cmp)(void*));



/**
 * Adds the animation file names of @p n parts to the rider's load list (mLoadList), skipping duplicates, and sorts it.
 *
 * Each part's name is at +0x28; empty names are skipped and a name already in the table (strcmp) is not added
 * again. New entries (0x108 bytes: the name, -1, 0) go at the end and the count (+0x2940) is incremented; there is no
 * check against the table's 40 entries. The table is then sorted with qsort and compareLoadListNames.
 *
 * @param list  the part entries
 * @param n  how many there are
 */
void cRider::addPartModelsToLoadList(void** list, int n)
{
    for (int i = 0; i < n; i++, list++)
    {
        char* name = *(char**)((char*)*list + 0x28);
        tRiderLoadList* nameTable = ((tRiderLoadList*)mLoadList);
        if (name == 0) continue;
        if (*name == 0) continue;
        int j;
        for (j = 0; j < nameTable->MdlLoadNum; j++)
        {
            if (strcmp(name, &((&nameTable->LoadList[0]))[j]) == 0) goto next;
        }
        {
            tRiderLoader* entry = &((&nameTable->LoadList[0]))[nameTable->MdlLoadNum];
            strcpy(entry->FileName, name);
            entry->FileOp = -1;
            entry->DataPtr = 0;
            nameTable->MdlLoadNum++;
        }
    next:;
    }
    tRiderLoadList* sortTable = ((tRiderLoadList*)mLoadList);
    qsort(sortTable, sortTable->MdlLoadNum, 264, compareLoadListNames);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_addPartTexturesToLoadList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int strcmp(const void* a, const void* b);
extern "C" char* strcpy(char* dst, const char* src);
void* compareLoadListNames(void* self);
extern "C" void qsort(void* base, int n, int size, void* (*cmp)(void*));
extern "C" int cBEBoltInterface_getCharacterID(int c);
extern "C" int cBELibrary_getCheatCharacterID(int c);
extern "C" int cBEBoltInterface_getProfileIndex(int c);
extern "C" int cBEBoltInterface_buildBoltModelFileName(void* iface, int b, int a, void* item, char* out);



/**
 * Adds the texture names of @p n parts to the rider's second load list (mLoadList seen with a start index at +0x2940
 * and a count at +0x2944), skipping duplicates, and sorts that range.
 *
 * For a cheat character the name is the part's +0x2C string; otherwise cBEBoltInterface_buildBoltModelFileName on interface 9 builds it from the
 * character values cBEBoltInterface_getProfileIndex / cBEBoltInterface_getCharacterID and the part into a 256-byte buffer, skipping parts it rejects. New
 * entries go at start + count, without a bound check, and the range is sorted with qsort and compareLoadListNames.
 *
 * @param list  the part entries
 * @param n  how many there are
 */
void cRider::addPartTexturesToLoadList(void** list, int n)
{
    void* iface = cBE_getInterface(cBE_getBE(), 9);
    int characterId = cBEBoltInterface_getCharacterID(mAIIndex);
    if (cBELibrary_getCheatCharacterID(mAIIndex)) {
        for (int i = 0; i < n; i++) {
            char* name = *(char**)((char*)list[i] + 0x2C);
            tRiderLoadList* nameTable = ((tRiderLoadList*)mLoadList);
            if (name == 0) continue;
            if (*name == 0) continue;
            int j;
            int end = nameTable->TexLoadNum + nameTable->MdlLoadNum;
            for (j = nameTable->MdlLoadNum; j < end; j++) {
                if (strcmp(name, &((&nameTable->LoadList[0]))[j]) == 0) goto next;
            }
            {
                tRiderLoader* entry = &((&nameTable->LoadList[0]))[end];
                strcpy(entry->FileName, name);
                entry->FileOp = -1;
                entry->DataPtr = 0;
                nameTable->TexLoadNum++;
            }
        next:;
        }
    } else {
        int profileIndex = cBEBoltInterface_getProfileIndex(mAIIndex);
        for (int i = 0; i < n; i++) {
            char buf[0x100];
            if (cBEBoltInterface_buildBoltModelFileName(iface, profileIndex, characterId, list[i], buf) == 0) continue;
            char* name = buf;
            tRiderLoadList* nameTable = ((tRiderLoadList*)mLoadList);
            if (*name == 0) continue;
            int j;
            int end = nameTable->TexLoadNum + nameTable->MdlLoadNum;
            for (j = nameTable->MdlLoadNum; j < end; j++) {
                if (strcmp(name, &((&nameTable->LoadList[0]))[j]) == 0) goto next2;
            }
            {
                tRiderLoader* entry = &((&nameTable->LoadList[0]))[end];
                strcpy(entry->FileName, name);
                entry->FileOp = -1;
                entry->DataPtr = 0;
                nameTable->TexLoadNum++;
            }
        next2:;
        }
    }
    tRiderLoadList* sortTable = ((tRiderLoadList*)mLoadList);
    qsort(&((&sortTable->LoadList[0]))[sortTable->MdlLoadNum], sortTable->TexLoadNum, 264, compareLoadListNames);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_isSafeToDelete);
#ifdef SKIP_ASM

/**
 * Tells whether the renderer is done with the rider, so that it can be deleted.
 *
 * mSafeRenderTick is a graphics-manager tick recorded for the rider (kSafeRenderTickUnset = never set); the rider is
 * safe once the tick that the graphics manager's virtual slot 114 returns has passed it.
 *
 * @return true when mSafeRenderTick is set and lower than the graphics manager's tick
 */
bool cRider::isSafeToDelete()
{
    if (mSafeRenderTick != kSafeRenderTickUnset)
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        return mSafeRenderTick < ps2GraphicsMan->getCurrentFrameCount();
    }
    return false;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_parsePartsFromBuf);
#ifdef SKIP_ASM
void* cMdfArchive_cMdfArchive(void* self);
void cMdfArchive_attachArchive(void* self, int a1);
extern "C" void cMdfArchive__cMdfArchive(int* self, int flags);
extern "C" void* cMdfArchive_getModelPart(void* self, const char* name);
extern "C" int cRider_collectWornParts(cRider* self, tBEBoltDef** list);
extern "C" void cAnimModel_addModelPartLOD(void* model, int lod, void* part, int a3);


/**
 * Adds the rider's outfit model parts from an MDF archive buffer to its animation model.
 *
 * Wraps @p pData in a local cMdfArchive (cMdfArchive_cMdfArchive, cMdfArchive_attachArchive) and gets the rider's part entries
 * (cRider_collectWornParts: the equipped items of its character, or every part of a cheat character, into a 52-entry
 * list). For each entry's four LOD part names (lodPartNames), the named archive part is added to mAnimModel at
 * that LOD as variant 0. The archive is destroyed with cMdfArchive__cMdfArchive(arc, 2).
 *
 * @param pData  the MDF archive data, passed as an int (On Tour: pData)
 */
void cRider::parsePartsFromBuf(int pData)
{
    cMdfArchive arc;
    tBEBoltDef* list[52];
    cMdfArchive_cMdfArchive(&arc);
    cMdfArchive_attachArchive(&arc, pData);
    int n = this->collectWornParts(list);
    for (int i = 0; i < n; i++)
    {
        tBEBoltDef* item = list[i];
        for (int j = 0; j < 4; j++)
        {
            const char* name = item->lodPartNames[j];
            if (name != 0)
            {
                void* part = cMdfArchive_getModelPart(&arc, name);
                if (part != 0)
                {
                    cAnimModel_addModelPartLOD(mAnimModel, j, part, 0);
                }
            }
        }
    }
    cMdfArchive__cMdfArchive((int*)&arc, 2);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_initOnce);
#ifdef SKIP_ASM
extern "C" void cRider_loadRider(void* self);
extern "C" void THREAD_yieldticks(int ticks);

/**
 * Loads the rider, either with a single load step or step by step until loading completes.
 *
 * With @p immediate set it makes one cRider_loadRider() call and returns. Otherwise it keeps calling
 * cRider_loadRider(), yielding the thread for one tick after each call, until mLoadStep reaches kLoadStepDone.
 *
 * @param immediate  nonzero to make exactly one cRider_loadRider() call without waiting for mLoadStep
 */
void cRider::initOnce(int immediate)
{
    if (immediate)
    {
        cRider_loadRider(this);
        return;
    }
    while (mLoadStep != kLoadStepDone)
    {
        cRider_loadRider(this);
        THREAD_yieldticks(1);
    }
}
#endif

INCLUDE_ASM("ai/rider", cRider_loadRider);

//100%
INCLUDE_ASM("ai/rider", cRider_initAlways);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_11D390(void* be, int kind) __asm__("cBE_getInterface__Fv");
// cRiderMetrix_reset (cRiderMetrix reset) takes only the metrix; its $5 is left over from the store above.
extern "C" void cRiderMetrix_reset(void* metrix);
extern "C" void cRiderAnimBase_reset(void* anim);
extern "C" bool cBECharacterInterface_getIsGoofyFoot(void* iface, int riderIndex);
extern "C" void cRider_initPath(void* rider, int v);
extern "C" int cBENewPlayerInterface_isFirstTimeWC(void* iface, int index);
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cRider_putAtDropOffBC(cRider* self);
extern "C" void cRiderMetrix_evTakeoff(void* p, int flip, int c);
extern "C" void cRider_putAtStartpoint(cRider* self, int a1, int a2);
extern "C" void cRider_cleanupAfterPutAt(cRider* self, int notify);
extern "C" void cRider_initBoost(cRider* self);
extern "C" void cRiderHandler_resetRiderFX(void* self);
extern "C" void cRider_cancelRiderTransitions(cRider* self);
// PORT: prototype mismatch. The unit defines cRider_setControlState/cRider_setMotionState with one
// param, but their bodies forward $5 to cRiderHandler__setControlState/cRiderHandler__setMotionState (int arg).
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");


/**
 * Resets a loaded rider for a new run: stance, path, start position, timers and per-race state.
 *
 * Does nothing until the rider is loaded (mLoadStep == kLoadStepDone). Sets the handler's motion/control state words
 * (+0xDE0 = MS_Static, +0xDE4 = CS_Void), clears the rubber-band values, resets the metrix and anim base, sets
 * IsGoofyStance from cBECharacterInterface_getIsGoofyFoot (On Tour getIsGoofyFoot) and calls cRider_initPath
 * (initPath(true)).
 *
 * Placement: a rider flagged by cBENewPlayerInterface_isFirstTimeWC on location kLOCATION_ABC1 on the world circuit
 * (cGamePath kGAMEPATH_WORLDCIRCUIT), in game mode kGAMEMODE_FREERIDE, is put at the back-country drop-off
 * (cRider_putAtDropOffBC, MS_Air, CS_NaturalAir, takeoff scored); in kGAMEMODE_FREERIDE, kGAMEMODE_TIMECHALLENGE and
 * kGAMEMODE_PNTSCHALLENGE it goes to a start point chosen by the location (kLOCATION_HUB_A to kLOCATION_HUB_E: start
 * 0 of type 2; kLOCATION_ABC1 to kLOCATION_EBC3: its own start of type 1; else start 1 of type 2) and
 * cRider_cleanupAfterPutAt; otherwise MS_Static and CS_Gate. Off the world circuit (cGamePath non-zero) the boost is
 * initialised. Finally the finish/uber/boost timers, start tick (= AITick), penalty and overlap counts, rival fields
 * and transitions are cleared and the vibration reset (vtable slot 16) and rider FX reset run.
 */
void cRider::initAlways()
{
    if (mLoadStep == cRider::kLoadStepDone)
    {
        mRiderHandler->MotionState = MS_Static;
        mRiderHandler->ControlState = CS_Void;
        mRubberBandBehind = 0;
        mRubberBandAhead = 0;
        CheckID = -1;
        cRiderMetrix_reset(mMetrix);
        cRiderAnimBase_reset(mRiderAnim);
        IsGoofyStance = cBECharacterInterface_getIsGoofyFoot(cBE_getInterface_11D390(cBE_getBE(), 2), mAIIndex);
        cRider_initPath(this, 1);
        void* iface = cBE_getInterface_11D390(cBE_getBE(), 0);
        // PORT: the gotos reproduce the shared compare chain (mode 4, 5, 6) that two different entry conditions fall
        //       into.
        if (cBENewPlayerInterface_isFirstTimeWC(cBE_getInterface_11D390(cBE_getBE(), 1), mAIIndex) != 0
            && cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_WORLDCIRCUIT
            && *cBENewRaceInterface_getLocation(iface) == kLOCATION_ABC1)
        {
            if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_FREERIDE)
            {
                putAtDropOffBC();
                cRider_setMotionState_impl(this, MS_Air);
                cRider_setControlState_impl(this, CS_NaturalAir);
                cRiderMetrix_evTakeoff(mMetrix, 0, 0);
                goto done;
            }
            goto m5;
        }
        if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_FREERIDE)
            goto bb;
    m5:
        if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_TIMECHALLENGE)
            goto bb;
        if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_PNTSCHALLENGE)
        {
        bb:
            int location = *cBENewRaceInterface_getLocation(iface);
            switch (location)
            {
            case kLOCATION_HUB_A:
            case kLOCATION_HUB_B:
            case kLOCATION_HUB_C:
            case kLOCATION_HUB_D:
            case kLOCATION_HUB_E:
                putAtStartpoint(0, 2);
                cleanupAfterPutAt(0);
                break;
            case kLOCATION_ABC1:
            case kLOCATION_DBC2:
            case kLOCATION_EBC3:
                putAtStartpoint(mAIIndex, 1);
                cleanupAfterPutAt(0);
                break;
            default:
                putAtStartpoint(1, 2);
                cleanupAfterPutAt(0);
                break;
            }
        }
        else
        {
            cRider_setMotionState_impl(this, MS_Static);
            cRider_setControlState_impl(this, CS_Gate);
        }
    done:
        if (cBENewRaceInterface_newRace.cGamePath != 0)
            initBoost();
        mFinishTime = -1.0f;
        InTunnel = 0;
        mUberTimer = 0;
        mUberLevel = 0;
        mDidNotFinish = 0;
        mStartTick = SSXApp->Game->AI->AITick;
        mFinishTick = 0;
        mPenaltyTicks = 0;
        NumPathEventsCur = 0;
        mPowerUpBoostTimer = 0;
        mPowerUpTrickTimer = 0;
        ConsecutiveAirFrames = 0;
        sVEntry<void(void*)>* vt = (sVEntry<void (void*)>*)VPTR_AT(this, sizeof(cRiderState));
        vt[kRiderVt_vibrationReset].fn(vthis(this, vt[kRiderVt_vibrationReset]));
        cRiderHandler_resetRiderFX(mRiderHandler);
        VerticalJump = 0;
        DidPop = 0;
        RidingFlexRail = 0;
        PissedOff = 0;
        LastPunchTick = 0;
        PissedAIIndex = -1;
        ShouldCelebrateAtFinish = 1;
        cancelRiderTransitions();
        DangerLevel = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_isLoaded);
#ifdef SKIP_ASM
/**
 * Tells whether the rider has finished loading (mLoadStep reached kLoadStepDone).
 *
 * @return true when mLoadStep is kLoadStepDone
 */
bool cRider::isLoaded()
{
    int loadStep = mLoadStep;
    if (loadStep == kLoadStepDone)
        return true;
    return loadStep == kLoadStepDone;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_putAt);
#ifdef SKIP_ASM
// Places the rider at pos facing dir (xy), optionally dropping it onto the ground below
// (drop >= 0: probe from 200 above to 7000 below), and resets its motion state and animation.
extern "C" float func_0031C228(float x);
extern "C" float func_0031C128(float x);
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cRider_updateOrientationImplicit(cRider* self);
extern "C" void cRiderAnimBase_reset(void* anim);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cRiderAnimBase_setupAnimation(void* anim, int a1);
extern "C" void cRider_updateAnimation(cRider* self, float value);
extern "C" void cRider_updateSkeleton(void* self);
extern "C" void* cAnimModel_buildSkelMatrices(void* obj);
extern "C" void cRider_updateBoundBox(cRider* self, int a1);
extern "C" void cOverlapList_geteverything(void*, void*);
extern "C" void cRider_updateActiveLights(cRider*);
extern "C" void setRadarRiderResetFlag(int);

// The unit declares these later with their own vector types; bind them by asm label.
extern const t4Vector t4vect_Zero_D660 __asm__("t4vect_Zero");
extern const t4Vector t4vect_Origin_D660 __asm__("t4vect_Origin");
extern const t4Vector t4vect_ZAxis_D660 __asm__("t4vect_ZAxis");



void cWorldLine_cWorldLine_D660(cWorldLine* ray, const t4Vector& a, const t4Vector& b, int n, float t) __asm__("cWorldLine_cWorldLine");
extern float cBXNash_terrainIntersect1_D660(void* coll, cWorldLine* ray, tIntersectInfo* hit, int flags) __asm__("cBXNash_terrainIntersect1");


// A tQuaternion with EA's inline constructors; the 4-float one stores w, then y, x, z (that order reproduces the
// retail store schedule). Same layout as t4Vector: mRotation and the pose quaternions are t4Vectors holding a
// quaternion, written through an sQuatW cast.
struct sQuatW : tQuaternion {
    sQuatW() {}
    sQuatW(float ax, float ay, float az, float aw) { w = aw; y = ay; x = ax; z = az; }
} __attribute__((aligned(16)));

// Non-POD by-value vector (user copy ctor => passed by invisible reference).
struct sVecP_D660 {
    float x, y, z, w;
    sVecP_D660(const t4Vector& v) { *(t4Vector*)this = v; }
    sVecP_D660(const sVecP_D660& v) { *(t4Vector*)this = *(const t4Vector*)&v; }
} __attribute__((aligned(16)));

static inline float Atan2_D660(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

static inline sQuatW AxisAngle_D660(const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQuatW(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline sQuatW AxisAngleL_D660(const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    sQuatW q;
    q.w = c;
    q.x = s * axis.x;
    q.y = s * axis.y;
    q.z = s * axis.z;
    return q;
}

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector Scale_D660(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_D660(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void AddEq_D660(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_D660(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_D660(const t4Vector& a, const t4Vector& b)
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

static inline float DotV_D660(t4Vector a, const t4Vector& b)
{
    return Dot_D660(a, b);
}

static inline t4Vector SubV_D660(sVecP_D660 a, const t4Vector& b)
{
    return Sub_D660(*(t4Vector*)&a, b);
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector Cross_D660(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector length).
static inline float Length_D660(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void DivEq_D660(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s));
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector Normalize_D660(const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline t4Vector QuatMul_D660(const sQuatW& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline void ClearCache_D660(char* vc)
{
    tIntHint* c = (tIntHint*)vc;
    c->mPatch = 0;
    *(short*)&c->mU = 0;
    *(short*)&c->mV = 0;
    c->mFirstTri = 0;
    c->mPrecision = 0;
}

static inline void InitChannel_D660(cRider* self, int i, float v, float z)
{
    self->mSlewed[i].target = v;
    self->mSlewed[i].cur = v;
    self->mSlewed[i].rate = z;
}

// PORT: the unit declares cRider_putAt(void*, sVec4_0011DE60*, ..., int, float) later; bind the cRider* view by asm
// label.
// drop is declared before anim: the order of the parameter copies follows it (ABI-identical: f12 and a3 either way).
/**
 * Places the rider at a position facing a direction, optionally on the ground below, and resets its motion and
 * animation.
 *
 * Clears the line/wall hints, faces the rider along atan2(@p dir) and, when @p drop >= 0, casts a terrain ray from 200
 * above to 7000 below @p pos (cWorldLine_cWorldLine/cBXNash_terrainIntersect1) and stands the rider @p drop above the hit, tilted to the
 * hit normal. Then resets the ground frame, velocity, impulse, the 19 smoothed channels (channel 9 to 1), spin/flip
 * rates, boost, rail/press state, attack and HUD message, sets the stance from IsGoofyStance (anim heading turned half
 * a turn when switched), plays @p anim, sets up and runs the animation and skeleton (cRiderAnimBase_setupAnimation,
 * cRider_updateAnimation, cRider_updateSkeleton, cAnimModel_buildSkelMatrices), updates the bounds, overlap list and lights,
 * setRadarRiderResetFlag(mAIIndex), resets vibration, and resets the camera of a rider with a view (mViewIndex < 2).
 *
 * The C definition is cRider_putAt with an asm label.
 *
 * @param pos  where to put the rider
 * @param dir  the direction to face (On Tour desFront; only x and y are used)
 * @param drop  On Tour height: when >= 0, drop onto the ground below and stand this far above it
 * @param anim  the AI animation to start (On Tour tAIAnimID initAnim)
 */
void cRider::putAt(const t4Vector* pos, const float* dir, float drop, int anim)
{
    ClearCache_D660((char*)mLineHint);
    ClearCache_D660((char*)mWallHint);
    t4Vector placePos = *pos;
    t4Vector n = t4vect_ZAxis_D660;
    float angle = Atan2_D660(dir[1], dir[0]);
    mVisitedIds[0] = 0xFFFFFFFF;
    if (drop >= 0.0f)
    {
        t4Vector lo = Add_D660(*pos, Scale_D660(t4vect_ZAxis_D660, -7000.0f));
        t4Vector hi = Add_D660(*pos, Scale_D660(t4vect_ZAxis_D660, 200.0f));
        tIntersectInfo hit;
        cWorldLine ray;
        cWorldLine_cWorldLine_D660(&ray, lo, hi, 2, 0.9791666865348816f);
        if (cBXNash_terrainIntersect1_D660(SSXApp->Game->BXNash, &ray, &hit, 0) >= 0.0f)
        {
            placePos = hit.Ipos;
            n = hit.Inorm;
        }
        AddEq_D660(placePos, Scale_D660(t4vect_ZAxis_D660, drop));
    }
    mPosition = placePos;
    *(sQuatW*)&mRotation = AxisAngle_D660(t4vect_ZAxis_D660, angle - 1.5707963705062866f);
    mUseRootPoseOverride = 0;
    t4Vector axis = Cross_D660(t4vect_ZAxis_D660, n);
    float len = Length_D660(axis);
    if (len > 0.0010000000474974513f)
    {
        DivEq_D660(axis, len);
        // PORT: g++ minimum operator.
        sQuatW tiltRot = AxisAngleL_D660(axis, func_0031C128(len <? 1.0f));
        mRotation = QuatMul_D660(tiltRot, mRotation);
    }
    updateOrientationImplicit();
    mGroundNormal = n;
    GroundVelocity = t4vect_Zero_D660;
    mGroundForward = Normalize_D660(SubV_D660(mMatrix.row[1], Scale_D660(mGroundNormal, DotV_D660(mMatrix.row[1],
            mGroundNormal))));
    mGroundSide = Cross_D660(mGroundNormal, mGroundForward);
    mVelocity = t4vect_Zero_D660;
    mGroundContactPoint = mPosition;
    mSurfaceType = 0;
    SnowOffset = 0.0f;
    mImpulse = t4vect_Zero_D660;
    mImpactAccum = 0.0f;
    mImpactAccum2 = 0.0f;
    InitChannel_D660(this, 0, 0.0f, 0.0f);
    InitChannel_D660(this, 1, 0.0f, 0.0f);
    InitChannel_D660(this, 2, 0.0f, 0.0f);
    InitChannel_D660(this, 3, 0.0f, 0.0f);
    InitChannel_D660(this, 4, 0.0f, 0.0f);
    InitChannel_D660(this, 5, 0.0f, 0.0f);
    InitChannel_D660(this, 6, 0.0f, 0.0f);
    InitChannel_D660(this, 7, 0.0f, 0.0f);
    InitChannel_D660(this, 8, 0.0f, 0.0f);
    InitChannel_D660(this, 9, 1.0f, 0.0f);
    InitChannel_D660(this, 10, 0.0f, 0.0f);
    InitChannel_D660(this, 11, 0.0f, 0.0f);
    InitChannel_D660(this, 12, 0.0f, 0.0f);
    InitChannel_D660(this, 13, 0.0f, 0.0f);
    InitChannel_D660(this, 14, 0.0f, 0.0f);
    InitChannel_D660(this, 15, 0.0f, 0.0f);
    InitChannel_D660(this, 16, 0.0f, 0.0f);
    InitChannel_D660(this, 17, 0.0f, 0.0f);
    InitChannel_D660(this, 18, 0.0f, 0.0f);
    mSpinRate = 0.0f;
    mFlipRate = 0.0f;
    mBoostStrength = 0.0f;
    mTimeScale = 1.0f;
    LegIK = 1.0f;
    ChatterMag = 0.0f;
    RailslideMode = 0;
    HandplantMode = 0;
    PressMode = 0;
    PunchVect = t4vect_Zero_D660;
    PunchStrength = 0.0f;
    ReactionEventTick = -1;
    ReactionEventType = 0;
    ProximityActionDelay = 0.0f;
    IsRidingFakie = IsGoofyStance;
    cRiderAnimBase_reset(mRiderAnim);
    mRiderAnim->IsFlipped = IsRidingFakie;
    float yaw = 0.0f;
    if (IsRidingFakie != 0)
        yaw = 3.1415927410125732f;
    cRiderAnim* riderAnim = mRiderAnim;
    riderAnim->OrientationOffset.P = t4vect_Origin_D660;
    *(sQuatW*)&riderAnim->OrientationOffset.Q = AxisAngle_D660(t4vect_ZAxis_D660, -yaw);
    cRiderAnimBase_play(mRiderAnim, anim, 0, -1.0f);
    cRiderAnimBase_setupAnimation(mRiderAnim, 0);
    updateAnimation(1.0f);
    cRider_updateSkeleton(this);
    cAnimModel_buildSkelMatrices(mAnimModel);
    cRider_updateBoundBox(this, 0);
    cOverlapList_geteverything(mOverlapList, &mBoundsMin);
    updateActiveLights();
    setRadarRiderResetFlag(mAIIndex);
    sVEntry<void(void*)>* vt = *(sVEntry<void(void*)>**)&VPTR_AT(this, sizeof(cRiderState));
    vt[kRiderVt_vibrationReset].fn(vthis(this, vt[kRiderVt_vibrationReset]));
    if ((unsigned int)mViewIndex < 2)
    {
        cGameViewMan* views = SSXApp->Game->ViewMan;
        if (views != 0)
        {
            cChaseCameraController* viewHandler = views->PlayerCamera[mViewIndex]->ChaseController;
            sVEntry<void(void*)>* hv = (sVEntry<void(void*)>*)viewHandler->mVtbl;
            hv[4].fn(vthis(viewHandler, hv[4])); // cCameraController_snapCurrent
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_putAtDropOffBC);
#ifdef SKIP_ASM
extern "C" void* cPathSys_getStartpoint(void*, int, int);
extern char gPaths[];
extern "C" float cRider_getStartGateHorizOffset(void* self);


// The unit declares cRider_putAt later with its own vector type; bind it by asm label.
void cRider_putAt_11DD98(void* self, t4Vector* a, t4Vector* b, int n, float t) __asm__("cRider_putAt");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_11DD98(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddEq_11DD98(t4Vector& dst, t4Vector b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

/**
 * Places the rider at its backcountry drop-off point.
 *
 * Gets startpoint type 2 for the rider's mAIIndex from PathSys (cPathSys_getStartpoint, On Tour cPathSys::getStartpoint),
 * moves the point forward along its direction (floats 5-7) by cRider_getStartGateHorizOffset(self), and calls cRider::putAt
 * (cRider_putAt) with that position and direction, anim id 0x11F and 2999.94.
 */
void cRider::putAtDropOffBC()
{
    float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 2, mAIIndex);
    t4Vector dir;
    t4Vector pos;
    dir.x = startPoint[5];
    dir.y = startPoint[6];
    dir.z = startPoint[7];
    dir.w = 0.0f;
    pos.x = startPoint[2];
    pos.y = startPoint[3];
    pos.z = startPoint[4];
    pos.w = 1.0f;
    vu0AddEq_11DD98(pos, vu0Scale_11DD98(dir, cRider_getStartGateHorizOffset(this)));
    cRider_putAt_11DD98(this, &pos, &dir, 287, 2999.942626953125f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_putAtStartpoint);
#ifdef SKIP_ASM
extern "C" void* cPathSys_getStartpoint(void*, int, int);
extern char gPaths[];
extern "C" void cRiderMetrix_evReset(void* metrix, int flag);
// PORT: prototype mismatch. The unit defines cRider_setControlState/cRider_setMotionState with one
// param, but their bodies forward $5 to cRiderHandler__setControlState/cRiderHandler__setMotionState (int arg).
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");


extern "C" void cRider_putAt(void* self, t4Vector* a, t4Vector* b, int n, float t);

/**
 * Puts the rider on the ground at a path start point, facing the point's direction.
 *
 * Looks the point up in the start-point list gPaths (cPathSys_getStartpoint falls back to the list's first entry
 * when nothing matches), ends the current trick and combo in mMetrix (cRiderMetrix_evReset), sets control state
 * CS_Cruise and motion state MS_Ground, then places the rider with cRider_putAt (anim 5, height 0, so it is
 * dropped onto the ground).
 *
 * @param StartpointID  id of the start point (the rider resets pass 0, 1 or the rider's mAIIndex)
 * @param StartpointType  start-point type (On Tour: tPathStartType); 1 and 2 are used
 */
void cRider::putAtStartpoint(int StartpointID, int StartpointType)
{
    float* startPoint = (float*)cPathSys_getStartpoint(gPaths, StartpointType, StartpointID);
    t4Vector pos;
    t4Vector dir;
    float px = startPoint[2];
    float py = startPoint[3];
    float pz = startPoint[4];
    pos.x = px;
    pos.y = py;
    pos.z = pz;
    pos.w = 1.0f;
    float dx = startPoint[5];
    float dy = startPoint[6];
    float dz = startPoint[7];
    dir.x = dx;
    dir.y = dy;
    dir.z = dz;
    dir.w = 1.0f;
    cRiderMetrix_evReset(mMetrix, 1);
    cRider_setControlState_impl(this, 0);
    cRider_setMotionState_impl(this, 0);
    cRider_putAt(this, &pos, &dir, 5, 0.0f);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_cleanupAfterPutAt);
#ifdef SKIP_ASM
extern "C" cAnimSequence* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" void cRiderHandler_resetRiderFX(void* self);
extern "C" void cWScriptMan_eventReset(void* self);
extern void* cWScriptMan_TheWScriptMan;



// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_11DF18(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__ __volatile__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

/**
 * Finishes a placement: gives the rider a starting speed along its heading and resets its FX.
 *
 * Clears word +0x90 of the rider animation's sequence 2, sets mVelocity to the front axis times 833.33 with
 * no z part and resets the handler's FX. With @p notify set it then calls virtual slot 3 of view 0's current
 * camera controller with the rider's AI index, probably to reset the camera. Last, it queues world-script op
 * 0x70005 (cWScriptMan_eventReset).
 *
 * @param notify  nonzero to notify view 0's camera controller (On Tour: resetCamera)
 */
void cRider::cleanupAfterPutAt(int notify)
{
    t4Vector forwardAxis = *(&mMatrix.row[1]);
    cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 2)->TimeScale = 0;
    t4Vector launchVelocity = vu0Scale_11DF18(forwardAxis, 833.3333740234375f);
    *(&mVelocity) = launchVelocity;
    mVelocity.z = 0.0f;
    cRiderHandler_resetRiderFX(mRiderHandler);
    if (notify != 0)
    {
        char* sub = (char*)&VPTR_AT(this, sizeof(cRiderState));
        sVEntry<int(void*)>* vtA = *(sVEntry<int(void*)>**)sub;
        cCameraController* ctrl = SSXApp->Game->ViewMan->PlayerCamera[0]->Controller;
        sVEntry<void(void*, int)>* vtB = (sVEntry<void(void*, int)>*)ctrl->mVtbl; // slot 3: snap(subjectID)
        void* objB = vthis(ctrl, vtB[3]);
        vtB[3].fn(objB, vtA[kRiderVt_getAIIndex].fn(sub + vtA[kRiderVt_getAIIndex].delta));
    }
    cWScriptMan_eventReset(cWScriptMan_TheWScriptMan);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_rotateOrientation);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cRider_updateOrientationImplicit(cRider* self);

struct sQuat_0011DFE0
{
    float x, y, z, w;
    sQuat_0011DFE0() {}
    sQuat_0011DFE0(float ax, float ay, float az, float aw) { w = aw; x = ax; y = ay; z = az; }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline t4Vector QuatMul_0011DFE0(const sQuat_0011DFE0& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

static inline sQuat_0011DFE0 AxisAngle_0011DFE0(const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQuat_0011DFE0(s * axis.x, s * axis.y, s * axis.z, c);
}

/**
 * Rotates the rider's orientation by an angle about an axis and rebuilds its matrix.
 *
 * Pre-multiplies mRotation by the axis-angle quaternion, then calls cRider_updateOrientationImplicit().
 *
 * @param axis  rotation axis (a unit vector)
 * @param angle  rotation angle in radians
 */
void cRider::rotateOrientation(t4Vector* axis, float angle)
{
    mRotation = QuatMul_0011DFE0(AxisAngle_0011DFE0(*axis, angle), mRotation);
    updateOrientationImplicit();
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateOrientationImplicit);
#ifdef SKIP_ASM
/**
 * Normalises the rider's orientation quaternion and rebuilds its matrix from it.
 *
 * VU0 macro-mode code: scales mRotation to unit length in place, writes the rotation it describes into rows 0-2
 * of mMatrix (w components cleared) and copies mPosition into row 3.
 */
// PORT: PS2-only VU0 macro-mode asm. Normalises the orientation quaternion mRotation
// (0x120), then builds mMatrix (0x1A0) from it (rows 0-2) and mPosition (0x110, row 3).
void cRider::updateOrientationImplicit()
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2       $vf3, 0x120(%0)\n"
        "vaddw.x    $vf6, $vf0, $vf0w\n"
        "vmul.xyzw  $vf4, $vf3, $vf3\n"
        "vadday.x   ACC, $vf4, $vf4y\n"
        "vmaddaz.x  ACC, $vf6, $vf4z\n"
        "vmaddw.x   $vf4, $vf6, $vf4w\n"
        "vrsqrt     Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2       $vf5, 0x120(%0)\n"
        "lqc2       $vf4, 0x120(%0)\n"
        "lqc2       $vf3, 0x110(%0)\n"
        "vaddw.xyz  $vf1, $vf0, $vf0w\n"
        "vadd.xyz   $vf5, $vf4, $vf4\n"
        "vsub.w     $vf10, $vf10, $vf10\n"
        "vsub.w     $vf11, $vf11, $vf11\n"
        "vsub.w     $vf12, $vf12, $vf12\n"
        "vmul.xyz   $vf6, $vf5, $vf4\n"
        "vmulw.xyz  $vf7, $vf5, $vf4w\n"
        "vopmula.xyz ACC, $vf5, $vf4\n"
        "vmadd.xyz  $vf8, $vf0, $vf0\n"
        "vsubay.x   ACC, $vf1, $vf6y\n"
        "vmsubz.x   $vf10, $vf1, $vf6z\n"
        "vsubaz.y   ACC, $vf1, $vf6z\n"
        "vmsubx.y   $vf11, $vf1, $vf6x\n"
        "vsubax.z   ACC, $vf1, $vf6x\n"
        "vmsuby.z   $vf12, $vf1, $vf6y\n"
        "vaddaz.y   ACC, $vf0, $vf8z\n"
        "vmaddz.y   $vf10, $vf1, $vf7z\n"
        "vaddax.z   ACC, $vf0, $vf8x\n"
        "vmaddx.z   $vf11, $vf1, $vf7x\n"
        "vaddax.y   ACC, $vf0, $vf8x\n"
        "vmsubx.y   $vf12, $vf1, $vf7x\n"
        "vadday.z   ACC, $vf0, $vf8y\n"
        "vmsuby.z   $vf10, $vf1, $vf7y\n"
        "vaddaz.x   ACC, $vf0, $vf8z\n"
        "vmsubz.x   $vf11, $vf1, $vf7z\n"
        "vadday.x   ACC, $vf0, $vf8y\n"
        "vmaddy.x   $vf12, $vf1, $vf7y\n"
        "sqc2       $vf3, 0x30(%1)\n"
        "sqc2       $vf10, 0x0(%1)\n"
        "sqc2       $vf11, 0x10(%1)\n"
        "sqc2       $vf12, 0x20(%1)\n"
        ".set pop\n"
        :
        : "r"(this), "r"(&mMatrix)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateBoundBox);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

// 16-byte vectors with inline constructors (non-POD siblings of t4Vector). The classes differ only in the order
// their 4-float constructor stores the components; that order decides the float-register allocation of every
// caller (merging them changes code), so they are kept apart.
// sVec4 stores x, y, z, w.
struct sVec4 : t4Vector {
    sVec4() {}
    sVec4(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sVec4 Scale_E150(const sVec4& v, float s)
{
    sVec4 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec4 Add_E150(const sVec4& a, const sVec4& b)
{
    sVec4 r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4 Sub_E150(const sVec4& a, const sVec4& b)
{
    sVec4 r;
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


static inline void SetMin_E150(float& m, float v)
{
    if (v < m)
        m = v;
}

static inline void SetMax_E150(float& m, float v)
{
    if (v > m)
        m = v;
}

// Grows the box to contain c.
static inline void Grow_E150(tBoundBox& b, const sVec4& c)
{
    SetMin_E150((*(sVec4*)&b.MinPoint).x, c.x);
    SetMin_E150((*(sVec4*)&b.MinPoint).y, c.y);
    SetMin_E150((*(sVec4*)&b.MinPoint).z, c.z);
    SetMax_E150((*(sVec4*)&b.MaxPoint).x, c.x);
    SetMax_E150((*(sVec4*)&b.MaxPoint).y, c.y);
    SetMax_E150((*(sVec4*)&b.MaxPoint).z, c.z);
}

extern "C" void cRider_updateBoundBox_impl(cRider* self, sVec4* pin) __asm__("cRider_updateBoundBox");
/**
 * Recomputes the rider's bounding box and bounding sphere around a position.
 *
 * The box (mBoundsMin, mBoundsMax) is grown over the eight corners at +-150 along the front axis, +-100 along
 * the side axis and +250 or -50 along the up axis; mBoundSphere is the position with radius 250.
 *
 * @param pin  centre to use, or 0 for mPosition (On Tour: pRiderPos)
 */
extern "C" void cRider_updateBoundBox_impl(cRider* self, sVec4* pin)
{
    sVec4 up = *(sVec4*)&self->mMatrix.row[2];
    sVec4 side = *(sVec4*)&self->mMatrix.row[0];
    sVec4 front = *(sVec4*)&self->mMatrix.row[1];
    sVec4* basePos = pin ? pin : (sVec4*)&self->mPosition;
    *(sVec4*)&self->mBoundsMin = *(sVec4*)&self->mBoundsMax = *basePos;
    Grow_E150(*(tBoundBox*)&self->mBoundsMin, Add_E150(Add_E150(Add_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 250.0f)));
    Grow_E150(*(tBoundBox*)&self->mBoundsMin, Sub_E150(Add_E150(Add_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 50.0f)));
    Grow_E150(*(tBoundBox*)&self->mBoundsMin, Add_E150(Sub_E150(Add_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 250.0f)));
    Grow_E150(*(tBoundBox*)&self->mBoundsMin, Sub_E150(Sub_E150(Add_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 50.0f)));
    Grow_E150(*(tBoundBox*)&self->mBoundsMin, Add_E150(Add_E150(Sub_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 250.0f)));
    Grow_E150(*(tBoundBox*)&self->mBoundsMin, Sub_E150(Add_E150(Sub_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 50.0f)));
    Grow_E150(*(tBoundBox*)&self->mBoundsMin, Add_E150(Sub_E150(Sub_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 250.0f)));
    Grow_E150(*(tBoundBox*)&self->mBoundsMin, Sub_E150(Sub_E150(Sub_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 50.0f)));
    *(sVec4*)&self->mBoundSphere = *basePos;
    self->mBoundSphere.w = 250.0f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateAnimation);
#ifdef SKIP_ASM
extern "C" void cRiderAnimBase_updateAnimation(void*, int);
extern "C" void cRiderAnimBase_expireAnimation(void*);

/**
 * Advances the rider's animation by one step.
 *
 * Calls cRiderAnimBase_updateAnimation(mRiderAnim, unk_B1C), which steps every node of the animation's six sequencers, then
 * cRiderAnimBase_expireAnimation(mRiderAnim). The C declares cRiderAnimBase_updateAnimation without its float time argument, so @p value
 * reaches it untouched in the float argument register.
 *
 * @param value  animation time step (On Tour: timescale; callers pass 1.0 or mTimeScale)
 */
void cRider::updateAnimation(float value)
{
    cRiderAnimBase_updateAnimation(mRiderAnim, mUpdateMinAnimOnly);
    cRiderAnimBase_expireAnimation(mRiderAnim);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSkeleton);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

// 16-byte vectors with inline constructors (non-POD siblings of t4Vector). The classes differ only in the order
// their 4-float constructor stores the components; that order decides the float-register allocation of every
// caller (merging them changes code), so they are kept apart.
// sVec4 stores x, y, z, w.
// sVec4W stores w first, then x, y, z.
struct sVec4W : t4Vector {
    sVec4W() {}
    sVec4W(float ax, float ay, float az, float aw)
    {
        w = aw;
        x = ax;
        y = ay;
        z = az;
    }
} __attribute__((aligned(16)));
extern t4Vector t4vect_Origin;
extern t4Vector t4vect_ZAxis;
extern t4Vector tquat_Identity;
extern "C" void cSpinControl_applyFlipSpinAngle(void* self, void* xf);
extern "C" void cRider_doLeanPoseAdjust(void* self, void* xf);
extern "C" void cAnimModel_transform(cAnimModel* self, void* roots);
extern "C" void cRider_doLegIK(void* self, void* obj, void* xf, float t);
extern "C" void cRider_updateCollideInfo(void* self);
extern "C" float func_0031C128(float x);
extern "C" void fSinCos(float* s, float* c, float angle);
int cRider_getControlState(void* self);

// A bone transform with sVec4W members: the same layout as sAnimModelBone (pos 0x00, rot 0x10), so the bones are
// viewed through it.
struct sXf_EB98 {
    sVec4W pos;
    sVec4W rot;
};


// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sVec4W Scale_EB98(const t4Vector& v, float s)
{
    sVec4W r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec4W Add_EB98(const sVec4W& a, const sVec4W& b)
{
    sVec4W r;
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

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void AddTo_EB98(sVec4W& a, const sVec4W& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}
// vector += vector through VU0. Declared to return the vector by value (as if for chaining) but there is no return
// statement: g++ 2.95 accepts it, every caller ignores the result, and the ignored by-value result is the unwritten
// 16-byte call temp the retail frame (and the July 2003 prototype's) has at 0x70.
static inline sVec4W operator+=(sVec4W& a, const sVec4W& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4W Sub_EB98(const sVec4W& a, const t4Vector& b)
{
    sVec4W r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "o"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sVec4W Cross_EB98(const sVec4W& a, const sVec4W& b)
{
    sVec4W r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector length).
static inline float Length_EB98(const sVec4W& v)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_EB98(const sVec4W& a, const sVec4W& b)
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

// PORT: PS2-only VU0 inline asm (in-place vector / scalar via vdiv).
static inline void DivEq_EB98(sVec4W& r, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(r), "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (v /= s, then the 4-component dot product of a and b).
static inline float DivDot_EB98(sVec4W& v, float s, const sVec4W& a, const sVec4W& b)
{
    float r;
    int t, u;
    __asm__(
        "mfc1      %3, %6\n"
        "qmtc2.ni  %3, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %4\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        "lqc2      $vf3, %7\n"
        "lqc2      $vf5, %8\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %2, $vf4\n"
        "mtc1      %2, %1\n"
        : "=m"(v), "=f"(r), "=&r"(t), "=&r"(u)
        : "m"(v), "m"(v), "f"(s), "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sVec4W QuatMul_EB98(const sVec4W& a, const sVec4W& b)
{
    sVec4W r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline sVec4W Rot_EB98(const sVec4W& q, const sVec4W& v)
{
    sVec4W r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 macro-mode asm (quaternion + position to a 4x4 matrix).
static inline void QuatToMat_EB98(void* dst, const sVec4W* q, const sVec4W* p)
{
    __asm__ __volatile__(
        "lqc2       $vf4, 0(%1)\n"
        "lqc2       $vf3, 0(%2)\n"
        "vaddw.xyz  $vf1, $vf0, $vf0w\n"
        "vadd.xyz   $vf5, $vf4, $vf4\n"
        "vsub.w     $vf10, $vf10, $vf10\n"
        "vsub.w     $vf11, $vf11, $vf11\n"
        "vsub.w     $vf12, $vf12, $vf12\n"
        "vmul.xyz   $vf6, $vf5, $vf4\n"
        "vmulw.xyz  $vf7, $vf5, $vf4w\n"
        "vopmula.xyz ACC, $vf5, $vf4\n"
        "vmadd.xyz  $vf8, $vf0, $vf0\n"
        "vsubay.x   ACC, $vf1, $vf6y\n"
        "vmsubz.x   $vf10, $vf1, $vf6z\n"
        "vsubaz.y   ACC, $vf1, $vf6z\n"
        "vmsubx.y   $vf11, $vf1, $vf6x\n"
        "vsubax.z   ACC, $vf1, $vf6x\n"
        "vmsuby.z   $vf12, $vf1, $vf6y\n"
        "vaddaz.y   ACC, $vf0, $vf8z\n"
        "vmaddz.y   $vf10, $vf1, $vf7z\n"
        "vaddax.z   ACC, $vf0, $vf8x\n"
        "vmaddx.z   $vf11, $vf1, $vf7x\n"
        "vaddax.y   ACC, $vf0, $vf8x\n"
        "vmsubx.y   $vf12, $vf1, $vf7x\n"
        "vadday.z   ACC, $vf0, $vf8y\n"
        "vmsuby.z   $vf10, $vf1, $vf7y\n"
        "vaddaz.x   ACC, $vf0, $vf8z\n"
        "vmsubz.x   $vf11, $vf1, $vf7z\n"
        "vadday.x   ACC, $vf0, $vf8y\n"
        "vmaddy.x   $vf12, $vf1, $vf7y\n"
        "sqc2       $vf3, 0x30(%0)\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "sqc2       $vf11, 0x10(%0)\n"
        "sqc2       $vf12, 0x20(%0)\n"
        :
        : "r"(dst), "r"(q), "r"(p)
        : "memory");
}

static inline sVec4W AxisAngle_EB98(const sVec4W& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sVec4W(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline float Min_EB98(float a, float b)
{
    return a <? b;
}

// Child transform = parent transform a composed with local transform b.
static inline sXf_EB98 Combine_EB98(const sXf_EB98& a, const sXf_EB98& b)
{
    sXf_EB98 r;
    r.rot = QuatMul_EB98(a.rot, b.rot);
    r.pos = Add_EB98(Rot_EB98(a.rot, Sub_EB98(b.pos, t4vect_Origin)), a.pos);
    return r;
}

/**
 * Poses the rider's skeleton from its position and rotation plus spin, flip, lean, board tilt and leg adjustments.
 *
 * The root pose is mPosition/mRotation; in control state 5 (cRider_getControlState) the spin control (handler +0x230) adds the
 * flip/spin rotation (cSpinControl_applyFlipSpinAngle), then cRider_doLeanPoseAdjust leans it. Unless wiping out (state 13) channel 18
 * raises the body along its up axis. With mUseRootPoseOverride the board bones are reset and the board root takes the
 * override pose. cAnimModel_transform poses the bones from the body and board roots.
 *
 * Channel 17 then tilts the board-root bone toward the axis at +0x390 (and lifts it along mGroundNormal by unk_31C),
 * re-deriving the board child bone; unless wiping out, LegIK drives cRider_doLegIK (per-leg adjustment) for the two
 * leg records at +0x8E0/+0x910. Finally the rider matrix (+0x160) is rebuilt from the pose and cRider_updateCollideInfo runs.
 *
 * The C definition is cRider_updateSkeleton with an asm label.
 */
void cRider::updateSkeleton()
{
    sXf_EB98 xf;
    xf.pos = *(sVec4W*)&mPosition;
    xf.rot = *(sVec4W*)&mRotation;
    if (getControlState() == CS_Spin)
        cSpinControl_applyFlipSpinAngle(&mRiderHandler->SpinControl, &xf);
    cRider_doLeanPoseAdjust(this, &xf);
    sXf_EB98 ab[2];
    ab[1] = xf;
    ab[0] = xf;
    if (getControlState() != CS_Void && mSlewed[18].cur != 0.0f)
    {
        sVec4W liftOffset = Scale_EB98(t4vect_ZAxis, mSlewed[18].cur);
        ab[0].pos += Rot_EB98(ab[0].rot, liftOffset);
    }
    if (mUseRootPoseOverride != 0)
    {
        mAnimModel->localPos[mBoneBoardRoot] = t4vect_Origin;
        mAnimModel->localRot[mBoneBoardRoot] = tquat_Identity;
        mAnimModel->localPos[mBoneBoardChild] = t4vect_Origin;
        mAnimModel->localRot[mBoneBoardChild] = tquat_Identity;
        ab[1].pos = *(sVec4W*)&mRootPoseOverridePos;
        ab[1].rot = *(sVec4W*)&mRootPoseOverrideRot;
    }
    cAnimModel_transform(mAnimModel, ab);
    if (mSlewed[17].cur != 0.0f)
    {
        sXf_EB98* boardRoot = (sXf_EB98*)&mAnimModel->bones[mBoneBoardRoot];
        float qx = boardRoot->rot.x, qy = boardRoot->rot.y, qz = boardRoot->rot.z, qw = boardRoot->rot.w;
        sVec4W ax;
        ax.w = 0.0f;
        ax.x = 1.0f - 2.0f * (qy * qy + qz * qz);
        ax.y = 2.0f * (qx * qy + qw * qz);
        ax.z = 2.0f * (qx * qz - qw * qy);
        sVec4W tiltAxis = Cross_EB98(*(sVec4W*)&DampGroundNormal, ax);
        float len = Length_EB98(tiltAxis);
        if (len > 9.99999993922529e-09f)
        {
            float tiltDot = DivDot_EB98(tiltAxis, len, *(sVec4W*)&DampGroundNormal, ax);
            float sinTilt = Min_EB98(len, 1.0f);
            sVec4W tiltRot;
            if (tiltDot < 0.0f)
                tiltRot = AxisAngle_EB98(tiltAxis, (func_0031C128(sinTilt) - 1.5707963705062866f) * mSlewed[17].cur);
            else
                tiltRot = AxisAngle_EB98(tiltAxis, (1.5707963705062866f - func_0031C128(sinTilt)) * mSlewed[17].cur);
            boardRoot->rot = QuatMul_EB98(tiltRot, boardRoot->rot);
        }
        if (0.0f < ChatterMag)
            AddTo_EB98(boardRoot->pos, Scale_EB98(Scale_EB98(*(sVec4W*)&mGroundNormal, ChatterMag), mSlewed[17].cur));
        {
            cAnimModel* am = mAnimModel;
            sXf_EB98 local;
            local.pos = sVec4W(am->localPos[mBoneBoardChild].x * am->ScaleVect.x,
                    am->localPos[mBoneBoardChild].y * am->ScaleVect.y,
                    am->localPos[mBoneBoardChild].z * am->ScaleVect.z,
                    am->localPos[mBoneBoardChild].w * am->ScaleVect.w);
            sVec4W* lr = (sVec4W*)&am->localRot[mBoneBoardChild];
            local.rot = *lr;
            sXf_EB98* bn = (sXf_EB98*)&am->bones[mBoneBoardChild];
            *bn = Combine_EB98(*boardRoot, local);
        }
    }
    if (getControlState() != CS_Void && LegIK != 0.0f)
    {
        sXf_EB98* bb = (sXf_EB98*)&mAnimModel->bones[mBoneBoardChild];
        {
            sXf_EB98 xx = Combine_EB98(*bb, *(sXf_EB98*)&RightLeg.BoardRelativePose);
            cRider_doLegIK(this, &RightLeg, &xx, LegIK);
        }
        {
            sXf_EB98 xx = Combine_EB98(*bb, *(sXf_EB98*)&LeftLeg.BoardRelativePose);
            cRider_doLegIK(this, &LeftLeg, &xx, LegIK);
        }
    }
    QuatToMat_EB98(&RiderRoot, &xf.rot, &xf.pos);
    cRider_updateCollideInfo(this);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_doLegIK);
#ifdef SKIP_ASM
// Two-bone IK: moves bone chain[2] (end effector) toward target (blended by blend), then bends the
// chain[1] joint and swings the chain[0] root so the end reaches it. Uses the unit's sVec4W / sXf_EB98
// helpers (cRider_updateSkeleton, this function's caller).
extern "C" sVec4W tquat_slerp(const sVec4W& a, const sVec4W& b, float t);

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4W Normalize_F3D8(const sVec4W& v)
{
    sVec4W r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: g++ `<?` (min) operator.
static inline float Clamp_F3D8(float v, float lo, float hi)
{
    return lo <= v ? (v <? hi) : lo;
}

// Point v of transform x's local space, in x's parent space.
static inline sVec4W XformPoint_F3D8(const sXf_EB98& x, const sVec4& v)
{
    return Add_EB98(Rot_EB98(x.rot, Sub_EB98(*(const sVec4W*)&v, t4vect_Origin)), x.pos);
}

extern "C" void cRider_doLegIK_impl(cRider* self, int* chain, sXf_EB98* target, float blend) __asm__("cRider_doLegIK");
/**
 * Bends one of the rider's legs so its foot reaches a target transform (two-bone IK).
 *
 * @p chain holds the leg's three bone indices (On Tour tLegInfo): [0] the root (hip), [1] the middle joint (knee), [2]
 * the end (foot). The end bone is set to @p target when @p blend is 1.0f, otherwise moved toward it: position lerp,
 * rotation through tquat_slerp (a quaternion slerp). The two segment lengths are the x of the middle and end bones'
 * local offsets scaled by the model's ScaleVect (its length scale).
 *
 * The knee angle follows from the law of cosines (func_0031C128 is an arcsine; its argument is clamped to [-1, 1]); the
 * middle bone's local transform is the upper length along x with a rotation about Z. The root is then turned about the
 * cross product of the bent leg's direction and the root-to-target direction, by the arcsine of that cross product's
 * length (skipped below 1e-5), and the middle bone is rebuilt from the turned root. cRider_updateSkeleton calls it for
 * the legs at +0x8E0 and +0x910 with blend LegIK.
 *
 * @param chain  the leg's bone indices: root, middle joint, end
 * @param target  the transform (position, rotation) the end bone should take
 * @param blend  how far to move the end bone to @p target; 1.0f snaps it
 */
extern "C" void cRider_doLegIK_impl(cRider* self, int* chain, sXf_EB98* target, float blend)
{
    sXf_EB98* end = (sXf_EB98*)&self->mAnimModel->bones[chain[2]];
    if (blend == 1.0f) {
        *end = *target;
    } else {
        end->pos = Add_EB98(end->pos, Scale_EB98(Sub_EB98(target->pos, end->pos), blend));
        end->rot = tquat_slerp(end->rot, target->rot, blend);
    }
    cAnimModel* am = self->mAnimModel;
    sAnimModelBone* bones = am->bones;
    float upperLen = sVec4W(am->localPos[chain[1]].x * am->ScaleVect.x, am->localPos[chain[1]].y * am->ScaleVect.y,
                     am->localPos[chain[1]].z * am->ScaleVect.z, am->localPos[chain[1]].w * am->ScaleVect.w).x;
    sXf_EB98* mid = (sXf_EB98*)&bones[chain[1]];
    float lowerLen = sVec4W(am->localPos[chain[2]].x * am->ScaleVect.x, am->localPos[chain[2]].y * am->ScaleVect.y,
                     am->localPos[chain[2]].z * am->ScaleVect.z, am->localPos[chain[2]].w * am->ScaleVect.w).x;
    sXf_EB98* root = (sXf_EB98*)&bones[chain[0]];
    sVec4W rootToEnd = Sub_EB98(end->pos, root->pos);
    float ang = func_0031C128(Clamp_F3D8((Dot_EB98(rootToEnd, rootToEnd) - upperLen * upperLen - lowerLen * lowerLen) * 0.5f / (upperLen * lowerLen), -1.0f, 1.0f));
    sXf_EB98 local;
    // x-first sVec4 here (the store order shows it); local.pos itself is an sVec4W.
    (t4Vector&)local.pos = sVec4(upperLen, 0.0f, 0.0f, 1.0f);
    local.rot = AxisAngle_EB98(*(sVec4W*)&t4vect_ZAxis, ang - 1.5707963705062866f);
    sXf_EB98 kneeXf = Combine_EB98(*root, local);
    sVec4W dir = Sub_EB98(XformPoint_F3D8(kneeXf, sVec4(lowerLen, 0.0f, 0.0f, 1.0f)), root->pos);
    sVec4W axis = Cross_EB98(Normalize_F3D8(dir), Normalize_F3D8(rootToEnd));
    float len = Length_EB98(axis);
    if (len > 9.999999747378752e-06f) {
        DivEq_EB98(axis, len);
        sVec4W alignRot;
        alignRot = AxisAngle_EB98(axis, func_0031C128(len));
        root->rot = QuatMul_EB98(alignRot, root->rot);
    }
    *mid = Combine_EB98(*root, local);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_doLeanPoseAdjust);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

// sVec4XW stores x, w, y, z.
struct sVec4XW : t4Vector {
    sVec4XW() {}
    sVec4XW(float ax, float ay, float az, float aw) { x = ax; w = aw; y = ay; z = az; }
} __attribute__((aligned(16)));
extern t4Vector t4vect_Origin;
extern t4Vector t4vect_XAxis;
extern t4Vector t4vect_YAxis;
extern "C" void fSinCos(float* sout, float* cout, float x);

struct sXf_LP {
    sVec4XW pos;
    sVec4XW rot;
};


// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sVec4XW QuatMul_LP(const sVec4XW& a, const sVec4XW& b)
{
    sVec4XW r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b, constant b through a register).
static inline sVec4XW SubO_LP(const sVec4XW& a, const t4Vector& b)
{
    sVec4XW r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "o"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline sVec4XW Rot_LP(const sVec4XW& q, const sVec4XW& v)
{
    sVec4XW r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4XW Scale_LP(const sVec4XW& v, float s)
{
    sVec4XW r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (dst += b, label keeps the store out of a following jal delay slot).
static inline void AddToL_LP(sVec4XW& dst, const sVec4XW& b)
{
    __asm__(
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set reorder\n"
        : "=m"(dst)
        : "m"(dst), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void AddTo_LP(sVec4XW& dst, const sVec4XW& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b)
        : "memory");
}

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_LP(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline void SetAA_LP(sVec4XW& q, const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    q.x = s * axis.x;
    q.w = c;
    q.y = s * axis.y;
    q.z = s * axis.z;
}

static inline sVec4XW AxisAngle_LP(const t4Vector& axis, float angle)
{
    sVec4XW q;
    SetAA_LP(q, axis, angle);
    return q;
}

static inline void QuatMulTo_LP(sVec4XW& a, sVec4XW b)
{
    a = QuatMul_LP(a, b);
}

// rot *= q (quaternion product). Declared to return the quaternion by value but has no return statement (the
// classic forgotten return); the caller ignores the result, and that ignored by-value result is the unwritten
// 16-byte temp the retail frame keeps during the first lean rotation. The other two sites use QuatMulTo_LP.
static inline sVec4XW operator*=(sVec4XW& a, const sVec4XW& b)
{
    a = QuatMul_LP(a, b);
}

static inline float deadzone_LP(float v, float k)
{
    float d = absf_LP(v) - (absf_LP(k) + absf_LP(k));
    if (d < 0.0f)
        d = 0.0f;
    else if (v < 0.0f)
        d = -d;
    return d;
}

static inline void LeanAbout_LP(cRider* self, sXf_LP* xf, float a, float b, float ka, float kb)
{
    cAnimModel* trk = self->mAnimModel;
    sVec4XW* n = (sVec4XW*)((self->mBoneBoardRoot << 4) + (int)trk->localPos);
    sVec4XW d = SubO_LP(sVec4XW(n->x * trk->ScaleVect.x, n->y * trk->ScaleVect.y,
                            n->z * trk->ScaleVect.z, n->w * trk->ScaleVect.w),
                      t4vect_Origin);
    {
        sVec4XW v = Rot_LP(xf->rot, d);
        AddToL_LP(xf->pos, v);
        xf->rot *= AxisAngle_LP(t4vect_XAxis, a * ka);
    }
    QuatMulTo_LP(xf->rot, AxisAngle_LP(t4vect_YAxis, b * kb));
    sVec4XW w = Scale_LP(d, -1.0f);
    sVec4XW u = Rot_LP(xf->rot, w);
    AddTo_LP(xf->pos, u);
}

extern "C" void cRider_doLeanPoseAdjust_x(cRider* self, sXf_LP* xf) __asm__("cRider_doLeanPoseAdjust");
/**
 * Tilts the rider's root pose for its lean inputs.
 *
 * Takes slewed channels 2 and 0, each less a dead zone of twice |channel 3|; when either is nonzero the pose
 * is rotated about the board root bone (mBoneBoardRoot) by channel 2 x 30 degrees about X and channel 0 x 45
 * degrees about Y. A nonzero channel 8 adds channel 8 x 45 degrees about X, and the position moves along
 * mGroundSide by channel 0 x 45 x the model's length scale (mAnimModel->ScaleVect.x).
 *
 * @param xf  the root pose to adjust, position and rotation quaternion (On Tour: tPose & root)
 */
extern "C" void cRider_doLeanPoseAdjust_x(cRider* self, sXf_LP* xf)
{
    float leanAboutX = deadzone_LP(self->mSlewed[2].cur, self->mSlewed[3].cur);
    float leanAboutY = deadzone_LP(self->mSlewed[0].cur, self->mSlewed[3].cur);
    if (leanAboutY != 0.0f || leanAboutX != 0.0f)
        LeanAbout_LP(self, xf, leanAboutX, leanAboutY, 0.523598849773407f, 0.7853982448577881f);
    float k = self->mSlewed[8].cur;
    if (k != 0.0f) {
        QuatMulTo_LP(xf->rot, AxisAngle_LP(t4vect_XAxis, k * 0.7853982448577881f));
    }
    sVec4XW sideShift = Scale_LP(*(sVec4XW*)&self->mGroundSide,
                                 self->mAnimModel->ScaleVect.x * (self->mSlewed[0].cur * 45.0f));
    AddTo_LP(xf->pos, sideShift);
}
#endif

extern "C" void* cRiderHandler__setMotionState(void* obj);

//100%
INCLUDE_ASM("ai/rider", cRider_setMotionState__FPv);
#ifdef SKIP_ASM
/**
 * Forwards a motion-state change to the rider's handler.
 *
 * Calls cRiderHandler__setMotionState() on mRiderHandler. Only the rider is declared here; any further
 * arguments the caller passed in registers reach the handler unchanged.
 *
 * @return the handler call's result
 */
void* cRider::setMotionState()
{
    return cRiderHandler__setMotionState(mRiderHandler);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getMotionState__FPv);
#ifdef SKIP_ASM
/**
 * Returns the rider's motion state: its mRiderHandler's MotionState (+0xDE0).
 *
 * 0 ground, 1 air, 2 wipeout, 3 static, 4 railslide, 5 handplant (On Tour's eMotionState).
 *
 * @return the motion state
 */
int cRider::getMotionState()
{
    return mRiderHandler->MotionState;
}
#endif

extern "C" void* cRiderHandler__setControlState(void* obj);

//100%
INCLUDE_ASM("ai/rider", cRider_getMotionStateName);
#ifdef SKIP_ASM
extern "C" void cRiderHandler__getMotionStateName(cRiderHandler* handler);

/**
 * Returns the name of the rider's motion state: cRiderHandler__getMotionStateName on mRiderHandler.
 *
 * The C body has no return, but the callee's string pointer is left in v0.
 *
 */
void cRider::getMotionStateName()
{
    cRiderHandler__getMotionStateName(mRiderHandler);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_setControlState__FPv);
#ifdef SKIP_ASM
/**
 * Switches the rider's control state (cruise, air, wipeout, ...) through its rider handler.
 *
 * A thin wrapper: cRiderHandler__setControlState(mRiderHandler, state). The C prototype has only the rider; the new
 * state rides through in $a1 (callers declare it as (rider, int)).
 *
 * @return whatever cRiderHandler__setControlState returns (nothing meaningful; On Tour's is void)
 */
void* cRider::setControlState()
{
    return cRiderHandler__setControlState(mRiderHandler);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getControlState__FPv);
#ifdef SKIP_ASM
/**
 * Returns the rider's control state: its mRiderHandler's ControlState (+0xDE4).
 *
 * 0 cruise, 1 board press, 2 prewind, 3 bump, 4 natural air, 5 spin, 6 gate, 7 railslide, 8 wipeout, 9 reset, 10 finish
 * line, 11 handplant, 12 uber grind, 13 void (On Tour's eControlState).
 *
 * @return the control state
 */
int cRider::getControlState()
{
    return mRiderHandler->ControlState;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getControlStateName);
#ifdef SKIP_ASM
extern "C" void cRiderHandler__getControlStateName(cRiderHandler* handler);

/**
 * Returns the name of the rider's control state: cRiderHandler__getControlStateName on mRiderHandler.
 *
 * The C body has no return, but the callee's string pointer is left in v0.
 *
 */
void cRider::getControlStateName()
{
    cRiderHandler__getControlStateName(mRiderHandler);
}
#endif
//100%
INCLUDE_ASM("ai/rider", cRider_getControlCmd);
#ifdef SKIP_ASM

/**
 * Fetches the rider's current control command from its rider handler (slot 1 of the handler's vtable at +0xDE8).
 *
 * @param out  receives the command (On Tour tControlCmd& i_Cmd)
 */
// Forwards its second argument to vtable slot 1 of the rider handler.
void cRider::getControlCmd(tControlCmd* out)
{
    cRiderHandler* obj = mRiderHandler;
    sVEntry<void(void*, void*)>* vt = (sVEntry<void (void*, void*)>*)obj->mVtbl;
    vt[1].fn(vthis(obj, vt[1]), out);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getHipPosition);
#ifdef SKIP_ASM

/**
 * Returns the position of the rider's hip bone (mBoneHips) in its animation model, as a 16-byte vector.
 *
 * The vector is returned through @p dst (On Tour returns a t4Vector by value).
 *
 * @param dst  receives the hip bone's position quadword
 * @return @p dst
 */
// Copies the quadword of the anim model's transform unk_89C into dst.
extern "C" void* cRider_getHipPosition(void* dst, cRider* self)
{
    cAnimModel* animModel = self->mAnimModel;
    int i = self->mBoneHips;
    sAnimModelBone* animModelBone = animModel->bones;
    *(cQuad128*)dst = (*(cQuad128*)&animModelBone[i].pos);
    return dst;
}
#endif

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBECharacterInterface_getWeight(void* iface, int character);
extern "C" float cBEStatInterface_getCollisionAttrib(void* iface, int character, int stat);

//100%
INCLUDE_ASM("ai/rider", cRider_getHeadPosition);
#ifdef SKIP_ASM
/**
 * Copies the position of the rider's head bone (mBoneHead in mAnimModel's bone array) into @p dst.
 *
 * @param dst  receives the position
 * @return @p dst
 */
// Copies the quadword of the anim model's transform unk_8A8 into dst (sibling of cRider_getHipPosition).
extern "C" void* cRider_getHeadPosition(void* dst, cRider* self)
{
    cAnimModel* animModel = self->mAnimModel;
    int i = self->mBoneHead;
    sAnimModelBone* animModelBone = animModel->bones;
    *(cQuad128*)dst = (*(cQuad128*)&animModelBone[i].pos);
    return dst;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getMass);
#ifdef SKIP_ASM
/**
 * Computes the rider's mass from character weight, collision toughness and boost strength.
 *
 * mass = weight * (1 + 1.5 * toughness) * (1 + 10 * mBoostStrength), where weight comes from the BE character
 * interface for mAIIndex and toughness is the BE stat interface's collision attribute for mAIIndex and
 * mStatsIndex (the 1.5 is 1.5003352 in the binary).
 *
 * @return the rider's mass
 */
float cRider::getMass()
{
    int weight = cBECharacterInterface_getWeight(cBE_getInterface(cBE_getBE(), 2), mAIIndex);
    float toughness = cBEStatInterface_getCollisionAttrib(cBE_getInterface(cBE_getBE(), 3),
                                                          mAIIndex, mStatsIndex);

    return (float)weight * (toughness * 1.5003352165222168f + 1.0f) * (mBoostStrength * 10.0f + 1.0f);
}
#endif

extern "C" float cBEStatInterface_getTrickAttrib(void* iface, int character, int stat);

// Grab playback speed: 1 + grab stat * 0.2998.
//100%
INCLUDE_ASM("ai/rider", cRider_getGrabAnimSpeedScale);
#ifdef SKIP_ASM
/**
 * Returns the speed factor for the rider's grab animations, from a character stat.
 *
 * The stat is cBEStatInterface_getTrickAttrib(BE interface 3, mAIIndex, mStatsIndex).
 *
 * @return the stat x 0.29988 + 1
 */
float cRider::getGrabAnimSpeedScale()
{
    void* iface = cBE_getInterface(cBE_getBE(), 3);
    return cBEStatInterface_getTrickAttrib(iface, mAIIndex, mStatsIndex) * 0.29988324642181396f + 1.0f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateTimeWarp);
#ifdef SKIP_ASM
/**
 * Moves the rider's time scale (mTimeScale) toward a target by at most 1/1200 per call.
 *
 * @param x  desired time scale (On Tour: DesiredTimeWarp)
 */
void cRider::updateTimeWarp(float x)
{
    float cur = mTimeScale;
    if (x + 0.00083333341171965f < cur)
        x = cur - 0.00083333341171965f;
    else if (cur < x - 0.00083333341171965f)
        x = cur + 0.00083333341171965f;
    mTimeScale = x;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateBoost);
#ifdef SKIP_ASM
extern "C" void cRider_flagEventReaction(void* self, int mode, float t);


static inline float decay_1200D0(float v, float step)
{
    if (step < v) {
        return v - step;
    }
    if (v < -step) {
        return v + step;
    }
    return 0.0f;
}

/**
 * Runs the rider's per-frame boost meter, power-up and uber timers.
 *
 * With step = mTimeScale / 60: mPowerUpBoostTimer decays, and mPowerUpTrickTimer too unless it is in its last
 * frame while airborne. mUberTimer cannot run out while airborne or in CS_UberGrind (it is held at 1/60);
 * otherwise it decays, and reaching 0 calls cRider_flagEventReaction(self, 6, 0).
 *
 * At uber level eUS_SuperUberMax the boost meter (unk_2F8) stays full until the timer runs out, then the level drops to
 * eUS_SuperUberCapable with a 20 s timer. Otherwise mBoostMode sets the meter: kBOOST_Unlimited full, kBOOST_Normal and
 * kBOOST_ExtraLeaky drain by step x 0.005 or step x 0.024 except in CS_Gate. The isComputer virtual is also called and
 * its result ignored.
 */
void cRider::updateBoost()
{
    float step = mTimeScale * 0.01666666753590107f;
    mPowerUpBoostTimer = decay_1200D0(mPowerUpBoostTimer, step);
    if (0.01666666753590107f < mPowerUpTrickTimer || getMotionState() != MS_Air) {
        mPowerUpTrickTimer = decay_1200D0(mPowerUpTrickTimer, step);
    }
    char* sub = (char*)&VPTR_AT(this, sizeof(cRiderState));
    sVEntry<int(void*)>* vtEntry = &(*(sVEntry<int(void*)>**)sub)[kRiderVt_isComputer];
    vtEntry->fn(vthis(sub, *vtEntry));
    if ((getMotionState() == MS_Air || getControlState() == CS_UberGrind) && 0.0f < mUberTimer
        && mUberTimer <= 0.01666666753590107f) {
        mUberTimer = 0.01666666753590107f;
    } else if (0.0f < mUberTimer) {
        mUberTimer = decay_1200D0(mUberTimer, step);
        if (mUberTimer == 0.0f) {
            cRider_flagEventReaction(this, 6, 0.0f);
        }
    }
    if (mUberLevel == eUS_SuperUberMax) {
        BoostPowerBar = 1.0f;
        if (mUberTimer == 0.0f) {
            mUberLevel = eUS_SuperUberCapable;
            mUberTimer = 20.0f;
        }
    } else {
        switch (mBoostMode) {
        case kBOOST_Unlimited:
            BoostPowerBar = 1.0f;
            break;
        case kBOOST_Normal:
            if (getControlState() != CS_Gate) {
                BoostPowerBar = decay_1200D0(BoostPowerBar, step * 0.004999999888241291f);
            }
            break;
        case kBOOST_ExtraLeaky:
            if (getControlState() != CS_Gate) {
                BoostPowerBar = decay_1200D0(BoostPowerBar, step * 0.02398611046373844f);
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateLookAnims);
#ifdef SKIP_ASM
struct sV_120378 {
    float x, y, z, w;
    sV_120378() {}
    sV_120378(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

// One head/upper-body look controller (three of them from cRider+0x940).
struct sLook_120378 {
    int enabled;
    char pad04[0xC];
    sV_120378 rest;      // 0x10 offset quaternion (IsRidingFakie == 0)
    sV_120378 restAlt;   // 0x20 offset quaternion (IsRidingFakie != 0)
};

int cRider_getControlState(void*);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" int AITick();
int cRiderAnimBase_getCurrentAnimID(void*, int);

// PORT: PS2-only VU0 inline asm (a - b).
static inline sV_120378 Sub_120378(const sV_120378& a, const sV_120378& b)
{
    sV_120378 r;
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

// PORT: PS2-only VU0 inline asm (in-place v += b).
static inline void AddEq_120378(sV_120378& v, const sV_120378& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_120378(const sV_120378& v)
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

// PORT: PS2-only VU0 inline asm (dot product).
static inline float Dot_120378(const sV_120378& a, const sV_120378& b)
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

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sV_120378 QuatMul_120378(const t4Vector& a, const sV_120378& b)
{
    sV_120378 r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: abs.s
static inline float Abs_120378(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// Rotation-matrix rows of a unit quaternion.
static inline sV_120378 Front_120378(const sV_120378& q)
{
    return sV_120378(1.0f - (q.y * q.y + q.z * q.z) * 2.0f, (q.x * q.y + q.w * q.z) * 2.0f, (q.x * q.z - q.w * q.y) * 2.0f, 0.0f);
}

static inline sV_120378 Side_120378(const sV_120378& q)
{
    return sV_120378((q.y * q.x - q.w * q.z) * 2.0f, 1.0f - (q.z * q.z + q.x * q.x) * 2.0f, (q.y * q.z + q.w * q.x) * 2.0f, 0.0f);
}

static inline sV_120378 Up_120378(const sV_120378& q)
{
    return sV_120378((q.x * q.z + q.w * q.y) * 2.0f, (q.z * q.y - q.w * q.x) * 2.0f, 1.0f - (q.x * q.x + q.y * q.y) * 2.0f, 0.0f);
}

// PORT: g++ `<?` operator.
static inline float Clamp_120378(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

// Picks the look anim (base + 0..6) for one controller from where v points in the bone's frame.
#define LOOK_120378(layer, bone, look, base)                                                        \
    if ((look)->enabled) {                                                                          \
        int cur = cRiderAnimBase_getCurrentAnimID(self->mRiderAnim, layer);                                           \
        int anim;                                                                                   \
        if (still) {                                                                                \
            anim = (base);                                                                          \
        } else {                                                                                    \
            if (self->IsRidingFakie) {                                                                    \
                q = QuatMul_120378(*(t4Vector*)((char*)self->mAnimModel->bones + ((bone) << 5) + 0x10), (look)->restAlt);             \
            } else {                                                                                \
                q = QuatMul_120378(*(t4Vector*)((char*)self->mAnimModel->bones + ((bone) << 5) + 0x10), (look)->rest);                \
            }                                                                                       \
            float fwd = Dot_120378(v, Front_120378(q));                                             \
            float up = Dot_120378(v, Up_120378(q));                                                 \
            if (upLimit < Abs_120378(up) && Abs_120378(fwd) < Abs_120378(up)) {                     \
                if (0.0f < up) {                                                                    \
                    anim = (base) + 1;                                                              \
                } else {                                                                            \
                    anim = (base) + 2;                                                              \
                }                                                                                   \
            } else if (sideLimit < Abs_120378(fwd)) {                                               \
                if (fwd < 0.0f) {                                                                   \
                    anim = (base) + 4;                                                              \
                } else {                                                                            \
                    anim = (base) + 3;                                                              \
                }                                                                                   \
            } else {                                                                                \
                anim = (base) + 5;                                                                  \
                if (Dot_120378(v, Side_120378(q)) < 0.0f) {                                         \
                    anim = (base) + 6;                                                              \
                }                                                                                   \
            }                                                                                       \
        }                                                                                           \
        if (anim != cur) {                                                                          \
            cRiderAnimBase_play(self->mRiderAnim, anim, 0, -1.0f);                                  \
        }                                                                                           \
    }

/**
 * Drives the rider's three look animations (head, upper body, hips) from its velocity relative to the ground.
 *
 * The relative velocity is mVelocity - GroundVelocity. Layers 3-5's current sequences get a playback rate of speed / 1666.7,
 * clamped to 0.5-2 (sequence + 0x90). Every 6th AI tick, on the tick equal to mAIIndex, each enabled look controller
 * (rider + 0x940, 0x970, 0x9A0: an enabled word and two offset quaternions, the second used when IsRidingFakie) picks
 * an animation from its base (411, 418, 425): base when the rider is nearly still (speed 138.9 or less, except in
 * control state 11, which also adds 1000 upward), base + 1 / + 2 for a mostly up / down direction above 888.9, base + 3
 * / + 4 for forward / back above 416.7, else base + 5 or + 6 by side, measured in the bone's frame (mBoneHead, the bone
 * at +0x8AC, mBoneHips). A different animation is started with cRiderAnimBase_play. Called by cRider_updateSplit4.
 *
 * @param vself  the cRider
 */
// Sets the look-layer anim speed from the rider's speed and (on its frame) picks the head/body look anims.
void cRider::updateLookAnims()
{
    cRider* self = (cRider*)this;
    float sideLimit = 416.66668701171875f;
    float upLimit = 888.888916015625f;
    sV_120378 v = Sub_120378(*(sV_120378*)&self->mVelocity, *(sV_120378*)&self->GroundVelocity);
    float speed = Length_120378(v);
    float rate = Clamp_120378(speed / 1666.666748046875f, 0.5f, 2.0f);
    if (cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 3)) {
        cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 3)->TimeScale = rate;
    }
    if (cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 4)) {
        cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 4)->TimeScale = rate;
    }
    if (cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 5)) {
        cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 5)->TimeScale = rate;
    }
    if (AITick() % 6 == self->mAIIndex) {
        int still = speed <= 138.88888549804688f;
        if (self->getControlState() == CS_Handplant) {
            still = 0;
            AddEq_120378(v, sV_120378(0.0f, 0.0f, 1000.0f, 0.0f));
        }
        sV_120378 q;
        LOOK_120378(3, self->mBoneHead, (sLook_120378*)((char*)self + 2368), 411)
        LOOK_120378(4, *(int*)((char*)self + 2220), (sLook_120378*)((char*)self + 2416), 418)
        LOOK_120378(5, self->mBoneHips, (sLook_120378*)((char*)self + 2464), 425)
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_initBoost);
#ifdef SKIP_ASM
/**
 * Sets the rider's starting boost meter (unk_2F8) from its boost mode.
 *
 * The meter starts at mBoostMeterStart, at 0 in boost mode kBOOST_Off, or full (1) in kBOOST_Unlimited.
 */
void cRider::initBoost()
{
    BoostPowerBar = mBoostMeterStart;
    int state = mBoostMode;
    if (state == kBOOST_Off) {
        BoostPowerBar = 0.0f;
    } else if (state == kBOOST_Unlimited) {
        BoostPowerBar = 1.0f;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateLegIK);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);

static inline float riderApproach_00120D90(float v, float t, float s)
{
    if (v > t + s)
        return v - s;
    if (v < t - s)
        return v + s;
    return t;
}

/**
 * Fades the weight at LegIK, probably the leg-IK blend, in or out at 0.05 per frame.
 *
 * It goes toward 0 while the current animation of the rider animation's sequencer 2 (cRiderAnimBase_getCurrentAnimCategory) is
 * 0x14-0x1A, otherwise toward 1.
 */
void cRider::updateLegIK()
{
    int animState = cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 2);
    if (animState >= 20 && animState <= 26)
        LegIK = riderApproach_00120D90(LegIK, 0.0f, 0.05000000447034836f);
    else
        LegIK = riderApproach_00120D90(LegIK, 1.0f, 0.05000000447034836f);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateFocus);
#ifdef SKIP_ASM
/**
 * Moves the rider's focus box to its current position.
 *
 * Copies mPosition into the first quadword of mFocusBox when there is one.
 */
void cRider::updateFocus()
{
    void* focusBox = mFocusBox;
    if (focusBox != 0) {
        *(cQuad128*)focusBox = *(cQuad128*)&mPosition;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateOverlapList);
#ifdef SKIP_ASM
extern "C" void cOverlapList_geteverything(void*, void*);
extern "C" void cRider_updateActiveLights(cRider*);

/**
 * Refreshes the rider's overlap list for its bounding box, then picks its active lights.
 *
 * Calls cOverlapList_geteverything(mOverlapList, &mBoundsMin) (a world sphere-tree query) and cRider_updateActiveLights().
 */
void cRider::updateOverlapList()
{
    cOverlapList_geteverything(mOverlapList, &mBoundsMin);
    updateActiveLights();
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplitMiscFX);
#ifdef SKIP_ASM
extern "C" void cAttachedEmitterFX_update(void*);
extern "C" void cBoltOnFXInterface_updateIfDirty(void*);

/**
 * Updates the rider's attached-emitter FX (cAttachedEmitterFX_update) and bolt-on FX (cBoltOnFXInterface_updateIfDirty).
 */
void cRider::updateSplitMiscFX()
{
    cAttachedEmitterFX_update(mAttachedEmitterFX);
    cBoltOnFXInterface_updateIfDirty(mBoltOnFX);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateHandlerFX);
#ifdef SKIP_ASM
extern "C" void cRiderHandler_updateRiderFX(cRiderHandler* handler);

/**
 * Updates the rider's handler FX: cRiderHandler_updateRiderFX on mRiderHandler. Nothing in the source calls it.
 *
 */
void cRider::updateHandlerFX()
{
    cRiderHandler_updateRiderFX(mRiderHandler);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplitPaused);
#ifdef SKIP_ASM
extern "C" void cRider_updateSkeleton(void* self);

/**
 * Paused-game rider step (On Tour name): orientation, plus animation and skeleton outside an NIS.
 *
 * Rebuilds mMatrix (cRider_updateOrientationImplicit()); unless mIsInNIS, steps the animation with time scale
 * 1.0 and runs the skeleton update cRider_updateSkeleton.
 */
void cRider::updateSplitPaused()
{
    updateOrientationImplicit();
    if (mIsInNIS == 0)
    {
        updateAnimation(1.0f);
        cRider_updateSkeleton(this);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int AITick();
extern "C" bool cPlaybackMan_isRecord(void* obj);
extern "C" void cOverlapList_geteverything(void*, void*);
extern "C" void cRider_updateActiveLights(cRider*);
extern "C" void cRider_SetNISGroundVectors(void* self, void* p);
extern "C" void cRider_updateTerminalVelocity(void* self);

// t4vect_Zero is declared later in the unit as a 4-aligned sVec4; this view gives the lq/sq copy.
extern t4Vector t4vect_Zero_q120F20 __asm__("t4vect_Zero");

static inline int IsState1_120F20(void* obj)
{
    return *(int*)((char*)obj + 0x70) == 1;
}

/**
 * First step of the rider's frame update: overlap list, NIS ground probe and per-frame resets.
 *
 * Clears InTunnel and mShadowRendered. The overlap list and active lights are refreshed on the AI ticks
 * where AITick % 3 equals mAIIndex % 3, and on other ticks too unless the list's word +4 equals the Nash
 * grid's word +0xA0 while the playback manager's state (+0x70) is neither 1 nor 3.
 *
 * In an NIS the ground under mPosition (or the board root bone when unk_AFC is set) is probed with
 * cRider_SetNISGroundVectors. Then mModelOffset and unk_A30 are zeroed, mVisitedIds set to 0xFFFFFFFF, cRider_updateTerminalVelocity
 * called (probably On Tour's updateTerminalVelocity) and, once the rider has finished, mFinishTime advanced
 * by 1/60.
 */
void cRider::updateSplit1()
{
    InTunnel = 0;
    mShadowRendered = 0;
    int frameSlot = AITick() % 3;
    if (frameSlot != mAIIndex % 3)
    {
        cGame* game = SSXApp->Game;
        if (mOverlapList->deletecount == game->BXNash->deletecount)
        {
            void* obj = game->PlaybackMan;
            int busy = 0;
            if (cPlaybackMan_isRecord(obj) || IsState1_120F20(obj))
                busy = 1;
            if (busy == 0)
                goto skip;
        }
    }
    cOverlapList_geteverything(mOverlapList, &mBoundsMin);
    updateActiveLights();
skip:
    if (mIsInNIS)
    {
        char* refPos = (char*)&mPosition;
        if (mNISShowBoardVisualFX)
            refPos = (char*)&mAnimModel->bones[mBoneBoardRoot];
        cRider_SetNISGroundVectors(this, refPos);
    }
    *(&mModelOffset) = t4vect_Zero_q120F20;
    CollideIntersectInfo.Inst = 0;
    mVisitedIds[0] = 0xFFFFFFFF;
    cRider_updateTerminalVelocity(this);
    float finishTime = mFinishTime;
    if (finishTime >= 0.0f)
        mFinishTime = finishTime + 0.01666666753590107f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit2);
#ifdef SKIP_ASM

extern "C" void cRiderHandler_updateControl(void* obj, void* v);

/**
 * Reads the rider's control command and feeds it to its current control state.
 *
 * Calls the handler's getControlCmd virtual (slot 1) into a 16-byte buffer, then
 * cRiderHandler_updateControl() with it.
 */
void cRider::updateSplit2()
{
    int buf[4];
    cRiderHandler* obj = mRiderHandler;
    sVEntry<void(void*, void*)>* vt = (sVEntry<void (void*, void*)>*)obj->mVtbl;
    vt[1].fn(vthis(obj, vt[1]), buf);
    cRiderHandler_updateControl(mRiderHandler, buf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_updateSplit3);
#ifdef SKIP_ASM
extern "C" void cRider_checkForReset(void* rider, int a, int b);
extern "C" void cRider_updateBoost(cRider* self);
extern "C" void cRider_updateLegIK(cRider* self);


// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (v *= s).
static inline void VecScale_1210B0(t4Vector* v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(*v), "=&r"(t)
        : "m"(*v), "f"(s));
}

/**
 * Damps the rider's impact values and auto-resets it after hard hits or long falls.
 *
 * Scales mImpulse by 0.98, mImpactAccum by 0.956 and mImpactAccum2 by 0.97833. Calls
 * cRider_checkForReset(self, 0, 2) when mImpactAccum exceeds 4.5, or while airborne (MS_Air, or a wipeout in
 * its air phase) when the air predictor is in state 1 or 3 with its +0x98 minus +0xA0 above 45, or
 * mImpactAccum2 exceeds 5. Then runs cRider_updateBoost() and cRider_updateLegIK().
 */
void cRider::updateSplit3()
{
    VecScale_1210B0((&mImpulse), 0.9800000190734863f);
    mImpactAccum *= 0.9559999704360962f;
    mImpactAccum2 *= 0.9783333539962769f;
    if (mImpactAccum > 4.5f)
    {
        cRider_checkForReset(this, 0, 2);
    }
    else if (getMotionState() == MS_Air
             || (getMotionState() == MS_Wipeout
                 && mRiderHandler->WipeoutMotion.WipeoutMotionState == cWipeoutMotion::kWMS_Air))
    {
        cAirPredictor* obj = mAirPredictor;
        int ok = 0;
        if (obj->SimState == cAirPredictor::kSS_SimDone || obj->SimState == cAirPredictor::kSS_SimCheckObjectsDone)
            ok = 1;
        obj = (cAirPredictor*)mAirPredictor;
        if ((ok && obj->TotalAirTime - obj->CurAirTime > 45.0f) || mImpactAccum2 > 5.0f)
            cRider_checkForReset(this, 0, 2);
    }
    updateBoost();
    updateLegIK();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_updateSplit4);
#ifdef SKIP_ASM
extern "C" void cRider_updateLookAnims(cRider* self);

static inline void approach_11F8(sRiderChannel* riderChannel)
{
    float res;
    float target = riderChannel->target;
    float rate = riderChannel->rate;
    float cur = riderChannel->cur;
    if (target + rate < cur)
        res = cur - rate;
    else if (cur < target - rate)
        res = cur + rate;
    else
        res = target;
    riderChannel->cur = res;
}

/**
 * Runs the rider's look controllers, then moves all 19 slewed channels toward their targets.
 *
 * cRider_updateLookAnims updates the head and upper-body look controllers; each mSlewed channel then steps cur toward
 * target by at most rate.
 */
void cRider::updateSplit4()
{
    this->updateLookAnims();
    approach_11F8(&mSlewed[0]);
    approach_11F8(&mSlewed[1]);
    approach_11F8(&mSlewed[2]);
    approach_11F8(&mSlewed[3]);
    approach_11F8(&mSlewed[4]);
    approach_11F8(&mSlewed[5]);
    approach_11F8(&mSlewed[6]);
    approach_11F8(&mSlewed[7]);
    approach_11F8(&mSlewed[8]);
    approach_11F8(&mSlewed[9]);
    approach_11F8(&mSlewed[10]);
    approach_11F8(&mSlewed[11]);
    approach_11F8(&mSlewed[12]);
    approach_11F8(&mSlewed[13]);
    approach_11F8(&mSlewed[14]);
    approach_11F8(&mSlewed[15]);
    approach_11F8(&mSlewed[16]);
    approach_11F8(&mSlewed[17]);
    approach_11F8(&mSlewed[18]);
}
#endif

extern "C" void* cRiderHandler_stepMotion(void* obj);

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit5__FPv);
#ifdef SKIP_ASM
/**
 * Steps the rider's motion: cRiderHandler_stepMotion on mRiderHandler.
 *
 * cAI_update calls it for every rider.
 *
 * @return cRiderHandler_stepMotion's result
 */
void* cRider::updateSplit5()
{
    return cRiderHandler_stepMotion(mRiderHandler);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit6);
#ifdef SKIP_ASM
extern "C" void cRider_updateAnimation(cRider* self, float value);

/**
 * Steps the rider's animation by its time scale (mTimeScale), unless it is in an NIS.
 */
void cRider::updateSplit6()
{
    if (mIsInNIS == 0)
    {
        updateAnimation(mTimeScale);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit7);
#ifdef SKIP_ASM
extern "C" void cRider_updateSkeleton(void* self);

/**
 * Runs the rider's skeleton update (cRider_updateSkeleton), unless it is in an NIS.
 *
 * cRider_updateSkeleton transforms the anim model's bones (cAnimModel_transform) with the lean pose adjustment
 * (cRider_doLeanPoseAdjust).
 */
void cRider::updateSplit7()
{
    if (mIsInNIS == 0)
    {
        cRider_updateSkeleton(this);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit8);
#ifdef SKIP_ASM
extern "C" void cRiderHandler_resolveMotion(void* p);
extern "C" void cAnimModel_applyOffset(void* model, void* v);

struct sVec4_00121750
{
    float x, y, z, w;
};
extern sVec4_00121750 t4vect_Zero;

static inline int VecChanged_00121750(cRider* self, const sVec4_00121750& b)
{
    return self->mModelOffset.x != b.x || self->mModelOffset.y != b.y
        || self->mModelOffset.z != b.z || self->mModelOffset.w != b.w;
}

/**
 * Resolves the rider's motion, then applies the frame's model offset to its animation model.
 *
 * Calls cRiderHandler_resolveMotion(); when mModelOffset is not zero, cAnimModel_applyOffset() moves the
 * bones and skeleton matrices by it.
 */
void cRider::updateSplit8()
{
    cRiderHandler_resolveMotion(mRiderHandler);
    if (VecChanged_00121750(this, t4vect_Zero))
    {
        cAnimModel_applyOffset(mAnimModel, &mModelOffset);
    }
}
#endif

extern "C" void* cAnimModel_buildSkelMatrices(void* obj);

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit9__FPv);
#ifdef SKIP_ASM
/**
 * Ninth slice of the rider's per-frame update: builds the skeleton matrices.
 *
 * cAnimModel_buildSkelMatrices(mAnimModel). cAI_update runs each updateSplitN over all riders in turn.
 *
 * @return cAnimModel_buildSkelMatrices's value (On Tour's updateSplit9 returns void)
 */
void* cRider::updateSplit9()
{
    return cAnimModel_buildSkelMatrices(mAnimModel);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit10);
#ifdef SKIP_ASM
extern void* cWScriptMan_TheWScriptMan;
extern "C" void cWScriptMan_doOnCollideScript(void* mgr, void* a, void* b, void* c);
void cRider_updateTestSurface(void* self);
extern "C" void cRider_updateTrackPath(void* self);
extern "C" void cRider_updateAIPath(void* self);
extern "C" bool cRider_updateFinishTest(cRider* self);
extern "C" void cRiderMetrix_update(void* metrix);


/**
 * Runs the rider's object script hook, then its path, finish, metrix and focus updates.
 *
 * When the rider collided with an instance (CollideIntersectInfo.Inst), calls its active node's onCollide with
 * CollideInfo, CollideIntersectInfo and the rider, or, without a node, the world-script manager's
 * cWScriptMan_doOnCollideScript with the same arguments. Then cRider_updateTestSurface (empty),
 * cRider_updateTrackPath (outside an NIS), cRider_updateAIPath, cRider_updateFinishTest(), the metrix update cRiderMetrix_update
 * (outside an NIS) and cRider_updateFocus().
 */
void cRider::updateSplit10()
{
    tInstanceRecord* collideInst = CollideIntersectInfo.Inst;
    if (((char*)collideInst) != 0)
    {
        cInstanceNode* node = collideInst->m_pActiveNode;
        if (node != 0)
        {
            cBeing& being = *this;
            node->onCollide(&CollideInfo, &CollideIntersectInfo, &being);
        }
        else
        {
            cWScriptMan_doOnCollideScript(cWScriptMan_TheWScriptMan, (char*)&CollideInfo, (char*)&CollideIntersectInfo,
                    (char*)&VPTR_AT(this, sizeof(cRiderState)));
        }
    }
    this->updateTestSurface();
    if (mIsInNIS == 0)
    {
        cRider_updateTrackPath(this);
    }
    cRider_updateAIPath(this);
    updateFinishTest();
    if (mIsInNIS == 0)
    {
        cRiderMetrix_update(mMetrix);
    }
    updateFocus();
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit11);
#ifdef SKIP_ASM
extern "C" int cSectionMan_getBESectionIDFromWorldSectionID(void* self, int a1);
extern "C" void cRenderStateMan_update(int index, float a, float b, float c);
extern "C" void cPlayerLights_Clear(void* p);


/**
 * Updates the rider's ground section and lens FX position, and clears its handler lights.
 *
 * When mGroundPolyId is valid, the BE section id of its world section (its low byte) is stored in
 * mGroundMaterial, so that field holds a section id here. Then calls cRenderStateMan_update(AI index, contact x,
 * contact y, -99999) (lens FX, unknown) and clears the handler's cPlayerLights (cPlayerLights_Clear).
 */
void cRider::updateSplit11()
{
    if (~mGroundPolyId != 0)
        mGroundMaterial = cSectionMan_getBESectionIDFromWorldSectionID(SSXApp->Game->SectionMan,
                (unsigned char)mGroundPolyId);
    cRenderStateMan_update(getAIIndex(), mGroundContactPoint.x, mGroundContactPoint.y, -99999.0f);
    cRiderHandler* handler = mRiderHandler;
    cPlayerLights_Clear(handler ? (char*)&handler->PlayerLights : 0);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit12);
#ifdef SKIP_ASM
extern "C" int cRider_isGroundNormalValid(cRider* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_UpdateAudioWorldPainter(void* mgr, void* rider);
extern "C" float func_0031C228(float x);
extern "C" void cRider_updateDangerLevel(cRider* self);


// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_121950(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Length_121950(const t2Vector& v)
{
    return Sqrt_121950(v.x * v.x + v.y * v.y);
}

static inline float Atan2_121950(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f)
            return y;
        float a = -1.5707963705062866f;
        if (y >= 0.0f)
            a = 1.5707963705062866f;
        return a;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

/**
 * Updates the rider's audio world painter and its smoothed slope angle, then its danger level.
 *
 * While the ground normal is valid, mSmoothedHeading moves 15% of the way (difference wrapped to +-pi) toward
 * the slope angle atan2(|n.xy|, n.z). Then cRider_updateDangerLevel().
 */
void cRider::updateSplit12()
{
    cSSXAudio_UpdateAudioWorldPainter(cAudioInterface_bxAudio(), this);
    if (isGroundNormalValid())
    {
        t2Vector normalXY(mGroundNormal.x, mGroundNormal.y);
        float z = mGroundNormal.z;
        float ang = Atan2_121950(Length_121950(normalXY), z);
        float headingDelta = ang - mSmoothedHeading;
        if (headingDelta > 3.1415927410125732f)
            headingDelta -= 6.2831854820251465f;
        else if (headingDelta < -3.1415927410125732f)
            headingDelta += 6.2831854820251465f;
        mSmoothedHeading += headingDelta * 0.15000000596046448f;
    }
    updateDangerLevel();
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_alignRiderAxis);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

// 16-byte vectors with inline constructors (non-POD siblings of t4Vector). The classes differ only in the order
// their 4-float constructor stores the components; that order decides the float-register allocation of every
// caller (merging them changes code), so they are kept apart.
// sVec4 stores x, y, z, w.
// sVec4W stores w first, then x, y, z.
extern t4Vector t4vect_Zero_1AA0 __asm__("t4vect_Zero");
extern t4Vector t4vect_XAxis;
extern "C" void tquat_prerotateto(void* q, const void* a, const void* b);
extern "C" float func_0031C128(float x);
extern "C" float fSin(float x);


// PORT: PS2-only VU0 macro-mode asm (rotate v by the conjugate of quaternion q).
static inline sVec4 QuatRot_1AA0(const sVec4W& q, const sVec4& v)
{
    sVec4 r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vsub.w     $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmsubaw.xyz ACC, $vf6, $vf4w\n"
        "vmsubaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sVec4W QuatMul_1AA0(const sVec4W& a, const sVec4W& b)
{
    sVec4W r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline sVec4 Div_1AA0(const sVec4& v, float s)
{
    sVec4 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

static inline sVec4W AxisAngle_1AA0(const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sVec4W(s * axis.x, s * axis.y, s * axis.z, c);
}

// q = rotation(axis, angle) * q. Declared to return the quaternion by value but has no return statement (the classic
// forgotten return); the caller ignores the result, and that ignored by-value result is the unwritten 16-byte temp the
// retail frame keeps during the heading rotation. The final rotation below is written out instead (99.19 with this
// helper).
static inline sVec4W Rotate_1AA0(sVec4W& q, const sVec4& axis, float angle)
{
    q = QuatMul_1AA0(AxisAngle_1AA0(axis, angle), q);
}

static inline float Atan2_1AA0(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f)
            return y;
        float a = -1.5707963705062866f;
        if (y >= 0.0f)
            a = 1.5707963705062866f;
        return a;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

static inline float Acos_1AA0(float x)
{
    return 1.5707963705062866f - func_0031C128(x);
}

#define ROT_1AA0 (*(sVec4W*)&self->mRotation)

static inline sVec4W Conj_1AA0(cRider* self)
{
    sVec4W r(-ROT_1AA0.x, -ROT_1AA0.y, -ROT_1AA0.z, ROT_1AA0.w);
    return r;
}

static inline int neq_1AA0(const sVec4* a, const t4Vector& b)
{
    return a->x != b.x || a->y != b.y || a->z != b.z || a->w != b.w;
}

/**
 * Turns the rider's orientation toward a desired up axis and heading, limited per frame.
 *
 * The target is mRotation turned so its up axis lies on @p up and, when @p fwd is not zero, turned about @p
 * up to face @p fwd. The rotation from the current orientation to the target is applied by min(maxRate, angle
 * x rate) / 60 radians about its axis (the X axis below 1e-5), then mMatrix is rebuilt.
 *
 * @param up  desired up axis (On Tour: desUp)
 * @param fwd  desired front direction, or the zero vector to keep the heading (On Tour: desFront)
 * @param rate  fraction of the remaining angle to turn per second (On Tour: rateFactor)
 * @param maxRate  turn-rate limit in radians per second
 */
extern "C" void cRider_alignRiderAxis(cRider* self, const sVec4* up, const sVec4* fwd, float rate, float maxRate)
{
    sVec4W targetRot = ROT_1AA0;
    {
        sVec4 cur(2.0f * (ROT_1AA0.z * ROT_1AA0.x + ROT_1AA0.w * ROT_1AA0.y), 2.0f * (ROT_1AA0.z * ROT_1AA0.y - ROT_1AA0.w * ROT_1AA0.x),
                    1.0f - 2.0f * (ROT_1AA0.x * ROT_1AA0.x + ROT_1AA0.y * ROT_1AA0.y), 0.0f);
        tquat_prerotateto(&targetRot, &cur, up);
    }
    if (neq_1AA0(fwd, t4vect_Zero_1AA0)) {
        sVec4 rotatedFwd = QuatRot_1AA0(targetRot, *fwd);
        float heading = -Atan2_1AA0(rotatedFwd.x, rotatedFwd.y);
        Rotate_1AA0(targetRot, *up, heading);
    }
    sVec4W deltaRot = QuatMul_1AA0(targetRot, Conj_1AA0(self));
    float angle;
    t4Vector axis;
    if (0.0f < deltaRot.w) {
        angle = 2.0f * Acos_1AA0(deltaRot.w <? 1.0f);
        if (9.999999747378752e-06f <= angle) {
            axis = Div_1AA0(sVec4(deltaRot.x, deltaRot.y, deltaRot.z, 0.0f), fSin(angle * 0.5f));
            goto have_axis;
        }
    } else {
        angle = 2.0f * Acos_1AA0(-(deltaRot.w >? -1.0f));
        if (9.999999747378752e-06f <= angle) {
            axis = Div_1AA0(sVec4(deltaRot.x, deltaRot.y, deltaRot.z, 0.0f), fSin(angle * -0.5f));
            goto have_axis;
        }
    }
    axis = t4vect_XAxis;
have_axis:
    angle = (maxRate <? angle * rate) * 0.01666666753590107f;
    deltaRot = AxisAngle_1AA0(axis, angle);
    ROT_1AA0 = QuatMul_1AA0(deltaRot, ROT_1AA0);
    self->updateOrientationImplicit();
}
#undef ROT_1AA0
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateLOD);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);



// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4x4 matrix * vector).
static inline t4Vector mtxMulVec_121F30(t4Vector* m, t4Vector* v)
{
    t4Vector r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(*v), "r"(m)
        : "memory");
    return r;
}

/**
 * Picks the rider's mLOD from its distance in front of the camera.
 *
 * Transforms the rider's position (virtual slot 5) by GrMan's view matrix (slot 35), scales the depth by tan(fov) (slot
 * 29) and 0.01: below 6 gives LOD 0, below 10 LOD 1, else 2. Called by cRider_renderBody.
 */
void cRider::updateLOD()
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    t4Vector* viewMtx = (t4Vector*)ps2GraphicsMan->GetView();
    t4Vector* pos = getPosition();
    t4Vector viewPos = mtxMulVec_121F30(viewMtx, pos);
    float z = viewPos.z;
    cPS2GraphicsMan* graphicsMan = GrMan;
    float fov = graphicsMan->GetViewportFOV();
    float sinFov, cosFov;
    fSinCos(&sinFov, &cosFov, fov);
    z = z * (sinFov / cosFov);
    z = z * 0.009999999776482582f;
    if (z < 6.0f)
    {
        mLOD = 0;
    }
    else
    {
        int lod = 2;
        if (z < 10.0f) lod = 1;
        mLOD = lod;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", setDefaultIrradiance);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Builds a 0xA0-byte record with cIrradiance_buildDefault and passes it to virtual slot 69 of the object at GrMan.
extern "C" void cIrradiance_buildDefault(void* rec, int flags);

/**
 * Builds a cleared 160-byte light record (cIrradiance_buildDefault with 0) and hands it to GrMan's virtual slot 69 with 0.
 *
 * Nothing in the source calls it.
 */
extern "C" void setDefaultIrradiance()
{
    char rec[160];
    cIrradiance_buildDefault(rec, 0);
    GrMan->SetIrradiance(*(const tIrradianceInfo*)rec, 0);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateActiveLights);
#ifdef SKIP_ASM
extern "C" void cWorldLightMan_IntersectLights(void* a, void* q, void* b, void* c, int d);

/**
 * Picks the world lights that light the rider into its active-light list (unk_794).
 *
 * cWorldLightMan_IntersectLights keeps the 8 best-scoring lights of type 6 among those of the overlap list, scored from the
 * rider's hip position.
 */
void cRider::updateActiveLights()
{
    cQuad128 hipPos;
    cRider_getHipPosition(&hipPos, this);
    cWorldLightMan_IntersectLights(SSXApp->Game->WorldLightMan, &hipPos, mOverlapList, ActiveLights, 8);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_SetRiderLights);
#ifdef SKIP_ASM
// tRGB with the constructor the light calls build their colour argument with.
struct tRGBColour : tRGB {
    tRGBColour(float r, float g, float b)
    {
        R = r;
        G = g;
        B = b;
    }
};
extern "C" void cIrradiance_Clear(void* p);
extern "C" void* cRenderStateMan_getIrradiance(int index);
extern "C" float cRenderStateMan_RimLightIntensityOffset(int index);
extern "C" void* cRider_getHipPosition(void* dst, cRider* self);
extern "C" void cIrradiance_addSunTerm(void* m, void* q, float f);
extern "C" void cIrradiance_addWorldLight(void* m, void* q, void* p);
extern "C" void addVec3InPlace(void* m, tRGB* v);
extern "C" void cIrradiance_addLightFromDirection(void* m, t4Vector* q, tRGB* v);

/**
 * Builds the rider's light block (PlayerIrradiance) from its environment, active world lights and handler lights.
 *
 * Clears the 10-quadword block, copies the rider's lens-table entry (cRenderStateMan_getIrradiance) to its start, adds
 * cIrradiance_addSunTerm's term at the hip position with cRenderStateMan_RimLightIntensityOffset's value, adds each active light of unk_794
 * (cIrradiance_addWorldLight), then the handler cPlayerLights' extra ambient (addVec3InPlace) and each of its directional
 * lights (cIrradiance_addLightFromDirection).
 */
void cRider::SetRiderLights()
{
    t4Vector hipPos;
    cIrradiance_Clear(&PlayerIrradiance);
    {
        (cIrradiance&)PlayerIrradiance = *(cIrradiance*)cRenderStateMan_getIrradiance(getAIIndex());
    }
    cRider_getHipPosition(&hipPos, this);
    tIrradianceInfo* lightState = &PlayerIrradiance;
    {
        cIrradiance_addSunTerm(lightState, &hipPos, cRenderStateMan_RimLightIntensityOffset(getAIIndex()));
    }
    for (int i = 0; i < 8; i++) {
        void* light = ActiveLights[i];
        if (light != 0)
            cIrradiance_addWorldLight(&PlayerIrradiance, &hipPos, light);
    }
    cRiderHandler* x = mRiderHandler;
    cPlayerLights* playerLights = x ? &x->PlayerLights : 0;
    if (playerLights != 0) {
        {
            tRGBColour lightColour(playerLights->mExtraAmbientLight.R, playerLights->mExtraAmbientLight.G,
                                   playerLights->mExtraAmbientLight.B);
            addVec3InPlace(&PlayerIrradiance, &lightColour);
        }
        for (int j = 0; j < playerLights->mNumDirectionalLights; j++) {
            t4Vector* qq = &playerLights->mDirectionalLightDirections[j];
            tRGBColour lightColour(playerLights->mDirectionalLightColours[j].R,
                                   playerLights->mDirectionalLightColours[j].G,
                                   playerLights->mDirectionalLightColours[j].B);
            cIrradiance_addLightFromDirection(&PlayerIrradiance, qq, &lightColour);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_renderClip);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallmsr/cfc2); the PC port needs a C version.
extern char D_828[];

struct sMat_122278 { float m[16]; };
struct sVE_122278 {
    short delta;
    short index;
    void* fn;
};
typedef sMat_122278 (*GetMatFn_122278)(void*);
typedef int (*TestFn_122278)(void*, void*, void*, sMat_122278*);

static inline int inFrustum_122278(t4Vector* v)
{
    cOcclusionMan* inst = SSXApp->Game->OcclusionMan;
    int mask = (int)inst->m_uiVisMask;
    for (int i = 0; i < inst->m_iNumActive; i++) {
        int bit = 1 << i;
        if (mask & bit) {
            t4Vector* m = (t4Vector*)(&inst->m_InView[i].pVis->m_fvPlaneCoeffs);
            int r;
            __asm__ __volatile__(
                ".set push\n"
                ".set noreorder\n"
                "lqc2      $vf1, %1\n"
                "lqc2      $vf2, %2\n"
                "lqc2      $vf3, %3\n"
                "lqc2      $vf4, %4\n"
                "lqc2      $vf5, %5\n"
                "lqc2      $vf6, %6\n"
                "ctc2.ni   %7, $vi27\n"
                "vnop\n"
                "vnop\n"
                "vcallmsr  $vi27\n"
                "cfc2.i    %0, $vi1\n"
                ".set pop\n"
                : "=&r"(r)
                : "m"(m[0]), "m"(m[1]), "m"(m[2]), "m"(m[3]), "m"(m[4]), "m"(*v), "r"((int)D_828 >> 3)
                : "memory");
            int ok = r == 1;
            if (ok)
                return 1;
        }
    }
    return 0;
}

static inline int test_122278(void* a, void* b)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVE_122278* vt = (sVE_122278*)VPTR_OF(cGraphicsMan, ps2GraphicsMan);
    char* obj = (char*)ps2GraphicsMan + vt[93].delta;
    sMat_122278 m = ((GetMatFn_122278)vt[43].fn)((char*)ps2GraphicsMan + vt[43].delta);
    return ((TestFn_122278)vt[93].fn)(obj, a, b, &m);
}

/**
 * Decides whether the rider is on screen this frame and stores the answer in mIsOnScreen.
 *
 * Off screen when mRenderFlag is clear, the rider is not loaded or unk_AD0 is set. Otherwise on screen unless
 * the graphics manager's box test (virtual slot 93, ClipBoundBox) puts the bounding box outside the view
 * (GM_CLIPCODE_OUTSIDE); with
 * mUseRootPoseOverride the handler's box (+0x80, +0x90) is tried too, and without it a bounding sphere hidden
 * by an occluder of the occlusion manager counts as off screen.
 *
 * @return the new mIsOnScreen: 1 on screen, 0 not
 */
int cRider::renderClip()
{
    if (mRenderFlag == 0 || mLoadStep != kLoadStepDone || mHiddenNIS != 0) {
        mIsOnScreen = 0;
        return 0;
    }
    mIsOnScreen = test_122278(&mBoundsMin, &mBoundsMax) != GM_CLIPCODE_OUTSIDE;
    if (mUseRootPoseOverride != 0) {
        if (mIsOnScreen == 0) {
            cRiderHandler* handler = mRiderHandler;
            mIsOnScreen = test_122278(&handler->WipeoutMotion.BoardIntersectBox,
                    &handler->WipeoutMotion.BoardIntersectBox.MaxPoint) != GM_CLIPCODE_OUTSIDE;
        }
    }
    if (mUseRootPoseOverride == 0) {
        if (inFrustum_122278((&mBoundSphere)))
            mIsOnScreen = 0;
    }
    return mIsOnScreen;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_renderBody);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRider_updateLOD(cRider* self);
extern "C" void cAnimModel_render(void* model, int lod, int a, int b);

struct sVec4_122448 { float v[4]; };
extern sVec4_122448 t4vect_Zero_v122448 __asm__("t4vect_Zero");


/**
 * Draws the rider's model when it is on screen, then updates the low-detail flag unk_B1C.
 *
 * Picks mLOD (cRider_updateLOD), loads the rider's lights (graphics slot 69 with PlayerIrradiance), sets bits 2-6 of the
 * draw state's word 1 to 5 and draws mAnimModel between two graphics slot-27 calls (with mPosition, then the
 * zero vector). The board and player additive flags come from the handler's cPlayerLights, or uber level 11
 * and up. When the view manager's word +0x14 is 0 or unk_B1C is set, unk_B1C becomes 1 if the rider is off
 * screen or at LOD 2.
 */
void cRider::renderBody()
{
    if (mIsOnScreen)
    {
        cRiderHandler* handler = mRiderHandler;
        cPlayerLights* playerLights = (cPlayerLights*)(handler ? (char*)&handler->PlayerLights : 0);
        int additivePlayer = 0;
        if (playerLights->mPlayerIsAdditive || mUberLevel >= 11)
            additivePlayer = 1;
        int additiveBoard = 0;
        if (playerLights->mBoardIsAdditive || mUberLevel >= 11)
            additiveBoard = 1;
        this->updateLOD();
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEntry<void(void*, void*, int)>* vt = (sVEntry<void(void*, void*, int)>*)VPTR_OF(cGraphicsMan, ps2GraphicsMan);
        vt[69].fn(vthis(ps2GraphicsMan, vt[69]), &PlayerIrradiance, 1);
        cPS2GraphicsMan* graphicsMan = GrMan;
        int* rs = (int*)graphicsMan->m_pCurrentDrawState;
        rs[1] = (rs[1] & ~0x7C) | 0x14;
        sVEntry<void(void*, float, void*)>* vt2 = (sVEntry<void(void*, float, void*)>*)VPTR_OF(cGraphicsMan, graphicsMan);
        vt2[27].fn(vthis(graphicsMan, vt2[27]), 0.0f, &mPosition);
        cAnimModel_render(mAnimModel, mLOD, additiveBoard, additivePlayer);
        cPS2GraphicsMan* grManReset = GrMan;
        sVEntry<void(void*, float, void*)>* vt3 = (sVEntry<void(void*, float, void*)>*)VPTR_OF(cGraphicsMan, grManReset);
        vt3[27].fn(vthis(grManReset, vt3[27]), 0.0f, &t4vect_Zero_v122448);
    }
    if ((int)SSXApp->Game->ViewMan->m_uiCurrentView == 0 || mUpdateMinAnimOnly)
    {
        int* lowDetailFlag = &mUpdateMinAnimOnly;
        int lowDetail = 0;
        if (mIsOnScreen == 0 || mLOD == kLOD_Far)
            lowDetail = 1;
        *lowDetailFlag = lowDetail;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_renderLights);
#ifdef SKIP_ASM
extern "C" void cRider_SetRiderLights(cRider*);

/**
 * Builds the rider's lights (cRider_SetRiderLights) when it is on screen.
 *
 * @return always 1
 */
int cRider::renderLights()
{
    if (mIsOnScreen != 0) {
        SetRiderLights();
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_renderShadow);
#ifdef SKIP_ASM
extern "C" void cRider_RenderShadow(cRider* self);

/**
 * Draws the rider's shadow (cRider_RenderShadow) when it is on screen.
 */
void cRider::renderShadow()
{
    if (mIsOnScreen != 0)
    {
        this->RenderShadow();
    }
}
#endif

extern "C" void* cAttachedEmitterFX_render(void* obj);

//100%
INCLUDE_ASM("ai/rider", cRider_renderFX);
#ifdef SKIP_ASM
extern "C" void cRiderHandler_renderRiderFX(cRiderHandler* handler);

/**
 * Renders the rider's handler FX: cRiderHandler_renderRiderFX on mRiderHandler. Nothing in the source calls it.
 *
 */
void cRider::renderFX()
{
    cRiderHandler_renderRiderFX(mRiderHandler);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_renderAttachedEmitterFX__FPv);
#ifdef SKIP_ASM
/**
 * Renders the rider's attached emitter FX: cAttachedEmitterFX_render on mAttachedEmitterFX.
 *
 * cAI_render calls it for every rider last.
 *
 * @return cAttachedEmitterFX_render's result
 */
void* cRider::renderAttachedEmitterFX()
{
    return cAttachedEmitterFX_render(mAttachedEmitterFX);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_flushActiveLights);
#ifdef SKIP_ASM
// MEM_fill looks like memset(dst, value, size).
extern "C" void* MEM_fill(void* dst, int value, int size);

/**
 * Clears the rider's active-light list (the 8 light pointers at unk_794).
 */
void cRider::flushActiveLights()
{
    MEM_fill(ActiveLights, 0, sizeof(ActiveLights));
}
#endif

//100%
INCLUDE_ASM("ai/rider", drawEmptyHud2DPass);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: an empty ASSERT macro (`ASSERT_2680(SSXApp != 0)`, `do { } while (0)`) at entry, an unused `int unused[4]`
// (frame +0x10), and the (0, 0) store to o->f28 through the layout-identical sV2v_2680 whose by-value ctor stores y
// before x; plain form 99.17% (99.25% / 99.94% / 99.98% without each alone). Byte-identical and behaves like the plain
// form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct sGsState_2680 {
    unsigned int w0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 5;
    unsigned int f4_12 : 8;
    unsigned int f4_20 : 2;
    unsigned int f4_22 : 1;
    unsigned int f4_23 : 2;
    unsigned int f4_25 : 7;
    unsigned int f8_0 : 5;
    unsigned int f8_5 : 5;
    unsigned int f8_10 : 19;
    unsigned int f8_29 : 3;
    unsigned int wC;
    int f10 : 16;
    unsigned int f10hi : 16;
};

// The graphics manager (EA name cGraphicsMan_2680, from the prototype); D_004A289C_2680 is the instance.
class cGraphicsMan_2680 {
public:
    char pad[0xE84];
    sGsState_2680* stateTop;
    char padE88[0x10D8 - 0xE88];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void PushViewport();
    virtual void PopViewport();
    virtual void v22(); virtual void v23(); virtual void v24();
    virtual void SetViewportOrthographic(float, float, float, float, int, int, float, float);
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void PushView();
    virtual void PopView();
    virtual void v32();
    virtual void SetView(void*);
};




extern sGsState_2680 sDrawState_s_defaultDrawState;
extern t4Matrix t4mat_Identity;
extern int cRenderToggles_m_DisableHUD;
// Release build: the assert compiles to nothing (gcc still emits its loop notes).
#define ASSERT_2680(cond) do { } while (0)


// PORT: second vec2 temp is built by a by-value ctor.
struct sV2v_2680 {
    float x, y;
    sV2v_2680(float ax, float ay) { y = ay; x = ax; }
};


/**
 * Opens and closes a 640x480 2D pass with the default draw state, drawing nothing, unless the HUD is disabled.
 *
 * Unless cRenderToggles_m_DisableHUD is set: copies the DebugFont object's +0x30 pair to +0x38, calls GrMan's
 * PushViewport and PushView, pushes sDrawState_s_defaultDrawState onto the draw-state stack, sets the viewport and
 * texture, two draw-state fields and the DebugFont's +0x28 pair (2, 2, then 0, 0), and ends with PopView, PopViewport and a
 * stack pop. Nothing in the source calls it.
 */
extern "C" void drawEmptyHud2DPass(void)
{
    ASSERT_2680(SSXApp != 0);
    if (cRenderToggles_m_DisableHUD == 0) {
        cFont* debugFont = SSXApp->Game->DebugFont;
        (*(t2Vector*)&debugFont->Scale[0]) = (*(t2Vector*)&debugFont->BaseScale[0]);
        GrMan->PushViewport();
        GrMan->PushView();
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        ((cGraphicsMan_2680*)ps2GraphicsMan)->stateTop[1] = ((cGraphicsMan_2680*)ps2GraphicsMan)->stateTop[0];
        ((cGraphicsMan_2680*)ps2GraphicsMan)->stateTop++;
        *((cGraphicsMan_2680*)GrMan)->stateTop = sDrawState_s_defaultDrawState;
        *GrMan->m_pCurrentDrawState->m_textureState.m_texid = -1;
        GrMan->SetViewportOrthographic(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f, 0, 0);
        GrMan->SetView1(t4mat_Identity);
        ((cGraphicsMan_2680*)GrMan)->stateTop->f8_5 = 24;
        ((cGraphicsMan_2680*)GrMan)->stateTop->f4_2 = 5;
        (*(t2Vector*)&debugFont->ShadowOffset[0]) = t2Vector(2.0f, 2.0f);
        *(sV2v_2680*)&(*(t2Vector*)&debugFont->ShadowOffset[0]) = sV2v_2680(0.0f, 0.0f);
        GrMan->PopView();
        GrMan->PopViewport();
        ((cGraphicsMan_2680*)GrMan)->stateTop--;
        int framePad[4]; // frame +16 (see FAKEMATCH above)
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_RenderShadow);
#ifdef SKIP_ASM
struct sV4_2898 {
    float x, y, z, w;
    sV4_2898() {}
    sV4_2898(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));



struct sRS_2898 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
};

struct sObj_2898 {
    char pad0[0x110];
    t4Matrix mat;                  // 0x110
    sV4_2898 f150;                  // 0x150
    sV4_2898 f160;                  // 0x160
    void* f170;                     // 0x170
};

struct sVEI_2898 { short delta; short index; int (*fn)(void*); };
struct sVEI3_2898 { short delta; short index; int (*fn)(void*, void*, const sV4_2898&); };
struct sVE3_2898 { short delta; short index; void (*fn)(void*, void*, void*); };
struct sVE4_2898 { short delta; short index; void (*fn)(void*, void*, void*, void*); };

extern int D_004A4C00;
extern t3Vector cRider_RenderShadow_lnorm;

extern "C" void func_00374D00(void* obj, t3Vector* a, t3Vector* b, t3Vector* dir, t4Matrix* bones, int f);
extern "C" void cAnimModel_rendershadow(void* model, int lod, sV4_2898* a, sV4_2898* b);
extern "C" void cPlayerShadow_AddShadow(void* obj);
int cRider_getMotionState(void*);

// PORT: PS2-only VU0 inline asm (vector add).
static inline sV4_2898 Add_2898(const sV4_2898& a, const sV4_2898& b)
{
    sV4_2898 r;
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV4_2898 Scale_2898(const sV4_2898& v, float s)
{
    sV4_2898 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void CopyMat_2898(t4Matrix* d, t4Matrix* s)
{
    __asm__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

static inline void Set3_2898(t3Vector& v, float x, float y, float z)
{
    v.x = x;
    v.y = y;
    v.z = z;
}

static inline cAnimModel* Model_2898(cRider* rider) { return rider->mAnimModel; }
static inline sObj_2898* Shadow_2898(cRider* rider) { return (sObj_2898*)rider->mShadow; }
// Translation row of bone i's 0x40-byte matrix.
static inline sV4_2898 BonePos_2898(cRider* rider, int i) { return *(sV4_2898*)((char*)Model_2898(rider)->SkelMatrices + (i << 6) + 0x30); }

static inline void* Tex_2898(sObj_2898* o) { return o->f170; }

static inline sRS_2898*& top_2898(void* g) { return *(sRS_2898**)((char*)g + 0xE84); }

/**
 * Renders the rider's projected shadow texture once per frame, and draws the shadow.
 *
 * On the frame's first call (mShadowRendered clear): pushes a draw state, sets up the projection (func_00374D00)
 * centred between bones unk_918 and unk_8E8, lit straight down, with the rider's up axis, renders the model's
 * shadow pass (cAnimModel_rendershadow at LOD 3; board colour grey 0.3, black on the ground; shadow colour grey 0.3)
 * through graphics slots 31, 53, 55, 101, 54 and 32, and pops the state. The shadow itself is drawn on every
 * call (cPlayerShadow_AddShadow).
 */
void cRider::RenderShadow()
{
    if (mShadowRendered == 0) {
        mShadowRendered = 1;
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        top_2898(ps2GraphicsMan)[1] = top_2898(ps2GraphicsMan)[0];
        top_2898(ps2GraphicsMan)++;
        t3Vector center;
        {
            sV4_2898 midpoint = Scale_2898(Add_2898(BonePos_2898(this, LeftLeg.FootSkelIndex), BonePos_2898(this,
                    RightLeg.FootSkelIndex)), 0.5f);
            center.x = midpoint.x;
            center.y = midpoint.y;
            center.z = midpoint.z;
        }
        if (D_004A4C00 == 0) {
            Set3_2898(cRider_RenderShadow_lnorm, 0.0f, 0.0f, -1.0f);
            D_004A4C00 = 1;
        }
        t3Vector objUp;
        {
            sV4_2898 upRow = *(sV4_2898*)&mMatrix.row[2];
            objUp.x = upRow.x;
            objUp.y = upRow.y;
            objUp.z = upRow.z;
        }
        float k = 0.3f;
        func_00374D00(Shadow_2898(this), &center, &objUp, &cRider_RenderShadow_lnorm, Model_2898(this)->SkelMatrices,
                0);
        cPS2GraphicsMan* grManPushView = GrMan;
        top_2898(grManPushView)->f8 = (top_2898(grManPushView)->f8 & ~0x3E0) | 0x20;
        top_2898(grManPushView)->f4 = (top_2898(grManPushView)->f4 & 0xFE7FFFFF) | 0x1000000;
        top_2898(grManPushView)->f4 = top_2898(grManPushView)->f4 & 0xFFCFFFFF;
        top_2898(grManPushView)->f4 = (top_2898(grManPushView)->f4 & 0xFFF00FFF) | 0x14000;
        top_2898(grManPushView)->f4 = (top_2898(grManPushView)->f4 & 0xFFBFFFFF) | 0x400000;
        top_2898(grManPushView)->f4 = (top_2898(grManPushView)->f4 & ~0x7C) | 4;
        sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)VPTR_OF(cGraphicsMan, grManPushView);
        vt[31].fn(vthis(grManPushView, vt[31]));
        cPS2GraphicsMan* grManLockTex = GrMan;
        sVEntry<void(void*, void*, int)>* vt3 = (sVEntry<void(void*, void*, int)>*)VPTR_OF(cGraphicsMan, grManLockTex);
        vt3[53].fn(vthis(grManLockTex, vt3[53]), Shadow_2898(this)->f170, 1);
        cPS2GraphicsMan* grManClearTex = GrMan;
        sVEI3_2898* vt4 = (sVEI3_2898*)VPTR_OF(cGraphicsMan, grManClearTex);
        vt4[55].fn(vthis(grManClearTex, vt4[55]), Tex_2898(Shadow_2898(this)), sV4_2898(0.0f, 0.0f, 0.0f, 0.0f));
        if (getMotionState() == 0) {
            k = 0.0f;
        }
        t4Matrix mat;
        CopyMat_2898(&mat, &Shadow_2898(this)->mat);
        sV4_2898 tuvScale = Shadow_2898(this)->f150;
        sV4_2898 tuvTrans = Shadow_2898(this)->f160;
        cPS2GraphicsMan* grManTuvBatch = GrMan;
        sVE4_2898* vt5 = (sVE4_2898*)VPTR_OF(cGraphicsMan, grManTuvBatch);
        vt5[101].fn(vthis(grManTuvBatch, vt5[101]), &mat, &tuvScale, &tuvTrans);
        sV4_2898 boardColour;
        boardColour.x = 1.0f;
        boardColour.y = k;
        boardColour.z = k;
        boardColour.w = k;
        sV4_2898 shadowColour;
        shadowColour.x = 1.0f;
        shadowColour.y = 0.3f;
        shadowColour.z = 0.3f;
        shadowColour.w = 0.3f;
        cAnimModel_rendershadow(Model_2898(this), 3, &boardColour, &shadowColour);
        cPS2GraphicsMan* grManRestoreLock = GrMan;
        sVEI_2898* vt6 = (sVEI_2898*)VPTR_OF(cGraphicsMan, grManRestoreLock);
        vt6[54].fn(vthis(grManRestoreLock, vt6[54]));
        cPS2GraphicsMan* grManPopView = GrMan;
        sVEntry<void(void*)>* vt7 = (sVEntry<void(void*)>*)VPTR_OF(cGraphicsMan, grManPopView);
        vt7[32].fn(vthis(grManPopView, vt7[32]));
        cPS2GraphicsMan* grManPopState = GrMan;
        top_2898(grManPopState)--;
    }
    cPlayerShadow_AddShadow(Shadow_2898(this));
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getEventStartPos);
#ifdef SKIP_ASM
extern "C" void* cPathSys_getStartpoint(void*, int, int);
extern char gPaths[];

/**
 * Returns the rider's start position for the event.
 *
 * Looks up start point mAIIndex of type 1 in gPaths (cPathSys_getStartpoint, On Tour cPathSys::getStartpoint) and copies its
 * position (words 2-4) with w = 1.
 *
 * @param out  receives the position (the by-value return)
 * @param rider  the rider (this)
 * @return @p out
 */
extern "C" float* cRider_getEventStartPos(float* out, cRider* rider)
{
    float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 1, rider->mAIIndex);
    out[0] = startPoint[2];
    out[1] = startPoint[3];
    out[2] = startPoint[4];
    out[3] = 1.0f;
    return out;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getVertDangerAngleMin);
#ifdef SKIP_ASM
/**
 * Returns 0.92508197, the limit cRider_isInVertZone compares mSmoothedHeading with (at or above it, or below its
 * negative).
 *
 * @return 0.92508197
 */
extern "C" float cRider_getVertDangerAngleMin(void) {
    return 0.92508197f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_isInVertZone);
#ifdef SKIP_ASM
/**
 * Tells whether the rider's smoothed slope angle is steep enough to count as vert.
 *
 * The limit is 0.925 rad (about 53 degrees) on either side of zero.
 *
 * @return true when mSmoothedHeading >= 0.925 or < -0.925
 */
bool cRider::isInVertZone()
{
    float heading = mSmoothedHeading;
    float k = 0.9250819683074951f;
    return heading >= k || heading < -k;
}
#endif

extern "C" int cRider_isGroundNormalValid(cRider* self);

//100%
INCLUDE_ASM("ai/rider", cRider_isVertZoneApplicable__FPv);
#ifdef SKIP_ASM
/**
 * Returns cRider_isGroundNormalValid for the rider.
 *
 * cRider_updateDangerLevel calls it.
 *
 * @return cRider_isGroundNormalValid's result
 */
int cRider::isVertZoneApplicable()
{
    return isGroundNormalValid();
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateDangerLevel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cDangerMan_GetRiderDangerLevel(void* self, int character);
extern "C" bool cRider_isInVertZone(cRider* self);
float cRiderMetrix_evDangerLevelChange_2(void* self, int state) __asm__("cRiderMetrix_evDangerLevelChange__FPv");

/**
 * Updates the rider's danger level (unk_5B0) from the AI's danger manager.
 *
 * Takes the manager's level for mAIIndex (cDangerMan_GetRiderDangerLevel); level 0 becomes 1 when the rider is on the ground
 * in a vert zone. A change goes to cRiderMetrix_evDangerLevelChange on mMetrix, an empty stub in retail.
 */
void cRider::updateDangerLevel()
{
    int old = DangerLevel;
    int dangerLevel = cDangerMan_GetRiderDangerLevel(SSXApp->Game->AI->DangerMan, mAIIndex);
    DangerLevel = dangerLevel;
    if (dangerLevel == 0)
    {
        if (this->isVertZoneApplicable() != 0)
        {
            if (isInVertZone())
                DangerLevel = 1;
        }
    }
    int cur = DangerLevel;
    if (old != cur)
        cRiderMetrix_evDangerLevelChange_2(mMetrix, cur);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_estimateFinishTick);
#ifdef SKIP_ASM
float cRider_getTotalPathDistance(void* self);


/**
 * Estimates the race-clock tick at which the rider will finish.
 *
 * The speed is the distance covered (cRider_getTotalPathDistance's float at +0x4D8, probably the course length, minus
 * mDistToFinish) over the race clock (AI EventTimerTick), but at least 30 minus the AI index per tick.
 *
 * @return the race clock plus mDistToFinish over that speed
 */
int cRider::estimateFinishTick()
{
    int n = SSXApp->Game->AI->EventTimerTick;
    float rate = (cRider_getTotalPathDistance(this) - mDistToFinish) / (float)n;
    if (rate < 30.0f - (float)getAIIndex())
    {
        rate = 30.0f - (float)getAIIndex();
    }
    return n + (int)(mDistToFinish / rate);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_estimateFinishScore);
#ifdef SKIP_ASM
float cRider_getTotalPathDistance(void* self);

/**
 * Projects the rider's score at the finish from its score rate so far.
 *
 * n is the metrix word at +0x198 plus 1, and the rate is n over the distance covered (at least 1).
 *
 * @return n plus the remaining distance less 1000 (at least 0) times the rate
 */
int cRider::estimateFinishScore()
{
    int n = mMetrix->Totals.TotalPoints + 1;
    float distCovered = cRider_getTotalPathDistance(this) - mDistToFinish;
    if (distCovered < 1.0f)
        distCovered = 1.0f;
    float scorePerDist = (float)n / distCovered;
    float distLeft = mDistToFinish - 1000.0f;
    if (distLeft < 0.0f)
        distLeft = 0.0f;
    return n + (int)(distLeft * scorePerDist);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getMissionInt);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cSpinControl_isGrabHeld(void* p);
extern "C" int cSpinControl_isTweakHeld(void* p);
extern "C" int cSpinControl_isUberHeld(void* p);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void* self, int i);
extern "C" int tEventFlags_isSet1(void* self, int a1);

/**
 * Answers a mission query about the rider.
 *
 * @p query (eMissionQuery): kMQ_Wipeout (MS_Wipeout), kMQ_Reset (CS_Reset), kMQ_Air (MS_Air, or MS_Wipeout with the
 * wipeout motion airborne), kMQ_Ground, kMQ_Railslide, kMQ_GrabHeld / kMQ_TweakHeld / kMQ_UberHeld during a spin
 * (cSpinControl_isGrabHeld, isTweakHeld, isUberHeld on the handler's spin control at +0x230), kMQ_TotalPoints the
 * metrix's points (+0x198), kMQ_NosePress / kMQ_TailPress (PressMode kPM_Nose / kPM_Tail), kMQ_Boosting
 * (mBoostStrength non-zero), kMQ_NumPerfectLanding to kMQ_TotalStylePoints the metrix's +0x138, +0x13C and +0x19C,
 * kMQ_Punching a layer-1 anim of category kANCAT_Punch with event flag 0 set and flag 1 clear. kMQ_Stall, kMQ_OffAxis,
 * kMQ_Inverted and others return 0. The script functions RiderActionFunc,
 * RiderMetricFunc and CurrentRailFunc call it.
 *
 * @param query  the query number
 * @return the answer (a flag or a count)
 */
int cRider::getMissionInt(int query)
{
    int res;
    switch (query) {
    case kMQ_Wipeout:
        return cRider_getMotionState((char*)this) == MS_Wipeout;
    case kMQ_Reset:
        return cRider_getControlState((char*)this) == CS_Reset;
    case kMQ_Air:
        {
            bool ok = false;
            if (cRider_getMotionState((char*)this) == MS_Air
                || (cRider_getMotionState((char*)this) == MS_Wipeout
                    && mRiderHandler->WipeoutMotion.WipeoutMotionState == cWipeoutMotion::kWMS_Air)) {
                ok = true;
            }
            return ok;
        }
    case kMQ_Ground:
        return cRider_getMotionState((char*)this) == MS_Ground;
    case kMQ_Railslide:
        return cRider_getMotionState((char*)this) == MS_Railslide;
    case kMQ_GrabHeld:
        res = 0;
        if (cRider_getControlState((char*)this) == CS_Spin) {
            res = cSpinControl_isGrabHeld(&mRiderHandler->SpinControl) != 0;
        }
        return res;
    case kMQ_TweakHeld:
        res = 0;
        if (cRider_getControlState((char*)this) == CS_Spin) {
            res = cSpinControl_isTweakHeld(&mRiderHandler->SpinControl) != 0;
        }
        return res;
    case kMQ_UberHeld:
        res = 0;
        if (cRider_getControlState((char*)this) == CS_Spin) {
            res = cSpinControl_isUberHeld(&mRiderHandler->SpinControl) != 0;
        }
        return res;
    case kMQ_NosePress:
        return PressMode == kPM_Nose;
    case kMQ_TailPress:
        return PressMode == kPM_Tail;
    case kMQ_Boosting:
        return mBoostStrength != 0.0f;
    case kMQ_TotalPoints:
        return mMetrix->Totals.TotalPoints;
    case kMQ_NumPerfectLanding:
        return mMetrix->Totals.NumPerfectLandingBonus;
    case kMQ_NumLastSecondLanding:
        return mMetrix->Totals.NumLastSecondLandingBonus;
    case kMQ_TotalStylePoints:
        return mMetrix->Totals.TotalStylePoints;
    case kMQ_Punching:
        res = 0;
        if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 1) == kANCAT_Punch) {
            if (tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->Events, 0)) {
                res = !tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->Events, 1);
            }
        }
        return res;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getIdentity);
#ifdef SKIP_ASM
signed char cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Returns the rider's character id from the BE player interface (cBENewPlayerInterface_getRiderCharID).
 *
 * @return the character id for mAIIndex
 */
int cRider::getIdentity()
{
    return cBENewPlayerInterface_getRiderCharID(cBE_getInterface_Fv(cBE_getBE(), 1), mAIIndex);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getCheatIdentity);
#ifdef SKIP_ASM
extern "C" int cBENewPlayerInterface_getRiderCheatCharID(void* iface, int riderIndex);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Returns the cheat character chosen for this rider's player.
 *
 * Asks the new-player interface (cBE interface kind 1) through cBENewPlayerInterface_getRiderCheatCharID for the rider's mAIIndex; game mode code
 * stores the same value in iCheatCharacters.
 *
 * @return the cheat character id for the rider's player
 */
int cRider::getCheatIdentity()
{
    return cBENewPlayerInterface_getRiderCheatCharID(cBE_getInterface_Fv(cBE_getBE(), 1), mAIIndex);
}
#endif

// Grounded predicate: motion 0, or motion 2 with owner+0x30 == 0.
//100%
INCLUDE_ASM("ai/rider", cRider_isGroundNormalValid);
#ifdef SKIP_ASM
/**
 * Tells whether the rider is on the ground, so that its ground normal is meaningful.
 *
 * @return 1 in motion state MS_Ground, or MS_Wipeout with the wipeout on the ground; otherwise 0
 */
int cRider::isGroundNormalValid()
{
    int grounded = 0;

    if (getMotionState() == MS_Ground
        || (getMotionState() == MS_Wipeout
            && mRiderHandler->WipeoutMotion.WipeoutMotionState == cWipeoutMotion::kWMS_Ground)) {
        grounded = 1;
    }
    return grounded;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_transport);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.

extern t4Vector t4vect_XAxis;

// 16-byte vectors with inline constructors (non-POD siblings of t4Vector). The classes differ only in the order
// their 4-float constructor stores the components; that order decides the float-register allocation of every
// caller (merging them changes code), so they are kept apart.
// sVec4 stores x, y, z, w.
extern "C" void cReplay_eventReset(cReplay* replay, int riderIndex);
extern "C" void cRider_cancelRiderTransitions(cRider* self);
extern "C" void cRiderHandler_resetRiderFX(void* self);
extern "C" void cAttachedEmitterFX_clearDisableSnowTrailSpray(void* a);
extern "C" void cRiderMetrix_evReset(void* metrix, int flag);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

void cRider_putAt_3210(void* self, sVec4* a, sVec4* b, int n, float t) __asm__("cRider_putAt");

// PORT: PS2-only VU0 inline asm (4x4 matrix times vector).
static inline sVec4 MatMulV_3210(const t4Matrix* m, const t4Vector& v)
{
    sVec4 r;
    __asm__(
        "lqc2       $vf8, %2\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "lqc2       $vf5, 0x10(%1)\n"
        "lqc2       $vf6, 0x20(%1)\n"
        "lqc2       $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2       $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "o"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec4 Scale_3210(const sVec4& v, float s)
{
    sVec4 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a += b).
static inline void AddEq_3210(sVec4& a, const sVec4& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_3210(const sVec4& v)
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


// Inline wrapper: the vector addresses reach cRider_putAt through inline parameters,
// so they are not CSE'd with the if-chain's temporaries (the target recomputes sp+0x20).
static inline void SetPath_3210(void* self, sVec4* p, sVec4* d)
{
    cRider_putAt_3210(self, p, d, 5, 0.0f);
}

/**
 * Teleports the rider to a transform, keeping its speed, with a per-rider offset so riders do not overlap.
 *
 * The rider's replay-cache slot is reset (cReplay_eventReset). The position is the matrix's row 3 plus an offset
 * in the matrix's frame times 100: rider 0 (2, 0), 1 (1, 0), 2 (2, -2), 3 (2, 2), 4 (1, -2), 5 (2, 1); the
 * heading is the matrix's X axis. Ends the trick, sets MS_Ground and CS_Cruise, cancels camera transitions,
 * notifies view 0's camera controller (slot 3), places the rider (cRider_putAt), points the old speed along
 * the new front axis and resets the FX.
 *
 * @param mat  the target transform, row 3 = position (On Tour: const t4Matrix & mat)
 */
void cRider::transport(t4Matrix* mat)
{
    cReplay_eventReset(SSXApp->Game->Replay, mAIIndex);
    sVec4 pos = *(const sVec4*)&mat->row[3];
    sVec4 off(0.0f, 0.0f, 0.0f, 0.0f);
    int i = mAIIndex;
    if (i == 0) {
        off = sVec4(2.0f, 0.0f, 0.0f, 0.0f);
    } else if (i == 1) {
        off = sVec4(1.0f, 0.0f, 0.0f, 0.0f);
    } else if (i == 2) {
        off = sVec4(2.0f, -2.0f, 0.0f, 0.0f);
    } else if (i == 3) {
        off = sVec4(2.0f, 2.0f, 0.0f, 0.0f);
    } else if (i == 4) {
        off = sVec4(1.0f, -2.0f, 0.0f, 0.0f);
    } else if (i == 5) {
        off = sVec4(2.0f, 1.0f, 0.0f, 0.0f);
    }
    off = Scale_3210(MatMulV_3210(mat, off), 100.0f);
    AddEq_3210(pos, off);
    sVec4 dir = MatMulV_3210(mat, t4vect_XAxis);
    cRiderMetrix_evReset(mMetrix, 1);
    cRider_setMotionState_impl(this, 0);
    cRider_setControlState_impl(this, 0);
    cancelRiderTransitions();
    char* sub = (char*)&VPTR_AT(this, sizeof(cRiderState));
    sVEntry<int(void*)>* vtA = *(sVEntry<int(void*)>**)sub;
    cCameraController* ctrl = SSXApp->Game->ViewMan->PlayerCamera[0]->Controller;
    sVEntry<void(void*, int)>* vtB = (sVEntry<void(void*, int)>*)ctrl->mVtbl; // slot 3: snap(subjectID)
    void* objB = vthis(ctrl, vtB[3]);
    vtB[3].fn(objB, vtA[kRiderVt_getAIIndex].fn(sub + vtA[kRiderVt_getAIIndex].delta));
    float len = Length_3210(*(sVec4*)&mVelocity);
    SetPath_3210(this, &pos, &dir);
    mVelocity = Scale_3210(sVec4(*(sVec4*)&mMatrix.row[1]), len);
    cRiderHandler_resetRiderFX(mRiderHandler);
    cAttachedEmitterFX_clearDisableSnowTrailSpray(mAttachedEmitterFX);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_initForMission);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.

extern t4Vector t4vect_XAxis;
extern "C" void cReplay_eventReset(cReplay* replay, int riderIndex);
extern "C" void cRider_cancelRiderTransitions(cRider* self);
extern "C" void cAttachedEmitterFX_clearDisableSnowTrailSpray(void* a);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

void cRider_putAt_34D0(void* self, t4Vector* a, t4Vector* b, int n, float t) __asm__("cRider_putAt");

// PORT: PS2-only VU0 macro-mode asm (4x4 matrix times vector).
// PORT: g++ named-return-value extension.
static inline t4Vector MatMulV_34D0(const char* m, const t4Vector& v) return r
{
    __asm__(
        "lqc2       $vf8, %2\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "lqc2       $vf5, 0x10(%1)\n"
        "lqc2       $vf6, 0x20(%1)\n"
        "lqc2       $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2       $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "o"(v)
        : "memory");
}


/**
 * Places the rider at a mission's start transform, sitting in a start gate.
 *
 * Position = the matrix's row 3, heading = its X axis. Ends the trick, sets MS_Static and CS_Void, places the
 * rider (cRider_putAt), sets the gate control's GateState to GS_Sit and switches to CS_Gate, cancels camera
 * transitions, resets the FX and, for a rider with a split-screen view, calls slot 4 of that view's chase
 * camera controller.
 *
 * @param mat  the start transform (On Tour: const t4Matrix & mat)
 */
void cRider::initForMission(t4Matrix* mat)
{
    t4Vector pos;
    t4Vector dir;
    cReplay_eventReset(SSXApp->Game->Replay, mAIIndex);
    pos = mat->row[3];
    dir = MatMulV_34D0(((char*)mat), t4vect_XAxis);
    cRiderMetrix_evReset(mMetrix, 1);
    cRider_setMotionState_impl(this, 3);
    cRider_setControlState_impl(this, 13);
    cRider_putAt_34D0(this, &pos, &dir, 5, 0.0f);
    mRiderHandler->GateControl.GateState = 2;
    cRider_setControlState_impl(this, 6);
    cancelRiderTransitions();
    cRiderHandler_resetRiderFX(mRiderHandler);
    cAttachedEmitterFX_clearDisableSnowTrailSpray(mAttachedEmitterFX);
    if ((unsigned int)mViewIndex < 2) {
        cChaseCameraController* obj = SSXApp->Game->ViewMan->PlayerCamera[mViewIndex]->ChaseController;
        sVEntry<void(void*)>* vt = (sVEntry<void (void*)>*)obj->mVtbl;
        vt[4].fn(vthis(obj, vt[4]));
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_resetForMission);
#ifdef SKIP_ASM
// PORT: prototype mismatch. The unit defines cRider_setControlState/cRider_setMotionState with one
// param, but their bodies forward $5 to cRiderHandler__setControlState/cRiderHandler__setMotionState (int arg).
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

/**
 * Starts an automatic reset for a mission rider.
 *
 * Sets the reset control's IsAutoReset, control state CS_Reset, motion state MS_Static and the gate control's
 * GateState to 0 (GS_GateRock).
 */
void cRider::resetForMission()
{
    mRiderHandler->ResetControl.IsAutoReset = 1;
    cRider_setControlState_impl(this, 9);
    cRider_setMotionState_impl(this, 3);
    mRiderHandler->GateControl.GateState = 0;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_OnStartObjectUse);
#ifdef SKIP_ASM
extern "C" void cScriptObjRider_OnStartObjectUse(void* self);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");
extern "C" void cRiderAnimBase_reset(void* anim);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
tSSXHeaderActor* GetSSXActorHeader(void* self);
extern "C" void GetSSXActorFrame(void* pPlayer, void* frame);
struct sQuat_001241C0;
extern "C" void cRider_EulerToQuat(cRider* self, tQuaternion* out, float rx, float ry, float rz);
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cRider_updateOrientationImplicit(cRider* self);
extern "C" void cRiderHandler_resetRiderFX(void* self);
extern "C" void cAnimModel_disablePart(void* model, int i, int channel);
extern "C" void cAnimModel_enablePart(void* model, int i, int channel);
void* cBEBoltInterface_getBoltDB_3640() __asm__("cBEBoltInterface_getBoltDB__FPv");
extern "C" void* cBEBoltInterface_getRiderCharProfile(void* iface, int id);
int cBEBoltDB_getNumCharBolts(void* tbl, int g);
extern "C" void* cBEBoltDB_getCharBolts(void* tbl, int g);
extern "C" int cRider_getIdentity(cRider* self);
extern "C" void cRider_setupBoostWarp(void* rider, int a, int b);

extern sVec4 t4vect_Zero_3640 __asm__("t4vect_Zero");
extern sVec4 t4vect_ZAxis_3640 __asm__("t4vect_ZAxis");

static inline sVec4 AxisAngle_3640(const sVec4& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sVec4(s * axis.x, s * axis.y, s * axis.z, c);
}

// PORT: PS2-only VU0 macro-mode asm (4x4 matrix times vector).
// PORT: g++ named-return-value extension.
static inline sVec4 MatMulV_3640(const char* m, const sVec4& v) return r
{
    __asm__(
        "lqc2       $vf8, %2\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "lqc2       $vf5, 0x10(%1)\n"
        "lqc2       $vf6, 0x20(%1)\n"
        "lqc2       $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2       $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
}

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sVec4 QuatMul_3640(const sVec4& a, const sVec4& b)
{
    sVec4 r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// resets slewed channel i to v (cur and target) with rate z
static inline void initTrack_3640(cRider* self, int i, float v, float z)
{
    self->mSlewed[i].target = v;
    self->mSlewed[i].cur = v;
    self->mSlewed[i].rate = z;
}



static inline tBEProfileBolt* lookup_3640(cBECharProfileDB* d, int i)
{
    short k = d->mpBoltsMap[i];
    if (k >= 0) {
        return &((&d->mBolts[0]))[k];
    }
    return 0;
}

/**
 * Starts a scripted sequence (NIS) for the rider: leaves normal control and poses the rider from the script.
 *
 * Sets CS_Void and MS_Static, plays anim 5, sets mIsInNIS and clears the event flag and sound ids. The actor
 * header's (GetSSXActorHeader) isRidingBoard sets mNISShowBoardVisualFX (probe the ground at the board root),
 * hidden sets mHiddenNIS (rider not drawn). The entity's channels give the start position, put through
 * mScriptMatrix into mPosition, and three angles in degrees that set mRotation.
 *
 * Velocity and speed are zeroed, the FX and the 19 slewed channels reset (channel 9 to 1), parts 5, 6, 8 and
 * 9 enabled and 4 and 7 disabled at LOD 0; with hideIllegalBoltOn set, the model parts of the rider's owned
 * items (bolt slot flag 0x10) whose definition has flag 0x40 are disabled at every LOD. Ends any boost
 * (cRider_setupBoostWarp(self, 0, 0)).
 */
void cRider::OnStartObjectUse()
{
    int buf[12];
    cScriptObjRider_OnStartObjectUse(&mScriptObjRiderVtbl);
    cRider_setControlState_impl(this, 13);
    cRider_setMotionState_impl(this, 3);
    cRiderAnimBase_reset(mRiderAnim);
    cRiderAnimBase_play(mRiderAnim, 5, 0, -1.0f);
    mIsInNIS = 1;
    mWasFlipped = IsRidingFakie;
    mHasTriggeredSpeechEvents = 0;
    int j;
    for (j = 3; j >= 0; j--)
        mSoundHandles[j] = -1;
    float zeroF;
    float one = 1.0f;
    tSSXHeaderActor* rec = GetSSXActorHeader(mpEntity);
    zeroF = 0.0f;
    mNISShowBoardVisualFX = rec->isRidingBoard != 0;
    mOnGroundNIS = 1;
    mHiddenNIS = rec->hidden != 0;
    GetSSXActorFrame(mpEntity, buf);
    float* fb = (float*)buf;
    sVec4 poseVec;
    poseVec.x = fb[6];
    poseVec.y = fb[7];
    poseVec.z = fb[8];
    poseVec.w = one;
    mScriptCurrRotX = fb[9] * 0.01745329424738884f;
    mScriptCurrRotY = fb[10] * 0.01745329424738884f;
    mScriptCurrRotZ = fb[11] * 0.01745329424738884f;
    *(sVec4*)&mScriptCurrPosOffset = poseVec;
    poseVec = MatMulV_3640((char*)&mScriptMatrix, *(sVec4*)&mScriptCurrPosOffset);
    *(sVec4*)&mPosition = poseVec;
    float ry = mBaseRotY;
    float rz = mBaseRotX;
    EulerToQuat((tQuaternion*)&poseVec, zeroF,
                  mScriptCurrRotY + ry,
                  mScriptCurrRotX + rz);
    float rx = mBaseRotZ;
    *(sVec4*)&mRotation = AxisAngle_3640(t4vect_ZAxis_3640, mScriptCurrRotZ + rx - 1.5707963705062866f);
    *(sVec4*)&mRotation = QuatMul_3640(poseVec, *(sVec4*)&mRotation);
    this->updateOrientationImplicit();
    *(sVec4*)&mVelocity = t4vect_Zero_3640;
    mLastSpeed = zeroF;
    cRiderHandler_resetRiderFX(mRiderHandler);
    initTrack_3640(this, 0, zeroF, zeroF);
    initTrack_3640(this, 1, zeroF, zeroF);
    initTrack_3640(this, 2, zeroF, zeroF);
    initTrack_3640(this, 3, zeroF, zeroF);
    initTrack_3640(this, 4, zeroF, zeroF);
    initTrack_3640(this, 5, zeroF, zeroF);
    initTrack_3640(this, 6, zeroF, zeroF);
    initTrack_3640(this, 7, zeroF, zeroF);
    initTrack_3640(this, 8, zeroF, zeroF);
    initTrack_3640(this, 9, one, zeroF);
    initTrack_3640(this, 10, zeroF, zeroF);
    initTrack_3640(this, 11, zeroF, zeroF);
    initTrack_3640(this, 12, zeroF, zeroF);
    initTrack_3640(this, 13, zeroF, zeroF);
    initTrack_3640(this, 14, zeroF, zeroF);
    initTrack_3640(this, 15, zeroF, zeroF);
    initTrack_3640(this, 16, zeroF, zeroF);
    initTrack_3640(this, 17, zeroF, zeroF);
    initTrack_3640(this, 18, zeroF, zeroF);
    cAnimModel_enablePart(mAnimModel, 5, 0);
    cAnimModel_enablePart(mAnimModel, 6, 0);
    cAnimModel_enablePart(mAnimModel, 8, 0);
    cAnimModel_enablePart(mAnimModel, 9, 0);
    cAnimModel_disablePart(mAnimModel, 4, 0);
    cAnimModel_disablePart(mAnimModel, 7, 0);
    if (rec->hideIllegalBoltOn != 0) {
        void* iface = cBE_getInterface(cBE_getBE(), 9);
        void* tbl = cBEBoltInterface_getBoltDB_3640();
        cBECharProfileDB* db = (cBECharProfileDB*)cBEBoltInterface_getRiderCharProfile(iface, mAIIndex);
        int identity = getIdentity();
        int n = cBEBoltDB_getNumCharBolts(tbl, identity);
        tBEBoltDef* entry = (tBEBoltDef*)cBEBoltDB_getCharBolts(tbl, identity);
        int i;
        for (i = 0; i < n; i++, entry++) {
            int id = entry->modelPart;
            if (id >= 0 && (entry->flags & 0x40)) {
                if (lookup_3640(db, entry->id)->flags & 0x10) {
                    cAnimModel_disablePart(mAnimModel, id, 0);
                    cAnimModel_disablePart(mAnimModel, id, 1);
                    cAnimModel_disablePart(mAnimModel, id, 2);
                    cAnimModel_disablePart(mAnimModel, id, 3);
                }
            }
        }
    }
    cRider_setupBoostWarp(this, 0, 0);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_OnEndObjectUse);
#ifdef SKIP_ASM
extern "C" void cScriptObjRider_OnEndObjectUse(void* self);
extern "C" void cRider_initAlways(void* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
void func_002A1BD0(void* p);
extern "C" void cAnimModel_disablePart(void* model, int i, int channel);
extern "C" void cAnimModel_enablePart(void* model, int i, int channel);
tSSXHeaderActor* GetSSXActorHeader(void* self);
// PORT: cBEBoltInterface_getBoltDB is declared (void*) but never reads its argument; called here with none
void* cBEBoltInterface_getBoltDB_noarg() __asm__("cBEBoltInterface_getBoltDB__FPv");
extern "C" void* cBEBoltInterface_getRiderCharProfile(void* iface, int id);
int cBEBoltDB_getNumCharBolts(void* tbl, int g);
extern "C" void* cBEBoltDB_getCharBolts(void* tbl, int g);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* bank, int id, int a, float f);



static inline tBEProfileBolt* lookup_123B48(cBECharProfileDB* d, int i)
{
    short k = d->mpBoltsMap[i];
    if (k >= 0) {
        return &((&d->mBolts[0]))[k];
    }
    return 0;
}

static inline int entId_123B48(tBEBoltDef* e)
{
    return e->modelPart;
}

static inline int entOn_123B48(tBEBoltDef* e)
{
    return e->flags & 0x40;
}

static inline void* model_123B48(cRider* self)
{
    return self->mAnimModel;
}

static inline void setChan_123B48(cRider* self, int id, int ch)
{
    cAnimModel_enablePart(model_123B48(self), id, ch);
}

static inline int entItem_123B48(tBEBoltDef* e)
{
    return e->id;
}

static inline unsigned short itemFlags_123B48(cBECharProfileDB* db, tBEBoltDef* e)
{
    return lookup_123B48(db, entItem_123B48(e))->flags;
}

/**
 * Ends a scripted sequence (NIS) and puts the rider back into the race.
 *
 * Ends the script object's use (cScriptObjRider_OnEndObjectUse), resets the rider (cRider_initAlways), calls the audio stub
 * func_002A1BD0 if an event fired, clears the NIS flags, disables parts 5, 6, 8, 9 and 11 and re-enables 4
 * and 7 at LOD 0, re-enables the items cRider_OnStartObjectUse() hid, and stops the up to four sounds in
 * unk_B08.
 */
void cRider::OnEndObjectUse()
{
    cScriptObjRider_OnEndObjectUse((char*)&mScriptObjRiderVtbl);
    cRider_initAlways(this);
    if (mHasTriggeredSpeechEvents != 0) {
        func_002A1BD0(cAudioInterface_bxAudio());
    }
    mNISShowBoardVisualFX = 0;
    mLastSpeed = 0;
    mIsInNIS = 0;
    int* ids = mSoundHandles;
    mHiddenNIS = 0;
    mOnGroundNIS = 1;
    cAnimModel_disablePart(mAnimModel, 5, 0);
    cAnimModel_disablePart(mAnimModel, 6, 0);
    cAnimModel_disablePart(mAnimModel, 8, 0);
    cAnimModel_disablePart(mAnimModel, 9, 0);
    cAnimModel_disablePart(mAnimModel, 11, 0);
    cAnimModel_enablePart(mAnimModel, 4, 0);
    cAnimModel_enablePart(mAnimModel, 7, 0);
    if (GetSSXActorHeader(mpEntity)->hideIllegalBoltOn != 0) {
        void* iface = cBE_getInterface(cBE_getBE(), 9);
        void* tbl = cBEBoltInterface_getBoltDB_noarg();
        cBECharProfileDB* db = (cBECharProfileDB*)cBEBoltInterface_getRiderCharProfile(iface, mAIIndex);
        int identity = getIdentity();
        int n = cBEBoltDB_getNumCharBolts(tbl, identity);
        tBEBoltDef* entry = (tBEBoltDef*)cBEBoltDB_getCharBolts(tbl, identity);
        int i;
        for (i = 0; i < n; i++, entry++) {
            int id = entId_123B48(entry);
            if (id >= 0 && entOn_123B48(entry)) {
                if (itemFlags_123B48(db, entry) & 0x10) {
                    setChan_123B48(this, id, 0);
                    setChan_123B48(this, id, 1);
                    setChan_123B48(this, id, 2);
                    setChan_123B48(this, id, 3);
                }
            }
        }
    }
    int j;
    for (j = 0; j < 4; j++) {
        if (ids[j] >= 0) {
            cBankMonitor_BANKMONITOR_StopHandle(
                    &cAudioInterface_bxAudio()->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                    ids[j], 1, 0.0f);
        }
        ids[j] = -1;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_OnInSeqUpdate);
#ifdef SKIP_ASM
extern "C" void GetSSXActorFrame(void* pPlayer, void* frame);
extern "C" void cRider_DoInSeqAnimUpdate(void*, void*, float, float, int);
extern "C" void cRider_DoInSeqSoundUpdate(cRider*, void*, float, float, int);

/**
 * Per-frame update from the in-sequence script player while the rider is in an NIS.
 *
 * Reads the entity's channel values (GetSSXActorFrame) and passes them to cRider_DoInSeqAnimUpdate (animation) and
 * cRider_DoInSeqSoundUpdate (sound), probably On Tour's DoInSeqAnimUpdate and DoInSeqSoundUpdate. The middle parameter
 * names do not match the argument registers: the two floats are On Tour's currFrame and frameDelta, the int
 * is its type.
 *
 * @param entity  the script player entity (On Tour: pEntity)
 * @param currFrame  the current frame (the first float argument; On Tour: currFrame)
 * @param frameDelta  the frame step (the second float argument; On Tour: frameDelta)
 * @param type  the entity type (On Tour: type); unused
 * @param flags  event bits, passed on to both updates
 */
void cRider::OnInSeqUpdate(cScriptPlayerEntity* entity, float currFrame, float frameDelta, int type, int flags)
{
    int buf[12];
    if (mIsInNIS)
    {
        GetSSXActorFrame(entity, buf);
        cRider_DoInSeqAnimUpdate(this, buf, currFrame, frameDelta, flags);
        DoInSeqSoundUpdate(buf, currFrame, frameDelta, flags);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_OnInSeqKeyEvent);
#ifdef SKIP_ASM
extern "C" void GetSSXActorFrame(void* pPlayer, void* frame);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int cSSXAudio_HandleRiderSpeechMessage(void* self, int msg, cRider* rider);
extern "C" int cSSXAudio_HandleSubjectSpeechEvent(void* self, int type, cRider* rider, void* otherRider);
extern "C" void* cSSXScriptEngine_GetSubjectRider(void* self, int i, cScriptPlayerMain* pPlayer);



/**
 * Handles an in-sequence script's key events: sends its sound events for the rider.
 *
 * Only when @p flags has bit 0x10: reads the entity's channels (GetSSXActorFrame) and, for each event of channel
 * 2 except 0x66, sends ids below 0x64 to the audio system (cSSXAudio_HandleRiderSpeechMessage) and ids from 0x64 on, less 0x64,
 * with the event's subject rider (cSSXScriptEngine_GetSubjectRider) to cSSXAudio_HandleSubjectSpeechEvent. Each event sent sets
 * unk_AC8.
 *
 * @param entity  the script player entity (On Tour: pEntity); its mpOwner is passed to the subject lookup
 * @param currFrame  the current frame (the first float argument; On Tour: currFrame); unused
 * @param ignoredFloat  a second float argument that the caller (cScriptPlayerEntity_ExecutePlay) does not set; unused
 * @param type  the entity type (On Tour: type); unused
 * @param flags  event bits; bit 0x10 enables the event handling
 */
void cRider::OnInSeqKeyEvent(cScriptPlayerEntity* entity, float currFrame, float ignoredFloat, int type, int flags)
{
    tSSXFrameActor info;
    if (flags & 0x10)
    {
        GetSSXActorFrame(entity, &info);
        for (int i = 0; i < info.numSpeechEvent; i++)
        {
            tSpeechEvent* event = &(info.pSpeechEventArray)[i];
            int ev = event->eventID;
            if (ev == 102) continue;
            if (ev < 100)
            {
                cSSXAudio_HandleRiderSpeechMessage(cAudioInterface_bxAudio(), ev, this);
                mHasTriggeredSpeechEvents = 1;
            }
            else
            {
                ev -= 100;
                void* snd = cAudioInterface_bxAudio();
                void* subjectRider = cSSXScriptEngine_GetSubjectRider(ScriptEngine, *((signed char*)event->pCustom),
                                                                      entity->mpOwner);
                cSSXAudio_HandleSubjectSpeechEvent(snd, ev, this, subjectRider);
                mHasTriggeredSpeechEvents = 1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_GetNISEndPosition);
#ifdef SKIP_ASM
extern "C" void* cPathSys_getStartpoint(void*, int, int);
extern char gPaths[];
tSSXHeaderActor* GetSSXActorHeader(void* self);


extern t4Vector t4vect_ZAxis_3F38 __asm__("t4vect_ZAxis");



extern "C" void cWorldLine_cWorldLine_3F38(cWorldLine* ray, const t4Vector& a, const t4Vector& b, int n, float t) __asm__("cWorldLine_cWorldLine");
extern "C" float cBXNash_terrainIntersect1(void* self, cWorldLine* ray, tIntersectInfo* out, int a3);

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector Scale_3F38(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_3F38(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void AddTo_3F38(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

/**
 * Returns where the rider should end up after its NIS, snapped to the ground.
 *
 * Returns mPosition unchanged when unk_6E4 <= 0. Otherwise byte 5 of the script record picks the point: 0 the
 * current position; 1 start point (type 1, id = AI index); 2 start point (2, 0); 3 by the section manager's
 * location (mBELocationID): kLOCATION_HUB_A to kLOCATION_HUB_E (2, 0), kLOCATION_ABC1 to kLOCATION_EBC3 (2, AI
 * index), others (2, 1); 4 start point (2, AI index), raised by 3000
 * after the snap. A ray from 7000 below to 200 above the point against the Nash grid snaps it to the ground
 * when it hits.
 *
 * @return the end position
 */
extern "C" t4Vector cRider_GetNISEndPosition(cRider* self)
{
    if (self->mInUse <= 0)
        return *(&self->mPosition);
    float lift = 0.0f;
    tSSXHeaderActor* rec = GetSSXActorHeader(self->mpEntity);
    t4Vector pos = *(&self->mPosition);
    switch (rec->scriptEndLocation) {
    case 0:
        break;
    case 1:
        {
            float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 1, self->mAIIndex);
            t4Vector startPos;
            startPos.x = startPoint[2];
            startPos.y = startPoint[3];
            startPos.z = startPoint[4];
            startPos.w = 1.0f;
            pos = startPos;
        }
        break;
    case 2:
        {
            float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 2, 0);
            t4Vector startPos;
            startPos.x = startPoint[2];
            startPos.y = startPoint[3];
            startPos.z = startPoint[4];
            startPos.w = 1.0f;
            pos = startPos;
        }
        break;
    case 3:
        switch (SSXApp->Game->SectionMan->mBELocationID) {
        case kLOCATION_HUB_A:
        case kLOCATION_HUB_B:
        case kLOCATION_HUB_C:
        case kLOCATION_HUB_D:
        case kLOCATION_HUB_E:
            {
                float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 2, 0);
                t4Vector startPos;
                startPos.x = startPoint[2];
                startPos.y = startPoint[3];
                startPos.z = startPoint[4];
                startPos.w = 1.0f;
                pos = startPos;
            }
            break;
        case kLOCATION_ABC1:
        case kLOCATION_DBC2:
        case kLOCATION_EBC3:
            {
                float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 2, self->mAIIndex);
                t4Vector startPos;
                startPos.x = startPoint[2];
                startPos.y = startPoint[3];
                startPos.z = startPoint[4];
                startPos.w = 1.0f;
                pos = startPos;
            }
            break;
        default:
            {
                float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 2, 1);
                t4Vector startPos;
                startPos.x = startPoint[2];
                startPos.y = startPoint[3];
                startPos.z = startPoint[4];
                startPos.w = 1.0f;
                pos = startPos;
            }
            break;
        }
        break;
    case 4:
        {
            float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 2, self->mAIIndex);
            t4Vector startPos;
            startPos.x = startPoint[2];
            startPos.y = startPoint[3];
            startPos.z = startPoint[4];
            startPos.w = 1.0f;
            pos = startPos;
        }
        lift = 2999.942626953125f;
        break;
    }
    t4Vector lo = Add_3F38(pos, Scale_3F38(t4vect_ZAxis_3F38, -7000.0f));
    t4Vector hi = Add_3F38(pos, Scale_3F38(t4vect_ZAxis_3F38, 200.0f));
    tIntersectInfo hit;
    cWorldLine ray;
    cWorldLine_cWorldLine_3F38(&ray, lo, hi, 2, 0.9791666865348816f);
    if (cBXNash_terrainIntersect1(SSXApp->Game->BXNash, &ray, &hit, 0) >= 0.0f)
        pos = hit.Ipos;
    AddTo_3F38(pos, Scale_3F38(t4vect_ZAxis_3F38, lift));
    return pos;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_EulerToQuat);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);


/**
 * Builds a quaternion from three Euler angles.
 *
 * The result is Rz(rx) Ry(ry) Rx(rz): despite the names, @p rx turns about Z (On Tour: heading) and @p rz
 * about X (On Tour: roll). The rider itself is not used.
 *
 * @param out  receives the quaternion (x, y, z, w)
 * @param rx  heading in radians, about the Z axis
 * @param ry  pitch in radians, about the Y axis
 * @param rz  roll in radians, about the X axis
 */
void cRider::EulerToQuat(tQuaternion* out, float rx, float ry, float rz)
{
    tQuaternion quat;
    float sz, cz, sy, cy, sx, cx;
    fSinCos(&sz, &cz, rz * 0.5f);
    fSinCos(&sy, &cy, ry * 0.5f);
    fSinCos(&sx, &cx, rx * 0.5f);
    float cc = cy * cx;
    float ss = sy * sx;
    quat.w = cz * cc + sz * ss;
    quat.x = sz * cc - cz * ss;
    quat.y = cz * sy * cx + sz * cy * sx;
    quat.z = cz * cy * sx - sz * sy * cx;
    *out = quat;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_SetNISGroundVectors);
#ifdef SKIP_ASM

extern t4Vector t4vect_Zero_42B0 __asm__("t4vect_Zero");
extern t4Vector t4vect_ZAxis_42B0 __asm__("t4vect_ZAxis");




extern "C" void cWorldLine_cWorldLine(cWorldLine* ray, const t4Vector& a, const t4Vector& b, int n, float t);
extern "C" float cOverlapList_terrainIntersect1(void* coll, cWorldLine* ray, tIntersectInfo* hit, int flags);
void cRiderHandler_recordSprayBurst_42B0(void* self, void* a, void* b, int c, float x) __asm__("cRiderHandler_recordSprayBurst__FPv");

// PORT: PS2-only VU0 inline asm (vector length).
static inline float Length_42B0(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_42B0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector Scale_42B0(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_42B0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_42B0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector Cross_42B0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector Normalize_42B0(const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

static inline float DotV_42B0(t4Vector a, const t4Vector& b)
{
    return Dot_42B0(a, b);
}


// Non-POD by-value vector (user copy ctor => passed by invisible reference, no callee copy).
struct sVecP_42B0 {
    float x, y, z, w;
    sVecP_42B0(const t4Vector& v) { *(t4Vector*)this = v; }
    sVecP_42B0(const sVecP_42B0& v) { *(t4Vector*)this = *(const t4Vector*)&v; }
} __attribute__((aligned(16)));

static inline t4Vector SubV_42B0(sVecP_42B0 a, const t4Vector& b)
{
    return Sub_42B0(*(t4Vector*)&a, b);
}

// PORT: the unit declares cRider_SetNISGroundVectors(void*, void*); this definition binds the cRider* view by asm
// label.
/**
 * Probes the ground under a position during an NIS (in-game cutscene) and sets the rider's ground vectors.
 *
 * The probe base is @p pRiderPos moved along mGroundSide by mAnimModel->ScaleVect.x * mSlewed[0].cur * 45; a line
 * from 100 below to 200 above it (along mGroundNormal) is built with cWorldLine_cWorldLine and intersected with the
 * terrain through mOverlapList (cOverlapList_terrainIntersect1). The speed is the length of the virtual getVelocity,
 * and LastGroundNormal keeps the previous mGroundNormal. A hit patch sets mGroundPolyId (the patch's m_RID),
 * PatchUV (the hit's u / v) and SurfaceFlags (the patch's m_sSurfaceFlags); otherwise mGroundPolyId becomes -1.
 *
 * On a miss (negative distance): unk_B00 = 0, mGroundNormal = the Z axis, GroundVelocity = zero, mSurfaceType and the four
 * audio values 0. On a hit: the first contact (unk_AFC set, unk_B00 clear) is reported to the rider handler
 * (cRiderHandler_recordSprayBurst with the hit point, its normal, material and the length of its third vector); then unk_B00 = 1,
 * mSurfaceType = the material (0 for -1), mGroundNormal, mGroundContactPoint and GroundVelocity come from the hit, unk_454 =
 * the probe base's height above the hit, mAudioSpeed = speed * 0.00072 (at most 1), mAudioPressure 0.3, mAudioEdging 0,
 * and mAudioBraking = (speed - unk_B04) * 0.0036 (at most 1) when the speed rose since the last call, else 0: the sign
 * looks inverted, but On Tour computes it the same way. Both cases rebuild mGroundForward (row 1 of mMatrix projected
 * onto the ground plane, normalised) and mGroundSide (normal x forward), and store the speed in unk_B04.
 *
 * @param pRiderPos  the position to probe under (mPosition, or the board root bone; see cRider_updateSplit1)
 */
void cRider::SetNISGroundVectors(void* pRiderPos)
{
    char* vtObj = (char*)&VPTR_AT(this, sizeof(cRiderState));
    sVEntry<t4Vector*(void*)>* ve = &(*(sVEntry<t4Vector*(void*)>**)vtObj)[kRiderVt_getVelocity];
    float speed = Length_42B0(*ve->fn(vthis(vtObj, *ve)));
    float sideOffset = mAnimModel->ScaleVect.x * (mSlewed[0].cur * 45.0f);
    t4Vector probeBase = Add_42B0(*(t4Vector*)pRiderPos, Scale_42B0(*(&mGroundSide), sideOffset));
    t4Vector lo = Add_42B0(probeBase, Scale_42B0(*(&mGroundNormal), -100.0f));
    t4Vector hi = Add_42B0(probeBase, Scale_42B0(*(&mGroundNormal), 200.0f));
    tIntersectInfo hit;
    cWorldLine ray;
    cWorldLine_cWorldLine(&ray, lo, hi, 2, 0.5f);
    float hitDist = cOverlapList_terrainIntersect1(mOverlapList, &ray, &hit, 0);
    *(&LastGroundNormal) = *(&mGroundNormal);
    if (hitDist > 0.0f && hit.Patch != 0) {
        mGroundPolyId = hit.Patch->m_RID;
        PatchUV.x = hit.u;
        PatchUV.y = hit.v;
        SurfaceFlags = hit.Patch->m_sSurfaceFlags;
    } else {
        mGroundPolyId = 0xFFFFFFFF;
    }
    if (hitDist < 0.0f) {
        mOnGroundNIS = 0;
        *(&mGroundNormal) = t4vect_ZAxis_42B0;
        *(&GroundVelocity) = t4vect_Zero_42B0;
        *(&mGroundForward) = Normalize_42B0(SubV_42B0(*(&mMatrix.row[1]), Scale_42B0(*(&mGroundNormal),
                DotV_42B0(*(&mMatrix.row[1]), *(&mGroundNormal)))));
        *(&mGroundSide) = Cross_42B0(*(&mGroundNormal), *(&mGroundForward));
        mSurfaceType = 0;
        mAudioBraking = 0.0f;
        mAudioEdging = 0.0f;
        mAudioPressure = 0.0f;
        mAudioSpeed = 0.0f;
        mLastSpeed = speed;
        return;
    }
    if (mNISShowBoardVisualFX != 0 && mOnGroundNIS == 0) {
        cRiderHandler_recordSprayBurst_42B0(mRiderHandler, &hit, &hit.Inorm, hit.Stype, Length_42B0(hit.Ivel));
    }
    mOnGroundNIS = 1;
    mSurfaceType = hit.Stype;
    if (hit.Stype == eST_Undefined)
        mSurfaceType = eST_Default;
    *(&mGroundNormal) = hit.Inorm;
    *(&mGroundContactPoint) = hit.Ipos;
    *(&GroundVelocity) = hit.Ivel;
    *(&mGroundForward) = Normalize_42B0(SubV_42B0(*(&mMatrix.row[1]), Scale_42B0(*(&mGroundNormal),
            DotV_42B0(*(&mMatrix.row[1]), *(&mGroundNormal)))));
    *(&mGroundSide) = Cross_42B0(*(&mGroundNormal), *(&mGroundForward));
    SnowOffset = Dot_42B0(Sub_42B0(probeBase, hit.Ipos), *(&mGroundNormal));
    float speedDrop = mLastSpeed - speed;
    mAudioBraking = 0.0f;
    if (speedDrop < mAudioBraking) {
        float braking = -speedDrop * 0.0036000001709908247f;
        mAudioBraking = braking;
        if (1.0f < braking)
            mAudioBraking = 1.0f;
    }
    float speedLevel = speed * 0.0007200000109151006f;
    mAudioSpeed = speedLevel;
    if (1.0f < speedLevel)
        mAudioSpeed = 1.0f;
    mLastSpeed = speed;
    mAudioPressure = 0.30000001192092896f;
    mAudioEdging = 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_DoInSeqAnimUpdate);
#ifdef SKIP_ASM
// On Tour: tAnimID (bitfields SourceID:8, AnimID:24; ctor tAnimID(int s, int i)).
struct tAnimID {
    unsigned int SourceID : 8;
    unsigned int AnimID : 24;
    tAnimID(int s, int i) { AnimID = i; SourceID = s; }
};

void cScriptObjRider_ScriptAnimIDToGameAnimInfo(void* self, int id, void* outA, void* outB);
extern "C" void cRiderAnimBase_getAIAnimFrame(void* buf, void* anim, int id, tAnimID a3, float t);
extern "C" void func_0030ECD8(void* xf, void* model, void* buf, int a3);
extern "C" void cRiderAnimBase_changeOrientationOffset(void* self, void* xf);
extern "C" void cRiderAnimBase_changeHeadingOffset(void* self, float heading);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cRider_updateOrientationImplicit(cRider* self);
extern "C" float cRider_getStartGateKneeOffset(cRider* self);
extern "C" float cRider_getStartGateHorizOffset(void* self);
extern "C" void cRiderAnimBase_playFromAnimID(void* anim, int id, tAnimID animId, float t);
extern "C" cAnimSequence* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" void cAnimSequence_setSubAnimTime(void* p, int a1, float t);
extern "C" void cRiderAnimBase_setupAnimation(void* anim, int a1);
extern "C" void cRider_EulerToQuat(cRider* self, tQuaternion* out, float rx, float ry, float rz);
extern "C" void cRider_updateAnimation(cRider* self, float value);
extern "C" void cRider_updateSkeleton(void* self);
extern "C" int tEventFlags_isSet1(void* self, int a1);
extern "C" int tEventFlags_isJustChanged(void* self, int a1);
extern "C" void tEventFlags_clear1(void* self, int a1);
extern "C" void cAnimModel_enablePart(void* model, int i, int channel);
extern "C" void cAnimModel_disablePart(void* model, int i, int channel);
extern "C" void cRider_updateBoundBox_4788(cRider* self, sVec4* p) __asm__("cRider_updateBoundBox");
extern void cRider_updateCollideInfo_4788(cRider* self) __asm__("cRider_updateCollideInfo");

extern sVec4 t4vect_Origin_124788 __asm__("t4vect_Origin");


// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sVec4 QuatMul_124788(const sVec4& a, const sVec4& b)
{
    sVec4 r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4 Sub_124788(const sVec4& a, const sVec4& b)
{
    sVec4 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec4 Add_124788(const sVec4& a, const sVec4& b)
{
    sVec4 r;
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

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void AddEq_124788(sVec4& dst, sVec4 b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline sVec4 Rot_124788(const sVec4& q, const sVec4& v)
{
    sVec4 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4 Scale_124788(const sVec4& v, float s)
{
    sVec4 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 macro-mode asm (4x4 matrix times vector).
static inline sVec4 MatMulV_124788(const char* m, const sVec4& v)
{
    sVec4 r;
    __asm__(
        "lqc2       $vf8, %2\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "lqc2       $vf5, 0x10(%1)\n"
        "lqc2       $vf6, 0x20(%1)\n"
        "lqc2       $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2       $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

static inline sVec4 Conj_124788(const tPose& p)
{
    sVec4 r;
    r.w = (*(sVec4*)&p.Q).w;
    r.x = (*(sVec4*)&p.Q).x;
    r.y = (*(sVec4*)&p.Q).y;
    r.z = (*(sVec4*)&p.Q).z;
    r.x = -r.x;
    r.y = -r.y;
    r.z = -r.z;
    return r;
}

static inline tPose Invert_124788(const tPose& a)
{
    tPose r;
    (*(sVec4*)&r.Q) = Conj_124788(a);
    (*(sVec4*)&r.P) = Add_124788(Scale_124788(Rot_124788((*(sVec4*)&r.Q), Sub_124788((*(sVec4*)&a.P),
                                                                                      t4vect_Origin_124788)),
                                              -1.0f),
                                 t4vect_Origin_124788);
    return r;
}

static inline tPose Combine_124788(const tPose& a, const tPose& b)
{
    tPose r;
    (*(sVec4*)&r.Q) = QuatMul_124788((*(sVec4*)&a.Q), (*(sVec4*)&b.Q));
    (*(sVec4*)&r.P) = Add_124788(Rot_124788((*(sVec4*)&a.Q), Sub_124788((*(sVec4*)&b.P), t4vect_Origin_124788)),
                                 (*(sVec4*)&a.P));
    return r;
}

static inline sVec4 GetFront_124788(cRider* rider)
{
    return *(sVec4*)&rider->mMatrix.row[1];
}




// The unit declares cRider_DoInSeqAnimUpdate(void*, void*, float, float, int); bind the typed definition by asm label.
/**
 * Drives the rider from an in-game scripted sequence (NIS) frame: starts its animation and follows the scripted
 * position and angles.
 *
 * On a key event (@p eventBits bit 0) whose main anim starts at @p currFrame: maps the script anim to a game anim
 * (cScriptObjRider_ScriptAnimIDToGameAnimInfo), re-bases the rider so the anim's root starts at the hips (custom flag root), applies the start-gate
 * knee/horizontal offsets (drift), flips the heading offset when the anim's heading flag changes, and plays it from its
 * start frame at its speed (cRiderAnimBase_playFromAnimID, sequence 2). On bit 1 with no active anim it plays anim 5.
 *
 * Every call: moves the rider by the change of the script position (+0xAE0, through the script matrix) and turns it by
 * the change of the script Euler angles (+0xAF0..+0xAF8, degrees to radians), runs cRider_updateAnimation(@p
 * frameDelta) and cRider_updateSkeleton (skeleton), sets the velocity from the movement (or the board root's movement with
 * unk_AFC) per frame, toggles anim-model channel 0xB from the sequence's flags, and refreshes the bound box around the
 * hips and cRider_updateCollideInfo.
 *
 * The C definition is cRider_DoInSeqAnimUpdate with an asm label.
 *
 * @param pFrameInfo  the script actor's frame data (On Tour tSSXFrameActor)
 * @param currFrame  the sequence's current frame
 * @param frameDelta  frames advanced this update
 * @param eventBits  bit 0: key event on this frame; bit 1: the sequence ended
 */
void cRider::DoInSeqAnimUpdate(tSSXFrameActor* pFrameInfo, float currFrame, float frameDelta, int eventBits)
{
    if (eventBits & 1) {
        if (pFrameInfo->numMainAnim == 0) {
            return;
        }
        tMainAnim* pCurrMainAnim = pFrameInfo->pMainAnimList;
        tTagCustomMainAnim* pCurrMainAnimCustom = pCurrMainAnim->pCustom;
        if ((float)pCurrMainAnim->data.scriptStartFrame != currFrame) {
            return;
        }
        float startTime = pCurrMainAnim->data.currObjectFrame * 0.01666666753590107f;
        int animID;
        int sourceID;
        cScriptObjRider_ScriptAnimIDToGameAnimInfo(&mScriptObjRiderVtbl, pCurrMainAnim->data.objectID, &animID,
                &sourceID);
        tAnimID animInfo(sourceID, animID);
        if (pCurrMainAnimCustom->alignBody != 0) {
            char buf[80];
            cRiderAnimBase_getAIAnimFrame(buf, mRiderAnim, 432, animInfo, startTime);
            tPose animPose;
            func_0030ECD8(&animPose, mAnimModel, buf, 0);
            cAnimModel* trk = mAnimModel;
            sVec4* n = (sVec4*)((mBoneHips << 4) + (int)trk->localPos);
            tPose node;
            (*(sVec4*)&node.P) = sVec4(n->x * trk->ScaleVect.x, n->y * trk->ScaleVect.y,
                                 n->z * trk->ScaleVect.z, n->w * trk->ScaleVect.w);
            t4Vector* rot = &mAnimModel->localRot[mBoneHips];
            node.Q = *rot;
            tPose y = Combine_124788(node, Invert_124788(animPose));
            tPose* rx = (tPose*)&mPosition;
            *rx = Combine_124788(*rx, y);
            updateOrientationImplicit();
            tPose w = Invert_124788(y);
            cRiderAnimBase_changeOrientationOffset(mRiderAnim, &w);
        }
        if (pCurrMainAnimCustom->applySGOffsets != 0) {
            float kneeOffset = cRider_getStartGateKneeOffset(this);
            mSlewed[18].target = kneeOffset;
            mSlewed[18].rate = 0;
            mSlewed[18].cur = kneeOffset;
            AddEq_124788(*(sVec4*)&mPosition,
                         Scale_124788(GetFront_124788(this), cRider_getStartGateHorizOffset(this)));
            updateOrientationImplicit();
        }
        int isFlipped = pCurrMainAnimCustom->isFlipped != 0;
        if (isFlipped != mWasFlipped) {
            float headingOffset = 0.0f;
            if (isFlipped) {
                headingOffset = 3.1415927410125732f;
            }
            cRiderAnimBase_changeHeadingOffset(mRiderAnim, headingOffset);
            mWasFlipped = isFlipped;
        }
        float start = pCurrMainAnim->data.blendInFrames;
        mRiderAnim->FrameRateFactor = pCurrMainAnim->data.playRate;
        cRiderAnimBase_playFromAnimID(mRiderAnim, 432, animInfo, start * 0.01666666753590107f);
        cAnimSequence_setSubAnimTime(cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 2), 0, startTime);
        cRiderAnimBase_setupAnimation(mRiderAnim, 0);
        mRiderAnim->FrameRateFactor = 1.0f;
    } else if ((eventBits & 2) && pFrameInfo->numMainAnim == 0) {
        cRiderAnimBase_play(mRiderAnim, 5, 0, -1.0f);
    }
    sVec4 oldStart = *(sVec4*)&mScriptCurrPosOffset;
    *(sVec4*)&mScriptCurrPosOffset = sVec4(pFrameInfo->posX, pFrameInfo->posY, pFrameInfo->posZ, 1.0f);
    float prevAngX = mScriptCurrRotX;
    float prevAngY = mScriptCurrRotY;
    float prevAngZ = mScriptCurrRotZ;
    sVec4 oldPos = *(sVec4*)&mPosition;
    mScriptCurrRotX = pFrameInfo->rotX * 0.01745329424738884f;
    mScriptCurrRotY = pFrameInfo->rotY * 0.01745329424738884f;
    mScriptCurrRotZ = pFrameInfo->rotZ * 0.01745329424738884f;
    AddEq_124788(*(sVec4*)&mPosition,
                 MatMulV_124788((char*)&mScriptMatrix,
                                Sub_124788(*(sVec4*)&mScriptCurrPosOffset, oldStart)));
    sVec4 delta = Sub_124788(*(sVec4*)&mPosition, oldPos);
    *(sVec4*)&mVelocity = delta;
    EulerToQuat((tQuaternion*)&delta, mScriptCurrRotZ - prevAngZ, mScriptCurrRotY - prevAngY,
            mScriptCurrRotX - prevAngX);
    *(sVec4*)&mRotation = QuatMul_124788(delta, *(sVec4*)&mRotation);
    updateOrientationImplicit();
    sVec4* bone = (sVec4*)&mAnimModel->bones[mBoneBoardRoot].pos;
    sVec4 before = *bone;
    updateAnimation(frameDelta);
    cRider_updateSkeleton(this);
    if (mNISShowBoardVisualFX != 0) {
        *(sVec4*)&mVelocity = Sub_124788(*bone, before);
    }
    if (frameDelta > 0.0f) {
        mVelocity.x = mVelocity.x / frameDelta;
        mVelocity.y = mVelocity.y / frameDelta;
        mVelocity.z = mVelocity.z / frameDelta;
    }
    tEventFlags* ch = &cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 2)->Events;
    int on = 0;
    if (tEventFlags_isSet1(ch, 0) != 0) {
        on = tEventFlags_isJustChanged(ch, 0) != 0;
    }
    if (on) {
        cAnimModel_enablePart(mAnimModel, 11, 0);
        tEventFlags_clear1(ch, 0);
    } else {
        int on2 = 0;
        if (tEventFlags_isSet1(ch, 1) != 0) {
            on2 = tEventFlags_isJustChanged(ch, 1) != 0;
        }
        if (on2) {
            cAnimModel_disablePart(mAnimModel, 11, 0);
            tEventFlags_clear1(ch, 1);
        }
    }
    sAnimModelBone* hips = &mAnimModel->bones[mBoneHips];
    sVec4 hipsPos = *(sVec4*)&hips->pos;
    hipsPos.z = mPosition.z;
    cRider_updateBoundBox_4788(this, &hipsPos);
    cRider_updateCollideInfo_4788(this);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_cancelRiderTransitions);
#ifdef SKIP_ASM
cTransitionMan* cGame_getTransitionMan(void* self, int id);
extern "C" void cTransitionMan_EndTransition(void* p);

/**
 * Cancels a camera transition in progress on the rider's view.
 *
 * When the game has a view manager and the rider a view, and Game->TransitionMan[mViewIndex] was started by a rider
 * (eTransitionInitiator_Rider), resets that transition manager with cTransitionMan_EndTransition.
 */
void cRider::cancelRiderTransitions()
{
    cGame* w = SSXApp->Game;
    if (w->ViewMan != 0 && mViewIndex >= 0)
    {
        if (cGame_getTransitionMan(w, mViewIndex)->mCurrentInitiator == eTransitionInitiator_Rider)
            cTransitionMan_EndTransition(cGame_getTransitionMan(SSXApp->Game, mViewIndex));
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_applyBoost);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_startLaunchIntoAir(void* self, void* a, void* b, float t);


// PORT: PS2-only VU0 inline asm (a += b).
static inline void riderAddEq_001250A8(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b));
}

/**
 * Adds an impulse to the rider's velocity, relaunching its air prediction when airborne.
 *
 * In MS_Air, cAirPredictor_startLaunchIntoAir() restarts from mPosition and the new mVelocity (last argument
 * 3333.33).
 *
 * @param impulse  the velocity change (On Tour: const t4Vector & f)
 */
void cRider::applyBoost(t4Vector* impulse)
{
    t4Vector* velocity = (&mVelocity);
    riderAddEq_001250A8(*velocity, *impulse);
    if (getMotionState() == MS_Air)
        cAirPredictor_startLaunchIntoAir(mAirPredictor, &mPosition, velocity, 3333.33349609375f);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_doFinishEvent);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_evFinishRace(void* self);
extern "C" void cChaseCameraController_switchCamera_EndRace(void* self);
extern "C" void cGameModeMan_raceFinish(void* self, int riderIndex, tRiderStats* stats);
extern "C" void cSSXAudio_OnFinishEvent(void* self, void* rider);
extern "C" cSSXAudio* cAudioInterface_bxAudio();


static inline void* getC0_125108()
{
    return SSXApp->GameModeMan;
}

/**
 * Records that the rider has finished (or did not finish) and tells the game mode, camera and audio.
 *
 * Online (cGameComm present), a human who finished sets the local or remote finished flag. Calls the metrix
 * finish (cRiderMetrix_evFinishRace), sets mFinishTick to the race clock plus mPenaltyTicks and mFinishTime to at least
 * 0, reports cGameModeMan_raceFinish(AI index, the metrix's Totals at +0xFC), switches a split-screen rider's
 * chase camera to the end-race camera and calls cSSXAudio_OnFinishEvent on the audio system. A rider who did not finish
 * also gets ShouldCelebrateAtFinish cleared.
 */
void cRider::doFinishEvent()
{
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0 && mDidNotFinish == 0 && mIsHuman != 0)
    {
        if (mIsLocal != 0)
            gameComm->mLocalFinishedRace = 1;
        else
            gameComm->mRemoteFinishedRace = 1;
    }
    cRiderMetrix_evFinishRace(mMetrix);
    mFinishTick = SSXApp->Game->AI->EventTimerTick + mPenaltyTicks;
    if (mFinishTime < 0.0f) mFinishTime = 0.0f;
    cGameModeMan_raceFinish(getC0_125108(), getAIIndex(), &mMetrix->Totals);
    unsigned int k = mViewIndex;
    if (k < 2)
    {
        cGameViewMan* tbl = SSXApp->Game->ViewMan;
        cChaseCameraController_switchCamera_EndRace(tbl->PlayerCamera[k]->ChaseController);
    }
    cSSXAudio_OnFinishEvent(cAudioInterface_bxAudio(), this);
    if (mDidNotFinish == true) ShouldCelebrateAtFinish = 0;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateFinishTest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cRider_doFinishEvent(cRider* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_SpeakArcadeResult(void* a, void* self, int n);
extern "C" unsigned int fptoui(float f);
extern int D_005308D0[];

/**
 * Checks whether a human rider has finished, applying a timed event's time limit.
 *
 * For a human who has not finished, outside game mode kGAMEMODE_FREERIDE: in a timed event (GameModeMan
 * bIsTimedEvent) with race-rule bit 9 clear, once the race clock passes uiTimeLimit (rounded down to whole seconds)
 * the rider gets mDidNotFinish, its finish event and an audio cue (cSSXAudio_SpeakArcadeResult). A rider already marked DNF gets its
 * finish event when the game state is kGFSM_Riding.
 *
 * @return true when the rider has finished (mFinishTime >= 0)
 */
bool cRider::updateFinishTest()
{
    if (isHuman()) {
        float finishTime = mFinishTime;
        float zeroF = 0.0f;
        int done = finishTime >= zeroF;
        if (!done) {
            cBE_getInterface(cBE_getBE(), 0);
            if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_FREERIDE)
                return mFinishTime >= zeroF;
            cSSXApp* app = SSXApp;
            cGameModeMan* gameModeMan = app->GameModeMan;
            if (gameModeMan->bIsTimedEvent != 0 && ((D_005308D0[0] >> 9) & 1) == 0) {
                int eventTicks = app->Game->AI->EventTimerTick;
                int secs = fptoui((float)gameModeMan->uiTimeLimit * 0.01666666753590107f);
                if (secs * 60 < eventTicks) {
                    mDidNotFinish = 1;
                    doFinishEvent();
                    cSSXAudio_SpeakArcadeResult(cAudioInterface_bxAudio(), this, 2);
                    goto end;
                }
            }
            if (mDidNotFinish == true && SSXApp->Game->mCurrentStateID == kGFSM_Riding)
                doFinishEvent();
        }
    }
end:
    return mFinishTime >= 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_quitEvent);
#ifdef SKIP_ASM

extern int cGameModeMan_bGiveUp[];

static inline bool riderHasFinished001253D0(cRider* rider)
{
    return rider->mFinishTime >= 0.0f;
}

/**
 * Marks a rider who quits before finishing as did-not-finish.
 *
 * Sets mDidNotFinish and the rider's entry in the per-rider table cGameModeMan_bGiveUp, which the front-end popups
 * read.
 */
void cRider::quitEvent()
{
    bool finished = riderHasFinished001253D0(this);
    if (!finished)
    {
        mDidNotFinish = 1;
        cGameModeMan_bGiveUp[getAIIndex()] = 1;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_DoInSeqSoundUpdate);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int cBankMonitor_BANKMONITOR_GetHandle(void* bank, int id, int a);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* bank, int id, int a, float f);
void cScriptObjRider_ScriptSoundIDToGameSoundInfo(void* self, int a1, void* a2, void* a3);
extern "C" float* cSSXAudio_GetCurrAppliedVolumePtr(void* self, int volumeClass, int index);
extern "C" int cSSXAudio_GetClosestCamera1(void* self, void* rider);
extern "C" int cBankSys_SFX_End(void* self);
extern "C" void* memset(void* dst, int c, int n);

#define MIN_5448(a, b) ((a) < (b) ? (a) : (b))
#define MAX_5448(a, b) ((a) < (b) ? (b) : (a))

/**
 * Plays and stops the rider's sounds for an in-game scripted sequence (NIS) frame.
 *
 * First forgets any of the rider's 4 sequence sound handles (mSoundHandles) the bank monitor no longer has. On a key
 * event (bit 0 of @p flags) it walks the frame's sound list while each entry's frame equals @p when, maps
 * the script sound id (cScriptObjRider_ScriptSoundIDToGameSoundInfo), fills a new sound-play slot in the audio system with defaults, the entry's
 * volume (clamped 0..127), the volume control (cSSXAudio_GetCurrAppliedVolumePtr) and the rider's position and closest camera
 * (cSSXAudio_GetClosestCamera1), starts it (cBankSys_SFX_End) and keeps the handle in the entry's slot. At the end of the sequence
 * (bit 1) every slot no longer in the list is stopped.
 *
 * @param vobj  the script actor's frame data (a tSSXFrameActor, On Tour pFrameInfo); walks its pSoundList
 * @param when  the sequence's current frame (On Tour currFrame)
 * @param frameDelta  frames advanced (On Tour frameDelta); unused
 * @param flags  On Tour eventBits: bit 0 key event, bit 1 sequence ended
 */
void cRider::DoInSeqSoundUpdate(void* vobj, float when, float frameDelta, int flags)
{
    cRider* self = (cRider*)this;
    tSSXFrameActor* frame = (tSSXFrameActor*)vobj;
    int* ids = self->mSoundHandles;
    cSSXAudio* snd = cAudioInterface_bxAudio();
    int i;
    for (i = 0; i < 4; i++) {
        if (ids[i] >= 0) {
            if (cBankMonitor_BANKMONITOR_GetHandle(&snd->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor, ids[i],
                                                   1) == 0)
                ids[i] = -1;
        }
    }
    if (flags & 1) {
        tSound* curSound = frame->pSoundList;
        if (curSound != 0) {
        const t4Vector* riderPos = &self->mPosition;
        do {
            if ((float)curSound->data.scriptStartFrame != when)
                return;
            int gameSoundIndex, type;
            cScriptObjRider_ScriptSoundIDToGameSoundInfo(&self->mScriptObjRiderVtbl, curSound->data.objectID, &gameSoundIndex, &type);
            int tp = type;
            int ar = gameSoundIndex;
            // the play-option push (cSndPlayOpts_OPTS_Open) inlined on the bank manager's cBankMonitor
            cBankManager* curTrack = snd->mpVBankManager;
            cBankMonitor* trackSub = &curTrack->BankMonitor;
            curTrack->BankMonitor.mInst++;
            trackSub->mCurrent[curTrack->BankMonitor.mInst] = trackSub->mDefault;
            curTrack->BankMonitor.mPosition[curTrack->BankMonitor.mInst] = 0;
            trackSub->mPosStatic[curTrack->BankMonitor.mInst] = 0;
            trackSub->mBaseVol[curTrack->BankMonitor.mInst] = trackSub->mCurrent[curTrack->BankMonitor.mInst].vol;
            trackSub->mVolControl[curTrack->BankMonitor.mInst] = 0;
            trackSub->mCallback[curTrack->BankMonitor.mInst] = 0;
            trackSub->mPtr[curTrack->BankMonitor.mInst] = 0;
            trackSub->mViewport[curTrack->BankMonitor.mInst] = 0;
            trackSub->mVanish[curTrack->BankMonitor.mInst] = 0;
            trackSub->mRadius[curTrack->BankMonitor.mInst] = 0;
            trackSub->mDelay[curTrack->BankMonitor.mInst].mCount = 0;
            trackSub->mDelay[curTrack->BankMonitor.mInst].mTime = 100;
            trackSub->mDelay[curTrack->BankMonitor.mInst].mFeedback = 90;
            trackSub->mDelay[curTrack->BankMonitor.mInst].mLevel = 50;
            trackSub->mExternalVol[curTrack->BankMonitor.mInst] = 0;
            trackSub->mTwoPlayerPan[curTrack->BankMonitor.mInst] = 0;
            trackSub->mTwoPlayerSide[curTrack->BankMonitor.mInst] = 0;
            trackSub->mMapToMaxVol[curTrack->BankMonitor.mInst] = 0x7F;
            trackSub->mCloseOnEnd[curTrack->BankMonitor.mInst] = 1;
            snd->mBank[snd->mpVBankManager->BankMonitor.mInst] = tp;
            snd->mPatch[snd->mpVBankManager->BankMonitor.mInst] = ar;
            {
                cBankManager* volTrack = snd->MusicSys.mpVStreamSys->mpVBankManager;
                cBankMonitor* volOpts = &volTrack->BankMonitor;
                volOpts->mBaseVol[volTrack->BankMonitor.mInst] = curSound->pCustom->volume;
                volOpts->mCurrent[volTrack->BankMonitor.mInst].vol =
                    MAX_5448(MIN_5448(volOpts->mBaseVol[volOpts->mInst], 0x7F), 0);
            }
            {
                cBankManager** pt = &snd->MusicSys.mpVStreamSys->mpVBankManager;
                float* volControl = cSSXAudio_GetCurrAppliedVolumePtr(snd, 4, 0);
                (*pt)->BankMonitor.mVolControl[(*pt)->BankMonitor.mInst] = volControl;
            }
            {
                cBankManager* posTrack = snd->MusicSys.mpVStreamSys->mpVBankManager;
                posTrack->BankMonitor.mPosition[posTrack->BankMonitor.mInst] = riderPos;
                cBankManager** pt = &snd->MusicSys.mpVStreamSys->mpVBankManager;
                int camera = cSSXAudio_GetClosestCamera1(snd, self);
                (*pt)->BankMonitor.mViewport[(*pt)->BankMonitor.mInst] = camera;
            }
            int handle = cBankSys_SFX_End(snd);
            int& handleSlot = ids[curSound->data.trackIndex];
            handleSlot = handle;
            curSound = curSound->pNext;
        } while (curSound != 0);
        }
    } else if (flags & 2) {
        int keep[4];
        int on = 1;
        for (i = 3; i >= 0; i--)
            keep[i] = on;
        tSound* curSound;
        for (curSound = frame->pSoundList; curSound != 0; curSound = curSound->pNext)
            keep[curSound->data.trackIndex] = 0;
        for (i = 0; i < 4; i++) {
            if (keep[i] != 0) {
                if (ids[i] >= 0) {
                    cBankMonitor_BANKMONITOR_StopHandle(&snd->MusicSys.mpVStreamSys->mpVBankManager->BankMonitor,
                                                        ids[i], 1, 0.0f);
                    ids[i] = -1;
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_setUbersAllowed);
#ifdef SKIP_ASM
/**
 * Sets which uber tricks the rider may do; 0 also cancels any uber in progress.
 *
 * Stores the flags in mUberFlags (3 by default); 0 also clears mUberLevel and mUberTimer.
 *
 * @param uberAllowedFlags  uber permission flags (On Tour: uberAllowedFlags)
 */
void cRider::setUbersAllowed(int uberAllowedFlags)
{
    mUberFlags = uberAllowedFlags;
    if (uberAllowedFlags == 0) {
        mUberLevel = 0;
        mUberTimer = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateWind);
#ifdef SKIP_ASM
extern "C" float cRenderStateMan_PlayerWindIntensity(int rider);
extern "C" float cRenderStateMan_WindRotation(int rider);
extern float cSinCosTable_gSinCosTable[];



// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_125970(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_125970(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

/**
 * Pushes a human rider along the wind of its environment.
 *
 * The wind speed (cRenderStateMan_PlayerWindIntensity x 27.78, capped at 1388.9) and direction (cRenderStateMan_WindRotation, in degrees) come
 * from the rider's environment entry; below 277.78 nothing happens. When the rider's velocity along the wind
 * is lower than the wind speed, the applyBoost virtual gets the wind direction times the difference times
 * 0.015.
 */
void cRider::updateWind()
{
    if (mIsHuman == 0)
        return;
    float maxSpeed = 1388.888916015625f;
    float speed = cRenderStateMan_PlayerWindIntensity(mAIIndex) * 27.77777862548828f;
    if (speed > maxSpeed)
        speed = maxSpeed;
    if (speed >= 277.77777099609375f)
    {
        float ang = cRenderStateMan_WindRotation(mAIIndex) * 0.01745329424738884f;
        t4Vector dir;
        dir.z = 0.0f;
        dir.w = 0.0f;
        int idx = (int)(ang * 81.4873275756836f) & 0x1FF;
        dir.y = cSinCosTable_gSinCosTable[idx];
        dir.x = cSinCosTable_gSinCosTable[idx + 0x80];
        float speedDeficit = speed - Dot_125970(*(&mVelocity), dir);
        if (speedDeficit > dir.z)
        {
            t4Vector imp = Scale_125970(Scale_125970(dir, speedDeficit), 0.015000000596046448f);
            applyBoost(&imp);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateTestSurface__FPv);
#ifdef SKIP_ASM
/**
 * Debug hook for testing surface types; compiled out, does nothing.
 */
void cRider::updateTestSurface()
{
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_syncCheckSum);
#ifdef SKIP_ASM
/**
 * Returns a cheap checksum of the rider's state: the float sum of its position and rotation, as bits.
 *
 * Adds mPosition x, y, z and mRotation x, y, z, w (On Tour returns an unsigned int).
 *
 * @return the sum's bit pattern
 */
int cRider::syncCheckSum()
{
    union { float f; int i; } sumBits;
    sumBits.f = mPosition.x + mPosition.y + mPosition.z
        + (mRotation.x + mRotation.y + mRotation.z + mRotation.w);
    return sumBits.i;
}
#endif

//100%
INCLUDE_ASM("ai/rider", updatePadVibration);
#ifdef SKIP_ASM
extern "C" void cPadDevice_clearAllVibrations(void* obj);


/**
 * Decays a player's two vibration levels and drives the pad's two motors from them.
 *
 * With @p on 0 both levels are zeroed and cPadDevice_clearAllVibrations stops the pad. Otherwise level 0 (bump) becomes max(level x
 * 0.9133 - 18.5, 0) and level 1 (rumble) is multiplied by 0.99167; when the pad's virtual slot 3 returns 2, slot 4 sets
 * motor 0 to max((rumble - 0.5) x 0.01, (bump - 2) / 972.2, 0) and motor 1 to max((rumble - 70) / 160, (bump - 100) /
 * 5000, 0). Called by cPlayer_vibrationUpdate with &mVibrationBump.
 *
 * @param vibState  the two levels (bump, rumble)
 * @param controller  the cPadDevice
 * @param on  0 to stop
 */
extern "C" void updatePadVibration(void* vibState, void* controller, int on)
{
    float* vibLevels = (float*)vibState;
    void* obj = controller;
    if (on == 0)
    {
        vibLevels[0] = vibLevels[1] = 0.0f;
        cPadDevice_clearAllVibrations(obj);
        return;
    }
    float decayed = vibLevels[0] * 0.9133333563804626f - 18.518518447875977f;
    float clamped = 0.0f;
    if (decayed >= 0.0f)
        clamped = decayed;
    vibLevels[0] = clamped;
    vibLevels[1] *= 0.9916666746139526f;
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)obj;
    if (vt[3].fn(vthis(obj, vt[3])) == 2)
    {
        sVEntry<void(void*, float, int)>* vt2 = *(sVEntry<void(void*, float, int)>**)obj;
        vt2[4].fn(vthis(obj, vt2[4]), ((vibLevels[1] - 0.5f) * 0.009999999776482582f >? (vibLevels[0] - 2.0f) / 972.2222290039062f) >? 0.0f, 0);
        sVEntry<void(void*, float, int)>* vt3 = *(sVEntry<void(void*, float, int)>**)obj;
        vt3[4].fn(vthis(obj, vt3[4]), ((vibLevels[1] - 70.0f) * 0.0062500000931322575f >? (vibLevels[0] - 100.0f) / 5000.0f) >? 0.0f, 1);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cPlayer_cPlayer);
#ifdef SKIP_ASM
// g++ 2.95 vtable entry (no thunks): {delta, index, fn}. Vtables are double-aligned.
struct sVtEnt_125C70 {
    short delta;
    short index;
    void* fn;
};
struct sVt9_125C70 {
    sVtEnt_125C70 e[9];
} __attribute__((aligned(8)));
struct sVt22_125C70 {
    sVtEnt_125C70 e[22];
} __attribute__((aligned(8)));
extern const sVt9_125C70 D_00458360;
extern const sVt22_125C70 D_004583A8;
extern char cRider_cScriptObjRider_vtbl[];
extern char cPlayer_vtbl[];
extern "C" void cRider_cRider(void* self);
extern "C" void cRiderHandler_cRiderHandler(void* self, int inChrg);
extern "C" void* cReplay_addCache(void* p);

/**
 * Constructs a human player: the shared cRider virtual base, the rider handler and cPlayer's fields.
 *
 * In charge, points the 31 rider parts' virtual-base pointers at the cRider at +0xE10 and constructs it. Runs
 * the cRiderHandler constructor (not in charge) and installs cPlayer's vtables in the cRider. Not in charge,
 * two of them are then replaced by stack copies with adjusted deltas (the g++ 2.95 pattern the PORT note
 * describes), which leaves the rider pointing into this function's stack frame.
 *
 * Then sets the cPlayer vtable (mVtbl), clears Pad and the two rumble floats, sets the rider's mIsHuman and adds
 * a replay cache (ReplayCache = cReplay_addCache).
 *
 * @param inChrg  nonzero when this object is the most derived one and must build the virtual base
 * @return @p self
 */
// PORT: hand-written form of g++ 2.95's constructor for a class with a virtual base
// (cRider at +0xE10): vtable copies with delta fixups when not in charge.
extern "C" void* cPlayer_cPlayer(cPlayer* self, int inChrg)
{
    sVt9_125C70 ctorVtbl9;
    sVt22_125C70 ctorVtbl22;
    if (inChrg) {
        // cRiderHandler's layout (ai/riderhandler.h) names the parts: cPlayer keeps that base as raw bytes
        cRiderHandler* handler = (cRiderHandler*)self;
        cRider* vb = &self->Rider;
        handler->CheatFX.mRider = vb;
        handler->PunchFX.mRider = vb;
        handler->DynamicSprayFX.mRider = vb;
        handler->PlayerIndicatorFX.mRider = vb;
        handler->AngerFX.mRider = vb;
        handler->AirStreamerFX.mRider = vb;
        handler->TrickBoostFX.mRider = vb;
        handler->SpeedBoostFX.mRider = vb;
        handler->BoardTrailFX.mRider = vb;
        handler->BoardSparkFX.mRider = vb;
        handler->BoardWakeFX.mRider = vb;
        handler->VoidControl.mRider = vb;
        handler->UberGrindControl.mRider = vb;
        handler->HandplantControl.mRider = vb;
        handler->FinishLineControl.mRider = vb;
        handler->ResetControl.mRider = vb;
        handler->WipeoutControl.mRider = vb;
        handler->RailslideControl.mRider = vb;
        handler->GateControl.mRider = vb;
        handler->SpinControl.mRider = vb;
        handler->NaturalAirControl.mRider = vb;
        handler->BumpControl.mRider = vb;
        handler->PrewindControl.mRider = vb;
        handler->BoardPressControl.mRider = vb;
        handler->CruiseControl.mRider = vb;
        handler->HandplantMotion.mRider = vb;
        handler->RailslideMotion.mRider = vb;
        handler->StaticMotion.mRider = vb;
        handler->WipeoutMotion.mRider = vb;
        handler->AirMotion.mRider = vb;
        self->mRider = vb;
        cRider_cRider(vb);
    }
    cRiderHandler_cRiderHandler(self, 0);
    self->mRider->mScriptObjectVtbl = (void*)&D_00458360;
    self->mRider->mScriptObjRiderVtbl = cRider_cScriptObjRider_vtbl;
    VPTR_AT(self->mRider, sizeof(cRiderState)) = (void**)&D_004583A8;
    if (inChrg == 0) {
        int vc;
        ctorVtbl9 = D_00458360;
        self->mRider->mScriptObjectVtbl = &ctorVtbl9;
        {
            char* vbo = (char*)self->mRider - 0xE10;
            vc = (char*)self - vbo;
        }
        ctorVtbl9.e[1].delta = D_00458360.e[1].delta + vc;
        ctorVtbl22 = D_004583A8;
        VPTR_AT(self->mRider, sizeof(cRiderState)) = (void**)&ctorVtbl22;
        ctorVtbl22.e[1].delta = D_004583A8.e[1].delta + vc;
        ctorVtbl22.e[16].delta = D_004583A8.e[16].delta + vc;
        ctorVtbl22.e[17].delta = D_004583A8.e[17].delta + vc;
        ctorVtbl22.e[18].delta = D_004583A8.e[18].delta + vc;
        ctorVtbl22.e[19].delta = D_004583A8.e[19].delta + vc;
        ctorVtbl22.e[20].delta = D_004583A8.e[20].delta + vc;
    }
    self->mVtbl = (void**)cPlayer_vtbl;
    self->Pad = 0;
    self->mVibrationBump = 0;
    self->mVibrationRumble = 0;
    self->mRider->mIsHuman = 1;
    self->ReplayCache = (cReplayCache*)cReplay_addCache(SSXApp->Game->Replay);
    return self;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cPlayer__cPlayer);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/rider_classes.h"
#include "object/being.h"

/**
 * Destroys a player rider: its replay cache, its effect and wipeout-motion members, then the cRider base.
 *
 * Written as the destructor of cRider_125EB8 (the player layout in ai/rider_classes.h) and exported under the address
 * name by an asm alias: cReplay_removeCache(game replay, mDF8), then g++ destroys the members (cCheatFX, cPunchFX,
 * cDynamicSprayFX, cPlayerIndicatorFX, cAngerFX, cAirStreamerFX, cTrickBoostFX, cSpeedBoostFX, cBoardTrailFX,
 * cBoardSparkFX, cBoardWakeFX, cWipeoutMotion) and the virtual base (cRider__cRider), and frees the object when the
 * in-charge flag asks.
 */
cRider_125EB8::~cRider_125EB8()
{
    cReplay_removeCache(SSXApp->Game->Replay, mDF8);
}

// PORT: the compiler names this dtor _$_13cRider_125EB8; expose it under the address-derived name.
__asm__(".globl cPlayer__cPlayer\n.type cPlayer__cPlayer,@function\ncPlayer__cPlayer = _$_13cRider_125EB8\n.size cPlayer__cPlayer,0x1838\n");

// PORT: the out-of-line destructors called here, under their address-derived names (as seg/seg_1218 does).
__asm__("_$_14cRiderVb_rider = cRider__cRider\n"
        "_$_15cVbDtor00136AD0 = cWipeoutMotion__cWipeoutMotion\n"
        "_$_15cVbDtor002DCDA0 = cBoardWakeFX__cBoardWakeFX\n"
        "_$_15cVbDtor002DA8E8 = cBoardSparkFX__cBoardSparkFX\n"
        "_$_15cVbDtor002E83F0 = cBoardTrailFX__cBoardTrailFX\n"
        "_$_15cVbDtor002E64C0 = cSpeedBoostFX__cSpeedBoostFX\n"
        "_$_15cVbDtor002EAC60 = cTrickBoostFX__cTrickBoostFX\n"
        "_$_15cVbDtor002EF530 = cAirStreamerFX__cAirStreamerFX\n"
        "_$_15cVbDtor002D4A70 = cAngerFX__cAngerFX\n"
        "_$_15cVbDtor002E37D0 = cPlayerIndicatorFX__cPlayerIndicatorFX\n"
        "_$_15cVbDtor002DF1C8 = cDynamicSprayFX__cDynamicSprayFX\n"
        "_$_15cVbDtor002F0FE8 = cPunchFX__cPunchFX\n"
        "_$_15cVbDtor002F6388 = cCheatFX__cCheatFX\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$13cRider_125EB8 = cPlayer_vtbl\n"
        "_vt$13cRider_125EB8$15cRiderVbC_rider = D_00458360\n"
        "_vt$13cRider_125EB8$14cRiderVb_rider = D_004583A8\n"
        "_vt$17cRiderBase_125EB8 = cRiderHandler_vtbl\n"
        "_vt$16cRiderVbB0_rider = cRider_cScriptObjRider_vtbl\n"
        "_vt$16cVbPart3A0_rider$15cRiderVbC_rider = D_00458928\n"
        "_vt$16cVbPart3A0_rider$14cRiderVb_rider = D_00458970\n"
        "_vt$16cVbPart390_rider$15cRiderVbC_rider = D_00458D08\n"
        "_vt$16cVbPart390_rider$14cRiderVb_rider = D_00458D50\n"
        "_vt$16cVbPart370_rider$15cRiderVbC_rider = D_00458830\n"
        "_vt$16cVbPart370_rider$14cRiderVb_rider = D_00458878\n"
        "_vt$16cVbPart360_rider$15cRiderVbC_rider = D_00458B18\n"
        "_vt$16cVbPart360_rider$14cRiderVb_rider = D_00458B60\n"
        "_vt$16cVbPart350_rider$15cRiderVbC_rider = D_00458A20\n"
        "_vt$16cVbPart350_rider$14cRiderVb_rider = D_00458A68\n"
        "_vt$16cVbPart2C0_rider$14cRiderVb_rider = D_00458C58\n"
        "_vt$16cVbPart2C0_rider$15cRiderVbC_rider = D_00458C10\n"
        "_vt$16cVbPart2B0_rider$14cRiderVb_rider = D_00458E48\n"
        "_vt$16cVbPart2B0_rider$15cRiderVbC_rider = D_00458E00\n"
        "_vt$16cVbPart290_rider$14cRiderVb_rider = D_00458F40\n"
        "_vt$16cVbPart290_rider$15cRiderVbC_rider = D_00458EF8\n"
        "_vt$16cVbPart230_rider$15cRiderVbC_rider = D_00458FF0\n"
        "_vt$16cVbPart230_rider$14cRiderVb_rider = D_00459038\n"
        "_vt$16cVbPart210_rider$15cRiderVbC_rider = D_004590E8\n"
        "_vt$16cVbPart210_rider$14cRiderVb_rider = D_00459130\n"
        "_vt$16cVbPart200_rider$15cRiderVbC_rider = D_004591E0\n"
        "_vt$16cVbPart200_rider$14cRiderVb_rider = D_00459228\n"
        "_vt$16cVbPart1F0_rider$15cRiderVbC_rider = D_004592D8\n"
        "_vt$16cVbPart1F0_rider$14cRiderVb_rider = D_00459320\n"
        "_vt$16cVbPart1D0_rider$15cRiderVbC_rider = D_004593D0\n"
        "_vt$16cVbPart1D0_rider$14cRiderVb_rider = D_00459418\n"
        "_vt$16cVbPart1C0_rider$15cRiderVbC_rider = D_004594C8\n"
        "_vt$16cVbPart1C0_rider$14cRiderVb_rider = D_00459510\n"
        "_vt$16cVbPart110_rider$15cRiderVbC_rider = D_004595C0\n"
        "_vt$16cVbPart110_rider$14cRiderVb_rider = D_00459608\n"
        "_vt$16cVbPart0B0_rider$15cRiderVbC_rider = D_004598A8\n"
        "_vt$16cVbPart0B0_rider$14cRiderVb_rider = D_004598F0\n"
        "_vt$16cVbPart0A0_rider$15cRiderVbC_rider = D_004596B8\n"
        "_vt$16cVbPart0A0_rider$14cRiderVb_rider = D_00459700\n"
        "_vt$16cVbPart020_rider$15cRiderVbC_rider = D_004599A0\n"
        "_vt$16cVbPart020_rider$14cRiderVb_rider = D_004599E8\n"
        "_vt$16cVbPart000_rider$15cRiderVbC_rider = D_00459A98\n"
        "_vt$16cVbPart000_rider$14cRiderVb_rider = D_00459AE0\n"
        "_vt$17cRiderBase_125EB8$15cRiderVbC_rider = D_00458738\n"
        "_vt$17cRiderBase_125EB8$14cRiderVb_rider = D_00458780\n");
#endif

//100%
INCLUDE_ASM("ai/rider", cPlayer_getGrabHeld);
#ifdef SKIP_ASM
int cInput_getState(void*, int);
/**
 * Returns which grab input the player holds: the first of input states 31-45 that is set, as 0-14, or -1.
 *
 * Reads Input (+0xDF0) with cInput_getState. cPlayer_getControlCmd packs it into the command in control states 4 and 5.
 *
 * @return the grab index, or -1
 */
int cPlayer::getGrabHeld()
{
    if (cInput_getState(Input, 0x1F))
        return 0;
    if (cInput_getState(Input, 0x20))
        return 1;
    if (cInput_getState(Input, 33))
        return 2;
    if (cInput_getState(Input, 34))
        return 3;
    if (cInput_getState(Input, 35))
        return 4;
    if (cInput_getState(Input, 36))
        return 5;
    if (cInput_getState(Input, 37))
        return 6;
    if (cInput_getState(Input, 38))
        return 7;
    if (cInput_getState(Input, 39))
        return 8;
    if (cInput_getState(Input, 40))
        return 9;
    if (cInput_getState(Input, 41))
        return 10;
    if (cInput_getState(Input, 42))
        return 11;
    if (cInput_getState(Input, 43))
        return 12;
    if (cInput_getState(Input, 44))
        return 13;
    if (cInput_getState(Input, 45))
        return 14;
    return -1;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cPlayer_getUberGrindHeld);
#ifdef SKIP_ASM
int cInput_getState(void*, int);

/**
 * Returns which uber-grind input the player holds: the first of input states 27-30 that is set, as 0-3, or -1.
 *
 * cPlayer_getControlCmd packs it into the command in control states 7 and 12.
 *
 * @return the grind index, or -1
 */
int cPlayer::getUberGrindHeld()
{
    if (cInput_getState(Input, 27))
        return 0;
    if (cInput_getState(Input, 0x1C))
        return 1;
    if (cInput_getState(Input, 29))
        return 2;
    if (cInput_getState(Input, 0x1E))
        return 3;
    return -1;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cPlayer_vibrationReset__FPv);
#ifdef SKIP_ASM
/**
 * Clears the player's two vibration levels, mVibrationBump and mVibrationRumble (cRider virtual slot 16).
 */
void cPlayer::vibrationReset()
{
    mVibrationRumble = 0.0f;
    mVibrationBump = 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cPlayer_vibrationBump);
#ifdef SKIP_ASM
/**
 * Raises the player's bump vibration level (mVibrationBump) to @p val when that is higher (cRider virtual slot 17).
 *
 * @param val  the bump strength
 */
void cPlayer::vibrationBump(float val)
{
    mVibrationBump = val >? mVibrationBump;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cPlayer_vibrationRumble__FPvf);
#ifdef SKIP_ASM
/**
 * Sets the player's rumble vibration level (mVibrationRumble) to @p val (cRider virtual slot 18).
 *
 * @param val  the rumble strength
 */
void cPlayer::vibrationRumble(float val)
{
    mVibrationRumble = val;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cPlayer_vibrationUpdate);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getUseDualShock(void* iface, int id);
extern "C" void updatePadVibration(void* vibState, void* controller, int on);


static inline bool riderInRange_1278E8(int v)
{
    return v != 0 && v < 10;
}

/**
 * Updates the player's pad vibration once a frame (cRider virtual slot 19).
 *
 * Only with a Pad. Vibration stays on when the option interface (cBE_getInterface kind 1, cBENewPlayerInterface_getUseDualShock) allows it for
 * the rider's AI index and the replay's first word is not 1-9; then updatePadVibration decays the levels and drives the pad.
 */
void cPlayer::vibrationUpdate()
{
    cPlayer* player = (cPlayer*)this;
    if (player->Pad != 0)
    {
        int flag = 0;
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        if (cBENewPlayerInterface_getUseDualShock(iface, player->mRider->getAIIndex()) != 0)
        {
            flag = !riderInRange_1278E8(*(int*)SSXApp->Game->Replay);
        }
        updatePadVibration(&player->mVibrationBump, player->Pad, flag);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cPlayer_getControlCmd);
#ifdef SKIP_ASM
int cRider_getControlState(void*);
extern "C" float cInput_getValue(void* pad, int axis);
extern "C" void cReplayCache_updateControlCmd(void* cache, void* input);

// One frame of pad input packed for the rider's current state (cRider_getControlState); each state has its own layout.
// Buttons are 1 bit, analog values are 0..31 in 6 bits.
/**
 * Fills this frame's control command from the player's pad, packed for the rider's current control state, and records
 * it for replays.
 *
 * Zeroes the 8-byte command, then switches on the control state of the rider at +0x18 (cRider_getControlState, On Tour
 * getControlState). Each state packs its own layout (the low 12 bits stay zero): buttons (cInput_getState on the pad at
 * +0xDF0, 1 bit each) and analog values (cInput_getValue * 31, 6 bits each). CS_NaturalAir and CS_Spin also pack
 * cPlayer_getGrabHeld's value and CS_Railslide and CS_UberGrind cPlayer_getUberGrindHeld's (8 bits each); CS_Spin adds
 * a three-way value from axis 3 (-1 below -0.5, 1 above 0.5, else 0); CS_Handplant packs two buttons and an analog
 * value, CS_Wipeout two buttons, CS_Gate and CS_FinishLine one analog value and CS_Void nothing. When a replay recorder
 * is attached (+0xDF8) the command is passed to cReplayCache_updateControlCmd.
 *
 * @param cmd  receives the packed command (On Tour tControlCmd)
 */
// Reads the pad into the input record for the rider's state, and records it for replays.
void cPlayer::getControlCmd(tControlCmd* cmd)
{
    MEM_fill(cmd, 0, 8);
    switch (mRider->getControlState()) {
    case CS_Cruise: {
        float k = 31.0f;
        cmd->CruiseCmd.Reset = cInput_getState(Input, 46);
        cmd->CruiseCmd.Handplant = cInput_getState(Input, 25);
        cmd->CruiseCmd.JumpPressed = cInput_getState(Input, 17);
        cmd->CruiseCmd.JumpHeld = cInput_getState(Input, 18);
        cmd->CruiseCmd.BoostPressed = cInput_getState(Input, 19);
        cmd->CruiseCmd.BoostHeld = cInput_getState(Input, 20);
        cmd->CruiseCmd.AttackLeft = cInput_getState(Input, 5);
        cmd->CruiseCmd.AttackRight = cInput_getState(Input, 6);
        cmd->CruiseCmd.CruiseTurn = (int)(cInput_getValue(Input, 0) * k);
        cmd->CruiseCmd.CruiseCrouch = (int)(cInput_getValue(Input, 1) * k);
        cmd->CruiseCmd.CruiseBrake = (int)(cInput_getValue(Input, 2) * k);
        cmd->CruiseCmd.BoardPress = (int)(cInput_getValue(Input, 3) * k);
        break;
    }
    case CS_Handplant:
        cmd->HandplantCmd.Reset = cInput_getState(Input, 46);
        cmd->HandplantCmd.Handplant = cInput_getState(Input, 25);
        cmd->HandplantCmd.HandplantBalance = (int)(cInput_getValue(Input, 0x18) * 31.0f);
        break;
    case CS_BoardPress: {
        float k = 31.0f;
        cmd->BoardPressCmd.Reset = cInput_getState(Input, 46);
        cmd->BoardPressCmd.BoostPressed = cInput_getState(Input, 19);
        cmd->BoardPressCmd.BoostHeld = cInput_getState(Input, 20);
        cmd->BoardPressCmd.JumpPressed = cInput_getState(Input, 17);
        cmd->BoardPressCmd.JumpHeld = cInput_getState(Input, 18);
        cmd->BoardPressCmd.OllieHeld = cInput_getState(Input, 22);
        cmd->BoardPressCmd.RailBalance = (int)(cInput_getValue(Input, 23) * k);
        cmd->BoardPressCmd.CruiseTurn = (int)(cInput_getValue(Input, 0) * k);
        cmd->BoardPressCmd.BoardPress = (int)(cInput_getValue(Input, 3) * k);
        cmd->BoardPressCmd.BoardPivot = (int)(cInput_getValue(Input, 4) * k);
        break;
    }
    case CS_Gate:
        cmd->GateCmd.Anticipate = (int)(cInput_getValue(Input, 26) * 31.0f);
        break;
    case CS_Railslide: {
        float k = 31.0f;
        cmd->RailslideCmd.Reset = cInput_getState(Input, 46);
        cmd->RailslideCmd.JumpPressed = cInput_getState(Input, 17);
        cmd->RailslideCmd.JumpHeld = cInput_getState(Input, 18);
        cmd->RailslideCmd.BoostPressed = cInput_getState(Input, 19);
        cmd->RailslideCmd.BoostHeld = cInput_getState(Input, 20);
        cmd->RailslideCmd.UberGrindID = getUberGrindHeld();
        cmd->RailslideCmd.RailBalance = (int)(cInput_getValue(Input, 23) * k);
        cmd->RailslideCmd.RailSpin = (int)(cInput_getValue(Input, 13) * k);
        cmd->RailslideCmd.BoardPress = (int)(cInput_getValue(Input, 3) * k);
        break;
    }
    case CS_UberGrind:
        cmd->UberGrindCmd.Reset = cInput_getState(Input, 46);
        cmd->UberGrindCmd.BoostPressed = cInput_getState(Input, 19);
        cmd->UberGrindCmd.BoostHeld = cInput_getState(Input, 20);
        cmd->UberGrindCmd.UberGrindID = getUberGrindHeld();
        cmd->UberGrindCmd.RailBalance = (int)(cInput_getValue(Input, 23) * 31.0f);
        break;
    case CS_Prewind: {
        float k = 31.0f;
        cmd->PrewindCmd.Reset = cInput_getState(Input, 46);
        cmd->PrewindCmd.JumpHeld = cInput_getState(Input, 18);
        cmd->PrewindCmd.BoostHeld = cInput_getState(Input, 20);
        cmd->PrewindCmd.PrewindSpin = (int)(cInput_getValue(Input, 8) * k);
        cmd->PrewindCmd.PrewindFlip = (int)(cInput_getValue(Input, 9) * k);
        cmd->PrewindCmd.PrewindTurn = (int)(cInput_getValue(Input, 7) * k);
        cmd->PrewindCmd.RailBalance = (int)(cInput_getValue(Input, 23) * k);
        break;
    }
    case CS_Bump: {
        float k = 31.0f;
        cmd->BumpCmd.Reset = cInput_getState(Input, 46);
        cmd->BumpCmd.BoostPressed = cInput_getState(Input, 19);
        cmd->BumpCmd.BoostHeld = cInput_getState(Input, 20);
        cmd->BumpCmd.CruiseTurn = (int)(cInput_getValue(Input, 0) * k);
        cmd->BumpCmd.RailBalance = (int)(cInput_getValue(Input, 23) * k);
        break;
    }
    case CS_NaturalAir: {
        float k = 31.0f;
        cmd->NaturalAirCmd.Reset = cInput_getState(Input, 46);
        cmd->NaturalAirCmd.Handplant = cInput_getState(Input, 25);
        cmd->NaturalAirCmd.AttackLeft = cInput_getState(Input, 5);
        cmd->NaturalAirCmd.AttackRight = cInput_getState(Input, 6);
        cmd->NaturalAirCmd.GrabID = getGrabHeld();
        cmd->NaturalAirCmd.CruiseTurn = (int)(cInput_getValue(Input, 0) * k);
        cmd->NaturalAirCmd.CruiseCrouch = (int)(cInput_getValue(Input, 1) * k);
        break;
    }
    case CS_Spin: {
        float k = 31.0f;
        cmd->SpinCmd.Reset = cInput_getState(Input, 46);
        cmd->SpinCmd.Handplant = cInput_getState(Input, 25);
        cmd->SpinCmd.Tweak = cInput_getState(Input, 21);
        cmd->SpinCmd.LateSpin = cInput_getState(Input, 0xC);
        cmd->SpinCmd.GrabID = getGrabHeld();
        cmd->SpinCmd.SpinAdjust = (int)(cInput_getValue(Input, 0xE) * k);
        cmd->SpinCmd.FlipAdjust = (int)(cInput_getValue(Input, 0xF) * k);
        cmd->SpinCmd.AirAdjRotLR = (int)(cInput_getValue(Input, 11) * k);
        cmd->SpinCmd.AirAdjRotFB = (int)(cInput_getValue(Input, 10) * k);
        float axisValue = cInput_getValue(Input, 3);
        int axisDir;
        if (-0.5f <= axisValue) {
            axisDir = 0.5f < axisValue;
        } else {
            axisDir = -1;
        }
        cmd->SpinCmd.BoardPress = axisDir;
        break;
    }
    case CS_Wipeout:
        cmd->WipeoutCmd.Reset = cInput_getState(Input, 46);
        cmd->WipeoutCmd.Recover = cInput_getState(Input, 0x10);
        break;
    case CS_FinishLine:
        cmd->FinishLineCmd.CruiseTurn = (int)(cInput_getValue(Input, 0) * 31.0f);
        break;
    case CS_Void:
        break;
    }
    if (ReplayCache) {
        cReplayCache_updateControlCmd(ReplayCache, cmd);
    }
}
#endif

extern "C" void* cRiderHandler_writeToReplayFrame(void* self);

//100%
INCLUDE_ASM("ai/rider", cPlayer_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes the player controller's state into a replay frame: only its cRiderHandler part.
 *
 * Calls cRiderHandler_writeToReplayFrame on the same object; the frame pointer (the second argument, which this
 * declaration leaves out) passes through untouched. cPlayer records nothing of its own.
 *
 * @return no meaningful value (cRiderHandler_writeToReplayFrame returns nothing)
 */
void* cPlayer::writeToReplayFrame()
{
    return cRiderHandler_writeToReplayFrame(this);
}
#endif

extern "C" void* cRiderHandler_readFromReplayFrame(void* self);

//100%
INCLUDE_ASM("ai/rider", cPlayer_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Restores the player controller's state from a replay frame: only its cRiderHandler part.
 *
 * Calls cRiderHandler_readFromReplayFrame on the same object; the frame pointer (the second argument, which this
 * declaration leaves out) passes through untouched. cPlayer reads nothing of its own.
 *
 * @return no meaningful value (cRiderHandler_readFromReplayFrame returns nothing)
 */
void* cPlayer::readFromReplayFrame()
{
    return cRiderHandler_readFromReplayFrame(this);
}
#endif

