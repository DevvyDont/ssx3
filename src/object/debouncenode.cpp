#include "common.h"

//100%
INCLUDE_ASM("object/debouncenode", cDebounceNode_cDebounceNode);
#ifdef SKIP_ASM
struct sDebounceVEntry2D10 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* cMoveNode_cMoveNode(void* self, void*, void*);
extern char D_004906F0[];

extern "C" void* cDebounceNode_cDebounceNode(void* self, void* unused, void* stream)
{
    cMoveNode_cMoveNode(self, unused, stream);
    *(void**)((char*)self + 0xC) = D_004906F0;
    sDebounceVEntry2D10* e = &(*(sDebounceVEntry2D10**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x2C, 4);
    e = &(*(sDebounceVEntry2D10**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x30, 4);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00342D88);
#ifdef SKIP_ASM
struct sDebounceVEntry2D88 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" int func_00342D88(void* self)
{
    int n = *(int*)((char*)self + 0x2C);
    if (n > 0) {
        n--;
        *(int*)((char*)self + 0x2C) = n;
        if (n == 0) {
            sDebounceVEntry2D88* vt = *(sDebounceVEntry2D88**)((char*)self + 0xC);
            vt[34].fn((char*)self + vt[34].delta, 1);
            return 0;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00342DD8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_003506D8(void* self, void* parent);
extern const char D_0048E738[];

struct sDebounceVEntry2DD8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00342DD8(void* self)
{
    if (*(int*)((char*)self + 0x2C) >= 0) {
        if (*(int*)((char*)self + 0x30) == 0) {
            void* parent = *(void**)((char*)self + 0x18);
            if (self != 0) {
                sDebounceVEntry2DD8* vt = *(sDebounceVEntry2DD8**)((char*)self + 0xC);
                vt[1].fn((char*)self + vt[1].delta, 3);
            }
            func_003506D8(cMemMan_alloc(0x1C, D_0048E738, 0x20000000, 0), parent);
        } else {
            if (self != 0) {
                sDebounceVEntry2DD8* vt = *(sDebounceVEntry2DD8**)((char*)self + 0xC);
                vt[1].fn((char*)self + vt[1].delta, 3);
            }
        }
    }
}
#endif

extern "C" void* func_00356B08(void* self);

//100%
INCLUDE_ASM("object/debouncenode", func_00342E78__FPv);
#ifdef SKIP_ASM
void* func_00342E78(void* self)
{
    return func_00356B08(self);
}
#endif

INCLUDE_ASM("object/debouncenode", func_00342E98);

//100%
INCLUDE_ASM("object/debouncenode", func_00342FA8);
#ifdef SKIP_ASM
struct sDebounceVEntry2FA8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00356B30(void* self, void* stream);

extern "C" void func_00342FA8(void* self, void* stream)
{
    func_00356B30(self, stream);
    sDebounceVEntry2FA8* e = &(*(sDebounceVEntry2FA8**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x2C, 4);
    e = &(*(sDebounceVEntry2FA8**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x30, 4);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00343010);
#ifdef SKIP_ASM
struct sVec2_3010 {
    float x;
    float y;
};

struct sDesc_3010 {
    int f0;
    float f4;
    float f8;
    float fC;
    int f10[8];
};

struct sFlags_3010 {
    char pad_0x00[0x8];
    unsigned int w;
};

struct sDebounce_3010 {
    char pad_0x00[0xC];
    char* vt;               // 0xC
    char pad_0x10[0x8];
    sFlags_3010* flags;     // 0x18
    float f1C;              // 0x1C
    unsigned char c20[8];   // 0x20
    char pad_0x28[0x10];
    sVec2_3010 v38;         // 0x38
    int f40;
    int f44;
};

extern char D_00490570[];
extern "C" void* func_0034FB00(void* self, void* a1, int type, void* a3);

extern "C" void* func_00343010(sDebounce_3010* self, void* a1, void* a2, sDesc_3010* desc)
{
    func_0034FB00(self, a1, 0xF, a2);
    self->vt = D_00490570;
    self->f1C = desc->f4;
    sVec2_3010 v;
    v.x = desc->f8;
    v.y = desc->fC;
    self->v38 = v;
    self->f40 = 0;
    self->f44 = 0;
    for (int i = 0; i < 8; i++) {
        self->c20[i] = desc->f10[i];
    }
    self->flags->w = (self->flags->w & ~2) | 4;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_003430D0);
#ifdef SKIP_ASM
struct sDebounceVEntry30D0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* cInstanceNode_cInstanceNode(void* self, void*, void*);
extern char D_00490570[];

extern "C" void* func_003430D0(void* self, void* unused, void* stream)
{
    cInstanceNode_cInstanceNode(self, unused, stream);
    *(void**)((char*)self + 0xC) = D_00490570;
    sDebounceVEntry30D0* vt = *(sDebounceVEntry30D0**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)self + 0x1C, 0x2C);
    return self;
}
#endif

INCLUDE_ASM("object/debouncenode", func_00343130);

//100%
INCLUDE_ASM("object/debouncenode", func_003434E8);
#ifdef SKIP_ASM
struct sDebounceScroll {
    char pad_0x0[0x38];
    float du; // 0x38
    float dv; // 0x3c
    float u;  // 0x40
    float v;  // 0x44
};

extern "C" void func_003434E8(sDebounceScroll* self)
{
    self->u += self->du;
    self->v += self->dv;
    if (self->u > 1.0f) {
        self->u -= 1.0f;
    } else if (self->u < -1.0f) {
        self->u += 1.0f;
    }
    if (self->v > 1.0f) {
        self->v -= 1.0f;
    } else if (self->v < -1.0f) {
        self->v += 1.0f;
    }
}
#endif

INCLUDE_ASM("object/debouncenode", func_00343588);

//100%
INCLUDE_ASM("object/debouncenode", func_00343718);
#ifdef SKIP_ASM
struct sDebounceVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0034FE90(void* self, void* stream);

extern "C" void func_00343718(void* self, void* stream)
{
    func_0034FE90(self, stream);
    sDebounceVEntry* vt = *(sDebounceVEntry**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x1C, 0x2C);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00343768);
#ifdef SKIP_ASM
extern "C" void* func_0034FB00(void* self, void* a1, int type, void* a3);
extern char D_004903F0[];

extern "C" void* func_00343768(void* self, void* a1, void* a2, void* a3)
{
    func_0034FB00(self, a1, 7, a2);
    *(void**)((char*)self + 0xC) = D_004903F0;
    *(int*)((char*)self + 0x1C) = *(short*)((char*)a3 + 0x4);
    *(int*)((char*)self + 0x20) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_003437C0);
#ifdef SKIP_ASM
struct sDebounceVEntry37C0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* cInstanceNode_cInstanceNode(void* self, void*, void*);
extern char D_004903F0[];

extern "C" void* func_003437C0(void* self, void* unused, void* stream)
{
    cInstanceNode_cInstanceNode(self, unused, stream);
    *(void**)((char*)self + 0xC) = D_004903F0;
    sDebounceVEntry37C0* vt = *(sDebounceVEntry37C0**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)self + 0x1C, 0x8);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00343820);
#ifdef SKIP_ASM
struct sDebounceVEntry1 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00343820(void* self)
{
    if (*(int*)((char*)self + 0x1C) == 0) {
        *(int*)((char*)self + 0x1C) = -1;
        sDebounceVEntry1* vt = *(sDebounceVEntry1**)((char*)self + 0xC);
        vt[34].fn((char*)self + vt[34].delta, 1);
    }
}
#endif

INCLUDE_ASM("object/debouncenode", func_00343868);

//100%
INCLUDE_ASM("object/debouncenode", func_00343A18);
#ifdef SKIP_ASM
struct sDebounceVEntryA18 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0034FE90(void* self, void* stream);

extern "C" void func_00343A18(void* self, void* stream)
{
    func_0034FE90(self, stream);
    sDebounceVEntryA18* vt = *(sDebounceVEntryA18**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x1C, 0x8);
}
#endif

INCLUDE_ASM("object/debouncenode", func_00343A68);

INCLUDE_ASM("object/debouncenode", func_00343B28);

//100%
INCLUDE_ASM("object/debouncenode", func_00343BC0);
#ifdef SKIP_ASM
struct sDebounceState;
extern "C" void func_003442E0(sDebounceState* self);
void* func_00344348(void* self);

struct sDebounceBlock {
    char data[0x1F4];
};

extern "C" void func_00343BC0(sDebounceBlock* self)
{
    int i;
    for (i = 0; i < 5; i++) {
        func_00344348(&self[i]);
        func_003442E0((sDebounceState*)&self[i]);
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00343C08);
#ifdef SKIP_ASM
struct sDebounceElem3C08 {
    char data[0x1F4];
};

extern "C" void func_00344898(void* elem, void* arg);

extern "C" void func_00343C08(sDebounceElem3C08* self, void* arg)
{
    int i;
    for (i = 0; i < 5; i++) {
        func_00344898(&self[i], arg);
    }
}
#endif

INCLUDE_ASM("object/debouncenode", func_00343C60);

INCLUDE_ASM("object/debouncenode", func_00343F38);

//100%
INCLUDE_ASM("object/debouncenode", func_003440C8);
#ifdef SKIP_ASM
struct sDebounceElem40C8 {
    int field_0x0;
    unsigned int field_0x4;
    unsigned int field_0x8;
    char pad_0xc[0x1E8];
};

extern "C" void func_003449F0(void* elem);

extern "C" void func_003440C8(sDebounceElem40C8* self)
{
    int i;
    sDebounceElem40C8* p = self;
    for (i = 0; i < 5; i++) {
        if (self[i].field_0x8 != 0xFFFFFFFF) {
            func_003449F0(p);
        }
        p++;
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00344138);
#ifdef SKIP_ASM
struct sDebounceElem4138 {
    int field_0x0;
    unsigned int field_0x4;
    unsigned int field_0x8;
    char pad_0xc[0x1E8];
};

extern "C" void func_00344E18(void* elem);

extern "C" void func_00344138(sDebounceElem4138* self)
{
    int i;
    sDebounceElem4138* p = self;
    for (i = 0; i < 5; i++) {
        if (self[i].field_0x8 != 0xFFFFFFFF) {
            func_00344E18(p);
        }
        p++;
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_003441A8);
#ifdef SKIP_ASM
class cDebounceStream;
extern "C" void func_00344FC0(void* self, cDebounceStream* s);

struct sDebounceElem41A8 {
    int field_0x0;
    unsigned int field_0x4;
    unsigned int field_0x8;
    char pad_0xc[0x1E8];
};

struct sDebounceVEntry41A8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_003441A8(sDebounceElem41A8* self, cDebounceStream* s)
{
    int i;
    for (i = 0; i < 5; i++) {
        sDebounceVEntry41A8* e = &(*(sDebounceVEntry41A8**)s)[1];
        e->fn((char*)s + e->delta, &self[i].field_0x8, 4);
        if (self[i].field_0x8 != 0xFFFFFFFF) {
            func_00344FC0(&self[i], s);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/debouncenode", func_00344240);
#ifdef SKIP_ASM
class cDebounceStream;
extern "C" void func_00344FF8(void* self, cDebounceStream* s);

struct sDebounceElem4240 {
    int field_0x0;
    unsigned int field_0x4;
    unsigned int field_0x8;
    char pad_0xc[0x1E8];
};

struct sDebounceVEntry4240 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00344240(sDebounceElem4240* self, cDebounceStream* s)
{
    int i;
    func_00343BC0((sDebounceBlock*)self);
    for (i = 0; i < 5; i++) {
        sDebounceVEntry4240* e = &(*(sDebounceVEntry4240**)s)[2];
        e->fn((char*)s + e->delta, &self[i].field_0x8, 4);
        if (self[i].field_0x8 != 0xFFFFFFFF) {
            func_00344FF8(&self[i], s);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_003442E0);
#ifdef SKIP_ASM
struct sDebounceSlot {
    unsigned int id; // 0x0
    int value;       // 0x4
};

struct sDebounceState {
    int field_0x0;            // 0x000
    unsigned int field_0x4;   // 0x004
    unsigned int field_0x8;   // 0x008
    sDebounceSlot slots[32];  // 0x00c
    int field_0x10c;          // 0x10c
    int field_0x110;          // 0x110
    int field_0x114;          // 0x114
    char pad_0x118[0xc4];     // 0x118
    int field_0x1dc;          // 0x1dc
    int field_0x1e0;          // 0x1e0
    int field_0x1e4;          // 0x1e4
    int field_0x1e8;          // 0x1e8
    int field_0x1ec;          // 0x1ec
    int field_0x1f0;          // 0x1f0
};

extern "C" void func_003442E0(sDebounceState* self)
{
    int i;
    self->field_0x0 = 0;
    self->field_0x1dc = 0;
    self->field_0x8 = 0xFFFFFFFF;
    self->field_0x4 = 0xFFFFFFFF;
    self->field_0x1e8 = 0;
    self->field_0x1ec = 0;
    self->field_0x1f0 = 0;
    self->field_0x114 = 0;
    self->field_0x110 = 0;
    for (i = 0; i < 32; i++) {
        self->slots[i].id = 0xFFFFFFFF;
        self->slots[i].value = 0;
    }
}
#endif

extern "C" void* func_00344800(void* self);

//100%
INCLUDE_ASM("object/debouncenode", func_00344348__FPv);
#ifdef SKIP_ASM
void* func_00344348(void* self)
{
    return func_00344800(self);
}
#endif

INCLUDE_ASM("object/debouncenode", func_00344368);

INCLUDE_ASM("object/debouncenode", func_003443A8);

//100%
INCLUDE_ASM("object/debouncenode", func_00344730);
#ifdef SKIP_ASM
struct sTrackWorld51C0;
extern "C" sTrackWorld51C0** func_002D1BD8();

struct sItem4730 {
    char pad_0x0[0x10];
    char* data;             // 0x10
};

struct sItemList4730 {
    int count;              // 0x0
    sItem4730* items[1];    // 0x4
};

struct sModel4730 {
    char pad_0x0[0x94];
    sItemList4730* list;    // 0x94
};

static inline sModel4730* refToPtr4730(unsigned int p)
{
    return (sModel4730*)(p << 2);
}

struct sModelSet4730 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C

    sModel4730* lookup(unsigned int idx)
    {
        unsigned int p = refs[idx] >> 8;
        if (p == 0) {
            return 0;
        }
        return refToPtr4730(p);
    }
};

struct sWorld4730 {
    char pad_0x0[0x8];
    sModelSet4730** sets;   // 0x8
};


struct sModelRef4730 {
    unsigned int id;

    sModel4730* get()
    {
        sModelSet4730* set = (*(sWorld4730**)func_002D1BD8())->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        return set->lookup(id >> 8);
    }
};

struct sSlot4730 {
    unsigned short value;   // 0x0
    char pad_0x2[0x12];
};

struct sDebounce4730 {
    int field_0x0;
    sModelRef4730 model;    // 0x4
    char pad_0x8[0x108];
    int count;              // 0x110
    char pad_0x114[0x28];
    sSlot4730 slots[1];     // 0x13C
};

extern "C" void func_00344730(sDebounce4730* self, int index)
{
    sModel4730* m = self->model.get();
    for (int i = 0; i < self->count; i++) {
        char* d = m->list->items[i]->data;
        if (d != 0) {
            self->slots[i].value = *(unsigned short*)(d + (index << 2) + 4);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00344800);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cMemMan_free(void*);

struct sDebounceVEntry4800 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sDebounceItem4800 {
    char pad_0x0[0x84];
    sDebounceVEntry4800* vt;    // 0x84
    char pad_0x88[0x48];
};

struct sDebounceOwner4800 {
    char pad_0x0[0x1E8];
    void* buf;                  // 0x1E8
    sDebounceItem4800* items;   // 0x1EC, array-new block (count at -0x10)
};

// The unit declares func_00344800 as `void* (void*)`, but the body returns nothing
// (its wrapper func_00344348 only forwards). Bound by asm label.
void func_00344800_v(sDebounceOwner4800* self) __asm__("func_00344800");

void func_00344800_v(sDebounceOwner4800* self)
{
    sDebounceItem4800* items = self->items;
    if (items != 0) {
        sDebounceItem4800* p = items + ((int*)items)[-4];
        while (self->items != p) {
            p--;
            p->vt[1].fn((char*)p + p->vt[1].delta, 0);
        }
        cMemMan_free((char*)self->items - 0x10);
    }
    if (self->buf != 0) {
        cMemMan_free(self->buf);
    }
    self->items = 0;
    self->buf = 0;
}
#endif

INCLUDE_ASM("object/debouncenode", func_00344898);

//100%
INCLUDE_ASM("object/debouncenode", func_003449F0);
#ifdef SKIP_ASM
struct sDebounceVEntry49F0 {
    short delta;
    short index;
    void (*fn)(void*, float);
};

struct sDebounceItem49F0 {
    char pad_0x0[0x84];
    sDebounceVEntry49F0* vt;    // 0x84
    char pad_0x88[0x48];
};

struct sDebounceOwner49F0 {
    char pad_0x0[0x10C];
    int count;                  // 0x10C
    char pad_0x110[0xCC];
    float time;                 // 0x1DC
    float period;               // 0x1E0
    float step;                 // 0x1E4
    void* buf;                  // 0x1E8
    sDebounceItem49F0* items;   // 0x1EC
};

extern "C" void func_00344AA0(sDebounceOwner49F0* self);

// PORT: pointer held in int for the element address (index-first addu); not 64-bit safe.
extern "C" void func_003449F0(void* elem)
{
    sDebounceOwner49F0* self = (sDebounceOwner49F0*)elem;
    float t = self->time + self->step;
    self->time = t;
    if (t > self->period) {
        self->time = t - self->period;
    }
    for (int i = 0; i < self->count; i++) {
        sDebounceItem49F0* p = (sDebounceItem49F0*)(i * 0xD0 + (int)self->items);
        p->vt[2].fn((char*)p + p->vt[2].delta, self->time);
    }
    func_00344AA0(self);
}
#endif

INCLUDE_ASM("object/debouncenode", func_00344AA0);

INCLUDE_ASM("object/debouncenode", func_00344E18);

//100%
INCLUDE_ASM("object/debouncenode", func_00344FC0);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
class cDebounceStream {
public:
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

extern "C" void func_00344FC0(void* self, cDebounceStream* s)
{
    s->v01(self, 0x10C);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00344FF8);
#ifdef SKIP_ASM
extern "C" void func_003443A8(void* self);

extern "C" void func_00344FF8(void* self, cDebounceStream* s)
{
    s->v02(self, 0x10C);
    func_00344800(self);
    func_003443A8(self);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00345048);
#ifdef SKIP_ASM
struct sDebVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sDebMtx {
    sDebVec4 r[4];
};

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (matrix * vector).
static inline sDebVec4 debMtxApply(sDebMtx* m, const sDebVec4& v)
{
    sDebVec4 out;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf8, %1\n"
        "lqc2      $vf4, 0x0(%2)\n"
        "lqc2      $vf5, 0x10(%2)\n"
        "lqc2      $vf6, 0x20(%2)\n"
        "lqc2      $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        ".set pop\n"
        : "=m"(out)
        : "m"(v), "r"(m)
        : "memory");
    return out;
}

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (v * s).
static inline sDebVec4 debVecScale(const sDebVec4& v, float s)
{
    sDebVec4 out;
    int t;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(out), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return out;
}

struct sDebCurve {
    char pad_0x00[0x10];
    sDebMtx basis;   // 0x10
    float coef[4];   // 0x50: time-warp cubic
    char pad_0x60[0x24];
    float t0;        // 0x84
};

// Evaluate the curve at time t: position, velocity and acceleration.
extern "C" void func_00345048(sDebCurve* self, float t, sDebVec4* pos, sDebVec4* vel, sDebVec4* acc)
{
    t -= self->t0;
    float u = ((self->coef[0] * t + self->coef[1]) * t + self->coef[2]) * t + self->coef[3];
    float du = (self->coef[0] * (t * 3.0f) + self->coef[1] * 2.0f) * t + self->coef[2];
    sDebVec4 a;
    a.x = u * 6.0f;
    a.y = 2.0f;
    a.z = 0.0f;
    a.w = 0.0f;
    *acc = debVecScale(debMtxApply(&self->basis, a), du);
    sDebVec4 p;
    p.x = u * u * u;
    p.y = u * u;
    p.z = u;
    p.w = 1.0f;
    *pos = debMtxApply(&self->basis, p);
    sDebVec4 v;
    v.x = u * u * 3.0f;
    v.y = u * 2.0f;
    v.z = 1.0f;
    v.w = 0.0f;
    *vel = debMtxApply(&self->basis, v);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_003451C0);
#ifdef SKIP_ASM
struct cSpline;
float cSpline_calcLength(cSpline* self);

struct sTrackNode51C0 {
    char pad_0x0[0x24];
    int value;              // 0x24
};

struct sTrackSet51C0 {
    char pad_0x0[0x44];
    unsigned int* refs;     // 0x44, (node >> 2) << 8 | low byte
};

struct sTrackWorld51C0 {
    char pad_0x0[0x8];
    sTrackSet51C0** sets;   // 0x8
};

extern "C" sTrackWorld51C0** func_002D1BD8();

static inline sTrackNode51C0* refToNode51C0(unsigned int p)
{
    return (sTrackNode51C0*)(p << 2);
}

struct sTrackRef51C0 {
    unsigned int id;

    sTrackNode51C0* get()
    {
        sTrackSet51C0* set = (*func_002D1BD8())->sets[id & 0xFF];
        if (set != 0) {
            unsigned int p = set->refs[id >> 8] >> 8;
            if (p != 0) {
                return refToNode51C0(p);
            }
        }
        return 0;
    }
};

struct sSplineFollow51C0 {
    sTrackRef51C0 ref;      // 0x0
    int field_0x4;          // 0x4
    int field_0x8;          // 0x8
    float length;           // 0xC
};

extern "C" void func_003451C0(sSplineFollow51C0* self, unsigned int id)
{
    self->ref.id = id;
    sTrackNode51C0* node = self->ref.get();
    self->field_0x8 = node->value;
    self->field_0x4 = 0;
    self->length = cSpline_calcLength((cSpline*)self);
}
#endif

INCLUDE_ASM("object/debouncenode", func_00345248);

INCLUDE_ASM("object/debouncenode", func_00345430);

