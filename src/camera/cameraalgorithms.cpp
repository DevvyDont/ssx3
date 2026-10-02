#include "common.h"

INCLUDE_ASM("camera/cameraalgorithms", cChaseCameraController_createChaseAlgorithmBlend);

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015D698);
#ifdef SKIP_ASM
struct func_0015D698_sVec4 { float x, y, z, w; } __attribute__((aligned(16)));

extern void* D_0045B8B0[];
extern func_0015D698_sVec4 D_004FF140;

extern "C" void* func_0015D698(void* self)
{
    func_0015D698_sVec4 v;
    *(void***)((char*)self + 0x90) = D_0045B8B0;
    v.x = 0; v.y = 0; v.z = 0; v.w = 0;
    *(func_0015D698_sVec4*)((char*)self + 0x20) = v;
    v.w = 1.0f;
    v.x = 0;
    v.y = 1.0f;
    v.z = 0;
    *(func_0015D698_sVec4*)((char*)self + 0x30) = v;
    *(func_0015D698_sVec4*)((char*)self + 0x80) = D_004FF140;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015D700);
#ifdef SKIP_ASM
extern "C" void* func_0015D928(void* self);

extern "C" void func_0015D700(void* self, float x, float y, float z)
{
    func_0015D698_sVec4 v;
    *(float*)((char*)self + 0xC) = x;
    *(float*)((char*)self + 0x10) = y;
    *(float*)((char*)self + 0x14) = z;
    *(float*)((char*)self + 0x0) = x;
    *(float*)((char*)self + 0x4) = y;
    *(float*)((char*)self + 0x8) = z;
    v.x = 0; v.y = 0; v.z = 0; v.w = 0;
    *(func_0015D698_sVec4*)((char*)self + 0x20) = v;
    v.w = 1.0f;
    v.x = 0;
    v.y = 1.0f;
    v.z = 0;
    *(func_0015D698_sVec4*)((char*)self + 0x30) = v;
    *(func_0015D698_sVec4*)((char*)self + 0x80) = D_004FF140;
    func_0015D928(self);
}
#endif

INCLUDE_ASM("camera/cameraalgorithms", func_0015D928);

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015DAC0);
#ifdef SKIP_ASM
extern void* D_004A28A8;

extern "C" void func_0015DAC0(void)
{
    void* cam = *(void**)((char*)D_004A28A8 + 0x84);
    void* chase = *(void**)((char*)cam + 0x84);
    void* ctrl = *(void**)((char*)chase + 0x4);
    *(int*)((char*)ctrl + 0x4A4) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", func_0015DB58);
#ifdef SKIP_ASM
extern "C" void func_0016C968(void* self);
void cCameraTriggerMan_setInGameTriggers(void* self);
extern "C" void func_00162170(void* cam);
extern char D_004C5830[];

struct sCamVE_0015DB58 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_0015DB58(void)
{
    func_0016C968(D_004C5830);
    cCameraTriggerMan_setInGameTriggers(D_004C5830);
    char* riders = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x84);
    int n = *(int*)riders;
    func_00162170(*(void**)(*(char**)(riders + 4) + 0xA8));
    char* cam = *(char**)(*(char**)(riders + 4) + 0xA8);
    sCamVE_0015DB58* e = &(*(sCamVE_0015DB58**)(cam + 0x14))[4];
    e->fn(cam + e->delta);
    *(int*)(*(char**)(riders + 4) + 0x460) = 0;
    *(int*)(*(char**)(riders + 4) + 0x4A4) = 1;
    if (n >= 2)
    {
        func_00162170(*(void**)(*(char**)(riders + 8) + 0xA8));
        char* cam2 = *(char**)(*(char**)(riders + 8) + 0xA8);
        sCamVE_0015DB58* e2 = &(*(sCamVE_0015DB58**)(cam2 + 0x14))[4];
        e2->fn(cam2 + e2->delta);
        *(int*)(*(char**)(riders + 8) + 0x460) = 0;
    }
}
#endif

