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

INCLUDE_ASM("sound/bankmonitor", func_002AD3C0);

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

INCLUDE_ASM("sound/bankmonitor", func_002AD940);

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

INCLUDE_ASM("sound/bankmonitor", func_002ADDA0);

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

INCLUDE_ASM("sound/bankmonitor", func_002ADF80);

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

INCLUDE_ASM("sound/bankmonitor", func_002AE138);

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

INCLUDE_ASM("sound/bankmonitor", func_002AE1F0);

INCLUDE_ASM("sound/bankmonitor", func_002AE230);

INCLUDE_ASM("sound/bankmonitor", func_002AE298);

INCLUDE_ASM("sound/bankmonitor", func_002AE328);

INCLUDE_ASM("sound/bankmonitor", func_002AE3A8);

INCLUDE_ASM("sound/bankmonitor", func_002AE478);

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

INCLUDE_ASM("sound/bankmonitor", func_002AE9F8);

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

INCLUDE_ASM("sound/bankmonitor", func_002AF370);

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

INCLUDE_ASM("sound/bankmonitor", func_002AFD10);

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

INCLUDE_ASM("sound/bankmonitor", func_002B0E28);

INCLUDE_ASM("sound/bankmonitor", func_002B0E60);

INCLUDE_ASM("sound/bankmonitor", func_002B0F48);

INCLUDE_ASM("sound/bankmonitor", func_002B11B0);

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

INCLUDE_ASM("sound/bankmonitor", func_002B1458);

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

INCLUDE_ASM("sound/bankmonitor", func_002B1A98);

INCLUDE_ASM("sound/bankmonitor", func_002B1B68);

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

INCLUDE_ASM("sound/bankmonitor", func_002B23E8);

INCLUDE_ASM("sound/bankmonitor", func_002B2418);

INCLUDE_ASM("sound/bankmonitor", func_002B2488);

INCLUDE_ASM("sound/bankmonitor", func_002B24C0);

INCLUDE_ASM("sound/bankmonitor", func_002B2578);

INCLUDE_ASM("sound/bankmonitor", func_002B25C8);

INCLUDE_ASM("sound/bankmonitor", func_002B2600);

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

INCLUDE_ASM("sound/bankmonitor", func_002B3B88);

INCLUDE_ASM("sound/bankmonitor", func_002B3BC0);

INCLUDE_ASM("sound/bankmonitor", func_002B3C28);

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

INCLUDE_ASM("sound/bankmonitor", func_002B3D10);

INCLUDE_ASM("sound/bankmonitor", func_002B3D48);

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

INCLUDE_ASM("sound/bankmonitor", func_002B42B8);

INCLUDE_ASM("sound/bankmonitor", func_002B4388);

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

INCLUDE_ASM("sound/bankmonitor", func_002B4878);

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

INCLUDE_ASM("sound/bankmonitor", func_002B4978);

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

INCLUDE_ASM("sound/bankmonitor", func_002B4AA0);

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

