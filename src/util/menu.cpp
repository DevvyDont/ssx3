#include "common.h"

struct cMenuItem {
    char pad_0x00[0x4];
    int field_0x4;
    void* field_0x8;
    int field_0xC;
    void* vtable; // 0x10
};
extern void* D_00486F28[16];

//100%
INCLUDE_ASM("util/menu", cMenuItem_cMenuItem__FP9cMenuItemPv);
#ifdef SKIP_ASM
cMenuItem* cMenuItem_cMenuItem(cMenuItem* self, void* text)
{
    self->vtable = D_00486F28;
    self->field_0x4 = 1;
    self->field_0x8 = text;
    self->field_0xC = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA280);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_002CA280(int* self, int flags)
{
    *(void***)((char*)self + 0x10) = D_00486F28;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA2B0);
#ifdef SKIP_ASM
extern "C" int func_002CA3E8(void** self);

extern "C" int func_002CA2B0(void* self)
{
    switch (func_002CA3E8((void**)self)) {
    case 8:
        return 1;
    case 7:
        return 2;
    case 0:
        return 4;
    case 1:
        return 3;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA320);
#ifdef SKIP_ASM
extern "C" int func_002CA378(void* self);
extern "C" int func_002CA3B0(void* self);

extern "C" int func_002CA320(void* self)
{
    int r = 0;
    if (func_002CA378(self) != 0) {
        r = func_002CA3B0(self) == 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA368__FPv);
#ifdef SKIP_ASM
void func_002CA368(void* self)
{
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA370__FPv);
#ifdef SKIP_ASM
void func_002CA370(void* self)
{
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA378);
#ifdef SKIP_ASM
extern "C" int func_002CA378(void* self)
{
    return *(int*)((char*)self + 0x4) & 1;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA388);
#ifdef SKIP_ASM
extern "C" void func_002CA388(void* self, int enable)
{
    if (enable) {
        *(int*)((char*)self + 0x4) |= 1;
    } else {
        *(int*)((char*)self + 0x4) &= ~1;
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA3B0);
#ifdef SKIP_ASM
extern "C" int func_002CA3B0(void* self)
{
    return *(int*)((char*)self + 0x4) & 8;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA3E8);
#ifdef SKIP_ASM
int func_002CC248(void* self);

extern "C" int func_002CA3E8(void** self)
{
    return func_002CC248(*(void**)((char*)*self + 0x124));
}
#endif

extern "C" void* func_002CC260(int);

//100%
INCLUDE_ASM("util/menu", func_002CA408__FPv);
#ifdef SKIP_ASM
void* func_002CA408(void* self)
{
    return func_002CC260(*(int*)((char*)*(void**)self + 0x124));
}
#endif

extern "C" void* func_002CC280(int);

//100%
INCLUDE_ASM("util/menu", func_002CA428__FPv);
#ifdef SKIP_ASM
void* func_002CA428(void* self)
{
    return func_002CC280(*(int*)((char*)*(void**)self + 0x124));
}
#endif

extern "C" void* func_002CC2A0(int);

//100%
INCLUDE_ASM("util/menu", func_002CA448__FPv);
#ifdef SKIP_ASM
void* func_002CA448(void* self)
{
    return func_002CC2A0(*(int*)((char*)*(void**)self + 0x124));
}
#endif

extern "C" void* func_002CC2C0(int);

//100%
INCLUDE_ASM("util/menu", func_002CA468__FPv);
#ifdef SKIP_ASM
void* func_002CA468(void* self)
{
    return func_002CC2C0(*(int*)((char*)*(void**)self + 0x124));
}
#endif

extern "C" void* func_002CC2E0(int);

//100%
INCLUDE_ASM("util/menu", func_002CA488__FPv);
#ifdef SKIP_ASM
void* func_002CA488(void* self)
{
    return func_002CC2E0(*(int*)((char*)*(void**)self + 0x124));
}
#endif

extern "C" void* func_002CC318(int);

//100%
INCLUDE_ASM("util/menu", func_002CA4A8__FPv);
#ifdef SKIP_ASM
void* func_002CA4A8(void* self)
{
    return func_002CC318(*(int*)((char*)*(void**)self + 0x124));
}
#endif

INCLUDE_ASM("util/menu", func_002CA4C8);

INCLUDE_ASM("util/menu", func_002CA768);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CA988);
#ifdef SKIP_ASM
extern "C" void* func_002CBF30(void* self);
extern char D_004D5380[];
extern char D_004D5390[];

struct func_002CA988_sVec3 {
    float x;
    float y;
    float z;
};
extern func_002CA988_sVec3 D_004D53A0;
extern func_002CA988_sVec3 D_004D53B0;

struct func_002CA988_sColor {
    float a;
    float r;
    float g;
    float b;
};

extern "C" void* func_002CA768(void* self, int a1, int a2, int a3, void* a4, char* style, func_002CA988_sColor* col);

// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the body reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

void* func_002CA988_5(void* self, int a1, int a2, int a3, void* a4)
{
    int focused = self == func_002CBF30(*(void**)self);
    char* style = focused ? D_004D5380 : D_004D5390;
    func_002CA988_sVec3* v = focused ? &D_004D53A0 : &D_004D53B0;
    func_002CA988_sColor c;
    c.a = 1.0f;
    c.r = v->x;
    c.g = v->y;
    c.b = v->z;
    return func_002CA768(self, a1, a2, a3, a4, style, &c);
}
#endif

extern void* D_00486ED0[];

//100%
INCLUDE_ASM("util/menu", func_002CAA58__FPv);
#ifdef SKIP_ASM
void* func_002CAA58(void* self)
{
    int t0 = 0;
    *(int*)self = t0;
    *(int*)((char*)self + 0x12c) = (int)(void*)D_00486ED0;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)((char*)self + 0x8) = t0;
    *(int*)((char*)self + 0x124) = t0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CAA80);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern void* D_00486ED0[];

extern "C" void func_002CAA80(int* self, int flags)
{
    *(void***)((char*)self + 0x12C) = D_00486ED0;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CAAB0);
#ifdef SKIP_ASM
class func_002CAAB0_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int v03();
};

extern "C" void func_002CAB08(void* self);

extern "C" void func_002CAAB0(void* self)
{
    func_002CAAB0_cItem* item = *(func_002CAAB0_cItem**)((char*)self + (*(int*)((char*)self + 0x4) << 2) + 0xC);
    if (item->v03() == 0) {
        func_002CAB08(self);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CAB08);
#ifdef SKIP_ASM
class func_002CAB08_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int);
    virtual void v07();
};

struct func_002CAB08_sMenu {
    int count;
    int cur;
    int f8;
    func_002CAB08_cItem* items[1];
};

extern "C" void func_002CAB08(void* menu)
{
    func_002CAB08_sMenu* self = (func_002CAB08_sMenu*)menu;
    int start = self->cur;
    do {
        self->cur = (self->cur + 1) % self->count;
        if (self->cur == start) {
            return;
        }
    } while (!self->items[self->cur]->v03());
    self->items[start]->v07();
    self->items[self->cur]->v06(0);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CABE0);
#ifdef SKIP_ASM
class func_002CABE0_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int);
    virtual void v07();
};

struct func_002CABE0_sMenu {
    int count;
    int cur;
    int f8;
    func_002CABE0_cItem* items[1];
};

extern "C" void func_002CABE0(void* menu)
{
    func_002CABE0_sMenu* self = (func_002CABE0_sMenu*)menu;
    int start = self->cur;
    do {
        self->cur = (self->cur + self->count - 1) % self->count;
        if (self->cur == start) {
            return;
        }
    } while (!self->items[self->cur]->v03());
    self->items[start]->v07();
    self->items[self->cur]->v06(1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CACC0);
#ifdef SKIP_ASM
class func_002CACC0_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02();
};

extern "C" void func_002CABE0(void* self);

extern "C" int func_002CACC0(void* self)
{
    func_002CAAB0(self);
    func_002CACC0_cItem* item = *(func_002CACC0_cItem**)((char*)self + (*(int*)((char*)self + 0x4) << 2) + 0xC);
    int r = item->v02();
    switch (r) {
    case 3:
        func_002CAB08(self);
        return 0;
    case 4:
        func_002CABE0(self);
        return 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CAD48__FPv);
#ifdef SKIP_ASM
int func_002CAD48(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CAD50__FPv);
#ifdef SKIP_ASM
void func_002CAD50(void* self)
{
}
#endif

INCLUDE_ASM("util/menu", func_002CAD58);

INCLUDE_ASM("util/menu", func_002CB180);

INCLUDE_ASM("util/menu", func_002CB2F8);

INCLUDE_ASM("util/menu", func_002CB350);

INCLUDE_ASM("util/menu", func_002CB498);

//100%
INCLUDE_ASM("util/menu", func_002CB880);
#ifdef SKIP_ASM
extern char D_004D5380[];
extern char D_004D5390[];

typedef void (*fn2CB880)(void*, void*, char*);

struct sVEntry2CB880 {
    short delta;
    short index;
    fn2CB880 fn;
};

extern "C" void func_002CB880(void* self, void* a1, int on)
{
    sVEntry2CB880* vt = *(sVEntry2CB880**)((char*)self + 0x12C);
    fn2CB880* f = &vt[8].fn;
    (*f)((char*)self + vt[8].delta, a1, on ? D_004D5380 : D_004D5390);
}
#endif

INCLUDE_ASM("util/menu", func_002CB8C8);

INCLUDE_ASM("util/menu", func_002CBAC8);

//100%
INCLUDE_ASM("util/menu", cMenu_addItem);
#ifdef SKIP_ASM
struct cMenuList {
    int count;          // 0x0
    int selected;       // 0x4
    int unk8;           // 0x8
    void* items[70];    // 0xC
    int wrap;           // 0x124
};

extern "C" void cMenu_addItem(cMenuList* menu, void** item, int index)
{
    *item = menu;
    if (index < 0) {
        index = menu->count;
    }
    for (int i = menu->count; index < i; i--) {
        menu->items[i] = menu->items[i - 1];
    }
    menu->items[index] = item;
    if (menu->wrap != 0 && menu->selected >= index && menu->count != 0) {
        menu->selected++;
    }
    menu->count++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CBE68);
#ifdef SKIP_ASM
class func_002CBE68_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int);
    virtual void v07();
};

struct func_002CBE68_sMenu {
    int count;          // 0x0
    int selected;       // 0x4
    int unk8;           // 0x8
    func_002CBE68_cItem* items[70];    // 0xC
    int wrap;           // 0x124
};

extern "C" void func_002CBE68(func_002CBE68_sMenu* self, int index)
{
    if (index < 0) {
        index += self->count;
    }
    if (index != self->selected) {
        self->items[self->selected]->v07();
        self->selected = index;
        self->items[index]->v06(2);
        func_002CAAB0(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CBF08);
#ifdef SKIP_ASM
extern "C" void func_002CAAB0(void* self);

extern "C" int func_002CBF08(void* self)
{
    func_002CAAB0(self);
    return *(int*)((char*)self + 0x4);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CBF30);
#ifdef SKIP_ASM
extern "C" void* func_002CBF30(void* self)
{
    return *(void**)((char*)self + (func_002CBF08(self) << 2) + 0xC);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CBF60);
#ifdef SKIP_ASM
class func_002CBF60_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int);
};

struct func_002CBF60_sMenu {
    int count;          // 0x0
    int selected;       // 0x4
    int unk8;           // 0x8
    func_002CBF60_cItem* items[70];    // 0xC
    int wrap;           // 0x124
};

extern "C" void func_002CBF60(void* p, int wrap)
{
    func_002CBF60_sMenu* self = (func_002CBF60_sMenu*)p;
    self->wrap = wrap;
    if (self->selected >= self->count) {
        self->selected = self->count - 1;
    }
    self->items[self->selected]->v06(2);
    func_002CAAB0(self);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CBFD0);
#ifdef SKIP_ASM
struct sVEntry2CBFD0 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct s2CBFD0Menu {
    int count;          // 0x0
    int selected;       // 0x4
    int unk8;           // 0x8
    void* items[70];    // 0xC
    int wrap;           // 0x124
};

extern "C" void func_002CBFD0(void* p)
{
    s2CBFD0Menu* self = (s2CBFD0Menu*)p;
    void* item = self->items[self->selected];
    sVEntry2CBFD0* vt = *(sVEntry2CBFD0**)((char*)item + 0x10);
    vt[7].fn((char*)item + vt[7].delta);
    self->wrap = 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC018);
#ifdef SKIP_ASM
void func_002CC070(void* self);

extern "C" void* func_002CC018(void* self)
{
    *(int*)self = 0;
    func_002CC070(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC048);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_002CC048(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC070__FPv);
#ifdef SKIP_ASM
void func_002CC070(void* self)
{
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(int*)self = 0;
    *(int*)((char*)self + 0x64) = 0;
    *(int*)((char*)self + 0x4c) = -1;
    *(int*)((char*)self + 0x48) = 11;
    *(int*)((char*)self + 0x50) = -1;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC098);
#ifdef SKIP_ASM
extern "C" int func_002CC098(void* self)
{
    int r = 0;
    if (*(int*)self != 0) {
        r = *(int*)((char*)self + 0x54) != 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC0B8__FPvi);
#ifdef SKIP_ASM
void func_002CC0B8(void* self, int val)
{
    *(int*)((char*)self + 0x54) = val;
}
#endif

INCLUDE_ASM("util/menu", func_002CC0C0);

//100%
INCLUDE_ASM("util/menu", func_002CC248__FPv);
#ifdef SKIP_ASM
int func_002CC248(void* self)
{
    return *(int*)((char*)self + 0x48);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC250__FPv);
#ifdef SKIP_ASM
int func_002CC250(void* self)
{
    return *(int*)((char*)self + 0x4C);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC258__FPv);
#ifdef SKIP_ASM
int func_002CC258(void* self)
{
    return *(int*)((char*)self + 0x50);
}
#endif

extern "C" void* func_00320BF0(int, int);

//100%
INCLUDE_ASM("util/menu", func_002CC260__FPv);
#ifdef SKIP_ASM
void* func_002CC260(void* self)
{
    return func_00320BF0(*(int*)((char*)self + 0x5c), 0x3e);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC280__FPv);
#ifdef SKIP_ASM
void* func_002CC280(void* self)
{
    return func_00320BF0(*(int*)((char*)self + 0x5c), 0x3f);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC2A0__FPv);
#ifdef SKIP_ASM
void* func_002CC2A0(void* self)
{
    return func_00320BF0(*(int*)((char*)self + 0x5c), 0x40);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC2C0__FPv);
#ifdef SKIP_ASM
void* func_002CC2C0(void* self)
{
    return func_00320BF0(*(int*)((char*)self + 0x5c), 0x41);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC2E0);
#ifdef SKIP_ASM
// PORT: func_00320BF0 is declared returning void* in this unit; this caller reads a float result ($f0).
float func_00320BF0_f(int, int) __asm__("func_00320BF0");
// PORT: the unit declares func_002CC2E0 as void*(int); the body takes the object pointer.
int func_002CC2E0_impl(void* self) __asm__("func_002CC2E0");

int func_002CC2E0_impl(void* self)
{
    return func_00320BF0_f(*(int*)((char*)self + 0x5c), 0x42) != 0.0f;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC318);
#ifdef SKIP_ASM
// PORT: func_00320BF0 is declared returning void* in this unit; this caller reads a float result ($f0).
float func_00320BF0_f(int, int) __asm__("func_00320BF0");
// PORT: the unit declares func_002CC318 as void*(int); the body takes the object pointer.
int func_002CC318_impl(void* self) __asm__("func_002CC318");

int func_002CC318_impl(void* self)
{
    return func_00320BF0_f(*(int*)((char*)self + 0x5c), 0x43) != 0.0f;
}
#endif

INCLUDE_ASM("util/menu", func_002CC350);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CC3B8);
#ifdef SKIP_ASM
void func_002CC0B8(void*, int);

class func_002CC3B8_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(void*);
    virtual void v07();
};

struct func_002CC3B8_sStack {
    int count;
    func_002CC3B8_cMenu* items[1];
};

extern "C" void func_002CC3B8(void* stack, void* item)
{
    func_002CC3B8_sStack* self = (func_002CC3B8_sStack*)stack;
    if (self->count != 0) {
        self->items[self->count - 1]->v07();
    }
    self->items[self->count++] = (func_002CC3B8_cMenu*)item;
    self->items[self->count - 1]->v06(self);
    func_002CC0B8(self, 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CC460);
#ifdef SKIP_ASM
class func_002CC460_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(void*);
    virtual void v07();
};

struct func_002CC460_sStack {
    int count;                          // 0x0
    func_002CC460_cMenu* entries[8];    // 0x4
};

extern "C" void func_002CC460(void* p)
{
    func_002CC460_sStack* self = (func_002CC460_sStack*)p;
    self->count--;
    self->entries[self->count]->v07();
    if (self->count != 0) {
        self->entries[self->count - 1]->v06(self);
        if (self->count != 0) {
            return;
        }
    }
    func_002CC0B8(self, 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC578);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int func_002CA3B0(void* self);
extern "C" int func_002CA3E8(void** self);

class func_002CC578_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04(void*);
};

extern "C" int func_002CC578(void** self)
{
    int r = func_002CA2B0(self);
    if (func_002CA3B0(self) == 0 && r == 0) {
        if ((self[6] != 0 && self[7] != 0 && (func_002CA3E8(self) == 2 || func_002CA3E8(self) == 3)) || func_002CA3E8(self) == 6) {
            *(int*)self[8] ^= 1;
            ((func_002CC578_cMenu*)self[0])->v04(self);
        }
    }
    return r;
}
#endif

INCLUDE_ASM("util/menu", func_002CC648);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CC758);
#ifdef SKIP_ASM
// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

extern "C" void func_002CC758(void* self, int a1)
{
    void* a = *(void**)((char*)self + 0x18);
    void* b;
    if (a != 0 && (b = *(void**)((char*)self + 0x1C)) != 0) {
        func_002CA988_5(self, a1, *(int*)((char*)self + 0x14), 0, **(int**)((char*)self + 0x20) ? a : b);
    } else {
        func_002CA988_5(self, a1, 0, *(int*)((char*)self + 0x14), 0);
    }
}
#endif

INCLUDE_ASM("util/menu", func_002CC8F0);

INCLUDE_ASM("util/menu", func_002CCB18);

INCLUDE_ASM("util/menu", func_002CCC38);

//100%
INCLUDE_ASM("util/menu", cSubMenuItem_cSubMenuItem);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* D_00486DE0[];

struct cSubMenuItem_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    void* field_0x14;
    void* field_0x18;
};

extern "C" cSubMenuItem_sItem* cSubMenuItem_cSubMenuItem(cSubMenuItem_sItem* self, void* a1, void* a2)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = a2;
    self->field_0x18 = a1;
    self->vtable = D_00486DE0;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CCD20);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int func_002CA3E8(void** self);
extern "C" void func_002CC3B8(void* stack, void* item);

extern "C" int func_002CCD20(void* self)
{
    int r = func_002CA2B0(self);
    if (func_002CA3B0(self) == 0 && r == 0 && func_002CA3E8((void**)self) == 6) {
        func_002CC3B8(*(void**)((char*)*(void**)self + 0x124), *(void**)((char*)self + 0x18));
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CCD90);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);

extern "C" void* func_002CCD90(void* self, void* a1)
{
    func_002CA4C8(self, a1, 0, *(int*)((char*)a1 + 0x14), 0);
    return self;
}
#endif

extern "C" void* func_002CA988(void*, int, int, int);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CCDC8__FPvi);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

void* func_002CCDC8(void* self, int a1)
{
    return func_002CA988_5(self, a1, 0, *(int*)((char*)self + 0x14), 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CCDF0);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* D_00486D90[];

struct s2CCDF0Item {
    char pad_0x00[0x10];
    void* vtable;
    void* field_0x14;
};

extern "C" s2CCDF0Item* func_002CCDF0(s2CCDF0Item* self, void* text)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = text;
    self->vtable = D_00486D90;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CCE38);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int func_002CA3E8(void** self);
extern "C" void func_002CC460(void* stack);

extern "C" int func_002CCE38(void* self)
{
    int r = func_002CA2B0(self);
    if (func_002CA3B0(self) == 0 && r == 0 && func_002CA3E8((void**)self) == 6) {
        func_002CC460(*(void**)((char*)*(void**)self + 0x124));
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CCEA8);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);

extern "C" void* func_002CCEA8(void* self, void* a1)
{
    func_002CA4C8(self, a1, 0, *(int*)((char*)a1 + 0x14), 0);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CCEE0__FPvi);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

void* func_002CCEE0(void* self, int a1)
{
    return func_002CA988_5(self, a1, 0, *(int*)((char*)self + 0x14), 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CCF08);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* D_00486D40[];

struct s2CCF08Item {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    int field_0x14;
    int field_0x18;
    int field_0x1C;
    int field_0x20;
};

extern "C" s2CCF08Item* func_002CCF08(s2CCF08Item* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = 0;
    self->vtable = D_00486D40;
    self->field_0x1C = 0;
    self->field_0x20 = 0;
    self->field_0x18 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CCF98);
#ifdef SKIP_ASM
extern void* D_00486D40[];

struct func_002CCF98_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    int field_0x14;
    int field_0x18;
    int field_0x1C;
    int field_0x20;
};

extern "C" func_002CCF98_sItem* func_002CCF98(func_002CCF98_sItem* self, void* text, int a2, int a3)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->field_0x14 = a2;
    self->vtable = D_00486D40;
    self->field_0x1C = a3;
    self->field_0x20 = 0;
    self->field_0x18 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD008);
#ifdef SKIP_ASM
class func_002CD008_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int v03(void* item, void* data);
};

extern "C" int func_002CA2B0(void* self);
extern "C" int func_002CA3E8(void** self);

extern "C" int func_002CD008(void* self)
{
    int r = func_002CA2B0(self);
    if (func_002CA3B0(self) == 0 && r == 0 && func_002CA3E8((void**)self) == 6) {
        r = (*(func_002CD008_cMenu**)self)->v03(self, *(void**)((char*)self + 0x1C));
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD090);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);

extern "C" void* func_002CD090(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x18);
    if (p != 0) {
        func_002CA4C8(self, a1, *(int*)((char*)a1 + 0x14), 0, p);
    } else {
        func_002CA4C8(self, a1, 0, *(int*)((char*)a1 + 0x14), 0);
    }
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD0E8);
#ifdef SKIP_ASM
// PORT: The unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

extern "C" void func_002CD0E8(void* self, int a1)
{
    void* p = *(void**)((char*)self + 0x18);
    if (p != 0) {
        func_002CA988_5(self, a1, *(int*)((char*)self + 0x14), 0, p);
    } else {
        func_002CA988_5(self, a1, 0, *(int*)((char*)self + 0x14), 0);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD1D0);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int func_002CA3E8(void** self);

extern "C" int func_002CD1D0(void* self)
{
    int r = func_002CA2B0(self);
    if (func_002CA3B0(self) == 0 && r == 0 && func_002CA3E8((void**)self) == 6) {
        (*(void (**)())((char*)self + 0x18))();
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD240);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern void* D_004866C8[];

extern "C" void* func_002CD240(void* self, void* a1)
{
    func_002CA4C8(self, a1, *(int*)((char*)a1 + 0x14), 0, D_004866C8);
    return self;
}
#endif

extern void* D_004866C8[];
extern "C" void* func_002CA988(void*, int, int, int);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD278__FPvi);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

void* func_002CD278(void* self, int a1)
{
    return func_002CA988_5(self, a1, *(int*)((char*)self + 0x14), 0, D_004866C8);
}
#endif

extern void* D_00486CA0[16];

struct cNullMenuItem {
    char pad_0x00[0x10];
    void* vtable;
    void* field_0x14;
};

//100%
INCLUDE_ASM("util/menu", cNullMenuItem_cNullMenuItem__FP13cNullMenuItemPv);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
// A void* constant -1 materializes as lui/ori; the original passed an int.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = text;
    self->vtable = D_00486CA0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD2E8__FPv);
#ifdef SKIP_ASM
int func_002CD2E8(void* self)
{
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD2F0);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);

extern "C" void* func_002CD2F0(void* self, void* a1)
{
    func_002CA4C8(self, a1, 0, *(int*)((char*)a1 + 0x14), 0);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD328__FPvi);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

void* func_002CD328(void* self, int a1)
{
    return func_002CA988_5(self, a1, 0, *(int*)((char*)self + 0x14), 0);
}
#endif

extern void* D_00486C50[16];

struct cSpaceMenuItem {
    char pad_0x00[0x10];
    void* vtable;
    void* field_0x14;
};

//100%
INCLUDE_ASM("util/menu", cSpaceMenuItem_cSpaceMenuItem__FP14cSpaceMenuItemPv);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
// A void* constant -1 materializes as lui/ori; the original passed an int.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = text;
    self->vtable = D_00486C50;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD398__FPv);
#ifdef SKIP_ASM
int func_002CD398(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD3A0);
#ifdef SKIP_ASM
struct s2CD3A0Vec {
    float x, y, z, w;
};

extern "C" s2CD3A0Vec func_002CD3A0(void* self)
{
    s2CD3A0Vec v;
    v.x = v.y = v.z = 0.0f;
    v.w = (float)*(int*)((char*)self + 0x14);
    return v;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD3F0__FPv);
#ifdef SKIP_ASM
void func_002CD3F0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD578);
#ifdef SKIP_ASM
extern "C" void func_002CD578(void* self)
{
    if (*(signed char*)((char*)self + 0x1C) <= 0) {
        **(signed char**)((char*)self + 0x18) = 0;
    } else {
        **(signed char**)((char*)self + 0x18) = *(signed char*)((char*)self + 0x1C);
    }
}
#endif

INCLUDE_ASM("util/menu", func_002CD5A0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD7B8);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern void* D_004866E0[];

extern "C" void* func_002CD7B8(void* self, void* a1)
{
    func_002CA4C8(self, a1, *(int*)((char*)a1 + 0x14), 0, D_004866E0);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD7F0);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");
extern char D_004A3970[];

extern "C" void func_002CD7F0(void* self, int a1)
{
    char buf[0x70];
    sprintf(buf, D_004A3970, **(signed char**)((char*)self + 0x18));
    func_002CA988_5(self, a1, *(int*)((char*)self + 0x14), 0, buf);
}
#endif

//100%
INCLUDE_ASM("util/menu", cIntMenuItem_cIntMenuItem);
#ifdef SKIP_ASM
extern void* D_00486BB0[];

struct cIntMenuItem_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    int field_0x14;
    int field_0x18;
    int field_0x1C;
    int field_0x20;
    int field_0x24;
    int field_0x28;
    int field_0x2C;
};

extern "C" cIntMenuItem_sItem* cIntMenuItem_cIntMenuItem(cIntMenuItem_sItem* self, void* text, int a2, int a3, int a4, int a5)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->field_0x14 = a5;
    self->field_0x18 = a2;
    self->field_0x1C = a3;
    self->field_0x20 = a4;
    self->vtable = D_00486BB0;
    self->field_0x28 = 0;
    self->field_0x24 = 0;
    self->field_0x2C = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD9B8);
#ifdef SKIP_ASM
extern "C" void func_002CD9B8(void* self)
{
    int* p = *(int**)((char*)self + 0x18);
    int v = *(int*)((char*)self + 0x1c);
    if (v <= 0) {
        *p = 0;
    } else {
        *p = v;
    }
}
#endif

INCLUDE_ASM("util/menu", func_002CD9D8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CDBF0);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern void* D_004866E0[];

extern "C" void* func_002CDBF0(void* self, void* a1)
{
    func_002CA4C8(self, a1, *(int*)((char*)a1 + 0x14), 0, D_004866E0);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CDC28);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");
extern char D_004A3970[];

extern "C" void func_002CDC28(void* self, int a1)
{
    char buf[0x70];
    sprintf(buf, D_004A3970, **(int**)((char*)self + 0x18));
    func_002CA988_5(self, a1, *(int*)((char*)self + 0x14), 0, buf);
}
#endif

//100%
INCLUDE_ASM("util/menu", cFloatMenuItem_cFloatMenuItem);
#ifdef SKIP_ASM
extern void* D_00486B60[];

struct cFloatMenuItem_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    int field_0x14;
    int field_0x18;
    float field_0x1C;
    float field_0x20;
    float field_0x24;
    int field_0x28;
    int field_0x2C;
    int field_0x30;
};

extern "C" cFloatMenuItem_sItem* cFloatMenuItem_cFloatMenuItem(cFloatMenuItem_sItem* self, void* text, int a2, int a3, float f1, float f2)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->field_0x14 = a3;
    self->field_0x18 = a2;
    self->field_0x1C = f1;
    self->field_0x20 = f2;
    self->vtable = D_00486B60;
    self->field_0x24 = 1.0f;
    self->field_0x30 = 0;
    self->field_0x2C = 0;
    self->field_0x28 = 0;
    return self;
}
#endif

INCLUDE_ASM("util/menu", func_002CDD78);

//100%
INCLUDE_ASM("util/menu", func_002CDF78);
#ifdef SKIP_ASM
extern "C" void func_002CDF78(void* self)
{
    float* p = *(float**)((char*)self + 0x18);
    float v = *(float*)((char*)self + 0x1c);
    if (v <= 0.0f) {
        *p = 0.0f;
    } else {
        *p = v;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CDFA0);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern void* D_004866F0[];

extern "C" void* func_002CDFA0(void* self, void* a1)
{
    func_002CA4C8(self, a1, *(int*)((char*)a1 + 0x14), 0, D_004866F0);
    return self;
}
#endif

INCLUDE_ASM("util/menu", func_002CDFD8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CE100);
#ifdef SKIP_ASM
// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");
extern "C" void cBXString__cBXString(void* self, int flags);

struct func_002CE100_sString {
    char* str;
};

// returns a cBXString by value (hidden result pointer in $4)
extern "C" void func_002CDFD8(func_002CE100_sString* out, float v);

extern "C" void func_002CE100(void* self, int a1)
{
    func_002CE100_sString s;
    func_002CDFD8(&s, **(float**)((char*)self + 0x18) * *(float*)((char*)self + 0x24) + *(float*)((char*)self + 0x28));
    func_002CA988_5(self, a1, *(int*)((char*)self + 0x14), 0, s.str);
    cBXString__cBXString(&s, 2);
}
#endif

INCLUDE_ASM("util/menu", func_002CE1F0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CE368);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern void* D_004866F0[];

extern "C" void* func_002CE368(void* self, void* a1)
{
    func_002CA4C8(self, a1, *(int*)((char*)a1 + 0x14), 0, D_004866F0);
    return self;
}
#endif

INCLUDE_ASM("util/menu", cAngleMenuItem_render);

//100%
INCLUDE_ASM("util/menu", func_002CE418);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern void* D_00486AC0[];

struct func_002CE418_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    int field_0x14;
    char name[0x10]; // 0x18
    int field_0x28;
};

extern "C" func_002CE418_sItem* func_002CE418(func_002CE418_sItem* self, int a1, const char* name)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = a1;
    self->field_0x28 = 0;
    self->vtable = D_00486AC0;
    strncpy(self->name, name, 0xF);
    self->name[0xF] = 0;
    return self;
}
#endif

INCLUDE_ASM("util/menu", func_002CE488);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CE820);
#ifdef SKIP_ASM
extern "C" void* func_002CBF30(void* self);
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" char* func_004162D0(char* dst, const char* src);
extern char D_004A39D0[];

struct func_002CE820_sResult {
    int a;
    int b;
    int c;
    int d;
};

static inline func_002CE820_sResult func_002CE820_make(void* item, int id, char* text)
{
    func_002CE820_sResult t;
    func_002CA4C8(&t, item, id, 0, text);
    return t;
}

extern "C" func_002CE820_sResult func_002CE820(void* item)
{
    char buf[0x20];
    char* name = (char*)item + 0x18;
    int focused = item == func_002CBF30(*(void**)item);
    strcpy(buf, name);
    int len = strlen(name);
    if (focused && *(int*)((char*)item + 0x28) == len && *(int*)((char*)item + 0x28) < 15) {
        func_004162D0(buf, D_004A39D0);
    }
    func_002CE820_sResult r = func_002CE820_make(item, *(int*)((char*)item + 0x14), buf);
    return r;
}
#endif

INCLUDE_ASM("util/menu", func_002CE910);

//100%
INCLUDE_ASM("util/menu", cColorMenuItem__cColorMenuItem);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* D_00486A70[];

struct cColorMenuItem__cColorMenuItem_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    void* field_0x14;
    void* field_0x18;
};

extern "C" cColorMenuItem__cColorMenuItem_sItem* cColorMenuItem__cColorMenuItem(cColorMenuItem__cColorMenuItem_sItem* self, void* a1, void* a2)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = a1;
    self->field_0x18 = a2;
    self->vtable = D_00486A70;
    return self;
}
#endif

INCLUDE_ASM("util/menu", func_002CED78);

INCLUDE_ASM("util/menu", func_002CEEE8);

INCLUDE_ASM("util/menu", func_002CF1C8);

//100%
INCLUDE_ASM("util/menu", func_002CF788);
#ifdef SKIP_ASM
class func_002CF788_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int v09(int a, int b);
    virtual int v10(int a);
};

extern "C" int func_002CF788(void* self, int a, int b)
{
    return (*(func_002CF788_cItem**)((char*)self + 0x130))->v09(a, b);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CF7B8);
#ifdef SKIP_ASM
class func_002CF7B8_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int v09(int a, int b);
    virtual void v10(int a);
};

extern "C" void func_002CF7B8(void* self, int a)
{
    (*(func_002CF7B8_cItem**)((char*)self + 0x130))->v10(a);
}
#endif

INCLUDE_ASM("util/menu", func_002CF860);

//100%
INCLUDE_ASM("util/menu", func_002CF8D8);
#ifdef SKIP_ASM
class func_002CF8D8_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
};

extern "C" void* func_002CBF30(void* self);

extern "C" void func_002CF8D8(void* self)
{
    ((func_002CF8D8_cItem*)func_002CBF30((char*)self + 0x18))->v08();
}
#endif

void cMenu_addItem(void* menu, void* item);

//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_addItem__FPvT0);
#ifdef SKIP_ASM
void cExpandMenuItem_addItem(void* self, void* item)
{
    cMenu_addItem((char*)self + 0x18, item);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CFA08);
#ifdef SKIP_ASM
extern "C" int func_002CA3B0(void* self);
extern "C" void func_002CAAB0(void* self);
extern "C" void func_002CAB08(void* self);
extern "C" void func_002CABE0(void* self);

class func_002CFA08_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02();
};

struct func_002CFA08_sOwner {
    char pad[0x18];
    func_002CBE68_sMenu menu;
};

// PORT: returns a key code (int) through the unit's void* declaration.
extern "C" void* func_002CFA08(void* p)
{
    func_002CFA08_sOwner* self = (func_002CFA08_sOwner*)p;
    func_002CAAB0(&self->menu);
    int prev = self->menu.selected;
    int key = ((func_002CFA08_cItem*)self->menu.items[prev])->v02();
    if (func_002CA3B0(self) != 0) {
        return (void*)key;
    }
    switch (key) {
    case 3:
        func_002CAB08(&self->menu);
        if (prev < self->menu.selected) {
            return 0;
        }
        func_002CBE68(&self->menu, prev);
        return (void*)3;
    case 4:
        func_002CABE0(&self->menu);
        if (self->menu.selected < prev) {
            return 0;
        }
        func_002CBE68(&self->menu, prev);
        return (void*)4;
    }
    return (void*)key;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CFAF8);
#ifdef SKIP_ASM
class func_002CFAF8_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int v03(int a, int b);
};

extern "C" int func_002CFAF8(func_002CFAF8_cMenu** self, int a, int b)
{
    return (*self)->v03(a, b);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CFB28);
#ifdef SKIP_ASM
class func_002CFB28_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04(void*);
};

extern "C" void func_002CFB28(func_002CFB28_cMenu** self)
{
    (*self)->v04(self);
}
#endif

INCLUDE_ASM("util/menu", func_002CFB58);

INCLUDE_ASM("util/menu", func_002CFD28);

INCLUDE_ASM("util/menu", func_002CFE78);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CFEF8);
#ifdef SKIP_ASM
extern "C" void func_002CBFD0(void* p);

extern "C" void func_002CFEF8(void* self)
{
    func_002CA370(self);
    func_002CBFD0((char*)self + 0x18);
}
#endif

//100%
INCLUDE_ASM("util/menu", cRGBTitleMenuItem_cRGBTitleMenuItem);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* D_00486968[];

struct cRGBTitleMenuItem_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    void* field_0x14;
    void* field_0x18;
};

extern "C" cRGBTitleMenuItem_sItem* cRGBTitleMenuItem_cRGBTitleMenuItem(cRGBTitleMenuItem_sItem* self, void* a1, void* a2)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = a2;
    self->field_0x18 = a1;
    self->vtable = D_00486968;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CFF80__FPv);
#ifdef SKIP_ASM
int func_002CFF80(void* self)
{
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CFF88);
#ifdef SKIP_ASM
struct func_002CFF88_sRect {
    float x;
    float y;
    float w;
    float h;
};

// PORT: `>?` (g++ max operator); returns the rect through the hidden result pointer.
extern "C" func_002CFF88_sRect* func_002CFF88(func_002CFF88_sRect* out, void* item)
{
    func_002CFF88_sRect r;
    func_002CA4C8(&r, item, *(int*)((char*)item + 0x18), 0, 0);
    r.w += 36.0f;
    r.h = r.h >? 29.0f;
    *out = r;
    return out;
}
#endif

INCLUDE_ASM("util/menu", func_002D0008);

INCLUDE_ASM("util/menu", cARGBMenuItem_cARGBMenuItem);

extern "C" void* func_002CFA08(void* self);

//99.29%
INCLUDE_ASM("util/menu", func_002D03D0__FPv);
#ifdef SKIP_ASM
void* func_002D03D0(void* self)
{
    return func_002CFA08(self);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D0448__FPv);
#ifdef SKIP_ASM
int func_002D0448(void* self)
{
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002D0450);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_00413AF8(float f);
extern char D_00486720[];

extern "C" void* func_002D0450(void* ret, void* item)
{
    char buf[0x80];
    const char* fmt = D_00486720;
    int x = func_00413AF8((*(float**)((char*)item + 0x14))[0]);
    int y = func_00413AF8((*(float**)((char*)item + 0x14))[1]);
    int z = func_00413AF8((*(float**)((char*)item + 0x14))[2]);
    sprintf(buf, fmt, x, y, z);
    func_002CA4C8(ret, item, *(int*)((char*)item + 0x18), 0, buf);
    return ret;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002D0500);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_00413AF8(float f);
extern char D_00486720[];
// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

extern "C" void func_002D0500(void* self, int a1)
{
    char buf[0x80];
    const char* fmt = D_00486720;
    int x = func_00413AF8((*(float**)((char*)self + 0x14))[0]);
    int y = func_00413AF8((*(float**)((char*)self + 0x14))[1]);
    int z = func_00413AF8((*(float**)((char*)self + 0x14))[2]);
    sprintf(buf, fmt, x, y, z);
    func_002CA988_5(self, a1, *(int*)((char*)self + 0x18), 0, buf);
}
#endif

INCLUDE_ASM("util/menu", func_002D06E8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002D08E0);
#ifdef SKIP_ASM
void func_002CA368(void* self);

extern "C" void func_002D08E0(void* self)
{
    func_002CA368(self);
    *(int*)((char*)self + 0x40) = 0;
}
#endif

INCLUDE_ASM("util/menu", func_002D0908);

INCLUDE_ASM("util/menu", func_002D0D48);

INCLUDE_ASM("util/menu", func_002D0EF8);

//100%
INCLUDE_ASM("util/menu", func_002D18B0);
#ifdef SKIP_ASM
extern float D_00445AB0[];

extern "C" float func_002D18B0(float t)
{
    t = t * 160.0f;
    int i = (int)t;
    t = t - (float)i;
    i = i % 160;
    return D_00445AB0[i] * (1.0f - t) + D_00445AB0[i + 1] * t;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002D1928);
#ifdef SKIP_ASM
extern "C" float func_002D18B0(float t);

extern "C" float func_002D1928(int n, float x)
{
    float sum = 0.0f;
    float amp = 1.0f;
    float v = 0.0f;
    float freq = 1.0f;
    int i;
    for (i = 0; i < n; i++) {
        float t = x * freq;
        freq = freq * 2.0f;
        v = func_002D18B0(t) * amp;
        amp = amp * 0.5f;
        sum = sum + v;
    }
    return sum + v;
}
#endif

INCLUDE_ASM("util/menu", func_002D19B8);

INCLUDE_ASM("util/menu", func_002D19E8);

INCLUDE_ASM("util/menu", func_002D1A30);

INCLUDE_ASM("util/menu", func_002D1AC8);

INCLUDE_ASM("util/menu", func_002D1AF0);

INCLUDE_ASM("util/menu", func_002D1B08);

INCLUDE_ASM("util/menu", func_002D1B30);

INCLUDE_ASM("util/menu", func_002D1B58);

//100%
INCLUDE_ASM("util/menu", func_002D1BA0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00144C98(void* iface);

extern "C" int func_002D1BA0()
{
    return func_00144C98(cBE_getInterface_Fv(cBE_getBE(), 0)) + 1;
}
#endif

INCLUDE_ASM("util/menu", func_002D1BD8);

INCLUDE_ASM("util/menu", func_002D1BE0);

INCLUDE_ASM("util/menu", func_002D1BF0);

INCLUDE_ASM("util/menu", func_002D1C20);

INCLUDE_ASM("util/menu", func_002D1C58);

INCLUDE_ASM("util/menu", func_002D1C70);

INCLUDE_ASM("util/menu", func_002D1C98);

INCLUDE_ASM("util/menu", func_002D1CB0);

extern "C" void* func_002FC2C0(void* self);

//100%
INCLUDE_ASM("util/menu", func_002D1CC0__FPv);
#ifdef SKIP_ASM
void* func_002D1CC0(void* self)
{
    return func_002FC2C0(self);
}
#endif

