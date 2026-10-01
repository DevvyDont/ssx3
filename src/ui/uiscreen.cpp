#include "common.h"

extern "C" void cListNode_removeFromList(void* self);

struct sDeleteTarget {
    char pad_0x00[0x8];
    short field_0x8;
    char pad_0xA[2];
    void (*fn)(void*, int); // 0xC
};

//100%
INCLUDE_ASM("ui/uiscreen", cUIThread_deleteThread__FPv);
#ifdef SKIP_ASM
void cUIThread_deleteThread(void* self)
{
    cListNode_removeFromList(self);
    if (self != 0) {
        sDeleteTarget* target = *(sDeleteTarget**)((char*)self + 0x8);
        target->fn((char*)self + target->field_0x8, 3);
    }
}
#endif

INCLUDE_ASM("ui/uiscreen", func_0039C558);

INCLUDE_ASM("ui/uiscreen", cUIScreen_setData);

//100%
INCLUDE_ASM("ui/uiscreen", func_0039C728);
#ifdef SKIP_ASM
extern "C" void* func_0039C728(void* self, unsigned short frame)
{
    int* list = *(int**)((char*)self + 0x34);
    char* e = (char*)list + 4;
    unsigned int i;
    for (i = 0; i < (unsigned int)*list; i++) {
        if (*(unsigned short*)(e + 0x8) == frame) {
            return e;
        }
        e = e + *(int*)(e + 0x4);
    }
    return 0;
}
#endif

struct sFrameEntry {
    int label; // 0x0
    int stride; // 0x4
    unsigned short field_0x8; // 0x8
};

struct sFrameList {
    int count; // 0x0
};

struct cUIScreen {
    char pad_0x00[0x34];
    sFrameList* list; // 0x34
};

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_getFrameByLabel__FP9cUIScreeni);
#ifdef SKIP_ASM
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label)
{
    sFrameList* list = self->list;
    sFrameEntry* e = (sFrameEntry*)((char*)list + 4);
    unsigned int i;
    for (i = 0; i < (unsigned int)list->count; i++) {
        if (e->label == label) {
            return e->field_0x8;
        }
        e = (sFrameEntry*)((char*)e + e->stride);
    }
    return 0xFFFF;
}
#endif

INCLUDE_ASM("ui/uiscreen", cUIScreen_getPrimaryThread);

extern "C" void* cUIScreen_getPrimaryThread(void* self);
void cUIThread_deleteThread(void* self);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);

//99.75%
INCLUDE_ASM("ui/uiscreen", cUIScreen_jumpToFrame__FPvUs);
#ifdef SKIP_ASM
void cUIScreen_jumpToFrame(void* self, unsigned short frame)
{
    void* thread = cUIScreen_getPrimaryThread(self);
    if (thread != 0) {
        cUIThread_deleteThread(thread);
    }
    cUIScreen_playFrame(self, frame, 0);
}
#endif

INCLUDE_ASM("ui/uiscreen", cUIScreen_playFrame);

INCLUDE_ASM("ui/uiscreen", func_0039C978);

//100%
INCLUDE_ASM("ui/uiscreen", func_0039CBC8);
#ifdef SKIP_ASM
extern "C" void func_0039CBC8(void* p0, void* p1, void* p2, void* p3)
{
    *(unsigned int*)((char*)p3 + 0x10) &= 0xFEFFFFFFU;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", func_0039CBE0);
#ifdef SKIP_ASM
extern "C" void func_0039CBE0(cUIScreen* self, void* p1, void* p2, void* p3)
{
    sFrameEntry* e = (sFrameEntry*)((char*)self->list + 4);
    unsigned int i;
    for (i = 0; i < (unsigned int)self->list->count; i++) {
        if (e->label == *(int*)((char*)p2 + 4)) {
            *(unsigned short*)((char*)p3 + 0xC) = e->field_0x8;
            return;
        }
        e = (sFrameEntry*)((char*)e + e->stride);
    }
}
#endif

INCLUDE_ASM("ui/uiscreen", func_0039CC38);

INCLUDE_ASM("ui/uiscreen", func_0039CCA8);

INCLUDE_ASM("ui/uiscreen", func_0039CD30);

//100%
INCLUDE_ASM("ui/uiscreen", func_0039CD90__FPv);
#ifdef SKIP_ASM
void func_0039CD90(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", func_0039CD98__FPv);
#ifdef SKIP_ASM
void func_0039CD98(void* self)
{
}
#endif

INCLUDE_ASM("ui/uiscreen", func_0039CDA0);

//100%
INCLUDE_ASM("ui/uiscreen", func_0039CE20);
#ifdef SKIP_ASM
struct sUIFlags1C {
    unsigned int lo : 8;
    unsigned int mode : 6;
};

extern "C" void func_0039CE20(void* self)
{
    void* p = *(void**)((char*)self + 0xd0);
    if (p != 0) {
        ((sUIFlags1C*)((char*)p + 0x1c))->mode = 3;
    }
}
#endif

INCLUDE_ASM("ui/uiscreen", func_0039CE48);

//100%
INCLUDE_ASM("ui/uiscreen", func_0039CE90__FPv);
#ifdef SKIP_ASM
void func_0039CE90(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", func_0039CE98);
#ifdef SKIP_ASM
extern "C" void func_0039CE98(void* self, void* p1, void* p2, void* p3)
{
    *(unsigned int*)((char*)p3 + 0x10) &= 0xFEFFFFFFU;
    void* p = *(void**)((char*)self + 0xd0);
    if (p != 0) {
        ((sUIFlags1C*)((char*)p + 0x1c))->mode = 7;
    }
}
#endif

INCLUDE_ASM("ui/uiscreen", cUIScreen_createAllObjects);

INCLUDE_ASM("ui/uiscreen", cUIScreen_createObjectByStruct);

extern "C" void* func_00398798(void*);

//100%
INCLUDE_ASM("ui/uiscreen", func_0039D758__FPv);
#ifdef SKIP_ASM
void* func_0039D758(void* self)
{
    return func_00398798((char*)self + 0x40);
}
#endif

INCLUDE_ASM("ui/uiscreen", func_0039D778);

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_getObjectByHashName);
#ifdef SKIP_ASM
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash)
{
    void** objs = *(void***)((char*)self + 0x3C);
    if (objs != 0) {
        for (unsigned int i = 0; i < **(unsigned int**)((char*)self + 0x38); i++) {
            void* o = objs[i];
            if (o != 0 && *(int*)((char*)o + 0x38) == hash) {
                return o;
            }
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("ui/uiscreen", func_0039D8C0);

extern "C" void* func_0039FE00(void* self);

//100%
INCLUDE_ASM("ui/uiscreen", func_0039D948__FPv);
#ifdef SKIP_ASM
void* func_0039D948(void* self)
{
    return func_0039FE00(self);
}
#endif

INCLUDE_ASM("ui/uiscreen", func_0039D968);

INCLUDE_ASM("ui/uiscreen", func_0039DA20);

INCLUDE_ASM("ui/uiscreen", func_0039DE68);

INCLUDE_ASM("ui/uiscreen", func_0039DF28);

INCLUDE_ASM("ui/uiscreen", func_0039DFB0);

INCLUDE_ASM("ui/uiscreen", func_0039DFE8);

INCLUDE_ASM("ui/uiscreen", func_0039E130);

extern void* D_0046DD60[];

//100%
INCLUDE_ASM("ui/uiscreen", func_0039E288__FPv);
#ifdef SKIP_ASM
void* func_0039E288(void* self)
{
    *(int*)self = (int)(void*)D_0046DD60;
    return self;
}
#endif

INCLUDE_ASM("ui/uiscreen", func_0039E2A0);

INCLUDE_ASM("ui/uiscreen", func_0039E318);

INCLUDE_ASM("ui/uiscreen", func_0039E390);

extern "C" void* func_00397948(void*);

//100%
INCLUDE_ASM("ui/uiscreen", func_0039E4A0__FPv);
#ifdef SKIP_ASM
void* func_0039E4A0(void* self)
{
    return func_00397948(((char*)self + 0x24));
}
#endif

INCLUDE_ASM("ui/uiscreen", func_0039E4C0);

//100%
INCLUDE_ASM("ui/uiscreen", func_0039E508__FPv);
#ifdef SKIP_ASM
void func_0039E508(void* self)
{
}
#endif

INCLUDE_ASM("ui/uiscreen", func_0039E510);

INCLUDE_ASM("ui/uiscreen", func_0039E688);

INCLUDE_ASM("ui/uiscreen", func_0039E6B8);

struct cList;
struct cListNode;
void cList_addToEnd(cList*, cListNode*);

//100%
INCLUDE_ASM("ui/uiscreen", func_0039E758);
#ifdef SKIP_ASM
extern "C" void func_0039E758(void* self, cListNode* node)
{
    cList_addToEnd((cList*)((char*)self + 0x24), node);
}
#endif

INCLUDE_ASM("ui/uiscreen", func_0039E868);

