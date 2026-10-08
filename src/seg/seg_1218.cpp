#include "common.h"

INCLUDE_ASM("seg/seg_1218", func_00100218);

//100%
INCLUDE_ASM("seg/seg_1218", func_00100250);
#ifdef SKIP_ASM
struct sVec4_0250 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vDot_0250(const sVec4_0250& a, const sVec4_0250& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_0250 vScale_0250(const sVec4_0250& v, float s)
{
    sVec4_0250 r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4_0250 vSub_0250(const sVec4_0250& a, const sVec4_0250& b)
{
    sVec4_0250 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4_0250 vNorm_0250(const sVec4_0250& v)
{
    sVec4_0250 r;
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

extern "C" float func_00100250(void* self, const sVec4_0250* v)
{
    char* p = *(char**)((char*)self + 0x18);
    sVec4_0250 a = *(sVec4_0250*)(p + 0x1A0);
    sVec4_0250 b = *(sVec4_0250*)(p + 0x1C0);
    sVec4_0250 u = vSub_0250(*v, vScale_0250(a, vDot_0250(*v, a)));
    float r = -vDot_0250(vNorm_0250(u), b);
    float f = *(float*)((char*)self + 0xDF4);
    if (f == 1.0f)
        return r;
    return r * 0.12667641043663025f * f;
}
#endif

INCLUDE_ASM("seg/seg_1218", func_00100348);

INCLUDE_ASM("seg/seg_1218", func_00100610);

INCLUDE_ASM("seg/seg_1218", func_00100680);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", func_001009E0);
#ifdef SKIP_ASM
struct sVec4_09E0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPad_09E0 {
    unsigned int pad0 : 14;
    unsigned int b14 : 2;
    unsigned int pad16 : 4;
    int b20 : 6;
    unsigned int pad26 : 6;
    int w0 : 6;
};

class cObj_8E88;
typedef void (cObj_8E88::*Fn_8E88)();
extern "C" Fn_8E88 D_0043CF20[4];

extern "C" int func_0010B980(char* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6);
extern "C" void func_0010BFA8_09E0(char*, int, int, int, int) __asm__("func_0010BFA8");
extern "C" void func_00100610_09E0(sVec4_09E0*, void*) __asm__("func_00100610");
extern "C" float func_00100348_09E0(void*, sVec4_09E0*) __asm__("func_00100348");
extern "C" void func_00100680(void*, void*);
extern "C" void func_0010C140(void*, void*);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_09E0(const sVec4_09E0& v)
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

// PORT: PS2 abs.s helper (stands in for fabsf).
static inline float vAbs_09E0(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

extern "C" void func_001009E0(char* self, sPad_09E0* pad)
{
    int below, o3, o4, o5, o6;
    if (!func_0010B980(self, (float*)(self + 0xDF0), &below, &o3, &o4, &o5, &o6)) {
        *(Fn_8E88*)(self + 0xF44) = D_0043CF20[1];
        *(short*)(self + 0xF38) = 1;
        func_00100680(self, pad);
        return;
    }
    sVec4_09E0 v;
    func_00100610_09E0(&v, self);
    float f = func_00100348_09E0(self, &v);
    pad->b20 = (int)(f * 31.0f);
    char* rider = *(char**)(self + 0x18);
    float len = vLen_09E0(*(sVec4_09E0*)(rider + 0x1E0));
    if (below && *(float*)(rider + 0x4C8) < 10000.0f && vAbs_09E0(f) < 1.0f) {
        func_0010BFA8_09E0(self, o5, o6, o4, o3);
        pad->b14 = 3;
        return;
    }
    float sp = *(float*)(self + 0xDF0);
    if (len < sp - 138.88890075683594f) {
        func_0010C140(self, pad);
        return;
    }
    if (sp + 138.88890075683594f < len)
        pad->w0 = 0x1F;
}
#endif

INCLUDE_ASM("seg/seg_1218", func_00100B90);

INCLUDE_ASM("seg/seg_1218", func_00100F88);

INCLUDE_ASM("seg/seg_1218", func_00101310);

INCLUDE_ASM("seg/seg_1218", func_001013A8);

INCLUDE_ASM("seg/seg_1218", func_00101688);

//100%
INCLUDE_ASM("seg/seg_1218", func_00101728);
#ifdef SKIP_ASM
struct sVec4_1728 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sXform_1728 {
    char pad[0x30];
    sVec4_1728 pos;
};

class cComp_1728 {
public:
    int f0;
    int f4;
    int f8;
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
    virtual int IsDead();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual sXform_1728* GetXform();
};

struct sObj_1728 {
    int pad[3];
    cComp_1728* comp;
};

struct sBank_1728 {
    char pad[0x1C];
    unsigned int* handles;
};

struct sWorld_1728 {
    int pad[2];
    sBank_1728** banks;
};

struct sEnt_1728 {
    float x;
    float y;
    int state;
    float px, py, pz;
    unsigned int id;
};

struct sMgr_1728 {
    int f0;
    sEnt_1728 ents[64];
    int count;
};

extern sWorld_1728** D_004A47B8;
extern "C" void func_00101888(sMgr_1728*, unsigned int*);

static inline sObj_1728* lookup_1728(unsigned int id)
{
    sBank_1728* b = (*D_004A47B8)->banks[id & 0xFF];
    if (b) {
        unsigned int v = b->handles[id >> 8] >> 8;
        if (v)
            return (sObj_1728*)(v << 2); // PORT: packed pointer in a 32-bit handle
    }
    return 0;
}

extern "C" void func_00101728(sMgr_1728* self, unsigned int* idp, float x, float y)
{
    cComp_1728* c = lookup_1728(*idp)->comp;
    if (c == 0)
        return;
    if (c->IsDead())
        return;
    func_00101888(self, idp);
    for (int i = 0; i < 64; i++) {
        sEnt_1728* e = &self->ents[i];
        if (e->state == 2) {
            e->state = 0;
            e->id = *idp;
            e->x = x;
            e->y = y;
            sVec4_1728 p = c->GetXform()->pos;
            e->px = p.x;
            e->py = p.y;
            e->pz = p.z;
            self->count++;
            return;
        }
    }
}
#endif

INCLUDE_ASM("seg/seg_1218", func_00101888);

INCLUDE_ASM("seg/seg_1218", func_00101900);

INCLUDE_ASM("seg/seg_1218", func_00101970);

//100%
INCLUDE_ASM("seg/seg_1218", func_00101A10);
#ifdef SKIP_ASM
extern "C" int func_00101A10(int arg0, int arg1) {
    return (*(int *)((char*)(((arg1 * 0xC) + arg0)) + (0x710)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", func_00101A28);
#ifdef SKIP_ASM
struct sLink_1A28 {
    sLink_1A28* next;
};

struct sNode_1A28 {
    sNode_1A28* kids[8];
    sLink_1A28* items;
};

struct sList_1A28 {
    int n;
    sLink_1A28* items[1];
};

extern int D_004A3DD8;
extern "C" int func_0030A310(int, sLink_1A28*);

extern "C" void func_00101A28(sList_1A28* list, sNode_1A28* node, int recurse)
{
    sLink_1A28* p;
    for (p = node->items; p; p = p->next) {
        if (func_0030A310(D_004A3DD8, p))
            list->items[list->n++] = p;
    }
    if (recurse) {
        if (node->kids[0]) func_00101A28(list, node->kids[0], 1);
        if (node->kids[1]) func_00101A28(list, node->kids[1], 1);
        if (node->kids[2]) func_00101A28(list, node->kids[2], 1);
        if (node->kids[3]) func_00101A28(list, node->kids[3], 1);
        if (node->kids[4]) func_00101A28(list, node->kids[4], 1);
        if (node->kids[5]) func_00101A28(list, node->kids[5], 1);
        if (node->kids[6]) func_00101A28(list, node->kids[6], 1);
        if (node->kids[7]) func_00101A28(list, node->kids[7], 1);
    }
}
#endif

INCLUDE_ASM("seg/seg_1218", func_00101B60);

INCLUDE_ASM("seg/seg_1218", func_001032C0);

INCLUDE_ASM("seg/seg_1218", func_00103308);

INCLUDE_ASM("seg/seg_1218", func_00103358);

INCLUDE_ASM("seg/seg_1218", func_001033B0);

INCLUDE_ASM("seg/seg_1218", func_001033F8);

//100%
INCLUDE_ASM("seg/seg_1218", func_00103480);
#ifdef SKIP_ASM
class cStream_3480 {
public:
    virtual void Write(void* p, int n);
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void WriteInt(int v);
};

struct sEnt_3480 {
    char d[0x20];
};

struct sObj_3480 {
    int n;
    char pad4[0x1C];
    sEnt_3480 ents[5];
    char padC0[0x10];
    int fD0;
    int pad;
    int m;
    int vals[1];
};

extern "C" void func_00103480(sObj_3480* self, cStream_3480* s)
{
    s->Write(&self->fD0, 4);
    for (int i = 0; i < self->n; i++)
        s->Write(&self->ents[i], 0x10);
    s->Write(&self->m, 4);
    for (int j = 0; j < self->m; j++)
        s->WriteInt(self->vals[j]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", func_00103578);
#ifdef SKIP_ASM
struct sItem_3578 {
    int f0;
    int f4;
    unsigned int flags;
};

class cStream_3578 {
public:
    virtual void v0();
    virtual void Read(void* p, int n);
    virtual sItem_3578* ReadPtr();
};

struct sEnt_3578 {
    char d[0x20];
};

struct sObj_3578 {
    int n;
    char pad4[0x1C];
    sEnt_3578 ents[5];
    char padC0[0x10];
    int fD0;
    int fD4;
    int m;
    sItem_3578* items[1];
};

extern "C" void func_00103578(sObj_3578* self, cStream_3578* s)
{
    s->Read(&self->fD0, 4);
    for (int i = 0; i < self->n; i++)
        s->Read(&self->ents[i], 0x10);
    self->fD4 = 1;
    s->Read(&self->m, 4);
    for (int j = 0; j < self->m; j++) {
        sItem_3578* it = s->ReadPtr();
        it->flags = (it->flags | 0x100) & 0xFFFFFDFF;
        self->items[j] = it;
    }
}
#endif

INCLUDE_ASM("seg/seg_1218", func_001036A0);

INCLUDE_ASM("seg/seg_1218", func_00103918);

//100%
INCLUDE_ASM("seg/seg_1218", func_00103AA0);
#ifdef SKIP_ASM
struct sEnt_3AA0 {
    char pad[0x10];
    unsigned short first;
    unsigned short count;
};

struct sRec_3AA0 {
    unsigned short a;
    unsigned short b;
};

struct sBank_3AA0 {
    int pad[2];
    sEnt_3AA0* ents;
    int padC;
    sRec_3AA0* recs;
};

struct sObj_3AA0 {
    int f0;
    unsigned int id;
    char pad[0xA8];
    char sub[1];
};

struct sBankTab_3AA0 {
    sBank_3AA0* banks[1];
};

extern sBankTab_3AA0* D_004A3DF8;
extern "C" int func_001446A0(void*, int);
extern "C" int func_001446B8(void*, int);
extern "C" void func_00144670(void*, int);
extern "C" void func_00104CC8(void*, int);

extern "C" void func_00103AA0(void* self, sObj_3AA0* o)
{
    int n = D_004A3DF8->banks[o->id & 0xFF]->ents[o->id >> 8].count;
    for (int i = 0; i < n; i++) {
        int ok = 0;
        if (func_001446A0(o->sub, i) && func_001446B8(o->sub, i))
            ok = 1;
        if (ok) {
            sBank_3AA0* b = D_004A3DF8->banks[o->id & 0xFF];
            unsigned short r = b->recs[b->ents[o->id >> 8].first + i].b;
            if (r & 0x8000) {
                func_00144670(o->sub, i);
                func_00104CC8(self, r & 0x7FFF);
            }
        }
    }
}
#endif

INCLUDE_ASM("seg/seg_1218", func_00103BE0);

//100%
INCLUDE_ASM("seg/seg_1218", func_00103CC8);
#ifdef SKIP_ASM
extern char* D_004A3E7C;
extern "C" void func_00313D40(void*, int, int);
extern "C" void func_003135B0(void*, int, float);
extern "C" void func_00313C50(void*, int, int);
extern "C" void func_00313CF0(void*, int, float);
extern "C" void func_00313D28(void*, int, float);
extern "C" int func_00313800(void*, float);
extern "C" void func_003145F8(void*, void*);

extern "C" int func_00103CC8(void* self, void* x, char* anim, int a, int b, float dt, float w, int c)
{
    func_00313C50(anim, 0, *(int*)(D_004A3E7C + b * 4 + 0x1030));
    if (0.0f < w) {
        func_00313C50(anim, 1, *(int*)(D_004A3E7C + c * 4 + 0x1030));
    } else {
        w = -w;
        func_00313C50(anim, 1, *(int*)(D_004A3E7C + a * 4 + 0x1030));
    }
    func_00313D28(anim, 0, 1.0f - w);
    func_00313D28(anim, 1, w);
    func_00313D40(anim, 0, 1);
    float t = dt * 0.01666666753590107f;
    func_003135B0(anim, 0, t);
    func_00313CF0(anim, 1, *(float*)(anim + 8) / *(float*)(anim + 0x10) * *(float*)(anim + 0x2C));
    if (func_00313800(anim, t)) {
        func_003145F8(x, anim);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", func_00103E28);
#ifdef SKIP_ASM
extern char* D_004A3E7C;
extern "C" void func_00313D40(void*, int, int);
extern "C" void func_003135B0(void*, int, float);
extern "C" void func_00313C50(void*, int, int);
extern "C" void func_00313CF0(void*, int, float);
extern "C" void func_00313D28(void*, int, float);
extern "C" int func_00313800(void*, float);
extern "C" void func_003145F8(void*, void*);

extern "C" int func_00103E28(void* self, void* x, char* anim, int i0, int i1, int i2, int i3, int i4, float dt, float v)
{
    int from, to;
    float w;
    if (v > 0.5f) {
        from = i4;
        to = i3;
        w = (v - 0.5f) * 2.0f;
    } else if (v > 0.0f) {
        from = i3;
        to = i2;
        w = v * 2.0f;
    } else if (v > -0.5f) {
        from = i2;
        to = i1;
        w = v * 2.0f + 1.0f;
    } else {
        from = i1;
        to = i0;
        w = (v + 1.0f) * 2.0f;
    }
    if (*(int*)(D_004A3E7C + from * 4 + 0x1030) != *(int*)(anim + 4)) {
        float r = *(float*)(anim + 8) / *(float*)(anim + 0x10);
        func_00313C50(anim, 0, *(int*)(D_004A3E7C + from * 4 + 0x1030));
        func_00313CF0(anim, 0, *(float*)(anim + 0x10) * r);
    }
    func_00313C50(anim, 1, *(int*)(D_004A3E7C + to * 4 + 0x1030));
    func_00313D28(anim, 0, w);
    func_00313D28(anim, 1, 1.0f - w);
    func_00313D40(anim, 0, 1);
    float t = dt * 0.01666666753590107f;
    func_003135B0(anim, 0, t);
    func_00313CF0(anim, 1, *(float*)(anim + 8) / *(float*)(anim + 0x10) * *(float*)(anim + 0x2C));
    if (func_00313800(anim, t)) {
        func_003145F8(x, anim);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", func_00103FF8);
#ifdef SKIP_ASM
extern char* D_004A3E7C;
extern "C" void func_00313D40(void*, int, int);
extern "C" void func_003135B0(void*, int, float);
extern "C" void func_00313C50(void*, int, int);
extern "C" void func_00313CF0(void*, int, float);
extern "C" int func_00313800(void*, float);
extern "C" void func_003145F8(void*, void*);

extern "C" int func_00103FF8(void* self, void* x, char* anim, int a, float dt, int b)
{
    if (*(int*)(anim + 4) == *(int*)(D_004A3E7C + a * 4 + 0x1030)) {
        func_00313D40(anim, 0, 0);
        func_003135B0(anim, 0, dt * 0.01666666753590107f);
        if (*(float*)(anim + 8) >= *(float*)(anim + 0x10)) {
            func_00313C50(anim, 0, *(int*)(D_004A3E7C + b * 4 + 0x1030));
            func_00313D40(anim, 0, 1);
            func_00313CF0(anim, 0, 0.0f);
        }
    } else {
        func_003135B0(anim, 0, dt * 0.01666666753590107f);
    }
    if (func_00313800(anim, dt * 0.01666666753590107f)) {
        func_003145F8(x, anim);
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("seg/seg_1218", func_00104110);

INCLUDE_ASM("seg/seg_1218", func_00104178);

INCLUDE_ASM("seg/seg_1218", func_00104238);

INCLUDE_ASM("seg/seg_1218", func_001042A8);

INCLUDE_ASM("seg/seg_1218", func_001042E0);

INCLUDE_ASM("seg/seg_1218", func_00104358);

INCLUDE_ASM("seg/seg_1218", func_001043F8);

INCLUDE_ASM("seg/seg_1218", func_001045B8);

INCLUDE_ASM("seg/seg_1218", func_001045D8);

INCLUDE_ASM("seg/seg_1218", func_00104660);

INCLUDE_ASM("seg/seg_1218", func_001046B0);

INCLUDE_ASM("seg/seg_1218", func_00104728);

INCLUDE_ASM("seg/seg_1218", func_001047F0);

INCLUDE_ASM("seg/seg_1218", func_001048C0);

INCLUDE_ASM("seg/seg_1218", func_00104940);

INCLUDE_ASM("seg/seg_1218", func_001049C0);

INCLUDE_ASM("seg/seg_1218", func_00104A40);

INCLUDE_ASM("seg/seg_1218", func_00104A60);

INCLUDE_ASM("seg/seg_1218", func_00104B48);

INCLUDE_ASM("seg/seg_1218", func_00104B78);

INCLUDE_ASM("seg/seg_1218", func_00104B98);

INCLUDE_ASM("seg/seg_1218", func_00104BB8);

INCLUDE_ASM("seg/seg_1218", func_00104BD8);

INCLUDE_ASM("seg/seg_1218", func_00104C18);

INCLUDE_ASM("seg/seg_1218", func_00104C38);

INCLUDE_ASM("seg/seg_1218", func_00104C80);

INCLUDE_ASM("seg/seg_1218", func_00104CA0);

INCLUDE_ASM("seg/seg_1218", func_00104CC8);

INCLUDE_ASM("seg/seg_1218", func_00104CF8);

//100%
INCLUDE_ASM("seg/seg_1218", func_00104D38);
#ifdef SKIP_ASM
extern "C" void func_00104D38(void *arg0, void *arg1) {
    (*(int *)((char*)(arg0) + (0x64))) = 0;
    (*(void **)((char*)(arg0) + (0x60))) = arg1;
    (*(int *)((char*)(arg0) + (0x54))) = (int) (*(int *)((char*)(arg1) + (0x780)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", func_00104D50);
#ifdef SKIP_ASM
extern "C" void func_00104D50(void *arg0, void *arg1) {
    (*(void **)((char*)(arg0) + (0x64))) = arg1;
    (*(int *)((char*)(arg0) + (0x60))) = 0;
    (*(int *)((char*)(arg0) + (0x54))) = (int) (*(int *)((char*)(arg1) + (8)));
}
#endif

INCLUDE_ASM("seg/seg_1218", func_00104D68);

INCLUDE_ASM("seg/seg_1218", func_00104DE0);

INCLUDE_ASM("seg/seg_1218", func_00104E70);

INCLUDE_ASM("seg/seg_1218", func_00105398);

INCLUDE_ASM("seg/seg_1218", func_001057B8);

INCLUDE_ASM("seg/seg_1218", func_00105D98);

INCLUDE_ASM("seg/seg_1218", func_00106538);

INCLUDE_ASM("seg/seg_1218", func_001065B0);

//100%
INCLUDE_ASM("seg/seg_1218", func_00106828);
#ifdef SKIP_ASM
extern "C" void func_00329AE0(int);

extern "C" void func_00106828(void *arg0) {
    func_00329AE0((*(int *)((char*)(arg0) + (0xAA0))));
}
#endif

INCLUDE_ASM("seg/seg_1218", func_00106848);

INCLUDE_ASM("seg/seg_1218", func_00106F78);

INCLUDE_ASM("seg/seg_1218", func_00107578);

INCLUDE_ASM("seg/seg_1218", func_00107888);

INCLUDE_ASM("seg/seg_1218", func_00107E70);

INCLUDE_ASM("seg/seg_1218", func_00108388);

INCLUDE_ASM("seg/seg_1218", func_001086B8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", func_00108A48);
#ifdef SKIP_ASM
struct sVec4_8A48 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sNode_8A48 {
    sVec4_8A48 pos;
    char pad[0x10];
};

struct sPath_8A48 {
    char pad[0x2C];
    sNode_8A48* nodes;
};

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec4_8A48 vAdd_8A48(const sVec4_8A48& a, const sVec4_8A48& b)
{
    sVec4_8A48 r;
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
static inline sVec4_8A48 vSub_8A48(const sVec4_8A48& a, const sVec4_8A48& b)
{
    sVec4_8A48 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_8A48(const sVec4_8A48& v)
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
static inline float vDot_8A48(const sVec4_8A48& a, const sVec4_8A48& b)
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

int func_0011FE98(void*);
extern "C" int func_00334680(void*, sVec4_8A48*, sVec4_8A48*, int, float);
extern "C" int func_00311AE8(void*, int);
extern "C" char* func_00311B20(void*, int);
extern "C" int func_001446A0(void*, int);
extern "C" int func_001086B8(void*, sVec4_8A48*, sVec4_8A48*);

extern "C" int func_00108A48(char* self, sVec4_8A48* tgt)
{
    if (func_0011FE98(self) != 0 && func_0011FE98(self) != 1)
        return 0;
    sVec4_8A48 p = vAdd_8A48(*(sVec4_8A48*)((char*)(*(sPath_8A48**)(self + 0x780))->nodes + (*(int*)(self + 0x8A0) << 5)),
                             *(sVec4_8A48*)(self + 0x9D0));
    if (!func_00334680(*(void**)(self + 0x860), &p, tgt, 1, 300.0f))
        return 0;
    float spd = vLen_8A48(*(sVec4_8A48*)(self + 0x1E0));
    if (0.001f < spd) {
        sVec4_8A48 d = vSub_8A48(*tgt, p);
        if (vDot_8A48(d, *(sVec4_8A48*)(self + 0x1E0)) < spd * -0.20000000298023224f)
            return 0;
    }
    if (func_00311AE8(*(void**)(self + 0x784), 2) == 0x12) {
        if (!func_001446A0(func_00311B20(*(void**)(self + 0x784), 2) + 0xB0, 2) &&
            func_001446A0(func_00311B20(*(void**)(self + 0x784), 2) + 0xB0, 0))
            return 0;
    } else if (func_00311AE8(*(void**)(self + 0x784), 2) == 0x13 ||
               func_00311AE8(*(void**)(self + 0x784), 2) == 0x14) {
        if (!func_001446A0(func_00311B20(*(void**)(self + 0x784), 2) + 0xB0, 2))
            return 0;
    }
    return func_001086B8(self, tgt, tgt + 1);
}
#endif

INCLUDE_ASM("seg/seg_1218", func_00108C28);

INCLUDE_ASM("seg/seg_1218", func_00108C80);

INCLUDE_ASM("seg/seg_1218", func_00108E88);

//100%
INCLUDE_ASM("seg/seg_1218", func_00108F88);
#ifdef SKIP_ASM
extern "C" void func_00108E88();

extern "C" void func_00108F88(void) {
    func_00108E88();
}
#endif

INCLUDE_ASM("seg/seg_1218", func_00108FA8);

INCLUDE_ASM("seg/seg_1218", func_0010A768);

INCLUDE_ASM("seg/seg_1218", func_0010A898);

INCLUDE_ASM("seg/seg_1218", func_0010A8E8);

//100%
INCLUDE_ASM("seg/seg_1218", func_0010A960);
#ifdef SKIP_ASM
struct sFlags_A960 {
    unsigned int pad0 : 12;
    unsigned int b12 : 1;
    unsigned int pad13 : 5;
    unsigned int b18 : 1;
    unsigned int b19 : 1;
};

class cObj_A960;
typedef void (cObj_A960::*Fn_A960)(sFlags_A960*);

class cObj_A960 {
public:
    virtual void v0();
    char pad[0xE20];
    int fE20;
    char padE24[0xF44 - 0xE24];
    Fn_A960 state;
};

extern "C" int func_0010D1A0(void*);
extern "C" int func_0010DA10(void*);
extern "C" int func_0010DBF0(void*, int*);

extern "C" void func_0010A960(cObj_A960* self, sFlags_A960* f)
{
    int t;
    f->b12 = func_0010D1A0(self);
    self->fE20 = 0;
    if (func_0010DBF0(self, &t)) {
        if (t)
            f->b19 = 1;
        else
            f->b18 = 1;
    } else if (func_0010DA10(self)) {
        f->b19 = 1;
        f->b18 = 1;
    }
    (self->*(self->state))(f);
}
#endif

INCLUDE_ASM("seg/seg_1218", func_0010AA70);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", func_0010AD78);
#ifdef SKIP_ASM
struct sVec4_AD78 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPad_AD78 {
    unsigned int pad0 : 18;
    int b18 : 6;
    int b24 : 6;
    unsigned int pad30 : 2;
    int w0 : 6;
};

extern "C" int func_0010BBF8(void*, int*);
extern "C" void func_00100610(sVec4_AD78*, void*);
extern "C" float func_00100348(void*, sVec4_AD78*);

// PORT: PS2 abs.s helper (stands in for fabsf).
static inline float vAbs_AD78(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float clamp_AD78(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

extern "C" void func_0010AD78(char* self, sPad_AD78* pad)
{
    int flag;
    int r = func_0010BBF8(self, &flag);
    float s = 1.0f;
    if (flag)
        s = -1.0f;
    {
        sVec4_AD78 v;
        func_00100610(&v, self);
        pad->b18 = (int)(func_00100348(self, &v) * 31.0f);
    }
    if (r) {
        pad->w0 = (int)(s * 31.0f);
        sVec4_AD78 p = *(sVec4_AD78*)(*(char**)(self + 0x18) + 0x1A0);
        float z = p.z + p.z;
        float y = clamp_AD78(z, -1.0f, 1.0f);
        if (vAbs_AD78(y) > 0.1f)
            pad->b24 = (int)(y * 31.0f);
    } else {
        pad->w0 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", func_0010AED8);
#ifdef SKIP_ASM
struct sVec4_AED8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPad_AED8 {
    unsigned int pad0 : 25;
    int b25 : 6;
    unsigned int pad31 : 1;
    int w0 : 6;
};

extern "C" int func_0010B980(char* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6);
extern "C" void func_0010BFA8_AED8(char*, int, int, int, int) __asm__("func_0010BFA8");
extern "C" int func_00311AE8(void*, int);
extern "C" int func_0010BB18(void*, float*, int*);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_AED8(const sVec4_AED8& v)
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

// PORT: PS2 abs.s helper (stands in for fabsf).
static inline float vAbs_AED8(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float clamp_AED8(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

extern "C" void func_0010AED8(char* self, sPad_AED8* pad)
{
    int below, o3, o4, o5, o6, ok;
    int r = func_0010B980(self, (float*)(self + 0xDF0), &below, &o3, &o4, &o5, &o6);
    *(int*)(self + 0xE20) = o4;
    *(unsigned int*)pad |= 0x1FE0000;
    if (r && below) {
        *(unsigned int*)pad |= 0x6000;
        func_0010BFA8_AED8(self, o5, o6, o4, o3);
        return;
    }
    sVec4_AED8 p = *(sVec4_AED8*)(*(char**)(self + 0x18) + 0x1A0);
    float z = p.z + p.z;
    float y = clamp_AED8(z, -1.0f, 1.0f);
    if (vAbs_AED8(y) > 0.1f)
        pad->b25 = (int)(y * 31.0f);
    char* rider = *(char**)(self + 0x18);
    if ((unsigned int)(*(int*)(rider + 0x328) - 3) >= 2 &&
        func_00311AE8(*(void**)(rider + 0x784), 2) != 0xE) {
        float s = (*(float*)(self + 0xE38) < 0.0f) ? 1.0f : -1.0f;
        pad->w0 = (int)(s * 31.0f);
    }
    int r2 = func_0010BB18(self, (float*)(self + 0xDF0), &ok);
    float len = vLen_AED8(*(sVec4_AED8*)(*(char**)(self + 0x18) + 0x1E0));
    if (r2 && ok && len < *(float*)(self + 0xDF0) - 138.88890075683594f)
        *(unsigned int*)pad |= 0x10000;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", func_0010B0E8);
#ifdef SKIP_ASM
extern "C" void func_00135CB0(void*, float*, float*);
extern "C" void func_00135DB0(void*, float*, float*);

extern "C" void func_0010B0E8(char* self, float t)
{
    float a, b, c, d;
    func_00135CB0(*(char**)(*(char**)(self + 0x18) + 0x77C) + 0x230, &a, &b);
    func_00135DB0(*(char**)(*(char**)(self + 0x18) + 0x77C) + 0x230, &c, &d);
    int stop = 0;
    if (*(int*)(self + 0xE20) && t < 1.0f)
        stop = 1;
    if (stop) {
        *(float*)(self + 0xE3C) = 0.0f;
        *(float*)(self + 0xE38) = 0.0f;
        return;
    }
    if (!*(int*)(self + 0xE10))
        return;
    if (*(float*)(self + 0xE38) != 0.0f) {
        float lim = t + 0.1f;
        if (lim < a)
            *(float*)(self + 0xE38) = 0.0f;
        else if (lim < b && a - 0.4f < 0.0f)
            *(float*)(self + 0xE38) = 0.0f;
    }
    if (*(float*)(self + 0xE3C) != 0.0f) {
        float lim = t + 0.1f;
        if (lim < c) {
            *(float*)(self + 0xE3C) = 0.0f;
            *(float*)(self + 0xE38) = 0.0f;
        } else if (lim < d && c - 0.4f < 0.0f) {
            *(float*)(self + 0xE3C) = 0.0f;
            *(float*)(self + 0xE38) = 0.0f;
        }
    }
}
#endif

INCLUDE_ASM("seg/seg_1218", func_0010B250);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", func_0010B590);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVec4_B590 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPad_B590 {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
};

struct sPadBits_B590 {
    unsigned int pad0 : 24;
    int b24 : 6;
};

int AIrand();
extern "C" void func_00100610_B590(sVec4_B590*, void*) __asm__("func_00100610");
extern "C" float func_00100348_B590(void*, sVec4_B590*) __asm__("func_00100348");

extern "C" void func_0010B590(char* self, sPad_B590* pad)
{
    char* o = *(char**)(*(char**)(self + 0x18) + 0x788);
    int st = *(int*)(o + 0xAC);
    int ok = 0;
    if (st == 1 || st == 3)
        ok = 1;
    float d;
    if (ok)
        d = *(float*)(o + 0x98) - *(float*)(o + 0xA0);
    else
        d = 0.0f;
    *(int*)(self + 0xE10) = 0;
    *(int*)(self + 0xE14) = 0;
    *(int*)(self + 0xE18) = 1;
    *(int*)(self + 0xE20) = 0;
    *(int*)(self + 0xE0C) = -1;
    if (d > 1.0f) {
        *(int*)(self + 0xE10) = 1;
        *(float*)(self + 0xE38) = 1.0f;
        if (AIrand() & 1)
            *(float*)(self + 0xE38) = -*(float*)(self + 0xE38);
    } else {
        *(float*)(self + 0xE38) = 0.0f;
    }
    if (d > 3.0f) {
        *(int*)(self + 0xE10) = 1;
        *(float*)(self + 0xE3C) = 1.0f;
        if (AIrand() & 1)
            *(float*)(self + 0xE3C) = -*(float*)(self + 0xE3C);
    } else {
        *(float*)(self + 0xE3C) = 0.0f;
    }
    if (d < 0.5f && *(float*)(self + 0xE3C) == 0.0f && *(float*)(self + 0xE38) == 0.0f) {
        sVec4_B590 v;
        func_00100610_B590(&v, self);
        ((sPadBits_B590*)pad)->b24 = (int)(-func_00100348_B590(self, &v) * 31.0f);
        *(int*)(self + 0xE0C) = -1;
    }
    pad->b2 = *(int*)(self + 0xE0C);
}
#endif

INCLUDE_ASM("seg/seg_1218", func_0010B750);

//100%
INCLUDE_ASM("seg/seg_1218", func_0010B790);
#ifdef SKIP_ASM
struct sPad_B790 {
    unsigned int pad0 : 12;
    int b12 : 6;
};

class cLvl_B790 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual int Level();
};

int AIrand();
extern "C" int func_001298C8();

extern "C" void func_0010B790(char* self, sPad_B790* pad)
{
    float t = 180.0f - (float)func_001298C8();
    float th = 33.0f;
    th -= (float)((((cLvl_B790*)(*(char**)(self + 0x18) + 0x6C0))->Level() - 1) * 10);
    if (t < th && ((cLvl_B790*)(*(char**)(self + 0x18) + 0x6C0))->Level() < 4) {
        pad->b12 = 0x1F;
        return;
    }
    if (t < 72.0f && ((cLvl_B790*)(*(char**)(self + 0x18) + 0x6C0))->Level() < 3) {
        pad->b12 = -0x1F;
        return;
    }
    if (func_001298C8() % 20 == 0) {
        unsigned int r = AIrand();
        float* p = (float*)(self + 0xDF0);
        *p = (float)r * 0.009999999776482582f;
        if ((AIrand() & 1) == 0)
            *(float*)(self + 0xDF0) = -*(float*)(self + 0xDF0);
    }
    pad->b12 = (int)(*(float*)(self + 0xDF0) * 31.0f);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", func_0010B980);
#ifdef SKIP_ASM
struct sVec4_B980 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRec_B980 {
    int type;
    int flags;
    float a;
    float b;
};

class cTrack_B980 {
public:
    char pad[0x34];
    virtual int Query(sRec_B980* buf, int max, float x, float y);
};

extern "C" sVec4_B980 func_0026AB20(cTrack_B980*, int, float);

extern "C" int func_0010B980(char* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6)
{
    sRec_B980 buf[12];
    char* rider = *(char**)(self + 0x18);
    cTrack_B980* trk = *(cTrack_B980**)(rider + 0xAB8);
    int n = trk->Query(buf, 12, *(float*)(rider + 0x4C0), *(float*)(rider + 0x4C4) + 300.0f);
    for (int i = 0; i < n; i++) {
        if (buf[i].type == 0x10) {
            int fl = buf[i].flags;
            *speed = (float)(fl >> 4) * 27.777780532836914f;
            *o3 = (fl >> 3) & 1;
            *o4 = (fl >> 2) & 1;
            *o5 = (fl >> 1) & 1;
            *o6 = fl & 1;
            *below = (buf[i].a <= *(float*)(*(char**)(self + 0x18) + 0x4C4)) ? 1 : 0;
            *(sVec4_B980*)(self + 0xE50) = func_0026AB20(trk, 0, buf[i].a);
            *(sVec4_B980*)(self + 0xE60) = func_0026AB20(trk, 0, buf[i].b);
            return 1;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("seg/seg_1218", func_0010BB18);

INCLUDE_ASM("seg/seg_1218", func_0010BBF8);

INCLUDE_ASM("seg/seg_1218", func_0010BC98);

INCLUDE_ASM("seg/seg_1218", func_0010BD10);

//100%
INCLUDE_ASM("seg/seg_1218", func_0010BFA8);
#ifdef SKIP_ASM
int AIrand();
extern "C" int func_0010C1D0(void*);

struct sObj_BFA8 {
    char pad[0xE0C];
    int fE0C;
    int fE10;
    int fE14;
    int fE18;
    int fE1C;
    int fE20;
    char padE24[0x14];
    float fE38;
    float fE3C;
};

extern "C" void func_0010BFA8(sObj_BFA8* self, int a, int b, int c, int d)
{
    self->fE0C = -1;
    int* p = &self->fE10;
    *p = (a || b);
    self->fE14 = func_0010C1D0(self);
    self->fE20 = c;
    self->fE1C = d;
    self->fE18 = 0;
    if (self->fE10) {
        self->fE38 = a ? 1.0f : 0.0f;
        self->fE3C = b ? 1.0f : 0.0f;
        if (AIrand() & 1)
            self->fE38 = -self->fE38;
        if (AIrand() & 1)
            self->fE3C = -self->fE3C;
    }
}
#endif

INCLUDE_ASM("seg/seg_1218", func_0010C0A8);

INCLUDE_ASM("seg/seg_1218", func_0010C140);

INCLUDE_ASM("seg/seg_1218", func_0010C1D0);

INCLUDE_ASM("seg/seg_1218", func_0010C258);

INCLUDE_ASM("seg/seg_1218", func_0010C320);

INCLUDE_ASM("seg/seg_1218", func_0010C3B8);

INCLUDE_ASM("seg/seg_1218", func_0010C450);
