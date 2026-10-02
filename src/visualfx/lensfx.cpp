#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* func_002EC418(void* self);
extern const char D_004A3B48[];

//99.23%
INCLUDE_ASM("visualfx/lensfx", cLensFxMan_construct__Fv);
#ifdef SKIP_ASM
void* cLensFxMan_construct()
{
    void* mem = cMemMan_alloc(0x6820, D_004A3B48, 0, 0);
    return func_002EC418(mem);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EC418);
#ifdef SKIP_ASM
extern "C" void func_002E2E18(void* self);
extern void* D_00487F00[];

// PORT: asm label because the unit's forward declaration above has C++ linkage;
// fix that declaration to extern "C" and this can be a plain extern "C" definition.
void* func_002EC418_impl(void* self) __asm__("func_002EC418");
void* func_002EC418_impl(void* self)
{
    func_002E2E18(self);
    *(int*)((char*)self + 0x649c) = (int)(void*)D_00487F00;
    return self;
}
#endif

extern void* D_00487F00[];
extern "C" void* func_002E3060(void*);

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EC450__FPv);
#ifdef SKIP_ASM
void* func_002EC450(void* self)
{
    *(int*)((char*)self + 0x649c) = (int)(void*)D_00487F00;
    return func_002E3060(self);
}
#endif

INCLUDE_ASM("visualfx/lensfx", func_002EC478);

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EC9D0__FPv);
#ifdef SKIP_ASM
void* func_002EC9D0(void* self)
{
    *(int*)self = -1;
    return self;
}
#endif

INCLUDE_ASM("visualfx/lensfx", func_002EC9E0);

INCLUDE_ASM("visualfx/lensfx", func_002ECA68);

//100%
INCLUDE_ASM("visualfx/lensfx", func_002ECAF0);
#ifdef SKIP_ASM
struct sLensEntry {
    int id;
    int a;
    int b;
};

struct sLensFx {
    char pad00[0x10];
    sLensEntry entries[15];
};

extern "C" void func_002ECAF0(sLensFx* self)
{
    int i;
    for (i = 0; i < 15; i++) {
        self->entries[i].id = -1;
    }
}
#endif

INCLUDE_ASM("visualfx/lensfx", func_002ECB28);

INCLUDE_ASM("visualfx/lensfx", func_002ECBC0);

INCLUDE_ASM("visualfx/lensfx", func_002ECC28);

INCLUDE_ASM("visualfx/lensfx", func_002ECCB8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", func_002ECF78);
#ifdef SKIP_ASM
extern "C" void func_002ECCB8(void* self);

extern "C" void func_002ECF78(void* self)
{
    if (*(int*)self != 0) {
        func_002ECCB8(self);
    }
}
#endif

INCLUDE_ASM("visualfx/lensfx", func_002ECFA0);

INCLUDE_ASM("visualfx/lensfx", func_002ED048);

INCLUDE_ASM("visualfx/lensfx", func_002ED1D0);

//100%
INCLUDE_ASM("visualfx/lensfx", func_002ED338);
#ifdef SKIP_ASM
struct sLerpV4 {
    float x, y, z, w;
    sLerpV4() {}
    sLerpV4(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
};

static inline sLerpV4 operator*(const sLerpV4& a, float s)
{
    return sLerpV4(a.x * s, a.y * s, a.z * s, a.w * s);
}

static inline sLerpV4 operator+(const sLerpV4& a, const sLerpV4& b)
{
    return sLerpV4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

struct sLensLerp {
    char pad00[0x28];
    sLerpV4 a;
    sLerpV4 b;
};

extern "C" void func_002ED338(sLensLerp* self, const sLerpV4* a, const sLerpV4* b, float t)
{
    float s = 1.0f - t;
    self->a = sLerpV4(self->a.x * s, self->a.y * s, self->a.z * s, self->a.w * s) + *a * t;
    self->b = sLerpV4(self->b.x * s, self->b.y * s, self->b.z * s, self->b.w * s) + *b * t;
}
#endif

INCLUDE_ASM("visualfx/lensfx", func_002ED490);

INCLUDE_ASM("visualfx/lensfx", func_002EDB20);

INCLUDE_ASM("visualfx/lensfx", func_002EDF00);

// padded past the 8-byte gp-relative threshold so the compiler emits
// absolute lui/lo addressing like the target
struct sPad16 { char x; int pad[3]; };
extern sPad16 D_004FA3C0;

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EDFB8);
#ifdef SKIP_ASM
extern "C" void* func_002EDFB8(int a0)
{
    return (char*)&D_004FA3C0 + a0 * 0xf0;
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EDFD0);
#ifdef SKIP_ASM
extern char D_005047F8[];
extern "C" int func_0038AC50(void*, void*);

extern "C" void func_002EDFD0(void* self)
{
    *(int*)self = func_0038AC50(D_005047F8, self);
    *(int*)((char*)self + 4) = 0x123400;
}
#endif

INCLUDE_ASM("visualfx/lensfx", func_002EE010);

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE070);
#ifdef SKIP_ASM
struct sLensFxVEntry {
    short delta;
    short index;
    float* (*fn)(void*);
};

struct sLensFxObj {
    int unk0;
    sLensFxVEntry* vt; // 0x4
};

struct sLensFxSlot {
    sLensFxObj** p0;   // 0x0
    int pad4[3];
    sLensFxObj** p10;  // 0x10
    sLensFxObj** p14;  // 0x14
    int pad18[2];
    sLensFxObj** p20;  // 0x20
    char pad24[0xF0 - 0x24];
};

extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE070(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[27].fn((char*)obj + obj->vt[27].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE0B8);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE0B8(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[28].fn((char*)obj + obj->vt[28].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE100);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE100(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[29].fn((char*)obj + obj->vt[29].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE148);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE148(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[30].fn((char*)obj + obj->vt[30].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE190);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE190(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[31].fn((char*)obj + obj->vt[31].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE1D8);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE1D8(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[32].fn((char*)obj + obj->vt[32].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE220);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" int func_002EE220(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p14;
    return *(int*)obj->vt[33].fn((char*)obj + obj->vt[33].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE268);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE268(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[34].fn((char*)obj + obj->vt[34].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE2B0);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE2B0(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p14;
    return *obj->vt[35].fn((char*)obj + obj->vt[35].delta);
}
#endif

INCLUDE_ASM("visualfx/lensfx", func_002EE2F8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", func_002EE318);
#ifdef SKIP_ASM
extern "C" void func_002EE010(void* p);
extern "C" void* func_002EF0E8(void);

extern "C" void func_002EE318(void)
{
    func_002EE010(func_002EF0E8());
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", func_002EE340);
#ifdef SKIP_ASM
extern "C" void func_002EE010(void* p);
extern "C" void* func_002EF140(void);

extern "C" void func_002EE340(void)
{
    func_002EE010(func_002EF140());
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/lensfx", func_002EE368);
#ifdef SKIP_ASM
extern "C" void func_002EE010(void* p);
extern "C" void* func_002EF198(void);

extern "C" void func_002EE368(void)
{
    func_002EE010(func_002EF198());
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE3B8);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE3B8(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p0;
    return *obj->vt[6].fn((char*)obj + obj->vt[6].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE400);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE400(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p10;
    return *obj->vt[26].fn((char*)obj + obj->vt[26].delta);
}
#endif

INCLUDE_ASM("visualfx/lensfx", func_002EE448);

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE4C0);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE4C0(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[44].fn((char*)obj + obj->vt[44].delta);
}
#endif

INCLUDE_ASM("visualfx/lensfx", func_002EE508);

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE570);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE570(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[46].fn((char*)obj + obj->vt[46].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE5B8);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE5B8(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[49].fn((char*)obj + obj->vt[49].delta);
}
#endif

INCLUDE_ASM("visualfx/lensfx", func_002EE600);

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE660);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE660(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[52].fn((char*)obj + obj->vt[52].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE6A8);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE6A8(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[50].fn((char*)obj + obj->vt[50].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE6F0);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE6F0(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[48].fn((char*)obj + obj->vt[48].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE738);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE738(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[53].fn((char*)obj + obj->vt[53].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/lensfx", func_002EE780);
#ifdef SKIP_ASM
extern sLensFxSlot D_004FA370[];

extern "C" float func_002EE780(int i)
{
    sLensFxSlot* s = &D_004FA370[i];
    sLensFxObj* obj = *s->p20;
    return *obj->vt[54].fn((char*)obj + obj->vt[54].delta);
}
#endif

