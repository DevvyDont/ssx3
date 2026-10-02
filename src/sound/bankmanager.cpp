#include "common.h"

INCLUDE_ASM("sound/bankmanager", cBankInstance_AllocMem);

INCLUDE_ASM("sound/bankmanager", func_0028B650);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028B730);
#ifdef SKIP_ASM
struct sBankNode {
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    sBankNode* next;
    sBankNode* prev;
};

extern "C" void func_0028B730(sBankNode* self)
{
    if (self->next != 0) {
        self->unkC = -1;
        self->unk10 = 0;
        self->unk14 = 0;
        self->unk8 = 0;
        if (self->next == self->prev) {
            self->next->next = 0;
            self->next->prev = 0;
        } else {
            self->next->prev = self->prev;
            self->prev->next = self->next;
        }
        self->prev = 0;
        self->next = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028B788);
#ifdef SKIP_ASM
extern "C" void func_0028B7E0(void*);
extern "C" int func_003B6098(void* sema, int a1);

extern "C" void func_0028B788(void* self, int a1, int a2)
{
    func_0028B7E0(self);
    *(int*)((char*)self + 0x10) = a1;
    *(int*)((char*)self + 0x14) = a2;
    func_003B6098((char*)self + 0x4, a1);
    *(int*)self = 1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028B7E0);
#ifdef SKIP_ASM
extern "C" int func_003B6300(int id);

extern "C" void func_0028B7E0(void* self)
{
    *(int*)self = 0;
    if (*(int*)((char*)self + 0x4) != -1) {
        func_003B6300(*(int*)((char*)self + 0x4));
        *(int*)((char*)self + 0x4) = -1;
    }
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028B830);
#ifdef SKIP_ASM
extern "C" int func_004165A8(void*, void*);

extern "C" int func_0028B830(void* self, void* name)
{
    int s = *(int*)self;
    if (s != 1) {
        return 0;
    }
    if (func_004165A8(name, (char*)self + 0x20) != 0) {
        s = 0;
    }
    return s;
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028B878);

//100%
INCLUDE_ASM("sound/bankmanager", cBankInstance_OnAsyncMainMemAlloc);
#ifdef SKIP_ASM
extern "C" void* func_00252FA0(void* a0, int a1, int a2);
extern char D_004829A8[];

// PORT: real body returns the buffer; a later caller in this unit declares it void(void*, int, int)
void* cBankInstance_OnAsyncMainMemAlloc_impl(void* self, int size, int flags) __asm__("cBankInstance_OnAsyncMainMemAlloc");

void* cBankInstance_OnAsyncMainMemAlloc_impl(void* self, int size, int flags)
{
    if (*(int*)((char*)self + 0x8) == 0) {
        *(void**)((char*)self + 0x10) = func_00252FA0(D_004829A8, size, flags);
        *(int*)((char*)self + 0x14) = size;
    }
    return *(void**)((char*)self + 0x10);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028B928__FPv);
#ifdef SKIP_ASM
int func_0028B928(void* self)
{
    return *(int*)((char*)self + 0x14);
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028B930);

INCLUDE_ASM("sound/bankmanager", cBankManager_cBankManager);

INCLUDE_ASM("sound/bankmanager", func_0028BB10);

INCLUDE_ASM("sound/bankmanager", func_0028BC58);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028BCE8);
#ifdef SKIP_ASM
extern "C" void func_0028B528(void*);

extern "C" void func_0028BCE8(void* self, int i)
{
    func_0028B528((char*)*(void**)((char*)self + 0xACC) + i * 0x60);
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028BD10);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028BDA8);
#ifdef SKIP_ASM
extern "C" void* cBankInstance_AllocMem(void* inst, int a, int b, int c);

extern "C" void* func_0028BDA8(void* self, int i, int a, int b, int c)
{
    return cBankInstance_AllocMem((char*)*(void**)((char*)self + 0xACC) + i * 0x60, a, b, c);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028BDE0);
#ifdef SKIP_ASM
extern "C" void func_0028B650(void*);

extern "C" void func_0028BDE0(void* self, int i)
{
    func_0028B650((char*)*(void**)((char*)self + 0xACC) + i * 0x60);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028BE60);
#ifdef SKIP_ASM
extern "C" void func_0028B788(void* inst, int a, int b);

extern "C" void func_0028BE60(void* self, int i, int a, int b)
{
    func_0028B788((char*)*(void**)((char*)self + 0xACC) + i * 0x60, a, b);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028BE90);
#ifdef SKIP_ASM
extern "C" void func_0028B7E0(void*);

extern "C" void func_0028BE90(void* self, int i)
{
    func_0028B7E0((char*)*(void**)((char*)self + 0xACC) + i * 0x60);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028BEB8);
#ifdef SKIP_ASM
extern "C" void func_0028B878(void*);

extern "C" void func_0028BEB8(void* self, int i)
{
    func_0028B878((char*)*(void**)((char*)self + 0xACC) + i * 0x60);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028BEE0);
#ifdef SKIP_ASM
extern "C" void cBankInstance_OnAsyncMainMemAlloc(void* inst, int a, int b);

extern "C" void func_0028BEE0(void* self, int i, int a, int b)
{
    cBankInstance_OnAsyncMainMemAlloc((char*)*(void**)((char*)self + 0xACC) + i * 0x60, a, b);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028BF10);
#ifdef SKIP_ASM
int func_0028B928(void*);

extern "C" void func_0028BF10(void* self, int i)
{
    func_0028B928((char*)*(void**)((char*)self + 0xACC) + i * 0x60);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028BF38);
#ifdef SKIP_ASM
extern "C" void func_0028B930(void* inst, int a, int b, int c, int d, int e);

extern "C" void func_0028BF38(void* self, int i, int a, int b, int c, int d, int e)
{
    func_0028B930((char*)*(void**)((char*)self + 0xACC) + i * 0x60, a, b, c, d, e);
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028BF78);

INCLUDE_ASM("sound/bankmanager", func_0028C2D0);

INCLUDE_ASM("sound/bankmanager", func_0028C430);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028C8C0__FPvi);
#ifdef SKIP_ASM
void func_0028C8C0(void* self, int val)
{
    *(int*)((char*)self + 0x6250) = val;
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028C8C8);

INCLUDE_ASM("sound/bankmanager", func_0028CD48);

INCLUDE_ASM("sound/bankmanager", func_0028CDF8);

INCLUDE_ASM("sound/bankmanager", func_0028CF98);

INCLUDE_ASM("sound/bankmanager", func_0028D488);

INCLUDE_ASM("sound/bankmanager", func_0028D5A0);

INCLUDE_ASM("sound/bankmanager", func_0028D630);

INCLUDE_ASM("sound/bankmanager", func_0028D740);

INCLUDE_ASM("sound/bankmanager", func_0028D7D8);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D898__FPv);
#ifdef SKIP_ASM
int func_0028D898(void* self)
{
    return *(int*)((char*)self + 0x623C);
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028D8A0);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D960);
#ifdef SKIP_ASM
extern "C" int func_0028D960(void* self)
{
    unsigned int s = *(unsigned int*)((char*)self + 0x608c);
    if (s < 2 || s == 3) {
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028D988);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028DEF0);
#ifdef SKIP_ASM
extern "C" int func_0028DEF0(void* self, int a1)
{
    int old = *(int*)((char*)self + 0x629c);
    *(int*)((char*)self + 0x629c) = a1;
    return old != a1;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028DF08__FPv);
#ifdef SKIP_ASM
void func_0028DF08(void* self)
{
    *(int*)((char*)self + 0x629C) = -1;
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028DF18);

INCLUDE_ASM("sound/bankmanager", func_0028E100);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028E888);
#ifdef SKIP_ASM
extern "C" void func_0028E8C0(void* self, int id, int a2);

extern "C" void func_0028E888(void* self)
{
    if (*(int*)((char*)self + 0x6254) != 0) {
        func_0028E8C0(self, 0x13, 1);
        *(int*)((char*)self + 0x6254) = 0;
    }
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028E8C0);

INCLUDE_ASM("sound/bankmanager", func_0028EF90);

INCLUDE_ASM("sound/bankmanager", func_0028F000);

INCLUDE_ASM("sound/bankmanager", func_0028F108);

INCLUDE_ASM("sound/bankmanager", func_0028F140);

INCLUDE_ASM("sound/bankmanager", func_0028F200);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F2C0);
#ifdef SKIP_ASM
extern "C" void* func_002B3AC0(void*);

struct sBankMgrVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0028F2C0(void* self)
{
    if (*(int*)((char*)self + 0x6294) != 0) {
        func_002B3AC0((char*)self + 0x118);
    } else if (*(int*)((char*)self + 0x6298) != 0) {
        char* obj = (char*)self + 0x118;
        sBankMgrVEntry* vt = *(sBankMgrVEntry**)((char*)self + 0x5558);
        vt[4].fn(obj + vt[4].delta, 0x24);
    }
    *(int*)((char*)self + 0x6294) = 0;
    *(int*)((char*)self + 0x6298) = 0;
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028F328);

INCLUDE_ASM("sound/bankmanager", func_0028F3C8);

INCLUDE_ASM("sound/bankmanager", func_0028F478);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F520);
#ifdef SKIP_ASM
extern "C" int func_0028D960(void* self);
extern "C" void* func_002B3AC0(void*);

extern "C" void func_0028F520(void* self)
{
    if (func_0028D960(self) != 0) {
        func_002B3AC0((char*)self + 0x118);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028F558);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" void func_0028EF90(void* self);

extern "C" void func_0028F558(void* self, int id)
{
    *(int*)((char*)self + 0x6284) = id;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (iface != 0) {
        if (*func_00144BC0(iface) == *(int*)((char*)self + 0x6284)) {
            func_0028EF90(self);
        }
    }
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028F5B8);

INCLUDE_ASM("sound/bankmanager", func_0028F678);

INCLUDE_ASM("sound/bankmanager", func_0028F700);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028F730);
#ifdef SKIP_ASM
extern "C" void func_0028FA98(void* self, float v);
extern "C" void func_0028BCE8(void* self, int i);

extern "C" void func_0028F730(void* self)
{
    func_0028FA98(self, 0.0f);
    func_0028BCE8(**(void***)((char*)self + 0x118), 0);
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028F768);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028FA98);
#ifdef SKIP_ASM
extern "C" void func_002AD5F0(void* p, int idx, int a2, float v);

extern "C" void func_0028FA98(void* self, float v)
{
    int idx = *(int*)((char*)self + 0x62A8);
    if (idx >= 0) {
        func_002AD5F0((char*)**(void***)((char*)self + 0x118) + 0x1D8, idx, 1, v);
        *(int*)((char*)self + 0x62A8) = -1;
    }
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028FAE0);

extern "C" void* func_002B3AC0(void*);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028FC38__FPv);
#ifdef SKIP_ASM
void* func_0028FC38(void* self)
{
    return func_002B3AC0((char*)self + 0x118);
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028FC58);

