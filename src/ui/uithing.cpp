#include "common.h"

struct cUIThing {
    char pad_0x00[0x3C];
    unsigned short* mEvents; // 0x3C
};

//100%
INCLUDE_ASM("ui/uithing", cUIThing_getKeyframerEvent__FP8cUIThingi);
#ifdef SKIP_ASM
unsigned short cUIThing_getKeyframerEvent(cUIThing* self, int x)
{
    signed char idx = (signed char)x;
    if (self->mEvents != 0) {
        return self->mEvents[idx];
    }
    return 0xFFFF;
}
#endif

INCLUDE_ASM("ui/uithing", func_0039FFA0);

//100%
INCLUDE_ASM("ui/uithing", func_003A0000);
#ifdef SKIP_ASM
extern "C" void func_003975E0(void*, void*, unsigned char, unsigned short);
extern "C" void func_003974B0(void*, unsigned char);

extern "C" void func_003A0000(void* self, unsigned short ev)
{
    void* a = *(void**)((char*)self + 0xC);
    if (a != 0) {
        func_003975E0(a, self, *(unsigned char*)((char*)self + 0x10), ev);
        func_003974B0(*(void**)((char*)self + 0xC), *(unsigned char*)((char*)self + 0x10));
    }
}
#endif

INCLUDE_ASM("ui/uithing", func_003A0048);

INCLUDE_ASM("ui/uithing", func_003A0158);

//100%
INCLUDE_ASM("ui/uithing", func_003A0290);
#ifdef SKIP_ASM
struct sUIVec3 {
    float x, y, z;
};

struct sUIThingPos {
    char pad0[0x40];
    sUIThingPos* parent;    // 0x40
    sUIVec3 pos;            // 0x44
};

extern "C" void func_003A0290(sUIThingPos* self, sUIVec3* out)
{
    *out = self->pos;
    for (sUIThingPos* p = self->parent; p != 0; p = p->parent) {
        sUIVec3 v = p->pos;
        out->x += v.x;
        out->y += v.y;
        out->z += v.z;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uithing", func_003A0318);
#ifdef SKIP_ASM
extern "C" int func_003A0318(void* self, int a1)
{
    self = (char*)self + ((signed char)a1 << 2);
    return *(int*)((char*)self + 0x6c);
}
#endif

//100%
INCLUDE_ASM("ui/uithing", func_003A0330);
#ifdef SKIP_ASM
extern "C" void func_003A0330(void* self, int a1, int a2)
{
    self = (char*)self + ((signed char)a1 << 2);
    *(int*)((char*)self + 0x6c) = a2;
}
#endif

//100%
INCLUDE_ASM("ui/uithing", func_003A0348);
#ifdef SKIP_ASM
extern "C" void* func_0039FB30(void* self);
extern void* D_00494588[];

struct func_003A0348_sQuad {
    int a, b, c, d;
};
extern func_003A0348_sQuad D_004C66C8;

struct func_003A0348_sObj {
    char base[0x74];
    unsigned char flags;
    char pad75[0x88 - 0x75];
    func_003A0348_sQuad q88;
    func_003A0348_sQuad q98;
    int fA8;
    int fAC;
    int fB0;
    int fB4;
    short fB8;
    short fBA;
};

extern "C" func_003A0348_sObj* func_003A0348(func_003A0348_sObj* self)
{
    func_0039FB30(self);
    *(void***)((char*)self + 8) = D_00494588;
    self->flags = (self->flags | 4) & 0x87;
    self->fA8 = 0;
    self->fAC = 0;
    self->fB0 = 0;
    self->fB4 = 0;
    self->fB8 = 0;
    self->fBA = 0;
    self->q88 = D_004C66C8;
    self->q98 = D_004C66C8;
    return self;
}
#endif

INCLUDE_ASM("ui/uithing", func_003A03F0);

//100%
INCLUDE_ASM("ui/uithing", func_003A04F0);
#ifdef SKIP_ASM
extern "C" int func_003A04F0(void* self)
{
    void* p = *(void**)((char*)*(void**)((char*)*(void**)((char*)self + 0x5c) + 0xd0) + 0x10);
    if (p != 0) {
        return *(int*)((char*)*(void**)((char*)p + 0x8) + (*(unsigned char*)((char*)self + 0x74) & 3) * 12 + 0xC);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uithing", func_003A0528);
#ifdef SKIP_ASM
extern "C" void func_003A0D00(void* self, unsigned short* str);
extern "C" void cUIText_setUnicodeStringPrivate(void* self, unsigned short* str);

struct func_003A0528_sVEntry {
    short delta;
    short index;
    void* fn;
};

extern "C" void func_003A0528(void* self)
{
    int id = *(int*)((char*)self + 0xB0);
    if (id != 0) {
        void* a = *(void**)((char*)self + 0x5C);
        void* b = *(void**)((char*)a + 0xD0);
        void* c = *(void**)((char*)b + 0x10);
        char* loc = *(char**)((char*)c + 0x10);
        if (loc != 0) {
            func_003A0528_sVEntry* vt = *(func_003A0528_sVEntry**)(loc + 4);
            unsigned short* str = ((unsigned short* (*)(void*, int))vt[4].fn)(loc + vt[4].delta, id);
            if (str != 0) {
                int f14 = *(int*)((char*)self + 0x14) >> 7;
                if ((f14 & 1) || ((*(int*)((char*)self + 0x74) >> 6) & 1)) {
                    func_003A0D00(self, str);
                } else {
                    cUIText_setUnicodeStringPrivate(self, str);
                }
            }
            func_003A0528_sVEntry* vt2 = *(func_003A0528_sVEntry**)((char*)self + 8);
            ((void (*)(void*, int))vt2[5].fn)((char*)self + vt2[5].delta, 1);
        }
    }
}
#endif

