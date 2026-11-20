#include "common.h"

//100%
INCLUDE_ASM("main/debugmenu", cRenderTogglesMenu_cRenderTogglesMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cRenderTogglesMenu_vtbl[];
extern char str_Render_Toggles_Menu[];
extern char str_AI_Frames_per_Render_Frame[];
extern char str_Display_Render_Stats[];
extern char str_Reset_Stats[];
extern char str_Use_Small_Texture_0[];
extern char str_Use_Small_Texture_1[];
extern char str_Disable_Patches[];
extern char str_Disable_Patch_Reflection[];
extern char str_Disable_Patch_Shadows[];
extern char str_Disable_Sparkles[];
extern char str_Disable_Line_Strips[];
extern char str_Disable_Tri_Strips[];
extern char str_Disable_Transform_Strips[];
extern char str_Disable_Meshes[];
extern char str_Disable_Mesh_Reflection[];
extern char str_Disable_Static_Emitters[];
extern char str_Disable_Dynamic_Emitters[];
extern char str_Disable_Dynamic_Colour_Emitters[];
extern char str_Disable_Sprites[];
extern char str_Disable_3D_Sprites[];
extern char str_Disable_Texture_Rects[];
extern char str_Disable_Player_Strips[];
extern char str_Disable_Shadows[];
extern char str_Disable_Board_Trail[];
extern char str_Disable_AI_Board_Trail[];
extern char str_Disable_Visibility_Rects[];
extern char str_Disable_Snowfall[];
extern char str_Disable_Board_Wake[];
extern char str_Disable_Sky_Box[];
extern char str_Disable_Character_Specular[];
extern char str_Disable_AI_Visual_FX[];
extern char str_Disable_Bounding_Boxes[];
extern char str_Use_Reflection_Vector_for_Sphere_Map[];
extern char str_Disable_HUD_elements[];
extern char str_Test_Tunnel[];
extern char str_Particle_Size_Scale[];
extern char str_Test_Surface_Types[];
extern char str_Software_Based_Dynamic_Systems[];
extern char str_Temp_Int[];
extern char str_Enable_Build_Info[];
extern char str_Enable_NIS_Mem_Usage_Info[];
extern char str_Enable_Gameflow_Info[];
extern char str_Profile_Individual_Effects[];
extern char str_Show_Surface_Types[];
extern char str_PROG2_x[];
extern char str_PROG2_y[];
extern char str_PROG2_z[];
extern char str_PROG2_w[];
extern int D_004A44C0;
extern int D_004A44C4;
extern int D_004A44C8;
extern int D_004A44CC;
extern int D_004A44D0;
extern int D_004A44D4;
extern int D_004A44D8;
extern int D_004A44DC;
extern int D_004A44E0;
extern int D_004A44E4;
extern int D_004A44E8;
extern int D_004A44EC;
extern int D_004A44F0;
extern int D_004A44F4;
extern int D_004A44F8;
extern int D_004A44FC;
extern int D_004A4500;
extern int D_004A4504;
extern int D_004A4508;
extern int D_004A450C;
extern int D_004A4510;
extern int D_004A4514;
extern int D_004A4518;
extern int D_004A451C;
extern int D_004A4520;
extern int cRenderToggles_m_DisableSnowFall;
extern int cRenderToggles_m_DisableBoardWake;
extern int cRenderToggles_m_DisableSkyBox;
extern int cRenderToggles_m_DisableCharacterSpecular;
extern int cRenderToggles_m_DisableAIVisualFX;
extern int cRenderToggles_m_DisableBoundingBoxes;
extern int cRenderToggles_m_UseReflectionVector;
extern int cRenderToggles_m_DisableHUD;
extern int D_004A4544;
extern float D_004A4548;
extern int D_004A454C;
extern int D_004A4550;
extern int D_004A4554;
extern float D_004A4558;
extern float D_004A455C;
extern float D_004A4560;
extern float D_004A4564;
extern int D_004A4568;
extern int D_004A456C;
extern int D_004A4570;
extern int D_004A4574;
extern int D_004A4578;

/**
 * Builds the "Render Toggles Menu" debug menu: a title, a spacer and 47 int and float items bound to render
 * debug globals: AI frames per render frame, render stats, one "Disable ..." toggle per render pass, test
 * values and the build, NIS-memory and game-flow info overlays.
 *
 * @return @p self
 */
extern "C" void* cRenderTogglesMenu_cRenderTogglesMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cRenderTogglesMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Render_Toggles_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A44C0, 0, 0x3c, str_AI_Frames_per_Render_Frame);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &D_004A44C4, 0, 1, str_Display_Render_Stats);
    cIntMenuItem_cIntMenuItem((char*)self + 0x1C4, 2, &D_004A44C8, 0, 1, str_Reset_Stats);
    cIntMenuItem_cIntMenuItem((char*)self + 0x1F4, 3, &D_004A44CC, 0, 1, str_Use_Small_Texture_0);
    cIntMenuItem_cIntMenuItem((char*)self + 0x224, 4, &D_004A44D0, 0, 1, str_Use_Small_Texture_1);
    cIntMenuItem_cIntMenuItem((char*)self + 0x254, 5, &D_004A44D4, 0, 1, str_Disable_Patches);
    cIntMenuItem_cIntMenuItem((char*)self + 0x284, 6, &D_004A44D8, 0, 1, str_Disable_Patch_Reflection);
    cIntMenuItem_cIntMenuItem((char*)self + 0x2B4, 7, &D_004A44DC, 0, 1, str_Disable_Patch_Shadows);
    cIntMenuItem_cIntMenuItem((char*)self + 0x2E4, 8, &D_004A44E0, 0, 1, str_Disable_Sparkles);
    cIntMenuItem_cIntMenuItem((char*)self + 0x314, 9, &D_004A44E4, 0, 1, str_Disable_Line_Strips);
    cIntMenuItem_cIntMenuItem((char*)self + 0x344, 10, &D_004A44E8, 0, 3, str_Disable_Tri_Strips);
    cIntMenuItem_cIntMenuItem((char*)self + 0x374, 11, &D_004A44EC, 0, 1, str_Disable_Transform_Strips);
    cIntMenuItem_cIntMenuItem((char*)self + 0x3A4, 0xc, &D_004A44F0, 0, 1, str_Disable_Meshes);
    cIntMenuItem_cIntMenuItem((char*)self + 0x3D4, 13, &D_004A44F4, 0, 1, str_Disable_Mesh_Reflection);
    cIntMenuItem_cIntMenuItem((char*)self + 0x404, 0xe, &D_004A44F8, 0, 1, str_Disable_Static_Emitters);
    cIntMenuItem_cIntMenuItem((char*)self + 0x434, 0xf, &D_004A44FC, 0, 1, str_Disable_Dynamic_Emitters);
    cIntMenuItem_cIntMenuItem((char*)self + 0x464, 0x10, &D_004A4500, 0, 1, str_Disable_Dynamic_Colour_Emitters);
    cIntMenuItem_cIntMenuItem((char*)self + 0x494, 17, &D_004A4504, 0, 1, str_Disable_Sprites);
    cIntMenuItem_cIntMenuItem((char*)self + 0x4C4, 18, &D_004A4508, 0, 1, str_Disable_3D_Sprites);
    cIntMenuItem_cIntMenuItem((char*)self + 0x4F4, 19, &D_004A450C, 0, 1, str_Disable_Texture_Rects);
    cIntMenuItem_cIntMenuItem((char*)self + 0x524, 20, &D_004A4510, 0, 1, str_Disable_Player_Strips);
    cIntMenuItem_cIntMenuItem((char*)self + 0x554, 21, &D_004A4514, 0, 1, str_Disable_Shadows);
    cIntMenuItem_cIntMenuItem((char*)self + 0x584, 22, &D_004A4518, 0, 1, str_Disable_Board_Trail);
    cIntMenuItem_cIntMenuItem((char*)self + 0x5B4, 23, &D_004A451C, 0, 1, str_Disable_AI_Board_Trail);
    cIntMenuItem_cIntMenuItem((char*)self + 0x5E4, 0x18, &D_004A4520, 0, 1, str_Disable_Visibility_Rects);
    cIntMenuItem_cIntMenuItem((char*)self + 0x614, 25, &cRenderToggles_m_DisableSnowFall, 0, 1, str_Disable_Snowfall);
    cIntMenuItem_cIntMenuItem((char*)self + 0x644, 26, &cRenderToggles_m_DisableBoardWake, 0, 1, str_Disable_Board_Wake);
    cIntMenuItem_cIntMenuItem((char*)self + 0x674, 27, &cRenderToggles_m_DisableSkyBox, 0, 1, str_Disable_Sky_Box);
    cIntMenuItem_cIntMenuItem((char*)self + 0x6A4, 0x1c, &cRenderToggles_m_DisableCharacterSpecular, 0, 1, str_Disable_Character_Specular);
    cIntMenuItem_cIntMenuItem((char*)self + 0x6D4, 29, &cRenderToggles_m_DisableAIVisualFX, 0, 1, str_Disable_AI_Visual_FX);
    cIntMenuItem_cIntMenuItem((char*)self + 0x704, 0x1e, &cRenderToggles_m_DisableBoundingBoxes, 0, 1, str_Disable_Bounding_Boxes);
    cIntMenuItem_cIntMenuItem((char*)self + 0x734, 0x1f, &cRenderToggles_m_UseReflectionVector, 0, 1, str_Use_Reflection_Vector_for_Sphere_Map);
    cIntMenuItem_cIntMenuItem((char*)self + 0x764, 0x20, &cRenderToggles_m_DisableHUD, 0, 1, str_Disable_HUD_elements);
    cIntMenuItem_cIntMenuItem((char*)self + 0x794, 33, &D_004A4544, 0, 1, str_Test_Tunnel);
    cFloatMenuItem_ctor((char*)self + 0x7C4, 34, &D_004A4548, 1.0f, 2000.0f, str_Particle_Size_Scale);
    cIntMenuItem_cIntMenuItem((char*)self + 0x7F8, 35, &D_004A454C, 0, 1, str_Test_Surface_Types);
    cIntMenuItem_cIntMenuItem((char*)self + 0x828, 36, &D_004A4550, 0, 1, str_Software_Based_Dynamic_Systems);
    cIntMenuItem_cIntMenuItem((char*)self + 0x858, 37, &D_004A4554, 0, 20, str_Temp_Int);
    cFloatMenuItem_ctor((char*)self + 0x888, 38, &D_004A4558, -10.0f, 10.0f, str_PROG2_x);
    cFloatMenuItem_ctor((char*)self + 0x8BC, 39, &D_004A455C, -10.0f, 10.0f, str_PROG2_y);
    cFloatMenuItem_ctor((char*)self + 0x8F0, 40, &D_004A4560, -10.0f, 10.0f, str_PROG2_z);
    cFloatMenuItem_ctor((char*)self + 0x924, 41, &D_004A4564, -10.0f, 10.0f, str_PROG2_w);
    cIntMenuItem_cIntMenuItem((char*)self + 0x958, 42, &D_004A4568, 0, 1, str_Enable_Build_Info);
    cIntMenuItem_cIntMenuItem((char*)self + 0x988, 43, &D_004A456C, 0, 1, str_Enable_NIS_Mem_Usage_Info);
    cIntMenuItem_cIntMenuItem((char*)self + 0x9B8, 44, &D_004A4570, 0, 1, str_Enable_Gameflow_Info);
    cIntMenuItem_cIntMenuItem((char*)self + 0x9E8, 45, &D_004A4574, 0, 1, str_Profile_Individual_Effects);
    cIntMenuItem_cIntMenuItem((char*)self + 0xA18, 46, &D_004A4578, 0, 1, str_Show_Surface_Types);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C4, -1);
    cMenu_addItem(self, (char*)self + 0x1F4, -1);
    cMenu_addItem(self, (char*)self + 0x224, -1);
    cMenu_addItem(self, (char*)self + 0x254, -1);
    cMenu_addItem(self, (char*)self + 0x284, -1);
    cMenu_addItem(self, (char*)self + 0x2B4, -1);
    cMenu_addItem(self, (char*)self + 0x2E4, -1);
    cMenu_addItem(self, (char*)self + 0x314, -1);
    cMenu_addItem(self, (char*)self + 0x344, -1);
    cMenu_addItem(self, (char*)self + 0x374, -1);
    cMenu_addItem(self, (char*)self + 0x3A4, -1);
    cMenu_addItem(self, (char*)self + 0x3D4, -1);
    cMenu_addItem(self, (char*)self + 0x404, -1);
    cMenu_addItem(self, (char*)self + 0x434, -1);
    cMenu_addItem(self, (char*)self + 0x464, -1);
    cMenu_addItem(self, (char*)self + 0x494, -1);
    cMenu_addItem(self, (char*)self + 0x4C4, -1);
    cMenu_addItem(self, (char*)self + 0x4F4, -1);
    cMenu_addItem(self, (char*)self + 0x524, -1);
    cMenu_addItem(self, (char*)self + 0x554, -1);
    cMenu_addItem(self, (char*)self + 0x584, -1);
    cMenu_addItem(self, (char*)self + 0x5B4, -1);
    cMenu_addItem(self, (char*)self + 0x5E4, -1);
    cMenu_addItem(self, (char*)self + 0x614, -1);
    cMenu_addItem(self, (char*)self + 0x644, -1);
    cMenu_addItem(self, (char*)self + 0x674, -1);
    cMenu_addItem(self, (char*)self + 0x6A4, -1);
    cMenu_addItem(self, (char*)self + 0x6D4, -1);
    cMenu_addItem(self, (char*)self + 0x704, -1);
    cMenu_addItem(self, (char*)self + 0x734, -1);
    cMenu_addItem(self, (char*)self + 0x764, -1);
    cMenu_addItem(self, (char*)self + 0x794, -1);
    cMenu_addItem(self, (char*)self + 0x7C4, -1);
    cMenu_addItem(self, (char*)self + 0x7F8, -1);
    cMenu_addItem(self, (char*)self + 0x828, -1);
    cMenu_addItem(self, (char*)self + 0x858, -1);
    cMenu_addItem(self, (char*)self + 0x888, -1);
    cMenu_addItem(self, (char*)self + 0x8BC, -1);
    cMenu_addItem(self, (char*)self + 0x8F0, -1);
    cMenu_addItem(self, (char*)self + 0x924, -1);
    cMenu_addItem(self, (char*)self + 0x958, -1);
    cMenu_addItem(self, (char*)self + 0x988, -1);
    cMenu_addItem(self, (char*)self + 0x9B8, -1);
    cMenu_addItem(self, (char*)self + 0x9E8, -1);
    cMenu_addItem(self, (char*)self + 0xA18, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cVisualEffectsTestMenu_cVisualEffectsTestMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cVisualEffectsTestMenu_vtbl[];
extern char str_Visual_Effects_Test_Menu[];
extern char str_Show_All_HUD_Graphics[];
extern char str_Show_Current_Element[];
extern char str_Output_HUD_Data[];
extern char str_Current_Element_Id[];
extern char str_Position_X[];
extern char str_Position_Y[];
extern char str_Layer_Offset[];
extern char str_World_Painter_Display_Enable[];
extern char str_World_Painter_LayerId[];
extern char str_Size_X[];
extern char str_Size_Y[];
extern char str_Scale_X[];
extern char str_Scale_Y[];
extern char str_Alpha[];
extern char str_Red[];
extern char str_Green[];
extern char str_Blue[];
extern int D_004A4478;
extern int D_004A447C;
extern int D_004A4480;
extern int D_004A4484;
extern int D_004A4488;
extern int D_004A448C;
extern int D_004A4490;
extern int D_004A4494;
extern int D_004A4498;
extern int D_004A449C;
extern int D_004A44A0;
extern int D_004A44A4;
extern int D_004A44A8;
extern int D_004A44AC;
extern int D_004A44B0;
extern int cVisualEffectsTest_m_Enable;
extern int cVisualEffectsTest_m_LayerId;

/**
 * Builds the "Visual Effects Test Menu" debug menu: a title, a spacer and 17 int items for inspecting HUD
 * graphics (show all, current element, output HUD data, its position, size, scale, colour and layer) and the
 * world painter display.
 *
 * @return @p self
 */
extern "C" void* cVisualEffectsTestMenu_cVisualEffectsTestMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cVisualEffectsTestMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Visual_Effects_Test_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A4478, 0, 1, str_Show_All_HUD_Graphics);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &D_004A447C, 0, 1, str_Show_Current_Element);
    cIntMenuItem_cIntMenuItem((char*)self + 0x1C4, 2, &D_004A4480, 0, 1, str_Output_HUD_Data);
    cIntMenuItem_cIntMenuItem((char*)self + 0x1F4, 3, &D_004A4484, 0, 1000, str_Current_Element_Id);
    cIntMenuItem_cIntMenuItem((char*)self + 0x224, 4, &D_004A4488, -640, 640, str_Position_X);
    cIntMenuItem_cIntMenuItem((char*)self + 0x254, 5, &D_004A448C, -480, 0x1e0, str_Position_Y);
    cIntMenuItem_cIntMenuItem((char*)self + 0x284, 6, &D_004A4490, -640, 640, str_Size_X);
    cIntMenuItem_cIntMenuItem((char*)self + 0x2B4, 7, &D_004A4494, -640, 640, str_Size_Y);
    cIntMenuItem_cIntMenuItem((char*)self + 0x2E4, 8, &D_004A4498, -1, 1000, str_Scale_X);
    cIntMenuItem_cIntMenuItem((char*)self + 0x314, 9, &D_004A449C, -1, 1000, str_Scale_Y);
    cIntMenuItem_cIntMenuItem((char*)self + 0x344, 10, &D_004A44A0, -255, 0xff, str_Alpha);
    cIntMenuItem_cIntMenuItem((char*)self + 0x374, 11, &D_004A44A4, -255, 0xff, str_Red);
    cIntMenuItem_cIntMenuItem((char*)self + 0x3A4, 0xc, &D_004A44A8, -255, 0xff, str_Green);
    cIntMenuItem_cIntMenuItem((char*)self + 0x3D4, 13, &D_004A44AC, -255, 0xff, str_Blue);
    cIntMenuItem_cIntMenuItem((char*)self + 0x404, 0xe, &D_004A44B0, -16, 0x10, str_Layer_Offset);
    cIntMenuItem_cIntMenuItem((char*)self + 0x434, 0xf, &cVisualEffectsTest_m_Enable, 0, 1, str_World_Painter_Display_Enable);
    cIntMenuItem_cIntMenuItem((char*)self + 0x464, 0x10, &cVisualEffectsTest_m_LayerId, 0, 100, str_World_Painter_LayerId);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C4, -1);
    cMenu_addItem(self, (char*)self + 0x1F4, -1);
    cMenu_addItem(self, (char*)self + 0x224, -1);
    cMenu_addItem(self, (char*)self + 0x254, -1);
    cMenu_addItem(self, (char*)self + 0x284, -1);
    cMenu_addItem(self, (char*)self + 0x2B4, -1);
    cMenu_addItem(self, (char*)self + 0x2E4, -1);
    cMenu_addItem(self, (char*)self + 0x314, -1);
    cMenu_addItem(self, (char*)self + 0x344, -1);
    cMenu_addItem(self, (char*)self + 0x374, -1);
    cMenu_addItem(self, (char*)self + 0x3A4, -1);
    cMenu_addItem(self, (char*)self + 0x3D4, -1);
    cMenu_addItem(self, (char*)self + 0x404, -1);
    cMenu_addItem(self, (char*)self + 0x434, -1);
    cMenu_addItem(self, (char*)self + 0x464, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cPlantsMenu_cPlantsMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cPlantsMenu_vtbl[];
extern char str_Plants_Menu[];
extern char str_Sway_Amplitude[];
extern char str_Sway_Offset[];
extern char str_Branch_Amplitude[];
extern char str_Rotation_Amplitude[];
extern char str_Position_Scale[];
extern char str_Sway_Frequency_x_600_Hz[];
extern char str_Rotation_Frequency[];
extern char str_Branch_Frequency_x_600_Hz[];
extern char str_Branch_Frequency_2_x_600_Hz[];
extern char str_Sway_Direction[];
extern char str_Branch_Direction[];
extern char str_Enable[];
extern int cPlants_m_Enable;
extern float D_004A4448;
extern float D_004A444C;
extern float D_004A4450;
extern float D_004A4454;
extern float D_004A4458;
extern int D_004A445C;
extern float D_004A4460;
extern float D_004A4464;
extern float D_004A4468;
extern float D_004A446C;
extern float D_004A4470;

/**
 * Builds the "Plants Menu" debug menu: a title, a spacer and an enable switch and 11 items for the plant sway
 * and branch animation (amplitudes, frequencies, directions).
 *
 * @return @p self
 */
extern "C" void* cPlantsMenu_cPlantsMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cPlantsMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Plants_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &cPlants_m_Enable, 0, 2, str_Enable);
    cFloatMenuItem_ctor((char*)self + 0x194, 1, &D_004A4448, 0.0f, 2.0f, str_Sway_Amplitude);
    cFloatMenuItem_ctor((char*)self + 0x1C8, 2, &D_004A444C, -5.0f, 5.0f, str_Sway_Offset);
    cFloatMenuItem_ctor((char*)self + 0x1FC, 3, &D_004A4450, 0.0f, 2.0f, str_Branch_Amplitude);
    cFloatMenuItem_ctor((char*)self + 0x230, 4, &D_004A4454, 0.0f, 2.0f, str_Rotation_Amplitude);
    cFloatMenuItem_ctor((char*)self + 0x264, 5, &D_004A4458, 0.0f, 1000.0f, str_Position_Scale);
    cIntMenuItem_cIntMenuItem((char*)self + 0x298, 6, &D_004A445C, 0, 6000, str_Sway_Frequency_x_600_Hz);
    cFloatMenuItem_ctor((char*)self + 0x2C8, 7, &D_004A4460, 0.0f, 6000.0f, str_Rotation_Frequency);
    cFloatMenuItem_ctor((char*)self + 0x2FC, 8, &D_004A4464, 0.0f, 6000.0f, str_Branch_Frequency_x_600_Hz);
    cFloatMenuItem_ctor((char*)self + 0x330, 9, &D_004A4468, 0.0f, 6000.0f, str_Branch_Frequency_2_x_600_Hz);
    cFloatMenuItem_ctor((char*)self + 0x364, 10, &D_004A446C, 0.0f, 360.0f, str_Sway_Direction);
    cFloatMenuItem_ctor((char*)self + 0x398, 11, &D_004A4470, 0.0f, 360.0f, str_Branch_Direction);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C8, -1);
    cMenu_addItem(self, (char*)self + 0x1FC, -1);
    cMenu_addItem(self, (char*)self + 0x230, -1);
    cMenu_addItem(self, (char*)self + 0x264, -1);
    cMenu_addItem(self, (char*)self + 0x298, -1);
    cMenu_addItem(self, (char*)self + 0x2C8, -1);
    cMenu_addItem(self, (char*)self + 0x2FC, -1);
    cMenu_addItem(self, (char*)self + 0x330, -1);
    cMenu_addItem(self, (char*)self + 0x364, -1);
    cMenu_addItem(self, (char*)self + 0x398, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cLightGlowMenu_cLightGlowMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cLightGlowMenu_vtbl[];
extern char str_Light_Glow_Menu[];
extern char str_Override_World_Painter[];
extern char str_PS2_Capture_Size_2_x[];
extern char str_PS2_Minimum_Intensity_Cutoff[];
extern char str_PS2_Post_Cutoff_Scale[];
extern char str_PS2_Copy_Intensity[];
extern char str_PS2_Frame_Source_Intensity[];
extern char str_PS2_Frame_Blend_Intensity[];
extern char str_PS2_Blend_Texture_0[];
extern char str_PS2_Blend_Texture_1[];
extern char str_PS2_Blend_Texture_2[];
extern char str_PS2_Blend_Texture_3[];
extern char str_PS2_Texture_Jitter[];
extern char str_PS2_Show_Textures[];
extern char str_Colour_Power[];
extern char str_Colour_Scale[];
extern char str_Colour_Ambient[];
extern char str_Size_Power[];
extern char str_Size_Scale[];
extern char str_Size_Ambient[];
extern char str_Fade_Rate[];
extern char str_Background_Alpha[];
extern char str_Intensity_Red[];
extern char str_Intensity_Green[];
extern char str_Intensity_Blue[];
extern char str_Intensity_Alpha[];
extern char str_Intensity_Scale[];
extern char str_Enable[];
extern char str_Size[];
extern char str_Cutoff[];
extern int cLightGlow_m_Enable;
extern int cLightGlow_m_OverrideWP;
extern int D_004A43D0;
extern float cLightGlow_m_PS2MinIntensity;
extern float cLightGlow_m_PS2ModIntensity;
extern float cLightGlow_m_PS2CopyIntensity;
extern float cLightGlow_m_PS2FrameSrcScale;
extern float cLightGlow_m_PS2FrameDstScale;
extern float D_004A43E8;
extern float D_004A43EC;
extern float cLightGlow_m_PS2IntensityTex2;
extern float cLightGlow_m_PS2IntensityTex3;
extern float D_004A43F8;
extern int D_004A43FC;
extern float D_004A4400;
extern float D_004A4404;
extern float D_004A4408;
extern float D_004A440C;
extern float D_004A4410;
extern float D_004A4414;
extern float D_004A4418;
extern float D_004A441C;
extern float D_004A4420;
extern int D_004A4424;
extern float D_004A4428;
extern float D_004A442C;
extern float D_004A4430;
extern float D_004A4434;
extern float D_004A4438;

/**
 * Builds the "Light Glow Menu" debug menu: a title, a spacer and 29 items for the light glow effect: enable,
 * world painter override, the PS2 capture and blend settings, and glow size, colour and intensity curves.
 *
 * @return @p self
 */
extern "C" void* cLightGlowMenu_cLightGlowMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cLightGlowMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Light_Glow_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &cLightGlow_m_Enable, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &cLightGlow_m_OverrideWP, 0, 1, str_Override_World_Painter);
    cIntMenuItem_cIntMenuItem((char*)self + 0x1C4, 2, &D_004A43D0, 4, 8, str_PS2_Capture_Size_2_x);
    cFloatMenuItem_ctor((char*)self + 0x1F4, 3, &cLightGlow_m_PS2MinIntensity, 0.0f, 2.0f, str_PS2_Minimum_Intensity_Cutoff);
    cFloatMenuItem_ctor((char*)self + 0x228, 4, &cLightGlow_m_PS2ModIntensity, 0.0f, 2.0f, str_PS2_Post_Cutoff_Scale);
    cFloatMenuItem_ctor((char*)self + 0x25C, 5, &cLightGlow_m_PS2CopyIntensity, 0.0f, 8.0f, str_PS2_Copy_Intensity);
    cFloatMenuItem_ctor((char*)self + 0x290, 6, &cLightGlow_m_PS2FrameSrcScale, 0.0f, 2.0f, str_PS2_Frame_Source_Intensity);
    cFloatMenuItem_ctor((char*)self + 0x2C4, 7, &cLightGlow_m_PS2FrameDstScale, 0.0f, 2.0f, str_PS2_Frame_Blend_Intensity);
    cFloatMenuItem_ctor((char*)self + 0x2F8, 8, &D_004A43E8, 0.0f, 2.0f, str_PS2_Blend_Texture_0);
    cFloatMenuItem_ctor((char*)self + 0x32C, 9, &D_004A43EC, 0.0f, 2.0f, str_PS2_Blend_Texture_1);
    cFloatMenuItem_ctor((char*)self + 0x360, 10, &cLightGlow_m_PS2IntensityTex2, 0.0f, 2.0f, str_PS2_Blend_Texture_2);
    cFloatMenuItem_ctor((char*)self + 0x394, 11, &cLightGlow_m_PS2IntensityTex3, 0.0f, 2.0f, str_PS2_Blend_Texture_3);
    cFloatMenuItem_ctor((char*)self + 0x3C8, 12, &D_004A43F8, 0.0f, 8.0f, str_PS2_Texture_Jitter);
    cIntMenuItem_cIntMenuItem((char*)self + 0x3FC, 13, &D_004A43FC, 0, 1, str_PS2_Show_Textures);
    cFloatMenuItem_ctor((char*)self + 0x42C, 14, &D_004A4400, 0.0f, 64.0f, str_Size);
    cFloatMenuItem_ctor((char*)self + 0x460, 15, &D_004A4404, 0.0f, 1.0f, str_Cutoff);
    cFloatMenuItem_ctor((char*)self + 0x494, 16, &D_004A4408, 0.0f, 4.0f, str_Colour_Power);
    cFloatMenuItem_ctor((char*)self + 0x4C8, 17, &D_004A440C, 0.0f, 64.0f, str_Colour_Scale);
    cFloatMenuItem_ctor((char*)self + 0x4FC, 18, &D_004A4410, 0.0f, 16.0f, str_Colour_Ambient);
    cFloatMenuItem_ctor((char*)self + 0x530, 19, &D_004A4414, 0.0f, 4.0f, str_Size_Power);
    cFloatMenuItem_ctor((char*)self + 0x564, 20, &D_004A4418, 0.0f, 64.0f, str_Size_Scale);
    cFloatMenuItem_ctor((char*)self + 0x598, 21, &D_004A441C, 0.0f, 64.0f, str_Size_Ambient);
    cFloatMenuItem_ctor((char*)self + 0x5CC, 22, &D_004A4420, 0.0f, 1.0f, str_Fade_Rate);
    cIntMenuItem_cIntMenuItem((char*)self + 0x600, 23, &D_004A4424, 0, 255, str_Background_Alpha);
    cFloatMenuItem_ctor((char*)self + 0x630, 24, &D_004A4428, 0.0f, 1.0f, str_Intensity_Red);
    cFloatMenuItem_ctor((char*)self + 0x664, 25, &D_004A442C, 0.0f, 1.0f, str_Intensity_Green);
    cFloatMenuItem_ctor((char*)self + 0x698, 26, &D_004A4430, 0.0f, 1.0f, str_Intensity_Blue);
    cFloatMenuItem_ctor((char*)self + 0x6CC, 27, &D_004A4434, 0.0f, 1.0f, str_Intensity_Alpha);
    cFloatMenuItem_ctor((char*)self + 0x700, 28, &D_004A4438, 0.0f, 1.0f, str_Intensity_Scale);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C4, -1);
    cMenu_addItem(self, (char*)self + 0x1F4, -1);
    cMenu_addItem(self, (char*)self + 0x228, -1);
    cMenu_addItem(self, (char*)self + 0x25C, -1);
    cMenu_addItem(self, (char*)self + 0x290, -1);
    cMenu_addItem(self, (char*)self + 0x2C4, -1);
    cMenu_addItem(self, (char*)self + 0x2F8, -1);
    cMenu_addItem(self, (char*)self + 0x32C, -1);
    cMenu_addItem(self, (char*)self + 0x360, -1);
    cMenu_addItem(self, (char*)self + 0x394, -1);
    cMenu_addItem(self, (char*)self + 0x3C8, -1);
    cMenu_addItem(self, (char*)self + 0x3FC, -1);
    cMenu_addItem(self, (char*)self + 0x42C, -1);
    cMenu_addItem(self, (char*)self + 0x460, -1);
    cMenu_addItem(self, (char*)self + 0x494, -1);
    cMenu_addItem(self, (char*)self + 0x4C8, -1);
    cMenu_addItem(self, (char*)self + 0x4FC, -1);
    cMenu_addItem(self, (char*)self + 0x530, -1);
    cMenu_addItem(self, (char*)self + 0x564, -1);
    cMenu_addItem(self, (char*)self + 0x598, -1);
    cMenu_addItem(self, (char*)self + 0x5CC, -1);
    cMenu_addItem(self, (char*)self + 0x600, -1);
    cMenu_addItem(self, (char*)self + 0x630, -1);
    cMenu_addItem(self, (char*)self + 0x664, -1);
    cMenu_addItem(self, (char*)self + 0x698, -1);
    cMenu_addItem(self, (char*)self + 0x6CC, -1);
    cMenu_addItem(self, (char*)self + 0x700, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cDepthFogMenu_cDepthFogMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
// PORT: the unit declares cFloatMenuItem_ctor(self, id, var, lo, hi, text); the real parameter order is
// (self, id, var, lo, hi, text) like cIntMenuItem (it decides the arg-move order and FP register allocation here).
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cDepthFogMenu_vtbl[];
extern char str_Depth_Fog_Menu[];
extern char str_Enable[];
extern char str_Override_World_Painter[];
extern char str_Near_Plane[];
extern char str_Far_Plane[];
extern char str_Exponent[];
extern char str_Density[];
extern char str_XBox_Skybox_Density_Scale[];
extern char str_Colour[];
extern char cDepthFog_m_Colour[];
extern int cDepthFog_m_Enable;
extern int cDepthFog_m_OverrideWP;
extern float cDepthFog_m_NearPlane;
extern float cDepthFog_m_FarPlane;
extern int cDepthFog_m_Exponent;
extern float cDepthFog_m_Density;
extern float cDepthFog_m_XBoxDensityScale;

/**
 * Builds the "Depth Fog Menu" debug menu: a title, a spacer and 7 items: enable, world painter override, near
 * and far planes, exponent, density, colour and the Xbox skybox density scale.
 *
 * @return @p self
 */
extern "C" void* cDepthFogMenu_cDepthFogMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cDepthFogMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Depth_Fog_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &cDepthFog_m_Enable, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &cDepthFog_m_OverrideWP, 0, 1, str_Override_World_Painter);
    cFloatMenuItem_ctor((char*)self + 0x1C4, 2, &cDepthFog_m_NearPlane, 1.0f, 100000.0f, str_Near_Plane);
    cFloatMenuItem_ctor((char*)self + 0x1F8, 3, &cDepthFog_m_FarPlane, 1.0f, 100000.0f, str_Far_Plane);
    cIntMenuItem_cIntMenuItem((char*)self + 0x22C, 4, &cDepthFog_m_Exponent, 0, 2, str_Exponent);
    cFloatMenuItem_ctor((char*)self + 0x25C, 5, &cDepthFog_m_Density, 0.0f, 100.0f, str_Density);
    cFloatMenuItem_ctor((char*)self + 0x290, 6, &cDepthFog_m_XBoxDensityScale, 0.0f, 1.0f, str_XBox_Skybox_Density_Scale);
    cARGBMenuItem_cARGBMenuItem((char*)self + 0x2C4, 7, str_Colour, cDepthFog_m_Colour);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C4, -1);
    cMenu_addItem(self, (char*)self + 0x1F8, -1);
    cMenu_addItem(self, (char*)self + 0x22C, -1);
    cMenu_addItem(self, (char*)self + 0x25C, -1);
    cMenu_addItem(self, (char*)self + 0x290, -1);
    cMenu_addItem(self, (char*)self + 0x2C4, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cHeightFogMenu_cHeightFogMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
extern "C" void* cFloatMenuItem_cFloatMenuItem(void* self, int a1, void* var, void* text, float lo, float hi);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cHeightFogMenu_vtbl[];
extern char str_Height_Fog_Menu[];
extern char str_Enable[];
extern char str_Start_Height[];
extern char str_Thickness[];
extern int D_004A43B4;
extern float D_004A43B8;
extern float D_004A43BC;

/**
 * Builds the "Height Fog Menu" debug menu: a title, a spacer and an enable switch, start height and
 * thickness.
 *
 * @return @p self
 */
extern "C" void* cHeightFogMenu_cHeightFogMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cHeightFogMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Height_Fog_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A43B4, 0, 1, str_Enable);
    cFloatMenuItem_cFloatMenuItem((char*)self + 0x194, 1, &D_004A43B8, str_Start_Height, -100000.0f, 100000.0f);
    cFloatMenuItem_cFloatMenuItem((char*)self + 0x1C8, 2, &D_004A43BC, str_Thickness, 9.999999747378752e-05f, 10000.0f);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C8, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cDetailSystemMenu_cDetailSystemMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
// PORT: the unit declares cFloatMenuItem_ctor(self, id, var, lo, hi, text); the real parameter order is
// (self, id, var, lo, hi, text) like cIntMenuItem (it decides the arg-move order and FP register allocation here).
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cDetailSystemMenu_vtbl[];
extern char str_Detail_System_Menu[];
extern char str_Sparkle_Spin_Scale[];
extern char str_PS2_Sparkle_Count[];
extern char str_PS2_Sparkle_World_Size[];
extern char str_PS2_Min_Sparkle_Screen_Size[];
extern char str_PS2_Max_Sparkle_Screen_Size[];
extern char str_XBox_Sparkle_Count[];
extern char str_XBox_World_Size[];
extern char str_XBox_Min_Screen_Size[];
extern char str_XBox_Max_Screen_Size[];
extern char str_Enable[];
extern int D_004A4340;
extern float D_004A4344;
extern float D_004A4348;
extern float D_004A434C;
extern float D_004A4350;
extern float D_004A4354;
extern float D_004A4358;
extern float D_004A435C;
extern float D_004A4360;
extern float D_004A4364;

/**
 * Builds the "Detail System Menu" debug menu: a title, a spacer and an enable switch and the sparkle settings
 * for PS2 and Xbox (count, world size, screen size limits).
 *
 * @return @p self
 */
extern "C" void* cDetailSystemMenu_cDetailSystemMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cDetailSystemMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Detail_System_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A4340, 0, 1, str_Enable);
    cFloatMenuItem_ctor((char*)self + 0x194, 1, &D_004A4344, -1.0f, 1.0f, str_Sparkle_Spin_Scale);
    cFloatMenuItem_ctor((char*)self + 0x1C8, 2, &D_004A4348, 0.0f, 20.0f, str_PS2_Sparkle_Count);
    cFloatMenuItem_ctor((char*)self + 0x1FC, 3, &D_004A434C, 0.0f, 500.0f, str_PS2_Sparkle_World_Size);
    cFloatMenuItem_ctor((char*)self + 0x230, 4, &D_004A4350, 0.0f, 100.0f, str_PS2_Min_Sparkle_Screen_Size);
    cFloatMenuItem_ctor((char*)self + 0x264, 5, &D_004A4354, 0.0f, 100.0f, str_PS2_Max_Sparkle_Screen_Size);
    cFloatMenuItem_ctor((char*)self + 0x298, 6, &D_004A4358, 1.0f, 300.0f, str_XBox_Sparkle_Count);
    cFloatMenuItem_ctor((char*)self + 0x2CC, 7, &D_004A435C, 0.0f, 500.0f, str_XBox_World_Size);
    cFloatMenuItem_ctor((char*)self + 0x300, 8, &D_004A4360, 0.0f, 100.0f, str_XBox_Min_Screen_Size);
    cFloatMenuItem_ctor((char*)self + 0x334, 9, &D_004A4364, 0.0f, 100.0f, str_XBox_Max_Screen_Size);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C8, -1);
    cMenu_addItem(self, (char*)self + 0x1FC, -1);
    cMenu_addItem(self, (char*)self + 0x230, -1);
    cMenu_addItem(self, (char*)self + 0x264, -1);
    cMenu_addItem(self, (char*)self + 0x298, -1);
    cMenu_addItem(self, (char*)self + 0x2CC, -1);
    cMenu_addItem(self, (char*)self + 0x300, -1);
    cMenu_addItem(self, (char*)self + 0x334, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cShellRendererMenu_cShellRendererMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
// PORT: the unit declares cFloatMenuItem_ctor(self, id, var, lo, hi, text); the real parameter order is
// (self, id, var, lo, hi, text) like cIntMenuItem (it decides the arg-move order and FP register allocation here).
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cShellRendererMenu_vtbl[];
extern char str_Near_Plane[];
extern char str_Far_Plane[];
extern char str_Shell_Renderer_Menu[];
extern char str_Model_Texture_Id[];
extern char str_Shell_Texture_Id[];
extern char str_DistScale[];
extern char str_Depth_Scale[];
extern char str_Depth_Brightness[];
extern char str_Max_Shells[];
extern char str_Enable[];
extern int D_004A46B8;
extern int D_004A46BC;
extern int D_004A46C0;
extern int D_004A46C4;
extern int D_004A46C8;
extern int D_004A46CC;
extern float D_004A46D0;
extern float D_004A46D4;
extern int D_004A46D8;

/**
 * Builds the "Shell Renderer Menu" debug menu: a title, a spacer and 9 items for the shell renderer: enable,
 * model and shell texture IDs, near and far planes, distance and depth scales, depth brightness and the shell
 * count.
 *
 * @return @p self
 */
extern "C" void* cShellRendererMenu_cShellRendererMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cShellRendererMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Shell_Renderer_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A46B8, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &D_004A46BC, 0, 200, str_Model_Texture_Id);
    cIntMenuItem_cIntMenuItem((char*)self + 0x1C4, 2, &D_004A46C0, 0, 200, str_Shell_Texture_Id);
    cIntMenuItem_cIntMenuItem((char*)self + 0x1F4, 3, &D_004A46C4, 1, 10000, str_Near_Plane);
    cIntMenuItem_cIntMenuItem((char*)self + 0x224, 4, &D_004A46C8, 1, 10000, str_Far_Plane);
    cIntMenuItem_cIntMenuItem((char*)self + 0x254, 5, &D_004A46CC, 1, 0x186a0, str_DistScale);
    cFloatMenuItem_ctor((char*)self + 0x284, 6, &D_004A46D0, 1e-05f, 2.0f, str_Depth_Scale);
    cFloatMenuItem_ctor((char*)self + 0x2B8, 7, &D_004A46D4, 0.0f, 1.0f, str_Depth_Brightness);
    cIntMenuItem_cIntMenuItem((char*)self + 0x2EC, 8, &D_004A46D8, 1, 0x40, str_Max_Shells);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C4, -1);
    cMenu_addItem(self, (char*)self + 0x1F4, -1);
    cMenu_addItem(self, (char*)self + 0x224, -1);
    cMenu_addItem(self, (char*)self + 0x254, -1);
    cMenu_addItem(self, (char*)self + 0x284, -1);
    cMenu_addItem(self, (char*)self + 0x2B8, -1);
    cMenu_addItem(self, (char*)self + 0x2EC, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cScreenTintMenu_cScreenTintMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cScreenTintMenu_vtbl[];
extern char str_Override_World_Painter[];
extern char str_Screen_Tint_Menu[];
extern char str_Display_Values[];
extern char str_Tint_Red[];
extern char str_Tint_Green[];
extern char str_Tint_Blue[];
extern char str_Fill_Red[];
extern char str_Fill_Green[];
extern char str_Fill_Blue[];
extern char str_XN_Tint_Red[];
extern char str_XN_Tint_Green[];
extern char str_XN_Tint_Blue[];
extern char str_Enable_Lightning[];
extern char str_Frame_Id[];
extern char str_Lightning_Chance[];
extern char str_Flash_Fade_In[];
extern char str_Flash_Sustain[];
extern char str_Inverse_Duration[];
extern char str_Recover_Fade_In[];
extern char str_Flash_Scale[];
extern char str_Inverse_Scale[];
extern char str_Recover_Scale[];
extern char str_Flash_Tint_Red[];
extern char str_Flash_Tint_Green[];
extern char str_Flash_Tint_Blue[];
extern char str_Inverse_Tint_Red[];
extern char str_Inverse_Tint_Green[];
extern char str_Inverse_Tint_Blue[];
extern char str_Recover_Tint_Red[];
extern char str_Recover_Tint_Green[];
extern char str_Recover_Tint_Blue[];
extern char str_Gamma_Enable[];
extern char str_Gamma_Power[];
extern char str_Gamma_Tint_Red[];
extern char str_Gamma_Tint_Green[];
extern char str_Gamma_Tint_Blue[];
extern char str_Gamma_Fill_Red[];
extern char str_Gamma_Fill_Green[];
extern char str_Gamma_Fill_Blue[];
extern char str_Enable[];
extern char str_XN_Lerp[];
extern int cScreenTint_m_Enable;
extern int cScreenTint_m_OverrideWP;
extern int cScreenTint_m_DisplayValues;
extern float cScreenTint_m_ColourR;
extern float cScreenTint_m_ColourG;
extern float cScreenTint_m_ColourB;
extern float cScreenTint_m_FillR;
extern float cScreenTint_m_FillG;
extern float cScreenTint_m_FillB;
extern float cScreenTint_m_XNColourR;
extern float cScreenTint_m_XNColourG;
extern float cScreenTint_m_XNColourB;
extern float D_004A4608;
extern int cScreenTint_m_LightningEnable;
extern int D_004A4610;
extern float cScreenTint_m_LightningChance;
extern int D_004A4618;
extern int D_004A461C;
extern int D_004A4620;
extern int D_004A4624;
extern float D_004A4628;
extern float D_004A462C;
extern float D_004A4630;
extern float D_004A4634;
extern float D_004A4638;
extern float D_004A463C;
extern float D_004A4640;
extern float D_004A4644;
extern float D_004A4648;
extern float D_004A464C;
extern float D_004A4650;
extern float D_004A4654;
extern int D_004A4658;
extern float D_004A465C;
extern float D_004A4660;
extern float D_004A4664;
extern float D_004A4668;
extern float D_004A466C;
extern float D_004A4670;
extern float D_004A4674;

/**
 * Builds the "Screen Tint Menu" debug menu: a title, a spacer and 40 items for the screen tint and fill
 * colours, the lightning flash (chance, fade, sustain, inverse and recover tints) and gamma correction.
 *
 * @return @p self
 */
extern "C" void* cScreenTintMenu_cScreenTintMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cScreenTintMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Screen_Tint_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &cScreenTint_m_Enable, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &cScreenTint_m_OverrideWP, 0, 1, str_Override_World_Painter);
    cIntMenuItem_cIntMenuItem((char*)self + 0x1C4, 2, &cScreenTint_m_DisplayValues, 0, 1, str_Display_Values);
    cFloatMenuItem_ctor((char*)self + 0x1F4, 3, &cScreenTint_m_ColourR, 0.0f, 2.0f, str_Tint_Red);
    cFloatMenuItem_ctor((char*)self + 0x228, 4, &cScreenTint_m_ColourG, 0.0f, 2.0f, str_Tint_Green);
    cFloatMenuItem_ctor((char*)self + 0x25C, 5, &cScreenTint_m_ColourB, 0.0f, 2.0f, str_Tint_Blue);
    cFloatMenuItem_ctor((char*)self + 0x290, 6, &cScreenTint_m_FillR, 0.0f, 1.0f, str_Fill_Red);
    cFloatMenuItem_ctor((char*)self + 0x2C4, 7, &cScreenTint_m_FillG, 0.0f, 1.0f, str_Fill_Green);
    cFloatMenuItem_ctor((char*)self + 0x2F8, 8, &cScreenTint_m_FillB, 0.0f, 1.0f, str_Fill_Blue);
    cFloatMenuItem_ctor((char*)self + 0x32C, 9, &cScreenTint_m_XNColourR, 0.0f, 2.0f, str_XN_Tint_Red);
    cFloatMenuItem_ctor((char*)self + 0x360, 10, &cScreenTint_m_XNColourG, 0.0f, 2.0f, str_XN_Tint_Green);
    cFloatMenuItem_ctor((char*)self + 0x394, 11, &cScreenTint_m_XNColourB, 0.0f, 2.0f, str_XN_Tint_Blue);
    cFloatMenuItem_ctor((char*)self + 0x3C8, 0xc, &D_004A4608, 0.0f, 2.0f, str_XN_Lerp);
    cIntMenuItem_cIntMenuItem((char*)self + 0x3FC, 13, &cScreenTint_m_LightningEnable, 0, 1, str_Enable_Lightning);
    cIntMenuItem_cIntMenuItem((char*)self + 0x42C, 0xe, &D_004A4610, -1, 1000, str_Frame_Id);
    cFloatMenuItem_ctor((char*)self + 0x45C, 0xf, &cScreenTint_m_LightningChance, 0.0f, 1.0f, str_Lightning_Chance);
    cIntMenuItem_cIntMenuItem((char*)self + 0x490, 0x10, &D_004A4618, 0, 600, str_Flash_Fade_In);
    cIntMenuItem_cIntMenuItem((char*)self + 0x4C0, 17, &D_004A461C, 0, 600, str_Flash_Sustain);
    cIntMenuItem_cIntMenuItem((char*)self + 0x4F0, 18, &D_004A4620, 0, 600, str_Inverse_Duration);
    cIntMenuItem_cIntMenuItem((char*)self + 0x520, 19, &D_004A4624, 0, 600, str_Recover_Fade_In);
    cFloatMenuItem_ctor((char*)self + 0x550, 20, &D_004A4628, 0.0f, 2.0f, str_Flash_Scale);
    cFloatMenuItem_ctor((char*)self + 0x584, 21, &D_004A462C, 0.0f, 2.0f, str_Inverse_Scale);
    cFloatMenuItem_ctor((char*)self + 0x5B8, 22, &D_004A4630, 0.0f, 2.0f, str_Recover_Scale);
    cFloatMenuItem_ctor((char*)self + 0x5EC, 23, &D_004A4634, 0.0f, 2.0f, str_Flash_Tint_Red);
    cFloatMenuItem_ctor((char*)self + 0x620, 0x18, &D_004A4638, 0.0f, 2.0f, str_Flash_Tint_Green);
    cFloatMenuItem_ctor((char*)self + 0x654, 25, &D_004A463C, 0.0f, 2.0f, str_Flash_Tint_Blue);
    cFloatMenuItem_ctor((char*)self + 0x688, 26, &D_004A4640, 0.0f, 2.0f, str_Inverse_Tint_Red);
    cFloatMenuItem_ctor((char*)self + 0x6BC, 27, &D_004A4644, 0.0f, 2.0f, str_Inverse_Tint_Green);
    cFloatMenuItem_ctor((char*)self + 0x6F0, 0x1c, &D_004A4648, 0.0f, 2.0f, str_Inverse_Tint_Blue);
    cFloatMenuItem_ctor((char*)self + 0x724, 29, &D_004A464C, 0.0f, 2.0f, str_Recover_Tint_Red);
    cFloatMenuItem_ctor((char*)self + 0x758, 0x1e, &D_004A4650, 0.0f, 2.0f, str_Recover_Tint_Green);
    cFloatMenuItem_ctor((char*)self + 0x78C, 0x1f, &D_004A4654, 0.0f, 2.0f, str_Recover_Tint_Blue);
    cIntMenuItem_cIntMenuItem((char*)self + 0x7C0, 0x20, &D_004A4658, 0, 1, str_Gamma_Enable);
    cFloatMenuItem_ctor((char*)self + 0x7F0, 33, &D_004A465C, 0.0f, 2.0f, str_Gamma_Power);
    cFloatMenuItem_ctor((char*)self + 0x824, 34, &D_004A4660, 0.0f, 2.0f, str_Gamma_Tint_Red);
    cFloatMenuItem_ctor((char*)self + 0x858, 35, &D_004A4664, 0.0f, 2.0f, str_Gamma_Tint_Green);
    cFloatMenuItem_ctor((char*)self + 0x88C, 36, &D_004A4668, 0.0f, 2.0f, str_Gamma_Tint_Blue);
    cFloatMenuItem_ctor((char*)self + 0x8C0, 37, &D_004A466C, 0.0f, 1.0f, str_Gamma_Fill_Red);
    cFloatMenuItem_ctor((char*)self + 0x8F4, 38, &D_004A4670, 0.0f, 1.0f, str_Gamma_Fill_Green);
    cFloatMenuItem_ctor((char*)self + 0x928, 39, &D_004A4674, 0.0f, 1.0f, str_Gamma_Fill_Blue);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C4, -1);
    cMenu_addItem(self, (char*)self + 0x1F4, -1);
    cMenu_addItem(self, (char*)self + 0x228, -1);
    cMenu_addItem(self, (char*)self + 0x25C, -1);
    cMenu_addItem(self, (char*)self + 0x290, -1);
    cMenu_addItem(self, (char*)self + 0x2C4, -1);
    cMenu_addItem(self, (char*)self + 0x2F8, -1);
    cMenu_addItem(self, (char*)self + 0x32C, -1);
    cMenu_addItem(self, (char*)self + 0x360, -1);
    cMenu_addItem(self, (char*)self + 0x394, -1);
    cMenu_addItem(self, (char*)self + 0x3C8, -1);
    cMenu_addItem(self, (char*)self + 0x3FC, -1);
    cMenu_addItem(self, (char*)self + 0x42C, -1);
    cMenu_addItem(self, (char*)self + 0x45C, -1);
    cMenu_addItem(self, (char*)self + 0x490, -1);
    cMenu_addItem(self, (char*)self + 0x4C0, -1);
    cMenu_addItem(self, (char*)self + 0x4F0, -1);
    cMenu_addItem(self, (char*)self + 0x520, -1);
    cMenu_addItem(self, (char*)self + 0x550, -1);
    cMenu_addItem(self, (char*)self + 0x584, -1);
    cMenu_addItem(self, (char*)self + 0x5B8, -1);
    cMenu_addItem(self, (char*)self + 0x5EC, -1);
    cMenu_addItem(self, (char*)self + 0x620, -1);
    cMenu_addItem(self, (char*)self + 0x654, -1);
    cMenu_addItem(self, (char*)self + 0x688, -1);
    cMenu_addItem(self, (char*)self + 0x6BC, -1);
    cMenu_addItem(self, (char*)self + 0x6F0, -1);
    cMenu_addItem(self, (char*)self + 0x724, -1);
    cMenu_addItem(self, (char*)self + 0x758, -1);
    cMenu_addItem(self, (char*)self + 0x78C, -1);
    cMenu_addItem(self, (char*)self + 0x7C0, -1);
    cMenu_addItem(self, (char*)self + 0x7F0, -1);
    cMenu_addItem(self, (char*)self + 0x824, -1);
    cMenu_addItem(self, (char*)self + 0x858, -1);
    cMenu_addItem(self, (char*)self + 0x88C, -1);
    cMenu_addItem(self, (char*)self + 0x8C0, -1);
    cMenu_addItem(self, (char*)self + 0x8F4, -1);
    cMenu_addItem(self, (char*)self + 0x928, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cShadowMenu_cShadowMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
// PORT: the unit declares cFloatMenuItem_ctor(self, id, var, lo, hi, text); the real parameter order is
// (self, id, var, lo, hi, text) like cIntMenuItem (it decides the arg-move order and FP register allocation here).
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cShadowMenu_vtbl[];
extern char str_Shadow_Menu[];
extern char str_Show_Shadow_Polys[];
extern char str_Shadow_Dir_X[];
extern char str_Shadow_Dir_Y[];
extern char str_Shadow_Dir_Z[];
extern char str_Intensity[];
extern char str_Start_UV[];
extern char str_Enable[];
extern char str_Length[];
extern char str_End_UV[];
extern int D_004A41B0;
extern int D_004A41B4;
extern float D_004A41B8;
extern float D_004A41BC;
extern float D_004A41C0;
extern float D_004A41C4;
extern float D_004A41C8;
extern float D_004A41CC;
extern float D_004A41D0;

/**
 * Builds the "Shadow Menu" debug menu: a title, a spacer and 9 items: enable, show shadow polys, the shadow
 * direction, length, intensity and UV range.
 *
 * @return @p self
 */
extern "C" void* cShadowMenu_cShadowMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cShadowMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Shadow_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A41B0, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &D_004A41B4, 0, 1, str_Show_Shadow_Polys);
    cFloatMenuItem_ctor((char*)self + 0x1C4, 2, &D_004A41B8, -1.0f, 1.0f, str_Shadow_Dir_X);
    cFloatMenuItem_ctor((char*)self + 0x1F8, 3, &D_004A41BC, -1.0f, 1.0f, str_Shadow_Dir_Y);
    cFloatMenuItem_ctor((char*)self + 0x22C, 4, &D_004A41C0, -1.0f, 1.0f, str_Shadow_Dir_Z);
    cFloatMenuItem_ctor((char*)self + 0x260, 5, &D_004A41C4, 0.1f, 100000.0f, str_Length);
    cFloatMenuItem_ctor((char*)self + 0x294, 6, &D_004A41C8, 0.0f, 1.0f, str_Intensity);
    cFloatMenuItem_ctor((char*)self + 0x2C8, 7, &D_004A41CC, -64.0f, 64.0f, str_Start_UV);
    cFloatMenuItem_ctor((char*)self + 0x2FC, 8, &D_004A41D0, -64.0f, 64.0f, str_End_UV);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C4, -1);
    cMenu_addItem(self, (char*)self + 0x1F8, -1);
    cMenu_addItem(self, (char*)self + 0x22C, -1);
    cMenu_addItem(self, (char*)self + 0x260, -1);
    cMenu_addItem(self, (char*)self + 0x294, -1);
    cMenu_addItem(self, (char*)self + 0x2C8, -1);
    cMenu_addItem(self, (char*)self + 0x2FC, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cSnowSurfaceMenu_cSnowSurfaceMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
extern "C" void* cFloatMenuItem_cFloatMenuItem(void* self, int a1, void* var, void* text, float lo, float hi);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cSnowSurfaceMenu_vtbl[];
extern char str_Snow_Surface_Menu[];
extern char str_Enable[];
extern char str_Enable_Patch_Reflect[];
extern char str_Base[];
extern char str_Bump[];
extern char str_Noise[];
extern char str_Min_Colour_Add[];
extern char str_Max_Colour_Scale[];
extern int D_004A46DC;
extern int D_004A46E0;
extern float D_004A46E4;
extern float D_004A46E8;
extern float D_004A46EC;
extern float D_004A46F0;
extern float D_004A46F4;

/**
 * Builds the "Snow Surface Menu" debug menu: a title, a spacer and an enable switch, patch reflection, the
 * colour add and scale limits and base, bump and noise values.
 *
 * @return @p self
 */
extern "C" void* cSnowSurfaceMenu_cSnowSurfaceMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cSnowSurfaceMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Snow_Surface_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A46DC, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &D_004A46E0, 0, 1, str_Enable_Patch_Reflect);
    cFloatMenuItem_cFloatMenuItem((char*)self + 0x1C4, 2, &D_004A46E4, str_Base, 0.0f, 32.0f);
    cFloatMenuItem_cFloatMenuItem((char*)self + 0x1F8, 3, &D_004A46E8, str_Bump, 0.0f, 32.0f);
    cFloatMenuItem_cFloatMenuItem((char*)self + 0x22C, 4, &D_004A46EC, str_Noise, 0.0f, 32.0f);
    cFloatMenuItem_cFloatMenuItem((char*)self + 0x260, 5, &D_004A46F0, str_Min_Colour_Add, 0.0f, 3.0f);
    cFloatMenuItem_cFloatMenuItem((char*)self + 0x294, 6, &D_004A46F4, str_Max_Colour_Scale, 0.0f, 3.0f);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C4, -1);
    cMenu_addItem(self, (char*)self + 0x1F8, -1);
    cMenu_addItem(self, (char*)self + 0x22C, -1);
    cMenu_addItem(self, (char*)self + 0x260, -1);
    cMenu_addItem(self, (char*)self + 0x294, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cBoardTrailMenu_cBoardTrailMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cBoardTrailMenu_vtbl[];
extern char str_Board_Trail_Menu[];
extern char str_Inner_Width_Scale[];
extern char str_Outer_Width_Scale[];
extern char str_Height_Scale[];
extern char str_Depth_to_Height_Scale[];
extern char str_Packed_Depth_Scale[];
extern char str_Loose_Depth_Scale[];
extern char str_Powder_Depth_Scale[];
extern char str_Deep_Powder_Depth_Scale[];
extern char str_Top_Bias_Front[];
extern char str_Top_Bias_Behind[];
extern char str_Top_Bias_Clear[];
extern char str_Inner_Position_Jitter[];
extern char str_Outer_Normal_Offset[];
extern char str_Inner_Normal_Offset[];
extern char str_Base_Normal_Offset[];
extern char str_Num_Fade_Segments[];
extern char str_Draw_Fixup_Layer[];
extern char str_Use_Slice_Caps[];
extern char str_Enable[];
extern char str_Base_U[];
extern char str_Inner_U[];
extern char str_Outer_U[];
extern int D_004A457C;
extern int D_004A45C8;
struct sShadowOpt_cBoardTrailMenu {
    int fba;
};
extern sShadowOpt_cBoardTrailMenu D_004A45CC;
extern int D_004A45D0;
extern float D_004A4580;
extern float D_004A4584;
extern float D_004A4588;
extern float D_004A458C;
extern float D_004A4590;
extern float D_004A4594;
extern float D_004A4598;
extern float D_004A459C;
extern float D_004A45A0;
extern float D_004A45A4;
extern float D_004A45A8;
extern float D_004A45AC;
extern float D_004A45B0;
extern float D_004A45B4;
extern float D_004A45B8;
extern float D_004A45BC;
extern float D_004A45C0;
extern float D_004A45C4;

/**
 * Builds the "Board Trail Menu" debug menu: a title, a spacer and 22 items for the board trail mesh: widths,
 * depth per snow type, top biases, UVs, normal offsets, fade segments and slice caps.
 *
 * @return @p self
 */
extern "C" void* cBoardTrailMenu_cBoardTrailMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cBoardTrailMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Board_Trail_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A457C, 0, 0x1, str_Enable);
    cFloatMenuItem_ctor((char*)self + 0x194, 1, &D_004A4580, -100.0f, 100.0f, str_Inner_Width_Scale);
    cFloatMenuItem_ctor((char*)self + 0x1C8, 2, &D_004A4584, -1000.0f, 1000.0f, str_Outer_Width_Scale);
    cFloatMenuItem_ctor((char*)self + 0x1FC, 3, &D_004A4588, -20.0f, 20.0f, str_Height_Scale);
    cFloatMenuItem_ctor((char*)self + 0x230, 4, &D_004A458C, 0.0f, 1.0f, str_Depth_to_Height_Scale);
    cFloatMenuItem_ctor((char*)self + 0x264, 5, &D_004A4590, -100.0f, 100.0f, str_Packed_Depth_Scale);
    cFloatMenuItem_ctor((char*)self + 0x298, 6, &D_004A4594, -100.0f, 100.0f, str_Loose_Depth_Scale);
    cFloatMenuItem_ctor((char*)self + 0x2CC, 7, &D_004A4598, -1000.0f, 1000.0f, str_Powder_Depth_Scale);
    cFloatMenuItem_ctor((char*)self + 0x300, 8, &D_004A459C, -1000.0f, 1000.0f, str_Deep_Powder_Depth_Scale);
    cFloatMenuItem_ctor((char*)self + 0x334, 9, &D_004A45A0, -1000.0f, 1000.0f, str_Top_Bias_Front);
    cFloatMenuItem_ctor((char*)self + 0x368, 10, &D_004A45A4, -1000.0f, 1000.0f, str_Top_Bias_Behind);
    cFloatMenuItem_ctor((char*)self + 0x39C, 11, &D_004A45A8, -16777215.0f, 16777215.0f, str_Top_Bias_Clear);
    cFloatMenuItem_ctor((char*)self + 0x3D0, 12, &D_004A45AC, -10.0f, 10.0f, str_Inner_Position_Jitter);
    cFloatMenuItem_ctor((char*)self + 0x404, 13, &D_004A45B0, 0.0f, 1.0f, str_Base_U);
    cFloatMenuItem_ctor((char*)self + 0x438, 14, &D_004A45B4, 0.0f, 1.0f, str_Inner_U);
    cFloatMenuItem_ctor((char*)self + 0x46C, 15, &D_004A45B8, 0.0f, 1.0f, str_Outer_U);
    cFloatMenuItem_ctor((char*)self + 0x4A0, 16, &D_004A45BC, -10.0f, 10.0f, str_Outer_Normal_Offset);
    cFloatMenuItem_ctor((char*)self + 0x4D4, 17, &D_004A45C0, -10.0f, 10.0f, str_Inner_Normal_Offset);
    cFloatMenuItem_ctor((char*)self + 0x508, 18, &D_004A45C4, -10.0f, 10.0f, str_Base_Normal_Offset);
    cIntMenuItem_cIntMenuItem((char*)self + 0x53C, 19, &D_004A45C8, 0, 0x40, str_Num_Fade_Segments);
    cIntMenuItem_cIntMenuItem((char*)self + 0x56C, 20, &D_004A45CC.fba, 0, 0x1, str_Draw_Fixup_Layer);
    cIntMenuItem_cIntMenuItem((char*)self + 0x59C, 21, &D_004A45D0, 0, 0x1, str_Use_Slice_Caps);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C8, -1);
    cMenu_addItem(self, (char*)self + 0x1FC, -1);
    cMenu_addItem(self, (char*)self + 0x230, -1);
    cMenu_addItem(self, (char*)self + 0x264, -1);
    cMenu_addItem(self, (char*)self + 0x298, -1);
    cMenu_addItem(self, (char*)self + 0x2CC, -1);
    cMenu_addItem(self, (char*)self + 0x300, -1);
    cMenu_addItem(self, (char*)self + 0x334, -1);
    cMenu_addItem(self, (char*)self + 0x368, -1);
    cMenu_addItem(self, (char*)self + 0x39C, -1);
    cMenu_addItem(self, (char*)self + 0x3D0, -1);
    cMenu_addItem(self, (char*)self + 0x404, -1);
    cMenu_addItem(self, (char*)self + 0x438, -1);
    cMenu_addItem(self, (char*)self + 0x46C, -1);
    cMenu_addItem(self, (char*)self + 0x4A0, -1);
    cMenu_addItem(self, (char*)self + 0x4D4, -1);
    cMenu_addItem(self, (char*)self + 0x508, -1);
    cMenu_addItem(self, (char*)self + 0x53C, -1);
    cMenu_addItem(self, (char*)self + 0x56C, -1);
    cMenu_addItem(self, (char*)self + 0x59C, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cFlagsMenu_cFlagsMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cFlagsMenu_vtbl[];
extern char str_Flags_Menu[];
extern char str_EnableUpdate[];
extern char str_EnableRender[];
extern int D_004A4240;
extern int D_004A4244;

/**
 * Builds the "Flags Menu" debug menu: a title, a spacer and two toggles, EnableUpdate and EnableRender.
 *
 * @return @p self
 */
extern "C" void* cFlagsMenu_cFlagsMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cFlagsMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Flags_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A4240, 0, 1, str_EnableUpdate);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &D_004A4244, 0, 1, str_EnableRender);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cPowerUpFXTogglesMenu_cPowerUpFXTogglesMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cPowerUpFXTogglesMenu_vtbl[];
extern char str_PowerUpFXToggles_Menu[];
extern char str_Enable_Speed_Boost_Streamers[];
extern char str_Enable_Speed_Boost_Screen_Blur[];
extern char str_Max_Screen_Blur_Percent[];
extern char str_Screen_Blur_Transition_On_Rate[];
extern char str_Screen_Blur_Transition_Off_Rate[];
extern char str_Screen_Blur_is_not_active_at_this_speed[];
extern char str_Screen_Blur_is_full_at_this_speed[];
extern char str_Speed_Boost_Time_before_Blur[];
extern char str_Test_Trick_Boost[];
extern char str_Enable_SpeedBoostFX[];
extern char str_Uber_State_Test[];
extern char str_Super_Uber_State_Test[];
extern char str_Test_Speed_Boost_Ribbon[];
extern char str_Test_Red_Ribbon[];
extern char str_Test_Orange_Ribbon[];
extern int cPowerUpFXToggles_m_EnableSpeedBoostStreamers;
extern int cPowerUpFXToggles_m_EnableSpeedBoostScreenBlur;
extern float cPowerUpFXToggles_m_SpeedBoostScreenBlurMaxPercent;
extern float cPowerUpFXToggles_m_SpeedBoostScreenBlurIncreaseRate;
extern float cPowerUpFXToggles_m_SpeedBoostScreenBlurDecreaseRate;
extern float cPowerUpFXToggles_m_SpeedBoostMinSpeed;
extern float cPowerUpFXToggles_m_SpeedBoostMaxSpeed;
extern int cPowerUpFXToggles_m_MinSpeedBoostTimeBeforeBlur;
extern int cPowerUpFXToggles_m_TestTrickBoost;
extern int cPowerUpFXToggles_m_EnableSpeedBoostFX;
extern int cPowerUpFXToggles_m_UberStateTest;
extern int cPowerUpFXToggles_m_SuperUberStateTest;
extern int cPowerUpFXToggles_m_TestSpeedBoostRibbon;
extern int cPowerUpFXToggles_m_TestRedRibbon;
extern int cPowerUpFXToggles_m_TestOrangeRibbon;

/**
 * Builds the "PowerUpFXToggles Menu" debug menu: a title, a spacer and 15 items for the speed-boost effects
 * (streamers, screen blur and its speed range) and test switches for the trick boost, uber states and boost
 * ribbons.
 *
 * @return @p self
 */
extern "C" void* cPowerUpFXTogglesMenu_cPowerUpFXTogglesMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cPowerUpFXTogglesMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_PowerUpFXToggles_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &cPowerUpFXToggles_m_EnableSpeedBoostStreamers, 0, 1, str_Enable_Speed_Boost_Streamers);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &cPowerUpFXToggles_m_EnableSpeedBoostScreenBlur, 0, 1, str_Enable_Speed_Boost_Screen_Blur);
    cFloatMenuItem_ctor((char*)self + 0x1C4, 2, &cPowerUpFXToggles_m_SpeedBoostScreenBlurMaxPercent, 0.0f, 1.0f, str_Max_Screen_Blur_Percent);
    cFloatMenuItem_ctor((char*)self + 0x1F8, 3, &cPowerUpFXToggles_m_SpeedBoostScreenBlurIncreaseRate, 0.0f, 60.0f, str_Screen_Blur_Transition_On_Rate);
    cFloatMenuItem_ctor((char*)self + 0x22C, 4, &cPowerUpFXToggles_m_SpeedBoostScreenBlurDecreaseRate, 0.0f, 60.0f, str_Screen_Blur_Transition_Off_Rate);
    cFloatMenuItem_ctor((char*)self + 0x260, 5, &cPowerUpFXToggles_m_SpeedBoostMinSpeed, 0.0f, 120.0f, str_Screen_Blur_is_not_active_at_this_speed);
    cFloatMenuItem_ctor((char*)self + 0x294, 6, &cPowerUpFXToggles_m_SpeedBoostMaxSpeed, 0.0f, 120.0f, str_Screen_Blur_is_full_at_this_speed);
    cIntMenuItem_cIntMenuItem((char*)self + 0x2C8, 7, &cPowerUpFXToggles_m_MinSpeedBoostTimeBeforeBlur, 0, 100, str_Speed_Boost_Time_before_Blur);
    cIntMenuItem_cIntMenuItem((char*)self + 0x2F8, 8, &cPowerUpFXToggles_m_TestTrickBoost, 0, 1, str_Test_Trick_Boost);
    cIntMenuItem_cIntMenuItem((char*)self + 0x328, 9, &cPowerUpFXToggles_m_EnableSpeedBoostFX, 0, 1, str_Enable_SpeedBoostFX);
    cIntMenuItem_cIntMenuItem((char*)self + 0x358, 10, &cPowerUpFXToggles_m_UberStateTest, 0, 1, str_Uber_State_Test);
    cIntMenuItem_cIntMenuItem((char*)self + 0x388, 11, &cPowerUpFXToggles_m_SuperUberStateTest, 0, 1, str_Super_Uber_State_Test);
    cIntMenuItem_cIntMenuItem((char*)self + 0x3B8, 0xc, &cPowerUpFXToggles_m_TestSpeedBoostRibbon, 0, 1, str_Test_Speed_Boost_Ribbon);
    cIntMenuItem_cIntMenuItem((char*)self + 0x3E8, 13, &cPowerUpFXToggles_m_TestRedRibbon, 0, 1, str_Test_Red_Ribbon);
    cIntMenuItem_cIntMenuItem((char*)self + 0x418, 0xe, &cPowerUpFXToggles_m_TestOrangeRibbon, 0, 1, str_Test_Orange_Ribbon);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C4, -1);
    cMenu_addItem(self, (char*)self + 0x1F8, -1);
    cMenu_addItem(self, (char*)self + 0x22C, -1);
    cMenu_addItem(self, (char*)self + 0x260, -1);
    cMenu_addItem(self, (char*)self + 0x294, -1);
    cMenu_addItem(self, (char*)self + 0x2C8, -1);
    cMenu_addItem(self, (char*)self + 0x2F8, -1);
    cMenu_addItem(self, (char*)self + 0x328, -1);
    cMenu_addItem(self, (char*)self + 0x358, -1);
    cMenu_addItem(self, (char*)self + 0x388, -1);
    cMenu_addItem(self, (char*)self + 0x3B8, -1);
    cMenu_addItem(self, (char*)self + 0x3E8, -1);
    cMenu_addItem(self, (char*)self + 0x418, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cSnowfallTogglesMenu_cSnowfallTogglesMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cSnowfallTogglesMenu_vtbl[];
extern char str_Snowfall_Toggles[];
extern char str_Enable_Test_Values[];
extern char str_Test_Snowfall_Intensity[];
extern char str_Test_Snowfall_Wind[];
extern char str_Test_Snowflake_Size[];
extern char str_Test_Wind_Rotation[];
extern char str_Test_Snow_Flurries[];
extern char str_Snow_Gravity_Multiplier[];
extern char str_Snow_Fluff_Intensity[];
extern char str_Snow_Fluff_Wind_Intensity[];
extern char str_Snow_Flake_Colour[];
extern char str_Snow_Fluff_Colour[];
extern char str_Test_Additive_Snowfall[];
extern char cSnowfallToggles_m_SnowFlakeColour[];
extern char cSnowfallToggles_m_SnowFluffColour[];
extern int cSnowfallToggles_m_EnableTest;
extern float cSnowfallToggles_m_TestSnowIntensity;
extern float cSnowfallToggles_m_TestWindIntensity;
extern float cSnowfallToggles_m_TestFlakeSize;
extern float cSnowfallToggles_m_TestWindRotation;
extern float cSnowfallToggles_m_TestSnowFlurries;
extern float cSnowfallToggles_m_SnowGravityMultiplier;
extern float cSnowfallToggles_m_SnowFluffItensity;
extern float cSnowfallToggles_m_SnowFluffWindIntensity;
extern int cSnowfallToggles_m_TestAdditive;

/**
 * Builds the "Snowfall Toggles" debug menu: a title, a spacer and 10 test values for snowfall intensity,
 * wind, flake size, flurries, gravity and fluff.
 *
 * @return @p self
 */
extern "C" void* cSnowfallTogglesMenu_cSnowfallTogglesMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cSnowfallTogglesMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Snowfall_Toggles);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &cSnowfallToggles_m_EnableTest, 0, 1, str_Enable_Test_Values);
    cFloatMenuItem_ctor((char*)self + 0x194, 1, &cSnowfallToggles_m_TestSnowIntensity, 0.0f, 4.0f, str_Test_Snowfall_Intensity);
    cFloatMenuItem_ctor((char*)self + 0x1C8, 2, &cSnowfallToggles_m_TestWindIntensity, 0.0f, 100.0f, str_Test_Snowfall_Wind);
    cFloatMenuItem_ctor((char*)self + 0x1FC, 3, &cSnowfallToggles_m_TestFlakeSize, 0.1f, 10.0f, str_Test_Snowflake_Size);
    cFloatMenuItem_ctor((char*)self + 0x230, 4, &cSnowfallToggles_m_TestWindRotation, 0.0f, 360.0f, str_Test_Wind_Rotation);
    cFloatMenuItem_ctor((char*)self + 0x264, 5, &cSnowfallToggles_m_TestSnowFlurries, 0.0f, 1.0f, str_Test_Snow_Flurries);
    cFloatMenuItem_ctor((char*)self + 0x298, 6, &cSnowfallToggles_m_SnowGravityMultiplier, -2.0f, 5.0f, str_Snow_Gravity_Multiplier);
    cFloatMenuItem_ctor((char*)self + 0x2CC, 7, &cSnowfallToggles_m_SnowFluffItensity, 0.0f, 1.0f, str_Snow_Fluff_Intensity);
    cFloatMenuItem_ctor((char*)self + 0x300, 8, &cSnowfallToggles_m_SnowFluffWindIntensity, 0.0f, 100.0f, str_Snow_Fluff_Wind_Intensity);
    cARGBMenuItem_cARGBMenuItem((char*)self + 0x334, 9, str_Snow_Flake_Colour, cSnowfallToggles_m_SnowFlakeColour);
    cARGBMenuItem_cARGBMenuItem((char*)self + 0x990, 10, str_Snow_Fluff_Colour, cSnowfallToggles_m_SnowFluffColour);
    cIntMenuItem_cIntMenuItem((char*)self + 0xFEC, 11, &cSnowfallToggles_m_TestAdditive, 0, 1, str_Test_Additive_Snowfall);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C8, -1);
    cMenu_addItem(self, (char*)self + 0x1FC, -1);
    cMenu_addItem(self, (char*)self + 0x230, -1);
    cMenu_addItem(self, (char*)self + 0x264, -1);
    cMenu_addItem(self, (char*)self + 0x298, -1);
    cMenu_addItem(self, (char*)self + 0x2CC, -1);
    cMenu_addItem(self, (char*)self + 0x300, -1);
    cMenu_addItem(self, (char*)self + 0x334, -1);
    cMenu_addItem(self, (char*)self + 0x990, -1);
    cMenu_addItem(self, (char*)self + 0xFEC, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cSplashTogglesMenu_cSplashTogglesMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cSplashTogglesMenu_vtbl[];
extern char str_Camera_Splash_Menu[];
extern char str_Max_Drops[];
extern char str_Max_Crystals[];
extern char str_Percentage_Ice_Crystals[];
extern char str_Percentage_Ice_Crystal_Spawn[];
extern char str_Ice_Crystal_Minimum_in_Group[];
extern char str_Ice_Crystal_Maximum_in_Group[];
extern char str_Max_Spawn_Per_Crystal[];
extern char str_Farthest_Impact_Distance[];
extern char str_Lowest_Impact_Intensity[];
extern char str_Lowest_Snowfall_For_Impacts[];
extern char str_Impact_Multiplier[];
extern char str_Lowest_Snowfall_Amount[];
extern char str_Snowfall_Multiplier[];
extern char str_Min_Velocity_For_Pushoff[];
extern char str_Drop_Min_Life_Span[];
extern char str_Drop_Max_Life_Span[];
extern char str_Drop_Min_Size[];
extern char str_Drop_Max_Size[];
extern char str_Drop_Max_Width_Height_Difference[];
extern char str_Drop_Min_Velocity_Jiggle_X[];
extern char str_Drop_Max_Velocity_Jiggle_X[];
extern char str_Drop_Min_Velocity_Jiggle_Y[];
extern char str_Drop_Max_Velocity_Jiggle_Y[];
extern char str_Drop_Min_Size_Growth[];
extern char str_Drop_Max_Size_Growth[];
extern char str_Drop_Min_Push_Off_Speed[];
extern char str_Drop_Max_Push_Off_Speed[];
extern char str_Drop_Maximum_Velocity[];
extern char str_Drop_Min_Velocity_Usage[];
extern char str_Drop_Max_Velocity_Usage[];
extern char str_Crystal_Max_Rotation_Overall_Change[];
extern char str_Drop_Tint_Min[];
extern char str_Drop_Tint_Max[];
extern char str_Crystal_Group_Spacing_X[];
extern char str_Crystal_Group_Spacing_Y[];
extern char str_Crystal_Min_Life_Span[];
extern char str_Crystal_Max_Life_Span[];
extern char str_Crystal_Min_Size[];
extern char str_Crystal_Max_Size[];
extern char str_Crystal_Max_Width_Height_Difference[];
extern char str_Crystal_Min_Velocity_Jiggle_X[];
extern char str_Crystal_Max_Velocity_Jiggle_X[];
extern char str_Crystal_Min_Velocity_Jiggle_Y[];
extern char str_Crystal_Max_Velocity_Jiggle_Y[];
extern char str_Crsytal_Min_Time_In_One_Jiggle_Direction[];
extern char str_Crsytal_Max_Time_In_One_Jiggle_Direction[];
extern char str_Crystal_Min_Size_Growth[];
extern char str_Crystal_Max_Size_Growth[];
extern char str_Crystal_Min_Push_Off_Speed[];
extern char str_Crystal_Max_Push_Off_Speed[];
extern char str_Crystal_Maximum_Velocity[];
extern char str_Crystal_Tint_Min[];
extern char str_Crystal_Tint_Max[];
extern char str_Enable[];
extern char str_Render[];
extern char D_005046E0[];
extern char D_005046F0[];
extern char D_00504700[];
extern char D_00504710[];
extern int D_004A424C;
extern int D_004A4250;
extern int D_004A4254;
extern int D_004A4258;
extern float D_004A425C;
extern float D_004A4260;
extern int D_004A4264;
extern int D_004A4268;
extern int D_004A426C;
extern float D_004A4270;
extern float D_004A4274;
extern float D_004A4278;
extern float D_004A427C;
extern float D_004A4280;
extern float D_004A4284;
extern float D_004A4288;
extern float D_004A428C;
extern float D_004A4290;
extern float D_004A4294;
extern float D_004A4298;
extern float D_004A429C;
extern float D_004A42A0;
extern float D_004A42A4;
extern float D_004A42A8;
extern float D_004A42AC;
extern float D_004A42B0;
extern float D_004A42B4;
extern float D_004A42B8;
extern float D_004A42BC;
extern float D_004A42C0;
extern float D_004A42C4;
extern float D_004A42C8;
extern float D_004A42CC;
extern float D_004A42D0;
extern float D_004A42D4;
extern float D_004A42D8;
extern float D_004A42DC;
extern float D_004A42E0;
extern float D_004A42E4;
extern float D_004A42E8;
extern float D_004A42EC;
extern float D_004A42F0;
extern float D_004A42F4;
extern float D_004A42F8;
extern float D_004A42FC;
extern float D_004A4300;
extern float D_004A4304;
extern float D_004A4308;
extern float D_004A430C;
extern float D_004A4310;
extern float D_004A4314;
extern float D_004A4318;

/**
 * Builds the "Camera Splash Menu" debug menu: a title, a spacer and 52 items for snow drops and ice crystals
 * hitting the camera: counts, spawn chances, impact limits, life spans, sizes, jiggle, push-off and velocity
 * ranges.
 *
 * @return @p self
 */
extern "C" void* cSplashTogglesMenu_cSplashTogglesMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cSplashTogglesMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Camera_Splash_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A424C, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &D_004A4250, 0, 1, str_Render);
    cIntMenuItem_cIntMenuItem((char*)self + 0x1C4, 2, &D_004A4254, 1, 0x1e, str_Max_Drops);
    cIntMenuItem_cIntMenuItem((char*)self + 0x1F4, 3, &D_004A4258, 1, 0x1e, str_Max_Crystals);
    cFloatMenuItem_ctor((char*)self + 0x224, 4, &D_004A425C, 0.0f, 1.0f, str_Percentage_Ice_Crystals);
    cFloatMenuItem_ctor((char*)self + 0x258, 5, &D_004A4260, 0.0f, 1.0f, str_Percentage_Ice_Crystal_Spawn);
    cIntMenuItem_cIntMenuItem((char*)self + 0x28C, 6, &D_004A4264, 1, 10, str_Ice_Crystal_Minimum_in_Group);
    cIntMenuItem_cIntMenuItem((char*)self + 0x2BC, 7, &D_004A4268, 1, 10, str_Ice_Crystal_Maximum_in_Group);
    cIntMenuItem_cIntMenuItem((char*)self + 0x2EC, 8, &D_004A426C, 1, 10, str_Max_Spawn_Per_Crystal);
    cFloatMenuItem_ctor((char*)self + 0x31C, 9, &D_004A4270, 0.0f, 2000.0f, str_Farthest_Impact_Distance);
    cFloatMenuItem_ctor((char*)self + 0x350, 10, &D_004A4274, 0.0f, 120.0f, str_Lowest_Impact_Intensity);
    cFloatMenuItem_ctor((char*)self + 0x384, 11, &D_004A4278, 0.0f, 10.0f, str_Lowest_Snowfall_For_Impacts);
    cFloatMenuItem_ctor((char*)self + 0x3B8, 0xc, &D_004A427C, 0.0f, 10.0f, str_Impact_Multiplier);
    cFloatMenuItem_ctor((char*)self + 0x3EC, 13, &D_004A4280, 0.0f, 10.0f, str_Lowest_Snowfall_Amount);
    cFloatMenuItem_ctor((char*)self + 0x420, 0xe, &D_004A4284, 0.0f, 10.0f, str_Snowfall_Multiplier);
    cFloatMenuItem_ctor((char*)self + 0x454, 0xf, &D_004A4288, 0.0f, 10.0f, str_Min_Velocity_For_Pushoff);
    cFloatMenuItem_ctor((char*)self + 0x488, 0x10, &D_004A428C, 0.0f, 100.0f, str_Drop_Min_Life_Span);
    cFloatMenuItem_ctor((char*)self + 0x4BC, 17, &D_004A4290, 0.0f, 100.0f, str_Drop_Max_Life_Span);
    cFloatMenuItem_ctor((char*)self + 0x4F0, 18, &D_004A4294, 0.0f, 40.0f, str_Drop_Min_Size);
    cFloatMenuItem_ctor((char*)self + 0x524, 19, &D_004A4298, 0.0f, 40.0f, str_Drop_Max_Size);
    cFloatMenuItem_ctor((char*)self + 0x558, 20, &D_004A429C, 0.0f, 10.0f, str_Drop_Max_Width_Height_Difference);
    cFloatMenuItem_ctor((char*)self + 0x58C, 21, &D_004A42A0, -10.0f, 10.0f, str_Drop_Min_Velocity_Jiggle_X);
    cFloatMenuItem_ctor((char*)self + 0x5C0, 22, &D_004A42A4, -10.0f, 10.0f, str_Drop_Max_Velocity_Jiggle_X);
    cFloatMenuItem_ctor((char*)self + 0x5F4, 23, &D_004A42A8, -10.0f, 10.0f, str_Drop_Min_Velocity_Jiggle_Y);
    cFloatMenuItem_ctor((char*)self + 0x628, 0x18, &D_004A42AC, -10.0f, 10.0f, str_Drop_Max_Velocity_Jiggle_Y);
    cFloatMenuItem_ctor((char*)self + 0x65C, 25, &D_004A42B0, -10.0f, 10.0f, str_Drop_Min_Size_Growth);
    cFloatMenuItem_ctor((char*)self + 0x690, 26, &D_004A42B4, -10.0f, 10.0f, str_Drop_Max_Size_Growth);
    cFloatMenuItem_ctor((char*)self + 0x6C4, 27, &D_004A42B8, 0.0f, 10.0f, str_Drop_Min_Push_Off_Speed);
    cFloatMenuItem_ctor((char*)self + 0x6F8, 0x1c, &D_004A42BC, 0.0f, 10.0f, str_Drop_Max_Push_Off_Speed);
    cFloatMenuItem_ctor((char*)self + 0x72C, 29, &D_004A42C0, 0.0f, 100.0f, str_Drop_Maximum_Velocity);
    cFloatMenuItem_ctor((char*)self + 0x760, 0x1e, &D_004A42C4, 0.0f, 10.0f, str_Drop_Min_Velocity_Usage);
    cFloatMenuItem_ctor((char*)self + 0x794, 0x1f, &D_004A42C8, 0.0f, 10.0f, str_Drop_Max_Velocity_Usage);
    cFloatMenuItem_ctor((char*)self + 0x7C8, 0x20, &D_004A42CC, 0.0f, 720.0f, str_Crystal_Max_Rotation_Overall_Change);
    cARGBMenuItem_cARGBMenuItem((char*)self + 0x7FC, 33, str_Drop_Tint_Min, D_005046E0);
    cARGBMenuItem_cARGBMenuItem((char*)self + 0xE58, 34, str_Drop_Tint_Max, D_005046F0);
    cFloatMenuItem_ctor((char*)self + 0x14B4, 35, &D_004A42D0, 0.0f, 100.0f, str_Crystal_Group_Spacing_X);
    cFloatMenuItem_ctor((char*)self + 0x14E8, 36, &D_004A42D4, 0.0f, 100.0f, str_Crystal_Group_Spacing_Y);
    cFloatMenuItem_ctor((char*)self + 0x151C, 37, &D_004A42D8, 0.0f, 100.0f, str_Crystal_Min_Life_Span);
    cFloatMenuItem_ctor((char*)self + 0x1550, 38, &D_004A42DC, 0.0f, 100.0f, str_Crystal_Max_Life_Span);
    cFloatMenuItem_ctor((char*)self + 0x1584, 39, &D_004A42E0, 0.0f, 40.0f, str_Crystal_Min_Size);
    cFloatMenuItem_ctor((char*)self + 0x15B8, 40, &D_004A42E4, 0.0f, 40.0f, str_Crystal_Max_Size);
    cFloatMenuItem_ctor((char*)self + 0x15EC, 41, &D_004A42E8, 0.0f, 10.0f, str_Crystal_Max_Width_Height_Difference);
    cFloatMenuItem_ctor((char*)self + 0x1620, 42, &D_004A42EC, -10.0f, 10.0f, str_Crystal_Min_Velocity_Jiggle_X);
    cFloatMenuItem_ctor((char*)self + 0x1654, 43, &D_004A42F0, -10.0f, 10.0f, str_Crystal_Max_Velocity_Jiggle_X);
    cFloatMenuItem_ctor((char*)self + 0x1688, 44, &D_004A42F4, -10.0f, 10.0f, str_Crystal_Min_Velocity_Jiggle_Y);
    cFloatMenuItem_ctor((char*)self + 0x16BC, 45, &D_004A42F8, -10.0f, 10.0f, str_Crystal_Max_Velocity_Jiggle_Y);
    cFloatMenuItem_ctor((char*)self + 0x16F0, 46, &D_004A42FC, 0.0f, 10.0f, str_Crsytal_Min_Time_In_One_Jiggle_Direction);
    cFloatMenuItem_ctor((char*)self + 0x1724, 47, &D_004A4300, 0.0f, 10.0f, str_Crsytal_Max_Time_In_One_Jiggle_Direction);
    cFloatMenuItem_ctor((char*)self + 0x1758, 0x30, &D_004A4304, -10.0f, 10.0f, str_Crystal_Min_Size_Growth);
    cFloatMenuItem_ctor((char*)self + 0x178C, 49, &D_004A4308, -10.0f, 10.0f, str_Crystal_Max_Size_Growth);
    cFloatMenuItem_ctor((char*)self + 0x17C0, 50, &D_004A430C, 0.0f, 10.0f, str_Crystal_Min_Push_Off_Speed);
    cFloatMenuItem_ctor((char*)self + 0x17F4, 51, &D_004A4310, 0.0f, 10.0f, str_Crystal_Max_Push_Off_Speed);
    cFloatMenuItem_ctor((char*)self + 0x1828, 52, &D_004A4314, 0.0f, 100.0f, str_Crystal_Maximum_Velocity);
    cFloatMenuItem_ctor((char*)self + 0x185C, 53, &D_004A4318, 0.0f, 720.0f, str_Crystal_Max_Rotation_Overall_Change);
    cARGBMenuItem_cARGBMenuItem((char*)self + 0x1890, 54, str_Crystal_Tint_Min, D_00504700);
    cARGBMenuItem_cARGBMenuItem((char*)self + 0x1EEC, 55, str_Crystal_Tint_Max, D_00504710);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C4, -1);
    cMenu_addItem(self, (char*)self + 0x1F4, -1);
    cMenu_addItem(self, (char*)self + 0x224, -1);
    cMenu_addItem(self, (char*)self + 0x258, -1);
    cMenu_addItem(self, (char*)self + 0x28C, -1);
    cMenu_addItem(self, (char*)self + 0x2BC, -1);
    cMenu_addItem(self, (char*)self + 0x2EC, -1);
    cMenu_addItem(self, (char*)self + 0x31C, -1);
    cMenu_addItem(self, (char*)self + 0x350, -1);
    cMenu_addItem(self, (char*)self + 0x384, -1);
    cMenu_addItem(self, (char*)self + 0x3B8, -1);
    cMenu_addItem(self, (char*)self + 0x3EC, -1);
    cMenu_addItem(self, (char*)self + 0x420, -1);
    cMenu_addItem(self, (char*)self + 0x454, -1);
    cMenu_addItem(self, (char*)self + 0x488, -1);
    cMenu_addItem(self, (char*)self + 0x4BC, -1);
    cMenu_addItem(self, (char*)self + 0x4F0, -1);
    cMenu_addItem(self, (char*)self + 0x524, -1);
    cMenu_addItem(self, (char*)self + 0x558, -1);
    cMenu_addItem(self, (char*)self + 0x58C, -1);
    cMenu_addItem(self, (char*)self + 0x5C0, -1);
    cMenu_addItem(self, (char*)self + 0x5F4, -1);
    cMenu_addItem(self, (char*)self + 0x628, -1);
    cMenu_addItem(self, (char*)self + 0x65C, -1);
    cMenu_addItem(self, (char*)self + 0x690, -1);
    cMenu_addItem(self, (char*)self + 0x6C4, -1);
    cMenu_addItem(self, (char*)self + 0x6F8, -1);
    cMenu_addItem(self, (char*)self + 0x72C, -1);
    cMenu_addItem(self, (char*)self + 0x760, -1);
    cMenu_addItem(self, (char*)self + 0x794, -1);
    cMenu_addItem(self, (char*)self + 0x7C8, -1);
    cMenu_addItem(self, (char*)self + 0x7FC, -1);
    cMenu_addItem(self, (char*)self + 0xE58, -1);
    cMenu_addItem(self, (char*)self + 0x14B4, -1);
    cMenu_addItem(self, (char*)self + 0x14E8, -1);
    cMenu_addItem(self, (char*)self + 0x151C, -1);
    cMenu_addItem(self, (char*)self + 0x1550, -1);
    cMenu_addItem(self, (char*)self + 0x1584, -1);
    cMenu_addItem(self, (char*)self + 0x15B8, -1);
    cMenu_addItem(self, (char*)self + 0x15EC, -1);
    cMenu_addItem(self, (char*)self + 0x1620, -1);
    cMenu_addItem(self, (char*)self + 0x1654, -1);
    cMenu_addItem(self, (char*)self + 0x1688, -1);
    cMenu_addItem(self, (char*)self + 0x16BC, -1);
    cMenu_addItem(self, (char*)self + 0x16F0, -1);
    cMenu_addItem(self, (char*)self + 0x1724, -1);
    cMenu_addItem(self, (char*)self + 0x1758, -1);
    cMenu_addItem(self, (char*)self + 0x178C, -1);
    cMenu_addItem(self, (char*)self + 0x17C0, -1);
    cMenu_addItem(self, (char*)self + 0x17F4, -1);
    cMenu_addItem(self, (char*)self + 0x1828, -1);
    cMenu_addItem(self, (char*)self + 0x185C, -1);
    cMenu_addItem(self, (char*)self + 0x1890, -1);
    cMenu_addItem(self, (char*)self + 0x1EEC, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cTubeMenu_cTubeMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
extern "C" void* cFloatMenuItem_cFloatMenuItem(void* self, int a1, void* var, void* text, float lo, float hi);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cTubeMenu_vtbl[];
extern char str_Tube_Menu[];
extern char str_Enable[];
extern char str_Scale_Width[];
extern char str_Scale_Height[];
extern char D_004A2DA8[];
extern char D_004A2DB0[];
extern char D_004A2DB8[];
extern int D_004A41D4;
extern float D_004A41D8;
extern float D_004A41DC;
extern int D_004A41E0;
extern int D_004A41E4;
extern int D_004A41E8;

/**
 * Builds the "Tube Menu" debug menu: a title, a spacer and 6 items: enable, width and height scales and the
 * R, G and B colour.
 *
 * @return @p self
 */
extern "C" void* cTubeMenu_cTubeMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cTubeMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Tube_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A41D4, 0, 1, str_Enable);
    cFloatMenuItem_cFloatMenuItem((char*)self + 0x194, 1, &D_004A41D8, str_Scale_Width, 0.0f, 1000.0f);
    cFloatMenuItem_cFloatMenuItem((char*)self + 0x1C8, 2, &D_004A41DC, str_Scale_Height, 0.0f, 1000.0f);
    cIntMenuItem_cIntMenuItem((char*)self + 0x1FC, 3, &D_004A41E0, 0, 0xFF, D_004A2DA8);
    cIntMenuItem_cIntMenuItem((char*)self + 0x22C, 4, &D_004A41E4, 0, 0xFF, D_004A2DB0);
    cIntMenuItem_cIntMenuItem((char*)self + 0x25C, 5, &D_004A41E8, 0, 0xFF, D_004A2DB8);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C8, -1);
    cMenu_addItem(self, (char*)self + 0x1FC, -1);
    cMenu_addItem(self, (char*)self + 0x22C, -1);
    cMenu_addItem(self, (char*)self + 0x25C, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cPathArrowMenu_cPathArrowMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cPathArrowMenu_vtbl[];
extern char str_PathArrow_Menu[];
extern char str_Forward_Width[];
extern char str_Middle_Width[];
extern char str_Rear_Width[];
extern char str_Forward_Length[];
extern char str_Rear_Length[];
extern char str_Move_Speed[];
extern char str_Draw_Speed[];
extern char str_Distance[];
extern char str_Max_Tick[];
extern char str_Enable[];
extern char D_004A2DA8[];
extern char D_004A2DB0[];
extern char D_004A2DB8[];
extern char str_Height[];
extern char D_004A2DC8[];
extern char str_Path_Id[];
extern int D_004A41EC;
extern float D_004A41F0;
extern float D_004A41F4;
extern float D_004A41F8;
extern float D_004A41FC;
extern float D_004A4200;
extern float D_004A4204;
extern int D_004A4208;
extern int D_004A420C;
extern int D_004A4210;
extern int D_004A4214;
extern float D_004A4218;
extern float D_004A421C;
extern float D_004A4220;
extern int D_004A4224;
extern int D_004A4228;

/**
 * Builds the "PathArrow Menu" debug menu: a title, a spacer and 16 items for the path arrow: enable, widths,
 * lengths, height, RGBA colour, speeds, distance, path ID and maximum tick.
 *
 * @return @p self
 */
extern "C" void* cPathArrowMenu_cPathArrowMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cPathArrowMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_PathArrow_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &D_004A41EC, 0, 1, str_Enable);
    cFloatMenuItem_ctor((char*)self + 0x194, 1, &D_004A41F0, 0.0f, 1000.0f, str_Forward_Width);
    cFloatMenuItem_ctor((char*)self + 0x1C8, 2, &D_004A41F4, 0.0f, 1000.0f, str_Middle_Width);
    cFloatMenuItem_ctor((char*)self + 0x1FC, 3, &D_004A41F8, 0.0f, 1000.0f, str_Rear_Width);
    cFloatMenuItem_ctor((char*)self + 0x230, 4, &D_004A41FC, 0.0f, 1000.0f, str_Forward_Length);
    cFloatMenuItem_ctor((char*)self + 0x264, 5, &D_004A4200, 0.0f, 1000.0f, str_Rear_Length);
    cFloatMenuItem_ctor((char*)self + 0x298, 6, &D_004A4204, 0.0f, 1000.0f, str_Height);
    cIntMenuItem_cIntMenuItem((char*)self + 0x2CC, 7, &D_004A4208, 0, 0xff, D_004A2DA8);
    cIntMenuItem_cIntMenuItem((char*)self + 0x2FC, 8, &D_004A420C, 0, 0xff, D_004A2DB0);
    cIntMenuItem_cIntMenuItem((char*)self + 0x32C, 9, &D_004A4210, 0, 0xff, D_004A2DB8);
    cIntMenuItem_cIntMenuItem((char*)self + 0x35C, 10, &D_004A4214, 0, 0xff, D_004A2DC8);
    cFloatMenuItem_ctor((char*)self + 0x38C, 11, &D_004A4218, 0.0f, 1000.0f, str_Move_Speed);
    cFloatMenuItem_ctor((char*)self + 0x3C0, 0xc, &D_004A421C, 0.0f, 1000.0f, str_Draw_Speed);
    cFloatMenuItem_ctor((char*)self + 0x3F4, 13, &D_004A4220, 50.0f, 1000.0f, str_Distance);
    cIntMenuItem_cIntMenuItem((char*)self + 0x428, 0xe, &D_004A4224, 0, 1000, str_Path_Id);
    cIntMenuItem_cIntMenuItem((char*)self + 0x458, 0xf, &D_004A4228, 1, 18000, str_Max_Tick);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C8, -1);
    cMenu_addItem(self, (char*)self + 0x1FC, -1);
    cMenu_addItem(self, (char*)self + 0x230, -1);
    cMenu_addItem(self, (char*)self + 0x264, -1);
    cMenu_addItem(self, (char*)self + 0x298, -1);
    cMenu_addItem(self, (char*)self + 0x2CC, -1);
    cMenu_addItem(self, (char*)self + 0x2FC, -1);
    cMenu_addItem(self, (char*)self + 0x32C, -1);
    cMenu_addItem(self, (char*)self + 0x35C, -1);
    cMenu_addItem(self, (char*)self + 0x38C, -1);
    cMenu_addItem(self, (char*)self + 0x3C0, -1);
    cMenu_addItem(self, (char*)self + 0x3F4, -1);
    cMenu_addItem(self, (char*)self + 0x428, -1);
    cMenu_addItem(self, (char*)self + 0x458, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cVisualEffectsMainMenu_cVisualEffectsMainMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, int a1, void* text, void* color);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cVisualEffectsMainMenu_vtbl[];
extern char str_Visual_Effects_Main_Menu[];
extern char str_Render_Toggles[];
extern char str_Visual_Effects_Test[];
extern char str_Light_Glow[];
extern char str_Depth_Fog[];
extern char str_Height_Fog[];
extern char str_Detail_System[];
extern char str_Screen_Tint[];
extern char str_Snow_Surface[];
extern char str_Board_Trail[];
extern char str_Power_Up_Effects_Toggles[];
extern char str_Snowfall_Test_Effects[];
extern char str_Camera_Snow_Splash_Effects[];
extern char str_Tube_Effects[];
extern char str_Path_Arrow_Effects[];
extern char str_Plants[];
extern char str_Shadow_b[];
extern char str_Flags[];
extern char D_004C98E0[];
extern char D_004CA328[];
extern char D_004CA7C0[];
extern char D_004CAEF8[];
extern char D_004CB2C8[];
extern char D_004CBBE8[];
extern char D_004CBDE8[];
extern char D_004CC150[];
extern char D_004CCAB0[];
extern char D_004CCDE0[];
extern char D_004CD0A8[];
extern char D_004CD678[];
extern char D_004CD840[];
extern char D_004CDC88[];
extern char D_004CECA8[];
extern char D_004D11F0[];
extern char D_004D1480[];

/**
 * Builds the "Visual Effects Main Menu" debug menu: one submenu entry for each of the 17 visual effects
 * menus.
 *
 * @return @p self
 */
extern "C" void* cVisualEffectsMainMenu_cVisualEffectsMainMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cVisualEffectsMainMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Visual_Effects_Main_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cSubMenuItem_cSubMenuItem((char*)self + 0x164, D_004C98E0, str_Render_Toggles);
    cSubMenuItem_cSubMenuItem((char*)self + 0x180, D_004CA328, str_Visual_Effects_Test);
    cSubMenuItem_cSubMenuItem((char*)self + 0x19C, D_004CA7C0, str_Light_Glow);
    cSubMenuItem_cSubMenuItem((char*)self + 0x1B8, D_004CAEF8, str_Plants);
    cSubMenuItem_cSubMenuItem((char*)self + 0x1D4, D_004CB2C8, str_Depth_Fog);
    cSubMenuItem_cSubMenuItem((char*)self + 0x1F0, D_004CBBE8, str_Height_Fog);
    cSubMenuItem_cSubMenuItem((char*)self + 0x20C, D_004CBDE8, str_Detail_System);
    cSubMenuItem_cSubMenuItem((char*)self + 0x228, D_004CC150, str_Screen_Tint);
    cSubMenuItem_cSubMenuItem((char*)self + 0x244, D_004CCAB0, str_Shadow_b);
    cSubMenuItem_cSubMenuItem((char*)self + 0x260, D_004CCDE0, str_Snow_Surface);
    cSubMenuItem_cSubMenuItem((char*)self + 0x27C, D_004CD0A8, str_Board_Trail);
    cSubMenuItem_cSubMenuItem((char*)self + 0x298, D_004CD678, str_Flags);
    cSubMenuItem_cSubMenuItem((char*)self + 0x2B4, D_004CD840, str_Power_Up_Effects_Toggles);
    cSubMenuItem_cSubMenuItem((char*)self + 0x2D0, D_004CDC88, str_Snowfall_Test_Effects);
    cSubMenuItem_cSubMenuItem((char*)self + 0x2EC, D_004CECA8, str_Camera_Snow_Splash_Effects);
    cSubMenuItem_cSubMenuItem((char*)self + 0x308, D_004D11F0, str_Tube_Effects);
    cSubMenuItem_cSubMenuItem((char*)self + 0x324, D_004D1480, str_Path_Arrow_Effects);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x180, -1);
    cMenu_addItem(self, (char*)self + 0x19C, -1);
    cMenu_addItem(self, (char*)self + 0x1B8, -1);
    cMenu_addItem(self, (char*)self + 0x1D4, -1);
    cMenu_addItem(self, (char*)self + 0x1F0, -1);
    cMenu_addItem(self, (char*)self + 0x20C, -1);
    cMenu_addItem(self, (char*)self + 0x228, -1);
    cMenu_addItem(self, (char*)self + 0x244, -1);
    cMenu_addItem(self, (char*)self + 0x260, -1);
    cMenu_addItem(self, (char*)self + 0x27C, -1);
    cMenu_addItem(self, (char*)self + 0x298, -1);
    cMenu_addItem(self, (char*)self + 0x2B4, -1);
    cMenu_addItem(self, (char*)self + 0x2D0, -1);
    cMenu_addItem(self, (char*)self + 0x2EC, -1);
    cMenu_addItem(self, (char*)self + 0x308, -1);
    cMenu_addItem(self, (char*)self + 0x324, -1);
    return self;
}
#endif

extern "C" void* cMenu_onGainFocus(void* self);

//100%
INCLUDE_ASM("main/debugmenu", cVisualEffectsMainMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Visual Effects Main Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * Only @p self is declared; the cMenuSys pointer the caller passes second reaches cMenu_onGainFocus() unchanged
 * in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cVisualEffectsMainMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

extern "C" void* cMenu_onLoseFocus(void* self);

//100%
INCLUDE_ASM("main/debugmenu", cVisualEffectsMainMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Visual Effects Main Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cVisualEffectsMainMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_0024D6D0);
#ifdef SKIP_ASM
/**
 * Does nothing and returns its first argument: an empty variadic function.
 *
 * It only spills the remaining integer argument registers and the float argument registers to its frame (the variadic
 * prologue) before returning.
 *
 * @return @p self
 */
extern "C" void* func_0024D6D0(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cRenderTogglesMenu__cRenderTogglesMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Render Toggles Menu" debug menu: its 49 embedded menu items in reverse order, then the cMenu
 * base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Render Toggles". __sti__visualeffectsmenu_cpp calls it with flags
 * 2 to destroy the static instance at exit.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cRenderTogglesMenu__cRenderTogglesMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0xA18, 2);
    cMenuItem__cMenuItem((char*)self + 0x9E8, 2);
    cMenuItem__cMenuItem((char*)self + 0x9B8, 2);
    cMenuItem__cMenuItem((char*)self + 0x988, 2);
    cMenuItem__cMenuItem((char*)self + 0x958, 2);
    cMenuItem__cMenuItem((char*)self + 0x924, 2);
    cMenuItem__cMenuItem((char*)self + 0x8F0, 2);
    cMenuItem__cMenuItem((char*)self + 0x8BC, 2);
    cMenuItem__cMenuItem((char*)self + 0x888, 2);
    cMenuItem__cMenuItem((char*)self + 0x858, 2);
    cMenuItem__cMenuItem((char*)self + 0x828, 2);
    cMenuItem__cMenuItem((char*)self + 0x7F8, 2);
    cMenuItem__cMenuItem((char*)self + 0x7C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x794, 2);
    cMenuItem__cMenuItem((char*)self + 0x764, 2);
    cMenuItem__cMenuItem((char*)self + 0x734, 2);
    cMenuItem__cMenuItem((char*)self + 0x704, 2);
    cMenuItem__cMenuItem((char*)self + 0x6D4, 2);
    cMenuItem__cMenuItem((char*)self + 0x6A4, 2);
    cMenuItem__cMenuItem((char*)self + 0x674, 2);
    cMenuItem__cMenuItem((char*)self + 0x644, 2);
    cMenuItem__cMenuItem((char*)self + 0x614, 2);
    cMenuItem__cMenuItem((char*)self + 0x5E4, 2);
    cMenuItem__cMenuItem((char*)self + 0x5B4, 2);
    cMenuItem__cMenuItem((char*)self + 0x584, 2);
    cMenuItem__cMenuItem((char*)self + 0x554, 2);
    cMenuItem__cMenuItem((char*)self + 0x524, 2);
    cMenuItem__cMenuItem((char*)self + 0x4F4, 2);
    cMenuItem__cMenuItem((char*)self + 0x4C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x494, 2);
    cMenuItem__cMenuItem((char*)self + 0x464, 2);
    cMenuItem__cMenuItem((char*)self + 0x434, 2);
    cMenuItem__cMenuItem((char*)self + 0x404, 2);
    cMenuItem__cMenuItem((char*)self + 0x3D4, 2);
    cMenuItem__cMenuItem((char*)self + 0x3A4, 2);
    cMenuItem__cMenuItem((char*)self + 0x374, 2);
    cMenuItem__cMenuItem((char*)self + 0x344, 2);
    cMenuItem__cMenuItem((char*)self + 0x314, 2);
    cMenuItem__cMenuItem((char*)self + 0x2E4, 2);
    cMenuItem__cMenuItem((char*)self + 0x2B4, 2);
    cMenuItem__cMenuItem((char*)self + 0x284, 2);
    cMenuItem__cMenuItem((char*)self + 0x254, 2);
    cMenuItem__cMenuItem((char*)self + 0x224, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F4, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cVisualEffectsTestMenu__cVisualEffectsTestMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Visual Effects Test Menu" debug menu: its 19 embedded menu items in reverse order, then the
 * cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Visual Effects Test". __sti__visualeffectsmenu_cpp calls it with
 * flags 2 to destroy the static instance at exit.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cVisualEffectsTestMenu__cVisualEffectsTestMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x464, 2);
    cMenuItem__cMenuItem((char*)self + 0x434, 2);
    cMenuItem__cMenuItem((char*)self + 0x404, 2);
    cMenuItem__cMenuItem((char*)self + 0x3D4, 2);
    cMenuItem__cMenuItem((char*)self + 0x3A4, 2);
    cMenuItem__cMenuItem((char*)self + 0x374, 2);
    cMenuItem__cMenuItem((char*)self + 0x344, 2);
    cMenuItem__cMenuItem((char*)self + 0x314, 2);
    cMenuItem__cMenuItem((char*)self + 0x2E4, 2);
    cMenuItem__cMenuItem((char*)self + 0x2B4, 2);
    cMenuItem__cMenuItem((char*)self + 0x284, 2);
    cMenuItem__cMenuItem((char*)self + 0x254, 2);
    cMenuItem__cMenuItem((char*)self + 0x224, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F4, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cLightGlowMenu__cLightGlowMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Light Glow Menu" debug menu: its 31 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Light Glow". __sti__visualeffectsmenu_cpp calls it with flags 2 to
 * destroy the static instance at exit.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cLightGlowMenu__cLightGlowMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x700, 2);
    cMenuItem__cMenuItem((char*)self + 0x6CC, 2);
    cMenuItem__cMenuItem((char*)self + 0x698, 2);
    cMenuItem__cMenuItem((char*)self + 0x664, 2);
    cMenuItem__cMenuItem((char*)self + 0x630, 2);
    cMenuItem__cMenuItem((char*)self + 0x600, 2);
    cMenuItem__cMenuItem((char*)self + 0x5CC, 2);
    cMenuItem__cMenuItem((char*)self + 0x598, 2);
    cMenuItem__cMenuItem((char*)self + 0x564, 2);
    cMenuItem__cMenuItem((char*)self + 0x530, 2);
    cMenuItem__cMenuItem((char*)self + 0x4FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x4C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x494, 2);
    cMenuItem__cMenuItem((char*)self + 0x460, 2);
    cMenuItem__cMenuItem((char*)self + 0x42C, 2);
    cMenuItem__cMenuItem((char*)self + 0x3FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x3C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x394, 2);
    cMenuItem__cMenuItem((char*)self + 0x360, 2);
    cMenuItem__cMenuItem((char*)self + 0x32C, 2);
    cMenuItem__cMenuItem((char*)self + 0x2F8, 2);
    cMenuItem__cMenuItem((char*)self + 0x2C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x290, 2);
    cMenuItem__cMenuItem((char*)self + 0x25C, 2);
    cMenuItem__cMenuItem((char*)self + 0x228, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F4, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cDepthFogMenu__cDepthFogMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

static inline void dtorSubMenuItem_24DC68(void* p)
{
    cMenu__cMenu((char*)p + 0x18, 2);
    cMenuItem__cMenuItem(p, 2);
}

/**
 * Destroys the "Depth Fog Menu" debug menu: its 10 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set. The ARGB colour item is torn down inline:
 * its four channel items and RGB title item, its expand sub-menu (cMenu__cMenu() with flags 2), then the item
 * itself.
 *
 * The "Visual Effects Main Menu" lists it as "Depth Fog". __sti__visualeffectsmenu_cpp calls it with flags 2 to
 * destroy the static instance at exit.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cDepthFogMenu__cDepthFogMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x904, 2);
    cMenuItem__cMenuItem((char*)self + 0x8E0, 2);
    cMenuItem__cMenuItem((char*)self + 0x8BC, 2);
    cMenuItem__cMenuItem((char*)self + 0x898, 2);
    cMenuItem__cMenuItem((char*)self + 0x874, 2);
    dtorSubMenuItem_24DC68((char*)self + 0x2C4);
    cMenuItem__cMenuItem((char*)self + 0x290, 2);
    cMenuItem__cMenuItem((char*)self + 0x25C, 2);
    cMenuItem__cMenuItem((char*)self + 0x22C, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F8, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cScreenTintMenu__cScreenTintMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Screen Tint Menu" debug menu: its 42 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Screen Tint". __sti__visualeffectsmenu_cpp calls it with flags 2
 * to destroy the static instance at exit.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScreenTintMenu__cScreenTintMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x928, 2);
    cMenuItem__cMenuItem((char*)self + 0x8F4, 2);
    cMenuItem__cMenuItem((char*)self + 0x8C0, 2);
    cMenuItem__cMenuItem((char*)self + 0x88C, 2);
    cMenuItem__cMenuItem((char*)self + 0x858, 2);
    cMenuItem__cMenuItem((char*)self + 0x824, 2);
    cMenuItem__cMenuItem((char*)self + 0x7F0, 2);
    cMenuItem__cMenuItem((char*)self + 0x7C0, 2);
    cMenuItem__cMenuItem((char*)self + 0x78C, 2);
    cMenuItem__cMenuItem((char*)self + 0x758, 2);
    cMenuItem__cMenuItem((char*)self + 0x724, 2);
    cMenuItem__cMenuItem((char*)self + 0x6F0, 2);
    cMenuItem__cMenuItem((char*)self + 0x6BC, 2);
    cMenuItem__cMenuItem((char*)self + 0x688, 2);
    cMenuItem__cMenuItem((char*)self + 0x654, 2);
    cMenuItem__cMenuItem((char*)self + 0x620, 2);
    cMenuItem__cMenuItem((char*)self + 0x5EC, 2);
    cMenuItem__cMenuItem((char*)self + 0x5B8, 2);
    cMenuItem__cMenuItem((char*)self + 0x584, 2);
    cMenuItem__cMenuItem((char*)self + 0x550, 2);
    cMenuItem__cMenuItem((char*)self + 0x520, 2);
    cMenuItem__cMenuItem((char*)self + 0x4F0, 2);
    cMenuItem__cMenuItem((char*)self + 0x4C0, 2);
    cMenuItem__cMenuItem((char*)self + 0x490, 2);
    cMenuItem__cMenuItem((char*)self + 0x45C, 2);
    cMenuItem__cMenuItem((char*)self + 0x42C, 2);
    cMenuItem__cMenuItem((char*)self + 0x3FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x3C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x394, 2);
    cMenuItem__cMenuItem((char*)self + 0x360, 2);
    cMenuItem__cMenuItem((char*)self + 0x32C, 2);
    cMenuItem__cMenuItem((char*)self + 0x2F8, 2);
    cMenuItem__cMenuItem((char*)self + 0x2C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x290, 2);
    cMenuItem__cMenuItem((char*)self + 0x25C, 2);
    cMenuItem__cMenuItem((char*)self + 0x228, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F4, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cBoardTrailMenu__cBoardTrailMenu);
#ifdef SKIP_ASM
/**
 * Destroys the "Board Trail Menu" debug menu: its 24 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Board Trail". __sti__visualeffectsmenu_cpp calls it with flags 2
 * to destroy the static instance at exit.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cBoardTrailMenu__cBoardTrailMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x59C, 2);
    cMenuItem__cMenuItem((char*)self + 0x56C, 2);
    cMenuItem__cMenuItem((char*)self + 0x53C, 2);
    cMenuItem__cMenuItem((char*)self + 0x508, 2);
    cMenuItem__cMenuItem((char*)self + 0x4D4, 2);
    cMenuItem__cMenuItem((char*)self + 0x4A0, 2);
    cMenuItem__cMenuItem((char*)self + 0x46C, 2);
    cMenuItem__cMenuItem((char*)self + 0x438, 2);
    cMenuItem__cMenuItem((char*)self + 0x404, 2);
    cMenuItem__cMenuItem((char*)self + 0x3D0, 2);
    cMenuItem__cMenuItem((char*)self + 0x39C, 2);
    cMenuItem__cMenuItem((char*)self + 0x368, 2);
    cMenuItem__cMenuItem((char*)self + 0x334, 2);
    cMenuItem__cMenuItem((char*)self + 0x300, 2);
    cMenuItem__cMenuItem((char*)self + 0x2CC, 2);
    cMenuItem__cMenuItem((char*)self + 0x298, 2);
    cMenuItem__cMenuItem((char*)self + 0x264, 2);
    cMenuItem__cMenuItem((char*)self + 0x230, 2);
    cMenuItem__cMenuItem((char*)self + 0x1FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cPowerUpFXTogglesMenu__cPowerUpFXTogglesMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "PowerUpFXToggles Menu" debug menu: its 17 embedded menu items in reverse order, then the cMenu
 * base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Power Up Effects Toggles". __sti__visualeffectsmenu_cpp calls it
 * with flags 2 to destroy the static instance at exit.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPowerUpFXTogglesMenu__cPowerUpFXTogglesMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x418, 2);
    cMenuItem__cMenuItem((char*)self + 0x3E8, 2);
    cMenuItem__cMenuItem((char*)self + 0x3B8, 2);
    cMenuItem__cMenuItem((char*)self + 0x388, 2);
    cMenuItem__cMenuItem((char*)self + 0x358, 2);
    cMenuItem__cMenuItem((char*)self + 0x328, 2);
    cMenuItem__cMenuItem((char*)self + 0x2F8, 2);
    cMenuItem__cMenuItem((char*)self + 0x2C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x294, 2);
    cMenuItem__cMenuItem((char*)self + 0x260, 2);
    cMenuItem__cMenuItem((char*)self + 0x22C, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F8, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cSnowfallTogglesMenu__cSnowfallTogglesMenu);
#ifdef SKIP_ASM
static inline void dtorSubMenuItem_24E200(void* p)
{
    cMenu__cMenu((char*)p + 0x18, 2);
    cMenuItem__cMenuItem(p, 2);
}

/**
 * Destroys the "Snowfall Toggles" debug menu: its 14 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set. The ARGB colour items are torn down
 * inline: each one's four channel items and RGB title item, its expand sub-menu (cMenu__cMenu() with flags 2),
 * then the item itself.
 *
 * The "Visual Effects Main Menu" lists it as "Snowfall Test Effects". __sti__visualeffectsmenu_cpp calls it with
 * flags 2 to destroy the static instance at exit.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cSnowfallTogglesMenu__cSnowfallTogglesMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0xFEC, 2);
    cMenuItem__cMenuItem((char*)self + 0xFD0, 2);
    cMenuItem__cMenuItem((char*)self + 0xFAC, 2);
    cMenuItem__cMenuItem((char*)self + 0xF88, 2);
    cMenuItem__cMenuItem((char*)self + 0xF64, 2);
    cMenuItem__cMenuItem((char*)self + 0xF40, 2);
    dtorSubMenuItem_24E200((char*)self + 0x990);
    cMenuItem__cMenuItem((char*)self + 0x974, 2);
    cMenuItem__cMenuItem((char*)self + 0x950, 2);
    cMenuItem__cMenuItem((char*)self + 0x92C, 2);
    cMenuItem__cMenuItem((char*)self + 0x908, 2);
    cMenuItem__cMenuItem((char*)self + 0x8E4, 2);
    dtorSubMenuItem_24E200((char*)self + 0x334);
    cMenuItem__cMenuItem((char*)self + 0x300, 2);
    cMenuItem__cMenuItem((char*)self + 0x2CC, 2);
    cMenuItem__cMenuItem((char*)self + 0x298, 2);
    cMenuItem__cMenuItem((char*)self + 0x264, 2);
    cMenuItem__cMenuItem((char*)self + 0x230, 2);
    cMenuItem__cMenuItem((char*)self + 0x1FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cSplashTogglesMenu__cSplashTogglesMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

static inline void dtorSubMenuItem_24E388(void* p)
{
    cMenu__cMenu((char*)p + 0x18, 2);
    cMenuItem__cMenuItem(p, 2);
}

/**
 * Destroys the "Camera Splash Menu" debug menu: its 58 embedded menu items in reverse order, then the cMenu
 * base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set. The ARGB colour items are torn down
 * inline: each one's four channel items and RGB title item, its expand sub-menu (cMenu__cMenu() with flags 2),
 * then the item itself.
 *
 * The "Visual Effects Main Menu" lists it as "Camera Snow Splash Effects". __sti__visualeffectsmenu_cpp calls it
 * with flags 2 to destroy the static instance at exit.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cSplashTogglesMenu__cSplashTogglesMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x252C, 2);
    cMenuItem__cMenuItem((char*)self + 0x2508, 2);
    cMenuItem__cMenuItem((char*)self + 0x24E4, 2);
    cMenuItem__cMenuItem((char*)self + 0x24C0, 2);
    cMenuItem__cMenuItem((char*)self + 0x249C, 2);
    dtorSubMenuItem_24E388((char*)self + 0x1EEC);
    cMenuItem__cMenuItem((char*)self + 0x1ED0, 2);
    cMenuItem__cMenuItem((char*)self + 0x1EAC, 2);
    cMenuItem__cMenuItem((char*)self + 0x1E88, 2);
    cMenuItem__cMenuItem((char*)self + 0x1E64, 2);
    cMenuItem__cMenuItem((char*)self + 0x1E40, 2);
    dtorSubMenuItem_24E388((char*)self + 0x1890);
    cMenuItem__cMenuItem((char*)self + 0x185C, 2);
    cMenuItem__cMenuItem((char*)self + 0x1828, 2);
    cMenuItem__cMenuItem((char*)self + 0x17F4, 2);
    cMenuItem__cMenuItem((char*)self + 0x17C0, 2);
    cMenuItem__cMenuItem((char*)self + 0x178C, 2);
    cMenuItem__cMenuItem((char*)self + 0x1758, 2);
    cMenuItem__cMenuItem((char*)self + 0x1724, 2);
    cMenuItem__cMenuItem((char*)self + 0x16F0, 2);
    cMenuItem__cMenuItem((char*)self + 0x16BC, 2);
    cMenuItem__cMenuItem((char*)self + 0x1688, 2);
    cMenuItem__cMenuItem((char*)self + 0x1654, 2);
    cMenuItem__cMenuItem((char*)self + 0x1620, 2);
    cMenuItem__cMenuItem((char*)self + 0x15EC, 2);
    cMenuItem__cMenuItem((char*)self + 0x15B8, 2);
    cMenuItem__cMenuItem((char*)self + 0x1584, 2);
    cMenuItem__cMenuItem((char*)self + 0x1550, 2);
    cMenuItem__cMenuItem((char*)self + 0x151C, 2);
    cMenuItem__cMenuItem((char*)self + 0x14E8, 2);
    cMenuItem__cMenuItem((char*)self + 0x14B4, 2);
    cMenuItem__cMenuItem((char*)self + 0x1498, 2);
    cMenuItem__cMenuItem((char*)self + 0x1474, 2);
    cMenuItem__cMenuItem((char*)self + 0x1450, 2);
    cMenuItem__cMenuItem((char*)self + 0x142C, 2);
    cMenuItem__cMenuItem((char*)self + 0x1408, 2);
    dtorSubMenuItem_24E388((char*)self + 0xE58);
    cMenuItem__cMenuItem((char*)self + 0xE3C, 2);
    cMenuItem__cMenuItem((char*)self + 0xE18, 2);
    cMenuItem__cMenuItem((char*)self + 0xDF4, 2);
    cMenuItem__cMenuItem((char*)self + 0xDD0, 2);
    cMenuItem__cMenuItem((char*)self + 0xDAC, 2);
    dtorSubMenuItem_24E388((char*)self + 0x7FC);
    cMenuItem__cMenuItem((char*)self + 0x7C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x794, 2);
    cMenuItem__cMenuItem((char*)self + 0x760, 2);
    cMenuItem__cMenuItem((char*)self + 0x72C, 2);
    cMenuItem__cMenuItem((char*)self + 0x6F8, 2);
    cMenuItem__cMenuItem((char*)self + 0x6C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x690, 2);
    cMenuItem__cMenuItem((char*)self + 0x65C, 2);
    cMenuItem__cMenuItem((char*)self + 0x628, 2);
    cMenuItem__cMenuItem((char*)self + 0x5F4, 2);
    cMenuItem__cMenuItem((char*)self + 0x5C0, 2);
    cMenuItem__cMenuItem((char*)self + 0x58C, 2);
    cMenuItem__cMenuItem((char*)self + 0x558, 2);
    cMenuItem__cMenuItem((char*)self + 0x524, 2);
    cMenuItem__cMenuItem((char*)self + 0x4F0, 2);
    cMenuItem__cMenuItem((char*)self + 0x4BC, 2);
    cMenuItem__cMenuItem((char*)self + 0x488, 2);
    cMenuItem__cMenuItem((char*)self + 0x454, 2);
    cMenuItem__cMenuItem((char*)self + 0x420, 2);
    cMenuItem__cMenuItem((char*)self + 0x3EC, 2);
    cMenuItem__cMenuItem((char*)self + 0x3B8, 2);
    cMenuItem__cMenuItem((char*)self + 0x384, 2);
    cMenuItem__cMenuItem((char*)self + 0x350, 2);
    cMenuItem__cMenuItem((char*)self + 0x31C, 2);
    cMenuItem__cMenuItem((char*)self + 0x2EC, 2);
    cMenuItem__cMenuItem((char*)self + 0x2BC, 2);
    cMenuItem__cMenuItem((char*)self + 0x28C, 2);
    cMenuItem__cMenuItem((char*)self + 0x258, 2);
    cMenuItem__cMenuItem((char*)self + 0x224, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F4, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cPathArrowMenu__cPathArrowMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "PathArrow Menu" debug menu: its 18 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Path Arrow Effects". __sti__visualeffectsmenu_cpp calls it with
 * flags 2 to destroy the static instance at exit.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPathArrowMenu__cPathArrowMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x458, 2);
    cMenuItem__cMenuItem((char*)self + 0x428, 2);
    cMenuItem__cMenuItem((char*)self + 0x3F4, 2);
    cMenuItem__cMenuItem((char*)self + 0x3C0, 2);
    cMenuItem__cMenuItem((char*)self + 0x38C, 2);
    cMenuItem__cMenuItem((char*)self + 0x35C, 2);
    cMenuItem__cMenuItem((char*)self + 0x32C, 2);
    cMenuItem__cMenuItem((char*)self + 0x2FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x2CC, 2);
    cMenuItem__cMenuItem((char*)self + 0x298, 2);
    cMenuItem__cMenuItem((char*)self + 0x264, 2);
    cMenuItem__cMenuItem((char*)self + 0x230, 2);
    cMenuItem__cMenuItem((char*)self + 0x1FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", __sti__visualeffectsmenu_cpp);
#ifdef SKIP_ASM
struct sV4_E8D8 {
    float x, y, z, w;
    sV4_E8D8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
    // Class-scope placement new: the unit's global one is defined later (__sti__aivisualeffectsmenu_cpp).
    void* operator new(unsigned int, void* p) { return p; }
} __attribute__((aligned(16)));

struct sSlot_E8D8 {
    unsigned a : 8;
    unsigned b : 24;
    int c;
    sSlot_E8D8(int n) { a = n; b = n; }
    void* operator new(unsigned int, void* p) { return p; }
};

extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);
extern "C" void* cRenderTogglesMenu_cRenderTogglesMenu(void* self);
extern "C" void cRenderTogglesMenu__cRenderTogglesMenu(void* self, int flags);
extern "C" void* cVisualEffectsTestMenu_cVisualEffectsTestMenu(void* self);
extern "C" void cVisualEffectsTestMenu__cVisualEffectsTestMenu(void* self, int flags);
extern "C" void* cLightGlowMenu_cLightGlowMenu(void* self);
extern "C" void cLightGlowMenu__cLightGlowMenu(void* self, int flags);
extern "C" void* cPlantsMenu_cPlantsMenu(void* self);
extern "C" void* cDepthFogMenu_cDepthFogMenu(void* self);
extern "C" void cDepthFogMenu__cDepthFogMenu(void* self, int flags);
extern "C" void* cHeightFogMenu_cHeightFogMenu(void* self);
extern "C" void* cDetailSystemMenu_cDetailSystemMenu(void* self);
extern "C" void* cScreenTintMenu_cScreenTintMenu(void* self);
extern "C" void cScreenTintMenu__cScreenTintMenu(void* self, int flags);
extern "C" void* cShadowMenu_cShadowMenu(void* self);
extern "C" void* cSnowSurfaceMenu_cSnowSurfaceMenu(void* self);
extern "C" void* cBoardTrailMenu_cBoardTrailMenu(void* self);
extern "C" void cBoardTrailMenu__cBoardTrailMenu(void* self, int flags);
extern "C" void* cFlagsMenu_cFlagsMenu(void* self);
extern "C" void* cPowerUpFXTogglesMenu_cPowerUpFXTogglesMenu(void* self);
extern "C" void cPowerUpFXTogglesMenu__cPowerUpFXTogglesMenu(void* self, int flags);
extern "C" void* cSnowfallTogglesMenu_cSnowfallTogglesMenu(void* self);
extern "C" void cSnowfallTogglesMenu__cSnowfallTogglesMenu(void* self, int flags);
extern "C" void* cSplashTogglesMenu_cSplashTogglesMenu(void* self);
extern "C" void cSplashTogglesMenu__cSplashTogglesMenu(void* self, int flags);
extern "C" void* cTubeMenu_cTubeMenu(void* self);
extern "C" void* cPathArrowMenu_cPathArrowMenu(void* self);
extern "C" void cPathArrowMenu__cPathArrowMenu(void* self, int flags);

// cPlantsMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cPlantsMenu_dtor(void* self)
{
    cMenuItem__cMenuItem((char*)self + 0x398, 2);
    cMenuItem__cMenuItem((char*)self + 0x364, 2);
    cMenuItem__cMenuItem((char*)self + 0x330, 2);
    cMenuItem__cMenuItem((char*)self + 0x2FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x2C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x298, 2);
    cMenuItem__cMenuItem((char*)self + 0x264, 2);
    cMenuItem__cMenuItem((char*)self + 0x230, 2);
    cMenuItem__cMenuItem((char*)self + 0x1FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, 2);
}

// cHeightFogMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cHeightFogMenu_dtor(void* self)
{
    cMenuItem__cMenuItem((char*)self + 0x1C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, 2);
}

// cDetailSystemMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cDetailSystemMenu_dtor(void* self)
{
    cMenuItem__cMenuItem((char*)self + 0x334, 2);
    cMenuItem__cMenuItem((char*)self + 0x300, 2);
    cMenuItem__cMenuItem((char*)self + 0x2CC, 2);
    cMenuItem__cMenuItem((char*)self + 0x298, 2);
    cMenuItem__cMenuItem((char*)self + 0x264, 2);
    cMenuItem__cMenuItem((char*)self + 0x230, 2);
    cMenuItem__cMenuItem((char*)self + 0x1FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, 2);
}

// cShadowMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cShadowMenu_dtor(void* self)
{
    cMenuItem__cMenuItem((char*)self + 0x2FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x2C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x294, 2);
    cMenuItem__cMenuItem((char*)self + 0x260, 2);
    cMenuItem__cMenuItem((char*)self + 0x22C, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F8, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, 2);
}

// cSnowSurfaceMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cSnowSurfaceMenu_dtor(void* self)
{
    cMenuItem__cMenuItem((char*)self + 0x294, 2);
    cMenuItem__cMenuItem((char*)self + 0x260, 2);
    cMenuItem__cMenuItem((char*)self + 0x22C, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F8, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, 2);
}

// cFlagsMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cFlagsMenu_dtor(void* self)
{
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, 2);
}

// cTubeMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cTubeMenu_dtor(void* self)
{
    cMenuItem__cMenuItem((char*)self + 0x25C, 2);
    cMenuItem__cMenuItem((char*)self + 0x22C, 2);
    cMenuItem__cMenuItem((char*)self + 0x1FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, 2);
}

extern sV4_E8D8 D_004C9560;
extern sV4_E8D8 D_004C9570;
extern sV4_E8D8 D_004C9580;
extern sV4_E8D8 D_004C9590;
extern sV4_E8D8 D_004C95A0;
extern sV4_E8D8 D_004C95B0;
extern sV4_E8D8 D_004C95C0;
extern sV4_E8D8 D_004C95D0;
extern sV4_E8D8 D_004C95E0;
extern sV4_E8D8 D_004C95F0;
extern sV4_E8D8 D_004C9600;
extern sV4_E8D8 D_004C9610;
extern sV4_E8D8 D_004C9620;
extern sV4_E8D8 D_004C9630;
extern sV4_E8D8 D_004C9640;
extern sV4_E8D8 D_004C9650;
extern sV4_E8D8 D_004C9660;
extern sV4_E8D8 D_004C9670;
extern sV4_E8D8 D_004C9680;
extern sV4_E8D8 D_004C9690;
extern sV4_E8D8 D_004C96A0;
extern sV4_E8D8 D_004C96B0;
extern sV4_E8D8 D_004C96C0;
extern sV4_E8D8 D_004C96D0;
extern sV4_E8D8 D_004C96E0;
extern sV4_E8D8 D_004C96F0;
extern sV4_E8D8 D_004C9700;
extern sV4_E8D8 D_004C9710;
extern sV4_E8D8 D_004C9720;
extern sV4_E8D8 D_004C9730;
extern sV4_E8D8 D_004C9740;
extern sV4_E8D8 D_004C9750;
extern sV4_E8D8 D_004C9760;
extern sV4_E8D8 D_004C9770;
extern sV4_E8D8 D_004C9780;
extern sV4_E8D8 D_004C9790;
extern sV4_E8D8 D_004C97A0;
extern sV4_E8D8 D_004C97B0;
extern sV4_E8D8 D_004C97C0;
extern sV4_E8D8 D_004C97D0;
extern sV4_E8D8 D_004C97E0;
extern sV4_E8D8 D_004C97F0;
extern sV4_E8D8 D_004C9800;
extern sV4_E8D8 D_004C9810;
extern sV4_E8D8 D_004C9820;
extern sV4_E8D8 D_004C9830;
extern sV4_E8D8 D_004C9840;
extern sV4_E8D8 D_004C9850;
extern sV4_E8D8 D_004C9860;
extern sV4_E8D8 D_004C9870;
extern sV4_E8D8 D_004C9880;
extern sV4_E8D8 D_004C9890;
extern sV4_E8D8 D_004C98A0;
extern sV4_E8D8 D_004C98B0;
extern sV4_E8D8 D_004C98C0;
extern sV4_E8D8 D_004C98D0;
extern char D_004C98E0[], D_004CA328[], D_004CA7C0[], D_004CAEF8[], D_004CB2C8[], D_004CBBE8[], D_004CBDE8[], D_004CC150[], D_004CCAB0[], D_004CCDE0[], D_004CD0A8[], D_004CD678[], D_004CD840[], D_004CDC88[], D_004CECA8[], D_004D11F0[], D_004D1480[];
extern int D_004A4FE8;
extern int D_004A4FEC;
extern sSlot_E8D8 D_004A4FF0, D_004A4FF8, D_004A5000, D_004A5008, D_004A5010, D_004A5018, D_004A5020, D_004A5028, D_004A5030, D_004A5038, D_004A5040, D_004A5048, D_004A5050, D_004A5058;

// PORT: the unit declares __sti__visualeffectsmenu_cpp as void*(int, int); it returns nothing.
extern "C" void __sti__visualeffectsmenu_cpp_impl(int init, int prio) __asm__("__sti__visualeffectsmenu_cpp");
/**
 * Constructs or destroys the visual-effects debug menus' static objects (the g++ static initialisation and
 * destruction routine of visualeffectsmenu.cpp).
 *
 * Acts only for the default priority. With @p init set it builds the 56 four-float constants (the same table
 * that opens the static initialisers of path/pathsys and object/railmodifier, probably colours), zeroes
 * D_004A4FE8 and D_004A4FEC, numbers the 14 key objects D_004A4FF0..D_004A5058 from 0 to 13, then constructs the
 * 17 static sub-menus that the "Visual Effects Main Menu" links to. With @p init clear it destroys those 17
 * menus in reverse order with flags 2 (no free), inlining the destructors of seven of them.
 *
 * @param init  nonzero to construct the static objects, 0 to destroy them
 * @param prio  initialisation priority; nothing happens unless it is 0xFFFF (the default)
 */
extern "C" void __sti__visualeffectsmenu_cpp_impl(int init, int prio)
{
    if (prio == 0xFFFF) {
        if (init != 0) {
            new (&D_004C9560) sV4_E8D8(1.0f, 1.0f, 1.0f, 0.5f);
            new (&D_004C9570) sV4_E8D8(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
            new (&D_004C9580) sV4_E8D8(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
            new (&D_004C9590) sV4_E8D8(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
            new (&D_004C95A0) sV4_E8D8(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
            new (&D_004C95B0) sV4_E8D8(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
            new (&D_004C95C0) sV4_E8D8(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
            new (&D_004C95D0) sV4_E8D8(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
            new (&D_004C95E0) sV4_E8D8(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
            new (&D_004C95F0) sV4_E8D8(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
            new (&D_004C9600) sV4_E8D8(1.0f, 1.0f, 1.0f, 1.0f);
            new (&D_004C9610) sV4_E8D8(1.0f, 0.0f, 0.0f, 0.0f);
            new (&D_004C9620) sV4_E8D8(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
            new (&D_004C9630) sV4_E8D8(1.0f, 1.0f, 1.0f, 1.0f);
            new (&D_004C9640) sV4_E8D8(1.0f, 1.0f, 1.0f, 1.0f);
            new (&D_004C9650) sV4_E8D8(1.0f, 0.0f, 0.0f, 0.0f);
            new (&D_004C9660) sV4_E8D8(1.0f, 0.0f, 0.0f, 0.0f);
            new (&D_004C9670) sV4_E8D8(1.0f, 1.0f, 1.0f, 1.0f);
            new (&D_004C9680) sV4_E8D8(1.0f, 0.5f, 1.0f, 0.5f);
            new (&D_004C9690) sV4_E8D8(1.0f, 0.5f, 1.0f, 0.5f);
            new (&D_004C96A0) sV4_E8D8(1.0f, 0.5f, 1.0f, 0.5f);
            new (&D_004C96B0) sV4_E8D8(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
            new (&D_004C96C0) sV4_E8D8(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
            new (&D_004C96D0) sV4_E8D8(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
            new (&D_004C96E0) sV4_E8D8(1.0f, 1.0f, 1.0f, 1.0f);
            new (&D_004C96F0) sV4_E8D8(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
            new (&D_004C9700) sV4_E8D8(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
            new (&D_004C9710) sV4_E8D8(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
            new (&D_004C9720) sV4_E8D8(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
            new (&D_004C9730) sV4_E8D8(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
            new (&D_004C9740) sV4_E8D8(1.0f, 1.0f, 1.0f, 1.0f);
            new (&D_004C9750) sV4_E8D8(1.0f, 1.0f, 1.0f, 1.0f);
            new (&D_004C9760) sV4_E8D8(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
            new (&D_004C9770) sV4_E8D8(1.0f, 1.0f, 1.0f, 1.0f);
            new (&D_004C9780) sV4_E8D8(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
            new (&D_004C9790) sV4_E8D8(1.0f, 1.0f, 1.0f, 1.0f);
            new (&D_004C97A0) sV4_E8D8(1.0f, 1.0f, 0.0f, 0.0f);
            new (&D_004C97B0) sV4_E8D8(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
            new (&D_004C97C0) sV4_E8D8(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
            new (&D_004C97D0) sV4_E8D8(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
            new (&D_004C97E0) sV4_E8D8(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
            new (&D_004C97F0) sV4_E8D8(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
            new (&D_004C9800) sV4_E8D8(1.0f, 1.0f, 1.0f, 1.0f);
            new (&D_004C9810) sV4_E8D8(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
            new (&D_004C9820) sV4_E8D8(1.0f, 1.0f, 0.0f, 0.0f);
            new (&D_004C9830) sV4_E8D8(1.0f, 1.0f, 0.0f, 0.0f);
            new (&D_004C9840) sV4_E8D8(1.0f, 1.0f, 0.0f, 0.0f);
            new (&D_004C9850) sV4_E8D8(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
            new (&D_004C9860) sV4_E8D8(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
            new (&D_004C9870) sV4_E8D8(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
            new (&D_004C9880) sV4_E8D8(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
            new (&D_004C9890) sV4_E8D8(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
            new (&D_004C98A0) sV4_E8D8(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
            new (&D_004C98B0) sV4_E8D8(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
            new (&D_004C98C0) sV4_E8D8(1.0f, 0.0f, 1.0f, 0.0f);
            new (&D_004C98D0) sV4_E8D8(1.0f, 1.0f, 0.0f, 0.0f);
            D_004A4FEC = 0;
            D_004A4FE8 = 0;
            new (&D_004A4FF0) sSlot_E8D8(0);
            new (&D_004A4FF8) sSlot_E8D8(1);
            new (&D_004A5000) sSlot_E8D8(2);
            new (&D_004A5008) sSlot_E8D8(3);
            new (&D_004A5010) sSlot_E8D8(4);
            new (&D_004A5018) sSlot_E8D8(5);
            new (&D_004A5020) sSlot_E8D8(6);
            new (&D_004A5028) sSlot_E8D8(7);
            new (&D_004A5030) sSlot_E8D8(8);
            new (&D_004A5038) sSlot_E8D8(9);
            new (&D_004A5040) sSlot_E8D8(10);
            new (&D_004A5048) sSlot_E8D8(11);
            new (&D_004A5050) sSlot_E8D8(12);
            new (&D_004A5058) sSlot_E8D8(13);
            cRenderTogglesMenu_cRenderTogglesMenu(D_004C98E0);
            cVisualEffectsTestMenu_cVisualEffectsTestMenu(D_004CA328);
            cLightGlowMenu_cLightGlowMenu(D_004CA7C0);
            cPlantsMenu_cPlantsMenu(D_004CAEF8);
            cDepthFogMenu_cDepthFogMenu(D_004CB2C8);
            cHeightFogMenu_cHeightFogMenu(D_004CBBE8);
            cDetailSystemMenu_cDetailSystemMenu(D_004CBDE8);
            cScreenTintMenu_cScreenTintMenu(D_004CC150);
            cShadowMenu_cShadowMenu(D_004CCAB0);
            cSnowSurfaceMenu_cSnowSurfaceMenu(D_004CCDE0);
            cBoardTrailMenu_cBoardTrailMenu(D_004CD0A8);
            cFlagsMenu_cFlagsMenu(D_004CD678);
            cPowerUpFXTogglesMenu_cPowerUpFXTogglesMenu(D_004CD840);
            cSnowfallTogglesMenu_cSnowfallTogglesMenu(D_004CDC88);
            cSplashTogglesMenu_cSplashTogglesMenu(D_004CECA8);
            cTubeMenu_cTubeMenu(D_004D11F0);
            cPathArrowMenu_cPathArrowMenu(D_004D1480);
        } else {
            cPathArrowMenu__cPathArrowMenu(D_004D1480, 2);
            cTubeMenu_dtor(D_004D11F0);
            cSplashTogglesMenu__cSplashTogglesMenu(D_004CECA8, 2);
            cSnowfallTogglesMenu__cSnowfallTogglesMenu(D_004CDC88, 2);
            cPowerUpFXTogglesMenu__cPowerUpFXTogglesMenu(D_004CD840, 2);
            cFlagsMenu_dtor(D_004CD678);
            cBoardTrailMenu__cBoardTrailMenu(D_004CD0A8, 2);
            cSnowSurfaceMenu_dtor(D_004CCDE0);
            cShadowMenu_dtor(D_004CCAB0);
            cScreenTintMenu__cScreenTintMenu(D_004CC150, 2);
            cDetailSystemMenu_dtor(D_004CBDE8);
            cHeightFogMenu_dtor(D_004CBBE8);
            cDepthFogMenu__cDepthFogMenu(D_004CB2C8, 2);
            cPlantsMenu_dtor(D_004CAEF8);
            cLightGlowMenu__cLightGlowMenu(D_004CA7C0, 2);
            cVisualEffectsTestMenu__cVisualEffectsTestMenu(D_004CA328, 2);
            cRenderTogglesMenu__cRenderTogglesMenu(D_004C98E0, 2);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cVisualEffectsMainMenu__cVisualEffectsMainMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Visual Effects Main Menu" debug menu: its 19 embedded menu items in reverse order, then the
 * cMenu base.
 *
 * The items are the title, the spacer and the 17 sub-menu links to the static effect menus. Each item goes
 * through the base cMenuItem destructor with flags 2, which only resets the item's vtable; cMenu__cMenu() then
 * frees the object when bit 0 of @p flags is set.
 *
 * No direct call is known: it is reached through the destructor slot of the menu's vtable.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cVisualEffectsMainMenu__cVisualEffectsMainMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x324, 2);
    cMenuItem__cMenuItem((char*)self + 0x308, 2);
    cMenuItem__cMenuItem((char*)self + 0x2EC, 2);
    cMenuItem__cMenuItem((char*)self + 0x2D0, 2);
    cMenuItem__cMenuItem((char*)self + 0x2B4, 2);
    cMenuItem__cMenuItem((char*)self + 0x298, 2);
    cMenuItem__cMenuItem((char*)self + 0x27C, 2);
    cMenuItem__cMenuItem((char*)self + 0x260, 2);
    cMenuItem__cMenuItem((char*)self + 0x244, 2);
    cMenuItem__cMenuItem((char*)self + 0x228, 2);
    cMenuItem__cMenuItem((char*)self + 0x20C, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F0, 2);
    cMenuItem__cMenuItem((char*)self + 0x1D4, 2);
    cMenuItem__cMenuItem((char*)self + 0x1B8, 2);
    cMenuItem__cMenuItem((char*)self + 0x19C, 2);
    cMenuItem__cMenuItem((char*)self + 0x180, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cRenderTogglesMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Render Toggles Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Render Toggles". Only @p self is declared; the cMenuSys
 * pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cRenderTogglesMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cRenderTogglesMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Render Toggles Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cRenderTogglesMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cVisualEffectsTestMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Visual Effects Test Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Visual Effects Test". Only @p self is declared; the
 * cMenuSys pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cVisualEffectsTestMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cVisualEffectsTestMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Visual Effects Test Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cVisualEffectsTestMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cPlantsMenu__cPlantsMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Plants Menu" debug menu: its 14 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Plants". __sti__visualeffectsmenu_cpp destroys the static instance
 * with an inlined copy of this body, so this out-of-line copy is reached through the menu's vtable.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPlantsMenu__cPlantsMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x398, 2);
    cMenuItem__cMenuItem((char*)self + 0x364, 2);
    cMenuItem__cMenuItem((char*)self + 0x330, 2);
    cMenuItem__cMenuItem((char*)self + 0x2FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x2C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x298, 2);
    cMenuItem__cMenuItem((char*)self + 0x264, 2);
    cMenuItem__cMenuItem((char*)self + 0x230, 2);
    cMenuItem__cMenuItem((char*)self + 0x1FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cPlantsMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Plants Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Plants". Only @p self is declared; the cMenuSys pointer
 * the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cPlantsMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cPlantsMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Plants Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cPlantsMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cLightGlowMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Light Glow Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Light Glow". Only @p self is declared; the cMenuSys
 * pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cLightGlowMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cLightGlowMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Light Glow Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cLightGlowMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cDepthFogMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Depth Fog Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Depth Fog". Only @p self is declared; the cMenuSys
 * pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cDepthFogMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cDepthFogMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Depth Fog Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cDepthFogMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cHeightFogMenu__cHeightFogMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Height Fog Menu" debug menu: its 5 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Height Fog". __sti__visualeffectsmenu_cpp destroys the static
 * instance with an inlined copy of this body, so this out-of-line copy is reached through the menu's vtable.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cHeightFogMenu__cHeightFogMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x1C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cHeightFogMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Height Fog Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Height Fog". Only @p self is declared; the cMenuSys
 * pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cHeightFogMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cHeightFogMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Height Fog Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cHeightFogMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cDetailSystemMenu__cDetailSystemMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Detail System Menu" debug menu: its 12 embedded menu items in reverse order, then the cMenu
 * base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Detail System". __sti__visualeffectsmenu_cpp destroys the static
 * instance with an inlined copy of this body, so this out-of-line copy is reached through the menu's vtable.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cDetailSystemMenu__cDetailSystemMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x334, 2);
    cMenuItem__cMenuItem((char*)self + 0x300, 2);
    cMenuItem__cMenuItem((char*)self + 0x2CC, 2);
    cMenuItem__cMenuItem((char*)self + 0x298, 2);
    cMenuItem__cMenuItem((char*)self + 0x264, 2);
    cMenuItem__cMenuItem((char*)self + 0x230, 2);
    cMenuItem__cMenuItem((char*)self + 0x1FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cDetailSystemMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Detail System Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Detail System". Only @p self is declared; the cMenuSys
 * pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cDetailSystemMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cDetailSystemMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Detail System Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cDetailSystemMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cShellRendererMenu__cShellRendererMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Shell Renderer Menu" debug menu: its 11 embedded menu items in reverse order, then the cMenu
 * base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * No direct call is known: it is reached through the destructor slot of the menu's vtable.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cShellRendererMenu__cShellRendererMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x2EC, 2);
    cMenuItem__cMenuItem((char*)self + 0x2B8, 2);
    cMenuItem__cMenuItem((char*)self + 0x284, 2);
    cMenuItem__cMenuItem((char*)self + 0x254, 2);
    cMenuItem__cMenuItem((char*)self + 0x224, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F4, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cShellRendererMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Shell Renderer Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * Only @p self is declared; the cMenuSys pointer the caller passes second reaches cMenu_onGainFocus() unchanged
 * in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cShellRendererMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cShellRendererMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Shell Renderer Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cShellRendererMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cScreenTintMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Screen Tint Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Screen Tint". Only @p self is declared; the cMenuSys
 * pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cScreenTintMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cScreenTintMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Screen Tint Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cScreenTintMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cShadowMenu__cShadowMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Shadow Menu" debug menu: its 11 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Shadow". __sti__visualeffectsmenu_cpp destroys the static instance
 * with an inlined copy of this body, so this out-of-line copy is reached through the menu's vtable.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cShadowMenu__cShadowMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x2FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x2C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x294, 2);
    cMenuItem__cMenuItem((char*)self + 0x260, 2);
    cMenuItem__cMenuItem((char*)self + 0x22C, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F8, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cShadowMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Shadow Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Shadow". Only @p self is declared; the cMenuSys pointer
 * the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cShadowMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cShadowMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Shadow Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cShadowMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cSnowSurfaceMenu__cSnowSurfaceMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Snow Surface Menu" debug menu: its 9 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Snow Surface". __sti__visualeffectsmenu_cpp destroys the static
 * instance with an inlined copy of this body, so this out-of-line copy is reached through the menu's vtable.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cSnowSurfaceMenu__cSnowSurfaceMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x294, 2);
    cMenuItem__cMenuItem((char*)self + 0x260, 2);
    cMenuItem__cMenuItem((char*)self + 0x22C, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F8, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cSnowSurfaceMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Snow Surface Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Snow Surface". Only @p self is declared; the cMenuSys
 * pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cSnowSurfaceMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cSnowSurfaceMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Snow Surface Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cSnowSurfaceMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cBoardTrailMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Board Trail Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Board Trail". Only @p self is declared; the cMenuSys
 * pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cBoardTrailMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cBoardTrailMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Board Trail Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cBoardTrailMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cFlagsMenu__cFlagsMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Flags Menu" debug menu: its 4 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Flags". __sti__visualeffectsmenu_cpp destroys the static instance
 * with an inlined copy of this body, so this out-of-line copy is reached through the menu's vtable.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFlagsMenu__cFlagsMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cFlagsMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Flags Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Flags". Only @p self is declared; the cMenuSys pointer
 * the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cFlagsMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cFlagsMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Flags Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cFlagsMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cPowerUpFXTogglesMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "PowerUpFXToggles Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Power Up Effects Toggles". Only @p self is declared;
 * the cMenuSys pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cPowerUpFXTogglesMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cPowerUpFXTogglesMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "PowerUpFXToggles Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cPowerUpFXTogglesMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cSnowfallTogglesMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Snowfall Toggles" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Snowfall Test Effects". Only @p self is declared; the
 * cMenuSys pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cSnowfallTogglesMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cSnowfallTogglesMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Snowfall Toggles" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cSnowfallTogglesMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cSplashTogglesMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the splash toggles menu focus using the base cMenu_onGainFocus().
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cSplashTogglesMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cSplashTogglesMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Removes focus from the splash toggles menu using the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cSplashTogglesMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cTubeMenu__cTubeMenu);
#ifdef SKIP_ASM
/**
 * Destroys the "Tube Menu" debug menu: its 8 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "Visual Effects Main Menu" lists it as "Tube Effects". __sti__visualeffectsmenu_cpp destroys the static
 * instance with an inlined copy of this body, so this out-of-line copy is reached through the menu's vtable.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cTubeMenu__cTubeMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x25C, 2);
    cMenuItem__cMenuItem((char*)self + 0x22C, 2);
    cMenuItem__cMenuItem((char*)self + 0x1FC, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C8, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cTubeMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Tube Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Tube Effects". Only @p self is declared; the cMenuSys
 * pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cTubeMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cTubeMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Tube Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cTubeMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cPathArrowMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "PathArrow Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "Visual Effects Main Menu" debug menu lists it as "Path Arrow Effects". Only @p self is declared; the
 * cMenuSys pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cPathArrowMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cPathArrowMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "PathArrow Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cPathArrowMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

extern "C" void* __sti__visualeffectsmenu_cpp(int, int);

//99.38%
INCLUDE_ASM("main/debugmenu", func_002501A8__FPv);
#ifdef SKIP_ASM
/**
 * Runs the visual effects menu unit's static constructors: calls __sti__visualeffectsmenu_cpp(1, 0xFFFF).
 *
 * It is entry +0x30 of the global constructor table D_0043CE38 that __do_global_ctors (libgcc's __do_global_ctors) walks;
 * func_002501C8 is its destructor twin. The declared parameter is not used.
 *
 * @return __sti__visualeffectsmenu_cpp's result
 */
void* func_002501A8(void* self)
{
    return __sti__visualeffectsmenu_cpp(1, 0xffff);
}
#endif

extern "C" void* __sti__visualeffectsmenu_cpp(int, int);

//99.38%
INCLUDE_ASM("main/debugmenu", func_002501C8__FPv);
#ifdef SKIP_ASM
/**
 * Runs the visual effects menu unit's static destructors: calls __sti__visualeffectsmenu_cpp(0, 0xFFFF).
 *
 * It is entry +0x14 of the global destructor table D_0043CEE4; func_002501A8 is its constructor twin. The declared
 * parameter is not used.
 *
 * @return __sti__visualeffectsmenu_cpp's result
 */
void* func_002501C8(void* self)
{
    return __sti__visualeffectsmenu_cpp(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cAvalancheMenu_cAvalancheMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cIntMenuItem_cIntMenuItem(void* self, int a1, void* var, int a3, int a4, void* text);
void* cFloatMenuItem_ctor(void* self, int a1, void* var, float lo, float hi, void* text) __asm__("cFloatMenuItem_cFloatMenuItem");
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cAvalancheMenu_vtbl[];
extern char str_Avalanche_Menu[];
extern char str_Brightness[];
extern char str_Use_Light_Map_Colour[];
extern char str_Use_End_Colour[];
extern char str_Square_Lerp_Value[];
extern char str_Min_Impact_Velocity[];
extern char str_Render_Emitters[];
extern char str_Render_N_Lines[];
extern char str_Trigger_Avalanche[];
extern char str_Update_Time_Step[];
extern char str_Rotational_Velocity_Scale[];
extern char str_Random_Bounce_Chance[];
extern char str_Random_Bounce_Velocity_Scale_XY[];
extern char str_Random_Bounce_Velocity_Scale_Z[];
extern char str_Random_Bounce_Velocity_Scale[];
extern char str_Avalanche_Id[];
extern char str_Velocity_Damping[];
extern char str_Normal_Velocity_Scale[];
extern char str_Forward_Velocity_Scale[];
extern char str_Normal_To_Forward_Scale[];
extern char str_Max_Velocity[];
extern char str_Num_Particles[];
extern char str_Default_Size[];
extern char str_Patch_Colour_Scale[];
extern char str_Patch_Colour_Scale_2[];
extern char str_Enable_b[];
extern char str_Recalc[];
extern char str_Save[];
extern char str_Load[];
extern char str_Update[];
extern char str_Gravity[];
extern char str_Damping[];
extern char str_Seed_b[];
extern int cAvalanche_m_Enable;
extern int cAvalanche_m_Recalc;
extern int D_004A3A3C;
extern int D_004A3A40;
extern int D_004A3A44;
extern int D_004A3A48;
extern int D_004A3A4C;
extern int D_004A3A54;
extern int D_004A3A58;
extern int cAvalanche_m_Trigger;
extern int cAvalanche_m_Update;
extern int cAvalanche_m_AvalancheId;
extern int D_004A3A9C;
extern float D_004A3A38;
extern float D_004A3A50;
extern float cAvalanche_m_UpdateTimeStep;
extern float cAvalanche_m_RotationalVelocityScale;
extern float cAvalanche_m_RandomBounceChance;
extern float cAvalanche_m_RandomBounceVelocityScaleXY;
extern float cAvalanche_m_RandomBounceVelocityScaleZ;
extern float cAvalanche_m_RandomBounceVelocityScale;
extern float cAvalanche_m_Gravity;
extern float D_004A3A84;
extern float cAvalanche_m_NormalVelocityScale;
extern float D_004A3A8C;
extern float D_004A3A90;
extern float D_004A3A94;
extern float D_004A3A98;
extern float D_004A3AA0;
extern float D_004A3AA4;
extern float D_004A3AA8;
extern float D_004A3AAC;

/**
 * Builds the "Avalanche Menu" debug menu: a title, a spacer and 32 items for the avalanche effect: enable,
 * recalc, save and load, trigger, update step, bounce and velocity scales, particle count, size, colour
 * scales and seed.
 *
 * @return @p self
 */
extern "C" void* cAvalancheMenu_cAvalancheMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cAvalancheMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_Avalanche_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cIntMenuItem_cIntMenuItem((char*)self + 0x164, 0, &cAvalanche_m_Enable, 0, 1, str_Enable_b);
    cIntMenuItem_cIntMenuItem((char*)self + 0x194, 1, &cAvalanche_m_Recalc, -1, 1, str_Recalc);
    cFloatMenuItem_ctor((char*)self + 0x1C4, 2, &D_004A3A38, 0.0f, 4.0f, str_Brightness);
    cIntMenuItem_cIntMenuItem((char*)self + 0x1F8, 3, &D_004A3A3C, 0, 1, str_Use_Light_Map_Colour);
    cIntMenuItem_cIntMenuItem((char*)self + 0x228, 4, &D_004A3A40, 0, 1, str_Use_End_Colour);
    cIntMenuItem_cIntMenuItem((char*)self + 0x258, 5, &D_004A3A44, 0, 1, str_Square_Lerp_Value);
    cIntMenuItem_cIntMenuItem((char*)self + 0x288, 6, &D_004A3A48, 0, 1, str_Save);
    cIntMenuItem_cIntMenuItem((char*)self + 0x2B8, 7, &D_004A3A4C, 0, 1, str_Load);
    cFloatMenuItem_ctor((char*)self + 0x2E8, 8, &D_004A3A50, 0.0f, 120.0f, str_Min_Impact_Velocity);
    cIntMenuItem_cIntMenuItem((char*)self + 0x31C, 9, &D_004A3A54, 0, 1, str_Render_Emitters);
    cIntMenuItem_cIntMenuItem((char*)self + 0x34C, 10, &D_004A3A58, 0, 0x3F, str_Render_N_Lines);
    cIntMenuItem_cIntMenuItem((char*)self + 0x37C, 11, &cAvalanche_m_Trigger, -1, 1, str_Trigger_Avalanche);
    cIntMenuItem_cIntMenuItem((char*)self + 0x3AC, 0xC, &cAvalanche_m_Update, -1, 1, str_Update);
    cFloatMenuItem_ctor((char*)self + 0x3DC, 13, &cAvalanche_m_UpdateTimeStep, 0.0f, 1.0f, str_Update_Time_Step);
    cFloatMenuItem_ctor((char*)self + 0x410, 0xE, &cAvalanche_m_RotationalVelocityScale, 0.0f, 1000.0f, str_Rotational_Velocity_Scale);
    cFloatMenuItem_ctor((char*)self + 0x444, 0xF, &cAvalanche_m_RandomBounceChance, 0.0f, 1.0f, str_Random_Bounce_Chance);
    cFloatMenuItem_ctor((char*)self + 0x478, 0x10, &cAvalanche_m_RandomBounceVelocityScaleXY, 0.0f, 1000.0f, str_Random_Bounce_Velocity_Scale_XY);
    cFloatMenuItem_ctor((char*)self + 0x4AC, 17, &cAvalanche_m_RandomBounceVelocityScaleZ, 0.0f, 1000.0f, str_Random_Bounce_Velocity_Scale_Z);
    cFloatMenuItem_ctor((char*)self + 0x4E0, 18, &cAvalanche_m_RandomBounceVelocityScale, 1.0f, 1000.0f, str_Random_Bounce_Velocity_Scale);
    cIntMenuItem_cIntMenuItem((char*)self + 0x514, 19, &cAvalanche_m_AvalancheId, 0, 1000, str_Avalanche_Id);
    cFloatMenuItem_ctor((char*)self + 0x544, 20, &cAvalanche_m_Gravity, 0.0f, 127.0f, str_Gravity);
    cFloatMenuItem_ctor((char*)self + 0x578, 21, &D_004A3A84, 0.0f, 1.0f, str_Velocity_Damping);
    cFloatMenuItem_ctor((char*)self + 0x5AC, 22, &cAvalanche_m_NormalVelocityScale, 0.0f, 1.0f, str_Normal_Velocity_Scale);
    cFloatMenuItem_ctor((char*)self + 0x5E0, 23, &D_004A3A8C, 0.0f, 1.0f, str_Forward_Velocity_Scale);
    cFloatMenuItem_ctor((char*)self + 0x614, 0x18, &D_004A3A90, 0.0f, 1.0f, str_Normal_To_Forward_Scale);
    cFloatMenuItem_ctor((char*)self + 0x648, 25, &D_004A3A94, 1.0f, 119.0f, str_Max_Velocity);
    cFloatMenuItem_ctor((char*)self + 0x67C, 26, &D_004A3A98, 0.0f, 1.0f, str_Damping);
    cIntMenuItem_cIntMenuItem((char*)self + 0x6B0, 27, &D_004A3A9C, 1, 0x400, str_Num_Particles);
    cFloatMenuItem_ctor((char*)self + 0x6E0, 0x1C, &D_004A3AA0, 1.0f, 1000.0f, str_Default_Size);
    cFloatMenuItem_ctor((char*)self + 0x714, 29, &D_004A3AA4, 0.0f, 10.0f, str_Patch_Colour_Scale);
    cFloatMenuItem_ctor((char*)self + 0x748, 0x1E, &D_004A3AA8, 0.0f, 10.0f, str_Patch_Colour_Scale_2);
    cFloatMenuItem_ctor((char*)self + 0x77C, 0x1F, &D_004A3AAC, -1.0f, 1000.0f, str_Seed_b);
    cMenu_addItem(self, (char*)self + 0x130, -1);
    cMenu_addItem(self, (char*)self + 0x14C, -1);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C4, -1);
    cMenu_addItem(self, (char*)self + 0x1F8, -1);
    cMenu_addItem(self, (char*)self + 0x228, -1);
    cMenu_addItem(self, (char*)self + 0x258, -1);
    cMenu_addItem(self, (char*)self + 0x288, -1);
    cMenu_addItem(self, (char*)self + 0x2B8, -1);
    cMenu_addItem(self, (char*)self + 0x2E8, -1);
    cMenu_addItem(self, (char*)self + 0x31C, -1);
    cMenu_addItem(self, (char*)self + 0x34C, -1);
    cMenu_addItem(self, (char*)self + 0x37C, -1);
    cMenu_addItem(self, (char*)self + 0x3AC, -1);
    cMenu_addItem(self, (char*)self + 0x3DC, -1);
    cMenu_addItem(self, (char*)self + 0x410, -1);
    cMenu_addItem(self, (char*)self + 0x444, -1);
    cMenu_addItem(self, (char*)self + 0x478, -1);
    cMenu_addItem(self, (char*)self + 0x4AC, -1);
    cMenu_addItem(self, (char*)self + 0x4E0, -1);
    cMenu_addItem(self, (char*)self + 0x514, -1);
    cMenu_addItem(self, (char*)self + 0x544, -1);
    cMenu_addItem(self, (char*)self + 0x578, -1);
    cMenu_addItem(self, (char*)self + 0x5AC, -1);
    cMenu_addItem(self, (char*)self + 0x5E0, -1);
    cMenu_addItem(self, (char*)self + 0x614, -1);
    cMenu_addItem(self, (char*)self + 0x648, -1);
    cMenu_addItem(self, (char*)self + 0x67C, -1);
    cMenu_addItem(self, (char*)self + 0x6B0, -1);
    cMenu_addItem(self, (char*)self + 0x6E0, -1);
    cMenu_addItem(self, (char*)self + 0x714, -1);
    cMenu_addItem(self, (char*)self + 0x748, -1);
    cMenu_addItem(self, (char*)self + 0x77C, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cAIVisualEffectsMainMenu_cAIVisualEffectsMainMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* cAIVisualEffectsMainMenu_vtbl[];
extern char str_AIVisual_Effects_Main_Menu[];
extern char D_004D1C88[];
extern char str_Avalanche_Effects[];

/**
 * Builds the "AIVisual Effects Main Menu" debug menu, whose only entry opens the avalanche effects menu.
 *
 * @return @p self
 */
extern "C" void* cAIVisualEffectsMainMenu_cAIVisualEffectsMainMenu(void* self)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = cAIVisualEffectsMainMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x130), str_AIVisual_Effects_Main_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x14C), (void*)40);
    cSubMenuItem_cSubMenuItem((char*)self + 0x164, D_004D1C88, str_Avalanche_Effects);
    cMenu_addItem(self, (char*)self + 0x164, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cAIVisualEffectsMainMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "AIVisual Effects Main Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * Only @p self is declared; the cMenuSys pointer the caller passes second reaches cMenu_onGainFocus() unchanged
 * in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cAIVisualEffectsMainMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cAIVisualEffectsMainMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "AIVisual Effects Main Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cAIVisualEffectsMainMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", func_00250AA0);
#ifdef SKIP_ASM
/**
 * Does nothing and returns its first argument: an empty variadic function, the same body as func_0024D6D0.
 *
 * It only spills the remaining integer argument registers and the float argument registers to its frame before
 * returning.
 *
 * @return @p self
 */
extern "C" void* func_00250AA0(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cAvalancheMenu__cAvalancheMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "Avalanche Menu" debug menu: its 34 embedded menu items in reverse order, then the cMenu base.
 *
 * Each item goes through the base cMenuItem destructor with flags 2, which only resets the item's vtable;
 * cMenu__cMenu() then frees the object when bit 0 of @p flags is set.
 *
 * The "AIVisual Effects Main Menu" lists it as "Avalanche Effects". __sti__aivisualeffectsmenu_cpp calls it with
 * flags 2 to destroy the static instance at exit.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cAvalancheMenu__cAvalancheMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x77C, 2);
    cMenuItem__cMenuItem((char*)self + 0x748, 2);
    cMenuItem__cMenuItem((char*)self + 0x714, 2);
    cMenuItem__cMenuItem((char*)self + 0x6E0, 2);
    cMenuItem__cMenuItem((char*)self + 0x6B0, 2);
    cMenuItem__cMenuItem((char*)self + 0x67C, 2);
    cMenuItem__cMenuItem((char*)self + 0x648, 2);
    cMenuItem__cMenuItem((char*)self + 0x614, 2);
    cMenuItem__cMenuItem((char*)self + 0x5E0, 2);
    cMenuItem__cMenuItem((char*)self + 0x5AC, 2);
    cMenuItem__cMenuItem((char*)self + 0x578, 2);
    cMenuItem__cMenuItem((char*)self + 0x544, 2);
    cMenuItem__cMenuItem((char*)self + 0x514, 2);
    cMenuItem__cMenuItem((char*)self + 0x4E0, 2);
    cMenuItem__cMenuItem((char*)self + 0x4AC, 2);
    cMenuItem__cMenuItem((char*)self + 0x478, 2);
    cMenuItem__cMenuItem((char*)self + 0x444, 2);
    cMenuItem__cMenuItem((char*)self + 0x410, 2);
    cMenuItem__cMenuItem((char*)self + 0x3DC, 2);
    cMenuItem__cMenuItem((char*)self + 0x3AC, 2);
    cMenuItem__cMenuItem((char*)self + 0x37C, 2);
    cMenuItem__cMenuItem((char*)self + 0x34C, 2);
    cMenuItem__cMenuItem((char*)self + 0x31C, 2);
    cMenuItem__cMenuItem((char*)self + 0x2E8, 2);
    cMenuItem__cMenuItem((char*)self + 0x2B8, 2);
    cMenuItem__cMenuItem((char*)self + 0x288, 2);
    cMenuItem__cMenuItem((char*)self + 0x258, 2);
    cMenuItem__cMenuItem((char*)self + 0x228, 2);
    cMenuItem__cMenuItem((char*)self + 0x1F8, 2);
    cMenuItem__cMenuItem((char*)self + 0x1C4, 2);
    cMenuItem__cMenuItem((char*)self + 0x194, 2);
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", __sti__aivisualeffectsmenu_cpp);
#ifdef SKIP_ASM
inline void* operator new(unsigned int, void* p) { return p; }

struct sV4_0CB0 {
    float x, y, z, w;
    sV4_0CB0(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sSlot_0CB0 {
    unsigned a : 8;
    unsigned b : 24;
    int c;
    sSlot_0CB0(int n) { a = n; b = n; }
};

extern "C" void* cAvalancheMenu_cAvalancheMenu(void* self);
extern "C" void cAvalancheMenu__cAvalancheMenu(void* self, int flags);

extern char D_004D1908[], D_004D1918[], D_004D1928[], D_004D1938[], D_004D1948[], D_004D1958[], D_004D1968[], D_004D1978[], D_004D1988[], D_004D1998[], D_004D19A8[], D_004D19B8[], D_004D19C8[], D_004D19D8[], D_004D19E8[], D_004D19F8[], D_004D1A08[], D_004D1A18[], D_004D1A28[], D_004D1A38[], D_004D1A48[], D_004D1A58[], D_004D1A68[], D_004D1A78[], D_004D1A88[], D_004D1A98[], D_004D1AA8[], D_004D1AB8[], D_004D1AC8[], D_004D1AD8[], D_004D1AE8[], D_004D1AF8[], D_004D1B08[], D_004D1B18[], D_004D1B28[], D_004D1B38[], D_004D1B48[], D_004D1B58[], D_004D1B68[], D_004D1B78[], D_004D1B88[], D_004D1B98[], D_004D1BA8[], D_004D1BB8[], D_004D1BC8[], D_004D1BD8[], D_004D1BE8[], D_004D1BF8[], D_004D1C08[], D_004D1C18[], D_004D1C28[], D_004D1C38[], D_004D1C48[], D_004D1C58[], D_004D1C68[], D_004D1C78[];
extern char D_004D1C88[];
extern int D_004A5060;
extern int D_004A5064;
extern sSlot_0CB0 D_004A5068, D_004A5070, D_004A5078, D_004A5080, D_004A5088, D_004A5090, D_004A5098, D_004A50A0, D_004A50A8, D_004A50B0, D_004A50B8, D_004A50C0, D_004A50C8, D_004A50D0;

// PORT: the unit declares __sti__aivisualeffectsmenu_cpp as void*(int, int); it returns nothing.
extern "C" void __sti__aivisualeffectsmenu_cpp_impl(int init, int prio) __asm__("__sti__aivisualeffectsmenu_cpp");
/**
 * Constructs or destroys the AI visual-effects debug menu's static objects (the g++ static initialisation and
 * destruction routine of aivisualeffectsmenu.cpp).
 *
 * Acts only for the default priority. With @p init set it builds the same 56 four-float constants as
 * __sti__visualeffectsmenu_cpp (its own copies at D_004D1908..D_004D1C78), zeroes D_004A5060 and D_004A5064,
 * numbers the 14 key objects D_004A5068..D_004A50D0 from 0 to 13 and constructs the static "Avalanche Menu" at
 * D_004D1C88. With @p init clear it destroys that menu with flags 2 (no free).
 *
 * @param init  nonzero to construct the static objects, 0 to destroy them
 * @param prio  initialisation priority; nothing happens unless it is 0xFFFF (the default)
 */
extern "C" void __sti__aivisualeffectsmenu_cpp_impl(int init, int prio)
{
    if (prio == 0xFFFF) {
        if (init != 0) {
            new (D_004D1908) sV4_0CB0(1.0f, 1.0f, 1.0f, 0.5f);
            new (D_004D1918) sV4_0CB0(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
            new (D_004D1928) sV4_0CB0(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
            new (D_004D1938) sV4_0CB0(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
            new (D_004D1948) sV4_0CB0(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
            new (D_004D1958) sV4_0CB0(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
            new (D_004D1968) sV4_0CB0(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
            new (D_004D1978) sV4_0CB0(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
            new (D_004D1988) sV4_0CB0(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
            new (D_004D1998) sV4_0CB0(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
            new (D_004D19A8) sV4_0CB0(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004D19B8) sV4_0CB0(1.0f, 0.0f, 0.0f, 0.0f);
            new (D_004D19C8) sV4_0CB0(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
            new (D_004D19D8) sV4_0CB0(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004D19E8) sV4_0CB0(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004D19F8) sV4_0CB0(1.0f, 0.0f, 0.0f, 0.0f);
            new (D_004D1A08) sV4_0CB0(1.0f, 0.0f, 0.0f, 0.0f);
            new (D_004D1A18) sV4_0CB0(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004D1A28) sV4_0CB0(1.0f, 0.5f, 1.0f, 0.5f);
            new (D_004D1A38) sV4_0CB0(1.0f, 0.5f, 1.0f, 0.5f);
            new (D_004D1A48) sV4_0CB0(1.0f, 0.5f, 1.0f, 0.5f);
            new (D_004D1A58) sV4_0CB0(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
            new (D_004D1A68) sV4_0CB0(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
            new (D_004D1A78) sV4_0CB0(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
            new (D_004D1A88) sV4_0CB0(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004D1A98) sV4_0CB0(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
            new (D_004D1AA8) sV4_0CB0(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
            new (D_004D1AB8) sV4_0CB0(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
            new (D_004D1AC8) sV4_0CB0(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
            new (D_004D1AD8) sV4_0CB0(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
            new (D_004D1AE8) sV4_0CB0(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004D1AF8) sV4_0CB0(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004D1B08) sV4_0CB0(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
            new (D_004D1B18) sV4_0CB0(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004D1B28) sV4_0CB0(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
            new (D_004D1B38) sV4_0CB0(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004D1B48) sV4_0CB0(1.0f, 1.0f, 0.0f, 0.0f);
            new (D_004D1B58) sV4_0CB0(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
            new (D_004D1B68) sV4_0CB0(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
            new (D_004D1B78) sV4_0CB0(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
            new (D_004D1B88) sV4_0CB0(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
            new (D_004D1B98) sV4_0CB0(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
            new (D_004D1BA8) sV4_0CB0(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004D1BB8) sV4_0CB0(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
            new (D_004D1BC8) sV4_0CB0(1.0f, 1.0f, 0.0f, 0.0f);
            new (D_004D1BD8) sV4_0CB0(1.0f, 1.0f, 0.0f, 0.0f);
            new (D_004D1BE8) sV4_0CB0(1.0f, 1.0f, 0.0f, 0.0f);
            new (D_004D1BF8) sV4_0CB0(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
            new (D_004D1C08) sV4_0CB0(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
            new (D_004D1C18) sV4_0CB0(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
            new (D_004D1C28) sV4_0CB0(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
            new (D_004D1C38) sV4_0CB0(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
            new (D_004D1C48) sV4_0CB0(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
            new (D_004D1C58) sV4_0CB0(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
            new (D_004D1C68) sV4_0CB0(1.0f, 0.0f, 1.0f, 0.0f);
            new (D_004D1C78) sV4_0CB0(1.0f, 1.0f, 0.0f, 0.0f);
            D_004A5064 = 0;
            D_004A5060 = 0;
            new (&D_004A5068) sSlot_0CB0(0);
            new (&D_004A5070) sSlot_0CB0(1);
            new (&D_004A5078) sSlot_0CB0(2);
            new (&D_004A5080) sSlot_0CB0(3);
            new (&D_004A5088) sSlot_0CB0(4);
            new (&D_004A5090) sSlot_0CB0(5);
            new (&D_004A5098) sSlot_0CB0(6);
            new (&D_004A50A0) sSlot_0CB0(7);
            new (&D_004A50A8) sSlot_0CB0(8);
            new (&D_004A50B0) sSlot_0CB0(9);
            new (&D_004A50B8) sSlot_0CB0(10);
            new (&D_004A50C0) sSlot_0CB0(11);
            new (&D_004A50C8) sSlot_0CB0(12);
            new (&D_004A50D0) sSlot_0CB0(13);
            cAvalancheMenu_cAvalancheMenu(D_004D1C88);
        } else {
            cAvalancheMenu__cAvalancheMenu(D_004D1C88, 2);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cAvalancheMenu_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the "Avalanche Menu" debug menu focus through the base cMenu_onGainFocus().
 *
 * The "AIVisual Effects Main Menu" debug menu lists it as "Avalanche Effects". Only @p self is declared; the
 * cMenuSys pointer the caller passes second reaches cMenu_onGainFocus() unchanged in its register.
 *
 * @return no meaningful value (cMenu_onGainFocus() returns nothing)
 */
void* cAvalancheMenu_onGainFocus(void* self)
{
    return cMenu_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cAvalancheMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Takes focus away from the "Avalanche Menu" debug menu through the base cMenu_onLoseFocus().
 *
 * @return no meaningful value (cMenu_onLoseFocus() returns nothing)
 */
void* cAvalancheMenu_onLoseFocus(void* self)
{
    return cMenu_onLoseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", cAIVisualEffectsMainMenu__cAIVisualEffectsMainMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(void* p, int flags);
extern "C" void cMenu__cMenu(void* self, int flags);

/**
 * Destroys the "AIVisual Effects Main Menu" debug menu: its 3 embedded menu items in reverse order, then the
 * cMenu base.
 *
 * The items are the title, the spacer and the sub-menu link to the "Avalanche Menu". Each item goes through the
 * base cMenuItem destructor with flags 2, which only resets the item's vtable; cMenu__cMenu() then frees the
 * object when bit 0 of @p flags is set.
 *
 * No direct call is known: it is reached through the destructor slot of the menu's vtable.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cAIVisualEffectsMainMenu__cAIVisualEffectsMainMenu(void* self, int flags)
{
    cMenuItem__cMenuItem((char*)self + 0x164, 2);
    cMenuItem__cMenuItem((char*)self + 0x14C, 2);
    cMenuItem__cMenuItem((char*)self + 0x130, 2);
    cMenu__cMenu(self, flags);
}
#endif

extern "C" void* __sti__aivisualeffectsmenu_cpp(int, int);

//99.38%
INCLUDE_ASM("main/debugmenu", func_00251690__FPv);
#ifdef SKIP_ASM
/**
 * Runs the AI visual effects menu unit's static constructors: calls __sti__aivisualeffectsmenu_cpp(1, 0xFFFF).
 *
 * It is entry +0x34 of the global constructor table D_0043CE38; func_002516B0 is its destructor twin. The declared
 * parameter is not used.
 *
 * @return __sti__aivisualeffectsmenu_cpp's result
 */
void* func_00251690(void* self)
{
    return __sti__aivisualeffectsmenu_cpp(1, 0xffff);
}
#endif

extern "C" void* __sti__aivisualeffectsmenu_cpp(int, int);

//99.38%
INCLUDE_ASM("main/debugmenu", func_002516B0__FPv);
#ifdef SKIP_ASM
/**
 * Runs the AI visual effects menu unit's static destructors: calls __sti__aivisualeffectsmenu_cpp(0, 0xFFFF).
 *
 * It is entry +0x18 of the global destructor table D_0043CEE4; func_00251690 is its constructor twin. The declared
 * parameter is not used.
 *
 * @return __sti__aivisualeffectsmenu_cpp's result
 */
void* func_002516B0(void* self)
{
    return __sti__aivisualeffectsmenu_cpp(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("main/debugmenu", MEM_initblock);
#ifdef SKIP_ASM
/**
 * Writes a memory-manager block header: the "MB" magic (0x424D), flags, size and the next and previous
 * blocks.
 *
 * Belongs to the memory manager (its only callers are in mem/memstd); the name and header-size arguments are
 * not
 * stored.
 *
 * @param name  the heap name (unused)
 * @param size  block size in bytes, excluding the 16-byte header
 * @param infoSize  the heap's extra header size (unused)
 * @param flags  block flags
 * @param prev  the previous block
 * @param next  the next block
 * @return @p size + 16, the block's size with its header (declared as a pointer)
 */
extern "C" void* MEM_initblock(void* self, int name, void* size, int infoSize, short flags, int prev, int next)
{
    *(short*)self = 16973;
    *(short*)((char*)self + 0x2) = flags;
    *(int*)((char*)self + 0x4) = (int)size;
    *(int*)((char*)self + 0x8) = next;
    *(int*)((char*)self + 0xc) = prev;
    return (char*)size + 0x10;
}
#endif


//100%
INCLUDE_ASM("main/debugmenu", func_002516F0);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int val, int n);
extern char D_004D2438[];

/**
 * Clears the 256-byte buffer D_004D2438 with MEM_fill.
 */
extern "C" void func_002516F0(void)
{
    MEM_fill(D_004D2438, 0, 0x100);
}
#endif
