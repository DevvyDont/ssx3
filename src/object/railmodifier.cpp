#include "common.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

INCLUDE_ASM("object/railmodifier", cRailModifier_buildXform);

INCLUDE_ASM("object/railmodifier", func_0035C4E0);

INCLUDE_ASM("object/railmodifier", func_0035C540);

INCLUDE_ASM("object/railmodifier", func_0035C5A0);

INCLUDE_ASM("object/railmodifier", func_0035C698);

//100%
INCLUDE_ASM("object/railmodifier", func_0035CFE0__FPv);
#ifdef SKIP_ASM
void func_0035CFE0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035CFE8__FPv);
#ifdef SKIP_ASM
void* func_0035CFE8(void* self)
{
    return self;
}
#endif

INCLUDE_ASM("object/railmodifier", func_0035CFF0);

//100%
INCLUDE_ASM("object/railmodifier", func_0035D288);
#ifdef SKIP_ASM
struct sRmVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRmMat33 {
    float m[3][3];
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sRmVec4 vu0ScaleRM(const sRmVec4& v, float s)
{
    sRmVec4 r;
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

struct sRmBody {
    char pad_0x00[0x20];
    sRmVec4 dir;        // 0x20
    sRmVec4 pos;        // 0x30
    float scale;        // 0x40
    char pad_0x44[0xC];
    sRmMat33 rot;       // 0x50
    char pad_0x74[0xC];
    sRmVec4 outDir;     // 0x80
    sRmVec4 outPos;     // 0x90
};

static inline sRmVec4 xformRM(sRmBody* b)
{
    sRmVec4 r;
    float x = b->pos.x;
    float y = b->pos.y;
    float z = b->pos.z;
    r.x = b->rot.m[0][0] * x + b->rot.m[1][0] * y + b->rot.m[2][0] * z;
    r.y = b->rot.m[0][1] * x + b->rot.m[1][1] * y + b->rot.m[2][1] * z;
    r.z = b->rot.m[0][2] * x + b->rot.m[1][2] * y + b->rot.m[2][2] * z;
    r.w = b->pos.w;
    return r;
}

extern "C" void func_0035D288(sRmBody* self)
{
    self->outDir = vu0ScaleRM(self->dir, self->scale);
    self->outPos = xformRM(self);
}
#endif

INCLUDE_ASM("object/railmodifier", func_0035D340);

INCLUDE_ASM("object/railmodifier", func_0035D4A0);

INCLUDE_ASM("object/railmodifier", func_0035D908);

//100%
INCLUDE_ASM("object/railmodifier", func_0035DA00);
#ifdef SKIP_ASM
class func_0035DA00_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

extern "C" void func_0035DA00(void* self, func_0035DA00_cObj* obj)
{
    obj->v01(self, 0x50);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035DA38);
#ifdef SKIP_ASM
class func_0035DA38_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

extern "C" void func_0035DA38(void* self, func_0035DA38_cObj* obj)
{
    obj->v02(self, 0x50);
}
#endif

INCLUDE_ASM("object/railmodifier", func_0035DA70);

INCLUDE_ASM("object/railmodifier", func_0035DDE8);

INCLUDE_ASM("object/railmodifier", func_0035DF70);

INCLUDE_ASM("object/railmodifier", func_0035E1E0);

INCLUDE_ASM("object/railmodifier", func_0035E248);

//100%
INCLUDE_ASM("object/railmodifier", func_0035E770);
#ifdef SKIP_ASM
struct sRmXform {
    sRmVec4 pos;    // 0x00
    sRmVec4 quat;   // 0x10
};

// PORT: PS2-only VU0 inline asm (dst = v * s).
static inline void vu0ScaleToRM(sRmVec4& dst, const sRmVec4& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst), "=&r"(t)
        : "m"(v), "f"(s));
}

// PORT: PS2-only VU0 inline asm (dst = v rotated by quaternion q).
static inline void vu0QuatRotToRM(sRmVec4& dst, const sRmVec4& q, const sRmVec4& v)
{
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
        : "=m"(dst)
        : "m"(q), "m"(v));
}

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void vu0AddToRM(sRmVec4& dst, const sRmVec4& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

extern "C" sRmXform func_0035E770(void* self)
{
    sRmXform r = *(sRmXform*)((char*)self + 0x30);
    sRmVec4 c;
    sRmVec4 b;
    sRmVec4 a;
    sRmVec4 e;
    sRmVec4 d;
    vu0QuatRotToRM(a, *(sRmVec4*)((char*)self + 0x40), *(sRmVec4*)((char*)self + 0x2B0));
    b = a;
    vu0ScaleToRM(a, b, -1.0f);
    c = a;
    vu0QuatRotToRM(d, r.quat, c);
    e = d;
    vu0AddToRM(r.pos, e);
    return r;
}
#endif

INCLUDE_ASM("object/railmodifier", func_0035E850);

//100%
INCLUDE_ASM("object/railmodifier", func_0035ED88__FPv);
#ifdef SKIP_ASM
void func_0035ED88(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035ED90);
#ifdef SKIP_ASM
extern "C" void func_0032C630(void* p, const sRmVec4* d);

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void vu0AddED90(sRmVec4& dst, const sRmVec4& b)
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

extern "C" void func_0035ED90(sRmBody* self, const sRmVec4* b)
{
    vu0AddED90(self->pos, *b);
    func_0032C630((char*)self + 0xE0, b);
}
#endif

INCLUDE_ASM("object/railmodifier", func_0035EDC8);

INCLUDE_ASM("object/railmodifier", func_0035F0B8);

INCLUDE_ASM("object/railmodifier", func_0035F1A8);

INCLUDE_ASM("object/railmodifier", func_0035F218);

//100%
INCLUDE_ASM("object/railmodifier", func_0035F378);
#ifdef SKIP_ASM
struct sRailModSelect;
extern "C" void func_0035F3A8(sRailModSelect* self);

extern "C" void func_0035F378(sRailModSelect* self, int i)
{
    int n = *(int*)((char*)self + 0x20);
    if (i >= n) {
        i = n - 1;
    }
    *(int*)((char*)self + 0x14) = i;
    func_0035F3A8(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_0035F3A8);
#ifdef SKIP_ASM
struct sRailOut14 {
    unsigned short value;   // 0x0
    char pad_0x2[0x12];
};

struct sRailModSelect {
    char pad_0x0[0x14];
    int index;              // 0x14
    char* owner;            // 0x18
    int count;              // 0x1c
    char pad_0x20[0x28];
    sRailOut14 out[1];      // 0x48
};

extern "C" void func_0035F3A8(sRailModSelect* self)
{
    int i;
    for (i = 0; i < self->count; i++) {
        char* list = *(char**)(self->owner + 0x94);
        char* item = *(char**)(list + (i << 2) + 4);
        char* table = *(char**)(item + 0x10);
        if (table != 0) {
            self->out[i].value = *(unsigned short*)(table + (self->index << 2) + 4);
        }
    }
}
#endif

INCLUDE_ASM("object/railmodifier", func_0035F410);

INCLUDE_ASM("object/railmodifier", func_0035F598);

INCLUDE_ASM("object/railmodifier", func_0035F688);

INCLUDE_ASM("object/railmodifier", func_0035F6E8);

//100%
INCLUDE_ASM("object/railmodifier", func_0035F788);
#ifdef SKIP_ASM
struct sRailSerVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_0035F788(void* self, void* stream)
{
    sRailSerVEntry* vt = *(sRailSerVEntry**)stream;
    vt[2].fn((char*)stream + vt[2].delta, self, 0x50);
    return self;
}
#endif

INCLUDE_ASM("object/railmodifier", func_0035F7D0);

//100%
INCLUDE_ASM("object/railmodifier", func_0035FB30);
#ifdef SKIP_ASM
extern "C" void func_0035FB30(void* self, int id, float v)
{
    if (id == 200 || id == 201) {
        if (v >= -1.0f && v <= 1.0f) {
            if (id == 201) {
                *(float*)((char*)self + 0x38) = v;
            } else {
                *(float*)((char*)self + 0x3C) = v;
            }
        }
    } else if (id == 202 || id == 203) {
        if (v >= 0.0f && v <= 60.0f) {
            if (id == 202) {
                *(float*)((char*)self + 0x8) = v;
            } else {
                *(float*)((char*)self + 0xC) = v;
            }
        }
    } else if (id == 204 || id == 205) {
        if (v >= -4.0f && v <= 4.0f) {
            if (id == 204) {
                *(float*)((char*)self + 0x30) = v;
            } else {
                *(float*)((char*)self + 0x34) = v;
            }
        }
    }
}
#endif

INCLUDE_ASM("object/railmodifier", func_0035FC20);

//100%
INCLUDE_ASM("object/railmodifier", func_0035FD98);
#ifdef SKIP_ASM
class func_0035FD98_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

extern "C" void func_0035FD98(void* self, func_0035FD98_cObj* obj)
{
    obj->v01(self, 0x50);
}
#endif

INCLUDE_ASM("object/railmodifier", func_0035FE10);

//100%
INCLUDE_ASM("object/railmodifier", func_00360720__FPv);
#ifdef SKIP_ASM
void func_00360720(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360728__FPv);
#ifdef SKIP_ASM
void func_00360728(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360730__FPv);
#ifdef SKIP_ASM
void func_00360730(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360738__FPv);
#ifdef SKIP_ASM
void func_00360738(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360740__FPv);
#ifdef SKIP_ASM
int func_00360740(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360748__FPv);
#ifdef SKIP_ASM
int func_00360748(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360750__FPv);
#ifdef SKIP_ASM
int func_00360750(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360758__FPv);
#ifdef SKIP_ASM
int func_00360758(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360760__FPv);
#ifdef SKIP_ASM
int func_00360760(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360768);
#ifdef SKIP_ASM
extern "C" unsigned int func_00360768()
{
    return 0xFFFFFFFFU;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360778__FPv);
#ifdef SKIP_ASM
void func_00360778(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360780__FPv);
#ifdef SKIP_ASM
void func_00360780(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360788__FPv);
#ifdef SKIP_ASM
void func_00360788(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360790__FPv);
#ifdef SKIP_ASM
void func_00360790(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360798__FPv);
#ifdef SKIP_ASM
int func_00360798(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003607A0);
#ifdef SKIP_ASM
extern "C" int func_003607A0(void* self, int a1)
{
    return *(short*)((char*)self + 0x10) == a1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360800);
#ifdef SKIP_ASM
struct sRailVEntryU800 {
    short delta;
    short index;
    unsigned int (*fn)(void*);
};

extern "C" int func_00360800(void* self, void* obj)
{
    sRailVEntryU800* vt = *(sRailVEntryU800**)((char*)obj + 0xC);
    return vt[8].fn((char*)obj + vt[8].delta) >= *(unsigned int*)((char*)self + 0x14);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360840);
#ifdef SKIP_ASM
struct sRailVEntryU840 {
    short delta;
    short index;
    unsigned int (*fn)(void*);
};

extern "C" int func_00360840(void* self, void* obj)
{
    sRailVEntryU840* vt = *(sRailVEntryU840**)((char*)obj + 0xC);
    return *(unsigned int*)((char*)self + 0x14) == vt[8].fn((char*)obj + vt[8].delta);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360880__FPv);
#ifdef SKIP_ASM
int func_00360880(void* self)
{
    return *(int*)((char*)self + 0x14);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003608E8);
#ifdef SKIP_ASM
class cRailVirt {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    // vptr lands at 0xC (g++ 2.95 places it after the class's own data);
    // slot N lives at vtable offset N*8 (delta at +0, function at +4)
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
    virtual int v15();
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
};

extern "C" int func_003608E8(cRailVirt* self)
{
    return self->v15();
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360910__FPv);
#ifdef SKIP_ASM
int func_00360910(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360918__FPv);
#ifdef SKIP_ASM
int func_00360918(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360920__FPv);
#ifdef SKIP_ASM
unsigned char func_00360920(void* self)
{
    return *(unsigned char*)((char*)*(void**)((char*)self + 0x18) + 0x78);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360930__FPv);
#ifdef SKIP_ASM
int func_00360930(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360938__FPv);
#ifdef SKIP_ASM
void func_00360938(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360940__FPv);
#ifdef SKIP_ASM
int func_00360940(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360948__FPv);
#ifdef SKIP_ASM
int func_00360948(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360950__FPv);
#ifdef SKIP_ASM
int func_00360950(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360958__FPv);
#ifdef SKIP_ASM
int func_00360958(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360960__FPv);
#ifdef SKIP_ASM
int func_00360960(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360970__FPv);
#ifdef SKIP_ASM
void* func_00360970(void* self)
{
    return (char*)*(void**)((char*)self + 0x18) + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360980__FPv);
#ifdef SKIP_ASM
void* func_00360980(void* self)
{
    return (char*)*(void**)((char*)self + 0x18) + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360990__FPv);
#ifdef SKIP_ASM
int func_00360990(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360998__FPv);
#ifdef SKIP_ASM
int func_00360998(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609A0__FPv);
#ifdef SKIP_ASM
int func_003609A0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609A8__FPv);
#ifdef SKIP_ASM
int func_003609A8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609B0__FPv);
#ifdef SKIP_ASM
float func_003609B0(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609C0__FPv);
#ifdef SKIP_ASM
int func_003609C0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609C8__FPv);
#ifdef SKIP_ASM
int func_003609C8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609D0__FPv);
#ifdef SKIP_ASM
void func_003609D0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609D8__FPv);
#ifdef SKIP_ASM
void func_003609D8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609E0__FPv);
#ifdef SKIP_ASM
int func_003609E0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609E8__FPv);
#ifdef SKIP_ASM
int func_003609E8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609F0__FPv);
#ifdef SKIP_ASM
void func_003609F0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003609F8__FPv);
#ifdef SKIP_ASM
void func_003609F8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360A00__FPv);
#ifdef SKIP_ASM
void func_00360A00(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360A08__FPv);
#ifdef SKIP_ASM
void func_00360A08(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360A10__FPv);
#ifdef SKIP_ASM
int func_00360A10(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360A18__FPv);
#ifdef SKIP_ASM
int func_00360A18(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360A20__FPv);
#ifdef SKIP_ASM
int func_00360A20(void* self)
{
    return 0;
}
#endif

extern void* D_00491B00[];
extern "C" void* func_0034FBF0(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00360A28__FPv);
#ifdef SKIP_ASM
void* func_00360A28(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_00491B00;
    return func_0034FBF0(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360A50__FPv);
#ifdef SKIP_ASM
void func_00360A50(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360A58__FPv);
#ifdef SKIP_ASM
void func_00360A58(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360A60__FPv);
#ifdef SKIP_ASM
void func_00360A60(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360A68__FPv);
#ifdef SKIP_ASM
void func_00360A68(void* self)
{
}
#endif

extern void* D_00491800[];
extern "C" void* func_0034FBF0(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00360A70__FPv);
#ifdef SKIP_ASM
void* func_00360A70(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_00491800;
    return func_0034FBF0(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360A98__FPv);
#ifdef SKIP_ASM
void func_00360A98(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AA0__FPv);
#ifdef SKIP_ASM
void func_00360AA0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AA8__FPv);
#ifdef SKIP_ASM
void func_00360AA8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AB0__FPv);
#ifdef SKIP_ASM
void func_00360AB0(void* self)
{
}
#endif

extern void* D_00491680[];
extern "C" void* func_0034FBF0(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00360AB8__FPv);
#ifdef SKIP_ASM
void* func_00360AB8(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_00491680;
    return func_0034FBF0(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AE0__FPv);
#ifdef SKIP_ASM
void func_00360AE0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AE8__FPv);
#ifdef SKIP_ASM
void func_00360AE8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AF0__FPv);
#ifdef SKIP_ASM
void func_00360AF0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360AF8__FPv);
#ifdef SKIP_ASM
void func_00360AF8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B00);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

extern "C" void func_00360B00(void* self, int flags)
{
    *(void***)self = D_004913F8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B30__FPv);
#ifdef SKIP_ASM
void func_00360B30(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B38__FPv);
#ifdef SKIP_ASM
void func_00360B38(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B40__FPv);
#ifdef SKIP_ASM
void func_00360B40(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B48__FPv);
#ifdef SKIP_ASM
void func_00360B48(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B50__FPv);
#ifdef SKIP_ASM
int func_00360B50(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B58__FPv);
#ifdef SKIP_ASM
void func_00360B58(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B60__FPv);
#ifdef SKIP_ASM
int func_00360B60(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B68__FPv);
#ifdef SKIP_ASM
int func_00360B68(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B70__FPv);
#ifdef SKIP_ASM
void func_00360B70(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B78__FPv);
#ifdef SKIP_ASM
void func_00360B78(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B80__FPv);
#ifdef SKIP_ASM
int func_00360B80(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B88__FPv);
#ifdef SKIP_ASM
void func_00360B88(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360B90__FPv);
#ifdef SKIP_ASM
float func_00360B90(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360BA0__FPv);
#ifdef SKIP_ASM
void func_00360BA0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360BA8__FPv);
#ifdef SKIP_ASM
int func_00360BA8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360BB0__FPv);
#ifdef SKIP_ASM
int func_00360BB0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360BB8__FPv);
#ifdef SKIP_ASM
int func_00360BB8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360BC0__FPv);
#ifdef SKIP_ASM
int func_00360BC0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360BC8__FPv);
#ifdef SKIP_ASM
int func_00360BC8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360BD0__FPv);
#ifdef SKIP_ASM
int func_00360BD0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360BD8__FPv);
#ifdef SKIP_ASM
void func_00360BD8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360BE0__FPv);
#ifdef SKIP_ASM
int func_00360BE0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360BE8__FPv);
#ifdef SKIP_ASM
void func_00360BE8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360BF0__FPv);
#ifdef SKIP_ASM
int func_00360BF0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360BF8__FPv);
#ifdef SKIP_ASM
int func_00360BF8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360C00__FPv);
#ifdef SKIP_ASM
void func_00360C00(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360C28);
#ifdef SKIP_ASM
extern void* D_004913B8[];
void operator_delete(int*);

extern "C" void func_00360C28(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004913B8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360C58__FPv);
#ifdef SKIP_ASM
void func_00360C58(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360C60__FPv);
#ifdef SKIP_ASM
void func_00360C60(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360C68__FPv);
#ifdef SKIP_ASM
int func_00360C68(void* self)
{
    return *(int*)((char*)self + 0x0);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360C70__FPv);
#ifdef SKIP_ASM
int func_00360C70(void* self)
{
    return 0;
}
#endif

extern "C" void* func_00370C08(void* self);

//100%
INCLUDE_ASM("object/railmodifier", func_00360C78__FPv);
#ifdef SKIP_ASM
void* func_00360C78(void* self)
{
    return func_00370C08(self);
}
#endif

INCLUDE_ASM("object/railmodifier", func_00360C98);

//100%
INCLUDE_ASM("object/railmodifier", func_00360CE8__FPv);
#ifdef SKIP_ASM
int func_00360CE8(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360CF0__FPv);
#ifdef SKIP_ASM
void func_00360CF0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360CF8__FPv);
#ifdef SKIP_ASM
void func_00360CF8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360D00__FPv);
#ifdef SKIP_ASM
void func_00360D00(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360D08__FPv);
#ifdef SKIP_ASM
void func_00360D08(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360D10__FPv);
#ifdef SKIP_ASM
void func_00360D10(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360D20__FPv);
#ifdef SKIP_ASM
int func_00360D20(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360D28__FPv);
#ifdef SKIP_ASM
int func_00360D28(void* self)
{
    return 0x2;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360D38__FPv);
#ifdef SKIP_ASM
int func_00360D38(void* self)
{
    return 0x3;
}
#endif

INCLUDE_ASM("object/railmodifier", func_00360D40);

//100%
INCLUDE_ASM("object/railmodifier", func_00360D90__FPv);
#ifdef SKIP_ASM
void func_00360D90(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360DA0);
#ifdef SKIP_ASM
extern "C" void func_00360DA0(cRailVirt* self)
{
    self->v50();
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360DC8__FPv);
#ifdef SKIP_ASM
void func_00360DC8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360DD0__FPv);
#ifdef SKIP_ASM
int func_00360DD0(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00360DD8);
#ifdef SKIP_ASM
extern "C" void func_00360DD8(void* self)
{
    unsigned short* p = (unsigned short*)((char*)self + 0x12);
    *p &= 0xFFFE;
}
#endif

extern void* D_00490E80[];
extern "C" void* func_003553C0(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00361038__FPv);
#ifdef SKIP_ASM
void* func_00361038(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_00490E80;
    return func_003553C0(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361060__FPv);
#ifdef SKIP_ASM
int func_00361060(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361068);
#ifdef SKIP_ASM
extern "C" float func_00361068(void* self)
{
    float input = *(float*)((char*)self + 0x0);
    return input * 30.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361080__FPv);
#ifdef SKIP_ASM
float func_00361080(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361090__FPv);
#ifdef SKIP_ASM
int func_00361090(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361098);
#ifdef SKIP_ASM
struct sRailVEntry1098 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" int func_00361098(void* self)
{
    if (*(unsigned short*)((char*)self + 0x26) & 1) {
        void* sub = (char*)self + 0x14;
        sRailVEntry1098* vt = *(sRailVEntry1098**)((char*)self + 0x20);
        vt[51].fn((char*)sub + vt[51].delta);
    }
    return *(int*)((char*)self + 0x44);
}
#endif

INCLUDE_ASM("object/railmodifier", func_003610E0);

//100%
INCLUDE_ASM("object/railmodifier", func_00361150__FPv);
#ifdef SKIP_ASM
void* func_00361150(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361158);
#ifdef SKIP_ASM
extern "C" float func_00361158(void* self)
{
    return *(int*)((char*)self + 0x8) == 0 ? *(float*)((char*)self + 0x10) : 0.0f;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361170);
#ifdef SKIP_ASM
extern "C" int func_00361170(void* self)
{
    int r = -1;
    if (*(float*)((char*)self + 0x10) >= 0.0f) {
        r = 1;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361198__FPv);
#ifdef SKIP_ASM
int func_00361198(void* self)
{
    return *(int*)((char*)self + 0x8);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003611D8);
#ifdef SKIP_ASM
extern void* D_00490AF0[];
void operator_delete(int*);

extern "C" void func_003611D8(void* self, int flags)
{
    *(void***)((char*)self + 0x84) = D_00490AF0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361290);
#ifdef SKIP_ASM
extern void* D_00490AF0[];
void operator_delete(int*);

extern "C" void func_00361290(void* self, int flags)
{
    *(void***)((char*)self + 0x84) = D_00490AF0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003612C0);
#ifdef SKIP_ASM
extern char D_004FF1A0[];

// Copy the 4x4 matrix D_004FF1A0 into self+0x90.
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
extern "C" void func_003612C0(void* self)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"((char*)self + 0x90), "r"(D_004FF1A0)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361340);
#ifdef SKIP_ASM
extern void* D_00490AF0[];
void operator_delete(int*);

extern "C" void func_00361340(void* self, int flags)
{
    *(void***)((char*)self + 0x84) = D_00490AF0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("object/railmodifier", func_00361400);

//100%
INCLUDE_ASM("object/railmodifier", func_00361458__FPv);
#ifdef SKIP_ASM
void* func_00361458(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361460);
#ifdef SKIP_ASM
extern "C" int func_00361460(void* self)
{
    int r = -1;
    if (*(float*)((char*)self + 0x4) >= 0.0f) {
        r = 1;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361488);
#ifdef SKIP_ASM
extern void* D_004913B8[];
void operator_delete(int*);

extern "C" void func_00361488(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004913B8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003614B8__FPv);
#ifdef SKIP_ASM
int func_003614B8(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003614C0__FPv);
#ifdef SKIP_ASM
void func_003614C0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003614C8__FPv);
#ifdef SKIP_ASM
void func_003614C8(void* self)
{
}
#endif

extern void* D_004906F0[];
extern "C" void* func_003553C0(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_003614D0__FPv);
#ifdef SKIP_ASM
void* func_003614D0(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_004906F0;
    return func_003553C0(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003614F8__FPv);
#ifdef SKIP_ASM
void func_003614F8(void* self)
{
}
#endif

extern void* D_00490570[];
extern "C" void* func_0034FBF0(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00361500__FPv);
#ifdef SKIP_ASM
void* func_00361500(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_00490570;
    return func_0034FBF0(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361528__FPv);
#ifdef SKIP_ASM
void* func_00361528(void* self)
{
    return (char*)self + 0x1C;
}
#endif

extern void* D_004903F0[];
extern "C" void* func_0034FBF0(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00361530__FPv);
#ifdef SKIP_ASM
void* func_00361530(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_004903F0;
    return func_0034FBF0(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361558__FPv);
#ifdef SKIP_ASM
void* func_00361558(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361560__FPv);
#ifdef SKIP_ASM
int func_00361560(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361568__FPv);
#ifdef SKIP_ASM
int func_00361568(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361570__FPv);
#ifdef SKIP_ASM
void* func_00361570(void* self)
{
    return (char*)self + 0x90;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361578__FPv);
#ifdef SKIP_ASM
void func_00361578(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361580__FPv);
#ifdef SKIP_ASM
void func_00361580(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361588__FPv);
#ifdef SKIP_ASM
void func_00361588(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361590__FPv);
#ifdef SKIP_ASM
void func_00361590(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361598__FPv);
#ifdef SKIP_ASM
int func_00361598(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003615A0__FPv);
#ifdef SKIP_ASM
int func_003615A0(void* self)
{
    return 0x1;
}
#endif

// 0x50-byte elements reached through a pointer at self+0x70; indexing the
// real element type is what reproduces the target's addu operand order
struct sRailEntry50 {
    char pad_0x00[0x44];
    int field_0x44;
    char pad_0x48[8];
};

//100%
INCLUDE_ASM("object/railmodifier", func_003615A8);
#ifdef SKIP_ASM
extern "C" int func_003615A8(void* self, int a1)
{
    sRailEntry50* p = *(sRailEntry50**)((char*)self + 0x70);
    return p[a1].field_0x44;
}
#endif

struct sRailEntry60 {
    char pad_0x00[0x54];
    int field_0x54;
    char pad_0x58[8];
};

//100%
INCLUDE_ASM("object/railmodifier", func_003615C0);
#ifdef SKIP_ASM
extern "C" int func_003615C0(void* self, int a1)
{
    sRailEntry60* p = *(sRailEntry60**)((char*)self + 0x70);
    return p[a1].field_0x54;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361768__FPv);
#ifdef SKIP_ASM
void* func_00361768(void* self)
{
    return (char*)self + 0x2C;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361770__FPv);
#ifdef SKIP_ASM
int func_00361770(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361778);
#ifdef SKIP_ASM
extern "C" int func_00352B88(void* p);

extern "C" int func_00361778(void* self)
{
    void* p = *(void**)((char*)self + 0x1C);
    if (p != 0) {
        return func_00352B88(p);
    }
    return *(int*)((char*)self + 0x78);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003617B0__FPv);
#ifdef SKIP_ASM
int func_003617B0(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003617B8);
#ifdef SKIP_ASM
struct sRailVEntry17B8 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" int func_003617B8(void* self)
{
    if (*(unsigned short*)((char*)self + 0x12) & 1) {
        sRailVEntry17B8* vt = *(sRailVEntry17B8**)((char*)self + 0xC);
        vt[51].fn((char*)self + vt[51].delta);
    }
    return *(int*)((char*)self + 0x74);
}
#endif

INCLUDE_ASM("object/railmodifier", func_00361800);

INCLUDE_ASM("object/railmodifier", func_00361858);

//100%
INCLUDE_ASM("object/railmodifier", func_003618C8);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

extern "C" void func_003618C8(void* self, int flags)
{
    *(void***)self = D_004913F8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003618F8__FPv);
#ifdef SKIP_ASM
void func_003618F8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361900__FPv);
#ifdef SKIP_ASM
int func_00361900(void* self)
{
    return *(int*)((char*)self + 0x44);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361908__FPv);
#ifdef SKIP_ASM
int func_00361908(void* self)
{
    return 0x3;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361910__FPv);
#ifdef SKIP_ASM
void* func_00361910(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361918);
#ifdef SKIP_ASM
extern "C" void func_00361918(void* self, void* a1)
{
    *(cQuad128*)((char*)self + 0x10) = *(cQuad128*)a1;
    *(cQuad128*)((char*)self + 0x20) = *(cQuad128*)((char*)a1 + 0x10);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361930__FPv);
#ifdef SKIP_ASM
float func_00361930(void* self)
{
    return *(float*)((char*)self + 0x40);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361938__FPvf);
#ifdef SKIP_ASM
void func_00361938(void* self, float val)
{
    *(float*)((char*)self + 0x40) = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361940);
#ifdef SKIP_ASM
struct sRailVEntry1940 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void* func_00361940(void* self)
{
    if (*(int*)((char*)self + 0x44) != 0) {
        sRailVEntry1940* vt = *(sRailVEntry1940**)self;
        vt[3].fn((char*)self + vt[3].delta);
    }
    return (char*)self + 0x50;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361988);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

extern "C" void func_00361988(void* self, int flags)
{
    *(void***)self = D_004913F8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003619B8__FPv);
#ifdef SKIP_ASM
void func_003619B8(void* self)
{
    *(int*)((char*)self + 0x44) = 1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003619C8__FPv);
#ifdef SKIP_ASM
int func_003619C8(void* self)
{
    return *(int*)((char*)self + 0x90);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003619D0__FPv);
#ifdef SKIP_ASM
int func_003619D0(void* self)
{
    return 0x4;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003619E0);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

extern "C" void func_003619E0(void* self, int flags)
{
    *(void***)self = D_004913F8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361A10__FPv);
#ifdef SKIP_ASM
int func_00361A10(void* self)
{
    return (*(int*)((char*)self + 0x50) != 0);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361A20__FPv);
#ifdef SKIP_ASM
int func_00361A20(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361A28__FPv);
#ifdef SKIP_ASM
void* func_00361A28(void* self)
{
    return (char*)self + 0x20;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361A30);
#ifdef SKIP_ASM
extern "C" void func_00361A30(void* self, void* a1)
{
    *(cQuad128*)((char*)self + 0x20) = *(cQuad128*)a1;
    *(cQuad128*)((char*)self + 0x30) = *(cQuad128*)((char*)a1 + 0x10);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361A48__FPv);
#ifdef SKIP_ASM
float func_00361A48(void* self)
{
    return *(float*)((char*)self + 0x40);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361A50__FPv);
#ifdef SKIP_ASM
void func_00361A50(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361A58__FPv);
#ifdef SKIP_ASM
int func_00361A58(void* self)
{
    return 0x6;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361A60);
#ifdef SKIP_ASM
struct sRailVEntry1A60 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void* func_00361A60(void* self)
{
    if (*(int*)((char*)self + 0x50) != 0) {
        sRailVEntry1A60* vt = *(sRailVEntry1A60**)self;
        vt[3].fn((char*)self + vt[3].delta);
    }
    return (char*)self + 0x60;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361AA8);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

extern "C" void func_00361AA8(void* self, int flags)
{
    *(void***)self = D_004913F8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361AD8__FPv);
#ifdef SKIP_ASM
void* func_00361AD8(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361AE0);
#ifdef SKIP_ASM
extern "C" void func_00361AE0(void* self, void* a1)
{
    *(cQuad128*)((char*)self + 0x10) = *(cQuad128*)a1;
    *(cQuad128*)((char*)self + 0x20) = *(cQuad128*)((char*)a1 + 0x10);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361AF8__FPv);
#ifdef SKIP_ASM
float func_00361AF8(void* self)
{
    return *(float*)((char*)self + 0x30);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B00__FPvf);
#ifdef SKIP_ASM
void func_00361B00(void* self, float val)
{
    *(float*)((char*)self + 0x30) = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B08__FPv);
#ifdef SKIP_ASM
int func_00361B08(void* self)
{
    return 0x7;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B10__FPv);
#ifdef SKIP_ASM
void* func_00361B10(void* self)
{
    return (char*)self + 0x40;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B18);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

extern "C" void func_00361B18(void* self, int flags)
{
    *(void***)self = D_004913F8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B48__FPv);
#ifdef SKIP_ASM
int func_00361B48(void* self)
{
    return *(int*)((char*)self + 0x50);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B50__FPv);
#ifdef SKIP_ASM
void func_00361B50(void* self)
{
    *(int*)((char*)self + 0x50) = 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B58__FPv);
#ifdef SKIP_ASM
void* func_00361B58(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B60);
#ifdef SKIP_ASM
extern "C" void func_00361B60(void* self, void* a1)
{
    *(cQuad128*)((char*)self + 0x10) = *(cQuad128*)a1;
    *(cQuad128*)((char*)self + 0x20) = *(cQuad128*)((char*)a1 + 0x10);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B78__FPv);
#ifdef SKIP_ASM
float func_00361B78(void* self)
{
    return *(float*)((char*)self + 0x4C);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B80__FPvf);
#ifdef SKIP_ASM
void func_00361B80(void* self, float val)
{
    *(float*)((char*)self + 0x4C) = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B88__FPv);
#ifdef SKIP_ASM
int func_00361B88(void* self)
{
    return *(int*)((char*)self + 0x58);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361B90);
#ifdef SKIP_ASM
struct sRailVEntry1B90 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void* func_00361B90(void* self)
{
    if (*(int*)((char*)self + 0x58) != 0) {
        sRailVEntry1B90* vt = *(sRailVEntry1B90**)self;
        vt[3].fn((char*)self + vt[3].delta);
    }
    return (char*)self + 0x60;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361BD8__FPv);
#ifdef SKIP_ASM
int func_00361BD8(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361BE0__FPv);
#ifdef SKIP_ASM
void* func_00361BE0(void* self)
{
    return (char*)*(void**)((char*)self + 0x3c) + 0x30;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361BF0);
#ifdef SKIP_ASM
extern "C" void func_00361BF0(void* self, void* a1)
{
    void* p = *(void**)((char*)self + 0x3c);
    *(cQuad128*)((char*)p + 0x30) = *(cQuad128*)a1;
    *(cQuad128*)((char*)p + 0x40) = *(cQuad128*)((char*)a1 + 0x10);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361C10__FPv);
#ifdef SKIP_ASM
float func_00361C10(void* self)
{
    return *(float*)((char*)self + 0x18);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361C18__FPvf);
#ifdef SKIP_ASM
void func_00361C18(void* self, float val)
{
    *(float*)((char*)self + 0x18) = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361C20);
#ifdef SKIP_ASM
struct sRailVEntry1C20 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void* func_00361C20(void* self)
{
    if (*(int*)((char*)self + 0x30) != 0) {
        sRailVEntry1C20* vt = *(sRailVEntry1C20**)self;
        vt[3].fn((char*)self + vt[3].delta);
    }
    return **(char***)((char*)self + 0x44) + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361C70__FPv);
#ifdef SKIP_ASM
int func_00361C70(void* self)
{
    return 0x2;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361C78);
#ifdef SKIP_ASM
extern void* D_004913F8[];
void operator_delete(int*);

extern "C" void func_00361C78(void* self, int flags)
{
    *(void***)self = D_004913F8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361CA8__FPv);
#ifdef SKIP_ASM
int func_00361CA8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361CB0);
#ifdef SKIP_ASM
extern "C" int func_00361CB0(void* self)
{
    return *(float*)((char*)self + 0x20) >= 17.0f;
}
#endif

extern "C" void* func_0035DDE8(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00361CD8__FPv);
#ifdef SKIP_ASM
void* func_00361CD8(void* self)
{
    *(int*)((char*)self + 0x20) = 0;
    return func_0035DDE8(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361CF8__FPv);
#ifdef SKIP_ASM
int func_00361CF8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361D00);
#ifdef SKIP_ASM
extern "C" void func_00361D00(void* self, void* a1)
{
    *(cQuad128*)((char*)self + 0x220) = *(cQuad128*)a1;
    *(cQuad128*)((char*)self + 0x230) = *(cQuad128*)((char*)a1 + 0x10);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361D18__FPv);
#ifdef SKIP_ASM
void* func_00361D18(void* self)
{
    return (char*)self + 0x220;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361D20__FPv);
#ifdef SKIP_ASM
float func_00361D20(void* self)
{
    return *(float*)((char*)self + 0x24);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361D28__FPvf);
#ifdef SKIP_ASM
void func_00361D28(void* self, float val)
{
    *(float*)((char*)self + 0x24) = val;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361D30__FPv);
#ifdef SKIP_ASM
void* func_00361D30(void* self)
{
    return (char*)self + 0x240;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361D38__FPv);
#ifdef SKIP_ASM
void* func_00361D38(void* self)
{
    return (char*)self + 0x240;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361D40__FPv);
#ifdef SKIP_ASM
int func_00361D40(void* self)
{
    return 0x5;
}
#endif

extern void* D_0048EE60[];
extern "C" void* func_003553C0(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00361D50__FPv);
#ifdef SKIP_ASM
void* func_00361D50(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_0048EE60;
    return func_003553C0(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361D78);
#ifdef SKIP_ASM
extern "C" unsigned int func_00361D78()
{
    return 0xFFFFFFFFU;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361D88__FPv);
#ifdef SKIP_ASM
void* func_00361D88(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361D90);
#ifdef SKIP_ASM
extern "C" int func_00361D90(void* self)
{
    int r;
    if (*(signed char*)((char*)self + 0x14) != 0) {
        r = -1;
        if (*(float*)((char*)self + 0x0) >= 0.0f) {
            r = 1;
        }
        return r;
    }
    r = -1;
    if (*(float*)((char*)self + 0x28) >= 0.0f) {
        r = 1;
    }
    return r;
}
#endif

extern void* D_0048EA70[];
extern "C" void* func_00341CF0(void*);

//100%
INCLUDE_ASM("object/railmodifier", func_00361DE0__FPv);
#ifdef SKIP_ASM
void* func_00361DE0(void* self)
{
    *(int*)((char*)self + 0x3c) = (int)(void*)D_0048EA70;
    return func_00341CF0(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361E08__FPv);
#ifdef SKIP_ASM
void* func_00361E08(void* self)
{
    return self;
}
#endif

extern "C" void* func_0035FE10(int, int);

//99.38%
INCLUDE_ASM("object/railmodifier", func_00361E10__FPv);
#ifdef SKIP_ASM
void* func_00361E10(void* self)
{
    return func_0035FE10(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361E30);
#ifdef SKIP_ASM
extern "C" int func_00362340(void);
extern "C" int func_003623A8(void);
extern "C" void func_00361F98(void* self, int cause, int (*handler)(void), int arg);

extern "C" void func_00361E30(void* self)
{
    func_00361F98(self, 5, func_00362340, 0);
    func_00361F98(self, 7, func_003623A8, 0);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361E80);
#ifdef SKIP_ASM
extern "C" void* func_00362008(void* self, int id);

extern "C" void* func_00361E80(void* self)
{
    func_00362008(self, 5);
    return func_00362008(self, 7);
}
#endif

INCLUDE_ASM("object/railmodifier", func_00361EB8);

extern "C" void* func_00361E80(void* self);

//99.29%
INCLUDE_ASM("object/railmodifier", func_00361F40__FPv);
#ifdef SKIP_ASM
void* func_00361F40(void* self)
{
    return func_00361E80(self);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361F60);
#ifdef SKIP_ASM
struct sRailPair {
    int a;
    int b;
};

struct sRailPairs {
    char pad_0x00[0x80];
    sRailPair pairs[3];
};

extern "C" void func_00361F60(sRailPairs* self)
{
    int i;
    for (i = 0; i < 3; i++) {
        self->pairs[i].a = 0;
        self->pairs[i].b = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00361F90__FPv);
#ifdef SKIP_ASM
void func_00361F90(void* self)
{
}
#endif

INCLUDE_ASM("object/railmodifier", func_00361F98);

INCLUDE_ASM("object/railmodifier", func_00362008);

//100%
INCLUDE_ASM("object/railmodifier", func_00362120);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm (dump all 32 FPU registers as 64-bit words into buf).
extern "C" void func_00362120(void* buf)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "por       $2, $0, $0\n"
        "mfc1      $2, $f0\n"
        "sd        $2, 0x0(%0)\n"
        "mfc1      $2, $f1\n"
        "sd        $2, 0x8(%0)\n"
        "mfc1      $2, $f2\n"
        "sd        $2, 0x10(%0)\n"
        "mfc1      $2, $f3\n"
        "sd        $2, 0x18(%0)\n"
        "mfc1      $2, $f4\n"
        "sd        $2, 0x20(%0)\n"
        "mfc1      $2, $f5\n"
        "sd        $2, 0x28(%0)\n"
        "mfc1      $2, $f6\n"
        "sd        $2, 0x30(%0)\n"
        "mfc1      $2, $f7\n"
        "sd        $2, 0x38(%0)\n"
        "mfc1      $2, $f8\n"
        "sd        $2, 0x40(%0)\n"
        "mfc1      $2, $f9\n"
        "sd        $2, 0x48(%0)\n"
        "mfc1      $2, $f10\n"
        "sd        $2, 0x50(%0)\n"
        "mfc1      $2, $f11\n"
        "sd        $2, 0x58(%0)\n"
        "mfc1      $2, $f12\n"
        "sd        $2, 0x60(%0)\n"
        "mfc1      $2, $f13\n"
        "sd        $2, 0x68(%0)\n"
        "mfc1      $2, $f14\n"
        "sd        $2, 0x70(%0)\n"
        "mfc1      $2, $f15\n"
        "sd        $2, 0x78(%0)\n"
        "mfc1      $2, $f16\n"
        "sd        $2, 0x80(%0)\n"
        "mfc1      $2, $f17\n"
        "sd        $2, 0x88(%0)\n"
        "mfc1      $2, $f18\n"
        "sd        $2, 0x90(%0)\n"
        "mfc1      $2, $f19\n"
        "sd        $2, 0x98(%0)\n"
        "mfc1      $2, $f20\n"
        "sd        $2, 0xA0(%0)\n"
        "mfc1      $2, $f21\n"
        "sd        $2, 0xA8(%0)\n"
        "mfc1      $2, $f22\n"
        "sd        $2, 0xB0(%0)\n"
        "mfc1      $2, $f23\n"
        "sd        $2, 0xB8(%0)\n"
        "mfc1      $2, $f24\n"
        "sd        $2, 0xC0(%0)\n"
        "mfc1      $2, $f25\n"
        "sd        $2, 0xC8(%0)\n"
        "mfc1      $2, $f26\n"
        "sd        $2, 0xD0(%0)\n"
        "mfc1      $2, $f27\n"
        "sd        $2, 0xD8(%0)\n"
        "mfc1      $2, $f28\n"
        "sd        $2, 0xE0(%0)\n"
        "mfc1      $2, $f29\n"
        "sd        $2, 0xE8(%0)\n"
        "mfc1      $2, $f30\n"
        "sd        $2, 0xF0(%0)\n"
        "mfc1      $2, $f31\n"
        "sd        $2, 0xF8(%0)\n"
        ".set pop\n"
        :
        : "r"(buf)
        : "$2", "memory");
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00362230);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm (reload all 32 FPU registers from buf; deliberately
// no FPU clobbers so the compiler doesn't save/restore $f20-$f31 around it).
extern "C" void func_00362230(void* buf)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "por       $2, $0, $0\n"
        "ld        $2, 0x0(%0)\n"
        "mtc1      $2, $f0\n"
        "ld        $2, 0x8(%0)\n"
        "mtc1      $2, $f1\n"
        "ld        $2, 0x10(%0)\n"
        "mtc1      $2, $f2\n"
        "ld        $2, 0x18(%0)\n"
        "mtc1      $2, $f3\n"
        "ld        $2, 0x20(%0)\n"
        "mtc1      $2, $f4\n"
        "ld        $2, 0x28(%0)\n"
        "mtc1      $2, $f5\n"
        "ld        $2, 0x30(%0)\n"
        "mtc1      $2, $f6\n"
        "ld        $2, 0x38(%0)\n"
        "mtc1      $2, $f7\n"
        "ld        $2, 0x40(%0)\n"
        "mtc1      $2, $f8\n"
        "ld        $2, 0x48(%0)\n"
        "mtc1      $2, $f9\n"
        "ld        $2, 0x50(%0)\n"
        "mtc1      $2, $f10\n"
        "ld        $2, 0x58(%0)\n"
        "mtc1      $2, $f11\n"
        "ld        $2, 0x60(%0)\n"
        "mtc1      $2, $f12\n"
        "ld        $2, 0x68(%0)\n"
        "mtc1      $2, $f13\n"
        "ld        $2, 0x70(%0)\n"
        "mtc1      $2, $f14\n"
        "ld        $2, 0x78(%0)\n"
        "mtc1      $2, $f15\n"
        "ld        $2, 0x80(%0)\n"
        "mtc1      $2, $f16\n"
        "ld        $2, 0x88(%0)\n"
        "mtc1      $2, $f17\n"
        "ld        $2, 0x90(%0)\n"
        "mtc1      $2, $f18\n"
        "ld        $2, 0x98(%0)\n"
        "mtc1      $2, $f19\n"
        "ld        $2, 0xA0(%0)\n"
        "mtc1      $2, $f20\n"
        "ld        $2, 0xA8(%0)\n"
        "mtc1      $2, $f21\n"
        "ld        $2, 0xB0(%0)\n"
        "mtc1      $2, $f22\n"
        "ld        $2, 0xB8(%0)\n"
        "mtc1      $2, $f23\n"
        "ld        $2, 0xC0(%0)\n"
        "mtc1      $2, $f24\n"
        "ld        $2, 0xC8(%0)\n"
        "mtc1      $2, $f25\n"
        "ld        $2, 0xD0(%0)\n"
        "mtc1      $2, $f26\n"
        "ld        $2, 0xD8(%0)\n"
        "mtc1      $2, $f27\n"
        "ld        $2, 0xE0(%0)\n"
        "mtc1      $2, $f28\n"
        "ld        $2, 0xE8(%0)\n"
        "mtc1      $2, $f29\n"
        "ld        $2, 0xF0(%0)\n"
        "mtc1      $2, $f30\n"
        "ld        $2, 0xF8(%0)\n"
        "mtc1      $2, $f31\n"
        ".set pop\n"
        :
        : "r"(buf)
        : "$2", "memory");
}
#endif

INCLUDE_ASM("object/railmodifier", func_00362340);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/railmodifier", func_003623A8);
#ifdef SKIP_ASM
extern "C" void func_00362120(void*);
extern "C" void func_00362230(void*);

extern "C" int func_003623A8(void)
{
    char buf[0x100];
    func_00362120(buf);
    func_00362230(buf);
    // PORT: PS2-only: re-enable interrupts (EI).
    __asm__ __volatile__("sync.l\n\tei");
    return 1;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003623D8);
#ifdef SKIP_ASM
struct sGifPacket {
    int qwc;        // 0x0
    ulong* cur;     // 0x4
};

// PORT: 64-bit GS register writes (ulong).
extern "C" void func_003623D8(sGifPacket* pkt, int x, int w, int y, int h)
{
    ulong scissor = (ulong)x | ((ulong)(x + (w - 1)) << 16) | ((ulong)y << 32) | ((ulong)(y + (h - 1)) << 48);
    pkt->cur[0] = scissor;
    pkt->cur[1] = 0x40;
    pkt->cur[2] = scissor;
    pkt->cur[3] = 0x41;
    pkt->cur += 4;
    pkt->qwc += 2;
}
#endif

INCLUDE_ASM("object/railmodifier", func_00362478);

//100%
INCLUDE_ASM("object/railmodifier", func_003625C0);
#ifdef SKIP_ASM
// PORT: 64-bit GS register writes (ulong).
extern "C" void func_003625C0(sGifPacket* pkt, int mode, int ctx)
{
    ulong reg;
    switch (ctx) {
    case 0:
        reg = 8;
        break;
    case 1:
        reg = 9;
        break;
    default:
        reg = 8;
        break;
    }
    switch (mode) {
    case 3:
        pkt->cur[0] = 5;
        break;
    case 1:
        pkt->cur[0] = 4;
        break;
    case 2:
        pkt->cur[0] = 1;
        break;
    default:
        pkt->cur[0] = 0;
        break;
    }
    pkt->cur[1] = reg;
    pkt->cur += 2;
    pkt->qwc++;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00362660);
#ifdef SKIP_ASM
// PORT: 64-bit GS register writes (ulong).
extern "C" void func_00362660(sGifPacket* pkt, unsigned int zbp, unsigned int psm, int zmsk, int ctx)
{
    ulong reg;
    switch (ctx) {
    case 0:
        reg = 0x4E;
        break;
    case 1:
        reg = 0x4F;
        break;
    default:
        reg = 0x4E;
        break;
    }
    pkt->cur[0] = (ulong)zbp | ((ulong)(psm - 0x30) << 24) | ((zmsk == 1) ? ((ulong)1 << 32) : 0);
    pkt->cur[1] = reg;
    pkt->cur += 2;
    pkt->qwc++;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_003626D8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sRmGsPacket {
    int count;      // 0x0
    ulong* ptr;     // 0x4
};

// GS TEST register (TEST_1 = 0x47, TEST_2 = 0x48), as in libgraph's SCE_GS_SET_TEST.
extern "C" void func_003626D8(sRmGsPacket* pkt, int ztest, int atest, unsigned int aref, int ctx)
{
    ulong reg;
    int atst;
    int ztst;

    switch (ctx) {
    case 0:
        reg = 0x47;
        break;
    case 1:
        reg = 0x48;
        break;
    default:
        reg = 0x47;
        break;
    }

    switch (atest) {
    case 0:
    case 1:
        atst = 1;
        break;
    case 2:
        atst = 2;
        break;
    case 3:
        atst = 6;
        break;
    default:
        atst = 1;
        break;
    }

    ztst = 1;
    switch (ztest) {
    case 1:
        ztst = 2;
        break;
    case 0:
        ztst = 3;
        break;
    case 2:
        break;
    }

    pkt->ptr[0] = (ulong)1 | ((ulong)atst << 1) | ((ulong)aref << 4) | ((ulong)1 << 12)
                | ((ulong)0 << 14) | ((ulong)0 << 15) | ((ulong)1 << 16) | ((ulong)ztst << 17);
    pkt->ptr[1] = reg;
    pkt->ptr += 2;
    pkt->count++;
}
#endif

INCLUDE_ASM("object/railmodifier", func_003627A8);

//100%
INCLUDE_ASM("object/railmodifier", func_00362978);
#ifdef SKIP_ASM
// Append a DMA packet to a chain: the first one becomes the head, later ones are
// linked by patching the previous tail's tag (via the uncached-accelerated
// mirror, | 0x30000000) into a NEXT tag pointing at the new packet.
// PORT: 64-bit DMA tag writes (ulong) and addresses held in int.
extern "C" void func_00362978(void* self, int packet, int tail)
{
    if (*(int*)((char*)self + 0xc) != 0) {
        ulong* tag = (ulong*)(*(int*)((char*)self + 0x10) | 0x30000000);
        tag[0] = ((ulong)packet << 32) | 0x20000000;
        tag[1] = 0;
        *(int*)((char*)self + 0x10) = tail;
    } else {
        *(int*)((char*)self + 0xc) = packet;
        *(int*)((char*)self + 0x10) = tail;
    }
}
#endif

INCLUDE_ASM("object/railmodifier", func_003629B8);

INCLUDE_ASM("object/railmodifier", func_00362CC8);

INCLUDE_ASM("object/railmodifier", func_00362DE8);

INCLUDE_ASM("object/railmodifier", func_00363490);

INCLUDE_ASM("object/railmodifier", func_00363C20);

INCLUDE_ASM("object/railmodifier", func_00364050);

INCLUDE_ASM("object/railmodifier", func_00364240);

INCLUDE_ASM("object/railmodifier", func_00364360);

INCLUDE_ASM("object/railmodifier", func_003645B8);

//100%
INCLUDE_ASM("object/railmodifier", func_00364B88);
#ifdef SKIP_ASM
struct sRailKey4 {
    unsigned short a, b, c, d;
};

struct sRailKeyTable {
    char pad_0x00[0x69CC4];
    int found;                  // 0x69CC4
    char pad_0x69CC8[0x6AED0 - 0x69CC8];
    int count;                  // 0x6AED0
    sRailKey4 keys[1];          // 0x6AED4
};

extern "C" void func_00364B88(sRailKeyTable* self, sRailKey4* k)
{
    int i;
    self->found = -1;
    for (i = 0; i < self->count; i++) {
        if (self->keys[i].a == k->a && self->keys[i].b == k->b &&
            self->keys[i].c == k->c && self->keys[i].d == k->d) {
            self->found = i;
            break;
        }
    }
    if (self->found < 0) {
        self->keys[self->count].a = k->a;
        self->keys[self->count].b = k->b;
        self->keys[self->count].c = k->c;
        self->keys[self->count].d = k->d;
        self->found = self->count;
        self->count++;
    }
}
#endif

INCLUDE_ASM("object/railmodifier", func_00364CD0);

INCLUDE_ASM("object/railmodifier", func_00365E40);

//100%
INCLUDE_ASM("object/railmodifier", func_00365F68);
#ifdef SKIP_ASM
extern "C" void func_00424698(void* begin, void* end);
extern "C" int func_0038F708(void* base, int size, int mode);

extern "C" void func_00365F68(void* self)
{
    func_00424698(self, (char*)self + 0x1FEF);
    *(int*)((char*)self + 0x1FF0) = func_0038F708(self, 0x1FF0, 2);
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00365FA8);
#ifdef SKIP_ASM
extern "C" void func_004247D8(void* begin, void* end);
extern "C" void func_0038F738(void* base, int size, int mode);

extern "C" void func_00365FA8(void* self)
{
    func_004247D8(self, (char*)self + 0x1FEF);
    func_0038F738(self, 0x1FF0, 2);
    *(void**)((char*)self + 0x1FF0) = self;
}
#endif

// 0x1c-byte elements reached through a pointer at self+0x1ff0, indexed by
// the cursor at self+0x23dc (which the function then advances)
struct sRailLink {
    char pad_0x00[0x10];
    int field_0x10;
    char pad_0x14[0x8];
};

//95.0% - logic verified correct; the target puts the mult result in v1 and
// ours lands in v0. Reordering the locals does not shift the allocation.
INCLUDE_ASM("object/railmodifier", func_00365FE8);
#ifdef SKIP_ASM
extern "C" void func_00365FE8(void* self)
{
    int i = *(int*)((char*)self + 0x23dc);
    sRailLink* b = *(sRailLink**)((char*)self + 0x1ff0);
    *(int*)((char*)self + 0x23dc) = b[i].field_0x10;
}
#endif

//100%
INCLUDE_ASM("object/railmodifier", func_00366008);
#ifdef SKIP_ASM
struct sRailNode {
    int field_0x0;
    int size;               // 0x04
    int field_0x8;
    int prev;               // 0x0c
    int next;               // 0x10
    int prev2;              // 0x14
    int next2;              // 0x18
};

struct sRailNodeMan {
    char pad_0x0[0x1ff0];
    sRailNode* nodes;       // 0x1ff0
    int field_0x1ff4;
    sRailNode headsA[17];   // 0x1ff8
    sRailNode headsB[18];   // 0x21d4
    char pad_0x23cc[0x10];
    int freeHead;           // 0x23dc
};

extern "C" void func_00366008(sRailNodeMan* self, int idx)
{
    sRailNode* node = &self->nodes[idx];
    sRailNode* prev;
    int link = node->prev;
    if (link < 0) {
        if ((link & 0x70000000) == 0x10000000) {
            prev = &self->headsA[link & 0x0FFFFFFF];
        } else {
            prev = &self->headsB[link & 0x0FFFFFFF];
        }
    } else {
        prev = &self->nodes[link];
    }
    prev->next = node->next;
    if (node->next >= 0) {
        self->nodes[node->next].prev = node->prev;
    }
    node->next = self->freeHead;
    self->freeHead = idx;
}
#endif

INCLUDE_ASM("object/railmodifier", func_003660A8);

//100%
INCLUDE_ASM("object/railmodifier", func_00366238);
#ifdef SKIP_ASM
extern "C" void func_00366238(sRailNodeMan* self, int idx)
{
    sRailNode* node = &self->nodes[idx];
    sRailNode* prev;
    int link = node->prev2;
    if (link < 0) {
        if ((link & 0x70000000) == 0x10000000) {
            prev = &self->headsA[link & 0x0FFFFFFF];
        } else {
            prev = &self->headsB[link & 0x0FFFFFFF];
        }
    } else {
        prev = &self->nodes[link];
    }
    prev->next2 = node->next2;
    if (node->next2 >= 0) {
        self->nodes[node->next2].prev2 = node->prev2;
    }
}
#endif

INCLUDE_ASM("object/railmodifier", func_003662D0);

INCLUDE_ASM("object/railmodifier", func_003663D8);

INCLUDE_ASM("object/railmodifier", func_00366548);

INCLUDE_ASM("object/railmodifier", func_00366618);

INCLUDE_ASM("object/railmodifier", func_003666F8);

//100%
INCLUDE_ASM("object/railmodifier", func_003667F8);
#ifdef SKIP_ASM
// PORT: PS2-only MMI count-leading-sign-bits (plzcw); off-PS2 use a clz builtin.
static inline int railFloorLog2(int x)
{
    long r;
    __asm__("plzcw %0, %1\n\txori %0, %0, 0x1f\n\taddiu %0, %0, -1" : "=r"(r) : "0"(x));
    return r;
}

extern "C" int func_003667F8(sRailNodeMan* self, int size)
{
    int b;
    for (b = railFloorLog2(size) - 6; b < 17; b++) {
        int i = self->headsB[b].next2;
        while (i >= 0) {
            if (self->nodes[i].size >= size) {
                return i;
            }
            i = self->nodes[i].next2;
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("object/railmodifier", func_003668F8);

INCLUDE_ASM("object/railmodifier", func_003669F0);

INCLUDE_ASM("object/railmodifier", func_00366CE0);

INCLUDE_ASM("object/railmodifier", func_00366E30);

INCLUDE_ASM("object/railmodifier", func_00366E90);

//100%
INCLUDE_ASM("object/railmodifier", func_00366FE0__FPvi);
#ifdef SKIP_ASM
void func_00366FE0(void* self, int val)
{
    *(int*)((char*)self + 0x1F58) = val;
}
#endif

INCLUDE_ASM("object/railmodifier", func_00366FE8);

INCLUDE_ASM("object/railmodifier", func_00367078);

INCLUDE_ASM("object/railmodifier", func_003670E0);

