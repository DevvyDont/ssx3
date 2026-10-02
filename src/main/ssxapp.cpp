#include "common.h"

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_cSSXApp);
#ifdef SKIP_ASM
extern "C" void* cAppMan_cAppMan(void* self);
extern "C" void func_003E6448(void* dst, int c, int n);
extern int D_004A3E90;
extern void* D_0047D9C8[];

struct cSSXApp00226830 {
    char base[0x5C];
    void** vt;
    int f60;
    int f64;
    int f68;
    int f6C;
    int f70;
    int f74;
    char pad78[0xA8 - 0x78];
    char fA8[8];
    char fB0[8];
    char fB8[8];
    int fC0;
    int fC4;
    int fC8;
    int fCC;
    int fD0;
    int fD4;
    int fD8;
    int fDC;
    int fE0;
    int fE4;
    int fE8;
    int arr[11];
    int f118;
};

extern "C" cSSXApp00226830* cSSXApp_cSSXApp(cSSXApp00226830* self)
{
    cAppMan_cAppMan(self);
    self->vt = D_0047D9C8;
    self->fC4 = D_004A3E90;
    self->fC8 = D_004A3E90;
    self->fCC = D_004A3E90;
    self->fD4 = 0;
    self->fD8 = 0;
    self->fDC = 0;
    self->fE0 = 0;
    self->fE4 = 0;
    self->fE8 = 0;
    int i;
    for (i = 0; i < 11; i++) {
        self->arr[i] = 0;
    }
    self->f118 = 0;
    self->f74 = 0;
    self->f68 = 0;
    self->f6C = 0;
    self->f70 = 0;
    self->f60 = 1;
    self->f64 = 1;
    func_003E6448(self->fA8, 0, 8);
    func_003E6448(self->fB8, 0, 8);
    func_003E6448(self->fB0, 0, 8);
    return self;
}
#endif

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

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_preUpdate);
#ifdef SKIP_ASM
extern "C" int func_00326B48(void* pad);
extern "C" void func_00255A20(void* p);
extern "C" void func_002668E8(void* p);
extern "C" int func_00326CA0(void* self, int i);
extern "C" void* func_00326CC8(void* self, int i);
extern "C" void func_00321298(void* target, int a, void* data);
extern void* D_004A28A0;
extern void* D_004A2EB8;
extern void* D_004A33F0;

extern "C" int cSSXApp_preUpdate(void* self)
{
    if (D_004A28A0 != 0 && func_00326B48(D_004A28A0) != 0) {
        if (D_004A2EB8 != 0) {
            func_00255A20(D_004A2EB8);
        }
        if (D_004A33F0 != 0) {
            func_002668E8(D_004A33F0);
        }
        int i;
        for (i = 0; i < 2; i++) {
            int a = func_00326CA0(D_004A28A0, i);
            void* d = func_00326CC8(D_004A28A0, i);
            func_00321298(((void**)((char*)self + 0xA8))[i], a, d);
        }
        return 1;
    }
    return 0;
}
#endif

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

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_startGameLoad);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_0022E968(void* self);
struct cAppMan;
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" void* cGameModeMan_getGM();
extern char D_0047A808[];

extern "C" void cSSXApp_startGameLoad(void* self)
{
    void* m = func_0022E968(cMemMan_alloc(0x22C, D_0047A808, 0, 0));
    *(void**)((char*)self + 0x84) = m;
    cAppMan_setNextModule((cAppMan*)self, (unsigned int)m);
    *(void**)((char*)self + 0xC0) = cGameModeMan_getGM();
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00228238);
#ifdef SKIP_ASM
extern int D_004A203C;
extern char D_0047A818[];
extern "C" void* func_001A1CE8(void* p);
extern "C" void* func_0017F7B8(void* p);

extern "C" void func_00228238(void* self)
{
    D_004A203C = 0;
    if (*(int*)((char*)self + 0x60) != 0) {
        void* m = func_001A1CE8(cMemMan_alloc(0xB5AE0, D_0047A818, 0, 0));
        *(void**)((char*)self + 0x7C) = m;
        cAppMan_setNextModule((cAppMan*)self, (unsigned int)m);
    } else {
        void* m = func_0017F7B8(cMemMan_alloc(0x238, D_0047A818, 0, 0));
        *(void**)((char*)self + 0x80) = m;
        cAppMan_setNextModule((cAppMan*)self, (unsigned int)m);
    }
}
#endif

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

//100%
INCLUDE_ASM("main/ssxapp", func_00229278);
#ifdef SKIP_ASM
extern "C" void func_00229398(void* self);
extern int D_004A2A00;
void operator_delete(int* p);
void cCrowdRender2D__cCrowdRender2D(int* self, int flags);

extern "C" void func_00229278(void* self, int flags)
{
    func_00229398(self);
    int* p = *(int**)((char*)self + 4);
    D_004A2A00 = 0;
    operator_delete(p);
    int* r = *(int**)self;
    if (r != 0) {
        cCrowdRender2D__cCrowdRender2D(r, 3);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_002292E0);
#ifdef SKIP_ASM
extern "C" void func_003E6448(void* dst, int c, int n);
unsigned int BXrand();

struct sCrowdSlot_2292E0 {
    char pad_0x00[0x30];
    int delay;              // 0x30
    char pad_0x34[0xC];
};

struct sCrowd_2292E0 {
    int pad0;
    int pad4;
    unsigned int ids[128];          // 0x8
    char pad_0x208[0x8];
    sCrowdSlot_2292E0 slots[128];   // 0x210
    int count;                      // 0x2210
    int count2;                     // 0x2214
    char pad_0x2218[0x8];
    char rest[0x500];               // 0x2220
};

extern "C" void func_002292E0(void* self)
{
    sCrowd_2292E0* c = (sCrowd_2292E0*)self;
    int i;
    c->count = 0;
    func_003E6448(c->slots, 0, 0x2000);
    for (i = 0; i < 128; i++) {
        c->ids[i] = 0xFFFFFFFF;
        c->slots[i].delay = BXrand() % 300 + 300;
    }
    c->count2 = 0;
    func_003E6448(c->rest, 0, 0x500);
}
#endif

INCLUDE_ASM("main/ssxapp", func_00229398);

//100%
INCLUDE_ASM("main/ssxapp", func_00229408);
#ifdef SKIP_ASM
extern "C" void func_00229B90(void* self, int a1);

extern "C" void func_00229408(void* self, int id)
{
    int i;
    for (i = 0; i < 0x80; i++) {
        unsigned* e = (unsigned*)((char*)self + 0x8) + i;
        if (*e != 0xFFFFFFFF && *(unsigned char*)e == id) {
            func_00229B90(self, i);
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("main/ssxapp", func_002294C8);
#ifdef SKIP_ASM
extern "C" void cCrowdAnim2D_update(void* p);
extern "C" void func_00229530(void* self);
extern short D_00536690[];

extern "C" void func_002294C8(void* self)
{
    int i;
    char* p;
    int t;
    cCrowdAnim2D_update(*(void**)((char*)self + 0x4));
    t = *(int*)((char*)*(void**)((char*)self + 0x4) + 0x10);
    D_00536690[0] = t;
    p = (char*)self;
    for (i = 0x27; i >= 0; i--) {
        if (*(int*)(p + 0x2230) > 0) {
            *(int*)(p + 0x2230) = *(int*)(p + 0x2230) - 1;
        }
        p += 0x20;
    }
    func_00229530(self);
}
#endif

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

//100%
INCLUDE_ASM("main/ssxapp", func_00229788);
#ifdef SKIP_ASM
extern "C" void func_00229910(void* self, int slot, int a1);

extern "C" void func_00229788(void* self, int a1)
{
    int i;
    for (i = 0; i < 0x40; i++) {
        if (((unsigned int*)((char*)self + 0x8))[i] == 0xFFFFFFFF) {
            func_00229910(self, i, a1);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_002297D8);
#ifdef SKIP_ASM
extern "C" void func_00229B90(void* self, int a1);

extern "C" void func_002297D8(void* self, int val)
{
    int i;
    for (i = 0; i < 0x40; i++) {
        if (((int*)self)[i + 2] == val) {
            func_00229B90(self, i);
            break;
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("main/ssxapp", func_00229E20);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
class cSSXAppStream {
public:
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

extern "C" void func_00229E20(void* self, cSSXAppStream* s)
{
    s->v01((char*)self + 0x8, 0x200);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229E58);
#ifdef SKIP_ASM
extern "C" void func_00229398(void* self);
extern "C" void func_002292E0(void* self);
extern "C" void func_00229910(void* self, int slot, int a1);

// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
class cSSXAppStream_229E58 {
public:
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

struct sCrowdId_229E58 {
    unsigned int id;
    sCrowdId_229E58() : id(0xFFFFFFFF) {}
};

extern "C" void func_00229E58(void* self, cSSXAppStream_229E58* s)
{
    func_00229398(self);
    func_002292E0(self);
    sCrowdId_229E58 ids[128];
    s->v02(ids, 0x200);
    int i;
    for (i = 0; i < 128; i++) {
        if (ids[i].id != 0xFFFFFFFF) {
            func_00229910(self, i, ids[i].id);
        }
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/ssxapp", func_0022A368);
#ifdef SKIP_ASM
extern "C" void func_00229FC8(void* self, void* node, int frustum);

struct sNode_A368 {
    sNode_A368* next;
};

struct sSrc_A368 {
    char pad_0x0[0x20];
    sNode_A368* head;   // 0x20
};

struct sList_A368 {
    sSrc_A368* src;
    int vuFrustum;
};

extern "C" void func_0022A368(void* self, sList_A368* list, int count)
{
    for (; count > 0; count--, list++) {
        sNode_A368* n = list->src->head;
        while (n != 0) {
            func_00229FC8(self, n, list->vuFrustum);
            n = n->next;
        }
    }
    if (*(int*)((char*)self + 0x50A4) != 0) {
        func_00229FC8(self, 0, 0);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/ssxapp", func_0022A408);
#ifdef SKIP_ASM
extern "C" void func_0022A128(void* self, void* node, int frustum);

struct sNode_A408 {
    sNode_A408* next;
};

struct sSrc_A408 {
    char pad_0x0[0x24];
    sNode_A408* head;   // 0x24
};

struct sList_A408 {
    sSrc_A408* src;
    int vuFrustum;
};

extern "C" void func_0022A408(void* self, sList_A408* list, int count)
{
    for (; count > 0; count--, list++) {
        sNode_A408* n = list->src->head;
        while (n != 0) {
            func_0022A128(self, n, list->vuFrustum);
            n = n->next;
        }
    }
    if (*(int*)((char*)self + 0x78B0) != 0) {
        func_0022A128(self, 0, 0);
    }
}
#endif

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

