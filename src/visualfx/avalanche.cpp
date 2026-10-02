#include "common.h"

INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_getFrameData);

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D6378);
#ifdef SKIP_ASM
extern void* D_00488648[];
extern void* D_00488680[];
struct func_002D6378_sElem {
    char pad[0x2F0];
};
extern func_002D6378_sElem D_004EE840[];
extern "C" void func_002D9B40(void* self);
extern "C" void func_003715B0(void* p, int flags);
void operator_delete(int* ptr);

extern "C" void func_002D6378(int* self, int flags)
{
    *(void***)self = D_00488648;
    func_002D9B40(self);
    int i;
    for (i = 0; i < 64; i++) {
        func_003715B0(&D_004EE840[i], 1);
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

INCLUDE_ASM("visualfx/avalanche", func_002D64D8);

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

INCLUDE_ASM("visualfx/avalanche", func_002D7CA8);

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D7DD8);
#ifdef SKIP_ASM
extern "C" void cDynamicColourEmitter_reset(void* self);

struct sVEntry2D7DD8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_002D7DD8(void* self, int force)
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

INCLUDE_ASM("visualfx/avalanche", func_002D7EF8);

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
            func_002D7DD8(n, 1);
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
INCLUDE_ASM("visualfx/avalanche", func_002D8258);
#ifdef SKIP_ASM
extern char D_004A3AD0[];
extern char D_004A3AD8[];
extern char D_004A3AE0[];
extern "C" void func_002D83B8(int, char*);

extern "C" void func_002D8258(void)
{
    func_002D83B8(1, D_004A3AD0);
    func_002D83B8(1, D_004A3AD8);
    func_002D83B8(0, D_004A3AE0);
}
#endif

INCLUDE_ASM("visualfx/avalanche", func_002D82A0);

INCLUDE_ASM("visualfx/avalanche", func_002D83B8);

INCLUDE_ASM("visualfx/avalanche", func_002D87D0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", func_002D8948);
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
extern "C" void func_002D96E0(void);
extern "C" void cAvalanche_resolveDataPointers(void);
extern "C" int cAvalanche_triggerAvalanche(int a);
extern "C" void func_002D7EF8(void* self);

extern "C" void func_002D8948(void)
{
    if (D_004A3A30 != 0) {
        if (D_004A3A34 != 0) {
            func_002D96E0();
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

INCLUDE_ASM("visualfx/avalanche", func_002D8A00);

INCLUDE_ASM("visualfx/avalanche", func_002D8EA8);

INCLUDE_ASM("visualfx/avalanche", func_002D9130);

INCLUDE_ASM("visualfx/avalanche", cAvalanche_addAvalancheNode);

INCLUDE_ASM("visualfx/avalanche", func_002D9538);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", func_002D9660);
#ifdef SKIP_ASM
extern "C" void tAvalancheNode_calculate(void* node, void* self);

extern "C" void func_002D9660(void* self)
{
    void* n = *(void**)((char*)self + 0x4);
    while (n != 0) {
        tAvalancheNode_calculate(n, self);
        n = *(void**)((char*)n + 0xF4);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/avalanche", func_002D96E0);
#ifdef SKIP_ASM
extern "C" void func_002D9A80(void);
extern "C" void func_002D8258(void);
extern "C" void func_002D87D0(void);
extern void* D_004A3AB0;

extern "C" void func_002D96E0(void)
{
    func_002D9A80();
    for (void* p = D_004A3AB0; p != 0; p = *(void**)((char*)p + 0x8)) {
        func_002D9660(p);
    }
    func_002D8258();
    func_002D87D0();
}
#endif

INCLUDE_ASM("visualfx/avalanche", func_002D9738);

INCLUDE_ASM("visualfx/avalanche", cAvalanche_triggerAvalanche);

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D9A80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct func_002D9A80_sSlot {
    int active;
    char pad[0x18];
};
// Typed view of D_00538938 (func_002D8948 declares it earlier in the unit with its own element type).
extern func_002D9A80_sSlot D_slots_9A80[] __asm__("D_00538938");
extern int D_004A3AB8;
void cMemMan_free(void*);
extern "C" void func_002D81B0(void* self);

static inline void freeSlot_9A80(func_002D9A80_sSlot* s)
{
    if (s->active != 0) {
        func_002D81B0(s);
    }
}

extern "C" void func_002D9A80(void)
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
INCLUDE_ASM("visualfx/avalanche", func_002D9B40);
#ifdef SKIP_ASM
extern "C" void func_002D9A80(void);
extern "C" void func_00416210(void* dst, int c, int n);
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern char D_00538450[];
extern int D_004A3AC0;
extern void* D_004A3AB4;
extern int D_004A3AB8;
extern int D_004A3ABC;

extern "C" void func_002D9B40(void* self)
{
    func_002D9A80();
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

INCLUDE_ASM("visualfx/avalanche", func_002D9CB0);

INCLUDE_ASM("visualfx/avalanche", cAvalanche_readFromReplayFrame);

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
    func_002D9B40(data);
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

INCLUDE_ASM("visualfx/avalanche", cAvalanche_resolveDataPointers);

INCLUDE_ASM("visualfx/avalanche", func_002DA1C0);

