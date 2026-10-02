#include "common.h"

INCLUDE_ASM("animation/animmodel", cAnimModel_addModelPartLOD);

//100%
INCLUDE_ASM("animation/animmodel", func_0030DB70__FPvi);
#ifdef SKIP_ASM
struct sAnimModelList {
    char pad_0x00[0x60];
    int count;     // 0x60
    int items[1];  // 0x64
};

void func_0030DB70(void* self, int item)
{
    sAnimModelList* list = (sAnimModelList*)self;
    list->items[list->count++] = item;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_0030DB90);
#ifdef SKIP_ASM
extern "C" int func_0030DB90(void* self, void* a1)
{
    return *(int*)self - *(int*)a1;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_0030DBA0);
#ifdef SKIP_ASM
struct sVec4_0030DBA0
{
    float x, y, z, w;
} __attribute__((aligned(16)));

extern "C" void func_0030DBA0(void* self, float a, float b)
{
    sVec4_0030DBA0 v;
    v.x = a;
    v.y = b;
    v.z = b;
    v.w = 1.0f;
    *(sVec4_0030DBA0*)((char*)self + 0x140) = v;
}
#endif

INCLUDE_ASM("animation/animmodel", cAnimModel_compile);

//100%
INCLUDE_ASM("animation/animmodel", func_0030E9E0);
#ifdef SKIP_ASM
struct sAnimChannel_0030E9E0
{
    int active;
    int pad;
};

struct sAnimEntry_0030E9E0
{
    char pad[0x10];
    ulong mask;                          // 0x10
    sAnimChannel_0030E9E0 channels[4];   // 0x18
    char pad2[0x18];
    int param;                           // 0x50
    int pad3;
};

struct sAnimModel_0030E9E0
{
    char pad0[0x8];
    int numEntries;               // 0x8
    sAnimEntry_0030E9E0* entries; // 0xC
    char pad1[0x4];
    int count;                    // 0x14
    char pad2[0x4];
    int* indices;                 // 0x1C
    char pad3[0x130];
    ulong mask;                   // 0x150
};

// PORT: ulong is 64-bit here
extern "C" void func_0030E9E0(sAnimModel_0030E9E0* self, int i, int channel)
{
    if (i > self->count)
    {
        return;
    }
    if (self->indices == 0)
    {
        return;
    }
    int idx = self->indices[i];
    if (idx < 0)
    {
        return;
    }
    sAnimEntry_0030E9E0* e = &self->entries[idx];
    self->mask |= e->mask;
    if (channel < 0)
    {
        for (int k = 0; k < 4; k++)
        {
            e->channels[k].active = 1;
        }
    }
    else
    {
        e->channels[channel].active = 1;
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_0030EA80);
#ifdef SKIP_ASM
// PORT: ulong is 64-bit here
extern "C" void func_0030EA80(sAnimModel_0030E9E0* self, int i, int channel)
{
    if (i > self->count)
    {
        return;
    }
    if (self->indices == 0)
    {
        return;
    }
    int idx = self->indices[i];
    if (idx < 0)
    {
        return;
    }
    sAnimEntry_0030E9E0* e = &self->entries[idx];
    self->mask &= ~e->mask;
    if (channel < 0)
    {
        for (int k = 0; k < 4; k++)
        {
            e->channels[k].active = 0;
        }
    }
    else
    {
        e->channels[channel].active = 0;
        for (int k = 0; k < 4; k++)
        {
            if (e->channels[k].active)
            {
                self->mask |= e->mask;
                break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_0030EB60);
#ifdef SKIP_ASM
// PORT: ulong is 64-bit here
extern "C" void func_0030EB60(sAnimModel_0030E9E0* self)
{
    for (int j = 0; j < self->numEntries; j++)
    {
        for (int k = 0; k < 4; k++)
        {
            self->entries[j].channels[k].active = 0;
        }
    }
    self->mask = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_0030EBC0);
#ifdef SKIP_ASM
extern "C" int func_0030EBC0(sAnimModel_0030E9E0* self, int i, int channel)
{
    int idx;
    if (i > self->count || self->indices == 0 || (idx = self->indices[i]) < 0)
    {
        return 0;
    }
    return self->entries[idx].channels[channel].active;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_0030EC18);
#ifdef SKIP_ASM
// PORT: ulong is 64-bit here
extern "C" void func_0030EC18(sAnimModel_0030E9E0* self, int i, int param)
{
    if (i > self->count)
    {
        return;
    }
    if (self->indices == 0)
    {
        return;
    }
    int idx = self->indices[i];
    if (idx < 0)
    {
        return;
    }
    sAnimEntry_0030E9E0* e = &self->entries[idx];
    e->param = param;
    self->mask |= e->mask;
    for (int k = 0; k < 4; k++)
    {
        e->channels[k].active = 1;
    }
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_0030ECA0);
#ifdef SKIP_ASM
struct sAnimEntry_0030ECA0
{
    char pad[0x10];
    ulong mask;
    char pad2[0x40];
};

struct sAnimModel_0030ECA0
{
    char pad0[0xC];
    sAnimEntry_0030ECA0* entries; // 0xC
    char pad1[0xC];
    int* indices;                 // 0x1C
    char pad2[0x138];
    ulong mask;                   // 0x158
};

// PORT: ulong is 64-bit here
extern "C" void func_0030ECA0(sAnimModel_0030ECA0* self, int i)
{
    sAnimEntry_0030ECA0* e = &self->entries[self->indices[i]];
    self->mask |= e->mask;
}
#endif

INCLUDE_ASM("animation/animmodel", func_0030ECD8);

INCLUDE_ASM("animation/animmodel", func_0030F2B0);

INCLUDE_ASM("animation/animmodel", func_00310120);

INCLUDE_ASM("animation/animmodel", func_00310200);

INCLUDE_ASM("animation/animmodel", func_003103F0);

//100%
INCLUDE_ASM("animation/animmodel", func_00310530);
#ifdef SKIP_ASM
struct sAmVec4
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sAmMtx
{
    sAmVec4 row[4];
};

struct sAmPair
{
    sAmVec4 a;
    sAmVec4 b;
};

struct sAmEntry
{
    int unk0;
    int start;
    char pad8[0x3C];
    int count;
    char pad48[0x10];
};

struct sAmModel
{
    int unk0;
    int unk4;
    int numEntries;
    sAmEntry* entries;
    char pad10[0x1C];
    sAmPair* pairs;
    sAmMtx* mtxA;
    sAmMtx* mtxB;
};

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void amVecAddEq(sAmVec4* dst, sAmVec4* v)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(*dst)
        : "m"(*dst), "m"(*v));
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sAmVec4 amVecAdd(const sAmVec4& src, sAmVec4* b)
{
    sAmVec4 a = src;
    sAmVec4 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(*b)
        : "memory");
    return r;
}

extern "C" void func_00310530(sAmModel* self, sAmVec4* v)
{
    for (int i = 0; i < self->numEntries; i++) {
        for (int j = 0; j < self->entries[i].count; j++) {
            int k = j + self->entries[i].start;
            amVecAddEq(&self->pairs[k].a, v);
            {
                sAmVec4 t = amVecAdd(self->mtxA[k].row[3], v);
                self->mtxA[k].row[3] = t;
            }
            {
                sAmVec4 t = amVecAdd(self->mtxB[k].row[3], v);
                self->mtxB[k].row[3] = t;
            }
        }
    }
}
#endif

INCLUDE_ASM("animation/animmodel", func_00310640);

INCLUDE_ASM("animation/animmodel", func_00310948);

INCLUDE_ASM("animation/animmodel", func_00310C48);

INCLUDE_ASM("animation/animmodel", func_00310CE8);

//100%
INCLUDE_ASM("animation/animmodel", func_00310EA8);
#ifdef SKIP_ASM
class cAmStream {
public:
    virtual void read(void* data, int size);
    virtual void write(void* data, int size);
};

extern "C" void func_00310EA8(void* self, cAmStream* s)
{
    s->read(*(void**)((char*)self + 0x2C), *(int*)((char*)self + 0x10) << 5);
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_00310EE0);
#ifdef SKIP_ASM
extern "C" void func_00310EE0(void* self, cAmStream* s)
{
    s->write(*(void**)((char*)self + 0x2C), *(int*)((char*)self + 0x10) << 5);
}
#endif

INCLUDE_ASM("animation/animmodel", func_00310F18);

INCLUDE_ASM("animation/animmodel", func_00311048);

//100%
INCLUDE_ASM("animation/animmodel", func_003110D0);
#ifdef SKIP_ASM
extern int D_004FE7A0[];
extern int D_004FE860[];

extern "C" void func_003110D0(void)
{
    for (int i = 0; i < 48; i++)
    {
        D_004FE7A0[i] = 0;
        D_004FE860[i] = 0;
    }
}
#endif

INCLUDE_ASM("animation/animmodel", func_00311110);

//100%
INCLUDE_ASM("animation/animmodel", func_00311220);
#ifdef SKIP_ASM
extern "C" void func_00311290(void* self, char* data);

extern "C" void* func_00311220(void* self, char* data)
{
    *(int*)self = 0;
    func_00311290(self, data);
    return self;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_00311250);
#ifdef SKIP_ASM
extern "C" char* func_003E22F0(void* src, int flags);

extern "C" void* func_00311250(char** self, void* src)
{
    char* data = func_003E22F0(src, 0);
    *self = data;
    func_00311290(self, data);
    return self;
}
#endif

//100%
INCLUDE_ASM("animation/animmodel", func_00311290);
#ifdef SKIP_ASM
extern "C" void func_00311290(void* self, char* data)
{
    *(char**)((char*)self + 0x4) = data;
    *(char**)((char*)self + 0x8) = data + 0x10;
    *(char**)((char*)self + 0xC) = data + *(int*)(data + 0x4);
    *(char**)((char*)self + 0x10) = data + *(int*)(data + 0x8);
    *(char**)((char*)self + 0x14) = data + *(int*)(data + 0xC);
}
#endif

INCLUDE_ASM("animation/animmodel", func_003112C8);

INCLUDE_ASM("animation/animmodel", func_00311318);

