#include "common.h"

//100%
INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_cRiderAnimBase);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void func_003142E8(void* self);
extern "C" void func_00311A50(void* self);
extern char D_0048D7D8[];
extern const char D_0048A4E0[];

extern "C" void* cRiderAnimBase_cRiderAnimBase(void* self)
{
    *(int*)((char*)self + 0x54) = 0;
    *(void**)((char*)self + 0x58) = D_0048D7D8;
    *(void**)((char*)self + 0x50) = operator_new_tag(0x30, D_0048A4E0, 0, 0);
    for (int i = 0; i < 6; i++)
    {
        func_003142E8(*(char**)((char*)self + 0x50) + (i << 3));
    }
    func_00311A50(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", func_00311958);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int* p);
extern "C" void func_00314668(void* self);
extern char D_0048D7D8[];

extern "C" void func_00311958(void* self, int flags)
{
    *(void**)((char*)self + 0x58) = D_0048D7D8;
    for (int i = 0; i < 6; i++)
    {
        func_00314668(*(char**)((char*)self + 0x50) + (i << 3));
    }
    void* p = *(void**)((char*)self + 0x50);
    if (p)
        cMemMan_free(p);
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", func_00311A50);
#ifdef SKIP_ASM
extern "C" void func_00314668(void* self);

struct sRabQuad32 {
    int w[8];
} __attribute__((aligned(16)));

extern sRabQuad32 D_004FF230;

struct sRiderAnimBase {
    int mAnims[6];      // 0x00
    int m18;            // 0x18
    float m1C;          // 0x1C
    long m20;           // 0x20 PORT: 64-bit long (sd)
    int pad28[2];       // 0x28
    sRabQuad32 m30;     // 0x30
    char* mSequencers;  // 0x50
};

extern "C" void func_00311A50(void* p)
{
    sRiderAnimBase* self = (sRiderAnimBase*)p;
    for (int i = 0; i < 6; i++)
    {
        func_00314668(self->mSequencers + (i << 3));
        self->mAnims[i] = 0x1B6;
    }
    //START
    self->m18 = 0;
    self->m1C = 1.0f;
    self->m30 = D_004FF230;
    self->m20 = -1;
    //END
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", func_00311AE8);
#ifdef SKIP_ASM
extern "C" int* cAIAnimEventMap_getBlendInTime(int anim);

extern "C" int func_00311AE8(void* self, int i)
{
    int anim = *(int*)((char*)self + (i << 2));
    if (anim == 0x1B6) {
        return 0;
    }
    return *cAIAnimEventMap_getBlendInTime(anim);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", func_00311B20);
#ifdef SKIP_ASM
struct cAnimSequenceNode;

struct cAnimSequencer {
    int mCount;                        // 0x0
    cAnimSequenceNode* mFirstSequence; // 0x4
};

cAnimSequenceNode* cAnimSequencer_getSequence(cAnimSequencer* self, int index);

extern "C" cAnimSequenceNode* func_00311B20(void* self, int i)
{
    cAnimSequencer* seqs = *(cAnimSequencer**)((char*)self + 0x50);
    return cAnimSequencer_getSequence(&seqs[i], 0);
}
#endif

INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_changeHeadingOffset);

INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_changeOrientationOffset);

//100%
INCLUDE_ASM("animation/rideranimbase", func_00311E88);
#ifdef SKIP_ASM
extern "C" void func_00314718(void* self, int v);
extern "C" void func_003146D0(void* self, float v);

extern "C" void func_00311E88(char* self, int i, float v)
{
    func_00314718(*(char**)(self + 0x50) + (i << 3), 0x3F);
    func_003146D0(*(char**)(self + 0x50) + (i << 3), v);
    *(int*)(self + (i << 2)) = 0x1B6;
}
#endif

INCLUDE_ASM("animation/rideranimbase", func_00311F00);

INCLUDE_ASM("animation/rideranimbase", func_003123C0);

INCLUDE_ASM("animation/rideranimbase", func_00312490);

INCLUDE_ASM("animation/rideranimbase", func_00312598);

INCLUDE_ASM("animation/rideranimbase", func_00312660);

INCLUDE_ASM("animation/rideranimbase", func_00312790);

INCLUDE_ASM("animation/rideranimbase", func_00312820);

INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_play);

INCLUDE_ASM("animation/rideranimbase", func_003129E0);

//100%
INCLUDE_ASM("animation/rideranimbase", func_00312AA0__FPvi);
#ifdef SKIP_ASM
int func_00312AA0(void* self, int i)
{
    return *(int*)((char*)self + (i << 2));
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", func_00312AB0);
#ifdef SKIP_ASM
extern "C" float func_00312AB0(void* self, int i)
{
    cAnimSequencer* seqs = *(cAnimSequencer**)((char*)self + 0x50);
    cAnimSequenceNode* node = cAnimSequencer_getSequence(&seqs[i], 0);
    return *(float*)((char*)node + 0x8) / *(float*)((char*)node + 0x10);
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", func_00312AE8);
#ifdef SKIP_ASM
extern "C" int func_00312AE8(void* self, int i)
{
    cAnimSequencer* seqs = *(cAnimSequencer**)((char*)self + 0x50);
    return *(int*)((char*)cAnimSequencer_getSequence(&seqs[i], 0) + 0xC0);
}
#endif

INCLUDE_ASM("animation/rideranimbase", func_00312B18);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/rideranimbase", func_00312BD0);
#ifdef SKIP_ASM
extern "C" void func_00144670(void* self, int bit);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

extern "C" void func_00312BD0(void* self, void* unused, void* rider, int anim)
{
    func_00144670((char*)rider + 0xB0, 0x3F);
    cRiderAnimBase_play(self, anim, 0, -1.0f);
}
#endif

INCLUDE_ASM("animation/rideranimbase", func_00312C20);

INCLUDE_ASM("animation/rideranimbase", func_003130A8);

INCLUDE_ASM("animation/rideranimbase", func_00313508);

INCLUDE_ASM("animation/rideranimbase", func_003135B0);

//100%
INCLUDE_ASM("animation/rideranimbase", func_00313800);
#ifdef SKIP_ASM
struct sBlend_00313800
{
    char pad0[0x94];
    float value;     // 0x94
    float target;    // 0x98
    float remaining; // 0x9C
    int notify;      // 0xA0
};

// Moves value toward target over the remaining time; returns 1 when the blend
// finishes and notify is set.
extern "C" int func_00313800(sBlend_00313800* self, float dt)
{
    float t = self->remaining;
    if (t != 0.0f)
    {
        if (t <= dt)
        {
            self->value = self->target;
            self->remaining = 0.0f;
            if (self->notify != 0)
            {
                return 1;
            }
        }
        else
        {
            float v = self->value;
            self->remaining = t - dt;
            self->value = v + (dt / t) * (self->target - v);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", func_00313868);
#ifdef SKIP_ASM
struct sAnimEvent_00313868
{
    int bit;                        // 0x0
    int kind;                       // 0x4
    float time;                     // 0x8
    sAnimEvent_00313868* next;      // 0xC
};

struct sAnimEvents_00313868
{
    char pad0[0xAC];
    sAnimEvent_00313868* head;      // 0xAC
    ulong active;                   // 0xB0
    ulong triggered;                // 0xB8
};

// Fires every kind-0 event whose time lies in (from, to] (or [to, from) when
// playing backwards).
// PORT: ulong is 64-bit here
extern "C" void func_00313868(sAnimEvents_00313868* self, float from, float to)
{
    if (from == to)
    {
        return;
    }
    int forward = from <= to;
    for (sAnimEvent_00313868* e = self->head; e != 0; e = e->next)
    {
        if (e->kind == 0)
        {
            float t = e->time;
            if (forward)
            {
                if (!(from < t && t <= to))
                    continue;
            }
            else
            {
                if (!(to <= t && t < from))
                    continue;
            }
            ulong bit = (ulong)1 << e->bit;
            ulong old = self->active;
            self->active = old | bit;
            self->triggered |= bit & ~old;
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", func_00313938);
#ifdef SKIP_ASM
struct sAnimEvent_00313938
{
    int bit;                        // 0x0
    int kind;                       // 0x4
    float time;                     // 0x8
    sAnimEvent_00313938* next;      // 0xC
};

struct sAnimEvents_00313938
{
    char pad0[0xAC];
    sAnimEvent_00313938* head;      // 0xAC
    ulong active;                   // 0xB0
    ulong triggered;                // 0xB8
    int dirty;                      // 0xC0
};

// PORT: ulong is 64-bit here
extern "C" void func_00313938(sAnimEvents_00313938* self, float time)
{
    for (sAnimEvent_00313938* e = self->head; e != 0; e = e->next)
    {
        if (e->kind == 0 && e->time == time)
        {
            ulong bit = (ulong)1 << e->bit;
            ulong old = self->active;
            self->active = old | bit;
            self->triggered |= bit & ~old;
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/rideranimbase", func_003139A8);
#ifdef SKIP_ASM
// PORT: ulong is 64-bit here
extern "C" void func_003139A8(sAnimEvents_00313938* self)
{
    self->dirty = 1;
    for (sAnimEvent_00313938* e = self->head; e != 0; e = e->next)
    {
        if (e->kind != 0)
        {
            ulong bit = (ulong)1 << e->bit;
            ulong old = self->active;
            self->active = old | bit;
            self->triggered |= bit & ~old;
        }
    }
}
#endif

