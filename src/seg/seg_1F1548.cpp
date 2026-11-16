#include "common.h"
#include "ai/riderhandler.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("seg/seg_1F1548", cPSPVisualEffectsMan_cPSPVisualEffectsMan);
#ifdef SKIP_ASM
extern "C" void *cMemMan_alloc(int, const char *, int, int);
extern "C" void* cVisualEffectsMan_cVisualEffectsMan(void*);
extern "C" void* func_00372AA0(void*);
void* func_003759E8(void*);
extern char D_00487D28[];
extern char D_00487BB8[];
extern char D_00487BC8[];
extern char D_00487BE0[];
extern char D_00487BF0[];
extern char D_00487C00[];
extern char D_00487C10[];
extern char D_00487C20[];
extern char D_00487C30[];
extern char D_00487C40[];
extern char D_00487C50[];
extern char D_00487C68[];
extern char D_00487C78[];
extern char D_00487C88[];
extern char D_00487DC8[];
extern char D_00487DF8[];
extern char D_00487E28[];
extern char D_00487E58[];
extern char D_00487E88[];
extern char D_00487EB8[];
extern char D_00487FA8[];
extern char D_00488050[];
extern char D_004882B0[];
extern char D_00488330[];
extern char D_00492FA0[];
extern char D_00492FD0[];
extern char D_004930A0[];
extern char D_00493178[];
extern char D_004931A8[];
extern char D_004A3B88[];
extern char D_004A3B90[];
extern char D_004A3B98[];
extern char D_004A3BA0[];

struct sMgr_0548 {
    void* objs[17];
    char* vptr;
};

/**
 * Constructs the platform visual-effects manager: the base manager plus its 17 effect-control objects.
 *
 * Runs cVisualEffectsMan_cVisualEffectsMan(), stores this class's vtable (+0x44) and allocates objs[0..16],
 * 4-byte objects (mostly just a vtable) tagged RenderToggles, VisualEffectsTest, LightGlow, Plants, DepthFog,
 * HeightFog, DetailSystem, ScreenTint, Shadow, SnowSurface, BoardTrail, Flags, PowerUpFXToggles,
 * SnowfallToggles, SplashToggles, Tube and PathArrow. The name is On Tour's (PSP); retail tags the allocation
 * "PS2VisualEffectsMan".
 *
 * @return self
 */
extern "C" sMgr_0548* cPSPVisualEffectsMan_cPSPVisualEffectsMan(sMgr_0548* self)
{
    cVisualEffectsMan_cVisualEffectsMan(self);
    self->vptr = D_00487D28;
    { void* effect = cMemMan_alloc(4, D_00487BB8, 0, 0); *(char**)effect = D_004931A8; self->objs[0] = effect; }
    { void* effect = cMemMan_alloc(4, D_00487BC8, 0, 0); *(char**)effect = D_00492FD0; self->objs[1] = effect; }
    { void* effect = cMemMan_alloc(4, D_00487BE0, 0, 0); *(char**)effect = D_00487EB8; self->objs[2] = effect; }
    { void* effect = cMemMan_alloc(4, D_004A3B88, 0, 0); *(char**)effect = D_00493178; self->objs[3] = effect; }
    { void* effect = cMemMan_alloc(4, D_00487BF0, 0, 0); *(char**)effect = D_00487E88; self->objs[4] = effect; }
    { void* effect = cMemMan_alloc(4, D_00487C00, 0, 0); *(char**)effect = D_00487E58; self->objs[5] = effect; }
    self->objs[6] = func_003759E8(cMemMan_alloc(4, D_00487C10, 0, 0));
    { void* effect = cMemMan_alloc(4, D_00487C20, 0, 0); *(char**)effect = D_00492FA0; self->objs[7] = effect; }
    { void* effect = cMemMan_alloc(4, D_004A3B90, 0, 0); *(char**)effect = D_00487E28; self->objs[8] = effect; }
    { void* effect = cMemMan_alloc(4, D_00487C30, 0, 0); *(char**)effect = D_00487DF8; self->objs[9] = effect; }
    { void* effect = cMemMan_alloc(4, D_00487C40, 0, 0); *(char**)effect = D_00487FA8; self->objs[10] = effect; }
    { void* effect = cMemMan_alloc(4, D_004A3B98, 0, 0); *(char**)effect = D_00487DC8; self->objs[11] = effect; }
    { void* effect = cMemMan_alloc(4, D_00487C50, 0, 0); *(char**)effect = D_00488330; self->objs[12] = effect; }
    { void* effect = cMemMan_alloc(4, D_00487C68, 0, 0); *(char**)effect = D_00488050; self->objs[13] = effect; }
    { void* effect = cMemMan_alloc(4, D_00487C78, 0, 0); *(char**)effect = D_004882B0; self->objs[14] = effect; }
    { void* effect = cMemMan_alloc(4, D_004A3BA0, 0, 0); *(char**)effect = D_004930A0; self->objs[15] = effect; }
    self->objs[16] = func_00372AA0(cMemMan_alloc(4, D_00487C88, 0, 0));
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F0820);
#ifdef SKIP_ASM
void operator_delete(int *);
struct Obj0820 {
    virtual ~Obj0820();
};
extern char D_00487D28[];
extern char cVisualEffectsMan_vtbl[];

struct sMgr_0820 {
    Obj0820* objs[17];
    char* vptr;
};

extern "C" void func_002F0820(sMgr_0820* self, int arg1)
{
    self->vptr = D_00487D28;
    delete self->objs[0];
    delete self->objs[1];
    delete self->objs[2];
    delete self->objs[3];
    delete self->objs[4];
    delete self->objs[5];
    delete self->objs[6];
    delete self->objs[7];
    delete self->objs[8];
    delete self->objs[9];
    delete self->objs[10];
    delete self->objs[11];
    delete self->objs[12];
    delete self->objs[13];
    delete self->objs[14];
    delete self->objs[15];
    delete self->objs[16];
    self->vptr = cVisualEffectsMan_vtbl;
    if (arg1 & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F0A98);
#ifdef SKIP_ASM
class cObj_0A98 {
public:
    virtual void v0();
    virtual void Reset();
};

class cWorld_0A98 {
public:
    char pad[0x10D8];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
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
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void v79();
    virtual void v80();
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual void v84();
    virtual void v85();
    virtual void v86();
    virtual void v87();
    virtual void v88();
    virtual void v89();
    virtual void v90();
    virtual void v91();
    virtual void v92();
    virtual void v93();
    virtual void v94();
    virtual void v95();
    virtual void v96();
    virtual void v97();
    virtual void v98();
    virtual void v99();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void ResetSlot(int i);
};

struct sMgr_0A98 {
    cObj_0A98* objs[17];
};

extern int D_004A3BA8;
extern "C" void func_002F00A0(void*, int);

extern "C" void func_002F0A98(sMgr_0A98* self)
{
    self->objs[0]->Reset();
    self->objs[1]->Reset();
    self->objs[2]->Reset();
    self->objs[3]->Reset();
    self->objs[4]->Reset();
    self->objs[5]->Reset();
    self->objs[6]->Reset();
    self->objs[7]->Reset();
    self->objs[8]->Reset();
    self->objs[9]->Reset();
    self->objs[10]->Reset();
    self->objs[11]->Reset();
    self->objs[12]->Reset();
    self->objs[13]->Reset();
    self->objs[14]->Reset();
    self->objs[15]->Reset();
    self->objs[16]->Reset();
    for (int i = 0; i < *(int*)((char*)SSXApp->Game->ViewMan + 0x10); i++) {
        func_002F00A0(self, (&D_004A3BA8)[i]); // PORT: D_004A3BA8 is an int array ($gp-addressed)
        ((cWorld_0A98*)GrMan)->ResetSlot(i);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F0CD0);
#ifdef SKIP_ASM
class cObj_0CD0 {
public:
    virtual void v0();
    virtual void v1();
    virtual void Update();
};

struct sMgr_0CD0 {
    cObj_0CD0* objs[17];
};

extern "C" void func_002F0CD0(sMgr_0CD0* self)
{
    self->objs[0]->Update();
    self->objs[1]->Update();
    self->objs[2]->Update();
    self->objs[3]->Update();
    self->objs[4]->Update();
    self->objs[5]->Update();
    self->objs[6]->Update();
    self->objs[7]->Update();
    self->objs[8]->Update();
    self->objs[9]->Update();
    self->objs[10]->Update();
    self->objs[11]->Update();
    self->objs[12]->Update();
    self->objs[13]->Update();
    self->objs[14]->Update();
    self->objs[15]->Update();
    self->objs[16]->Update();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", cPunchFX_cPunchFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "seg/seg_1F1548_classes.h"

/**
 * Constructs a punch effect, a class with cRider as a virtual base, and clears its active flag.
 *
 * As g++ 2.95 does for virtual bases, a hidden second argument says whether this is the most-derived object;
 * only then does it build the cRider at +0xB0 and point the base pointer (+0x0) at it. It sets the cRider
 * part's vtables (+0x6C0, +0x6D0, +0x6E8 there) to this class's and calls readFromReplayFrame() to reset.
 */
cSeg_2F0E88::cSeg_2F0E88()
{
    cPunchFX_readFromReplayFrame_0E88(this);
}

// PORT: the compiler names this ctor __11cSeg_2F0E88i; expose it under the address-derived name.
__asm__(".globl cPunchFX_cPunchFX\n.type cPunchFX_cPunchFX,@function\ncPunchFX_cPunchFX = __11cSeg_2F0E88i\n.size cPunchFX_cPunchFX,0x160\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("__12cRiderVb_s1f = cRider_cRider\n"
        "_vt$11cSeg_2F0E88$13cRiderVbC_s1f = D_004887D8\n"
        "_vt$11cSeg_2F0E88$14cRiderVbBC_s1f = D_00459B90\n"
        "_vt$11cSeg_2F0E88$12cRiderVb_s1f = D_00488820\n");
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", cPunchFX__cPunchFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "seg/seg_1F1548_classes.h"

/**
 * Destroys a punch effect: restores this class's vtables in its virtual cRider base, then tears down.
 *
 * The hidden g++ destructor flags decide the rest: bit 1 destroys the cRider virtual base (cRider__cRider)
 * and bit 0 frees the object. The C++ body itself is empty.
 */
cVbDtor002F0FE8::~cVbDtor002F0FE8()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor002F0FE8; expose it under the address-derived name.
__asm__(".globl cPunchFX__cPunchFX\n.type cPunchFX__cPunchFX,@function\ncPunchFX__cPunchFX = _$_15cVbDtor002F0FE8\n.size cPunchFX__cPunchFX,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002F0FE8$13cRiderVbC_s1f = D_004887D8\n"
        "_vt$15cVbDtor002F0FE8$14cRiderVbBC_s1f = D_00459B90\n"
        "_vt$15cVbDtor002F0FE8$12cRiderVb_s1f = D_00488820\n"
        "_$_12cRiderVb_s1f = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", cPunchFX_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Resets the punch effect by clearing its active flag (+0xA0).
 *
 * Despite the name it reads nothing from a replay frame (On Tour's version takes a cReplayFramePtr*); the
 * constructor calls it as a reset.
 */
extern "C" void cPunchFX_readFromReplayFrame(cPunchFX* self) {
    (*(int *)((char*)(self) + (0xA0))) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", cPunchFX_update);
#ifdef SKIP_ASM
int func_00312AA0(void* self, int i);
extern "C" int func_00311AE8(void*, int);
extern "C" char* func_00311B20(void*, int);
extern "C" int func_001446A0(void*, int);
extern int D_004A3AFC;

struct sVec4_1150 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sCol_1150 {
    float r, g, b, a;
};

struct sMat_1150 {
    sVec4_1150 r[4];
};

extern sVec4_1150 t4vect_Zero;

struct sFx_1150 {
    char* owner;
    char pad4[0xC];
    sVec4_1150 pos[4];
    sCol_1150 col[4];
    float size[4];
    int active;
};

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void CopyMat_1150(sMat_1150* dst, sMat_1150* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline void MulMat_1150(sVec4_1150* out, const sMat_1150* m, const sVec4_1150& v)
{
    sVec4_1150 r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    *out = r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline void Add_1150(sVec4_1150* out, const sVec4_1150& a, const sVec4_1150& b)
{
    sVec4_1150 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    *out = r;
}

// PORT: float bit-pattern random number in [0,1) (seed reused as float storage).
static inline float randf_1150()
{
    D_004A3AFC = ((D_004A3AFC * 0x18FCD + 0xE9507C) & 0x7FFFFF) | 0x3F800000;
    return *(float*)&D_004A3AFC - 1.0f;
}

static inline float randRange_1150(float lo, float hi)
{
    return lo + (hi - lo) * randf_1150();
}

/**
 * Places the punch effect's four sprites around the rider's punching hand while a punch is playing.
 *
 * Active while the rider's mAttackBlend (+0x350) is positive and a punch animation runs: 0x146 or 0x147 in
 * anim slot 0, or 0x143 or 0x144 in slot 1 while its sequence's event bit 0 is set and bit 1 clear. That
 * picks the hand bone (+0x8B0 or +0x8B8, swapped when +0x320 is set). Each sprite gets a random offset of up
 * to 3 * blend around a point 8 units along the bone, a size of 15 to 15 + 15 * blend and a grey level of at
 * most 0.9 x blend.
 */
extern "C" void cPunchFX_update(sFx_1150* self)
{
    int active = 0.0f < *(float*)(self->owner + 0x350);
    self->active = active;
    if (!active)
        return;
    int bone = *(int*)(self->owner + 0x8B0);
    int flag = 0;
    int a = func_00312AA0(*(void**)(self->owner + 0x784), 0);
    int layer1Anim = func_00312AA0(*(void**)(self->owner + 0x784), 1);
    if (func_00311AE8(*(void**)(self->owner + 0x784), 1) == 0xD) {
        if (func_001446A0(func_00311B20(*(void**)(self->owner + 0x784), 1) + 0xB0, 1) == 0) {
            flag = func_001446A0(func_00311B20(*(void**)(self->owner + 0x784), 1) + 0xB0, 0) != 0;
        }
    }
    if (a == 0x146 || (layer1Anim == 0x143 && flag)) {
        if (*(int*)(self->owner + 0x320))
            bone = *(int*)(self->owner + 0x8B0);
        else
            bone = *(int*)(self->owner + 0x8B8);
    } else if (a == 0x147 || (layer1Anim == 0x144 && flag)) {
        if (*(int*)(self->owner + 0x320))
            bone = *(int*)(self->owner + 0x8B8);
        else
            bone = *(int*)(self->owner + 0x8B0);
    } else {
        self->active = 0;
    }
    if (self->active) {
        sVec4_1150 base = *(sVec4_1150*)(*(char**)(*(char**)(self->owner + 0x780) + 0x30) + (bone << 6) + 0x30);
        sMat_1150 boneMat;
        CopyMat_1150(&boneMat, &(*(sMat_1150**)(*(char**)(self->owner + 0x780) + 0x30))[bone]);
        boneMat.r[3] = t4vect_Zero;
        sVec4_1150 point;
        sVec4_1150 rnd;
        {
            sVec4_1150 dir;
            dir.x = 8.0f;
            dir.y = 0.0f;
            dir.z = 0.0f;
            dir.w = 0.0f;
            MulMat_1150(&rnd, &boneMat, dir);
            Add_1150(&point, base, rnd);
            base = point;
        }
        for (int i = 0; i < 4; i++) {
            float attackBlend = *(float*)(self->owner + 0x350);
            float a = attackBlend * 3.0f;
            float hi = attackBlend * 15.0f + 15.0f;
            rnd.w = 0.0f;
            rnd.x = randRange_1150(-a, a);
            rnd.y = randRange_1150(-a, a);
            rnd.z = randRange_1150(-a, a);
            Add_1150(&point, base, rnd);
            self->pos[i] = point;
            self->size[i] = randRange_1150(15.0f, hi);
            float shade = *(float*)(self->owner + 0x350) * 0.9f;
            shade *= randf_1150();
            point.x = 1.0f;
            point.y = shade;
            point.z = shade;
            point.w = shade;
            self->col[i] = *(sCol_1150*)&point;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", cPunchFX_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sV2_1510 {
    float x, y;
    sV2_1510(const float& ax, const float& ay) : x(ax), y(ay) {}
};

struct sGs_1510 {
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
    unsigned int f10 : 16;
    unsigned int f10hi : 16;
    void SetF22(int v) { f4_22 = v; }
};

extern sGs_1510 D_00501420_1510 __asm__("D_00501420");

class cGfx_1510 {
public:
    char pad[0xE84];
    sGs_1510* stateTop;
    char padE88[0xFB0 - 0xE88];
    int fFB0;
    int GetFB0() { return fFB0; }
    void PushState() { stateTop[1] = stateTop[0]; stateTop++; }
    char padFB4[0x10D8 - 0xFB4];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76();
    virtual void DrawSprite(const sVec4_1150&, const sV2_1510&, const sV2_1510&, const sV2_1510&, const sCol_1150&);
};

/**
 * Draws the punch effect's four sprites when it is active.
 *
 * Pushes a copy of the default draw state (D_00501420) with the effect texture (GrMan _EffectTexMap[24],
 * +0xFB0) and its blend settings, draws each sprite at pos[i] with its size and colour over the whole
 * texture, then pops the state.
 */
extern "C" void cPunchFX_render(sFx_1150* self)
{
    if (self->active == 0)
        return;
    ((cGfx_1510*)GrMan)->PushState();
    *((cGfx_1510*)GrMan)->stateTop = D_00501420_1510;
    // Raw char** read of the stack top (as in ovstatehud) so the global is re-read afterwards.
    *(short*)((char*)GrMan->m_pCurrentDrawState + 0x10) = ((cGfx_1510*)GrMan)->GetFB0();
    ((cGfx_1510*)GrMan)->stateTop->w0 |= 0xC;
    ((cGfx_1510*)GrMan)->stateTop->f4_23 = 0;
    ((cGfx_1510*)GrMan)->stateTop->f4_20 = 0;
    ((cGfx_1510*)GrMan)->stateTop->f4_12 = 0x14;
    ((cGfx_1510*)GrMan)->stateTop->f4_0 = 2;
    ((cGfx_1510*)GrMan)->stateTop->f8_10 = 0;
    ((cGfx_1510*)GrMan)->stateTop->f8_5 = 7;
    ((cGfx_1510*)GrMan)->stateTop->f4_2 = 7;
    ((cGfx_1510*)GrMan)->stateTop->SetF22(1);
    for (int i = 0; i < 4; i++) {
        ((cGfx_1510*)GrMan)->DrawSprite(self->pos[i], sV2_1510(self->size[i], self->size[i]),
                                            sV2_1510(0.0f, 0.0f), sV2_1510(1.0f, 1.0f), self->col[i]);
    }
    ((cGfx_1510*)GrMan)->stateTop--;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F1768);
#ifdef SKIP_ASM
extern "C" void func_00370B60();
extern char D_004881E8[];

extern "C" void *func_002F1768(void *arg0) {
    func_00370B60();
    *(int *)((char*)arg0 + 0x200) = 0;
    *(char **)((char*)arg0 + 0x1F8) = D_004881E8;
    *(int *)((char*)arg0 + 0x204) = 0;
    *(int *)((char*)arg0 + 0x208) = 0;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F17A8);
#ifdef SKIP_ASM
extern "C" void cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(void *);
extern char D_004881E8[];

extern "C" void func_002F17A8(void *arg0) {
    *(char **)((char*)arg0 + 0x1F8) = D_004881E8;
    cBaseClass_DynamicEmitter__cBaseClass_DynamicEmitter(arg0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F17D0);
#ifdef SKIP_ASM
extern "C" void func_002F83B0();
extern "C" void func_002F9838();
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
extern "C" void* operator_new__FUi_17D0(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_00487C98[];
extern int D_004A3BB0;

extern "C" void func_002F17D0(void) {
    D_004A3BB0 = (int)operator_new__FUi_17D0(0x1540, D_00487C98, 0, 0);
    func_002F83B0();
    func_002F9838();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F1810);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv(int);
extern "C" void func_002F8FF8__FPv();
extern "C" void func_002FA5F8__FPv();
extern int D_004A3BB0;

extern "C" void func_002F1810(void) {
    func_002F8FF8__FPv();
    func_002FA5F8__FPv();
    if (D_004A3BB0 != 0) {
        cMemMan_free__FPv(D_004A3BB0);
    }
    D_004A3BB0 = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F1850);
#ifdef SKIP_ASM
extern "C" void func_002F18D8(void *);
extern char D_004881D0[];

extern "C" void *func_002F1850(void *arg0, int arg1) {
    *(int *)((char*)arg0 + 0x94) = 0;
    *(int *)((char*)arg0 + 0x9C) = 0;
    *(char **)((char*)arg0 + 0xA4) = D_004881D0;
    *(int *)((char*)arg0 + 0x98) = arg1;
    func_002F18D8(arg0);
    return arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F1890);
#ifdef SKIP_ASM
extern "C" void func_002F18D8(void *);
extern "C" void func_002F1A00(void *);
extern char D_004881D0[];

extern "C" void *func_002F1890(void *arg0, int arg1) {
    *(int *)((char*)arg0 + 0x94) = 0;
    *(int *)((char*)arg0 + 0x98) = 0;
    *(int *)((char*)arg0 + 0x9C) = arg1;
    *(char **)((char*)arg0 + 0xA4) = D_004881D0;
    func_002F18D8(arg0);
    func_002F1A00(arg0);
    return arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F18D8);
#ifdef SKIP_ASM
extern "C" void func_002F17D0();
extern int D_004A3BB4;

extern "C" void func_002F18D8(void *arg0) {
    if (D_004A3BB4 == 0) {
        func_002F17D0();
    }
    D_004A3BB4 += 1;
    for (int i = 9; i >= 0; i--)
        *(int *)((char*)arg0 + 0x6C + i * 4) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F1940);
#ifdef SKIP_ASM
extern "C" void func_002F1810();
void operator_delete(int *);
extern char D_004881D0[];
extern int D_004A3BB4;
struct Base1940 { char pad[0x1F8]; };
struct Obj1940 : Base1940 {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual ~Obj1940();
};

extern "C" void func_002F1940(void *arg0, int arg1) {
    int c = D_004A3BB4 - 1;
    *(char **)((char*)arg0 + 0xA4) = D_004881D0;
    D_004A3BB4 = c;
    if (c == 0) {
        func_002F1810();
    }
    Obj1940 **p = (Obj1940 **)((char*)arg0 + 0x6C);
    int n = 9;
    do {
        if (*p != 0) {
            delete *p;
        }
        *p = 0;
        n--;
        p++;
    } while (n >= 0);
    if (arg1 & 1) {
        operator_delete((int *)arg0);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F19F0);
#ifdef SKIP_ASM
extern "C" void func_002F19F0(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x94))) = arg1;
    (*(int *)((char*)(arg0) + (0))) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F1A00);
#ifdef SKIP_ASM
extern "C" void func_002F1A00(void *arg0) {
    (*(int *)((char*)(arg0) + (0xA0))) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F1A08);
#ifdef SKIP_ASM
// FAKEMATCH: one local `side` carries 75.0f into both bone-23/24 arms (assigned in each, so loop.c never treats it as a
// movable and each arm loads the constant inline, as retail does); plain form (`ox + 75.0f` / `ox - 75.0f`) 95.54
// UNPOSITIONED. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14 ruling (portable,
// behaviour-preserving steering only); replace it when an honest spelling matches.
struct sObj_22C8;
extern "C" void func_002F22C8(sObj_22C8* self);
extern "C" float func_002D1928(int n, float x);
extern "C" void func_003710D0(void* fx, sVec4_1150* pos, sVec4_1150* vel, int withVel, float dt);

// Emitter parameters (an emitter's +0x20C).
struct sFxParams_1A08 {
    char pad0[0xD8];
    float velScale;         // 0xD8: rider velocity factor
    float nrmScale;         // 0xDC: ground normal factor
    float nrmSpeedScale;    // 0xE0: ground normal factor, times speed
    float offX;             // 0xE4: offset from the bone
    float offY;             // 0xE8
    float offZ;             // 0xEC
    float dirX;             // 0xF0: extra velocity in bone space
    float dirY;             // 0xF4
    float dirZ;             // 0xF8
    float minSpeed;         // 0xFC: km/h (x 27.78 = units/s)
    float maxSpeed;         // 0x100
    float chance;           // 0x104
    float pulse;            // 0x108
    float pulseRate;        // 0x10C
};

struct sFxEmit_1A08 {
    char pad0[0x200];
    int bone;               // 0x200: index into the owner's bone handles
    int off;                // 0x204
    float phase;            // 0x208
    sFxParams_1A08* params; // 0x20C
};

struct sFxVE_1A08 {
    short delta;
    short index;
    sVec4_1150* (*fn)(void*);
};

// Same object as sObj_22C8 (func_002F22C8 fills ready/h).
struct sFxMgr_1A08 {
    int ready;              // 0x00
    int h[26];              // 0x04: bone handles
    sFxEmit_1A08* fx[10];   // 0x6C
    char* model;            // 0x94 (cAnimModel*)
    char* rider;            // 0x98 (cRider*)
    int f9C;                // 0x9C
    int fA0;                // 0xA0
};

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float len_1A08(const sVec4_1150& v)
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

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sVec4_1150 scale_1A08(const sVec4_1150& v, float s)
{
    sVec4_1150 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec4_1150 add_1A08(const sVec4_1150& a, const sVec4_1150& b)
{
    sVec4_1150 r;
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
static inline void addEq_1A08(sVec4_1150& a, const sVec4_1150& b)
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

// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline sVec4_1150 mul_1A08(const sMat_1150& m, const sVec4_1150& v)
{
    sVec4_1150 r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(&m), "m"(v)
        : "memory");
    return r;
}

static inline sVec4_1150 groundNormal_1A08(char* rider)
{
    return *(sVec4_1150*)(rider + 0x370);
}

static inline sVec4_1150 bonePos_1A08(char* model, int i)
{
    return *(sVec4_1150*)(*(char**)(model + 0x30) + (i << 6) + 0x30);
}

extern "C" void func_002F1A08(sFxMgr_1A08* self)
{
    if (self->ready == 0)
        func_002F22C8((sObj_22C8*)self);
    sVec4_1150 vel = t4vect_Zero;
    self->fA0 = 0;
    sVec4_1150 nrm;
    nrm.x = 0.0f;
    nrm.y = 0.0f;
    nrm.z = 1.0f;
    nrm.w = 1.0f;
    if (self->rider != 0) {
        char* sub = self->rider + 0x6C0;
        sFxVE_1A08* vt = *(sFxVE_1A08**)sub;
        vel = *vt[2].fn(sub + vt[2].delta);
        nrm = groundNormal_1A08(self->rider);
    }
    float speed = len_1A08(vel);
    for (int i = 0; i < 10; i++) {
        int emit = 0;
        if (self->fx[i] == 0)
            continue;
        sFxParams_1A08* d = self->fx[i]->params;
        if (self->fx[i]->off == 0 && d->minSpeed * 27.77777862548828f <= speed && speed <= d->maxSpeed * 27.77777862548828f)
            emit = 1;
        if (d->chance < 0.99f && d->chance < randf_1150())
            emit = 0;
        if (d->pulse < 0.99f) {
            self->fx[i]->phase += d->pulseRate * 0.01666666753590107f;
            if (self->fx[i]->phase >= 4.0f)
                self->fx[i]->phase -= 4.0f;
            if (d->pulse < (func_002D1928(3, self->fx[i]->phase) + 1.0f) * 0.5f)
                emit = 0;
        }
        sVec4_1150 pos = bonePos_1A08(self->model, self->h[self->fx[i]->bone]);
        {
            float ox = d->offX;
            float oy = d->offY;
            float oz = d->offZ;
            sVec4_1150 off;
            off.x = ox;
            off.y = oy;
            off.z = oz;
            off.w = 1.0f;
            int which = self->fx[i]->bone;
            float side;
            if (which == 25) {
                self->fA0 = 1;
                float rx = randRange_1150(-85.0f, 85.0f);
                float rz = randRange_1150(-14.0f, 14.0f);
                off.x = ox + rx;
                off.z = oz + rz;
            } else if (which == 23) {
                side = 75.0f;
                off.x = ox + side;
            } else if (which == 24) {
                side = 75.0f;
                off.x = ox - side;
            }
            off.x *= *(float*)(self->model + 0x140);
            off.y *= *(float*)(self->model + 0x144);
            off.z *= *(float*)(self->model + 0x148);
            if (off.x != 0.0f || off.y != 0.0f || off.z != 0.0f) {
                sMat_1150 m;
                CopyMat_1150(&m, &(*(sMat_1150**)(self->model + 0x30))[self->h[self->fx[i]->bone]]);
                m.r[3] = t4vect_Zero;
                off = mul_1A08(m, off);
                addEq_1A08(pos, off);
            }
        }
        if (emit) {
            sVec4_1150 v = add_1A08(add_1A08(scale_1A08(vel, d->velScale), scale_1A08(nrm, d->nrmScale)),
                                    scale_1A08(scale_1A08(nrm, d->nrmSpeedScale), speed));
            if (d->dirX != 0.0f || d->dirY != 0.0f || d->dirZ != 0.0f) {
                sMat_1150 m;
                CopyMat_1150(&m, &(*(sMat_1150**)(self->model + 0x30))[self->h[self->fx[i]->bone]]);
                m.r[3] = t4vect_Zero;
                float dx = d->dirX;
                float dy = d->dirY;
                float dz = d->dirZ;
                sVec4_1150 dir;
                dir.x = dx;
                dir.y = dy;
                dir.z = dz;
                dir.w = 1.0f;
                dir = mul_1A08(m, dir);
                addEq_1A08(v, dir);
            }
            func_003710D0(self->fx[i], &pos, &v, 1, 0.01666666753590107f);
        } else {
            func_003710D0(self->fx[i], &pos, 0, 0, 0.01666666753590107f);
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2088);
#ifdef SKIP_ASM
extern "C" void cDynamicEmitter_render(void* obj, int level);

struct sRiderInfo_2088 {
    char pad[0xB18];
    int fB18;               // 0xB18
    char padB1C[0xC2C - 0xB1C];
    int fC2C;               // 0xC2C
    char padC30[0xCC8 - 0xC30];
    int fCC8;               // 0xCC8
};

struct sOwner_2088 {
    char pad[0x6C];
    void* items[10];        // 0x6C
    char pad94[4];
    sRiderInfo_2088* a;     // 0x98
    sRiderInfo_2088* b;     // 0x9C
};

extern "C" void func_002F2088(sOwner_2088* self)
{
    int i;
    for (i = 0; i < 10; i++) {
        if (self->items[i] != 0) {
            int level = 7;
            if (self->b != 0)
                level = (self->b->fC2C < 0) ? 0x16 : self->b->fC2C + 1;
            sRiderInfo_2088* r = self->b;
            if ((self->a != 0 && self->a->fB18 != 0) || (r != 0 && r->fCC8 != 0))
                cDynamicEmitter_render(self->items[i], level);
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2130);
#ifdef SKIP_ASM
struct Base2130 { char pad[0x1F8]; };
struct Obj2130 : Base2130 {
    virtual void v0();
    virtual void v1();
    virtual void v2(int, float);
};
extern "C" void *cMemMan_alloc(int, const char *, int, int);
extern "C" void *func_002F1768(void *);
extern char D_00487CB0[];
extern int D_004A3BB0;

extern "C" int func_002F2130(void *arg0, int arg1, int arg2) {
    Obj2130 **p = (Obj2130 **)((char*)arg0 + 0x6C);
    int i;
    for (i = 0; i < 10; i++, p++) {
        if (*p == 0) {
            Obj2130 *n = (Obj2130 *)func_002F1768(cMemMan_alloc(0x210, D_00487CB0, 0, 0));
            int t = D_004A3BB0 + arg1 * 0x110;
            *p = n;
            *(int *)((char*)*p + 0x200) = arg2;
            *(int *)((char*)*p + 0x204) = 0;
            *(int *)((char*)*p + 0x20C) = t;
            Obj2130 *o = *p;
            o->v2(*(int *)((char*)o + 0x20C), *(float *)((char*)o + 0x58));
            (*p)->v1();
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2218);
#ifdef SKIP_ASM
struct sVE_2F2218 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_002F2218(char* self, int idx)
{
    int off = idx * 4;
    self += 0x6C;
    char** slot = (char**)(self + off);
    char* o = *slot;
    if (o != 0) {
        sVE_2F2218* vt = *(sVE_2F2218**)(o + 0x1F8);
        vt[4].fn(o + vt[4].delta, 3);
    }
    *slot = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2268);
#ifdef SKIP_ASM
extern "C" void func_002F2268(void *arg0, int arg1) {
    *(int *)((char *)(*(void **)((char *)arg0 + (arg1 << 2) + 0x6C)) + 0x204) = 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2280);
#ifdef SKIP_ASM
extern "C" void func_002F2280(void *arg0, int arg1) {
    *(int *)((char *)(*(void **)((char *)arg0 + (arg1 << 2) + 0x6C)) + 0x204) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2298);
#ifdef SKIP_ASM
extern "C" void func_002F2298(void *arg0, int arg1) {
    *(int *)((char *)(*(void **)((char *)arg0 + (arg1 << 2) + 0x6C)) + 0x174) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F22B0);
#ifdef SKIP_ASM
extern "C" void func_002F22B0(void *arg0, int arg1) {
    *(int *)((char *)(*(void **)((char *)arg0 + (arg1 << 2) + 0x6C)) + 0x174) = 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F22C8);
#ifdef SKIP_ASM
extern "C" int cAnimModel_getBoneSkelIndex(void*, int, const char*);
extern char D_004879D8[];
extern char D_004879E8[];
extern char D_004879F8[];
extern char D_00487A08[];
extern char D_00487A18[];
extern char D_00487A28[];
extern char D_00487A38[];
extern char D_00487A48[];
extern char D_00487A58[];
extern char D_00487A68[];
extern char D_00487A78[];
extern char D_00487A88[];
extern char D_00487A98[];
extern char D_00487AA8[];
extern char D_00487AB8[];
extern char D_00487AC8[];
extern char D_00487AD8[];
extern char D_00487AE8[];
extern char D_00487AF8[];
extern char D_004A3B00[];
extern char D_004A3B08[];
extern char D_004A3BB8[];
extern char D_00487CC8[];


struct sObj_22C8 {
    int ready;
    int h[26];
    char pad[0x94 - 0x6C];
    void* model;
};

extern "C" void func_002F22C8(sObj_22C8* self)
{
    self->h[0] = cAnimModel_getBoneSkelIndex(self->model, 0, D_004A3BB8);
    self->h[1] = cAnimModel_getBoneSkelIndex(self->model, 0, D_004A3B08);
    self->h[2] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487A58);
    self->h[3] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487A48);
    self->h[4] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487A38);
    self->h[5] = cAnimModel_getBoneSkelIndex(self->model, 0, D_004A3B00);
    self->h[6] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487A68);
    self->h[7] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487A88);
    self->h[8] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487A98);
    self->h[9] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487AA8);
    self->h[10] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487AB8);
    self->h[11] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487AC8);
    self->h[12] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487AD8);
    self->h[13] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487AE8);
    self->h[14] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487AF8);
    self->h[15] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487A78);
    self->h[16] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487A18);
    self->h[17] = cAnimModel_getBoneSkelIndex(self->model, 0, D_004879D8);
    self->h[18] = cAnimModel_getBoneSkelIndex(self->model, 0, D_004879E8);
    self->h[19] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487A28);
    self->h[20] = cAnimModel_getBoneSkelIndex(self->model, 0, D_004879F8);
    self->h[21] = cAnimModel_getBoneSkelIndex(self->model, 0, D_00487A08);
    self->h[22] = cAnimModel_getBoneSkelIndex(self->model, 1, D_00487CC8);
    self->h[23] = cAnimModel_getBoneSkelIndex(self->model, 1, D_00487CC8);
    self->h[24] = cAnimModel_getBoneSkelIndex(self->model, 1, D_00487CC8);
    self->h[25] = cAnimModel_getBoneSkelIndex(self->model, 1, D_00487CC8);
    for (int i = 0; i < 26; i++) {
        // NOTE: empty in retail (likely a compiled-out per-handle assert)
    }
    self->ready = 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2588);
#ifdef SKIP_ASM
extern "C" int *func_002F2588(int *arg0) {
    *arg0 = 0;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2598);
#ifdef SKIP_ASM
extern int D_004A3AFC;
// NOTE: tunables are float members of global records; splat names each field separately.
struct sTun_2598 {
    float v;
};
extern sTun_2598 D_004A428C_2598 __asm__("D_004A428C");
extern sTun_2598 D_004A4290_2598 __asm__("D_004A4290");
extern sTun_2598 D_004A4294_2598 __asm__("D_004A4294");
extern sTun_2598 D_004A429C_2598 __asm__("D_004A429C");
extern sTun_2598 D_004A42CC_2598 __asm__("D_004A42CC");

struct sV2_2598 {
    float x, y;
    sV2_2598() {}
    sV2_2598(float ax, float ay) : x(ax), y(ay) {}
};

struct sV4_2598 {
    float x, y, z, w;
    sV4_2598() {}
    sV4_2598(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
};

extern sV4_2598 D_005046E0;
extern sV4_2598 D_005046F0;
extern sV2_2598 D_004A5960[]; // PORT: 8-byte object, declared unsized so it is not $gp-addressed

struct sPart_2598 {
    int active;
    sV2_2598 pos;
    sV2_2598 size;
    sV2_2598 vel;
    int age;
    float life;
    sV4_2598 color;
    float angle;
    float spin;
};

// PORT: float bit-pattern random number in [0,1) (seed reused as float storage).
static inline float randf_2598()
{
    D_004A3AFC = ((D_004A3AFC * 0x18FCD + 0xE9507C) & 0x7FFFFF) | 0x3F800000;
    return *(float*)&D_004A3AFC - 1.0f;
}

static inline float randRange_2598(float lo, float hi)
{
    return lo + (hi - lo) * randf_2598();
}

static inline sV4_2598 operator*(const sV4_2598& a, float s)
{
    return sV4_2598(a.x * s, a.y * s, a.z * s, a.w * s);
}

static inline sV4_2598 operator+(const sV4_2598& a, const sV4_2598& b)
{
    return sV4_2598(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

extern "C" void func_002F2598_2598(sPart_2598* p, const sV2_2598& pos, sV2_2598* range) __asm__("func_002F2598");
extern "C" void func_002F2598_2598(sPart_2598* p, const sV2_2598& pos, sV2_2598* range)
{
    float maxY = range->y;
    float maxX = range->x;
    float x = randRange_2598(D_004A4294_2598.v, maxX);
    float v = x + randRange_2598(-D_004A429C_2598.v, D_004A429C_2598.v);
    float y = (v >= D_004A4294_2598.v) ? (v <? maxY) : D_004A4294_2598.v; // PORT: g++ <? operator
    p->age = 0;
    p->life = randRange_2598(D_004A428C_2598.v, D_004A4290_2598.v);
    p->active = 1;
    p->size = sV2_2598(x, y);
    p->pos = pos;
    p->vel = D_004A5960[0];
    p->angle = randf_2598() * 360.0f;
    p->spin = randRange_2598(-D_004A42CC_2598.v, D_004A42CC_2598.v);
    float t = randf_2598();
    p->color = D_005046E0 * (1.0f - t) + D_005046F0 * t;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2810);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A3AFC;
struct sTun_2810 {
    float v;
};
extern sTun_2810 D_004A4288_2810 __asm__("D_004A4288");
extern sTun_2810 D_004A42A0_2810 __asm__("D_004A42A0");
extern sTun_2810 D_004A42A4_2810 __asm__("D_004A42A4");
extern sTun_2810 D_004A42A8_2810 __asm__("D_004A42A8");
extern sTun_2810 D_004A42AC_2810 __asm__("D_004A42AC");
extern sTun_2810 D_004A42B0_2810 __asm__("D_004A42B0");
extern sTun_2810 D_004A42B4_2810 __asm__("D_004A42B4");
extern sTun_2810 D_004A42B8_2810 __asm__("D_004A42B8");
extern sTun_2810 D_004A42BC_2810 __asm__("D_004A42BC");
extern sTun_2810 D_004A42C0_2810 __asm__("D_004A42C0");
extern sTun_2810 D_004A42C4_2810 __asm__("D_004A42C4");
extern sTun_2810 D_004A42C8_2810 __asm__("D_004A42C8");
extern sTun_2810 D_004A55B8_2810 __asm__("D_004A55B8");
extern sTun_2810 D_004A55BC_2810 __asm__("D_004A55BC");

// PORT: sqrt.s (sqrtf without errno check)
static inline float sqrtf_2810(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

struct sV2_2810 {
    float x, y;
    sV2_2810() {}
    sV2_2810(float ax, float ay) : x(ax), y(ay) {}
    sV2_2810(const sV2_2810& o) : x(o.x), y(o.y) {}
    float Length() const { return sqrtf_2810(x * x + y * y); }
};

struct sPart_2810 {
    int active;
    sV2_2810 pos;
    sV2_2810 size;
    sV2_2810 vel;
    float age;
    float life;
};

static inline float randf_2810()
{
    D_004A3AFC = ((D_004A3AFC * 0x18FCD + 0xE9507C) & 0x7FFFFF) | 0x3F800000;
    return *(float*)&D_004A3AFC - 1.0f;
}

static inline float randRange_2810(float lo, float hi)
{
    return lo + (hi - lo) * randf_2810();
}

static inline sV2_2810 randScale_2810(const float& x, const float& y, float lo, float hi)
{
    return sV2_2810(randRange_2810(x * lo, x * hi), randRange_2810(y * lo, y * hi));
}

static inline float clamp_2810(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo; // PORT: g++ <? operator
}

static inline sV2_2810 operator*(const sV2_2810& a, float s)
{
    return sV2_2810(a.x * s, a.y * s);
}

static inline float Length2_2810(const float& x, const float& y)
{
    return sqrtf_2810(x * x + y * y);
}

static inline sV2_2810 Normalize_2810(const sV2_2810& v)
{
    float len = v.Length();
    if (len != 0.0f) {
        float inv = 1.0f / len;
        return sV2_2810(v.x * inv, v.y * inv);
    }
    return v;
}

static inline void addRand_2810(float& v, float lo, float hi)
{
    v += randRange_2810(lo, hi);
}

extern "C" void func_002F2810_2810(sPart_2810* p, float t) __asm__("func_002F2810");
extern "C" void func_002F2810_2810(sPart_2810* p, float t)
{
    p->age += 0.016666667f;
    addRand_2810(p->vel.x, D_004A42A0_2810.v, D_004A42A4_2810.v);
    addRand_2810(p->vel.y, D_004A42A8_2810.v, D_004A42AC_2810.v);
    sV2_2810 dir = Normalize_2810(sV2_2810(p->pos.x - D_004A55B8_2810.v, p->pos.y - D_004A55BC_2810.v));
    float k = clamp_2810((t - D_004A4288_2810.v) / (120.0f - D_004A4288_2810.v), 0.0f, 1.0f);
    {
        sV2_2810 a = dir * k * randRange_2810(D_004A42B8_2810.v, D_004A42BC_2810.v);
        p->vel.x += a.x;
        p->vel.y += a.y;
    }
    float speed = Length2_2810(p->vel.x, p->vel.y);
    addRand_2810(p->size.x, D_004A42B0_2810.v, D_004A42B4_2810.v);
    addRand_2810(p->size.y, D_004A42B0_2810.v, D_004A42B4_2810.v);
    float maxSpeed = D_004A42C0_2810.v;
    if (speed > maxSpeed) {
        sV2_2810& v = p->vel;
        v = (speed != 0.0f ? sV2_2810(p->vel.x * (1.0f / speed), p->vel.y * (1.0f / speed)) : v) * maxSpeed;
    }
    {
        sV2_2810 d = randScale_2810(p->vel.x, p->vel.y, D_004A42C4_2810.v, D_004A42C8_2810.v);
        p->pos.x += d.x;
        p->pos.y += d.y;
    }
    if (p->age >= p->life || p->pos.x + p->size.x <= 0.0f || 640.0f <= p->pos.x - p->size.x
        || p->pos.y + p->size.y <= 0.0f || 480.0f <= p->pos.y - p->size.y) {
        p->active = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2C30);
#ifdef SKIP_ASM
struct sVec2_2C30 {
    float x, y;
    sVec2_2C30() {}
    sVec2_2C30(float ax, float ay) : x(ax), y(ay) {}
};

struct sVec4_2C30 {
    float x, y, z, w;
    sVec4_2C30() {}
    sVec4_2C30(const sVec2_2C30& v, const float& az, const float& aw) : x(v.x), y(v.y), z(az), w(aw) {}
};

class cRender_2C30 {
public:
    char pad[0x10D8];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
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
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void DrawSprite(const sVec4_2C30& pos, void* tex, void* a, void* b, sVec4_2C30* col, int n, float ang);
};

extern int D_004A54F0;
extern int D_004A54F4;
extern int D_004A54F8;
extern float D_004A5500;
extern float D_004A5504;
extern int D_004A5508;
// PORT: the unit declares cGraphicsMan_TheGraphicsMan later as cGfx_3E28*; this view is bound by asm label.

static inline void setId_2C30(char* p, int v)
{
    *(short*)(p + 0x10) = v;
}

static inline float lerp_2C30(float a, float d, float t)
{
    t *= d;
    return a + t;
}

// PORT: the unit declares func_002F2C30 later as (void*, sV2_3E28*); typed body bound by asm label.
extern "C" void func_002F2C30_impl(char* obj, sVec2_2C30* off) __asm__("func_002F2C30");
extern "C" void func_002F2C30_impl(char* obj, sVec2_2C30* off)
{
    if (!D_004A54F8) {
        D_004A54F0 = 0;
        D_004A54F8 = 1;
        D_004A54F4 = 0;
    }
    if (!D_004A5508) {
        D_004A5508 = 1;
        D_004A5500 = 1.0f;
        D_004A5504 = 1.0f;
    }
    // The life ratio time/duration is written out twice (fade, then rotation);
    // CSE turns the second division into the mov.s copy retail has.
    float fade = *(float*)(obj + 0x1C) / *(float*)(obj + 0x20);
    sVec4_2C30 c = *(sVec4_2C30*)(obj + 0x24);
    fade = 1.0f - fade * fade;
    c.x *= fade;
    float ang = lerp_2C30(*(float*)(obj + 0x34), *(float*)(obj + 0x38), *(float*)(obj + 0x1C) / *(float*)(obj + 0x20));
    int id = GrMan->_EffectTexMap[66];
    setId_2C30(((char*)GrMan->m_pCurrentDrawState), id);
    ((cRender_2C30*)cGraphicsMan_TheGraphicsMan)->DrawSprite(sVec4_2C30(sVec2_2C30(*(float*)(obj + 4) + off->x, *(float*)(obj + 8) + off->y), 0.0f, 1.0f), obj + 0xC, &D_004A54F0, &D_004A5500, &c, 1, ang * 0.01745329424738884f);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2D60);
#ifdef SKIP_ASM
extern "C" void *func_002F2D60(void *arg0) {
    (*(int *)((char*)(arg0) + (4))) = 0;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2D70);
#ifdef SKIP_ASM
extern int D_004A3AFC;
// NOTE: tunables are float members of global records; splat names each field separately.
struct sTun_2D70 {
    float v;
};
extern sTun_2D70 D_004A42D8_2D70 __asm__("D_004A42D8");
extern sTun_2D70 D_004A42DC_2D70 __asm__("D_004A42DC");
extern sTun_2D70 D_004A42E0_2D70 __asm__("D_004A42E0");
extern sTun_2D70 D_004A42E4_2D70 __asm__("D_004A42E4");
extern sTun_2D70 D_004A42E8_2D70 __asm__("D_004A42E8");
extern sTun_2D70 D_004A4318_2D70 __asm__("D_004A4318");

struct sV2_2D70 {
    float x, y;
    sV2_2D70() {}
    sV2_2D70(float ax, float ay) : x(ax), y(ay) {}
};

struct sV4_2D70 {
    float x, y, z, w;
    sV4_2D70() {}
    sV4_2D70(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
};

extern sV4_2D70 D_00504700;
extern sV4_2D70 D_00504710;
extern sV2_2D70 D_004A5960_2D70[] __asm__("D_004A5960"); // PORT: 8-byte object, declared unsized so it is not $gp-addressed

struct sPart_2D70 {
    int dead;
    int active;
    sV2_2D70 pos;
    sV2_2D70 size;
    sV2_2D70 vel;
    int age;
    float life;
    sV4_2D70 color;
    float angle;
    float spin;
    int f40;
    int f44;
    int f48;
};

// PORT: float bit-pattern random number in [0,1) (seed reused as float storage).
static inline float randf_2D70()
{
    D_004A3AFC = ((D_004A3AFC * 0x18FCD + 0xE9507C) & 0x7FFFFF) | 0x3F800000;
    return *(float*)&D_004A3AFC - 1.0f;
}

static inline float randRange_2D70(float lo, float hi)
{
    return lo + (hi - lo) * randf_2D70();
}

static inline float randSym_2D70(const float s)
{
    return randRange_2D70(-s, s);
}

static inline float clamp_2D70(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo; // PORT: g++ <? operator
}

static inline sV4_2D70 operator*(const sV4_2D70& a, float s)
{
    return sV4_2D70(a.x * s, a.y * s, a.z * s, a.w * s);
}

static inline sV4_2D70 operator+(const sV4_2D70& a, const sV4_2D70& b)
{
    return sV4_2D70(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

extern "C" void func_002F2D70_2D70(sPart_2D70* p, const sV2_2D70& pos, int n) __asm__("func_002F2D70");
extern "C" void func_002F2D70_2D70(sPart_2D70* p, const sV2_2D70& pos, int n)
{
    float x = randRange_2D70(D_004A42E0_2D70.v, D_004A42E4_2D70.v);
    float v = x + randSym_2D70(D_004A42E8_2D70.v);
    float y = clamp_2D70(v, D_004A42E0_2D70.v, D_004A42E4_2D70.v);
    for (int i = 0; i < n; i++) {
        x *= 0.699999988079071f;
        y *= 0.699999988079071f;
    }
    p->age = 0;
    p->life = randRange_2D70(D_004A42D8_2D70.v, D_004A42DC_2D70.v);
    p->active = 1;
    p->size = sV2_2D70(x, y);
    p->pos = pos;
    p->vel = D_004A5960_2D70[0];
    float t = randf_2D70();
    p->color = D_00504700 * (1.0f - t) + D_00504710 * t;
    p->angle = randf_2D70() * 360.0f;
    p->spin = randSym_2D70(D_004A4318_2D70.v);
    p->f40 = 0;
    p->f44 = 0;
    p->f48 = 0;
    p->dead = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F3030);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A3AFC;
struct sTun_3030 {
    float v;
};
extern sTun_3030 D_004A4288_3030 __asm__("D_004A4288");
extern sTun_3030 D_004A42EC_3030 __asm__("D_004A42EC");
extern sTun_3030 D_004A42F0_3030 __asm__("D_004A42F0");
extern sTun_3030 D_004A42F4_3030 __asm__("D_004A42F4");
extern sTun_3030 D_004A42F8_3030 __asm__("D_004A42F8");
extern sTun_3030 D_004A42FC_3030 __asm__("D_004A42FC");
extern sTun_3030 D_004A4300_3030 __asm__("D_004A4300");
extern sTun_3030 D_004A4304_3030 __asm__("D_004A4304");
extern sTun_3030 D_004A4308_3030 __asm__("D_004A4308");
extern sTun_3030 D_004A430C_3030 __asm__("D_004A430C");
extern sTun_3030 D_004A4310_3030 __asm__("D_004A4310");
extern sTun_3030 D_004A4314_3030 __asm__("D_004A4314");
extern sTun_3030 D_004A55B8_3030 __asm__("D_004A55B8");
extern sTun_3030 D_004A55BC_3030 __asm__("D_004A55BC");

// PORT: sqrt.s (sqrtf without errno check)
static inline float sqrtf_3030(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

struct sV2_3030 {
    float x, y;
    sV2_3030() {}
    sV2_3030(float ax, float ay) : x(ax), y(ay) {}
    sV2_3030(const sV2_3030& o) : x(o.x), y(o.y) {}
    float Length() const { return sqrtf_3030(x * x + y * y); }
};

struct sPart_3030 {
    int dead;
    int active;
    sV2_3030 pos;
    sV2_3030 size;
    sV2_3030 vel;
    float age;
    float life;
    char pad28[0x40 - 0x28];
    float f40;
    float f44;
    float f48;
};

static inline float randf_3030()
{
    D_004A3AFC = ((D_004A3AFC * 0x18FCD + 0xE9507C) & 0x7FFFFF) | 0x3F800000;
    return *(float*)&D_004A3AFC - 1.0f;
}

static inline float randRange_3030(float lo, float hi)
{
    return lo + (hi - lo) * randf_3030();
}

static inline float clamp_3030(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo; // PORT: g++ <? operator
}

static inline sV2_3030 operator*(const sV2_3030& a, float s)
{
    return sV2_3030(a.x * s, a.y * s);
}

static inline float Length2_3030(const float& x, const float& y)
{
    return sqrtf_3030(x * x + y * y);
}

static inline sV2_3030 Normalize_3030(const sV2_3030& v)
{
    float len = v.Length();
    if (len != 0.0f) {
        float inv = 1.0f / len;
        return sV2_3030(v.x * inv, v.y * inv);
    }
    return v;
}

static inline void addRand_3030(float& v, float lo, float hi)
{
    v += randRange_3030(lo, hi);
}

extern "C" void func_002F3030_3030(sPart_3030* p, float t) __asm__("func_002F3030");
extern "C" void func_002F3030_3030(sPart_3030* p, float t)
{
    p->f40 -= 0.016666667f;
    p->age += 0.016666667f;
    if (p->f40 < 0.0f) {
        p->f40 = randRange_3030(D_004A42FC_3030.v, D_004A4300_3030.v);
        p->f44 = randRange_3030(D_004A42EC_3030.v, D_004A42F0_3030.v);
        p->f48 = randRange_3030(D_004A42F4_3030.v, D_004A42F8_3030.v);
    }
    p->vel.x += p->f44;
    p->vel.y += p->f48;
    sV2_3030 dir = Normalize_3030(sV2_3030(p->pos.x - D_004A55B8_3030.v, p->pos.y - D_004A55BC_3030.v));
    float k = clamp_3030((t - D_004A4288_3030.v) / (120.0f - D_004A4288_3030.v), 0.0f, 1.0f);
    {
        sV2_3030 a = dir * k * randRange_3030(D_004A430C_3030.v, D_004A4310_3030.v);
        p->vel.x += a.x;
        p->vel.y += a.y;
    }
    float speed = Length2_3030(p->vel.x, p->vel.y);
    addRand_3030(p->size.x, D_004A4304_3030.v, D_004A4308_3030.v);
    addRand_3030(p->size.y, D_004A4304_3030.v, D_004A4308_3030.v);
    float maxSpeed = D_004A4314_3030.v;
    if (speed > maxSpeed) {
        sV2_3030& v = p->vel;
        v = (speed != 0.0f ? sV2_3030(p->vel.x * (1.0f / speed), p->vel.y * (1.0f / speed)) : v) * maxSpeed;
    }
    p->pos.x += p->vel.x;
    p->pos.y += p->vel.y;
    if (p->age >= p->life || p->pos.x + p->size.x <= 0.0f || 640.0f <= p->pos.x - p->size.x
        || p->pos.y + p->size.y <= 0.0f || 480.0f <= p->pos.y - p->size.y) {
        p->active = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F3418);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sV2_3418 {
    float x, y;
};
struct sV3_3418 {
    float x, y, z;
};
struct sV4_3418 {
    float x, y, z, w;
};
struct sPos_3418 {
    float x, y, z, w;
    float u, v;
    sPos_3418(float ax, float ay, const float& az = 0.0f, const float& aw = 1.0f) {
        u = ax;
        v = ay;
        x = ax;
        y = ay;
        w = aw;
        z = az;
    }
};

class cGfx_3418 {
public:
    char pad[0x10D8];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76(); virtual void v77();
    virtual void DrawX(float, const sPos_3418&, const sV3_3418&, const sV2_3418&, const sV2_3418&, const sV4_3418&, int);
};

struct sWorld_3418 {
    char pad[0xE84];
    char* fE84;
    char pad2[0x105C - 0xE88];
    int f105C;
    int GetFB() { return f105C; }
};

struct sEm_3418 {
    int f0, f4;
    float f8, fC;
    sV3_3418 f10;
    float pad1C_;
    float f20, f24;
    sV4_3418 col;
    float f38, f3C;
};

extern float D_004A5510_3418 __asm__("D_004A5510");
extern float D_004A5514_3418 __asm__("D_004A5514");
extern int D_004A5518_3418 __asm__("D_004A5518");
extern float D_004A5520_3418 __asm__("D_004A5520");
extern float D_004A5524_3418 __asm__("D_004A5524");
extern int D_004A5528_3418 __asm__("D_004A5528");

extern "C" void func_002F3418(sEm_3418* self, const sV2_3418* off)
{
    if (D_004A5518_3418 == 0) {
        D_004A5510_3418 = 0.0f;
        D_004A5518_3418 = 1;
        D_004A5514_3418 = 0.0f;
    }
    if (D_004A5528_3418 == 0) {
        D_004A5528_3418 = 1;
        D_004A5520_3418 = 1.0f;
        D_004A5524_3418 = 1.0f;
    }
    float r = self->f20 / self->f24;
    sV4_3418 col = self->col;
    r = 1.0f - r * r;
    col.x *= r;
    *(short*)((char*)GrMan->m_pCurrentDrawState + 0x10) = ((sWorld_3418*)GrMan)->GetFB();
    ((cGfx_3418*)cGraphicsMan_TheGraphicsMan)->DrawX((self->f38 + self->f20 / self->f24 * self->f3C) * 0.01745329424738884f, sPos_3418(self->f8 + off->x, self->fC + off->y), self->f10, *(sV2_3418*)&D_004A5510_3418, *(sV2_3418*)&D_004A5520_3418, col, 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", cSplashManager_cSplashManager);
#ifdef SKIP_ASM
extern "C" void* cObjNode_cObjNode1(void* self, void* a1);
extern "C" int* func_002F2588(int* arg0);
extern "C" void* func_002F2D60(void* arg0);
extern "C" void func_002F39C8(void* arg0);
extern char cSplashManager_vtbl[];

/**
 * Constructs a splash manager for one view: an object node with empty pools of drops and ice crystals.
 *
 * Runs the cObjNode constructor with @p bucket, stores @p viewIndex (+0x10) and the vtable (+0xC), builds 30
 * drops (0x3C bytes from +0x1C) and 30 ice crystals (0x4C bytes from +0x724), clears the rest of the state
 * (+0x101C = 1.0) and resets it (func_002F39C8). Drop and crystal are the SplashToggles debug menu's words.
 *
 * @param p          the object to construct (this)
 * @param bucket     passed to the cObjNode constructor (On Tour: const int bucket)
 * @param viewIndex  the view whose camera the splashes belong to
 * @return the constructed object (@p p)
 */
// Particle emitter constructor: builds the two 30-entry element pools and resets the emitter state.
extern "C" void* cSplashManager_cSplashManager(void* p, void* bucket, int viewIndex)
{
    char* self = (char*)p;
    cObjNode_cObjNode1(self, bucket);
    *(int*)(self + 0x10) = viewIndex;
    *(int*)(self + 0x14) = 0;
    *(char**)(self + 0xC) = cSplashManager_vtbl;
    *(int*)(self + 0x18) = 0;
    {
        char* entry = self + 0x1C;
        int i = 29;
        do {
            func_002F2588((int*)entry);
            i--;
            entry += 0x3C;
        } while (i != -1);
    }
    {
        char* entry = self + 0x724;
        int i = 29;
        do {
            func_002F2D60(entry);
            i--;
            entry += 0x4C;
        } while (i != -1);
    }
    *(int*)(self + 0x100C) = 0;
    *(int*)(self + 0x1010) = 0;
    *(int*)(self + 0x1014) = 0;
    *(int*)(self + 0x1018) = 0;
    *(float*)(self + 0x101C) = 1.0f;
    *(int*)(self + 0x1020) = 0;
    *(int*)(self + 0x1024) = 0;
    func_002F39C8(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", cSplashManager__cSplashManager);
#ifdef SKIP_ASM
extern "C" void cObjNode__cObjNode(void *);
extern char cSplashManager_vtbl[];

/**
 * Destroys a splash manager: restores its vtable (+0xC), then runs the cObjNode destructor.
 *
 * The g++ delete flags arrive in the second argument register, which this body leaves untouched, so
 * cObjNode__cObjNode() receives them and frees the object when bit 0 is set.
 */
extern "C" void cSplashManager__cSplashManager(void *self) {
    *(char **)((char*)self + 0xC) = cSplashManager_vtbl;
    cObjNode__cObjNode(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1F1548", func_002F3640);
#ifdef SKIP_ASM
struct sVec2_3640 {
    float x, y;
};

struct sPart_3640 {
    char d[0x4C];
};

struct sEmit_3640 {
    char pad[0x18];
    int count;
    char pad1C[0x724 - 0x1C];
    sPart_3640 parts[1];
};

extern int D_004A3AFC;
extern int D_004A4258;
extern float D_004A42D0;
extern float D_004A42D4;
extern "C" void func_002F2D70(sPart_3640*, sVec2_3640*, int);

// PORT: float bit-pattern random number in [0,1) (seed reused as float storage).
static inline float randf_3640()
{
    D_004A3AFC = ((D_004A3AFC * 0x18FCD + 0xE9507C) & 0x7FFFFF) | 0x3F800000;
    return *(float*)&D_004A3AFC - 1.0f;
}

static inline float randRange_3640(float lo, float hi)
{
    return lo + (hi - lo) * randf_3640();
}

extern "C" void func_002F3640(sEmit_3640* self, sVec2_3640* p, int n)
{
    for (int i = 0; i < n; i++) {
        if (self->count < D_004A4258) {
            sVec2_3640 off;
            off.x = randRange_3640(-D_004A42D0, D_004A42D0);
            off.y = randRange_3640(-D_004A42D4, D_004A42D4);
            sVec2_3640 pos;
            pos.x = p->x + off.x;
            pos.y = p->y + off.y;
            func_002F2D70(&self->parts[self->count], &pos, i);
            self->count++;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1F1548", func_002F37A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S37A8 { int a, b; S37A8() {} S37A8(const S37A8 &o) { *this = o; } };
extern "C" void func_002F2598(void *, S37A8, void *);
extern int D_004A4254;

extern "C" void func_002F37A8_37A8(void *arg0, S37A8 *arg1, void *arg2) __asm__("func_002F37A8");
extern "C" void func_002F37A8_37A8(void *arg0, S37A8 *arg1, void *arg2) {
    int n = *(int *)((char*)arg0 + 0x14);
    if (n < D_004A4254) {
        func_002F2598((char*)arg0 + (n * 0x3C + 0x1C), *arg1, arg2);
        *(int *)((char*)arg0 + 0x14) = *(int *)((char*)arg0 + 0x14) + 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1F1548", func_002F3810);
#ifdef SKIP_ASM
struct sVec2_3810 {
    float x, y;
};

extern int D_004A3AFC;
extern float D_004A425C;
extern int D_004A4264;
extern int D_004A4268;
extern float D_004A4298;
unsigned int BXrand();
extern "C" void func_002F3640_3810(void*, sVec2_3810*, int) __asm__("func_002F3640");
extern "C" void func_002F37A8(void*, sVec2_3810*, sVec2_3810*);

// PORT: float bit-pattern random number in [0,1) (seed reused as float storage).
static inline float randf_3810()
{
    D_004A3AFC = ((D_004A3AFC * 0x18FCD + 0xE9507C) & 0x7FFFFF) | 0x3F800000;
    return *(float*)&D_004A3AFC - 1.0f;
}

static inline float randScaled_3810(float s)
{
    return randf_3810() * s;
}

extern "C" void func_002F3810(char* self)
{
    float acc = *(float*)(self + 0x1024);
    int n = (int)acc;
    *(float*)(self + 0x1024) = acc - (float)n;
    while (n > 0) {
        sVec2_3810 pos;
        pos.x = randScaled_3810(640.0f);
        pos.y = randScaled_3810(480.0f);
        if (randf_3810() < D_004A425C) {
            int k = D_004A4264 + BXrand() % (unsigned int)(D_004A4268 - D_004A4264);
            if (k <= 0)
                k = 1;
            n -= k;
            func_002F3640_3810(self, &pos, k);
        } else {
            sVec2_3810 v;
            v.x = D_004A4298;
            v.y = D_004A4298;
            func_002F37A8(self, &pos, &v);
            n--;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F39C8);
#ifdef SKIP_ASM
extern "C" void func_002F39C8(void *arg0) {
    *(int *)((char*)arg0 + 0x100C) = 0;
    *(int *)((char*)arg0 + 0x1024) = 0;
    *(int *)((char*)arg0 + 0x14) = 0;
    *(int *)((char*)arg0 + 0x18) = 0;
    *(int *)((char*)arg0 + 0x1028) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", cSplashManager_update);
#ifdef SKIP_ASM
extern int D_004A424C;
extern int D_004A4258;
extern float D_004A4260;
extern int D_004A426C;
// NOTE: tunable float member of a global record; splat names the field separately.
struct sTun_39E0 {
    float v;
};
extern sTun_39E0 D_004A4294_39E0 __asm__("D_004A4294");
unsigned int BXrand();

struct sV4_39E0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sV2_39E0 {
    float x, y;
    sV2_39E0() {}
    sV2_39E0(float ax, float ay) : x(ax), y(ay) {}
    sV2_39E0 operator*(float s) const { return sV2_39E0(x * s, y * s); }
    float& X() { return x; }
    float& Y() { return y; }
};

struct sSmoke_39E0 {
    int active;
    char pad[0x3C - 4];
};

struct sPart_39E0 {
    int bursts;
    int active;
    sV2_39E0 pos;
    sV2_39E0 size;
    char pad[0x4C - 0x18];
};

struct sBase_39E0 {
    int pad[3];
};

struct sVBase_39E0 : sBase_39E0 {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void Reset();
};

struct sEmitter_39E0 : sVBase_39E0 {
    unsigned int idx;
    int nSmoke;
    int nParts;
    sSmoke_39E0 smoke[30];
    sPart_39E0 parts[30];
    int hasPrev;
    sV4_39E0 prevPos;
    float speed;
};

extern "C" void func_002F3810(char* self);
extern "C" void func_002F2810_39E0(sSmoke_39E0*, float) __asm__("func_002F2810");
extern "C" void func_002F3030_39E0(sPart_39E0*, float) __asm__("func_002F3030");
extern "C" void func_002F37A8_39E0(void*, sV2_39E0*, sV2_39E0*) __asm__("func_002F37A8");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline void Sub_39E0(sV4_39E0* out, const sV4_39E0& a, const sV4_39E0& b)
{
    sV4_39E0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    *out = r;
}

// PORT: PS2-only VU0 inline asm (vector length via vsqrt).
static inline float Length_39E0(const sV4_39E0& v)
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

/**
 * Updates this view's screen splashes: tracks the camera speed, spawns ice crystals and lets them shed drops.
 *
 * Needs SplashToggles "Enable" (D_004A424C) and an existing view. The speed is the view position's change per
 * frame times 60 * 0.036 (probably km/h); above 1000 (a camera cut) everything resets. func_002F3810 spawns
 * crystals, every drop and crystal is updated with the speed and dead ones are replaced by the last. With a
 * chance set by "Percentage Ice Crystal Spawn", one random crystal sheds a drop of 0.6 x its size, at most
 * "Max Spawn Per Crystal" times and only above "Drop Min Size".
 */
extern "C" void cSplashManager_update(sEmitter_39E0* self)
{
    if (D_004A424C == 0)
        return;
    char* w = (char*)SSXApp->Game->ViewMan;
    if (self->idx >= *(unsigned int*)(w + 0x10))
        return;
    sV4_39E0 pos = *(sV4_39E0*)(*(char**)(w + (self->idx << 2) + 4) + 0x20);
    if (self->hasPrev) {
        sV4_39E0 moved;
        Sub_39E0(&moved, pos, self->prevPos);
        self->speed = Length_39E0(moved) * 59.999996185302734f * 0.035999998450279236f;
    } else {
        self->speed = 0.0f;
        self->hasPrev = 1;
    }
    if (self->speed > 1000.0f) {
        self->Reset();
        self->speed = 0.0f;
    }
    self->prevPos = pos;
    func_002F3810((char*)self);

    int i = 0;
    while (i < self->nSmoke) {
        sSmoke_39E0* sm = &self->smoke[i];
        func_002F2810_39E0(sm, self->speed);
        if (sm->active) {
            i++;
        } else {
            self->smoke[i] = self->smoke[self->nSmoke - 1];
            self->nSmoke--;
        }
    }

    int pick = -1;
    union {
        unsigned int i;
        float f;
    } rnd;
    rnd.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    if ((rnd.f - 1.0f) * (float)self->nParts / (float)D_004A4258 < D_004A4260 && self->nParts > 0)
        pick = BXrand() % self->nParts;

    i = 0;
    while (i < self->nParts) {
        sPart_39E0* part = &self->parts[i];
        func_002F3030_39E0(part, self->speed);
        if (part->active) {
            if (pick == i && self->parts[i].bursts < D_004A426C) {
                self->parts[i].bursts++;
                sV2_39E0 sz = part->size * 0.6000000238418579f;
                if (sz.X() > D_004A4294_39E0.v && sz.Y() > D_004A4294_39E0.v)
                    func_002F37A8_39E0(self, &self->parts[i].pos, &sz);
            }
            i++;
        } else {
            self->parts[i] = self->parts[self->nParts - 1];
            self->nParts--;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1F1548", cSplashManager_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A424C;
extern int D_004A4250;
extern int D_004A3B60;
extern char t4mat_Identity[];

struct sV2_3E28 {
    float x, y;
    sV2_3E28() {}
    sV2_3E28(const sV2_3E28& o) : x(o.x), y(o.y) {}
    sV2_3E28& operator=(const sV2_3E28& o)
    {
        x = o.x;
        y = o.y;
        return *this;
    }
};

struct sGsState_3E28 {
    unsigned int w0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 10;
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
    unsigned int w10;
};

extern sGsState_3E28 D_00501420;

static inline void SetF22_3E28(sGsState_3E28* s, int v)
{
    s->f4_22 = v;
}

class cGfx_3E28 {
public:
    char pad[0xE84];
    sGsState_3E28* stateTop;
    char pad2[0x10D8 - 0xE88];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void BeginScene();
    virtual void EndScene();
    virtual void v22(); virtual void v23(); virtual void v24();
    virtual void SetViewport(float, float, float, float, int, int, float, float);
    virtual void v26();
    virtual sV2_3E28 GetScreenSize();
    virtual void v28(); virtual void v29();
    virtual void Begin2D();
    virtual void End2D();
    virtual void v32();
    virtual void SetTexture(void*);
};


struct sBase_3E28 {
    int pad[3];
};

struct sVBase_3E28 : sBase_3E28 {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void Reset();
};

struct sEmitter_3E28 : sVBase_3E28 {
    int idx;
    int nSmoke;
    int nParts;
    char smoke[30][0x3C];
    char parts[30][0x4C];
};

extern "C" void func_002F2C30(void*, sV2_3E28*);
extern void func_002F3418_3E28(void*, sV2_3E28*) __asm__("func_002F3418");

/**
 * Draws this view's splash drops and ice crystals as 2D sprites over the 640x480 screen.
 *
 * Needs SplashToggles "Enable" and "Render" and this view being the one rendered. When D_004A3B60 (a
 * script-controlled flag that also gates other lens effects) is set, it resets the splashes instead. Draws in
 * its own 2D scene with a pushed copy of the default draw state (D_00501420) and the current screen size.
 */
extern "C" void cSplashManager_render(sEmitter_3E28* self)
{
    if (D_004A424C == 0)
        return;
    if (D_004A4250 == 0)
        return;
    if (self->idx != *(int*)((char*)SSXApp->Game->ViewMan + 0x14))
        return;
    if (D_004A3B60) {
        self->Reset();
        return;
    }
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    int i = 0;
    ((cGfx_3E28*)ps2GraphicsMan)->BeginScene();
    const sV2_3E28& screen = ((cGfx_3E28*)ps2GraphicsMan)->GetScreenSize();
    sV2_3E28 scr = screen;
    ((cGfx_3E28*)ps2GraphicsMan)->SetViewport(0.0f, 0.0f, 640.0f, 480.0f, 1, 0, 0.0f, 1.0f);
    ((cGfx_3E28*)ps2GraphicsMan)->Begin2D();
    ((cGfx_3E28*)ps2GraphicsMan)->SetTexture(t4mat_Identity);
    ((cGfx_3E28*)ps2GraphicsMan)->stateTop[1] = ((cGfx_3E28*)ps2GraphicsMan)->stateTop[0];
    ((cGfx_3E28*)ps2GraphicsMan)->stateTop++;
    *((cGfx_3E28*)ps2GraphicsMan)->stateTop = D_00501420;
    ((cGfx_3E28*)ps2GraphicsMan)->stateTop->f8_5 = 8;
    ((cGfx_3E28*)ps2GraphicsMan)->stateTop->f4_23 = 2;
    ((cGfx_3E28*)ps2GraphicsMan)->stateTop->f4_20 = 0;
    ((cGfx_3E28*)ps2GraphicsMan)->stateTop->f4_12 = 0;
    ((cGfx_3E28*)ps2GraphicsMan)->stateTop->f4_0 = 0;
    ((cGfx_3E28*)ps2GraphicsMan)->stateTop->f8_10 = 0;
    SetF22_3E28(((cGfx_3E28*)ps2GraphicsMan)->stateTop, 1);
    for (i = 0; i < self->nSmoke; i++)
        func_002F2C30(self->smoke[i], &scr);
    for (i = 0; i < self->nParts; i++)
        func_002F3418_3E28(self->parts[i], &scr);
    ((cGfx_3E28*)ps2GraphicsMan)->stateTop--;
    ((cGfx_3E28*)ps2GraphicsMan)->End2D();
    ((cGfx_3E28*)ps2GraphicsMan)->EndScene();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F4118);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void *func_002306A8(void *, int);
extern "C" void func_002F4260_4118(void *, float, void *) __asm__("func_002F4260");
struct N4118 { char pad[0x84]; N4118 *c; };
struct M4118 { char pad[0x10]; unsigned int cnt; };

extern "C" void func_002F4118(void *arg0, float arg1) {
    unsigned int i = 0;
    if (((M4118 *)((N4118 *)((N4118 *)SSXApp)->c)->c)->cnt != 0) {
        do {
            func_002F4260_4118(func_002306A8(((N4118 *)SSXApp)->c, i++), arg1, arg0);
        } while (i < ((M4118 *)((N4118 *)((N4118 *)SSXApp)->c)->c)->cnt);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F41A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Base41A8 { int pad[3]; };
struct Obj41A8 : Base41A8 {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
};
Obj41A8 *func_002306A8_41A8(void *, int) __asm__("func_002306A8__FPvi");
#define F84(p) (*(char **)((char*)(p) + 0x84))

extern "C" void func_002F41A8(int arg0) {
    unsigned int i = 0;
    if (*(unsigned int *)(F84(F84(SSXApp)) + 0x10) != 0) {
        do {
            char *t5 = F84(SSXApp);
            if (arg0 == *(int *)(F84(t5) + (i << 2) + 4)) {
                if (func_002306A8_41A8(t5, i) != 0) {
                    Obj41A8 *o = func_002306A8_41A8(F84(SSXApp), i);
                    o->v13();
                }
            }
            i += 1;
        } while (i < *(unsigned int *)(F84(F84(SSXApp)) + 0x10));
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F4260);
#ifdef SKIP_ASM
struct sV4_4260 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sV4_4260 Sub_4260(const sV4_4260& a, const sV4_4260& b)
{
    sV4_4260 r;
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

// PORT: PS2-only VU0 inline asm (vector length via vsqrt).
static inline float Length_4260(const sV4_4260& v)
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

extern float D_004A4270;
extern float D_004A4274;
extern float D_004A4278;
extern float D_004A427C;

extern "C" void func_002F4260(char* self, sV4_4260* p, float speed)
{
    float k = 1.0f - Length_4260(Sub_4260(*(sV4_4260*)(self + 0x1010), *p)) / D_004A4270;
    if (k > 0.0f && *(float*)(self + 0x1028) > D_004A4278) {
        float lo = D_004A4274;
        float t = (speed * 0.036f - lo) / (120.0f - lo);
        float c = (t >= 0.0f) ? (t <? 1.0f) : 0.0f;
        *(float*)(self + 0x1024) += k * c * D_004A427C;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F4330);
#ifdef SKIP_ASM
extern float D_004A4280;
extern float D_004A4284;

extern "C" void func_002F4330(void *arg0, float arg1) {
    *(float *)((char*)arg0 + 0x1028) = arg1;
    if (D_004A4280 < arg1) {
        *(float *)((char*)arg0 + 0x1024) += arg1 * (*(float *)((char*)arg0 + 0x1020) + 10.0f) * (D_004A4284 * 0.007692307699471712f);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F4380);
#ifdef SKIP_ASM
extern void func_002E2860(void *);
extern "C" void func_002F43E0(void *);
inline void* operator new[](unsigned int, void* p) { return p; }
struct Elem4380 { Elem4380() {} };

extern "C" void *func_002F4380(void *arg0) {
    func_002E2860(arg0);
    new ((char*)arg0 + 0x40) Elem4380[16];
    *(int *)((char*)arg0 + 0x204) = 0;
    *(short *)((char*)arg0 + 0x2E) = 0;
    *(short *)((char*)arg0 + 0x2C) = 0;
    func_002F43E0(arg0);
    return arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F43E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sCol_43E0 {
    float r, g, b, a;
};

extern sCol_43E0 D_004FAB80;
extern sCol_43E0 D_004FABA0;
extern sCol_43E0 D_004FABC0;
extern sCol_43E0 D_004FABE0;
extern sCol_43E0 D_004FAC00;
extern sCol_43E0 D_004FAC20;
extern sCol_43E0 D_004FAC40;
extern sCol_43E0 D_004FAC60;
extern sCol_43E0 D_004FAC80;

struct sLight_43E0 {
    int type;
    float a;
    float b;
    sCol_43E0 col;
};

struct sLightSet_43E0 {
    char pad[0x30];
    sLight_43E0 lights[16];
    char pad1F0[0x10];
    int count;
};

extern "C" void func_002F43E0_43E0(sLightSet_43E0* self) __asm__("func_002F43E0");
extern "C" void func_002F43E0_43E0(sLightSet_43E0* self)
{
    self->count = 0;
    self->lights[0].type = 1;
    self->lights[0].a = 1.2999999523162842f;
    self->lights[0].b = 0.03999999910593033f;
    self->lights[0].col = D_004FAB80;
    self->count++;
    self->lights[1].type = 3;
    self->lights[1].a = 0.595413327217102f;
    self->lights[1].b = 0.03453768044710159f;
    self->lights[1].col = D_004FABA0;
    self->count++;
    self->lights[2].type = 0;
    self->lights[2].a = 0.5f;
    self->lights[2].b = 0.20000000298023224f;
    self->lights[2].col = D_004FABC0;
    self->count++;
    self->lights[3].type = 2;
    self->lights[3].a = 0.20000000298023224f;
    self->lights[3].b = 0.025077050551772118f;
    self->lights[3].col = D_004FABE0;
    self->count++;
    self->lights[4].type = 0;
    self->lights[4].a = 0.0f;
    self->lights[4].b = 0.03999999910593033f;
    self->lights[4].col = D_004FAC00;
    self->count++;
    self->lights[5].type = 0;
    self->lights[5].a = -0.5141515731811523f;
    self->lights[5].b = 0.06998095661401749f;
    self->lights[5].col = D_004FAC20;
    self->count++;
    self->lights[6].type = 1;
    self->lights[6].a = -0.40145567059516907f;
    self->lights[6].b = 0.030118949711322784f;
    self->lights[6].col = D_004FAC40;
    self->count++;
    self->lights[7].type = 3;
    self->lights[7].a = -0.6160849332809448f;
    self->lights[7].b = 0.03999999910593033f;
    self->lights[7].col = D_004FAC60;
    self->count++;
    self->lights[8].type = 1;
    self->lights[8].a = -1.0f;
    self->lights[8].b = 0.0206892192363739f;
    self->lights[8].col = D_004FAC80;
    self->count++;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F4690);
#ifdef SKIP_ASM
extern "C" float func_002EE268(int);

struct sV2_4690 {
    float x, y;
    sV2_4690() {}
    sV2_4690(const float& ax, const float& ay) : x(ax), y(ay) {}
};

static inline sV2_4690 operator-(const sV2_4690& a, const sV2_4690& b)
{
    return sV2_4690(a.x - b.x, a.y - b.y);
}

static inline sV2_4690 operator+(const sV2_4690& a, const sV2_4690& b)
{
    return sV2_4690(a.x + b.x, a.y + b.y);
}

static inline sV2_4690 operator*(const sV2_4690& a, float s)
{
    return sV2_4690(a.x * s, a.y * s);
}

struct sV4_4690 {
    float x, y, z, w;
    sV4_4690(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
};

struct sCol_4690 {
    float r, g, b, a;
};

struct sLight_4690 {
    int type;
    float dist;
    float size;
    sCol_4690 col;
};

struct sFlare_4690 {
    char pad[0x30];
    sLight_4690 lights[16];
    float sunX;
    float sunY;
    char pad1F8[0x8];
    int count;
    float intensity;
};

struct sGs_4690 {
    unsigned int w0, w4, w8, wC;
    unsigned int f10 : 16;
    unsigned int f10hi : 16;
    void SetF10(int v) { f10 = v; }
};

class cWorld_4690 {
public:
    char pad[0xE84];
    sGs_4690* stateTop;
    char padE88[0x1004 - 0xE88];
    int f1004;
    int GetF1004() { return f1004; }
    char pad1008[0x10D8 - 0x1008];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8();
    virtual unsigned int GetWidth();
    virtual unsigned int GetHeight();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
    virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70();
    virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76();
    virtual void DrawSprite(const sV4_4690&, const sV2_4690&, const sV2_4690&, const sV2_4690&, const sCol_4690&);
};


extern "C" void func_002F4690(sFlare_4690* self)
{
    float cx = (float)((cWorld_4690*)GrMan)->GetWidth() * 0.5f;
    float cy = (float)((cWorld_4690*)GrMan)->GetHeight() * 0.5f;
    sV2_4690 center(cx, cy);
    sV2_4690 sun(self->sunX, self->sunY);
    sV2_4690 dir = sun - center;
    cPS2GraphicsMan* w = GrMan;
    ((cWorld_4690*)w)->stateTop->SetF10(((cWorld_4690*)w)->GetF1004());
    for (int i = 0; i < self->count; i++) {
        sV2_4690 pos = center + dir * self->lights[i].dist;
        sV2_4690 uv0;
        sCol_4690 col = self->lights[i].col;
        col.r *= self->intensity * func_002EE268(6);
        sV2_4690 uv1;
        switch (self->lights[i].type) {
        case 0:
            uv0 = sV2_4690(0.0f, 0.0f);
            uv1 = sV2_4690(0.5f, 0.5f);
            break;
        case 1:
            uv0 = sV2_4690(0.5f, 0.0f);
            uv1 = sV2_4690(1.0f, 0.5f);
            break;
        case 2:
            uv0 = sV2_4690(0.0f, 0.5f);
            uv1 = sV2_4690(0.5f, 1.0f);
            break;
        case 3:
            uv0 = sV2_4690(0.5f, 0.5f);
            uv1 = sV2_4690(1.0f, 1.0f);
            break;
        default:
            uv0 = sV2_4690(0.0f, 0.0f);
            uv1 = sV2_4690(0.5f, 1.0f);
            break;
        }
        float s = self->lights[i].size * 300.0f;
        sV2_4690 sz(s, s);
        ((cWorld_4690*)GrMan)->DrawSprite(sV4_4690(pos.x, pos.y, 0.0f, 1.0f), sz, uv0, uv1, col);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F4A08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" float func_002EE100(int);
extern "C" float func_002EE148(int);
extern "C" float func_002EE190(int);
extern "C" float func_002EE1D8(int);
extern "C" int func_002EE220(int);
extern "C" float func_002EE2B0(int);
extern "C" void func_002F4690_4A08(void*, void*) __asm__("func_002F4690");
extern char t4mat_Identity[];

struct sV2_4A08 {
    float x, y;
    sV2_4A08(const float& ax, const float& ay) : x(ax), y(ay) {}
};

struct sV4_4A08 {
    float x, y, z, w;
    sV4_4A08() {}
    sV4_4A08(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
};

struct sGsState_4A08 {
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
    unsigned int f10 : 16;
    unsigned int f10hi : 16;
    void SetF22(int v) { f4_22 = v; }
};

extern sGsState_4A08 D_00501420_4A08 __asm__("D_00501420");

class cGfx_4A08 {
public:
    char pad[0xE84];
    sGsState_4A08* stateTop;
    char padE88[0xF50 - 0xE88];
    int texTab[(0x10D8 - 0xF50) / 4];
    int GetTexId(int i) { return texTab[i + 0x33]; }
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void BeginScene();
    virtual void EndScene();
    virtual void v22(); virtual void v23(); virtual void v24();
    virtual void SetViewport(float, float, float, float, int, int, float, float);
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void Begin2D();
    virtual void End2D();
    virtual void v32();
    virtual void SetTexture(void*);
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57();
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73();
    virtual void v74(); virtual void v75(); virtual void v76();
    virtual void DrawSprite(const sV4_4A08&, const sV2_4A08&, const sV2_4A08&, const sV2_4A08&, const sV4_4A08&);
};


struct sSun_4A08 {
    char pad[0x2C];
    unsigned short visible;
    char pad2E[0x1F0 - 0x2E];
    float sunX;
    float sunY;
    char pad1F8[0x204 - 0x1F8];
    float intensity;
};

extern "C" void func_002F4A08(sSun_4A08* self, sV4_4A08* vp, int flag)
{
    if (self->visible == 0)
        return;
    int id = !flag ? 6 : 7;
    float r = func_002EE100(id);
    float g = func_002EE148(id);
    float b = func_002EE190(id);
    sV4_4A08 col;
    col = sV4_4A08(1.0f, r, g, b);
    col.x *= self->intensity * func_002EE1D8(id);
    float size = func_002EE2B0(id);
    int tex = func_002EE220(id);
    if (size == 1.0f)
        size = 320.0f;
    if (col.x <= 0.0f)
        return;
    cPS2GraphicsMan* gfx = cGraphicsMan_TheGraphicsMan;
    ((cGfx_4A08*)gfx)->stateTop[1] = ((cGfx_4A08*)gfx)->stateTop[0];
    ((cGfx_4A08*)gfx)->stateTop++;
    *((cGfx_4A08*)gfx)->stateTop = D_00501420_4A08;
    ((cGfx_4A08*)gfx)->BeginScene();
    ((cGfx_4A08*)gfx)->SetViewport(vp->x, vp->y, vp->z, vp->w, 1, 0, 0.0f, 1.0f);
    ((cGfx_4A08*)gfx)->Begin2D();
    ((cGfx_4A08*)gfx)->SetTexture(t4mat_Identity);
    ((cGfx_4A08*)gfx)->stateTop->SetF22(1);
    ((cGfx_4A08*)gfx)->stateTop->f4_23 = 2;
    ((cGfx_4A08*)gfx)->stateTop->f4_20 = 0;
    ((cGfx_4A08*)gfx)->stateTop->f4_12 = 0x14;
    ((cGfx_4A08*)gfx)->stateTop->f4_0 = 0;
    ((cGfx_4A08*)gfx)->stateTop->f8_10 = 0;
    ((cGfx_4A08*)gfx)->stateTop->f4_2 = 7;
    ((cGfx_4A08*)gfx)->stateTop->f8_5 = 8;
    ((cGfx_4A08*)gfx)->stateTop->f10 = ((cGfx_4A08*)gfx)->GetTexId(tex);
    sV4_4A08 pos(self->sunX, self->sunY, 0.0f, 1.0f);
    ((cGfx_4A08*)gfx)->DrawSprite(pos, sV2_4A08(size, size),
                    sV2_4A08(0.0f, 0.0f), sV2_4A08(1.0f, 1.0f), col);
    func_002F4690_4A08(self, vp);
    ((cGfx_4A08*)gfx)->stateTop--;
    ((cGfx_4A08*)gfx)->End2D();
    ((cGfx_4A08*)gfx)->EndScene();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F4DB8);
#ifdef SKIP_ASM
extern "C" int func_002EE220(int);
extern "C" float func_002EE070(int);
extern "C" float func_002EE0B8(int);
extern "C" void func_0031BE50(float, float*, float*);

struct sV4_4DB8 {
    float x, y, z, w;
    sV4_4DB8() {}
    sV4_4DB8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sRect_4DB8 {
    float x, y, w, h;
};

struct sCam_4DB8 {
    sRect_4DB8 vp;
    sV4_4DB8 m[4];
    char pad50[8];
    float farClip;
    void MulVec(sV4_4DB8* out, const sV4_4DB8& v) const
    {
        *out = v;
        out->x = v.x * m[0].x + v.y * m[0].y + v.z * m[0].z + v.w * m[0].w;
        out->y = v.x * m[1].x + v.y * m[1].y + v.z * m[1].z + v.w * m[1].w;
        out->z = v.x * m[2].x + v.y * m[2].y + v.z * m[2].z + v.w * m[2].w;
        out->w = v.x * m[3].x + v.y * m[3].y + v.z * m[3].z + v.w * m[3].w;
    }
};

struct sSun_4DB8 {
    sRect_4DB8 rect;
    sRect_4DB8 clip;
    float f20;
    float depth;
    char pad28[4];
    unsigned short visible;
    unsigned short onScreen;
    char pad30[0x1F0 - 0x30];
    sV4_4DB8 screen;
    char pad200[4];
    float intensity;
};

class cGfx_4DB8 {
public:
    char pad[0x10D8];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87();
    virtual void v88(); virtual void v89();
    virtual int IsCulled(const sV4_4DB8&);
    virtual sV4_4DB8 Project(const sV4_4DB8&);
};


// PORT: PS2-only VU0 inline asm (in-place vector * scalar).
static inline void ScaleEq_4DB8(sV4_4DB8& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(v), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void AddEq_4DB8(sV4_4DB8& a, const sV4_4DB8& b)
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

static inline float clamp_4DB8(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo; // PORT: g++ <? operator
}

extern "C" void func_002F4DB8(sSun_4DB8* self, sCam_4DB8* cam, int flag)
{
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    if (func_002EE220(6) == -1) {
        self->visible = 0;
        self->onScreen = 0;
        return;
    }
    float dist = cam->farClip - 500.0f;
    int id = !flag ? 6 : 7;
    float a = func_002EE0B8(id) * 0.01745329424738884f;
    float b = func_002EE070(id) * 0.01745329424738884f;
    float sa, ca, sb, cb;
    if (a == 0.0f && b == 0.0f) {
        func_0031BE50(1.8675024509429932f, &sa, &ca);
        func_0031BE50(0.27925270795822144f, &sb, &cb);
    } else {
        func_0031BE50(a, &sa, &ca);
        func_0031BE50(b, &sb, &cb);
    }
    sV4_4DB8 dir;
    sV4_4DB8 sun;
    {
        // PORT: PS2-only VU0 inline asm (dir = row 3 of the view matrix * -1; written out in place:
        // an inline helper's parameter/frame pseudo keeps &temp in an s-reg up to the Project call).
        sV4_4DB8 v = cam->m[3];
        sV4_4DB8 r;
        int t;
        __asm__(
            "mfc1      %1, %3\n"
            "lqc2      $vf4, %2\n"
            "qmtc2.ni  %1, $vf3\n"
            "vmulx.xyzw $vf5, $vf4, $vf3x\n"
            "sqc2      $vf5, %0\n"
            : "=m"(r), "=&r"(t)
            : "m"(v), "f"(-1.0f)
            : "memory");
        dir = r;
    }
    cam->MulVec(&sun, dir);
    dir = sun;
    dir.w = 1.0f;
    sun.x = ca * cb;
    sun.y = sa * cb;
    sun.z = sb;
    sun.w = 0.0f;
    ScaleEq_4DB8(sun, dist);
    AddEq_4DB8(sun, dir);
    if (((cGfx_4DB8*)ps2GraphicsMan)->IsCulled(sun)) {
        self->visible = 0;
        self->onScreen = 0;
        self->intensity = 0.0f;
        return;
    }
    self->visible = 1;
    self->onScreen = 1;
    self->screen = ((cGfx_4DB8*)ps2GraphicsMan)->Project(sun);
    self->rect.x = self->screen.x - 8.0f;
    self->rect.y = self->screen.y - 8.0f;
    self->rect.w = 16.0f;
    self->rect.h = 16.0f;
    self->clip = self->rect;
    self->clip.x = clamp_4DB8(self->clip.x, 0.0f, cam->vp.x + cam->vp.w - 16.0f);
    self->clip.y = clamp_4DB8(self->clip.y, 0.0f, cam->vp.x + cam->vp.h - 16.0f);
    self->depth = self->screen.z;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F5148);
#ifdef SKIP_ASM
extern "C" void func_002F5400(int, void *, int);

extern "C" void func_002F5148(void *arg0) {
    func_002F5400(((int)SSXApp->Game->WorldLightMan), arg0, ((int) (*(int *)((char*)(arg0) + (8))) >> 0xC) & 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", cWorldLightMan_cWorldLightMan);
#ifdef SKIP_ASM
extern "C" void cWorldLightMan_initLightCache(void *);
struct cWorldPainterManArg;
void *cWorldPainterMan_getQuery(void *, cWorldPainterManArg *);
extern char kWPR_Lighting;
struct A5180 { unsigned int v; char pad[0x3C]; A5180() { v = 0xFFFFFFFF; } };
struct B5180 { unsigned int v; char pad[0x17C]; B5180() { v = 0xFFFFFFFF; } };

/**
 * Constructs the world light manager: empty light tables, the light cache and a lighting query.
 *
 * Marks the 8 0x40-byte entries at +0x10 and the 32 0x180-byte entries at +0x210 unused (first word -1),
 * keeps @p pNash at +0x0, builds the light cache (cWorldLightMan_initLightCache) and stores at +0x4 the world
 * painter manager's query for kWPR_Lighting.
 *
 * @param pNash  the world's NASH data (On Tour: cBXNash*), passed as an int
 * @return self
 */
extern "C" void *cWorldLightMan_cWorldLightMan(void *self, int pNash) {
    new ((char*)self + 0x10) A5180[8];
    new ((char*)self + 0x210) B5180[32];
    *(int *)self = pNash;
    cWorldLightMan_initLightCache(self);
    char *world = (char*)SSXApp->Game->World;
    *(void **)((char*)self + 4) = cWorldPainterMan_getQuery(*(void **)(world + 8), (cWorldPainterManArg *)&kWPR_Lighting);
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F5230);
#ifdef SKIP_ASM
extern "C" void func_002F53B0(void *);
void operator_delete(int *);
struct Base5230 { int pad; };
struct Inner5230 : Base5230 { virtual ~Inner5230(); };
struct Outer5230 { int pad; Inner5230 *p; };

extern "C" void func_002F5230(Outer5230 *arg0, int arg1) {
    if (arg0->p != 0) {
        delete arg0->p;
    }
    func_002F53B0(arg0);
    if (arg1 & 1) {
        operator_delete((int *)arg0);
    }
}
#endif
