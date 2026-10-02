#include "common.h"

INCLUDE_ASM("dirtysock/tagsunk", cDirtysock_tag_TagFieldSetUnk);

INCLUDE_ASM("dirtysock/tagsunk", cDirtysock_tag_TagFieldGetUnk);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00259390__FPv);
#ifdef SKIP_ASM
void func_00259390(void* self)
{
}
#endif

extern "C" void* func_0041AA88(void*, void*);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00259398__FPviT0T0);
#ifdef SKIP_ASM
void* func_00259398(void* self, int a1, void* a2, void* a3)
{
    return func_0041AA88((char*)a2 + 0x1c, (char*)a3 + 0x1c);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002593B8__FPviT0T0);
#ifdef SKIP_ASM
void* func_002593B8(void* self, int a1, void* a2, void* a3)
{
    return func_0041AA88((char*)a2 + 0x8, (char*)a3 + 0x8);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002594D0);

INCLUDE_ASM("dirtysock/tagsunk", func_00259628);

extern "C" void* func_003E8FD0(int);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002598A8__FPv);
#ifdef SKIP_ASM
void* func_002598A8(void* self)
{
    return func_003E8FD0(*(int*)((char*)self + 0x54));
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002598C8);

INCLUDE_ASM("dirtysock/tagsunk", func_002599B0);

INCLUDE_ASM("dirtysock/tagsunk", func_00259A60);

INCLUDE_ASM("dirtysock/tagsunk", func_00259B10);

INCLUDE_ASM("dirtysock/tagsunk", func_0025A1D0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025A650);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025A6F8);
#ifdef SKIP_ASM
extern "C" void func_003E6E70(void*);

extern "C" void func_0025A6F8(void* self)
{
    void* p = *(void**)((char*)self + 0xF4);
    if (p != 0) {
        func_003E6E70(p);
        *(void**)((char*)self + 0xF4) = 0;
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025A730);

INCLUDE_ASM("dirtysock/tagsunk", func_0025A778);

INCLUDE_ASM("dirtysock/tagsunk", func_0025A850);

INCLUDE_ASM("dirtysock/tagsunk", func_0025A8B0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025A948);

INCLUDE_ASM("dirtysock/tagsunk", func_0025AA98);

INCLUDE_ASM("dirtysock/tagsunk", func_0025AAE0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025AB40);

INCLUDE_ASM("dirtysock/tagsunk", func_0025AC50);

INCLUDE_ASM("dirtysock/tagsunk", func_0025B608);

INCLUDE_ASM("dirtysock/tagsunk", func_0025B650);

INCLUDE_ASM("dirtysock/tagsunk", func_0025B688);

INCLUDE_ASM("dirtysock/tagsunk", func_0025B6D8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025B800);

INCLUDE_ASM("dirtysock/tagsunk", func_0025B848);

INCLUDE_ASM("dirtysock/tagsunk", func_0025B9C8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025BC48);

INCLUDE_ASM("dirtysock/tagsunk", func_0025BD30);

INCLUDE_ASM("dirtysock/tagsunk", func_0025BF18);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C0C0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C1E0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C2D8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C3F8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C4F0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C610);

INCLUDE_ASM("dirtysock/tagsunk", func_0025C8A8);

extern "C" void* func_0025CD50(void*, int);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025CD28__FPvi);
#ifdef SKIP_ASM
void* func_0025CD28(void* self, int a1)
{
    return func_0025CD50(self, *(int*)((char*)((char*)self + a1 * 4) + 0x70));
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025CD50);

INCLUDE_ASM("dirtysock/tagsunk", func_0025CEB0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025CFE0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D048);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D1B8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D2D8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D428);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D538);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D6C0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D770);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D800);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D860);

INCLUDE_ASM("dirtysock/tagsunk", func_0025D8F8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025DA90);
#ifdef SKIP_ASM
extern "C" char* func_0025DA90(void* self)
{
    char* s;
    if (*(int*)((char*)self + 0x54) == 0) {
        return 0;
    }
    s = *(char**)((char*)self + 0x84);
    if (s != 0) {
        if (*s != 0) {
            return s;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025DAC0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025DB80);

INCLUDE_ASM("dirtysock/tagsunk", func_0025DD08);

INCLUDE_ASM("dirtysock/tagsunk", func_0025DDE0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025DF00);

INCLUDE_ASM("dirtysock/tagsunk", func_0025DF98);

INCLUDE_ASM("dirtysock/tagsunk", func_0025E0C8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025E138);

INCLUDE_ASM("dirtysock/tagsunk", func_0025E348);

INCLUDE_ASM("dirtysock/tagsunk", func_0025E420);

INCLUDE_ASM("dirtysock/tagsunk", func_0025E590);

INCLUDE_ASM("dirtysock/tagsunk", func_0025E6D0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025E7F0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025EE20);

INCLUDE_ASM("dirtysock/tagsunk", func_0025EED8);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F030);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F120);
#ifdef SKIP_ASM
extern "C" int func_003EE558(void*);

extern "C" int func_0025F120(void* self)
{
    if (*(int*)self == 0) {
        return 0;
    }
    return func_003EE558(*(void**)((char*)self + 0x5C));
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F150);
#ifdef SKIP_ASM
extern "C" void* func_003EE560(void*);

extern "C" void* func_0025F150(void* self)
{
    if (*(int*)self == 0) {
        return 0;
    }
    return (char*)func_003EE560(*(void**)((char*)self + 0x5C)) + 0x1C;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F180);
#ifdef SKIP_ASM
extern "C" void* func_003EE560(void*);

static inline short getFlags(void* p)
{
    return *(short*)((char*)p + 0xA);
}

extern "C" int func_0025F180(void* self)
{
    if (*(int*)self == 0) {
        return 0;
    }
    return getFlags(func_003EE560(*(void**)((char*)self + 0x5C))) & 1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025F1B0);
#ifdef SKIP_ASM
extern "C" int func_003EE558(void*);

extern "C" int func_0025F1B0(void* self)
{
    if (*(int*)self == 0) {
        return 0;
    }
    return func_003EE558(*(void**)((char*)self + 0x60));
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025F1E0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F248);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F288);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F338);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F388);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F430);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F4C0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F628);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F740);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F858);

INCLUDE_ASM("dirtysock/tagsunk", func_0025F8A0);

INCLUDE_ASM("dirtysock/tagsunk", func_0025FA48);

INCLUDE_ASM("dirtysock/tagsunk", func_0025FAD0);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FB58);
#ifdef SKIP_ASM
extern "C" void func_0025FC70(void* self, int a1, int a2);

extern "C" void func_0025FB58(void* self, int a1, int a2)
{
    if (*(int*)self != 0) {
        *(int*)((char*)self + 0xA4) = a2;
        func_0025FC70(self, a1, 0x17);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025FB80);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FC20__FPv);
#ifdef SKIP_ASM
void func_0025FC20(void* self)
{
}
#endif

extern void* D_004A3270[];
extern "C" void* func_0025FB80(void*, int, void*);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FC28__FPvi);
#ifdef SKIP_ASM
void* func_0025FC28(void* self, int a1)
{
    return func_0025FB80(self, a1, (void*)D_004A3270);
}
#endif

extern void* D_004807F8[];
extern "C" void* func_0025FB80(void*, int, void*);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_0025FC48__FPv);
#ifdef SKIP_ASM
void* func_0025FC48(void* self)
{
    return func_0025FB80(self, *(int*)((char*)self + 0x98), (void*)D_004807F8);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_0025FC70);

INCLUDE_ASM("dirtysock/tagsunk", func_0025FD50);

INCLUDE_ASM("dirtysock/tagsunk", func_00260C68);

INCLUDE_ASM("dirtysock/tagsunk", func_00260E50);

INCLUDE_ASM("dirtysock/tagsunk", func_00260F10);

INCLUDE_ASM("dirtysock/tagsunk", func_00260F80);

INCLUDE_ASM("dirtysock/tagsunk", func_00261008);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261058__FPv);
#ifdef SKIP_ASM
void func_00261058(void* self)
{
    *(char*)self = 0;
    *(int*)((char*)self + 0x40) = -1;
    *(char*)((char*)self + 0x44) = 0;
    *(char*)((char*)self + 0x64) = 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00261070);

INCLUDE_ASM("dirtysock/tagsunk", func_002610D0);

INCLUDE_ASM("dirtysock/tagsunk", func_00261248);

INCLUDE_ASM("dirtysock/tagsunk", func_002613C8);

INCLUDE_ASM("dirtysock/tagsunk", func_00261408);

INCLUDE_ASM("dirtysock/tagsunk", func_00261460);

INCLUDE_ASM("dirtysock/tagsunk", func_00261490);

INCLUDE_ASM("dirtysock/tagsunk", func_002614E8);

INCLUDE_ASM("dirtysock/tagsunk", func_00261530);

INCLUDE_ASM("dirtysock/tagsunk", func_002615C8);

INCLUDE_ASM("dirtysock/tagsunk", func_00261770);

INCLUDE_ASM("dirtysock/tagsunk", func_002618C0);

INCLUDE_ASM("dirtysock/tagsunk", func_00261970);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00261A20__FPv);
#ifdef SKIP_ASM
void func_00261A20(void* self)
{
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00261A28);

INCLUDE_ASM("dirtysock/tagsunk", func_00261CD0);

INCLUDE_ASM("dirtysock/tagsunk", func_00261EF8);

INCLUDE_ASM("dirtysock/tagsunk", func_002620D8);

INCLUDE_ASM("dirtysock/tagsunk", func_00262280);

INCLUDE_ASM("dirtysock/tagsunk", func_002622F0);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262360);
#ifdef SKIP_ASM
extern "C" int func_00262468(void* self, int a1);
// PORT: the unit declares func_00262390 as `int (void*)`; this caller passes a
// second argument (it reaches func_00262620's callee in $a1). Bound by asm label.
int func_00262390_2(void* self, int a1) __asm__("func_00262390__FPv");

extern "C" int func_00262360(void* self, int a1)
{
    return func_00262390_2(self, func_00262468(self, a1));
}
#endif

extern "C" void* func_00262620(void* self);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262390__FPv);
#ifdef SKIP_ASM
int func_00262390(void* self)
{
    return (func_00262620(self) != 0);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002623B0);
#ifdef SKIP_ASM
extern "C" int func_00262468(void* self, int a1);
// PORT: the unit declares func_002623E0 as `int (void*)`; this caller passes a
// second argument (it reaches func_00262658's callee in $a1). Bound by asm label.
int func_002623E0_2(void* self, int a1) __asm__("func_002623E0__FPv");

extern "C" int func_002623B0(void* self, int a1)
{
    return func_002623E0_2(self, func_00262468(self, a1));
}
#endif

extern "C" void* func_00262658(void* self);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002623E0__FPv);
#ifdef SKIP_ASM
int func_002623E0(void* self)
{
    return (func_00262658(self) != 0);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262400);
#ifdef SKIP_ASM
extern "C" void* func_002625C8(void* self, int idx);

extern "C" int func_00262400(void* self, int a1)
{
    void* p = func_002625C8(self, a1);
    if (p != 0) {
        return (*(int*)((char*)p + 0xA8) >> 20) & 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262438);
#ifdef SKIP_ASM
extern "C" int func_00262468(void* self, int a1);
extern "C" int func_00262400(void* self, int a1);

extern "C" int func_00262438(void* self, int a1)
{
    return func_00262400(self, func_00262468(self, a1));
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00262468);

INCLUDE_ASM("dirtysock/tagsunk", func_002624F8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262598);
#ifdef SKIP_ASM
extern "C" int func_003EE550(void*);

extern "C" int func_00262598(void* self)
{
    void* p = *(void**)((char*)self + 0xCC);
    if (p == 0) {
        return 0;
    }
    return func_003EE550(p);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002625C8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262620);
#ifdef SKIP_ASM
extern "C" void* func_002625C8(void* self, int idx);
// PORT: the unit declares func_00262620 as `void* (void*)`; the body passes a second
// argument through to func_002625C8 in $a1. Bound by asm label.
void* func_00262620_2(void* self, int idx) __asm__("func_00262620");

void* func_00262620_2(void* self, int idx)
{
    void* p = func_002625C8(self, idx);
    if ((p != 0) && (*(int*)((char*)p + 0xA8) & 0x4)) {
        return p;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262658);
#ifdef SKIP_ASM
extern "C" void* func_002625C8(void* self, int idx);
// PORT: the unit declares func_00262658 as `void* (void*)`; the body passes a second
// argument through to func_002625C8 in $a1. Bound by asm label.
void* func_00262658_2(void* self, int idx) __asm__("func_00262658");

void* func_00262658_2(void* self, int idx)
{
    void* p = func_002625C8(self, idx);
    if ((p != 0) && (*(int*)((char*)p + 0xA8) & 0x200)) {
        return p;
    }
    return 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00262690);

INCLUDE_ASM("dirtysock/tagsunk", func_00262768);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002628C0);
#ifdef SKIP_ASM
extern "C" int func_002628C0(void* self, int sel)
{
    if (*(int*)((char*)self + 0xC8) == 0) {
        return -1;
    }
    switch (sel) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00262938);
#ifdef SKIP_ASM
extern "C" int func_00262938(void* self)
{
    return (*(int*)((char*)self + 0xb0) - *(int*)((char*)self + 0xac)) >> 2;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00262950);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_00262A08);
#ifdef SKIP_ASM
extern "C" int func_00262468(void* self, int a1);
extern "C" int func_00262950(void* self, int a1);

extern "C" int func_00262A08(void* self, int a1)
{
    return func_00262950(self, func_00262468(self, a1));
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00262A38);

INCLUDE_ASM("dirtysock/tagsunk", func_00262AF0);

INCLUDE_ASM("dirtysock/tagsunk", func_00262E20);

INCLUDE_ASM("dirtysock/tagsunk", func_00262EC0);

INCLUDE_ASM("dirtysock/tagsunk", func_00263018);

INCLUDE_ASM("dirtysock/tagsunk", func_00263128);

INCLUDE_ASM("dirtysock/tagsunk", func_00263270);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263308);
#ifdef SKIP_ASM
extern "C" void* func_00263308(void* self, unsigned int i)
{
    void** begin = *(void***)((char*)self + 0xBC);
    void** end = *(void***)((char*)self + 0xC0);
    if (i >= (unsigned int)(end - begin)) {
        return 0;
    }
    return *(void**)((char*)begin + (i << 2));
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00263338);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_002633C8);
#ifdef SKIP_ASM
extern "C" int func_00262468(void* self, int a1);
extern "C" int func_00263338(void* self, int a1);

extern "C" int func_002633C8(void* self, int a1)
{
    return func_00263338(self, func_00262468(self, a1));
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002633F8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("dirtysock/tagsunk", func_00263490);
#ifdef SKIP_ASM
extern "C" int func_00262468(void* self, int a1);
extern "C" int func_002633F8(void* self, int a1);

extern "C" int func_00263490(void* self, int a1)
{
    return func_002633F8(self, func_00262468(self, a1));
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002634C0);

INCLUDE_ASM("dirtysock/tagsunk", func_002635A8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263630);
#ifdef SKIP_ASM
extern "C" void* func_00263630(void* self, unsigned int i)
{
    void** begin = *(void***)((char*)self + 0xAC);
    void** end = *(void***)((char*)self + 0xB0);
    if (i >= (unsigned int)(end - begin)) {
        return 0;
    }
    return *(void**)((char*)begin + (i << 2));
}
#endif

extern void* D_004810C8[];

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263660__FPv);
#ifdef SKIP_ASM
void* func_00263660(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 0x8) = (int)(void*)D_004810C8;
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263678);
#ifdef SKIP_ASM
extern void* D_004810C8[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00263678(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_004810C8;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002636A8);

INCLUDE_ASM("dirtysock/tagsunk", func_002636F0);

INCLUDE_ASM("dirtysock/tagsunk", func_00263828);

extern void* D_004810B0[];

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00263998__FPv);
#ifdef SKIP_ASM
void* func_00263998(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 0x14) = (int)(void*)D_004810B0;
    return self;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002639B0);
#ifdef SKIP_ASM
extern void* D_004810B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_002639B0(void* self, int flags)
{
    *(void***)((char*)self + 0x14) = D_004810B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002639E0);

INCLUDE_ASM("dirtysock/tagsunk", func_00263BA8);

INCLUDE_ASM("dirtysock/tagsunk", func_00263CF0);

INCLUDE_ASM("dirtysock/tagsunk", func_00264030);

INCLUDE_ASM("dirtysock/tagsunk", func_00264098);

INCLUDE_ASM("dirtysock/tagsunk", func_002640C0);

INCLUDE_ASM("dirtysock/tagsunk", func_00264138);

INCLUDE_ASM("dirtysock/tagsunk", func_002642B8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264960);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_004805A8[];

extern "C" void* func_00264960(void)
{
    return cMemMan_alloc(0xF0, D_004805A8, 0x20000000, 0);
}
#endif

void operator_delete(int*);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00264990);
#ifdef SKIP_ASM
extern "C" void func_00264990(void* self, int* p)
{
    operator_delete(p);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002649B0);

INCLUDE_ASM("dirtysock/tagsunk", func_00264A58);

INCLUDE_ASM("dirtysock/tagsunk", func_00264B20);

INCLUDE_ASM("dirtysock/tagsunk", func_00264BE8);

INCLUDE_ASM("dirtysock/tagsunk", func_00264CB8);

INCLUDE_ASM("dirtysock/tagsunk", func_00264CF0);

INCLUDE_ASM("dirtysock/tagsunk", func_00264D80);

INCLUDE_ASM("dirtysock/tagsunk", func_00264E50);

INCLUDE_ASM("dirtysock/tagsunk", func_00264F88);

INCLUDE_ASM("dirtysock/tagsunk", func_00265018);

INCLUDE_ASM("dirtysock/tagsunk", func_00265140);

INCLUDE_ASM("dirtysock/tagsunk", func_00265190);

INCLUDE_ASM("dirtysock/tagsunk", func_00265290);

INCLUDE_ASM("dirtysock/tagsunk", func_00265398);

INCLUDE_ASM("dirtysock/tagsunk", func_00265458);

INCLUDE_ASM("dirtysock/tagsunk", func_00265568);

INCLUDE_ASM("dirtysock/tagsunk", func_00265688);

INCLUDE_ASM("dirtysock/tagsunk", func_00265700);

INCLUDE_ASM("dirtysock/tagsunk", func_00265768);

INCLUDE_ASM("dirtysock/tagsunk", func_00265880);

INCLUDE_ASM("dirtysock/tagsunk", func_002658B8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002658E0);
#ifdef SKIP_ASM
extern "C" void func_00265950(void* self);

extern "C" void* func_002658E0(void* self)
{
    func_00265950(self);
    return self;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00265908);

INCLUDE_ASM("dirtysock/tagsunk", func_00265950);

INCLUDE_ASM("dirtysock/tagsunk", func_002659C0);

INCLUDE_ASM("dirtysock/tagsunk", func_002659E8);

INCLUDE_ASM("dirtysock/tagsunk", func_00265A38);

INCLUDE_ASM("dirtysock/tagsunk", func_00265A68);

INCLUDE_ASM("dirtysock/tagsunk", func_00265CB0);

INCLUDE_ASM("dirtysock/tagsunk", func_00265D08);

INCLUDE_ASM("dirtysock/tagsunk", func_00265D38);

INCLUDE_ASM("dirtysock/tagsunk", func_00265D68);

INCLUDE_ASM("dirtysock/tagsunk", func_00265D98);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265DF0__FPv);
#ifdef SKIP_ASM
void func_00265DF0(void* self)
{
    *(int*)self = 2;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E00__FPv);
#ifdef SKIP_ASM
void func_00265E00(void* self)
{
    *(int*)self = 6;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E10__FPv);
#ifdef SKIP_ASM
void func_00265E10(void* self)
{
    *(int*)self = 10;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E20__FPv);
#ifdef SKIP_ASM
void func_00265E20(void* self)
{
    *(int*)self = 14;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00265E30__FPv);
#ifdef SKIP_ASM
void func_00265E30(void* self)
{
    *(int*)self = 18;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00265E40);

INCLUDE_ASM("dirtysock/tagsunk", func_00265E80);

INCLUDE_ASM("dirtysock/tagsunk", func_00265EC8);

INCLUDE_ASM("dirtysock/tagsunk", func_00265F00);

INCLUDE_ASM("dirtysock/tagsunk", func_00265F18);

INCLUDE_ASM("dirtysock/tagsunk", func_00265F68);

INCLUDE_ASM("dirtysock/tagsunk", func_00265FB8);

INCLUDE_ASM("dirtysock/tagsunk", func_00266010);

INCLUDE_ASM("dirtysock/tagsunk", func_00266038);

INCLUDE_ASM("dirtysock/tagsunk", func_00266060);

INCLUDE_ASM("dirtysock/tagsunk", func_002660A0);

INCLUDE_ASM("dirtysock/tagsunk", func_002660C8);

INCLUDE_ASM("dirtysock/tagsunk", func_00266148);

INCLUDE_ASM("dirtysock/tagsunk", func_00266170);

INCLUDE_ASM("dirtysock/tagsunk", func_002661D8);

INCLUDE_ASM("dirtysock/tagsunk", func_00266240);

INCLUDE_ASM("dirtysock/tagsunk", func_00266298);

INCLUDE_ASM("dirtysock/tagsunk", func_002662C0);

INCLUDE_ASM("dirtysock/tagsunk", func_002662E8);

INCLUDE_ASM("dirtysock/tagsunk", func_00266320);

INCLUDE_ASM("dirtysock/tagsunk", func_00266348);

INCLUDE_ASM("dirtysock/tagsunk", func_00266358);

INCLUDE_ASM("dirtysock/tagsunk", func_00266550);

INCLUDE_ASM("dirtysock/tagsunk", func_00266578);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266728);
#ifdef SKIP_ASM
extern char D_00481080[];
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

extern "C" void* func_00266728(void* self, int size, int kind)
{
    int flags = 0x20000000;
    if (kind == 0x20) {
        flags = 0x23000000;
    }
    return operator_new_tag(size + 0x400, D_00481080, flags, 0);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266760);
#ifdef SKIP_ASM
void* cMemMan_free(void* ptr);

extern "C" void func_00266760(void* self, void* ptr)
{
    if (ptr != 0) {
        cMemMan_free(ptr);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00266788);

INCLUDE_ASM("dirtysock/tagsunk", func_002667E8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002668A8);
#ifdef SKIP_ASM
extern "C" void func_003F6358(void* self);
extern "C" void func_0040C7C0(void* self, int a1);
extern "C" void func_0040D430(void* self, int a1);

extern "C" void func_002668A8(void* self)
{
    func_003F6358(self);
    func_0040C7C0(self, 0);
    func_0040D430(self, 0);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002668E8);

INCLUDE_ASM("dirtysock/tagsunk", func_00266BA8);

INCLUDE_ASM("dirtysock/tagsunk", func_00266CA0);

INCLUDE_ASM("dirtysock/tagsunk", func_00266CD8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266D00__FPv);
#ifdef SKIP_ASM
void* func_00266D00(void* self)
{
    int t0 = 0;
    *(int*)self = t0;
    *(int*)((char*)self + 0x4) = t0;
    return self;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00266D10);

INCLUDE_ASM("dirtysock/tagsunk", func_00266D58);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266DF8);
#ifdef SKIP_ASM
extern "C" void func_003F6AF0(void* self);

extern "C" void func_00266DF8(void* self)
{
    if (*(void**)self != 0) {
        func_003F6AF0(self);
        *(void**)self = 0;
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00266E30);

INCLUDE_ASM("dirtysock/tagsunk", func_00266E88);

extern "C" void* func_003F6C40(void* self);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266F38__FPv);
#ifdef SKIP_ASM
void* func_00266F38(void* self)
{
    return func_003F6C40(self);
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00266F58);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00266F90);
#ifdef SKIP_ASM
extern "C" void func_003F6BA0(void* a, void* b);

extern "C" void func_00266F90(void* self, void* a, void* b)
{
    if (*(int*)self != 0) {
        func_003F6BA0(a, b);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00267128);

INCLUDE_ASM("dirtysock/tagsunk", func_002672C8);

INCLUDE_ASM("dirtysock/tagsunk", func_00267468);

extern "C" void* func_00255840(void* self);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267DA8__FPv);
#ifdef SKIP_ASM
void* func_00267DA8(void* self)
{
    return func_00255840(self);
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267DC8__FPv);
#ifdef SKIP_ASM
void func_00267DC8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267DD0__FPv);
#ifdef SKIP_ASM
int func_00267DD0(void* self)
{
    return 0;
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00267DD8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267DE8__FPv);
#ifdef SKIP_ASM
int func_00267DE8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267DF0__FPv);
#ifdef SKIP_ASM
int func_00267DF0(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267DF8__FPv);
#ifdef SKIP_ASM
int func_00267DF8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267E00);
#ifdef SKIP_ASM
extern "C" int func_00267E00(void* self)
{
    return *(int*)((char*)self + 0xc) == 2;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267E10__FPv);
#ifdef SKIP_ASM
int func_00267E10(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267E18);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00267E18(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00267E48);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267E98);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00267E98(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00267EC8);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267F28);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00267F28(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00267F58);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00267FB8);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00267FB8(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00267FE8);

INCLUDE_ASM("dirtysock/tagsunk", func_00268048);

INCLUDE_ASM("dirtysock/tagsunk", func_002680A8);

INCLUDE_ASM("dirtysock/tagsunk", func_00268158);

INCLUDE_ASM("dirtysock/tagsunk", func_002681B0);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268210);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00268210(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00268240);

INCLUDE_ASM("dirtysock/tagsunk", func_002682A8);

INCLUDE_ASM("dirtysock/tagsunk", func_00268308);

INCLUDE_ASM("dirtysock/tagsunk", func_00268398);

INCLUDE_ASM("dirtysock/tagsunk", func_002683F0);

INCLUDE_ASM("dirtysock/tagsunk", func_00268460);

INCLUDE_ASM("dirtysock/tagsunk", func_002684C0);

INCLUDE_ASM("dirtysock/tagsunk", func_00268588);

INCLUDE_ASM("dirtysock/tagsunk", func_002685E0);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268658);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00268658(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00268688);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_002686F0);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_002686F0(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_00268720);

//100%
INCLUDE_ASM("dirtysock/tagsunk", func_00268790);
#ifdef SKIP_ASM
extern void* D_004812B0[];
void operator_delete(int*);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_00268790(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004812B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("dirtysock/tagsunk", func_002687C0);

