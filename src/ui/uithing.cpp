#include "common.h"

struct cUIThing {
    char pad_0x00[0x3C];
    unsigned short* mEvents; // 0x3C
};

//100%
INCLUDE_ASM("ui/uithing", cUIThing_getKeyframerEvent__FP8cUIThingi);
#ifdef SKIP_ASM
unsigned short cUIThing_getKeyframerEvent(cUIThing* self, int x)
{
    signed char idx = (signed char)x;
    if (self->mEvents != 0) {
        return self->mEvents[idx];
    }
    return 0xFFFF;
}
#endif

INCLUDE_ASM("ui/uithing", func_0039FFA0);

INCLUDE_ASM("ui/uithing", func_003A0000);

INCLUDE_ASM("ui/uithing", func_003A0048);

INCLUDE_ASM("ui/uithing", func_003A0158);

//100%
INCLUDE_ASM("ui/uithing", func_003A0290);
#ifdef SKIP_ASM
struct sUIVec3 {
    float x, y, z;
};

struct sUIThingPos {
    char pad0[0x40];
    sUIThingPos* parent;    // 0x40
    sUIVec3 pos;            // 0x44
};

extern "C" void func_003A0290(sUIThingPos* self, sUIVec3* out)
{
    *out = self->pos;
    for (sUIThingPos* p = self->parent; p != 0; p = p->parent) {
        sUIVec3 v = p->pos;
        out->x += v.x;
        out->y += v.y;
        out->z += v.z;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uithing", func_003A0318);
#ifdef SKIP_ASM
extern "C" int func_003A0318(void* self, int a1)
{
    self = (char*)self + ((signed char)a1 << 2);
    return *(int*)((char*)self + 0x6c);
}
#endif

//100%
INCLUDE_ASM("ui/uithing", func_003A0330);
#ifdef SKIP_ASM
extern "C" void func_003A0330(void* self, int a1, int a2)
{
    self = (char*)self + ((signed char)a1 << 2);
    *(int*)((char*)self + 0x6c) = a2;
}
#endif

INCLUDE_ASM("ui/uithing", func_003A0348);

INCLUDE_ASM("ui/uithing", func_003A03F0);

//100%
INCLUDE_ASM("ui/uithing", func_003A04F0);
#ifdef SKIP_ASM
extern "C" int func_003A04F0(void* self)
{
    void* p = *(void**)((char*)*(void**)((char*)*(void**)((char*)self + 0x5c) + 0xd0) + 0x10);
    if (p != 0) {
        return *(int*)((char*)*(void**)((char*)p + 0x8) + (*(unsigned char*)((char*)self + 0x74) & 3) * 12 + 0xC);
    }
    return 0;
}
#endif

INCLUDE_ASM("ui/uithing", func_003A0528);

