#include "common.h"

INCLUDE_ASM("sound/bankmanager", cBankInstance_AllocMem);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028B650);
#ifdef SKIP_ASM
extern "C" void func_0028B528(void*);
extern "C" void* func_002523A8(void* self);
extern char* D_004A3614;

class cSndStream_B650 {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void stop(int id);
};

extern "C" void func_0028B650(void* self)
{
    func_0028B528(self);
    int state = *(int*)((char*)self + 0x8);
    if (state != 0) {
        if (state == 2) {
            (**(cSndStream_B650***)(D_004A3614 + 0x1D8))->stop(*(int*)((char*)self + 0xC));
            *(int*)((char*)self + 0xC) = -1;
        }
        func_002523A8(*(void**)((char*)self + 0x10));
        *(int*)((char*)self + 0x10) = 0;
        *(int*)((char*)self + 0x14) = 0;
        *(int*)((char*)self + 0x8) = 0;
    }
}
#endif

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

//100%
INCLUDE_ASM("sound/bankmanager", func_0028B878);
#ifdef SKIP_ASM
extern "C" void func_0028B528(void*);
extern "C" void func_0028B2D0(void* self);
extern char* D_004A3614;

class cSndStream_B878 {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5(int id);
    virtual void v6(int id);
};

extern "C" void func_0028B878(void* self)
{
    func_0028B528(self);
    func_0028B2D0(self);
    *(int*)self = 2;
    if (*(int*)((char*)self + 0x8) == 2) {
        (**(cSndStream_B878***)(D_004A3614 + 0x1D8))->v6(*(int*)((char*)self + 0xC));
    }
}
#endif

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

//100%
INCLUDE_ASM("sound/bankmanager", func_0028B930);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern char* D_004A3614;

class cSndStream_B930 {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5(int id);
    virtual void v6(int id);
    virtual void v7();
};

// PORT: name is passed as int (matches the unit's 6-int caller declaration); d, e unused.
extern "C" void func_0028B930(void* self, int id, int flag, int name, int d, int e)
{
    (**(cSndStream_B930***)(D_004A3614 + 0x1D8))->v7();
    *(int*)((char*)self + 0x4) = id;
    if (flag == 0) {
        *(int*)self = 1;
    } else {
        *(int*)self = 0;
    }
    strcpy((char*)self + 0x20, (const char*)name);
}
#endif

INCLUDE_ASM("sound/bankmanager", cBankManager_cBankManager);

INCLUDE_ASM("sound/bankmanager", func_0028BB10);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028BC58);
#ifdef SKIP_ASM
extern "C" void func_0028A728(void* self, int i);
extern "C" void func_0028B320(void* bank, void* id, int a3);

extern "C" void func_0028BC58(void* self, int i, void* id, int a3)
{
    int off = i * 0x60;
    if (func_0028B830((char*)*(void**)((char*)self + 0xACC) + off, id) == 0) {
        func_0028A728(self, i);
        func_0028B320((char*)*(void**)((char*)self + 0xACC) + off, id, a3);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028BCE8);
#ifdef SKIP_ASM
extern "C" void func_0028B528(void*);

extern "C" void func_0028BCE8(void* self, int i)
{
    func_0028B528((char*)*(void**)((char*)self + 0xACC) + i * 0x60);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028BD10);
#ifdef SKIP_ASM
extern "C" void func_0028A728(void* self, int i);
extern "C" void func_0028A558(void* self, int i, void* id, int a3, int a4);

struct sBankInst60 { char pad0[8]; int f8; char pad1[0x60 - 0xC]; };

extern "C" void func_0028BD10(void* self, int i, void* id, int a3)
{
    if (func_0028B830((char*)*(void**)((char*)self + 0xACC) + i * 0x60, id) == 0) {
        func_0028A728(self, i);
        func_0028A558(self, i, id, (*(sBankInst60**)((char*)self + 0xACC))[i].f8, a3);
    }
}
#endif

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

//100%
INCLUDE_ASM("sound/bankmanager", func_0028CD48);
#ifdef SKIP_ASM
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" void func_002B49C0(void* self, int a1);
extern "C" void func_002B3C28(void* self, int a1);
extern char D_004A3628[];

struct sBmVtCD48 {
    short delta;
    short index;
    void* fn;
};

typedef void (*tBmLoadCD48)(void*, void*, int, void*, int);
typedef int (*tBmSetCD48)(void*, int);

extern "C" void func_0028CD48(void* self)
{
    char* mon = (char*)self + 0x118;
    sBmVtCD48* vt = *(sBmVtCD48**)((char*)self + 0x5558);
    ((tBmLoadCD48)vt[2].fn)(mon + vt[2].delta, D_004A3628, 0xC, func_00287968(self, 1, 0), 1);
    func_002B49C0(mon, 0x191);
    sBmVtCD48* vt2 = *(sBmVtCD48**)((char*)self + 0x5558);
    ((tBmSetCD48)vt2[4].fn)(mon + vt2[4].delta, 1);
    func_002B3C28(mon, 0x7F);
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028CDF8);

INCLUDE_ASM("sound/bankmanager", func_0028CF98);

INCLUDE_ASM("sound/bankmanager", func_0028D488);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D5A0);
#ifdef SKIP_ASM
extern "C" int func_002B49E0(void* monitor);
extern "C" int func_002A1E20(void* self, int id);
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);

extern "C" void func_0028D5A0(void* self, int id)
{
    int cur;
    int type;
    int want;
    if (*(int*)((char*)self + 0x6284) == 0x17) {
        return;
    }
    cur = func_002B49E0((char*)self + 0x118);
    type = func_002A1E20(self, id);
    if (type == 1) {
        want = 0x65;
    } else if (type == 2) {
        want = 0x66;
    } else {
        want = 0x67;
    }
    if (want != cur) {
        func_0028CF98(self, 0, 0, type, 0);
    }
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028D630);

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D740);
#ifdef SKIP_ASM
extern "C" void func_0028D7D8(void* self, void* a1);

struct sVtEnt { short delta; short index; int (*fn)(void*); };

extern "C" void func_0028D740(void* self, int id, void* obj)
{
    int ok;
    if (*(int*)((char*)self + 0x6240) == id) {
        return;
    }
    ok = 0;
    if (*(int*)((char*)self + 0x62A0) != -1) {
        char* o = (char*)obj + 0x6C0;
        sVtEnt* e = &(*(sVtEnt**)o)[7];
        ok = e->fn(o + e->delta) == *(int*)((char*)self + 0x62A0);
    }
    if (ok) {
        if (*(int*)((char*)self + 0x623C) == id) {
            *(int*)((char*)self + 0x6240) = id;
            return;
        }
        func_0028D7D8(self, obj);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D7D8);
#ifdef SKIP_ASM
struct sBmVtD7D8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

// PORT: the unit declares func_0028D7D8 as returning void; it really returns 0/1.
int func_0028D7D8_impl(void* self, void* obj) __asm__("func_0028D7D8");

int func_0028D7D8_impl(void* self, void* obj)
{
    int ok = 0;
    if (obj == 0) {
        ok = 1;
        *(int*)((char*)self + 0x62A0) = -1;
    } else {
        int none = -1;
        if (*(int*)((char*)self + 0x62A0) != none) {
            char* o = (char*)obj + 0x6C0;
            sVtEnt* e = &(*(sVtEnt**)o)[7];
            if (e->fn(o + e->delta) == *(int*)((char*)self + 0x62A0)) {
                *(int*)((char*)self + 0x62A0) = none;
                ok = 1;
            }
        }
    }
    if (ok) {
        int n = *(int*)((char*)self + 0x623C);
        if (n != 0) {
            char* mon = (char*)self + 0x118;
            sBmVtD7D8* vt = *(sBmVtD7D8**)((char*)self + 0x5558);
            vt[4].fn(mon + vt[4].delta, n + 1);
            *(int*)((char*)self + 0x623C) = 0;
            *(int*)((char*)self + 0x6240) = 0;
        }
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D898__FPv);
#ifdef SKIP_ASM
int func_0028D898(void* self)
{
    return *(int*)((char*)self + 0x623C);
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028D8A0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* func_00144BC0(void*);
extern "C" int func_002A4318(void* self);
extern "C" int func_002A4368(void* self);
extern "C" int func_002A40E0(void* self, int id);
extern "C" int func_002A4158(void* self, int id);
extern "C" int func_002A41C8(void* self, int id);
extern "C" int func_002A4238(void* self, int id);

extern "C" int func_0028D8A0(void* self)
{
    int id = *func_00144BC0(cBE_getInterface_Fv(cBE_getBE(), 0));
    int r;
    if (func_002A4318(self)) {
        r = 0;
    } else if (func_002A4368(self)) {
        r = 1;
    } else if (func_002A40E0(self, id)) {
        r = 0;
    } else if (func_002A4158(self, id)) {
        r = 1;
    } else if (func_002A41C8(self, id)) {
        r = 2;
    } else if (func_002A4238(self, id)) {
        r = 3;
    } else {
        r = 4;
    }
    return r;
}
#endif

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

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F328);
#ifdef SKIP_ASM
extern "C" void func_002B3B88(void* self, int a1);
extern "C" int func_002B49E0(void* monitor);
extern int D_004A3620;

class cBankMonitor_F328 {
public:
    char data[0x5440];
    virtual void v1();
    virtual void v2();
    virtual void v3(int a1);
};

extern "C" void func_0028F328(void* self, int code)
{
    void* monitor = (char*)self + 0x118;
    func_002B3B88(monitor, code);
    if (func_002B49E0(monitor) == -1) {
        if ((unsigned)(code - 10) < 2 || code == 0x12) {
            if (D_004A3620 == 1) {
                ((cBankMonitor_F328*)((char*)self + 0x118))->v3(8);
            }
            *(int*)((char*)self + 0x530) = 0;
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F5B8);
#ifdef SKIP_ASM
extern "C" int func_0028B1B0(void);
extern "C" void func_0029CE28(void* self);
extern "C" void func_00294F78(void* self, int a1);
extern "C" void func_002B3D48(void* self, float v);
extern "C" void func_0028F768(void* self);
extern "C" void func_002B3C28(void* self, int a1);

struct sBmVtF5B8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

extern "C" void func_0028F5B8(void* self)
{
    *(int*)((char*)self + 0x6088) = 0;
    if (*(int*)((char*)self + 0x5FB4) != 0) return;
    if (func_0028B1B0() == 0) return;
    if (*(int*)((char*)self + 0x5818) == 0) {
        func_0029CE28(self);
        *(int*)((char*)self + 0x6088) = 1;
    }
    func_00294F78(self, 9);
    if (*(int*)((char*)self + 0x608C) != 2) {
        char* mon = (char*)self + 0x118;
        func_002B3D48(mon, 1.0f);
        func_0028F768(self);
        sBmVtF5B8* vt = *(sBmVtF5B8**)((char*)self + 0x5558);
        vt[4].fn(mon + vt[4].delta, 0);
        func_002B3C28(mon, 0x7F);
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F678);
#ifdef SKIP_ASM
extern "C" void func_0029CE70(void* self);
extern "C" int func_002A3F90(void* self);
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_0028FA98(void* self, float v);

extern "C" void func_0028F678(void* self, int a1)
{
    if (*(int*)((char*)self + 0x6088) != 0) {
        func_0029CE70(self);
        *(int*)((char*)self + 0x6088) = 0;
    }
    if (*(int*)((char*)self + 0x62A8) >= 0) {
        func_0028FA98(self, 1.0f);
        if (a1 != 0 && func_002A3F90(self) != 0) {
            func_0028CF98(self, 0, 0, -1, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/bankmanager", func_0028F700);
#ifdef SKIP_ASM
extern "C" void func_0028BC58(void* self, int i, void* id, int a3);
extern void* D_004A3624;

extern "C" void func_0028F700(void* self)
{
    func_0028BC58(**(void***)((char*)self + 0x118), 0xD, D_004A3624, 0);
}
#endif

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

//100%
INCLUDE_ASM("sound/bankmanager", func_0028FC58);
#ifdef SKIP_ASM
extern "C" void* func_00287968(void* self, int a1, int a2);
extern "C" void func_002B49C0(void* self, int a1);
extern char D_004A3598[];

struct sBmVtFC58 {
    short delta;
    short index;
    void* fn;
};

typedef void (*tBmLoadFC58)(void*, void*, int, void*, int);
typedef int (*tBmSetFC58)(void*, int);

extern "C" void func_0028FC58(void* self)
{
    char* mon = (char*)self + 0x118;
    sBmVtFC58* vt = *(sBmVtFC58**)((char*)self + 0x5558);
    ((tBmLoadFC58)vt[2].fn)(mon + vt[2].delta, D_004A3598, 0xC, func_00287968(self, 1, 0), 1);
    func_002B49C0(mon, 0x12D);
    sBmVtFC58* vt2 = *(sBmVtFC58**)((char*)self + 0x5558);
    ((tBmSetFC58)vt2[4].fn)(mon + vt2[4].delta, *(int*)((char*)self + 0x6290));
}
#endif

