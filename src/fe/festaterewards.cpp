#include "common.h"

INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_onCreateScreen);

INCLUDE_ASM("fe/festaterewards", func_001CF168);

//100%
INCLUDE_ASM("fe/festaterewards", func_001CF238);
#ifdef SKIP_ASM
extern "C" void func_001D0238(void* self);
extern "C" void func_001CFF10(void* self);
extern "C" void* func_0039E510(void* self);

extern "C" void func_001CF238(void* self)
{
    func_001D0238(self);
    func_001CFF10(self);
    func_0039E510(self);
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001CF270);

INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_onWidgetCreate);

INCLUDE_ASM("fe/festaterewards", func_001CFD18);

INCLUDE_ASM("fe/festaterewards", func_001CFDD0);

INCLUDE_ASM("fe/festaterewards", func_001CFE60);

INCLUDE_ASM("fe/festaterewards", func_001CFF10);

INCLUDE_ASM("fe/festaterewards", func_001CFFF8);

INCLUDE_ASM("fe/festaterewards", func_001D00C0);

INCLUDE_ASM("fe/festaterewards", func_001D0180);

INCLUDE_ASM("fe/festaterewards", func_001D0238);

INCLUDE_ASM("fe/festaterewards", func_001D0320);

INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_showReward);

INCLUDE_ASM("fe/festaterewards", func_001D0510);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D05F0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00157468(void* iface, int a, int b, int c, int d);

extern "C" void func_001D05F0(void* self, int arg)
{
    func_00157468(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(signed char*)((char*)self + 0x44),
                  *(int*)((char*)self + 0x50), *(int*)((char*)self + 0x4C), arg);
}
#endif

INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateHelpText);

INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updateRow);

INCLUDE_ASM("fe/festaterewards", func_001D0B00);

INCLUDE_ASM("fe/festaterewards", cFEStateRewardGalleryBase_updatePageNumber);

INCLUDE_ASM("fe/festaterewards", func_001D0D30);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D0E78);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" const char* func_00198AF0(void* a0);

extern "C" void func_001D0E78(void* self)
{
    if (*(cUIText**)((char*)self + 0x910) != 0) {
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x910), func_00198AF0(*(void**)((char*)self + 0x54)));
    }
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D0EB8);

INCLUDE_ASM("fe/festaterewards", func_001D0F30);

INCLUDE_ASM("fe/festaterewards", func_001D10A8);

INCLUDE_ASM("fe/festaterewards", func_001D1128);

extern void* D_00467070[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1198__FPv);
#ifdef SKIP_ASM
void* func_001D1198(void* self)
{
    return (void*)D_00467070;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D11A8);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D11A8(void* self, int i)
{
    char buf[5];
    void* e = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)e + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D11F8);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1290);
#ifdef SKIP_ASM
extern "C" void func_001D1290(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 42; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D12C8);

extern void* D_00467098[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1330__FPv);
#ifdef SKIP_ASM
void* func_001D1330(void* self)
{
    return (void*)D_00467098;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D1340);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D1340(void* self, int i)
{
    char buf[5];
    void* e = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)e + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D1390);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1428);
#ifdef SKIP_ASM
extern "C" void func_001D1428(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 115; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D1460);

extern void* D_004670C8[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D14C8__FPv);
#ifdef SKIP_ASM
void* func_001D14C8(void* self)
{
    return (void*)D_004670C8;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D14D8);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D14D8(void* self, int i)
{
    char buf[5];
    void* e = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)e + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D1528);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D15C0);
#ifdef SKIP_ASM
extern "C" void func_001D15C0(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 99; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D15F8);

extern void* D_004670F0[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1660__FPv);
#ifdef SKIP_ASM
void* func_001D1660(void* self)
{
    return (void*)D_004670F0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D1670);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D1670(void* self, int i)
{
    char buf[5];
    void* e = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)e + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D16C0);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1758);
#ifdef SKIP_ASM
extern "C" void func_001D1758(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 1; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D1790);

extern void* D_00467118[];

//100%
INCLUDE_ASM("fe/festaterewards", func_001D17F8__FPv);
#ifdef SKIP_ASM
void* func_001D17F8(void* self)
{
    return (void*)D_00467118;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festaterewards", func_001D1808);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001CFDD0(void* self, char* name);

extern "C" void func_001D1808(void* self, int i)
{
    char buf[5];
    void* e = *(void**)((char*)self + (i << 2) + 0xA28);
    strncpy(buf, *(const char**)((char*)e + 8), 4);
    buf[4] = 0;
    func_001CFDD0(self, buf);
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D1858);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D18F0);
#ifdef SKIP_ASM
extern "C" void func_001D18F0(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 27; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D1928);

INCLUDE_ASM("fe/festaterewards", func_001D1990);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1A28);
#ifdef SKIP_ASM
extern "C" void func_001D1A28(void* self)
{
    int i;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0xa24) = 0;
    for (i = 27; i >= 0; i--) {
        ((int*)((char*)self + 0xa28))[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1A60);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* func_00398380(void* self, int id);

extern "C" void func_001D1A60(void* self, int i)
{
    char* obj = *(char**)((char*)self + 0x10);
    void* e = *(void**)((char*)self + (i << 2) + 0xA28);
    int h = GetHashValue32(*(char**)((char*)e + 4));
    func_00398380(obj + 0x58, h);
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D1AA0);

INCLUDE_ASM("fe/festaterewards", func_001D1B68);

INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onCreateScreen);

INCLUDE_ASM("fe/festaterewards", cFEStatePreviewReward_onWidgetCreate);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1F78);
#ifdef SKIP_ASM
extern "C" int func_001D1F78(void* self, int a1, unsigned int a2)
{
    switch (a2) {
    case 6:
    case 8:
    case 9:
        return 0x100;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D1FB0);
#ifdef SKIP_ASM
extern "C" void func_001D2198(void* self);
extern "C" void* func_0039E510(void* self);

extern "C" void func_001D1FB0(void* self)
{
    if (*(int*)((char*)self + 0x6C) >= 0) {
        func_001D2198(self);
    }
    func_0039E510(self);
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D1FF0);

INCLUDE_ASM("fe/festaterewards", func_001D20D8);

INCLUDE_ASM("fe/festaterewards", func_001D2150);

INCLUDE_ASM("fe/festaterewards", func_001D2198);

INCLUDE_ASM("fe/festaterewards", func_001D22F0);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2380);
#ifdef SKIP_ASM
extern "C" void func_00253418(void* p, int a1);
extern "C" void func_0039E390(void* self, int flags);
extern void* D_00469928[];
extern void* D_0046D0D0[];

extern "C" void func_001D2380(void* self, int flags)
{
    *(void***)((char*)self + 8) = D_00469928;
    if (*(void**)((char*)self + 0x248) != 0) {
        func_00253418(*(void**)((char*)self + 0x248), 3);
    }
    *(void***)((char*)self + 8) = D_0046D0D0;
    func_0039E390(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D23E0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002533C8(void* mem);
extern "C" void func_001D25E8(void* self);
extern char D_00461320[];

extern "C" void func_001D23E0(void* self)
{
    *(void**)((char*)self + 0x248) = func_002533C8(cMemMan_alloc(0x44, D_00461320, 0, 0));
    func_001D25E8(self);
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2430);
#ifdef SKIP_ASM
extern "C" void func_00253418(void* p, int a1);

extern "C" void func_001D2430(void* self)
{
    void* p = *(void**)((char*)self + 0x248);
    if (p != 0) {
        func_00253418(p, 3);
    }
    *(void**)((char*)self + 0x248) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2468);
#ifdef SKIP_ASM
extern "C" void func_00253890(void* p, int a1);
extern "C" void* func_0039E6B8(void* self);

extern "C" void func_001D2468(void* self)
{
    void* p = *(void**)((char*)self + 0x248);
    if (p != 0) {
        *(int*)((char*)self + 0x278) = 1;
        func_00253890(p, 0);
    }
    func_0039E6B8(self);
}
#endif

INCLUDE_ASM("fe/festaterewards", func_001D24A8);

INCLUDE_ASM("fe/festaterewards", func_001D2518);

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2598);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);

extern "C" void func_001D2598(void* self, const char* a, const char* b)
{
    strcpy((char*)self + 0x48, a);
    if (b != 0) {
        strcpy((char*)self + 0x148, b);
    } else {
        *((char*)self + 0x148) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D25E8);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_002B3A70(void* p);
extern "C" int func_002534A8(void* p, void* q);
extern "C" void func_00253860(void* p);

extern "C" void func_001D25E8(void* self)
{
    func_002B3A70((char*)func_0028B180() + 0x118);
    *(int*)((char*)self + 0x254) = 1;
    func_002534A8(*(void**)((char*)self + 0x248), (char*)self + 0x24C);
    func_00253860(*(void**)((char*)self + 0x248));
    *(int*)((char*)self + 0x278) = 0;
    *(int*)((char*)self + 0x274) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festaterewards", func_001D2638);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_002B3A98(void* self);
extern "C" void func_0039F190(void* self, int a1);

extern "C" void func_001D2638(void* self)
{
    func_002B3A98((char*)func_0028B180() + 0x118);
    *(int*)((char*)self + 0x27C) = 0;
    func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
}
#endif

