#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "animation/mdfarchive.h"
#include "be/bedbprofile.h"
#include "ealib/shape.h"
#include "sound/ssxaudio.h"
#include "bx/math3d.h"
#include "animation/animmodel.h"
#include "fe/fereal.h"
#include "fe/feridermanager.h"
#include "fe/ferider.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"
#include "render/graphicsman.h"
#include "ai/rideranim.h"
#include "visualfx/boltonfx.h"
#include "visualfx/attachedemitterfx.h"

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_initBoneMap);
#ifdef SKIP_ASM
extern "C" int cAnimModel_getBoneSkelIndex(void* self, int idx, const char* name);
extern char str_head[];
extern char str_hips[];
extern char str_handleft[];
extern char str_forearmleft[];
extern char str_thighleft[];
extern char str_shinleft[];
extern char str_footleft[];
extern char str_handright[];
extern char str_forearmright[];
extern char str_thighright[];
extern char str_shinright[];
extern char str_footright[];
extern char str_board_rootg[];
extern char str_board_childg[];

/**
 * Looks up the front-end rider model's bone indices by name: head, hips, the limbs, and the board root and
 * child.
 */
void cFERider::initBoneMap()
{
    mBoneMap[0] = cAnimModel_getBoneSkelIndex(mpAnimModel, 0, str_head);
    mBoneMap[1] = cAnimModel_getBoneSkelIndex(mpAnimModel, 0, str_hips);
    mBoneMap[2] = cAnimModel_getBoneSkelIndex(mpAnimModel, 0, str_handleft);
    mBoneMap[3] = cAnimModel_getBoneSkelIndex(mpAnimModel, 0, str_forearmleft);
    mBoneMap[4] = cAnimModel_getBoneSkelIndex(mpAnimModel, 0, str_thighleft);
    mBoneMap[5] = cAnimModel_getBoneSkelIndex(mpAnimModel, 0, str_shinleft);
    mBoneMap[6] = cAnimModel_getBoneSkelIndex(mpAnimModel, 0, str_footleft);
    mBoneMap[7] = cAnimModel_getBoneSkelIndex(mpAnimModel, 0, str_handright);
    mBoneMap[8] = cAnimModel_getBoneSkelIndex(mpAnimModel, 0, str_forearmright);
    mBoneMap[9] = cAnimModel_getBoneSkelIndex(mpAnimModel, 0, str_thighright);
    mBoneMap[10] = cAnimModel_getBoneSkelIndex(mpAnimModel, 0, str_shinright);
    mBoneMap[11] = cAnimModel_getBoneSkelIndex(mpAnimModel, 0, str_footright);
    mBoneMap[12] = cAnimModel_getBoneSkelIndex(mpAnimModel, 1, str_board_rootg);
    mBoneMap[13] = cAnimModel_getBoneSkelIndex(mpAnimModel, 1, str_board_childg);
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_queueLoadExtraTextures);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" void* func_0014D998(void* db, int g);
int func_0014D988(void* db, int g);
extern "C" int func_0014B988(void* iface, int a1, int a2, void* ent, char* buf);
extern "C" int func_0019DA10(void* mgr, int bank, char* buf, int a3, int a4, int a5);
extern "C" int func_0019CCE8(void* self, int a1, int a2);
extern "C" void func_0019CD38(void* self, int bit, int a2, int val);
extern void* D_004A1968;

struct sBoneEntE7F0 {
    char f0;
    char f1;
    char f2;
    signed char type;       // 0x3
    short slot;             // 0x4
    char pad_0x6[0x38 - 0x6];
};

static inline tBEProfileBolt* getItemK19E7F0(cBECharProfileDB* list, int i)
{
    if (i >= 0)
        return &list->mBolts[i];
    return 0;
}

/**
 * Queues the loading of the front-end rider's extra textures.
 *
 * Clears +0xCB8, then for each of the character's part entries (func_0014D998 / func_0014D988 on the item database) of
 * type 6 whose item has no texture file queued yet (func_0019CCE8 on D_004A1968 below 0), builds the file name
 * (func_0014B988) and, when there is one, queues it in the front end's file manager (func_0019DA10 at RealFE + 0x1A70,
 * kind 11) and records the file slot (func_0019CD38).
 */
void cFERider::queueLoadExtraTextures()
{
    char* mgr = SSXApp->RealFE->mBoltFileManager;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 9);
    void* db = func_0014BDB8_noarg();
    cBECharProfileDB* list = (cBECharProfileDB*)func_0014AD28(iface, mProfileIndex, mCharID);
    sBoneEntE7F0* partEntry = (sBoneEntE7F0*)func_0014D998(db, mCharID);
    int n = func_0014D988(db, mCharID);
    mDoneLoadingTextures = 0;
    int i;
    for (i = 0; i < n; i++, partEntry++) {
        if (partEntry->type == 6) {
            tBEProfileBolt* it = getItemK19E7F0(list, list->mpBoltsMap[partEntry->slot]);
            if (func_0019CCE8(D_004A1968, mProfileIndex, it->id) < 0) {
                char buf[0x100];
                if (func_0014B988(iface, mProfileIndex, mCharID, partEntry, buf)) {
                    int fileSlot = func_0019DA10(mgr, mCharID, buf, 11, 1, 1);
                    func_0019CD38(D_004A1968, mProfileIndex, it->id, fileSlot);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_init);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" void cFERider_purge(cFERider* self);
extern "C" void cFERider_queueLoadExtraTextures(cFERider* self);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002F1890(void* mem, void* owner);
extern "C" void* cBoltOnFXInterface_cBoltOnFXInterface1(void* mem, void* owner, void* a2);
extern "C" void* cAnimModel_cAnimModel(void* mem);
extern "C" void cAnimModel_init(void* pool, int a1, int a2);
extern "C" void func_002F19F0(void* a, void* b);
// PORT: these are defined later in the unit with struct-typed params; bind void* views by asm label
extern "C" void cFERider_loadModels_E980(int* self, void* list, void* prev, int force, int a4) __asm__("cFERider_loadModels");
extern "C" void cFERider_loadTextures_E980(int* self, void* list, int a2, int a3) __asm__("cFERider_loadTextures");
extern "C" void cFERider_update_E980(void* self) __asm__("cFERider_update");
extern char str_AttachedEmitterFX_b[];
extern char str_cBoltOnFXInterface_b[];
extern char str_AnimModel[];

// PORT: callers declare cFERider_init as returning void; the body returns 1
extern "C" int cFERider_init_impl(int* self, int profile, int character, int slot) __asm__("cFERider_init");
/**
 * Sets up a front-end rider for a character: emitters, bolt-on FX, the animated model, then the character's
 * item list.
 *
 * Does nothing when the same character and @p slot are already loaded; otherwise releases the previous rider.
 * Also
 * sets a one-bit mask for the character (+0xCD8).
 *
 * @param profile  profile index (On Tour profileIndex)
 * @param character  character ID (On Tour charID)
 * @param slot  nonzero to load everything (On Tour loadAll), which adds cFERider_queueLoadExtraTextures()
 * @return 1
 */
extern "C" int cFERider_init_impl(int* self, int profile, int character, int slot)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 9);
    if (self[0] == character && self[4] == slot) {
        return 1;
    }
    if (self[0] >= 0) {
        ((cFERider*)self)->purge();
    }
    int dirty = (self[3268 / 4] != 0) | (self[3272 / 4] != 0);
    self[4] = slot;
    self[1] = profile;
    self[0] = character;
    self[3112 / 4] = 0;
    self[20 / 4] = 0;
    self[3272 / 4] = 0;
    self[3252 / 4] = 0;
    self[3256 / 4] = 0;
    self[3260 / 4] = 0;
    self[3264 / 4] = 0;
    self[3108 / 4] = -1;
    self[3268 / 4] = dirty;
    *(void**)&self[3184 / 4] = func_002F1890(cMemMan_alloc(168, str_AttachedEmitterFX_b, 0, 0), self);
    *(void**)&self[3188 / 4] = cBoltOnFXInterface_cBoltOnFXInterface1(cMemMan_alloc(200, str_cBoltOnFXInterface_b, 0, 0), self, *(void**)&self[3184 / 4]);
    *(void**)&self[2] = cAnimModel_cAnimModel(cMemMan_alloc(352, str_AnimModel, 0, 0));
    cAnimModel_init(*(void**)&self[2], 100, 15);
    func_002F19F0(*(void**)&self[3184 / 4], *(void**)&self[2]);
    void* list = func_0014AD28(iface, profile, character);
    cFERider_loadModels_E980(self, list, 0, self[4], 1);
    if (self[4] != 0) {
        ((cFERider*)self)->queueLoadExtraTextures();
    }
    cFERider_loadTextures_E980(self, list, 0, 1);
    cFERider_update_E980(self);
    switch (self[0]) {
    case 0: self[3288 / 4] = 1; break;
    case 1: self[3288 / 4] = 4; break;
    case 2: self[3288 / 4] = 0x20; break;
    case 3: self[3288 / 4] = 2; break;
    case 4: self[3288 / 4] = 8; break;
    case 5: self[3288 / 4] = 0x100; break;
    case 6: self[3288 / 4] = 0x200; break;
    case 7: self[3288 / 4] = 0x80; break;
    case 8: self[3288 / 4] = 0x10; break;
    case 9: self[3288 / 4] = 0x40; break;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_purge);
#ifdef SKIP_ASM
// The virtual destructor (slot 1) of the three owned objects, called with flags 3 (destroy and free).
typedef sVEntry<void(void*, int)> tDtorSlot;
extern "C" void cAnimModel__cAnimModel(void* p, int flags);
extern "C" void cFERider_cancelAllFiles(cFERider* self);

/**
 * Releases everything the front-end rider holds and marks it empty.
 *
 * Destroys the anim model (mpAnimModel, cAnimModel__cAnimModel with 3), the animation (mpRiderAnim), the bolt-on FX
 * interface (mpBoltOnFX) and the attached-emitter FX (mpAttachedEmitterFX) through their virtual destructors (slot 1,
 * flags 3), clears those pointers, cancels the rider's files (cFERider_cancelAllFiles), sets mCharID, mProfileIndex and
 * mSafeRenderCount to -1 and zeroes mVisible and mBoltPoints.
 */
void cFERider::purge()
{
    void* animModel = mpAnimModel;
    if (animModel != 0) {
        cAnimModel__cAnimModel(animModel, 3);
    }
    cRiderAnim* riderAnim = mpRiderAnim;
    if (riderAnim != 0) {
        tDtorSlot* vt = (tDtorSlot*)riderAnim->mVtbl;
        vt[1].fn(vthis(riderAnim, vt[1]), 3);
    }
    cBoltOnFXInterface* boltOnFX = (cBoltOnFXInterface*)mpBoltOnFX;
    if (boltOnFX != 0) {
        tDtorSlot* vt = (tDtorSlot*)boltOnFX->mVtbl;
        vt[1].fn(vthis(boltOnFX, vt[1]), 3);
    }
    cAttachedEmitterFX* emitterFX = (cAttachedEmitterFX*)mpAttachedEmitterFX;
    if (emitterFX != 0) {
        tDtorSlot* vt = (tDtorSlot*)emitterFX->mVtbl;
        vt[1].fn(vthis(emitterFX, vt[1]), 3);
    }
    mpAnimModel = 0;
    mpRiderAnim = 0;
    mpAttachedEmitterFX = 0;
    mpBoltOnFX = 0;
    cancelAllFiles();
    mCharID = -1;
    mProfileIndex = -1;
    mVisible = 0;
    mBoltPoints = 0;
    mSafeRenderCount = -1;
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_cancelAllFiles);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" int func_0019CBE0(void* self, int a1, int a2);
extern "C" int func_0019CCE8(void* self, int a1, int a2);
extern "C" void func_0019CC30(void* self, int bit, int a2, int val);
extern "C" void func_0019CD38(void* self, int bit, int a2, int val);
extern "C" void func_0019DC20(void* self, int bank, int i);
extern void* D_004A1968;


/**
 * Cancels every file the front-end rider has queued for its items.
 *
 * Needs a profile and character (+0x4, +0x0) set. For each item of the character's list (func_0014AD28), releases a
 * queued texture file (func_0019CCE8 / func_0019DC20, slot reset with func_0019CD38) and a queued model-part file
 * (func_0019CBE0 / func_0019DC20, reset with func_0019CC30).
 *
 */
void cFERider::cancelAllFiles()
{
    int* self = (int*)this;
    if (self[1] < 0)
        return;
    if (self[0] < 0)
        return;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 9);
    char* mgr = SSXApp->RealFE->mBoltFileManager;
    cBECharProfileDB* list = (cBECharProfileDB*)func_0014AD28(iface, self[1], self[0]);
    int n = list->mNumBolts;
    int i;
    for (i = 0; i < n; i++) {
        int fileSlot = func_0019CCE8(D_004A1968, self[1], list->mBolts[i].id);
        if (fileSlot >= 0) {
            func_0019DC20(mgr, self[0], fileSlot);
            func_0019CD38(D_004A1968, self[1], list->mBolts[i].id, -1);
        }
        fileSlot = func_0019CBE0(D_004A1968, self[1], list->mBolts[i].id);
        if (fileSlot >= 0) {
            func_0019DC20(mgr, self[0], fileSlot);
            func_0019CC30(D_004A1968, self[1], list->mBolts[i].id, -1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_compileModel);
#ifdef SKIP_ASM
void cGraphicsMan_InitTextureFiles(void* self, int a1, int a2);
extern "C" void cGraphicsMan_SetTextureRefs(void* self, int count, void* src);
extern "C" void cAnimModel_compile(void* self, int a1);
// PORT: cMemMan_alloc bound as a placement operator new (gcc treats operator new as malloc-like)
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void* cRiderAnimBase_cRiderAnimBase(void* self);
extern "C" void cRiderAnim_linkToFERider(void* anim, void* rider);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cRiderAnimBase_setupAnimation(void* self, int a1);
extern "C" void cFERider_initBoneMap(cFERider* self);
extern char str_RiderAnim[];
extern void* cRiderAnim_vtbl[];

// tPose with the original's empty constructor (the pose pair below is an array of it; retail still runs the empty
// array-constructor loop) and an assignment from a plain tPose.
struct tPoseCtor : public tPose {
    tPoseCtor() {}
    tPoseCtor& operator=(const tPose& p)
    {
        tPose::operator=(p);
        return *this;
    }
};
extern tPose tPose_Identity;
extern "C" void cAnimModel_transform(void* model, void* pose);


/**
 * Compiles the front-end rider's model and starts its animation.
 *
 * Sets mCompiledModel, prepares the graphics manager's bone setup (cGraphicsMan_InitTextureFiles,
 * cGraphicsMan_SetTextureRefs with the model's bones), compiles the anim model (cAnimModel_compile with 1), creates its
 * animation (a cRiderAnim: cRiderAnimBase with cRiderAnim_vtbl, Rider and FERider cleared, tag "RiderAnim"), binds it
 * to the rider (cRiderAnim_linkToFERider), plays animation 434, applies the identity pose (cAnimModel_transform with
 * tPose_Identity) and builds the bone map (cFERider_initBoneMap).
 */
void cFERider::compileModel()
{
    mCompiledModel = 1;
    cGraphicsMan_InitTextureFiles(GrMan, 0, 0);
    cGraphicsMan_SetTextureRefs(GrMan, mNumTextureRefs, ((char*)&mTextureRefs[0]));
    cAnimModel_compile(mpAnimModel, 1);
    cRiderAnim* anim = new (str_RiderAnim, 0, 0) cRiderAnim;
    cRiderAnimBase_cRiderAnimBase(anim);
    anim->mVtbl = cRiderAnim_vtbl;
    anim->Rider = 0;
    mpRiderAnim = anim;
    anim->FERider = 0;
    cRiderAnim_linkToFERider(anim, (this));
    cRiderAnimBase_play(mpRiderAnim, 434, 0, -1.0f);
    cRiderAnimBase_setupAnimation(mpRiderAnim, 0);
    tPoseCtor identityPose[2];
    identityPose[1] = tPose_Identity;
    identityPose[0] = tPose_Identity;
    cAnimModel_transform(mpAnimModel, identityPose);
    initBoneMap();
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_render);
#ifdef SKIP_ASM
extern char cIrradianceDataBase_TheDataBase[];
extern char cIrradiance_kv4White[];
extern char str_moby[];
extern char str_kaori[];
extern char str_allegra[];
extern char str_mac[];
extern char str_zoe[];
extern char str_griff[];
extern char str_elise[];
extern char str_nate[];
extern char str_psymon[];
extern char str_viggo[];
extern char str_fe_map[];


struct sMtx_EE88 {
    float m[40];
} __attribute__((aligned(16)));

extern "C" void cIrradiance_Clear(sMtx_EE88* m);
extern "C" void* cIrradianceDataBase_Get1(char* tbl, char* name);
extern "C" void func_00389590(sMtx_EE88* m, void* a, char* b, float s);
extern "C" void func_00389CB8(sMtx_EE88* m, t4Vector* v, float s);
extern "C" void cAnimModel_render(void* model, int a, int b, int c);
extern "C" void func_002F2088(void* p);

struct sModel_EE88 {
    char pad0[0x2C];
    t4Vector* bones;  // 0x2C
};


struct sVE_EE88 {
    short delta;
    short index;
    void (*fn)(void*, sMtx_EE88*, int);
};

/**
 * Renders the front-end rider at its character's spot on the front-end map.
 *
 * Skipped while an init is deferred, without a model or before a pose was built (+0x14). The world matrix comes from
 * the character's locator in cIrradianceDataBase_TheDataBase ("moby" .. "viggo", "fe_map" for others; characters 7 and 8, and 0 when +0xCD0
 * is set, use "elise", so the "nate" and "psymon" cases are never reached), moved by the model's hips bone; it is set
 * on the graphics manager (vtable entry 69). The draw state's layer bits (5-9) take the rider's draw layer when it is
 * not negative while the model (cAnimModel_render) and the attached-emitter FX (func_002F2088) render, then are put
 * back.
 *
 */
void cFERider::render()
{
    cFERider* self = this;
    sMtx_EE88 worldMatrix;
    t4Vector pos;
    if (self->mSafeRenderCount >= 0)
        return;
    if (((sModel_EE88*)self->mpAnimModel) == 0)
        return;
    if (self->mRiderIsPosed == 0)
        return;
    cIrradiance_Clear(&worldMatrix);
    int type = self->mCharID;
    if (self->mUberIrradiance != 0) {
        switch (type) {
        case 0:
        case 7:
        case 8:
            type = 6;
            break;
        }
    }
    switch (type) {
    case 7:
    case 8:
        type = 6;
        break;
    }
    switch (type) {
    case 0:
        func_00389590(&worldMatrix, cIrradianceDataBase_Get1(cIrradianceDataBase_TheDataBase, str_moby), cIrradiance_kv4White, 1.0f);
        break;
    case 1:
        func_00389590(&worldMatrix, cIrradianceDataBase_Get1(cIrradianceDataBase_TheDataBase, str_kaori), cIrradiance_kv4White, 1.0f);
        break;
    case 2:
        func_00389590(&worldMatrix, cIrradianceDataBase_Get1(cIrradianceDataBase_TheDataBase, str_allegra), cIrradiance_kv4White, 1.0f);
        break;
    case 3:
        func_00389590(&worldMatrix, cIrradianceDataBase_Get1(cIrradianceDataBase_TheDataBase, str_mac), cIrradiance_kv4White, 1.0f);
        break;
    case 4:
        func_00389590(&worldMatrix, cIrradianceDataBase_Get1(cIrradianceDataBase_TheDataBase, str_zoe), cIrradiance_kv4White, 1.0f);
        break;
    case 5:
        func_00389590(&worldMatrix, cIrradianceDataBase_Get1(cIrradianceDataBase_TheDataBase, str_griff), cIrradiance_kv4White, 1.0f);
        break;
    case 6:
        func_00389590(&worldMatrix, cIrradianceDataBase_Get1(cIrradianceDataBase_TheDataBase, str_elise), cIrradiance_kv4White, 1.0f);
        break;
    case 7:
        func_00389590(&worldMatrix, cIrradianceDataBase_Get1(cIrradianceDataBase_TheDataBase, str_nate), cIrradiance_kv4White, 1.0f);
        break;
    case 8:
        func_00389590(&worldMatrix, cIrradianceDataBase_Get1(cIrradianceDataBase_TheDataBase, str_psymon), cIrradiance_kv4White, 1.0f);
        break;
    case 9:
        func_00389590(&worldMatrix, cIrradianceDataBase_Get1(cIrradianceDataBase_TheDataBase, str_viggo), cIrradiance_kv4White, 1.0f);
        break;
    default:
        func_00389590(&worldMatrix, cIrradianceDataBase_Get1(cIrradianceDataBase_TheDataBase, str_fe_map), cIrradiance_kv4White, 1.0f);
        break;
    }
    int idx = cAnimModel_getBoneSkelIndex(((sModel_EE88*)self->mpAnimModel), 0, str_hips);
    pos = ((sModel_EE88*)self->mpAnimModel)->bones[idx * 2];
    func_00389CB8(&worldMatrix, &pos, 1.0f);
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        ps2GraphicsMan->SetIrradiance(*(const tIrradianceInfo*)&worldMatrix, 1);
    }
    int* fl = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
    unsigned int old = (unsigned int)(*fl & 0x3E0) >> 5;
    if (self->mDrawLayer >= 0)
        *fl = (*fl & ~0x3E0) | ((self->mDrawLayer << 5) & 0x3E0);
    cAnimModel_render(((sModel_EE88*)self->mpAnimModel), 0, 0, 0);
    func_002F2088(self->mpAttachedEmitterFX);
    fl = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
    *fl = (*fl & ~0x3E0) | (old << 5);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feridermanager", cFERider_update);
#ifdef SKIP_ASM
extern "C" void cFERider_init(void* self, int a1, int a2, int a3);
extern "C" int cFERider_isDoneLoadingModels(int* self);
extern "C" int cFERider_isDoneLoadingTextures(cFERider* self);
extern "C" void cFERider_updateAnimModelParts(cFERider* self);
extern "C" void cFERider_updateModelPose(cFERider* self);
extern "C" void cRiderAnimBase_updateAnimation(void* anim, int a1, float blend);
extern "C" void cRiderAnimBase_expireAnimation(void* anim);
extern "C" void func_002F1A08(void* p);
extern "C" void func_002ECF78(void* p);



/**
 * Updates the front-end rider each frame: finishes deferred loads, compiles the model and advances its animation.
 *
 * While an init is deferred (its frame word not negative) waits until the graphics manager's vtable entry 114 passes it
 * and then runs cFERider_init with the stored arguments. Otherwise, for a loaded character (id below 10): marks the
 * model parts loaded once cFERider_isDoneLoadingModels succeeds (+0xCBC), the textures once cFERider_isDoneLoadingTextures does (+0xCB8) and then
 * compiles the model (cFERider_compileModel) if needed; once all three are done applies the part variants once (cFERider_updateAnimModelParts,
 * +0xCC0) and folds the pending show request (+0xCC4) into the shown flag (+0xCC8). While shown, advances the animation
 * (cRiderAnimBase_updateAnimation, expireAnimation), rebuilds the pose (cFERider_updateModelPose) and updates both FX objects.
 *
 */
void cFERider::update()
{
    cFERider* self = this;
    if (self->mSafeRenderCount >= 0) {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        if (self->mSafeRenderCount < ps2GraphicsMan->getCurrentFrameCount()) {
            self->mSafeRenderCount = -1;
            cFERider_init(self, self->mProfileIndex, self->mNewCharID, self->mNewLoadAll);
        }
    } else {
        if (((unsigned int)self->mCharID) >= 10) {
            return;
        }
        if (self->mDoneLoadingModels == 0 && cFERider_isDoneLoadingModels((int*)self)) {
            self->mDoneLoadingModels = 1;
        } else if (self->mDoneLoadingModels != 0) {
            if (self->mDoneLoadingTextures == 0 && self->isDoneLoadingTextures()) {
                self->mDoneLoadingTextures = 1;
                if (self->mCompiledModel == 0) {
                    self->compileModel();
                }
            } else if (self->mDoneLoadingModels != 0 && self->mDoneLoadingTextures != 0 && self->mCompiledModel != 0 && self->mDoneUpdatingModelParts == 0) {
                self->updateAnimModelParts();
                self->mDoneUpdatingModelParts = 1;
                int* pv = &self->mVisible;
                *pv = self->mSetVisible != 0 || *pv != 0;
                self->mSetVisible = 0;
            }
        }
    }
    if (self->mVisible != 0) {
        if (((void*)self->mpRiderAnim) != 0) {
            cRiderAnimBase_updateAnimation(((void*)self->mpRiderAnim), 0, 1.0f);
            cRiderAnimBase_expireAnimation(((void*)self->mpRiderAnim));
        }
        self->updateModelPose();
        func_002F1A08(self->mpAttachedEmitterFX);
        func_002ECF78(self->mpBoltOnFX);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_updateModelPose);
#ifdef SKIP_ASM
extern t4Vector t4vect_Origin;
extern t4Vector tquat_Identity; // a tQuaternion; cAnimModel::localRot holds them as t4Vector
extern "C" void cAnimModel_transform(void* model, void* pose);
extern "C" void cAnimModel_buildSkelMatrices(void* model);

/**
 * Rebuilds the front-end rider's skeleton from its rider and board transforms and marks the pose built
 * (mRiderIsPosed).
 *
 * With mAnimateBoard set uses the rider transform (mPose) for both; otherwise mPose and mBoardPose, first resetting the
 * two board bones (mBoneMap[12], mBoneMap[13]) to the origin and tquat_Identity. Then cAnimModel_transform and
 * cAnimModel_buildSkelMatrices.
 */
void cFERider::updateModelPose()
{
    tPose poses[2];
    if (mAnimateBoard != 0) {
        poses[0] = mPose;
        poses[1] = mPose;
        cAnimModel_transform(mpAnimModel, poses);
        cAnimModel_buildSkelMatrices(mpAnimModel);
    } else {
        poses[0] = mPose;
        poses[1] = mBoardPose;
        mpAnimModel->localPos[mBoneMap[12]] = t4vect_Origin;
        mpAnimModel->localRot[mBoneMap[12]] = tquat_Identity;
        mpAnimModel->localPos[mBoneMap[13]] = t4vect_Origin;
        mpAnimModel->localRot[mBoneMap[13]] = tquat_Identity;
        cAnimModel_transform(mpAnimModel, poses);
        cAnimModel_buildSkelMatrices(mpAnimModel);
    }
    mRiderIsPosed = 1;
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_setPose);
#ifdef SKIP_ASM
struct sQuadRM { int x[4]; } __attribute__((aligned(16)));
extern "C" void cFERider_updateModelPose(cFERider* self);

/**
 * Sets the front-end rider's transform (position +0xC30, rotation +0xC40) and rebuilds the pose (cFERider_updateModelPose).
 *
 * @param src  the new transform (two quadwords)
 */
extern "C" void cFERider_setPose(cFERider* self, sQuadRM* src)
{
    *(sQuadRM*)&self->mPose = src[0];
    *(sQuadRM*)&self->mPose.Q = src[1];
    self->updateModelPose();
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_translate);
#ifdef SKIP_ASM
extern "C" void cFERider_updateModelPose(cFERider* self);


// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline t4Vector Rot_F418(const t4Vector& q, const t4Vector& v)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void AddTo_F418(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "+m"(dst)
        : "m"(b)
        : "memory");
}

/**
 * Moves the front-end rider by a vector turned into its current rotation (+0xC40) and rebuilds the pose.
 *
 * @param delta  the move, in the rider's frame
 */
void cFERider::translate(t4Vector* delta)
{
    t4Vector* pos = (t4Vector*)&mPose;
    t4Vector* rot = ((&mPose.Q));
    AddTo_F418(*pos, Rot_F418(*rot, *delta));
    updateModelPose();
}
#endif
//100%
INCLUDE_ASM("fe/feridermanager", cFERider_rotate);
#ifdef SKIP_ASM
extern "C" void cFERider_updateModelPose(cFERider* self);
extern "C" void fSinCos(float* s, float* c, float angle);


// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline t4Vector QuatMul_F488(const t4Vector& a, const t4Vector& b)
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

/**
 * Turns the front-end rider about an axis by an angle (its rotation +0xC40 times the axis-angle quaternion) and
 * rebuilds the pose.
 *
 * @param axis   the axis (x, y, z used)
 * @param angle  the angle in radians
 */
void cFERider::rotate(t4Vector* axis, float angle)
{
    t4Vector* rot = ((&mPose.Q));
    t4Vector turn;
    float sinHalf, cosHalf;
    fSinCos(&sinHalf, &cosHalf, angle * 0.5f);
    t4Vector rotQuat;
    rotQuat.w = cosHalf;
    rotQuat.x = sinHalf * axis->x;
    rotQuat.y = sinHalf * axis->y;
    rotQuat.z = sinHalf * axis->z;
    turn = rotQuat;
    *rot = QuatMul_F488(*rot, turn);
    updateModelPose();
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_setBoardPose);
#ifdef SKIP_ASM
extern "C" void cFERider_updateModelPose(cFERider* self);

/**
 * Sets the front-end rider's board transform (position +0xC50, rotation +0xC60) and rebuilds the pose.
 *
 * @param src  the new transform (two quadwords)
 */
extern "C" void cFERider_setBoardPose(cFERider* self, sQuadRM* src)
{
    *(sQuadRM*)&self->mBoardPose = src[0];
    *(sQuadRM*)&self->mBoardPose.Q = src[1];
    self->updateModelPose();
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_translateBoard);
#ifdef SKIP_ASM
extern "C" void cFERider_updateModelPose(cFERider* self);


// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline t4Vector Rot_F578(const t4Vector& q, const t4Vector& v)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void AddTo_F578(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "+m"(dst)
        : "m"(b)
        : "memory");
}

/**
 * Moves the front-end rider's board by a vector turned into the board's rotation (+0xC60) and rebuilds the pose.
 *
 * @param delta  the move, in the board's frame
 */
void cFERider::translateBoard(t4Vector* delta)
{
    t4Vector* pos = (t4Vector*)&mBoardPose;
    t4Vector* rot = ((&mBoardPose.Q));
    AddTo_F578(*pos, Rot_F578(*rot, *delta));
    updateModelPose();
}
#endif
//100%
INCLUDE_ASM("fe/feridermanager", cFERider_rotateBoard);
#ifdef SKIP_ASM
extern "C" void cFERider_updateModelPose(cFERider* self);
extern "C" void fSinCos(float* s, float* c, float angle);


// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline t4Vector QuatMul_F5E8(const t4Vector& a, const t4Vector& b)
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

/**
 * Turns the front-end rider's board about an axis by an angle (rotation +0xC60) and rebuilds the pose.
 *
 * @param axis   the axis (x, y, z used)
 * @param angle  the angle in radians
 */
void cFERider::rotateBoard(t4Vector* axis, float angle)
{
    t4Vector* rot = ((&mBoardPose.Q));
    t4Vector turn;
    float sinHalf, cosHalf;
    fSinCos(&sinHalf, &cosHalf, angle * 0.5f);
    t4Vector rotQuat;
    rotQuat.w = cosHalf;
    rotQuat.x = sinHalf * axis->x;
    rotQuat.y = sinHalf * axis->y;
    rotQuat.z = sinHalf * axis->z;
    turn = rotQuat;
    *rot = QuatMul_F5E8(*rot, turn);
    updateModelPose();
}
#endif
//100%
INCLUDE_ASM("fe/feridermanager", cFERider_getBonePosition);
#ifdef SKIP_ASM
/**
 * Returns the address of a bone's position in the front-end rider's model.
 *
 * The bone comes from the rider's bone map (+0xC7C + 4 x index) and indexes the model's 0x20-byte bone records (+0x2C).
 *
 * @param index      the bone id
 * @return the address of the bone's position (first quadword)
 */
int cFERider::getBonePosition(int index)
{
    return ((int)mpAnimModel->bones) + (mBoneMap[index] << 5);
}
#endif
//100%
INCLUDE_ASM("fe/feridermanager", cFERider_setBonePosition);
#ifdef SKIP_ASM
// R5900 128-bit GPR quadword (single lq/sq pair); same typedef as ai/rider.cpp.
typedef int cQuad128 __attribute__((mode(TI)));


/**
 * Sets a bone's position in the front-end rider's model (the mapped bone's first quadword).
 *
 * @param slot  the bone id (through the bone map at +0xC7C)
 * @param src   the new position
 */
extern "C" void cFERider_setBonePosition(cFERider* self, int slot, cQuad128* src)
{
    cAnimModel* model = self->mpAnimModel;
    int bone = self->mBoneMap[slot];
    sAnimModelBone* bones = model->bones;
    (*(cQuad128*)&bones[bone].pos) = *src;
}
#endif
//100%
INCLUDE_ASM("fe/feridermanager", cFERider_getBoneQuaternion);
#ifdef SKIP_ASM
/**
 * Returns the address of a bone's rotation in the front-end rider's model (the mapped bone's second quadword).
 *
 * @param index      the bone id (through the bone map at +0xC7C)
 * @return the address of the bone's quaternion
 */
int cFERider::getBoneQuaternion(int index)
{
    return ((int)mpAnimModel->bones) + (mBoneMap[index] << 5) + 0x10;
}
#endif
//100%
INCLUDE_ASM("fe/feridermanager", cFERider_setBoneQuaternion);
#ifdef SKIP_ASM
typedef int cQuad128 __attribute__((mode(TI)));


/**
 * Sets a bone's rotation in the front-end rider's model (the mapped bone's second quadword).
 *
 * @param slot  the bone id (through the bone map at +0xC7C)
 * @param src   the new quaternion
 */
extern "C" void cFERider_setBoneQuaternion(cFERider* self, int slot, cQuad128* src)
{
    cAnimModel* model = self->mpAnimModel;
    int bone = self->mBoneMap[slot];
    sAnimModelBone* bones = model->bones;
    (*(cQuad128*)&bones[bone].rot) = *src;
}
#endif
//100%
INCLUDE_ASM("fe/feridermanager", cFERider_getAnimID);
#ifdef SKIP_ASM
extern "C" void cRiderAnimBase_getCurrentAnimID__FPvi(cRiderAnim*, int);

/**
 * Asks the front-end rider's animation (+0xC) for its current animation id (cRiderAnimBase_getCurrentAnimID with 2).
 *
 */
void cFERider::getAnimID()
{
    cRiderAnimBase_getCurrentAnimID__FPvi((mpRiderAnim), 2);
}
#endif
//100%
INCLUDE_ASM("fe/feridermanager", cFERider_getAnimPercent);
#ifdef SKIP_ASM
extern "C" void cRiderAnimBase_getCurrentAnimFramePercent(cRiderAnim*, int);

/**
 * Returns how far the front-end rider's current animation has played (cRiderAnimBase_getCurrentAnimFramePercent on its animation +0xC, sequencer
 * 2).
 *
 * cRiderAnimBase_getCurrentAnimFramePercent divides the playing sequence's time (+0x8) by its length (+0x10); its float result is passed through
 * (the declared return type is void).
 *
 */
void cFERider::getAnimPercent()
{
    cRiderAnimBase_getCurrentAnimFramePercent((mpRiderAnim), 2);
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_updateAnimModelParts);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" int func_0014D448(void* self, int g, int key);
extern "C" void func_0030EB60(void* model);
extern "C" void cAnimModel_setModelPartVariant(void* model, int i, int param);

struct sBoneEntF780 {
    char f0;
    char variant;
    char pad_0x2[0xE];
    char partID;
    char pad_0x11[0x38 - 0x11];
};
struct sBoneDbF780 {
    int f0;
    sBoneEntF780* entries;  // 0x4
};

static inline sBoneEntF780* getEntK19F780(sBoneDbF780* db, int i)
{
    if (i < 0)
        return 0;
    return &db->entries[i];
}

/**
 * Applies the variant of each equipped item part to the front-end rider's model and flags its bolt-on FX.
 *
 * Resets the model's parts (func_0030EB60), then for each item of the character's list with flag 0x10 looks up its part
 * entry (func_0014D448 on the item database) and, when its part index (+0x10) is not negative, sets that model part's
 * variant (cAnimModel_setModelPartVariant). Finally sets the first word of the bolt-on FX interface (+0xC74) to 1.
 */
void cFERider::updateAnimModelParts()
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 9);
    sBoneDbF780* db = (sBoneDbF780*)func_0014BDB8_noarg();
    cBECharProfileDB* list = (cBECharProfileDB*)func_0014AD28(iface, ((int*)this)[1], ((int*)this)[0]);
    int n = list->mNumBolts;
    tBEProfileBolt* it = ((&list->mBolts[0]));
    func_0030EB60(mpAnimModel);
    int i;
    for (i = 0; i < n; i++) {
        if (it[i].flags & 0x10) {
            sBoneEntF780* partEntry = getEntK19F780(db, func_0014D448(db, ((int*)this)[0], it[i].id));
            if (partEntry->partID >= 0)
                cAnimModel_setModelPartVariant(mpAnimModel, partEntry->partID, partEntry->variant);
        }
    }
    *(int*)mpBoltOnFX = 1;
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_getTextureID);
#ifdef SKIP_ASM
extern "C" int strcmp(const void* a, const void* b);



/**
 * Looks up a texture by name in the front-end rider's texture table.
 *
 * @param name  the texture name
 * @return its texture id, or -1 when the table has no such name
 */
int cFERider::getTextureID(const char* name)
{
    int i;
    for (i = 0; i < mNumTextureRefs; i++) {
        if (strcmp(name, ((&mTextureRefs[0]))[i].Name) == 0) {
            return ((&mTextureRefs[0]))[i].TextureID;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_addTextureFile);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void SHAPE_name(void* pkg, int i, char* name);
extern "C" char* strcpy(char* dst, const char* src);
extern char D_004A19A8[];
extern char str_charshp[];

struct sPkgEnt_F908 {
    int off;
    int f4;
};
struct sName2_F908 { char c[2]; };
/**
 * Registers the textures of a loaded shape package in the front-end rider's texture table.
 *
 * For each image of @p pkg builds its name (prefix D_004A19A8, SHAPE_name); a known name is reloaded in place (graphics
 * manager vtable entry 48) only when @p replace is set; a new one is created (vtable entry 46, kind "charshp") and
 * appended to the table.
 *
 * @param pkg      the loaded shape package
 * @param replace  nonzero to refresh textures already in the table
 */
void cFERider::addTextureFile(SHAPEFILE* pkg, int replace)
{
    int n = pkg->num;
    int i;
    for (i = 0; i < n; i++) {
        sPkgEnt_F908* imageEntry = &((sPkgEnt_F908*)&pkg->tbl[0].offset)[i];
        char* data = (char*)pkg + imageEntry->off;
        char name[8];
        *(sName2_F908*)name = *(sName2_F908*)D_004A19A8;
        memset(name + 2, 0, 6);
        SHAPE_name(pkg, i, name);
        int id = getTextureID(name);
        if (id >= 0) {
            if (replace) {
                cPS2GraphicsMan* ps2GraphicsMan = GrMan;
                ps2GraphicsMan->SetNewTextureData(id, (const SHAPE*)data, 1);
            }
        } else {
            strcpy(((&mTextureRefs[0]))[mNumTextureRefs].Name, name);
            cPS2GraphicsMan* ps2GraphicsMan = GrMan;
            ((&mTextureRefs[0]))[mNumTextureRefs].TextureID = ps2GraphicsMan->AddTexture((const SHAPE*)data,
                    str_charshp, 0, 1, -1);
            mNumTextureRefs++;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", func_0019FA78);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" void* func_0014D998(void* db, int g);
int func_0014D988(void* db, int g);
extern "C" int func_0019CCE8(void* self, int a1, int a2);
extern "C" void func_0019CD38(void* self, int bit, int a2, int val);
extern "C" void func_0019DC20(void* self, int bank, int i);
extern void* D_004A1968;

struct sBoneEntFA78 {
    char f0;
    char f1;
    char f2;
    signed char type;       // 0x3
    short slot;             // 0x4
    char pad_0x6[0x38 - 0x6];
};

static inline tBEProfileBolt* getItemK19FA78(cBECharProfileDB* list, int i)
{
    if (i >= 0)
        return &list->mBolts[i];
    return 0;
}

/**
 * Cancels the front-end rider's queued textures except those of extra (type 6) parts.
 *
 * Needs a profile and character set. For each of the character's part entries whose item has a texture file queued,
 * releases it (func_0019DC20, slot reset to -1) unless the part is of type 6. Sets +0xCB8 (textures ready) only when
 * nothing of type 6 is still queued.
 *
 * @param list  the character's item list, or null to fetch it (func_0014AD28)
 */
extern "C" void func_0019FA78(cFERider* self, cBECharProfileDB* list)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 9);
    void* db = func_0014BDB8_noarg();
    char* mgr = SSXApp->RealFE->mBoltFileManager;
    if (self->mProfileIndex < 0)
        return;
    if (self->mCharID < 0)
        return;
    if (list == 0)
        list = (cBECharProfileDB*)func_0014AD28(iface, self->mProfileIndex, self->mCharID);
    int flag = 1;
    int n = func_0014D988(db, self->mCharID);
    sBoneEntFA78* partEntry = (sBoneEntFA78*)func_0014D998(db, self->mCharID);
    int i;
    for (i = 0; i < n; i++, partEntry++) {
        tBEProfileBolt* it = getItemK19FA78(list, list->mpBoltsMap[partEntry->slot]);
        int fileSlot = func_0019CCE8(D_004A1968, self->mProfileIndex, it->id);
        if (fileSlot >= 0) {
            if (partEntry->type != 6) {
                func_0019DC20(mgr, self->mCharID, fileSlot);
                func_0019CD38(D_004A1968, self->mProfileIndex, it->id, -1);
            } else {
                flag = 0;
            }
        }
    }
    self->mDoneLoadingTextures = flag;
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_loadTextures);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
extern "C" void* func_0014D998(void* db, int g);
int func_0014D988(void* db, int g);
extern "C" int func_0014B988(void* iface, int a1, int a2, void* ent, char* buf);
extern "C" int func_0019DA10(void* mgr, int bank, char* buf, int a3, int a4, int a5);
extern "C" int func_0019CCE8(void* self, int a1, int a2);
extern "C" void func_0019CD38(void* self, int bit, int a2, int val);
extern "C" void func_0019E070(void* mgr, int bank);
extern "C" void func_0019FA78(cFERider* self, cBECharProfileDB* list);
extern void* D_004A1968;

struct sBoneEntFBE0 {
    char f0;
    char f1;
    char f2;
    signed char type;       // 0x3
    short slot;             // 0x4
    char pad_0x6[0x38 - 0x6];
};

static inline tBEProfileBolt* getItemK19FBE0(cBECharProfileDB* list, int i)
{
    if (i >= 0)
        return &list->mBolts[i];
    return 0;
}

/**
 * Requeues the front-end rider's textures for its equipped items.
 *
 * Cancels the other textures (func_0019FA78), clears +0xCB8 and +0xCC0, queues the texture file of every part whose
 * item has flag 0x10 and no queued file (func_0014B988 name, func_0019DA10 kind 11) and starts the file manager on the
 * character's bank (func_0019E070).
 *
 * @param list  the character's item list
 * @param ignored    not used
 * @param keepLoaded    passed to func_0019DA10 as its keep-existing argument (nonzero keeps a slot already loading or loaded)
 */
void cFERider::loadTextures(cBECharProfileDB* list, int ignored, int keepLoaded)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 9);
    void* db = func_0014BDB8_noarg();
    char* mgr = SSXApp->RealFE->mBoltFileManager;
    func_0019FA78(this, (cBECharProfileDB*)list);
    mDoneLoadingTextures = 0;
    mDoneUpdatingModelParts = 0;
    int n = func_0014D988(db, mCharID);
    sBoneEntFBE0* partEntry = (sBoneEntFBE0*)func_0014D998(db, mCharID);
    int i;
    for (i = 0; i < n; i++, partEntry++) {
        tBEProfileBolt* it = getItemK19FBE0(list, list->mpBoltsMap[partEntry->slot]);
        if (func_0019CCE8(D_004A1968, mProfileIndex, it->id) < 0) {
            if (it->flags & 0x10) {
                char buf[0x100];
                if (func_0014B988(iface, mProfileIndex, mCharID, partEntry, buf)) {
                    int fileSlot = func_0019DA10(mgr, mCharID, buf, 11, keepLoaded, 1);
                    func_0019CD38(D_004A1968, mProfileIndex, it->id, fileSlot);
                }
            }
        }
    }
    func_0019E070(mgr, mCharID);
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_isDoneLoadingTextures);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" int func_0019CCE8(void* self, int a1, int a2);
extern "C" void func_0019CD38(void* self, int bit, int a2, int val);
extern "C" void func_0019DC20(void* self, int bank, int i);
extern "C" int func_0019E238(void* mgr, int bank, int i);
extern "C" void* func_0019E2B0(void* mgr, int bank, int i);
extern "C" void MEM_fill(void* dst, int c, int n);
extern void* D_004A1968;


/**
 * Finishes the front-end rider's texture loads: reports whether all are done and registers them.
 *
 * Returns 1 at once when +0xCB8 is set. Otherwise checks every queued texture file (func_0019E238); when all are ready,
 * registers each loaded package once (cFERider_addTextureFile, replacing for items with flag 0x10) and releases the files.
 *
 * @return 1 when every texture is loaded, else 0
 */
int cFERider::isDoneLoadingTextures()
{
    if (mDoneLoadingTextures != 0) {
        return 1;
    }
    int ok = 1;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 9);
    cBECharProfileDB* list = (cBECharProfileDB*)func_0014AD28(iface, mProfileIndex, mCharID);
    int n = list->mNumBolts;
    char* mgr = SSXApp->RealFE->mBoltFileManager;
    for (int i = 0; i < n; i++) {
        tBEProfileBolt* it = &list->mBolts[i];
        int fileSlot = func_0019CCE8(D_004A1968, mProfileIndex, it->id);
        if (fileSlot >= 0) {
            if (func_0019E238(mgr, mCharID, fileSlot) == 0) {
                ok = 0;
            }
        }
    }
    if (ok) {
        int used[256];
        MEM_fill(used, 0, sizeof(used));
        for (int i = 0; i < n; i++) {
            tBEProfileBolt* it = &list->mBolts[i];
            int fileSlot = func_0019CCE8(D_004A1968, mProfileIndex, it->id);
            if (fileSlot >= 0) {
                if (func_0019E2B0(mgr, mCharID, fileSlot) && used[fileSlot] == 0) {
                    SHAPEFILE* pkg = (SHAPEFILE*)func_0019E2B0(mgr, mCharID, fileSlot);
                    int fl = it->flags & 0x10;
                    addTextureFile(pkg, fl != 0);
                }
                used[fileSlot] = 1;
                func_0019DC20(mgr, mCharID, fileSlot);
                func_0019CD38(D_004A1968, mProfileIndex, it->id, -1);
            }
        }
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_loadModels);
#ifdef SKIP_ASM
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
extern "C" int func_0014D448(void* self, int g, int key);
extern "C" int func_0019CBE0(void* self, int a1, int a2);
extern "C" void func_0019CC30(void* self, int bit, int a2, int val);
extern "C" int func_0019DA10(void* mgr, int bank, char* buf, int a3, int a4, int a5);
extern "C" void func_0019DC20(void* self, int bank, int i);
extern "C" void func_0019DF20(void* mgr, int bank);
extern void* D_004A1968;

struct sBoneEntFF00 {
    char pad_0x0[0x28];
    char* file;             // 0x28
    char pad_0x2C[0x38 - 0x2C];
};
struct sBoneDbFF00 {
    int f0;
    sBoneEntFF00* entries;  // 0x4
};

static inline sBoneEntFF00* getEntK19FF00(sBoneDbFF00* db, int i)
{
    if (i < 0)
        return 0;
    return &db->entries[i];
}

/**
 * Queues the model-part files of the front-end rider's items.
 *
 * Unless @p force is set, first releases the queued parts of items without flag 0x10. Then for each item with no part
 * queued: when @p force is set, or the item has flag 0x10 and the matching entry of @p prev does not, queues the part's
 * file (kind 8) and clears +0xCBC and +0xCB4, or records -1 when the part has no file. Finally starts the file manager
 * on the character's bank (func_0019DF20).
 *
 * @param list   the character's item list
 * @param prev   the previous item list to compare with, or null
 * @param force  nonzero to queue every item's part
 * @param keepLoaded     passed to func_0019DA10 as its keep-existing argument
 */
void cFERider::loadModels(cBECharProfileDB* list, tBEProfileBolt* prev, int force, int keepLoaded)
{
    int n = list->mNumBolts;
    sBoneDbFF00* db = (sBoneDbFF00*)func_0014BDB8_noarg();
    char* mgr = SSXApp->RealFE->mBoltFileManager;
    for (int i = 0; i < n; i++) {
        tBEProfileBolt* it = &list->mBolts[i];
        if (force == 0 && (it->flags & 0x10) == 0) {
            if (func_0019CBE0(D_004A1968, mProfileIndex, it->id) >= 0) {
                func_0019DC20(mgr, mCharID, func_0019CBE0(D_004A1968, mProfileIndex, it->id));
                func_0019CC30(D_004A1968, mProfileIndex, it->id, -1);
            }
        }
    }
    for (int i = 0; i < n; i++) {
        tBEProfileBolt* it = &list->mBolts[i];
        if (func_0019CBE0(D_004A1968, mProfileIndex, it->id) < 0) {
            if (force || ((it->flags & 0x10) && (prev == 0 || (prev->flags & 0x10) == 0))) {
                sBoneEntFF00* partEntry = getEntK19FF00(db, func_0014D448(db, mCharID, it->id));
                if (partEntry->file) {
                    int fileSlot = func_0019DA10(mgr, mCharID, partEntry->file, 8, keepLoaded, 1);
                    func_0019CC30(D_004A1968, mProfileIndex, it->id, fileSlot);
                    mDoneLoadingModels = 0;
                    mCompiledModel = 0;
                } else {
                    func_0019CC30(D_004A1968, mProfileIndex, it->id, -1);
                }
            }
            if (prev) {
                prev++;
            }
        }
    }
    func_0019DF20(mgr, mCharID);
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_isDoneLoadingModels);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" int func_0014D448(void* self, int g, int key);
extern "C" int func_0019CBE0(void* self, int a1, int a2);
extern "C" void func_0019CC30(void* self, int bit, int a2, int val);
extern "C" void func_0019DC20(void* self, int bank, int i);
extern "C" int func_0019E238(void* mgr, int bank, int i);
extern "C" void* func_0019E2B0(void* mgr, int bank, int i);
void* cMdfArchive_cMdfArchive(void* self);
// PORT: cMdfArchive_attachConstArchive forwards its second argument (the archive data); bind the 2-arg form.
void* cMdfArchive_attachConstArchive_data(void* self, void* data) __asm__("cMdfArchive_attachConstArchive__FPv");
extern "C" void cMdfArchive__cMdfArchive(int* self, int flags);
extern "C" void* cMdfArchive_getModelPart(void* self, const char* name);
extern "C" void cAnimModel_addModelPartLOD(void* model, int lod, void* part, int a3);
extern void* D_004A1968;

struct sBoneEnt_A0100 {
    char f0;
    signed char lod;        // 0x1
    char pad_0x2[0x16];
    char* file;             // 0x18
    char pad_0x1C[0x38 - 0x1C];
};
struct sBoneDb_A0100 {
    int f0;
    sBoneEnt_A0100* entries;    // 0x4
};

static inline sBoneEnt_A0100* getEntK_A0100(sBoneDb_A0100* db, int i)
{
    if (i < 0)
        return 0;
    return &db->entries[i];
}

static inline sBoneEnt_A0100* getEnt_A0100(sBoneDb_A0100* db, int id, int g)
{
    return getEntK_A0100(db, func_0014D448(db, g, id));
}

/**
 * Adds the front-end rider's loaded model parts to its model: reports whether all are done.
 *
 * Returns 1 at once when +0xCBC is set. Otherwise checks every queued part file (func_0019E238); when all are ready,
 * opens each loaded archive once (cMdfArchive_cMdfArchive, cMdfArchive_attachConstArchive), adds the model part named by each item sharing it as
 * LOD (cMdfArchive_getModelPart, cAnimModel_addModelPartLOD), releases the files and closes the archive.
 *
 * @return 1 when every part is loaded, else 0
 */
// FAKEMATCH: identical arms on an always-non-null pointer in the inner loop (`void* g; if (self) g = D_004A1968; else
// g = D_004A1968;`, g passed to the first func_0019CBE0 call); the extra inner-loop insns keep the `li 56` (sizeof entry)
// out of loop.c's hoisting, and cross-jumping merges the arms after reload; plain form 98.50%. Byte-identical and
// behaves like the plain form; landed marked under the 2026-11-14 ruling (portable, behaviour-preserving steering only);
// replace it when an honest spelling matches.
extern "C" int cFERider_isDoneLoadingModels(int* self)
{
    if (*(int*)((char*)self + 0xCBC) != 0) {
        return 1;
    }
    int ok = 1;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 9);
    cBECharProfileDB* list = (cBECharProfileDB*)func_0014AD28(iface, self[1], self[0]);
    int n = list->mNumBolts;
    char* mgr = SSXApp->RealFE->mBoltFileManager;
    for (int i = 0; i < n; i++) {
        tBEProfileBolt* it = &((&list->mBolts[0]))[i];
        int fileSlot = func_0019CBE0(D_004A1968, self[1], it->id);
        if (fileSlot >= 0) {
            if (func_0019E238(mgr, self[0], fileSlot) == 0) {
                ok = 0;
            }
        }
    }
    if (ok) {
        for (int i = 0; i < n;) {
            int next = i + 1;
            tBEProfileBolt* it = &((&list->mBolts[0]))[i];
            int fileSlot = func_0019CBE0(D_004A1968, self[1], it->id);
            if (fileSlot >= 0) {
                cMdfArchive arc;
                cMdfArchive_cMdfArchive(&arc);
                void* pkg = func_0019E2B0(mgr, self[0], fileSlot);
                if (pkg) {
                    cMdfArchive_attachConstArchive_data(&arc, pkg);
                }
                for (int j = i; j < n; j++) {
                    tBEProfileBolt* jt = &((&list->mBolts[0]))[j];
                    void* slotTable;
                    if (self) {
                        slotTable = D_004A1968;
                    } else {
                        slotTable = D_004A1968;
                    }
                    if (func_0019CBE0(slotTable, self[1], jt->id) == fileSlot) {
                        if (pkg) {
                            sBoneEnt_A0100* partEntry = getEnt_A0100((sBoneDb_A0100*)func_0014BDB8_noarg(), jt->id, self[0]);
                            if (partEntry->file) {
                                void* part = cMdfArchive_getModelPart(&arc, partEntry->file);
                                if (part) {
                                    cAnimModel_addModelPartLOD(*(void**)((char*)self + 0x8), 0, part, partEntry->lod);
                                }
                            }
                        }
                        func_0019DC20(mgr, self[0], fileSlot);
                        func_0019CC30(D_004A1968, self[1], jt->id, -1);
                    }
                }
                cMdfArchive__cMdfArchive((int*)&arc, 2);
            }
            i = next;
        }
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERider_speak);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_002A16B0(void* mgr, int id);
extern "C" void func_002A1778(void* mgr, int id);

/**
 * Plays the front-end rider's pending speech line and clears the request.
 *
 * The customize request (+0xC1C, set by the equip screen) plays through func_002A1778 (sound 8381); otherwise the
 * character-select request (+0xC20) through func_002A16B0 (sound 8380); both on cAudioInterface_bxAudio() with the character id.
 */
void cFERider::speak()
{
    if (mCustomized != 0) {
        func_002A1778(cAudioInterface_bxAudio(), *(int*)this);
        mCustomized = 0;
    } else if (mSelected != 0) {
        func_002A16B0(cAudioInterface_bxAudio(), *(int*)this);
        mSelected = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERiderManager_cFERiderManager);
#ifdef SKIP_ASM
extern "C" void* cFERider_cFERider(void* self);

/**
 * Constructs the front end's rider manager: constructs its two riders (cFERider_cFERider, 0xCE0 bytes each).
 *
 * @return @p self
 */
extern "C" void* cFERiderManager_cFERiderManager(cFERiderManager* self)
{
    char* slot = (char*)self;
    int i;
    for (i = 1; i != -1; i--, slot += 0xCE0) {
        cFERider_cFERider(slot);
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERiderManager__cFERiderManager);
#ifdef SKIP_ASM
extern "C" void cFERiderManager_purge(cFERiderManager* self);
extern "C" void cFERider__cFERider(void* self, int flags);
void operator_delete(int* p);

/**
 * Destroys the front end's rider manager, freeing it when asked.
 *
 * Purges both riders (cFERiderManager_purge), destroys them (cFERider__cFERider) and calls operator_delete when bit 0 of @p flags is
 * set. cFE's destructor cFE__cFE calls it on +0xB0 with 2.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cFERiderManager__cFERiderManager(cFERiderManager* self, int flags)
{
    self->purge();
    if (self != 0) {
        cFERider* slot = self->mHumans + 2;
        while (self->mHumans != slot) {
            slot--;
            cFERider__cFERider(slot, 0);
        }
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERiderManager_init);
#ifdef SKIP_ASM

/**
 * Numbers the front end's two riders: sets each one's profile index (+0x4) to its slot (0, 1).
 *
 * cRealFE_load calls it on the rider manager.
 */
void cFERiderManager::init()
{
    int i;
    cFERider* slot = ((cFERider*)this);
    for (i = 0; i < 2; i++) {
        slot->mProfileIndex = i;
        slot++;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERiderManager_purge);
#ifdef SKIP_ASM
extern "C" void cFERider_purge(cFERider* slot);

/**
 * Purges both of the front end's riders (cFERider purge, cFERider_purge).
 *
 * cRealFE_purge calls it on the rider manager.
 */
void cFERiderManager::purge()
{
    int i;
    for (i = 0; i < 2; i++) {
        mHumans[i].purge();
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERiderManager_setHumanCharacter);
#ifdef SKIP_ASM
extern "C" void cFERider_setCharID(void* slot, int a1, int a2);

/**
 * Sets the character of front-end rider @p idx (cFERider_setCharID on that rider).
 *
 * @param idx  the rider (human index)
 * @param charId   the character id
 * @param loadAll   nonzero to load everything
 */
void cFERiderManager::setHumanCharacter(int idx, int charId, int loadAll)
{
    cFERider_setCharID(&mHumans[idx], charId, loadAll);
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERiderManager_getHuman);
#ifdef SKIP_ASM
/**
 * Returns front-end rider @p index (the manager plus 0xCE0 x index).
 *
 * @param index  the rider
 * @return the rider
 */
void* cFERiderManager::getHuman(int index)
{
    return (char*)&mHumans[index];
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERiderManager_getHuman1);
#ifdef SKIP_ASM
/**
 * Returns front-end rider @p index (the manager plus 0xCE0 x index).
 *
 * @param index  the rider
 * @return the rider
 */
cFERider* cFERiderManager::getHuman1(int index)
{
    return (cFERider*)(char*)&mHumans[index];
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERiderManager_isHumanVisible);
#ifdef SKIP_ASM
/**
 * Reports whether front-end rider @p index is shown (its +0xCC8).
 *
 * @param index  the rider
 * @return the shown flag
 */
int cFERiderManager::isHumanVisible(int index)
{
    return mHumans[index].mVisible;
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERiderManager_setHumanVisible);
#ifdef SKIP_ASM
extern "C" void cFERider_setVisible(void* slot, int v);

/**
 * Shows or hides front-end rider @p i (cFERider_setVisible on that rider).
 *
 * @param i  the rider
 * @param visible  nonzero to show it
 */
void cFERiderManager::setHumanVisible(int i, int visible)
{
    cFERider_setVisible(&mHumans[i], visible);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/feridermanager", cFERiderManager_render);
#ifdef SKIP_ASM
extern "C" void cFERider_render(cFERider* rider);

/**
 * Renders the front end's shown riders.
 *
 * Sets the graphics manager's words +0x24 to 46, +0x84 to 47 and +0x10A8 to its +0x1008, then renders each rider whose
 * shown flag (+0xCC8) is set (cFERider_render).
 */
void cFERiderManager::render()
{
    char* riderBase = (char*)GrMan;
    *(int*)(riderBase + 0x24) = 46;
    *(int*)(riderBase + 0x84) = 47;
    *(int*)(riderBase + 0x10A8) = *(int*)(riderBase + 0x1008);
    for (int i = 0; i < 2; i++) {
        riderBase = (char*)&mHumans[i];
        if (*(int*)(riderBase + 0xCC8) != 0)
            ((cFERider*)riderBase)->render();
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", cFERiderManager_update);
#ifdef SKIP_ASM
extern "C" void cFERider_update(cFERider* slot);

/**
 * Updates both front-end riders (cFERider_update).
 */
void cFERiderManager::update()
{
    int i;
    for (i = 0; i < 2; i++) {
        mHumans[i].update();
    }
}
#endif

