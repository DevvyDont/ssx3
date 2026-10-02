#include "common.h"

INCLUDE_ASM("sound/bankmonitor", cBankMonitor_cBankMonitor);

INCLUDE_ASM("sound/bankmonitor", func_002ACE40);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002ACF60);
#ifdef SKIP_ASM
extern "C" int func_002ABB80(void*);

extern "C" int func_002ACF60(void* self, int n)
{
    for (int i = 0; i < 64; i++) {
        if (func_002ABB80((char*)*(void**)((char*)self + 0x8EC) + i * 0xC0) != 0) {
            if (--n == 0) {
                return i;
            }
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_Create);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD2A8);
#ifdef SKIP_ASM
extern "C" void func_002ACAC8(void*);

extern "C" void func_002AD2A8(void* self)
{
    *(int*)((char*)self + 0x8E8) = 1;
    for (int i = 0; i < 64; i++) {
        func_002ACAC8((char*)*(void**)((char*)self + 0x8EC) + i * 0xC0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD300);
#ifdef SKIP_ASM
extern "C" void func_002ACB30(void*);

extern "C" void func_002AD300(void* self)
{
    for (int i = 0; i < 64; i++) {
        func_002ACB30((char*)*(void**)((char*)self + 0x8EC) + i * 0xC0);
    }
    *(int*)((char*)self + 0x8E8) = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD398);
#ifdef SKIP_ASM
extern "C" void func_002ABB38(void*);

extern "C" void func_002AD398(void* self, int i)
{
    func_002ABB38((char*)*(void**)((char*)self + 0x8EC) + i * 0xC0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002AD3C0);
#ifdef SKIP_ASM
extern "C" void func_002AD398(void* self, int i);

extern "C" void func_002AD3C0(void* self)
{
    int i;
    for (i = 0; i < 64; i++) {
        func_002AD398(self, i);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD410);
#ifdef SKIP_ASM
extern "C" void func_002ABB38(void*);

struct sBmKeyD410 {
    short a;
    short id;
    union {
        int v;
        short s;
    };
};

static inline int func_002AD410_match(char* e, sBmKeyD410 k)
{
    int r = 0;
    if (k.id == *(short*)(e + 0xAA)) {
        if ((k.id != -1 && k.s == *(short*)(e + 0xAC)) || k.v == *(int*)(e + 0xAC)) {
            r = 1;
        }
    }
    return r;
}

extern "C" void func_002AD410(void* self, sBmKeyD410 key)
{
    for (int i = 0; i < 64; i++) {
        if (func_002AD410_match((char*)*(void**)((char*)self + 0x8EC) + i * 0xC0, key)) {
            func_002ABB38((char*)*(void**)((char*)self + 0x8EC) + i * 0xC0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD4D8);
#ifdef SKIP_ASM
extern "C" void func_002AD4D8(void* self, int id)
{
    for (int i = 0; i < 64; i++) {
        if (*(int*)((char*)*(void**)((char*)self + 0x8EC) + i * 0xC0 + 0x80) == id) {
            func_002ABB38((char*)*(void**)((char*)self + 0x8EC) + i * 0xC0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD550);
#ifdef SKIP_ASM
struct sVoiceC0 { int key; int idx; char pad[0xC0 - 8]; };

extern "C" void* func_002AD550(void* self, int idx, int key)
{
    for (int i = 0; i < 64; i++) {
        if (func_002ABB80((char*)*(void**)((char*)self + 0x8EC) + i * 0xC0) == 0
            && (*(sVoiceC0**)((char*)self + 0x8EC))[i].key == key
            && (*(sVoiceC0**)((char*)self + 0x8EC))[i].idx == idx) {
            return &(*(sVoiceC0**)((char*)self + 0x8EC))[i];
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002AD5F0);
#ifdef SKIP_ASM
extern "C" void* func_002AD550(void* self, int idx, int a2);
extern "C" void func_002ABB38(void*);
extern "C" void func_002ACB80(void* voice, int a, int b, float v);

extern "C" void func_002AD5F0(void* self, int idx, int a2, float v)
{
    void* voice = func_002AD550(self, idx, a2);
    if (voice != 0) {
        if (v != 0.0f) {
            func_002ACB80(voice, -1, 1, v);
        } else {
            func_002ABB38(voice);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD650);
#ifdef SKIP_ASM
extern "C" int func_002ABB80(void*);
extern "C" void func_002ABB38(void*);
// PORT: same ABI as the unit's func_002ACB80(void*, int, int, float); the float is
// the 2nd parameter in the original, which orders the argument moves.
void func_002ACB80_f(void* voice, float v, int a, int b) __asm__("func_002ACB80");

struct sVoiceD650 {
    int state;      // 0x0
    int pad_4[2];
    int id;         // 0xC
    char pad[0xC0 - 0x10];
};

extern "C" void func_002AD650(void* self, int id, float v)
{
    for (int i = 0; i < 64; i++) {
        if (func_002ABB80((char*)*(void**)((char*)self + 0x8EC) + i * 0xC0) == 0) {
            // PORT: pointer arithmetic through int
            sVoiceD650* e = (sVoiceD650*)(i * 0xC0 + *(int*)((char*)self + 0x8EC));
            if (e->state == 1 && e->id == id) {
                if (v != 0.0f) {
                    func_002ACB80_f(e, v, -1, 1);
                } else {
                    func_002ABB38(e);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD810);
#ifdef SKIP_ASM
struct sBankVoice {
    char pad[0x5c];
    float value;
    char pad2[0xc0 - 0x60];
};

extern "C" void func_002AD810(void* self)
{
    for (int i = 0; i < 64; i++) {
        (*(sBankVoice**)((char*)self + 0x8ec))[i].value = -1.0f;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD848__FPv);
#ifdef SKIP_ASM
void* func_002AD848(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 0x4) = -1;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD860);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_002AD860(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD888);
#ifdef SKIP_ASM
extern "C" void func_002AD940(void*);
void func_002AD970(void* self, int a1);
void func_002AD980(void* self);

struct sBmKeyD888 {
    short a;
    short id;
    int v;
};

// PORT: 64-bit long sound key
extern "C" void func_002AD888(void* self, int a1, long key, int b, int c, int d, int e, int f)
{
    func_002AD940(self);
    func_002AD970(self, a1);
    // PORT: the 8-byte key is copied through its in-memory layout
    *(sBmKeyD888*)((char*)self + 0xC) = *(sBmKeyD888*)&key;
    *(int*)((char*)self + 0x18) = b;
    *(int*)((char*)self + 0x1C) = c;
    *(int*)((char*)self + 0x20) = d;
    *(int*)((char*)self + 0x24) = e;
    *(int*)((char*)self + 0x28) = f;
    func_002AD980(self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD930);
#ifdef SKIP_ASM
extern "C" int func_002AD930(void* self)
{
    return *(int*)((char*)self + 0x4) == -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD940);
#ifdef SKIP_ASM
void func_002AD990(void* self);
void func_002AD970(void* self, int a1);

extern "C" void func_002AD940(void* self)
{
    func_002AD990(self);
    func_002AD970(self, -1);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD970__FPvi);
#ifdef SKIP_ASM
void func_002AD970(void* self, int a1)
{
    *(int*)((char*)self + 0x4) = a1;
    *(int*)((char*)self + 0x8) = a1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD980__FPv);
#ifdef SKIP_ASM
void func_002AD980(void* self)
{
    *(int*)self = 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD990__FPv);
#ifdef SKIP_ASM
void func_002AD990(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AD998);
#ifdef SKIP_ASM
class func_002AD998_cObj {
public:
    char data[0x8D8];
    virtual void v01();
};

extern func_002AD998_cObj* D_004A3778;

extern "C" void func_002AD998()
{
    D_004A3778->v01();
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002AD9C8);

INCLUDE_ASM("sound/bankmonitor", func_002ADAC8);

extern "C" void func_002AD998();
extern "C" void* func_003B5910(void*);

//98.75%
INCLUDE_ASM("sound/bankmonitor", func_002ADBF0__FPv);
#ifdef SKIP_ASM
void* func_002ADBF0(void* self)
{
    return func_003B5910((void*)func_002AD998);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002ADC10);
#ifdef SKIP_ASM
extern "C" void func_002ADC10(void* self)
{
    if (*(int*)((char*)self + 0x94) == 0) {
        *(int*)((char*)self + 0x88) += 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002ADC30);
#ifdef SKIP_ASM
extern "C" int func_002ADC30(void* self, int n)
{
    for (int i = 0; i < 48; i++) {
        if (func_002AD930((char*)self + 0x98 + i * 0x2C) != 0) {
            if (--n == 0) {
                return i;
            }
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002ADCA0);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002ADDA0);
#ifdef SKIP_ASM
extern "C" void func_002AD940(void*);

extern "C" void func_002ADDA0(void* self)
{
    char* p = (char*)self + 0x98;
    int i;
    for (i = 47; i >= 0; i--) {
        func_002AD940(p);
        p += 0x2C;
    }
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002ADDE0);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002ADE88);
#ifdef SKIP_ASM
class func_002ADE88_cObj {
public:
    virtual void v01(int a);
};

extern "C" func_002ADE88_cObj* func_002A8FA0();
extern char D_00483C00[];
extern void* D_004A377C;

extern "C" void* func_002ADE88(void* self, int a)
{
    *(void**)((char*)self + 0x4) = D_00483C00;
    D_004A377C = self;
    func_002ADE88_cObj* obj = func_002A8FA0();
    *(func_002ADE88_cObj**)self = obj;
    obj->v01(a);
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002ADEE8);
#ifdef SKIP_ASM
class func_002ADEE8_cObj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03(int a);
};

void operator_delete(int* ptr);
extern char D_00483C00[];
extern void* D_004A377C;

extern "C" void func_002ADEE8(void* self, int flags)
{
    *(void**)((char*)self + 0x4) = D_00483C00;
    (*(func_002ADEE8_cObj**)self)->v03(flags);
    operator_delete(*(int**)self);
    D_004A377C = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002ADF60);
#ifdef SKIP_ASM
int BXrand();

extern "C" int func_002ADF60()
{
    return BXrand();
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002ADF80);
#ifdef SKIP_ASM
class func_002ADF80_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(int a);
};

struct func_002ADF80_sHolder {
    func_002ADF80_cObj* obj;
};

extern "C" int func_002ADF80(func_002ADF80_sHolder* self, int a)
{
    return self->obj->v02(a);
}
#endif

extern void* D_004D3E98[];

//100%
INCLUDE_ASM("sound/bankmonitor", func_002ADFB0__Fi);
#ifdef SKIP_ASM
int func_002ADFB0(int self)
{
    return *(int*)((char*)(void*)D_004D3E98 + self * 4);
}
#endif

extern "C" void* func_002523A8(void* self);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE000__FPv);
#ifdef SKIP_ASM
void* func_002AE000(void* self)
{
    return func_002523A8(self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE020);
#ifdef SKIP_ASM
extern void (*D_004A3798)();

extern "C" int func_002AE020()
{
    if (D_004A3798 != 0) {
        D_004A3798();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE048);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void REAL_abortmessage();
extern "C" void func_004175C8();
extern "C" void* func_00252FA0(void* a0, int a1, int a2);
extern "C" void* func_002526B8(void* block, int size);
extern "C" void SYNCTASK_add(int (*fn)(), int a, int b);
extern int D_004A379C;
extern void (*D_004A3780)();
extern void (*D_004A3784)();
extern void* (*D_004A378C)(void*, int, int);
extern void* (*D_004A3790)(void*, int);
extern void* (*D_004A3794)(void*);

struct sBankMonSlot28_E048 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
};
// D_004D3EF8's own type is defined later in the unit; bind a same-layout view.
extern sBankMonSlot28_E048 D_004D3EF8_E048[] __asm__("D_004D3EF8");

extern "C" void func_002AE048(void (*cb)(), int id)
{
    for (int i = 0; i < 24; i++) {
        D_004D3E98[i] = 0;
        D_004D3EF8_E048[i].f8 = -1;
        D_004D3EF8_E048[i].f4 = -1;
        D_004D3EF8_E048[i].f0 = -1;
        D_004D3EF8_E048[i].f18 = 0;
    }
    D_004A3798 = cb;
    D_004A379C = id;
    func_003B5910((void*)id);
    SYNCTASK_add(func_002AE020, 5, 100);
    D_004A3780 = REAL_abortmessage;
    D_004A3784 = func_004175C8;
    D_004A378C = func_00252FA0;
    D_004A3790 = func_002526B8;
    D_004A3794 = func_002AE000;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE100);
#ifdef SKIP_ASM
extern "C" int func_002AE020();
extern "C" void SYNCTASK_del(int (*fn)());
extern "C" void func_003B5948(int id);
extern void (*D_004A3798)();
extern int D_004A379C;

extern "C" void func_002AE100()
{
    if (D_004A3798 != 0) {
        func_003B5948(D_004A379C);
        SYNCTASK_del(func_002AE020);
        D_004A3798 = 0;
        D_004A379C = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE138);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);

extern "C" void func_002AE138(void* self)
{
    func_00416210(self, 0, 0x70);
    *(int*)((char*)self + 0x0) = -1;
    *(int*)((char*)self + 0x4) = -1;
    *(char*)((char*)self + 0x66) = -1;
    *(int*)((char*)self + 0x5C) = -1;
    *(int*)((char*)self + 0x8) = -1;
    *(char*)((char*)self + 0x65) = 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE188);
#ifdef SKIP_ASM
extern "C" void func_002AE188(void* p)
{
    for (int i = 0; i < 24; i++) {
        if (D_004D3E98[i] == p) {
            D_004D3E98[i] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE1B8);
#ifdef SKIP_ASM
extern "C" int func_002AE1B8(void)
{
    for (int i = 0; i < 24; i++) {
        if (D_004D3E98[i] == 0) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE1F0);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);

extern "C" void func_002AE1F0(int* self)
{
    func_00416210(self, 0, 0x28);
    self[2] = -1;
    self[1] = -1;
    self[0] = -1;
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002AE230);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002AE298);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sBankMonSlot28v {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
};
// sBankMonSlot28 / D_004D3EF8 are defined later in the unit; bind a same-layout view.
extern sBankMonSlot28v D_004D3EF8_v[] __asm__("D_004D3EF8");
extern "C" int func_002AE230(int a, int b);

extern "C" int func_002AE298(int a, int unused, int b, int c)
{
    int i = func_002AE230(a, b);
    if (i >= 0 || (i = func_002AE230(-1, -1)) >= 0) {
        sBankMonSlot28v* e = &D_004D3EF8_v[i];
        e->f1C = 1;
        e->f8 = a;
        e->f0 = b;
        e->f4 = c;
        return c;
    }
    return -13;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002AE328);
#ifdef SKIP_ASM
struct sBankMonSlot28 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
};
extern sBankMonSlot28 D_004D3EF8[];
extern "C" int func_002AE230(int a, int b);
extern "C" int func_003B6300(int id);

extern "C" int func_002AE328(int a, int b)
{
    int none = -1;
    int i = func_002AE230(a, b);
    if (i < 0) {
        return -1;
    }
    sBankMonSlot28* e = &D_004D3EF8[i];
    if (e->f4 >= 0) {
        func_003B6300(-1);
        e->f14 = 0;
        e->f4 = none;
        e->f8 = none;
        e->f0 = none;
        e->f1C = 0;
        e->f24 = none;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002AE3A8);
#ifdef SKIP_ASM
extern sBankMonSlot28 D_004D3EF8[];
extern "C" int func_002AE230(int a, int b);
extern "C" int func_002AE328(int a, int b);

extern "C" int func_002AE3A8(int a)
{
    int best = 0x7FFFFFF;
    int bestIdx = -1;
    int i = 0;
    sBankMonSlot28* tbl = D_004D3EF8;
    int id;
    while (i < 24 && (id = func_002AE230(a, i)) >= 0) {
        int t = tbl[id].f24;
        if (t >= 0 && t < best) {
            best = t;
            bestIdx = i;
        }
        i++;
    }
    if (bestIdx == -1) {
        return 0;
    }
    return func_002AE328(a, bestIdx);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE478);
#ifdef SKIP_ASM
extern "C" int func_003B7B00(int n, int m);

extern "C" int func_002AE478(int n, int m)
{
    int size = n * 0xC + 0x70;
    if (m > 0) {
        size += func_003B7B00(n, m);
    }
    return size + (0x10 - (size & 0xF));
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002AE4C0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002AE690);
#ifdef SKIP_ASM
extern void* (*D_004A3794)(void*);
extern "C" void func_003B7838(int id);

extern "C" void func_002AE690(int idx)
{
    void* m = D_004D3E98[idx];
    if (*(signed char*)((char*)m + 0x64) != 0) {
        int id = func_002AE230(idx, -1);
        while (id >= 0) {
            sBankMonSlot28* e = &D_004D3EF8[id];
            if (D_004A3794 != 0 && e->f14 != 0) {
                D_004A3794((void*)e->f14);
            }
            func_002AE1F0((int*)e);
            id = func_002AE230(idx, -1);
        }
    } else {
        func_003B7838(*(int*)m);
    }
    func_002AE188(m);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE768);
#ifdef SKIP_ASM
extern "C" void func_002AE768(unsigned int idx, int a1, int a2)
{
    if (idx < 24) {
        void* m = D_004D3E98[idx];
        if (m != 0 && *(signed char*)((char*)m + 0x64) == 0) {
            *(int*)((char*)m + 0x28) = a1;
            *(int*)((char*)m + 0x2c) = a2;
        }
    }
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002AE7A8);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AE9F8);
#ifdef SKIP_ASM
extern "C" void func_003B7D80(void*);

extern "C" void func_002AE9F8(int i)
{
    void* p = D_004D3E98[i];
    if (*(signed char*)((char*)p + 0x64) == 0) {
        func_003B7D80(*(void**)((char*)*(void**)((char*)p + 0x6C) + 0x4));
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AEAA8);
#ifdef SKIP_ASM
extern "C" void func_002AEFA8(int a0);

struct sMonEntC { int f0; int f4; int f8; };

extern "C" int func_002AEAA8(int idx, int sub)
{
    void* m = D_004D3E98[idx];
    if (m != 0 && *(signed char*)((char*)m + 0x65) != 0) {
        func_002AEFA8(0);
    }
    if (sub < 0) {
        return *(int*)((char*)m + 0x54);
    }
    if (sub < *(signed char*)((char*)m + 0x66)) {
        if ((*(sMonEntC**)((char*)m + 0x6C))[sub].f4 >= 0) {
            return (*(sMonEntC**)((char*)m + 0x6C))[sub].f8;
        }
        return -8;
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AEBD0);
#ifdef SKIP_ASM
extern "C" void func_002AEFA8(int a0);

extern "C" int func_002AEBD0(int idx)
{
    if (D_004D3E98[idx] != 0) {
        if (*(signed char*)((char*)D_004D3E98[idx] + 0x65) != 0) {
            func_002AEFA8(0);
        }
        return *(int*)((char*)D_004D3E98[idx] + 0x58);
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AEC28);
#ifdef SKIP_ASM
extern "C" int func_002AEC28(int idx)
{
    void* m = D_004D3E98[idx];
    if (m == 0) {
        return -8;
    }
    *(char*)((char*)m + 0x65) = 1;
    return 1;
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002AEC58);

INCLUDE_ASM("sound/bankmonitor", func_002AEFA8);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF2F0);
#ifdef SKIP_ASM
extern "C" void func_002AEFA8(int a0);

extern "C" int func_002AF2F0(int idx, int* a, int* b)
{
    void* m = D_004D3E98[idx];
    if (m == 0) {
        return -8;
    }
    if (*(signed char*)((char*)m + 0x65) != 0) {
        func_002AEFA8(0);
    }
    *a = *(int*)((char*)m + 0x48);
    *b = *(int*)((char*)m + 0x4C);
    if (*(int*)((char*)m + 0x60) != 0) {
        return *(int*)((char*)m + 0x60);
    }
    return *(int*)((char*)m + 0x5C);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF370);
#ifdef SKIP_ASM
extern "C" int func_003E4E98();
extern "C" void func_004139F8(float);
extern int D_00450DC8[];

extern "C" void func_002AF370(void)
{
    func_004139F8((float)func_003E4E98() / (float)D_00450DC8[0] * 1000.0f);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF428);
#ifdef SKIP_ASM
extern "C" void func_002AEFA8(int a0);

extern "C" int func_002AF428(int idx)
{
    if (D_004D3E98[idx] != 0) {
        if (*(signed char*)((char*)D_004D3E98[idx] + 0x65) != 0) {
            func_002AEFA8(0);
        }
        return *(int*)((char*)D_004D3E98[idx] + 0xC);
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF480);
#ifdef SKIP_ASM
extern "C" int func_002AF480(int idx)
{
    void* m = D_004D3E98[idx];
    if (m != 0) {
        return *(int*)((char*)m + 0x14);
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF4B0);
#ifdef SKIP_ASM
extern "C" int func_002AF4B0(int idx)
{
    void* m = D_004D3E98[idx];
    if (m != 0) {
        return *(int*)((char*)m + 0x10);
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF4E0);
#ifdef SKIP_ASM
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" int func_003B8050(void* h, int a, int b);
extern "C" int func_003B88C8(void* h, int a, int b);

extern "C" int func_002AF4E0(int idx, int a, int b)
{
    void* m = D_004D3E98[idx];
    int r = -8;
    if (m != 0) {
        func_003B58A0();
        if (*(signed char*)((char*)m + 0x64) != 0) {
            r = func_003B88C8(*(void**)m, a, b);
        } else {
            r = func_003B8050(*(void**)m, a, b);
        }
        func_003B58D8();
        *(int*)((char*)m + 0x10) = b;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF580);
#ifdef SKIP_ASM
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" int func_003B8010(void* h, int a);
extern "C" int func_003B8700(void* h, int a);

extern "C" int func_002AF580(int idx, int a)
{
    void* m = D_004D3E98[idx];
    int r = -8;
    if (m != 0) {
        func_003B58A0();
        if (*(signed char*)((char*)m + 0x64) != 0) {
            r = func_003B8700(*(void**)m, a);
        } else {
            r = func_003B8010(*(void**)m, a);
        }
        func_003B58D8();
        *(int*)((char*)m + 0x14) = a;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF608);
#ifdef SKIP_ASM
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" int func_003B8530(int h, int v, int c);
// PORT: the unit declares func_003B7EB0 as void; this caller uses its result.
int func_003B7EB0_r(int h, int v, int c) __asm__("func_003B7EB0");

extern "C" int func_002AF608(int idx, int c, int v, char busy)
{
    void* m = D_004D3E98[idx];
    int r = 0;
    if (busy == 0) {
        func_003B58A0();
        *(int*)((char*)m + 0x8) = c;
        if (*(signed char*)((char*)m + 0x64) != 0) {
            r = func_003B8530(*(int*)((char*)*(void**)((char*)m + 0x6C) + 0x4), v / 10, c);
        } else {
            r = func_003B7EB0_r(*(int*)m, v, c);
        }
        func_003B58D8();
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF6C0);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_002AEFA8(int a0);
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" void func_003B85F0(int h, int v);
extern "C" void func_003B7E70(int h, int v);

extern "C" int func_002AF6C0(int idx, int v)
{
    void* m = D_004D3E98[idx];
    int r = 0;
    if (*(int*)((char*)func_0028B180() + 0x6280) == 0) return 0;
    if (*(signed char*)((char*)m + 0x65) != 0) {
        func_002AEFA8(0);
    }
    int cur = *(int*)((char*)m + 0xC);
    if (*(int*)((char*)m + 0x8) < 0 || *(int*)((char*)m + 0x8) == cur) {
        *(int*)((char*)m + 0x8) = -1;
        if (cur != v && v >= 0 && cur >= 0) {
            func_003B58A0();
            if (*(signed char*)((char*)m + 0x64) != 0) {
                func_003B85F0(*(int*)((char*)*(void**)((char*)m + 0x6C) + 0x4), v);
            } else {
                func_003B7E70(*(int*)m, v);
            }
            func_003B58D8();
            *(int*)((char*)m + 0xC) = v;
        }
    } else {
        r = -8;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF7A0);
#ifdef SKIP_ASM
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" void func_003B7FB8(int h, int v);
extern "C" void func_003B8790(int h, int v);

extern "C" int func_002AF7A0(int idx, int reset)
{
    void* m = D_004D3E98[idx];
    unsigned short v;
    if (reset != 0) {
        v = 0;
        *(int*)((char*)m + 0x18) = *(unsigned short*)((char*)m + 0x3C);
    } else {
        v = *(int*)((char*)m + 0x18);
    }
    func_003B58A0();
    if (*(signed char*)((char*)m + 0x64) != 0) {
        func_003B8790(*(int*)((char*)*(void**)((char*)m + 0x6C) + 4), v);
    } else {
        func_003B7FB8(*(int*)m, v);
    }
    func_003B58D8();
    *(char*)((char*)m + 0x65) = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF838);
#ifdef SKIP_ASM
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" void func_003B78F8(int);
extern "C" void func_003B89E0(int);

extern "C" int func_002AF838(int idx)
{
    void* m = D_004D3E98[idx];
    func_003B58A0();
    if (*(signed char*)((char*)m + 0x64) != 0) {
        func_003B89E0(*(int*)((char*)*(void**)((char*)m + 0x6C) + 0x4));
    } else {
        func_003B78F8(*(int*)((char*)m + 0x0));
    }
    func_003B58D8();
    *(char*)((char*)m + 0x65) = 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF8A8);
#ifdef SKIP_ASM
extern "C" int func_002B0F48(void* self, int a1);
extern char* D_004A37B4;
extern int D_004A37BC;

// PORT: real signature is int(int); the unit later declares func_002AF8A8 as void() for its callback use.
extern "C" int func_002AF8A8_impl(int a) __asm__("func_002AF8A8");
extern "C" int func_002AF8A8_impl(int a)
{
    if (D_004A37BC != 0) {
        return -1;
    }
    return func_002B0F48(D_004A37B4, a);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF8E0__FPv);
#ifdef SKIP_ASM
void* func_002AF8E0(void* self)
{
    int t0 = 0;
    *(int*)((char*)self + 0x48) = t0;
    *(int*)self = -1;
    *(int*)((char*)self + 0x44) = t0;
    *(signed char*)((char*)self + 0x4) = (signed char)t0;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AF900);
#ifdef SKIP_ASM
extern "C" void func_003D6CA0(void* a0);

extern "C" void func_002AF900(int* self, int flags)
{
    func_003D6CA0(*(void**)self);
    if (*(int*)((char*)self + 0x48) != 0) {
        func_002523A8(*(void**)((char*)self + 0x44));
    }
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002AF960);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AFA58);
#ifdef SKIP_ASM
extern "C" int func_002AFA58(void* self, int a1)
{
    return a1 == *(int*)self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AFA68);
#ifdef SKIP_ASM
extern char* D_004A37B4;
extern "C" void func_002AA910(void* sys, int id, int a, int b, int c);
extern "C" void func_002B0088(void* self, int a, int b, int c);

struct sBmPair_FA68 {
    int a;
    int b;
};

struct sBmStream_FA68 {
    int f0;                     // 0x0
    int f4;                     // 0x4
    int f8;                     // 0x8
    char fC;                    // 0xC
    char pad_0xD[0xCC - 0xD];
    sBmPair_FA68 pairs[4];      // 0xCC
    int fEC;                    // 0xEC
    int fF0;                    // 0xF0
    char pad_0xF4[0x8C4 - 0xF4];
    int f8C4;                   // 0x8C4
    int f8C8;                   // 0x8C8
    int f8CC;                   // 0x8CC
    int f8D0;                   // 0x8D0
    char pad_0x8D4[0x8F4 - 0x8D4];
    int f8F4;                   // 0x8F4
    char pad_0x8F8[0x900 - 0x8F8];
    int f900;                   // 0x900
};

extern "C" void* func_002AFA68(void* mem, int a1, int a2, int a3, int a4, int a5, int a6)
{
    sBmStream_FA68* self = (sBmStream_FA68*)mem;
    self->f0 = a1;
    self->f4 = a2;
    self->f900 = 1;
    self->f8 = a6;
    self->fEC = 0;
    self->fF0 = 0;
    self->f8C4 = 0;
    self->f8C8 = 0;
    self->f8CC = 0;
    self->f8D0 = 0;
    self->f8F4 = 0;
    self->fC = 0;
    for (int i = 0; i < 4; i++) {
        self->pairs[i].a = 0;
        self->pairs[i].b = 0;
    }
    func_002AA910(*(void**)D_004A37B4, self->f4, a4, 0, 0x20000);
    func_002B0088(self, a1, a3, a5);
    return self;
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002AFB38);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AFC88);
#ifdef SKIP_ASM
extern "C" int func_002B0290(void* p);
extern "C" void* func_003D7EC8(void* p);
extern "C" void* func_003D8040(void* p);
extern char* D_004A37B4;

class func_002AFC88_cObj {
public:
    char data[0x1BC];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
};

// PORT: i is passed through as a pointer-sized handle
extern "C" void func_002AFC88(void* self, int i)
{
    if (func_002B0290(self) != 0) {
        if (*(int*)((char*)self + 0x900) != 0) {
            ((func_002AFC88_cObj*)D_004A37B4)->v04();
            if (func_003D7EC8((void*)i) != 0) {
                *(int*)((char*)self + 0x900) = 0;
            }
        } else {
            func_003D8040((void*)i);
            *(int*)((char*)self + 0x900) = 1;
        }
    } else {
        *(int*)((char*)self + 0x900) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AFD10);
#ifdef SKIP_ASM
extern "C" void* func_003D7EC8(void* p);

// PORT: real arity is (self, p); the unit later declares func_002AFD10(int)
void* func_002AFD10_impl(void* self, void* p) __asm__("func_002AFD10");

void* func_002AFD10_impl(void* self, void* p)
{
    void* r = func_003D7EC8(p);
    if (r != 0) {
        *(int*)((char*)self + 0x900) = 0;
    }
    return r;
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002AFD40);

INCLUDE_ASM("sound/bankmonitor", func_002B0088);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B0290);
#ifdef SKIP_ASM
extern "C" int func_002AAE08(void* self, int i);
extern "C" float func_002AAE78(void* self, int i);
extern "C" int func_002AB028(void* self, int i);
extern char* D_004A37B4;

extern "C" int func_002B0290(void* p)
{
    int state = func_002AAE08(*(void**)D_004A37B4, *(int*)((char*)p + 0x4));
    if (state == 0) {
        return 1;
    }
    if (state != 1) {
        return 0;
    }
    if (func_002AB028(*(void**)D_004A37B4, *(int*)((char*)p + 0x4)) == 0) {
        return 0;
    }
    int ok = (int)func_002AAE78(*(void**)D_004A37B4, *(int*)((char*)p + 0x4)) < 500;
    int r = 0;
    if (ok) {
        r = state;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B0320);
#ifdef SKIP_ASM
extern "C" void func_002AAE40(void* self, int i);
extern char* D_004A37B4;

extern "C" void func_002B0320(void* p)
{
    func_002AAE40(*(void**)D_004A37B4, *(int*)((char*)p + 0x4));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B0348);
#ifdef SKIP_ASM
extern "C" int func_002AF960(int a, int b, int c);
extern int D_004A37B8;

struct sBankList_0348 {
    char pad[0xF0];
    int count;     // 0xF0
    int ids[1];    // 0xF4
};

extern "C" void func_002B0348(sBankList_0348* self, int a, int b, int c)
{
    self->ids[self->count] = func_002AF960(a, b, c);
    if (self->ids[self->count] != 0) {
        self->count++;
        D_004A37B8++;
    }
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002B03B8);

INCLUDE_ASM("sound/bankmonitor", func_002B04D8);

INCLUDE_ASM("sound/bankmonitor", func_002B07F8);

INCLUDE_ASM("sound/bankmonitor", func_002B0AE8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B0C78);
#ifdef SKIP_ASM
struct sBankSlots;
extern "C" void func_002AFC88(void*, int);
extern "C" void func_002B1720(sBankSlots*);

extern "C" void func_002B0C78(void* self)
{
    for (int i = 0; i < 2; i++) {
        void* p = (*(void***)((char*)self + 0x4))[i];
        if (p != 0) {
            func_002AFC88(p, i);
        }
    }
    func_002B1720((sBankSlots*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B0CE0);
#ifdef SKIP_ASM
extern "C" int func_003D6778(int a, int b, int c);
// PORT: the unit defines func_002ADF60() with no args; this caller passes one.
int func_002ADF60_a(int seed) __asm__("func_002ADF60");
extern "C" void bxLogPrint(const char* fmt, ...);
extern "C" void func_003D6840(void* fn, int seed, int buf);
extern "C" int func_003D6C58(int n);
extern "C" void* func_00252FA0(void* a0, int a1, int a2);
extern "C" void func_003D6C60(int n, void* p);
extern "C" void* func_003D6768();
extern "C" void func_002AF8A8();
extern "C" int func_003E4E98();
extern char D_00483390[];
extern char D_004833A8[];

extern "C" void func_002B0CE0(void* self)
{
    int buf = func_003D6778(0x5622, 0x10, 1);
    int seed = func_002ADF60_a(*(int*)(**(char***)self + 0x1D8));
    bxLogPrint(D_00483390, seed);
    func_003D6840((void*)func_002AF8A8, seed, buf);
    int n = func_003D6C58(500);
    void* p = func_00252FA0(D_004833A8, n, 0);
    *(void**)((char*)self + 0x8) = p;
    func_003D6C60(500, p);
    *(void**)((char*)func_003D6768() + 0x8) = (void*)func_003E4E98;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B0DA0);
#ifdef SKIP_ASM
extern "C" void func_002AFB38(void* p, int mode);
extern "C" void func_003D67F8(void);

extern "C" void func_002B0DA0(void* self)
{
    for (int i = 0; i < 2; i++) {
        void* p = (*(void***)((char*)self + 0x4))[i];
        if (p != 0) {
            func_002AFB38(p, 3);
        }
        (*(void***)((char*)self + 0x4))[i] = 0;
    }
    func_003D67F8();
    func_002523A8(*(void**)((char*)self + 0x8));
    *(void**)((char*)self + 0x8) = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B0E28);
#ifdef SKIP_ASM
extern "C" int func_002B0290(void* p);

extern "C" int func_002B0E28(void* self, int i)
{
    void* p = (*(void***)((char*)self + 0x4))[i];
    if (p != 0) {
        return func_002B0290(p);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B0E60);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002AFA68(void* mem, int i, int a, int b, int c, int d, int e);
extern const char D_004833B8[];

extern "C" void func_002B0E60(void* self, int i, int a, int b, int c, int d, int e)
{
    // PORT: pointer arithmetic through int
    void** slot = (void**)(i * 4 + *(int*)((char*)self + 0x4));
    *slot = func_002AFA68(cMemMan_alloc(0x904, D_004833B8, 0, 0), i, a, b, c, d, e);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B0F48);
#ifdef SKIP_ASM
extern "C" int func_002B04D8(void* p, int a1);

extern "C" int func_002B0F48(void* self, int a1)
{
    for (int i = 0; i < 2; i++) {
        void* p = (*(void***)((char*)self + 0x4))[i];
        if (p != 0) {
            int r = func_002B04D8(p, a1);
            if (r >= 0) {
                if (*(int*)((char*)self + 0x10) == i) {
                    (*(void (**)(int))((char*)self + 0xC))(r);
                }
                return r;
            }
        }
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B11B0);
#ifdef SKIP_ASM
extern "C" void func_002B0320(void* p);

extern "C" void func_002B11B0(void* self, int i)
{
    void* p = (*(void***)((char*)self + 0x4))[i];
    if (p != 0) {
        func_002B0320(p);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1220);
#ifdef SKIP_ASM
extern "C" int func_002B1220(void* self, int i)
{
    void* e = (*(void***)((char*)self + 0x4))[i];
    if (e != 0) {
        return *(int*)((char*)e + 0x4);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1428);
#ifdef SKIP_ASM
struct sBankSlot0018 {
    int value;
    int timer;
    char pad[0x18];
};

struct sBankSlots {
    char pad[0x18];
    sBankSlot0018 slots[10];
};

extern "C" void func_002B1428(sBankSlots* self)
{
    for (int i = 9; i >= 0; i--) {
        self->slots[i].value = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1458);
#ifdef SKIP_ASM
struct sBankEntry20 {
    int used;
    int timer;
    int a;
    int b;
    int c;
    int d;
    float f;
    int e;
};

struct sBankEntries {
    char pad[0x18];
    sBankEntry20 entries[10];
};

extern "C" int func_002B1458(sBankEntries* self, int a, int b, int c, int d, int e, int refresh, float f)
{
    int freeIdx = -1;
    int i;
    for (i = 0; i < 10; i++) {
        if (self->entries[i].used != 0) {
            if (self->entries[i].a == a && self->entries[i].b == b) {
                if (refresh == 0) {
                    return 0;
                }
                self->entries[i].timer = 180;
                return 1;
            }
        } else {
            if (freeIdx == -1) {
                freeIdx = i;
            }
        }
    }
    if (freeIdx == -1) {
        return 0;
    }
    self->entries[freeIdx].used = 1;
    self->entries[freeIdx].timer = 180;
    self->entries[freeIdx].a = a;
    self->entries[freeIdx].b = b;
    self->entries[freeIdx].c = c;
    self->entries[freeIdx].d = d;
    self->entries[freeIdx].f = f;
    self->entries[freeIdx].e = e;
    return 1;
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002B1520);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1720);
#ifdef SKIP_ASM
extern "C" void func_002B1720(sBankSlots* self)
{
    for (int i = 0; i < 10; i++) {
        if (self->slots[i].value != 0) {
            if (--self->slots[i].timer <= 0) {
                self->slots[i].value = 0;
            }
        }
    }
}
#endif

extern "C" void* func_002AFD10(int);

//96.5%
INCLUDE_ASM("sound/bankmonitor", func_002B1758__FPvi);
#ifdef SKIP_ASM
void* func_002B1758(void* self, int a1)
{
    return func_002AFD10(*(int*)((char*)*(void**)((char*)self + 0x4) + a1 * 4));
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1A98);
#ifdef SKIP_ASM
extern char D_00483980[];
extern unsigned char D_004A35A8[];

struct func_002B1A98_sElem20 {
    unsigned char b;
    char pad[0x1F];
};

extern "C" void* func_002B1A98(void* self)
{
    *(void**)((char*)self + 0xA8) = D_00483980;
    *(int*)((char*)self + 0x40) = 0;
    *(int*)((char*)self + 0x64) = 4;
    *(int*)((char*)self + 0x68) = 2;
    *(int*)((char*)self + 0x6C) = 4;
    *(int*)((char*)self + 0x70) = 8;
    *(int*)((char*)self + 0x74) = 0x10;
    *(int*)((char*)self + 0x78) = 0;
    *(int*)((char*)self + 0x7C) = 0x5A;
    *(int*)((char*)self + 0x80) = 100;
    *(int*)((char*)self + 0x84) = 0x32;
    *(int*)((char*)self + 0x88) = 100;
    *(int*)((char*)self + 0x8C) = 100;
    *(float*)((char*)self + 0x90) = 120.0f;
    *(int*)((char*)self + 0x94) = 1;
    *(int*)((char*)self + 0x98) = -1;
    *(int*)((char*)self + 0x9C) = 0xFFFF;
    *(int*)((char*)self + 0xA0) = -1;
    *(int*)((char*)self + 0xA4) = 1;
    *(unsigned char*)((char*)self + 0x0) = D_004A35A8[0];
    *(unsigned char*)((char*)self + 0x20) = D_004A35A8[0];
    func_002B1A98_sElem20* p = (func_002B1A98_sElem20*)((char*)self + 0x44);
    for (int i = 0; i >= 0; i--, p++) {
        p->b = D_004A35A8[0];
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1B68);
#ifdef SKIP_ASM
extern char D_00483980[];

extern "C" void func_002B1B68(int* self, int flags)
{
    *(void**)((char*)self + 0xA8) = D_00483980;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1B98);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002B1C50(void* mem, int a0, int a1, int a2, int a3, char a4, int a5, int a6, int a7);
extern const char D_004833E0[];

extern "C" void* func_002B1B98(int a0, int a1, int a2, int a3, char a4, int a5, int a6, int a7)
{
    return func_002B1C50(cMemMan_alloc(0xC4, D_004833E0, 0, 0), a0, a1, a2, a3, a4, a5, a6, a7);
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002B1C50);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B1F78);
#ifdef SKIP_ASM
extern "C" void func_0028A728(void* self, int i);
extern "C" int func_003D2A68(int id);
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern void*** D_004A37EC;

extern "C" void func_002B1F78(void* self, int flags)
{
    func_0028A728(**D_004A37EC, *(int*)((char*)self + 0x34));
    func_003D2A68(0x11000001);
    if (*(int*)(*(char**)self + 0x40) > 0) {
        func_003D2A68(0x11000002);
    }
    if (*(void**)((char*)self + 0x24) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x24));
    }
    if (*(void**)((char*)self + 0x28) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x28));
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2018);
#ifdef SKIP_ASM
extern "C" int func_002B2488(void* self);
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" void func_003B7FB8(int h, int v);
extern "C" void func_003D1038(int cmd, int v);

extern "C" void func_002B2018(void* self)
{
    func_003B58A0();
    int h = func_002B2488(self);
    if (h >= 0) {
        func_003B7FB8(h, 0);
    }
    func_003B58D8();
    func_003D1038(0x11000002, 1);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2070);
#ifdef SKIP_ASM
extern "C" int func_002B2488(void* self);
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" void func_003B7FB8(int h, int v);
extern "C" void func_003D1038(int cmd, int v);

extern "C" void func_002B2070(void* self)
{
    func_003B58A0();
    int h = func_002B2488(self);
    if (h >= 0) {
        func_003B7FB8(h, 0x1000);
    }
    func_003B58D8();
    func_003D1038(0x11000002, 0);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B20C8);
#ifdef SKIP_ASM
extern "C" void func_003D16F0(int id, short v);

extern "C" void func_002B20C8(void* self, int v, int force)
{
    if (*(int*)((char*)self + 0x5C) != 0 && force == 0) {
        return;
    }
    if (*(int*)((char*)self + 0x2C) != 0) {
        func_003D16F0(-1, v);
    } else if (*(int*)((char*)self + 0x30) == -1) {
        *(int*)((char*)self + 0x30) = v;
    }
}
#endif

extern "C" void func_003D0D70(unsigned int);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2120);
#ifdef SKIP_ASM
extern "C" void func_002B2120()
{
    func_003D0D70(0x11000001U);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2160);
#ifdef SKIP_ASM
extern "C" int func_002B2488(void* self);
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" void func_003B80A0(int h, int v);

extern "C" void func_002B2160(void* self, int v)
{
    if (v > 0xFFFF) {
        v = 0xFFFF;
    }
    if (v < 0) {
        v = 0;
    }
    if (v != *(int*)((char*)self + 0x44)) {
        func_003B58A0();
        int h = func_002B2488(self);
        if (h >= 0) {
            *(int*)((char*)self + 0x44) = v;
            func_003B80A0(h, v);
        }
        func_003B58D8();
    }
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002B21E0);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B23E8);
#ifdef SKIP_ASM
extern "C" void func_003D5508(int cmd, int v);

extern "C" void func_002B23E8(void* self, signed char v)
{
    if (*(int*)((char*)self + 0x54) != 0) {
        func_003D5508(0x11000002, v);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2418);
#ifdef SKIP_ASM
extern "C" int func_002B2488(void* self);
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" void func_003B7EB0(int h, int v, int c);

extern "C" void func_002B2418(void* self, float t)
{
    func_003B58A0();
    int h = func_002B2488(self);
    if (h >= 0) {
        func_003B7EB0(h, (int)(t * 1000.0f), 0);
    }
    func_003B58D8();
    *(int*)((char*)self + 0x48) = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B2488);
#ifdef SKIP_ASM
extern "C" int func_003D3030(int id);

extern "C" int func_002B2488(void* self)
{
    int idx = func_003D3030(0x11000001);
    if (idx < 0) {
        return -1;
    }
    // PORT: func_002ADFB0 returns a pointer held in int
    return *(int*)func_002ADFB0(idx);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B24C0);
#ifdef SKIP_ASM
// PORT: func_002ADF60 is defined () but this caller passes the monitor.
unsigned int func_002ADF60_p(void* p) __asm__("func_002ADF60");
extern void* D_004A377C;

extern "C" int func_002B24C0(void* p)
{
    return func_002ADF60_p(D_004A377C) % *(unsigned int*)(*(char**)p + 0x40);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2578);
#ifdef SKIP_ASM
extern "C" int func_002B2488(void* self);
extern "C" int func_003B7B48(int h, void* out);

// PORT: returns int; a later caller in this unit declares it void(void*, void*)
int func_002B2578_impl(void* self, void* out) __asm__("func_002B2578");

int func_002B2578_impl(void* self, void* out)
{
    return func_002B2488(self) < 0 ? -1 : func_003B7B48(func_002B2488(self), out);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B25C8);
#ifdef SKIP_ASM
extern "C" void func_002B2578(void* self, void* out);
extern "C" void func_003B7C40(int h, int v);

struct sBmInfo {
    int a;
    int handle;
    int c;
    int d;
};

extern "C" void func_002B25C8(void* self, int v)
{
    sBmInfo info;
    func_002B2578(self, &info);
    func_003B7C40(info.handle, v);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B2600);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit declares func_002B25C8(void*, int); its second arg is really an out pointer.
void func_002B25C8_out(void* self, void* out) __asm__("func_002B25C8");

struct func_002B2600_sInfo {
    int type;
    int value;
    int c;
    int d;
};

extern "C" float func_002B2600(void* self)
{
    func_002B2600_sInfo info;
    func_002B25C8_out(self, &info);
    if (info.type == 2) {
        return (float)info.value;
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2690);
#ifdef SKIP_ASM
extern "C" float func_002B2690(void* self)
{
    void* p = *(void**)self;
    return (60.0f / *(float*)((char*)p + 0x90)) * 1000.0f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B2718);
#ifdef SKIP_ASM
extern "C" float func_002B2600(void* self);
extern "C" float func_0040D8D0(float x, float y);

extern "C" float func_002B2718(void* self)
{
    float a = func_002B2690(self);
    float b = func_002B2600(self);
    return a - func_0040D8D0(b, func_002B2690(self));
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2850);
#ifdef SKIP_ASM
struct sBMVec16 {
    int x;
    int y;
    int z;
    int w;
};

struct sBMTrack {
    char pad0[0x1F4];
    int cur;
    char pad1F8[0x248 - 0x1F8];
    sBMVec16* vecs;
};

struct sBMSrc {
    char pad0[0x78];
    int a;
    int b;
    int c;
};

extern "C" void func_002B2850(sBMSrc** src, sBMTrack*** dst)
{
    (**dst)->vecs[(**dst)->cur].y = (*src)->a;
    (**dst)->vecs[(**dst)->cur].z = (*src)->b;
    (**dst)->vecs[(**dst)->cur].x = (*src)->c;
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002B28C0);

INCLUDE_ASM("sound/bankmonitor", func_002B3398);

INCLUDE_ASM("sound/bankmonitor", func_002B35A0);

INCLUDE_ASM("sound/bankmonitor", func_002B3838);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3A70);
#ifdef SKIP_ASM
extern "C" void func_002B2018(void*);

extern "C" void func_002B3A70(void* self)
{
    void* p = *(void**)((char*)self + 0x408);
    if (p != 0) {
        func_002B2018(p);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3A98);
#ifdef SKIP_ASM
extern "C" void func_002B2070(void*);

extern "C" void func_002B3A98(void* self)
{
    void* p = *(void**)((char*)self + 0x408);
    if (p != 0) {
        func_002B2070(p);
    }
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002B3AC0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3B88);
#ifdef SKIP_ASM
extern "C" void func_002B20C8(void* mon, int a1, int a2);

extern "C" void func_002B3B88(void* self, int a1)
{
    if (*(int*)((char*)self + 0x418) != 0) {
        void* m = *(void**)((char*)self + 0x408);
        if (m != 0) {
            func_002B20C8(m, a1, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3BC0);
#ifdef SKIP_ASM
struct sBankMonVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" int func_002B3BC0(void* self, int v)
{
    int r = 0;
    if (*(int*)((char*)self + 0x418) != 0 && v != *(int*)((char*)self + 0x41C)) {
        sBankMonVEntry* vt = *(sBankMonVEntry**)((char*)self + 0x5440);
        vt[3].fn((char*)self + vt[3].delta);
        *(int*)((char*)self + 0x41C) = v;
        r = 1;
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3C28);
#ifdef SKIP_ASM
// PORT: called with the monitor as `this`; the unit declares func_002B2120() with no args
void func_002B2120_m(void* mon) __asm__("func_002B2120");

extern "C" void func_002B3C28(void* self)
{
    if (*(int*)((char*)self + 0x418) != 0) {
        void* m = *(void**)((char*)self + 0x408);
        if (m != 0) {
            func_002B2120_m(m);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3C98);
#ifdef SKIP_ASM
extern "C" void func_002B3C98(void* self, signed char a1)
{
    if (*(int*)((char*)self + 0x418) != 0) {
        void* p = *(void**)((char*)self + 0x408);
        if (p != 0) {
            *(signed char*)((char*)p + 0x40) = a1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3CC0);
#ifdef SKIP_ASM
extern "C" void func_002B3CC0(void* self, int a1)
{
    void* p = *(void**)((char*)self + 0x408);
    if (p != 0) {
        *(int*)((char*)p + 0x54) = a1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3D10);
#ifdef SKIP_ASM
extern "C" void func_002B23E8(void* self, signed char v);

extern "C" void func_002B3D10(void* self, signed char v)
{
    if (*(int*)((char*)self + 0x418) != 0) {
        void* m = *(void**)((char*)self + 0x408);
        if (m != 0) {
            func_002B23E8(m, v);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B3D48);
#ifdef SKIP_ASM
extern "C" void func_002B2418(void* mon, float v);

extern "C" void func_002B3D48(void* self, float v)
{
    if (*(int*)((char*)self + 0x418) != 0) {
        void* m = *(void**)((char*)self + 0x408);
        if (m != 0) {
            func_002B2418(m, v);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3EB0);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_002B3EB0(void* self)
{
    *(ulong*)((char*)self + 0x3f8) = 0;
    *(int*)((char*)self + 0x3e8) = 0;
    for (int i = 4; i >= 0; i--) {
        ((int*)((char*)self + 0x920))[i] = 0;
    }
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002B3EE8);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B3FE8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sBankMonUse {
    char pad0[0x3E8];
    int count;                  // 0x3E8
    char pad3EC[0x3F8 - 0x3EC];
    ulong mask;                 // 0x3F8
    char pad400[0x420 - 0x400];
    int grid[64][5];            // 0x420
    int totals[5];              // 0x920
};

extern "C" void func_002B3FE8(sBankMonUse* self, int idx)
{
    if (self->count < 64) {
        self->count++;
        self->mask |= (ulong)1 << idx;
        for (int i = 0; i < 5; i++) {
            if (self->grid[idx][i] != 0) {
                self->totals[i]++;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4060__FPv);
#ifdef SKIP_ASM
void func_002B4060(void* self)
{
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B4070);
#ifdef SKIP_ASM
// PORT: 64-bit `long` mask (8 bytes on EE); use uint64_t off-PS2.
struct sBankMonUse;
extern "C" void func_002B3EB0(void* self);
extern "C" void func_002B3FE8(sBankMonUse* self, int idx);
void func_002B4060(void* self);

extern "C" void func_002B4070(void* self, ulong mask)
{
    func_002B3EB0(self);
    for (int i = 0; i < *(int*)((char*)self + 0x3EC); i++) {
        int bit = (mask >> i) & 1;
        if (bit) {
            func_002B3FE8((sBankMonUse*)self, i);
        }
    }
    func_002B4060(self);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B40F0);
#ifdef SKIP_ASM
struct sBankEntry64 {
    char data[0x64];
};

struct sBankMonitor934 {
    char pad[0x3f0];
    int current;                // 0x3F0
    char pad2[0x934 - 0x3f4];
    sBankEntry64 a[64];         // 0x934
    sBankEntry64 b[64];         // 0x2234
    sBankEntry64 c[64];         // 0x3B34
};

extern "C" sBankEntry64* func_002B40F0(sBankMonitor934* self, int i)
{
    if (i < 0) {
        i = self->current;
        if (i < 0) {
            return 0;
        }
    }
    return &self->a[i];
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4120);
#ifdef SKIP_ASM
extern "C" sBankEntry64* func_002B4120(sBankMonitor934* self, int i)
{
    if (i < 0) {
        i = self->current;
        if (i < 0) {
            return 0;
        }
    }
    return &self->b[i];
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4150);
#ifdef SKIP_ASM
extern "C" sBankEntry64* func_002B4150(sBankMonitor934* self, int i)
{
    if (i < 0) {
        i = self->current;
        if (i < 0) {
            return 0;
        }
    }
    return &self->c[i];
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B42B8);
#ifdef SKIP_ASM
struct func_002B42B8_sMon {
    char pad[0x3E4];
    int count;              // 0x3E4
    char pad3E8[0x3F0 - 0x3E8];
    int current;            // 0x3F0
    char pad3F4[0x3F8 - 0x3F4];
    unsigned long mask;     // 0x3F8 (PORT: 64-bit long)
    char pad400[0x420 - 0x400];
    int table[64][5];       // 0x420
    int counts[5];          // 0x920
};

extern "C" void func_002B42B8(func_002B42B8_sMon* self, int filter, int kind)
{
    if (filter == 0 || self->counts[kind] == 0) {
        do {
            if (++self->current >= self->count) {
                self->current = 0;
            }
        } while (!((self->mask >> self->current) & 1));
    } else {
        do {
            if (++self->current >= self->count) {
                self->current = 0;
            }
        } while (!((self->mask >> self->current) & 1) || self->table[self->current][kind] == 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4388);
#ifdef SKIP_ASM
extern "C" void func_002B21E0(void*, int);
extern "C" void func_002B4620(void*);

extern "C" void func_002B4388(void* self)
{
    void* p = *(void**)((char*)self + 0x408);
    if (p != 0) {
        func_002B21E0(p, 0);
        if (*(int*)((char*)*(void**)((char*)self + 0x408) + 0x50) != 0) {
            func_002B4620(self);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B43D8);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int func_002B24C0(void* p);
extern "C" void func_0028BD10(void* self, int i, void* id, int a3);
extern char D_004A36E0[];

struct sBankMonHdr { char pad[0x40]; int f40; char names[1][0x20]; };

extern "C" void func_002B43D8(void* self)
{
    char buf[256];
    void* p = *(void**)((char*)self + 0x408);
    if (p != 0) {
        int bank = *(int*)((char*)p + 0x34);
        if ((*(sBankMonHdr**)p)->f40 != 0) {
            int n = func_002B24C0(p);
            void* q = *(void**)((char*)self + 0x408);
            sprintf(buf, D_004A36E0, (char*)q + 0x60, (*(sBankMonHdr**)q)->names[n]);
            func_0028BD10(**(void***)self, bank, buf, 0x100);
            *(int*)((char*)*(void**)((char*)self + 0x408) + 0x38) = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4620);
#ifdef SKIP_ASM
extern "C" void func_003D2DF8(int cmd, int a, int b);

struct sBmStream4620 {
    int state;      // 0x0
    int handle;     // 0x4
    char pad[0x60 - 8];
};

static inline int func_002B4620_ready(sBmStream4620* e)
{
    return e->state == 1;
}

// PORT: the unit declares func_002B4620 as void; it really returns 0/1.
int func_002B4620_impl(void* self) __asm__("func_002B4620");

int func_002B4620_impl(void* self)
{
    int one = 1;
    if (*(char**)((char*)self + 0x408) == 0) return 0;
    *(int*)(*(char**)((char*)self + 0x408) + 0x4C) = one;
    int r = 0;
    char* m = *(char**)((char*)self + 0x408);
    int st = *(int*)(m + 0x38);
    if (st == one) {
        // PORT: pointer arithmetic through int
        sBmStream4620* e = (sBmStream4620*)(*(int*)(m + 0x34) * 0x60 + *(int*)(**(char***)self + 0xACC));
        if (func_002B4620_ready(e)) {
            func_003D2DF8(0x11000002, 0, e->handle);
            *(int*)(*(char**)((char*)self + 0x408) + 0x38) = 2;
            r = 1;
        }
    } else if (st == 2) {
        r = one;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B46D0);
#ifdef SKIP_ASM
extern "C" void func_002B46D0(void* self)
{
    void* p = *(void**)((char*)self + 0x408);
    if (p != 0) {
        *(int*)((char*)p + 0x50) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B46E8);
#ifdef SKIP_ASM
extern "C" void* func_002B46E8(void* self)
{
    void* inner = *(void**)((char*)self + 0x408);
    if (inner == 0) {
        return 0;
    }
    return *(void**)((char*)inner + 0x4C);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4708);
#ifdef SKIP_ASM
extern "C" void func_002B4708(void* self)
{
    void* p = *(void**)((char*)self + 0x408);
    if (p != 0) {
        *(int*)((char*)p + 0x4c) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4868__FPv);
#ifdef SKIP_ASM
int func_002B4868(void* self)
{
    return *(int*)((char*)*(void**)self + 0x94);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B4878);
#ifdef SKIP_ASM
extern "C" int func_002B4878(void* self)
{
    if (*(int*)((char*)self + 0x418) != 0) {
        void* m = *(void**)((char*)self + 0x408);
        if (m != 0) {
            return func_002B4868(m);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4908__FPv);
#ifdef SKIP_ASM
int func_002B4908(void* self)
{
    void* p = *(void**)((char*)self + (*(int*)((char*)self + 0x3f0) << 2) + 0x4);
    return *(int*)((char*)p + 0x98);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmonitor", func_002B4920);
#ifdef SKIP_ASM
extern "C" void func_003B58A0(void);
extern "C" void func_003B58D8(void);
extern "C" float func_002B2718(void* self);

extern "C" float func_002B4920(void* self)
{
    if (*(int*)((char*)self + 0x418) == 0) {
        return 0.0f;
    }
    func_003B58A0();
    float r = func_002B2718(*(void**)((char*)self + 0x408));
    func_003B58D8();
    return r;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4978);
#ifdef SKIP_ASM
class func_002B4978_cObj {
public:
    int unk0;
    void* entries[(0x5440 - 4) / 4];
    // vptr at 0x5440; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03(int a);
};

extern "C" void func_002B4978(func_002B4978_cObj* self, int i)
{
    int v = *(int*)((char*)self->entries[i] + 0xA0);
    if (v >= 0) {
        self->v03(v);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B49C0);
#ifdef SKIP_ASM
extern "C" void func_002B49C0(void* self, int a1)
{
    if (*(int*)((char*)self + 0x418) != 0) {
        void* p = *(void**)((char*)self + 0x408);
        if (p != 0) {
            *(int*)((char*)p + 0x58) = a1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B49E0);
#ifdef SKIP_ASM
extern "C" int func_002B49E0(void* self)
{
    if (*(int*)((char*)self + 0x418) != 0) {
        void* p = *(void**)((char*)self + 0x408);
        if (p != 0) {
            return *(int*)((char*)p + 0x58);
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4A08);
#ifdef SKIP_ASM
extern "C" void func_002B4A08(void* self, int a1)
{
    void* p = *(void**)((char*)self + 0x408);
    if (p != 0) {
        *(int*)((char*)p + 0x5c) = a1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4A20);
#ifdef SKIP_ASM
extern "C" void func_003DFAF0(void* f);
extern "C" void ASYNCFILE_release(void* f, int a, int b);
extern "C" void* func_003DF748(void* name, void* buf, int size);
extern "C" void func_003E1B68(void* name, void* buf, int size);

extern "C" void func_002B4A20(void* self, void* name, int sync)
{
    if (*(void**)((char*)self + 0x5438) != 0) {
        func_003DFAF0(*(void**)((char*)self + 0x5438));
        ASYNCFILE_release(*(void**)((char*)self + 0x5438), 0, 0);
    }
    if (sync == 0) {
        *(void**)((char*)self + 0x5438) = func_003DF748(name, *(void**)((char*)self + 0x5434), 0x6400);
    } else {
        func_003E1B68(name, *(void**)((char*)self + 0x5434), 0x6400);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4AA0);
#ifdef SKIP_ASM
extern "C" int func_003DF980(void*);

extern "C" int func_002B4AA0(void* self, int force)
{
    void* p;
    if (force != 0) {
        return 1;
    }
    p = *(void**)((char*)self + 0x5438);
    if (p != 0 && func_003DF980(p) == 1) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4AE8__FPv);
#ifdef SKIP_ASM
int func_002B4AE8(void* self)
{
    return *(int*)((char*)self + 0x5434);
}
#endif

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B4AF0);
#ifdef SKIP_ASM
// PORT: needs a 64-bit return (ulong); use uint64_t off-PS2.
extern "C" ulong func_002B4AF0(void* self)
{
    return (*(ulong*)((char*)self + 0x3f8) >> *(int*)((char*)self + 0x3f0)) & 1;
}
#endif

