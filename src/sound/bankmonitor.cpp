#include "common.h"

INCLUDE_ASM("sound/bankmonitor", cBankMonitor_cBankMonitor);

INCLUDE_ASM("sound/bankmonitor", func_002ACE40);

INCLUDE_ASM("sound/bankmonitor", func_002ACF60);

INCLUDE_ASM("sound/bankmonitor", cBankMonitor_BANKMONITOR_Create);

INCLUDE_ASM("sound/bankmonitor", func_002AD2A8);

INCLUDE_ASM("sound/bankmonitor", func_002AD300);

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

INCLUDE_ASM("sound/bankmonitor", func_002AD410);

INCLUDE_ASM("sound/bankmonitor", func_002AD4D8);

INCLUDE_ASM("sound/bankmonitor", func_002AD550);

INCLUDE_ASM("sound/bankmonitor", func_002AD5F0);

INCLUDE_ASM("sound/bankmonitor", func_002AD650);

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

INCLUDE_ASM("sound/bankmonitor", func_002AD888);

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

INCLUDE_ASM("sound/bankmonitor", func_002AD998);

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

INCLUDE_ASM("sound/bankmonitor", func_002ADC30);

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

INCLUDE_ASM("sound/bankmonitor", func_002ADE88);

INCLUDE_ASM("sound/bankmonitor", func_002ADEE8);

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

INCLUDE_ASM("sound/bankmonitor", func_002AE020);

INCLUDE_ASM("sound/bankmonitor", func_002AE048);

INCLUDE_ASM("sound/bankmonitor", func_002AE100);

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

INCLUDE_ASM("sound/bankmonitor", func_002AE298);

INCLUDE_ASM("sound/bankmonitor", func_002AE328);

INCLUDE_ASM("sound/bankmonitor", func_002AE3A8);

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

INCLUDE_ASM("sound/bankmonitor", func_002AE690);

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

INCLUDE_ASM("sound/bankmonitor", func_002AEAA8);

INCLUDE_ASM("sound/bankmonitor", func_002AEBD0);

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

INCLUDE_ASM("sound/bankmonitor", func_002AF2F0);

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

INCLUDE_ASM("sound/bankmonitor", func_002AF428);

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

INCLUDE_ASM("sound/bankmonitor", func_002AF4E0);

INCLUDE_ASM("sound/bankmonitor", func_002AF580);

INCLUDE_ASM("sound/bankmonitor", func_002AF608);

INCLUDE_ASM("sound/bankmonitor", func_002AF6C0);

INCLUDE_ASM("sound/bankmonitor", func_002AF7A0);

INCLUDE_ASM("sound/bankmonitor", func_002AF838);

INCLUDE_ASM("sound/bankmonitor", func_002AF8A8);

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

INCLUDE_ASM("sound/bankmonitor", func_002AF900);

INCLUDE_ASM("sound/bankmonitor", func_002AF960);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002AFA58);
#ifdef SKIP_ASM
extern "C" int func_002AFA58(void* self, int a1)
{
    return a1 == *(int*)self;
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002AFA68);

INCLUDE_ASM("sound/bankmonitor", func_002AFB38);

INCLUDE_ASM("sound/bankmonitor", func_002AFC88);

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

INCLUDE_ASM("sound/bankmonitor", func_002B0290);

INCLUDE_ASM("sound/bankmonitor", func_002B0320);

INCLUDE_ASM("sound/bankmonitor", func_002B0348);

INCLUDE_ASM("sound/bankmonitor", func_002B03B8);

INCLUDE_ASM("sound/bankmonitor", func_002B04D8);

INCLUDE_ASM("sound/bankmonitor", func_002B07F8);

INCLUDE_ASM("sound/bankmonitor", func_002B0AE8);

INCLUDE_ASM("sound/bankmonitor", func_002B0C78);

INCLUDE_ASM("sound/bankmonitor", func_002B0CE0);

INCLUDE_ASM("sound/bankmonitor", func_002B0DA0);

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

INCLUDE_ASM("sound/bankmonitor", func_002B0E60);

INCLUDE_ASM("sound/bankmonitor", func_002B0F48);

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

INCLUDE_ASM("sound/bankmonitor", func_002B1B98);

INCLUDE_ASM("sound/bankmonitor", func_002B1C50);

INCLUDE_ASM("sound/bankmonitor", func_002B1F78);

INCLUDE_ASM("sound/bankmonitor", func_002B2018);

INCLUDE_ASM("sound/bankmonitor", func_002B2070);

INCLUDE_ASM("sound/bankmonitor", func_002B20C8);

extern "C" void func_003D0D70(unsigned int);

//100%
INCLUDE_ASM("sound/bankmonitor", func_002B2120);
#ifdef SKIP_ASM
extern "C" void func_002B2120()
{
    func_003D0D70(0x11000001U);
}
#endif

INCLUDE_ASM("sound/bankmonitor", func_002B2160);

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

INCLUDE_ASM("sound/bankmonitor", func_002B2418);

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

INCLUDE_ASM("sound/bankmonitor", func_002B24C0);

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

INCLUDE_ASM("sound/bankmonitor", func_002B2718);

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

INCLUDE_ASM("sound/bankmonitor", func_002B3BC0);

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

INCLUDE_ASM("sound/bankmonitor", func_002B4070);

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

INCLUDE_ASM("sound/bankmonitor", func_002B43D8);

INCLUDE_ASM("sound/bankmonitor", func_002B4620);

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

INCLUDE_ASM("sound/bankmonitor", func_002B4920);

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

INCLUDE_ASM("sound/bankmonitor", func_002B4A20);

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

