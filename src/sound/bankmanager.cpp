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

INCLUDE_ASM("sound/bankmanager", func_0028B788);

INCLUDE_ASM("sound/bankmanager", func_0028B7E0);

INCLUDE_ASM("sound/bankmanager", func_0028B830);

INCLUDE_ASM("sound/bankmanager", func_0028B878);

INCLUDE_ASM("sound/bankmanager", cBankInstance_OnAsyncMainMemAlloc);

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

INCLUDE_ASM("sound/bankmanager", func_0028BDA8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028BDE0);
#ifdef SKIP_ASM
extern "C" void func_0028B650(void*);

extern "C" void func_0028BDE0(void* self, int i)
{
    func_0028B650((char*)*(void**)((char*)self + 0xACC) + i * 0x60);
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028BE60);

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

INCLUDE_ASM("sound/bankmanager", func_0028BEE0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/bankmanager", func_0028BF10);
#ifdef SKIP_ASM
int func_0028B928(void*);

extern "C" void func_0028BF10(void* self, int i)
{
    func_0028B928((char*)*(void**)((char*)self + 0xACC) + i * 0x60);
}
#endif

INCLUDE_ASM("sound/bankmanager", func_0028BF38);

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

INCLUDE_ASM("sound/bankmanager", func_0028E888);

INCLUDE_ASM("sound/bankmanager", func_0028E8C0);

INCLUDE_ASM("sound/bankmanager", func_0028EF90);

INCLUDE_ASM("sound/bankmanager", func_0028F000);

INCLUDE_ASM("sound/bankmanager", func_0028F108);

INCLUDE_ASM("sound/bankmanager", func_0028F140);

INCLUDE_ASM("sound/bankmanager", func_0028F200);

INCLUDE_ASM("sound/bankmanager", func_0028F2C0);

INCLUDE_ASM("sound/bankmanager", func_0028F328);

INCLUDE_ASM("sound/bankmanager", func_0028F3C8);

INCLUDE_ASM("sound/bankmanager", func_0028F478);

INCLUDE_ASM("sound/bankmanager", func_0028F520);

INCLUDE_ASM("sound/bankmanager", func_0028F558);

INCLUDE_ASM("sound/bankmanager", func_0028F5B8);

INCLUDE_ASM("sound/bankmanager", func_0028F678);

INCLUDE_ASM("sound/bankmanager", func_0028F700);

INCLUDE_ASM("sound/bankmanager", func_0028F730);

INCLUDE_ASM("sound/bankmanager", func_0028F768);

INCLUDE_ASM("sound/bankmanager", func_0028FA98);

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

