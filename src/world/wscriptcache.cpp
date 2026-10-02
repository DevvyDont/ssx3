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

//100%
INCLUDE_ASM("world/wscriptcache", func_003ACA70);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int*);

extern "C" void func_003ACA70(int* self, int flags)
{
    if (*(void**)((char*)self + 0x8) != 0 && *(short*)((char*)self + 0x2) == 0) {
        cMemMan_free(*(void**)((char*)self + 0x8));
    }
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003ACC50);

//100%
INCLUDE_ASM("world/wscriptcache", func_003ACC90);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

extern "C" void func_003ACC90(void* self)
{
    void* buf = *(void**)((char*)self + 0x8);
    if (buf != 0) {
        if (*(short*)((char*)self + 0x2) == 0) {
            cMemMan_free(buf);
        }
        *(void**)((char*)self + 0x8) = 0;
    }
    *(short*)self = 0;
}
#endif

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

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADC18);
#ifdef SKIP_ASM
extern void* D_00495150[];
void operator_delete(int*);

extern "C" void func_003ADC18(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_00495150;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

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

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADDC0);
#ifdef SKIP_ASM
extern "C" void func_003ADEC8(void* self, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

extern "C" void* func_003ADDC0(void* self, int a1, int a2, int a3, int a4)
{
    *(int*)((char*)self + 0x60) = 0;
    func_003ADEC8(self, a1, 0, a2, 0, 0, 0, a3, a4);
    return self;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADE08);
#ifdef SKIP_ASM
extern "C" void func_003ADEC8(void* self, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

extern "C" void* func_003ADE08(void* self, int a1, int a2, int a3, int a4, int a5, int a6)
{
    *(int*)((char*)self + 0x60) = 0;
    func_003ADEC8(self, a1, 0, a2, a3, 0, a4, a5, a6);
    return self;
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003ADEC8);

INCLUDE_ASM("world/wscriptcache", func_003AE300);

INCLUDE_ASM("world/wscriptcache", func_003AE450);

//100%
INCLUDE_ASM("world/wscriptcache", func_003AE6C8);
#ifdef SKIP_ASM
extern "C" int func_004139F8(float f);
extern "C" int func_003B0600(void* p, int a1);
extern "C" int func_003B0580(void* self);

extern "C" int func_003AE6C8(void* self, float f)
{
    *(float*)((char*)self + 0x48) = f;
    *(int*)((char*)self + 0x74) = func_003B0600(*(void**)((char*)self + 0x64), func_004139F8(f));
    *(int*)((char*)self + 0x44) = func_003B0580(*(void**)((char*)self + 0x64));
    return *(int*)((char*)self + 0x74);
}
#endif

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

//100%
INCLUDE_ASM("world/wscriptcache", func_003AECE0);
#ifdef SKIP_ASM
extern "C" void* func_003E13E8(void* p, int a1);
extern void* (*D_00509434[])(void*);

extern "C" void* func_003AECE0(int self, int a1)
{
    int* obj = (int*)a1;
    void* r = func_003E13E8(*(void**)(self + 0x30), *obj);
    if (obj != 0) {
        r = D_00509434[0](obj);
    }
    return r;
}
#endif

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

//100%
INCLUDE_ASM("world/wscriptcache", func_003AEE30);
#ifdef SKIP_ASM
extern "C" void func_003B7838(void* a);
extern void* (*D_00509434[])(void*);

extern "C" void func_003AEE30(void* self, int flags)
{
    if (*(void**)((char*)self + 0x4) != 0) {
        func_003B7838(*(void**)((char*)self + 0xC));
        D_00509434[0](*(void**)((char*)self + 0x4));
        *(void**)((char*)self + 0x4) = 0;
    }
    if (flags & 1) {
        D_00509434[0](self);
    }
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003AEE98);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B7FB8(int h, int v);

extern "C" int func_003AEE98(void* self, int v)
{
    int r;
    if (*(int*)((char*)self + 0xC) != -1) {
        func_003B58A0();
        r = func_003B7FB8(*(int*)((char*)self + 0xC), v);
        func_003B58D8();
        return r;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003AEEF8);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B7D80(int h, int a1);

extern "C" int func_003AEEF8(void* self)
{
    int r;
    if (*(int*)((char*)self + 0x10) != -1) {
        func_003B58A0();
        r = func_003B7D80(*(int*)((char*)self + 0x10), 0);
        func_003B58D8();
        return r;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003AEFA8);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B7B48(int id, void* out);
extern "C" int func_003B7C40(int id, void* out);

struct func_003AEFA8_sInfo {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xC;
};

extern "C" int func_003AEFA8(void* self)
{
    func_003AEFA8_sInfo b;
    func_003AEFA8_sInfo a;
    func_003B58A0();
    if (func_003B7B48(*(int*)((char*)self + 0xC), &a) >= 0 && func_003B7C40(a.field_0x4, &b) >= 0) {
        func_003B58D8();
        return b.field_0x8 == 0;
    }
    func_003B58D8();
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0430);
#ifdef SKIP_ASM
void* func_003B0738(void* self);
extern void* D_004568A8[];

struct func_003B0430_sKey {
    int v[4];
};

struct func_003B0430_sObj {
    int field_0x0;
    func_003B0430_sKey key;   // 0x4
    int field_0x14;
    void** vtable;            // 0x18
};

extern "C" func_003B0430_sObj* func_003B0430(func_003B0430_sObj* self, func_003B0430_sKey* key)
{
    self->vtable = D_004568A8;
    func_003B0738(&self->key);
    self->key = *key;
    self->field_0x14 = 0;
    self->field_0x0 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003B04A0);
#ifdef SKIP_ASM
extern "C" void func_003B0538(void* self);
extern void* (*D_00509434[])(void*);
extern void* D_004568A8[];

extern "C" void func_003B04A0(void* self, int flags)
{
    *(void***)((char*)self + 0x18) = D_004568A8;
    func_003B0538(self);
    if (flags & 1) {
        D_00509434[0](self);
    }
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003B04F8);
#ifdef SKIP_ASM
class func_003B04F8_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void* a, int b);
    virtual void v03();
    virtual int v04();
    virtual float v05();
};

extern "C" void func_003B04F8(void* self, func_003B04F8_cObj* obj, int a2)
{
    *(func_003B04F8_cObj**)((char*)self + 0x14) = obj;
    *(int*)self = a2;
    obj->v02(self, a2);
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0538);
#ifdef SKIP_ASM
struct sVEntry003B0538 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_003B0538(void* self)
{
    void* obj = *(void**)((char*)self + 0x14);
    if (obj != 0) {
        sVEntry003B0538* vt = *(sVEntry003B0538**)obj;
        vt[1].fn((char*)obj + vt[1].delta, 3);
        *(void**)((char*)self + 0x14) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0580);
#ifdef SKIP_ASM
extern "C" int func_003B0580(void* self)
{
    func_003B04F8_cObj* obj = *(func_003B04F8_cObj**)((char*)self + 0x14);
    if (obj == 0) {
        return 0;
    }
    return obj->v04();
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003B05C0);
#ifdef SKIP_ASM
extern "C" float func_003B05C0(void* self)
{
    func_003B04F8_cObj* obj = *(func_003B04F8_cObj**)((char*)self + 0x14);
    if (obj == 0) {
        return 0.0f;
    }
    return obj->v05();
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0600);
#ifdef SKIP_ASM
class func_003B0600_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int v03(int);
};

struct func_003B0600_sObj {
    int loaded;                                    // 0x0
    void* arg;                                     // 0x4
    void (*load)(func_003B0600_sObj*, void*, func_003B0600_sObj*); // 0x8
    char pad_0xC[0x8];
    func_003B0600_cVirt* obj;                      // 0x14
};

extern "C" int func_003B0600(void* p, int a1)
{
    func_003B0600_sObj* self = (func_003B0600_sObj*)p;
    if (self->obj == 0) {
        if (self->loaded == 0) {
            self->load(self, self->arg, self);
        }
    }
    int r;
    if (self->obj != 0) {
        r = self->obj->v03(a1);
    } else {
        r = 0;
    }
    return r;
}
#endif

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

//100%
INCLUDE_ASM("world/wscriptcache", func_003B06B0);
#ifdef SKIP_ASM
struct func_003B06B0_sReq {
    int pending;
    int arg;
    void (*fn)(func_003B06B0_sReq* self, int arg, int* out);
};

extern "C" int func_003B06B0(func_003B06B0_sReq* self)
{
    int r;
    int v = self->pending;
    if (v != 0) {
        self->pending = 0;
        r = v;
        return v;
    }
    self->fn(self, self->arg, &r);
    return r;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/wscriptcache", func_003B0770);
#ifdef SKIP_ASM
extern char D_00509430[];
extern void* D_0050943C[];

extern "C" void* func_003B0770(int init, int prio)
{
    if (prio == 0xFFFF) {
        if (init != 0) {
            func_003B0410(D_00509430);
        } else {
            D_0050943C[0] = (void*)D_00456890;
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("world/wscriptcache", func_003B08E0);
#ifdef SKIP_ASM
extern "C" void func_003B2B68(char* buf);
extern void* (*D_00509434[])(void*);

extern "C" void func_003B08E0(void* self, int flags)
{
    if (*(void**)((char*)self + 0x4) != 0) {
        func_003B2B68(*(char**)((char*)self + 0x4));
        *(void**)((char*)self + 0x4) = 0;
    }
    *(int*)((char*)self + 0x8) = 7;
    *(int*)((char*)self + 0xC) = 7;
    if (flags & 1) {
        D_00509434[0](self);
    }
}
#endif

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

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0B10);
#ifdef SKIP_ASM
extern "C" void func_003B0B40(void* a, void* b, void* c);

extern "C" int func_003B0B10(void* self, void* a1, void* a2)
{
    func_003B0B40(a2, self, a1);
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("world/wscriptcache", func_003B0FB8);
#ifdef SKIP_ASM
struct sWSPool;
struct sWSItem;
extern "C" sWSItem* func_003B10D0(sWSPool* self);
extern "C" void func_00402A10(void* dma, void* data, int qwc);

extern "C" void* func_003B0FB8(void* self)
{
    sWSItem* item = func_003B10D0((sWSPool*)self);
    char* hdr = *(char**)((char*)item + 0x4);
    char* data;
    if (*(int*)(hdr + 0xC) & 0x1000) {
        data = hdr + *(int*)(hdr + 0x10);
    } else {
        data = hdr + 0x10;
    }
    func_00402A10((char*)self + 0x30, data, *(int*)((char*)self + 0x8) / 16 * *(int*)((char*)self + 0xC) / 16);
    *(int*)((char*)self + 0x10) += 1;
    return item;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/wscriptcache", func_003B1050);
#ifdef SKIP_ASM
extern "C" void* func_003B0FB8(void* self);
extern "C" int func_00402B38(void* sema);

class func_003B1050_cCache {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(void*);
};

extern "C" void* func_003B1050(func_003B1050_cCache* self, unsigned int limit)
{
    void* item = 0;
    do {
        if (item != 0) {
            self->v06(item);
        }
        if (func_00402B38((char*)self + 0x30) != 0) {
            return 0;
        }
        item = func_003B0FB8(self);
    } while (*(unsigned int*)((char*)self + 0x10) < limit);
    return item;
}
#endif

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

//100%
INCLUDE_ASM("world/wscriptcache", func_003B1228);
#ifdef SKIP_ASM
// defined (padded) later in the unit; incomplete here keeps lui/lo addressing
struct sD_0050A088;
extern sD_0050A088 D_0050A088;

extern "C" void func_003B1290(int n);

extern "C" void func_003B1228(void* data)
{
    char* r = *(char**)&D_0050A088;
    *(void**)(r + 0x0) = data;
    *(int*)(r + 0x8) = 0;
    *(int*)(r + 0x4) = 0;
    func_003B1290(0);
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/wscriptcache", func_003B1300);
#ifdef SKIP_ASM
extern "C" void func_003B1290(int n);

// PORT: returns a bit-field value as void* to fit the unit's existing
// `extern "C" void* func_003B1300(int)` declaration (really unsigned int).
extern "C" void* func_003B1300(int n)
{
    unsigned int v = func_003B1258(n);
    func_003B1290(n);
    return (void*)v;
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B1340);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/wscriptcache", func_003B13A8);
#ifdef SKIP_ASM
extern "C" void func_003B1340(void);

extern "C" unsigned int func_003B13A8(void)
{
    unsigned int v = func_003B1258(0x20);
    func_003B1340();
    return v;
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B13D8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/wscriptcache", func_003B1498);
#ifdef SKIP_ASM
extern "C" void func_003B1290(int n);

// PORT: no return value here; the unit declares it void* and callers forward $v0.
void func_003B1498_impl(void* self) __asm__("func_003B1498");

void func_003B1498_impl(void* self)
{
    func_003B1290(*(int*)((char*)D_0050A088.ptr + 0x8) & 7);
    while (func_003B1258(0x18) != 1) {
        func_003B1290(8);
    }
}
#endif

INCLUDE_ASM("world/wscriptcache", func_003B14F0);

INCLUDE_ASM("world/wscriptcache", func_003B16E0);

INCLUDE_ASM("world/wscriptcache", func_003B17A8);

INCLUDE_ASM("world/wscriptcache", func_003B1988);

INCLUDE_ASM("world/wscriptcache", func_003B1AB0);

//100%
INCLUDE_ASM("world/wscriptcache", func_003B1CC0);
#ifdef SKIP_ASM
extern void* D_00509538[];
extern void* D_0050953C[];
extern void* D_00509540[];
extern void* D_00509544[];
extern void* D_00509548[];
extern void* D_0050954C[];
extern void* D_00509550[];
extern char D_004954D0[];
void* func_003B2360(void* self);

extern "C" void func_003B1CC0(void)
{
    D_00509538[0] = func_003B1300(3);
    D_0050953C[0] = func_003B1300(1);
    if (D_0050953C[0] != 0) {
        D_00509540[0] = func_003B1300(8);
        D_00509544[0] = func_003B1300(8);
        D_00509548[0] = func_003B1300(8);
    }
    D_0050954C[0] = func_003B1300(0xE);
    func_003B2360(D_004954D0);
    D_00509550[0] = func_003B1300(0xE);
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/wscriptcache", func_003B2318);
#ifdef SKIP_ASM
// PORT: func_003B1270 takes a void* it never reads; this caller passes nothing, so it binds
// a no-argument declaration to the mangled symbol.
void* func_003B1270_noarg() __asm__("func_003B1270__FPv");

extern "C" int func_003B2318(void)
{
    int n = 0;
    while (func_003B1270_noarg() != 0) {
        func_003B1290(8);
        n++;
    }
    return n;
}
#endif

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

//100%
INCLUDE_ASM("world/wscriptcache", func_003B23A0);
#ifdef SKIP_ASM
extern void* D_00509610[];
extern void* D_00509614[];
extern void* D_00509618[];
extern void* D_0050961C[];
extern void* D_00509620[];
extern void* D_00509624[];
extern char D_00495630[];
extern char D_00495658[];
extern char D_00495688[];
void* func_003B2360(void* self);

extern "C" void func_003B23A0(void)
{
    D_00509610[0] = func_003B1300(1);
    D_00509614[0] = func_003B1300(8);
    D_00509618[0] = func_003B1300(1);
    func_003B1300(7);
    func_003B2360(D_00495630);
    D_0050961C[0] = func_003B1300(0x14);
    func_003B2360(D_00495658);
    D_00509620[0] = func_003B1300(0x16);
    func_003B2360(D_00495688);
    D_00509624[0] = func_003B1300(0x16);
}
#endif

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

