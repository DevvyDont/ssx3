#include "common.h"

INCLUDE_ASM("main/ssxapp", cSSXApp_cSSXApp);

INCLUDE_ASM("main/ssxapp", cSSXApp_init);

INCLUDE_ASM("main/ssxapp", cSSXApp_loadInputMap);

INCLUDE_ASM("main/ssxapp", cSSXApp_parseCommandLine);

extern "C" int func_00326C60(void* mgr);
extern void* D_004A28A0;

//99.9%
INCLUDE_ASM("main/ssxapp", cSSXApp_flush__Fv);
#ifdef SKIP_ASM
int cSSXApp_flush()
{
    if (D_004A28A0 != 0) {
        return func_00326C60(D_004A28A0);
    }
    return 0;
}
#endif

INCLUDE_ASM("main/ssxapp", cSSXApp_preUpdate);

extern "C" void func_00326B88(void* mgr);

//99.89%
INCLUDE_ASM("main/ssxapp", cSSXApp_timerCallback__Fv);
#ifdef SKIP_ASM
void cSSXApp_timerCallback()
{
    if (D_004A28A0 != 0) {
        func_00326B88(D_004A28A0);
    }
}
#endif

void* cMCOverlayManager_getManager();

//100%
INCLUDE_ASM("main/ssxapp", func_00227F80);
#ifdef SKIP_ASM
extern "C" void* func_00227F80()
{
    return cMCOverlayManager_getManager();
}
#endif

INCLUDE_ASM("main/ssxapp", cSSXApp_purge);

INCLUDE_ASM("main/ssxapp", cSSXApp_startGameLoad);

INCLUDE_ASM("main/ssxapp", func_00228238);

INCLUDE_ASM("main/ssxapp", cSSXApp_initload);

INCLUDE_ASM("main/ssxapp", cSSXApp_initLocale);

extern "C" void func_002B4B48(void* self);
extern "C" void func_00284C28();
extern "C" void cAppMan_loadexecpurge(void* self);

struct sExecPurgeVTable {
    char pad_0x00[0x3B8];
    short field_0x3B8;
    char pad_0x3BA[2];
    void (*fn)(void*);
};

struct sExecPurgeMgr {
    char pad_0x00[0x10D8];
    sExecPurgeVTable* vtable;
};

extern sExecPurgeMgr* D_004A5B80;

//99.95%
INCLUDE_ASM("main/ssxapp", cSSXApp_loadexecpurge__FPv);
#ifdef SKIP_ASM
void cSSXApp_loadexecpurge(void* self)
{
    func_002B4B48(self);
    func_00284C28();
    cAppMan_loadexecpurge(self);
    sExecPurgeVTable* vt = D_004A5B80->vtable;
    vt->fn((char*)D_004A5B80 + vt->field_0x3B8);
}
#endif

INCLUDE_ASM("main/ssxapp", func_00228C08);

INCLUDE_ASM("main/ssxapp", initOnline);

INCLUDE_ASM("main/ssxapp", func_00229180);

INCLUDE_ASM("main/ssxapp", func_00229278);

INCLUDE_ASM("main/ssxapp", func_002292E0);

INCLUDE_ASM("main/ssxapp", func_00229398);

INCLUDE_ASM("main/ssxapp", func_00229408);

//100%
INCLUDE_ASM("main/ssxapp", func_00229498);
#ifdef SKIP_ASM
extern "C" void func_00229398(void* self);
extern "C" void func_002292E0(void* self);

extern "C" void func_00229498(void* self)
{
    func_00229398(self);
    func_002292E0(self);
}
#endif

INCLUDE_ASM("main/ssxapp", func_002294C8);

INCLUDE_ASM("main/ssxapp", func_00229530);

//100%
INCLUDE_ASM("main/ssxapp", func_00229738);
#ifdef SKIP_ASM
struct sQuad229 {
    int v[4];
} __attribute__((aligned(16)));

struct sRingEntry {
    sQuad229 q;     // 0x0
    int value;      // 0x10
    int pad[3];
};

struct sRingOwner {
    char pad_0x0[0x2214];
    int index;              // 0x2214
    int pad_0x2218[2];
    sRingEntry entries[40]; // 0x2220
};

extern "C" void func_00229738(sRingOwner* self, sQuad229* q, int value)
{
    self->entries[self->index].q = *q;
    self->entries[self->index].value = value;
    self->index++;
    self->index %= 40;
}
#endif

INCLUDE_ASM("main/ssxapp", func_00229788);

INCLUDE_ASM("main/ssxapp", func_002297D8);

INCLUDE_ASM("main/ssxapp", func_00229820);

INCLUDE_ASM("main/ssxapp", func_00229910);

//100%
INCLUDE_ASM("main/ssxapp", func_00229B90);
#ifdef SKIP_ASM
extern "C" void func_00229B90(void* self, int a1)
{
    ((unsigned int*)self)[a1 + 2] = 0xFFFFFFFFU;
}
#endif

INCLUDE_ASM("main/ssxapp", func_00229BA8);

INCLUDE_ASM("main/ssxapp", func_00229E20);

INCLUDE_ASM("main/ssxapp", func_00229E58);

//100%
INCLUDE_ASM("main/ssxapp", func_00229F30);
#ifdef SKIP_ASM
// Upload two 4x4 matrices (a, b) to VU0 data memory starting at qword 4 (address 0x40).
// PORT: PS2-only VU0 inline asm (ctc2/lqc2/vsqi); the PC port needs its own matrix store.
extern "C" void func_00229F30(void* a, void* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "ctc2.ni   %0, $vi1\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "vsqi.xyzw $vf1, ($vi1++)\n"
        "vsqi.xyzw $vf2, ($vi1++)\n"
        "vsqi.xyzw $vf3, ($vi1++)\n"
        "vsqi.xyzw $vf4, ($vi1++)\n"
        "lqc2      $vf1, 0x0(%2)\n"
        "lqc2      $vf2, 0x10(%2)\n"
        "lqc2      $vf3, 0x20(%2)\n"
        "lqc2      $vf4, 0x30(%2)\n"
        "vsqi.xyzw $vf1, ($vi1++)\n"
        "vsqi.xyzw $vf2, ($vi1++)\n"
        "vsqi.xyzw $vf3, ($vi1++)\n"
        "vsqi.xyzw $vf4, ($vi1++)\n"
        ".set reorder\n"
        :
        : "r"(0x40), "r"(a), "r"(b)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229F80__FPv);
#ifdef SKIP_ASM
void func_00229F80(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229F88);
#ifdef SKIP_ASM
// Upload a vector plus a 4x4 matrix (5 qwords) to VU0 data memory slot `index`,
// starting at qword 0x50 + index * 5.
// PORT: PS2-only VU0 inline asm (ctc2/lqc2/vsqi); the PC port needs its own store.
extern "C" void func_00229F88(int index, void* v, void* m)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "ctc2.ni   %0, $vi1\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x0(%2)\n"
        "lqc2      $vf3, 0x10(%2)\n"
        "lqc2      $vf4, 0x20(%2)\n"
        "lqc2      $vf5, 0x30(%2)\n"
        "vsqi.xyzw $vf1, ($vi1++)\n"
        "vsqi.xyzw $vf2, ($vi1++)\n"
        "vsqi.xyzw $vf3, ($vi1++)\n"
        "vsqi.xyzw $vf4, ($vi1++)\n"
        "vsqi.xyzw $vf5, ($vi1++)\n"
        ".set reorder\n"
        :
        : "r"(index * 5 + 0x50), "r"(v), "r"(m)
        : "memory");
}
#endif

INCLUDE_ASM("main/ssxapp", func_00229FC8);

INCLUDE_ASM("main/ssxapp", func_0022A128);

INCLUDE_ASM("main/ssxapp", func_0022A270);

INCLUDE_ASM("main/ssxapp", func_0022A368);

INCLUDE_ASM("main/ssxapp", func_0022A408);

INCLUDE_ASM("main/ssxapp", func_0022A4A8);

INCLUDE_ASM("main/ssxapp", func_0022A5A0);

INCLUDE_ASM("main/ssxapp", func_0022A698);

//100%
INCLUDE_ASM("main/ssxapp", func_0022A770);
#ifdef SKIP_ASM
struct sVisNode_A770 {
    sVisNode_A770* next;    // 0x00
    int pad_0x4;
    int type;               // 0x08
    char pad_0xC[0x44];
    int sphere[4];          // 0x50
};

struct sVisSrc_A770 {
    char pad_0x0[0x28];
    sVisNode_A770* head;    // 0x28
};

struct sVisList_A770 {
    sVisSrc_A770* src;
    int vuFrustum;
};

struct sVisOwner_A770 {
    char pad_0x0[0x78B4];
    int countA;                 // 0x78B4
    sVisNode_A770* nodesA[192]; // 0x78B8
    int pad_0x7BB8[2];
    int countB;                 // 0x7BC0
    sVisNode_A770* nodesB[1];   // 0x7BC4
};

// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallms/cfc2); the PC port needs a C
// version of the microprogram at 0xEF0 (sphere vs frustum test; 2 = outside).
static inline int visSphereTest_A770(int* sphere, int frustum)
{
    int r;
    __asm__ __volatile__(
        "lqc2      $vf22, 0x0(%1)\n"
        "ctc2.ni   %0, $vi14\n"
        "vcallms   0xEF0\n"
        "cfc2.i    %0, $vi1\n"
        : "=r"(r)
        : "r"(sphere), "0"(frustum));
    return r;
}

extern "C" void func_0022A770(sVisOwner_A770* self, sVisList_A770* list, int count)
{
    for (; count > 0; count--, list++) {
        sVisNode_A770* n = list->src->head;
        while (n != 0) {
            if (n->type == 7) {
                int frustum = list->vuFrustum;
                if (frustum == 0 || visSphereTest_A770(n->sphere, frustum) != 2) {
                    self->nodesA[self->countA++] = n;
                }
            } else if (n->type == 8) {
                self->nodesB[self->countB++] = n;
            }
            n = n->next;
        }
    }
}
#endif

INCLUDE_ASM("main/ssxapp", func_0022A830);

INCLUDE_ASM("main/ssxapp", func_0022ADD8);

INCLUDE_ASM("main/ssxapp", func_0022B008);

