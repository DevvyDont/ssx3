#include "common.h"
#include "render/ps2graphicsman.h"

//100%
INCLUDE_ASM("render/ps2graphicsman", cPSPGraphicsMan_NewNonBindTexID);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_NBTexInfo[];

struct cTexSlots367150 {
    int unk_0x0;
    int count;
    void* slots[2000];
    void** freeHead;
};

/**
 * Takes a free texture id from the texture manager and allocates its texture info.
 *
 * Pops the head of the free list (+0x1F48, a pointer into the 2000-slot array at +0x8; each free slot holds
 * the next free pointer), counts it at +0x4 and stores a new 0x58-byte "NBTexInfo" in the slot. The free list
 * is not checked for empty. Despite the On Tour class name, self is the graphics manager's mpTextureMan
 * (func_00367440).
 *
 * @return the new texture id (its slot index)
 */
extern "C" int cPSPGraphicsMan_NewNonBindTexID(cTexSlots367150* self)
{
    void** head = self->freeHead;
    int id = head - self->slots;
    self->freeHead = (void**)*head;
    self->count++;
    char* tex = (char*)self + 8;
    void** slot = (void**)(tex + (id << 2));
    *slot = cMemMan_alloc(0x58, str_NBTexInfo, 0x21000000, 0);
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
extern const char str_BTexInfo[];

/**
 * Allocates the texture info for a texture id the caller chose.
 *
 * Stores a new 0x58-byte "BTexInfo" in slot @p id of the array at +0x8 and increments the bound count at
 * +0x0; the free list is not touched. self is the texture manager, as for cPSPGraphicsMan_NewNonBindTexID().
 *
 * @param id  texture id (slot index) to fill
 */
extern "C" void cPSPGraphicsMan_NewBindTexID(void* self, int id)
{
    char* tex = (char*)self + 8;
    void** slot = (void**)(tex + (id << 2));
    *slot = cMemMan_alloc(0x58, str_BTexInfo, 0x21000000, 0);
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
    int** base = self->entries; int** slot = base + idx;
    operator_delete(*slot);
    *slot = 0;
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
void* func_00367340(void* self, int start, int size)
{
    return func_00365E40((char*)self + 0x4350, start, size, (char*)self + 0x8);
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

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003673D8);
#ifdef SKIP_ASM
extern "C" int func_00366CE0(void* heap, int size, int idx, int other, int a4, int a5);

// Allocates a block in the texture heap and marks the matching entry as resident and locked.
extern "C" int func_003673D8(char* self, int size)
{
    int idx = func_00366CE0(self + 0x1F60, size, -1, -1, -1, 1);
    char* node = *(char**)(self + 0x3F50) + idx * 0x1C;
    *(int*)node |= 0x22;
    *(int*)(node + 8) = 0x7FFFFFFF;
    return *(int*)node >> 13;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367440);
#ifdef SKIP_ASM
extern "C" void cPSPGraphicsMan_NewBindTexID(void* self, int id);
extern "C" int func_00366CE0(void* heap, int size, int idx, int other, int a4, int a5);
extern "C" void func_00366E90(void* self, void* tex);
extern "C" void* MEM_copy(void*, void*, int);
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
// PORT: operator_new__FUi is the game's tagged allocator (size, tag, flags, d); bound by asm label
void* operator_new_7440(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

// PORT: PS2-only MMI count-leading-sign-bits (plzcw) = floor(log2(x)); off-PS2 use a clz builtin.
// Volatile: the target keeps each log2 in order (zero-extend, plzcw, truncate).
// Volatile: the target keeps each log2 in order (zero-extend, plzcw, truncate) before the next.
static inline int texLog2_7440(unsigned int x)
{
    long r;
    __asm__ __volatile__("plzcw %0, %1\n\txori %0, %0, 0x1f\n\taddiu %0, %0, -1" : "=r"(r) : "r"((unsigned long)x));
    return r;
}

// GS pixel storage mode -> bits per pixel
static inline int psmBpp_7440(int psm)
{
    switch (psm) {
    case 0x0:
    case 0x1:
    case 0x30:
    case 0x31:
        return 32;
    case 0x2:
    case 0xA:
    case 0x32:
    case 0x3A:
        return 16;
    case 0x13:
    case 0x1B:
        return 8;
    case 0x14:
    case 0x24:
    case 0x2C:
        return 4;
    }
    return 0;
}

static inline int imax_7440(int a, int b)
{
    return a < b ? b : a;
}

// VRAM bytes for one level: the image rounded up to whole GS pages, then to 256 bytes
static inline int vramBytes_7440(int w, int h, int bpp)
{
    int pw, ph;
    switch (bpp) {
    case 4:
        pw = 0x80;
        ph = 0x80;
        break;
    case 8:
        pw = 0x80;
        ph = 0x40;
        break;
    case 15:
    case 16:
        pw = 0x40;
        ph = 0x40;
        break;
    case 32:
    default:
        pw = 0x40;
        ph = 0x20;
        break;
    }
    int n = imax_7440(w, pw) * imax_7440(h, ph) * bpp / 8;
    return n + (-n & 0xFF);
}

// CLUT format -> bytes per CLUT entry
static inline int clutEntryBytes_7440(int f)
{
    switch (f) {
    case 0:
        return 0;
    case 1:
        return 4;
    case 2:
        return 2;
    }
    return 0;
}

// CLUT format -> GS CPSM
static inline int clutPsm_7440(int f)
{
    switch (f) {
    case 0:
        return 0;
    case 1:
        return 0;
    case 2:
        return 2;
    }
    return 0;
}

extern "C" int func_00367440(void* self, int a1, void* a2, int a3, int a4, int a5, int a6, int a7,
                             int a8, int vramMode, int ownsData, int bindId)
{
    char* texMan = (char*)self;
    void* data = (void*)a1;
    const char* name = (const char*)a2;
    unsigned int w = a3;
    unsigned int height = a4;
    int mip = a5;
    int fmt = a6;
    int clutFmt = a7;
    void* clut = (void*)a8;
    int id;
    if (bindId < 0) {
        id = cPSPGraphicsMan_NewNonBindTexID((cTexSlots367150*)self);
    } else {
        cPSPGraphicsMan_NewBindTexID(self, bindId);
        id = bindId;
    }
    char* tex = *(char**)(texMan + (id << 2) + 8);
    strncpy(tex + 4, name, 8);
    *(int*)(tex + 0x0) = ownsData;
    *(int*)(tex + 0xC) = fmt;
    *(int*)(tex + 0x28) = -1;
    *(int*)(tex + 0x30) = -1;
    *(int*)(tex + 0x2C) = -1;
    *(int*)(tex + 0x24) = mip;
    *(int*)(tex + 0x34) = -1;
    int psm;
    switch (fmt) {
    case 8:
        psm = 1;
        break;
    case 2:
        psm = 2;
        break;
    case 9:
    case 10:
        psm = 0x13;
        break;
    case 11:
    case 12:
        psm = 0x14;
        break;
    case 0:
    default:
        psm = 0;
        break;
    }
    int bpp = psmBpp_7440(psm);
    *(int*)(tex + 0x18) = bpp;
    if (bpp <= 0)
        return -1;
    if (data != 0) {
        int size = 0;
        int vram = 0;
        for (int lv = 0; lv <= mip; lv++) {
            int lw = w >> lv;
            int lh = height >> lv;
            int n = lw * lh * bpp / 8;
            if (n < 0x100)
                n = 0x100;
            size += n;
            vram += vramBytes_7440(lw, lh, bpp);
        }
        *(int*)(tex + 0x14) = vram;
        *(int*)(tex + 0x10) = size;
        if (*(int*)(tex + 0x0) != 0) {
            void* copy = operator_new_7440(size, name, *(int*)(texMan + 0x1F58) | 0x05000000, 0);
            *(void**)(tex + 0x1C) = copy;
            MEM_copy(copy, data, size);
        } else {
            *(void**)(tex + 0x1C) = data;
        }
    } else {
        *(void**)(tex + 0x1C) = 0;
        *(int*)(tex + 0x10) = 0;
        *(int*)(tex + 0x14) = vramBytes_7440(w, height, bpp);
    }
    int esz;
    if (clut != 0 && (esz = clutEntryBytes_7440(clutFmt)) != 0) {
        int entries = (unsigned int)(*(int*)(tex + 0xC) - 0xB) <= 1 ? 0x10 : 0x100;
        if (*(int*)(tex + 0x0) != 0) {
            void* copy = operator_new_7440(0x400, name, *(int*)(texMan + 0x1F58) | 0x05000000, 0);
            *(void**)(tex + 0x20) = copy;
            MEM_copy(copy, clut, entries * esz);
        } else {
            *(void**)(tex + 0x20) = clut;
        }
    } else {
        *(void**)(tex + 0x20) = 0;
    }
    int lcm = 0;
    int lodL = 0;
    int k = -0xB9;
    int mmin = 5;
    if (mip == 0) {
        lcm = 1;
        lodL = mip;
        k = 0;
        mmin = 1;
    }
    ulong* tex0p = (ulong*)(tex + 0x38);
    int lw2 = texLog2_7440(w - 1);
    int lh2 = texLog2_7440(height - 1);
    int cpsm = clutPsm_7440(clutFmt);
    *tex0p = (ulong)0 | ((ulong)((w + 63) >> 6) << 14) | ((ulong)psm << 20) | ((ulong)(lw2 + 1) << 26)
        | ((ulong)(lh2 + 1) << 30) | ((ulong)1 << 34) | ((ulong)0 << 35) | ((ulong)0 << 37)
        | ((ulong)cpsm << 51) | ((ulong)0 << 55) | ((ulong)0 << 56) | ((ulong)(bpp <= 8) << 61);
    *(ulong*)(tex + 0x40) = (ulong)lcm | ((ulong)(unsigned int)mip << 2) | ((ulong)1 << 5)
        | ((ulong)mmin << 6) | ((ulong)lodL << 19) | ((ulong)k << 32);
    if (mip != 0) {
        *(ulong*)(tex + 0x48) = ((ulong)((w + 0x7F) >> 7) << 14) | ((ulong)((w + 0xFF) >> 8) << 34)
            | ((ulong)((w + 0x1FF) >> 9) << 54);
        *(ulong*)(tex + 0x50) = ((ulong)((w + 0x3FF) >> 10) << 14) | ((ulong)((w + 0x7FF) >> 11) << 34)
            | ((ulong)((w + 0xFFF) >> 12) << 54);
    }
    if (vramMode == 1) {
        int node = func_00366CE0(texMan + 0x1F60, *(int*)(tex + 0x14), id, -1, -1, 1);
        int* nodeFlags = (int*)(*(char**)(texMan + 0x3F50) + node * 0x1C);
        int fl = *nodeFlags | 2;
        *nodeFlags = fl;
        *(int*)(tex + 0x30) = node;
        *(int*)(tex + 0x28) = fl >> 8;
    } else if (vramMode == 3) {
        int node = func_00366CE0(texMan + 0x1F60, *(int*)(tex + 0x14) + 0x2000, id, -1, -1, 1);
        int* nodeFlags = (int*)(*(char**)(texMan + 0x3F50) + node * 0x1C);
        int fl = *nodeFlags | 0x22;
        *nodeFlags = fl;
        *(int*)(tex + 0x30) = node;
        int vramAddr = fl >> 8;
        *(int*)(tex + 0x28) = vramAddr + (-vramAddr & 0x1F);
    }
    if (*(int*)(tex + 0x0) != 0) {
        if (*(int*)(tex + 0xC) == 9) {
            if (*(int*)(texMan + 0x6344) < *(int*)(tex + 0x14) && vramMode == 0)
                func_00366E90(self, tex);
            else
                *(ulong*)(tex + 0x38) = (*(ulong*)(tex + 0x38) & ~((ulong)0x3F << 20)) | ((ulong)0x1B << 20);
        }
    } else if (*(int*)(tex + 0xC) == 9) {
        *(ulong*)(tex + 0x38) = (*(ulong*)(tex + 0x38) & ~((ulong)0x3F << 20)) | ((ulong)0x1B << 20);
    }
    return id;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367B60);
#ifdef SKIP_ASM
extern "C" void func_00367B60(void* self, int i)
{
    char* obj = *(char**)((char*)self + (i << 2) + 8);
    if (*(int*)(obj + 0x28) != -1) {
        char* sub = *(int*)(obj + 0xC) != 9 ? (char*)self + 0x1F60 : (char*)self + 0x4350;
        char* node = *(char**)(sub + 0x1FF0) + *(int*)(obj + 0x30) * 0x1C;
        *(int*)node &= ~2;
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
    char* selfBytes = (char*)self;
    sGmTex7BC0* tex = self->texs[id];
    if (tex->slotA != -1) {
        func_003663D8(tex->kind != 9 ? selfBytes + 0x1F60 : selfBytes + 0x4350, tex->idA);
    }
    if (tex->slotB != -1) {
        func_003663D8(selfBytes + 0x1F60, tex->idB);
    }
    if (tex->owned != 0) {
        if (tex->bufA != 0) {
            cMemMan_free(tex->bufA);
        }
        if (tex->bufB != 0) {
            cMemMan_free(tex->bufB);
        }
    }
    if (id < 0x5DC) {
        func_003672C0((sGmPtrTable*)self, id);
    } else {
        func_003671C8((cTexSlots3671C8*)self, id);
    }
    fill7BC0((int*)(selfBytes + 0x1F4C), -1);
    *(int*)(selfBytes + 0x1F54) = -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/ps2graphicsman", func_00367CD0);
#ifdef SKIP_ASM
extern "C" int func_00367440(void* self, int a1, void* a2, int a3, int a4, int a5, int a6, int a7,
                             int a8, int a9, int a10, int a11);
extern char str_LockTex[];

extern "C" int func_00367CD0(void* self, int width, int height, int format)
{
    return func_00367440(self, 0, str_LockTex, width, height, 0, format, 0, 0, 3, 0, -1);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367D20);
#ifdef SKIP_ASM
extern "C" void* MEM_copy(void*, void*, int);
extern "C" void func_00369098(void* self, int idx);

extern "C" void func_00367D20(void* self, int idx, void* data, int width, int height, int a5, int a6,
                              int a7, void* clut, int upload)
{
    char* tex = *(char**)((char*)self + (idx << 2) + 8);
    if (upload != 0) {
        if (data != 0) {
            MEM_copy(*(void**)(tex + 0x1C), data, *(int*)(tex + 0x10));
        }
        if (clut != 0) {
            MEM_copy(*(void**)(tex + 0x20), clut, 4 << *(int*)(tex + 0x18));
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
    sPs2RenderTex* renderTex = self->tex[idx];
    ulong* packet = *pp;
    packet[0] = 0x10000005;
    packet[1] = 0;
    packet[2] = (ulong)0x8800 << 45;
    packet[3] = (ulong)0x50000004 << 32;
    packet[4] = ((ulong)0x10000000 << 32) | 0x8003;
    packet[5] = 0xE;
    packet[6] = gsSetFrame367DB8(renderTex->addr >> 5, (1 << renderTex->tex0.TW) >> 6, renderTex->tex0.PSM, 0);
    packet[7] = 0x4C;
    packet[8] = ((ulong)((1 << renderTex->tex0.TW) - 1) << 16) | ((ulong)((1 << renderTex->tex0.TH) - 1) << 48);
    packet[9] = 0x40;
    packet[10] = (ulong)((0x800 - ((1 << renderTex->tex0.TW) >> 1)) << 4) |
            ((ulong)((0x800 - ((1 << renderTex->tex0.TH) >> 1)) << 4) << 32);
    packet[11] = 0x18;
    packet += 12;
    *pp = packet;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367F18);
#ifdef SKIP_ASM
// PORT: 64-bit GS register words (ulong is 64-bit here).
struct sGsTex0Bits7F18 {
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

struct sPs2RenderTex7F18 {
    char pad_0x00[0x38];
    sGsTex0Bits7F18 tex0;   // 0x38
};

struct sPs2TexSet7F18 {
    char pad_0x00[0x8];
    sPs2RenderTex7F18* tex[1];  // 0x08
};

union sFloatBits7F18 {
    float f;
    int i;
};

// Draw a full-size sprite over render texture idx, tinted with color (a, r, g, b in 0..1).
extern "C" void func_00367F18(sPs2TexSet7F18* self, int idx, float* color, ulong** pp)
{
    sPs2RenderTex7F18* renderTex = self->tex[idx];
    ulong* packet = *pp;
    sFloatBits7F18 qValue;
    qValue.f = 1.0f;
    packet[0] = 0x10000006;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = (ulong)0x50000005 << 32;
    packet[4] = ((ulong)0x44000000 << 32) | 1;
    packet[5] = 0x5510;
    packet[6] = 0x10E;
    packet[7] = (ulong)(int)(color[1] * 128.0f) | ((ulong)(int)(color[2] * 128.0f) << 8) |
           ((ulong)(int)(color[3] * 128.0f) << 16) | ((ulong)(int)(color[0] * 128.0f) << 24) |
           ((ulong)qValue.i << 32);
    packet[8] = (ulong)((0x800 - ((1 << renderTex->tex0.TW) >> 1)) << 4) |
           ((ulong)((0x800 - ((1 << renderTex->tex0.TH) >> 1)) << 4) << 16) | ((ulong)1 << 32);
    packet[9] = (ulong)((((1 << renderTex->tex0.TW) >> 1) + 0x801) << 4) |
           ((ulong)((((1 << renderTex->tex0.TH) >> 1) + 0x801) << 4) << 16) | ((ulong)1 << 32);
    packet[10] = ((ulong)0x10000000 << 32) | 0x8001;
    packet[11] = 0xE;
    {
        ulong hi = ((ulong)((1 << renderTex->tex0.TH) - 2) << 48) | (((ulong)1 << 32) | 1);
        packet[12] = ((ulong)((1 << renderTex->tex0.TW) - 2) << 16) | hi;
    }
    packet[13] = 0x40;
    packet += 14;
    *pp = packet;
}
#endif

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

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00368170);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef int cQuad128_8170 __attribute__((mode(TI)));

struct sBXTex_8170 {
    char pad_0x00[0xC];
    int psm;                // 0xC
    char pad_0x10[0x8];
    int bpp;                // 0x18
    int src;                // 0x1C
    char pad_0x20[0x4];
    int levels;             // 0x24
    int dbp;                // 0x28
    char pad_0x2C[0xC];
    ulong tex0;             // 0x38

    int Width() { return 1 << (int)((tex0 >> 26) & 0xF); }
    int Height() { return 1 << (int)((tex0 >> 30) & 0xF); }
    int BPP() { return bpp; }
};

struct sTexMgr_8170 {
    char pad_0x0[0x8];
    sBXTex_8170* texs[1];   // 0x8
};

template <class T> static inline T Max_8170(T a, T b)
{
    return a >= b ? a : b;
}

// PORT: 64-bit `ulong` GIF/DMA packet words.
extern "C" void func_00368170(void* self_, int idx, ulong** dma, ulong** gif)
{
    sTexMgr_8170* self = (sTexMgr_8170*)self_;
    sBXTex_8170* texture = self->texs[idx];
    int psm = texture->psm;
    ulong* dmaCursor = *dma;
    int w = texture->Width();
    int height = texture->Height();
    int fmt = (int)((texture->tex0 >> 20) & 0x3F);
    ulong* gifCursor = *gif;
    int src = texture->src;
    int dbp = texture->dbp;
    int shift = 8;
    if (psm == 10) {
        fmt = 0;
        w >>= 1;
        height >>= 1;
    } else if (psm == 9) {
        fmt = 0x1B;
        shift = 6;
    } else if (psm == 12) {
        fmt = 0;
        if (w == height) {
            w >>= 1;
            height >>= 2;
        } else {
            w >>= 2;
            height >>= 1;
        }
    }
    for (int i = 0; i <= texture->levels; i++) {
        int lw = texture->Width() >> i;
        int lh = texture->Height() >> i;
        int size = Max_8170(lw * lh * texture->BPP() / 8, 256);
        int bpp = texture->BPP();
        int pw, ph;
        switch (bpp) {
        case 4:
            pw = 0x80;
            ph = 0x80;
            break;
        case 8:
            pw = 0x80;
            ph = 0x40;
            break;
        case 15:
        case 16:
            pw = 0x40;
            ph = 0x40;
            break;
        case 32:
        default:
            pw = 0x40;
            ph = 0x20;
            break;
        }
        int vram = Max_8170(lw, pw) * Max_8170(lh, ph) * bpp / 8;
        vram += -vram & 0xFF;
        *(cQuad128_8170*)gifCursor = 0x10000006;
        gifCursor[2] = ((ulong)0x10000000 << 32) | 4;
        gifCursor[3] = 0xE;
        gifCursor[4] = ((ulong)dbp << 32) | ((ulong)((w + 63) >> 6) << 48) | ((ulong)fmt << 56);
        gifCursor[5] = 0x50;
        gifCursor[6] = 0;
        gifCursor[7] = 0x51;
        gifCursor[8] = w | ((ulong)height << 32);
        gifCursor[9] = 0x52;
        gifCursor[10] = 0;
        gifCursor[11] = 0x53;
        int nloop = size >> 4;
        if (nloop < 1) nloop = 1;
        gifCursor[12] = nloop | ((ulong)2 << 58);
        gifCursor[13] = 0;
        int qwc = size >> 4;
        if (qwc < 1) qwc = 1;
        gifCursor[14] = qwc | 0x30000000 | ((ulong)src << 32);
        gifCursor[15] = 0;
        gifCursor[16] = 0x10000002;
        gifCursor[17] = 0;
        gifCursor[18] = i == texture->levels ? ((ulong)0x10000000 << 32) | 0x8001 : ((ulong)0x10000000 << 32) | 1;
        gifCursor[19] = 0xE;
        gifCursor[20] = 0;
        gifCursor[21] = 0x3F;
        src += size;
        dbp += vram >> shift;
        w >>= 1;
        height >>= 1;
        gifCursor += 22;
    }
    func_00368138(self, &dmaCursor);
    *dma = dmaCursor;
    *gif = gifCursor;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003684F0);
#ifdef SKIP_ASM
struct sPs2UpTex {
    char pad_0x00[0xC];
    unsigned int psm;       // 0xC
    char pad_0x10[0x10];
    int addr;               // 0x20
    char pad_0x24[0x8];
    int dbp;                // 0x2C
};

struct sPs2UpMgr {
    char pad_0x0[0x8];
    sPs2UpTex* texs[1];     // 0x8
};

// PORT: 64-bit `ulong` GIF/DMA packet words.
extern "C" void func_003684F0(sPs2UpMgr* self, int idx, ulong** dma, ulong** gif)
{
    sPs2UpTex* tex = self->texs[idx];
    ulong* gifCursor = *gif;
    ulong* dmaCursor = *dma;
    int w;
    int height;
    int qwc;
    if (tex->psm - 0xB < 2) {
        w = 8;
        height = 2;
        qwc = 0x10;
    } else {
        w = 0x10;
        height = 0x10;
        qwc = 0x40;
    }
    gifCursor[0] = 0x10000006;
    gifCursor[1] = 0;
    gifCursor[2] = ((ulong)0x10000000 << 32) | 4;
    gifCursor[3] = 0xE;
    gifCursor[4] = ((ulong)tex->dbp << 32) | ((ulong)1 << 48);
    gifCursor[5] = 0x50;
    gifCursor[6] = 0;
    gifCursor[7] = 0x51;
    gifCursor[8] = w | ((ulong)height << 32);
    gifCursor[9] = 0x52;
    gifCursor[10] = 0;
    gifCursor[11] = 0x53;
    gifCursor[12] = qwc | ((ulong)2 << 58);
    gifCursor[13] = 0;
    gifCursor[14] = 0x30000000 | qwc | ((ulong)tex->addr << 32);
    gifCursor[15] = 0;
    gifCursor[16] = 0x10000002;
    gifCursor[17] = 0;
    gifCursor[18] = ((ulong)0x10000000 << 32) | 0x8001;
    gifCursor[19] = 0xE;
    gifCursor[20] = 0;
    gifCursor[21] = 0x3F;
    func_00368138(self, &dmaCursor);
    gifCursor += 22;
    *dma = dmaCursor;
    *gif = gifCursor;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/ps2graphicsman", func_00368660);
#ifdef SKIP_ASM
extern "C" int func_00366CE0(void* heap, int size, int idx, int other, int a4, int a5);
extern "C" void func_00368170(void* self, int idx, ulong** dma, ulong** gif);
extern "C" void func_00368970(void* self, int idx, int slot, ulong** dma);
extern "C" ulong* func_0038F460(void* ring, unsigned int addr, int size, int flags);
extern "C" void* func_0038F668(void* ring, ulong* p, int a2);
extern void* D_004A4474;

struct sUpNode_8660 {
    int flags;
    int f4[6];
};

struct sUpHeap_8660 {
    char pad_0x0[0x1FF0];
    sUpNode_8660* nodes;    // 0x1FF0
};

struct sUpTex_8660 {
    char pad_0x00[0xC];
    unsigned int psm;       // 0xC
    char pad_0x10[0x4];
    int size;               // 0x14
    char pad_0x18[0x8];
    int clut;               // 0x20
    char pad_0x24[0x4];
    int addr;               // 0x28
    int caddr;              // 0x2C
    int node;               // 0x30
    int cnode;              // 0x34
};

struct sUpMgr_8660 {
    char pad_0x0[0x8];
    sUpTex_8660* texs[0x7D1];   // 0x8
    int state[2];               // 0x1F4C
};

// PORT: 64-bit `ulong` GIF/DMA packet words.
extern "C" void func_00368660(char* self, int a, int b, int a3, ulong** pp, void** gifOut)
{
    ulong* cur = *pp;
    sUpTex_8660* objs[2];
    if (a < 0 && b < 0)
        return;
    objs[0] = a >= 0 ? ((sUpMgr_8660*)self)->texs[a] : 0;
    objs[1] = b >= 0 ? ((sUpMgr_8660*)self)->texs[b] : 0;
    int sent = 0;
    ulong* gif = 0;
    for (int k = 0; k < 2; k++) {
        int idx = b;
        if (k == 0)
            idx = a;
        if (idx < 0)
            continue;
        int other = a;
        if (k == 0)
            other = b;
        sUpTex_8660* t = ((sUpMgr_8660*)self)->texs[idx];
        sUpHeap_8660* heap;
        int shift;
        if (t->psm == 9) {
            heap = (sUpHeap_8660*)(self + 0x4350);
            shift = 2;
        } else {
            heap = (sUpHeap_8660*)(self + 0x1F60);
            shift = 0;
        }
        if (t->addr < 0) {
            ((sUpMgr_8660*)self)->state[k] = -1;
            int n = func_00366CE0(heap, t->size, idx, other, a3, 1);
            sUpNode_8660* nd = &heap->nodes[n];
            int fl = nd->flags;
            t->node = n;
            t->addr = (fl >> 8) << shift;
            if (gif == 0)
                gif = func_0038F460(D_004A4474, *(unsigned int*)gifOut, -1, 0);
            func_00368170(self, idx, &cur, &gif);
            sent = 1;
        }
        if (t->clut != 0 && t->caddr < 0) {
            *(int*)(self + 0x1F54) = -1;
            int n = func_00366CE0(self + 0x1F60, t->psm - 0xB < 2 ? 0x100 : 0x400, idx, other, a3, 0);
            int* f = &((sUpHeap_8660*)(self + 0x1F60))->nodes[n].flags;
            int fl = *f | 4;
            *f = fl;
            t->cnode = n;
            t->caddr = fl >> 8;
            if (gif == 0)
                gif = func_0038F460(D_004A4474, *(unsigned int*)gifOut, -1, 0);
            func_003684F0((sPs2UpMgr*)self, idx, &cur, &gif);
            sent = 1;
        }
    }
    if (gif != 0)
        *gifOut = func_0038F668(D_004A4474, gif, 4);
    if (!sent) {
        cur[0] = 0x10000001;
        cur[1] = 0;
        cur[2] = 0;
        cur[3] = (ulong)0x8800 << 45;
        cur += 4;
    }
    if (a >= 0)
        func_00368970(self, a, 0, &cur);
    if (b >= 0)
        func_00368970(self, b, 1, &cur);
    *pp = cur;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00368970);
#ifdef SKIP_ASM
static inline int imax_8970(int a, int b)
{
    return a < b ? b : a;
}

// VRAM bytes of a w x h image: rounded up to whole GS pages, then to 256 bytes
static inline int vramBytes_8970(int w, int h, int bpp)
{
    int pw, ph;
    switch (bpp) {
    case 4:
        pw = 0x80;
        ph = 0x80;
        break;
    case 8:
        pw = 0x80;
        ph = 0x40;
        break;
    case 15:
    case 16:
        pw = 0x40;
        ph = 0x40;
        break;
    case 32:
    default:
        pw = 0x40;
        ph = 0x20;
        break;
    }
    int n = imax_8970(w, pw) * imax_8970(h, ph) * bpp / 8;
    return n + (-n & 0xFF);
}

// TEX0.TW / TEX0.TH of a texture record
static inline int texTW_8970(char* t)
{
    return (*(ulong*)(t + 0x38) >> 26) & 0xF;
}

static inline int texTH_8970(char* t)
{
    return (*(ulong*)(t + 0x38) >> 30) & 0xF;
}

// VRAM blocks of mip level lv
static inline int levelBlocks_8970(char* t, int lv, int shift)
{
    return vramBytes_8970((1 << texTW_8970(t)) >> lv, (1 << texTH_8970(t)) >> lv, *(int*)(t + 0x18)) >> shift;
}

// self: the GS texture manager. +0x8 the texture records, +0x1F4C the texture bound to each context,
// +0x1F54 the texture whose CLUT is loaded.
// PORT: 64-bit `ulong` GIF/DMA packet words.
extern "C" void func_00368970(void* self, int idx, int slot, ulong** dma)
{
    char* selfBytes = (char*)self;
    ulong* packet = *dma;
    int off = slot << 2;
    char* bound = selfBytes + 0x1F4C;
    int* cur = (int*)(bound + off);
    if (*cur != idx) {
        *cur = idx;
        char* tex = *(char**)(selfBytes + (idx << 2) + 8);
        int rTex0, rTex1, rMip1, rMip2;
        if (slot == 0) {
            rTex0 = 6;
            rTex1 = 0x14;
            rMip1 = 0x34;
            rMip2 = 0x36;
        } else {
            rTex0 = 7;
            rTex1 = 0x15;
            rMip1 = 0x35;
            rMip2 = 0x37;
        }
        int shift = *(int*)(tex + 0xC) == 9 ? 6 : 8;
        int n;
        if (*(int*)(tex + 0x24) >= 4) {
            n = 4;
        } else {
            n = 2;
            if (*(int*)(tex + 0x24) >= 1)
                n = 3;
        }
        packet[0] = (n + 2) | 0x10000000;
        packet[1] = 0;
        packet[2] = 0;
        unsigned int direct = (n + 1) | 0x50000000;
        packet[3] = (ulong)direct << 32;
        packet[4] = (ulong)(n | 0x8000) | ((ulong)1 << 60);
        packet[5] = 0xE;
        packet += 6;
        if (*(int*)(tex + 0x24) > 0) {
            int tbp1 = *(int*)(tex + 0x28) + levelBlocks_8970(tex, 0, shift);
            int tbp2 = tbp1 + levelBlocks_8970(tex, 1, shift);
            int tbp3 = tbp2 + levelBlocks_8970(tex, 2, shift);
            packet[0] = *(ulong*)(tex + 0x48) | ((ulong)tbp1 | ((ulong)tbp2 << 20) | ((ulong)tbp3 << 40));
            packet[1] = rMip1;
            packet += 2;
            if (*(int*)(tex + 0x24) >= 4) {
                int tbp4 = tbp3 + levelBlocks_8970(tex, 3, shift);
                int tbp5 = tbp4 + levelBlocks_8970(tex, 4, shift);
                int tbp6 = tbp5 + levelBlocks_8970(tex, 5, shift);
                packet[0] = *(ulong*)(tex + 0x50) | ((ulong)tbp4 | ((ulong)tbp5 << 20) | ((ulong)tbp6 << 40));
                packet[1] = rMip2;
                packet += 2;
            }
        }
        packet[0] = *(ulong*)(tex + 0x40);
        packet[1] = rTex1;
        int caddr = *(int*)(tex + 0x2C);
        int cbp = 0;
        if (caddr >= 0) {
            *(int*)(selfBytes + 0x1F54) = idx;
            cbp = caddr;
        }
        packet[2] = *(ulong*)(tex + 0x38) | ((ulong)*(int*)(tex + 0x28) | ((ulong)cbp << 37));
        packet[3] = rTex0;
        packet += 4;
    } else if (*(int*)(selfBytes + 0x1F54) != idx) {
        char* tex = *(char**)(selfBytes + (idx << 2) + 8);
        if (*(int*)(tex + 0x2C) >= 0) {
            *(int*)(selfBytes + 0x1F54) = idx;
            packet[0] = 0x10000003;
            packet[1] = 0;
            packet[2] = 0;
            packet[3] = (ulong)0x50000002 << 32;
            packet[4] = (ulong)(1 | 0x8000) | ((ulong)1 << 60);
            packet[5] = 0xE;
            packet[6] = *(ulong*)(tex + 0x38) | ((ulong)*(int*)(tex + 0x2C) << 37);
            packet[7] = slot == 0 ? 0x16 : 0x17;
            packet += 8;
        }
    }
    *dma = packet;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00369098);
#ifdef SKIP_ASM
extern "C" void func_00366E30(void* list, int i);

extern "C" void func_00369098(void* self, int idx)
{
    char* tex = *(char**)((char*)self + (idx << 2) + 8);
    if (*(int*)(tex + 0x28) != -1) {
        if (*(int*)(tex + 0xC) != 9) {
            func_00366E30((char*)self + 0x1F60, idx);
        } else {
            func_00366E30((char*)self + 0x4350, idx);
        }
    }
    int unbound = -1;
    int* bound = (int*)((char*)self + 0x1F4C);
    for (int i = 1; i >= 0; i--) {
        bound[i] = unbound;
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

extern "C" void func_00369130(sGfxTexTable1* self, int idx, int mmin, int mmag, int lodL, int k)
{
    sGfxTex1* entry = self->entries[idx];
    sGsTex1 tex1Reg = entry->tex1;
    tex1Reg.L = lodL;
    tex1Reg.K = k;
    tex1Reg.MMIN = mmin;
    tex1Reg.MMAG = mmag;
    entry->tex1 = tex1Reg;
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

//100%
INCLUDE_ASM("render/ps2graphicsman", cGraphicsMan_cGraphicsMan);
#ifdef SKIP_ASM
extern "C" void func_00416210(void* dst, int c, int n);
extern "C" void MEM_fill(void* dst, unsigned int value, int size);
extern void* cGraphicsMan_vtbl[];

inline void* operator new(unsigned int, void* p) { return p; }

struct sTexPair_691F8 {
    short s[2];
    sTexPair_691F8() { for (unsigned int i = 0; i < 2; i++) s[i] = -1; }
};

struct sRS_691F8 {
    int f0;                     // 0x00
    int flagsA;                 // 0x04
    int flagsB;                 // 0x08
    int fC;                     // 0x0C
    sTexPair_691F8 tex;         // 0x10
    sRS_691F8() { func_00416210(this, 0, 0x10); }
};

struct sGfxPair_691F8 {
    int a, b;
    sGfxPair_691F8() {}
};

struct sGfxState_691F8 {
    sRS_691F8* top;                 // 0x00 (0xE84)
    sRS_691F8 stack[4];             // 0x04 (0xE88)
    sRS_691F8 cur;                  // 0x54 (0xED8)
    int pad68;                      // 0x68 (0xEEC)
    sTexPair_691F8 texs[4];         // 0x6C (0xEF0)
    sTexPair_691F8 tex;             // 0x7C (0xF00)
    sGfxPair_691F8 p0[2];           // 0x80 (0xF04)
    sGfxPair_691F8 p1[2];           // 0x90 (0xF14)
    sGfxState_691F8() {}
};

struct sKey8_691F8 {
    int a, b;
};

extern sRS_691F8 sDrawState_s_defaultDrawState;
extern sTexPair_691F8 D_004A5A10[];
extern sKey8_691F8 D_004A5960[];

static inline void gfxSet0_691F8(sRS_691F8* r, int v, int mask, int shift)
{
    r->f0 = (r->f0 & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

static inline void gfxSetA_691F8(sRS_691F8* r, int v, int mask, int shift)
{
    r->flagsA = (r->flagsA & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

static inline void gfxSetB_691F8(sRS_691F8* r, int v, int mask, int shift)
{
    r->flagsB = (r->flagsB & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

static inline void gfxSetC_691F8(sRS_691F8* r, int v, int mask, int shift)
{
    r->fC = (r->fC & ~(mask << shift)) | ((v << shift) & (mask << shift));
}

static inline void gfxSetTex_691F8(sRS_691F8* r, const sTexPair_691F8& t)
{
    r->tex = t;
}

/**
 * Constructs the base graphics manager and publishes it in cGraphicsMan_TheGraphicsMan.
 *
 * Stores cGraphicsMan_vtbl at +0x10D8, builds the default draw state sDrawState_s_defaultDrawState (depth, alpha, blend and texture
 * fields; texture ids -1), copies it into all 4 draw-state stack entries (+0xE88) and makes the first current, fills
 * the 86-entry effect texture map (+0xF50) with -1 and sets mNumViews (+0x270) to 1.
 *
 * @return @p self
 */
extern "C" void* cGraphicsMan_cGraphicsMan(char* self)
{
    *(void***)(self + 0x10D8) = cGraphicsMan_vtbl;
    new (self + 0xE84) sGfxState_691F8;
    *(int*)(self + 0xF44) = 0;
    cGraphicsMan_TheGraphicsMan = (cPS2GraphicsMan*)(self);
    for (unsigned int i = 0; i < 2; i++) D_004A5A10[0].s[i] = -1;
    func_00416210(&sDrawState_s_defaultDrawState, 0, 0x10);
    gfxSetA_691F8(&sDrawState_s_defaultDrawState, 1, 3, 23);
    gfxSetA_691F8(&sDrawState_s_defaultDrawState, 0, 1, 22);
    gfxSetA_691F8(&sDrawState_s_defaultDrawState, 3, 3, 20);
    gfxSetA_691F8(&sDrawState_s_defaultDrawState, 0, 0xFF, 12);
    gfxSetA_691F8(&sDrawState_s_defaultDrawState, 0, 3, 0);
    gfxSetB_691F8(&sDrawState_s_defaultDrawState, 4, 0x1F, 5);
    gfxSetB_691F8(&sDrawState_s_defaultDrawState, 0, 0x1F, 0);
    gfxSet0_691F8(&sDrawState_s_defaultDrawState, 0, 3, 0);
    gfxSet0_691F8(&sDrawState_s_defaultDrawState, 0, 3, 10);
    gfxSet0_691F8(&sDrawState_s_defaultDrawState, 0, 0xF, 6);
    gfxSetC_691F8(&sDrawState_s_defaultDrawState, 0, 0x3FFFFFFF, 0);
    for (unsigned int k = 0; k < 2; k++) {
        gfxSetA_691F8(&sDrawState_s_defaultDrawState, 5, 0x1F, 2 + k * 5);
        gfxSet0_691F8(&sDrawState_s_defaultDrawState, 0, 0xFF, 12 + k * 8);
    }
    for (unsigned int k = 0; k < 2; k++) {
        gfxSet0_691F8(&sDrawState_s_defaultDrawState, 0, 3, 2 + k * 2);
    }
    gfxSetTex_691F8(&sDrawState_s_defaultDrawState, D_004A5A10[0]);
    for (int k = 0; k < 4; k++) {
        ((sRS_691F8*)(self + 0xE88))[k] = sDrawState_s_defaultDrawState;
    }
    *(sRS_691F8**)(self + 0xE84) = (sRS_691F8*)(self + 0xE88);
    *(sKey8_691F8*)self = D_004A5960[0];
    *(int*)(self + 0x8) = 0;
    MEM_fill(self + 0xF50, 0xFFFFFFFF, 0x158);
    *(int*)(self + 0x270) = 1;
    *(int*)(self + 0x26C) = 0;
    *(int*)(self + 0x274) = 0;
    *(int*)(self + 0xF48) = 0;
    *(int*)(self + 0xF4C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cGraphicsMan__cGraphicsMan);
#ifdef SKIP_ASM
extern void* cGraphicsMan_vtbl[];
void operator_delete(int* ptr);

/**
 * Destroys the base graphics manager: stores cGraphicsMan_vtbl, clears cGraphicsMan_TheGraphicsMan and frees the
 * object when @p flags bit 0 is set.
 *
 * @param flags  destructor flags; bit 0 frees the object
 */
extern "C" void cGraphicsMan__cGraphicsMan(cPS2GraphicsMan* self, int flags)
{
    self->mVtbl = cGraphicsMan_vtbl;
    cGraphicsMan_TheGraphicsMan = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cGraphicsMan_ForceActiveDrawState);
#ifdef SKIP_ASM
struct sVEntry00369610 {
    short delta;
    short index;
    void (*fn)(void*, int*);
};

/**
 * Forces the whole draw state to be resent: the cached active state (+0xED8, 4 words) becomes ~mask so every field
 * differs, the cached texture ids become 0x7FFF, then virtual slot 62 applies @p mask.
 *
 * @param mask  the draw state to apply (4 words)
 */
extern "C" void cGraphicsMan_ForceActiveDrawState(cPS2GraphicsMan* self, int* mask)
{
    int i;
    unsigned int j;
    for (i = 0; i < 4; i++) {
        ((int*)((char*)self + 0xED8))[i] = ~mask[i];
    }
    for (j = 0; j < 2; j++) {
        ((short*)((char*)self + 0xEE8))[j] = 0x7FFF;
    }
    sVEntry00369610* vt = (sVEntry00369610*)self->mVtbl;
    vt[62].fn((char*)self + vt[62].delta, mask);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00369690);
#ifdef SKIP_ASM
extern "C" void* MEM_copy(void*, void*, int);

extern "C" void func_00369690(void* self, int count, void* src)
{
    *(int*)((char*)self + 0xE80) = count;
    MEM_copy((char*)self + 0x280, src, count * 0xC);
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", cGraphicsMan_GetTextureID);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);
extern "C" void* SHAPE_locatez(void* src, const char* name);
extern "C" char* strcpy(char* dst, const char* src);
extern char str_charshp_b[];

struct sTexEnt_96C8 {
    int handle;     // 0x0
    char name[8];   // 0x4
};

struct sVE_96C8 {
    short delta;
    short index;
    int (*fn)(void*, void*, const char*, int, int, int);
};

struct sGfx_96C8 {
    char pad0[0x278];
    int numSrc;                 // 0x278
    void** src;                 // 0x27C
    sTexEnt_96C8 ents[256];     // 0x280
    int count;                  // 0xE80
};

/**
 * Returns the texture id for a short texture name, loading and caching the texture on first use.
 *
 * Uses up to the first 4 characters of @p name. Searches the texture-ref cache (On Tour mTexRefs: 256 entries of {id,
 * name[8]} at +0x280, count +0xE80) with func_004165A8 (strcmp). On a miss it copies the name into the next entry and
 * asks each loaded texture file (+0x27C, count +0x278) for the shape (SHAPE_locatez); the first hit is added through
 * vtable slot 46 (func_0037C8C0) as a "charshp" texture. A name no file has is cached with id -1. There is no check
 * against the 256-entry limit (On Tour asserts mNumTexRefs < arraysize(mTexRefs)): a 257th distinct name would
 * overwrite the count.
 *
 * @param self_  the graphics manager (this)
 * @param name   the texture name
 * @return the texture id, or -1 when no texture file has the shape
 */
// Look up (or load and cache) a texture by its 4-character name.
extern "C" int cGraphicsMan_GetTextureID(cPS2GraphicsMan* self_, const char* name)
{
    sGfx_96C8* self = (sGfx_96C8*)self_;
    char buf[0x10];
    const char* p = name;
    if (*p) {
        char* d = buf;
        do
            *d++ = *p++;
        while (*p && p - name < 4);
    }
    buf[p - name] = 0;
    for (int i = 0; i < self->count; i++) {
        if (func_004165A8(buf, self->ents[i].name) == 0)
            return self->ents[i].handle;
    }
    strcpy(self->ents[self->count].name, buf);
    for (int j = 0; j < self->numSrc; j++) {
        void* r = SHAPE_locatez(self->src[j], self->ents[self->count].name);
        if (r != 0) {
            sVE_96C8* vt = *(sVE_96C8**)((char*)self + 0x10D8);
            self->ents[self->count].handle = vt[46].fn((char*)self + vt[46].delta, r, str_charshp_b, 0, 1, -1);
            return self->ents[self->count++].handle;
        }
    }
    self->ents[self->count].handle = -1;
    return self->ents[self->count++].handle;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00369890__FPvii);
#ifdef SKIP_ASM
void func_00369890(void* self, int numFiles, int fileNames)
{
    *(int*)((char*)self + 0x27c) = fileNames;
    *(int*)((char*)self + 0x278) = numFiles;
    *(int*)((char*)self + 0xe80) = 0;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003698A0);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

extern "C" void func_003698A0(void* self)
{
    void* history = *(void**)((char*)self + 0xF48);
    if (history != 0) {
        cMemMan_free(history);
    }
    *(void**)((char*)self + 0xF48) = 0;
    *(int*)((char*)self + 0xF4C) = 0;
}
#endif

