#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_00492EE0[];
extern void* D_00493650[16];

struct cPSPLightMan;
cPSPLightMan* cPSPLightMan_cPSPLightMan(cPSPLightMan* self);

//100%
INCLUDE_ASM("render/lightman", cLightMan_construct__Fv);
#ifdef SKIP_ASM
void* cLightMan_construct()
{
    void* mem = cMemMan_alloc(0x14, D_00492EE0, 0, 0);
    cPSPLightMan_cPSPLightMan((cPSPLightMan*)mem);
    *(void**)mem = D_00493650;
    return mem;
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038DC50__FPv);
#ifdef SKIP_ASM
void func_0038DC50(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038DC58__FPv);
#ifdef SKIP_ASM
void func_0038DC58(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038DC60__FPv);
#ifdef SKIP_ASM
void func_0038DC60(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038DC68__FPv);
#ifdef SKIP_ASM
void func_0038DC68(void* self)
{
}
#endif

INCLUDE_ASM("render/lightman", func_0038DC70);

//100%
INCLUDE_ASM("render/lightman", func_0038DEE8);
#ifdef SKIP_ASM
class cObj0038DEE8 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void* v01(int a);
    virtual void* v02(int a);
};

// PORT: the unit defines func_0038DC50(void*) (an empty stub), but this caller
// passes (void*, int, float); bind the 3-arg call by asm label.
void func_0038DC50_3(void* p, int b, float f) __asm__("func_0038DC50__FPv");

extern "C" void func_0038DEE8(cObj0038DEE8* obj, int a, int b, float f)
{
    func_0038DC50_3(obj->v02(a), b, f);
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038DF38);
#ifdef SKIP_ASM
class cObj0038DF38 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void* v01(int a);
    virtual void* v02(int a);
};

// PORT: the unit defines func_0038DC58(void*) (an empty stub), but this caller
// passes (void*, int, int, float); bind the 4-arg call by asm label.
void func_0038DC58_4(void* p, int b, int c, float f) __asm__("func_0038DC58__FPv");

extern "C" void func_0038DF38(cObj0038DF38* obj, int a, int b, int c, float f)
{
    func_0038DC58_4(obj->v02(a), b, c, f);
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038DF98);
#ifdef SKIP_ASM
class cObj0038DF98 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void* v01(int a);
    virtual void* v02(int a);
};

// PORT: the unit defines func_0038DC60(void*) (an empty stub), but this caller
// passes (void*, int, int, float, int); bind the 5-arg call by asm label.
void func_0038DC60_5(void* p, int b, int c, float f, int d) __asm__("func_0038DC60__FPv");

extern "C" void func_0038DF98(cObj0038DF98* obj, int a, int b, int c, float f, int d)
{
    func_0038DC60_5(obj->v02(a), b, c, f, d);
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038E008);
#ifdef SKIP_ASM
class cObj0038E008 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void* v01(int a);
    virtual void* v02(int a);
};

// PORT: the unit defines func_0038DC68(void*) (an empty stub), but this caller
// passes (void*, int, int, int, float, float, int, float); bind the call by asm label.
void func_0038DC68_8(void* p, int b, int c, int d, float x, float y, int e, float z) __asm__("func_0038DC68__FPv");

extern "C" void func_0038E008(cObj0038E008* obj, int a, int b, int c, int d, float x, float y, int e, float z)
{
    func_0038DC68_8(obj->v02(a), b, c, d, x, y, e, z);
}
#endif

INCLUDE_ASM("render/lightman", func_0038EC40);

INCLUDE_ASM("render/lightman", func_0038EE78);

//100%
INCLUDE_ASM("render/lightman", func_0038F2A8);
#ifdef SKIP_ASM
extern float D_004A4458;

struct sLmCell_F2A8 {
    char data[0x40];
};
extern sLmCell_F2A8 D_00504D80[];

extern "C" sLmCell_F2A8* func_0038F2A8(float* pos)
{
    float d2 = pos[0] * pos[0] + pos[1] * pos[1];
    float d;
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(d) : "f"(d2));
    int i = (int)(D_004A4458 * d);
    return &D_00504D80[i % 8];
}
#endif

INCLUDE_ASM("render/lightman", func_0038F300);

//100%
INCLUDE_ASM("render/lightman", func_0038F460);
#ifdef SKIP_ASM
struct sLightRing {
    int field_0x0;
    int count;        // 0x4
    int next;         // 0x8
    int field_0xC;
    int defSize;      // 0x10
    int result[3];    // 0x14
    unsigned int addr[3]; // 0x20
    int size[3];      // 0x2C
};

extern "C" void func_0038F768(void);

extern "C" int func_0038F460(void* self, unsigned int addr, int size, int flags)
{
    sLightRing* r = (sLightRing*)self;
    if (flags & 4) {
        func_0038F768();
    }
    if (size < 0) {
        size = r->defSize;
    } else {
        size >>= 4;
    }
    int i = r->next++;
    r->next %= r->count;
    r->addr[i] = addr;
    r->size[i] = size;
    return r->result[i];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/lightman", func_0038F4F8);
#ifdef SKIP_ASM
extern "C" int func_0038F460(void* self, unsigned int addr, int size, int flags);
extern "C" void func_00371D10(unsigned int madr, int sadr, int qwc, int flags);

extern "C" int func_0038F4F8(void* self, unsigned int addr, int size, int flags)
{
    int r = func_0038F460(self, addr, size, 0);
    func_00371D10(addr, r, size >> 4, flags);
    return r;
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038F598);
#ifdef SKIP_ASM
struct sLightDma_F598 {
    int field_0x0;
    int count;              // 0x4
    int next;               // 0x8
    int cur;                // 0xC
    int defSize;            // 0x10
    int result[3];          // 0x14
    unsigned int addr[3];   // 0x20
};

extern "C" void func_0038F768(void);
extern "C" void func_0038F6A8(void* self);
extern "C" void func_00371DD8(int sadr, unsigned int madr, int qwc, int flags);

// PORT: the unit declares func_0038F598 as `void (sLmStack*, int, int, int)`; the body takes
// (self, size, flags) and returns the buffer address, so it is bound by asm label.
unsigned int func_0038F598_impl(sLightDma_F598* self, int size, int flags) __asm__("func_0038F598");

unsigned int func_0038F598_impl(sLightDma_F598* self, int size, int flags)
{
    if (size == 0) {
        if (flags & 6) {
            func_0038F768();
        }
        unsigned int r = self->addr[self->cur];
        func_0038F6A8(self);
        return r;
    }
    size >>= 4;
    // PORT: EE sync (memory barrier before kicking the DMA)
    __asm__ __volatile__("sync");
    unsigned int* addrs = self->addr;
    func_00371DD8(self->result[self->cur], *(unsigned int*)((char*)addrs + (self->cur << 2)), size, flags);
    int cur = self->cur;
    unsigned int r = *(unsigned int*)((char*)addrs + (cur << 2)) + (size << 4);
    self->cur = (cur + 1) % self->count;
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/lightman", func_0038F668);
#ifdef SKIP_ASM
struct sLmStack {
    char pad_0x00[0xC];
    int depth;          // 0x0C
    char pad_0x10[4];
    int marks[1];       // 0x14
};

extern "C" void func_0038F598(sLmStack* self, int size, int arg, int base);

extern "C" void func_0038F668(sLmStack* self, int end, int arg)
{
    int base = self->marks[self->depth];
    int size = end - base;
    func_0038F598(self, size + (-size & 0xF), arg, base);
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038F6A8);
#ifdef SKIP_ASM
extern "C" void func_0038F6A8(void* self)
{
    *(int*)((char*)self + 0xC) = (*(int*)((char*)self + 0xC) + 1) % *(int*)((char*)self + 0x4);
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038F708);
#ifdef SKIP_ASM
extern int D_004A5B84;
extern "C" void func_00371D10(unsigned int madr, int sadr, int qwc, int flags);

extern "C" int func_0038F708(unsigned int madr, int size, int flags)
{
    func_00371D10(madr, D_004A5B84, size >> 4, flags);
    return D_004A5B84;
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038F738);
#ifdef SKIP_ASM
extern int D_004A5B84;
extern "C" void func_00371DD8(int sadr, unsigned int madr, int qwc, int flags);

extern "C" void func_0038F738(unsigned int madr, int size, int flags)
{
    func_00371DD8(D_004A5B84, madr, size >> 4, flags);
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038F768);
#ifdef SKIP_ASM
// PORT: polls PS2 hardware register 0x1000D000 directly; needs a platform shim.
extern "C" void func_0038F768(void)
{
    while (*(volatile int*)0x1000D000 & 0x100) {
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038F7B0);
#ifdef SKIP_ASM
// PORT: polls PS2 hardware register 0x1000D400 directly; needs a platform shim.
extern "C" void func_0038F7B0(void)
{
    while (*(volatile int*)0x1000D400 & 0x100) {
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038F7F8__FPv);
#ifdef SKIP_ASM
void func_0038F7F8(void* self)
{
}
#endif

extern "C" void* func_002BB6B8(void* self);

//100%
INCLUDE_ASM("render/lightman", func_0038F800__FPv);
#ifdef SKIP_ASM
void* func_0038F800(void* self)
{
    return func_002BB6B8(self);
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038F930);
#ifdef SKIP_ASM
extern int D_0044BDE0[];

extern "C" int func_0038F930(void* self, unsigned char* src, unsigned char* dst)
{
    unsigned int n = 0;
    unsigned int i;
    for (i = 0; i < 4; i++) {
        unsigned int k = (i & 1) * 64;
        unsigned int j;
        for (j = 0; j < 16; j++) {
            unsigned int m;
            for (m = 0; m < 4; m++) {
                dst[n] = src[D_0044BDE0[k]];
                k++;
                n++;
            }
        }
        src += 0x40;
    }
    return 0;
}
#endif

INCLUDE_ASM("render/lightman", func_0038FC48);

INCLUDE_ASM("render/lightman", func_00390198);

//100%
INCLUDE_ASM("render/lightman", func_00390458);
#ifdef SKIP_ASM
extern float D_004A45E4;
extern float D_004A45E8;
extern float D_004A45EC;
extern float D_004A45F0;
extern float D_004A45F4;
extern float D_004A45F8;

struct sLmEntry_0458 {
    float v[6];
};

struct sLmOwner_0458 {
    char pad[0x6CA4];
    sLmEntry_0458 entries[1];
};

extern "C" void func_00390458(void* self, int idx)
{
    char* e = (char*)&((sLmOwner_0458*)self)->entries[idx];
    *(float*)(e + 0x0) = D_004A45E4;
    *(float*)(e + 0x4) = D_004A45E8;
    *(float*)(e + 0x8) = D_004A45EC;
    *(float*)(e + 0xC) = D_004A45F0;
    *(float*)(e + 0x10) = D_004A45F4;
    *(float*)(e + 0x14) = D_004A45F8;
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_003904A0);
#ifdef SKIP_ASM
extern int D_004A45D8;
extern int D_004A45DC;
extern void* D_004A4474;

extern "C" void func_003905E8(void* self, int idx, int* mark);
// PORT: func_0038F668 is declared void in this unit, but it returns its tail call's
// value and this caller uses it; bind an int-returning view to the same symbol.
extern "C" int func_0038F668_r(void* self, int end, int arg) __asm__("func_0038F668");
extern "C" void func_00368138(void* self, void* pkt);

extern "C" void func_003904A0(void* self, int idx, void* pkt, int* addr)
{
    if (D_004A45D8 == 0) {
        return;
    }
    if (D_004A45DC != 0) {
        func_00390458(self, idx);
    }
    float* e = ((sLmOwner_0458*)self)->entries[idx].v;
    if (e[0] == 1.0f && e[1] == 1.0f && e[2] == 1.0f && e[3] == 0.0f && e[4] == 0.0f && e[5] == 0.0f) {
        return;
    }
    void* ring = D_004A4474;
    int mark = func_0038F460(ring, *addr, -1, 0);
    func_003905E8(self, idx, &mark);
    *addr = func_0038F668_r(ring, mark, 4);
    func_00368138(*(void**)((char*)self + 0x18F4), pkt);
}
#endif

INCLUDE_ASM("render/lightman", func_003905E8);

//100%
INCLUDE_ASM("render/lightman", func_00390C20);
#ifdef SKIP_ASM
extern char* D_004A5B80;
extern int D_004A44C0;
extern int D_004A44C8;

struct sLmVEntry_0C20 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00390C20()
{
    char* mgr = D_004A5B80;
    sLmVEntry_0C20* vt = *(sLmVEntry_0C20**)(mgr + 0x10D8);
    vt[16].fn(mgr + vt[16].delta, D_004A44C0);
    if (D_004A44C8 != 0) {
        D_004A44C8 = 0;
    }
}
#endif

INCLUDE_ASM("render/lightman", func_00390C60);

//100%
INCLUDE_ASM("render/lightman", func_00390EC8);
#ifdef SKIP_ASM
extern int D_004A460C;
extern int D_004A4698;
extern int D_004A469C;
extern float D_004A46B0;
extern int D_004A46B4;

extern "C" void func_00390EC8(float t)
{
    if (D_004A460C != 0) {
        D_004A4698 = 0;
        D_004A469C = 0;
        D_004A46B0 = t;
        D_004A46B4 = (int)(t * 0.0018072288949042559f);
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_00390EF8);
#ifdef SKIP_ASM
extern int D_004A460C;
extern int D_004A46B4;

extern "C" int func_00390EF8()
{
    if (D_004A460C != 0 && D_004A46B4 == 0) {
        D_004A46B4 = -1;
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("render/lightman", func_00390F20);

//100%
INCLUDE_ASM("render/lightman", func_003912A8);
#ifdef SKIP_ASM
extern char D_0043BD00[];
extern "C" void func_00424020(int);

// PORT: kicks a DMA chain on channel 0 (VIF0) through the PS2 hardware registers at
// 0x10008000 (CHCR), 0x10008020 (QWC) and 0x10008030 (TADR); needs a platform shim.
extern "C" void func_003912A8(void)
{
    while (*(volatile int*)0x10008000 & 0x100) {
    }
    *(volatile int*)0x10008030 = (int)D_0043BD00;
    *(volatile int*)0x10008020 = 0;
    *(volatile int*)0x10008000 = 0x104;
    while (*(volatile int*)0x10008000 & 0x100) {
    }
    func_00424020(0);
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_00391360);
#ifdef SKIP_ASM
extern char D_0043CCA0[];
extern "C" void func_00424020(int);

// PORT: kicks a DMA chain on channel 0 (VIF0) through the PS2 hardware registers at
// 0x10008000 (CHCR), 0x10008020 (QWC) and 0x10008030 (TADR); needs a platform shim.
extern "C" void func_00391360(void)
{
    while (*(volatile int*)0x10008000 & 0x100) {
    }
    *(volatile int*)0x10008030 = (int)D_0043CCA0;
    *(volatile int*)0x10008020 = 0;
    *(volatile int*)0x10008000 = 0x104;
    while (*(volatile int*)0x10008000 & 0x100) {
    }
    func_00424020(0);
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_00391418);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (lqc2 x16 into vf1-vf16, then vcallmsr of the
// microprogram at VU0 micro-memory address 0); the PC port needs a C version.
extern "C" void func_00391418(void* self, void* m)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "lqc2      $vf5, 0x40(%0)\n"
        "lqc2      $vf6, 0x50(%0)\n"
        "lqc2      $vf7, 0x60(%0)\n"
        "lqc2      $vf8, 0x70(%0)\n"
        "lqc2      $vf9, 0x80(%0)\n"
        "lqc2      $vf10, 0x90(%0)\n"
        "lqc2      $vf11, 0xA0(%0)\n"
        "lqc2      $vf12, 0xB0(%0)\n"
        "lqc2      $vf13, 0xC0(%0)\n"
        "lqc2      $vf14, 0xD0(%0)\n"
        "lqc2      $vf15, 0xE0(%0)\n"
        "lqc2      $vf16, 0xF0(%0)\n"
        "lui       $2, 0\n"
        "addiu     $2, $2, 0\n"
        "srl       $2, $2, 3\n"
        "ctc2.ni   $2, $vi27\n"
        "vnop\n"
        "vnop\n"
        "vcallmsr  $vi27\n"
        ".set pop\n"
        :
        : "r"(m)
        : "$2", "memory");
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_00391480);
#ifdef SKIP_ASM
struct sQuad16 {
    int w[4];
} __attribute__((aligned(16)));

extern "C" void func_00391480(void* self, sQuad16* dst)
{
    sQuad16* src = (sQuad16*)0x11004200;
    int i;
    for (i = 0; i < 10; i++) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
        dst[6] = src[6];
        dst[7] = src[7];
        dst[8] = src[8];
        dst[9] = src[9];
        src += 10;
        dst += 10;
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_003914F8);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 integer register read (vi1); the PC port needs a C fallback.
extern "C" int func_003914F8(void)
{
    int r;
    __asm__ __volatile__("cfc2.i %0, $vi1" : "=r"(r));
    return r;
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_003915E8);
#ifdef SKIP_ASM
struct sLmVec2_15E8 {
    float x, y;

    sLmVec2_15E8() {}
    sLmVec2_15E8(float a, float b)
    {
        x = a;
        y = b;
    }
};

struct sLmVec4_15E8 {
    float x, y, z, w;

    sLmVec4_15E8() {}
    sLmVec4_15E8(float a, float b, float c, float d)
    {
        x = a;
        y = b;
        z = c;
        w = d;
    }
};

struct sLmLight_15E8 {
    char pad_0x0[0x8];
    int field_0x8;          // 0x8
    char pad_0xc[0x1C];
    sLmVec2_15E8 v28;       // 0x28
    sLmVec2_15E8 v30;       // 0x30
    sLmVec2_15E8 v38;       // 0x38
    sLmVec4_15E8 v40;       // 0x40
    sLmVec4_15E8 v50;       // 0x50
    char pad_0x60[0x4];
    unsigned char b64;      // 0x64
};

extern "C" void func_00391708(void* self, const char* name, int flags);

extern "C" sLmLight_15E8* func_003915E8(sLmLight_15E8* self, const char* name, int flags)
{
    self->field_0x8 = 0;
    self->v28 = sLmVec2_15E8(0.0f, 0.0f);
    self->v30 = sLmVec2_15E8(1.0f, 1.0f);
    self->v38 = self->v30;
    {
        sLmVec4_15E8 t;
        t.x = 1.0f;
        t.y = 1.0f;
        t.z = 1.0f;
        t.w = 1.0f;
        self->v40 = t;
    }
    self->v50 = sLmVec4_15E8(1.0f, 0.0f, 0.0f, 0.0f);
    self->b64 = 0;
    func_00391708(self, name, flags);
    return self;
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_003916C0);
#ifdef SKIP_ASM
extern "C" void func_003919E8(void* self);
void operator_delete(int* ptr);

extern "C" void func_003916C0(int* self, int flags)
{
    func_003919E8(self);
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_00391708);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* FILE_load(const char* name, int flags);
extern "C" void cFont_linkFont(void* self, void* data, int flags);
void cMemMan_free(void*);
extern const char D_00492F30[];

extern "C" void func_00391708(void* self, const char* name, int flags)
{
    void* data;

    sprintf((char*)self + 0x64, D_00492F30, name);
    data = FILE_load(name, flags ^ 0x100);
    cFont_linkFont(self, data, flags);
    if (data != 0) {
        cMemMan_free(data);
    }
}
#endif

