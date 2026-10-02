#include "common.h"

//100%
INCLUDE_ASM("sound/ssxAudio", SSXAUDIO_Init);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" char* func_00284C68(void* mem, int a1, int a2, int a3, int a4);
void func_002ADBF0(void*);
extern const char D_004828B0[];
extern char* D_004A3500;

extern "C" void SSXAUDIO_Init(int a)
{
    if (D_004A3500 == 0) {
        D_004A3500 = func_00284C68(cMemMan_alloc(0x7780, D_004828B0, 0x80000400, 0), 1, 0x1C, 3, a);
    }
    func_002ADBF0(**(char***)(D_004A3500 + 0x118) + 0x1D8);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00284C28);
#ifdef SKIP_ASM
class func_00284C28_cObj {
public:
    char data[0x1D4];
    virtual void v01(int flags);
};

extern char* D_004A3500;

extern "C" void func_00284C28()
{
    if (D_004A3500 != 0) {
        (**(func_00284C28_cObj***)(D_004A3500 + 0x118))->v01(3);
    }
    D_004A3500 = 0;
}
#endif

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

//100%
INCLUDE_ASM("sound/ssxAudio", func_00286D18);
#ifdef SKIP_ASM
extern "C" void func_002AD650(void* p, int id, float v);
extern "C" void func_0029D678(void* self, float v);
extern "C" void func_0028BE90(void* mgr, int i);

static inline int saIsOn(char* cfg, int off) { return *(int*)(cfg + off) == 1; }

extern "C" void func_00286D18(void* self, unsigned int handle)
{
    unsigned int slot = handle >> 8;
    if (*(unsigned int*)((char*)self + (slot << 2) + 0x6C78) != (handle & 0xFF)) return;
    if (slot == 0) {
        char* mgr = **(char***)((char*)self + 0x118);
        char* cfg = *(char**)(mgr + 0xACC);
        char* bm = mgr + 0x1D8;
        int id = saIsOn(cfg, 0x300);
        if (id) id = *(int*)(cfg + 0x304); else id = -1;
        func_002AD650(bm, id, 0.0f);
    } else {
        func_0029D678(self, 0.0f);
        char* mgr = **(char***)((char*)self + 0x118);
        char* cfg = *(char**)(mgr + 0xACC);
        char* bm = mgr + 0x1D8;
        int id = saIsOn(cfg, 0x360);
        if (id) id = *(int*)(cfg + 0x364); else id = -1;
        func_002AD650(bm, id, 0.0f);
    }
    int off = slot << 2;
    char* tbl = (char*)self + 0x6C70;
    int* p = (int*)(tbl + off);
    if (*p != 0) {
        func_0028BE90(**(void***)((char*)self + 0x118), slot + 8);
        *p = 0;
        *(int*)((char*)self + (slot << 2) + 0x6C78) = -1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00286E20);
#ifdef SKIP_ASM
extern "C" void func_00285BE0(void* self, int a1);
void func_002B6900(void* self);
extern "C" void func_002929D8(void* self);
extern "C" void* func_0028B1C8();
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_002A39E0(void* self);
extern signed char D_00535C11[];
extern void* D_004A52D4;

extern "C" void func_00286E20(void* self)
{
    func_00285BE0(self, 6);
    func_002B6900(D_004A52D4);
    func_002929D8(self);
    *(int*)((char*)self + 0x582C) = *(int*)(*(char**)((char*)func_0028B1C8() + 0xC) + 0x8);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int mode = D_00535C11[0];
    if (mode == 1) {
        goto load;
    }
    if (mode == 2) {
    load:
        func_002A39E0(self);
    }
}
#endif

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

//100%
INCLUDE_ASM("sound/ssxAudio", func_00287108);
#ifdef SKIP_ASM
extern "C" void func_002871B0(void* self);
extern "C" int func_002A4040(void* self);
extern "C" int func_00295028(void* self, int a, int b);
extern "C" void func_00295628(void* self, int a, int b, int c, int d, int e);
extern "C" int func_002A4168(void* self);
extern "C" int func_002A41D8(void* self);
extern void* D_004A28A8;

extern "C" void func_00287108(void* self)
{
    func_002871B0(self);
    if (func_002A4168(self) != 0 || func_002A41D8(self) != 0) {
        if (func_002A4040(self) == 0
            && *(int*)((char*)self + 0x5824) >= *(int*)(*(char**)((char*)D_004A28A8 + 0xC0) + 0x70)
            && func_00295028(self, 0, 0) == 0) {
            func_00295628(self, 0, 0, -1, 0, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_002871B0);
#ifdef SKIP_ASM
extern "C" void func_002948A0(void* self);
extern "C" void func_0029B3C0(void* self, int a1);
extern "C" void func_00287F00(void* self, int a1, float f0, float f1);
extern "C" char* func_0028B1D8();
extern "C" void func_00296E20(void* self, int id);
extern "C" void func_00297438(void* self, int id);
extern "C" void func_002AD5F0(void* p, int idx, int a2, float v);

extern "C" void func_002871B0(void* self)
{
    int i = 0;
    func_002948A0(self);
    func_0029B3C0(self, 0);
    func_00287F00(self, 3, 0.5f, 1.0f);
    func_002B6900(D_004A52D4);
    int n = *(int*)(func_0028B1D8() + 0x78);
    for (; i < n; i++) {
        func_00296E20(self, *(int*)(func_0028B1D8() + (i << 2) + 0x28));
        func_00297438(self, *(int*)(func_0028B1D8() + (i << 2) + 0x28));
    }
    *(int*)((char*)self + 0x6074) = 0;
    *(int*)((char*)self + 0x6078) = 0;
    if (*(int*)((char*)self + 0x59E4) > 0) {
        func_002AD5F0(**(char***)((char*)self + 0x118) + 0x1D8, *(int*)((char*)self + 0x59E0), 1, *(float*)((char*)self + 0x6074));
        *(int*)((char*)self + 0x59E4) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_002872A8);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0028D488(void* self);
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_002A26F0(void* self, int a1, int a2);
void func_002B1758(void* self, int a1);
int func_002B4908(void* mon);
extern char D_00535BC8[];

struct sSaVt72A8 { short delta; short index; int (*fn)(void*, int); };

extern "C" void func_002872A8(void* self)
{
    *(int*)((char*)self + 0x582C) = *(int*)(*(char**)((char*)func_0028B1C8() + 0xC) + 0x8);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    signed char* g = (signed char*)D_00535BC8;
    int mode = g[0x48];
    if (mode != 5 && mode != 6) return;
    char* mon = (char*)self + 0x118;
    if (*(int*)((char*)self + 0x530) != 0) {
        sSaVt72A8* vt = *(sSaVt72A8**)((char*)self + 0x5558);
        vt[4].fn(mon + vt[4].delta, 0);
    } else {
        func_0028D488(self);
        func_0028CF98(self, 0, 0, -1, 0);
    }
    if (func_00295028(self, 0, 0) != 0) return;
    signed char* g2 = (signed char*)D_00535BC8;
    int a = g2[0x4A];
    if (a == 4 || a == 5) {
        if (g2[0x49] != 2) {
            *(int*)((char*)self + 0x5788) = 1;
        } else {
            func_002A26F0(self, 0, 0);
            func_002B1758((char*)self + 0x5560, 0);
        }
    }
    *(int*)((char*)self + 0x5774) = 1;
    *(int*)((char*)self + 0x5778) = func_002B4908(mon);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_002873D8);
#ifdef SKIP_ASM
// PORT: real arity is (self, n); the unit declares func_002873D8(void*) for its callers.
// PORT: g++ `<?` (min) operator, removed in GCC 4.3.
static inline float clamp_73D8(float f, float lo, float hi)
{
    if (f >= lo) {
        return f <? hi;
    }
    return lo;
}

extern "C" float func_002873D8_impl(void* self, int n) __asm__("func_002873D8");
extern "C" float func_002873D8_impl(void* self, int n)
{
    return clamp_73D8(n * 0.09090909361839294f, 0.0f, 1.0f);
}
#endif

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

//100%
INCLUDE_ASM("sound/ssxAudio", func_00288370);
#ifdef SKIP_ASM
extern "C" float func_00288370(void* self, int i, int pct)
{
    if (pct >= 0) {
        return pct * 0.009999999776482582f;
    }
    if (*(int*)((char*)self + 0x6410) != 0) {
        return *(float*)((char*)self + (i << 2) + 0x6440);
    }
    return *(float*)((char*)self + (i << 2) + 0x63E4);
}
#endif

INCLUDE_ASM("sound/ssxAudio", func_002883B0);

//100%
INCLUDE_ASM("sound/ssxAudio", func_002887A8);
#ifdef SKIP_ASM
struct sAudioFade_87A8 {
    char pad[0x63E4];
    float cur[12];     // 0x63E4
    float rate[11];    // 0x6414
    float target[11];  // 0x6440
};

extern "C" void func_002887A8(sAudioFade_87A8* self, int i, int smooth, float target, float time)
{
    self->target[i] = target;
    float cur = self->cur[i];
    if (target != cur) {
        if (smooth != 0) {
            self->rate[i] = (target - cur) / time * 0.01666666753590107f;
        }
    } else {
        self->rate[i] = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_002887F8);
#ifdef SKIP_ASM
extern float D_004A352C;
extern float D_004A3530;
extern float D_004A3534;
extern float D_004A3538;
extern float D_004A353C;
extern float D_004A3540;
extern float D_004A3544;
extern float D_004A3548;
extern float D_004A354C;
extern float D_004A3550;

struct sSaMix87F8 {
    char pad0[0x62C4];
    float chan[11][4];          // 0x62C4 (gain, out, -, -)
    char pad6374[0x63E4 - 0x6374];
    float cur[11];              // 0x63E4
    int dirty;                  // 0x6410
    float delta[11];            // 0x6414
    float target[11];           // 0x6440
};

struct sSaMixOut87F8 {
    char pad0[0x6308];
    float master;               // 0x6308
    char pad630C[0x636C - 0x630C];
    float out[10];              // 0x636C
};

extern "C" void func_002887F8(sSaMix87F8* self)
{
    if (self->dirty == 0) return;
    self->dirty = 0;
    for (int i = 0; i < 11; i++) {
        if (self->delta[i] == 0.0f) continue;
        float v = self->cur[i] + self->delta[i];
        self->cur[i] = v;
        if (self->delta[i] < 0.0f) {
            if (v <= self->target[i]) {
                self->cur[i] = self->target[i];
                self->delta[i] = 0.0f;
            } else {
                self->dirty = 1;
            }
        } else {
            if (self->target[i] <= v) {
                self->cur[i] = self->target[i];
                self->delta[i] = 0.0f;
            } else {
                self->dirty = 1;
            }
        }
        self->chan[i][1] = self->chan[i][0] * self->cur[i];
    }
    sSaMixOut87F8* o = (sSaMixOut87F8*)self;
    float m = o->master;
    o->out[0] = m * D_004A352C;
    o->out[1] = m * D_004A3530;
    o->out[2] = m * D_004A3534;
    o->out[3] = m * D_004A3538;
    o->out[4] = m * D_004A353C;
    o->out[5] = m * D_004A3540;
    o->out[6] = m * D_004A3544;
    o->out[7] = m * D_004A3548;
    o->out[8] = m * D_004A354C;
    o->out[9] = m * D_004A3550;
}
#endif

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

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289470);
#ifdef SKIP_ASM
extern "C" int BXFILE_exists(char* name);
extern "C" void func_002A77F8(void* self, char* name, int size);
extern "C" char* func_002A7B08(void* self);
extern "C" void func_002A7890(void* self, int flags);
extern "C" char* strcpy(char* dst, const char* src);
extern char* D_004A355C;
struct sAudio_9470 { char pad[0x60A4]; char names[20][0x14]; };

extern "C" void func_00289470(void* self)
{
    if (BXFILE_exists(D_004A355C) != 0) {
        char parser[0x810];
        func_002A77F8(parser, D_004A355C, 0x100);
        *(int*)((char*)self + 0x6234) = 0;
        while (*(int*)((char*)self + 0x6234) < 0x14) {
            char* s = func_002A7B08(parser);
            if (s == 0) {
                break;
            }
            strcpy(((sAudio_9470*)self)->names[*(int*)((char*)self + 0x6234)], s);
            *(int*)((char*)self + 0x6234) = *(int*)((char*)self + 0x6234) + 1;
        }
        func_002A7890(parser, 2);
        return;
    }
    *(int*)((char*)self + 0x6234) = 0;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289520);
#ifdef SKIP_ASM
extern "C" void func_002A77F8(void* self, char* name, int size);
extern "C" void func_002A7890(void* self, int flags);
extern "C" int func_002A7C68(void* self, const char* section);
extern "C" void func_002A7A20(void* self);
extern "C" int func_002A7F90(void* self);
extern "C" char* func_002A7DA0(void* self);
extern "C" void func_002A7FF8(void* self, const char* key, char* out);
extern "C" int func_0041AA88(const char* a, const char* b);
extern "C" int func_004165A8(const void* a, const void* b);
extern "C" void func_002B3EB0(void* self);
extern "C" void func_002B3EE8(void* self, const char* name);
void func_002B4060(void* self);
extern char* D_004A355C;
extern char D_004A3608[];

// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern "C" void func_00289520(void* self, const char* name)
{
    char parser[0x810];
    char buf[0x40];
    func_002B3EB0((char*)self + 0x118);
    func_002A77F8(parser, D_004A355C, 0x100);
    if (func_002A7C68(parser, name) != 0) {
        func_002A7A20(parser);
        while (func_002A7F90(parser) == 0) {
            char* key = func_002A7DA0(parser);
            if (func_0041AA88(D_004A3608, key) == 0) {
                func_002A7FF8(parser, D_004A3608, buf);
                func_002B3EE8((char*)self + 0x118, buf);
            }
        }
    }
    *(int*)((char*)self + 0x6238) = 20;
    for (int i = 0; i < 20; i++) {
        if (func_004165A8(((sAudio_9470*)self)->names[i], name) == 0) {
            *(int*)((char*)self + 0x6238) = i;
            break;
        }
    }
    func_002B4060((char*)self + 0x118);
    *(long*)((char*)self + 0x518) = *(long*)((char*)self + 0x510);
    func_002A7890(parser, 2);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289650);
#ifdef SKIP_ASM
// PORT: func_00289680 is defined (void*) but this caller passes (system, msg).
void func_00289680_2(void* self, void* msg) __asm__("func_00289680__FPv");
extern char* D_004A3500;

extern "C" void func_00289650(void* msg)
{
    if (*(int*)msg == 3) {
        func_00289680_2(D_004A3500, msg);
    }
}
#endif

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

//100%
INCLUDE_ASM("sound/ssxAudio", func_002898A8);
#ifdef SKIP_ASM
extern "C" int func_0028D960(void* self);
extern "C" void func_0028D988(void* self, void* rider, void* obj);
extern "C" int func_002899E8(void* self);
extern "C" void func_002899F8(void* self, void* rider, void* obj);
extern "C" void func_00289C98(void* self, int id, void* obj);

struct sSaVec98A8 { float x, y, z, w; } __attribute__((aligned(16)));
struct sSaVt98A8a { short delta; short index; sSaVec98A8* (*fn)(void*); };
struct sSaVt98A8b { short delta; short index; void* (*fn)(void*, float, float, float); };

struct sSaObjs98A8 {
    char pad0[0x5FA0];
    char* c[2];                 // 0x5FA0
    char pad5FA8[0x625C - 0x5FA8];
    char* a[2];                 // 0x625C
    char pad6264[0x6C80 - 0x6264];
    char* b[2];                 // 0x6C80
};

extern "C" void func_002898A8(void* self, char* rider)
{
    unsigned int idx = *(unsigned int*)(rider + 0x870);
    if (idx >= 2) return;
    sSaVec98A8 pos;
    {
        char* o = rider + 0x6C0;
        sSaVt98A8a* vt = *(sSaVt98A8a**)o;
        pos = *vt[5].fn(o + vt[5].delta);
    }
    if (func_0028D960(self) != 0) {
        float k = -99999.0f;
        char* a = ((sSaObjs98A8*)self)->a[idx];
        sSaVt98A8b* va = *(sSaVt98A8b**)(a + 4);
        func_0028D988(self, rider, va[2].fn(a + va[2].delta, pos.x, pos.y, k));
        char* b = ((sSaObjs98A8*)self)->b[idx];
        sSaVt98A8b* vb = *(sSaVt98A8b**)(b + 4);
        // PORT: func_00289C98's unit declaration takes the rider as int
        func_00289C98(self, (int)rider, vb[2].fn(b + vb[2].delta, pos.x, pos.y, k));
    }
    if (func_002899E8(self) != 0) {
        char* c = ((sSaObjs98A8*)self)->c[idx];
        sSaVt98A8b* vc = *(sSaVt98A8b**)(c + 4);
        func_002899F8(self, rider, vc[2].fn(c + vc[2].delta, pos.x, pos.y, -99999.0f));
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_002899E8);
#ifdef SKIP_ASM
extern "C" int func_002899E8(void* self)
{
    return *(int*)((char*)self + 0x608c) == 2;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", func_002899F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);
extern "C" int func_002B49E0(void* monitor);
// func_00285D98 returns a rider pointer here (the unit declares it int).
void* func_00285D98_p(void* self, int which) __asm__("func_00285D98");

struct sSaVt99F8a { short delta; short index; int* (*fn)(void*); };
struct sSaVt99F8b { short delta; short index; int (*fn)(void*, int); };

extern "C" void func_002899F8(void* self, void* rider, void* obj)
{
    if (rider != func_00285D98_p(self, -1)) return;
    if (obj == 0) return;
    sSaVt99F8a* vt = *(sSaVt99F8a**)((char*)obj + 4);
    int id = *vt[4].fn((char*)obj + vt[4].delta);
    if (id == -1) return;
    if (id == 0x1F) {
        if (func_002B49E0((char*)self + 0x118) != 0x65) {
            func_0028CF98(self, 0, 0, 1, 0);
        }
    } else if (id == 0x20) {
        if (func_002B49E0((char*)self + 0x118) != 0x66) {
            func_0028CF98(self, 0, 0, 2, 0);
        }
    } else if (id != 0x21) {
        char* mon = (char*)self + 0x118;
        sSaVt99F8b* vb = *(sSaVt99F8b**)((char*)self + 0x5558);
        vb[4].fn(mon + vb[4].delta, id);
    }
}
#endif

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

//100%
INCLUDE_ASM("sound/ssxAudio", func_0028A058);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_00483B10[];
extern char D_00482820[];
extern void* D_004A3610;

extern "C" void* func_0028A058(void* self)
{
    *(int*)((char*)self + 0x18C) = 6;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x194) = 0;
    *(int*)((char*)self + 0x198) = 0;
    *(void**)((char*)self + 0x1D4) = D_00483B10;
    *(void**)((char*)self + 0x190) = operator_new_tag(0x18, D_00482820, 0, 0);
    *(int*)((char*)self + 0x19C) = 6;
    *(int*)((char*)self + 0x1A4) = 0;
    *(int*)((char*)self + 0x1A8) = 0;
    *(void**)((char*)self + 0x1A0) = operator_new_tag(0x18, D_00482820, 0, 0);
    *(int*)((char*)self + 0x1AC) = 6;
    *(int*)((char*)self + 0x1B4) = 0;
    *(int*)((char*)self + 0x1B8) = 0;
    *(void**)((char*)self + 0x1B0) = operator_new_tag(0x18, D_00482820, 0, 0);
    *(int*)((char*)self + 0x1BC) = 6;
    *(int*)((char*)self + 0x1C4) = 0;
    *(int*)((char*)self + 0x1C8) = 0;
    *(void**)((char*)self + 0x1C0) = operator_new_tag(0x18, D_00482820, 0, 0);
    *(int*)((char*)self + 0x1CC) = 0;
    *(int*)((char*)self + 0x1D0) = 0;
    D_004A3610 = self;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_0028A148);
#ifdef SKIP_ASM
extern "C" void func_0028A230(void*);
void cMemMan_free(void*);
void operator_delete(int*);
extern char D_00483B10[];
extern void* D_004A3610;

struct sAudioSys_A148 {
    char pad0[0x190];
    void* buf190;       // 0x190
    char pad194[0xC];
    void* buf1A0;       // 0x1A0
    char pad1A4[0xC];
    void* buf1B0;       // 0x1B0
    char pad1B4[0xC];
    void* buf1C0;       // 0x1C0
    char pad1C4[0x10];
    void* vtbl;         // 0x1D4
};

extern "C" void func_0028A148(sAudioSys_A148* self, int flags)
{
    self->vtbl = D_00483B10;
    func_0028A230(self);
    D_004A3610 = 0;
    if (self->buf1C0 != 0) {
        cMemMan_free(self->buf1C0);
    }
    if (self->buf1B0 != 0) {
        cMemMan_free(self->buf1B0);
    }
    if (self->buf1A0 != 0) {
        cMemMan_free(self->buf1A0);
    }
    if (self->buf190 != 0) {
        cMemMan_free(self->buf190);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

