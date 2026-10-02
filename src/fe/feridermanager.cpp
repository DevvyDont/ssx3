#include "common.h"

INCLUDE_ASM("fe/feridermanager", cFERider_initBoneMap);

INCLUDE_ASM("fe/feridermanager", func_0019E7F0);

INCLUDE_ASM("fe/feridermanager", cFERider_init);

INCLUDE_ASM("fe/feridermanager", func_0019EBA0);

INCLUDE_ASM("fe/feridermanager", func_0019EC68);

INCLUDE_ASM("fe/feridermanager", func_0019ED80);

INCLUDE_ASM("fe/feridermanager", func_0019EE88);

INCLUDE_ASM("fe/feridermanager", func_0019F138);

INCLUDE_ASM("fe/feridermanager", func_0019F2D0);

//100%
INCLUDE_ASM("fe/feridermanager", func_0019F3E8);
#ifdef SKIP_ASM
struct sQuadRM { int x[4]; } __attribute__((aligned(16)));
extern "C" void func_0019F2D0(void* self);

extern "C" void func_0019F3E8(void* self, sQuadRM* src)
{
    *(sQuadRM*)((char*)self + 0xC30) = src[0];
    *(sQuadRM*)((char*)self + 0xC40) = src[1];
    func_0019F2D0(self);
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", func_0019F548);
#ifdef SKIP_ASM
extern "C" void func_0019F2D0(void* self);

extern "C" void func_0019F548(void* self, sQuadRM* src)
{
    *(sQuadRM*)((char*)self + 0xC50) = src[0];
    *(sQuadRM*)((char*)self + 0xC60) = src[1];
    func_0019F2D0(self);
}
#endif

INCLUDE_ASM("fe/feridermanager", func_0019F780);

//100%
INCLUDE_ASM("fe/feridermanager", func_0019F878);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);

struct sRiderName_F878 {
    int id;
    char name[8];
};

struct sRiderTable_F878 {
    char pad_0x0[0x18];
    int count;
    sRiderName_F878 entries[1];
};

extern "C" int func_0019F878(sRiderTable_F878* self, const char* name)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (func_004165A8(name, self->entries[i].name) == 0) {
            return self->entries[i].id;
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("fe/feridermanager", func_0019F908);

INCLUDE_ASM("fe/feridermanager", func_0019FA78);

INCLUDE_ASM("fe/feridermanager", func_0019FBE0);

INCLUDE_ASM("fe/feridermanager", func_0019FD58);

INCLUDE_ASM("fe/feridermanager", func_0019FF00);

INCLUDE_ASM("fe/feridermanager", func_001A0100);

//100%
INCLUDE_ASM("fe/feridermanager", func_001A0358);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_002A16B0(void* mgr, int id);
extern "C" void func_002A1778(void* mgr, int id);

extern "C" void func_001A0358(void* self)
{
    if (*(int*)((char*)self + 0xC1C) != 0) {
        func_002A1778(func_0028B180(), *(int*)self);
        *(int*)((char*)self + 0xC1C) = 0;
    } else if (*(int*)((char*)self + 0xC20) != 0) {
        func_002A16B0(func_0028B180(), *(int*)self);
        *(int*)((char*)self + 0xC20) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", func_001A03C0);
#ifdef SKIP_ASM
extern "C" void* func_0019E3D0(void* self);

extern "C" void* func_001A03C0(void* self)
{
    char* p = (char*)self;
    int i;
    for (i = 1; i != -1; i--, p += 0xCE0) {
        func_0019E3D0(p);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", func_001A0420);
#ifdef SKIP_ASM
extern "C" void func_001A04C8(void* self);
extern "C" void func_0019E498(void* self, int flags);
void operator_delete(int* p);

extern "C" void func_001A0420(void* self, int flags)
{
    func_001A04C8(self);
    char* base = (char*)self;
    if (base != 0) {
        char* p = base + 0x19C0;
        while (base != p) {
            p -= 0xCE0;
            func_0019E498(p, 0);
        }
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", func_001A0498);
#ifdef SKIP_ASM
struct sRiderSlot_001A0498 {
    int field_0x0;
    int index;
    char pad[0xce0 - 8];
};

extern "C" void func_001A0498(sRiderSlot_001A0498* self)
{
    int i;
    sRiderSlot_001A0498* p = self;
    for (i = 0; i < 2; i++) {
        p->index = i;
        p++;
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", func_001A04C8);
#ifdef SKIP_ASM
extern "C" void func_0019EBA0(void* slot);

extern "C" void func_001A04C8(void* self)
{
    int i;
    for (i = 0; i < 2; i++) {
        func_0019EBA0((char*)self + i * 0xce0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", func_001A0508);
#ifdef SKIP_ASM
extern "C" void func_0019E588(void* slot, int a1, int a2);

extern "C" void func_001A0508(void* self, int idx, int a2, int a3)
{
    func_0019E588((char*)self + idx * 0xce0, a2, a3);
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", func_001A0538);
#ifdef SKIP_ASM
extern "C" void* func_001A0538(void* self, int a1)
{
    return (char*)self + a1 * 0xce0;
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", func_001A0548);
#ifdef SKIP_ASM
extern "C" void* func_001A0548(void* self, int a1)
{
    return (char*)self + a1 * 0xce0;
}
#endif

//100%
INCLUDE_ASM("fe/feridermanager", func_001A0570);
#ifdef SKIP_ASM
extern "C" void func_0019E538(void* slot, int v);

extern "C" void func_001A0570(void* self, int i, int v)
{
    func_0019E538((char*)self + i * 0xce0, v);
}
#endif

INCLUDE_ASM("fe/feridermanager", func_001A0598);

//100%
INCLUDE_ASM("fe/feridermanager", func_001A0608);
#ifdef SKIP_ASM
extern "C" void func_0019F138(void* slot);

extern "C" void func_001A0608(void* self)
{
    int i;
    for (i = 0; i < 2; i++) {
        func_0019F138((char*)self + i * 0xce0);
    }
}
#endif

