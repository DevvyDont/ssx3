#include "common.h"

INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_camaction);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTActionBoundedCam);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int get_float(void* reader, void* dst);
extern "C" int get_uint(void* reader, void* dst);
// get_t3Vector returns the byte count read (the unit declares it void*); bind an int-returning alias.
extern "C" int get_t3Vector_n(void* reader, void* dst) __asm__("get_t3Vector");

extern "C" int get_cCTActionBoundedCam(void* reader, char* obj)
{
    int n;
    *(int*)obj = 1;
    n = get_float(reader, obj + 0xC);
    n += get_float(reader, obj + 0x10);
    n += get_float(reader, obj + 0x14);
    n += get_float(reader, obj + 0x18);
    n += get_float(reader, obj + 0x1C);
    n += get_float(reader, obj + 0x20);
    n += get_uint(reader, obj + 0x24);
    n += get_t3Vector_n(reader, obj + 0x28);
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTActionSwitchCam);
#ifdef SKIP_ASM
extern "C" int get_float(void* reader, void* dst);
extern "C" int get_uint(void* reader, void* dst);

extern "C" int get_cCTActionSwitchCam(void* reader, void* dst)
{
    *(int*)dst = 0;
    int n = get_float(reader, (char*)dst + 0xC);
    return n + get_uint(reader, (char*)dst + 0x8);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTActionSpline);
#ifdef SKIP_ASM
extern "C" int get_cCTActionSpline(void* reader, void* dst)
{
    *(int*)dst = 2;
    int n = get_float(reader, (char*)dst + 0xC);
    n += get_float(reader, (char*)dst + 0x10);
    n += get_float(reader, (char*)dst + 0x14);
    n += get_float(reader, (char*)dst + 0x18);
    return n + get_float(reader, (char*)dst + 0x1C);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTActionNone__FPvPi);
#ifdef SKIP_ASM
void* get_cCTActionNone(void* unused, int* outType)
{
    *outType = 3;
    return 0;
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_camboundobj);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTBoundObjEllipse);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// get_t3Vector returns the byte count read (the unit declares it void*); bind an int-returning alias.
extern "C" int get_t3Vector_n(void* reader, void* dst) __asm__("get_t3Vector");

class cCTBoundObjEllK2 {
public:
    char pad_0x00[0x24];
    // vptr at 0x24; int at 0x28
    virtual void* center();
    virtual void* sizeX();
    virtual void* sizeY();
    virtual void* sizeZ();
    virtual void* axis();
};

extern "C" int get_cCTBoundObjEllipse(void* reader, cCTBoundObjEllK2* obj)
{
    int n;
    *(int*)((char*)obj + 0x28) = 0;
    n = get_t3Vector_n(reader, obj->center());
    n += get_t3Vector_n(reader, obj->axis());
    n += get_float(reader, obj->sizeX());
    n += get_float(reader, obj->sizeY());
    n += get_float(reader, obj->sizeZ());
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTBoundObjBox);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// get_t3Vector returns the byte count read (the unit declares it void*); bind an int-returning alias.
extern "C" int get_t3Vector_n(void* reader, void* dst) __asm__("get_t3Vector");

class cCTBoundObjBoxK2 {
public:
    char pad_0x00[0x24];
    // vptr at 0x24; int at 0x28
    virtual void* center();
    virtual void* sizeX();
    virtual void* sizeY();
    virtual void* sizeZ();
    virtual void* axis();
};

extern "C" int get_cCTBoundObjBox(void* reader, cCTBoundObjBoxK2* obj)
{
    int n;
    *(int*)((char*)obj + 0x28) = 1;
    n = get_t3Vector_n(reader, obj->center());
    n += get_t3Vector_n(reader, obj->axis());
    n += get_float(reader, obj->sizeX());
    n += get_float(reader, obj->sizeY());
    n += get_float(reader, obj->sizeZ());
    return n;
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTBoundObjLine);

struct sBoundObjVTable {
    char pad_0x00[0x8];
    short field_0x8;
    char pad_0xA[2];
    void* (*fn)(void*); // 0xC
};

struct sBoundObjPoint {
    char pad_0x00[0x24];
    sBoundObjVTable* vtable;
    int field_0x28;
};

extern "C" void* get_t3Vector(void* self, void* v);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTBoundObjPoint__FPvP14sBoundObjPoint);
#ifdef SKIP_ASM
void* get_cCTBoundObjPoint(void* self, sBoundObjPoint* obj)
{
    obj->field_0x28 = 3;
    void* result = obj->vtable->fn((char*)obj + obj->vtable->field_0x8);
    return get_t3Vector(self, result);
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_camspline);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00171FA8);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001721C0);
#ifdef SKIP_ASM
extern "C" float func_001731C0(float* v);

struct sCTVec3K2 {
    float x, y, z;
    sCTVec3K2() {}
    sCTVec3K2(float a, float b, float c) : x(a), y(b), z(c) {}
};

static inline sCTVec3K2 operator-(const sCTVec3K2& a, float s)
{
    return sCTVec3K2(a.x - s, a.y - s, a.z - s);
}

static inline sCTVec3K2 operator+(const sCTVec3K2& a, float s)
{
    return sCTVec3K2(a.x + s, a.y + s, a.z + s);
}

struct sCTVec4K2 {
    float x, y, z, w;
    sCTVec4K2() {}
    sCTVec4K2(const sCTVec3K2& v, float w_) : x(v.x), y(v.y), z(v.z), w(w_) {}
} __attribute__((aligned(16)));

struct sCTBoxK2 {
    sCTVec4K2 min;
    sCTVec4K2 max;
    sCTBoxK2(const sCTVec4K2& a, const sCTVec4K2& b) : min(a), max(b) {}
};

// Bounding box of a trigger volume: centre +- the largest half-extent.
extern "C" sCTBoxK2 func_001721C0(char* obj)
{
    float r = func_001731C0((float*)(obj + 0x18));
    sCTVec3K2 rv(r, r, r);
    const sCTVec3K2& c = *(sCTVec3K2*)obj;
    return sCTBoxK2(sCTVec4K2(c - r, 1.0f), sCTVec4K2(c + r, 1.0f));
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00172278);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00172840);
#ifdef SKIP_ASM
extern "C" float func_001731C0(float* v);

struct sCTVec3K2b {
    float x, y, z;
    sCTVec3K2b() {}
    sCTVec3K2b(float a, float b, float c) : x(a), y(b), z(c) {}
};

static inline sCTVec3K2b operator-(const sCTVec3K2b& a, float s)
{
    return sCTVec3K2b(a.x - s, a.y - s, a.z - s);
}

static inline sCTVec3K2b operator+(const sCTVec3K2b& a, float s)
{
    return sCTVec3K2b(a.x + s, a.y + s, a.z + s);
}

struct sCTVec4K2b {
    float x, y, z, w;
    sCTVec4K2b() {}
    sCTVec4K2b(const sCTVec3K2b& v, float w_) : x(v.x), y(v.y), z(v.z), w(w_) {}
} __attribute__((aligned(16)));

struct sCTBoxK2b {
    sCTVec4K2b min;
    sCTVec4K2b max;
    sCTBoxK2b(const sCTVec4K2b& a, const sCTVec4K2b& b) : min(a), max(b) {}
};

// Bounding box of a trigger volume: centre +- the largest half-extent.
extern "C" sCTBoxK2b func_00172840(char* obj)
{
    float r = func_001731C0((float*)(obj + 0x18));
    sCTVec3K2b rv(r, r, r);
    const sCTVec3K2b& c = *(sCTVec3K2b*)obj;
    return sCTBoxK2b(sCTVec4K2b(c - r, 1.0f), sCTVec4K2b(c + r, 1.0f));
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001728F8);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_camvolume);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTVolumeEllipse);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// get_t3Vector returns the byte count read (the unit declares it void*); bind an int-returning alias.
extern "C" int get_t3Vector_n(void* reader, void* dst) __asm__("get_t3Vector");

class cCTVolumeEllK2 {
public:
    char pad_0x00[0x24];
    // vptr at 0x24; int at 0x28
    virtual void* center();
    virtual void* sizeX();
    virtual void* sizeY();
    virtual void* sizeZ();
    virtual void* axis();
};

extern "C" int get_cCTVolumeEllipse(void* reader, cCTVolumeEllK2* obj)
{
    int n;
    *(int*)((char*)obj + 0x28) = 0;
    n = get_t3Vector_n(reader, obj->center());
    n += get_t3Vector_n(reader, obj->axis());
    n += get_float(reader, obj->sizeX());
    n += get_float(reader, obj->sizeY());
    n += get_float(reader, obj->sizeZ());
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", get_cCTVolumeBox);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// get_t3Vector returns the byte count read (the unit declares it void*); bind an int-returning alias.
extern "C" int get_t3Vector_n(void* reader, void* dst) __asm__("get_t3Vector");

class cCTVolumeBoxK2 {
public:
    char pad_0x00[0x24];
    // vptr at 0x24; int at 0x28
    virtual void* center();
    virtual void* sizeX();
    virtual void* sizeY();
    virtual void* sizeZ();
    virtual void* axis();
};

extern "C" int get_cCTVolumeBox(void* reader, cCTVolumeBoxK2* obj)
{
    int n;
    *(int*)((char*)obj + 0x28) = 1;
    n = get_t3Vector_n(reader, obj->center());
    n += get_t3Vector_n(reader, obj->axis());
    n += get_float(reader, obj->sizeX());
    n += get_float(reader, obj->sizeY());
    n += get_float(reader, obj->sizeZ());
    return n;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001731C0);
#ifdef SKIP_ASM
extern "C" float func_001731C0(float* v)
{
    float x = v[0];
    float y = v[1];
    float r = v[2];
    if (y < x) {
        if (r < x) {
            return x;
        }
    } else if (r < y) {
        r = y;
    }
    return r;
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00173208);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001732B8);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// PORT: the unit declares func_001732B8 as `void* (void*)` (callers pass the
// flags through in $a1); the real body takes (self, flags). Bound by asm label.
void func_001732B8_impl(void* self, int flags) __asm__("func_001732B8");

// deleting destructor: reset the vtable, free when bit 0 of flags is set
void func_001732B8_impl(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001732E8);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00173678);
#ifdef SKIP_ASM
struct sVE173678a {
    short delta;
    short index;
    void* (*fn)(void*);
};

struct sVE173678b {
    short delta;
    short index;
    void* (*fn)(void*, void*);
};

extern "C" void* func_00173678(void* self)
{
    sVE173678b* vt = *(sVE173678b**)((char*)self + 0x10);
    char* o = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sVE173678a* vt2 = *(sVE173678a**)o;
    return vt[3].fn((char*)self + vt[3].delta, vt2[7].fn(o + vt2[7].delta));
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001736E0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00173E40);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00174190);
#ifdef SKIP_ASM
extern void* D_0045C458[];
extern "C" void* func_00173208(void* self);

extern "C" void* func_00174190(void* self)
{
    func_00173208(self);
    *(int*)((char*)self + 0xC) = 0x5B;
    *(void***)((char*)self + 0x10) = D_0045C458;
    *(int*)((char*)self + 0x80) = 0;
    *(int*)((char*)self + 0x74) = 0;
    return self;
}
#endif

extern void* D_0045C458[];
extern "C" void* func_001732B8(void*);

//99.5%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001741D8__FPv);
#ifdef SKIP_ASM
void* func_001741D8(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045C458;
    return func_001732B8(self);
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00174200);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001744B0);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001747A0);
#ifdef SKIP_ASM
struct sVE1747A0a {
    short delta;
    short index;
    void* (*fn)(void*);
};

struct sVE1747A0b {
    short delta;
    short index;
    void* (*fn)(void*, void*);
};

extern "C" void* func_001747A0(void* self)
{
    sVE1747A0b* vt = *(sVE1747A0b**)((char*)self + 0x10);
    char* o = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sVE1747A0a* vt2 = *(sVE1747A0a**)o;
    return vt[3].fn((char*)self + vt[3].delta, vt2[7].fn(o + vt2[7].delta));
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00174848);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00175A20);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176328);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00176328(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176358);
#ifdef SKIP_ASM
extern "C" void* func_00176358(void* self, void* a1)
{
    *(cQuad128*)self = *(cQuad128*)((char*)a1 + 0x20);
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176368);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00176368(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176398);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00176398(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001763C8);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_001763C8(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001763F8);
#ifdef SKIP_ASM
extern void* D_0045BDE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_001763F8(void* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_0045BDE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176440__FPv);
#ifdef SKIP_ASM
void* func_00176440(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176448__FPv);
#ifdef SKIP_ASM
void* func_00176448(void* self)
{
    return (char*)self + 0xC;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176450__FPv);
#ifdef SKIP_ASM
void* func_00176450(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176458__FPv);
#ifdef SKIP_ASM
void* func_00176458(void* self)
{
    return (char*)self + 0x14;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176460__FPv);
#ifdef SKIP_ASM
void* func_00176460(void* self)
{
    return (char*)self + 0x18;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001765F8__FPv);
#ifdef SKIP_ASM
void func_001765F8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176808);
#ifdef SKIP_ASM
struct sVec4A16 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern "C" void func_00176808(float* v, sVec4A16* out)
{
    sVec4A16 t;
    t.x = v[0];
    t.y = v[1];
    t.z = v[2];
    t.w = 1.0f;
    *out = t;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176840);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_00176840(void* self)
{
    int* p = *(int**)((char*)self + 0x8);
    if (p != 0) {
        operator_delete(p);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176868);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_00176868(void* self)
{
    int* p = *(int**)((char*)self + 0x8);
    if (p != 0) {
        operator_delete(p);
    }
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176890);

extern "C" void* func_00175A20(int, int);

//99.38%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176A28__FPv);
#ifdef SKIP_ASM
void* func_00176A28(void* self)
{
    return func_00175A20(1, 0xffff);
}
#endif

extern "C" void* func_00175A20(int, int);

//99.38%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176A48__FPv);
#ifdef SKIP_ASM
void* func_00176A48(void* self)
{
    return func_00175A20(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176A68);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045CAB0[];

extern "C" void* func_00176A68(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x3C;
    *(void***)((char*)self + 0x10) = D_0045CAB0;
    return self;
}
#endif

extern void* D_0045CAB0[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176AA8__FPv);
#ifdef SKIP_ASM
void* func_00176AA8(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045CAB0;
    return func_00162458(self);
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176AD0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176AE0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176B10);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176CE0);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176D68);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045CA38[];

extern "C" void* func_00176D68(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x3D;
    *(void***)((char*)self + 0x10) = D_0045CA38;
    return self;
}
#endif

extern void* D_0045CA38[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176DA8__FPv);
#ifdef SKIP_ASM
void* func_00176DA8(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045CA38;
    return func_00162458(self);
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176DD0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176DE0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176E10);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00176FE0);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177068);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045C9C0[];

extern "C" void* func_00177068(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x3E;
    *(void***)((char*)self + 0x10) = D_0045C9C0;
    return self;
}
#endif

extern void* D_0045C9C0[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001770A8__FPv);
#ifdef SKIP_ASM
void* func_001770A8(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045C9C0;
    return func_00162458(self);
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001770D0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001770E0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177110);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001772E0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177368);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001774C0);

extern void* D_0045CB28[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177520__FPv);
#ifdef SKIP_ASM
void* func_00177520(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045CB28;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177548__FPv);
#ifdef SKIP_ASM
float func_00177548(void* self)
{
    return 0.0f;
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177558);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177588);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177650);

extern "C" void* func_00177650(int, int);

//99.38%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177E30__FPv);
#ifdef SKIP_ASM
void* func_00177E30(void* self)
{
    return func_00177650(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177E50);
#ifdef SKIP_ASM
extern "C" void* func_00162318(void* self);
extern void* D_0045CCC8[];

extern "C" void* func_00177E50(void* self)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x42;
    *(void***)((char*)self + 0x10) = D_0045CCC8;
    return self;
}
#endif

extern void* D_0045CCC8[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177E90__FPv);
#ifdef SKIP_ASM
void* func_00177E90(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045CCC8;
    return func_00162458(self);
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177EB8);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177EC8);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177EF8);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00177FC0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178208);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001783E0);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178520);
#ifdef SKIP_ASM
struct func_00178520_sVec4 { float x, y, z, w; } __attribute__((aligned(16)));
extern void* D_0045CC50[];
extern "C" void* func_00162318(void* self);

extern "C" void* func_00178520(void* self, func_00178520_sVec4* a, func_00178520_sVec4* b)
{
    func_00162318(self);
    *(int*)((char*)self + 0xC) = 0x43;
    *(void***)((char*)self + 0x10) = D_0045CC50;
    *(func_00178520_sVec4*)((char*)self + 0x390) = *a;
    *(func_00178520_sVec4*)((char*)self + 0x3A0) = *b;
    return self;
}
#endif

extern void* D_0045CC50[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178588__FPv);
#ifdef SKIP_ASM
void* func_00178588(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045CC50;
    return func_00162458(self);
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001785B0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001785C0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001785F0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178758);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001787E0);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178938);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_001789E8);

extern void* D_0045CBD8[];
extern "C" void* func_00162458(void*);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178B70__FPv);
#ifdef SKIP_ASM
void* func_00178B70(void* self)
{
    *(int*)((char*)self + 0x10) = (int)(void*)D_0045CBD8;
    return func_00162458(self);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178B98__FPv);
#ifdef SKIP_ASM
float func_00178B98(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178BA8__FPvT0);
#ifdef SKIP_ASM
void func_00178BA8(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178BB0);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178E90);
#ifdef SKIP_ASM
extern "C" void func_00166C60(void* self, int id);
extern "C" void func_00166550(void* self, float a, float b);

struct sCamVEntryIntA {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sCamVEntryVoidA {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00178E90(void* self, int id)
{
    char* obj = *(char**)(*(char**)((char*)self + 0x30) + 0x4) + 0x6C0;
    sCamVEntryIntA* vt = *(sCamVEntryIntA**)obj;
    if (id == vt[7].fn(obj + vt[7].delta))
    {
        func_00166C60(self, id);
        func_00166550(self, 100.0f, 100.0f);
        sCamVEntryVoidA* vt2 = *(sCamVEntryVoidA**)((char*)self + 0x10);
        vt2[5].fn((char*)self + vt2[5].delta);
    }
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00178F58);

extern "C" void* func_00178F58(int, int);

//99.38%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00179738__FPv);
#ifdef SKIP_ASM
void* func_00179738(void* self)
{
    return func_00178F58(1, 0xffff);
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00179798);

extern "C" void* func_00179798(int, int);

//99.38%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00179FA8__FPv);
#ifdef SKIP_ASM
void* func_00179FA8(void* self)
{
    return func_00179798(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_00179FC8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00144CC0(void* iface, int id);

extern "C" int func_00179FC8(void* self, int id)
{
    if (func_00144CC0(cBE_getInterface_Fv(cBE_getBE(), 0), id) == 1) {
        return 1;
    }
    if (id >= 0xE && id <= 0x10) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_0017A028);
#ifdef SKIP_ASM
struct func_0017A028_sEntry {
    int active;
    char pad_0x4[0x88];
};

struct func_0017A028_sMgr {
    int count;
    char pad_0x4[0x88];
    func_0017A028_sEntry entries[1];
};

extern "C" int func_0017A028(func_0017A028_sMgr* self, int i)
{
    if (*(int*)((char*)self + 0x8C08) == 0) {
        return -1;
    }
    i = (i + 1) % self->count;
    while (self->entries[i].active == 0) {
        i = (i + 1) % self->count;
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_0017A0B8);
#ifdef SKIP_ASM
extern "C" int func_0017A0B8(func_0017A028_sMgr* self, int i)
{
    if (*(int*)((char*)self + 0x8C08) == 0) {
        return -1;
    }
    i = (i + self->count - 1) % self->count;
    while (self->entries[i].active == 0) {
        i = (i + self->count - 1) % self->count;
    }
    return i;
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_0017A158);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_0017A220);
#ifdef SKIP_ASM
extern "C" int func_0017A158(int* self, int i, void* key);

// Circular search: first index after `start` for which func_0017A158 matches.
extern "C" int func_0017A220(int* self, int start, void* key)
{
    int n;
    int i = (start + 1) % self[0];
    for (n = 0; n < self[0]; n++) {
        if (func_0017A158(self, i, key))
            return i;
        i = (i + 1) % self[0];
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_0017A2C8);
#ifdef SKIP_ASM
extern "C" int func_0017A158(int* self, int i, void* key);

// Circular search backwards: first index before `start` for which func_0017A158 matches.
extern "C" int func_0017A2C8(int* self, int start, void* key)
{
    int n;
    int i = (start + self[0] - 1) % self[0];
    for (n = 0; n < self[0]; n++) {
        if (func_0017A158(self, i, key))
            return i;
        i = (i + self[0] - 1) % self[0];
    }
    return -1;
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_0017A638);

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_0017A978);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_0017ABC8__FPv);
#ifdef SKIP_ASM
void* func_0017ABC8(void* self)
{
    int t0 = 0;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)((char*)self + 0x8) = -1;
    *(int*)self = t0;
    return self;
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_0017ABE0);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerfactory", func_0017AC98);
#ifdef SKIP_ASM
extern "C" int func_0017AC98(void* self, int i)
{
    if (i >= 0 && i < *(int*)((char*)self + 0x4)) {
        *(int*)((char*)self + 0x8) = i;
    } else {
        *(int*)((char*)self + 0x8) = -1;
    }
    return 1;
}
#endif

