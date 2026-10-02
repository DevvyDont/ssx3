#include "common.h"

//100%
INCLUDE_ASM("ui/uistate", cUIState_hideObjSafe);
#ifdef SKIP_ASM
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
#endif

//100%
INCLUDE_ASM("ui/uistate", cUIState_showObjSafe);
#ifdef SKIP_ASM
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
#endif

INCLUDE_ASM("ui/uistate", func_0039E9D8);

INCLUDE_ASM("ui/uistate", func_0039EA90);

//100%
INCLUDE_ASM("ui/uistate", func_0039EB60);
#ifdef SKIP_ASM
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
#endif

INCLUDE_ASM("ui/uistate", func_0039EBE0);

//100%
INCLUDE_ASM("ui/uistate", func_0039ECA8__FPv);
#ifdef SKIP_ASM
void func_0039ECA8(void* self)
{
}
#endif

INCLUDE_ASM("ui/uistate", func_0039ECB0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uistate", func_0039F100);
#ifdef SKIP_ASM
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
#endif

//100%
INCLUDE_ASM("ui/uistate", func_0039F190);
#ifdef SKIP_ASM
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
#endif

