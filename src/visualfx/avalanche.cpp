#include "common.h"

INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_getFrameData);

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D6290);
#ifdef SKIP_ASM
struct sVec4_6290 {
    float x, y, z, w;
    sVec4_6290() {}
    sVec4_6290(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));
extern float D_004A3AA8;
extern sVec4_6290 D_004FF130;
extern char D_00538450[];

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_6290 vScale_6290(const sVec4_6290& v, float s)
{
    sVec4_6290 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

extern "C" sVec4_6290 func_002D6290(void* self)
{
    char* node = ((char**)D_00538450)[*(unsigned short*)((char*)self + 0x92) >> 8];
    char* p = *(char**)(node + 0x114);
    if (p != 0) {
        return sVec4_6290(*(float*)(p + 0xCC), *(float*)(p + 0xC0), *(float*)(p + 0xC4), *(float*)(p + 0xC8));
    }
    unsigned char* c = *(unsigned char**)(node + 0xFC);
    if (c != 0) {
        return vScale_6290(sVec4_6290(1.0f, (float)c[3] * 0.0078125f, (float)c[4] * 0.0078125f, (float)c[5] * 0.0078125f), D_004A3AA8);
    }
    return D_004FF130;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D6378);
#ifdef SKIP_ASM
extern void* D_00488648[];
extern void* D_00488680[];
struct func_002D6378_sElem {
    char pad[0x2F0];
};
extern func_002D6378_sElem D_004EE840[];
extern "C" void cAvalanche_reset(void* self);
extern "C" void cDynamicColourEmitter_purge(void* p, int flags);
void operator_delete(int* ptr);

extern "C" void func_002D6378(int* self, int flags)
{
    *(void***)self = D_00488648;
    cAvalanche_reset(self);
    int i;
    for (i = 0; i < 64; i++) {
        cDynamicColourEmitter_purge(&D_004EE840[i], 1);
    }
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D6410);
#ifdef SKIP_ASM
struct func_002D6410_sElem {
    float x;
    float y;
    float z;
    float w;
    int r;
    int g;
    int b;
    int a;
    char pad[0x10];
};
extern func_002D6410_sElem D_004D5B70[];
extern func_002D6410_sElem D_004EDB70[];
extern char D_00538450[];
extern int D_004A3AC0;
extern "C" void func_00416210(void* dst, int c, int n);

extern "C" void func_002D6410(void)
{
    int i;
    for (i = 0; i < 0x800; i++) {
        int c = (i & 1) ? 0xFF : 0;
        func_002D6410_sElem* e = &D_004D5B70[i];
        e->a = 0xFF;
        e->r = c;
        e->g = c;
        e->b = c;
        e->z = 1.0f;
        e->w = 0.0f;
        e->x = 0.0f;
        e->y = 0.0f;
    }
    for (i = 0; i < 0x20; i++) {
        func_002D6410_sElem* e = &D_004EDB70[i];
        e->a = 0xFF;
        e->r = 0xFF;
        e->g = 0xFF;
        e->b = 0;
        e->z = 1.0f;
        e->w = 0.0f;
        e->x = 0.0f;
        e->y = 0.0f;
    }
    D_004A3AC0 = 0;
    func_00416210(D_00538450, 0, 0x400);
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D64D8);
#ifdef SKIP_ASM
struct sVec4_64D8 {
    float x, y, z, w;
    sVec4_64D8() {}
    sVec4_64D8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));
extern float D_004A3A94;

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_64D8(const sVec4_64D8& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4_64D8 vNorm_64D8(const sVec4_64D8& v)
{
    sVec4_64D8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_64D8 vScale_64D8(const sVec4_64D8& v, float s)
{
    sVec4_64D8 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

static inline int isBad_64D8(const float* f)
{
    unsigned int b = *(const unsigned int*)f;
    int r = 0;
    if (b == 0x7F800000 || b == 0x7FC00000 || b == 0xFF800000 || b == 0xFFC00000)
        r = 1;
    return r;
}

extern "C" void func_002D64D8(void* self, sVec4_64D8* v)
{
    if (D_004A3A94 < vLen_64D8(*v))
        *v = vScale_64D8(vNorm_64D8(*v), D_004A3A94);
    int bad = isBad_64D8(&v->x) || isBad_64D8(&v->y) || isBad_64D8(&v->z) || isBad_64D8(&v->w);
    if (bad)
        *v = sVec4_64D8(10.0f, 10.0f, 10.0f, 0.0f);
}
#endif

INCLUDE_ASM("visualfx/avalanche", tAvalancheNode_calculate);

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_calculateScale);
#ifdef SKIP_ASM
struct sAvalancheData {
    char pad[0xF0];
    unsigned short type;
    char padF2[0xE];
    float f100;
    float f104;
    float f108;
};

extern "C" void tActiveAvalancheNode_calculateScale(void* self, float t)
{
    sAvalancheData* d = *(sAvalancheData**)((char*)self + 0x2E0);
    float x = t * 30.0f;
    if (x < d->f104) {
        *(float*)((char*)self + 0xB0) = 1.0f;
        *(float*)((char*)self + 0xB4) = x / d->f104;
        return;
    }
    if (x < d->f100 - d->f108) {
        *(float*)((char*)self + 0xB4) = 1.0f;
        *(float*)((char*)self + 0xB0) = 1.0f;
        return;
    }
    if (d->type != 2) {
        float v = (d->f100 - x) / d->f108;
        *(float*)((char*)self + 0xB4) = v;
        *(float*)((char*)self + 0xB0) = v;
        if (v > 0.0f) {
            // PORT: sqrt.s (sqrtf without errno check)
            __asm__("sqrt.s %0, %1" : "=f"(v) : "f"(v));
            *(float*)((char*)self + 0xB0) = v;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", func_002D7CA8);
#ifdef SKIP_ASM
struct sAvVec4CA8 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sAvMatrixCA8 {
    sAvVec4CA8 row[4];
};
struct sAvFrameCA8 {
    sAvVec4CA8 pos;             // 0x0
    sAvVec4CA8 vel;             // 0x10
    sAvMatrixCA8 rot;           // 0x20
};
struct sAvDataCA8 {
    char pad0[0xF0];
    unsigned short type;        // 0xF0
    unsigned char emit;         // 0xF2
    char padF3[0x100 - 0xF3];
    float duration;             // 0x100
    char pad104[0x10C - 0x104];
    float speed;                // 0x10C
};
struct sAvNodeCA8 {
    sAvFrameCA8 getFrameData(float t) __asm__("tActiveAvalancheNode_getFrameData");
    char pad0[0x60];
    sAvVec4CA8 pos;             // 0x60
    sAvMatrixCA8 rot;           // 0x70
    float scale;                // 0xB0
    float fB4;                  // 0xB4
    char padB8[0xC0 - 0xB8];
    sAvVec4CA8 vel;             // 0xC0
    char emitter[0x1FC - 0xD0]; // 0xD0
    float f1FC;                 // 0x1FC
    char pad200[0x2E0 - 0x200];
    sAvDataCA8* data;           // 0x2E0
};

extern "C" void tActiveAvalancheNode_end(void* self, int force);
extern "C" void func_003717C0(void* em, void* pos, void* d, void* vel, int alive, float dt);
extern sAvVec4CA8 D_004FF120_7CA8 __asm__("D_004FF120");

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMatrixCA8(sAvMatrixCA8* dst, sAvMatrixCA8* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

extern "C" int func_002D7CA8(sAvNodeCA8* self, float t)
{
    if (self->data == 0) return 0;
    t *= self->data->speed;
    sAvFrameCA8 fd = self->getFrameData(t);
    if (t * 30.0f < self->data->duration) {
        sAvVec4CA8* pos = &self->pos;
        self->pos = fd.pos;
        vu0CopyMatrixCA8(&self->rot, &fd.rot);
        self->vel = fd.vel;
        sAvDataCA8* d = self->data;
        sAvVec4CA8* vel = &self->vel;
        if (d->type != 2 && d->emit != 0) {
            void* org = &D_004FF120_7CA8;
            void* em = self->emitter;
            self->f1FC = self->fB4;
            int alive = t < d->duration;
            func_003717C0(em, pos, org, vel, alive, 0.01666666753590107f);
        }
        return 1;
    }
    if (self->data->type == 2) return 1;
    tActiveAvalancheNode_end(self, 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_end);
#ifdef SKIP_ASM
extern "C" void cDynamicColourEmitter_reset(void* self);

struct sVEntry2D7DD8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void tActiveAvalancheNode_end(void* self, int force)
{
    char* p = *(char**)((char*)self + 0x2E0);
    if (p != 0 && (force != 0 || *(unsigned short*)(p + 0xF0) != 2)) {
        char* q = *(char**)(p + 0xF8);
        if (q != 0) {
            char* obj = *(char**)(q + 0xC);
            if (obj != 0) {
                sVEntry2D7DD8* vt = *(sVEntry2D7DD8**)(obj + 0xC);
                vt[1].fn(obj + vt[1].delta, 3);
            }
        }
        *(int*)(*(char**)((char*)self + 0x2E0) + 0x114) = 0;
        *(char**)((char*)self + 0x2E0) = 0;
        cDynamicColourEmitter_reset((char*)self + 0xD0);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalanche_buildArray);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_00487548[];

struct tActiveAvalanche_node {
    char pad[0x2E4];
    tActiveAvalanche_node* next; // 0x2E4
};

struct tActiveAvalanche {
    int unk0;
    tActiveAvalanche_node* head;   // 0x4
    tActiveAvalanche_node** array; // 0x8
};

extern "C" void tActiveAvalanche_buildArray(tActiveAvalanche* self)
{
    tActiveAvalanche_node* p;
    int n = 0;
    for (p = self->head; p != 0; p = p->next) {
        n++;
    }
    self->array = (tActiveAvalanche_node**)operator_new_tag(n * 4, D_00487548, 0, 0);
    int i = 0;
    for (p = self->head; p != 0; p = p->next) {
        self->array[i++] = p;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D7EF8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sAvV4_7EF8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sAvObj_7EF8 {
    char pad_0x00[0xC];
    void* spawned;          // 0xC
    char pad_0x10[0x30];
    sAvV4_7EF8 pos;         // 0x40
    char pad_0x50[0x10];
    sAvV4_7EF8 sndPos;      // 0x60
};

struct sAvEvent_7EF8 {
    float time;             // 0x0
    sAvObj_7EF8* obj;       // 0x4
};

struct sAvSound_7EF8 {
    unsigned short frame;   // 0x0
    unsigned short obj;     // 0x2
};

struct sAvData_7EF8 {
    char pad_0x00[0xC];
    unsigned short nEvents;     // 0xC
    unsigned short nSounds;     // 0xE
    sAvEvent_7EF8 events[32];   // 0x10
    sAvSound_7EF8 sounds[1];    // 0x110
};

struct sAvNode_7EF8 {
    char pad_0x000[0x2E4];
    sAvNode_7EF8* next;     // 0x2E4
};

struct sAvActive_7EF8 {
    sAvData_7EF8* data;         // 0x0
    sAvNode_7EF8* nodes;        // 0x4
    sAvObj_7EF8** objs;         // 0x8
    int pad_0xC;
    int nextEvent;              // 0x10
    int nextSound;              // 0x14
    float time;                 // 0x18
};

struct sAvEnd_7EF8 {};  // empty trailing member of the defaults record; its value-init copies a byte through a stack temp

extern float D_004A3A64;
extern int D_004A54C8;
// Defaults record of an animated-float spawn (read by func_00341AA0, sSphFloatDef_1AA0 in intersect/worldsphtree).
// The original keeps it as a function-local static; the ctor's float parameters are what lets loop.c hoist -1.0f/30.0f.
struct sAvFloatDef_7EF8 {
    unsigned id;        // 0x00
    int f04;            // 0x04
    int dir;            // 0x08
    float min;          // 0x0C
    float max;          // 0x10
    float speed;        // 0x14
    float speedVar;     // 0x18
    float value;        // 0x1C
    int random;         // 0x20
    int f24;            // 0x24
    sAvEnd_7EF8 end;    // 0x28
    sAvFloatDef_7EF8(float aMin = -1.0f, float aMax = -1.0f, float aSpeed = 30.0f) : id(0xFFFFFFFF), f04(1), dir(0), min(aMin), max(aMax), speed(aSpeed), speedVar(0), value(-1.0f), random(0), f24(1), end() {}
};

// PORT: placement new; builds the defaults record in place in its retail storage.
inline void* operator new(unsigned int, void* p) { return p; }

extern sAvFloatDef_7EF8 D_004D5770;
extern char D_00487558[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00341AA0(void* self, int a, int type, void* b, void* c);
extern "C" bool func_002D7CA8_7EF8(void* node, float t) __asm__("func_002D7CA8");
extern "C" void* func_0028B180();
extern "C" void func_002D81B0_7EF8(void* self) __asm__("func_002D81B0");
void func_0029E560_7EF8(void* snd, const sAvV4_7EF8& pos) __asm__("func_0029E560__FPv");

static inline unsigned short soundFrame_7EF8(const sAvActive_7EF8* a)
{
    return a->data->sounds[a->nextSound].frame;
}

static inline float toFrames_7EF8(float t)
{
    return t * 30.0f;
}

static inline sAvV4_7EF8 pos_7EF8(const sAvObj_7EF8* o)
{
    return o->pos;
}

extern "C" void func_002D7EF8_7EF8(sAvActive_7EF8* self) __asm__("func_002D7EF8");
extern "C" void func_002D7EF8_7EF8(sAvActive_7EF8* self)
{
    if (self->data == 0) {
        return;
    }
    bool alive = false;
    self->time += D_004A3A64;
    int i = 0;
    for (sAvNode_7EF8* n = self->nodes; n != 0; n = n->next, i++) {
        bool ok = func_002D7CA8_7EF8(n, self->time);
        alive = ok | alive;
        if (!ok) {
            self->objs[i] = 0;
        }
    }
    while (self->nextEvent < self->data->nEvents && self->data->events[self->nextEvent].time <= self->time) {
        if (self->data->events[self->nextEvent].obj->spawned == 0) {
            // NOTE: function-local static of the original (guard D_004A54C8), written out as its guarded in-place construction.
            if (D_004A54C8 == 0) {
                new (&D_004D5770) sAvFloatDef_7EF8();
                D_004A54C8 = 1;
            }
            func_00341AA0(cMemMan_alloc(0x6C, D_00487558, 0x20000000, 0), 1, 1, self->data->events[self->nextEvent].obj, &D_004D5770);
        }
        func_0029E560_7EF8(func_0028B180(), pos_7EF8(self->data->events[self->nextEvent].obj));
        self->nextEvent++;
    }
    while (self->nextSound < self->data->nSounds && (float)soundFrame_7EF8(self) <= toFrames_7EF8(self->time)) {
        sAvObj_7EF8* o = self->objs[self->data->sounds[self->nextSound].obj];
        if (o != 0) {
            func_0029E560_7EF8(func_0028B180(), o->sndPos);
        }
        self->nextSound++;
    }
    if (!alive) {
        func_002D81B0_7EF8(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D81B0);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void* func_0028B180();
extern "C" void func_0029DEF0(void* self, int a1);
extern char* D_004A28A8;

extern "C" void func_002D81B0(void* self)
{
    if (*(void**)((char*)self + 0x8) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x8));
        *(void**)((char*)self + 0x8) = 0;
    }
    if (*(int*)self != 0) {
        char* n = *(char**)((char*)self + 0x4);
        while (n != 0) {
            tActiveAvalancheNode_end(n, 1);
            n = *(char**)(n + 0x2E4);
        }
        *(int*)self = 0;
        func_0029DEF0(func_0028B180(), 0);
        char* p = *(char**)(*(char**)(*(char**)(D_004A28A8 + 0x84) + 0xC) + 0xA8);
        if (*(int*)(p + 0x708) != 0) {
            *(int*)(p + 0x708) = *(int*)(p + 0x708) - 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_SaveData);
#ifdef SKIP_ASM
extern char D_004A3AD0[];
extern char D_004A3AD8[];
extern char D_004A3AE0[];
extern "C" void cAvalanche_SaveData1(int, char*);

extern "C" void cAvalanche_SaveData(void)
{
    cAvalanche_SaveData1(1, D_004A3AD0);
    cAvalanche_SaveData1(1, D_004A3AD8);
    cAvalanche_SaveData1(0, D_004A3AE0);
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D82A0);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void func_003DEC80(void* a, void* b, void* data, int size, int prio);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004A3AE8[];

extern "C" int func_002D82A0(int direct, void* a, void* b, char* src, int stride, int rows)
{
    if (direct) {
        func_003DEC80(a, b, src, stride * rows, 100);
    } else {
        char* buf = (char*)operator_new_tag(stride * rows, D_004A3AE8, 0, 0);
        char* d = buf;
        for (int i = 0; i < rows; i++) {
            char* end = src + stride;
            char* p = end;
            while (src != p) {
                *d++ = *--p;
            }
            src = end;
        }
        func_003DEC80(a, b, buf, stride * rows, 100);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    }
    return stride * rows;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_SaveData1);
#ifdef SKIP_ASM
extern void* D_004A3AB0;
extern int D_004A3A48;
extern char D_004A3AF0[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
extern "C" char* cBENewRaceInterface_getLocationFile(void* iface, int i);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_003DEBF0(char* name, int mode, int prio, int* fd);
extern "C" void func_003DECA0(int fd, int prio);
extern "C" int func_002D82A0(int direct, void* a, void* b, char* src, int stride, int rows);

struct sAvPart_83B8 {
    char pad_0x0[0xE0];
    float pos[3];                   // 0xE0
    char pad_0xEC[0xF4 - 0xEC];
    sAvPart_83B8* next;             // 0xF4
    char pad_0xF8[0x4];
    char* data;                     // 0xFC
    float count;                    // 0x100
};

struct sAvLink_83B8 {
    int id;                         // 0x0
    char* obj;                      // 0x4
};

struct sAvPair_83B8 {
    short a;
    short b;
};

struct sAvGroup_83B8 {
    int f0;
    sAvPart_83B8* parts;            // 0x4
    sAvGroup_83B8* next;            // 0x8
    unsigned short nlinks;          // 0xC
    unsigned short npairs;          // 0xE
    sAvLink_83B8 links[0x20];       // 0x10
    sAvPair_83B8 pairs[1];          // 0x110
};

// PORT: func_002D82A0 takes the file handle and offset as void* (ints passed through pointers).
extern "C" void cAvalanche_SaveData1(int direct, char* name)
{
    if (D_004A3AB0 == 0) {
        return;
    }
    int off = 0;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    char buf[0x100];
    sprintf(buf, D_004A3AF0, cBENewRaceInterface_getLocationFile(iface, cBENewRaceInterface_getLocationID(iface)), name);
    int fd;
    if (func_003DEBF0(buf, 0x26, 0x64, &fd)) {
        sAvGroup_83B8* g;
        for (g = (sAvGroup_83B8*)D_004A3AB0; g != 0; g = g->next) {
            for (sAvPart_83B8* p = g->parts; p != 0; p = p->next) {
                off += 0x10 + (int)p->count * 10;
            }
            off += 4;
            int k;
            for (k = 0; k < g->nlinks; k++) {
                off += 8;
            }
            for (k = 0; k < g->npairs; k++) {
                off += 4;
            }
        }
        int magic = 0x2BEEF00;
        func_002D82A0(direct, (void*)fd, 0, (char*)&magic, 4, 1);
        func_002D82A0(direct, (void*)fd, (void*)4, (char*)&off, 4, 1);
        off = 8;
        for (g = (sAvGroup_83B8*)D_004A3AB0; g != 0; g = g->next) {
            unsigned short h;
            for (sAvPart_83B8* p = g->parts; p != 0; p = p->next) {
                h = 0xBEEF;
                off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&h, 2, 1);
                h = (int)p->count * 10 + 0xC;
                off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&h, 2, 1);
                off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)p->pos, 4, 3);
                off += func_002D82A0(1, (void*)fd, (void*)off, p->data, 10, (int)p->count);
            }
            h = g->nlinks;
            off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&h, 2, 1);
            h = g->npairs;
            off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&h, 2, 1);
            int k;
            for (k = 0; k < g->nlinks; k++) {
                off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&g->links[k].id, 4, 1);
                off += func_002D82A0(direct, (void*)fd, (void*)off, g->links[k].obj + 0x78, 4, 1);
            }
            for (k = 0; k < g->npairs; k++) {
                off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&g->pairs[k].b, 2, 1);
                off += func_002D82A0(direct, (void*)fd, (void*)off, (char*)&g->pairs[k].a, 2, 1);
            }
        }
        func_003DECA0(fd, 0x64);
    }
    D_004A3A48 = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_LoadData);
#ifdef SKIP_ASM
extern void* D_004A3AB4;
extern int D_004A3AB8;
extern int D_004A3ABC;
extern int D_004A3AC4;
extern int D_004A3A4C;
extern char D_004A3AD8[];
// PORT: the read call returns func_003DEB50's result (bytes read); the ealib unit declares it void.
extern "C" int func_003DEC60_i(int fd, int off, void* data, int size, int prio) __asm__("func_003DEC60");
extern "C" void cAvalanche_end(void);
extern "C" void cAvalanche_resolveDataPointers(void);

// Loads the avalanche data file for the current mountain (magic 0x2BEEF00, then the data size) into a fresh buffer.
extern "C" void cAvalanche_LoadData(void)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    char buf[0x100];
    sprintf(buf, D_004A3AF0, cBENewRaceInterface_getLocationFile(iface, cBENewRaceInterface_getLocationID(iface)), D_004A3AD8);
    D_004A3ABC = 0;
    D_004A3AC4 = 1;
    int fd;
    if (func_003DEBF0(buf, 1, 0x64, &fd)) {
        cAvalanche_end();
        if (D_004A3AB4 != 0 && D_004A3AB8 != 0) {
            cMemMan_free(D_004A3AB4);
        }
        int* hdr = (int*)operator_new_tag(8, buf, 0, 0);
        func_003DEC60_i(fd, 0, hdr, 8, 0x64);
        if (hdr[0] != 0x2BEEF00) {
            if (hdr != 0) {
                cMemMan_free(hdr);
            }
            D_004A3AB4 = 0;
            D_004A3AB8 = 0;
        } else {
            int size = hdr[1];
            if (hdr != 0) {
                cMemMan_free(hdr);
            }
            void* data = operator_new_tag(size, buf, 0, 0);
            D_004A3AB8 = 1;
            D_004A3AB4 = data;
            func_003DEC60_i(fd, 0, data, size + 8, 0x64);
            func_003DECA0(fd, 0x64);
            D_004A3AC4 = 0;
        }
    }
    D_004A3A4C = 0;
    cAvalanche_resolveDataPointers();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", cAvalanche_update);
#ifdef SKIP_ASM
struct sAvalancheSlot {
    int active;
    char pad[0x18];
};
extern int D_004A3A30;
extern int D_004A3A34;
extern int D_004A3A5C;
extern int D_004A3A60;
extern int D_004A3A7C;
extern int D_004A3ABC;
extern int D_004A3AC4;
extern sAvalancheSlot D_00538938[];
extern "C" void cAvalanche_calculate(void);
extern "C" void cAvalanche_resolveDataPointers(void);
extern "C" int cAvalanche_triggerAvalanche(int a);
extern "C" void func_002D7EF8(void* self);

extern "C" void cAvalanche_update(void)
{
    if (D_004A3A30 != 0) {
        if (D_004A3A34 != 0) {
            cAvalanche_calculate();
            D_004A3A34 = 0;
            D_004A3AC4 = 0;
        }
        cAvalanche_resolveDataPointers();
        if (D_004A3A5C != 0) {
            cAvalanche_triggerAvalanche(D_004A3A7C);
            if (D_004A3A5C < 0) {
                D_004A3A5C = 0;
            }
        }
        if (D_004A3A30 != 0 && D_004A3ABC != 0 && D_004A3A60 != 0) {
            int i;
            for (i = 0; i < 16; i++) {
                func_002D7EF8(&D_00538938[i]);
            }
            if (D_004A3A60 < 0) {
                D_004A3A60 = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_renderLines);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sRow_8A00 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVec4_8A00 : sRow_8A00 {
    sVec4_8A00() {}
    sVec4_8A00(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

struct sVec3_8A00 {
    float x, y, z;
    sVec3_8A00() {}
    sVec3_8A00(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    sVec3_8A00(const sVec3_8A00& o) : x(o.x), y(o.y), z(o.z) {}
};

static inline float Sqrt_8A00(float x)
{
    float r;
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline sVec3_8A00 Normalize_8A00(const sVec3_8A00& a)
{
    float len = Sqrt_8A00(a.x * a.x + a.y * a.y + a.z * a.z);
    if (len != 0.0f) {
        float inv = 1.0f / len;
        return sVec3_8A00(a.x * inv, a.y * inv, a.z * inv);
    }
    return a;
}

struct sMat_8A00 {
    sRow_8A00 v[4];
} __attribute__((aligned(16)));

struct sRec_8A00 {
    signed char dx, dy, dz;
    unsigned char r, g, b;
    signed char ax, ay, az;
    unsigned char angle;
};

struct sObj_8A00 {
    char pad[0xE0];
    sRow_8A00 pos;              // 0xE0
    char padF0[0xC];
    sRec_8A00* recs;            // 0xFC
    float count;                // 0x100
};

struct sNode_8A00 {
    char pad[0x2E0];
    sObj_8A00* obj;             // 0x2E0
};

struct sOut_8A00 {
    float x, y, z, w;
    int r;
    int g;
    int b;
    int a;
    sRow_8A00 pos;
};

struct sRS_8A00 {
    int w0;
    int w1;
    int w2;
    int w3;
    int w4;
};

struct sVE_8A00 {
    short delta;
    short index;
    void (*fn)(void*, int, void*, int);
};

struct sCtx_8A00 {
    char pad[0xE84];
    sRS_8A00* top;              // 0xE84
    char pad2[0x10D8 - 0xE88];
    sVE_8A00* vt;               // 0x10D8
};

extern sCtx_8A00* D_004A289C_8A00 __asm__("D_004A289C");
extern int D_004A3A58;
extern sOut_8A00 D_004D5B70_8A00[] __asm__("D_004D5B70");
extern sMat_8A00 D_004FF1A0_8A00 __asm__("D_004FF1A0");
extern sRow_8A00 D_004FF130_8A00 __asm__("D_004FF130");
extern "C" void func_0031BE50(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (matrix copy).
static inline void CopyMat_8A00(sMat_8A00* d, const sMat_8A00* s)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector add in place).
static inline void AddTo_8A00(sRow_8A00& a, const sRow_8A00& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply: a = a * b).
static inline void MulMat_8A00(sMat_8A00* a, const sMat_8A00* b)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        :
        : "r"(a), "r"(b)
        : "memory");
}

// matrix *= matrix through VU0. Declared to return the matrix by value but there is no return statement (g++ 2.95
// accepts it); the ignored by-value result is the unwritten 64-byte call temp in the retail frame.
static inline sMat_8A00 operator*=(sMat_8A00& a, const sMat_8A00& b)
{
    MulMat_8A00(&a, &b);
}

// Rotate m about axis by angle, keeping its translation.
static inline void Rotate_8A00(sMat_8A00& m, const sRow_8A00& axis, float angle)
{
    sMat_8A00 old;
    float s;
    float c;
    CopyMat_8A00(&old, &m);
    sRow_8A00 trans = old.v[3];
    old.v[3] = D_004FF130_8A00;
    func_0031BE50(&s, &c, angle);
    float t = 1.0f - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    m.v[3].x = 0.0f;
    m.v[3].y = 0.0f;
    m.v[3].z = 0.0f;
    m.v[3].w = 1.0f;
    m.v[3] = trans;
    m.v[0].w = 0.0f;
    m.v[1].w = 0.0f;
    m.v[2].w = 0.0f;
    m.v[0].x = tx * x + c;
    m.v[1].x = tx * y + sz;
    m.v[2].x = tx * z - sy;
    m.v[0].y = ty * x - sz;
    m.v[1].y = ty * y + c;
    m.v[2].y = ty * z + sx;
    m.v[0].z = tz * x + sy;
    m.v[1].z = tz * y - sx;
    m.v[2].z = tz * z + c;
    m *= old;
}

static inline void PushRS_8A00(sCtx_8A00* ctx)
{
    ctx->top[1] = ctx->top[0];
    ctx->top++;
}

static inline void SetBits_8A00(int& w, int mask, int shift, int v)
{
    w = (w & ~mask) | ((v << shift) & mask);
}

// Draws the debris particles of an avalanche node.
extern "C" void tActiveAvalancheNode_renderLines(void* nodep)
{
    sNode_8A00* node = (sNode_8A00*)nodep;
    if (node->obj == 0) {
        return;
    }
    sMat_8A00 m;
    CopyMat_8A00(&m, &D_004FF1A0_8A00);
    PushRS_8A00(D_004A289C_8A00);
    SetBits_8A00(D_004A289C_8A00->top->w1, 0x7C, 2, 1);
    if (D_004A3A58 != 0) {
        int i = 0;
        sRow_8A00 pos = node->obj->pos;
        for (; i < (int)node->obj->count; i++) {
            sVec4_8A00 v(node->obj->recs[i].dx * 2.0f, node->obj->recs[i].dy * 2.0f, node->obj->recs[i].dz * 2.0f, 0.0f);
            AddTo_8A00(pos, v);
            sVec3_8A00 n = Normalize_8A00(sVec3_8A00(node->obj->recs[i].ax * 0.007874015718698502f, node->obj->recs[i].ay * 0.007874015718698502f, node->obj->recs[i].az * 0.007874015718698502f));
            v.x = n.x;
            v.y = n.y;
            v.z = n.z;
            v.w = 0.0f;
            Rotate_8A00(m, v, node->obj->recs[i].angle * 0.008213315159082413f);
            m.v[3] = pos;
            D_004D5B70_8A00[i].r = node->obj->recs[i].r;
            D_004D5B70_8A00[i].g = node->obj->recs[i].g;
            D_004D5B70_8A00[i].b = node->obj->recs[i].b;
            D_004D5B70_8A00[i].pos = pos;
        }
        D_004A289C_8A00->vt[0x248 / 8].fn((char*)D_004A289C_8A00 + D_004A289C_8A00->vt[0x248 / 8].delta, (int)node->obj->count, D_004D5B70_8A00, 0);
    }
    D_004A289C_8A00->top--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", tActiveAvalanche_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void tActiveAvalancheNode_renderLines(void* node);
extern "C" void func_00371688(void* p, int a);
extern char* D_004A28A8;
extern int D_004A3A54;
extern int D_004A3A58;

struct sAvVec4_D8EA8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sAvVert_D8EA8 {
    sAvVec4_D8EA8 uv;           // 0x0
    int r, g, b, a;             // 0x10
    sAvVec4_D8EA8 pos;          // 0x20
};
extern sAvVert_D8EA8 D_vtx_D8EA8[] __asm__("D_004EDB70");

struct sAvPair_D8EA8 { float v; char* node; };
struct sAvGroup_D8EA8 {
    int f0;
    void* nodes;                // 0x4
    void* next;                 // 0x8
    unsigned short nPairs;      // 0xC
    unsigned short nIdx;        // 0xE
    sAvPair_D8EA8 pairs[32];    // 0x10
};

struct sAvActive_D8EA8 {
    char pad0[0xD0];
    char fx[0x2E0 - 0xD0];      // 0xD0
    char* data;                 // 0x2E0
    sAvActive_D8EA8* next;      // 0x2E4
};

struct sAvSlot_D8EA8 {
    sAvGroup_D8EA8* group;      // 0x0
    sAvActive_D8EA8* active;    // 0x4
    char pad8[0x18 - 0x8];
    float limit;                // 0x18
};

struct sRState_D8EA8 {
    int f0;
    unsigned int f4;
};

class cRCtx_D8EA8 {
public:
    char pad[0xE84];
    sRState_D8EA8* top;         // 0xE84
    char padE88[0x10D8 - 0xE88];
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
    virtual void drawLines(int n, sAvVert_D8EA8* v, int f);
};
extern cRCtx_D8EA8* D_ctx_D8EA8 __asm__("D_004A5B80");

class cRiderPos_D8EA8 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual sAvVec4_D8EA8* getPos();
};

extern "C" void tActiveAvalanche_render(void* slotv)
{
    sAvSlot_D8EA8* slot = (sAvSlot_D8EA8*)slotv;
    cRCtx_D8EA8* ctx = D_ctx_D8EA8;
    if (slot->group == 0)
        return;
    int i = 0;
    for (sAvActive_D8EA8* n = slot->active; n != 0; n = n->next, i++) {
        if (n->data) {
            if (i < D_004A3A58)
                tActiveAvalancheNode_renderLines(n);
            if (D_004A3A54 && ((unsigned char*)n->data)[0xF2])
                func_00371688(n->fx, 7);
        }
    }
    if (D_004A3A58 == 0)
        return;
    for (int j = 0; j < slot->group->nPairs; j++) {
        sAvVec4_D8EA8 p = *(sAvVec4_D8EA8*)(slot->group->pairs[j].node + 0x40);
        D_vtx_D8EA8[2 * j].pos = p;
        D_vtx_D8EA8[2 * j].r = slot->group->pairs[j].v < slot->limit ? 0xFF : 0;
        D_vtx_D8EA8[2 * j].g = slot->group->pairs[j].v >= slot->limit ? 0xFF : 0;
        char* rider = *(char**)(*(char**)(*(char**)(D_004A28A8 + 0x84) + 0xC) + 0x28);
        D_vtx_D8EA8[2 * j + 1].pos = *((cRiderPos_D8EA8*)(rider + 0x6C0))->getPos();
        D_vtx_D8EA8[2 * j + 1].r = slot->group->pairs[j].v < slot->limit ? 0xFF : 0;
        D_vtx_D8EA8[2 * j + 1].g = slot->group->pairs[j].v >= slot->limit ? 0xFF : 0;
    }
    sRState_D8EA8* top = ctx->top;
    unsigned int w = top->f4;
    top->f4 = (w & 0xFE7FFFFF) | 0x1000000;
    int saved = (w & 0x1800000) >> 23;
    ctx->drawLines(slot->group->nPairs, D_vtx_D8EA8, 0);
    ctx->top->f4 = (ctx->top->f4 & 0xFE7FFFFF) | ((saved << 23) & 0x1800000);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", cAvalanche_render);
#ifdef SKIP_ASM
struct sRState_9130 {
    int f0;
    int f4;
    unsigned b0 : 5;
    unsigned b5 : 5;
    unsigned b10 : 22;
    int fC;
    short f10;
    short f12;
};
struct sRCtx_9130 {
    char pad[0xE84];
    sRState_9130* top;          // 0xE84
    char padE88[0x10D8 - 0xE88];
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
    virtual void setMatrix(void* m, int a);
    virtual void setPos(float x, float y, float z);
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void setRot(void* r);
};
struct sCam_9130 {
    int f0;
    float* views[4];            // 0x4
    int cur;                    // 0x14
    float mats[4][4];           // 0x18
};
extern char* D_004A28A8;
extern sRCtx_9130* D_004A5B80;
extern int D_004A41EC;
extern sRState_9130 D_00501420;
extern "C" void tActiveAvalanche_render(void* slot);

static inline sCam_9130* getCam_9130() { return *(sCam_9130**)(*(char**)(D_004A28A8 + 0x84) + 0x84); }

static inline void setB5_9130(sRCtx_9130* c, int v) { c->top->b5 = v; }

extern "C" void cAvalanche_render(void)
{
    if ((D_004A3A30 != 0 && D_004A3ABC != 0) || D_004A41EC != 0)
    {
        sRCtx_9130* ctx = D_004A5B80;
        float* view = getCam_9130()->views[getCam_9130()->cur];
        ctx->top[1] = ctx->top[0];
        ctx->top++;
        *ctx->top = D_00501420;
        setB5_9130(ctx, 7);
        *(short*)((char*)ctx->top + 0x10) = -1;
        ctx->setMatrix(getCam_9130()->mats[getCam_9130()->cur], 1);
        ctx->setPos(view[0], view[1], view[2]);
        ctx->setRot(view + 0x10);
        if (D_004A3A30 != 0)
        {
            for (int i = 0; i < 16; i++)
                tActiveAvalanche_render(&D_00538938[i]);
        }
        ctx->top--;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_addAvalancheNode);
#ifdef SKIP_ASM
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char D_00487568[];
extern char D_00487578[];
extern void* D_004A3AB0;
extern char D_00538450[];
extern int D_004A3AC0;
extern int D_004A3ABC;

struct sAvNodeD92D0 {
    char pad0[0xF0];
    unsigned short type;        // 0xF0
    unsigned char emit;         // 0xF2
    unsigned char owned;        // 0xF3
    sAvNodeD92D0* next;         // 0xF4
    char* obj;                  // 0xF8
    char* data;                 // 0xFC
    float count;                // 0x100
    float sy;                   // 0x104
    float sz;                   // 0x108
    float rate;                 // 0x10C
    float radius;               // 0x110
    int f114;                   // 0x114
    char pad118[0x120 - 0x118];
};
struct sAvGroupD92D0 {
    int id;                     // 0x0
    sAvNodeD92D0* nodes;        // 0x4
    sAvGroupD92D0* next;        // 0x8
    unsigned short nPairs;      // 0xC
    unsigned short nIdx;        // 0xE
    char pad10[0x210 - 0x10];
    // tAvalanche::init(int id) (On Tour declares it; body inlined here). The caller links the new
    // record in afterwards, overwriting m_next.
    void init(int inId)
    {
        id = inId;
        next = 0;
        nPairs = 0;
        nIdx = 0;
        nodes = 0;
    }
};

// PORT: abs.s via inline asm (as an SDK math-header fabsf would).
static inline float Abs_D92D0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

#define AV_MIN_D92D0(a, b) ((a) < (b) ? (a) : (b))

extern "C" void cAvalanche_addAvalancheNode(int id, char* obj, int type, float a, float b, float c, float d)
{
    sAvGroupD92D0* g = (sAvGroupD92D0*)D_004A3AB0;
    while (g != 0 && g->id != id)
        g = g->next;
    if (g == 0) {
        g = new (D_00487568, 0, 0) sAvGroupD92D0;
        g->init(id);
        g->next = (sAvGroupD92D0*)D_004A3AB0;
        D_004A3AB0 = g;
    }
    sAvNodeD92D0* n = new (D_00487578, 0, 0) sAvNodeD92D0;
    n->next = g->nodes;
    g->nodes = n;
    n->rate = d;
    n->count = a * 30.0f;
    n->sy = b * 30.0f;
    n->sz = c * 30.0f;
    n->obj = obj;
    // The target materialises &n->radius (addiu v1,a0,0x110; swc1 0(v1)): the source bound the field by reference.
    float& radius = n->radius;
    radius = AV_MIN_D92D0(AV_MIN_D92D0(Abs_D92D0(*(float*)(obj + 0x6C) - *(float*)(obj + 0x60)),
                                         Abs_D92D0(*(float*)(obj + 0x70) - *(float*)(obj + 0x64))),
                             Abs_D92D0(*(float*)(obj + 0x74) - *(float*)(obj + 0x68))) *
                0.20000000298023224f;
    n->data = 0;
    n->owned = 0;
    switch (type) {
    case 5:
        n->type = 2;
        break;
    case 2:
        n->type = 0;
        break;
    default:
        n->type = 1;
        break;
    }
    n->emit = 0;
    n->f114 = 0;
    int idx = D_004A3AC0;
    ((sAvNodeD92D0**)D_00538450)[idx] = n;
    *(unsigned short*)(n->obj + 0x92) = *(unsigned char*)(n->obj + 0x92);
    *(unsigned short*)(n->obj + 0x92) |= (idx << 8) | 2;
    D_004A3AC0 = idx + 1;
    D_004A3ABC = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D9538);
#ifdef SKIP_ASM
struct sAvParams9538 { int data[0xD8 / 4]; };
struct sAvNode9538 {
    sAvParams9538 params;       // 0x0
    char padD8[0xF0 - 0xD8];
    unsigned short type;        // 0xF0
    unsigned char emit;         // 0xF2
    char padF3;
    sAvNode9538* next;          // 0xF4
    int id;                     // 0xF8
};
struct sAvGroup9538 {
    int f0;
    sAvNode9538* nodes;         // 0x4
    sAvGroup9538* next;         // 0x8
};
extern void* D_004A3AB0;

extern "C" void func_002D9538(int id, sAvParams9538* params)
{
    for (sAvGroup9538* g = (sAvGroup9538*)D_004A3AB0; g != 0; g = g->next) {
        for (sAvNode9538* n = g->nodes; n != 0; n = n->next) {
            if (n->id == id) {
                if (n->type == 2) return;
                n->emit = 1;
                n->params = *params;
                return;
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", tAvalanche_calculate);
#ifdef SKIP_ASM
extern "C" void tAvalancheNode_calculate(void* node, void* self);

extern "C" void tAvalanche_calculate(void* self)
{
    void* n = *(void**)((char*)self + 0x4);
    while (n != 0) {
        tAvalancheNode_calculate(n, self);
        n = *(void**)((char*)n + 0xF4);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D96B0);
#ifdef SKIP_ASM
extern "C" void* func_002D96B0(void* self, int id)
{
    for (void* n = *(void**)((char*)self + 0x4); n != 0; n = *(void**)((char*)n + 0xF4)) {
        if (*(int*)((char*)n + 0xF8) == id) {
            return n;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", cAvalanche_calculate);
#ifdef SKIP_ASM
extern "C" void cAvalanche_end(void);
extern "C" void cAvalanche_SaveData(void);
extern "C" void cAvalanche_LoadData(void);
extern void* D_004A3AB0;

extern "C" void cAvalanche_calculate(void)
{
    cAvalanche_end();
    for (void* p = D_004A3AB0; p != 0; p = *(void**)((char*)p + 0x8)) {
        tAvalanche_calculate(p);
    }
    cAvalanche_SaveData();
    cAvalanche_LoadData();
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D9738);
#ifdef SKIP_ASM
struct sAvNode_9738 { char pad[0xF4]; sAvNode_9738* next; };
struct sAvEnt_9738 { short a; short depth; };
struct sAv_9738 {
    int f0;
    sAvNode_9738* head;      // 0x4
    char pad8[6];
    unsigned short count;    // 0xE
    char pad10[0x100];
    sAvEnt_9738 ents[64];    // 0x110
};
// Records (a, depth of stop in the node list) while there is room for it.
extern "C" void func_002D9738(sAv_9738* self, int a, sAvNode_9738* stop)
{
    if (self->count < 64) {
        int depth = 0;
        sAvNode_9738* p = self->head;
        while (p != 0 && p != stop) {
            depth++;
            p = p->next;
        }
        self->ents[self->count].a = a;
        self->ents[self->count].depth = depth;
        self->count++;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_triggerAvalanche);
#ifdef SKIP_ASM
extern "C" float cRenderStateMan_SnowFlakeColourR(int a);
extern "C" float cRenderStateMan_SnowFlakeColourG(int a);
extern "C" float cRenderStateMan_SnowFlakeColourB(int a);
extern "C" void func_00370DC8(void* self, void* node, float k);
extern "C" void tActiveAvalancheNode_calculateScale(void* self, float t);

extern "C" void func_0029DEF0(void* self, int a1);
extern char* D_004A28A8;
extern void* D_004A3AB0;

struct sVec_D97A8 {
    float x, y, z, w;
    sVec_D97A8() {}
    sVec_D97A8(const float& a, const float& b, const float& c, const float& d) : x(a), y(b), z(c), w(d) {}
} __attribute__((aligned(16)));

struct sMat_D97A8 {
    sVec_D97A8 row[4];
};
extern sMat_D97A8 D_004FF1A0_D97A8 __asm__("D_004FF1A0");

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void CopyMatrix_D97A8(sMat_D97A8* dst, sMat_D97A8* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

struct sAvAct_D97A8;
struct sAvNode_D97A8 {
    char pad0[0xE0];
    sVec_D97A8 pos;             // 0xE0
    unsigned short type;        // 0xF0
    unsigned char emit;         // 0xF2
    unsigned char owned;        // 0xF3
    sAvNode_D97A8* next;        // 0xF4
    char* obj;                  // 0xF8
    char padFC[0x114 - 0xFC];
    sAvAct_D97A8* act;          // 0x114
};

struct sAvGroup_D97A8 {
    int id;                     // 0x0
    sAvNode_D97A8* nodes;       // 0x4
    sAvGroup_D97A8* next;       // 0x8
};

struct sAvAct_D97A8 {
    int f0;                     // 0x0
    char pad4[0x10 - 0x4];
    sVec_D97A8 pos;             // 0x10
    sMat_D97A8 m20;             // 0x20
    sVec_D97A8 objPos;          // 0x60
    sMat_D97A8 m70;             // 0x70
    char padB0[0xC0 - 0xB0];
    sVec_D97A8 scale;           // 0xC0
    char emitter[0x1E0 - 0xD0]; // 0xD0
    sVec_D97A8 colour;          // 0x1E0
    char pad1F0[0x2D0 - 0x1F0];
    int f2D0;                   // 0x2D0
    char pad2D4[0x2E0 - 0x2D4];
    sAvNode_D97A8* data;        // 0x2E0
    sAvAct_D97A8* next;         // 0x2E4
    int pad2E8;
    int f2EC;                   // 0x2EC
};
extern sAvAct_D97A8 D_act_D97A8[] __asm__("D_004EE770");

struct sAvSlot_D97A8 {
    sAvGroup_D97A8* group;      // 0x0
    sAvAct_D97A8* active;       // 0x4
    void* arr;                  // 0x8
    int fC;
    int f10;                    // 0x10
    int f14;                    // 0x14
    float time;                 // 0x18
};
extern sAvSlot_D97A8 D_slots_D97A8[] __asm__("D_00538938");

extern "C" int cAvalanche_triggerAvalanche(int id)
{
    cAvalanche_resolveDataPointers();
    for (sAvGroup_D97A8* g = (sAvGroup_D97A8*)D_004A3AB0; g != 0; g = g->next) {
        if (g->id != id)
            continue;
        for (int j = 0; j < 16; j++) {
            if (D_slots_D97A8[j].group == g)
                return (int)0x80000000;
        }
        sAvSlot_D97A8* slot = D_slots_D97A8;
        for (int i = 0; i < 16; i++, slot++) {
            if (slot->group != 0)
                continue;
            slot->group = g;
            slot->active = 0;
            slot->time = 0.0f;
            slot->f10 = 0;
            slot->f14 = 0;
            sAvAct_D97A8* act = D_act_D97A8;
            int k = 0;
            for (sAvNode_D97A8* n = g->nodes; n != 0; n = n->next) {
                for (; k < 64; k++, act++) {
                    if (act->data == 0) {
                        act->data = n;
                        n->act = act;
                        act->next = slot->active;
                        slot->active = act;
                        act->f0 = 0;
                        act->pos = n->pos;
                        CopyMatrix_D97A8(&act->m20, &D_004FF1A0_D97A8);
                        act->f2EC = 0;
                        {
                            sVec_D97A8 p = *(sVec_D97A8*)(n->obj + 0x40);
                            act->objPos = p;
                        }
                        CopyMatrix_D97A8(&act->m70, &D_004FF1A0_D97A8);
                        act->scale = sVec_D97A8(1.0f, 1.0f, 1.0f, 1.0f);
                        tActiveAvalancheNode_calculateScale(act, 0.0f);
                        if (n->emit) {
                            cDynamicColourEmitter_reset(act->emitter);
                            func_00370DC8(act->emitter, n, 2.0f);
                            act->f2D0 = 1;
                            float r = cRenderStateMan_SnowFlakeColourR(0);
                            float gg = cRenderStateMan_SnowFlakeColourG(0);
                            float b = cRenderStateMan_SnowFlakeColourB(0);
                            act->colour = sVec_D97A8(r, gg, b, 0.0f);
                        }
                        break;
                    }
                }
                if (k == 64) {
                    slot->group = 0;
                    for (act = slot->active; act != 0; act = act->next) {
                        if (act->data)
                            act->data->act = 0;
                        act->data = 0;
                    }
                    return (int)0x80000000;
                }
            }
            tActiveAvalanche_buildArray((tActiveAvalanche*)slot);
            func_0029DEF0(func_0028B180(), 1);
            (*(int*)(*(char**)(*(char**)(*(char**)(D_004A28A8 + 0x84) + 0xC) + 0xA8) + 0x708))++;
            return slot - D_slots_D97A8;
        }
    }
    return (int)0x80000000;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_end);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cAvalanche_end_sSlot {
    int active;
    char pad[0x18];
};
// Typed view of D_00538938 (cAvalanche_update declares it earlier in the unit with its own element type).
extern cAvalanche_end_sSlot D_slots_9A80[] __asm__("D_00538938");
extern int D_004A3AB8;
void cMemMan_free(void*);
extern "C" void func_002D81B0(void* self);

static inline void freeSlot_9A80(cAvalanche_end_sSlot* s)
{
    if (s->active != 0) {
        func_002D81B0(s);
    }
}

extern "C" void cAvalanche_end(void)
{
    int i;
    for (i = 0; i < 16; i++) {
        freeSlot_9A80(&D_slots_9A80[i]);
    }
    if (D_004A3AB8 != 0) {
        for (char* p = (char*)D_004A3AB0; p != 0; p = *(char**)(p + 0x8)) {
            for (char* n = *(char**)(p + 0x4); n != 0; n = *(char**)(n + 0xF4)) {
                if (*(unsigned char*)(n + 0xF3) != 0 && *(void**)(n + 0xFC) != 0) {
                    cMemMan_free(*(void**)(n + 0xFC));
                }
                *(unsigned char*)(n + 0xF3) = 0;
                *(void**)(n + 0xFC) = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_reset);
#ifdef SKIP_ASM
extern "C" void cAvalanche_end(void);
extern "C" void func_00416210(void* dst, int c, int n);
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern char D_00538450[];
extern int D_004A3AC0;
extern void* D_004A3AB4;
extern int D_004A3AB8;
extern int D_004A3ABC;

extern "C" void cAvalanche_reset(void* self)
{
    cAvalanche_end();
    char* p = (char*)D_004A3AB0;
    while (p != 0) {
        char* cur = p;
        char* n = *(char**)(p + 0x4);
        while (n != 0) {
            char* next = *(char**)(n + 0xF4);
            operator_delete((int*)n);
            n = next;
        }
        p = *(char**)(p + 0x8);
        operator_delete((int*)cur);
    }
    D_004A3AB0 = 0;
    D_004A3AC0 = 0;
    func_00416210(D_00538450, 0, 0x400);
    if (D_004A3AB4 != 0 && D_004A3AB8 != 0) {
        cMemMan_free(D_004A3AB4);
        D_004A3AB4 = 0;
        D_004A3AB8 = 0;
    }
    D_004A3ABC = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D9C00);
#ifdef SKIP_ASM
struct sAvVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sAvMatrix {
    sAvVec4 row[4];
};

struct sAvNode {
    char pad[0x60];
    sAvVec4 pos;            // 0x60
    sAvMatrix rot;          // 0x70
    float scale;            // 0xB0
    char padB4[0x2E0 - 0xB4];
    void* data;             // 0x2E0
    char pad2E4[0x2F0 - 0x2E4];
};

extern sAvNode D_004EE770[64];

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMatrixAV(sAvMatrix* dst, sAvMatrix* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix times scalar).
static inline void vu0ScaleMatrixAV(sAvMatrix* dst, sAvMatrix* src, float s)
{
    int t;
    __asm__ __volatile__(
        "mfc1      %0, %3\n"
        "lqc2      $vf4, 0x0(%2)\n"
        "qmtc2.ni  %0, $vf3\n"
        "lqc2      $vf5, 0x10(%2)\n"
        "lqc2      $vf6, 0x20(%2)\n"
        "lqc2      $vf7, 0x30(%2)\n"
        "vmulx.xyzw $vf8, $vf4, $vf3x\n"
        "vmulx.xyzw $vf9, $vf5, $vf3x\n"
        "vmulx.xyzw $vf10, $vf6, $vf3x\n"
        "vmulx.xyzw $vf11, $vf7, $vf3x\n"
        "sqc2      $vf8, 0x0(%1)\n"
        "sqc2      $vf9, 0x10(%1)\n"
        "sqc2      $vf10, 0x20(%1)\n"
        "sqc2      $vf11, 0x30(%1)\n"
        : "=&r"(t)
        : "r"(dst), "r"(src), "f"(s)
        : "memory");
}

extern "C" void func_002D9C00(int id, sAvMatrix* out)
{
    int i;
    for (i = 0; i < 64; i++) {
        sAvNode* node = &D_004EE770[i];
        if (node->data != 0 && *(int*)((char*)node->data + 0xF8) == id) {
            vu0CopyMatrixAV(out, &node->rot);
            vu0ScaleMatrixAV(out, out, node->scale);
            out->row[3] = node->pos;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D9CB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct func_002D9CB0_sSlot {
    int* av;
    int pad4[5];
    float t;
    int isActive() { return av != 0; }
};
extern func_002D9CB0_sSlot D_00538938_9CB0[16] __asm__("D_00538938");

struct func_002D9CB0_sSave {
    int ids[16];
    float times[16];
};

class func_002D9CB0_cStream {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* data, int size);
};

extern "C" void func_002D9CB0(func_002D9CB0_cStream* stream)
{
    func_002D9CB0_sSave state;
    for (int i = 0; i < 16; i++) {
        state.ids[i] = D_00538938_9CB0[i].isActive() ? *D_00538938_9CB0[i].av : -1;
        state.times[i] = D_00538938_9CB0[i].isActive() ? D_00538938_9CB0[i].t : 0.0f;
    }
    stream->v01(&state, 0x80);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", cAvalanche_readFromReplayFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVEntry_RR {
    short delta;
    short index;
    void (*fn)(void*, int*, int);
};
struct sFrame_RR {
    char b[96];
};
struct sActRR {
    char pad_0x0[0x2E0];
    char* data;                     // 0x2E0
    sActRR* next;                   // 0x2E4
    int pad_0x2E8;
    int f2EC;                       // 0x2EC
};
struct sKeyRR { float t; int v; };
struct sKey2RR { unsigned short t; unsigned short v; };
struct sDataRR {
    char pad_0x0[0xC];
    unsigned short n0;              // 0xC
    unsigned short n1;              // 0xE
    sKeyRR a[32];                   // 0x10
    sKey2RR b[1];                   // 0x110
};
struct sSlotRR {
    sDataRR* group;                    // 0x0
    sActRR* active;                 // 0x4
    int pad_0x8;
    int pad_0xC;
    int cur0;                       // 0x10
    int cur1;                       // 0x14
    float time;                     // 0x18
};
extern sSlotRR D_slots_RR[] __asm__("D_00538938");
extern "C" int cAvalanche_triggerAvalanche(int id);
extern "C" void func_002D81B0(void* slot);
extern "C" void tActiveAvalancheNode_calculateScale(void* self, float t);
sFrame_RR getFrameData_RR(void* self, float t) __asm__("tActiveAvalancheNode_getFrameData");

static inline sSlotRR* slots_RR()
{
    return D_slots_RR;
}

static inline void clearSlot_RR(sSlotRR* s)
{
    if (s->group != 0) {
        func_002D81B0(s);
    }
}

extern "C" void cAvalanche_readFromReplayFrame(int* self)
{
    struct {
        int ids[16];
        float times[16];
    } buf;
    int i;
    for (i = 0; i < 16; i++) {
        clearSlot_RR(&slots_RR()[i]);
    }
    {
        sVEntry_RR* e = &(*(sVEntry_RR**)self)[2];
        e->fn((char*)self + e->delta, buf.ids, 0x80);
    }
    for (i = 0; i < 16; i++) {
        if (buf.ids[i] >= 0) {
            int n = cAvalanche_triggerAvalanche(buf.ids[i]);
            if (n >= 0) {
                sSlotRR* s = &slots_RR()[n];
                s->time = buf.times[i];
                if (buf.times[i] != 0.0f) {
                    sActRR* a = s->active;
                    while (a != 0) {
                        if (*(unsigned char*)(a->data + 0xF2) != 0) {
                            a->f2EC = 1;
                        }
                        tActiveAvalancheNode_calculateScale(a, buf.times[i]);
                        getFrameData_RR(a, s->time);
                        a = a->next;
                    }
                    while (s->cur0 < s->group->n0 && s->group->a[s->cur0].t <= s->time) {
                        s->cur0++;
                    }
                    while (s->cur1 < s->group->n1 && (float)s->group->b[s->cur1].t <= s->time) {
                        s->cur1++;
                    }
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D9FB8);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void cAvalanche_resolveDataPointers(void);
extern void* D_004A3AB4;
extern int D_004A3AB8;
extern int D_004A3AC4;

extern "C" void func_002D9FB8(void* data)
{
    cAvalanche_reset(data);
    if (D_004A3AB4 != 0 && D_004A3AB8 != 0) {
        cMemMan_free(D_004A3AB4);
    }
    D_004A3AB4 = data;
    D_004A3AB8 = 0;
    if (*(int*)data != 0x2BEEF00) {
        D_004A3AB4 = 0;
        D_004A3AC4 = 1;
    } else {
        D_004A3AC4 = 0;
        cAvalanche_resolveDataPointers();
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", cAvalanche_resolveDataPointers);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern void* D_004A3AB0;
extern void* D_004A3AB4;
extern int D_004A3ABC;

struct sAvVec4_A028 {
    float x, y, z, w;
    sAvVec4_A028() {}
    sAvVec4_A028(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));
struct sAvNodeA028 {
    char pad0[0xE0];
    sAvVec4_A028 pos;           // 0xE0
    unsigned short type;        // 0xF0
    unsigned char emit;         // 0xF2
    unsigned char owned;        // 0xF3
    sAvNodeA028* next;          // 0xF4
    int id;                     // 0xF8
    char* data;                 // 0xFC
    float count;                // 0x100
};
struct sAvPairA028 { float v; int f4; };
struct sAvIdxA028 { unsigned short a; unsigned short b; };
struct sAvGroupA028 {
    int f0;
    sAvNodeA028* nodes;         // 0x4
    sAvGroupA028* next;         // 0x8
    unsigned short nPairs;      // 0xC
    unsigned short nIdx;        // 0xE
    sAvPairA028 pairs[32];      // 0x10
    sAvIdxA028 idx[1];          // 0x110
};

extern "C" void cAvalanche_resolveDataPointers(void)
{
    if (D_004A3AB0 == 0)
        return;
    if (D_004A3AB4 == 0)
        return;
    if (D_004A3ABC != 0)
        return;
    char* p = (char*)D_004A3AB4 + 8;
    for (sAvGroupA028* g = (sAvGroupA028*)D_004A3AB0; g != 0; g = g->next) {
        for (sAvNodeA028* n = g->nodes; n != 0; n = n->next) {
            p += 4;
            float* f = (float*)p;
            n->pos = sAvVec4_A028(f[0], f[1], f[2], 1.0f);
            p += 12;
            if (n->owned && n->data)
                cMemMan_free(n->data);
            n->data = p;
            n->owned = 0;
            p += (int)n->count * 10;
        }
        g->nPairs = *(unsigned short*)p;
        p += 2;
        g->nIdx = *(unsigned short*)p;
        p += 2;
        for (int i = 0; i < g->nPairs; i++) {
            g->pairs[i].v = *(float*)p;
            g->pairs[i].f4 = 0;
            p += 8;
        }
        for (int j = 0; j < g->nIdx; j++) {
            g->idx[j].b = *(unsigned short*)p;
            p += 2;
            g->idx[j].a = *(unsigned short*)p;
            p += 2;
        }
        g->nPairs = 0;
    }
    D_004A3ABC = 1;
}
#endif

//100%
INCLUDE_ASM("visualfx/avalanche", func_002DA1C0);
#ifdef SKIP_ASM
struct sVec4_A1C0 {
    float x, y, z, w;
    sVec4_A1C0() {}
    sVec4_A1C0(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));
struct sVec3_A1C0 { float x, y, z; };
struct sAvInfo_A1C0 {
    sVec4_A1C0 center;          // 0x0
    float total;                // 0x10
    float minDist;              // 0x14
    int count;                  // 0x18
    bool any;                   // 0x1C
};
struct sAvNode_A1C0 {
    char pad0[0x60];
    sVec4_A1C0 pos;             // 0x60
    char pad70[0xB0 - 0x70];
    float weight;               // 0xB0
    char padB4[0x2E0 - 0xB4];
    char* src;                  // 0x2E0
    sAvNode_A1C0* next;         // 0x2E4
};
extern sVec4_A1C0 D_004FF120;

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4_A1C0 vSub_A1C0(const sVec4_A1C0& a, const sVec4_A1C0& b)
{
    sVec4_A1C0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_A1C0(const sVec4_A1C0& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vAddEq_A1C0(sVec4_A1C0& dst, const sVec4_A1C0& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void vDivEq_A1C0(sVec4_A1C0& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s));
}

// PORT: g++ `<?` (min) operator.
extern "C" sAvInfo_A1C0 func_002DA1C0(sVec3_A1C0 p)
{
    sAvInfo_A1C0 r;
    r.center = D_004FF120;
    r.minDist = 999999.0f;
    r.count = 0;
    r.total = 0.0f;
    r.any = 0;
    for (int i = 0; i < 16; i++) {
        sAvalancheSlot* s = &D_00538938[i];
        if (s->active == 0)
            continue;
        for (sAvNode_A1C0* n = *(sAvNode_A1C0**)s->pad; n != 0; n = n->next) {
            if (n->src == 0)
                continue;
            sVec4_A1C0 d = vSub_A1C0(sVec4_A1C0(p.x, p.y, p.z, 1.0f), n->pos);
            float dist = vLen_A1C0(d);
            if (dist < 225000000.0f) {
                vAddEq_A1C0(r.center, n->pos);
                r.total += n->weight;
                r.minDist = r.minDist <? dist;
                r.any |= *(unsigned short*)(n->src + 0xF0) == 2;
                r.count++;
            }
        }
    }
    vDivEq_A1C0(r.center, (float)r.count);
    r.total = r.total / (float)r.count;
    return r;
}
#endif

