#include "common.h"

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_initBlock);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char D_00481730[];

// same body as func_0026E950 (setBlock), inlined here
static inline void cReplayFramePtr_setBlockInl(void* self, char* buf, int size)
{
    *(char**)((char*)self + 0x4) = buf;
    *(char**)((char*)self + 0x8) = buf + size;
    *(char**)((char*)self + 0xc) = buf;
    *(int*)((char*)self + 0x10) = size;
}

extern "C" void cReplayFramePtr_initBlock(void* self, unsigned int size)
{
    cReplayFramePtr_setBlockInl(self, (char*)operator_new_tag(size, D_00481730, 0, 0), 0);
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026E950);
#ifdef SKIP_ASM
extern "C" void func_0026E950(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0x4) = a1;
    *(int*)((char*)self + 0x8) = a1 + a2;
    *(int*)((char*)self + 0xc) = a1;
    *(int*)((char*)self + 0x10) = a2;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026E968);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

extern "C" void func_0026E968(void* self)
{
    void* p = *(void**)((char*)self + 0x4);
    if (p != 0) {
        cMemMan_free(p);
    }
    cReplayFramePtr_setBlockInl(self, 0, 0);
}
#endif

struct cReplayFramePtr {
    char pad_0x00[0x4];
    int field_0x4;
    char pad_0x08[0x4];
    int field_0xC;
};

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_readRewind__FP15cReplayFramePtr);
#ifdef SKIP_ASM
void cReplayFramePtr_readRewind(cReplayFramePtr* self)
{
    self->field_0xC = self->field_0x4;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026E9C0__FPv);
#ifdef SKIP_ASM
int func_0026E9C0(void* self)
{
    int t0 = *(int*)((char*)self + 0x4);
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x8) = t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026E9D0);
#ifdef SKIP_ASM
extern "C" void func_003E6574(void* dst, void* src, int size);

extern "C" void func_0026E9D0(void* self, void* src, int n)
{
    func_003E6574(*(char**)((char*)self + 0x8), src, n);
    *(char**)((char*)self + 0x8) += n;
    *(int*)((char*)self + 0x10) += n;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026EA20);
#ifdef SKIP_ASM
extern "C" void func_003E6574(void* dst, void* src, int size);

extern "C" void func_0026EA20(void* self, void* dst, int n)
{
    func_003E6574(dst, *(char**)((char*)self + 0xC), n);
    *(char**)((char*)self + 0xC) += n;
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026EA68);
#ifdef SKIP_ASM
class cStream26EA68 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

// Peek the next frame header's id without consuming it.
extern "C" short func_0026EA68(cStream26EA68* self)
{
    struct {
        short id;
        short size;
    } hdr;
    int pos = *(int*)((char*)self + 0xC);
    self->v02(&hdr, 4);
    *(int*)((char*)self + 0xC) = pos;
    return hdr.id;
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026EAB8);

INCLUDE_ASM("replay/frameptr", func_0026EB48);

INCLUDE_ASM("replay/frameptr", func_0026EC08);

//100%
INCLUDE_ASM("replay/frameptr", func_0026ECD8);
#ifdef SKIP_ASM
struct sFpSerVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sFpSerObj {
    sFpSerVEntry* vt;
};

extern "C" void func_0026ECD8(sFpSerObj* out, void* obj)
{
    unsigned int id = 0xFFFFFFFF;
    if (obj != 0) {
        id = *(unsigned int*)((char*)obj + 0x78);
    }
    out->vt[1].fn((char*)out + out->vt[1].delta, &id, 4);
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026ED28);
#ifdef SKIP_ASM
extern "C" void func_0026ED28(sFpSerObj* out, void* obj)
{
    struct {
        unsigned int id;
        unsigned short type;
    } rec;
    rec.id = 0xFFFFFFFF;
    void* p = *(void**)((char*)obj + 0x18);
    if (p != 0) {
        rec.id = *(unsigned int*)((char*)p + 0x78);
        rec.type = *(unsigned short*)((char*)p + 0x8);
    } else {
        rec.type = 0;
    }
    out->vt[1].fn((char*)out + out->vt[1].delta, &rec, 8);
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026ED88__FPv);
#ifdef SKIP_ASM
void func_0026ED88(void* self)
{
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", cReplayFramePtr_getFrameBlock__Fv);
#ifdef SKIP_ASM
void cReplayFramePtr_getFrameBlock()
{
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026ED98);
#ifdef SKIP_ASM
extern "C" void func_0026EDD8(void* self);

extern "C" void func_0026ED98(void* self)
{
    *(int*)((char*)self + 0x630) = (*(int*)((char*)self + 0x630) + 1) % 9;
    func_0026EDD8(self);
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026EDD8);

//100%
INCLUDE_ASM("replay/frameptr", func_0026EE68);
#ifdef SKIP_ASM
void* func_0016D1D8(void* p);
extern void* D_004C5830[];
extern "C" void func_0026EDD8(void* self);

extern "C" void func_0026EE68(void* self)
{
    func_0016D1D8(D_004C5830);
    func_0026EDD8(self);
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026EEA0);

//100%
INCLUDE_ASM("replay/frameptr", func_0026EF30);
#ifdef SKIP_ASM
extern "C" void func_0026EF30(void* self)
{
    func_0016D1D8(D_004C5830);
    *(int*)((char*)self + 0x634) = (*(int*)((char*)self + 0x634) + 1) % 9;
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026EF80);

INCLUDE_ASM("replay/frameptr", func_0026EFB8);

INCLUDE_ASM("replay/frameptr", func_0026F088);

INCLUDE_ASM("replay/frameptr", func_0026F180);

INCLUDE_ASM("replay/frameptr", func_0026F228);

INCLUDE_ASM("replay/frameptr", func_0026F428);

//100%
INCLUDE_ASM("replay/frameptr", func_0026F498__FPv);
#ifdef SKIP_ASM
void func_0026F498(void* self)
{
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026F4A0__FPvi);
#ifdef SKIP_ASM
void func_0026F4A0(void* self, int val)
{
    *(int*)((char*)self + 0x0) = val;
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026F4A8);

//100%
INCLUDE_ASM("replay/frameptr", func_0026F7B8);
#ifdef SKIP_ASM
void func_0026F4A0(void* self, int state);

extern "C" void func_0026F7B8(int* self)
{
    int s = *self;
    bool ok = !(s == 14 || s == 15);
    if (ok) {
        if (s == 10) {
            func_0026F4A0(self, 11);
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026F7F8);
#ifdef SKIP_ASM
struct sFrameDesc26F7F8 {
    short type;
    char a;
    char b;
};

extern "C" void* func_0026D760(void* self, sFrameDesc26F7F8* desc);
// PORT: func_0026D740 is mangled with one parameter (__FPv) but is called here
// with (self, block); bind the 2-arg form to the symbol.
void func_0026D740_2(void* self, void* block) __asm__("func_0026D740__FPv");

extern "C" void func_0026F7F8(void* self)
{
    if (*(void**)((char*)self + 0x3D0) == 0) {
        sFrameDesc26F7F8 desc;
        desc.type = 1;
        desc.a = 0;
        desc.b = 0;
        void* block = func_0026D760(self, &desc);
        *(void**)((char*)self + 0x3D0) = block;
        func_0026D740_2(self, block);
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026F850);
#ifdef SKIP_ASM
extern "C" void func_0026DB88(void* self, void* p);
extern "C" void func_00270238(void* self, void* p);

extern "C" void func_0026F850(void* self)
{
    void* p = *(void**)((char*)self + 0x3D0);
    if (p != 0) {
        func_0026DB88(self, p);
        func_00270238(self, *(void**)((char*)self + 0x3D0));
        *(void**)((char*)self + 0x3D0) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/frameptr", func_0026F898__FPvi);
#ifdef SKIP_ASM
void func_0026F898(void* self, int val)
{
    *(int*)((char*)self + 0x28) = val;
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026F8A0);

INCLUDE_ASM("replay/frameptr", func_0026F980);

//100%
INCLUDE_ASM("replay/frameptr", func_0026FA50);
#ifdef SKIP_ASM
extern "C" void func_0026FA50(void* self)
{
    int s = *(int*)((char*)self + 0x0);
    bool ok = !(s == 0xE || s == 0xF);
    if (ok) {
        *(int*)((char*)self + 0x60C) = 1;
    }
}
#endif

INCLUDE_ASM("replay/frameptr", func_0026FA78);

INCLUDE_ASM("replay/frameptr", func_0026FB88);

INCLUDE_ASM("replay/frameptr", func_0026FE50);

