#include "common.h"

INCLUDE_ASM("seg/seg_1F1548", cPSPVisualEffectsMan_cPSPVisualEffectsMan);

INCLUDE_ASM("seg/seg_1F1548", func_002F0820);

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

extern void* D_004A28A8;
extern char* D_004A289C;
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
    for (int i = 0; i < *(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84) + 0x10); i++) {
        func_002F00A0(self, (&D_004A3BA8)[i]); // PORT: D_004A3BA8 is an int array ($gp-addressed)
        ((cWorld_0A98*)D_004A289C)->ResetSlot(i);
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

INCLUDE_ASM("seg/seg_1F1548", func_002F0E88);

INCLUDE_ASM("seg/seg_1F1548", func_002F0FE8);

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F1148);
#ifdef SKIP_ASM
extern "C" void func_002F1148(void *arg0) {
    (*(int *)((char*)(arg0) + (0xA0))) = 0;
}
#endif

INCLUDE_ASM("seg/seg_1F1548", func_002F1150);

INCLUDE_ASM("seg/seg_1F1548", func_002F1510);

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
extern "C" void func_00370C08(void *);
extern char D_004881E8[];

extern "C" void func_002F17A8(void *arg0) {
    *(char **)((char*)arg0 + 0x1F8) = D_004881E8;
    func_00370C08(arg0);
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

INCLUDE_ASM("seg/seg_1F1548", func_002F1A08);

INCLUDE_ASM("seg/seg_1F1548", func_002F2088);

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

INCLUDE_ASM("seg/seg_1F1548", func_002F2218);

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

INCLUDE_ASM("seg/seg_1F1548", func_002F22C8);

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2588);
#ifdef SKIP_ASM
extern "C" int *func_002F2588(int *arg0) {
    *arg0 = 0;
    return arg0;
}
#endif

INCLUDE_ASM("seg/seg_1F1548", func_002F2598);

INCLUDE_ASM("seg/seg_1F1548", func_002F2810);

INCLUDE_ASM("seg/seg_1F1548", func_002F2C30);

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F2D60);
#ifdef SKIP_ASM
extern "C" void *func_002F2D60(void *arg0) {
    (*(int *)((char*)(arg0) + (4))) = 0;
    return arg0;
}
#endif

INCLUDE_ASM("seg/seg_1F1548", func_002F2D70);

INCLUDE_ASM("seg/seg_1F1548", func_002F3030);

INCLUDE_ASM("seg/seg_1F1548", func_002F3418);

INCLUDE_ASM("seg/seg_1F1548", func_002F3550);

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F3618);
#ifdef SKIP_ASM
extern "C" void func_003546C8(void *);
extern char D_00488230[];

extern "C" void func_002F3618(void *arg0) {
    *(char **)((char*)arg0 + 0xC) = D_00488230;
    func_003546C8(arg0);
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

INCLUDE_ASM("seg/seg_1F1548", func_002F39E0);

INCLUDE_ASM("seg/seg_1F1548", func_002F3E28);

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F4118);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void *func_002306A8(void *, int);
extern "C" void func_002F4260_4118(void *, float, void *) __asm__("func_002F4260");
extern void *D_004A28A8;
struct N4118 { char pad[0x84]; N4118 *c; };
struct M4118 { char pad[0x10]; unsigned int cnt; };

extern "C" void func_002F4118(void *arg0, float arg1) {
    unsigned int i = 0;
    if (((M4118 *)((N4118 *)((N4118 *)D_004A28A8)->c)->c)->cnt != 0) {
        do {
            func_002F4260_4118(func_002306A8(((N4118 *)D_004A28A8)->c, i++), arg1, arg0);
        } while (i < ((M4118 *)((N4118 *)((N4118 *)D_004A28A8)->c)->c)->cnt);
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
extern void *D_004A28A8;
#define F84(p) (*(char **)((char*)(p) + 0x84))

extern "C" void func_002F41A8(int arg0) {
    unsigned int i = 0;
    if (*(unsigned int *)(F84(F84(D_004A28A8)) + 0x10) != 0) {
        do {
            char *t5 = F84(D_004A28A8);
            if (arg0 == *(int *)(F84(t5) + (i << 2) + 4)) {
                if (func_002306A8_41A8(t5, i) != 0) {
                    Obj41A8 *o = func_002306A8_41A8(F84(D_004A28A8), i);
                    o->v13();
                }
            }
            i += 1;
        } while (i < *(unsigned int *)(F84(F84(D_004A28A8)) + 0x10));
    }
}
#endif

INCLUDE_ASM("seg/seg_1F1548", func_002F4260);

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

INCLUDE_ASM("seg/seg_1F1548", func_002F43E0);

INCLUDE_ASM("seg/seg_1F1548", func_002F4690);

INCLUDE_ASM("seg/seg_1F1548", func_002F4A08);

INCLUDE_ASM("seg/seg_1F1548", func_002F4DB8);

//100%
INCLUDE_ASM("seg/seg_1F1548", func_002F5148);
#ifdef SKIP_ASM
extern "C" void func_002F5400(int, void *, int);
extern void *D_004A28A8;

extern "C" void func_002F5148(void *arg0) {
    func_002F5400((*(int *)((char*)((*(void **)((char*)(D_004A28A8) + (0x84)))) + (0x90))), arg0, ((int) (*(int *)((char*)(arg0) + (8))) >> 0xC) & 1);
}
#endif

INCLUDE_ASM("seg/seg_1F1548", func_002F5180);

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
