#include "common.h"

INCLUDE_ASM("world/wscriptcache", cWScriptCache_init);

INCLUDE_ASM("world/wscriptcache", func_003AC8F0);

//100%
INCLUDE_ASM("world/wscriptcache", func_003ACA10);
#ifdef SKIP_ASM
struct sWScriptCacheItem {
    char pad_0x00[0xC];
};

extern "C" void func_003ACC50(sWScriptCacheItem* e, int index, int value);

extern "C" void func_003ACA10(void* self, int index, int value)
{
    sWScriptCacheItem* p = *(sWScriptCacheItem**)((char*)self + 0x4);
    func_003ACC50(&p[index], index, value);
}
#endif

// 0xc-byte elements reached through a pointer at self+0x4
struct sWScriptCacheEntry {
    char pad_0x00[0x8];
    int field_0x8;
};

//100%
INCLUDE_ASM("world/wscriptcache", func_003ACA38);
#ifdef SKIP_ASM
extern "C" int func_003ACA38(void* self, int a1)
{
    sWScriptCacheEntry* p = *(sWScriptCacheEntry**)((char*)self + 0x4);
    return p[a1].field_0x8;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ACA50__FPv);
#ifdef SKIP_ASM
void* func_003ACA50(void* self)
{
    int t0 = 0;
    *(short*)self = (short)t0;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = t0;
    *(short*)((char*)self + 0x2) = (short)t0;
    return self;
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003ACA70);

INCLUDE_ASM("world/wscriptcache", func_003ACC50);

INCLUDE_ASM("world/wscriptcache", func_003ACC90);

INCLUDE_ASM("world/wscriptcache", func_003ACCD8);

INCLUDE_ASM("world/wscriptcache", func_003AD120);

INCLUDE_ASM("world/wscriptcache", func_003AD188);

INCLUDE_ASM("world/wscriptcache", func_003AD198);

INCLUDE_ASM("world/wscriptcache", func_003AD1A8);

extern "C" void* func_002E27E8(void* self);

//100%
INCLUDE_ASM("world/wscriptcache", func_003AD230__FPv);
#ifdef SKIP_ASM
void* func_003AD230(void* self)
{
    return func_002E27E8(self);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003AD290);

INCLUDE_ASM("world/wscriptcache", func_003ADC18);

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADC48__FPv);
#ifdef SKIP_ASM
int func_003ADC48(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADC50);
#ifdef SKIP_ASM
extern "C" int func_003ADC50(void* self, int i)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        return (*(int**)((char*)data + 0x3C))[i];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADC80);
#ifdef SKIP_ASM
extern "C" int func_003ADC80(void* self)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        if (data != 0) {
            return *(int*)((char*)data + 0x10);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADCB0);
#ifdef SKIP_ASM
extern "C" int func_003ADCB0(void* self)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        if (data != 0) {
            return *(int*)((char*)data + 0x14);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADCE0);
#ifdef SKIP_ASM
extern "C" int func_003ADCE0(void* self)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        if (data != 0) {
            return *(int*)((char*)data + 0x28);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADD10);
#ifdef SKIP_ASM
extern "C" int func_003ADD10(void* self)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        if (data != 0) {
            return *(int*)((char*)data + 0x2C);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADD40);
#ifdef SKIP_ASM
extern "C" int func_003ADD40(void* self)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        if (data != 0) {
            return *(int*)((char*)data + 0x20);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADD70);
#ifdef SKIP_ASM
extern "C" int func_003ADD70(void* self)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        if (data != 0) {
            return *(int*)((char*)data + 0x24);
        }
    }
    return 0;
}
#endif

extern "C" void* func_003AD290(int, int);

//99.38%
INCLUDE_ASM("world/wscriptcache", func_003ADDA0__FPv);
#ifdef SKIP_ASM
void* func_003ADDA0(void* self)
{
    return func_003AD290(1, 0xffff);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003ADDC0);

INCLUDE_ASM("world/wscriptcache", func_003ADE08);

INCLUDE_ASM("world/wscriptcache", func_003ADEC8);

INCLUDE_ASM("world/wscriptcache", func_003AE300);

INCLUDE_ASM("world/wscriptcache", func_003AE450);

INCLUDE_ASM("world/wscriptcache", func_003AE6C8);

INCLUDE_ASM("world/wscriptcache", func_003AE780);

//100%
INCLUDE_ASM("world/wscriptcache", func_003AE860);
#ifdef SKIP_ASM
extern "C" int func_003AEFA8(void* p);

extern "C" int func_003AE860(void** self)
{
    void* p = *self;
    int r = 1;
    if (p != 0) {
        r = func_003AEFA8(p);
    }
    return r;
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003AE888);

extern "C" void* func_003AE888(void*, int);

//100%
INCLUDE_ASM("world/wscriptcache", func_003AE938__FPv);
#ifdef SKIP_ASM
void* func_003AE938(void* self)
{
    return func_003AE888(self, 0);
}
#endif

extern "C" void* func_003AE888(void*, int);

//100%
INCLUDE_ASM("world/wscriptcache", func_003AE958__FPv);
#ifdef SKIP_ASM
void* func_003AE958(void* self)
{
    return func_003AE888(self, 0x1000);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003AE9A0);

INCLUDE_ASM("world/wscriptcache", func_003AEAD0);

extern "C" void* func_003AEAD0(int, void*);

//99.44%
INCLUDE_ASM("world/wscriptcache", func_003AECB8__FPvT0);
#ifdef SKIP_ASM
void* func_003AECB8(void* self, void* a1)
{
    return func_003AEAD0(*(int*)a1, self);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003AECE0);

extern "C" void* func_003AECE0(int, int);

//99.38%
INCLUDE_ASM("world/wscriptcache", func_003AED20__FPvT0i);
#ifdef SKIP_ASM
void* func_003AED20(void* self, void* a1, int a2)
{
    return func_003AECE0(*(int*)a1, a2);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003AED40);

INCLUDE_ASM("world/wscriptcache", func_003AEE30);

INCLUDE_ASM("world/wscriptcache", func_003AEE98);

INCLUDE_ASM("world/wscriptcache", func_003AEEF8);

INCLUDE_ASM("world/wscriptcache", func_003AEFA8);

extern void* D_00456890[];

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0410__FPv);
#ifdef SKIP_ASM
void* func_003B0410(void* self)
{
    int t0 = 0;
    *(int*)self = t0;
    *(int*)((char*)self + 0xc) = (int)(void*)D_00456890;
    *(int*)((char*)self + 0x4) = t0;
    return self;
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B0430);

INCLUDE_ASM("world/wscriptcache", func_003B04A0);

INCLUDE_ASM("world/wscriptcache", func_003B04F8);

INCLUDE_ASM("world/wscriptcache", func_003B0538);

INCLUDE_ASM("world/wscriptcache", func_003B0580);

INCLUDE_ASM("world/wscriptcache", func_003B05C0);

INCLUDE_ASM("world/wscriptcache", func_003B0600);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0680);
#ifdef SKIP_ASM
class func_003B0680_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int a);
};

extern "C" void func_003B0680(void* self, int a)
{
    (*(func_003B0680_cObj**)((char*)self + 0x14))->v06(a);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B06B0);

INCLUDE_ASM("world/wscriptcache", func_003B06F8);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0720__FPv);
#ifdef SKIP_ASM
void* func_003B0720(void* self)
{
    int t0 = 0;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)((char*)self + 0x8) = t0;
    *(int*)self = t0;
    return self;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0738__FPv);
#ifdef SKIP_ASM
void* func_003B0738(void* self)
{
    int t0 = 0;
    *(int*)self = t0;
    *(int*)((char*)self + 0xc) = 2;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)((char*)self + 0x8) = t0;
    return self;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0758);
#ifdef SKIP_ASM
extern "C" void* func_003B0758(void* self, int a1, int a2, int a3, int a4)
{
    *(int*)((char*)self + 0xc) = a4;
    *(int*)self = a1;
    *(int*)((char*)self + 0x4) = a2;
    *(int*)((char*)self + 0x8) = a3;
    return self;
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B0770);

extern "C" void* func_003B0770(int, int);

//99.38%
INCLUDE_ASM("world/wscriptcache", func_003B07B8__FPv);
#ifdef SKIP_ASM
void* func_003B07B8(void* self)
{
    return func_003B0770(1, 0xffff);
}
#endif

extern "C" void* func_003B0770(int, int);

//99.38%
INCLUDE_ASM("world/wscriptcache", func_003B07D8__FPv);
#ifdef SKIP_ASM
void* func_003B07D8(void* self)
{
    return func_003B0770(0, 0xffff);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B07F8);

INCLUDE_ASM("world/wscriptcache", func_003B08E0);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0948);
#ifdef SKIP_ASM
extern void* D_00456850[];

struct sWSNode2 {
    sWSNode2* next;
    sWSNode2* prev;
};

static inline void wsListInit(sWSNode2* h)
{
    h->next = (sWSNode2*)3;
    h->prev = (sWSNode2*)3;
    h->next = h;
    h->prev = h;
    h->next = h;
    h->prev = h;
}

extern "C" void* func_003B0948(void* self)
{
    *(void***)self = D_00456850;
    wsListInit((sWSNode2*)((char*)self + 0x18));
    wsListInit((sWSNode2*)((char*)self + 0x20));
    *(int*)((char*)self + 0x2C) = 0;
    *(int*)((char*)self + 0x78) = 0;
    return self;
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B09A0);

INCLUDE_ASM("world/wscriptcache", func_003B0B10);

INCLUDE_ASM("world/wscriptcache", func_003B0B40);

INCLUDE_ASM("world/wscriptcache", func_003B0C58);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0FA0__FPv);
#ifdef SKIP_ASM
int func_003B0FA0(void* self)
{
    return *(int*)((char*)self + 0x10);
}
#endif

// padded past the 8-byte gp-relative threshold so the compiler emits
// absolute lui/lo addressing like the target, instead of assuming small data
struct sD_00509508 { float value; int pad[2]; };
extern sD_00509508 D_00509508;

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0FA8);
#ifdef SKIP_ASM
extern "C" float func_003B0FA8()
{
    return D_00509508.value;
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B0FB8);

INCLUDE_ASM("world/wscriptcache", func_003B1050);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B10D0);
#ifdef SKIP_ASM
struct sWSNode {
    sWSNode* next;
    sWSNode* prev;

    void unlink()
    {
        sWSNode* n = next;
        sWSNode* p = prev;
        p->next = n;
        n->prev = p;
        prev = next = (sWSNode*)0xB;
    }
};

struct sWSList {
    sWSNode head;

    void addTail(sWSNode* n)
    {
        sWSNode* tail = head.prev;
        tail->next = n;
        head.prev = n;
        n->prev = tail;
        n->next = &head;
    }
};

struct sWSItemBase {
    int a;
    int b;
};

struct sWSItem : sWSItemBase, sWSNode {
    int refs; // 0x10
};

struct sWSPool {
    char pad00[0x18];
    sWSList used; // 0x18
    sWSList free; // 0x20
};

extern "C" sWSItem* func_003B10D0(sWSPool* self)
{
    sWSNode* n = self->free.head.next;
    if (n != &self->free.head) {
        sWSItem* item;
        n->unlink();
        item = static_cast<sWSItem*>(n);
        self->used.addTail(item);
        item->refs = 1;
        return item;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003B1140);
#ifdef SKIP_ASM
extern "C" void func_003B1140(sWSPool* self, sWSItem* item)
{
    if (--item->refs > 0) {
        return;
    }
    item->unlink();
    self->free.addTail(item);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B11A0);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B11E8__FPv);
#ifdef SKIP_ASM
void func_003B11E8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003B11F0);
#ifdef SKIP_ASM
extern int D_00495448[];

extern "C" int func_003B11F0(int v)
{
    unsigned int i;
    for (i = 0; i < 2; i++) {
        if (D_00495448[i] == v) {
            return 1;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B1228);

// padded past the 8-byte gp-relative threshold so the compiler emits
// absolute lui/lo addressing like the target
struct sD_0050A088 { void* ptr; int pad[2]; };
extern sD_0050A088 D_0050A088;

//100%
INCLUDE_ASM("world/wscriptcache", func_003B1258);
#ifdef SKIP_ASM
extern "C" unsigned int func_003B1258(int a0)
{
    return *(unsigned int*)((char*)D_0050A088.ptr + 0x4) >> (unsigned int)(-a0);
}
#endif

extern "C" void* func_003B1300(int);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B1270__FPv);
#ifdef SKIP_ASM
void* func_003B1270(void* self)
{
    return func_003B1300(1);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B1290);

INCLUDE_ASM("world/wscriptcache", func_003B1300);

INCLUDE_ASM("world/wscriptcache", func_003B1340);

INCLUDE_ASM("world/wscriptcache", func_003B13A8);

INCLUDE_ASM("world/wscriptcache", func_003B13D8);

INCLUDE_ASM("world/wscriptcache", func_003B1498);

INCLUDE_ASM("world/wscriptcache", func_003B14F0);

INCLUDE_ASM("world/wscriptcache", func_003B16E0);

INCLUDE_ASM("world/wscriptcache", func_003B17A8);

INCLUDE_ASM("world/wscriptcache", func_003B1988);

INCLUDE_ASM("world/wscriptcache", func_003B1AB0);

INCLUDE_ASM("world/wscriptcache", func_003B1CC0);

INCLUDE_ASM("world/wscriptcache", func_003B1D58);

INCLUDE_ASM("world/wscriptcache", func_003B1EE0);

INCLUDE_ASM("world/wscriptcache", func_003B1FC8);

INCLUDE_ASM("world/wscriptcache", func_003B20B8);

INCLUDE_ASM("world/wscriptcache", func_003B2230);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/wscriptcache", func_003B22F8);
#ifdef SKIP_ASM
extern int D_00495608[];

extern "C" void func_003B22F8(void)
{
    func_003B11E8(D_00495608);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B2318);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B2360__FPv);
#ifdef SKIP_ASM
void* func_003B2360(void* self)
{
    return func_003B1300(1);
}
#endif

extern "C" void* func_003B1498(void* self);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B2380__FPv);
#ifdef SKIP_ASM
void* func_003B2380(void* self)
{
    return func_003B1498(self);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B23A0);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B2440);
#ifdef SKIP_ASM
extern int D_00509648[];
extern int D_00509554[];
extern int D_00509558[];
extern int D_0044C378[];
extern int D_0044C37C[];
extern int D_0044C380[];
extern int D_0044C3C8[];
extern int D_0044C3CC[];
extern int D_0050A098[];

extern "C" void func_003B2440(void)
{
    if (D_0050A088.ptr != D_00509648) {
        return;
    }
    int last;
    if (D_00509558[0] != 3 && D_00509554[0] != (last = D_0044C3CC[0])) {
        if (D_0044C3C8[0] != 0) {
            D_0044C3C8[0] = 0;
            D_0044C378[0] += 0x400;
        }
        if (D_00509554[0] < last && D_0044C380[0] == 0) {
            D_0044C3C8[0] = 1;
        }
        D_0044C380[0] = 0;
        D_0044C3CC[0] = D_00509554[0];
    }
    D_0050A098[0] = D_0044C378[0] + D_00509554[0];
    if (D_0044C3C8[0] != 0 && D_0044C3CC[0] >= D_00509554[0]) {
        D_0050A098[0] += 0x400;
    }
    D_0044C37C[0] = (D_0044C37C[0] < D_0050A098[0]) ? D_0050A098[0] : D_0044C37C[0];
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003B2528);
#ifdef SKIP_ASM
struct func_003B2528_sKey {
    char c;
};

extern "C" char func_003B3DA8(int key);
extern "C" void func_003B39D8(func_003B2528_sKey* k);

extern "C" void func_003B2528(int key)
{
    func_003B2528_sKey k;
    if (key == 0) {
        key = 0x20;
    }
    k.c = func_003B3DA8(key);
    func_003B39D8(&k);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B2558);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B2688);
#ifdef SKIP_ASM
extern "C" int func_003B2688(int v)
{
    switch (v) {
    case 8:
        return 0x100;
    case 4:
        return 0x10;
    default:
        return 0;
    }
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B26B8);

INCLUDE_ASM("world/wscriptcache", func_003B27C8);

INCLUDE_ASM("world/wscriptcache", func_003B2A68);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B2B68);
#ifdef SKIP_ASM
extern void (*D_0044C458[])(char* buf);

extern "C" void func_003B2B68(char* buf)
{
    buf[0] = 0;
    D_0044C458[0](buf);
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B2B90);

INCLUDE_ASM("world/wscriptcache", func_003B2E78);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B3308);
#ifdef SKIP_ASM
// PORT: 64-bit `long` compares (ld); `long` is 8 bytes on this compiler.
extern "C" int func_003B3308(void* buf, int size, int type)
{
    if (type == 3) {
        unsigned long* p = (unsigned long*)buf;
        int n = size / 16;
        int i;
        for (i = 0; i < n; i++) {
            unsigned long v = *p++;
            if (v != 0xFFFFFFFF) {
                return 1;
            }
            if (*p != v) {
                return 1;
            }
            p += 3;
        }
    } else {
        unsigned short* q = (unsigned short*)buf;
        int n = size / 16;
        int i;
        for (i = 0; i < n; i++) {
            if (*q != 0xFFFF) {
                return 1;
            }
            q += 16;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B33B0);

