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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", func_0026E4B8);
#ifdef SKIP_ASM
// PORT: func_0026E448 is declared in this unit with one parameter, but this caller
// passes (self, index); bind the 2-arg form to the symbol.
void* func_0026E448_2(void* self, int idx) __asm__("func_0026E448__FPv");
extern "C" int func_0026D4D8(void* stream, unsigned int pos);

struct sReplay0026E4B8 {
    char pad_0x000[0x484];
    int pos;
    void* frame;
    void* streams[2];
};

extern "C" void func_0026E4B8(void* self_)
{
    sReplay0026E4B8* self = (sReplay0026E4B8*)self_;
    int i;
    void* p = func_0026E448_2(self, self->pos);
    self->frame = p;
    self->pos = *(int*)((char*)p + 0x30);
    for (i = 0; i < 2; i++) {
        if (self->streams[i] != 0) {
            func_0026D4D8(self->streams[i], self->pos);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", func_0026E528);
#ifdef SKIP_ASM
// PORT: func_0026E468 really takes (self, index); the unit declares it with 1 arg
void* func_0026E468_2(void* self, int idx) __asm__("func_0026E468");
extern "C" void func_0026E4B8(void* self);

extern "C" void func_0026E528(void* self)
{
    void* p = func_0026E468_2(self, *(int*)((char*)self + 0x484));
    *(void**)((char*)self + 0x488) = p;
    *(int*)((char*)self + 0x484) = *(int*)((char*)p + 0x30);
    func_0026E4B8(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replay", func_0026E568);
#ifdef SKIP_ASM
// PORT: func_0026E490 really takes (self, index); the unit declares it with 1 arg
void* func_0026E490_2(void* self, int idx) __asm__("func_0026E490");
extern "C" void func_0026E4B8(void* self);

extern "C" void func_0026E568(void* self)
{
    void* p = func_0026E490_2(self, *(int*)((char*)self + 0x484));
    *(void**)((char*)self + 0x488) = p;
    *(int*)((char*)self + 0x484) = *(int*)((char*)p + 0x30);
    func_0026E4B8(self);
}
#endif

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

//100%
INCLUDE_ASM("replay/replay", func_0026E838);
#ifdef SKIP_ASM
void func_0026D730(void*, int, int);

extern "C" int func_0026E838(void* self, void* o, int mode)
{
    if (mode) {
        int d = *(int*)((char*)o + 0x9C) - *(int*)((char*)self + 0xA0);
        if (d == 0) return 0;
        if (d >= 1000) {
            func_0026D730(*(void**)((char*)self + 0xB0), *(int*)self, d);
            return 1;
        }
        float v = *(float*)((char*)o + 0x44);
        if (v >= 5.0f) {
            func_0026D730(*(void**)((char*)self + 0xB0), *(int*)self, (int)(v * 1000.0f));
            return 1;
        }
    }
    int e = *(int*)((char*)o + 0xA4) - *(int*)((char*)self + 0xA8);
    if (e == 0) return 0;
    func_0026D730(*(void**)((char*)self + 0xB0), *(int*)self, -e);
    return 1;
}
#endif

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

