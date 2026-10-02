#include "common.h"

INCLUDE_ASM("sound/ssxAudio", SSXAUDIO_Init);

INCLUDE_ASM("sound/ssxAudio", func_00284C28);

INCLUDE_ASM("sound/ssxAudio", func_00284C68);

INCLUDE_ASM("sound/ssxAudio", func_00285210);

INCLUDE_ASM("sound/ssxAudio", func_002854A8);

INCLUDE_ASM("sound/ssxAudio", func_002854F8);

INCLUDE_ASM("sound/ssxAudio", func_00285930);

//100%
INCLUDE_ASM("sound/ssxAudio", func_00285BE0);
#ifdef SKIP_ASM
extern "C" void func_00285BE0(void* self, int a1)
{
    int* p = (int*)((char*)self + 0x6474);
    if (*p != a1) {
        *p = a1;
    }
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_00285BF8);

INCLUDE_ASM("sound/ssxAudio", func_00285D98);

//100%
INCLUDE_ASM("sound/ssxAudio", func_00285F48);
#ifdef SKIP_ASM
extern "C" void func_002A7718(void*);
extern "C" void func_002AD3C0(void*);

extern "C" void func_00285F48(void* self)
{
    func_002A7718(self);
    void* inner = **(void***)((char*)self + 0x118);
    func_002AD3C0((char*)inner + 0x1D8);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00285F80);
#ifdef SKIP_ASM
// PORT: callee declared variadic to reproduce the original by-value struct passing; real signature takes an 8-byte struct.
struct func_00285F80_sPair {
    int a;
    int b;
};

extern "C" void func_002AD410(void* obj, ...);

extern "C" void func_00285F80(void* self, func_00285F80_sPair p)
{
    void* inner = **(void***)((char*)self + 0x118);
    func_002AD410((char*)inner + 0x1D8, p);
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_00285FB0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", func_00286200);
#ifdef SKIP_ASM
struct func_00286200_vte { short delta; short index; void (*fn)(void*); };
extern "C" void func_002B3AC0(void*);
extern "C" void func_002B3D48(void*, float);
extern "C" void func_002ADDA0(void*);
extern "C" void func_0028BCE8(void*, int);

extern "C" void func_00286200(void* self)
{
    if (*(int*)((char*)self + 0x608C) == 2 || *(int*)((char*)self + 0x6090) == 2) {
        func_002B3AC0((char*)self + 0x118);
    } else {
        func_002B3D48((char*)self + 0x118, 1.0f);
    }
    func_00285BE0(self, 0);
    func_002ADDA0(**(char***)((char*)self + 0x118) + 0x1D8);
    char* in = **(char***)((char*)self + 0x118);
    char* obj = in + 0x1D8;
    func_00286200_vte* vt = *(func_00286200_vte**)(in + 0xAB0);
    vt[2].fn(obj + vt[2].delta);
    func_0028BCE8(**(void***)((char*)self + 0x118), 0);
}
#endif

INCLUDE_ASM("sound/ssxAudio", cSSXAudio_FrontEndLoad);

INCLUDE_ASM("sound/ssxAudio", func_002867E8);

INCLUDE_ASM("sound/ssxAudio", func_00286A80);

//100%
INCLUDE_ASM("sound/ssxAudio", func_00286C00);
#ifdef SKIP_ASM
extern "C" void func_002A6F38(void*);
extern "C" void func_00289DF0(void*, int);
extern "C" void func_0028BCE8(void*, int);
extern "C" void func_0028BDE0(void*, int);
extern "C" void func_0028A230(void*);
extern "C" void func_0029F5E0(void*);

extern "C" void func_00286C00(void* self)
{
    int i;
    func_002A6F38(self);
    func_00289DF0(**(void***)((char*)self + 0x118), 1);
    for (i = 0; i < 17; i++) {
        if (i != 13) {
            func_0028BCE8(**(void***)((char*)self + 0x118), i);
        }
    }
    func_0028BDE0(**(void***)((char*)self + 0x118), 3);
    func_0028BDE0(**(void***)((char*)self + 0x118), 12);
    func_0028A230(**(void***)((char*)self + 0x118));
    func_0029F5E0(self);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00286CA8);
#ifdef SKIP_ASM
struct sSsxAudioBanks {
    char pad[0x6C70];
    int loaded[2];
    int sub[2];
};
extern "C" void func_0028BE60(void* self, int i, int a, int b);

extern "C" void func_00286CA8(sSsxAudioBanks* self, unsigned int id, int on)
{
    unsigned int bank = id >> 8;
    if (on != 0) {
        func_0028BE60(**(void***)((char*)self + 0x118), bank + 8, on, 0x20000);
        self->loaded[bank] = 1;
        self->sub[bank] = id & 0xFF;
    }
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_00286D18);

INCLUDE_ASM("sound/ssxAudio", func_00286E20);

INCLUDE_ASM("sound/ssxAudio", func_00286EA0);

//100%
INCLUDE_ASM("sound/ssxAudio", func_002870A0);
#ifdef SKIP_ASM
extern "C" void func_002871B0(void* self);
extern "C" int func_002A4040(void* self);
extern "C" int func_00295028(void* self, int a, int b);
extern "C" void func_00295628(void* self, int a, int b, int c, int d, int e);

extern "C" void func_002870A0(void* self)
{
    func_002871B0(self);
    if (func_002A4040(self) == 0 && func_00295028(self, 0, 0) == 0) {
        func_00295628(self, 0, 0, -1, 0, 0);
    }
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_00287108);

INCLUDE_ASM("sound/ssxAudio", func_002871B0);

INCLUDE_ASM("sound/ssxAudio", func_002872A8);

INCLUDE_ASM("sound/ssxAudio", func_002873D8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", func_00287410);
#ifdef SKIP_ASM
extern "C" float func_002873D8(void*);
// PORT: func_00287700's real parameter order is (self, int, float); the unit declares (self, float, int).
void func_00287700_if(void*, int, float) __asm__("func_00287700");

extern "C" void func_00287410(void* self)
{
    float v = func_002873D8(self);
    func_00287700_if(self, 1, v);
    func_00287700_if(self, 8, v);
    if (*(int*)((char*)self + 0x62B8) != 0 && *(int*)((char*)self + 0x608C) != 2) {
        func_00287700_if(self, 2, v);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", func_00287488);
#ifdef SKIP_ASM
extern "C" float func_002873D8(void*);
// PORT: func_00287700's real parameter order is (self, int, float); the unit declares (self, float, int).
void func_00287700_if(void*, int, float) __asm__("func_00287700");

extern "C" void func_00287488(void* self)
{
    float v = func_002873D8(self);
    func_00287700_if(self, 5, v);
    func_00287700_if(self, 3, v);
    func_00287700_if(self, 6, v);
    func_00287700_if(self, 7, v);
    if (*(int*)((char*)self + 0x62B4) != 0) {
        func_00287700_if(self, 9, v);
        func_00287700_if(self, 10, v);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", func_00287520);
#ifdef SKIP_ASM
extern "C" float func_002873D8(void*);
extern "C" void func_00287700(void*, float, int);

extern "C" void func_00287520(void* self)
{
    func_00287700(self, func_002873D8(self), 4);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00287558);
#ifdef SKIP_ASM
extern "C" void func_00287700(void*, float, int);
extern "C" float func_00287920(void* self, int a1);

extern "C" void func_00287558(void* self, int on)
{
    if (on != 0 && *(int*)((char*)self + 0x608C) != 2 && *(int*)((char*)self + 0x608C) != 3) {
        func_00287700(self, func_00287920(self, 1), 2);
        *(int*)((char*)self + 0x62B8) = 1;
    } else {
        func_00287700(self, 0.0f, 2);
        *(int*)((char*)self + 0x62B8) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_002875D0);
#ifdef SKIP_ASM
extern "C" void func_00287700(void*, float, int);
extern "C" float func_00287920(void* self, int a1);

extern "C" void func_002875D0(void* self, int on)
{
    if (on != 0) {
        func_00287700(self, func_00287920(self, 5), 9);
        func_00287700(self, func_00287920(self, 5), 10);
    } else {
        func_00287700(self, 0.0f, 9);
        func_00287700(self, 0.0f, 10);
    }
    *(int*)((char*)self + 0x62B4) = on;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00287670);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int a1);

extern "C" int func_00287670(void* self)
{
    return (int)(func_00287920(self, 1) * 11.0f);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_002876A0);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int a1);

extern "C" int func_002876A0(void* self)
{
    return (int)(func_00287920(self, 5) * 11.0f);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_002876D0);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int a1);

extern "C" int func_002876D0(void* self)
{
    return (int)(func_00287920(self, 4) * 11.0f);
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_00287700);

//100%
INCLUDE_ASM("sound/ssxAudio", func_00287920);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int a1)
{
    self = (char*)self + (a1 << 4);
    return *(float*)((char*)self + 0x62bc);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00287930);
#ifdef SKIP_ASM
extern "C" float func_00287930(void* self, int a1, int a2)
{
    if (a1 != 4 || a2 >= 10) {
        return *(float*)((char*)self + (a1 << 4) + 0x62C8);
    }
    return *(float*)((char*)self + (a2 << 2) + 0x636C);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00287968);
#ifdef SKIP_ASM
extern "C" void* func_00287968(void* self, int a1, int a2)
{
    if (a1 != 4 || a2 >= 10) {
        // PORT: pointer held in int (only spelling found that gives idx-first addu)
        return (char*)((a1 << 4) + (int)self + 0x62C8);
    }
    return (char*)self + ((a2 << 2) + 0x636C);
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_00287A10);

INCLUDE_ASM("sound/ssxAudio", func_00287C48);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", func_00287F00);
#ifdef SKIP_ASM
extern "C" void func_00287A10(void* self, int a1, float a2, float a3);
extern "C" int func_00284BA0(int);

extern "C" void func_00287F00(void* self, int a1, float f0, float f1)
{
    if (a1 == 3) {
        if (*(int*)((char*)self + 0x63DC) != 0) {
            func_00287A10(self, func_00284BA0(*(int*)((char*)self + 0x63D4)), 1.0f, f0);
            *(float*)((char*)self + 0x63D8) = f1;
            *(int*)((char*)self + 0x63D4) = a1;
            *(int*)((char*)self + 0x63DC) = 0;
        }
    } else if (*(int*)((char*)self + 0x63DC) == 0) {
        func_00287A10(self, func_00284BA0(a1), f1, f0);
        *(int*)((char*)self + 0x63D4) = a1;
        *(int*)((char*)self + 0x63DC) = 1;
        *(float*)((char*)self + 0x63D8) = f1;
        *(int*)((char*)self + 0x63E0) = 0;
    }
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_00287FC8);

INCLUDE_ASM("sound/ssxAudio", func_00288370);

INCLUDE_ASM("sound/ssxAudio", func_002883B0);

INCLUDE_ASM("sound/ssxAudio", func_002887A8);

INCLUDE_ASM("sound/ssxAudio", func_002887F8);

INCLUDE_ASM("sound/ssxAudio", func_00288940);

//100%
INCLUDE_ASM("sound/ssxAudio", func_00288A20);
#ifdef SKIP_ASM
extern "C" int func_0028B1B0(void);
extern "C" int func_00288940(void* self, int id);
// PORT: callers pass self, but func_00288CE8's body takes no arguments (the unit declares it ()).
int func_00288CE8_self(void* self) __asm__("func_00288CE8");

struct sAudVtEnt { short delta; short index; int (*fn)(void*); };

extern "C" int func_00288A20(void* self, void* obj)
{
    if (func_0028B1B0() == 0) {
        return 0;
    }
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        return *(int*)((char*)obj + 0x870);
    }
    if (func_00288CE8_self(self) == 1) {
        return 0;
    }
    char* o = (char*)obj + 0x6C0;
    sAudVtEnt* e = &(*(sAudVtEnt**)o)[5];
    return func_00288940(self, e->fn(o + e->delta));
}
#endif

extern "C" void* func_0028B210(int);

//100%
INCLUDE_ASM("sound/ssxAudio", func_00288AC0__FPvi);
#ifdef SKIP_ASM
void* func_00288AC0(void* self, int a1)
{
    return (char*)func_0028B210(a1) + 0x20;
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_00288AE0);

//100%
INCLUDE_ASM("sound/ssxAudio", func_00288B40);
#ifdef SKIP_ASM
struct sAudV4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

void* func_00288AC0(void* self, int a1);

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sAudV4 audVu0Sub(const sAudV4& a, const sAudV4& b)
{
    sAudV4 r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float audVu0Dot(const sAudV4& a, const sAudV4& b)
{
    float r;
    int t;
    __asm__ __volatile__(
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

extern "C" int func_00288B40(void* self, sAudV4* pos, float radius)
{
    float r = radius * 100.0f;
    r = r * r;
    int i;
    for (i = 0; i < func_00288CE8_self(self); i++) {
        sAudV4 d = audVu0Sub(*(sAudV4*)func_00288AC0(self, i), *pos);
        if (audVu0Dot(d, d) < r) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00288C08);
#ifdef SKIP_ASM
void* func_00288AC0(void* self, int a1);

// PORT: PS2-only VU0 inline asm (vector subtract).
// PORT: PS2-only VU0 inline asm (4-component dot product).
extern "C" int func_00288C08(void* self, sAudV4* pos, float radius)
{
    float r = radius * 100.0f;
    r = r * r;
    int i;
    for (i = 0; i < func_00288CE8_self(self); i++) {
        sAudV4 d = audVu0Sub(*(sAudV4*)func_00288AC0(self, i), *pos);
        if (audVu0Dot(d, d) < r && 0.0f < d.z) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00288CE8);
#ifdef SKIP_ASM
extern "C" void* func_0028B1C0();
extern "C" int func_0028B1F8();

extern "C" int func_00288CE8()
{
    if (*(int*)((char*)func_0028B1C0() + 0x84) != 0) {
        return func_0028B1F8();
    }
    return 1;
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_00288D18);

INCLUDE_ASM("sound/ssxAudio", func_00288F60);

INCLUDE_ASM("sound/ssxAudio", func_00289470);

INCLUDE_ASM("sound/ssxAudio", func_00289520);

INCLUDE_ASM("sound/ssxAudio", func_00289650);

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289680__FPv);
#ifdef SKIP_ASM
void func_00289680(void* self)
{
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_00289688);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", func_00289830);
#ifdef SKIP_ASM
extern "C" void func_00287A10(void* self, int a1, float a2, float a3);

extern "C" void func_00289830(void* self, float v)
{
    if (*(float*)((char*)self + 0x6C5C) != v) {
        func_00287A10(self, 1, v, 0.0f);
        *(float*)((char*)self + 0x6C5C) = v;
    }
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_002898A8);

//100%
INCLUDE_ASM("sound/ssxAudio", func_002899E8);
#ifdef SKIP_ASM
extern "C" int func_002899E8(void* self)
{
    return *(int*)((char*)self + 0x608c) == 2;
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_002899F8);

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289AF0__FPv);
#ifdef SKIP_ASM
void func_00289AF0(void* self)
{
}
#endif

extern "C" void* func_00292AE8(void* self);

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289AF8__FPv);
#ifdef SKIP_ASM
void* func_00289AF8(void* self)
{
    return func_00292AE8(self);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289B18);
#ifdef SKIP_ASM
extern "C" void func_0029BCF8(void* a0, void* a1);
extern "C" void func_0029B968(void* a0, void* a1);
extern "C" void func_0029C088(void* a0, void* a1);

extern "C" void func_00289B18(void* a0, void* a1, int type)
{
    if (type == 0x50) {
        func_0029BCF8(a0, a1);
    } else if (type == 0x51) {
        func_0029B968(a0, a1);
    } else if (type == 0x52) {
        func_0029C088(a0, a1);
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289B70);
#ifdef SKIP_ASM
extern "C" void func_0029CE28(void* self);
extern "C" void func_002B3A70(void*);

extern "C" void func_00289B70(void* self)
{
    if (*(int*)((char*)self + 0x5FB4) == 0) {
        func_0029CE28(self);
        func_002B3A70((char*)self + 0x118);
        *(int*)((char*)self + 0x5FB4) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289BB8);
#ifdef SKIP_ASM
extern "C" void func_002B3A98(void*);
extern "C" void func_0029CE70(void*);
extern "C" void func_002B11B0(void*, int);
extern "C" void func_002A4550(void*);

extern "C" void func_00289BB8(void* self)
{
    if (*(int*)((char*)self + 0x5FB4) != 0) {
        func_002B3A98((char*)self + 0x118);
        func_0029CE70(self);
        *(int*)((char*)self + 0x5FB4) = 0;
        if (*(int*)((char*)self + 0x5828) != 0) {
            func_002B11B0((char*)self + 0x5560, 0);
            func_002A4550(self);
            *(int*)((char*)self + 0x5828) = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289C18);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00148950(void* iface, int id);

struct sSsxAudioVEntryI {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_00289C18(void* self, void* src)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 3);
    char* obj = (char*)src + 0x6C0;
    sSsxAudioVEntryI* vt = *(sSsxAudioVEntryI**)obj;
    int n = func_00148950(iface, vt[7].fn(obj + vt[7].delta));
    if (n < 4) {
        return 0;
    }
    if (n < 8) {
        return 1;
    }
    return 2;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", func_00289C98);
#ifdef SKIP_ASM
extern "C" int func_00285D98(void* self, int which);
extern "C" void func_002883B0(void* self, int id);

struct sSsxAudioVEntryP {
    short delta;
    short index;
    int* (*fn)(void*);
};

extern "C" void func_00289C98(void* self, int id, void* obj)
{
    if (id == func_00285D98(self, -1) && obj != 0) {
        sSsxAudioVEntryP* vt = *(sSsxAudioVEntryP**)((char*)obj + 0x4);
        func_002883B0(self, *vt[3].fn((char*)obj + vt[3].delta));
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289D08);
#ifdef SKIP_ASM
extern "C" int func_00289D08(void* self, unsigned char a1)
{
    int r = 1;
    switch (a1) {
    case 0:
        break;
    case 1:
        r = 2;
        break;
    case 2:
        r = 0;
        break;
    case 3:
        r = 3;
        break;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289D60);
#ifdef SKIP_ASM
extern "C" void* func_00289D60(void* self, int a1)
{
    int r = 0;
    switch (a1) {
    case 1:
        break;
    case 2:
        r = 1;
        break;
    case 0:
        r = 2;
        break;
    case 3:
        r = 3;
        break;
    }
    return (void*)r;
}
#endif

extern "C" void* func_00289D60(void*, int);

//99.29%
INCLUDE_ASM("sound/ssxAudio", func_00289DC0__FPv);
#ifdef SKIP_ASM
void* func_00289DC0(void* self)
{
    return func_00289D60(self, *(int*)((char*)self + 0x62b0));
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289DE0__FPv);
#ifdef SKIP_ASM
int func_00289DE0(void* self)
{
    int old = *(int*)((char*)self + 0x6c88);
    *(int*)((char*)self + 0x6c88) = 0;
    return old;
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_00289DF0);

INCLUDE_ASM("sound/ssxAudio", func_0028A058);

INCLUDE_ASM("sound/ssxAudio", func_0028A148);

