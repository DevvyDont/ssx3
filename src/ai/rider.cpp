#include "common.h"
#include "ai/rider.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("ai/rider", cRider_cRider);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void func_00140738(void* self);
extern "C" void* func_0027F968(void* self);
extern "C" void* func_002F1850(void* mem, void* rider);
extern "C" void* func_002EC9E0(void* self, int a1, int a2);
void* func_003758F8(void* self);
extern "C" void func_00283298(void* rig);
extern "C" void* func_0028B180();
// PORT: func_00289AF0 is declared (void*) but takes (mgr, rider).
void func_00289AF0_impl(void* mgr, void* rider) __asm__("func_00289AF0__FPv");
extern "C" void* MEM_fill(void* dst, int value, int size);
// PORT: memset-like; called as void here.
extern "C" void MEM_fill_v(void* dst, int value, int size) __asm__("MEM_fill");
extern void* D_004A28A8;
extern void* D_00459C18[];
extern char D_00459B90[];
extern void* D_00459BD0[];
extern void* D_00459CC8[];
extern const char D_004578A8[];
extern const char D_004578C8[];
extern const char D_004578E0[];
extern const char D_004578F0[];
extern const char D_00457908[];
extern const char D_00457920[];
extern const char D_00457938[];
extern const char D_00457950[];

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
extern "C" void* cRider_cRider_impl(cRider* self)
{
    unsigned int none = 0xFFFFFFFF;
    int one = 1;
    func_00140738(self);
    self->mVtbl = D_00459CC8;
    func_0027F968(&self->mScriptObjRiderVtbl);
    self->mScriptObjectVtbl = D_00459BD0;
    self->mAIIndex = -1;
    self->mViewIndex = -1;
    self->mSafeRenderTick = -1;
    self->mScriptObjRiderVtbl = D_00459B90;
    self->mVtbl = D_00459C18;
    self->mGroundMaterial = 0x31;
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
    self->mTrackPathHint = (int*)cMemMan_alloc(0x20, D_004578A8, 0, 0);
    self->mAIPathHint = (int*)cMemMan_alloc(0x20, D_004578C8, 0, 0);
    sRiderAux_B698* aux = new (D_004578E0, 0, 0) sRiderAux_B698;
    int team = *(int*)(*(char**)((char*)D_004A28A8 + 0x84) + 0x20);
    aux->f0 = team;
    aux->f4 = 0;
    aux->f8 = none;
    aux->f10C = none;
    aux->f210 = none;
    self->mOverlapList = aux;
    self->mAttachedEmitterFX = func_002F1850(cMemMan_alloc(0xA8, D_004578F0, 0, 0), self);
    self->mBoltOnFX = func_002EC9E0(cMemMan_alloc(0xC8, D_00457908, 0, 0), (int)self, (int)self->mAttachedEmitterFX);
    self->mLineHint = cMemMan_alloc(0x10, D_00457920, 0, 0);
    self->mWallHint = cMemMan_alloc(0x10, D_00457938, 0, 0);
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
    int m1 = -1;
    self->mShadow = func_003758F8(cMemMan_alloc(0x1A0, D_00457950, 0, 0));
    self->unk_450 = m1;
    self->unk_AA4 = 0;
    self->mIsOnScreen = 0;
    self->unk_B1C = 0;
    func_00283298(self->unk_6DC);
    self->mWipeoutSlideHandle = m1;
    self->mWipeoutTumbleHandle = m1;
    func_00289AF0_impl(func_0028B180(), self);
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
extern "C" void* func_0028B180();
// PORT: func_00289AF8 takes (mgr, rider); the mangled name says one void* param.
void func_00289AF8_impl(void* mgr, void* rider) __asm__("func_00289AF8__FPv");
extern "C" void cScriptObject_DeInitScriptObject(void* p);
extern "C" void func_00283228(void* p, int flags);
extern "C" void cAnimModel__cAnimModel(void* p, int flags);
extern "C" void cRiderSphereTree__cRiderSphereTree(void* p, int flags);
extern "C" void func_00113170(void* p, int flags);
extern "C" void func_001033F8(void* a, void* p);
extern "C" void func_00117290(void* p, int flags);
extern "C" void func_00375918(void* p, int flags);
void operator_delete(int* p);
extern void* D_004A28A8;
extern void* D_00459C18[];
extern char D_00459B90[];
extern void* D_00459BD0[];
extern void* D_00482018[];
extern void* D_00481FD8[];
extern void* D_00459CC8[];

static inline void vdel_11B978(char* o, int off)
{
    sVE_11B978* vt = *(sVE_11B978**)(o + off);
    vt[1].fn(o + vt[1].delta, 3);
}

extern "C" void cRider__cRider(cRider* self, int flags)
{
    self->mScriptObjectVtbl = D_00459BD0;
    self->mScriptObjRiderVtbl = D_00459B90;
    self->mVtbl = D_00459C18;
    func_00289AF8_impl(func_0028B180(), self);
    cScriptObject_DeInitScriptObject(self->unk_6DC);
    char* o = (char*)self->mRiderAnim;
    if (o != 0)
        vdel_11B978(o, 0x58);
    void* p = self->mAnimModel;
    if (p != 0)
        cAnimModel__cAnimModel(p, 3);
    p = self->mSphereTree;
    if (p != 0)
        cRiderSphereTree__cRiderSphereTree(p, 3);
    operator_delete(self->mTrackPathHint);
    operator_delete(self->mAIPathHint);
    p = self->mAirPredictor;
    if (p != 0)
        func_00113170(p, 3);
    void* box = self->mFocusBox;
    if (box != 0)
        func_001033F8(*(void**)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 0xA4), box);
    operator_delete((int*)self->mFocusBox);
    p = self->mMetrix;
    if (p != 0)
        func_00117290(p, 3);
    operator_delete((int*)self->mOverlapList);
    operator_delete((int*)self->mWallHint);
    operator_delete((int*)self->mLineHint);
    o = (char*)self->mBoltOnFX;
    if (o != 0)
        vdel_11B978(o, 0xC4);
    o = (char*)self->mAttachedEmitterFX;
    if (o != 0)
        vdel_11B978(o, 0xA4);
    p = self->mShadow;
    if (p != 0)
        func_00375918(p, 3);
    self->mScriptObjectVtbl = D_00482018;
    self->mScriptObjRiderVtbl = D_00481FD8;
    func_00283228(self->unk_6DC, 0);
    self->mVtbl = D_00459CC8;
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_addFocusBox);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_00457960[];
extern void* D_004A28A8;
extern "C" void func_001033B0(void* self, void* box);

extern "C" void cRider_addFocusBox(cRider* self)
{
    void* box = cMemMan_alloc(0x10, D_00457960, 0, 0);
    self->mFocusBox = box;
    func_001033B0(*(void**)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 0xA4), box);
}
#endif

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cRiderMetrix_cRiderMetrix(void* mem);
extern "C" void cRiderMetrix_linkToRider(void* metrix);
extern const char D_00457970[];

//100%
INCLUDE_ASM("ai/rider", cRider_addRiderMetrix__FP6cRider);
#ifdef SKIP_ASM
// PORT: cRiderMetrix_linkToRider really takes (metrix, rider); the unit declares one arg.
extern "C" void cRiderMetrix_linkToRider_impl(void* metrix, cRider* rider) __asm__("cRiderMetrix_linkToRider");

void cRider_addRiderMetrix(cRider* self)
{
    void* mem = cMemMan_alloc(0x1CC, D_00457970, 0, 0);
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
        sBEEntry_11BBE8* e = (sBEEntry_11BBE8*)func_0014D998(db, ch);
        int cnt = func_0014D988(db, ch);
        for (int i = 0; i < cnt; i++)
        {
            list[n] = (char*)e;
            n++;
            e++;
        }
    }
    else
    {
        sRiderData_11BBE8* r = (sRiderData_11BBE8*)func_0014AD50(iface, self->mAIIndex);
        int cnt = func_0014D988(db, ch);
        sBEEntry_11BBE8* e = (sBEEntry_11BBE8*)func_0014D998(db, ch);
        for (int i = 0; i < cnt; i++, e++)
        {
            short idx = r->map[e->f4];
            sSlot_11BBE8* s;
            if (idx >= 0)
                s = &r->slots[idx];
            else
                s = 0;
            if (s->flags & 0x10)
            {
                list[n] = (char*)e;
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

extern "C" void func_0011BD60(cRider* self, void** list, int n)
{
    for (int i = 0; i < n; i++, list++)
    {
        char* name = *(char**)((char*)*list + 0x28);
        sAnimNameTable_11BD60* t = ((sAnimNameTable_11BD60*)self->mLoadList);
        if (name == 0) continue;
        if (*name == 0) continue;
        int j;
        for (j = 0; j < t->count; j++)
        {
            if (func_004165A8(name, &t->e[j]) == 0) goto next;
        }
        {
            sAnimName_11BD60* e = &t->e[t->count];
            strcpy(e->name, name);
            e->f100 = -1;
            e->f104 = 0;
            t->count++;
        }
    next:;
    }
    sAnimNameTable_11BD60* t2 = ((sAnimNameTable_11BD60*)self->mLoadList);
    func_00418EF8(t2, t2->count, 0x108, func_0011B678);
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

extern "C" void func_0011BE88(cRider* self, void** list, int n)
{
    void* iface = cBE_getInterface(cBE_getBE(), 9);
    int a = func_0014AC30(self->mAIIndex);
    if (cBELibrary_getCheatCharacterID(self->mAIIndex)) {
        for (int i = 0; i < n; i++) {
            char* name = *(char**)((char*)list[i] + 0x2C);
            sAnimNameTable_11BE88* t = ((sAnimNameTable_11BE88*)self->mLoadList);
            if (name == 0) continue;
            if (*name == 0) continue;
            int j;
            int end = t->count + t->start;
            for (j = t->start; j < end; j++) {
                if (func_004165A8(name, &t->e[j]) == 0) goto next;
            }
            {
                sAnimName_11BE88* e = &t->e[end];
                strcpy(e->name, name);
                e->f100 = -1;
                e->f104 = 0;
                t->count++;
            }
        next:;
        }
    } else {
        int b = func_0014AC50(self->mAIIndex);
        for (int i = 0; i < n; i++) {
            char buf[0x100];
            if (func_0014B988(iface, b, a, list[i], buf) == 0) continue;
            char* name = buf;
            sAnimNameTable_11BE88* t = ((sAnimNameTable_11BE88*)self->mLoadList);
            if (*name == 0) continue;
            int j;
            int end = t->count + t->start;
            for (j = t->start; j < end; j++) {
                if (func_004165A8(name, &t->e[j]) == 0) goto next2;
            }
            {
                sAnimName_11BE88* e = &t->e[end];
                strcpy(e->name, name);
                e->f100 = -1;
                e->f104 = 0;
                t->count++;
            }
        next2:;
        }
    }
    sAnimNameTable_11BE88* t2 = ((sAnimNameTable_11BE88*)self->mLoadList);
    func_00418EF8(&t2->e[t2->start], t2->count, 0x108, func_0011B678);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011C0E0);
#ifdef SKIP_ASM
struct sRiderVE_0011C0E0
{
    short delta;
    short index;
    int (*fn)(void*);
};
extern void* D_004A289C;

extern "C" int func_0011C0E0(cRider* self)
{
    if (self->mSafeRenderTick != -1)
    {
        void* g = D_004A289C;
        sRiderVE_0011C0E0* vt = *(sRiderVE_0011C0E0**)((char*)g + 0x10D8);
        return self->mSafeRenderTick < vt[114].fn((char*)g + vt[114].delta);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_0011C138);
#ifdef SKIP_ASM
void* func_00314978(void* self);
void func_00314A18(void* self, int a1);
extern "C" void func_00314988(int* self, int flags);
extern "C" void* cMdfArchive_getModelPart(void* self, const char* name);
extern "C" int func_0011BBE8(void* self, char** list);
extern "C" void cAnimModel_addModelPartLOD(void* model, int lod, void* part, int a3);

struct sMdfArchive_11C138
{
    int a[4];
};

extern "C" void func_0011C138(cRider* self, int a1)
{
    sMdfArchive_11C138 arc;
    char* list[52];
    func_00314978(&arc);
    func_00314A18(&arc, a1);
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
    func_00314988((int*)&arc, 2);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_initOnce);
#ifdef SKIP_ASM
extern "C" void cRider_loadRider(void* self);
extern "C" void THREAD_yieldticks(int ticks);

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
INCLUDE_ASM("ai/rider", func_0011D390);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.
template <class F> struct sVEntry {
    short delta;
    short index;
    F* fn;
};
extern void* D_004A28A8;
extern signed char D_00535BC8[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface_11D390(void* be, int kind) __asm__("cBE_getInterface__Fv");
// cRiderMetrix_reset (cRiderMetrix reset) takes only the metrix; its $5 is left over from the store above.
extern "C" void cRiderMetrix_reset(void* metrix);
extern "C" void cRiderAnimBase_reset(void* anim);
extern "C" bool func_0014EF70(void* iface, int riderIndex);
extern "C" void func_00112180(void* rider, int v);
extern "C" int func_00145C38(void* iface, int index);
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void func_0011DD98(cRider* self);
extern "C" void func_00119E38(void* p, int flip, int c);
extern "C" void func_0011DE60(cRider* self, int a1, int a2);
extern "C" void func_0011DF18(cRider* self, int notify);
extern "C" void func_00120D58(cRider* self);
extern "C" void func_00111890(void* self);
extern "C" void func_00125038(cRider* self);
// PORT: prototype mismatch. The unit defines func_0011FEC8/cRider_setMotionState with one
// param, but their bodies forward $5 to func_00111538/cRiderHandler__setMotionState (int arg).
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");


void func_0011D390_impl(cRider* self) __asm__("func_0011D390");
void func_0011D390_impl(cRider* self)
{
    if (self->mLoadStep == 7)
    {
        *(int*)((char*)self->mRiderHandler + 0xDE0) = 3;
        *(int*)((char*)self->mRiderHandler + 0xDE4) = 0xD;
        self->mRubberBandBehind = 0;
        self->mRubberBandAhead = 0;
        self->unk_0D8 = -1;
        cRiderMetrix_reset(self->mMetrix);
        cRiderAnimBase_reset(self->mRiderAnim);
        self->mNaturalStance = func_0014EF70(cBE_getInterface_11D390(cBE_getBE(), 2), self->mAIIndex);
        func_00112180(self, 1);
        void* iface = cBE_getInterface_11D390(cBE_getBE(), 0);
        // PORT: the gotos reproduce the shared compare chain (mode 4, 5, 6) that two different entry conditions fall into.
        if (func_00145C38(cBE_getInterface_11D390(cBE_getBE(), 1), self->mAIIndex) != 0
            && D_00535BC8[0x49] == 0 && *cBENewRaceInterface_getLocation(iface) == 0xE)
        {
            if (D_00535BC8[0x48] == 4)
            {
                func_0011DD98(self);
                cRider_setMotionState_impl(self, 1);
                func_0011FEC8_impl(self, 4);
                func_00119E38(self->mMetrix, 0, 0);
                goto done;
            }
            goto m5;
        }
        if (D_00535BC8[0x48] == 4)
            goto bb;
    m5:
        if (D_00535BC8[0x48] == 5)
            goto bb;
        if (D_00535BC8[0x48] == 6)
        {
        bb:
            int v = *cBENewRaceInterface_getLocation(iface);
            switch (v)
            {
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
                func_0011DE60(self, 0, 2);
                func_0011DF18(self, 0);
                break;
            case 14:
            case 15:
            case 16:
                func_0011DE60(self, self->mAIIndex, 1);
                func_0011DF18(self, 0);
                break;
            default:
                func_0011DE60(self, 1, 2);
                func_0011DF18(self, 0);
                break;
            }
        }
        else
        {
            cRider_setMotionState_impl(self, 3);
            func_0011FEC8_impl(self, 6);
        }
    done:
        if (D_00535BC8[0x49] != 0)
            func_00120D58(self);
        self->mFinishTime = -1.0f;
        self->unk_3FC = 0;
        self->mUberTimer = 0;
        self->mUberLevel = 0;
        self->mDidNotFinish = 0;
        self->mStartTick = *(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 8);
        self->mFinishTick = 0;
        self->mPenaltyTicks = 0;
        self->mOverlapCount = 0;
        self->mPowerUpBoostTimer = 0;
        self->mPowerUpTrickTimer = 0;
        self->unk_5A0 = 0;
        sVEntry<void(void*)>* vt = *(sVEntry<void(void*)>**)((char*)self + 0x6C0);
        vt[kRiderVt_vibrationReset].fn((char*)self + vt[kRiderVt_vibrationReset].delta);
        func_00111890(self->mRiderHandler);
        self->unk_5A4 = 0;
        self->unk_5A8 = 0;
        self->unk_5AC = 0;
        self->mHasRival = 0;
        self->mLastShoveFrame = 0;
        self->mRivalIndex = -1;
        self->unk_100 = 1;
        func_00125038(self);
        self->unk_5B0 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011D640);
#ifdef SKIP_ASM
extern "C" int func_0011D640(cRider* self)
{
    int s = self->mLoadStep;
    if (s == 7)
        return 1;
    return s == 7;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011D660);
#ifdef SKIP_ASM
// Places the rider at pos facing dir (xy), optionally dropping it onto the ground below
// (drop >= 0: probe from 200 above to 7000 below), and resets its motion state and animation.
extern "C" float func_0031C228(float x);
extern "C" float func_0031C128(float x);
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" void cRider_updateOrientationImplicit(cRider* self);
extern "C" void cRiderAnimBase_reset(void* anim);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void func_00312598(void* anim, int a1);
extern "C" void func_0011EB60(cRider* self, float value);
extern "C" void func_0011EB98(void* self);
extern "C" void* func_003103F0(void* obj);
extern "C" void func_0011E150(cRider* self, int a1);
extern "C" void func_00332DB8(void*, void*);
extern "C" void func_00122088(cRider*);
extern "C" void func_002105B0(int);
extern void* D_004A28A8;

// The unit declares these later with their own vector types; bind them by asm label.
extern const t4Vector D_004FF120_D660 __asm__("D_004FF120");
extern const t4Vector D_004FF130_D660 __asm__("D_004FF130");
extern const t4Vector D_004FF160_D660 __asm__("D_004FF160");

struct sRay_D660 {
    char data[0xB0];
} __attribute__((aligned(16)));

struct sHit_D660 {
    t4Vector pos;       // 0x00
    t4Vector normal;    // 0x10
    char pad20[0x60];
} __attribute__((aligned(16)));

void func_0032E100_D660(sRay_D660* ray, const t4Vector& a, const t4Vector& b, int n, float t) __asm__("func_0032E100");
extern float func_00336850_D660(void* coll, sRay_D660* ray, sHit_D660* hit, int flags) __asm__("func_00336850");

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
    func_0031BE50(&s, &c, angle * 0.5f);
    return sQuat_D660(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline sQuat_D660 AxisAngleL_D660(const t4Vector& axis, float angle)
{
    float s, c;
    func_0031BE50(&s, &c, angle * 0.5f);
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

// PORT: the unit declares func_0011D660(void*, sVec4_0011DE60*, ..., int, float) later; bind the cRider* view by asm label.
// drop is declared before anim: the order of the parameter copies follows it (ABI-identical: f12 and a3 either way).
extern "C" void func_0011D660_impl(cRider* self, const t4Vector* pos, const float* dir, float drop, int anim) __asm__("func_0011D660");
extern "C" void func_0011D660_impl(cRider* self, const t4Vector* pos, const float* dir, float drop, int anim)
{
    ClearCache_D660((char*)self->mLineHint);
    ClearCache_D660((char*)self->mWallHint);
    t4Vector p = *pos;
    t4Vector n = D_004FF160_D660;
    float angle = Atan2_D660(dir[1], dir[0]);
    self->mVisitedIds = 0xFFFFFFFF;
    if (drop >= 0.0f)
    {
        t4Vector lo = Add_D660(*pos, Scale_D660(D_004FF160_D660, -7000.0f));
        t4Vector hi = Add_D660(*pos, Scale_D660(D_004FF160_D660, 200.0f));
        sHit_D660 hit;
        sRay_D660 ray;
        func_0032E100_D660(&ray, lo, hi, 2, 0.9791666865348816f);
        if (func_00336850_D660(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x20), &ray, &hit, 0) >= 0.0f)
        {
            p = hit.pos;
            n = hit.normal;
        }
        AddEq_D660(p, Scale_D660(D_004FF160_D660, drop));
    }
    self->mPosition = p;
    *(sQuat_D660*)&self->mRotation = AxisAngle_D660(D_004FF160_D660, angle - 1.5707963705062866f);
    self->mUseRootPoseOverride = 0;
    t4Vector axis = Cross_D660(D_004FF160_D660, n);
    float len = Length_D660(axis);
    if (len > 0.0010000000474974513f)
    {
        DivEq_D660(axis, len);
        // PORT: g++ minimum operator.
        sQuat_D660 q = AxisAngleL_D660(axis, func_0031C128(len <? 1.0f));
        self->mRotation = QuatMul_D660(q, self->mRotation);
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
    self->mRailStyle = 0;
    self->unk_32C = 0;
    self->mPressType = 0;
    self->mAttackDir = D_004FF120_D660;
    self->mAttackBlend = 0.0f;
    self->mHudMsgExpire = -1;
    self->mHudMsgId = 0;
    self->unk_35C = 0.0f;
    self->unk_320 = self->mNaturalStance;
    cRiderAnimBase_reset(self->mRiderAnim);
    *(int*)((char*)self->mRiderAnim + 0x18) = self->unk_320;
    float yaw = 0.0f;
    if (self->unk_320 != 0)
        yaw = 3.1415927410125732f;
    char* ra = (char*)self->mRiderAnim;
    *(t4Vector*)(ra + 0x30) = D_004FF130_D660;
    *(sQuat_D660*)(ra + 0x40) = AxisAngle_D660(D_004FF160_D660, -yaw);
    cRiderAnimBase_play(self->mRiderAnim, anim, 0, -1.0f);
    func_00312598(self->mRiderAnim, 0);
    func_0011EB60(self, 1.0f);
    func_0011EB98(self);
    func_003103F0(self->mAnimModel);
    func_0011E150(self, 0);
    func_00332DB8(self->mOverlapList, &self->mBoundsMin);
    func_00122088(self);
    func_002105B0(self->mAIIndex);
    sVE_D660* vt = *(sVE_D660**)&self->mVtbl;
    vt[kRiderVt_vibrationReset].fn((char*)self + vt[kRiderVt_vibrationReset].delta);
    if ((unsigned int)self->mViewIndex < 2)
    {
        char* views = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84);
        if (views != 0)
        {
            char* h = *(char**)(*(char**)(views + (self->mViewIndex << 2) + 4) + 0xA8);
            sVE_D660* hv = *(sVE_D660**)(h + 0x14);
            hv[4].fn(h + hv[4].delta);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_0011DD98);
#ifdef SKIP_ASM
extern "C" void* func_0026B5E0(void*, int, int);
extern char D_004D33A0[];
extern "C" float func_00115B08(void* self);

struct sVec4_0011DD98
{
    float x, y, z, w;
} __attribute__((aligned(16)));

// The unit declares func_0011D660 later with its own vector type; bind it by asm label.
void func_0011D660_11DD98(void* self, sVec4_0011DD98* a, sVec4_0011DD98* b, int n, float t) __asm__("func_0011D660");

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

extern "C" void func_0011DD98(cRider* self)
{
    float* p = (float*)func_0026B5E0(D_004D33A0, 2, self->mAIIndex);
    sVec4_0011DD98 dir;
    sVec4_0011DD98 pos;
    dir.x = p[5];
    dir.y = p[6];
    dir.z = p[7];
    dir.w = 0.0f;
    pos.x = p[2];
    pos.y = p[3];
    pos.z = p[4];
    pos.w = 1.0f;
    vu0AddEq_11DD98(pos, vu0Scale_11DD98(dir, func_00115B08(self)));
    func_0011D660_11DD98(self, &pos, &dir, 0x11F, 2999.942626953125f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_0011DE60);
#ifdef SKIP_ASM
extern "C" void* func_0026B5E0(void*, int, int);
extern char D_004D33A0[];
extern "C" void func_00119368(void* metrix, int flag);
// PORT: prototype mismatch. The unit defines func_0011FEC8/cRider_setMotionState with one
// param, but their bodies forward $5 to func_00111538/cRiderHandler__setMotionState (int arg).
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

struct sVec4_0011DE60
{
    float x, y, z, w;
};

extern "C" void func_0011D660(void* self, sVec4_0011DE60* a, sVec4_0011DE60* b, int n, float t);

extern "C" void func_0011DE60(cRider* self, int a1, int a2)
{
    float* p = (float*)func_0026B5E0(D_004D33A0, a2, a1);
    sVec4_0011DE60 pos;
    sVec4_0011DE60 dir;
    float px = p[2];
    float py = p[3];
    float pz = p[4];
    pos.x = px;
    pos.y = py;
    pos.z = pz;
    pos.w = 1.0f;
    float dx = p[5];
    float dy = p[6];
    float dz = p[7];
    dir.x = dx;
    dir.y = dy;
    dir.z = dz;
    dir.w = 1.0f;
    func_00119368(self->mMetrix, 1);
    func_0011FEC8_impl(self, 0);
    cRider_setMotionState_impl(self, 0);
    func_0011D660(self, &pos, &dir, 5, 0.0f);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011DF18);
#ifdef SKIP_ASM
extern "C" char* func_00311B20(void*, int);
extern "C" void func_00111890(void* self);
extern "C" void func_003099F8(void* self);
extern void* D_004A3DD8;

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

extern "C" void func_0011DF18(cRider* self, int notify)
{
    sVec4_0011DF18 v = *(sVec4_0011DF18*)&self->mMatrix.row[1];
    *(int*)(func_00311B20(self->mRiderAnim, 2) + 0x90) = 0;
    sVec4_0011DF18 t = vu0Scale_11DF18(v, 833.3333740234375f);
    *(sVec4_0011DF18*)&self->mVelocity = t;
    self->mVelocity.z = 0.0f;
    func_00111890(self->mRiderHandler);
    if (notify != 0)
    {
        char* sub = (char*)&self->mVtbl;
        sVEntryA_0011DF18* vtA = *(sVEntryA_0011DF18**)sub;
        char* obj = *(char**)(*(char**)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84) + 0x4) + 0xA0);
        sVEntryB_0011DF18* vtB = *(sVEntryB_0011DF18**)(obj + 0x14);
        char* objB = obj + vtB[3].delta;
        vtB[3].fn(objB, vtA[kRiderVt_getAIIndex].fn(sub + vtA[kRiderVt_getAIIndex].delta));
    }
    func_003099F8(D_004A3DD8);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011DFE0);
#ifdef SKIP_ASM
extern "C" void func_0031BE50(float* s, float* c, float angle);
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
    func_0031BE50(&s, &c, angle * 0.5f);
    return sQuat_0011DFE0(s * axis.x, s * axis.y, s * axis.z, c);
}

extern "C" void func_0011DFE0(cRider* self, t4Vector* axis, float angle)
{
    self->mRotation = QuatMul_0011DFE0(AxisAngle_0011DFE0(*axis, angle), self->mRotation);
    cRider_updateOrientationImplicit(self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_updateOrientationImplicit);
#ifdef SKIP_ASM
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
INCLUDE_ASM("ai/rider", func_0011E150);
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

extern "C" void func_0011E150_impl(cRider* self, sVec4* pin) __asm__("func_0011E150");
extern "C" void func_0011E150_impl(cRider* self, sVec4* pin)
{
    sVec4 up = *(sVec4*)&self->mMatrix.row[2];
    sVec4 side = *(sVec4*)&self->mMatrix.row[0];
    sVec4 front = *(sVec4*)&self->mMatrix.row[1];
    sVec4* p = pin ? pin : (sVec4*)&self->mPosition;
    *(sVec4*)&self->mBoundsMin = *(sVec4*)&self->mBoundsMax = *p;
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Add_E150(Add_E150(Add_E150(*p, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 250.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Sub_E150(Add_E150(Add_E150(*p, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 50.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Add_E150(Sub_E150(Add_E150(*p, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 250.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Sub_E150(Sub_E150(Add_E150(*p, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 50.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Add_E150(Add_E150(Sub_E150(*p, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 250.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Sub_E150(Add_E150(Sub_E150(*p, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 50.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Add_E150(Sub_E150(Sub_E150(*p, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 250.0f)));
    Grow_E150(*(sBox_E150*)&self->mBoundsMin, Sub_E150(Sub_E150(Sub_E150(*p, Scale_E150(front, 150.0f)), Scale_E150(side, 100.0f)), Scale_E150(up, 50.0f)));
    *(sVec4*)&self->mBoundSphere = *p;
    self->mBoundSphere.w = 250.0f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011EB60);
#ifdef SKIP_ASM
extern "C" void func_003123C0(void*, int);
extern "C" void func_00312490(void*);

extern "C" void func_0011EB60(cRider* self, float value)
{
    func_003123C0(self->mRiderAnim, self->unk_B1C);
    func_00312490(self->mRiderAnim);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011EB98);
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
extern t4Vector D_004FF130;
extern t4Vector D_004FF160;
extern t4Vector D_004FF220;
extern "C" void func_00134DD0(void* self, void* xf);
extern "C" void cRider_doLeanPoseAdjust(void* self, void* xf);
extern "C" void func_00310200(cAnimModel* self, void* roots);
extern "C" void func_0011F3D8(void* self, void* obj, void* xf, float t);
extern "C" void func_00106828(void* self);
extern "C" float func_0031C128(float x);
extern "C" void func_0031BE50(float* s, float* c, float angle);
int func_0011FEE8(void* self);

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
    func_0031BE50(&s, &c, angle * 0.5f);
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
    r.pos = Add_EB98(Rot_EB98(a.rot, Sub_EB98(b.pos, D_004FF130)), a.pos);
    return r;
}

extern "C" void func_0011EB98_impl(cRider* self) __asm__("func_0011EB98");
extern "C" void func_0011EB98_impl(cRider* self)
{
    sXf_EB98 xf;
    xf.pos = *(sVec4W*)&self->mPosition;
    xf.rot = *(sVec4W*)&self->mRotation;
    if (func_0011FEE8(self) == 5)
        func_00134DD0((char*)self->mRiderHandler + 0x230, &xf);
    cRider_doLeanPoseAdjust(self, &xf);
    sXf_EB98 ab[2];
    ab[1] = xf;
    ab[0] = xf;
    if (func_0011FEE8(self) != 13 && self->mSlewed[18].cur != 0.0f)
    {
        sVec4W s = Scale_EB98(D_004FF160, self->mSlewed[18].cur);
        ab[0].pos += Rot_EB98(ab[0].rot, s);
    }
    if (self->mUseRootPoseOverride != 0)
    {
        self->mAnimModel->localPos[self->mBoneBoardRoot] = D_004FF130;
        self->mAnimModel->localRot[self->mBoneBoardRoot] = D_004FF220;
        self->mAnimModel->localPos[self->mBoneBoardChild] = D_004FF130;
        self->mAnimModel->localRot[self->mBoneBoardChild] = D_004FF220;
        ab[1].pos = *(sVec4W*)&self->mRootPoseOverridePos;
        ab[1].rot = *(sVec4W*)&self->unk_140[0];
    }
    func_00310200(self->mAnimModel, ab);
    if (self->mSlewed[17].cur != 0.0f)
    {
        sXf_EB98* b1 = (sXf_EB98*)(self->mAnimModel->bones + self->mBoneBoardRoot);
        float qx = b1->rot.x, qy = b1->rot.y, qz = b1->rot.z, qw = b1->rot.w;
        sVec4W ax;
        ax.w = 0.0f;
        ax.x = 1.0f - 2.0f * (qy * qy + qz * qz);
        ax.y = 2.0f * (qx * qy + qw * qz);
        ax.z = 2.0f * (qx * qz - qw * qy);
        sVec4W c = Cross_EB98(*(sVec4W*)&self->unk_390, ax);
        float len = Length_EB98(c);
        if (len > 9.99999993922529e-09f)
        {
            float d = DivDot_EB98(c, len, *(sVec4W*)&self->unk_390, ax);
            float m = Min_EB98(len, 1.0f);
            sVec4W q;
            if (d < 0.0f)
                q = AxisAngle_EB98(c, (func_0031C128(m) - 1.5707963705062866f) * self->mSlewed[17].cur);
            else
                q = AxisAngle_EB98(c, (1.5707963705062866f - func_0031C128(m)) * self->mSlewed[17].cur);
            b1->rot = QuatMul_EB98(q, b1->rot);
        }
        if (0.0f < *(float*)((char*)self + 0x31C))
            AddTo_EB98(b1->pos, Scale_EB98(Scale_EB98(*(sVec4W*)&self->mGroundNormal, *(float*)((char*)self + 0x31C)), self->mSlewed[17].cur));
        {
            cAnimModel* am = self->mAnimModel;
            sXf_EB98 local;
            local.pos = sVec4W(am->localPos[self->mBoneBoardChild].x * am->unk_140.x, am->localPos[self->mBoneBoardChild].y * am->unk_140.y, am->localPos[self->mBoneBoardChild].z * am->unk_140.z, am->localPos[self->mBoneBoardChild].w * am->unk_140.w);
            sVec4W* lr = (sVec4W*)&am->localRot[self->mBoneBoardChild];
            local.rot = *lr;
            sXf_EB98* bn = (sXf_EB98*)&am->bones[self->mBoneBoardChild];
            *bn = Combine_EB98(*b1, local);
        }
    }
    if (func_0011FEE8(self) != 13 && self->unk_318 != 0.0f)
    {
        sXf_EB98* bb = (sXf_EB98*)(self->mAnimModel->bones + self->mBoneBoardChild);
        {
            sXf_EB98 xx = Combine_EB98(*bb, *(sXf_EB98*)((char*)self + 0x8F0));
            func_0011F3D8(self, (char*)self + 0x8E0, &xx, self->unk_318);
        }
        {
            sXf_EB98 xx = Combine_EB98(*bb, *(sXf_EB98*)((char*)self + 0x920));
            func_0011F3D8(self, (char*)self + 0x910, &xx, self->unk_318);
        }
    }
    QuatToMat_EB98((char*)self + 0x160, &xf.rot, &xf.pos);
    func_00106828(self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011F3D8);
#ifdef SKIP_ASM
// Two-bone IK: moves bone chain[2] (end effector) toward target (blended by blend), then bends the
// chain[1] joint and swings the chain[0] root so the end reaches it. Uses the unit's sVec4W / sXf_EB98
// helpers (func_0011EB98, this function's caller).
extern "C" sVec4W func_0031BCB0(const sVec4W& a, const sVec4W& b, float t);

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
    return Add_EB98(Rot_EB98(x.rot, Sub_EB98(*(const sVec4W*)&v, D_004FF130)), x.pos);
}

extern "C" void func_0011F3D8_impl(cRider* self, int* chain, sXf_EB98* target, float blend) __asm__("func_0011F3D8");
extern "C" void func_0011F3D8_impl(cRider* self, int* chain, sXf_EB98* target, float blend)
{
    sXf_EB98* end = (sXf_EB98*)&self->mAnimModel->bones[chain[2]];
    if (blend == 1.0f) {
        *end = *target;
    } else {
        end->pos = Add_EB98(end->pos, Scale_EB98(Sub_EB98(target->pos, end->pos), blend));
        end->rot = func_0031BCB0(end->rot, target->rot, blend);
    }
    cAnimModel* am = self->mAnimModel;
    sAnimModelBone* bones = am->bones;
    float a = sVec4W(am->localPos[chain[1]].x * am->unk_140.x, am->localPos[chain[1]].y * am->unk_140.y,
                     am->localPos[chain[1]].z * am->unk_140.z, am->localPos[chain[1]].w * am->unk_140.w).x;
    sXf_EB98* mid = (sXf_EB98*)&bones[chain[1]];
    float b = sVec4W(am->localPos[chain[2]].x * am->unk_140.x, am->localPos[chain[2]].y * am->unk_140.y,
                     am->localPos[chain[2]].z * am->unk_140.z, am->localPos[chain[2]].w * am->unk_140.w).x;
    sXf_EB98* root = (sXf_EB98*)&bones[chain[0]];
    sVec4W d = Sub_EB98(end->pos, root->pos);
    float ang = func_0031C128(Clamp_F3D8((Dot_EB98(d, d) - a * a - b * b) * 0.5f / (a * b), -1.0f, 1.0f));
    sXf_EB98 local;
    // x-first sVec4 here (the store order shows it); local.pos itself is an sVec4W.
    (t4Vector&)local.pos = sVec4(a, 0.0f, 0.0f, 1.0f);
    local.rot = AxisAngle_EB98(*(sVec4W*)&D_004FF160, ang - 1.5707963705062866f);
    sXf_EB98 t1 = Combine_EB98(*root, local);
    sVec4W dir = Sub_EB98(XformPoint_F3D8(t1, sVec4(b, 0.0f, 0.0f, 1.0f)), root->pos);
    sVec4W axis = Cross_EB98(Normalize_F3D8(dir), Normalize_F3D8(d));
    float len = Length_EB98(axis);
    if (len > 9.999999747378752e-06f) {
        DivEq_EB98(axis, len);
        sVec4W q;
        q = AxisAngle_EB98(axis, func_0031C128(len));
        root->rot = QuatMul_EB98(q, root->rot);
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
extern t4Vector D_004FF130;
extern t4Vector D_004FF140;
extern t4Vector D_004FF150;
extern "C" void func_0031BE50(float* sout, float* cout, float x);

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
    func_0031BE50(&s, &c, angle * 0.5f);
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
                      D_004FF130);
    {
        sVec4XW v = Rot_LP(xf->rot, d);
        AddToL_LP(xf->pos, v);
        xf->rot *= AxisAngle_LP(D_004FF140, a * ka);
    }
    QuatMulTo_LP(xf->rot, AxisAngle_LP(D_004FF150, b * kb));
    sVec4XW w = Scale_LP(d, -1.0f);
    sVec4XW u = Rot_LP(xf->rot, w);
    AddTo_LP(xf->pos, u);
}

extern "C" void cRider_doLeanPoseAdjust_x(char* self, sXf_LP* xf) __asm__("cRider_doLeanPoseAdjust");
extern "C" void cRider_doLeanPoseAdjust_x(char* self, sXf_LP* xf)
{
    float a = deadzone_LP(*(float*)(self + 0x208), *(float*)(self + 0x214));
    float b = deadzone_LP(*(float*)(self + 0x1F0), *(float*)(self + 0x214));
    if (b != 0.0f || a != 0.0f)
        LeanAbout_LP(self, xf, a, b, 0.523598849773407f, 0.7853982448577881f);
    float k = *(float*)(self + 0x250);
    if (k != 0.0f) {
        QuatMulTo_LP(xf->rot, AxisAngle_LP(D_004FF140, k * 0.7853982448577881f));
    }
    sVec4XW g = Scale_LP(*(sVec4XW*)(self + 0x3B0), *(float*)(*(char**)(self + 0x780) + 0x140) * (*(float*)(self + 0x1F0) * 45.0f));
    AddTo_LP(xf->pos, g);
}
#endif

extern "C" void* cRiderHandler__setMotionState(void* obj);

//100%
INCLUDE_ASM("ai/rider", cRider_setMotionState__FPv);
#ifdef SKIP_ASM
void* cRider_setMotionState(void* vself)
{
    cRider* self = (cRider*)vself;
    return cRiderHandler__setMotionState(self->mRiderHandler);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011FE98__FPv);
#ifdef SKIP_ASM
int func_0011FE98(void* vself)
{
    cRider* self = (cRider*)vself;
    return *(int*)((char*)self->mRiderHandler + 0xDE0);
}
#endif

extern "C" void* func_00111538(void* obj);

//100%
INCLUDE_ASM("ai/rider", func_0011FEA8);
#ifdef SKIP_ASM
extern "C" void func_00111830(int);

extern "C" void func_0011FEA8(void *arg0) {
    func_00111830((*(int *)((char*)(arg0) + (0x77C))));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011FEC8__FPv);
#ifdef SKIP_ASM
void* func_0011FEC8(void* vself)
{
    cRider* self = (cRider*)vself;
    return func_00111538(self->mRiderHandler);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011FEE8__FPv);
#ifdef SKIP_ASM
int func_0011FEE8(void* vself)
{
    cRider* self = (cRider*)vself;
    return *(int*)((char*)self->mRiderHandler + 0xDE4);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_0011FEF8);
#ifdef SKIP_ASM
extern "C" void func_00111860(int);

extern "C" void func_0011FEF8(void *arg0) {
    func_00111860((*(int *)((char*)(arg0) + (0x77C))));
}
#endif
//100%
INCLUDE_ASM("ai/rider", func_0011FF18);
#ifdef SKIP_ASM
struct sVEntry0011FF18 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

// Forwards its second argument to vtable slot 1 of the rider handler.
extern "C" void func_0011FF18(cRider* self, void* out)
{
    char* obj = (char*)self->mRiderHandler;
    sVEntry0011FF18* vt = *(sVEntry0011FF18**)(obj + 0xDE8);
    vt[1].fn(obj + vt[1].delta, out);
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getHipPosition);
#ifdef SKIP_ASM
// 0x20-byte elements with the quadword at offset 0
struct sRiderXform {
    cQuad128 q;
    char pad_0x10[0x10];
};

// Copies the quadword of the anim model's transform unk_89C into dst.
extern "C" void* cRider_getHipPosition(void* dst, cRider* self)
{
    cAnimModel* q = self->mAnimModel;
    int i = self->mBoneHips;
    sRiderXform* b = (sRiderXform*)q->bones;
    *(cQuad128*)dst = b[i].q;
    return dst;
}
#endif

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBECharacterInterface_getWeight(void* iface, int character);
extern "C" float cBEStatInterface_getCollisionAttrib(void* iface, int character, int stat);

//100%
INCLUDE_ASM("ai/rider", func_0011FF70);
#ifdef SKIP_ASM
// Copies the quadword of the anim model's transform unk_8A8 into dst (sibling of cRider_getHipPosition).
extern "C" void* func_0011FF70(void* dst, cRider* self)
{
    cAnimModel* q = self->mAnimModel;
    int i = self->mBoneHead;
    sRiderXform* b = (sRiderXform*)q->bones;
    *(cQuad128*)dst = b[i].q;
    return dst;
}
#endif

//100%
INCLUDE_ASM("ai/rider", cRider_getMass);
#ifdef SKIP_ASM
extern "C" float cRider_getMass(cRider* self)
{
    int weight = cBECharacterInterface_getWeight(cBE_getInterface(cBE_getBE(), 2), self->mAIIndex);
    float toughness = cBEStatInterface_getCollisionAttrib(cBE_getInterface(cBE_getBE(), 3),
                                                          self->mAIIndex, self->mStatsIndex);

    return (float)weight * (toughness * 1.5003352165222168f + 1.0f) * (self->mBoostStrength * 10.0f + 1.0f);
}
#endif

extern "C" float func_00149690(void* iface, int character, int stat);

// Grab playback speed: 1 + grab stat * 0.2998.
//100%
INCLUDE_ASM("ai/rider", func_00120038);
#ifdef SKIP_ASM
extern "C" float func_00120038(cRider* self)
{
    void* iface = cBE_getInterface(cBE_getBE(), 3);
    return func_00149690(iface, self->mAIIndex, self->mStatsIndex) * 0.29988324642181396f + 1.0f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120090);
#ifdef SKIP_ASM
extern "C" void func_00120090(cRider* self, float x)
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
INCLUDE_ASM("ai/rider", func_001200D0);
#ifdef SKIP_ASM
extern "C" void func_0010E028(void* self, int mode, float t);

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

extern "C" void func_001200D0(cRider* self)
{
    float step = self->mTimeScale * 0.01666666753590107f;
    self->mPowerUpBoostTimer = decay_1200D0(self->mPowerUpBoostTimer, step);
    if (0.01666666753590107f < self->mPowerUpTrickTimer || func_0011FE98(self) != 1) {
        self->mPowerUpTrickTimer = decay_1200D0(self->mPowerUpTrickTimer, step);
    }
    char* sub = (char*)&self->mVtbl;
    sVt_1200D0* e = &(*(sVt_1200D0**)sub)[kRiderVt_isComputer];
    e->fn(sub + e->delta);
    if ((func_0011FE98(self) == 1 || func_0011FEE8(self) == 12) && 0.0f < self->mUberTimer && self->mUberTimer <= 0.01666666753590107f) {
        self->mUberTimer = 0.01666666753590107f;
    } else if (0.0f < self->mUberTimer) {
        self->mUberTimer = decay_1200D0(self->mUberTimer, step);
        if (self->mUberTimer == 0.0f) {
            func_0010E028(self, 6, 0.0f);
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
            if (func_0011FEE8(self) != 6) {
                self->unk_2F8 = decay_1200D0(self->unk_2F8, step * 0.004999999888241291f);
            }
            break;
        case 2:
            if (func_0011FEE8(self) != 6) {
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
    sV_120378 rest;      // 0x10 offset quaternion (unk_320 == 0)
    sV_120378 restAlt;   // 0x20 offset quaternion (unk_320 != 0)
};

int func_0011FEE8(void*);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" int func_001298C8();
int func_00312AA0(void*, int);

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
        int cur = func_00312AA0(self->mRiderAnim, layer);                                           \
        int anim;                                                                                   \
        if (still) {                                                                                \
            anim = (base);                                                                          \
        } else {                                                                                    \
            if (self->unk_320) {                                                                    \
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

// Sets the look-layer anim speed from the rider's speed and (on its frame) picks the head/body look anims.
extern "C" void func_00120378(void* vself)
{
    cRider* self = (cRider*)vself;
    float sideLimit = 416.66668701171875f;
    float upLimit = 888.888916015625f;
    sV_120378 v = Sub_120378(*(sV_120378*)&self->mVelocity, *(sV_120378*)&self->unk_3D0);
    float speed = Length_120378(v);
    float rate = Clamp_120378(speed / 1666.666748046875f, 0.5f, 2.0f);
    if (func_00311B20(self->mRiderAnim, 3)) {
        *(float*)(func_00311B20(self->mRiderAnim, 3) + 0x90) = rate;
    }
    if (func_00311B20(self->mRiderAnim, 4)) {
        *(float*)(func_00311B20(self->mRiderAnim, 4) + 0x90) = rate;
    }
    if (func_00311B20(self->mRiderAnim, 5)) {
        *(float*)(func_00311B20(self->mRiderAnim, 5) + 0x90) = rate;
    }
    if (func_001298C8() % 6 == self->mAIIndex) {
        int still = speed <= 138.88888549804688f;
        if (func_0011FEE8(self) == 11) {
            still = 0;
            AddEq_120378(v, sV_120378(0.0f, 0.0f, 1000.0f, 0.0f));
        }
        sV_120378 q;
        LOOK_120378(3, self->mBoneHead, (sLook_120378*)((char*)self + 0x940), 0x19B)
        LOOK_120378(4, *(int*)((char*)self + 0x8AC), (sLook_120378*)((char*)self + 0x970), 0x1A2)
        LOOK_120378(5, self->mBoneHips, (sLook_120378*)((char*)self + 0x9A0), 0x1A9)
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120D58);
#ifdef SKIP_ASM
extern "C" void func_00120D58(cRider* self)
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
INCLUDE_ASM("ai/rider", func_00120D90);
#ifdef SKIP_ASM
extern "C" int func_00311AE8(void*, int);

static inline float riderApproach_00120D90(float v, float t, float s)
{
    if (v > t + s)
        return v - s;
    if (v < t - s)
        return v + s;
    return t;
}

extern "C" void func_00120D90(cRider* self)
{
    int a = func_00311AE8(self->mRiderAnim, 2);
    if (a >= 0x14 && a <= 0x1A)
        self->unk_318 = riderApproach_00120D90(self->unk_318, 0.0f, 0.05000000447034836f);
    else
        self->unk_318 = riderApproach_00120D90(self->unk_318, 1.0f, 0.05000000447034836f);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120E30);
#ifdef SKIP_ASM
extern "C" void func_00120E30(cRider* self)
{
    void* p = self->mFocusBox;
    if (p != 0) {
        *(cQuad128*)p = *(cQuad128*)&self->mPosition;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120E50);
#ifdef SKIP_ASM
extern "C" void func_00332DB8(void*, void*);
extern "C" void func_00122088(cRider*);

extern "C" void func_00120E50(cRider* self)
{
    func_00332DB8(self->mOverlapList, &self->mBoundsMin);
    func_00122088(self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120E88);
#ifdef SKIP_ASM
extern "C" void func_002F1A08(void*);
extern "C" void func_002ECF78(void*);

extern "C" void func_00120E88(cRider* self)
{
    func_002F1A08(self->mAttachedEmitterFX);
    func_002ECF78(self->mBoltOnFX);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120EB8);
#ifdef SKIP_ASM
extern "C" void func_00111948(int);

extern "C" void func_00120EB8(void *arg0) {
    func_00111948((*(int *)((char*)(arg0) + (0x77C))));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120ED8);
#ifdef SKIP_ASM
extern "C" void func_0011EB98(void* self);

extern "C" void func_00120ED8(cRider* self)
{
    cRider_updateOrientationImplicit(self);
    if (self->mIsInNIS == 0)
    {
        func_0011EB60(self, 1.0f);
        func_0011EB98(self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00120F20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_001298C8();
extern "C" int func_001448A8(void* obj);
extern "C" void func_00332DB8(void*, void*);
extern "C" void func_00122088(cRider*);
extern "C" void func_001242B0(void* self, void* p);
extern "C" void func_0011B3F8(void* self);
extern void* D_004A28A8;

struct sQuad_120F20
{
    float v[4];
} __attribute__((aligned(16)));
// D_004FF120 is declared later in the unit as a 4-aligned sVec4; this view gives the lq/sq copy.
extern sQuad_120F20 D_004FF120_q120F20 __asm__("D_004FF120");

static inline int IsState1_120F20(void* obj)
{
    return *(int*)((char*)obj + 0x70) == 1;
}

extern "C" void func_00120F20(cRider* self)
{
    self->unk_3FC = 0;
    self->unk_AA4 = 0;
    int a = func_001298C8() % 3;
    if (a != self->mAIIndex % 3)
    {
        char* g = *(char**)((char*)D_004A28A8 + 0x84);
        if (*(int*)((char*)self->mOverlapList + 4) == *(int*)(*(char**)(g + 0x20) + 0xA0))
        {
            void* obj = *(void**)(g + 0x34);
            int busy = 0;
            if (func_001448A8(obj) || IsState1_120F20(obj))
                busy = 1;
            if (busy == 0)
                goto skip;
        }
    }
    func_00332DB8(self->mOverlapList, &self->mBoundsMin);
    func_00122088(self);
skip:
    if (self->mIsInNIS)
    {
        char* p = (char*)&self->mPosition;
        if (self->unk_AFC)
            p = (char*)self->mAnimModel->bones + (self->mBoneBoardRoot << 5);
        func_001242B0(self, p);
    }
    *(sQuad_120F20*)&self->mModelOffset = D_004FF120_q120F20;
    self->unk_A30 = 0;
    self->mVisitedIds = 0xFFFFFFFF;
    func_0011B3F8(self);
    float t = self->mFinishTime;
    if (t >= 0.0f)
        self->mFinishTime = t + 0.01666666753590107f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121068);
#ifdef SKIP_ASM
struct sVEntry00121068 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

extern "C" void func_00111728(void* obj, void* v);

extern "C" void func_00121068(cRider* self)
{
    int buf[4];
    char* obj = (char*)self->mRiderHandler;
    sVEntry00121068* vt = *(sVEntry00121068**)(obj + 0xDE8);
    vt[1].fn(obj + vt[1].delta, buf);
    func_00111728(self->mRiderHandler, buf);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_001210B0);
#ifdef SKIP_ASM
extern "C" void cRider_checkForReset(void* rider, int a, int b);
extern "C" void func_001200D0(cRider* self);
extern "C" void func_00120D90(cRider* self);

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

extern "C" void func_001210B0(cRider* self)
{
    VecScale_1210B0((sVec4_1210B0*)&self->mImpulse, 0.9800000190734863f);
    self->mImpactAccum *= 0.9559999704360962f;
    self->mImpactAccum2 *= 0.9783333539962769f;
    if (self->mImpactAccum > 4.5f)
    {
        cRider_checkForReset(self, 0, 2);
    }
    else if (func_0011FE98(self) == 1 ||
             (func_0011FE98(self) == 2 && *(int*)((char*)self->mRiderHandler + 0x30) == 1))
    {
        char* obj = (char*)self->mAirPredictor;
        int ok = 0;
        if (*(int*)(obj + 0xAC) == 1 || *(int*)(obj + 0xAC) == 3)
            ok = 1;
        obj = (char*)self->mAirPredictor;
        if ((ok && *(float*)(obj + 0x98) - *(float*)(obj + 0xA0) > 45.0f) || self->mImpactAccum2 > 5.0f)
            cRider_checkForReset(self, 0, 2);
    }
    func_001200D0(self);
    func_00120D90(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_001211F8);
#ifdef SKIP_ASM
extern "C" void func_00120378(void* self);

static inline void approach_11F8(sRiderChannel* t)
{
    float res;
    float target = t->target;
    float rate = t->rate;
    float cur = t->cur;
    if (target + rate < cur)
        res = cur - rate;
    else if (cur < target - rate)
        res = cur + rate;
    else
        res = target;
    t->cur = res;
}

extern "C" void func_001211F8(cRider* self)
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

extern "C" void* func_00111408(void* obj);

//100%
INCLUDE_ASM("ai/rider", func_001216E0__FPv);
#ifdef SKIP_ASM
void* func_001216E0(void* vself)
{
    cRider* self = (cRider*)vself;
    return func_00111408(self->mRiderHandler);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121700);
#ifdef SKIP_ASM
extern "C" void func_0011EB60(cRider* self, float value);

extern "C" void func_00121700(cRider* self)
{
    if (self->mIsInNIS == 0)
    {
        func_0011EB60(self, self->mTimeScale);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121728);
#ifdef SKIP_ASM
extern "C" void func_0011EB98(void* self);

extern "C" void func_00121728(cRider* self)
{
    if (self->mIsInNIS == 0)
    {
        func_0011EB98(self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121750);
#ifdef SKIP_ASM
extern "C" void func_001114A0(void* p);
extern "C" void func_00310530(void* model, void* v);

struct sVec4_00121750
{
    float x, y, z, w;
};
extern sVec4_00121750 D_004FF120;

static inline int VecChanged_00121750(cRider* self, const sVec4_00121750& b)
{
    return self->mModelOffset.x != b.x || self->mModelOffset.y != b.y
        || self->mModelOffset.z != b.z || self->mModelOffset.w != b.w;
}

extern "C" void func_00121750(cRider* self)
{
    func_001114A0(self->mRiderHandler);
    if (VecChanged_00121750(self, D_004FF120))
    {
        func_00310530(self->mAnimModel, &self->mModelOffset);
    }
}
#endif

extern "C" void* func_003103F0(void* obj);

//100%
INCLUDE_ASM("ai/rider", func_001217F8__FPv);
#ifdef SKIP_ASM
void* func_001217F8(void* vself)
{
    cRider* self = (cRider*)vself;
    return func_003103F0(self->mAnimModel);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121818);
#ifdef SKIP_ASM
extern void* D_004A3DD8;
extern "C" void func_0030A060(void* mgr, void* a, void* b, void* c);
void func_00125AD0(void* self);
extern "C" void func_00112338(void* self);
extern "C" void func_001125C0(void* self);
extern "C" int func_00125228(cRider* self);
extern "C" void func_00117C28(void* metrix);

struct sVEntry00121818 { short delta; short index; void (*fn)(void*, void*, void*, void*); };

extern "C" void func_00121818(cRider* self)
{
    char* h = (char*)self->unk_A30;
    if (h != 0)
    {
        char* obj = *(char**)(h + 0xC);
        if (obj != 0)
        {
            sVEntry00121818* vt = *(sVEntry00121818**)(obj + 0xC);
            vt[40].fn(obj + vt[40].delta, self->unk_A60, self->unk_9E0, (char*)&self->mVtbl);
        }
        else
        {
            func_0030A060(D_004A3DD8, self->unk_A60, self->unk_9E0, (char*)&self->mVtbl);
        }
    }
    func_00125AD0(self);
    if (self->mIsInNIS == 0)
    {
        func_00112338(self);
    }
    func_001125C0(self);
    func_00125228(self);
    if (self->mIsInNIS == 0)
    {
        func_00117C28(self->mMetrix);
    }
    func_00120E30(self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001218D0);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" int func_0022E0E0(void* self, int a1);
extern "C" void func_002ED490(void* obj, float a, float b, float c);
extern "C" void func_00392D18(void* p);

struct sRiderVE_001218D0
{
    short delta;
    short index;
    void* (*fn)(void*);
};

extern "C" void func_001218D0(cRider* self)
{
    if (~self->mGroundPolyId != 0)
        self->mGroundMaterial = func_0022E0E0(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x78), (unsigned char)self->mGroundPolyId);
    char* obj = (char*)&self->mVtbl;
    sRiderVE_001218D0* vt = *(sRiderVE_001218D0**)obj;
    func_002ED490(vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta), self->mGroundContactPoint.x, self->mGroundContactPoint.y, -99999.0f);
    char* p = (char*)self->mRiderHandler;
    func_00392D18(p ? p + 0xD30 : 0);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121950);
#ifdef SKIP_ASM
extern "C" int func_001231A8(cRider* self);
extern "C" void* func_0028B180();
extern "C" void func_002898A8(void* mgr, void* rider);
extern "C" float func_0031C228(float x);
extern "C" void func_00122CF0(cRider* self);

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

extern "C" void func_00121950(cRider* self)
{
    func_002898A8(func_0028B180(), self);
    if (func_001231A8(self))
    {
        sVec2_121950 v(self->mGroundNormal.x, self->mGroundNormal.y);
        float z = self->mGroundNormal.z;
        float ang = Atan2_121950(Length_121950(v), z);
        float d = ang - self->mSmoothedHeading;
        if (d > 3.1415927410125732f)
            d -= 6.2831854820251465f;
        else if (d < -3.1415927410125732f)
            d += 6.2831854820251465f;
        self->mSmoothedHeading += d * 0.15000000596046448f;
    }
    func_00122CF0(self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121AA0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

// 16-byte vectors with inline constructors (non-POD siblings of t4Vector). The classes differ only in the order
// their 4-float constructor stores the components; that order decides the float-register allocation of every
// caller (merging them changes code), so they are kept apart.
// sVec4 stores x, y, z, w.
// sVec4W stores w first, then x, y, z.
extern t4Vector D_004FF120_1AA0 __asm__("D_004FF120");
extern t4Vector D_004FF140;
extern "C" void func_0031BB30(void* q, const void* a, const void* b);
extern "C" float func_0031C128(float x);
extern "C" float func_0031BF60(float x);


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
    func_0031BE50(&s, &c, angle * 0.5f);
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

extern "C" void func_00121AA0(cRider* self, const sVec4* up, const sVec4* fwd, float rate, float maxRate)
{
    sVec4W q = ROT_1AA0;
    {
        sVec4 cur(2.0f * (ROT_1AA0.z * ROT_1AA0.x + ROT_1AA0.w * ROT_1AA0.y), 2.0f * (ROT_1AA0.z * ROT_1AA0.y - ROT_1AA0.w * ROT_1AA0.x),
                    1.0f - 2.0f * (ROT_1AA0.x * ROT_1AA0.x + ROT_1AA0.y * ROT_1AA0.y), 0.0f);
        func_0031BB30(&q, &cur, up);
    }
    if (neq_1AA0(fwd, D_004FF120_1AA0)) {
        sVec4 v = QuatRot_1AA0(q, *fwd);
        float heading = -Atan2_1AA0(v.x, v.y);
        Rotate_1AA0(q, *up, heading);
    }
    sVec4W d = QuatMul_1AA0(q, Conj_1AA0(self));
    float angle;
    t4Vector axis;
    if (0.0f < d.w) {
        angle = 2.0f * Acos_1AA0(d.w <? 1.0f);
        if (9.999999747378752e-06f <= angle) {
            axis = Div_1AA0(sVec4(d.x, d.y, d.z, 0.0f), func_0031BF60(angle * 0.5f));
            goto have_axis;
        }
    } else {
        angle = 2.0f * Acos_1AA0(-(d.w >? -1.0f));
        if (9.999999747378752e-06f <= angle) {
            axis = Div_1AA0(sVec4(d.x, d.y, d.z, 0.0f), func_0031BF60(angle * -0.5f));
            goto have_axis;
        }
    }
    axis = D_004FF140;
have_axis:
    angle = (maxRate <? angle * rate) * 0.01666666753590107f;
    d = AxisAngle_1AA0(axis, angle);
    ROT_1AA0 = QuatMul_1AA0(d, ROT_1AA0);
    cRider_updateOrientationImplicit(self);
}
#undef ROT_1AA0
#endif

//100%
INCLUDE_ASM("ai/rider", func_00121F30);
#ifdef SKIP_ASM
extern void* D_004A289C;
extern "C" void func_0031BE50(float* s, float* c, float angle);

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

extern "C" void func_00121F30(cRider* self)
{
    void* g = D_004A289C;
    sVEntryP_121F30* gvt = *(sVEntryP_121F30**)((char*)g + 0x10D8);
    sVec4_121F30* m = (sVec4_121F30*)gvt[35].fn((char*)g + gvt[35].delta);
    char* obj = (char*)&self->mVtbl;
    sVEntryP_121F30* vt = *(sVEntryP_121F30**)obj;
    sVec4_121F30* pos = (sVec4_121F30*)vt[kRiderVt_getPosition].fn(obj + vt[kRiderVt_getPosition].delta);
    sVec4_121F30 v = mtxMulVec_121F30(m, pos);
    float z = v.z;
    void* g2 = D_004A289C;
    sVEntryF_121F30* gvt2 = *(sVEntryF_121F30**)((char*)g2 + 0x10D8);
    float fov = gvt2[29].fn((char*)g2 + gvt2[29].delta);
    float s, c;
    func_0031BE50(&s, &c, fov);
    z = z * (s / c);
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
// Builds a 0xA0-byte record with func_00389C38 and passes it to virtual slot 69 of the object at D_004A289C.
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

extern "C" void func_00122048()
{
    char rec[0xA0];
    func_00389C38(rec, 0);
    ((cRiderSys_122048*)D_004A289C)->v69(rec, 0);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122088);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void func_002F5B68(void* a, void* q, void* b, void* c, int d);

extern "C" void func_00122088(cRider* self)
{
    cQuad128 q;
    cRider_getHipPosition(&q, self);
    func_002F5B68(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x90), &q, self->mOverlapList, self->unk_794, 8);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001220D8);
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

extern "C" void func_001220D8(cRider* self)
{
    sQ_20D8 q;
    func_00389260(self->unk_7C0);
    {
        char* obj = (char*)&self->mVtbl;
        sVE_20D8* vt = *(sVE_20D8**)obj;
        *(sXf_20D8*)&self->unk_7C0[0] = *(sXf_20D8*)func_002EDFB8(vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta));
    }
    cRider_getHipPosition(&q, self);
    char* m = self->unk_7C0;
    {
        char* obj = (char*)&self->mVtbl;
        sVE_20D8* vt = *(sVE_20D8**)obj;
        func_00389CB8(m, &q, func_002EEFA8(vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta)));
    }
    for (int i = 0; i < 8; i++) {
        void* p = self->unk_794[i];
        if (p != 0)
            func_0038A6A8(self->unk_7C0, &q, p);
    }
    char* x = (char*)self->mRiderHandler;
    sPath_20D8* t = x ? (sPath_20D8*)(x + 0xD30) : 0;
    if (t != 0) {
        {
            sV3_20D8 v(t->x, t->y, t->z);
            func_00389558(self->unk_7C0, &v);
        }
        for (int j = 0; j < t->n; j++) {
            sQ_20D8* qq = &t->q[j];
            sV3_20D8 v(t->pts[j].x, t->pts[j].y, t->pts[j].z);
            func_00389520(self->unk_7C0, qq, &v);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122278);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallmsr/cfc2); the PC port needs a C version.
extern void* D_004A289C;
extern void* D_004A28A8;
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
    char* inst = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x8C);
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
    char* g = (char*)D_004A289C;
    sVE_122278* vt = *(sVE_122278**)(g + 0x10D8);
    char* obj = g + vt[93].delta;
    sMat_122278 m = ((GetMatFn_122278)vt[43].fn)(g + vt[43].delta);
    return ((TestFn_122278)vt[93].fn)(obj, a, b, &m);
}

extern "C" int func_00122278(cRider* self)
{
    if (self->mRenderFlag == 0 || self->mLoadStep != 7 || self->unk_AD0 != 0) {
        self->mIsOnScreen = 0;
        return 0;
    }
    self->mIsOnScreen = test_122278(&self->mBoundsMin, &self->mBoundsMax) != 1;
    if (self->mUseRootPoseOverride != 0) {
        if (self->mIsOnScreen == 0) {
            char* s = (char*)self->mRiderHandler;
            self->mIsOnScreen = test_122278(s + 0x80, s + 0x90) != 1;
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
INCLUDE_ASM("ai/rider", func_00122448);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_004A289C;
extern void* D_004A28A8;
extern "C" void func_00121F30(cRider* self);
extern "C" void func_00310640(void* model, int lod, int a, int b);

struct sVec4_122448 { float v[4]; };
extern sVec4_122448 D_004FF120_v122448 __asm__("D_004FF120");

struct sVEntryI_122448 { short delta; short index; void (*fn)(void*, void*, int); };
struct sVEntryF_122448 { short delta; short index; void (*fn)(void*, float, void*); };

extern "C" void func_00122448(cRider* self)
{
    if (self->mIsOnScreen)
    {
        char* p = (char*)self->mRiderHandler;
        char* q = p ? p + 0xD30 : 0;
        int b = 0;
        if (*(int*)(q + 0xA0) || self->mUberLevel >= 11)
            b = 1;
        int a = 0;
        if (*(int*)(q + 0xA4) || self->mUberLevel >= 11)
            a = 1;
        func_00121F30(self);
        void* g = D_004A289C;
        sVEntryI_122448* vt = *(sVEntryI_122448**)((char*)g + 0x10D8);
        vt[69].fn((char*)g + vt[69].delta, self->unk_7C0, 1);
        void* g2 = D_004A289C;
        int* rs = *(int**)((char*)g2 + 0xE84);
        rs[1] = (rs[1] & ~0x7C) | 0x14;
        sVEntryF_122448* vt2 = *(sVEntryF_122448**)((char*)g2 + 0x10D8);
        vt2[27].fn((char*)g2 + vt2[27].delta, 0.0f, &self->mPosition);
        func_00310640(self->mAnimModel, self->mLOD, a, b);
        void* g3 = D_004A289C;
        sVEntryF_122448* vt3 = *(sVEntryF_122448**)((char*)g3 + 0x10D8);
        vt3[27].fn((char*)g3 + vt3[27].delta, 0.0f, &D_004FF120_v122448);
    }
    if (*(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84) + 0x14) == 0 || self->unk_B1C)
    {
        int* f = &self->unk_B1C;
        int v = 0;
        if (self->mIsOnScreen == 0 || self->mLOD == 2)
            v = 1;
        *f = v;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_001225C0);
#ifdef SKIP_ASM
extern "C" void func_001220D8(cRider*);

extern "C" int func_001225C0(cRider* self)
{
    if (self->mIsOnScreen != 0) {
        func_001220D8(self);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001225F0);
#ifdef SKIP_ASM
extern "C" void func_00122898(cRider* self);

extern "C" void func_001225F0(cRider* self)
{
    if (self->mIsOnScreen != 0)
    {
        func_00122898(self);
    }
}
#endif

extern "C" void* func_002F2088(void* obj);

//100%
INCLUDE_ASM("ai/rider", func_00122618);
#ifdef SKIP_ASM
extern "C" void func_001119F8(int);

extern "C" void func_00122618(void *arg0) {
    func_001119F8((*(int *)((char*)(arg0) + (0x77C))));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122638__FPv);
#ifdef SKIP_ASM
void* func_00122638(void* vself)
{
    cRider* self = (cRider*)vself;
    return func_002F2088(self->mAttachedEmitterFX);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122658);
#ifdef SKIP_ASM
// MEM_fill looks like memset(dst, value, size).
extern "C" void* MEM_fill(void* dst, int value, int size);

extern "C" void func_00122658(cRider* self)
{
    MEM_fill(self->unk_794, 0, sizeof(self->unk_794));
}
#endif

INCLUDE_ASM("ai/rider", func_00122680);

//100%
INCLUDE_ASM("ai/rider", func_00122898);
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

extern void* D_004A289C;
extern int D_004A4C00;
extern sV3_2898 D_004A5E40;

extern "C" void func_00374D00(void* obj, sV3_2898* a, sV3_2898* b, sV3_2898* dir, sMat_2898* bones, int f);
extern "C" void func_00310948(void* model, int lod, sV4_2898* a, sV4_2898* b);
extern "C" void func_00375890(void* obj);
int func_0011FE98(void*);

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

static inline cAnimModel* Model_2898(cRider* r) { return r->mAnimModel; }
static inline sObj_2898* Shadow_2898(cRider* r) { return (sObj_2898*)r->mShadow; }
// Translation row of bone i's 0x40-byte matrix.
static inline sV4_2898 BonePos_2898(cRider* r, int i) { return *(sV4_2898*)((char*)Model_2898(r)->unk_030 + (i << 6) + 0x30); }

static inline void* Tex_2898(sObj_2898* o) { return o->f170; }

static inline sRS_2898*& top_2898(void* g) { return *(sRS_2898**)((char*)g + 0xE84); }

extern "C" void func_00122898(cRider* self)
{
    if (self->unk_AA4 == 0) {
        self->unk_AA4 = 1;
        void* g = D_004A289C;
        top_2898(g)[1] = top_2898(g)[0];
        top_2898(g)++;
        sV3_2898 a;
        {
            sV4_2898 m = Scale_2898(Add_2898(BonePos_2898(self, self->unk_918), BonePos_2898(self, self->unk_8E8)), 0.5f);
            a.x = m.x;
            a.y = m.y;
            a.z = m.z;
        }
        if (D_004A4C00 == 0) {
            Set3_2898(D_004A5E40, 0.0f, 0.0f, -1.0f);
            D_004A4C00 = 1;
        }
        sV3_2898 b;
        {
            sV4_2898 p = *(sV4_2898*)&self->mMatrix.row[2];
            b.x = p.x;
            b.y = p.y;
            b.z = p.z;
        }
        float k = 0.3f;
        func_00374D00(Shadow_2898(self), &a, &b, &D_004A5E40, (sMat_2898*)Model_2898(self)->unk_030, 0);
        void* g2 = D_004A289C;
        top_2898(g2)->f8 = (top_2898(g2)->f8 & ~0x3E0) | 0x20;
        top_2898(g2)->f4 = (top_2898(g2)->f4 & 0xFE7FFFFF) | 0x1000000;
        top_2898(g2)->f4 = top_2898(g2)->f4 & 0xFFCFFFFF;
        top_2898(g2)->f4 = (top_2898(g2)->f4 & 0xFFF00FFF) | 0x14000;
        top_2898(g2)->f4 = (top_2898(g2)->f4 & 0xFFBFFFFF) | 0x400000;
        top_2898(g2)->f4 = (top_2898(g2)->f4 & ~0x7C) | 4;
        sVE0_2898* vt = *(sVE0_2898**)((char*)g2 + 0x10D8);
        vt[31].fn((char*)g2 + vt[31].delta);
        void* g3 = D_004A289C;
        sVE2_2898* vt3 = *(sVE2_2898**)((char*)g3 + 0x10D8);
        vt3[53].fn((char*)g3 + vt3[53].delta, Shadow_2898(self)->f170, 1);
        void* g4 = D_004A289C;
        sVEI3_2898* vt4 = *(sVEI3_2898**)((char*)g4 + 0x10D8);
        vt4[55].fn((char*)g4 + vt4[55].delta, Tex_2898(Shadow_2898(self)), sV4_2898(0.0f, 0.0f, 0.0f, 0.0f));
        if (func_0011FE98(self) == 0) {
            k = 0.0f;
        }
        sMat_2898 mat;
        CopyMat_2898(&mat, &Shadow_2898(self)->mat);
        sV4_2898 p0 = Shadow_2898(self)->f150;
        sV4_2898 p1 = Shadow_2898(self)->f160;
        void* g5 = D_004A289C;
        sVE4_2898* vt5 = *(sVE4_2898**)((char*)g5 + 0x10D8);
        vt5[101].fn((char*)g5 + vt5[101].delta, &mat, &p0, &p1);
        sV4_2898 c1;
        c1.x = 1.0f;
        c1.y = k;
        c1.z = k;
        c1.w = k;
        sV4_2898 c2;
        c2.x = 1.0f;
        c2.y = 0.3f;
        c2.z = 0.3f;
        c2.w = 0.3f;
        func_00310948(Model_2898(self), 3, &c1, &c2);
        void* g6 = D_004A289C;
        sVEI_2898* vt6 = *(sVEI_2898**)((char*)g6 + 0x10D8);
        vt6[54].fn((char*)g6 + vt6[54].delta);
        void* g7 = D_004A289C;
        sVE0_2898* vt7 = *(sVE0_2898**)((char*)g7 + 0x10D8);
        vt7[32].fn((char*)g7 + vt7[32].delta);
        void* g8 = D_004A289C;
        top_2898(g8)--;
    }
    func_00375890(Shadow_2898(self));
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122C28);
#ifdef SKIP_ASM
extern "C" void* func_0026B5E0(void*, int, int);
extern char D_004D33A0[];

extern "C" float* func_00122C28(float* out, cRider* rider)
{
    float* p = (float*)func_0026B5E0(D_004D33A0, 1, rider->mAIIndex);
    out[0] = p[2];
    out[1] = p[3];
    out[2] = p[4];
    out[3] = 1.0f;
    return out;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122C88);
#ifdef SKIP_ASM
extern "C" float func_00122C88(void) {
    return 0.92508197f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122C98);
#ifdef SKIP_ASM
extern "C" int func_00122C98(cRider* self)
{
    float v = self->mSmoothedHeading;
    float k = 0.9250819683074951f;
    return v >= k || v < -k;
}
#endif

extern "C" int func_001231A8(cRider* self);

//100%
INCLUDE_ASM("ai/rider", func_00122CD0__FPv);
#ifdef SKIP_ASM
void* func_00122CD0(void* self)
{
    return (void*)func_001231A8((cRider*)self);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122CF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void* D_004A28A8;
extern "C" int func_00101A10(void* self, int character);
extern "C" int func_00122C98(cRider* self);
float func_0011A0C0_2(void* self, int state) __asm__("func_0011A0C0__FPv");

extern "C" void func_00122CF0(cRider* self)
{
    int old = self->unk_5B0;
    int r = func_00101A10(*(void**)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 0xA8), self->mAIIndex);
    self->unk_5B0 = r;
    if (r == 0)
    {
        if (func_00122CD0(self) != 0)
        {
            if (func_00122C98(self))
                self->unk_5B0 = 1;
        }
    }
    int cur = self->unk_5B0;
    if (old != cur)
        func_0011A0C0_2(self->mMetrix, cur);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122D78);
#ifdef SKIP_ASM
float func_00113130(void* self);

struct sVEntry00122D78 { short delta; short index; int (*fn)(void*); };

extern "C" int func_00122D78(cRider* self)
{
    char* sub = (char*)&self->mVtbl;
    int n = *(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 0xC);
    float rate = (func_00113130(self) - self->mDistToFinish) / (float)n;
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
INCLUDE_ASM("ai/rider", func_00122E50);
#ifdef SKIP_ASM
float func_00113130(void* self);

extern "C" int func_00122E50(cRider* self)
{
    int n = *(int*)((char*)self->mMetrix + 0x198) + 1;
    float t = func_00113130(self) - self->mDistToFinish;
    if (t < 1.0f)
        t = 1.0f;
    float r = (float)n / t;
    float d = self->mDistToFinish - 1000.0f;
    if (d < 0.0f)
        d = 0.0f;
    return n + (int)(d * r);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00122EE8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_00135EB0(void* p);
extern "C" int func_00135EF0(void* p);
extern "C" int func_00135F30(void* p);
extern "C" int func_00311AE8(void* self, int i);
extern "C" int func_001446A0(void* self, int a1);

extern "C" int func_00122EE8(char* self, int q)
{
    int res;
    switch (q) {
    case 0:
        return func_0011FE98(self) == 2;
    case 1:
        return func_0011FEE8(self) == 9;
    case 2:
        {
            bool ok = false;
            if (func_0011FE98(self) == 1 || (func_0011FE98(self) == 2 && *(int*)(*(char**)(self + 0x77C) + 0x30) == 1)) {
                ok = true;
            }
            return ok;
        }
    case 3:
        return func_0011FE98(self) == 0;
    case 4:
        return func_0011FE98(self) == 4;
    case 5:
        res = 0;
        if (func_0011FEE8(self) == 5) {
            res = func_00135EB0(*(char**)(self + 0x77C) + 0x230) != 0;
        }
        return res;
    case 6:
        res = 0;
        if (func_0011FEE8(self) == 5) {
            res = func_00135EF0(*(char**)(self + 0x77C) + 0x230) != 0;
        }
        return res;
    case 7:
        res = 0;
        if (func_0011FEE8(self) == 5) {
            res = func_00135F30(*(char**)(self + 0x77C) + 0x230) != 0;
        }
        return res;
    case 9:
        return *(int*)(self + 0x330) == 1;
    case 10:
        return *(int*)(self + 0x330) == 2;
    case 11:
        return *(float*)(self + 0x2FC) != 0.0f;
    case 8:
        return *(int*)(*(char**)(self + 0x790) + 0x198);
    case 15:
        return *(int*)(*(char**)(self + 0x790) + 0x138);
    case 16:
        return *(int*)(*(char**)(self + 0x790) + 0x13C);
    case 17:
        return *(int*)(*(char**)(self + 0x790) + 0x19C);
    case 18:
        res = 0;
        if (func_00311AE8(*(void**)(self + 0x784), 1) == 13) {
            if (func_001446A0(func_00311B20(*(void**)(self + 0x784), 1) + 0xB0, 0)) {
                res = !func_001446A0(func_00311B20(*(void**)(self + 0x784), 1) + 0xB0, 1);
            }
        }
        return res;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00123128);
#ifdef SKIP_ASM
signed char cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int func_00123128(cRider* self)
{
    return cBENewPlayerInterface_getRiderCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mAIIndex);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00123168);
#ifdef SKIP_ASM
extern "C" int func_00147410(void* iface, int riderIndex);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int func_00123168(cRider* self)
{
    return func_00147410(cBE_getInterface_Fv(cBE_getBE(), 1), self->mAIIndex);
}
#endif

// Grounded predicate: motion 0, or motion 2 with owner+0x30 == 0.
//100%
INCLUDE_ASM("ai/rider", func_001231A8);
#ifdef SKIP_ASM
extern "C" int func_001231A8(cRider* self)
{
    int grounded = 0;

    if (func_0011FE98(self) == 0 ||
        (func_0011FE98(self) == 2 && *(int*)((char*)self->mRiderHandler + 0x30) == 0)) {
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

extern t4Vector D_004FF140;

// 16-byte vectors with inline constructors (non-POD siblings of t4Vector). The classes differ only in the order
// their 4-float constructor stores the components; that order decides the float-register allocation of every
// caller (merging them changes code), so they are kept apart.
// sVec4 stores x, y, z, w.
extern "C" void func_00270970(int a, int b);
extern "C" void func_00125038(cRider* self);
extern "C" void func_00111890(void* self);
extern "C" void func_002F1A00(void* a);
extern "C" void func_00119368(void* metrix, int flag);
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

void func_0011D660_3210(void* self, sVec4* a, sVec4* b, int n, float t) __asm__("func_0011D660");

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


// Inline wrapper: the vector addresses reach func_0011D660 through inline parameters,
// so they are not CSE'd with the if-chain's temporaries (the target recomputes sp+0x20).
static inline void SetPath_3210(void* self, sVec4* p, sVec4* d)
{
    func_0011D660_3210(self, p, d, 5, 0.0f);
}

extern "C" void cRider_transport_v(cRider* self, char* m) __asm__("cRider_transport");

extern "C" void cRider_transport_v(cRider* self, char* m)
{
    func_00270970(*(int*)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28), self->mAIIndex);
    sVec4 pos = *(sVec4*)(m + 0x30);
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
    off = Scale_3210(MatMulV_3210(m, off), 100.0f);
    AddEq_3210(pos, off);
    sVec4 dir = MatMulV_3210(m, D_004FF140);
    func_00119368(self->mMetrix, 1);
    cRider_setMotionState_impl(self, 0);
    func_0011FEC8_impl(self, 0);
    func_00125038(self);
    char* sub = (char*)&self->mVtbl;
    sVEntry<int(void*)>* vtA = *(sVEntry<int(void*)>**)sub;
    char* obj = *(char**)(*(char**)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84) + 0x4) + 0xA0);
    sVEntry<void(void*, int)>* vtB = *(sVEntry<void(void*, int)>**)(obj + 0x14);
    char* objB = obj + vtB[3].delta;
    vtB[3].fn(objB, vtA[kRiderVt_getAIIndex].fn(sub + vtA[kRiderVt_getAIIndex].delta));
    float len = Length_3210(*(sVec4*)&self->mVelocity);
    SetPath_3210(self, &pos, &dir);
    self->mVelocity = Scale_3210(sVec4(*(sVec4*)&self->mMatrix.row[1]), len);
    func_00111890(self->mRiderHandler);
    func_002F1A00(self->mAttachedEmitterFX);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_001234D0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.

extern t4Vector D_004FF140;
extern "C" void func_00270970(int a, int b);
extern "C" void func_00125038(cRider* self);
extern "C" void func_002F1A00(void* a);
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

void func_0011D660_34D0(void* self, t4Vector* a, t4Vector* b, int n, float t) __asm__("func_0011D660");

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


extern "C" void func_001234D0(cRider* self, char* m)
{
    t4Vector pos;
    t4Vector dir;
    func_00270970(*(int*)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28), self->mAIIndex);
    pos = *(t4Vector*)(m + 0x30);
    dir = MatMulV_34D0(m, D_004FF140);
    func_00119368(self->mMetrix, 1);
    cRider_setMotionState_impl(self, 3);
    func_0011FEC8_impl(self, 0xD);
    func_0011D660_34D0(self, &pos, &dir, 5, 0.0f);
    *(int*)((char*)self->mRiderHandler + 0x290) = 2;
    func_0011FEC8_impl(self, 6);
    func_00125038(self);
    func_00111890(self->mRiderHandler);
    func_002F1A00(self->mAttachedEmitterFX);
    if ((unsigned int)self->mViewIndex < 2) {
        char* obj = *(char**)(*(char**)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84) + (self->mViewIndex << 2) + 4) + 0xA8);
        sVEntry<void(void*)>* vt = *(sVEntry<void(void*)>**)(obj + 0x14);
        vt[4].fn(obj + vt[4].delta);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001235F8);
#ifdef SKIP_ASM
// PORT: prototype mismatch. The unit defines func_0011FEC8/cRider_setMotionState with one
// param, but their bodies forward $5 to func_00111538/cRiderHandler__setMotionState (int arg).
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");

extern "C" void func_001235F8(cRider* self)
{
    *(int*)((char*)self->mRiderHandler + 0x350) = 1;
    func_0011FEC8_impl(self, 9);
    cRider_setMotionState_impl(self, 3);
    *(int*)((char*)self->mRiderHandler + 0x290) = 0;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00123640);
#ifdef SKIP_ASM
extern "C" void func_0027F9F8(void* self);
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");
void cRider_setMotionState_impl(void* self, int v) __asm__("cRider_setMotionState__FPv");
extern "C" void cRiderAnimBase_reset(void* anim);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
int func_0027C958(void* self);
extern "C" void func_0027C9B0(int, void*);
struct sQuat_001241C0;
extern "C" void func_001241C0(cRider* self, sQuat_001241C0* out, float rx, float ry, float rz);
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" void cRider_updateOrientationImplicit(cRider* self);
extern "C" void func_00111890(void* self);
extern "C" void func_0030EA80(void* model, int i, int channel);
extern "C" void func_0030E9E0(void* model, int i, int channel);
void* func_0014BDB8_3640() __asm__("func_0014BDB8__FPv");
extern "C" void* func_0014AD50(void* iface, int id);
int func_0014D988(void* tbl, int g);
extern "C" void* func_0014D998(void* tbl, int g);
extern "C" int func_00123128(cRider* self);
extern "C" void func_00114130(void* rider, int a, int b);

struct sV_3640
{
    float x, y, z, w;
    sV_3640() {}
    sV_3640(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern sV_3640 D_004FF120_3640 __asm__("D_004FF120");
extern sV_3640 D_004FF160_3640 __asm__("D_004FF160");

static inline sV_3640 AxisAngle_3640(const sV_3640& axis, float angle)
{
    float s, c;
    func_0031BE50(&s, &c, angle * 0.5f);
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

extern "C" void func_00123640(cRider* self)
{
    int buf[12];
    func_0027F9F8(&self->mScriptObjRiderVtbl);
    func_0011FEC8_impl(self, 0xD);
    cRider_setMotionState_impl(self, 3);
    cRiderAnimBase_reset(self->mRiderAnim);
    cRiderAnimBase_play(self->mRiderAnim, 5, 0, -1.0f);
    self->mIsInNIS = 1;
    *(int*)((char*)self + 0x2D8) = *(int*)((char*)self + 0x320);
    self->unk_AC8 = 0;
    int j;
    for (j = 3; j >= 0; j--)
        self->unk_B08[j] = -1;
    float zero;
    float one = 1.0f;
    // PORT: func_0027C958 returns a record pointer as int
    char* rec = (char*)func_0027C958(self->unk_6D8);
    zero = 0.0f;
    self->unk_AFC = rec[7] != 0;
    self->unk_B00 = 1;
    self->unk_AD0 = rec[4] != 0;
    func_0027C9B0((int)self->unk_6D8, buf);
    float* fb = (float*)buf;
    sV_3640 q;
    q.x = fb[6];
    q.y = fb[7];
    q.z = fb[8];
    q.w = one;
    *(float*)((char*)self + 0xAF0) = fb[9] * 0.01745329424738884f;
    *(float*)((char*)self + 0xAF4) = fb[10] * 0.01745329424738884f;
    *(float*)((char*)self + 0xAF8) = fb[11] * 0.01745329424738884f;
    *(sV_3640*)((char*)self + 0xAE0) = q;
    q = MatMulV_3640((char*)self + 0x700, *(sV_3640*)((char*)self + 0xAE0));
    *(sV_3640*)&self->mPosition = q;
    float ry = *(float*)((char*)self + 0x744);
    float rz = *(float*)((char*)self + 0x740);
    func_001241C0(self, (sQuat_001241C0*)&q, zero,
                  *(float*)((char*)self + 0xAF4) + ry,
                  *(float*)((char*)self + 0xAF0) + rz);
    float rx = *(float*)((char*)self + 0x748);
    *(sV_3640*)&self->mRotation = AxisAngle_3640(D_004FF160_3640, *(float*)((char*)self + 0xAF8) + rx - 1.5707963705062866f);
    *(sV_3640*)&self->mRotation = QuatMul_3640(q, *(sV_3640*)&self->mRotation);
    cRider_updateOrientationImplicit(self);
    *(sV_3640*)&self->mVelocity = D_004FF120_3640;
    *(float*)((char*)self + 0xB04) = zero;
    func_00111890(self->mRiderHandler);
    initTrack_3640(self, 0x1F0, zero, zero);
    initTrack_3640(self, 0x1FC, zero, zero);
    initTrack_3640(self, 0x208, zero, zero);
    initTrack_3640(self, 0x214, zero, zero);
    initTrack_3640(self, 0x220, zero, zero);
    initTrack_3640(self, 0x22C, zero, zero);
    initTrack_3640(self, 0x238, zero, zero);
    initTrack_3640(self, 0x244, zero, zero);
    initTrack_3640(self, 0x250, zero, zero);
    initTrack_3640(self, 0x25C, one, zero);
    initTrack_3640(self, 0x268, zero, zero);
    initTrack_3640(self, 0x274, zero, zero);
    initTrack_3640(self, 0x280, zero, zero);
    initTrack_3640(self, 0x28C, zero, zero);
    initTrack_3640(self, 0x298, zero, zero);
    initTrack_3640(self, 0x2A4, zero, zero);
    initTrack_3640(self, 0x2B0, zero, zero);
    initTrack_3640(self, 0x2BC, zero, zero);
    initTrack_3640(self, 0x2C8, zero, zero);
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
        int g = func_00123128(self);
        int n = func_0014D988(tbl, g);
        sEnt_3640* e = (sEnt_3640*)func_0014D998(tbl, g);
        int i;
        for (i = 0; i < n; i++, e++) {
            int id = e->id;
            if (id >= 0 && (e->flags & 0x40)) {
                if (lookup_3640(db, e->item)->flags & 0x10) {
                    func_0030EA80(self->mAnimModel, id, 0);
                    func_0030EA80(self->mAnimModel, id, 1);
                    func_0030EA80(self->mAnimModel, id, 2);
                    func_0030EA80(self->mAnimModel, id, 3);
                }
            }
        }
    }
    func_00114130(self, 0, 0);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00123B48);
#ifdef SKIP_ASM
extern "C" void func_002803F0(void* self);
extern "C" void func_0011D390(void* self);
extern "C" void* func_0028B180();
void func_002A1BD0(void* p);
extern "C" void func_0030EA80(void* model, int i, int channel);
extern "C" void func_0030E9E0(void* model, int i, int channel);
int func_0027C958(void* self);
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
extern "C" void* func_0014AD50(void* iface, int id);
int func_0014D988(void* tbl, int g);
extern "C" void* func_0014D998(void* tbl, int g);
extern "C" void func_002AD5F0(void* bank, int id, int a, float f);

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

extern "C" void func_00123B48(cRider* self)
{
    func_002803F0((char*)&self->mScriptObjRiderVtbl);
    func_0011D390(self);
    if (self->unk_AC8 != 0) {
        func_002A1BD0(func_0028B180());
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
    // PORT: func_0027C958 returns a record pointer as int
    if (*(signed char*)((char*)func_0027C958(self->unk_6D8) + 6) != 0) {
        void* iface = cBE_getInterface(cBE_getBE(), 9);
        void* tbl = func_0014BDB8_noarg();
        sDb_123B48* db = (sDb_123B48*)func_0014AD50(iface, self->mAIIndex);
        int g = func_00123128(self);
        int n = func_0014D988(tbl, g);
        sEnt_123B48* e = (sEnt_123B48*)func_0014D998(tbl, g);
        int i;
        for (i = 0; i < n; i++, e++) {
            int id = entId_123B48(e);
            if (id >= 0 && entOn_123B48(e)) {
                if (itemFlags_123B48(db, e) & 0x10) {
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
            func_002AD5F0(*(char**)*(char**)((char*)func_0028B180() + 0x118) + 0x1D8, ids[j], 1, 0.0f);
        }
        ids[j] = -1;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00123DA8);
#ifdef SKIP_ASM
extern "C" void func_0027C9B0(int, void*);
extern "C" void func_00124788(void*, void*, float, float, int);
extern "C" void func_00125448(void*, void*, float, float, int);

extern "C" void func_00123DA8(cRider* self, int id, float a, float b, int unused, int flags)
{
    int buf[12];
    if (self->mIsInNIS)
    {
        func_0027C9B0(id, buf);
        func_00124788(self, buf, a, b, flags);
        func_00125448(self, buf, a, b, flags);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00123E30);
#ifdef SKIP_ASM
extern "C" void func_0027C9B0(int, void*);
extern "C" void* func_0028B180();
extern "C" int func_002A1B58(void* self, int msg, int a);
extern "C" int func_002A1B88(void* self, int type, int a, int b);
extern "C" void* func_00279F18(void* self, int i, int a2);
extern void* D_004A28A4;

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

extern "C" void func_00123E30(cRider* self, int id, float a, float b, int unused, int flags)
{
    sAnimEventInfo_123E30 info;
    if (flags & 0x10)
    {
        func_0027C9B0(id, &info);
        for (int i = 0; i < info.count; i++)
        {
            sAnimEvent_123E30* e = &info.events[i];
            int ev = e->id;
            if (ev == 0x66) continue;
            if (ev < 0x64)
            {
                func_002A1B58(func_0028B180(), ev, (int)self);
                self->unk_AC8 = 1;
            }
            else
            {
                ev -= 0x64;
                void* snd = func_0028B180();
                // PORT: id is a handle that is also read as a pointer here.
                int v = (int)func_00279F18(D_004A28A4, *e->arg, *(int*)(id + 8));
                func_002A1B88(snd, ev, (int)self, v);
                self->unk_AC8 = 1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00123F38);
#ifdef SKIP_ASM
extern "C" void* func_0026B5E0(void*, int, int);
extern char D_004D33A0[];
int func_0027C958(void* self);

struct sV_3F38 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sV_3F38 D_004FF160_3F38 __asm__("D_004FF160");

struct sRay_3F38 {
    char data[0xB0];
} __attribute__((aligned(16)));

struct sHit_3F38 {
    sV_3F38 pos;
    char pad[0x70];
} __attribute__((aligned(16)));

extern "C" void func_0032E100_3F38(sRay_3F38* ray, const sV_3F38& a, const sV_3F38& b, int n, float t) __asm__("func_0032E100");
extern "C" float func_00336850(void* self, sRay_3F38* ray, sHit_3F38* out, int a3);

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

extern "C" sV_3F38 func_00123F38(cRider* self)
{
    if (*(int*)((char*)self + 0x6E4) <= 0)
        return *(sV_3F38*)&self->mPosition;
    float lift = 0.0f;
    // PORT: func_0027C958 returns a record pointer as int
    char* rec = (char*)func_0027C958(self->unk_6D8);
    sV_3F38 pos = *(sV_3F38*)&self->mPosition;
    switch (*(signed char*)(rec + 5)) {
    case 0:
        break;
    case 1:
        {
            float* p = (float*)func_0026B5E0(D_004D33A0, 1, self->mAIIndex);
            sV_3F38 v;
            v.x = p[2];
            v.y = p[3];
            v.z = p[4];
            v.w = 1.0f;
            pos = v;
        }
        break;
    case 2:
        {
            float* p = (float*)func_0026B5E0(D_004D33A0, 2, 0);
            sV_3F38 v;
            v.x = p[2];
            v.y = p[3];
            v.z = p[4];
            v.w = 1.0f;
            pos = v;
        }
        break;
    case 3:
        switch (*(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x78) + 0x1BC)) {
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            {
                float* p = (float*)func_0026B5E0(D_004D33A0, 2, 0);
                sV_3F38 v;
                v.x = p[2];
                v.y = p[3];
                v.z = p[4];
                v.w = 1.0f;
                pos = v;
            }
            break;
        case 14:
        case 15:
        case 16:
            {
                float* p = (float*)func_0026B5E0(D_004D33A0, 2, self->mAIIndex);
                sV_3F38 v;
                v.x = p[2];
                v.y = p[3];
                v.z = p[4];
                v.w = 1.0f;
                pos = v;
            }
            break;
        default:
            {
                float* p = (float*)func_0026B5E0(D_004D33A0, 2, 1);
                sV_3F38 v;
                v.x = p[2];
                v.y = p[3];
                v.z = p[4];
                v.w = 1.0f;
                pos = v;
            }
            break;
        }
        break;
    case 4:
        {
            float* p = (float*)func_0026B5E0(D_004D33A0, 2, self->mAIIndex);
            sV_3F38 v;
            v.x = p[2];
            v.y = p[3];
            v.z = p[4];
            v.w = 1.0f;
            pos = v;
        }
        lift = 2999.942626953125f;
        break;
    }
    sV_3F38 lo = Add_3F38(pos, Scale_3F38(D_004FF160_3F38, -7000.0f));
    sV_3F38 hi = Add_3F38(pos, Scale_3F38(D_004FF160_3F38, 200.0f));
    sHit_3F38 hit;
    sRay_3F38 ray;
    func_0032E100_3F38(&ray, lo, hi, 2, 0.9791666865348816f);
    if (func_00336850(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x20), &ray, &hit, 0) >= 0.0f)
        pos = hit.pos;
    AddTo_3F38(pos, Scale_3F38(D_004FF160_3F38, lift));
    return pos;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001241C0);
#ifdef SKIP_ASM
extern "C" void func_0031BE50(float* s, float* c, float angle);

struct sQuat_001241C0
{
    float x, y, z, w;
} __attribute__((aligned(16)));

extern "C" void func_001241C0(cRider* self, sQuat_001241C0* out, float rx, float ry, float rz)
{
    sQuat_001241C0 q;
    float sz, cz, sy, cy, sx, cx;
    func_0031BE50(&sz, &cz, rz * 0.5f);
    func_0031BE50(&sy, &cy, ry * 0.5f);
    func_0031BE50(&sx, &cx, rx * 0.5f);
    float cc = cy * cx;
    float ss = sy * sx;
    q.w = cz * cc + sz * ss;
    q.x = sz * cc - cz * ss;
    q.y = cz * sy * cx + sz * cy * sx;
    q.z = cz * cy * sx - sz * sy * cx;
    *out = q;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001242B0);
#ifdef SKIP_ASM
struct sV_42B0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sV_42B0 D_004FF120_42B0 __asm__("D_004FF120");
extern sV_42B0 D_004FF160_42B0 __asm__("D_004FF160");

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

extern "C" void func_0032E100(sRay_42B0* ray, const sV_42B0& a, const sV_42B0& b, int n, float t);
extern "C" float func_003342D0(void* coll, sRay_42B0* ray, sHit_42B0* hit, int flags);
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

// PORT: the unit declares func_001242B0(void*, void*); this definition binds the cRider* view by asm label.
extern "C" void func_001242B0_impl(cRider* self, void* p) __asm__("func_001242B0");
extern "C" void func_001242B0_impl(cRider* self, void* p)
{
    char* o = (char*)&self->mVtbl;
    sVE_42B0* ve = &(*(sVE_42B0**)o)[kRiderVt_getVelocity];
    float speed = Length_42B0(*ve->fn(o + ve->delta));
    float s = self->mAnimModel->unk_140.x * (*(float*)((char*)self + 0x1F0) * 45.0f);
    sV_42B0 b = Add_42B0(*(sV_42B0*)p, Scale_42B0(*(sV_42B0*)((char*)self + 0x3B0), s));
    sV_42B0 lo = Add_42B0(b, Scale_42B0(*(sV_42B0*)&self->mGroundNormal, -100.0f));
    sV_42B0 hi = Add_42B0(b, Scale_42B0(*(sV_42B0*)&self->mGroundNormal, 200.0f));
    sHit_42B0 hit;
    sRay_42B0 ray;
    func_0032E100(&ray, lo, hi, 2, 0.5f);
    float t = func_003342D0(self->mOverlapList, &ray, &hit, 0);
    *(sV_42B0*)((char*)self + 0x380) = *(sV_42B0*)&self->mGroundNormal;
    if (t > 0.0f && hit.obj != 0) {
        self->mGroundPolyId = *(int*)(hit.obj + 0x150);
        *(float*)((char*)self + 0xAAC) = hit.f6C;
        *(float*)((char*)self + 0xAB0) = hit.f70;
        *(unsigned short*)((char*)self + 0x2D4) = *(unsigned short*)(hit.obj + 0xA);
    } else {
        *(unsigned int*)&self->mGroundPolyId = 0xFFFFFFFF;
    }
    if (t < 0.0f) {
        self->unk_B00 = 0;
        *(sV_42B0*)&self->mGroundNormal = D_004FF160_42B0;
        *(sV_42B0*)((char*)self + 0x3D0) = D_004FF120_42B0;
        *(sV_42B0*)((char*)self + 0x3A0) = Normalize_42B0(SubV_42B0(*(sV_42B0*)&self->mMatrix.row[1], Scale_42B0(*(sV_42B0*)&self->mGroundNormal, DotV_42B0(*(sV_42B0*)&self->mMatrix.row[1], *(sV_42B0*)&self->mGroundNormal))));
        *(sV_42B0*)((char*)self + 0x3B0) = Cross_42B0(*(sV_42B0*)&self->mGroundNormal, *(sV_42B0*)((char*)self + 0x3A0));
        *(int*)((char*)self + 0x438) = 0;
        self->mAudioBraking = 0.0f;
        self->mAudioEdging = 0.0f;
        self->mAudioPressure = 0.0f;
        self->mAudioSpeed = 0.0f;
        *(float*)((char*)self + 0xB04) = speed;
        return;
    }
    if (self->unk_AFC != 0 && self->unk_B00 == 0) {
        func_00111AA0_42B0(self->mRiderHandler, &hit, &hit.nrm, hit.mat, Length_42B0(hit.v20));
    }
    self->unk_B00 = 1;
    *(int*)((char*)self + 0x438) = hit.mat;
    if (hit.mat == -1)
        *(int*)((char*)self + 0x438) = 0;
    *(sV_42B0*)&self->mGroundNormal = hit.nrm;
    *(sV_42B0*)((char*)self + 0x460) = hit.pos;
    *(sV_42B0*)((char*)self + 0x3D0) = hit.v20;
    *(sV_42B0*)((char*)self + 0x3A0) = Normalize_42B0(SubV_42B0(*(sV_42B0*)&self->mMatrix.row[1], Scale_42B0(*(sV_42B0*)&self->mGroundNormal, DotV_42B0(*(sV_42B0*)&self->mMatrix.row[1], *(sV_42B0*)&self->mGroundNormal))));
    *(sV_42B0*)((char*)self + 0x3B0) = Cross_42B0(*(sV_42B0*)&self->mGroundNormal, *(sV_42B0*)((char*)self + 0x3A0));
    *(float*)((char*)self + 0x454) = Dot_42B0(Sub_42B0(b, hit.pos), *(sV_42B0*)&self->mGroundNormal);
    float d = *(float*)((char*)self + 0xB04) - speed;
    *(int*)&self->mAudioBraking = 0;
    if (d < self->mAudioBraking) {
        float v = -d * 0.0036000001709908247f;
        self->mAudioBraking = v;
        if (1.0f < v)
            self->mAudioBraking = 1.0f;
    }
    float v2 = speed * 0.0007200000109151006f;
    self->mAudioSpeed = v2;
    if (1.0f < v2)
        self->mAudioSpeed = 1.0f;
    *(float*)((char*)self + 0xB04) = speed;
    self->mAudioPressure = 0.30000001192092896f;
    self->mAudioEdging = 0.0f;
}
#endif

INCLUDE_ASM("ai/rider", func_00124788);

//100%
INCLUDE_ASM("ai/rider", func_00125038);
#ifdef SKIP_ASM
extern void* D_004A28A8;
void* func_00230698(void* self, int id);
extern "C" void func_002E4578(void* p);

extern "C" void func_00125038(cRider* self)
{
    void* w = *(void**)((char*)D_004A28A8 + 0x84);
    if (*(void**)((char*)w + 0x84) != 0 && self->mViewIndex >= 0)
    {
        if (*(int*)((char*)func_00230698(w, self->mViewIndex) + 0x78) == 1)
            func_002E4578(func_00230698(*(void**)((char*)D_004A28A8 + 0x84), self->mViewIndex));
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001250A8);
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

extern "C" void func_001250A8(cRider* self, sRiderVec4_001250A8* v)
{
    sRiderVec4_001250A8* p = (sRiderVec4_001250A8*)&self->mVelocity;
    riderAddEq_001250A8(*p, *v);
    if (func_0011FE98(self) == 1)
        cAirPredictor_startLaunchIntoAir(self->mAirPredictor, &self->mPosition, p, 3333.33349609375f);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00125108);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern char* D_004A2EEC;
extern "C" float func_001193E0(void* self);
extern "C" void func_00162258(void* self);
extern "C" void func_00238358(void* self, int a1, int a2);
extern "C" void func_00286EA0(void* self, void* rider);
extern "C" void* func_0028B180();

struct sVEntry125108 { short delta; short index; int (*fn)(void*); };

static inline void* getC0_125108()
{
    return *(void**)((char*)D_004A28A8 + 0xC0);
}

extern "C" void func_00125108(cRider* self)
{
    char* g = D_004A2EEC;
    if (g != 0 && self->mDidNotFinish == 0 && self->mIsHuman != 0)
    {
        if (self->mIsLocal != 0)
            *(int*)(g + 0xA0) = 1;
        else
            *(int*)(g + 0xA4) = 1;
    }
    func_001193E0(self->mMetrix);
    self->mFinishTick = *(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 0xC) + self->mPenaltyTicks;
    if (self->mFinishTime < 0.0f) self->mFinishTime = 0.0f;
    char* obj = (char*)&self->mVtbl;
    sVEntry125108* vt = *(sVEntry125108**)obj;
    func_00238358(getC0_125108(), vt[kRiderVt_getAIIndex].fn(obj + vt[kRiderVt_getAIIndex].delta), (int)self->mMetrix + 0xFC);
    unsigned int k = *(unsigned int*)&self->mViewIndex;
    if (k < 2)
    {
        char* tbl = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84);
        func_00162258(*(void**)(*(char**)(tbl + (k << 2) + 4) + 0xA8));
    }
    func_00286EA0(func_0028B180(), self);
    if (self->mDidNotFinish == 1) self->unk_100 = 0;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00125228);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVE_125228 {
    short delta;
    short index;
    int (*fn)(void*);
};
extern "C" void* cBE_getBE();
void* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00125108(cRider* self);
extern "C" void* func_0028B180();
extern "C" void func_002A3C00(void* a, void* self, int n);
extern "C" unsigned int func_004139F8(float f);
extern void* D_004A28A8;
extern signed char D_00535C10[];
extern int D_005308D0[];

extern "C" int func_00125228(cRider* self)
{
    char* obj = (char*)&self->mVtbl;
    sVE_125228* vt = *(sVE_125228**)obj;
    if (vt[kRiderVt_isHuman].fn(obj + vt[kRiderVt_isHuman].delta)) {
        float h = self->mFinishTime;
        float zero = 0.0f;
        int done = h >= zero;
        if (!done) {
            cBE_getInterface(cBE_getBE(), 0);
            if (D_00535C10[0] == 4)
                return self->mFinishTime >= zero;
            char* g = (char*)D_004A28A8;
            char* p = *(char**)(g + 0xC0);
            if (*(int*)(p + 0x88) != 0 && ((D_005308D0[0] >> 9) & 1) == 0) {
                int t = *(int*)(*(char**)(*(char**)(g + 0x84) + 0xC) + 0xC);
                int secs = func_004139F8((float)*(unsigned int*)(p + 0x78) * 0.01666666753590107f);
                if (secs * 60 < t) {
                    self->mDidNotFinish = 1;
                    func_00125108(self);
                    func_002A3C00(func_0028B180(), self, 2);
                    goto end;
                }
            }
            if (self->mDidNotFinish == 1 && *(int*)(*(char**)((char*)D_004A28A8 + 0x84) + 0x214) == 4)
                func_00125108(self);
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

extern int D_005366D0[];

static inline bool riderHasFinished001253D0(cRider* rider)
{
    return rider->mFinishTime >= 0.0f;
}

extern "C" void cRider_quitEvent(cRider* self)
{
    bool finished = riderHasFinished001253D0(self);
    if (!finished)
    {
        self->mDidNotFinish = 1;
        char* sub = (char*)&self->mVtbl;
        sVEntry001253D0* vt = *(sVEntry001253D0**)sub;
        D_005366D0[vt[kRiderVt_getAIIndex].fn(sub + vt[kRiderVt_getAIIndex].delta)] = 1;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00125448);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" int func_002AD550(void* bank, int id, int a);
extern "C" void func_002AD5F0(void* bank, int id, int a, float f);
void func_00280548(void* self, int a1, void* a2, void* a3);
extern "C" int func_00287968(void* self, int a1, int a2);
extern "C" int func_00288A20(void* self, void* rider);
extern "C" int func_002906B8(void* self);
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

extern "C" void func_00125448(void* vself, void* vobj, float when, float b, int flags)
{
    cRider* self = (cRider*)vself;
    char* obj = (char*)vobj;
    int* ids = self->unk_B08;
    sWtSnd_5448* snd = (sWtSnd_5448*)func_0028B180();
    int i;
    for (i = 0; i < 4; i++) {
        if (ids[i] >= 0) {
            if (func_002AD550((char*)*snd->cur + 0x1D8, ids[i], 1) == 0)
                ids[i] = -1;
        }
    }
    if (flags & 1) {
        sSfxEnt_5448* e = *(sSfxEnt_5448**)(obj + 0xC);
        if (e != 0) {
        char* rp = (char*)&self->mPosition;
        do {
            if ((float)e->when != when)
                return;
            int a, type;
            func_00280548(&self->mScriptObjRiderVtbl, e->id, &a, &type);
            int tp = type;
            int ar = a;
            sWtTrack_5448* t = snd->track;
            sWtSub_5448* s = &t->sub;
            t->sub.cur++;
            s->ents[t->sub.cur] = s->cfg;
            t->sub.arr24[t->sub.cur] = 0;
            s->p34[t->sub.cur] = 0;
            s->p38[t->sub.cur] = s->ents[t->sub.cur].b0;
            s->p3C[t->sub.cur] = 0;
            func_00416210((char*)s + t->sub.cur * 8 + 0x40, 0, 8);
            s->p60[t->sub.cur] = 0;
            s->p64[t->sub.cur] = 0;
            s->p68[t->sub.cur] = 0;
            s->p6C[t->sub.cur] = 0;
            s->p70[t->sub.cur].f4 = 0;
            s->p70[t->sub.cur].f0 = 100;
            s->p70[t->sub.cur].f8 = 90;
            s->p70[t->sub.cur].fC = 50;
            s->p74[t->sub.cur] = 0;
            s->p80[t->sub.cur] = 0;
            s->p84[t->sub.cur] = 0;
            s->p78[t->sub.cur] = 0x7F;
            s->p7C[t->sub.cur] = 1;
            snd->types[snd->track->sub.cur] = tp;
            snd->args[snd->track->sub.cur] = ar;
            {
                sWtTrack_5448* t2 = *snd->cur;
                sWtSub_5448* s2 = &t2->sub;
                s2->p38[t2->sub.cur] = *e->vol;
                s2->ents[t2->sub.cur].b0 = MAX_5448(MIN_5448(s2->p38[s2->cur], 0x7F), 0);
            }
            {
                sWtTrack_5448** pt = snd->cur;
                int h = func_00287968(snd, 4, 0);
                (*pt)->sub.p3C[(*pt)->sub.cur] = h;
            }
            {
                sWtTrack_5448* t3 = *snd->cur;
                t3->sub.arr24[t3->sub.cur] = (int)rp;
                sWtTrack_5448** pt = snd->cur;
                int h = func_00288A20(snd, self);
                (*pt)->sub.p64[(*pt)->sub.cur] = h;
            }
            *(int*)((char*)ids + (e->slot << 2)) = func_002906B8(snd);
            e = e->next;
        } while (e != 0);
        }
    } else if (flags & 2) {
        int keep[4];
        int on = 1;
        for (i = 3; i >= 0; i--)
            keep[i] = on;
        sSfxEnt_5448* e;
        for (e = *(sSfxEnt_5448**)(obj + 0xC); e != 0; e = e->next)
            keep[e->slot] = 0;
        for (i = 0; i < 4; i++) {
            if (keep[i] != 0) {
                if (ids[i] >= 0) {
                    func_002AD5F0((char*)*snd->cur + 0x1D8, ids[i], 1, 0.0f);
                    ids[i] = -1;
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00125958);
#ifdef SKIP_ASM
extern "C" void func_00125958(cRider* self, int a1)
{
    self->mUberFlags = a1;
    if (a1 == 0) {
        self->mUberLevel = 0;
        *(int*)&self->mUberTimer = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00125970);
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

extern "C" void func_00125970(cRider* self)
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
        float d = speed - Dot_125970(*(sVec4_125970*)&self->mVelocity, dir);
        if (d > dir.z)
        {
            sVec4_125970 imp = Scale_125970(Scale_125970(dir, d), 0.015000000596046448f);
            char* obj = (char*)&self->mVtbl;
            sVEntry_125970* vt = *(sVEntry_125970**)obj;
            vt[kRiderVt_applyBoost].fn(obj + vt[kRiderVt_applyBoost].delta, &imp);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00125AD0__FPv);
#ifdef SKIP_ASM
void func_00125AD0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00125AD8);
#ifdef SKIP_ASM
extern "C" int func_00125AD8(cRider* self)
{
    union { float f; int i; } u;
    u.f = *(float*)&self->mPosition + self->mPosition.y + self->mPosition.z
        + (*(float*)&self->mRotation + self->mRotation.y + self->mRotation.z + self->mRotation.w);
    return u.i;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00125B18);
#ifdef SKIP_ASM
extern "C" void func_00326CF0(void* obj);

struct sVEntryI_125B18 { short delta; short index; int (*fn)(void*); };
struct sVEntryF_125B18 { short delta; short index; void (*fn)(void*, float, int); };

// PORT: the unit declares (void*, int, int); the second argument is really an object pointer.
extern "C" void func_00125B18(void* p, int a, int on)
{
    float* s = (float*)p;
    void* obj = (void*)a;
    if (on == 0)
    {
        s[0] = s[1] = 0.0f;
        func_00326CF0(obj);
        return;
    }
    float v = s[0] * 0.9133333563804626f - 18.518518447875977f;
    float r = 0.0f;
    if (v >= 0.0f)
        r = v;
    s[0] = r;
    s[1] *= 0.9916666746139526f;
    sVEntryI_125B18* vt = *(sVEntryI_125B18**)obj;
    if (vt[3].fn((char*)obj + vt[3].delta) == 2)
    {
        sVEntryF_125B18* vt2 = *(sVEntryF_125B18**)obj;
        vt2[4].fn((char*)obj + vt2[4].delta, ((s[1] - 0.5f) * 0.009999999776482582f >? (s[0] - 2.0f) / 972.2222290039062f) >? 0.0f, 0);
        sVEntryF_125B18* vt3 = *(sVEntryF_125B18**)obj;
        vt3[4].fn((char*)obj + vt3[4].delta, ((s[1] - 70.0f) * 0.0062500000931322575f >? (s[0] - 100.0f) / 5000.0f) >? 0.0f, 1);
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
extern char D_00458338[];
extern void* D_004A28A8;
extern "C" void cRider_cRider(void* self);
extern "C" void cRiderHandler_cRiderHandler(void* self, int inChrg);
extern "C" void* cReplay_addCache(void* p);

// PORT: hand-written form of g++ 2.95's constructor for a class with a virtual base
// (cRider at +0xE10): vtable copies with delta fixups when not in charge.
extern "C" void* cPlayer_cPlayer(char* self, int inChrg)
{
    sVt9_125C70 t1;
    sVt22_125C70 t2;
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
        t1 = D_00458360;
        *(void**)(*(char**)(self + 0x18) + 0x6E8) = &t1;
        {
            char* vbo = *(char**)(self + 0x18) - 0xE10;
            vc = self - vbo;
        }
        t1.e[1].delta = D_00458360.e[1].delta + vc;
        t2 = D_004583A8;
        *(void**)(*(char**)(self + 0x18) + 0x6C0) = &t2;
        t2.e[1].delta = D_004583A8.e[1].delta + vc;
        t2.e[16].delta = D_004583A8.e[16].delta + vc;
        t2.e[17].delta = D_004583A8.e[17].delta + vc;
        t2.e[18].delta = D_004583A8.e[18].delta + vc;
        t2.e[19].delta = D_004583A8.e[19].delta + vc;
        t2.e[20].delta = D_004583A8.e[20].delta + vc;
    }
    *(void**)(self + 0xDE8) = D_00458338;
    *(int*)(self + 0xDF4) = 0;
    *(int*)(self + 0xDFC) = 0;
    *(int*)(self + 0xE00) = 0;
    *(int*)(*(char**)(self + 0x18) + 0x874) = 1;
    *(void**)(self + 0xDF8) = cReplay_addCache(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28));
    return self;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00125EB8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/rider_classes.h"

cRider_125EB8::~cRider_125EB8()
{
    func_002700D8(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28), mDF8);
}

// PORT: the compiler names this dtor _$_13cRider_125EB8; expose it under the address-derived name.
__asm__(".globl func_00125EB8\n.type func_00125EB8,@function\nfunc_00125EB8 = _$_13cRider_125EB8\n.size func_00125EB8,0x1838\n");

// PORT: the out-of-line destructors called here, under their address-derived names (as seg/seg_1218 does).
__asm__("_$_14cRiderVb_rider = cRider__cRider\n"
        "_$_15cVbDtor00136AD0 = func_00136AD0\n"
        "_$_15cVbDtor002DCDA0 = func_002DCDA0\n"
        "_$_15cVbDtor002DA8E8 = func_002DA8E8\n"
        "_$_15cVbDtor002E83F0 = func_002E83F0\n"
        "_$_15cVbDtor002E64C0 = func_002E64C0\n"
        "_$_15cVbDtor002EAC60 = func_002EAC60\n"
        "_$_15cVbDtor002EF530 = func_002EF530\n"
        "_$_15cVbDtor002D4A70 = func_002D4A70\n"
        "_$_15cVbDtor002E37D0 = func_002E37D0\n"
        "_$_15cVbDtor002DF1C8 = func_002DF1C8\n"
        "_$_15cVbDtor002F0FE8 = func_002F0FE8\n"
        "_$_15cVbDtor002F6388 = func_002F6388\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$13cRider_125EB8 = D_00458338\n"
        "_vt$13cRider_125EB8$15cRiderVbC_rider = D_00458360\n"
        "_vt$13cRider_125EB8$14cRiderVb_rider = D_004583A8\n"
        "_vt$17cRiderBase_125EB8 = D_00458710\n"
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
INCLUDE_ASM("ai/rider", func_001276F0);
#ifdef SKIP_ASM
int cInput_getState(void*, int);
extern "C" int func_001276F0(char* self)
{
    if (cInput_getState(*(void**)(self + 0xDF0), 0x1F))
        return 0;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x20))
        return 1;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x21))
        return 2;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x22))
        return 3;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x23))
        return 4;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x24))
        return 5;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x25))
        return 6;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x26))
        return 7;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x27))
        return 8;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x28))
        return 9;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x29))
        return 10;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x2A))
        return 11;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x2B))
        return 12;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x2C))
        return 13;
    if (cInput_getState(*(void**)(self + 0xDF0), 0x2D))
        return 14;
    return -1;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00127848);
#ifdef SKIP_ASM
int cInput_getState(void*, int);

extern "C" int func_00127848(void* self)
{
    if (cInput_getState(*(void**)((char*)self + 0xDF0), 0x1B))
        return 0;
    if (cInput_getState(*(void**)((char*)self + 0xDF0), 0x1C))
        return 1;
    if (cInput_getState(*(void**)((char*)self + 0xDF0), 0x1D))
        return 2;
    if (cInput_getState(*(void**)((char*)self + 0xDF0), 0x1E))
        return 3;
    return -1;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001278C0__FPv);
#ifdef SKIP_ASM
void func_001278C0(void* self)
{
    *(float*)((char*)self + 0xE00) = 0.0f;
    *(float*)((char*)self + 0xDFC) = 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001278D0);
#ifdef SKIP_ASM
extern "C" void func_001278D0(void* self, float val)
{
    *(float*)((char*)self + 0xDFC) = val >? *(float*)((char*)self + 0xDFC);
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_001278E0__FPvf);
#ifdef SKIP_ASM
void func_001278E0(void* self, float val)
{
    *(float*)((char*)self + 0xE00) = val;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/rider", func_001278E8);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_001474E8(void* iface, int id);
extern "C" void func_00125B18(void* p, int a, int b);

struct sVEntry001278E8 { short delta; short index; int (*fn)(void*); };

static inline bool riderInRange_1278E8(int v)
{
    return v != 0 && v < 10;
}

extern "C" void func_001278E8(void* self)
{
    char* s = (char*)self;
    if (*(int*)(s + 0xDF4) != 0)
    {
        int flag = 0;
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        char* sub = *(char**)(s + 0x18) + 0x6C0;
        sVEntry001278E8* vt = *(sVEntry001278E8**)sub;
        if (func_001474E8(iface, vt[7].fn(sub + vt[7].delta)) != 0)
        {
            flag = !riderInRange_1278E8(**(int**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28));
        }
        func_00125B18(s + 0xDFC, *(int*)(s + 0xDF4), flag);
    }
}
#endif

//100%
INCLUDE_ASM("ai/rider", func_00127998);
#ifdef SKIP_ASM
int func_0011FEE8(void*);
extern "C" float cInput_getValue(void* pad, int axis);
extern "C" void func_0026D178(void* cache, void* input);

// One frame of pad input packed for the rider's current state (func_0011FEE8); each state has its own layout.
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

// Reads the pad into the input record for the rider's state, and records it for replays.
extern "C" void func_00127998(char* self, sRiderInput_127998* in)
{
    MEM_fill(in, 0, 8);
    switch (func_0011FEE8(*(void**)(self + 0x18))) {
    case 0: {
        float k = 31.0f;
        in->s0.b12 = cInput_getState(PAD_127998, 0x2E);
        in->s0.b13 = cInput_getState(PAD_127998, 0x19);
        in->s0.b14 = cInput_getState(PAD_127998, 0x11);
        in->s0.b15 = cInput_getState(PAD_127998, 0x12);
        in->s0.b16 = cInput_getState(PAD_127998, 0x13);
        in->s0.b17 = cInput_getState(PAD_127998, 0x14);
        in->s0.b18 = cInput_getState(PAD_127998, 5);
        in->s0.b19 = cInput_getState(PAD_127998, 6);
        in->s0.a20 = (int)(cInput_getValue(PAD_127998, 0) * k);
        in->s0.a26 = (int)(cInput_getValue(PAD_127998, 1) * k);
        in->s0.w0 = (int)(cInput_getValue(PAD_127998, 2) * k);
        in->s0.w6 = (int)(cInput_getValue(PAD_127998, 3) * k);
        break;
    }
    case 11:
        in->s11.b12 = cInput_getState(PAD_127998, 0x2E);
        in->s11.b13 = cInput_getState(PAD_127998, 0x19);
        in->s11.a14 = (int)(cInput_getValue(PAD_127998, 0x18) * 31.0f);
        break;
    case 1: {
        float k = 31.0f;
        in->s1.b12 = cInput_getState(PAD_127998, 0x2E);
        in->s1.b13 = cInput_getState(PAD_127998, 0x13);
        in->s1.b14 = cInput_getState(PAD_127998, 0x14);
        in->s1.b16 = cInput_getState(PAD_127998, 0x11);
        in->s1.b15 = cInput_getState(PAD_127998, 0x12);
        in->s1.b17 = cInput_getState(PAD_127998, 0x16);
        in->s1.a24 = (int)(cInput_getValue(PAD_127998, 0x17) * k);
        in->s1.a18 = (int)(cInput_getValue(PAD_127998, 0) * k);
        in->s1.w0 = (int)(cInput_getValue(PAD_127998, 3) * k);
        in->s1.w6 = (int)(cInput_getValue(PAD_127998, 4) * k);
        break;
    }
    case 6:
        in->s6.a12 = (int)(cInput_getValue(PAD_127998, 0x1A) * 31.0f);
        break;
    case 7: {
        float k = 31.0f;
        in->s7.b12 = cInput_getState(PAD_127998, 0x2E);
        in->s7.b13 = cInput_getState(PAD_127998, 0x11);
        in->s7.b14 = cInput_getState(PAD_127998, 0x12);
        in->s7.b15 = cInput_getState(PAD_127998, 0x13);
        in->s7.b16 = cInput_getState(PAD_127998, 0x14);
        in->s7.v17 = func_00127848(self);
        in->s7.a25 = (int)(cInput_getValue(PAD_127998, 0x17) * k);
        in->s7.w0 = (int)(cInput_getValue(PAD_127998, 0xD) * k);
        in->s7.w6 = (int)(cInput_getValue(PAD_127998, 3) * k);
        break;
    }
    case 12:
        in->s12.b12 = cInput_getState(PAD_127998, 0x2E);
        in->s12.b13 = cInput_getState(PAD_127998, 0x13);
        in->s12.b14 = cInput_getState(PAD_127998, 0x14);
        in->s12.v15 = func_00127848(self);
        in->s12.a23 = (int)(cInput_getValue(PAD_127998, 0x17) * 31.0f);
        break;
    case 2: {
        float k = 31.0f;
        in->s2.b12 = cInput_getState(PAD_127998, 0x2E);
        in->s2.b13 = cInput_getState(PAD_127998, 0x12);
        in->s2.b14 = cInput_getState(PAD_127998, 0x14);
        in->s2.a15 = (int)(cInput_getValue(PAD_127998, 8) * k);
        in->s2.a21 = (int)(cInput_getValue(PAD_127998, 9) * k);
        in->s2.w0 = (int)(cInput_getValue(PAD_127998, 7) * k);
        in->s2.w6 = (int)(cInput_getValue(PAD_127998, 0x17) * k);
        break;
    }
    case 3: {
        float k = 31.0f;
        in->s2.b12 = cInput_getState(PAD_127998, 0x2E);
        in->s2.b13 = cInput_getState(PAD_127998, 0x13);
        in->s2.b14 = cInput_getState(PAD_127998, 0x14);
        in->s2.a15 = (int)(cInput_getValue(PAD_127998, 0) * k);
        in->s2.a21 = (int)(cInput_getValue(PAD_127998, 0x17) * k);
        break;
    }
    case 4: {
        float k = 31.0f;
        in->s4.b12 = cInput_getState(PAD_127998, 0x2E);
        in->s4.b13 = cInput_getState(PAD_127998, 0x19);
        in->s4.b14 = cInput_getState(PAD_127998, 5);
        in->s4.b15 = cInput_getState(PAD_127998, 6);
        in->s4.v16 = func_001276F0(self);
        in->s4.a24 = (int)(cInput_getValue(PAD_127998, 0) * k);
        in->s4.w0 = (int)(cInput_getValue(PAD_127998, 1) * k);
        break;
    }
    case 5: {
        float k = 31.0f;
        in->s4.b12 = cInput_getState(PAD_127998, 0x2E);
        in->s4.b13 = cInput_getState(PAD_127998, 0x19);
        in->s4.b14 = cInput_getState(PAD_127998, 0x15);
        in->s4.b15 = cInput_getState(PAD_127998, 0xC);
        in->s4.v16 = func_001276F0(self);
        in->s4.a24 = (int)(cInput_getValue(PAD_127998, 0xE) * k);
        in->s4.w0 = (int)(cInput_getValue(PAD_127998, 0xF) * k);
        in->s4.w12 = (int)(cInput_getValue(PAD_127998, 0xB) * k);
        in->s4.w6 = (int)(cInput_getValue(PAD_127998, 0xA) * k);
        float v = cInput_getValue(PAD_127998, 3);
        int t;
        if (-0.5f <= v) {
            t = 0.5f < v;
        } else {
            t = -1;
        }
        in->s4.t18 = t;
        break;
    }
    case 8:
        in->s11.b12 = cInput_getState(PAD_127998, 0x2E);
        in->s11.b13 = cInput_getState(PAD_127998, 0x10);
        break;
    case 10:
        in->s6.a12 = (int)(cInput_getValue(PAD_127998, 0) * 31.0f);
        break;
    case 13:
        break;
    }
    if (*(void**)(self + 0xDF8)) {
        func_0026D178(*(void**)(self + 0xDF8), in);
    }
}
#endif

extern "C" void* func_00111AC0(void* self);

//100%
INCLUDE_ASM("ai/rider", func_00128660__FPv);
#ifdef SKIP_ASM
void* func_00128660(void* self)
{
    return func_00111AC0(self);
}
#endif

extern "C" void* func_00111D98(void* self);

//100%
INCLUDE_ASM("ai/rider", func_00128680__FPv);
#ifdef SKIP_ASM
void* func_00128680(void* self)
{
    return func_00111D98(self);
}
#endif

