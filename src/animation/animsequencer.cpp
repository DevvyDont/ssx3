#include "common.h"

struct cAnimSequence {
    char pad_0x00[0x98];
    float mCurrentWeight; // 0x98
    float mTargetWeight; // 0x9C
    float mFadeSpeed; // 0xA0
};

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_fadeWeight__FP13cAnimSequenceff);
#ifdef SKIP_ASM
void cAnimSequence_fadeWeight(cAnimSequence* self, float weight, float target)
{
    self->mCurrentWeight = weight;
    self->mTargetWeight = target;
    self->mFadeSpeed = 0.0f;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00313A20);
#ifdef SKIP_ASM
extern "C" void func_00313A20(void* self, float v)
{
    if (*(float*)((char*)self + 0x98) != 0.0f || *(float*)((char*)self + 0x9C) == 0.0f || v < *(float*)((char*)self + 0x9C))
    {
        *(float*)((char*)self + 0x9C) = v;
    }
    *(float*)((char*)self + 0x98) = 0.0f;
    *(int*)((char*)self + 0xA0) = 1;
}
#endif

INCLUDE_ASM("animation/animsequencer", func_00313A70);

//100%
INCLUDE_ASM("animation/animsequencer", func_00313AD8);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_0048A5B0[];

struct sSeqNode313AD8
{
    int key;
    int active;
    int unk8;
    sSeqNode313AD8* next;
};

extern "C" void func_00313AD8(void* self, int key)
{
    sSeqNode313AD8* n = (sSeqNode313AD8*)cMemMan_alloc(0x10, D_0048A5B0, 0x20000000, 0);
    sSeqNode313AD8* old = *(sSeqNode313AD8**)((char*)self + 0xAC);
    *(sSeqNode313AD8**)((char*)self + 0xAC) = n;
    n->key = key;
    n->active = 1;
    n->next = old;
    n->unk8 = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00313BA8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
struct sSeqNode313BA8
{
    int key;
    int active;
    int unk8;
    sSeqNode313BA8* next;
};

extern "C" void func_00313BA8(void* self, int key)
{
    sSeqNode313BA8** pp;
    for (pp = (sSeqNode313BA8**)((char*)self + 0xAC); *pp != 0; pp = &(*pp)->next)
    {
        if ((*pp)->key == key)
        {
            sSeqNode313BA8* n = *pp;
            if (n->active != 0)
            {
                *pp = n->next;
                operator_delete((int*)n);
                return;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00313C08);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_00313C08(char* self)
{
    while (*(char**)(self + 0xAC) != 0)
    {
        char* node = *(char**)(self + 0xAC);
        *(char**)(self + 0xAC) = *(char**)(node + 0xC);
        operator_delete((int*)node);
    }
}
#endif

INCLUDE_ASM("animation/animsequencer", func_00313C50);

//100%
INCLUDE_ASM("animation/animsequencer", func_00313CF0);
#ifdef SKIP_ASM
extern "C" void func_00313CF0(void* self, int a1, float val)
{
    char* p = (char*)self + a1 * 0x1c;
    *(float*)(p + 0x8) = val;
    *(int*)((char*)self + 0xc4) = 1;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00313D28);
#ifdef SKIP_ASM
extern "C" void func_00313D28(void* self, int a1, float val)
{
    char* p = (char*)self + a1 * 0x1c;
    *(float*)(p + 0x14) = val;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00313D40);
#ifdef SKIP_ASM
extern "C" void func_00313D40(void* self, int a1, int val)
{
    char* p = (char*)self + a1 * 0x1c;
    *(int*)(p + 0x1c) = val;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00313D70);
#ifdef SKIP_ASM
struct sSeqTrack_00313D70
{
    char pad0[0x8];
    float time;   // 0x8
    char pad1[0x4];
    float end;    // 0x10
    float weight; // 0x14
    int flag;     // 0x18
};

struct sSeq_00313D70
{
    sSeqTrack_00313D70 tracks[5]; // 0x0
    char pad0[0x8];
    float scale;   // 0x94
    char pad1[0xC];
    float fadeOut; // 0xA4
    float fadeIn;  // 0xA8
};

// PORT: g++ <? (min) operator, removed in GCC 4.3.
extern "C" float func_00313D70(sSeq_00313D70* self, int i)
{
    float time = self->tracks[i].time;
    float w = self->scale * self->tracks[i].weight;
    float end = self->tracks[i].end;
    if (time < self->fadeIn)
    {
        w = (time / self->fadeIn) <? w;
    }
    float rem = end - time;
    if (rem < self->fadeOut)
    {
        w = (rem / self->fadeOut) <? w;
    }
    return w;
}
#endif

INCLUDE_ASM("animation/animsequencer", func_00313DD0);

INCLUDE_ASM("animation/animsequencer", func_00314050);

//100%
INCLUDE_ASM("animation/animsequencer", func_003142E8__FPv);
#ifdef SKIP_ASM
void func_003142E8(void* self)
{
    *(int*)self = 0;
    *(void**)((char*)self + 0x4) = 0;
}
#endif

INCLUDE_ASM("animation/animsequencer", func_003142F8);

//100%
INCLUDE_ASM("animation/animsequencer", func_00314368);
#ifdef SKIP_ASM
// Same layout as the unit's cAnimSequenceNode / cAnimSequencer (declared later in the unit).
struct sSeqNode_00314368 {
    char pad_0x00[0xC8];
    sSeqNode_00314368* next; // 0xC8
};

struct sSequencer_00314368 {
    int mCount;                      // 0x0
    sSeqNode_00314368* mFirstSequence; // 0x4
};

extern "C" void func_00314368(sSequencer_00314368* self, sSeqNode_00314368* node)
{
    if (self->mFirstSequence != 0)
    {
        sSeqNode_00314368* p = self->mFirstSequence;
        while (p->next != 0)
        {
            p = p->next;
        }
        p->next = node;
    }
    else
    {
        self->mFirstSequence = node;
    }
    node->next = 0;
    self->mCount++;
}
#endif

INCLUDE_ASM("animation/animsequencer", func_00314418);

INCLUDE_ASM("animation/animsequencer", func_00314518);

INCLUDE_ASM("animation/animsequencer", func_00314588);

INCLUDE_ASM("animation/animsequencer", func_003145F8);

INCLUDE_ASM("animation/animsequencer", func_00314668);

//100%
INCLUDE_ASM("animation/animsequencer", func_003146D0);
#ifdef SKIP_ASM
extern "C" void func_003146D0(void* self, float v)
{
    for (char* node = *(char**)((char*)self + 0x4); node != 0; node = *(char**)(node + 0xC8))
    {
        func_00313A20(node, v);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/animsequencer", func_00314718);
#ifdef SKIP_ASM
extern "C" void func_00313BA8(void* self, int v);

extern "C" void func_00314718(void* self, int v)
{
    for (char* node = *(char**)((char*)self + 0x4); node != 0; node = *(char**)(node + 0xC8))
    {
        func_00313BA8(node, v);
    }
}
#endif

struct cAnimSequenceNode {
    char pad_0x00[0xC8];
    cAnimSequenceNode* next; // 0xC8
};

struct cAnimSequencer {
    char pad_0x00[0x4];
    cAnimSequenceNode* mFirstSequence; // 0x4
};

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_getSequence__FP14cAnimSequenceri);
#ifdef SKIP_ASM
cAnimSequenceNode* cAnimSequencer_getSequence(cAnimSequencer* self, int index)
{
    cAnimSequenceNode* cur = self->mFirstSequence;
    while (cur != 0 && index-- > 0) {
        cur = cur->next;
    }
    return cur;
}
#endif

INCLUDE_ASM("animation/animsequencer", func_003147F0);

INCLUDE_ASM("animation/animsequencer", func_00314880);

//100%
INCLUDE_ASM("animation/animsequencer", func_00314978__FPv);
#ifdef SKIP_ASM
void* func_00314978(void* self)
{
    *(int*)self = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00314988);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void func_003149D0(void** self);

extern "C" void func_00314988(int* self, int flags)
{
    func_003149D0((void**)self);
    if (flags & 1)
    {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_003149D0);
#ifdef SKIP_ASM
void cMemMan_free(void* ptr);

extern "C" void func_003149D0(void** self)
{
    if (self[0] != 0)
    {
        cMemMan_free(self[0]);
    }
    self[0] = 0;
    self[1] = 0;
    self[2] = 0;
    self[3] = 0;
}
#endif

extern "C" void* func_00314AA0(void*);

//100%
INCLUDE_ASM("animation/animsequencer", func_00314A18__FPvi);
#ifdef SKIP_ASM
// PORT: asm label (same binding the unit uses for func_00314AA0's two-argument body).
void func_00314AA0_impl(void* self, char* data) __asm__("func_00314AA0");

void func_00314A18(void* self, int a1)
{
    *(int*)self = a1;
    func_00314AA0_impl(self, (char*)a1);
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00314A40__FPv);
#ifdef SKIP_ASM
void* func_00314A40(void* self)
{
    *(int*)self = 0;
    return func_00314AA0(self);
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00314AA0);
#ifdef SKIP_ASM
// The original had the same mismatch: callers (func_00314A18/func_00314A40, the
// latter already 100%) see the one-argument declaration above and leave $5 as-is,
// while the definition takes two. The asm label binds this two-argument body to
// the symbol without changing the callers' prototype.
// PORT: asm labels are GCC/Clang-only; the port should give callers the real signature.
void func_00314AA0_impl(void* self, char* data) __asm__("func_00314AA0");
void func_00314AA0_impl(void* self, char* data)
{
    *(char**)((char*)self + 0x4) = data;
    *(char**)((char*)self + 0x8) = data + *(short*)(data + 0x6);
    *(char**)((char*)self + 0xC) = data + *(int*)(data + 0x8);
}
#endif

INCLUDE_ASM("animation/animsequencer", func_00314C00);

