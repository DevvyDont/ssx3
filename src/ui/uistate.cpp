#include "common.h"

//100%
struct cList;
struct cListNode;
cListNode* cList_first(cList*);
int cListNode_isSentinel(cListNode*);
int GetHashValue32(char*);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

struct sVEntry39E8B8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void cUIState_hideObjSafe(void* self, char* name)
{
    cListNode* n = cList_first((cList*)((char*)self + 0x24));
    if (n != 0) {
        do {
            void* obj = cUIScreen_getObjectByHashName(n, GetHashValue32(name));
            if (obj != 0) {
                sVEntry39E8B8* vt = *(sVEntry39E8B8**)((char*)obj + 8);
                vt[9].fn((char*)obj + vt[9].delta, 0);
                return;
            }
            n = *(cListNode**)((char*)n + 4);
        } while (!cListNode_isSentinel(n));
    }
}

//100%
struct cList;
struct cListNode;
cListNode* cList_first(cList*);
int cListNode_isSentinel(cListNode*);
int GetHashValue32(char*);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

struct sVEntry39E948 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void cUIState_showObjSafe(void* self, char* name)
{
    cListNode* n = cList_first((cList*)((char*)self + 0x24));
    if (n != 0) {
        do {
            void* obj = cUIScreen_getObjectByHashName(n, GetHashValue32(name));
            if (obj != 0) {
                sVEntry39E948* vt = *(sVEntry39E948**)((char*)obj + 8);
                vt[9].fn((char*)obj + vt[9].delta, 1);
                return;
            }
            n = *(cListNode**)((char*)n + 4);
        } while (!cListNode_isSentinel(n));
    }
}

//100%
extern "C" void func_0039F8C8(void* self);
extern "C" void func_00397948(void*);
extern "C" void func_003A4868(void* self);
void operator_delete(int*);
extern void* D_00494C98[];
extern void* D_00494CC0[];

extern "C" void func_0039E9D8(void* self, int flags)
{
    func_0039F8C8(self);
    *(void***)((char*)self + 0x34) = D_00494C98;
    func_00397948((char*)self + 0x1C);
    *(void***)((char*)self + 0x30) = D_00494CC0;
    func_003A4868((char*)self + 0x28);
    *(void***)((char*)self + 0x24) = D_00494CC0;
    func_003A4868((char*)self + 0x1C);
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

//100%
struct func_0039EA90_sFlags {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
};

struct func_0039EA90_node {
    char pad0[0x4];
    func_0039EA90_node* next; // 0x4
    char pad8[0x1C - 0x8];
    union {
        int flags; // 0x1C
        func_0039EA90_sFlags bits;
    };
};

static inline void func_0039EA90_setB3(func_0039EA90_node* n, int v) { n->bits.b3 = v; }
static inline void func_0039EA90_setB5(func_0039EA90_node* n, int v) { n->bits.b5 = v; }
static inline void func_0039EA90_setId(func_0039EA90_node* n, int v) { n->bits.id = v; }
static inline void func_0039EA90_setB4(func_0039EA90_node* n, int v) { n->bits.b4 = v; }

extern "C" int func_0039EA90(cList* list, signed char id)
{
    func_0039EA90_node* n = (func_0039EA90_node*)cList_first(list);
    if (n != 0) {
        do {
            if ((((unsigned int)n->flags >> 8) & 0x3F) == id && ((n->flags >> 6) & 1) == 0) {
                func_0039EA90_setB3(n, 1);
                func_0039EA90_setB5(n, 1);
                func_0039EA90_setId(n, 2);
                func_0039EA90_setB4(n, 1);
                return 1;
            }
            n = n->next;
        } while (!cListNode_isSentinel((cListNode*)n));
    }
    return 0;
}

//100%
struct cList;
struct cListNode;
cListNode* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

struct func_0039EB60_node {
    char pad0[0x4];
    func_0039EB60_node* next; // 0x4
    char pad8[0x1C - 0x8];
    int flags; // 0x1C
};

extern "C" int func_0039EB60(cList* list, signed char id)
{
    func_0039EB60_node* n = (func_0039EB60_node*)cList_first(list);
    if (n != 0) {
        do {
            if ((((unsigned int)n->flags >> 8) & 0x3F) == id && ((n->flags >> 6) & 1) == 0) {
                return 1;
            }
            n = n->next;
        } while (!cListNode_isSentinel((cListNode*)n));
    }
    return 0;
}

//100%
struct func_0039EBE0_sVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0039EBE0(void* self)
{
    char* n = (char*)cList_first((cList*)self);
    if (n != 0) {
        do {
            if ((*(int*)(n + 0x1C) >> 3) & 1) {
                func_0039EBE0_sVEntry* vt = *(func_0039EBE0_sVEntry**)(n + 8);
                vt[15].fn(n + vt[15].delta);
            }
            n = *(char**)(n + 4);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
    n = (char*)cList_first((cList*)((char*)self + 0x1C));
    if (n != 0) {
        do {
            if ((*(int*)(n + 0x1C) >> 3) & 1) {
                func_0039EBE0_sVEntry* vt = *(func_0039EBE0_sVEntry**)(n + 8);
                vt[15].fn(n + vt[15].delta);
            }
            n = *(char**)(n + 4);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
}

//100%
void func_0039ECA8(void* self)
{
}

//100%
extern "C" void func_0039E868(void* self);
extern "C" void func_0039F7B8(void* self, void* st, int a2, int a3);
extern "C" void cListNode_removeFromList(void* self);
extern "C" void* cUIStateStack_getCurrentState(void* self);

struct func_0039ECB0_sFlags {
    unsigned int b0 : 1;
    unsigned int cnt : 2;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
};

struct func_0039ECB0_sVEntry {
    short delta;
    short index;
    int (*fn)(void*, ...);
};

struct func_0039ECB0_node {
    func_0039ECB0_sVEntry* pad0;
    func_0039ECB0_node* next; // 0x4
    func_0039ECB0_sVEntry* vt; // 0x8
    char padC[0x1C - 0xC];
    union {
        int flags; // 0x1C
        func_0039ECB0_sFlags bits;
    };
    func_0039ECB0_node* owner; // 0x20
};

struct func_0039ECB0_sVEntryV {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

// g++ 2.95 virtual call through a {delta, index, fn} vtable entry; the V form discards the result.
#define VCALL_ECB0(n, i, args...) (n)->vt[i].fn((char*)(n) + (n)->vt[i].delta , ## args)
#define VCALLV_ECB0(n, i, args...) ((func_0039ECB0_sVEntryV*)(n)->vt)[i].fn((char*)(n) + (n)->vt[i].delta , ## args)

extern "C" int func_0039ECB0(void* self)
{
    int popped = 0;
    int any = 0;
    func_0039ECB0_node* n = (func_0039ECB0_node*)cList_first((cList*)self);
    if (n != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            func_0039ECB0_node* next = n->next;
            if ((n->flags >> 5) & 1) {
                switch ((signed char)n->bits.id) {
                case 2:
                    any = 1;
                    VCALLV_ECB0(n, 4);
                    n->bits.id = 4;
                    func_0039E868(n);
                    if (n->flags & 1) {
                        func_0039ECB0_node* o = n->owner;
                        if (o != 0) {
                            func_0039F7B8(self, o, 1, 1);
                            VCALLV_ECB0(o, 17, n, 0x15);
                        }
                    }
                case 4:
                    if (VCALL_ECB0(n, 7, (n->flags >> 4) & 1)) {
                        n->bits.id = 3;
                    }
                    n->bits.b4 = 0;
                    func_0039E868(n);
                    break;
                case 3:
                    if ((n->flags >> 4) & 1) {
                        VCALL_ECB0(n, 7, 1);
                        n->bits.b4 = 0;
                    }
                    VCALLV_ECB0(n, 6, 0);
                    n->bits.id = 5;
                    func_0039E868(n);
                    break;
                case 5:
                    VCALLV_ECB0(n, 12);
                    func_0039E868(n);
                    break;
                case 6:
                    if (VCALL_ECB0(n, 10)) {
                        n->bits.id = 7;
                    }
                    n->bits.b4 = 1;
                    func_0039E868(n);
                    break;
                case 7: {
                    int top;
                    int found = 0;
                    popped = 1;
                    VCALLV_ECB0(n, 11, 0);
                    VCALLV_ECB0(n, 5);
                    n->bits.id = 8;
                    n->bits.b5 = 0;
                    if ((n->flags >> 7) & 1) {
                        cListNode_removeFromList(n);
                    }
                    top = n->flags & 1;
                    if (top) {
                        func_0039ECB0_node* o = n->owner;
                        popped = 0;
                        if (((n->flags >> 14) & 1) == 0) {
                            found = func_0039EB60((cList*)self, 1);
                        } else {
                            found = 0;
                        }
                        if (found == 0) {
                            n->bits.b7 = 1;
                            if (o != 0) {
                                if (o->bits.cnt != 0) {
                                    o->bits.cnt = o->bits.cnt - 1;
                                }
                                o->bits.b5 = o->bits.cnt == 0;
                                VCALLV_ECB0(o, 17, n, 0x16);
                            }
                        } else if (o != 0) {
                            func_0039F7B8(self, o, 0, 1);
                        }
                    }
                    if (n != 0) {
                        if ((n->flags >> 7) & 1) {
                            cListNode_removeFromList(n);
                            VCALLV_ECB0(n, 1, 3);
                        } else {
                            n->bits.b3 = 0;
                        }
                    }
                    if (top == 0 || found != 0) {
                        if (cUIStateStack_getCurrentState(self) == 0 && func_0039EA90((cList*)self, 1) == 0) {
                            func_0039EA90((cList*)self, 8);
                        }
                    }
                    break;
                }
                }
            }
            n = next;
        }
    }
    if (popped) {
        return any ^ 1;
    }
    return 0;
}

//100% - objdiff report; single-function view differs only in a relocation name
extern "C" int func_0039ECB0(void* self);
extern "C" void func_0039E868(void* self);

struct sVEntry39F100 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0039F100(void* self)
{
    if (func_0039ECB0(self) != 0) {
        func_0039ECB0(self);
    }
    cListNode* n = cList_first((cList*)((char*)self + 0x1C));
    if (n != 0) {
        do {
            if ((*(int*)((char*)n + 0x1C) >> 5) & 1) {
                sVEntry39F100* vt = *(sVEntry39F100**)((char*)n + 8);
                vt[0xC].fn((char*)n + vt[0xC].delta);
            }
            func_0039E868(n);
            n = *(cListNode**)((char*)n + 4);
        } while (!cListNode_isSentinel(n));
    }
}

//100%
struct cList;
struct cListNode;
cListNode* cList_first(cList*);

struct func_0039F190_node {
    char pad0[0x4];
    func_0039F190_node* next; // 0x4
    char pad8[0x1C - 0x8];
    unsigned int b0 : 6;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int id : 6;
};

static inline void func_0039F190_setId(func_0039F190_node* n, int v) { n->id = v; }
static inline void func_0039F190_setB7(func_0039F190_node* n, int v) { n->b7 = v; }

extern "C" func_0039F190_node* func_0039F190(cList* list, int count)
{
    func_0039F190_node* n = (func_0039F190_node*)cList_first(list);
    int i;
    for (i = 0; i < count; i++) {
        if (n == 0) {
            goto done;
        }
        func_0039F190_setId(n, 6);
        func_0039F190_setB7(n, 1);
        n = n->next;
    }
done:
    return n;
}


INCLUDE_ASM("ui/uistate", func_0039F200);
