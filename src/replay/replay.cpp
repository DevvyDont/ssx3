#include "common.h"

INCLUDE_ASM("replay/replay", cReplay_restoreFrame);

INCLUDE_ASM("replay/replay", cReplay_restoreBucket);

INCLUDE_ASM("replay/replay", func_0026DE58);

INCLUDE_ASM("replay/replay", cReplay_restoreObject);

INCLUDE_ASM("replay/replay", cReplay_restoreDeadBucket);

extern "C" void* func_0026E6A0(void*);

//100%
INCLUDE_ASM("replay/replay", func_0026E448__FPv);
#ifdef SKIP_ASM
void* func_0026E448(void* self)
{
    return func_0026E6A0((char*)self + 0x3b0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", func_0026E468);
#ifdef SKIP_ASM
extern "C" void* func_0026E468(void* self)
{
    void* p = func_0026E448(self);
    void* q = *(void**)((char*)p + 0x14);
    if (q != 0) {
        p = q;
    }
    return p;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", func_0026E490);
#ifdef SKIP_ASM
extern "C" void* func_0026E490(void* self)
{
    void* p = func_0026E448(self);
    void* q = *(void**)((char*)p + 0x18);
    if (q != 0) {
        p = q;
    }
    return p;
}
#endif

INCLUDE_ASM("replay/replay", func_0026E4B8);

INCLUDE_ASM("replay/replay", func_0026E528);

INCLUDE_ASM("replay/replay", func_0026E568);

//100%
INCLUDE_ASM("replay/replay", func_0026E5A8__FPv);
#ifdef SKIP_ASM
void* func_0026E5A8(void* self)
{
    int t0 = 0;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)self = t0;
    *(int*)((char*)self + 0x8) = t0;
    return self;
}
#endif

//100%
INCLUDE_ASM("replay/replay", func_0026E5C0__FPv);
#ifdef SKIP_ASM
void func_0026E5C0(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x8) = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replay", func_0026E5D0);
#ifdef SKIP_ASM
struct sReplayNode {
    char pad[0x14];
    sReplayNode* next;
    sReplayNode* prev;
};

struct sReplayList {
    sReplayNode* head;
    sReplayNode* tail;
    int count;
};

extern "C" void func_0026E5D0(sReplayList* list, sReplayNode* node)
{
    node->prev = list->tail;
    node->next = 0;
    if (list->tail) {
        list->tail->next = node;
    }
    list->tail = node;
    if (!list->head) {
        list->head = node;
    }
    list->count++;
}
#endif

//100%
INCLUDE_ASM("replay/replay", func_0026E608);
#ifdef SKIP_ASM
extern "C" void func_0026E608(sReplayList* list, sReplayNode* node)
{
    if (node == list->head) {
        list->head = node->next;
    }
    if (node == list->tail) {
        list->tail = node->prev;
    }
    if (node->next != 0) {
        node->next->prev = node->prev;
    }
    if (node->prev != 0) {
        node->prev->next = node->next;
    }
    node->next = 0;
    node->prev = 0;
    list->count--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", func_0026E670);
#ifdef SKIP_ASM
extern "C" sReplayNode* func_0026E670(sReplayList* list)
{
    sReplayNode* node = list->head;
    func_0026E608(list, node);
    return node;
}
#endif

//100%
INCLUDE_ASM("replay/replay", func_0026E6A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sReplayFrame {
    char pad[0x14];
    sReplayFrame* next;
    char pad2[0x30 - 0x18];
    int time;
};

struct sReplayFrameList {
    sReplayFrame* head;
    sReplayFrame* tail;
    int count;
};

// the unit declares this with one arg; bind the real 2-arg body to the symbol
sReplayFrame* func_0026E6A0_impl(sReplayFrameList* list, int time) __asm__("func_0026E6A0");

sReplayFrame* func_0026E6A0_impl(sReplayFrameList* list, int time)
{
    sReplayFrame* f = list->head;
    while (f->next) {
        if (f->time == time) {
            break;
        }
        if (f->time < time && time < f->next->time) {
            break;
        }
        f = f->next;
    }
    return f;
}
#endif

INCLUDE_ASM("replay/replay", func_0026E6E0);

//100%
INCLUDE_ASM("replay/replay", func_0026E7F8__FPv);
#ifdef SKIP_ASM
void func_0026E7F8(void* self)
{
    *(int*)((char*)self + 0xB0) = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replay", func_0026E800);
#ifdef SKIP_ASM
extern "C" void func_0026E800(void* self)
{
    unsigned char* p = *(unsigned char**)((char*)self + 0xb0);
    if (p) {
        p[0x1e] &= ~(1 << *(int*)self);
        *(int*)((char*)self + 0xb0) = 0;
    }
}
#endif

INCLUDE_ASM("replay/replay", func_0026E838);

extern void* D_00481898[];

//100%
INCLUDE_ASM("replay/replay", func_0026E8E0__FPv);
#ifdef SKIP_ASM
void* func_0026E8E0(void* self)
{
    int t0 = 0;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)self = (int)(void*)D_00481898;
    *(int*)((char*)self + 0x8) = t0;
    *(int*)((char*)self + 0xc) = t0;
    *(int*)((char*)self + 0x10) = t0;
    return self;
}
#endif

