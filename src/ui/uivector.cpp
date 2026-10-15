#include "common.h"

// Flag words at cUIVector + 0x14 and + 0x74. The setters take an int and store it into a
// bool bitfield: that int->bool conversion is the `sltu` in their asm.
struct sUIVectorFlags {
    bool bit0 : 1;
    bool bit1 : 1;
    bool bit2 : 1;
    bool bit3 : 1;
    bool bit4 : 1;
    bool bit5 : 1;
    bool bit6 : 1;
};

INCLUDE_ASM("ui/uivector", cUIVector_setUIData);

INCLUDE_ASM("ui/uivector", func_003A36D0);

INCLUDE_ASM("ui/uivector", func_003A3808);

//100%
// PORT: func_004139F8 is libgcc's __fixunssfsi (float -> unsigned); called by its address name.
extern "C" unsigned int func_004139F8(float f);
extern void* D_004A289C;

struct sV3_3A39F8 {
    float x, y, z;
};

struct sV4_3A39F8 {
    float v[4];
} __attribute__((aligned(16)));

struct sCol_3A39F8 {
    float r, g, b, a;
};

struct sVtx_3A39F8 {
    float s;
    float t;
    float q;
    int padC;
    int col[4];         // 0x10
    sV4_3A39F8 pos;     // 0x20
};

struct sCount_3A39F8 {
    unsigned int flag : 1;
    unsigned int pad1 : 1;
    unsigned int mode : 15;
    unsigned int count : 15;
};

struct sFlags14_3A39F8 {
    unsigned char b14;
    unsigned char layer : 5;
};

struct sRS_3A39F8 {
    int f0;
    int f4;
    int f8;
    int fC;
    short f10;
};

struct sVE_3A39F8 { short delta; short index; void* fn; };

typedef void (*tDraw_3A39F8)(void*, int, int, void*, void*, int, void*, void*);
typedef void (*tDraw2_3A39F8)(void*, int, void*, int);

static inline void SetCol_3A39F8(sVtx_3A39F8& vtx, int k, float v)
{
    vtx.col[k] = func_004139F8(v * 0.5f);
}

static inline void AddPos_3A39F8(sV4_3A39F8& p, int k, float d)
{
    p.v[k] += d;
}

extern "C" void func_003A39F8(char* self, float x, float y)
{
    if (*(sV3_3A39F8**)(self + 0x80) == 0)
        return;
    for (unsigned int i = 0; i < ((sCount_3A39F8*)(self + 0x74))->count; i++)
    {
        (*(sV3_3A39F8**)(self + 0x84))[i] = (*(sV3_3A39F8**)(self + 0x80))[i];
        (*(sV3_3A39F8**)(self + 0x84))[i].x *= *(float*)(self + 0x50);
        (*(sV3_3A39F8**)(self + 0x84))[i].y *= *(float*)(self + 0x54);
    }
    sRS_3A39F8* rs = *(sRS_3A39F8**)((char*)D_004A289C + 0xE84);
    rs->f8 = (rs->f8 & ~0x3E0) | (((((sFlags14_3A39F8*)(self + 0x14))->layer) + 9) << 5 & 0x3E0);
    *(short*)(*(char**)((char*)D_004A289C + 0xE84) + 0x10) = -1;
    if (((sCount_3A39F8*)(self + 0x74))->flag)
    {
        float pos[3];
        pos[0] = *(float*)(self + 0x44) + x;
        pos[1] = *(float*)(self + 0x48) + y;
        pos[2] = 0.0f;
        char* g = (char*)D_004A289C;
        sVE_3A39F8* vt = *(sVE_3A39F8**)(g + 0x10D8);
        ((tDraw_3A39F8)vt[72].fn)(g + vt[72].delta, ((sCount_3A39F8*)(self + 0x74))->mode, ((sCount_3A39F8*)(self + 0x74))->count,
                                  *(void**)(self + 0x78), *(void**)(self + 0x84), 0, *(void**)(self + 0x7C), pos);
        return;
    }
    if (*(sVtx_3A39F8**)(self + 0x8C) == 0)
        return;
    for (unsigned int i = 0; i < ((sCount_3A39F8*)(self + 0x74))->count; i++)
    {
        sV4_3A39F8 tt;
        tt.v[0] = (*(sV3_3A39F8**)(self + 0x84))[i].x;
        tt.v[1] = (*(sV3_3A39F8**)(self + 0x84))[i].y;
        tt.v[2] = (*(sV3_3A39F8**)(self + 0x84))[i].z;
        tt.v[3] = 1.0f;
        (*(sVtx_3A39F8**)(self + 0x8C))[i].pos = tt;
        AddPos_3A39F8((*(sVtx_3A39F8**)(self + 0x8C))[i].pos, 0, *(float*)(self + 0x44) + x);
        AddPos_3A39F8((*(sVtx_3A39F8**)(self + 0x8C))[i].pos, 1, *(float*)(self + 0x48) + y);
        (*(sVtx_3A39F8**)(self + 0x8C))[i].q = 1.0f;
        (*(sVtx_3A39F8**)(self + 0x8C))[i].s = 0.0f;
        (*(sVtx_3A39F8**)(self + 0x8C))[i].t = 0.0f;
        SetCol_3A39F8((*(sVtx_3A39F8**)(self + 0x8C))[i], 3, (*(sCol_3A39F8**)(self + 0x7C))[i].r);
        SetCol_3A39F8((*(sVtx_3A39F8**)(self + 0x8C))[i], 0, (*(sCol_3A39F8**)(self + 0x7C))[i].g);
        SetCol_3A39F8((*(sVtx_3A39F8**)(self + 0x8C))[i], 1, (*(sCol_3A39F8**)(self + 0x7C))[i].b);
        SetCol_3A39F8((*(sVtx_3A39F8**)(self + 0x8C))[i], 2, (*(sCol_3A39F8**)(self + 0x7C))[i].a);
    }
    char* g = (char*)D_004A289C;
    sVE_3A39F8* vt = *(sVE_3A39F8**)(g + 0x10D8);
    ((tDraw2_3A39F8)vt[73].fn)(g + vt[73].delta, ((sCount_3A39F8*)(self + 0x74))->count, *(void**)(self + 0x8C), 0);
}

//100%
extern "C" void func_003975E0(void*, void*, unsigned char, unsigned short);
extern "C" void func_003974B0(void*, unsigned char);

extern "C" void func_003A3D28(void* self, unsigned short ev)
{
    void* a = *(void**)((char*)self + 0xC);
    if (a != 0) {
        func_003975E0(a, self, *(unsigned char*)((char*)self + 0x10), ev);
        func_003974B0(*(void**)((char*)self + 0xC), *(unsigned char*)((char*)self + 0x10));
    }
}

//100%
struct func_003A3D70_sVec3 {
    float x;
    float y;
    float z;
};

extern func_003A3D70_sVec3 D_004FF0D8;

extern "C" void func_003A3D70(void* self, func_003A3D70_sVec3* out)
{
    if (*(func_003A3D70_sVec3**)((char*)self + 0x80) == 0) {
        return;
    }
    *out = D_004FF0D8;
    float minX = 1000.0f;
    float maxX = -1000.0f;
    float minY = 1000.0f;
    float maxY = -1000.0f;
    unsigned int i;
    for (i = 0; i < *(unsigned int*)((char*)self + 0x74) >> 17; i++) {
        func_003A3D70_sVec3* p = &(*(func_003A3D70_sVec3**)((char*)self + 0x80))[i];
        if (p->x < minX) {
            minX = p->x;
        }
        if (maxX < p->x) {
            maxX = p->x;
        }
        if (p->y < minY) {
            minY = p->y;
        }
        if (maxY < p->y) {
            maxY = p->y;
        }
    }
    out->x = maxX - minX;
    out->y = maxY - minY;
}

//100%
struct func_003A3E38_sVec3 {
    float x;
    float y;
    float z;
};

struct func_003A3E38_sCount74 {
    unsigned int lo : 17;
    unsigned int count : 15;
};

extern "C" void func_003A3E38(void* self, func_003A3E38_sVec3* off)
{
    if (((func_003A3E38_sCount74*)((char*)self + 0x74))->count == 4) {
        signed char i;
        for (i = 1; i < 4; i++) {
            (*(func_003A3E38_sVec3**)((char*)self + 0x80))[i] = (*(func_003A3E38_sVec3**)((char*)self + 0x80))[0];
        }
        (*(func_003A3E38_sVec3**)((char*)self + 0x80))[1].x += off->x;
        (*(func_003A3E38_sVec3**)((char*)self + 0x80))[3].y += off->y;
        (*(func_003A3E38_sVec3**)((char*)self + 0x80))[2].x += off->x;
        (*(func_003A3E38_sVec3**)((char*)self + 0x80))[2].y += off->y;
    }
    *(func_003A3E38_sVec3*)((char*)self + 0x60) = *off;
}

INCLUDE_ASM("ui/uivector", func_003A3F08);

INCLUDE_ASM("ui/uivector", func_003A3F48);

INCLUDE_ASM("ui/uivector", func_003A4778);

INCLUDE_ASM("ui/uivector", func_003A4790);

extern "C" void func_003A47A0(void* self, short value)
{
    *(short*)((char*)self + 0xA) = value;
}

//100%
void operator_delete(int* ptr);

extern void* D_00494CE8[];

extern "C" void func_003A47A8(int* self, int flags)
{
    *(void***)self = D_00494CE8;
    if (flags & 1) {
        operator_delete(self);
    }
}

INCLUDE_ASM("ui/uivector", func_003A47D8);

//100%
void operator_delete(int* ptr);
extern "C" void func_003A4868(void* self);

extern void* D_00494CC0[];

extern "C" void func_003A47F8(int* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_00494CC0;
    func_003A4868(self);
    if (flags & 1) {
        operator_delete(self);
    }
}

extern "C" void cListNode_removeFromList(void*);

extern "C" int func_003A4848(void* self)
{
    return *(int*)((char*)self + 0x0);
}

extern "C" int func_003A4850(void* self)
{
    return *(int*)((char*)self + 0x4);
}

extern "C" void func_003A4858(void* self, int value)
{
    *(int*)((char*)self + 0x0) = value;
}

extern "C" void func_003A4860(void* self, int value)
{
    *(int*)((char*)self + 0x4) = value;
}

//100%
extern "C" void func_003A4868(void* self)
{
    cListNode_removeFromList(self);
}

//100%
extern "C" void* func_00397948(void*);
extern void* D_00494C98[];
extern void* D_00494CC0[];

extern "C" void func_003A4888(void* self, int flags)
{
    *(void***)((char*)self + 0x18) = D_00494C98;
    func_00397948(self);
    *(void***)((char*)self + 0x14) = D_00494CC0;
    func_003A4868((char*)self + 0xC);
    *(void***)((char*)self + 0x8) = D_00494CC0;
    func_003A4868(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}

INCLUDE_ASM("ui/uivector", func_003A4900);

INCLUDE_ASM("ui/uivector", func_003A4940);

extern "C" int func_003A4950(void* self)
{
    return *(int*)((char*)self + 0xC);
}

INCLUDE_ASM("ui/uivector", func_003A4958);

INCLUDE_ASM("ui/uivector", func_003A4968);

INCLUDE_ASM("ui/uivector", func_003A4978);

INCLUDE_ASM("ui/uivector", func_003A4988);

INCLUDE_ASM("ui/uivector", func_003A4998);

INCLUDE_ASM("ui/uivector", func_003A49A8);

INCLUDE_ASM("ui/uivector", func_003A49B8);

INCLUDE_ASM("ui/uivector", func_003A49C8);

INCLUDE_ASM("ui/uivector", func_003A49D8);

//100%
extern "C" void func_003A49E8(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x14))->bit1 = enable;
}

//100%
extern "C" void func_003A4A08(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x14))->bit2 = enable;
}

//100%
extern "C" void func_003A4A28(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x14))->bit3 = enable;
}

//100%
extern "C" void func_003A4A48(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x14))->bit4 = enable;
}

//100%
extern "C" void func_003A4A68(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x14))->bit5 = enable;
}

//100%
extern "C" void func_003A4A88(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x14))->bit6 = enable;
}

INCLUDE_ASM("ui/uivector", func_003A4AA8);

extern "C" int func_003A4AC8(void* self)
{
    return *(int*)((char*)self + 0x18);
}

extern "C" void func_003A4AD0(void* self, int value)
{
    *(int*)((char*)self + 0x18) = value;
}

INCLUDE_ASM("ui/uivector", func_003A4AD8);

//100%
// 16 bytes, 4-byte aligned (copied with ldl/ldr pairs). Float fields are a guess.
struct sVec4 {
    float x;
    float y;
    float z;
    float w;
};

extern "C" void func_003A4AF8(void* self, sVec4* in)
{
    *(sVec4*)((char*)self + 0x1C) = *in;
}

extern "C" void* func_003A4B20(void* self)
{
    return (char*)self + 0x1C;
}

INCLUDE_ASM("ui/uivector", func_003A4B28);

extern "C" void func_003A4B50(void* self, float value)
{
    *(float*)((char*)self + 0x34) = value;
}

extern "C" float func_003A4B58(void* self)
{
    return *(float*)((char*)self + 0x34);
}

INCLUDE_ASM("ui/uivector", func_003A4B60);

INCLUDE_ASM("ui/uivector", func_003A4B70);

INCLUDE_ASM("ui/uivector", func_003A4B90);

INCLUDE_ASM("ui/uivector", func_003A4BB8);

extern "C" void func_003A4BC8(void* self, int value)
{
    *(int*)((char*)self + 0x38) = value;
}

extern "C" int func_003A4BD0(void* self)
{
    return *(int*)((char*)self + 0x38);
}

//100%
int func_003A4BD8(void* self)
{
    return 0;
}

//100%
void func_003A4BE0(void* self)
{
}

//100%
void func_003A4BE8(void* self)
{
}

//100%
void func_003A4BF0(void* self)
{
}

extern "C" void func_003A4BF8(void* self, int value)
{
    *(int*)((char*)self + 0x40) = value;
}

extern "C" int func_003A4C00(void* self)
{
    return *(int*)((char*)self + 0x40);
}

INCLUDE_ASM("ui/uivector", func_003A4C08);

INCLUDE_ASM("ui/uivector", func_003A4C28);

INCLUDE_ASM("ui/uivector", func_003A4C48);

INCLUDE_ASM("ui/uivector", func_003A4C68);

extern "C" int func_003A4C88(void* self)
{
    return *(int*)((char*)self + 0x5C);
}

//100%
// 12 bytes, 4-byte aligned (copied with ldl/ldr + lw). Float fields are a guess.
struct sVec3 {
    float x;
    float y;
    float z;
};

extern "C" void func_003A4C90(void* self, sVec3* out)
{
    *out = *(sVec3*)((char*)self + 0x60);
}

//100%
extern "C" void func_003A4CB0(void* self, sVec3* in)
{
    *(sVec3*)((char*)self + 0x60) = *in;
}

//100%
void operator_delete(int* ptr);

extern void* D_00494CE8[];

extern "C" void func_003A4CD0(int* self, int flags)
{
    *(void***)self = D_00494CE8;
    if (flags & 1) {
        operator_delete(self);
    }
}

//100%
struct sVEntry3A4D00 {
    short delta;
    short index;
    int (*fn)(void*, void*);
};

extern "C" void* func_003A4D00(void* self, void* obj)
{
    sVEntry3A4D00* vt = *(sVEntry3A4D00**)((char*)obj + 8);
    if (vt[0xF].fn((char*)obj + vt[0xF].delta, *(void**)((char*)self + 4)) == 0) {
        return 0;
    }
    return obj;
}

//100%
void operator_delete(int* ptr);

extern void* D_00494CE8[];

extern "C" void func_003A4D40(int* self, int flags)
{
    *(void***)self = D_00494CE8;
    if (flags & 1) {
        operator_delete(self);
    }
}

//100%
class func_003A4D70_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int);
};

extern "C" int func_003A4D70(void* self, func_003A4D70_cVirt* obj)
{
    obj->v07(*(int*)((char*)self + 0x4));
    return 0;
}

extern void* D_00494AA8[];
extern "C" void* func_0039FC48(void*);

INCLUDE_ASM("ui/uivector", func_003A4DA8);

extern "C" void func_003A4DD8()
{
}

extern "C" void func_003A4DE0()
{
}

extern "C" void func_003A4DE8()
{
}

INCLUDE_ASM("ui/uivector", func_003A4DF0);

INCLUDE_ASM("ui/uivector", func_003A4E38);

//100%
void* func_003A4E80(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_00494AA8;
    return func_0039FC48(self);
}

//100%
int func_003A4EA8(void* self)
{
    return 0x11;
}

INCLUDE_ASM("ui/uivector", func_003A4EB0);

INCLUDE_ASM("ui/uivector", func_003A4EC8);

INCLUDE_ASM("ui/uivector", func_003A4ED8);

INCLUDE_ASM("ui/uivector", func_003A4EF8);

INCLUDE_ASM("ui/uivector", func_003A4F08);

INCLUDE_ASM("ui/uivector", func_003A4F28);

INCLUDE_ASM("ui/uivector", func_003A4F38);

extern "C" int func_003A4F58(void* self)
{
    return *(int*)((char*)self + 0x7C);
}

INCLUDE_ASM("ui/uivector", func_003A4F60);

extern "C" int func_003A4F70(void* self)
{
    return *(int*)((char*)self + 0x78);
}

INCLUDE_ASM("ui/uivector", func_003A4F78);

INCLUDE_ASM("ui/uivector", func_003A4F88);

INCLUDE_ASM("ui/uivector", func_003A4FA0);

INCLUDE_ASM("ui/uivector", func_003A4FB8);

INCLUDE_ASM("ui/uivector", func_003A4FE0);

INCLUDE_ASM("ui/uivector", func_003A5028);

INCLUDE_ASM("ui/uivector", func_003A5068);

//100%
int func_003A50F8(void* self)
{
    return 0x10;
}

INCLUDE_ASM("ui/uivector", func_003A5100);

extern "C" void func_00397858(void* p);

extern "C" void func_003A5120(void* self)
{
    func_00397858((char*)self + 0x74);
}

extern "C" void func_00397870(void* p);

extern "C" void func_003A5140(void* self)
{
    func_00397870((char*)self + 0x74);
}

INCLUDE_ASM("ui/uivector", func_003A5160);

INCLUDE_ASM("ui/uivector", func_003A5180);

//100%
class func_003A51A0_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void*);
};

extern "C" int func_003A51A0(void* a, func_003A51A0_cVirt* obj)
{
    return obj->v02(a);
}

//100%
class func_003A51D0_cFunctor {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void*);
};

extern "C" void* func_00397A68(void* list, func_003A51D0_cFunctor* fn);

extern "C" void* func_003A51D0(void* self, func_003A51D0_cFunctor* fn)
{
    if (fn->v02(self) != 0) {
        return self;
    }
    return func_00397A68((char*)self + 0x74, fn);
}

INCLUDE_ASM("ui/uivector", func_003A5230);

INCLUDE_ASM("ui/uivector", func_003A52A0);

//100%
int func_003A5330(void* self)
{
    return 0x13;
}

//100%
extern "C" void* func_003A5338(void* self)
{
    *(unsigned char*)self &= 0xf0;
    return self;
}

INCLUDE_ASM("ui/uivector", func_003A5350);

INCLUDE_ASM("ui/uivector", func_003A5360);

INCLUDE_ASM("ui/uivector", func_003A5380);

INCLUDE_ASM("ui/uivector", func_003A5390);

INCLUDE_ASM("ui/uivector", func_003A53B0);

INCLUDE_ASM("ui/uivector", func_003A53C0);

extern "C" unsigned char func_003A53E0(void* self)
{
    return *(unsigned char*)((char*)self + 0x95);
}

extern "C" unsigned char func_003A53E8(void* self)
{
    return *(unsigned char*)((char*)self + 0x98);
}

extern "C" unsigned char func_003A53F0(void* self)
{
    return *(unsigned char*)((char*)self + 0x96);
}

extern "C" void func_003A53F8(void* self, char value)
{
    *(char*)((char*)self + 0x95) = value;
}

extern "C" void func_003A5400(void* self, char value)
{
    *(char*)((char*)self + 0x96) = value;
}

extern "C" void func_003A5408(void* self, char value)
{
    *(char*)((char*)self + 0x98) = value;
}

extern "C" int func_003A5410(void* self)
{
    return *(int*)((char*)self + 0xA0);
}

extern "C" unsigned char func_003A5418(void* self)
{
    return *(unsigned char*)((char*)self + 0x95);
}

INCLUDE_ASM("ui/uivector", func_003A5420);

INCLUDE_ASM("ui/uivector", func_003A5468);

//100%
extern void* D_00494868[];
extern "C" void func_0039BE70(void* self);
// PORT: the unit declares func_0039FC48 with one parameter; the real body is a
// destructor taking (self, flags). Bound to the same symbol with an asm label.
extern "C" void* func_0039FC48_dtor(void* self, int flags) __asm__("func_0039FC48");

extern "C" void func_003A54B8(void* self, int flags)
{
    *(void***)((char*)self + 8) = D_00494868;
    func_0039BE70(self);
    func_0039FC48_dtor(self, flags);
}

//100%
int func_003A5500(void* self)
{
    return 0x15;
}

//100%
extern "C" void* func_003A5508(void* self)
{
    *(unsigned char*)((char*)self + 0x1) &= 0xc0;
    *(unsigned char*)((char*)self + 0x0) &= 0xc0;
    return self;
}

extern "C" int func_003A5528(void* self)
{
    return *(int*)((char*)self + 0x78);
}

INCLUDE_ASM("ui/uivector", func_003A5530);

extern "C" int func_003A5580(void* self)
{
    return *(int*)((char*)self + 0x7C);
}

INCLUDE_ASM("ui/uivector", func_003A5588);

INCLUDE_ASM("ui/uivector", func_003A55D8);

//100%
extern void* D_00494798[];
extern "C" void func_0039A670(void* self);
// PORT: the unit declares func_0039FC48 with one parameter; the real body is a
// destructor taking (self, flags). Bound to the same symbol with an asm label.
extern "C" void* func_0039FC48_dtor(void* self, int flags) __asm__("func_0039FC48");

extern "C" void func_003A5658(void* self, int flags)
{
    *(void***)((char*)self + 8) = D_00494798;
    func_0039A670(self);
    func_0039FC48_dtor(self, flags);
}

//100%
int func_003A56A0(void* self)
{
    return 0x12;
}

INCLUDE_ASM("ui/uivector", func_003A56A8);

INCLUDE_ASM("ui/uivector", func_003A56C8);

//100%
extern "C" void func_003A56D8(void* self, int enable) {
    ((sUIVectorFlags*)((char*)self + 0x74))->bit2 = enable;
}

INCLUDE_ASM("ui/uivector", func_003A56F8);

//100%
extern "C" void func_003A5708(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x74))->bit5 = enable;
}

INCLUDE_ASM("ui/uivector", func_003A5728);

INCLUDE_ASM("ui/uivector", func_003A5738);

INCLUDE_ASM("ui/uivector", func_003A5758);

INCLUDE_ASM("ui/uivector", func_003A5780);

extern "C" unsigned char func_003A57A8(void* self)
{
    return *(unsigned char*)((char*)self + 0x319);
}

extern "C" unsigned char func_003A57B0(void* self)
{
    return *(unsigned char*)((char*)self + 0x318);
}

INCLUDE_ASM("ui/uivector", func_003A57B8);

extern "C" void func_003A5800(void* self, int value)
{
    *(int*)((char*)self + 0xC) = value;
}

extern "C" int func_003A5808(void* self)
{
    return *(int*)((char*)self + 0xC);
}

extern "C" int func_003A5810(void* self)
{
    return *(int*)((char*)self + 0x10);
}

extern "C" void func_003A5818(void* self, char value)
{
    *(char*)((char*)self + 0x14) = value;
}

extern "C" signed char func_003A5820(void* self)
{
    return *(signed char*)((char*)self + 0x14);
}

extern "C" void func_003A5828(void* self, char value)
{
    *(char*)((char*)self + 0x15) = value;
}

extern "C" unsigned char func_003A5830(void* self)
{
    return *(unsigned char*)((char*)self + 0x15);
}

extern "C" int func_003A5838(void* self)
{
    return *(int*)((char*)self + 0x18);
}

extern "C" void func_003A5840(void* self, int value)
{
    *(int*)((char*)self + 0x18) = value;
}

INCLUDE_ASM("ui/uivector", func_003A5848);

INCLUDE_ASM("ui/uivector", func_003A5858);

INCLUDE_ASM("ui/uivector", func_003A5878);

INCLUDE_ASM("ui/uivector", func_003A5888);

INCLUDE_ASM("ui/uivector", func_003A58A8);

INCLUDE_ASM("ui/uivector", func_003A58B8);

INCLUDE_ASM("ui/uivector", func_003A58D8);

INCLUDE_ASM("ui/uivector", func_003A58E8);

INCLUDE_ASM("ui/uivector", func_003A5908);

INCLUDE_ASM("ui/uivector", func_003A5918);

INCLUDE_ASM("ui/uivector", func_003A5940);

INCLUDE_ASM("ui/uivector", func_003A5950);

INCLUDE_ASM("ui/uivector", func_003A5970);

INCLUDE_ASM("ui/uivector", func_003A5980);

INCLUDE_ASM("ui/uivector", func_003A59A0);

INCLUDE_ASM("ui/uivector", func_003A59B0);

INCLUDE_ASM("ui/uivector", func_003A59C0);

INCLUDE_ASM("ui/uivector", func_003A59E0);

INCLUDE_ASM("ui/uivector", func_003A5A10);

INCLUDE_ASM("ui/uivector", func_003A5A48);

INCLUDE_ASM("ui/uivector", func_003A5A58);

INCLUDE_ASM("ui/uivector", func_003A5A78);

INCLUDE_ASM("ui/uivector", func_003A5A88);

//100%
void func_003A5AA8(void* self)
{
}

//100%
int func_003A5AB0(void* self)
{
    return 0;
}

//100%
int func_003A5AB8(void* self)
{
    return 0;
}

//100%
int func_003A5AC0(void* self)
{
    return 0;
}

//100%
void func_003A5AC8(void* self)
{
}

//100%
void func_003A5AD0(void* self)
{
}

//100%
int func_003A5AD8(void* self)
{
    return 0;
}

extern "C" void func_003A5AE0(void* self, int value)
{
    *(int*)((char*)self + 0x20) = value;
}

extern "C" int func_003A5AE8(void* self)
{
    return *(int*)((char*)self + 0x20);
}

extern "C" int func_003A5AF0(void* self)
{
    return *(int*)((char*)self + 0x40);
}

extern "C" void func_003A5AF8(void* self, char value)
{
    *(char*)((char*)self + 0x44) = value;
}

extern "C" signed char func_003A5B00(void* self)
{
    return *(signed char*)((char*)self + 0x44);
}

//100%
void func_003A5B08(void* self)
{
}

//100%
void func_003A5B10(void* self)
{
}

//100%
void func_003A5B18(void* self)
{
}

//100%
int func_003A5B20(void* self)
{
    return 0x101;
}

//100%
int func_003A5B28(void* self)
{
    return 0x101;
}

//100%
int func_003A5B30(void* self)
{
    return 0x101;
}

//100%
int func_003A5B38(void* self)
{
    return 0;
}

//100%
int func_003A5B40(void* self)
{
    return 0;
}

//100%
void operator_delete(int* ptr);
extern "C" void func_003A4868(void* self);

extern void* D_00494CC0[];

extern "C" void func_003A5B48(int* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_00494CC0;
    func_003A4868(self);
    if (flags & 1) {
        operator_delete(self);
    }
}

INCLUDE_ASM("ui/uivector", func_003A5B98);

INCLUDE_ASM("ui/uivector", func_003A5C08);

INCLUDE_ASM("ui/uivector", func_003A5C98);

INCLUDE_ASM("ui/uivector", func_003A5CA8);

INCLUDE_ASM("ui/uivector", func_003A5CC0);

INCLUDE_ASM("ui/uivector", func_003A5CD0);

INCLUDE_ASM("ui/uivector", func_003A5CF0);

extern "C" void* func_003A5D10(void* self)
{
    return (char*)self + 0x40;
}

INCLUDE_ASM("ui/uivector", func_003A5D18);

INCLUDE_ASM("ui/uivector", func_003A5D38);

INCLUDE_ASM("ui/uivector", func_003A5D48);

extern "C" int func_003A5D58(void* self)
{
    return *(int*)((char*)self + 0xD0);
}

INCLUDE_ASM("ui/uivector", func_003A5D60);

//100%
extern void* D_00494588[];
extern "C" void cUIText_deleteText(void* self);
// PORT: the unit declares func_0039FC48 with one parameter; the real body is a
// destructor taking (self, flags). Bound to the same symbol with an asm label.
extern "C" void* func_0039FC48_dtor(void* self, int flags) __asm__("func_0039FC48");

extern "C" void func_003A5DA8(void* self, int flags)
{
    *(void***)((char*)self + 8) = D_00494588;
    cUIText_deleteText(self);
    func_0039FC48_dtor(self, flags);
}

//100%
int func_003A5DF0(void* self)
{
    return 0x17;
}

INCLUDE_ASM("ui/uivector", func_003A5DF8);

INCLUDE_ASM("ui/uivector", func_003A5E18);

INCLUDE_ASM("ui/uivector", func_003A5E28);

INCLUDE_ASM("ui/uivector", func_003A5E38);

INCLUDE_ASM("ui/uivector", func_003A5E48);

INCLUDE_ASM("ui/uivector", func_003A5E58);

//100%
extern "C" void func_003A5E68(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x74))->bit2 = enable;
}

//100%
extern "C" void func_003A5E88(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x74))->bit3 = enable;
}

//100%
extern "C" void func_003A5EA8(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x74))->bit4 = enable;
}

//100%
extern "C" void func_003A5EC8(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x74))->bit5 = enable;
}

//100%
extern "C" void func_003A5EE8(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x74))->bit6 = enable;
}

INCLUDE_ASM("ui/uivector", func_003A5F08);

INCLUDE_ASM("ui/uivector", func_003A5F30);

extern "C" int func_003A5F58(void* self)
{
    return *(int*)((char*)self + 0xB0);
}

INCLUDE_ASM("ui/uivector", func_003A5F60);

INCLUDE_ASM("ui/uivector", func_003A5FA0);

//100%
int func_003A6000(void* self)
{
    return 0x21;
}

extern "C" void func_003A6008(void* self, int value)
{
    *(int*)((char*)self + 0x74) = value;
}

extern "C" void func_003A6010(void* self, int value)
{
    *(int*)((char*)self + 0x78) = value;
}

extern "C" void func_003A6018(void* self, int value)
{
    *(int*)((char*)self + 0x7C) = value;
}

extern "C" void func_003A6020(void* self, int value)
{
    *(int*)((char*)self + 0x80) = value;
}

extern "C" void func_003A6028(void* self, int value)
{
    *(int*)((char*)self + 0x84) = value;
}

extern "C" void func_003A6030(void* self, int value)
{
    *(int*)((char*)self + 0x88) = value;
}

INCLUDE_ASM("ui/uivector", func_003A6038);

INCLUDE_ASM("ui/uivector", func_003A6088);

extern "C" int func_003A60D8(void* self)
{
    return *(int*)((char*)self + 0xC0);
}

INCLUDE_ASM("ui/uivector", func_003A60E0);

INCLUDE_ASM("ui/uivector", func_003A6120);

//100%
int func_003A6168(void* self)
{
    return 0x19;
}

extern "C" void func_003A6170(void* self, int value)
{
    *(int*)((char*)self + 0x74) = value;
}

extern "C" void func_003A6178(void* self, int value)
{
    *(int*)((char*)self + 0x78) = value;
}

extern "C" int func_003A6180(void* self)
{
    return *(int*)((char*)self + 0x7C);
}

extern "C" void func_003A6188(void* self, int value)
{
    *(int*)((char*)self + 0x80) = value;
}

INCLUDE_ASM("ui/uivector", func_003A6190);

INCLUDE_ASM("ui/uivector", func_003A61D0);

//100%
int func_003A6220(void* self)
{
    return 0x16;
}

extern "C" int func_003A6228(void* self)
{
    return *(int*)((char*)self + 0x74);
}

extern "C" void func_003A6230(void* self, int value)
{
    *(int*)((char*)self + 0x74) = value;
}

extern "C" void func_003A6238(void* self, int value)
{
    *(int*)((char*)self + 0x78) = value;
}

extern "C" void func_003A6240(void* self, int value)
{
    *(int*)((char*)self + 0x88) = value;
}

extern "C" void func_003A6248(void* self, int value)
{
    *(int*)((char*)self + 0x8C) = value;
}

INCLUDE_ASM("ui/uivector", func_003A6250);

//100%
extern void* D_00494278[];
extern void* D_00494C98[];
extern void* D_00494CC0[];
extern "C" void func_003A4868(void* self);
extern "C" void func_003A18C0(void* self);
// PORT: the unit declares func_00397948 as returning void*; its body (util/list) is void.
extern "C" void func_00397948_v(void* self) __asm__("func_00397948");
// PORT: the unit declares func_0039FC48 with one parameter; the real body is a
// destructor taking (self, flags). Bound to the same symbol with an asm label.
extern "C" void* func_0039FC48_dtor(void* self, int flags) __asm__("func_0039FC48");

extern "C" void func_003A62A0(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_00494278;
    func_003A18C0(self);
    *(void***)((char*)self + 0xA4) = D_00494C98;
    func_00397948_v((char*)self + 0x8C);
    *(void***)((char*)self + 0xA0) = D_00494CC0;
    func_003A4868((char*)self + 0x98);
    *(void***)((char*)self + 0x94) = D_00494CC0;
    func_003A4868((char*)self + 0x8C);
    func_0039FC48_dtor(self, flags);
}

//100%
int func_003A6330(void* self)
{
    return 0x20;
}

INCLUDE_ASM("ui/uivector", func_003A6338);

INCLUDE_ASM("ui/uivector", func_003A6358);

INCLUDE_ASM("ui/uivector", func_003A6368);

//100%
extern "C" void func_003A6378(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x74))->bit3 = enable;
}

//100%
extern "C" void func_003A6398(void* self, int enable)
{
    ((sUIVectorFlags*)((char*)self + 0x74))->bit4 = enable;
}

INCLUDE_ASM("ui/uivector", func_003A63B8);

INCLUDE_ASM("ui/uivector", func_003A63E0);

INCLUDE_ASM("ui/uivector", func_003A6400);

INCLUDE_ASM("ui/uivector", func_003A6420);

INCLUDE_ASM("ui/uivector", func_003A6440);

INCLUDE_ASM("ui/uivector", func_003A6460);

INCLUDE_ASM("ui/uivector", func_003A6480);

INCLUDE_ASM("ui/uivector", func_003A6490);

INCLUDE_ASM("ui/uivector", func_003A64A0);

extern "C" void func_003A64C8(void* self, int value)
{
    *(int*)((char*)self + 0xB8) = value;
}

extern "C" void func_003A64D0(void* self, short value)
{
    *(short*)((char*)self + 0xC6) = value;
}

extern "C" unsigned short func_003A64D8(void* self)
{
    return *(unsigned short*)((char*)self + 0xC6);
}

extern "C" unsigned short func_003A64E0(void* self)
{
    return *(unsigned short*)((char*)self + 0xC8);
}

extern "C" void func_003A64E8(void* self, short value)
{
    *(short*)((char*)self + 0xC4) = value;
}

extern "C" unsigned short func_003A64F0(void* self)
{
    return *(unsigned short*)((char*)self + 0xC4);
}

INCLUDE_ASM("ui/uivector", func_003A64F8);

//100%
int func_003A6540(void* self)
{
    return 0x18;
}

extern "C" void* func_003A3F48(int, int);

INCLUDE_ASM("ui/uivector", func_003A6548);

INCLUDE_ASM("ui/uivector", func_003A65A0);

INCLUDE_ASM("ui/uivector", func_003A65B8);

INCLUDE_ASM("ui/uivector", func_003A65D8);

INCLUDE_ASM("ui/uivector", func_003A65E8);

INCLUDE_ASM("ui/uivector", func_003A6600);

INCLUDE_ASM("ui/uivector", func_003A6618);

//99.38%
void* func_003A6648(void* self)
{
    return func_003A3F48(1, 0xffff);
}

extern "C" void* func_003A3F48(int, int);

//99.38%
void* func_003A6668(void* self)
{
    return func_003A3F48(0, 0xffff);
}

