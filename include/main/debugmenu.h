#ifndef MAIN_DEBUGMENU_H
#define MAIN_DEBUGMENU_H

// The visual-effects debug menus: 21 cMenu subclasses (util/menu.h) of the retail unit main/debugmenu, one per effect
// system (render toggles, light glow, fogs, screen tint, snow, board trail, splash, path arrow, avalanche, ...) plus
// the two top-level menus that link to them with cSubMenuItem rows. Each menu is a cMenu whose own members are its
// rows, embedded by value from 0x130 on; nothing else. EA's source: main/visualeffectsmenu.cpp (19 classes) and
// main/aivisualeffectsmenu.cpp (cAvalancheMenu, cAIVisualEffectsMainMenu): the retail static initialisers are
// __sti__visualeffectsmenu_cpp / __sti__aivisualeffectsmenu_cpp, and On Tour's DWARF compile units are
// z:/main/game/source/main/{visualeffectsmenu,aivisualeffectsmenu}.cpp. This header is named after the retail unit.
//
// Every class has the same shape:
//   - base cMenu (0x000-0x12F, written out flat below; its vptr at 0x12C, as in util/menu.h);
//   - the constructor <Class>_<Class> (extern "C", returns this) calls cMenu_cMenu(this), stores <Class>_vtbl at 0x12C,
//     constructs every row (a cNullMenuItem title at 0x130, a cSpaceMenuItem of 40 at 0x14C, then the editing rows
//     bound to the effect's tuning globals) and cMenu_addItem()s them in order;
//   - the destructor <Class>__<Class>(this, flags) (vtable entry 1) runs cMenuItem__cMenuItem(row, 2) on every row in
//     reverse (the cARGBMenuItem rows are torn down inline: their 4 channel rows and title row, cMenu__cMenu on their
//     ExpandMenu at +0x18, then the row), then cMenu__cMenu(this, flags);
//   - the vtable (9 entries) is cMenu's with entries 1 (destructor), 6 (onGainFocus) and 7 (onLoseFocus) overridden;
//     <Class>_onGainFocus / _onLoseFocus (C++ linkage, __FPv) only forward to cMenu_onGainFocus / cMenu_onLoseFocus.
// The rows' types are util/menu.h's (cNullMenuItem 0x1C, cSpaceMenuItem 0x18, cIntMenuItem 0x30, cFloatMenuItem 0x34,
// cSubMenuItem 0x1C, cARGBMenuItem 0x65C); consecutive rows follow those sizes exactly, with no padding.
//
// Sizes: the end of the last row the constructor builds (= the start of the next static object: the 18 static
// instances below are laid out back to back, each rounded up to 8). On Tour's DWARF (class sizes and members with
// names, all starting at 0x130) agrees for 18 classes; for cDepthFogMenu, cDetailSystemMenu and cBoardTrailMenu
// On Tour has extra (PSP / ski) rows, and retail's rows are named by On Tour's order and the retail row captions.
//
// A FLAT layout, like main/game.h and util/menu.h's cErrorMenu: every field at its retail offset, no C++ inheritance
// or virtuals. Evidence tags:
//   [R] retail constructor / destructor (the C bodies in src/main/debugmenu.cpp, all 100%, and their asm), the static
//       instances' addresses (__sti__visualeffectsmenu_cpp / __sti__aivisualeffectsmenu_cpp);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class <Class>: member names, types and order; offsets match retail unless noted).
// A row's "[R] ctor (...)" gives the constructor's arguments, e.g. (id, "caption", &bound variable, min..max) for the
// int/float rows; every row is also destroyed by the class's destructor (entry 1) and, unless noted, added to the menu.

#include "layout.h"
#include "util/menu.h"


// cRenderTogglesMenu (0xA48): the "Render Toggles Menu" debug menu: a title, a spacer and 47 editing rows.
// Size: [R] the constructor's last row, cIntMenuItem ShowSurfaceTypesItem, is at 0xA18 and ends at 0xA48; the static
// instance __RenderTogglesMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __VisualEffectsTestMenu (+0xA48); [O] size
// 0xA48.
// Base cMenu. Vtable cRenderTogglesMenu_vtbl (0x0047FCE0); destructor cRenderTogglesMenu__cRenderTogglesMenu. EA
// source: main/visualeffectsmenu.cpp.
struct cRenderTogglesMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's
                                                     //       first call)
    int FocusItem;                                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                            // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s
                                                     //       every row
    cMenuSys* MenuSys;                               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U]
                                                     //       onGainFocus/onLoseFocus forward to cMenu's, which
                                                     //       set/clear it
    int DrawLayer;                                   // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                                    // 0x12C [R] the constructor stores cRenderTogglesMenu_vtbl
                                                     //       (0x0047FCE0)
    // cRenderTogglesMenu's rows (0x130-0xA47)
    cNullMenuItem TitleItem;                         // 0x130 [O]; [R] ctor ("Render Toggles Menu")
    cSpaceMenuItem SpaceItem;                        // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem AIUpdatesPerRenderItem;             // 0x164 [O]; [R] ctor (0, "AI Frames per Render Frame",
                                                     //       &cRenderToggles_m_AIUpdatesPerRender, 0..60)
    cIntMenuItem DisplayRenderStatsItem;             // 0x194 [O]; [R] ctor (1, "Display Render Stats", &cRenderToggles_m_DisplayRenderStats,
                                                     //       0..1)
    cIntMenuItem ResetStatsItem;                     // 0x1C4 [O]; [R] ctor (2, "Reset Stats", &cRenderToggles_m_ResetStats, 0..1)
    cIntMenuItem UseSmallTexture0Item;               // 0x1F4 [O]; [R] ctor (3, "Use Small Texture 0", &cRenderToggles_m_UseSmallTexture0,
                                                     //       0..1)
    cIntMenuItem UseSmallTexture1Item;               // 0x224 [O]; [R] ctor (4, "Use Small Texture 1", &cRenderToggles_m_UseSmallTexture1,
                                                     //       0..1)
    cIntMenuItem DisablePatchesItem;                 // 0x254 [O]; [R] ctor (5, "Disable Patches", &cRenderToggles_m_DisablePatches, 0..1)
    cIntMenuItem DisablePatchReflectionItem;         // 0x284 [O]; [R] ctor (6, "Disable Patch Reflection", &cRenderToggles_m_DisablePatchReflection,
                                                     //       0..1)
    cIntMenuItem DisablePatchShadowItem;             // 0x2B4 [O]; [R] ctor (7, "Disable Patch Shadows", &cRenderToggles_m_DisablePatchShadow,
                                                     //       0..1)
    cIntMenuItem DisableSparklesItem;                // 0x2E4 [O]; [R] ctor (8, "Disable Sparkles", &cRenderToggles_m_DisableSparkles, 0..1)
    cIntMenuItem DisableLineStripsItem;              // 0x314 [O]; [R] ctor (9, "Disable Line Strips", &cRenderToggles_m_DisableLineStrips,
                                                     //       0..1)
    cIntMenuItem DisableTriStripsItem;               // 0x344 [O]; [R] ctor (10, "Disable Tri Strips", &cRenderToggles_m_DisableTriStrips,
                                                     //       0..3)
    cIntMenuItem DisableTransformStripsItem;         // 0x374 [O]; [R] ctor (11, "Disable Transform Strips",
                                                     //       &cRenderToggles_m_DisableTransformStrips, 0..1)
    cIntMenuItem DisableMeshesItem;                  // 0x3A4 [O]; [R] ctor (12, "Disable Meshes", &cRenderToggles_m_DisableMeshes, 0..1)
    cIntMenuItem DisableMeshReflectionItem;          // 0x3D4 [O]; [R] ctor (13, "Disable Mesh Reflection", &cRenderToggles_m_DisableMeshReflection,
                                                     //       0..1)
    cIntMenuItem DisableStaticEmittersItem;          // 0x404 [O]; [R] ctor (14, "Disable Static Emitters", &cRenderToggles_m_DisableStaticEmitters,
                                                     //       0..1)
    cIntMenuItem DisableDynamicEmittersItem;         // 0x434 [O]; [R] ctor (15, "Disable Dynamic Emitters",
                                                     //       &cRenderToggles_m_DisableDynamicEmitters, 0..1)
    cIntMenuItem DisableDynamicColourEmittersItem;   // 0x464 [O]; [R] ctor (16, "Disable Dynamic Colour Emitters",
                                                     //       &cRenderToggles_m_DisableDynamicColourEmitters, 0..1)
    cIntMenuItem DisableSpritesItem;                 // 0x494 [O]; [R] ctor (17, "Disable Sprites", &cRenderToggles_m_DisableSprites, 0..1)
    cIntMenuItem Disable3DSpritesItem;               // 0x4C4 [O]; [R] ctor (18, "Disable 3D Sprites", &cRenderToggles_m_Disable3DSprites,
                                                     //       0..1)
    cIntMenuItem DisableTextureRectsItem;            // 0x4F4 [O]; [R] ctor (19, "Disable Texture Rects", &cRenderToggles_m_DisableTextureRects,
                                                     //       0..1)
    cIntMenuItem DisablePlayerStripsItem;            // 0x524 [O]; [R] ctor (20, "Disable Player Strips", &cRenderToggles_m_DisablePlayerStrips,
                                                     //       0..1)
    cIntMenuItem DisableShadowsItem;                 // 0x554 [O]; [R] ctor (21, "Disable Shadows", &cRenderToggles_m_DisableShadows, 0..1)
    cIntMenuItem DisableBoardTrailItem;              // 0x584 [O]; [R] ctor (22, "Disable Board Trail", &cRenderToggles_m_DisableBoardTrail,
                                                     //       0..1)
    cIntMenuItem DisableAIBoardTrailItem;            // 0x5B4 [O]; [R] ctor (23, "Disable AI Board Trail", &cRenderToggles_m_DisableAIBoardTrail,
                                                     //       0..1)
    cIntMenuItem DisableVisibilityRectsItem;         // 0x5E4 [O]; [R] ctor (24, "Disable Visibility Rects",
                                                     //       &cRenderToggles_m_DisableVisibilityRects, 0..1)
    cIntMenuItem DisableSnowFallItem;                // 0x614 [O]; [R] ctor (25, "Disable Snowfall",
                                                     //       &cRenderToggles_m_DisableSnowFall, 0..1)
    cIntMenuItem DisableBoardWakeItem;               // 0x644 [O]; [R] ctor (26, "Disable Board Wake",
                                                     //       &cRenderToggles_m_DisableBoardWake, 0..1)
    cIntMenuItem DisableSkyBoxItem;                  // 0x674 [O]; [R] ctor (27, "Disable Sky Box",
                                                     //       &cRenderToggles_m_DisableSkyBox, 0..1)
    cIntMenuItem DisableCharacterSpecularItem;       // 0x6A4 [O]; [R] ctor (28, "Disable Character Specular",
                                                     //       &cRenderToggles_m_DisableCharacterSpecular, 0..1)
    cIntMenuItem DisableAIVisualFXItem;              // 0x6D4 [O]; [R] ctor (29, "Disable AI Visual FX",
                                                     //       &cRenderToggles_m_DisableAIVisualFX, 0..1)
    cIntMenuItem DisableBoundingBoxesItem;           // 0x704 [O]; [R] ctor (30, "Disable Bounding Boxes",
                                                     //       &cRenderToggles_m_DisableBoundingBoxes, 0..1)
    cIntMenuItem UseReflectionVectorItem;            // 0x734 [O]; [R] ctor (31, "Use Reflection Vector for Sphere Map",
                                                     //       &cRenderToggles_m_UseReflectionVector, 0..1)
    cIntMenuItem DisableHUDItem;                     // 0x764 [O]; [R] ctor (32, "Disable HUD elements",
                                                     //       &cRenderToggles_m_DisableHUD, 0..1)
    cIntMenuItem TestTunnelItem;                     // 0x794 [O]; [R] ctor (33, "Test Tunnel", &cRenderToggles_m_TestTunnel, 0..1)
    cFloatMenuItem ParticleSizeScaleItem;            // 0x7C4 [O]; [R] ctor (34, "Particle Size Scale", &cRenderToggles_m_ParticleSizeScale,
                                                     //       1..2000)
    cIntMenuItem TestSurfaceTypesItem;               // 0x7F8 [O]; [R] ctor (35, "Test Surface Types", &cRenderToggles_m_TestSurfaceTypes,
                                                     //       0..1)
    cIntMenuItem SoftwareDynamicParticleSystemsItem; // 0x828 [O]; [R] ctor (36, "Software Based Dynamic Systems",
                                                     //       &cRenderToggles_m_SoftwareDynamicParticleSystems, 0..1)
    cIntMenuItem TempIntItem;                        // 0x858 [O]; [R] ctor (37, "Temp Int", &cRenderToggles_m_TempInt, 0..20)
    cFloatMenuItem Prog2xItem;                       // 0x888 [O]; [R] ctor (38, "PROG2 x", &cRenderToggles_m_Prog2x, -10..10)
    cFloatMenuItem Prog2yItem;                       // 0x8BC [O]; [R] ctor (39, "PROG2 y", &cRenderToggles_m_Prog2y, -10..10)
    cFloatMenuItem Prog2zItem;                       // 0x8F0 [O]; [R] ctor (40, "PROG2 z", &cRenderToggles_m_Prog2z, -10..10)
    cFloatMenuItem Prog2wItem;                       // 0x924 [O]; [R] ctor (41, "PROG2 w", &cRenderToggles_m_Prog2w, -10..10)
    cIntMenuItem DebugInfoItem;                      // 0x958 [O]; [R] ctor (42, "Enable Build Info", &cRenderToggles_m_DebugInfo, 0..1)
    cIntMenuItem DebugNISMemInfoItem;                // 0x988 [O]; [R] ctor (43, "Enable NIS Mem Usage Info",
                                                     //       &cRenderToggles_m_DebugNISMemInfo, 0..1)
    cIntMenuItem DebugGameModeInfoItem;              // 0x9B8 [O]; [R] ctor (44, "Enable Gameflow Info", &cRenderToggles_m_DebugGameModeInfo,
                                                     //       0..1)
    cIntMenuItem ProfileIndividualFXItem;            // 0x9E8 [O]; [R] ctor (45, "Profile Individual Effects",
                                                     //       &cRenderToggles_m_ProfileIndividualFX, 0..1)
    cIntMenuItem ShowSurfaceTypesItem;               // 0xA18 [O]; [R] ctor (46, "Show Surface Types", &cRenderToggles_m_ShowSurfaceTypes,
                                                     //       0..1)
};

CHECK_OFFSET(cRenderTogglesMenu, NumItems, 0x000);
CHECK_OFFSET(cRenderTogglesMenu, FocusItem, 0x004);
CHECK_OFFSET(cRenderTogglesMenu, TopItem, 0x008);
CHECK_OFFSET(cRenderTogglesMenu, Items, 0x00C);
CHECK_OFFSET(cRenderTogglesMenu, MenuSys, 0x124);
CHECK_OFFSET(cRenderTogglesMenu, DrawLayer, 0x128);
CHECK_OFFSET(cRenderTogglesMenu, mVtbl, 0x12C);
CHECK_OFFSET(cRenderTogglesMenu, TitleItem, 0x130);
CHECK_OFFSET(cRenderTogglesMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cRenderTogglesMenu, AIUpdatesPerRenderItem, 0x164);
CHECK_OFFSET(cRenderTogglesMenu, DisplayRenderStatsItem, 0x194);
CHECK_OFFSET(cRenderTogglesMenu, ResetStatsItem, 0x1C4);
CHECK_OFFSET(cRenderTogglesMenu, UseSmallTexture0Item, 0x1F4);
CHECK_OFFSET(cRenderTogglesMenu, UseSmallTexture1Item, 0x224);
CHECK_OFFSET(cRenderTogglesMenu, DisablePatchesItem, 0x254);
CHECK_OFFSET(cRenderTogglesMenu, DisablePatchReflectionItem, 0x284);
CHECK_OFFSET(cRenderTogglesMenu, DisablePatchShadowItem, 0x2B4);
CHECK_OFFSET(cRenderTogglesMenu, DisableSparklesItem, 0x2E4);
CHECK_OFFSET(cRenderTogglesMenu, DisableLineStripsItem, 0x314);
CHECK_OFFSET(cRenderTogglesMenu, DisableTriStripsItem, 0x344);
CHECK_OFFSET(cRenderTogglesMenu, DisableTransformStripsItem, 0x374);
CHECK_OFFSET(cRenderTogglesMenu, DisableMeshesItem, 0x3A4);
CHECK_OFFSET(cRenderTogglesMenu, DisableMeshReflectionItem, 0x3D4);
CHECK_OFFSET(cRenderTogglesMenu, DisableStaticEmittersItem, 0x404);
CHECK_OFFSET(cRenderTogglesMenu, DisableDynamicEmittersItem, 0x434);
CHECK_OFFSET(cRenderTogglesMenu, DisableDynamicColourEmittersItem, 0x464);
CHECK_OFFSET(cRenderTogglesMenu, DisableSpritesItem, 0x494);
CHECK_OFFSET(cRenderTogglesMenu, Disable3DSpritesItem, 0x4C4);
CHECK_OFFSET(cRenderTogglesMenu, DisableTextureRectsItem, 0x4F4);
CHECK_OFFSET(cRenderTogglesMenu, DisablePlayerStripsItem, 0x524);
CHECK_OFFSET(cRenderTogglesMenu, DisableShadowsItem, 0x554);
CHECK_OFFSET(cRenderTogglesMenu, DisableBoardTrailItem, 0x584);
CHECK_OFFSET(cRenderTogglesMenu, DisableAIBoardTrailItem, 0x5B4);
CHECK_OFFSET(cRenderTogglesMenu, DisableVisibilityRectsItem, 0x5E4);
CHECK_OFFSET(cRenderTogglesMenu, DisableSnowFallItem, 0x614);
CHECK_OFFSET(cRenderTogglesMenu, DisableBoardWakeItem, 0x644);
CHECK_OFFSET(cRenderTogglesMenu, DisableSkyBoxItem, 0x674);
CHECK_OFFSET(cRenderTogglesMenu, DisableCharacterSpecularItem, 0x6A4);
CHECK_OFFSET(cRenderTogglesMenu, DisableAIVisualFXItem, 0x6D4);
CHECK_OFFSET(cRenderTogglesMenu, DisableBoundingBoxesItem, 0x704);
CHECK_OFFSET(cRenderTogglesMenu, UseReflectionVectorItem, 0x734);
CHECK_OFFSET(cRenderTogglesMenu, DisableHUDItem, 0x764);
CHECK_OFFSET(cRenderTogglesMenu, TestTunnelItem, 0x794);
CHECK_OFFSET(cRenderTogglesMenu, ParticleSizeScaleItem, 0x7C4);
CHECK_OFFSET(cRenderTogglesMenu, TestSurfaceTypesItem, 0x7F8);
CHECK_OFFSET(cRenderTogglesMenu, SoftwareDynamicParticleSystemsItem, 0x828);
CHECK_OFFSET(cRenderTogglesMenu, TempIntItem, 0x858);
CHECK_OFFSET(cRenderTogglesMenu, Prog2xItem, 0x888);
CHECK_OFFSET(cRenderTogglesMenu, Prog2yItem, 0x8BC);
CHECK_OFFSET(cRenderTogglesMenu, Prog2zItem, 0x8F0);
CHECK_OFFSET(cRenderTogglesMenu, Prog2wItem, 0x924);
CHECK_OFFSET(cRenderTogglesMenu, DebugInfoItem, 0x958);
CHECK_OFFSET(cRenderTogglesMenu, DebugNISMemInfoItem, 0x988);
CHECK_OFFSET(cRenderTogglesMenu, DebugGameModeInfoItem, 0x9B8);
CHECK_OFFSET(cRenderTogglesMenu, ProfileIndividualFXItem, 0x9E8);
CHECK_OFFSET(cRenderTogglesMenu, ShowSurfaceTypesItem, 0xA18);
CHECK_SIZE(cRenderTogglesMenu, 0xA48);

// cVisualEffectsTestMenu (0x494): the "Visual Effects Test Menu" debug menu: a title, a spacer and 17 editing rows.
// Size: [R] the constructor's last row, cIntMenuItem LayerIdItem, is at 0x464 and ends at 0x494; the static instance
// __VisualEffectsTestMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __LightGlowMenu (+0x498); [O] size 0x494.
// Base cMenu. Vtable cVisualEffectsTestMenu_vtbl (0x0047FC88); destructor
// cVisualEffectsTestMenu__cVisualEffectsTestMenu. EA source: main/visualeffectsmenu.cpp.
struct cVisualEffectsTestMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                        // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                       // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                         // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;                   // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus
                                         //       forward to cMenu's, which set/clear it
    int DrawLayer;                       // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                        // 0x12C [R] the constructor stores cVisualEffectsTestMenu_vtbl (0x0047FC88)
    // cVisualEffectsTestMenu's rows (0x130-0x493)
    cNullMenuItem TitleItem;             // 0x130 [O]; [R] ctor ("Visual Effects Test Menu")
    cSpaceMenuItem SpaceItem;            // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem HUDEnableItem;          // 0x164 [O]; [R] ctor (0, "Show All HUD Graphics", &cVisualEffectsTest_m_HUDEnable, 0..1)
    cIntMenuItem ShowCurrentElementItem; // 0x194 [O]; [R] ctor (1, "Show Current Element", &cVisualEffectsTest_m_ShowCurrentElement, 0..1)
    cIntMenuItem HUDPrintItem;           // 0x1C4 [O]; [R] ctor (2, "Output HUD Data", &cVisualEffectsTest_m_HUDPrint, 0..1)
    cIntMenuItem CurrElementItem;        // 0x1F4 [O]; [R] ctor (3, "Current Element Id", &cVisualEffectsTest_m_CurrElement, 0..1000)
    cIntMenuItem PositionXItem;          // 0x224 [O]; [R] ctor (4, "Position X", &cVisualEffectsTest_m_PositionX, -640..640)
    cIntMenuItem PositionYItem;          // 0x254 [O]; [R] ctor (5, "Position Y", &cVisualEffectsTest_m_PositionY, -480..480)
    cIntMenuItem SizeXItem;              // 0x284 [O]; [R] ctor (6, "Size X", &cVisualEffectsTest_m_SizeX, -640..640)
    cIntMenuItem SizeYItem;              // 0x2B4 [O]; [R] ctor (7, "Size Y", &cVisualEffectsTest_m_SizeY, -640..640)
    cIntMenuItem ScaleXItem;             // 0x2E4 [O]; [R] ctor (8, "Scale X", &cVisualEffectsTest_m_ScaleX, -1..1000)
    cIntMenuItem ScaleYItem;             // 0x314 [O]; [R] ctor (9, "Scale Y", &cVisualEffectsTest_m_ScaleY, -1..1000)
    cIntMenuItem AlphaItem;              // 0x344 [O]; [R] ctor (10, "Alpha", &cVisualEffectsTest_m_Alpha, -255..255)
    cIntMenuItem RedItem;                // 0x374 [O]; [R] ctor (11, "Red", &cVisualEffectsTest_m_Red, -255..255)
    cIntMenuItem GreenItem;              // 0x3A4 [O]; [R] ctor (12, "Green", &cVisualEffectsTest_m_Green, -255..255)
    cIntMenuItem BlueItem;               // 0x3D4 [O]; [R] ctor (13, "Blue", &cVisualEffectsTest_m_Blue, -255..255)
    cIntMenuItem LayerOffsetItem;        // 0x404 [O]; [R] ctor (14, "Layer Offset", &cVisualEffectsTest_m_LayerOffset, -16..16)
    cIntMenuItem EnableItem;             // 0x434 [O]; [R] ctor (15, "World Painter Display Enable",
                                         //       &cVisualEffectsTest_m_Enable, 0..1)
    cIntMenuItem LayerIdItem;            // 0x464 [O]; [R] ctor (16, "World Painter LayerId",
                                         //       &cVisualEffectsTest_m_LayerId, 0..100)
};

CHECK_OFFSET(cVisualEffectsTestMenu, NumItems, 0x000);
CHECK_OFFSET(cVisualEffectsTestMenu, FocusItem, 0x004);
CHECK_OFFSET(cVisualEffectsTestMenu, TopItem, 0x008);
CHECK_OFFSET(cVisualEffectsTestMenu, Items, 0x00C);
CHECK_OFFSET(cVisualEffectsTestMenu, MenuSys, 0x124);
CHECK_OFFSET(cVisualEffectsTestMenu, DrawLayer, 0x128);
CHECK_OFFSET(cVisualEffectsTestMenu, mVtbl, 0x12C);
CHECK_OFFSET(cVisualEffectsTestMenu, TitleItem, 0x130);
CHECK_OFFSET(cVisualEffectsTestMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cVisualEffectsTestMenu, HUDEnableItem, 0x164);
CHECK_OFFSET(cVisualEffectsTestMenu, ShowCurrentElementItem, 0x194);
CHECK_OFFSET(cVisualEffectsTestMenu, HUDPrintItem, 0x1C4);
CHECK_OFFSET(cVisualEffectsTestMenu, CurrElementItem, 0x1F4);
CHECK_OFFSET(cVisualEffectsTestMenu, PositionXItem, 0x224);
CHECK_OFFSET(cVisualEffectsTestMenu, PositionYItem, 0x254);
CHECK_OFFSET(cVisualEffectsTestMenu, SizeXItem, 0x284);
CHECK_OFFSET(cVisualEffectsTestMenu, SizeYItem, 0x2B4);
CHECK_OFFSET(cVisualEffectsTestMenu, ScaleXItem, 0x2E4);
CHECK_OFFSET(cVisualEffectsTestMenu, ScaleYItem, 0x314);
CHECK_OFFSET(cVisualEffectsTestMenu, AlphaItem, 0x344);
CHECK_OFFSET(cVisualEffectsTestMenu, RedItem, 0x374);
CHECK_OFFSET(cVisualEffectsTestMenu, GreenItem, 0x3A4);
CHECK_OFFSET(cVisualEffectsTestMenu, BlueItem, 0x3D4);
CHECK_OFFSET(cVisualEffectsTestMenu, LayerOffsetItem, 0x404);
CHECK_OFFSET(cVisualEffectsTestMenu, EnableItem, 0x434);
CHECK_OFFSET(cVisualEffectsTestMenu, LayerIdItem, 0x464);
CHECK_SIZE(cVisualEffectsTestMenu, 0x494);

// cPlantsMenu (0x3CC): the "Plants Menu" debug menu: a title, a spacer and 12 editing rows.
// Size: [R] the constructor's last row, cFloatMenuItem BranchDirectionItem, is at 0x398 and ends at 0x3CC; the static
// instance __PlantsMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __DepthFogMenu (+0x3D0); [O] size
// 0x3CC.
// Base cMenu. Vtable cPlantsMenu_vtbl (0x0047FC30); destructor cPlantsMenu__cPlantsMenu (the static instance's
// destruction in __sti__visualeffectsmenu_cpp inlines it). EA source: main/visualeffectsmenu.cpp.
struct cPlantsMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                         // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                        // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                          // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                 // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;                    // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus
                                          //       forward to cMenu's, which set/clear it
    int DrawLayer;                        // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                         // 0x12C [R] the constructor stores cPlantsMenu_vtbl (0x0047FC30)
    // cPlantsMenu's rows (0x130-0x3CB)
    cNullMenuItem TitleItem;              // 0x130 [O]; [R] ctor ("Plants Menu")
    cSpaceMenuItem SpaceItem;             // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;              // 0x164 [O]; [R] ctor (0, "Enable", &cPlants_m_Enable, 0..2)
    cFloatMenuItem SwayAmplitudeItem;     // 0x194 [O]; [R] ctor (1, "Sway Amplitude", &cPlants_m_SwayAmplitude, 0..2)
    cFloatMenuItem SwayOffsetItem;        // 0x1C8 [O]; [R] ctor (2, "Sway Offset", &cPlants_m_SwayOffset, -5..5)
    cFloatMenuItem BranchAmplitudeItem;   // 0x1FC [O]; [R] ctor (3, "Branch Amplitude", &cPlants_m_BranchAmplitude, 0..2)
    cFloatMenuItem RotationAmplitudeItem; // 0x230 [O]; [R] ctor (4, "Rotation Amplitude", &cPlants_m_RotationAmplitude, 0..2)
    cFloatMenuItem PositionScaleItem;     // 0x264 [O]; [R] ctor (5, "Position Scale", &cPlants_m_PositionScale, 0..1000)
    cIntMenuItem SwayFrequencyItem;       // 0x298 [O]; [R] ctor (6, "Sway Frequency (x/600)Hz", &cPlants_m_SwayFrequency, 0..6000)
    cFloatMenuItem RotationFrequencyItem; // 0x2C8 [O]; [R] ctor (7, "Rotation Frequency", &cPlants_m_RotationFrequency, 0..6000)
    cFloatMenuItem BranchFrequencyItem;   // 0x2FC [O]; [R] ctor (8, "Branch Frequency (x/600)Hz", &cPlants_m_BranchFrequency, 0..6000)
    cFloatMenuItem BranchFrequency2Item;  // 0x330 [O]; [R] ctor (9, "Branch Frequency 2 (x/600)Hz", &cPlants_m_BranchFrequency2,
                                          //       0..6000)
    cFloatMenuItem SwayDirectionItem;     // 0x364 [O]; [R] ctor (10, "Sway Direction", &cPlants_m_SwayDirection, 0..360)
    cFloatMenuItem BranchDirectionItem;   // 0x398 [O]; [R] ctor (11, "Branch Direction", &cPlants_m_BranchDirection, 0..360)
};

CHECK_OFFSET(cPlantsMenu, NumItems, 0x000);
CHECK_OFFSET(cPlantsMenu, FocusItem, 0x004);
CHECK_OFFSET(cPlantsMenu, TopItem, 0x008);
CHECK_OFFSET(cPlantsMenu, Items, 0x00C);
CHECK_OFFSET(cPlantsMenu, MenuSys, 0x124);
CHECK_OFFSET(cPlantsMenu, DrawLayer, 0x128);
CHECK_OFFSET(cPlantsMenu, mVtbl, 0x12C);
CHECK_OFFSET(cPlantsMenu, TitleItem, 0x130);
CHECK_OFFSET(cPlantsMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cPlantsMenu, EnableItem, 0x164);
CHECK_OFFSET(cPlantsMenu, SwayAmplitudeItem, 0x194);
CHECK_OFFSET(cPlantsMenu, SwayOffsetItem, 0x1C8);
CHECK_OFFSET(cPlantsMenu, BranchAmplitudeItem, 0x1FC);
CHECK_OFFSET(cPlantsMenu, RotationAmplitudeItem, 0x230);
CHECK_OFFSET(cPlantsMenu, PositionScaleItem, 0x264);
CHECK_OFFSET(cPlantsMenu, SwayFrequencyItem, 0x298);
CHECK_OFFSET(cPlantsMenu, RotationFrequencyItem, 0x2C8);
CHECK_OFFSET(cPlantsMenu, BranchFrequencyItem, 0x2FC);
CHECK_OFFSET(cPlantsMenu, BranchFrequency2Item, 0x330);
CHECK_OFFSET(cPlantsMenu, SwayDirectionItem, 0x364);
CHECK_OFFSET(cPlantsMenu, BranchDirectionItem, 0x398);
CHECK_SIZE(cPlantsMenu, 0x3CC);

// cLightGlowMenu (0x734): the "Light Glow Menu" debug menu: a title, a spacer and 29 editing rows.
// Size: [R] the constructor's last row, cFloatMenuItem IntensityScaleItem, is at 0x700 and ends at 0x734; the static
// instance __LightGlowMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __PlantsMenu (+0x738); [O] size
// 0x734.
// Base cMenu. Vtable cLightGlowMenu_vtbl (0x0047FBD8); destructor cLightGlowMenu__cLightGlowMenu. EA source:
// main/visualeffectsmenu.cpp.
struct cLightGlowMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                        // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                       // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                         // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;                   // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus
                                         //       forward to cMenu's, which set/clear it
    int DrawLayer;                       // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                        // 0x12C [R] the constructor stores cLightGlowMenu_vtbl (0x0047FBD8)
    // cLightGlowMenu's rows (0x130-0x733)
    cNullMenuItem TitleItem;             // 0x130 [O]; [R] ctor ("Light Glow Menu")
    cSpaceMenuItem SpaceItem;            // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;             // 0x164 [O]; [R] ctor (0, "Enable", &cLightGlow_m_Enable, 0..1)
    cIntMenuItem OverrideWPItem;         // 0x194 [O]; [R] ctor (1, "Override World Painter", &cLightGlow_m_OverrideWP,
                                         //       0..1)
    cIntMenuItem PS2CaptureSizeItem;     // 0x1C4 [O]; [R] ctor (2, "PS2 Capture Size (2^x)", &cLightGlow_m_PS2CaptureSize, 4..8)
    cFloatMenuItem PS2MinIntensityItem;  // 0x1F4 [O]; [R] ctor (3, "PS2 Minimum Intensity Cutoff",
                                         //       &cLightGlow_m_PS2MinIntensity, 0..2)
    cFloatMenuItem PS2ModIntensityItem;  // 0x228 [O]; [R] ctor (4, "PS2 Post-Cutoff Scale",
                                         //       &cLightGlow_m_PS2ModIntensity, 0..2)
    cFloatMenuItem PS2CopyIntensityItem; // 0x25C [O]; [R] ctor (5, "PS2 Copy Intensity",
                                         //       &cLightGlow_m_PS2CopyIntensity, 0..8)
    cFloatMenuItem PS2FrameSrcScaleItem; // 0x290 [O]; [R] ctor (6, "PS2 Frame Source Intensity",
                                         //       &cLightGlow_m_PS2FrameSrcScale, 0..2)
    cFloatMenuItem PS2FrameDstScaleItem; // 0x2C4 [O]; [R] ctor (7, "PS2 Frame Blend Intensity",
                                         //       &cLightGlow_m_PS2FrameDstScale, 0..2)
    cFloatMenuItem PS2IntensityTex0Item; // 0x2F8 [O]; [R] ctor (8, "PS2 Blend Texture 0", &cLightGlow_m_PS2IntensityTex0, 0..2)
    cFloatMenuItem PS2IntensityTex1Item; // 0x32C [O]; [R] ctor (9, "PS2 Blend Texture 1", &cLightGlow_m_PS2IntensityTex1, 0..2)
    cFloatMenuItem PS2IntensityTex2Item; // 0x360 [O]; [R] ctor (10, "PS2 Blend Texture 2",
                                         //       &cLightGlow_m_PS2IntensityTex2, 0..2)
    cFloatMenuItem PS2IntensityTex3Item; // 0x394 [O]; [R] ctor (11, "PS2 Blend Texture 3",
                                         //       &cLightGlow_m_PS2IntensityTex3, 0..2)
    cFloatMenuItem PS2JitterTexItem;     // 0x3C8 [O]; [R] ctor (12, "PS2 Texture Jitter", &cLightGlow_m_PS2JitterTex, 0..8)
    cIntMenuItem PS2ShowTexturesItem;    // 0x3FC [O]; [R] ctor (13, "PS2 Show Textures", &cLightGlow_m_PS2ShowTextures, 0..1)
    cFloatMenuItem SizeItem;             // 0x42C [O]; [R] ctor (14, "Size", &cLightGlow_m_Size, 0..64)
    cFloatMenuItem CutoffItem;           // 0x460 [O]; [R] ctor (15, "Cutoff", &cLightGlow_m_Cutoff, 0..1)
    cFloatMenuItem ColourPowerItem;      // 0x494 [O]; [R] ctor (16, "Colour Power", &cLightGlow_m_ColourPower, 0..4)
    cFloatMenuItem ColourScaleItem;      // 0x4C8 [O]; [R] ctor (17, "Colour Scale", &cLightGlow_m_ColourScale, 0..64)
    cFloatMenuItem ColourAmbientItem;    // 0x4FC [O]; [R] ctor (18, "Colour Ambient", &cLightGlow_m_ColourAmbient, 0..16)
    cFloatMenuItem SizePowerItem;        // 0x530 [O]; [R] ctor (19, "Size Power", &cLightGlow_m_SizePower, 0..4)
    cFloatMenuItem SizeScaleItem;        // 0x564 [O]; [R] ctor (20, "Size Scale", &cLightGlow_m_SizeScale, 0..64)
    cFloatMenuItem SizeAmbientItem;      // 0x598 [O]; [R] ctor (21, "Size Ambient", &cLightGlow_m_SizeAmbient, 0..64)
    cFloatMenuItem FadeRateItem;         // 0x5CC [O]; [R] ctor (22, "Fade Rate", &cLightGlow_m_FadeRate, 0..1)
    cIntMenuItem ClearAlphaItem;         // 0x600 [O]; [R] ctor (23, "Background Alpha", &cLightGlow_m_ClearAlpha, 0..255)
    cFloatMenuItem IntensityRItem;       // 0x630 [O]; [R] ctor (24, "Intensity Red", &cLightGlow_m_IntensityR, 0..1)
    cFloatMenuItem IntensityGItem;       // 0x664 [O]; [R] ctor (25, "Intensity Green", &cLightGlow_m_IntensityG, 0..1)
    cFloatMenuItem IntensityBItem;       // 0x698 [O]; [R] ctor (26, "Intensity Blue", &cLightGlow_m_IntensityB, 0..1)
    cFloatMenuItem IntensityAItem;       // 0x6CC [O]; [R] ctor (27, "Intensity Alpha", &cLightGlow_m_IntensityA, 0..1)
    cFloatMenuItem IntensityScaleItem;   // 0x700 [O]; [R] ctor (28, "Intensity Scale", &cLightGlow_m_IntensityScale, 0..1)
};

CHECK_OFFSET(cLightGlowMenu, NumItems, 0x000);
CHECK_OFFSET(cLightGlowMenu, FocusItem, 0x004);
CHECK_OFFSET(cLightGlowMenu, TopItem, 0x008);
CHECK_OFFSET(cLightGlowMenu, Items, 0x00C);
CHECK_OFFSET(cLightGlowMenu, MenuSys, 0x124);
CHECK_OFFSET(cLightGlowMenu, DrawLayer, 0x128);
CHECK_OFFSET(cLightGlowMenu, mVtbl, 0x12C);
CHECK_OFFSET(cLightGlowMenu, TitleItem, 0x130);
CHECK_OFFSET(cLightGlowMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cLightGlowMenu, EnableItem, 0x164);
CHECK_OFFSET(cLightGlowMenu, OverrideWPItem, 0x194);
CHECK_OFFSET(cLightGlowMenu, PS2CaptureSizeItem, 0x1C4);
CHECK_OFFSET(cLightGlowMenu, PS2MinIntensityItem, 0x1F4);
CHECK_OFFSET(cLightGlowMenu, PS2ModIntensityItem, 0x228);
CHECK_OFFSET(cLightGlowMenu, PS2CopyIntensityItem, 0x25C);
CHECK_OFFSET(cLightGlowMenu, PS2FrameSrcScaleItem, 0x290);
CHECK_OFFSET(cLightGlowMenu, PS2FrameDstScaleItem, 0x2C4);
CHECK_OFFSET(cLightGlowMenu, PS2IntensityTex0Item, 0x2F8);
CHECK_OFFSET(cLightGlowMenu, PS2IntensityTex1Item, 0x32C);
CHECK_OFFSET(cLightGlowMenu, PS2IntensityTex2Item, 0x360);
CHECK_OFFSET(cLightGlowMenu, PS2IntensityTex3Item, 0x394);
CHECK_OFFSET(cLightGlowMenu, PS2JitterTexItem, 0x3C8);
CHECK_OFFSET(cLightGlowMenu, PS2ShowTexturesItem, 0x3FC);
CHECK_OFFSET(cLightGlowMenu, SizeItem, 0x42C);
CHECK_OFFSET(cLightGlowMenu, CutoffItem, 0x460);
CHECK_OFFSET(cLightGlowMenu, ColourPowerItem, 0x494);
CHECK_OFFSET(cLightGlowMenu, ColourScaleItem, 0x4C8);
CHECK_OFFSET(cLightGlowMenu, ColourAmbientItem, 0x4FC);
CHECK_OFFSET(cLightGlowMenu, SizePowerItem, 0x530);
CHECK_OFFSET(cLightGlowMenu, SizeScaleItem, 0x564);
CHECK_OFFSET(cLightGlowMenu, SizeAmbientItem, 0x598);
CHECK_OFFSET(cLightGlowMenu, FadeRateItem, 0x5CC);
CHECK_OFFSET(cLightGlowMenu, ClearAlphaItem, 0x600);
CHECK_OFFSET(cLightGlowMenu, IntensityRItem, 0x630);
CHECK_OFFSET(cLightGlowMenu, IntensityGItem, 0x664);
CHECK_OFFSET(cLightGlowMenu, IntensityBItem, 0x698);
CHECK_OFFSET(cLightGlowMenu, IntensityAItem, 0x6CC);
CHECK_OFFSET(cLightGlowMenu, IntensityScaleItem, 0x700);
CHECK_SIZE(cLightGlowMenu, 0x734);

// cDepthFogMenu (0x920): the "Depth Fog Menu" debug menu: a title, a spacer and 8 editing rows.
// Size: [R] the constructor's last row, cARGBMenuItem ColourItem, is at 0x2C4 and ends at 0x920; the static instance
// __DepthFogMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __HeightFogMenu (+0x920); [O] On Tour's class
// is 0x954, with PSPDensityScaleItem that retail does not have.
// Base cMenu. Vtable cDepthFogMenu_vtbl (0x0047FB80); destructor cDepthFogMenu__cDepthFogMenu. EA source:
// main/visualeffectsmenu.cpp.
struct cDepthFogMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                        // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                       // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                         // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;                   // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus
                                         //       forward to cMenu's, which set/clear it
    int DrawLayer;                       // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                        // 0x12C [R] the constructor stores cDepthFogMenu_vtbl (0x0047FB80)
    // cDepthFogMenu's rows (0x130-0x91F)
    cNullMenuItem TitleItem;             // 0x130 [O]; [R] ctor ("Depth Fog Menu")
    cSpaceMenuItem SpaceItem;            // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;             // 0x164 [O]; [R] ctor (0, "Enable", &cDepthFog_m_Enable, 0..1)
    cIntMenuItem OverrideWPItem;         // 0x194 [O]; [R] ctor (1, "Override World Painter", &cDepthFog_m_OverrideWP,
                                         //       0..1)
    cFloatMenuItem NearPlaneItem;        // 0x1C4 [O]; [R] ctor (2, "Near Plane", &cDepthFog_m_NearPlane, 1..100000)
    cFloatMenuItem FarPlaneItem;         // 0x1F8 [O]; [R] ctor (3, "Far Plane", &cDepthFog_m_FarPlane, 1..100000)
    cIntMenuItem ExponentItem;           // 0x22C [O]; [R] ctor (4, "Exponent", &cDepthFog_m_Exponent, 0..2)
    cFloatMenuItem DensityItem;          // 0x25C [O]; [R] ctor (5, "Density", &cDepthFog_m_Density, 0..100)
    cFloatMenuItem XBoxDensityScaleItem; // 0x290 [O]; [R] ctor (6, "XBox Skybox Density Scale",
                                         //       &cDepthFog_m_XBoxDensityScale, 0..1)
    cARGBMenuItem ColourItem;            // 0x2C4 [O] (On Tour 0x2F8; placed by the caption and On Tour's order); [R]
                                         //       ctor (7, "Colour", &cDepthFog_m_Colour); the dtor tears it down inline
};

CHECK_OFFSET(cDepthFogMenu, NumItems, 0x000);
CHECK_OFFSET(cDepthFogMenu, FocusItem, 0x004);
CHECK_OFFSET(cDepthFogMenu, TopItem, 0x008);
CHECK_OFFSET(cDepthFogMenu, Items, 0x00C);
CHECK_OFFSET(cDepthFogMenu, MenuSys, 0x124);
CHECK_OFFSET(cDepthFogMenu, DrawLayer, 0x128);
CHECK_OFFSET(cDepthFogMenu, mVtbl, 0x12C);
CHECK_OFFSET(cDepthFogMenu, TitleItem, 0x130);
CHECK_OFFSET(cDepthFogMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cDepthFogMenu, EnableItem, 0x164);
CHECK_OFFSET(cDepthFogMenu, OverrideWPItem, 0x194);
CHECK_OFFSET(cDepthFogMenu, NearPlaneItem, 0x1C4);
CHECK_OFFSET(cDepthFogMenu, FarPlaneItem, 0x1F8);
CHECK_OFFSET(cDepthFogMenu, ExponentItem, 0x22C);
CHECK_OFFSET(cDepthFogMenu, DensityItem, 0x25C);
CHECK_OFFSET(cDepthFogMenu, XBoxDensityScaleItem, 0x290);
CHECK_OFFSET(cDepthFogMenu, ColourItem, 0x2C4);
CHECK_SIZE(cDepthFogMenu, 0x920);

// cHeightFogMenu (0x1FC): the "Height Fog Menu" debug menu: a title, a spacer and 3 editing rows.
// Size: [R] the constructor's last row, cFloatMenuItem ThicknessItem, is at 0x1C8 and ends at 0x1FC; the static
// instance __HeightFogMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __DetailSystemMenu (+0x200); [O] size
// 0x1FC.
// Base cMenu. Vtable cHeightFogMenu_vtbl (0x0047FB28); destructor cHeightFogMenu__cHeightFogMenu (the static instance's
// destruction in __sti__visualeffectsmenu_cpp inlines it). EA source: main/visualeffectsmenu.cpp.
struct cHeightFogMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus forward
                                     //       to cMenu's, which set/clear it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                    // 0x12C [R] the constructor stores cHeightFogMenu_vtbl (0x0047FB28)
    // cHeightFogMenu's rows (0x130-0x1FB)
    cNullMenuItem TitleItem;         // 0x130 [O]; [R] ctor ("Height Fog Menu")
    cSpaceMenuItem SpaceItem;        // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;         // 0x164 [O]; [R] ctor (0, "Enable", &cHeightFog_m_Enable, 0..1)
    cFloatMenuItem StartHeightItem;  // 0x194 [O]; [R] ctor (1, "Start Height", &cHeightFog_m_StartHeight, -100000..100000)
    cFloatMenuItem ThicknessItem;    // 0x1C8 [O]; [R] ctor (2, "Thickness", &cHeightFog_m_Thickness, 0.0001..10000)
};

CHECK_OFFSET(cHeightFogMenu, NumItems, 0x000);
CHECK_OFFSET(cHeightFogMenu, FocusItem, 0x004);
CHECK_OFFSET(cHeightFogMenu, TopItem, 0x008);
CHECK_OFFSET(cHeightFogMenu, Items, 0x00C);
CHECK_OFFSET(cHeightFogMenu, MenuSys, 0x124);
CHECK_OFFSET(cHeightFogMenu, DrawLayer, 0x128);
CHECK_OFFSET(cHeightFogMenu, mVtbl, 0x12C);
CHECK_OFFSET(cHeightFogMenu, TitleItem, 0x130);
CHECK_OFFSET(cHeightFogMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cHeightFogMenu, EnableItem, 0x164);
CHECK_OFFSET(cHeightFogMenu, StartHeightItem, 0x194);
CHECK_OFFSET(cHeightFogMenu, ThicknessItem, 0x1C8);
CHECK_SIZE(cHeightFogMenu, 0x1FC);

// cDetailSystemMenu (0x368): the "Detail System Menu" debug menu: a title, a spacer and 10 editing rows.
// Size: [R] the constructor's last row, cFloatMenuItem XBoxMaxSizeItem, is at 0x334 and ends at 0x368; the static
// instance __DetailSystemMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __ScreenTintMenu (+0x368); [O] On
// Tour's class is 0x438, with PSPNumSparklesItem, PSPSizeItem, PSPMinSizeItem, PSPMaxSizeItem that retail does not
// have.
// Base cMenu. Vtable cDetailSystemMenu_vtbl (0x0047FAD0); destructor cDetailSystemMenu__cDetailSystemMenu (the static
// instance's destruction in __sti__visualeffectsmenu_cpp inlines it). EA source: main/visualeffectsmenu.cpp.
struct cDetailSystemMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                         // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                        // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                          // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                 // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;                    // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus
                                          //       forward to cMenu's, which set/clear it
    int DrawLayer;                        // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                         // 0x12C [R] the constructor stores cDetailSystemMenu_vtbl (0x0047FAD0)
    // cDetailSystemMenu's rows (0x130-0x367)
    cNullMenuItem TitleItem;              // 0x130 [O]; [R] ctor ("Detail System Menu")
    cSpaceMenuItem SpaceItem;             // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;              // 0x164 [O]; [R] ctor (0, "Enable", &cDetailSystem_m_Enable, 0..1)
    cFloatMenuItem SpinScaleItem;         // 0x194 [O]; [R] ctor (1, "Sparkle Spin Scale", &cDetailSystem_m_SpinScale, -1..1)
    cFloatMenuItem PS2SparkleCountItem;   // 0x1C8 [O]; [R] ctor (2, "PS2 Sparkle Count", &cDetailSystem_m_PS2SparkleCount, 0..20)
    cFloatMenuItem PS2SparkleSizeItem;    // 0x1FC [O]; [R] ctor (3, "PS2 Sparkle World Size", &cDetailSystem_m_PS2SparkleSize, 0..500)
    cFloatMenuItem PS2MinSparkleSizeItem; // 0x230 [O]; [R] ctor (4, "PS2 Min Sparkle Screen Size", &cDetailSystem_m_PS2MinSparkleSize, 0..100)
    cFloatMenuItem PS2MaxSparkleSizeItem; // 0x264 [O]; [R] ctor (5, "PS2 Max Sparkle Screen Size", &cDetailSystem_m_PS2MaxSparkleSize, 0..100)
    cFloatMenuItem XBoxNumSparklesItem;   // 0x298 [O]; [R] ctor (6, "XBox Sparkle Count", &cDetailSystem_m_XBoxNumSparkles, 1..300)
    cFloatMenuItem XBoxSizeItem;          // 0x2CC [O]; [R] ctor (7, "XBox - World Size", &cDetailSystem_m_XBoxSize, 0..500)
    cFloatMenuItem XBoxMinSizeItem;       // 0x300 [O]; [R] ctor (8, "XBox - Min Screen Size", &cDetailSystem_m_XBoxMinSize, 0..100)
    cFloatMenuItem XBoxMaxSizeItem;       // 0x334 [O]; [R] ctor (9, "XBox - Max Screen Size", &cDetailSystem_m_XBoxMaxSize, 0..100)
};

CHECK_OFFSET(cDetailSystemMenu, NumItems, 0x000);
CHECK_OFFSET(cDetailSystemMenu, FocusItem, 0x004);
CHECK_OFFSET(cDetailSystemMenu, TopItem, 0x008);
CHECK_OFFSET(cDetailSystemMenu, Items, 0x00C);
CHECK_OFFSET(cDetailSystemMenu, MenuSys, 0x124);
CHECK_OFFSET(cDetailSystemMenu, DrawLayer, 0x128);
CHECK_OFFSET(cDetailSystemMenu, mVtbl, 0x12C);
CHECK_OFFSET(cDetailSystemMenu, TitleItem, 0x130);
CHECK_OFFSET(cDetailSystemMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cDetailSystemMenu, EnableItem, 0x164);
CHECK_OFFSET(cDetailSystemMenu, SpinScaleItem, 0x194);
CHECK_OFFSET(cDetailSystemMenu, PS2SparkleCountItem, 0x1C8);
CHECK_OFFSET(cDetailSystemMenu, PS2SparkleSizeItem, 0x1FC);
CHECK_OFFSET(cDetailSystemMenu, PS2MinSparkleSizeItem, 0x230);
CHECK_OFFSET(cDetailSystemMenu, PS2MaxSparkleSizeItem, 0x264);
CHECK_OFFSET(cDetailSystemMenu, XBoxNumSparklesItem, 0x298);
CHECK_OFFSET(cDetailSystemMenu, XBoxSizeItem, 0x2CC);
CHECK_OFFSET(cDetailSystemMenu, XBoxMinSizeItem, 0x300);
CHECK_OFFSET(cDetailSystemMenu, XBoxMaxSizeItem, 0x334);
CHECK_SIZE(cDetailSystemMenu, 0x368);

// cShellRendererMenu (0x31C): the "Shell Renderer Menu" debug menu: a title, a spacer and 9 editing rows.
// Size: [R] the constructor's last row, cIntMenuItem MaxShellsItem, is at 0x2EC and ends at 0x31C; no static instance
// (no retail caller of the constructor); [O] size 0x31C.
// Base cMenu. Vtable cShellRendererMenu_vtbl (0x0047FA78); destructor cShellRendererMenu__cShellRendererMenu. EA
// source: main/visualeffectsmenu.cpp.
struct cShellRendererMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                       // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                      // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                        // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];               // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;                  // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus
                                        //       forward to cMenu's, which set/clear it
    int DrawLayer;                      // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                       // 0x12C [R] the constructor stores cShellRendererMenu_vtbl (0x0047FA78)
    // cShellRendererMenu's rows (0x130-0x31B)
    cNullMenuItem TitleItem;            // 0x130 [O]; [R] ctor ("Shell Renderer Menu")
    cSpaceMenuItem SpaceItem;           // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;            // 0x164 [O]; [R] ctor (0, "Enable", &D_004A46B8, 0..1)
    cIntMenuItem ModelTextureIdItem;    // 0x194 [O]; [R] ctor (1, "Model Texture Id", &D_004A46BC, 0..200)
    cIntMenuItem ShellTextureIdItem;    // 0x1C4 [O]; [R] ctor (2, "Shell Texture Id", &D_004A46C0, 0..200)
    cIntMenuItem NearPlaneItem;         // 0x1F4 [O]; [R] ctor (3, "Near Plane", &D_004A46C4, 1..10000)
    cIntMenuItem FarPlaneItem;          // 0x224 [O]; [R] ctor (4, "Far Plane", &D_004A46C8, 1..10000)
    cIntMenuItem DistScaleItem;         // 0x254 [O]; [R] ctor (5, "DistScale", &D_004A46CC, 1..100000)
    cFloatMenuItem DepthScaleItem;      // 0x284 [O]; [R] ctor (6, "Depth Scale", &D_004A46D0, 1e-05..2)
    cFloatMenuItem DepthBrightnessItem; // 0x2B8 [O]; [R] ctor (7, "Depth Brightness", &D_004A46D4, 0..1)
    cIntMenuItem MaxShellsItem;         // 0x2EC [O]; [R] ctor (8, "Max Shells", &D_004A46D8, 1..64)
};

CHECK_OFFSET(cShellRendererMenu, NumItems, 0x000);
CHECK_OFFSET(cShellRendererMenu, FocusItem, 0x004);
CHECK_OFFSET(cShellRendererMenu, TopItem, 0x008);
CHECK_OFFSET(cShellRendererMenu, Items, 0x00C);
CHECK_OFFSET(cShellRendererMenu, MenuSys, 0x124);
CHECK_OFFSET(cShellRendererMenu, DrawLayer, 0x128);
CHECK_OFFSET(cShellRendererMenu, mVtbl, 0x12C);
CHECK_OFFSET(cShellRendererMenu, TitleItem, 0x130);
CHECK_OFFSET(cShellRendererMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cShellRendererMenu, EnableItem, 0x164);
CHECK_OFFSET(cShellRendererMenu, ModelTextureIdItem, 0x194);
CHECK_OFFSET(cShellRendererMenu, ShellTextureIdItem, 0x1C4);
CHECK_OFFSET(cShellRendererMenu, NearPlaneItem, 0x1F4);
CHECK_OFFSET(cShellRendererMenu, FarPlaneItem, 0x224);
CHECK_OFFSET(cShellRendererMenu, DistScaleItem, 0x254);
CHECK_OFFSET(cShellRendererMenu, DepthScaleItem, 0x284);
CHECK_OFFSET(cShellRendererMenu, DepthBrightnessItem, 0x2B8);
CHECK_OFFSET(cShellRendererMenu, MaxShellsItem, 0x2EC);
CHECK_SIZE(cShellRendererMenu, 0x31C);

// cScreenTintMenu (0x95C): the "Screen Tint Menu" debug menu: a title, a spacer and 40 editing rows.
// Size: [R] the constructor's last row, cFloatMenuItem GammaFillBItem, is at 0x928 and ends at 0x95C; the static
// instance __ScreenTintMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __ShadowMenu (+0x960); [O] size
// 0x95C.
// Base cMenu. Vtable cScreenTintMenu_vtbl (0x0047FA20); destructor cScreenTintMenu__cScreenTintMenu. EA source:
// main/visualeffectsmenu.cpp.
struct cScreenTintMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                         // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                        // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                          // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                 // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;                    // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus
                                          //       forward to cMenu's, which set/clear it
    int DrawLayer;                        // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                         // 0x12C [R] the constructor stores cScreenTintMenu_vtbl (0x0047FA20)
    // cScreenTintMenu's rows (0x130-0x95B)
    cNullMenuItem TitleItem;              // 0x130 [O]; [R] ctor ("Screen Tint Menu")
    cSpaceMenuItem SpaceItem;             // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;              // 0x164 [O]; [R] ctor (0, "Enable", &cScreenTint_m_Enable, 0..1)
    cIntMenuItem OverrideWPItem;          // 0x194 [O]; [R] ctor (1, "Override World Painter",
                                          //       &cScreenTint_m_OverrideWP, 0..1)
    cIntMenuItem DisplayValuesItem;       // 0x1C4 [O]; [R] ctor (2, "Display Values", &cScreenTint_m_DisplayValues,
                                          //       0..1)
    cFloatMenuItem ColourRItem;           // 0x1F4 [O]; [R] ctor (3, "Tint Red", &cScreenTint_m_ColourR, 0..2)
    cFloatMenuItem ColourGItem;           // 0x228 [O]; [R] ctor (4, "Tint Green", &cScreenTint_m_ColourG, 0..2)
    cFloatMenuItem ColourBItem;           // 0x25C [O]; [R] ctor (5, "Tint Blue", &cScreenTint_m_ColourB, 0..2)
    cFloatMenuItem FillRItem;             // 0x290 [O]; [R] ctor (6, "Fill Red", &cScreenTint_m_FillR, 0..1)
    cFloatMenuItem FillGItem;             // 0x2C4 [O]; [R] ctor (7, "Fill Green", &cScreenTint_m_FillG, 0..1)
    cFloatMenuItem FillBItem;             // 0x2F8 [O]; [R] ctor (8, "Fill Blue", &cScreenTint_m_FillB, 0..1)
    cFloatMenuItem XNColourRItem;         // 0x32C [O]; [R] ctor (9, "XN Tint Red", &cScreenTint_m_XNColourR, 0..2)
    cFloatMenuItem XNColourGItem;         // 0x360 [O]; [R] ctor (10, "XN Tint Green", &cScreenTint_m_XNColourG, 0..2)
    cFloatMenuItem XNColourBItem;         // 0x394 [O]; [R] ctor (11, "XN Tint Blue", &cScreenTint_m_XNColourB, 0..2)
    cFloatMenuItem XNLerpItem;            // 0x3C8 [O]; [R] ctor (12, "XN Lerp", &cScreenTint_m_XNLerp, 0..2)
    cIntMenuItem LightningEnableItem;     // 0x3FC [O]; [R] ctor (13, "Enable Lightning",
                                          //       &cScreenTint_m_LightningEnable, 0..1)
    cIntMenuItem FrameIdItem;             // 0x42C [O]; [R] ctor (14, "Frame Id", &cScreenTint_m_FrameId, -1..1000)
    cFloatMenuItem LightningChanceItem;   // 0x45C [O]; [R] ctor (15, "Lightning Chance",
                                          //       &cScreenTint_m_LightningChance, 0..1)
    cIntMenuItem LightningFadeInItem;     // 0x490 [O]; [R] ctor (16, "Flash Fade In", &cScreenTint_m_LightningFadeIn, 0..600)
    cIntMenuItem LightningFadeOutItem;    // 0x4C0 [O]; [R] ctor (17, "Flash Sustain", &cScreenTint_m_LightningFadeOut, 0..600)
    cIntMenuItem LightningFadeIn2Item;    // 0x4F0 [O]; [R] ctor (18, "Inverse Duration", &cScreenTint_m_LightningFadeIn2, 0..600)
    cIntMenuItem LightningFadeIn3Item;    // 0x520 [O]; [R] ctor (19, "Recover Fade In", &cScreenTint_m_LightningFadeIn3, 0..600)
    cFloatMenuItem LightningScaleItem;    // 0x550 [O]; [R] ctor (20, "Flash Scale", &cScreenTint_m_LightningScale, 0..2)
    cFloatMenuItem LightningScale2Item;   // 0x584 [O]; [R] ctor (21, "Inverse Scale", &cScreenTint_m_LightningScale2, 0..2)
    cFloatMenuItem LightningScale3Item;   // 0x5B8 [O]; [R] ctor (22, "Recover Scale", &cScreenTint_m_LightningScale3, 0..2)
    cFloatMenuItem LightningColourRItem;  // 0x5EC [O]; [R] ctor (23, "Flash Tint Red", &cScreenTint_m_LightningColourR, 0..2)
    cFloatMenuItem LightningColourGItem;  // 0x620 [O]; [R] ctor (24, "Flash Tint Green", &cScreenTint_m_LightningColourG, 0..2)
    cFloatMenuItem LightningColourBItem;  // 0x654 [O]; [R] ctor (25, "Flash Tint Blue", &cScreenTint_m_LightningColourB, 0..2)
    cFloatMenuItem LightningColour2RItem; // 0x688 [O]; [R] ctor (26, "Inverse Tint Red", &cScreenTint_m_LightningColour2R, 0..2)
    cFloatMenuItem LightningColour2GItem; // 0x6BC [O]; [R] ctor (27, "Inverse Tint Green", &cScreenTint_m_LightningColour2G, 0..2)
    cFloatMenuItem LightningColour2BItem; // 0x6F0 [O]; [R] ctor (28, "Inverse Tint Blue", &cScreenTint_m_LightningColour2B, 0..2)
    cFloatMenuItem LightningColour3RItem; // 0x724 [O]; [R] ctor (29, "Recover Tint Red", &cScreenTint_m_LightningColour3R, 0..2)
    cFloatMenuItem LightningColour3GItem; // 0x758 [O]; [R] ctor (30, "Recover Tint Green", &cScreenTint_m_LightningColour3G, 0..2)
    cFloatMenuItem LightningColour3BItem; // 0x78C [O]; [R] ctor (31, "Recover Tint Blue", &cScreenTint_m_LightningColour3B, 0..2)
    cIntMenuItem GammaEnableItem;         // 0x7C0 [O]; [R] ctor (32, "Gamma Enable", &cScreenTint_m_GammaEnable, 0..1)
    cFloatMenuItem GammaPowerItem;        // 0x7F0 [O]; [R] ctor (33, "Gamma Power", &cScreenTint_m_GammaPower, 0..2)
    cFloatMenuItem GammaColourRItem;      // 0x824 [O]; [R] ctor (34, "Gamma Tint Red", &cScreenTint_m_GammaColourR, 0..2)
    cFloatMenuItem GammaColourGItem;      // 0x858 [O]; [R] ctor (35, "Gamma Tint Green", &cScreenTint_m_GammaColourG, 0..2)
    cFloatMenuItem GammaColourBItem;      // 0x88C [O]; [R] ctor (36, "Gamma Tint Blue", &cScreenTint_m_GammaColourB, 0..2)
    cFloatMenuItem GammaFillRItem;        // 0x8C0 [O]; [R] ctor (37, "Gamma Fill Red", &cScreenTint_m_GammaFillR, 0..1)
    cFloatMenuItem GammaFillGItem;        // 0x8F4 [O]; [R] ctor (38, "Gamma Fill Green", &cScreenTint_m_GammaFillG, 0..1)
    cFloatMenuItem GammaFillBItem;        // 0x928 [O]; [R] ctor (39, "Gamma Fill Blue", &cScreenTint_m_GammaFillB, 0..1)
};

CHECK_OFFSET(cScreenTintMenu, NumItems, 0x000);
CHECK_OFFSET(cScreenTintMenu, FocusItem, 0x004);
CHECK_OFFSET(cScreenTintMenu, TopItem, 0x008);
CHECK_OFFSET(cScreenTintMenu, Items, 0x00C);
CHECK_OFFSET(cScreenTintMenu, MenuSys, 0x124);
CHECK_OFFSET(cScreenTintMenu, DrawLayer, 0x128);
CHECK_OFFSET(cScreenTintMenu, mVtbl, 0x12C);
CHECK_OFFSET(cScreenTintMenu, TitleItem, 0x130);
CHECK_OFFSET(cScreenTintMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cScreenTintMenu, EnableItem, 0x164);
CHECK_OFFSET(cScreenTintMenu, OverrideWPItem, 0x194);
CHECK_OFFSET(cScreenTintMenu, DisplayValuesItem, 0x1C4);
CHECK_OFFSET(cScreenTintMenu, ColourRItem, 0x1F4);
CHECK_OFFSET(cScreenTintMenu, ColourGItem, 0x228);
CHECK_OFFSET(cScreenTintMenu, ColourBItem, 0x25C);
CHECK_OFFSET(cScreenTintMenu, FillRItem, 0x290);
CHECK_OFFSET(cScreenTintMenu, FillGItem, 0x2C4);
CHECK_OFFSET(cScreenTintMenu, FillBItem, 0x2F8);
CHECK_OFFSET(cScreenTintMenu, XNColourRItem, 0x32C);
CHECK_OFFSET(cScreenTintMenu, XNColourGItem, 0x360);
CHECK_OFFSET(cScreenTintMenu, XNColourBItem, 0x394);
CHECK_OFFSET(cScreenTintMenu, XNLerpItem, 0x3C8);
CHECK_OFFSET(cScreenTintMenu, LightningEnableItem, 0x3FC);
CHECK_OFFSET(cScreenTintMenu, FrameIdItem, 0x42C);
CHECK_OFFSET(cScreenTintMenu, LightningChanceItem, 0x45C);
CHECK_OFFSET(cScreenTintMenu, LightningFadeInItem, 0x490);
CHECK_OFFSET(cScreenTintMenu, LightningFadeOutItem, 0x4C0);
CHECK_OFFSET(cScreenTintMenu, LightningFadeIn2Item, 0x4F0);
CHECK_OFFSET(cScreenTintMenu, LightningFadeIn3Item, 0x520);
CHECK_OFFSET(cScreenTintMenu, LightningScaleItem, 0x550);
CHECK_OFFSET(cScreenTintMenu, LightningScale2Item, 0x584);
CHECK_OFFSET(cScreenTintMenu, LightningScale3Item, 0x5B8);
CHECK_OFFSET(cScreenTintMenu, LightningColourRItem, 0x5EC);
CHECK_OFFSET(cScreenTintMenu, LightningColourGItem, 0x620);
CHECK_OFFSET(cScreenTintMenu, LightningColourBItem, 0x654);
CHECK_OFFSET(cScreenTintMenu, LightningColour2RItem, 0x688);
CHECK_OFFSET(cScreenTintMenu, LightningColour2GItem, 0x6BC);
CHECK_OFFSET(cScreenTintMenu, LightningColour2BItem, 0x6F0);
CHECK_OFFSET(cScreenTintMenu, LightningColour3RItem, 0x724);
CHECK_OFFSET(cScreenTintMenu, LightningColour3GItem, 0x758);
CHECK_OFFSET(cScreenTintMenu, LightningColour3BItem, 0x78C);
CHECK_OFFSET(cScreenTintMenu, GammaEnableItem, 0x7C0);
CHECK_OFFSET(cScreenTintMenu, GammaPowerItem, 0x7F0);
CHECK_OFFSET(cScreenTintMenu, GammaColourRItem, 0x824);
CHECK_OFFSET(cScreenTintMenu, GammaColourGItem, 0x858);
CHECK_OFFSET(cScreenTintMenu, GammaColourBItem, 0x88C);
CHECK_OFFSET(cScreenTintMenu, GammaFillRItem, 0x8C0);
CHECK_OFFSET(cScreenTintMenu, GammaFillGItem, 0x8F4);
CHECK_OFFSET(cScreenTintMenu, GammaFillBItem, 0x928);
CHECK_SIZE(cScreenTintMenu, 0x95C);

// cShadowMenu (0x330): the "Shadow Menu" debug menu: a title, a spacer and 9 editing rows.
// Size: [R] the constructor's last row, cFloatMenuItem EndUVItem, is at 0x2FC and ends at 0x330; the static instance
// __ShadowMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __SnowSurfaceMenu (+0x330); [O] size 0x330.
// Base cMenu. Vtable cShadowMenu_vtbl (0x0047F9C8); destructor cShadowMenu__cShadowMenu (the static instance's
// destruction in __sti__visualeffectsmenu_cpp inlines it). EA source: main/visualeffectsmenu.cpp.
struct cShadowMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                     // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                    // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                      // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];             // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;                // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus forward
                                      //       to cMenu's, which set/clear it
    int DrawLayer;                    // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                     // 0x12C [R] the constructor stores cShadowMenu_vtbl (0x0047F9C8)
    // cShadowMenu's rows (0x130-0x32F)
    cNullMenuItem TitleItem;          // 0x130 [O]; [R] ctor ("Shadow Menu")
    cSpaceMenuItem SpaceItem;         // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;          // 0x164 [O]; [R] ctor (0, "Enable", &cShadow_m_Enable, 0..1)
    cIntMenuItem ShowShadowPolysItem; // 0x194 [O]; [R] ctor (1, "Show Shadow Polys", &cShadow_m_ShowShadowPolys, 0..1)
    cFloatMenuItem ShadowDirXItem;    // 0x1C4 [O]; [R] ctor (2, "Shadow Dir X", &cShadow_m_ShadowDirX, -1..1)
    cFloatMenuItem ShadowDirYItem;    // 0x1F8 [O]; [R] ctor (3, "Shadow Dir Y", &cShadow_m_ShadowDirY, -1..1)
    cFloatMenuItem ShadowDirZItem;    // 0x22C [O]; [R] ctor (4, "Shadow Dir Z", &cShadow_m_ShadowDirZ, -1..1)
    cFloatMenuItem LengthItem;        // 0x260 [O]; [R] ctor (5, "Length", &cShadow_m_Length, 0.1..100000)
    cFloatMenuItem IntensityItem;     // 0x294 [O]; [R] ctor (6, "Intensity", &cShadow_m_Intensity, 0..1)
    cFloatMenuItem StartUVItem;       // 0x2C8 [O]; [R] ctor (7, "Start UV", &cShadow_m_StartUV, -64..64)
    cFloatMenuItem EndUVItem;         // 0x2FC [O]; [R] ctor (8, "End UV", &cShadow_m_EndUV, -64..64)
};

CHECK_OFFSET(cShadowMenu, NumItems, 0x000);
CHECK_OFFSET(cShadowMenu, FocusItem, 0x004);
CHECK_OFFSET(cShadowMenu, TopItem, 0x008);
CHECK_OFFSET(cShadowMenu, Items, 0x00C);
CHECK_OFFSET(cShadowMenu, MenuSys, 0x124);
CHECK_OFFSET(cShadowMenu, DrawLayer, 0x128);
CHECK_OFFSET(cShadowMenu, mVtbl, 0x12C);
CHECK_OFFSET(cShadowMenu, TitleItem, 0x130);
CHECK_OFFSET(cShadowMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cShadowMenu, EnableItem, 0x164);
CHECK_OFFSET(cShadowMenu, ShowShadowPolysItem, 0x194);
CHECK_OFFSET(cShadowMenu, ShadowDirXItem, 0x1C4);
CHECK_OFFSET(cShadowMenu, ShadowDirYItem, 0x1F8);
CHECK_OFFSET(cShadowMenu, ShadowDirZItem, 0x22C);
CHECK_OFFSET(cShadowMenu, LengthItem, 0x260);
CHECK_OFFSET(cShadowMenu, IntensityItem, 0x294);
CHECK_OFFSET(cShadowMenu, StartUVItem, 0x2C8);
CHECK_OFFSET(cShadowMenu, EndUVItem, 0x2FC);
CHECK_SIZE(cShadowMenu, 0x330);

// cSnowSurfaceMenu (0x2C8): the "Snow Surface Menu" debug menu: a title, a spacer and 7 editing rows.
// Size: [R] the constructor's last row, cFloatMenuItem MaxColourScaleItem, is at 0x294 and ends at 0x2C8; the static
// instance __SnowSurfaceMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __BoardTrailMenu (+0x2C8); [O] size
// 0x2C8.
// Base cMenu. Vtable cSnowSurfaceMenu_vtbl (0x0047F970); destructor cSnowSurfaceMenu__cSnowSurfaceMenu (the static
// instance's destruction in __sti__visualeffectsmenu_cpp inlines it). EA source: main/visualeffectsmenu.cpp.
struct cSnowSurfaceMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                        // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                       // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                         // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;                   // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus
                                         //       forward to cMenu's, which set/clear it
    int DrawLayer;                       // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                        // 0x12C [R] the constructor stores cSnowSurfaceMenu_vtbl (0x0047F970)
    // cSnowSurfaceMenu's rows (0x130-0x2C7)
    cNullMenuItem TitleItem;             // 0x130 [O]; [R] ctor ("Snow Surface Menu")
    cSpaceMenuItem SpaceItem;            // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;             // 0x164 [O]; [R] ctor (0, "Enable", &cSnowSurface_m_Enable, 0..1)
    cIntMenuItem EnablePatchReflectItem; // 0x194 [O]; [R] ctor (1, "Enable Patch Reflect", &cSnowSurface_m_EnablePatchReflect, 0..1)
    cFloatMenuItem BaseItem;             // 0x1C4 [O]; [R] ctor (2, "Base", &cSnowSurface_m_Base, 0..32)
    cFloatMenuItem BumpItem;             // 0x1F8 [O]; [R] ctor (3, "Bump", &cSnowSurface_m_Bump, 0..32)
    cFloatMenuItem NoiseItem;            // 0x22C [O]; [R] ctor (4, "Noise", &cSnowSurface_m_Noise, 0..32)
    cFloatMenuItem MinColourScaleItem;   // 0x260 [O]; [R] ctor (5, "Min Colour Add", &cSnowSurface_m_MinColourScale, 0..3)
    cFloatMenuItem MaxColourScaleItem;   // 0x294 [O]; [R] ctor (6, "Max Colour Scale", &cSnowSurface_m_MaxColourScale, 0..3)
};

CHECK_OFFSET(cSnowSurfaceMenu, NumItems, 0x000);
CHECK_OFFSET(cSnowSurfaceMenu, FocusItem, 0x004);
CHECK_OFFSET(cSnowSurfaceMenu, TopItem, 0x008);
CHECK_OFFSET(cSnowSurfaceMenu, Items, 0x00C);
CHECK_OFFSET(cSnowSurfaceMenu, MenuSys, 0x124);
CHECK_OFFSET(cSnowSurfaceMenu, DrawLayer, 0x128);
CHECK_OFFSET(cSnowSurfaceMenu, mVtbl, 0x12C);
CHECK_OFFSET(cSnowSurfaceMenu, TitleItem, 0x130);
CHECK_OFFSET(cSnowSurfaceMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cSnowSurfaceMenu, EnableItem, 0x164);
CHECK_OFFSET(cSnowSurfaceMenu, EnablePatchReflectItem, 0x194);
CHECK_OFFSET(cSnowSurfaceMenu, BaseItem, 0x1C4);
CHECK_OFFSET(cSnowSurfaceMenu, BumpItem, 0x1F8);
CHECK_OFFSET(cSnowSurfaceMenu, NoiseItem, 0x22C);
CHECK_OFFSET(cSnowSurfaceMenu, MinColourScaleItem, 0x260);
CHECK_OFFSET(cSnowSurfaceMenu, MaxColourScaleItem, 0x294);
CHECK_SIZE(cSnowSurfaceMenu, 0x2C8);

// cBoardTrailMenu (0x5CC): the "Board Trail Menu" debug menu: a title, a spacer and 22 editing rows.
// Size: [R] the constructor's last row, cIntMenuItem UseSliceCapsItem, is at 0x59C and ends at 0x5CC; the static
// instance __BoardTrailMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __FlagsMenu (+0x5D0); [O] On
// Tour's class is 0x704, with SkiInnerWidthScaleItem, SkiOuterWidthScaleItem, SkiHeightScaleItem,
// SkiDepth2HeightScaleItem, SkiWidthScaleItem, SkiMinWidthItem that retail does not have.
// Base cMenu. Vtable cBoardTrailMenu_vtbl (0x0047F918); destructor cBoardTrailMenu__cBoardTrailMenu. EA source:
// main/visualeffectsmenu.cpp.
struct cBoardTrailMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                            // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first
                                             //       call)
    int FocusItem;                           // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                             // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                    // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;                       // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus
                                             //       forward to cMenu's, which set/clear it
    int DrawLayer;                           // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                            // 0x12C [R] the constructor stores cBoardTrailMenu_vtbl (0x0047F918)
    // cBoardTrailMenu's rows (0x130-0x5CB)
    cNullMenuItem TitleItem;                 // 0x130 [O]; [R] ctor ("Board Trail Menu")
    cSpaceMenuItem SpaceItem;                // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;                 // 0x164 [O]; [R] ctor (0, "Enable", &cBoardTrail_m_Enable, 0..1)
    cFloatMenuItem InnerWidthScaleItem;      // 0x194 [O]; [R] ctor (1, "Inner Width Scale", &cBoardTrail_m_InnerWidthScale, -100..100)
    cFloatMenuItem OuterWidthScaleItem;      // 0x1C8 [O]; [R] ctor (2, "Outer Width Scale", &cBoardTrail_m_OuterWidthScale, -1000..1000)
    cFloatMenuItem HeightScaleItem;          // 0x1FC [O]; [R] ctor (3, "Height Scale", &cBoardTrail_m_HeightScale, -20..20)
    cFloatMenuItem Depth2HeightScaleItem;    // 0x230 [O]; [R] ctor (4, "Depth to Height Scale", &cBoardTrail_m_Depth2HeightScale, 0..1)
    cFloatMenuItem PackedDepthScaleItem;     // 0x264 [O] (On Tour 0x39C; placed by the caption and On Tour's order);
                                             //       [R] ctor (5, "Packed Depth Scale", &cBoardTrail_m_PackedDepthScale, -100..100)
    cFloatMenuItem LooseDepthScaleItem;      // 0x298 [O] (On Tour 0x3D0; placed by the caption and On Tour's order);
                                             //       [R] ctor (6, "Loose Depth Scale", &cBoardTrail_m_LooseDepthScale, -100..100)
    cFloatMenuItem PowderDepthScaleItem;     // 0x2CC [O] (On Tour 0x404; placed by the caption and On Tour's order);
                                             //       [R] ctor (7, "Powder Depth Scale", &cBoardTrail_m_PowderDepthScale, -1000..1000)
    cFloatMenuItem DeepPowderDepthScaleItem; // 0x300 [O] (On Tour 0x438; placed by the caption and On Tour's order);
                                             //       [R] ctor (8, "Deep Powder Depth Scale", &cBoardTrail_m_DeepPowderDepthScale, -1000..1000)
    cFloatMenuItem TopBiasFrontItem;         // 0x334 [O] (On Tour 0x46C; placed by the caption and On Tour's order);
                                             //       [R] ctor (9, "Top Bias Front", &cBoardTrail_m_TopBiasFront, -1000..1000)
    cFloatMenuItem TopBiasBehindItem;        // 0x368 [O] (On Tour 0x4A0; placed by the caption and On Tour's order);
                                             //       [R] ctor (10, "Top Bias Behind", &cBoardTrail_m_TopBiasBehind, -1000..1000)
    cFloatMenuItem TopBiasClearItem;         // 0x39C [O] (On Tour 0x4D4; placed by the caption and On Tour's order);
                                             //       [R] ctor (11, "Top Bias Clear", &cBoardTrail_m_TopBiasClear, -16777215..16777215)
    cFloatMenuItem InnerPositionJitterItem;  // 0x3D0 [O] (On Tour 0x508; placed by the caption and On Tour's order);
                                             //       [R] ctor (12, "Inner Position Jitter", &cBoardTrail_m_InnerPositionJitter, -10..10)
    cFloatMenuItem BaseUItem;                // 0x404 [O] (On Tour 0x53C; placed by the caption and On Tour's order);
                                             //       [R] ctor (13, "Base U", &cBoardTrail_m_BaseU, 0..1)
    cFloatMenuItem InnerUItem;               // 0x438 [O] (On Tour 0x570; placed by the caption and On Tour's order);
                                             //       [R] ctor (14, "Inner U", &cBoardTrail_m_InnerU, 0..1)
    cFloatMenuItem OuterUItem;               // 0x46C [O] (On Tour 0x5A4; placed by the caption and On Tour's order);
                                             //       [R] ctor (15, "Outer U", &cBoardTrail_m_OuterU, 0..1)
    cFloatMenuItem OuterNormalOffsetItem;    // 0x4A0 [O] (On Tour 0x5D8; placed by the caption and On Tour's order);
                                             //       [R] ctor (16, "Outer Normal Offset", &cBoardTrail_m_OuterNormalOffset, -10..10)
    cFloatMenuItem InnerNormalOffsetItem;    // 0x4D4 [O] (On Tour 0x60C; placed by the caption and On Tour's order);
                                             //       [R] ctor (17, "Inner Normal Offset", &cBoardTrail_m_InnerNormalOffset, -10..10)
    cFloatMenuItem BaseNormalOffsetItem;     // 0x508 [O] (On Tour 0x640; placed by the caption and On Tour's order);
                                             //       [R] ctor (18, "Base Normal Offset", &cBoardTrail_m_BaseNormalOffset, -10..10)
    cIntMenuItem NumFadeSegmentsItem;        // 0x53C [O] (On Tour 0x674; placed by the caption and On Tour's order);
                                             //       [R] ctor (19, "Num Fade Segments", &cBoardTrail_m_NumFadeSegments, 0..64)
    cIntMenuItem DrawFixupLayerItem;         // 0x56C [O] (On Tour 0x6A4; placed by the caption and On Tour's order);
                                             //       [R] ctor (20, "Draw Fixup Layer", &D_004A45CC, 0..1)
    cIntMenuItem UseSliceCapsItem;           // 0x59C [O] (On Tour 0x6D4; placed by the caption and On Tour's order);
                                             //       [R] ctor (21, "Use Slice Caps", &cBoardTrail_m_UseSliceCaps, 0..1)
};

CHECK_OFFSET(cBoardTrailMenu, NumItems, 0x000);
CHECK_OFFSET(cBoardTrailMenu, FocusItem, 0x004);
CHECK_OFFSET(cBoardTrailMenu, TopItem, 0x008);
CHECK_OFFSET(cBoardTrailMenu, Items, 0x00C);
CHECK_OFFSET(cBoardTrailMenu, MenuSys, 0x124);
CHECK_OFFSET(cBoardTrailMenu, DrawLayer, 0x128);
CHECK_OFFSET(cBoardTrailMenu, mVtbl, 0x12C);
CHECK_OFFSET(cBoardTrailMenu, TitleItem, 0x130);
CHECK_OFFSET(cBoardTrailMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cBoardTrailMenu, EnableItem, 0x164);
CHECK_OFFSET(cBoardTrailMenu, InnerWidthScaleItem, 0x194);
CHECK_OFFSET(cBoardTrailMenu, OuterWidthScaleItem, 0x1C8);
CHECK_OFFSET(cBoardTrailMenu, HeightScaleItem, 0x1FC);
CHECK_OFFSET(cBoardTrailMenu, Depth2HeightScaleItem, 0x230);
CHECK_OFFSET(cBoardTrailMenu, PackedDepthScaleItem, 0x264);
CHECK_OFFSET(cBoardTrailMenu, LooseDepthScaleItem, 0x298);
CHECK_OFFSET(cBoardTrailMenu, PowderDepthScaleItem, 0x2CC);
CHECK_OFFSET(cBoardTrailMenu, DeepPowderDepthScaleItem, 0x300);
CHECK_OFFSET(cBoardTrailMenu, TopBiasFrontItem, 0x334);
CHECK_OFFSET(cBoardTrailMenu, TopBiasBehindItem, 0x368);
CHECK_OFFSET(cBoardTrailMenu, TopBiasClearItem, 0x39C);
CHECK_OFFSET(cBoardTrailMenu, InnerPositionJitterItem, 0x3D0);
CHECK_OFFSET(cBoardTrailMenu, BaseUItem, 0x404);
CHECK_OFFSET(cBoardTrailMenu, InnerUItem, 0x438);
CHECK_OFFSET(cBoardTrailMenu, OuterUItem, 0x46C);
CHECK_OFFSET(cBoardTrailMenu, OuterNormalOffsetItem, 0x4A0);
CHECK_OFFSET(cBoardTrailMenu, InnerNormalOffsetItem, 0x4D4);
CHECK_OFFSET(cBoardTrailMenu, BaseNormalOffsetItem, 0x508);
CHECK_OFFSET(cBoardTrailMenu, NumFadeSegmentsItem, 0x53C);
CHECK_OFFSET(cBoardTrailMenu, DrawFixupLayerItem, 0x56C);
CHECK_OFFSET(cBoardTrailMenu, UseSliceCapsItem, 0x59C);
CHECK_SIZE(cBoardTrailMenu, 0x5CC);

// cFlagsMenu (0x1C4): the "Flags Menu" debug menu: a title, a spacer and 2 editing rows.
// Size: [R] the constructor's last row, cIntMenuItem EnableRenderItem, is at 0x194 and ends at 0x1C4; the static
// instance __FlagsMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, D_004CD840 (+0x1C8); [O] size
// 0x1C4.
// Base cMenu. Vtable cFlagsMenu_vtbl (0x0047F8C0); destructor cFlagsMenu__cFlagsMenu (the static instance's destruction
// in __sti__visualeffectsmenu_cpp inlines it). EA source: main/visualeffectsmenu.cpp.
struct cFlagsMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus forward
                                     //       to cMenu's, which set/clear it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                    // 0x12C [R] the constructor stores cFlagsMenu_vtbl (0x0047F8C0)
    // cFlagsMenu's rows (0x130-0x1C3)
    cNullMenuItem TitleItem;         // 0x130 [O]; [R] ctor ("Flags Menu")
    cSpaceMenuItem SpaceItem;        // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableUpdateItem;   // 0x164 [O]; [R] ctor (0, "EnableUpdate", &cFlags_m_EnableUpdate, 0..1)
    cIntMenuItem EnableRenderItem;   // 0x194 [O]; [R] ctor (1, "EnableRender", &cFlags_m_EnableRender, 0..1)
};

CHECK_OFFSET(cFlagsMenu, NumItems, 0x000);
CHECK_OFFSET(cFlagsMenu, FocusItem, 0x004);
CHECK_OFFSET(cFlagsMenu, TopItem, 0x008);
CHECK_OFFSET(cFlagsMenu, Items, 0x00C);
CHECK_OFFSET(cFlagsMenu, MenuSys, 0x124);
CHECK_OFFSET(cFlagsMenu, DrawLayer, 0x128);
CHECK_OFFSET(cFlagsMenu, mVtbl, 0x12C);
CHECK_OFFSET(cFlagsMenu, TitleItem, 0x130);
CHECK_OFFSET(cFlagsMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cFlagsMenu, EnableUpdateItem, 0x164);
CHECK_OFFSET(cFlagsMenu, EnableRenderItem, 0x194);
CHECK_SIZE(cFlagsMenu, 0x1C4);

// cPowerUpFXTogglesMenu (0x448): the "PowerUpFXToggles Menu" debug menu: a title, a spacer and 15 editing rows.
// Size: [R] the constructor's last row, cIntMenuItem TestOrangeRibbonItem, is at 0x418 and ends at 0x448; the static
// instance D_004CD840 (built by __sti__visualeffectsmenu_cpp) ends at the next object, D_004CDC88 (+0x448); [O] size
// 0x448.
// Base cMenu. Vtable cPowerUpFXTogglesMenu_vtbl (0x0047F868); destructor cPowerUpFXTogglesMenu__cPowerUpFXTogglesMenu.
// EA source: main/visualeffectsmenu.cpp.
struct cPowerUpFXTogglesMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                                        // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the
                                                         //       constructor's first call)
    int FocusItem;                                       // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                                         // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                                // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s
                                                         //       every row
    cMenuSys* MenuSys;                                   // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U]
                                                         //       onGainFocus/onLoseFocus forward to cMenu's, which
                                                         //       set/clear it
    int DrawLayer;                                       // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                                        // 0x12C [R] the constructor stores cPowerUpFXTogglesMenu_vtbl
                                                         //       (0x0047F868)
    // cPowerUpFXTogglesMenu's rows (0x130-0x447)
    cNullMenuItem TitleItem;                             // 0x130 [O]; [R] ctor ("PowerUpFXToggles Menu")
    cSpaceMenuItem SpaceItem;                            // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableSpeedBoostStreamersItem;          // 0x164 [O]; [R] ctor (0, "Enable Speed Boost Streamers",
                                                         //       &cPowerUpFXToggles_m_EnableSpeedBoostStreamers, 0..1)
    cIntMenuItem EnableSpeedBoostScreenBlurItem;         // 0x194 [O]; [R] ctor (1, "Enable Speed Boost Screen Blur",
                                                         //       &cPowerUpFXToggles_m_EnableSpeedBoostScreenBlur, 0..1)
    cFloatMenuItem SpeedBoostScreenBlurMaxPercentItem;   // 0x1C4 [O]; [R] ctor (2, "Max Screen Blur Percent",
                                                         //       &cPowerUpFXToggles_m_SpeedBoostScreenBlurMaxPercent,
                                                         //       0..1)
    cFloatMenuItem SpeedBoostScreenBlurIncreaseRateItem; // 0x1F8 [O]; [R] ctor (3, "Screen Blur Transition On Rate",
                                                         //       &cPowerUpFXToggles_m_SpeedBoostScreenBlurIncreaseRate,
                                                         //       0..60)
    cFloatMenuItem SpeedBoostScreenBlurDecreaseRateItem; // 0x22C [O]; [R] ctor (4, "Screen Blur Transition Off Rate",
                                                         //       &cPowerUpFXToggles_m_SpeedBoostScreenBlurDecreaseRate,
                                                         //       0..60)
    cFloatMenuItem SpeedBoostMinSpeedItem;               // 0x260 [O]; [R] ctor (5, "Screen Blur is not active at this
                                                         //       speed", &cPowerUpFXToggles_m_SpeedBoostMinSpeed,
                                                         //       0..120)
    cFloatMenuItem SpeedBoostMaxSpeedItem;               // 0x294 [O]; [R] ctor (6, "Screen Blur is full at this speed",
                                                         //       &cPowerUpFXToggles_m_SpeedBoostMaxSpeed, 0..120)
    cIntMenuItem MinSpeedBoostTimeBeforeBlurItem;        // 0x2C8 [O]; [R] ctor (7, "Speed Boost Time before Blur",
                                                         //       &cPowerUpFXToggles_m_MinSpeedBoostTimeBeforeBlur,
                                                         //       0..100)
    cIntMenuItem TestTrickBoostItem;                     // 0x2F8 [O]; [R] ctor (8, "Test Trick Boost",
                                                         //       &cPowerUpFXToggles_m_TestTrickBoost, 0..1)
    cIntMenuItem EnableSpeedBoostFXItem;                 // 0x328 [O]; [R] ctor (9, "Enable SpeedBoostFX",
                                                         //       &cPowerUpFXToggles_m_EnableSpeedBoostFX, 0..1)
    cIntMenuItem UberStateTestItem;                      // 0x358 [O]; [R] ctor (10, "Uber State Test",
                                                         //       &cPowerUpFXToggles_m_UberStateTest, 0..1)
    cIntMenuItem SuperUberStateTestItem;                 // 0x388 [O]; [R] ctor (11, "Super Uber State Test",
                                                         //       &cPowerUpFXToggles_m_SuperUberStateTest, 0..1)
    cIntMenuItem TestSpeedBoostRibbonItem;               // 0x3B8 [O]; [R] ctor (12, "Test Speed Boost Ribbon",
                                                         //       &cPowerUpFXToggles_m_TestSpeedBoostRibbon, 0..1)
    cIntMenuItem TestRedRibbonItem;                      // 0x3E8 [O]; [R] ctor (13, "Test Red Ribbon",
                                                         //       &cPowerUpFXToggles_m_TestRedRibbon, 0..1)
    cIntMenuItem TestOrangeRibbonItem;                   // 0x418 [O]; [R] ctor (14, "Test Orange Ribbon",
                                                         //       &cPowerUpFXToggles_m_TestOrangeRibbon, 0..1)
};

CHECK_OFFSET(cPowerUpFXTogglesMenu, NumItems, 0x000);
CHECK_OFFSET(cPowerUpFXTogglesMenu, FocusItem, 0x004);
CHECK_OFFSET(cPowerUpFXTogglesMenu, TopItem, 0x008);
CHECK_OFFSET(cPowerUpFXTogglesMenu, Items, 0x00C);
CHECK_OFFSET(cPowerUpFXTogglesMenu, MenuSys, 0x124);
CHECK_OFFSET(cPowerUpFXTogglesMenu, DrawLayer, 0x128);
CHECK_OFFSET(cPowerUpFXTogglesMenu, mVtbl, 0x12C);
CHECK_OFFSET(cPowerUpFXTogglesMenu, TitleItem, 0x130);
CHECK_OFFSET(cPowerUpFXTogglesMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cPowerUpFXTogglesMenu, EnableSpeedBoostStreamersItem, 0x164);
CHECK_OFFSET(cPowerUpFXTogglesMenu, EnableSpeedBoostScreenBlurItem, 0x194);
CHECK_OFFSET(cPowerUpFXTogglesMenu, SpeedBoostScreenBlurMaxPercentItem, 0x1C4);
CHECK_OFFSET(cPowerUpFXTogglesMenu, SpeedBoostScreenBlurIncreaseRateItem, 0x1F8);
CHECK_OFFSET(cPowerUpFXTogglesMenu, SpeedBoostScreenBlurDecreaseRateItem, 0x22C);
CHECK_OFFSET(cPowerUpFXTogglesMenu, SpeedBoostMinSpeedItem, 0x260);
CHECK_OFFSET(cPowerUpFXTogglesMenu, SpeedBoostMaxSpeedItem, 0x294);
CHECK_OFFSET(cPowerUpFXTogglesMenu, MinSpeedBoostTimeBeforeBlurItem, 0x2C8);
CHECK_OFFSET(cPowerUpFXTogglesMenu, TestTrickBoostItem, 0x2F8);
CHECK_OFFSET(cPowerUpFXTogglesMenu, EnableSpeedBoostFXItem, 0x328);
CHECK_OFFSET(cPowerUpFXTogglesMenu, UberStateTestItem, 0x358);
CHECK_OFFSET(cPowerUpFXTogglesMenu, SuperUberStateTestItem, 0x388);
CHECK_OFFSET(cPowerUpFXTogglesMenu, TestSpeedBoostRibbonItem, 0x3B8);
CHECK_OFFSET(cPowerUpFXTogglesMenu, TestRedRibbonItem, 0x3E8);
CHECK_OFFSET(cPowerUpFXTogglesMenu, TestOrangeRibbonItem, 0x418);
CHECK_SIZE(cPowerUpFXTogglesMenu, 0x448);

// cSnowfallTogglesMenu (0x101C): the "Snowfall Toggles" debug menu: a title, a spacer and 12 editing rows.
// Size: [R] the constructor's last row, cIntMenuItem TestAdditiveItem, is at 0xFEC and ends at 0x101C; the static
// instance D_004CDC88 (built by __sti__visualeffectsmenu_cpp) ends at the next object, __SplashTogglesMenu (+0x1020); [O] size
// 0x101C.
// Base cMenu. Vtable cSnowfallTogglesMenu_vtbl (0x0047F810); destructor cSnowfallTogglesMenu__cSnowfallTogglesMenu. EA
// source: main/visualeffectsmenu.cpp.
struct cSnowfallTogglesMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                              // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first
                                               //       call)
    int FocusItem;                             // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                               // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                      // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;                         // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus
                                               //       forward to cMenu's, which set/clear it
    int DrawLayer;                             // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                              // 0x12C [R] the constructor stores cSnowfallTogglesMenu_vtbl
                                               //       (0x0047F810)
    // cSnowfallTogglesMenu's rows (0x130-0x101B)
    cNullMenuItem TitleItem;                   // 0x130 [O]; [R] ctor ("Snowfall Toggles")
    cSpaceMenuItem SpaceItem;                  // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableTestItem;               // 0x164 [O]; [R] ctor (0, "Enable Test Values",
                                               //       &cSnowfallToggles_m_EnableTest, 0..1)
    cFloatMenuItem TestSnowIntensityItem;      // 0x194 [O]; [R] ctor (1, "Test Snowfall Intensity",
                                               //       &cSnowfallToggles_m_TestSnowIntensity, 0..4)
    cFloatMenuItem TestWindIntensityItem;      // 0x1C8 [O]; [R] ctor (2, "Test Snowfall Wind",
                                               //       &cSnowfallToggles_m_TestWindIntensity, 0..100)
    cFloatMenuItem TestFlakeSizeItem;          // 0x1FC [O]; [R] ctor (3, "Test Snowflake Size",
                                               //       &cSnowfallToggles_m_TestFlakeSize, 0.1..10)
    cFloatMenuItem TestWindRotationItem;       // 0x230 [O]; [R] ctor (4, "Test Wind Rotation",
                                               //       &cSnowfallToggles_m_TestWindRotation, 0..360)
    cFloatMenuItem TestSnowFlurriesItem;       // 0x264 [O]; [R] ctor (5, "Test Snow Flurries",
                                               //       &cSnowfallToggles_m_TestSnowFlurries, 0..1)
    cFloatMenuItem SnowGravityMultiplierItem;  // 0x298 [O]; [R] ctor (6, "Snow Gravity Multiplier",
                                               //       &cSnowfallToggles_m_SnowGravityMultiplier, -2..5)
    cFloatMenuItem SnowFluffItensityItem;      // 0x2CC [O]; [R] ctor (7, "Snow Fluff Intensity",
                                               //       &cSnowfallToggles_m_SnowFluffItensity, 0..1)
    cFloatMenuItem SnowFluffWindIntensityItem; // 0x300 [O]; [R] ctor (8, "Snow Fluff Wind Intensity",
                                               //       &cSnowfallToggles_m_SnowFluffWindIntensity, 0..100)
    cARGBMenuItem SnowFlakeColourItem;         // 0x334 [O]; [R] ctor (9, "Snow Flake Colour",
                                               //       &cSnowfallToggles_m_SnowFlakeColour); the dtor tears it down
                                               //       inline
    cARGBMenuItem SnowFluffColourItem;         // 0x990 [O]; [R] ctor (10, "Snow Fluff Colour",
                                               //       &cSnowfallToggles_m_SnowFluffColour); the dtor tears it down
                                               //       inline
    cIntMenuItem TestAdditiveItem;             // 0xFEC [O]; [R] ctor (11, "Test Additive Snowfall",
                                               //       &cSnowfallToggles_m_TestAdditive, 0..1)
};

CHECK_OFFSET(cSnowfallTogglesMenu, NumItems, 0x000);
CHECK_OFFSET(cSnowfallTogglesMenu, FocusItem, 0x004);
CHECK_OFFSET(cSnowfallTogglesMenu, TopItem, 0x008);
CHECK_OFFSET(cSnowfallTogglesMenu, Items, 0x00C);
CHECK_OFFSET(cSnowfallTogglesMenu, MenuSys, 0x124);
CHECK_OFFSET(cSnowfallTogglesMenu, DrawLayer, 0x128);
CHECK_OFFSET(cSnowfallTogglesMenu, mVtbl, 0x12C);
CHECK_OFFSET(cSnowfallTogglesMenu, TitleItem, 0x130);
CHECK_OFFSET(cSnowfallTogglesMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cSnowfallTogglesMenu, EnableTestItem, 0x164);
CHECK_OFFSET(cSnowfallTogglesMenu, TestSnowIntensityItem, 0x194);
CHECK_OFFSET(cSnowfallTogglesMenu, TestWindIntensityItem, 0x1C8);
CHECK_OFFSET(cSnowfallTogglesMenu, TestFlakeSizeItem, 0x1FC);
CHECK_OFFSET(cSnowfallTogglesMenu, TestWindRotationItem, 0x230);
CHECK_OFFSET(cSnowfallTogglesMenu, TestSnowFlurriesItem, 0x264);
CHECK_OFFSET(cSnowfallTogglesMenu, SnowGravityMultiplierItem, 0x298);
CHECK_OFFSET(cSnowfallTogglesMenu, SnowFluffItensityItem, 0x2CC);
CHECK_OFFSET(cSnowfallTogglesMenu, SnowFluffWindIntensityItem, 0x300);
CHECK_OFFSET(cSnowfallTogglesMenu, SnowFlakeColourItem, 0x334);
CHECK_OFFSET(cSnowfallTogglesMenu, SnowFluffColourItem, 0x990);
CHECK_OFFSET(cSnowfallTogglesMenu, TestAdditiveItem, 0xFEC);
CHECK_SIZE(cSnowfallTogglesMenu, 0x101C);

// cSplashTogglesMenu (0x2548): the "Camera Splash Menu" debug menu: a title, a spacer and 56 editing rows.
// Size: [R] the constructor's last row, cARGBMenuItem CrystalTintMaxItem, is at 0x1EEC and ends at 0x2548; the static
// instance __SplashTogglesMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __TubeMenu (+0x2548); [O] size
// 0x2548.
// Base cMenu. Vtable cSplashTogglesMenu_vtbl (0x0047F7B8); destructor cSplashTogglesMenu__cSplashTogglesMenu. EA
// source: main/visualeffectsmenu.cpp.
struct cSplashTogglesMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                                       // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's
                                                        //       first call)
    int FocusItem;                                      // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                                        // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                               // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s
                                                        //       every row
    cMenuSys* MenuSys;                                  // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U]
                                                        //       onGainFocus/onLoseFocus forward to cMenu's, which
                                                        //       set/clear it
    int DrawLayer;                                      // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                                       // 0x12C [R] the constructor stores cSplashTogglesMenu_vtbl
                                                        //       (0x0047F7B8)
    // cSplashTogglesMenu's rows (0x130-0x2547)
    cNullMenuItem TitleItem;                            // 0x130 [O]; [R] ctor ("Camera Splash Menu")
    cSpaceMenuItem SpaceItem;                           // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;                            // 0x164 [O]; [R] ctor (0, "Enable", &cSplashToggles_m_Enable, 0..1)
    cIntMenuItem RenderItem;                            // 0x194 [O]; [R] ctor (1, "Render", &cSplashToggles_m_Render, 0..1)
    cIntMenuItem NumDropsItem;                          // 0x1C4 [O]; [R] ctor (2, "Max Drops", &cSplashToggles_m_NumDrops, 1..30)
    cIntMenuItem NumIceCrystalsItem;                    // 0x1F4 [O]; [R] ctor (3, "Max Crystals", &cSplashToggles_m_NumIceCrystals, 1..30)
    cFloatMenuItem PercentageIceCrystalItem;            // 0x224 [O]; [R] ctor (4, "Percentage Ice Crystals",
                                                        //       &cSplashToggles_m_PercentageIceCrystal, 0..1)
    cFloatMenuItem PercentageIceCrystalSpawnChanceItem; // 0x258 [O]; [R] ctor (5, "Percentage Ice Crystal Spawn",
                                                        //       &cSplashToggles_m_PercentageIceCrystalSpawnChance, 0..1)
    cIntMenuItem IceCrystalMinGroupingItem;             // 0x28C [O]; [R] ctor (6, "Ice Crystal Minimum in Group",
                                                        //       &cSplashToggles_m_IceCrystalMinGrouping, 1..10)
    cIntMenuItem IceCrystalMaxGroupingItem;             // 0x2BC [O]; [R] ctor (7, "Ice Crystal Maximum in Group",
                                                        //       &cSplashToggles_m_IceCrystalMaxGrouping, 1..10)
    cIntMenuItem MaxSpawnPerCrystalItem;                // 0x2EC [O]; [R] ctor (8, "Max Spawn Per Crystal", &cSplashToggles_m_MaxSpawnPerCrystal,
                                                        //       1..10)
    cFloatMenuItem FarthestImpactDistanceItem;          // 0x31C [O]; [R] ctor (9, "Farthest Impact Distance",
                                                        //       &cSplashToggles_m_FarthestImpactDistance, 0..2000)
    cFloatMenuItem LowestImpactItensityItem;            // 0x350 [O]; [R] ctor (10, "Lowest Impact Intensity",
                                                        //       &cSplashToggles_m_LowestImpactItensity, 0..120)
    cFloatMenuItem MinimumSnowfallForImpactsItem;       // 0x384 [O]; [R] ctor (11, "Lowest Snowfall For Impacts",
                                                        //       &cSplashToggles_m_MinimumSnowfallForImpacts, 0..10)
    cFloatMenuItem ImpactMultiplierItem;                // 0x3B8 [O]; [R] ctor (12, "Impact Multiplier", &cSplashToggles_m_ImpactMultiplier,
                                                        //       0..10)
    cFloatMenuItem LowestSnowfallAmountItem;            // 0x3EC [O]; [R] ctor (13, "Lowest Snowfall Amount",
                                                        //       &cSplashToggles_m_LowestSnowfallAmount, 0..10)
    cFloatMenuItem SnowfallMultiplierItem;              // 0x420 [O]; [R] ctor (14, "Snowfall Multiplier", &cSplashToggles_m_SnowfallMultiplier,
                                                        //       0..10)
    cFloatMenuItem MinVelocityForPushoffItem;           // 0x454 [O]; [R] ctor (15, "Min Velocity For Pushoff",
                                                        //       &cSplashToggles_m_MinVelocityForPushoff, 0..10)
    cFloatMenuItem DropMinLifeSpanItem;                 // 0x488 [O]; [R] ctor (16, "Drop Min Life Span", &cSplashToggles_m_DropMinLifeSpan,
                                                        //       0..100)
    cFloatMenuItem DropMaxLifeSpanItem;                 // 0x4BC [O]; [R] ctor (17, "Drop Max Life Span", &cSplashToggles_m_DropMaxLifeSpan,
                                                        //       0..100)
    cFloatMenuItem DropMinSizeItem;                     // 0x4F0 [O]; [R] ctor (18, "Drop Min Size", &cSplashToggles_m_DropMinSize, 0..40)
    cFloatMenuItem DropMaxSizeItem;                     // 0x524 [O]; [R] ctor (19, "Drop Max Size", &cSplashToggles_m_DropMaxSize, 0..40)
    cFloatMenuItem DropMaxWidthHeightDifferenceItem;    // 0x558 [O]; [R] ctor (20, "Drop Max Width Height Difference",
                                                        //       &cSplashToggles_m_DropMaxWidthHeightDifference, 0..10)
    cFloatMenuItem DropMinVelocityJiggleXItem;          // 0x58C [O]; [R] ctor (21, "Drop Min Velocity Jiggle X",
                                                        //       &cSplashToggles_m_DropMinVelocityJiggleX, -10..10)
    cFloatMenuItem DropMaxVelocityJiggleXItem;          // 0x5C0 [O]; [R] ctor (22, "Drop Max Velocity Jiggle X",
                                                        //       &cSplashToggles_m_DropMaxVelocityJiggleX, -10..10)
    cFloatMenuItem DropMinVelocityJiggleYItem;          // 0x5F4 [O]; [R] ctor (23, "Drop Min Velocity Jiggle Y",
                                                        //       &cSplashToggles_m_DropMinVelocityJiggleY, -10..10)
    cFloatMenuItem DropMaxVelocityJiggleYItem;          // 0x628 [O]; [R] ctor (24, "Drop Max Velocity Jiggle Y",
                                                        //       &cSplashToggles_m_DropMaxVelocityJiggleY, -10..10)
    cFloatMenuItem DropMinSizeGrowthItem;               // 0x65C [O]; [R] ctor (25, "Drop Min Size Growth", &cSplashToggles_m_DropMinSizeGrowth,
                                                        //       -10..10)
    cFloatMenuItem DropMaxSizeGrowthItem;               // 0x690 [O]; [R] ctor (26, "Drop Max Size Growth", &cSplashToggles_m_DropMaxSizeGrowth,
                                                        //       -10..10)
    cFloatMenuItem DropMinPushOffSpeedItem;             // 0x6C4 [O]; [R] ctor (27, "Drop Min Push Off Speed",
                                                        //       &cSplashToggles_m_DropMinPushOffSpeed, 0..10)
    cFloatMenuItem DropMaxPushOffSpeedItem;             // 0x6F8 [O]; [R] ctor (28, "Drop Max Push Off Speed",
                                                        //       &cSplashToggles_m_DropMaxPushOffSpeed, 0..10)
    cFloatMenuItem DropMaxVelocityItem;                 // 0x72C [O]; [R] ctor (29, "Drop Maximum Velocity",
                                                        //       &cSplashToggles_m_DropMaxVelocity, 0..100)
    cFloatMenuItem DropMinVelocityUsageItem;            // 0x760 [O]; [R] ctor (30, "Drop Min Velocity Usage",
                                                        //       &cSplashToggles_m_DropMinVelocityUsage, 0..10)
    cFloatMenuItem DropMaxVelocityUsageItem;            // 0x794 [O]; [R] ctor (31, "Drop Max Velocity Usage",
                                                        //       &cSplashToggles_m_DropMaxVelocityUsage, 0..10)
    cFloatMenuItem DropMaxRotationChangeItem;           // 0x7C8 [O]; [R] ctor (32, "Crystal Max Rotation Overall
                                                        //       Change", &cSplashToggles_m_DropMaxRotationChange, 0..720)
    cARGBMenuItem DropTintMinItem;                      // 0x7FC [O]; [R] ctor (33, "Drop Tint Min", &D_005046E0); the
                                                        //       dtor tears it down inline
    cARGBMenuItem DropTintMaxItem;                      // 0xE58 [O]; [R] ctor (34, "Drop Tint Max", &D_005046F0); the
                                                        //       dtor tears it down inline
    cFloatMenuItem CrystalGroupSpacingXItem;            // 0x14B4 [O]; [R] ctor (35, "Crystal Group Spacing X",
                                                        //       &cSplashToggles_m_CrystalGroupSpacingX, 0..100)
    cFloatMenuItem CrystalGroupSpacingYItem;            // 0x14E8 [O]; [R] ctor (36, "Crystal Group Spacing Y",
                                                        //       &cSplashToggles_m_CrystalGroupSpacingY, 0..100)
    cFloatMenuItem CrystalMinLifeSpanItem;              // 0x151C [O]; [R] ctor (37, "Crystal Min Life Span",
                                                        //       &cSplashToggles_m_CrystalMinLifeSpan, 0..100)
    cFloatMenuItem CrystalMaxLifeSpanItem;              // 0x1550 [O]; [R] ctor (38, "Crystal Max Life Span",
                                                        //       &cSplashToggles_m_CrystalMaxLifeSpan, 0..100)
    cFloatMenuItem CrystalMinSizeItem;                  // 0x1584 [O]; [R] ctor (39, "Crystal Min Size", &cSplashToggles_m_CrystalMinSize,
                                                        //       0..40)
    cFloatMenuItem CrystalMaxSizeItem;                  // 0x15B8 [O]; [R] ctor (40, "Crystal Max Size", &cSplashToggles_m_CrystalMaxSize,
                                                        //       0..40)
    cFloatMenuItem CrystalMaxWidthHeightDifferenceItem; // 0x15EC [O]; [R] ctor (41, "Crystal Max Width Height
                                                        //       Difference", &cSplashToggles_m_CrystalMaxWidthHeightDifference, 0..10)
    cFloatMenuItem CrystalMinVelocityJiggleXItem;       // 0x1620 [O]; [R] ctor (42, "Crystal Min Velocity Jiggle X",
                                                        //       &cSplashToggles_m_CrystalMinVelocityJiggleX, -10..10)
    cFloatMenuItem CrystalMaxVelocityJiggleXItem;       // 0x1654 [O]; [R] ctor (43, "Crystal Max Velocity Jiggle X",
                                                        //       &cSplashToggles_m_CrystalMaxVelocityJiggleX, -10..10)
    cFloatMenuItem CrystalMinVelocityJiggleYItem;       // 0x1688 [O]; [R] ctor (44, "Crystal Min Velocity Jiggle Y",
                                                        //       &cSplashToggles_m_CrystalMinVelocityJiggleY, -10..10)
    cFloatMenuItem CrystalMaxVelocityJiggleYItem;       // 0x16BC [O]; [R] ctor (45, "Crystal Max Velocity Jiggle Y",
                                                        //       &cSplashToggles_m_CrystalMaxVelocityJiggleY, -10..10)
    cFloatMenuItem CrystalMinJiggleTimeItem;            // 0x16F0 [O]; [R] ctor (46, "Crsytal Min Time In One Jiggle
                                                        //       Direction", &cSplashToggles_m_CrystalMinJiggleTime, 0..10)
    cFloatMenuItem CrystalMaxJiggleTimeItem;            // 0x1724 [O]; [R] ctor (47, "Crsytal Max Time In One Jiggle
                                                        //       Direction", &cSplashToggles_m_CrystalMaxJiggleTime, 0..10)
    cFloatMenuItem CrystalMinSizeGrowthItem;            // 0x1758 [O]; [R] ctor (48, "Crystal Min Size Growth",
                                                        //       &cSplashToggles_m_CrystalMinSizeGrowth, -10..10)
    cFloatMenuItem CrystalMaxSizeGrowthItem;            // 0x178C [O]; [R] ctor (49, "Crystal Max Size Growth",
                                                        //       &cSplashToggles_m_CrystalMaxSizeGrowth, -10..10)
    cFloatMenuItem CrystalMinPushOffSpeedItem;          // 0x17C0 [O]; [R] ctor (50, "Crystal Min Push Off Speed",
                                                        //       &cSplashToggles_m_CrystalMinPushOffSpeed, 0..10)
    cFloatMenuItem CrystalMaxPushOffSpeedItem;          // 0x17F4 [O]; [R] ctor (51, "Crystal Max Push Off Speed",
                                                        //       &cSplashToggles_m_CrystalMaxPushOffSpeed, 0..10)
    cFloatMenuItem CrystalMaxVelocityItem;              // 0x1828 [O]; [R] ctor (52, "Crystal Maximum Velocity",
                                                        //       &cSplashToggles_m_CrystalMaxVelocity, 0..100)
    cFloatMenuItem CrystalMaxRotationChangeItem;        // 0x185C [O]; [R] ctor (53, "Crystal Max Rotation Overall
                                                        //       Change", &cSplashToggles_m_CrystalMaxRotationChange, 0..720)
    cARGBMenuItem CrystalTintMinItem;                   // 0x1890 [O]; [R] ctor (54, "Crystal Tint Min", &D_00504700);
                                                        //       the dtor tears it down inline
    cARGBMenuItem CrystalTintMaxItem;                   // 0x1EEC [O]; [R] ctor (55, "Crystal Tint Max", &D_00504710);
                                                        //       the dtor tears it down inline
};

CHECK_OFFSET(cSplashTogglesMenu, NumItems, 0x000);
CHECK_OFFSET(cSplashTogglesMenu, FocusItem, 0x004);
CHECK_OFFSET(cSplashTogglesMenu, TopItem, 0x008);
CHECK_OFFSET(cSplashTogglesMenu, Items, 0x00C);
CHECK_OFFSET(cSplashTogglesMenu, MenuSys, 0x124);
CHECK_OFFSET(cSplashTogglesMenu, DrawLayer, 0x128);
CHECK_OFFSET(cSplashTogglesMenu, mVtbl, 0x12C);
CHECK_OFFSET(cSplashTogglesMenu, TitleItem, 0x130);
CHECK_OFFSET(cSplashTogglesMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cSplashTogglesMenu, EnableItem, 0x164);
CHECK_OFFSET(cSplashTogglesMenu, RenderItem, 0x194);
CHECK_OFFSET(cSplashTogglesMenu, NumDropsItem, 0x1C4);
CHECK_OFFSET(cSplashTogglesMenu, NumIceCrystalsItem, 0x1F4);
CHECK_OFFSET(cSplashTogglesMenu, PercentageIceCrystalItem, 0x224);
CHECK_OFFSET(cSplashTogglesMenu, PercentageIceCrystalSpawnChanceItem, 0x258);
CHECK_OFFSET(cSplashTogglesMenu, IceCrystalMinGroupingItem, 0x28C);
CHECK_OFFSET(cSplashTogglesMenu, IceCrystalMaxGroupingItem, 0x2BC);
CHECK_OFFSET(cSplashTogglesMenu, MaxSpawnPerCrystalItem, 0x2EC);
CHECK_OFFSET(cSplashTogglesMenu, FarthestImpactDistanceItem, 0x31C);
CHECK_OFFSET(cSplashTogglesMenu, LowestImpactItensityItem, 0x350);
CHECK_OFFSET(cSplashTogglesMenu, MinimumSnowfallForImpactsItem, 0x384);
CHECK_OFFSET(cSplashTogglesMenu, ImpactMultiplierItem, 0x3B8);
CHECK_OFFSET(cSplashTogglesMenu, LowestSnowfallAmountItem, 0x3EC);
CHECK_OFFSET(cSplashTogglesMenu, SnowfallMultiplierItem, 0x420);
CHECK_OFFSET(cSplashTogglesMenu, MinVelocityForPushoffItem, 0x454);
CHECK_OFFSET(cSplashTogglesMenu, DropMinLifeSpanItem, 0x488);
CHECK_OFFSET(cSplashTogglesMenu, DropMaxLifeSpanItem, 0x4BC);
CHECK_OFFSET(cSplashTogglesMenu, DropMinSizeItem, 0x4F0);
CHECK_OFFSET(cSplashTogglesMenu, DropMaxSizeItem, 0x524);
CHECK_OFFSET(cSplashTogglesMenu, DropMaxWidthHeightDifferenceItem, 0x558);
CHECK_OFFSET(cSplashTogglesMenu, DropMinVelocityJiggleXItem, 0x58C);
CHECK_OFFSET(cSplashTogglesMenu, DropMaxVelocityJiggleXItem, 0x5C0);
CHECK_OFFSET(cSplashTogglesMenu, DropMinVelocityJiggleYItem, 0x5F4);
CHECK_OFFSET(cSplashTogglesMenu, DropMaxVelocityJiggleYItem, 0x628);
CHECK_OFFSET(cSplashTogglesMenu, DropMinSizeGrowthItem, 0x65C);
CHECK_OFFSET(cSplashTogglesMenu, DropMaxSizeGrowthItem, 0x690);
CHECK_OFFSET(cSplashTogglesMenu, DropMinPushOffSpeedItem, 0x6C4);
CHECK_OFFSET(cSplashTogglesMenu, DropMaxPushOffSpeedItem, 0x6F8);
CHECK_OFFSET(cSplashTogglesMenu, DropMaxVelocityItem, 0x72C);
CHECK_OFFSET(cSplashTogglesMenu, DropMinVelocityUsageItem, 0x760);
CHECK_OFFSET(cSplashTogglesMenu, DropMaxVelocityUsageItem, 0x794);
CHECK_OFFSET(cSplashTogglesMenu, DropMaxRotationChangeItem, 0x7C8);
CHECK_OFFSET(cSplashTogglesMenu, DropTintMinItem, 0x7FC);
CHECK_OFFSET(cSplashTogglesMenu, DropTintMaxItem, 0xE58);
CHECK_OFFSET(cSplashTogglesMenu, CrystalGroupSpacingXItem, 0x14B4);
CHECK_OFFSET(cSplashTogglesMenu, CrystalGroupSpacingYItem, 0x14E8);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMinLifeSpanItem, 0x151C);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMaxLifeSpanItem, 0x1550);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMinSizeItem, 0x1584);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMaxSizeItem, 0x15B8);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMaxWidthHeightDifferenceItem, 0x15EC);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMinVelocityJiggleXItem, 0x1620);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMaxVelocityJiggleXItem, 0x1654);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMinVelocityJiggleYItem, 0x1688);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMaxVelocityJiggleYItem, 0x16BC);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMinJiggleTimeItem, 0x16F0);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMaxJiggleTimeItem, 0x1724);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMinSizeGrowthItem, 0x1758);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMaxSizeGrowthItem, 0x178C);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMinPushOffSpeedItem, 0x17C0);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMaxPushOffSpeedItem, 0x17F4);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMaxVelocityItem, 0x1828);
CHECK_OFFSET(cSplashTogglesMenu, CrystalMaxRotationChangeItem, 0x185C);
CHECK_OFFSET(cSplashTogglesMenu, CrystalTintMinItem, 0x1890);
CHECK_OFFSET(cSplashTogglesMenu, CrystalTintMaxItem, 0x1EEC);
CHECK_SIZE(cSplashTogglesMenu, 0x2548);

// cTubeMenu (0x28C): the "Tube Menu" debug menu: a title, a spacer and 6 editing rows.
// Size: [R] the constructor's last row, cIntMenuItem BItem, is at 0x25C and ends at 0x28C; the static instance
// __TubeMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __PathArrowMenu (+0x290); [O] size 0x28C.
// Base cMenu. Vtable cTubeMenu_vtbl (0x0047F760); destructor cTubeMenu__cTubeMenu (the static instance's destruction in
// __sti__visualeffectsmenu_cpp inlines it). EA source: main/visualeffectsmenu.cpp.
struct cTubeMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus forward
                                     //       to cMenu's, which set/clear it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                    // 0x12C [R] the constructor stores cTubeMenu_vtbl (0x0047F760)
    // cTubeMenu's rows (0x130-0x28B)
    cNullMenuItem TitleItem;         // 0x130 [O]; [R] ctor ("Tube Menu")
    cSpaceMenuItem SpaceItem;        // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;         // 0x164 [O]; [R] ctor (0, "Enable", &cTube_m_Enable, 0..1)
    cFloatMenuItem ScaleWidthItem;   // 0x194 [O]; [R] ctor (1, "Scale Width", &cTube_m_ScaleWidth, 0..1000)
    cFloatMenuItem ScaleHeightItem;  // 0x1C8 [O]; [R] ctor (2, "Scale Height", &cTube_m_ScaleHeight, 0..1000)
    cIntMenuItem RItem;              // 0x1FC [O]; [R] ctor (3, "R", &cTube_m_R, 0..255)
    cIntMenuItem GItem;              // 0x22C [O]; [R] ctor (4, "G", &cTube_m_G, 0..255)
    cIntMenuItem BItem;              // 0x25C [O]; [R] ctor (5, "B", &cTube_m_B, 0..255)
};

CHECK_OFFSET(cTubeMenu, NumItems, 0x000);
CHECK_OFFSET(cTubeMenu, FocusItem, 0x004);
CHECK_OFFSET(cTubeMenu, TopItem, 0x008);
CHECK_OFFSET(cTubeMenu, Items, 0x00C);
CHECK_OFFSET(cTubeMenu, MenuSys, 0x124);
CHECK_OFFSET(cTubeMenu, DrawLayer, 0x128);
CHECK_OFFSET(cTubeMenu, mVtbl, 0x12C);
CHECK_OFFSET(cTubeMenu, TitleItem, 0x130);
CHECK_OFFSET(cTubeMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cTubeMenu, EnableItem, 0x164);
CHECK_OFFSET(cTubeMenu, ScaleWidthItem, 0x194);
CHECK_OFFSET(cTubeMenu, ScaleHeightItem, 0x1C8);
CHECK_OFFSET(cTubeMenu, RItem, 0x1FC);
CHECK_OFFSET(cTubeMenu, GItem, 0x22C);
CHECK_OFFSET(cTubeMenu, BItem, 0x25C);
CHECK_SIZE(cTubeMenu, 0x28C);

// cPathArrowMenu (0x488): the "PathArrow Menu" debug menu: a title, a spacer and 16 editing rows.
// Size: [R] the constructor's last row, cIntMenuItem MaxTickItem, is at 0x458 and ends at 0x488; the static instance
// __PathArrowMenu (built by __sti__visualeffectsmenu_cpp) ends at the next object, __AvalancheMenu (+0x808); [O] size 0x488.
// Base cMenu. Vtable cPathArrowMenu_vtbl (0x0047F708); destructor cPathArrowMenu__cPathArrowMenu. EA source:
// main/visualeffectsmenu.cpp.
struct cPathArrowMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                         // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                        // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                          // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                 // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s every row
    cMenuSys* MenuSys;                    // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus
                                          //       forward to cMenu's, which set/clear it
    int DrawLayer;                        // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                         // 0x12C [R] the constructor stores cPathArrowMenu_vtbl (0x0047F708)
    // cPathArrowMenu's rows (0x130-0x487)
    cNullMenuItem TitleItem;              // 0x130 [O]; [R] ctor ("PathArrow Menu")
    cSpaceMenuItem SpaceItem;             // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;              // 0x164 [O]; [R] ctor (0, "Enable", &cPathArrow_m_Enable, 0..1)
    cFloatMenuItem ScaleForwardWidthItem; // 0x194 [O]; [R] ctor (1, "Forward Width", &cPathArrow_m_ScaleForwardWidth, 0..1000)
    cFloatMenuItem ScaleMiddleWidthItem;  // 0x1C8 [O]; [R] ctor (2, "Middle Width", &cPathArrow_m_ScaleMiddleWidth, 0..1000)
    cFloatMenuItem ScaleRearWidthItem;    // 0x1FC [O]; [R] ctor (3, "Rear Width", &cPathArrow_m_ScaleRearWidth, 0..1000)
    cFloatMenuItem ScaleFowardLengthItem; // 0x230 [O]; [R] ctor (4, "Forward Length", &cPathArrow_m_ScaleFowardLength, 0..1000)
    cFloatMenuItem ScaleRearLengthItem;   // 0x264 [O]; [R] ctor (5, "Rear Length", &cPathArrow_m_ScaleRearLength, 0..1000)
    cFloatMenuItem ScaleHeightItem;       // 0x298 [O]; [R] ctor (6, "Height", &cPathArrow_m_ScaleHeight, 0..1000)
    cIntMenuItem RItem;                   // 0x2CC [O]; [R] ctor (7, "R", &cPathArrow_m_R, 0..255)
    cIntMenuItem GItem;                   // 0x2FC [O]; [R] ctor (8, "G", &cPathArrow_m_G, 0..255)
    cIntMenuItem BItem;                   // 0x32C [O]; [R] ctor (9, "B", &cPathArrow_m_B, 0..255)
    cIntMenuItem AItem;                   // 0x35C [O]; [R] ctor (10, "A", &cPathArrow_m_A, 0..255)
    cFloatMenuItem MoveSpeedItem;         // 0x38C [O]; [R] ctor (11, "Move Speed", &cPathArrow_m_MoveSpeed, 0..1000)
    cFloatMenuItem DrawSpeedItem;         // 0x3C0 [O]; [R] ctor (12, "Draw Speed", &cPathArrow_m_DrawSpeed, 0..1000)
    cFloatMenuItem DistanceItem;          // 0x3F4 [O]; [R] ctor (13, "Distance", &cPathArrow_m_Distance, 50..1000)
    cIntMenuItem PathIdItem;              // 0x428 [O]; [R] ctor (14, "Path Id", &cPathArrow_m_PathId, 0..1000)
    cIntMenuItem MaxTickItem;             // 0x458 [O]; [R] ctor (15, "Max Tick", &cPathArrow_m_MaxTick, 1..18000)
};

CHECK_OFFSET(cPathArrowMenu, NumItems, 0x000);
CHECK_OFFSET(cPathArrowMenu, FocusItem, 0x004);
CHECK_OFFSET(cPathArrowMenu, TopItem, 0x008);
CHECK_OFFSET(cPathArrowMenu, Items, 0x00C);
CHECK_OFFSET(cPathArrowMenu, MenuSys, 0x124);
CHECK_OFFSET(cPathArrowMenu, DrawLayer, 0x128);
CHECK_OFFSET(cPathArrowMenu, mVtbl, 0x12C);
CHECK_OFFSET(cPathArrowMenu, TitleItem, 0x130);
CHECK_OFFSET(cPathArrowMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cPathArrowMenu, EnableItem, 0x164);
CHECK_OFFSET(cPathArrowMenu, ScaleForwardWidthItem, 0x194);
CHECK_OFFSET(cPathArrowMenu, ScaleMiddleWidthItem, 0x1C8);
CHECK_OFFSET(cPathArrowMenu, ScaleRearWidthItem, 0x1FC);
CHECK_OFFSET(cPathArrowMenu, ScaleFowardLengthItem, 0x230);
CHECK_OFFSET(cPathArrowMenu, ScaleRearLengthItem, 0x264);
CHECK_OFFSET(cPathArrowMenu, ScaleHeightItem, 0x298);
CHECK_OFFSET(cPathArrowMenu, RItem, 0x2CC);
CHECK_OFFSET(cPathArrowMenu, GItem, 0x2FC);
CHECK_OFFSET(cPathArrowMenu, BItem, 0x32C);
CHECK_OFFSET(cPathArrowMenu, AItem, 0x35C);
CHECK_OFFSET(cPathArrowMenu, MoveSpeedItem, 0x38C);
CHECK_OFFSET(cPathArrowMenu, DrawSpeedItem, 0x3C0);
CHECK_OFFSET(cPathArrowMenu, DistanceItem, 0x3F4);
CHECK_OFFSET(cPathArrowMenu, PathIdItem, 0x428);
CHECK_OFFSET(cPathArrowMenu, MaxTickItem, 0x458);
CHECK_SIZE(cPathArrowMenu, 0x488);

// cVisualEffectsMainMenu (0x340): the "Visual Effects Main Menu" debug menu: a title, a spacer and one cSubMenuItem per
// effect menu (17, each opening one of the static menus below).
// Size: [R] the constructor's last row, cSubMenuItem PathArrowItem, is at 0x324 and ends at 0x340; no static instance
// (no retail caller of the constructor); [O] size 0x340.
// Base cMenu. Vtable cVisualEffectsMainMenu_vtbl (0x0047F6B0); destructor
// cVisualEffectsMainMenu__cVisualEffectsMainMenu. EA source: main/visualeffectsmenu.cpp.
struct cVisualEffectsMainMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                       // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                      // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                        // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];               // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s the sub-menu
                                        //       rows only (not the title and spacer)
    cMenuSys* MenuSys;                  // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus
                                        //       forward to cMenu's, which set/clear it
    int DrawLayer;                      // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                       // 0x12C [R] the constructor stores cVisualEffectsMainMenu_vtbl (0x0047F6B0)
    // cVisualEffectsMainMenu's rows (0x130-0x33F)
    cNullMenuItem TitleItem;            // 0x130 [O]; [R] ctor ("Visual Effects Main Menu"); not added to the menu
    cSpaceMenuItem SpaceItem;           // 0x14C [O]; [R] ctor (space 40); not added to the menu
    cSubMenuItem RenderTogglesItem;     // 0x164 [O]; [R] ctor ("Render Toggles", Sub = &__RenderTogglesMenu, the static
                                        //       cRenderTogglesMenu)
    cSubMenuItem VisualEffectsTestItem; // 0x180 [O]; [R] ctor ("Visual Effects Test", Sub = &__VisualEffectsTestMenu, the static
                                        //       cVisualEffectsTestMenu)
    cSubMenuItem LightGlowItem;         // 0x19C [O]; [R] ctor ("Light Glow", Sub = &__LightGlowMenu, the static
                                        //       cLightGlowMenu)
    cSubMenuItem PlantsItem;            // 0x1B8 [O]; [R] ctor ("Plants", Sub = &__PlantsMenu, the static cPlantsMenu)
    cSubMenuItem DepthFogItem;          // 0x1D4 [O]; [R] ctor ("Depth Fog", Sub = &__DepthFogMenu, the static
                                        //       cDepthFogMenu)
    cSubMenuItem HeightFogItem;         // 0x1F0 [O]; [R] ctor ("Height Fog", Sub = &__HeightFogMenu, the static
                                        //       cHeightFogMenu)
    cSubMenuItem DetailSystemItem;      // 0x20C [O]; [R] ctor ("Detail System", Sub = &__DetailSystemMenu, the static
                                        //       cDetailSystemMenu)
    cSubMenuItem ScreenTintItem;        // 0x228 [O]; [R] ctor ("Screen Tint", Sub = &__ScreenTintMenu, the static
                                        //       cScreenTintMenu)
    cSubMenuItem ShadowItem;            // 0x244 [O]; [R] ctor ("Shadow", Sub = &__ShadowMenu, the static cShadowMenu)
    cSubMenuItem SnowSurfaceItem;       // 0x260 [O]; [R] ctor ("Snow Surface", Sub = &__SnowSurfaceMenu, the static
                                        //       cSnowSurfaceMenu)
    cSubMenuItem BoardTrailItem;        // 0x27C [O]; [R] ctor ("Board Trail", Sub = &__BoardTrailMenu, the static
                                        //       cBoardTrailMenu)
    cSubMenuItem FlagsItem;             // 0x298 [O]; [R] ctor ("Flags", Sub = &__FlagsMenu, the static cFlagsMenu)
    cSubMenuItem PowerUpFXTogglesItem;  // 0x2B4 [O]; [R] ctor ("Power Up Effects Toggles", Sub = &D_004CD840, the
                                        //       static cPowerUpFXTogglesMenu)
    cSubMenuItem SnowfallTogglesItem;   // 0x2D0 [O]; [R] ctor ("Snowfall Test Effects", Sub = &D_004CDC88, the static
                                        //       cSnowfallTogglesMenu)
    cSubMenuItem SplashTogglesItem;     // 0x2EC [O]; [R] ctor ("Camera Snow Splash Effects", Sub = &__SplashTogglesMenu, the
                                        //       static cSplashTogglesMenu)
    cSubMenuItem TubeItem;              // 0x308 [O]; [R] ctor ("Tube Effects", Sub = &__TubeMenu, the static cTubeMenu)
    cSubMenuItem PathArrowItem;         // 0x324 [O]; [R] ctor ("Path Arrow Effects", Sub = &__PathArrowMenu, the static
                                        //       cPathArrowMenu)
};

CHECK_OFFSET(cVisualEffectsMainMenu, NumItems, 0x000);
CHECK_OFFSET(cVisualEffectsMainMenu, FocusItem, 0x004);
CHECK_OFFSET(cVisualEffectsMainMenu, TopItem, 0x008);
CHECK_OFFSET(cVisualEffectsMainMenu, Items, 0x00C);
CHECK_OFFSET(cVisualEffectsMainMenu, MenuSys, 0x124);
CHECK_OFFSET(cVisualEffectsMainMenu, DrawLayer, 0x128);
CHECK_OFFSET(cVisualEffectsMainMenu, mVtbl, 0x12C);
CHECK_OFFSET(cVisualEffectsMainMenu, TitleItem, 0x130);
CHECK_OFFSET(cVisualEffectsMainMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cVisualEffectsMainMenu, RenderTogglesItem, 0x164);
CHECK_OFFSET(cVisualEffectsMainMenu, VisualEffectsTestItem, 0x180);
CHECK_OFFSET(cVisualEffectsMainMenu, LightGlowItem, 0x19C);
CHECK_OFFSET(cVisualEffectsMainMenu, PlantsItem, 0x1B8);
CHECK_OFFSET(cVisualEffectsMainMenu, DepthFogItem, 0x1D4);
CHECK_OFFSET(cVisualEffectsMainMenu, HeightFogItem, 0x1F0);
CHECK_OFFSET(cVisualEffectsMainMenu, DetailSystemItem, 0x20C);
CHECK_OFFSET(cVisualEffectsMainMenu, ScreenTintItem, 0x228);
CHECK_OFFSET(cVisualEffectsMainMenu, ShadowItem, 0x244);
CHECK_OFFSET(cVisualEffectsMainMenu, SnowSurfaceItem, 0x260);
CHECK_OFFSET(cVisualEffectsMainMenu, BoardTrailItem, 0x27C);
CHECK_OFFSET(cVisualEffectsMainMenu, FlagsItem, 0x298);
CHECK_OFFSET(cVisualEffectsMainMenu, PowerUpFXTogglesItem, 0x2B4);
CHECK_OFFSET(cVisualEffectsMainMenu, SnowfallTogglesItem, 0x2D0);
CHECK_OFFSET(cVisualEffectsMainMenu, SplashTogglesItem, 0x2EC);
CHECK_OFFSET(cVisualEffectsMainMenu, TubeItem, 0x308);
CHECK_OFFSET(cVisualEffectsMainMenu, PathArrowItem, 0x324);
CHECK_SIZE(cVisualEffectsMainMenu, 0x340);

// cAvalancheMenu (0x7B0): the "Avalanche Menu" debug menu: a title, a spacer and 32 editing rows.
// Size: [R] the constructor's last row, cFloatMenuItem SeedItem, is at 0x77C and ends at 0x7B0; the static instance
// __AvalancheMenu (built by __sti__aivisualeffectsmenu_cpp) ends at the next object, memclass (+0x7B0); [O] size 0x7B0.
// Base cMenu. Vtable cAvalancheMenu_vtbl (0x00480040); destructor cAvalancheMenu__cAvalancheMenu. EA source:
// main/aivisualeffectsmenu.cpp.
struct cAvalancheMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                                   // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's
                                                    //       first call)
    int FocusItem;                                  // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                                    // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];                           // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s
                                                    //       every row
    cMenuSys* MenuSys;                              // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U]
                                                    //       onGainFocus/onLoseFocus forward to cMenu's, which set/clear
                                                    //       it
    int DrawLayer;                                  // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                                   // 0x12C [R] the constructor stores cAvalancheMenu_vtbl (0x00480040)
    // cAvalancheMenu's rows (0x130-0x7AF)
    cNullMenuItem TitleItem;                        // 0x130 [O]; [R] ctor ("Avalanche Menu")
    cSpaceMenuItem SpaceItem;                       // 0x14C [O]; [R] ctor (space 40)
    cIntMenuItem EnableItem;                        // 0x164 [O]; [R] ctor (0, "Enable", &cAvalanche_m_Enable, 0..1)
    cIntMenuItem RecalcItem;                        // 0x194 [O]; [R] ctor (1, "Recalc", &cAvalanche_m_Recalc, -1..1)
    cFloatMenuItem BrightnessItem;                  // 0x1C4 [O]; [R] ctor (2, "Brightness", &cAvalanche_m_Brightness, 0..4)
    cIntMenuItem UseLightMapColourItem;             // 0x1F8 [O]; [R] ctor (3, "Use Light Map Colour", &cAvalanche_m_UseLightMapColour,
                                                    //       0..1)
    cIntMenuItem UseEndColourItem;                  // 0x228 [O]; [R] ctor (4, "Use End Colour", &cAvalanche_m_UseEndColour, 0..1)
    cIntMenuItem SquareLerpItem;                    // 0x258 [O]; [R] ctor (5, "Square Lerp Value", &cAvalanche_m_SquareLerp, 0..1)
    cIntMenuItem SaveItem;                          // 0x288 [O]; [R] ctor (6, "Save", &cAvalanche_m_Save, 0..1)
    cIntMenuItem LoadItem;                          // 0x2B8 [O]; [R] ctor (7, "Load", &cAvalanche_m_Load, 0..1)
    cFloatMenuItem MinImpactVelocityItem;           // 0x2E8 [O]; [R] ctor (8, "Min Impact Velocity", &cAvalanche_m_MinImpactVelocity,
                                                    //       0..120)
    cIntMenuItem RenderEmittersItem;                // 0x31C [O]; [R] ctor (9, "Render Emitters", &cAvalanche_m_RenderEmitters, 0..1)
    cIntMenuItem RenderLinesItem;                   // 0x34C [O]; [R] ctor (10, "Render N Lines", &cAvalanche_m_RenderLines, 0..63)
    cIntMenuItem TriggerItem;                       // 0x37C [O]; [R] ctor (11, "Trigger Avalanche",
                                                    //       &cAvalanche_m_Trigger, -1..1)
    cIntMenuItem UpdateItem;                        // 0x3AC [O]; [R] ctor (12, "Update", &cAvalanche_m_Update, -1..1)
    cFloatMenuItem UpdateTimeStepItem;              // 0x3DC [O]; [R] ctor (13, "Update Time Step",
                                                    //       &cAvalanche_m_UpdateTimeStep, 0..1)
    cFloatMenuItem RotationalVelocityScaleItem;     // 0x410 [O]; [R] ctor (14, "Rotational Velocity Scale",
                                                    //       &cAvalanche_m_RotationalVelocityScale, 0..1000)
    cFloatMenuItem RandomBounceChanceItem;          // 0x444 [O]; [R] ctor (15, "Random Bounce Chance",
                                                    //       &cAvalanche_m_RandomBounceChance, 0..1)
    cFloatMenuItem RandomBounceVelocityScaleXYItem; // 0x478 [O]; [R] ctor (16, "Random Bounce Velocity Scale XY",
                                                    //       &cAvalanche_m_RandomBounceVelocityScaleXY, 0..1000)
    cFloatMenuItem RandomBounceVelocityScaleZItem;  // 0x4AC [O]; [R] ctor (17, "Random Bounce Velocity Scale Z",
                                                    //       &cAvalanche_m_RandomBounceVelocityScaleZ, 0..1000)
    cFloatMenuItem RandomBounceVelocityScaleItem;   // 0x4E0 [O]; [R] ctor (18, "Random Bounce Velocity Scale",
                                                    //       &cAvalanche_m_RandomBounceVelocityScale, 1..1000)
    cIntMenuItem AvalancheIdItem;                   // 0x514 [O]; [R] ctor (19, "Avalanche Id",
                                                    //       &cAvalanche_m_AvalancheId, 0..1000)
    cFloatMenuItem GravityItem;                     // 0x544 [O]; [R] ctor (20, "Gravity", &cAvalanche_m_Gravity,
                                                    //       0..127)
    cFloatMenuItem VelocityDampingItem;             // 0x578 [O]; [R] ctor (21, "Velocity Damping", &cAvalanche_m_VelocityDamping, 0..1)
    cFloatMenuItem NormalVelocityScaleItem;         // 0x5AC [O]; [R] ctor (22, "Normal Velocity Scale",
                                                    //       &cAvalanche_m_NormalVelocityScale, 0..1)
    cFloatMenuItem ForwardVelocityScaleItem;        // 0x5E0 [O]; [R] ctor (23, "Forward Velocity Scale", &cAvalanche_m_ForwardVelocityScale,
                                                    //       0..1)
    cFloatMenuItem NormalToForwardScaleItem;        // 0x614 [O]; [R] ctor (24, "Normal To Forward Scale", &cAvalanche_m_NormalToForwardScale,
                                                    //       0..1)
    cFloatMenuItem MaxVelocityItem;                 // 0x648 [O]; [R] ctor (25, "Max Velocity", &cAvalanche_m_MaxVelocity, 1..119)
    cFloatMenuItem DampingItem;                     // 0x67C [O]; [R] ctor (26, "Damping", &cAvalanche_m_Damping, 0..1)
    cIntMenuItem NumParticlesItem;                  // 0x6B0 [O]; [R] ctor (27, "Num Particles", &cAvalanche_m_NumParticles, 1..1024)
    cFloatMenuItem DefaultSizeItem;                 // 0x6E0 [O]; [R] ctor (28, "Default Size", &cAvalanche_m_DefaultSize, 1..1000)
    cFloatMenuItem PatchColourScaleItem;            // 0x714 [O]; [R] ctor (29, "Patch Colour Scale", &cAvalanche_m_PatchColourScale,
                                                    //       0..10)
    cFloatMenuItem PatchColourScale2Item;           // 0x748 [O]; [R] ctor (30, "Patch Colour Scale 2", &cAvalanche_m_PatchColourScale2,
                                                    //       0..10)
    cFloatMenuItem SeedItem;                        // 0x77C [O]; [R] ctor (31, "Seed", &cAvalanche_m_Seed, -1..1000)
};

CHECK_OFFSET(cAvalancheMenu, NumItems, 0x000);
CHECK_OFFSET(cAvalancheMenu, FocusItem, 0x004);
CHECK_OFFSET(cAvalancheMenu, TopItem, 0x008);
CHECK_OFFSET(cAvalancheMenu, Items, 0x00C);
CHECK_OFFSET(cAvalancheMenu, MenuSys, 0x124);
CHECK_OFFSET(cAvalancheMenu, DrawLayer, 0x128);
CHECK_OFFSET(cAvalancheMenu, mVtbl, 0x12C);
CHECK_OFFSET(cAvalancheMenu, TitleItem, 0x130);
CHECK_OFFSET(cAvalancheMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cAvalancheMenu, EnableItem, 0x164);
CHECK_OFFSET(cAvalancheMenu, RecalcItem, 0x194);
CHECK_OFFSET(cAvalancheMenu, BrightnessItem, 0x1C4);
CHECK_OFFSET(cAvalancheMenu, UseLightMapColourItem, 0x1F8);
CHECK_OFFSET(cAvalancheMenu, UseEndColourItem, 0x228);
CHECK_OFFSET(cAvalancheMenu, SquareLerpItem, 0x258);
CHECK_OFFSET(cAvalancheMenu, SaveItem, 0x288);
CHECK_OFFSET(cAvalancheMenu, LoadItem, 0x2B8);
CHECK_OFFSET(cAvalancheMenu, MinImpactVelocityItem, 0x2E8);
CHECK_OFFSET(cAvalancheMenu, RenderEmittersItem, 0x31C);
CHECK_OFFSET(cAvalancheMenu, RenderLinesItem, 0x34C);
CHECK_OFFSET(cAvalancheMenu, TriggerItem, 0x37C);
CHECK_OFFSET(cAvalancheMenu, UpdateItem, 0x3AC);
CHECK_OFFSET(cAvalancheMenu, UpdateTimeStepItem, 0x3DC);
CHECK_OFFSET(cAvalancheMenu, RotationalVelocityScaleItem, 0x410);
CHECK_OFFSET(cAvalancheMenu, RandomBounceChanceItem, 0x444);
CHECK_OFFSET(cAvalancheMenu, RandomBounceVelocityScaleXYItem, 0x478);
CHECK_OFFSET(cAvalancheMenu, RandomBounceVelocityScaleZItem, 0x4AC);
CHECK_OFFSET(cAvalancheMenu, RandomBounceVelocityScaleItem, 0x4E0);
CHECK_OFFSET(cAvalancheMenu, AvalancheIdItem, 0x514);
CHECK_OFFSET(cAvalancheMenu, GravityItem, 0x544);
CHECK_OFFSET(cAvalancheMenu, VelocityDampingItem, 0x578);
CHECK_OFFSET(cAvalancheMenu, NormalVelocityScaleItem, 0x5AC);
CHECK_OFFSET(cAvalancheMenu, ForwardVelocityScaleItem, 0x5E0);
CHECK_OFFSET(cAvalancheMenu, NormalToForwardScaleItem, 0x614);
CHECK_OFFSET(cAvalancheMenu, MaxVelocityItem, 0x648);
CHECK_OFFSET(cAvalancheMenu, DampingItem, 0x67C);
CHECK_OFFSET(cAvalancheMenu, NumParticlesItem, 0x6B0);
CHECK_OFFSET(cAvalancheMenu, DefaultSizeItem, 0x6E0);
CHECK_OFFSET(cAvalancheMenu, PatchColourScaleItem, 0x714);
CHECK_OFFSET(cAvalancheMenu, PatchColourScale2Item, 0x748);
CHECK_OFFSET(cAvalancheMenu, SeedItem, 0x77C);
CHECK_SIZE(cAvalancheMenu, 0x7B0);

// cAIVisualEffectsMainMenu (0x180): the "AIVisual Effects Main Menu" debug menu: a title, a spacer and one cSubMenuItem
// that opens the static cAvalancheMenu.
// Size: [R] the constructor's last row, cSubMenuItem AvalancheItem, is at 0x164 and ends at 0x180; no static instance
// (no retail caller of the constructor); [O] size 0x180.
// Base cMenu. Vtable cAIVisualEffectsMainMenu_vtbl (0x0047FFE8); destructor
// cAIVisualEffectsMainMenu__cAIVisualEffectsMainMenu. EA source: main/aivisualeffectsmenu.cpp.
struct cAIVisualEffectsMainMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu (the constructor's first call)
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] the constructor cMenu_addItem()s the sub-menu rows
                                     //       only (not the title and spacer)
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu; [U] onGainFocus/onLoseFocus forward
                                     //       to cMenu's, which set/clear it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                    // 0x12C [R] the constructor stores cAIVisualEffectsMainMenu_vtbl (0x0047FFE8)
    // cAIVisualEffectsMainMenu's rows (0x130-0x17F)
    cNullMenuItem TitleItem;         // 0x130 [O]; [R] ctor ("AIVisual Effects Main Menu"); not added to the menu
    cSpaceMenuItem SpaceItem;        // 0x14C [O]; [R] ctor (space 40); not added to the menu
    cSubMenuItem AvalancheItem;      // 0x164 [O]; [R] ctor ("Avalanche Effects", Sub = &__AvalancheMenu, the static
                                     //       cAvalancheMenu)
};

CHECK_OFFSET(cAIVisualEffectsMainMenu, NumItems, 0x000);
CHECK_OFFSET(cAIVisualEffectsMainMenu, FocusItem, 0x004);
CHECK_OFFSET(cAIVisualEffectsMainMenu, TopItem, 0x008);
CHECK_OFFSET(cAIVisualEffectsMainMenu, Items, 0x00C);
CHECK_OFFSET(cAIVisualEffectsMainMenu, MenuSys, 0x124);
CHECK_OFFSET(cAIVisualEffectsMainMenu, DrawLayer, 0x128);
CHECK_OFFSET(cAIVisualEffectsMainMenu, mVtbl, 0x12C);
CHECK_OFFSET(cAIVisualEffectsMainMenu, TitleItem, 0x130);
CHECK_OFFSET(cAIVisualEffectsMainMenu, SpaceItem, 0x14C);
CHECK_OFFSET(cAIVisualEffectsMainMenu, AvalancheItem, 0x164);
CHECK_SIZE(cAIVisualEffectsMainMenu, 0x180);

#endif /* MAIN_DEBUGMENU_H */
