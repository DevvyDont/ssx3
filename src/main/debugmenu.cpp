#include "common.h"
#include "main/debugmenu.h"

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
extern int cRenderToggles_m_AIUpdatesPerRender;
extern int cRenderToggles_m_DisplayRenderStats;
extern int cRenderToggles_m_ResetStats;
extern int cRenderToggles_m_UseSmallTexture0;
extern int cRenderToggles_m_UseSmallTexture1;
extern int cRenderToggles_m_DisablePatches;
extern int cRenderToggles_m_DisablePatchReflection;
extern int cRenderToggles_m_DisablePatchShadow;
extern int cRenderToggles_m_DisableSparkles;
extern int cRenderToggles_m_DisableLineStrips;
extern int cRenderToggles_m_DisableTriStrips;
extern int cRenderToggles_m_DisableTransformStrips;
extern int cRenderToggles_m_DisableMeshes;
extern int cRenderToggles_m_DisableMeshReflection;
extern int cRenderToggles_m_DisableStaticEmitters;
extern int cRenderToggles_m_DisableDynamicEmitters;
extern int cRenderToggles_m_DisableDynamicColourEmitters;
extern int cRenderToggles_m_DisableSprites;
extern int cRenderToggles_m_Disable3DSprites;
extern int cRenderToggles_m_DisableTextureRects;
extern int cRenderToggles_m_DisablePlayerStrips;
extern int cRenderToggles_m_DisableShadows;
extern int cRenderToggles_m_DisableBoardTrail;
extern int cRenderToggles_m_DisableAIBoardTrail;
extern int cRenderToggles_m_DisableVisibilityRects;
extern int cRenderToggles_m_DisableSnowFall;
extern int cRenderToggles_m_DisableBoardWake;
extern int cRenderToggles_m_DisableSkyBox;
extern int cRenderToggles_m_DisableCharacterSpecular;
extern int cRenderToggles_m_DisableAIVisualFX;
extern int cRenderToggles_m_DisableBoundingBoxes;
extern int cRenderToggles_m_UseReflectionVector;
extern int cRenderToggles_m_DisableHUD;
extern int cRenderToggles_m_TestTunnel;
extern float cRenderToggles_m_ParticleSizeScale;
extern int cRenderToggles_m_TestSurfaceTypes;
extern int cRenderToggles_m_SoftwareDynamicParticleSystems;
extern int cRenderToggles_m_TempInt;
extern float cRenderToggles_m_Prog2x;
extern float cRenderToggles_m_Prog2y;
extern float cRenderToggles_m_Prog2z;
extern float cRenderToggles_m_Prog2w;
extern int cRenderToggles_m_DebugInfo;
extern int cRenderToggles_m_DebugNISMemInfo;
extern int cRenderToggles_m_DebugGameModeInfo;
extern int cRenderToggles_m_ProfileIndividualFX;
extern int cRenderToggles_m_ShowSurfaceTypes;

/**
 * Builds the "Render Toggles Menu" debug menu: a title, a spacer and 47 int and float items bound to render
 * debug globals: AI frames per render frame, render stats, one "Disable ..." toggle per render pass, test
 * values and the build, NIS-memory and game-flow info overlays.
 *
 * @return @p self
 */
extern "C" void* cRenderTogglesMenu_cRenderTogglesMenu(cRenderTogglesMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cRenderTogglesMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Render_Toggles_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->AIUpdatesPerRenderItem, 0, &cRenderToggles_m_AIUpdatesPerRender, 0, 0x3c, str_AI_Frames_per_Render_Frame);
    cIntMenuItem_cIntMenuItem(&self->DisplayRenderStatsItem, 1, &cRenderToggles_m_DisplayRenderStats, 0, 1, str_Display_Render_Stats);
    cIntMenuItem_cIntMenuItem(&self->ResetStatsItem, 2, &cRenderToggles_m_ResetStats, 0, 1, str_Reset_Stats);
    cIntMenuItem_cIntMenuItem(&self->UseSmallTexture0Item, 3, &cRenderToggles_m_UseSmallTexture0, 0, 1, str_Use_Small_Texture_0);
    cIntMenuItem_cIntMenuItem(&self->UseSmallTexture1Item, 4, &cRenderToggles_m_UseSmallTexture1, 0, 1, str_Use_Small_Texture_1);
    cIntMenuItem_cIntMenuItem(&self->DisablePatchesItem, 5, &cRenderToggles_m_DisablePatches, 0, 1, str_Disable_Patches);
    cIntMenuItem_cIntMenuItem(&self->DisablePatchReflectionItem, 6, &cRenderToggles_m_DisablePatchReflection, 0, 1, str_Disable_Patch_Reflection);
    cIntMenuItem_cIntMenuItem(&self->DisablePatchShadowItem, 7, &cRenderToggles_m_DisablePatchShadow, 0, 1, str_Disable_Patch_Shadows);
    cIntMenuItem_cIntMenuItem(&self->DisableSparklesItem, 8, &cRenderToggles_m_DisableSparkles, 0, 1, str_Disable_Sparkles);
    cIntMenuItem_cIntMenuItem(&self->DisableLineStripsItem, 9, &cRenderToggles_m_DisableLineStrips, 0, 1, str_Disable_Line_Strips);
    cIntMenuItem_cIntMenuItem(&self->DisableTriStripsItem, 10, &cRenderToggles_m_DisableTriStrips, 0, 3, str_Disable_Tri_Strips);
    cIntMenuItem_cIntMenuItem(&self->DisableTransformStripsItem, 11, &cRenderToggles_m_DisableTransformStrips, 0, 1, str_Disable_Transform_Strips);
    cIntMenuItem_cIntMenuItem(&self->DisableMeshesItem, 0xc, &cRenderToggles_m_DisableMeshes, 0, 1, str_Disable_Meshes);
    cIntMenuItem_cIntMenuItem(&self->DisableMeshReflectionItem, 13, &cRenderToggles_m_DisableMeshReflection, 0, 1, str_Disable_Mesh_Reflection);
    cIntMenuItem_cIntMenuItem(&self->DisableStaticEmittersItem, 0xe, &cRenderToggles_m_DisableStaticEmitters, 0, 1, str_Disable_Static_Emitters);
    cIntMenuItem_cIntMenuItem(&self->DisableDynamicEmittersItem, 0xf, &cRenderToggles_m_DisableDynamicEmitters, 0, 1, str_Disable_Dynamic_Emitters);
    cIntMenuItem_cIntMenuItem(&self->DisableDynamicColourEmittersItem, 0x10, &cRenderToggles_m_DisableDynamicColourEmitters, 0, 1, str_Disable_Dynamic_Colour_Emitters);
    cIntMenuItem_cIntMenuItem(&self->DisableSpritesItem, 17, &cRenderToggles_m_DisableSprites, 0, 1, str_Disable_Sprites);
    cIntMenuItem_cIntMenuItem(&self->Disable3DSpritesItem, 18, &cRenderToggles_m_Disable3DSprites, 0, 1, str_Disable_3D_Sprites);
    cIntMenuItem_cIntMenuItem(&self->DisableTextureRectsItem, 19, &cRenderToggles_m_DisableTextureRects, 0, 1, str_Disable_Texture_Rects);
    cIntMenuItem_cIntMenuItem(&self->DisablePlayerStripsItem, 20, &cRenderToggles_m_DisablePlayerStrips, 0, 1, str_Disable_Player_Strips);
    cIntMenuItem_cIntMenuItem(&self->DisableShadowsItem, 21, &cRenderToggles_m_DisableShadows, 0, 1, str_Disable_Shadows);
    cIntMenuItem_cIntMenuItem(&self->DisableBoardTrailItem, 22, &cRenderToggles_m_DisableBoardTrail, 0, 1, str_Disable_Board_Trail);
    cIntMenuItem_cIntMenuItem(&self->DisableAIBoardTrailItem, 23, &cRenderToggles_m_DisableAIBoardTrail, 0, 1, str_Disable_AI_Board_Trail);
    cIntMenuItem_cIntMenuItem(&self->DisableVisibilityRectsItem, 0x18, &cRenderToggles_m_DisableVisibilityRects, 0, 1, str_Disable_Visibility_Rects);
    cIntMenuItem_cIntMenuItem(&self->DisableSnowFallItem, 25, &cRenderToggles_m_DisableSnowFall, 0, 1, str_Disable_Snowfall);
    cIntMenuItem_cIntMenuItem(&self->DisableBoardWakeItem, 26, &cRenderToggles_m_DisableBoardWake, 0, 1, str_Disable_Board_Wake);
    cIntMenuItem_cIntMenuItem(&self->DisableSkyBoxItem, 27, &cRenderToggles_m_DisableSkyBox, 0, 1, str_Disable_Sky_Box);
    cIntMenuItem_cIntMenuItem(&self->DisableCharacterSpecularItem, 0x1c, &cRenderToggles_m_DisableCharacterSpecular, 0, 1, str_Disable_Character_Specular);
    cIntMenuItem_cIntMenuItem(&self->DisableAIVisualFXItem, 29, &cRenderToggles_m_DisableAIVisualFX, 0, 1, str_Disable_AI_Visual_FX);
    cIntMenuItem_cIntMenuItem(&self->DisableBoundingBoxesItem, 0x1e, &cRenderToggles_m_DisableBoundingBoxes, 0, 1, str_Disable_Bounding_Boxes);
    cIntMenuItem_cIntMenuItem(&self->UseReflectionVectorItem, 0x1f, &cRenderToggles_m_UseReflectionVector, 0, 1, str_Use_Reflection_Vector_for_Sphere_Map);
    cIntMenuItem_cIntMenuItem(&self->DisableHUDItem, 0x20, &cRenderToggles_m_DisableHUD, 0, 1, str_Disable_HUD_elements);
    cIntMenuItem_cIntMenuItem(&self->TestTunnelItem, 33, &cRenderToggles_m_TestTunnel, 0, 1, str_Test_Tunnel);
    cFloatMenuItem_ctor(&self->ParticleSizeScaleItem, 34, &cRenderToggles_m_ParticleSizeScale, 1.0f, 2000.0f, str_Particle_Size_Scale);
    cIntMenuItem_cIntMenuItem(&self->TestSurfaceTypesItem, 35, &cRenderToggles_m_TestSurfaceTypes, 0, 1, str_Test_Surface_Types);
    cIntMenuItem_cIntMenuItem(&self->SoftwareDynamicParticleSystemsItem, 36, &cRenderToggles_m_SoftwareDynamicParticleSystems, 0, 1, str_Software_Based_Dynamic_Systems);
    cIntMenuItem_cIntMenuItem(&self->TempIntItem, 37, &cRenderToggles_m_TempInt, 0, 20, str_Temp_Int);
    cFloatMenuItem_ctor(&self->Prog2xItem, 38, &cRenderToggles_m_Prog2x, -10.0f, 10.0f, str_PROG2_x);
    cFloatMenuItem_ctor(&self->Prog2yItem, 39, &cRenderToggles_m_Prog2y, -10.0f, 10.0f, str_PROG2_y);
    cFloatMenuItem_ctor(&self->Prog2zItem, 40, &cRenderToggles_m_Prog2z, -10.0f, 10.0f, str_PROG2_z);
    cFloatMenuItem_ctor(&self->Prog2wItem, 41, &cRenderToggles_m_Prog2w, -10.0f, 10.0f, str_PROG2_w);
    cIntMenuItem_cIntMenuItem(&self->DebugInfoItem, 42, &cRenderToggles_m_DebugInfo, 0, 1, str_Enable_Build_Info);
    cIntMenuItem_cIntMenuItem(&self->DebugNISMemInfoItem, 43, &cRenderToggles_m_DebugNISMemInfo, 0, 1, str_Enable_NIS_Mem_Usage_Info);
    cIntMenuItem_cIntMenuItem(&self->DebugGameModeInfoItem, 44, &cRenderToggles_m_DebugGameModeInfo, 0, 1, str_Enable_Gameflow_Info);
    cIntMenuItem_cIntMenuItem(&self->ProfileIndividualFXItem, 45, &cRenderToggles_m_ProfileIndividualFX, 0, 1, str_Profile_Individual_Effects);
    cIntMenuItem_cIntMenuItem(&self->ShowSurfaceTypesItem, 46, &cRenderToggles_m_ShowSurfaceTypes, 0, 1, str_Show_Surface_Types);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->AIUpdatesPerRenderItem, -1);
    cMenu_addItem(self, &self->DisplayRenderStatsItem, -1);
    cMenu_addItem(self, &self->ResetStatsItem, -1);
    cMenu_addItem(self, &self->UseSmallTexture0Item, -1);
    cMenu_addItem(self, &self->UseSmallTexture1Item, -1);
    cMenu_addItem(self, &self->DisablePatchesItem, -1);
    cMenu_addItem(self, &self->DisablePatchReflectionItem, -1);
    cMenu_addItem(self, &self->DisablePatchShadowItem, -1);
    cMenu_addItem(self, &self->DisableSparklesItem, -1);
    cMenu_addItem(self, &self->DisableLineStripsItem, -1);
    cMenu_addItem(self, &self->DisableTriStripsItem, -1);
    cMenu_addItem(self, &self->DisableTransformStripsItem, -1);
    cMenu_addItem(self, &self->DisableMeshesItem, -1);
    cMenu_addItem(self, &self->DisableMeshReflectionItem, -1);
    cMenu_addItem(self, &self->DisableStaticEmittersItem, -1);
    cMenu_addItem(self, &self->DisableDynamicEmittersItem, -1);
    cMenu_addItem(self, &self->DisableDynamicColourEmittersItem, -1);
    cMenu_addItem(self, &self->DisableSpritesItem, -1);
    cMenu_addItem(self, &self->Disable3DSpritesItem, -1);
    cMenu_addItem(self, &self->DisableTextureRectsItem, -1);
    cMenu_addItem(self, &self->DisablePlayerStripsItem, -1);
    cMenu_addItem(self, &self->DisableShadowsItem, -1);
    cMenu_addItem(self, &self->DisableBoardTrailItem, -1);
    cMenu_addItem(self, &self->DisableAIBoardTrailItem, -1);
    cMenu_addItem(self, &self->DisableVisibilityRectsItem, -1);
    cMenu_addItem(self, &self->DisableSnowFallItem, -1);
    cMenu_addItem(self, &self->DisableBoardWakeItem, -1);
    cMenu_addItem(self, &self->DisableSkyBoxItem, -1);
    cMenu_addItem(self, &self->DisableCharacterSpecularItem, -1);
    cMenu_addItem(self, &self->DisableAIVisualFXItem, -1);
    cMenu_addItem(self, &self->DisableBoundingBoxesItem, -1);
    cMenu_addItem(self, &self->UseReflectionVectorItem, -1);
    cMenu_addItem(self, &self->DisableHUDItem, -1);
    cMenu_addItem(self, &self->TestTunnelItem, -1);
    cMenu_addItem(self, &self->ParticleSizeScaleItem, -1);
    cMenu_addItem(self, &self->TestSurfaceTypesItem, -1);
    cMenu_addItem(self, &self->SoftwareDynamicParticleSystemsItem, -1);
    cMenu_addItem(self, &self->TempIntItem, -1);
    cMenu_addItem(self, &self->Prog2xItem, -1);
    cMenu_addItem(self, &self->Prog2yItem, -1);
    cMenu_addItem(self, &self->Prog2zItem, -1);
    cMenu_addItem(self, &self->Prog2wItem, -1);
    cMenu_addItem(self, &self->DebugInfoItem, -1);
    cMenu_addItem(self, &self->DebugNISMemInfoItem, -1);
    cMenu_addItem(self, &self->DebugGameModeInfoItem, -1);
    cMenu_addItem(self, &self->ProfileIndividualFXItem, -1);
    cMenu_addItem(self, &self->ShowSurfaceTypesItem, -1);
    return ((void*)self);
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
extern int cVisualEffectsTest_m_HUDEnable;
extern int cVisualEffectsTest_m_ShowCurrentElement;
extern int cVisualEffectsTest_m_HUDPrint;
extern int cVisualEffectsTest_m_CurrElement;
extern int cVisualEffectsTest_m_PositionX;
extern int cVisualEffectsTest_m_PositionY;
extern int cVisualEffectsTest_m_SizeX;
extern int cVisualEffectsTest_m_SizeY;
extern int cVisualEffectsTest_m_ScaleX;
extern int cVisualEffectsTest_m_ScaleY;
extern int cVisualEffectsTest_m_Alpha;
extern int cVisualEffectsTest_m_Red;
extern int cVisualEffectsTest_m_Green;
extern int cVisualEffectsTest_m_Blue;
extern int cVisualEffectsTest_m_LayerOffset;
extern int cVisualEffectsTest_m_Enable;
extern int cVisualEffectsTest_m_LayerId;

/**
 * Builds the "Visual Effects Test Menu" debug menu: a title, a spacer and 17 int items for inspecting HUD
 * graphics (show all, current element, output HUD data, its position, size, scale, colour and layer) and the
 * world painter display.
 *
 * @return @p self
 */
extern "C" void* cVisualEffectsTestMenu_cVisualEffectsTestMenu(cVisualEffectsTestMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cVisualEffectsTestMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Visual_Effects_Test_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->HUDEnableItem, 0, &cVisualEffectsTest_m_HUDEnable, 0, 1, str_Show_All_HUD_Graphics);
    cIntMenuItem_cIntMenuItem(&self->ShowCurrentElementItem, 1, &cVisualEffectsTest_m_ShowCurrentElement, 0, 1, str_Show_Current_Element);
    cIntMenuItem_cIntMenuItem(&self->HUDPrintItem, 2, &cVisualEffectsTest_m_HUDPrint, 0, 1, str_Output_HUD_Data);
    cIntMenuItem_cIntMenuItem(&self->CurrElementItem, 3, &cVisualEffectsTest_m_CurrElement, 0, 1000, str_Current_Element_Id);
    cIntMenuItem_cIntMenuItem(&self->PositionXItem, 4, &cVisualEffectsTest_m_PositionX, -640, 640, str_Position_X);
    cIntMenuItem_cIntMenuItem(&self->PositionYItem, 5, &cVisualEffectsTest_m_PositionY, -480, 0x1e0, str_Position_Y);
    cIntMenuItem_cIntMenuItem(&self->SizeXItem, 6, &cVisualEffectsTest_m_SizeX, -640, 640, str_Size_X);
    cIntMenuItem_cIntMenuItem(&self->SizeYItem, 7, &cVisualEffectsTest_m_SizeY, -640, 640, str_Size_Y);
    cIntMenuItem_cIntMenuItem(&self->ScaleXItem, 8, &cVisualEffectsTest_m_ScaleX, -1, 1000, str_Scale_X);
    cIntMenuItem_cIntMenuItem(&self->ScaleYItem, 9, &cVisualEffectsTest_m_ScaleY, -1, 1000, str_Scale_Y);
    cIntMenuItem_cIntMenuItem(&self->AlphaItem, 10, &cVisualEffectsTest_m_Alpha, -255, 0xff, str_Alpha);
    cIntMenuItem_cIntMenuItem(&self->RedItem, 11, &cVisualEffectsTest_m_Red, -255, 0xff, str_Red);
    cIntMenuItem_cIntMenuItem(&self->GreenItem, 0xc, &cVisualEffectsTest_m_Green, -255, 0xff, str_Green);
    cIntMenuItem_cIntMenuItem(&self->BlueItem, 13, &cVisualEffectsTest_m_Blue, -255, 0xff, str_Blue);
    cIntMenuItem_cIntMenuItem(&self->LayerOffsetItem, 0xe, &cVisualEffectsTest_m_LayerOffset, -16, 0x10, str_Layer_Offset);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0xf, &cVisualEffectsTest_m_Enable, 0, 1, str_World_Painter_Display_Enable);
    cIntMenuItem_cIntMenuItem(&self->LayerIdItem, 0x10, &cVisualEffectsTest_m_LayerId, 0, 100, str_World_Painter_LayerId);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->HUDEnableItem, -1);
    cMenu_addItem(self, &self->ShowCurrentElementItem, -1);
    cMenu_addItem(self, &self->HUDPrintItem, -1);
    cMenu_addItem(self, &self->CurrElementItem, -1);
    cMenu_addItem(self, &self->PositionXItem, -1);
    cMenu_addItem(self, &self->PositionYItem, -1);
    cMenu_addItem(self, &self->SizeXItem, -1);
    cMenu_addItem(self, &self->SizeYItem, -1);
    cMenu_addItem(self, &self->ScaleXItem, -1);
    cMenu_addItem(self, &self->ScaleYItem, -1);
    cMenu_addItem(self, &self->AlphaItem, -1);
    cMenu_addItem(self, &self->RedItem, -1);
    cMenu_addItem(self, &self->GreenItem, -1);
    cMenu_addItem(self, &self->BlueItem, -1);
    cMenu_addItem(self, &self->LayerOffsetItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->LayerIdItem, -1);
    return ((void*)self);
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
extern float cPlants_m_SwayAmplitude;
extern float cPlants_m_SwayOffset;
extern float cPlants_m_BranchAmplitude;
extern float cPlants_m_RotationAmplitude;
extern float cPlants_m_PositionScale;
extern int cPlants_m_SwayFrequency;
extern float cPlants_m_RotationFrequency;
extern float cPlants_m_BranchFrequency;
extern float cPlants_m_BranchFrequency2;
extern float cPlants_m_SwayDirection;
extern float cPlants_m_BranchDirection;

/**
 * Builds the "Plants Menu" debug menu: a title, a spacer and an enable switch and 11 items for the plant sway
 * and branch animation (amplitudes, frequencies, directions).
 *
 * @return @p self
 */
extern "C" void* cPlantsMenu_cPlantsMenu(cPlantsMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cPlantsMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Plants_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cPlants_m_Enable, 0, 2, str_Enable);
    cFloatMenuItem_ctor(&self->SwayAmplitudeItem, 1, &cPlants_m_SwayAmplitude, 0.0f, 2.0f, str_Sway_Amplitude);
    cFloatMenuItem_ctor(&self->SwayOffsetItem, 2, &cPlants_m_SwayOffset, -5.0f, 5.0f, str_Sway_Offset);
    cFloatMenuItem_ctor(&self->BranchAmplitudeItem, 3, &cPlants_m_BranchAmplitude, 0.0f, 2.0f, str_Branch_Amplitude);
    cFloatMenuItem_ctor(&self->RotationAmplitudeItem, 4, &cPlants_m_RotationAmplitude, 0.0f, 2.0f, str_Rotation_Amplitude);
    cFloatMenuItem_ctor(&self->PositionScaleItem, 5, &cPlants_m_PositionScale, 0.0f, 1000.0f, str_Position_Scale);
    cIntMenuItem_cIntMenuItem(&self->SwayFrequencyItem, 6, &cPlants_m_SwayFrequency, 0, 6000, str_Sway_Frequency_x_600_Hz);
    cFloatMenuItem_ctor(&self->RotationFrequencyItem, 7, &cPlants_m_RotationFrequency, 0.0f, 6000.0f, str_Rotation_Frequency);
    cFloatMenuItem_ctor(&self->BranchFrequencyItem, 8, &cPlants_m_BranchFrequency, 0.0f, 6000.0f, str_Branch_Frequency_x_600_Hz);
    cFloatMenuItem_ctor(&self->BranchFrequency2Item, 9, &cPlants_m_BranchFrequency2, 0.0f, 6000.0f, str_Branch_Frequency_2_x_600_Hz);
    cFloatMenuItem_ctor(&self->SwayDirectionItem, 10, &cPlants_m_SwayDirection, 0.0f, 360.0f, str_Sway_Direction);
    cFloatMenuItem_ctor(&self->BranchDirectionItem, 11, &cPlants_m_BranchDirection, 0.0f, 360.0f, str_Branch_Direction);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->SwayAmplitudeItem, -1);
    cMenu_addItem(self, &self->SwayOffsetItem, -1);
    cMenu_addItem(self, &self->BranchAmplitudeItem, -1);
    cMenu_addItem(self, &self->RotationAmplitudeItem, -1);
    cMenu_addItem(self, &self->PositionScaleItem, -1);
    cMenu_addItem(self, &self->SwayFrequencyItem, -1);
    cMenu_addItem(self, &self->RotationFrequencyItem, -1);
    cMenu_addItem(self, &self->BranchFrequencyItem, -1);
    cMenu_addItem(self, &self->BranchFrequency2Item, -1);
    cMenu_addItem(self, &self->SwayDirectionItem, -1);
    cMenu_addItem(self, &self->BranchDirectionItem, -1);
    return ((void*)self);
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
extern int cLightGlow_m_PS2CaptureSize;
extern float cLightGlow_m_PS2MinIntensity;
extern float cLightGlow_m_PS2ModIntensity;
extern float cLightGlow_m_PS2CopyIntensity;
extern float cLightGlow_m_PS2FrameSrcScale;
extern float cLightGlow_m_PS2FrameDstScale;
extern float cLightGlow_m_PS2IntensityTex0;
extern float cLightGlow_m_PS2IntensityTex1;
extern float cLightGlow_m_PS2IntensityTex2;
extern float cLightGlow_m_PS2IntensityTex3;
extern float cLightGlow_m_PS2JitterTex;
extern int cLightGlow_m_PS2ShowTextures;
extern float cLightGlow_m_Size;
extern float cLightGlow_m_Cutoff;
extern float cLightGlow_m_ColourPower;
extern float cLightGlow_m_ColourScale;
extern float cLightGlow_m_ColourAmbient;
extern float cLightGlow_m_SizePower;
extern float cLightGlow_m_SizeScale;
extern float cLightGlow_m_SizeAmbient;
extern float cLightGlow_m_FadeRate;
extern int cLightGlow_m_ClearAlpha;
extern float cLightGlow_m_IntensityR;
extern float cLightGlow_m_IntensityG;
extern float cLightGlow_m_IntensityB;
extern float cLightGlow_m_IntensityA;
extern float cLightGlow_m_IntensityScale;

/**
 * Builds the "Light Glow Menu" debug menu: a title, a spacer and 29 items for the light glow effect: enable,
 * world painter override, the PS2 capture and blend settings, and glow size, colour and intensity curves.
 *
 * @return @p self
 */
extern "C" void* cLightGlowMenu_cLightGlowMenu(cLightGlowMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cLightGlowMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Light_Glow_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cLightGlow_m_Enable, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem(&self->OverrideWPItem, 1, &cLightGlow_m_OverrideWP, 0, 1, str_Override_World_Painter);
    cIntMenuItem_cIntMenuItem(&self->PS2CaptureSizeItem, 2, &cLightGlow_m_PS2CaptureSize, 4, 8, str_PS2_Capture_Size_2_x);
    cFloatMenuItem_ctor(&self->PS2MinIntensityItem, 3, &cLightGlow_m_PS2MinIntensity, 0.0f, 2.0f, str_PS2_Minimum_Intensity_Cutoff);
    cFloatMenuItem_ctor(&self->PS2ModIntensityItem, 4, &cLightGlow_m_PS2ModIntensity, 0.0f, 2.0f, str_PS2_Post_Cutoff_Scale);
    cFloatMenuItem_ctor(&self->PS2CopyIntensityItem, 5, &cLightGlow_m_PS2CopyIntensity, 0.0f, 8.0f, str_PS2_Copy_Intensity);
    cFloatMenuItem_ctor(&self->PS2FrameSrcScaleItem, 6, &cLightGlow_m_PS2FrameSrcScale, 0.0f, 2.0f, str_PS2_Frame_Source_Intensity);
    cFloatMenuItem_ctor(&self->PS2FrameDstScaleItem, 7, &cLightGlow_m_PS2FrameDstScale, 0.0f, 2.0f, str_PS2_Frame_Blend_Intensity);
    cFloatMenuItem_ctor(&self->PS2IntensityTex0Item, 8, &cLightGlow_m_PS2IntensityTex0, 0.0f, 2.0f, str_PS2_Blend_Texture_0);
    cFloatMenuItem_ctor(&self->PS2IntensityTex1Item, 9, &cLightGlow_m_PS2IntensityTex1, 0.0f, 2.0f, str_PS2_Blend_Texture_1);
    cFloatMenuItem_ctor(&self->PS2IntensityTex2Item, 10, &cLightGlow_m_PS2IntensityTex2, 0.0f, 2.0f, str_PS2_Blend_Texture_2);
    cFloatMenuItem_ctor(&self->PS2IntensityTex3Item, 11, &cLightGlow_m_PS2IntensityTex3, 0.0f, 2.0f, str_PS2_Blend_Texture_3);
    cFloatMenuItem_ctor(&self->PS2JitterTexItem, 12, &cLightGlow_m_PS2JitterTex, 0.0f, 8.0f, str_PS2_Texture_Jitter);
    cIntMenuItem_cIntMenuItem(&self->PS2ShowTexturesItem, 13, &cLightGlow_m_PS2ShowTextures, 0, 1, str_PS2_Show_Textures);
    cFloatMenuItem_ctor(&self->SizeItem, 14, &cLightGlow_m_Size, 0.0f, 64.0f, str_Size);
    cFloatMenuItem_ctor(&self->CutoffItem, 15, &cLightGlow_m_Cutoff, 0.0f, 1.0f, str_Cutoff);
    cFloatMenuItem_ctor(&self->ColourPowerItem, 16, &cLightGlow_m_ColourPower, 0.0f, 4.0f, str_Colour_Power);
    cFloatMenuItem_ctor(&self->ColourScaleItem, 17, &cLightGlow_m_ColourScale, 0.0f, 64.0f, str_Colour_Scale);
    cFloatMenuItem_ctor(&self->ColourAmbientItem, 18, &cLightGlow_m_ColourAmbient, 0.0f, 16.0f, str_Colour_Ambient);
    cFloatMenuItem_ctor(&self->SizePowerItem, 19, &cLightGlow_m_SizePower, 0.0f, 4.0f, str_Size_Power);
    cFloatMenuItem_ctor(&self->SizeScaleItem, 20, &cLightGlow_m_SizeScale, 0.0f, 64.0f, str_Size_Scale);
    cFloatMenuItem_ctor(&self->SizeAmbientItem, 21, &cLightGlow_m_SizeAmbient, 0.0f, 64.0f, str_Size_Ambient);
    cFloatMenuItem_ctor(&self->FadeRateItem, 22, &cLightGlow_m_FadeRate, 0.0f, 1.0f, str_Fade_Rate);
    cIntMenuItem_cIntMenuItem(&self->ClearAlphaItem, 23, &cLightGlow_m_ClearAlpha, 0, 255, str_Background_Alpha);
    cFloatMenuItem_ctor(&self->IntensityRItem, 24, &cLightGlow_m_IntensityR, 0.0f, 1.0f, str_Intensity_Red);
    cFloatMenuItem_ctor(&self->IntensityGItem, 25, &cLightGlow_m_IntensityG, 0.0f, 1.0f, str_Intensity_Green);
    cFloatMenuItem_ctor(&self->IntensityBItem, 26, &cLightGlow_m_IntensityB, 0.0f, 1.0f, str_Intensity_Blue);
    cFloatMenuItem_ctor(&self->IntensityAItem, 27, &cLightGlow_m_IntensityA, 0.0f, 1.0f, str_Intensity_Alpha);
    cFloatMenuItem_ctor(&self->IntensityScaleItem, 28, &cLightGlow_m_IntensityScale, 0.0f, 1.0f, str_Intensity_Scale);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->OverrideWPItem, -1);
    cMenu_addItem(self, &self->PS2CaptureSizeItem, -1);
    cMenu_addItem(self, &self->PS2MinIntensityItem, -1);
    cMenu_addItem(self, &self->PS2ModIntensityItem, -1);
    cMenu_addItem(self, &self->PS2CopyIntensityItem, -1);
    cMenu_addItem(self, &self->PS2FrameSrcScaleItem, -1);
    cMenu_addItem(self, &self->PS2FrameDstScaleItem, -1);
    cMenu_addItem(self, &self->PS2IntensityTex0Item, -1);
    cMenu_addItem(self, &self->PS2IntensityTex1Item, -1);
    cMenu_addItem(self, &self->PS2IntensityTex2Item, -1);
    cMenu_addItem(self, &self->PS2IntensityTex3Item, -1);
    cMenu_addItem(self, &self->PS2JitterTexItem, -1);
    cMenu_addItem(self, &self->PS2ShowTexturesItem, -1);
    cMenu_addItem(self, &self->SizeItem, -1);
    cMenu_addItem(self, &self->CutoffItem, -1);
    cMenu_addItem(self, &self->ColourPowerItem, -1);
    cMenu_addItem(self, &self->ColourScaleItem, -1);
    cMenu_addItem(self, &self->ColourAmbientItem, -1);
    cMenu_addItem(self, &self->SizePowerItem, -1);
    cMenu_addItem(self, &self->SizeScaleItem, -1);
    cMenu_addItem(self, &self->SizeAmbientItem, -1);
    cMenu_addItem(self, &self->FadeRateItem, -1);
    cMenu_addItem(self, &self->ClearAlphaItem, -1);
    cMenu_addItem(self, &self->IntensityRItem, -1);
    cMenu_addItem(self, &self->IntensityGItem, -1);
    cMenu_addItem(self, &self->IntensityBItem, -1);
    cMenu_addItem(self, &self->IntensityAItem, -1);
    cMenu_addItem(self, &self->IntensityScaleItem, -1);
    return ((void*)self);
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
extern "C" void* cDepthFogMenu_cDepthFogMenu(cDepthFogMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cDepthFogMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Depth_Fog_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cDepthFog_m_Enable, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem(&self->OverrideWPItem, 1, &cDepthFog_m_OverrideWP, 0, 1, str_Override_World_Painter);
    cFloatMenuItem_ctor(&self->NearPlaneItem, 2, &cDepthFog_m_NearPlane, 1.0f, 100000.0f, str_Near_Plane);
    cFloatMenuItem_ctor(&self->FarPlaneItem, 3, &cDepthFog_m_FarPlane, 1.0f, 100000.0f, str_Far_Plane);
    cIntMenuItem_cIntMenuItem(&self->ExponentItem, 4, &cDepthFog_m_Exponent, 0, 2, str_Exponent);
    cFloatMenuItem_ctor(&self->DensityItem, 5, &cDepthFog_m_Density, 0.0f, 100.0f, str_Density);
    cFloatMenuItem_ctor(&self->XBoxDensityScaleItem, 6, &cDepthFog_m_XBoxDensityScale, 0.0f, 1.0f, str_XBox_Skybox_Density_Scale);
    cARGBMenuItem_cARGBMenuItem(&self->ColourItem, 7, str_Colour, cDepthFog_m_Colour);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->OverrideWPItem, -1);
    cMenu_addItem(self, &self->NearPlaneItem, -1);
    cMenu_addItem(self, &self->FarPlaneItem, -1);
    cMenu_addItem(self, &self->ExponentItem, -1);
    cMenu_addItem(self, &self->DensityItem, -1);
    cMenu_addItem(self, &self->XBoxDensityScaleItem, -1);
    cMenu_addItem(self, &self->ColourItem, -1);
    return ((void*)self);
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
extern int cHeightFog_m_Enable;
extern float cHeightFog_m_StartHeight;
extern float cHeightFog_m_Thickness;

/**
 * Builds the "Height Fog Menu" debug menu: a title, a spacer and an enable switch, start height and
 * thickness.
 *
 * @return @p self
 */
extern "C" void* cHeightFogMenu_cHeightFogMenu(cHeightFogMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cHeightFogMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Height_Fog_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cHeightFog_m_Enable, 0, 1, str_Enable);
    cFloatMenuItem_cFloatMenuItem(&self->StartHeightItem, 1, &cHeightFog_m_StartHeight, str_Start_Height, -100000.0f, 100000.0f);
    cFloatMenuItem_cFloatMenuItem(&self->ThicknessItem, 2, &cHeightFog_m_Thickness, str_Thickness, 9.999999747378752e-05f, 10000.0f);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->StartHeightItem, -1);
    cMenu_addItem(self, &self->ThicknessItem, -1);
    return ((void*)self);
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
extern int cDetailSystem_m_Enable;
extern float cDetailSystem_m_SpinScale;
extern float cDetailSystem_m_PS2SparkleCount;
extern float cDetailSystem_m_PS2SparkleSize;
extern float cDetailSystem_m_PS2MinSparkleSize;
extern float cDetailSystem_m_PS2MaxSparkleSize;
extern float cDetailSystem_m_XBoxNumSparkles;
extern float cDetailSystem_m_XBoxSize;
extern float cDetailSystem_m_XBoxMinSize;
extern float cDetailSystem_m_XBoxMaxSize;

/**
 * Builds the "Detail System Menu" debug menu: a title, a spacer and an enable switch and the sparkle settings
 * for PS2 and Xbox (count, world size, screen size limits).
 *
 * @return @p self
 */
extern "C" void* cDetailSystemMenu_cDetailSystemMenu(cDetailSystemMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cDetailSystemMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Detail_System_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cDetailSystem_m_Enable, 0, 1, str_Enable);
    cFloatMenuItem_ctor(&self->SpinScaleItem, 1, &cDetailSystem_m_SpinScale, -1.0f, 1.0f, str_Sparkle_Spin_Scale);
    cFloatMenuItem_ctor(&self->PS2SparkleCountItem, 2, &cDetailSystem_m_PS2SparkleCount, 0.0f, 20.0f, str_PS2_Sparkle_Count);
    cFloatMenuItem_ctor(&self->PS2SparkleSizeItem, 3, &cDetailSystem_m_PS2SparkleSize, 0.0f, 500.0f, str_PS2_Sparkle_World_Size);
    cFloatMenuItem_ctor(&self->PS2MinSparkleSizeItem, 4, &cDetailSystem_m_PS2MinSparkleSize, 0.0f, 100.0f, str_PS2_Min_Sparkle_Screen_Size);
    cFloatMenuItem_ctor(&self->PS2MaxSparkleSizeItem, 5, &cDetailSystem_m_PS2MaxSparkleSize, 0.0f, 100.0f, str_PS2_Max_Sparkle_Screen_Size);
    cFloatMenuItem_ctor(&self->XBoxNumSparklesItem, 6, &cDetailSystem_m_XBoxNumSparkles, 1.0f, 300.0f, str_XBox_Sparkle_Count);
    cFloatMenuItem_ctor(&self->XBoxSizeItem, 7, &cDetailSystem_m_XBoxSize, 0.0f, 500.0f, str_XBox_World_Size);
    cFloatMenuItem_ctor(&self->XBoxMinSizeItem, 8, &cDetailSystem_m_XBoxMinSize, 0.0f, 100.0f, str_XBox_Min_Screen_Size);
    cFloatMenuItem_ctor(&self->XBoxMaxSizeItem, 9, &cDetailSystem_m_XBoxMaxSize, 0.0f, 100.0f, str_XBox_Max_Screen_Size);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->SpinScaleItem, -1);
    cMenu_addItem(self, &self->PS2SparkleCountItem, -1);
    cMenu_addItem(self, &self->PS2SparkleSizeItem, -1);
    cMenu_addItem(self, &self->PS2MinSparkleSizeItem, -1);
    cMenu_addItem(self, &self->PS2MaxSparkleSizeItem, -1);
    cMenu_addItem(self, &self->XBoxNumSparklesItem, -1);
    cMenu_addItem(self, &self->XBoxSizeItem, -1);
    cMenu_addItem(self, &self->XBoxMinSizeItem, -1);
    cMenu_addItem(self, &self->XBoxMaxSizeItem, -1);
    return ((void*)self);
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
extern "C" void* cShellRendererMenu_cShellRendererMenu(cShellRendererMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cShellRendererMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Shell_Renderer_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &D_004A46B8, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem(&self->ModelTextureIdItem, 1, &D_004A46BC, 0, 200, str_Model_Texture_Id);
    cIntMenuItem_cIntMenuItem(&self->ShellTextureIdItem, 2, &D_004A46C0, 0, 200, str_Shell_Texture_Id);
    cIntMenuItem_cIntMenuItem(&self->NearPlaneItem, 3, &D_004A46C4, 1, 10000, str_Near_Plane);
    cIntMenuItem_cIntMenuItem(&self->FarPlaneItem, 4, &D_004A46C8, 1, 10000, str_Far_Plane);
    cIntMenuItem_cIntMenuItem(&self->DistScaleItem, 5, &D_004A46CC, 1, 0x186a0, str_DistScale);
    cFloatMenuItem_ctor(&self->DepthScaleItem, 6, &D_004A46D0, 1e-05f, 2.0f, str_Depth_Scale);
    cFloatMenuItem_ctor(&self->DepthBrightnessItem, 7, &D_004A46D4, 0.0f, 1.0f, str_Depth_Brightness);
    cIntMenuItem_cIntMenuItem(&self->MaxShellsItem, 8, &D_004A46D8, 1, 0x40, str_Max_Shells);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->ModelTextureIdItem, -1);
    cMenu_addItem(self, &self->ShellTextureIdItem, -1);
    cMenu_addItem(self, &self->NearPlaneItem, -1);
    cMenu_addItem(self, &self->FarPlaneItem, -1);
    cMenu_addItem(self, &self->DistScaleItem, -1);
    cMenu_addItem(self, &self->DepthScaleItem, -1);
    cMenu_addItem(self, &self->DepthBrightnessItem, -1);
    cMenu_addItem(self, &self->MaxShellsItem, -1);
    return ((void*)self);
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
extern float cScreenTint_m_XNLerp;
extern int cScreenTint_m_LightningEnable;
extern int cScreenTint_m_FrameId;
extern float cScreenTint_m_LightningChance;
extern int cScreenTint_m_LightningFadeIn;
extern int cScreenTint_m_LightningFadeOut;
extern int cScreenTint_m_LightningFadeIn2;
extern int cScreenTint_m_LightningFadeIn3;
extern float cScreenTint_m_LightningScale;
extern float cScreenTint_m_LightningScale2;
extern float cScreenTint_m_LightningScale3;
extern float cScreenTint_m_LightningColourR;
extern float cScreenTint_m_LightningColourG;
extern float cScreenTint_m_LightningColourB;
extern float cScreenTint_m_LightningColour2R;
extern float cScreenTint_m_LightningColour2G;
extern float cScreenTint_m_LightningColour2B;
extern float cScreenTint_m_LightningColour3R;
extern float cScreenTint_m_LightningColour3G;
extern float cScreenTint_m_LightningColour3B;
extern int cScreenTint_m_GammaEnable;
extern float cScreenTint_m_GammaPower;
extern float cScreenTint_m_GammaColourR;
extern float cScreenTint_m_GammaColourG;
extern float cScreenTint_m_GammaColourB;
extern float cScreenTint_m_GammaFillR;
extern float cScreenTint_m_GammaFillG;
extern float cScreenTint_m_GammaFillB;

/**
 * Builds the "Screen Tint Menu" debug menu: a title, a spacer and 40 items for the screen tint and fill
 * colours, the lightning flash (chance, fade, sustain, inverse and recover tints) and gamma correction.
 *
 * @return @p self
 */
extern "C" void* cScreenTintMenu_cScreenTintMenu(cScreenTintMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cScreenTintMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Screen_Tint_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cScreenTint_m_Enable, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem(&self->OverrideWPItem, 1, &cScreenTint_m_OverrideWP, 0, 1, str_Override_World_Painter);
    cIntMenuItem_cIntMenuItem(&self->DisplayValuesItem, 2, &cScreenTint_m_DisplayValues, 0, 1, str_Display_Values);
    cFloatMenuItem_ctor(&self->ColourRItem, 3, &cScreenTint_m_ColourR, 0.0f, 2.0f, str_Tint_Red);
    cFloatMenuItem_ctor(&self->ColourGItem, 4, &cScreenTint_m_ColourG, 0.0f, 2.0f, str_Tint_Green);
    cFloatMenuItem_ctor(&self->ColourBItem, 5, &cScreenTint_m_ColourB, 0.0f, 2.0f, str_Tint_Blue);
    cFloatMenuItem_ctor(&self->FillRItem, 6, &cScreenTint_m_FillR, 0.0f, 1.0f, str_Fill_Red);
    cFloatMenuItem_ctor(&self->FillGItem, 7, &cScreenTint_m_FillG, 0.0f, 1.0f, str_Fill_Green);
    cFloatMenuItem_ctor(&self->FillBItem, 8, &cScreenTint_m_FillB, 0.0f, 1.0f, str_Fill_Blue);
    cFloatMenuItem_ctor(&self->XNColourRItem, 9, &cScreenTint_m_XNColourR, 0.0f, 2.0f, str_XN_Tint_Red);
    cFloatMenuItem_ctor(&self->XNColourGItem, 10, &cScreenTint_m_XNColourG, 0.0f, 2.0f, str_XN_Tint_Green);
    cFloatMenuItem_ctor(&self->XNColourBItem, 11, &cScreenTint_m_XNColourB, 0.0f, 2.0f, str_XN_Tint_Blue);
    cFloatMenuItem_ctor(&self->XNLerpItem, 0xc, &cScreenTint_m_XNLerp, 0.0f, 2.0f, str_XN_Lerp);
    cIntMenuItem_cIntMenuItem(&self->LightningEnableItem, 13, &cScreenTint_m_LightningEnable, 0, 1, str_Enable_Lightning);
    cIntMenuItem_cIntMenuItem(&self->FrameIdItem, 0xe, &cScreenTint_m_FrameId, -1, 1000, str_Frame_Id);
    cFloatMenuItem_ctor(&self->LightningChanceItem, 0xf, &cScreenTint_m_LightningChance, 0.0f, 1.0f, str_Lightning_Chance);
    cIntMenuItem_cIntMenuItem(&self->LightningFadeInItem, 0x10, &cScreenTint_m_LightningFadeIn, 0, 600, str_Flash_Fade_In);
    cIntMenuItem_cIntMenuItem(&self->LightningFadeOutItem, 17, &cScreenTint_m_LightningFadeOut, 0, 600, str_Flash_Sustain);
    cIntMenuItem_cIntMenuItem(&self->LightningFadeIn2Item, 18, &cScreenTint_m_LightningFadeIn2, 0, 600, str_Inverse_Duration);
    cIntMenuItem_cIntMenuItem(&self->LightningFadeIn3Item, 19, &cScreenTint_m_LightningFadeIn3, 0, 600, str_Recover_Fade_In);
    cFloatMenuItem_ctor(&self->LightningScaleItem, 20, &cScreenTint_m_LightningScale, 0.0f, 2.0f, str_Flash_Scale);
    cFloatMenuItem_ctor(&self->LightningScale2Item, 21, &cScreenTint_m_LightningScale2, 0.0f, 2.0f, str_Inverse_Scale);
    cFloatMenuItem_ctor(&self->LightningScale3Item, 22, &cScreenTint_m_LightningScale3, 0.0f, 2.0f, str_Recover_Scale);
    cFloatMenuItem_ctor(&self->LightningColourRItem, 23, &cScreenTint_m_LightningColourR, 0.0f, 2.0f, str_Flash_Tint_Red);
    cFloatMenuItem_ctor(&self->LightningColourGItem, 0x18, &cScreenTint_m_LightningColourG, 0.0f, 2.0f, str_Flash_Tint_Green);
    cFloatMenuItem_ctor(&self->LightningColourBItem, 25, &cScreenTint_m_LightningColourB, 0.0f, 2.0f, str_Flash_Tint_Blue);
    cFloatMenuItem_ctor(&self->LightningColour2RItem, 26, &cScreenTint_m_LightningColour2R, 0.0f, 2.0f, str_Inverse_Tint_Red);
    cFloatMenuItem_ctor(&self->LightningColour2GItem, 27, &cScreenTint_m_LightningColour2G, 0.0f, 2.0f, str_Inverse_Tint_Green);
    cFloatMenuItem_ctor(&self->LightningColour2BItem, 0x1c, &cScreenTint_m_LightningColour2B, 0.0f, 2.0f, str_Inverse_Tint_Blue);
    cFloatMenuItem_ctor(&self->LightningColour3RItem, 29, &cScreenTint_m_LightningColour3R, 0.0f, 2.0f, str_Recover_Tint_Red);
    cFloatMenuItem_ctor(&self->LightningColour3GItem, 0x1e, &cScreenTint_m_LightningColour3G, 0.0f, 2.0f, str_Recover_Tint_Green);
    cFloatMenuItem_ctor(&self->LightningColour3BItem, 0x1f, &cScreenTint_m_LightningColour3B, 0.0f, 2.0f, str_Recover_Tint_Blue);
    cIntMenuItem_cIntMenuItem(&self->GammaEnableItem, 0x20, &cScreenTint_m_GammaEnable, 0, 1, str_Gamma_Enable);
    cFloatMenuItem_ctor(&self->GammaPowerItem, 33, &cScreenTint_m_GammaPower, 0.0f, 2.0f, str_Gamma_Power);
    cFloatMenuItem_ctor(&self->GammaColourRItem, 34, &cScreenTint_m_GammaColourR, 0.0f, 2.0f, str_Gamma_Tint_Red);
    cFloatMenuItem_ctor(&self->GammaColourGItem, 35, &cScreenTint_m_GammaColourG, 0.0f, 2.0f, str_Gamma_Tint_Green);
    cFloatMenuItem_ctor(&self->GammaColourBItem, 36, &cScreenTint_m_GammaColourB, 0.0f, 2.0f, str_Gamma_Tint_Blue);
    cFloatMenuItem_ctor(&self->GammaFillRItem, 37, &cScreenTint_m_GammaFillR, 0.0f, 1.0f, str_Gamma_Fill_Red);
    cFloatMenuItem_ctor(&self->GammaFillGItem, 38, &cScreenTint_m_GammaFillG, 0.0f, 1.0f, str_Gamma_Fill_Green);
    cFloatMenuItem_ctor(&self->GammaFillBItem, 39, &cScreenTint_m_GammaFillB, 0.0f, 1.0f, str_Gamma_Fill_Blue);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->OverrideWPItem, -1);
    cMenu_addItem(self, &self->DisplayValuesItem, -1);
    cMenu_addItem(self, &self->ColourRItem, -1);
    cMenu_addItem(self, &self->ColourGItem, -1);
    cMenu_addItem(self, &self->ColourBItem, -1);
    cMenu_addItem(self, &self->FillRItem, -1);
    cMenu_addItem(self, &self->FillGItem, -1);
    cMenu_addItem(self, &self->FillBItem, -1);
    cMenu_addItem(self, &self->XNColourRItem, -1);
    cMenu_addItem(self, &self->XNColourGItem, -1);
    cMenu_addItem(self, &self->XNColourBItem, -1);
    cMenu_addItem(self, &self->XNLerpItem, -1);
    cMenu_addItem(self, &self->LightningEnableItem, -1);
    cMenu_addItem(self, &self->FrameIdItem, -1);
    cMenu_addItem(self, &self->LightningChanceItem, -1);
    cMenu_addItem(self, &self->LightningFadeInItem, -1);
    cMenu_addItem(self, &self->LightningFadeOutItem, -1);
    cMenu_addItem(self, &self->LightningFadeIn2Item, -1);
    cMenu_addItem(self, &self->LightningFadeIn3Item, -1);
    cMenu_addItem(self, &self->LightningScaleItem, -1);
    cMenu_addItem(self, &self->LightningScale2Item, -1);
    cMenu_addItem(self, &self->LightningScale3Item, -1);
    cMenu_addItem(self, &self->LightningColourRItem, -1);
    cMenu_addItem(self, &self->LightningColourGItem, -1);
    cMenu_addItem(self, &self->LightningColourBItem, -1);
    cMenu_addItem(self, &self->LightningColour2RItem, -1);
    cMenu_addItem(self, &self->LightningColour2GItem, -1);
    cMenu_addItem(self, &self->LightningColour2BItem, -1);
    cMenu_addItem(self, &self->LightningColour3RItem, -1);
    cMenu_addItem(self, &self->LightningColour3GItem, -1);
    cMenu_addItem(self, &self->LightningColour3BItem, -1);
    cMenu_addItem(self, &self->GammaEnableItem, -1);
    cMenu_addItem(self, &self->GammaPowerItem, -1);
    cMenu_addItem(self, &self->GammaColourRItem, -1);
    cMenu_addItem(self, &self->GammaColourGItem, -1);
    cMenu_addItem(self, &self->GammaColourBItem, -1);
    cMenu_addItem(self, &self->GammaFillRItem, -1);
    cMenu_addItem(self, &self->GammaFillGItem, -1);
    cMenu_addItem(self, &self->GammaFillBItem, -1);
    return ((void*)self);
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
extern int cShadow_m_Enable;
extern int cShadow_m_ShowShadowPolys;
extern float cShadow_m_ShadowDirX;
extern float cShadow_m_ShadowDirY;
extern float cShadow_m_ShadowDirZ;
extern float cShadow_m_Length;
extern float cShadow_m_Intensity;
extern float cShadow_m_StartUV;
extern float cShadow_m_EndUV;

/**
 * Builds the "Shadow Menu" debug menu: a title, a spacer and 9 items: enable, show shadow polys, the shadow
 * direction, length, intensity and UV range.
 *
 * @return @p self
 */
extern "C" void* cShadowMenu_cShadowMenu(cShadowMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cShadowMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Shadow_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cShadow_m_Enable, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem(&self->ShowShadowPolysItem, 1, &cShadow_m_ShowShadowPolys, 0, 1, str_Show_Shadow_Polys);
    cFloatMenuItem_ctor(&self->ShadowDirXItem, 2, &cShadow_m_ShadowDirX, -1.0f, 1.0f, str_Shadow_Dir_X);
    cFloatMenuItem_ctor(&self->ShadowDirYItem, 3, &cShadow_m_ShadowDirY, -1.0f, 1.0f, str_Shadow_Dir_Y);
    cFloatMenuItem_ctor(&self->ShadowDirZItem, 4, &cShadow_m_ShadowDirZ, -1.0f, 1.0f, str_Shadow_Dir_Z);
    cFloatMenuItem_ctor(&self->LengthItem, 5, &cShadow_m_Length, 0.1f, 100000.0f, str_Length);
    cFloatMenuItem_ctor(&self->IntensityItem, 6, &cShadow_m_Intensity, 0.0f, 1.0f, str_Intensity);
    cFloatMenuItem_ctor(&self->StartUVItem, 7, &cShadow_m_StartUV, -64.0f, 64.0f, str_Start_UV);
    cFloatMenuItem_ctor(&self->EndUVItem, 8, &cShadow_m_EndUV, -64.0f, 64.0f, str_End_UV);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->ShowShadowPolysItem, -1);
    cMenu_addItem(self, &self->ShadowDirXItem, -1);
    cMenu_addItem(self, &self->ShadowDirYItem, -1);
    cMenu_addItem(self, &self->ShadowDirZItem, -1);
    cMenu_addItem(self, &self->LengthItem, -1);
    cMenu_addItem(self, &self->IntensityItem, -1);
    cMenu_addItem(self, &self->StartUVItem, -1);
    cMenu_addItem(self, &self->EndUVItem, -1);
    return ((void*)self);
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
extern int cSnowSurface_m_Enable;
extern int cSnowSurface_m_EnablePatchReflect;
extern float cSnowSurface_m_Base;
extern float cSnowSurface_m_Bump;
extern float cSnowSurface_m_Noise;
extern float cSnowSurface_m_MinColourScale;
extern float cSnowSurface_m_MaxColourScale;

/**
 * Builds the "Snow Surface Menu" debug menu: a title, a spacer and an enable switch, patch reflection, the
 * colour add and scale limits and base, bump and noise values.
 *
 * @return @p self
 */
extern "C" void* cSnowSurfaceMenu_cSnowSurfaceMenu(cSnowSurfaceMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cSnowSurfaceMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Snow_Surface_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cSnowSurface_m_Enable, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem(&self->EnablePatchReflectItem, 1, &cSnowSurface_m_EnablePatchReflect, 0, 1, str_Enable_Patch_Reflect);
    cFloatMenuItem_cFloatMenuItem(&self->BaseItem, 2, &cSnowSurface_m_Base, str_Base, 0.0f, 32.0f);
    cFloatMenuItem_cFloatMenuItem(&self->BumpItem, 3, &cSnowSurface_m_Bump, str_Bump, 0.0f, 32.0f);
    cFloatMenuItem_cFloatMenuItem(&self->NoiseItem, 4, &cSnowSurface_m_Noise, str_Noise, 0.0f, 32.0f);
    cFloatMenuItem_cFloatMenuItem(&self->MinColourScaleItem, 5, &cSnowSurface_m_MinColourScale, str_Min_Colour_Add, 0.0f, 3.0f);
    cFloatMenuItem_cFloatMenuItem(&self->MaxColourScaleItem, 6, &cSnowSurface_m_MaxColourScale, str_Max_Colour_Scale, 0.0f, 3.0f);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->EnablePatchReflectItem, -1);
    cMenu_addItem(self, &self->BaseItem, -1);
    cMenu_addItem(self, &self->BumpItem, -1);
    cMenu_addItem(self, &self->NoiseItem, -1);
    cMenu_addItem(self, &self->MinColourScaleItem, -1);
    cMenu_addItem(self, &self->MaxColourScaleItem, -1);
    return ((void*)self);
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
extern int cBoardTrail_m_Enable;
extern int cBoardTrail_m_NumFadeSegments;
struct sShadowOpt_cBoardTrailMenu {
    int fba;
};
extern sShadowOpt_cBoardTrailMenu D_004A45CC;
extern int cBoardTrail_m_UseSliceCaps;
extern float cBoardTrail_m_InnerWidthScale;
extern float cBoardTrail_m_OuterWidthScale;
extern float cBoardTrail_m_HeightScale;
extern float cBoardTrail_m_Depth2HeightScale;
extern float cBoardTrail_m_PackedDepthScale;
extern float cBoardTrail_m_LooseDepthScale;
extern float cBoardTrail_m_PowderDepthScale;
extern float cBoardTrail_m_DeepPowderDepthScale;
extern float cBoardTrail_m_TopBiasFront;
extern float cBoardTrail_m_TopBiasBehind;
extern float cBoardTrail_m_TopBiasClear;
extern float cBoardTrail_m_InnerPositionJitter;
extern float cBoardTrail_m_BaseU;
extern float cBoardTrail_m_InnerU;
extern float cBoardTrail_m_OuterU;
extern float cBoardTrail_m_OuterNormalOffset;
extern float cBoardTrail_m_InnerNormalOffset;
extern float cBoardTrail_m_BaseNormalOffset;

/**
 * Builds the "Board Trail Menu" debug menu: a title, a spacer and 22 items for the board trail mesh: widths,
 * depth per snow type, top biases, UVs, normal offsets, fade segments and slice caps.
 *
 * @return @p self
 */
extern "C" void* cBoardTrailMenu_cBoardTrailMenu(cBoardTrailMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cBoardTrailMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Board_Trail_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cBoardTrail_m_Enable, 0, 0x1, str_Enable);
    cFloatMenuItem_ctor(&self->InnerWidthScaleItem, 1, &cBoardTrail_m_InnerWidthScale, -100.0f, 100.0f, str_Inner_Width_Scale);
    cFloatMenuItem_ctor(&self->OuterWidthScaleItem, 2, &cBoardTrail_m_OuterWidthScale, -1000.0f, 1000.0f, str_Outer_Width_Scale);
    cFloatMenuItem_ctor(&self->HeightScaleItem, 3, &cBoardTrail_m_HeightScale, -20.0f, 20.0f, str_Height_Scale);
    cFloatMenuItem_ctor(&self->Depth2HeightScaleItem, 4, &cBoardTrail_m_Depth2HeightScale, 0.0f, 1.0f, str_Depth_to_Height_Scale);
    cFloatMenuItem_ctor(&self->PackedDepthScaleItem, 5, &cBoardTrail_m_PackedDepthScale, -100.0f, 100.0f, str_Packed_Depth_Scale);
    cFloatMenuItem_ctor(&self->LooseDepthScaleItem, 6, &cBoardTrail_m_LooseDepthScale, -100.0f, 100.0f, str_Loose_Depth_Scale);
    cFloatMenuItem_ctor(&self->PowderDepthScaleItem, 7, &cBoardTrail_m_PowderDepthScale, -1000.0f, 1000.0f, str_Powder_Depth_Scale);
    cFloatMenuItem_ctor(&self->DeepPowderDepthScaleItem, 8, &cBoardTrail_m_DeepPowderDepthScale, -1000.0f, 1000.0f, str_Deep_Powder_Depth_Scale);
    cFloatMenuItem_ctor(&self->TopBiasFrontItem, 9, &cBoardTrail_m_TopBiasFront, -1000.0f, 1000.0f, str_Top_Bias_Front);
    cFloatMenuItem_ctor(&self->TopBiasBehindItem, 10, &cBoardTrail_m_TopBiasBehind, -1000.0f, 1000.0f, str_Top_Bias_Behind);
    cFloatMenuItem_ctor(&self->TopBiasClearItem, 11, &cBoardTrail_m_TopBiasClear, -16777215.0f, 16777215.0f, str_Top_Bias_Clear);
    cFloatMenuItem_ctor(&self->InnerPositionJitterItem, 12, &cBoardTrail_m_InnerPositionJitter, -10.0f, 10.0f, str_Inner_Position_Jitter);
    cFloatMenuItem_ctor(&self->BaseUItem, 13, &cBoardTrail_m_BaseU, 0.0f, 1.0f, str_Base_U);
    cFloatMenuItem_ctor(&self->InnerUItem, 14, &cBoardTrail_m_InnerU, 0.0f, 1.0f, str_Inner_U);
    cFloatMenuItem_ctor(&self->OuterUItem, 15, &cBoardTrail_m_OuterU, 0.0f, 1.0f, str_Outer_U);
    cFloatMenuItem_ctor(&self->OuterNormalOffsetItem, 16, &cBoardTrail_m_OuterNormalOffset, -10.0f, 10.0f, str_Outer_Normal_Offset);
    cFloatMenuItem_ctor(&self->InnerNormalOffsetItem, 17, &cBoardTrail_m_InnerNormalOffset, -10.0f, 10.0f, str_Inner_Normal_Offset);
    cFloatMenuItem_ctor(&self->BaseNormalOffsetItem, 18, &cBoardTrail_m_BaseNormalOffset, -10.0f, 10.0f, str_Base_Normal_Offset);
    cIntMenuItem_cIntMenuItem(&self->NumFadeSegmentsItem, 19, &cBoardTrail_m_NumFadeSegments, 0, 0x40, str_Num_Fade_Segments);
    cIntMenuItem_cIntMenuItem(&self->DrawFixupLayerItem, 20, &D_004A45CC.fba, 0, 0x1, str_Draw_Fixup_Layer);
    cIntMenuItem_cIntMenuItem(&self->UseSliceCapsItem, 21, &cBoardTrail_m_UseSliceCaps, 0, 0x1, str_Use_Slice_Caps);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->InnerWidthScaleItem, -1);
    cMenu_addItem(self, &self->OuterWidthScaleItem, -1);
    cMenu_addItem(self, &self->HeightScaleItem, -1);
    cMenu_addItem(self, &self->Depth2HeightScaleItem, -1);
    cMenu_addItem(self, &self->PackedDepthScaleItem, -1);
    cMenu_addItem(self, &self->LooseDepthScaleItem, -1);
    cMenu_addItem(self, &self->PowderDepthScaleItem, -1);
    cMenu_addItem(self, &self->DeepPowderDepthScaleItem, -1);
    cMenu_addItem(self, &self->TopBiasFrontItem, -1);
    cMenu_addItem(self, &self->TopBiasBehindItem, -1);
    cMenu_addItem(self, &self->TopBiasClearItem, -1);
    cMenu_addItem(self, &self->InnerPositionJitterItem, -1);
    cMenu_addItem(self, &self->BaseUItem, -1);
    cMenu_addItem(self, &self->InnerUItem, -1);
    cMenu_addItem(self, &self->OuterUItem, -1);
    cMenu_addItem(self, &self->OuterNormalOffsetItem, -1);
    cMenu_addItem(self, &self->InnerNormalOffsetItem, -1);
    cMenu_addItem(self, &self->BaseNormalOffsetItem, -1);
    cMenu_addItem(self, &self->NumFadeSegmentsItem, -1);
    cMenu_addItem(self, &self->DrawFixupLayerItem, -1);
    cMenu_addItem(self, &self->UseSliceCapsItem, -1);
    return ((void*)self);
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
extern int cFlags_m_EnableUpdate;
extern int cFlags_m_EnableRender;

/**
 * Builds the "Flags Menu" debug menu: a title, a spacer and two toggles, EnableUpdate and EnableRender.
 *
 * @return @p self
 */
extern "C" void* cFlagsMenu_cFlagsMenu(cFlagsMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cFlagsMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Flags_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableUpdateItem, 0, &cFlags_m_EnableUpdate, 0, 1, str_EnableUpdate);
    cIntMenuItem_cIntMenuItem(&self->EnableRenderItem, 1, &cFlags_m_EnableRender, 0, 1, str_EnableRender);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableUpdateItem, -1);
    cMenu_addItem(self, &self->EnableRenderItem, -1);
    return ((void*)self);
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
extern "C" void* cPowerUpFXTogglesMenu_cPowerUpFXTogglesMenu(cPowerUpFXTogglesMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cPowerUpFXTogglesMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_PowerUpFXToggles_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableSpeedBoostStreamersItem, 0, &cPowerUpFXToggles_m_EnableSpeedBoostStreamers, 0, 1, str_Enable_Speed_Boost_Streamers);
    cIntMenuItem_cIntMenuItem(&self->EnableSpeedBoostScreenBlurItem, 1, &cPowerUpFXToggles_m_EnableSpeedBoostScreenBlur, 0, 1, str_Enable_Speed_Boost_Screen_Blur);
    cFloatMenuItem_ctor(&self->SpeedBoostScreenBlurMaxPercentItem, 2, &cPowerUpFXToggles_m_SpeedBoostScreenBlurMaxPercent, 0.0f, 1.0f, str_Max_Screen_Blur_Percent);
    cFloatMenuItem_ctor(&self->SpeedBoostScreenBlurIncreaseRateItem, 3, &cPowerUpFXToggles_m_SpeedBoostScreenBlurIncreaseRate, 0.0f, 60.0f, str_Screen_Blur_Transition_On_Rate);
    cFloatMenuItem_ctor(&self->SpeedBoostScreenBlurDecreaseRateItem, 4, &cPowerUpFXToggles_m_SpeedBoostScreenBlurDecreaseRate, 0.0f, 60.0f, str_Screen_Blur_Transition_Off_Rate);
    cFloatMenuItem_ctor(&self->SpeedBoostMinSpeedItem, 5, &cPowerUpFXToggles_m_SpeedBoostMinSpeed, 0.0f, 120.0f, str_Screen_Blur_is_not_active_at_this_speed);
    cFloatMenuItem_ctor(&self->SpeedBoostMaxSpeedItem, 6, &cPowerUpFXToggles_m_SpeedBoostMaxSpeed, 0.0f, 120.0f, str_Screen_Blur_is_full_at_this_speed);
    cIntMenuItem_cIntMenuItem(&self->MinSpeedBoostTimeBeforeBlurItem, 7, &cPowerUpFXToggles_m_MinSpeedBoostTimeBeforeBlur, 0, 100, str_Speed_Boost_Time_before_Blur);
    cIntMenuItem_cIntMenuItem(&self->TestTrickBoostItem, 8, &cPowerUpFXToggles_m_TestTrickBoost, 0, 1, str_Test_Trick_Boost);
    cIntMenuItem_cIntMenuItem(&self->EnableSpeedBoostFXItem, 9, &cPowerUpFXToggles_m_EnableSpeedBoostFX, 0, 1, str_Enable_SpeedBoostFX);
    cIntMenuItem_cIntMenuItem(&self->UberStateTestItem, 10, &cPowerUpFXToggles_m_UberStateTest, 0, 1, str_Uber_State_Test);
    cIntMenuItem_cIntMenuItem(&self->SuperUberStateTestItem, 11, &cPowerUpFXToggles_m_SuperUberStateTest, 0, 1, str_Super_Uber_State_Test);
    cIntMenuItem_cIntMenuItem(&self->TestSpeedBoostRibbonItem, 0xc, &cPowerUpFXToggles_m_TestSpeedBoostRibbon, 0, 1, str_Test_Speed_Boost_Ribbon);
    cIntMenuItem_cIntMenuItem(&self->TestRedRibbonItem, 13, &cPowerUpFXToggles_m_TestRedRibbon, 0, 1, str_Test_Red_Ribbon);
    cIntMenuItem_cIntMenuItem(&self->TestOrangeRibbonItem, 0xe, &cPowerUpFXToggles_m_TestOrangeRibbon, 0, 1, str_Test_Orange_Ribbon);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableSpeedBoostStreamersItem, -1);
    cMenu_addItem(self, &self->EnableSpeedBoostScreenBlurItem, -1);
    cMenu_addItem(self, &self->SpeedBoostScreenBlurMaxPercentItem, -1);
    cMenu_addItem(self, &self->SpeedBoostScreenBlurIncreaseRateItem, -1);
    cMenu_addItem(self, &self->SpeedBoostScreenBlurDecreaseRateItem, -1);
    cMenu_addItem(self, &self->SpeedBoostMinSpeedItem, -1);
    cMenu_addItem(self, &self->SpeedBoostMaxSpeedItem, -1);
    cMenu_addItem(self, &self->MinSpeedBoostTimeBeforeBlurItem, -1);
    cMenu_addItem(self, &self->TestTrickBoostItem, -1);
    cMenu_addItem(self, &self->EnableSpeedBoostFXItem, -1);
    cMenu_addItem(self, &self->UberStateTestItem, -1);
    cMenu_addItem(self, &self->SuperUberStateTestItem, -1);
    cMenu_addItem(self, &self->TestSpeedBoostRibbonItem, -1);
    cMenu_addItem(self, &self->TestRedRibbonItem, -1);
    cMenu_addItem(self, &self->TestOrangeRibbonItem, -1);
    return ((void*)self);
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
extern "C" void* cSnowfallTogglesMenu_cSnowfallTogglesMenu(cSnowfallTogglesMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cSnowfallTogglesMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Snowfall_Toggles);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableTestItem, 0, &cSnowfallToggles_m_EnableTest, 0, 1, str_Enable_Test_Values);
    cFloatMenuItem_ctor(&self->TestSnowIntensityItem, 1, &cSnowfallToggles_m_TestSnowIntensity, 0.0f, 4.0f, str_Test_Snowfall_Intensity);
    cFloatMenuItem_ctor(&self->TestWindIntensityItem, 2, &cSnowfallToggles_m_TestWindIntensity, 0.0f, 100.0f, str_Test_Snowfall_Wind);
    cFloatMenuItem_ctor(&self->TestFlakeSizeItem, 3, &cSnowfallToggles_m_TestFlakeSize, 0.1f, 10.0f, str_Test_Snowflake_Size);
    cFloatMenuItem_ctor(&self->TestWindRotationItem, 4, &cSnowfallToggles_m_TestWindRotation, 0.0f, 360.0f, str_Test_Wind_Rotation);
    cFloatMenuItem_ctor(&self->TestSnowFlurriesItem, 5, &cSnowfallToggles_m_TestSnowFlurries, 0.0f, 1.0f, str_Test_Snow_Flurries);
    cFloatMenuItem_ctor(&self->SnowGravityMultiplierItem, 6, &cSnowfallToggles_m_SnowGravityMultiplier, -2.0f, 5.0f, str_Snow_Gravity_Multiplier);
    cFloatMenuItem_ctor(&self->SnowFluffItensityItem, 7, &cSnowfallToggles_m_SnowFluffItensity, 0.0f, 1.0f, str_Snow_Fluff_Intensity);
    cFloatMenuItem_ctor(&self->SnowFluffWindIntensityItem, 8, &cSnowfallToggles_m_SnowFluffWindIntensity, 0.0f, 100.0f, str_Snow_Fluff_Wind_Intensity);
    cARGBMenuItem_cARGBMenuItem(&self->SnowFlakeColourItem, 9, str_Snow_Flake_Colour, cSnowfallToggles_m_SnowFlakeColour);
    cARGBMenuItem_cARGBMenuItem(&self->SnowFluffColourItem, 10, str_Snow_Fluff_Colour, cSnowfallToggles_m_SnowFluffColour);
    cIntMenuItem_cIntMenuItem(&self->TestAdditiveItem, 11, &cSnowfallToggles_m_TestAdditive, 0, 1, str_Test_Additive_Snowfall);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableTestItem, -1);
    cMenu_addItem(self, &self->TestSnowIntensityItem, -1);
    cMenu_addItem(self, &self->TestWindIntensityItem, -1);
    cMenu_addItem(self, &self->TestFlakeSizeItem, -1);
    cMenu_addItem(self, &self->TestWindRotationItem, -1);
    cMenu_addItem(self, &self->TestSnowFlurriesItem, -1);
    cMenu_addItem(self, &self->SnowGravityMultiplierItem, -1);
    cMenu_addItem(self, &self->SnowFluffItensityItem, -1);
    cMenu_addItem(self, &self->SnowFluffWindIntensityItem, -1);
    cMenu_addItem(self, &self->SnowFlakeColourItem, -1);
    cMenu_addItem(self, &self->SnowFluffColourItem, -1);
    cMenu_addItem(self, &self->TestAdditiveItem, -1);
    return ((void*)self);
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
extern int cSplashToggles_m_Enable;
extern int cSplashToggles_m_Render;
extern int cSplashToggles_m_NumDrops;
extern int cSplashToggles_m_NumIceCrystals;
extern float cSplashToggles_m_PercentageIceCrystal;
extern float cSplashToggles_m_PercentageIceCrystalSpawnChance;
extern int cSplashToggles_m_IceCrystalMinGrouping;
extern int cSplashToggles_m_IceCrystalMaxGrouping;
extern int cSplashToggles_m_MaxSpawnPerCrystal;
extern float cSplashToggles_m_FarthestImpactDistance;
extern float cSplashToggles_m_LowestImpactItensity;
extern float cSplashToggles_m_MinimumSnowfallForImpacts;
extern float cSplashToggles_m_ImpactMultiplier;
extern float cSplashToggles_m_LowestSnowfallAmount;
extern float cSplashToggles_m_SnowfallMultiplier;
extern float cSplashToggles_m_MinVelocityForPushoff;
extern float cSplashToggles_m_DropMinLifeSpan;
extern float cSplashToggles_m_DropMaxLifeSpan;
extern float cSplashToggles_m_DropMinSize;
extern float cSplashToggles_m_DropMaxSize;
extern float cSplashToggles_m_DropMaxWidthHeightDifference;
extern float cSplashToggles_m_DropMinVelocityJiggleX;
extern float cSplashToggles_m_DropMaxVelocityJiggleX;
extern float cSplashToggles_m_DropMinVelocityJiggleY;
extern float cSplashToggles_m_DropMaxVelocityJiggleY;
extern float cSplashToggles_m_DropMinSizeGrowth;
extern float cSplashToggles_m_DropMaxSizeGrowth;
extern float cSplashToggles_m_DropMinPushOffSpeed;
extern float cSplashToggles_m_DropMaxPushOffSpeed;
extern float cSplashToggles_m_DropMaxVelocity;
extern float cSplashToggles_m_DropMinVelocityUsage;
extern float cSplashToggles_m_DropMaxVelocityUsage;
extern float cSplashToggles_m_DropMaxRotationChange;
extern float cSplashToggles_m_CrystalGroupSpacingX;
extern float cSplashToggles_m_CrystalGroupSpacingY;
extern float cSplashToggles_m_CrystalMinLifeSpan;
extern float cSplashToggles_m_CrystalMaxLifeSpan;
extern float cSplashToggles_m_CrystalMinSize;
extern float cSplashToggles_m_CrystalMaxSize;
extern float cSplashToggles_m_CrystalMaxWidthHeightDifference;
extern float cSplashToggles_m_CrystalMinVelocityJiggleX;
extern float cSplashToggles_m_CrystalMaxVelocityJiggleX;
extern float cSplashToggles_m_CrystalMinVelocityJiggleY;
extern float cSplashToggles_m_CrystalMaxVelocityJiggleY;
extern float cSplashToggles_m_CrystalMinJiggleTime;
extern float cSplashToggles_m_CrystalMaxJiggleTime;
extern float cSplashToggles_m_CrystalMinSizeGrowth;
extern float cSplashToggles_m_CrystalMaxSizeGrowth;
extern float cSplashToggles_m_CrystalMinPushOffSpeed;
extern float cSplashToggles_m_CrystalMaxPushOffSpeed;
extern float cSplashToggles_m_CrystalMaxVelocity;
extern float cSplashToggles_m_CrystalMaxRotationChange;

/**
 * Builds the "Camera Splash Menu" debug menu: a title, a spacer and 52 items for snow drops and ice crystals
 * hitting the camera: counts, spawn chances, impact limits, life spans, sizes, jiggle, push-off and velocity
 * ranges.
 *
 * @return @p self
 */
extern "C" void* cSplashTogglesMenu_cSplashTogglesMenu(cSplashTogglesMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cSplashTogglesMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Camera_Splash_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cSplashToggles_m_Enable, 0, 1, str_Enable);
    cIntMenuItem_cIntMenuItem(&self->RenderItem, 1, &cSplashToggles_m_Render, 0, 1, str_Render);
    cIntMenuItem_cIntMenuItem(&self->NumDropsItem, 2, &cSplashToggles_m_NumDrops, 1, 0x1e, str_Max_Drops);
    cIntMenuItem_cIntMenuItem(&self->NumIceCrystalsItem, 3, &cSplashToggles_m_NumIceCrystals, 1, 0x1e, str_Max_Crystals);
    cFloatMenuItem_ctor(&self->PercentageIceCrystalItem, 4, &cSplashToggles_m_PercentageIceCrystal, 0.0f, 1.0f, str_Percentage_Ice_Crystals);
    cFloatMenuItem_ctor(&self->PercentageIceCrystalSpawnChanceItem, 5, &cSplashToggles_m_PercentageIceCrystalSpawnChance, 0.0f, 1.0f, str_Percentage_Ice_Crystal_Spawn);
    cIntMenuItem_cIntMenuItem(&self->IceCrystalMinGroupingItem, 6, &cSplashToggles_m_IceCrystalMinGrouping, 1, 10, str_Ice_Crystal_Minimum_in_Group);
    cIntMenuItem_cIntMenuItem(&self->IceCrystalMaxGroupingItem, 7, &cSplashToggles_m_IceCrystalMaxGrouping, 1, 10, str_Ice_Crystal_Maximum_in_Group);
    cIntMenuItem_cIntMenuItem(&self->MaxSpawnPerCrystalItem, 8, &cSplashToggles_m_MaxSpawnPerCrystal, 1, 10, str_Max_Spawn_Per_Crystal);
    cFloatMenuItem_ctor(&self->FarthestImpactDistanceItem, 9, &cSplashToggles_m_FarthestImpactDistance, 0.0f, 2000.0f, str_Farthest_Impact_Distance);
    cFloatMenuItem_ctor(&self->LowestImpactItensityItem, 10, &cSplashToggles_m_LowestImpactItensity, 0.0f, 120.0f, str_Lowest_Impact_Intensity);
    cFloatMenuItem_ctor(&self->MinimumSnowfallForImpactsItem, 11, &cSplashToggles_m_MinimumSnowfallForImpacts, 0.0f, 10.0f, str_Lowest_Snowfall_For_Impacts);
    cFloatMenuItem_ctor(&self->ImpactMultiplierItem, 0xc, &cSplashToggles_m_ImpactMultiplier, 0.0f, 10.0f, str_Impact_Multiplier);
    cFloatMenuItem_ctor(&self->LowestSnowfallAmountItem, 13, &cSplashToggles_m_LowestSnowfallAmount, 0.0f, 10.0f, str_Lowest_Snowfall_Amount);
    cFloatMenuItem_ctor(&self->SnowfallMultiplierItem, 0xe, &cSplashToggles_m_SnowfallMultiplier, 0.0f, 10.0f, str_Snowfall_Multiplier);
    cFloatMenuItem_ctor(&self->MinVelocityForPushoffItem, 0xf, &cSplashToggles_m_MinVelocityForPushoff, 0.0f, 10.0f, str_Min_Velocity_For_Pushoff);
    cFloatMenuItem_ctor(&self->DropMinLifeSpanItem, 0x10, &cSplashToggles_m_DropMinLifeSpan, 0.0f, 100.0f, str_Drop_Min_Life_Span);
    cFloatMenuItem_ctor(&self->DropMaxLifeSpanItem, 17, &cSplashToggles_m_DropMaxLifeSpan, 0.0f, 100.0f, str_Drop_Max_Life_Span);
    cFloatMenuItem_ctor(&self->DropMinSizeItem, 18, &cSplashToggles_m_DropMinSize, 0.0f, 40.0f, str_Drop_Min_Size);
    cFloatMenuItem_ctor(&self->DropMaxSizeItem, 19, &cSplashToggles_m_DropMaxSize, 0.0f, 40.0f, str_Drop_Max_Size);
    cFloatMenuItem_ctor(&self->DropMaxWidthHeightDifferenceItem, 20, &cSplashToggles_m_DropMaxWidthHeightDifference, 0.0f, 10.0f, str_Drop_Max_Width_Height_Difference);
    cFloatMenuItem_ctor(&self->DropMinVelocityJiggleXItem, 21, &cSplashToggles_m_DropMinVelocityJiggleX, -10.0f, 10.0f, str_Drop_Min_Velocity_Jiggle_X);
    cFloatMenuItem_ctor(&self->DropMaxVelocityJiggleXItem, 22, &cSplashToggles_m_DropMaxVelocityJiggleX, -10.0f, 10.0f, str_Drop_Max_Velocity_Jiggle_X);
    cFloatMenuItem_ctor(&self->DropMinVelocityJiggleYItem, 23, &cSplashToggles_m_DropMinVelocityJiggleY, -10.0f, 10.0f, str_Drop_Min_Velocity_Jiggle_Y);
    cFloatMenuItem_ctor(&self->DropMaxVelocityJiggleYItem, 0x18, &cSplashToggles_m_DropMaxVelocityJiggleY, -10.0f, 10.0f, str_Drop_Max_Velocity_Jiggle_Y);
    cFloatMenuItem_ctor(&self->DropMinSizeGrowthItem, 25, &cSplashToggles_m_DropMinSizeGrowth, -10.0f, 10.0f, str_Drop_Min_Size_Growth);
    cFloatMenuItem_ctor(&self->DropMaxSizeGrowthItem, 26, &cSplashToggles_m_DropMaxSizeGrowth, -10.0f, 10.0f, str_Drop_Max_Size_Growth);
    cFloatMenuItem_ctor(&self->DropMinPushOffSpeedItem, 27, &cSplashToggles_m_DropMinPushOffSpeed, 0.0f, 10.0f, str_Drop_Min_Push_Off_Speed);
    cFloatMenuItem_ctor(&self->DropMaxPushOffSpeedItem, 0x1c, &cSplashToggles_m_DropMaxPushOffSpeed, 0.0f, 10.0f, str_Drop_Max_Push_Off_Speed);
    cFloatMenuItem_ctor(&self->DropMaxVelocityItem, 29, &cSplashToggles_m_DropMaxVelocity, 0.0f, 100.0f, str_Drop_Maximum_Velocity);
    cFloatMenuItem_ctor(&self->DropMinVelocityUsageItem, 0x1e, &cSplashToggles_m_DropMinVelocityUsage, 0.0f, 10.0f, str_Drop_Min_Velocity_Usage);
    cFloatMenuItem_ctor(&self->DropMaxVelocityUsageItem, 0x1f, &cSplashToggles_m_DropMaxVelocityUsage, 0.0f, 10.0f, str_Drop_Max_Velocity_Usage);
    cFloatMenuItem_ctor(&self->DropMaxRotationChangeItem, 0x20, &cSplashToggles_m_DropMaxRotationChange, 0.0f, 720.0f, str_Crystal_Max_Rotation_Overall_Change);
    cARGBMenuItem_cARGBMenuItem(&self->DropTintMinItem, 33, str_Drop_Tint_Min, D_005046E0);
    cARGBMenuItem_cARGBMenuItem(&self->DropTintMaxItem, 34, str_Drop_Tint_Max, D_005046F0);
    cFloatMenuItem_ctor(&self->CrystalGroupSpacingXItem, 35, &cSplashToggles_m_CrystalGroupSpacingX, 0.0f, 100.0f, str_Crystal_Group_Spacing_X);
    cFloatMenuItem_ctor(&self->CrystalGroupSpacingYItem, 36, &cSplashToggles_m_CrystalGroupSpacingY, 0.0f, 100.0f, str_Crystal_Group_Spacing_Y);
    cFloatMenuItem_ctor(&self->CrystalMinLifeSpanItem, 37, &cSplashToggles_m_CrystalMinLifeSpan, 0.0f, 100.0f, str_Crystal_Min_Life_Span);
    cFloatMenuItem_ctor(&self->CrystalMaxLifeSpanItem, 38, &cSplashToggles_m_CrystalMaxLifeSpan, 0.0f, 100.0f, str_Crystal_Max_Life_Span);
    cFloatMenuItem_ctor(&self->CrystalMinSizeItem, 39, &cSplashToggles_m_CrystalMinSize, 0.0f, 40.0f, str_Crystal_Min_Size);
    cFloatMenuItem_ctor(&self->CrystalMaxSizeItem, 40, &cSplashToggles_m_CrystalMaxSize, 0.0f, 40.0f, str_Crystal_Max_Size);
    cFloatMenuItem_ctor(&self->CrystalMaxWidthHeightDifferenceItem, 41, &cSplashToggles_m_CrystalMaxWidthHeightDifference, 0.0f, 10.0f, str_Crystal_Max_Width_Height_Difference);
    cFloatMenuItem_ctor(&self->CrystalMinVelocityJiggleXItem, 42, &cSplashToggles_m_CrystalMinVelocityJiggleX, -10.0f, 10.0f, str_Crystal_Min_Velocity_Jiggle_X);
    cFloatMenuItem_ctor(&self->CrystalMaxVelocityJiggleXItem, 43, &cSplashToggles_m_CrystalMaxVelocityJiggleX, -10.0f, 10.0f, str_Crystal_Max_Velocity_Jiggle_X);
    cFloatMenuItem_ctor(&self->CrystalMinVelocityJiggleYItem, 44, &cSplashToggles_m_CrystalMinVelocityJiggleY, -10.0f, 10.0f, str_Crystal_Min_Velocity_Jiggle_Y);
    cFloatMenuItem_ctor(&self->CrystalMaxVelocityJiggleYItem, 45, &cSplashToggles_m_CrystalMaxVelocityJiggleY, -10.0f, 10.0f, str_Crystal_Max_Velocity_Jiggle_Y);
    cFloatMenuItem_ctor(&self->CrystalMinJiggleTimeItem, 46, &cSplashToggles_m_CrystalMinJiggleTime, 0.0f, 10.0f, str_Crsytal_Min_Time_In_One_Jiggle_Direction);
    cFloatMenuItem_ctor(&self->CrystalMaxJiggleTimeItem, 47, &cSplashToggles_m_CrystalMaxJiggleTime, 0.0f, 10.0f, str_Crsytal_Max_Time_In_One_Jiggle_Direction);
    cFloatMenuItem_ctor(&self->CrystalMinSizeGrowthItem, 0x30, &cSplashToggles_m_CrystalMinSizeGrowth, -10.0f, 10.0f, str_Crystal_Min_Size_Growth);
    cFloatMenuItem_ctor(&self->CrystalMaxSizeGrowthItem, 49, &cSplashToggles_m_CrystalMaxSizeGrowth, -10.0f, 10.0f, str_Crystal_Max_Size_Growth);
    cFloatMenuItem_ctor(&self->CrystalMinPushOffSpeedItem, 50, &cSplashToggles_m_CrystalMinPushOffSpeed, 0.0f, 10.0f, str_Crystal_Min_Push_Off_Speed);
    cFloatMenuItem_ctor(&self->CrystalMaxPushOffSpeedItem, 51, &cSplashToggles_m_CrystalMaxPushOffSpeed, 0.0f, 10.0f, str_Crystal_Max_Push_Off_Speed);
    cFloatMenuItem_ctor(&self->CrystalMaxVelocityItem, 52, &cSplashToggles_m_CrystalMaxVelocity, 0.0f, 100.0f, str_Crystal_Maximum_Velocity);
    cFloatMenuItem_ctor(&self->CrystalMaxRotationChangeItem, 53, &cSplashToggles_m_CrystalMaxRotationChange, 0.0f, 720.0f, str_Crystal_Max_Rotation_Overall_Change);
    cARGBMenuItem_cARGBMenuItem(&self->CrystalTintMinItem, 54, str_Crystal_Tint_Min, D_00504700);
    cARGBMenuItem_cARGBMenuItem(&self->CrystalTintMaxItem, 55, str_Crystal_Tint_Max, D_00504710);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->RenderItem, -1);
    cMenu_addItem(self, &self->NumDropsItem, -1);
    cMenu_addItem(self, &self->NumIceCrystalsItem, -1);
    cMenu_addItem(self, &self->PercentageIceCrystalItem, -1);
    cMenu_addItem(self, &self->PercentageIceCrystalSpawnChanceItem, -1);
    cMenu_addItem(self, &self->IceCrystalMinGroupingItem, -1);
    cMenu_addItem(self, &self->IceCrystalMaxGroupingItem, -1);
    cMenu_addItem(self, &self->MaxSpawnPerCrystalItem, -1);
    cMenu_addItem(self, &self->FarthestImpactDistanceItem, -1);
    cMenu_addItem(self, &self->LowestImpactItensityItem, -1);
    cMenu_addItem(self, &self->MinimumSnowfallForImpactsItem, -1);
    cMenu_addItem(self, &self->ImpactMultiplierItem, -1);
    cMenu_addItem(self, &self->LowestSnowfallAmountItem, -1);
    cMenu_addItem(self, &self->SnowfallMultiplierItem, -1);
    cMenu_addItem(self, &self->MinVelocityForPushoffItem, -1);
    cMenu_addItem(self, &self->DropMinLifeSpanItem, -1);
    cMenu_addItem(self, &self->DropMaxLifeSpanItem, -1);
    cMenu_addItem(self, &self->DropMinSizeItem, -1);
    cMenu_addItem(self, &self->DropMaxSizeItem, -1);
    cMenu_addItem(self, &self->DropMaxWidthHeightDifferenceItem, -1);
    cMenu_addItem(self, &self->DropMinVelocityJiggleXItem, -1);
    cMenu_addItem(self, &self->DropMaxVelocityJiggleXItem, -1);
    cMenu_addItem(self, &self->DropMinVelocityJiggleYItem, -1);
    cMenu_addItem(self, &self->DropMaxVelocityJiggleYItem, -1);
    cMenu_addItem(self, &self->DropMinSizeGrowthItem, -1);
    cMenu_addItem(self, &self->DropMaxSizeGrowthItem, -1);
    cMenu_addItem(self, &self->DropMinPushOffSpeedItem, -1);
    cMenu_addItem(self, &self->DropMaxPushOffSpeedItem, -1);
    cMenu_addItem(self, &self->DropMaxVelocityItem, -1);
    cMenu_addItem(self, &self->DropMinVelocityUsageItem, -1);
    cMenu_addItem(self, &self->DropMaxVelocityUsageItem, -1);
    cMenu_addItem(self, &self->DropMaxRotationChangeItem, -1);
    cMenu_addItem(self, &self->DropTintMinItem, -1);
    cMenu_addItem(self, &self->DropTintMaxItem, -1);
    cMenu_addItem(self, &self->CrystalGroupSpacingXItem, -1);
    cMenu_addItem(self, &self->CrystalGroupSpacingYItem, -1);
    cMenu_addItem(self, &self->CrystalMinLifeSpanItem, -1);
    cMenu_addItem(self, &self->CrystalMaxLifeSpanItem, -1);
    cMenu_addItem(self, &self->CrystalMinSizeItem, -1);
    cMenu_addItem(self, &self->CrystalMaxSizeItem, -1);
    cMenu_addItem(self, &self->CrystalMaxWidthHeightDifferenceItem, -1);
    cMenu_addItem(self, &self->CrystalMinVelocityJiggleXItem, -1);
    cMenu_addItem(self, &self->CrystalMaxVelocityJiggleXItem, -1);
    cMenu_addItem(self, &self->CrystalMinVelocityJiggleYItem, -1);
    cMenu_addItem(self, &self->CrystalMaxVelocityJiggleYItem, -1);
    cMenu_addItem(self, &self->CrystalMinJiggleTimeItem, -1);
    cMenu_addItem(self, &self->CrystalMaxJiggleTimeItem, -1);
    cMenu_addItem(self, &self->CrystalMinSizeGrowthItem, -1);
    cMenu_addItem(self, &self->CrystalMaxSizeGrowthItem, -1);
    cMenu_addItem(self, &self->CrystalMinPushOffSpeedItem, -1);
    cMenu_addItem(self, &self->CrystalMaxPushOffSpeedItem, -1);
    cMenu_addItem(self, &self->CrystalMaxVelocityItem, -1);
    cMenu_addItem(self, &self->CrystalMaxRotationChangeItem, -1);
    cMenu_addItem(self, &self->CrystalTintMinItem, -1);
    cMenu_addItem(self, &self->CrystalTintMaxItem, -1);
    return ((void*)self);
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
extern int cTube_m_Enable;
extern float cTube_m_ScaleWidth;
extern float cTube_m_ScaleHeight;
extern int cTube_m_R;
extern int cTube_m_G;
extern int cTube_m_B;

/**
 * Builds the "Tube Menu" debug menu: a title, a spacer and 6 items: enable, width and height scales and the
 * R, G and B colour.
 *
 * @return @p self
 */
extern "C" void* cTubeMenu_cTubeMenu(cTubeMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cTubeMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Tube_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cTube_m_Enable, 0, 1, str_Enable);
    cFloatMenuItem_cFloatMenuItem(&self->ScaleWidthItem, 1, &cTube_m_ScaleWidth, str_Scale_Width, 0.0f, 1000.0f);
    cFloatMenuItem_cFloatMenuItem(&self->ScaleHeightItem, 2, &cTube_m_ScaleHeight, str_Scale_Height, 0.0f, 1000.0f);
    cIntMenuItem_cIntMenuItem(&self->RItem, 3, &cTube_m_R, 0, 0xFF, D_004A2DA8);
    cIntMenuItem_cIntMenuItem(&self->GItem, 4, &cTube_m_G, 0, 0xFF, D_004A2DB0);
    cIntMenuItem_cIntMenuItem(&self->BItem, 5, &cTube_m_B, 0, 0xFF, D_004A2DB8);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->ScaleWidthItem, -1);
    cMenu_addItem(self, &self->ScaleHeightItem, -1);
    cMenu_addItem(self, &self->RItem, -1);
    cMenu_addItem(self, &self->GItem, -1);
    cMenu_addItem(self, &self->BItem, -1);
    return ((void*)self);
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
extern int cPathArrow_m_Enable;
extern float cPathArrow_m_ScaleForwardWidth;
extern float cPathArrow_m_ScaleMiddleWidth;
extern float cPathArrow_m_ScaleRearWidth;
extern float cPathArrow_m_ScaleFowardLength;
extern float cPathArrow_m_ScaleRearLength;
extern float cPathArrow_m_ScaleHeight;
extern int cPathArrow_m_R;
extern int cPathArrow_m_G;
extern int cPathArrow_m_B;
extern int cPathArrow_m_A;
extern float cPathArrow_m_MoveSpeed;
extern float cPathArrow_m_DrawSpeed;
extern float cPathArrow_m_Distance;
extern int cPathArrow_m_PathId;
extern int cPathArrow_m_MaxTick;

/**
 * Builds the "PathArrow Menu" debug menu: a title, a spacer and 16 items for the path arrow: enable, widths,
 * lengths, height, RGBA colour, speeds, distance, path ID and maximum tick.
 *
 * @return @p self
 */
extern "C" void* cPathArrowMenu_cPathArrowMenu(cPathArrowMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cPathArrowMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_PathArrow_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cPathArrow_m_Enable, 0, 1, str_Enable);
    cFloatMenuItem_ctor(&self->ScaleForwardWidthItem, 1, &cPathArrow_m_ScaleForwardWidth, 0.0f, 1000.0f, str_Forward_Width);
    cFloatMenuItem_ctor(&self->ScaleMiddleWidthItem, 2, &cPathArrow_m_ScaleMiddleWidth, 0.0f, 1000.0f, str_Middle_Width);
    cFloatMenuItem_ctor(&self->ScaleRearWidthItem, 3, &cPathArrow_m_ScaleRearWidth, 0.0f, 1000.0f, str_Rear_Width);
    cFloatMenuItem_ctor(&self->ScaleFowardLengthItem, 4, &cPathArrow_m_ScaleFowardLength, 0.0f, 1000.0f, str_Forward_Length);
    cFloatMenuItem_ctor(&self->ScaleRearLengthItem, 5, &cPathArrow_m_ScaleRearLength, 0.0f, 1000.0f, str_Rear_Length);
    cFloatMenuItem_ctor(&self->ScaleHeightItem, 6, &cPathArrow_m_ScaleHeight, 0.0f, 1000.0f, str_Height);
    cIntMenuItem_cIntMenuItem(&self->RItem, 7, &cPathArrow_m_R, 0, 0xff, D_004A2DA8);
    cIntMenuItem_cIntMenuItem(&self->GItem, 8, &cPathArrow_m_G, 0, 0xff, D_004A2DB0);
    cIntMenuItem_cIntMenuItem(&self->BItem, 9, &cPathArrow_m_B, 0, 0xff, D_004A2DB8);
    cIntMenuItem_cIntMenuItem(&self->AItem, 10, &cPathArrow_m_A, 0, 0xff, D_004A2DC8);
    cFloatMenuItem_ctor(&self->MoveSpeedItem, 11, &cPathArrow_m_MoveSpeed, 0.0f, 1000.0f, str_Move_Speed);
    cFloatMenuItem_ctor(&self->DrawSpeedItem, 0xc, &cPathArrow_m_DrawSpeed, 0.0f, 1000.0f, str_Draw_Speed);
    cFloatMenuItem_ctor(&self->DistanceItem, 13, &cPathArrow_m_Distance, 50.0f, 1000.0f, str_Distance);
    cIntMenuItem_cIntMenuItem(&self->PathIdItem, 0xe, &cPathArrow_m_PathId, 0, 1000, str_Path_Id);
    cIntMenuItem_cIntMenuItem(&self->MaxTickItem, 0xf, &cPathArrow_m_MaxTick, 1, 18000, str_Max_Tick);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->ScaleForwardWidthItem, -1);
    cMenu_addItem(self, &self->ScaleMiddleWidthItem, -1);
    cMenu_addItem(self, &self->ScaleRearWidthItem, -1);
    cMenu_addItem(self, &self->ScaleFowardLengthItem, -1);
    cMenu_addItem(self, &self->ScaleRearLengthItem, -1);
    cMenu_addItem(self, &self->ScaleHeightItem, -1);
    cMenu_addItem(self, &self->RItem, -1);
    cMenu_addItem(self, &self->GItem, -1);
    cMenu_addItem(self, &self->BItem, -1);
    cMenu_addItem(self, &self->AItem, -1);
    cMenu_addItem(self, &self->MoveSpeedItem, -1);
    cMenu_addItem(self, &self->DrawSpeedItem, -1);
    cMenu_addItem(self, &self->DistanceItem, -1);
    cMenu_addItem(self, &self->PathIdItem, -1);
    cMenu_addItem(self, &self->MaxTickItem, -1);
    return ((void*)self);
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
extern char __RenderTogglesMenu[];
extern char __VisualEffectsTestMenu[];
extern char __LightGlowMenu[];
extern char __PlantsMenu[];
extern char __DepthFogMenu[];
extern char __HeightFogMenu[];
extern char __DetailSystemMenu[];
extern char __ScreenTintMenu[];
extern char __ShadowMenu[];
extern char __SnowSurfaceMenu[];
extern char __BoardTrailMenu[];
extern char __FlagsMenu[];
extern char D_004CD840[];
extern char D_004CDC88[];
extern char __SplashTogglesMenu[];
extern char __TubeMenu[];
extern char __PathArrowMenu[];

/**
 * Builds the "Visual Effects Main Menu" debug menu: one submenu entry for each of the 17 visual effects
 * menus.
 *
 * @return @p self
 */
extern "C" void* cVisualEffectsMainMenu_cVisualEffectsMainMenu(cVisualEffectsMainMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cVisualEffectsMainMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Visual_Effects_Main_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cSubMenuItem_cSubMenuItem(&self->RenderTogglesItem, __RenderTogglesMenu, str_Render_Toggles);
    cSubMenuItem_cSubMenuItem(&self->VisualEffectsTestItem, __VisualEffectsTestMenu, str_Visual_Effects_Test);
    cSubMenuItem_cSubMenuItem(&self->LightGlowItem, __LightGlowMenu, str_Light_Glow);
    cSubMenuItem_cSubMenuItem(&self->PlantsItem, __PlantsMenu, str_Plants);
    cSubMenuItem_cSubMenuItem(&self->DepthFogItem, __DepthFogMenu, str_Depth_Fog);
    cSubMenuItem_cSubMenuItem(&self->HeightFogItem, __HeightFogMenu, str_Height_Fog);
    cSubMenuItem_cSubMenuItem(&self->DetailSystemItem, __DetailSystemMenu, str_Detail_System);
    cSubMenuItem_cSubMenuItem(&self->ScreenTintItem, __ScreenTintMenu, str_Screen_Tint);
    cSubMenuItem_cSubMenuItem(&self->ShadowItem, __ShadowMenu, str_Shadow_b);
    cSubMenuItem_cSubMenuItem(&self->SnowSurfaceItem, __SnowSurfaceMenu, str_Snow_Surface);
    cSubMenuItem_cSubMenuItem(&self->BoardTrailItem, __BoardTrailMenu, str_Board_Trail);
    cSubMenuItem_cSubMenuItem(&self->FlagsItem, __FlagsMenu, str_Flags);
    cSubMenuItem_cSubMenuItem(&self->PowerUpFXTogglesItem, D_004CD840, str_Power_Up_Effects_Toggles);
    cSubMenuItem_cSubMenuItem(&self->SnowfallTogglesItem, D_004CDC88, str_Snowfall_Test_Effects);
    cSubMenuItem_cSubMenuItem(&self->SplashTogglesItem, __SplashTogglesMenu, str_Camera_Snow_Splash_Effects);
    cSubMenuItem_cSubMenuItem(&self->TubeItem, __TubeMenu, str_Tube_Effects);
    cSubMenuItem_cSubMenuItem(&self->PathArrowItem, __PathArrowMenu, str_Path_Arrow_Effects);
    cMenu_addItem(self, &self->RenderTogglesItem, -1);
    cMenu_addItem(self, &self->VisualEffectsTestItem, -1);
    cMenu_addItem(self, &self->LightGlowItem, -1);
    cMenu_addItem(self, &self->PlantsItem, -1);
    cMenu_addItem(self, &self->DepthFogItem, -1);
    cMenu_addItem(self, &self->HeightFogItem, -1);
    cMenu_addItem(self, &self->DetailSystemItem, -1);
    cMenu_addItem(self, &self->ScreenTintItem, -1);
    cMenu_addItem(self, &self->ShadowItem, -1);
    cMenu_addItem(self, &self->SnowSurfaceItem, -1);
    cMenu_addItem(self, &self->BoardTrailItem, -1);
    cMenu_addItem(self, &self->FlagsItem, -1);
    cMenu_addItem(self, &self->PowerUpFXTogglesItem, -1);
    cMenu_addItem(self, &self->SnowfallTogglesItem, -1);
    cMenu_addItem(self, &self->SplashTogglesItem, -1);
    cMenu_addItem(self, &self->TubeItem, -1);
    cMenu_addItem(self, &self->PathArrowItem, -1);
    return ((void*)self);
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
extern "C" void cRenderTogglesMenu__cRenderTogglesMenu(cRenderTogglesMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->ShowSurfaceTypesItem, 2);
    cMenuItem__cMenuItem(&self->ProfileIndividualFXItem, 2);
    cMenuItem__cMenuItem(&self->DebugGameModeInfoItem, 2);
    cMenuItem__cMenuItem(&self->DebugNISMemInfoItem, 2);
    cMenuItem__cMenuItem(&self->DebugInfoItem, 2);
    cMenuItem__cMenuItem(&self->Prog2wItem, 2);
    cMenuItem__cMenuItem(&self->Prog2zItem, 2);
    cMenuItem__cMenuItem(&self->Prog2yItem, 2);
    cMenuItem__cMenuItem(&self->Prog2xItem, 2);
    cMenuItem__cMenuItem(&self->TempIntItem, 2);
    cMenuItem__cMenuItem(&self->SoftwareDynamicParticleSystemsItem, 2);
    cMenuItem__cMenuItem(&self->TestSurfaceTypesItem, 2);
    cMenuItem__cMenuItem(&self->ParticleSizeScaleItem, 2);
    cMenuItem__cMenuItem(&self->TestTunnelItem, 2);
    cMenuItem__cMenuItem(&self->DisableHUDItem, 2);
    cMenuItem__cMenuItem(&self->UseReflectionVectorItem, 2);
    cMenuItem__cMenuItem(&self->DisableBoundingBoxesItem, 2);
    cMenuItem__cMenuItem(&self->DisableAIVisualFXItem, 2);
    cMenuItem__cMenuItem(&self->DisableCharacterSpecularItem, 2);
    cMenuItem__cMenuItem(&self->DisableSkyBoxItem, 2);
    cMenuItem__cMenuItem(&self->DisableBoardWakeItem, 2);
    cMenuItem__cMenuItem(&self->DisableSnowFallItem, 2);
    cMenuItem__cMenuItem(&self->DisableVisibilityRectsItem, 2);
    cMenuItem__cMenuItem(&self->DisableAIBoardTrailItem, 2);
    cMenuItem__cMenuItem(&self->DisableBoardTrailItem, 2);
    cMenuItem__cMenuItem(&self->DisableShadowsItem, 2);
    cMenuItem__cMenuItem(&self->DisablePlayerStripsItem, 2);
    cMenuItem__cMenuItem(&self->DisableTextureRectsItem, 2);
    cMenuItem__cMenuItem(&self->Disable3DSpritesItem, 2);
    cMenuItem__cMenuItem(&self->DisableSpritesItem, 2);
    cMenuItem__cMenuItem(&self->DisableDynamicColourEmittersItem, 2);
    cMenuItem__cMenuItem(&self->DisableDynamicEmittersItem, 2);
    cMenuItem__cMenuItem(&self->DisableStaticEmittersItem, 2);
    cMenuItem__cMenuItem(&self->DisableMeshReflectionItem, 2);
    cMenuItem__cMenuItem(&self->DisableMeshesItem, 2);
    cMenuItem__cMenuItem(&self->DisableTransformStripsItem, 2);
    cMenuItem__cMenuItem(&self->DisableTriStripsItem, 2);
    cMenuItem__cMenuItem(&self->DisableLineStripsItem, 2);
    cMenuItem__cMenuItem(&self->DisableSparklesItem, 2);
    cMenuItem__cMenuItem(&self->DisablePatchShadowItem, 2);
    cMenuItem__cMenuItem(&self->DisablePatchReflectionItem, 2);
    cMenuItem__cMenuItem(&self->DisablePatchesItem, 2);
    cMenuItem__cMenuItem(&self->UseSmallTexture1Item, 2);
    cMenuItem__cMenuItem(&self->UseSmallTexture0Item, 2);
    cMenuItem__cMenuItem(&self->ResetStatsItem, 2);
    cMenuItem__cMenuItem(&self->DisplayRenderStatsItem, 2);
    cMenuItem__cMenuItem(&self->AIUpdatesPerRenderItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cVisualEffectsTestMenu__cVisualEffectsTestMenu(cVisualEffectsTestMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->LayerIdItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->LayerOffsetItem, 2);
    cMenuItem__cMenuItem(&self->BlueItem, 2);
    cMenuItem__cMenuItem(&self->GreenItem, 2);
    cMenuItem__cMenuItem(&self->RedItem, 2);
    cMenuItem__cMenuItem(&self->AlphaItem, 2);
    cMenuItem__cMenuItem(&self->ScaleYItem, 2);
    cMenuItem__cMenuItem(&self->ScaleXItem, 2);
    cMenuItem__cMenuItem(&self->SizeYItem, 2);
    cMenuItem__cMenuItem(&self->SizeXItem, 2);
    cMenuItem__cMenuItem(&self->PositionYItem, 2);
    cMenuItem__cMenuItem(&self->PositionXItem, 2);
    cMenuItem__cMenuItem(&self->CurrElementItem, 2);
    cMenuItem__cMenuItem(&self->HUDPrintItem, 2);
    cMenuItem__cMenuItem(&self->ShowCurrentElementItem, 2);
    cMenuItem__cMenuItem(&self->HUDEnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cLightGlowMenu__cLightGlowMenu(cLightGlowMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->IntensityScaleItem, 2);
    cMenuItem__cMenuItem(&self->IntensityAItem, 2);
    cMenuItem__cMenuItem(&self->IntensityBItem, 2);
    cMenuItem__cMenuItem(&self->IntensityGItem, 2);
    cMenuItem__cMenuItem(&self->IntensityRItem, 2);
    cMenuItem__cMenuItem(&self->ClearAlphaItem, 2);
    cMenuItem__cMenuItem(&self->FadeRateItem, 2);
    cMenuItem__cMenuItem(&self->SizeAmbientItem, 2);
    cMenuItem__cMenuItem(&self->SizeScaleItem, 2);
    cMenuItem__cMenuItem(&self->SizePowerItem, 2);
    cMenuItem__cMenuItem(&self->ColourAmbientItem, 2);
    cMenuItem__cMenuItem(&self->ColourScaleItem, 2);
    cMenuItem__cMenuItem(&self->ColourPowerItem, 2);
    cMenuItem__cMenuItem(&self->CutoffItem, 2);
    cMenuItem__cMenuItem(&self->SizeItem, 2);
    cMenuItem__cMenuItem(&self->PS2ShowTexturesItem, 2);
    cMenuItem__cMenuItem(&self->PS2JitterTexItem, 2);
    cMenuItem__cMenuItem(&self->PS2IntensityTex3Item, 2);
    cMenuItem__cMenuItem(&self->PS2IntensityTex2Item, 2);
    cMenuItem__cMenuItem(&self->PS2IntensityTex1Item, 2);
    cMenuItem__cMenuItem(&self->PS2IntensityTex0Item, 2);
    cMenuItem__cMenuItem(&self->PS2FrameDstScaleItem, 2);
    cMenuItem__cMenuItem(&self->PS2FrameSrcScaleItem, 2);
    cMenuItem__cMenuItem(&self->PS2CopyIntensityItem, 2);
    cMenuItem__cMenuItem(&self->PS2ModIntensityItem, 2);
    cMenuItem__cMenuItem(&self->PS2MinIntensityItem, 2);
    cMenuItem__cMenuItem(&self->PS2CaptureSizeItem, 2);
    cMenuItem__cMenuItem(&self->OverrideWPItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cDepthFogMenu__cDepthFogMenu(cDepthFogMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->ColourItem.TitleItem, 2);
    cMenuItem__cMenuItem(&self->ColourItem.Blue, 2);
    cMenuItem__cMenuItem(&self->ColourItem.Green, 2);
    cMenuItem__cMenuItem(&self->ColourItem.Red, 2);
    cMenuItem__cMenuItem(&self->ColourItem.Alpha, 2);
    dtorSubMenuItem_24DC68(&self->ColourItem);
    cMenuItem__cMenuItem(&self->XBoxDensityScaleItem, 2);
    cMenuItem__cMenuItem(&self->DensityItem, 2);
    cMenuItem__cMenuItem(&self->ExponentItem, 2);
    cMenuItem__cMenuItem(&self->FarPlaneItem, 2);
    cMenuItem__cMenuItem(&self->NearPlaneItem, 2);
    cMenuItem__cMenuItem(&self->OverrideWPItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cScreenTintMenu__cScreenTintMenu(cScreenTintMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->GammaFillBItem, 2);
    cMenuItem__cMenuItem(&self->GammaFillGItem, 2);
    cMenuItem__cMenuItem(&self->GammaFillRItem, 2);
    cMenuItem__cMenuItem(&self->GammaColourBItem, 2);
    cMenuItem__cMenuItem(&self->GammaColourGItem, 2);
    cMenuItem__cMenuItem(&self->GammaColourRItem, 2);
    cMenuItem__cMenuItem(&self->GammaPowerItem, 2);
    cMenuItem__cMenuItem(&self->GammaEnableItem, 2);
    cMenuItem__cMenuItem(&self->LightningColour3BItem, 2);
    cMenuItem__cMenuItem(&self->LightningColour3GItem, 2);
    cMenuItem__cMenuItem(&self->LightningColour3RItem, 2);
    cMenuItem__cMenuItem(&self->LightningColour2BItem, 2);
    cMenuItem__cMenuItem(&self->LightningColour2GItem, 2);
    cMenuItem__cMenuItem(&self->LightningColour2RItem, 2);
    cMenuItem__cMenuItem(&self->LightningColourBItem, 2);
    cMenuItem__cMenuItem(&self->LightningColourGItem, 2);
    cMenuItem__cMenuItem(&self->LightningColourRItem, 2);
    cMenuItem__cMenuItem(&self->LightningScale3Item, 2);
    cMenuItem__cMenuItem(&self->LightningScale2Item, 2);
    cMenuItem__cMenuItem(&self->LightningScaleItem, 2);
    cMenuItem__cMenuItem(&self->LightningFadeIn3Item, 2);
    cMenuItem__cMenuItem(&self->LightningFadeIn2Item, 2);
    cMenuItem__cMenuItem(&self->LightningFadeOutItem, 2);
    cMenuItem__cMenuItem(&self->LightningFadeInItem, 2);
    cMenuItem__cMenuItem(&self->LightningChanceItem, 2);
    cMenuItem__cMenuItem(&self->FrameIdItem, 2);
    cMenuItem__cMenuItem(&self->LightningEnableItem, 2);
    cMenuItem__cMenuItem(&self->XNLerpItem, 2);
    cMenuItem__cMenuItem(&self->XNColourBItem, 2);
    cMenuItem__cMenuItem(&self->XNColourGItem, 2);
    cMenuItem__cMenuItem(&self->XNColourRItem, 2);
    cMenuItem__cMenuItem(&self->FillBItem, 2);
    cMenuItem__cMenuItem(&self->FillGItem, 2);
    cMenuItem__cMenuItem(&self->FillRItem, 2);
    cMenuItem__cMenuItem(&self->ColourBItem, 2);
    cMenuItem__cMenuItem(&self->ColourGItem, 2);
    cMenuItem__cMenuItem(&self->ColourRItem, 2);
    cMenuItem__cMenuItem(&self->DisplayValuesItem, 2);
    cMenuItem__cMenuItem(&self->OverrideWPItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cBoardTrailMenu__cBoardTrailMenu(cBoardTrailMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->UseSliceCapsItem, 2);
    cMenuItem__cMenuItem(&self->DrawFixupLayerItem, 2);
    cMenuItem__cMenuItem(&self->NumFadeSegmentsItem, 2);
    cMenuItem__cMenuItem(&self->BaseNormalOffsetItem, 2);
    cMenuItem__cMenuItem(&self->InnerNormalOffsetItem, 2);
    cMenuItem__cMenuItem(&self->OuterNormalOffsetItem, 2);
    cMenuItem__cMenuItem(&self->OuterUItem, 2);
    cMenuItem__cMenuItem(&self->InnerUItem, 2);
    cMenuItem__cMenuItem(&self->BaseUItem, 2);
    cMenuItem__cMenuItem(&self->InnerPositionJitterItem, 2);
    cMenuItem__cMenuItem(&self->TopBiasClearItem, 2);
    cMenuItem__cMenuItem(&self->TopBiasBehindItem, 2);
    cMenuItem__cMenuItem(&self->TopBiasFrontItem, 2);
    cMenuItem__cMenuItem(&self->DeepPowderDepthScaleItem, 2);
    cMenuItem__cMenuItem(&self->PowderDepthScaleItem, 2);
    cMenuItem__cMenuItem(&self->LooseDepthScaleItem, 2);
    cMenuItem__cMenuItem(&self->PackedDepthScaleItem, 2);
    cMenuItem__cMenuItem(&self->Depth2HeightScaleItem, 2);
    cMenuItem__cMenuItem(&self->HeightScaleItem, 2);
    cMenuItem__cMenuItem(&self->OuterWidthScaleItem, 2);
    cMenuItem__cMenuItem(&self->InnerWidthScaleItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cPowerUpFXTogglesMenu__cPowerUpFXTogglesMenu(cPowerUpFXTogglesMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->TestOrangeRibbonItem, 2);
    cMenuItem__cMenuItem(&self->TestRedRibbonItem, 2);
    cMenuItem__cMenuItem(&self->TestSpeedBoostRibbonItem, 2);
    cMenuItem__cMenuItem(&self->SuperUberStateTestItem, 2);
    cMenuItem__cMenuItem(&self->UberStateTestItem, 2);
    cMenuItem__cMenuItem(&self->EnableSpeedBoostFXItem, 2);
    cMenuItem__cMenuItem(&self->TestTrickBoostItem, 2);
    cMenuItem__cMenuItem(&self->MinSpeedBoostTimeBeforeBlurItem, 2);
    cMenuItem__cMenuItem(&self->SpeedBoostMaxSpeedItem, 2);
    cMenuItem__cMenuItem(&self->SpeedBoostMinSpeedItem, 2);
    cMenuItem__cMenuItem(&self->SpeedBoostScreenBlurDecreaseRateItem, 2);
    cMenuItem__cMenuItem(&self->SpeedBoostScreenBlurIncreaseRateItem, 2);
    cMenuItem__cMenuItem(&self->SpeedBoostScreenBlurMaxPercentItem, 2);
    cMenuItem__cMenuItem(&self->EnableSpeedBoostScreenBlurItem, 2);
    cMenuItem__cMenuItem(&self->EnableSpeedBoostStreamersItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cSnowfallTogglesMenu__cSnowfallTogglesMenu(cSnowfallTogglesMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->TestAdditiveItem, 2);
    cMenuItem__cMenuItem(&self->SnowFluffColourItem.TitleItem, 2);
    cMenuItem__cMenuItem(&self->SnowFluffColourItem.Blue, 2);
    cMenuItem__cMenuItem(&self->SnowFluffColourItem.Green, 2);
    cMenuItem__cMenuItem(&self->SnowFluffColourItem.Red, 2);
    cMenuItem__cMenuItem(&self->SnowFluffColourItem.Alpha, 2);
    dtorSubMenuItem_24E200(&self->SnowFluffColourItem);
    cMenuItem__cMenuItem(&self->SnowFlakeColourItem.TitleItem, 2);
    cMenuItem__cMenuItem(&self->SnowFlakeColourItem.Blue, 2);
    cMenuItem__cMenuItem(&self->SnowFlakeColourItem.Green, 2);
    cMenuItem__cMenuItem(&self->SnowFlakeColourItem.Red, 2);
    cMenuItem__cMenuItem(&self->SnowFlakeColourItem.Alpha, 2);
    dtorSubMenuItem_24E200(&self->SnowFlakeColourItem);
    cMenuItem__cMenuItem(&self->SnowFluffWindIntensityItem, 2);
    cMenuItem__cMenuItem(&self->SnowFluffItensityItem, 2);
    cMenuItem__cMenuItem(&self->SnowGravityMultiplierItem, 2);
    cMenuItem__cMenuItem(&self->TestSnowFlurriesItem, 2);
    cMenuItem__cMenuItem(&self->TestWindRotationItem, 2);
    cMenuItem__cMenuItem(&self->TestFlakeSizeItem, 2);
    cMenuItem__cMenuItem(&self->TestWindIntensityItem, 2);
    cMenuItem__cMenuItem(&self->TestSnowIntensityItem, 2);
    cMenuItem__cMenuItem(&self->EnableTestItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cSplashTogglesMenu__cSplashTogglesMenu(cSplashTogglesMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->CrystalTintMaxItem.TitleItem, 2);
    cMenuItem__cMenuItem(&self->CrystalTintMaxItem.Blue, 2);
    cMenuItem__cMenuItem(&self->CrystalTintMaxItem.Green, 2);
    cMenuItem__cMenuItem(&self->CrystalTintMaxItem.Red, 2);
    cMenuItem__cMenuItem(&self->CrystalTintMaxItem.Alpha, 2);
    dtorSubMenuItem_24E388(&self->CrystalTintMaxItem);
    cMenuItem__cMenuItem(&self->CrystalTintMinItem.TitleItem, 2);
    cMenuItem__cMenuItem(&self->CrystalTintMinItem.Blue, 2);
    cMenuItem__cMenuItem(&self->CrystalTintMinItem.Green, 2);
    cMenuItem__cMenuItem(&self->CrystalTintMinItem.Red, 2);
    cMenuItem__cMenuItem(&self->CrystalTintMinItem.Alpha, 2);
    dtorSubMenuItem_24E388(&self->CrystalTintMinItem);
    cMenuItem__cMenuItem(&self->CrystalMaxRotationChangeItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMaxVelocityItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMaxPushOffSpeedItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMinPushOffSpeedItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMaxSizeGrowthItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMinSizeGrowthItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMaxJiggleTimeItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMinJiggleTimeItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMaxVelocityJiggleYItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMinVelocityJiggleYItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMaxVelocityJiggleXItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMinVelocityJiggleXItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMaxWidthHeightDifferenceItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMaxSizeItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMinSizeItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMaxLifeSpanItem, 2);
    cMenuItem__cMenuItem(&self->CrystalMinLifeSpanItem, 2);
    cMenuItem__cMenuItem(&self->CrystalGroupSpacingYItem, 2);
    cMenuItem__cMenuItem(&self->CrystalGroupSpacingXItem, 2);
    cMenuItem__cMenuItem(&self->DropTintMaxItem.TitleItem, 2);
    cMenuItem__cMenuItem(&self->DropTintMaxItem.Blue, 2);
    cMenuItem__cMenuItem(&self->DropTintMaxItem.Green, 2);
    cMenuItem__cMenuItem(&self->DropTintMaxItem.Red, 2);
    cMenuItem__cMenuItem(&self->DropTintMaxItem.Alpha, 2);
    dtorSubMenuItem_24E388(&self->DropTintMaxItem);
    cMenuItem__cMenuItem(&self->DropTintMinItem.TitleItem, 2);
    cMenuItem__cMenuItem(&self->DropTintMinItem.Blue, 2);
    cMenuItem__cMenuItem(&self->DropTintMinItem.Green, 2);
    cMenuItem__cMenuItem(&self->DropTintMinItem.Red, 2);
    cMenuItem__cMenuItem(&self->DropTintMinItem.Alpha, 2);
    dtorSubMenuItem_24E388(&self->DropTintMinItem);
    cMenuItem__cMenuItem(&self->DropMaxRotationChangeItem, 2);
    cMenuItem__cMenuItem(&self->DropMaxVelocityUsageItem, 2);
    cMenuItem__cMenuItem(&self->DropMinVelocityUsageItem, 2);
    cMenuItem__cMenuItem(&self->DropMaxVelocityItem, 2);
    cMenuItem__cMenuItem(&self->DropMaxPushOffSpeedItem, 2);
    cMenuItem__cMenuItem(&self->DropMinPushOffSpeedItem, 2);
    cMenuItem__cMenuItem(&self->DropMaxSizeGrowthItem, 2);
    cMenuItem__cMenuItem(&self->DropMinSizeGrowthItem, 2);
    cMenuItem__cMenuItem(&self->DropMaxVelocityJiggleYItem, 2);
    cMenuItem__cMenuItem(&self->DropMinVelocityJiggleYItem, 2);
    cMenuItem__cMenuItem(&self->DropMaxVelocityJiggleXItem, 2);
    cMenuItem__cMenuItem(&self->DropMinVelocityJiggleXItem, 2);
    cMenuItem__cMenuItem(&self->DropMaxWidthHeightDifferenceItem, 2);
    cMenuItem__cMenuItem(&self->DropMaxSizeItem, 2);
    cMenuItem__cMenuItem(&self->DropMinSizeItem, 2);
    cMenuItem__cMenuItem(&self->DropMaxLifeSpanItem, 2);
    cMenuItem__cMenuItem(&self->DropMinLifeSpanItem, 2);
    cMenuItem__cMenuItem(&self->MinVelocityForPushoffItem, 2);
    cMenuItem__cMenuItem(&self->SnowfallMultiplierItem, 2);
    cMenuItem__cMenuItem(&self->LowestSnowfallAmountItem, 2);
    cMenuItem__cMenuItem(&self->ImpactMultiplierItem, 2);
    cMenuItem__cMenuItem(&self->MinimumSnowfallForImpactsItem, 2);
    cMenuItem__cMenuItem(&self->LowestImpactItensityItem, 2);
    cMenuItem__cMenuItem(&self->FarthestImpactDistanceItem, 2);
    cMenuItem__cMenuItem(&self->MaxSpawnPerCrystalItem, 2);
    cMenuItem__cMenuItem(&self->IceCrystalMaxGroupingItem, 2);
    cMenuItem__cMenuItem(&self->IceCrystalMinGroupingItem, 2);
    cMenuItem__cMenuItem(&self->PercentageIceCrystalSpawnChanceItem, 2);
    cMenuItem__cMenuItem(&self->PercentageIceCrystalItem, 2);
    cMenuItem__cMenuItem(&self->NumIceCrystalsItem, 2);
    cMenuItem__cMenuItem(&self->NumDropsItem, 2);
    cMenuItem__cMenuItem(&self->RenderItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cPathArrowMenu__cPathArrowMenu(cPathArrowMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->MaxTickItem, 2);
    cMenuItem__cMenuItem(&self->PathIdItem, 2);
    cMenuItem__cMenuItem(&self->DistanceItem, 2);
    cMenuItem__cMenuItem(&self->DrawSpeedItem, 2);
    cMenuItem__cMenuItem(&self->MoveSpeedItem, 2);
    cMenuItem__cMenuItem(&self->AItem, 2);
    cMenuItem__cMenuItem(&self->BItem, 2);
    cMenuItem__cMenuItem(&self->GItem, 2);
    cMenuItem__cMenuItem(&self->RItem, 2);
    cMenuItem__cMenuItem(&self->ScaleHeightItem, 2);
    cMenuItem__cMenuItem(&self->ScaleRearLengthItem, 2);
    cMenuItem__cMenuItem(&self->ScaleFowardLengthItem, 2);
    cMenuItem__cMenuItem(&self->ScaleRearWidthItem, 2);
    cMenuItem__cMenuItem(&self->ScaleMiddleWidthItem, 2);
    cMenuItem__cMenuItem(&self->ScaleForwardWidthItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void* cRenderTogglesMenu_cRenderTogglesMenu(cRenderTogglesMenu* self);
extern "C" void cRenderTogglesMenu__cRenderTogglesMenu(cRenderTogglesMenu* self, int flags);
extern "C" void* cVisualEffectsTestMenu_cVisualEffectsTestMenu(cVisualEffectsTestMenu* self);
extern "C" void cVisualEffectsTestMenu__cVisualEffectsTestMenu(cVisualEffectsTestMenu* self, int flags);
extern "C" void* cLightGlowMenu_cLightGlowMenu(cLightGlowMenu* self);
extern "C" void cLightGlowMenu__cLightGlowMenu(cLightGlowMenu* self, int flags);
extern "C" void* cPlantsMenu_cPlantsMenu(cPlantsMenu* self);
extern "C" void* cDepthFogMenu_cDepthFogMenu(cDepthFogMenu* self);
extern "C" void cDepthFogMenu__cDepthFogMenu(cDepthFogMenu* self, int flags);
extern "C" void* cHeightFogMenu_cHeightFogMenu(cHeightFogMenu* self);
extern "C" void* cDetailSystemMenu_cDetailSystemMenu(cDetailSystemMenu* self);
extern "C" void* cScreenTintMenu_cScreenTintMenu(cScreenTintMenu* self);
extern "C" void cScreenTintMenu__cScreenTintMenu(cScreenTintMenu* self, int flags);
extern "C" void* cShadowMenu_cShadowMenu(cShadowMenu* self);
extern "C" void* cSnowSurfaceMenu_cSnowSurfaceMenu(cSnowSurfaceMenu* self);
extern "C" void* cBoardTrailMenu_cBoardTrailMenu(cBoardTrailMenu* self);
extern "C" void cBoardTrailMenu__cBoardTrailMenu(cBoardTrailMenu* self, int flags);
extern "C" void* cFlagsMenu_cFlagsMenu(cFlagsMenu* self);
extern "C" void* cPowerUpFXTogglesMenu_cPowerUpFXTogglesMenu(cPowerUpFXTogglesMenu* self);
extern "C" void cPowerUpFXTogglesMenu__cPowerUpFXTogglesMenu(cPowerUpFXTogglesMenu* self, int flags);
extern "C" void* cSnowfallTogglesMenu_cSnowfallTogglesMenu(cSnowfallTogglesMenu* self);
extern "C" void cSnowfallTogglesMenu__cSnowfallTogglesMenu(cSnowfallTogglesMenu* self, int flags);
extern "C" void* cSplashTogglesMenu_cSplashTogglesMenu(cSplashTogglesMenu* self);
extern "C" void cSplashTogglesMenu__cSplashTogglesMenu(cSplashTogglesMenu* self, int flags);
extern "C" void* cTubeMenu_cTubeMenu(cTubeMenu* self);
extern "C" void* cPathArrowMenu_cPathArrowMenu(cPathArrowMenu* self);
extern "C" void cPathArrowMenu__cPathArrowMenu(cPathArrowMenu* self, int flags);

// cPlantsMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cPlantsMenu_dtor(void* vself)
{
    cPlantsMenu* self = (cPlantsMenu*)vself;
    cMenuItem__cMenuItem(&self->BranchDirectionItem, 2);
    cMenuItem__cMenuItem(&self->SwayDirectionItem, 2);
    cMenuItem__cMenuItem(&self->BranchFrequency2Item, 2);
    cMenuItem__cMenuItem(&self->BranchFrequencyItem, 2);
    cMenuItem__cMenuItem(&self->RotationFrequencyItem, 2);
    cMenuItem__cMenuItem(&self->SwayFrequencyItem, 2);
    cMenuItem__cMenuItem(&self->PositionScaleItem, 2);
    cMenuItem__cMenuItem(&self->RotationAmplitudeItem, 2);
    cMenuItem__cMenuItem(&self->BranchAmplitudeItem, 2);
    cMenuItem__cMenuItem(&self->SwayOffsetItem, 2);
    cMenuItem__cMenuItem(&self->SwayAmplitudeItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
    cMenu__cMenu(self, 2);
}

// cHeightFogMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cHeightFogMenu_dtor(void* vself)
{
    cHeightFogMenu* self = (cHeightFogMenu*)vself;
    cMenuItem__cMenuItem(&self->ThicknessItem, 2);
    cMenuItem__cMenuItem(&self->StartHeightItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
    cMenu__cMenu(self, 2);
}

// cDetailSystemMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cDetailSystemMenu_dtor(void* vself)
{
    cDetailSystemMenu* self = (cDetailSystemMenu*)vself;
    cMenuItem__cMenuItem(&self->XBoxMaxSizeItem, 2);
    cMenuItem__cMenuItem(&self->XBoxMinSizeItem, 2);
    cMenuItem__cMenuItem(&self->XBoxSizeItem, 2);
    cMenuItem__cMenuItem(&self->XBoxNumSparklesItem, 2);
    cMenuItem__cMenuItem(&self->PS2MaxSparkleSizeItem, 2);
    cMenuItem__cMenuItem(&self->PS2MinSparkleSizeItem, 2);
    cMenuItem__cMenuItem(&self->PS2SparkleSizeItem, 2);
    cMenuItem__cMenuItem(&self->PS2SparkleCountItem, 2);
    cMenuItem__cMenuItem(&self->SpinScaleItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
    cMenu__cMenu(self, 2);
}

// cShadowMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cShadowMenu_dtor(void* vself)
{
    cShadowMenu* self = (cShadowMenu*)vself;
    cMenuItem__cMenuItem(&self->EndUVItem, 2);
    cMenuItem__cMenuItem(&self->StartUVItem, 2);
    cMenuItem__cMenuItem(&self->IntensityItem, 2);
    cMenuItem__cMenuItem(&self->LengthItem, 2);
    cMenuItem__cMenuItem(&self->ShadowDirZItem, 2);
    cMenuItem__cMenuItem(&self->ShadowDirYItem, 2);
    cMenuItem__cMenuItem(&self->ShadowDirXItem, 2);
    cMenuItem__cMenuItem(&self->ShowShadowPolysItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
    cMenu__cMenu(self, 2);
}

// cSnowSurfaceMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cSnowSurfaceMenu_dtor(void* vself)
{
    cSnowSurfaceMenu* self = (cSnowSurfaceMenu*)vself;
    cMenuItem__cMenuItem(&self->MaxColourScaleItem, 2);
    cMenuItem__cMenuItem(&self->MinColourScaleItem, 2);
    cMenuItem__cMenuItem(&self->NoiseItem, 2);
    cMenuItem__cMenuItem(&self->BumpItem, 2);
    cMenuItem__cMenuItem(&self->BaseItem, 2);
    cMenuItem__cMenuItem(&self->EnablePatchReflectItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
    cMenu__cMenu(self, 2);
}

// cFlagsMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cFlagsMenu_dtor(void* vself)
{
    cFlagsMenu* self = (cFlagsMenu*)vself;
    cMenuItem__cMenuItem(&self->EnableRenderItem, 2);
    cMenuItem__cMenuItem(&self->EnableUpdateItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
    cMenu__cMenu(self, 2);
}

// cTubeMenu's destructor, inlined here (no out-of-line copy is called).
static inline void cTubeMenu_dtor(void* vself)
{
    cTubeMenu* self = (cTubeMenu*)vself;
    cMenuItem__cMenuItem(&self->BItem, 2);
    cMenuItem__cMenuItem(&self->GItem, 2);
    cMenuItem__cMenuItem(&self->RItem, 2);
    cMenuItem__cMenuItem(&self->ScaleHeightItem, 2);
    cMenuItem__cMenuItem(&self->ScaleWidthItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
    cMenu__cMenu(self, 2);
}

extern sV4_E8D8 cfm_sav_Render_Halo_SUN_kSunColour;
extern sV4_E8D8 cfm_sav_Render_Halo_SUN_LENS0_kLens0Colour;
extern sV4_E8D8 cfm_sav_Render_Halo_SUN_LENS1_kLens1Colour;
extern sV4_E8D8 cfm_sav_Render_Halo_SUN_LENS2_kLens2Colour;
extern sV4_E8D8 cfm_sav_Render_Halo_SUN_LENS3_kLens3Colour;
extern sV4_E8D8 cfm_sav_Render_Halo_SUN_LENS4_kLens4Colour;
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
extern char __RenderTogglesMenu[], __VisualEffectsTestMenu[], __LightGlowMenu[], __PlantsMenu[], __DepthFogMenu[], __HeightFogMenu[], __DetailSystemMenu[], __ScreenTintMenu[], __ShadowMenu[], __SnowSurfaceMenu[], __BoardTrailMenu[], __FlagsMenu[], D_004CD840[], D_004CDC88[], __SplashTogglesMenu[], __TubeMenu[], __PathArrowMenu[];
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
            new (&cfm_sav_Render_Halo_SUN_kSunColour) sV4_E8D8(1.0f, 1.0f, 1.0f, 0.5f);
            new (&cfm_sav_Render_Halo_SUN_LENS0_kLens0Colour) sV4_E8D8(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
            new (&cfm_sav_Render_Halo_SUN_LENS1_kLens1Colour) sV4_E8D8(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
            new (&cfm_sav_Render_Halo_SUN_LENS2_kLens2Colour) sV4_E8D8(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
            new (&cfm_sav_Render_Halo_SUN_LENS3_kLens3Colour) sV4_E8D8(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
            new (&cfm_sav_Render_Halo_SUN_LENS4_kLens4Colour) sV4_E8D8(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
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
            cRenderTogglesMenu_cRenderTogglesMenu((cRenderTogglesMenu*)__RenderTogglesMenu);
            cVisualEffectsTestMenu_cVisualEffectsTestMenu((cVisualEffectsTestMenu*)__VisualEffectsTestMenu);
            cLightGlowMenu_cLightGlowMenu((cLightGlowMenu*)__LightGlowMenu);
            cPlantsMenu_cPlantsMenu((cPlantsMenu*)__PlantsMenu);
            cDepthFogMenu_cDepthFogMenu((cDepthFogMenu*)__DepthFogMenu);
            cHeightFogMenu_cHeightFogMenu((cHeightFogMenu*)__HeightFogMenu);
            cDetailSystemMenu_cDetailSystemMenu((cDetailSystemMenu*)__DetailSystemMenu);
            cScreenTintMenu_cScreenTintMenu((cScreenTintMenu*)__ScreenTintMenu);
            cShadowMenu_cShadowMenu((cShadowMenu*)__ShadowMenu);
            cSnowSurfaceMenu_cSnowSurfaceMenu((cSnowSurfaceMenu*)__SnowSurfaceMenu);
            cBoardTrailMenu_cBoardTrailMenu((cBoardTrailMenu*)__BoardTrailMenu);
            cFlagsMenu_cFlagsMenu((cFlagsMenu*)__FlagsMenu);
            cPowerUpFXTogglesMenu_cPowerUpFXTogglesMenu((cPowerUpFXTogglesMenu*)D_004CD840);
            cSnowfallTogglesMenu_cSnowfallTogglesMenu((cSnowfallTogglesMenu*)D_004CDC88);
            cSplashTogglesMenu_cSplashTogglesMenu((cSplashTogglesMenu*)__SplashTogglesMenu);
            cTubeMenu_cTubeMenu((cTubeMenu*)__TubeMenu);
            cPathArrowMenu_cPathArrowMenu((cPathArrowMenu*)__PathArrowMenu);
        } else {
            cPathArrowMenu__cPathArrowMenu((cPathArrowMenu*)__PathArrowMenu, 2);
            cTubeMenu_dtor(__TubeMenu);
            cSplashTogglesMenu__cSplashTogglesMenu((cSplashTogglesMenu*)__SplashTogglesMenu, 2);
            cSnowfallTogglesMenu__cSnowfallTogglesMenu((cSnowfallTogglesMenu*)D_004CDC88, 2);
            cPowerUpFXTogglesMenu__cPowerUpFXTogglesMenu((cPowerUpFXTogglesMenu*)D_004CD840, 2);
            cFlagsMenu_dtor(__FlagsMenu);
            cBoardTrailMenu__cBoardTrailMenu((cBoardTrailMenu*)__BoardTrailMenu, 2);
            cSnowSurfaceMenu_dtor(__SnowSurfaceMenu);
            cShadowMenu_dtor(__ShadowMenu);
            cScreenTintMenu__cScreenTintMenu((cScreenTintMenu*)__ScreenTintMenu, 2);
            cDetailSystemMenu_dtor(__DetailSystemMenu);
            cHeightFogMenu_dtor(__HeightFogMenu);
            cDepthFogMenu__cDepthFogMenu((cDepthFogMenu*)__DepthFogMenu, 2);
            cPlantsMenu_dtor(__PlantsMenu);
            cLightGlowMenu__cLightGlowMenu((cLightGlowMenu*)__LightGlowMenu, 2);
            cVisualEffectsTestMenu__cVisualEffectsTestMenu((cVisualEffectsTestMenu*)__VisualEffectsTestMenu, 2);
            cRenderTogglesMenu__cRenderTogglesMenu((cRenderTogglesMenu*)__RenderTogglesMenu, 2);
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
extern "C" void cVisualEffectsMainMenu__cVisualEffectsMainMenu(cVisualEffectsMainMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->PathArrowItem, 2);
    cMenuItem__cMenuItem(&self->TubeItem, 2);
    cMenuItem__cMenuItem(&self->SplashTogglesItem, 2);
    cMenuItem__cMenuItem(&self->SnowfallTogglesItem, 2);
    cMenuItem__cMenuItem(&self->PowerUpFXTogglesItem, 2);
    cMenuItem__cMenuItem(&self->FlagsItem, 2);
    cMenuItem__cMenuItem(&self->BoardTrailItem, 2);
    cMenuItem__cMenuItem(&self->SnowSurfaceItem, 2);
    cMenuItem__cMenuItem(&self->ShadowItem, 2);
    cMenuItem__cMenuItem(&self->ScreenTintItem, 2);
    cMenuItem__cMenuItem(&self->DetailSystemItem, 2);
    cMenuItem__cMenuItem(&self->HeightFogItem, 2);
    cMenuItem__cMenuItem(&self->DepthFogItem, 2);
    cMenuItem__cMenuItem(&self->PlantsItem, 2);
    cMenuItem__cMenuItem(&self->LightGlowItem, 2);
    cMenuItem__cMenuItem(&self->VisualEffectsTestItem, 2);
    cMenuItem__cMenuItem(&self->RenderTogglesItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cPlantsMenu__cPlantsMenu(cPlantsMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->BranchDirectionItem, 2);
    cMenuItem__cMenuItem(&self->SwayDirectionItem, 2);
    cMenuItem__cMenuItem(&self->BranchFrequency2Item, 2);
    cMenuItem__cMenuItem(&self->BranchFrequencyItem, 2);
    cMenuItem__cMenuItem(&self->RotationFrequencyItem, 2);
    cMenuItem__cMenuItem(&self->SwayFrequencyItem, 2);
    cMenuItem__cMenuItem(&self->PositionScaleItem, 2);
    cMenuItem__cMenuItem(&self->RotationAmplitudeItem, 2);
    cMenuItem__cMenuItem(&self->BranchAmplitudeItem, 2);
    cMenuItem__cMenuItem(&self->SwayOffsetItem, 2);
    cMenuItem__cMenuItem(&self->SwayAmplitudeItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cHeightFogMenu__cHeightFogMenu(cHeightFogMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->ThicknessItem, 2);
    cMenuItem__cMenuItem(&self->StartHeightItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cDetailSystemMenu__cDetailSystemMenu(cDetailSystemMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->XBoxMaxSizeItem, 2);
    cMenuItem__cMenuItem(&self->XBoxMinSizeItem, 2);
    cMenuItem__cMenuItem(&self->XBoxSizeItem, 2);
    cMenuItem__cMenuItem(&self->XBoxNumSparklesItem, 2);
    cMenuItem__cMenuItem(&self->PS2MaxSparkleSizeItem, 2);
    cMenuItem__cMenuItem(&self->PS2MinSparkleSizeItem, 2);
    cMenuItem__cMenuItem(&self->PS2SparkleSizeItem, 2);
    cMenuItem__cMenuItem(&self->PS2SparkleCountItem, 2);
    cMenuItem__cMenuItem(&self->SpinScaleItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cShellRendererMenu__cShellRendererMenu(cShellRendererMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->MaxShellsItem, 2);
    cMenuItem__cMenuItem(&self->DepthBrightnessItem, 2);
    cMenuItem__cMenuItem(&self->DepthScaleItem, 2);
    cMenuItem__cMenuItem(&self->DistScaleItem, 2);
    cMenuItem__cMenuItem(&self->FarPlaneItem, 2);
    cMenuItem__cMenuItem(&self->NearPlaneItem, 2);
    cMenuItem__cMenuItem(&self->ShellTextureIdItem, 2);
    cMenuItem__cMenuItem(&self->ModelTextureIdItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cShadowMenu__cShadowMenu(cShadowMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->EndUVItem, 2);
    cMenuItem__cMenuItem(&self->StartUVItem, 2);
    cMenuItem__cMenuItem(&self->IntensityItem, 2);
    cMenuItem__cMenuItem(&self->LengthItem, 2);
    cMenuItem__cMenuItem(&self->ShadowDirZItem, 2);
    cMenuItem__cMenuItem(&self->ShadowDirYItem, 2);
    cMenuItem__cMenuItem(&self->ShadowDirXItem, 2);
    cMenuItem__cMenuItem(&self->ShowShadowPolysItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cSnowSurfaceMenu__cSnowSurfaceMenu(cSnowSurfaceMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->MaxColourScaleItem, 2);
    cMenuItem__cMenuItem(&self->MinColourScaleItem, 2);
    cMenuItem__cMenuItem(&self->NoiseItem, 2);
    cMenuItem__cMenuItem(&self->BumpItem, 2);
    cMenuItem__cMenuItem(&self->BaseItem, 2);
    cMenuItem__cMenuItem(&self->EnablePatchReflectItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cFlagsMenu__cFlagsMenu(cFlagsMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->EnableRenderItem, 2);
    cMenuItem__cMenuItem(&self->EnableUpdateItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
extern "C" void cTubeMenu__cTubeMenu(cTubeMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->BItem, 2);
    cMenuItem__cMenuItem(&self->GItem, 2);
    cMenuItem__cMenuItem(&self->RItem, 2);
    cMenuItem__cMenuItem(&self->ScaleHeightItem, 2);
    cMenuItem__cMenuItem(&self->ScaleWidthItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
 * It is entry +0x30 of the global constructor table __CTOR_LIST__ that __do_global_ctors (libgcc's __do_global_ctors) walks;
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
extern int cAvalanche_m_UseLightMapColour;
extern int cAvalanche_m_UseEndColour;
extern int cAvalanche_m_SquareLerp;
extern int cAvalanche_m_Save;
extern int cAvalanche_m_Load;
extern int cAvalanche_m_RenderEmitters;
extern int cAvalanche_m_RenderLines;
extern int cAvalanche_m_Trigger;
extern int cAvalanche_m_Update;
extern int cAvalanche_m_AvalancheId;
extern int cAvalanche_m_NumParticles;
extern float cAvalanche_m_Brightness;
extern float cAvalanche_m_MinImpactVelocity;
extern float cAvalanche_m_UpdateTimeStep;
extern float cAvalanche_m_RotationalVelocityScale;
extern float cAvalanche_m_RandomBounceChance;
extern float cAvalanche_m_RandomBounceVelocityScaleXY;
extern float cAvalanche_m_RandomBounceVelocityScaleZ;
extern float cAvalanche_m_RandomBounceVelocityScale;
extern float cAvalanche_m_Gravity;
extern float cAvalanche_m_VelocityDamping;
extern float cAvalanche_m_NormalVelocityScale;
extern float cAvalanche_m_ForwardVelocityScale;
extern float cAvalanche_m_NormalToForwardScale;
extern float cAvalanche_m_MaxVelocity;
extern float cAvalanche_m_Damping;
extern float cAvalanche_m_DefaultSize;
extern float cAvalanche_m_PatchColourScale;
extern float cAvalanche_m_PatchColourScale2;
extern float cAvalanche_m_Seed;

/**
 * Builds the "Avalanche Menu" debug menu: a title, a spacer and 32 items for the avalanche effect: enable,
 * recalc, save and load, trigger, update step, bounce and velocity scales, particle count, size, colour
 * scales and seed.
 *
 * @return @p self
 */
extern "C" void* cAvalancheMenu_cAvalancheMenu(cAvalancheMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cAvalancheMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_Avalanche_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cIntMenuItem_cIntMenuItem(&self->EnableItem, 0, &cAvalanche_m_Enable, 0, 1, str_Enable_b);
    cIntMenuItem_cIntMenuItem(&self->RecalcItem, 1, &cAvalanche_m_Recalc, -1, 1, str_Recalc);
    cFloatMenuItem_ctor(&self->BrightnessItem, 2, &cAvalanche_m_Brightness, 0.0f, 4.0f, str_Brightness);
    cIntMenuItem_cIntMenuItem(&self->UseLightMapColourItem, 3, &cAvalanche_m_UseLightMapColour, 0, 1, str_Use_Light_Map_Colour);
    cIntMenuItem_cIntMenuItem(&self->UseEndColourItem, 4, &cAvalanche_m_UseEndColour, 0, 1, str_Use_End_Colour);
    cIntMenuItem_cIntMenuItem(&self->SquareLerpItem, 5, &cAvalanche_m_SquareLerp, 0, 1, str_Square_Lerp_Value);
    cIntMenuItem_cIntMenuItem(&self->SaveItem, 6, &cAvalanche_m_Save, 0, 1, str_Save);
    cIntMenuItem_cIntMenuItem(&self->LoadItem, 7, &cAvalanche_m_Load, 0, 1, str_Load);
    cFloatMenuItem_ctor(&self->MinImpactVelocityItem, 8, &cAvalanche_m_MinImpactVelocity, 0.0f, 120.0f, str_Min_Impact_Velocity);
    cIntMenuItem_cIntMenuItem(&self->RenderEmittersItem, 9, &cAvalanche_m_RenderEmitters, 0, 1, str_Render_Emitters);
    cIntMenuItem_cIntMenuItem(&self->RenderLinesItem, 10, &cAvalanche_m_RenderLines, 0, 0x3F, str_Render_N_Lines);
    cIntMenuItem_cIntMenuItem(&self->TriggerItem, 11, &cAvalanche_m_Trigger, -1, 1, str_Trigger_Avalanche);
    cIntMenuItem_cIntMenuItem(&self->UpdateItem, 0xC, &cAvalanche_m_Update, -1, 1, str_Update);
    cFloatMenuItem_ctor(&self->UpdateTimeStepItem, 13, &cAvalanche_m_UpdateTimeStep, 0.0f, 1.0f, str_Update_Time_Step);
    cFloatMenuItem_ctor(&self->RotationalVelocityScaleItem, 0xE, &cAvalanche_m_RotationalVelocityScale, 0.0f, 1000.0f, str_Rotational_Velocity_Scale);
    cFloatMenuItem_ctor(&self->RandomBounceChanceItem, 0xF, &cAvalanche_m_RandomBounceChance, 0.0f, 1.0f, str_Random_Bounce_Chance);
    cFloatMenuItem_ctor(&self->RandomBounceVelocityScaleXYItem, 0x10, &cAvalanche_m_RandomBounceVelocityScaleXY, 0.0f, 1000.0f, str_Random_Bounce_Velocity_Scale_XY);
    cFloatMenuItem_ctor(&self->RandomBounceVelocityScaleZItem, 17, &cAvalanche_m_RandomBounceVelocityScaleZ, 0.0f, 1000.0f, str_Random_Bounce_Velocity_Scale_Z);
    cFloatMenuItem_ctor(&self->RandomBounceVelocityScaleItem, 18, &cAvalanche_m_RandomBounceVelocityScale, 1.0f, 1000.0f, str_Random_Bounce_Velocity_Scale);
    cIntMenuItem_cIntMenuItem(&self->AvalancheIdItem, 19, &cAvalanche_m_AvalancheId, 0, 1000, str_Avalanche_Id);
    cFloatMenuItem_ctor(&self->GravityItem, 20, &cAvalanche_m_Gravity, 0.0f, 127.0f, str_Gravity);
    cFloatMenuItem_ctor(&self->VelocityDampingItem, 21, &cAvalanche_m_VelocityDamping, 0.0f, 1.0f, str_Velocity_Damping);
    cFloatMenuItem_ctor(&self->NormalVelocityScaleItem, 22, &cAvalanche_m_NormalVelocityScale, 0.0f, 1.0f, str_Normal_Velocity_Scale);
    cFloatMenuItem_ctor(&self->ForwardVelocityScaleItem, 23, &cAvalanche_m_ForwardVelocityScale, 0.0f, 1.0f, str_Forward_Velocity_Scale);
    cFloatMenuItem_ctor(&self->NormalToForwardScaleItem, 0x18, &cAvalanche_m_NormalToForwardScale, 0.0f, 1.0f, str_Normal_To_Forward_Scale);
    cFloatMenuItem_ctor(&self->MaxVelocityItem, 25, &cAvalanche_m_MaxVelocity, 1.0f, 119.0f, str_Max_Velocity);
    cFloatMenuItem_ctor(&self->DampingItem, 26, &cAvalanche_m_Damping, 0.0f, 1.0f, str_Damping);
    cIntMenuItem_cIntMenuItem(&self->NumParticlesItem, 27, &cAvalanche_m_NumParticles, 1, 0x400, str_Num_Particles);
    cFloatMenuItem_ctor(&self->DefaultSizeItem, 0x1C, &cAvalanche_m_DefaultSize, 1.0f, 1000.0f, str_Default_Size);
    cFloatMenuItem_ctor(&self->PatchColourScaleItem, 29, &cAvalanche_m_PatchColourScale, 0.0f, 10.0f, str_Patch_Colour_Scale);
    cFloatMenuItem_ctor(&self->PatchColourScale2Item, 0x1E, &cAvalanche_m_PatchColourScale2, 0.0f, 10.0f, str_Patch_Colour_Scale_2);
    cFloatMenuItem_ctor(&self->SeedItem, 0x1F, &cAvalanche_m_Seed, -1.0f, 1000.0f, str_Seed_b);
    cMenu_addItem(self, &self->TitleItem, -1);
    cMenu_addItem(self, &self->SpaceItem, -1);
    cMenu_addItem(self, &self->EnableItem, -1);
    cMenu_addItem(self, &self->RecalcItem, -1);
    cMenu_addItem(self, &self->BrightnessItem, -1);
    cMenu_addItem(self, &self->UseLightMapColourItem, -1);
    cMenu_addItem(self, &self->UseEndColourItem, -1);
    cMenu_addItem(self, &self->SquareLerpItem, -1);
    cMenu_addItem(self, &self->SaveItem, -1);
    cMenu_addItem(self, &self->LoadItem, -1);
    cMenu_addItem(self, &self->MinImpactVelocityItem, -1);
    cMenu_addItem(self, &self->RenderEmittersItem, -1);
    cMenu_addItem(self, &self->RenderLinesItem, -1);
    cMenu_addItem(self, &self->TriggerItem, -1);
    cMenu_addItem(self, &self->UpdateItem, -1);
    cMenu_addItem(self, &self->UpdateTimeStepItem, -1);
    cMenu_addItem(self, &self->RotationalVelocityScaleItem, -1);
    cMenu_addItem(self, &self->RandomBounceChanceItem, -1);
    cMenu_addItem(self, &self->RandomBounceVelocityScaleXYItem, -1);
    cMenu_addItem(self, &self->RandomBounceVelocityScaleZItem, -1);
    cMenu_addItem(self, &self->RandomBounceVelocityScaleItem, -1);
    cMenu_addItem(self, &self->AvalancheIdItem, -1);
    cMenu_addItem(self, &self->GravityItem, -1);
    cMenu_addItem(self, &self->VelocityDampingItem, -1);
    cMenu_addItem(self, &self->NormalVelocityScaleItem, -1);
    cMenu_addItem(self, &self->ForwardVelocityScaleItem, -1);
    cMenu_addItem(self, &self->NormalToForwardScaleItem, -1);
    cMenu_addItem(self, &self->MaxVelocityItem, -1);
    cMenu_addItem(self, &self->DampingItem, -1);
    cMenu_addItem(self, &self->NumParticlesItem, -1);
    cMenu_addItem(self, &self->DefaultSizeItem, -1);
    cMenu_addItem(self, &self->PatchColourScaleItem, -1);
    cMenu_addItem(self, &self->PatchColourScale2Item, -1);
    cMenu_addItem(self, &self->SeedItem, -1);
    return ((void*)self);
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
extern char __AvalancheMenu[];
extern char str_Avalanche_Effects[];

/**
 * Builds the "AIVisual Effects Main Menu" debug menu, whose only entry opens the avalanche effects menu.
 *
 * @return @p self
 */
extern "C" void* cAIVisualEffectsMainMenu_cAIVisualEffectsMainMenu(cAIVisualEffectsMainMenu* self)
{
    cMenu_cMenu(self);
    self->mVtbl = cAIVisualEffectsMainMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->TitleItem), str_AIVisual_Effects_Main_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->SpaceItem), (void*)40);
    cSubMenuItem_cSubMenuItem(&self->AvalancheItem, __AvalancheMenu, str_Avalanche_Effects);
    cMenu_addItem(self, &self->AvalancheItem, -1);
    return ((void*)self);
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
extern "C" void cAvalancheMenu__cAvalancheMenu(cAvalancheMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->SeedItem, 2);
    cMenuItem__cMenuItem(&self->PatchColourScale2Item, 2);
    cMenuItem__cMenuItem(&self->PatchColourScaleItem, 2);
    cMenuItem__cMenuItem(&self->DefaultSizeItem, 2);
    cMenuItem__cMenuItem(&self->NumParticlesItem, 2);
    cMenuItem__cMenuItem(&self->DampingItem, 2);
    cMenuItem__cMenuItem(&self->MaxVelocityItem, 2);
    cMenuItem__cMenuItem(&self->NormalToForwardScaleItem, 2);
    cMenuItem__cMenuItem(&self->ForwardVelocityScaleItem, 2);
    cMenuItem__cMenuItem(&self->NormalVelocityScaleItem, 2);
    cMenuItem__cMenuItem(&self->VelocityDampingItem, 2);
    cMenuItem__cMenuItem(&self->GravityItem, 2);
    cMenuItem__cMenuItem(&self->AvalancheIdItem, 2);
    cMenuItem__cMenuItem(&self->RandomBounceVelocityScaleItem, 2);
    cMenuItem__cMenuItem(&self->RandomBounceVelocityScaleZItem, 2);
    cMenuItem__cMenuItem(&self->RandomBounceVelocityScaleXYItem, 2);
    cMenuItem__cMenuItem(&self->RandomBounceChanceItem, 2);
    cMenuItem__cMenuItem(&self->RotationalVelocityScaleItem, 2);
    cMenuItem__cMenuItem(&self->UpdateTimeStepItem, 2);
    cMenuItem__cMenuItem(&self->UpdateItem, 2);
    cMenuItem__cMenuItem(&self->TriggerItem, 2);
    cMenuItem__cMenuItem(&self->RenderLinesItem, 2);
    cMenuItem__cMenuItem(&self->RenderEmittersItem, 2);
    cMenuItem__cMenuItem(&self->MinImpactVelocityItem, 2);
    cMenuItem__cMenuItem(&self->LoadItem, 2);
    cMenuItem__cMenuItem(&self->SaveItem, 2);
    cMenuItem__cMenuItem(&self->SquareLerpItem, 2);
    cMenuItem__cMenuItem(&self->UseEndColourItem, 2);
    cMenuItem__cMenuItem(&self->UseLightMapColourItem, 2);
    cMenuItem__cMenuItem(&self->BrightnessItem, 2);
    cMenuItem__cMenuItem(&self->RecalcItem, 2);
    cMenuItem__cMenuItem(&self->EnableItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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

extern "C" void* cAvalancheMenu_cAvalancheMenu(cAvalancheMenu* self);
extern "C" void cAvalancheMenu__cAvalancheMenu(cAvalancheMenu* self, int flags);

extern char cfm_sav_Render_Halo_SUN_kSunColour_2[], cfm_sav_Render_Halo_SUN_LENS0_kLens0Colour_2[], cfm_sav_Render_Halo_SUN_LENS1_kLens1Colour_2[], cfm_sav_Render_Halo_SUN_LENS2_kLens2Colour_2[], cfm_sav_Render_Halo_SUN_LENS3_kLens3Colour_2[], cfm_sav_Render_Halo_SUN_LENS4_kLens4Colour_2[], D_004D1968[], D_004D1978[], D_004D1988[], D_004D1998[], D_004D19A8[], D_004D19B8[], D_004D19C8[], D_004D19D8[], D_004D19E8[], D_004D19F8[], D_004D1A08[], D_004D1A18[], D_004D1A28[], D_004D1A38[], D_004D1A48[], D_004D1A58[], D_004D1A68[], D_004D1A78[], D_004D1A88[], D_004D1A98[], D_004D1AA8[], D_004D1AB8[], D_004D1AC8[], D_004D1AD8[], D_004D1AE8[], D_004D1AF8[], D_004D1B08[], D_004D1B18[], D_004D1B28[], D_004D1B38[], D_004D1B48[], D_004D1B58[], D_004D1B68[], D_004D1B78[], D_004D1B88[], D_004D1B98[], D_004D1BA8[], D_004D1BB8[], D_004D1BC8[], D_004D1BD8[], D_004D1BE8[], D_004D1BF8[], D_004D1C08[], D_004D1C18[], D_004D1C28[], D_004D1C38[], D_004D1C48[], D_004D1C58[], D_004D1C68[], D_004D1C78[];
extern char __AvalancheMenu[];
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
 * __sti__visualeffectsmenu_cpp (its own copies at cfm_sav_Render_Halo_SUN_kSunColour_2..D_004D1C78), zeroes D_004A5060 and D_004A5064,
 * numbers the 14 key objects D_004A5068..D_004A50D0 from 0 to 13 and constructs the static "Avalanche Menu" at
 * __AvalancheMenu. With @p init clear it destroys that menu with flags 2 (no free).
 *
 * @param init  nonzero to construct the static objects, 0 to destroy them
 * @param prio  initialisation priority; nothing happens unless it is 0xFFFF (the default)
 */
extern "C" void __sti__aivisualeffectsmenu_cpp_impl(int init, int prio)
{
    if (prio == 0xFFFF) {
        if (init != 0) {
            new (cfm_sav_Render_Halo_SUN_kSunColour_2) sV4_0CB0(1.0f, 1.0f, 1.0f, 0.5f);
            new (cfm_sav_Render_Halo_SUN_LENS0_kLens0Colour_2) sV4_0CB0(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
            new (cfm_sav_Render_Halo_SUN_LENS1_kLens1Colour_2) sV4_0CB0(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
            new (cfm_sav_Render_Halo_SUN_LENS2_kLens2Colour_2) sV4_0CB0(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
            new (cfm_sav_Render_Halo_SUN_LENS3_kLens3Colour_2) sV4_0CB0(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
            new (cfm_sav_Render_Halo_SUN_LENS4_kLens4Colour_2) sV4_0CB0(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
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
            cAvalancheMenu_cAvalancheMenu((cAvalancheMenu*)__AvalancheMenu);
        } else {
            cAvalancheMenu__cAvalancheMenu((cAvalancheMenu*)__AvalancheMenu, 2);
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
extern "C" void cAIVisualEffectsMainMenu__cAIVisualEffectsMainMenu(cAIVisualEffectsMainMenu* self, int flags)
{
    cMenuItem__cMenuItem(&self->AvalancheItem, 2);
    cMenuItem__cMenuItem(&self->SpaceItem, 2);
    cMenuItem__cMenuItem(&self->TitleItem, 2);
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
 * It is entry +0x34 of the global constructor table __CTOR_LIST__; func_002516B0 is its destructor twin. The declared
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
extern char memclass[];

/**
 * Clears the 256-byte buffer memclass with MEM_fill.
 */
extern "C" void func_002516F0(void)
{
    MEM_fill(memclass, 0, 0x100);
}
#endif
