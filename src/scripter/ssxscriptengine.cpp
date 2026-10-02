#include "common.h"

INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_Load);

INCLUDE_ASM("scripter/ssxscriptengine", func_00278590);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00278710__FPv);
#ifdef SKIP_ASM
void func_00278710(void* self)
{
    *(int*)((char*)self + 0x554) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00278718__FPv);
#ifdef SKIP_ASM
void func_00278718(void* self)
{
    *(int*)((char*)self + 0x554) = 0;
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_00278720);

INCLUDE_ASM("scripter/ssxscriptengine", func_00278918);

extern "C" void* func_00283AF8(void*);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_002789C0__FPv);
#ifdef SKIP_ASM
void* func_002789C0(void* self)
{
    return func_00283AF8((char*)self + 0x500);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_002789E0);
#ifdef SKIP_ASM
extern "C" void func_002790F8(void* self);
extern "C" void func_00279488(void* self);

extern "C" void func_002789E0(void* self)
{
    func_002790F8(self);
    func_00279488(self);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_00278A10);

INCLUDE_ASM("scripter/ssxscriptengine", func_00278A58);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00278A98);
#ifdef SKIP_ASM
extern "C" void func_002790F8(void* self);
extern "C" void func_00279488(void* self);
extern "C" void func_00282390(void* self);

extern "C" void func_00278A98(void* self)
{
    func_002790F8(self);
    func_00279488(self);
    func_00282390(self);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_00278AD0);

INCLUDE_ASM("scripter/ssxscriptengine", func_00278B98);

INCLUDE_ASM("scripter/ssxscriptengine", func_00278CD8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", func_00278D58);
#ifdef SKIP_ASM
extern "C" void func_00278918(void*);
extern "C" int func_0027BF50(void*, int);

extern "C" int func_00278D58(void* self)
{
    int r;
    func_00278918(self);
    r = 0;
    if (*(int*)((char*)self + 0x538) == 0) {
        r = func_0027BF50(self, 1) == 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00278DA0);
#ifdef SKIP_ASM
extern "C" int func_002770C0(void*);

extern "C" int func_00278DA0(void* self)
{
    int i = *(int*)((char*)self + 0x550);
    if (i == 2) {
        return 1;
    }
    return func_002770C0(*(char**)((char*)self + 0x54C) + i * 0xCC);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00278DE8__FPvi);
#ifdef SKIP_ASM
void func_00278DE8(void* self, int val)
{
    *(int*)((char*)self + 0x554) = val;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00278E20);
#ifdef SKIP_ASM
struct sSsxScriptRider {
    char pad[0xCC];
};

extern "C" void func_00275DD8(sSsxScriptRider* r, int a, int b);

extern "C" void func_00278E20(void* self, int i, int a, int b)
{
    func_00275DD8(&(*(sSsxScriptRider**)((char*)self + 0x54C))[i], a, b);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00278E50);
#ifdef SKIP_ASM
extern "C" void func_00275ED0(void* e, int a, int b, int c, int d, int f, int g);

extern "C" void func_00278E50(void* self, int i, int a, int b, int c, int d, int f, int g)
{
    func_00275ED0(*(char**)((char*)self + 0x54C) + i * 0xCC, a, b, c, d, f, g);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00278E90);
#ifdef SKIP_ASM
extern "C" void func_00276048(void* e, int a, int b, int c, int d, int f);

extern "C" void func_00278E90(void* self, int i, int a, int b, int c, int d, int f)
{
    func_00276048(*(char**)((char*)self + 0x54C) + i * 0xCC, a, b, c, d, f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", func_00278ED0);
#ifdef SKIP_ASM
struct sScriptKeyEntry {
    int unk0;
    int key;
    int unk8;
};

extern "C" void func_00278ED0(void* self, int a1, int key, int b, int c, int d, int f)
{
    for (int i = 0; i < **(int**)((char*)self + 0x52C); i++) {
        if ((*(sScriptKeyEntry**)((char*)self + 0x530))[i].key == key) {
            func_00278E90(self, a1, i, b, c, d, f);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00278F38);
#ifdef SKIP_ASM
extern "C" void func_00276270(sSsxScriptRider* r, int a);

extern "C" void func_00278F38(void* self, int i)
{
    func_00276270(&(*(sSsxScriptRider**)((char*)self + 0x54C))[i], 0);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_00278F68);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279040);
#ifdef SKIP_ASM
void func_002766B0(void* r, int a);

extern "C" void func_00279040(void* self, int i, int a)
{
    func_002766B0(&(*(sSsxScriptRider**)((char*)self + 0x54C))[i], a);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279070);
#ifdef SKIP_ASM
extern "C" void func_002766D0(sSsxScriptRider* r, int a, int b);

extern "C" void func_00279070(void* self, int i, int a, int b)
{
    func_002766D0(&(*(sSsxScriptRider**)((char*)self + 0x54C))[i], a, b);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_002790A0);
#ifdef SKIP_ASM
extern "C" void func_00276868(void* self, int a1);
extern "C" void func_0027BF90(void* self);

extern "C" void func_002790A0(void* self, int i)
{
    func_00276868(*(char**)((char*)self + 0x54C) + i * 0xCC, 1);
    if (*(int*)((char*)self + 0x550) == i) {
        func_0027BF90(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", func_002790F8);
#ifdef SKIP_ASM
extern "C" void func_002790A0(void*, int);

extern "C" void func_002790F8(void* self)
{
    int i;
    for (i = 0; i < 2; i++) {
        func_002790A0(self, i);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279148);
#ifdef SKIP_ASM
extern "C" void func_00276998(sSsxScriptRider* r, int a);

extern "C" void func_00279148(void* self, int i, int a)
{
    func_00276998(&(*(sSsxScriptRider**)((char*)self + 0x54C))[i], a);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", func_002791D8);
#ifdef SKIP_ASM
extern "C" void func_00278918(void*);
extern "C" void func_00276B98(void*);

extern "C" void func_002791D8(void* self, int i)
{
    func_00278918(self);
    func_00276B98(*(char**)((char*)self + 0x54C) + i * 0xCC);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279220);
#ifdef SKIP_ASM
extern "C" void func_00276BD8(void*);

extern "C" void func_00279220(void* self, int i)
{
    func_00276BD8((char*)*(void**)((char*)self + 0x54C) + i * 0xCC);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279248);
#ifdef SKIP_ASM
void func_00276CA8(void*);

extern "C" void func_00279248(void* self, int i)
{
    func_00276CA8((char*)*(void**)((char*)self + 0x54C) + i * 0xCC);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279298);
#ifdef SKIP_ASM
int func_002772B8(void*);

extern "C" int func_00279298(void* self)
{
    int i = *(int*)((char*)self + 0x550);
    if (i == 2) {
        return 0;
    }
    return func_002772B8(*(char**)((char*)self + 0x54C) + i * 0xCC);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_002792E0);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279370);
#ifdef SKIP_ASM
extern "C" int func_002771C8(void*, int);

extern "C" int func_00279370(void* self, int a)
{
    int i = *(int*)((char*)self + 0x550);
    if (i == 2) {
        return 0;
    }
    return func_002771C8(*(char**)((char*)self + 0x54C) + i * 0xCC, a);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279488);
#ifdef SKIP_ASM
extern "C" void func_00276868(void* self, int a1);

extern "C" void func_00279488(void* self)
{
    for (int i = 0; i < 2; i++) {
        func_00276868(&(*(sSsxScriptRider**)((char*)self + 0x624))[i], 1);
    }
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_00279528);

INCLUDE_ASM("scripter/ssxscriptengine", func_002795D0);

INCLUDE_ASM("scripter/ssxscriptengine", func_00279720);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_002797A0);
#ifdef SKIP_ASM
extern "C" int* cSSXScriptEngine_GetScriptFromCategory(void*, int);

extern "C" int func_002797A0(void* self, int category)
{
    int* script = cSSXScriptEngine_GetScriptFromCategory(self, category);
    if (script == 0)
        return -1;
    return *script;
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_002797C8);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279840);
#ifdef SKIP_ASM
extern "C" int func_0027B578(void* self, int a1, int a2);
extern "C" void func_0027B4B8(void* self, int idx, int a);

extern "C" void func_00279840(void* self, int a1)
{
    func_0027B4B8(self, func_0027B578(self, a1, 2), a1);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279888);
#ifdef SKIP_ASM
extern "C" int func_0027B578(void* self, int a1, int a2);
extern "C" void func_0027B410(void* self, int idx);

extern "C" void func_00279888(void* self, int a1)
{
    int idx = func_0027B578(self, a1, 2);
    if (idx >= 0) {
        func_0027B410(self, idx);
    }
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_002798C0);

INCLUDE_ASM("scripter/ssxscriptengine", func_002799E0);

INCLUDE_ASM("scripter/ssxscriptengine", func_00279A70);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279CE0);
#ifdef SKIP_ASM
void func_0027BD90(void*, void*);

extern "C" void func_00279CE0(void* self, int unused, void* arg)
{
    func_0027BD90(self, arg);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_00279D00);

INCLUDE_ASM("scripter/ssxscriptengine", func_00279DA8);

INCLUDE_ASM("scripter/ssxscriptengine", func_00279E50);

extern "C" void* func_00275718(void*);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279ED8__FPv);
#ifdef SKIP_ASM
void* func_00279ED8(void* self)
{
    return func_00275718((char*)self + 0xa28);
}
#endif

extern "C" void* func_002836D8(void*);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00279EF8__FPv);
#ifdef SKIP_ASM
void* func_00279EF8(void* self)
{
    return func_002836D8((char*)self + 0xa38);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_00279F18);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027A0D8);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027A4A0);

extern "C" void* func_00282BB0(void* self);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027A668__FPv);
#ifdef SKIP_ASM
void* func_0027A668(void* self)
{
    return func_00282BB0(self);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027A688);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027A6B0);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027A720);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027A798);
#ifdef SKIP_ASM
extern "C" void func_002773A0(void* self, int a1, void* obj);
int func_00282BE0(void* self, void* a1);

extern "C" void func_0027A798(void* self, void* a1)
{
    int r = *(int*)((char*)self + 0x550);
    if (r != 2) {
        func_002773A0(*(char**)((char*)self + 0x54C) + r * 0xCC, func_00282BE0(self, a1), a1);
    }
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027A7F0);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027A860);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027A9F0);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027AA80);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027AAF8);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027AC60);

INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetScriptFromCategory);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027B268);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027B370);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027B410);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027B4B8);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027B528);
#ifdef SKIP_ASM
struct sSlot0628 {
    int a;
    int b;
    int c;
    int d;
};

struct sScriptEng0628 {
    char pad[0x628];
    sSlot0628 slots[10];
};

static inline int isSlotFree(sSlot0628* s)
{
    if (s->c == 0) {
        if (s->a == 0) return 1;
    }
    return 0;
}

extern "C" int func_0027B528(sScriptEng0628* self)
{
    for (int i = 2; i < 10; i++) {
        if (isSlotFree(&self->slots[i])) {
            return i;
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027B578);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027B5D8);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027B6B0);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027B750);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027B948);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027BB08);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027BD78__FPv);
#ifdef SKIP_ASM
void* func_0027BD78(void* self)
{
    void* head = *(void**)((char*)self + 0x548);
    *(void**)((char*)self + 0x548) = *(void**)((char*)head + 0x10);
    *(void**)((char*)head + 0x10) = 0;
    return head;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027BD90__FPvT0);
#ifdef SKIP_ASM
void func_0027BD90(void* self, void* a1)
{
    *(void**)((char*)a1 + 0x10) = *(void**)((char*)self + 0x548);
    *(int*)a1 = -1;
    *(int*)((char*)a1 + 0x4) = -1;
    *(int*)((char*)a1 + 0x8) = -1;
    *(int*)((char*)a1 + 0xc) = -1;
    *(void**)((char*)self + 0x548) = a1;
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027BDB8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", func_0027BF50);
#ifdef SKIP_ASM
extern "C" void func_00278918(void*);

extern "C" int func_0027BF50(void* self, int i)
{
    func_00278918(self);
    return *(int*)((char*)self + (i << 4) + 0x630) != 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027BF90);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_0028E888(void*);
extern "C" void func_002EF368(int);

extern "C" void func_0027BF90(void* self)
{
    if (*(int*)((char*)self + 0x550) != 2) {
        *(int*)((char*)self + 0x550) = 2;
        *(int*)((char*)self + 0x554) = 0;
        func_0028E888(func_0028B180());
        func_002EF368(0);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027BFD0);
#ifdef SKIP_ASM
void func_00282F60(void*);
void* func_00283200(void*, int);
extern void* D_00482318[];
extern void* D_004822E0[];

extern "C" void* func_0027BFD0(void* self)
{
    func_00282F60(self);
    func_00283200((char*)self + 0xC, 4);
    *(void***)((char*)self + 0x18) = D_00482318;
    *(void***)self = D_004822E0;
    return self;
}
#endif

void func_00283440(void*);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027C020);
#ifdef SKIP_ASM
extern "C" void func_0027C020(void* self)
{
    func_00283440((char*)self + 0xc);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027C040);
#ifdef SKIP_ASM
extern "C" int func_0027C040(void* self, void* a1, int a2)
{
    return a2 == 6;
}
#endif

extern "C" void func_0027CBB0(void*, void*);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027C050);
#ifdef SKIP_ASM
extern "C" void func_0027C050(void* self, void* a1)
{
    char buf[16];
    func_0027CBB0(a1, buf);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027C070__FPv);
#ifdef SKIP_ASM
int func_0027C070(void* self)
{
    return *(int*)((char*)self + 0x14);
}
#endif

extern "C" void* func_00274C38(void* self);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027C078__FPv);
#ifdef SKIP_ASM
void* func_0027C078(void* self)
{
    return func_00274C38(self);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027C098__FPv);
#ifdef SKIP_ASM
int func_0027C098(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027C0F0);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027C2A8__FPv);
#ifdef SKIP_ASM
int func_0027C2A8(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027C300);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027C4B8__FPv);
#ifdef SKIP_ASM
int func_0027C4B8(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027C510);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027C6A0__FPv);
#ifdef SKIP_ASM
int func_0027C6A0(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027C6F8);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027C888__FPv);
#ifdef SKIP_ASM
int func_0027C888(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027C8E0);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027C958__FPv);
#ifdef SKIP_ASM
int func_0027C958(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

extern "C" void* func_002742E0(void* self);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027C960__FPv);
#ifdef SKIP_ASM
void* func_0027C960(void* self)
{
    return func_002742E0(self);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027C9B0);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027CBB0);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027CC28);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027CDD0);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027CE58);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027CEA8);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027CF88);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D050);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D0C0);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D170);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D210);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027D2C8);
#ifdef SKIP_ASM
extern "C" int func_0027D2C8(void* self, int i, int val)
{
    char* p = (char*)self + i * 0x14;
    return *(int*)(p + 0x58) == val;
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D2E8);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027D2F0);
#ifdef SKIP_ASM
struct sVar0050 {
    int a;
    int b;
    int value;
    int type;
    int e;
};

struct sScriptEng0050 {
    char pad[0x50];
    sVar0050 vars[1];
};

extern "C" int func_0027D2F0(sScriptEng0050* self, void* a1)
{
    sVar0050* v = &self->vars[*(int*)((char*)a1 + 0x4)];
    if (v->type != 1) {
        return -1;
    }
    return v->value;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027D320__FPvi);
#ifdef SKIP_ASM
int func_0027D320(void* self, int i)
{
    return *(int*)((char*)self + (i << 2));
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D330);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D390);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D400);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D4A0);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027D540);
#ifdef SKIP_ASM
extern "C" void func_0027D540(void* self, void* a1, int a2)
{
    int i = *(int*)((char*)a1 + 0x4);
    char* p = (char*)self + i * 0x14;
    *(int*)(p + 0x60) = a2;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027D558);
#ifdef SKIP_ASM
extern "C" void func_0027D558(void* self, void* a1)
{
    int i = *(int*)((char*)a1 + 0x4);
    char* p = (char*)self + i * 0x14;
    *(int*)(p + 0x60) = -1;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027D578);
#ifdef SKIP_ASM
extern "C" int func_0027D578(sScriptEng0050* self, int key, int* out, int max)
{
    int n = 0;
    int i = 0;
    sVar0050* v = self->vars;
    while (i < 30 && n != max) {
        if (v->a != 0 && v->type != 1 && (v->e < 0 || v->e == key)) {
            *out++ = v->a;
            n++;
        }
        i++;
        if (i < 30) v = &self->vars[i];
    }
    return n;
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D5E8);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027D680__FPvi);
#ifdef SKIP_ASM
void func_0027D680(void* self, int val)
{
    *(int*)((char*)self + 0x10) = val;
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D688);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027D788__FPvf);
#ifdef SKIP_ASM
void func_0027D788(void* self, float val)
{
    *(float*)((char*)self + 0x18) = val;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", func_0027D790);
#ifdef SKIP_ASM
extern "C" void* func_0027D2E8(void);
extern "C" int func_0027D2C8(void* self, int i, int val);

extern "C" int func_0027D790(void* self, int val)
{
    if (*(void**)((char*)self + 0x14) == 0) {
        return 0;
    }
    return func_0027D2C8(func_0027D2E8(), *(int*)(*(char**)((char*)self + 0x14) + 0x4), val);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027D7F0);
#ifdef SKIP_ASM
class cSsxScriptVirt {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    virtual void v01();
    virtual void v02();
    virtual void v03();
};

extern "C" void func_0027D7F0(void* self)
{
    (*(cSsxScriptVirt**)((char*)self + 0x10))->v02();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_0027D820);
#ifdef SKIP_ASM
extern "C" void func_0027D820(void* self)
{
    (*(cSsxScriptVirt**)((char*)self + 0x10))->v03();
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D850);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D8D8);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027D970);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027F968);

INCLUDE_ASM("scripter/ssxscriptengine", func_0027F9F8);

INCLUDE_ASM("scripter/ssxscriptengine", func_002803F0);

INCLUDE_ASM("scripter/ssxscriptengine", func_00280458);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00280520__FPviT0T0);
#ifdef SKIP_ASM
void func_00280520(void* self, int a1, void* a2, void* a3)
{
    *(int*)a3 = *(int*)((char*)*(void**)((char*)self + 0x1c) + 0x4);
    *(int*)a2 = (*(int**)((char*)*(void**)((char*)self + 0x1c) + 0x10))[a1];
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00280548__FPviT0T0);
#ifdef SKIP_ASM
void func_00280548(void* self, int a1, void* a2, void* a3)
{
    *(int*)a3 = *(int*)((char*)*(void**)((char*)self + 0x20) + 0x4);
    *(int*)a2 = a1;
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_00280560);

INCLUDE_ASM("scripter/ssxscriptengine", func_002805B8);

INCLUDE_ASM("scripter/ssxscriptengine", func_00280640);

INCLUDE_ASM("scripter/ssxscriptengine", func_00280730);

INCLUDE_ASM("scripter/ssxscriptengine", func_002807B0);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_002808D8);
#ifdef SKIP_ASM
extern "C" int func_002808D8(void* self, void* a1, int a2)
{
    return a2 == 7;
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_002808E8);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00281000__FPviT0T0);
#ifdef SKIP_ASM
void func_00281000(void* self, int a1, void* a2, void* a3)
{
    *(int*)a3 = *(int*)((char*)*(void**)((char*)self + 0xf8) + 0x4);
    *(int*)a2 = a1;
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_00281018);

INCLUDE_ASM("scripter/ssxscriptengine", func_00281100);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00281160__FPvi);
#ifdef SKIP_ASM
void func_00281160(void* self, int val)
{
    *(int*)((char*)self + 0xF8) = val;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00281168__FPv);
#ifdef SKIP_ASM
int func_00281168(void* self)
{
    return 0xA;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00281170);
#ifdef SKIP_ASM
extern "C" void* func_00281170(void* self, int a1)
{
    if (a1 == 0) {
        return (char*)self + 0x10;
    }
    return (char*)self + (a1 * 0x14 + 0x8);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", func_00281190);
#ifdef SKIP_ASM
void func_0027D680(void* self, int val);

extern "C" int func_00281190(void* self, void* obj)
{
    void** slots = (void**)((char*)self + 0xD0);
    for (int i = 0; i < 2; i++) {
        if (slots[i] == 0) {
            slots[i] = obj;
            func_0027D680(obj, (int)self); // PORT: pointer passed as int (callee mangled __FPvi)
            return 1;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_002811E8);

INCLUDE_ASM("scripter/ssxscriptengine", func_002812E0);

INCLUDE_ASM("scripter/ssxscriptengine", func_00281370);

INCLUDE_ASM("scripter/ssxscriptengine", func_00281400);

INCLUDE_ASM("scripter/ssxscriptengine", func_002814F8);

INCLUDE_ASM("scripter/ssxscriptengine", func_002816A0);

INCLUDE_ASM("scripter/ssxscriptengine", func_00281A00);

INCLUDE_ASM("scripter/ssxscriptengine", func_00281A50);

INCLUDE_ASM("scripter/ssxscriptengine", func_00281AC8);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", func_00281C40);
#ifdef SKIP_ASM
extern "C" int func_00281C40(void* self, int i)
{
    int j = *(int*)((char*)self + (i << 2) + 0xdc);
    if (j < 0) {
        return 0;
    }
    return *(int*)((char*)self + (j << 2) + 0xd0);
}
#endif

INCLUDE_ASM("scripter/ssxscriptengine", func_00281C68);

INCLUDE_ASM("scripter/ssxscriptengine", func_00281E80);

INCLUDE_ASM("scripter/ssxscriptengine", func_00281ED8);

