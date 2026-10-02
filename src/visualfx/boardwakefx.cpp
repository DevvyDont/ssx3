#include "common.h"

INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX_cBoardWakeFX);

INCLUDE_ASM("visualfx/boardwakefx", func_002DCDA0);

INCLUDE_ASM("visualfx/boardwakefx", func_002DCF28);

INCLUDE_ASM("visualfx/boardwakefx", func_002DD0B8);

INCLUDE_ASM("visualfx/boardwakefx", func_002DDAB8);

INCLUDE_ASM("visualfx/boardwakefx", func_002DDD30);

INCLUDE_ASM("visualfx/boardwakefx", func_002DE058);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002DE368);
#ifdef SKIP_ASM
struct sVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sVec4 D_004FF120;

extern "C" void func_002DE368(void* self)
{
    *(int*)((char*)self + 0x34) = 0;
    *(int*)((char*)self + 0x30) = 0;
    *(sVec4*)((char*)self + 0x0) = D_004FF120;
    *(sVec4*)((char*)self + 0x20) = D_004FF120;
    *(sVec4*)((char*)self + 0x10) = D_004FF120;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002DE398);

INCLUDE_ASM("visualfx/boardwakefx", func_002DE4A8);

INCLUDE_ASM("visualfx/boardwakefx", func_002DF1C8);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002DF3B0);
#ifdef SKIP_ASM
extern "C" void func_002E26B8(void* self);
extern "C" void func_002E2550(void* self);

extern "C" void func_002DF3B0(void* self)
{
    sVec4 c;
    c.x = 1.0f;
    c.y = 1.0f;
    c.z = 1.0f;
    c.w = 1.0f;
    *(float*)((char*)self + 0x120) = 0.5f;
    *(int*)((char*)self + 0x10) = 0;
    *(sVec4*)((char*)self + 0x90) = c;
    *(int*)((char*)self + 0x74) = 0;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x80) = 0;
    *(int*)((char*)self + 0x78) = 0;
    *(int*)((char*)self + 0xE0) = 0;
    *(int*)((char*)self + 0x12C) = 0;
    if (*(int*)((char*)self + 0x124) != (*(int*)(*(char**)self + 0x870) >= 0)) {
        func_002E26B8(self);
    }
    func_002E2550(self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002DF448);
#ifdef SKIP_ASM
struct sVEntry2DF448 {
    short delta;
    short index;
    void (*fn)(void*, void*, float);
};

struct func_002DF448_sElem {
    char pad0[0x58];
    float f58;                      // 0x58
    char pad5C[0x1F8 - 0x5C];
    sVEntry2DF448* vt;              // 0x1F8
    char pad1FC[0x210 - 0x1FC];
};

struct func_002DF448_sFx {
    char pad0[0x28];
    func_002DF448_sElem* elems;     // 0x28
    char* data;                     // 0x2C
    char pad30[0x60 - 0x30];
    int f60;                        // 0x60
};

extern "C" void func_002DF448(func_002DF448_sFx* self)
{
    int i;
    for (i = 0; i < 10; i++) {
        // PORT: pointer held in int (index-first address arithmetic)
        func_002DF448_sElem* e = (func_002DF448_sElem*)(i * 0x210 + (int)self->elems);
        sVEntry2DF448* vt = e->vt;
        vt[3].fn((char*)e + vt[3].delta, self->data + i * 0xE8, e->f58);
    }
    self->f60 = 0;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002DF4D0);

INCLUDE_ASM("visualfx/boardwakefx", func_002DF920);

INCLUDE_ASM("visualfx/boardwakefx", func_002DFE88);

INCLUDE_ASM("visualfx/boardwakefx", func_002E02B8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E0EE8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E1120);

INCLUDE_ASM("visualfx/boardwakefx", func_002E1598);

INCLUDE_ASM("visualfx/boardwakefx", func_002E1A80);

INCLUDE_ASM("visualfx/boardwakefx", func_002E1F70);

INCLUDE_ASM("visualfx/boardwakefx", func_002E2260);

INCLUDE_ASM("visualfx/boardwakefx", func_002E23E0);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E24D0);
#ifdef SKIP_ASM
extern "C" void func_00371688(void* self, int mode);

struct func_002E24D0_sWake {
    char pad_0x000[0x174];
    void* field_0x174;
    char pad_0x178[0x1E0 - 0x178];
    int field_0x1E0;
    char pad_0x1E4[0x210 - 0x1E4];
};

struct func_002E24D0_sFx {
    void* owner;                    // 0x0
    char pad_0x04[0x24];
    func_002E24D0_sWake* wakes;     // 0x28
};

extern "C" void func_002E24D0(func_002E24D0_sFx* self)
{
    if (*(int*)((char*)self->owner + 0xB18) != 0) {
        int i;
        for (i = 0; i < 10; i++) {
            func_002E24D0_sWake* w = &self->wakes[i];
            if (w->field_0x174 != 0 && w->field_0x1E0 > 0) {
                func_00371688(w, 7);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E2550);
#ifdef SKIP_ASM
#define S28(off) (*(int*)((char*)*(void**)((char*)self + 0x28) + (off)))

extern "C" void func_002E2550(void* self)
{
    int mode = *(int*)((char*)*(void**)self + 0x898);
    *(int*)((char*)self + 0x128) = mode;
    switch (mode) {
    case 0:
        S28(0xBC4) = 1;
        S28(0xDD4) = 1;
        S28(0x174) = 1;
        {
            int v = 0;
            if (*(int*)((char*)self + 0x124) != 0 || *(int*)((char*)*(void**)self + 0xAC4) != 0)
                v = 1;
            S28(0x9B4) = v;
        }
        S28(0x384) = *(int*)((char*)self + 0x124);
        S28(0x594) = *(int*)((char*)self + 0x124);
        S28(0x7A4) = *(int*)((char*)self + 0x124);
        S28(0xFE4) = *(int*)((char*)self + 0x124);
        S28(0x11F4) = *(int*)((char*)self + 0x124);
        S28(0x1404) = *(int*)((char*)self + 0x124);
        break;
    case 1:
        S28(0xBC4) = 1;
        S28(0xDD4) = 1;
        S28(0x174) = 1;
        S28(0x384) = *(int*)((char*)self + 0x124);
        S28(0x594) = *(int*)((char*)self + 0x124);
        S28(0x7A4) = *(int*)((char*)self + 0x124);
        S28(0xFE4) = *(int*)((char*)self + 0x124);
        S28(0x11F4) = *(int*)((char*)self + 0x124);
        S28(0x9B4) = 0;
        S28(0x1404) = 0;
        break;
    default:
        S28(0xBC4) = 1;
        S28(0xDD4) = *(int*)((char*)self + 0x124);
        S28(0x384) = 0;
        S28(0x174) = 0;
        S28(0x594) = 0;
        S28(0x7A4) = 0;
        S28(0xFE4) = 0;
        S28(0x9B4) = 0;
        S28(0x11F4) = 0;
        S28(0x1404) = 0;
        break;
    }
}
#undef S28
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E26B8);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E27E8);
#ifdef SKIP_ASM
extern "C" float func_002E27E8(int type)
{
    switch (type) {
    case 0x10:
        return 180.0f;
    case 0x20:
        return 100.0f;
    case 0x40:
        return 350.0f;
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E2860__FPv);
#ifdef SKIP_ASM
void* func_002E2860(void* self)
{
    return self;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E2868);

INCLUDE_ASM("visualfx/boardwakefx", func_002E2B00);

INCLUDE_ASM("visualfx/boardwakefx", func_002E2E18);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E2F98__FPvi);
#ifdef SKIP_ASM
void func_002E2F98(void* self, int i)
{
    *(int*)((char*)self + (i << 2) + 0x60) = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E2FA8);
#ifdef SKIP_ASM
struct sWakePoint {
    int a;
    int pad04;
    int c;
    short d;
    short e;
    char pad10[0x20];
};

struct sWakeFx {
    char pad00[0x60];
    int counts[10];
    sWakePoint points[1][256];
};

extern "C" void func_002E2FA8(sWakeFx* self, int v, int i)
{
    int n = self->counts[i];
    self->points[i][n].a = v;
    self->points[i][n].d = 0;
    self->points[i][n].e = 0;
    self->points[i][n].c = 0;
    self->counts[i]++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", func_002E2FF8);
#ifdef SKIP_ASM
extern "C" void func_002E2FA8(sWakeFx* self, int v, int i);

extern "C" void func_002E2FF8(sWakeFx* self, int* vals, int count, int unused, int i)
{
    int n;
    for (n = 0; n < count; n++) {
        func_002E2FA8(self, vals[n], i);
    }
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E3060);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E30D0);
#ifdef SKIP_ASM
extern "C" void func_002E3338(void* self, int i);
extern "C" void func_002E3478(void* self, int i);

extern "C" void func_002E30D0(void* self, int i)
{
    func_002E3338(self, i);
    func_002E3478(self, i);
}
#endif

extern "C" void* func_002E3130(void* self);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E3110__FPv);
#ifdef SKIP_ASM
void* func_002E3110(void* self)
{
    return func_002E3130(self);
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E3130);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3338);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3478);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3578);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3668);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3680);

INCLUDE_ASM("visualfx/boardwakefx", func_002E37D0);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3930);

INCLUDE_ASM("visualfx/boardwakefx", func_002E39D8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3AF8);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4228);
#ifdef SKIP_ASM
extern "C" void* func_00354648(void* self, void* a1);
extern "C" void* func_00282CD0(void* self);
extern "C" void func_00283298(void* self);
extern "C" void func_002E4CB0(void);
extern void* D_00488100[];
extern void* D_00488158[];

extern "C" void* func_002E4228(void* self, void* a1, int a2)
{
    func_00354648(self, a1);
    func_00282CD0((char*)self + 0x10);
    *(int*)((char*)self + 0x74) = a2;
    *(int*)((char*)self + 0x40) = 0;
    *(int*)((char*)self + 0x44) = 0;
    *(int*)((char*)self + 0x4C) = 0;
    *(int*)((char*)self + 0x50) = 0;
    *(int*)((char*)self + 0x60) = 0;
    *(int*)((char*)self + 0x64) = 0;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0x70) = 0;
    *(int*)((char*)self + 0x78) = 0;
    *(int*)((char*)self + 0x7C) = 0;
    *(int*)((char*)self + 0x80) = 0;
    *(void***)((char*)self + 0x1C) = D_00488100;
    *(void***)((char*)self + 0xC) = D_00488158;
    *(int*)((char*)self + 0x6C) = -1;
    func_002E4CB0();
    func_00283298((char*)self + 0x10);
    return self;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E42D0);

INCLUDE_ASM("visualfx/boardwakefx", func_002E4370);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", func_002E44F0);
#ifdef SKIP_ASM
// PORT: the unit declares func_002E4370 as void; this caller returns its int result.
extern "C" int func_002E4370_i(void* self, int a1, int a2, int a3, float f0, float f1, int a4, int a5, float f2) __asm__("func_002E4370");

extern "C" int func_002E44F0(void* self, int a1, int a2, float f)
{
    if (*(int*)((char*)self + 0x44) == 0) {
        return func_002E4370_i(self, a1, 0, a2, 0.0f, 0.0f, 0, 0, f);
    }
    *(int*)((char*)self + 0x50) = a2;
    *(float*)((char*)self + 0x5C) = f;
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", func_002E4540);
#ifdef SKIP_ASM
extern "C" void func_002E4370(void* self, int a1, int a2, int a3, float f0, float f1, int a4, int a5, float f2);

extern "C" void func_002E4540(void* self, int a1, int a3, int a5, float f2)
{
    func_002E4370(self, a1, 0, a3, 0.0f, 0.0f, 1, a5, f2);
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E4578);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4638);
#ifdef SKIP_ASM
extern "C" void func_002E4638(void* self, int up)
{
    if (*(int*)((char*)self + 0x44) == 0) {
        *(int*)((char*)self + 0x40) = 0;
        return;
    }
    if (up) {
        *(int*)((char*)self + 0x40) += 1;
        return;
    }
    if (*(int*)((char*)self + 0x40) != 0) {
        *(int*)((char*)self + 0x40) -= 1;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4678);
#ifdef SKIP_ASM
extern "C" float func_002E4678(void* self)
{
    if (*(int*)((char*)self + 0x44) == 0) {
        return 0.0f;
    }
    float v = *(float*)((char*)self + 0x54) + *(float*)((char*)self + 0x58)
            + *(float*)((char*)self + 0x5C) - *(float*)((char*)self + 0x48);
    if (v < 0.0f) {
        return 0.0f;
    }
    return v;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E46C8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E47E8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E4B98);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4C60);
#ifdef SKIP_ASM
void func_00283440(void*);
extern "C" void func_002E4578(void* self);

extern "C" void func_002E4C60(void* self)
{
    func_00283440((char*)self + 0x10);
    func_002E4578(self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4C90);
#ifdef SKIP_ASM
extern "C" int func_002E4C90(void* self)
{
    int r = 0;
    if (*(int*)((char*)self + 0x60) != 0) {
        r = *(int*)((char*)self + 0x64) != 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4CB0);
#ifdef SKIP_ASM
extern int D_00538850[];

extern "C" void func_002E4CB0(void)
{
    int i;
    for (i = 0; i < 4; i++) {
        D_00538850[i] = 0;
    }
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E4CE8);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4D30);
#ifdef SKIP_ASM
struct sWakeSlot {
    char pad[0x64];
};

extern sWakeSlot D_005382C0[];
extern int D_00538850[];

extern "C" void func_002E4D30(void* p)
{
    int i;
    if (p == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (&D_005382C0[i] == p) {
            D_00538850[i] = 0;
            return;
        }
    }
}
#endif

extern void* D_004880E0[];

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4D70__FPv);
#ifdef SKIP_ASM
void* func_002E4D70(void* self)
{
    *(int*)self = (int)(void*)D_004880E0;
    return self;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E4D88);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4F10__FPv);
#ifdef SKIP_ASM
void func_002E4F10(void* self)
{
    *(int*)((char*)self + 0x94) = 0;
    *(int*)((char*)self + 0xb0) = 1;
    *(int*)((char*)self + 0x90) = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4F28);
#ifdef SKIP_ASM
void operator_delete(int* p);

extern "C" void func_002E4F28(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E4F50);

INCLUDE_ASM("visualfx/boardwakefx", func_002E5430);

INCLUDE_ASM("visualfx/boardwakefx", func_002E55D8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E5920);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", func_002E5D18);
#ifdef SKIP_ASM
extern void* D_00487FD8[];
extern "C" void func_00319CC8(void);
extern "C" void func_002E4F28(int* self, int flags);
extern "C" void func_003546C8(void* self, int flags);

extern "C" void func_002E5D18(void* self, int flags)
{
    *(void***)((char*)self + 0xC) = D_00487FD8;
    func_00319CC8();
    int i;
    for (i = 0; i < 12; i++) {
        int* p = ((int**)((char*)self + 0x10))[i];
        if (p != 0) {
            func_002E4F28(p, 3);
        }
    }
    func_00319CC8();
    func_003546C8(self, flags);
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E5DA0);

INCLUDE_ASM("visualfx/boardwakefx", func_002E6008);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", func_002E62C8);
#ifdef SKIP_ASM
void func_002E4F10(void*);

extern "C" void func_002E62C8(void* self)
{
    int i;
    void** p = (void**)((char*)self + 0x10);
    for (i = 0; i < 12; i++) {
        if (p[i] != 0) {
            func_002E4F10(p[i]);
        }
    }
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E6320);

INCLUDE_ASM("visualfx/boardwakefx", func_002E64C0);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E6640);
#ifdef SKIP_ASM
struct sWakeVec4 {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16)));

extern "C" void func_002E6640(void* self)
{
    sWakeVec4 v;
    v.x = 1.0f;
    v.y = 0.0f;
    v.z = 0.0f;
    v.w = 1.0f;
    *(int*)((char*)self + 0x398) = 0x39;
    *(int*)((char*)self + 0x8) = 0;
    *(sWakeVec4*)((char*)self + 0x40) = v;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x18) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x30) = 0;
    *(int*)((char*)self + 0x3A0) = 0;
    *(int*)((char*)self + 0x39C) = 0;
    *(int*)((char*)self + 0x3A4) = (*(int*)(*(char**)self + 0x870) >= 0) ? 0x14 : 0x12;
    *(int*)((char*)self + 0x3A8) = 0;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E66B8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E6C08);

INCLUDE_ASM("visualfx/boardwakefx", func_002E7A10);

