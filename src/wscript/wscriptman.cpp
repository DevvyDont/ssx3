#include "common.h"

INCLUDE_ASM("wscript/wscriptman", cWScriptMan_addProcess);

INCLUDE_ASM("wscript/wscriptman", cWScriptMan_addProcess1);

INCLUDE_ASM("wscript/wscriptman", func_00309750);

INCLUDE_ASM("wscript/wscriptman", func_00309798);

INCLUDE_ASM("wscript/wscriptman", func_003097E0);

INCLUDE_ASM("wscript/wscriptman", func_00309848);

INCLUDE_ASM("wscript/wscriptman", func_003098B0);

INCLUDE_ASM("wscript/wscriptman", func_00309918);

INCLUDE_ASM("wscript/wscriptman", func_00309990);

INCLUDE_ASM("wscript/wscriptman", func_003099F8);

INCLUDE_ASM("wscript/wscriptman", func_00309A60);

INCLUDE_ASM("wscript/wscriptman", func_00309AA0);

INCLUDE_ASM("wscript/wscriptman", func_00309B00);

INCLUDE_ASM("wscript/wscriptman", func_00309B70);

INCLUDE_ASM("wscript/wscriptman", func_00309BA8);

INCLUDE_ASM("wscript/wscriptman", func_00309C88);

INCLUDE_ASM("wscript/wscriptman", func_00309D20);

INCLUDE_ASM("wscript/wscriptman", func_00309DD0);

INCLUDE_ASM("wscript/wscriptman", func_00309E50);

INCLUDE_ASM("wscript/wscriptman", func_00309F18);

INCLUDE_ASM("wscript/wscriptman", func_0030A060);

INCLUDE_ASM("wscript/wscriptman", func_0030A270);

INCLUDE_ASM("wscript/wscriptman", func_0030A298);

INCLUDE_ASM("wscript/wscriptman", func_0030A2E8);

INCLUDE_ASM("wscript/wscriptman", func_0030A310);

INCLUDE_ASM("wscript/wscriptman", func_0030A3A0);

INCLUDE_ASM("wscript/wscriptman", func_0030A460);

INCLUDE_ASM("wscript/wscriptman", func_0030A548);

INCLUDE_ASM("wscript/wscriptman", func_0030A598);

INCLUDE_ASM("wscript/wscriptman", func_0030A5C0);

INCLUDE_ASM("wscript/wscriptman", func_0030A610);

INCLUDE_ASM("wscript/wscriptman", func_0030A688);

INCLUDE_ASM("wscript/wscriptman", func_0030A6D8);

INCLUDE_ASM("wscript/wscriptman", func_0030A700);

INCLUDE_ASM("wscript/wscriptman", func_0030A728);

INCLUDE_ASM("wscript/wscriptman", func_0030A868);

INCLUDE_ASM("wscript/wscriptman", func_0030AC98);

INCLUDE_ASM("wscript/wscriptman", func_0030ADA8);

INCLUDE_ASM("wscript/wscriptman", func_0030AEB8);

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

INCLUDE_ASM("wscript/wscriptman", func_0030BA80);

INCLUDE_ASM("wscript/wscriptman", func_0030BAC8);

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

INCLUDE_ASM("wscript/wscriptman", func_0030C468);

INCLUDE_ASM("wscript/wscriptman", func_0030C4A8);

INCLUDE_ASM("wscript/wscriptman", func_0030C6C8);

INCLUDE_ASM("wscript/wscriptman", func_0030C700);

INCLUDE_ASM("wscript/wscriptman", func_0030C738);

INCLUDE_ASM("wscript/wscriptman", func_0030C760);

INCLUDE_ASM("wscript/wscriptman", func_0030C790);

INCLUDE_ASM("wscript/wscriptman", func_0030C7C0);

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

INCLUDE_ASM("wscript/wscriptman", func_0030D420);

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

INCLUDE_ASM("wscript/wscriptman", func_0030D4B8);

INCLUDE_ASM("wscript/wscriptman", func_0030D540);

INCLUDE_ASM("wscript/wscriptman", func_0030D840);

