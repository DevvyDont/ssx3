#include "common.h"

//100%
INCLUDE_ASM("fe/fereal", cFECustom_cFECustom);
#ifdef SKIP_ASM
struct sVE0720 {
    short delta;
    short index;
    int (*fn)(void*);
};
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0039E2A0(void* self, void* owner);
extern "C" void cUIStateStack_pushExplicit(void* stack, void* state);
extern char D_00460B90[];
extern void* D_0046D810[];
extern sVE0720 D_0046BB88[];

extern "C" void* cFECustom_cFECustom(void* self, void* owner)
{
    *(void**)((char*)self + 0x0) = owner;
    *(void**)((char*)self + 0x10) = 0;
    *(void***)((char*)self + 0x4) = D_0046D810;
    char* o = (char*)cMemMan_alloc(0x50, D_00460B90, 0, 0);
    func_0039E2A0(o, *(void**)((char*)self + 0x0));
    *(int*)(o + 0x48) = 0;
    *(int*)(o + 0x4C) = 0;
    *(sVE0720**)(o + 0x8) = D_0046BB88;
    *(void**)((char*)self + 0x10) = o;
    if (o != 0) {
        D_0046BB88[4].fn(o + D_0046BB88[4].delta);
        cUIStateStack_pushExplicit((char*)*(void**)((char*)self + 0x0) + 0x18, *(void**)((char*)self + 0x10));
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fereal", func_001A07C8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045FFF8[];
struct sVEntry001A07C8 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

extern "C" int func_001A07C8(void* self, int a1, int hash)
{
    if (hash == GetHashValue32(D_0045FFF8)) {
        void* obj = *(void**)((char*)self + 0x10);
        if (obj != 0) {
            sVEntry001A07C8* vt = *(sVEntry001A07C8**)((char*)obj + 8);
            vt[24].fn((char*)obj + vt[24].delta, 1, 0);
        }
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/fereal", cFECustom_getNextState);

INCLUDE_ASM("fe/fereal", func_001A16C0);

//100%
INCLUDE_ASM("fe/fereal", func_001A1CB8);
#ifdef SKIP_ASM
extern "C" void func_001A1CB8(void* self, int a1, int val)
{
    char* p = (char*)self + (signed char)a1;
    *(char*)(p + 0x8) = val;
}
#endif

//100%
INCLUDE_ASM("fe/fereal", func_001A1CD0);
#ifdef SKIP_ASM
extern "C" unsigned char func_001A1CD0(void* self, int a1)
{
    char* p = (char*)self + (signed char)a1;
    return *(unsigned char*)(p + 0x8);
}
#endif

//100%
INCLUDE_ASM("fe/fereal", func_001A1CE8);
#ifdef SKIP_ASM
extern "C" void* cFE_cFE(void* self);
void func_00263660(void* p);
void func_00263998(void* p);
extern void* D_0046D7C0[];

extern "C" void* func_001A1CE8(void* self)
{
    cFE_cFE(self);
    *(void***)self = D_0046D7C0;
    func_00263660((char*)self + 0xB5AB0);
    func_00263998((char*)self + 0xB5ABC);
    *(int*)((char*)self + 0xB5AD4) = 0;
    *(int*)((char*)self + 0xB5AD8) = 0;
    *(int*)((char*)self + 0xB5ADC) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/fereal", func_001A1D50);
#ifdef SKIP_ASM
extern void* D_0046D7C0[];
extern "C" void func_002639B0(void* self, int flags);
extern "C" void func_00263678(void* self, int flags);
extern "C" void func_001957D0(void* self, int flags);

extern "C" void func_001A1D50(void* self, int flags)
{
    *(void***)self = D_0046D7C0;
    func_002639B0((char*)self + 0xB5ABC, 2);
    func_00263678((char*)self + 0xB5AB0, 2);
    func_001957D0(self, flags);
}
#endif

INCLUDE_ASM("fe/fereal", func_001A1DC0);

INCLUDE_ASM("fe/fereal", cRealFE_load);

//100%
INCLUDE_ASM("fe/fereal", func_001A2128);
#ifdef SKIP_ASM
extern "C" void* func_00231CF0(void* self);
extern "C" void func_00253418(void* p, int a1);
extern "C" void func_001A06B0(void* flow);
extern int D_004A19CC;
extern char D_004A4E80;

extern "C" int func_001A2128(void* self)
{
    if (func_00231CF0(self) == 0) {
        return 0;
    }
    void* h = *(void**)((char*)self + 0xB5AD4);
    if (h != 0) {
        func_00253418(h, 3);
        *(void**)((char*)self + 0xB5AD4) = 0;
    }
    *(int*)((char*)self + 0xB5AD8) = 0;
    D_004A19CC = 0;
    *(int*)((char*)self + 0xB5ADC) = 0;
    func_001A06B0(&D_004A4E80);
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fereal", func_001A2190);
#ifdef SKIP_ASM
struct sVEntry001A2190 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern void* D_004A289C;
extern void* D_004A28A8;
extern char D_005047F8[];
extern "C" void func_001A2BC0(void* self);
extern "C" void func_001A04C8(void* p);
extern "C" void func_0019CF40(void* p);
extern "C" void func_0038ADB0(void* p);
extern "C" void* func_0028B180();
extern "C" void func_00286200(void* p);

extern "C" void func_001A2190(void* self)
{
    char* g = (char*)D_004A289C;
    sVEntry001A2190* vt = *(sVEntry001A2190**)(g + 0x10D8);
    vt[19].fn(g + vt[19].delta);
    func_001A2BC0(self);
    func_001A04C8((char*)self + 0xB0);
    func_0019CF40((char*)self + 0x1A70);
    func_0038ADB0(D_005047F8);
    char* st = (char*)D_004A28A8;
    *(int*)(st + 0x7C) = 0;
    *(int*)(st + 0x80) = 0;
    func_00286200(func_0028B180());
}
#endif

INCLUDE_ASM("fe/fereal", func_001A2208);

INCLUDE_ASM("fe/fereal", func_001A27A0);

INCLUDE_ASM("fe/fereal", cRealFE_loadCharAnimations);

//100%
INCLUDE_ASM("fe/fereal", func_001A2BC0);
#ifdef SKIP_ASM
extern void** D_004A3DF8;
extern void* D_004A3E7C;
extern "C" void func_00314FE8(void* a, void* b);
extern "C" void func_00311110(void* a);
extern "C" void func_003112C8(void* a, int b);

extern "C" void func_001A2BC0(void* self)
{
    int i;
    for (i = 0; i < 1; i++) {
        unsigned char idx = i;
        void* p = *(void**)((char*)D_004A3DF8 + (idx << 2));
        if (p != 0) {
            func_00314FE8(D_004A3E7C, p);
            void** tbl = D_004A3DF8;
            *(void**)((char*)tbl + (idx << 2)) = 0;
            func_00311110(tbl);
            func_003112C8(p, 3);
        }
    }
}
#endif

INCLUDE_ASM("fe/fereal", cRealFE_loadStartState);

//100%
INCLUDE_ASM("fe/fereal", func_001A2E20);
#ifdef SKIP_ASM
void* func_0039E288(void* self);
extern void* D_00469348[];

extern "C" void* func_001A2E20(void* self)
{
    func_0039E288(self);
    *(void***)self = D_00469348;
    *(int*)((char*)self + 0x4) = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/fereal", func_001A2E58);

//100%
INCLUDE_ASM("fe/fereal", func_001A2F38);
#ifdef SKIP_ASM
void* func_0039E288(void* self);
extern void* D_00469318[];

extern "C" void* func_001A2F38(void* self)
{
    func_0039E288(self);
    *(void***)self = D_00469318;
    *(int*)((char*)self + 0x4) = 0;
    return self;
}
#endif

INCLUDE_ASM("fe/fereal", func_001A2F70);

