#include "common.h"

INCLUDE_ASM("fe/festateloadscreen", cFELoadScreen_load);

INCLUDE_ASM("fe/festateloadscreen", func_00233640);

INCLUDE_ASM("fe/festateloadscreen", func_002338C8);

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233930);
#ifdef SKIP_ASM
struct cGame00233930 {
    char pad[0x10D8];
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
    virtual void v50(int id);
};

extern void* D_004A289C;
extern "C" void func_00397B70(void* self, int a1);
extern "C" void func_0036A020(void* self);
extern "C" void* func_0028B180();
extern "C" void func_0028FA98(void* self, float f);

extern "C" void func_00233930(void* self)
{
    ((cGame00233930*)D_004A289C)->v19();
    int id = *(int*)((char*)self + 0x18);
    if (id >= 0) {
        ((cGame00233930*)D_004A289C)->v50(id);
    }
    void* o = *(void**)((char*)self + 0xC);
    if (o != 0) {
        func_00397B70(o, 3);
    }
    int i;
    for (i = 0; i < 2; i++) {
        int h = *(int*)((char*)self + 0x10 + i * 4);
        if (h != 0) {
            ((cGame00233930*)D_004A289C)->v50(h);
        }
    }
    func_0036A020(D_004A289C);
    func_0028FA98(func_0028B180(), 1.0f);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_002339F8);
#ifdef SKIP_ASM
struct sVEntryI002339F8 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVEntryV002339F8 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern void* D_004A289C;
extern "C" int func_00231D60(void* self);
extern "C" void func_00397DF8(void* engine);

extern "C" int func_002339F8(void* self)
{
    char* g = (char*)D_004A289C;
    sVEntryI002339F8* vt = *(sVEntryI002339F8**)(g + 0x10D8);
    if (vt[17].fn(g + vt[17].delta) == 0) {
        return 0;
    }
    if (func_00231D60(self) == 0) {
        func_00397DF8(*(void**)((char*)self + 0xC));
    }
    g = (char*)D_004A289C;
    sVEntryV002339F8* vt2 = *(sVEntryV002339F8**)(g + 0x10D8);
    vt2[20].fn(g + vt2[20].delta);
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233A70);
#ifdef SKIP_ASM
extern "C" void func_00398038(void*);
void func_00231CB0(void*);

extern "C" void func_00233A70(void* self)
{
    func_00398038(*(void**)((char*)self + 0xC));
    func_00231CB0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233AA0);
#ifdef SKIP_ASM
extern void* D_004A2C68;
extern "C" void func_00231250(void* self, int a, int b, int refresh);

extern "C" void func_00233AA0(void* self)
{
    int a = *(int*)self;
    if (a != 0) {
        func_00231250(D_004A2C68, a, *(int*)((char*)self + 4), 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233AF0);
#ifdef SKIP_ASM
struct sVEntry_func_00233AF0 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

// PORT: the unit declares func_00233AF0 as `void* (void*)`; the body also uses $a1 (obj).
// Bound by asm label.
void* func_00233AF0_2(void* self, void* obj) __asm__("func_00233AF0");

void* func_00233AF0_2(void* self, void* obj)
{
    sVEntry_func_00233AF0* vt = *(sVEntry_func_00233AF0**)obj;
    return vt[1].fn((char*)obj + vt[1].delta, self, 4);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233B28);
#ifdef SKIP_ASM
struct sVEntry_func_00233B28 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

// PORT: the unit declares func_00233B28 as `void* (void*)`; the body also uses $a1 (obj).
// Bound by asm label.
void* func_00233B28_2(void* self, void* obj) __asm__("func_00233B28");

void* func_00233B28_2(void* self, void* obj)
{
    sVEntry_func_00233B28* vt = *(sVEntry_func_00233B28**)obj;
    return vt[2].fn((char*)obj + vt[2].delta, self, 4);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233B60);
#ifdef SKIP_ASM
extern void* D_004A2C68;
extern "C" void func_00231250(void* self, int a, int b, int refresh);

extern "C" void func_00233B60(void)
{
    func_00231250(D_004A2C68, 0xA, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233B88__FPv);
#ifdef SKIP_ASM
void func_00233B88(void* self)
{
}
#endif

extern "C" void* func_00233AF0(void* self);

//99.29%
INCLUDE_ASM("fe/festateloadscreen", func_00233B90__FPv);
#ifdef SKIP_ASM
void* func_00233B90(void* self)
{
    return func_00233AF0(self);
}
#endif

extern "C" void* func_00233B28(void* self);

//99.29%
INCLUDE_ASM("fe/festateloadscreen", func_00233BB0__FPv);
#ifdef SKIP_ASM
void* func_00233BB0(void* self)
{
    return func_00233B28(self);
}
#endif

//99.29%
INCLUDE_ASM("fe/festateloadscreen", func_00233BD0__FPv);
#ifdef SKIP_ASM
void* func_00233BD0(void* self)
{
    return func_00233AF0(self);
}
#endif

//99.29%
INCLUDE_ASM("fe/festateloadscreen", func_00233BF0__FPv);
#ifdef SKIP_ASM
void* func_00233BF0(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233C10);
#ifdef SKIP_ASM
extern void* D_0047D6E8[];
extern void* D_0047D5E0[];
extern void* D_004A2C68;

extern "C" void* func_00233C10(void* self)
{
    *(int*)self = 0;
    *(void***)((char*)self + 0xC) = D_0047D6E8;
    D_004A2C68 = 0;
    *(void***)((char*)self + 0xC) = D_0047D5E0;
    *(int*)self = 7;
    *(int*)((char*)self + 0x10) = 2;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x14) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233C50);
#ifdef SKIP_ASM
extern void* D_004A2C68;
extern void* D_004A28A4;
extern void* D_004A28A8;
extern void* D_004A2EEC;
extern "C" void func_0026FA50(void* self);
extern "C" void func_0039F840(void* list);
extern "C" void func_002790A0(void* self, int i);
extern "C" void func_001F36C0(void* self, int bit);
void func_00244880(void* self);

extern "C" void func_00233C50(void* self)
{
    func_0026FA50(*(void**)((char*)D_004A2C68 + 0x28));
    func_0039F840(*(char**)((char*)D_004A2C68 + 0x48) + 0x18);
    func_002790A0(D_004A28A4, 0);
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0xB4;
    func_001F36C0(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x94), 4);
    void* p = D_004A2EEC;
    if (p != 0) {
        func_00244880(p);
        *(int*)((char*)p + 0x90) = 1;
    }
}
#endif

INCLUDE_ASM("fe/festateloadscreen", func_00233CD8);

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00234008);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sLoadScreen00234008 {
    int state;
    int pad4[3];
    int mode;
};
extern void* D_004A28A8;
extern void* D_004A28A4;
void func_00162290_v(void* self) __asm__("func_00162290__FPv");
extern "C" int func_00278F68(void* self, int i, int a, int b);
void func_00278DE8(void* self, int val);

extern "C" void func_00234008(sLoadScreen00234008* self)
{
    int n = *(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84) + 0x10);
    int i;
    for (i = 0; i < n; i++) {
        char* list = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84);
        char* item = *(char**)(list + (i << 2) + 4);
        func_00162290_v(*(void**)(item + 0xA8));
    }
    self->mode = 2;
    func_00278F68(D_004A28A4, 0, 1, 0);
    func_00278DE8(D_004A28A4, 1);
    self->state = 7;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateloadscreen", func_002340B8);
#ifdef SKIP_ASM
struct sVEntry_func_002340B8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_002340B8(void* self, void* stream)
{
    func_00233AF0_2(self, stream);
    sVEntry_func_002340B8* e = &(*(sVEntry_func_002340B8**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_002340B8**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateloadscreen", func_00234120);
#ifdef SKIP_ASM
struct sVEntry_func_00234120 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00234120(void* self, void* stream)
{
    func_00233B28_2(self, stream);
    sVEntry_func_00234120* e = &(*(sVEntry_func_00234120**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00234120**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00234188);
#ifdef SKIP_ASM
extern void* D_0047D6E8[];
extern void* D_0047D530[];
extern void* D_004A2C68;

extern "C" void* func_00234188(void* self)
{
    *(int*)self = 0;
    *(void***)((char*)self + 0xC) = D_0047D6E8;
    D_004A2C68 = 0;
    *(void***)((char*)self + 0xC) = D_0047D530;
    *(int*)self = 2;
    *(int*)((char*)self + 0x1C) = 4;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x8) = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/festateloadscreen", func_002341D0);

INCLUDE_ASM("fe/festateloadscreen", func_002343B0);

INCLUDE_ASM("fe/festateloadscreen", func_00234750);

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00234910);
#ifdef SKIP_ASM
extern void* D_004A28A4;
extern "C" int func_00278F68(void* self, int i, int a, int b);

static inline int isState00234910(void* e, int s)
{
    return *(int*)((char*)e + 0x550) == s;
}

extern "C" void func_00234910(void* self)
{
    if (isState00234910(D_004A28A4, 2)) {
        func_00278F68(D_004A28A4, 1, 1, 1);
    }
    if (isState00234910(D_004A28A4, 2)) {
        func_00278F68(D_004A28A4, 0, 1, 1);
    }
    *(int*)((char*)self + 0x14) = 0;
    if (*(int*)((char*)D_004A28A4 + 0x550) == 1) {
        *(int*)((char*)self + 0x14) = 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateloadscreen", func_00234990);
#ifdef SKIP_ASM
// PORT: the unit declares func_00233AF0 as `void* (void*)`; the body also uses $a1 (obj).
void* func_00233AF0_2(void* self, void* obj) __asm__("func_00233AF0");

struct sVEntry_func_00234990 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00234990(void* self, void* stream)
{
    func_00233AF0_2(self, stream);
    sVEntry_func_00234990* e = &(*(sVEntry_func_00234990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00234990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00234990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x1C, 4);
    e = &(*(sVEntry_func_00234990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x8, 4);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateloadscreen", func_00234A30);
#ifdef SKIP_ASM
// PORT: the unit declares func_00233B28 as `void* (void*)`; the body also uses $a1 (obj).
void* func_00233B28_2(void* self, void* obj) __asm__("func_00233B28");

struct sVEntry_func_00234A30 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00234A30(void* self, void* stream)
{
    func_00233B28_2(self, stream);
    sVEntry_func_00234A30* e = &(*(sVEntry_func_00234A30**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00234A30**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00234A30**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x1C, 4);
    e = &(*(sVEntry_func_00234A30**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x8, 4);
}
#endif

