#include "common.h"
#include "render/ps2graphicsman.h"
#include "animation/animmodel.h"
#include "be/beintnewrace.h"
#include "online/gamecomm.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"
#include "ai/ai.h"

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
extern "C" void* func_002F1850(void* mem, void* rider);
extern "C" void* func_002EC9E0(void* self, int a1, int a2);
void* cPlayerShadow_cPlayerShadow(void* self);
extern "C" void cScriptObject_InitScriptObject(void* rig);
extern "C" void* cAudioInterface_bxAudio();
// PORT: cSSXAudio_RiderCreated is declared (void*) but takes (mgr, rider).
void cSSXAudio_RiderCreated_impl(void* mgr, void* rider) __asm__("cSSXAudio_RiderCreated__FPv");
extern "C" void* MEM_fill(void* dst, int value, int size);
// PORT: memset-like; called as void here.
extern "C" void MEM_fill_v(void* dst, int value, int size) __asm__("MEM_fill");
extern void* D_00459C18[];
extern char D_00459B90[];
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

struct sKey8_B698 {
    int a, b;
};
extern sKey8_B698 D_004A5960[];

struct sRiderAux_B698 {
    int f0;                     // 0x000
    int f4;                     // 0x004
    unsigned int f8;            // 0x008
    char padC[0x100];           // 0x00C
    unsigned int f10C;          // 0x10C
    char pad110[0x100];         // 0x110
    unsigned int f210;          // 0x210
    char pad214[0xA14 - 0x214];
};

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
    self->mVtbl = cBeing_vtbl;
    cScriptObjRider_cScriptObjRider(&self->mScriptObjRiderVtbl);
    self->mScriptObjectVtbl = D_00459BD0;
    self->mAIIndex = -1;
    self->mViewIndex = -1;
    self->mSafeRenderTick = -1;
    self->mScriptObjRiderVtbl = D_00459B90;
    self->mVtbl = D_00459C18;
    self->mGroundMaterial = 49;
    self->mLoadStep = 0;
    self->mRenderFlag = one;
    self->mIsHuman = 0;
    self->mIsActor = 0;
    self->mIsLocal = one;
    self->mRiderAnim = 0;
    self->mAnimModel = 0;
    self->mSphereTree = 0;
    self->unk_3FC = 0;
    self->mGroundPolyId = none;
    *(sKey8_B698*)&self->unk_AAC = D_004A5960[0];
    self->mTrackPathHint = (int*)cMemMan_alloc(0x20, str_Track_path_intersect_info, 0, 0);
    self->mAIPathHint = (int*)cMemMan_alloc(0x20, str_AI_path_intersect_info, 0, 0);
    sRiderAux_B698* aux = new (str_OverlapList, 0, 0) sRiderAux_B698;
    int nash = (int)SSXApp->Game->BXNash;  // the overlap list queries the game's Nash collision grid
    aux->f0 = nash;
    aux->f4 = 0;
    aux->f8 = none;
    aux->f10C = none;
    aux->f210 = none;
    self->mOverlapList = aux;
    self->mAttachedEmitterFX = func_002F1850(cMemMan_alloc(168, str_AttachedEmitterFX, 0, 0), self);
    self->mBoltOnFX = func_002EC9E0(cMemMan_alloc(200, str_cBoltOnFXInterface, 0, 0), (int)self, (int)self->mAttachedEmitterFX);
    self->mLineHint = cMemMan_alloc(0x10, str_Line_Intersect_Hint, 0, 0);
    self->mWallHint = cMemMan_alloc(0x10, str_Wall_Intersect_Hint, 0, 0);
    MEM_fill_v(self->unk_794, 0, 0x20);
    self->mFocusBox = 0;
    self->unk_5B0 = 0;
    self->mIsInNIS = 0;
    self->unk_AC8 = 0;
    self->unk_AD0 = 0;
    *(cQuad128*)&self->unk_AE0 = 0;
    self->unk_AF0 = 0;
    self->unk_AF4 = 0;
    self->unk_AF8 = 0;
    self->unk_AFC = 0;
    self->unk_B00 = one;
    self->unk_B04 = 0;
    int i;
    for (i = 3; i >= 0; i--) {
        self->unk_B08[i] = -1;
    }
    int invalidHandle = -1;
    self->mShadow = cPlayerShadow_cPlayerShadow(cMemMan_alloc(416, str_BoaderShadow, 0, 0));
    self->unk_450 = invalidHandle;
    self->unk_AA4 = 0;
    self->mIsOnScreen = 0;
    self->unk_B1C = 0;
    cScriptObject_InitScriptObject(self->unk_6DC);
    self->mWipeoutSlideHandle = invalidHandle;
    self->mWipeoutTumbleHandle = invalidHandle;
    cSSXAudio_RiderCreated_impl(cAudioInterface_bxAudio(), self);
    self->mBoostMeterStart = 0.0f;
    self->mBoostMode = 1;
    self->unk_B20 = 1;
    self->mStartHandicap = 0;
    self->mStatsIndex = 0;
    self->mBoostSourceMask = 31;
    self->mUberFlags = 3;
    self->unk_308 = 1;
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
extern "C" void* cAudioInterface_bxAudio();
// PORT: func_00289AF8 takes (mgr, rider); the mangled name says one void* param.
void func_00289AF8_impl(void* mgr, void* rider) __asm__("func_00289AF8__FPv");
extern "C" void cScriptObject_DeInitScriptObject(void* p);
extern "C" void cScriptObject__cScriptObject(void* p, int flags);
extern "C" void cAnimModel__cAnimModel(void* p, int flags);
extern "C" void cRiderSphereTree__cRiderSphereTree(void* p, int flags);
extern "C" void cAirPredictor__cAirPredictor(void* p, int flags);
extern "C" void cFocusMan_removebox(void* a, void* p);
extern "C" void cRiderMetrix__cRiderMetrix(void* p, int flags);
extern "C" void cPlayerShadow__cPlayerShadow(void* p, int flags);
void operator_delete(int* p);
extern void* D_00459C18[];
extern char D_00459B90[];
extern void* D_00459BD0[];
extern void* D_00482018[];
extern void* cScriptObjRider_vtbl[];
extern void* cBeing_vtbl[];

static inline void vdel_11B978(char* o, int off)
{
    sVE_11B978* vt = *(sVE_11B978**)(o + off);
    vt[1].fn(o + vt[1].delta, 3);
}

/**
 * Destroys a rider: frees the helper objects it owns, then unwinds its base classes.
 *
 * Reinstalls the cRider vtables, stops the rider's sounds (func_00289AF8), deinitialises the script object
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
    self->mScriptObjRiderVtbl = D_00459B90;
    self->mVtbl = D_00459C18;
    func_00289AF8_impl(cAudioInterface_bxAudio(), self);
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
    operator_delete(self->mTrackPathHint);
    operator_delete(self->mAIPathHint);
    member = self->mAirPredictor;
    if (member != 0)
        cAirPredictor__cAirPredictor(member, 3);
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
        cPlayerShadow__cPlayerShadow(member, 3);
    self->mScriptObjectVtbl = D_00482018;
    self->mScriptObjRiderVtbl = cScriptObjRider_vtbl;
    cScriptObject__cScriptObject(self->unk_6DC, 0);
    self->mVtbl = cBeing_vtbl;
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
extern "C" void cRider_addFocusBox(cRider* self)
{
    void* box = cMemMan_alloc(0x10, str_FocusBox, 0, 0);
    self->mFocusBox = box;
    cFocusMan_addbox(SSXApp->Game->AI->FocusMan, box);
}
#endif

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cRiderMetrix_cRiderMetrix(void* mem);
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
 * Allocates 0x1CC bytes tagged "RiderMetrix", constructs it with cRiderMetrix_cRiderMetrix(), stores it in
 * mMetrix and calls cRiderMetrix_linkToRider(metrix, self).
 */
void cRider_addRiderMetrix(cRider* self)
{
    void* mem = cMemMan_alloc(460, str_RiderMetrix, 0, 0);
    void* metrix = cRiderMetrix_cRiderMetrix(mem);
    self->mMetrix = (cRiderMetrix*)metrix;
    cRiderMetrix_linkToRider_impl(metrix, self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011BBE8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBELibrary_getCharacterID(int);
extern "C" int cBELibrary_getCheatCharacterID(int);
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
int func_0014D988(void* self, int i);
extern "C" void* func_0014D998(void* self, int i);
extern "C" void* func_0014AD50(void* iface, int rider);

struct sBEEntry_11BBE8
{
    int f0;
    short f4;
    char pad[0x32];
};

struct sSlot_11BBE8
{
    short a;
    unsigned short flags;
};

struct sRiderData_11BBE8
{
    char pad[0x288];
    short* map;         // 0x288
    int f28C;
    sSlot_11BBE8 slots[1]; // 0x290
};

/**
 * Fills @p list with the BE part entries the rider wears and returns how many.
 *
 * Uses the rider's character (cBELibrary_getCharacterID with mAIIndex) and the parts DB (func_0014BDB8, entry list
 * func_0014D998, count func_0014D988; entries are 0x38 bytes). A cheat character (cBELibrary_getCheatCharacterID
 * non-zero) takes all of its own entries. Otherwise each entry's slot in the rider's BE data (func_0014AD50 on
 * interface 9: map at +0x288 indexed by the entry's +4 short, slots of 4 bytes from +0x290) is taken when the slot's
 * flags have bit 0x10. Likely bug (retail too): an entry whose map value is negative gets a null slot whose flags are
 * still read (address 2). Called by cRider_parsePartsFromBuf with a 52-entry list.
 *
 * @param vself  the cRider
 * @param list  receives the entry pointers
 * @return the number of entries written
 */
extern "C" int func_0011BBE8(void* vself, char** list)
{
    cRider* self = (cRider*)vself;
    int n = 0;
    void* iface = cBE_getInterface(cBE_getBE(), 9);
    int ch = cBELibrary_getCharacterID(self->mAIIndex);
    int other = cBELibrary_getCheatCharacterID(self->mAIIndex);
    void* db = func_0014BDB8_noarg();
    if (other != 0)
    {
        ch = other;
        sBEEntry_11BBE8* part = (sBEEntry_11BBE8*)func_0014D998(db, ch);
        int cnt = func_0014D988(db, ch);
        for (int i = 0; i < cnt; i++)
        {
            list[n] = (char*)part;
            n++;
            part++;
        }
    }
    else
    {
        sRiderData_11BBE8* riderData = (sRiderData_11BBE8*)func_0014AD50(iface, self->mAIIndex);
        int cnt = func_0014D988(db, ch);
        sBEEntry_11BBE8* part = (sBEEntry_11BBE8*)func_0014D998(db, ch);
        for (int i = 0; i < cnt; i++, part++)
        {
            short idx = riderData->map[part->f4];
            sSlot_11BBE8* slot;
            if (idx >= 0)
                slot = &riderData->slots[idx];
            else
                slot = 0;
            if (slot->flags & 0x10)
            {
                list[n] = (char*)part;
                n++;
            }
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011BD60);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);
extern "C" char* strcpy(char* dst, const char* src);
void* func_0011B678(void* self);
extern "C" void func_00418EF8(void* base, int n, int size, void* (*cmp)(void*));

struct sAnimName_11BD60
{
    char name[0x100];
    int f100;
    int f104;
};

struct sAnimNameTable_11BD60
{
    sAnimName_11BD60 e[40];
    int count;      // 0x2940
};

/**
 * Adds the animation file names of @p n parts to the rider's load list (mLoadList), skipping duplicates, and sorts it.
 *
 * Each part's name is at +0x28; empty names are skipped and a name already in the table (func_004165A8) is not added
 * again. New entries (0x108 bytes: the name, -1, 0) go at the end and the count (+0x2940) is incremented; there is no
 * check against the table's 40 entries. The table is then sorted with func_00418EF8 and func_0011B678.
 *
 * @param list  the part entries
 * @param n  how many there are
 */
extern "C" void func_0011BD60(cRider* self, void** list, int n)
{
    for (int i = 0; i < n; i++, list++)
    {
        char* name = *(char**)((char*)*list + 0x28);
        sAnimNameTable_11BD60* nameTable = ((sAnimNameTable_11BD60*)self->mLoadList);
        if (name == 0) continue;
        if (*name == 0) continue;
        int j;
        for (j = 0; j < nameTable->count; j++)
        {
            if (func_004165A8(name, &nameTable->e[j]) == 0) goto next;
        }
        {
            sAnimName_11BD60* entry = &nameTable->e[nameTable->count];
            strcpy(entry->name, name);
            entry->f100 = -1;
            entry->f104 = 0;
            nameTable->count++;
        }
    next:;
    }
    sAnimNameTable_11BD60* sortTable = ((sAnimNameTable_11BD60*)self->mLoadList);
    func_00418EF8(sortTable, sortTable->count, 264, func_0011B678);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011BE88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_004165A8(const void* a, const void* b);
extern "C" char* strcpy(char* dst, const char* src);
void* func_0011B678(void* self);
extern "C" void func_00418EF8(void* base, int n, int size, void* (*cmp)(void*));
extern "C" int func_0014AC30(int c);
extern "C" int cBELibrary_getCheatCharacterID(int c);
extern "C" int func_0014AC50(int c);
extern "C" int func_0014B988(void* iface, int b, int a, void* item, char* out);

struct sAnimName_11BE88
{
    char name[0x100];
    int f100;
    int f104;
};

struct sAnimNameTable_11BE88
{
    sAnimName_11BE88 e[40];
    int start;      // 0x2940
    int count;      // 0x2944
};

/**
 * Adds the texture names of @p n parts to the rider's second load list (mLoadList seen with a start index at +0x2940
 * and a count at +0x2944), skipping duplicates, and sorts that range.
 *
 * For a cheat character the name is the part's +0x2C string; otherwise func_0014B988 on interface 9 builds it from the
 * character values func_0014AC50 / func_0014AC30 and the part into a 256-byte buffer, skipping parts it rejects. New
 * entries go at start + count, without a bound check, and the range is sorted with func_00418EF8 and func_0011B678.
 *
 * @param list  the part entries
 * @param n  how many there are
 */
extern "C" void func_0011BE88(cRider* self, void** list, int n)
{
    void* iface = cBE_getInterface(cBE_getBE(), 9);
    int characterId = func_0014AC30(self->mAIIndex);
    if (cBELibrary_getCheatCharacterID(self->mAIIndex)) {
        for (int i = 0; i < n; i++) {
            char* name = *(char**)((char*)list[i] + 0x2C);
            sAnimNameTable_11BE88* nameTable = ((sAnimNameTable_11BE88*)self->mLoadList);
            if (name == 0) continue;
            if (*name == 0) continue;
            int j;
            int end = nameTable->count + nameTable->start;
            for (j = nameTable->start; j < end; j++) {
                if (func_004165A8(name, &nameTable->e[j]) == 0) goto next;
            }
            {
                sAnimName_11BE88* entry = &nameTable->e[end];
                strcpy(entry->name, name);
                entry->f100 = -1;
                entry->f104 = 0;
                nameTable->count++;
            }
        next:;
        }
    } else {
        int profileIndex = func_0014AC50(self->mAIIndex);
        for (int i = 0; i < n; i++) {
            char buf[0x100];
            if (func_0014B988(iface, profileIndex, characterId, list[i], buf) == 0) continue;
            char* name = buf;
            sAnimNameTable_11BE88* nameTable = ((sAnimNameTable_11BE88*)self->mLoadList);
            if (*name == 0) continue;
            int j;
            int end = nameTable->count + nameTable->start;
            for (j = nameTable->start; j < end; j++) {
                if (func_004165A8(name, &nameTable->e[j]) == 0) goto next2;
            }
            {
                sAnimName_11BE88* entry = &nameTable->e[end];
                strcpy(entry->name, name);
                entry->f100 = -1;
                entry->f104 = 0;
                nameTable->count++;
            }
        next2:;
        }
    }
    sAnimNameTable_11BE88* sortTable = ((sAnimNameTable_11BE88*)self->mLoadList);
    func_00418EF8(&sortTable->e[sortTable->start], sortTable->count, 264, func_0011B678);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_isSafeToDelete);
#ifdef SKIP_ASM
struct sRiderVE_0011C0E0
{
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Tells whether the renderer is done with the rider, so that it can be deleted.
 *
 * mSafeRenderTick is a graphics-manager tick recorded for the rider (-1 = never set); the rider is safe once
 * the tick that the graphics manager's virtual slot 114 returns has passed it.
 *
 * @return 1 when mSafeRenderTick is set and lower than the graphics manager's tick, otherwise 0
 */
extern "C" int cRider_isSafeToDelete(cRider* self)
{
    if (self->mSafeRenderTick != -1)
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sRiderVE_0011C0E0* vt = (sRiderVE_0011C0E0*)ps2GraphicsMan->mVtbl;
        return self->mSafeRenderTick < vt[114].fn((char*)ps2GraphicsMan + vt[114].delta);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_parsePartsFromBuf);
#ifdef SKIP_ASM
void* cMdfArchive_cMdfArchive(void* self);
void cMdfArchive_attachArchive(void* self, int a1);
extern "C" void cMdfArchive__cMdfArchive(int* self, int flags);
extern "C" void* cMdfArchive_getModelPart(void* self, const char* name);
extern "C" int func_0011BBE8(void* self, char** list);
extern "C" void cAnimModel_addModelPartLOD(void* model, int lod, void* part, int a3);

struct sMdfArchive_11C138
{
    int a[4];
};

/**
 * Adds the rider's outfit model parts from an MDF archive buffer to its animation model.
 *
 * Wraps @p pData in a local cMdfArchive (cMdfArchive_cMdfArchive, cMdfArchive_attachArchive) and gets the rider's part entries
 * (func_0011BBE8: the equipped items of its character, or every part of a cheat character, into a 52-entry
 * list). For each entry's four LOD part names (at +0x18), the named archive part is added to mAnimModel at
 * that LOD as variant 0. The archive is destroyed with cMdfArchive__cMdfArchive(arc, 2).
 *
 * @param pData  the MDF archive data, passed as an int (On Tour: pData)
 */
extern "C" void cRider_parsePartsFromBuf(cRider* self, int pData)
{
    sMdfArchive_11C138 arc;
    char* list[52];
    cMdfArchive_cMdfArchive(&arc);
    cMdfArchive_attachArchive(&arc, pData);
    int n = func_0011BBE8(self, list);
    for (int i = 0; i < n; i++)
    {
        char* item = list[i];
        for (int j = 0; j < 4; j++)
        {
            const char* name = ((const char**)(item + 0x18))[j];
            if (name != 0)
            {
                void* part = cMdfArchive_getModelPart(&arc, name);
                if (part != 0)
                {
                    cAnimModel_addModelPartLOD(self->mAnimModel, j, part, 0);
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
 * cRider_loadRider(), yielding the thread for one tick after each call, until mLoadStep reaches 7.
 *
 * @param immediate  nonzero to make exactly one cRider_loadRider() call without waiting for mLoadStep
 */
extern "C" void cRider_initOnce(cRider* self, int immediate)
{
    if (immediate)
    {
        cRider_loadRider(self);
        return;
    }
    while (self->mLoadStep != 7)
    {
        cRider_loadRider(self);
        THREAD_yieldticks(1);
    }
}
#endif

INCLUDE_ASM("ai/rider", cRider_loadRider);

//100%
INCLUDE_ASM("ai/rider", cRider_initAlways);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface_11D390(void* be, int kind) __asm__("cBE_getInterface__Fv");
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


void cRider_initAlways_impl(cRider* self) __asm__("cRider_initAlways");
/**
 * Resets a loaded rider for a new run: stance, path, start position, timers and per-race state.
 *
 * Does nothing until the rider is loaded (mLoadStep == 7). Sets the handler's motion/control state words (+0xDE0 = 3,
 * +0xDE4 = 13), clears the rubber-band values, resets the metrix and anim base, sets IsGoofyStance from cBECharacterInterface_getIsGoofyFoot
 * (On Tour getIsGoofyFoot) and calls cRider_initPath (initPath(true)).
 *
 * Placement: a rider flagged by cBENewPlayerInterface_isFirstTimeWC on location 14 with no game path, in game mode 4, is put at the
 * back-country drop-off (cRider_putAtDropOffBC, motion 1, control 4, takeoff scored); in modes 4-6 it goes to a start
 * point chosen by the location (17-21: start 0 of type 2; 14-16: its own start of type 1; else start 1 of type 2) and
 * cRider_cleanupAfterPutAt; otherwise motion state 3 and control state 6. With a game path set the boost is
 * initialised. Finally the finish/uber/boost timers, start tick (= AITick), penalty and overlap counts, rival fields
 * and transitions are cleared and the vibration reset (vtable slot 16) and rider FX reset run.
 */
void cRider_initAlways_impl(cRider* self)
{
    if (self->mLoadStep == 7)
    {
        *(int*)((char*)self->mRiderHandler + 0xDE0) = 3;
        *(int*)((char*)self->mRiderHandler + 0xDE4) = 13;
        self->mRubberBandBehind = 0;
        self->mRubberBandAhead = 0;
        self->unk_0D8 = -1;
        cRiderMetrix_reset(self->mMetrix);
        cRiderAnimBase_reset(self->mRiderAnim);
        self->IsGoofyStance = cBECharacterInterface_getIsGoofyFoot(cBE_getInterface_11D390(cBE_getBE(), 2), self->mAIIndex);
        cRider_initPath(self, 1);
        void* iface = cBE_getInterface_11D390(cBE_getBE(), 0);
        // PORT: the gotos reproduce the shared compare chain (mode 4, 5, 6) that two different entry conditions fall into.
        if (cBENewPlayerInterface_isFirstTimeWC(cBE_getInterface_11D390(cBE_getBE(), 1), self->mAIIndex) != 0
            && cBENewRaceInterface_newRace.cGamePath == 0 && *cBENewRaceInterface_getLocation(iface) == 0xE)
        {
            if (cBENewRaceInterface_newRace.cGameMode == 4)
            {
                cRider_putAtDropOffBC(self);
                cRider_setMotionState_impl(self, 1);
                cRider_setControlState_impl(self, 4);
                cRiderMetrix_evTakeoff(self->mMetrix, 0, 0);
                goto done;
            }
            goto m5;
        }
        if (cBENewRaceInterface_newRace.cGameMode == 4)
            goto bb;
    m5:
        if (cBENewRaceInterface_newRace.cGameMode == 5)
            goto bb;
        if (cBENewRaceInterface_newRace.cGameMode == 6)
        {
        bb:
            int location = *cBENewRaceInterface_getLocation(iface);
            switch (location)
            {
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
                cRider_putAtStartpoint(self, 0, 2);
                cRider_cleanupAfterPutAt(self, 0);
                break;
            case 14:
            case 15:
            case 16:
                cRider_putAtStartpoint(self, self->mAIIndex, 1);
                cRider_cleanupAfterPutAt(self, 0);
                break;
            default:
                cRider_putAtStartpoint(self, 1, 2);
                cRider_cleanupAfterPutAt(self, 0);
                break;
            }
        }
        else
        {
            cRider_setMotionState_impl(self, 3);
            cRider_setControlState_impl(self, 6);
        }
    done:
        if (cBENewRaceInterface_newRace.cGamePath != 0)
            cRider_initBoost(self);
        self->mFinishTime = -1.0f;
        self->unk_3FC = 0;
        self->mUberTimer = 0;
        self->mUberLevel = 0;
        self->mDidNotFinish = 0;
        self->mStartTick = SSXApp->Game->AI->AITick;
        self->mFinishTick = 0;
        self->mPenaltyTicks = 0;
        self->mOverlapCount = 0;
        self->mPowerUpBoostTimer = 0;
        self->mPowerUpTrickTimer = 0;
        self->unk_5A0 = 0;
        sVEntry<void(void*)>* vt = (sVEntry<void (void*)>*)self->mVtbl;
        vt[kRiderVt_vibrationReset].fn((char*)self + vt[kRiderVt_vibrationReset].delta);
        cRiderHandler_resetRiderFX(self->mRiderHandler);
        self->unk_5A4 = 0;
        self->unk_5A8 = 0;
        self->unk_5AC = 0;
        self->mHasRival = 0;
        self->mLastShoveFrame = 0;
        self->mRivalIndex = -1;
        self->unk_100 = 1;
        cRider_cancelRiderTransitions(self);
        self->unk_5B0 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_isLoaded);
#ifdef SKIP_ASM
/**
 * Tells whether the rider has finished loading (mLoadStep reached 7).
 *
 * @return 1 when mLoadStep is 7, otherwise 0
 */
extern "C" int cRider_isLoaded(cRider* self)
{
    int loadStep = self->mLoadStep;
    if (loadStep == 7)
        return 1;
    return loadStep == 7;
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
extern "C" void func_002105B0(int);

// The unit declares these later with their own vector types; bind them by asm label.
extern const t4Vector D_004FF120_D660 __asm__("t4vect_Zero");
extern const t4Vector D_004FF130_D660 __asm__("t4vect_Origin");
extern const t4Vector D_004FF160_D660 __asm__("t4vect_ZAxis");

struct sRay_D660 {
    char data[0xB0];
} __attribute__((aligned(16)));

struct sHit_D660 {
    t4Vector pos;       // 0x00
    t4Vector normal;    // 0x10
    char pad20[0x60];
} __attribute__((aligned(16)));

void cWorldLine_cWorldLine_D660(sRay_D660* ray, const t4Vector& a, const t4Vector& b, int n, float t) __asm__("cWorldLine_cWorldLine");
extern float cBXNash_terrainIntersect1_D660(void* coll, sRay_D660* ray, sHit_D660* hit, int flags) __asm__("cBXNash_terrainIntersect1");

struct sVE_D660 { short delta; short index; void (*fn)(void*); };

// Quaternion; the ctor stores w, then y, x, z (that order reproduces the retail store schedule).
struct sQuat_D660
{
    float x, y, z, w;
    sQuat_D660() {}
    sQuat_D660(float ax, float ay, float az, float aw) { w = aw; y = ay; x = ax; z = az; }
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

static inline sQuat_D660 AxisAngle_D660(const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQuat_D660(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline sQuat_D660 AxisAngleL_D660(const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    sQuat_D660 q;
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
static inline t4Vector QuatMul_D660(const sQuat_D660& a, const t4Vector& b)
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

static inline void ClearCache_D660(char* c)
{
    *(int*)(c + 0x0) = 0;
    *(short*)(c + 0x4) = 0;
    *(short*)(c + 0x6) = 0;
    *(int*)(c + 0x8) = 0;
    *(int*)(c + 0xC) = 0;
}

static inline void InitChannel_D660(cRider* self, int i, float v, float z)
{
    self->mSlewed[i].target = v;
    self->mSlewed[i].cur = v;
    self->mSlewed[i].rate = z;
}

// PORT: the unit declares cRider_putAt(void*, sVec4_0011DE60*, ..., int, float) later; bind the cRider* view by asm label.
// drop is declared before anim: the order of the parameter copies follows it (ABI-identical: f12 and a3 either way).
extern "C" void cRider_putAt_impl(cRider* self, const t4Vector* pos, const float* dir, float drop, int anim) __asm__("cRider_putAt");
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
 * func_002105B0(mAIIndex), resets vibration, and resets the camera of a rider with a view (mViewIndex < 2).
 *
 * The C definition is cRider_putAt with an asm label.
 *
 * @param pos  where to put the rider
 * @param dir  the direction to face (On Tour desFront; only x and y are used)
 * @param drop  On Tour height: when >= 0, drop onto the ground below and stand this far above it
 * @param anim  the AI animation to start (On Tour tAIAnimID initAnim)
 */
extern "C" void cRider_putAt_impl(cRider* self, const t4Vector* pos, const float* dir, float drop, int anim)
{
    ClearCache_D660((char*)self->mLineHint);
    ClearCache_D660((char*)self->mWallHint);
    t4Vector placePos = *pos;
    t4Vector n = D_004FF160_D660;
    float angle = Atan2_D660(dir[1], dir[0]);
    self->mVisitedIds = 0xFFFFFFFF;
    if (drop >= 0.0f)
    {
        t4Vector lo = Add_D660(*pos, Scale_D660(D_004FF160_D660, -7000.0f));
        t4Vector hi = Add_D660(*pos, Scale_D660(D_004FF160_D660, 200.0f));
        sHit_D660 hit;
        sRay_D660 ray;
        cWorldLine_cWorldLine_D660(&ray, lo, hi, 2, 0.9791666865348816f);
        if (cBXNash_terrainIntersect1_D660(SSXApp->Game->BXNash, &ray, &hit, 0) >= 0.0f)
        {
            placePos = hit.pos;
            n = hit.normal;
        }
        AddEq_D660(placePos, Scale_D660(D_004FF160_D660, drop));
    }
    self->mPosition = placePos;
    *(sQuat_D660*)&self->mRotation = AxisAngle_D660(D_004FF160_D660, angle - 1.5707963705062866f);
    self->mUseRootPoseOverride = 0;
    t4Vector axis = Cross_D660(D_004FF160_D660, n);
    float len = Length_D660(axis);
    if (len > 0.0010000000474974513f)
    {
        DivEq_D660(axis, len);
        // PORT: g++ minimum operator.
        sQuat_D660 tiltRot = AxisAngleL_D660(axis, func_0031C128(len <? 1.0f));
        self->mRotation = QuatMul_D660(tiltRot, self->mRotation);
    }
    cRider_updateOrientationImplicit(self);
    self->mGroundNormal = n;
    self->unk_3D0 = D_004FF120_D660;
    self->mGroundForward = Normalize_D660(SubV_D660(self->mMatrix.row[1], Scale_D660(self->mGroundNormal, DotV_D660(self->mMatrix.row[1], self->mGroundNormal))));
    self->mGroundSide = Cross_D660(self->mGroundNormal, self->mGroundForward);
    self->mVelocity = D_004FF120_D660;
    self->mGroundContactPoint = self->mPosition;
    self->mSurfaceType = 0;
    self->unk_454 = 0.0f;
    self->mImpulse = D_004FF120_D660;
    self->mImpactAccum = 0.0f;
    self->mImpactAccum2 = 0.0f;
    InitChannel_D660(self, 0, 0.0f, 0.0f);
    InitChannel_D660(self, 1, 0.0f, 0.0f);
    InitChannel_D660(self, 2, 0.0f, 0.0f);
    InitChannel_D660(self, 3, 0.0f, 0.0f);
    InitChannel_D660(self, 4, 0.0f, 0.0f);
    InitChannel_D660(self, 5, 0.0f, 0.0f);
    InitChannel_D660(self, 6, 0.0f, 0.0f);
    InitChannel_D660(self, 7, 0.0f, 0.0f);
    InitChannel_D660(self, 8, 0.0f, 0.0f);
    InitChannel_D660(self, 9, 1.0f, 0.0f);
    InitChannel_D660(self, 10, 0.0f, 0.0f);
    InitChannel_D660(self, 11, 0.0f, 0.0f);
    InitChannel_D660(self, 12, 0.0f, 0.0f);
    InitChannel_D660(self, 13, 0.0f, 0.0f);
    InitChannel_D660(self, 14, 0.0f, 0.0f);
    InitChannel_D660(self, 15, 0.0f, 0.0f);
    InitChannel_D660(self, 16, 0.0f, 0.0f);
    InitChannel_D660(self, 17, 0.0f, 0.0f);
    InitChannel_D660(self, 18, 0.0f, 0.0f);
    self->mSpinRate = 0.0f;
    self->mFlipRate = 0.0f;
    self->mBoostStrength = 0.0f;
    self->mTimeScale = 1.0f;
    self->unk_318 = 1.0f;
    self->unk_31C = 0.0f;
    self->RailslideMode = 0;
    self->HandplantMode = 0;
    self->PressMode = 0;
    self->PunchVect = D_004FF120_D660;
    self->PunchStrength = 0.0f;
    self->ReactionEventTick = -1;
    self->ReactionEventType = 0;
    self->unk_35C = 0.0f;
    self->IsRidingFakie = self->IsGoofyStance;
    cRiderAnimBase_reset(self->mRiderAnim);
    *(int*)((char*)self->mRiderAnim + 0x18) = self->IsRidingFakie;
    float yaw = 0.0f;
    if (self->IsRidingFakie != 0)
        yaw = 3.1415927410125732f;
    char* riderAnim = (char*)self->mRiderAnim;
    *(t4Vector*)(riderAnim + 0x30) = D_004FF130_D660;
    *(sQuat_D660*)(riderAnim + 0x40) = AxisAngle_D660(D_004FF160_D660, -yaw);
    cRiderAnimBase_play(self->mRiderAnim, anim, 0, -1.0f);
    cRiderAnimBase_setupAnimation(self->mRiderAnim, 0);
    cRider_updateAnimation(self, 1.0f);
    cRider_updateSkeleton(self);
    cAnimModel_buildSkelMatrices(self->mAnimModel);
    cRider_updateBoundBox(self, 0);
    cOverlapList_geteverything(self->mOverlapList, &self->mBoundsMin);
    cRider_updateActiveLights(self);
    func_002105B0(self->mAIIndex);
    sVE_D660* vt = *(sVE_D660**)&self->mVtbl;
    vt[kRiderVt_vibrationReset].fn((char*)self + vt[kRiderVt_vibrationReset].delta);
    if ((unsigned int)self->mViewIndex < 2)
    {
        char* views = (char*)SSXApp->Game->ViewMan;
        if (views != 0)
        {
            char* viewHandler = *(char**)(*(char**)(views + (self->mViewIndex << 2) + 4) + 0xA8);
            sVE_D660* hv = *(sVE_D660**)(viewHandler + 0x14);
            hv[4].fn(viewHandler + hv[4].delta);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_putAtDropOffBC);
#ifdef SKIP_ASM
extern "C" void* cPathSys_getStartpoint(void*, int, int);
extern char gPaths[];
extern "C" float func_00115B08(void* self);

struct sVec4_0011DD98
{
    float x, y, z, w;
} __attribute__((aligned(16)));

// The unit declares cRider_putAt later with its own vector type; bind it by asm label.
void cRider_putAt_11DD98(void* self, sVec4_0011DD98* a, sVec4_0011DD98* b, int n, float t) __asm__("cRider_putAt");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_0011DD98 vu0Scale_11DD98(const sVec4_0011DD98& v, float s)
{
    sVec4_0011DD98 r;
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
static inline void vu0AddEq_11DD98(sVec4_0011DD98& dst, sVec4_0011DD98 b)
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
 * moves the point forward along its direction (floats 5-7) by func_00115B08(self), and calls cRider::putAt
 * (cRider_putAt) with that position and direction, anim id 0x11F and 2999.94.
 */
extern "C" void cRider_putAtDropOffBC(cRider* self)
{
    float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 2, self->mAIIndex);
    sVec4_0011DD98 dir;
    sVec4_0011DD98 pos;
    dir.x = startPoint[5];
    dir.y = startPoint[6];
    dir.z = startPoint[7];
    dir.w = 0.0f;
    pos.x = startPoint[2];
    pos.y = startPoint[3];
    pos.z = startPoint[4];
    pos.w = 1.0f;
    vu0AddEq_11DD98(pos, vu0Scale_11DD98(dir, func_00115B08(self)));
    cRider_putAt_11DD98(self, &pos, &dir, 287, 2999.942626953125f);
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

struct sVec4_0011DE60
{
    float x, y, z, w;
};

extern "C" void cRider_putAt(void* self, sVec4_0011DE60* a, sVec4_0011DE60* b, int n, float t);

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
extern "C" void cRider_putAtStartpoint(cRider* self, int StartpointID, int StartpointType)
{
    float* startPoint = (float*)cPathSys_getStartpoint(gPaths, StartpointType, StartpointID);
    sVec4_0011DE60 pos;
    sVec4_0011DE60 dir;
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
    cRiderMetrix_evReset(self->mMetrix, 1);
    cRider_setControlState_impl(self, 0);
    cRider_setMotionState_impl(self, 0);
    cRider_putAt(self, &pos, &dir, 5, 0.0f);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_cleanupAfterPutAt);
#ifdef SKIP_ASM
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" void cRiderHandler_resetRiderFX(void* self);
extern "C" void cWScriptMan_eventReset(void* self);
extern void* cWScriptMan_TheWScriptMan;

struct sVec4_0011DF18
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVEntryA_0011DF18 { short delta; short index; int (*fn)(void*); };
struct sVEntryB_0011DF18 { short delta; short index; void (*fn)(void*, int); };

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_0011DF18 vu0Scale_11DF18(const sVec4_0011DF18& v, float s)
{
    sVec4_0011DF18 r;
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
extern "C" void cRider_cleanupAfterPutAt(cRider* self, int notify)
{
    sVec4_0011DF18 forwardAxis = *(sVec4_0011DF18*)&self->mMatrix.row[1];
    *(int*)(cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 2) + 0x90) = 0;
    sVec4_0011DF18 launchVelocity = vu0Scale_11DF18(forwardAxis, 833.3333740234375f);
    *(sVec4_0011DF18*)&self->mVelocity = launchVelocity;
    self->mVelocity.z = 0.0f;
    cRiderHandler_resetRiderFX(self->mRiderHandler);
    if (notify != 0)
    {
        char* sub = (char*)&self->mVtbl;
        sVEntryA_0011DF18* vtA = *(sVEntryA_0011DF18**)sub;
        char* obj = *(char**)(*(char**)((char*)SSXApp->Game->ViewMan + 0x4) + 0xA0);
        sVEntryB_0011DF18* vtB = *(sVEntryB_0011DF18**)(obj + 0x14);
        char* objB = obj + vtB[3].delta;
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
extern "C" void cRider_rotateOrientation(cRider* self, t4Vector* axis, float angle)
{
    self->mRotation = QuatMul_0011DFE0(AxisAngle_0011DFE0(*axis, angle), self->mRotation);
    cRider_updateOrientationImplicit(self);
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
extern "C" void cRider_updateOrientationImplicit(cRider* self)
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
        : "r"(self), "r"(&self->mMatrix)
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

struct sBox_E150 {
    sVec4 min;
    sVec4 max;
};

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
static inline void Grow_E150(sBox_E150& b, const sVec4& c)
{
    SetMin_E150(b.min.x, c.x);
    SetMin_E150(b.min.y, c.y);
    SetMin_E150(b.min.z, c.z);
    SetMax_E150(b.max.x, c.x);
    SetMax_E150(b.max.y, c.y);
    SetMax_E150(b.max.z, c.z);
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
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Add_E150(Add_E150(Add_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 250.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Sub_E150(Add_E150(Add_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 50.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Add_E150(Sub_E150(Add_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 250.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Sub_E150(Sub_E150(Add_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 50.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Add_E150(Add_E150(Sub_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 250.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Sub_E150(Add_E150(Sub_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 50.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Add_E150(Sub_E150(Sub_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 250.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Sub_E150(Sub_E150(Sub_E150(*basePos, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 50.0f)));
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
extern "C" void cRider_updateAnimation(cRider* self, float value)
{
    cRiderAnimBase_updateAnimation(self->mRiderAnim, self->unk_B1C);
    cRiderAnimBase_expireAnimation(self->mRiderAnim);
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
extern t4Vector D_004FF220;
extern "C" void cSpinControl_applyFlipSpinAngle(void* self, void* xf);
extern "C" void cRider_doLeanPoseAdjust(void* self, void* xf);
extern "C" void cAnimModel_transform(cAnimModel* self, void* roots);
extern "C" void cRider_doLegIK(void* self, void* obj, void* xf, float t);
extern "C" void cRider_updateCollideInfo(void* self);
extern "C" float func_0031C128(float x);
extern "C" void fSinCos(float* s, float* c, float angle);
int cRider_getControlState(void* self);

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

extern "C" void cRider_updateSkeleton_impl(cRider* self) __asm__("cRider_updateSkeleton");
/**
 * Poses the rider's skeleton from its position and rotation plus spin, flip, lean, board tilt and leg adjustments.
 *
 * The root pose is mPosition/mRotation; in control state 5 (cRider_getControlState) the spin control (handler +0x230) adds the
 * flip/spin rotation (cSpinControl_applyFlipSpinAngle), then cRider_doLeanPoseAdjust leans it. Unless wiping out (state 13) channel 18
 * raises the body along its up axis. With mUseRootPoseOverride the board bones are reset and the board root takes the
 * override pose. cAnimModel_transform poses the bones from the body and board roots.
 *
 * Channel 17 then tilts the board-root bone toward the axis at +0x390 (and lifts it along mGroundNormal by unk_31C),
 * re-deriving the board child bone; unless wiping out, unk_318 drives cRider_doLegIK (per-leg adjustment) for the two
 * leg records at +0x8E0/+0x910. Finally the rider matrix (+0x160) is rebuilt from the pose and cRider_updateCollideInfo runs.
 *
 * The C definition is cRider_updateSkeleton with an asm label.
 */
extern "C" void cRider_updateSkeleton_impl(cRider* self)
{
    sXf_EB98 xf;
    xf.pos = *(sVec4W*)&self->mPosition;
    xf.rot = *(sVec4W*)&self->mRotation;
    if (cRider_getControlState(self) == 5)
        cSpinControl_applyFlipSpinAngle((char*)self->mRiderHandler + 0x230, &xf);
    cRider_doLeanPoseAdjust(self, &xf);
    sXf_EB98 ab[2];
    ab[1] = xf;
    ab[0] = xf;
    if (cRider_getControlState(self) != 13 && self->mSlewed[18].cur != 0.0f)
    {
        sVec4W liftOffset = Scale_EB98(t4vect_ZAxis, self->mSlewed[18].cur);
        ab[0].pos += Rot_EB98(ab[0].rot, liftOffset);
    }
    if (self->mUseRootPoseOverride != 0)
    {
        self->mAnimModel->localPos[self->mBoneBoardRoot] = t4vect_Origin;
        self->mAnimModel->localRot[self->mBoneBoardRoot] = D_004FF220;
        self->mAnimModel->localPos[self->mBoneBoardChild] = t4vect_Origin;
        self->mAnimModel->localRot[self->mBoneBoardChild] = D_004FF220;
        ab[1].pos = *(sVec4W*)&self->mRootPoseOverridePos;
        ab[1].rot = *(sVec4W*)&self->unk_140[0];
    }
    cAnimModel_transform(self->mAnimModel, ab);
    if (self->mSlewed[17].cur != 0.0f)
    {
        sXf_EB98* boardRoot = (sXf_EB98*)(self->mAnimModel->bones + self->mBoneBoardRoot);
        float qx = boardRoot->rot.x, qy = boardRoot->rot.y, qz = boardRoot->rot.z, qw = boardRoot->rot.w;
        sVec4W ax;
        ax.w = 0.0f;
        ax.x = 1.0f - 2.0f * (qy * qy + qz * qz);
        ax.y = 2.0f * (qx * qy + qw * qz);
        ax.z = 2.0f * (qx * qz - qw * qy);
        sVec4W tiltAxis = Cross_EB98(*(sVec4W*)&self->unk_390, ax);
        float len = Length_EB98(tiltAxis);
        if (len > 9.99999993922529e-09f)
        {
            float tiltDot = DivDot_EB98(tiltAxis, len, *(sVec4W*)&self->unk_390, ax);
            float sinTilt = Min_EB98(len, 1.0f);
            sVec4W tiltRot;
            if (tiltDot < 0.0f)
                tiltRot = AxisAngle_EB98(tiltAxis, (func_0031C128(sinTilt) - 1.5707963705062866f) * self->mSlewed[17].cur);
            else
                tiltRot = AxisAngle_EB98(tiltAxis, (1.5707963705062866f - func_0031C128(sinTilt)) * self->mSlewed[17].cur);
            boardRoot->rot = QuatMul_EB98(tiltRot, boardRoot->rot);
        }
        if (0.0f < self->unk_31C)
            AddTo_EB98(boardRoot->pos, Scale_EB98(Scale_EB98(*(sVec4W*)&self->mGroundNormal, self->unk_31C), self->mSlewed[17].cur));
        {
            cAnimModel* am = self->mAnimModel;
            sXf_EB98 local;
            local.pos = sVec4W(am->localPos[self->mBoneBoardChild].x * am->unk_140.x, am->localPos[self->mBoneBoardChild].y * am->unk_140.y, am->localPos[self->mBoneBoardChild].z * am->unk_140.z, am->localPos[self->mBoneBoardChild].w * am->unk_140.w);
            sVec4W* lr = (sVec4W*)&am->localRot[self->mBoneBoardChild];
            local.rot = *lr;
            sXf_EB98* bn = (sXf_EB98*)&am->bones[self->mBoneBoardChild];
            *bn = Combine_EB98(*boardRoot, local);
        }
    }
    if (cRider_getControlState(self) != 13 && self->unk_318 != 0.0f)
    {
        sXf_EB98* bb = (sXf_EB98*)(self->mAnimModel->bones + self->mBoneBoardChild);
        {
            sXf_EB98 xx = Combine_EB98(*bb, *(sXf_EB98*)((char*)self + 0x8F0));
            cRider_doLegIK(self, (char*)self + 0x8E0, &xx, self->unk_318);
        }
        {
            sXf_EB98 xx = Combine_EB98(*bb, *(sXf_EB98*)((char*)self + 0x920));
            cRider_doLegIK(self, (char*)self + 0x910, &xx, self->unk_318);
        }
    }
    QuatToMat_EB98((char*)self + 0x160, &xf.rot, &xf.pos);
    cRider_updateCollideInfo(self);
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
 * local offsets scaled by the model's unk_140 (its length scale).
 *
 * The knee angle follows from the law of cosines (func_0031C128 is an arcsine; its argument is clamped to [-1, 1]); the
 * middle bone's local transform is the upper length along x with a rotation about Z. The root is then turned about the
 * cross product of the bent leg's direction and the root-to-target direction, by the arcsine of that cross product's
 * length (skipped below 1e-5), and the middle bone is rebuilt from the turned root. cRider_updateSkeleton calls it for
 * the legs at +0x8E0 and +0x910 with blend unk_318.
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
    float upperLen = sVec4W(am->localPos[chain[1]].x * am->unk_140.x, am->localPos[chain[1]].y * am->unk_140.y,
                     am->localPos[chain[1]].z * am->unk_140.z, am->localPos[chain[1]].w * am->unk_140.w).x;
    sXf_EB98* mid = (sXf_EB98*)&bones[chain[1]];
    float lowerLen = sVec4W(am->localPos[chain[2]].x * am->unk_140.x, am->localPos[chain[2]].y * am->unk_140.y,
                     am->localPos[chain[2]].z * am->unk_140.z, am->localPos[chain[2]].w * am->unk_140.w).x;
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

static inline void LeanAbout_LP(char* self, sXf_LP* xf, float a, float b, float ka, float kb)
{
    char* trk = *(char**)(self + 0x780);
    sVec4XW* n = (sVec4XW*)((*(int*)(self + 0x8A0) << 4) + *(int*)(trk + 0x24));
    sVec4XW d = SubO_LP(sVec4XW(n->x * *(float*)(trk + 0x140), n->y * *(float*)(trk + 0x144),
                            n->z * *(float*)(trk + 0x148), n->w * *(float*)(trk + 0x14C)),
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

extern "C" void cRider_doLeanPoseAdjust_x(char* self, sXf_LP* xf) __asm__("cRider_doLeanPoseAdjust");
/**
 * Tilts the rider's root pose for its lean inputs.
 *
 * Takes slewed channels 2 and 0, each less a dead zone of twice |channel 3|; when either is nonzero the pose
 * is rotated about the board root bone (mBoneBoardRoot) by channel 2 x 30 degrees about X and channel 0 x 45
 * degrees about Y. A nonzero channel 8 adds channel 8 x 45 degrees about X, and the position moves along
 * mGroundSide by channel 0 x 45 x the model's length scale (unk_140.x).
 *
 * @param xf  the root pose to adjust, position and rotation quaternion (On Tour: tPose & root)
 */
extern "C" void cRider_doLeanPoseAdjust_x(char* self, sXf_LP* xf)
{
    float leanAboutX = deadzone_LP(*(float*)(self + 0x208), *(float*)(self + 0x214));
    float leanAboutY = deadzone_LP(*(float*)(self + 0x1F0), *(float*)(self + 0x214));
    if (leanAboutY != 0.0f || leanAboutX != 0.0f)
        LeanAbout_LP(self, xf, leanAboutX, leanAboutY, 0.523598849773407f, 0.7853982448577881f);
    float k = *(float*)(self + 0x250);
    if (k != 0.0f) {
        QuatMulTo_LP(xf->rot, AxisAngle_LP(t4vect_XAxis, k * 0.7853982448577881f));
    }
    sVec4XW sideShift = Scale_LP(*(sVec4XW*)(self + 0x3B0), *(float*)(*(char**)(self + 0x780) + 0x140) * (*(float*)(self + 0x1F0) * 45.0f));
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
 * @param vself  the cRider, untyped
 * @return the handler call's result
 */
void* cRider_setMotionState(void* vself)
{
    cRider* self = (cRider*)vself;
    return cRiderHandler__setMotionState(self->mRiderHandler);
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
 * @param vself  the cRider
 * @return the motion state
 */
int cRider_getMotionState(void* vself)
{
    cRider* self = (cRider*)vself;
    return *(int*)((char*)self->mRiderHandler + 0xDE0);
}
#endif

extern "C" void* cRiderHandler__setControlState(void* obj);

//100%
INCLUDE_ASM("ai/rider", cRider_getMotionStateName);
#ifdef SKIP_ASM
extern "C" void cRiderHandler__getMotionStateName(int);

/**
 * Returns the name of the rider's motion state: cRiderHandler__getMotionStateName on mRiderHandler.
 *
 * The C body has no return, but the callee's string pointer is left in v0.
 *
 * @param rider  the cRider
 */
extern "C" void cRider_getMotionStateName(cRider* rider) {
    cRiderHandler__getMotionStateName(((int)rider->mRiderHandler));
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
 * @param vself  the rider (this); the new control state follows in the second argument register
 * @return whatever cRiderHandler__setControlState returns (nothing meaningful; On Tour's is void)
 */
void* cRider_setControlState(void* vself)
{
    cRider* self = (cRider*)vself;
    return cRiderHandler__setControlState(self->mRiderHandler);
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
 * @param vself  the cRider
 * @return the control state
 */
int cRider_getControlState(void* vself)
{
    cRider* self = (cRider*)vself;
    return *(int*)((char*)self->mRiderHandler + 0xDE4);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getControlStateName);
#ifdef SKIP_ASM
extern "C" void cRiderHandler__getControlStateName(int);

/**
 * Returns the name of the rider's control state: cRiderHandler__getControlStateName on mRiderHandler.
 *
 * The C body has no return, but the callee's string pointer is left in v0.
 *
 * @param rider  the cRider
 */
extern "C" void cRider_getControlStateName(cRider* rider) {
    cRiderHandler__getControlStateName(((int)rider->mRiderHandler));
}
#endif
//100%
INCLUDE_ASM("ai/rider", cRider_getControlCmd);
#ifdef SKIP_ASM
struct sVEntry0011FF18 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

/**
 * Fetches the rider's current control command from its rider handler (slot 1 of the handler's vtable at +0xDE8).
 *
 * @param out  receives the command (On Tour tControlCmd& i_Cmd)
 */
// Forwards its second argument to vtable slot 1 of the rider handler.
extern "C" void cRider_getControlCmd(cRider* self, void* out)
{
    char* obj = (char*)self->mRiderHandler;
    sVEntry0011FF18* vt = *(sVEntry0011FF18**)(obj + 0xDE8);
    vt[1].fn(obj + vt[1].delta, out);
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
void* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
extern "C" float cRider_getMass(cRider* self)
{
    int weight = cBECharacterInterface_getWeight(cBE_getInterface(cBE_getBE(), 2), self->mAIIndex);
    float toughness = cBEStatInterface_getCollisionAttrib(cBE_getInterface(cBE_getBE(), 3),
                                                          self->mAIIndex, self->mStatsIndex);

    return (float)weight * (toughness * 1.5003352165222168f + 1.0f) * (self->mBoostStrength * 10.0f + 1.0f);
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
extern "C" float cRider_getGrabAnimSpeedScale(cRider* self)
{
    void* iface = cBE_getInterface(cBE_getBE(), 3);
    return cBEStatInterface_getTrickAttrib(iface, self->mAIIndex, self->mStatsIndex) * 0.29988324642181396f + 1.0f;
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
extern "C" void cRider_updateTimeWarp(cRider* self, float x)
{
    float cur = self->mTimeScale;
    if (x + 0.00083333341171965f < cur)
        x = cur - 0.00083333341171965f;
    else if (cur < x - 0.00083333341171965f)
        x = cur + 0.00083333341171965f;
    self->mTimeScale = x;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateBoost);
#ifdef SKIP_ASM
extern "C" void cRider_flagEventReaction(void* self, int mode, float t);

struct sVt_1200D0 {
    short delta;
    short index;
    int (*fn)(void*);
};

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
 * At uber level 10 the boost meter (unk_2F8) stays full until the timer runs out, then the level drops to 5
 * with a 20 s timer. Otherwise mBoostMode sets the meter: 0 full, 1 and 2 drain by step x 0.005 or step x
 * 0.024 except in CS_Gate. The isComputer virtual is also called and its result ignored.
 */
extern "C" void cRider_updateBoost(cRider* self)
{
    float step = self->mTimeScale * 0.01666666753590107f;
    self->mPowerUpBoostTimer = decay_1200D0(self->mPowerUpBoostTimer, step);
    if (0.01666666753590107f < self->mPowerUpTrickTimer || cRider_getMotionState(self) != 1) {
        self->mPowerUpTrickTimer = decay_1200D0(self->mPowerUpTrickTimer, step);
    }
    char* sub = (char*)&self->mVtbl;
    sVt_1200D0* vtEntry = &(*(sVt_1200D0**)sub)[kRiderVt_isComputer];
    vtEntry->fn(sub + vtEntry->delta);
    if ((cRider_getMotionState(self) == 1 || cRider_getControlState(self) == 12) && 0.0f < self->mUberTimer && self->mUberTimer <= 0.01666666753590107f) {
        self->mUberTimer = 0.01666666753590107f;
    } else if (0.0f < self->mUberTimer) {
        self->mUberTimer = decay_1200D0(self->mUberTimer, step);
        if (self->mUberTimer == 0.0f) {
            cRider_flagEventReaction(self, 6, 0.0f);
        }
    }
    if (self->mUberLevel == 10) {
        self->unk_2F8 = 1.0f;
        if (self->mUberTimer == 0.0f) {
            self->mUberLevel = 5;
            self->mUberTimer = 20.0f;
        }
    } else {
        switch (self->mBoostMode) {
        case 0:
            self->unk_2F8 = 1.0f;
            break;
        case 1:
            if (cRider_getControlState(self) != 6) {
                self->unk_2F8 = decay_1200D0(self->unk_2F8, step * 0.004999999888241291f);
            }
            break;
        case 2:
            if (cRider_getControlState(self) != 6) {
                self->unk_2F8 = decay_1200D0(self->unk_2F8, step * 0.02398611046373844f);
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120378);
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
 * The relative velocity is mVelocity - unk_3D0. Layers 3-5's current sequences get a playback rate of speed / 1666.7,
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
extern "C" void func_00120378(void* vself)
{
    cRider* self = (cRider*)vself;
    float sideLimit = 416.66668701171875f;
    float upLimit = 888.888916015625f;
    sV_120378 v = Sub_120378(*(sV_120378*)&self->mVelocity, *(sV_120378*)&self->unk_3D0);
    float speed = Length_120378(v);
    float rate = Clamp_120378(speed / 1666.666748046875f, 0.5f, 2.0f);
    if (cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 3)) {
        *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 3) + 0x90) = rate;
    }
    if (cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 4)) {
        *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 4) + 0x90) = rate;
    }
    if (cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 5)) {
        *(float*)(cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 5) + 0x90) = rate;
    }
    if (AITick() % 6 == self->mAIIndex) {
        int still = speed <= 138.88888549804688f;
        if (cRider_getControlState(self) == 11) {
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
 * The meter starts at mBoostMeterStart, at 0 in boost mode 3, or full (1) in mode 0.
 */
extern "C" void cRider_initBoost(cRider* self)
{
    self->unk_2F8 = self->mBoostMeterStart;
    int state = self->mBoostMode;
    if (state == 3) {
        self->unk_2F8 = 0.0f;
    } else if (state == 0) {
        self->unk_2F8 = 1.0f;
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
 * Fades the weight at unk_318, probably the leg-IK blend, in or out at 0.05 per frame.
 *
 * It goes toward 0 while the current animation of the rider animation's sequencer 2 (cRiderAnimBase_getCurrentAnimCategory) is
 * 0x14-0x1A, otherwise toward 1.
 */
extern "C" void cRider_updateLegIK(cRider* self)
{
    int animState = cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 2);
    if (animState >= 20 && animState <= 26)
        self->unk_318 = riderApproach_00120D90(self->unk_318, 0.0f, 0.05000000447034836f);
    else
        self->unk_318 = riderApproach_00120D90(self->unk_318, 1.0f, 0.05000000447034836f);
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
extern "C" void cRider_updateFocus(cRider* self)
{
    void* focusBox = self->mFocusBox;
    if (focusBox != 0) {
        *(cQuad128*)focusBox = *(cQuad128*)&self->mPosition;
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
extern "C" void cRider_updateOverlapList(cRider* self)
{
    cOverlapList_geteverything(self->mOverlapList, &self->mBoundsMin);
    cRider_updateActiveLights(self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplitMiscFX);
#ifdef SKIP_ASM
extern "C" void func_002F1A08(void*);
extern "C" void func_002ECF78(void*);

/**
 * Updates the rider's attached-emitter FX (func_002F1A08) and bolt-on FX (func_002ECF78).
 */
extern "C" void cRider_updateSplitMiscFX(cRider* self)
{
    func_002F1A08(self->mAttachedEmitterFX);
    func_002ECF78(self->mBoltOnFX);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120EB8);
#ifdef SKIP_ASM
extern "C" void cRiderHandler_updateRiderFX(int);

/**
 * Updates the rider's handler FX: cRiderHandler_updateRiderFX on mRiderHandler. Nothing in the source calls it.
 *
 * @param rider  the cRider
 */
extern "C" void func_00120EB8(void *rider) {
    cRiderHandler_updateRiderFX((*(int *)((char*)(rider) + (0x77C))));
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
extern "C" void cRider_updateSplitPaused(cRider* self)
{
    cRider_updateOrientationImplicit(self);
    if (self->mIsInNIS == 0)
    {
        cRider_updateAnimation(self, 1.0f);
        cRider_updateSkeleton(self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int AITick();
extern "C" int cPlaybackMan_isRecord(void* obj);
extern "C" void cOverlapList_geteverything(void*, void*);
extern "C" void cRider_updateActiveLights(cRider*);
extern "C" void cRider_SetNISGroundVectors(void* self, void* p);
extern "C" void cRider_updateTerminalVelocity(void* self);

struct sQuad_120F20
{
    float v[4];
} __attribute__((aligned(16)));
// t4vect_Zero is declared later in the unit as a 4-aligned sVec4; this view gives the lq/sq copy.
extern sQuad_120F20 D_004FF120_q120F20 __asm__("t4vect_Zero");

static inline int IsState1_120F20(void* obj)
{
    return *(int*)((char*)obj + 0x70) == 1;
}

/**
 * First step of the rider's frame update: overlap list, NIS ground probe and per-frame resets.
 *
 * Clears unk_3FC and unk_AA4 (shadow drawn). The overlap list and active lights are refreshed on the AI ticks
 * where AITick % 3 equals mAIIndex % 3, and on other ticks too unless the list's word +4 equals the Nash
 * grid's word +0xA0 while the playback manager's state (+0x70) is neither 1 nor 3.
 *
 * In an NIS the ground under mPosition (or the board root bone when unk_AFC is set) is probed with
 * cRider_SetNISGroundVectors. Then mModelOffset and unk_A30 are zeroed, mVisitedIds set to 0xFFFFFFFF, cRider_updateTerminalVelocity
 * called (probably On Tour's updateTerminalVelocity) and, once the rider has finished, mFinishTime advanced
 * by 1/60.
 */
extern "C" void cRider_updateSplit1(cRider* self)
{
    self->unk_3FC = 0;
    self->unk_AA4 = 0;
    int frameSlot = AITick() % 3;
    if (frameSlot != self->mAIIndex % 3)
    {
        char* game = (char*)SSXApp->Game;
        if (*(int*)((char*)self->mOverlapList + 4) == *(int*)(*(char**)(game + 0x20) + 0xA0))
        {
            void* obj = *(void**)(game + 0x34);
            int busy = 0;
            if (cPlaybackMan_isRecord(obj) || IsState1_120F20(obj))
                busy = 1;
            if (busy == 0)
                goto skip;
        }
    }
    cOverlapList_geteverything(self->mOverlapList, &self->mBoundsMin);
    cRider_updateActiveLights(self);
skip:
    if (self->mIsInNIS)
    {
        char* refPos = (char*)&self->mPosition;
        if (self->unk_AFC)
            refPos = (char*)self->mAnimModel->bones + (self->mBoneBoardRoot << 5);
        cRider_SetNISGroundVectors(self, refPos);
    }
    *(sQuad_120F20*)&self->mModelOffset = D_004FF120_q120F20;
    self->unk_A30 = 0;
    self->mVisitedIds = 0xFFFFFFFF;
    cRider_updateTerminalVelocity(self);
    float finishTime = self->mFinishTime;
    if (finishTime >= 0.0f)
        self->mFinishTime = finishTime + 0.01666666753590107f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit2);
#ifdef SKIP_ASM
struct sVEntry00121068 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

extern "C" void cRiderHandler_updateControl(void* obj, void* v);

/**
 * Reads the rider's control command and feeds it to its current control state.
 *
 * Calls the handler's getControlCmd virtual (slot 1) into a 16-byte buffer, then
 * cRiderHandler_updateControl() with it.
 */
extern "C" void cRider_updateSplit2(cRider* self)
{
    int buf[4];
    char* obj = (char*)self->mRiderHandler;
    sVEntry00121068* vt = *(sVEntry00121068**)(obj + 0xDE8);
    vt[1].fn(obj + vt[1].delta, buf);
    cRiderHandler_updateControl(self->mRiderHandler, buf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_updateSplit3);
#ifdef SKIP_ASM
extern "C" void cRider_checkForReset(void* rider, int a, int b);
extern "C" void cRider_updateBoost(cRider* self);
extern "C" void cRider_updateLegIK(cRider* self);

struct sVec4_1210B0
{
    float v[4];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (v *= s).
static inline void VecScale_1210B0(sVec4_1210B0* v, float s)
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
extern "C" void cRider_updateSplit3(cRider* self)
{
    VecScale_1210B0((sVec4_1210B0*)&self->mImpulse, 0.9800000190734863f);
    self->mImpactAccum *= 0.9559999704360962f;
    self->mImpactAccum2 *= 0.9783333539962769f;
    if (self->mImpactAccum > 4.5f)
    {
        cRider_checkForReset(self, 0, 2);
    }
    else if (cRider_getMotionState(self) == 1 ||
             (cRider_getMotionState(self) == 2 && *(int*)((char*)self->mRiderHandler + 0x30) == 1))
    {
        char* obj = (char*)self->mAirPredictor;
        int ok = 0;
        if (*(int*)(obj + 0xAC) == 1 || *(int*)(obj + 0xAC) == 3)
            ok = 1;
        obj = (char*)self->mAirPredictor;
        if ((ok && *(float*)(obj + 0x98) - *(float*)(obj + 0xA0) > 45.0f) || self->mImpactAccum2 > 5.0f)
            cRider_checkForReset(self, 0, 2);
    }
    cRider_updateBoost(self);
    cRider_updateLegIK(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_updateSplit4);
#ifdef SKIP_ASM
extern "C" void func_00120378(void* self);

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
 * func_00120378 updates the head and upper-body look controllers; each mSlewed channel then steps cur toward
 * target by at most rate.
 */
extern "C" void cRider_updateSplit4(cRider* self)
{
    func_00120378(self);
    approach_11F8(&self->mSlewed[0]);
    approach_11F8(&self->mSlewed[1]);
    approach_11F8(&self->mSlewed[2]);
    approach_11F8(&self->mSlewed[3]);
    approach_11F8(&self->mSlewed[4]);
    approach_11F8(&self->mSlewed[5]);
    approach_11F8(&self->mSlewed[6]);
    approach_11F8(&self->mSlewed[7]);
    approach_11F8(&self->mSlewed[8]);
    approach_11F8(&self->mSlewed[9]);
    approach_11F8(&self->mSlewed[10]);
    approach_11F8(&self->mSlewed[11]);
    approach_11F8(&self->mSlewed[12]);
    approach_11F8(&self->mSlewed[13]);
    approach_11F8(&self->mSlewed[14]);
    approach_11F8(&self->mSlewed[15]);
    approach_11F8(&self->mSlewed[16]);
    approach_11F8(&self->mSlewed[17]);
    approach_11F8(&self->mSlewed[18]);
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
 * @param vself  the cRider
 * @return cRiderHandler_stepMotion's result
 */
void* cRider_updateSplit5(void* vself)
{
    cRider* self = (cRider*)vself;
    return cRiderHandler_stepMotion(self->mRiderHandler);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit6);
#ifdef SKIP_ASM
extern "C" void cRider_updateAnimation(cRider* self, float value);

/**
 * Steps the rider's animation by its time scale (mTimeScale), unless it is in an NIS.
 */
extern "C" void cRider_updateSplit6(cRider* self)
{
    if (self->mIsInNIS == 0)
    {
        cRider_updateAnimation(self, self->mTimeScale);
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
extern "C" void cRider_updateSplit7(cRider* self)
{
    if (self->mIsInNIS == 0)
    {
        cRider_updateSkeleton(self);
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
extern "C" void cRider_updateSplit8(cRider* self)
{
    cRiderHandler_resolveMotion(self->mRiderHandler);
    if (VecChanged_00121750(self, t4vect_Zero))
    {
        cAnimModel_applyOffset(self->mAnimModel, &self->mModelOffset);
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
 * @param vself  the rider (this), viewed as cRider* self
 * @return cAnimModel_buildSkelMatrices's value (On Tour's updateSplit9 returns void)
 */
void* cRider_updateSplit9(void* vself)
{
    cRider* self = (cRider*)vself;
    return cAnimModel_buildSkelMatrices(self->mAnimModel);
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
extern "C" int cRider_updateFinishTest(cRider* self);
extern "C" void cRiderMetrix_update(void* metrix);

struct sVEntry00121818 { short delta; short index; void (*fn)(void*, void*, void*, void*); };

/**
 * Runs the rider's object script hook, then its path, finish, metrix and focus updates.
 *
 * When unk_A30 is set, calls virtual slot 40 of its object (+0xC) with unk_A60, unk_9E0 and the rider, or,
 * without one, the world-script manager's cWScriptMan_doOnCollideScript with the same arguments. Then cRider_updateTestSurface (empty),
 * cRider_updateTrackPath (outside an NIS), cRider_updateAIPath, cRider_updateFinishTest(), the metrix update cRiderMetrix_update
 * (outside an NIS) and cRider_updateFocus().
 */
extern "C" void cRider_updateSplit10(cRider* self)
{
    char* collideInst = (char*)self->unk_A30;
    if (collideInst != 0)
    {
        char* obj = *(char**)(collideInst + 0xC);
        if (obj != 0)
        {
            sVEntry00121818* vt = *(sVEntry00121818**)(obj + 0xC);
            vt[40].fn(obj + vt[40].delta, self->unk_A60, self->unk_9E0, (char*)&self->mVtbl);
        }
        else
        {
            cWScriptMan_doOnCollideScript(cWScriptMan_TheWScriptMan, self->unk_A60, self->unk_9E0, (char*)&self->mVtbl);
        }
    }
    cRider_updateTestSurface(self);
    if (self->mIsInNIS == 0)
    {
        cRider_updateTrackPath(self);
    }
    cRider_updateAIPath(self);
    cRider_updateFinishTest(self);
    if (self->mIsInNIS == 0)
    {
        cRiderMetrix_update(self->mMetrix);
    }
    cRider_updateFocus(self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit11);
#ifdef SKIP_ASM
extern "C" int cSectionMan_getBESectionIDFromWorldSectionID(void* self, int a1);
extern "C" void cRenderStateMan_update(void* obj, float a, float b, float c);
extern "C" void cPlayerLights_Clear(void* p);

struct sRiderVE_001218D0
{
    short delta;
    short index;
    void* (*fn)(void*);
};

/**
 * Updates the rider's ground section and lens FX position, and clears its handler lights.
 *
 * When mGroundPolyId is valid, the BE section id of its world section (its low byte) is stored in
 * mGroundMaterial, so that field holds a section id here. Then calls cRenderStateMan_update(AI index, contact x,
 * contact y, -99999) (lens FX, unknown) and clears the handler's cPlayerLights (cPlayerLights_Clear).
 */
extern "C" void cRider_updateSplit11(cRider* self)
{
    if (~self->mGroundPolyId != 0)
        self->mGroundMaterial = cSectionMan_getBESectionIDFromWorldSectionID(SSXApp->Game->SectionMan, (unsigned char)self->mGroundPolyId);
    char* obj = (char*)&self->mVtbl;
    sRiderVE_001218D0* vt = *(sRiderVE_001218D0**)obj;
    cRenderStateMan_update(vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta), self->mGroundContactPoint.x, self->mGroundContactPoint.y, -99999.0f);
    char* handler = (char*)self->mRiderHandler;
    cPlayerLights_Clear(handler ? handler + 0xD30 : 0);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateSplit12);
#ifdef SKIP_ASM
extern "C" int cRider_isGroundNormalValid(cRider* self);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_UpdateAudioWorldPainter(void* mgr, void* rider);
extern "C" float func_0031C228(float x);
extern "C" void cRider_updateDangerLevel(cRider* self);

struct sVec2_121950
{
    float x, y;
    sVec2_121950(float a, float b) : x(a), y(b) {}
};

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_121950(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Length_121950(const sVec2_121950& v)
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
extern "C" void cRider_updateSplit12(cRider* self)
{
    cSSXAudio_UpdateAudioWorldPainter(cAudioInterface_bxAudio(), self);
    if (cRider_isGroundNormalValid(self))
    {
        sVec2_121950 normalXY(self->mGroundNormal.x, self->mGroundNormal.y);
        float z = self->mGroundNormal.z;
        float ang = Atan2_121950(Length_121950(normalXY), z);
        float headingDelta = ang - self->mSmoothedHeading;
        if (headingDelta > 3.1415927410125732f)
            headingDelta -= 6.2831854820251465f;
        else if (headingDelta < -3.1415927410125732f)
            headingDelta += 6.2831854820251465f;
        self->mSmoothedHeading += headingDelta * 0.15000000596046448f;
    }
    cRider_updateDangerLevel(self);
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
extern t4Vector D_004FF120_1AA0 __asm__("t4vect_Zero");
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
// retail frame keeps during the heading rotation. The final rotation below is written out instead (99.19 with this helper).
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
    if (neq_1AA0(fwd, D_004FF120_1AA0)) {
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
    cRider_updateOrientationImplicit(self);
}
#undef ROT_1AA0
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121F30);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);

struct sVec4_121F30
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVEntryP_121F30 { short delta; short index; void* (*fn)(void*); };
struct sVEntryF_121F30 { short delta; short index; float (*fn)(void*); };

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4x4 matrix * vector).
static inline sVec4_121F30 mtxMulVec_121F30(sVec4_121F30* m, sVec4_121F30* v)
{
    sVec4_121F30 r;
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
extern "C" void func_00121F30(cRider* self)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVEntryP_121F30* gvt = (sVEntryP_121F30*)ps2GraphicsMan->mVtbl;
    sVec4_121F30* viewMtx = (sVec4_121F30*)gvt[35].fn((char*)ps2GraphicsMan + gvt[35].delta);
    char* obj = (char*)&self->mVtbl;
    sVEntryP_121F30* vt = *(sVEntryP_121F30**)obj;
    sVec4_121F30* pos = (sVec4_121F30*)vt[kRiderVt_getPosition].fn(obj + vt[kRiderVt_getPosition].delta);
    sVec4_121F30 viewPos = mtxMulVec_121F30(viewMtx, pos);
    float z = viewPos.z;
    cPS2GraphicsMan* graphicsMan = GrMan;
    sVEntryF_121F30* gvt2 = (sVEntryF_121F30*)graphicsMan->mVtbl;
    float fov = gvt2[29].fn((char*)graphicsMan + gvt2[29].delta);
    float sinFov, cosFov;
    fSinCos(&sinFov, &cosFov, fov);
    z = z * (sinFov / cosFov);
    z = z * 0.009999999776482582f;
    if (z < 6.0f)
    {
        self->mLOD = 0;
    }
    else
    {
        int lod = 2;
        if (z < 10.0f) lod = 1;
        self->mLOD = lod;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122048);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Builds a 0xA0-byte record with func_00389C38 and passes it to virtual slot 69 of the object at GrMan.
class cRiderSys_122048
{
public:
    char pad_0x00[0x10D8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69(void* rec, int flags);
};
extern "C" void func_00389C38(void* rec, int flags);

/**
 * Builds a cleared 160-byte light record (func_00389C38 with 0) and hands it to GrMan's virtual slot 69 with 0.
 *
 * Nothing in the source calls it.
 */
extern "C" void func_00122048()
{
    char rec[160];
    func_00389C38(rec, 0);
    ((cRiderSys_122048*)GrMan)->v69(rec, 0);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateActiveLights);
#ifdef SKIP_ASM
extern "C" void func_002F5B68(void* a, void* q, void* b, void* c, int d);

/**
 * Picks the world lights that light the rider into its active-light list (unk_794).
 *
 * func_002F5B68 keeps the 8 best-scoring lights of type 6 among those of the overlap list, scored from the
 * rider's hip position.
 */
extern "C" void cRider_updateActiveLights(cRider* self)
{
    cQuad128 hipPos;
    cRider_getHipPosition(&hipPos, self);
    func_002F5B68(SSXApp->Game->WorldLightMan, &hipPos, self->mOverlapList, self->unk_794, 8);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_SetRiderLights);
#ifdef SKIP_ASM
struct sVE_20D8 {
    short delta;
    short index;
    void* (*fn)(void*);
};
struct sXf_20D8 {
    float m[40];
} __attribute__((aligned(16)));
struct sV3_20D8 {
    float x, y, z;
    sV3_20D8() {}
    sV3_20D8(float a, float b, float c) : x(a), y(b), z(c) {}
};
struct sPt_20D8 { float x, y, z; };
struct sQ_20D8 { float v[4]; };
struct sPath_20D8 {
    float x, y, z;
    int n;
    sPt_20D8 pts[5];
    int pad;
    sQ_20D8 q[5];
};
extern "C" void func_00389260(void* p);
extern "C" void* func_002EDFB8(void* p);
extern "C" float func_002EEFA8(void* p);
extern "C" void* cRider_getHipPosition(void* dst, cRider* self);
extern "C" void func_00389CB8(void* m, void* q, float f);
extern "C" void func_0038A6A8(void* m, void* q, void* p);
extern "C" void func_00389558(void* m, sV3_20D8* v);
extern "C" void func_00389520(void* m, sQ_20D8* q, sV3_20D8* v);

/**
 * Builds the rider's light block (unk_7C0) from its environment, active world lights and handler lights.
 *
 * Clears the 10-quadword block, copies the rider's lens-table entry (func_002EDFB8) to its start, adds
 * func_00389CB8's term at the hip position with func_002EEFA8's value, adds each active light of unk_794
 * (func_0038A6A8), then the handler cPlayerLights' extra ambient (func_00389558) and each of its directional
 * lights (func_00389520).
 */
extern "C" void cRider_SetRiderLights(cRider* self)
{
    sQ_20D8 hipPos;
    func_00389260(self->unk_7C0);
    {
        char* obj = (char*)&self->mVtbl;
        sVE_20D8* vt = *(sVE_20D8**)obj;
        *(sXf_20D8*)&self->unk_7C0[0] = *(sXf_20D8*)func_002EDFB8(vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta));
    }
    cRider_getHipPosition(&hipPos, self);
    char* lightState = self->unk_7C0;
    {
        char* obj = (char*)&self->mVtbl;
        sVE_20D8* vt = *(sVE_20D8**)obj;
        func_00389CB8(lightState, &hipPos, func_002EEFA8(vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta)));
    }
    for (int i = 0; i < 8; i++) {
        void* light = self->unk_794[i];
        if (light != 0)
            func_0038A6A8(self->unk_7C0, &hipPos, light);
    }
    char* x = (char*)self->mRiderHandler;
    sPath_20D8* playerLights = x ? (sPath_20D8*)(x + 0xD30) : 0;
    if (playerLights != 0) {
        {
            sV3_20D8 lightColour(playerLights->x, playerLights->y, playerLights->z);
            func_00389558(self->unk_7C0, &lightColour);
        }
        for (int j = 0; j < playerLights->n; j++) {
            sQ_20D8* qq = &playerLights->q[j];
            sV3_20D8 lightColour(playerLights->pts[j].x, playerLights->pts[j].y, playerLights->pts[j].z);
            func_00389520(self->unk_7C0, qq, &lightColour);
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
struct sQ128_122278 { float v[4]; } __attribute__((aligned(16)));
struct sVE_122278 {
    short delta;
    short index;
    void* fn;
};
typedef sMat_122278 (*GetMatFn_122278)(void*);
typedef int (*TestFn_122278)(void*, void*, void*, sMat_122278*);

static inline int inFrustum_122278(sQ128_122278* v)
{
    char* inst = (char*)SSXApp->Game->OcclusionMan;
    int mask = *(int*)(inst + 0x110);
    for (int i = 0; i < *(int*)(inst + 0x10C); i++) {
        int bit = 1 << i;
        if (mask & bit) {
            sQ128_122278* m = (sQ128_122278*)(*(char**)(inst + 8 + i * 8) + 0x50);
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
    sVE_122278* vt = (sVE_122278*)ps2GraphicsMan->mVtbl;
    char* obj = (char*)ps2GraphicsMan + vt[93].delta;
    sMat_122278 m = ((GetMatFn_122278)vt[43].fn)((char*)ps2GraphicsMan + vt[43].delta);
    return ((TestFn_122278)vt[93].fn)(obj, a, b, &m);
}

/**
 * Decides whether the rider is on screen this frame and stores the answer in mIsOnScreen.
 *
 * Off screen when mRenderFlag is clear, the rider is not loaded or unk_AD0 is set. Otherwise on screen unless
 * the graphics manager's box test (virtual slot 93) puts the bounding box outside the view; with
 * mUseRootPoseOverride the handler's box (+0x80, +0x90) is tried too, and without it a bounding sphere hidden
 * by an occluder of the occlusion manager counts as off screen.
 *
 * @return the new mIsOnScreen: 1 on screen, 0 not
 */
extern "C" int cRider_renderClip(cRider* self)
{
    if (self->mRenderFlag == 0 || self->mLoadStep != 7 || self->unk_AD0 != 0) {
        self->mIsOnScreen = 0;
        return 0;
    }
    self->mIsOnScreen = test_122278(&self->mBoundsMin, &self->mBoundsMax) != 1;
    if (self->mUseRootPoseOverride != 0) {
        if (self->mIsOnScreen == 0) {
            char* handler = (char*)self->mRiderHandler;
            self->mIsOnScreen = test_122278(handler + 0x80, handler + 0x90) != 1;
        }
    }
    if (self->mUseRootPoseOverride == 0) {
        if (inFrustum_122278((sQ128_122278*)&self->mBoundSphere))
            self->mIsOnScreen = 0;
    }
    return self->mIsOnScreen;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_renderBody);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00121F30(cRider* self);
extern "C" void cAnimModel_render(void* model, int lod, int a, int b);

struct sVec4_122448 { float v[4]; };
extern sVec4_122448 D_004FF120_v122448 __asm__("t4vect_Zero");

struct sVEntryI_122448 { short delta; short index; void (*fn)(void*, void*, int); };
struct sVEntryF_122448 { short delta; short index; void (*fn)(void*, float, void*); };

/**
 * Draws the rider's model when it is on screen, then updates the low-detail flag unk_B1C.
 *
 * Picks mLOD (func_00121F30), loads the rider's lights (graphics slot 69 with unk_7C0), sets bits 2-6 of the
 * draw state's word 1 to 5 and draws mAnimModel between two graphics slot-27 calls (with mPosition, then the
 * zero vector). The board and player additive flags come from the handler's cPlayerLights, or uber level 11
 * and up. When the view manager's word +0x14 is 0 or unk_B1C is set, unk_B1C becomes 1 if the rider is off
 * screen or at LOD 2.
 */
extern "C" void cRider_renderBody(cRider* self)
{
    if (self->mIsOnScreen)
    {
        char* handler = (char*)self->mRiderHandler;
        char* playerLights = handler ? handler + 0xD30 : 0;
        int additivePlayer = 0;
        if (*(int*)(playerLights + 0xA0) || self->mUberLevel >= 11)
            additivePlayer = 1;
        int additiveBoard = 0;
        if (*(int*)(playerLights + 0xA4) || self->mUberLevel >= 11)
            additiveBoard = 1;
        func_00121F30(self);
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEntryI_122448* vt = (sVEntryI_122448*)ps2GraphicsMan->mVtbl;
        vt[69].fn((char*)ps2GraphicsMan + vt[69].delta, self->unk_7C0, 1);
        cPS2GraphicsMan* graphicsMan = GrMan;
        int* rs = (int*)graphicsMan->m_pCurrentDrawState;
        rs[1] = (rs[1] & ~0x7C) | 0x14;
        sVEntryF_122448* vt2 = (sVEntryF_122448*)graphicsMan->mVtbl;
        vt2[27].fn((char*)graphicsMan + vt2[27].delta, 0.0f, &self->mPosition);
        cAnimModel_render(self->mAnimModel, self->mLOD, additiveBoard, additivePlayer);
        cPS2GraphicsMan* grManReset = GrMan;
        sVEntryF_122448* vt3 = (sVEntryF_122448*)grManReset->mVtbl;
        vt3[27].fn((char*)grManReset + vt3[27].delta, 0.0f, &D_004FF120_v122448);
    }
    if (*(int*)((char*)SSXApp->Game->ViewMan + 0x14) == 0 || self->unk_B1C)
    {
        int* lowDetailFlag = &self->unk_B1C;
        int lowDetail = 0;
        if (self->mIsOnScreen == 0 || self->mLOD == 2)
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
extern "C" int cRider_renderLights(cRider* self)
{
    if (self->mIsOnScreen != 0) {
        cRider_SetRiderLights(self);
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
extern "C" void cRider_renderShadow(cRider* self)
{
    if (self->mIsOnScreen != 0)
    {
        cRider_RenderShadow(self);
    }
}
#endif

extern "C" void* func_002F2088(void* obj);

//100%
INCLUDE_ASM("ai/rider", cRider_renderFX);
#ifdef SKIP_ASM
extern "C" void cRiderHandler_renderRiderFX(int);

/**
 * Renders the rider's handler FX: cRiderHandler_renderRiderFX on mRiderHandler. Nothing in the source calls it.
 *
 * @param rider  the cRider
 */
extern "C" void cRider_renderFX(cRider* rider) {
    cRiderHandler_renderRiderFX(((int)rider->mRiderHandler));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122638__FPv);
#ifdef SKIP_ASM
/**
 * Renders the rider's attached emitter FX: func_002F2088 on mAttachedEmitterFX.
 *
 * cAI_render calls it for every rider last.
 *
 * @param vself  the cRider
 * @return func_002F2088's result
 */
void* func_00122638(void* vself)
{
    cRider* self = (cRider*)vself;
    return func_002F2088(self->mAttachedEmitterFX);
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
extern "C" void cRider_flushActiveLights(cRider* self)
{
    MEM_fill(self->unk_794, 0, sizeof(self->unk_794));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122680);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: an empty ASSERT macro (`ASSERT_2680(SSXApp != 0)`, `do { } while (0)`) at entry, an unused `int unused[4]` (frame +0x10), and the (0, 0) store to o->f28 through the layout-identical sV2v_2680 whose by-value ctor stores y before x; plain form 99.17% (99.25% / 99.94% / 99.98% without each alone). Byte-identical and behaves like the plain form; landed marked under
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



struct sV2_2680 {
    float x, y;
    sV2_2680(const float& ax, const float& ay) : x(ax), y(ay) {}
};

extern sGsState_2680 sDrawState_s_defaultDrawState;
extern char t4mat_Identity[];
extern int cRenderToggles_m_DisableHUD;
// Release build: the assert compiles to nothing (gcc still emits its loop notes).
#define ASSERT_2680(cond) do { } while (0)


// PORT: second vec2 temp is built by a by-value ctor.
struct sV2v_2680 {
    float x, y;
    sV2v_2680(float ax, float ay) { y = ay; x = ax; }
};

struct sOverlay_2680 {
    char pad00[0x28];
    sV2_2680 f28;
    sV2_2680 f30;
    sV2_2680 f38;
};

/**
 * Opens and closes a 640x480 2D pass with the default draw state, drawing nothing, unless the HUD is disabled.
 *
 * Unless cRenderToggles_m_DisableHUD is set: copies the DebugFont object's +0x30 pair to +0x38, calls GrMan's
 * PushViewport and PushView, pushes sDrawState_s_defaultDrawState onto the draw-state stack, sets the viewport and
 * texture, two draw-state fields and the DebugFont's +0x28 pair (2, 2, then 0, 0), and ends with PopView, PopViewport and a
 * stack pop. Nothing in the source calls it.
 */
extern "C" void func_00122680(void)
{
    ASSERT_2680(SSXApp != 0);
    if (cRenderToggles_m_DisableHUD == 0) {
        sOverlay_2680* debugFont = (sOverlay_2680*)SSXApp->Game->DebugFont;
        debugFont->f38 = debugFont->f30;
        ((cGraphicsMan_2680*)GrMan)->PushViewport();
        ((cGraphicsMan_2680*)GrMan)->PushView();
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        ((cGraphicsMan_2680*)ps2GraphicsMan)->stateTop[1] = ((cGraphicsMan_2680*)ps2GraphicsMan)->stateTop[0];
        ((cGraphicsMan_2680*)ps2GraphicsMan)->stateTop++;
        *((cGraphicsMan_2680*)GrMan)->stateTop = sDrawState_s_defaultDrawState;
        *(short*)((char*)GrMan->m_pCurrentDrawState + 0x10) = -1;
        ((cGraphicsMan_2680*)GrMan)->SetViewportOrthographic(0.0f, 0.0f, 640.0f, 480.0f, 0, 0, 0.0f, 1.0f);
        ((cGraphicsMan_2680*)GrMan)->SetView(t4mat_Identity);
        ((cGraphicsMan_2680*)GrMan)->stateTop->f8_5 = 24;
        ((cGraphicsMan_2680*)GrMan)->stateTop->f4_2 = 5;
        debugFont->f28 = sV2_2680(2.0f, 2.0f);
        *(sV2v_2680*)&debugFont->f28 = sV2v_2680(0.0f, 0.0f);
        ((cGraphicsMan_2680*)GrMan)->PopView();
        ((cGraphicsMan_2680*)GrMan)->PopViewport();
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

struct sV3_2898 {
    float x, y, z;
};

struct sMat_2898 {
    float m[4][4];
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
    sMat_2898 mat;                  // 0x110
    sV4_2898 f150;                  // 0x150
    sV4_2898 f160;                  // 0x160
    void* f170;                     // 0x170
};

struct sVE0_2898 { short delta; short index; void (*fn)(void*); };
struct sVEI_2898 { short delta; short index; int (*fn)(void*); };
struct sVEI3_2898 { short delta; short index; int (*fn)(void*, void*, const sV4_2898&); };
struct sVE2_2898 { short delta; short index; void (*fn)(void*, void*, int); };
struct sVE3_2898 { short delta; short index; void (*fn)(void*, void*, void*); };
struct sVE4_2898 { short delta; short index; void (*fn)(void*, void*, void*, void*); };

extern int D_004A4C00;
extern sV3_2898 D_004A5E40;

extern "C" void func_00374D00(void* obj, sV3_2898* a, sV3_2898* b, sV3_2898* dir, sMat_2898* bones, int f);
extern "C" void cAnimModel_rendershadow(void* model, int lod, sV4_2898* a, sV4_2898* b);
extern "C" void func_00375890(void* obj);
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
static inline void CopyMat_2898(sMat_2898* d, sMat_2898* s)
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

static inline void Set3_2898(sV3_2898& v, float x, float y, float z)
{
    v.x = x;
    v.y = y;
    v.z = z;
}

static inline cAnimModel* Model_2898(cRider* rider) { return rider->mAnimModel; }
static inline sObj_2898* Shadow_2898(cRider* rider) { return (sObj_2898*)rider->mShadow; }
// Translation row of bone i's 0x40-byte matrix.
static inline sV4_2898 BonePos_2898(cRider* rider, int i) { return *(sV4_2898*)((char*)Model_2898(rider)->unk_030 + (i << 6) + 0x30); }

static inline void* Tex_2898(sObj_2898* o) { return o->f170; }

static inline sRS_2898*& top_2898(void* g) { return *(sRS_2898**)((char*)g + 0xE84); }

/**
 * Renders the rider's projected shadow texture once per frame, and draws the shadow.
 *
 * On the frame's first call (unk_AA4 clear): pushes a draw state, sets up the projection (func_00374D00)
 * centred between bones unk_918 and unk_8E8, lit straight down, with the rider's up axis, renders the model's
 * shadow pass (cAnimModel_rendershadow at LOD 3; board colour grey 0.3, black on the ground; shadow colour grey 0.3)
 * through graphics slots 31, 53, 55, 101, 54 and 32, and pops the state. The shadow itself is drawn on every
 * call (func_00375890).
 */
extern "C" void cRider_RenderShadow(cRider* self)
{
    if (self->unk_AA4 == 0) {
        self->unk_AA4 = 1;
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        top_2898(ps2GraphicsMan)[1] = top_2898(ps2GraphicsMan)[0];
        top_2898(ps2GraphicsMan)++;
        sV3_2898 center;
        {
            sV4_2898 midpoint = Scale_2898(Add_2898(BonePos_2898(self, self->unk_918), BonePos_2898(self, self->unk_8E8)), 0.5f);
            center.x = midpoint.x;
            center.y = midpoint.y;
            center.z = midpoint.z;
        }
        if (D_004A4C00 == 0) {
            Set3_2898(D_004A5E40, 0.0f, 0.0f, -1.0f);
            D_004A4C00 = 1;
        }
        sV3_2898 objUp;
        {
            sV4_2898 upRow = *(sV4_2898*)&self->mMatrix.row[2];
            objUp.x = upRow.x;
            objUp.y = upRow.y;
            objUp.z = upRow.z;
        }
        float k = 0.3f;
        func_00374D00(Shadow_2898(self), &center, &objUp, &D_004A5E40, (sMat_2898*)Model_2898(self)->unk_030, 0);
        cPS2GraphicsMan* grManPushView = GrMan;
        top_2898(grManPushView)->f8 = (top_2898(grManPushView)->f8 & ~0x3E0) | 0x20;
        top_2898(grManPushView)->f4 = (top_2898(grManPushView)->f4 & 0xFE7FFFFF) | 0x1000000;
        top_2898(grManPushView)->f4 = top_2898(grManPushView)->f4 & 0xFFCFFFFF;
        top_2898(grManPushView)->f4 = (top_2898(grManPushView)->f4 & 0xFFF00FFF) | 0x14000;
        top_2898(grManPushView)->f4 = (top_2898(grManPushView)->f4 & 0xFFBFFFFF) | 0x400000;
        top_2898(grManPushView)->f4 = (top_2898(grManPushView)->f4 & ~0x7C) | 4;
        sVE0_2898* vt = (sVE0_2898*)grManPushView->mVtbl;
        vt[31].fn((char*)grManPushView + vt[31].delta);
        cPS2GraphicsMan* grManLockTex = GrMan;
        sVE2_2898* vt3 = (sVE2_2898*)grManLockTex->mVtbl;
        vt3[53].fn((char*)grManLockTex + vt3[53].delta, Shadow_2898(self)->f170, 1);
        cPS2GraphicsMan* grManClearTex = GrMan;
        sVEI3_2898* vt4 = (sVEI3_2898*)grManClearTex->mVtbl;
        vt4[55].fn((char*)grManClearTex + vt4[55].delta, Tex_2898(Shadow_2898(self)), sV4_2898(0.0f, 0.0f, 0.0f, 0.0f));
        if (cRider_getMotionState(self) == 0) {
            k = 0.0f;
        }
        sMat_2898 mat;
        CopyMat_2898(&mat, &Shadow_2898(self)->mat);
        sV4_2898 tuvScale = Shadow_2898(self)->f150;
        sV4_2898 tuvTrans = Shadow_2898(self)->f160;
        cPS2GraphicsMan* grManTuvBatch = GrMan;
        sVE4_2898* vt5 = (sVE4_2898*)grManTuvBatch->mVtbl;
        vt5[101].fn((char*)grManTuvBatch + vt5[101].delta, &mat, &tuvScale, &tuvTrans);
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
        cAnimModel_rendershadow(Model_2898(self), 3, &boardColour, &shadowColour);
        cPS2GraphicsMan* grManRestoreLock = GrMan;
        sVEI_2898* vt6 = (sVEI_2898*)grManRestoreLock->mVtbl;
        vt6[54].fn((char*)grManRestoreLock + vt6[54].delta);
        cPS2GraphicsMan* grManPopView = GrMan;
        sVE0_2898* vt7 = (sVE0_2898*)grManPopView->mVtbl;
        vt7[32].fn((char*)grManPopView + vt7[32].delta);
        cPS2GraphicsMan* grManPopState = GrMan;
        top_2898(grManPopState)--;
    }
    func_00375890(Shadow_2898(self));
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
 * @return 1 when mSmoothedHeading >= 0.925 or < -0.925, otherwise 0
 */
extern "C" int cRider_isInVertZone(cRider* self)
{
    float heading = self->mSmoothedHeading;
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
void* cRider_isVertZoneApplicable(void* self)
{
    return (void*)cRider_isGroundNormalValid((cRider*)self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateDangerLevel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00101A10(void* self, int character);
extern "C" int cRider_isInVertZone(cRider* self);
float cRiderMetrix_evDangerLevelChange_2(void* self, int state) __asm__("cRiderMetrix_evDangerLevelChange__FPv");

/**
 * Updates the rider's danger level (unk_5B0) from the AI's danger manager.
 *
 * Takes the manager's level for mAIIndex (func_00101A10); level 0 becomes 1 when the rider is on the ground
 * in a vert zone. A change goes to cRiderMetrix_evDangerLevelChange on mMetrix, an empty stub in retail.
 */
extern "C" void cRider_updateDangerLevel(cRider* self)
{
    int old = self->unk_5B0;
    int dangerLevel = func_00101A10(SSXApp->Game->AI->DangerMan, self->mAIIndex);
    self->unk_5B0 = dangerLevel;
    if (dangerLevel == 0)
    {
        if (cRider_isVertZoneApplicable(self) != 0)
        {
            if (cRider_isInVertZone(self))
                self->unk_5B0 = 1;
        }
    }
    int cur = self->unk_5B0;
    if (old != cur)
        cRiderMetrix_evDangerLevelChange_2(self->mMetrix, cur);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_estimateFinishTick);
#ifdef SKIP_ASM
float cRider_getTotalPathDistance(void* self);

struct sVEntry00122D78 { short delta; short index; int (*fn)(void*); };

/**
 * Estimates the race-clock tick at which the rider will finish.
 *
 * The speed is the distance covered (cRider_getTotalPathDistance's float at +0x4D8, probably the course length, minus
 * mDistToFinish) over the race clock (AI EventTimerTick), but at least 30 minus the AI index per tick.
 *
 * @return the race clock plus mDistToFinish over that speed
 */
extern "C" int cRider_estimateFinishTick(cRider* self)
{
    char* sub = (char*)&self->mVtbl;
    int n = SSXApp->Game->AI->EventTimerTick;
    float rate = (cRider_getTotalPathDistance(self) - self->mDistToFinish) / (float)n;
    sVEntry00122D78* vt = *(sVEntry00122D78**)sub;
    if (rate < 30.0f - (float)vt[kRiderVt_getAIIndex].fn(sub + vt[kRiderVt_getAIIndex].delta))
    {
        sVEntry00122D78* vt2 = *(sVEntry00122D78**)sub;
        rate = 30.0f - (float)vt2[kRiderVt_getAIIndex].fn(sub + vt2[kRiderVt_getAIIndex].delta);
    }
    return n + (int)(self->mDistToFinish / rate);
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
extern "C" int cRider_estimateFinishScore(cRider* self)
{
    int n = *(int*)((char*)self->mMetrix + 0x198) + 1;
    float distCovered = cRider_getTotalPathDistance(self) - self->mDistToFinish;
    if (distCovered < 1.0f)
        distCovered = 1.0f;
    float scorePerDist = (float)n / distCovered;
    float distLeft = self->mDistToFinish - 1000.0f;
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
extern "C" int func_001446A0(void* self, int a1);

/**
 * Answers a mission query about the rider.
 *
 * @p query 0 wipeout (motion state 2), 1 reset (control state 9), 2 air (motion 1, or 2 with the handler's +0x30 word
 * 1), 3 ground, 4 railslide, 5-7 grab / tweak / uber held during a spin (cSpinControl_isGrabHeld, isTweakHeld,
 * isUberHeld on the handler's spin control at +0x230), 8 the metrix's points (+0x198), 9 / 10 PressMode 1 / 2, 11
 * boosting (mBoostStrength non-zero), 15-17 the metrix's +0x138, +0x13C and +0x19C, 18 a layer-1 anim of category 13
 * with event flag 0 set and flag 1 clear. 12-14 and others return 0. Nothing in the source calls it.
 *
 * @param query  the query number
 * @return the answer (a flag or a count)
 */
extern "C" int cRider_getMissionInt(cRider* self, int query)
{
    int res;
    switch (query) {
    case 0:
        return cRider_getMotionState((char*)self) == 2;
    case 1:
        return cRider_getControlState((char*)self) == 9;
    case 2:
        {
            bool ok = false;
            if (cRider_getMotionState((char*)self) == 1 || (cRider_getMotionState((char*)self) == 2 && *(int*)((char*)self->mRiderHandler + 0x30) == 1)) {
                ok = true;
            }
            return ok;
        }
    case 3:
        return cRider_getMotionState((char*)self) == 0;
    case 4:
        return cRider_getMotionState((char*)self) == 4;
    case 5:
        res = 0;
        if (cRider_getControlState((char*)self) == 5) {
            res = cSpinControl_isGrabHeld((char*)self->mRiderHandler + 0x230) != 0;
        }
        return res;
    case 6:
        res = 0;
        if (cRider_getControlState((char*)self) == 5) {
            res = cSpinControl_isTweakHeld((char*)self->mRiderHandler + 0x230) != 0;
        }
        return res;
    case 7:
        res = 0;
        if (cRider_getControlState((char*)self) == 5) {
            res = cSpinControl_isUberHeld((char*)self->mRiderHandler + 0x230) != 0;
        }
        return res;
    case 9:
        return self->PressMode == 1;
    case 10:
        return self->PressMode == 2;
    case 11:
        return self->mBoostStrength != 0.0f;
    case 8:
        return *(int*)((char*)self->mMetrix + 0x198);
    case 15:
        return *(int*)((char*)self->mMetrix + 0x138);
    case 16:
        return *(int*)((char*)self->mMetrix + 0x13C);
    case 17:
        return *(int*)((char*)self->mMetrix + 0x19C);
    case 18:
        res = 0;
        if (cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 1) == 13) {
            if (func_001446A0(cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0xB0, 0)) {
                res = !func_001446A0(cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0xB0, 1);
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
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Returns the rider's character id from the BE player interface (cBENewPlayerInterface_getRiderCharID).
 *
 * @return the character id for mAIIndex
 */
extern "C" int cRider_getIdentity(cRider* self)
{
    return cBENewPlayerInterface_getRiderCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mAIIndex);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getCheatIdentity);
#ifdef SKIP_ASM
extern "C" int cBENewPlayerInterface_getRiderCheatCharID(void* iface, int riderIndex);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Returns the cheat character chosen for this rider's player.
 *
 * Asks the new-player interface (cBE interface kind 1) through cBENewPlayerInterface_getRiderCheatCharID for the rider's mAIIndex; game mode code
 * stores the same value in iCheatCharacters.
 *
 * @return the cheat character id for the rider's player
 */
extern "C" int cRider_getCheatIdentity(cRider* self)
{
    return cBENewPlayerInterface_getRiderCheatCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mAIIndex);
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
extern "C" int cRider_isGroundNormalValid(cRider* self)
{
    int grounded = 0;

    if (cRider_getMotionState(self) == 0 ||
        (cRider_getMotionState(self) == 2 && *(int*)((char*)self->mRiderHandler + 0x30) == 0)) {
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
extern "C" void cReplay_eventReset(int a, int b);
extern "C" void cRider_cancelRiderTransitions(cRider* self);
extern "C" void cRiderHandler_resetRiderFX(void* self);
extern "C" void func_002F1A00(void* a);
extern "C" void cRiderMetrix_evReset(void* metrix, int flag);
void cRider_setControlState_impl(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

void cRider_putAt_3210(void* self, sVec4* a, sVec4* b, int n, float t) __asm__("cRider_putAt");

// PORT: PS2-only VU0 inline asm (4x4 matrix times vector).
static inline sVec4 MatMulV_3210(const char* m, const t4Vector& v)
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

extern "C" void cRider_transport_v(cRider* self, char* m) __asm__("cRider_transport");

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
extern "C" void cRider_transport_v(cRider* self, char* mat)
{
    cReplay_eventReset((int)SSXApp->Game->Replay, self->mAIIndex);
    sVec4 pos = *(sVec4*)(mat + 0x30);
    sVec4 off(0.0f, 0.0f, 0.0f, 0.0f);
    int i = self->mAIIndex;
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
    cRiderMetrix_evReset(self->mMetrix, 1);
    cRider_setMotionState_impl(self, 0);
    cRider_setControlState_impl(self, 0);
    cRider_cancelRiderTransitions(self);
    char* sub = (char*)&self->mVtbl;
    sVEntry<int(void*)>* vtA = *(sVEntry<int(void*)>**)sub;
    char* obj = *(char**)(*(char**)((char*)SSXApp->Game->ViewMan + 0x4) + 0xA0);
    sVEntry<void(void*, int)>* vtB = *(sVEntry<void(void*, int)>**)(obj + 0x14);
    char* objB = obj + vtB[3].delta;
    vtB[3].fn(objB, vtA[kRiderVt_getAIIndex].fn(sub + vtA[kRiderVt_getAIIndex].delta));
    float len = Length_3210(*(sVec4*)&self->mVelocity);
    SetPath_3210(self, &pos, &dir);
    self->mVelocity = Scale_3210(sVec4(*(sVec4*)&self->mMatrix.row[1]), len);
    cRiderHandler_resetRiderFX(self->mRiderHandler);
    func_002F1A00(self->mAttachedEmitterFX);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cRider_initForMission);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.

extern t4Vector t4vect_XAxis;
extern "C" void cReplay_eventReset(int a, int b);
extern "C" void cRider_cancelRiderTransitions(cRider* self);
extern "C" void func_002F1A00(void* a);
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
extern "C" void cRider_initForMission(cRider* self, char* mat)
{
    t4Vector pos;
    t4Vector dir;
    cReplay_eventReset((int)SSXApp->Game->Replay, self->mAIIndex);
    pos = *(t4Vector*)(mat + 0x30);
    dir = MatMulV_34D0(mat, t4vect_XAxis);
    cRiderMetrix_evReset(self->mMetrix, 1);
    cRider_setMotionState_impl(self, 3);
    cRider_setControlState_impl(self, 13);
    cRider_putAt_34D0(self, &pos, &dir, 5, 0.0f);
    *(int*)((char*)self->mRiderHandler + 0x290) = 2;
    cRider_setControlState_impl(self, 6);
    cRider_cancelRiderTransitions(self);
    cRiderHandler_resetRiderFX(self->mRiderHandler);
    func_002F1A00(self->mAttachedEmitterFX);
    if ((unsigned int)self->mViewIndex < 2) {
        char* obj = *(char**)(*(char**)((char*)SSXApp->Game->ViewMan + (self->mViewIndex << 2) + 4) + 0xA8);
        sVEntry<void(void*)>* vt = *(sVEntry<void(void*)>**)(obj + 0x14);
        vt[4].fn(obj + vt[4].delta);
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
extern "C" void cRider_resetForMission(cRider* self)
{
    *(int*)((char*)self->mRiderHandler + 0x350) = 1;
    cRider_setControlState_impl(self, 9);
    cRider_setMotionState_impl(self, 3);
    *(int*)((char*)self->mRiderHandler + 0x290) = 0;
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
int GetSSXActorHeader(void* self);
extern "C" void GetSSXActorFrame(int, void*);
struct sQuat_001241C0;
extern "C" void cRider_EulerToQuat(cRider* self, sQuat_001241C0* out, float rx, float ry, float rz);
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cRider_updateOrientationImplicit(cRider* self);
extern "C" void cRiderHandler_resetRiderFX(void* self);
extern "C" void func_0030EA80(void* model, int i, int channel);
extern "C" void func_0030E9E0(void* model, int i, int channel);
void* func_0014BDB8_3640() __asm__("func_0014BDB8__FPv");
extern "C" void* func_0014AD50(void* iface, int id);
int func_0014D988(void* tbl, int g);
extern "C" void* func_0014D998(void* tbl, int g);
extern "C" int cRider_getIdentity(cRider* self);
extern "C" void cRider_setupBoostWarp(void* rider, int a, int b);

struct sV_3640
{
    float x, y, z, w;
    sV_3640() {}
    sV_3640(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern sV_3640 D_004FF120_3640 __asm__("t4vect_Zero");
extern sV_3640 D_004FF160_3640 __asm__("t4vect_ZAxis");

static inline sV_3640 AxisAngle_3640(const sV_3640& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sV_3640(s * axis.x, s * axis.y, s * axis.z, c);
}

// PORT: PS2-only VU0 macro-mode asm (4x4 matrix times vector).
// PORT: g++ named-return-value extension.
static inline sV_3640 MatMulV_3640(const char* m, const sV_3640& v) return r
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
static inline sV_3640 QuatMul_3640(const sV_3640& a, const sV_3640& b)
{
    sV_3640 r;
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

struct sTrack_3640
{
    float cur;
    float rate;
    float target;
};

static inline void initTrack_3640(cRider* self, int off, float v, float z)
{
    *(float*)((char*)self + off + 8) = v;
    *(float*)((char*)self + off) = v;
    *(float*)((char*)self + off + 4) = z;
}

struct sItem_3640 {
    short a;
    unsigned short flags;
};

struct sDb_3640 {
    char pad0[0x288];
    short* map;                 // 0x288
    char pad28C[0x4];
    sItem_3640 items[1];        // 0x290
};

struct sEnt_3640 {
    char pad0[0x4];
    short item;                 // 0x04
    char pad6[0xA];
    signed char id;             // 0x10
    char pad11[0x23];
    int flags;                  // 0x34
};

static inline sItem_3640* lookup_3640(sDb_3640* d, int i)
{
    short k = d->map[i];
    if (k >= 0) {
        return &d->items[k];
    }
    return 0;
}

/**
 * Starts a scripted sequence (NIS) for the rider: leaves normal control and poses the rider from the script.
 *
 * Sets CS_Void and MS_Static, plays anim 5, sets mIsInNIS and clears the event flag and sound ids. Byte 7 of
 * the script record (GetSSXActorHeader) sets unk_AFC (probe the ground at the board root), byte 4 sets unk_AD0
 * (rider not drawn). The entity's channels give the start position, put through mScriptMatrix into mPosition,
 * and three angles in degrees that set mRotation.
 *
 * Velocity and speed are zeroed, the FX and the 19 slewed channels reset (channel 9 to 1), parts 5, 6, 8 and
 * 9 enabled and 4 and 7 disabled at LOD 0; when byte 6 is set, the rider's items with flag 0x10 are disabled
 * at every LOD. Ends any boost (cRider_setupBoostWarp(self, 0, 0)).
 */
extern "C" void cRider_OnStartObjectUse(cRider* self)
{
    int buf[12];
    cScriptObjRider_OnStartObjectUse(&self->mScriptObjRiderVtbl);
    cRider_setControlState_impl(self, 13);
    cRider_setMotionState_impl(self, 3);
    cRiderAnimBase_reset(self->mRiderAnim);
    cRiderAnimBase_play(self->mRiderAnim, 5, 0, -1.0f);
    self->mIsInNIS = 1;
    self->unk_2D8 = self->IsRidingFakie;
    self->unk_AC8 = 0;
    int j;
    for (j = 3; j >= 0; j--)
        self->unk_B08[j] = -1;
    float zeroF;
    float one = 1.0f;
    // PORT: GetSSXActorHeader returns a record pointer as int
    char* rec = (char*)GetSSXActorHeader(self->unk_6D8);
    zeroF = 0.0f;
    self->unk_AFC = rec[7] != 0;
    self->unk_B00 = 1;
    self->unk_AD0 = rec[4] != 0;
    GetSSXActorFrame((int)self->unk_6D8, buf);
    float* fb = (float*)buf;
    sV_3640 poseVec;
    poseVec.x = fb[6];
    poseVec.y = fb[7];
    poseVec.z = fb[8];
    poseVec.w = one;
    self->unk_AF0 = fb[9] * 0.01745329424738884f;
    self->unk_AF4 = fb[10] * 0.01745329424738884f;
    self->unk_AF8 = fb[11] * 0.01745329424738884f;
    *(sV_3640*)&self->unk_AE0 = poseVec;
    poseVec = MatMulV_3640((char*)self + 0x700, *(sV_3640*)&self->unk_AE0);
    *(sV_3640*)&self->mPosition = poseVec;
    float ry = self->unk_744;
    float rz = self->unk_740;
    cRider_EulerToQuat(self, (sQuat_001241C0*)&poseVec, zeroF,
                  self->unk_AF4 + ry,
                  self->unk_AF0 + rz);
    float rx = self->unk_748;
    *(sV_3640*)&self->mRotation = AxisAngle_3640(D_004FF160_3640, self->unk_AF8 + rx - 1.5707963705062866f);
    *(sV_3640*)&self->mRotation = QuatMul_3640(poseVec, *(sV_3640*)&self->mRotation);
    cRider_updateOrientationImplicit(self);
    *(sV_3640*)&self->mVelocity = D_004FF120_3640;
    self->unk_B04 = zeroF;
    cRiderHandler_resetRiderFX(self->mRiderHandler);
    initTrack_3640(self, 0x1F0, zeroF, zeroF);
    initTrack_3640(self, 0x1FC, zeroF, zeroF);
    initTrack_3640(self, 520, zeroF, zeroF);
    initTrack_3640(self, 532, zeroF, zeroF);
    initTrack_3640(self, 544, zeroF, zeroF);
    initTrack_3640(self, 556, zeroF, zeroF);
    initTrack_3640(self, 568, zeroF, zeroF);
    initTrack_3640(self, 580, zeroF, zeroF);
    initTrack_3640(self, 592, zeroF, zeroF);
    initTrack_3640(self, 604, one, zeroF);
    initTrack_3640(self, 616, zeroF, zeroF);
    initTrack_3640(self, 628, zeroF, zeroF);
    initTrack_3640(self, 640, zeroF, zeroF);
    initTrack_3640(self, 652, zeroF, zeroF);
    initTrack_3640(self, 664, zeroF, zeroF);
    initTrack_3640(self, 676, zeroF, zeroF);
    initTrack_3640(self, 688, zeroF, zeroF);
    initTrack_3640(self, 700, zeroF, zeroF);
    initTrack_3640(self, 712, zeroF, zeroF);
    func_0030E9E0(self->mAnimModel, 5, 0);
    func_0030E9E0(self->mAnimModel, 6, 0);
    func_0030E9E0(self->mAnimModel, 8, 0);
    func_0030E9E0(self->mAnimModel, 9, 0);
    func_0030EA80(self->mAnimModel, 4, 0);
    func_0030EA80(self->mAnimModel, 7, 0);
    if (rec[6] != 0) {
        void* iface = cBE_getInterface(cBE_getBE(), 9);
        void* tbl = func_0014BDB8_3640();
        sDb_3640* db = (sDb_3640*)func_0014AD50(iface, self->mAIIndex);
        int identity = cRider_getIdentity(self);
        int n = func_0014D988(tbl, identity);
        sEnt_3640* entry = (sEnt_3640*)func_0014D998(tbl, identity);
        int i;
        for (i = 0; i < n; i++, entry++) {
            int id = entry->id;
            if (id >= 0 && (entry->flags & 0x40)) {
                if (lookup_3640(db, entry->item)->flags & 0x10) {
                    func_0030EA80(self->mAnimModel, id, 0);
                    func_0030EA80(self->mAnimModel, id, 1);
                    func_0030EA80(self->mAnimModel, id, 2);
                    func_0030EA80(self->mAnimModel, id, 3);
                }
            }
        }
    }
    cRider_setupBoostWarp(self, 0, 0);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_OnEndObjectUse);
#ifdef SKIP_ASM
extern "C" void cScriptObjRider_OnEndObjectUse(void* self);
extern "C" void cRider_initAlways(void* self);
extern "C" void* cAudioInterface_bxAudio();
void func_002A1BD0(void* p);
extern "C" void func_0030EA80(void* model, int i, int channel);
extern "C" void func_0030E9E0(void* model, int i, int channel);
int GetSSXActorHeader(void* self);
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
extern "C" void* func_0014AD50(void* iface, int id);
int func_0014D988(void* tbl, int g);
extern "C" void* func_0014D998(void* tbl, int g);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* bank, int id, int a, float f);

struct sItem_123B48 {
    short a;
    unsigned short flags;
};

struct sDb_123B48 {
    char pad0[0x288];
    short* map;                 // 0x288
    char pad28C[0x4];
    sItem_123B48 items[1];      // 0x290
};

struct sEnt_123B48 {
    char pad0[0x4];
    short item;                 // 0x04
    char pad6[0xA];
    signed char id;             // 0x10
    char pad11[0x23];
    int flags;                  // 0x34
};

static inline sItem_123B48* lookup_123B48(sDb_123B48* d, int i)
{
    short k = d->map[i];
    if (k >= 0) {
        return &d->items[k];
    }
    return 0;
}

static inline int entId_123B48(sEnt_123B48* e)
{
    return e->id;
}

static inline int entOn_123B48(sEnt_123B48* e)
{
    return e->flags & 0x40;
}

static inline void* model_123B48(cRider* self)
{
    return self->mAnimModel;
}

static inline void setChan_123B48(cRider* self, int id, int ch)
{
    func_0030E9E0(model_123B48(self), id, ch);
}

static inline int entItem_123B48(sEnt_123B48* e)
{
    return e->item;
}

static inline unsigned short itemFlags_123B48(sDb_123B48* db, sEnt_123B48* e)
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
extern "C" void cRider_OnEndObjectUse(cRider* self)
{
    cScriptObjRider_OnEndObjectUse((char*)&self->mScriptObjRiderVtbl);
    cRider_initAlways(self);
    if (self->unk_AC8 != 0) {
        func_002A1BD0(cAudioInterface_bxAudio());
    }
    self->unk_AFC = 0;
    self->unk_B04 = 0;
    self->mIsInNIS = 0;
    int* ids = self->unk_B08;
    self->unk_AD0 = 0;
    self->unk_B00 = 1;
    func_0030EA80(self->mAnimModel, 5, 0);
    func_0030EA80(self->mAnimModel, 6, 0);
    func_0030EA80(self->mAnimModel, 8, 0);
    func_0030EA80(self->mAnimModel, 9, 0);
    func_0030EA80(self->mAnimModel, 11, 0);
    func_0030E9E0(self->mAnimModel, 4, 0);
    func_0030E9E0(self->mAnimModel, 7, 0);
    // PORT: GetSSXActorHeader returns a record pointer as int
    if (*(signed char*)((char*)GetSSXActorHeader(self->unk_6D8) + 6) != 0) {
        void* iface = cBE_getInterface(cBE_getBE(), 9);
        void* tbl = func_0014BDB8_noarg();
        sDb_123B48* db = (sDb_123B48*)func_0014AD50(iface, self->mAIIndex);
        int identity = cRider_getIdentity(self);
        int n = func_0014D988(tbl, identity);
        sEnt_123B48* entry = (sEnt_123B48*)func_0014D998(tbl, identity);
        int i;
        for (i = 0; i < n; i++, entry++) {
            int id = entId_123B48(entry);
            if (id >= 0 && entOn_123B48(entry)) {
                if (itemFlags_123B48(db, entry) & 0x10) {
                    setChan_123B48(self, id, 0);
                    setChan_123B48(self, id, 1);
                    setChan_123B48(self, id, 2);
                    setChan_123B48(self, id, 3);
                }
            }
        }
    }
    int j;
    for (j = 0; j < 4; j++) {
        if (ids[j] >= 0) {
            cBankMonitor_BANKMONITOR_StopHandle(*(char**)*(char**)((char*)cAudioInterface_bxAudio() + 0x118) + 0x1D8, ids[j], 1, 0.0f);
        }
        ids[j] = -1;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_OnInSeqUpdate);
#ifdef SKIP_ASM
extern "C" void GetSSXActorFrame(int, void*);
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
 * @param id  the script player entity (On Tour: pEntity)
 * @param currFrame  the current frame (the first float argument; On Tour: currFrame)
 * @param frameDelta  the frame step (the second float argument; On Tour: frameDelta)
 * @param type  the entity type (On Tour: type); unused
 * @param flags  event bits, passed on to both updates
 */
extern "C" void cRider_OnInSeqUpdate(cRider* self, int id, float currFrame, float frameDelta, int type, int flags)
{
    int buf[12];
    if (self->mIsInNIS)
    {
        GetSSXActorFrame(id, buf);
        cRider_DoInSeqAnimUpdate(self, buf, currFrame, frameDelta, flags);
        cRider_DoInSeqSoundUpdate(self, buf, currFrame, frameDelta, flags);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_OnInSeqKeyEvent);
#ifdef SKIP_ASM
extern "C" void GetSSXActorFrame(int, void*);
extern "C" void* cAudioInterface_bxAudio();
extern "C" int func_002A1B58(void* self, int msg, int a);
extern "C" int func_002A1B88(void* self, int type, int a, int b);
extern "C" void* cSSXScriptEngine_GetSubjectRider(void* self, int i, int a2);

struct sAnimEvent_123E30
{
    int id;
    signed char* arg;
};

struct sAnimEventInfo_123E30
{
    int pad[4];
    int count;                      // 0x10
    sAnimEvent_123E30* events;      // 0x14
    int pad18[6];
};

/**
 * Handles an in-sequence script's key events: sends its sound events for the rider.
 *
 * Only when @p flags has bit 0x10: reads the entity's channels (GetSSXActorFrame) and, for each event of channel
 * 2 except 0x66, sends ids below 0x64 to the audio system (func_002A1B58) and ids from 0x64 on, less 0x64,
 * with the event's subject rider (cSSXScriptEngine_GetSubjectRider) to func_002A1B88. Each event sent sets
 * unk_AC8.
 *
 * @param id  the script player entity (On Tour: pEntity); its word +8 is passed to the subject lookup
 * @param currFrame  the current frame (the first float argument; On Tour: currFrame); unused
 * @param ignoredFloat  a second float argument that the caller (cScriptPlayerEntity_ExecutePlay) does not set; unused
 * @param type  the entity type (On Tour: type); unused
 * @param flags  event bits; bit 0x10 enables the event handling
 */
extern "C" void cRider_OnInSeqKeyEvent(cRider* self, int id, float currFrame, float ignoredFloat, int type, int flags)
{
    sAnimEventInfo_123E30 info;
    if (flags & 0x10)
    {
        GetSSXActorFrame(id, &info);
        for (int i = 0; i < info.count; i++)
        {
            sAnimEvent_123E30* event = &info.events[i];
            int ev = event->id;
            if (ev == 102) continue;
            if (ev < 100)
            {
                func_002A1B58(cAudioInterface_bxAudio(), ev, (int)self);
                self->unk_AC8 = 1;
            }
            else
            {
                ev -= 100;
                void* snd = cAudioInterface_bxAudio();
                // PORT: id is a handle that is also read as a pointer here.
                int subjectRider = (int)cSSXScriptEngine_GetSubjectRider(ScriptEngine, *event->arg, *(int*)(id + 8));
                func_002A1B88(snd, ev, (int)self, subjectRider);
                self->unk_AC8 = 1;
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
int GetSSXActorHeader(void* self);

struct sV_3F38 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sV_3F38 D_004FF160_3F38 __asm__("t4vect_ZAxis");

struct sRay_3F38 {
    char data[0xB0];
} __attribute__((aligned(16)));

struct sHit_3F38 {
    sV_3F38 pos;
    char pad[0x70];
} __attribute__((aligned(16)));

extern "C" void cWorldLine_cWorldLine_3F38(sRay_3F38* ray, const sV_3F38& a, const sV_3F38& b, int n, float t) __asm__("cWorldLine_cWorldLine");
extern "C" float cBXNash_terrainIntersect1(void* self, sRay_3F38* ray, sHit_3F38* out, int a3);

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sV_3F38 Scale_3F38(const sV_3F38& v, float s)
{
    sV_3F38 r;
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
static inline sV_3F38 Add_3F38(const sV_3F38& a, const sV_3F38& b)
{
    sV_3F38 r;
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
static inline void AddTo_3F38(sV_3F38& a, const sV_3F38& b)
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
 * word +0x1BC: 17-21 (2, 0), 14-16 (2, AI index), others (2, 1); 4 start point (2, AI index), raised by 3000
 * after the snap. A ray from 7000 below to 200 above the point against the Nash grid snaps it to the ground
 * when it hits.
 *
 * @return the end position
 */
extern "C" sV_3F38 cRider_GetNISEndPosition(cRider* self)
{
    if (self->unk_6E4 <= 0)
        return *(sV_3F38*)&self->mPosition;
    float lift = 0.0f;
    // PORT: GetSSXActorHeader returns a record pointer as int
    char* rec = (char*)GetSSXActorHeader(self->unk_6D8);
    sV_3F38 pos = *(sV_3F38*)&self->mPosition;
    switch (*(signed char*)(rec + 5)) {
    case 0:
        break;
    case 1:
        {
            float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 1, self->mAIIndex);
            sV_3F38 startPos;
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
            sV_3F38 startPos;
            startPos.x = startPoint[2];
            startPos.y = startPoint[3];
            startPos.z = startPoint[4];
            startPos.w = 1.0f;
            pos = startPos;
        }
        break;
    case 3:
        switch (*(int*)((char*)SSXApp->Game->SectionMan + 0x1BC)) {
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            {
                float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 2, 0);
                sV_3F38 startPos;
                startPos.x = startPoint[2];
                startPos.y = startPoint[3];
                startPos.z = startPoint[4];
                startPos.w = 1.0f;
                pos = startPos;
            }
            break;
        case 14:
        case 15:
        case 16:
            {
                float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 2, self->mAIIndex);
                sV_3F38 startPos;
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
                sV_3F38 startPos;
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
            sV_3F38 startPos;
            startPos.x = startPoint[2];
            startPos.y = startPoint[3];
            startPos.z = startPoint[4];
            startPos.w = 1.0f;
            pos = startPos;
        }
        lift = 2999.942626953125f;
        break;
    }
    sV_3F38 lo = Add_3F38(pos, Scale_3F38(D_004FF160_3F38, -7000.0f));
    sV_3F38 hi = Add_3F38(pos, Scale_3F38(D_004FF160_3F38, 200.0f));
    sHit_3F38 hit;
    sRay_3F38 ray;
    cWorldLine_cWorldLine_3F38(&ray, lo, hi, 2, 0.9791666865348816f);
    if (cBXNash_terrainIntersect1(SSXApp->Game->BXNash, &ray, &hit, 0) >= 0.0f)
        pos = hit.pos;
    AddTo_3F38(pos, Scale_3F38(D_004FF160_3F38, lift));
    return pos;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_EulerToQuat);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);

struct sQuat_001241C0
{
    float x, y, z, w;
} __attribute__((aligned(16)));

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
extern "C" void cRider_EulerToQuat(cRider* self, sQuat_001241C0* out, float rx, float ry, float rz)
{
    sQuat_001241C0 quat;
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
struct sV_42B0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sV_42B0 D_004FF120_42B0 __asm__("t4vect_Zero");
extern sV_42B0 D_004FF160_42B0 __asm__("t4vect_ZAxis");

struct sRay_42B0 {
    char data[0xB0];
} __attribute__((aligned(16)));

struct sHit_42B0 {
    sV_42B0 pos;        // 0x00
    sV_42B0 nrm;        // 0x10
    sV_42B0 v20;        // 0x20
    char pad30[0x1C];
    int mat;            // 0x4C
    int pad50;
    char* obj;          // 0x54
    char pad58[0x14];
    float f6C;          // 0x6C
    float f70;          // 0x70
    char pad74[0xC];
} __attribute__((aligned(16)));

struct sVE_42B0 { short delta; short index; sV_42B0* (*fn)(void*); };

extern "C" void cWorldLine_cWorldLine(sRay_42B0* ray, const sV_42B0& a, const sV_42B0& b, int n, float t);
extern "C" float cOverlapList_terrainIntersect1(void* coll, sRay_42B0* ray, sHit_42B0* hit, int flags);
void func_00111AA0_42B0(void* self, void* a, void* b, int c, float x) __asm__("func_00111AA0__FPv");

// PORT: PS2-only VU0 inline asm (vector length).
static inline float Length_42B0(const sV_42B0& v)
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
static inline float Dot_42B0(const sV_42B0& a, const sV_42B0& b)
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
static inline sV_42B0 Scale_42B0(const sV_42B0& v, float s)
{
    sV_42B0 r;
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
static inline sV_42B0 Add_42B0(const sV_42B0& a, const sV_42B0& b)
{
    sV_42B0 r;
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
static inline sV_42B0 Sub_42B0(const sV_42B0& a, const sV_42B0& b)
{
    sV_42B0 r;
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
static inline sV_42B0 Cross_42B0(const sV_42B0& a, const sV_42B0& b)
{
    sV_42B0 r;
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
static inline sV_42B0 Normalize_42B0(const sV_42B0& v)
{
    sV_42B0 r;
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

static inline float DotV_42B0(sV_42B0 a, const sV_42B0& b)
{
    return Dot_42B0(a, b);
}


// Non-POD by-value vector (user copy ctor => passed by invisible reference, no callee copy).
struct sVecP_42B0 {
    float x, y, z, w;
    sVecP_42B0(const sV_42B0& v) { *(sV_42B0*)this = v; }
    sVecP_42B0(const sVecP_42B0& v) { *(sV_42B0*)this = *(const sV_42B0*)&v; }
} __attribute__((aligned(16)));

static inline sV_42B0 SubV_42B0(sVecP_42B0 a, const sV_42B0& b)
{
    return Sub_42B0(*(sV_42B0*)&a, b);
}

// PORT: the unit declares cRider_SetNISGroundVectors(void*, void*); this definition binds the cRider* view by asm label.
extern "C" void cRider_SetNISGroundVectors_impl(cRider* self, void* p) __asm__("cRider_SetNISGroundVectors");
/**
 * Probes the ground under a position during an NIS (in-game cutscene) and sets the rider's ground vectors.
 *
 * The probe base is @p pRiderPos moved along mGroundSide by mAnimModel->unk_140.x * mSlewed[0].cur * 45; a line from 100 below
 * to 200 above it (along mGroundNormal) is built with cWorldLine_cWorldLine and intersected with the terrain through
 * mOverlapList (cOverlapList_terrainIntersect1). The speed is the length of the virtual getVelocity, and unk_380 keeps the previous
 * mGroundNormal. A hit object sets mGroundPolyId (object +0x150), unk_AAC / unk_AB0 (the hit's +0x6C / +0x70) and
 * unk_2D4 (the object's half-word +0xA); otherwise mGroundPolyId becomes -1.
 *
 * On a miss (negative distance): unk_B00 = 0, mGroundNormal = the Z axis, unk_3D0 = zero, mSurfaceType and the four
 * audio values 0. On a hit: the first contact (unk_AFC set, unk_B00 clear) is reported to the rider handler
 * (func_00111AA0 with the hit point, its normal, material and the length of its third vector); then unk_B00 = 1,
 * mSurfaceType = the material (0 for -1), mGroundNormal, mGroundContactPoint and unk_3D0 come from the hit, unk_454 =
 * the probe base's height above the hit, mAudioSpeed = speed * 0.00072 (at most 1), mAudioPressure 0.3, mAudioEdging 0,
 * and mAudioBraking = (speed - unk_B04) * 0.0036 (at most 1) when the speed rose since the last call, else 0: the sign
 * looks inverted, but On Tour computes it the same way. Both cases rebuild mGroundForward (row 1 of mMatrix projected
 * onto the ground plane, normalised) and mGroundSide (normal x forward), and store the speed in unk_B04.
 *
 * @param pRiderPos  the position to probe under (mPosition, or the board root bone; see cRider_updateSplit1)
 */
extern "C" void cRider_SetNISGroundVectors_impl(cRider* self, void* pRiderPos)
{
    char* vtObj = (char*)&self->mVtbl;
    sVE_42B0* ve = &(*(sVE_42B0**)vtObj)[kRiderVt_getVelocity];
    float speed = Length_42B0(*ve->fn(vtObj + ve->delta));
    float sideOffset = self->mAnimModel->unk_140.x * (self->mSlewed[0].cur * 45.0f);
    sV_42B0 probeBase = Add_42B0(*(sV_42B0*)pRiderPos, Scale_42B0(*(sV_42B0*)&self->mGroundSide, sideOffset));
    sV_42B0 lo = Add_42B0(probeBase, Scale_42B0(*(sV_42B0*)&self->mGroundNormal, -100.0f));
    sV_42B0 hi = Add_42B0(probeBase, Scale_42B0(*(sV_42B0*)&self->mGroundNormal, 200.0f));
    sHit_42B0 hit;
    sRay_42B0 ray;
    cWorldLine_cWorldLine(&ray, lo, hi, 2, 0.5f);
    float hitDist = cOverlapList_terrainIntersect1(self->mOverlapList, &ray, &hit, 0);
    *(sV_42B0*)&self->unk_380 = *(sV_42B0*)&self->mGroundNormal;
    if (hitDist > 0.0f && hit.obj != 0) {
        self->mGroundPolyId = *(int*)(hit.obj + 0x150);
        self->unk_AAC = hit.f6C;
        self->unk_AB0 = hit.f70;
        self->unk_2D4 = *(unsigned short*)(hit.obj + 0xA);
    } else {
        *(unsigned int*)&self->mGroundPolyId = 0xFFFFFFFF;
    }
    if (hitDist < 0.0f) {
        self->unk_B00 = 0;
        *(sV_42B0*)&self->mGroundNormal = D_004FF160_42B0;
        *(sV_42B0*)&self->unk_3D0 = D_004FF120_42B0;
        *(sV_42B0*)&self->mGroundForward = Normalize_42B0(SubV_42B0(*(sV_42B0*)&self->mMatrix.row[1], Scale_42B0(*(sV_42B0*)&self->mGroundNormal, DotV_42B0(*(sV_42B0*)&self->mMatrix.row[1], *(sV_42B0*)&self->mGroundNormal))));
        *(sV_42B0*)&self->mGroundSide = Cross_42B0(*(sV_42B0*)&self->mGroundNormal, *(sV_42B0*)&self->mGroundForward);
        self->mSurfaceType = 0;
        self->mAudioBraking = 0.0f;
        self->mAudioEdging = 0.0f;
        self->mAudioPressure = 0.0f;
        self->mAudioSpeed = 0.0f;
        self->unk_B04 = speed;
        return;
    }
    if (self->unk_AFC != 0 && self->unk_B00 == 0) {
        func_00111AA0_42B0(self->mRiderHandler, &hit, &hit.nrm, hit.mat, Length_42B0(hit.v20));
    }
    self->unk_B00 = 1;
    self->mSurfaceType = hit.mat;
    if (hit.mat == -1)
        self->mSurfaceType = 0;
    *(sV_42B0*)&self->mGroundNormal = hit.nrm;
    *(sV_42B0*)&self->mGroundContactPoint = hit.pos;
    *(sV_42B0*)&self->unk_3D0 = hit.v20;
    *(sV_42B0*)&self->mGroundForward = Normalize_42B0(SubV_42B0(*(sV_42B0*)&self->mMatrix.row[1], Scale_42B0(*(sV_42B0*)&self->mGroundNormal, DotV_42B0(*(sV_42B0*)&self->mMatrix.row[1], *(sV_42B0*)&self->mGroundNormal))));
    *(sV_42B0*)&self->mGroundSide = Cross_42B0(*(sV_42B0*)&self->mGroundNormal, *(sV_42B0*)&self->mGroundForward);
    self->unk_454 = Dot_42B0(Sub_42B0(probeBase, hit.pos), *(sV_42B0*)&self->mGroundNormal);
    float speedDrop = self->unk_B04 - speed;
    *(int*)&self->mAudioBraking = 0;
    if (speedDrop < self->mAudioBraking) {
        float braking = -speedDrop * 0.0036000001709908247f;
        self->mAudioBraking = braking;
        if (1.0f < braking)
            self->mAudioBraking = 1.0f;
    }
    float speedLevel = speed * 0.0007200000109151006f;
    self->mAudioSpeed = speedLevel;
    if (1.0f < speedLevel)
        self->mAudioSpeed = 1.0f;
    self->unk_B04 = speed;
    self->mAudioPressure = 0.30000001192092896f;
    self->mAudioEdging = 0.0f;
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
extern "C" float func_00115B08(void* self);
extern "C" void cRiderAnimBase_playFromAnimID(void* anim, int id, tAnimID animId, float t);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" void cAnimSequence_setSubAnimTime(void* p, int a1, float t);
extern "C" void cRiderAnimBase_setupAnimation(void* anim, int a1);
extern "C" void cRider_EulerToQuat(cRider* self, sQuat_001241C0* out, float rx, float ry, float rz);
extern "C" void cRider_updateAnimation(cRider* self, float value);
extern "C" void cRider_updateSkeleton(void* self);
extern "C" int func_001446A0(void* self, int a1);
extern "C" int func_001446B8(void* self, int a1);
extern "C" void func_00144670(void* self, int a1);
extern "C" void func_0030E9E0(void* model, int i, int channel);
extern "C" void func_0030EA80(void* model, int i, int channel);
struct sQ_124788;
extern "C" void cRider_updateBoundBox_4788(cRider* self, sQ_124788* p) __asm__("cRider_updateBoundBox");
extern void cRider_updateCollideInfo_4788(cRider* self) __asm__("cRider_updateCollideInfo");

struct sQ_124788
{
    float x, y, z, w;
    sQ_124788() {}
    sQ_124788(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern sQ_124788 D_004FF130_124788 __asm__("t4vect_Origin");

struct sXf_124788
{
    sQ_124788 pos;
    sQ_124788 rot;
};

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sQ_124788 QuatMul_124788(const sQ_124788& a, const sQ_124788& b)
{
    sQ_124788 r;
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
static inline sQ_124788 Sub_124788(const sQ_124788& a, const sQ_124788& b)
{
    sQ_124788 r;
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
static inline sQ_124788 Add_124788(const sQ_124788& a, const sQ_124788& b)
{
    sQ_124788 r;
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
static inline void AddEq_124788(sQ_124788& dst, sQ_124788 b)
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
static inline sQ_124788 Rot_124788(const sQ_124788& q, const sQ_124788& v)
{
    sQ_124788 r;
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
static inline sQ_124788 Scale_124788(const sQ_124788& v, float s)
{
    sQ_124788 r;
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
static inline sQ_124788 MatMulV_124788(const char* m, const sQ_124788& v)
{
    sQ_124788 r;
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

static inline sQ_124788 Conj_124788(const sXf_124788& p)
{
    sQ_124788 r;
    r.w = p.rot.w;
    r.x = p.rot.x;
    r.y = p.rot.y;
    r.z = p.rot.z;
    r.x = -r.x;
    r.y = -r.y;
    r.z = -r.z;
    return r;
}

static inline sXf_124788 Invert_124788(const sXf_124788& a)
{
    sXf_124788 r;
    r.rot = Conj_124788(a);
    r.pos = Add_124788(Scale_124788(Rot_124788(r.rot, Sub_124788(a.pos, D_004FF130_124788)), -1.0f), D_004FF130_124788);
    return r;
}

static inline sXf_124788 Combine_124788(const sXf_124788& a, const sXf_124788& b)
{
    sXf_124788 r;
    r.rot = QuatMul_124788(a.rot, b.rot);
    r.pos = Add_124788(Rot_124788(a.rot, Sub_124788(b.pos, D_004FF130_124788)), a.pos);
    return r;
}

static inline sQ_124788 GetFront_124788(cRider* rider)
{
    return *(sQ_124788*)&rider->mMatrix.row[1];
}

struct sAnimFlags_124788 {
    signed char heading;    // 0x0
    signed char root;       // 0x1
    signed char drift;      // 0x2
};

struct sAnimInfo_124788 {
    int name;                       // 0x00
    int frame;                      // 0x04
    char pad08[0x8];
    float rate;                     // 0x10
    float speed;                    // 0x14
    unsigned int start;             // 0x18
    sAnimFlags_124788* flags;       // 0x1C
};

struct sAnimEvent_124788 {
    int active;                     // 0x00
    sAnimInfo_124788* info;         // 0x04
    char pad08[0x10];
    float pos[3];                   // 0x18
    float ang[3];                   // 0x24 degrees
};

// The unit declares cRider_DoInSeqAnimUpdate(void*, void*, float, float, int); bind the typed definition by asm label.
extern "C" void cRider_DoInSeqAnimUpdate_typed(cRider* self, sAnimEvent_124788* pFrameInfo, float currFrame, float frameDelta, int eventBits) __asm__("cRider_DoInSeqAnimUpdate");
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
extern "C" void cRider_DoInSeqAnimUpdate_typed(cRider* self, sAnimEvent_124788* pFrameInfo, float currFrame, float frameDelta, int eventBits)
{
    if (eventBits & 1) {
        if (pFrameInfo->active == 0) {
            return;
        }
        sAnimInfo_124788* pCurrMainAnim = pFrameInfo->info;
        sAnimFlags_124788* pCurrMainAnimCustom = pCurrMainAnim->flags;
        if ((float)pCurrMainAnim->frame != currFrame) {
            return;
        }
        float startTime = pCurrMainAnim->rate * 0.01666666753590107f;
        int animID;
        int sourceID;
        cScriptObjRider_ScriptAnimIDToGameAnimInfo(&self->mScriptObjRiderVtbl, pCurrMainAnim->name, &animID, &sourceID);
        tAnimID animInfo(sourceID, animID);
        if (pCurrMainAnimCustom->root != 0) {
            char buf[80];
            cRiderAnimBase_getAIAnimFrame(buf, self->mRiderAnim, 432, animInfo, startTime);
            sXf_124788 animPose;
            func_0030ECD8(&animPose, self->mAnimModel, buf, 0);
            cAnimModel* trk = self->mAnimModel;
            sQ_124788* n = (sQ_124788*)((self->mBoneHips << 4) + (int)trk->localPos);
            sXf_124788 node;
            node.pos = sQ_124788(n->x * trk->unk_140.x, n->y * trk->unk_140.y,
                                 n->z * trk->unk_140.z, n->w * trk->unk_140.w);
            node.rot = *(sQ_124788*)((char*)self->mAnimModel->localRot + (self->mBoneHips << 4));
            sXf_124788 y = Combine_124788(node, Invert_124788(animPose));
            sXf_124788* rx = (sXf_124788*)&self->mPosition;
            *rx = Combine_124788(*rx, y);
            cRider_updateOrientationImplicit(self);
            sXf_124788 w = Invert_124788(y);
            cRiderAnimBase_changeOrientationOffset(self->mRiderAnim, &w);
        }
        if (pCurrMainAnimCustom->drift != 0) {
            float kneeOffset = cRider_getStartGateKneeOffset(self);
            self->mSlewed[18].target = kneeOffset;
            self->mSlewed[18].rate = 0;
            self->mSlewed[18].cur = kneeOffset;
            AddEq_124788(*(sQ_124788*)&self->mPosition, Scale_124788(GetFront_124788(self), func_00115B08(self)));
            cRider_updateOrientationImplicit(self);
        }
        int isFlipped = pCurrMainAnimCustom->heading != 0;
        if (isFlipped != self->unk_2D8) {
            float headingOffset = 0.0f;
            if (isFlipped) {
                headingOffset = 3.1415927410125732f;
            }
            cRiderAnimBase_changeHeadingOffset(self->mRiderAnim, headingOffset);
            self->unk_2D8 = isFlipped;
        }
        float start = pCurrMainAnim->start;
        *(float*)((char*)self->mRiderAnim + 0x1C) = pCurrMainAnim->speed;
        cRiderAnimBase_playFromAnimID(self->mRiderAnim, 432, animInfo, start * 0.01666666753590107f);
        cAnimSequence_setSubAnimTime(cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 2), 0, startTime);
        cRiderAnimBase_setupAnimation(self->mRiderAnim, 0);
        *(float*)((char*)self->mRiderAnim + 0x1C) = 1.0f;
    } else if ((eventBits & 2) && pFrameInfo->active == 0) {
        cRiderAnimBase_play(self->mRiderAnim, 5, 0, -1.0f);
    }
    sQ_124788 oldStart = *(sQ_124788*)&self->unk_AE0;
    *(sQ_124788*)&self->unk_AE0 = sQ_124788(pFrameInfo->pos[0], pFrameInfo->pos[1], pFrameInfo->pos[2], 1.0f);
    float prevAngX = self->unk_AF0;
    float prevAngY = self->unk_AF4;
    float prevAngZ = self->unk_AF8;
    sQ_124788 oldPos = *(sQ_124788*)&self->mPosition;
    self->unk_AF0 = pFrameInfo->ang[0] * 0.01745329424738884f;
    self->unk_AF4 = pFrameInfo->ang[1] * 0.01745329424738884f;
    self->unk_AF8 = pFrameInfo->ang[2] * 0.01745329424738884f;
    AddEq_124788(*(sQ_124788*)&self->mPosition,
                 MatMulV_124788((char*)&self->mScriptMatrix, Sub_124788(*(sQ_124788*)&self->unk_AE0, oldStart)));
    sQ_124788 delta = Sub_124788(*(sQ_124788*)&self->mPosition, oldPos);
    *(sQ_124788*)&self->mVelocity = delta;
    cRider_EulerToQuat(self, (sQuat_001241C0*)&delta, self->unk_AF8 - prevAngZ, self->unk_AF4 - prevAngY, self->unk_AF0 - prevAngX);
    *(sQ_124788*)&self->mRotation = QuatMul_124788(delta, *(sQ_124788*)&self->mRotation);
    cRider_updateOrientationImplicit(self);
    sQ_124788* bone = (sQ_124788*)&self->mAnimModel->bones[self->mBoneBoardRoot].pos;
    sQ_124788 before = *bone;
    cRider_updateAnimation(self, frameDelta);
    cRider_updateSkeleton(self);
    if (self->unk_AFC != 0) {
        *(sQ_124788*)&self->mVelocity = Sub_124788(*bone, before);
    }
    if (frameDelta > 0.0f) {
        self->mVelocity.x = self->mVelocity.x / frameDelta;
        self->mVelocity.y = self->mVelocity.y / frameDelta;
        self->mVelocity.z = self->mVelocity.z / frameDelta;
    }
    char* ch = cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 2) + 0xB0;
    int on = 0;
    if (func_001446A0(ch, 0) != 0) {
        on = func_001446B8(ch, 0) != 0;
    }
    if (on) {
        func_0030E9E0(self->mAnimModel, 11, 0);
        func_00144670(ch, 0);
    } else {
        int on2 = 0;
        if (func_001446A0(ch, 1) != 0) {
            on2 = func_001446B8(ch, 1) != 0;
        }
        if (on2) {
            func_0030EA80(self->mAnimModel, 11, 0);
            func_00144670(ch, 1);
        }
    }
    sQ_124788 hipsPos = *(sQ_124788*)((char*)self->mAnimModel->bones + (self->mBoneHips << 5));
    hipsPos.z = self->mPosition.z;
    cRider_updateBoundBox_4788(self, &hipsPos);
    cRider_updateCollideInfo_4788(self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_cancelRiderTransitions);
#ifdef SKIP_ASM
void* cGame_getTransitionMan(void* self, int id);
extern "C" void func_002E4578(void* p);

/**
 * Cancels a camera transition in progress on the rider's view.
 *
 * When the game has a view manager and the rider a view, and Game->TransitionMan[mViewIndex] is in state 1,
 * resets that transition manager with func_002E4578.
 */
extern "C" void cRider_cancelRiderTransitions(cRider* self)
{
    void* w = SSXApp->Game;
    if (*(void**)((char*)w + 0x84) != 0 && self->mViewIndex >= 0)
    {
        if (*(int*)((char*)cGame_getTransitionMan(w, self->mViewIndex) + 0x78) == 1)
            func_002E4578(cGame_getTransitionMan(SSXApp->Game, self->mViewIndex));
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_applyBoost);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_startLaunchIntoAir(void* self, void* a, void* b, float t);

struct sRiderVec4_001250A8
{
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (a += b).
static inline void riderAddEq_001250A8(sRiderVec4_001250A8& a, const sRiderVec4_001250A8& b)
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
extern "C" void cRider_applyBoost(cRider* self, sRiderVec4_001250A8* impulse)
{
    sRiderVec4_001250A8* velocity = (sRiderVec4_001250A8*)&self->mVelocity;
    riderAddEq_001250A8(*velocity, *impulse);
    if (cRider_getMotionState(self) == 1)
        cAirPredictor_startLaunchIntoAir(self->mAirPredictor, &self->mPosition, velocity, 3333.33349609375f);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_doFinishEvent);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_evFinishRace(void* self);
extern "C" void cChaseCameraController_switchCamera_EndRace(void* self);
extern "C" void cGameModeMan_raceFinish(void* self, int a1, int a2);
extern "C" void func_00286EA0(void* self, void* rider);
extern "C" void* cAudioInterface_bxAudio();

struct sVEntry125108 { short delta; short index; int (*fn)(void*); };

static inline void* getC0_125108()
{
    return SSXApp->GameModeMan;
}

/**
 * Records that the rider has finished (or did not finish) and tells the game mode, camera and audio.
 *
 * Online (cGameComm present), a human who finished sets the local or remote finished flag. Calls the metrix
 * finish (cRiderMetrix_evFinishRace), sets mFinishTick to the race clock plus mPenaltyTicks and mFinishTime to at least
 * 0, reports cGameModeMan_raceFinish(AI index, the metrix's stats at +0xFC), switches a split-screen rider's
 * chase camera to the end-race camera and calls func_00286EA0 on the audio system. A rider who did not finish
 * also gets unk_100 cleared.
 */
extern "C" void cRider_doFinishEvent(cRider* self)
{
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0 && self->mDidNotFinish == 0 && self->mIsHuman != 0)
    {
        if (self->mIsLocal != 0)
            gameComm->mLocalFinishedRace = 1;
        else
            gameComm->mRemoteFinishedRace = 1;
    }
    cRiderMetrix_evFinishRace(self->mMetrix);
    self->mFinishTick = SSXApp->Game->AI->EventTimerTick + self->mPenaltyTicks;
    if (self->mFinishTime < 0.0f) self->mFinishTime = 0.0f;
    char* obj = (char*)&self->mVtbl;
    sVEntry125108* vt = *(sVEntry125108**)obj;
    cGameModeMan_raceFinish(getC0_125108(), vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta), (int)self->mMetrix + 0xFC);
    unsigned int k = *(unsigned int*)&self->mViewIndex;
    if (k < 2)
    {
        char* tbl = (char*)SSXApp->Game->ViewMan;
        cChaseCameraController_switchCamera_EndRace(*(void**)(*(char**)(tbl + (k << 2) + 4) + 0xA8));
    }
    func_00286EA0(cAudioInterface_bxAudio(), self);
    if (self->mDidNotFinish == 1) self->unk_100 = 0;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateFinishTest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVE_125228 {
    short delta;
    short index;
    int (*fn)(void*);
};
extern "C" void* cBE_getBE();
void* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cRider_doFinishEvent(cRider* self);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_002A3C00(void* a, void* self, int n);
extern "C" unsigned int func_004139F8(float f);
extern signed char D_00535C10[];
extern int D_005308D0[];

/**
 * Checks whether a human rider has finished, applying a timed event's time limit.
 *
 * For a human who has not finished, outside game mode 4: in a timed event (GameModeMan bIsTimedEvent) with
 * race-rule bit 9 clear, once the race clock passes uiTimeLimit (rounded down to whole seconds) the rider
 * gets mDidNotFinish, its finish event and an audio cue (func_002A3C00). A rider already marked DNF gets its
 * finish event when the game state is 4.
 *
 * @return 1 when the rider has finished (mFinishTime >= 0), otherwise 0
 */
extern "C" int cRider_updateFinishTest(cRider* self)
{
    char* obj = (char*)&self->mVtbl;
    sVE_125228* vt = *(sVE_125228**)obj;
    if (vt[kRiderVt_isHuman].fn(obj + vt[kRiderVt_isHuman].delta)) {
        float finishTime = self->mFinishTime;
        float zeroF = 0.0f;
        int done = finishTime >= zeroF;
        if (!done) {
            cBE_getInterface(cBE_getBE(), 0);
            if (D_00535C10[0] == 4)
                return self->mFinishTime >= zeroF;
            char* app = (char*)SSXApp;
            char* gameModeMan = *(char**)(app + 0xC0);
            if (*(int*)(gameModeMan + 0x88) != 0 && ((D_005308D0[0] >> 9) & 1) == 0) {
                int eventTicks = *(int*)(*(char**)(*(char**)(app + 0x84) + 0xC) + 0xC);
                int secs = func_004139F8((float)*(unsigned int*)(gameModeMan + 0x78) * 0.01666666753590107f);
                if (secs * 60 < eventTicks) {
                    self->mDidNotFinish = 1;
                    cRider_doFinishEvent(self);
                    func_002A3C00(cAudioInterface_bxAudio(), self, 2);
                    goto end;
                }
            }
            if (self->mDidNotFinish == 1 && SSXApp->Game->mCurrentStateID == 4)
                cRider_doFinishEvent(self);
        }
    }
end:
    return self->mFinishTime >= 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_quitEvent);
#ifdef SKIP_ASM
struct sVEntry001253D0 {
    short delta;
    short index;
    int (*fn)(void*);
};

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
extern "C" void cRider_quitEvent(cRider* self)
{
    bool finished = riderHasFinished001253D0(self);
    if (!finished)
    {
        self->mDidNotFinish = 1;
        char* sub = (char*)&self->mVtbl;
        sVEntry001253D0* vt = *(sVEntry001253D0**)sub;
        cGameModeMan_bGiveUp[vt[kRiderVt_getAIIndex].fn(sub + vt[kRiderVt_getAIIndex].delta)] = 1;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_DoInSeqSoundUpdate);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_bxAudio();
extern "C" int cBankMonitor_BANKMONITOR_GetHandle(void* bank, int id, int a);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* bank, int id, int a, float f);
void cScriptObjRider_ScriptSoundIDToGameSoundInfo(void* self, int a1, void* a2, void* a3);
extern "C" int func_00287968(void* self, int a1, int a2);
extern "C" int func_00288A20(void* self, void* rider);
extern "C" int cBankSys_SFX_End(void* self);
extern "C" void* func_00416210(void* dst, int c, int n);

struct sWtEnt_5448 {
    signed char b0;
    char pad1[3];
    int f4[5];
};

struct sWtPair_5448 {
    int a;
    int b;
};

struct sWtV16_5448 {
    int f0;
    int f4;
    int f8;
    int fC;
};

struct sWtSub_5448 {
    int f0;
    sWtEnt_5448 cfg;         // 0x04
    volatile int cur;        // 0x1C
    sWtEnt_5448* ents;       // 0x20
    int arr24[4];            // 0x24
    int* p34;                // 0x34
    int* p38;                // 0x38
    int* p3C;                // 0x3C
    sWtPair_5448 arr40[4];   // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sWtV16_5448* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};

struct sWtTrack_5448 {
    char pad0[0x1D8];
    sWtSub_5448 sub;         // 0x1D8
};

struct sWtSnd_5448 {
    sWtTrack_5448* track;    // 0x0
    int* types;              // 0x4
    int* args;               // 0x8
    char pad[0x10C];
    sWtTrack_5448** cur;     // 0x118
};

struct sSfxEnt_5448 {
    int id;                  // 0x0
    int when;                // 0x4
    char pad8[0xC - 0x8];
    int slot;                // 0xC
    char pad10[0x1C - 0x10];
    signed char* vol;        // 0x1C
    sSfxEnt_5448* next;      // 0x20
};


#define MIN_5448(a, b) ((a) < (b) ? (a) : (b))
#define MAX_5448(a, b) ((a) < (b) ? (b) : (a))

/**
 * Plays and stops the rider's sounds for an in-game scripted sequence (NIS) frame.
 *
 * First forgets any of the rider's 4 sequence sound handles (+0xB08) the bank monitor no longer has. On a key event
 * (bit 0 of @p flags) it walks the frame's sound list (+0xC of @p vobj) while each entry's frame equals @p when, maps
 * the script sound id (cScriptObjRider_ScriptSoundIDToGameSoundInfo), fills a new sound-play slot in the audio system with defaults, the entry's
 * volume (clamped 0..127), the volume control (func_00287968) and the rider's position and closest camera
 * (func_00288A20), starts it (cBankSys_SFX_End) and keeps the handle in the entry's slot. At the end of the sequence
 * (bit 1) every slot no longer in the list is stopped.
 *
 * @param vself  the rider (this)
 * @param vobj  the script actor's frame data (On Tour tSSXFrameActor *pFrameInfo); its sound list is at +0xC
 * @param when  the sequence's current frame (On Tour currFrame)
 * @param frameDelta  frames advanced (On Tour frameDelta); unused
 * @param flags  On Tour eventBits: bit 0 key event, bit 1 sequence ended
 */
extern "C" void cRider_DoInSeqSoundUpdate(cRider* vself, void* vobj, float when, float frameDelta, int flags)
{
    cRider* self = (cRider*)vself;
    char* obj = (char*)vobj;
    int* ids = self->unk_B08;
    sWtSnd_5448* snd = (sWtSnd_5448*)cAudioInterface_bxAudio();
    int i;
    for (i = 0; i < 4; i++) {
        if (ids[i] >= 0) {
            if (cBankMonitor_BANKMONITOR_GetHandle((char*)*snd->cur + 0x1D8, ids[i], 1) == 0)
                ids[i] = -1;
        }
    }
    if (flags & 1) {
        sSfxEnt_5448* curSound = *(sSfxEnt_5448**)(obj + 0xC);
        if (curSound != 0) {
        char* rp = (char*)&self->mPosition;
        do {
            if ((float)curSound->when != when)
                return;
            int gameSoundIndex, type;
            cScriptObjRider_ScriptSoundIDToGameSoundInfo(&self->mScriptObjRiderVtbl, curSound->id, &gameSoundIndex, &type);
            int tp = type;
            int ar = gameSoundIndex;
            sWtTrack_5448* curTrack = snd->track;
            sWtSub_5448* trackSub = &curTrack->sub;
            curTrack->sub.cur++;
            trackSub->ents[curTrack->sub.cur] = trackSub->cfg;
            curTrack->sub.arr24[curTrack->sub.cur] = 0;
            trackSub->p34[curTrack->sub.cur] = 0;
            trackSub->p38[curTrack->sub.cur] = trackSub->ents[curTrack->sub.cur].b0;
            trackSub->p3C[curTrack->sub.cur] = 0;
            func_00416210((char*)trackSub + curTrack->sub.cur * 8 + 0x40, 0, 8);
            trackSub->p60[curTrack->sub.cur] = 0;
            trackSub->p64[curTrack->sub.cur] = 0;
            trackSub->p68[curTrack->sub.cur] = 0;
            trackSub->p6C[curTrack->sub.cur] = 0;
            trackSub->p70[curTrack->sub.cur].f4 = 0;
            trackSub->p70[curTrack->sub.cur].f0 = 100;
            trackSub->p70[curTrack->sub.cur].f8 = 90;
            trackSub->p70[curTrack->sub.cur].fC = 50;
            trackSub->p74[curTrack->sub.cur] = 0;
            trackSub->p80[curTrack->sub.cur] = 0;
            trackSub->p84[curTrack->sub.cur] = 0;
            trackSub->p78[curTrack->sub.cur] = 0x7F;
            trackSub->p7C[curTrack->sub.cur] = 1;
            snd->types[snd->track->sub.cur] = tp;
            snd->args[snd->track->sub.cur] = ar;
            {
                sWtTrack_5448* volTrack = *snd->cur;
                sWtSub_5448* s2 = &volTrack->sub;
                s2->p38[volTrack->sub.cur] = *curSound->vol;
                s2->ents[volTrack->sub.cur].b0 = MAX_5448(MIN_5448(s2->p38[s2->cur], 0x7F), 0);
            }
            {
                sWtTrack_5448** pt = snd->cur;
                int h = func_00287968(snd, 4, 0);
                (*pt)->sub.p3C[(*pt)->sub.cur] = h;
            }
            {
                sWtTrack_5448* posTrack = *snd->cur;
                posTrack->sub.arr24[posTrack->sub.cur] = (int)rp;
                sWtTrack_5448** pt = snd->cur;
                int h = func_00288A20(snd, self);
                (*pt)->sub.p64[(*pt)->sub.cur] = h;
            }
            *(int*)((char*)ids + (curSound->slot << 2)) = cBankSys_SFX_End(snd);
            curSound = curSound->next;
        } while (curSound != 0);
        }
    } else if (flags & 2) {
        int keep[4];
        int on = 1;
        for (i = 3; i >= 0; i--)
            keep[i] = on;
        sSfxEnt_5448* curSound;
        for (curSound = *(sSfxEnt_5448**)(obj + 0xC); curSound != 0; curSound = curSound->next)
            keep[curSound->slot] = 0;
        for (i = 0; i < 4; i++) {
            if (keep[i] != 0) {
                if (ids[i] >= 0) {
                    cBankMonitor_BANKMONITOR_StopHandle((char*)*snd->cur + 0x1D8, ids[i], 1, 0.0f);
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
extern "C" void cRider_setUbersAllowed(cRider* self, int uberAllowedFlags)
{
    self->mUberFlags = uberAllowedFlags;
    if (uberAllowedFlags == 0) {
        self->mUberLevel = 0;
        *(int*)&self->mUberTimer = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateWind);
#ifdef SKIP_ASM
extern "C" float func_002EF0A0(int rider);
extern "C" float func_002EE570(int rider);
extern float D_00504FB8[];

struct sVec4_125970
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVEntry_125970 { short delta; short index; void (*fn)(void*, sVec4_125970*); };

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_125970(const sVec4_125970& a, const sVec4_125970& b)
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
static inline sVec4_125970 Scale_125970(const sVec4_125970& v, float s)
{
    sVec4_125970 r;
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
 * The wind speed (func_002EF0A0 x 27.78, capped at 1388.9) and direction (func_002EE570, in degrees) come
 * from the rider's environment entry; below 277.78 nothing happens. When the rider's velocity along the wind
 * is lower than the wind speed, the applyBoost virtual gets the wind direction times the difference times
 * 0.015.
 */
extern "C" void cRider_updateWind(cRider* self)
{
    if (self->mIsHuman == 0)
        return;
    float maxSpeed = 1388.888916015625f;
    float speed = func_002EF0A0(self->mAIIndex) * 27.77777862548828f;
    if (speed > maxSpeed)
        speed = maxSpeed;
    if (speed >= 277.77777099609375f)
    {
        float ang = func_002EE570(self->mAIIndex) * 0.01745329424738884f;
        sVec4_125970 dir;
        dir.z = 0.0f;
        dir.w = 0.0f;
        int idx = (int)(ang * 81.4873275756836f) & 0x1FF;
        dir.y = D_00504FB8[idx];
        dir.x = D_00504FB8[idx + 0x80];
        float speedDeficit = speed - Dot_125970(*(sVec4_125970*)&self->mVelocity, dir);
        if (speedDeficit > dir.z)
        {
            sVec4_125970 imp = Scale_125970(Scale_125970(dir, speedDeficit), 0.015000000596046448f);
            char* obj = (char*)&self->mVtbl;
            sVEntry_125970* vt = *(sVEntry_125970**)obj;
            vt[kRiderVt_applyBoost].fn(obj + vt[kRiderVt_applyBoost].delta, &imp);
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
void cRider_updateTestSurface(void* self)
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
extern "C" int cRider_syncCheckSum(cRider* self)
{
    union { float f; int i; } sumBits;
    sumBits.f = *(float*)&self->mPosition + self->mPosition.y + self->mPosition.z
        + (*(float*)&self->mRotation + self->mRotation.y + self->mRotation.z + self->mRotation.w);
    return sumBits.i;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00125B18);
#ifdef SKIP_ASM
extern "C" void cPadDevice_clearAllVibrations(void* obj);

struct sVEntryI_125B18 { short delta; short index; int (*fn)(void*); };
struct sVEntryF_125B18 { short delta; short index; void (*fn)(void*, float, int); };

/**
 * Decays a player's two vibration levels and drives the pad's two motors from them.
 *
 * With @p on 0 both levels are zeroed and cPadDevice_clearAllVibrations stops the pad. Otherwise level 0 (bump) becomes max(level x
 * 0.9133 - 18.5, 0) and level 1 (rumble) is multiplied by 0.99167; when the pad's virtual slot 3 returns 2, slot 4 sets
 * motor 0 to max((rumble - 0.5) x 0.01, (bump - 2) / 972.2, 0) and motor 1 to max((rumble - 70) / 160, (bump - 100) /
 * 5000, 0). Called by cPlayer_vibrationUpdate with &unk_DFC.
 *
 * @param vibState  the two levels (bump, rumble)
 * @param controller  the cPadDevice
 * @param on  0 to stop
 */
// PORT: the unit declares (void*, int, int); the second argument is really an object pointer.
extern "C" void func_00125B18(void* vibState, int controller, int on)
{
    float* vibLevels = (float*)vibState;
    void* obj = (void*)controller;
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
    sVEntryI_125B18* vt = *(sVEntryI_125B18**)obj;
    if (vt[3].fn((char*)obj + vt[3].delta) == 2)
    {
        sVEntryF_125B18* vt2 = *(sVEntryF_125B18**)obj;
        vt2[4].fn((char*)obj + vt2[4].delta, ((vibLevels[1] - 0.5f) * 0.009999999776482582f >? (vibLevels[0] - 2.0f) / 972.2222290039062f) >? 0.0f, 0);
        sVEntryF_125B18* vt3 = *(sVEntryF_125B18**)obj;
        vt3[4].fn((char*)obj + vt3[4].delta, ((vibLevels[1] - 70.0f) * 0.0062500000931322575f >? (vibLevels[0] - 100.0f) / 5000.0f) >? 0.0f, 1);
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
extern char D_00459B90[];
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
 * Then sets the cPlayer vtable (+0xDE8), clears Pad and the two rumble floats, sets mIsHuman and adds a
 * replay cache (cReplay_addCache).
 *
 * @param inChrg  nonzero when this object is the most derived one and must build the virtual base
 * @return @p self
 */
// PORT: hand-written form of g++ 2.95's constructor for a class with a virtual base
// (cRider at +0xE10): vtable copies with delta fixups when not in charge.
extern "C" void* cPlayer_cPlayer(char* self, int inChrg)
{
    sVt9_125C70 ctorVtbl9;
    sVt22_125C70 ctorVtbl22;
    if (inChrg) {
        char* vb = self + 0xE10;
        *(void**)(self + 0xD20) = vb;
        *(void**)(self + 0xC70) = vb;
        *(void**)(self + 0xB40) = vb;
        *(void**)(self + 0xB00) = vb;
        *(void**)(self + 0xAF0) = vb;
        *(void**)(self + 0xAD0) = vb;
        *(void**)(self + 0x9C0) = vb;
        *(void**)(self + 0x610) = vb;
        *(void**)(self + 0x520) = vb;
        *(void**)(self + 0x470) = vb;
        *(void**)(self + 0x3B0) = vb;
        *(void**)(self + 0x3A0) = vb;
        *(void**)(self + 0x398) = vb;
        *(void**)(self + 0x384) = vb;
        *(void**)(self + 0x364) = vb;
        *(void**)(self + 0x358) = vb;
        *(void**)(self + 0x340) = vb;
        *(void**)(self + 0x2B8) = vb;
        *(void**)(self + 0x2A4) = vb;
        *(void**)(self + 0x288) = vb;
        *(void**)(self + 0x224) = vb;
        *(void**)(self + 0x200) = vb;
        *(void**)(self + 0x1F0) = vb;
        *(void**)(self + 0x1E4) = vb;
        *(void**)(self + 0x1C0) = vb;
        *(void**)(self + 0x1B0) = vb;
        *(void**)(self + 0x100) = vb;
        *(void**)(self + 0xA0) = vb;
        *(void**)(self + 0x70) = vb;
        *(void**)(self + 0x24) = vb;
        *(void**)(self + 0x18) = vb;
        cRider_cRider(vb);
    }
    cRiderHandler_cRiderHandler(self, 0);
    *(void**)(*(char**)(self + 0x18) + 0x6E8) = (void*)&D_00458360;
    *(void**)(*(char**)(self + 0x18) + 0x6D0) = D_00459B90;
    *(void**)(*(char**)(self + 0x18) + 0x6C0) = (void*)&D_004583A8;
    if (inChrg == 0) {
        int vc;
        ctorVtbl9 = D_00458360;
        *(void**)(*(char**)(self + 0x18) + 0x6E8) = &ctorVtbl9;
        {
            char* vbo = *(char**)(self + 0x18) - 0xE10;
            vc = self - vbo;
        }
        ctorVtbl9.e[1].delta = D_00458360.e[1].delta + vc;
        ctorVtbl22 = D_004583A8;
        *(void**)(*(char**)(self + 0x18) + 0x6C0) = &ctorVtbl22;
        ctorVtbl22.e[1].delta = D_004583A8.e[1].delta + vc;
        ctorVtbl22.e[16].delta = D_004583A8.e[16].delta + vc;
        ctorVtbl22.e[17].delta = D_004583A8.e[17].delta + vc;
        ctorVtbl22.e[18].delta = D_004583A8.e[18].delta + vc;
        ctorVtbl22.e[19].delta = D_004583A8.e[19].delta + vc;
        ctorVtbl22.e[20].delta = D_004583A8.e[20].delta + vc;
    }
    *(void**)(self + 0xDE8) = cPlayer_vtbl;
    *(int*)(self + 0xDF4) = 0;
    *(int*)(self + 0xDFC) = 0;
    *(int*)(self + 0xE00) = 0;
    *(int*)(*(char**)(self + 0x18) + 0x874) = 1;
    *(void**)(self + 0xDF8) = cReplay_addCache(SSXApp->Game->Replay);
    return self;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cPlayer__cPlayer);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/rider_classes.h"

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
        "_vt$16cRiderVbB0_rider = D_00459B90\n"
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
extern "C" int cPlayer_getGrabHeld(char* self)
{
    if (cInput_getState(*(void**)(self + 0xDF0), 0x1F))
        return 0;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x20))
        return 1;
    if (cInput_getState(*(void**)(self + 0xDF0), 33))
        return 2;
    if (cInput_getState(*(void**)(self + 0xDF0), 34))
        return 3;
    if (cInput_getState(*(void**)(self + 0xDF0), 35))
        return 4;
    if (cInput_getState(*(void**)(self + 0xDF0), 36))
        return 5;
    if (cInput_getState(*(void**)(self + 0xDF0), 37))
        return 6;
    if (cInput_getState(*(void**)(self + 0xDF0), 38))
        return 7;
    if (cInput_getState(*(void**)(self + 0xDF0), 39))
        return 8;
    if (cInput_getState(*(void**)(self + 0xDF0), 40))
        return 9;
    if (cInput_getState(*(void**)(self + 0xDF0), 41))
        return 10;
    if (cInput_getState(*(void**)(self + 0xDF0), 42))
        return 11;
    if (cInput_getState(*(void**)(self + 0xDF0), 43))
        return 12;
    if (cInput_getState(*(void**)(self + 0xDF0), 44))
        return 13;
    if (cInput_getState(*(void**)(self + 0xDF0), 45))
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
extern "C" int cPlayer_getUberGrindHeld(void* self)
{
    if (cInput_getState(*(void**)((char*)self + 0xDF0), 27))
        return 0;
    if (cInput_getState(*(void**)((char*)self + 0xDF0), 0x1C))
        return 1;
    if (cInput_getState(*(void**)((char*)self + 0xDF0), 29))
        return 2;
    if (cInput_getState(*(void**)((char*)self + 0xDF0), 0x1E))
        return 3;
    return -1;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cPlayer_vibrationReset__FPv);
#ifdef SKIP_ASM
/**
 * Clears the player's two vibration levels, unk_DFC and unk_E00 (cRider virtual slot 16).
 */
void cPlayer_vibrationReset(void* self)
{
    *(float*)((char*)self + 0xE00) = 0.0f;
    *(float*)((char*)self + 0xDFC) = 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cPlayer_vibrationBump);
#ifdef SKIP_ASM
/**
 * Raises the player's bump vibration level (unk_DFC) to @p val when that is higher (cRider virtual slot 17).
 *
 * @param val  the bump strength
 */
extern "C" void cPlayer_vibrationBump(void* self, float val)
{
    *(float*)((char*)self + 0xDFC) = val >? *(float*)((char*)self + 0xDFC);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cPlayer_vibrationRumble__FPvf);
#ifdef SKIP_ASM
/**
 * Sets the player's rumble vibration level (unk_E00) to @p val (cRider virtual slot 18).
 *
 * @param val  the rumble strength
 */
void cPlayer_vibrationRumble(void* self, float val)
{
    *(float*)((char*)self + 0xE00) = val;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", cPlayer_vibrationUpdate);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getUseDualShock(void* iface, int id);
extern "C" void func_00125B18(void* p, int a, int b);

struct sVEntry001278E8 { short delta; short index; int (*fn)(void*); };

static inline bool riderInRange_1278E8(int v)
{
    return v != 0 && v < 10;
}

/**
 * Updates the player's pad vibration once a frame (cRider virtual slot 19).
 *
 * Only with a Pad. Vibration stays on when the option interface (cBE_getInterface kind 1, cBENewPlayerInterface_getUseDualShock) allows it for
 * the rider's AI index and the replay's first word is not 1-9; then func_00125B18 decays the levels and drives the pad.
 */
extern "C" void cPlayer_vibrationUpdate(void* self)
{
    char* player = (char*)self;
    if (*(int*)(player + 0xDF4) != 0)
    {
        int flag = 0;
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        char* sub = *(char**)(player + 0x18) + 0x6C0;
        sVEntry001278E8* vt = *(sVEntry001278E8**)sub;
        if (cBENewPlayerInterface_getUseDualShock(iface, vt[7].fn(sub + vt[7].delta)) != 0)
        {
            flag = !riderInRange_1278E8(*(int*)SSXApp->Game->Replay);
        }
        func_00125B18(player + 0xDFC, *(int*)(player + 0xDF4), flag);
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
union sRiderInput_127998 {
    struct {
        unsigned pad0 : 12;
        unsigned b12 : 1, b13 : 1, b14 : 1, b15 : 1, b16 : 1, b17 : 1, b18 : 1, b19 : 1;
        unsigned a20 : 6, a26 : 6;
        unsigned w0 : 6, w6 : 6;
    } s0;
    struct {
        unsigned pad0 : 12;
        unsigned b12 : 1, b13 : 1, b14 : 1, b15 : 1, b16 : 1, b17 : 1;
        unsigned a18 : 6, a24 : 6;
        unsigned : 0;
        unsigned w0 : 6, w6 : 6;
    } s1;
    struct {
        unsigned pad0 : 12;
        unsigned b12 : 1, b13 : 1, b14 : 1;
        unsigned a15 : 6, a21 : 6;
        unsigned : 0;
        unsigned w0 : 6, w6 : 6;
    } s2;
    struct {
        unsigned pad0 : 12;
        unsigned b12 : 1, b13 : 1, b14 : 1, b15 : 1;
        unsigned v16 : 8;
        unsigned a24 : 6;
        unsigned : 0;
        unsigned w0 : 6, w6 : 6, w12 : 6;
        int t18 : 2;
    } s4;
    struct {
        unsigned pad0 : 12;
        unsigned a12 : 6;
    } s6;
    struct {
        unsigned pad0 : 12;
        unsigned b12 : 1, b13 : 1, b14 : 1, b15 : 1, b16 : 1;
        unsigned v17 : 8;
        unsigned a25 : 6;
        unsigned : 0;
        unsigned w0 : 6, w6 : 6;
    } s7;
    struct {
        unsigned pad0 : 12;
        unsigned b12 : 1, b13 : 1;
        unsigned a14 : 6;
    } s11;
    struct {
        unsigned pad0 : 12;
        unsigned b12 : 1, b13 : 1, b14 : 1;
        unsigned v15 : 8;
        unsigned a23 : 6;
    } s12;
};

#define PAD_127998 (*(void**)(self + 0xDF0))

/**
 * Fills this frame's control command from the player's pad, packed for the rider's current control state, and records
 * it for replays.
 *
 * Zeroes the 8-byte command, then switches on the control state of the rider at +0x18 (cRider_getControlState, On Tour
 * getControlState). Each state packs its own layout (the low 12 bits stay zero): buttons (cInput_getState on the pad at
 * +0xDF0, 1 bit each) and analog values (cInput_getValue * 31, 6 bits each). States 4 and 5 also pack cPlayer_getGrabHeld's
 * value and states 7 and 12 cPlayer_getUberGrindHeld's (8 bits each); state 5 adds a three-way value from axis 3 (-1 below -0.5, 1
 * above 0.5, else 0); state 11 packs two buttons and an analog value, state 8 two buttons, states 6 and 10 one analog
 * value and state 13 nothing. When a replay recorder is attached (+0xDF8) the command is passed to cReplayCache_updateControlCmd.
 *
 * @param cmd  receives the packed command (On Tour tControlCmd)
 */
// Reads the pad into the input record for the rider's state, and records it for replays.
extern "C" void cPlayer_getControlCmd(char* self, sRiderInput_127998* cmd)
{
    MEM_fill(cmd, 0, 8);
    switch (cRider_getControlState(*(void**)(self + 0x18))) {
    case 0: {
        float k = 31.0f;
        cmd->s0.b12 = cInput_getState(PAD_127998, 46);
        cmd->s0.b13 = cInput_getState(PAD_127998, 25);
        cmd->s0.b14 = cInput_getState(PAD_127998, 17);
        cmd->s0.b15 = cInput_getState(PAD_127998, 18);
        cmd->s0.b16 = cInput_getState(PAD_127998, 19);
        cmd->s0.b17 = cInput_getState(PAD_127998, 20);
        cmd->s0.b18 = cInput_getState(PAD_127998, 5);
        cmd->s0.b19 = cInput_getState(PAD_127998, 6);
        cmd->s0.a20 = (int)(cInput_getValue(PAD_127998, 0) * k);
        cmd->s0.a26 = (int)(cInput_getValue(PAD_127998, 1) * k);
        cmd->s0.w0 = (int)(cInput_getValue(PAD_127998, 2) * k);
        cmd->s0.w6 = (int)(cInput_getValue(PAD_127998, 3) * k);
        break;
    }
    case 11:
        cmd->s11.b12 = cInput_getState(PAD_127998, 46);
        cmd->s11.b13 = cInput_getState(PAD_127998, 25);
        cmd->s11.a14 = (int)(cInput_getValue(PAD_127998, 0x18) * 31.0f);
        break;
    case 1: {
        float k = 31.0f;
        cmd->s1.b12 = cInput_getState(PAD_127998, 46);
        cmd->s1.b13 = cInput_getState(PAD_127998, 19);
        cmd->s1.b14 = cInput_getState(PAD_127998, 20);
        cmd->s1.b16 = cInput_getState(PAD_127998, 17);
        cmd->s1.b15 = cInput_getState(PAD_127998, 18);
        cmd->s1.b17 = cInput_getState(PAD_127998, 22);
        cmd->s1.a24 = (int)(cInput_getValue(PAD_127998, 23) * k);
        cmd->s1.a18 = (int)(cInput_getValue(PAD_127998, 0) * k);
        cmd->s1.w0 = (int)(cInput_getValue(PAD_127998, 3) * k);
        cmd->s1.w6 = (int)(cInput_getValue(PAD_127998, 4) * k);
        break;
    }
    case 6:
        cmd->s6.a12 = (int)(cInput_getValue(PAD_127998, 26) * 31.0f);
        break;
    case 7: {
        float k = 31.0f;
        cmd->s7.b12 = cInput_getState(PAD_127998, 46);
        cmd->s7.b13 = cInput_getState(PAD_127998, 17);
        cmd->s7.b14 = cInput_getState(PAD_127998, 18);
        cmd->s7.b15 = cInput_getState(PAD_127998, 19);
        cmd->s7.b16 = cInput_getState(PAD_127998, 20);
        cmd->s7.v17 = cPlayer_getUberGrindHeld(self);
        cmd->s7.a25 = (int)(cInput_getValue(PAD_127998, 23) * k);
        cmd->s7.w0 = (int)(cInput_getValue(PAD_127998, 13) * k);
        cmd->s7.w6 = (int)(cInput_getValue(PAD_127998, 3) * k);
        break;
    }
    case 12:
        cmd->s12.b12 = cInput_getState(PAD_127998, 46);
        cmd->s12.b13 = cInput_getState(PAD_127998, 19);
        cmd->s12.b14 = cInput_getState(PAD_127998, 20);
        cmd->s12.v15 = cPlayer_getUberGrindHeld(self);
        cmd->s12.a23 = (int)(cInput_getValue(PAD_127998, 23) * 31.0f);
        break;
    case 2: {
        float k = 31.0f;
        cmd->s2.b12 = cInput_getState(PAD_127998, 46);
        cmd->s2.b13 = cInput_getState(PAD_127998, 18);
        cmd->s2.b14 = cInput_getState(PAD_127998, 20);
        cmd->s2.a15 = (int)(cInput_getValue(PAD_127998, 8) * k);
        cmd->s2.a21 = (int)(cInput_getValue(PAD_127998, 9) * k);
        cmd->s2.w0 = (int)(cInput_getValue(PAD_127998, 7) * k);
        cmd->s2.w6 = (int)(cInput_getValue(PAD_127998, 23) * k);
        break;
    }
    case 3: {
        float k = 31.0f;
        cmd->s2.b12 = cInput_getState(PAD_127998, 46);
        cmd->s2.b13 = cInput_getState(PAD_127998, 19);
        cmd->s2.b14 = cInput_getState(PAD_127998, 20);
        cmd->s2.a15 = (int)(cInput_getValue(PAD_127998, 0) * k);
        cmd->s2.a21 = (int)(cInput_getValue(PAD_127998, 23) * k);
        break;
    }
    case 4: {
        float k = 31.0f;
        cmd->s4.b12 = cInput_getState(PAD_127998, 46);
        cmd->s4.b13 = cInput_getState(PAD_127998, 25);
        cmd->s4.b14 = cInput_getState(PAD_127998, 5);
        cmd->s4.b15 = cInput_getState(PAD_127998, 6);
        cmd->s4.v16 = cPlayer_getGrabHeld(self);
        cmd->s4.a24 = (int)(cInput_getValue(PAD_127998, 0) * k);
        cmd->s4.w0 = (int)(cInput_getValue(PAD_127998, 1) * k);
        break;
    }
    case 5: {
        float k = 31.0f;
        cmd->s4.b12 = cInput_getState(PAD_127998, 46);
        cmd->s4.b13 = cInput_getState(PAD_127998, 25);
        cmd->s4.b14 = cInput_getState(PAD_127998, 21);
        cmd->s4.b15 = cInput_getState(PAD_127998, 0xC);
        cmd->s4.v16 = cPlayer_getGrabHeld(self);
        cmd->s4.a24 = (int)(cInput_getValue(PAD_127998, 0xE) * k);
        cmd->s4.w0 = (int)(cInput_getValue(PAD_127998, 0xF) * k);
        cmd->s4.w12 = (int)(cInput_getValue(PAD_127998, 11) * k);
        cmd->s4.w6 = (int)(cInput_getValue(PAD_127998, 10) * k);
        float axisValue = cInput_getValue(PAD_127998, 3);
        int axisDir;
        if (-0.5f <= axisValue) {
            axisDir = 0.5f < axisValue;
        } else {
            axisDir = -1;
        }
        cmd->s4.t18 = axisDir;
        break;
    }
    case 8:
        cmd->s11.b12 = cInput_getState(PAD_127998, 46);
        cmd->s11.b13 = cInput_getState(PAD_127998, 0x10);
        break;
    case 10:
        cmd->s6.a12 = (int)(cInput_getValue(PAD_127998, 0) * 31.0f);
        break;
    case 13:
        break;
    }
    if (*(void**)(self + 0xDF8)) {
        cReplayCache_updateControlCmd(*(void**)(self + 0xDF8), cmd);
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
void* cPlayer_writeToReplayFrame(void* self)
{
    return cRiderHandler_writeToReplayFrame(self);
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
void* cPlayer_readFromReplayFrame(void* self)
{
    return cRiderHandler_readFromReplayFrame(self);
}
#endif

