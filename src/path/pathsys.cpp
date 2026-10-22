#include "common.h"

//100%
INCLUDE_ASM("path/pathsys", cPathSys_resolvePaths);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_0026BA68(void* dst, void* src);
extern "C" int func_0026A180(void* path, char* data);
extern char D_00481438[];
extern void* D_00481448[];

inline void* operator new[](unsigned int, void* p) { return p; }

struct sPathB310 {
    char pad_0x0[0x34];
    void** vtable;          // 0x34
    int field_0x38;         // 0x38
    sPathB310() { vtable = D_00481448; }
};

struct sPathSysB310 {
    char pad_0x0[0x10];
    int count;              // 0x10
    sPathB310* paths;       // 0x14
};

extern "C" int cPathSys_resolvePaths(sPathSysB310* self, char* p)
{
    int count;
    sPathB310** slot = &self->paths;
    char* start = p;
    func_0026BA68(&count, p);
    p += 4;
    self->count = count;
    *slot = new (operator_new_tag(count * sizeof(sPathB310), D_00481438, 0, 0)) sPathB310[count];
    for (int i = 0; i < self->count; i++) {
        p += func_0026A180(&self->paths[i], p);
    }
    return p - start;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B410);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_0026BA68(void* dst, void* src);
extern "C" int func_0026A180(void* path, char* data);
extern char D_004A3408[];
extern void* D_00481478[];

struct sPathB410 {
    char pad_0x0[0x34];
    void** vtable;          // 0x34
    int field_0x38;         // 0x38
    int field_0x3C;         // 0x3C
    sPathB410() { vtable = D_00481478; }
    void* operator new[](unsigned int, void* p) { return p; }
};

struct sPathSysB410 {
    char pad_0x0[0x8];
    int count;              // 0x8
    sPathB410* paths;       // 0xC
};

extern "C" int func_0026B410(sPathSysB410* self, char* p)
{
    int count;
    sPathB410** slot = &self->paths;
    char* start = p;
    func_0026BA68(&count, p);
    p += 4;
    self->count = count;
    *slot = new (operator_new_tag(count * sizeof(sPathB410), D_004A3408, 0, 0)) sPathB410[count];
    for (int i = 0; i < self->count; i++) {
        p += func_0026A180(&self->paths[i], p);
    }
    return p - start;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B508);
#ifdef SKIP_ASM
struct sPathEntry28 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x20];
};

struct sPathEntry40 {
    char pad_0x0[0x40];
};

struct sPathEntry3C {
    char pad_0x0[0x3c];
};

struct sPathSys {
    int count28;             // 0x0
    sPathEntry28* entries28; // 0x4
    int count40;             // 0x8
    sPathEntry40* entries40; // 0xc
    int count3C;             // 0x10
    sPathEntry3C* entries3C; // 0x14
};

extern "C" int func_0026B508(sPathSys* self, sPathEntry3C* entry)
{
    int i;
    if (entry != 0) {
        for (i = 0; i < self->count3C; i++) {
            if (&self->entries3C[i] == entry) {
                return i;
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B550);
#ifdef SKIP_ASM
extern "C" int func_0026B550(sPathSys* self, sPathEntry40* entry)
{
    int i;
    if (entry != 0) {
        for (i = 0; i < self->count40; i++) {
            if (&self->entries40[i] == entry) {
                return i;
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B598);
#ifdef SKIP_ASM
extern "C" int func_0026B598(sPathSys* self, int a, int b)
{
    int i;
    int count = self->count28;
    for (i = 0; i < count; i++) {
        if (self->entries28[i].field_0x4 == a && self->entries28[i].field_0x0 == b) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B5E0);
#ifdef SKIP_ASM
struct sPathEntry {
    int id;                 // 0x0
    int type;               // 0x4
    char pad_0x8[0x20];
};

struct sPathEntryList {
    int count;              // 0x0
    sPathEntry* entries;    // 0x4
};

extern "C" sPathEntry* func_0026B5E0(sPathEntryList* list, int type, int id)
{
    int i;
    for (i = 0; i < list->count; i++) {
        if (list->entries[i].type == type && list->entries[i].id == id) {
            return &list->entries[i];
        }
    }
    if (list->count > 0) {
        return list->entries;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B648);
#ifdef SKIP_ASM
extern "C" int func_0026B648(void *arg0, int arg1) {
    int temp_2;

    temp_2 = (*(int *)((char*)(arg0) + (0)));
    if ((temp_2 <= 0) || (arg1 >= temp_2) || (arg1 < 0)) {
        return 0;
    }
    return (*(int *)((char*)(arg0) + (4))) + (arg1 * 0x28);
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B680);
#ifdef SKIP_ASM
struct sPathVec3 {
    float x, y, z;
    sPathVec3() {}
    sPathVec3(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

static inline sPathVec3 operator-(const sPathVec3& a, const sPathVec3& b)
{
    return sPathVec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

struct sPathNearEntry {
    int id;             // 0x0
    int type;           // 0x4
    sPathVec3 pos;      // 0x8
    char pad_0x14[0x14];
};

struct sPathNearList {
    int count;                  // 0x0
    sPathNearEntry* entries;    // 0x4
};

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00154630(void* self, int value, int id);
extern char* D_004A28A8;

// PORT: sqrt.s helper
static inline float pathSqrt(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float pathLen(const sPathVec3& v)
{
    return pathSqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

extern "C" int func_0026B680(sPathNearList* list, sPathVec3* pos)
{
    int i;
    float best = 10000000.0f;
    int bestId = -1;
    for (i = 0; i < list->count; i++) {
        if (list->entries[i].type == 2 && list->entries[i].id != 0) {
            if (func_00154630(cBE_getInterface_Fv(cBE_getBE(), 10), list->entries[i].id,
                              *(int*)(*(char**)(*(char**)(D_004A28A8 + 0x84) + 0x78) + 0x1BC)) != 0) {
                sPathVec3 d = list->entries[i].pos - *pos;
                float dist = pathLen(d);
                if (dist < best) {
                    best = dist;
                    bestId = list->entries[i].id;
                }
            }
        }
    }
    return bestId;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B7D8);
#ifdef SKIP_ASM
extern "C" void func_0026BA68(void* dst, void* src);
extern "C" void func_0026BA88(void* dst, void* src);

struct sPathPair_B7D8 {
    int a;      // 0x0
    int b;      // 0x4
};

struct sPathSys_B7D8 {
    char pad_0x0[0x18];
    sPathPair_B7D8 pairs[1];    // 0x18
};

extern "C" int func_0026B7D8(sPathSys_B7D8* self, char* p)
{
    int count;
    char* start = p;
    func_0026BA68(&count, p);
    p += 4;
    for (int i = 0; i < count; i++) {
        func_0026BA88(&self->pairs[i].b, p);
        p += 4;
        func_0026BA68(&self->pairs[i].a, p);
        p += 4;
    }
    return p - start;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B880);
#ifdef SKIP_ASM
extern "C" void func_0026BA68(void* dst, void* src);
extern "C" void func_0026BA48(void* dst, void* src);

extern int D_00445428[];

struct sPathNode_B880 {
    int id;             // 0x0
    int type;           // 0x4
    char pos[0xC];      // 0x8
    char dir[0xC];      // 0x14
    char* f20;          // 0x20
    char* f24;          // 0x24
};

struct sPathSys_B880 {
    int count;                  // 0x0
    sPathNode_B880* nodes;      // 0x4
    int pad_0x8;                // 0x8
    char* fC;                   // 0xC
    int pad_0x10;               // 0x10
    char* f14;                  // 0x14
};

extern "C" int func_0026B880(sPathSys_B880* self, char* p)
{
    int tmp;
    int a;
    int b;
    char* start = p;
    func_0026BA68(&tmp, p);
    p += 4;
    self->nodes = (sPathNode_B880*)p;
    self->count = tmp;
    for (int i = 0; i < self->count; i++) {
        func_0026BA68(&self->nodes[i].id, p);
        p += 4;
        func_0026BA68(&tmp, p);
        p += 4;
        func_0026BA48(self->nodes[i].pos, p);
        p += 0xC;
        func_0026BA48(self->nodes[i].dir, p);
        p += 0xC;
        func_0026BA68(&a, p);
        p += 4;
        func_0026BA68(&b, p);
        p += 4;
        self->nodes[i].type = 0;
        for (int j = 0; j < 3; j++) {
            if (D_00445428[j] == tmp) {
                self->nodes[i].type = j;
                break;
            }
        }
        self->nodes[i].f24 = self->fC + a * 0x40;
        self->nodes[i].f20 = self->f14 + b * 0x3C;
    }
    return p - start;
}
#endif

extern "C" void* func_003E6574(void*, void*, int);

//100%
INCLUDE_ASM("path/pathsys", func_0026BA48);
#ifdef SKIP_ASM
extern "C" void func_0026BA48(void* dst, void* src)
{
    if (dst != src) {
        func_003E6574(dst, src, 0xc);
    }
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026BA68);
#ifdef SKIP_ASM
extern "C" void func_0026BA68(void* dst, void* src)
{
    if (dst != src) {
        func_003E6574(dst, src, 0x4);
    }
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026BA88);
#ifdef SKIP_ASM
extern "C" void func_0026BA88(void* dst, void* src)
{
    if (dst != src) {
        func_003E6574(dst, src, 0x4);
    }
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026BAA8);
#ifdef SKIP_ASM
extern "C" void* func_0026BAA8(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026BAE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// A 16-byte vector global constructed from four floats.
struct sV4_BAE8 {
    float x, y, z, w;
    sV4_BAE8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
    void* operator new(unsigned int, void* p) { return p; }
} __attribute__((aligned(16)));

// A 16-byte-aligned three-float vector global.
struct sV3_BAE8 {
    float x, y, z;
    sV3_BAE8(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    void* operator new(unsigned int, void* p) { return p; }
} __attribute__((aligned(16)));

// Three packed three-float vectors (D_004D3378).
struct sV3x3_BAE8 {
    float v[9];
    void* operator new(unsigned int, void* p) { return p; }
};

struct sSlot_BAE8 {
    unsigned a : 8;
    unsigned b : 24;
    int c;
    sSlot_BAE8(int n) { a = n; b = n; }
    void* operator new(unsigned int, void* p) { return p; }
};

extern sV4_BAE8 D_004D2FB8;
extern sV4_BAE8 D_004D2FC8;
extern sV4_BAE8 D_004D2FD8;
extern sV4_BAE8 D_004D2FE8;
extern sV4_BAE8 D_004D2FF8;
extern sV4_BAE8 D_004D3008;
extern sV4_BAE8 D_004D3018;
extern sV4_BAE8 D_004D3028;
extern sV4_BAE8 D_004D3038;
extern sV4_BAE8 D_004D3048;
extern sV4_BAE8 D_004D3058;
extern sV4_BAE8 D_004D3068;
extern sV4_BAE8 D_004D3078;
extern sV4_BAE8 D_004D3088;
extern sV4_BAE8 D_004D3098;
extern sV4_BAE8 D_004D30A8;
extern sV4_BAE8 D_004D30B8;
extern sV4_BAE8 D_004D30C8;
extern sV4_BAE8 D_004D30D8;
extern sV4_BAE8 D_004D30E8;
extern sV4_BAE8 D_004D30F8;
extern sV4_BAE8 D_004D3108;
extern sV4_BAE8 D_004D3118;
extern sV4_BAE8 D_004D3128;
extern sV4_BAE8 D_004D3138;
extern sV4_BAE8 D_004D3148;
extern sV4_BAE8 D_004D3158;
extern sV4_BAE8 D_004D3168;
extern sV4_BAE8 D_004D3178;
extern sV4_BAE8 D_004D3188;
extern sV4_BAE8 D_004D3198;
extern sV4_BAE8 D_004D31A8;
extern sV4_BAE8 D_004D31B8;
extern sV4_BAE8 D_004D31C8;
extern sV4_BAE8 D_004D31D8;
extern sV4_BAE8 D_004D31E8;
extern sV4_BAE8 D_004D31F8;
extern sV4_BAE8 D_004D3208;
extern sV4_BAE8 D_004D3218;
extern sV4_BAE8 D_004D3228;
extern sV4_BAE8 D_004D3238;
extern sV4_BAE8 D_004D3248;
extern sV4_BAE8 D_004D3258;
extern sV4_BAE8 D_004D3268;
extern sV4_BAE8 D_004D3278;
extern sV4_BAE8 D_004D3288;
extern sV4_BAE8 D_004D3298;
extern sV4_BAE8 D_004D32A8;
extern sV4_BAE8 D_004D32B8;
extern sV4_BAE8 D_004D32C8;
extern sV4_BAE8 D_004D32D8;
extern sV4_BAE8 D_004D32E8;
extern sV4_BAE8 D_004D32F8;
extern sV4_BAE8 D_004D3308;
extern sV4_BAE8 D_004D3318;
extern sV4_BAE8 D_004D3328;
extern sV3_BAE8 D_004D3338;
extern sV3_BAE8 D_004D3348;
extern sV3_BAE8 D_004D3358;
extern sV3_BAE8 D_004D3368;
extern sV3x3_BAE8 D_004D3378;
extern sSlot_BAE8 D_004A5158;

// Static initialisation of the unit's colour globals (g++ __static_initialization_and_destruction_0).
// The unit declares func_0026BAE8 returning void*; bind this definition by asm label.
extern "C" void func_0026BAE8_init(int initialize, int priority) __asm__("func_0026BAE8");
extern "C" void func_0026BAE8_init(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004D2FB8) sV4_BAE8(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004D2FC8) sV4_BAE8(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004D2FD8) sV4_BAE8(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004D2FE8) sV4_BAE8(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004D2FF8) sV4_BAE8(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004D3008) sV4_BAE8(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004D3018) sV4_BAE8(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004D3028) sV4_BAE8(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004D3038) sV4_BAE8(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004D3048) sV4_BAE8(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004D3058) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D3068) sV4_BAE8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D3078) sV4_BAE8(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004D3088) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D3098) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D30A8) sV4_BAE8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D30B8) sV4_BAE8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D30C8) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D30D8) sV4_BAE8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D30E8) sV4_BAE8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D30F8) sV4_BAE8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D3108) sV4_BAE8(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004D3118) sV4_BAE8(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004D3128) sV4_BAE8(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004D3138) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D3148) sV4_BAE8(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004D3158) sV4_BAE8(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004D3168) sV4_BAE8(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004D3178) sV4_BAE8(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004D3188) sV4_BAE8(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004D3198) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D31A8) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D31B8) sV4_BAE8(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004D31C8) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D31D8) sV4_BAE8(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004D31E8) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D31F8) sV4_BAE8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D3208) sV4_BAE8(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004D3218) sV4_BAE8(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004D3228) sV4_BAE8(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004D3238) sV4_BAE8(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004D3248) sV4_BAE8(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004D3258) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D3268) sV4_BAE8(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004D3278) sV4_BAE8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D3288) sV4_BAE8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D3298) sV4_BAE8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D32A8) sV4_BAE8(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004D32B8) sV4_BAE8(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004D32C8) sV4_BAE8(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004D32D8) sV4_BAE8(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004D32E8) sV4_BAE8(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004D32F8) sV4_BAE8(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004D3308) sV4_BAE8(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004D3318) sV4_BAE8(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004D3328) sV4_BAE8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D3338) sV3_BAE8(1.0f, 0.0f, 0.5f);
        new (&D_004D3348) sV3_BAE8(0.5f, 0.0f, 0.0f);
        new (&D_004D3358) sV3_BAE8(0.0f, 0.5f, 1.0f);
        new (&D_004D3368) sV3_BAE8(0.0f, 0.0f, 0.5f);
        D_004D3378.v[0] = 1.0f;
        D_004D3378.v[1] = 1.0f;
        D_004D3378.v[2] = 1.0f;
        D_004D3378.v[3] = 0.5f;
        D_004D3378.v[4] = 0.5f;
        D_004D3378.v[5] = 0.5f;
        D_004D3378.v[6] = 0.800000011920929f;
        D_004D3378.v[7] = 0.30000001192092896f;
        D_004D3378.v[8] = 0.800000011920929f;
        new (&D_004A5158) sSlot_BAE8(-1);
    }
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026C3D0);
#ifdef SKIP_ASM
extern "C" void* func_0026C3D0(void* dst, void* src)
{
    *(float*)((char*)dst + 0x0) = *(float*)((char*)src + 0xC);
    *(float*)((char*)dst + 0x4) = *(float*)((char*)src + 0x10);
    *(float*)((char*)dst + 0x8) = *(float*)((char*)src + 0x14);
    *(float*)((char*)dst + 0xC) = 1.0f;
    return dst;
}
#endif
//100%
INCLUDE_ASM("path/pathsys", func_0026C400);
#ifdef SKIP_ASM
extern "C" int func_0026C400(void *arg0) {
    return (*(int *)((char*)(arg0) + (4)));
}
#endif
//100%
INCLUDE_ASM("path/pathsys", func_0026C408);
#ifdef SKIP_ASM
extern "C" int func_0026C408(int *arg0) {
    return *arg0;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026C410__FPv);
#ifdef SKIP_ASM
void func_0026C410(void* self)
{
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026C418__FPv);
#ifdef SKIP_ASM
void func_0026C418(void* self)
{
}
#endif

extern "C" void* func_0026BAE8(int, int);

//100%
INCLUDE_ASM("path/pathsys", func_0026C420);
#ifdef SKIP_ASM
extern "C" int func_0026C420(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x38)));
}
#endif
//100%
INCLUDE_ASM("path/pathsys", func_0026C428);
#ifdef SKIP_ASM
extern "C" int func_0026C428(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x3C)));
}
#endif
//100%
INCLUDE_ASM("path/pathsys", func_0026C430);
#ifdef SKIP_ASM
extern "C" float func_0026C430(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x38)));
}
#endif

//99.38%
INCLUDE_ASM("path/pathsys", func_0026C438__FPv);
#ifdef SKIP_ASM
void* func_0026C438(void* self)
{
    return func_0026BAE8(1, 0xffff);
}
#endif

