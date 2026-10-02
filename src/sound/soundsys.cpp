#include "common.h"

INCLUDE_ASM("sound/soundsys", cBankSys_cBankSys);

INCLUDE_ASM("sound/soundsys", func_0028FEA0);

INCLUDE_ASM("sound/soundsys", func_002906B8);

INCLUDE_ASM("sound/soundsys", func_00290B58);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_00290C10);
#ifdef SKIP_ASM
struct func_00290B58_sCurve {
    float x[5];
    float y[5];
};

extern "C" float func_00290B58(func_00290B58_sCurve* c, float v);
extern func_00290B58_sCurve D_00445898[];

extern "C" float func_00290C10(int i, float v)
{
    return func_00290B58(&D_00445898[i], v);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_00290C40);
#ifdef SKIP_ASM
struct sSndVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sSndVEntryVec {
    short delta;
    short index;
    sSndVec4* (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float sndVu0Length(const sSndVec4& v)
{
    float r;
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
        : "=r"(r)
        : "m"(v));
    return r;
}

extern "C" float func_00290C10(int i, float v);

extern "C" int func_00290C40(void* self, void* src)
{
    char* obj = (char*)src + 0x6C0;
    sSndVEntryVec* vt = *(sSndVEntryVec**)obj;
    sSndVec4* p = vt[2].fn(obj + vt[2].delta);
    int v = (int)func_00290C10(3, sndVu0Length(*p));
    if (v > 127) {
        v = 127;
    }
    if (v < 0) {
        v = 0;
    }
    return v;
}
#endif

INCLUDE_ASM("sound/soundsys", func_00290CC0);

//100%
INCLUDE_ASM("sound/soundsys", func_00290F58);
#ifdef SKIP_ASM
extern "C" int func_00285D98(void* self, int which);
extern "C" int func_00288CE8(void* self);

extern "C" void func_00290F58(void* self, int id, float v)
{
    if (func_00285D98(self, 0) == id) {
        *(float*)((char*)self + 0x6074) = v;
    } else if (func_00288CE8(self) == 2 && func_00285D98(self, 1) == id) {
        *(float*)((char*)self + 0x6078) = v;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_00290FD0);

INCLUDE_ASM("sound/soundsys", func_002910E0);

INCLUDE_ASM("sound/soundsys", func_002913D8);

INCLUDE_ASM("sound/soundsys", func_00291438);

INCLUDE_ASM("sound/soundsys", func_00291710);

//100%
INCLUDE_ASM("sound/soundsys", func_002917B8);
#ifdef SKIP_ASM
extern "C" int func_002917B8(void* self, int a1)
{
    if (a1 > 3) {
        return 1;
    }
    return 0x10;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002917D0);

INCLUDE_ASM("sound/soundsys", func_00291C88);

INCLUDE_ASM("sound/soundsys", func_00292508);

//100%
INCLUDE_ASM("sound/soundsys", func_002929D8);
#ifdef SKIP_ASM
extern "C" void* func_0028B1D8();
extern "C" void func_00292A50(void* self, int id);
extern "C" void func_00292B48(void* self);

extern "C" void func_002929D8(void* self)
{
    func_00292B48(self);
    for (int i = 0; i < *(int*)((char*)func_0028B1D8() + 0x78); i++) {
        func_00292A50(self, *(int*)((char*)func_0028B1D8() + (i << 2) + 0x28));
    }
    *(int*)((char*)self + 0x5830) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_00292A50);
#ifdef SKIP_ASM
int func_0011FE98(void*);
extern "C" int func_00291710(void* self, int id);
extern "C" void func_002917D0(void* self, int id);
extern "C" void func_00291C88(void* self, int id, int n);
extern "C" void func_00292508(void* self, int id, int n);

// PORT: id is a pointer passed as int (the unit declares func_00292A50(void*, int)).
extern "C" void func_00292A50(void* self, int id)
{
    int a = func_0011FE98((void*)id);
    int n = func_00291710(self, id);
    *(int*)((char*)id + 0x760) = a;
    *(int*)((char*)id + 0x764) = a;
    *(int*)((char*)id + 0x768) = n;
    *(int*)((char*)id + 0x76C) = n;
    func_002917D0(self, id);
    func_00291C88(self, id, n);
    func_00292508(self, id, n);
}
#endif

INCLUDE_ASM("sound/soundsys", func_00292AE8);

INCLUDE_ASM("sound/soundsys", func_00292B48);

INCLUDE_ASM("sound/soundsys", func_00294170);

INCLUDE_ASM("sound/soundsys", func_00294678);

INCLUDE_ASM("sound/soundsys", func_002947B0);

//100%
INCLUDE_ASM("sound/soundsys", func_00294880);
#ifdef SKIP_ASM
extern "C" void func_00294880(void* self, int i, int v)
{
    if (i < 6) {
        *(int*)((char*)self + (i << 2) + 0x59e8) = v;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002948A0);
#ifdef SKIP_ASM
extern "C" void func_002948A0(void* self)
{
    int i;
    for (i = 5; i >= 0; i--) {
        *(int*)((char*)self + (i << 2) + 0x59e8) = 0;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002948D0);

//100%
INCLUDE_ASM("sound/soundsys", func_00294F48);
#ifdef SKIP_ASM
struct sSndVEntry294F48 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

extern "C" int func_00294F48(void* self)
{
    sSndVEntry294F48* vt = *(sSndVEntry294F48**)((char*)self + 0xC);
    return vt[1].fn((char*)self + vt[1].delta, 0, 0);
}
#endif

INCLUDE_ASM("sound/soundsys", func_00294F78);

INCLUDE_ASM("sound/soundsys", func_00295028);

INCLUDE_ASM("sound/soundsys", func_00295208);

INCLUDE_ASM("sound/soundsys", func_00295628);

INCLUDE_ASM("sound/soundsys", func_00295950);

INCLUDE_ASM("sound/soundsys", func_00296088);

INCLUDE_ASM("sound/soundsys", func_002961F0);

INCLUDE_ASM("sound/soundsys", func_00296310);

INCLUDE_ASM("sound/soundsys", func_00296868);

//100%
INCLUDE_ASM("sound/soundsys", func_00296E20);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_002AD5F0(void* p, int idx, int a2, float v);

extern "C" void func_00296E20(void* self, void* obj)
{
    if (*(int*)((char*)obj + 0x774) >= 0) {
        func_002AD5F0((char*)**(void***)((char*)func_0028B180() + 0x118) + 0x1D8, *(int*)((char*)obj + 0x774), 1, 0.75f);
        *(int*)((char*)obj + 0x774) = -1;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_00296E80);

//100%
INCLUDE_ASM("sound/soundsys", func_00297438);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_002AD5F0(void* p, int idx, int a2, float v);

extern "C" void func_00297438(void* self, void* obj)
{
    if (*(int*)((char*)obj + 0x778) >= 0) {
        func_002AD5F0((char*)**(void***)((char*)func_0028B180() + 0x118) + 0x1D8, *(int*)((char*)obj + 0x778), 1, 0.75f);
        *(int*)((char*)obj + 0x778) = -1;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002974A0);

INCLUDE_ASM("sound/soundsys", func_00297950);

//100%
INCLUDE_ASM("sound/soundsys", func_00297EB8);
#ifdef SKIP_ASM
extern "C" void func_002AD5F0(void* p, int idx, int a2, float v);

extern "C" void func_00297EB8(void* self, int a1, int a2)
{
    for (int i = 0; i < 30; i++) {
        if (*(int*)((char*)self + i * 0x30 + 0x5A00) == 1 &&
            *(int*)((char*)self + i * 0x30 + 0x5A04) == a1 &&
            *(int*)((char*)self + i * 0x30 + 0x5A20) == a2) {
            func_002AD5F0(**(char***)((char*)self + 0x118) + 0x1D8,
                          *(int*)((char*)self + i * 0x30 + 0x5A24), 1, 0.0f);
            *(int*)((char*)self + i * 0x30 + 0x5A00) = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_00297F70);
#ifdef SKIP_ASM
extern "C" void func_00297F70(void* self)
{
    int i;
    for (i = 29; i >= 0; i--) {
        *(int*)((char*)self + i * 0x30 + 0x5a00) = 0;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_00297FA0);

//100%
INCLUDE_ASM("sound/soundsys", func_002980B0);
#ifdef SKIP_ASM
extern "C" void func_002AD5F0(void* p, int idx, int a2, float v);

extern "C" void func_002980B0(void* self)
{
    int i;
    for (i = 0; i < 30; i++) {
        if (*(int*)((char*)self + i * 0x30 + 0x5A00) == 1) {
            func_002AD5F0((char*)**(void***)((char*)self + 0x118) + 0x1D8, *(int*)((char*)self + i * 0x30 + 0x5A24), 1, 0.0f);
            *(int*)((char*)self + i * 0x30 + 0x5A00) = 0;
        }
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_00298138);

INCLUDE_ASM("sound/soundsys", func_00298488);

INCLUDE_ASM("sound/soundsys", func_002989A8);

//100%
INCLUDE_ASM("sound/soundsys", func_00298D00);
#ifdef SKIP_ASM
struct sSndKey8 {
    long v;
} __attribute__((packed));
extern sSndKey8 D_004A3678[];
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" void func_002ADCA0(void*, int, long, int, int, int, int, int);

// PORT: 64-bit long sound key
extern "C" void func_00298D00(void* self, void* obj)
{
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        char* p = **(char***)((char*)self + 0x118) + 0x1D8;
        sSndKey8* key = D_004A3678;
        int r = (int)func_00287968(self, 9, 0);
        func_002ADCA0(p, 0xFA, key->v, r, 0, 0x72, 0x7F, 0);
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_00298D90);

//100%
INCLUDE_ASM("sound/soundsys", func_002992D8);
#ifdef SKIP_ASM
extern "C" int func_00288AE0(void* self);
extern "C" void func_002AD5F0(void* p, int idx, int a2, float v);

extern "C" void func_002992D8(void* self, void* obj)
{
    if (func_00288AE0(self) != 0) {
        return;
    }
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        int n = --*(int*)((char*)self + 0x59E4);
        if (n <= 0) {
            if (n == 0) {
                func_002AD5F0((char*)**(void***)((char*)self + 0x118) + 0x1D8, *(int*)((char*)self + 0x59E0), 1, 0.0f);
            }
            *(int*)((char*)self + 0x59E4) = 0;
        }
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_00299368);

INCLUDE_ASM("sound/soundsys", func_00299638);

INCLUDE_ASM("sound/soundsys", func_002997B8);

INCLUDE_ASM("sound/soundsys", func_00299B70);

INCLUDE_ASM("sound/soundsys", func_00299E28);

INCLUDE_ASM("sound/soundsys", func_0029A220);

INCLUDE_ASM("sound/soundsys", func_0029A530);

INCLUDE_ASM("sound/soundsys", func_0029A7D8);

//100%
INCLUDE_ASM("sound/soundsys", func_0029AB08);
#ifdef SKIP_ASM
extern "C" void func_0029AB08(void* self, void* obj)
{
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        *(short*)((char*)self + (*(int*)((char*)obj + 0x870) << 1) + 0x5FE8) = 0x1000;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029AB40);

INCLUDE_ASM("sound/soundsys", func_0029B0E0);

//100%
INCLUDE_ASM("sound/soundsys", func_0029B3C0);
#ifdef SKIP_ASM
extern "C" void func_002AD5F0(void* p, int idx, int a2, float v);

extern "C" void func_0029B3C0(void* self, void* obj)
{
    if (obj != 0) {
        int ok = 0;
        if (*(int*)((char*)obj + 0x874) != 0) {
            ok = *(int*)((char*)obj + 0x87C) != 0;
        }
        if (!ok) {
            return;
        }
    }
    if (*(int*)((char*)self + 0x5FDC) != -1) {
        func_002AD5F0(**(char***)((char*)self + 0x118) + 0x1D8, *(int*)((char*)self + 0x5FDC), 1, 0.0f);
        *(int*)((char*)self + 0x5FDC) = -1;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029B430);

//100%
INCLUDE_ASM("sound/soundsys", func_0029B738);
#ifdef SKIP_ASM
extern "C" int func_00288AE0(void* self);
extern "C" int func_00285D98(void* self, int which);
extern "C" void* func_0028B1C8();
extern "C" void func_002A3DE0(void* self, void* obj, int ev);

extern "C" void func_0029B738(void* self)
{
    if (func_00288AE0(self) != 0) return;
    // PORT: func_00285D98 returns a rider pointer as int
    char* r = (char*)func_00285D98(self, -1);
    if (*(int*)(r + 0x870) == *(int*)((char*)self + 0x5820)) {
        if (*(int*)(r + 0x2F4) == 10 && *(int*)((char*)self + 0x581C) != *(int*)(r + 0x2F4) &&
            *(int*)((char*)func_0028B1C8() + 0x214) == 4) {
            func_002A3DE0(self, r, 2);
        }
    } else {
        *(int*)((char*)self + 0x5820) = *(int*)(r + 0x870);
    }
    *(int*)((char*)self + 0x581C) = *(int*)(r + 0x2F4);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029B7E0);
#ifdef SKIP_ASM
extern "C" void func_002A3DE0(void* self, void* obj, int ev);

extern "C" void func_0029B7E0(void* self, void* obj)
{
    int ok = 0;
    if (*(int*)((char*)obj + 0x874) != 0) {
        ok = *(int*)((char*)obj + 0x87C) != 0;
    }
    if (ok) {
        func_002A3DE0(self, obj, 8);
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029B818);

//100%
INCLUDE_ASM("sound/soundsys", func_0029B960__FPv);
#ifdef SKIP_ASM
void func_0029B960(void* self)
{
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029B968);

INCLUDE_ASM("sound/soundsys", func_0029BCF8);

INCLUDE_ASM("sound/soundsys", func_0029C088);

//100%
INCLUDE_ASM("sound/soundsys", func_0029C418__FPv);
#ifdef SKIP_ASM
void func_0029C418(void* self)
{
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029C420);

INCLUDE_ASM("sound/soundsys", func_0029C7B0);

INCLUDE_ASM("sound/soundsys", func_0029CCA8);

//100%
INCLUDE_ASM("sound/soundsys", func_0029CE28);
#ifdef SKIP_ASM
extern "C" void func_002AD2A8(void*);

extern "C" void func_0029CE28(void* self)
{
    if (*(int*)((char*)self + 0x5FB0) == 0) {
        *(int*)((char*)self + 0x5FB0) = 1;
        *(int*)((char*)**(void***)((char*)self + 0x118) + 0x26C) = 1;
        func_002AD2A8((char*)**(void***)((char*)self + 0x118) + 0x1D8);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029CE70);
#ifdef SKIP_ASM
extern "C" void func_002AD300(void* self);
extern "C" void func_002AD810(void* self);
extern "C" void func_002B6A68(void* self);
extern void* D_004A52D4;

extern "C" void func_0029CE70(void* self)
{
    if (*(int*)((char*)self + 0x5FB0) != 0) {
        *(int*)((char*)self + 0x5FB0) = 0;
        func_002AD300(**(char***)((char*)self + 0x118) + 0x1D8);
        *(int*)(**(char***)((char*)self + 0x118) + 0x26C) = 0;
        func_002AD810(**(char***)((char*)self + 0x118) + 0x1D8);
        func_002B6A68(D_004A52D4);
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029CED8);

//100%
INCLUDE_ASM("sound/soundsys", func_0029D290);
#ifdef SKIP_ASM
extern "C" void* func_0028B1D8();
extern "C" void func_0029D370(void* self);
extern "C" void func_0029D678(void* self, float v);
extern "C" void func_002AD650(void* self, int id, float v);

static inline int sndIsOn(char* t)
{
    return *(int*)t == 1;
}

extern "C" void func_0029D290(void* self)
{
    char* s = *(char**)((char*)func_0028B1D8() + 0x28);
    int v = *(int*)(s + 0x430);
    int ok = v != -1;
    if (ok) {
        if ((v & 0xFF) != *(unsigned char*)((char*)self + 0x5FC0)) {
            *(int*)((char*)self + 0x5FC0) = v;
            if (*(int*)(s + 0x434) < 0x16) {
                func_0029D370(self);
            } else {
                func_0029D678(self, 5.029983997344971f);

                char* o = **(char***)((char*)self + 0x118);
                char* bm = o + 0x1D8;
                char* e = *(char**)(o + 0xACC);
                int id = sndIsOn(e + 0x300);
                if (id) id = *(int*)(e + 0x304); else id = -1;
                func_002AD650(bm, id, 5.029983997344971f);

                char* o2 = **(char***)((char*)self + 0x118);
                char* bm2 = o2 + 0x1D8;
                char* e2 = *(char**)(o2 + 0xACC);
                int id2 = sndIsOn(e2 + 0x360);
                if (id2) id2 = *(int*)(e2 + 0x364); else id2 = -1;
                func_002AD650(bm2, id2, 5.029983997344971f);
            }
        }
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029D370);

//100%
INCLUDE_ASM("sound/soundsys", func_0029D610);
#ifdef SKIP_ASM
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" int func_003B8AB8(int h);
extern "C" void func_0029D678(void* self, float v);
extern "C" void func_0029D370(void* self);

extern "C" void func_0029D610(void* self)
{
    if (*(int*)((char*)self + 0x5FC4) != 0) {
        func_003B58A0();
        int done = func_003B8AB8(*(int*)((char*)self + 0x5FC8)) != 0;
        func_003B58D8();
        if (done) {
            func_0029D678(self, 0.0f);
            func_0029D370(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029D678);
#ifdef SKIP_ASM
extern "C" void func_002AD5F0(void* p, int idx, int a2, float v);

extern "C" void func_0029D678(void* self, float v)
{
    if (*(int*)((char*)self + 0x5FC4) != 0) {
        int idx = *(int*)((char*)self + 0x5FC8);
        if (idx >= 0) {
            func_002AD5F0((char*)**(void***)((char*)self + 0x118) + 0x1D8, idx, 1, v);
            *(int*)((char*)self + 0x5FC8) = -1;
        }
        *(int*)((char*)self + 0x5FC4) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029D6D0__FPv);
#ifdef SKIP_ASM
void func_0029D6D0(void* self)
{
    *(int*)((char*)self + 0x5FD8) = 1;
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029D6E0);

INCLUDE_ASM("sound/soundsys", func_0029D8E0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_0029DBB0);
#ifdef SKIP_ASM
extern "C" void func_00294F78(void* self, int id);
extern "C" void func_00296E20(void* self, void* obj);
extern "C" void func_00297438(void* self, void* obj);
extern "C" void func_0029B3C0(void* self, void* obj);

struct sSndVtEnt { short delta; short index; void (*fn)(void*, int); };

extern "C" void func_0029DBB0(void* self, int keep)
{
    if (*(int*)((char*)self + 0x5FD4) != 0) {
        if (keep == 0) {
            func_00294F78(self, 0xE);
        }
        func_00296E20(self, *(void**)((char*)func_0028B1D8() + 0x28));
        func_00297438(self, *(void**)((char*)func_0028B1D8() + 0x28));
        func_0029B3C0(self, 0);
        char* o = (char*)self + 0x118;
        sSndVtEnt* e = &(*(sSndVtEnt**)((char*)self + 0x5558))[4];
        e->fn(o + e->delta, 0x27);
        *(int*)((char*)self + 0x5FD0) = 0;
        *(int*)((char*)self + 0x5FD4) = 0;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029DC48);

INCLUDE_ASM("sound/soundsys", func_0029DEF0);

INCLUDE_ASM("sound/soundsys", func_0029E438);

//100%
INCLUDE_ASM("sound/soundsys", func_0029E560__FPv);
#ifdef SKIP_ASM
void func_0029E560(void* self)
{
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029E578);
#ifdef SKIP_ASM
extern "C" void func_0029E578(void* self, void* a1)
{
    int idx = *(int*)((char*)a1 + 0x870);
    self = (char*)self + idx * 4;
    *(int*)((char*)self + 0x6080) = 0;
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029E590);

INCLUDE_ASM("sound/soundsys", func_0029E970);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_0029ED18);
#ifdef SKIP_ASM
extern "C" void* func_0028B1D8();
extern "C" void func_0029B3C0(void* self, void* obj);
extern "C" void func_002AD5F0(void* p, int idx, int a2, float v);

extern "C" void func_0029ED18(void* self)
{
    func_00296E20(self, *(void**)((char*)func_0028B1D8() + 0x28));
    func_00297438(self, *(void**)((char*)func_0028B1D8() + 0x28));
    func_0029B3C0(self, 0);
    if (*(int*)((char*)self + 0x59E4) > 0) {
        func_002AD5F0(**(char***)((char*)self + 0x118) + 0x1D8, *(int*)((char*)self + 0x59E0), 1, 0.0f);
        *(int*)((char*)self + 0x59E4) = 0;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029ED90);

INCLUDE_ASM("sound/soundsys", func_0029EEE0);

//100%
INCLUDE_ASM("sound/soundsys", func_0029F000);
#ifdef SKIP_ASM
extern "C" void func_00287F00(void* self, int a1, float f0, float f1);

extern "C" void* func_0029F000(void* self, int a1, int type, int a3)
{
    if (a1 == 0) {
        if (type == 2) {
            func_00287F00(self, 0, 0.9999024868011475f, 0.6499993801116943f);
        }
        if (type == 0xB) {
            return (char*)self + 0x5730;
        }
        return func_00287968(self, type, a3);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F088);
#ifdef SKIP_ASM
extern "C" int func_0029F088(void* self, int a1)
{
    if (a1 == 0) {
        return *(int*)((char*)self + 0x5738);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F0A0);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int ch);

extern "C" int func_0029F0A0(void* self)
{
    if (func_00287920(self, 4) == 0.0f) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F0D8);
#ifdef SKIP_ASM
extern "C" int func_00288AE0(void* self);
extern "C" float func_00287920(void* self, int ch);

extern "C" int func_0029F0D8(void* self)
{
    if (func_00288AE0(self) != 0) {
        return 0;
    }
    if (func_00287920(self, 2) == 0.0f) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F128);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int ch);

extern "C" int func_0029F128(void* self)
{
    if (func_00287920(self, 3) == 0.0f) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F160);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int ch);

extern "C" int func_0029F160(void* self)
{
    if (func_00287920(self, 0xA) == 0.0f) {
        return 0;
    }
    return 1;
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029F198);

INCLUDE_ASM("sound/soundsys", func_0029F2B0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_0029F330);
#ifdef SKIP_ASM
extern "C" int func_00123128(void*);
extern "C" int func_00123168(void*);
extern "C" void func_0029F2B0(int, int);

extern "C" void func_0029F330(void* self)
{
    int a = func_00123128(self);
    func_0029F2B0(a, func_00123168(self));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F378);
#ifdef SKIP_ASM
extern "C" int func_0029F378(void* self, void* obj)
{
    switch (func_00123168(obj)) {
    case 0:
        return func_00123128(obj);
    case 14:
    case 16:
    case 21:
        return 6;
    default:
        return 7;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029F3F8);

//100%
INCLUDE_ASM("sound/soundsys", func_0029F5E0);
#ifdef SKIP_ASM
extern "C" void func_0028BCE8(void* self, int i);

extern "C" void func_0029F5E0(void* self)
{
    func_0028BCE8(**(void***)((char*)self + 0x118), 0xE);
    func_0028BCE8(**(void***)((char*)self + 0x118), 0xF);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F620);
#ifdef SKIP_ASM
struct sSndVEntry29F620 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_0029F620(void* self, void* obj)
{
    void* sub = (char*)obj + 0x6C0;
    sSndVEntry29F620* vt = *(sSndVEntry29F620**)sub;
    if (vt[7].fn((char*)sub + vt[7].delta) == 0) {
        return 0xE;
    }
    return 0xF;
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029F660);

INCLUDE_ASM("sound/soundsys", func_0029FCC8);

INCLUDE_ASM("sound/soundsys", func_0029FF80);

INCLUDE_ASM("sound/soundsys", func_002A02D8);

INCLUDE_ASM("sound/soundsys", func_002A0560);

INCLUDE_ASM("sound/soundsys", func_002A0A30);

INCLUDE_ASM("sound/soundsys", func_002A0E70);

//100%
INCLUDE_ASM("sound/soundsys", func_002A1028);
#ifdef SKIP_ASM
extern "C" int func_002B1220(void* self, int i);
extern "C" int func_002AB150(void* self, int i);
extern "C" void* func_002AD550(void* self, int idx, int a2);
extern "C" void func_002B11B0(void*, int);

extern "C" int func_002A1028(void* self, int id)
{
    int idx = func_002AB150(*(void**)((char*)self + 0x118), func_002B1220((char*)self + 0x5560, 0));
    if (idx < 0) {
        return 1;
    }
    void* voice = func_002AD550(**(char***)((char*)self + 0x118) + 0x1D8, idx, 3);
    if (voice == 0) {
        return 0;
    }
    if (*(int*)((char*)voice + 0x80) == id) {
        func_002B11B0((char*)self + 0x5560, 0);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A10C0);
#ifdef SKIP_ASM
extern "C" int func_002B1220(void* self, int i);
extern "C" int func_002AB150(void* self, int i);
extern "C" void* func_002AD550(void* self, int idx, int a2);

extern "C" int func_002A10C0(void* self, int id)
{
    int idx = func_002AB150(*(void**)((char*)self + 0x118), func_002B1220((char*)self + 0x5560, 0));
    if (idx < 0) {
        return 0;
    }
    void* voice = func_002AD550(**(char***)((char*)self + 0x118) + 0x1D8, idx, 3);
    if (voice == 0) {
        return 0;
    }
    return *(int*)((char*)voice + 0x80) == id;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A1138);

INCLUDE_ASM("sound/soundsys", func_002A1280);

//100%
INCLUDE_ASM("sound/soundsys", func_002A1388);
#ifdef SKIP_ASM
extern "C" void* func_0028B1D8();
extern "C" void func_002A1400(void* self, int a);

extern "C" int func_002A1388(void* self, void* obj)
{
    int i = *(int*)((char*)obj + 0x1C);
    int id;
    if (i >= 0 && i < *(int*)((char*)func_0028B1D8() + 0x78)) {
        id = *(int*)((char*)func_0028B1D8() + (i << 2) + 0x28);
    } else {
        id = *(int*)((char*)func_0028B1D8() + 0x28);
    }
    func_002A1400(self, id);
    return 1;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A1400);

INCLUDE_ASM("sound/soundsys", func_002A1560);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A16B0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_0029F0A0(void* self);
extern "C" int func_002B0E28(void* self, int i);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
// func_0029F2B0 returns a value (the unit's earlier declaration says void).
int func_0029F2B0_r(int a, int b) __asm__("func_0029F2B0");
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);
extern signed char D_00535C11[];

extern "C" void func_002A16B0(void* self, int a1)
{
    if (func_0029F0A0(self) != 0) {
        char* bm = (char*)self + 0x5560;
        if (func_002B0E28(bm, 0) != 0) {
            cBE_getInterface_Fv(cBE_getBE(), 0);
            if (D_00535C11[0] != 0) {
                if (func_002B1458(bm, 0, 0x20BC, 0, a1, 0, 1, 0.0f) != 0) {
                    int r = func_0029F2B0_r(a1, 0);
                    D_004A482C(func_003D8008(1, 0, 0x20BC), 1, r);
                }
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A1778);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0A0(void* self);
extern "C" int func_002B0E28(void* self, int i);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
// func_0029F2B0 returns a value (the unit's earlier declaration says void).
int func_0029F2B0_r(int a, int b) __asm__("func_0029F2B0");
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A1778(void* self, int a1)
{
    if (func_0029F0A0(self) != 0) {
        char* bm = (char*)self + 0x5560;
        if (func_002B0E28(bm, 0) != 0) {
            if (func_002B1458(bm, 0, 0x20BD, 0, a1, 0, 1, 0.0f) != 0) {
                int r = func_0029F2B0_r(a1, 0);
                D_004A482C(func_003D8008(1, 0, 0x20BD), 1, r);
            }
        }
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A1820);

INCLUDE_ASM("sound/soundsys", func_002A19D8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A1B58);
#ifdef SKIP_ASM
extern "C" void func_002A1400(void* self, int a);

extern "C" int func_002A1B58(void* self, int msg, int a)
{
    if (msg == 2) {
        func_002A1400(self, a);
        return 1;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A1B88);
#ifdef SKIP_ASM
extern "C" void func_002A1138(void* self, int a, int b);
extern "C" void func_002A1280(void* self, int a, int b);

extern "C" int func_002A1B88(void* self, int type, int a, int b)
{
    if (type == 0) {
        func_002A1138(self, a, b);
        return 1;
    }
    if (type == 1) {
        func_002A1280(self, a, b);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A1BD0__FPv);
#ifdef SKIP_ASM
int func_002A1BD0(void* self)
{
    return 0x1;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A1BD8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A1D48);
#ifdef SKIP_ASM
extern "C" int func_00123128(void*);
extern "C" int func_00123168(void*);
extern "C" void func_002A1DA0(void* self, int a, int b);

extern "C" void func_002A1D48(void* self, void* obj)
{
    int a = func_00123128(obj);
    func_002A1DA0(self, a, func_00123168(obj));
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A1DA0);

INCLUDE_ASM("sound/soundsys", func_002A1E20);

INCLUDE_ASM("sound/soundsys", func_002A1E68);

//100%
INCLUDE_ASM("sound/soundsys", func_002A2018);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2018(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20C8, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1E20(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)));
            D_004A482C_4(func_003D8008(1, 0, 0x20C8), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A20D0);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A20D0(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20C9, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1E20(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)));
            D_004A482C_4(func_003D8008(1, 0, 0x20C9), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2188);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1D8();
// func_002A1D48 returns its tail call's value (the unit defines it as void).
int func_002A1D48_r(void* self, void* obj) __asm__("func_002A1D48");
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2188(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20CA, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int a = func_002A1D48_r(self, *(void**)((char*)func_0028B1D8() + 0x28));
            int m = func_002A1E20(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)));
            D_004A482C_5(func_003D8008(1, 0, 0x20CA), 3, r, a, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2260);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2260(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20E7, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1E20(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)));
            D_004A482C_4(func_003D8008(1, 0, 0x20E7), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2318);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2318(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20E8, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1E20(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)));
            D_004A482C_4(func_003D8008(1, 0, 0x20E8), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A23D0);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A23D0(void* self, int mode)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20E9, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m;
            if (mode == -1) {
                m = func_002A1E20(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)));
            } else {
                m = func_002A1E20(self, mode);
            }
            D_004A482C_4(func_003D8008(1, 0, 0x20E9), 2, r, m);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A24B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A24B0(void* self, int a1)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20CC, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1BD8(self, a1, -1);
            D_004A482C_5(func_003D8008(1, 0, 0x20CC), 3, r, m, 3);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A2568);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2568(void* self, int a1)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20CC, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1BD8(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)), a1);
            D_004A482C_5(func_003D8008(1, 0, 0x20CC), 3, r, m, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2638);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2638(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20BA, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1E20(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)));
            D_004A482C_4(func_003D8008(1, 0, 0x20BA), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A26F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0D8(void* self);
extern "C" void func_002B11B0(void*, int);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_003D8008(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A26F0(void* self, int a, int b)
{
    if (func_0029F0D8(self) != 0) {
        char* bm = (char*)self + 0x5560;
        func_002B11B0(bm, 0);
        if (func_002B1458(bm, 0, 0x20E5, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int x = 2;
            if (a) x = 1;
            if (*(int*)((char*)self + 0x5794) != 0) {
                *(int*)((char*)self + 0x5794) = 0;
            }
            int y = 2;
            if (!b) y = 1;
            D_004A482C_5(func_003D8008(1, 0, 0x20E5), 3, r, x, y);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A27D8);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A27D8(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20E6, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            D_004A482C(func_003D8008(1, 0, 0x20E6), 1, r);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2860);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_003D8008(int a, int b, int c);
// 6-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_6)(void*, int, int, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2860(void* self, int n)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20CF, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int lo = 0;
            int hi = 0;
            if (n >= 0 && n != 999) {
                if (n < 100) {
                    lo = 1 << n;
                } else {
                    n -= 100;
                    hi = 1 << n;
                }
            }
            D_004A482C_6(func_003D8008(1, 0, 0x20CF), 4, r, 3, lo, hi);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2938);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1D8();
// func_002A1D48 returns its tail call's value (the unit defines it as void).
int func_002A1D48_r(void* self, void* obj) __asm__("func_002A1D48");
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 5-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_5)(void*, int, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2938(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x2102, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int a = func_002A1D48_r(self, *(void**)((char*)func_0028B1D8() + 0x28));
            int m = func_002A1E20(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)));
            D_004A482C_5(func_003D8008(1, 0, 0x2102), 3, r, a, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2A10);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00145750(void*);
// func_002A1DA0 returns a value (the unit declares it void).
int func_002A1DA0_r(void* self, int a, int b) __asm__("func_002A1DA0");
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2A10(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x210B, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1DA0_r(self, func_00145750(cBE_getInterface_Fv(func_0028B1E8(), 0)), 0);
            D_004A482C_4(func_003D8008(1, 0, 0x210B), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2AD0);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1E20(void* self, unsigned int a1);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2AD0(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x212B, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1E20(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)));
            D_004A482C_4(func_003D8008(1, 0, 0x212B), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A2B88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2B88(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x212C, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int v = *(int*)((char*)self + 0x5780);
            D_004A482C_4(func_003D8008(1, 0, 0x212C), 2, r, v);
        }
        *(int*)((char*)self + 0x5780) = 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A2C30);
#ifdef SKIP_ASM
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2C30(void* self)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x212D, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1BD8(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)), -1);
            D_004A482C_4(func_003D8008(1, 0, 0x212D), 2, r, m);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A2CF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2CF0(void* self, int a1)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x212E, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1BD8(self, a1, -1);
            D_004A482C_4(func_003D8008(1, 0, 0x212E), 2, r, m);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A2DA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F0D8(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A2DA0(void* self, int a1)
{
    if (func_0029F0D8(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x212F, 0, 0xA, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1BD8(self, a1, -1);
            D_004A482C_4(func_003D8008(1, 0, 0x212F), 2, r, m);
        }
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A2E50);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A3170);
#ifdef SKIP_ASM
extern "C" void func_002A26F0(void* self, int a, int b);
void func_002B1758(void* self, int a1);
int func_002B4908(void* self);

extern "C" void func_002A3170(void* self)
{
    func_002A26F0(self, 0, 0);
    func_002B1758((char*)self + 0x5560, 0);
    *(int*)((char*)self + 0x5774) = 1;
    *(int*)((char*)self + 0x5778) = func_002B4908((char*)self + 0x118);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A31C0);
#ifdef SKIP_ASM
extern "C" int func_0029F128(void* self);
extern "C" int func_002A4040(void* self);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C11[];
extern char* D_004A28A8;
extern "C" int func_00295028(void* self, int a, int b);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A31C0(void* self)
{
    if (func_0029F128(self) == 0) return;
    if (func_002A4040(self) != 0) return;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] != 0) return;
    if (**(int**)(D_004A28A8 + 0xC0) < 2) {
        if (func_00295028(self, 0, 0) != 0) return;
        if (func_002B1458((char*)self + 0x5560, 0, 0x20C0, 0, 0xB, 0, 0, 0.0f) == 0) return;
        int r = func_0029F198(self);
        D_004A482C_4(func_003D8008(1, 0, 0x20C0), 2, r, 1);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A32B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F128(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1D8();
// func_002A1D48 returns its tail call's value (the unit defines it as void).
int func_002A1D48_r(void* self, void* obj) __asm__("func_002A1D48");
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A32B0(void* self)
{
    if (func_0029F128(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20C1, 0, 0xB, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int a = func_002A1D48_r(self, *(void**)((char*)func_0028B1D8() + 0x28));
            D_004A482C_4(func_003D8008(1, 0, 0x20C1), 2, r, a);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3358);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F128(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1D8();
// func_002A1D48 returns its tail call's value (the unit defines it as void).
int func_002A1D48_r(void* self, void* obj) __asm__("func_002A1D48");
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A3358(void* self)
{
    if (func_0029F128(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20C2, 0, 0xB, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int a = func_002A1D48_r(self, *(void**)((char*)func_0028B1D8() + 0x28));
            D_004A482C_4(func_003D8008(1, 0, 0x20C2), 2, r, a);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A3400);
#ifdef SKIP_ASM
extern "C" int func_0029F128(void* self);
extern "C" int func_00288AE0(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A3400(void* self)
{
    if (func_0029F128(self) == 0) return;
    if (func_00288AE0(self) != 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 0x20C3, 0, 0xB, 0, 0, 0.0f) == 0) return;
    int r = func_0029F198(self);
    int m = func_002A1BD8(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)), -1);
    D_004A482C_4(func_003D8008(1, 0, 0x20C3), 2, r, m);
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A34D0);

INCLUDE_ASM("sound/soundsys", func_002A3708);

INCLUDE_ASM("sound/soundsys", func_002A3860);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A39E0);
#ifdef SKIP_ASM
extern "C" int func_0029F128(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" int func_0029F198(void* self);
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A1BD8(void* self, int a, int b);
extern "C" void* func_003D8008(int a, int b, int c);
// 4-argument view of the D_004A482C callback (the unit declares a 3-argument one).
extern void (*D_004A482C_4)(void*, int, int, int) __asm__("D_004A482C");

extern "C" void func_002A39E0(void* self)
{
    if (func_0029F128(self) != 0) {
        if (func_002B1458((char*)self + 0x5560, 0, 0x20BB, 0, 0xB, 0, 0, 0.0f) != 0) {
            int r = func_0029F198(self);
            int m = func_002A1BD8(self, *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0)), -1);
            D_004A482C_4(func_003D8008(1, 0, 0x20BB), 2, r, m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3B18);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F160(void* self);
// func_00285D98 returns an object pointer here (the unit declares it int).
void* func_00285D98_p(void* self, int which) __asm__("func_00285D98");
extern "C" int func_00288AE0(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A3B18(void* self, void* obj, int a2)
{
    if (func_0029F160(self) == 0) return;
    if (obj == 0) return;
    int done = *(float*)((char*)obj + 0x470) >= 0.0f;
    if (done) return;
    if (obj != func_00285D98_p(self, -1)) return;
    if (func_00288AE0(self) != 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 0x20A7, 0, 0xC, 0, 0, 0.0f) != 0) {
        D_004A482C(func_003D8008(1, 0, 0x20A7), 1, a2);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3C00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_0029F160(void* self);
// func_00285D98 returns an object pointer here (the unit declares it int).
void* func_00285D98_p(void* self, int which) __asm__("func_00285D98");
extern "C" int func_00288AE0(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A3C00(void* self, void* obj, int a2)
{
    if (func_0029F160(self) == 0) return;
    if (obj == 0) return;
    if (a2 != 2) {
        int done = *(float*)((char*)obj + 0x470) >= 0.0f;
        if (done) return;
    }
    if (obj != func_00285D98_p(self, -1)) return;
    if (func_00288AE0(self) != 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 0x20A8, 0, 0xC, 0, 0, 0.0f) != 0) {
        D_004A482C(func_003D8008(1, 0, 0x20A8), 1, a2);
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A3CE8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A3DE0);
#ifdef SKIP_ASM
extern "C" int func_0029F160(void* self);
extern "C" int func_00288AE0(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A3DE0(void* self, void* obj, int ev)
{
    if (func_0029F160(self) == 0) return;
    if (obj == 0) return;
    int done = *(float*)((char*)obj + 0x470) >= 0.0f;
    if (done) return;
    if (func_00288AE0(self) != 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 0x2133, 0, 0xC, 0, 0, 0.0f) != 0) {
        D_004A482C(func_003D8008(1, 0, 0x2133), 1, ev);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3EB8);
#ifdef SKIP_ASM
extern "C" int func_0029F160(void* self);
extern "C" int func_00288AE0(void* self);
extern "C" int func_002B1458(void* self, int a, int b, int c, int d, int e, int refresh, float f);
extern "C" void* func_003D8008(int a, int b, int c);
extern void (*D_004A482C)(void*, int, int);

extern "C" void func_002A3EB8(void* self, void* obj, int ev)
{
    if (func_0029F160(self) == 0) return;
    if (obj == 0) return;
    int done = *(float*)((char*)obj + 0x470) >= 0.0f;
    if (done) return;
    if (func_00288AE0(self) != 0) return;
    if (func_002B1458((char*)self + 0x5560, 0, 0x2145, 0, 0xC, 0, 0, 0.0f) != 0) {
        D_004A482C(func_003D8008(1, 0, 0x2145), 1, ev);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3F90);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A3FD8(void* self, unsigned int a1);

extern "C" int func_002A3F90(void* self)
{
    return func_002A3FD8(self, *func_00144BC0(cBE_getInterface_Fv(cBE_getBE(), 0)));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3FD8);
#ifdef SKIP_ASM
extern "C" int func_002A3FD8(void* self, unsigned int a1)
{
    return a1 - 0x11 < 5;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A3FE8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A4030(void* self, unsigned int a1);

extern "C" int func_002A3FE8(void* self)
{
    return func_002A4030(self, *func_00144BC0(cBE_getInterface_Fv(cBE_getBE(), 0)));
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4030);
#ifdef SKIP_ASM
extern "C" int func_002A4030(void* self, unsigned int a1)
{
    return a1 - 0xe < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4040);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C10[];

extern "C" int func_002A4040(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    return D_00535C10[0] == 4;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4078);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A40E0(void* self, unsigned int a1);
extern signed char D_00535C10[];

extern "C" int func_002A4078(void* self)
{
    if (func_002A40E0(self, *func_00144BC0(cBE_getInterface_Fv(cBE_getBE(), 0))) != 0) {
        return 1;
    }
    if (D_00535C10[0] == 0) {
        return 1;
    }
    return D_00535C10[0] == 5;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A40E0);
#ifdef SKIP_ASM
extern "C" int func_002A40E0(void* self, unsigned int a1)
{
    return a1 < 5;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A40E8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A4158(void* self, unsigned int a1);
extern signed char D_00535C10[];

extern "C" int func_002A40E8(void* self)
{
    if (func_002A4158(self, *func_00144BC0(cBE_getInterface_Fv(cBE_getBE(), 0))) != 0) {
        return 1;
    }
    if (D_00535C10[0] == 1) {
        return 1;
    }
    int r = 0;
    if (D_00535C10[0] == 6) {
        r = 1;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4158);
#ifdef SKIP_ASM
extern "C" int func_002A4158(void* self, unsigned int a1)
{
    return a1 - 5 < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4168);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A41C8(void* self, unsigned int a1);
extern signed char D_00535C10[];

extern "C" int func_002A4168(void* self)
{
    if (func_002A41C8(self, *func_00144BC0(cBE_getInterface_Fv(cBE_getBE(), 0))) != 0) {
        return 1;
    }
    return D_00535C10[0] == 2;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A41C8);
#ifdef SKIP_ASM
extern "C" int func_002A41C8(void* self, unsigned int a1)
{
    return a1 - 8 < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A41D8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A4238(void* self, unsigned int a1);
extern signed char D_00535C10[];

extern "C" int func_002A41D8(void* self)
{
    if (func_002A4238(self, *func_00144BC0(cBE_getInterface_Fv(cBE_getBE(), 0))) != 0) {
        return 1;
    }
    return D_00535C10[0] == 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4238);
#ifdef SKIP_ASM
extern "C" int func_002A4238(void* self, unsigned int a1)
{
    return a1 - 0xb < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4248);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C12[];

extern "C" int func_002A4248(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    switch (D_00535C12[0]) {
    case 4:
        return 1;
    case 5:
        return 1;
    default:
        return 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4290);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C12[];

extern "C" int func_002A4290(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    return D_00535C12[0] == 4;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A42C8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C12[];

extern "C" int func_002A42C8(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C12[0] >= 6) {
        if (D_00535C12[0] <= 11) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4318);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C12[];

extern "C" int func_002A4318(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C12[0] >= 6) {
        if (D_00535C12[0] <= 8) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4368);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C12[];

extern "C" int func_002A4368(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C12[0] >= 9) {
        if (D_00535C12[0] <= 11) {
            return 1;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A43B8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A4550);
#ifdef SKIP_ASM
extern "C" void func_002A4550(void* self)
{
    *(int*)((char*)self + 0x5740) = 0;
    *(int*)((char*)self + 0x5744) = 0;
    *(int*)((char*)self + 0x574C) = 0;
    *(int*)((char*)self + 0x5750) = 0;
    *(int*)((char*)self + 0x5754) = 0;
    *(int*)((char*)self + 0x5758) = 0;
    *(int*)((char*)self + 0x5760) = 0;
    *(int*)((char*)self + 0x5768) = 0;
    *(int*)((char*)self + 0x576C) = 0;
    *(int*)((char*)self + 0x5770) = 0;
    *(int*)((char*)self + 0x5774) = 0;
    *(int*)((char*)self + 0x577C) = 0;
    *(int*)((char*)self + 0x5784) = 0;
    *(int*)((char*)self + 0x5788) = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4590);
#ifdef SKIP_ASM
extern "C" void func_002A4590(void* self, int flag)
{
    *(int*)((char*)self + 0x57F8) = 0;
    *(int*)((char*)self + 0x57FC) = 0;
    *(int*)((char*)self + 0x5804) = 0;
    *(int*)((char*)self + 0x5808) = 0;
    *(int*)((char*)self + 0x5800) = -1;
    *(int*)((char*)self + 0x5810) = -1;
    if (flag) {
        *(int*)((char*)self + 0x580C) = -1;
    }
    *(int*)((char*)self + 0x5814) = 0x17;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A45C0);
#ifdef SKIP_ASM
extern "C" void func_002A4590(void* self, int flag);
extern "C" void* func_0028B1D8();
extern "C" int func_002A4078(void* self);
extern "C" void func_002A4660(void* self, int id, int a, int b, int c);
extern char* D_004A28A8;
extern int D_00536730[];

extern "C" void func_002A45C0(void* self)
{
    if (**(int**)(D_004A28A8 + 0xC0) == 1) {
        func_002A4590(self, 0);
    }
    int id = D_00536730[0];
    char* rider = *(char**)((char*)func_0028B1D8() + 0x28);
    int a;
    if (func_002A4078(self) != 0) {
        a = *(int*)(*(char**)(rider + 0x790) + 0x128);
    } else {
        a = 0;
    }
    func_002A4660(self, id, a, *(int*)(*(char**)(rider + 0x790) + 0x114), *(int*)(rider + 0x480) ^ 1);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4660);
#ifdef SKIP_ASM
extern char* D_004A28A8;
extern "C" void* func_0028B1E8();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);

extern "C" void func_002A4660(void* self, int a1, int a2, int a3, int a4)
{
    char* g = *(char**)(D_004A28A8 + 0xC0);
    if (*(int*)(g + 0x98) != 0 || *(int*)(g + 0x70) <= *(int*)g) {
        *(int*)((char*)self + 0x57F8) = 1;
        if (a4 != 0 && *(int*)g == 3) {
            *(int*)((char*)self + 0x57FC) = 1;
        }
        *(int*)((char*)self + 0x5800) = a1;
        *(int*)((char*)self + 0x5810) = *func_00144BC0(cBE_getInterface_Fv(func_0028B1E8(), 0));
    }
    *(int*)((char*)self + 0x5804) += a2;
    *(int*)((char*)self + 0x5808) += a3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4718);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);

extern "C" int func_002A4718(void* self)
{
    int id = *func_00144BC0(cBE_getInterface_Fv(cBE_getBE(), 0));
    int cur = *(int*)((char*)self + 0x5814);
    if (id == cur || cur == 0x17) {
        return 0;
    }
    return 1;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A4770);

INCLUDE_ASM("sound/soundsys", func_002A49E8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A4A38);
#ifdef SKIP_ASM
extern "C" int func_002A10C0(void* self, int id);

extern "C" int func_002A4A38(void* self)
{
    if (*(int*)((char*)self + 0x5818) != 0) {
        return func_002A10C0(self, 10) != 0;
    }
    return 0;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A4A78);

//100%
INCLUDE_ASM("sound/soundsys", func_002A4B68);
#ifdef SKIP_ASM
extern "C" int func_002B49E0(void*);
extern "C" void func_002B11B0(void*, int);

extern "C" void func_002A4B68(void* self)
{
    if (func_002B49E0((char*)self + 0x118) == 0x191) {
        func_002B11B0((char*)self + 0x5560, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4BA8);
#ifdef SKIP_ASM
extern "C" void* func_002A4BA8(void* self)
{
    int i;
    *(int*)self = 0;
    for (i = 4; i >= 0; i--) {
        *(int*)((char*)self + (i << 2) + 4) = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4BE0);
#ifdef SKIP_ASM
extern "C" void func_002A4CF8(void*);
void operator_delete(int*);

extern "C" void func_002A4BE0(void* self, int flags)
{
    func_002A4CF8(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4C28);
#ifdef SKIP_ASM
struct sSndHandleList5 {
    int count;
    int items[5];
};
extern "C" int func_003E1908(int a, int b);

extern "C" int func_002A4C28(sSndHandleList5* list, int a, int b)
{
    if (list->count >= 5 || (list->items[list->count] = func_003E1908(a, b), list->items[list->count] == 0)) {
        return 0;
    }
    list->count++;
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4CA0);
#ifdef SKIP_ASM
unsigned int BXrand();

struct sSndRandList {
    unsigned int count;
    int items[1];
};

extern "C" int func_002A4CA0(sSndRandList* list)
{
    if (list->count == 0) {
        return 0;
    }
    return list->items[BXrand() % list->count];
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4CF8);
#ifdef SKIP_ASM
struct sSndPtrList {
    int count;
    void* items[5];
};
void cMemMan_free(void*);

extern "C" void func_002A4CF8(void* self)
{
    sSndPtrList* list = (sSndPtrList*)self;
    for (int i = 0; i < list->count; i++) {
        if (list->items[i] != 0) {
            cMemMan_free(list->items[i]);
        }
        list->items[i] = 0;
    }
    list->count = 0;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A4D68);

//100%
INCLUDE_ASM("sound/soundsys", func_002A4E38);
#ifdef SKIP_ASM
extern "C" void func_002A6848(void* self);
extern "C" void func_002A6430(void* self, float v);
void operator_delete(int*);

extern "C" void func_002A4E38(void* self, int flags)
{
    func_002A6848(self);
    func_002A6430(self, 0.0f);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A4E88);

//100%
INCLUDE_ASM("sound/soundsys", func_002A5CD8);
#ifdef SKIP_ASM
struct s2A5CD8Entry {
    int active;
    int value;
    int pad;
};

struct s2A5CD8 {
    char pad[0x20];
    s2A5CD8Entry entries[1];
};

extern "C" void func_002A5CD8(s2A5CD8* self, int i, int v)
{
    if (i >= 0) {
        if (self->entries[i].active) {
            self->entries[i].value = v;
        }
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A5D08);

INCLUDE_ASM("sound/soundsys", func_002A62F0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A6430);
#ifdef SKIP_ASM
extern "C" void func_002A62F0(void* voice, int b, float v);

extern "C" void func_002A6430(void* self, float v)
{
    for (int i = 0; i < 15; i++) {
        if (*(int*)((char*)self + 0x20 + i * 0xC) != 0) {
            func_002A62F0(self, i, v);
        }
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A64A0);

INCLUDE_ASM("sound/soundsys", func_002A6648);

//100%
INCLUDE_ASM("sound/soundsys", func_002A67F0);
#ifdef SKIP_ASM
extern "C" void func_002A67F0(void* self)
{
    if (*(int*)((char*)self + 0x180) == 0) {
        *(int*)((char*)self + 0x180) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A6808);
#ifdef SKIP_ASM
extern "C" void func_002A6848(void* self);

extern "C" void func_002A6808(void* self)
{
    if (*(int*)((char*)self + 0x180) == 2) {
        func_002A6848(self);
    }
    *(int*)((char*)self + 0x180) = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A6848);
#ifdef SKIP_ASM
extern "C" void func_002AD5F0(void* p, int idx, int a2, float v);
extern char** D_004A36F8;

extern "C" void func_002A6848(void* self)
{
    float lim = 0.25f;
    if (lim < *(float*)((char*)self + 0x188)) {
        *(float*)((char*)self + 0x188) = lim;
    }
    *(int*)((char*)self + 0x180) = 0;
    if (*(int*)((char*)self + 0x174) >= 0) {
        func_002AD5F0(*D_004A36F8 + 0x1D8, *(int*)((char*)self + 0x174), 2, 0.0f);
        *(int*)((char*)self + 0x174) = -1;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A69A0);

INCLUDE_ASM("sound/soundsys", func_002A6B50);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A6D18);
#ifdef SKIP_ASM
extern "C" void func_002A4E88(void* voice);

extern "C" void func_002A6D18(void* self)
{
    for (int i = 0; i < *(int*)((char*)self + 0x14); i++) {
        func_002A4E88((char*)*(void**)((char*)self + 0x18) + i * 0x190);
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A6D78);

INCLUDE_ASM("sound/soundsys", func_002A6F38);

INCLUDE_ASM("sound/soundsys", func_002A7040);

//100%
INCLUDE_ASM("sound/soundsys", func_002A72C0);
#ifdef SKIP_ASM
extern "C" void func_002A72C0(void* self, int a1, int* a2, int* a3)
{
    *a2 = a1 & 0xff;
    *a3 = a1 >> 8;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A72D8);
#ifdef SKIP_ASM
extern "C" void func_002A62F0(void* voice, int b, float v);

extern "C" void func_002A72D8(void* self, int id, float v)
{
    if (id >= 0 && *(void**)((char*)self + 0x18) != 0) {
        int a;
        int b;
        func_002A72C0(self, id, &a, &b);
        func_002A62F0((char*)*(void**)((char*)self + 0x18) + a * 0x190, b, v);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7340);
#ifdef SKIP_ASM
extern "C" void func_002A7340(void* self, int id, int v)
{
    if (id >= 0 && *(void**)((char*)self + 0x18) != 0) {
        int a;
        int b;
        func_002A72C0(self, id, &a, &b);
        func_002A5CD8((s2A5CD8*)((char*)*(void**)((char*)self + 0x18) + a * 0x190), b, v);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A73A8);
#ifdef SKIP_ASM
extern "C" void func_002A64A0(void* voice, int a1);

struct sSndVoice190 {
    char data[0x190];
};

extern "C" void func_002A73A8(void* self, int a1, int i)
{
    sSndVoice190* v = *(sSndVoice190**)((char*)self + 0x18);
    if (v != 0) {
        func_002A64A0(&v[i], a1);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A73D8);
#ifdef SKIP_ASM
extern "C" void func_002A6648(void* voice, int a1);

extern "C" void func_002A73D8(void* self, int a1, int i)
{
    sSndVoice190* v = *(sSndVoice190**)((char*)self + 0x18);
    if (v != 0) {
        func_002A6648(&v[i], a1);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7408);
#ifdef SKIP_ASM
extern "C" void func_002A67F0(void* voice);

extern "C" void func_002A7408(void* self, int i)
{
    sSndVoice190* v = *(sSndVoice190**)((char*)self + 0x18);
    if (v != 0) {
        func_002A67F0(&v[i]);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7438);
#ifdef SKIP_ASM
extern "C" void func_002A6808(void* voice);

extern "C" void func_002A7438(void* self, int i)
{
    sSndVoice190* v = *(sSndVoice190**)((char*)self + 0x18);
    if (v != 0) {
        func_002A6808(&v[i]);
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A7468);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7678);
#ifdef SKIP_ASM
extern "C" int func_002A5D08(void* voice, void* bank, int a1);

extern "C" int func_002A7678(void* self, int a1)
{
    sSndVoice190* v = *(sSndVoice190**)((char*)self + 0x18);
    if (v == 0) {
        return -1;
    }
    char* p = *(char**)self + 0x1D8;
    int r = func_002A5D08(&v[(*(int**)(p + 0x64))[*(int*)(p + 0x1C)]], p, a1);
    if (r >= 0) {
        char* q = *(char**)self + 0x1D8;
        r = (r << 8) | (*(int**)(q + 0x64))[*(int*)(q + 0x1C)];
    }
    (*(int*)(*(char**)self + 0x1F4))--;
    return r;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A7718);

//100%
INCLUDE_ASM("sound/soundsys", func_002A77C8);
#ifdef SKIP_ASM
struct s2A77C8Item {
    char pad[0x18C];
    int value;
};

extern "C" int func_002A77C8(void* self, int i)
{
    if (i < *(int*)((char*)self + 0x14)) {
        return (*(s2A77C8Item**)((char*)self + 0x18))[i].value;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A77F8);
#ifdef SKIP_ASM
extern "C" int func_00317F98(const char* name);
extern "C" int func_003E1B68(const char* name, void* buf, int size);
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

struct sSndFileBuf {
    int size;
    char* data;
    char* cur;
};

extern "C" sSndFileBuf* func_002A77F8(sSndFileBuf* self, const char* name, int flags)
{
    self->data = 0;
    self->cur = 0;
    self->size = func_00317F98(name);
    if (self->size != 0) {
        self->data = (char*)operator_new_tag(self->size + 1, name, flags, 0);
        func_003E1B68(name, self->data, self->size + 1);
        self->cur = self->data;
        self->data[self->size] = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A7890);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

extern "C" void func_002A7890(void* self, int flags)
{
    void* p = *(void**)((char*)self + 0x4);
    if (p != 0) {
        cMemMan_free(p);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A78E0__FPv);
#ifdef SKIP_ASM
signed char func_002A78E0(void* self)
{
    return *(*(signed char**)((char*)self + 0x8))++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A78F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char func_002A78E0(void* self);
extern "C" int func_002A79C8(void* self);

// func_002A78F8 returns int (its body leaves a result in v0); the unit declares it void.
int func_002A78F8_r(void*) __asm__("func_002A78F8");

int func_002A78F8_r(void* self)
{
    while (**(signed char**)((char*)self + 0x8) != 0) {
        if (func_002A79C8(self) == 1) return 1;
        signed char c = **(signed char**)((char*)self + 0x8);
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            int r = 0;
            if (c == '[') r = 1;
            return r;
        }
        func_002A78E0(self);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A79C8);
#ifdef SKIP_ASM
extern "C" int func_002A7A90(void* self);

extern "C" int func_002A79C8(void* self)
{
    signed char c = **(signed char**)((char*)self + 0x8);
    if (c != 0) {
        if (c == '#' || c == ';') {
            int r = func_002A7A90(self);
            if (r != 0) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7A20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_002A78F8 returns int (its body leaves a result in v0); the unit declares it void.
int func_002A78F8_r(void*) __asm__("func_002A78F8");
extern "C" char* func_002A7B08(void*);
extern "C" int func_002A7F90(void*);

extern "C" void func_002A7A20(void* self)
{
    func_002A78F8_r(self);
    while (**(signed char**)((char*)self + 0x8) == '[') {
        func_002A7B08(self);
        func_002A78F8_r(self);
    }
    func_002A7F90(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7A90);
#ifdef SKIP_ASM
extern "C" int func_002A7A90(void* self)
{
    while (**(signed char**)((char*)self + 0x8) != 0) {
        signed char c = **(signed char**)((char*)self + 0x8);
        if (c == '\r') {
            return 0;
        }
        if (c == '\n') {
            return 0;
        }
        func_002A78E0(self);
    }
    return 1;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A7B08);

//100%
INCLUDE_ASM("sound/soundsys", func_002A7BF8);
#ifdef SKIP_ASM
extern "C" char* func_002A7B08(void*);
extern "C" int func_0041AA88(const char* a, const char* b);

extern "C" int func_002A7BF8(void* self, const char* name)
{
    char* s;
    while (**(signed char**)((char*)self + 0x8) != 0 && (s = func_002A7B08(self)) != 0) {
        if (func_0041AA88(s, name) == 0) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A7C68);
#ifdef SKIP_ASM
extern "C" char* func_002A7B08(void*);
extern "C" char* func_0041AD80(const char* s, const char* sub);

extern "C" int func_002A7C68(void* self, char* name)
{
    char* tok;
    while (**(char**)((char*)self + 0x8) != 0 && (tok = func_002A7B08(self)) != 0) {
        if (func_0041AD80(name, tok) == name || func_0041AD80(tok, name) == tok) {
            return 1;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7CF0);
#ifdef SKIP_ASM
signed char func_002A78E0(void* self);
extern "C" void func_002A78F8(void*);
extern "C" int func_002A7EF0(void* self, const char* s);
extern const char D_004A3728[];

extern "C" char* func_002A7CF0(void* self)
{
    int n = 0;
    func_002A78F8(self);
    func_002A7EF0(self, D_004A3728);
    if (**(signed char**)((char*)self + 0x8) != '"') {
        char* dst = (char*)self + 0xC;
        do {
            dst[n] = func_002A78E0(self);
            n++;
        } while (**(signed char**)((char*)self + 0x8) != '"');
    }
    func_002A7EF0(self, D_004A3728);
    char* buf = (char*)self + 0xC;
    buf[n] = 0;
    return buf;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A7DA0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7E60);
#ifdef SKIP_ASM
extern "C" void func_002A78F8(void*);
extern "C" void func_002A7DA0(void*);
extern "C" int func_004178B0(const char* s, const char* fmt, ...);
extern char D_004A3730[];

extern "C" float func_002A7E60(void* self)
{
    float f;
    func_002A78F8(self);
    func_002A7DA0(self);
    func_004178B0((char*)self + 0xC, D_004A3730, &f);
    return f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7EA8);
#ifdef SKIP_ASM
extern "C" void func_002A78F8(void*);
extern "C" void func_002A7DA0(void*);
extern "C" int func_004178B0(const char* s, const char* fmt, ...);
extern char D_004A3738[];

extern "C" int func_002A7EA8(void* self)
{
    int v;
    func_002A78F8(self);
    func_002A7DA0(self);
    func_004178B0((char*)self + 0xC, D_004A3738, &v);
    return v;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7EF0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char func_002A78E0(void* self);
int func_002A78F8_r(void*) __asm__("func_002A78F8");

extern "C" int func_002A7EF0(void* self, const char* name)
{
    int i = 0;
    func_002A78F8_r(self);
    while (**(char**)((char*)self + 0x8) != 0 && name[i] != 0) {
        if (func_002A78E0(self) != name[i]) {
            return 1;
        }
        i++;
    }
    return name[i];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7F90);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int func_002A78F8_r(void*) __asm__("func_002A78F8");

extern "C" int func_002A7F90(void* self)
{
    if (*(char**)((char*)self + 0x8) - *(char**)((char*)self + 0x4) >= *(int*)((char*)self + 0x0)) {
        return 1;
    }
    func_002A78F8_r(self);
    signed char c = **(signed char**)((char*)self + 0x8);
    int r = 0;
    if (c == 0 || c == '[') {
        r = 1;
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7FF8);
#ifdef SKIP_ASM
extern "C" int func_0041AA88(const char* a, const char* b);
extern "C" int func_002A7EF0(void* self, const char* s);
extern "C" char* func_002A7CF0(void* self);
extern "C" char* strcpy(char* dst, const char* src);
extern char D_004A3740[];

extern "C" int func_002A7FF8(void* self, const char* name, char* out)
{
    if (func_0041AA88((char*)self + 0xC, name) != 0) {
        return 0;
    }
    if (func_002A7EF0(self, D_004A3740) != 0) {
        return 0;
    }
    char* s = func_002A7CF0(self);
    if (s == 0) {
        return 0;
    }
    strcpy(out, s);
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A8070);
#ifdef SKIP_ASM
extern "C" int func_0041AA88(const char* a, const char* b);
extern "C" int func_002A7EF0(void* self, const char* s);
extern "C" float func_002A7E60(void* self);
extern char D_004A3740[];

extern "C" int func_002A8070(void* self, const char* name, float* out)
{
    if (func_0041AA88((char*)self + 0xC, name) != 0) {
        return 0;
    }
    if (func_002A7EF0(self, D_004A3740) != 0) {
        return 0;
    }
    *out = func_002A7E60(self);
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A80D8);
#ifdef SKIP_ASM
extern "C" int func_0041AA88(const char* a, const char* b);
extern "C" int func_002A7EF0(void* self, const char* s);
extern "C" int func_002A7EA8(void* self);
extern char D_004A3740[];

extern "C" int func_002A80D8(void* self, const char* name, int* out)
{
    if (func_0041AA88((char*)self + 0xC, name) != 0) {
        return 0;
    }
    if (func_002A7EF0(self, D_004A3740) != 0) {
        return 0;
    }
    *out = func_002A7EA8(self);
    return 1;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A8140);

INCLUDE_ASM("sound/soundsys", func_002A82D8);

INCLUDE_ASM("sound/soundsys", func_002A8450);

INCLUDE_ASM("sound/soundsys", func_002A86B8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A8C20);
#ifdef SKIP_ASM
extern char D_004835F8[];

extern "C" void* func_002A8C20(void* self)
{
    *(void**)self = D_004835F8;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x30C) = -1;
    *(int*)((char*)self + 0x310) = -1;
    *(int*)((char*)self + 0x31C) = 0;
    *(int*)((char*)self + 0x318) = -1;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A8C50);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

extern "C" void func_002A8C50(void* self, int flags)
{
    *(void**)self = D_004835F8;
    void* p = *(void**)((char*)self + 0x4);
    if (p != 0) {
        cMemMan_free(p);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A8CB0);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_003B5478();
extern "C" void func_003B5158(void* buf, int size, int flags);
extern "C" int func_003B5B68(int* out);
extern const char D_00483158[];

struct sSndVtE8CB0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sSndVoice8CB0 {
    int idx;
    int a;
    int b;
    int c;
    int d;
    int next;
};

struct sSndSys8CB0 {
    sSndVtE8CB0* vt;            // 0x0
    void* buf;                  // 0x4
    int pad_8;                  // 0x8
    sSndVoice8CB0 voices[32];   // 0xC
    int f30C;                   // 0x30C
    int freeHead;               // 0x310
    int pad_314;                // 0x314
    int f318;                   // 0x318
    int f31C;                   // 0x31C
};

extern "C" void func_002A8CB0(sSndSys8CB0* self, int a1)
{
    self->buf = operator_new_tag(0x32000, D_00483158, 0, 0);
    func_003B5478();
    sSndVtE8CB0* vt = self->vt;
    vt[2].fn((char*)self + vt[2].delta, a1);
    func_003B5158(self->buf, 0x32000, 0x80303);
    self->freeHead = 31;
    self->f318 = -1;
    for (int i = 31; i >= 0; i--) {
        self->voices[i].idx = i;
        self->voices[i].a = -1;
        self->voices[i].b = -1;
        self->voices[i].c = 0;
        self->voices[i].d = 0;
        self->voices[i].next = i - 1;
    }
    self->f31C = 0;
    int t = 0;
    int r = func_003B5B68(&t);
    self->f30C = t + r;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A8D90);

//100%
INCLUDE_ASM("sound/soundsys", func_002A8F50);
#ifdef SKIP_ASM
extern "C" void func_003B5B20(int a, int b);
extern "C" void func_003B5320();
void cMemMan_free(void*);

extern "C" void func_002A8F50(void* self)
{
    void* p;
    func_003B5B20(-1, -1);
    func_003B5320();
    p = *(void**)((char*)self + 0x4);
    if (p != 0) {
        cMemMan_free(p);
    }
    *(void**)((char*)self + 0x4) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A8FA0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002A8C20(void* self);
extern const char D_00483168[];

extern "C" void* func_002A8FA0()
{
    return func_002A8C20(cMemMan_alloc(0x320, D_00483168, 0, 0));
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A8FD8);

INCLUDE_ASM("sound/soundsys", func_002A9128);

//100%
INCLUDE_ASM("sound/soundsys", func_002A9250);
#ifdef SKIP_ASM
extern "C" void func_003B5B20(int a, int b);

struct sSndPoolEntry2A9250 {
    int unk0;
    int unk4;
    int unk8;
    char pad[0x14 - 0xC];
    int next;
};

struct sSndPool2A9250 {
    char pad[0xC];
    sSndPoolEntry2A9250 entries[32];
    int unk30C;
    int freeHead;
    char pad2[0x31C - 0x314];
    int unk31C;
};

extern "C" void func_002A9250(sSndPool2A9250* self, int i)
{
    self->unk31C = 1;
    sSndPoolEntry2A9250* e = &self->entries[i];
    func_003B5B20(e->unk4, e->unk8);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A9288);
#ifdef SKIP_ASM
extern "C" void func_003B5B20(int a, int b);

extern "C" void func_002A9288(void* self)
{
    func_003B5B20(-1, *(int*)((char*)self + 0x30C));
    *(int*)((char*)self + 0x31C) = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A92B8);
#ifdef SKIP_ASM
struct s2A92B8Entry {
    char pad[0x14];
    int next;
};

struct s2A92B8 {
    char pad[0xC];
    s2A92B8Entry entries[32];
    int unk30C;
    int freeHead;
};

extern "C" s2A92B8Entry* func_002A92B8(s2A92B8* self)
{
    int i = self->freeHead;
    if (i < 0) {
        return 0;
    }
    s2A92B8Entry* e = &self->entries[i];
    self->freeHead = e->next;
    e->next = -1;
    return e;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A92F8);
#ifdef SKIP_ASM
extern "C" void func_003B5B20(int a, int b);

struct s2A92F8Entry {
    int index;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int next;
};

extern "C" void func_002A92F8(void* self, s2A92F8Entry* e)
{
    *(int*)((char*)self + 0x30C) = e->unk8;
    e->unk4 = -1;
    e->unk8 = -1;
    e->unkC = 0;
    e->unk10 = 0;
    e->next = *(int*)((char*)self + 0x310);
    *(int*)((char*)self + 0x310) = e->index;
    if (*(int*)((char*)self + 0x31C) == 0) {
        func_003B5B20(-1, *(int*)((char*)self + 0x30C));
    }
}
#endif

