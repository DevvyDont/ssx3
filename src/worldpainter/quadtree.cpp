#include "common.h"

INCLUDE_ASM("worldpainter/quadtree", cQuadTree_getFreeNode);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C1CD8);
#ifdef SKIP_ASM
extern "C" void* func_002C1CD8(void* self, float fx, float fz)
{
    int x = (int)((fx - *(float*)((char*)self + 4)) * *(float*)self);
    int z = (int)((fz - *(float*)((char*)self + 8)) * *(float*)self);
    if ((unsigned int)x > 0x7FFF || (unsigned int)z > 0x7FFF) {
        return (char*)self + 0x18;
    }
    unsigned short ux = x << 1;
    unsigned short uz = z << 1;
    char* nodes = *(char**)((char*)self + 0x20);
    unsigned short* node = (unsigned short*)(nodes + *(unsigned short*)((char*)self + 0x14) * 8);
    while (node[0] & 1) {
        int idx = ((ux >> 15) << 1) | (uz >> 15);
        ux <<= 1;
        uz <<= 1;
        node = (unsigned short*)(nodes + (*(unsigned short*)((char*)node + (idx << 1)) >> 1) * 8);
    }
    if (node[0] & 1) {
        return 0;
    }
    return node;
}
#endif

INCLUDE_ASM("worldpainter/quadtree", cQuadTree_exportTree);

INCLUDE_ASM("worldpainter/quadtree", func_002C2088);

INCLUDE_ASM("worldpainter/quadtree", func_002C2210);

INCLUDE_ASM("worldpainter/quadtree", func_002C2268);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C22C8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002C3FA8(void*);
extern const char D_00485FF0[];

extern "C" void* func_002C22C8(void)
{
    return func_002C3FA8(cMemMan_alloc(0x1500, D_00485FF0, 0x100, 0));
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2300);
#ifdef SKIP_ASM
class func_002C2300_cObj {
public:
    virtual ~func_002C2300_cObj();
};

extern "C" void func_002C2300(func_002C2300_cObj* self)
{
    delete self;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2338);
#ifdef SKIP_ASM
struct func_002C2338_sRange {
    unsigned short base;
    unsigned short first;
};
extern func_002C2338_sRange D_00445940[];
extern unsigned short D_00445950[];

extern "C" short func_002C2338(unsigned char c)
{
    int off = 0;
    int idx = 0;
    unsigned short v;

    if (c >= 0x20 && c <= 0x2F) {
        off = 1;
    } else if (c >= 0x30 && c <= 0x39) {
    } else if (c >= 0x3A && c <= 0x40) {
        off = 0xB;
    } else if (c >= 0x41 && c <= 0x5A) {
        idx = 1;
    } else if (c >= 0x5B && c <= 0x60) {
        off = 0x25;
    } else if (c >= 0x61 && c <= 0x7A) {
        idx = 2;
    } else if (c >= 0x7B && c <= 0x7E) {
        off = 0x3F;
    } else {
        return 0;
    }
    if (off != 0) {
        v = D_00445950[(c - 0x20) + 1 - off];
    } else {
        v = D_00445940[idx].base + c - D_00445940[idx].first;
    }
    return (v << 8) | (v >> 8);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C2430);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C24D0);
#ifdef SKIP_ASM
extern "C" int func_002C24D0(unsigned short* s)
{
    int n = 0;
    while (*s++ != 0) {
        n++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2508);
#ifdef SKIP_ASM
extern "C" unsigned short* func_002C2508(unsigned short* dst, unsigned short* src)
{
    unsigned short* ret = dst;
    while (*src != 0) {
        *dst++ = *src++;
    }
    *dst = 0;
    return ret;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2540);
#ifdef SKIP_ASM
extern "C" unsigned short* func_002C2540(unsigned short* dst, char* src)
{
    unsigned short* ret = dst;
    while (*src != 0) {
        *dst++ = *src++;
    }
    *dst = 0;
    return ret;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2580);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit declares this as extern "C" void* func_002C2580(void*) further down
// (its caller func_002C5500 passes one arg); bind the 2-arg body to the symbol.
char* func_002C2580_impl(char* dst, unsigned short* src) __asm__("func_002C2580");
char* func_002C2580_impl(char* dst, unsigned short* src)
{
    char* ret = dst;
    while (*src != 0) {
        *dst++ = *src++;
    }
    *dst = 0;
    return ret;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C25B8);
#ifdef SKIP_ASM
extern "C" unsigned short* func_002C25B8(unsigned short* dst, unsigned short* src, int n)
{
    unsigned short* ret = dst;
    while (*src != 0 && n > 0) {
        *dst++ = *src++;
        n--;
    }
    *dst = 0;
    return ret;
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C2688);

INCLUDE_ASM("worldpainter/quadtree", func_002C26D0);

INCLUDE_ASM("worldpainter/quadtree", func_002C2718);

INCLUDE_ASM("worldpainter/quadtree", func_002C27C0);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C2868);
#ifdef SKIP_ASM
extern "C" void func_002C2868(char* s)
{
    while (*s != 0) {
        if (*s >= 'A' && *s <= 'Z') {
            *s += 'a' - 'A';
        }
        s++;
    }
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C2F18);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C3FA0__FPv);
#ifdef SKIP_ASM
void func_002C3FA0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C3FA8);
#ifdef SKIP_ASM
extern "C" void func_002C4050(void*);
extern void* D_00486F78[];

extern "C" void* func_002C3FA8(void* self)
{
    *(void***)self = D_00486F78;
    func_002C4050(self);
    return self;
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C3FE0);

INCLUDE_ASM("worldpainter/quadtree", func_002C4050);

INCLUDE_ASM("worldpainter/quadtree", func_002C4198);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C41D8);
#ifdef SKIP_ASM
extern "C" void* func_002C48C0(void*, int);

extern "C" void func_002C41D8(void* self, const char* name)
{
    strcpy((char*)self + 0x139, name);
    func_002C48C0(self, 5);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C4210);

extern "C" void* func_002C48C0(void*, int);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4290__FPvi);
#ifdef SKIP_ASM
void* func_002C4290(void* self, int a1)
{
    *(int*)((char*)self + 0x1c) = a1;
    return func_002C48C0(self, 0x22);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C42B0);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4368__FPv);
#ifdef SKIP_ASM
void* func_002C4368(void* self)
{
    return func_002C48C0(self, 0x11);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C4388);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C43E0__FPvi);
#ifdef SKIP_ASM
void* func_002C43E0(void* self, int a1)
{
    *(int*)((char*)self + 0x1c) = a1;
    return func_002C48C0(self, 0x13);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4400__FPv);
#ifdef SKIP_ASM
int func_002C4400(void* self)
{
    return *(int*)((char*)self + 0x490);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4408__FPvi);
#ifdef SKIP_ASM
void func_002C4408(void* self, int val)
{
    *(int*)((char*)self + 0x24) = val;
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C4410);

INCLUDE_ASM("worldpainter/quadtree", func_002C4480);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C44E0);
#ifdef SKIP_ASM
struct sQTLevel {
    int f00;
    void* f04;
    int f08;
    int f0C;
    int f10;
};

struct sQTState {
    char pad000[0xC];
    int level;
    char pad010[0x16C];
    sQTLevel levels[1];
};

extern "C" void func_002C44E0(sQTState* self)
{
    self->levels[self->level].f10 = 0;
    if (self->levels[self->level].f08 == -1) {
        self->levels[self->level].f08 = 0;
    }
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C4520);

INCLUDE_ASM("worldpainter/quadtree", func_002C4578);

INCLUDE_ASM("worldpainter/quadtree", func_002C45D0);

INCLUDE_ASM("worldpainter/quadtree", func_002C4648);

INCLUDE_ASM("worldpainter/quadtree", func_002C46C0);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4720__FPvii);
#ifdef SKIP_ASM
void* func_002C4720(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0x2c) = a1;
    *(int*)((char*)self + 0x20) = a2;
    return func_002C48C0(self, 0x17);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4748);
#ifdef SKIP_ASM
extern "C" int func_002C4748(void* self)
{
    return *(int*)((char*)self + 0x4) == 0x34;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4758__FPv);
#ifdef SKIP_ASM
void func_002C4758(void* self)
{
    *(int*)((char*)self + 0x48) = 1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4768__FPv);
#ifdef SKIP_ASM
void* func_002C4768(void* self)
{
    return func_002C48C0(self, 0x18);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C4788);

INCLUDE_ASM("worldpainter/quadtree", func_002C4800);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4858__FPvii);
#ifdef SKIP_ASM
void* func_002C4858(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0x2c) = a1;
    *(int*)((char*)self + 0x20) = a2;
    return func_002C48C0(self, 0x1d);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4880);
#ifdef SKIP_ASM
extern "C" int func_002C4880(void* self)
{
    return *(int*)((char*)self + 0x4) == 0x35;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C4890__FPv);
#ifdef SKIP_ASM
void func_002C4890(void* self)
{
    *(int*)((char*)self + 0x44) = 1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C48A0__FPv);
#ifdef SKIP_ASM
void* func_002C48A0(void* self)
{
    return func_002C48C0(self, 0x1e);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C48C0);

INCLUDE_ASM("worldpainter/quadtree", func_002C50E0);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5140);
#ifdef SKIP_ASM
extern "C" int func_002C5140(void* self)
{
    if (*(int*)((char*)self + 0x4) != 0 || *(int*)((char*)self + 0x40) != 0) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5168);
#ifdef SKIP_ASM
extern "C" int func_002C5168(sQTState* self)
{
    switch (self->levels[self->level].f08) {
    case -1:
    case -3:
    case -8:
    case -9:
    case -10:
    case -10001:
    case -10002:
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C51D0);
#ifdef SKIP_ASM
// PORT: the unit declares this as `extern "C" void* func_002C51D0(void*, int)`
// for its callers; the body returns an int code, bound with an asm label.
extern "C" int func_002C51D0_impl(sQTState* self, int i) __asm__("func_002C51D0");

extern "C" int func_002C51D0_impl(sQTState* self, int i)
{
    switch (self->levels[i].f08) {
    case -10002:
    case -10001:
    case -7:
    case -6:
    case -5:
    case -4:
    case -3:
    case -2:
    case -1:
    case 0:
        return self->levels[i].f08;
    case -10:
    case -9:
    case -8:
    default:
        return -10;
    }
}
#endif

extern "C" void* func_002C51D0(void*, int);

//99.38%
INCLUDE_ASM("worldpainter/quadtree", func_002C5230__FPv);
#ifdef SKIP_ASM
int func_002C5230(void* self)
{
    return (func_002C51D0(self, *(int*)((char*)self + 0xc)) != 0);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5250);
#ifdef SKIP_ASM
extern "C" void func_002C5250(sQTState* self)
{
    self->levels[self->level].f00 = 0;
    self->levels[self->level].f04 = 0;
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C5278);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C52D8);
#ifdef SKIP_ASM
extern "C" void* func_003E6448(void* dst, int c, int n);

extern "C" void func_002C52D8(void* self)
{
    func_003E6448((char*)self + 0x480, 0, 0x40);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5300);
#ifdef SKIP_ASM
extern "C" int func_002C5300(void* self, int a1)
{
    char* p = (char*)self + a1 * 0x14;
    return *(int*)(p + 0x184) == -0x2712;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5320);
#ifdef SKIP_ASM
extern "C" void* func_002C5320(void* self, int a1)
{
    return *(void**)((char*)self + a1 * 0x14 + 0x180);
}
#endif

//99.29%
INCLUDE_ASM("worldpainter/quadtree", func_002C5338__FPv);
#ifdef SKIP_ASM
void* func_002C5338(void* self)
{
    return func_002C5320(self, *(int*)((char*)self + 0xc));
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5358);
#ifdef SKIP_ASM
extern "C" int func_002C5358(sQTState* self, int i)
{
    switch (self->levels[i].f08) {
    case -10001:
    case -7:
    case -5:
    case -4:
    case -3:
    case -2:
    case -1:
    case 0:
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C53B0);
#ifdef SKIP_ASM
extern "C" int func_002C53B0(void* self, int a1)
{
    char* p = (char*)self + a1 * 0x14;
    return *(int*)(p + 0x188) != 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C53C8);
#ifdef SKIP_ASM
extern "C" int func_002C53C8(void* self, int a1)
{
    char* p = (char*)self + a1 * 0x14;
    return *(int*)(p + 0x18c) != 0;
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C53E0);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C54D0);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);

extern "C" void func_002C54D0(void* self, int a1, char* dst)
{
    char* p = (char*)self + a1 * 0x78;
    strcpy(dst, p + 0x1b0);
}
#endif

extern "C" void* func_002C2580(void*);

//99.29%
INCLUDE_ASM("worldpainter/quadtree", func_002C5500__FPv);
#ifdef SKIP_ASM
void* func_002C5500(void* self)
{
    return func_002C2580((char*)self + 0x58);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5520__FPv);
#ifdef SKIP_ASM
void func_002C5520(void* self)
{
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5528);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);

extern "C" bool func_002C5528(void* self, int a1)
{
    char* p = (char*)self + a1 * 0x78;
    return strlen(p + 0x1b0) != 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5558);
#ifdef SKIP_ASM
extern "C" void func_002C5558(void* self, int a1)
{
    char* p = (char*)self + a1 * 0x78;
    *(int*)(p + 0x1a4) = 1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5570);
#ifdef SKIP_ASM
extern "C" void func_002C5570(void* self, int a1)
{
    char* p = (char*)self + a1 * 0x78;
    *(int*)(p + 0x1a8) = 1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C5588);
#ifdef SKIP_ASM
struct sQTSlot {
    int a;
    int b;
    char pad08[0x70];
};

struct sQTSlots {
    char pad000[0x1A4];
    sQTSlot slots[1];
};

extern "C" int func_002C5588(sQTSlots* self, int i)
{
    if (self->slots[i].b != 0 || self->slots[i].a != 0) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C55C0);
#ifdef SKIP_ASM
extern "C" int func_002C55C0(void* self, int a1)
{
    char* p = (char*)self + a1 * 0x78;
    return *(int*)(p + 0x1ac) != 0;
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C55D8);

INCLUDE_ASM("worldpainter/quadtree", func_002C6128);

INCLUDE_ASM("worldpainter/quadtree", func_002C6280);

INCLUDE_ASM("worldpainter/quadtree", func_002C6360);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C63E8);
#ifdef SKIP_ASM
extern "C" int func_002C63E8(void* self, unsigned int a1)
{
    return a1 + 0x270f < 0x2706;
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C63F8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", func_002C64A8);
#ifdef SKIP_ASM
extern char D_004A3968[];

extern "C" void func_002C64A8(void* self, int a1, unsigned short* dst)
{
    func_002C2540(dst, D_004A3968);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/quadtree", func_002C64D0);
#ifdef SKIP_ASM
extern char D_004A3968[];

extern "C" void func_002C64D0(void* self, int a1, unsigned short* dst)
{
    func_002C2540(dst, D_004A3968);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C64F8);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C66B8);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);

extern "C" void func_002C66B8(void* self, const char* name, int flag)
{
    if (flag == 0) {
        strcpy((char*)self + 0x14C0, name);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C66D8);
#ifdef SKIP_ASM
extern "C" int func_002C66D8(void* self)
{
    char* p = (char*)self + *(int*)((char*)self + 0xc) * 0x14;
    return *(int*)(p + 0x184) == -0xb;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6700__FPv);
#ifdef SKIP_ASM
int func_002C6700(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6708__FPv);
#ifdef SKIP_ASM
int func_002C6708(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6710__FPv);
#ifdef SKIP_ASM
int func_002C6710(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6718);
#ifdef SKIP_ASM
#define QT_V10(p) virtual void p##0(); virtual void p##1(); virtual void p##2(); virtual void p##3(); virtual void p##4(); \
    virtual void p##5(); virtual void p##6(); virtual void p##7(); virtual void p##8(); virtual void p##9();

class cQuadTreeVObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8 (delta at +0, function at +4)
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    QT_V10(v1)
    virtual void v20(); virtual void v21();
    virtual int v22(int a); // 0xB0
    virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29();
    QT_V10(v3) QT_V10(v4)
    virtual int v50(int a); // 0x190
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual int v61(int a); // 0x1E8
};

extern "C" void func_002C6718(cQuadTreeVObj* self)
{
    self->v61(*(int*)((char*)self + 0xC));
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6748__FPv);
#ifdef SKIP_ASM
int func_002C6748(void* self)
{
    return 0x4000;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6750__FPv);
#ifdef SKIP_ASM
int func_002C6750(void* self)
{
    return -0x1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6758);
#ifdef SKIP_ASM
extern "C" int func_002C6758(void* self, int a1)
{
    return (a1 + 0x3ff) / 0x400 + 0x4d;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6778);
#ifdef SKIP_ASM
#define QT_V10(p) virtual void p##0(); virtual void p##1(); virtual void p##2(); virtual void p##3(); virtual void p##4(); \
    virtual void p##5(); virtual void p##6(); virtual void p##7(); virtual void p##8(); virtual void p##9();

extern "C" void func_002C6778(cQuadTreeVObj* self)
{
    self->v22(*(int*)((char*)self + 0x24));
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C67A8__FPv);
#ifdef SKIP_ASM
int func_002C67A8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C67B0);
#ifdef SKIP_ASM
#define QT_V10(p) virtual void p##0(); virtual void p##1(); virtual void p##2(); virtual void p##3(); virtual void p##4(); \
    virtual void p##5(); virtual void p##6(); virtual void p##7(); virtual void p##8(); virtual void p##9();

extern "C" void func_002C67B0(cQuadTreeVObj* self)
{
    self->v50(*(int*)((char*)self + 0xC));
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C67E0__FPv);
#ifdef SKIP_ASM
int func_002C67E0(void* self)
{
    return -0x1;
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C67E8__FPvi);
#ifdef SKIP_ASM
void* func_002C67E8(void* self, int a1)
{
    *(int*)((char*)self + 0xc) = a1;
    *(int*)((char*)self + 0x10) = a1;
    return func_002C48C0(self, 3);
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6810);
#ifdef SKIP_ASM
extern "C" void func_002C6810(void* self, const char* name)
{
    *(int*)((char*)self + 0x54) = 0;
    strcpy((char*)self + 0x139, name);
    func_002C48C0(self, 0x3C);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C6848);

INCLUDE_ASM("worldpainter/quadtree", func_002C68C8);

INCLUDE_ASM("worldpainter/quadtree", func_002C68F0);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6968);
#ifdef SKIP_ASM
extern "C" void func_002C6968(void* self, const char* name, int a2)
{
    *(int*)((char*)self + 0x14E0) = a2;
    strcpy((char*)self + 0xF9, name);
    func_002C48C0(self, 0x3F);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C69A0);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6A08__FPv);
#ifdef SKIP_ASM
void* func_002C6A08(void* self)
{
    return func_002C48C0(self, 0x49);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C6A28);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6A60__FPvii);
#ifdef SKIP_ASM
void* func_002C6A60(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0x14e4) = a1;
    *(int*)((char*)self + 0x14e8) = a2;
    return func_002C48C0(self, 0x41);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C6A88);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6AE0__FPv);
#ifdef SKIP_ASM
void* func_002C6AE0(void* self)
{
    return func_002C48C0(self, 0x47);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C6B00);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6B50__FPvii);
#ifdef SKIP_ASM
void* func_002C6B50(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0x2c) = a1;
    *(int*)((char*)self + 0x24) = a2;
    return func_002C48C0(self, 0x43);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C6B78);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6BD0__FPvii);
#ifdef SKIP_ASM
void* func_002C6BD0(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0x2c) = a1;
    *(int*)((char*)self + 0x24) = a2;
    return func_002C48C0(self, 0x45);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C6BF8);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6C50);
#ifdef SKIP_ASM
extern "C" void func_002C6C50(void* self, const char* name, int a2)
{
    *(int*)((char*)self + 0x20) = a2;
    strcpy((char*)self + 0xF9, name);
    func_002C48C0(self, 0x4B);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C6C88);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6CE0);
#ifdef SKIP_ASM
extern "C" void func_002C6CE0(void* self, const char* name)
{
    strcpy((char*)self + 0xF9, name);
    func_002C48C0(self, 0x4D);
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C6D18);

INCLUDE_ASM("worldpainter/quadtree", func_002C6D70);

extern "C" void* func_003DBB68(int);

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6DC0__FPv);
#ifdef SKIP_ASM
void* func_002C6DC0(void* self)
{
    return func_003DBB68(*(int*)((char*)self + 0x4));
}
#endif

//100%
INCLUDE_ASM("worldpainter/quadtree", func_002C6DE0);
#ifdef SKIP_ASM
struct func_002C6DE0_sEntry {
    unsigned int key;
    int value;
};

struct func_002C6DE0_sMap {
    int pad0;
    int pad4;
    int last;
    int count;
    func_002C6DE0_sEntry* entries;
};

extern "C" int func_002C6DE0(func_002C6DE0_sMap* self, unsigned int key)
{
    int lo = 0;
    int hi = self->last;
    if (self->count != 0) {
        if (self->entries[hi].key < key || key < self->entries[0].key) {
            return 0;
        }
        int mid = hi >> 1;
        while (self->entries[mid].key != key) {
            if (key < self->entries[mid].key) {
                hi = mid;
            } else {
                lo = mid;
            }
            int next = ((hi - lo) >> 1) + lo;
            if (next == mid) {
                return 0;
            }
            mid = next;
        }
        return self->entries[mid].value;
    }
    return 0;
}
#endif

INCLUDE_ASM("worldpainter/quadtree", func_002C6E98);

