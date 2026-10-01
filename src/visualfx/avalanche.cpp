#include "common.h"

INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_getFrameData);

INCLUDE_ASM("visualfx/avalanche", func_002D6378);

INCLUDE_ASM("visualfx/avalanche", func_002D6410);

INCLUDE_ASM("visualfx/avalanche", func_002D64D8);

INCLUDE_ASM("visualfx/avalanche", tAvalancheNode_calculate);

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_calculateScale);
#ifdef SKIP_ASM
struct sAvalancheData {
    char pad[0xF0];
    unsigned short type;
    char padF2[0xE];
    float f100;
    float f104;
    float f108;
};

extern "C" void tActiveAvalancheNode_calculateScale(void* self, float t)
{
    sAvalancheData* d = *(sAvalancheData**)((char*)self + 0x2E0);
    float x = t * 30.0f;
    if (x < d->f104) {
        *(float*)((char*)self + 0xB0) = 1.0f;
        *(float*)((char*)self + 0xB4) = x / d->f104;
        return;
    }
    if (x < d->f100 - d->f108) {
        *(float*)((char*)self + 0xB4) = 1.0f;
        *(float*)((char*)self + 0xB0) = 1.0f;
        return;
    }
    if (d->type != 2) {
        float v = (d->f100 - x) / d->f108;
        *(float*)((char*)self + 0xB4) = v;
        *(float*)((char*)self + 0xB0) = v;
        if (v > 0.0f) {
            // PORT: sqrt.s (sqrtf without errno check)
            __asm__("sqrt.s %0, %1" : "=f"(v) : "f"(v));
            *(float*)((char*)self + 0xB0) = v;
        }
    }
}
#endif

INCLUDE_ASM("visualfx/avalanche", func_002D7CA8);

INCLUDE_ASM("visualfx/avalanche", func_002D7DD8);

INCLUDE_ASM("visualfx/avalanche", tActiveAvalanche_buildArray);

INCLUDE_ASM("visualfx/avalanche", func_002D7EF8);

INCLUDE_ASM("visualfx/avalanche", func_002D81B0);

INCLUDE_ASM("visualfx/avalanche", func_002D8258);

INCLUDE_ASM("visualfx/avalanche", func_002D82A0);

INCLUDE_ASM("visualfx/avalanche", func_002D83B8);

INCLUDE_ASM("visualfx/avalanche", func_002D87D0);

INCLUDE_ASM("visualfx/avalanche", func_002D8948);

INCLUDE_ASM("visualfx/avalanche", func_002D8A00);

INCLUDE_ASM("visualfx/avalanche", func_002D8EA8);

INCLUDE_ASM("visualfx/avalanche", func_002D9130);

INCLUDE_ASM("visualfx/avalanche", cAvalanche_addAvalancheNode);

INCLUDE_ASM("visualfx/avalanche", func_002D9538);

INCLUDE_ASM("visualfx/avalanche", func_002D9660);

INCLUDE_ASM("visualfx/avalanche", func_002D96E0);

INCLUDE_ASM("visualfx/avalanche", func_002D9738);

INCLUDE_ASM("visualfx/avalanche", cAvalanche_triggerAvalanche);

INCLUDE_ASM("visualfx/avalanche", func_002D9A80);

INCLUDE_ASM("visualfx/avalanche", func_002D9B40);

//100%
INCLUDE_ASM("visualfx/avalanche", func_002D9C00);
#ifdef SKIP_ASM
struct sAvVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sAvMatrix {
    sAvVec4 row[4];
};

struct sAvNode {
    char pad[0x60];
    sAvVec4 pos;            // 0x60
    sAvMatrix rot;          // 0x70
    float scale;            // 0xB0
    char padB4[0x2E0 - 0xB4];
    void* data;             // 0x2E0
    char pad2E4[0x2F0 - 0x2E4];
};

extern sAvNode D_004EE770[64];

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMatrixAV(sAvMatrix* dst, sAvMatrix* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix times scalar).
static inline void vu0ScaleMatrixAV(sAvMatrix* dst, sAvMatrix* src, float s)
{
    int t;
    __asm__ __volatile__(
        "mfc1      %0, %3\n"
        "lqc2      $vf4, 0x0(%2)\n"
        "qmtc2.ni  %0, $vf3\n"
        "lqc2      $vf5, 0x10(%2)\n"
        "lqc2      $vf6, 0x20(%2)\n"
        "lqc2      $vf7, 0x30(%2)\n"
        "vmulx.xyzw $vf8, $vf4, $vf3x\n"
        "vmulx.xyzw $vf9, $vf5, $vf3x\n"
        "vmulx.xyzw $vf10, $vf6, $vf3x\n"
        "vmulx.xyzw $vf11, $vf7, $vf3x\n"
        "sqc2      $vf8, 0x0(%1)\n"
        "sqc2      $vf9, 0x10(%1)\n"
        "sqc2      $vf10, 0x20(%1)\n"
        "sqc2      $vf11, 0x30(%1)\n"
        : "=&r"(t)
        : "r"(dst), "r"(src), "f"(s)
        : "memory");
}

extern "C" void func_002D9C00(int id, sAvMatrix* out)
{
    int i;
    for (i = 0; i < 64; i++) {
        sAvNode* node = &D_004EE770[i];
        if (node->data != 0 && *(int*)((char*)node->data + 0xF8) == id) {
            vu0CopyMatrixAV(out, &node->rot);
            vu0ScaleMatrixAV(out, out, node->scale);
            out->row[3] = node->pos;
            return;
        }
    }
}
#endif

INCLUDE_ASM("visualfx/avalanche", func_002D9CB0);

INCLUDE_ASM("visualfx/avalanche", cAvalanche_readFromReplayFrame);

INCLUDE_ASM("visualfx/avalanche", func_002D9FB8);

INCLUDE_ASM("visualfx/avalanche", cAvalanche_resolveDataPointers);

INCLUDE_ASM("visualfx/avalanche", func_002DA1C0);

