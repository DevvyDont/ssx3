#include "common.h"

INCLUDE_ASM("ui/uimenu", cUIMenu_setSelected);

INCLUDE_ASM("ui/uimenu", func_0039AE98);

INCLUDE_ASM("ui/uimenu", func_0039B000);

//100%
INCLUDE_ASM("ui/uimenu", func_0039B6A0);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);
extern "C" void func_0039DA20(void* p, unsigned char a, unsigned char b, unsigned char c);

struct func_0039B6A0_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, float, float);
};

extern "C" void func_0039B6A0(void* self, float x, float y)
{
    void* p = *(void**)((char*)self + 0x9C);
    if (p != 0) {
        func_0039DA20(p, *(unsigned char*)((char*)self + 0x98), *(unsigned char*)((char*)self + 0x97), *(unsigned char*)((char*)self + 0x96));
    }
    void* n = cList_first((cList*)((char*)self + 0x74));
    if (n != 0) {
        do {
            if ((*(int*)((char*)n + 0x14) >> 6) & 1) {
                func_0039B6A0_sVEntry* vt = *(func_0039B6A0_sVEntry**)((char*)n + 8);
                vt[0x11].fn((char*)n + vt[0x11].delta, *(float*)((char*)self + 0x44) + x, *(float*)((char*)self + 0x48) + y);
            }
            n = *(void**)((char*)n + 4);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039B760);
#ifdef SKIP_ASM
extern "C" char func_003979C0(void*);

extern "C" void func_0039B760(void* self, char a1)
{
    *(char*)((char*)self + 0x95) = 0;
    *(char*)((char*)self + 0x96) = a1;
    *(int*)((char*)self + 0x90) |= 4;
    *(char*)((char*)self + 0x97) = func_003979C0((char*)self + 0x74);
    *(char*)((char*)self + 0x98) = 0;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039B7B0);
#ifdef SKIP_ASM
extern "C" void* func_00397870(void* list, unsigned char i);

struct func_0039B7B0_sVEntry {
    short delta;
    short index;
    int (*fn)(void*, void*, int, unsigned char);
};

extern "C" unsigned char func_0039B7B0(void* self, unsigned char start)
{
    unsigned char i = start == 0xFF ? 0 : start;
    for (; i < *(unsigned char*)((char*)self + 0x96); i++) {
        void* item = func_00397870((char*)self + 0x74, i);
        if (*(int*)((char*)self + 0x90) & 4) {
            char* mgr = *(char**)(*(char**)((char*)self + 0x5C) + 0xD0);
            func_0039B7B0_sVEntry* vt = *(func_0039B7B0_sVEntry**)(mgr + 8);
            if (vt[20].fn(mgr + vt[20].delta, self, 1, i)) {
                return i;
            }
        } else {
            int f = *(int*)((char*)item + 0x14);
            if (((f >> 5) & 1) == 0) {
                return i;
            }
        }
    }
    return 0xFF;
}
#endif

INCLUDE_ASM("ui/uimenu", cUIMenu_setSelectedByIndex);

extern "C" void* func_0039FE00(void* self);

//100%
INCLUDE_ASM("ui/uimenu", func_0039BAB0__FPv);
#ifdef SKIP_ASM
void* func_0039BAB0(void* self)
{
    return func_0039FE00(self);
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039BAD0);
#ifdef SKIP_ASM
class func_0039BAD0_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04(int);
};

struct func_0039BAD0_s {
    char pad0[0x14];
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    char pad18[0x78 - 0x18];
    func_0039BAD0_cVirt* a;
    func_0039BAD0_cVirt* b;
};

extern "C" void func_0039BAD0(func_0039BAD0_s* self, int on)
{
    self->b1 = (on != 0);
    self->a->v04(on);
    func_0039BAD0_cVirt* b = self->b;
    if (b != 0) {
        b->v04(on);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039BB50);
#ifdef SKIP_ASM
struct func_0039BB50_sFlags {
    unsigned int pad0 : 6;
    unsigned int on : 1;
};

struct func_0039BB50_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0039BB50(void* self, int on)
{
    ((func_0039BB50_sFlags*)((char*)self + 0x14))->on = (on != 0);
    void* a = *(void**)((char*)self + 0x78);
    if (a != 0) {
        func_0039BB50_sVEntry* vt = *(func_0039BB50_sVEntry**)((char*)a + 8);
        vt[9].fn((char*)a + vt[9].delta, on);
    }
    void* b = *(void**)((char*)self + 0x7C);
    if (b != 0) {
        func_0039BB50_sVEntry* vt = *(func_0039BB50_sVEntry**)((char*)b + 8);
        vt[9].fn((char*)b + vt[9].delta, on);
    }
}
#endif

INCLUDE_ASM("ui/uimenu", func_0039BBD8);

//100%
INCLUDE_ASM("ui/uimenu", func_0039BD38);
#ifdef SKIP_ASM
class func_0039BD38_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void*);
};

extern "C" int func_0039BD38(void* a, func_0039BD38_cVirt* obj)
{
    return obj->v02(a);
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039BD68);
#ifdef SKIP_ASM
class func_0039BD68_cPred {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void*);
};

class func_0039BD68_cNode {
public:
    char pad[0x8];
    // vptr at 0x8
    virtual void v01();
    virtual void v02();
    virtual void* v03(func_0039BD68_cPred*);
};

extern "C" void* func_0039BD68(void* self, func_0039BD68_cPred* pred)
{
    if (pred->v02(self)) {
        return self;
    }
    func_0039BD68_cNode* child = *(func_0039BD68_cNode**)((char*)self + 0x78);
    if (child != 0 && child->v03(pred) != 0) {
        return *(void**)((char*)self + 0x78);
    }
    func_0039BD68_cNode* next = *(func_0039BD68_cNode**)((char*)self + 0x7C);
    if (next != 0) {
        return next->v03(pred);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039BE10);
#ifdef SKIP_ASM
class func_0039BE10_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
};

extern "C" void func_0039BE10(void* self)
{
    func_0039BE10_cVirt* a = *(func_0039BE10_cVirt**)((char*)self + 0x78);
    if (a != 0) {
        a->v16();
    }
    func_0039BE10_cVirt* b = *(func_0039BE10_cVirt**)((char*)self + 0x7C);
    if (b != 0) {
        b->v16();
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039BE70);
#ifdef SKIP_ASM
class func_0039BE70_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01(int);
};

extern "C" void func_0039BE70(void* self)
{
    func_0039BE70_cVirt* a = *(func_0039BE70_cVirt**)((char*)self + 0x78);
    if (a != 0) {
        a->v01(3);
        *(func_0039BE70_cVirt**)((char*)self + 0x78) = 0;
    }
    func_0039BE70_cVirt* b = *(func_0039BE70_cVirt**)((char*)self + 0x7C);
    if (b != 0) {
        b->v01(3);
        *(func_0039BE70_cVirt**)((char*)self + 0x7C) = 0;
    }
}
#endif

INCLUDE_ASM("ui/uimenu", func_0039BED8);

//100%
INCLUDE_ASM("ui/uimenu", func_0039C130);
#ifdef SKIP_ASM
struct func_0039C130_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, float, float);
};

extern "C" void func_0039C130(void* self, float x, float y)
{
    void* a = *(void**)((char*)self + 0x78);
    if (a != 0) {
        func_0039C130_sVEntry* e = &(*(func_0039C130_sVEntry**)((char*)a + 8))[17];
        e->fn((char*)a + e->delta, x + *(float*)((char*)self + 0x44), y + *(float*)((char*)self + 0x48));
    }
    void* b = *(void**)((char*)self + 0x7C);
    if (b != 0) {
        func_0039C130_sVEntry* e = &(*(func_0039C130_sVEntry**)((char*)b + 8))[17];
        e->fn((char*)b + e->delta, x + *(float*)((char*)self + 0x44), y + *(float*)((char*)self + 0x48));
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039C1C8);
#ifdef SKIP_ASM
extern "C" void func_003A0000(void* self, unsigned short ev);

class func_0039C1C8_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19(unsigned short);
};

extern "C" void func_0039C1C8(void* self, unsigned short ev)
{
    func_003A0000(self, ev);
    func_0039C1C8_cVirt* a = *(func_0039C1C8_cVirt**)((char*)self + 0x78);
    if (a != 0) {
        a->v19(ev);
    }
    func_0039C1C8_cVirt* b = *(func_0039C1C8_cVirt**)((char*)self + 0x7C);
    if (b != 0) {
        b->v19(ev);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039C240);
#ifdef SKIP_ASM
extern "C" void func_0039FC48(void* self, int flags);
extern void* D_00494408[];

struct func_0039C240_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0039C240(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_00494408;
    void* a = *(void**)((char*)self + 0x74);
    if (a != 0) {
        func_0039C240_sVEntry* vt = *(func_0039C240_sVEntry**)((char*)a + 8);
        vt[1].fn((char*)a + vt[1].delta, 3);
    }
    void* b = *(void**)((char*)self + 0x78);
    if (b != 0) {
        func_0039C240_sVEntry* vt = *(func_0039C240_sVEntry**)((char*)b + 8);
        vt[1].fn((char*)b + vt[1].delta, 3);
    }
    func_0039FC48(self, flags);
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039C2C8);
#ifdef SKIP_ASM
extern "C" void func_0039C2C8(void* self)
{
    func_0039FE00(self);
    *(int*)((char*)self + 0x80) = 10;
    *(int*)((char*)self + 0x84) = 100;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039C300);
#ifdef SKIP_ASM
struct func_0039C300_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, float, float);
};

extern "C" void func_0039C300(void* self, float x, float y)
{
    void* a = *(void**)((char*)self + 0x74);
    if (a != 0 && *(void**)((char*)self + 0x78) != 0) {
        float px = x + *(float*)((char*)self + 0x44);
        float py = y + *(float*)((char*)self + 0x48);
        func_0039C300_sVEntry* vt = *(func_0039C300_sVEntry**)((char*)a + 8);
        vt[17].fn((char*)a + vt[17].delta, px, py);
        void* b = *(void**)((char*)self + 0x78);
        func_0039C300_sVEntry* vt2 = *(func_0039C300_sVEntry**)((char*)b + 8);
        vt2[17].fn((char*)b + vt2[17].delta, px, py);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039C398);
#ifdef SKIP_ASM
extern "C" void func_003975E0(void* anim, void* thing, unsigned char flags, unsigned short ev);
extern "C" void func_003974B0(void* anim, unsigned char flags);

struct func_0039C398_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, unsigned short);
};

extern "C" void func_0039C398(void* self, unsigned short ev)
{
    if (*(void**)((char*)self + 0xC) != 0) {
        func_003975E0(*(void**)((char*)self + 0xC), self, *(unsigned char*)((char*)self + 0x10), ev);
        func_003974B0(*(void**)((char*)self + 0xC), *(unsigned char*)((char*)self + 0x10));
    }
    void* a = *(void**)((char*)self + 0x78);
    if (a != 0) {
        func_0039C398_sVEntry* vt = *(func_0039C398_sVEntry**)((char*)a + 8);
        vt[19].fn((char*)a + vt[19].delta, ev);
    }
    void* b = *(void**)((char*)self + 0x74);
    if (b != 0) {
        func_0039C398_sVEntry* vt = *(func_0039C398_sVEntry**)((char*)b + 8);
        vt[19].fn((char*)b + vt[19].delta, ev);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039C428);
#ifdef SKIP_ASM
struct func_0039C428_sVec3 {
    float x;
    float y;
    float z;
};
extern func_0039C428_sVec3 D_004FF0D8;

class func_0039C428_cWidget {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20(func_0039C428_sVec3*);
    virtual void v21(func_0039C428_sVec3*);
};

extern "C" void func_0039C428(void* self, int pos)
{
    *(int*)((char*)self + 0x7C) = pos;
    func_0039C428_cWidget* a = *(func_0039C428_cWidget**)((char*)self + 0x74);
    func_0039C428_sVec3 v = D_004FF0D8;
    if (a != 0) {
        a->v20(&v);
    }
    if (*(void**)((char*)self + 0x78) != 0) {
        int n = *(int*)((char*)self + 0x80);
        if (n != 0) {
            if (!(*(int*)((char*)self + 0x14) & 1)) {
                v.x *= (float)*(int*)((char*)self + 0x7C) / (float)n;
            } else {
                v.y *= (float)*(int*)((char*)self + 0x7C) / (float)n;
            }
            (*(func_0039C428_cWidget**)((char*)self + 0x78))->v21(&v);
        }
    }
}
#endif

