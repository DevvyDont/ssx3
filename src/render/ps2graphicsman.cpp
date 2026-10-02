#include "common.h"

//100%
INCLUDE_ASM("render/ps2graphicsman", cPSPGraphicsMan_NewNonBindTexID);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_00492068[];

struct cTexSlots367150 {
    int unk_0x0;
    int count;
    void* slots[2000];
    void** freeHead;
};

extern "C" int cPSPGraphicsMan_NewNonBindTexID(cTexSlots367150* self)
{
    void** head = self->freeHead;
    int id = head - self->slots;
    self->freeHead = (void**)*head;
    self->count++;
    char* tex = (char*)self + 8;
    void** slot = (void**)(tex + (id << 2));
    *slot = cMemMan_alloc(0x58, D_00492068, 0x21000000, 0);
    return id;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003671C8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

struct cTexSlots3671C8 {
    int unk_0x0;
    int count;
    void* slots[2000];
    void* freeHead;
};

extern "C" void func_003671C8(cTexSlots3671C8* self, int id)
{
    operator_delete((int*)self->slots[id]);
    self->slots[id] = self->freeHead;
    self->freeHead = &self->slots[id];
    self->count--;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367230);
#ifdef SKIP_ASM
extern "C" void func_00367230(void)
{
    for (int i = 0; i < 500; i++) {
    }
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cPSPGraphicsMan_NewBindTexID);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_00492078[];

extern "C" void cPSPGraphicsMan_NewBindTexID(void* self, int id)
{
    char* tex = (char*)self + 8;
    void** slot = (void**)(tex + (id << 2));
    *slot = cMemMan_alloc(0x58, D_00492078, 0x21000000, 0);
    *(int*)self += 1;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003672C0);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

struct sGmPtrTable {
    int count;
    int pad_0x4;
    int* entries[1];
};

extern "C" void func_003672C0(sGmPtrTable* self, int idx)
{
    int** base = self->entries; int** e = base + idx;
    operator_delete(*e);
    *e = 0;
    self->count--;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367310);
#ifdef SKIP_ASM
extern "C" void func_00367310(void)
{
    for (int i = 0; i < 1500; i++) {
    }
}
#endif

extern "C" void* func_00365E40(void*, int, int, void*);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367340__FPvii);
#ifdef SKIP_ASM
void* func_00367340(void* self, int a1, int a2)
{
    return func_00365E40((char*)self + 0x4350, a1, a2, (char*)self + 0x8);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367360);
#ifdef SKIP_ASM
extern "C" void func_00366548(void* p);
// PORT: the unit defines func_00367310(void), but this caller passes self.
void func_00367310_1(void* self) __asm__("func_00367310");

extern "C" void func_00367360(void* self)
{
    int i;
    func_00367230();
    func_00367310_1(self);
    func_00366548((char*)self + 0x1F60);
    func_00366548((char*)self + 0x4350);
    for (i = 1; i >= 0; i--) {
        ((int*)((char*)self + 0x1F4C))[i] = -1;
    }
    *(int*)((char*)self + 0x1F54) = -1;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003673D0__FPv);
#ifdef SKIP_ASM
void func_003673D0(void* self)
{
}
#endif

INCLUDE_ASM("render/ps2graphicsman", func_003673D8);

INCLUDE_ASM("render/ps2graphicsman", func_00367440);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367B60);
#ifdef SKIP_ASM
extern "C" void func_00367B60(void* self, int i)
{
    char* obj = *(char**)((char*)self + (i << 2) + 8);
    if (*(int*)(obj + 0x28) != -1) {
        char* sub = *(int*)(obj + 0xC) != 9 ? (char*)self + 0x1F60 : (char*)self + 0x4350;
        char* e = *(char**)(sub + 0x1FF0) + *(int*)(obj + 0x30) * 0x1C;
        *(int*)e &= ~2;
    }
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367BC0);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
extern "C" void func_003663D8(void* self, int id);

struct sGmTex7BC0 {
    int owned;              // 0x0
    char pad_0x4[0x8];
    int kind;               // 0xC
    char pad_0x10[0xC];
    void* bufA;             // 0x1C
    void* bufB;             // 0x20
    char pad_0x24[0x4];
    int slotA;              // 0x28
    int slotB;              // 0x2C
    int idA;                // 0x30
    int idB;                // 0x34
};

struct sGmMan7BC0 {
    int count;
    int pad_0x4;
    sGmTex7BC0* texs[1];    // 0x8
};

static inline void fill7BC0(int* p, int v)
{
    for (int i = 1; i >= 0; i--) {
        p[i] = v;
    }
}

extern "C" void func_00367BC0(sGmMan7BC0* self, int id)
{
    char* s = (char*)self;
    sGmTex7BC0* t = self->texs[id];
    if (t->slotA != -1) {
        func_003663D8(t->kind != 9 ? s + 0x1F60 : s + 0x4350, t->idA);
    }
    if (t->slotB != -1) {
        func_003663D8(s + 0x1F60, t->idB);
    }
    if (t->owned != 0) {
        if (t->bufA != 0) {
            cMemMan_free(t->bufA);
        }
        if (t->bufB != 0) {
            cMemMan_free(t->bufB);
        }
    }
    if (id < 0x5DC) {
        func_003672C0((sGmPtrTable*)self, id);
    } else {
        func_003671C8((cTexSlots3671C8*)self, id);
    }
    fill7BC0((int*)(s + 0x1F4C), -1);
    *(int*)(s + 0x1F54) = -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/ps2graphicsman", func_00367CD0);
#ifdef SKIP_ASM
extern "C" int func_00367440(void* self, int a1, void* a2, int a3, int a4, int a5, int a6, int a7,
                             int a8, int a9, int a10, int a11);
extern char D_004A4068[];

extern "C" int func_00367CD0(void* self, int a, int b, int c)
{
    return func_00367440(self, 0, D_004A4068, a, b, 0, c, 0, 0, 3, 0, -1);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367D20);
#ifdef SKIP_ASM
extern "C" void* func_003E6574(void*, void*, int);
extern "C" void func_00369098(void* self, int idx);

extern "C" void func_00367D20(void* self, int idx, void* data, int a3, int a4, int a5, int a6,
                              int a7, void* clut, int upload)
{
    char* e = *(char**)((char*)self + (idx << 2) + 8);
    if (upload != 0) {
        if (data != 0) {
            func_003E6574(*(void**)(e + 0x1C), data, *(int*)(e + 0x10));
        }
        if (clut != 0) {
            func_003E6574(*(void**)(e + 0x20), clut, 4 << *(int*)(e + 0x18));
        }
    }
    func_00369098(self, idx);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367DB8);
#ifdef SKIP_ASM
// PORT: 64-bit GS register words (ulong is 64-bit here).
struct sGsTex0Bits {
    ulong TBP0 : 14;
    ulong TBW : 6;
    ulong PSM : 6;
    ulong TW : 4;
    ulong TH : 4;
    ulong TCC : 1;
    ulong TFX : 2;
    ulong CBP : 14;
    ulong CPSM : 4;
    ulong CSM : 1;
    ulong CSA : 5;
    ulong CLD : 3;
};

struct sPs2RenderTex {
    char pad_0x00[0x28];
    int addr;               // 0x28
    char pad_0x2C[0xC];
    sGsTex0Bits tex0;       // 0x38
};

struct sPs2TexSet {
    char pad_0x00[0x8];
    sPs2RenderTex* tex[1];  // 0x08
};

static inline ulong gsSetFrame367DB8(int fbp, int fbw, int psm, int fbmsk)
{
    return (ulong)fbp | ((ulong)fbw << 16) | ((ulong)psm << 24) | ((ulong)fbmsk << 32);
}

extern "C" void func_00367DB8(sPs2TexSet* self, int idx, ulong** pp)
{
    sPs2RenderTex* t = self->tex[idx];
    ulong* p = *pp;
    p[0] = 0x10000005;
    p[1] = 0;
    p[2] = (ulong)0x8800 << 45;
    p[3] = (ulong)0x50000004 << 32;
    p[4] = ((ulong)0x10000000 << 32) | 0x8003;
    p[5] = 0xE;
    p[6] = gsSetFrame367DB8(t->addr >> 5, (1 << t->tex0.TW) >> 6, t->tex0.PSM, 0);
    p[7] = 0x4C;
    p[8] = ((ulong)((1 << t->tex0.TW) - 1) << 16) | ((ulong)((1 << t->tex0.TH) - 1) << 48);
    p[9] = 0x40;
    p[10] = (ulong)((0x800 - ((1 << t->tex0.TW) >> 1)) << 4) |
            ((ulong)((0x800 - ((1 << t->tex0.TH) >> 1)) << 4) << 32);
    p[11] = 0x18;
    p += 12;
    *pp = p;
}
#endif

INCLUDE_ASM("render/ps2graphicsman", func_00367F18);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00368138);
#ifdef SKIP_ASM
extern char D_0044B200[];

// PORT: 64-bit `ulong` DMA tag, pointer packed into the upper word
extern "C" void func_00368138(void* self, ulong** pkt)
{
    (*pkt)[0] = ((ulong)(int)D_0044B200 << 32) | 0x30000022;
    (*pkt)[1] = 0;
    *pkt += 2;
}
#endif

INCLUDE_ASM("render/ps2graphicsman", func_00368170);

INCLUDE_ASM("render/ps2graphicsman", func_003684F0);

INCLUDE_ASM("render/ps2graphicsman", func_00368660);

INCLUDE_ASM("render/ps2graphicsman", func_00368970);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00369098);
#ifdef SKIP_ASM
extern "C" void func_00366E30(void* list, int i);

extern "C" void func_00369098(void* self, int idx)
{
    char* e = *(char**)((char*)self + (idx << 2) + 8);
    if (*(int*)(e + 0x28) != -1) {
        if (*(int*)(e + 0xC) != 9) {
            func_00366E30((char*)self + 0x1F60, idx);
        } else {
            func_00366E30((char*)self + 0x4350, idx);
        }
    }
    int v = -1;
    int* a = (int*)((char*)self + 0x1F4C);
    for (int i = 1; i >= 0; i--) {
        a[i] = v;
    }
    *(int*)((char*)self + 0x1F54) = -1;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00369130);
#ifdef SKIP_ASM
// PORT: 64-bit `ulong` bitfield (GS TEX1 register layout)
struct sGsTex1 {
    ulong LCM : 1;
    ulong pad0 : 1;
    ulong MXL : 3;
    ulong MMAG : 1;
    ulong MMIN : 3;
    ulong MTBA : 1;
    ulong pad1 : 9;
    ulong L : 2;
    ulong pad2 : 11;
    ulong K : 12;
    ulong pad3 : 20;
};

struct sGfxTex1 {
    char pad[0x40];
    sGsTex1 tex1;
};

struct sGfxTexTable1 {
    int pad[2];
    sGfxTex1* entries[1];
};

extern "C" void func_00369130(sGfxTexTable1* self, int idx, int mmin, int mmag, int l, int k)
{
    sGfxTex1* e = self->entries[idx];
    sGsTex1 t = e->tex1;
    t.L = l;
    t.K = k;
    t.MMIN = mmin;
    t.MMAG = mmag;
    e->tex1 = t;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003691B0);
#ifdef SKIP_ASM
// PORT: 64-bit `ulong` bitfield (GS TEX0 register layout)
struct sGsTex0 {
    ulong TBP0 : 14;
    ulong TBW : 6;
    ulong PSM : 6;
    ulong TW : 4;
    ulong TH : 4;
    ulong TCC : 1;
    ulong TFX : 2;
    ulong CBP : 14;
    ulong CPSM : 4;
    ulong CSM : 1;
    ulong CSA : 5;
    ulong CLD : 3;
};

struct sGfxTex {
    char pad[0x38];
    sGsTex0 tex0;
};

struct sGfxTexTable {
    int pad[2];
    sGfxTex* entries[1];
};

extern "C" void func_003691B0(sGfxTexTable* self, int idx, int tfx)
{
    if (idx != -1) {
        self->entries[idx]->tex0.TFX = tfx;
    }
}
#endif

INCLUDE_ASM("render/ps2graphicsman", func_003691F8);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003695D8);
#ifdef SKIP_ASM
extern void* D_004A5B80;
extern void* D_00493950[];
void operator_delete(int* ptr);

extern "C" void func_003695D8(void* self, int flags)
{
    *(void***)((char*)self + 0x10D8) = D_00493950;
    D_004A5B80 = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00369610);
#ifdef SKIP_ASM
struct sVEntry00369610 {
    short delta;
    short index;
    void (*fn)(void*, int*);
};

extern "C" void func_00369610(void* self, int* mask)
{
    int i;
    unsigned int j;
    for (i = 0; i < 4; i++) {
        ((int*)((char*)self + 0xED8))[i] = ~mask[i];
    }
    for (j = 0; j < 2; j++) {
        ((short*)((char*)self + 0xEE8))[j] = 0x7FFF;
    }
    sVEntry00369610* vt = *(sVEntry00369610**)((char*)self + 0x10D8);
    vt[62].fn((char*)self + vt[62].delta, mask);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00369690);
#ifdef SKIP_ASM
extern "C" void* func_003E6574(void*, void*, int);

extern "C" void func_00369690(void* self, int count, void* src)
{
    *(int*)((char*)self + 0xE80) = count;
    func_003E6574((char*)self + 0x280, src, count * 0xC);
}
#endif

INCLUDE_ASM("render/ps2graphicsman", func_003696C8);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00369890__FPvii);
#ifdef SKIP_ASM
void func_00369890(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0x27c) = a2;
    *(int*)((char*)self + 0x278) = a1;
    *(int*)((char*)self + 0xe80) = 0;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003698A0);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

extern "C" void func_003698A0(void* self)
{
    void* p = *(void**)((char*)self + 0xF48);
    if (p != 0) {
        cMemMan_free(p);
    }
    *(void**)((char*)self + 0xF48) = 0;
    *(int*)((char*)self + 0xF4C) = 0;
}
#endif

