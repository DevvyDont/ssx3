#include "common.h"

INCLUDE_ASM("wscript/wscriptman", cWScriptMan_addProcess);

INCLUDE_ASM("wscript/wscriptman", cWScriptMan_addProcess1);

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309750);
#ifdef SKIP_ASM
struct sVEntry00309750 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00309750(void* self, void* obj)
{
    void* saved = *(void**)((char*)self + 0x2A4);
    *(void**)((char*)self + 0x2A4) = obj;
    sVEntry00309750* vt = *(sVEntry00309750**)((char*)obj + 0x5C);
    vt[4].fn((char*)obj + vt[4].delta);
    *(void**)((char*)self + 0x2A4) = saved;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309798);
#ifdef SKIP_ASM
struct sVEntry00309798 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00309798(void* self, void* obj)
{
    void* saved = *(void**)((char*)self + 0x2A4);
    *(void**)((char*)self + 0x2A4) = obj;
    sVEntry00309798* vt = *(sVEntry00309798**)((char*)obj + 0x5C);
    vt[6].fn((char*)obj + vt[6].delta);
    *(void**)((char*)self + 0x2A4) = saved;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_003097E0);
#ifdef SKIP_ASM
struct sWSOpData {
    int a;
    int b;
};

struct sWSOp {
    int op;
    sWSOpData data;
};

struct sWSOpList {
    int count;
    sWSOp ops[1];
};

static inline void sWSOpList_push(sWSOpList* l, sWSOp o)
{
    l->ops[l->count++] = o;
}

extern "C" void func_003097E0(void* self, int v)
{
    sWSOp o;
    o.op = 0x70000;
    o.data.a = v;
    sWSOpList_push((sWSOpList*)((char*)self + 0x40), o);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309848);
#ifdef SKIP_ASM
extern "C" void func_00309848(void* self)
{
    sWSOp o;
    o.op = 0x70001;
    sWSOpList_push((sWSOpList*)((char*)self + 0x40), o);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_003098B0);
#ifdef SKIP_ASM
extern "C" void func_003098B0(void* self)
{
    sWSOp o;
    o.op = 0x70002;
    sWSOpList_push((sWSOpList*)((char*)self + 0x40), o);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309918);
#ifdef SKIP_ASM
extern "C" void func_00309918(void* self, sWSOpData* d)
{
    sWSOp o;
    o.op = 0x70003;
    o.data = *d;
    sWSOpList_push((sWSOpList*)((char*)self + 0x40), o);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309990);
#ifdef SKIP_ASM
extern "C" void func_00309990(void* self)
{
    sWSOp o;
    o.op = 0x70004;
    sWSOpList_push((sWSOpList*)((char*)self + 0x40), o);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_003099F8);
#ifdef SKIP_ASM
extern "C" void func_003099F8(void* self)
{
    sWSOp o;
    o.op = 0x70005;
    sWSOpList_push((sWSOpList*)((char*)self + 0x40), o);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309A60);
#ifdef SKIP_ASM
struct cWScriptListHead;
struct cWScriptListNode;
extern "C" int func_0030B2D0(cWScriptListHead* self, cWScriptListNode* node);
int func_0030B260(void* self, void* a1);

extern "C" void func_00309A60(void* self, cWScriptListNode* node)
{
    func_0030B2D0((cWScriptListHead*)((char*)self + 0x2B4), node);
    func_0030B260((char*)self + 0x2B8, node);
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_00309AA0);

INCLUDE_ASM("wscript/wscriptman", func_00309B00);

//100%
INCLUDE_ASM("wscript/wscriptman", func_00309B70);
#ifdef SKIP_ASM
extern "C" int func_003A6D18(void*, int*);

static inline bool func_00309B70_valid(int id) { return id != -1; }

extern "C" int func_00309B70(void* self, int id)
{
    if (func_00309B70_valid(id)) {
        return func_003A6D18(*(void**)((char*)self + 0x28C), &id);
    }
    return 0;
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_00309BA8);

INCLUDE_ASM("wscript/wscriptman", func_00309C88);

INCLUDE_ASM("wscript/wscriptman", func_00309D20);

INCLUDE_ASM("wscript/wscriptman", func_00309DD0);

INCLUDE_ASM("wscript/wscriptman", func_00309E50);

INCLUDE_ASM("wscript/wscriptman", func_00309F18);

INCLUDE_ASM("wscript/wscriptman", func_0030A060);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030A270);
#ifdef SKIP_ASM
extern "C" int func_003A6B78(void* world, int id, int flag);

extern "C" int func_0030A270(void* self, int id)
{
    return func_003A6B78(*(void**)((char*)self + 0x28C), id, 1) != -1;
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030A298);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030A2E8);
#ifdef SKIP_ASM
extern "C" int func_003A6B78(void* world, int id, int flag);

extern "C" int func_0030A2E8(void* self, int id)
{
    return func_003A6B78(*(void**)((char*)self + 0x28C), id, 3) != -1;
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030A310);

INCLUDE_ASM("wscript/wscriptman", func_0030A3A0);

INCLUDE_ASM("wscript/wscriptman", func_0030A460);

INCLUDE_ASM("wscript/wscriptman", func_0030A548);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030A598);
#ifdef SKIP_ASM
extern "C" int func_003A6B78(void* world, int id, int flag);

extern "C" int func_0030A598(void* self, int id)
{
    return func_003A6B78(*(void**)((char*)self + 0x28C), id, 4) != -1;
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030A5C0);

INCLUDE_ASM("wscript/wscriptman", func_0030A610);

INCLUDE_ASM("wscript/wscriptman", func_0030A688);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030A6D8);
#ifdef SKIP_ASM
extern "C" void func_00307EC0(void* p);

extern "C" void func_0030A6D8(void* self)
{
    void* p = *(void**)((char*)self + 0x2A4);
    if (p != 0) {
        func_00307EC0(p);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030A700);
#ifdef SKIP_ASM
extern "C" void func_00307F58(void* p);

extern "C" void func_0030A700(void* self)
{
    void* p = *(void**)((char*)self + 0x2A4);
    if (p != 0) {
        func_00307F58(p);
    }
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030A728);

INCLUDE_ASM("wscript/wscriptman", func_0030A868);

INCLUDE_ASM("wscript/wscriptman", func_0030AC98);

INCLUDE_ASM("wscript/wscriptman", func_0030ADA8);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030AEB8);
#ifdef SKIP_ASM
struct sVEntry0030AEB8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* func_0030AEB8(void* self)
{
    void* obj = *(void**)((char*)self + 0x2A4);
    if (obj != 0) {
        sVEntry0030AEB8* vt = *(sVEntry0030AEB8**)((char*)obj + 0x5C);
        if (vt[3].fn((char*)obj + vt[3].delta) != 0) {
            return *(void**)((char*)self + 0x2A4);
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030AF08);

struct cWScriptListNode {
    char pad_0x00[0x18];
    void* next;
};

struct cWScriptListHead {
    void* head;
};

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B058__FPv);
#ifdef SKIP_ASM
void* func_0030B058(void* self)
{
    ((cWScriptListHead*)self)->head = 0;
    return self;
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030B068);

INCLUDE_ASM("wscript/wscriptman", func_0030B0E8);

INCLUDE_ASM("wscript/wscriptman", func_0030B1B8);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B208);
#ifdef SKIP_ASM
extern "C" void* func_0030B208(void* self)
{
    cWScriptListNode* p = (cWScriptListNode*)((cWScriptListHead*)self)->head;
    if (p != 0) {
        ((cWScriptListHead*)self)->head = p->next;
    }
    return p;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B228);
#ifdef SKIP_ASM
extern "C" int func_0030B228(cWScriptListHead* self)
{
    int n = 0;
    cWScriptListNode* p = (cWScriptListNode*)self->head;
    while (p != 0) {
        p = (cWScriptListNode*)p->next;
        n++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B260__FPvT0);
#ifdef SKIP_ASM
int func_0030B260(void* self, void* a1)
{
    int t0 = (int)((cWScriptListHead*)self)->head;
    ((cWScriptListNode*)a1)->next = (void*)t0;
    ((cWScriptListHead*)self)->head = a1;
    return t0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B270);
#ifdef SKIP_ASM
extern "C" void func_0030B270(cWScriptListHead* self, cWScriptListNode* node)
{
    node->next = 0;
    cWScriptListNode* p = (cWScriptListNode*)self->head;
    if (p != 0) {
        while (p->next != 0) {
            p = (cWScriptListNode*)p->next;
        }
        p->next = node;
    } else {
        self->head = node;
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B2C0__FPvT0);
#ifdef SKIP_ASM
int func_0030B2C0(void* self, void* a1)
{
    int t0 = (int)((cWScriptListHead*)self)->head;
    ((cWScriptListNode*)a1)->next = (void*)t0;
    ((cWScriptListHead*)self)->head = a1;
    return t0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B2D0);
#ifdef SKIP_ASM
extern "C" int func_0030B2D0(cWScriptListHead* self, cWScriptListNode* node)
{
    if (node == self->head) {
        self->head = node->next;
        return 1;
    }
    cWScriptListNode* p = (cWScriptListNode*)self->head;
    if (p != 0) {
        cWScriptListNode* prev = p;
        do {
            p = (cWScriptListNode*)p->next;
            if (p == node) {
                prev->next = p->next;
                return 1;
            }
            prev = p;
        } while (p != 0);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B320);
#ifdef SKIP_ASM
extern "C" void* func_0030B320(void* self, int id)
{
    void* p = *(void**)self;
    while (p != 0) {
        if (*(int*)((char*)p + 0x20) == id) {
            return p;
        }
        p = *(void**)((char*)p + 0x18);
    }
    return 0;
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030B388);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B428);
#ifdef SKIP_ASM
extern "C" int func_0030B428(cWScriptListHead* self, cWScriptListNode* node)
{
    cWScriptListNode* p;
    for (p = (cWScriptListNode*)self->head; p != 0; p = (cWScriptListNode*)p->next) {
        if (p == node) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B4B8__FPv);
#ifdef SKIP_ASM
void func_0030B4B8(void* self)
{
}
#endif

// self+0x4, +0x10, +0x1c, +0x28 are 0xc-byte slots of shape
// { int enabled; int valueB; int valueC; } (see cWScriptSlot below);
// kept as raw offsets here since struct-pointer codegen regresses
// these below their current objdiff match on this compiler.
struct cWScriptSlot {
    int enabled;
    int valueB;
    int valueC;
};

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B4C0__FPvii);
#ifdef SKIP_ASM
void func_0030B4C0(void* self, int b, int c)
{
    *(int*)((char*)self + 0x4) = 1;
    *(int*)((char*)self + 0x8) = b;
    *(int*)((char*)self + 0xc) = c;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B4D8__FPvii);
#ifdef SKIP_ASM
void func_0030B4D8(void* self, int b, int c)
{
    *(int*)((char*)self + 0x10) = 1;
    *(int*)((char*)self + 0x14) = b;
    *(int*)((char*)self + 0x18) = c;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B4F0__FPvii);
#ifdef SKIP_ASM
void func_0030B4F0(void* self, int b, int c)
{
    *(int*)((char*)self + 0x1c) = 1;
    *(int*)((char*)self + 0x20) = b;
    *(int*)((char*)self + 0x24) = c;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B508__FPviT0);
#ifdef SKIP_ASM
void func_0030B508(void* self, int c, void* src)
{
    *(int*)((char*)self + 0x28) = 1;
    *(int*)((char*)self + 0x30) = c;
    *(int*)((char*)self + 0x2c) = *(int*)((char*)src + 0x78);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B520);
#ifdef SKIP_ASM
struct cWScriptPair {
    int valueB;
    int valueC;
};

extern "C" void func_0030B520(void* self, cWScriptPair* src)
{
    *(int*)((char*)self + 0x34) = 1;
    *(cWScriptPair*)((char*)self + 0x38) = *src;
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030B540);

INCLUDE_ASM("wscript/wscriptman", func_0030B658);

INCLUDE_ASM("wscript/wscriptman", func_0030B6F8);

INCLUDE_ASM("wscript/wscriptman", func_0030B758);

INCLUDE_ASM("wscript/wscriptman", func_0030B7F8);

extern "C" void* func_0030B320(void*, int);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030B898__FPvT0);
#ifdef SKIP_ASM
int func_0030B898(void* self, void* a1)
{
    return (func_0030B320((char*)self + 0x2b8, *(int*)((char*)a1 + 0x20)) != 0);
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030B8C0);

INCLUDE_ASM("wscript/wscriptman", func_0030B928);

INCLUDE_ASM("wscript/wscriptman", func_0030B9A0);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030BA80);
#ifdef SKIP_ASM
struct func_0030BA80_sEntry {
    int id;
    char pad04[0x10C - 4];
};

struct func_0030BA80_sMan {
    char pad000[0x2C0];
    func_0030BA80_sEntry entries[1];
};

extern "C" int func_0030C468(void* entry);

extern "C" int func_0030BA80(func_0030BA80_sMan* self, int id)
{
    func_0030BA80_sEntry* e = self->entries;
    int i;
    for (i = 0; i < 1; i++, e++) {
        if (e->id == id) {
            return func_0030C468(e);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030BAC8);
#ifdef SKIP_ASM
struct func_0030BAC8_sEntry {
    int id;
    char pad04[0x10C - 4];
};

struct func_0030BAC8_sMan {
    char pad000[0x2C0];
    func_0030BAC8_sEntry entries[1];
};

extern "C" void func_0030C4A8(void* e, int a1, int a2);

extern "C" void func_0030BAC8(func_0030BAC8_sMan* self, int id, int a1, int a2)
{
    func_0030BAC8_sEntry* e = self->entries;
    int i;
    for (i = 0; i < 1; i++, e++) {
        if (e->id == id) {
            func_0030C4A8(e, a1, a2);
            return;
        }
    }
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030BB10);

INCLUDE_ASM("wscript/wscriptman", func_0030BC80);

INCLUDE_ASM("wscript/wscriptman", func_0030BD20);

INCLUDE_ASM("wscript/wscriptman", func_0030BEE8);

INCLUDE_ASM("wscript/wscriptman", func_0030BFC0);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C390);
#ifdef SKIP_ASM
struct cWScriptIdTable {
    unsigned int id;
    int a;
    int b;
    unsigned int entries[64];
};

extern "C" void func_0030C390(cWScriptIdTable* self)
{
    int i;
    self->id = 0xFFFFFFFF;
    self->a = 0;
    self->b = 0;
    for (i = 0; i < 64; i++) {
        self->entries[i] = 0xFFFFFFFF;
    }
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030C3E0);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C468);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int func_00153498(void* iface, int a1, int id);

extern "C" int func_0030C468(void* entry)
{
    return func_00153498(cBE_getInterface_Fv(cBE_getBE(), 10), 0, *(int*)entry);
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030C4A8);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C6C8);
#ifdef SKIP_ASM
class func_0030C6C8_cObj {
public:
    // slot N at vtable offset N*8
    virtual int v01(void* a, int b);
};

extern "C" int func_0030C6C8(void* a, func_0030C6C8_cObj* obj)
{
    return obj->v01(a, 0x10C);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C700);
#ifdef SKIP_ASM
class func_0030C700_cObj {
public:
    // slot N at vtable offset N*8
    virtual int v01(void* a, int b);
    virtual int v02(void* a, int b);
};

extern "C" int func_0030C700(void* a, func_0030C700_cObj* obj)
{
    return obj->v02(a, 0x10C);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C738);
#ifdef SKIP_ASM
extern "C" void func_0030C760(void* self);

extern "C" void* func_0030C738(void* self)
{
    func_0030C760(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C760);
#ifdef SKIP_ASM
extern "C" void func_003E6448(void* dst, int value, int size);

extern "C" void func_0030C760(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 4) = 0;
    func_003E6448((char*)self + 8, 0, 0xC0);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C790);
#ifdef SKIP_ASM
extern "C" void func_003E6448(void* dst, int value, int size);

extern "C" void func_0030C790(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 4) = 0;
    func_003E6448((char*)self + 8, 0, 0xC0);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C7C0);
#ifdef SKIP_ASM
// Same layout as cWScriptQueue / cWScriptQueueEntry, which the unit defines
// further down (above func_0030C820). Renamed here only to avoid a redefinition.
struct cWSQEntry {
    int a;
    int b;
    int c;
};

struct cWSQueue {
    int head;
    int tail;
    cWSQEntry entries[16];
};

extern "C" void func_0030C7C0(cWSQueue* self, int a, int b, int c)
{
    self->entries[self->tail].a = a;
    self->entries[self->tail].b = b;
    self->entries[self->tail].c = c;
    self->tail = (self->tail + 1) % 16;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C820);
#ifdef SKIP_ASM
struct cWScriptQueueEntry {
    int a;
    int b;
    int c;
};

struct cWScriptQueue {
    int head;
    int tail;
    cWScriptQueueEntry entries[16];
};

extern "C" cWScriptQueueEntry* func_0030C820(cWScriptQueue* self)
{
    int head = self->head;
    if (head == self->tail) {
        return 0;
    }
    self->head = (head + 1) % 16;
    return &self->entries[head];
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030C870);
#ifdef SKIP_ASM
extern "C" cWScriptQueueEntry* func_0030C870(cWScriptQueue* self)
{
    if (self->head == self->tail) {
        return 0;
    }
    return &self->entries[self->head];
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030C8D8);

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D3D8__FPv);
#ifdef SKIP_ASM
int func_0030D3D8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D420);
#ifdef SKIP_ASM
extern void* D_00489AE0[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_0030D420(void* self, int flags)
{
    *(void***)((char*)self + 0x40) = D_00489AE0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D480__FPv);
#ifdef SKIP_ASM
int func_0030D480(void* self)
{
    return 0x1;
}
#endif

extern "C" void* func_0030C8D8(int, int);

//99.38%
INCLUDE_ASM("wscript/wscriptman", func_0030D498__FPv);
#ifdef SKIP_ASM
void* func_0030D498(void* self)
{
    return func_0030C8D8(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptman", func_0030D4B8);
#ifdef SKIP_ASM
struct sWSVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sWSSlot8 {
    int a;
    int b;
};

extern "C" void* func_0030D4B8(void* self)
{
    sWSVec4 v;
    *(int*)((char*)self + 0x38) = 0;
    *(int*)((char*)self + 0x30) = 0;
    *(int*)((char*)self + 0x34) = 0;
    v.x = 1.0f;
    v.y = 1.0f;
    v.z = 1.0f;
    v.w = 1.0f;
    *(sWSVec4*)((char*)self + 0x140) = v;
    *(int*)((char*)self + 0x2C) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x3C) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x12C) = 0;
    *(int*)((char*)self + 0x130) = 0;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x28) = 0;
    *(int*)((char*)self + 0x20) = 0;
    for (int i = 3; i >= 0; i--) {
        ((sWSSlot8*)((char*)self + 0x44))[i].a = 0;
    }
    return self;
}
#endif

INCLUDE_ASM("wscript/wscriptman", func_0030D540);

INCLUDE_ASM("wscript/wscriptman", func_0030D840);

