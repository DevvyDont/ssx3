#include "common.h"

INCLUDE_ASM("animation/rideranimbase", cRiderAnimBase_cRiderAnimBase);

INCLUDE_ASM("animation/rideranimbase", func_00311958);

INCLUDE_ASM("animation/rideranimbase", func_00311A50);

INCLUDE_ASM("animation/rideranimbase", func_00311AE8);

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

INCLUDE_ASM("animation/rideranimbase", func_00311E88);

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

INCLUDE_ASM("animation/rideranimbase", func_00312AB0);

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

INCLUDE_ASM("animation/rideranimbase", func_00312BD0);

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

