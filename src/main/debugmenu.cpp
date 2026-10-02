#include "common.h"

INCLUDE_ASM("main/debugmenu", cRenderTogglesMenu_cRenderTogglesMenu);

INCLUDE_ASM("main/debugmenu", cVisualEffectsTestMenu_cVisualEffectsTestMenu);

INCLUDE_ASM("main/debugmenu", cPlantsMenu_cPlantsMenu);

INCLUDE_ASM("main/debugmenu", cLightGlowMenu_cLightGlowMenu);

INCLUDE_ASM("main/debugmenu", cDepthFogMenu_cDepthFogMenu);

//100%
INCLUDE_ASM("main/debugmenu", cHeightFogMenu_cHeightFogMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* func_002CAA58(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
extern "C" void* cFloatMenuItem_cFloatMenuItem(void* self, int a1, void* var, void* text, float lo, float hi);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0047FB28[];
extern char D_0047E4C0[];
extern char D_004A2D30[];
extern char D_0047E4D0[];
extern char D_0047E4E0[];
extern int D_004A43B4;
extern float D_004A43B8;
extern float D_004A43BC;

extern "C" void* cHeightFogMenu_cHeightFogMenu(void* self)
{
    func_002CAA58(self);
    *(void***)((char*)self + 0x12C) = D_0047FB28;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), D_0047E4C0);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)0x28);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A43B4, 0, 1, D_004A2D30);
    cFloatMenuItem_cFloatMenuItem((char*)self + 0x194, 1, &D_004A43B8, D_0047E4D0, -100000.0f, 100000.0f);
    cFloatMenuItem_cFloatMenuItem((char*)self + 0x1C8, 2, &D_004A43BC, D_0047E4E0, 9.999999747378752e-05f, 10000.0f);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C8, -1);
    return self;
}
#endif

INCLUDE_ASM("main/debugmenu", cDetailSystemMenu_cDetailSystemMenu);

INCLUDE_ASM("main/debugmenu", cShellRendererMenu_cShellRendererMenu);

INCLUDE_ASM("main/debugmenu", cScreenTintMenu_cScreenTintMenu);

INCLUDE_ASM("main/debugmenu", cShadowMenu_cShadowMenu);

INCLUDE_ASM("main/debugmenu", cSnowSurfaceMenu_cSnowSurfaceMenu);

INCLUDE_ASM("main/debugmenu", cBoardTrailMenu_cBoardTrailMenu);

//100%
INCLUDE_ASM("main/debugmenu", cFlagsMenu_cFlagsMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* func_002CAA58(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0047F8C0[];
extern char D_0047EBB8[];
extern char D_0047EBC8[];
extern char D_0047EBD8[];
extern int D_004A4240;
extern int D_004A4244;

extern "C" void* cFlagsMenu_cFlagsMenu(void* self)
{
    func_002CAA58(self);
    *(void***)((char*)self + 0x12C) = D_0047F8C0;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), D_0047EBB8);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)0x28);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A4240, 0, 1, D_0047EBC8);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &D_004A4244, 0, 1, D_0047EBD8);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    return self;
}
#endif

INCLUDE_ASM("main/debugmenu", cPowerUpFXTogglesMenu_cPowerUpFXTogglesMenu);

INCLUDE_ASM("main/debugmenu", cSnowfallTogglesMenu_cSnowfallTogglesMenu);

INCLUDE_ASM("main/debugmenu", cSplashTogglesMenu_cSplashTogglesMenu);

INCLUDE_ASM("main/debugmenu", cTubeMenu_cTubeMenu);

INCLUDE_ASM("main/debugmenu", cPathArrowMenu_cPathArrowMenu);

INCLUDE_ASM("main/debugmenu", cVisualEffectsMainMenu_cVisualEffectsMainMenu);

extern "C" void* func_002CBF60(void* self);

//100%
INCLUDE_ASM("main/debugmenu", func_0024D690__FPv);
#ifdef SKIP_ASM
void* func_0024D690(void* self)
{
    return func_002CBF60(self);
}
#endif

extern "C" void* func_002CBFD0(void* self);

//100%
INCLUDE_ASM("main/debugmenu", func_0024D6B0__FPv);
#ifdef SKIP_ASM
void* func_0024D6B0(void* self)
{
    return func_002CBFD0(self);
}
#endif

INCLUDE_ASM("main/debugmenu", func_0024D710);

//100%
INCLUDE_ASM("main/debugmenu", func_0024D998);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

extern "C" void func_0024D998(void* self, int flags)
{
    func_002CA280((char*)self + 0x464, 2);
    func_002CA280((char*)self + 0x434, 2);
    func_002CA280((char*)self + 0x404, 2);
    func_002CA280((char*)self + 0x3D4, 2);
    func_002CA280((char*)self + 0x3A4, 2);
    func_002CA280((char*)self + 0x374, 2);
    func_002CA280((char*)self + 0x344, 2);
    func_002CA280((char*)self + 0x314, 2);
    func_002CA280((char*)self + 0x2E4, 2);
    func_002CA280((char*)self + 0x2B4, 2);
    func_002CA280((char*)self + 0x284, 2);
    func_002CA280((char*)self + 0x254, 2);
    func_002CA280((char*)self + 0x224, 2);
    func_002CA280((char*)self + 0x1F4, 2);
    func_002CA280((char*)self + 0x1C4, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

INCLUDE_ASM("main/debugmenu", func_0024DAB8);

//100%
INCLUDE_ASM("main/debugmenu", func_0024DC68);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

static inline void dtorSubMenuItem_24DC68(void* p)
{
    func_002CAA80((char*)p + 0x18, 2);
    func_002CA280(p, 2);
}

extern "C" void func_0024DC68(void* self, int flags)
{
    func_002CA280((char*)self + 0x904, 2);
    func_002CA280((char*)self + 0x8E0, 2);
    func_002CA280((char*)self + 0x8BC, 2);
    func_002CA280((char*)self + 0x898, 2);
    func_002CA280((char*)self + 0x874, 2);
    dtorSubMenuItem_24DC68((char*)self + 0x2C4);
    func_002CA280((char*)self + 0x290, 2);
    func_002CA280((char*)self + 0x25C, 2);
    func_002CA280((char*)self + 0x22C, 2);
    func_002CA280((char*)self + 0x1F8, 2);
    func_002CA280((char*)self + 0x1C4, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

INCLUDE_ASM("main/debugmenu", func_0024DD70);

//100%
INCLUDE_ASM("main/debugmenu", func_0024DFA0);
#ifdef SKIP_ASM
extern "C" void func_0024DFA0(void* self, int flags)
{
    func_002CA280((char*)self + 0x59C, 2);
    func_002CA280((char*)self + 0x56C, 2);
    func_002CA280((char*)self + 0x53C, 2);
    func_002CA280((char*)self + 0x508, 2);
    func_002CA280((char*)self + 0x4D4, 2);
    func_002CA280((char*)self + 0x4A0, 2);
    func_002CA280((char*)self + 0x46C, 2);
    func_002CA280((char*)self + 0x438, 2);
    func_002CA280((char*)self + 0x404, 2);
    func_002CA280((char*)self + 0x3D0, 2);
    func_002CA280((char*)self + 0x39C, 2);
    func_002CA280((char*)self + 0x368, 2);
    func_002CA280((char*)self + 0x334, 2);
    func_002CA280((char*)self + 0x300, 2);
    func_002CA280((char*)self + 0x2CC, 2);
    func_002CA280((char*)self + 0x298, 2);
    func_002CA280((char*)self + 0x264, 2);
    func_002CA280((char*)self + 0x230, 2);
    func_002CA280((char*)self + 0x1FC, 2);
    func_002CA280((char*)self + 0x1C8, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024E0F8);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

extern "C" void func_0024E0F8(void* self, int flags)
{
    func_002CA280((char*)self + 0x418, 2);
    func_002CA280((char*)self + 0x3E8, 2);
    func_002CA280((char*)self + 0x3B8, 2);
    func_002CA280((char*)self + 0x388, 2);
    func_002CA280((char*)self + 0x358, 2);
    func_002CA280((char*)self + 0x328, 2);
    func_002CA280((char*)self + 0x2F8, 2);
    func_002CA280((char*)self + 0x2C8, 2);
    func_002CA280((char*)self + 0x294, 2);
    func_002CA280((char*)self + 0x260, 2);
    func_002CA280((char*)self + 0x22C, 2);
    func_002CA280((char*)self + 0x1F8, 2);
    func_002CA280((char*)self + 0x1C4, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024E200);
#ifdef SKIP_ASM
static inline void dtorSubMenuItem_24E200(void* p)
{
    func_002CAA80((char*)p + 0x18, 2);
    func_002CA280(p, 2);
}

extern "C" void func_0024E200(void* self, int flags)
{
    func_002CA280((char*)self + 0xFEC, 2);
    func_002CA280((char*)self + 0xFD0, 2);
    func_002CA280((char*)self + 0xFAC, 2);
    func_002CA280((char*)self + 0xF88, 2);
    func_002CA280((char*)self + 0xF64, 2);
    func_002CA280((char*)self + 0xF40, 2);
    dtorSubMenuItem_24E200((char*)self + 0x990);
    func_002CA280((char*)self + 0x974, 2);
    func_002CA280((char*)self + 0x950, 2);
    func_002CA280((char*)self + 0x92C, 2);
    func_002CA280((char*)self + 0x908, 2);
    func_002CA280((char*)self + 0x8E4, 2);
    dtorSubMenuItem_24E200((char*)self + 0x334);
    func_002CA280((char*)self + 0x300, 2);
    func_002CA280((char*)self + 0x2CC, 2);
    func_002CA280((char*)self + 0x298, 2);
    func_002CA280((char*)self + 0x264, 2);
    func_002CA280((char*)self + 0x230, 2);
    func_002CA280((char*)self + 0x1FC, 2);
    func_002CA280((char*)self + 0x1C8, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

INCLUDE_ASM("main/debugmenu", func_0024E388);

//100%
INCLUDE_ASM("main/debugmenu", func_0024E7C8);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

extern "C" void func_0024E7C8(void* self, int flags)
{
    func_002CA280((char*)self + 0x458, 2);
    func_002CA280((char*)self + 0x428, 2);
    func_002CA280((char*)self + 0x3F4, 2);
    func_002CA280((char*)self + 0x3C0, 2);
    func_002CA280((char*)self + 0x38C, 2);
    func_002CA280((char*)self + 0x35C, 2);
    func_002CA280((char*)self + 0x32C, 2);
    func_002CA280((char*)self + 0x2FC, 2);
    func_002CA280((char*)self + 0x2CC, 2);
    func_002CA280((char*)self + 0x298, 2);
    func_002CA280((char*)self + 0x264, 2);
    func_002CA280((char*)self + 0x230, 2);
    func_002CA280((char*)self + 0x1FC, 2);
    func_002CA280((char*)self + 0x1C8, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

INCLUDE_ASM("main/debugmenu", func_0024E8D8);

//100%
INCLUDE_ASM("main/debugmenu", func_0024F6C0);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

extern "C" void func_0024F6C0(void* self, int flags)
{
    func_002CA280((char*)self + 0x324, 2);
    func_002CA280((char*)self + 0x308, 2);
    func_002CA280((char*)self + 0x2EC, 2);
    func_002CA280((char*)self + 0x2D0, 2);
    func_002CA280((char*)self + 0x2B4, 2);
    func_002CA280((char*)self + 0x298, 2);
    func_002CA280((char*)self + 0x27C, 2);
    func_002CA280((char*)self + 0x260, 2);
    func_002CA280((char*)self + 0x244, 2);
    func_002CA280((char*)self + 0x228, 2);
    func_002CA280((char*)self + 0x20C, 2);
    func_002CA280((char*)self + 0x1F0, 2);
    func_002CA280((char*)self + 0x1D4, 2);
    func_002CA280((char*)self + 0x1B8, 2);
    func_002CA280((char*)self + 0x19C, 2);
    func_002CA280((char*)self + 0x180, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024F7E0__FPv);
#ifdef SKIP_ASM
void* func_0024F7E0(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024F800__FPv);
#ifdef SKIP_ASM
void* func_0024F800(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024F820__FPv);
#ifdef SKIP_ASM
void* func_0024F820(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024F840__FPv);
#ifdef SKIP_ASM
void* func_0024F840(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024F860);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

extern "C" void func_0024F860(void* self, int flags)
{
    func_002CA280((char*)self + 0x398, 2);
    func_002CA280((char*)self + 0x364, 2);
    func_002CA280((char*)self + 0x330, 2);
    func_002CA280((char*)self + 0x2FC, 2);
    func_002CA280((char*)self + 0x2C8, 2);
    func_002CA280((char*)self + 0x298, 2);
    func_002CA280((char*)self + 0x264, 2);
    func_002CA280((char*)self + 0x230, 2);
    func_002CA280((char*)self + 0x1FC, 2);
    func_002CA280((char*)self + 0x1C8, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024F940__FPv);
#ifdef SKIP_ASM
void* func_0024F940(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024F960__FPv);
#ifdef SKIP_ASM
void* func_0024F960(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024F980__FPv);
#ifdef SKIP_ASM
void* func_0024F980(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024F9A0__FPv);
#ifdef SKIP_ASM
void* func_0024F9A0(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024F9C0__FPv);
#ifdef SKIP_ASM
void* func_0024F9C0(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024F9E0__FPv);
#ifdef SKIP_ASM
void* func_0024F9E0(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FA00);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

extern "C" void func_0024FA00(void* self, int flags)
{
    func_002CA280((char*)self + 0x1C8, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FA78__FPv);
#ifdef SKIP_ASM
void* func_0024FA78(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FA98__FPv);
#ifdef SKIP_ASM
void* func_0024FA98(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FAB8);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

extern "C" void func_0024FAB8(void* self, int flags)
{
    func_002CA280((char*)self + 0x334, 2);
    func_002CA280((char*)self + 0x300, 2);
    func_002CA280((char*)self + 0x2CC, 2);
    func_002CA280((char*)self + 0x298, 2);
    func_002CA280((char*)self + 0x264, 2);
    func_002CA280((char*)self + 0x230, 2);
    func_002CA280((char*)self + 0x1FC, 2);
    func_002CA280((char*)self + 0x1C8, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FB80__FPv);
#ifdef SKIP_ASM
void* func_0024FB80(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FBA0__FPv);
#ifdef SKIP_ASM
void* func_0024FBA0(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FBC0);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

extern "C" void func_0024FBC0(void* self, int flags)
{
    func_002CA280((char*)self + 0x2EC, 2);
    func_002CA280((char*)self + 0x2B8, 2);
    func_002CA280((char*)self + 0x284, 2);
    func_002CA280((char*)self + 0x254, 2);
    func_002CA280((char*)self + 0x224, 2);
    func_002CA280((char*)self + 0x1F4, 2);
    func_002CA280((char*)self + 0x1C4, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FC80__FPv);
#ifdef SKIP_ASM
void* func_0024FC80(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FCA0__FPv);
#ifdef SKIP_ASM
void* func_0024FCA0(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FCC0__FPv);
#ifdef SKIP_ASM
void* func_0024FCC0(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FCE0__FPv);
#ifdef SKIP_ASM
void* func_0024FCE0(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FD00);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

extern "C" void func_0024FD00(void* self, int flags)
{
    func_002CA280((char*)self + 0x2FC, 2);
    func_002CA280((char*)self + 0x2C8, 2);
    func_002CA280((char*)self + 0x294, 2);
    func_002CA280((char*)self + 0x260, 2);
    func_002CA280((char*)self + 0x22C, 2);
    func_002CA280((char*)self + 0x1F8, 2);
    func_002CA280((char*)self + 0x1C4, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FDC0__FPv);
#ifdef SKIP_ASM
void* func_0024FDC0(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FDE0__FPv);
#ifdef SKIP_ASM
void* func_0024FDE0(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FE00);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

extern "C" void func_0024FE00(void* self, int flags)
{
    func_002CA280((char*)self + 0x294, 2);
    func_002CA280((char*)self + 0x260, 2);
    func_002CA280((char*)self + 0x22C, 2);
    func_002CA280((char*)self + 0x1F8, 2);
    func_002CA280((char*)self + 0x1C4, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FEA8__FPv);
#ifdef SKIP_ASM
void* func_0024FEA8(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FEC8__FPv);
#ifdef SKIP_ASM
void* func_0024FEC8(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FEE8__FPv);
#ifdef SKIP_ASM
void* func_0024FEE8(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FF08__FPv);
#ifdef SKIP_ASM
void* func_0024FF08(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FF28);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

extern "C" void func_0024FF28(void* self, int flags)
{
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FF90__FPv);
#ifdef SKIP_ASM
void* func_0024FF90(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FFB0__FPv);
#ifdef SKIP_ASM
void* func_0024FFB0(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FFD0__FPv);
#ifdef SKIP_ASM
void* func_0024FFD0(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024FFF0__FPv);
#ifdef SKIP_ASM
void* func_0024FFF0(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00250010__FPv);
#ifdef SKIP_ASM
void* func_00250010(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00250030__FPv);
#ifdef SKIP_ASM
void* func_00250030(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00250050__FPv);
#ifdef SKIP_ASM
void* func_00250050(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00250070__FPv);
#ifdef SKIP_ASM
void* func_00250070(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00250090);
#ifdef SKIP_ASM
extern "C" void func_00250090(void* self, int flags)
{
    func_002CA280((char*)self + 0x25C, 2);
    func_002CA280((char*)self + 0x22C, 2);
    func_002CA280((char*)self + 0x1FC, 2);
    func_002CA280((char*)self + 0x1C8, 2);
    func_002CA280((char*)self + 0x194, 2);
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00250128__FPv);
#ifdef SKIP_ASM
void* func_00250128(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00250148__FPv);
#ifdef SKIP_ASM
void* func_00250148(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00250168__FPv);
#ifdef SKIP_ASM
void* func_00250168(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00250188__FPv);
#ifdef SKIP_ASM
void* func_00250188(void* self)
{
    return func_002CBFD0(self);
}
#endif

extern "C" void* func_0024E8D8(int, int);

//99.38%
INCLUDE_ASM("main/debugmenu", func_002501A8__FPv);
#ifdef SKIP_ASM
void* func_002501A8(void* self)
{
    return func_0024E8D8(1, 0xffff);
}
#endif

extern "C" void* func_0024E8D8(int, int);

//99.38%
INCLUDE_ASM("main/debugmenu", func_002501C8__FPv);
#ifdef SKIP_ASM
void* func_002501C8(void* self)
{
    return func_0024E8D8(0, 0xffff);
}
#endif

INCLUDE_ASM("main/debugmenu", cAvalancheMenu_cAvalancheMenu);

//100%
INCLUDE_ASM("main/debugmenu", cAIVisualEffectsMainMenu_cAIVisualEffectsMainMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* func_002CAA58(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0047FFE8[];
extern char D_0047FFB0[];
extern char D_004D1C88[];
extern char D_0047FFD0[];

extern "C" void* cAIVisualEffectsMainMenu_cAIVisualEffectsMainMenu(void* self)
{
    func_002CAA58(self);
    *(void***)((char*)self + 0x12C) = D_0047FFE8;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), D_0047FFB0);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)0x28);
    cSubMenuItem_cSubMenuItem((char*)self + 0x164, D_004D1C88, D_0047FFD0);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00250A60__FPv);
#ifdef SKIP_ASM
void* func_00250A60(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00250A80__FPv);
#ifdef SKIP_ASM
void* func_00250A80(void* self)
{
    return func_002CBFD0(self);
}
#endif

INCLUDE_ASM("main/debugmenu", func_00250AE0);

INCLUDE_ASM("main/debugmenu", func_00250CB0);

//100%
INCLUDE_ASM("main/debugmenu", func_002515F0__FPv);
#ifdef SKIP_ASM
void* func_002515F0(void* self)
{
    return func_002CBF60(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00251610__FPv);
#ifdef SKIP_ASM
void* func_00251610(void* self)
{
    return func_002CBFD0(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00251630);
#ifdef SKIP_ASM
extern "C" void func_002CA280(void* p, int flags);
extern "C" void func_002CAA80(void* self, int flags);

extern "C" void func_00251630(void* self, int flags)
{
    func_002CA280((char*)self + 0x164, 2);
    func_002CA280((char*)self + 0x14C, 2);
    func_002CA280((char*)self + 0x130, 2);
    func_002CAA80(self, flags);
}
#endif

extern "C" void* func_00250CB0(int, int);

//99.38%
INCLUDE_ASM("main/debugmenu", func_00251690__FPv);
#ifdef SKIP_ASM
void* func_00251690(void* self)
{
    return func_00250CB0(1, 0xffff);
}
#endif

extern "C" void* func_00250CB0(int, int);

//99.38%
INCLUDE_ASM("main/debugmenu", func_002516B0__FPv);
#ifdef SKIP_ASM
void* func_002516B0(void* self)
{
    return func_00250CB0(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", MEM_initblock);
#ifdef SKIP_ASM
extern "C" void* MEM_initblock(void* self, int p1, void* p2, int p3, short p4, int p5, int p6)
{
    *(short*)self = 0x424d;
    *(short*)((char*)self + 0x2) = p4;
    *(int*)((char*)self + 0x4) = (int)p2;
    *(int*)((char*)self + 0x8) = p6;
    *(int*)((char*)self + 0xc) = p5;
    return (char*)p2 + 0x10;
}
#endif

